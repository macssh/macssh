/* proxy.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Balázs Scheidler
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "proxy.h"

#include "channel_commands.h"
#include "command.h"
#include "connection.h"
#include "exception.h"
#include "format.h"
#include "gateway_channel.h"
#include "io_commands.h"
#include "proxy_session.h"
#include "proxy_userauth.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <arpa/inet.h>

#define CHAINED_CONNECTION (&chained_connection.super.super)

struct command_simple chained_connection;

#include "proxy.c.x" 

/* GABA:
   (class
     (name exc_chain_connections_handler)
     (super exception_handler)
     (vars
       (connection object ssh_connection)))
*/

static void 
do_exc_chain_connections_handler(struct exception_handler *c,
				 const struct exception *e)
{
  CAST(exc_chain_connections_handler, self, c);
  switch (e->type) 
    {
    case EXC_FINISH_READ:
    case EXC_FINISH_IO:
      {
	struct ssh_connection *chain = self->connection->chain;
	if (chain)
	  {
	    self->connection->chain->chain = NULL;
	    self->connection->chain = NULL; /* prevent raising this exception again */

	    /* FIXME: is it possible to pass the same exception to two handlers?
	     *
	     * It should be. /nisse
	     */
	    
	    EXCEPTION_RAISE(chain->e, e);
	  }
      }
    }
  EXCEPTION_RAISE(c->parent, e);  
}

static struct exception_handler *
make_exc_chain_connections_handler(struct ssh_connection *connection,
				   struct exception_handler *parent,
				   const char *context)
{
  NEW(exc_chain_connections_handler, self);

  self->super.parent = parent;
  self->super.raise = do_exc_chain_connections_handler;
  self->super.context = context;
  
  self->connection = connection;

  return &self->super;
}

/* GABA:
   (class
     (name chain_connections_continuation)
     (super command_continuation)
     (vars
       (connection object ssh_connection)
       (up object command_continuation)))
 */

static void
do_chain_connections_continuation(struct command_continuation *s,
				  struct lsh_object *x)
{
  CAST(chain_connections_continuation, self, s);
  CAST(ssh_connection, chained, x);

  self->connection->chain = chained;
  chained->chain = self->connection;

  /* FIXME: this a little bit kludgy here */
  self->connection->e = 
    make_exc_chain_connections_handler(self->connection,
				       self->connection->e, 
				       HANDLER_CONTEXT);
  chained->e =
    make_exc_chain_connections_handler(chained,
				       chained->e,
				       HANDLER_CONTEXT);

  COMMAND_RETURN(self->up, x);
}

static struct command_continuation *
make_chain_connections_continuation(struct ssh_connection *connection,
				    struct command_continuation *c)
{
  NEW(chain_connections_continuation, self);
  
  self->super.c = do_chain_connections_continuation;
  self->connection = connection;
  self->up = c;
  return &self->super;
}

/* GABA:
   (class
     (name chain_connections_client)
     (super command_frame)
     (vars
       (client_addr object listen_value)
       (client_callback object command)))
*/

static void
do_chain_connections_client(struct command_continuation *s,
			    struct lsh_object *x)
{
  CAST(chain_connections_client, closure, s);
  CAST(ssh_connection, server_connection, x);

  COMMAND_CALL(closure->client_callback,
	       &closure->client_addr->super,
	       make_chain_connections_continuation(server_connection, closure->super.up),
	       closure->super.e);
}

static struct command_continuation *
make_chain_connections_client(struct command *client_callback,
			      struct listen_value *client_addr,
			      struct command_continuation *c,
			      struct exception_handler *e)
{
  NEW(chain_connections_client, self);

  self->super.super.c = do_chain_connections_client;
  self->super.up = c;
  self->super.e = e;
  self->client_callback = client_callback;
  self->client_addr = client_addr;

  return &self->super.super;
}

/* GABA:
   (class
     (name chain_connections)
     (super command)
     (vars
       (server_callback object command)
       (client_callback object command)))
*/

static void
do_chain_connections(struct command *s,
		     struct lsh_object *x,
		     struct command_continuation *c,
		     struct exception_handler *e)
{
  CAST(chain_connections, self, s);
  CAST(listen_value, client_addr, x);

  COMMAND_CALL(self->server_callback, &client_addr->super, 
               make_chain_connections_client(self->client_callback, client_addr, c, e),
               e);
}

static struct command *
make_chain_connections(struct command *server_callback,
		       struct command *client_callback)
{
  NEW(chain_connections, self);

  self->super.call = do_chain_connections;
  self->server_callback = server_callback;
  self->client_callback = client_callback;
  return &self->super;
}

static struct lsh_object *
do_collect_chain_params(struct collect_info_2 *info UNUSED,
			struct lsh_object *a,
			struct lsh_object *b)
{
  CAST_SUBTYPE(command, server_callback, a);
  CAST_SUBTYPE(command, client_callback, b);

  return &make_chain_connections(server_callback, client_callback)->super;
}

struct collect_info_2 chain_connections_2 =
STATIC_COLLECT_2_FINAL(do_collect_chain_params);

struct collect_info_1 chain_connections =
STATIC_COLLECT_1(&chain_connections_2);

/* (proxy_connection_service user connection) -> connection */
/* GABA:
   (class
     (name proxy_connection_service)
     (super command)
     (vars
       ; hooks called on the client side (the server connection)
       (server_hooks object object_list)
       ; hooks called on the server side (the client connection)
       (client_hooks object object_list)))
*/

DEFINE_COMMAND_SIMPLE(chained_connection, a)
{
  CAST(ssh_connection, connection, a);
  return &connection->chain->super.super;
}

/* GABA:
   (expr
     (name make_call_hooks)
     (params
       (server_hooks object object_list)
       (client_hooks object object_list))
     (expr 
       (lambda (connection) 
         (progn 
           client_hooks 
           (chained_connection (progn server_hooks connection))))))
 */

static void
do_login(struct command *s,
	 struct lsh_object *x UNUSED,
	 struct command_continuation *c,
	 struct exception_handler *e UNUSED)
{
  CAST(proxy_connection_service, self, s);

  COMMAND_RETURN(c, make_call_hooks(self->server_hooks, self->client_hooks));
}

struct command *
make_proxy_connection_service(struct object_list *server_hooks,
			      struct object_list *client_hooks)
{
  NEW(proxy_connection_service, self);

  self->super.call = do_login;
  self->server_hooks = server_hooks;
  self->client_hooks = client_hooks;
  return &self->super;
}

/* GABA:
   (class
     (name proxy_accept_service_handler)
     (super packet_handler)
     (vars
       (name . int)
       (service object command)
       (c object command_continuation)
       (e object exception_handler)))
*/

static void
do_proxy_accept_service(struct packet_handler *c,
			struct ssh_connection *connection,
			struct lsh_string *packet)
{
  CAST(proxy_accept_service_handler, closure, c);

  struct simple_buffer buffer;
  UINT32 msg_number;
  int name;

  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_SERVICE_ACCEPT)
      && (
#if DATAFELLOWS_WORKAROUNDS
	  (connection->peer_flags & PEER_SERVICE_ACCEPT_KLUDGE)
#else
	  0
#endif
	  || (parse_atom(&buffer, &name)
	      && (name == closure->name)))
      && parse_eod(&buffer))
    {
      struct lsh_string *new_packet;

      connection->dispatch[SSH_MSG_SERVICE_ACCEPT] = &connection_fail_handler;
#if DATAFELLOWS_WORKAROUNDS
      if ((connection->chain->peer_flags & PEER_SERVICE_ACCEPT_KLUDGE) ==
	  (connection->peer_flags & PEER_SERVICE_ACCEPT_KLUDGE))
	new_packet = packet;
      else
	{
	  if (connection->chain->peer_flags & PEER_SERVICE_ACCEPT_KLUDGE)
	    new_packet = ssh_format("%c", SSH_MSG_SERVICE_ACCEPT);
	  else
	    new_packet = ssh_format("%c%a", SSH_MSG_SERVICE_ACCEPT, closure->name);
	  lsh_string_free(packet);
	}
#else
      new_packet = packet;
#endif

      C_WRITE(connection->chain, new_packet);
      COMMAND_CALL(closure->service,
		   connection->chain,
		   closure->c, closure->e);
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(closure->e, "Invalid SSH_MSG_SERVICE_ACCEPT message");
    }
}

static struct packet_handler *
make_proxy_accept_service_handler(UINT32 name,
				  struct command *service,
				  struct command_continuation *c,
				  struct exception_handler *e)
{
  NEW(proxy_accept_service_handler, self);

  self->super.handler = do_proxy_accept_service;
  self->name = name;
  self->service = service;
  self->c = c;
  self->e = e;
  return &self->super;
}

/* GABA:
   (class
     (name proxy_service_handler)
     (super packet_handler)
     (vars
       (services object alist)
       (c object command_continuation)
       (e object exception_handler)))
*/

static void
do_proxy_service_request(struct packet_handler *c,
			 struct ssh_connection *connection,
			 struct lsh_string *packet)
{
  CAST(proxy_service_handler, self, c);

  struct simple_buffer buffer;
  unsigned msg_number;
  int name;

  simple_buffer_init(&buffer, packet->length, packet->data);
  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_SERVICE_REQUEST)
      && parse_atom(&buffer, &name)
      && parse_eod(&buffer))
    {
      if (name)
	{
	  CAST_SUBTYPE(command, service, ALIST_GET(self->services, name));
	  if (service)
	    {
	      /* Don't accept any further service requests */
	      connection->dispatch[SSH_MSG_SERVICE_REQUEST]
		= &connection_fail_handler;

	      connection->chain->dispatch[SSH_MSG_SERVICE_ACCEPT]
		= make_proxy_accept_service_handler(name, service, self->c, self->e);

	      C_WRITE(connection->chain, packet);

	      return;
	    }
	}

      EXCEPTION_RAISE(connection->e,
		      make_protocol_exception(SSH_DISCONNECT_SERVICE_NOT_AVAILABLE, NULL));
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid SERVICE_REQUEST message");
    }

}

static struct packet_handler *
make_proxy_service_handler(struct alist *services,
			   struct command_continuation *c,
			   struct exception_handler *e)
{
  NEW(proxy_service_handler, self);

  self->super.handler = do_proxy_service_request;
  self->services = services;
  self->c = c;
  self->e = e;
  return &self->super;
}

/* GABA:
   (class
     (name proxy_offer_service)
     (super command)
     (vars
       (services object alist)))
*/

static void
do_proxy_offer_service(struct command *s,
		       struct lsh_object *x,
		       struct command_continuation *c,
		       struct exception_handler *e)
{
  CAST(proxy_offer_service, self, s);
  CAST(ssh_connection, connection, x);

  connection->dispatch[SSH_MSG_SERVICE_REQUEST]
    = make_proxy_service_handler(self->services, c, e);

#if 0
  /* currently servers may not ask for services in clients */
  connection->chain->dispatch[SSH_MSG_SERVICE_REQUEST]
    = make_proxy_service_request(self->server_services, c, e);
#endif
}

struct command *
make_proxy_offer_service(struct alist *services)
{
  NEW(proxy_offer_service, self);

  self->super.call = do_proxy_offer_service;
  self->services = services;
  return &self->super;
}


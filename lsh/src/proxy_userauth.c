/* proxy_userauth.c
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

#include "proxy_userauth.h"
#include "proxy.h"
#include "server_userauth.h"
#include "client_userauth.h"
#include "xalloc.h"
#include "ssh.h"
#include "lsh.h"
#include "werror.h"

#define GABA_DEFINE
#include "proxy_userauth.h.x"
#undef GABA_DEFINE

#include "proxy_userauth.c.x"

/* authenticated user is returned in this class */
static struct proxy_user *
make_proxy_user(struct lsh_string *name)
{
  NEW(proxy_user, self);
  self->name = name;
  return self;
}

/* GABA:
   (class
     (name proxy_userauth)
     (vars
       (proxy_auth method void "struct ssh_connection *"
                               "struct lsh_string *username"
			       "UINT32 service"
			       "struct simple_buffer *args")))
*/

#define PROXY_AUTH(u, c, n, s, a) ((u)->proxy_auth(u, c, n, s, a))

static void
do_forward_password_userauth(struct proxy_userauth *ignored UNUSED,
			     struct ssh_connection *connection,
			     struct lsh_string *username,
			     UINT32 service,
			     struct simple_buffer *args)
{
  struct lsh_string *password;
  int change_password;

  if (parse_boolean(args, &change_password) &&
      (password = parse_string_copy(args)) &&
      parse_eod(args))
    {
      C_WRITE(connection->chain, format_userauth_password(username, service, password, 1));
    }
}

struct proxy_userauth proxy_password_auth =
{ STATIC_HEADER, do_forward_password_userauth };

/* GABA:
   (class
     (name proxy_userauth_success)
     (super packet_handler)
     (vars
       (name string)
       (c object command_continuation)))
*/

static void
do_forward_success(struct packet_handler *c,
		   struct ssh_connection *connection,
		   struct lsh_string *packet)
{
  CAST(proxy_userauth_success, self, c);

  struct simple_buffer buffer;
  unsigned msg_number;

  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_USERAUTH_SUCCESS)
      && parse_eod(&buffer))
    {
      struct lsh_string *name = self->name;
      self->name = NULL;
      C_WRITE(connection->chain, packet);
      COMMAND_RETURN(self->c, make_proxy_user(name));
    }
  else
    {
      PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_USERAUTH_SUCCESS message.");
      lsh_string_free(packet);
    }
}


static struct packet_handler *
make_forward_success(struct lsh_string *name, 
		     struct command_continuation *c)
{
  NEW(proxy_userauth_success, self);
  self->super.handler = do_forward_success;
  self->name = name;
  self->c = c;
  return &self->super;
}

/* GABA:
   (class
     (name proxy_userauth_failure)
     (super packet_handler)
     (vars
       (e object exception_handler)))
*/

/* Arbitrary limit on list length */
#define USERAUTH_MAX_METHODS 47

static void
do_forward_failure(struct packet_handler *c,
		   struct ssh_connection *connection,
		   struct lsh_string *packet)
{
  CAST(proxy_userauth_failure, closure, c);
  struct simple_buffer buffer;
  unsigned msg_number;
  struct int_list *methods = NULL;
  int partial_success;

  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_USERAUTH_FAILURE)
      && ( (methods = parse_atom_list(&buffer, USERAUTH_MAX_METHODS)) )
      && parse_boolean(&buffer, &partial_success)
      && parse_eod(&buffer))
    {
      static const struct exception userauth_failed
	= STATIC_EXCEPTION(EXC_USERAUTH,
			   "Server authentication error.");

      lsh_string_free(packet);
      verbose("Authentication failure");
      
      EXCEPTION_RAISE(closure->e, &userauth_failed);

    }
  else
    {
      PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_USERAUTH_FAILURE message.");
      lsh_string_free(packet);
    }
}

static struct packet_handler *
make_forward_failure(struct exception_handler *e)
{
  NEW(proxy_userauth_failure, self);
  self->super.handler = do_forward_failure;
  self->e = e;
  return &self->super;
}

/* GABA:
   (class
     (name proxy_userauth_handler)
     (super packet_handler)
     (vars
       ; What to do after successful authentication
       (c object command_continuation)
       ; or failed.
       (e object exception_handler)
       
       ; Maps authentication methods to userath objects
       (methods object alist)

       ; Maps services to commands
       (services object alist)))
*/

static void
do_handle_userauth(struct packet_handler *c,
		   struct ssh_connection *connection,
		   struct lsh_string *packet)
{
  CAST(proxy_userauth_handler,  closure, c);
  struct simple_buffer buffer;

  unsigned msg_number;
  struct lsh_string *user;
  int requested_service;
  int method;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_USERAUTH_REQUEST)
      && ( (user = parse_string_copy(&buffer)) )
      && parse_atom(&buffer, &requested_service)
      && parse_atom(&buffer, &method))
    {
      CAST_SUBTYPE(proxy_userauth, auth, ALIST_GET(closure->methods, method));
      CAST_SUBTYPE(command, service,
		   ALIST_GET(closure->services, requested_service));

      connection_lock(connection);
      
      if (auth && service)
	{
	  connection->chain->dispatch[SSH_MSG_USERAUTH_FAILURE] = 
	    make_forward_failure(closure->e);
	  
	  connection->chain->dispatch[SSH_MSG_USERAUTH_SUCCESS] =
	    make_forward_success(user, make_delay_continuation(service, closure->c));

	  PROXY_AUTH(auth, connection, user, requested_service, &buffer);
	}
      else
	{
	  static const struct exception userauth_failed
	    = STATIC_EXCEPTION(EXC_USERAUTH,
			       "Unknown auth method or service.");

	  EXCEPTION_RAISE(closure->e, &userauth_failed);
	}
    }
  else
    PROTOCOL_ERROR(connection->e, "Invalid USERAUTH message.");

  lsh_string_free(packet);
}

static struct packet_handler *
make_proxy_userauth_handler(struct alist *methods,
			    struct alist *services,
			    struct command_continuation *c,
			    struct exception_handler *e)
{
  NEW(proxy_userauth_handler, auth);

  auth->super.handler = do_handle_userauth;
  auth->methods = methods;
  auth->services = services;
  auth->c = c;
  auth->e = e;

  return &auth->super;
}

/* chained into the returning of the authenticated user, ignores all
   authentication messages */
/* GABA:
   (class
     (name proxy_userauth_continuation)
     (super command_frame)
     (vars
       (connection object ssh_connection)))
*/

static void
do_proxy_userauth_continuation(struct command_continuation *c,
			       struct lsh_object *x)
{
  CAST(proxy_userauth_continuation, self, c);
  CAST(delayed_apply, action, x);
  int i;

  connection_unlock(self->connection);

  /* Ignore any further userauth messages. */
  for (i = SSH_FIRST_USERAUTH_GENERIC; i < SSH_FIRST_CONNECTION_GENERIC; i++) 
    self->connection->dispatch[i] = &connection_ignore_handler;
  
  FORCE_APPLY(action, self->super.up, self->super.e);

  /* FIXME: Possibly call connection_handle_pending. */
}

static struct command_continuation *
make_proxy_userauth_continuation(struct ssh_connection *connection,
				 struct command_continuation *c,
				 struct exception_handler *e)
{
  NEW(proxy_userauth_continuation, self);
  
  self->super.super.c = do_proxy_userauth_continuation;
  self->super.up = c;
  self->super.e = e;
  self->connection = connection;
  return &self->super.super;
}

#define AUTH_ATTEMPTS 20

/* Install an SSH_MSG_USERAUTH_REQUEST handler */
static void
do_userauth_proxy(struct command *s,
		  struct lsh_object *x, 
		  struct command_continuation *c,
		  struct exception_handler *e)
{
  CAST(userauth_service, self, s);
  CAST(ssh_connection, connection, x);

  connection->dispatch[SSH_MSG_USERAUTH_REQUEST] =
    make_proxy_userauth_handler(self->methods,
				self->services, 
				make_once_continuation
				(NULL, 
				 make_proxy_userauth_continuation
				 (connection, c, e)),
				make_exc_userauth_handler(connection, 
							  self->advertised_methods,
							  AUTH_ATTEMPTS, e,
							  HANDLER_CONTEXT));
}

struct command *
make_proxy_userauth(struct int_list *allowed_methods,
		    struct alist *methods,
		    struct alist *services)
{
  NEW(userauth_service, self);

  self->super.call = do_userauth_proxy;
  self->advertised_methods = allowed_methods;
  self->methods = methods;
  self->services = services;
  return &self->super;
}

/* tcpforward.c
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Balázs Scheidler, Niels Möller
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "tcpforward.h"

#include "channel_commands.h"
#include "channel_forward.h"
#include "format.h"
#include "io_commands.h"
#include "ssh.h"
#include "werror.h"

#include <assert.h>
#include <errno.h>
#include <string.h>


#define GABA_DEFINE
#include "tcpforward.h.x"
#undef GABA_DEFINE

#include "tcpforward.c.x"

/* Structures used to keep track of forwarded ports */

static struct local_port *
make_local_port(struct address_info *address, struct lsh_fd *socket)
{
  NEW(local_port, self);

  self->super.listen = address;  
  self->socket = socket;
  return self;
}

struct remote_port *
make_remote_port(struct address_info *listen,
		 struct command *callback)
{
  NEW(remote_port, self);

  self->super.listen = listen;  
  self->callback = callback;

  return self;
}

static struct forwarded_port *
lookup_forward(struct object_queue *q,
	       UINT32 length, const UINT8 *ip, UINT32 port)
{
  FOR_OBJECT_QUEUE(q, n)
    {
      CAST_SUBTYPE(forwarded_port, f, n);
      
      if ( (port == f->listen->port)
	   && lsh_string_eq_l(f->listen->ip, length, ip) )
	return f;
    }
  return NULL;
}

static struct local_port *
remove_forward(struct object_queue *q, int null_ok,
	       UINT32 length, const UINT8 *ip, UINT32 port)
{
  FOR_OBJECT_QUEUE(q, n)
    {
      CAST(local_port, f, n);
      
      if ( (port == f->super.listen->port)
	   && lsh_string_eq_l(f->super.listen->ip, length, ip) )
	{
	  if (null_ok || f->socket)
	    {
	      FOR_OBJECT_QUEUE_REMOVE(q, n);
	      return f;
	    }
	  else return NULL;
	}
    }
  return NULL;
}


/* Handle channel open requests */

/* Exception handler that promotes connect and dns errors to
 * CHANNEL_OPEN exceptions */

static void
do_exc_tcip_connect_handler(struct exception_handler *s,
			    const struct exception *e)
{
  switch(e->type)
    {
    case EXC_IO_CONNECT:
    case EXC_RESOLVE:
      EXCEPTION_RAISE(s->parent,
		      make_channel_open_exception(SSH_OPEN_CONNECT_FAILED,
						  e->msg));
      break;
    default:
      EXCEPTION_RAISE(s->parent, e);
    }
}

static struct exception_handler *
make_exc_tcpip_connect_handler(struct exception_handler *parent,
			       const char *context)
{
  return make_exception_handler(do_exc_tcip_connect_handler, parent, context);
}

/* GABA:
   (class
     (name open_forwarded_tcpip_continuation)
     (super command_continuation)
     (vars
       (up object command_continuation)
       (connection object ssh_connection)))
*/

/* NOTE: This continuation should not duplicate the work done by
 * channel_open_continuation. It must also not send any packets on the
 * channel, because it is not yet completely initialized. */
static void
do_open_forwarded_tcpip_continuation(struct command_continuation *s,
				     struct lsh_object *x)
{
  CAST(open_forwarded_tcpip_continuation, self, s);
  CAST(channel_forward, channel, x);

  assert(channel);

  channel_forward_start_io(channel);

  COMMAND_RETURN(self->up, channel);
}

static struct command_continuation *
make_open_forwarded_tcpip_continuation(struct ssh_connection *connection,
				       struct command_continuation *c)
{
  NEW(open_forwarded_tcpip_continuation, self);
  self->super.c = do_open_forwarded_tcpip_continuation;
  self->up = c;
  self->connection = connection;

  return &self->super;
}


/* GABA:
   (class
     (name channel_open_direct_tcpip)
     (super channel_open)
     (vars
       (callback object command)))
*/

static void
do_channel_open_direct_tcpip(struct channel_open *s,
			     struct ssh_connection *connection,
			     struct channel_open_info *info UNUSED,
			     struct simple_buffer *args,
			     struct command_continuation *c,
			     struct exception_handler *e)
{
  CAST(channel_open_direct_tcpip, closure, s);

  struct lsh_string *dest_host;
  UINT32 dest_port;
  const UINT8 *orig_host;
  UINT32 orig_host_length;
  UINT32 orig_port;
  
  if ( (dest_host = parse_string_copy(args))
       && parse_uint32(args, &dest_port) 
       && parse_string(args, &orig_host_length, &orig_host)
       && parse_uint32(args, &orig_port) 
       && parse_eod(args))
    {
      verbose("direct-tcpip connection attempt\n");

      COMMAND_CALL(closure->callback,
		   make_address_info(dest_host, dest_port),
		   make_open_forwarded_tcpip_continuation(connection, c), 
		   /* NOTE: This exception handler will be associated with the
		    * fd for its entire lifetime. */
		   make_exc_tcpip_connect_handler(e, HANDLER_CONTEXT));
    }
  else
    {
      lsh_string_free(dest_host);
      
      werror("do_channel_open_direct_tcpip: Invalid message!\n");
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_OPEN direct-tcp message.");
    }
}

struct channel_open *
make_channel_open_direct_tcpip(struct command *callback)
{
  NEW(channel_open_direct_tcpip, self);
  
  self->super.handler = do_channel_open_direct_tcpip;
  self->callback = callback;
  return &self->super;
}


/* Global requests for forwarding */

/* GABA:
   (class
     (name tcpip_forward_request_continuation)
     (super command_continuation)
     (vars
       (forward object local_port)
       (c object command_continuation)))
*/

static void
do_tcpip_forward_request_continuation(struct command_continuation *c,
				      struct lsh_object *x)
{
  CAST(tcpip_forward_request_continuation, self, c);
  CAST(lsh_fd, fd, x);

  assert(self->forward);
  assert(fd);

  self->forward->socket = fd;

  COMMAND_RETURN(self->c, &self->forward->super.super);
}

static struct command_continuation *
make_tcpip_forward_request_continuation(struct local_port *forward,
					struct command_continuation *c)
{
  NEW(tcpip_forward_request_continuation, self);

  self->forward = forward;
  self->c = c;
  
  self->super.c = do_tcpip_forward_request_continuation;

  return &self->super;
}

/* GABA:
   (class
     (name tcpip_forward_request_handler)
     (super exception_handler)
     (vars
       (connection object ssh_connection)
       (forward object local_port)))
*/

static void
do_tcpip_forward_request_exc(struct exception_handler *s,
			     const struct exception *e)
{
  CAST(tcpip_forward_request_handler, self, s);
  
  switch(e->type)
    {
    case EXC_IO_LISTEN:
    case EXC_RESOLVE:
      {
	struct local_port *port
	  = remove_forward(&self->connection->table->local_ports,
			   1,
			   self->forward->super.listen->ip->length,
			   self->forward->super.listen->ip->data,
			   self->forward->super.listen->port);
	assert(port);
	assert(port == self->forward);
	EXCEPTION_RAISE(s->parent,
			make_simple_exception(EXC_GLOBAL_REQUEST,
					      e->msg));
	break;
      }
    default:
      if (e->type & EXC_IO)
	{
	  werror("I/O error on forwarded connection: %z\n",
		 e->msg);
	}
      else
	EXCEPTION_RAISE(self->super.parent, e);
    }
}

static struct exception_handler *
make_tcpip_forward_request_exc(struct ssh_connection *connection,
			       struct local_port *forward,
			       struct exception_handler *parent,
			       const char *context)
{
  NEW(tcpip_forward_request_handler, self);
  self->super.raise = do_tcpip_forward_request_exc;
  self->super.parent = parent;
  self->super.context = context;

  self->connection = connection;
  self->forward = forward;

  return &self->super;
}

/* GABA:
   (class
     (name tcpip_forward_request)
     (super global_request)
     (vars
       ; The callback is invoked for each request, with the port as
       ; argument. If successful, it should return the fd object
       ; associated with the listening port. It need not remember the port;
       ; the continuation installed by do_tcpip_forward_request
       ; takes care of that.
       (callback object command)))
*/

static void
do_tcpip_forward_request(struct global_request *s, 
			 struct ssh_connection *connection,
			 UINT32 type UNUSED,
			 int want_reply UNUSED,
			 struct simple_buffer *args,
			 struct command_continuation *c,
			 struct exception_handler *e)
{
  CAST(tcpip_forward_request, self, s);
  struct lsh_string *bind_host;
  UINT32 bind_port;
  
  if ((bind_host = parse_string_copy(args)) 
      && parse_uint32(args, &bind_port) 
      && parse_eod(args))
    {
      struct address_info *a = make_address_info(bind_host, bind_port);
      struct local_port *forward;

      if (bind_port < 1024)
	{
	  werror("Denying forwarding of privileged port %i.\n", bind_port);
	  COMMAND_RETURN(c, NULL);
	  return;
	}

      if (lookup_forward(&connection->table->local_ports,
			 bind_host->length, bind_host->data, bind_port))
	{
	  static const struct exception again = 
	    STATIC_EXCEPTION(EXC_GLOBAL_REQUEST, "An already requested tcp-forward requested again");

	  verbose("An already requested tcp-forward requested again\n");
	  EXCEPTION_RAISE(e, &again);
	  return;
	}
      
      verbose("Adding forward-tcpip\n");
      forward = make_local_port(a, NULL);
      object_queue_add_head(&connection->table->local_ports,
			    &forward->super.super);

      {
	COMMAND_CALL(self->callback,
		     a,
		     make_tcpip_forward_request_continuation(forward, c),
		     make_tcpip_forward_request_exc(connection, forward,
						    e, HANDLER_CONTEXT));
	
	return;
      }
    }
  else
    {
      werror("Incorrectly formatted tcpip-forward request\n");
      PROTOCOL_ERROR(e, "Invalid tcpip-forward message.");
    }
}

struct global_request *make_tcpip_forward_request(struct command *callback)
{
  NEW(tcpip_forward_request, self);
  
  self->super.handler = do_tcpip_forward_request;
  self->callback = callback;
  
  return &self->super;
}

static void
do_tcpip_cancel_forward(struct global_request *s UNUSED, 
			struct ssh_connection *connection,
			UINT32 type UNUSED,
			int want_reply UNUSED,
			struct simple_buffer *args,
			struct command_continuation *c,
			struct exception_handler *e)
{
  UINT32 bind_host_length;
  const UINT8 *bind_host;
  UINT32 bind_port;
  
  if (parse_string(args, &bind_host_length, &bind_host) &&
      parse_uint32(args, &bind_port) &&
      parse_eod(args))
    {
      /* FIXME: Using null_ok == 0 is not quite right. If the
       * tcpip_forward_hook doesn't return immediately, and we receive
       * a cancel request before the forwarding is setup (which should
       * be ok, if the forwarding was requested with want_reply == 0),
       * cancelling fails and the client has to try again later. */

      struct local_port *port
	= remove_forward(&connection->table->local_ports, 0,
			 bind_host_length,
			 bind_host,
			 bind_port);

      if (port)
        {
	  assert(port->socket);
	  verbose("Cancelling a requested tcpip-forward.\n");

	  close_fd(port->socket);
	  port->socket = NULL;

	  COMMAND_RETURN(c, NULL);
	  return;
	}
      else
	{      
	  static const struct exception notfound = 
	    STATIC_EXCEPTION(EXC_GLOBAL_REQUEST, "Could not find tcpip-forward to cancel");
	  verbose("Could not find tcpip-forward to cancel\n");

	  EXCEPTION_RAISE(e, &notfound);
	  return;
	}
    }
  else
    {
      werror("Incorrectly formatted cancel-tcpip-forward request\n");
      PROTOCOL_ERROR(connection->e, "Invalid cancel-tcpip-forward message.");
    }
}

struct global_request tcpip_cancel_forward =
{ STATIC_HEADER, do_tcpip_cancel_forward }; 


/* Remote forwarding */

static void
do_channel_open_forwarded_tcpip(struct channel_open *s UNUSED,
				struct ssh_connection *connection,
				struct channel_open_info *info UNUSED,
				struct simple_buffer *args,
				struct command_continuation *c,
				struct exception_handler *e)
{
  UINT32 listen_ip_length;
  const UINT8 *listen_ip;
  UINT32 listen_port;
  struct lsh_string *peer_host = NULL;
  UINT32 peer_port;

  if (parse_string(args, &listen_ip_length, &listen_ip)
      && parse_uint32(args, &listen_port)
      && (peer_host = parse_string_copy(args))
      && parse_uint32(args, &peer_port)
      && parse_eod(args))
    {
      CAST(remote_port, port,
	   lookup_forward(&connection->table->remote_ports,
			  listen_ip_length, listen_ip, listen_port));
	   
      if (port && port->callback)
	{
	  COMMAND_CALL(port->callback,
		       make_address_info(peer_host, peer_port),
		       make_open_forwarded_tcpip_continuation(connection, c),
		       /* NOTE: This exception handler will be
			* associated with the fd for its entire
			* lifetime. */
		       make_exc_tcpip_connect_handler(e, HANDLER_CONTEXT));
	  return;
	}
      werror("Received a forwarded-tcpip request on a port for which we\n"
	     "haven't requested forwarding. Denying.\n");

      lsh_string_free(peer_host);
      EXCEPTION_RAISE(e,
		      make_channel_open_exception(SSH_OPEN_ADMINISTRATIVELY_PROHIBITED,
						  "Unexpected tcpip-forward request"));
      return;
    }
  else
    {
      werror("do_channel_open_forwarded_tcpip: Invalid message!\n");

      lsh_string_free(peer_host);
      PROTOCOL_ERROR(e, "Invalid tcpip-forward message");
    }
}

struct channel_open channel_open_forwarded_tcpip =
{ STATIC_HEADER, do_channel_open_forwarded_tcpip};

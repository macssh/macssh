/* tcpforward_commands.c
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

#include "tcpforward_commands.h"

#include "atoms.h"
#include "channel_commands.h"
#include "connection_commands.h"
#include "format.h"
#include "io_commands.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

/* Forward declarations */

extern struct collect_info_1 open_direct_tcpip;
extern struct collect_info_1 remote_listen_command;
extern struct collect_info_1 open_forwarded_tcpip;
extern struct command tcpip_start_io;
extern struct command tcpip_connect_io;

struct collect_info_1 install_direct_tcpip_handler;
struct collect_info_1 install_forwared_tcpip_handler;

static struct command make_direct_tcpip_handler;

struct collect_info_1 install_tcpip_forward_handler;
static struct command make_tcpip_forward_handler;

#define OPEN_DIRECT_TCPIP (&open_direct_tcpip.super.super.super)
#define REMOTE_LISTEN (&remote_listen_command.super.super.super)
#define TCPIP_START_IO (&tcpip_start_io.super)
#define TCPIP_CONNECT_IO (&tcpip_connect_io.super)
#define OPEN_FORWARDED_TCPIP (&open_forwarded_tcpip.super.super.super)
#define DIRECT_TCPIP_HANDLER (&make_direct_tcpip_handler.super)
#define INSTALL_DIRECT_TCPIP (&install_direct_tcpip_handler.super.super.super)

static struct catch_report_collect catch_channel_open;
#define CATCH_CHANNEL_OPEN (&catch_channel_open.super.super.super)

#include "tcpforward_commands.c.x"

static struct report_exception_info open_tcpip_report =
STATIC_REPORT_EXCEPTION_INFO(EXC_ALL, EXC_CHANNEL_OPEN, "Failed to open tcpip channel");

static struct catch_report_collect catch_channel_open
= STATIC_CATCH_REPORT(&open_tcpip_report);

/* Takes a socket as argument, and returns a tcpip channel. Used by
 * the party receiving a open-tcp request, when a channel to the
 * target has been opened. */

#define TCPIP_WINDOW_SIZE 10000

/* NOTE: make_tcpip_channel() adds the fd to the channel's resource list. */
static void
do_tcpip_connect_io(struct command *ignored UNUSED,
		    struct lsh_object *x,
		    struct command_continuation *c,
		    struct exception_handler *e UNUSED)
{
  CAST(listen_value, lv, x);

  assert(lv);
  assert(lv->fd);
  
  COMMAND_RETURN(c, make_tcpip_channel(lv->fd, TCPIP_WINDOW_SIZE));
}

struct command tcpip_connect_io = STATIC_COMMAND(do_tcpip_connect_io);

/* Used by the party requesting tcp forwarding, i.e. when a socket is
 * already open, and we have asked the other end to forward it. Takes
 * a channel as argument, and connects it to the socket. Returns the
 * channel. */

static void
do_tcpip_start_io(struct command *s UNUSED, 
		  struct lsh_object *x,
		  struct command_continuation *c,
		  struct exception_handler *e UNUSED)
{
  CAST_SUBTYPE(ssh_channel, channel, x);

  assert(channel);
  
  tcpip_channel_start_io(channel);

  COMMAND_RETURN(c, channel);
}

struct command tcpip_start_io =
{ STATIC_HEADER, do_tcpip_start_io };


/* Requesting the opening of a forwarded tcpip channel. */

/* Used for both forwarded-tcpip and direct-tcpip. Takes a listen
 * value as argument, and returns a channel connected to some tcpip
 * port at the other end. */

/* GABA:
   (class
     (name open_tcpip_command)
     (super channel_open_command)
     (vars
       ; ATOM_FORWARDED_TCPIP or ATOM_DIRECT_TCPIP
       (type . int)

       (initial_window . UINT32)

       ; For forwarded-tcpip, port is the port listened to.
       ; For direct-tcpip, port is the port to connect to.
       ; In both cases, it's a port used on the server end.
       (port object address_info)
       (peer object listen_value)))
*/

static struct ssh_channel *
new_tcpip_channel(struct channel_open_command *c,
		  struct ssh_connection *connection,
		  UINT32 local_channel_number,
		  struct lsh_string **request)
{
  CAST(open_tcpip_command, self, c);
  struct ssh_channel *channel;

  /* NOTE: All accepted fd:s must end up in this function, so it
   * should be ok to delay the REMEMBER() call until here. It is done
   * by make_tcpip_channel(). */

  debug("tcpforward_commands.c: new_tcpip_channel()\n");

  channel = make_tcpip_channel(self->peer->fd, TCPIP_WINDOW_SIZE);
  channel->write = connection->write;

  *request = format_channel_open(self->type, local_channel_number,
				 channel, 
				 "%S%i%S%i",
				 self->port->ip, self->port->port,
				 self->peer->peer->ip, self->peer->peer->port);
  
  return channel;
}

static struct command *
make_open_tcpip_command(int type, UINT32 initial_window,
			struct address_info *port,
			struct listen_value *peer)
{
  NEW(open_tcpip_command, self);

  debug("tcpforward_commands.c: make_open_tcpip_command()\n");

  self->super.super.call = do_channel_open_command;
  self->super.new_channel = new_tcpip_channel;

  self->type = type;
  self->initial_window = initial_window;
  
  self->port = port;
  self->peer = peer;
  
  return &self->super.super;
}

static struct lsh_object *
collect_open_forwarded_tcpip(struct collect_info_2 *info,
			     struct lsh_object *a,
			     struct lsh_object *b)
{
  CAST(address_info, local, a);
  CAST(listen_value, peer, b);

  assert(!info->next);

  return &make_open_tcpip_command(ATOM_FORWARDED_TCPIP, TCPIP_WINDOW_SIZE,
				  local, peer)->super;
}

static struct collect_info_2 collect_open_forwarded_tcpip_2 =
STATIC_COLLECT_2_FINAL(collect_open_forwarded_tcpip);

struct collect_info_1 open_forwarded_tcpip =
STATIC_COLLECT_1(&collect_open_forwarded_tcpip_2);

static struct lsh_object *
collect_open_direct_tcpip(struct collect_info_2 *info,
			  struct lsh_object *a,
			  struct lsh_object *b)
{
  CAST(address_info, local, a);
  CAST(listen_value, peer, b);

  assert(!info->next);

  return &make_open_tcpip_command(ATOM_DIRECT_TCPIP, TCPIP_WINDOW_SIZE,
				  local, peer)->super;
}

static struct collect_info_2 collect_open_direct_tcpip_2 =
STATIC_COLLECT_2_FINAL(collect_open_direct_tcpip);

struct collect_info_1 open_direct_tcpip =
STATIC_COLLECT_1(&collect_open_direct_tcpip_2);


/* Requesting remote forwarding of a port */

/* GABA:
   (class
     (name remote_port_install_continuation)
     (super command_frame)
     (vars
       (port object remote_port)
       (callback object command)))
*/

static void
do_remote_port_install_continuation(struct command_continuation *s,
				    struct lsh_object *x)
{
  CAST(remote_port_install_continuation, self, s);
  CAST(ssh_connection, connection, x);

  assert(connection);

  debug("tcpforward_commands.c: do_remote_port_install_continuation(), success.\n");
  self->port->callback = self->callback;

  COMMAND_RETURN(self->super.up, x);
}

static struct command_continuation *
make_remote_port_install_continuation(struct remote_port *port,
				      struct command *callback,
				      struct command_continuation *c)
{
  NEW(remote_port_install_continuation, self);

  debug("tcpforward_commands.c: make_remote_port_install_continuation()\n");

  self->super.super.c = do_remote_port_install_continuation;
  self->super.up = c;

  self->port = port;
  self->callback = callback;

  return &self->super.super;
}

/* Listening on a remote port
 *
 * (remote_listen callback port connection)
 *
 * Returns a remote_port or NULL.
 * 
 * callback is invoked with a address_info peer as argument, and
 * should return a channel or NULL.
 */

/* GABA:
   (class
     (name request_tcpip_forward_command)
     (super global_request_command)
     (vars
       ; Invoked when a forwarded_tcpip request is received.
       ; Called with the struct address_info *peer as argument.
       (callback object command)
       (port object address_info))) */

static struct lsh_string *
do_format_request_tcpip_forward(struct global_request_command *s,
				struct ssh_connection *connection,
				struct command_continuation **c)
{
  CAST(request_tcpip_forward_command, self, s);
  struct remote_port *port;
  int want_reply;

  debug("tcpforward_commands.c: do_format_request_tcpip_forward()\n");

  if (CONTINUATION_USED_P(*c))
    {
      /* FIXME: Use some exception handler to remove the port from the
       * list if the request fails. */
      port = make_remote_port(self->port, NULL);
      *c = make_remote_port_install_continuation(port, self->callback, *c);
      want_reply = 1;
    }
  else
    {
      port = make_remote_port(self->port, self->callback);
      want_reply = 0;
    }
  
  object_queue_add_tail(&connection->table->remote_ports,
			&port->super.super);
  
  return format_global_request(ATOM_TCPIP_FORWARD, want_reply, "%S%i",
			       self->port->ip, self->port->port);
}
		    
static struct command *
make_request_tcpip_forward_command(struct command *callback,
				   struct address_info *listen)
{
  NEW(request_tcpip_forward_command, self);

  debug("tcpforward_commands.c: make_request_tcpip_forward_command()\n");
  
  self->super.super.call = do_channel_global_command;
  self->super.format_request = do_format_request_tcpip_forward;

  self->callback = callback;
  self->port = listen;
  
  return &self->super.super;
}

static struct lsh_object *
collect_remote_listen(struct collect_info_2 *info,
		      struct lsh_object *a, struct lsh_object *b)
{
  CAST_SUBTYPE(command, callback, a);
  CAST(address_info, port, b);
  assert(!info->next);
  
  return &make_request_tcpip_forward_command(callback, port)->super;
}

static struct collect_info_2 collect_info_remote_listen_2 =
STATIC_COLLECT_2_FINAL(collect_remote_listen);

static struct collect_info_1 remote_listen_command =
STATIC_COLLECT_1(&collect_info_remote_listen_2);


/* Cancel a remotely forwarded port.
 * FIXME: Not implemented */



/* GABA:
   (expr
     (name forward_local_port)
     (params
       (backend object io_backend)
       (local object address_info)
       (target object address_info))
     (expr
       (lambda (connection)
         (connection_remember connection
           (listen_callback
	     (lambda (peer)
	       ;; Remembering is done by open_direct_tcpip
	       ;; and new_tcpip_channel.
	       (tcpip_start_io
	         (catch_channel_open 
		   (open_direct_tcpip target peer) connection)))
	     backend
	     local)))))
*/

struct command *
make_forward_local_port(struct io_backend *backend,
			struct address_info *local,
			struct address_info *target)
{
  CAST_SUBTYPE(command, res,
	       forward_local_port(backend, local, target));

  trace("tcpforward_commands.c: forward_local_port()\n");

  return res;
}

/* GABA:
   (expr
     (name forward_remote_port)
     (params
       (connect object command)
       (remote object address_info))
     (expr
       (lambda (connection)
         (remote_listen (lambda (peer)
	                  (tcpip_connect_io 
			     ; NOTE: The use of prog1 is needed to
			     ; delay the connect call until the
			     ; (otherwise ignored) peer argument is
			     ; available.  
			     (connect (prog1 connection peer))))
	                remote
			connection))))
*/

struct command *
make_forward_remote_port(struct io_backend *backend,
			 struct address_info *remote,
			 struct address_info *target)
{
  CAST_SUBTYPE(command, res,
	       forward_remote_port(make_connect_port(backend, target), remote));

  debug("tcpforward_commands.c: forward_remote_port()\n");
  
  return res;
}

/* Takes a callback function and returns a channel_open
 * handler. */
static void
do_make_direct_tcpip_handler(struct command *s UNUSED,
			     struct lsh_object *x,
			     struct command_continuation *c,
			     struct exception_handler *e UNUSED)
{
  CAST_SUBTYPE(command, callback,  x);

  trace("tcpforward_commands.c: do_make_open_tcp_handler()\n");
  
  COMMAND_RETURN(c,
		 &make_channel_open_direct_tcpip(callback)->super);
}

static struct command
make_direct_tcpip_handler = STATIC_COMMAND(do_make_direct_tcpip_handler);

/* Takes a callback function and returns a global_request handler. */
static void
do_make_tcpip_forward_handler(struct command *s UNUSED,
			      struct lsh_object *x,
			      struct command_continuation *c,
			      struct exception_handler *e UNUSED)
{
  CAST_SUBTYPE(command, callback,  x);

  debug("tcpforward_commands.c: do_make_tcpip_forward_handler()\n");
  
  COMMAND_RETURN(c,
		 &make_tcpip_forward_request(callback)->super);
}

static struct command
make_tcpip_forward_handler
= STATIC_COMMAND(do_make_tcpip_forward_handler);


/* Commands to install handlers */
struct install_info install_direct_tcpip_info_2 =
STATIC_INSTALL_OPEN_HANDLER(ATOM_DIRECT_TCPIP);

struct collect_info_1 install_direct_tcpip_handler =
STATIC_COLLECT_1(&install_direct_tcpip_info_2.super);

struct install_info install_forwarded_tcpip_info_2 =
STATIC_INSTALL_OPEN_HANDLER(ATOM_FORWARDED_TCPIP);

struct collect_info_1 install_forwarded_tcpip_handler =
STATIC_COLLECT_1(&install_forwarded_tcpip_info_2.super);

/* Server side callbacks */

/* Make this non-static? */
/* GABA:
   (expr
     (name direct_tcpip_hook)
     (params
       (connect object command))
     (expr
       (lambda (connection)
         (install_direct_tcpip connection
	   (direct_tcpip_handler (lambda (port)
	     (tcpip_connect_io (connect connection port))))))))
*/

struct command *
make_direct_tcpip_hook(struct io_backend *backend)
{
  CAST_SUBTYPE(command, res,
	       direct_tcpip_hook(make_connect_connection(backend)));

  debug("tcpforward_commands.c: make_direct_tcpip_hook()\n");

  return res;
}


struct install_info install_tcpip_forward_info_2 =
STATIC_INSTALL_GLOBAL_HANDLER(ATOM_TCPIP_FORWARD);

struct collect_info_1 install_tcpip_forward_handler =
STATIC_COLLECT_1(&install_tcpip_forward_info_2.super);

/* GABA:
   (expr
     (name tcpip_forward_hook)
     (globals
       (install "&install_tcpip_forward_handler.super.super.super")
       (handler "&make_tcpip_forward_handler.super"))
     (params
       (backend object io_backend))
     (expr
       (lambda (connection)
         ;; Called when the ssh-connection is established
         (install connection
	   (handler (lambda (port)
	     ;; Called when the client requests remote forwarding.
	     ;; It should return the fd associated with the port.
	     ;; NOTE: The caller, do_tcpip_forward_request, is responsible
	     ;; for handling I/O exceptions, and for remembering the port.
	     (listen_callback (lambda (peer)
  		  		;; Called when someone connects to the
		  		;; forwarded port.
				;; Remembering is done by open_direct_tcpip
				;; and new_tcpip_channel.
				(tcpip_start_io
		  		  (catch_channel_open 
		  		    (open_forwarded_tcpip port peer) connection)))
		  	      backend port))))))))
*/

struct command *
make_tcpip_forward_hook(struct io_backend *backend)
{
  CAST_SUBTYPE(command, res, tcpip_forward_hook(backend));

  debug("tcpforward_commands.c: tcpip_forward_hook()\n");
  
  return res;
}

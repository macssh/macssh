/* proxy_session.c
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

#include "proxy_session.h"

#include "channel_commands.h"
#include "format.h"
#include "gateway_channel.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include "proxy_session.c.x"

#define WINDOW_SIZE 10000


/* GABA:
   (class
     (name proxy_open_session)
     (super channel_open)
     (vars
       ; requests to accept from server -> client
       (server_requests object alist)
       ; requests to accept from client -> server
       (client_requests object alist)))

*/

static void
do_proxy_open_session(struct channel_open *s,
		      struct ssh_connection *connection,
		      struct channel_open_info *info,
		      struct simple_buffer *args,
		      struct command_continuation *c,
		      struct exception_handler *e)
{
  CAST(proxy_open_session, closure, s);

  debug("server.c: do_proxy_open_session()\n");

  if (parse_eod(args))
    {
      struct gateway_channel *server
	= make_gateway_channel(closure->server_requests);

      /* NOTE: The origin's rec_window_size and rec_max_packet becomes the target's
       * send_window_size and send_max_packet. */
      struct command *o =
	make_gateway_channel_open_command(info,
					  ssh_format(""),
					  closure->client_requests);

      COMMAND_CALL(o,
		   connection->chain,
		   make_gateway_channel_open_continuation(c, NULL, server),
		   e);

    }
  else
    {
      PROTOCOL_ERROR(e, "Trailing garbage in open message");
    }
}

struct channel_open *
make_proxy_open_session(struct alist *server_requests,
			struct alist *client_requests)
{
  NEW(proxy_open_session, self);

  self->super.handler = do_proxy_open_session;
  self->server_requests = server_requests;
  self->client_requests = client_requests;
  return &self->super;
}

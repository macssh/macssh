/* proxy_tcpforward.c
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

#include "proxy_tcpforward.h"

#include "gateway_channel.h"
#include "xalloc.h"
#include "ssh.h"
#include "werror.h"
#include "channel_commands.h"
#include "format.h"

#define WINDOW_SIZE 10000

static void
do_proxy_open_direct_tcpip(struct channel_open *s UNUSED,
			   struct ssh_connection *connection,
			   struct channel_open_info *info,
			   struct simple_buffer *args,
			   struct command_continuation *c,
			   struct exception_handler *e)
{
  struct lsh_string *host = NULL, *orig_host = NULL;
  UINT32 port, orig_port;

  debug("server.c: do_proxy_open_direct_tcpip\n");

  if ((host = parse_string_copy(args)) &&
      parse_uint32(args, &port) &&
      (orig_host = parse_string_copy(args)) &&
      parse_uint32(args, &orig_port) &&
      parse_eod(args))
    {
      struct gateway_channel *server
	= make_gateway_channel(NULL);

      /* NOTE: The origin's rec_window_size and rec_max_packet becomes the target's
       * send_window_size and send_max_packet. */

      struct command *o
	= make_gateway_channel_open_command(info,
					    ssh_format("%S%i%S%i",
						       host, port, 
						       orig_host, orig_port), NULL);

      werror("direct-tcpip open request: host to connect=%S:%i, originator=%S:%i",
	     host, port, orig_host, orig_port);
      COMMAND_CALL(o,
		   connection->chain,
		   make_gateway_channel_open_continuation(c, NULL, server),
		   e);

    }
  else
    {
      PROTOCOL_ERROR(e, "Trailing garbage in open message");
    }
  lsh_string_free(host);
  lsh_string_free(orig_host);
}

struct channel_open *
make_proxy_open_direct_tcpip(void)			     
{
  NEW(channel_open, self);

  self->handler = do_proxy_open_direct_tcpip;
  return self;
}

static void
do_proxy_open_forwarded_tcpip(struct channel_open *s UNUSED,
			      struct ssh_connection *connection,
			      struct channel_open_info *info,
			      struct simple_buffer *args,
			      struct command_continuation *c,
			      struct exception_handler *e)
{
  struct lsh_string *host = NULL, *orig_host = NULL;
  UINT32 port, orig_port;

  debug("server.c: do_proxy_open_forwarded_tcpip\n");

  if ((host = parse_string_copy(args)) &&
      parse_uint32(args, &port) &&
      (orig_host = parse_string_copy(args)) &&
      parse_uint32(args, &orig_port) &&
      parse_eod(args))
    {
      struct gateway_channel *server
	= make_gateway_channel(NULL);

      /* NOTE: The origin's rec_window_size and rec_max_packet becomes the target's
       * send_window_size and send_max_packet. */
      struct command *o
	= make_gateway_channel_open_command(info,
					    ssh_format("%S%i%S%i", 
						       host, port, 
						       orig_host, orig_port), NULL);

      werror("direct-tcpip open request: address where connection was accepted=%S:%i, originator=%S:%i",
	     host, port, orig_host, orig_port);
      COMMAND_CALL(o,
		   connection->chain,
		   make_gateway_channel_open_continuation(c, NULL, server),
		   e);

    }
  else
    {
      PROTOCOL_ERROR(e, "Trailing garbage in open message");
    }
  lsh_string_free(host);
  lsh_string_free(orig_host);
}

struct channel_open *
make_proxy_open_forwarded_tcpip(void)
{
  NEW(channel_open, self);

  self->handler = do_proxy_open_forwarded_tcpip;
  return self;
}

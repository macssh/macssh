/* proxy_agentforward.c
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

#include "proxy_agentforward.h"

#include "channel_commands.h"
#include "format.h"
#include "gateway_channel.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#define WINDOW_SIZE 10000

static void
do_proxy_open_auth_agent(struct channel_open *s UNUSED,
			 struct ssh_connection *connection,
			 struct channel_open_info *info,
			 struct simple_buffer *args,
			 struct command_continuation *c,
			 struct exception_handler *e)
{
  if (parse_eod(args))
    {
      struct gateway_channel *server
	= make_gateway_channel(NULL);

      /* NOTE: The origin's rec_window_size and rec_max_packet becomes the target's
       * send_window_size and send_max_packet. */
      struct command *o
	= make_gateway_channel_open_command(info,
					    ssh_format(""),
					    NULL);

      werror("auth-agent open request\n");
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
make_proxy_open_auth_agent(void)
{
  NEW(channel_open, self);

  self->handler = do_proxy_open_auth_agent;
  return self;
}


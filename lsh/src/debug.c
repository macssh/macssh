/* debug.c
 *
 *
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Niels Möller
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

#include "debug.h"

#include "connection.h"
#include "format.h"
#include "parse.h"
#include "ssh.h"
#include "xalloc.h"
#include "werror.h"

#include "debug.c.x"

#if MACOS
#include "lsh_context.h"
#endif

/* GABA:
   (class
     (name packet_debug)
     (super abstract_write_pipe)
     (vars
       (prefix string)))
*/

static void
do_debug(struct abstract_write *w,
	 struct lsh_string *packet)
{
  CAST(packet_debug, closure, w);

  if (!packet->length)
    debug("DEBUG: %S empty packet\n", closure->prefix);
  else
    {
      UINT8 type = packet->data[0];
      if (type == SSH_MSG_USERAUTH_REQUEST)
	debug("DEBUG: %S %z *****\n",
	      closure->prefix, packet_types[type]);
      else
	debug("DEBUG: %S %z %xS\n",
	      closure->prefix, packet_types[type],
	      packet);
    }
  A_WRITE(closure->super.next, packet);
}

struct abstract_write *
make_packet_debug(struct abstract_write *next,
		  struct lsh_string *prefix)
{
  NEW(packet_debug, closure);

  closure->super.super.write = do_debug;
  closure->super.next = next;
  closure->prefix = prefix;

  return &closure->super.super;
}


static struct lsh_string *
make_debug_packet(const char *msg, int always_display)
{
  return ssh_format("%c%c%z%z",
		    SSH_MSG_DEBUG,
		    always_display,
		    msg,
		    /* Empty language tag */ 
		    "");
}

/* Send a debug message to the other end. */
void
send_debug_message(struct abstract_write *write, const char *msg, int always_display)
{
  A_WRITE(write, make_debug_packet(msg, always_display));
}

void
send_debug(struct abstract_write *write, const char *msg, int always_display)
{
  if (debug_flag)
    send_debug_message(write, msg, always_display);
}

void
send_verbose(struct abstract_write *write, const char *msg, int always_display)
{
  if (verbose_flag)
    send_debug_message(write, msg, always_display);
}

DEFINE_PACKET_HANDLER(, connection_debug_handler, connection UNUSED, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  unsigned always_display;
  UINT32 length;
  const UINT8 *msg;
  int language;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (!(parse_uint8(&buffer, &msg_number)
	&& parse_uint8(&buffer, &always_display)
	&& parse_string(&buffer, &length, &msg)
	&& parse_atom(&buffer, &language)
	&& parse_eod(&buffer)))
    {
      PROTOCOL_ERROR(connection->e, "Invalid DEBUG message.");
    }
  else
    {
      if (always_display)
	werror("Received debug: %ups\n", length, msg);

      else
	verbose("Received debug: %ups\n", length, msg);
    }
}

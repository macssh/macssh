/* disconnect.c
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

#include "disconnect.h"

#include "connection.h"
#include "format.h"
#include "parse.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

struct lsh_string *format_disconnect(int code, const char *msg, 
                                     const char *language)
{
  return ssh_format("%c%i%z%z",
		    SSH_MSG_DISCONNECT,
		    code,
		    msg, language);
}

static void
do_disconnect(struct packet_handler *closure UNUSED,
	      struct ssh_connection *connection,
	      struct lsh_string *packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 reason;

  UINT32 length;
  const UINT8 *msg;

  const UINT8 *language;
  UINT32 language_length;
  
  static const struct exception disconnect_exception =
    STATIC_EXCEPTION(EXC_FINISH_IO, "Received disconnect message.");
    
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_DISCONNECT)
      && (parse_uint32(&buffer, &reason))
      && (parse_string(&buffer, &length, &msg))
      && (parse_string(&buffer, &language_length, &language))
      && parse_eod(&buffer))
    {
      /* FIXME: Display a better message */
      werror("Disconnect for reason %i: %ups\n", reason, length, msg);
    }
  else
    werror("Invalid disconnect message!\n");

  lsh_string_free(packet);
  
  EXCEPTION_RAISE(connection->e, &disconnect_exception);
}

struct packet_handler disconnect_handler =
{ STATIC_HEADER, do_disconnect };

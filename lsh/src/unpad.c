/* unpad.c
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

#include "unpad.h"

#include "format.h"
#include "ssh.h"
#include "xalloc.h"

#include "unpad.c.x"

/* GABA:
   (class
     (name packet_unpad)
     (super abstract_write_pipe)
     (vars
       ; Needed only for its exception_handler
       (connection object ssh_connection)))
*/

static void
do_unpad(struct abstract_write *w,
	 struct lsh_string *packet)
{
  CAST(packet_unpad, closure, w);
  
  UINT8 padding_length;
  UINT32 payload_length;
  struct lsh_string *new;
  
  if (packet->length < 1)
    {
      lsh_string_free(packet);

      PROTOCOL_ERROR(closure->connection->e,
		     "Empty packet received.");
      return;
    }
  
  padding_length = packet->data[0];

  if ( (padding_length < 4)
       || (padding_length >= packet->length) )
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(closure->connection->e,
		     "Bogus padding length.");
      return;
    }

  payload_length = packet->length - 1 - padding_length;

  /* FIXME: This check seems redundant; fuzzy length check is done in
   * read_packet.c, and a stricter check is done in connection.c. Some
   * additional checking, using the SSH_MAX_PACKET constant, is also
   * done in zlib.c */
  
  if (payload_length > (closure->connection->rec_max_packet
			+ SSH_MAX_PACKET_FUZZ))
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(closure->connection->e,
		     "Max payload length exceeded.");
      return;
    }
  new = ssh_format("%ls", payload_length, packet->data + 1);

  /* Keep sequence number */
  new->sequence_number = packet->sequence_number;

  lsh_string_free(packet);

  A_WRITE(closure->super.next, new);
}

struct abstract_write *
make_packet_unpad(struct ssh_connection *connection,
		  struct abstract_write *next)
{
  NEW(packet_unpad, closure);

  closure->super.super.write = do_unpad;
  closure->super.next = next;
  closure->connection = connection;

  return &closure->super.super;
}

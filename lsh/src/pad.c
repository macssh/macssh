/* pad.c
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

#include <assert.h>
#include <string.h>

#include "pad.h"

#include "format.h"
#include "xalloc.h"

#include "pad.c.x"

/* GABA:
   (class
     (name packet_pad)
     (super abstract_write_pipe)
     (vars
       (connection object ssh_connection)
       (random object randomness)))
*/

static void
do_pad(struct abstract_write *w,
       struct lsh_string *packet)
{
  CAST(packet_pad, closure, w);
  struct ssh_connection *connection = closure->connection;

  struct lsh_string *new;
  
  UINT32 new_size;
  UINT8 padding;

  UINT8 *data;
  UINT32 block_size;

  block_size = connection->send_crypto
    ? connection->send_crypto->block_size : 8;
  
  /* new_size is (packet->length + 9) rounded up to a multiple of
   * block_size */
  new_size = block_size
    * (1 + (8 + packet->length) / block_size);

  padding = new_size - packet->length - 5;
  assert(padding >= 4);

  new = ssh_format("%i%c%lr", packet->length + padding + 1,
		   padding, packet->length + padding, &data);

  assert(new->length == new_size);
  
  memcpy(data, packet->data, packet->length);
  RANDOM(closure->random, padding, data + packet->length);
  
  lsh_string_free(packet);

  A_WRITE(closure->super.next, new);
}
  
struct abstract_write *
make_packet_pad(struct abstract_write *next,
		struct ssh_connection *connection,
		struct randomness *random)
{
  NEW(packet_pad, closure);

  closure->super.super.write = do_pad;
  closure->super.next = next;
  closure->connection = connection;
  closure->random = random;

  return &closure->super.super;
}

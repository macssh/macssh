/* compress.c
 *
 * packet compressor
 * 
 * $Id$ */

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

#include "compress.h"

#include "exception.h"
#include "ssh.h"
#include "xalloc.h"

#include "compress.c.x"

#include <assert.h>

/* GABA:
   (class
     (name packet_compressor)
     (super abstract_write_pipe)
     (vars
       (compressor object compress_instance)
       (connection object ssh_connection)))
*/

static void
do_packet_deflate(struct abstract_write *closure,
		  struct lsh_string *packet)
{
  CAST(packet_compressor, self, closure);

  if (self->connection->send_compress)
    {
      packet = CODEC(self->connection->send_compress, packet, 1);
      assert(packet);
    }
  
  A_WRITE(self->super.next, packet);
}

static void
do_packet_inflate(struct abstract_write *closure,
		  struct lsh_string *packet)
{
  CAST(packet_compressor, self, closure);

  if (self->connection->rec_compress)
    {
      packet = CODEC(self->connection->rec_compress, packet, 1);
      if (!packet)
	{
	  /* FIXME: It would be nice to pass the error message from zlib on
	   * to the exception handler. */
	  EXCEPTION_RAISE
	    (self->connection->e,
	     make_protocol_exception(SSH_DISCONNECT_COMPRESSION_ERROR,
				     "Inflating compressed data failed."));
	  return;
	}
    }
  A_WRITE(self->super.next, packet);
}

struct abstract_write *
make_packet_codec(struct abstract_write *next,
		  struct ssh_connection *connection,
		  int mode)
{
  NEW(packet_compressor, res);
	
  res->super.super.write = (mode == COMPRESS_INFLATE)
    ? do_packet_inflate
    : do_packet_deflate;
  
  res->super.next = next;
  res->connection = connection;

  return &res->super.super;
}

/* read_data.c
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

#include "read_data.h"

#include "io.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "read_data.c.x"

/* GABA:
   (class
     (name read_data)
     (super io_consuming_read)
     (vars
       ; For flow control. 
   
       ; FIXME: Perhaps the information that is needed for flow
       ; control should be abstracted out from the channel struct? 
       
       (channel object ssh_channel)))
*/

static UINT32
do_read_data_query(struct io_consuming_read *s)
{
  CAST(read_data, self, s);
  
  assert(self->channel->sources);
    
  if (self->channel->flags &
      (CHANNEL_RECEIVED_CLOSE | CHANNEL_SENT_CLOSE | CHANNEL_SENT_EOF))
    {
      werror("read_data: Receiving data on closed channel. Ignoring.\n");
      return 0;
    }

  return MIN(self->channel->send_max_packet,
	     self->channel->send_window_size);
}


struct io_callback *
make_read_data(struct ssh_channel *channel,
	       struct abstract_write *write)
{
  NEW(read_data, self);

  init_consuming_read(&self->super, write);
  
  self->super.query = do_read_data_query;
  self->super.consumer = write;

  self->channel = channel;

  channel->sources++;
  
  return &self->super.super;
}
				  

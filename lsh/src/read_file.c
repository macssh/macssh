/* read_file.c
 *
 * A read handler for reading an entire file as a string.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
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

#include "read_file.h"

#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

#include "read_file.c.x"

/* GABA:
   (class
     (name read_file)
     (super read_handler)
     (vars
       (c object abstract_write)
       (buffer string)
       (pos . UINT32)))
*/

static UINT32
do_read_file(struct read_handler **h,
	     UINT32 available,
	     UINT8 *data)
{
  CAST(read_file, self, *h);

  if (!self->buffer)
    {
      /* Ignore any extra data. */
      werror("do_read_file: buffer == NULL, ignoring extra data.\n");
      return available;
    }
  
  if (!available)
    {
      /* EOF reached */
      self->buffer->length = self->pos;
	  
      A_WRITE(self->c, self->buffer);
      self->buffer = NULL;
	  
      *h = NULL;
      return 0;
    }
  else    
    {
      UINT32 left = self->buffer->length - self->pos;
      if (available >= left)
	{
	  memcpy(self->buffer->data + self->pos, data, left);
	  A_WRITE(self->c, self->buffer);
	  self->buffer = NULL;

#if 0
	  if (available > left)
	    A_WRITE(self->c, NULL);
#endif
	  *h = NULL;
	  return available;
	}
      else
	{
	  memcpy(self->buffer->data + self->pos, data, available);
	  self->pos += available;
	  return available;
	}
    }
}

struct read_handler *
make_read_file(struct abstract_write *c,
	       UINT32 max_size)
{
  NEW(read_file, self);
  assert(max_size);
  
  self->super.handler = do_read_file;
  self->c = c;
  self->buffer = lsh_string_alloc(max_size);
  self->pos = 0;

  return &self->super;
}

  

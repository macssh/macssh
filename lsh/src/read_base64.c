/* read_base64.c
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

#include "digits.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "read_base64.c.x"

/* GABA:
   (class
     (name read_base64)
     (super read_handler)
     (vars
       (c object abstract_write)
       (state . "struct base64_state")

       (buffer string)
       (pos . UINT32)))
*/

/* We can use any non-digit as terminator for base_64_decode */
#define TERMINATOR 0

static UINT32
do_read_base64(struct read_handler **h,
	     UINT32 available,
	     UINT8 *data)
{
  CAST(read_base64, self, *h);

  if (!self->buffer)
    {
      /* Ignore any extra data. */
      werror("do_read_base64: buffer == NULL, ignoring extra data.\n");
      return available;
    }
  
  if (!available)
    {
      /* EOF reached */
      if (base64_decode(&self->state, TERMINATOR)
	  == BASE64_END)
	{
	  self->buffer->length = self->pos;
	  A_WRITE(self->c, self->buffer);
	}
      else
	{
	  A_WRITE(self->c, NULL);
	  lsh_string_free(self->buffer);
	}

      self->buffer = NULL;
	  
      *h = NULL;
      return 0;
    }
  else    
    {
      UINT32 i;
      for (i = 0; i < available; )
	{
	  int digit = base64_decode(&self->state, data[i++]);
	  switch (digit)
	    {
	    case BASE64_INVALID:
	    case BASE64_END:
	      A_WRITE(self->c, NULL);
	      *h = NULL;
	      return available;

	    case BASE64_SPACE:
	    case BASE64_PARTIAL:
	      break;

	    default:
	      self->buffer->data[self->pos++] = digit;
	      if (self->pos == self->buffer->length)
		{
		  A_WRITE(self->c, self->buffer);
		  self->buffer = NULL;
		  *h = NULL;
		  return i;
		}
	    }
	}
      return available;
    }
}

struct read_handler *
make_read_base64(struct abstract_write *c,
		 UINT32 max_size)
{
  NEW(read_base64, self);
  assert(max_size);
  
  self->super.handler = do_read_base64;
  self->c = c;
  base64_init(&self->state, TERMINATOR);

  self->buffer = lsh_string_alloc(max_size);
  self->pos = 0;

  return &self->super;
}

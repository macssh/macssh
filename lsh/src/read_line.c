/* read_line.c
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

#include "read_line.h"

#include "werror.h"
#include "xalloc.h"

#define GABA_DEFINE
#include "read_line.h.x"
#undef GABA_DEFINE

#include "read_line.c.x"

/* GABA:
   (class
     (name read_line)
     (super read_handler)
     (vars
       (handler object line_handler)
       (e object exception_handler)
       
       ; Line buffer       
       (pos . UINT32)
       (buffer array UINT8 MAX_LINE)))
*/

static UINT32
do_read_line(struct read_handler **h,
	     UINT32 available,
	     UINT8 *data /*, struct exception_handler *e */)
{
  CAST(read_line, self, *h);

  UINT8 *eol;
  UINT32 consumed;
  UINT32 tail;
  UINT32 length;

  if (!available)
    {
      /* FIXME: Should we use some other exception type for this? */
      EXCEPTION_RAISE(self->e,
		      make_protocol_exception(0, "Unexpected EOF"));
      *h = NULL;
      return 0;
    }
  
  eol = memchr(data, 0x0a, available);

  if (!eol)
    {
      /* No newline character yet */
      if (available + self->pos >= 255)
	{
	  /* Too long line */
	  EXCEPTION_RAISE(self->e,
			  make_protocol_exception(0, "Line too long."));
	}
      else
	{
	  memcpy(self->buffer + self->pos, data, available);
	  self->pos += available;
	}
      return available;
    }

  tail = eol - data; /* Excludes the newline character */
  consumed = tail + 1; /* Includes newline character */

  if ( (self->pos + consumed) > 255)
    {
      /* Too long line */
      EXCEPTION_RAISE(self->e,
		      make_protocol_exception(0, "Line too long."));
    }

  /* Ok, now we have a line. Copy it into the buffer. */
  memcpy(self->buffer + self->pos, data, tail);
  length = self->pos + tail;
  
  /* Exclude carriage return character, if any */
  if (length && (data[length-1] == 0xd))
    length--;

  /* NOTE: This call can modify both self->handler and *h. */
  PROCESS_LINE(self->handler, h, length, self->buffer, self->e);
		    
  /* Reset */
  self->pos = 0;

  return consumed;
}

struct read_handler *make_read_line(struct line_handler *handler,
				    struct exception_handler *e)
{
  NEW(read_line, closure);
  assert(e);
  
  closure->super.handler = do_read_line;
  closure->pos = 0;

  closure->handler = handler;
  closure->e = e;
  
  return &closure->super;
}

  

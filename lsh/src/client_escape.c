/* client_escape.h
 *
 * Escape char handling.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, 2000, 2001 Niels Möller
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

#include "client.h"

#include "format.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

/* This must be defined before including the .x file. */

enum escape_state { GOT_NONE, GOT_NEWLINE, GOT_ESCAPE };

#include "client_escape.c.x"
  
struct escape_info *
make_escape_info(UINT8 escape)
{
  NEW(escape_info, self);
  unsigned i;

  self->escape = escape;
  
  for (i = 0; i<0x100; i++)
    self->dispatch[i] = NULL;

  return self;
}

/* GABA:
   (class
     (name escape_handler)
     (super abstract_write_pipe)
     (vars
       (info object escape_info)
       ; Number of characters of the prefix NL escape
       ; that have been read.
       (state . "enum escape_state")))
*/

static inline int
newlinep(UINT8 c)
{
  return (c == '\n') || (c == '\r');
}

/* Search for NEWLINE ESCAPE, starting at pos. If successful, returns
 * 1 and returns the index of the escape char. Otherwise, returns
 * zero. */
static UINT32
scan_escape(struct lsh_string *packet, UINT32 pos, UINT8 escape)
{
  for ( ; (pos + 2) <= packet->length; pos++)
    {
      if (newlinep(packet->data[pos])
	  && (packet->data[pos+1] == escape))
	return pos + 1;
    }
  return 0;
}

/* Returns 1 for the quote action. */ 
static int
escape_dispatch(struct escape_info *info,
		UINT8 c)
{
  struct lsh_callback *f;

  if (c == info->escape)
    return 1;
  
  f = info->dispatch[c];
  if (f)
    LSH_CALLBACK(f);
  else
    werror("<escape> `%pc' not defined.\n", c);
  
  return 0;
}

static void
do_escape_handler(struct abstract_write *s, struct lsh_string *packet)
{
  CAST(escape_handler, self, s);
  UINT32 pos;
  UINT32 done;

#if 0
  trace("do_escape_handler: state = %i, packet length = %i\n",
        self->state, packet->length);
#endif
  
  if (!packet)
    {
      /* EOF. Pass it on */
      A_WRITE(self->super.next, packet);
      return;
    }
  
  assert(packet->length);

  /* done is the length of the prefix of the data that is already
   * consumed in one way or the other, while pos is the next position
   * where it makes sense to look for the escape sequence. */
  done = pos = 0;

  /* Look for an escape at the start of the packet. */
  switch (self->state)
    {
    case GOT_NEWLINE:
      /* We already got newline. Is the next character the escape? */
      if (packet->data[0] == self->info->escape)
	{
	  if (packet->length == 1)
	    {
              self->state = GOT_ESCAPE;
	      lsh_string_free(packet);
	      return;
	    }
	  else
	    {
	      pos = 2;
	      if (escape_dispatch(self->info, packet->data[1]))
		/* The quote action. Keep the escape. */
		done = 1;
	      else
		done = 2;
            }
	}
      break;
    case GOT_ESCAPE:
      pos = 1;
      if (escape_dispatch(self->info, packet->data[0]))
	/* The quote action. Keep the escape. */
	done = 0;
      else
	done = 1;

      break;

    case GOT_NONE:
      /* No special processing needed. */
      break;
    }
  
  while ( (pos = scan_escape(packet, pos, self->info->escape)) )
    {
      /* We found another escape char! */

      /* First, pass on the data before the escape.  */
      assert(pos > done);
      A_WRITE(self->super.next,
	      ssh_format("%ls", pos - done, packet->data + done));
      
      /* Point to the action character. */
      pos++;
      if (pos == packet->length)
	{
	  /* Remember how far we got. */
	  self->state = GOT_ESCAPE;
	  lsh_string_free(packet);
	  return;
	}
      if (escape_dispatch(self->info, packet->data[pos]))
	/* Keep escape */
	done = pos;
      else
	done = pos + 1;
      pos++;
    }

  /* Handle any data after the final escape */
  if (done < packet->length)
    {
      /* Rember if the last character is a newline. */
      if (newlinep(packet->data[packet->length - 1]))
	self->state = GOT_NEWLINE;
      else
	self->state = GOT_NONE;
      
      /* Send data on */
      if (done)
	{
	  /* Partial packet */
	  A_WRITE(self->super.next,
		  ssh_format("%ls", packet->length - done,
			     packet->data + done));
	  lsh_string_free(packet);
	}
      else
	A_WRITE(self->super.next, packet);
    }
  else
    {
      self->state = GOT_NONE;
      lsh_string_free(packet);
    }
}

struct abstract_write *
make_handle_escape(struct escape_info *info, struct abstract_write *next)
{
  NEW(escape_handler, self);

  self->super.super.write = do_escape_handler;
  self->super.next = next;
  self->info = info;
  self->state = GOT_NONE;
  
  return &self->super.super;
}

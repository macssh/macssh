/* string_buffer.c
 *
 * Functions for building strings whose lengths are not known from the
 * start.
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

#include "string_buffer.h"

#include "xalloc.h"

#include <assert.h>
#include <string.h>

struct string_node
{
  struct string_node *prev;
  struct lsh_string *s;
};

void string_buffer_init(struct string_buffer *buffer,
			UINT32 guess)
{
  buffer->partial = lsh_string_alloc(guess);
  buffer->left = guess;
  buffer->current = buffer->partial->data;

  buffer->tail = NULL;
  /* buffer->nlist = 0; */

  buffer->total = 0;
}

void string_buffer_clear(struct string_buffer *buffer)
{
  struct string_node *n;

  lsh_string_free(buffer->partial);
  for (n = buffer->tail; n; )
    {
      struct string_node *old = n;
      n = old->prev;

      lsh_string_free(old->s);
      lsh_space_free(old);
    }
}

/* Assumes that the buffer->partial string is full */
void string_buffer_grow(struct string_buffer *buffer, UINT32 increment)
{
  struct string_node *n;

  NEW_SPACE(n);

  buffer->total += buffer->partial->length;
  
  n->s = buffer->partial;
  n->prev = buffer->tail;
  buffer->tail = n;

  buffer->partial = lsh_string_alloc(increment);
  buffer->current = buffer->partial->data;
  buffer->left = increment;
}

#if 0
void string_buffer_putc(struct string_buffer *buffer, UINT8 c)
{
  if (!buffer->left)
    string_buffer_grow(buffer, buffer->increment);

  assert(buffer->left);
  
  *buffer->current++ = c;
  buffer->left--;
}

void string_buffer_write(struct string_buffer *buffer,
			UINT32 length, const UINT8 *s)
{
  if (length > buffer->left)
    {
      memcpy(buffer->current, s, buffer->left);
      s += buffer->left;
      length -= buffer->left;
      string_buffer_grow(MAX(length, buffer->increment));
    }

  assert(length <= buffer->left);

  memcpy(buffer->current, s, length);
  buffer->current += length;
  buffer->left -= length;
}

struct lsh_string *string_buffer_final_write(struct string_buffer *buffer,
					     UINT32 length, const UINT8 *s)
{
  UINT32 final = buffer->total + length;
  
  if ( (length < left) && !buffer->tail)
    {
      /* This should be the usual case. */
      if (length)
	memcpy(buffer->current, s, length);

      buffer->partial->length = final
      return buffer->partial;
    }
  else
    {
      struct lsh_string *res = lsh_string_alloc(final);
      UINT8 *p = res->data + final;
      struct string_node *n;
      
      if (length)
	{
	  p -= length;
	  memcpy(p, s, length);
	}

      length = buffer->partial->length - buffer->left;
      p -= length;
      memcpy(p, buffer->partial->data, length);
      lsh_string_free(buffer->partial);
      
      for (n = buffer->tail; n; )
	{
	  struct string_node *old = n;
	  n = n->next;

	  p -= old->s->length;
	  memcpy(p, old->s->data, old->s->length);

	  lsh_string_free(old->s);
	  lsh_space_free(old);
	}

      assert(p == res->data);

      return res;
    }
}
#endif

#if 0
UINT32 string_buffer_length(struct string_buffer *buffer)
{
  return buffer->total + length;
}
#endif

struct lsh_string *string_buffer_final(struct string_buffer *buffer,
				       UINT32 left_over)
{
  UINT32 length = buffer->partial->length - left_over;
  UINT32 final = buffer->total + length;
  
  if (!buffer->tail)
    {
      /* This should be the usual case. */

      buffer->partial->length = final;
      return buffer->partial;
    }
  else
    {
      struct lsh_string *res = lsh_string_alloc(final);
      UINT8 *p = res->data + final;
      struct string_node *n;
      
      p -= length;
      memcpy(p, buffer->partial->data, length);
      lsh_string_free(buffer->partial);
      
      for (n = buffer->tail; n; )
	{
	  struct string_node *old = n;
	  n = n->prev;

	  p -= old->s->length;
	  memcpy(p, old->s->data, old->s->length);

	  lsh_string_free(old->s);
	  lsh_space_free(old);
	}
      
      assert(p == res->data);

      return res;
    }
}

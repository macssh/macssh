/* string_buffer.h
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

#ifndef LSH_STRING_BUFFER_H_INCLUDED
#define LSH_STRING_BUFFER_H_INCLUDED

/* We optimize for operations where we can guess an upper limit which
 * is reasonable most of the time. */

#include "lsh.h"

struct string_node;

struct string_buffer
{
#if 0
  /* Fail if the buffer grows larger than this value; zero means that
   * there is no limit. */
  UINT32 max;
  
  /* Amount of space to allocate at a time */
  UINT32 increment;
#endif

  struct lsh_string *partial; /* Partial block. */

  UINT32 left;
  UINT8 *current;

  /* List of blocks beyond the first one */
  struct string_node *tail;
#if 0
  unsigned nlist; /* Number of nodes */
#endif
  UINT32 total; /* Total string length, in list (i.e. not including
		 * partial) */
};

void string_buffer_init(struct string_buffer *buffer, UINT32 guess);

#if 0
int string_buffer_putc(struct string_buffer *buffer, UINT8 c);
int string_buffer_write(struct string_buffer *buffer,
			UINT32 length, const UINT8 *s);
#endif

void string_buffer_clear(struct string_buffer *buffer);

/* Assumes that the buffer->partial string is full */
void string_buffer_grow(struct string_buffer *buffer, UINT32 increment);

struct lsh_string *string_buffer_final(struct string_buffer *buffer,
				       UINT32 left);

#endif /* LSH_STRING_BUFFER_H_INCLUDED */

/* parse.c
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

#include "parse.h"

#include "format.h"
#include "list.h"
#include "werror.h"
#include "xalloc.h"

#include "parse_macros.h"

#include <assert.h>
#include <string.h>

void
simple_buffer_init(struct simple_buffer *buffer,
		   UINT32 capacity, const UINT8 *data)
{
  buffer->capacity = capacity;
  buffer->pos = 0;
  buffer->data = data;
}

int
parse_uint32(struct simple_buffer *buffer, UINT32 *result)
{
  if (LEFT < 4)
    return 0;

  *result = READ_UINT32(HERE);
  ADVANCE(4);
  return 1;
}

int
parse_string(struct simple_buffer *buffer,
	     UINT32 *length, const UINT8 **start)
{
  UINT32 l;

  if (!parse_uint32(buffer, &l))
    return 0;

  if (LEFT < l)
    return 0;

  *length = l;
  *start = HERE;
  ADVANCE(l);
  return 1;
}

int
parse_octets(struct simple_buffer *buffer,
	     UINT32 length, UINT8 *start)
{
  if (LEFT < length)
    return 0;
  memcpy(start, HERE, length);
  ADVANCE(length);
  return 1;
}

struct lsh_string *
parse_string_copy(struct simple_buffer *buffer)
{
  UINT32 length;
  const UINT8 *start;
  
  if (!parse_string(buffer, &length, &start))
    return NULL;

  return ssh_format("%ls", length, start);
}

/* Initializes subbuffer to parse a string from buffer */
int
parse_sub_buffer(struct simple_buffer *buffer,
		 struct simple_buffer *subbuffer)
{
  UINT32 length;
  const UINT8 *data;

  if (!parse_string(buffer, &length, &data))
    return 0;

  simple_buffer_init(subbuffer, length, data);
  return 1;
}

int
parse_uint8(struct simple_buffer *buffer, unsigned *result)
{
  if (!LEFT)
    return 0;

  *result = HERE[0];
  ADVANCE(1);
  return 1;
}

int
parse_utf8(struct simple_buffer *buffer, UINT32 *result)
{
  UINT32 first;
  int length;
  int i;
  
  if (!LEFT)
    return -1;

  first = HERE[0];

  if (first < 0x80)
    {
      *result = first;
      ADVANCE(1);
      return 1;
    }

  switch(first & 0xF0)
    {
    default:
      return 0;
    case 0xC0:
    case 0xD0:
      length = 2;
      *result = first & 0x1F;
      break;
    case 0xE0:
      length = 3;
      *result = first & 0x0F;
      break;
    case 0xF0:
      switch(first & 0x0E)
	{
	case 0: case 2: case 4: case 6:
	  length = 4;
	  *result = first & 0x07;
	  break;
	case 8: case 0xA:
	  length = 5;
	  *result = first & 0x03;
	  break;
	case 0xC:
	  length = 6;
	  *result = first & 0x01;
	  break;
	default:
	  fatal("Internal error!\n");
	}
      break;
    }
  for(i = 1; i<length; i++)
    {
      UINT32 c = HERE[i];
      if ( (c & 0xC0) != 0x80)
	return 0;
      *result = (*result << 6) | (c & 0x3f);
    }
  ADVANCE(length);
  return 1;
}  
      
int
parse_boolean(struct simple_buffer *buffer, int *result)
{
  if (!LEFT)
    return 0;
  *result = HERE[0];
  ADVANCE(1);
  return 1;
}

int
parse_bignum(struct simple_buffer *buffer, mpz_t result, UINT32 limit)
{
  UINT32 length;
  const UINT8 *digits;

  if (!parse_string(buffer, &length, &digits))
    return 0;

  /* NOTE: If the reciever expects an integer less than 256^limit,
   * there may still be limit + 1 digits, as a leading zero is
   * sometimes required to resolve signedness. */
  if (limit && (length > (limit + 1)))
    return 0;
  
  bignum_parse_s(result, length, digits);

  return 1;
}

int
parse_atom(struct simple_buffer *buffer, int *result)
{
  UINT32 length;
  const UINT8 *start;

  if ( (!parse_string(buffer, &length, &start))
       || length > 64)
    return 0;

  *result = lookup_atom(length, start);

  return 1;
}

/* NOTE: This functions record the fact that it has read to the end of
 * the buffer by setting the position to *beyond* the end of the
 * buffer. */
static int
parse_next_atom(struct simple_buffer *buffer, int *result)
{
  UINT32 i;

  assert (buffer->pos <=buffer->capacity);

  for(i = 0; i < LEFT; i++)
    {
      if (HERE[i] == ',')
	break;
      if (i == 64)
	/* Atoms can be no larger than 64 characters */
	return 0;
    }

  /* No empty atoms. */
  if (!i)
    return 0;

  *result = lookup_atom(i, HERE);
  ADVANCE(i+1);  /* If the atom was terminated at the end of the
		  * buffer, rather than by a comma, this points beyond
		  * the end of the buffer */
  return 1;
}

struct int_list *
parse_atoms(struct simple_buffer *buffer, unsigned limit)
{
  unsigned count;
  unsigned i;
  struct int_list *res;

  assert(limit);

  if (!LEFT)
    return make_int_list(0, -1);
  
  /* Count commas (no commas means one atom) */
  for (i = buffer->pos, count = 1; i < buffer->capacity; i++)
    if (buffer->data[i] == ',')
      {
	if (count >= limit)
	  return NULL;
	count++;
      }

  res = alloc_int_list(count);

  for (i = 0; i < count; i++)
    {
      if (!parse_next_atom(buffer, LIST(res)+i))
	{
	  KILL(res);
	  return NULL;
	}
    }

  return res;
}

struct int_list *
parse_atom_list(struct simple_buffer *buffer, unsigned limit)
{
  struct simple_buffer sub_buffer;

  if (!parse_sub_buffer(buffer, &sub_buffer))
    return NULL;

  return parse_atoms(&sub_buffer, limit);
}

void
parse_rest(struct simple_buffer *buffer,
	   UINT32 *length, const UINT8 **start)
{
  *length = LEFT;
  *start = HERE;

  ADVANCE(*length);
}

struct lsh_string *
parse_rest_copy(struct simple_buffer *buffer)
{
  UINT32 length = LEFT;
  struct lsh_string *s = ssh_format("%ls", length, HERE);

  ADVANCE(length);
  assert(!LEFT);

  return s;
}

/* Returns success (i.e. 1) iff there is no data left */
int
parse_eod(struct simple_buffer *buffer)
{
  return !LEFT;
}

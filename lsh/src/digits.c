/* digits.h
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

#include "digits.h"

#include "digit_table.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

void
base64_init(struct base64_state *state, UINT8 terminator)
{
  state->buffer = 0;
  state->bits = 0;
  state->terminator = terminator;
}

int
base64_decode(struct base64_state *state, UINT8 c)
{
  unsigned res;
  int digit;
  
  assert(state->bits < 8);
  if (c == state->terminator)
    {
      /* Check for unused bits */
      if (state->bits && ( ( (1<<state->bits) - 1) & state->buffer))
	{
	  werror("base64_decode: Base64 terminated with %i leftover bits.\n",
		 state->bits);
	  return BASE64_INVALID;
	}
      return BASE64_END;
    }

  digit = base64_digits[c];
  switch(digit)
    {
    case BASE64_SPACE:
    case BASE64_INVALID:
      return digit;
    default:
      assert(digit >= 0);
      state->buffer = (state->buffer << 6) | (unsigned) digit;
      state->bits += 6;
    }
  if (state->bits < 8)
    return BASE64_PARTIAL;

  res = (state->buffer >> (state->bits - 8)) & 0xff;
  state->bits -= 8;
  
  return res;
}

struct lsh_string *
simple_decode_hex(const unsigned char *in)
{
  unsigned length = strlen(in);
  unsigned i, j;
  
  struct lsh_string *out;
  
  assert(!(length % 2));
  out = lsh_string_alloc(length/2);

  for (i = j = 0; i<length; )
  {
    int d1 = hex_digits[in[i++]];
    int d2 = hex_digits[in[i++]];

    assert(d1 >= 0);
    assert(d2 >= 0);

    out->data[j++] = (d1 << 4) | d2;
  }

  return out;
}

struct lsh_string *
decode_base64(UINT32 length, const UINT8 *in)
{
  struct base64_state state;
  struct lsh_string *out = lsh_string_alloc(length * 3 / 4 + 1);
  UINT32 i;
  UINT32 j;

  base64_init(&state, 0);
  for (i=0, j=0; i<length; i++)
    {
      int digit = base64_decode(&state, in[i]);
      switch (digit)
	{
	case BASE64_INVALID:
	case BASE64_END:
	  lsh_string_free(out);
	  return NULL;
	case BASE64_SPACE:
	case BASE64_PARTIAL:
	  continue;
	default:
	  assert(j < out->length);
	  out->data[j++] = digit;
	}
    }
  out->length = j;
  return out;
}
      
struct lsh_string *
simple_decode_base64(const unsigned char *in)
{
  return decode_base64(strlen(in), in);
}

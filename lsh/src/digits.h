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

#ifndef LSH_DIGITS_H_INCLUDED
#define LSH_DIGITS_H_INCLUDED

#include "lsh_types.h"

#define HEX_INVALID -1
#define HEX_END -2

extern int hex_digits[0x100];

#define BASE64_INVALID -1
#define BASE64_END -2
#define BASE64_SPACE -3
#define BASE64_PARTIAL -4

extern int base64_digits[0x100];

struct base64_state
{
  /* Bits are shifted into the buffer from the right, 6 at a time */
  unsigned buffer;
  /* Bits currently in the buffer */
  unsigned bits;

  UINT8 terminator;
};

#define BASE64_INIT(t) {0, 0, (t)}

void
base64_init(struct base64_state *state, UINT8 terminator);

int
base64_decode(struct base64_state *state, UINT8 c);

struct lsh_string *
decode_base64(UINT32 length, const UINT8 *in);


/* Only used by the testsuite */
struct lsh_string *
simple_decode_hex(const unsigned char *in);

struct lsh_string *
simple_decode_base64(const unsigned char *in);

#endif /* LSH_DIGITS_H_INCLUDED */

/* format.h
 *
 * Create a packet from a format string and arguments.
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

#ifndef LSH_FORMAT_H_INCLUDED
#define LSH_FORMAT_H_INCLUDED

#include <stdarg.h>

#include "atoms.h"
#include "bignum.h"

/* Format strings can contain the following %-specifications:
 *
 * %%  Insert a %-sign
 *
 * %c  Insert an 8-bit character
 *
 * %i  Insert a 32-bit integer, in network byte order
 *
 * %s  Insert a string, given by a length and a pointer.
 *
 * %S  Insert a string, given as a struct lsh_string pointer.
 *
 * %z  Insert a string, using a null-terminated argument.
 *
 * %r  Reserves space in the string, and stores a pointer to this space
 *     into the given UINT8 ** argument.
 *
 * %a  Insert a string containing one atom.
 *
 * %A  Insert a string containing a list of atoms. The input is an
 *     int_list object. Zero elements are allowed and ignored.
 *
 * %X  Insert a string containing a list of atoms. The corresponding
 *     argument sublist should be terminated with a zero. (Not used)
 *
 * %n  Insert a string containing a bignum.
 *
 * There are three valid modifiers:
 *
 * "l" (as in literal). It is applicable to the s, a, A, n and r
 * specifiers, and outputs strings *without* a length field.
 *
 * "d" (as in decimal). For integers, convert the integer to decimal
 * digits. For strings, format the input string using sexp syntax;
 * i.e. prefixed with the length in decimal.
 *
 * "x" (as in heXadecimal). For strings, format each character as two
 * hexadecimal digits. Does not currently mean any thing for numbers.
 * Note that this modifier is orthogonal to the decimal modifier.
 * 
 * "f" (as in free). Frees the input string after it has been copied.
 * Applicable to %S only.
 *
 * "u" (as in unsigned). Used with bignums, to use unsigned-only
 * number format. */

struct lsh_string *ssh_format(const char *format, ...);
UINT32 ssh_format_length(const char *format, ...);
void ssh_format_write(const char *format, UINT32 length, UINT8 *buffer, ...);

UINT32 ssh_vformat_length(const char *format, va_list args);
void ssh_vformat_write(const char *format, UINT32 length, UINT8 *buffer, va_list args);

/* Used for generating strings that can be used with standard libc
 * functions. It checks that the resulting string doesn't contain any
 * NUL characters (if it does, NULL is returned), and adds a
 * termianting NUL character at the end, not included in the string
 * length. */
struct lsh_string *ssh_cformat(const char *format, ...);

/* Short cuts */
#define lsh_string_dup(s) (ssh_format("%lS", (s)))

/* FIXME: The names of these three functions are not quite intuitive.
 * Perhaps they should be renamed or deleted. */

struct lsh_string *
format_cstring(const char *s);

struct lsh_string *
make_cstring_l(UINT32 length, const UINT8 *data);

struct lsh_string *
make_cstring(struct lsh_string *s, int free);

unsigned format_size_in_decimal(UINT32 n);
void format_decimal(unsigned length, UINT8 *buffer, UINT32 n);

/* FIXME: This function doesn't really belong here */
int lsh_string_eq(const struct lsh_string *a, const struct lsh_string *b);
int lsh_string_eq_l(const struct lsh_string *a, UINT32 length, const UINT8 *b);

int lsh_string_prefixp(const struct lsh_string *prefix,
		       const struct lsh_string *s);

#endif /* LSH_FORMAT_H_INCLUDED */

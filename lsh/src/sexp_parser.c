/* sexp_parser.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, Ron Rivest, Niels Möller
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

/* This parser reads an sexp from a buffer in memory, and supports
 * *only* the canonical syntax. */

#include "sexp.h"

#include "digits.h"
#include "format.h"
#include "parse_macros.h"
#include "queue.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

/* Automatically generated files. */
#include "sexp_table.h"

/* Returns the length of the segment of characters of a class */
static UINT32
sexp_scan_class(struct simple_buffer *buffer, int class)
{
  UINT32 i;

  for (i=0; i<LEFT; i++)
    if (!(sexp_char_classes[HERE[i]] & class))
      break;

  return i;
}

/* Skip the next input character on input stream struct simple_buffer
 * *buffer, if it is equal to a given character. Return 1 on success,
 * 0 on error. */
static int
sexp_skip_char(struct simple_buffer *buffer, UINT8 expect)
{
  UINT8 c;
  
  if (!LEFT)
    {
      werror("sexp: Unexpected EOF when expecting character %xi.\n",
	     expect);
      return 0;
    }
  c = GET();
  if (c != expect)
    {
      werror("sexp: Expected char %xi, got %xi.\n", expect, c);
      return 0;
    }

#if 0
  trace("sexp_skip_char: Skipped expected character %xi.\n", c);
#endif
  return 1;
}

#define MAX_DIGITS 8

/* Parse a decimal number */
static int
sexp_parse_decimal(struct simple_buffer *buffer, UINT32 *value)
{
  unsigned length = sexp_scan_class(buffer, CHAR_digit);
  unsigned i;

  if (!length)
    return 0;
  
  if ((*HERE == '0') && (length != 1))
    {
      /* No leading zeros allowed */
      werror("sexp: Unexpected leading zeroes\n");
      return 0;
    }
  if (length > MAX_DIGITS)
    {
      werror("sexp: Decimal number too long (%i digits, max is 8).\n",
	     length);
      return 0;
    }
  for (i = 0, *value = 0; i<length; i++)
    *value = *value * 10 + HERE[i] - '0';

  ADVANCE(length);
  return 1;
}

/* Reads a literal string of given length. */
static struct lsh_string *
sexp_parse_literal(struct simple_buffer *buffer, UINT32 length)
{
  struct lsh_string *res;
  
  if (LEFT < length)
    {
      werror("sexp: Unexpected EOF in literal.\n");
      return NULL;
    }

  res = ssh_format("%ls", length, HERE);
  ADVANCE(length);

  return res;
}

/* Reads and returns a simple string from the input stream, using the
 * canonical encoding. */
static struct lsh_string *
sexp_parse_string_canonical(struct simple_buffer *buffer)
{
  UINT32 length;
      
  if (sexp_parse_decimal(buffer, &length)
      && sexp_skip_char(buffer, ':'))
    return sexp_parse_literal(buffer, length);

  return NULL;
}

/* Called when the leading '[' is already consumed */
static struct sexp *
sexp_parse_display_canonical(struct simple_buffer *buffer)
{
  struct lsh_string *display;

  display = sexp_parse_string_canonical(buffer);

  if (display)
    {
      if (sexp_skip_char(buffer, ']'))
	{
	  struct lsh_string *contents;
	  
	  contents = sexp_parse_string_canonical(buffer);

	  if (contents)
	    return sexp_s(display, contents);
	}
      lsh_string_free(display);
    }

  return NULL;
}

static struct sexp *
sexp_parse_list_canonical(struct simple_buffer *buffer)
{
  struct object_queue p;
  object_queue_init(&p);

  while (LEFT)
    {
      struct sexp *e;
      
      if (*HERE == ')')
	{
	  ADVANCE(1);
	  return sexp_v(queue_to_list_and_kill(&p));
	}
      e = sexp_parse_canonical(buffer);
      if (!e)
	{
	  object_queue_kill(&p);
	  return NULL;
	}
      object_queue_add_tail(&p, &e->super);
    }
  werror("sexp: Unexpected EOF (missing ')')\n");
  
  object_queue_kill(&p);
  return NULL;
}

struct sexp *
sexp_parse_canonical(struct simple_buffer *buffer)
{
  if (!LEFT)
    {
      werror("sexp: Unexpected EOF.\n");
      return NULL;
    }
  
  switch(*HERE)
    {
    case '(':
      ADVANCE(1);
      return sexp_parse_list_canonical(buffer);
    case '[':
      ADVANCE(1);
      return sexp_parse_display_canonical(buffer);
    default:
      {
	struct lsh_string *s = sexp_parse_string_canonical(buffer);
	return s ? sexp_s(NULL, s) : NULL;
      }
    }
}


static void
skip_space(struct simple_buffer *buffer)
{
  while (LEFT)
    {
      UINT32 length = sexp_scan_class(buffer, CHAR_space);
      ADVANCE(length);
      
      if (LEFT && (*HERE == ';'))
	while (LEFT && (*HERE != '\n'))
	  ADVANCE(1);

      else if (!length)
	/* No comment, and no spaces skipped. Done. */
	return;
    }
}

/* NOTE: Implements the transport syntax. Also allows comments,
 * starting with ';' and terminating at end of line. */

struct sexp *
sexp_parse_transport(struct simple_buffer *buffer)
{
  struct sexp *expr = NULL;
  
  skip_space(buffer);
  
  if (!LEFT)
    {
      werror("sexp: Unexpected EOF.\n");
      return NULL;
    }
  
  if (*HERE != '{')
    expr = sexp_parse_canonical(buffer);
  
  else
    {
      UINT32 length;

      ADVANCE(1); /* Skip '{', and search for '}'. */

      for (length = 0; length < LEFT; length++)
	if (HERE[length] == '}')
	  {
	    struct lsh_string *canonical = decode_base64(length, HERE);
	    struct simple_buffer inner;
	    
	    if (!canonical)
	      {
		werror("sexp: Invalid transport encoding.\n");
		return NULL;
	      }
	    simple_buffer_init(&inner, canonical->length, canonical->data);
	    expr = sexp_parse_canonical(&inner);
	    
	    lsh_string_free(canonical);
	    ADVANCE(length + 1);
	    break;
	  }
    }
  if (expr)
    /* It seems a little redundant to skip space *both* before and after
     * parsing an expression, but it's the simplest way to make sure we allow
     * space both at the beginning and at the end of a file.
     */
    skip_space(buffer);
  else
    werror("sexp: Missing } in transport encoding.\n");
	    
	    return expr;
	  }

struct sexp *
sexp_parse(int style, struct simple_buffer *buffer)
{
  switch(style)
    {
    case SEXP_CANONICAL:
      return sexp_parse_canonical(buffer);
    case SEXP_ADVANCED:
    case SEXP_INTERNATIONAL:
      werror("sexp syntax %i not supported in parser. "
	     "Using transport syntax instead.\n", style);
      /* Fall through */
    case SEXP_TRANSPORT:
      return sexp_parse_transport(buffer);
    default:
      fatal("sexp_parse: Internal error, style %i not implemented.\n", style);
    }
}

struct sexp *
string_to_sexp(int style, const struct lsh_string *src, int free)
{
  struct simple_buffer buffer;
  struct sexp *e;
  
  simple_buffer_init(&buffer, src->length, src->data);

  e = sexp_parse(style, &buffer);

  if (e && !parse_eod(&buffer))
    e = NULL;
  
      if (free)
	lsh_string_free(src);

      return e;
    }

/* sexp.c
 *
 * An implementation of Ron Rivest's S-expressions, used in spki.
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

#include "sexp.h"

#include "format.h"
#include "queue.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define GABA_DEFINE
#include "sexp.h.x"
#undef GABA_DEFINE

/* Defines int sexp_char_classes[0x100] */
#define CHAR_CLASSES_TABLE sexp_char_classes
#include "sexp_table.h"
#undef CHAR_CLASSES_TABLE

#include "sexp.c.x"

/* GABA:
   (class
     (name sexp_string)
     (super sexp)
     (vars
       (display const string)
       (contents const string)))
*/

/* For advanced format */
static struct lsh_string *
do_format_simple_string(const struct lsh_string *s,
			int style,
			unsigned indent)
{
  int quote_friendly = ( (~CHAR_control & ~CHAR_international)
			 | CHAR_escapable);
  
  switch(style)
    {
    case SEXP_TRANSPORT:
      fatal("Internal error!\n");
    case SEXP_CANONICAL:
      return ssh_format("%dS", s);
    case SEXP_INTERNATIONAL:
      quote_friendly |= CHAR_international;
      /* Fall through */
    case SEXP_ADVANCED:
      {
	int c;
	unsigned i;

	if (!s->length)
	  return ssh_format("\"\"");

	/* Compute the set of all character classes represented in the string */
	for (c = 0, i = 0; i < s->length; i++)
	  c |= sexp_char_classes[s->data[i]];

	if (! ( (sexp_char_classes[s->data[0]] & CHAR_digit)
		|| (c & ~(CHAR_alpha | CHAR_digit | CHAR_punctuation))))
	  /* Output token, without any quoting at all */
	  return lsh_string_dup(s);

	if (! (c & ~quote_friendly))
	  {
	    /* Count the number of characters needing escape */
	    unsigned length = s->length;
	    unsigned i;
	    struct lsh_string *res;
	    UINT8 *dst;
	    
	    for (i = 0; i<s->length; i++)
	      if (sexp_char_classes[s->data[i]] & CHAR_escapable)
		length++;

	    res = ssh_format("\"%lr\"", length, &dst);
	    for (i=0; i<s->length; i++)
	      if (sexp_char_classes[s->data[i]] & CHAR_escapable)
		{
		  *dst++ = '\\';
		  switch(s->data[i])
		    {
		    case '\b':
		      *dst++ = 'b';
		      break;
		    case '\t':
		      *dst++ = 't';
		      break;
		    case '\v':
		      *dst++ = 'v';
		      break;
		    case '\n':
		      *dst++ = 'n';
		      break;
		    case '\f':
		      *dst++ = 'f';
		      break;
		    case '\r':
		      *dst++ = 'r';
		      break;
		    case '\"':
		      *dst++ = '\"';
		      break;
		    case '\\':
		      *dst++ = '\\';
		      break;
		    default:
		      fatal("Internal error!\n");
		    }
		}
	      else
		*dst++ = s->data[i];

	    assert(dst == (res->data + 1 + length));

	    return res;
	  }
	/* Base 64 string */
	return encode_base64(s, "||", 1, indent + 1, 0);
      }
    default:
      fatal("do_format_sexp_string: Unknown output style.\n");
    }
}
  
static struct lsh_string *
do_format_sexp_string(struct sexp *s,
		      int style, unsigned indent)
{
  CAST(sexp_string, self, s);

  switch(style)
    {
    case SEXP_TRANSPORT:
      fatal("Internal error!\n");
    case SEXP_ADVANCED:
    case SEXP_INTERNATIONAL:
    case SEXP_CANONICAL:
      return (self->display)
	? ssh_format("[%lfS]%lfS",
		     do_format_simple_string(self->display, style, indent + 1),
		     do_format_simple_string(self->contents, style, indent))
	: do_format_simple_string(self->contents, style, indent);
      
    default:
      fatal("do_format_sexp_string: Unknown output style.\n");
    }
}

/* Consumes its args (display may be NULL) */
struct sexp *
sexp_s(struct lsh_string *d, struct lsh_string *c)
{
  NEW(sexp_string, s);
  assert(c);
  
  s->super.format = do_format_sexp_string;
  s->super.iter = NULL;
  
  s->display = d;
  s->contents = c;
  
  return &s->super;
}

const struct lsh_string *
sexp_contents(const struct sexp *e)
{
  CAST(sexp_string, self, e);
  return self->contents;
}

const struct lsh_string *
sexp_display(const struct sexp *e)
{
  CAST(sexp_string, self, e);
  return self->display;
}

/* The nil expression. */

static struct sexp *
do_nil_get(struct sexp_iterator *c UNUSED)
{
  return NULL;
}

static struct sexp *
do_nil_assoc(struct sexp_iterator *s UNUSED, UINT32 length UNUSED,
	     const UINT8 *name UNUSED, struct sexp_iterator **i UNUSED)
{
  return NULL;
}

static unsigned
do_nil_left(struct sexp_iterator *c UNUSED)
{
  return 0;
}

static void
do_nil_next(struct sexp_iterator *c UNUSED)
{}

static struct sexp_iterator *
make_iter_nil(struct sexp *e UNUSED)
{
  static struct sexp_iterator nil_iterator =
    { STATIC_HEADER, do_nil_get, do_nil_assoc, do_nil_left, do_nil_next };
  return &nil_iterator;
}

static struct lsh_string *
do_format_nil(struct sexp *ignored UNUSED, int style UNUSED,
		   unsigned indent UNUSED)
{
  return ssh_format("()");
}

struct sexp sexp_nil =
{ STATIC_HEADER, make_iter_nil, do_format_nil };

#define SEXP_NIL (&sexp_nil)

/* For assoc */
struct sexp_iterator *
sexp_check_type_l(struct sexp *e, UINT32 length,
		  const UINT8 *name)
{
  if (!sexp_atomp(e))
    {
      struct sexp_iterator *i = SEXP_ITER(e);

      if (sexp_eq(SEXP_GET(i), length, name))
	{
	  SEXP_NEXT(i);
	  return i;
	}
      else
	KILL(i);
    }  
  return NULL;
}

/* Returns 1 if the type matches.
 *
 * FIXME: Do we relly need this interface, that allows res == NULL? */
int
sexp_check_type(struct sexp *e, int type, struct sexp_iterator **res)
{
  struct sexp_iterator *i =
    sexp_check_type_l(e, get_atom_length(type), get_atom_name(type));

  if (i)
    {
      if (res)
	*res = i;
      else
	KILL(i);
      return 1;
    }
  return 0;
}

static int
is_short(struct lsh_string *s)
{
  return ( (s->length < 15)
	   && !memchr(s->data, '\n', s->length) );
}

/* GABA:
   (class
     (name sexp_vector)
     (super sexp)
     (vars
       ; FIXME: With better var-array support, this
       ; could use an embedded var-array instead.
       (elements object object_list)))
*/

/* GABA:
   (class
     (name sexp_iter_vector)
     (super sexp_iterator)
     (vars
       (l object object_list)
       (i . unsigned)))
*/

static struct sexp *
do_vector_get(struct sexp_iterator *c)
{
  CAST(sexp_iter_vector, i, c);
  if (i->i < LIST_LENGTH(i->l))
    {
      CAST_SUBTYPE(sexp, res, LIST(i->l)[i->i]);
      return res;
    }
  return NULL;
}

static struct sexp *
do_vector_assoc(struct sexp_iterator *s, UINT32 length,
		const UINT8 *name, struct sexp_iterator **i)
{
  CAST(sexp_iter_vector, self, s);
  unsigned j;
  
  for (j = self->i; j < LIST_LENGTH(self->l); j++)
    {
      CAST_SUBTYPE(sexp, e, LIST(self->l)[j]);
      struct sexp_iterator *inner = sexp_check_type_l(e, length, name);
      
      if (inner)
	{
	  if (i)
	    *i = inner;

	  return e;
	}
    }
  return NULL;
}

static unsigned
do_vector_left(struct sexp_iterator *s)
{
  CAST(sexp_iter_vector, i, s);
  return LIST_LENGTH(i->l) - i->i;
}

static void
do_vector_next(struct sexp_iterator *c)
{
  CAST(sexp_iter_vector, i, c);
  if (i->i < LIST_LENGTH(i->l))
    i->i++;
}

static struct sexp_iterator *
make_iter_vector(struct sexp *s)
{
  CAST(sexp_vector, v, s);
  NEW(sexp_iter_vector, iter);

  iter->super.get = do_vector_get;
  iter->super.assoc = do_vector_assoc;
  iter->super.left = do_vector_left;
  iter->super.next = do_vector_next;

  iter->l = v->elements;
  iter->i = 0;

  return &iter->super;
}

static struct lsh_string *
do_format_sexp_vector(struct sexp *e,
		      int style, unsigned indent)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  CAST(sexp_vector, v, e);

  struct lsh_string *res;
  UINT8 *dst;
  
  CAST_SUBTYPE(sexp, car, LIST(v->elements)[0]);
      
  assert(LIST_LENGTH(v->elements));

  indent++;
  
  if (LIST_LENGTH(v->elements) == 1)
    {
      CAST_SUBTYPE(sexp, car, LIST(v->elements)[0]);

      return ssh_format("(%lfS)", sexp_format(car, style, indent));
    }
  
  switch(style)
    {
    case SEXP_TRANSPORT:
      fatal("Internal error!\n");
    case SEXP_ADVANCED:
    case SEXP_INTERNATIONAL:
      {
	struct lsh_string **elements;

	unsigned i;
	UINT32 size;

	/* align_after means formatting like
	 *
	 * (tag a
	 *      b)
	 */
	unsigned align_after = 0;
	
	struct lsh_string *tag = sexp_format(car, style, indent);

	if (is_short(tag))
	  {
	    indent += tag->length + 1;
	    align_after = 1;
	  }

	/* NOTE: The first element is never used. */
	elements = alloca(LIST_LENGTH(v->elements)
			  * sizeof(struct lsh_string *) );
	
	for (i = 1, size = 0; i<LIST_LENGTH(v->elements); i++)
	  {
	    CAST_SUBTYPE(sexp, o, LIST(v->elements)[i]);
	    
	    elements[i] = sexp_format(o, style, indent);
	    size += elements[i]->length;
	  }

	/* Count a single character separator between each element. */
	size += LIST_LENGTH(v->elements) - 1;
	
	/* Count extra indentation */
	size += indent * (LIST_LENGTH(v->elements) + !align_after - 2);

	res = ssh_format("(%lS%lr)", tag, size, &dst);

	for (i = 1; i<LIST_LENGTH(v->elements); i++)
	  {
	    if (i <= align_after)
	      *dst++ = ' ';
	    else
	      {
		*dst++ = '\n';
		memset(dst, ' ', indent);
		dst += indent;
	      }
	    memcpy(dst, elements[i]->data, elements[i]->length);
	    dst += elements[i]->length;
	    
	    lsh_string_free(elements[i]);
	  }

	break;
      }
    case SEXP_CANONICAL:
      {
	struct lsh_string **elements
	  = alloca(LIST_LENGTH(v->elements) 
		   * sizeof(struct lsh_string *) );

	unsigned i;
	UINT32 size;

	for (i = 0, size = 0; i<LIST_LENGTH(v->elements); i++)
	  {
	    CAST_SUBTYPE(sexp, o, LIST(v->elements)[i]);
	    
	    elements[i] = sexp_format(o, style, indent + 1);
	    size += elements[i]->length;
	  }

	res = ssh_format("(%lr)", size, &dst);
	
	for (i = 0; i<LIST_LENGTH(v->elements); i++)
	  {
	    memcpy(dst, elements[i]->data, elements[i]->length);
	    dst += elements[i]->length;
	    
	    lsh_string_free(elements[i]);
	  }
	
	break;
      }
    default:
      fatal("do_format_sexp_vector: Unknown output style.\n");
    }

  assert( (dst + 1) == (res->data + res->length));
  
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
  return res;

}

struct sexp *
sexp_v(struct object_list *l)
{
  if (LIST_LENGTH(l))
    {
      NEW(sexp_vector, v);
      
      v->super.format = do_format_sexp_vector;
      v->super.iter = make_iter_vector;
      
      v->elements = l;
      
      return &v->super;
    }
  else
    return SEXP_NIL;
}

struct sexp *
sexp_l(unsigned n, ...)
{
  va_list args;

  va_start(args, n);

  if (!n)
    {
      assert(va_arg(args, int) == -1);
      va_end(args);
      return SEXP_NIL;
    }
  else
    {
      struct sexp *v = sexp_v(make_object_listv(n, args));
      
      va_end(args);

      return v;
    }
}

struct sexp *
sexp_a(const int a)
{
  return sexp_s(NULL, ssh_format("%la", a));
}

struct sexp *
sexp_z(const char *s)
{
  return sexp_s(NULL, ssh_format("%lz", s));
}

/* mpz->sexp */
struct sexp *
sexp_un(const mpz_t n)
{
  struct lsh_string *s;
  UINT32 l = bignum_format_u_length(n);

  s = lsh_string_alloc(l);
  l -= bignum_format_u(n, s->data);

  assert(!l);
  
  return sexp_s(NULL, s);
}

/* Small unsigned int -> sexp */
struct sexp *
sexp_uint32(UINT32 n)
{
  /* FIXME: Eliminate redundant leading zeroes. */
  struct lsh_string *digits = lsh_string_alloc(4);
  WRITE_UINT32(digits->data, n);
  return sexp_s(NULL, digits);
}

struct lsh_string *
sexp_format(struct sexp *e, int style, unsigned indent)
{
  switch(style)
    {
    case SEXP_TRANSPORT:
      return encode_base64(sexp_format(e, SEXP_CANONICAL, 0), "{}", 1, indent, 1);
    case SEXP_CANONICAL:
    case SEXP_ADVANCED:
    case SEXP_INTERNATIONAL:
      /* NOTE: Check for NULL here? I don't think so. */
      return SEXP_FORMAT(e, style, indent);
    default:
      fatal("sexp_format: Unknown output style.\n");
    }
}

static void
encode_base64_group(UINT32 n, UINT8 *dest)
{
  static const UINT8 digits[64] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
    "ghijklmnopqrstuvwxyz0123456789+/";
  unsigned i;

  for (i = 0; i<4; i++)
    {
      dest[3 - i] = digits[n & 0x3f];
      n >>= 6;
    }
}

/* In pretty-print mode, print 40 characters or 10 groups on each
 * line. */
#define GROUPS_PER_LINE 10

struct lsh_string *
encode_base64(const struct lsh_string *s,
	      const char *delimiters,
	      int break_lines,
	      unsigned indent,
	      int free)				 
{
  UINT32 full_groups = (s->length) / 3;
  unsigned last = (s->length) % 3;

  /* We never fold after the last full group, so we don't fold strings
   * with full_groups <= GROUPS_PER_LINE. */
  unsigned linebreaks = ( (break_lines && full_groups)
			  ? (full_groups - 1) / GROUPS_PER_LINE
			  : 0);
  unsigned length = (full_groups + !!last) * 4 + (indent + 1) * linebreaks;

  const UINT8 *src = s->data;
  UINT8 *dst;
    
  struct lsh_string *res
    = (delimiters
       ? ssh_format("%c%lr%c", delimiters[0], length, &dst, delimiters[1])
       : ssh_format("%lr", length, &dst));
  
  if (full_groups)
    {
      unsigned i;
      
      /* Loop over all but the last group. */
      for (i=0; i<full_groups; dst += 4, src += 3, i++)
	{
	  if (break_lines && i && !(i % GROUPS_PER_LINE))
	    {
	      assert(linebreaks);
	      linebreaks--;
	      *dst++ = '\n';
	      memset(dst, ' ', indent);
	      dst += indent;
	    }
	  encode_base64_group( (src[0] << 16)
			       | (src[1] << 8)
			       | src[2], dst);
	}
    }

  assert(!linebreaks);
  
  switch(last)
    {
    case 0:
      /* Finished */
      break;
    case 1:
      encode_base64_group( src[0] << 16, dst);
      dst += 2;
      *dst++ = '=';
      *dst++ = '=';
      break;
    case 2:
      encode_base64_group( (src[0] << 16)
			   | (src[1] << 8), dst);
      dst += 3;
      *dst++ = '=';
      break;
    default:
      fatal("encode_base64: Internal error!\n");
    }

  assert( (dst + !!delimiters) == (res->data + res->length));

  if (free)
    lsh_string_free(s);
  
  return res;
}

int
sexp_nullp(const struct sexp *e)
{
  return (e == SEXP_NIL);
}

int
sexp_atomp(const struct sexp *e)
{
  return !e->iter;
}

/* Checks that the sexp is a simple string (i.e. no display part) */
const struct lsh_string *
sexp2string(struct sexp *e)
{
  return ( (e && sexp_atomp(e) && !sexp_display(e))
	   ? sexp_contents(e) : NULL);
}
  

int
sexp2atom(struct sexp *e)
{
  const struct lsh_string *s = sexp2string(e);
  return s ? lookup_atom(s->length, s->data) : 0;
}

/* If limit is nonzero, at most that number of octets are allowed. */
int
sexp2bignum_u(struct sexp *e, mpz_t n, UINT32 limit)
{
  const struct lsh_string *s = sexp2string(e);

  if (s)
    {
      if (limit && (s->length > limit))
	return 0;
      
      bignum_parse_u(n, s->length, s->data);
      return 1;
    }
  else
    return 0;
}

int
sexp2uint32(struct sexp *e, UINT32 *n)
{
  const struct lsh_string *digits = sexp2string(e);
  
  if (!digits)
    return 0;

  switch(digits->length)
    {
    case 0:
      return 0;
    case 1:
      *n = digits->data[0];
      return 1;
    case 2:
      *n = READ_UINT16(digits->data);
      return 1;
    case 3:
      *n = READ_UINT24(digits->data);
      return 1;
    case 4:
      *n = READ_UINT32(digits->data);
      return 1;
    default:
      {
	UINT32 i;
	UINT32 left;

	for (i = 0, left = digits->length;
	     left > 4;
	     i++, left--)
	  if (digits->data[i])
	    return 0;

	*n = READ_UINT32(digits->data + i);
	return 1;
      }
    }
}

int
sexp_eq(struct sexp *e, UINT32 length, const UINT8 *name)
{
  const struct lsh_string *c = sexp2string(e);

  return c && lsh_string_eq_l(c, length, name);
}

/* FIXME: sexp_atom_eq compares an sexp to a given atom, while
 *        sexp_atoms_eq compares two atomic sexps.
 *
 * This naming seems a little confusing. */
int
sexp_atom_eq(struct sexp *e, int atom)
{
  return sexp_eq(e, get_atom_length(atom), get_atom_name(atom));
}

/* Assumes that both expressions are atoms */
int
sexp_atoms_eq(struct sexp *a, struct sexp *b)
{
  const struct lsh_string *ac = sexp_contents(a);
  const struct lsh_string *ad = sexp_display(a);
  const struct lsh_string *bc = sexp_contents(b);
  const struct lsh_string *bd = sexp_display(b);

  return lsh_string_eq(ac, bc)
    && (ad ? (bd && lsh_string_eq(ad, bd))
	: !bd);
}

struct sexp *
sexp_assq(struct sexp_iterator *i, int atom)
{
  struct sexp_iterator *inner;
  if (SEXP_ASSOC(i, get_atom_length(atom), get_atom_name(atom), &inner)
      && (SEXP_LEFT(inner) == 1))
    {
      struct sexp *value = SEXP_GET(inner);
      assert(value);
      
      KILL(inner);
      return value;
    }
  else
    return NULL;
}

int
sexp_get_un(struct sexp_iterator *i, int atom, mpz_t n, UINT32 limit)
{
  return sexp2bignum_u(sexp_assq(i, atom), n, limit);
}

/* Command line options */
struct sexp_format
{
  char *name;
  int id;
};

static const struct sexp_format
sexp_formats[] =
{
  { "transport", SEXP_TRANSPORT },
  { "canonical", SEXP_CANONICAL },
  { "advanced", SEXP_ADVANCED },
  { "international", SEXP_INTERNATIONAL },
  { NULL, 0 }
};

static int
lookup_sexp_format(const char *name)
{
  int i;

  for (i = 0; sexp_formats[i].name; i++)
    {
      if (strcmp(sexp_formats[i].name, name) == 0)
	return sexp_formats[i].id;
    }
  return -1;
}

static const struct argp_option
sexp_input_options[] =
{
  { NULL, 0, NULL, 0, "Valid sexp-formats are transport, canonical, "
    "advanced and international.", 0 },
  { "input-format", 'i', "format", 0,
    "Variant of the s-expression syntax to accept.", 0},
  { NULL, 0, NULL, 0, NULL, 0 }
};

static const struct argp_option
sexp_output_options[] =
{
  { NULL, 0, NULL, 0, "Valid sexp-formats are transport, canonical, "
    "advanced and international.", 0 },
  { "output-format", 'f', "format", 0,
    "Variant of the s-expression syntax to generate.", 0},
  { NULL, 0, NULL, 0, NULL, 0 }
};

static error_t
sexp_argp_parser(int key, char *arg, struct argp_state *state)
{
  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case 'f':
    case 'i':
      {
	int format = lookup_sexp_format(arg);
	if (format < 0)
	  argp_error(state, "Unknown s-expression format '%s'", arg);
	else
	  *(sexp_argp_state *) (state->input) = format;

	break;
      }
    }
  return 0;
}

const struct argp
sexp_input_argp =
{
  sexp_input_options,
  sexp_argp_parser,
  NULL, NULL, NULL, NULL, NULL
};

const struct argp
sexp_output_argp =
{
  sexp_output_options,
  sexp_argp_parser,
  NULL, NULL, NULL, NULL, NULL
};

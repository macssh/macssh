/* charset.c
 *
 * Translate local characterset to and from utf8.
 *
 * $Id$
 */

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

#include "charset.h"

#include "format.h"  /* For lsh_string_dup() */
#include "parse.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

static int local_charset;

void set_local_charset(int charset)
{
  local_charset = charset;
}

UINT32 local_to_ucs4(int c)
{
  switch (local_charset)
    {
    case CHARSET_USASCII:
    case CHARSET_LATIN1:
      return (UINT32) c;
    default:
      fatal("Internal error");
    };
}

/* NOTE: This function does not filter any control characters */
int ucs4_to_local(UINT32 c)
{
  switch (local_charset)
    {
    case CHARSET_USASCII:
      return (c < 0x80) ? (int) c : -1;
    case CHARSET_LATIN1:
      return (c < 0x100) ? (int) c : -1;
    default:
      fatal("Internal error");
    };
}

struct lsh_string *
local_to_utf8(struct lsh_string *s, int free)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  switch (local_charset)
    {
    case CHARSET_UTF8:
    case CHARSET_USASCII:
      return s;
    default:
      {
	UINT32 *chars = alloca(s->length * sizeof(UINT32));
	unsigned char *lengths = alloca(s->length);

	UINT32 total = 0;
	{
	  UINT32 i;
	
	  /* First convert to ucs-4, and compute the length of the corresponding
	   * utf-8 string. */
	  for (i = 0; i<s->length; i++)
	    {
	      UINT32 c = local_to_ucs4(s->data[i]);
	      unsigned char l = 1;

	      if (c >= (1UL<<7))
		{
		  l++;
		  if (c >= (1UL<<11))
		    {
		      l++;
		      if (c >= (1UL<<16))
			{
			  l++;
			  if (c >= (1UL<<21))
			    {
			      l++;
			      if (c >= (1UL<<25))
				{
				  l++;
				  if (c >= (1UL<<31))
				    fatal("Internal error!\n");
				}}}}}
	      chars[i] = c;
	      lengths[i] = l;
	      total += l;
	    }
	}
	{
	  struct lsh_string *res = lsh_string_alloc(total);
	  UINT32 i, j;

	  for(i = j = 0; i<s->length; i++)
	    {
	      static const UINT8 prefix[]
		= {0, 0xC0, 0xE0, 0xF0, 0xF8, 0XFC };
	      
	      UINT32 c = chars[i];
	      unsigned char l = lengths[i] - 1;
	      int k;
	      
	      for (k = l; k; k--)
		{
		  res->data[j+k] = 0x80 | (c & 0x3f);
		  c >>= 6;
		}
	      assert( !(prefix[l] & c) );
	      res->data[j] = prefix[l] | c;

	      j += lengths[i];
	    }
	  assert(j == total);

	  if (free)
	    lsh_string_free(s);

#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return res;
	}
      }
    }
}

int local_is_utf8(void) { return (local_charset == CHARSET_UTF8); }

struct lsh_string *low_utf8_to_local(UINT32 length, UINT8 *s, int strict)
{
  UINT32 i;
  struct lsh_string *res;
  struct simple_buffer buffer;
  
  assert(!local_is_utf8());

  /* The string can't grow when converted to local charset */
  res = lsh_string_alloc(length);

  simple_buffer_init(&buffer, length, s);

  for (i = 0; 1; i++)
    {
      UINT32 ucs4;

      switch(parse_utf8(&buffer, &ucs4))
	{
	case -1:
	  assert(i<=res->length);
	  
	  res->length = i;

	  return res;

	case 1:
	  {
	    int local = ucs4_to_local(ucs4);

	    if (local >= 0)
	      {
		res->data[i] = local;
		break;
	      }
	    else if (!strict)
	      {
		/* Replace unkonwn characters. */
		res->data[i] = '?';
		break;
	      }
	    /* Fall through */
	  }
	case 0: /* Error */
	  lsh_string_free(res);

	  return NULL;

	default:

	  fatal("Internal error!\n");
	}
    }
}

struct lsh_string *utf8_to_local(struct lsh_string *s, int strict, int free)
{
  struct lsh_string *res;
  
  if (local_is_utf8())
    return free ? s : lsh_string_dup(s);

  res = low_utf8_to_local(s->length, s->data, strict);

  if (free)
    lsh_string_free(s);

  return res;
}

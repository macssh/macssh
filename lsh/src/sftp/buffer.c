/* buffer.c
 *
 * $Id$
 *
 * Buffering for sftp.
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2001 Niels Möller
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */

#include "buffer.h"

#include "xmalloc.h"
#include "sftp.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define FATAL(x) do { fputs("sftp-server: " x "\n", stderr); exit(EXIT_FAILURE); } while (0)

#include <stdlib.h>

/* Reads a 32-bit integer, in network byte order */
#define READ_UINT32(p)				\
(  (((UINT32) (p)[0]) << 24)			\
 | (((UINT32) (p)[1]) << 16)			\
 | (((UINT32) (p)[2]) << 8)			\
 |  ((UINT32) (p)[3]))

#define WRITE_UINT32(p, i)			\
do {						\
  (p)[0] = ((i) >> 24) & 0xff;			\
  (p)[1] = ((i) >> 16) & 0xff;			\
  (p)[2] = ((i) >> 8) & 0xff;			\
  (p)[3] = (i) & 0xff;				\
} while(0)

#define SFTP_MAX_STRINGS 2

struct sftp_input
{
  FILE *f;
  UINT32 left;

  /* Strings that we own */
  UINT8 *strings[SFTP_MAX_STRINGS];
  unsigned used_strings;
};

struct sftp_output
{
  FILE *f;

  /* The message type is the first byte of a message, after the
   * length. */
  UINT8 msg;

  /* The next word is either the id, or the version. */
  UINT32 first;

  /* The rest of the packet is variable length. */
  UINT8 *data;
  UINT32 size;
  UINT32 i;
};

/* Input */

static int
sftp_check_input(const struct sftp_input *i, UINT32 length)
{
  return (i->left >= length);
}

int
sftp_get_data(struct sftp_input *i, UINT32 length, UINT8 *data)
{
  if (sftp_check_input(i, length))
    {
      i->left -= length;
      return (fread(data, 1, length, i->f) == length);
    }
  return 0;
}

#define GET_DATA(i, buf) \
(sftp_get_data((i), sizeof((buf)), (buf)))

int
sftp_get_uint8(struct sftp_input *i, UINT8 *value)
{
  return sftp_get_data(i, 1, value);    
}

int
sftp_get_uint32(struct sftp_input *i, UINT32 *value)
{
  UINT8 buf[4];
  if (!GET_DATA(i, buf))
    return 0;

  *value = READ_UINT32(buf);
  return 1;
}

UINT8 *
sftp_get_string(struct sftp_input *i, UINT32 *length)
{
  UINT8 *data;

  if (!(sftp_get_uint32(i, length) && sftp_check_input(i, *length)))
    return NULL;

  data = xmalloc(*length + 1);

  if (!sftp_get_data(i, *length, data))
    {
      free(data);
      return NULL;
    }

  /* NUL-terminate, for convenience */
  data[*length] = '\0';

  return data;
}

void
sftp_free_string(UINT8 *s)
{
  free(s);
}

UINT8 *
sftp_get_string_auto(struct sftp_input *i, UINT32 *length)
{
  UINT8 *data;

  assert(i->used_strings < SFTP_MAX_STRINGS);

  data = sftp_get_string(i, length);

  if (!data)
    return NULL;

  /* Remember the string. */
  i->strings[i->used_strings++] = data;
  return data;
}

int
sftp_get_eod(struct sftp_input *i)
{
  return !i->left;
}

/* Input */
struct sftp_input *
sftp_make_input(FILE *f)
{
  struct sftp_input *i = xmalloc(sizeof(struct sftp_input));

  i->f = f;
  i->left = 0;
  i->used_strings = 0;

  return i;
}

void
sftp_input_clear_strings(struct sftp_input *i)
{
  unsigned k;

  for (k = 0; k < i->used_strings; k++)
    sftp_free_string(i->strings[k]);

  i->used_strings = 0;
}

/* Returns 1 of all was well, 0 on error, and -1 on EOF */
int
sftp_read_packet(struct sftp_input *i)
{
  UINT8 buf[4];
  int done;

  if (i->left)
    return 0;

  /* First, deallocate the strings. */
  sftp_input_clear_strings(i);
  
  done = fread(buf, 1, sizeof(buf), i->f);

  switch (done)
    {
    case 0:
      return feof(i->f) ? -1 : 0;
    case 4:
      i->left = READ_UINT32(buf);
      return 1;
    default:
      return 0;
    }
}

/* Output */

static void
sftp_check_output(struct sftp_output *o, UINT32 length)
{
  UINT32 needed = o->i + length;
  if (!o->data || (needed > o->size))
  {
    UINT32 size = 2 * needed + 40;
    o->data = xrealloc(o->data, size);

    o->size = size;
  }
}

void
sftp_put_data(struct sftp_output *o, UINT32 length, const UINT8 *data)
{
  sftp_check_output(o, length);

  memcpy(o->data + o->i, data, length);
  o->i += length;
}

#define PUT_DATA(o, buf) \
(sftp_put_data((o), sizeof((buf)), (buf)))

void
sftp_put_uint8(struct sftp_output *o, UINT8 value)
{
  sftp_check_output(o, 1);

  o->data[o->i++] = value;
}

void
sftp_put_uint32(struct sftp_output *o, UINT32 value)
{
  UINT8 buf[4];

  WRITE_UINT32(buf, value);
  PUT_DATA(o, buf);
}

void
sftp_put_string(struct sftp_output *o, UINT32 length, const UINT8 *data)
{
  sftp_put_uint32(o, length);
  sftp_put_data(o, length, data);
}

UINT32
sftp_put_reserve_length(struct sftp_output *o)
{
  UINT32 index;
  sftp_check_output(o, 4);

  index = o->i;
  o->i += 4;

  return index;
}

void
sftp_put_length(struct sftp_output *o,
		UINT32 index,
		UINT32 length)
{
  assert( (index + 4) < o->i);
  WRITE_UINT32(o->data + index, length);
}

void
sftp_put_final_length(struct sftp_output *o,
		      UINT32 index)
{
  sftp_put_length(o, index, o->i - index - 4);
}

void
sftp_put_reset(struct sftp_output *o,
	       UINT32 index)
{
  assert(index < o->i);
  o->i = index;
}

UINT32
sftp_put_printf(struct sftp_output *o, const char *format, ...)
{
  /* Initial buffer space */
  int needed;
  int length;
  
  for (needed = 100;; needed *= 2)
    {
      va_list args;
      
      va_start(args, format);

      sftp_check_output(o, needed);
      length = vsnprintf(o->data + o->i, needed, format, args);
      
      va_end(args);

      if ( (length >= 0) && (length < needed))
	break;
    }
  o->i += length;
  
  return length;
}

/* If SIZE > 0 it is the desired field length, and
 * smaller output is padded with blanks. */
UINT32
sftp_put_strftime(struct sftp_output *o, UINT32 size, const char *format,
		  const struct tm *tm)
{
  /* Initial buffer space */
  size_t needed;
  size_t length;
  
  for (needed = size ? size : 100;; needed *= 2)
    {
      sftp_check_output(o, needed);
      length = strftime(o->data + o->i, needed, format, tm);

      if ( (length > 0) && (length < needed))
	break;
    }

  while (length < size)
    o->data[o->i + length++] = ' ';

  o->i += length;
  
  return length;
}


/* 64-bit stuff */
#if SIZEOF_OFF_T > 4
#define READ_UINT64(p)				\
(  (((UINT64) (p)[0]) << 56)			\
 | (((UINT64) (p)[1]) << 48)			\
 | (((UINT64) (p)[2]) << 40)			\
 | (((UINT64) (p)[3]) << 32)			\
 | (((UINT64) (p)[4]) << 24)			\
 | (((UINT64) (p)[5]) << 16)			\
 | (((UINT64) (p)[6]) << 8)			\
 |  ((UINT64) (p)[7]))


int
sftp_get_uint64(struct sftp_input *i, off_t *value)
{
  UINT8 buf[8];
  if (!GET_DATA(i, buf))
    return 0;

  *value = READ_UINT64(buf);
  return 1;
}

#define WRITE_UINT64(p, i)			\
do {						\
  (p)[0] = ((i) >> 56) & 0xff;			\
  (p)[1] = ((i) >> 48) & 0xff;			\
  (p)[2] = ((i) >> 40) & 0xff;			\
  (p)[3] = ((i) >> 32) & 0xff;			\
  (p)[4] = ((i) >> 24) & 0xff;			\
  (p)[5] = ((i) >> 16) & 0xff;			\
  (p)[6] = ((i) >> 8) & 0xff;			\
  (p)[7] = (i) & 0xff;				\
} while(0)

void
sftp_put_uint64(struct sftp_output *o, off_t value)
{
  UINT8 buf[8];

  WRITE_UINT64(buf, value);
  PUT_DATA(o, buf);
}

#else /* SIZEOF_OFF_T <= 4 */

/* Fail for too large numbers. */
int
sftp_get_uint64(struct sftp_input *i, off_t *value)
{
  UINT32 high;
  UINT32 low;

  if (sftp_get_uint32(i, &high)
      && !high
      && sftp_get_uint32(i, &low))
    {
      *value = low;
      return 1;
    }
  else
    return 0;
}

void
sftp_put_uint64(struct sftp_output *o, off_t value)
{
  sftp_put_uint32(o, 0);
  sftp_put_uint32(o, value);
}

#endif /* SIZEOF_OFF_T <= 4 */

/* The first part of the buffer is always
 *
 * uint32 length
 * uint8  msg
 * uint32 id/version
 */

struct sftp_output *
sftp_make_output(FILE *f)
{
  struct sftp_output *o = xmalloc(sizeof(struct sftp_output));

  o->f = f;
  o->data = NULL;
  o->size = 0;
  o->i = 0;

  return o;
}

void
sftp_set_msg(struct sftp_output *o, UINT8 msg)
{
  o->msg = msg;
}

void
sftp_set_id(struct sftp_output *o, UINT32 id)
{
  o->first = id;
}

int
sftp_write_packet(struct sftp_output *o)
{
  UINT32 length = o->i + 5;
  UINT8 buf[9];

  WRITE_UINT32(buf, length);
  buf[4] = o->msg;
  WRITE_UINT32(buf + 5, o->first);

  if (fwrite(buf, 1, 9, o->f) != 9)
    return 0;
  if (fwrite(o->data, 1, o->i, o->f) != o->i)
    return 0;

  o->i = 0;

  /* FIXME: Flushing after each packet is sub-optimal. */
  if (fflush(o->f))
    return 0;

  return 1;
}

/* General functions */

void
sftp_clear_attrib(struct sftp_attrib *a)
{
  a->flags = 0;
  a->size = 0;
  a->uid = 0;
  a->gid = 0;
  a->permissions = 0;
  a->atime = 0;
  a->mtime = 0;
};

int
sftp_skip_extension(struct sftp_input *i)
{
  UINT32 length;
  UINT8 *data;
  unsigned j;
  
  /* Skip name and value*/
  for (j = 0; j<2; j++)
    {
      if (!(data = sftp_get_string(i, &length)))
	return 0;
    }
  sftp_input_clear_strings(i);
  
  return 1;
}

int
sftp_get_attrib(struct sftp_input *i, struct sftp_attrib *a)
{
  sftp_clear_attrib(a);
  
  if (!sftp_get_uint32(i, &a->flags))
    return 0;

  if (a->flags & SSH_FILEXFER_ATTR_SIZE)
    {
      if (!sftp_get_uint64(i, &a->size))
	return 0;
    }

  if (a->flags & SSH_FILEXFER_ATTR_UIDGID)
    {
      if (!sftp_get_uint32(i, &a->uid))
	return 0;

      if (!sftp_get_uint32(i, &a->gid))
	return 0;
    }

  if (a->flags & SSH_FILEXFER_ATTR_PERMISSIONS)
    {
      if (!sftp_get_uint32(i, &a->permissions))
	return 0;
    }

  if (a->flags & SSH_FILEXFER_ATTR_ACMODTIME)
    {
      if (!sftp_get_uint32(i, &a->atime))
	return 0;

      if (!sftp_get_uint32(i, &a->mtime))
	return 0;
    }

  if (a->flags & SSH_FILEXFER_ATTR_EXTENDED)
    {
      UINT32 count;
      UINT32 n;

      if (!sftp_get_uint32(i, &count))
	return 0;

      /* Just skip the extensions */
      for (n = 0; n < count; n++)
	if (!sftp_skip_extension(i))
	  return 0;
    }
  return 1;
}

void
sftp_put_attrib(struct sftp_output *o, const struct sftp_attrib *a)
{
  assert(!(a->flags & SSH_FILEXFER_ATTR_EXTENDED));
  
  sftp_put_uint32(o, a->flags);

  if (a->flags & SSH_FILEXFER_ATTR_SIZE)
    sftp_put_uint64(o, a->size);

  if (a->flags & SSH_FILEXFER_ATTR_UIDGID)
    {
      sftp_put_uint32(o, a->uid);
      sftp_put_uint32(o, a->gid);
    }

  if (a->flags & SSH_FILEXFER_ATTR_PERMISSIONS)
    sftp_put_uint32(o, a->permissions);

  if (a->flags & SSH_FILEXFER_ATTR_ACMODTIME)
    {
      sftp_put_uint32(o, a->atime);
      sftp_put_uint32(o, a->mtime);
    }
}

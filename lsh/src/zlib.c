/* zlib.c
 *
 * zlib compression algorithm
 * 
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Balázs Scheidler, Niels Möller
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

#include "compress.h"
#include "format.h"
#include "ssh.h"
#include "string_buffer.h"
#include "werror.h"
#include "xalloc.h"

#if WITH_ZLIB

#if HAVE_ZLIB_H
#include <zlib.h>
#else
#warning zlib.h not present
#endif

#include <assert.h>

static void do_free_zstream(z_stream *z);
  
 
//#define verbose2 verbose
//#define werror2 werror
#define verbose2 debug
#define werror2 debug

#include "zlib.c.x"

/* GABA:
   (class
     (name zlib_instance)
     (super compress_instance)
     (vars
       ; Fail before producing larger packets than this
       (max . UINT32)
       (rate . UINT32)
       (f pointer (function int "z_stream *" int))
       (z indirect-special z_stream
          #f do_free_zstream)))
*/

/* GABA:
   (class
     (name zlib_algorithm)
     (super compress_algorithm)
     (vars
       (level . int)))
*/

/* Stored in the opaque pointer. */
struct zlib_type
{
  int (*free_func)(z_stream *z);
  const char *operation;
};

#define ZLIB_TYPE(z) ((struct zlib_type *)((z)->opaque))

static const struct zlib_type
zlib_inflate = {  inflateEnd, "inflate" };

static const struct zlib_type
zlib_deflate = {  inflateEnd, "deflate" };

/* zlib memory functions */
void *
zlib_alloc(void *opaque UNUSED, unsigned int items, unsigned int size)
{
  return lsh_space_alloc(items * size);
}

void
zlib_free(void *opaque UNUSED, void *address)
{
  lsh_space_free(address);
}

static void
do_free_zstream(z_stream *z)
{
  /* We use the opaque pointer, as there's nothing else to help us
   * figure if we should be calling inflateEnd or deflateEnd. */

  const struct zlib_type *type = ZLIB_TYPE(z);
  
  int res = type->free_func(z);

  if (res != Z_OK)
    debug("do_free_zstream (%z): Freeing failed: %z\n",
	  type->operation, z->msg ? z->msg : "No error");
}

/* Estimates of the resulting packet sizes. We use fixnum arithmetic,
 * with one represented as 1<<10=1024. Only rates between 1/16 and 16
 * are used. This may be a little too conservative; I have observed
 * compression ratios of about 50. */
 
#define RATE_UNIT 1024
#define RATE_MAX (RATE_UNIT * 16)
#define RATE_MIN (RATE_UNIT / 16)
#define MARGIN 200
#define INSIGNIFICANT 100

static UINT32 estimate_size(UINT32 rate, UINT32 input, UINT32 max)
{
  UINT32 guess = rate * input / RATE_UNIT + MARGIN;
  return MIN(max, guess);
}

/* Assumes that input is nonzero */
static UINT32 estimate_update(UINT32 rate, UINT32 input, UINT32 output)
{
  /* Decay old estimate */
  rate = rate * 15 / 16;

  /* FIXME: Following the envelope is suboptimal for small inputs. We
   * do it only for input packets of reasonable size. This method
   * could be improved.
   *
   * Perhaps a linear combination k * rate + (1-k) estimate, where k
   * depends on the size of the sample (i.e. input) would make sense?
   * Or use different rate estimates for different lengths? */
  
  if (input > INSIGNIFICANT)
    {
      UINT32 estimate = output * RATE_UNIT / input;

      if (estimate > RATE_MAX)
	return RATE_MAX;

      /* Follow the "envelope" */
      rate = MAX(estimate, rate);
    }
  
  return MAX(rate, RATE_MIN);
}

/* Compress incoming data */
static struct lsh_string *
do_zlib(struct compress_instance *c,
	struct lsh_string *packet,
	int free)
{
  CAST(zlib_instance, self, c);
  struct string_buffer buffer;
  
  /* LIMIT keeps track of the amount of storage we may still need to
   * allocate. To detect that a packet grows unexpectedly large, we
   * need a little extra buffer space beyond the maximum size. */
  UINT32 limit = self->max + 1;

  UINT32 estimate;
  
  debug("do_zlib (%z): length in: %i\n",
	ZLIB_TYPE(&self->z)->operation, packet->length);
  
  if (!packet->length)
    {
      werror("do_zlib (%z): Compressing empty packet.\n",
	     ZLIB_TYPE(&self->z)->operation);
      return free ? packet : lsh_string_dup(packet);
    }

  estimate = estimate_size(self->rate, packet->length, limit);
  debug("do_zlib (%z): estimate:  %i\n",
	ZLIB_TYPE(&self->z)->operation,
	estimate);

  string_buffer_init(&buffer, estimate);

  limit -= buffer.partial->length;

  self->z.next_in = packet->data;
  self->z.avail_in = packet->length;

  for (;;)
    {
      int rc;
      
      self->z.next_out = buffer.current;
      self->z.avail_out = buffer.left;

      assert(self->z.avail_out);
      
      rc = self->f(&self->z, Z_SYNC_FLUSH);

      switch (rc)
	{
	case Z_BUF_ERROR:
	  werror2("do_zlib (%z): Z_BUF_ERROR (probably harmless),\n"
		 "  avail_in = %i, avail_out = %i\n",
		 ZLIB_TYPE(&self->z)->operation,
		 self->z.avail_in, self->z.avail_out);
	  /* Fall through */
	case Z_OK:
	  break;
	default:
	  werror("do_zlib: %z() failed: %z\n",
		 ZLIB_TYPE(&self->z)->operation,
		 self->z.msg ? self->z.msg : "No error(?)");
	  if (free)
	    lsh_string_free(packet);
	  
	  return NULL;
	}
      
      /* NOTE: It's not enough to check that avail_in is zero to
       * determine that all data have been flushed. avail_in == 0 and
       * avail_out > 0 implies that all data has been flushed, but if
       * avail_in == avail_out == 0, we have to allocate more output
       * space. */
	 
      if (!self->z.avail_in && !self->z.avail_out)
	verbose2("do_zlib (%z): Both avail_in and avail_out are zero.\n",
		ZLIB_TYPE(&self->z)->operation);
      
      if (!self->z.avail_out)
	{ /* All output space consumed */  
	  if (!limit)
	    {
	      werror("do_zlib (%z): Packet grew too large!\n",
		     ZLIB_TYPE(&self->z)->operation);
	      
	      if (free)
		lsh_string_free(packet);

	      string_buffer_clear(&buffer);
	      return NULL;
	    }

	  /* Grow to about double size. */
	  string_buffer_grow(&buffer, MIN(limit, buffer.partial->length + buffer.total + 100));
	  limit -= buffer.partial->length;
	}
      else if (!self->z.avail_in)
	{ /* Compressed entire packet */
	  UINT32 input = packet->length;

	  if (free)
	    lsh_string_free(packet);
	  
	  packet =
	    string_buffer_final(&buffer, self->z.avail_out);

	  assert(packet->length <= self->max);

	  debug("do_zlib (%z): length out: %i\n",
		ZLIB_TYPE(&self->z)->operation,
		packet->length);

	  if (packet->length > estimate)
	    verbose2("do_zlib (%z): Estimated size exceeded: input = %i, estimate = %i, output = %i\n",
		    ZLIB_TYPE(&self->z)->operation,
		    input, estimate, packet->length);
	  
	  self->rate = estimate_update(self->rate, input, packet->length);

	  return packet;
	}
    }
}

static struct compress_instance *
make_zlib_instance(struct compress_algorithm *c, int mode)
{
  CAST(zlib_algorithm, closure, c);
  NEW(zlib_instance, res);

  res->z.zalloc = zlib_alloc;
  res->z.zfree = zlib_free;

  switch (mode)
    {
      case COMPRESS_DEFLATE:
	res->z.opaque = (void *) &zlib_deflate;
	res->f = deflate;
        res->super.codec = do_zlib;

	/* Maximum expansion is 0.1% + 12 bytes. We use 1% + 12, to be conservative.
	 *
	 * FIXME: These figures are documented for the entire stream,
	 * does they really apply to all segments, separated by
	 * Z_SYNC_FLUSH ? */

	res->max = SSH_MAX_PACKET + SSH_MAX_PACKET / 100 + 12;
	res->rate = RATE_UNIT;
	
        deflateInit(&res->z, closure->level);
        break;

    case COMPRESS_INFLATE:
	res->z.opaque = (void *) &zlib_inflate;
	res->f = inflate;
        res->super.codec = do_zlib;

	/* FIXME: Perhaps we ought to use the connection's
	 * rec_max_packet size? */
	res->max = SSH_MAX_PACKET;
	res->rate = 2 * RATE_UNIT;
	
        inflateInit(&res->z);
        break;
    }
  return &res->super;
}

struct compress_algorithm *make_zlib_algorithm(int level)
{
  if ( (level != Z_DEFAULT_COMPRESSION)
       && ( (level < Z_NO_COMPRESSION)
	    || (level > Z_BEST_COMPRESSION) ))
    return NULL;
  else
    {
      NEW(zlib_algorithm, closure);
      
      closure->super.make_compress = make_zlib_instance;
      closure->level = level;

      return &closure->super;
    }
}

struct compress_algorithm *make_zlib(void)
{
  return make_zlib_algorithm(Z_DEFAULT_COMPRESSION);
}

#endif /* WITH_ZLIB */

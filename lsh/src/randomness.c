/* randomness.c
 *
 *
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, 2000, Niels Möller
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

#include "randomness.h"

#include "werror.h"

#include "crypto.h"
#include "exception.h"
#include "xalloc.h"

#include "nettle/arcfour.h"

#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <string.h>

#define GABA_DEFINE
#include "randomness.h.x"
#undef GABA_DEFINE

#include "randomness.c.x"

#ifdef MACOS
#include <Events.h>
#ifdef __powerc
// The timebase is available through mfspr on 601, mftb on later chips.
// Motorola recommends that an 601 implementation map mftb to mfspr
// through an exception, but I haven't tested to see if MacOS actually
// does this. We only sample the lower 32 bits of the timer (i.e. a
// few minutes of resolution)
static unsigned long GetPPCTimer();
static asm unsigned long GetPPCTimer()
{
	mftb  	r3		// Available on 603 and later.
}
#endif
#endif

/* Random */
/* GABA:
   (class
     (name poor_random)
     (super randomness)
     (vars
       (hash object hash_instance)
       (pos . UINT32)
       (buffer space UINT8)))
*/

static void
do_poor_random(struct randomness *r, UINT32 length, UINT8 *dst)
{
  CAST(poor_random, self, r);

  while(length)
    {
      UINT32 available = self->hash->hash_size - self->pos;
      UINT32 to_copy;
      
      if (!available)
	{
	  time_t now = time(NULL); /* To avoid cycles */

#if MACOS
#ifdef __powerc
	  unsigned long tick = GetPPCTimer();
#else
	  unsigned long tick = (pid_t)TickCount();
#endif
	  Point mouseLoc;
	  HASH_UPDATE(self->hash, sizeof(unsigned long), (UINT8 *) &tick);
	  GetMouse(&mouseLoc);
	  HASH_UPDATE(self->hash, sizeof(mouseLoc), (UINT8 *) &mouseLoc);
#endif
	  HASH_UPDATE(self->hash, sizeof(now), (UINT8 *) &now);
	  HASH_UPDATE(self->hash, self->hash->hash_size,
		      self->buffer);
	  HASH_DIGEST(self->hash, self->buffer);

	  available = self->hash->hash_size;
	  self->pos = 0;
	}
      to_copy = MIN(available, length);

      memcpy(dst, self->buffer + self->pos, to_copy);
      length -= to_copy;
      dst += to_copy;
      self->pos += to_copy;
    }
}

struct randomness *
make_poor_random(struct hash_algorithm *hash,
		 struct lsh_string *init)
{
  NEW(poor_random, self);
  time_t now = time(NULL); 
#ifndef MACOS
  pid_t pid = getpid();
#else
#ifdef __powerc
  pid_t pid = GetPPCTimer();
#else
  pid_t pid = (pid_t)TickCount();
#endif
  Point mouseLoc;
#endif
  
  self->super.random = do_poor_random;
  self->super.quality = 0;
  
  self->hash = MAKE_HASH(hash);
  self->buffer = lsh_space_alloc(hash->hash_size);
  
  HASH_UPDATE(self->hash, sizeof(now), (UINT8 *) &now);
  HASH_UPDATE(self->hash, sizeof(pid), (UINT8 *) &pid);
#ifdef MACOS
  GetMouse(&mouseLoc);
  HASH_UPDATE(self->hash, sizeof(mouseLoc), (UINT8 *) &mouseLoc);
#endif
  
  if (init)
    {
      HASH_UPDATE(self->hash, init->length, init->data);
      lsh_string_free(init);
    }
  HASH_DIGEST(self->hash, self->buffer);

  self->pos = 0;

  return &self->super;
}

struct randomness *make_bad_random()
{
  return make_poor_random(&sha1_algorithm, NULL);
}

/* GABA:
   (class
     (name device_random)
     (super randomness)
     (vars
       (fd . int)))
*/

static void
do_device_random(struct randomness *r, UINT32 length, UINT8 *dst)
{
  CAST(device_random, self, r);

  while(length)
    {
      int n = read(self->fd, dst, length);

      if (!n)
	fatal("do_device_random: EOF on random source.\n");

      if (n<0)
	switch(errno)
	  {
	  case EINTR:
	    break;
	  default:
	    fatal("Read from random device failed (errno = %i): %z\n",
		  errno, STRERROR(errno));
	  }
      else
	{
	  length -= n;
	  dst += n;
	}
    }
}

/* NOTE: In most cases, blocking while waiting for more entropy to
 * arrive is not acceptable. So use /dev/urandom, not /dev/random. The
 * alternative is to read a smaller seed from /dev/random at startup,
 * and use an internal pseudorandom generator. That
 * would be friendlier to other applications, but would not buy as
 * more security, as /dev/urandom should degenerate to a fairly strong
 * pseudorandom generator when it runs out of entropy. */

struct randomness *
make_device_random(const char *device)
{
  int fd = open(device, O_RDONLY);

  if (fd < 0)
    {
      werror("make_device_random: Failed to open '%z' (errno = %i): %z\n",
	     device, errno, STRERROR(errno));
      return NULL;
    }
  else
    {
      NEW(device_random, self);
      
      self->super.random = do_device_random;

      /* The quality depends on the used device. */
      self->super.quality = 2;
      self->fd = fd;
      
      return &self->super;
    }
}

/* GABA:
   (class
     (name arcfour_random)
     (super randomness_with_poll)
     (vars
       (e object exception_handler)
       
       ; The pool that is used to create the output bytes
       (pool . "struct arcfour_ctx")

       ; Accumulate randomness here before it is added to the main
       ; pool
       (staging_area object hash_instance)
       (staging_count . unsigned)))
*/

#define STAGE_THRESHOLD 100

static void
do_arcfour_random(struct randomness *r, UINT32 length, UINT8 *dst)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  CAST(arcfour_random, self, r);
  
  self->staging_count += RANDOM_POLL_FAST(self->super.poller, self->staging_area);

  if (self->staging_count > STAGE_THRESHOLD)
    {
      /* Pour the collected randomness into the pool */
      UINT8 *buf = alloca(self->staging_area->hash_size);

      verbose("do_arcfour_random: Pouring staging area into pool.\n");
      
      /* Get some data out of the pool, in order to keep any entropy
       * there. */
      arcfour_stream(&self->pool, self->staging_area->hash_size, buf);

      HASH_UPDATE(self->staging_area, self->staging_area->hash_size, buf);
      
      HASH_DIGEST(self->staging_area, buf);
      arcfour_set_key(&self->pool, self->staging_area->hash_size, buf);

      self->staging_count = 0;
    }

  arcfour_stream(&self->pool, length, dst);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

static void
do_arcfour_random_slow(struct randomness *r, UINT32 length, UINT8 *dst)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  CAST(arcfour_random, self, r);

  unsigned count = RANDOM_POLL_SLOW(self->super.poller, self->staging_area);

  debug("arcfour_random: entropy estimate for initialization: %i bits.\n",
	count);
  
  if (count < STAGE_THRESHOLD)
    {
      const struct exception low_entropy =
	STATIC_EXCEPTION(EXC_RANDOMNESS_LOW_ENTROPY,
			 "Could not get enough entropy from the environment.");
      EXCEPTION_RAISE(self->e, &low_entropy);
    }
  else
    self->super.super.quality = 1;
  
  {
    /* Initialize the pool. */
    UINT8 *buf = alloca(self->staging_area->hash_size);
    
    verbose("do_arcfour_random_slow: Initalizing randomness pool.\n");
    HASH_DIGEST(self->staging_area, buf);
    arcfour_set_key(&self->pool, self->staging_area->hash_size, buf);

    self->staging_count = 0;
  }
  
  self->super.super.random = do_arcfour_random;

  arcfour_stream(&self->pool, length, dst);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

struct randomness_with_poll *
make_arcfour_random(struct random_poll *poller,
		    struct hash_algorithm *hash,
		    struct exception_handler *e)
{
  NEW(arcfour_random, self);
  self->super.super.random = do_arcfour_random_slow;
  self->super.super.quality = 0;

  self->super.poller = poller;
  self->e = e;
  
  self->staging_area = MAKE_HASH(hash);
  self->staging_count = 0;
  
  return &self->super;
}

#if 0
struct randomness *
make_reasonably_random(void)
{
  struct randomness *r = make_device_random("/dev/urandom");

  if (r)
    r->quality = 1;
  else
  {
    werror("Warning: Falling back to an insecure pseudorandom generator.\n");
    r = make_poor_random(&sha1_algorithm, NULL);
  }
  return r;
}
#endif

struct randomness_with_poll *
make_default_random(struct reap *reaper,
		    struct exception_handler *e)
{
  struct random_poll *poller = make_unix_random(reaper);

  /* FIXME: Move away from using rc4. */
  return make_arcfour_random(poller, &sha1_algorithm, e);
}


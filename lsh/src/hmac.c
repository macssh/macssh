/* hmac.c
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

#include "crypto.h"

#include "xalloc.h"

#include <string.h>

#include "hmac.c.x"

#if !HAVE_MEMXOR
#include "memxor.h"
#endif

/* The HMAC (rfc-2104)  construction */
/* GABA:
   (class
     (name hmac_algorithm)
     (super mac_algorithm)
     (vars
       (hash object hash_algorithm)))
*/

/* GABA:
   (class
     (name hmac_instance)
     (super mac_instance)
     (vars
       ; Initialized hash objects 
       (hinner object hash_instance)
       (houter object hash_instance)

       ; Modified by update.
       (state object hash_instance)))
*/

static void
do_hmac_update(struct mac_instance *s,
	       UINT32 length, const UINT8 *data)
{
  CAST(hmac_instance, self, s);

  HASH_UPDATE(self->state, length, data);
}

static void
do_hmac_digest(struct mac_instance *s,
	       UINT8 *data)
{
  CAST(hmac_instance, self, s);
  struct hash_instance *h = self->state;

  HASH_DIGEST(h, data);   /* Inner hash */
  KILL(h);
  h = HASH_COPY(self->houter);
  HASH_UPDATE(h, self->super.mac_size, data);
  HASH_DIGEST(h, data);
  KILL(h);

  self->state = HASH_COPY(self->hinner);
}

static struct mac_instance *
do_hmac_copy(struct mac_instance *s)
{
  CAST(hmac_instance, self, s);
  CLONED(hmac_instance, new, self);

  new->state = HASH_COPY(self->state);

  return &new->super;
}

#define IPAD 0x36
#define OPAD 0x5c

static struct mac_instance *
make_hmac_instance(struct mac_algorithm *s,
		   UINT32 key_length,
		   const UINT8 *key)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  CAST(hmac_algorithm, self, s);
  NEW(hmac_instance, instance);
  UINT8 *pad = alloca(self->hash->block_size);

  if (key_length > self->hash->block_size)
    {
      /* Reduce key to the algorithm's hash size */
      UINT8 *digest = alloca(self->hash->hash_size);
      struct hash_instance *h = MAKE_HASH(self->hash);

      HASH_UPDATE(h, key_length, key);
      HASH_DIGEST(h, digest);

      key = digest;
      key_length = self->hash->hash_size;
    }

  instance->super.hash_size = self->super.hash_size;
  instance->super.update = do_hmac_update;
  instance->super.digest = do_hmac_digest;
  instance->super.copy = do_hmac_copy;

  instance->hinner = MAKE_HASH(self->hash);

  memset(pad, IPAD, self->hash->block_size);
  memxor(pad, key, key_length);

  HASH_UPDATE(instance->hinner, self->hash->block_size, pad);

  instance->houter = MAKE_HASH(self->hash);

  memset(pad, OPAD, self->hash->block_size);
  memxor(pad, key, key_length);
  
  HASH_UPDATE(instance->houter, self->hash->block_size, pad);

  instance->state = HASH_COPY(instance->hinner);

#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
  return &instance->super;
} 
  
struct mac_algorithm *
make_hmac_algorithm(struct hash_algorithm *h)
{
  NEW(hmac_algorithm, self);

  self->super.hash_size = h->hash_size;
  /* Recommended in RFC-2104 */
  self->super.key_size = h->hash_size;
  self->super.make_mac = make_hmac_instance;

  self->hash = h;

  return &self->super;
}

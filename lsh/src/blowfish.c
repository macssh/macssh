/* blowfish.c
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

#include "werror.h"
#include "xalloc.h"

#include "nettle/blowfish.h"

#include <assert.h>

#include "blowfish.c.x"

/* Blowfish */
/* GABA:
   (class
     (name blowfish_instance)
     (super crypto_instance)
     (vars
       (ctx . "struct blowfish_ctx")))
*/

static void
do_blowfish_encrypt(struct crypto_instance *s,
		    UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(blowfish_instance, self, s);

  blowfish_encrypt(&self->ctx, length, dst, src);
}

static void
do_blowfish_decrypt(struct crypto_instance *s,
		    UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(blowfish_instance, self, s);

  blowfish_decrypt(&self->ctx, length, dst, src);
}

static struct crypto_instance *
make_blowfish_instance(struct crypto_algorithm *algorithm, int mode, 
		       const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(blowfish_instance, self);

  self->super.block_size = BLOWFISH_BLOCK_SIZE;
  self->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			? do_blowfish_encrypt
			: do_blowfish_decrypt);
  
  if (blowfish_set_key(&self->ctx, algorithm->key_size, key))
      return &self->super;
  else
    {
      werror("Detected a weak blowfish key!\n");
      KILL(self);
      return NULL;
    }
}

struct crypto_algorithm *
make_blowfish_algorithm(UINT32 key_size)
{
  NEW(crypto_algorithm, algorithm);

  assert(key_size <= BLOWFISH_MAX_KEY_SIZE);
  assert(key_size >= BLOWFISH_MIN_KEY_SIZE);
  
  algorithm->block_size = BLOWFISH_BLOCK_SIZE;
  algorithm->key_size = key_size;
  algorithm->iv_size = 0;
  algorithm->make_crypt = make_blowfish_instance;

  return algorithm;
}

struct crypto_algorithm blowfish_algorithm =
{ STATIC_HEADER, BLOWFISH_BLOCK_SIZE, BLOWFISH_KEY_SIZE,
  0, make_blowfish_instance};

/* twofish.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Niels Möller, J.H.M. Dassen (Ray)
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

#include "twofish.h"

#include <assert.h>

#include "twofish.c.x"

/* Twofish */
/* GABA:
   (class
     (name twofish_instance)
     (super crypto_instance)
     (vars
       (ctx . "TWOFISH_context")))
*/

static void do_twofish_encrypt(struct crypto_instance *s,
				UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(twofish_instance, self, s);

  FOR_BLOCKS(length, src, dst, TWOFISH_BLOCKSIZE)
    twofish_encrypt(&self->ctx, src, dst);
}

static void do_twofish_decrypt(struct crypto_instance *s,
				UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(twofish_instance, self, s);

  FOR_BLOCKS(length, src, dst, TWOFISH_BLOCKSIZE)
    twofish_decrypt(&self->ctx, src, dst);
}

static struct crypto_instance *
make_twofish_instance(struct crypto_algorithm *algorithm, int mode,
		       const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(twofish_instance, self);

  self->super.block_size = TWOFISH_BLOCKSIZE;
  self->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			? do_twofish_encrypt
			: do_twofish_decrypt);

  /* We don't have to deal with weak keys - being an AES candidate, Twofish was
   * designed to have none. */
  twofish_setup(&self->ctx, algorithm->key_size, key);

  return &self->super;
}

struct crypto_algorithm *
make_twofish_algorithm(UINT32 key_size)
{
  NEW(crypto_algorithm, algorithm);

  assert(key_size <= TWOFISH_MAX_KEYSIZE);
  assert(key_size >= TWOFISH_MIN_KEYSIZE);

  algorithm->block_size = TWOFISH_BLOCKSIZE;
  algorithm->key_size = key_size;
  algorithm->iv_size = 0;
  algorithm->make_crypt = make_twofish_instance;

  return algorithm;
}

struct crypto_algorithm twofish128_algorithm =
{ STATIC_HEADER, TWOFISH_BLOCKSIZE, 16, 0, make_twofish_instance};

struct crypto_algorithm twofish192_algorithm =
{ STATIC_HEADER, TWOFISH_BLOCKSIZE, 24, 0, make_twofish_instance};

struct crypto_algorithm twofish256_algorithm =
{ STATIC_HEADER, TWOFISH_BLOCKSIZE, 32, 0, make_twofish_instance};

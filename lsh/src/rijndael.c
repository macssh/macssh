/* rijndael.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999, 2000 Niels Möller, Rafael R. Sevilla
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

#include "nettle/aes.h"

#include <assert.h>

#include "rijndael.c.x"

/* Rijndael */

/* GABA:
   (class
     (name rijndael_instance)
     (super crypto_instance)
     (vars
       (ctx . "struct aes_ctx")))
*/

static void
do_rijndael_encrypt(struct crypto_instance *s,
		    UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(rijndael_instance, self, s);

  aes_encrypt(&self->ctx, length, dst, src);
}

static void
do_rijndael_decrypt(struct crypto_instance *s,
		    UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(rijndael_instance, self, s);

  aes_decrypt(&self->ctx, length, dst, src);
}

static struct crypto_instance *
make_rijndael_instance(struct crypto_algorithm *algorithm, int mode,
		       const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(rijndael_instance, self);

  self->super.block_size = AES_BLOCK_SIZE;
  self->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			? do_rijndael_encrypt
			: do_rijndael_decrypt);

  aes_set_key(&self->ctx, algorithm->key_size, key);

  return(&self->super);
}

struct crypto_algorithm *
make_rijndael_algorithm(UINT32 key_size)
{
  NEW(crypto_algorithm, algorithm);

  assert(key_size <= AES_MAX_KEY_SIZE);
  assert(key_size >= AES_MIN_KEY_SIZE);

  algorithm->block_size = AES_BLOCK_SIZE;
  algorithm->key_size = key_size;
  algorithm->iv_size = 0;
  algorithm->make_crypt = make_rijndael_instance;

  return algorithm;
}

struct crypto_algorithm rijndael128_algorithm =
{ STATIC_HEADER, AES_BLOCK_SIZE, 16, 0, make_rijndael_instance};

struct crypto_algorithm rijndael192_algorithm =
{ STATIC_HEADER, AES_BLOCK_SIZE, 24, 0, make_rijndael_instance};

struct crypto_algorithm rijndael256_algorithm =
{ STATIC_HEADER, AES_BLOCK_SIZE, 32, 0, make_rijndael_instance};

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

#include "rijndael.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "rijndael.c.x"

/* Rijndael */

/* GABA:
   (class
     (name rijndael_instance)
     (super crypto_instance)
     (vars
       (ctx . "RIJNDAEL_context")))
*/

static void
do_rijndael_encrypt(struct crypto_instance *s,
		    UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(rijndael_instance, self, s);

  FOR_BLOCKS(length, src, dst, RIJNDAEL_BLOCKSIZE)
    rijndael_encrypt(&self->ctx, src, dst);
}

static void
do_rijndael_decrypt(struct crypto_instance *s,
		    UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(rijndael_instance, self, s);

  FOR_BLOCKS(length, src, dst, RIJNDAEL_BLOCKSIZE)
    rijndael_decrypt(&self->ctx, src, dst);
}

static struct crypto_instance *
make_rijndael_instance(struct crypto_algorithm *algorithm, int mode,
		       const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(rijndael_instance, self);

  self->super.block_size = RIJNDAEL_BLOCKSIZE;
  self->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			? do_rijndael_encrypt
			: do_rijndael_decrypt);

  rijndael_setup(&self->ctx, algorithm->key_size, key);

  return(&self->super);
}

struct crypto_algorithm *
make_rijndael_algorithm(UINT32 key_size)
{
  NEW(crypto_algorithm, algorithm);

  assert(key_size <= RIJNDAEL_MAX_KEYSIZE);
  assert(key_size >= RIJNDAEL_MIN_KEYSIZE);

  algorithm->block_size = RIJNDAEL_BLOCKSIZE;
  algorithm->key_size = key_size;
  algorithm->iv_size = 0;
  algorithm->make_crypt = make_rijndael_instance;

  return algorithm;
}

struct crypto_algorithm rijndael128_algorithm =
{ STATIC_HEADER, RIJNDAEL_BLOCKSIZE, 16, 0, make_rijndael_instance};

struct crypto_algorithm rijndael192_algorithm =
{ STATIC_HEADER, RIJNDAEL_BLOCKSIZE, 24, 0, make_rijndael_instance};

struct crypto_algorithm rijndael256_algorithm =
{ STATIC_HEADER, RIJNDAEL_BLOCKSIZE, 32, 0, make_rijndael_instance};

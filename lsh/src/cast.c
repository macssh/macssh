/* cast.c - wrapper arround the CAST128 implementation
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Niels Möller
 * Copyright (C) 1999 J.H.M. Dassen (Ray) <jdassen@wi.LeidenUniv.nl>
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

#include "nettle/cast128.h"

#include <assert.h>

#include "cast.c.x"

struct crypto_algorithm *make_cast_algorithm(UINT32 key_size);

/* Cast */
/* GABA:
   (class
     (name cast_instance)
     (super crypto_instance)
     (vars
       (ctx . "struct cast128_ctx")))
*/


static void
do_cast_encrypt(struct crypto_instance *s,
		UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(cast_instance, self, s);

  cast128_encrypt(&self->ctx, length, dst, src);
}

static void
do_cast_decrypt(struct crypto_instance *s,
		UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(cast_instance, self, s);

  cast128_decrypt(&self->ctx, length, dst, src);
}

static struct crypto_instance *
make_cast_instance(struct crypto_algorithm *algorithm, int mode,
		       const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(cast_instance, self);

  self->super.block_size = CAST128_BLOCK_SIZE;
  self->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			? do_cast_encrypt
			: do_cast_decrypt);

  cast128_set_key(&self->ctx, algorithm->key_size, key);
  return &self->super;
}

struct crypto_algorithm *make_cast_algorithm(UINT32 key_size)
{
  NEW(crypto_algorithm, algorithm);

  assert(key_size <= CAST128_MAX_KEY_SIZE);
  assert(key_size >= CAST128_MIN_KEY_SIZE);

  algorithm->block_size = CAST128_BLOCK_SIZE;
  algorithm->key_size = key_size;
  algorithm->iv_size = 0;
  algorithm->make_crypt = make_cast_instance;

  return algorithm;
}

struct crypto_algorithm cast128_algorithm =
{ STATIC_HEADER, CAST128_BLOCK_SIZE, CAST128_MAX_KEY_SIZE, 0, make_cast_instance};

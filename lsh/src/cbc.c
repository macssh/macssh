/* cbc.c
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

#include <assert.h>
#include <string.h>

#include "cbc.c.x"

#if !HAVE_MEMXOR
#include "nettle/memxor.h"
#endif

/* GABA:
   (class
     (name cbc_algorithm)
     (super crypto_algorithm)
     (vars
       (inner object crypto_algorithm)))
*/

/* GABA:
   (class
     (name cbc_instance)
     (super crypto_instance)
     (vars
       (inner object crypto_instance)
       (iv space UINT8)))
*/
		    
static void do_cbc_encrypt(struct crypto_instance *s,
			   UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(cbc_instance, self, s);
  
  FOR_BLOCKS(length, src, dst, self->super.block_size)
    {
      memxor(self->iv, src, self->super.block_size);

      CRYPT(self->inner, self->super.block_size, self->iv, dst);

      memcpy(self->iv, dst, self->super.block_size);
    }
}

static void do_cbc_decrypt(struct crypto_instance *s,
			   UINT32 length, const UINT8 *src, UINT8 *dst)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  CAST(cbc_instance, self, s);
  
  if (length % self->super.block_size)
    fatal("Internal error\n");

  if (!length)
    return;

  if (src == dst)
    {
      /* Keep a copy of the ciphertext. */
      UINT8 *tmp = alloca(length);
      memcpy(tmp, src, length);
      src = tmp;
    }
  
  /* Decrypt in ECB mode */
  CRYPT(self->inner, length, src, dst);

  /* XOR the cryptotext, shifted one block */
  memxor(dst,
	 self->iv, self->super.block_size);
  memxor(dst + self->super.block_size,
	 src, length - self->super.block_size);
  memcpy(self->iv,
	 src + length - self->super.block_size, self->super.block_size);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

static struct crypto_instance *
do_make_cbc_instance(struct crypto_algorithm *s,
		     int mode, const UINT8 *key, const UINT8 *iv)
{
  CAST(cbc_algorithm, algorithm, s);
  NEW(cbc_instance, instance);

  instance->super.block_size = algorithm->super.block_size;

  /* NOTE: We use a prefix of the iv, and pass the tail on to the
   * inner block crypto. This allows nested chaining, although the
   * semantics may be a little obscure.. */
  instance->inner = MAKE_CRYPT(algorithm->inner, mode, key,
			       iv + algorithm->super.block_size);
  if (!instance->inner)
    {
      /* Weak key */
      KILL(instance);
      return NULL;
    }
  instance->iv = lsh_space_alloc(algorithm->super.block_size);
  memcpy(instance->iv, iv, algorithm->super.block_size);

  instance->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			    ? do_cbc_encrypt
			    : do_cbc_decrypt);
  return &instance->super;
}

struct crypto_algorithm *crypto_cbc(struct crypto_algorithm *inner)
{
  NEW(cbc_algorithm, algorithm);
  algorithm->super.block_size = inner->block_size;
  algorithm->super.key_size = inner->key_size;
  algorithm->super.iv_size = inner->iv_size + inner->block_size;

  algorithm->inner = inner;
  algorithm->super.make_crypt = do_make_cbc_instance;

  return &algorithm->super;
} 

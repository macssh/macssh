/* des.c
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

#include "nettle/des.h"

#include <assert.h>

#include "des.c.x"

/* GABA:
   (class
     (name des_instance)
     (super crypto_instance)
     (vars
       (ctx . "struct des_ctx")))
*/

static void do_des_encrypt(struct crypto_instance *s,
			   UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(des_instance, self, s);

  des_encrypt(&self->ctx, length, dst, src);
}

static void do_des_decrypt(struct crypto_instance *s,
			 UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(des_instance, self, s);

  des_decrypt(&self->ctx, length, dst, src);
}

static struct crypto_instance *
make_des_instance(struct crypto_algorithm *algorithm UNUSED, int mode, 
		  const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(des_instance, self);
  UINT8 pkey[DES_KEY_SIZE];
  unsigned i;

  /* Fix odd parity */
  for (i=0; i<DES_KEY_SIZE; i++)
    {
      UINT8 p = key[i];
      p ^= (p >> 4);
      p ^= (p >> 2);
      p ^= (p >> 1);
      pkey[i] = key[i] ^ (p & 1) ^ 1;
#if 0
      debug("make_des_instance: computing parity: key[%i] = %xi,\n"
	    "                                    pkey[%i] = %xi,\n"
	    "                   parity = %xi\n",
	    i, key[i], i, pkey[i], p);
#endif
    }

  self->super.block_size = DES_BLOCK_SIZE;
  self->super.crypt = ( (mode == CRYPTO_ENCRYPT)
			? do_des_encrypt
			: do_des_decrypt);
  
  if (des_set_key(&self->ctx, pkey))
      return &self->super;
  
  switch(self->ctx.status)
    {
    case DES_BAD_PARITY:
      fatal("Internal error! Bad parity in make_des_instance.\n");
    case DES_WEAK_KEY:
      werror("Detected weak DES key.\n");
      KILL(self);
      return NULL;
    default:
      fatal("Internal error!\n");
    }
}

struct crypto_algorithm crypto_des_algorithm =
{ STATIC_HEADER,
  DES_BLOCK_SIZE, DES_KEY_SIZE, 0, make_des_instance };

struct crypto_algorithm *make_des3(void)
{
  return crypto_cascade(3,
			&crypto_des_algorithm,
			crypto_invert(&crypto_des_algorithm),
			&crypto_des_algorithm,
			-1);
}

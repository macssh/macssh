/* cascade.c
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

#include "abstract_crypto.h"

#include "list.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "cascade.c.x"

/* GABA:
   (class
     (name crypto_cascade_algorithm)
     (super crypto_algorithm)
     (vars
       (cascade object object_list)))
*/

/* GABA:
   (class
     (name crypto_cascade_instance)
     (super crypto_instance)
     (vars
       (cascade object object_list)))
*/     
       
static void do_cascade_crypt(struct crypto_instance *s,
			     UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(crypto_cascade_instance, self, s);
  unsigned i;
  
  if (length % self->super.block_size)
    fatal("Internal error!\n");

  assert(LIST_LENGTH(self->cascade));

  {
    CAST_SUBTYPE(crypto_instance, o, LIST(self->cascade)[0]);
    CRYPT(o, length, src, dst);
  }
  for (i = 1; i<LIST_LENGTH(self->cascade); i++)
    {
      CAST_SUBTYPE(crypto_instance, o, LIST(self->cascade)[i]);
      CRYPT(o, length, dst, dst);
    }
}

static struct crypto_instance *
do_make_cascade(struct crypto_algorithm *s,
		int mode, const UINT8 *key, const UINT8 *iv)
{
  CAST(crypto_cascade_algorithm, algorithm, s);
  NEW(crypto_cascade_instance, instance);
  unsigned i;
  unsigned l = LIST_LENGTH(algorithm->cascade);
  
  instance->super.block_size = algorithm->super.block_size;
  instance->cascade = alloc_object_list(l);

  for (i = 0; i<l; i++)
    {
      /* When decrypting, the crypto algorithms should be used in
       * reverse order! */

      unsigned j = ( (mode == CRYPTO_ENCRYPT)
		     ? i : l - i - 1);
      
      CAST_SUBTYPE(crypto_algorithm, a, LIST(algorithm->cascade)[i]);
      struct crypto_instance *o	= MAKE_CRYPT(a, mode, key, iv);
      
      if (!o)
	{
	  KILL(instance);
	  return NULL;
	}

      LIST(instance->cascade)[j] = (struct lsh_object *) o;
      key += a->key_size;
      iv += a->iv_size;
    }

  instance->super.crypt = do_cascade_crypt;
  
  return &instance->super;
}

struct crypto_algorithm *crypto_cascadel(struct object_list *cascade)
{
  NEW(crypto_cascade_algorithm, self);
  unsigned i;
  
  self->cascade = cascade;

  self->super.key_size = self->super.iv_size = 0;
  self->super.block_size = 1;

  for (i = 0; i<LIST_LENGTH(self->cascade); i++)
    {
      CAST_SUBTYPE(crypto_algorithm, a, LIST(self->cascade)[i]);
      self->super.key_size += a->key_size;
      self->super.iv_size += a->iv_size;
      self->super.block_size = lcm(self->super.block_size, a->block_size);
    }

  self->super.make_crypt = do_make_cascade;

  return &self->super;
} 

struct crypto_algorithm *crypto_cascade(unsigned n, ...)
{
  va_list args;
  struct object_list *l;

  va_start(args, n);
  l = make_object_listv(n, args);
  va_end(args);

  return crypto_cascadel(l);
}

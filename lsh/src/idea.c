/* idea.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Balázs Scheidler, Niels Möller
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

#ifdef WITH_IDEA

#include "idea.h"

#include <assert.h>

#include "idea.c.x"

/* GABA:
   (class
     (name idea_instance)
     (super crypto_instance)
     (vars
       (ctx array UINT16 IDEA_KEYLEN)))
*/

static void do_idea_crypt(struct crypto_instance *s,
			  UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(idea_instance, self, s);

  FOR_BLOCKS(length, src, dst, IDEA_BLOCKSIZE)
    idea_crypt(self->ctx, dst, src);
}

static struct crypto_instance *
make_idea_instance(struct crypto_algorithm *algorithm UNUSED, int mode, 
		   const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(idea_instance, self);

  idea_expand(self->ctx, key);
  if (mode == CRYPTO_DECRYPT)
    idea_invert(self->ctx, self->ctx);
  
  self->super.block_size = IDEA_BLOCKSIZE;
  self->super.crypt = do_idea_crypt;
  return &self->super;
}

struct crypto_algorithm idea_algorithm =
{ STATIC_HEADER, IDEA_BLOCKSIZE, IDEA_KEYSIZE, 0, make_idea_instance };

#endif /* WITH_IDEA */

/* arcfour.c
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

#include "nettle/arcfour.h"

#include "arcfour.c.x"

/* GABA:
   (class
     (name arcfour_instance)
     (super crypto_instance)
     (vars
       (ctx . "struct arcfour_ctx")))
*/
   
static void do_crypt_arcfour(struct crypto_instance *s,
			     UINT32 length, const UINT8 *src, UINT8 *dst)
{
  CAST(arcfour_instance, self, s);

  if (length % 8)
    fatal("Internal error\n");

  arcfour_crypt(&self->ctx, length, dst, src);
}

static struct crypto_instance *
make_arcfour_instance(struct crypto_algorithm *ignored UNUSED,
		      int mode UNUSED,
		      const UINT8 *key, const UINT8 *iv UNUSED)
{
  NEW(arcfour_instance, self);

  self->super.block_size = 8;
  self->super.crypt = do_crypt_arcfour;

  arcfour_set_key(&self->ctx, 16, key);

  return &self->super;
}

struct crypto_algorithm crypto_arcfour_algorithm =
{ STATIC_HEADER,
  8, 16, 0, make_arcfour_instance };

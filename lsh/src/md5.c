/* md5.c
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

#include "xalloc.h"

#include "md5.h"

#include "md5.c.x"

/* MD5 hash */
/* GABA:
   (class
     (name md5_instance)
     (super hash_instance)
     (vars
       (ctx . "struct md5_ctx")))
*/

static void
do_md5_update(struct hash_instance *s,
	      UINT32 length, const UINT8 *data)
{
  CAST(md5_instance, self, s);

  md5_update(&self->ctx, data, length);
}

static void
do_md5_digest(struct hash_instance *s,
	      UINT8 *dst)
{
  CAST(md5_instance, self, s);

  md5_final(&self->ctx);
  md5_digest(&self->ctx, dst);
  md5_init(&self->ctx);
}

static struct hash_instance *
do_md5_copy(struct hash_instance *s)
{
  return &CLONE(md5_instance, s)->super;
}

static struct hash_instance *
make_md5_instance(struct hash_algorithm *ignored UNUSED)
{
  NEW(md5_instance, res);

  res->super.hash_size = 16;
  res->super.update = do_md5_update;
  res->super.digest = do_md5_digest;
  res->super.copy = do_md5_copy;

  md5_init(&res->ctx);

  return &res->super;
}

struct hash_algorithm md5_algorithm =
{ STATIC_HEADER,
  MD5_DATASIZE, MD5_DIGESTSIZE, make_md5_instance };


/* sha.c
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

#include "nettle/sha1.h"

#include "sha.c.x"


/* SHA1 hash */
/* GABA:
   (class
     (name sha_instance)
     (super hash_instance)
     (vars
       (ctx . "struct sha1_ctx")))
*/

static void
do_sha_update(struct hash_instance *s,
	      UINT32 length, const UINT8 *data)
{
  CAST(sha_instance, self, s);

  sha1_update(&self->ctx, length, data);
}

static void
do_sha_digest(struct hash_instance *s,
	      UINT8 *dst)
{
  CAST(sha_instance, self, s);

  sha1_final(&self->ctx);
  sha1_digest(&self->ctx, SHA1_DIGEST_SIZE, dst);
  sha1_init(&self->ctx);
}

static struct hash_instance *
do_sha_copy(struct hash_instance *s)
{
  return &CLONE(sha_instance, s)->super;
}

static struct hash_instance *
make_sha_instance(struct hash_algorithm *ignored UNUSED)
{
  NEW(sha_instance, res);

  res->super.hash_size = SHA1_DIGEST_SIZE;
  res->super.update = do_sha_update;
  res->super.digest = do_sha_digest;
  res->super.copy = do_sha_copy;

  sha1_init(&res->ctx);

  return &res->super;
}

struct hash_algorithm sha1_algorithm =
{ STATIC_HEADER,
  SHA1_DATA_SIZE, SHA1_DIGEST_SIZE, make_sha_instance };


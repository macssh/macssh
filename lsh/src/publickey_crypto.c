/* publickey_crypto.c
 *
 *
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

#include "publickey_crypto.h"

#include "atoms.h"
#include "bignum.h"
#include "connection.h"
#include "crypto.h"
#include "format.h"
#include "parse.h"
#include "sha.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "publickey_crypto.h.x"
#undef GABA_DEFINE

#include "publickey_crypto.c.x"

struct keypair *
make_keypair(UINT32 type,
	     struct lsh_string *public,
	     struct signer *private)
{
  NEW(keypair, self);
  
  self->type = type;
  self->public = public;
  self->private = private;
  return self;
}

/* Groups */
/* GABA:
   (class
     (name group_zn)
     (super abstract_group)
     (vars
       (modulo bignum)))
*/

static int
zn_range(struct abstract_group *c, mpz_t x)
{
  CAST(group_zn, closure, c);

  return ( (mpz_sgn(x) == 1) && (mpz_cmp(x, closure->modulo) < 0) );
}

#if 0
static int
zn_member(struct abstract_group *c, mpz_t x)
{
  if (zn_range(c, x))
    {
      CAST(group_zn, closure, c);
      mpz_t t;
      int res;
      
      mpz_init(t);

      mpz_powm(t, x, closure->order, closure->modulo);
      res = !mpz_cmp_ui(t, 1);

      mpz_clear(t);

      return res;
    }
  return 0;
}
#endif

static void
zn_invert(struct abstract_group *c, mpz_t res, mpz_t x)
{
  CAST(group_zn, closure, c);

  if (!mpz_invert(res, x, closure->modulo))
    fatal("zn_invert: element is non-invertible\n");

  mpz_fdiv_r(res, res, closure->modulo);
}

static void
zn_combine(struct abstract_group *c, mpz_t res, mpz_t a, mpz_t b)
{
  CAST(group_zn, closure, c);

  mpz_mul(res, a, b);
  mpz_fdiv_r(res, res, closure->modulo);
}

static void
zn_power(struct abstract_group *c, mpz_t res, mpz_t g, mpz_t e)
{
  CAST(group_zn, closure, c);

  mpz_powm(res, g, e, closure->modulo);
}

static void
zn_small_power(struct abstract_group *c, mpz_t res, mpz_t g, UINT32 e)
{
  CAST(group_zn, closure, c);

  mpz_powm_ui(res, g, e, closure->modulo);
}

/* Assumes p is a prime number */
struct abstract_group *
make_group_zn(mpz_t p, mpz_t g, mpz_t order)
{
  NEW(group_zn, res);

  res->super.range = zn_range;
  res->super.invert = zn_invert;
  res->super.combine = zn_combine;
  res->super.power = zn_power;     /* Pretty Mutation! Magical Recall! */
  res->super.small_power = zn_small_power;
  res->super.add = NULL;
  res->super.subtract = NULL;
  
  mpz_init_set(res->modulo, p);
  mpz_init_set(res->super.generator, g);
  mpz_init_set(res->super.order, order);

  return &res->super;
}

static int
zn_ring_add(struct abstract_group *s,
	    mpz_t res, mpz_t a, mpz_t b)
{
  CAST(group_zn, self, s);
  mpz_add(res, a, b);
  mpz_fdiv_r(res, res, self->modulo);

  return mpz_sgn(res);
}

static int 
zn_ring_subtract(struct abstract_group *s,
		 mpz_t res, mpz_t a, mpz_t b)
{
  CAST(group_zn, self, s);
  mpz_sub(res, a, b);
  mpz_fdiv_r(res, res, self->modulo);
  
  return mpz_sgn(res);
}

/* Assumes p is a prime number, and g a primitive root. */
struct abstract_group *
make_ring_zn(mpz_t p, mpz_t g)
{
  NEW(group_zn, res);

  res->super.range = zn_range;
  res->super.invert = zn_invert;
  res->super.combine = zn_combine;
  res->super.power = zn_power;
  res->super.small_power = zn_small_power;
  res->super.add = zn_ring_add;
  res->super.subtract = zn_ring_subtract;
  
  mpz_init_set(res->modulo, p);
  mpz_init_set(res->super.generator, g);
  mpz_init_set(res->super.order, p);
  mpz_sub_ui(res->super.order, res->super.order, 1);

  return &res->super;
}

struct abstract_group *
make_ssh_group1(void)
{
  struct abstract_group *G;
  mpz_t p;
  mpz_t g;
  mpz_t order;

  /* Prime and generator as defined in
   * draft-ietf-secsh-transport-07.txt. */
  
  mpz_init_set_str(p,
		   "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
		   "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
		   "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
		   "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
		   "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE65381"
		   "FFFFFFFFFFFFFFFF", 16);

  mpz_init_set(order, p);
  mpz_sub_ui(order, order, 1);
  mpz_fdiv_q_2exp(order, order, 1);
  mpz_init_set_ui(g, 2);

  G = make_group_zn(p, g, order);

  mpz_clear(p);
  mpz_clear(g);
  mpz_clear(order);

  return G;
}

struct abstract_group *
make_ssh_ring_srp_1(void)
{
  struct abstract_group *G;
  mpz_t p;
  mpz_t g;

  /* Same prime as in draft-ietf-secsh-transport-07.txt, but a
   * different generator. */
  mpz_init_set_str(p,
		   "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
		   "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
		   "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
		   "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
		   "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE65381"
		   "FFFFFFFFFFFFFFFF", 16);

  /* 5 is a primitive root */
  mpz_init_set_ui(g, 5);

  G = make_ring_zn(p, g);

  mpz_clear(p);
  mpz_clear(g);

  return G;
}

/* dsa_keygen.c
 *
 * Generate dsa key pairs..
 *
 * $Id$
 */

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

#include "dsa.h"

#include "randomness.h"
#include "sexp.h"
#include "werror.h"

#include "nettle/sha1.h"

#if !HAVE_MEMXOR
#include "nettle/memxor.h"
#endif

#include <assert.h>

#define SA(x) sexp_a(ATOM_##x)

/* The (slow) NIST method of generating DSA primes. Algorithm 4.56 of
 * Handbook of Applied Cryptography. */

#define SEED_LENGTH SHA1_DIGEST_SIZE
#define SEED_BITS (SEED_LENGTH * 8)

static void
hash(mpz_t x, UINT8 *digest)
{
  mpz_t t;
  UINT8 data[SEED_LENGTH];
  struct sha1_ctx ctx;
  
  mpz_init_set(t, x);
  mpz_fdiv_r_2exp(t, t, SEED_BITS);
  
  bignum_write(t, SEED_LENGTH, data);
  mpz_clear(t);

  sha1_init(&ctx);
  sha1_update(&ctx, SEED_LENGTH, data);
  sha1_final(&ctx);
  sha1_digest(&ctx, SHA1_DIGEST_SIZE, digest);
}

void
dsa_nist_gen(mpz_t p, mpz_t q, struct randomness *r, unsigned l)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  unsigned L;
  unsigned n, b;
  mpz_t s, t, c;
  
  assert(l <= 8);

  L = 512 + 64*l;
  n = (L-1) / 160; b = (L-1) % 160;

  mpz_init(s);
  mpz_init(t);
  mpz_init(c);
  
  for (;;)
    {
      { /* Generate q */
	UINT8 h1[SHA1_DIGEST_SIZE];
	UINT8 h2[SHA1_DIGEST_SIZE];
	
	bignum_random_size(s, r, SEED_BITS);
	
	hash(s, h1);
	
	mpz_set(t, s);
	mpz_add_ui(t, t, 1);
	
	hash(t, h2);
	
	memxor(h1, h2, SHA1_DIGEST_SIZE);
	
	h1[0] |= 0x80;
	h1[SHA1_DIGEST_SIZE - 1] |= 1;
	
	bignum_parse_u(q, SHA1_DIGEST_SIZE, h1);
	
	if (bignum_small_factor(q, 1000)
	    || !mpz_probab_prime_p(q, 18))
	  /* Try new seed. */
	  continue;
      }
      /* q is a prime, with overwelming probability. */

      {
	unsigned size = (n+1) * SHA1_DIGEST_SIZE;
	UINT8 *buffer = alloca(size);
	unsigned i, j;
	
	for (i = 0, j = 2; i<4096; i++, j+= n+1)
	  {
	    unsigned k;
	    
#if MACOS
		extern void ssh2_sched();
		ssh2_sched();
#endif
	    for (k = 0; k<=n ; k++)
	      {
		mpz_set(t, s);
		mpz_add_ui(t, t, j + k);
		hash(t, buffer + ( (n-k) * SHA1_DIGEST_SIZE));
	      }
	    bignum_parse_u(p, size, buffer);

	    mpz_fdiv_r_2exp(p, p, L);
	    mpz_setbit(p, L-1);

	    mpz_set(t, q);
	    mpz_mul_2exp(t, t, 1);

	    mpz_fdiv_r(c, p, t);

	    mpz_sub_ui(c, c, 1);

	    mpz_sub(p, p, c);

	    if (!bignum_small_factor(p, 1000)
		&& mpz_probab_prime_p(p, 5))
	      {
		/* Done! */
		mpz_clear(s);
		mpz_clear(t);
		mpz_clear(c);

#if ALLOCA_68K_BUG
		ALLOCA_FREE(alloca_ref);
#endif
		return;
	      }
	  }
      }
    }
}

void
dsa_find_generator(mpz_t g, struct randomness *r, mpz_t p, mpz_t q)
{
  mpz_t e;
  mpz_t n;
  
  /* e = (p-1)/q */
  mpz_init_set(e, p);
  mpz_sub_ui(e, e, 1);
  mpz_divexact(e, e, q);

  /* n = p-2 = |2, 3, ... p-1| */
  mpz_init_set(n, p);
  mpz_sub_ui(n, n, 2);

  for (;;)
    {
#if MACOS
	  extern void ssh2_sched();
	  ssh2_sched();
#endif
      bignum_random(g, r, n);
      mpz_add_ui(g, g, 2);

      mpz_powm(g, g, e, p);
      if (mpz_cmp_ui(g, 1))
	{
	  /* g != 1. Finished. */
	  mpz_clear(e);
	  mpz_clear(n);

	  return;
	}
    }
}

struct sexp *
dsa_generate_key(struct randomness *r, unsigned level)
{
  struct sexp *key = NULL;
  
  mpz_t p; mpz_t q;
  mpz_t g; mpz_t y;
  mpz_t x;
  mpz_t t;

  mpz_init(p); mpz_init(q);
  mpz_init(g); mpz_init(y);
  mpz_init(x);
  mpz_init(t);

  dsa_nist_gen(p, q, r, level);
  
  debug("p = %xn\nq = %xn\n", p, q);

  /* Sanity check. */
  if (!mpz_probab_prime_p(p, 10))
    {
      werror("p not a prime!\n");
      goto done;
    }

  if (!mpz_probab_prime_p(q, 10))
    {
      werror("q not a prime!\n");
      goto done;
    }

  mpz_fdiv_r(t, p, q);
  if (mpz_cmp_ui(t, 1))
    {
      fatal("q doesn't divide p-1 !\n");
      goto done;
    }

  dsa_find_generator(g, r, p, q);
    
  mpz_set(t, q);
  mpz_sub_ui(t, t, 2);
  bignum_random(x, r, t);
  
  mpz_add_ui(x, x, 1);
    
  mpz_powm(y, g, x, p);

  key = sexp_l(2, SA(PRIVATE_KEY),
	       sexp_l(6, SA(DSA),
		      sexp_l(2, SA(P), sexp_un(p), -1),
		      sexp_l(2, SA(Q), sexp_un(q), -1),
		      sexp_l(2, SA(G), sexp_un(g), -1),
		      sexp_l(2, SA(Y), sexp_un(y), -1),
		      sexp_l(2, SA(X), sexp_un(x), -1), -1), -1);

 done:

  mpz_clear(p); mpz_clear(q);
  mpz_clear(g); mpz_clear(y);
  mpz_clear(x);
  mpz_clear(t);

  return key;
}

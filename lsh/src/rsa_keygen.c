/* rsa_keygen.c
 *
 * Generate rsa key pairs..
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
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

#include "rsa.h"

#include "sexp.h"
#include "werror.h"

#define SA(x) sexp_a(ATOM_##x)

/* Takes the public exponent e as argument. */
struct sexp *
rsa_generate_key(mpz_t e, struct randomness *r, UINT32 bits)
{
  struct sexp *key = NULL;
  
  mpz_t n;
  mpz_t d;
  mpz_t p; mpz_t q;
  mpz_t a; mpz_t b; mpz_t c;
  mpz_t phi;
  mpz_t tmp;
  
  mpz_init(n); mpz_init(d); mpz_init(p), mpz_init(q);
  mpz_init(a); mpz_init(b); mpz_init(c);

  mpz_init(phi); mpz_init(tmp);
  
  /* Generate primes */
  bignum_random_prime(p, r, bits/2);
  bignum_random_prime(q, r, (bits+1)/2);

  debug("p = %xn\nq = %xn\n", p, q);
  
  /* Compute modulo */
  mpz_mul(n, p, q);

  debug("n = %xn\n", n);
  
  /* Compute phi */
  mpz_sub_ui(phi, p, 1);
  mpz_sub_ui(tmp, q, 1);
  mpz_mul(phi, phi, tmp);

  debug("phi = %xn\ne = %xn\n", phi, e);
  
  /* Compute d such that e d = 1 (mod phi) */
  /* NOTE: mpz_invert sometimes generates negative inverses. */
  if (!mpz_invert(d, e, phi))
    {
      debug("rsa_generate_key: e not invertible.\n");
      goto done;
    }

  if (mpz_sgn(d) < 0)
    mpz_fdiv_r(d, d, phi);

  debug("d = %xn\n", d);
  
  /* Compute extra values that are needed for the CRT optimization */

  /* a = d % (p-1) */
  mpz_sub_ui(tmp, p, 1);
  mpz_fdiv_r(a, d, tmp);

  debug("a = %xn\n", a);
  
  mpz_sub_ui(tmp, q, 1);
  mpz_fdiv_r(b, d, tmp);

  debug("b = %xn\n", b);
  
  if (!mpz_invert(c, q, p))
    {
      werror("rsa_generate_key: q not invertible.");
      goto done;
    }

  if (mpz_sgn(c) < 0)
    mpz_fdiv_r(c, c, p);

  debug("a = %xn\n", c);

  /* FIXME: Add sanity checking */
  
  key = sexp_l(2, SA(PRIVATE_KEY),
	       sexp_l(9, SA(RSA_PKCS1_SHA1),
		      sexp_l(2, SA(N), sexp_un(n), -1),
		      sexp_l(2, SA(E), sexp_un(e), -1),
		      sexp_l(2, SA(D), sexp_un(d), -1),
		      sexp_l(2, SA(P), sexp_un(p), -1),
		      sexp_l(2, SA(Q), sexp_un(q), -1),
		      sexp_l(2, SA(A), sexp_un(a), -1),
		      sexp_l(2, SA(B), sexp_un(b), -1),
		      sexp_l(2, SA(C), sexp_un(c), -1), -1), -1);

 done:
  mpz_clear(n); mpz_clear(d); mpz_clear(p), mpz_clear(q);
  mpz_clear(a); mpz_clear(b); mpz_clear(c);

  mpz_clear(phi); mpz_clear(tmp);

  return key;
}


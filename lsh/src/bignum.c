/* bignum.c
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

#include "bignum.h"
#include "randomness.h"
#include "werror.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "prime_table.h"

static void
limbs_to_octets(const mpz_t n, UINT32 length,
		UINT8 pad, UINT8 *data)
{
  UINT8 *dst = data + length - 1;
	
  mp_limb_t *l = n->_mp_d;  /* Starts at the least significant limb */
  int left;
	
  for (left = n->_mp_size;
      (length > 0) && (left  > 0);
      left--)
    {
      size_t i;
      mp_limb_t word = *l++;
      for(i = 0; i<sizeof(mp_limb_t); i++)
	{
	  *dst-- = word & 0xff;
	  word >>= 8;
	  length--;
	  if (!length)
	    break;
	}
    }
  while (length > 0)
    {
      *dst-- = pad;
      length--;
    }
}

/* Formatting of signed numbers */
void
bignum_parse_s(mpz_t n, UINT32 length, const UINT8 *data)
{
  int negative = length && (*data & 0x80);
  size_t i;
  mpz_t digit;

  mpz_init(digit);
  mpz_set_ui(n, 0);
  for (i = 0; i < length; i++)
    {
      mpz_set_ui(digit, data[i]);
      mpz_mul_2exp(digit, digit, (length - i - 1) * 8);
      mpz_ior(n, n, digit);
    }
  if (negative)
    {
      mpz_set_ui(digit, 1);
      mpz_mul_2exp(digit, digit, length*8);
      mpz_sub(n, n, digit);
    }
  mpz_clear(digit);
}

static int
mpz_size_of_complement(const mpz_t n)
{
  int bits;
	
  /* One's complement(x) = - x - 1 */
  mpz_t complement;
  mpz_init(complement);
  mpz_com(complement, n);
  
  /* Note that bits == 1 if complement == 0, i.e n = -1 */
  bits = mpz_sizeinbase(complement, 2);
  
  mpz_clear(complement);
  
  return bits;
}

/* This function should handle both positive and negative numbers */
UINT32 bignum_format_s_length(const mpz_t n)
{
  switch(mpz_sgn(n))
    {
    case 0:
      return 0;
    case 1:
      return mpz_sizeinbase(n, 2)/8 + 1;
    case -1:
      return mpz_size_of_complement(n)/8 + 1;
    default:
      fatal("Internal error");
    }
}
  
UINT32
bignum_format_s(const mpz_t n, UINT8 *data)
{
  switch(mpz_sgn(n))
    {
    case 0:
      return 0;
    case 1:
      {
	size_t length = mpz_sizeinbase(n, 2)/8 + 1;

	limbs_to_octets(n, length, 0, data);
	return length;
      }
    case -1:
      {
	mpz_t complement;
	size_t length;
	int i;
	
	mpz_init(complement);
	mpz_com(complement, n);

	/* Note that mpz_sizeinbase(0) == 0.*/
	length = mpz_sizeinbase(complement, 2)/8 + 1;
	
	for (i = 0; i<complement->_mp_size; i++)
	  complement->_mp_d[i] = ~complement->_mp_d[i];
	
	limbs_to_octets(complement, length, 0xff, data);

	mpz_clear(complement);
	return length;
      }
    default:
      fatal("Internal error");
    }
}

/* Formatting of unsigned numbers */
void
bignum_parse_u(mpz_t n, UINT32 length, const UINT8 *data)
{
  size_t i;
  mpz_t digit;

  mpz_init(digit);
  mpz_set_ui(n, 0);
  for (i = 0; i < length; i++)
    {
      mpz_set_ui(digit, data[i]);
      mpz_mul_2exp(digit, digit, (length - i - 1) * 8);
      mpz_ior(n, n, digit);
    }
  mpz_clear(digit);
}

UINT32
bignum_format_u_length(const mpz_t n)
{
  switch(mpz_sgn(n))
    {
    case 0:
      return 0;
    case 1:
      return (mpz_sizeinbase(n, 2) + 7) / 8;
    default:
      fatal("Internal error: Negative number to bignum_format_u_length\n");
    }
}

void
bignum_write(mpz_t n, unsigned length, UINT8 *data)
{
  limbs_to_octets(n, length, 0, data);
}

UINT32
bignum_format_u(const mpz_t n, UINT8 *data)
{
  switch(mpz_sgn(n))
    {
    case 0:
      return 0;
    case 1:
      {
	size_t length = (mpz_sizeinbase(n, 2) + 7) / 8;

	limbs_to_octets(n, length, 0, data);
	return length;
      }
    default:
      fatal("Internal error: Negative number to bignum_format_u\n");
    }
}

/* Returns a random number, 0 <= x < 2^bits. */
void
bignum_random_size(mpz_t x, struct randomness *random, unsigned bits)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  unsigned length = (bits + 7) / 8;
  UINT8 *data = alloca(length);

  RANDOM(random, length, data);

  bignum_parse_u(x, length, data);

  if (bits % 8)
    mpz_fdiv_r_2exp(x, x, bits);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

/* Returns a random number, 0 <= x < n. */
void
bignum_random(mpz_t x, struct randomness *random, mpz_t n)
{
  /* FIXME: This leaves some bias, which may be bad for DSA. A better
   * way might to generate a random number of mpz_sizeinbase(n, 2)
   * bits, and loop until one smaller than n is found. */

  /* From Daniel Bleichenbacher (via coderpunks):
   *
   * There is still a theoretical attack possible with 8 extra bits.
   * But, the attack would need about 2^66 signatures 2^66 memory and
   * 2^66 time (if I remember that correctly). Compare that to DSA,
   * where the attack requires 2^22 signatures 2^40 memory and 2^64
   * time. And of course, the numbers above are not a real threat for
   * PGP. Using 16 extra bits (i.e. generating a 176 bit random number
   * and reducing it modulo q) will defeat even this theoretical
   * attack.
   * 
   * More generally log_2(q)/8 extra bits are enoug to defeat my
   * attack. NIST also plans to update the standard.
   */

  /* Add a few bits extra, to decrease the bias from the final modulo
   * operation. */
  bignum_random_size(x, random, mpz_sizeinbase(n, 2) + 10);

  mpz_fdiv_r(x, x, n);
}

/* Returns a small factor of n, or 0 if none is found.*/
unsigned long
bignum_small_factor(mpz_t n, int limit)
{
  int i;
  unsigned long stop;
  
  if (limit > NUMBER_OF_PRIMES)
    limit = NUMBER_OF_PRIMES;

  stop = mpz_get_ui(n);
  if (mpz_cmp_ui(n, stop) != 0)
    stop = ULONG_MAX;

  for (i = 0;
       (i < limit)
	 /* These squares could be tabulated as well, but I don't
	  * think it's worth the effort to get rid of this extra
	  * multiplication. */
	 && (SQR(primes[i]) <= stop); 
       i++)
    if (mpz_fdiv_ui(n, primes[i]) == 0)
      return primes[i];
  return 0;
}

void
bignum_next_prime(mpz_t p, mpz_t n, int count, int prime_limit)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  mpz_t tmp;
  unsigned long *moduli = NULL;
  unsigned long difference;
  
  /* First handle tiny numbers */
  if (mpz_cmp_ui(n, 2) <= 0)
    {
      mpz_set_ui(p, 2);
      return;
    }
  mpz_set(p, n);
  mpz_setbit(p, 0);

  if (mpz_cmp_ui(p, 8) < 0)
    return;

  mpz_init(tmp);

  if (prime_limit > (NUMBER_OF_PRIMES -1))
    prime_limit = NUMBER_OF_PRIMES - 1;

  if (prime_limit && (mpz_cmp_ui(p, primes[prime_limit]) <= 0) )
    /* Don't use table for small numbers */
    prime_limit = 0;

  if (prime_limit)
    {
      /* Compute residues modulo small odd primes */
      int i;

      moduli = alloca(prime_limit * sizeof(*moduli));
      for (i = 0; i < prime_limit; i++)
	moduli[i] = mpz_fdiv_ui(p, primes[i + 1]);
    }

  for (difference = 0; ; difference += 2)
    {
#if MACOS
	  extern void ssh2_sched();
	  ssh2_sched();
#endif
      if (difference >= ULONG_MAX - 10)
	{ /* Should not happen, at least not very often... */
	  mpz_add_ui(p, p, difference);
	  difference = 0;
	}

      /* First check residues */
      if (prime_limit)
	{
	  int composite = 0;
      	  int i;

	  for (i = 0; i < prime_limit; i++)
	    {
	      if (moduli[i] == 0)
		composite = 1;
	      moduli[i] = (moduli[i] + 2) % primes[i + 1];
	    }
	  if (composite)
	    continue;
	}
      
      mpz_add_ui(p, p, difference);
      difference = 0;

      /* Fermat test, with respect to 2 */
      mpz_set_ui(tmp, 2);
      mpz_powm(tmp, tmp, p, p);
      if (mpz_cmp_ui(tmp, 2) != 0)
	continue;

      /* Miller-Rabin test */
      if (mpz_probab_prime_p(p, count))
	break;
    }
  mpz_clear(tmp);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

void
bignum_random_prime(mpz_t x, struct randomness *random, unsigned bits)
{
  assert(bits);
  
  do
    {
#if MACOS
	  extern void ssh2_sched();
	  ssh2_sched();
#endif
      bignum_random_size(x, random, bits);
      mpz_setbit(x, bits - 1);

      /* Miller-rabin count of 25, and use primes in the table. */
      bignum_next_prime(x, x, 25, 5000);
    }
  while (mpz_sizeinbase(x, 2) > bits);

  assert(mpz_sizeinbase(x, 2) == bits);
}

/* rsa.c
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

#include "atoms.h"
#include "crypto.h"
#include "format.h"
#include "parse.h"
#include "sexp.h"
/* #include "ssh.h" */
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

#define GABA_DEFINE
#include "rsa.h.x"
#undef GABA_DEFINE

#include "rsa.c.x"

#define SA(x) sexp_a(ATOM_##x)

#define RSA_CRT 1

/* GABA:
   (class
     (name rsa_verifier)
     (super verifier)
     (vars
       (params object rsa_algorithm)
       (size . unsigned)
       (n bignum)
       (e bignum)))
*/

/* GABA:
   (class
     (name rsa_signer)
     (super signer)
     (vars
       (verifier object rsa_verifier)

       ; Secret exponent
       (d bignum)

       ; The two factors
       (p bignum)
       (q bignum)

       ; d % (p-1), i.e. a e = 1 (mod p)
       (a bignum)

       ; d % (q-1), i.e. b e = 1 (mod q)
       (b bignum)

       ; modular inverse of q , i.e. c q = 1 (mod p)
       (c bignum)))
*/


/* Utility functions */
   
static void
pkcs1_encode(mpz_t m,
	     struct rsa_algorithm *params,
	     UINT32 length,
	     UINT32 msg_length,
	     const UINT8 *msg)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  UINT8 *em = alloca(length);
  unsigned i = length;
  
  struct hash_instance *h = MAKE_HASH(params->hash);
  HASH_UPDATE(h, msg_length, msg);

  assert(i >= h->hash_size);
  i -= h->hash_size;

  HASH_DIGEST(h, em + i);
  KILL(h);

  assert(i >= params->prefix_length);
  i -= params->prefix_length;

  memcpy(em + i, params->prefix, params->prefix_length);

  assert(i);
  em[--i] = 0;

  assert(i >= 9);
  em[0] = 1;
  memset(em + 1, 0xff, i - 1);
  
  bignum_parse_u(m, length, em);

  debug("pkcs1_encode: m = %xn\n", m);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

static int
rsa_check_size(struct rsa_verifier *key)
{
  /* Size in octets */
  key->size = (mpz_sizeinbase(key->n, 2) + 7) / 8;

  /* For PKCS#1 to make sense, the size of the modulo, in octets, must
   * be at least 1 + the length of the DER-encoded Digest Info.
   *
   * And a DigestInfo is 34 octets for md5, and 35 octets for sha1.
   * 46 octets is 368 bits. */
  return (key->size >= 46);
}

static struct sexp *
encode_rsa_sig_val(mpz_t s)
{
  return sexp_un(s);
}

static int
decode_rsa_sig_val(struct sexp *e, mpz_t s, unsigned limit)
{
  return sexp2bignum_u(e, s, limit);
}

static int
spki_init_rsa_verifier(struct rsa_verifier *key,
		       struct sexp_iterator *i)
{
  return (sexp_get_un(i, ATOM_N, key->n, RSA_MAX_SIZE)
	  && sexp_get_un(i, ATOM_E, key->e, RSA_MAX_SIZE)
	  && rsa_check_size(key));
}


/* Signature verification */
static int
rsa_pkcs1_verify(struct rsa_verifier *self,
		 UINT32 length,
		 const UINT8 *msg,
		 mpz_t signature)
{
  int res;
  mpz_t m;
  mpz_t s;
  
  if (mpz_cmp(signature, self->n) >= 0)
    return 0;

  mpz_init(m);
  mpz_init(s);

  debug("rsa_pkcs1_verify: n = %xn\n"
	"                  e = %xn\n"
	"                  s = %xn\n",
	self->n, self->e, signature);

  mpz_powm(s, signature, self->e, self->n);

  debug("rsa_pkcs1_verify: m = %xn\n", s);
  
  pkcs1_encode(m, self->params, self->size - 1,
	       length, msg);
  
  res = !mpz_cmp(m, s);
  mpz_clear(m); mpz_clear(s);

  return res;
}

static int
do_rsa_verify(struct verifier *v,
	      int algorithm,
	      UINT32 length,
	      const UINT8 *msg,
	      UINT32 signature_length,
	      const UINT8 *signature_data)
{
  CAST(rsa_verifier, self, v);
  mpz_t s;
  int res = 0;

  trace("do_rsa_verify: Verifying %a signature\n", algorithm);
  
  mpz_init(s);
  
  switch(algorithm)
    {
#if 0
    case ATOM_RSA_PKCS1_SHA1:
    case ATOM_RSA_PKCS1_SHA1_LOCAL:

      if (signature_length > self->size)
	goto fail;
  
      bignum_parse_u(s, signature_length, signature_data);
      break;

    case ATOM_SPKI:
#endif
    case ATOM_SSH_RSA:
      {
	struct simple_buffer buffer;
	UINT32 length;
	const UINT8 *digits;
	int atom;
	
	simple_buffer_init(&buffer, signature_length, signature_data);

	if (!(parse_atom(&buffer, &atom)
	      && (atom == ATOM_SSH_RSA)
	      && parse_string(&buffer, &length, &digits)
	      && (length <= self->size)
	      && parse_eod(&buffer) ))
	  goto fail;

	bignum_parse_u(s, length, digits);

	break;
      }
      
      /* It doesn't matter here which flavour of SPKI is used. */
    case ATOM_SPKI_SIGN_RSA:
    case ATOM_SPKI_SIGN_DSS:
      {
	struct simple_buffer buffer;
	struct sexp *e;
	
	simple_buffer_init(&buffer, signature_length, signature_data);

	if (! ( (e = sexp_parse_canonical(&buffer))
		&& parse_eod(&buffer)
		&& decode_rsa_sig_val(e, s, self->size)) )
	  goto fail;

	break;
      }
      
    default:
      fatal("do_rsa_verify: Internal error!\n");
    }
  
  res = rsa_pkcs1_verify(self, length, msg, s);

 fail:
  mpz_clear(s);
  
  return res;
}

static int
do_rsa_verify_spki(struct verifier *v,
		   UINT32 length,
		   const UINT8 *msg,
		   struct sexp *e)
{
  CAST(rsa_verifier, self, v);
  mpz_t s;
  int res;

  mpz_init(s);
  
  res = (decode_rsa_sig_val(e, s, self->size)
	 && rsa_pkcs1_verify(self, length, msg, s));
  
  mpz_clear(s);

  return res;
}

static struct lsh_string *
do_rsa_public_key(struct verifier *s)
{
  CAST(rsa_verifier, self, s);

  return ssh_format("%a%n%n", ATOM_SSH_RSA,
		     self->e, self->n);
}

static struct sexp *
do_rsa_public_spki_key(struct verifier *s)
{
  CAST(rsa_verifier, self, s);

  return sexp_l(3, sexp_a(self->params->name),
		sexp_l(2, SA(N), sexp_un(self->n), -1),
		sexp_l(2, SA(E), sexp_un(self->e), -1),
		-1);
}

/* NOTE: To initialize an rsa verifier, one must
 *
 * 1. Call this function.
 * 2. Initialize the modulo n and exponent e.
 * 3. Call rsa_check_size.
 */
static void
init_rsa_verifier(struct rsa_verifier *self, struct rsa_algorithm *params)
{
  self->params = params;

  /* FIXME: The allocator could do this kind of initialization
   * automatically. */
  mpz_init(self->n);
  mpz_init(self->e);

  self->size = 0;

  self->super.verify = do_rsa_verify;
  self->super.verify_spki = do_rsa_verify_spki;
  self->super.public_key = do_rsa_public_key;
  self->super.public_spki_key = do_rsa_public_spki_key;
}

static struct rsa_verifier *
make_rsa_verifier_internal(struct rsa_algorithm *params,
			   struct sexp_iterator *i)
{
  NEW(rsa_verifier, res);
  init_rsa_verifier(res, params);

  assert(SEXP_LEFT(i) >= 2);
  
  if (spki_init_rsa_verifier(res, i))
    {
      return res;
    }
  else
    {
      KILL(res);
      return NULL;
    }
}
  
/* Alternative constructor using a key of type ssh-rsa, when the atom
 * "ssh-rsa" is already read from the buffer. */
struct verifier *
parse_ssh_rsa_public(struct simple_buffer *buffer)
{
  NEW(rsa_verifier, res);
  init_rsa_verifier(res, &rsa_sha1_algorithm);

  if (parse_bignum(buffer, res->e, RSA_MAX_SIZE)
      && (mpz_sgn(res->e) == 1)
      && parse_bignum(buffer, res->n, RSA_MAX_SIZE)
      && (mpz_sgn(res->n) == 1)
      && (mpz_cmp(res->e, res->n) < 0)
      && parse_eod(buffer)
      && rsa_check_size(res))
    return &res->super;

  else
    {
      KILL(res);
      return NULL;
    }
}

/* Signature creation */

/* Compute x, the d:th root of m. Calling it with x == m is allowed. */
static void
rsa_compute_root(struct rsa_signer *self, mpz_t x, mpz_t m)
{
  debug("rsa_compute_root: n = %xn\n"
	"                  e = %xn\n"
	"                  m = %xn\n",
	self->verifier->n, self->verifier->e, m);

#if RSA_CRT
  {
    mpz_t xp; /* modulo p */
    mpz_t xq; /* modulo q */

    mpz_init(xp); mpz_init(xq);    

#if 0
    debug("rsa_compute_root: p = %xn\n"
	  "                  q = %xn\n",
	  self->p, self->q);

    debug("rsa_compute_root: d = %xn\n"
	  "                  a = %xn\n"
	  "                  b = %xn\n"
	  "                  c = %xn\n",
	  self->d, self->a, self->b, self->c);
#endif
    
    /* Compute xq = m^d % q = (m%q)^b % q */
    mpz_fdiv_r(xq, m, self->q);
    mpz_powm(xq, xq, self->b, self->q);

#if 0
    debug("rsa_compute_root: xq = %xn\n", xq);
#endif
    
    /* Compute xp = m^d % p = (m%p)^a % p */
    mpz_fdiv_r(xp, m, self->p);
    mpz_powm(xp, xp, self->a, self->p);

#if 0
    debug("rsa_compute_root: xp = %xn\n", xp);
#endif
    
    /* Set xp' = (xp - xq) c % p. */
    mpz_sub(xp, xp, xq);
    mpz_mul(xp, xp, self->c);
    mpz_fdiv_r(xp, xp, self->p);

#if 0
    debug("rsa_compute_root: xp' = %xn\n", xp);
#endif
    
    /* Finally, compute x = xq + q xp'
     *
     * To prove that this works, note that
     *
     *   xp  = x + i p,
     *   xq  = x + j q,
     *   c q = 1 + k p
     *
     * for some integers i, j and k. Now, for some integer l,
     *
     *   xp' = (xp - xq) c + l p
     *       = (x + i p - (x + j q)) c + l p
     *       = (i p - j q) c + l p
     *       = (i + l) p - j (c q)
     *       = (i + l) p - j (1 + kp)
     *       = (i + l - j k) p - j
     *
     * which shows that xp' = -j (mod p). We get
     *
     *   xq + q xp' = x + j q + (i + l - j k) p q - j q
     *              = x + (i + l - j k) p q
     *
     * so that
     *
     *   xq + q xp' = x (mod pq)
     *
     * We also get 0 <= xq + q xp' < p q, because
     *
     *   0 <= xq < q and 0 <= * xp' < p.
     */
    mpz_mul(x, self->q, xp);
    mpz_add(x, x, xq);

    mpz_clear(xp); mpz_clear(xq);
  }  
#else /* !RSA_CRT */
  mpz_powm(x, m, self->d, self->verifier->n);
#endif /* !RSA_CRT */
  debug("rsa_compute_root: x = %xn\n", x);
}

static struct lsh_string *
do_rsa_sign(struct signer *s,
	    int algorithm,
	    UINT32 msg_length,
	    const UINT8 *msg)
{
  CAST(rsa_signer, self, s);
  struct lsh_string *res;
  mpz_t m;

  trace("do_rsa_sign: Signing according to %a\n", algorithm);
  
  mpz_init(m);
  pkcs1_encode(m, self->verifier->params, self->verifier->size - 1,
	       msg_length, msg);

  rsa_compute_root(self, m, m);

  switch (algorithm)
    {
    case ATOM_SSH_RSA:
      /* Uses the encoding:
       *
       * string ssh-rsa
       * string signature-blob
       */
  
      res = ssh_format("%a%un", ATOM_SSH_RSA, m);
      break;
#if 0
    case ATOM_RSA_PKCS1_SHA1:
    case ATOM_RSA_PKCS1_SHA1_LOCAL:
      /* Uses the encoding:
       *
       * string rsa-pkcs1
       * string signature-blob
       */
  
      res = ssh_format("%a%un", ATOM_RSA_PKCS1_SHA1, m);
      break;
#endif
      /* It doesn't matter here which flavour of SPKI is used. */
    case ATOM_SPKI_SIGN_RSA:
    case ATOM_SPKI_SIGN_DSS:

      res = sexp_format(encode_rsa_sig_val(m), SEXP_CANONICAL, 0);
      break;
    default:
      fatal("do_rsa_sign: Internal error!\n");
    }
  mpz_clear(m);
  return res;
}

static struct sexp *
do_rsa_sign_spki(struct signer *s,
		 /* struct sexp *hash, struct sexp *principal, */
		 UINT32 msg_length,
		 const UINT8 *msg)
{
  CAST(rsa_signer, self, s);
  mpz_t m;
  struct sexp *signature;

  mpz_init(m);
  pkcs1_encode(m, self->verifier->params, self->verifier->size - 1,
	       msg_length, msg);

  rsa_compute_root(self, m, m);

  signature = encode_rsa_sig_val(m);
  
  mpz_clear(m);
  return signature;
}

static struct verifier *
do_rsa_get_verifier(struct signer *s)
{
  CAST(rsa_signer, self, s);
  return &self->verifier->super;
}


static struct verifier *
make_rsa_verifier(struct signature_algorithm *s,
		  struct sexp_iterator *i)
{
  CAST(rsa_algorithm, self, s);

  return ( (SEXP_LEFT(i) == 2)
	   ? &make_rsa_verifier_internal(self, i)->super
	   : NULL);
}


static struct signer *
make_rsa_signer(struct signature_algorithm *s,
		struct sexp_iterator *i)
{
  CAST(rsa_algorithm, params, s);
  NEW(rsa_signer, res);

  if ( (SEXP_LEFT(i) == 8)
       && ( (res->verifier = make_rsa_verifier_internal(params, i)) )
       && sexp_get_un(i, ATOM_D, res->d, RSA_MAX_SIZE)
       && sexp_get_un(i, ATOM_P, res->p, RSA_MAX_SIZE)
       && sexp_get_un(i, ATOM_Q, res->q, RSA_MAX_SIZE)
       && sexp_get_un(i, ATOM_A, res->a, RSA_MAX_SIZE)
       && sexp_get_un(i, ATOM_B, res->b, RSA_MAX_SIZE)
       && sexp_get_un(i, ATOM_C, res->c, RSA_MAX_SIZE) )
    {
      res->super.sign = do_rsa_sign;
      res->super.sign_spki = do_rsa_sign_spki;
      res->super.get_verifier = do_rsa_get_verifier;
      
      return &res->super;
    }
  else
    {
      KILL(res);
      return NULL;
    }
}


struct signature_algorithm *
make_rsa_algorithm(struct hash_algorithm *hash,
		   int name,
		   UINT32 prefix_length,
		   const UINT8 *prefix)
{
  NEW(rsa_algorithm, self);
  self->super.make_signer = make_rsa_signer;
  self->super.make_verifier = make_rsa_verifier;
  self->hash = hash;
  self->name = name;
  
  self->prefix_length = prefix_length;
  self->prefix = prefix;

  return &self->super;
}

#define STATIC_RSA_ALGORITHM(a, n, l, id) \
{ { STATIC_HEADER, make_rsa_signer, make_rsa_verifier }, \
  (a), (n), (l), (id) }

/* From pkcs-1v2
 *
 *   md5 OBJECT IDENTIFIER ::=
 *     {iso(1) member-body(2) US(840) rsadsi(113549) digestAlgorithm(2) 5}
 *
 * The parameters part of the algorithm identifier is NULL:
 *
 *   md5Identifier ::= AlgorithmIdentifier {md5, NULL}
 */

static const UINT8 md5_prefix[] =
{
  /* 18 octets prefic 16 octets hash, 34 total. */
  0x30,       32, /* SEQUENCE */
    0x30,     12, /* SEQUENCE */
      0x06,    8, /* OBJECT IDENTIFIER */
  	0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05,
      0x05,    0, /* NULL */
    0x04,     16  /* OCTET STRING */
      /* Here comes the raw hash value */
};

struct rsa_algorithm rsa_md5_algorithm =
STATIC_RSA_ALGORITHM(&md5_algorithm, ATOM_RSA_PKCS1_MD5, 18, md5_prefix);

/* From pkcs-1v2
 *
 *   id-sha1 OBJECT IDENTIFIER ::=
 *     {iso(1) identified-organization(3) oiw(14) secsig(3)
 *   	 algorithms(2) 26}
 *   
 *   The default hash function is SHA-1: 
 *   sha1Identifier ::= AlgorithmIdentifier {id-sha1, NULL}
 */

static const UINT8 sha1_prefix[] =
{
  /* 15 octets prefix, 20 octets hash, total 35 */
  0x30,       33, /* SEQUENCE */
    0x30,      9, /* SEQUENCE */
      0x06,    5, /* OBJECT IDENTIFIER */
  	  0x2b, 0x0e, 0x03, 0x02, 0x1a,
      0x05,    0, /* NULL */
    0x04,     20  /* OCTET STRING */
      /* Here comes the raw hash value */
};

struct rsa_algorithm rsa_sha1_algorithm =
STATIC_RSA_ALGORITHM(&sha1_algorithm, ATOM_RSA_PKCS1_SHA1, 15, sha1_prefix);

struct verifier *
make_ssh_rsa_verifier(UINT32 public_length,
		      const UINT8 *public)
{
  struct simple_buffer buffer;
  int atom;
  
  simple_buffer_init(&buffer, public_length, public);

  return ( (parse_atom(&buffer, &atom)
	    && (atom == ATOM_SSH_RSA))
	   ? parse_ssh_rsa_public(&buffer)
	   : NULL);
}

/* dsa.c
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

#include "atoms.h"
#include "crypto.h"
#include "format.h"
#include "parse.h"
#include "sexp.h"
#include "spki.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include "nettle/sha1.h"

#include <assert.h>

#include "dsa.c.x" 

/* The standard says that DSA public keys are at most 1024 bits, i.e.
 * 128 octets. We are a little more liberal than that. Note that
 * allowing really large keys opens for Denial-of-service attacks. */

#define DSA_MAX_SIZE 300

#define DSA_MAX_QSIZE SHA1_DIGEST_SIZE

#define SA(x) sexp_a(ATOM_##x)

/* DSA signatures */

/* GABA:
   (class
     (name dsa_algorithm)
     (super signature_algorithm)
     (vars
       (random object randomness)))
*/

/* GABA:
   (class
     (name dsa_verifier)
     (super verifier)
     (vars
       (p bignum)
       (q bignum)
       (g bignum)
       (y bignum)))
*/

/* GABA:
   (class
     (name dsa_signer)
     (super signer)
     (vars
       (verifier object dsa_verifier)
       (random object randomness)
       (a bignum)))
*/


/* Utility functions */

static void
dsa_hash(mpz_t h, UINT32 length, const UINT8 *msg)
{
  /* Compute hash */
  UINT8 digest[SHA1_DIGEST_SIZE];
  struct sha1_ctx ctx;
  sha1_init(&ctx);
  sha1_update(&ctx, length, msg);
  sha1_final(&ctx);
  sha1_digest(&ctx, SHA1_DIGEST_SIZE, digest);

  bignum_parse_u(h, SHA1_DIGEST_SIZE, digest);

  debug("DSA hash: %xn\n", h);
}

static struct sexp *
encode_dsa_sig_val(mpz_t r, mpz_t s)
{
  return sexp_l(2, sexp_l(2, SA(R), sexp_un(r), -1),
		sexp_l(2, SA(S), sexp_un(s), -1), -1);
}

static int
decode_dsa_sig_val(struct sexp *e, mpz_t r, mpz_t s)
{
  if (sexp_atomp(e))
    return 0;
  else
    {
      struct sexp_iterator *i = SEXP_ITER(e);
      return ( (SEXP_LEFT(i) == 2)
	       && sexp_get_un(i, ATOM_R, r, DSA_MAX_QSIZE)
	       && sexp_get_un(i, ATOM_S, s, DSA_MAX_QSIZE));
    }
}


/* Verifying DSA signatures */

/* The caller should make sure that r and s are non-negative, and not
 * extremely large. That they are less than q is checked here. */
static int
generic_dsa_verify(struct dsa_verifier *key,
		   UINT32 length,
		   const UINT8 *msg,
		   mpz_t r, mpz_t s)
{
  mpz_t w, tmp, v;
  int res;
  
  debug("generic_dsa_verify, r: %xn\n"
	"                    s: %xn\n", r, s);

  if ( (mpz_cmp(r, key->q) >= 0)
       || (mpz_cmp(s, key->q) >= 0) )
    return 0;
  
  /* Compute w = s^-1 (mod q) */
  mpz_init(w);

  /* NOTE: In gmp-2, mpz_invert sometimes generates negative inverses. */
  if (!mpz_invert(w, s, key->q))
    {
      werror("generic_dsa_verify: s non-invertible.\n");
      mpz_clear(w);
      return 0;
    }

  debug("generic_dsa_verify, w: %xn\n", w);

  /* Compute hash */
  mpz_init(tmp);
  dsa_hash(tmp, length, msg);

  /* g^{w * h (mod q)} (mod p)  */

  mpz_init(v);

  mpz_mul(tmp, tmp, w);
  mpz_fdiv_r(tmp, tmp, key->q);

  debug("u1: %xn\n", tmp);
  
  mpz_powm(v, key->g, tmp, key->p);

  /* y^{w * r (mod q) } (mod p) */
  mpz_mul(tmp, r, w);
  mpz_fdiv_r(tmp, tmp, key->q);

  debug("u2: %xn\n", tmp);

  mpz_powm(tmp, key->y, tmp, key->p);
  
  /* (g^{w * h} * y^{w * r} (mod p) ) (mod q) */
  mpz_mul(v, v, tmp);
  mpz_fdiv_r(v, v, key->p);

  debug("generic_dsa_verify, group element: %xn\n", v);
  
  mpz_fdiv_r(v, v, key->q);

  debug("generic_dsa_verify, v: %xn\n", v);

  res = !mpz_cmp(v, r);

  mpz_clear(w);
  mpz_clear(tmp);
  mpz_clear(v);

  return res;
}

static int
do_dsa_verify(struct verifier *c, int algorithm,
	      UINT32 length,
	      const UINT8 *msg,
	      UINT32 signature_length,
	      const UINT8 *signature_data)
{
  CAST(dsa_verifier, self, c);
  struct simple_buffer buffer;

  int res = 0;
  
  mpz_t r, s;

  trace("do_dsa_verify: Verifying %a signature\n", algorithm);
  
  mpz_init(r);
  mpz_init(s);
  
  switch (algorithm)
    {
    case ATOM_SSH_DSS:
      {
	/* NOTE: draft-ietf-secsh-transport-X.txt (x <= 07) uses an extra
	 * length field, which should be removed in the next version. */
	
	UINT32 buf_length;
	const UINT8 *buf;
	int atom;
      
	simple_buffer_init(&buffer, signature_length, signature_data);
	if (!(parse_atom(&buffer, &atom)
	      && (atom == ATOM_SSH_DSS)
	      && parse_string(&buffer, &buf_length, &buf)
	      && !(buf_length % 2)
	      && (buf_length <= (2 * DSA_MAX_QSIZE))
	      && parse_eod(&buffer)))
	  goto fail;

	buf_length /= 2;
  
	bignum_parse_u(r, buf_length, buf);
	bignum_parse_u(s, buf_length, buf + buf_length);

	break;
      }

#if DATAFELLOWS_WORKAROUNDS
    case ATOM_SSH_DSS_KLUDGE_LOCAL:
      {
	UINT32 buf_length;

	/* NOTE: This doesn't include any length field. Is that right? */

	if ( (signature_length % 2)
	     || (signature_length > (2 * DSA_MAX_QSIZE)) )
	  goto fail;

	buf_length = signature_length / 2;

	bignum_parse_u(r, buf_length, signature_data);
	bignum_parse_u(s, buf_length, signature_data + buf_length);
	break;
      }
#endif
      /* It doesn't matter here which flavour of SPKI is used. */
    case ATOM_SPKI_SIGN_RSA:
    case ATOM_SPKI_SIGN_DSS:
      {
	struct simple_buffer buffer;
	struct sexp *e;
	
	simple_buffer_init(&buffer, signature_length, signature_data);
	
	if (! ( (e = sexp_parse_canonical(&buffer))
		&& parse_eod(&buffer)
		&& decode_dsa_sig_val(e, r, s)) )
	  goto fail;

	break;
      }
    default:
      fatal("do_dsa_verify: Internal error!\n");
    }
  res = generic_dsa_verify(self, length, msg, r, s);
 fail:
  mpz_clear(r);
  mpz_clear(s);

  return res;
}

static int
do_dsa_verify_spki(struct verifier *c,
		   UINT32 length,
		   const UINT8 *msg,
		   struct sexp *e)
{
  CAST(dsa_verifier, self, c);

  int res;
  mpz_t r, s;

  mpz_init(r);
  mpz_init(s);

  /* NOTE: With the current definition of sexp_get_un, there are no
   * requirements on the order in which r and s occur. */
  res = (decode_dsa_sig_val(e, r, s)
	 && generic_dsa_verify(self, length, msg, r, s));

  mpz_clear(r);
  mpz_clear(s);

  return res;
}

static struct lsh_string *
do_dsa_public_key(struct verifier *s)
{
  CAST(dsa_verifier, self, s);
  return ssh_format("%a%n%n%n%n",
		    ATOM_SSH_DSS,
		    self->p, self->q,
		    self->g, self->y);
}

static struct sexp *
do_dsa_public_spki_key(struct verifier *s)
{
  CAST(dsa_verifier, self, s);

  return sexp_l(5, SA(DSA),
		sexp_l(2, SA(P), sexp_un(self->p), -1),
		sexp_l(2, SA(Q), sexp_un(self->q), -1),
		sexp_l(2, SA(G), sexp_un(self->g), -1),
		sexp_l(2, SA(Y), sexp_un(self->y), -1),
		-1);
}

static void
init_dsa_verifier(struct dsa_verifier *self)
{
  /* FIXME: The allocator could do this kind of initialization
   * automatically. */
  mpz_init(self->p);
  mpz_init(self->q);
  mpz_init(self->g);
  mpz_init(self->y);

  self->super.verify = do_dsa_verify;
  self->super.verify_spki = do_dsa_verify_spki;
  self->super.public_spki_key = do_dsa_public_spki_key;
  self->super.public_key = do_dsa_public_key;
}

static struct dsa_verifier *
make_dsa_verifier_internal(struct sexp_iterator *i)
{
  NEW(dsa_verifier, res);
  init_dsa_verifier(res);

  assert(SEXP_LEFT(i) >= 4);

  if (sexp_get_un(i, ATOM_P, res->p, DSA_MAX_SIZE)
      && sexp_get_un(i, ATOM_Q, res->q, DSA_MAX_QSIZE)
      && sexp_get_un(i, ATOM_G, res->g, DSA_MAX_SIZE)
      && sexp_get_un(i, ATOM_Y, res->y, DSA_MAX_SIZE))
    {
      return res;
    }
  else
    {
      KILL(res);
      return NULL;
    }
}
  
/* Alternative constructor using a key of type ssh-dss, when the atom
 * "ssh-dss" is already read from the buffer. */
struct verifier *
parse_ssh_dss_public(struct simple_buffer *buffer)
{
  NEW(dsa_verifier, res);
  init_dsa_verifier(res);

  if (parse_bignum(buffer, res->p, DSA_MAX_SIZE)
      && (mpz_sgn(res->p) == 1)
      && parse_bignum(buffer, res->q, DSA_MAX_QSIZE)
      && (mpz_sgn(res->q) == 1)
      && (mpz_cmp(res->q, res->p) < 0) /* q < p */ 
      && parse_bignum(buffer, res->g, DSA_MAX_SIZE)
      && (mpz_sgn(res->g) == 1)
      && (mpz_cmp(res->g, res->p) < 0) /* g < p */ 
      && parse_bignum(buffer, res->y, DSA_MAX_SIZE) 
      && (mpz_sgn(res->y) == 1)
      && (mpz_cmp(res->y, res->p) < 0) /* y < p */
      && parse_eod(buffer))
    
    return &res->super;

  else
    {
      KILL(res);
      return NULL;
    }
}

  
/* Creating signatures */
static void
generic_dsa_sign(struct dsa_signer *self,
		 UINT32 length, const UINT8 *msg,
		 mpz_t r, mpz_t s)
{
  mpz_t k, tmp;

  assert(r && s);
  
  /* Select k, 0<k<q, randomly */
  mpz_init_set(tmp, self->verifier->q);
  mpz_sub_ui(tmp, tmp, 1);

  mpz_init(k);
  bignum_random(k, self->random, tmp);
  mpz_add_ui(k, k, 1);

  /* NOTE: Enabling this leaks information about the secret
   * key to the debug output. */
  /* debug("generic_dsa_sign, k: %xn\n", k); */
  
  /* Compute r = (g^k (mod p)) (mod q) */
  mpz_powm(r, self->verifier->g, k, self->verifier->p);

  debug("generic_dsa_sign, group element: %xn\n", r);
  
  mpz_fdiv_r(r, r, self->verifier->q);

  debug("generic_dsa_sign, r: %xn\n", r);

  /* Compute hash */
  dsa_hash(tmp, length, msg);
  
  /* Compute k^-1 (mod q) */
  if (!mpz_invert(k, k, self->verifier->q))
    {
      fatal("generic_dsa_sign: k non-invertible\n");
    }

  /* Compute signature s = k^-1(h + ar) (mod q) */
  mpz_mul(s, r, self->a);
  mpz_fdiv_r(s, s, self->verifier->q);
  mpz_add(s, s, tmp);
  mpz_mul(s, s, k);
  mpz_fdiv_r(s, s, self->verifier->q);

  debug("generic_dsa_sign, s: %xn\n", s);
  
  mpz_clear(k);
  mpz_clear(tmp);
}

static UINT32
dsa_blob_length(mpz_t r, mpz_t s)
{
  UINT32 r_length = bignum_format_u_length(r);
  UINT32 s_length = bignum_format_u_length(s);

  return MAX(r_length, s_length);
}

static void
dsa_blob_write(mpz_t r, mpz_t s, UINT32 length, UINT8 *buf)
{
  bignum_write(r, length, buf);
  bignum_write(s, length, buf + length);
}

static struct lsh_string *
do_dsa_sign(struct signer *c,
	    int algorithm,
	    UINT32 msg_length,
	    const UINT8 *msg)
{
  CAST(dsa_signer, self, c);
  mpz_t r, s;
  struct lsh_string *signature;
  UINT32 buf_length;
  UINT8 *p;

  trace("do_dsa_sign: Signing according to %a\n", algorithm);
  
  mpz_init(r); mpz_init(s);
  generic_dsa_sign(self, msg_length, msg, r, s);

  /* Build signature */
  buf_length = dsa_blob_length(r, s);

  switch (algorithm)
    {
    case ATOM_SSH_DSS:
      /* NOTE: draft-ietf-secsh-transport-X.txt (x <= 07) uses an extra
       * length field, which should be removed in the next version. */
      signature = ssh_format("%a%r", ATOM_SSH_DSS, buf_length * 2, &p);
      dsa_blob_write(r, s, buf_length, p);

      break;
      
#if DATAFELLOWS_WORKAROUNDS
    case ATOM_SSH_DSS_KLUDGE_LOCAL:
      
      /* NOTE: This doesn't include any length field. Is that right? */
      signature = ssh_format("%lr", buf_length * 2, &p);
      dsa_blob_write(r, s, buf_length, p);

      break;

#endif
      /* It doesn't matter here which flavour of SPKI is used. */
    case ATOM_SPKI_SIGN_RSA:
    case ATOM_SPKI_SIGN_DSS:

      /* NOTE: Generates the <sig-val> only. */
      signature
	= sexp_format(encode_dsa_sig_val(r, s),
		      SEXP_CANONICAL, 0);
      
      break;
    default:
      fatal("do_dsa_sign: Internal error.");
    }
  mpz_clear(r);
  mpz_clear(s);

  return signature;
}

static struct sexp *
do_dsa_sign_spki(struct signer *c,
		 /* struct sexp *hash, struct sexp *principal, */
		 UINT32 msg_length,
		 const UINT8 *msg)
{
  CAST(dsa_signer, self, c);
  mpz_t r, s;
  struct sexp *signature;
    
  mpz_init(r); mpz_init(s);
  generic_dsa_sign(self, msg_length, msg, r, s);
      
  /* Build signature */
  signature = encode_dsa_sig_val(r, s);
  
  mpz_clear(r);
  mpz_clear(s);

  return signature;
}

static struct verifier *
do_dsa_get_verifier(struct signer *s)
{
  CAST(dsa_signer, self, s);
  return &self->verifier->super;
}


static struct verifier *
make_dsa_verifier(struct signature_algorithm *self UNUSED,
		  struct sexp_iterator *i)
{
  return ( (SEXP_LEFT(i) == 4)
	   ? &make_dsa_verifier_internal(i)->super
	   : NULL);
}

static struct signer *
make_dsa_signer(struct signature_algorithm *c,
		struct sexp_iterator *i)
{
  CAST(dsa_algorithm, self, c);
  NEW(dsa_signer, res);
  mpz_init(res->a);

#if 0
  debug("make_dsa_signer: SEXP_LEFT(i) == %i\n");
  debug("make_dsa_signer: SEXP_GET(i) == %fS\n",
	sexp_format(SEXP_GET(i), SEXP_ADVANCED, 0));
#endif
  
  if ( (SEXP_LEFT(i) == 5)
       && ( (res->verifier = make_dsa_verifier_internal(i)) )
       && sexp_get_un(i, ATOM_X, res->a, DSA_MAX_QSIZE) )
    {
      res->random = self->random;
      res->super.sign = do_dsa_sign;
      res->super.sign_spki = do_dsa_sign_spki;
      res->super.get_verifier = do_dsa_get_verifier;
      
      return &res->super;
    }
  else
    {
      KILL(res);
      return NULL;
    }
}

struct signature_algorithm *
make_dsa_algorithm(struct randomness *random)
{
  NEW(dsa_algorithm, dsa);

  dsa->super.make_signer = make_dsa_signer;
  dsa->super.make_verifier = make_dsa_verifier;
  dsa->random = random;

  return &dsa->super;
}


struct verifier *
make_ssh_dss_verifier(UINT32 public_length,
		      const UINT8 *public)
{
  struct simple_buffer buffer;
  int atom;
  
  simple_buffer_init(&buffer, public_length, public);

  return ( (parse_atom(&buffer, &atom)
	    && (atom == ATOM_SSH_DSS))
	   ? parse_ssh_dss_public(&buffer)
	   : NULL);
}

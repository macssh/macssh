/* srp_exchange.c
 *
 * Thomas Wu's Secure Remote Password Protocol
 *
 * $Id$ */

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

#include "srp.h"

#include "crypto.h"
#include "format.h"
#include "sexp.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "srp.h.x"
#undef GABA_DEFINE

#if WITH_SRP

/* We use the following syntax for verifiers:
 *
 * (srp-verifier ssh-group1 <salt> <verifier>)
 *
 * For now, the second element is the name of a group; we could also substitute
 * ( (modulo <n>) (generator <g>) ) or something like that.
 */

/* Copies the name,rather than consuming it. */
struct srp_entry *
make_srp_entry(struct lsh_string *name, struct sexp *e)
{
  struct sexp_iterator *i;

  if (sexp_check_type(e, ATOM_SRP_VERIFIER, &i)
      && (SEXP_LEFT(i) == 3)
      && sexp_atom_eq(SEXP_GET(i), ATOM_SSH_RING1) )
    {
      NEW(srp_entry, res);
      struct lsh_string *salt;

      mpz_init(res->verifier);
      
      SEXP_NEXT(i);

      salt = sexp2string(SEXP_GET(i));
      if (!salt)
	{
	  KILL(res);
	  return NULL;
	}
      res->salt = lsh_string_dup(salt);

      SEXP_NEXT(i);

      /* FIXME: Pass a more restrictive limit to sexp2bignum_u. */
      if (!sexp2bignum_u(SEXP_GET(i), res->verifier, 0))
	{
	  KILL(res);
	  return NULL;
	}
      
      res->name = lsh_string_dup(name);

      return res;
    }
  else
    return NULL;
}

/* Consumes the salt */
struct sexp *
srp_make_verifier(struct abstract_group *G,
		  struct hash_algorithm *H,
		  struct lsh_string *salt,
		  struct lsh_string *name,
		  struct lsh_string *passwd)
{
  mpz_t x;
  struct sexp *e;
  
  mpz_init(x);

  srp_hash_password(x, H, salt, name, passwd);  
  GROUP_POWER(G, x, G->generator, x);

  e = sexp_l(4,
	     sexp_a(ATOM_SRP_VERIFIER), sexp_a(ATOM_SSH_RING1),
	     sexp_s(NULL, salt),
	     sexp_un(x),
	     -1);

  mpz_clear(x);

  return e;
}

/* Thomas Wu's Secure Remote Password Protocol, with a fixed group. */

void
srp_hash_password(mpz_t x,
		  struct hash_algorithm *H,
		  struct lsh_string *salt,
		  struct lsh_string *name,
		  struct lsh_string *passwd)
{
  struct lsh_string *h
    = hash_string(H, ssh_format("%S%fS", salt,
				hash_string(H, ssh_format("%S%S", name, passwd), 1)),
		  1);

  bignum_parse_u(x, h->length, h->data);
  lsh_string_free(h);
}

static void
srp_format_proofs(struct dh_instance *dh,
		  struct lsh_string **m1,
		  struct lsh_string **m2)
{
  struct mac_algorithm *mac_algorithm
    = make_hmac_algorithm(dh->method->H);
  struct mac_instance *hmac
    = MAKE_MAC(mac_algorithm,
	       dh->K->length, dh->K->data);
  struct lsh_string *s;
  
  *m1 = lsh_string_alloc(hmac->hash_size);
  *m2 = lsh_string_alloc(hmac->hash_size);
  
  HASH_UPDATE(hmac,
	      dh->exchange_hash->length, dh->exchange_hash->data);
  HASH_DIGEST(hmac, (*m1)->data);

  s = ssh_format("%n%S%S", dh->e, *m1, dh->exchange_hash);

  HASH_UPDATE(hmac, s->length, s->data);
  HASH_DIGEST(hmac, (*m2)->data);

  lsh_string_free(s);
  KILL(hmac);
}

/* dh_instance, name -> packet*/
struct lsh_string *
srp_make_init_msg(struct dh_instance *dh, struct lsh_string *name)
{
  dh_generate_secret(dh->method, dh->secret, dh->e);
  dh_hash_update(dh, ssh_format("%S", name), 1);

  debug("srp_make_init_msg: e = %xn\n", dh->e);
  return ssh_format("%c%S%n", SSH_MSG_KEXSRP_INIT, name, dh->e);
}

/* dh_instance, packet -> name */
struct lsh_string *
srp_process_init_msg(struct dh_instance *self, struct lsh_string *packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;

  struct lsh_string *name;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_KEXSRP_INIT)
      && ( (name = parse_string_copy(&buffer) ))
      /* FIXME: Pass a more restrictive limit to parse_bignum. */
      && parse_bignum(&buffer, self->e, 0)
      && (mpz_cmp_ui(self->e, 1) > 0)
      && GROUP_RANGE(self->method->G, self->e)
      && parse_eod(&buffer) )
    {
      debug("srp_process_init_msg: e = %xn\n", self->e);
      return name;
    }
  else
    {
      werror("Invalid SSH_MSG_KEXSRP_INIT message.\n");
      return NULL;
    }
}

/* dh_instance -> u */
static UINT32
srp_select_u(struct dh_instance *dh)
{
  struct lsh_string *h;
  UINT32 u;
  
  h = hash_string(dh->method->H, ssh_format("%ln", dh->f), 1);

  u = READ_UINT32(h->data);
  lsh_string_free(h);

  debug("srp_select_u: u = %xi\n", u);
  return u;
}

/* dh_instance, v -> packet */
struct lsh_string *
srp_make_reply_msg(struct dh_instance *dh, struct srp_entry *entry)
{
  UINT32 u;
  mpz_t tmp;
  
  debug("srp_make_reply_msg: v = %xn\n", entry->verifier);
  
  for (;;)
    {
      /* Loop, in case f or u turns out to be zero */
      dh_generate_secret(dh->method, dh->secret, dh->f);

      debug("srp_make_reply_msg: f - v = %xn\n", dh->f);
      
      if (!GROUP_ADD(dh->method->G, dh->f, dh->f, entry->verifier))
	{
	  werror("srp_exchange.c: Found cleartext password by mistake!\n");
	  continue;
	}

      debug("srp_make_reply_msg: f = %xn\n", dh->f);
      
      u = srp_select_u(dh);
      if (u)
	break;
    }

  /* Compute (e v^u) ^ b */
  mpz_init(tmp);
  
  GROUP_SMALL_POWER(dh->method->G, tmp, entry->verifier, u);
  GROUP_COMBINE(dh->method->G, tmp, dh->e, tmp);
  GROUP_POWER(dh->method->G, tmp, tmp, dh->secret);

  debug("srp_make_reply_msg: K = %xn\n", tmp);

  dh->K = ssh_format("%ln", tmp);

  mpz_clear(tmp);
  
  /* Update the exchange hash */
  
  dh_hash_update(dh, ssh_format("%S%S", entry->name, entry->salt), 1);
  dh_hash_digest(dh);
  
  return ssh_format("%c%S%n", SSH_MSG_KEXSRP_REPLY, entry->salt, dh->f);
}

/* dh_instance, packet -> salt */
struct lsh_string *
srp_process_reply_msg(struct dh_instance *dh, struct lsh_string *packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  struct lsh_string *salt;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_KEXSRP_REPLY)
      && ( (salt = parse_string_copy(&buffer) ))
      /* FIXME: Pass a more restrictive limit to parse_bignum. */
      && parse_bignum(&buffer, dh->f, 0)
      && (mpz_cmp_ui(dh->f, 1) > 0)
      && GROUP_RANGE(dh->method->G, dh->f)
      && parse_eod(&buffer))
    {
      debug("srp_process_reply_msg: f = %xn\n", dh->f);
      
      /* FIXME: It would be better to keep the u around. Now, we have
       * to compute it again later. */
      if (!srp_select_u(dh))
	{
	  werror("Recived SSH_MSG_KEXSRP_REPLY messge with u = 0.\n");
	  lsh_string_free(salt);
	  return NULL;
	}
      dh_hash_update(dh, ssh_format("%S", salt), 1);      
      return salt;
    }
  else
    {
      werror("Invalid SSH_MSG_KEXSRP_REPLY message.\n");
      return NULL;
    }
}

/* dh_instance, x -> packet, m2
 *
 * x is derived from the password using srp_hash_password */
struct lsh_string *
srp_make_client_proof(struct dh_instance *dh,
		      struct lsh_string **m2,
		      mpz_t x)
{
  UINT32 u = srp_select_u(dh);
  mpz_t v;
  mpz_t tmp;
  struct lsh_string *m1;
  
  assert(u);
  
  mpz_init(v);

  /* Compute the verifier */
  GROUP_POWER(dh->method->G, v, dh->method->G->generator, x);

  debug("srp_make_client_proof: v = %xn\n", v);
  
  if (!GROUP_SUBTRACT(dh->method->G, v, dh->f, v))
    {
      mpz_clear(v);
      return NULL;
    }

  debug("srp_make_client_proof: f - v = %xn\n", v);
  
  mpz_init(tmp);

  /* Compute the exponent */
  mpz_mul_ui(tmp, x, u);
  mpz_add(tmp, tmp, dh->secret);

  GROUP_POWER(dh->method->G, v, v, tmp);

  debug("srp_make_client_proof: K = %xn\n", v);
  dh->K = ssh_format("%ln", v);
  
  mpz_clear(v);
  mpz_clear(tmp);

  dh_hash_digest(dh);
  srp_format_proofs(dh, &m1, m2);
  
  return ssh_format("%c%fS", SSH_MSG_KEXSRP_PROOF, m1);
}

struct lsh_string *
srp_process_client_proof(struct dh_instance *dh, struct lsh_string *packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;

  UINT32 length;
  const UINT8 *client_m1;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_KEXSRP_PROOF)
      && parse_string(&buffer, &length, &client_m1)
      && parse_eod(&buffer))
    {
      struct lsh_string *m1;
      struct lsh_string *m2;

      srp_format_proofs(dh, &m1, &m2);
      
      if (!lsh_string_eq_l(m1, length, client_m1))
	{
	  werror("SRP failed: Received invalid m1 from client.\n");
	  lsh_string_free(m1);
	  lsh_string_free(m2);
	  
	  return NULL;
	}
      lsh_string_free(m1);
      return ssh_format("%c%fS", SSH_MSG_KEXSRP_PROOF,
			m2);
    }
  return NULL;
}

int
srp_process_server_proof(struct lsh_string *m2,
			 struct lsh_string *packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;

  UINT32 length;
  const UINT8 *server_m2;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_KEXSRP_PROOF)
      && parse_string(&buffer, &length, &server_m2)
      && parse_eod(&buffer))
    {
      return lsh_string_eq_l(m2, length, server_m2);
    }
  return 0;
}

struct dh_method *
make_srp1(struct randomness *r)
{
  return make_dh(make_ssh_ring_srp_1(), &sha1_algorithm, r);
}

#endif /* WITH_SRP */

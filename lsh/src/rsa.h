/* rsa.h
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

#ifndef LSH_RSA_H_INCLUDED
#define LSH_RSA_H_INCLUDED

#include "bignum.h"
#include "publickey_crypto.h"

#define GABA_DECLARE
#include "rsa.h.x"
#undef GABA_DECLARE

/* We don't allow keys larger than 5000 bits (i.e. 625 octets). Note
 * that allowing really large keys opens for Denial-of-service
 * attacks. */

#define RSA_MAX_SIZE 625

/* GABA:
   (class
     (name rsa_algorithm)
     (super signature_algorithm)
     (vars
       (hash object hash_algorithm)
       ; SPKI name for the algorithm
       (name . int)
       ; The complete prefix for a DigestInfo, including the algorithm
       ; identifier for the hash function. A DigestInfo is formed by
       ; cetenating this prefix with the raw hash value.
       (prefix_length . UINT32)
       (prefix . "const UINT8 *")))
*/

struct signature_algorithm *
make_rsa_algorithm(struct hash_algorithm *hash,
		   int name,
		   UINT32 prefix_length,
		   const UINT8 *prefix);

extern struct rsa_algorithm rsa_md5_algorithm;
extern struct rsa_algorithm rsa_sha1_algorithm;

/* Non spki keys */
struct verifier *
parse_ssh_rsa_public(struct simple_buffer *buffer);

struct verifier *
make_ssh_rsa_verifier(UINT32 public_length,
		      const UINT8 *public);

struct sexp *
rsa_generate_key(mpz_t e, struct randomness *r, UINT32 bits);

#endif /* LSH_RSA_H_INCLUDED */

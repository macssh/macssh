/* dsa.h
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

#ifndef LSH_DSA_H_INCLUDED
#define LSH_DSA_H_INCLUDED

#include "bignum.h"
#include "publickey_crypto.h"

/* DSA signatures */

struct signature_algorithm *
make_dsa_algorithm(struct randomness *random);

/* Non spki keys */
struct verifier *
parse_ssh_dss_public(struct simple_buffer *buffer);

struct verifier *
make_ssh_dss_verifier(UINT32 public_length,
		      const UINT8 *public);

void dsa_nist_gen(mpz_t p, mpz_t q, struct randomness *r, unsigned l);
void dsa_find_generator(mpz_t g, struct randomness *r, mpz_t p, mpz_t q);

struct sexp *
dsa_generate_key(struct randomness *r, unsigned level);

#endif /* LSH_DSA_H_INCLUDED */

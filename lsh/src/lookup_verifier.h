/* lookup_verifier.h
 *
 * Lookup signature verifiers of a public key
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Balázs Scheidler
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

#ifndef LSH_LOOKUP_VERIFIER_H_INCLUDED
#define LSH_LOOKUP_VERIFIER_H_INCLUDED

#include "lsh.h"
#include "abstract_crypto.h"

#define GABA_DECLARE
#include "lookup_verifier.h.x"
#undef GABA_DECLARE

/* Maps a key blob to a signature verifier, using some signature
 * algorithm and some method to determine the authenticity of the key.
 * Returns NULL if the key is invalid or not trusted. */

/* GABA:
   (class
     (name lookup_verifier)
     (vars
       (lookup method (object verifier)
                      "int method"

		      ; NOTE: The user is only needed on the server side.
                      "struct lsh_user *user"
		      "struct lsh_string *key")))
*/

#define LOOKUP_VERIFIER(l, m, kh, key) ((l)->lookup((l), (m), (kh), (key)))

#endif /* LSH_LOOKUP_VERIFIER_H_INCLUDED */

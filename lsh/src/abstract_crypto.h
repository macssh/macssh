/* abstract_crypto.h
 *
 * Interface to block cryptos and hash functions
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

#ifndef LSH_ABSTRACT_CRYPTO_H_INCLUDED
#define LSH_ABSTRACT_CRYPTO_H_INCLUDED

#include "list.h"

/* Use the same instance struct for both hash functions and macs. This
 * is a little ugly. */
#define mac_instance_class hash_instance_class
#define mac_instance hash_instance
#define mac_size hash_size

#define GABA_DECLARE
#include "abstract_crypto.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name crypto_instance)
     (vars
       (block_size . UINT32)
       ; Length must be a multiple of the block size.
       ; NOTE: src == dst is allowed, but no other overlaps.
       (crypt method void
              "UINT32 length" "const UINT8 *src" "UINT8 *dst")))
*/

#define CRYPT(instance, length, src, dst) \
((instance)->crypt((instance), (length), (src), (dst)))

#define CRYPTO_ENCRYPT 0
#define CRYPTO_DECRYPT 1

/* GABA:
   (class
     (name crypto_algorithm)
     (vars
       (block_size . UINT32)
       (key_size . UINT32)
       (iv_size . UINT32)
       (make_crypt method (object crypto_instance)
                   "int mode" "const UINT8 *key" "const UINT8 *iv")))
*/

#define MAKE_CRYPT(crypto, mode, key, iv) \
((crypto)->make_crypt((crypto), (mode), (key), (iv)))     

#define MAKE_ENCRYPT(crypto, key, iv) \
     MAKE_CRYPT((crypto), CRYPTO_ENCRYPT, (key), (iv))

#define MAKE_DECRYPT(crypto, key, iv) \
     MAKE_CRYPT((crypto), CRYPTO_DECRYPT, (key), (iv))
     
/* FIXME: Hashes could use non-virtual methods. */
     
/* GABA:
   (class
     (name hash_instance)
     (vars
       (hash_size . UINT32)
       (update method void 
	       "UINT32 length" "const UINT8 *data")
       (digest method void "UINT8 *result")

       ;; FIXME: Perhaps add an argument which
       ;; is a hash instance to copy to.
       (copy method (object hash_instance))))
*/

#define HASH_UPDATE(instance, length, data) \
((instance)->update((instance), (length), (data)))

#define HASH_DIGEST(instance, result) \
((instance)->digest((instance), (result)))

#define HASH_COPY(instance) ((instance)->copy((instance)))

/* GABA:
   (class
     (name hash_algorithm)
     (vars
       (block_size . UINT32)
       (hash_size . UINT32)
       (make_hash method (object hash_instance))))
*/

#define MAKE_HASH(h) ((h)->make_hash((h)))

/* GABA:
   (class
     (name mac_algorithm)
     (vars
       (hash_size . UINT32)
       ; Recommended key size
       (key_size . UINT32)
       (make_mac method (object mac_instance)
                 "UINT32 length" "const UINT8 *key")))
*/

#define MAKE_MAC(m, l, key) ((m)->make_mac((m), (l), (key)))


/* GABA:
   (class
     (name verifier)
     (vars
       (verify method int
               "int algorithm"
       	       "UINT32 length" "const UINT8 *data"
	       "UINT32 signature_length" "const UINT8 *signature_data")

       (verify_spki method int
       	  "UINT32 length" "const UINT8 *data"
       	  "struct sexp *e")

       (public_key method (string))
       
       ; Returns ( <pub-sig-alg-id> <s-expr>* ), i.e. the same
       ; structure that is expected by spki_make_verifier().
       (public_spki_key method (object sexp))))
*/

#define VERIFY(verifier, algorithm, length, data, slength, sdata) \
((verifier)->verify((verifier), (algorithm), (length), (data), (slength), (sdata)))

#define VERIFY_SPKI(verifier, length, data, e) \
((verifier)->verify_spki((verifier), (length), (data), (e)))

#define PUBLIC_KEY(signer) ((signer)->public_key((signer)))
#define PUBLIC_SPKI_KEY(signer) ((signer)->public_spki_key((signer)))


/* GABA:
   (class
     (name signer)
     (vars
       ; Returns a non-spki signature
       (sign method (string)
             "int algorithm" "UINT32 length" "const UINT8 *data")
       ; Returns a <sig-val> sexp
       (sign_spki method (object sexp)
       	     "UINT32 length" "const UINT8 *data")

       (get_verifier method (object verifier))))
*/

#define SIGN(signer, algorithm, length, data) \
((signer)->sign((signer), (algorithm), (length), (data)))
#define SIGN_SPKI(signer, length, data) ((signer)->sign_spki((signer), (length), (data)))
#define SIGNER_GET_VERIFIER(signer) ((signer)->get_verifier((signer)))


/* GABA:
   (class
     (name signature_algorithm)
     (vars
       ; Iterators should point past the algorithm tag
       (make_signer method (object signer)
                    "struct sexp_iterator *i")
		    
       (make_verifier method (object verifier)
                      "struct sexp_iterator *i")))
*/

#define MAKE_SIGNER(a, i) \
((a)->make_signer((a), (i)))

#define MAKE_VERIFIER(a, i) \
((a)->make_verifier((a), (i)))


/* Combining block cryptos */

/* Example: To create a tripple DES cbc encryptor:
 *
 * struct crypto_algorithm des3_cbc
 *  = make_cbc(crypto_cascade(3, des_algorithm,
 *                               crypto_invert(des_algorithm)
 *                               des_algorithm, -1));
 */

struct crypto_algorithm *crypto_cbc(struct crypto_algorithm *inner);
struct crypto_algorithm *crypto_invert(struct crypto_algorithm *inner);
struct crypto_algorithm *crypto_cascadel(struct object_list *cascade);
struct crypto_algorithm *crypto_cascade(unsigned n, ...);

/* Utility functions */
UINT32 gcd(UINT32 x, UINT32 y);
UINT32 lcm(UINT32 x, UINT32 y);

/* Simple hashing */
struct lsh_string *
hash_string(struct hash_algorithm *a,
	    struct lsh_string *in,
	    int free);

struct lsh_string *
mac_string(struct mac_algorithm *a,
	   struct lsh_string *key,
	   int kfree,
	   struct lsh_string *in,
	   int ifree);

struct lsh_string *
crypt_string(struct crypto_instance *c,
	     struct lsh_string *in,
	     int free);

struct lsh_string *
crypt_string_pad(struct crypto_instance *c,
		 struct lsh_string *in,
		 int free);

struct lsh_string *
crypt_string_unpad(struct crypto_instance *c,
		   struct lsh_string *in,
		   int free);

#endif /* LSH_ABSTRACT_CRYPTO_H_INCLUDED */

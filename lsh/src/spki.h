/* spki.h
 *
 * An implementation of SPKI certificate checking
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

#ifndef LSH_SPKI_H_INCLUDED
#define LSH_SPKI_H_INCLUDED

#include "alist.h"
#include "dsa.h"
#include "exception.h"
#include "publickey_crypto.h"

#include <time.h>

/* Needed by spki.h.x */
/* SPKI validity. No online validity tests supported. */
struct spki_validity
{
  char before_limit; /* Nonzero if not_before was supplied */
  char after_limit;  /* Nonzero if not_after was supplied */
  time_t not_before;
  time_t not_after;
};

struct spki_5_tuple;

#define GABA_DECLARE
#include "spki.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name spki_exception)
     (super exception)
     (vars
       (expr object sexp)))
*/

struct exception *
make_spki_exception(UINT32 type, const char *msg, struct sexp *expr);

int spki_get_type(struct sexp *e, struct sexp_iterator **res);

struct sexp *
make_ssh_hostkey_tag(struct address_info *host);

struct verifier *
spki_make_verifier(struct alist *algorithms,
		   struct sexp *e);

struct signer *
spki_make_signer(struct alist *algorithms,
		 struct sexp *e,
		 int *type);

struct sexp *
spki_make_public_key(struct verifier *verifier);

struct sexp *
spki_hash_data(struct hash_algorithm *algorithm,
	       int algorithm_name,
	       UINT32 length, UINT8 *data);

/* At a point in time, not all fields are known; fields may be added
 * later, or computed as needed. This information is not automatically
 * trusted in any way, except that any non-NULL attributes must be
 * consistent with each other. */

/* GABA:
   (class
     (name spki_subject)
     (vars
       ; (public-key ...) expression.
       (key object sexp)

       ; Verifier
       (verifier object verifier)
       (sha1 string)
       (md5 string)))
*/

struct spki_subject *
make_spki_subject(struct sexp *key,
		  struct verifier *verifier,
		  struct lsh_string *sha1,
		  struct lsh_string *md5);

/* Keeps track of spki_subjects and their keys.
 *
 * We try to make sure that subjects within one context can be
 * compared pointer-wise. I.e. if we get several (public-key ...) and
 * (hash ...) expressions representing the same principal, we merge
 * them into a single spki_subject object. However, there is one case
 * in which this fails: If we encounter several (hash ...)
 * expressions with different hash algorithms, before we encounter the
 * non-hashed (public-key ...) expression. */

/* GABA:
   (class
     (name spki_context)
     (vars
       ; Looks up a public-key or hash.
       (lookup method (object spki_subject)
                      "struct sexp *e"
		      ; If non-NULL, use this verifier for
		      ; the subject. Useful for non-SPKI keys.
		      "struct verifier *v")
       (add_tuple method void
                  "struct spki_5_tuple *tuple")
       (authorize method int
                         "struct spki_subject *subject"
			 "struct sexp *access")))
       ;; (clone method (object spki_context))))
*/

#define SPKI_LOOKUP(c, e, v) ((c)->lookup((c), (e), (v)))
#define SPKI_ADD_TUPLE(c, t) ((c)->add_tuple((c), ((t))))
#define SPKI_AUTHORIZE(c, s, a) ((c)->authorize((c), (s), (a)))
#define SPKI_CLONE(c) ((c)->clone((c)))

struct spki_context *
make_spki_context(struct alist *algorithms);

/* 5-tuples */

#define SPKI_TAG_ATOM 1
#define SPKI_TAG_LIST 2
#define SPKI_TAG_SET 3
#define SPKI_TAG_PREFIX 4
#define SPKI_TAG_ANY 5

/* GABA:
   (class
     (name spki_tag)
     (vars
       ; Explicit type field is needed only for computing
       ; intersections
       (type . int)
       ; Returns true iff the resources described by the tag
       ; include the resource described by the sexp.
       (match method int "struct sexp *")))
*/

#define SPKI_TAG_TYPE(t) ((t)->type)
#define SPKI_TAG_MATCH(t, e) ((t)->match((t), (e)))

/* The data in a 5-tuple is always trusted, to the extent a non-NULL
 * issuer field implies that the tuple was derived from a certificate
 * that was properly signed by that issuer. However, no trust in the
 * issuer is assumed. */

/* GABA:
   (class
     (name spki_5_tuple)
     (vars
       ; Principal
       (issuer object spki_subject)
       ; Principal (n-to-k not yet supported)
       (subject object spki_subject)
       ; Non-zero to allow delegation
       (propagate . int)
       ; Authorization, (tag ...) expression
       (authorization object spki_tag)
       ; Validity period
       (validity . "struct spki_validity")))
       
*/

struct spki_5_tuple *
make_spki_5_tuple(struct spki_subject *issuer,
		  struct spki_subject *subject,
		  int propagate,
		  struct spki_tag *authorization,
		  int before_limit, time_t not_before,
		  int after_limit, time_t not_after);


struct spki_tag *
spki_sexp_to_tag(struct sexp *e,
		 /* Some limit on the recursion */
		 unsigned limit);

struct spki_5_tuple *
spki_acl_entry_to_5_tuple(struct spki_context *ctx,
			  struct sexp_iterator *i);

int
spki_add_acl(struct spki_context *ctx,
	     struct sexp *e);

#endif /* LSH_SPKI_H_INCLUDED */

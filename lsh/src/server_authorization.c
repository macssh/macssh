/* server_authorization.c
 *
 * user authorization database
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

#include "server_authorization.h"

#include "dsa.h"
#include "format.h"
#include "rsa.h"
#include "server_userauth.h"
#include "sexp.h"
#include "spki.h"
#include "xalloc.h"

#include <assert.h>

#include "server_authorization.c.x"

/* For now a key is authorized if a file named as the hash of the
   SPKI pubkey exists */

/* GABA:
   (class
     (name authorization_db)
     (super lookup_verifier)
     (vars
       (index_name string)
       ;; (signalgo object signature_algorithm)
       (hashalgo object hash_algorithm)))
*/

static struct verifier *
do_key_lookup(struct lookup_verifier *c,
	      int method,
	      struct lsh_user *keyholder,
	      struct lsh_string *key)
{
  CAST(authorization_db, closure, c);
  struct lsh_string *filename;

  struct verifier *v = NULL;

  assert(keyholder);

  switch (method)
    {
    default:
      return NULL;

      /* FIXME: SPKI support. */
      
    case ATOM_SSH_DSS:
      /* FIXME: Perhaps this is the right place to choose to apply the
       * PEER_SSH_DSS_KLUDGE? */
      
      v = make_ssh_dss_verifier(key->length, key->data);
      break;

    case ATOM_SSH_RSA:
      v = make_ssh_rsa_verifier(key->length, key->data);
      break;
    }

  if (!v)
    return NULL;
  
  /* FIXME: Proper spki acl reading should go here. */
  
  filename = ssh_cformat(".lsh/%lS/%lxfS", 
			 closure->index_name,
			 hash_string(closure->hashalgo,
				     sexp_format(spki_make_public_key(v),
						 SEXP_CANONICAL, 0),
				     1));
  
  if (USER_FILE_EXISTS(keyholder, filename, 1))
    return v;

  return NULL;
}

struct lookup_verifier *
make_authorization_db(struct lsh_string *index_name, 
		      /* struct signature_algorithm *s, */
		      struct hash_algorithm *h)
{
  NEW(authorization_db, res);

  res->super.lookup = do_key_lookup;
  res->index_name = index_name;
  /* res->signalgo = s; */
  res->hashalgo = h;

  return &res->super;
}

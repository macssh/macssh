/* spki_commands.c
 *
 * SPKI interface
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Niels Möller
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

#include "spki_commands.h"

#include "atoms.h"
#include "crypto.h"
#include "format.h"
#include "queue.h"
#include "randomness.h"
#include "sexp_commands.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

/* Forward declarations */
struct command_simple spki_add_acl_command;
#define SPKI_ADD_ACL (&spki_add_acl_command.super.super)

struct command_simple spki_return_hostkeys;
#define RETURN_HOSTKEYS (&spki_return_hostkeys.super.super)

struct command_simple spki_add_hostkey_command;
#define SPKI_ADD_HOSTKEY (&spki_add_hostkey_command.super.super)

struct command_simple spki_return_userkeys;
#define RETURN_USERKEYS (&spki_return_userkeys.super.super)

struct command_simple spki_add_userkey_command;
#define SPKI_ADD_USERKEY (&spki_add_userkey_command.super.super)


#include "spki_commands.c.x"

#define SA(x) sexp_a(ATOM_##x)

#define SPKI_ERROR(e, msg, expr) \
EXCEPTION_RAISE((e), make_spki_exception(EXC_SPKI_TYPE, (msg), (expr)))


/* Various conversion functions */

DEFINE_COMMAND_SIMPLE(spki_signer2verifier, a)
{
  CAST_SUBTYPE(signer, private, a);
  return &SIGNER_GET_VERIFIER(private)->super;
}

DEFINE_COMMAND_SIMPLE(spki_verifier2public, a)
{
  CAST_SUBTYPE(verifier, v, a);
  return &spki_make_public_key(v)->super;
}

/* Create an SPKI hash from an s-expression. */
/* GABA:
   (class
     (name spki_hash)
     (super command)
     (vars
       (name . int)
       (algorithm object hash_algorithm)))
*/

static void
do_spki_hash(struct command *s,
	     struct lsh_object *a,
	     struct command_continuation *c,
	     struct exception_handler *e UNUSED)
{
  CAST(spki_hash, self, s);
  CAST_SUBTYPE(sexp, o, a);

  struct lsh_string *tmp = hash_string(self->algorithm,
				       sexp_format(o, SEXP_CANONICAL, 0),
				       1);
  struct sexp *hash = spki_hash_data(self->algorithm, self->name, 
				     tmp->length, tmp->data);
  lsh_string_free(tmp);
  
  COMMAND_RETURN(c, hash);
}

struct command *
make_spki_hash(int name, struct hash_algorithm *algorithm)
{
  NEW(spki_hash, self);
  self->super.call = do_spki_hash;
  self->name = name;
  self->algorithm = algorithm;

  return &self->super;
}

const struct spki_hash spki_hash_md5 =
{ STATIC_COMMAND(do_spki_hash), ATOM_MD5, &md5_algorithm };

const struct spki_hash spki_hash_sha1 =
{ STATIC_COMMAND(do_spki_hash), ATOM_SHA1, &sha1_algorithm };


/* Reading keys */
  
/* Used for both sexp2keypair and sexp2signer. */

/* GABA:
   (class
     (name spki_parse_key)
     (super command)
     (vars
       (algorithms object alist)))
*/


static void
do_spki_sexp2signer(struct command *s, 
		    struct lsh_object *a,
		    struct command_continuation *c,
		    struct exception_handler *e)
{
  CAST(spki_parse_key, self, s);
  CAST_SUBTYPE(sexp, key, a);
  
  struct sexp_iterator *i;
  
  if (sexp_check_type(key, ATOM_PRIVATE_KEY, &i)) 
    {
      struct sexp *expr = SEXP_GET(i);
      struct signer *s;
      
      if (!expr)
	SPKI_ERROR(e, "spki.c: Invalid key.", key); 

      s = spki_make_signer(self->algorithms, expr, NULL);
      
      if (s)
	/* Test key here? */
	COMMAND_RETURN(c, s);
      else
	SPKI_ERROR(e, "spki.c: Invalid key.", expr); 
    }
  else
    SPKI_ERROR(e, "spki.c: Expected private-key expression.", key);
}

/* (parse algorithms sexp) -> signer */
DEFINE_COMMAND_SIMPLE(spki_sexp2signer_command, a)
{
  CAST_SUBTYPE(alist, algorithms, a);
  NEW(spki_parse_key, self);
  
  self->super.call = do_spki_sexp2signer;
  self->algorithms = algorithms;
  return &self->super.super;
}


static void 
parse_private_key(struct alist *algorithms,
                  struct sexp_iterator *i,
		  struct command_continuation *c,
		  struct exception_handler *e)
{
  struct sexp *expr = SEXP_GET(i);
  int algorithm_name;
  struct signer *s;
  struct verifier *v;
  struct lsh_string *spki_public;
  
  if (!expr)
    {
      werror("parse_private_key: Invalid key.\n");
      SPKI_ERROR(e, "spki.c: Invalid key.", expr); 
      return;
    }

  s = spki_make_signer(algorithms, expr, &algorithm_name);

  if (!s)
    {
      SPKI_ERROR(e, "spki.c: Invalid key.", expr); 
      return;

    }

  v = SIGNER_GET_VERIFIER(s);
  spki_public = sexp_format(spki_make_public_key(SIGNER_GET_VERIFIER(s)),
			    SEXP_CANONICAL, 0);
  
  /* Test key here? */  
  switch (algorithm_name)
    {	  
    case ATOM_DSA:
      COMMAND_RETURN(c, make_keypair(ATOM_SSH_DSS,
				     PUBLIC_KEY(v),
				     s));
      COMMAND_RETURN(c, make_keypair(ATOM_SPKI_SIGN_DSS,
				     spki_public, s));
      break;

    case ATOM_RSA_PKCS1_SHA1:
      COMMAND_RETURN(c, make_keypair(ATOM_SSH_RSA,
				     PUBLIC_KEY(v),
				     s));
      /* Fall through */

    case ATOM_RSA_PKCS1_MD5:
      COMMAND_RETURN(c, make_keypair(ATOM_SPKI_SIGN_RSA,
				     spki_public, s));
      break;
      
    default:
      abort();
#if 0      
      /* Get a corresponding public key. */
      COMMAND_RETURN(c, make_keypair
		     (ATOM_SPKI,
		      sexp_format(spki_make_public_key(SIGNER_GET_VERIFIER(s)),
				  SEXP_CANONICAL, 0),
		      s));
#endif
      
      break;
    }
}

static void
do_spki_sexp2keypair(struct command *s, 
		     struct lsh_object *a,
		     struct command_continuation *c,
		     struct exception_handler *e)
{
  CAST(spki_parse_key, self, s);
  CAST_SUBTYPE(sexp, key, a);
  
  struct sexp_iterator *i;
  
  switch (spki_get_type(key, &i)) 
    {
      default:
        SPKI_ERROR(e, "spki.c: Expected private-key expression.", key);
        return;
      case ATOM_PRIVATE_KEY:
	parse_private_key(self->algorithms, i, c, e);
	break;
    } 
}


/* (parse algorithms sexp) -> one or more keypairs */
DEFINE_COMMAND_SIMPLE(spki_sexp2keypair_command, a)
{
  CAST_SUBTYPE(alist, algorithms, a);
  NEW(spki_parse_key, self);
  
  self->super.call = do_spki_sexp2keypair;
  self->algorithms = algorithms;
  return &self->super.super;
}

/* GABA:
   (class
     (name spki_command)
     (super command)
     (vars
       (ctx object spki_context)))
*/

/* Reading of ACL:s
 * ****************/
 
/* Adds an ACL s-expression to an SPKI-context. Returns the context. */

static void
do_spki_add_acl(struct command *s,
		struct lsh_object *a,
		struct command_continuation *c,
		struct exception_handler *e UNUSED)
{
  CAST(spki_command, self, s);
  CAST_SUBTYPE(sexp, expr, a);

  trace("do_spki_add_acl\n");
  spki_add_acl(self->ctx, expr);

  COMMAND_RETURN(c, self->ctx);
}

DEFINE_COMMAND_SIMPLE(spki_add_acl_command, a)
{
  CAST_SUBTYPE(spki_context, ctx, a);

  NEW(spki_command, self);
  self->super.call = do_spki_add_acl;
  self->ctx = ctx;

  trace("spki_add_acl_command\n");

  return &self->super.super;
}

DEFINE_COMMAND_SIMPLE(spki_make_context_command, a)
{
  CAST_SUBTYPE(alist, algorithms, a);
  trace("spki_make_context_command\n");
  
  return &make_spki_context(algorithms)->super;
}


/* Reads a file of ACL:s, and returns an spki_context. */

/* GABA:
   (expr
     (name spki_read_acl)
     (params
       (algorithms object alist))
     (expr
       (lambda (file)
         (let ((ctx (spki_make_context
	              ;; Delay call, so that we really
		      ;; create one context for each file.
	              (prog1 algorithms file))))
	   (for_sexp
	     (lambda (e) ctx) ; Return ctx
  	     ;; Keep on reading until an SEXP_EOF or
	     ;; SEXP_SYNTAX exception is raised. 
	     (spki_add_acl ctx)
	     file)))))
*/

struct command *
make_spki_read_acls(struct alist *algorithms)
{
  CAST_SUBTYPE(command, res, spki_read_acl(algorithms));

  trace("make_spki_read_acl\n");
  return res;
}

DEFINE_COMMAND_SIMPLE(spki_read_acls_command, a)
{
  CAST_SUBTYPE(alist, algorithms, a);
  CAST_SUBTYPE(command, res, spki_read_acl(algorithms));

  return &res->super;
}
  
/* Reading of host-keys
 * ********************/

/* GABA:
   (class
     (name spki_read_hostkey_context)
     (super command)
     (vars
       (keys object alist)))
*/

static void
do_spki_add_hostkey(struct command *s,
		    struct lsh_object *a,
		    struct command_continuation *c,
		    struct exception_handler *e UNUSED)
{
  CAST(spki_read_hostkey_context, self, s);
  CAST(keypair, key, a);

  trace("do_spki_add_hostkey\n");
  
  if (ALIST_GET(self->keys, key->type))
    werror("Multiple host keys of type %a.\n", key->type);
  else
    ALIST_SET(self->keys, key->type, &key->super);
    
  COMMAND_RETURN(c, self->keys);
}

/* Ignores its argument */
DEFINE_COMMAND_SIMPLE(spki_add_hostkey_command, a)
{
  NEW(spki_read_hostkey_context, self);

  trace("spki_add_hostkey_command\n");

  (void) a;
  
  self->super.call = do_spki_add_hostkey;
  self->keys = make_alist(0, -1);

  return &self->super.super;
}     

DEFINE_COMMAND_SIMPLE(spki_return_hostkeys, a)
{
  CAST(spki_read_hostkey_context, self, a);
  trace("spki_return_hostkeys\n");
  return &self->keys->super;
}

/* GABA:
   (expr
     (name spki_read_hostkeys)
     (params
       (algorithms object alist))
     (expr
       (lambda (file)
         (let ((add (spki_add_hostkey file)))
           (for_sexp (lambda (e)
	   		;; Delay return until we actually get an exception
			(return_hostkeys (prog1 add e)))
	             (lambda (key)
		       (add (sexp2keypair
		               algorithms key)))
		     file)))))
*/

DEFINE_COMMAND_SIMPLE(spki_read_hostkeys_command, a)
{
  CAST_SUBTYPE(alist, algorithms, a);
  CAST_SUBTYPE(command, res, spki_read_hostkeys(algorithms));

  trace("spki_read_hostkeys_command\n");
  
  return &res->super;
}

/* Reading of private user-keys
 * ****************************/

/* GABA:
   (class
     (name spki_read_userkey_context)
     (super command)
     (vars
       (keys struct object_queue)))
*/

static void
do_spki_add_userkey(struct command *s,
		    struct lsh_object *a,
		    struct command_continuation *c,
		    struct exception_handler *e UNUSED)
{
  CAST(spki_read_userkey_context, self, s);
  CAST(keypair, key, a);

  trace("do_spki_add_userkey\n");
  
  object_queue_add_tail(&self->keys, &key->super);

  COMMAND_RETURN(c, s);
}

/* Ignores its argument */
DEFINE_COMMAND_SIMPLE(spki_add_userkey_command, a)
{
  NEW(spki_read_userkey_context, self);
  (void) a;

  trace("spki_add_userkey_command\n");
  self->super.call = do_spki_add_userkey;
  object_queue_init(&self->keys);

  return &self->super.super;
}     

DEFINE_COMMAND_SIMPLE(spki_return_userkeys, a)
{
  CAST(spki_read_userkey_context, self, a);
  trace("spki_return_userkeys\n");
  
  return &queue_to_list(&self->keys)->super.super;
}

/* GABA:
   (expr
     (name spki_read_userkeys)
     (params
       (algorithms object alist)
       (decrypt object command))
     (expr
       (lambda (file)
         (let ((ctx (spki_add_userkey file)))
           (for_sexp (lambda (e)
	   		;; Delay return until we actually get an exception
			(return_userkeys (prog1 ctx e)))
	             (lambda (key)
		       (ctx (sexp2keypair
		               algorithms (decrypt key))))
		     file)))))
*/

struct command *
make_spki_read_userkeys(struct alist *algorithms,
			struct alist *signature_algorithms,
			struct interact *tty)
{
  struct command *decrypt;
  trace("make_spki_read_userkeys\n");

  if (tty)
    {
      struct alist *mac = make_alist(0, -1);
      struct alist *crypto = make_alist(0, -1);

      alist_select_l(mac, algorithms,
		     2, ATOM_HMAC_SHA1, ATOM_HMAC_MD5, -1);
      alist_select_l(crypto, algorithms,
		     3, ATOM_3DES_CBC, ATOM_BLOWFISH_CBC,
		     ATOM_RIJNDAEL_CBC_LOCAL, -1);
      decrypt = make_pkcs5_decrypt(mac, crypto, tty);
    }
  else
    decrypt = &command_I.super;

  {
    CAST_SUBTYPE(command, res,
		 spki_read_userkeys(signature_algorithms,
				    decrypt));

    return res;
  }
}

/* Encryption of private data.
 * For PKCS#5 (version 2) key derivation, we use
 *
 * (password-encrypted LABEL
 *   (Xpkcs5v2 hmac-sha1 (salt #...#)
 *                       (iterations #...#))
 *   ("3des-cbc" (iv #...#) (data #...#)))
 *
 * where the X:s will be removed when the format is more stable.
 *
 */

/* GABA:
   (class
     (name spki_password_encrypt)
     (super command)
     (vars
       (label string)
       (method object sexp)
       (algorithm_name . int)
       (algorithm object crypto_algorithm)
       (r object randomness)
       (key string)))
*/

static void
do_spki_encrypt(struct command *s,
		struct lsh_object *a,
		struct command_continuation *c,
		struct exception_handler *e UNUSED)
{
  CAST(spki_password_encrypt, self, s);
  CAST_SUBTYPE(sexp, expr, a);

  struct lsh_string *iv = NULL;
  
  if (self->algorithm->iv_size)
    {
      iv = lsh_string_alloc(self->algorithm->iv_size);
      RANDOM(self->r, iv->length, iv->data);
    }
  
  COMMAND_RETURN(c,
		 sexp_l(4,
			SA(PASSWORD_ENCRYPTED),
			sexp_s(NULL, lsh_string_dup(self->label)),
			self->method,
			sexp_l(3,
			       sexp_a(self->algorithm_name),
			       sexp_l(2, SA(IV), sexp_s(NULL, iv), -1),
			       sexp_l(2, SA(DATA),
				      sexp_s(NULL, crypt_string_pad
					     (MAKE_ENCRYPT(self->algorithm,
							   self->key->data, iv ? iv->data : NULL),
					      sexp_format(expr, SEXP_CANONICAL, 0), 1)),
				      -1),
			       -1),
			-1));
}

/* Consumes the label and password arguments. */
struct command *
make_pkcs5_encrypt(struct randomness *r,
		   struct lsh_string *label,
		   UINT32 prf_name,
		   struct mac_algorithm *prf,
		   int crypto_name,
		   struct crypto_algorithm *crypto,
		   UINT32 salt_length,
		   struct lsh_string *password,
		   UINT32 iterations)
{
  NEW(spki_password_encrypt, self);

  struct lsh_string *key;
  struct lsh_string *salt;
    
  assert(crypto);
  assert(prf);

  salt = lsh_string_alloc(salt_length);
  RANDOM(r, salt->length, salt->data);
    
  key = lsh_string_alloc(crypto->key_size);

  pkcs5_derive_key(prf,
		   password->length, password->data,
		   salt->length, salt->data,
		   iterations,
		   key->length, key->data);

  lsh_string_free(password);
  
  self->super.call = do_spki_encrypt;
  self->r = r;
  self->label = label;
  self->method = sexp_l(4, SA(XPKCS5V2), sexp_a(prf_name),
			sexp_l(2, SA(ITERATIONS), sexp_uint32(iterations), -1),
			sexp_l(2, SA(SALT), sexp_s(NULL, salt), -1), -1);
  self->algorithm_name = crypto_name;
  self->algorithm = crypto;
  self->key = key;

  return &self->super;
}

/* GABA:
   (class
     (name spki_password_decrypt)
     (super command)
     (vars
       (mac_algorithms object alist)
       (crypto_algorithms object alist)
       (tty object interact)))
*/

static void
do_spki_decrypt(struct command *s,
		struct lsh_object *a,
		struct command_continuation *c,
		struct exception_handler *e)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
#if MACOS
  UINT8 *key;
#endif
  CAST(spki_password_decrypt, self, s);
  CAST_SUBTYPE(sexp, expr, a);

  struct sexp_iterator *i;
  
  if (!sexp_check_type(expr, ATOM_PASSWORD_ENCRYPTED, &i))
    COMMAND_RETURN(c, expr);

  else
    {
      struct lsh_string *label;
      struct sexp *key_info;
      struct sexp *payload;

      struct crypto_algorithm *crypto;
      struct mac_algorithm *mac;
      
      struct lsh_string *salt;
      struct lsh_string *iv;
      struct lsh_string *data;
      UINT32 iterations;
      
      if (SEXP_LEFT(i) != 3)
	{
	  SPKI_ERROR(e, "Invalid (password-encrypted ...) expression.",
		     expr);
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return;
	}
	
      /* NOTE: This is a place where it might make sense to use a sexp
       * display type, but we don't support that for now. */
      label = sexp2string(SEXP_GET(i));

      if (!label)
	{
	  SPKI_ERROR(e, "Invalid label in (password-encrypted ...) expression.",
		     expr);
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return;
	}

      SEXP_NEXT(i);
      key_info = SEXP_GET(i);
      assert(key_info);

      SEXP_NEXT(i);
      payload = SEXP_GET(i);
      assert(payload);

      /* Examine the payload expression first, before asking for a
       * pass phrase. */

      {
	int algorithm_name = spki_get_type(payload, &i);
	CAST_SUBTYPE(crypto_algorithm, tmp,
		     ALIST_GET(self->crypto_algorithms, algorithm_name));
	crypto = tmp;
      }

      if (!crypto)
	{
	  SPKI_ERROR(e, "Unknown encryption algorithm for pkcs5v2.", payload);
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return;
	}

      iv = sexp2string(sexp_assq(i, ATOM_IV));

      if (crypto->iv_size)
	{
	  if (!iv || (iv->length != crypto->iv_size))
	    {
	      SPKI_ERROR(e, "Invalid IV for pkcs5v2.", payload);
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }
	}
      else if (iv)
	{
	  if (iv->length)
	    {
	      SPKI_ERROR(e, "Unexpected iv provided for pkcs5v2.", payload);
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }
	  iv = NULL;
	}
	
      data = sexp2string(sexp_assq(i, ATOM_DATA));

      if (!data)
	{
	  SPKI_ERROR(e, "Payload data missing for pkcs5v2.", payload);
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return;
	}

      if (crypto->block_size && (data->length & crypto->block_size))
	{
	  SPKI_ERROR(e, "Payload data doesn't match block size for pkcs5v2.", payload);
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return;
	}

      /* Get key */
      switch (spki_get_type(key_info, &i)) 
	{
	default:
	  SPKI_ERROR(e, "Unknown key derivation mechanism.", key_info);
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return;

	case ATOM_XPKCS5V2:
	  if (SEXP_LEFT(i) != 3)
	    {
	      SPKI_ERROR(e, "Invalid pkcs5v2 parameters.", key_info);
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }
	  
	  {
	    int algorithm_name = sexp2atom(SEXP_GET(i));
	    
	    CAST_SUBTYPE(mac_algorithm, tmp,
			 ALIST_GET(self->mac_algorithms,
				   algorithm_name));

	    mac = tmp;
	  }

	  if (!mac)
	    {
	      SPKI_ERROR(e, "Unknown mac for pkcs5v2.", key_info);
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }

	  SEXP_NEXT(i);
	  if (!sexp2uint32(sexp_assq(i, ATOM_ITERATIONS), &iterations)
	      || !iterations)
	    {
	      SPKI_ERROR(e, "Invalid iteration count for pkcs5v2.", key_info);
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }
	    
	  salt = sexp2string(sexp_assq(i, ATOM_SALT));

	  if (!salt)
	    {
	      SPKI_ERROR(e, "Invalid salt for pkcs5v2.", key_info);
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }

	  /* Do the work */



#if MACOS
	  key = alloca(crypto->key_size);
      while ( 1 )
#endif
	  {
	    struct lsh_string *password
	      = INTERACT_READ_PASSWORD(self->tty, 500, label, 0);
	    struct lsh_string *clear;
	    struct sexp *res;
#if !MACOS
	    UINT8 *key;
#endif
	    
	    if (!password)
	      {
		SPKI_ERROR(e, "No password provided for pkcs5v2.", key_info);
#if ALLOCA_68K_BUG
		ALLOCA_FREE(alloca_ref);
#endif
		return;
	      }

#if !MACOS
	    key = alloca(crypto->key_size);
#endif
	    pkcs5_derive_key(mac,
			     password->length, password->data,
			     salt->length, salt->data,
			     iterations,
			     crypto->key_size, key);

	    lsh_string_free(password);

	    clear = crypt_string_unpad(MAKE_DECRYPT(crypto,
						    key,
						    iv ? iv->data : NULL),
				       data, 0);

	    if (!clear)
	      {
#if MACOS
		  continue;
#endif
		SPKI_ERROR(e, "Bad password for pkcs5v2.", key_info);
#if ALLOCA_68K_BUG
		ALLOCA_FREE(alloca_ref);
#endif
		return;
	      }

	    res = string_to_sexp(SEXP_CANONICAL, clear, 1);

	    if (res)
	      COMMAND_RETURN(c, res);
	    else
	      {
#if MACOS
		  continue;
#endif
		SPKI_ERROR(e, "Bad password for pkcs5v2.", key_info);
#if ALLOCA_68K_BUG
		ALLOCA_FREE(alloca_ref);
#endif
		return;
	      }
#if MACOS
	      break;
#endif
	  }
	}
    }
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

struct command *
make_pkcs5_decrypt(struct alist *mac_algorithms,
		   struct alist *crypto_algorithms,
		   struct interact *tty)
{
  NEW(spki_password_decrypt, self);
  self->super.call = do_spki_decrypt;
  self->mac_algorithms = mac_algorithms;
  self->crypto_algorithms = crypto_algorithms;
  self->tty = tty;

  return &self->super;
}

/* algorithms.c
 *
 * Translate algorithm identifiers (or names) to algorithm objects.
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

#include "algorithms.h"

#include "atoms.h"
#include "compress.h"
#include "crypto.h"
#include "dsa.h"
#include "rsa.h"
#include "xalloc.h"

#include "lsh_argp.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#if MACOS
extern int strcasecmp(const char * str1, const char * str2);
#endif

#define GABA_DEFINE
#include "algorithms.h.x"
#undef GABA_DEFINE

struct alist *
all_symmetric_algorithms()
{
  return make_alist(9
#if WITH_IDEA
		    +1
#endif
#if WITH_ZLIB
		    +1
#endif
		    ,
		    ATOM_ARCFOUR, &crypto_arcfour_algorithm,
		    ATOM_BLOWFISH_CBC, crypto_cbc(&blowfish_algorithm),
		    ATOM_TWOFISH_CBC, crypto_cbc(&twofish256_algorithm),
		    ATOM_RIJNDAEL_CBC_LOCAL, crypto_cbc(&rijndael256_algorithm),
		    ATOM_SERPENT_CBC_LOCAL, crypto_cbc(&serpent256_algorithm),
		    ATOM_3DES_CBC, crypto_cbc(make_des3()),
		    ATOM_CAST128_CBC, crypto_cbc(&cast128_algorithm),
#if WITH_IDEA
		    ATOM_IDEA_CBC, crypto_cbc(&idea_algorithm),
#endif
		    ATOM_HMAC_SHA1, make_hmac_algorithm(&sha1_algorithm),
		    ATOM_HMAC_MD5, make_hmac_algorithm(&md5_algorithm),
#if WITH_ZLIB
		    ATOM_ZLIB, make_zlib(),
#endif
		    -1);
}

/* This is used for spki operations, and should therefore use spki names. */
struct alist *
all_signature_algorithms(struct randomness *r)
{
  return make_alist(3,
		    ATOM_DSA, make_dsa_algorithm(r),
		    ATOM_RSA_PKCS1_SHA1, &rsa_sha1_algorithm.super,
		    ATOM_RSA_PKCS1_MD5, &rsa_md5_algorithm.super,
		    -1);
}

/* Forward declaration */
struct int_list *
filter_algorithms_l(struct alist *algorithms, unsigned n, ...);

/* Includes only reasonably old algorithms and well studied
 * algorithms. */
static struct int_list *
default_crypto_algorithms(struct alist *algorithms)
{
  return filter_algorithms_l(algorithms, 3,
			     ATOM_3DES_CBC,
			     ATOM_BLOWFISH_CBC,
			     ATOM_ARCFOUR, -1);
}

/* Includes all supported algorithms, except none. In effect, the
 * peer is trusted in choosing an adequate algorithm. */
static struct int_list *
all_crypto_algorithms(struct alist *algorithms)
{
  return filter_algorithms_l(algorithms, 8,
			     ATOM_3DES_CBC,
			     ATOM_TWOFISH_CBC, 			   
			     ATOM_CAST128_CBC,
			     ATOM_SERPENT_CBC_LOCAL,
			     ATOM_RIJNDAEL_CBC_LOCAL,
			     ATOM_IDEA_CBC,
			     ATOM_BLOWFISH_CBC,
			     ATOM_ARCFOUR, -1);
}

static struct int_list *
default_mac_algorithms(struct alist *algorithms)
{
  return filter_algorithms_l(algorithms, 2, ATOM_HMAC_SHA1, ATOM_HMAC_MD5, -1);
}

static struct int_list *
default_compression_algorithms(struct alist *algorithms)
{
  return filter_algorithms_l(algorithms, 2, ATOM_NONE, ATOM_ZLIB, -1);
}

static struct int_list *
prefer_compression_algorithms(struct alist *algorithms)
{
  return filter_algorithms_l(algorithms, 2, ATOM_ZLIB, ATOM_NONE, -1);
}


/* This is not really efficient, but it doesn't matter. */
static int strcasecmp_list(const char *name, ...)
{
  va_list args;
  char *s;
  int res = 0;
  
  va_start(args, name);
  while ( (s = va_arg(args, char *)) )
    {
      if (!strcasecmp(name, s))
	{
	  res = 1;
	  break;
	}
    }
  va_end(args);

  return res;
}
    
int
lookup_crypto(struct alist *algorithms, const char *name, struct crypto_algorithm **ap)
{
  int atom;

  if (!strcasecmp(name, "none"))
    {
      if (ap)
	*ap = NULL;
      
      return ATOM_NONE;
    }
  else if (strcasecmp_list(name, "arcfour", NULL))
    atom = ATOM_ARCFOUR;
  else if (strcasecmp_list(name, "twofish-cbc", "twofish", NULL))
    atom = ATOM_TWOFISH_CBC;
  else if (strcasecmp_list(name, "blowfish-cbc", "blowfish", NULL))
    atom = ATOM_BLOWFISH_CBC;
  else if (strcasecmp_list(name, "3des-cbc", "3des", NULL))
    atom = ATOM_3DES_CBC;
  else if (strcasecmp_list(name, "rijndael-cbc@lysator.liu.se",
			   "rijndael-cbc", "rijndael", NULL))
    atom = ATOM_RIJNDAEL_CBC_LOCAL;
  else if (strcasecmp_list(name, "serpent-cbc@lysator.liu.se",
			   "serpent-cbc", "serpent", NULL))
    atom = ATOM_SERPENT_CBC_LOCAL;
  else if (strcasecmp_list(name, "idea-cbc", "idea", NULL))
    atom = ATOM_IDEA_CBC;
  else if (strcasecmp_list(name, "cast128-cbc", "cast",
		       "cast-cbc", "cast128", NULL))
    atom = ATOM_CAST128_CBC;
  else
    return 0;

  /* Is this crypto supported? */
  {
    CAST_SUBTYPE(crypto_algorithm, a, ALIST_GET(algorithms, atom));
    if (a)
      {
	if (ap)
	  *ap = a;

	return atom;
      }
    else
      return 0;
  }
}

int
lookup_mac(struct alist *algorithms, const char *name, struct mac_algorithm **ap)
{
  int atom;

  if (!strcasecmp(name, "none"))
    {
      if (ap)
	*ap = NULL;

      return ATOM_NONE;
    }
  if (strcasecmp_list(name, "hmac-sha1", "sha", "hmac-sha", "sha1", NULL))
    atom = ATOM_HMAC_SHA1;
  else if (strcasecmp_list(name, "hmac-md5", "md5", NULL))
    atom = ATOM_HMAC_MD5;
  else
    return 0;
  
  /* Is this mac supported? */
  {
    CAST_SUBTYPE(mac_algorithm, a, ALIST_GET(algorithms, atom));
    if (a)
      {
	if (ap)
	  *ap = a;

	return atom;
      }
    else
      return 0;
  }
}

int
lookup_compression(struct alist *algorithms, const char *name, struct compress_algorithm **ap)
{
  int atom;

  if (!strcasecmp(name, "none"))
    {
      if (ap)
	*ap = NULL;

      return ATOM_NONE;
    }
  if (strcasecmp_list(name, "zlib", "z", NULL))
    atom = ATOM_ZLIB;
  else
    return 0;
  
  /* Is this compression algorithm supported? */
  {
    CAST_SUBTYPE(compress_algorithm, a, ALIST_GET(algorithms, atom));
    if (a)
      {
	if (ap)
	  *ap = a;

	return atom;
      }
    else
      return 0;
  }
}

int
lookup_hostkey_algorithm(const char *name)
{
  if (!strcasecmp(name, "none"))
    return ATOM_NONE;
  else if (strcasecmp_list(name, "ssh-dss", "dsa", "dss", NULL))
    return ATOM_SSH_DSS;
  else if (strcasecmp_list(name, "spki-sign-rsa", "spki-rsa", NULL))
    return ATOM_SPKI_SIGN_RSA;
  else if (strcasecmp_list(name, "spki-sign-dss", "spki-dss",
			   "spki-sign-dsa", "spki-dsa", NULL))
    return ATOM_SPKI_SIGN_DSS;
  else
    return 0;
}

/* This function is used by sexp-conv */
int
lookup_hash(struct alist *algorithms, const char *name,
	    struct hash_algorithm **ap, int none_is_valid)
{
  int atom;

  if (none_is_valid && !strcasecmp(name, "none"))
    {
      if (ap)
	*ap = NULL;
      
      return ATOM_NONE;
    }
  if (strcasecmp_list(name, "md5", NULL))
    atom = ATOM_MD5;
  else if (strcasecmp_list(name, "sha1", NULL))
    atom = ATOM_SHA1;
  else
    return 0;

  /* Is this hash algorithm supported? */
  {
    CAST_SUBTYPE(hash_algorithm, a, ALIST_GET(algorithms, atom));
    if (a)
      {
	if (ap)
	  *ap = a;
	
	return atom;
      }
    else
      return 0;
  }
}

/* FIXME: Perhaps this function belongs in list.c or alist.c? */

/* Return an int list containing the elements of CANDIDATES
 * that have associated values in ALGORITHMS.
 * Returns a non-empty list or NULL. */
 
struct int_list *
filter_algorithms(struct alist *algorithms,
		  const struct int_list *candidates)
{
  struct int_list *l;
  unsigned i, j;
  unsigned supported;
  
  for (i = 0, supported = 0; i < LIST_LENGTH(candidates); i++)
    {
      int atom = LIST(candidates)[i];
      if (ALIST_GET(algorithms, atom)
	  || (atom == ATOM_NONE))
	supported++;
    }

  if (!supported)
    return NULL;
  
  l = alloc_int_list(supported);

  for (i = j = 0; i < LIST_LENGTH(candidates); i++)
    {
      int atom = LIST(candidates)[i];
      if (ALIST_GET(algorithms, atom)
	  || (atom == ATOM_NONE))
	{
	  assert(j < supported);
	  LIST(l)[j++] = atom;
	}
    }
  assert(j == supported);

  return l;
}


struct int_list *
filter_algorithms_l(struct alist *algorithms, unsigned n, ...)
{
  va_list args;
  struct int_list *l;
  struct int_list *candidates;
  
  va_start(args, n);
  candidates = make_int_listv(n, args);
  va_end(args);
  
  l = filter_algorithms(algorithms, candidates);

  assert(LIST_LENGTH(l));
  
  KILL(candidates);
  return l;
}


static void
list_algorithms(const struct argp_state *state,
		char *prefix,
		struct int_list *algorithms,
		int none)
{
  unsigned i;
  int separate;
  
  fprintf(state->out_stream, "%s", prefix);

  for (i = 0, separate = 0; i<LIST_LENGTH(algorithms); i++)
    {
      if (separate)
	fprintf(state->out_stream, ", ");
      
      fprintf(state->out_stream, "%s",
	      /* NOTE: This is the only place where we use that
	       * atom names are NUL-terminated. */
	      get_atom_name(LIST(algorithms)[i]));
      separate = 1;
    }

  if (none)
    {
      if (separate)
	fprintf(state->out_stream, ", ");
      fprintf(state->out_stream, "none");
    }
}

void
list_crypto_algorithms(const struct argp_state *state,
		       struct alist *algorithms)
{
  list_algorithms(state,
		  "Supported crypto algorithms: ",
		  all_crypto_algorithms(algorithms),
		  1);
}

void
list_mac_algorithms(const struct argp_state *state,
		    struct alist *algorithms)
{
  list_algorithms(state,
		  "Supported MAC algorithms: ",
		  default_mac_algorithms(algorithms),
		  1);
}

void
list_compression_algorithms(const struct argp_state *state,
			    struct alist *algorithms)
{
  list_algorithms(state, 
		  "Supported compression algorithms: ",
		  default_compression_algorithms(algorithms),
		  0);
}

void
list_hostkey_algorithms(const struct argp_state *state)
{
  fprintf(state->out_stream, "%s", "Supported hostkey algorithms: ssh-dss, spki, none");
}


#define OPT_LIST_ALGORITHMS 0x100
#define OPT_HOSTKEY_ALGORITHMS 0x101

static const struct argp_option
algorithms_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { NULL, 0, NULL, 0, "Algorithm selection:", 0},
  { "crypto", 'c', "Algorithm", 0, "", 0 },
  { "compression", 'z', "Algorithm",
    OPTION_ARG_OPTIONAL, "Default is zlib.", 0 },
  { "mac", 'm', "Algorithm", 0, "", 0 },
  { "hostkey-algorithm", OPT_HOSTKEY_ALGORITHMS, "Algorithm", 0, "", 0 }, 
  { "list-algorithms", OPT_LIST_ALGORITHMS, NULL, 0,
    "List supported algorithms.", 0 },
  { NULL, 0, NULL, 0, NULL, 0 }  
};

void
init_algorithms_options(struct algorithms_options *self,
			struct alist *algorithms)
{
  self->algorithms = algorithms;

  self->crypto_algorithms = NULL;
  self->mac_algorithms = NULL;
  self->compression_algorithms = NULL;
  self->hostkey_algorithms = NULL;
}

struct algorithms_options *
make_algorithms_options(struct alist *algorithms)
{
  NEW(algorithms_options, self);
  init_algorithms_options(self, algorithms);

  return self;
}

static error_t
algorithms_argp_parser(int key, char *arg, struct argp_state *state)
{
  CAST_SUBTYPE(algorithms_options, self, state->input);
  
  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_END:
      if (!self->crypto_algorithms)
	self->crypto_algorithms = default_crypto_algorithms(self->algorithms);
      if (!self->mac_algorithms)
	self->mac_algorithms = default_mac_algorithms(self->algorithms);
      if (!self->compression_algorithms)
	self->compression_algorithms = default_compression_algorithms(self->algorithms);
      if (!self->hostkey_algorithms)
	self->hostkey_algorithms = make_int_list(4, ATOM_SSH_RSA, ATOM_SSH_DSS,
						 ATOM_SPKI_SIGN_RSA, ATOM_SPKI_SIGN_DSS, -1);
      break;
    case 'c':
      {
	int crypto = lookup_crypto(self->algorithms, arg, NULL);

	if (crypto)
	  self->crypto_algorithms = make_int_list(1, crypto, -1);
	else if (strcasecmp_list(arg, "all", "any", NULL))
	  self->crypto_algorithms = all_crypto_algorithms(self->algorithms);
	
	else
	  {
	    list_crypto_algorithms(state, self->algorithms);
	    argp_error(state, "Unknown crypto algorithm '%s'.", arg);
	  }
	break;
      }
    case 'm':
      {
	int mac = lookup_mac(self->algorithms, arg, NULL);
	if (mac)
	  self->mac_algorithms = make_int_list(1, mac, -1);
	else
	  {
	    list_mac_algorithms(state, self->algorithms);
	    argp_error(state, "Unknown message authentication algorithm '%s'.", arg);
	  }	
	break;
      }
    case 'z':
      {
	if (!arg)
	  self->compression_algorithms = prefer_compression_algorithms(self->algorithms);
	else
	  {
	    int compression = lookup_compression(self->algorithms, arg, NULL);
	    if (compression)
	      self->compression_algorithms = make_int_list(1, compression, -1);
	    else
	      {
		list_compression_algorithms(state, self->algorithms);
		argp_error(state, "Unknown compression algorithm '%s'.", arg);
	      }
	  }
	break;
      }
    case OPT_HOSTKEY_ALGORITHMS:
      {
	int algorithm = lookup_hostkey_algorithm(arg);
	if (algorithm)
	  self->hostkey_algorithms = make_int_list(1, algorithm, -1);
	else
	  {
	    list_hostkey_algorithms(state);
	    argp_error(state, "Unknown hostkey algorithm '%s'.", arg);
	  }	
	break;
	
      }
      
    case OPT_LIST_ALGORITHMS:
      list_crypto_algorithms(state, self->algorithms);
      list_compression_algorithms(state, self->algorithms);
      list_mac_algorithms(state, self->algorithms);
      list_hostkey_algorithms(state);
      
      if (! (state->flags & ARGP_NO_EXIT))
	exit (0);
    }
  return 0;
}

const struct argp algorithms_argp =
{
  algorithms_options,
  algorithms_argp_parser,
  NULL, NULL, NULL, NULL, NULL
};

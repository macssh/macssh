/* lsh-keygen.c
 *
 * Generic key-generation program. Writes a spki-packaged private key
 * on stdout. You would usually pipe this to some other program to
 * extract the public key, encrypt the private key, and save the
 * results in two separate files.
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

#include "lshprefix.h"
extern void exit(int status);
extern void abort();
extern int strcasecmp(const char * str1, const char * str2);

#include "crypto.h"
#include "dsa.h"
#include "format.h"
#include "io.h"
#include "publickey_crypto.h"
#include "randomness.h"
#include "rsa.h"
#include "sexp.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include "lsh_argp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef MACOS
#include <QuickDraw.h>
#endif

#include "lsh-keygen.c.x"

/* Option parsing */

/* NONO: already defined in lsh
const char *argp_program_version
= "lsh-keygen-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;
*/

/* GABA:
   (class
     (name lsh_keygen_options)
     (vars
       (style . sexp_argp_state)
       ; 'd' means dsa, 'r' rsa
       (algorithm . int)
       (level . int)))
*/

static struct lsh_keygen_options *
make_lsh_keygen_options(void)
{
  NEW(lsh_keygen_options, self);
  self->style = SEXP_TRANSPORT;
  self->level = -1;
  self->algorithm = 'd';
  return self;
}

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "algorithm", 'a', "Algorithm", 0, "DSA or RSA. "
    "Default is to generate DSA keys", 0 },
  { "nist-level", 'l', "Security level", 0, "For DSA keys, this is the "
    "NIST security level: Level 0 uses 512-bit primes, level 8 uses "
    "1024 bit primes, and the default is 8. For RSA keys, it's the "
    "bit length of the modulus, and the default is 2048 bits.", 0 },
  { NULL, 0, NULL, 0, NULL, 0 }
};

static const struct argp_child
main_argp_children[] =
{
  { &sexp_output_argp, 0, NULL, 0 },
  { &werror_argp, 0, "", 0 },
  { NULL, 0, NULL, 0}
};

static error_t
main_argp_parser(int key, char *arg, struct argp_state *state)
{
  CAST(lsh_keygen_options, self, state->input);

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->style;
      state->child_inputs[1] = NULL;
      break;

    case ARGP_KEY_END:
      switch (self->algorithm)
	{
	case 'd':
	  if (self->level < 0)
	    self->level = 8;
	  else if (self->level > 8)
	    argp_error(state, "Security level for DSA should be in the range 0-8.");
	  
	  break;
	case 'r':
	  if (self->level < 0)
	    self->level = 2048;
	  else if (self->level < 512)
	    argp_error(state, "RSA keys should be at least 512 bits.");
	  break;
	default:
	  fatal("Internal error!\n");
	}
      break;
	  
    case 'l':
	{
	  char *end;
	  long l = strtol(arg, &end, 0);
	      
	  if (!*arg || *end)
	    {
	      argp_error(state, "Invalid security level.");
	      break;
	    }
	  if (l<0) 
	    {
	      argp_error(state, "Security level can't be negative.");
	      break;
	    }
	  self->level = l;
	  break;
	}

    case 'a':
      if (!strcasecmp(arg, "dsa"))
	self->algorithm = 'd';
      else if (!strcasecmp(arg, "rsa"))
	self->algorithm = 'r';
      else
	argp_error(state, "Unknown algorithm. The supported algorithms are "
		   "RSA and DSA.");

      break;
      
    }
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser, 
  "[-l LEVEL] [-a ALGORITHM]",
  ( "Generates a new private key for the given algorithm and security level.\v"
    "You will usually want to pipe the new key into a program like lsh-writekey, "
    "to split it into its private and public parts, and optionally encrypt "
    "the private information."),
  main_argp_children,
  NULL, NULL
};

static void
do_lsh_keygen_handler(struct exception_handler *s UNUSED,
		      const struct exception *e)
{
  werror("lsh-keygen: %z\n", e->msg);

  exit(EXIT_FAILURE);
}

static struct exception_handler handler =
STATIC_EXCEPTION_HANDLER(do_lsh_keygen_handler, NULL);

#ifdef MACOS
long random()
{
	return (Random() << 16) + Random();
}
#endif

#ifdef MACOS
//char *applname = "lsh-keygen";
//char *defargstr = "";
int kgappl_main(int argc, char **argv);
#define main kgappl_main
#endif

int main(int argc, char **argv)
{
  struct lsh_keygen_options * options
    = make_lsh_keygen_options();
  
  struct sexp *key;
  struct randomness_with_poll *r;

  struct lsh_string *out;
  const struct exception *e;
  
  argp_parse(&main_argp, argc, argv, 0, NULL, options);
  
  r = make_default_random(NULL, &handler);

  switch (options->algorithm)
    {
    case 'd':
      key = dsa_generate_key(&r->super, options->level);
      break;
    case 'r':
      {
	mpz_t e;
	mpz_init(e);

	for (;;)
	  {
	    /* Use a reasonably small random e, and make sure that at
	     * it is odd and has at most one more one bit. */
	    bignum_random_size(e, &r->super, 30);
	    mpz_setbit(e, 0);
	    mpz_setbit(e, 17);
	    
	    key = rsa_generate_key(e, &r->super, options->level);
	    if (key)
	      break;

	    verbose("Bad choice of public exponent. Trying another...\n");
	  }

	mpz_clear(e);
      }
      break;
    default:
      fatal("Internal error!\n");
    }

    /* Now, output a private key spki structure. */

  out = sexp_format(key, options->style, 0);
  e = write_raw(STDOUT_FILENO, out->length, out->data);

  if (e)
    {
      werror("lsh-keygen: %z\n", e->msg);
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }

  

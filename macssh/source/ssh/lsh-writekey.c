/* lsh-writekey.c
 *
 * Reads a (private) key on stdin, and saves it a private and a public file.
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

#include "algorithms.h"
#include "crypto.h"
#include "dsa.h"
#include "format.h"
#include "io_commands.h"
#include "interact.h"
#include "rsa.h"
#include "sexp_commands.h"
#include "spki_commands.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#define BLOCK_SIZE 2000

static struct read_sexp_command read_sexp
= STATIC_READ_SEXP(SEXP_TRANSPORT, 0);

#define READ_SEXP (&read_sexp.super.super)

struct lsh_writekey_options;

extern struct command_simple lsh_writekey_print_public;
#define PRINT_PUBLIC (&lsh_writekey_print_public.super.super)

extern struct command_simple lsh_writekey_print_private;
#define PRINT_PRIVATE (&lsh_writekey_print_private.super.super)

extern struct command_simple lsh_writekey_options2algorithms;
#define OPTIONS2ALGORITHMS (&lsh_writekey_options2algorithms.super.super)

extern struct command_simple lsh_writekey_options2transform;
#define TRANSFORM (&lsh_writekey_options2transform.super.super)

extern struct command_simple lsh_writekey_options2public_file;
#define OPTIONS2PUBLIC_FILE (&lsh_writekey_options2public_file.super.super)

extern struct command_simple lsh_writekey_options2private_file;
#define OPTIONS2PRIVATE_FILE (&lsh_writekey_options2private_file.super.super)

#include "lsh-writekey.c.x"


/* Option parsing */

/* NONO: already defined in lsh
const char *argp_program_version
= "lsh-writekey-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;
*/

/* GABA:
   (class
     (name lsh_writekey_options)
     (vars
       ; Base filename
       (file string)

       (tty object interact)
       
       (label string)
       (style . sexp_argp_state)
       (passphrase string)

       (crypto_algorithms object alist)
       (signature_algorithms object alist)
       (r object randomness)
       
       (crypto_name . int)
       (crypto object crypto_algorithm)
       (iterations . UINT32)))
*/

static struct lsh_writekey_options *
make_lsh_writekey_options(void)
{
  NEW(lsh_writekey_options, self);
  self->file = NULL;
  
  /* We don't need window change tracking. */
  self->tty = make_unix_interact(NULL);
    
  self->label = NULL;
  self->style = -1;

  self->passphrase = NULL;
  self->iterations = 1500;

  self->crypto_algorithms = all_symmetric_algorithms();

  /* NOTE: We don't need any randomness here, as we won't be signing
   * anything. */
  self->signature_algorithms = all_signature_algorithms(NULL);

  /* We use this only for salt and iv generation. */
  self->r = make_bad_random();

  /* A better default would be crypto_cbc(make_des3()) */
  self->crypto = NULL;
  
  return self;
}

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
#ifndef MACOS
  { "output-file", 'o', "Filename", 0, "Default is ~/.lsh/identity", 0 },
#else
  { "output-file", 'o', "Filename", 0, "Default is identity", 0 },
#endif
  { "iteration-count", 'i', "PKCS#5 iteration count", 0, "Default is 1500", 0 },
  { "crypto", 'c', "Algorithm", 0, "Encryption algorithm for the private key file.", 0 },
  { "label", 'l', "Text", 0, "Unencrypted label for the key.", 0 },
  { "passphrase", 'p', "Password", 0, NULL, 0 },
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
  CAST(lsh_writekey_options, self, state->input);

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;

    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->style;
      state->child_inputs[1] = NULL;
      break;

    case ARGP_KEY_END:
      if (!self->file)
	{
	  char *home = getenv("HOME");
	  struct lsh_string *s;
	  
	  if (!home)
	    {
	      argp_failure(state, EXIT_FAILURE, 0, "$HOME not set.");
	      return EINVAL;
	    }
	  else
	    {
#ifndef MACOS
	      s = ssh_cformat("%lz/.lsh", home);
	      if (mkdir(s->data, 0755) < 0)
		{
		  if (errno != EEXIST)
		    argp_failure(state, EXIT_FAILURE, errno, "Creating directory %s failed.", s->data);
		}
	      lsh_string_free(s);
	      self->file = ssh_cformat("%lz/.lsh/identity", home);
#else
	      self->file = ssh_cformat("%lzidentity", home);
#endif
	    }
	}
      if (self->crypto)
	{
	  if (!self->label)
	    {
	      const char *name = getenv("LOGNAME");
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 300
#endif
	      char host[MAXHOSTNAMELEN];
	  
	      if (!name)
		{
		  argp_failure(state, EXIT_FAILURE, 0,
			       "LOGNAME not set. Please use the -l option.");
		  return EINVAL;
		}

	      if ( (gethostname(host, sizeof(host)) < 0)
		   && (errno != ENAMETOOLONG) )
		argp_failure(state, EXIT_FAILURE, errno,
			     "Can't get the host name. Please use the -l option.");
	      
	      self->label = ssh_format("%lz@%lz", name, host);
	    }
	  while (!self->passphrase)
	    {
	      struct lsh_string *pw;
	      struct lsh_string *again;

	      pw = INTERACT_READ_PASSWORD(self->tty, 500,
					  ssh_format("Enter new passphrase: "), 1);
	      if (!pw)
		argp_failure(state, EXIT_FAILURE, 0, "Aborted.");

	      again = INTERACT_READ_PASSWORD(self->tty, 500,
					     ssh_format("Again: "), 1);
	      if (!again)
		argp_failure(state, EXIT_FAILURE, 0, "Aborted.");

	      if (lsh_string_eq(pw, again))
		self->passphrase = pw;
	      else
		lsh_string_free(pw);

	      lsh_string_free(again);
	    }
	}
      break;
      
    case 'o':
      self->file = format_cstring(arg);
      break;

    case 'i':
      {
	long i;
	char *end;
	i = strtol(arg, &end, 0);

	if ((end == arg) || *end || (i < 1))
	  {
	    argp_failure(state, EXIT_FAILURE, 0, "Invalid iteration count.");
	    return EINVAL;
	  }
	else if (i > PKCS5_MAX_ITERATIONS)
	  {
	    argp_error(state, "Iteration count ridiculously large (> %d).",
		       PKCS5_MAX_ITERATIONS);
	    return EINVAL;
	  }
	else
	  self->iterations = i;

	break;
      }

    case 'c':
      {
	int name = lookup_crypto(self->crypto_algorithms, arg, &self->crypto);

	if (name)
	  self->crypto_name = name;
	else
	  {
	    list_crypto_algorithms(state, self->crypto_algorithms);
	    argp_error(state, "Unknown crypto algorithm '%s'.", arg);
	  }
	break;
      }
      
    case 'l':
      self->label = ssh_format("%lz", arg);
      break;
      
    case 'p':
      self->passphrase = ssh_format("%lz", arg);
      break;
    }
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser, 
  NULL,
  ( "Splits a keypair in one private and one public file, "
    "optionally encrypting the private file using a passphrase.\v"
    "Common usage is to pipe the output from lsh-keygen into this program."
    ),
  main_argp_children,
  NULL, NULL
};

DEFINE_COMMAND_SIMPLE(lsh_writekey_print_public, a)
{
  CAST(lsh_writekey_options, options, a);

  return &make_sexp_print_command
    ((options->style > 0) ? options->style : SEXP_TRANSPORT)->super.super;
}

DEFINE_COMMAND_SIMPLE(lsh_writekey_print_private, a)
{
  CAST(lsh_writekey_options, options, a);

  return &make_sexp_print_command
    ((options->style > 0) ? options->style : SEXP_CANONICAL)->super.super;
}

DEFINE_COMMAND_SIMPLE(lsh_writekey_options2algorithms, a)
{
  CAST(lsh_writekey_options, options, a);
  return &options->signature_algorithms->super;
}

DEFINE_COMMAND_SIMPLE(lsh_writekey_options2transform, a)
{
  CAST(lsh_writekey_options, options, a);
  if (!options->crypto)
    return &command_I.super.super;
  else
    {
      CAST_SUBTYPE(mac_algorithm, hmac,
		   ALIST_GET(options->crypto_algorithms, ATOM_HMAC_SHA1));
      assert(hmac);
      
      return
	&make_pkcs5_encrypt(options->r,
			    lsh_string_dup(options->label),
			    ATOM_HMAC_SHA1,
			    hmac,
			    options->crypto_name,
			    options->crypto,
			    10, /* Salt length */
			    lsh_string_dup(options->passphrase),
			    options->iterations)->super;
    }
}

DEFINE_COMMAND_SIMPLE(lsh_writekey_options2public_file, a)
{
  CAST(lsh_writekey_options, options, a);
  struct lsh_string *public = ssh_cformat("%lS.pub", options->file);

  return
    &make_io_write_file_info(public->data,
			     O_CREAT | O_EXCL | O_WRONLY,
			     0644,
			     BLOCK_SIZE)->super;
}

DEFINE_COMMAND_SIMPLE(lsh_writekey_options2private_file, a)
{
  CAST(lsh_writekey_options, options, a);
  return
    &make_io_write_file_info(options->file->data,
			     O_CREAT | O_EXCL | O_WRONLY,
			     0600,
			     BLOCK_SIZE)->super;
}

/* GABA:
   (expr
     (name make_writekey)
     (globals
       (open IO_WRITE_FILE)
       (stdin IO_READ_STDIN))
     (params
       (options object lsh_writekey_options))
     (expr
       (lambda (backend)
         (let ((key (read_sexp (stdin backend))))
           (prog1 (print_public options (open backend (options2public_file options))
	                        (verifier2public
				  (signer2verifier
				    (sexp2signer (options2algorithms options)
				                 key))))
	          (print_private options (open backend (options2private_file options))
		                 (transform options key)))))))
*/

static void
do_lsh_writekey_handler(struct exception_handler *s UNUSED,
			const struct exception *e)
{
  /* NOTE: This is quite a complicated way to just write a friendlier
   * message if we fail because the output files already exists. This
   * way, it is easier to handle other kinds of errors later, as the
   * need arises. */
  
  switch (e->type)
    {
    case EXC_IO_OPEN_WRITE:
      {
	CAST_SUBTYPE(io_exception, x, e);
	switch(x->error)
	  {
	  case EEXIST:
	    /*werror("\nlsh-writekey doesn't overwrite existing key files.\n"
		   "If you *really* want to do that, you should delete\n"
		   "the existing files \"FOO\" and \"FOO.pub\" first\n"
		   "(where \"FOO\" usually is \"~/.lsh/identity\").\n");*/
	    werror("MacSSH doesn't overwrite existing key files.\n"
		   "If you *really* want to do that, you should delete\n"
		   "the existing files \"identity\" and \"identity.pub\" first.");
	    break;
	    
	  default:
	    goto outer_default;
	  }
	break;
      }
    outer_default:
    default:
      werror("lsh-writekey: %z\n", e->msg);
    }
  exit(EXIT_FAILURE);
}

static struct exception_handler exc_handler =
STATIC_EXCEPTION_HANDLER(do_lsh_writekey_handler, NULL);

#ifdef MACOS
//char *applname = "lsh-writekey";
//char *defargstr = "--debug --trace --verbose";
//char *defargstr = "";
int wkappl_main(int argc, char **argv);
#define main wkappl_main
#endif

int main(int argc, char **argv)
{
  struct io_backend *backend = make_io_backend();
  struct lsh_writekey_options *options = make_lsh_writekey_options();
  
  argp_parse(&main_argp, argc, argv, 0, NULL, options);

  {
    CAST_SUBTYPE
      (command, work,
       make_writekey(options));
    
    COMMAND_CALL(work, backend, &discard_continuation, &exc_handler);
  }
  io_run(backend);

  return EXIT_SUCCESS;
}

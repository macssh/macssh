/* srp-gen.c
 *
 * Create an SRP verifier
 *
 * $Id$ */

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

#include "srp.h"

#include "crypto.h"
#include "format.h"
#include "interact.h"
#include "io.h"
#include "randomness.h"
#include "sexp.h"
#include "srp.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#define BLOCK_SIZE 2000
#define SALT_SIZE 20

#include "srp-gen.c.x"

/* Option parsing */

const char *argp_program_version
= "srp-gen-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;

/* GABA:
   (class
     (name srp_gen_options)
     (vars
       (backend object io_backend)

       (tty object interact)
       
       (e object exception_handler)
       (G object abstract_group)
       (H object hash_algorithm)
       
       (file string)
       (dest object abstract_write)

       (style . sexp_argp_state)

       (name . "const char *")
       (passwd string)
       (r object randomness)))
*/

static struct srp_gen_options *
make_srp_gen_options(struct io_backend *backend,
		     struct exception_handler *e)
{
  NEW(srp_gen_options, self);

  self->backend = backend;

  /* We don't need window change tracking. */
  self->tty = make_unix_interact(NULL);
  
  self->e = e;

  self->G = make_ssh_ring_srp_1();
  self->H = &sha1_algorithm;
  self->file = NULL;
  self->dest = NULL;

  self->style = -1;

  self->name = getenv("LOGNAME");
  self->passwd = NULL;

  /* We use this only for generating the salt. */
  self->r = make_bad_random();

  return self;
}

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "user", 'l', "User name", 0, NULL, 0 },
  { "password", 'p', "Password", 0, NULL, 0 },
  { "output-file", 'o', "Filename", 0, "Default is to write to stdout.", 0 },
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
  CAST(srp_gen_options, self, state->input);

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;

    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->style;
      state->child_inputs[1] = NULL;
      break;

    case ARGP_KEY_END:
      if (!self->name)
	argp_error(state, "No user name given. Use the -l option, or set LOGNAME in the environment.");

      {
	struct lsh_fd *fd;
	
	if (self->file)
	  {
	    fd = io_write_file(self->backend, self->file->data,
			       O_CREAT | O_EXCL | O_WRONLY,
			       0600, BLOCK_SIZE,
			       NULL, self->e);
	    if (!fd)
	      argp_failure(state, EXIT_FAILURE, errno, "Could not open '%s'.", self->file->data);
	  }
	else
	  {
	    fd = io_write(make_lsh_fd(self->backend,
				      STDOUT_FILENO, "stdout",
				      self->e),
			  BLOCK_SIZE, NULL);
	  }
	self->dest = &fd->write_buffer->super;
      }
      
      while (!self->passwd)
	{
	  struct lsh_string *pw;
	  struct lsh_string *again;

	  pw = INTERACT_READ_PASSWORD(self->tty, 500,
				      ssh_format("Enter new SRP password: "), 1);
	  if (!pw)
	    argp_failure(state, EXIT_FAILURE, 0, "Aborted.");

	  again = INTERACT_READ_PASSWORD(self->tty, 500,
					 ssh_format("Again: "), 1);
	  if (!again)
	    argp_failure(state, EXIT_FAILURE, 0, "Aborted.");

	  if (lsh_string_eq(pw, again))
	    self->passwd = pw;
	  else
	    lsh_string_free(pw);

	  lsh_string_free(again);
	}
      if (self->style < 0)
	self->style = self->file ? SEXP_CANONICAL : SEXP_TRANSPORT;
      
      break;

    case 'o':
      self->file = make_string(arg);
      break;

    case 'p':
      self->passwd = ssh_format("%lz", arg);
      break;
    }
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser, 
  NULL,
  "Generates a password verifier for the Secure Remote Password protocol.",
  main_argp_children,
  NULL, NULL
};

static struct sexp *
srp_gen(struct srp_gen_options *options)
{
  struct lsh_string *salt;
  struct lsh_string *name;
  struct sexp *e;

  salt = lsh_string_alloc(SALT_SIZE);
  RANDOM(options->r, salt->length, salt->data);
  name = ssh_format("%lz", options->name);

  e = srp_make_verifier(options->G, options->H,
			salt, name, options->passwd);
  lsh_string_free(name);

  return e;
}

static void
do_srp_gen_handler(struct exception_handler *s UNUSED,
			const struct exception *e)
{
  werror("lsh_writekey: %z\n", e->msg);

  exit(EXIT_FAILURE);
}

static struct exception_handler exc_handler =
STATIC_EXCEPTION_HANDLER(do_srp_gen_handler, NULL);

int main(int argc, char **argv)
{
  struct io_backend *backend = make_io_backend();
  struct srp_gen_options *options
    = make_srp_gen_options(backend, &exc_handler);

  argp_parse(&main_argp, argc, argv, 0, NULL, options);

  A_WRITE(options->dest,
	  sexp_format(srp_gen(options), options->style, 0));

  io_run(backend);

  return EXIT_SUCCESS;
}

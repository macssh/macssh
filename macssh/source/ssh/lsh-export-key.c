/* lsh-export-key.c
 *
 * Reads an sexp in given form, and writes it in ssh2 form.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Jean-Pierre Stierlin, Niels Möller
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

/* Test output:
[nisse@cuckoo src]$ ./lsh-export-key < testkey.pub 
---- BEGIN SSH2 PUBLIC KEY ----
Comment: "768-bit dsa"
AAAAB3NzaC1kc3MAAABhAJw3J7CMyAKiX8F1Mz1dNguVQi7VZQQrLl8DeWNQaSkqmIPjsc
zSn4Cjv9BOt8FM46AZbw+aSou0jpiFPJJiQjpT5U1ArPLoMqRpopqcZqcVubRKALTzytgw
vvXyoHb84wAAABUAmm14nnnHQtwx5ZUgRrjv98iv4KcAAABgENZmq1qm4jdJJB7IAC5Ecr
vcjhlACNcPD4UQ0Bgk66/MJOxvrwf0V+ZtTfb8ZaQlKdu84vB2VxVcB8zo0ds01I6eLG2f
/nDENvwp0TkNKf1uyEWPjNQGI/ImAqukiSWjAAAAYDe6o/C8faYCpuduLPQrl8Co6z7HgC
yIaRCzBjD8bY6L5qZp4G//8PVJVhxXh3vAS6LbgDCFoa2HZ1/vxHpML+gl3FPjAOxZPs27
B2CTISEmV3KYx5NJpyKC3IBw/ckP6Q==
---- END SSH2 PUBLIC KEY ----
*/


#if macintosh
#include "lshprefix.h"
#include "lsh_context.h"
#endif


#include "algorithms.h"
#include "alist.h"
#include "atoms.h"
#include "crypto.h"
#include "format.h"
#include "io.h"
#include "lsh.h"
#include "lsh_argp.h"
#include "sexp_commands.h"
#include "spki.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "lsh-export-key.c.x"


/* Global, for simplicity */
int exit_code = EXIT_SUCCESS;

/* (write out sexp)
 *
 * Prints the sexp to the abstract_write OUT. Returns the sexp. */

/* GABA:
   (class
     (name ssh2_print_to)
     (super command)
     (vars
       (algorithms object alist)
       (subject . "const char *")
       (comment . "const char *")
       (dest object abstract_write)))
*/

/* GABA:
   (class
     (name ssh2_print_command)
     (super command_simple)
     (vars
       (algorithms object alist)
       (subject . "const char *")
       (comment . "const char *")))
*/

static struct lsh_string *
make_header(const char *name, const char *value)
{
  return value
    ? ssh_format("%lz: %lz\n", name, value)
    : ssh_format("");
}

static struct lsh_string *
insert_newlines(struct lsh_string *input, unsigned width, int free)
{
  unsigned out = input->length + ((input->length > width) ? input->length / width + 1 : 1);
  struct lsh_string *output = lsh_string_alloc(out);
  unsigned in = 0;
  out = 0;
  while ( in < input->length )
    {
      int len = (input->length - in < width) ? input->length - in : width;
      memcpy(output->data + out, input->data + in, len );
      in += len;
      out += len;
      output->data[out++] = '\n';
    }
  if (free)
    lsh_string_free(input);
  return output;
}

static void
do_ssh2_print(struct command *s,
	      struct lsh_object *a,
	      struct command_continuation *c,
	      struct exception_handler *e UNUSED)
{
  CAST(ssh2_print_to, self, s);
  CAST_SUBTYPE(sexp, expr, a);

  struct sexp_iterator *i;
  struct verifier *v;
  
  if (!sexp_check_type(expr, ATOM_PUBLIC_KEY, &i))
    {
      EXCEPTION_RAISE
	(e, make_simple_exception
	 (EXC_APP,
	  "Only conversion of public keys implemented.\n"));
      return;
    }

  expr = SEXP_GET(i);

  if (!expr)
    {
      EXCEPTION_RAISE
	(e, make_simple_exception
	 (EXC_APP,
	  "Invalid (empty) public key expression.\n"));
      return;
    }
      
  v = spki_make_verifier(self->algorithms, expr);
  if (!v)
    {
      EXCEPTION_RAISE
	(e, make_simple_exception
	 (EXC_APP,
	  "Unsupported algorithm."));
    }

  A_WRITE(self->dest, ssh_format("---- BEGIN SSH2 PUBLIC KEY ----\n"
				 "%lfS"
				 "%lfS"
				 "%lfS"
				 "---- END SSH2 PUBLIC KEY ----\n",
				 make_header("Subject", self->subject),
				 make_header("Comment", self->comment),
				 insert_newlines(encode_base64(PUBLIC_KEY(v), NULL, 0, 0, 1), 70, 1)
				 ));
  COMMAND_RETURN(c, a);
}

static struct command *
make_ssh2_print_to(struct alist *algorithms,
		   const char *s, const char *c,
		   struct abstract_write *dest)
{
  NEW(ssh2_print_to, self);
  self->super.call = do_ssh2_print;
  self->algorithms = algorithms;
  self->subject = s;
  self->comment = c;
  self->dest = dest;

  return &self->super;
}

static struct lsh_object *
do_ssh2_print_simple(struct command_simple *s,
		     struct lsh_object *a)
{
  CAST(ssh2_print_command, self, s);
  CAST_SUBTYPE(abstract_write, dest, a);

  return &make_ssh2_print_to(self->algorithms,
			     self->subject, self->comment, dest)->super;
}

static struct command_simple *
make_ssh2_print_command(struct alist *algorithms,
			const char *s, const char *c)
{
  NEW(ssh2_print_command, self);
  self->super.super.call = do_call_simple_command;
  self->super.call_simple = do_ssh2_print_simple;
  self->algorithms = algorithms;
  self->subject = s;
  self->comment = c;
  return &self->super;
}


/* GABA:
   (expr
     (name make_export_key)
     (params
       (read object command)
       (print object command)
       (dest object abstract_write))
     (expr
       (lambda (in)
         (print dest (read in)))))
*/


static void
do_exc_export_key_handler(struct exception_handler *self,
			 const struct exception *x)
{
  /* CAST(exc_export_key_handler, self, s); */
  
  switch (x->type)
    {
    case EXC_APP:
      werror("%z", x->msg);
      break;
    case EXC_SEXP_SYNTAX:
      werror("Invalid SEXP input.\n");
      exit_code = EXIT_FAILURE;
      /* Fall through */
    case EXC_SEXP_EOF:
      /* Normal termination */
      EXCEPTION_RAISE(self->parent, &finish_read_exception);
      break;
    case EXC_IO_WRITE:
    case EXC_IO_READ:
      {
	CAST(io_exception, e, x);
	exit_code = EXIT_FAILURE;
	werror("lsh-export-key: %z, (errno = %i)\n", x->msg, e->error);
	break;
      }
    default:
      exit_code = EXIT_FAILURE;
      EXCEPTION_RAISE(self->parent, x);
      return;
    }
}

/* Option parsing */

/*
const char *argp_program_version
= "lsh-export-key-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;
*/

#define OPT_INFILE 'r'
#define OPT_OUTFILE 'o'
#define OPT_SUBJECT 's'
#define OPT_COMMENT 'c'

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "input-file", OPT_INFILE, "Filename", 0, "Default is stdin", 0 },
  { "output-file", OPT_OUTFILE, "Filename", 0, "Default is stdout", 0 },
  { "subject", OPT_SUBJECT, "subject string", 0, "Add subject to output key.", 0 },
  { "comment", OPT_COMMENT, "comment string", 0, "Add comment to output key.", 0 },
  { NULL, 0, NULL, 0, NULL, 0 }
};

/* GABA:
(class
  (name export_key_options)
  (vars
    (input . sexp_argp_state)
    (algorithms object alist)
    (infile . "const char *")
    (outfile . "const char *")
    (subject . "const char *")
    (comment . "const char *")
    (print object command)
))
*/

static struct export_key_options *make_options(void)
{
  NEW(export_key_options, self);
  self->input = SEXP_TRANSPORT;
  self->infile = NULL;
  self->subject = NULL;
  self->comment = NULL;
  self->algorithms = all_signature_algorithms(NULL);

  return self;
}

static const struct argp_child
main_argp_children[] =
{
  { &sexp_input_argp, 0, NULL, 0 },
  { &werror_argp, 0, "", 0 },
  { NULL, 0, NULL, 0}
};

static error_t
main_argp_parser(int key, char *arg, struct argp_state *state)
{
  CAST(export_key_options, self, state->input);
#if MACOS
  struct lsh_string *tmp = NULL;
#endif

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->input;
      // state->child_inputs[1] = &self->output;
      state->child_inputs[1] = NULL;
      break;
    case ARGP_KEY_END:
      self->print = &(make_ssh2_print_command(self->algorithms,
                                              self->subject,
                                              self->comment)->super);
      break;
    case OPT_INFILE:
      self->infile = arg;
#if MACOS
      if ( access( arg, R_OK ) != 0 ) {
        tmp = ssh_cformat("%lz%lz", getenv("HOME"), arg);
        self->infile = (const char *)tmp->data;
      }
#endif
      break;
    case OPT_OUTFILE:
      self->outfile = arg;
      break;
    case OPT_SUBJECT:
      self->subject = arg;
      break;
    case OPT_COMMENT:
      self->comment = arg;
      break;
    }
  
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser, NULL,
  "Reads an s-expression on stdin, and outputs the same "
  "s-expression on stdout, using OpenSSH/SSH2 encoding format.",
  main_argp_children,
  NULL, NULL
};
  

#define SEXP_BUFFER_SIZE 1024

#ifdef MACOS
//char *applname = "lsh-export-key";
//char *defargstr = "-r identity.pub";
//char *defargstr = "";
int ekappl_main(int argc, char **argv);
#define main ekappl_main
#endif

int main(int argc, char **argv)
{
  struct export_key_options *options = make_options();
  struct exception_handler *e;
  struct lsh_fd *in;
  struct lsh_fd *out;

  struct io_backend *backend = make_io_backend();

  argp_parse(&main_argp, argc, argv, 0, NULL, options);

  /* Patch the parent pointer later */
  e = make_exception_handler(do_exc_export_key_handler,
			     NULL, HANDLER_CONTEXT);

  if (options->infile)
    {
      in = io_read_file(backend, options->infile, e);
      if (!in)
	{
	  werror("Failed to open '%z': %z\n",
		 options->infile, STRERROR(errno));
	  return EXIT_FAILURE;
	}
    }
  else
    in = make_lsh_fd(backend, STDIN_FILENO, e);
      
  if (options->outfile)
    {
      out = io_write_file(backend, options->outfile,
			  O_WRONLY | O_CREAT, 0666,
			  SEXP_BUFFER_SIZE, NULL, e);
    }
  else
    out = io_write(make_lsh_fd(backend, STDOUT_FILENO, e),
		   SEXP_BUFFER_SIZE, NULL);

  {
    CAST_SUBTYPE(command, work,
		 make_export_key(
		   make_read_sexp_command(options->input, 0),
		   options->print,
		   &(out->write_buffer->super)));

    /* Fixing the exception handler creates a circularity */
    e->parent = make_exc_finish_read_handler(in,
					     &default_exception_handler,
					     HANDLER_CONTEXT);
    
    COMMAND_CALL(work, in,
		 &discard_continuation, e);
  }
  io_run(backend);
  
  return exit_code;
    }
  

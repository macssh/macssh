/* sexp-conv.c
 *
 * Reads a sexp in given form from, and writes it in given form.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Balázs Scheidler, Niels Möller
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

#if macintosh
#include "lshprefix.h"
#include "lsh_context.h"
#endif

#include "algorithms.h"
#include "alist.h"
#include "atoms.h"
#include "crypto.h"
#include "io.h"
#include "lsh.h"
#include "lsh_argp.h"
#include "sexp_commands.h"
#include "spki_commands.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "sexp-conv.c.x"

/* Global, for simplicity */
int exit_code = EXIT_SUCCESS;

/* GABA:
   (expr
     (name make_sexp_conv)
     (params
       (read object command)
       (select object command)
       (transform object command)
       (print object command)
       (dest object abstract_write))
     (expr
       (lambda (in)
         (print dest (transform (select (read in)))))))
*/


static void
do_exc_sexp_conv_handler(struct exception_handler *self,
			 const struct exception *x)
{
  /* CAST(exc_sexp_conv_handler, self, s); */
  
  switch (x->type)
    {
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
	werror("sexp-conv: %z, (errno = %i)\n", x->msg, e->error);
	break;
      }
    default:
      exit_code = EXIT_FAILURE;
      EXCEPTION_RAISE(self->parent, x);
      return;
    }
}

enum sexp_select_ops { OP_CAR, OP_CDR, OP_RETURN_CAR };

/* GABA:
   (class
     (name sexp_select_command)
     (super command)
     (vars
       (ops object int_list)))
*/

static void
do_sexp_select(struct command *s,
	       struct lsh_object *x,
	       struct command_continuation *c,
	       struct exception_handler *e)
{
  CAST(sexp_select_command, self, s);
  CAST_SUBTYPE(sexp, expr, x);
  struct sexp_iterator *i;
  unsigned j;

  static const struct exception non_list =
    STATIC_EXCEPTION(EXC_SEXP_SYNTAX,
		     "Attemt to select in a non-list expression.");

  static const struct exception list_exhausted =
    STATIC_EXCEPTION(EXC_SEXP_SYNTAX,
		     "List exhausted while selecting.");

  if (sexp_atomp(expr))
    {
      EXCEPTION_RAISE(e, &non_list);
      return;
    }

  i = SEXP_ITER(expr);

  for (j = 0; j < LIST_LENGTH(self->ops); j++)
    switch (LIST(self->ops)[j])
      {
      case OP_RETURN_CAR:
	{
	  struct sexp *expr = SEXP_GET(i);
	  if (!expr)
	    EXCEPTION_RAISE(e, &list_exhausted);
	  else
	    COMMAND_RETURN(c, expr);

	  return;
	}
      case OP_CAR:
	{
	  struct sexp *expr = SEXP_GET(i);
	  if (!expr)
	    {
	      EXCEPTION_RAISE(e, &list_exhausted);
	      return;
	    }
	    
	  if (sexp_atomp(expr))
	    {
	      EXCEPTION_RAISE(e, &non_list);
	      return;
	    }
	  i = SEXP_ITER(expr);

	  break;
	}
      case OP_CDR:
	SEXP_NEXT(i);
	if (!SEXP_LEFT(i))
	  {
	    EXCEPTION_RAISE(e, &list_exhausted);
	    return;
	  }
	break;
      }
  
  fatal("do_sexp_select: Internal error!\n");
}
     
static struct command *
make_sexp_select_command(const char *arg)
{
  unsigned len = strlen(arg);

  /* Check syntax */
  if ( (len >= 3)
       && (arg[0] == 'c')
       && (arg[1] == 'a')
       && (arg[len-1] == 'r'))
    {
      struct int_list *ops = alloc_int_list(len - 2);
      unsigned i;
      
      for (i = 0; i < len - 3; i++)
	switch(arg[len - i - 2])
	  {
	  case 'a':
	    LIST(ops)[i] = OP_CAR;
	    break;
	  case 'd':
	    LIST(ops)[i] = OP_CDR;
	    break;
	  default:
	    KILL(ops);
	    return NULL;
	  }

      LIST(ops)[i++] = OP_RETURN_CAR;
      assert (i == LIST_LENGTH(ops));
      
      {
	NEW(sexp_select_command, self);
	self->super.call = do_sexp_select;
	self->ops = ops;

	return &self->super;
      }
    }
  else
    return NULL;
}

/* Option parsing */

const char *argp_program_version
= "sexp-conv-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;

#define OPT_HASH 0x200
#define OPT_SPKI_HASH 0x201
#define OPT_RAW_HASH 0x202
#define OPT_ONCE 0x203
#define OPT_SELECT 0x204

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "spki-hash", OPT_SPKI_HASH, NULL, 0, "Output an SPKI hash for the object.", 0 },
  { "raw-hash", OPT_RAW_HASH, NULL, 0, "Output the hash for the canonical "
    "representation of the object, in hexadecimal.", 0 },
  { "hash", OPT_HASH, "Algorithm", 0, "Hash algorithm (default sha1).", 0 },
  { "select", OPT_SELECT, "Operator", 0, "Select a subexpression "
    "(e.g \"caddr\") for processing.", 0 },
  { "once", OPT_ONCE, NULL, 0, "Process at most one s-expression.", 0 },
  { NULL, 0, NULL, 0, NULL, 0 }
};

#define MODE_VANILLA 0
#define MODE_RAW_HASH 1
#define MODE_SPKI_HASH 2

/* GABA:
(class
  (name sexp_conv_options)
  (vars
    (input . sexp_argp_state)
    (output . sexp_argp_state)
    (once . int)
    (mode . int)
    (algorithms object alist)
    (hash . int)
    (select object command)
    (transform object command)
    (print object command)
))
*/

static struct sexp_conv_options *make_options(void)
{
  NEW(sexp_conv_options, self);
  self->input = SEXP_TRANSPORT;
  self->output = SEXP_ADVANCED;
  self->once = 0;
  self->mode = MODE_VANILLA;
  self->select = &command_I.super;
  self->transform = &command_I.super;
  self->algorithms = make_alist(2,
				ATOM_MD5, &md5_algorithm,
				ATOM_SHA1, &sha1_algorithm,
				-1);
  self->hash = ATOM_SHA1;

  return self;
}

static const struct argp_child
main_argp_children[] =
{
  { &sexp_input_argp, 0, NULL, 0 },
  { &sexp_output_argp, 0, NULL, 0 },
  { &werror_argp, 0, "", 0 },
  { NULL, 0, NULL, 0}
};

static error_t
main_argp_parser(int key, char *arg, struct argp_state *state)
{
  CAST(sexp_conv_options, self, state->input);
  
  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->input;
      state->child_inputs[1] = &self->output;
      state->child_inputs[2] = NULL;
      break;
    case ARGP_KEY_END:
      {
	switch(self->mode)
	  {
	  default:
	    fatal("Internal error!");
	  case MODE_VANILLA:
	    self->transform = &command_I.super;
	    self->print = &make_sexp_print_command(self->output)->super;
	    break;
	  case MODE_SPKI_HASH:
	    {
	      CAST_SUBTYPE(hash_algorithm, a,
			   ALIST_GET(self->algorithms, self->hash));
	      self->transform = make_spki_hash(self->hash, a);
	      self->print = &make_sexp_print_command(self->output)->super;
	      break;
	    }
	  case MODE_RAW_HASH:
	    {
	      CAST_SUBTYPE(hash_algorithm, a,
			   ALIST_GET(self->algorithms, self->hash));
	      self->transform = &command_I.super;
	      self->print = make_sexp_print_raw_hash(a);
	      break;
	    }
	  }
	break;
      }
    case OPT_HASH:
      {
	int hash = lookup_hash(self->algorithms, arg, NULL, 0);
	if (hash)
	  self->hash = hash;
	else
	  argp_error(state, "Unknown hash algorithm '%s'.", arg);
	break;
      }
    case OPT_SPKI_HASH:
      self->mode = MODE_SPKI_HASH;
      break;
    case OPT_RAW_HASH:
      self->mode = MODE_RAW_HASH;
      break;
    case OPT_SELECT:
      {
	struct command *sel = make_sexp_select_command(arg);
	if (sel)
	  self->select = sel;
	else
	  argp_error(state, "Unsupported select operator '%s' (the supported "
		     "ones are ca[ad]*r).", arg);
	break;
      }
    case OPT_ONCE:
      self->once = 1;
      break;
    }
  
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser,
  "Conversion: sexp-conv [options] <INPUT-SEXP >OUTPUT\n"
  "Fingerprinting: sexp-conv --raw-hash [ --hash=ALGORITHM ] <PUBLIC-KEY",
  "Reads an s-expression on stdin, and outputs the same "
  "s-expression on stdout, possibly using a different "
  "encoding. By default, output uses the advanced encoding. ",
  main_argp_children,
  NULL, NULL
};
  

#define SEXP_BUFFER_SIZE 1024

#ifdef MACOS
char *applname = "sexp-conv";
char *defargstr = "-i advanced --once";
int appl_main(int argc, char **argv);
#define main appl_main
#endif

int main(int argc, char **argv)
{
  struct sexp_conv_options *options = make_options();
  struct exception_handler *e;
  struct io_backend *backend = make_io_backend();
  
  argp_parse(&main_argp, argc, argv, 0, NULL, options);

  /* Patch the parent pointer later */
  e = make_exception_handler(do_exc_sexp_conv_handler,
			     NULL, HANDLER_CONTEXT);
  
  {
    CAST_SUBTYPE(command, work,
		 make_sexp_conv(
		   make_read_sexp_command(options->input, !options->once),
		   options->select,
		   options->transform,
		   options->print,
		   &(io_write(make_lsh_fd(backend,
					 STDOUT_FILENO,
					 e),
			      SEXP_BUFFER_SIZE,
			      NULL)
		     ->write_buffer->super)));

    struct lsh_fd *in = make_lsh_fd(backend, STDIN_FILENO, e);

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

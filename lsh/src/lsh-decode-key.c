/* lsh-decode-key.c
 *
 * Decode ssh2 keys.
 *
 * $Id$
 */

#if macintosh
#include "lshprefix.h"
#include "lsh_context.h"
#endif

#include "dsa.h"
#include "io.h"
#include "lsh_argp.h"
#include "publickey_crypto.h"
#include "read_file.h"
#include "rsa.h"
#include "sexp_commands.h"
#include "spki.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include <unistd.h>
#include <fcntl.h>

#include "lsh-decode-key.c.x"

/* Option parsing */

const char *argp_program_version
= "lsh-decode-key-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;

/* GABA:
   (class
     (name lsh_decode_key_options)
     (vars
       ; Output filename
       (file . "char *")

       ; Assume input is base64
       (base64 . int)
       (style . sexp_argp_state)))
*/

static struct lsh_decode_key_options *
make_lsh_decode_key_options(void)
{
  NEW(lsh_decode_key_options, self);
  self->file = NULL;
  self->base64 = 0;
  self->style = -1;

  return self;
}

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "output-file", 'o', "Filename", 0, "Default is stdout", 0 },
  { "base64", 'b', NULL, 0, "Input is base64 encoded", 0 },
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
  CAST(lsh_decode_key_options, self, state->input);

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;

    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->style;
      break;

    case ARGP_KEY_END:
      if (self->style < 0)
	self->style = self->file ? SEXP_CANONICAL : SEXP_TRANSPORT;
      break;
      
    case 'b':
      self->base64 = 1;
      break;
      
    case 'o':
      self->file = arg;
      break;
    }
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser, 
  NULL,
  ( "Converts a raw OpenSSH/ssh2 public key to sexp-format.\v"
    "Usually invoked by the ssh-conv script."),
  main_argp_children,
  NULL, NULL
};


#define EXC_APP_UNKNOWN_KEY (EXC_APP + 1)
#define EXC_APP_BAD_KEY (EXC_APP + 2)
#define EXC_APP_BAD_ARMOUR (EXC_APP + 3)
#define EXC_APP_BAD_FORMAT (EXC_APP + 4)

/* GABA:
   (class
     (name decode_key)
     (super abstract_write)
     (vars
       (c object command_continuation)
       (e object exception_handler)))
*/

static void
do_decode_key(struct abstract_write *s,
	      struct lsh_string *contents)
{
  CAST(decode_key, self, s);

  /* Stop reading. */
  EXCEPTION_RAISE(self->e, &finish_io_exception);
  
  if (!contents)
    {
      EXCEPTION_RAISE(self->e,
		      make_simple_exception(EXC_APP_BAD_ARMOUR,
					    "Invalid base64 data."));
    }
  else
    {
      struct simple_buffer buffer;
      int type;

      simple_buffer_init(&buffer, contents->length, contents->data);

      if (!parse_atom(&buffer, &type))
	EXCEPTION_RAISE(self->e,
			make_simple_exception(EXC_APP_BAD_FORMAT,
					      "Invalid (binary) input data.\n"));
      else
	switch (type)
	  {
	  case ATOM_SSH_DSS:
	    {
	      struct verifier *v;
	    
	      werror("lsh-decode-key: Reading key of type ssh-dss...\n");

	      if (! ( (v = parse_ssh_dss_public(&buffer)) ))
		{
		  EXCEPTION_RAISE(self->e,
				  make_simple_exception(EXC_APP_BAD_KEY,
							"Invalid dsa key.\n"));
		  return;
		}

	      COMMAND_RETURN(self->c, spki_make_public_key(v));
	      break;
	    }
	  case ATOM_SSH_RSA:
	    {
	      struct verifier *v;
	    
	      werror("lsh-decode-key: Reading key of type ssh-rsa...\n");

	      if (! ( (v = parse_ssh_rsa_public(&buffer)) ))
		{
		  EXCEPTION_RAISE(self->e,
				  make_simple_exception(EXC_APP_BAD_KEY,
							"Invalid rsa key.\n"));
		  return;
		}

	      COMMAND_RETURN(self->c, spki_make_public_key(v));
	      break;
	    }	    
	  default:
	    EXCEPTION_RAISE(self->e,
			    make_simple_exception(EXC_APP_BAD_KEY,
						  "Unknown key type."));
	  }

    }
}

static struct abstract_write *
make_decode_key(struct command_continuation *c,
		struct exception_handler *e)
{
  NEW(decode_key, self);
  self->super.write = do_decode_key;
  self->c = c;
  self->e = e;

  return &self->super;
}

static void
do_exc_lsh_decode_key(struct exception_handler *s UNUSED,
		      const struct exception *e)
{
  werror("lsh-decode-key: %z\n", e->msg);

  exit(EXIT_FAILURE);
}

static struct exception_handler exc_handler =
STATIC_EXCEPTION_HANDLER(do_exc_lsh_decode_key, NULL);

#define MAX_FILE 30000
#define BLOCKSIZE 1024

#ifdef MACOS
char *applname = "lsh-decode-key";
char *defargstr = "";
int appl_main(int argc, char **argv);
#define main appl_main
#endif

int main(int argc, char **argv)
{
  struct io_backend *backend = make_io_backend();

  struct lsh_decode_key_options *options = make_lsh_decode_key_options();
  struct exception_handler *e;
  struct lsh_fd *in;
  struct lsh_fd *out;

  argp_parse(&main_argp, argc, argv, 0, NULL, options);

  in = make_lsh_fd(backend, STDIN_FILENO, "stdin",
		   &exc_handler);

  /* We want an exception handler that deals with EXC_FINISH_IO. */
  e = make_exc_finish_read_handler(in, &exc_handler, HANDLER_CONTEXT);
  
  if (options->file)
    out = io_write_file(backend, options->file,
			O_WRONLY | O_CREAT, 0666,
			BLOCKSIZE, NULL, e);
  else
    out = io_write(make_lsh_fd(backend, STDOUT_FILENO,
			       "stdout", e),
		   BLOCKSIZE, NULL);
  io_read
    (in,
     make_buffered_read(BLOCKSIZE,
			(options->base64 ? make_read_base64 : make_read_file)
			(make_decode_key
			 (make_apply
			  (make_sexp_print_to(options->style,
					      &out->write_buffer->super),
			   &discard_continuation, e), e),
			 MAX_FILE)),
     NULL);

  io_run(backend);

  return EXIT_SUCCESS;
}

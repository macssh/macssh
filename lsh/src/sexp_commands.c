/* sexp_commands.c
 *
 * Reading and writing of s-expressions.
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

#include "sexp_commands.h"

#include "format.h"
#include "io.h"
#include "read_file.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

/* Forward declarations */
static struct catch_command catch_sexp_exceptions;
#define CATCH_SEXP (&catch_sexp_exceptions.super.super)

static struct read_sexp_command read_sexp;
#define READ_SEXP (&read_sexp.super.super)

#define GABA_DEFINE
#include "sexp_commands.h.x"
#undef GABA_DEFINE

#include "sexp_commands.c.x"

/* (write out sexp)
 *
 * Prints the sexp to tha abstract_write OUT. Returns the sexp. */

static void
do_sexp_print(struct command_2 *s,
	      struct lsh_object *a1,
	      struct lsh_object *a2,
	      struct command_continuation *c,
	      struct exception_handler *e UNUSED)
{
  CAST(sexp_print_command, self, s);
  CAST_SUBTYPE(abstract_write, dest, a1);
  CAST_SUBTYPE(sexp, o, a2);

  A_WRITE(dest, sexp_format(o, self->format, 0));
  if (self->format != SEXP_CANONICAL)
    A_WRITE(dest, ssh_format("\n"));

  COMMAND_RETURN(c, o);
}

struct command_2 *
make_sexp_print_command(int format)
{
  NEW(sexp_print_command, self);
  self->super.super.call = do_command_2;
  self->super.invoke = do_sexp_print;
  self->format = format;

  return &self->super;
}

struct command *
make_sexp_print_to(int format, struct abstract_write *dest)
{
  return
    make_command_2_invoke(make_sexp_print_command(format),
			  &dest->super);
}

DEFINE_COMMAND3(sexp_print_raw_hash)
     (struct lsh_object *a1,
      struct lsh_object *a2,
      struct lsh_object *a3,
      struct command_continuation *c,
      struct exception_handler *e UNUSED)
{
  CAST_SUBTYPE(hash_algorithm, algorithm, a1);
  CAST_SUBTYPE(abstract_write, dest, a2);
  CAST_SUBTYPE(sexp, o, a3);

  struct lsh_string *canonical = sexp_format(o, SEXP_CANONICAL, 0);
  struct hash_instance *hash = MAKE_HASH(algorithm);
  struct lsh_string *digest = lsh_string_alloc(hash->hash_size);

  HASH_UPDATE(hash, canonical->length, canonical->data);
  HASH_DIGEST(hash, digest->data);
  
  lsh_string_free(canonical);
  KILL(hash);

  A_WRITE(dest, ssh_format("%lxfS\n", digest));

  COMMAND_RETURN(c, o);
}

struct command *
make_sexp_print_raw_hash(struct hash_algorithm *algorithm)
{
  return make_command_3_invoke(&sexp_print_raw_hash,
			       &algorithm->super);
}

/* GABA:
   (class
     (name sexp_parser)
     (super abstract_write)
     (vars
       (style . int)
       (c object command_continuation)
       (e object exception_handler)))
*/

static const struct exception
sexp_syntax_exception = STATIC_EXCEPTION(EXC_SEXP_SYNTAX,
					 "Sexp syntax error");

static const struct exception
sexp_eof_exception = STATIC_EXCEPTION(EXC_SEXP_EOF, "All sexps read");

static void
do_sexp_parse_once(struct abstract_write *s, struct lsh_string *input)
{
  CAST(sexp_parser, self, s);
  struct simple_buffer buffer;
  struct sexp *expr;
  
  assert(input);

  simple_buffer_init(&buffer, input->length, input->data);

  expr = sexp_parse(self->style, &buffer);
  if (!expr)
    EXCEPTION_RAISE(self->e, &sexp_syntax_exception);
  else
    COMMAND_RETURN(self->c, expr);

  lsh_string_free(input);
}

static void
do_sexp_parse_many(struct abstract_write *s, struct lsh_string *input)
{
  CAST(sexp_parser, self, s);
  struct simple_buffer buffer;
  const struct exception *e = &sexp_eof_exception;
    
  assert(input);

  simple_buffer_init(&buffer, input->length, input->data);

  while (!parse_eod(&buffer))
    {
      struct sexp *expr = sexp_parse(self->style, &buffer);
      if (!expr)
	{
	  e = &sexp_syntax_exception;
	  break;
	}
      else
	COMMAND_RETURN(self->c, expr);
    }

  EXCEPTION_RAISE(self->e, e);
  lsh_string_free(input);
}

static struct abstract_write *
make_sexp_parser(int style, int goon,
		 struct command_continuation *c,
		 struct exception_handler *e)
{
  NEW(sexp_parser, self);
  self->super.write = goon ? do_sexp_parse_many : do_sexp_parse_once;
  self->style = style;
  self->c = c;
  self->e = e;

  return &self->super;
}

#define SEXP_BUFFER_SIZE 1024

void
do_read_sexp(struct command *s,
	     struct lsh_object *a,
	     struct command_continuation *c,
	     struct exception_handler *e)
{
  CAST(read_sexp_command, self, s);
  CAST(lsh_fd, fd, a);

  trace("do_read_sexp\n");
    
  assert(fd);
  
  io_read(fd,
	  make_buffered_read
	  (SEXP_BUFFER_SIZE,
	   make_read_file
	   (make_sexp_parser(self->format,
			     self->goon,
			     c, e),
	    self->max_size)),
	  NULL);
}

struct command *
make_read_sexp_command(int format, int goon, UINT32 max_size)
{
  NEW(read_sexp_command, self);

  trace("make_read_sexp_command\n");
  
  self->super.call = do_read_sexp;
  self->format = format;
  self->goon = goon;
  self->max_size = max_size;

  return &self->super;
}

static struct catch_command catch_sexp_exceptions
= STATIC_CATCH_COMMAND(EXC_ALL, EXC_SEXP_EOF, 1);

/* Arbitrary limit on file size. */
/*#define MAX_SEXP_SIZE 10000*/
#define MAX_SEXP_SIZE 65536

static struct read_sexp_command read_sexp
= STATIC_READ_SEXP(SEXP_TRANSPORT, 1, MAX_SEXP_SIZE);

/* GABA:
   (expr
     (name for_sexp)
     (params
       (handler object command))
     (expr
       (lambda (proc)
         (catch_sexp handler
	             (lambda (file)
		       (proc (read_sexp file)))))))
*/

DEFINE_COMMAND(for_sexp_command)
     (struct command *s UNUSED,
      struct lsh_object *a,
      struct command_continuation *c,
      struct exception_handler *e UNUSED)
{
  CAST_SUBTYPE(command, handler, a);
  COMMAND_RETURN(c, for_sexp(handler));
}

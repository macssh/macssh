/* sexp_commands.h
 *
 * Reading and writing of s-expressions.
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

#ifndef SEXP_COMMANDS_H_INCLUDED
#define SEXP_COMMANDS_H_INCLUDED

#include "abstract_crypto.h"
#include "abstract_io.h"
#include "command.h"
#include "sexp.h"

#define GABA_DECLARE
#include "sexp_commands.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name sexp_print_command)
     (super command_simple)
     (vars
       (format . int)))
*/

struct lsh_object *
do_sexp_print_simple(struct command_simple *s,
		     struct lsh_object *a);

#define STATIC_PRINT_SEXP(format) \
{ STATIC_COMMAND_SIMPLE(do_sexp_print_simple), format }

struct command_simple *
make_sexp_print_command(int format);

struct command *
make_sexp_print_to(int format, struct abstract_write *dest);

extern struct collect_info_1 sexp_print_raw_hash;
#define SEXP_PRINT_RAW_HASH (&sexp_print_raw_hash.super.super)

struct command *
make_sexp_print_raw_hash(struct hash_algorithm *algorithm);

struct command *
make_sexp_print_raw_hash_to(struct hash_algorithm *algorithm,
			    struct abstract_write *dest);

/* GABA:
   (class
     (name read_sexp_command)
     (super command)
     (vars
       (format . int)
       (goon . int)))
*/

void
do_read_sexp(struct command *s,
	     struct lsh_object *a,
	     struct command_continuation *c,
	     struct exception_handler *e);

#define STATIC_READ_SEXP(format, goon) \
{ STATIC_COMMAND(do_read_sexp), format, goon }

struct command *
make_read_sexp_command(int format, int goon);

extern struct command_simple for_sexp_command;
#define FOR_SEXP (&for_sexp_command.super.super)

#endif /* SEXP_COMMANDS_H_INCLUDED */

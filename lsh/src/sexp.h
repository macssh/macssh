/* sexp.h
 *
 * An implementation of Ron Rivest's S-expressions, used in spki.
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

#ifndef LSH_SEXP_H_INCLUDED
#define LSH_SEXP_H_INCLUDED

#include "bignum.h"
#include "command.h"
#include "list.h"
#include "lsh_argp.h"
#include "parse.h"

#include "sexp_table.h"


#define GABA_DECLARE
#include "sexp.h.x"
#undef GABA_DECLARE

/* FIXME: Do some constification. Strings inside sexpressions should
 * always be const. */

/* GABA:
   (class
     (name sexp)
     (vars
       ; NULL for non-lists
       (iter method "struct sexp_iterator *")
       (format method "struct lsh_string *" "int style" "unsigned indent")))
*/

#define SEXP_FORMAT(e, s, i) ((e)->format((e), (s), (i)))
#define SEXP_ITER(e) ((e)->iter((e)))

/* GABA:
   (class
     (name sexp_cons)
     (super sexp)
     (vars
       (car object sexp)
       (cdr object sexp_cons)))
*/

/* ;; GABA:
   (class
     (name sexp_atom)
     (super sexp)
     (vars
       (atom . int)))
*/


/* Iterator abstraction idea taken from Ron's code */
/* FIXME: It would make a lot of sense to use (meta)class methods for
 * iterators. */
/* GABA:
   (class
     (name sexp_iterator)
     (vars
       (get method "struct sexp *")
       (set method void "struct sexp *")
       (assoc method "struct sexp *"
              "UINT32 length" "const UINT8 *name"
	      "struct sexp_iterator **i")
       (left method unsigned)
       (next method void)))
*/

#define SEXP_GET(i) ((i)->get((i)))
#define SEXP_SET(i, v) ((i)->set((i), (v)))
#define SEXP_ASSOC(s, l, n, i) ((s)->assoc((s), (l), (n), (i)))
#define SEXP_LEFT(i) ((i)->left((i)))
#define SEXP_NEXT(i) ((i)->next((i)))

/* Syntax styles */

#define SEXP_CANONICAL 0
#define SEXP_TRANSPORT 1
#define SEXP_ADVANCED 2

/* Like advanced, but allow international characters in quoted strings. */
#define SEXP_INTERNATIONAL 3

struct lsh_string *sexp_format(struct sexp *e, int style, unsigned indent);

struct lsh_string *encode_base64(struct lsh_string *s,
				 const char *delimiters,
				 int break_lines,
				 unsigned indent, int free);

/* Creating sexps */

/* Consumes its args (display may be NULL) */
struct sexp *sexp_s(struct lsh_string *d, struct lsh_string *c);

/* atom->sexp */
struct sexp *sexp_a(const int a);

/* cstring->sexp */
struct sexp *sexp_z(const char *s);

/* mpz->sexp */
struct sexp *sexp_un(const mpz_t n);
struct sexp *sexp_sn(const mpz_t n);

/* Small unsigned int -> sexp */
struct sexp *sexp_uint32(UINT32 n);

/* cons */
struct sexp *sexp_c(struct sexp *car, struct sexp_cons *cdr);

/* list */
struct sexp *sexp_l(unsigned n, ...);

/* vector */
struct sexp *sexp_v(struct object_list *l);

#if 0
/* Extracting information from sexp. These functions accept NULL
 * arguments, and return NULL if the conversion is not possible */

int sexp_consp(struct sexp *e);

/* For lists */
struct sexp *sexp_car(const struct sexp *e);
struct sexp *sexp_cdr(const struct sexp *e);
#endif

int sexp_nullp(const struct sexp *e);
int sexp_atomp(const struct sexp *e);

/* Checks that the sexp is a simple string (i.e. no display part).
 * e == NULL is allowed. */
struct lsh_string *sexp2string(struct sexp *e);

/* Returns an ATOM_FOO constant if e is a simple sexp string
 * corresponding to an atom. Or zero if that is not the case. */
int sexp2atom(struct sexp *e);

int
sexp2bignum_u(struct sexp *e, mpz_t n, UINT32 limit);

int
sexp2uint32(struct sexp *e, UINT32 *n);

/* int sexp_null_cdr(struct sexp *e); */

struct lsh_string *sexp_contents(const struct sexp *e);
struct lsh_string *sexp_display(const struct sexp *e);
int sexp_atom(const struct sexp *e);
int sexp_bignum_u(const struct sexp *e, mpz_t n);
int sexp_bignum_s(const struct sexp *e, mpz_t n);

/* Utility functions for parsing spki objects. */

int
sexp_eq(struct sexp *e, UINT32 length, const UINT8 *name);

int
sexp_atom_eq(struct sexp *e, int atom);

int
sexp_atoms_eq(struct sexp *a, struct sexp *b);

struct sexp *
sexp_assq(struct sexp_iterator *i, int atom);

struct sexp_iterator *
sexp_check_type_l(struct sexp *e, UINT32 length,
		  const UINT8 *name);

int
sexp_check_type(struct sexp *e, int type, struct sexp_iterator **res);


int sexp_get_un(struct sexp_iterator *i, int atom, mpz_t n, UINT32 limit);

extern int sexp_char_classes[];


/* Simple sexp parser */

struct sexp *
sexp_parse_canonical(struct simple_buffer *buffer);

struct sexp *
sexp_parse_transport(struct simple_buffer *buffer);

struct sexp *
string_to_sexp(int style, struct lsh_string *src, int free);


/* Streamed parser */

struct read_handler *
make_read_sexp(int style, int goon,
	       struct command_continuation *c,
	       struct exception_handler *e);

extern const struct argp sexp_input_argp;
extern const struct argp sexp_output_argp;
#define sexp_argp_state int

#endif /* LSH_SEXP_H_INCLUDED */

 

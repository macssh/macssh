/* combinators.c
 *
 * Builtin combinator functions (S, K, ...)
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

#include "command.h"
#include "xalloc.h"

#include <assert.h>

#include "combinators.c.x"

/* Combinators */

/* Ix == x */

DEFINE_COMMAND_SIMPLE(command_I, a) { return a; }


/* ((K x) y) == x */

/* Represents (K x) */
/* GABA:
   (class
     (name command_K_1)
     (super command_simple)
     (vars
       (x object lsh_object)))
*/

static struct lsh_object *
do_simple_command_K_1(struct command_simple *s,
		      struct lsh_object *ignored UNUSED)
{
  CAST(command_K_1, self, s);
  return self->x;
}

struct command *make_command_K_1(struct lsh_object *x)
{
  NEW(command_K_1, res);
  res->x = x;
  res->super.super.call = do_call_simple_command;
  res->super.call_simple = do_simple_command_K_1;

  return &res->super.super;
}

DEFINE_COMMAND_SIMPLE(command_K, a)
{ return &make_command_K_1(a)->super; }


/* ((S f) g)x == (f x)(g x) */

/* Represents ((S f) g) */
/* GABA:
   (class
     (name command_S_2)
     (super command_simple)
     (vars
       (f object command)
       (g object command)))
*/

/* Receives the value of (f x) */
/* GABA:
   (class
     (name command_S_continuation)
     (super command_frame)
     (vars
       (g object command)
       (x object lsh_object)))
*/

static void
do_command_S_continuation(struct command_continuation *c,
			  struct lsh_object *value)
{
  CAST(command_S_continuation, self, c);
  CAST_SUBTYPE(command, op, value);
  COMMAND_CALL(self->g, self->x,
	       make_apply(op, self->super.up, self->super.e),
	       self->super.e);
}

static struct command_continuation *
make_command_S_continuation(struct command *g,
			    struct lsh_object *x,
			    struct command_continuation *up,
			    struct exception_handler *e)
{
  NEW(command_S_continuation, c);
  c->g = g;
  c->x = x;
  c->super.up = up;
  c->super.e = e;
  c->super.super.c = do_command_S_continuation;

  return &c->super.super;
}

static void
do_command_S_2(struct command *s,
	       struct lsh_object *x,
	       struct command_continuation *c,
	       struct exception_handler *e)
{
  CAST(command_S_2, self, s);
  
  COMMAND_CALL(self->f, x,
	       make_command_S_continuation(self->g, x, c, e),
	       e);
}

static struct lsh_object *
do_simple_command_S_2(struct command_simple *s,
		      struct lsh_object *x)
{
  CAST(command_S_2, self, s);
  CAST_SUBTYPE(command_simple, fs, self->f);
  CAST_SUBTYPE(command_simple, gs, self->g);
  CAST_SUBTYPE(command_simple, op, COMMAND_SIMPLE_CALL(fs, x));
  
  return COMMAND_SIMPLE_CALL(op, COMMAND_SIMPLE_CALL(gs, x));
}

struct command *make_command_S_2(struct command *f,
				 struct command *g)
{
  NEW(command_S_2, res);
  res->f = f;
  res->g = g;
  res->super.super.call = do_command_S_2;
  res->super.call_simple = do_simple_command_S_2;
  
  return &res->super.super;
}

struct lsh_object *collect_S_2(struct collect_info_2 *info,
			       struct lsh_object *f,
			       struct lsh_object *g)
{
  CAST_SUBTYPE(command, cf, f);
  CAST_SUBTYPE(command, cg, g);
  assert(!info->next);
  
  return &make_command_S_2(cf, cg)->super;
}

struct collect_info_2 collect_info_S_2 =
STATIC_COLLECT_2_FINAL(collect_S_2);

struct collect_info_1 command_S =
STATIC_COLLECT_1(&collect_info_S_2);

/* S' c f g x == c (f x) (g x) */

/* Represents S' c f g */
/* GABA:
   (class
     (name command_Sp_3)
     (super command_simple)
     (vars
       (c object command)
       (f object command)
       (g object command)))
*/

static void
do_command_Sp_3(struct command *s,
		struct lsh_object *x,
		struct command_continuation *c,
		struct exception_handler *e)
{
  CAST(command_Sp_3, self, s);
  COMMAND_CALL(self->f, x,
	       make_apply(self->c,
			  make_command_S_continuation(self->g,
						      x, c, e),
			  e),
	       e);
}

static struct lsh_object *
do_simple_command_Sp_3(struct command_simple *s,
		       struct lsh_object *x)
{
  CAST(command_Sp_3, self, s);
  CAST_SUBTYPE(command_simple, cs, self->c);
  CAST_SUBTYPE(command_simple, fs, self->f);
  CAST_SUBTYPE(command_simple, gs, self->g);
  CAST_SUBTYPE(command_simple, op, COMMAND_SIMPLE_CALL(cs, COMMAND_SIMPLE_CALL(fs, x)));
  return COMMAND_SIMPLE_CALL(op, COMMAND_SIMPLE_CALL(gs, x));
}

struct command *make_command_Sp_3(struct command *c,
				  struct command *f,
				  struct command *g)
{
  NEW(command_Sp_3, res);
  res->c = c;
  res->f = f;
  res->g = g;
  res->super.super.call = do_command_Sp_3;
  res->super.call_simple = do_simple_command_Sp_3;
  
  return &res->super.super;
}

struct lsh_object *collect_Sp_3(struct collect_info_3 *info,
				struct lsh_object *c,
				struct lsh_object *f,
				struct lsh_object *g)
{
  CAST_SUBTYPE(command, cc, c);
  CAST_SUBTYPE(command, cf, f);
  CAST_SUBTYPE(command, cg, g);
  assert(!info->next);
  
  return &make_command_Sp_3(cc, cf, cg)->super;
}

struct collect_info_3 collect_info_Sp_3 =
STATIC_COLLECT_3_FINAL(collect_Sp_3);

struct collect_info_2 collect_info_Sp_2 =
STATIC_COLLECT_2(&collect_info_Sp_3);

struct collect_info_1 command_Sp =
STATIC_COLLECT_1(&collect_info_Sp_2);


/* B f g x == f (g x) */

/* Represents (B f g) */
/* GABA:
   (class
     (name command_B_2)
     (super command_simple)
     (vars
       (f object command)
       (g object command)))
*/

static void
do_command_B_2(struct command *s,
	       struct lsh_object *x,
	       struct command_continuation *c,
	       struct exception_handler *e)
{
  CAST(command_B_2, self, s);
  COMMAND_CALL(self->g, x,
	       make_apply(self->f, c, e), e);
}

static struct lsh_object *
do_simple_command_B_2(struct command_simple *s,
		      struct lsh_object *x)
{
  CAST(command_B_2, self, s);
  CAST_SUBTYPE(command_simple, fs, self->f);
  CAST_SUBTYPE(command_simple, gs, self->g);
  return COMMAND_SIMPLE_CALL(fs, COMMAND_SIMPLE_CALL(gs, x));
}

struct command *
make_command_B_2(struct command *f,
		 struct command *g)
{
  NEW(command_B_2, res);
  res->f = f;
  res->g = g;
  res->super.super.call = do_command_B_2;
  res->super.call_simple = do_simple_command_B_2;

  return &res->super.super;
}

struct lsh_object *collect_B_2(struct collect_info_2 *info,
			       struct lsh_object *f,
			       struct lsh_object *g)
{
  CAST_SUBTYPE(command, cf, f);
  CAST_SUBTYPE(command, cg, g);
  assert(!info->next);
  
  return &make_command_B_2(cf, cg)->super;
}

struct collect_info_2 collect_info_B_2 =
STATIC_COLLECT_2_FINAL(collect_B_2);

struct collect_info_1 command_B =
STATIC_COLLECT_1(&collect_info_B_2);


/* B' c f g x == c (f (g x)) */

/* Represents (B' c f g) */
/* GABA:
   (class
     (name command_Bp_3)
     (super command_simple)
     (vars
       (c object command)
       (f object command)
       (g object command)))
*/

static void
do_command_Bp_3(struct command *s,
		struct lsh_object *x,
		struct command_continuation *c,
		struct exception_handler *e)
{
  CAST(command_Bp_3, self, s);
  COMMAND_CALL(self->g, x,
	       make_apply(self->f,
			  make_apply(self->c, c, e), e),
	       e);
}

static struct lsh_object *
do_simple_command_Bp_3(struct command_simple *s,
		       struct lsh_object *x)
{
  CAST(command_Bp_3, self, s);
  CAST_SUBTYPE(command_simple, cs, self->c);
  CAST_SUBTYPE(command_simple, fs, self->f);
  CAST_SUBTYPE(command_simple, gs, self->g);
  return COMMAND_SIMPLE_CALL(cs, COMMAND_SIMPLE_CALL(fs, COMMAND_SIMPLE_CALL(gs, x)));
}

struct command *
make_command_Bp_3(struct command *c,
		  struct command *f,
		  struct command *g)
{
  NEW(command_Bp_3, res);
  res->c = c;
  res->f = f;
  res->g = g;
  res->super.super.call = do_command_Bp_3;
  res->super.call_simple = do_simple_command_Bp_3;

  return &res->super.super;
}

struct lsh_object *collect_Bp_3(struct collect_info_3 *info,
				struct lsh_object *c,
				struct lsh_object *f,
				struct lsh_object *g)
{
  CAST_SUBTYPE(command, cc, c);
  CAST_SUBTYPE(command, cf, f);
  CAST_SUBTYPE(command, cg, g);
  assert(!info->next);
  
  return &make_command_Bp_3(cc, cf, cg)->super;
}

struct collect_info_3 collect_info_Bp_3 =
STATIC_COLLECT_3_FINAL(collect_Bp_3);

struct collect_info_2 collect_info_Bp_2 =
STATIC_COLLECT_2(&collect_info_Bp_3);

struct collect_info_1 command_Bp =
STATIC_COLLECT_1(&collect_info_Bp_2);


/* ((C f) y) x == (f x) y  */

/* Represents ((C f) g) */
/* GABA:
   (class
     (name command_C_2)
     (super command_simple)
     (vars
       (f object command)
       (y object lsh_object)))
*/

/* GABA:
   (class
     (name command_C_continuation)
     (super command_frame)
     (vars
       (y object lsh_object)))
*/

static void
do_command_C_continuation(struct command_continuation *c,
			  struct lsh_object *value)
{
  CAST(command_C_continuation, self, c);
  CAST_SUBTYPE(command, op, value);
  
  COMMAND_CALL(op, self->y, self->super.up, self->super.e);
}

static struct command_continuation *
make_command_C_continuation(struct lsh_object *y,
			    struct command_continuation *up,
			    struct exception_handler *e)
{
  NEW(command_C_continuation, c);
  c->y = y;
  c->super.up = up;
  c->super.e = e;
  c->super.super.c = do_command_C_continuation;
  
  return &c->super.super;
}

static void
do_command_C_2(struct command *s,
	       struct lsh_object *x,
	       struct command_continuation *c,
	       struct exception_handler *e)
{
  CAST(command_C_2, self, s);
  
  COMMAND_CALL(self->f, x,
	       make_command_C_continuation(self->y, c, e),
	       e);
}

static struct lsh_object *do_simple_command_C_2(struct command_simple *s,
						struct lsh_object *x)
{
  CAST(command_C_2, self, s);
  CAST_SUBTYPE(command_simple, f, self->f);
  CAST_SUBTYPE(command_simple, v, COMMAND_SIMPLE_CALL(f, x));
  return COMMAND_SIMPLE_CALL(v, self->y);
}

struct command *
make_command_C_2(struct command *f,
		 struct lsh_object *y)
{
  NEW(command_C_2, res);
  res->f = f;
  res->y = y;
  res->super.super.call = do_command_C_2;
  res->super.call_simple = do_simple_command_C_2;

  return &res->super.super;
}

struct lsh_object *collect_C_2(struct collect_info_2 *info,
			       struct lsh_object *f,
			       struct lsh_object *y)
{
  CAST_SUBTYPE(command, cf, f);
  assert(!info->next);
  
  return &make_command_C_2(cf, y)->super;
}

struct collect_info_2 collect_info_C_2 =
STATIC_COLLECT_2_FINAL(collect_C_2);

struct collect_info_1 command_C =
STATIC_COLLECT_1(&collect_info_C_2);

/* C' c f y x == c (f x) y */

/* Represents (C' c f y x) */
/* GABA:
   (class
     (name command_Cp_3)
     (super command_simple)
     (vars
       (c object command)
       (f object command)
       (y object lsh_object)))
*/

static void
do_command_Cp_3(struct command *s,
		struct lsh_object *x,
		struct command_continuation *c,
		struct exception_handler *e)
{
  CAST(command_Cp_3, self, s);
  
  COMMAND_CALL(self->f, x,
	       make_apply(self->c,
			  make_command_C_continuation(self->y, c, e), e),
	       e);
}

static struct lsh_object *
do_simple_command_Cp_3(struct command_simple *s,
		      struct lsh_object *x)
{
  CAST(command_Cp_3, self, s);
  CAST_SUBTYPE(command_simple, cs, self->c);
  CAST_SUBTYPE(command_simple, fs, self->f);
  CAST_SUBTYPE(command_simple, op, COMMAND_SIMPLE_CALL(cs, COMMAND_SIMPLE_CALL(fs, x)));
  return COMMAND_SIMPLE_CALL(op, self->y);
}

struct command *
make_command_Cp_3(struct command *c,
		  struct command *f,
		  struct lsh_object *y)
{
  NEW(command_Cp_3, res);
  res->c = c;
  res->f = f;
  res->y = y;
  res->super.super.call = do_command_Cp_3;
  res->super.call_simple = do_simple_command_Cp_3;

  return &res->super.super;
}

struct lsh_object *
collect_Cp_3(struct collect_info_3 *info,
	     struct lsh_object *c,
	     struct lsh_object *f,
	     struct lsh_object *y)
{
  CAST_SUBTYPE(command, cc, c);
  CAST_SUBTYPE(command, cf, f);
  assert(!info->next);
  
  return &make_command_Cp_3(cc, cf, y)->super;
}

struct collect_info_3 collect_info_Cp_3 =
STATIC_COLLECT_3_FINAL(collect_Cp_3);

struct collect_info_2 collect_info_Cp_2 =
STATIC_COLLECT_2(&collect_info_Cp_3);

struct collect_info_1 command_Cp =
STATIC_COLLECT_1(&collect_info_Cp_2);



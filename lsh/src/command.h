/* command.h
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

#ifndef LSH_COMMAND_H_INCLUDED
#define LSH_COMMAND_H_INCLUDED

#include "lsh.h"

#include "exception.h"
#include "list.h"
/* #include "io.h" */

#include <stdarg.h>

#define GABA_DECLARE
#include "command.h.x"
#undef GABA_DECLARE

/* Continuation based command execution. A command can take one object
 * as argument, and returns one object. */

/* GABA:
   (class
     (name command_continuation)
     (vars
       (c method void "struct lsh_object *result")))
*/

#define COMMAND_RETURN(r, v) ((r)->c((r), (struct lsh_object *) (v)))

/* GABA:
   (class
     (name command)
     (vars
       (call method void "struct lsh_object *arg"
                         "struct command_continuation *c"
			 "struct exception_handler *e")))
*/

#define COMMAND_CALL(f, a, c, e) \
  ((void)&(f), ((f)->call((f), (struct lsh_object *) (a), (c), (e))))

/* NOTE: Except when inheriting command, use DEFINE_COMMAND instead. */
#define STATIC_COMMAND(f) { STATIC_HEADER, f }

#define DEFINE_COMMAND(cname)			\
static void					\
do_##cname(struct command *,			\
	   struct lsh_object *,			\
           struct command_continuation *,	\
           struct exception_handler *);		\
						\
struct command cname =				\
STATIC_COMMAND(do_##cname);			\
						\
static void					\
do_##cname

/* A command taking 2 arguments */
/* GABA:
   (class
     (name command_2)
     (super command)
     (vars
       (invoke method void
	       "struct lsh_object *a1"
	       "struct lsh_object *a2"
	       "struct command_continuation *c"
	       "struct exception_handler *e"))))
*/

#define COMMAND_2_INVOKE(s, a1, a2, c, e) \
((s)->invoke((s), (a1), (a2), (c), (e)))

void
do_command_2(struct command *s,
	     struct lsh_object *a1,
	     struct command_continuation *c,
	     struct exception_handler *e);

struct command *
make_command_2_invoke(struct command_2 *f,
		      struct lsh_object *a1);

#define DEFINE_COMMAND2(cname)				\
static void						\
do_##cname(struct command_2 *,				\
	   struct lsh_object *,				\
	   struct lsh_object *,				\
	   struct command_continuation *,		\
	   struct exception_handler *);			\
							\
struct command_2 cname =				\
{ { STATIC_HEADER, do_command_2 }, do_##cname };	\
							\
static void						\
do_##cname

/* A command taking 3 arguments */
/* GABA:
   (class
     (name command_3)
     (super command)
     (vars
       (invoke pointer
         (function void "struct lsh_object *a1"
			"struct lsh_object *a2"
			"struct lsh_object *a3"
			"struct command_continuation *c"
			"struct exception_handler *e"))))
*/

void
do_command_3(struct command *s,
	     struct lsh_object *a1,
	     struct command_continuation *c,
	     struct exception_handler *e);

struct command *
make_command_3_invoke(struct command_3 *f,
		      struct lsh_object *a1);

struct command *
make_command_3_invoke_2(struct command_3 *f,
			struct lsh_object *a1,
			struct lsh_object *a2);

#define DEFINE_COMMAND3(cname)				\
static void						\
do_##cname(struct lsh_object *,				\
	   struct lsh_object *,				\
	   struct lsh_object *,				\
	   struct command_continuation *,		\
	   struct exception_handler *);			\
							\
struct command_3 cname =				\
{ { STATIC_HEADER, do_command_3 }, do_##cname };	\
							\
static void						\
do_##cname


/* A command taking 4 arguments */
/* GABA:
   (class
     (name command_4)
     (super command)
     (vars
       (invoke pointer
         (function void "struct lsh_object *a1"
			"struct lsh_object *a2"
			"struct lsh_object *a3"
			"struct lsh_object *a4"
			"struct command_continuation *c"
			"struct exception_handler *e"))))
*/

void
do_command_4(struct command *s,
	     struct lsh_object *a1,
	     struct command_continuation *c,
	     struct exception_handler *e);

struct command *
make_command_4_invoke(struct command_4 *f,
		      struct lsh_object *a1);

struct command *
make_command_4_invoke_2(struct command_4 *f,
			struct lsh_object *a1,
			struct lsh_object *a2);

struct command *
make_command_4_invoke_3(struct command_4 *f,
			struct lsh_object *a1,
			struct lsh_object *a2,
			struct lsh_object *a3);

#define DEFINE_COMMAND4(cname)				\
static void						\
do_##cname(struct lsh_object *,				\
	   struct lsh_object *,				\
	   struct lsh_object *,				\
	   struct lsh_object *,				\
	   struct command_continuation *,		\
	   struct exception_handler *);			\
							\
struct command_4 cname =				\
{ { STATIC_HEADER, do_command_4 }, do_##cname };	\
							\
static void						\
do_##cname


void do_call_simple_command(struct command *s,
			    struct lsh_object *arg,
			    struct command_continuation *c,
			    struct exception_handler *e);

struct command *make_parallell_progn(struct object_list *body);
extern struct command progn_command;

extern struct command_continuation discard_continuation;

#define CONTINUATION_USED_P(c) ((c) != &discard_continuation)

/* GABA:
   (class
     (name command_frame)
     (super command_continuation)
     (vars
       (up object command_continuation)
       (e object exception_handler)))
*/

/* Used when the execution context must be saved for later use.
 *
 * Primarily used in channel_commands.c.
 */
/* GABA:
   (class
     (name command_context)
     (vars
       (c object command_continuation)
       (e object exception_handler)))
*/

struct command_context *
make_command_context(struct command_continuation *c,
		     struct exception_handler *e);

/* catch command,
 *
 * (catch handler body x)
 */

/* GABA:
   (class
     (name catch_command)
     (super command)
     (vars
       (mask . UINT32)
       (value . UINT32)

       ; Ignore return values from body. This means that the catch
       ; will return *only* if some exception is raised. Useful for
       ; reading until some EOF exception is raised.
       (ignore_value . int))) */

struct catch_handler_info *
make_catch_handler_info(UINT32 mask, UINT32 value,
			int ignore_value,
			struct command *handler);

struct command *
make_catch_apply(struct catch_handler_info *info,
		 struct command *body);

void
do_catch_simple(struct command *s,
		struct lsh_object *a,
		struct command_continuation *c,
		struct exception_handler *e);

#define STATIC_CATCH_COMMAND(m, v, i) \
{ STATIC_COMMAND(do_catch_simple), (m), (v), (i) }


/* GABA:
   (class
     (name catch_report_collect)
     (super command)
     (vars
       (info object report_exception_info)))
*/

struct command *
make_catch_report_apply(struct report_exception_info *info,
			struct command *body);

void
do_catch_report_collect(struct command *s,
			struct lsh_object *a,
			struct command_continuation *c,
			struct exception_handler *e);

#define STATIC_CATCH_REPORT(i) \
{ STATIC_COMMAND(do_catch_report_collect), i }


#if DEBUG_TRACE
struct command *make_trace(const char *name, struct command *real);
struct lsh_object *collect_trace(const char *name, struct lsh_object *real);

#define MAKE_TRACE collect_trace
#else /* !DEBUG_TRACE */
#define MAKE_TRACE(name, real) (real)
#endif /* !DEBUG_TRACE */

/* Useful clobal commands */
#define PROG1 (&command_K.super.super)
#define PROGN (&progn_command.super)

/* The GABA_* macros are used by automatically generated evaluation code */

struct command_continuation *
make_apply(struct command *f,
	   struct command_continuation *c, struct exception_handler *e);

struct lsh_object *gaba_apply(struct lsh_object *f,
			      struct lsh_object *x);

#define GABA_APPLY gaba_apply

extern struct command command_I;
#define GABA_VALUE_I (&command_I.super)
#define GABA_APPLY_I_1(x) (x)

extern struct command_2 command_K;
struct command *make_command_K_1(struct lsh_object *x);

#define GABA_VALUE_K (&command_K.super.super)
#define GABA_APPLY_K_1(x) \
((struct lsh_object *) make_command_2_invoke(&command_K, (x)))

extern struct command_3 command_S;

#define GABA_VALUE_S (&command_S.super.super)
#define GABA_APPLY_S_1(f) \
((struct lsh_object *) make_command_3_invoke(&command_S, (f)))
#define GABA_APPLY_S_2(f, g) \
((struct lsh_object *) make_command_3_invoke_2(&command_S, (f), (g)))

extern struct command_4 command_Sp;

#define GABA_VALUE_Sp (&command_Sp.super.super)
#define GABA_APPLY_Sp_1(c) \
((struct lsh_object *) make_command_4_invoke(&command_Sp, (f)))
#define GABA_APPLY_Sp_2(c, f) \
((struct lsh_object *) make_command_4_invoke_2(&command_Sp, (c), (f)))
#define GABA_APPLY_Sp_3(c, f, g) \
((struct lsh_object *) make_command_4_invoke_3(&command_Sp, (c), (f), (g)))

extern struct command_3 command_B;

#define GABA_VALUE_B (&command_B.super.super)
#define GABA_APPLY_B_1(f) \
((struct lsh_object *) make_command_3_invoke(&command_B, (f)))
#define GABA_APPLY_B_2(f, g) \
((struct lsh_object *) make_command_3_invoke_2(&command_B, (f), (g)))

extern struct command_4 command_Bp;

#define GABA_VALUE_Bp (&command_Bp.super.super)
#define GABA_APPLY_Bp_1(c) \
((struct lsh_object *) make_command_4_invoke(&command_Bp, (c)))
#define GABA_APPLY_Bp_2(c, f) \
((struct lsh_object *) make_command_4_invoke_2(&command_Bp, (c), (f)))
#define GABA_APPLY_Bp_3(c, f, g) \
((struct lsh_object *) make_command_4_invoke_3(&command_Bp, (c), (f), (g)))

extern struct command_3 command_C;

#define GABA_VALUE_C (&command_C.super.super)
#define GABA_APPLY_C_1(f) \
((struct lsh_object *) make_command_3_invoke(&command_C, (f)))
#define GABA_APPLY_C_2(f, g) \
((struct lsh_object *) make_command_3_invoke_2(&command_C, (f), (g)))

#if 1
extern struct command_4 command_Cp;

#define GABA_VALUE_Cp (&command_Cp.super.super)
#define GABA_APPLY_Cp_1(c) \
((struct lsh_object *) make_command_4_invoke(&command_Cp, (c)))
#define GABA_APPLY_Cp_2(c, f) \
((struct lsh_object *) make_command_4_invoke_2(&command_Cp, (c), (f)))
#define GABA_APPLY_Cp_3(c, f, g) \
((struct lsh_object *) make_command_4_invoke_3(&command_Cp, (c), (f), (g)))
#else

extern struct collect_info_1 command_Cp;
extern struct collect_info_2 collect_info_Cp_2;
extern struct collect_info_3 collect_info_Cp_3;

struct command *
make_command_Cp_3(struct command *c,
		  struct command *f,
		  struct lsh_object *y);
struct lsh_object *
collect_Cp_3(struct collect_info_3 *info,
	     struct lsh_object *c,
	     struct lsh_object *f,
	     struct lsh_object *y);

#define GABA_VALUE_Cp (&command_Cp.super.super.super)
#define GABA_APPLY_Cp_1(c) (make_collect_state_1(&command_Cp, (c)))
#define GABA_APPLY_Cp_2(c, f) \
  (make_collect_state_2(&collect_info_Cp_2, (c), (f)))
#define GABA_APPLY_Cp_3(c, f, y) (collect_Cp_3(&collect_info_Cp_3, (c), (f), (y)))
     
#endif

#endif /* LSH_COMMAND_H_INCLUDED */ 

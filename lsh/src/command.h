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

/* GABA:
   (class
     (name command)
     (vars
       (call method void "struct lsh_object *arg"
                         "struct command_continuation *c"
			 "struct exception_handler *e")))
*/

/* GABA:
   (class
     (name command_simple)
     (super command)
     (vars
       ; Like call, but returns the value immediately rather than
       ; using a continuation function.
       (call_simple method "struct lsh_object *" "struct lsh_object *")))
*/

#define COMMAND_CALL(f, a, c, e) \
  ((void)&(f), ((f)->call((f), (struct lsh_object *) (a), (c), (e))))

#define COMMAND_RETURN(r, v) ((r)->c((r), (struct lsh_object *) (v)))

#define COMMAND_SIMPLE_CALL(f, a) \
  ((f)->call_simple((f), (struct lsh_object *)(a)))

#define STATIC_COMMAND(f) { STATIC_HEADER, f }

#define STATIC_COMMAND_SIMPLE(f) \
{ STATIC_COMMAND(do_call_simple_command), f}


#define DEFINE_COMMAND_SIMPLE(cname, ARG)		\
static struct lsh_object *				\
do_simple_##cname(struct command_simple *,		\
		  struct lsh_object *a);		\
							\
struct command_simple cname =				\
STATIC_COMMAND_SIMPLE(do_simple_##cname);		\
							\
static struct lsh_object *				\
do_simple_##cname(struct command_simple *s UNUSED,	\
		  struct lsh_object *ARG)

#define DEFINE_COMMAND(cname, ARG, CC, EXC)	\
static void					\
do_##cname(struct command *s UNUSED,		\
	   struct lsh_object *ARG,		\
           struct command_continuation *CC,	\
           struct exception_handler *EXC);	\
						\
struct command cname =				\
STATIC_COMMAND(do_##cname);			\
						\
static void					\
do_##cname(struct command *s UNUSED,		\
	   struct lsh_object *ARG,		\
           struct command_continuation *CC,	\
           struct exception_handler *EXC)


void do_call_simple_command(struct command *s,
			    struct lsh_object *arg,
			    struct command_continuation *c,
			    struct exception_handler *e);

struct command *make_parallell_progn(struct object_list *body);
extern struct command_simple progn_command;

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
     (super command_simple)
     (vars
       (mask . UINT32)
       (value . UINT32)

       ; Ignore return values from body. This means that the catch
       ; will return *only* if some exception is raised. Useful for
       ; reading until some EOF exception ir raised.
       (ignore_value . int))) */

struct catch_handler_info *
make_catch_handler_info(UINT32 mask, UINT32 value,
			int ignore_value,
			struct command *handler);

struct command *
make_catch_apply(struct catch_handler_info *info,
		 struct command *body);

struct lsh_object *
do_catch_simple(struct command_simple *s,
		struct lsh_object *a);

#define STATIC_CATCH_COMMAND(m, v, i) \
{ STATIC_COMMAND_SIMPLE(do_catch_simple), (m), (v), (i) }


/* GABA:
   (class
     (name catch_report_collect)
     (super command_simple)
     (vars
       (info object report_exception_info)))
*/

struct command *
make_catch_report_apply(struct report_exception_info *info,
			struct command *body);

struct lsh_object *
do_catch_report_collect(struct command_simple *s,
			struct lsh_object *a);

#define STATIC_CATCH_REPORT(i) \
{ STATIC_COMMAND_SIMPLE(do_catch_report_collect), i }

       
/* Commands that need to collect some arguments before actually doing
 * anything. */

/* The collect_info_n classes keeps track about what to do whith the
 * next argument. As long as we collect arguments without doing
 * anything, the f field in collect_info_n will point to the
 * constructor make_collect_state_n. */
/* GABA:
   (class
     (name collect_info_4)
     (vars
       ; No next field
       (f method "struct lsh_object *"
                 "struct lsh_object *" "struct lsh_object *"
		 "struct lsh_object *" "struct lsh_object *")))
*/

/* GABA:
   (class
     (name collect_info_3)
     (vars
       (f method  "struct lsh_object *"
                  "struct lsh_object *" "struct lsh_object *"
		  "struct lsh_object *")
       (next object collect_info_4)))
*/

/* GABA:
   (class
     (name collect_info_2)
     (vars
       (f method  "struct lsh_object *"
                  "struct lsh_object *" "struct lsh_object *")
       (next object collect_info_3)))
*/

/* GABA:
   (class
     (name collect_info_1)
     (super command_simple)
     (vars
       (f method  "struct lsh_object *"
                  "struct lsh_object *")
       (next object collect_info_2)))
*/

struct lsh_object *
do_collect_1(struct command_simple *s, struct lsh_object *a);

struct lsh_object *
make_collect_state_1(struct collect_info_1 *info,
		     struct lsh_object *a);

struct lsh_object *
make_collect_state_2(struct collect_info_2 *info,
		     struct lsh_object *a,
		     struct lsh_object *b);

struct lsh_object *
make_collect_state_3(struct collect_info_3 *info,
		     struct lsh_object *a,
		     struct lsh_object *b,
		     struct lsh_object *c);

#define STATIC_COLLECT_1(next) \
{ { { STATIC_HEADER, do_call_simple_command }, do_collect_1}, \
  make_collect_state_1, next }

#if 0
#define STATIC_COLLECT_1_FINAL(f) \
{ { { STATIC_HEADER, do_call_simple_command }, do_collect_1}, \
  f, NULL }
#endif

#define STATIC_COLLECT_2(next) \
{ STATIC_HEADER, make_collect_state_2, next }

#define STATIC_COLLECT_2_FINAL(f) \
{ STATIC_HEADER, f, NULL }

#define STATIC_COLLECT_3(next) \
{ STATIC_HEADER, make_collect_state_3, next }

#define STATIC_COLLECT_3_FINAL(f) \
{ STATIC_HEADER, f, NULL }

extern struct command_simple command_unimplemented;
#define COMMAND_UNIMPLEMENTED (&command_unimplemented.super.super)

extern struct command command_die_on_null;


#if DEBUG_TRACE
struct command *make_trace(const char *name, struct command *real);
struct lsh_object *collect_trace(const char *name, struct lsh_object *real);

#define MAKE_TRACE collect_trace
#else /* !DEBUG_TRACE */
#define MAKE_TRACE(name, real) (real)
#endif /* !DEBUG_TRACE */

struct command_continuation *
make_once_continuation(const char *msg,
		       struct command_continuation *up);


/* Delayed application. This is just a convenient way to record how to
 * apply a function, at some later time. Currently used only by
 * server_userauth.c. */

/* GABA:
   (class
     (name delayed_apply)
     (super command_context)
     (vars
       (f object command)
       (a object lsh_object)))
*/

struct delayed_apply *
make_delayed_apply(struct command *f,
		   struct lsh_object *a);

#define FORCE_APPLY(d, c, e) COMMAND_CALL((d)->f, (d)->a, (c), (e))

struct command_continuation *
make_delay_continuation(struct command *f,
			struct command_continuation *c);

/* Useful clobal commands */
#define PROG1 (&command_K.super.super)
#define PROGN (&progn_command.super.super)

/* The GABA_* macros are used by automatically generated evaluation code */

struct command_continuation *
make_apply(struct command *f,
	   struct command_continuation *c, struct exception_handler *e);

struct lsh_object *gaba_apply(struct lsh_object *f,
			      struct lsh_object *x);

#define GABA_APPLY gaba_apply

extern struct command_simple command_I;
#define GABA_VALUE_I (&command_I.super.super)
#define GABA_APPLY_I_1(x) (x)

extern struct command_simple command_K;
struct command *make_command_K_1(struct lsh_object *x);

#define GABA_VALUE_K (&command_K.super.super)
#define GABA_APPLY_K_1(x) ((struct lsh_object *) make_command_K_1(x))

extern struct collect_info_1 command_S;
extern struct collect_info_2 collect_info_S_2; 

struct command *make_command_S_2(struct command *f,
				 struct command *g);

struct lsh_object *collect_S_2(struct collect_info_2 *info,
			       struct lsh_object *f,
			       struct lsh_object *g);

#define GABA_VALUE_S (&command_S.super.super.super)
#define GABA_APPLY_S_1(f) (make_collect_state_1(&command_S, (f)))
#define GABA_APPLY_S_2(f, g) (collect_S_2(&collect_info_S_2, (f), (g)))

extern struct collect_info_1 command_Sp;
extern struct collect_info_2 collect_info_Sp_2;
extern struct collect_info_3 collect_info_Sp_3;

struct command *make_command_Sp_3(struct command *c,
				  struct command *f,
				  struct command *g);

struct lsh_object *collect_Sp_3(struct collect_info_3 *info,
				struct lsh_object *c,
				struct lsh_object *f,
				struct lsh_object *g);

#define GABA_VALUE_Sp (&command_Sp.super.super.super)
#define GABA_APPLY_Sp_1(c) (make_collect_state_1(&command_Sp, (c)))
#define GABA_APPLY_Sp_2(c, f) \
  (make_collect_state_2(&collect_info_Sp_2, (c), (f)))
#define GABA_APPLY_Sp_3(c, f, g) (collect_Sp_3(&collect_info_Sp_3, (c), (f), (g)))

extern struct collect_info_1 command_B;
extern struct collect_info_2 collect_info_B_2; 

struct command *make_command_B_2(struct command *f,
				 struct command *g);
struct lsh_object *collect_B_2(struct collect_info_2 *info,
			       struct lsh_object *f,
			       struct lsh_object *g);

#define GABA_VALUE_B (&command_B.super.super.super)
#define GABA_APPLY_B_1(f) (make_collect_state_1(&command_B, (f)))
#define GABA_APPLY_B_2(f, g) (collect_B_2(&collect_info_B_2, (f), (g)))

extern struct collect_info_1 command_Bp;
extern struct collect_info_2 collect_info_Bp_2;
extern struct collect_info_3 collect_info_Bp_3;

struct command *make_command_Bp_3(struct command *c,
				  struct command *f,
				  struct command *g);
struct lsh_object *collect_Bp_3(struct collect_info_3 *info,
				struct lsh_object *c,
				struct lsh_object *f,
				struct lsh_object *g);

#define GABA_VALUE_Bp (&command_Bp.super.super.super)
#define GABA_APPLY_Bp_1(c) (make_collect_state_1(&command_Bp, (c)))
#define GABA_APPLY_Bp_2(c, f) \
  (make_collect_state_2(&collect_info_Bp_2, (c), (f)))
#define GABA_APPLY_Bp_3(c, f, g) (collect_Bp_3(&collect_info_Bp_3, (c), (f), (g)))

extern struct collect_info_1 command_C;
extern struct collect_info_2 collect_info_C_2; 

struct command *
make_command_C_2(struct command *f,
		 struct lsh_object *y);
struct lsh_object *
collect_C_2(struct collect_info_2 *info,
	    struct lsh_object *f,
	    struct lsh_object *y);

#define GABA_VALUE_C (&command_C.super.super.super)
#define GABA_APPLY_C_1(f) (make_collect_state_1(&command_C, (f)))
#define GABA_APPLY_C_2(f, y) (collect_C_2(&collect_info_C_2, (f), (y)))

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
     

#endif /* LSH_COMMAND_H_INCLUDED */ 

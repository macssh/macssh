/* command.c
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

#include "connection.h"
#include "io.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "command.h.x"
#undef GABA_DEFINE

#include "command.c.x"

static void
do_discard_continuation(struct command_continuation *ignored UNUSED,
			struct lsh_object *x UNUSED)
{}

struct command_continuation discard_continuation =
{ STATIC_HEADER, do_discard_continuation};

struct command_context *
make_command_context(struct command_continuation *c,
		     struct exception_handler *e)
{
  NEW(command_context, self);
  self->c = c;
  self->e = e;

  return self;
}
  
/* GABA:
   (class
     (name command_apply)
     (super command_frame)
     (vars
       (f object command)))
*/

static void
do_command_apply(struct command_continuation *s,
		 struct lsh_object *value)
{
  CAST(command_apply, self, s);
  COMMAND_CALL(self->f, value,
	       self->super.up, self->super.e);
}

struct command_continuation *
make_apply(struct command *f,
	   struct command_continuation *c, struct exception_handler *e)
{
  NEW(command_apply, res);
  assert(f);
  assert(c);
  assert(e);
  
  res->f = f;
  res->super.up = c;
  res->super.e = e;
  res->super.super.c = do_command_apply;

  return &res->super.super;
}

struct lsh_object *gaba_apply(struct lsh_object *f,
			      struct lsh_object *x)
{
  CAST_SUBTYPE(command_simple, cf, f);
  return COMMAND_SIMPLE_CALL(cf, x);
}

void
do_call_simple_command(struct command *s,
		       struct lsh_object *arg,
		       struct command_continuation *c,
		       struct exception_handler *e UNUSED)
{
  CAST_SUBTYPE(command_simple, self, s);
  COMMAND_RETURN(c, COMMAND_SIMPLE_CALL(self, arg));
}


/* Unimplemented command */
static void
do_command_unimplemented(struct command *s UNUSED,
			 struct lsh_object *o UNUSED,
			 struct command_continuation *c UNUSED,
			 struct exception_handler *e UNUSED)
{ fatal("command.c: Unimplemented command.\n"); }

static struct lsh_object *
do_command_simple_unimplemented(struct command_simple *s UNUSED,
				struct lsh_object *o UNUSED)
{ fatal("command.c: Unimplemented simple command.\n"); }

struct command_simple command_unimplemented =
{ { STATIC_HEADER, do_command_unimplemented}, do_command_simple_unimplemented};


/* Tracing */

#if DEBUG_TRACE

/* GABA:
   (class
     (name trace_continuation)
     (super command_continuation)
     (vars
       (name . "const char *")
       (real object command_continuation)))
*/

static void
do_trace_continuation(struct command_continuation *s,
		      struct lsh_object *x)
{
  CAST(trace_continuation, self, s);
  const char *type;

  if (x)
    type = (x->isa) ? x->isa->name : "<STATIC>";
  else
    type = "<NULL>";
  
  trace("Leaving %z, value of type %z.\n",
	self->name, type);
  COMMAND_RETURN(self->real, x);
}

static struct command_continuation *
make_trace_continuation(const char *name,
			struct command_continuation *real)
{
  NEW(trace_continuation, self);
  self->super.c = do_trace_continuation;
  self->name = name;
  self->real = real;

  return &self->super;
}

/* GABA:
   (class
     (name trace_command)
     (super command)
     (vars
       (name . "const char *")
       (real object command)))
*/

static void
do_trace_command(struct command *s,
		 struct lsh_object *x,
		 struct command_continuation *c,
		 struct exception_handler *e)
{
  CAST(trace_command, self, s);

  trace("Entering %z\n", self->name);
#if 1
  COMMAND_CALL(self->real, x,
	       make_trace_continuation(self->name, c),
	       e);
#else
  COMMAND_CALL(self->real, x, c, e);
#endif
}

struct command *make_trace(const char *name, struct command *real)
{
  NEW(trace_command, self);
  self->super.call = do_trace_command;
  self->name = name;
  self->real = real;

  return &self->super;
}

struct lsh_object *collect_trace(const char *name, struct lsh_object *c)
{
  CAST_SUBTYPE(command, real, c);
  return &make_trace(name, real)->super;
}
#endif /* DEBUG_TRACE */

/* Collecting arguments */
struct lsh_object *
do_collect_1(struct command_simple *s, struct lsh_object *a)
{
  CAST(collect_info_1, self, s);
  return self->f(self, a);
}

/* GABA:
   (class
     (name collect_state_1)
     (super command_simple)
     (vars
       (info object collect_info_2)
       (a object lsh_object)))
*/

/* GABA:
   (class
     (name collect_state_2)
     (super command_simple)
     (vars
       (info object collect_info_3)
       (a object lsh_object)
       (b object lsh_object)))
*/

/* GABA:
   (class
     (name collect_state_3)
     (super command_simple)
     (vars
       (info object collect_info_4)
       (a object lsh_object)
       (b object lsh_object)
       (c object lsh_object)))
*/

static struct lsh_object *
do_collect_2(struct command_simple *s,
	     struct lsh_object *x)
{
  CAST(collect_state_1, self, s);
  return self->info->f(self->info, self->a, x);
}

struct lsh_object *
make_collect_state_1(struct collect_info_1 *info,
		     struct lsh_object *a)
{
  NEW(collect_state_1, self);
  self->info = info->next;
  self->a = a;

  self->super.call_simple = do_collect_2;
  self->super.super.call = do_call_simple_command;
  
  return &self->super.super.super;
}

static struct lsh_object *
do_collect_3(struct command_simple *s,
	     struct lsh_object *x)
{
  CAST(collect_state_2, self, s);
  return self->info->f(self->info, self->a, self->b, x);
}

struct lsh_object *
make_collect_state_2(struct collect_info_2 *info,
		     struct lsh_object *a,
		     struct lsh_object *b)
{
  NEW(collect_state_2, self);
  self->info = info->next;
  self->a = a;
  self->b = b;
  
  self->super.call_simple = do_collect_3;
  self->super.super.call = do_call_simple_command;
  
  return &self->super.super.super;
}

static struct lsh_object *
do_collect_4(struct command_simple *s,
	     struct lsh_object *x)
{
  CAST(collect_state_3, self, s);
  return self->info->f(self->info, self->a, self->b, self->c, x);
}

struct lsh_object *
make_collect_state_3(struct collect_info_3 *info,
		     struct lsh_object *a,
		     struct lsh_object *b,
		     struct lsh_object *c)
{
  NEW(collect_state_3, self);
  self->info = info->next;
  self->a = a;
  self->b = b;
  self->c = c;
  
  self->super.call_simple = do_collect_4;
  self->super.super.call = do_call_simple_command;
  
  return &self->super.super.super;
}

/* GABA:
   (class
     (name parallell_progn)
     (super command)
     (vars
       (body object object_list)))
*/

static void
do_parallell_progn(struct command *s,
		   struct lsh_object *x,
		   struct command_continuation *c,
		   struct exception_handler *e)
{
  CAST(parallell_progn, self, s);
  unsigned i;
  
  for (i=0; i < LIST_LENGTH(self->body) - 1; i++)
    {
      CAST_SUBTYPE(command, command, LIST(self->body)[i]);
      COMMAND_CALL(command, x, &discard_continuation, e);
    }
  {
    CAST_SUBTYPE(command, command, LIST(self->body)[i]);
    
    COMMAND_CALL(command, x, c, e);
  }
}

struct command *make_parallell_progn(struct object_list *body)
{
  assert(LIST_LENGTH(body));
  {
    NEW(parallell_progn, self);
    self->body = body;
    self->super.call = do_parallell_progn;

    return &self->super;
  }
}

DEFINE_COMMAND_SIMPLE(progn_command, a)
{
  CAST(object_list, body, a);
  return LIST_LENGTH(body) ? &make_parallell_progn(body)->super
    : &command_I.super.super;
}


/* A continuation that passes on its value only the first time it is
 * invoked. */
/* GABA:
   (class
     (name once_continuation)
     (super command_continuation)
     (vars
       (invoked . int)
       (msg . "const char *")
       (up object command_continuation)))
*/

static void
do_once_continuation(struct command_continuation *s,
		     struct lsh_object *value)
{
  CAST(once_continuation, self, s);

  if (!self->invoked)
    {
      self->invoked = 1;
      COMMAND_RETURN(self->up, value);
    }
  else if (self->msg)
    debug("%z", self->msg);
}

struct command_continuation *
make_once_continuation(const char *msg,
		       struct command_continuation *up)
{
  NEW(once_continuation, self);
  self->super.c = do_once_continuation;
  self->invoked = 0;
  self->msg = msg;
  self->up = up;

  return &self->super;
}


/* Delayed application */

struct delayed_apply *
make_delayed_apply(struct command *f,
		   struct lsh_object *a)
{
  NEW(delayed_apply, self);
  self->f = f;
  self->a = a;
  return self;
}

/* GABA:
   (class
     (name delay_continuation)
     (super command_continuation)
     (vars
       (f object command)
       (up object command_continuation)))
*/

static void
do_delay_continuation(struct command_continuation *c,
		      struct lsh_object *o)
{
  CAST(delay_continuation, self, c);

  COMMAND_RETURN(self->up, make_delayed_apply(self->f, o));
}

struct command_continuation *
make_delay_continuation(struct command *f,
			struct command_continuation *c)
{
  NEW(delay_continuation, self);
  self->super.c = do_delay_continuation;
  self->up = c;
  self->f = f;

  return &self->super;
}

   
/* Catch command
 *
 * (catch handler body x)
 *
 * or
 *
 * ( (catch handler body) x)
 *
 * Invokes (body x), with an exception handler that passes exceptions
 * of certain types to handler. */

/* GABA:
   (class
     (name catch_handler_info)
     (vars
       (mask . UINT32)
       (value . UINT32)
       (ignore_value . int)
       ; NULL handler means ignore all caught exceptions.
       (handler object command)))
*/

struct catch_handler_info *
make_catch_handler_info(UINT32 mask, UINT32 value,
			int ignore_value,
			struct command *handler)
{
  NEW(catch_handler_info, self);
  self->mask = mask;
  self->value = value;
  self->ignore_value = ignore_value;
  self->handler = handler;

  return self;
}

/* GABA:
   (class
     (name catch_handler)
     (super exception_handler)
     (vars
       (c object command_continuation)
       (info object catch_handler_info)))
*/

static void
do_catch_handler(struct exception_handler *s,
		 const struct exception *e)
{
  CAST(catch_handler, self, s);
  
  if ((e->type & self->info->mask) == self->info->value)
    {
      if (self->info->handler)
	COMMAND_CALL(self->info->handler,
		     e, self->c, self->super.parent);
      else
	trace("do_catch_handler: Ignoring exception: %z.\n",
	      e->msg);
    }
  else
    EXCEPTION_RAISE(self->super.parent, e);
}

static struct exception_handler *
make_catch_handler(struct catch_handler_info *info,
		   struct command_continuation *c,
		   struct exception_handler *e,
		   const char *context)
{
  NEW(catch_handler, self);

  self->super.raise = do_catch_handler;
  self->super.parent = e;
  self->super.context = context;
  
  self->c = c;
  self->info = info;

  return &self->super;
}

/* GABA:
   (class
     (name catch_apply)
     (super command)
     (vars
       (info object catch_handler_info)
       (body object command)))
*/

static void
do_catch_apply(struct command *s,
	       struct lsh_object *a,
	       struct command_continuation *c,
	       struct exception_handler *e)
{
  CAST(catch_apply, self, s);

  COMMAND_CALL(self->body, a, (self->info->ignore_value
			       ? &discard_continuation
			       : c),
	       make_catch_handler(self->info, c, e,
				  HANDLER_CONTEXT));
}

struct command *
make_catch_apply(struct catch_handler_info *info,
		 struct command *body)
{
  NEW(catch_apply, self);
  self->super.call = do_catch_apply;
  self->info = info;
  self->body = body;

  return &self->super;
}


/* GABA:
   (class
     (name catch_collect_body)
     (super command_simple)
     (vars
       (info object catch_handler_info)))
*/

static struct lsh_object *
do_catch_collect_body(struct command_simple *s,
		      struct lsh_object *a)
{
  CAST(catch_collect_body, self, s);
  CAST_SUBTYPE(command, body, a);

  return &make_catch_apply(self->info, body)->super;
}

static struct command *
make_catch_collect_body(struct catch_handler_info *info)
{
  NEW(catch_collect_body, self);
  self->super.super.call = do_call_simple_command;
  self->super.call_simple = do_catch_collect_body;
  self->info = info;

  return &self->super.super;
}

struct lsh_object *
do_catch_simple(struct command_simple *s,
		struct lsh_object *a)
{
  CAST(catch_command, self, s);
  CAST_SUBTYPE(command, f, a);

  return &(make_catch_collect_body(make_catch_handler_info(self->mask,
							   self->value, self->ignore_value, f))
	   ->super);
}


/* Catch and report some exceptions.
 *
 * FIXME: This duplicates most of the catch command. */

/* GABA:
   (class
     (name catch_report_apply)
     (super command)
     (vars
       (info object report_exception_info)
       (body object command)))
*/
   
static void
do_catch_report_apply(struct command *s,
		      struct lsh_object *a,
		      struct command_continuation *c,
		      struct exception_handler *e)
{
  CAST(catch_report_apply, self, s);

  COMMAND_CALL(self->body, a, c,
	       make_report_exception_handler(self->info, e,
					     HANDLER_CONTEXT));
}

struct command *
make_catch_report_apply(struct report_exception_info *info,
			struct command *body)
{
  NEW(catch_report_apply, self);
  self->super.call = do_catch_report_apply;
  self->info = info;
  self->body = body;

  return &self->super;
}

struct lsh_object *
do_catch_report_collect(struct command_simple *s,
			struct lsh_object *a)
{
  CAST(catch_report_collect, self, s);
  CAST_SUBTYPE(command, body, a);

  return &make_catch_report_apply(self->info, body)->super;
}

#if 0
static struct command *
make_catch_report_collect_body(struct catch_handler_info *info)
{
  NEW(catch_report_collect_body, self);
  self->super.super.call = do_call_simple_command;
  self->super.call_simple = do_catch_report_collect_body;
  self->info = info;

  return &self->super.super;
}
#endif

/* exception.c
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

#include "exception.h"

#include "io.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "exception.h.x"
#undef GABA_DEFINE

#include "exception.c.x"

static void
do_default_handler(struct exception_handler *ignored UNUSED,
		   const struct exception *e)
{
  fatal("Unhandled exception of type 0x%xi: %z\n", e->type, e->msg);
}

struct exception_handler default_exception_handler =
STATIC_EXCEPTION_HANDLER(do_default_handler, NULL);

static void
do_ignore_exception_handler(struct exception_handler *self UNUSED,
			    const struct exception *e UNUSED)
{}

struct exception_handler ignore_exception_handler =
STATIC_EXCEPTION_HANDLER(do_ignore_exception_handler, NULL);

struct exception_handler *
make_exception_handler(void (*raise)(struct exception_handler *s,
				     const struct exception *x),
		       struct exception_handler *parent,
		       const char *context)
{
  NEW(exception_handler, self);
  self->raise = raise;
  self->parent = parent;
  self->context = context;
  
  return self;
}

struct report_exception_info *
make_report_exception_info(UINT32 mask, UINT32 value,
			   const char *prefix)
{
  NEW(report_exception_info, self);

  self->mask = mask;
  self->value = value;
  self->prefix = prefix;

  return self;
}

/* GABA:
   (class
     (name report_exception_handler)
     (super exception_handler)
     (vars
       (info object report_exception_info)))
*/

static void
do_report_exception_handler(struct exception_handler *s,
			    const struct exception *x)
{
  CAST(report_exception_handler, self, s);
  struct report_exception_info *info = self->info;
  
  if ( (x->type & info->mask) == info->value)
    werror("%z exception: %z\n", info->prefix, x->msg);
  else
    EXCEPTION_RAISE(self->super.parent, x);
}

struct exception_handler *
make_report_exception_handler(struct report_exception_info *info,
			      struct exception_handler *parent,
			      const char *context)
{
  NEW(report_exception_handler, self);
  self->super.raise = do_report_exception_handler;
  self->super.parent = parent;
  self->super.context = context;

  self->info = info;

  return &self->super;
}

struct exception *make_simple_exception(UINT32 type, const char *msg)
{
  NEW(exception, e);
  e->type = type;
  e->msg = msg;

  return e;
}

/* Reason == 0 means disconnect without sending any disconnect
 * message. */

struct exception *
make_protocol_exception(UINT32 reason, const char *msg)
{
  NEW(protocol_exception, self);

#define MAX_REASON 15
  const char *messages[MAX_REASON+1] =
  {
    NULL, "Host not allowed to connect",
    "Protocol error", "Key exchange failed",
    "Host authentication failed", "MAC error",
    "Compression error", "Service not available",
    "Protocol version not supported", "Host key not verifiable",
    "Connection lost", "By application",
    "Too many connections", "Auth cancelled by user",
    "No more auth methods available", "Illegal user name"
  };
    
  assert(reason <= MAX_REASON);
  assert(reason != SSH_DISCONNECT_RESERVED);
#undef MAX_REASON

  self->super.type = EXC_PROTOCOL;
  self->super.msg = msg ? msg : messages[reason];

  self->reason = reason;

  return &self->super;
}

#if DEBUG_TRACE
void exception_raise(struct exception_handler *h,
		     const struct exception *e,
		     const char *context)
{
  trace ("%z: Raising exception %z (type %i), using handler installed by %z\n",
	 context, e->msg, e->type, h->context);
  h->raise(h, e);
}
#endif /* DEBUG_TRACE */

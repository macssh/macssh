/* exception.h
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

#ifndef LSH_EXCEPTION_H_INCLUDED
#define LSH_EXCEPTION_H_INCLUDED

#include "lsh.h"

#define GABA_DECLARE
#include "exception.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name exception)
     (vars
       (type . UINT32)
       (msg . "const char *")))
*/

#define STATIC_EXCEPTION(type, name) \
{ STATIC_HEADER, (type), (name) }

/* GABA:
   (class
     (name exception_handler)
     (vars
       (raise method void "const struct exception *")
       (parent object exception_handler)

       ; Provide some context for debugging unhandled exceptions
       (context . "const char *")))
*/

void exception_raise(struct exception_handler *e,
		     const struct exception *h,
		     const char *context);

#if DEBUG_TRACE
#  define EXCEPTION_RAISE(h, e) exception_raise((h), (e), HANDLER_CONTEXT)
#else /* !DEBUG_TRACE */
#  define EXCEPTION_RAISE(h, e)  ((h)->raise((h), (e)))
#endif /* !DEBUG_TRACE */

/* NOTE: This is pretty useless, as it requires that the parent be
 * static as well. Used only for the default_exception_handler and
 * ignore_exception_handler, and perhaps some others with NULL parent.
 * */
#define STATIC_EXCEPTION_HANDLER(r, p) \
{ STATIC_HEADER, (r), (p), __FILE__ ":" STRING_LINE ": Static" }


#define HANDLER_CONTEXT   (__FILE__ ":" STRING_LINE ": " FUNCTION_NAME)


/* Exception types. */

/* Used in places where no real exception is defined yet.
 * Never handled. */
#define EXC_DUMMY 0

/* FIXME: This is an inappropriate name, as this exception type is
 * used for all events that should result in a disconnect message. */

/* Protocol errors */
#define EXC_PROTOCOL 0x1000

/* IO related errors */
#define EXC_IO 0x2000
#define EXC_IO_CONNECT 0x2001
#define EXC_IO_BLOCKING_WRITE 0x2003
#define EXC_IO_BLOCKING_READ 0x2004
#define EXC_IO_READ 0x2005
#define EXC_IO_WRITE 0x2006
#define EXC_IO_OPEN_WRITE 0x2007
#define EXC_IO_OPEN_READ 0x2008
#define EXC_IO_LISTEN 0x2009
#define EXC_IO_CLOSE 0x200A

/* Authorization errors */
#define EXC_AUTH 0x4000
#define EXC_USERAUTH 0x4001

/* Implies an exception struct with a reply packet in it. */
#define EXC_USERAUTH_SPECIAL 0x4002

/* Services */
#define EXC_SERVICE 0x8000
#define EXC_GLOBAL_REQUEST 0x8001
#define EXC_CHANNEL_REQUEST 0x8002

/* Use subtypes for the different error codes? */
#define EXC_CHANNEL_OPEN 0x8003

/* Closing down things */
#define EXC_FINISH 0x10000

/* Close a channel */
#define EXC_FINISH_CHANNEL 0x10001

/* Stop reading on some fd */
#define EXC_FINISH_READ 0x10002

/* Close the connection immediately */
#define EXC_FINISH_IO 0x10003

/* Put the connection into pending-close mode,
 * i.e. don't open any new channels, and close it
 * as soon as all channels are gone. */
#define EXC_FINISH_PENDING 0x10004

/* DNS errors */
#define EXC_RESOLVE 0x20001

/* SEXP parse errors */
#define EXC_SEXP 0x40000
#define EXC_SEXP_SYNTAX 0x40001
#define EXC_SEXP_EOF 0x40002

/* SPKI errors */
#define EXC_SPKI 0x80000
/* Unexpected or unsupported expression type */
#define EXC_SPKI_TYPE 0x80001

/* Pausing, used to temporarily stop reading packets on a connection. */
#define EXC_PAUSE 0x100000

#define EXC_PAUSE_READ 0x100001
#define EXC_PAUSE_START_READ 0x100002
#define EXC_PAUSE_CONNECTION 0x100003
#define EXC_PAUSE_START_CONNECTION 0x100004

#define EXC_RANDOMNESS 0x200000
#define EXC_RANDOMNESS_LOW_ENTROPY 0x200001

/* Application specific exceptions */
#define EXC_APP 0x400000

#define EXC_ALL (~0)

extern struct exception_handler default_exception_handler;
extern struct exception_handler ignore_exception_handler;
extern struct exception dummy_exception;

/* GABA:
   (class
     (name report_exception_info)
     (vars
       (mask . UINT32)
       (value . UINT32)
       (prefix . "const char *")))
*/

struct report_exception_info *
make_report_exception_info(UINT32 mask, UINT32 value,
			   const char *prefix);

#define STATIC_REPORT_EXCEPTION_INFO(m, v, p) \
{ STATIC_HEADER, m, v, p }

struct exception_handler *
make_report_exception_handler(struct report_exception_info *info,
			      struct exception_handler *parent,
			      const char *context);

struct exception *
make_simple_exception(UINT32 type, const char *msg);

/* Create a simple exception handler, with no internal state */
struct exception_handler *
make_exception_handler(void (*raise)(struct exception_handler *s,
				     const struct exception *x),
		       struct exception_handler *parent,
		       const char *context);

/* A protocol exception, that normally terminates the connection */
/* GABA:
   (class
     (name protocol_exception)
     (super exception)
     (vars
       ; A reason code that can be passed in a SSH_MSG_DISCONNECT message.
       ; Zero means terminate the connection without sending such a message.
       (reason . UINT32)))
*/


/* If msg is NULL, it is derived from the reason value */
struct exception *
make_protocol_exception(UINT32 reason, const char *msg);

#define STATIC_PROTOCOL_EXCEPTION(reason, msg) \
{ { STATIC_HEADER, EXC_PROTOCOL, (msg) }, (reason) }

#define PROTOCOL_ERROR(e, msg) \
EXCEPTION_RAISE((e), \
  make_protocol_exception(SSH_DISCONNECT_PROTOCOL_ERROR, (msg)))

#endif /* LSH_EXCEPTION_H_INCLUDED */

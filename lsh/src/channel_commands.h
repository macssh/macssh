/* channel_commands.h
 *
 * $Id$
 */

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

#ifndef LSH_CHANNEL_COMMANDS_H_INCLUDED
#define LSH_CHANNEL_COMMANDS_H_INCLUDED

#include "channel.h"

#define GABA_DECLARE
#include "channel_commands.h.x"
#undef GABA_DECLARE

/* Command to open a new channel. Takes a connection as argument
 * returns a new channel. Raises an EXC_CHANNEL_OPEN exception on
 * error. */

/* GABA:
   (class
     (name channel_open_command)
     (super command)
     (vars
       ; This method should return a partially filled in channel,
       ; and create a channel open request by calling
       ; prepare_channel_open.
       (new_channel method "struct ssh_channel *"
                    "struct ssh_connection *connection"
                    "UINT32 local_channel_number"
                    "struct lsh_string **request")))
*/

#define NEW_CHANNEL(s, c, n, r) ((s)->new_channel((s), (c), (n), (r)))

void do_channel_open_command(struct command *s,
			    struct lsh_object *x,
			    struct command_continuation *c,
			    struct exception_handler *e);

/* Takes a channel as argument, and returns the same channel. Raises
 * an exception on error. FIXME: Which exception??? */
/* GABA:
   (class
     (name channel_request_command)
     (super command)
     (vars
       ; This method should return a formatted request. The
       ; want_reply field in the request should be non_zero iff *c is
       ; non-NULL on return.  
       (format_request method "struct lsh_string *"
                       "struct ssh_channel *channel"
		       "struct command_continuation **c")))
*/

#define FORMAT_CHANNEL_REQUEST(r, c, w) \
((r)->format_request((r), (c), (w)))

void do_channel_request_command(struct command *s,
			       struct lsh_object *x,
			       struct command_continuation *c,
			       struct exception_handler *e);

/* GABA:
   (class
     (name global_request_command)
     (super command)
     (vars
       (format_request method "struct lsh_string *"
                       "struct ssh_connection *connection"
		       "struct command_continuation **c")))
*/

#define FORMAT_GLOBAL_REQUEST(r, conn, c) \
((r)->format_request((r), (conn), (c)))

/* For simple channel-related commands */
/* ;; GABA:
   (class
     (name channel_command)
     (super command)
     (vars
       (channel object ssh_channel)))
*/

void do_channel_global_command(struct command *s,
			       struct lsh_object *x,
			       struct command_continuation *c,
			       struct exception_handler *e);

struct command *
make_install_global_request_handler(UINT32 name,
				    struct global_request *handler);

/* Takes a connection and a handler as arguments, and installs the
 * handler. */
/* GABA:
   (class
     (name install_info)
     (super collect_info_2)
     (vars
       (name . int)))
*/

struct lsh_object *
do_install_global_request_handler(struct collect_info_2 *info,
				  struct lsh_object *a,
				  struct lsh_object *b);
struct lsh_object *
do_install_channel_open_handler(struct collect_info_2 *info,
				struct lsh_object *a,
				struct lsh_object *b);

#define STATIC_INSTALL_GLOBAL_HANDLER(atom) \
{ STATIC_COLLECT_2_FINAL(do_install_global_request_handler),(atom) }

#define STATIC_INSTALL_OPEN_HANDLER(atom) \
{ STATIC_COLLECT_2_FINAL(do_install_channel_open_handler),(atom) }

struct command *
make_install_fix_global_request_handler(UINT32 name,
					struct global_request *handler);

struct command *
make_install_fix_channel_open_handler(UINT32 name,
				      struct channel_open *handler);

#endif /* LSH_CHANNEL_COMMANDS_H_INCLUDED */

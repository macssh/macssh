/* server_session.h
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef LSH_SERVER_SESSION_H_INCLUDED
#define LSH_SERVER_SESSION_H_INCLUDED

#include "channel.h"
#include "io.h"
#include "server_userauth.h"

#include <assert.h>
#include <string.h>

struct ssh_channel *
make_server_session(UINT32 initial_window,
		    struct alist *request_types);
struct command *
make_server_connection_service(struct alist *session_requests);

struct channel_open *
make_open_session(struct alist *session_requests);

struct channel_request *
make_shell_handler(struct io_backend *backend);

struct channel_request *
make_exec_handler(struct io_backend *backend);

struct lsh_string *
format_exit_signal(struct ssh_channel *channel,
		   int core, int signal);
struct lsh_string *
format_exit(struct ssh_channel *channel, int value);

extern struct channel_request 
pty_request_handler;

#endif /* LSH_SERVER_SESSION_H_INCLUDED */

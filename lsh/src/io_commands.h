/* io_commands.h
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

#ifndef LSH_IO_COMMANDS_H_INCLUDED
#define LSH_IO_COMMANDS_H_INCLUDED

#include "command.h"
#include "connection.h"
#include "io.h"

#define GABA_DECLARE
#include "io_commands.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name io_write_file_info)
     (vars
       (name . "const char *")
       (flags . int)
       (mode . int)
       (block_size . UINT32)))
*/

struct io_write_file_info *
make_io_write_file_info(const char *name, int flags, int mode, UINT32 block_size);

extern struct command io_write_file_command;

#define IO_WRITE_FILE (&io_write_file_command.super)

/* Read a certain fd */

/* GABA:
   (class
     (name io_read_fd)
     (super command)
     (vars
       (fd . int)))
*/

void do_io_read_fd(struct command *s,
		   struct lsh_object *a,
		   struct command_continuation *c,
		   struct exception_handler *e);

#define STATIC_IO_READ_FD(fd) \
{ STATIC_COMMAND(do_io_read_fd), fd }

extern struct io_read_fd io_read_stdin;
#define IO_READ_STDIN (&io_read_stdin.super.super)


struct command *
make_listen_with_callback(struct command *callback,
			  struct io_backend *backend);

extern struct command_3 listen_with_callback;
#define LISTEN_CALLBACK (&listen_with_callback.super.super)

struct command *
make_connect_port(struct io_backend *backend,
				  struct address_info *target);

struct command *
make_connect_connection(struct io_backend *backend);

struct command *
make_simple_connect(struct io_backend *backend,
		    struct resource_list *resources);

struct command *
make_listen_local(struct io_backend *backend,
		  struct local_info *info);

struct command *
make_connect_local(struct io_backend *backend);

extern struct command connect_local_command;
#define CONNECT_LOCAL (&connect_local_command.super)

extern struct command io_log_peer_command;
#define LOG_PEER (&io_log_peer_command.super)

#endif /* LSH_IO_COMMANDS_H_INCLUDED */

/* tcpforward_commands.h
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Balázs Scheidler, Niels Möller
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

#ifndef LSH_TCPFORWARD_COMMANDS_H_INCLUDED
#define LSH_TCPFORWARD_COMMANDS_H_INCLUDED

#include "tcpforward.h"

struct command *
make_forward_local_port(struct io_backend *backend,
			struct address_info *local,
			struct address_info *target);

struct command *
make_forward_remote_port(struct io_backend *backend,
			 struct address_info *local,
			 struct address_info *target);

struct command *
make_direct_tcpip_hook(struct io_backend *backend);

struct command *
make_tcpip_forward_hook(struct io_backend *backend);

#endif /* LSH_TCPFORWARD_COMMANDS_H_INCLUDED */

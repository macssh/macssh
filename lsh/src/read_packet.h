/* read_packet.h
 *
 * Read-handler to read a packet at a time.
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

#ifndef LSH_READ_PACKET_H_INCLUDED
#define LSH_READ_PACKET_H_INCLUDED

#include "abstract_io.h"
#include "abstract_crypto.h"
#include "connection.h"

struct read_handler *make_read_packet(struct abstract_write *handler,
				      struct ssh_connection *connection);

#endif /* LSH_READ_PACKET_H_INCLUDED */

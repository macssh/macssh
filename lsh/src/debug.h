/* debug.h
 *
 * Packet processor dumping packets to a file.
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

#ifndef LSH_DEBUG_H_INCLUDED
#define LSH_DEBUG_H_INCLUDED

#include <stdio.h>
#include "abstract_io.h"
#include "connection.h"

struct abstract_write *
make_packet_debug(struct abstract_write *continuation,
		  struct lsh_string *prefix);

void send_debug_message(struct abstract_write *write, const char *msg, int always_display);
void send_debug(struct abstract_write *write, const char *msg, int always_display);
void send_verbose(struct abstract_write *write, const char *msg, int always_display);

extern struct packet_handler connection_debug_handler;

#endif /* LSH_DEBUG_H_INCLUDED */

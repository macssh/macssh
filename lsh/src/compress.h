/* compress.h
 *
 * header for compression algorithms
 * 
 * $Id$ */

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

#ifndef LSH_COMPRESS_H_INCLUDED
#define LSH_COMPRESS_H_INCLUDED

#include "abstract_io.h"
#include "abstract_compress.h"

#include "connection.h"

struct compress_algorithm *make_zlib_algorithm(int level);
struct compress_algorithm *make_zlib(void);

struct abstract_write *make_packet_codec(struct abstract_write *next,
					 struct ssh_connection *connection,
					 int mode);

#define make_packet_deflate(next, connection) \
 make_packet_codec(next, connection, COMPRESS_DEFLATE)
#define make_packet_inflate(next, connection) \
 make_packet_codec(next, connection, COMPRESS_INFLATE)

#endif /* LSH_COMPRESS_H_INCLUDED */


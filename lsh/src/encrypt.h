/* encrypt.h
 *
 * Handler to encrypt and authenticate packets.
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

#ifndef LSH_ENCRYPT_H_INCLUDED
#define LSH_ENCRYPT_H_INCLUDED

#include "abstract_io.h"
#include "abstract_crypto.h"
#include "connection.h"

struct abstract_write *
make_packet_encrypt(struct abstract_write *continuation,
		    struct ssh_connection *connection);		       
		    
#endif /* LSH_ENCRYPT_H_INCLUDED */

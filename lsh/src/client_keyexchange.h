/* client_keyexchange.h
 *
 * Client specific key exchange handling
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

#ifndef LSH_CLIENT_KEYEXCHANGE_H_INCLUDED
#define LSH_CLIENT_KEYEXCHANGE_H_INCLUDED

#include "interact.h"
#include "keyexchange.h"
#include "publickey_crypto.h"

struct keyexchange_algorithm *
make_dh_client(struct dh_method *dh);

struct keyexchange_algorithm *
make_srp_client(struct dh_method *dh,
		struct interact *tty, struct lsh_string *name);

#endif /* LSH_CLIENT_KEYEXCHANGE_H_INCLUDED */

/* server_authorization.c
 *
 * user authorization database
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Balázs Scheidler
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

#ifndef LSH_SERVER_AUTHORIZATION_H_INCLUDED
#define LSH_SERVER_AUTHORIZATION_H_INCLUDED

#include "lookup_verifier.h"
#include "publickey_crypto.h"

struct lookup_verifier *
make_authorization_db(struct lsh_string *index_name, 
		      /* struct signature_algorithm *s, */
		      struct hash_algorithm *h);

#endif /* LSH_SERVER_AUTHORIZATION_H_INCLUDED */

/* client_userauth.h
 *
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

#ifndef LSH_CLIENT_USERAUTH_H_INCLUDED
#define LSH_CLIENT_USERAUTH_H_INCLUDED

#include "alist.h"
#include "command.h"
#include "connection.h"
#include "interact.h"
#include "list.h"
#include "parse.h"
#include "userauth.h"

/* Client functions */
struct lsh_string *
format_userauth_password(struct lsh_string *name,
			 int service,
			 struct lsh_string *passwd,
			 int free);


/* Consumes the username */
struct command *make_client_userauth(struct lsh_string *username,
				     int service_name,
				     struct object_list *methods);

struct client_userauth_method *
make_client_password_auth(struct interact *tty);

struct client_userauth_method *
make_client_publickey_auth(struct object_list *);

struct client_userauth_method *
make_client_none_auth(void);

#endif /* LSH_CLIENT_USERAUTH_H_INCLUDED */


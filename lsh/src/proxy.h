/* proxy.h
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef LSH_PROXY_H_INCLUDED
#define LSH_PROXY_H_INCLUDED

#include "alist.h"
#include "command.h"

extern struct command_3 chain_connections;
#define CHAIN_CONNECTIONS (&chain_connections.super.super)

struct command *
make_proxy_connection_service(struct object_list *server_hooks,
			      struct object_list *client_hooks);


struct command *
make_proxy_offer_service(struct alist *services);

struct channel_open *
make_proxy_open_direct_tcpip(void);

struct channel_open *
make_proxy_open_forwarded_tcpip(void);



#endif /* LSH_PROXY_H_INCLUDED */

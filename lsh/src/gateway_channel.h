/* gateway_channel.h
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999, 2000 Niels Möller, Balázs Scheidler
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

#ifndef LSH_GATEWAY_CHANNEL_H_INCLUDED
#define LSH_GATEWAY_CHANNEL_H_INCLUDED

#include "channel.h"
#include "command.h"

#define GABA_DECLARE
#include "gateway_channel.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name gateway_channel)
     (super ssh_channel)
     (vars
       (chain object gateway_channel)))
*/

struct gateway_channel *
make_gateway_channel(struct alist *request_types);

struct command *
make_gateway_channel_open_command(struct channel_open_info *info,
				  struct lsh_string *args,
				  struct alist *requests);

struct command_continuation *
make_gateway_channel_open_continuation(struct command_continuation *up,
				       struct channel_request *fallback,
				       struct gateway_channel *origin);

extern struct channel_request gateway_channel_request;
extern struct global_request gateway_global_request;

extern struct channel_open gateway_channel_open_forward;

#endif /* LSH_GATEWAY_CHANNEL_H_INCLUDED */

 

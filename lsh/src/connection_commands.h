/* connection_commands.h
 *
 * $Id$
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef LSH_CONNECTION_COMMANDS_H_INCLUDED
#define LSH_CONNECTION_COMMANDS_H_INCLUDED

#include "command.h"

#define GABA_DECLARE
#include "connection_commands.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name connection_command)
     (super command)
     (vars
       (connection object ssh_connection)))
*/

extern struct command_2 connection_remember;
#define CONNECTION_REMEMBER (&connection_remember.super.super)

struct command *
make_connection_if_srp(struct command *then_f,
		       struct command *else_f);

extern struct command_3 connection_if_srp;
#define CONNECTION_IF_SRP (&connection_if_srp_command.super.super)

extern struct command connection_require_userauth;
#define CONNECTION_REQUIRE_USERAUTH (&connection_require_userauth.super)

#endif /* LSH_CONNECTION_COMMANDS_H_INCLUDED */

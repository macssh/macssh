/* gateway_commands.h
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
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

#ifndef GATEWAY_COMMANDS_H_INCLUDED
#define GATEWAY_COMMANDS_H_INCLUDED

#include "command.h"

extern struct command gateway_init;
#define GATEWAY_INIT (&gateway_init.super)

extern struct command_simple gateway_accept;
#define GATEWAY_ACCEPT (&gateway_accept.super.super)

struct command *
make_gateway_setup(struct command *listen);

extern struct command_simple gateway_setup_command;
#define GATEWAY_SETUP (&gateway_setup_command.super.super)

#endif /* GATEWAY_COMMANDS_H_INCLUDED */

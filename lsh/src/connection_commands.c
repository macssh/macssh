/* connection_commands.c
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

#include "connection_commands.h"

#include "connection.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "connection_commands.h.x"
#undef GABA_DEFINE

/* (remember connection resource) */
DEFINE_COMMAND2(connection_remember)
     (struct command_2 *s UNUSED,
      struct lsh_object *a1,
      struct lsh_object *a2,
      struct command_continuation *c,
      struct exception_handler *e UNUSED)
{
  CAST(ssh_connection, connection, a1);
  CAST_SUBTYPE(resource, resource, a2);

  if (resource)
    REMEMBER_RESOURCE(connection->resources, resource);

  COMMAND_RETURN(c, resource);
}


DEFINE_COMMAND(connection_require_userauth)
     (struct command *s UNUSED, struct lsh_object *a,
      struct command_continuation *c,
      struct exception_handler *e UNUSED)
{
  CAST(ssh_connection, connection, a);

  if (connection->user)
    COMMAND_RETURN(c, connection);
  else
    EXCEPTION_RAISE(connection->e,
		    make_protocol_exception(SSH_DISCONNECT_SERVICE_NOT_AVAILABLE,
					    "Access denied."));
}

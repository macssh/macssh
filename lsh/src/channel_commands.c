/* channel_commands.c
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

#include "channel_commands.h"

#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#define GABA_DEFINE
#include "channel_commands.h.x"
#undef GABA_DEFINE

#include "channel_commands.c.x"

#include <assert.h>

void
do_channel_open_command(struct command *s,
			struct lsh_object *x,
			struct command_continuation *c,
			struct exception_handler *e)
{
  CAST_SUBTYPE(channel_open_command, self, s);
  CAST(ssh_connection, connection, x);
  struct lsh_string *request;
  struct ssh_channel *channel;

  int index = alloc_channel(connection->table);

  if (index < 0)
    {
      /* We have run out of channel numbers. */
      werror("do_channel_open_command: alloc_channel failed\n");
      EXCEPTION_RAISE(e,
		      make_channel_open_exception(
			SSH_OPEN_RESOURCE_SHORTAGE,
			"Allocating a local channel number failed."));
    }

  channel = NEW_CHANNEL(self, connection, index, &request);

  if (!channel)
    {
      werror("do_channel_open_command: NEW_CHANNEL failed\n");
      dealloc_channel(connection->table, index);
    }
  else
    {
      /* FIXME: Set up channel->write here? If we do that, perhaps we
       * need not pass the connection to NEW_CHANNEL. */
      channel->open_continuation = c;
      channel->e = e;
      register_channel(connection, index, channel, 0);

      C_WRITE(connection, request);
    }
}

void
do_channel_request_command(struct command *s,
			   struct lsh_object *x,
			   struct command_continuation *c,
			   struct exception_handler *e)
{
  CAST_SUBTYPE(channel_request_command, self, s);
  CAST_SUBTYPE(ssh_channel, channel, x);

  struct lsh_string *request
    = FORMAT_CHANNEL_REQUEST(self, channel, &c);

  if (CONTINUATION_USED_P(c))
    object_queue_add_tail(&channel->pending_requests,
			  &make_command_context(c, e)->super);
  
  A_WRITE(channel->write, request);
}

void
do_channel_global_command(struct command *s,
			  struct lsh_object *x,
			  struct command_continuation *c,
			  struct exception_handler *e)
{
  CAST_SUBTYPE(global_request_command, self, s);
  CAST_SUBTYPE(ssh_connection, connection, x);

  struct lsh_string *request
    = FORMAT_GLOBAL_REQUEST(self, connection, &c);

  if (CONTINUATION_USED_P(c))
    object_queue_add_tail(&connection->table->pending_global_requests,
			  &make_command_context(c, e)->super);

  C_WRITE(connection, request);
}

struct lsh_object *
do_install_global_request_handler(struct collect_info_2 *info,
				  struct lsh_object *a,
				  struct lsh_object *b)
{
  CAST(install_info, self, info);
  CAST(ssh_connection, connection, a);
  CAST_SUBTYPE(global_request, handler, b);

  assert(!info->next);
  assert(handler);

  trace("Installing global request handler for '%a'\n", self->name);
  
  ALIST_SET(connection->table->global_requests,
	    self->name,
	    &handler->super);

  return a;
}

struct lsh_object *
do_install_channel_open_handler(struct collect_info_2 *info,
				struct lsh_object *a,
				struct lsh_object *b)
{
  CAST(install_info, self, info);
  CAST(ssh_connection, connection, a);
  CAST_SUBTYPE(channel_open, handler, b);

  assert(!info->next);
  assert(handler);
  
  trace("Installing channel open handler for '%a'\n", self->name);

  ALIST_SET(connection->table->channel_types,
	    self->name,
	    &handler->super);

  return a;
}

/* Special cases, when the handler is known early */
 
/* Takes a connection as argument, and installs a fix handler */
/* GABA:
   (class
     (name install_global_request_handler)
     (super command)
     (vars
       (name . int)
       (handler object global_request)))
*/

static void
do_install_fix_global_request_handler(struct command *s,
				      struct lsh_object *x,
				      struct command_continuation *c,
				      struct exception_handler *e UNUSED)
{
  CAST(install_global_request_handler, self, s);
  CAST(ssh_connection, connection, x);

  trace("Installing fix global request handler for '%a'\n", self->name);
  
  ALIST_SET(connection->table->global_requests,
	    self->name,
	    &self->handler->super);

  COMMAND_RETURN(c, x);
}

struct command *
make_install_fix_global_request_handler(UINT32 name,
					struct global_request *handler)
{  
  NEW(install_global_request_handler, self);

  assert(handler);
  
  self->super.call = do_install_fix_global_request_handler;
  self->name = name;
  self->handler = handler;

  return &self->super;
}

/* Takes a connection as argument, and installs a fix handler */
/* GABA:
   (class
     (name install_channel_open_handler)
     (super command)
     (vars
       (name . int)
       (handler object channel_open)))
*/

static void
do_install_fix_channel_open_handler(struct command *s,
				    struct lsh_object *x,
				    struct command_continuation *c,
				    struct exception_handler *e UNUSED)
{
  CAST(install_channel_open_handler, self, s);
  CAST(ssh_connection, connection, x);

  trace("Installing fix channel open handler for type '%a'\n", self->name);
  
  ALIST_SET(connection->table
	    ->channel_types,
	    self->name,
	    &self->handler->super);

  COMMAND_RETURN(c, connection);
}

struct command *
make_install_fix_channel_open_handler(UINT32 name,
				      struct channel_open *handler)
{
  NEW(install_channel_open_handler, self);

  assert(handler);
  
  self->super.call = do_install_fix_channel_open_handler;
  self->name = name;
  self->handler = handler;

  return &self->super;
}


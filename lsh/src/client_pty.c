/* client_pty.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, Niels Möller, Balázs Scheidler
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

#include "client.h"

#include "channel_commands.h"
#include "format.h"
#include "interact.h"
#include "tty.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "client_pty.c.x"

/* GABA:
   (class
     (name pty_request)
     (super channel_request_command)
     (vars
       (tty object interact)
       (term string)
       (attr object terminal_attributes)
       (dims . "struct terminal_dimensions")))
*/

/* GABA:
   (class
     (name client_tty_resource)
     (super resource)
     (vars
       (tty object interact)
       (attr object terminal_attributes)))
*/

static void
do_kill_client_tty_resource(struct resource *s)
{
  CAST(client_tty_resource, self, s);
  self->super.alive = 0;
  INTERACT_SET_ATTRIBUTES(self->tty, self->attr);
}

static struct resource *
make_client_tty_resource(struct interact *tty,
			 struct terminal_attributes *attr)
{
  NEW(client_tty_resource, self);
  init_resource(&self->super, do_kill_client_tty_resource);

  self->tty = tty;
  self->attr = attr;

  return &self->super;
}

/* GABA:
   (class
     (name client_winch_handler)
     (super window_change_callback)
     (vars
       (channel object ssh_channel)))
*/

static struct lsh_string *
format_window_change(struct ssh_channel *channel,
		     struct terminal_dimensions *dims)
{
  return format_channel_request
    (ATOM_WINDOW_CHANGE, channel,
     0, "%i%i%i%i",
     dims->char_width, dims->char_height,
     dims->pixel_width, dims->pixel_height); 
}

static void
do_client_winch_handler(struct window_change_callback *s,
			struct interact *tty)
{
  CAST(client_winch_handler, self, s);
  struct terminal_dimensions dims;

  if (!INTERACT_WINDOW_SIZE(tty, &dims))
    return;

  C_WRITE(self->channel->connection,
	  format_window_change(self->channel, &dims));
}

static struct window_change_callback *
make_client_winch_handler(struct ssh_channel *channel)
{
  NEW(client_winch_handler, self);
  self->super.f = do_client_winch_handler;
  self->channel = channel;

  return &self->super;
}

       
/* GABA:
   (class
     (name pty_request_continuation)
     (super command_frame)
     (vars
       (req object pty_request)))
*/

/* FIXME: !!! failed requests show up as an exception. /Bazsi
 *
 * I think that is normal. It's up to the caller to do something reasonable
 * about the exception. /nisse
 */
static void
do_pty_continuation(struct command_continuation *s,
		    struct lsh_object *x)
{
  CAST(pty_request_continuation, self, s);
  CAST_SUBTYPE(ssh_channel, channel, x);
  struct terminal_attributes *raw;
  
  assert(x);
  verbose("pty request succeeded\n");
  
  raw = TERM_MAKE_RAW(self->req->attr);
  if (!INTERACT_SET_ATTRIBUTES(self->req->tty, raw))
    {
      werror("do_pty_continuation: "
	     "Setting the attributes of the local terminal failed.\n");
    }

  REMEMBER_RESOURCE(channel->resources,
		    make_client_tty_resource(self->req->tty,
					     self->req->attr));
  
  REMEMBER_RESOURCE(channel->resources,
		    INTERACT_WINDOW_SUBSCRIBE
		    (self->req->tty,
		     make_client_winch_handler(channel)));
  
  COMMAND_RETURN(self->super.up, x);
}

static struct command_continuation *
make_pty_continuation(struct pty_request *req,
		      struct command_continuation *c)
{
  NEW(pty_request_continuation, self);
  self->req = req;
  self->super.up = c;
  self->super.super.c = do_pty_continuation;
  
  return &self->super.super;
}

static struct lsh_string *
do_format_pty_request(struct channel_request_command *s,
		      struct ssh_channel *channel,
		      struct command_continuation **c)
{
  CAST(pty_request, self, s);

  verbose("Requesting a remote pty.\n");

  *c = make_pty_continuation(self, *c);

  return format_channel_request
    (ATOM_PTY_REQ, channel, 1,
     "%S%i%i%i%i%fS",
     self->term,
     self->dims.char_width, self->dims.char_height,
     self->dims.pixel_width, self->dims.pixel_height,
     TERM_ENCODE(self->attr));
}

struct command *
make_pty_request(struct interact *tty)
{
  NEW(pty_request, req);
  char *term = getenv("TERM");

  req->attr = INTERACT_GET_ATTRIBUTES(tty);

  if (!req->attr)
    {
      KILL(req);
      return NULL;
    }
  
  if (!INTERACT_WINDOW_SIZE(tty, &req->dims))
    req->dims.char_width = req->dims.char_height
      = req->dims.pixel_width = req->dims.pixel_height = 0;

  req->super.super.call = do_channel_request_command;
  req->super.format_request = do_format_pty_request;
  
  req->tty = tty;
  req->term = term ? make_string(term) : ssh_format("");

  return &req->super.super;
}

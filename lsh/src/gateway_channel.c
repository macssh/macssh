/* gateway_channel.c
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

#include "gateway_channel.h"

#include "channel_commands.h"
#include "format.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#define GABA_DEFINE
#include "gateway_channel.h.x"
#undef GABA_DEFINE

#include "gateway_channel.c.x"

/* A pair of gateway_channel objects are chained together so that
 * requests and data received on one of the channels are directed to
 * the other.
 *
 * Chaining happens as follows:
 *
 * 1. First a CHANNEL_OPEN request is received on one connection, and
 *    a channel object is created. We refer to this object as the
 *    _originating_ channel.
 *
 * 2. Next, we send a similar CHANNEL_OPEN request on some other
 *    connection, and create a channel object referred to as the
 *    _target_ channel.
 *
 * 3. When we receive a reply to the CHANNEL_OPEN request sent in (2),
 *    we chain the two channel objects together, and reply to the
 *    CHANNEL_OPEN request we received in (1). */

/* FIXME: A gateway may want to handle unknown types of channel data. */
static void
do_receive(struct ssh_channel *c,
	   int type,
	   struct lsh_string *data)
{
  CAST(gateway_channel, channel, c);
  
  switch(type)
    {
    case CHANNEL_DATA:
      C_WRITE(channel->chain->super.connection,
	      channel_transmit_data(&channel->chain->super, data));
      break;
    case CHANNEL_STDERR_DATA:
      C_WRITE(channel->chain->super.connection,
	      channel_transmit_extended(&channel->chain->super, CHANNEL_STDERR_DATA, data));
      break;
    default:
      fatal("Internal error!\n");
    }
}

/* We may send more data */
static void
do_send_adjust(struct ssh_channel *s,
	       UINT32 i)
{
  CAST(gateway_channel, self, s);
  FLOW_CONTROL_REPORT(&self->chain->super.super, i);
}

static void
do_eof(struct ssh_channel *c)
{
  CAST(gateway_channel, channel, c);
  channel_eof(&channel->chain->super);
}

static void
do_close(struct ssh_channel *c)
{
  CAST(gateway_channel, channel, c);  
  channel_close(&channel->chain->super);
}

static void
gateway_init_io(struct gateway_channel *channel)
{
  channel->super.send_adjust = do_send_adjust;  
  channel->super.receive = do_receive;
  channel->super.eof = do_eof;
  channel->super.close = do_close;
}

/* NOTE: We don't initialize the rec_window_size and rec_max_packet fields here.
 *
 * The origin's rec_window_size and rec_max_packet are filled in
 * later, by do_gateway_channel_open_continuation. The target's
 * rec_window_size, on the other hand, must be filled in manually. */

struct gateway_channel *
make_gateway_channel(struct alist *request_types)
{
  NEW(gateway_channel, self);
  init_channel(&self->super);
  
  self->super.request_types = request_types;
  
  /* Never initiate close; let each end point decide when it is time
   * to send SSH_MSG_CHANNEL_CLOSE. */
  self->super.flags &= ~CHANNEL_CLOSE_AT_EOF;

  return self;
}


/* Command to request a channel open. Typically used after we have
 * received a CHANNEL_OPEN request from the originating end, to create
 * a channel to the target. */
/* GABA:
   (class
     (name gateway_channel_open_command)
     (super channel_open_command)
     (vars
       ; The channel type is represented as a string, as
       ; we should be able to forward unknown channel
       ; types (i.e. types not listen in atoms.in).
       (type string)
       (rec_window_size . UINT32)
       (rec_max_packet . UINT32)
       (requests object alist)
       (args string)))
*/

static struct ssh_channel *
do_gateway_channel_open(struct channel_open_command *c,
			struct ssh_connection *connection,
			UINT32 local_channel_number,
			struct lsh_string **request)
{
  CAST(gateway_channel_open_command, closure, c);
  
  struct gateway_channel *target
    = make_gateway_channel(closure->requests);

  target->super.rec_window_size = closure->rec_window_size;
  target->super.rec_max_packet = closure->rec_max_packet;
  target->super.connection = connection;

  *request = format_channel_open_s(closure->type,
				   local_channel_number,
				   &target->super,
				   closure->args);
  
  return &target->super;
}

struct command *
make_gateway_channel_open_command(struct channel_open_info *info,
				  struct lsh_string *args,
				  struct alist *requests)
{
  NEW(gateway_channel_open_command, self);
  
  self->super.new_channel = do_gateway_channel_open;
  self->super.super.call = do_channel_open_command;

  self->type = ssh_format("%ls", info->type_length,
			  info->type_data);

  self->rec_window_size = info->send_window_size;
  self->rec_max_packet = info->send_max_packet;
  self->requests = requests;
  self->args = args;

  return &self->super.super;
}


/* General channel requests */

/* GABA:
   (class
     (name general_channel_request_command)
     (super channel_request_command)
     (vars
       ; NOTE: This is only used once.
       (request string)))
*/

static struct lsh_string *
do_format_channel_general(struct channel_request_command *s,
			  struct ssh_channel *ch UNUSED,
			  struct command_continuation **c UNUSED)
{
  CAST(general_channel_request_command, self, s);

  struct lsh_string *r = self->request;
  self->request = NULL;
  return r;
}

static struct command *
make_general_channel_request_command(struct lsh_string *request)
{
  NEW(general_channel_request_command, self);
  self->super.super.call = do_channel_request_command;
  self->super.format_request = do_format_channel_general;
  self->request = request;
  return &self->super.super;
}

static void 
do_gateway_channel_request(struct channel_request *s UNUSED,
			   struct ssh_channel *ch,
			   struct channel_request_info *info,
			   struct simple_buffer *args,
			   struct command_continuation *c,
			   struct exception_handler *e)
{
  CAST(gateway_channel, channel, ch);
  UINT32 args_length;
  const UINT8 *args_data;
  struct command *command;
  
  parse_rest(args, &args_length, &args_data);

  command = make_general_channel_request_command
    (format_channel_request_i(info, &channel->chain->super,
			      args_length, args_data));
  COMMAND_CALL(command, channel->chain, c, e);
}

struct channel_request gateway_channel_request =
{ STATIC_HEADER, do_gateway_channel_request };


/* GABA:
   (class
     (name general_global_request_command)
     (super global_request_command)
     (vars
       (request string)))
*/

static struct lsh_string *
do_format_general_global_request(struct global_request_command *s,
			  	 struct ssh_connection *connection UNUSED,
				 struct command_continuation **c UNUSED)
{
  CAST(general_global_request_command, self, s);

  struct lsh_string *r = self->request;
  self->request = NULL;
  return r;
}

static struct command *
make_general_global_request_command(struct lsh_string *request)
{
  NEW(general_global_request_command, self);
  
  self->super.super.call = do_channel_global_command;
  self->super.format_request = do_format_general_global_request;
  self->request = request;
  return &self->super.super;
}

static void
do_gateway_global_request(struct global_request *s UNUSED,
			  struct ssh_connection *connection,
			  UINT32 type,
			  int want_reply,
			  struct simple_buffer *args,
			  struct command_continuation *c,
			  struct exception_handler *e)
{
  struct lsh_string *request =
    format_global_request(type, want_reply, "%ls",
			  args->capacity - args->pos, &args->data[args->pos]);

  struct command *send = make_general_global_request_command(request);

  COMMAND_CALL(send, connection->chain, c, e);
}

struct global_request gateway_global_request = 
{ STATIC_HEADER, do_gateway_global_request };

/* Continuation to handle the returned channel, and chain two channels
 * together.
 *
 * CHANNEL is the "originating" end of the channel. The argument to
 * the continuation is a new channel connected to the other end. */

/* GABA:
   (class
     (name gateway_channel_open_continuation)
     (super command_continuation)
     (vars
       (up object command_continuation)
       (fallback object channel_request fallback)
       (origin object gateway_channel)))
*/

static void
do_gateway_channel_open_continuation(struct command_continuation *c,
				     struct lsh_object *x)
{
  CAST(gateway_channel_open_continuation, self, c);
  CAST(gateway_channel, target, x);

  self->origin->chain = target;
  target->chain = self->origin;

  self->origin->super.rec_window_size = target->super.send_window_size;
  self->origin->super.rec_max_packet = target->super.send_max_packet;

  gateway_init_io(self->origin);
  gateway_init_io(target);

  target->super.request_fallback = self->fallback;
  
  COMMAND_RETURN(self->up, self->origin);
}

struct command_continuation *
make_gateway_channel_open_continuation(struct command_continuation *up,
				       struct channel_request *fallback,
				       struct gateway_channel *origin)
{
  NEW(gateway_channel_open_continuation, self);
  
  self->super.c = do_gateway_channel_open_continuation;
  self->origin = origin;
  self->fallback = fallback;
  self->up = up;

  return &self->super;
}

static void
do_channel_open_forward(struct channel_open *s UNUSED,
			struct ssh_connection *connection,
			struct channel_open_info *info,
			struct simple_buffer *args,
			struct command_continuation *c,
			struct exception_handler *e)
{
  struct gateway_channel *origin
    = make_gateway_channel(NULL);

  struct command *command
    = make_gateway_channel_open_command(info, parse_rest_copy(args), NULL);
  
  origin->super.request_fallback = &gateway_channel_request;
  
  COMMAND_CALL(command,
	       connection->chain,
	       make_gateway_channel_open_continuation
	         (c, &gateway_channel_request, origin),
	       e);
}

struct channel_open gateway_channel_open_forward =
{ STATIC_HEADER, do_channel_open_forward };


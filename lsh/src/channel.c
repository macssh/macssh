/* channel.c
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

#include "channel.h"

#include "format.h"
#include "io.h"
#include "read_data.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

#define GABA_DEFINE
#include "channel.h.x"
#undef GABA_DEFINE

#include "channel.c.x"

struct exception *
make_channel_open_exception(UINT32 error_code, const char *msg)
{
  NEW(channel_open_exception, self);

#define MAX_ERROR 4
  static const char *msgs[MAX_ERROR + 1] = {
    "",
    "Administratively prohibited",
    "Connect failed",
    "Unknown channel type",
    "Resource shortage"
  };

  assert(error_code > 0);
  assert(error_code <= MAX_ERROR);
#undef MAX_ERROR
  
  self->super.type = EXC_CHANNEL_OPEN;
  self->super.msg = msg ? msg : msgs[error_code];
  self->error_code = error_code;

  return &self->super;
}

/* GABA:
   (class
     (name connection_service)
     (super command)
     (vars
       ; Supported global requests 
       (global_requests object alist)

       (channel_types object alist) ))

       ; Initialize connection (for instance, request channels to be 
       ; opened or services to be forwarded).

       ; (start object connection_startup)))
*/


struct lsh_string *
format_global_failure(void)
{
  return ssh_format("%c", SSH_MSG_REQUEST_FAILURE);
}

struct lsh_string *
format_global_success(void)
{
  return ssh_format("%c", SSH_MSG_REQUEST_SUCCESS);
}

struct lsh_string *
format_open_confirmation(struct ssh_channel *channel,
			 UINT32 channel_number,
			 const char *format, ...)
{
  va_list args;
  UINT32 l1, l2;
  struct lsh_string *packet;

#define CONFIRM_FORMAT "%c%i%i%i%i"
#define CONFIRM_ARGS \
  SSH_MSG_CHANNEL_OPEN_CONFIRMATION, channel->channel_number, \
  channel_number, channel->rec_window_size, channel->rec_max_packet
    
  debug("format_open_confirmation: rec_window_size = %i,\n"
	"                          rec_max_packet = %i,\n",
       channel->rec_window_size,
       channel->rec_max_packet);
  l1 = ssh_format_length(CONFIRM_FORMAT, CONFIRM_ARGS);

  va_start(args, format);
  l2 = ssh_vformat_length(format, args);
  va_end(args);

  packet = lsh_string_alloc(l1 + l2);

  ssh_format_write(CONFIRM_FORMAT, l1, packet->data, CONFIRM_ARGS);

  va_start(args, format);
  ssh_vformat_write(format, l2, packet->data+l1, args);
  va_end(args);

  return packet;
#undef CONFIRM_FORMAT
#undef CONFIRM_ARGS
}

struct lsh_string *
format_open_failure(UINT32 channel, UINT32 reason,
		    const char *msg, const char *language)
{
  return ssh_format("%c%i%i%z%z", SSH_MSG_CHANNEL_OPEN_FAILURE,
		    channel, reason, msg, language);
}

struct lsh_string *
format_channel_success(UINT32 channel)
{
  return ssh_format("%c%i", SSH_MSG_CHANNEL_SUCCESS, channel);
}

struct lsh_string *
format_channel_failure(UINT32 channel)
{
  return ssh_format("%c%i", SSH_MSG_CHANNEL_FAILURE, channel);
}

struct lsh_string *
prepare_window_adjust(struct ssh_channel *channel,
		      UINT32 add)
{
  channel->rec_window_size += add;
  
  return ssh_format("%c%i%i",
		    SSH_MSG_CHANNEL_WINDOW_ADJUST,
		    channel->channel_number, add);
}

/* GABA:
   (class
     (name exc_finish_channel_handler)
     (super exception_handler)
     (vars
       (connection object ssh_connection)
       ; Non-zero if the channel has already been deallocated.
       (dead . int)
       ; Local channel number 
       (channel_number . UINT32)))
*/

static void
do_exc_finish_channel_handler(struct exception_handler *s,
			      const struct exception *e)
{
  CAST(exc_finish_channel_handler, self, s);

  switch (e->type)
    {
    case EXC_FINISH_PENDING:
      if (self->dead)
	werror("channel.c: EXC_FINISH_PENDING on dead channel.\n");

      self->connection->table->pending_close = 1;

      /* NOTE: We don't need to raise a EXC_FINISH_READ here. Only
       * code in a live channel is supposed to raise
       * EXC_FINISH_PENDING. The typical caller is a channel's
       * CHANNEL_CLOSE callback that is called below. */
#if 0
      if (!self->connection->table->channel_count)
	EXCEPTION_RAISE(self->connection->e, &finish_read_exception);
#endif
      break;
      
    case EXC_FINISH_CHANNEL:
      /* NOTE: This type of exception must be handled only once.
       * However, there is at least one case where it is difficult to
       * ensure that the exception is raised only once.
       *
       * For instance, in do_channel_close, the CHANNEL_EOF callback
       * can decide to call close_channel, which might raise this
       * exception. When control gets back to do_channel_close, and
       * CHANNEL_SENT_CLOSE is true, it raises the exception again.
       *
       * To get this right, we set a flag when the channel is
       * deallocated. */
      if (self->dead)
	debug("EXC_FINISH_CHANNEL on dead channel.\n");
      else
	{
	  struct ssh_channel *channel
	    = self->connection->table->channels[self->channel_number];

	  assert(channel);
	  assert(channel->resources->super.alive);

	  if (channel->close)
	    CHANNEL_CLOSE(channel);
	
	  KILL_RESOURCE_LIST(channel->resources);
	
	  dealloc_channel(self->connection->table, self->channel_number);
	  self->dead = 1;

	  if (self->connection->table->pending_close &&
	      !self->connection->table->channel_count)
	    {
	      /* FIXME: Send a SSH_DISCONNECT_BY_APPLICATION message? */
	      EXCEPTION_RAISE(self->connection->e, &finish_read_exception);
	    }
	}
      break;
    default:
      EXCEPTION_RAISE(self->super.parent, e);
    }
}

static struct exception_handler *
make_exc_finish_channel_handler(struct ssh_connection *connection,
				UINT32 channel_number,
				struct exception_handler *e,
				const char *context)
{
  NEW(exc_finish_channel_handler, self);
  self->super.parent = e;
  self->super.raise = do_exc_finish_channel_handler;
  self->super.context = context;

  self->connection = connection;
  self->channel_number = channel_number;
  self->dead = 0;
  
  return &self->super;
}
				

/* Channel objects */

#define INITIAL_CHANNELS 32
/* Arbitrary limit */
#define MAX_CHANNELS (1L<<17)

struct channel_table *
make_channel_table(void)
{
  NEW(channel_table, table);

  table->channels = lsh_space_alloc(sizeof(struct ssh_channel *)
				      * INITIAL_CHANNELS);
  table->in_use = lsh_space_alloc(INITIAL_CHANNELS);
  
  table->allocated_channels = INITIAL_CHANNELS;
  table->used_channels = 0;
  table->next_channel = 0;
  table->channel_count = 0;
  
  table->max_channels = MAX_CHANNELS;

  table->pending_close = 0;

  table->global_requests = make_alist(0, -1);
  table->channel_types = make_alist(0, -1);
  table->open_fallback = NULL;
  
  object_queue_init(&table->local_ports);
  object_queue_init(&table->remote_ports);
  
  object_queue_init(&table->active_global_requests);
  object_queue_init(&table->pending_global_requests);
  
  return table;
}

/* Returns -1 if allocation fails */
/* NOTE: This function returns locally chosen channel numbers, which
 * are always small integers. So there's no problem fitting them in
 * a signed int. */
int
alloc_channel(struct channel_table *table)
{
  UINT32 i;
  
  for(i = table->next_channel; i < table->used_channels; i++)
    {
      if (table->in_use[i] == CHANNEL_FREE)
	{
	  assert(!table->channels[i]);
	  table->in_use[i] = CHANNEL_RESERVED;
	  table->next_channel = i+1;

	  goto success;
	}
    }
  if (i == table->max_channels)
    return -1;

  if (i == table->allocated_channels) 
    {
      UINT32 new_size = table->allocated_channels * 2;
      struct ssh_channel **new_channels;
      UINT8 *new_in_use;

      new_channels = lsh_space_alloc(sizeof(struct ssh_channel *)
				     * new_size);
      memcpy(new_channels, table->channels,
	     sizeof(struct ssh_channel *) * table->used_channels);
      lsh_space_free(table->channels);
      table->channels = new_channels;

      /* FIXME: Use realloc(). */
      new_in_use = lsh_space_alloc(new_size);
      memcpy(new_in_use, table->in_use, table->used_channels);
      lsh_space_free(table->in_use);
      table->in_use = new_in_use;

      table->allocated_channels = new_size;
    }

  table->next_channel = table->used_channels = i+1;

  table->in_use[i] = CHANNEL_RESERVED;

 success:
  table->channel_count++;
  verbose("Allocated local channel number %i\n", i);

  return i;
}

void
dealloc_channel(struct channel_table *table, int i)
{
  assert(i >= 0);
  assert( (unsigned) i < table->used_channels);
  assert(table->channel_count);
  
  verbose("Deallocating local channel %i\n", i);
  table->channels[i] = NULL;
  table->in_use[i] = CHANNEL_FREE;

  table->channel_count--;
  
  if ( (unsigned) i < table->next_channel)
    table->next_channel = i;
}

void
use_channel(struct ssh_connection *connection,
	    UINT32 local_channel_number)
{
  struct channel_table *table = connection->table;
  struct ssh_channel *channel = table->channels[local_channel_number];

  assert(channel);
  assert(table->in_use[local_channel_number] == CHANNEL_RESERVED);
  
  table->in_use[local_channel_number] = CHANNEL_IN_USE;
  verbose("Taking channel %i in use, (local %i).\n",
	  channel->channel_number, local_channel_number);
}
	    
void
register_channel(struct ssh_connection *connection,
		 UINT32 local_channel_number,
		 struct ssh_channel *channel,
		 int take_into_use)
{
  struct channel_table *table = connection->table;
  
  assert(table->in_use[local_channel_number] == CHANNEL_RESERVED);
  assert(!table->channels[local_channel_number]);

  verbose("Registering local channel %i.\n",
	  local_channel_number);
  
  /* NOTE: Is this the right place to install this exception handler? */
  channel->e =
    make_exc_finish_channel_handler(connection,
				    local_channel_number,
				    channel->e ? channel->e : connection->e,
				    HANDLER_CONTEXT);

  table->channels[local_channel_number] = channel;

  if (take_into_use)
    use_channel(connection, local_channel_number);
  
  REMEMBER_RESOURCE(connection->resources, &channel->resources->super);
}

struct ssh_channel *
lookup_channel(struct channel_table *table, UINT32 i)
{
  return ( (i < table->used_channels)
	   && (table->in_use[i] == CHANNEL_IN_USE))
    ? table->channels[i] : NULL;
}

struct ssh_channel *
lookup_channel_reserved(struct channel_table *table, UINT32 i)
{
  return ( (i < table->used_channels)
	   && (table->in_use[i] == CHANNEL_RESERVED))
    ? table->channels[i] : NULL;
}


/* FIXME: It seems suboptimal to send a window adjust message for
 * *every* write that we do. A better scheme might be as follows:
 *
 * Delay window adjust messages, keeping track of both the locally
 * maintained window size, which is updated after each write, and the
 * size that has been reported to the remote end. When the difference
 * between these two values gets large enough (say, larger than one
 * half or one third of the maximum window size), we send a
 * window_adjust message to sync them. */
static void
adjust_rec_window(struct flow_controlled *f, UINT32 written)
{
  CAST_SUBTYPE(ssh_channel, channel, f);

  /* NOTE: The channel object (referenced as a flow-control callback)
   * may live longer than the actual channel. */
  if (! (channel->flags & (CHANNEL_RECEIVED_EOF | CHANNEL_RECEIVED_CLOSE
			   | CHANNEL_SENT_CLOSE)))
    A_WRITE(channel->write,
	    prepare_window_adjust(channel, written));
}

void
channel_start_receive(struct ssh_channel *channel,
		      UINT32 initial_window_size)
{
  if (channel->rec_window_size < initial_window_size)
    A_WRITE(channel->write,
	    prepare_window_adjust
	    (channel, initial_window_size - channel->rec_window_size));
}


/* Ugly macros to make it a little simpler to free the input packet at
 * the right time. */

#define RETURN goto foo_finish
#define END(s) do { foo_finish: \
                    lsh_string_free((s)); \
                    return; } while(0)


/* Channel related messages */

/* GABA:
   (class
     (name request_status)
     (vars
       ; -1 for still active requests,
       ; 0 for failure,
       ; 1 for success
       (status . int)))
*/

static struct request_status *
make_request_status(void)
{
  NEW(request_status, self);
  self->status = -1;

  return self;
}

/* GABA:
   (class
     (name global_request_continuation)
     (super command_continuation)
     (vars
       (connection object ssh_connection)
       (active object request_status)))
*/

static void 
send_global_request_responses(struct ssh_connection *connection, 
			      struct object_queue *q)
{
   for (;;)
     {
       CAST(request_status, n, object_queue_peek_head(q));
       if (!n || (n->status < 0))
	 break;
 
      object_queue_remove_head(q);

      C_WRITE(connection,
	      (n->status
	       ? format_global_success()
	       : format_global_failure()));
    }
}

static void
do_global_request_response(struct command_continuation *s,
			   struct lsh_object *x UNUSED)
{
  CAST(global_request_continuation, self, s);
  struct object_queue *q = &self->connection->table->active_global_requests;

  assert(self->active->status == -1);
  assert(!object_queue_is_empty(q));
	  
  self->active->status = 1;

  send_global_request_responses(self->connection, q);
}

static struct command_continuation *
make_global_request_response(struct ssh_connection *connection,
			     struct request_status *active)
{
  NEW(global_request_continuation, self);

  self->super.c = do_global_request_response;
  self->connection = connection;
  self->active = active;
   
  return &self->super;
}


/* GABA:
   (class
     (name global_request_exception_handler)
     (super exception_handler)
     (vars
       (connection object ssh_connection)
       (active object request_status)))
*/

/* NOTE: We handle *only* EXC_GLOBAL_REQUEST */
static void 
do_exc_global_request_handler(struct exception_handler *c,
			  const struct exception *e)
{
  CAST(global_request_exception_handler, self, c);
  if (e->type == EXC_GLOBAL_REQUEST)
    {
      struct object_queue *q = &self->connection->table->active_global_requests;
      
      assert(self->active->status == -1);
      assert(!object_queue_is_empty(q));

      self->active->status = 0;
  
      send_global_request_responses(self->connection, q);
    }
  else
    EXCEPTION_RAISE(c->parent, e);
}

static struct exception_handler *
make_global_request_exception_handler(struct ssh_connection *connection,
				      struct request_status *active,
				      struct exception_handler *h,
				      const char *context)
{
  NEW(global_request_exception_handler, self);

  self->super.raise = do_exc_global_request_handler;
  self->super.context = context;
  self->super.parent = h;
  self->active = active;
  self->connection = connection;
  return &self->super;
}

DEFINE_PACKET_HANDLER(static, global_request_handler, connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  int name;
  int want_reply;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_GLOBAL_REQUEST)
      && parse_atom(&buffer, &name)
      && parse_boolean(&buffer, &want_reply))
    {
      struct global_request *req = NULL;
      struct command_continuation *c = &discard_continuation;
      struct exception_handler *e = connection->e;

      if (name && connection->table->global_requests)
	{
	  CAST_SUBTYPE(global_request, r,
		       ALIST_GET(connection->table->global_requests,
				 name));
	  req = r;
	}
      if (!req)
	{
	  lsh_string_free(packet);

	  C_WRITE(connection, format_global_failure());
	  return;
	}
      else
	{
	  if (want_reply)
	    {
	      struct request_status *a = make_request_status();
	      
	      object_queue_add_tail(&connection->table->active_global_requests,
				    &a->super);
	      
	      c = make_global_request_response(connection, a);
	      e = make_global_request_exception_handler(connection, a, e, HANDLER_CONTEXT);
	    }
	  else
	    {
	      /* We should ignore failures. */
	      static struct report_exception_info global_req_ignore =
		STATIC_REPORT_EXCEPTION_INFO(EXC_ALL, EXC_GLOBAL_REQUEST,
					     "Ignored:");
	      
	      e = make_report_exception_handler(&global_req_ignore,
						e, HANDLER_CONTEXT);
	    }
	  GLOBAL_REQUEST(req, connection, name, want_reply, &buffer, c, e);
	}
    }
  else
    {
      PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_GLOBAL_REQUEST message.");
    }
  lsh_string_free(packet);
}

DEFINE_PACKET_HANDLER(static, global_success_handler,
		      connection, packet)
{
  if (packet->length != 1)
    {
      PROTOCOL_ERROR(connection->e, "Invalid GLOBAL_REQUEST_SUCCESS message.");
      RETURN;
    }

  assert(packet->data[0] == SSH_MSG_REQUEST_SUCCESS);

  if (object_queue_is_empty(&connection->table->pending_global_requests))
    {
      werror("do_global_request_success: Unexpected message, ignoring.\n");
      return;
    }
  {
    CAST_SUBTYPE(command_context, ctx,
		 object_queue_remove_head(&connection->table->pending_global_requests));
    COMMAND_RETURN(ctx->c, connection);
  }
  END(packet);
}

struct exception global_request_exception =
STATIC_EXCEPTION(EXC_GLOBAL_REQUEST, "Global request failed");

DEFINE_PACKET_HANDLER(static, global_failure_handler,
		      connection, packet)
{
  if (packet->length != 1)
    {
      PROTOCOL_ERROR(connection->e, "Invalid GLOBAL_REQUEST_FAILURE message.");
      RETURN;
    }

  assert(packet->data[0] == SSH_MSG_REQUEST_FAILURE);

  if (object_queue_is_empty(&connection->table->pending_global_requests))
    {
      werror("do_global_request_failure: Unexpected message, ignoring.\n");
    }
  else
    {
      CAST_SUBTYPE(command_context, ctx,
		   object_queue_remove_head(&connection->table->pending_global_requests));
      EXCEPTION_RAISE(ctx->e, &global_request_exception);
    }
  END(packet);
}

/* FIXME: Don't store the channel here, instead have it passed as the
 * argument of the continuation. This might also allow some
 * unification with the handling of global_requests. */

/* GABA:
   (class
     (name channel_request_continuation)
     (super command_continuation)
     (vars
       (connection object ssh_connection)
       (channel object ssh_channel)
       (active object request_status)))
*/

static void
send_channel_request_responses(struct ssh_connection *connection,
			       struct ssh_channel *channel,
			       struct object_queue *q)
{
  for (;;)
    {
      CAST(request_status, n, object_queue_peek_head(q));
      if (!n || (n->status < 0))
	break;

      object_queue_remove_head(q);

      C_WRITE(connection,
	      (n->status
	       ? format_channel_success(channel->channel_number)
	       : format_channel_failure(channel->channel_number)));
    }
}

static void
do_channel_request_response(struct command_continuation *s,
			    struct lsh_object *x UNUSED)
{
  CAST(channel_request_continuation, self, s);
  struct object_queue *q = &self->channel->active_requests;

  assert(self->active->status == -1);
  assert(!object_queue_is_empty(q));
	  
  self->active->status = 1;

  send_channel_request_responses(self->connection, self->channel, q);
}

static struct command_continuation *
make_channel_request_response(struct ssh_connection *connection,
			      struct ssh_channel *channel,
			      struct request_status *active)
{
  NEW(channel_request_continuation, self);

  self->super.c = do_channel_request_response;
  self->connection = connection;
  self->channel = channel;
  self->active = active;

  return &self->super;
}

/* GABA:
   (class
     (name channel_request_exception_handler)
     (super exception_handler)
     (vars
       (connection object ssh_connection)
       (channel object ssh_channel)
       (active object request_status)))
*/

/* NOTE: We handle *only* EXC_CHANNEL_REQUEST */
static void 
do_exc_channel_request_handler(struct exception_handler *c,
			       const struct exception *e)
{
  CAST(channel_request_exception_handler, self, c);
  if (e->type == EXC_CHANNEL_REQUEST)
    {
      struct object_queue *q = &self->channel->active_requests;

      assert(self->active->status == -1);
      assert(!object_queue_is_empty(q));
      
      self->active->status = 0;
      
      send_channel_request_responses(self->connection, self->channel, q);
    }
  else
    EXCEPTION_RAISE(c->parent, e);
}

static struct exception_handler *
make_channel_request_exception_handler(struct ssh_connection *connection,
				       struct ssh_channel *channel,
				       struct request_status *active,
				       struct exception_handler *h,
				       const char *context)
{
  NEW(channel_request_exception_handler, self);

  self->super.raise = do_exc_channel_request_handler;
  self->super.parent = h;
  self->super.context = context;
  self->connection = connection;
  self->channel = channel;
  self->active = active;

  return &self->super;
}

static int
parse_channel_request(struct simple_buffer *buffer,
		      UINT32 *channel_number,
		      struct channel_request_info *info)
{
  unsigned msg_number;

  if (parse_uint8(buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_REQUEST)
      && parse_uint32(buffer, channel_number)
      && parse_string(buffer,
		      &info->type_length, &info->type_data)
      && parse_boolean(buffer, &info->want_reply))
    {
      info->type = lookup_atom(info->type_length, info->type_data);
      return 1;
    }
  else
    return 0;
}

DEFINE_PACKET_HANDLER(static, channel_request_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  struct channel_request_info info;
  UINT32 channel_number;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_channel_request(&buffer, &channel_number, &info))
    {
      struct ssh_channel *channel = lookup_channel(connection->table,
						   channel_number);

      /* NOTE: We can't free packet yet, because it is not yet fully
       * parsed. There may be some more arguments, which are parsed by
       * the CHANNEL_REQUEST method below. */

      if (channel)
	{
	  struct channel_request *req = NULL;
	  struct command_continuation *c = &discard_continuation;
	  struct exception_handler *e = channel->e;

	  if (info.type && channel->request_types)
	    {
	      CAST_SUBTYPE(channel_request, r,
			   ALIST_GET(channel->request_types, info.type));
	      req = r;
	    }
	  if (!req)
	    req = channel->request_fallback;
	  
	  if (req)
	    {
	      if (info.want_reply)
		{
		  struct request_status *a = make_request_status();
		  
		  object_queue_add_tail(&channel->active_requests,
					&a->super);
		  
		  c = make_channel_request_response(connection, channel, a);
		  e = make_channel_request_exception_handler(connection, channel, a, e, HANDLER_CONTEXT);
		}
	      else
		{
		  /* We should ignore failures. */
		  static struct report_exception_info channel_req_ignore =
		    STATIC_REPORT_EXCEPTION_INFO(EXC_ALL, EXC_CHANNEL_REQUEST,
						 "Ignored:");
		  
		  e = make_report_exception_handler(&channel_req_ignore,
						    e, HANDLER_CONTEXT);
		}
	      
	      CHANNEL_REQUEST(req, channel, connection, &info, &buffer, c, e);
	    }
	  else
	    {
	      if (info.want_reply)
		C_WRITE(connection,
			format_channel_failure(channel->channel_number));
	    }
	}
      else
	{
	  werror("SSH_MSG_CHANNEL_REQUEST on nonexistant channel %i: %xS\n",
		 channel_number, packet);
	}
    }
  else
    PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_CHANNEL_REQUEST message.");
  
  lsh_string_free(packet);
}


/* GABA:
   (class
     (name channel_open_continuation)
     (super command_continuation)
     (vars
       (connection object ssh_connection)
       (local_channel_number . UINT32)
       (remote_channel_number . UINT32)
       (send_window_size . UINT32)
       (send_max_packet . UINT32)))
*/

static void
do_channel_open_continue(struct command_continuation *c,
			 struct lsh_object *value)
{
  CAST(channel_open_continuation, self, c);
  CAST_SUBTYPE(ssh_channel, channel, value);

  assert(channel);

  /* FIXME: This copying could just as well be done by the
   * CHANNEL_OPEN handler? Then we can remove the corresponding fields
   * from the closure as well. */
  channel->send_window_size = self->send_window_size;
  channel->send_max_packet = self->send_max_packet;
  channel->channel_number = self->remote_channel_number;

  /* FIXME: Is the channel->write field really needed? */
  channel->write = self->connection->write;

  register_channel(self->connection,
		   self->local_channel_number, channel,
		   1);

  /* FIXME: Doesn't support sending extra arguments with the
   * confirmation message. */

  C_WRITE(self->connection,
	  format_open_confirmation(channel, self->local_channel_number, ""));
}

static struct command_continuation *
make_channel_open_continuation(struct ssh_connection *connection,
			       UINT32 local_channel_number,
			       UINT32 remote_channel_number,
			       UINT32 send_window_size,
			       UINT32 send_max_packet)
{
  NEW(channel_open_continuation, self);

  self->super.c = do_channel_open_continue;
  self->connection = connection;
  self->local_channel_number = local_channel_number;
  self->remote_channel_number = remote_channel_number;
  self->send_window_size = send_window_size;
  self->send_max_packet = send_max_packet;

  return &self->super;
}
			       
/* GABA:
   (class
     (name exc_channel_open_handler)
     (super exception_handler)
     (vars
       (connection object ssh_connection)
       (local_channel_number . UINT32)
       (remote_channel_number . UINT32)))
*/

static void
do_exc_channel_open_handler(struct exception_handler *s,
			    const struct exception *e)
{
  CAST(exc_channel_open_handler, self, s);

  switch (e->type)
    {
    case EXC_CHANNEL_OPEN:
      {
	CAST_SUBTYPE(channel_open_exception, exc, e);
	struct channel_table *table = self->connection->table;
	
	assert(table->in_use[self->local_channel_number]);
	assert(!table->channels[self->local_channel_number]);

	dealloc_channel(table, self->local_channel_number);
	
        C_WRITE(self->connection,
		format_open_failure(self->remote_channel_number,
				    exc->error_code, e->msg, ""));
	break;
      }
    default:
      EXCEPTION_RAISE(self->super.parent, e);
    }      
}

static struct exception_handler *
make_exc_channel_open_handler(struct ssh_connection *connection,
			      UINT32 local_channel_number,
			      UINT32 remote_channel_number,
			      struct exception_handler *parent,
			      const char *context)
{
  NEW(exc_channel_open_handler, self);
  self->super.parent = parent;
  self->super.raise = do_exc_channel_open_handler;
  self->super.context = context;
  
  self->connection = connection;
  self->local_channel_number = local_channel_number;
  self->remote_channel_number = remote_channel_number;

  return &self->super;
}

static int
parse_channel_open(struct simple_buffer *buffer,
		   struct channel_open_info *info)
{
  unsigned msg_number;

  if (parse_uint8(buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_OPEN)
      && parse_string(buffer, &info->type_length, &info->type_data)
      && parse_uint32(buffer, &info->remote_channel_number)
      && parse_uint32(buffer, &info->send_window_size)
      && parse_uint32(buffer, &info->send_max_packet))
    {
      info->type = lookup_atom(info->type_length, info->type_data);

      /* We don't support larger packets than the default,
       * SSH_MAX_PACKET. The fuzz factor is because the
       * channel's max sizes refer to the data string inside the
       * packet, while the SSH_PACKET limit refers to the complete
       * packet including some overhead (9 octets for
       * SSH_MSG_CHANNEL_DATA and 13 octets for
       * SSH_MSG_CHANNEL_EXTENDED_DATA). */
      if (info->send_max_packet > (SSH_MAX_PACKET - SSH_CHANNEL_MAX_PACKET_FUZZ))
	{
	  werror("do_channel_open: The remote end asked for really large packets.\n");
	  info->send_max_packet = SSH_MAX_PACKET - SSH_CHANNEL_MAX_PACKET_FUZZ;
	}

      return 1;
    }
  else
    return 0;
}


DEFINE_PACKET_HANDLER(static, channel_open_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  struct channel_open_info info;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_channel_open(&buffer, &info))
    {
      struct channel_open *open = NULL;

      /* NOTE: We can't free the packet yet, as the buffer is passed
       * to the CHANNEL_OPEN method later. */

      if (connection->table->pending_close)
	{
	  /* We are waiting for channels to close. Don't open any new ones. */

	  C_WRITE(connection,
		  format_open_failure(info.remote_channel_number,
				      SSH_OPEN_ADMINISTRATIVELY_PROHIBITED,
				      "Waiting for channels to close.", ""));
	}
      else
	{
	  if (info.type)
	    {
	      CAST_SUBTYPE(channel_open, o,
			   ALIST_GET(connection->table->channel_types,
				     info.type));
	      open = o;
	    }

	  if (!open)
	    open = connection->table->open_fallback;
	  
	  if (!open)
	    {
	      C_WRITE(connection,
		      format_open_failure(info.remote_channel_number,
					  SSH_OPEN_UNKNOWN_CHANNEL_TYPE,
					  "Unknown channel type", ""));
	    }
	  else
	    {
	      int local_number = alloc_channel(connection->table);

	      if (local_number < 0)
		C_WRITE(connection,
			format_open_failure(info.remote_channel_number,
					    SSH_OPEN_RESOURCE_SHORTAGE,
					    "Channel limit exceeded.", ""));

	      CHANNEL_OPEN(open, connection,
			   &info,
			   &buffer,
			   make_channel_open_continuation(connection,
							  local_number,
							  info.remote_channel_number,
							  info.send_window_size,
							  info.send_max_packet),
			   make_exc_channel_open_handler(connection,
							 local_number,
							 info.remote_channel_number,
							 connection->e,
							 HANDLER_CONTEXT));

	    }
	}
    }
  else
    PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_CHANNEL_OPEN message.");

  lsh_string_free(packet);
}     

DEFINE_PACKET_HANDLER(static, window_adjust_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  UINT32 size;

  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_WINDOW_ADJUST)
      && parse_uint32(&buffer, &channel_number)
      && parse_uint32(&buffer, &size)
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel = lookup_channel(connection->table,
						   channel_number);

      lsh_string_free(packet);
      
      if (channel
	  && !(channel->flags & CHANNEL_RECEIVED_CLOSE))
	{
	  if (! (channel->flags & (CHANNEL_SENT_CLOSE | CHANNEL_SENT_EOF)))
	    {
	      channel->send_window_size += size;
	      if (channel->send_window_size && channel->send_adjust)
		{
		  assert(channel->send_window_size);
		  CHANNEL_SEND_ADJUST(channel, size);
		}
	    }
	}
      else
	{
	  werror("SSH_MSG_CHANNEL_WINDOW_ADJUST on nonexistant or closed "
		 "channel %i\n", channel_number);
	  PROTOCOL_ERROR(connection->e, "Unexpected CHANNEL_WINDOW_ADJUST");
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_WINDOW_ADJUST message.");
    }
}

DEFINE_PACKET_HANDLER(static, channel_data_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  struct lsh_string *data;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_DATA)
      && parse_uint32(&buffer, &channel_number)
      && ( (data = parse_string_copy(&buffer)) )
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel = lookup_channel(connection->table,
						   channel_number);

      lsh_string_free(packet);
      
      if (channel && channel->receive
	  && !(channel->flags & (CHANNEL_RECEIVED_EOF
				 | CHANNEL_RECEIVED_CLOSE)))
	{
	  if (channel->flags & CHANNEL_SENT_CLOSE)
	    {
	      lsh_string_free(data);
	      werror("Ignoring data on channel which is closing\n");
	      return;
	    }
	  else
	    {
              if (data->length > channel->rec_max_packet)
                {
                  werror("Channel data larger than rec_max_packet. Extra data ignored.\n");
                  data->length = channel->rec_max_packet;
                }

	      if (data->length > channel->rec_window_size)
		{
		  /* Truncate data to fit window */
		  werror("Channel data overflow. Extra data ignored.\n");
		  debug("   (data->length=%i, rec_window_size=%i).\n", 
			data->length, channel->rec_window_size);
		  
		  data->length = channel->rec_window_size;
		}

	      if (!data->length)
		{
		  /* Ignore data packet */
		  lsh_string_free(data);
		  return;
		}
	      channel->rec_window_size -= data->length;

	      CHANNEL_RECEIVE(channel, CHANNEL_DATA, data);
	    }
	}
      else
	{
	  werror("Data on closed or non-existant channel %i\n",
		 channel_number);
	  lsh_string_free(data);
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_DATA message.");
    }
}

DEFINE_PACKET_HANDLER(static, channel_extended_data_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  UINT32 type;
  struct lsh_string *data;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_EXTENDED_DATA)
      && parse_uint32(&buffer, &channel_number)
      && parse_uint32(&buffer, &type)
      && ( (data = parse_string_copy(&buffer)) )
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel = lookup_channel(connection->table,
						   channel_number);

      lsh_string_free(packet);
      
      if (channel && channel->receive
	  && !(channel->flags & (CHANNEL_RECEIVED_EOF
				 | CHANNEL_RECEIVED_CLOSE)))
	{
	  if (channel->flags & CHANNEL_SENT_CLOSE)
	    {
	      lsh_string_free(data);
	      werror("Ignoring extended data on channel which is closing\n");
	      return;
	    }
	  else
	    {
              if (data->length > channel->rec_max_packet)
                {
                  werror("Channel data larger than rec_max_packet. Extra data ignored.\n");
                  data->length = channel->rec_max_packet;
                }

	      if (data->length > channel->rec_window_size)
		{
		  /* Truncate data to fit window */
		  werror("Channel extended data overflow. "
			 "Extra data ignored.\n");
		  debug("   (data->length=%i, rec_window_size=%i).\n", 
			data->length, channel->rec_window_size);
		  data->length = channel->rec_window_size;
		}
	      
	      if (!data->length)
		{
		  /* Ignore data packet */
		  lsh_string_free(data);
		  return;
		}

	      channel->rec_window_size -= data->length;

	      switch(type)
		{
		case SSH_EXTENDED_DATA_STDERR:
		  CHANNEL_RECEIVE(channel, CHANNEL_STDERR_DATA, data);
		  break;
		default:
		  werror("Unknown type %i of extended data.\n",
			 type);
		  lsh_string_free(data);
		}
	    }
	}
      else
	{
	  werror("Extended data on closed or non-existant channel %i\n",
		 channel_number);
	  lsh_string_free(data);
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_EXTENDED_DATA message.");
    }
}

DEFINE_PACKET_HANDLER(static, channel_eof_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_EOF)
      && parse_uint32(&buffer, &channel_number)
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel = lookup_channel(connection->table,
						   channel_number);

      lsh_string_free(packet);

      if (channel)
	{
	  if (channel->flags & (CHANNEL_RECEIVED_EOF | CHANNEL_RECEIVED_CLOSE))
	    {
	      werror("Receiving EOF on channel on closed channel.\n");
	      PROTOCOL_ERROR(connection->e,
			     "Received EOF on channel on closed channel.");
	    }
	  else
	    {
	      verbose("Receiving EOF on channel %i (local %i)\n",
		      channel->channel_number, channel_number);
	      
	      channel->flags |= CHANNEL_RECEIVED_EOF;
	      
	      if (channel->eof)
		{
		  CHANNEL_EOF(channel);

		  /* Should we close the channel now? */
		  if ( (channel->flags & CHANNEL_SENT_EOF)
		       && (channel->flags & CHANNEL_CLOSE_AT_EOF))
		    channel_close(channel);
		}
	      else
		{
		  /* By default, close the channel. */
		  debug("No CHANNEL_EOF handler. Closing.\n"); 
		  channel_close(channel);
		}
	      
	    }
	}
      else
	{
	  werror("EOF on non-existant channel %i\n",
		 channel_number);
	  PROTOCOL_ERROR(connection->e, "EOF on non-existant channel");
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_EOF message");
    }
}

DEFINE_PACKET_HANDLER(static, channel_close_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_CLOSE)
      && parse_uint32(&buffer, &channel_number)
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel = lookup_channel(connection->table,
						   channel_number);

      lsh_string_free(packet);
      
      if (channel)
	{
	  verbose("Receiving CLOSE on channel %i (local %i)\n",
		  channel->channel_number, channel_number);
	      
	  if (channel->flags & CHANNEL_RECEIVED_CLOSE)
	    {
	      werror("Receiving multiple CLOSE on channel.\n");
	      PROTOCOL_ERROR(connection->e, "Receiving multiple CLOSE on channel.");
	    }
	  else
	    {
	      channel->flags |= CHANNEL_RECEIVED_CLOSE;
	  
	      if (! (channel->flags & (CHANNEL_RECEIVED_EOF | CHANNEL_NO_WAIT_FOR_EOF
				       | CHANNEL_SENT_CLOSE)))
		{
		  werror("Unexpected channel CLOSE.\n");
		}

#if 0
	      /* FIXME: Which eof-handlers rely on being called in
	       * this case? */
	      if (! (channel->flags & (CHANNEL_RECEIVED_EOF))
		  && channel->eof)
		CHANNEL_EOF(channel);
#endif
	      if (channel->flags & CHANNEL_SENT_CLOSE)
		{
		  static const struct exception finish_exception
		    = STATIC_EXCEPTION(EXC_FINISH_CHANNEL, "Received CLOSE message.");
	      
		  EXCEPTION_RAISE(channel->e,
				  &finish_exception);
		}
	      else
		channel_close(channel);
	    }
	}
      else
	{
	  werror("CLOSE on non-existant channel %i\n",
		 channel_number);
	  PROTOCOL_ERROR(connection->e, "CLOSE on non-existant channel");
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_CLOSE message");
    }
}

DEFINE_PACKET_HANDLER(static, channel_open_confirm_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 local_channel_number;
  UINT32 remote_channel_number;  
  UINT32 window_size;
  UINT32 max_packet;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_OPEN_CONFIRMATION)
      && parse_uint32(&buffer, &local_channel_number)
      && parse_uint32(&buffer, &remote_channel_number)
      && parse_uint32(&buffer, &window_size)
      && parse_uint32(&buffer, &max_packet)
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel =
	lookup_channel_reserved(connection->table,
				local_channel_number);

      lsh_string_free(packet);

      if (channel) 
	{
	  struct command_continuation *c = channel->open_continuation;
	  assert(c);

	  channel->open_continuation = NULL;

	  channel->channel_number = remote_channel_number;
	  channel->send_window_size = window_size;
	  channel->send_max_packet = max_packet;

	  /* FIXME: Initialize channel->write here? */
	  use_channel(connection, local_channel_number);

	  COMMAND_RETURN(c, channel);
	}
      else
	{
	  werror("Unexpected SSH_MSG_CHANNEL_OPEN_CONFIRMATION on channel %i\n",
		 local_channel_number);
	  PROTOCOL_ERROR(connection->e, "Unexpected CHANNEL_OPEN_CONFIRMATION.");
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_OPEN_CONFIRMATION message.");
    }
}

DEFINE_PACKET_HANDLER(static, channel_open_failure_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  UINT32 reason;

  const UINT8 *msg;
  UINT32 length;

  const UINT8 *language;
  UINT32 language_length;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_OPEN_FAILURE)
      && parse_uint32(&buffer, &channel_number)
      && parse_uint32(&buffer, &reason)
      && parse_string(&buffer, &length, &msg)
      && parse_string(&buffer, &language_length, &language)
      && parse_eod(&buffer))
    {
      struct ssh_channel *channel =
	lookup_channel_reserved(connection->table,
				channel_number);

      lsh_string_free(packet); 

      if (channel)
	{
	  static const struct exception finish_exception
	    = STATIC_EXCEPTION(EXC_FINISH_CHANNEL, "CHANNEL_OPEN failed.");

	  assert(channel->open_continuation);
	  
	  /* FIXME: It would be nice to pass the message on. */
	  EXCEPTION_RAISE(channel->e,
			  make_channel_open_exception(reason, "Refused by peer"));
	  EXCEPTION_RAISE(channel->e, &finish_exception);
	}
      else
	werror("Unexpected SSH_MSG_CHANNEL_OPEN_FAILURE on channel %i\n",
	       channel_number);
    }
  else
    {
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_OPEN_FAILURE message.");
      lsh_string_free(packet);
    }
}

DEFINE_PACKET_HANDLER(static, channel_success_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  struct ssh_channel *channel;
      
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_SUCCESS)
      && parse_uint32(&buffer, &channel_number)
      && parse_eod(&buffer)
      && (channel = lookup_channel(connection->table, channel_number)))
    {
      lsh_string_free(packet);

      if (object_queue_is_empty(&channel->pending_requests))
	{
	  werror("do_channel_success: Unexpected message. Ignoring.\n");
	}
      else
	{
	  CAST_SUBTYPE(command_context, ctx,
		       object_queue_remove_head(&channel->pending_requests));
	  
	  COMMAND_RETURN(ctx->c, channel);
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_SUCCESS message");
    }
}

DEFINE_PACKET_HANDLER(static, channel_failure_handler,
		      connection, packet)
{
  struct simple_buffer buffer;
  unsigned msg_number;
  UINT32 channel_number;
  struct ssh_channel *channel;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_CHANNEL_FAILURE)
      && parse_uint32(&buffer, &channel_number)
      && parse_eod(&buffer)
      && (channel = lookup_channel(connection->table, channel_number)))
    {
      lsh_string_free(packet);
      
      if (object_queue_is_empty(&channel->pending_requests))
	{
	  werror("do_channel_failure: No handler. Ignoring.\n");
	}
      else
	{
	  static const struct exception channel_request_exception =
	    STATIC_EXCEPTION(EXC_CHANNEL_REQUEST, "Channel request failed");

	  CAST_SUBTYPE(command_context, ctx,
		       object_queue_remove_head(&channel->pending_requests));
	  
	  EXCEPTION_RAISE(ctx->e, &channel_request_exception);
	}
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_FAILURE message.");
    }
}


void init_connection_service(struct ssh_connection *connection)
{
  struct channel_table *table = make_channel_table();
  
  debug("channel.c: do_connection_service\n");
  
  connection->table = table;

  connection->dispatch[SSH_MSG_GLOBAL_REQUEST]
    = &global_request_handler;
  connection->dispatch[SSH_MSG_CHANNEL_OPEN]
    = &channel_open_handler;
  connection->dispatch[SSH_MSG_CHANNEL_REQUEST]
    = &channel_request_handler;
  
  connection->dispatch[SSH_MSG_CHANNEL_WINDOW_ADJUST]
    = &window_adjust_handler;
  connection->dispatch[SSH_MSG_CHANNEL_DATA]
    = &channel_data_handler;
  connection->dispatch[SSH_MSG_CHANNEL_EXTENDED_DATA]
    = &channel_extended_data_handler;

  connection->dispatch[SSH_MSG_CHANNEL_EOF]
    = &channel_eof_handler;
  connection->dispatch[SSH_MSG_CHANNEL_CLOSE]
    = &channel_close_handler;

  connection->dispatch[SSH_MSG_CHANNEL_OPEN_CONFIRMATION]
    = &channel_open_confirm_handler;
  connection->dispatch[SSH_MSG_CHANNEL_OPEN_FAILURE]
    = &channel_open_failure_handler;
  
  connection->dispatch[SSH_MSG_CHANNEL_SUCCESS]
    = &channel_success_handler;
  connection->dispatch[SSH_MSG_CHANNEL_FAILURE]
    = &channel_failure_handler;

  connection->dispatch[SSH_MSG_REQUEST_SUCCESS]
    = &global_success_handler;
  connection->dispatch[SSH_MSG_REQUEST_FAILURE]
    = &global_failure_handler;
}
 
DEFINE_COMMAND_SIMPLE(connection_service_command, a)
{
  CAST(ssh_connection, connection, a);

  init_connection_service(connection);

  return a;
}

struct lsh_string *
format_channel_close(struct ssh_channel *channel)
{
  return ssh_format("%c%i",
		    SSH_MSG_CHANNEL_CLOSE,
		    channel->channel_number);
}

void
channel_close(struct ssh_channel *channel)
{
  static const struct exception finish_exception =
    STATIC_EXCEPTION(EXC_FINISH_CHANNEL, "Closing channel");

  if (! (channel->flags & CHANNEL_SENT_CLOSE))
    {
      verbose("Sending CLOSE on channel %i\n", channel->channel_number);

      channel->flags |= CHANNEL_SENT_CLOSE;
      
      A_WRITE(channel->write, format_channel_close(channel) );

      if (channel->flags & CHANNEL_RECEIVED_CLOSE)
	EXCEPTION_RAISE(channel->e, &finish_exception);
    }
}

struct lsh_string *
format_channel_eof(struct ssh_channel *channel)
{
  return ssh_format("%c%i",
		    SSH_MSG_CHANNEL_EOF,
		    channel->channel_number);
}

void
channel_eof(struct ssh_channel *channel)
{
  if (! (channel->flags &
	 (CHANNEL_SENT_EOF | CHANNEL_SENT_CLOSE | CHANNEL_RECEIVED_CLOSE)))
    {
      verbose("Sending EOF on channel %i\n", channel->channel_number);

      channel->flags |= CHANNEL_SENT_EOF;
      A_WRITE(channel->write, format_channel_eof(channel) );

      if ( (channel->flags & CHANNEL_CLOSE_AT_EOF)
	   && (channel->flags & (CHANNEL_RECEIVED_EOF | CHANNEL_NO_WAIT_FOR_EOF)) )
	{
	  /* Initiate close */
	  channel_close(channel);
	}
    }
}

void
init_channel(struct ssh_channel *channel)
{
  /* channel->super.handler = do_read_channel; */
  channel->write = NULL;
  channel->super.report = adjust_rec_window;
  
  channel->flags = CHANNEL_CLOSE_AT_EOF;
  channel->sources = 0;
  
  channel->request_types = NULL;
  channel->request_fallback = NULL;
  
  channel->receive = NULL;
  channel->send_adjust = NULL;

  channel->close = NULL;
  channel->eof = NULL;

  channel->open_continuation = NULL;

  channel->resources = empty_resource_list();
  
  object_queue_init(&channel->pending_requests);
  object_queue_init(&channel->active_requests);
}

struct lsh_string *
channel_transmit_data(struct ssh_channel *channel,
		      struct lsh_string *data)
{
  assert(data->length <= channel->send_window_size);
  assert(data->length <= channel->send_max_packet);
  channel->send_window_size -= data->length;
  
  return ssh_format("%c%i%fS",
		    SSH_MSG_CHANNEL_DATA,
		    channel->channel_number,
		    data);
}

struct lsh_string *
channel_transmit_extended(struct ssh_channel *channel,
			  UINT32 type,
			  struct lsh_string *data)
{
  assert(data->length <= channel->send_window_size);
  assert(data->length <= channel->send_max_packet);
  channel->send_window_size -= data->length;

  return ssh_format("%c%i%i%fS",
		    SSH_MSG_CHANNEL_EXTENDED_DATA,
		    channel->channel_number,
		    type,
		    data);
}

/* Writing data to a channel */

/* NOTE: Flow control when sending data on a channel works as follows:
 * When the i/o backend wants to read from one of the channel's
 * sources, it first calls do_read_data_query (in read_data.c),
 * which looks at the current value of send_window_size to determine
 * how much data can be sent right now. The backend reads at most that
 * amount of data, and then calls do_channel_write or
 * do_channel_write_extended. These objects are responsible for
 * subtracting the actual amount of data from the send_window_size.
 *
 * It is crucial that no other i/o is done between the call to
 * do_read_data_query and do_channel_write, otherwise we would have a
 * race condition.
 *
 * At EOF, decrementing the sources count is not done here; it's done
 * by the appropriate i/o close callback. These objects does checks if
 * sources == 1, to determine if any eof message should be sent. */

/* GABA:
   (class
     (name channel_write)
     (super abstract_write)
     (vars
       (channel object ssh_channel)))
*/

/* GABA:
   (class
     (name channel_write_extended)
     (super channel_write)
     (vars
       (type . UINT32)))
*/

static void
do_channel_write(struct abstract_write *w,
		 struct lsh_string *packet)
{
  CAST(channel_write, closure, w);

  if (!packet)
    {
      /* EOF */
      assert(closure->channel->sources);
      if (closure->channel->sources == 1)
	channel_eof(closure->channel);
    }
  else
    A_WRITE(closure->channel->write,
	    channel_transmit_data(closure->channel, packet) );
}

static void
do_channel_write_extended(struct abstract_write *w,
			  struct lsh_string *packet)
{
  CAST(channel_write_extended, closure, w);

  if (!packet)
    {
      /* EOF */
      assert(closure->super.channel->sources);
      if (closure->super.channel->sources == 1)
	channel_eof(closure->super.channel);
    }
  else
    A_WRITE(closure->super.channel->write,
	    channel_transmit_extended(closure->super.channel,
				      closure->type,
				      packet));
}

struct abstract_write *
make_channel_write(struct ssh_channel *channel)
{
  NEW(channel_write, closure);
  
  closure->super.write = do_channel_write;
  closure->channel = channel;

  return &closure->super;
}

struct abstract_write *
make_channel_write_extended(struct ssh_channel *channel,
			    UINT32 type)
{
  NEW(channel_write_extended, closure);

  closure->super.super.write = do_channel_write_extended;
  closure->super.channel = channel;
  closure->type = type;
  
  return &closure->super.super;
}

struct io_callback *
make_channel_read_data(struct ssh_channel *channel)
{
  /* byte      SSH_MSG_CHANNEL_DATA
   * uint32    recipient channel
   * string    data
   *
   * gives 9 bytes of overhead, including the length field. */
    
  return make_read_data(channel, make_channel_write(channel)); }

struct io_callback *
make_channel_read_stderr(struct ssh_channel *channel)
{
  /* byte      SSH_MSG_CHANNEL_EXTENDED_DATA
   * uint32    recipient_channel
   * uint32    data_type_code
   * string    data
   *
   * gives 13 bytes of overhead, including the length field for the string. */

  return make_read_data(channel,
			make_channel_write_extended(channel,
						    SSH_EXTENDED_DATA_STDERR));
}    

/* GABA:
   (class
     (name channel_close_callback)
     (super lsh_callback)
     (vars
       (channel object ssh_channel))) */


/* NOTE: This callback is almost redundant. The EOF cases in
 * do_channel_write and do_channel_write_extended should take care of
 * sending SSH_MSG_CHANNEL_EOF when appropriate. But we still need
 * this callback, in order to reliably decrement the sources count in
 * all cases, including i/o errors. */

/* Close callback for files we are reading from. */

static void
channel_read_close_callback(struct lsh_callback *c)
{
  CAST(channel_close_callback, closure, c);

  trace("channel_read_close_callback: File closed.\n");

  assert(closure->channel->sources);
  
  if (!--closure->channel->sources)
    {
      /* Send eof, unless already done. */
      channel_eof(closure->channel);
    }
}

struct lsh_callback *
make_channel_read_close_callback(struct ssh_channel *channel)
{
  NEW(channel_close_callback, closure);
  
  closure->super.f = channel_read_close_callback;
  closure->channel = channel;

  return &closure->super;
}

/* Exception handler that closes the channel on I/O errors.
 * Primarily used for write fd:s that the channel is fed into.
 *
 * FIXME: Ideally, I'd like to pass something like broken pipe to the
 * other end, on write errors, but I don't see how to do that.
 * 
 * NOTE: This isn't used by tcpforward channels. But that is not a big
 * problem, because there is only one fd involved. Any error (on
 * either read or write) will close that fd, and then the
 * channel_read_close_callback will close the channel. */

/* GABA:
   (class
     (name channel_io_exception_handler)
     (super exception_handler)
     (vars
       (channel object ssh_channel)
       (prefix . "const char *")))
*/

static void
do_channel_io_exception_handler(struct exception_handler *s,
				const struct exception *x)
{
  CAST(channel_io_exception_handler, self, s);
  if (x->type & EXC_IO)
    {
      werror("channel.c: I/O error on write, %z\n", x->msg);
#if 0
      send_debug_message(self->channel->write,
			 ssh_format("%z I/O error: %z\n",
				    self->prefix, x->msg),
			 1);
#endif
      channel_close(self->channel);
    }
  else
    EXCEPTION_RAISE(s->parent, x);
}

struct exception_handler *
make_channel_io_exception_handler(struct ssh_channel *channel,
				  const char *prefix,
				  struct exception_handler *parent,
				  const char *context)
{
  NEW(channel_io_exception_handler, self);
  self->super.raise = do_channel_io_exception_handler;
  self->super.parent = parent;
  self->super.context = context;
  
  self->channel = channel;
  self->prefix = prefix;

  return &self->super;
}

/* Used by do_gateway_channel_open */
struct lsh_string *
format_channel_open_s(struct lsh_string *type,
		      UINT32 local_channel_number,
		      struct ssh_channel *channel,
		      struct lsh_string *args)
{
  return ssh_format("%c%S%i%i%i%lS", SSH_MSG_CHANNEL_OPEN,
		    type, local_channel_number, 
		    channel->rec_window_size, channel->rec_max_packet,
 		    args);
}

struct lsh_string *
format_channel_open(int type, UINT32 local_channel_number,
		    struct ssh_channel *channel,
		    const char *format, ...)
{
  va_list args;
  UINT32 l1, l2;
  struct lsh_string *packet;
  
#define OPEN_FORMAT "%c%a%i%i%i"
#define OPEN_ARGS SSH_MSG_CHANNEL_OPEN, type, local_channel_number, \
  channel->rec_window_size, channel->rec_max_packet  

  debug("format_channel_open: rec_window_size = %i,\n"
	"                     rec_max_packet = %i,\n",
	channel->rec_window_size,
	channel->rec_max_packet);
  
  l1 = ssh_format_length(OPEN_FORMAT, OPEN_ARGS);
  
  va_start(args, format);
  l2 = ssh_vformat_length(format, args);
  va_end(args);

  packet = lsh_string_alloc(l1 + l2);

  ssh_format_write(OPEN_FORMAT, l1, packet->data, OPEN_ARGS);

  va_start(args, format);
  ssh_vformat_write(format, l2, packet->data+l1, args);
  va_end(args);

  return packet;
#undef OPEN_FORMAT
#undef OPEN_ARGS
}

struct lsh_string *
format_channel_request_i(struct channel_request_info *info,
			 struct ssh_channel *channel,
			 UINT32 args_length, const UINT8 *args_data)
{
  return ssh_format("%c%i%s%c%ls", SSH_MSG_CHANNEL_REQUEST,
		    channel->channel_number,
		    info->type_length, info->type_data,
		    info->want_reply,
		    args_length, args_data);
}

struct lsh_string *
format_channel_request(int type, struct ssh_channel *channel,
		       int want_reply, const char *format, 
		       ...)
{
  va_list args;
  UINT32 l1, l2;
  struct lsh_string *packet;

#define REQUEST_FORMAT "%c%i%a%c"
#define REQUEST_ARGS SSH_MSG_CHANNEL_REQUEST, channel->channel_number, \
  type, want_reply
    
  l1 = ssh_format_length(REQUEST_FORMAT, REQUEST_ARGS);
  
  va_start(args, format);
  l2 = ssh_vformat_length(format, args);
  va_end(args);

  packet = lsh_string_alloc(l1 + l2);

  ssh_format_write(REQUEST_FORMAT, l1, packet->data, REQUEST_ARGS);

  va_start(args, format);
  ssh_vformat_write(format, l2, packet->data+l1, args);
  va_end(args);

  return packet;
#undef REQUEST_FORMAT
#undef REQUEST_ARGS
}

struct lsh_string *
format_global_request(int type, int want_reply,
		      const char *format, ...)
{
  va_list args;
  UINT32 l1, l2;
  struct lsh_string *packet;

#define REQUEST_FORMAT "%c%a%c"
#define REQUEST_ARGS SSH_MSG_GLOBAL_REQUEST, type, want_reply
    
  l1 = ssh_format_length(REQUEST_FORMAT, REQUEST_ARGS);
  
  va_start(args, format);
  l2 = ssh_vformat_length(format, args);
  va_end(args);

  packet = lsh_string_alloc(l1 + l2);

  ssh_format_write(REQUEST_FORMAT, l1, packet->data, REQUEST_ARGS);

  va_start(args, format);
  ssh_vformat_write(format, l2, packet->data+l1, args);
  va_end(args);

  return packet;
#undef REQUEST_FORMAT
#undef REQUEST_ARGS
}

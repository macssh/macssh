/* connection.c
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "connection.h"

#include "compress.h"
#include "debug.h"
#include "disconnect.h"
#include "encrypt.h"
#include "exception.h"
#include "format.h"
#include "io.h"
#include "keyexchange.h"
#include "pad.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "connection.h.x"
#undef GABA_DEFINE

#include "connection.c.x"

const char *packet_types[0x100] =
#include "packet_types.h"
;

static void
connection_handle_packet(struct ssh_connection *closure,
			 struct lsh_string *packet)
{
  UINT8 msg;

  assert(!closure->paused);
  
  if (!packet->length)
    {
      werror("connection.c: Received empty packet!\n");
      PROTOCOL_ERROR(closure->e, "Received empty packet.");
      lsh_string_free(packet);
      return;
    }

  if (packet->length > closure->rec_max_packet)
    {
      werror("connection.c: Packet too large!\n");
      PROTOCOL_ERROR(closure->e, "Packet too large");
      lsh_string_free(packet);
      return;
    }
    
  msg = packet->data[0];

  debug("handle_connection: Received packet of type %i (%z)\n",
	msg, packet_types[msg]);
  
  switch(closure->kex_state)
    {
    case KEX_STATE_INIT:
      if (msg == SSH_MSG_NEWKEYS)
	{
	  werror("Unexpected NEWKEYS message!\n");
	  PROTOCOL_ERROR(closure->e, "Unexpected NEWKEYS message!");
	  lsh_string_free(packet);
	  return;
	}
      break;
    case KEX_STATE_IGNORE:
      debug("handle_connection: Ignoring packet %i\n", msg);

      /* It's conceivable with key exchange methods for which one
       * wants to switch to the NEWKEYS state immediately. But for
       * now, we always switch to the IN_PROGRESS state, to wait for a
       * KEXDH_INIT or KEXDH_REPLY message. */
      closure->kex_state = KEX_STATE_IN_PROGRESS;
      lsh_string_free(packet);
      return;

    case KEX_STATE_IN_PROGRESS:
      if ( (msg == SSH_MSG_NEWKEYS)
	   || (msg == SSH_MSG_KEXINIT))
	{
	  werror("Unexpected KEXINIT or NEWKEYS message!\n");
	  PROTOCOL_ERROR(closure->e, "Unexpected KEXINIT or NEWKEYS message!");
	  lsh_string_free(packet);
	  return;
	}
      break;
    case KEX_STATE_NEWKEYS:
      if (! ((msg == SSH_MSG_NEWKEYS)
	     || (msg == SSH_MSG_DISCONNECT)
	     || (msg == SSH_MSG_IGNORE)
	     || (msg == SSH_MSG_DEBUG)))
	{
	  werror("Expected NEWKEYS message, but received message %i!\n",
		 msg);
	  PROTOCOL_ERROR(closure->e, "Expected NEWKEYS message");
	  lsh_string_free(packet);
	  return;
	}
      break;
    default:
      fatal("handle_connection: Internal error.\n");
    }

  HANDLE_PACKET(closure->dispatch[msg], closure, packet);
  lsh_string_free(packet);
}

static void
connection_handle_pending(struct ssh_connection *self)
{
  while (!self->paused && !string_queue_is_empty(&self->pending))
    connection_handle_packet(self, string_queue_remove_head(&self->pending));
}

/* Deal with pausing of the connection. */
static void
do_handle_connection(struct abstract_write *w,
		     struct lsh_string *packet)
{
  CAST(ssh_connection, self, w);

  if (self->paused)
    string_queue_add_tail(&self->pending, packet);

  else if (string_queue_is_empty(&self->pending))
    connection_handle_packet(self, packet);

  else
    {
      string_queue_add_tail(&self->pending, packet);
      connection_handle_pending(self);
    }
}

DEFINE_PACKET_HANDLER(, connection_ignore_handler,
                      connection UNUSED, packet UNUSED)
{
}

DEFINE_PACKET_HANDLER(, connection_fail_handler, connection, packet UNUSED)
{
  PROTOCOL_ERROR(connection->e, NULL);
}

DEFINE_PACKET_HANDLER(, connection_unimplemented_handler, connection, packet)
{
  werror("Received packet of unimplemented type %i.\n",
	 packet->data[0]);

  C_WRITE(connection,
	  ssh_format("%c%i",
		     SSH_MSG_UNIMPLEMENTED,
		     packet->sequence_number));
}

DEFINE_PACKET_HANDLER(, connection_forward_handler, connection, packet)
{
  assert(connection->chain);
  C_WRITE(connection->chain, packet);
}


/* GABA:
   (class
     (name exc_connection_handler)
     (super exception_handler)
     (vars
       (backend object io_backend)
       (connection object ssh_connection)))
*/

static void
do_exc_connection_handler(struct exception_handler *s,
			  const struct exception *e)
{
  CAST(exc_connection_handler, self, s);

  switch (e->type)
    {
    case EXC_PROTOCOL:
      {
	CAST_SUBTYPE(protocol_exception, exc, e);

        werror("Protocol error: %z\n", e->msg);
        
	if (exc->reason)
	  C_WRITE(self->connection, format_disconnect(exc->reason, exc->super.msg, ""));
	
	EXCEPTION_RAISE(self->super.parent, &finish_read_exception);
      }
      break;

    case EXC_PAUSE_CONNECTION:
      assert(!self->connection->paused);
      self->connection->paused = 1;
      EXCEPTION_RAISE(self->super.parent,
		      make_simple_exception(EXC_PAUSE_READ, "locking connection."));
      break;

    case EXC_PAUSE_START_CONNECTION:
      /* NOTE: Raising EXC_PAUSE_START_READ will not by itself make
       * the connection start processing pending packets. Not until
       * the peer sends us more data.
       *
       * So any code that raises EXC_PAUSE_START_CONNECTION should
       * also call connection_handle_pending at a safe place. We
       * can't call it here, as we may be in the middle of the
       * handling of a packet. Installing a callout would be best. */
      
      assert(self->connection->paused);
      EXCEPTION_RAISE(self->super.parent,
		      make_simple_exception(EXC_PAUSE_START_READ, "unlocking connection."));
      
      self->connection->paused = 0;

      break;
      
    default:
      EXCEPTION_RAISE(self->super.parent, e);
    }
}

static struct exception_handler *
make_exc_connection_handler(struct ssh_connection *connection,
			  struct exception_handler *parent,
			  const char *context)
{
  NEW(exc_connection_handler, self);

  self->super.parent = parent;
  self->super.raise = do_exc_connection_handler;
  self->super.context = context;
  
  self->connection = connection;

  return &self->super;
}

struct ssh_connection *
make_ssh_connection(UINT32 flags,
		    struct address_info *peer,
		    const char *debug_comment,
		    struct command_continuation *c,
		    struct exception_handler *e)
{
  int i;

  NEW(ssh_connection, connection);

  connection->flags = flags;
  connection->peer = peer;
  
  connection->debug_comment = debug_comment;
  connection->super.write = do_handle_connection;

  /* Exception handler that sends a proper disconnect message on protocol errors */
  connection->e = make_exc_connection_handler(connection, e, HANDLER_CONTEXT);

  connection->established = c;
  
  /* Initialize instance variables */

  connection->versions[CONNECTION_SERVER]
    = connection->versions[CONNECTION_CLIENT]
    = connection->session_id = NULL;

  connection->peer_flags = 0;
  connection->user = NULL;
  
  connection->resources = empty_resource_list();
  
  connection->rec_max_packet = SSH_MAX_PACKET;

  /* Initial encryption state */
  connection->send_crypto = connection->rec_crypto = NULL;
  connection->send_mac = connection->rec_mac = NULL;
  connection->send_compress = connection->rec_compress = NULL;
  
  connection->paused = 0;
  string_queue_init(&connection->pending);
  
  connection->kex_state = KEX_STATE_INIT;

  connection->kexinits[CONNECTION_CLIENT]
    = connection->kexinits[CONNECTION_SERVER] = NULL;

  connection->literal_kexinits[CONNECTION_CLIENT]
    = connection->literal_kexinits[CONNECTION_SERVER] = NULL;
  
  for (i = 0; i < 0x100; i++)
    connection->dispatch[i] = &connection_unimplemented_handler;

  connection->dispatch[0] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_DISCONNECT] = &disconnect_handler;
  connection->dispatch[SSH_MSG_IGNORE] = &connection_ignore_handler;

  /* So far, all messages we send have to be supported. */ 
  connection->dispatch[SSH_MSG_UNIMPLEMENTED] = &connection_fail_handler;

  connection->dispatch[SSH_MSG_DEBUG] = &connection_debug_handler;

  /* Make all other known message types terminate the connection */

  connection->dispatch[SSH_MSG_SERVICE_REQUEST] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_SERVICE_ACCEPT] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_NEWKEYS] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_KEXDH_INIT] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_KEXDH_REPLY] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_USERAUTH_REQUEST] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_USERAUTH_FAILURE] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_USERAUTH_SUCCESS] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_USERAUTH_BANNER] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_USERAUTH_PK_OK] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_USERAUTH_PASSWD_CHANGEREQ] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_GLOBAL_REQUEST] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_REQUEST_SUCCESS] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_REQUEST_FAILURE] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_OPEN] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_OPEN_CONFIRMATION] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_OPEN_FAILURE] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_WINDOW_ADJUST] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_DATA] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_EXTENDED_DATA] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_EOF] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_CLOSE] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_REQUEST] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_SUCCESS] = &connection_fail_handler;
  connection->dispatch[SSH_MSG_CHANNEL_FAILURE] = &connection_fail_handler;
  
  return connection;
}

void
connection_init_io(struct ssh_connection *connection,
		   struct abstract_write *raw,
		   struct randomness *r)
{
  /* Initialize i/o hooks */
  connection->raw = raw;
  connection->write =
    make_packet_debug(
      make_packet_deflate(
	make_packet_pad(
	  make_packet_encrypt(raw, connection), 
	  connection,
	  r),
	connection),
      (connection->debug_comment
       ? ssh_format("%lz sent", connection->debug_comment)
       : ssh_format("Sent")));
}


/* GABA:
   (class
     (name connection_close_handler)
     (super lsh_callback)
     (vars
       (connection object ssh_connection)))
*/

static void
connection_die(struct lsh_callback *c)
{
  CAST(connection_close_handler, closure, c);
  
  verbose("Connection died.\n");
  
  KILL_RESOURCE_LIST(closure->connection->resources);
}

struct lsh_callback *
make_connection_close_handler(struct ssh_connection *c)
{
  NEW(connection_close_handler, closure);

  closure->connection = c;  
  closure->super.f = connection_die;

  return &closure->super;
}


/* Serialization. */

void connection_lock(struct ssh_connection *self)
{
  const struct exception pause
    = STATIC_EXCEPTION(EXC_PAUSE_CONNECTION, "locking connection.");
    
  EXCEPTION_RAISE(self->e, &pause);
}

void connection_unlock(struct ssh_connection *self)
{
  const struct exception unpause
    = STATIC_EXCEPTION(EXC_PAUSE_START_CONNECTION, "unlocking connection.");
    
  EXCEPTION_RAISE(self->e, &unpause);
}

/* gateway_commands.c
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

#include "gateway_commands.h"

#include "channel.h"
#include "connection_commands.h"
#include "debug.h"
#include "format.h"
#include "gateway_channel.h"
#include "io_commands.h"
#include "read_packet.h"
#include "ssh.h"
#include "unpad.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "gateway_commands.c.x"

/* A simplified version of do_pad() in pad.c. This one uses
 * a fixed block size and pads with zeros. */

/* FIXME: It seems very unnecessary to pad at all; we could just use
 *
 *   uint32     packet_length
 *   byte[n]    payload; n = packet_length
 *
 * But then we can reuse the plain read_packet class. */

static void
do_gateway_pad(struct abstract_write *w,
	       struct lsh_string *packet)
{
  CAST(abstract_write_pipe, closure, w);

  struct lsh_string *new;
  
  UINT32 new_size;
  UINT8 padding;

  UINT8 *data;

  /* new_size is (packet->length + 9) rounded up to a multiple of
   * block_size. But the block_size if fixed to 8 octets. */
  new_size = 8 * (2 + packet->length / 8);

  padding = new_size - packet->length - 5;
  assert(padding >= 4);

  new = ssh_format("%i%c%lr", packet->length + padding + 1,
		   padding, packet->length + padding, &data);

  assert(new->length == new_size);

  memcpy(data, packet->data, packet->length);
  memset(data + packet->length, '\0', padding);
  
  lsh_string_free(packet);

  A_WRITE(closure->next, new);
}

static struct abstract_write *
make_gateway_pad(struct abstract_write *next)
{
  NEW(abstract_write_pipe, closure);

  closure->super.write = do_gateway_pad;
  closure->next = next;

  return &closure->super;
}


/* Buffer size when reading from the socket */
#define BUF_SIZE (1<<14)

/* Blocksize when writing */
#define BLOCK_SIZE 2000

static struct ssh_connection *
gateway_make_connection(struct listen_value *lv,
			struct exception_handler *e)
{
  /* NOTE: lv->peer is usually NULL here. */
  struct ssh_connection *connection
    = make_ssh_connection(0, /* flags */
			  lv->peer, "gateway",
			  NULL, /* established_continuation */
			  make_exc_finish_read_handler(lv->fd, e, HANDLER_CONTEXT));

  /* Instead of calling connection_init_io(). */
  connection->raw =
    &io_read_write(lv->fd,
		   make_buffered_read(
		     BUF_SIZE,
		     make_read_packet(
		       make_packet_unpad(
			 connection,
			 make_packet_debug(&connection->super,
				 	  ssh_format("%lz received", connection->debug_comment))),
		       connection)),
		   BLOCK_SIZE,
		   make_connection_close_handler(connection))->write_buffer->super;
  
  connection->write = make_packet_debug(
			make_gateway_pad(connection->raw),
			ssh_format("%lz sent", connection->debug_comment));

  init_connection_service(connection);

  connection->table->open_fallback = &gateway_channel_open_forward;

  connection->dispatch[SSH_MSG_DEBUG] = &connection_forward_handler;
  connection->dispatch[SSH_MSG_IGNORE] = &connection_forward_handler;

  return connection;
}

DEFINE_COMMAND(gateway_init, a, c, e)
{
  CAST(listen_value, lv, a);

  COMMAND_RETURN(c, gateway_make_connection(lv, e));
}


/* (gateway_accept main-connection gateway-connection) */

static void
do_gateway_accept(struct command *s,
		 struct lsh_object *x,
		 struct command_continuation *c,
		 struct exception_handler *e)
{
  CAST(connection_command, self, s);
  CAST(listen_value, lv, x);

  struct ssh_connection *gateway = gateway_make_connection(lv, e);
  
  /* Kill gateway connection if the main connection goes down. */
  REMEMBER_RESOURCE(self->connection->resources, &lv->fd->super);
  
  gateway->chain = self->connection;

  COMMAND_RETURN(c, gateway);
}

DEFINE_COMMAND_SIMPLE(gateway_accept, a)
{
  CAST(ssh_connection, connection, a);
  NEW(connection_command, self);
  self->connection = connection;
  self->super.call = do_gateway_accept;

  return &self->super.super;
}


/* GABA:
   (expr
     (name gateway_setup)
     (params
       (listen object command))
     (expr
       (lambda (connection)
         (connection_remember connection
	   (listen
	     (lambda (peer)
	       (gateway_accept connection peer)))))))
*/

struct command *
make_gateway_setup(struct command *listen)
{
  CAST_SUBTYPE(command, res,
	       gateway_setup(listen));

  trace("make_gateway_setup\n");

  return res;
}

DEFINE_COMMAND_SIMPLE(gateway_setup_command, a)
{
  CAST_SUBTYPE(command, listen, a);
  CAST_SUBTYPE(command, res,
	       gateway_setup(listen));

  return &res->super;
}

/* channel_forward.h
 *
 * General channel type for forwarding data to an fd
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 2001 Balázs Scheidler, Niels Möller
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

#include "channel_forward.h"

#include "io.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

#define GABA_DEFINE
#include "channel_forward.h.x"
#undef GABA_DEFINE

/* NOTE: Adds the socket to the channel's resource list */
void
init_channel_forward(struct channel_forward *self,
		     struct lsh_fd *socket, UINT32 initial_window)
{
  assert(socket);
  
  init_channel(&self->super);

  /* The rest of the callbacks are not set up until tcpip_start_io. */

  /* NOTE: We don't need a close handler, as the channel's resource
   * list is taken care of automatically. */
  
  self->super.rec_window_size = initial_window;

  /* FIXME: Make maximum packet size configurable. */
  self->super.rec_max_packet = SSH_MAX_PACKET - SSH_CHANNEL_MAX_PACKET_FUZZ;
  
  self->socket = socket;

  REMEMBER_RESOURCE(self->super.resources, &socket->super);
}

struct channel_forward *
make_channel_forward(struct lsh_fd *socket, UINT32 initial_window)
{
  NEW(channel_forward, self);
  init_channel_forward(self, socket, initial_window);
  
  return self;
}

static void
do_channel_forward_receive(struct ssh_channel *c,
		 int type, struct lsh_string *data)
{
  CAST(channel_forward, closure, c);
  
  switch (type)
    {
    case CHANNEL_DATA:
      A_WRITE(&closure->socket->write_buffer->super, data);
      break;
    case CHANNEL_STDERR_DATA:
      werror("Ignoring unexpected stderr data.\n");
      lsh_string_free(data);
      break;
    default:
      fatal("Internal error. do_channel_forward_receive");
    }
}

static void
do_channel_forward_send_adjust(struct ssh_channel *s,
		     UINT32 i UNUSED)
{
  CAST(channel_forward, self, s);
  
  self->socket->want_read = 1;
}

static void
do_channel_forward_eof(struct ssh_channel *s)
{
  CAST(channel_forward, self, s);

  if (shutdown (self->socket->fd, SHUT_WR) < 0)
    werror("do_channel_forward_eof, shutdown failed, (errno = %i): %z\n",
	   errno, STRERROR(errno));
}


/* NOTE: Because this function is called by
 * do_open_forwarded_tcpip_continuation, the same restrictions apply.
 * I.e we can not assume that the channel is completely initialized
 * (channel_open_continuation has not yet done its work), and we can't
 * send any packets. */
void
channel_forward_start_io(struct channel_forward *channel)
{
  channel->super.receive = do_channel_forward_receive;
  channel->super.send_adjust = do_channel_forward_send_adjust;
  channel->super.eof = do_channel_forward_eof;
  
  /* Install callbacks on the local socket.
   * NOTE: We don't install any channel_io_exception_handler. */
  io_read_write(channel->socket,
		make_channel_read_data(&channel->super),
		/* FIXME: Make this configurable */
		SSH_MAX_PACKET * 10, /* self->block_size, */
		make_channel_read_close_callback(&channel->super));
  
  /* Flow control */
  channel->socket->write_buffer->report = &channel->super.super;
}


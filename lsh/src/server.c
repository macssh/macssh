/* server.c
 *
 * $Id$ */

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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "server.h"

#include "abstract_io.h"
#include "channel.h"
#include "compress.h"
#include "connection.h"
#include "debug.h"
#include "disconnect.h"
#include "format.h"
#include "keyexchange.h"
#include "read_line.h"
#include "read_packet.h"
#include "reaper.h"
#include "ssh.h"
#include "unpad.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>
#include <errno.h>


#include "server.c.x"


static struct lsh_string *
format_service_accept(int name)
{
  return ssh_format("%c%a", SSH_MSG_SERVICE_ACCEPT, name);
}

#if DATAFELLOWS_WORKAROUNDS	      
static struct lsh_string *
format_service_accept_kludge(void)
{
  return ssh_format("%c", SSH_MSG_SERVICE_ACCEPT);
}
#endif /* DATAFELLOWS_WORKAROUNDS */


/* GABA:
   (class
     (name service_handler)
     (super packet_handler)
     (vars
       (services object alist)
       (c object command_continuation)
       (e object exception_handler)))
*/

static void
do_service_request(struct packet_handler *c,
		   struct ssh_connection *connection,
		   struct lsh_string *packet)
{
  CAST(service_handler, closure, c);

  struct simple_buffer buffer;
  unsigned msg_number;
  int name;
  
  simple_buffer_init(&buffer, packet->length, packet->data);

  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_SERVICE_REQUEST)
      && parse_atom(&buffer, &name)
      && parse_eod(&buffer))
    {
      lsh_string_free(packet);
      
      if (name)
	{
	  CAST_SUBTYPE(command, service, ALIST_GET(closure->services, name));
	  if (service)
	    {
	      /* Don't accept any further service requests */
	      connection->dispatch[SSH_MSG_SERVICE_REQUEST]
		= &connection_fail_handler;

	      /* Start service */
#if DATAFELLOWS_WORKAROUNDS
	      if (connection->peer_flags & PEER_SERVICE_ACCEPT_KLUDGE)
		C_WRITE(connection, format_service_accept_kludge());
	      else
#endif /* DATAFELLOWS_WORKAROUNDS */
		C_WRITE(connection, format_service_accept(name));
	      
	      COMMAND_CALL(service, connection,
			   closure->c, closure->e);
	      return;
	    }
	}
      EXCEPTION_RAISE(connection->e,
		      make_protocol_exception(SSH_DISCONNECT_SERVICE_NOT_AVAILABLE, NULL));
      
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(connection->e, "Invalid SERVICE_REQUEST message");
    }
}

static struct packet_handler *
make_service_request_handler(struct alist *services,
			     struct command_continuation *c,
			     struct exception_handler *e)
{
  NEW(service_handler, self);

  self->super.handler = do_service_request;
  self->services = services;
  self->c = c;
  self->e = e;
  
  return &self->super;
}

     
/* GABA:
   (class
     (name offer_service)
     (super command)
     (vars
       (services object alist)))
*/

static void
do_offer_service(struct command *s,
		 struct lsh_object *x,
		 struct command_continuation *c,
		 struct exception_handler *e)
{
  CAST(offer_service, self, s);
  CAST(ssh_connection, connection, x);

  connection->dispatch[SSH_MSG_SERVICE_REQUEST]
    = make_service_request_handler(self->services, c, e);
}

struct command *make_offer_service(struct alist *services)
{
  NEW(offer_service, self);

  self->super.call = do_offer_service;
  self->services = services;

  return &self->super;
}



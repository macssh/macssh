/* handshake.c
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, 2000 Niels Möller
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

#include "handshake.h"

#include "compress.h"
#include "connection.h"
#include "debug.h"
#include "format.h"
#include "io.h"
#include "keyexchange.h"
#include "read_line.h"
#include "read_packet.h"
#include "ssh.h"
#include "unpad.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

#define GABA_DEFINE
#include "handshake.h.x"
#undef GABA_DEFINE

#include "handshake.c.x"

/* GABA:
   (class
     (name connection_line_handler)
     (super line_handler)
     (vars
       (connection object ssh_connection)
       ; Needed for fallback.
       (fd . int)
       (fallback object ssh1_fallback)))
*/

/* Returns -1 if the line is not the start of a SSH handshake,
 * 0 if the line appears to be an SSH handshake, but with bogus version fields,
 * or 1 if the line was parsed sucessfully. */
static int
split_version_string(UINT32 length, UINT8 *line,
		     UINT32 *protover_len, UINT8 **protover,
		     UINT32 *swver_len, UINT8 **swver,
		     UINT32 *comment_len, UINT8 **comment)
{
  UINT8 *sep;

  if (length < 4 || memcmp(line, "SSH-", 4) != 0)
    {
      /* not an ssh identification string */
      return -1;
    }
  line += 4; length -= 4;
  sep = memchr(line, '-', length);
  if (!sep)
    {
      return 0;
    }
  *protover_len = sep - line;
  *protover = line;
  
  line = sep + 1;
  length -= *protover_len + 1;

  /* FIXME: The spec is not clear about the separator here. Can there
   * be other white space than a single space character? */
  sep = memchr(line, ' ', length);
  if (!sep)
    {
      *swver_len = length;
      *swver = line;
      *comment = NULL;
      *comment_len = 0;
      return 1;
    }

  *swver_len = sep - line;
  *swver = line;
  *comment = sep + 1;
  *comment_len = length - *swver_len - 1;
  return 1;
}

static void
do_line(struct line_handler **h,
	struct read_handler **r,
	UINT32 length,
	UINT8 *line,
	struct exception_handler *e)
{
  CAST(connection_line_handler, closure, *h);
  UINT32 protover_len, swver_len, comment_len;
  UINT8 *protover, *swver, *comment;

  struct ssh_connection *connection = closure->connection;
  int mode = connection->flags & CONNECTION_MODE;
  
  switch(split_version_string(length, line, 
			      &protover_len, &protover, 
			      &swver_len, &swver,
			      &comment_len, &comment))
    {
    case 1:
      {
	/* Parse and remember format string */
	/* NOTE: According to the spec, there's no reason for the server
	 * to accept a client that wants version 1.99. But Datafellow's
	 * ssh2 client does exactly that, so we have to support it. And
	 * I don't think it causes any harm. */
	
	if ( ((protover_len >= 3) && !memcmp(protover, "2.0", 3))
	     || ((protover_len == 4) && !memcmp(protover, "1.99", 4)) )
	  {
	    struct read_handler *new;	  
#if WITH_SSH1_FALLBACK
	    if (closure->fallback)
	      {
		assert(mode == CONNECTION_SERVER);
	      
		/* Sending keyexchange packet was delayed. Do it now */
		initiate_keyexchange(connection);
	      }
#endif /* WITH_SSH1_FALLBACK */

#if DATAFELLOWS_WORKAROUNDS
	    if ( (swver_len >= 6) && !memcmp(swver, "2.0.", 4)
		 /* FIXME: Perhaps do a numerical comparison here? */
		 && (memcmp(swver + 4, "13", 2) <= 0) )
	      {
		connection->peer_flags
		  |= (PEER_SSH_DSS_KLUDGE | PEER_SERVICE_ACCEPT_KLUDGE
		      | PEER_USERAUTH_REQUEST_KLUDGE | PEER_SEND_NO_DEBUG 
		      | PEER_X11_OPEN_KLUDGE);
	      }
	    else if ( (swver_len >= 5) && !memcmp(swver, "2.1.0", 5) )
	      {
		connection->peer_flags
		  |= (PEER_SSH_DSS_KLUDGE
		      | PEER_USERAUTH_REQUEST_KLUDGE | PEER_SEND_NO_DEBUG);
		verbose("ssh.com 2.1.0\n");
	      }
#endif /* DATAFELLOWS_WORKAROUNDS */ 
#if MACOS
	    if ( (swver_len >= 6) && !memcmp(swver, "2.0.13", 6) )
		  connection->peer_flags |= PEER_WRONG_PK_OK;
#endif
	    new = 
	      make_read_packet(
		make_packet_unpad(
		  connection,
		  make_packet_inflate(
		    make_packet_debug(&connection->super,
				      (connection->debug_comment
				       ? ssh_format("%lz received", connection->debug_comment)
				       : ssh_format("Received"))),
		    connection
		    )
		  ),
		connection
		);
	    
	    connection->versions[!mode]
	      = ssh_format("%ls", length, line);

	    verbose("Client version: %pS\n"
		    "Server version: %pS\n",
		    connection->versions[CONNECTION_CLIENT],
		    connection->versions[CONNECTION_SERVER]);

	    *r = new;
	    return;
	  }
#if WITH_SSH1_FALLBACK      
      else if (closure->fallback
	       && (protover_len >= 2)
	       && !memcmp(protover, "1.", 2))
	{
	  *h = NULL;
	  SSH1_FALLBACK(closure->fallback,
			closure->fd,
			length, line,
			e);

	  return;
	}
#endif /* WITH_SSH1_FALLBACK */
      else
	{
	  werror("Unsupported protocol version: %ps\n",
		 length, line);

	  *h = NULL;

	  EXCEPTION_RAISE(connection->e,
			  make_protocol_exception
			  (SSH_DISCONNECT_PROTOCOL_VERSION_NOT_SUPPORTED,
			   NULL));
	  return;
	}
	fatal("Internal error!\n");
      }
    case 0:
      werror("Incorrectly formatted version string: %s\n", length, line);
      KILL(closure);
      *h = NULL;
      
      PROTOCOL_ERROR(connection->e,
		     "Incorrectly version string.");
      
      return;
    case -1:
      /* Display line */
      werror("%ps\n", length, line);

      /* Read next line */
      break;
    default:
      fatal("Internal error!\n");
    }
}

static struct read_handler *
make_connection_read_line(struct ssh_connection *connection, 
			  int fd,
			  struct ssh1_fallback *fallback)
{
  NEW(connection_line_handler, closure);

  closure->super.handler = do_line;
  closure->connection = connection;
  closure->fd = fd;
  closure->fallback = fallback;
  return make_read_line(&closure->super, connection->e);
}


struct handshake_info *
make_handshake_info(UINT32 flags,
		    const char *id_comment,
		    const char *debug_comment,
		    UINT32 block_size,
		    struct randomness *r,
		    struct alist *algorithms,
		    struct ssh1_fallback *fallback)
{
  NEW(handshake_info, self);
  self->flags = flags;
  self->id_comment = id_comment;
  self->debug_comment = debug_comment;
  self->block_size = block_size;
  self->random = r;
  self->algorithms = algorithms;
  self->fallback = fallback;

  return self;
}

/* (handshake handshake_info make_kexinit extra listen_value) -> connection 
 *
 * extra is passed on to KEYEXCHANGE_INIT, it is typically a set of
 * private keys (for the server) or a hostkey database (for the
 * client). */

/* GABA:
   (class
     (name handshake_command_2)
     (super command)
     (vars
       (info object handshake_info)
       (init object make_kexinit)
       (extra object lsh_object)))
*/

/* Buffer size when reading from the socket */
#define BUF_SIZE (1<<14)

static void
do_handshake(struct command *s,
	     struct lsh_object *x,
	     struct command_continuation *c,
	     struct exception_handler *e)
{
  CAST(handshake_command_2, self, s);
  CAST(listen_value, lv, x);
  struct lsh_string *version;
  struct ssh_connection *connection;
  int mode = self->info->flags & CONNECTION_MODE;
  
  verbose("Initiating handshake with %S\n", lv->peer->ip);
  
  switch (mode)
    {
    case CONNECTION_CLIENT:
      version = ssh_format("SSH-%lz-%lz %lz",
			   CLIENT_PROTOCOL_VERSION,
			   SOFTWARE_CLIENT_VERSION,
			   self->info->id_comment);
      break;
    case CONNECTION_SERVER:
#if WITH_SSH1_FALLBACK
      if (self->info->fallback)
	{
	  version =
	    ssh_format("SSH-%lz-%lz %lz",
		       SSH1_SERVER_PROTOCOL_VERSION,
		       SOFTWARE_SERVER_VERSION,
		       self->info->id_comment);
	}
      else
#endif
	version =
	  ssh_format("SSH-%lz-%lz %lz",
		     SERVER_PROTOCOL_VERSION,
		     SOFTWARE_SERVER_VERSION,
		     self->info->id_comment);
      break;
    default:
      fatal("do_handshake: Internal error\n");
    }
  
  /* Installing the right exception handler is a little tricky. The
   * passed in handler is typically the top-level handler provided by
   * lsh.c or lshd.c. On top of this, we add the io_exception_handler
   * which takes care of EXC_FINISH_READ exceptions and closes the
   * connection's socket. And on top of this, we have a
   * connection_exception handler, which takes care of EXC_PROTOCOL
   * exceptions, sends a disconnect message, and then raises an
   * EXC_FINISH_READ exception. */
  
  connection = make_ssh_connection
    (self->info->flags,
     lv->peer, self->info->debug_comment, 
     c,
     make_exc_finish_read_handler(lv->fd, e, HANDLER_CONTEXT));
  
  connection_init_io
    (connection, 
     &io_read_write(lv->fd,
		    make_buffered_read
		    (BUF_SIZE,
		     make_connection_read_line(connection, 
					       lv->fd->fd, self->info->fallback)),
		    self->info->block_size,
		    make_connection_close_handler(connection))
     ->write_buffer->super,
     self->info->random);

  connection->versions[mode] = version;
  connection->kexinits[mode] = MAKE_KEXINIT(self->init); 
  connection->dispatch[SSH_MSG_KEXINIT]
    = make_kexinit_handler(self->init,
			   self->extra, self->info->algorithms);

#if WITH_SSH1_FALLBACK
  /* In this mode the server SHOULD NOT send carriage return character (ascii
   * 13) after the version identification string.
   *
   * Furthermore, it should not send any data after the identification string,
   * until the client's identification string is received. */
  if (self->info->fallback)
    {
      A_WRITE(connection->raw,
	      ssh_format("%lS\n", version));
      return;
    }
#endif /* WITH_SSH1_FALLBACK */

  A_WRITE(connection->raw,
	  ssh_format("%lS\r\n", version));
  
  initiate_keyexchange(connection);
}

static struct lsh_object *
collect_handshake_3(struct collect_info_3 *info UNUSED,
		    struct lsh_object *h,
		    struct lsh_object *i,
		    struct lsh_object *extra)
{
  CAST(handshake_info, hinfo, h);
  CAST_SUBTYPE(make_kexinit, init, i);
  NEW(handshake_command_2, self);

#if 0
  assert(!info->next);
#endif
  self->super.call = do_handshake;
  self->info = hinfo;
  self->init = init;
  self->extra = extra;

  return &self->super.super;
}

static struct collect_info_3 collect_info_handshake_3 =
STATIC_COLLECT_3_FINAL(collect_handshake_3);

static struct collect_info_2 collect_info_handshake_2 =
STATIC_COLLECT_2(&collect_info_handshake_3);

struct collect_info_1 handshake_command =
STATIC_COLLECT_1(&collect_info_handshake_2);

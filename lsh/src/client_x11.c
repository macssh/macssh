/* client_x11.c
 *
 * Client side of X11 forwaarding.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2001 Niels Möller
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

#include "client.h"

#include "channel_forward.h"
#include "format.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"
#include "xauth.h"

#include "client_x11.c.x"

#include <assert.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

/* First port number for X, according to RFC 1013 */
#define X11_BASE_PORT 6000

#define X11_WINDOW_SIZE 10000

#define X11_COOKIE_LENGTH 16

/* ;; GABA:
   (class
     (name client_x11_auth_info)
     (vars
       ; Fake MIT-COOKIE-1
       (fake string)

       ; Real authentication info
       (name string)
       (auth string)))
*/

/* FIXME: Perhaps merge auth_info directly into this struct */
/* GABA:
   (class
     (name client_x11_display)
     (vars
       (address_length . socklen_t)
       (address space "struct sockaddr")

       ; Default screen
       (screen . UINT16)

       ; Fake MIT-COOKIE-1
       (fake string)

       ; Real authentication info
       (auth_name string)
       (auth_data string)))
*/
     
/* GABA:
   (class
     (name channel_open_x11)
     (super channel_open)
     (vars
       (backend object io_backend)))
*/

/* GABA:
   (class
     (name client_x11_channel)
     (super channel_forward)
     (vars
       (display object client_x11_display)
       (state . int)
       (little_endian . int)
       (name_length . unsigned)
       (auth_length . unsigned)
       (i . UINT32)
       (buffer string)))
*/

/* This function is responsible for checking the fake cookie, and
 * replacing it with the real one.
 *
 * It intercepts the first packet sent by the client, which has the
 * following format:
 *
 * Type                        Possible or typical values
 *
 * uint8_t  byte-order         'B' (big endian) or 'L' (little endian)
 * uint8_t  pad                0
 * uint16_t major-version      Usually 11
 * uint16_t minor-version      Usually 0.
 * uint16_t name_length        18
 * uint16_t auth_length        16
 * uint16_t pad                What's this?
 * uint8_t [name_length] name  "MIT-MAGIC-COOKIE-1"
 * uint8_t [auth_length] auth  Authentication data
 *
 * The last fields; name and auth, are padded to a multiple of four octets.
 *
 * The typical setup packet, with a 16-octet cookie, is 48 octets.
 */

/* Observed data:
 *
 *  $10 = {0x42, 0x0, 0x0, 0xb, 0x0, 0x0, 0x0, 0x12, 0x0, 0x10, 0x0, 0xa4, 0x4d, 
 *  0x49, 0x54, 0x2d, 0x4d, 0x41, 0x47, 0x49, 0x43, 0x2d, 0x43, 0x4f, 0x4f, 
 *  0x4b, 0x49, 0x45, 0x2d, 0x31, 0xff, 0xf7, 0x8b, 0x1e, 0x2c, 0xa0, 0x98, 
 *  0x11, 0x27, 0x82, 0xa9, 0x0, 0x2d, 0xc4, 0x68, 0x7f, 0x66, 0x2b}
 *
 */

/* From Pike's X.pmod:
 *
 *     /( Always uses network byteorder (big endian) 
 *     string msg = sprintf("B\0%2c%2c%2c%2c\0\0%s%s",
 * 			    11, 0,
 * 			    strlen(auth_data->name), strlen(auth_data->data),
 * 			    ._Xlib.pad(auth_data->name), ._Xlib.pad(auth_data->data));
 */

#define MIT_COOKIE_NAME "MIT-MAGIC-COOKIE-1"
#define MIT_COOKIE_NAME_LENGTH 18
#define MIT_COOKIE_LENGTH 16

#define X11_SETUP_VERSION_LENGTH 6
#define X11_SETUP_HEADER_LENGTH 12

/* The size of a connection setup message with a 16 octet
 * MIT-MAGIC-COOKIE-1. Using such a low value leaks the information
 * that we expect a 16-octet cookie, but I don't think that's areal
 * problem. */
#define X11_SETUP_MAX_LENGTH 48

enum { CLIENT_X11_START,
       CLIENT_X11_GOT_LENGTHS,
       CLIENT_X11_OK,
       CLIENT_X11_DENIED
};

/* And the other, little-endian, byteorder */
#define LE_READ_UINT16(p)			\
(  (((uint32_t) (p)[1]) << 8)			\
 |  ((uint32_t) (p)[0]))

#define LE_WRITE_UINT16(p, i)			\
do {						\
  (p)[1] = ((i) >> 8) & 0xff;			\
  (p)[0] = (i) & 0xff;				\
} while(0)

/* FIXME: This function is a little ugly. It would get cleaner if we
 * just replaced the channel's receive function pointer with NULL on
 * failure and do_channel_forward_receive on success. */
static void
do_client_channel_x11_receive(struct ssh_channel *s,
                              int type, struct lsh_string *data)
{
  CAST(client_x11_channel, self, s);

  if (type != CHANNEL_DATA)
    {
      werror("Ignoring unexpected stderr data.\n");
      lsh_string_free(data);
    }
  else switch (self->state)
    {
    case CLIENT_X11_OK:
      A_WRITE(&self->super.socket->write_buffer->super, data);
      break;
        
    fail:
      channel_close(&self->super.super);
      self->state = CLIENT_X11_DENIED;
        
      break;
        
    case CLIENT_X11_DENIED:
      /* Any data on the channel should be stopped before we get
       * here; the CHANNEL_SENT_CLOSE should be set. */
      fatal("Internal error!\n");

    default:
      {
        /* Copy data to buffer */
        UINT32 left = self->buffer->length - self->i;
          
        /* The small initial window size should ensure that we don't get
         * more data. */
        assert(data->length <= left);
          
        memcpy(self->buffer->data + self->i, data->data,
               data->length);
        self->i += data->length;
        lsh_string_free(data);
	
        switch (self->state)
          {
          case CLIENT_X11_START:
            /* We need byte-order, major, minor and name_length,
             * which is 6 octets */
               
            if (self->i < X11_SETUP_HEADER_LENGTH)
              break;

            self->state = CLIENT_X11_GOT_LENGTHS;

            switch (self->buffer->data[0])
              {
              case 'B': /* Big endian */
              case 'b': /* Big endian */
                self->little_endian = 0;
                self->name_length = READ_UINT16(self->buffer->data + 6);
                self->auth_length = READ_UINT16(self->buffer->data + 8);
                break;
              case 'L': /* Little endian */
              case 'l': /* Little endian */
                self->little_endian = 1;
                self->name_length = LE_READ_UINT16(self->buffer->data + 6);
                self->auth_length = LE_READ_UINT16(self->buffer->data + 8);
                break;
              default:
                werror("client_x11.c: Bad endian indicator.\n");
                goto fail;
              }
            if ( (self->name_length > 20)
                 || (self->auth_length > 16) )
              {
                werror("client_x11.c: Too long auth name or cookie\n");
                goto fail;
              }
            
            /* Fall through */
            
          case CLIENT_X11_GOT_LENGTHS:
            {
              const unsigned pad_length[4] = { 0, 3, 2, 1 };

#define PAD(l) (pad_length[ (l) % 4])
              
              UINT32 auth_offset = X11_SETUP_HEADER_LENGTH
                + self->name_length + PAD(self->name_length);

              UINT32 length = auth_offset + self->auth_length
                + pad_length[self->auth_length % 4]; 
                
              if (self->i < length)
                break;

              debug("Received cookie of type `%ps': %xs\n",
                    self->name_length, self->buffer->data + X11_SETUP_HEADER_LENGTH,
                    self->auth_length, self->buffer->data + auth_offset);

              /* Ok, now we have the connection setup message. Check if it's ok. */
              if ( (self->name_length == MIT_COOKIE_NAME_LENGTH)
                   && !memcmp(self->buffer->data + X11_SETUP_HEADER_LENGTH,
                              MIT_COOKIE_NAME, MIT_COOKIE_NAME_LENGTH)
                   && lsh_string_eq_l(self->display->fake,
                                      self->auth_length,
                                      self->buffer->data + auth_offset))
                {
                  struct lsh_string *msg;
                  UINT8 lengths[4];
                  static const UINT8 pad[3] = { 0, 0, 0 };
                  
                  /* Cookies match! */
                  verbose("client_x11: Allowing X11 connection; cookies match.\n");
                  
                  if (self->little_endian)
                    {
                      LE_WRITE_UINT16(lengths, self->display->auth_name->length);
                      LE_WRITE_UINT16(lengths + 2, self->display->auth_data->length);
                    }
                  else
                    {
                      WRITE_UINT16(lengths, self->display->auth_name->length);
                      WRITE_UINT16(lengths + 2, self->display->auth_data->length);
                    }

                  /* FIXME: Perhaps it would be easier to build the message by hand than
                   * using ssh_format? */
                  /* Construct the real setup message. */
                  msg = ssh_format("%ls%ls%c%c%ls%ls%ls%ls",
                                   X11_SETUP_VERSION_LENGTH, self->buffer->data,
                                   4, lengths,
                                   0, 0,
                                   self->display->auth_name->length,
                                   self->display->auth_name->data,
                                   PAD(self->display->auth_name->length), pad,
                                   self->display->auth_data->length,
                                   self->display->auth_data->data,
                                   self->i - length,
                                   self->buffer + self->i);

                  lsh_string_free(self->buffer);
                  self->buffer = NULL;

                  /* Bump window size */
                  channel_start_receive(&self->super.super, X11_WINDOW_SIZE - msg->length);

                  debug("client_x11.c: Sending real X11 setup message: %xS\n",
                        msg);
                  
                  /* Send real x11 connection setup message. */
                  A_WRITE(&self->super.socket->write_buffer->super, msg);

                  self->state = CLIENT_X11_OK;
                }
              else
                {
                  werror("client_x11: X11 connection denied; bad cookie.\n");
                  goto fail;
                }
              break;
#undef PAD
            }
          default:
            fatal("Internal error. do_client_channel_x11_receive");
            break;
          }
      }
      break;
    }
}

static struct client_x11_channel *
make_client_x11_channel(struct lsh_fd *fd,
			struct client_x11_display *display)
{
  NEW(client_x11_channel, self);

  /* Use a limited window size for the setup */
  init_channel_forward(&self->super, fd, X11_SETUP_MAX_LENGTH);
  self->display = display;
  self->state = 0;
  self->buffer = lsh_string_alloc(X11_SETUP_MAX_LENGTH);

  return self;
}

/* GABA:
   (class
     (name channel_open_x11_continuation)
     (super command_continuation)
     (vars
       (display object client_x11_display)
       (up object command_continuation)))
*/

static void
do_channel_open_x11_continuation(struct command_continuation *s,
				 struct lsh_object *a)
{
  CAST(channel_open_x11_continuation, self, s);
  CAST(lsh_fd, fd, a);
  
  struct client_x11_channel *channel = make_client_x11_channel(fd, self->display);
  channel_forward_start_io(&channel->super);
  channel->super.super.receive = do_client_channel_x11_receive;

  COMMAND_RETURN(self->up, channel);
}
				     
static struct command_continuation *
make_channel_open_x11_continuation(struct client_x11_display *display,
				   struct command_continuation *up)
{
  NEW(channel_open_x11_continuation, self);
  self->super.c = do_channel_open_x11_continuation;
  self->display = display;
  self->up = up;

  return &self->super;
}

/* Exception handler that promotes connect errors to CHANNEL_OPEN
 * exceptions */

static void
do_exc_x11_connect_handler(struct exception_handler *s,
			   const struct exception *e)
{
  switch(e->type)
    {
    case EXC_IO_CONNECT:
      EXCEPTION_RAISE(s->parent,
		      make_channel_open_exception(SSH_OPEN_CONNECT_FAILED,
						  e->msg));
      break;
    default:
      EXCEPTION_RAISE(s->parent, e);
    }
}

static struct exception_handler *
make_exc_x11_connect_handler(struct exception_handler *parent,
			     const char *context)
{
  return make_exception_handler(do_exc_x11_connect_handler, parent, context);
}

static void
do_channel_open_x11(struct channel_open *s,
		    struct ssh_connection *connection,
		    struct channel_open_info *info UNUSED,
		    struct simple_buffer *args,
		    struct command_continuation *c,
		    struct exception_handler *e)
{
  CAST(channel_open_x11, self, s);

  UINT32 originator_length;
  const UINT8 *originator;
  UINT32 originator_port;

  if (parse_string(args, &originator_length, &originator)
      && parse_uint32(args, &originator_port) 
      && parse_eod(args))
    {
      struct client_x11_display *display = connection->table->x11_display;
      
      verbose("x11 connection attempt, originator: %s:%i\n",
	      originator_length, originator, originator_port);

      
      if (display)
	{
	  struct lsh_fd *fd
	    = io_connect(self->backend,
			 display->address,
			 display->address_length,
			 make_channel_open_x11_continuation(display,
							    c),
			 make_exc_x11_connect_handler(e, HANDLER_CONTEXT));

	  if (fd)
	    REMEMBER_RESOURCE(connection->resources, &fd->super);
	  else
	    EXCEPTION_RAISE(e, 
		      make_channel_open_exception(SSH_OPEN_CONNECT_FAILED,
						  STRERROR(errno)));
	  
 	  /* FIXME: To handle single-connection feature,
	   * remove the display here. */
	}
      else
	EXCEPTION_RAISE(e, make_channel_open_exception
			(SSH_OPEN_CONNECT_FAILED,
			 "No X11 forwarding has been requested."));
    }
  else
    {
      werror("do_channel_open_x11: Invalid message!\n");
      PROTOCOL_ERROR(connection->e, "Invalid CHANNEL_OPEN x11 message.");
    }
}

struct channel_open *
make_channel_open_x11(struct io_backend *backend)
{
  NEW(channel_open_x11, self);

  self->super.handler = do_channel_open_x11;
  self->backend = backend;

  return &self->super;
}


/* Format is host:display.screen, where display and screen are numbers */
static int
parse_display(struct client_x11_display *self, const char *display)
{
  struct lsh_string *host;

  const char *num;
  unsigned num_length;
  unsigned display_num;
  
  /* Get host name */
  if (display[0] == ':')
    {
      /* Local transport */
      host = NULL;
      display++;
    }
  else
    {
      const char *separator = strchr(display, ':');
      size_t length;

      if (!separator)
	return 0;

      length = separator - display;
      host = ssh_format("%ls", length, display);
      
      display = separator + 1;
    }
  
  /* Get display number */
  {
    char *end;

    num = display;
    
    display_num = strtol(display, &end, 0);

    num_length = end - num;

    if (!num_length)
      {
	lsh_string_free(host);
	return 0;
      }
    
    if (!*end)
      /* Default screen number */
      self->screen = 0;
    else if (*end != '.')
      {
	lsh_string_free(host);
	return 0;
      }
    else
      {
        display = end + 1;
        self->screen = strtol(display, &end, 0);

        if (*end)
          {
            lsh_string_free(host);
            return 0;
          }
      }
  }
  
  if (host)
    {
      /* NOTE: How do we support IPv6 displays? I have no idea how
       * that would work with xauth. Actually, xauth ought to use DNS
       * names rather than IP addresses. */
      struct address_info *a = make_address_info(host, X11_BASE_PORT + display_num);
      
      self->address = address_info2sockaddr(&self->address_length, a, NULL, 1);

      KILL(a);

      if (!self->address)
	return 0;
    }
  else
    {
      /* Local transport */
      struct lsh_string *name = ssh_format("/tmp/.X11-unix/X%di", display_num);
      struct sockaddr_un *sa;

      verbose("Using local X11 transport `%pS'\n", name);
      
      self->address_length = offsetof(struct sockaddr_un, sun_path) + name->length;
      sa = lsh_space_alloc(self->address_length);
      sa->sun_family = AF_UNIX;
      memcpy(sa->sun_path, name->data, name->length);

      lsh_string_free(name);
      self->address = (struct sockaddr *) sa;
    }

  if (!xauth_lookup(self->address,
                    num_length, num,
                    &self->auth_name,
                    &self->auth_data))
    {
      /* Fallback: Don't use xauth, and hope that the X server uses
       * xhost to let us in anyway. */
      werror("Can't find any xauth information for X11 display.\n");

      self->auth_name = ssh_format("");
      self->auth_data = ssh_format("");
    }
  
  return 1;
}

struct client_x11_display *
make_client_x11_display(const char *display, struct lsh_string *fake)
{
  NEW(client_x11_display, self);

  self->fake = fake;
  
  if (!parse_display(self, display))
    {
      werror("Can't parse X11 display: `%z'\n", display);
      KILL(self);
      return NULL;
    }
  
  return self;
}

/* GABA:
   (class
     (name client_x11_display_resource)
     (super resource)
     (vars
       (connection object ssh_connection)
       (display object client_x11_display)))
*/

static void
do_kill_x11_display(struct resource *s)
{
  CAST(client_x11_display_resource, self, s);

  if (self->super.alive)
    {
      self->super.alive = 0;

      if (self->connection->table->x11_display == self->display)
	self->connection->table->x11_display = NULL;
      else
	werror("do_kill_x11_display: Display has been replaced.\n");
    }
}

static struct resource *
make_client_x11_display_resource(struct ssh_connection *connection,
				 struct client_x11_display *display)
{
  NEW(client_x11_display_resource, self);
  init_resource(&self->super, do_kill_x11_display);

  self->connection = connection;
  self->display = display;

  return &self->super;
}

/* GABA:
   (class
     (name request_x11_continuation)
     (super command_continuation)
     (vars
       (connection object ssh_connection)
       (display object client_x11_display)
       (up object command_continuation)))
*/

static void
do_request_x11_continuation(struct command_continuation *s,
			    struct lsh_object *a)
{
  CAST(request_x11_continuation, self, s);
  CAST_SUBTYPE(ssh_channel, channel, a);

  verbose("X11 request succeeded\n");

  if (self->connection->table->x11_display)
    werror("client_x11.c: Replacing old x11 forwarding.\n");

  self->connection->table->x11_display = self->display;

  REMEMBER_RESOURCE(channel->resources,
		    make_client_x11_display_resource(self->connection,
						     self->display));

  COMMAND_RETURN(self->up, a);
}

static struct command_continuation *
make_request_x11_continuation(struct ssh_connection *connection,
			      struct client_x11_display *display,
			      struct command_continuation *up)
{
  NEW(request_x11_continuation, self);
  self->super.c = do_request_x11_continuation;

  self->connection = connection;
  self->display = display;
  self->up = up;

  return &self->super;
}

/* GABA:
   (class
     (name request_x11_forward_command)
     (super channel_request_command)
     (vars
       (connection object ssh_connection)
       (display object client_x11_display)))
*/

static struct lsh_string *
do_format_request_x11_forward(struct channel_request_command *s,
			      struct ssh_channel *channel,
			      struct command_continuation **c)
{
  CAST(request_x11_forward_command, self, s);

  verbose("Requesting X11 forwarding.\n");
  
  *c = make_request_x11_continuation(channel->connection,
				     self->display, *c);

  /* NOTE: The cookie is hex encoded, appearantly so that it can be
   * passed directly to the xauth command line. That's really ugly,
   * but it's how the other ssh implementations do it. */
  
  return format_channel_request(ATOM_X11_REQ, channel, 1, "%c%s%xS%i",
				0, /* Single connection not supported */
				MIT_COOKIE_NAME_LENGTH, MIT_COOKIE_NAME,
				self->display->fake,
				self->display->screen);
}

/* Consumes fake */
struct command *
make_forward_x11(const char *display_string,
		 struct randomness *random)
{
  struct lsh_string *fake = lsh_string_alloc(X11_COOKIE_LENGTH);
  struct client_x11_display *display;

  RANDOM(random, fake->length, fake->data);

  debug("Generated X11 fake cookie %xS\n", fake);
  
  /* This deallocates fake if it fails. */
  display = make_client_x11_display(display_string, fake);

  if (display)
    {
      NEW(request_x11_forward_command, self);
      self->super.super.call = do_channel_request_command;
      self->super.format_request = do_format_request_x11_forward;

      self->display = display;
      return &self->super.super;
    }
  else
    return NULL;
}

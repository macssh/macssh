/* xauth.c
 *
 * Xauth parsing.
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

#include "xauth.h"

#include "format.h"
#include "werror.h"

#if HAVE_X11_XAUTH_H
#include <X11/Xauth.h>
#endif

#include <unistd.h>
#include <netinet/in.h>

#if HAVE_LIBXAU

static int xauth_process(const char *filename,
			 unsigned family,
			 unsigned address_length, const char *address,
			 unsigned number_length, const char *number,
			 struct lsh_string **name,
			 struct lsh_string **data)
{
  FILE *f = fopen(filename, "rb");
  Xauth *xa;

  if (!f)
    return 0;

  while ( (xa = XauReadAuth(f)) )
    {
      debug("xauth: family: %i\n", xa->family);
      debug("       address: %ps\n", xa->address_length, xa->address);
      debug("       display: %s\n", xa->number_length, xa->number);
      debug("       name: %s\n", xa->name_length, xa->name);
      debug("       data length: %i\n", xa->data_length);
      
      if ( (xa->family == family)
	   && (xa->address_length == address_length)
	   && (xa->number_length == number_length)
	   && !memcmp(xa->address, address, address_length)
	   && !memcmp(xa->number, number, number_length) )
	{
	  *name = ssh_format("%ls", xa->name_length, xa->name);
	  *data = ssh_format("%ls", xa->data_length, xa->data);

	  XauDisposeAuth(xa);
	  fclose(f);

	  return 1;
	}
    }
  XauDisposeAuth(xa);
  fclose(f);
  
  return 0;
}

int
xauth_lookup(struct sockaddr *sa,
             unsigned number_length,
             const char *number,
             struct lsh_string **name,
             struct lsh_string **data)
{
  int res = 0;
  unsigned family;

  const char *address;
  unsigned address_length;
  
  /* FIXME: Use xgethostbyname */
#define HOST_MAX 200
  char host[HOST_MAX];
  
  const char *filename = XauFileName();
  
  if (!filename)
    return 0;

  switch(sa->sa_family)
    {
    case AF_UNIX:
      if (gethostname(host, sizeof(host) - 1) < 0)
	return 0;
      address = host;
      address_length = strlen(host);
      family = FamilyLocal;
      break;

    case AF_INET:
      {
	struct sockaddr_in *s = (struct sockaddr_in *) sa;
	
	address = (char *) &s->sin_addr;
	address_length = 4;
	family = 0;
	break;
      }

#if WITH_IPV6
    case AF_INET6:
      {
	struct sockaddr_in6 *s = (struct sockaddr_in6 *) sa;
	
	address = (char *) s->sin6_addr;
	address_length = 16;
	family = 0;
	break;
      }
#endif
    default:
      return 0;
    }
  
  /* 5 retries, 1 second each */
  if (XauLockAuth(filename, 5, 1, 0) != LOCK_SUCCESS)
    return 0;

  res = xauth_process(filename,
		      family,
		      address_length, address,
		      number_length, number,
		      name, data);
    
  XauUnlockAuth(filename);

  return res;
}
#else /* !HAVE_LIBXAU */

int
xauth_lookup(struct sockaddr *address UNUSED,
             unsigned display_length UNUSED,
             const char *display UNUSED,
             struct lsh_string **name UNUSED,
             struct lsh_string **auth UNUSED)
{
  return 0;
}
#endif /* !HAVE_LIBXAU */

#if 0
#include "channel.h"

#include "format.h"
#include "werror.h"
#include "xalloc.h"

#include "xauth.c.x"

#include <assert.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

/* FIXME: Should move somewhere else. */
static struct lsh_string *
read_file(int fd, UINT32 limit)
{
  struct lsh_string *s = lsh_string_alloc(limit);
  UINT32 i = 0;

  while ( i < s->length)
    {
      int res = read(fd, s->data + i, s->length - i);
      if (res < 0)
	{
	  lsh_string_free(s);
	  return NULL;
	}
      else if (!res)
	break;
      else i += res;
    }

  s->length = i;
  return s;
}

/* Are the XAUTH file format and locking conventions documented anywhere?
 *
 * This file tryes to follow common practice. For locking the file
 * ".Xauthority", create a file ".Xauthority-c" (using exclusive
 * mode), and a hardlink ".Xauthority-l" to the same file.
 *
 * Fileformat: .Xauthority is a sequence of entries of the following format:
 *
 * uint16_t                family
 * uint16_t                address_length
 * uint8_t[address_length] address
 * uint16_t                display_length
 * uint8_t[display_length] display  (a number, formatted as ascii digits)
 * uint16_t                name_length
 * uint8_t[name_length]    name  (e.g "MIT-MAGIC-COOKIE-1")
 * uint16_t                data_length
 * uint8_t[data_length]    data
 *
 * All lengths in network byte order.
 *
 * Valid values for family are
 *
 *     0             (address is an ip address)
 *   252 FamilyLocalHost
 *   253 FamilyKrb5Principal
 *   254 FamilyNetname
 *   256 FamilyLocal (e.g. host/unix)
 * 65535 FamilyWild
 */

/* xauth-style file locking */
/* ;;GABA:
   (class
     (name xauth_lock)
     (super resource)
     (vars
       (c_name string)
       (l_name string)))
*/

static void
do_xauth_lock_kill(struct resource *s)
{
  CAST(xauth_lock, self, s);
  if (self->super.alive)
    {
      self->super.alive = 0;
      unlink (lsh_get_cstring(self->c_name));
      unlink (lsh_get_cstring(self->l_name));
    }
}

static struct resource *
xauth_lock(const char *name)
{
  NEW(xauth_lock, self);
  int f;
  
  init_resource(&self->super, do_xauth_lock_kill);

  self->c_name = ssh_format("%lz-c", name);
  self->l_name = ssh_format("%lz-l", name);

  f = open(lsh_get_cstring(self->c_name),
	   O_CREAT | O_EXCL | O_WRONLY, 0666);

  if (f >= 0)
    {
      close(f);
      if (link(lsh_get_cstring(self->c_name),
	       lsh_get_cstring(self->l_name)) == 0)
	/* Success! */
	return &self->super;
    }

  werror("locking xauth file `%S' failed.\n", name);
  
  self->super.alive = 0;
  KILL(self);
  return NULL;
}

/* GABA:
   (class
     (name x11_forward)
     (super channel_open)
     (vars
       ; NULL for local transport
       (host string)
       (display . UINT16)
       
       (auth_name string)
       (auth_data string)))
*/


#define XAUTH_MAX_SIZE 10000

int
xauth_lookup(struct sockaddr *address,
             struct lsh_string **name,
             struct lsh_string **auth)
{
  struct lsh_string *s = NULL;
  struct resource *lock;
  int fd;
  
  const char *fname = getenv("XAUTHORITY");
  if (!fname)
    {
      const char *home = getenv("HOME");
      if (!home)
	return 0;

      s = ssh_format("%lz/.Xauthority", home);
      fname = lsh_get_cstring(s);
    }
  assert(fname);

  lock = xauth_lock(fname);
  if (!lock)
    return 0;

  fd = open(fname, O_RDONLY);

  if (fd >= 0)
    {
      s = read_file(fd, XAUTH_MAX_SIZE);
      close(fd);
    }

  if (s)
    {
      struct simple_buffer buffer;
      simple_buffer_init(&buffer, s->length, s->data);

      while (!parse_eod(&buffer))
	{
	  UINT32 family;
	  UINT32 address_length;
	  const UINT8 *address;
	  UINT32 display_length;
	  const UINT8 *display;
	  UINT32 name_length;
	  const UINT8 *name;
	  UINT32 data_length;
	  const UINT8 *data;

	  if (parse_uint16(&buffer, &family)
	      && parse_string16(&buffer, &address_length, &address)
	      && parse_string16(&buffer, &display_length, &display)
	      && parse_string16(&buffer, &name_length, &name)
	      && parse_string16(&buffer, &data_length, &data))
	    {
	      werror("xauth: family: %i\n", family);
	      werror("       address: %ps\n", address_length, address);
	      werror("       display: %s\n", display_length, display);
	      werror("       name: %s\n", name_length, name);
	      werror("       data length: %i\n", data_length);

#if 0
	      switch (family)
		{
		default:
		  continue;
		case 0:
		  if (!self->host)
		    
		    
		}
#endif
	    }
	  else break;
	}
    }
      
  KILL_RESOURCE(lock);
  return 1;
}
#endif

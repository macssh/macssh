/* unix_interact.c
 *
 * Interact with the user.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Niels Möller
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

#include "interact.h"

#include "format.h"
#include "io.h"
#include "resource.h"
#include "suspend.h"
#include "tty.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <signal.h>

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if MACOS
#include "lsh_context.h"
#endif

#include "unix_interact.c.x"

#if MACOS
extern int yes_or_no(const struct lsh_string *s, int def, int free);
#else
static volatile sig_atomic_t window_changed;
#endif

static void winch_handler(int signum)
{
  assert(signum == SIGWINCH);

  window_changed = 1;
}


/* Depends on the tty being line buffered */
static int
read_line(int fd, UINT32 size, UINT8 *buffer)
{
  UINT32 i = 0;

  while (i < size)
    {
      int res = read(fd, buffer + i, size - i);
      if (!res)
	/* User pressed EOF (^D) */
	return i;
      else if (res < 0)
	switch(errno)
	  {
	  case EAGAIN:
	  case EINTR:
	    break;
	  default:
	    /* I/O error */
	    werror("unix_interact.c: read_line: %z (errno = %i)\n",
		   errno, STRERROR(errno));
	    return 0;
	  }
      else
	{
	  UINT32 j;
	  for (j = 0; j < (unsigned) res; j++, i++)
	    if (buffer[i] == '\n')
	      return i;
	}
    }
  /* We have filled our buffer already; continue reading until end of line */
#define BUFSIZE 512
  for (;;)
    {
      UINT8 b[BUFSIZE];
      int res = read(fd, b, BUFSIZE);
      if (!res)
	/* EOF */
	return size;
      else if (res < 0)
	switch(errno)
	  {
	  case EAGAIN:
	  case EINTR:
	    break;
	  default:
	    /* I/O error */
	    werror("tty_read_line: %z (errno = %i)\n",
		   errno, strerror(errno));
	    return 0;
	  }
      else
	{
	  UINT32 j;
	  for (j = 0; j < (unsigned) res; j++)
	    if (b[j] == '\n')
	      return res;
	}
    }
#undef BUFSIZE
}


/* NOTE: If there are more than one instance of this class, window
 * changes can be missed, as only one of them will have its signal
 * handler installed properly. */

/* GABA:
   (class
     (name window_subscriber)
     (super resource)
     (vars
       (next object window_subscriber)
       (callback object window_change_callback)))
*/

/* GABA:
   (class
     (name unix_interact)
     (super interact)
     (vars
       (tty_fd . int)
       (subscribers object window_subscriber)))
*/

#define IS_TTY(self) ((self)->tty_fd >= 0)

static int
unix_is_tty(struct interact *s)
{
  CAST(unix_interact, self, s);

  return IS_TTY(self);
}

/* FIXME: Rewrite to operate on tty_fd. */
static struct lsh_string *
unix_read_password(struct interact *s UNUSED,
		   UINT32 max_length UNUSED,
		   const struct lsh_string *prompt, int free)
{
  /* NOTE: Ignores max_length; instead getpass's limit applies. */

  char *password;
  const char *cprompt = lsh_get_cstring(prompt);

  if (!cprompt)
    {
      if (free)
	lsh_string_free(prompt);
      return NULL;
    }
  /* NOTE: This function uses a static buffer. */
  password = getpass(cprompt);

  if (free)
  lsh_string_free(prompt);

  if (!password)
    return NULL;
  
  return make_string(password);
}


static int
unix_yes_or_no(struct interact *s,
	       const struct lsh_string *prompt,
	       int def, int free)
{
#define TTY_BUFSIZE 10

  CAST(unix_interact, self, s);
  if (!IS_TTY(self) || quiet_flag)
    {
      if (free)
	lsh_string_free(prompt);
      return def;
    }
  else    
    {
#if MACOS
      return yes_or_no(prompt, def, free);
#else
      UINT8 buffer[TTY_BUFSIZE];
      const struct exception *e;
  
      e = write_raw(self->tty_fd, prompt->length, prompt->data);

      if (free)
	lsh_string_free(prompt);

      if (e)
	return def;

      if (!read_line(self->tty_fd, TTY_BUFSIZE, buffer))
	return def;

      switch (buffer[0])
	{
	case 'y':
	case 'Y':
	  return 1;
	default:
	  return 0;
	}
#endif
#undef TTY_BUFSIZE
    }
}

/* GABA:
   (class
     (name unix_termios)
     (super terminal_attributes)
     (vars
       (ios . "struct termios")))
*/

static struct terminal_attributes *
do_make_raw(struct terminal_attributes *s)
{
  CAST(unix_termios, self, s);
  CLONED(unix_termios, res, self);

  CFMAKERAW(&res->ios);

  return &res->super;
}

static struct lsh_string *
do_encode(struct terminal_attributes *s)
{
  CAST(unix_termios, self, s);
  return tty_encode_term_mode(&self->ios);
}

static struct terminal_attributes *
unix_get_attributes(struct interact *s)
{
  CAST(unix_interact, self, s);

  if (!IS_TTY(self))
    return NULL;
  else
    {
      NEW(unix_termios, res);
      res->super.make_raw = do_make_raw;
      res->super.encode = do_encode;
      
      if (!tty_getattr(self->tty_fd, &res->ios) < 0)
	{
	  KILL(res);
	  return NULL;
	}
      return &res->super;
    }
}

static int
unix_set_attributes(struct interact *s,
		    struct terminal_attributes *a)
{
  CAST(unix_interact, self, s);
  CAST(unix_termios, attr, a);

  return IS_TTY(self)
    && tty_setattr(self->tty_fd, &attr->ios);
}

static int
unix_window_size(struct interact *s,
		 struct terminal_dimensions *d)
{
  CAST(unix_interact, self, s);

  return IS_TTY(self)
    && tty_getwinsize(self->tty_fd, d);
}

static struct resource *
unix_window_change_subscribe(struct interact *s,
			     struct window_change_callback *callback)
{
  CAST(unix_interact, self, s);

  NEW(window_subscriber, subscriber);
  init_resource(&subscriber->super, NULL);

  subscriber->next = self->subscribers;
  subscriber->callback = callback;

  self->subscribers = subscriber;

  return &subscriber->super;
}

/* GABA:
   (class
     (name winch_handler)
     (super lsh_callback)
     (vars
       (interact object unix_interact)))
*/

static void
do_winch_handler(struct lsh_callback *s)
{
  CAST(winch_handler, self, s);
  struct unix_interact *i = self->interact;
  
  if (i->subscribers)
    {
      struct window_subscriber *s;
      struct window_subscriber **s_p;

      for (s_p = &i->subscribers; (s = *s_p) ;)
	{
	  if (!s->super.alive)
	    *s_p = s->next;
	  else
	    {
	      WINDOW_CHANGE_CALLBACK(s->callback, &i->super);
	      s_p = &s->next;
	    }
	}
    }
}

static struct lsh_callback *
make_winch_handler(struct unix_interact *i)
{
  NEW(winch_handler, self);
  self->super.f = do_winch_handler;
  self->interact = i;

  return &self->super;
}

struct interact *
make_unix_interact(struct io_backend *backend)
{
  NEW(unix_interact, self);
  
  self->super.is_tty = unix_is_tty;
  self->super.read_password = unix_read_password;
  self->super.yes_or_no = unix_yes_or_no;
  self->super.get_attributes = unix_get_attributes;
  self->super.set_attributes = unix_set_attributes;
  self->super.window_size = unix_window_size;
  self->super.window_change_subscribe = unix_window_change_subscribe;
  
  window_changed = 0;

  self->tty_fd = -1;
  
#if HAVE_STDTTY_FILENO
  if (isatty(STDTTY_FILENO))
    self->tty_fd = STDTTY_FILENO;
#else /* ! HAVE_STDTTY_FILENO */
#if MACOS
  self->tty_fd = open("dev:tty", O_RDWR);
#else
  self->tty_fd = open("/dev/tty", O_RDWR);
#endif
#endif

  if (backend && (self->tty_fd >= 0))
    {
      /* Track window changes. */
      struct sigaction winch;

      memset(&winch, 0, sizeof(winch));
      winch.sa_handler = winch_handler;
      sigemptyset(&winch.sa_mask);
      winch.sa_flags = 0;

      if (sigaction(SIGWINCH, &winch, NULL) < 0)
	werror("Failed to install SIGWINCH handler (errno = %i): %z\n",
	       errno, STRERROR(errno));

      io_signal_handler(backend, &window_changed,
			make_winch_handler(self));

      suspend_handle_tty(self->tty_fd);
    }
  return &self->super;
}

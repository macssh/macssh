/* server_pty.c
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, Niels Möller, Balázs Scheidler
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

#include "server_pty.h"

#include "channel.h"
#include "connection.h"
#include "format.h"
#include "parse.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <fcntl.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>  /* FIXME: for snprintf, maybe use a custom snprintf? Bazsi */

#if HAVE_STROPTS_H
#  include <stropts.h>  /* isastream() */
#endif

#if HAVE_PTY_H
#  include <pty.h>  /* openpty() */
#endif


#define GABA_DEFINE
#include "server_pty.h.x"
#undef GABA_DEFINE

static void
do_kill_pty_info(struct resource *r)
{
  CAST(pty_info, closure, r);

  if (closure->super.alive)
    {
      closure->super.alive = 0;
      if ( (closure->master >= 0) && (close(closure->master) < 0) )
	werror("do_kill_pty_info: closing master failed (errno = %i): %z\n",
	       errno, STRERROR(errno));

      if ( (closure->slave >= 0) && (close(closure->slave) < 0) )
	werror("do_kill_pty_info: closing slave failed (errno = %i): %z\n",
	       errno, STRERROR(errno));
    }
}

struct pty_info *
make_pty_info(void)
{
  NEW(pty_info, pty);

  resource_init(&pty->super, do_kill_pty_info);
  pty->tty_name = NULL;
  pty->master = pty->slave = -1;
  return pty;
}

/* FIXME: Maybe this name should be configurable? */
#ifndef TTY_GROUP
#define TTY_GROUP "tty"
#endif

#ifndef ACCESSPERMS
#define ACCESSPERMS 07777
#endif

/* Sets the permissions on the slave pty suitably for use by USER.
 * This function is derived from the grantpt function in
 * sysdeps/unix/grantpt.c in glibc-2.1. */

static int
pty_check_permissions(const char *name, uid_t user)
{
  struct stat st;
  struct group *grp;
  gid_t tty_gid;
      
  if (stat(name, &st) < 0)
    return 0;

  /* Make sure that the user owns the device. */
  if ( (st.st_uid != user)
       && (chown(name, user, st.st_gid) < 0) )
    return 0;

  /* Points to static area */
  grp = getgrnam(TTY_GROUP);

  if (grp)
    tty_gid = grp->gr_gid;
  else
    {
      /* If no tty group is found, use the server's gid */
      werror("lshd: server_pty.c: No tty group found.\n");
      tty_gid = getgid();
    }

  if ( (st.st_gid != tty_gid)
       && (chown(name, user, tty_gid) < 0))
    return 0;

  /* Make sure the permission mode is set to readable and writable
   * by the owner, and writable by the group. */

  if ( ((st.st_mode & ACCESSPERMS) != (S_IRUSR | S_IWUSR | S_IWGRP))
       && (chmod(name, S_IRUSR | S_IWUSR | S_IWGRP) < 0) )
    return 0;

  /* Everything is fine */
  return 1;
}

#if HAVE_UNIX98_PTYS

/* Returns the name of the slave tty, as a string with an extra
 * terminating NUL. */

static struct lsh_string *
pty_grantpt_uid(int master, uid_t user)
{
  uid_t me = getuid();
  if (me == user)
    {
      /* Use standard grantpt call */
      if (grantpt(master) < 0)
	return NULL;

      return format_cstring(ptsname(master));
    }
  else
    { /* Set up permissions for user */

      /* Pointer to static area */
      char *name = ptsname(master);
      return (pty_check_permissions(name, user)
	      ? format_cstring(name)
	      : NULL);
      
    }
}
#endif /* HAVE_UNIX98_PTYS */

int
pty_allocate(struct pty_info *pty,
	     uid_t user
#if !HAVE_UNIX98_PTYS
	     UNUSED
#endif
	     )
{
#if HAVE_UNIX98_PTYS
  struct lsh_string *name = NULL;
  if ((pty->master = open("/dev/ptmx", O_RDWR | O_NOCTTY)) < 0)
    {
      werror("pty_allocate: Opening /dev/ptmx failed (errno = %i): %z\n",
	     errno, STRERROR(errno));
      return 0;
    }
  
  if (! (name = pty_grantpt_uid(pty->master, user)))
    goto close_master;
  
  if (unlockpt(pty->master) < 0)
    goto close_master;

  pty->slave = open(name->data, O_RDWR | O_NOCTTY);
  if (pty->slave == -1)
    goto close_master;

# ifdef HAVE_STROPTS_H
  if (isastream(pty->slave))
    {
      if (ioctl(pty->slave, I_PUSH, "ptem") < 0
          || ioctl(pty->slave, I_PUSH, "ldterm") < 0)
        goto close_slave;
    }
#  endif /* HAVE_STROPTS_H */

  pty->tty_name = name;
  return 1;

close_slave:
  close (pty->slave); pty->slave = -1;
close_master:
  close (pty->master); pty->master = -1;
  
  if (name)
    lsh_string_free(name);
  return 0;
  
#elif PTY_BSD_SCHEME

#define PTY_BSD_SCHEME_MASTER "/dev/pty%c%c"
#define PTY_BSD_SCHEME_SLAVE  "/dev/tty%c%c"
  char first[] = PTY_BSD_SCHEME_FIRST_CHARS;
  char second[] = PTY_BSD_SCHEME_SECOND_CHARS;
  char master[MAX_TTY_NAME], slave[MAX_TTY_NAME];
  unsigned int i, j;
  int saved_errno;

#define CONST_STRLEN(s) (sizeof((s)) - sizeof(""))
  
  for (i = 0; i < CONST_STRLEN(first); i++)
    {
      for (j = 0; j < CONST_STRLEN(second); j++) 
        {
	  snprintf(master, sizeof(master),
		   PTY_BSD_SCHEME_MASTER, first[i], second[j]);
			
	  pty->master = open(master, O_RDWR | O_NOCTTY);
	  if (pty->master != -1) 
	    {
	      /* master succesfully opened */
	      snprintf(slave, sizeof(slave),
		       PTY_BSD_SCHEME_SLAVE, first[i], second[j]);
				
	      pty->slave = open(slave, O_RDWR | O_NOCTTY);
	      if (pty->slave == -1) 
	        {
		  saved_errno = errno;
		  close(pty->master); pty->master = -1;
		  errno = saved_errno;
		  return 0;
	        }
	      /* NOTE: As there is no locking, setting the permissions
	       * properly does not guarantee that nobody else has the
	       * pty open, and can snoop the traffic on it. But it
	       * should be a little better than nothing. */

	      /* FIXME: Should we do something about the master
	       * permissions as well? */
	      if (pty_check_permissions(slave, user))
		{
		  pty->tty_name = format_cstring(slave);
		  return 1;
		}
	      saved_errno = errno;
	      close(pty->master); pty->master = -1;
	      close(pty->slave); pty->slave = -1;
	      return 0;
	    }
        }
    }
  return 0;
#elif HAVE_OPENPTY
  /* FIXME: openpty may not work properly, when called with the
   * wrong uid. */
#error The openpty scheme is not currently supported.
  return (openpty(&pty->master, &pty->slave, NULL, NULL, NULL) == 0)
    && pty_check_permissions(ptsname..., user);

#else /* !HAVE_OPENPTY */
  /* No pty:s */
  return 0;
#endif
}

/* NOTE: This function also makes the current process a process group
 * leader. */
int
tty_setctty(struct pty_info *pty)
{
  debug("tty_setctty\n");
  if (setsid() < 0)
    {
      werror("tty_setctty: setsid failed, already process group leader?\n"
	     "   (errno = %i): %z\n", errno, STRERROR(errno));
      return 0;
    }
#if HAVE_UNIX98_PTYS
  {
    int fd;
    
    /* Open the slave, to make it our controlling tty */

    /* FIXME: According to carson@tla.org, there's a cleaner POSIX way
     * to make a tty the process's controlling tty, but I haven't
     * found out how. */

    debug("setctty: Attempting open\n");
    fd = open(pty->tty_name->data, O_RDWR);
    if (fd < 0)
      {
	werror("tty_setctty: open(\"%z\") failed,\n"
	       "   (errno = %i): %z\n",
	       pty->tty_name->data, errno, STRERROR(errno));
	return 0;
      }
    close(fd);

    return 1;
  }
#elif PTY_BSD_SCHEME
  {
    /* Is this really needed? setsid should unregister the
     * controlling tty */
#if 0
    int oldtty;
  
    oldtty = open("/dev/tty", O_RDWR | O_NOCTTY);
    if (oldtty >= 0)
      {
	ioctl(oldtty, TIOCNOTTY, NULL);
	close(oldtty);
	oldtty = open("/dev/tty", O_RDWR | O_NOCTTY);
	if (oldtty >= 0)
	  {
	    werror("pty_setctty: Error disconnecting from controlling tty.\n");
	    close(oldtty);
	    return 0;
	  }
      }
#endif
    
    if (ioctl(pty->slave, TIOCSCTTY, NULL) == -1)
      {
	werror("tty_setctty: Failed to set the controlling tty.\n"
	       "   (errno = %i): %z\n", errno, STRERROR(errno));
	return 0;
      }
    
    return 1;
  }
#else /* !PTY_BSD_SCHEME */
#error Dont know how to register a controlling tty
#endif
}

/* daemon.c
 *
 * Derived from
 * http://www.zip.com.au/~raf2/lib/software/daemon
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 raf, Niels Möller
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

#include "daemon.h"

#include "format.h"
#include "werror.h"
#include "xalloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

/* For setrlimit */
#include <sys/time.h>    /* Needed on BSD */
#include <sys/resource.h>

#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/socket.h>

#ifndef PID_DIR
#define PID_DIR "/var/run"
#endif

#ifndef ROOT_DIR
#define ROOT_DIR "/"
#endif

#ifndef PID_SUFFIX
#define PID_SUFFIX ".pid"
#endif

/*
** struct lsh_string *daemon_pidfile(const char *name)
**
** Creates a pid file for a daemon and locks it.
** The file has one line containing the process id of the daemon.
** The well-known location for the file is defined in PID_DIR
** ("/var/run" by default). The name of the file is the name of
** the daemon (given by the name argument) followed by ".pid".
**
** Returns 1 on success or 0 on failure (with errno set
** appropriately). Reasons for failure are: ENAMETOOLONG If the pid
** file's path is longer than the system path limit. open(2).
** fcntl(2). write(2). */


int daemon_pidfile(const char *name)
{
  int fd;
  
  /* Try to open the file atomically. This provides sufficient locking
   * on normal (non-NFS) file systems. */

  fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0644);

  if (fd < 0)
    {
      if (errno != EEXIST)
	{
	  werror("Failed to open pid file '%z' (errno = %i): %z\n",
		 name, errno, STRERROR(errno));
	  return 0;
	}

      werror("Pid file '%z' already exists.\n", name);
      return 0;
      
      /* FIXME: Various trickery to detect and ignore stale pid files.
       * Disabled for now. */
#if 0
      /* Attempt to lock and read the file */
      {
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	int fd = open(name, O_RDWR);
	
	if (fcntl(fd, F_SETLK, &lock) < 0)
	  {
	    werror("pid file '%z' exists and is locked.\n");
	    return 0;
	  }
      }
      fd = open(name, O_RDONLY);
      if (fd < 0)
	{
	  werror("Pid file '%z' already exists, and is unreadable (errno = %i): %z\n",
		 name, errno, STRERROR(errno));
	  return 0;
	}
      {
	/* Read some characters */
#define BUF_SIZE 20
	char buffer[BUF_SIZE];
	
	int length = read(fd, buffer, BUF_SIZE);
	close(fd);
	
	if (length < 0)
	  {
	    werror("Pid file '%z' already exists, but read failed (errno = %i): %z\n",
		   name, errno, STRERROR(errno));
	    return 0;
	  }
	if ( !length|| !buffer[0] || (length == BUF_SIZE) )
	  {
	    werror("Pid file '%z' already exists, but contents is garbled (errno = %i): %z\n",
		   name, errno, STRERROR(errno));
	    return 0;
	  }

	buffer[length] = '\0';
	
	{
	  long other;
	  char *end;

	  /* FIXME: strtol is somewhat inconvenient, too forgiving,
	   * and possibly locale dependent. */
	  other = strtol(buffer, &end, 10);
	  errno = 0;
	  
	  if ( (errno == ERANGE) || (end != buffer + length)
	       || (other <= 0) || (other != (pid_t) other))
	    {
	      werror("Pid file '%z' already exists, but contents is garbled (errno = %i): %z\n",
		     name, errno, STRERROR(errno));
	      return 0;
	    }

	  if ( (kill((pid_t) other, 0) < 0)
	       && (errno == SRCH))
	    {
	      werror("pid lock '%z' owned by process %i, which appear to be dead.\n",
		     name, other);
	      /* We could try to be clever and delete the file in this
	       * case, but then we get into race conditions if two
	       * processes tries to do that simultaneously. */
	    }
	  else
	    werror("pid lock '%z' owned by process %i, which appears to be alive.\n");
	  return 0;
	}
      }
#endif 
    }
  else
    {
      struct lsh_string *pid = ssh_format("%di", getpid());

      int res = write(fd, pid->data, pid->length);
      close(fd);
      
      if ( (res > 0) && ((unsigned) res == pid->length) )
	{
	  /* Success! */
	  lsh_string_free(pid);
	  return 1;
	}
      werror("Writing pid file '%z' failed (errno = %i): %z",
	     name, errno, STRERROR(errno));

      /* Attempt unlinking file */
      if (unlink(name) < 0)
	werror("Unlinking pid file '%z' failed (errno = %i): %z",
	       name, errno, STRERROR(errno));
      
      lsh_string_free(pid);
	  
      return 0;
    }
}

/*
** int daemon_started_by_init(void)
**
** Determines whether or not this process was started by init(8).
** If it was, we might be getting respawned so fork(2) and exit(2)
** would be a big mistake.
*/

int daemon_started_by_init(void)
{
  return (getppid() == 1);
}

/*
** int daemon_started_by_inetd(void)
**
** Determines whether or not this process was started by inetd(8).
** If it was, stdin, stdout and stderr would be opened to a socket.
** Closing them would be a big mistake. We also wouldn't need to
** fork(2) and exec(2) because there isn't a controlling terminal
** in sight.
*/

/* FIXME: Do we need to detect if the socket is listening or connected
 * to a peer? */
int
daemon_started_by_inetd(void)
{
  int optval;
  socklen_t optlen = sizeof(optval);
  
  return (getsockopt(STDIN_FILENO, SOL_SOCKET, SO_TYPE, &optval, &optlen) == 0);
}

/* Disable core files */
int
daemon_disable_core(void)
{
  struct rlimit limit = { 0, 0 };

  /*
  ** Disable core files to prevent security holes.
  */

  if (getrlimit(RLIMIT_CORE, &limit) == -1)
    return 0;

  limit.rlim_cur = 0;

  if (setrlimit(RLIMIT_CORE, &limit) == -1)
    return 0;

  return 1;
}

/*
** int daemon_init(void)
**
** Initialises a daemon:
** Disables core files to prevent security holes.
** If the process wasn't started by init(8) or inetd(8):
**   Backgrounds the process to lose process group leadership.
**   Becomes a process session leader.
**   When SVR4 is defined:
**     Backgrounds the process again to lose process group leadership.
**     This prevents the process from gaining a controlling terminal.
**     Under BSD, you must still include O_NOCTTY when opening terminals
**     to prevent the process from gaining a controlling terminal.
** Changes directory to the root directory so as not to hamper umounts.
** Clears the umask to enable explicit file modes.
** If the process wasn't started by inetd(8):
**   Closes all files (as determined by sysconf(2)).
**   Opens stdin, stdout and stderr to /dev/null in case something needs them.
** If the hup parameter is non-null,
**   Registers a SIGHUP handler.
** If the term parameter is non-null,
**   Registers a SIGTERM handler.
** If the name parameter is non-null,
**   Places the process id in the file system and locks it.
**
** Returns DAEMON_NORMAL, DAEMON_INIT or DAEMON_INETD on success, 0 on
** error (with errno set appropriately).
** 
** Reasons for failure are: getrlimit(2), setrlimit(2), fork(2), chdir(2),
** sysconf(2), open(2), dup2(2), daemon_pidfile(), sigemptyset(2),
** sigaddset(2), sigaction(2). */

int daemon_init(void)
{
  int mode = DAEMON_NORMAL;

  if (daemon_started_by_init())
    mode = DAEMON_INIT;
  else if (daemon_started_by_inetd())
    mode = DAEMON_INETD;
  
  /*
  ** Don't setup a daemon-friendly process context
  ** if started by init(8) or inetd(8).
  */

  if (mode == DAEMON_NORMAL)
    {
      /*
      ** Background the process.
      ** Lose process group leadership.
      */

      switch (fork())
	{
	case -1:
	  return 0;
	case 0:
	  /* Child */
	  break;
	default:
	  /* Parant */
	  exit(0);
	}
      /*
      ** Become a process session leader.
      */

      if (setsid() < 0)
	fatal("daemon_init: setsid failed.\n");

      /* This should not be needed. All potential tty:s should be
       * opened with O_NOCTTY. FIXME: What if a user links, say,
       * ~/.lsh/authorized_keys to a tty? Perhaps we have to make sure
       * that we never ever call open without the O_NOCTTY flag? */
#if 0
#ifdef SVR4
      /*
      ** Lose process session leadership
      ** to prevent gaining a controlling
      ** terminal in SVR4.
      */
      switch (fork())
	{
	case -1:
	  return 0;
	case 0:
	  break;
	default:
	  exit(0);
	}
#endif
#endif
    }

  /*
  ** Enter the root directory to prevent hampering umounts.
  */

  if (chdir(ROOT_DIR) == -1)
    return 0;

  /*
  ** Clear umask to enable explicit file modes.
  */

  umask(0);

  {
    int fd;

    fd = open("/dev/null", O_RDWR);

    if (fd < 0)
      return 0;

    /* Don't close stdin if we are started from inetd. */

    if ( (mode != DAEMON_INETD)
	 && (dup2(fd, STDIN_FILENO) < 0))
      {
	werror("daemon_init: Failed to redirect stdin to /dev/null.\n");
	close(fd);
	return 0;
      }

    if ( (dup2(fd, STDOUT_FILENO) < 0)
	 || (dup2(fd, STDERR_FILENO) < 0) )
      {
	werror("daemon_init: Failed to redirect stdout and stderr to /dev/null.\n");
	close(fd);
	return 0;
      }

    close(fd);

    /* It seems unfriendly to try toclose all fds (and on some
     * systems, e.g. the HURD, there are no limits to the number of
     * open files. It should be good enough to take care of stdio. */
    
#if 0
    /*
    ** Close all open files.
    ** Don't forget to open any future
    ** tty devices with O_NOCTTY so as
    ** to prevent gaining a controlling
    ** terminal (not necessary with SVR4).
    */
    
    if ((nopen = limit_open()) == -1)
      return -1;
    
    for (fd = 0; fd < nopen; ++fd)
      close(fd);
    
    /*
    ** Open stdin, stdout and stderr to /dev/null
    ** just in case someone expects them to be open.
    */

    if ((fd = open("/dev/null", O_RDWR)) == -1)
      return -1;

    /*
    ** This is only needed for very strange
    ** (hypothetical) posix implementations
    ** where STDIN_FILENO != 0 or STDOUT_FILE != 1
    ** or STERR_FILENO != 2 (yeah, right).
    */

    if (fd != STDIN_FILENO)
      {
	if (dup2(fd, STDIN_FILENO) == -1)
	  return -1;

	close(fd);
      }

    if (dup2(STDIN_FILENO, STDOUT_FILENO) == -1)
      return -1;

    if (dup2(STDIN_FILENO, STDERR_FILENO) == -1)
      return -1;
#endif
  }
  
#if 0
  /*
  ** Register a SIGHUP handler, if required.
  */

  if (hup && signal_set_handler(SIGHUP, 0, hup) == -1)
    return -1;

  /*
  ** Register a SIGTERM handler, if required.
  */

  if (term && signal_set_handler(SIGTERM, 0, term) == -1)
    return -1;
#endif

  return 1;
}

/*
** int daemon_close()
**
** Unlinks the daemon's (locked) process id file.
*/

int daemon_close(const char *name)
{
  if (unlink(name) < 0)
    {
      werror("daemon_close: Unlink of pid file '%z' failed (errno = %i): %z\n",
	     name, errno, STRERROR(errno));
      return 0;
    }
  return 1;
}

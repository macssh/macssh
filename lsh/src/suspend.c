/* suspend.c
 *
 * Handle process suspend for the client programs.
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

#include "suspend.h"

#include "tty.h"
#include "werror.h"

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

static struct termios original_mode;

static int tty_fd = -1;

/* On SIGTSTP, we restore the tty mode to the origanal.
 * On SIGCONT, we restore it, and the stdio fd flags, to what we hade before
 * the suspend. */
static void
stop_handler(int signum)
{
  struct termios mine;
  int need_tty_reset;
  int stdin_flags;
  int stdout_flags;
  int stderr_flags;
  
  assert(signum == SIGTSTP);

  stdin_flags = fcntl(STDIN_FILENO, F_GETFL);
  stdout_flags = fcntl(STDOUT_FILENO, F_GETFL);
  stderr_flags = fcntl(STDERR_FILENO, F_GETFL);

  need_tty_reset = (tty_fd > 0) ? tty_getattr(tty_fd, &mine) : 0;

  if (need_tty_reset)
    tty_setattr(tty_fd, &original_mode);

#ifndef MACOS
  kill(getpid(), SIGSTOP);
#endif

  if (need_tty_reset)
    tty_setattr(tty_fd, &mine);

  if (stdin_flags >= 0)
    fcntl(STDIN_FILENO, F_SETFL, stdin_flags);

  if (stdout_flags >= 0)
    fcntl(STDOUT_FILENO, F_SETFL, stdin_flags);

  if (stderr_flags >= 0)
    fcntl(STDERR_FILENO, F_SETFL, stdin_flags);
}

void
suspend_install_handler(void)
{
  struct sigaction stop;

  memset(&stop, 0, sizeof(stop));
  stop.sa_handler = stop_handler;
  sigemptyset(&stop.sa_mask);
  stop.sa_flags = 0;

  if (sigaction(SIGTSTP, &stop, NULL) < 0)
    werror("Failed to install SIGTSTP handler (errno = %i): %z\n",
	   errno, STRERROR(errno));
}

void
suspend_handle_tty(int fd)
{
  tty_fd = fd;

  if (!tty_getattr(fd, &original_mode))
    werror("install_suspend_handler: tty_getattr failed (errno = %i): %z\n",
	   errno, STRERROR(errno));
}


/* A callback that suspends the process. */
static void
do_suspend(struct lsh_callback *self UNUSED)
{
#ifndef MACOS
  if (kill(getpid(), SIGTSTP) < 0)
#endif
    werror("do_suspend: kill failed (errno = %i): %z\n",
	   errno, STRERROR(errno));
}

/* FIXME: Use const? */
struct lsh_callback
suspend_callback = { STATIC_HEADER, do_suspend };

/* reaper.c
 *
 * Handle child processes.
 *
 * $Id$
 */

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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "reaper.h"

#include "alist.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define GABA_DEFINE
#include "reaper.h.x"
#undef GABA_DEFINE

#include "reaper.c.x"

static volatile sig_atomic_t halloween;

static void child_handler(int signum)
{
  assert(signum == SIGCHLD);

  halloween = 1;
}

/* GABA:
   (class
     (name reaper)
     (super reap)
     (vars
       ; Mapping of from pids to exit-callbacks. 
       ; NOTE: This assumes that a pid_t fits in an int. 
       (children object alist)))
*/

static void
do_reap(struct reap *c,
		    pid_t pid, struct exit_callback *callback)
{
  CAST(reaper, closure, c);

  ALIST_SET(closure->children, pid, &callback->super);
}

/* GABA:
   (class
     (name reaper_callback)
     (super lsh_callback)
     (vars
       (reaper object reaper)))
*/

static void
do_reaper_callback(struct lsh_callback *s)
{
  CAST(reaper_callback, self, s);
  struct reaper *r = self->reaper;
  
  pid_t pid;
  int status;

  while( (pid = waitpid(-1, &status, WNOHANG)) )
    {
      if (pid > 0)
	{
	  int signaled;
	  int value;
	  int core;
	  struct exit_callback *callback;
	  
	  if (WIFEXITED(status))
	    {
	      verbose("Child %i died with exit code %i.\n",
		      pid, WEXITSTATUS(status));
	      signaled = 0;
	      core = 0;
	      value = WEXITSTATUS(status);
	    }
	  else if (WIFSIGNALED(status))
	    {
	      verbose("Child %i killed by signal %i.\n",
		      pid, WTERMSIG(status));
	      signaled = 1;
	      core = !!WCOREDUMP(status);
	      value = WTERMSIG(status);
	    }
	  else
	    fatal("Child died, but neither WIFEXITED or WIFSIGNALED is true.\n");

	  {
	    CAST_SUBTYPE(exit_callback, c, ALIST_GET(r->children, pid));
	    callback = c;
	  }
	  
	  if (callback)
	    {
	      ALIST_SET(r->children, pid, NULL);
	      EXIT_CALLBACK(callback, signaled, core, value);
	    }
	  else
	    {
	      if (WIFSIGNALED(status))
		werror("Unregistered child %i killed by signal %i.\n",
		       pid, value);
	      else
		werror("Unregistered child %i died with exit status %i.\n",
		       pid, value);
	    }
	}
      else switch(errno)
	{
	case EINTR:
	  werror("reaper.c: waitpid returned EINTR.\n");
	  break;
	case ECHILD:
	  /* No more child processes */
	  return;
	default:
	  fatal("reaper.c: waitpid failed (errno = %i), %z\n",
		errno, STRERROR(errno));
	}
    }
}

static struct lsh_callback *
make_reaper_callback(struct reaper *reaper)
{
  NEW(reaper_callback, self);
  self->super.f = do_reaper_callback;
  self->reaper = reaper;

  return &self->super;
}

static void
reaper_install_handler(struct reaper *reaper,
		       struct io_backend *b)
{
  struct sigaction chld;
  memset(&chld, 0, sizeof(chld));

  chld.sa_handler = child_handler;
  sigemptyset(&chld.sa_mask);
  chld.sa_flags = SA_NOCLDSTOP;
  
  halloween = 0;

  if (sigaction(SIGCHLD, &chld, NULL) < 0)
    fatal("Failed to install handler for SIGCHLD.\n");

  io_signal_handler(b, &halloween, make_reaper_callback(reaper));
}

struct reap *
make_reaper(struct io_backend *b)
{
  NEW(reaper, self);

  self->super.reap = do_reap;
  self->children = make_linked_alist(0, -1);

  reaper_install_handler(self, b);

  return &self->super;
}

/* unix_random.c
 *
 * $Id$
 *
 * Randomness polling on unix, using ideas from Peter Gutmann's
 * cryptlib. */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
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

#include "randomness.h"

#include "crypto.h"
#include "reaper.h"
#include "xalloc.h"
#include "werror.h"

#include <assert.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pwd.h>

#ifdef HAVE_POLL
# if HAVE_POLL_H
#  include <poll.h>
# elif HAVE_SYS_POLL_H
#  include <sys/poll.h>
# endif
#else
# include "jpoll.h"
#endif

/* Workaround for some version of FreeBSD. */
#ifdef POLLRDNORM
# define MY_POLLIN (POLLIN | POLLRDNORM)
#else /* !POLLRDNORM */
# define MY_POLLIN POLLIN
#endif /* !POLLRDNORM */

#include <sys/time.h>
#include <sys/resource.h>

enum poll_status { POLL_NO_POLL, POLL_RUNNING, POLL_FINISHED, POLL_FAILED };

#include "unix_random.c.x"

/* GABA:
   (class
     (name unix_random)
     (super random_poll)
     (vars
       ; For the slow poll
       (reaper object reap)
       (poll_uid . uid_t)
       (pid . pid_t)
       (status . "enum poll_status")
       ; NOTE: This fd is not known to the gc. 
       (fd . int)
     
       ; For the fast poll, count number of slow polls per second.
       (previous_time . time_t)
       (time_count . unsigned)))
*/

/* GABA:
   (class
     (name unix_random_callback)
     (super exit_callback)
     (vars
       (ctx object unix_random)))
*/

static void
do_unix_random_callback(struct exit_callback *s,
			int signalled, int core UNUSED,
			int value)
{
  CAST(unix_random_callback, self, s);

  if (signalled || value)
    {
      self->ctx->status = POLL_FAILED;
      trace("unix_random.c: do_unix_random_callback, background poll failed.\n");
    }
  else
    {
      self->ctx->status = POLL_FINISHED;
      trace("unix_random.c: do_unix_random_callback, background poll finished.\n");
    }
}

static struct exit_callback *
make_unix_random_callback(struct unix_random *ctx)
{
  NEW(unix_random_callback, self);
  self->super.exit = do_unix_random_callback;
  self->ctx = ctx;

  return &self->super;
}

#define UNIX_RANDOM_POLL_SIZE 20

/* This structure ought to fit in a pipe buffer (so that we can
 * waitpid() the process before reading its stdout). */

struct unix_random_poll_result
{
  UINT32 count;
  UINT8 data[UNIX_RANDOM_POLL_SIZE];
};

#define UNIX_RANDOM_SOURCE_SCALE 8192
struct unix_random_source
{
  const char *path;
  const char *arg; /* For now, use at most one argument. */
  int has_alternative;

 /* If non-zero, count quality bits of entropy if the amount of output
  * exceeds this value. */
  unsigned small;
  /* Bits of entropy per UNIX_RANDOM_SOURCE_SCALE bytes of output. */
  unsigned quality;
};

/* Lots of output expected; round downwards. */
#define WLARGE(x) 0, x
/* Small but significant output expected; round upwards */
#define WSMALL(x) 100, x

static const struct unix_random_source random_sources[] =
{
  { "/bin/vmstat", "-s", 	1, WSMALL(30) },
  { "/usr/bin/vmstat", "-s", 	0, WSMALL(30) },
  { "/bin/vmstat", "-c", 	1, WSMALL(30) },
  { "/usr/bin/vmstat", "-c", 	0, WSMALL(30) },
  { "/usr/bin/pfstat", NULL, 	0, WSMALL(20) },
  { "/bin/vmstat", "-i", 	1, WSMALL(20) },
  { "/usr/bin/vmstat", "-i", 	0, WSMALL(20) },
  { "/usr/ucb/netstat", "-s", 	1, WLARGE(20) },
  { "/usr/bin/netstat", "-s", 	1, WLARGE(20) },
  { "/usr/sbin/netstat", "-s", 	1, WLARGE(20) },
  { "/bin/netstat", "-s", 	1, WLARGE(20) },
  { "/usr/etc/netstat", "-s", 	0, WLARGE(20) },
  { "/usr/bin/nfsstat", NULL, 	0, WLARGE(20) },
  { "/usr/ucb/netstat", "-m", 	1, WSMALL(10) },
  { "/usr/bin/netstat", "-m", 	1, WSMALL(10) },
  { "/usr/sbin/netstat", "-m", 	1, WSMALL(10) },
  { "/bin/netstat", "-m", 	1, WSMALL(10) },
  { "/usr/etc/netstat", "-m", 	0, WSMALL(10) },
  { "/usr/ucb/netstat", "-in", 	1, WSMALL(10) },
  { "/usr/bin/netstat", "-in", 	1, WSMALL(10) },
  { "/usr/sbin/netstat", "-in", 1, WSMALL(10) },
  { "/bin/netstat", "-in", 	1, WSMALL(10) },
  { "/usr/etc/netstat", "-in", 	0, WSMALL(10) },
#if 0
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.7.1.0", 0 	WSMALL(10) }, /* UDP in */
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.7.4.0", 0	WSMALL(10) }, /* UDP out */
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.4.3.0", 0	WSMALL(10) }, /* IP ? */
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.6.10.0", 0	WSMALL(10) }, /* TCP ? */
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.6.11.0", 0	WSMALL(10) }, /* TCP ? */
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.6.13.0", 0	WSMALL(10) }, /* TCP ? */
#endif
  { "/usr/bin/mpstat", NULL,	0, WLARGE(10) },
  { "/usr/bin/w", NULL,		1, WLARGE(10) },
  { "/usr/bsd/w", NULL,		0, WLARGE(10) },
  { "/usr/bin/df", NULL,	1, WLARGE(10) },
  { "/bin/df", NULL,		0, WLARGE(10) },
  { "/usr/sbin/portstat", NULL,	0, WLARGE(10) },
  { "/usr/bin/iostat", NULL,	0, WLARGE(0) },
  { "/usr/bin/uptime", NULL,	1, WLARGE(0) },
  { "/usr/bsd/uptime", NULL,	0, WLARGE(0) },
  { "/bin/vmstat", "-f",	1, WLARGE(0) },
  { "/usr/bin/vmstat", "-f",	0, WLARGE(0) },
  { "/bin/vmstat", NULL,	1, WLARGE(0) },
  { "/usr/bin/vmstat", NULL,	0, WLARGE(0) },
  { "/usr/ucb/netstat", "-n",	1, WLARGE(5) },
  { "/usr/bin/netstat", "-n",	1, WLARGE(5) },
  { "/usr/sbin/netstat", "-n",	1, WLARGE(5) },
  { "/bin/netstat", "-n", 	1, WLARGE(5) },
  { "/usr/etc/netstat", "-n",	0, WLARGE(5) },
#if defined( __sgi ) || defined( __hpux )
  { "/bin/ps", "-el", 		1, WLARGE(3) },
#endif /* __sgi || __hpux */
  { "/usr/ucb/ps", "aux", 	1, WLARGE(3) },
  { "/usr/bin/ps", "aux", 	1, WLARGE(3) },
  { "/bin/ps", "aux", 		0, WLARGE(3) },
  { "/usr/bin/ipcs", "-a", 	1, WLARGE(5) },
  { "/bin/ipcs", "-a", 		0, WLARGE(5) },
  /* Unreliable source, depends on system usage */
  { "/etc/pstat", "-p", 	1, WLARGE(5) },
  { "/bin/pstat", "-p", 	0, WLARGE(5) },
  { "/etc/pstat", "-S", 	1, WLARGE(2) },
  { "/bin/pstat", "-S", 	0, WLARGE(2) },
  { "/etc/pstat", "-v", 	1, WLARGE(2) },
  { "/bin/pstat", "-v", 	0, WLARGE(2) },
  { "/etc/pstat", "-x", 	1, WLARGE(2) },
  { "/bin/pstat", "-x", 	0, WLARGE(2) },
  { "/etc/pstat", "-t", 	1, WLARGE(1) },
  { "/bin/pstat", "-t", 	0, WLARGE(1) },
  /* pstat is your friend */
  { "/usr/bin/last", "-n 50", 	1, WLARGE(3) },
#ifdef __sgi
  { "/usr/bsd/last", "-50",	0, WLARGE(3) },
#endif /* __sgi */
#ifdef __hpux
  { "/etc/last", "-50", 	0, WLARGE(3) },
#endif /* __hpux */
  { "/usr/bsd/last", "-n 50", 	0, WLARGE(3) },
  { "/usr/local/bin/lsof", "-lnwP", 0, WLARGE(3) },
  /* Output is very system and version-dependent */
#if 0
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.5.1.0", 0, WLARGE(1) }, /* ICMP ? */
  { "/usr/sbin/snmp_request", "localhost public get 1.3.6.1.2.1.5.3.0", 0, WLARGE(1) }, /* ICMP ? */
#endif
  { "/etc/arp", "-a",		1, WLARGE(1) },
  { "/usr/etc/arp", "-a",	1, WLARGE(1) },
  { "/usr/bin/arp", "-a",	1, WLARGE(1) },
  { "/usr/sbin/arp", "-a",	0, WLARGE(1) },
  { "/usr/sbin/ripquery", "-nw 1 127.0.0.1", 0, WLARGE(1) },
  { "/bin/lpstat", "-t",	1, WLARGE(1) },
  { "/usr/bin/lpstat", "-t",	1, WLARGE(1) },
  { "/usr/ucb/lpstat", "-t",	0, WLARGE(1) },
#if 0
  { "/usr/bin/tcpdump", "-c 5 -efvvx", 0, WLARGE(10) },
  /* This is very environment-dependant.  If
     network traffic is low, it'll probably time
     out before delivering 5 packets, which is OK
     because it'll probably be fixed stuff like
     ARP anyway */
  { "/usr/sbin/advfsstat", "-b usr_domain", 	0, WLARGE(0) },
  { "/usr/sbin/advfsstat", "-l 2 usr_domain", 	0, WLARGE(5) },
  { "/usr/sbin/advfsstat", "-p usr_domain", 	0, WLARGE(0) },
  /* This is a complex and screwball program.  Some
     systems have things like rX_dmn, x = integer,
     for RAID systems, but the statistics are
     pretty dodgy */

  /* The following aren't enabled since they're somewhat slow and not very
     unpredictable, however they give an indication of the sort of sources
     you can use (for example the finger might be more useful on a
     firewalled internal network) */
  { "/usr/bin/finger", "@ml.media.mit.edu", 	0, WLARGE(9) },
  { "/usr/local/bin/wget", "-O - http://lavarand.sgi.com/block.html", 0, WLARGE(9) },
  { "/bin/cat", "/usr/spool/mqueue/syslog", 	0, WLARGE(9) },
#endif /* 0 */
  { NULL, NULL, 0, 0, 0 }
};

#undef WSMALL
#undef WLARGE

struct unix_random_source_state
{
  const struct unix_random_source *source;
  pid_t pid;       /* Running process. */
  int fd;
  unsigned length; /* Amount of data read so far. */
};

static int
spawn_source_process(unsigned *index,
		     struct unix_random_source_state *state,
		     int null)
{
  unsigned i;
  pid_t pid;
  /* output[0] for reading, output[1] for writing. */
  int output[2];
      
  for (i = *index; random_sources[i].path; )
    {
      if (access(random_sources[i].path, X_OK) < 0)
	{
	  debug("unix_random.c: spawn_source_process: Skipping '%z'; not executable: %z\n",
		random_sources[i].path, STRERROR(errno));
	  i++;
	}
      else
	break;
    }

  if (!random_sources[i].path)
    {
      *index = i;
      return 0;
    }
  
  if (!lsh_make_pipe(output))
    {
      werror("unix_random.c: spawn_source_process: Can't create pipe (errno = %i): %z\n",
	     errno, STRERROR(errno));
      return 0;
    }
  
  state->source = random_sources + i;
  
  if (!random_sources[i].has_alternative)
    i++;
  else
    /* Skip alternatives */
    while (random_sources[i].has_alternative)
      i++;
  
  *index = i;

  debug("unix_random.c: Trying source %z %z\n",
	state->source->path,
	state->source->arg ? state->source->arg : "");
  
  pid = fork();
  switch(pid)
    {
    default:
      /* Parent */
      close (output[1]);
      state->fd = output[0];
      state->pid = pid;
      io_set_close_on_exec(state->fd);
      io_set_nonblocking(state->fd);
      state->length = 0;
      return 1;
      
    case -1:
      /* Error */
      close(output[0]);
      close(output[1]);
      return 0;
    case 0:
      /* Child */
      if (dup2(output[1], STDOUT_FILENO) < 0)
	{
	  werror("unix_random.c: spawn_source_process: dup2 for stdout failed (errno = %i): %z\n",
		 errno, STRERROR(errno));
	  _exit(EXIT_FAILURE);
	}

      /* Ignore stderr. */
      if (dup2(null, STDERR_FILENO) < 0)
	{
	  werror("unix_random.c: spawn_source_process: dup2 for stderr failed (errno = %i): %z\n",
		 errno, STRERROR(errno));
	  _exit(EXIT_FAILURE);
	}
	
      close (output[0]);
      close (output[1]);
      
      /* Works also if state->source->arg == NULL */
      execl(state->source->path, state->source->path, state->source->arg, NULL);
      
      werror("spawn_source_process: execl '%z' failed (errno = %i): %z\n",
	     state->source->path, errno, STRERROR(errno));
      
      _exit(EXIT_FAILURE);
    }
}

static unsigned
use_dev_random(struct hash_instance *hash)
{
#define DEVRANDOM_SIZE 40
  UINT8 buffer[DEVRANDOM_SIZE];
  unsigned count = 0;
  int res;
  
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd < 0)
    return 0;

  do
    { res = read(fd, buffer, DEVRANDOM_SIZE); }
  while ( (res < 0) && (errno == EINTR));

  if (res < 0)
    werror("unix_random.c: Reading from /dev/urandom failed (errno = %i): %z\n",
	   errno, STRERROR(errno));
  else if (res > 0)
    {
      HASH_UPDATE(hash, res, buffer);
      /* Count 4 bits of entropy for each byte. */
      count = res * 4;
    }
  else
    werror("unix_random.c: No data available on /dev/urandom!\n");

  close(fd);
  
  return count;
}

static unsigned
use_procfs(struct hash_instance *hash)
{
#if 0
  /* Peter Gutmann's code uses the following sources. I'm not sure
   * what quality to assign to them. */
  static const char *proc_sources[] =
  {
    "/proc/interrupts", "/proc/loadavg", "/proc/locks",
    "/proc/meminfo", "/proc/stat", "/proc/net/tcp", "/proc/net/udp", 
    "/proc/net/dev", "/proc/net/ipx", NULL
  };
#endif
  
  /* Not implemented. */
  return 0;
}

/* Spawn this number of processes. */
#define UNIX_RANDOM_POLL_PROCESSES 10
#define UNIX_RANDOM_THRESHOLD 200

static unsigned
background_poll(struct hash_instance *hash)
{
  struct unix_random_source_state state[UNIX_RANDOM_POLL_PROCESSES];
  unsigned count = 0;
  unsigned running = 0;
  unsigned i;
  unsigned index = 0;
  int null;
  
  trace("unix_random.c: background_poll\n");

  null = open("/dev/null", O_WRONLY);
  if (null < 0)
    {
      werror("unix_random.c: background_poll: Failed to open /dev/null (errno = %i): %z\n",
	     errno, STRERROR(errno));
      return count;
    }
  
  count += use_dev_random(hash);
  count += use_procfs(hash);
  
  for (i = 0; i < UNIX_RANDOM_POLL_PROCESSES; i++)
    state[i].fd = -1;

  for (running = 0; running<UNIX_RANDOM_POLL_PROCESSES; running++)
    {
      if (!spawn_source_process(&index, state + running, null))
	break;
    }

  while (running)
    {
      struct pollfd fds[UNIX_RANDOM_POLL_PROCESSES];
      unsigned nfds;
      unsigned i;

      for (i = 0, nfds = 0; i < UNIX_RANDOM_POLL_PROCESSES; i++)
	if (state[i].fd > 0)
	  {
	    fds[nfds].fd = state[i].fd;
	    fds[nfds].events = MY_POLLIN;
	    nfds++;
	  }

      assert(nfds == running);

      debug("unix_random.c: calling poll, nfds = %i\n", nfds);
      if (poll(fds, nfds, -1) < 0)
	{
	  werror("unix_random.c: background_poll poll() failed (errno = %i): %z\n",
		 errno, STRERROR(errno));
	  return count;
	}

      debug("unix_random.c: returned from poll.\n");

      for (i = 0; i<nfds; i++)
	{
	  debug("background_poll: poll for fd %i: events = 0x%xi, revents = 0x%xi.\n",
		fds[i].fd, fds[i].events, fds[i].revents);

	  /* Linux sets POLLHUP on EOF */
#ifndef POLLHUP
#define POLLHUP 0
#endif
	  if (fds[i].revents & (MY_POLLIN | POLLHUP))
	    {
#define BUFSIZE 1024
	      UINT8 buffer[BUFSIZE];
	      int res;
	      unsigned j;
	    
	      for (j = 0; j<UNIX_RANDOM_POLL_PROCESSES; j++)
		if (fds[i].fd == state[j].fd)
		  break;

	      assert(j < UNIX_RANDOM_POLL_PROCESSES);

	      debug("background_poll: reading from source %z\n",
		    state[j].source->path);
	      
	      res = read(fds[i].fd, buffer, BUFSIZE);
#undef BUFSIZE
	    
	      if (res < 0)
		{
		  if (errno != EINTR)
		    {
		      werror("unix_random.c: background_poll read() failed (errno = %i): %z\n",
			     errno, STRERROR(errno));
		      return count;
		    }
		}
	      else if (res > 0)
		{
		  HASH_UPDATE(hash, res, buffer);
		  state[j].length += res;
		}
	      else
		{ /* EOF */
		  int status;
		  unsigned entropy;
		
		  close(fds[i].fd);

		  state[j].fd = -1;

		  /* Estimate entropy */
		  entropy = state[j].length * state[j].source->quality / UNIX_RANDOM_SOURCE_SCALE ;

		  if (!entropy && state[j].source->small
		      && (state[j].length > state[j].source->small))
		    entropy = state[j].source->quality;
		
		  debug("unix_random.c: background_poll: Got %i bytes from %z %z,\n"
			"               estimating %i bits of entropy.\n",
			state[j].length,
			state[j].source->path, state[j].source->arg ? state[j].source->arg : "",
			entropy);

		  count += entropy;
		
		  if (waitpid(state[j].pid, &status, 0) < 0)
		    {
		      werror("unix_random.c: background_poll waitpid() failed (errno = %i): %z\n",
			     errno, STRERROR(errno));
		      return count;
		    }
		  if (WIFEXITED(status))
		    {
		      if (WEXITSTATUS(status))
			debug("unix_random.c: background_poll: %z exited unsuccessfully.\n",
			      state[j].source->path);
		    }
		  else if (WIFSIGNALED(status))
		    {
		      werror("unix_random.c: background_poll: %z died from signal %i (%z).\n",
			     state[j].source->path, WTERMSIG(status), STRSIGNAL(WTERMSIG(status)));
		    }

		  if (! ((count < UNIX_RANDOM_THRESHOLD)
			 && spawn_source_process(&index, state + j, null)))
		    running--;
		}
	    }
	}
    }
  return count;
}

static void
start_background_poll(struct unix_random *self)
{
  pid_t pid;
  int output[2];
  
  assert(self->status == POLL_NO_POLL);

  trace("unix_random.c: start_background_poll\n");
  
  if (!lsh_make_pipe(output))
    {
      werror("Failed to create pipe for background randomness poll.\n");
      return;
    }
  
  pid = fork();  
  switch(pid)
    {
    default:
      /* Parent */
      close(output[1]);
      self->fd = output[0];
      io_set_close_on_exec(self->fd);

      if (self->reaper)
	REAP(self->reaper, pid, make_unix_random_callback(self));
      
      self->pid = pid;
      self->status = POLL_RUNNING;

      verbose("unix_random.c: Started background poll. pid = %i.\n",
	      pid);
      
      return;

    case -1:
      /* Error */
      werror("Failed to fork background randomness poll (errno = %i): %z\n",
	     errno, STRERROR(errno));
      return;
      
    case 0:
      /* Child */
      {
	struct unix_random_poll_result result;
	struct hash_instance *hash = MAKE_HASH(&sha1_algorithm);

#if 0
	int hang = 1;
	while (hang)
	  sleep(1);
#endif
	int null = open("/dev/null", O_RDONLY);

	if (null < 0)
	  {
	    werror("start_background_poll: Failed to open /dev/null (errno = %i): %z\n",
		   errno, STRERROR(errno));
	    _exit(EXIT_FAILURE);
	  }
	
	if (dup2(null, STDIN_FILENO) < 0)
	  {
	    werror("start_background_poll: dup2 for stdin failed (errno = %i): %z\n",
		   errno, STRERROR(errno));

	    _exit(EXIT_FAILURE);
	  }

	close(null);
	close(output[0]);
	io_set_close_on_exec(output[1]);
	
	/* Change uid */
	if (!getuid())
	  setuid(self->poll_uid);
	
	if (!getuid())
	  _exit(EXIT_FAILURE);

	result.count = background_poll(hash);

	assert(hash->hash_size == sizeof(result.data));
	HASH_DIGEST(hash, result.data);
	
	if (!write_raw(output[1], sizeof(result), (UINT8 *) &result))
	  _exit(EXIT_SUCCESS);

	_exit(EXIT_FAILURE);
      }
    }
}

static void
wait_background_poll(struct unix_random *self)
{
  int status;
  
  assert(self->status == POLL_RUNNING);
  trace("unix_random.c: wait_background_poll\n");
  self->status = POLL_FAILED;
  
  if (waitpid(self->pid, &status, 0) == self->pid)
    {
      if (WIFEXITED(status) && !WEXITSTATUS(status))
	self->status = POLL_FINISHED;
    }

  if (self->reaper)
    REAP(self->reaper, self->pid, NULL);  
}


static unsigned
finish_background_poll(struct unix_random *self, struct hash_instance *hash)
{
  unsigned count = 0;

  switch(self->status)
    {
    case POLL_FINISHED:
      {
	struct unix_random_poll_result result;
	const struct exception *e;
	
	e = read_raw(self->fd, sizeof(result), (UINT8 *) &result);
	
	if (e)
	  werror("Failed to read result from background randomness poll.\n");
	else
	  {
	    HASH_UPDATE(hash, UNIX_RANDOM_POLL_SIZE, result.data);
	    count = result.count;
	  }
	break;
      }
    case POLL_FAILED:
      werror("Background randomness poll failed.\n");
      break;

    case POLL_NO_POLL:
      return 0;
      
    default:
      fatal("finish_background_poll: Internal error.\n");
    }
  close(self->fd);
  self->status = POLL_NO_POLL;

  return count;
}  

static unsigned
use_seed_file(struct hash_instance *hash)
{
  if (getuid())
    {
      /* Lock and read seed file. */
    }
  
  return 0;
}

#define HASH_OBJECT(h, x) HASH_UPDATE((h), sizeof(x), (UINT8 *) &(x))

static unsigned
do_unix_random_slow(struct random_poll *s, struct hash_instance *hash)
{
  CAST(unix_random, self, s);
  unsigned count;
  time_t now = time(NULL); 
  pid_t pid = getpid();

  /* Make sure we don't start with the same state, if several
   * instances are initializing at about the same time. */
  
  HASH_OBJECT(hash, now);
  HASH_OBJECT(hash, pid);
  
  if (self->status == POLL_NO_POLL)
    start_background_poll(self);

  if (self->status == POLL_RUNNING)
    wait_background_poll(self);

  count = finish_background_poll(self, hash);

  /* Do this in the main process, as the background process may run as
   * different user. */
  count += use_seed_file(hash);
  
  return count;
}

static unsigned
do_unix_random_fast(struct random_poll *s, struct hash_instance *hash)
{
  CAST(unix_random, self, s);
  unsigned count = 0;

#if HAVE_GETRUSAGE
  {
    struct rusage rusage;
    if (getrusage(RUSAGE_SELF, &rusage) < 0)
      fatal("do_unix_random_fast: getrusage() failed: (errno = %i) %z\n",
	    errno, STRERROR(errno));
    
    HASH_OBJECT(hash, rusage);
    count += 1;
  }
#endif /* HAVE_GETRUSAGE */
#if HAVE_GETTIMEOFDAY
  {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
      fatal("do_unix_random_fast: gettimeofday failed(errno = %i) %z\n",
	    errno, STRERROR(errno));

    HASH_OBJECT(hash, tv);
  }
#endif /* HAVE_GETTIMEOFDAY */

  {
    /* Fallback that is useful if nothing else works. Count the number
     * of slow polls between time() ticks, and count one bit of
     * entropy if we have more than 2 calls or more than two seconds
     * between calls. */
    
    time_t now = time(NULL);
    self->time_count++;
    if (now != self->previous_time)
      {
	if ( (self->time_count > 2) || ((now - self->previous_time) > 2))
	  count++;
	
	HASH_OBJECT(hash, now);
	HASH_OBJECT(hash, self->time_count);
	
	self->time_count = 0;
	self->previous_time = now;
      }
  }

  return count;
}

static void do_unix_random_background(struct random_poll *s)
{
  CAST(unix_random, self, s);

  if (self->status == POLL_NO_POLL)
    start_background_poll(self);
}

/* Using a NULL reaper argument is ok. It must be supplied only if the
 * application is using a reaper, as that may get to our child process
 * before we can waitpid it. */

struct random_poll *
make_unix_random(struct reap *reaper)
{
  NEW(unix_random, self);

  struct passwd *pw = getpwnam("nobody");
  
  self->super.slow = do_unix_random_slow;
  self->super.fast = do_unix_random_fast;
  self->super.background = do_unix_random_background;
  
  self->reaper = reaper;

  if (pw && pw->pw_uid)
    self->poll_uid = pw->pw_uid;
  else
    self->poll_uid = (uid_t) -1;

  self->status = POLL_NO_POLL;
  self->previous_time = time(NULL);
  self->time_count = 0;

  return &self->super;
}

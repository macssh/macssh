/* io.c
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "io.h"

#include "format.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

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

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/stat.h>


#define GABA_DEFINE
#include "io.h.x"
#undef GABA_DEFINE

#include "io.c.x"

#if MACOS
#define TRACKFTP 1
#if TRACKFTP
#include "lsh_context.h"
#endif
#endif

/* Calls trigged by a signal handler. */
/* GABA:
   (class
     (name lsh_signal_handler)
     (super resource)
     (vars
       (next object lsh_signal_handler)
       (flag . "volatile sig_atomic_t *")
       (action object lsh_callback)))
*/

/* Scheduled calls. FIXME: For now, no scheduling in time. */
/* GABA:
   (class
     (name lsh_callout)
     (super resource)
     (vars
       (next object lsh_callout)
       (action object lsh_callback)))
       ;; (when . time_t)
*/

/* GABA:
   (class
     (name io_backend)
     (vars
       ; Linked list of fds. 
       (files object lsh_fd)

       ; Stack of closed files
       ;; (closed object lsh_fd)

       ; Flags
       (signals object lsh_signal_handler)
       
       ; Callouts
       (callouts object lsh_callout)))
*/



#if MACOS

int io_check_signals(struct io_backend *b);
int io_check_callouts(struct io_backend *b);

int io_check_signals(struct io_backend *b)
{
  int signal_called = 0;
    
  /* Check all flags */
  if (b->signals)
    {
      struct lsh_signal_handler *f;
      struct lsh_signal_handler **f_p;
      for (f_p = &b->signals; (f = *f_p); )
	{
	  if (!f->super.alive)
	    *f_p = f->next;
	  else
	    {
	      if (*f->flag)
		{
		  *f->flag = 0;
		  LSH_CALLBACK(f->action);
		  signal_called = 1;
		}
	      f_p = &f->next;
	    }
	}
    }
  return signal_called;
}

int io_check_callouts(struct io_backend *b)
{
  int callout_needed = 0;
    
  /* Invoke all callouts. Clear the list first; if any callout
   * installs another one, that will not be invoked until the next
   * iteration. */
  if (b->callouts)
    {
      struct lsh_callout *p;
      
      for (p = b->callouts, b->callouts = NULL;
	   p; p = p->next)
	if (p->super.alive)
	  {
		callout_needed = 1;
	  }
    }
      
  return callout_needed;
}

#endif


/* Backend loop */

/* If there's nothing to do for this amount of time (ms), do
 * spontaneous gc. */

#define IDLE_TIME 100

int io_iter(struct io_backend *b)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  unsigned long nfds; /* FIXME: Should be nfds_t if that type is defined */
  struct pollfd *fds;
  struct lsh_fd **active_fds;
  
  /* FIXME: Real scheduling of callouts not implemented */
  /* int timeout; */
  int res;

#if MACOS
  io_check_signals( b );
#else
  /* Check all flags */
  if (b->signals)
    {
      struct lsh_signal_handler *f;
      struct lsh_signal_handler **f_p;
      for (f_p = &b->signals; (f = *f_p); )
	{
	  if (!f->super.alive)
	    *f_p = f->next;
	  else
	    {
	      if (*f->flag)
		{
		  *f->flag = 0;
		  LSH_CALLBACK(f->action);
		}
	      f_p = &f->next;
	    }
	}
    }
#endif
  
  /* Invoke all callouts. Clear the list first; if any callout
   * installs another one, that will not be invoked until the next
   * iteration. */
  if (b->callouts)
    {
      struct lsh_callout *p;
      
      for (p = b->callouts, b->callouts = NULL;
	   p; p = p->next)
	if (p->super.alive)
	  {
	    LSH_CALLBACK(p->action);
	    p->super.alive = 0;
	  }
    }
      
  {
    struct lsh_fd *fd;
    int need_close;
    
    /* Prepare fd:s. This phase calls the prepare-methods, also closes
     * and unlinks any fd:s that should be closed, and also counts how
     * many fd:s there are. */

    for (fd = b->files; fd; fd = fd->next)
      {
	if (fd->super.alive && fd->prepare)
	  FD_PREPARE(fd);
      }
    
    /* Note that calling a close callback might cause other files to
     * be closed as well, so we need a double loop.
     *
     * FIXME: How can we improve this? We could keep a stack of closed
     * files, but that will require backpointers from the fd:s to the
     * backend (so that kill_fd() can find the top of the stack). */

    do
      {
	struct lsh_fd **fd_p;
	need_close = 0;
	nfds = 0;
	
	for(fd_p = &b->files; (fd = *fd_p); )
	  {
	    if (!fd->super.alive)
	      {
		if (fd->fd < 0)
		  /* Unlink the file object, but don't close any
		   * underlying file. */
		  ;
		else
		  {
		    /* Used by write fd:s to make sure that writing to its
		     * buffer fails. */
		    if (fd->write_close)
		      FD_WRITE_CLOSE(fd);
		
		    if (fd->close_callback)
		      {
			LSH_CALLBACK(fd->close_callback);
			need_close = 1;
		      }
		    trace("io.c: Closing fd %i.\n", fd->fd);
		
		    if (close(fd->fd) < 0)
		      {
			werror("io.c: close() failed, (errno = %i): %z\n",
			       errno, STRERROR(errno));
			EXCEPTION_RAISE(fd->e,
					make_io_exception(EXC_IO_CLOSE, fd,
							  errno, NULL));
		      }
		  }
		/* Unlink this fd */
		*fd_p = fd->next;
		continue;
	      }

	    if (fd->want_read || fd->want_write)
	      nfds++;

	    fd_p = &fd->next;
	  }
      } while (need_close);
  }

  if (!nfds)
    /* Nothing more to do.
     *
     * NOTE: There might be some callouts left, but we won't wait for them. */
    return 0;

  fds = alloca(sizeof(struct pollfd) * nfds);
  active_fds = alloca(sizeof(struct lsh_fd *) *nfds);
  
  /* Fill out fds-array */
  {
    struct lsh_fd *fd;
    unsigned long i;
    int all_events = 0;
    
    for (fd = b->files, i = 0; fd; fd = fd->next)
      {
	assert(fd->super.alive);
	
	if (fd->want_read || fd->want_write)
	  {
	    assert(i < nfds);

	    active_fds[i] = fd;

	    fds[i].fd = fd->fd;
	    fds[i].events = 0;
	    
	    if (fd->want_read)
	      fds[i].events |= MY_POLLIN;

	    if (fd->want_write)
	      fds[i].events |= POLLOUT;

	    all_events |= fds[i].events;
	    i++;
	  }
      }
    assert(i == nfds);
    assert(all_events);
  }

#if MACOS
  res = poll(fds, nfds, 0);
  if (!res)
    {
      gc_maybe(&b->super, 0);
      while (!res)
        {
          res = poll(fds, nfds, 0);
          if ( !res && (io_check_signals(b) || io_check_callouts(b)) )
            res = EINTR;
        }
    }
  else
    gc_maybe(&b->super, 1);
#else
  res = poll(fds, nfds, IDLE_TIME);

  if (!res)
    {
      gc_maybe(&b->super, 0);
      res = poll(fds, nfds, -1);
    }
  else
    gc_maybe(&b->super, 1);
#endif
  
  if (!res)
    {
      /* Callouts are not implemented */
      fatal("Unexpected timeout\n");
    }
  if (res < 0)
    switch(errno)
      {
      case EAGAIN:
      case EINTR:
#if ALLOCA_68K_BUG
	ALLOCA_FREE(alloca_ref);
#endif
	return 1;
      default:
	fatal("io_iter: poll failed: %z", STRERROR(errno));
      }
  
  {
    /* Do io. Note that the callback functions may add new fds to the
     * head of the list, or clear the alive flag on any fd. But this
     * is less of a problem now, as we use the active_fd array.*/

    /* struct lsh_fd *fd; */
    unsigned long i;
    
    for(i=0; i<nfds; i++)
      {
	struct lsh_fd *fd = active_fds[i];
	assert(i<nfds);

	debug("io.c: poll for fd %i: events = 0x%xi, revents = 0x%xi.\n",
 	      fds[i].fd, fds[i].events, fds[i].revents);
	
	if (!fd->super.alive)
	  continue;

	/* On systems without poll(), we use jpoll.c to emulate some
	 * of poll(), but we lack POLLNVAL, POLLPRI and POLLHUP. */
#ifdef POLLNVAL
	if (fds[i].revents & POLLNVAL)
	  {
	    werror("io.c: poll request on fd %i, for events of type %xi\n"
		   "      return POLLNVAL, revents = %xi\n",
		   fds[i].fd, fds[i].events, fds[i].revents);
	    kill_fd(fd);
	    continue;
	  }
#endif /* POLLNVAL */

#ifdef POLLHUP
	/* NOTE: The behaviour of poll() at EOF varies quite a lot
	 * between systems.
	 *
	 * According to Solaris' man page, POLLHUP is mutually
	 * exclusive with POLLOUT, but orthogonal to POLLIN.
	 *
	 * However, on my system (sparc-linux) POLLHUP is set when we
	 * get EOF on an fd we are reading.
	 *
	 * I.e. on some systems, EOF is indicated by poll() setting
	 * POLLIN and read() returning 0 (in particular, this happens
	 * if the poll-by-select-code in jpoll.c is used), while on
	 * other systems, poll() sets POLLHUP and subsequent read()
	 * calls will return -1, not 0.
	 *
	 * But to complicate things some more, we can (also on Linux)
	 * get both POLLHUP and POLLIN set. In that case, we do an
	 * ordinary read.
	 *
	 * We set the hanged_up flag before calling FD_READ, which
	 * tells the io_callback that it should avoid calling read(). */

	if (fds[i].revents & POLLHUP)
	  {
	    if (fd->want_write)
	      /* Will raise an i/o error */
	      FD_WRITE(fd);
	    else if (fd->want_read)
	      {
		if (!fd->super.alive)
		  continue;

		/* If reading is not possible, treat this as EOF. */ 
		if (!(fds[i].revents & MY_POLLIN))
		  fd->hanged_up = 1;

		FD_READ(fd);
	      }
	    else
	      {
		werror("io.c: poll said POLLHUP on an inactive fd.\n");
		close_fd(fd);
	      }
	    continue;
	  }
#endif /* POLLHUP */

#ifdef POLLPRI
	if (fds[i].revents & POLLPRI)
	  {
	    werror("io.c: Peer is trying to send Out of Band data. Hanging up.\n");

	    /* FIXME: Should we raise any exception here? */

	    close_fd(fd); 

	    continue;
	  }
#endif /* POLLPRI */
	if (fds[i].revents & POLLOUT)
	  FD_WRITE(fd);

	if (!fd->super.alive)
	  continue;

	if (fds[i].revents & MY_POLLIN)
	  FD_READ(fd);
      }
    assert(i == nfds);
  }

#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
  return 1;
}

  
void
io_run(struct io_backend *b)
{
  struct sigaction pipe;
  memset(&pipe, 0, sizeof(pipe));

  pipe.sa_handler = SIG_IGN;
  sigemptyset(&pipe.sa_mask);
  pipe.sa_flags = 0;
  
  if (sigaction(SIGPIPE, &pipe, NULL) < 0)
    fatal("Failed to ignore SIGPIPE.\n");
  
  while(io_iter(b))
    ;
}

struct io_backend *
make_io_backend(void)
{
  NEW(io_backend, b);

  b->files = NULL;
  b->signals = NULL;
  b->callouts = NULL;

  return b;
}

struct resource *
io_signal_handler(struct io_backend *b,
		  volatile sig_atomic_t *flag,
		  struct lsh_callback *action)
{
  NEW(lsh_signal_handler, handler);
  resource_init(&handler->super, NULL);

  handler->next = b->signals;
  handler->flag = flag;
  handler->action = action;

  b->signals = handler;
  
  return &handler->super;
}

/* Delays not implemented. */
struct resource *
io_callout(struct io_backend *b,
	   UINT32 delay UNUSED,
	   struct lsh_callback *action)
{
  NEW(lsh_callout, callout);
  resource_init(&callout->super, NULL);

  callout->next = b->callouts;
  callout->action = action;
  b->callouts = callout;

  return &callout->super;
}

/* Read-related callbacks */

static void
do_buffered_read(struct io_callback *s,
		 struct lsh_fd *fd)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  CAST(io_buffered_read, self, s);
  UINT8 *buffer = alloca(self->buffer_size);
  int res;

  assert(fd->want_read);   

  /* If hanged_up is set, pretend that read() returned 0 */
  res = fd->hanged_up ? 0 : read(fd->fd, buffer, self->buffer_size);

  if (res < 0)
    switch(errno)
      {
      case EINTR:
	break;
      case EWOULDBLOCK:
	werror("io.c: read_callback: Unexpected EWOULDBLOCK\n");
	break;
      case EPIPE:
	/* Getting EPIPE from read() seems strange, but appearantly
	 * it happens sometimes. */
	werror("Unexpected EPIPE.\n");
      default:
	EXCEPTION_RAISE(fd->e, 
			make_io_exception(EXC_IO_READ, fd,
					  errno, NULL));
	break;
      }
  else if (res > 0)
    {
      UINT32 left = res;
    
      while (fd->super.alive && fd->read && left)
	{
	  UINT32 done;

	  /* FIXME: What to do if want_read is false? To improve the
	   * connection_lock() mechanism, it must be possible to
	   * temporarily stop reading, which means that fd->want_read
	   * has to be cleared.
	   *
	   * But when doing this, we have to keep the data that we
	   * have read, some of which is buffered here, on the stack,
	   * and the rest inside the read-handler.
	   *
	   * There are two alternatives: Save our buffer here, or
	   * continue looping, letting the read-handler process it
	   * into packets. In the latter case, the ssh_connection
	   * could keep a queue of waiting packets, but it would still
	   * have to clear the want_read flag, to prevent that queue
	   * from growing arbitrarily large.
	   *
	   * We now go with the second alternative. */

	  assert(self->handler);

	  /* NOTE: This call may replace self->handler */
	  done = READ_HANDLER(self->handler, left, buffer);
	  
	  buffer += done;
	  left -= done;

	  if (!fd->want_read)
	    debug("do_buffered_read: want_read = 0; handler needs a pause.\n");
	  
	  if (fd->want_read && !self->handler)
	    {
	      werror("do_buffered_read: Handler disappeared! Ignoring %i bytes\n",
		     left);
	      fd->want_read = 0;
#if ALLOCA_68K_BUG
	      ALLOCA_FREE(alloca_ref);
#endif
	      return;
	    }
	}

      if (left)
	verbose("read_buffered(): fd died, %i buffered bytes discarded\n",
		left);
    }
  else
    {
      /* We have read EOF. Pass available == 0 to the handler */
      assert(fd->super.alive);
      assert(fd->read);
      assert(fd->want_read);
      assert(self->handler);

      /* Close the fd, unless it has a write callback. */
      close_fd_read(fd);
      
      READ_HANDLER(self->handler, 0, NULL);
    }
	
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

struct io_callback *
make_buffered_read(UINT32 buffer_size,
		   struct read_handler *handler)
{
  NEW(io_buffered_read, self);

  self->super.f = do_buffered_read;
  self->buffer_size = buffer_size;
  self->handler = handler;

  return &self->super;
}

#if MACOS
#if TRACKFTP

char *strnstr( const char *d, const char *s, int len );

char *strnstr( const char *d, const char *s, int len )
{
#if !__POWERPC__
	unsigned char * e = (unsigned char *) d + len;
	unsigned char * s1 = (unsigned char *) d;
	unsigned char * p1 = (unsigned char *) s;
	unsigned char firstc, c1, c2;
	
	if (!(firstc = *p1++))
		return((char *) d);

	while (s1 < e && (c1 = *s1++) != '\0')
		if (c1 == firstc)
		{
			const unsigned char * s2 = s1;
			const unsigned char * p2 = p1;
			
			while (s2 < e && (c1 = *s2++) == (c2 = *p2++) && c1)
				;
			
			if (!c2)
				return((char *) s1 - 1);
		}
	
	return nil;
#else
	unsigned char * e = (unsigned char *) d + len - 1;
	unsigned char * s1 = (unsigned char *) d-1;
	unsigned char * p1 = (unsigned char *) s-1;
	unsigned long firstc, c1, c2;
	
	if (!(firstc = *++p1))
		return((char *) d);

	while ( s1 < e && (c1 = *++s1) != '\0')
		if (c1 == firstc)
		{
			const unsigned char * s2 = s1-1;
			const unsigned char * p2 = p1-1;
			
			while ( s2 < e && (c1 = *++s2) == (c2 = *++p2) && c1)
				;
			
			if (!c2)
				return((char *) s1);
		}
	
	return nil;
#endif
}

static char *getaddrport(char *s, unsigned long *addr, unsigned short *port)
{
	long h1,h2,h3,h4,p1,p2;
	h1 = strtol(s,&s,10);++s;
	h2 = strtol(s,&s,10);++s;
	h3 = strtol(s,&s,10);++s;
	h4 = strtol(s,&s,10);++s;
	p1 = strtol(s,&s,10);++s;
	p2 = strtol(s,&s,10);
	*addr = (h1<<24) | (h2<<16) | (h3<<8) | (h4);
	*port = (p1<<8) | p2;
	return (*addr && *port) ? s : NULL;
}

static void trackclientport(struct lsh_string *s)
{
  unsigned long addr;
  unsigned short port;
  char *e;

  if ( s->length >= 16
    && !memcmp(s->data, "PORT ", 5)
    && (e = getaddrport((char *)s->data + 5, &addr, &port)) != NULL ) {
    /*plnt_printf("trackclientport found : %d\n", port);*/
  }
}

static void trackserverport(struct write_buffer *buf, UINT32 size)
{
  unsigned long addr;
  unsigned short port;
  char *s;
  char *e;

  if ( size >= 34
    && (s = strnstr((char *)buf->buffer + buf->start, "Entering Passive Mode (", 32)) != NULL
    && (e = getaddrport(s + 23, &addr, &port)) != NULL ) {
    /*plnt_printf("trackserverport found : %d\n", port);*/

    /* launch a new thread with forwarded port to the same SSH2 host, */
    /* forwarded to remote host, dst_port, listening on localhost, dst_port  */
    /* then replace dst_ip & dst_port in packet with localhost, dst_port */

/*
(unsigned short)w->localport, w->remotehost, (unsigned short)w->remoteport

    if ( opendynport( addr, port, &port ) ) {

    }


	object_queue_add_tail(&self->actions,
			      &make_forward_local_port
			      (self->backend,
			       make_address_info((self->with_remote_peers
						  ? NULL
						  : ssh_format("%lz", "127.0.0.1")),
						 listen_port),
			       target)->super);


*/

  }
}
#endif
#endif


static void
do_consuming_read(struct io_callback *c,
		  struct lsh_fd *fd)
{
  CAST_SUBTYPE(io_consuming_read, self, c);
  UINT32 wanted = READ_QUERY(self);
#if MACOS
#if TRACKFTP
  lshcontext *context;
#endif
#endif

  assert(fd->want_read);

  if (fd->hanged_up)
    {
      /* If hanged_up is set, pretend that read() returned 0 */
      goto eof;
    }
  
  if (!wanted)
    {
      fd->want_read = 0;
    }
  else
    {
      struct lsh_string *s = lsh_string_alloc(wanted);
      int res = read(fd->fd, s->data, wanted);

      if (res < 0)
	switch(errno)
	  {
	  case EINTR:
	    break;
	  case EWOULDBLOCK:
	    werror("io.c: read_consume: Unexpected EWOULDBLOCK\n");
	    break;
	  case EPIPE:
	    /* FIXME: I don't understand why reading should return
	     * EPIPE, but it happens occasionally under linux. Perhaps
	     * we should treat it as EOF instead? */
	    werror("io.c: read_consume: Unexpected EPIPE.\n");
	  default:
	    EXCEPTION_RAISE(fd->e, 
			    make_io_exception(EXC_IO_READ,
					      fd, errno, NULL));
	    break;
	  }
      else if (res > 0)
	{
	  s->length = res;
#if MACOS
#if TRACKFTP
	  context = pthread_getspecific(ssh2threadkey);
	  if ( context && context->_forward && context->_localport == 21 ) {
	    trackclientport(s);
	  }
#endif
#endif
	  A_WRITE(self->consumer, s);
	}
      else
	{
	eof:
	  /* Close the fd, unless it has a write callback. */
	  close_fd_read(fd);
	  A_WRITE(self->consumer, NULL);
	}
    }
}

/* NOTE: Doesn't initialize the query field. That should be done in
 * the subclass's constructor. */
void init_consuming_read(struct io_consuming_read *self,
			 struct abstract_write *consumer)
{
  self->super.f = do_consuming_read;
  self->consumer = consumer;
}


/* Write related callbacks */
static void
do_write_callback(struct io_callback *s UNUSED,
		  struct lsh_fd *fd)
{
  /* CAST(io_write_callback, self, s); */
  UINT32 size;
  int res;
  
  size = MIN(fd->write_buffer->end - fd->write_buffer->start,
	     fd->write_buffer->block_size);
  assert(size);
  
  res = write(fd->fd,
	      fd->write_buffer->buffer + fd->write_buffer->start,
	      size);
  if (!res)
    fatal("Closed?");
  if (res < 0)
    switch(errno)
      {
      case EINTR:
      case EAGAIN:
	break;
      case EPIPE:
	debug("io.c: Broken pipe.\n");
	close_fd(fd);
	break;
      default:
	werror("io.c: write failed, %z\n", STRERROR(errno));
	EXCEPTION_RAISE(fd->e,
			make_io_exception(EXC_IO_WRITE, fd, errno, NULL));
	close_fd(fd);
	
	break;
      }
  else
    write_buffer_consume(fd->write_buffer, res);
}  

static struct io_callback io_write_callback =
{ STATIC_HEADER, do_write_callback };

static void
do_write_prepare(struct lsh_fd *fd)
{
  assert(fd->write_buffer);

  if (! (fd->want_write = write_buffer_pre_write(fd->write_buffer))
      && fd->write_buffer->closed)
    close_fd(fd);
}

/* NONO */

/* Write related callbacks */
static void
do_fdw_write_callback(struct io_callback *s UNUSED,
		  struct lsh_fd *fd)
{
  /* CAST(io_write_callback, self, s); */
  UINT32 size;
  int res;
#if MACOS
#if TRACKFTP
  lshcontext *context;
#endif
#endif
  
  size = MIN(fd->write_buffer->end - fd->write_buffer->start,
	     fd->write_buffer->block_size);
  assert(size);

#if MACOS
#if TRACKFTP
  context = pthread_getspecific(ssh2threadkey);
  if ( context && context->_forward && context->_localport == 21 ) {
    trackserverport(fd->write_buffer, size);
    size = MIN(fd->write_buffer->end - fd->write_buffer->start,
	       fd->write_buffer->block_size);
    assert(size);
  }
#endif
#endif

  res = write(fd->fd,
	      fd->write_buffer->buffer + fd->write_buffer->start,
	      size);
  if (!res)
    fatal("Closed?");
  if (res < 0)
    switch(errno)
      {
      case EINTR:
      case EAGAIN:
	break;
      case EPIPE:
	debug("io.c: Broken pipe.\n");
	close_fd(fd);
	break;
      default:
	werror("io.c: write failed, %z\n", STRERROR(errno));
	EXCEPTION_RAISE(fd->e,
			make_io_exception(EXC_IO_WRITE, fd, errno, NULL));
	close_fd(fd);
	
	break;
      }
  else
    write_buffer_consume(fd->write_buffer, res);
}  

static struct io_callback io_fwd_write_callback =
{ STATIC_HEADER, do_fdw_write_callback };

/* NONO */

static void
do_write_close(struct lsh_fd *fd)
{
  /* CAST(io_write_callback, self, s); */

  assert(fd->write_buffer);

  write_buffer_close(fd->write_buffer);
}

struct listen_value *
make_listen_value(struct lsh_fd *fd,
		  struct address_info *peer)
{
  NEW(listen_value, self);

  self->fd = fd;
  self->peer = peer;

  return self;
}


/* Listen callback */

/* GABA:
   (class
     (name io_listen_callback)
     (super io_callback)
     (vars
       (backend object io_backend)
       (c object command_continuation)
       (e object exception_handler)))
*/

static void
do_listen_callback(struct io_callback *s,
		   struct lsh_fd *fd)
{
  CAST(io_listen_callback, self, s);

#if WITH_IPV6
  struct sockaddr_storage peer;
#else
  struct sockaddr peer;
#endif

  socklen_t addr_len = sizeof(peer);
  int conn;

  conn = accept(fd->fd,
		(struct sockaddr *) &peer, &addr_len);
  if (conn < 0)
    {
      werror("io.c: accept() failed, %z", STRERROR(errno));
      return;
    }
  trace("io.c: accept() on fd %i\n", conn);
  COMMAND_RETURN(self->c,
		 make_listen_value(make_lsh_fd(self->backend,
					       conn, self->e),
				   sockaddr2info(addr_len,
						 (struct sockaddr *) &peer)));
}

struct io_callback *
make_listen_callback(struct io_backend *backend,
		     struct command_continuation *c,
		     struct exception_handler *e)
{
  NEW(io_listen_callback, self);
  self->super.f = do_listen_callback;
  self->backend = backend;
  self->c = c;
  self->e = e;
  
  return &self->super;
}

#if 0

static void
do_listen_callback_no_peer(struct io_callback *s,
			   struct lsh_fd *fd)
{
  CAST(io_listen_callback, self, s);

  int conn;

  conn = accept(fd->fd,
		(struct sockaddr *) &peer, &addr_len);
  if (conn < 0)
    {
      werror("io.c: accept() failed, %z", STRERROR(errno));
      return;
    }
  trace("io.c: accept() on fd %i\n", conn);
  COMMAND_RETURN(self->c, make_lsh_fd(self->backend,
				      conn, self->e));
}

struct io_callback *
make_listen_callback_no_peer(struct io_backend *backend,
			     struct command_continuation *c)
{
  NEW(io_listen_callback, self);
  self->super.f = do_listen_callback_no_peer;
  self->backend = backend;
  self->c = c;
  
  return &self->super;
}
#endif

/* Connect callback */

/* GABA:
   (class
     (name io_connect_callback)
     (super io_callback)
     (vars
       (c object command_continuation)))
*/

static void
do_connect_callback(struct io_callback *s,
		    struct lsh_fd *fd)
{
  CAST(io_connect_callback, self, s);
  int socket_error;
  socklen_t len = sizeof(socket_error);
  
  /* Check if the connection was successful */
  if ((getsockopt(fd->fd, SOL_SOCKET, SO_ERROR,
		  (char *) &socket_error, &len) < 0)
      || socket_error)
    {
      debug("io.c: connect_callback: Connect failed.\n");
      EXCEPTION_RAISE(fd->e,
		      make_io_exception(EXC_IO_CONNECT, fd, 0, "connect() failed."));
      kill_fd(fd);
    }
  else
    {
      fd->write = NULL;
      fd->want_write = 0;
      COMMAND_RETURN(self->c, fd);
    }
}

static struct io_callback *
make_connect_callback(struct command_continuation *c)
{
  NEW(io_connect_callback, self);

  self->super.f = do_connect_callback;
  self->c = c;

  return &self->super;
}


/* This function is called if a connection this file somehow depends
 * on disappears. For instance, the connection may have spawned a
 * child process, and this file may be the stdin of that process. */

/* To kill a file, mark it for closing and the backend will do the work. */
static void do_kill_fd(struct resource *r)
{
  CAST_SUBTYPE(lsh_fd, fd, r);

  /* We use close_fd_nicely, so that any data in the write buffer is
   * flushed before the fd is closed. */
  if (r->alive)
    close_fd_nicely(fd);
}

/* Closes the file on i/o errors, and passes the exception on */

static void
do_exc_io_handler(struct exception_handler *self,
		  const struct exception *x)
{
  if (x->type & EXC_IO)
    {
      CAST_SUBTYPE(io_exception, e, x);

      if (e->fd)
	close_fd(e->fd);
    }
  EXCEPTION_RAISE(self->parent, x);
  return;
}

/* Initializes a file structure, and adds it to the backend's list. */
static void
init_file(struct io_backend *b, struct lsh_fd *f, int fd,
	  struct exception_handler *e)
{
  resource_init(&f->super, do_kill_fd);

  f->fd = fd;

  f->e = make_exception_handler(do_exc_io_handler, e, HANDLER_CONTEXT);
  
  f->close_callback = NULL;

  f->prepare = NULL;

  f->hanged_up = 0;
  
  f->want_read = 0;
  f->read = NULL;

  f->want_write = 0;
  f->write = NULL;
  f->write_close = NULL;

  f->next = b->files;
  b->files = f;
}

/* These functions are used by werror() and friends */

/* For fd:s in blocking mode. */
const struct exception *
write_raw(int fd, UINT32 length, const UINT8 *data)
{
  while(length)
    {
      int written = write(fd, data, length);

      if (written < 0)
	switch(errno)
	  {
	  case EINTR:
	  case EAGAIN:
	    continue;
	  default:
	    return make_io_exception(EXC_IO_BLOCKING_WRITE,
				     NULL, errno, NULL);
	  }
      
      length -= written;
      data += written;
    }
  return NULL;
}

const struct exception *
write_raw_with_poll(int fd, UINT32 length, const UINT8 *data)
{
  while(length)
    {
      struct pollfd pfd;
      int res;
      int written;
      
      pfd.fd = fd;
      pfd.events = POLLOUT;

      res = poll(&pfd, 1, -1);

      if (res < 0)
	switch(errno)
	  {
	  case EINTR:
	  case EAGAIN:
	    continue;
	  default:
	    return make_io_exception(EXC_IO_BLOCKING_WRITE,
				     NULL, errno, NULL);
	  }
      
      written = write(fd, data, length);

      if (written < 0)
	switch(errno)
	  {
	  case EINTR:
	  case EAGAIN:
	    continue;
	  default:
	    return make_io_exception(EXC_IO_BLOCKING_WRITE,
				     NULL, errno, NULL);
	  }
      
      length -= written;
      data += written;
    }
  return NULL;
}

/* For fd:s in blocking mode. */
const struct exception *
read_raw(int fd, UINT32 length, UINT8 *data)
{
  while(length)
    {
      int done = read(fd, data, length);

      if (done < 0)
	switch(errno)
	  {
	  case EINTR:
	  case EAGAIN:
	    continue;
	  default:
	    return make_io_exception(EXC_IO_BLOCKING_READ,
				     NULL, errno, NULL);
	  }
      else if (done == 0)
	{
	  /* EOF. */
	  /* FIXME: Indicate the amount of data read, somehow. */
	  return make_io_exception(EXC_IO_BLOCKING_READ,
				   NULL, 0, NULL);
	}
	
      length -= done;
      data += done;
    }
  return NULL;
}

/* Network utility functions */

/* Converts a string port number or service name to a port number.
 * Returns the port number in _host_ byte order, or 0 if lookup
 * fails. */

int get_portno(const char *service, const char *protocol)
{
  if (service == NULL)
    return 0;
  else
    {
      char *end;
      long portno;

      portno = strtol(service, &end, 10);
      if (portno > 0
	  &&  portno <= 65535
	  &&  end != service
	  &&  *end == '\0')
	  return portno;
      else
	{
	  struct servent * serv;

	  serv = getservbyname(service, protocol);
	  if (!serv)
	    return 0;
	  return ntohs(serv->s_port);
	}
    }
}


/* If def != 0, use that value as a fallback if the lookup fails. */
struct address_info *
make_address_info_c(const char *host,
		    const char *port,
		    int def)
{
  int portno = get_portno(port, "tcp");
  if (!portno)
    portno = def;

  if (!portno)
    return NULL;

  else
    {
      NEW(address_info, info);
      
      info->port = portno;
      info->ip = host ? ssh_format("%lz", host) : NULL;
      
      return info;
    }
}

struct address_info *
make_address_info(struct lsh_string *host, UINT32 port)
{
  NEW(address_info, info);

  info->port = port; /* htons(port); */
  info->ip = host;
  return info;
}

struct address_info *
sockaddr2info(size_t addr_len UNUSED,
	      struct sockaddr *addr)
{
  NEW(address_info, info);
  
  switch(addr->sa_family)
    {
    case AF_INET:
      {
	struct sockaddr_in *in = (struct sockaddr_in *) addr;
	UINT32 ip = ntohl(in->sin_addr.s_addr);
	info->port = ntohs(in->sin_port);
	info->ip = ssh_format("%di.%di.%di.%di",
			      (ip >> 24) & 0xff,
			      (ip >> 16) & 0xff,
			      (ip >> 8) & 0xff,
			      ip & 0xff);
	return info;
      }
#if WITH_IPV6
    case AF_INET6:
      {
	struct sockaddr_in6 *in = (struct sockaddr_in6 *) addr;
	UINT8 *ip = in->sin6_addr.s6_addr;
	info->port = ntohs(in->sin6_port);
	info->ip = ssh_format("%xi:%xi:%xi:%xi:%xi:%xi:%xi:%xi",
			      (ip[0]  << 8) | ip[1],
			      (ip[2]  << 8) | ip[3],
			      (ip[4]  << 8) | ip[5],
			      (ip[6]  << 8) | ip[7],
			      (ip[8]  << 8) | ip[9],
			      (ip[10] << 8) | ip[11],
			      (ip[12] << 8) | ip[13],
			      (ip[14] << 8) | ip[15]);
	return info;
      }
#endif
    case AF_UNIX:
      /* Silently return NULL. This happens when a gateway client
       * connects. */
      return NULL;
    default:
      werror("io.c: sockaddr2info(): Unsupported address family.\n");
      return NULL;
    }
}

#if HAVE_GETADDRINFO
static struct addrinfo *
choose_address(struct addrinfo *list,
	       const int *preference)
{
  int i;
  for (i = 0; preference[i]; i++)
    {
      struct addrinfo *p;
      for (p = list; p; p = p->ai_next)
	if (preference[i] == p->ai_family)
	  return p;
    }
  return NULL;
}
#endif /* HAVE_GETADDRINFO */

/* FIXME: Perhaps this function should be changed to return a list of
 * sockaddr:s? */
struct sockaddr *
address_info2sockaddr(socklen_t *length,
		      struct address_info *a,
		      /* Preferred address families. Zero-terminated array. */
		      const int *preference,
		      int lookup)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  char *host;

  if (a->ip)
    {
      host = alloca(a->ip->length + 1);
  
      memcpy(host, a->ip->data, a->ip->length);
      host[a->ip->length] = '\0';
    }
  else
    host = NULL;

/* Some systems have getaddrinfo(), but still doesn't implement all of
 * RFC 2553 */
#if defined(HAVE_GETADDRINFO) && \
    defined(HAVE_GAI_STRERROR) && defined(HAVE_AI_NUMERICHOST)
  {
    struct addrinfo hints;
    struct addrinfo *list;
    struct addrinfo *chosen;
    struct sockaddr *res;
    const int default_preference
#if WITH_IPV6
      [3] = { AF_INET6, AF_INET }
#else
      [2] = { AF_INET, 0 }
#endif      
      ;
    int err;
    /* FIXME: It seems ugly to have to convert the port number to a
     * string. */
    struct lsh_string *service = ssh_cformat("%di", a->port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (!lookup)
      hints.ai_flags |= AI_NUMERICHOST;
    
    err = getaddrinfo(host, service->data, &hints, &list);
    lsh_string_free(service);

    if (err)
      {
	debug("address_info2sockaddr: getaddrinfo failed (err = %d): %z\n",
	      err, gai_strerror(err));
#if ALLOCA_68K_BUG
	ALLOCA_FREE(alloca_ref);
#endif
	return NULL;
      }

    chosen = choose_address(list,
			    preference ? preference : default_preference);
    if (!chosen)
      {
	freeaddrinfo(list);
#if ALLOCA_68K_BUG
	ALLOCA_FREE(alloca_ref);
#endif
	return NULL;
      }
    
    *length = chosen->ai_addrlen;
    
    res = lsh_space_alloc(*length);
    memcpy(res, chosen->ai_addr, *length);
    freeaddrinfo(list);

#if ALLOCA_68K_BUG
    ALLOCA_FREE(alloca_ref);
#endif
    return res;
  }
#else
/* !(defined(HAVE_GETADDRINFO) &&
     defined(HAVE_GAI_STRERROR) && defined(HAVE_AI_NUMERICHOST) */ 

#if WITH_IPV6
#error IPv6 enabled, but getaddrinfo() and friends were not found. 
#endif

  if (a->ip && memchr(a->ip->data, ':', a->ip->length))
    {
      debug("address_info2sockaddr: Literal IPv6 used. Failing.\n");
#if ALLOCA_68K_BUG
      ALLOCA_FREE(alloca_ref);
#endif
      return NULL;
    }
  else
    {
      struct sockaddr_in *addr;
      NEW_SPACE(addr);

      *length = sizeof(*addr);
      addr->sin_port = htons(a->port);

      /* Use IPv4 only */
      addr->sin_family = AF_INET;
    
      if (!host)
	/* Any interface */
	addr->sin_addr.s_addr = INADDR_ANY;

      else
	{
	  /* First check for numerical ip-number */
#if HAVE_INET_ATON
	  if (!inet_aton(host, &addr->sin_addr))
#else /* !HAVE_INET_ATON */
	    /* NOTE: It is wrong to work with ((unsigned long int) -1)
	     * directly, as this breaks Linux/Alpha systems. But
	     * INADDR_NONE isn't portable. That's what inet_aton is for;
	     * see the GNU libc documentation. */
# ifndef INADDR_NONE
# define INADDR_NONE ((unsigned long int) -1)
# endif /* !INADDR_NONE */
	  addr->sin_addr.s_addr = inet_addr(host);
	  if (addr->sin_addr.s_addr == INADDR_NONE)
#endif  /* !HAVE_INET_ATON */
	    {
	      struct hostent *hp;

	      if (! (lookup 
		     && (hp = gethostbyname(host))
		     && (hp->h_addrtype == AF_INET)))
		{
		  lsh_space_free(addr);
#if ALLOCA_68K_BUG
		  ALLOCA_FREE(alloca_ref);
#endif
		  return NULL;
		}

	      memcpy(&addr->sin_addr, hp->h_addr, hp->h_length);
	    }
	}
#if ALLOCA_68K_BUG
      ALLOCA_FREE(alloca_ref);
#endif
      return (struct sockaddr *) addr;
    }
#endif /* !HAVE_GETADDRINFO */  
}


void io_set_nonblocking(int fd)
{
#ifndef MACOS
  int old = fcntl(fd, F_GETFL);

  if (old < 0)
    fatal("io_set_nonblocking: fcntl(F_GETFL) failed, %z", STRERROR(errno));
  
  if (fcntl(fd, F_SETFL, old | O_NONBLOCK) < 0)
    fatal("io_set_nonblocking: fcntl(F_SETFL) failed, %z", STRERROR(errno));
#else
    /*printf("io_set_nonblocking: fd : %d\n", fd);
    fflush(stdout);*/
#endif
}

void io_set_close_on_exec(int fd)
{
#ifndef MACOS
  /* NOTE: There's only one documented flag bit, so reading the old
   * value should be redundant. */
  
  int old = fcntl(fd, F_GETFD);

  if (old < 0)
    fatal("io_set_nonblocking: fcntl(F_GETFD) failed, %z", STRERROR(errno));
  
  if (fcntl(fd, F_SETFD, old | 1) < 0)
    fatal("Can't set close-on-exec flag for fd %i: %z\n",
	  fd, STRERROR(errno));
#else
    /*printf("io_set_close_on_exec: fd : %d\n", fd);
    fflush(stdout);*/
#endif
}


/* ALL file descripters handled by the backend should use non-blocking mode,
 * and have the close-on-exec flag set. */

void io_init_fd(int fd)
{
  io_set_nonblocking(fd);
  io_set_close_on_exec(fd);
}

struct lsh_fd *
make_lsh_fd(struct io_backend *b,
	    int fd,
	    struct exception_handler *e)
{
  NEW(lsh_fd, f);

  io_init_fd(fd);
  init_file(b, f, fd, e);

  return f;
}

/* Some code is taken from Thomas Bellman's tcputils. */
struct lsh_fd *
io_connect(struct io_backend *b,
	   struct sockaddr *remote,
	   socklen_t remote_length,
	   struct command_continuation *c,
	   struct exception_handler *e)
{
  int s = socket(remote->sa_family, SOCK_STREAM, 0);
  struct lsh_fd *fd;
  
  if (s<0)
    return NULL;

  trace("io.c: Connecting using fd %i\n", s);
  
  io_init_fd(s);

#if 0
  if (local  &&  bind(s, (struct sockaddr *)local, sizeof *local) < 0)
    {
      int saved_errno = errno;
      close(s);
      errno = saved_errno;
      return NULL;
    }
#endif
  
  if ( (connect(s, remote, remote_length) < 0)
       && (errno != EINPROGRESS) )       
    {
      int saved_errno = errno;
      close(s);
      errno = saved_errno;
      return NULL;
    }

  fd = make_lsh_fd(b, s, e);
  
  fd->want_write = 1;
  fd->write = make_connect_callback(c);
    
  return fd;
}

struct lsh_fd *
io_listen(struct io_backend *b,
	  struct sockaddr *local,
	  socklen_t length,
	  struct io_callback *callback,
	  struct exception_handler *e)
{
  int s = socket(local->sa_family, SOCK_STREAM, 0);
  struct lsh_fd *fd;
  
  if (s<0)
    return NULL;

  trace("io.c: Listening on fd %i\n", s);
  
  io_init_fd(s);

#ifndef MACOS
  {
    int yes = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof yes);
  }
#endif

  if (bind(s, (struct sockaddr *)local, length) < 0)
    {
      close(s);
      return NULL;
    }

  if (listen(s, 256) < 0) 
    {
      close(s);
      return NULL;
    }

  fd = make_lsh_fd(b, s, e);

  fd->want_read = 1;
  fd->read = callback;

  return fd;
}


/* AF_LOCAL sockets */

/* Requires DIRECTORY and NAME to be NUL-terminated */

struct local_info *
make_local_info(struct lsh_string *directory,
		struct lsh_string *name)
{
  if (!directory || !name || memchr(name->data, '/', name->length))
    return NULL;

  assert(NUL_TERMINATED(directory));
  assert(NUL_TERMINATED(name));
  
  {
    NEW(local_info, self);
    self->directory = directory;
    self->name = name;
    return self;
  }
}

static void
safe_popd(int old_cd, const char *directory)
{
  while (fchdir(old_cd) < 0)
    if (errno != EINTR)
      fatal("io.c: Failed to cd back from %z (errno = %i): %z\n",
	    directory, errno, STRERROR(errno));
      
  close(old_cd);
}

/* Changes the cwd, making sure that it it has reasonable permissions,
 * and that we can change back later. */
static int
safe_pushd(const char *directory,
	   int create)
{
  int old_cd;
  struct stat sbuf;

  if (create)
    {  
      /* First create the directory, in case it doesn't yet exist. */
      if ( (mkdir(directory, 0700) < 0)
	   && (errno != EEXIST) )
	{
	  werror("io.c: Creating directory %z failed "
		 "(errno = %i): %z\n", directory, errno, STRERROR(errno));
	}
    }
    
  /* cd to it, but first save old cwd */

  old_cd = open(".", O_RDONLY);
  if (old_cd < 0)
    {
      werror("io.c: open(\".\") failed.\n");
      return -1;
    }

  /* Test if we are allowed to cd to our current working directory. */
  while (fchdir(old_cd) < 0)
    if (errno != EINTR)
      {
	werror("io.c: fchdir(\".\") failed (errno = %i): %z\n",
	       errno, strerror(errno));
	close(old_cd);
	return -1;
      }

  /* As far as I have been able to determine, all checks for
   * fchdir:ability is performed at the time the directory was opened.
   * Even if the directory is chmod:et to zero, or unlinked, we can
   * probably fchdir back to old_cd later. */

  while (chdir(directory) < 0)
    if (errno != EINTR)
      {
	close(old_cd);
	return -1;
      }

  /* Check that it has reasonable permissions */
  if (stat(".", &sbuf) < 0)
    {
      werror("io.c: Failed to stat() \".\" (supposed to be %z).\n"
	     "  (errno = %i): %z\n", directory, errno, STRERROR(errno));

      safe_popd(old_cd, directory);
      return -1;
    }

  if (sbuf.st_uid != getuid())
    {
      werror("io.c: Socket directory %z not owned by user.\n", directory);

      safe_popd(old_cd, directory);
      return -1;
    }
    
  if (sbuf.st_mode & (S_IRWXG | S_IRWXO))
    {
      werror("io.c: Permission bits on %z are too loose.\n", directory);

            safe_popd(old_cd, directory);
      return -1;
    }

  return old_cd;
}


struct lsh_fd *
io_listen_local(struct io_backend *b,
		struct local_info *info,
		struct io_callback *callback,
		struct exception_handler *e)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  int old_cd;

  mode_t old_umask;
  struct sockaddr_un *local;
  socklen_t local_length;

  struct lsh_fd *fd;
  
  assert(info->directory && NUL_TERMINATED(info->directory));
  assert(info->name && NUL_TERMINATED(info->name));

  /* NAME should not be a plain filename, with no directory separators.
   * In particular, it should not be an absolute filename. */
  assert(!memchr(info->name->data, '/', info->name->length));

  local_length = OFFSETOF(struct sockaddr_un, sun_path) + info->name->length;
  local = alloca(local_length);

  local->sun_family = AF_UNIX;
  memcpy(local->sun_path, info->name->data, info->name->length);

  /* cd to it, but first save old cwd */

  old_cd = safe_pushd(info->directory->data, 1);
  if (old_cd < 0)
    {
#if ALLOCA_68K_BUG
	  ALLOCA_FREE(alloca_ref);
#endif
	  return NULL;
    }

  /* Ok, now the current directory should be a decent place for
   * creating a socket. */

  /* Try unlinking any existing file. */
  if ( (unlink(info->name->data) < 0)
       && (errno != ENOENT))
    {
      werror("io.c: unlink '%S'/'%S' failed (errno = %i): %z\n",
	     info->directory, info->name, errno, STRERROR(errno));
      safe_popd(old_cd, info->directory->data);
#if ALLOCA_68K_BUG
      ALLOCA_FREE(alloca_ref);
#endif
      return NULL;
    }

  /* We have to change the umask, as that's the only way to control
   * the permissions that bind() uses. */
#ifndef MACOS
  old_umask = umask(0770);
#endif
  /* Bind and listen */
  fd = io_listen(b, (struct sockaddr *) local, local_length, callback, e);
  
  /* Ok, now we restore umask and cwd */
#ifndef MACOS
  umask(old_umask);
#endif
  safe_popd(old_cd, info->directory->data);

#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
  return fd;
}

/* Requires DIRECTORY and NAME to be NUL-terminated */
struct lsh_fd *
io_connect_local(struct io_backend *b,
		 struct local_info *info,
		 struct command_continuation *c,
		 struct exception_handler *e)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  int old_cd;

  struct sockaddr_un *addr;
  socklen_t addr_length;

  struct lsh_fd *fd;
  
  assert(info->directory && NUL_TERMINATED(info->directory));
  assert(info->name && NUL_TERMINATED(info->name));

  /* NAME should not be a plain filename, with no directory separators.
   * In particular, it should not be an absolute filename. */
  assert(!memchr(info->name->data, '/', info->name->length));

  addr_length = OFFSETOF(struct sockaddr_un, sun_path) + info->name->length;
  addr = alloca(addr_length);

  addr->sun_family = AF_UNIX;
  memcpy(addr->sun_path, info->name->data, info->name->length);

  /* cd to it, but first save old cwd */

  old_cd = safe_pushd(info->directory->data, 0);
  if (old_cd < 0)
    {
#if ALLOCA_68K_BUG
      ALLOCA_FREE(alloca_ref);
#endif
	  return NULL;
    }
  
  fd = io_connect(b, (struct sockaddr *) addr, addr_length, c, e);

  safe_popd(old_cd, info->directory->data);

#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
  return fd;
}

/* Constructors */

/* NONO */
struct lsh_fd *
fwd_io_read_write(struct lsh_fd *fd,
	      struct io_callback *read,
	      UINT32 block_size,
	      struct lsh_callback *close_callback)
{
  trace("io.c: Preparing fd %i for reading and writing\n",
	fd->fd);
  
  /* Reading */
  fd->read = read;
  fd->want_read = !!read;
  
  /* Writing */
  fd->write_buffer = write_buffer_alloc(block_size);
  fd->write = &io_fwd_write_callback;

  fd->prepare = do_write_prepare;
  fd->write_close = do_write_close;
  
  /* Closing */
  fd->close_callback = close_callback;

  return fd;
}
/* NONO */

struct lsh_fd *
io_read_write(struct lsh_fd *fd,
	      struct io_callback *read,
	      UINT32 block_size,
	      struct lsh_callback *close_callback)
{
  trace("io.c: Preparing fd %i for reading and writing\n",
	fd->fd);
  
  /* Reading */
  fd->read = read;
  fd->want_read = !!read;
  
  /* Writing */
  fd->write_buffer = write_buffer_alloc(block_size);
  fd->write = &io_write_callback;

  fd->prepare = do_write_prepare;
  fd->write_close = do_write_close;
  
  /* Closing */
  fd->close_callback = close_callback;

  return fd;
}

struct lsh_fd *
io_read(struct lsh_fd *fd,
	struct io_callback *read,
	struct lsh_callback *close_callback)
{
  trace("io.c: Preparing fd %i for reading\n", fd->fd);
  
  /* Reading */
  fd->want_read = !!read;
  fd->read = read;
  
  fd->close_callback = close_callback;

  return fd;
}

struct lsh_fd *
io_write(struct lsh_fd *fd,
	 UINT32 block_size,
	 struct lsh_callback *close_callback)
{
  trace("io.c: Preparing fd %i for writing\n", fd->fd);
  
  /* Writing */
  fd->write_buffer = write_buffer_alloc(block_size);
  fd->write = &io_write_callback;

  fd->prepare = do_write_prepare;
  fd->write_close = do_write_close;

  fd->close_callback = close_callback;

  return fd;
}

struct lsh_fd *
io_write_file(struct io_backend *backend,
	      const char *fname, int flags, int mode,
	      UINT32 block_size,
	      struct lsh_callback *c,
	      struct exception_handler *e)
{
  int fd = open(fname, flags, mode);
  if (fd < 0)
    return NULL;

  return io_write(make_lsh_fd(backend, fd, e), block_size, c);
}

struct lsh_fd *
io_read_file(struct io_backend *backend,
	     const char *fname, 
	     struct exception_handler *e)
{
  int fd = open(fname, O_RDONLY);
  if (fd < 0)
    return NULL;

  return make_lsh_fd(backend, fd, e);
}

void kill_fd(struct lsh_fd *fd)
{
  fd->super.alive = 0;
}

void close_fd(struct lsh_fd *fd)
{
  trace("io.c: Marking fd %i for closing.\n", fd->fd);
  kill_fd(fd);
}

void close_fd_nicely(struct lsh_fd *fd)
{
  /* Don't attempt to read any further. */

  fd->want_read = 0;
  fd->read = NULL;
  
  if (fd->write_close)
    /* Mark the write_buffer as closed */
    FD_WRITE_CLOSE(fd);
  else
    /* There's no data buffered for write. */
    kill_fd(fd);
}

/* Stop reading, but if the fd has a write callback, keep it open. */
void close_fd_read(struct lsh_fd *fd)
{
  fd->want_read = 0;
  fd->read = NULL;
  
  if (!fd->write)
    /* We won't be writing anything on this fd, so close it. */
    kill_fd(fd);
}

/* Responsible for handling the EXC_FINISH_READ exception. It should
 * be a parent to the connection related exception handlers, as for
 * instance the protocol error handler will raise the EXC_FINISH_READ
 * exception. */
/* GABA:
   (class
     (name exc_finish_read_handler)
     (super exception_handler)
     (vars
       (fd object lsh_fd)))
*/

static void
do_exc_finish_read_handler(struct exception_handler *s,
			   const struct exception *e)
{
  CAST(exc_finish_read_handler, self, s);
  switch(e->type)
    {
    case EXC_FINISH_READ:
      close_fd_nicely(self->fd);
      break;
    case EXC_FINISH_IO:
      break;
    case EXC_PAUSE_READ:
      self->fd->want_read = 0;
      break;
    case EXC_PAUSE_START_READ:
      if (self->fd->read)
	self->fd->want_read = 1;
      break;
    default:
      EXCEPTION_RAISE(self->super.parent, e);
    }
}

struct exception_handler *
make_exc_finish_read_handler(struct lsh_fd *fd,
			     struct exception_handler *parent,
			     const char *context)
{
  NEW(exc_finish_read_handler, self);

  self->super.parent = parent;
  self->super.raise = do_exc_finish_read_handler;
  self->super.context = context;
  
  self->fd = fd;

  return &self->super;
}

const struct exception finish_read_exception =
STATIC_EXCEPTION(EXC_FINISH_READ, "Stop reading");

const struct exception finish_io_exception =
STATIC_EXCEPTION(EXC_FINISH_IO, "Stop i/o");

struct exception *
make_io_exception(UINT32 type, struct lsh_fd *fd, int error, const char *msg)
{
  NEW(io_exception, self);
  assert(type & EXC_IO);
  
  self->super.type = type;

  if (msg)
    self->super.msg = msg;
  else
    self->super.msg = error ? strerror(error) : "Unknown i/o error";

  self->error = error;
  self->fd = fd;
  
  return &self->super;
}


/* Socket workround */
#ifndef SHUTDOWN_WORKS_WITH_UNIX_SOCKETS

/* There's an how++ missing in the af_unix shutdown implementation of
 * some linux versions. Try an ugly workaround. */
#ifdef linux

/* From src/linux/include/net/sock.h */
#define RCV_SHUTDOWN	1
#define SEND_SHUTDOWN	2

#undef SHUT_RD
#undef SHUT_WR
#undef SHUT_RD_WR

#define SHUT_RD RCV_SHUTDOWN
#define SHUT_WR SEND_SHUTDOWN
#define SHUT_RD_WR (RCV_SHUTDOWN | SEND_SHUTDOWN)

#else /* !linux */

/* Don't know how to work around the broken shutdown(). So disable it
 * completely. */

#define SHUTDOWN(fd, how) 0

#endif /* !linux */
#endif /* !SHUTDOWN_WORKS_WITH_UNIX_SOCKETS */

#ifndef SHUTDOWN
#define SHUTDOWN(fd, how) (shutdown((fd), (how)))
#endif

#ifndef SHUT_RD
#define SHUT_RD 0
#endif

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef SHUT_RD_WR
#define SHUT_RD_WR 2
#endif

/* Creates a one-way socket connection. Returns 1 on success, 0 on
 * failure. fds[0] is for reading, fds[1] for writing (like for the
 * pipe() system call). */
int
lsh_make_pipe(int *fds)
{
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
    {
      werror("socketpair() failed: %z\n", STRERROR(errno));
      return 0;
    }
  debug("Created socket pair. Using fd:s %i <-- %i\n", fds[0], fds[1]);

  if (SHUTDOWN(fds[0], SHUT_WR) < 0)
    {
      werror("shutdown(%i, SEND) failed: %z\n", fds[0], STRERROR(errno));
      goto fail;
    }
  if (SHUTDOWN(fds[1], SHUT_RD) < 0)
    {
      werror("shutdown(%i, REC) failed: %z\n", fds[0], STRERROR(errno));
    fail:
      {
	int saved_errno = errno;

	close(fds[0]);
	close(fds[1]);

	errno = saved_errno;
	return 0;
      }
    }
  
  return 1;
}

/* Copies data from one fd to another. Works no matter if the fd:s are
 * in blocking or non-blocking mode. Tries hard not to do premature
 * reads; we don't want to read data into the buffer, and then
 * discover that we can't write it out.
 *
 * The src fd may be the process stdin, and if we are too gready
 * reading it, we may consume data that belongs to the user's next
 * command. */
int
lsh_copy_file(int src, int dst)
{
#define BUF_SIZE 1024
  char buf[BUF_SIZE];
  struct pollfd src_poll;
  struct pollfd dst_poll;

  src_poll.fd = src;
  dst_poll.fd = dst;
  
  for (;;)
    {
      int res;
      UINT32 i, length;

      /* First wait until dst is writable; otherwise there's no point
       * in reading the input. */

      dst_poll.events = POLLOUT;

      do
	res = poll(&dst_poll, 1, -1);
      while ( (res < 0) && (errno == EINTR));

      debug("lsh_copy_file, initial poll on destination:\n"
	    "  res = %i, src = %i, dst = %i, events = %xi, revents = %xi.\n",
	    res, src, dst, dst_poll.events, dst_poll.revents);
      
      assert(res == 1);
      
      if (!(dst_poll.revents & POLLOUT))
	/* Most likely, dst is a pipe, and there are no readers. */
	return 1;

      /* Ok, can we read anything? */
      src_poll.events = MY_POLLIN;
      
      do
	res = poll(&src_poll, 1, -1);
      while ( (res < 0) && (errno == EINTR));
      
      if (res < 0)
	return 0;

      debug("lsh_copy_file, poll on src:\n"
	    "  res = %i, src = %i, dst = %i, events = %xi, revents = %xi.\n",
	    res, src, dst, src_poll.events, src_poll.revents);
      
      assert(res == 1);
      
      /* On linux, it seems that POLLHUP is set on read eof. */
      if (!(src_poll.revents & MY_POLLIN))
	/* EOF */
	return 1;

      assert(src_poll.revents & MY_POLLIN);

      /* Before actually reading anything, we need to check that
       * the dst fd is still alive. */

      dst_poll.events = POLLOUT;
      do
	res = poll(&dst_poll, 1, 0);
      while ( (res < 0) && (errno == EINTR));

      debug("lsh_copy_file, second poll on destination:\n"
	    "  res = %i, src = %i, dst = %i, events = %xi, revents = %xi.\n",
	    res, src, dst, dst_poll.events, dst_poll.revents);
      
      if (res && !(dst_poll.revents & POLLOUT))
	{
	  /* NOTE: Either somebody else filled up the buffer, or
	   * the fd is dead. How do we know which happened? We
	   * can't check POLLHUP, because it seems linux always
	   * sets it. As a kludge, and because this condition
	   * should be really rare, we check our ppid to see if
	   * the main process have died and left us to init. */

	  debug("lsh_copy_file: ppid = %i\n", getppid());
	  
#if !MACOS
	  if (getppid() == 1)
#endif
	    return 1;
	}

      do 
	res = read(src, buf, BUF_SIZE);
      while ( (res < 0) && (errno == EINTR));

      debug("lsh_copy_file: read on fd %i returned = %i\n", src, res);
      
      if (res < 0)
	return 0;
      else if (!res)
	/* EOF */
	return 1;

      length = res;

      for (i = 0; i<length; )
	{
	  dst_poll.events = POLLOUT;
	  do
	    res = poll(&dst_poll, 1, -1);
	  while ( (res < 0) && (errno == EINTR));

	  debug("lsh_copy_file, inner poll on destination:\n"
		"  res = %i, src = %i, dst = %i, events = %xi, revents = %xi.\n",
		res, src, dst, dst_poll.events, dst_poll.revents);
      
	  if (res < 0)
	    return 0;

	  assert(res == 1);
	  
	  if (!(dst_poll.revents & POLLOUT))
	    return 0;

	  do
	    res = write(dst, buf + i, length - i);
	  while ( (res < 0) && (errno == EINTR));

	  debug("lsh_copy_file: write on fd %i returned = %i\n", dst, res);
	  
	  if (res < 0)
	    return 0;

	  i += res;
	}
    }
#undef BUF_SIZE
}

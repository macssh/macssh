/* io.h
 *
 *
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

#ifndef LSH_IO_H_INCLUDED
#define LSH_IO_H_INCLUDED

#include "abstract_io.h"
#include "command.h"
#include "resource.h"
#include "write_buffer.h"

#include <time.h>
#include <netdb.h>
/* For sig_atomic_t */
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define GABA_DECLARE
#include "io.h.x"
#undef GABA_DECLARE


/* Declare the class object, so that io_commands.c can refer to it for
 * type-checking. */
extern struct lsh_class io_backend_class;

/* GABA:
   (class
     (name lsh_callback)
     (vars
       (f method void)))
*/

#define LSH_CALLBACK(c) ((c)->f((c)))

/* The fd io callback is a closure, in order to support different
 * reading styles (buffered and consuming). Also used for writing. */

/* GABA:
   (class
     (name io_callback)
     (vars
       (f method void "struct lsh_fd *fd")))
*/

#define IO_CALLBACK(c, fd) ((c)->f((c), (fd)))

/* GABA:
   (class
     (name lsh_fd)
     (super resource)
     (vars
       (next object lsh_fd)
       (fd . int)

       ;; (backend object io_backend)
       ;; (next_closed object lsh_fd)
       
       ; Used for raising i/o-exceptions.
       ; Also passed on to readers of the consuming type,
       ; which seems kind of bogus.
       (e object exception_handler)
       
       ; User's close callback
       (close_callback object lsh_callback)

       ; Called before poll
       (prepare method void)

       ; This flag is set by the backend if it detects that a
       ; connection is hanged up. 
       
       (hanged_up . int)
       
       (want_read . int)
       ; Called if poll indicates that data can be read. 
       (read object io_callback)

       (want_write . int)
       ; Called if poll indicates that data can be written.
       (write object io_callback)

       ; NOTE: We could put write_buffer inside the write callback,
       ; but it seems simpler to keep it here, as it is needed by the
       ; prepare and write_close methods.
       (write_buffer object write_buffer)
       
       ; Called to when fd is closed for writing.
       (write_close method void)))
*/

#define FD_PREPARE(fd) ((fd)->prepare(fd))
#define FD_READ(fd) IO_CALLBACK((fd)->read, (fd))
#define FD_WRITE(fd) IO_CALLBACK((fd)->write, (fd))
#define FD_WRITE_CLOSE(fd) ((fd)->write_close(fd))


/* Used for read handlers like read_line and read_packet that
 * processes a little data at a time, possibly replacing the handler
 * and leaving some data for the new one. */

/* GABA:
   (class
     (name io_buffered_read)
     (super io_callback)
     (vars
       (buffer_size . UINT32)
       (handler object read_handler)))
*/

struct io_callback *
make_buffered_read(UINT32 buffer_size,
		   struct read_handler *handler);

/* Used for read handlers like read_data, that know how much data they
 * can consume. */

/* GABA:
   (class
     (name io_consuming_read)
     (super io_callback)
     (vars
       (query method UINT32)
       ; Returns the maximum number of octets that
       ; can be consumed immediately.
       (consumer object abstract_write)))
*/

#define READ_QUERY(r) ((r)->query((r)))

void init_consuming_read(struct io_consuming_read *self,
			 struct abstract_write *consumer);

/* Passed to the listen callback, and to other functions and commands
 * dealing with addresses. */
/* GABA:
   (class
     (name address_info)
     (vars
       ; An ipnumber, in decimal dot notation, ipv6 format, or
       ; a dns name.
       (ip string)
       ; The port number here is always in host byte order
       (port . UINT32))) */

/* Used for listening and connecting to local sockets.
 * Both strings have to be NUL-terminated. */

/* GABA:
   (class
     (name local_info)
     (vars
       (directory string)
       (name string)))
*/

struct local_info *
make_local_info(struct lsh_string *directory,
		struct lsh_string *name);
     
/* Returned by listen. And also by connect, so this is an improper name.
 * Functions related to AF_UNIX sockets leave the peer field as NULL. */
/* GABA:
   (class
     (name listen_value)
     (vars
       (fd object lsh_fd)
       (peer object address_info)))
*/

struct listen_value *
make_listen_value(struct lsh_fd *fd,
		  struct address_info *peer);

/* I/O exceptions */
/* GABA:
   (class
     (name io_exception)
     (super exception)
     (vars
       ; NULL if no fd was involved
       (fd object lsh_fd)
       ; errno code, or zero if not available
       (error . int))))
*/

/* If msg is NULL, it is derived from errno */
struct exception *
make_io_exception(UINT32 type, struct lsh_fd *fd, int error, const char *msg);

/* Used in cases where the fd and errno are not available */
#define STATIC_IO_EXCEPTION(type, name) \
{ { STATIC_HEADER, (type), (name) }, NULL, 0}

extern const struct exception finish_read_exception;
extern const struct exception finish_io_exception;

struct io_backend *
make_io_backend(void);

int io_iter(struct io_backend *b);
void io_run(struct io_backend *b);

struct resource *
io_signal_handler(struct io_backend *b,
		  volatile sig_atomic_t *flag,
		  struct lsh_callback *action);

struct resource *
io_callout(struct io_backend *b,
	   UINT32 delay,
	   struct lsh_callback *action);

int blocking_read(int fd, struct read_handler *r);

int get_portno(const char *service, const char *protocol);

struct address_info *
make_address_info_c(const char *host,
		    const char *port,
		    int def);

struct address_info *
make_address_info(struct lsh_string *host, 
		  UINT32 port);

struct address_info *
sockaddr2info(size_t addr_len,
	      struct sockaddr *addr);

struct sockaddr *
address_info2sockaddr(socklen_t *length,
		      struct address_info *a,
		      const int *preference,
		      int lookup);

/* Returns an exception, if anything went wrong */
const struct exception *
write_raw(int fd, UINT32 length, const UINT8 *data);
const struct exception *
write_raw_with_poll(int fd, UINT32 length, const UINT8 *data);

const struct exception *
read_raw(int fd, UINT32 length, UINT8 *data);

void io_set_nonblocking(int fd);
void io_set_close_on_exec(int fd);
void io_init_fd(int fd);

struct lsh_fd *make_lsh_fd(struct io_backend *b,
			   int fd,
			   struct exception_handler *e);

struct exception_handler *
make_exc_finish_read_handler(struct lsh_fd *fd,
			     struct exception_handler *parent,
			     const char *context);

struct lsh_fd *
io_connect(struct io_backend *b,
	   struct sockaddr *remote,
	   socklen_t remote_length,
	   struct command_continuation *c,
	   struct exception_handler *e);

struct lsh_fd *
io_listen(struct io_backend *b,
	  struct sockaddr *local,
	  socklen_t length,
	  struct io_callback *callback,
	  struct exception_handler *e);

struct lsh_fd *
io_listen_local(struct io_backend *b,
		struct local_info *info,
		struct io_callback *callback,
		struct exception_handler *e);

struct lsh_fd *
io_connect_local(struct io_backend *b,
		 struct local_info *info,
		 struct command_continuation *c,
		 struct exception_handler *e);

struct io_callback *
make_listen_callback(struct io_backend *backend,
		     struct command_continuation *c,
		     struct exception_handler *e);

#if 0
struct io_callback *
make_listen_callback_no_peer(struct io_backend *backend,
			     struct command_continuation *c);
#endif

/* NONO */
struct lsh_fd *fwd_io_read_write(struct lsh_fd *fd,
			     struct io_callback *read,
			     UINT32 block_size,
			     struct lsh_callback *close_callback);
/* NONO */

struct lsh_fd *io_read_write(struct lsh_fd *fd,
			     struct io_callback *read,
			     UINT32 block_size,
			     struct lsh_callback *close_callback);

struct lsh_fd *io_read(struct lsh_fd *fd,
		       struct io_callback *read,
		       struct lsh_callback *close_callback);

struct lsh_fd *io_write(struct lsh_fd *fd,
			UINT32 block_size,
			struct lsh_callback *close_callback);

/* Marks a file for close, without touching the close_reason field. */
void kill_fd(struct lsh_fd *fd);

void close_fd(struct lsh_fd *fd);

/* Stop reading from the fd, and close it as soon as the buffer
 * is completely written. */
void close_fd_nicely(struct lsh_fd *fd);

/* Stop reading, but if the fd has a write callback, keep it open. */
void close_fd_read(struct lsh_fd *fd);

struct lsh_fd *io_write_file(struct io_backend *backend,
			    const char *fname, int flags,
			    int mode,
			    UINT32 block_size,
			    struct lsh_callback *c,
			    struct exception_handler *e);

struct lsh_fd *
io_read_file(struct io_backend *backend,
	     const char *fname, 
	     struct exception_handler *e);

int
lsh_make_pipe(int *fds);

int
lsh_copy_file(int src, int dst);

#endif /* LSH_IO_H_INCLUDED */

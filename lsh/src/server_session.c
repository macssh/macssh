/* server_session.c
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "server_session.h"

#include "channel_commands.h"
#include "format.h"
#include "read_data.h"
#include "reaper.h"
#include "server_pty.h"
#include "ssh.h"
#include "tcpforward.h"
#include "translate_signal.h"
#include "tty.h"
#include "werror.h"
#include "xalloc.h"

#include <errno.h>

/* For debug */

#include <string.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>

#include "server_session.c.x"


/* Session */
/* GABA:
   (class
     (name server_session)
     (super ssh_channel)
     (vars
       ; User information
       ;; (user object lsh_user)

       (initial_window . UINT32)

       ; Resource to kill when the channel is closed. 
       (process object lsh_process)

       ; pty
       (pty object pty_info)
       ; value of the TERM environment variable
       (term string)

       ; Child process's stdio 
       (in object lsh_fd)
       (out object lsh_fd)

       ; err may be NULL, if there's no separate stderr channel.
       ; This happens if we use a pty, and the bash workaround is used.
       (err object lsh_fd)))
*/

/* Receive channel data */
static void
do_receive(struct ssh_channel *c,
	   int type, struct lsh_string *data)
{
  CAST(server_session, closure, c);

  switch(type)
    {
    case CHANNEL_DATA:
      A_WRITE(&closure->in->write_buffer->super, data);
      break;
    case CHANNEL_STDERR_DATA:
      werror("Ignoring unexpected stderr data.\n");
      lsh_string_free(data);
      break;
    default:
      fatal("Internal error!\n");
    }
}

/* We may send more data */
static void
do_send_adjust(struct ssh_channel *s,
	       UINT32 i UNUSED)
{
  CAST(server_session, session, s);

  /* FIXME: Perhaps it's better to just check the read pointers, and
   * not bother with the alive-flags? */
  if (session->out->super.alive)
    {
      assert(session->out->read);

      session->out->want_read = 1;
    }
  
  if (session->err && session->err->super.alive)
    {
      assert(session->err->read);
  
      session->err->want_read = 1;
    }
}

/* FIXME: Perhaps unify with client_session.c:do_client_session_eof */
static void
do_eof(struct ssh_channel *channel)
{
  CAST(server_session, session, channel);

  trace("server_session.c: do_eof\n");

  write_buffer_close(session->in->write_buffer);
}

struct ssh_channel *
make_server_session(UINT32 initial_window,
		    struct alist *request_types)
{
  NEW(server_session, self);

  init_channel(&self->super);

  self->initial_window = initial_window;

  /* We don't want to receive any data before we have forked some
   * process to receive it. */
  self->super.rec_window_size = 0;

  /* FIXME: Make maximum packet size configurable. */
  self->super.rec_max_packet = SSH_MAX_PACKET - SSH_CHANNEL_MAX_PACKET_FUZZ;
  self->super.request_types = request_types;

  /* Note: We don't need a close handler; the channels resource list
   * is taken care of automatically. */
  
  self->process = NULL;
  
  self->in = NULL;
  self->out = NULL;
  self->err = NULL;
  
  return &self->super;
}


/* GABA:
   (class
     (name open_session)
     (super channel_open)
     (vars
       (session_requests object alist)))
*/

#define WINDOW_SIZE 10000

static void
do_open_session(struct channel_open *s,
		struct ssh_connection *connection UNUSED,
		struct channel_open_info *info UNUSED,
		struct simple_buffer *args,
		struct command_continuation *c,
		struct exception_handler *e)
{
  CAST(open_session, self, s);

  debug("server.c: do_open_session\n");

  assert(connection->user);
  
  if (parse_eod(args))
    {
      COMMAND_RETURN(c,
		     make_server_session(WINDOW_SIZE, self->session_requests));
    }
  else
    {
      PROTOCOL_ERROR(e, "trailing garbage in open message");
    }
}

struct channel_open *
make_open_session(struct alist *session_requests)
{
  NEW(open_session, closure);

  closure->super.handler = do_open_session;
  closure->session_requests = session_requests;
  
  return &closure->super;
}


struct lsh_string *
format_exit_signal(struct ssh_channel *channel,
		   int core, int signal)
{
  struct lsh_string *msg = ssh_format("Process killed by %lz.\n",
				      STRSIGNAL(signal));
  
  return format_channel_request(ATOM_EXIT_SIGNAL,
				channel,
				0,
				"%a%c%fS%z",
				signal_local_to_network(signal),
				core,
				msg, "");
}

struct lsh_string *
format_exit(struct ssh_channel *channel, int value)
{
  return format_channel_request(ATOM_EXIT_STATUS,
				channel,
				0,
				"%i", value);
}

/* GABA:
   (class
     (name exit_shell)
     (super exit_callback)
     (vars
       (session object server_session)))
*/

static void
do_exit_shell(struct exit_callback *c, int signaled,
	      int core, int value)
{
  CAST(exit_shell, closure, c);
  struct server_session *session = closure->session;
  struct ssh_channel *channel = &session->super;
  
  trace("server_session.c: do_exit_shell\n");
  
  /* NOTE: We don't close the child's stdio here. */
  
  if (!(channel->flags & CHANNEL_SENT_CLOSE))
    {
      verbose("server_session.c: Sending %a message on channel %i\n",
	      signaled ? ATOM_EXIT_SIGNAL : ATOM_EXIT_STATUS,
	      channel->channel_number);
      
      C_WRITE(channel->connection,
	      (signaled
	       ? format_exit_signal(channel, core, value)
	       : format_exit(channel, value)) );

      /* We want to close the channel as soon as all stdout and stderr
       * data has been sent. In particular, we don't wait for EOF from
       * the client, most clients never sends that. */
      
      channel->flags |= (CHANNEL_NO_WAIT_FOR_EOF | CHANNEL_CLOSE_AT_EOF);
      
      if (channel->flags & CHANNEL_SENT_EOF)
	{
	  /* We have sent EOF already, so initiate close */
	  channel_close(channel);
	}
    }
}

static struct exit_callback *
make_exit_shell(struct server_session *session)
{
  NEW(exit_shell, self);

  self->super.exit = do_exit_shell;
  self->session = session;

  return &self->super;
}

/* GABA:
   (class
     (name shell_request)
     (super channel_request)
     (vars
       (backend object io_backend)))
*/

static int
make_pipes(int *in, int *out, int *err)
{
  int saved_errno;
  
  if (lsh_make_pipe(in))
    {
      if (lsh_make_pipe(out))
	{
	  if (lsh_make_pipe(err))
	    {
              return 1;
            }
	  saved_errno = errno;
          close(out[0]);
          close(out[1]);
        }
      else
	saved_errno = errno;
      close(in[0]);
      close(in[1]);
    }
  else
    saved_errno = errno;
  
  errno = saved_errno;
  return 0;
}

#define BASH_WORKAROUND 1

#if WITH_PTY_SUPPORT
static int make_pty(struct pty_info *pty, int *in, int *out, int *err)
{
  int saved_errno = 0;

  debug("make_pty... ");
  if (pty)
    debug("exists: \n"
	  "  alive = %i\n"
	  "  master = %i\n"
	  "  slave = %i\n"
	  "... ",
	  pty->super.alive, pty->master, pty->slave);
  debug("\n");
  
  if (pty) 
    {
      assert(pty->super.alive);
      
      debug("make_pty: Using allocated pty.\n");
      in[0] = pty->slave;
      in[1] = pty->master;

      /* Ownership of the fd:s passes on to some file objects. */
      pty->super.alive = 0;

      /* FIXME: It seems unnecessary to dup the fd:s here. But perhaps
       * having equal in and out fds may confuse the cleanup code, so
       * we leave it for now. */
      if ((out[0] = dup(pty->master)) != -1)
        {
          if ((out[1] = dup(pty->slave)) != -1) 
            {
#if BASH_WORKAROUND
	      /* Don't use a separate stderr channel; just dup the
	       * stdout pty to stderr. */
	      if ((err[1] = dup(pty->slave)) != -1)
                {
                  err[0] = -1;
                  return 1;
                } 
#else /* !BASH_WORKAROUND */
	      if (lsh_make_pipe(err))
		{
		  /* Success! */
		  return 1;
		}
#endif /* !BASH_WORKAROUND */
              saved_errno = errno;
	      
            }
	  else
	    saved_errno = errno;
	  close(out[0]);
	}
      else 
	saved_errno = errno;
      close(in[0]);
      close(in[1]);

      werror("make_pty: duping pty filedescriptors failed (errno = %i): %z\n",
	     errno, STRERROR(errno));
    }
  errno = saved_errno;
  return 0;
}
#else /* !WITH_PTY_SUPPORT */
static int make_pty(struct pty_info *pty UNUSED,
		    int *in UNUSED, int *out UNUSED, int *err UNUSED)
{ return 0; }
#endif /* !WITH_PTY_SUPPORT */

/* Returns -1 on failure, 0 for child and +1 for parent */
static int
spawn_process(struct server_session *session,
	      struct lsh_user *user,
	      struct address_info *peer,
	      struct io_backend *backend)
{
  int in[2];
  int out[2];
  int err[2];

  int using_pty = 0;
  
  if (session->process)
    /* Already spawned a shell or command */
    return -1;
  
  /* {in|out|err}[0] is for reading,
   * {in|out|err}[1] for writing. */

  if (make_pty(session->pty, in, out, err))
    using_pty = 1;

  else if (!make_pipes(in, out, err))
    return -1;

  {
    struct lsh_process *child;
    
    if (USER_FORK(user, &child,
		  make_exit_shell(session),
		  peer, using_pty ? session->pty->tty_name : NULL))
      {
	if (child)
	  { /* Parent */
	    struct ssh_channel *channel = &session->super;
	    trace("spawn_process: Parent process\n");
	    
	    session->process = child;

	    /* Close the child's fd:s */
	    close(in[0]);
	    close(out[1]);
	    close(err[1]);

	    {
	      /* Exception handlers */
	      struct exception_handler *io_exception_handler
		= make_channel_io_exception_handler(channel,
						    "lshd: Child stdio: ",
						    &default_exception_handler,
						    HANDLER_CONTEXT);

	      /* Close callback for stderr and stdout */
	      struct lsh_callback *read_close_callback
		= make_channel_read_close_callback(channel);

	      session->in
		= io_write(make_lsh_fd(backend, in[1], "child stdin",
				       io_exception_handler),
			   SSH_MAX_PACKET, NULL);
	  
	      /* Flow control */
	      session->in->write_buffer->report = &session->super.super;

	      /* FIXME: Should we really use the same exception handler,
	       * which will close the channel on read errors, or is it
	       * better to just send EOF on read errors? */
	      session->out
		= io_read(make_lsh_fd(backend, out[0], "child stdout",
				      io_exception_handler),
			  make_channel_read_data(channel),
			  read_close_callback);
	      session->err 
		= ( (err[0] != -1)
		    ? io_read(make_lsh_fd(backend, err[0], "child stderr",
					  io_exception_handler),
			      make_channel_read_stderr(channel),
			      read_close_callback)
		    : NULL);
	    }
	
	    channel->receive = do_receive;
	    channel->send_adjust = do_send_adjust;
	    channel->eof = do_eof;
	  
	    /* Make sure that the process and it's stdio is
	     * cleaned up if the channel or connection dies. */
	    REMEMBER_RESOURCE
	      (channel->resources, &child->super);

	    /* FIXME: How to do this properly if in and out may use the
	     * same fd? */
	    REMEMBER_RESOURCE
	      (channel->resources, &session->in->super);
	    REMEMBER_RESOURCE
	      (channel->resources, &session->out->super);
	    if (session->err)
	      REMEMBER_RESOURCE
		(channel->resources, &session->err->super);

	    /* Don't close channel immediately at EOF, as we want to
	     * get a chance to send exit-status or exit-signal. */
	    session->super.flags &= ~CHANNEL_CLOSE_AT_EOF;
	    return 1;
	  }
	else
	  { /* Child */
	    trace("spawn_process: Child process\n");
	    assert(getuid() == user->uid);

#if 0
	    /* Debug timing problems */
	    if (sleep(5))
	      {
		trace("server_session.c: sleep interrupted\n");

		sleep(5);
	      }
#endif    
	    if (!USER_CHDIR_HOME(user))
	      {
		werror("Could not change to home (or root) directory!\n");
		_exit(EXIT_FAILURE);
	      }
	    
#if WITH_PTY_SUPPORT
	    if (using_pty)
	      {
		debug("lshd: server.c: Setting controlling tty...\n");
		if (!tty_setctty(session->pty))
		  {
		    debug("lshd: server.c: "
			  "Setting controlling tty... Failed!\n");
		    werror("lshd: Can't set controlling tty for child!\n");
		    _exit(EXIT_FAILURE);
		  }
		else
		  debug("lshd: server.c: Setting controlling tty... Ok.\n");
	      }
#endif /* WITH_PTY_SUPPORT */
	  
	    /* Close all descriptors but those used for
	     * communicationg with parent. We rely on the
	     * close-on-exec flag for all fd:s handled by the
	     * backend. */
	    
	    if (dup2(in[0], STDIN_FILENO) < 0)
	      {
		werror("Can't dup stdin!\n");
		_exit(EXIT_FAILURE);
	      }
	    close(in[0]);
	    close(in[1]);
	    
	    if (dup2(out[1], STDOUT_FILENO) < 0)
	      {
		werror("Can't dup stdout!\n");
		_exit(EXIT_FAILURE);
	      }
	    close(out[0]);
	    close(out[1]);

	    if (!dup_error_stream())
	      {
		werror("server_session: Failed to dup old stderr. Bye.\n");
		set_error_ignore();
	      }

	    if (dup2(err[1], STDERR_FILENO) < 0)
	      {
		werror("Can't dup stderr!\n");
		_exit(EXIT_FAILURE);
	      }
	    close(err[0]);
	    close(err[1]);

	    return 0;
	  }
      }
    /* fork failed */
    /* Close and return channel_failure */

    close(err[0]);
    close(err[1]);
    close(out[0]);
    close(out[1]);
    close(in[0]);
    close(in[1]);
  }
  return -1;
}

static void
do_spawn_shell(struct channel_request *s,
	       struct ssh_channel *channel,
	       struct channel_request_info *info UNUSED,
	       struct simple_buffer *args,
	       struct command_continuation *c,
	       struct exception_handler *e)
{
  CAST(shell_request, closure, s);
  CAST(server_session, session, channel);

static struct exception shell_request_failed =
STATIC_EXCEPTION(EXC_CHANNEL_REQUEST, "Shell request failed");

  if (!parse_eod(args))
    {
      PROTOCOL_ERROR(e, "Invalid shell CHANNEL_REQUEST message.");
      return;
    }
    
  if (session->process)
    /* Already spawned a shell or command */
    goto fail;

  switch (spawn_process(session, channel->connection->user,
			channel->connection->peer,
			closure->backend))
    {
    case 1: /* Parent */
      /* NOTE: The return value is not used. */
      COMMAND_RETURN(c, channel);
      channel_start_receive(channel, session->initial_window);
      return;
    case 0:
      { /* Child */
#define MAX_ENV 1
	/* No args, and the USER_EXEC method fills in argv[0]. */
	const char *argv[] = { NULL, NULL };
	
	struct env_value env[MAX_ENV];
	int env_length = 0;
	
	debug("do_spawn_shell: Child process\n");
	assert(getuid() == channel->connection->user->uid);
	    	    
	if (session->term)
	  {
	    env[env_length].name ="TERM";
	    env[env_length].value = session->term;
	    env_length++;
	  }
	assert(env_length <= MAX_ENV);
#undef MAX_ENV

#if 1
	USER_EXEC(channel->connection->user, 1, argv, env_length, env);
	
	/* exec failed! */
	verbose("server_session: exec failed (errno = %i): %z\n",
		errno, STRERROR(errno));
	_exit(EXIT_FAILURE);

#else
# define GREETING "Hello world!\n"
	if (write(STDOUT_FILENO, GREETING, strlen(GREETING)) < 0)
	  _exit(errno);
	kill(getuid(), SIGSTOP);
	if (write(STDOUT_FILENO, shell, strlen(shell)) < 0)
	  _exit(125);
	_exit(126);
# undef GREETING
#endif
      }
    case -1:
      /* fork failed */

      break;
    default:
      fatal("Internal error!");
  }
 fail:
  EXCEPTION_RAISE(e, &shell_request_failed);
}

struct channel_request *
make_shell_handler(struct io_backend *backend)
{
  NEW(shell_request, closure);

  closure->super.handler = do_spawn_shell;
  closure->backend = backend;
  
  return &closure->super;
}


static void
do_spawn_exec(struct channel_request *s,
	      struct ssh_channel *channel,
	      struct channel_request_info *info UNUSED,
	      struct simple_buffer *args,
	      struct command_continuation *c,
	      struct exception_handler *e)
{
  CAST(shell_request, closure, s);
  CAST(server_session, session, channel);

static struct exception exec_request_failed =
STATIC_EXCEPTION(EXC_CHANNEL_REQUEST, "Exec request failed");

  UINT32 command_len;
  const UINT8 *command;

  if (!(parse_string(args, &command_len, &command)
	&& parse_eod(args)))
    {
      PROTOCOL_ERROR(e, "Invalid exec CHANNEL_REQUEST message.");
      return;
    }
    
  if (/* Already spawned a shell or command */
      session->process
      /* Command can't contain NUL characters. */
      || memchr(command, '\0', command_len))
    
    EXCEPTION_RAISE(e, &exec_request_failed);
  else
    {
      struct lsh_string *command_line = ssh_format("%ls", command_len, command);
      
      switch (spawn_process(session, channel->connection->user,
			    channel->connection->peer,
			    closure->backend))
	{
	case 1: /* Parent */
	  lsh_string_free(command_line);
	  
	  /* NOTE: The return value is not used. */
	  COMMAND_RETURN(c, channel);
	  channel_start_receive(channel, session->initial_window);
	  return;
	case 0:
	  { /* Child */
#define MAX_ENV 1
	    struct env_value env[MAX_ENV];
	    int env_length = 0;
	    
	    /* No args, and the USER_EXEC method fills in argv[0]. */

	    /* NOTE: I'd like to use an array initializer, but that's
	     * not ANSI-C, and at least HPUX' compiler can't handle
	     * it. */
	    
	    const char *argv[4];
	argv[0] = NULL;
	argv[1] = "-c";
	    argv[2] = lsh_get_cstring(command_line);
	argv[3] = NULL;
	
	    debug("do_spawn_shell: Child process\n");

	    assert(getuid() == channel->connection->user->uid);
	    assert(argv[2]);
	    	    
	if (session->term)
	  {
	    env[env_length].name ="TERM";
	    env[env_length].value = session->term;
	    env_length++;
	  }
	assert(env_length <= MAX_ENV);
#undef MAX_ENV

	    USER_EXEC(channel->connection->user, 0, argv, env_length, env);
	
	/* exec failed! */
	verbose("server_session: exec failed (errno = %i): %z\n",
		errno, STRERROR(errno));
	_exit(EXIT_FAILURE);
      }
    case -1:
      /* fork failed */
      lsh_string_free(command_line);
	  EXCEPTION_RAISE(e, &exec_request_failed);

	  break;
	default:
	  fatal("Internal error!");
	}
    }
}

struct channel_request *
make_exec_handler(struct io_backend *backend)
{
  NEW(shell_request, closure);

  closure->super.handler = do_spawn_exec;
  closure->backend = backend;
  
  return &closure->super;
}

/* For simplicity, represent a subsystem simply as a name of the
 * executable. */

/* GABA:
   (class
     (name subsystem_request)
     (super shell_request)
     (vars
       ;(subsystems object alist)
       ; A list { name, program, name, program, NULL }
       (subsystems . "const char **")))
*/

/* ;; GABA:
   (class
     (name sybsystem_info)
     (vars
       (name "const char *")))
*/

static const char *
lookup_subsystem(struct subsystem_request *self,
		 UINT32 length, const UINT8 *name)
{
  unsigned i;
  if (memchr(name, 0, length))
    return NULL;

  for (i = 0; self->subsystems[i]; i+=2)
    {
      assert(self->subsystems[i+1]);
      if ((length == strlen(self->subsystems[i]))
	  && !memcmp(name, self->subsystems[i], length))
	return self->subsystems[i + 1];
    }
  return NULL;
}

static void
do_spawn_subsystem(struct channel_request *s,
		   struct ssh_channel *channel,
		   struct channel_request_info *info UNUSED,
		   struct simple_buffer *args,
		   struct command_continuation *c,
		   struct exception_handler *e)
{
  CAST(subsystem_request, self, s);
  CAST(server_session, session, channel);

  static struct exception subsystem_request_failed =
    STATIC_EXCEPTION(EXC_CHANNEL_REQUEST, "Subsystem request failed");

  const UINT8 *name;
  UINT32 name_length;

  const char *program;
      
  if (! (parse_string(args, &name_length, &name) && parse_eod(args)))
    {
      PROTOCOL_ERROR(e, "Invalid subsystem CHANNEL_REQUEST message.");
      return;
    }
  
  program = lookup_subsystem(self, name_length, name);
  
  if (!session->process && program)
    {
      /* Don't use any pty */
      if (session->pty)
	{
	  KILL_RESOURCE(&session->pty->super);
	  session->pty = NULL;
	}
      
      switch (spawn_process(session, channel->connection->user,
			    channel->connection->peer,
			    self->super.backend))
	{
	case 1: /* Parent */
	  /* NOTE: The return value is not used. */
	  COMMAND_RETURN(c, channel);
	  channel_start_receive(channel, session->initial_window);
	  return;

	case 0: /* Child */
	  {
	    /* No args, and the USER_EXEC method fills in argv[0]. */
	    const char *argv[] = { NULL, NULL };

	    debug("do_spawn_subsystem: Child process\n");
	  
	    USER_EXEC(channel->connection->user, 1, argv, 0, NULL);

	    werror("server_session: subsystem exec failed (errno = %i): %z\n",
		   errno, STRERROR(errno));
	    _exit(EXIT_FAILURE);
	  }
	case -1: /* Error */
	  break;

	default:
	  fatal("Internal error!");
	}
    }
  EXCEPTION_RAISE(e, &subsystem_request_failed);
}

struct channel_request *
make_subsystem_handler(struct io_backend *backend,
		       const char **subsystems)
{
  NEW(subsystem_request, self);

  self->super.super.handler = do_spawn_subsystem;
  self->super.backend = backend;
  self->subsystems = subsystems;
  
  return &self->super.super;
}


#if WITH_PTY_SUPPORT

/* pty_handler */
static void
do_alloc_pty(struct channel_request *c UNUSED,
	     struct ssh_channel *channel,
	     struct channel_request_info *info UNUSED,
	     struct simple_buffer *args,
	     struct command_continuation *s,
	     struct exception_handler *e)
{
  struct terminal_dimensions dims;
  const UINT8 *mode;
  UINT32 mode_length;
  struct lsh_string *term = NULL;

  static struct exception pty_request_failed =
    STATIC_EXCEPTION(EXC_CHANNEL_REQUEST, "pty request failed");

  CAST(server_session, session, channel);

  verbose("Client requesting a tty...\n");
  
  if ((term = parse_string_copy(args))
      && parse_uint32(args, &dims.char_width)
      && parse_uint32(args, &dims.char_height)
      && parse_uint32(args, &dims.pixel_width)
      && parse_uint32(args, &dims.pixel_height)
      && parse_string(args, &mode_length, &mode)
      && parse_eod(args))
    {
  /* The client may only request a tty once. */
      if (session->pty)
	{
	  lsh_string_free(term);
	  EXCEPTION_RAISE(e, &pty_request_failed);
	}
      else
    {
      struct pty_info *pty = make_pty_info();

	  if (pty_allocate(pty, channel->connection->user->uid))
        {
          struct termios ios;

          if (tty_getattr(pty->slave, &ios))
            {
	      assert(pty->super.alive);
              session->pty = pty;

	      /* Don't set TERM if the value is empty. */
	      if (!term->length)
		{
		  lsh_string_free(term);
		  term = NULL;
		}
	      
              session->term = term;
              tty_decode_term_mode(&ios, mode_length, mode); 
	      
              if (tty_setattr(pty->slave, &ios) &&
		      tty_setwinsize(pty->slave, &dims))
		{
		  REMEMBER_RESOURCE(channel->resources, &pty->super);

		  verbose(" granted.\n");
		  COMMAND_RETURN(s, NULL);

		  return;
		}
		}
	    }
	  
		/* Close fd:s and mark the pty-struct as dead */
		KILL_RESOURCE(&pty->super);
      KILL(pty);
	}
  verbose("Pty allocation failed.\n");
  lsh_string_free(term);
      EXCEPTION_RAISE(e, &pty_request_failed);
    }
  else
    {
      werror("Invalid pty request.\n");
      lsh_string_free(term);

      PROTOCOL_ERROR(e, "Invalid pty request.");
    }
}

struct channel_request
pty_request_handler =
{ STATIC_HEADER, do_alloc_pty };

static void
do_window_change_request(struct channel_request *c UNUSED,
			 struct ssh_channel *channel,
			 struct channel_request_info *info UNUSED,
			 struct simple_buffer *args,
			 struct command_continuation *s,
			 struct exception_handler *e)
{
  struct terminal_dimensions dims;
  CAST(server_session, session, channel);

  verbose("Receiving window-change request...\n");

  if (parse_uint32(args, &dims.char_width)
      && parse_uint32(args, &dims.char_height)
      && parse_uint32(args, &dims.pixel_width)
      && parse_uint32(args, &dims.pixel_height)
      && parse_eod(args))
    {
      static const struct exception winch_request_failed =
	STATIC_EXCEPTION(EXC_CHANNEL_REQUEST, "window-change request failed: No pty");

      if (!(session->pty && session->pty->super.alive))
	EXCEPTION_RAISE(e, &winch_request_failed);
      
      else if (!tty_setwinsize(session->pty->slave, &dims))
	EXCEPTION_RAISE(e, &winch_request_failed);

      else
	{
	  if (!SIGNAL_PROCESS(session->process, SIGWINCH))
	    werror("Sending SIGWINCH signal failed.\n");

	  COMMAND_RETURN(s, NULL);
	}
    }
  else
    PROTOCOL_ERROR(e, "Invalid window-change request.");
}

struct channel_request
window_change_request_handler =
{ STATIC_HEADER, do_window_change_request };

#endif /* WITH_PTY_SUPPORT */

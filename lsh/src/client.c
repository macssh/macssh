/* client.c
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, 2000 Niels Möller
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

#include "client.h"

#include "abstract_io.h"
#include "channel.h"
#include "channel_commands.h"
#include "connection.h"
#include "format.h"
#include "interact.h"
#include "io.h"
#include "pad.h"
#include "parse.h"
#include "ssh.h"
#include "tcpforward_commands.h"
#include "translate_signal.h"
#include "werror.h"
#include "xalloc.h"

#include <string.h>
#include <assert.h>

#include <signal.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lsh_argp.h"

#define GABA_DEFINE
#include "client.h.x"
#undef GABA_DEFINE

#include "client.c.x"

static struct lsh_string *
format_service_request(int name)
{
  return ssh_format("%c%a", SSH_MSG_SERVICE_REQUEST, name);
}

/* Start a service that the server has accepted (for instance
 * ssh-userauth). */
/* GABA:
   (class
     (name accept_service_handler)
     (super packet_handler)
     (vars
       (service . int)
       (c object command_continuation)

       ;; FIXME: Do we really need the exception handler here?
       (e object exception_handler)))
*/

static void
do_accept_service(struct packet_handler *c,
		  struct ssh_connection *connection,
		  struct lsh_string *packet)
{
  CAST(accept_service_handler, closure, c);

  struct simple_buffer buffer;
  unsigned msg_number;
  int name;

  simple_buffer_init(&buffer, packet->length, packet->data);
  
  if (parse_uint8(&buffer, &msg_number)
      && (msg_number == SSH_MSG_SERVICE_ACCEPT)
      && (
#if DATAFELLOWS_WORKAROUNDS
	  (connection->peer_flags & PEER_SERVICE_ACCEPT_KLUDGE)
#else
	  0
#endif
	  || (parse_atom(&buffer, &name)
	      && (name == closure->service)))
      && parse_eod(&buffer))
    {
      lsh_string_free(packet);
      connection->dispatch[SSH_MSG_SERVICE_ACCEPT] = &connection_fail_handler;
      
      COMMAND_RETURN(closure->c, connection);
    }
  else
    {
      lsh_string_free(packet);
      PROTOCOL_ERROR(closure->e, "Invalid SSH_MSG_SERVICE_ACCEPT message");
    }
}

struct packet_handler *
make_accept_service_handler(UINT32 service,
			    struct command_continuation *c,
			    struct exception_handler *e)
{
  NEW(accept_service_handler, closure);

  closure->super.handler = do_accept_service;
  closure->service = service;
  closure->c = c;
  closure->e = e;
  
  return &closure->super;
}

void
do_request_service(struct command *s,
		   struct lsh_object *x,
		   struct command_continuation *c,
		   struct exception_handler *e)
{
  CAST(request_service, self, s);
  CAST(ssh_connection, connection, x);
  
  connection->dispatch[SSH_MSG_SERVICE_ACCEPT]
    = make_accept_service_handler(self->service, c, e);
  
  C_WRITE(connection,
	  format_service_request(self->service));
}

struct command *
make_request_service(int service)
{
  NEW(request_service, closure);

  closure->super.call = do_request_service;
  closure->service = service;

  return &closure->super;
}


/* GABA:
   (class
     (name exit_handler)
     (super channel_request)
     (vars
       (exit_status . "int *")))
*/

static void
do_exit_status(struct channel_request *c,
	       struct ssh_channel *channel,
	       struct ssh_connection *connection UNUSED,
	       struct channel_request_info *info,
	       struct simple_buffer *args,
	       struct command_continuation *s,
	       struct exception_handler *e)
{
  CAST(exit_handler, closure, c);
  UINT32 status;

  if (!info->want_reply
      && parse_uint32(args, &status)
      && parse_eod(args))
    {
      verbose("client.c: Receiving exit-status %i on channel %i\n",
	      status, channel->channel_number);
      
      *closure->exit_status = status;

      ALIST_SET(channel->request_types, ATOM_EXIT_STATUS, NULL);
      ALIST_SET(channel->request_types, ATOM_EXIT_SIGNAL, NULL);

#if 0
      /* Send EOF, if we haven't done that already. */
      /* FIXME: Make this behaviour configurable, there may be some
       * child process alive that we could talk to. */

      channel_eof(channel);
#endif
      
      COMMAND_RETURN(s, NULL);
    }
  else
    /* Invalid request */
    PROTOCOL_ERROR(e, "Invalid exit-status message");
}

static void
do_exit_signal(struct channel_request *c,
	       struct ssh_channel *channel,
	       struct ssh_connection *connection UNUSED,
	       struct channel_request_info *info,
	       struct simple_buffer *args,
	       struct command_continuation *s,
	       struct exception_handler *e)
{
  CAST(exit_handler, closure, c);

  UINT32 signal;
  int core;

  const UINT8 *msg;
  UINT32 length;

  const UINT8 *language;
  UINT32 language_length;
  
  if (!info->want_reply
      && parse_uint32(args, &signal)
      && parse_boolean(args, &core)
      && parse_string(args, &length, &msg)
      && parse_string(args, &language_length, &language)
      && parse_eod(args))
    {
      /* FIXME: What exit status should be returned when the remote
       * process dies violently? */

      *closure->exit_status = 7;

#if 0
      signal = signal_network_to_local(signal);

      werror("%ups", length, msg);
#endif
      werror("Remote process was killed by signal: %ups %z\n",
	     length, msg,
	     core ? "(core dumped remotely)\n": "");

      ALIST_SET(channel->request_types, ATOM_EXIT_STATUS, NULL);
      ALIST_SET(channel->request_types, ATOM_EXIT_SIGNAL, NULL);

#if 0
      /* Send EOF, if we haven't done that already. */
      /* FIXME: Make this behaviour configurable, there may be some
       * child process alive that we could talk to. */

      channel_eof(channel);
#endif
      COMMAND_RETURN(s, NULL);
    }
  else
    /* Invalid request */
    PROTOCOL_ERROR(e, "Invalid exit-signal message");
}

struct channel_request *
make_handle_exit_status(int *exit_status)
{
  NEW(exit_handler, self);

  self->super.handler = do_exit_status;

  self->exit_status = exit_status;

  return &self->super;
}

struct channel_request *
make_handle_exit_signal(int *exit_status)
{
  NEW(exit_handler, self);

  self->super.handler = do_exit_signal;

  self->exit_status = exit_status;

  return &self->super;
}

/* GABA:
   (class
     (name session_open_command)
     (super channel_open_command)
     (vars
       ; This command can only be executed once,
       ; so we can allocate the session object in advance.
       (session object ssh_channel)))
*/

static struct ssh_channel *
new_session(struct channel_open_command *s,
	    struct ssh_connection *connection,
	    UINT32 local_channel_number,
	    struct lsh_string **request)
{
  CAST(session_open_command, self, s);
  struct ssh_channel *res;

  self->session->write = connection->write;
  
  *request = format_channel_open(ATOM_SESSION,
				 local_channel_number,
				 self->session, "");
  
  res = self->session;

  /* Make sure this command can not be invoked again */
  self->session = NULL;

  return res;
}

struct command *
make_open_session_command(struct ssh_channel *session)
{
  NEW(session_open_command, self);
  self->super.super.call = do_channel_open_command;
  self->super.new_channel = new_session;
  self->session = session;

  return &self->super.super;
}


static struct lsh_string *
do_format_shell_request(struct channel_request_command *s UNUSED,
			struct ssh_channel *channel,
			struct command_continuation **c)
{
  return format_channel_request(ATOM_SHELL, channel, !!*c, "");
}

struct channel_request_command request_shell =
{ { STATIC_HEADER, do_channel_request_command }, do_format_shell_request };


/* GABA:
   (class
     (name exec_request)
     (super channel_request_command)
     (vars
       (command string)))
*/

static struct lsh_string *
do_format_exec_request(struct channel_request_command *s,
		       struct ssh_channel *channel,
		       struct command_continuation **c)
{
  CAST(exec_request, self, s);

  verbose("lsh: Requesting remote exec.\n");

  return format_channel_request(ATOM_EXEC, channel,
				!!*c, "%S", self->command);
}

struct command *
make_exec_request(struct lsh_string *command)
{
  NEW(exec_request, req);

  req->super.format_request = do_format_exec_request;
  req->super.super.call = do_channel_request_command;
  req->command = command;

  return &req->super.super;
}


/* Handling of options and operations shared by the plain lsh client
   and lshg. */

/* Forward declaration */

static struct ssh_channel *
make_client_session(struct client_options *options);

/* Block size for stdout and stderr buffers */
#define BLOCK_SIZE 32768

/* Window size for the session channel
 *
 * NOTE: Large windows seem to trig a bug in sshd2. */
#define WINDOW_SIZE 10000

#define ARG_NOT 0x400

#define OPT_STDIN 0x210
#define OPT_STDOUT 0x211
#define OPT_STDERR 0x212

#define OPT_FORK_STDIO 0x213

void
init_client_options(struct client_options *self,
		    struct io_backend *backend,
		    struct exception_handler *handler,
		    int *exit_code)			 
{
  self->backend = backend;
  self->tty = make_unix_interact(backend);
  self->handler = handler;

  self->exit_code = exit_code;
  
  self->not = 0;
  self->port = "ssh";
  self->remote = NULL;

  self->local_user = self->user = getenv("LOGNAME");

  self->with_remote_peers = 0; 
  self->with_pty = -1;

  self->stdin_file = NULL;
  self->stdout_file = NULL;
  self->stderr_file = NULL;

  self->stdin_fork = 0;
  self->stdout_fork = 0;
#ifndef MACOS
  self->stderr_fork = 1;
#else
  self->stderr_fork = 0;
#endif
  self->used_stdin = 0;
  self->used_pty = 0;

  self->start_shell = 1;
  self->remote_forward = 0;

  object_queue_init(&self->actions);  
}

/* Host to connect to */
DEFINE_COMMAND_SIMPLE(client_options2remote, a)
{
  CAST_SUBTYPE(client_options, options, a);
  trace("client.c: client_options2remote\n");
  
  return &options->remote->super;
}

/* Host to connect to */
DEFINE_COMMAND_SIMPLE(client_options2actions, a)
{
  CAST_SUBTYPE(client_options, options, a);

  trace("client.c: client_options2actions, %i actions\n",
	options->actions.length);
  
  return &queue_to_list(&options->actions)->super.super;
}

static const struct argp_option
client_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "port", 'p', "Port", 0, "Connect to this port.", 0 },
  { "user", 'l', "User name", 0, "Login as this user.", 0 },

  { NULL, 0, NULL, 0, "Actions:", CLIENT_ARGP_ACTION_GROUP },
  { "forward-local-port", 'L', "local-port:target-host:target-port", 0, "", 0 },
#if 0
  { "forward-remote-port", 'R', "remote-port:target-host:target-port", 0, "", 0 },
#endif
  { "nop", 'N', NULL, 0, "No operation (suppresses the default action, "
    "which is to spawn a remote shell)", 0 },
  { "execute", 'E', "command", 0, "Execute a command on the remote machine", 0 },
  { "shell", 'S', "command", 0, "Spawn a remote shell", 0 },
  /* { "gateway", 'G', NULL, 0, "Setup a local gateway", 0 }, */
  { NULL, 0, NULL, 0, "Universal not:", 0 },
  { "no", 'n', NULL, 0, "Inverts the effect of the next modifier", 0 },

  { NULL, 0, NULL, 0, "Modifiers that apply to port forwarding:",
    CLIENT_ARGP_MODIFIER_GROUP - 10 },
  { "remote-peers", 'g', NULL, 0, "Allow remote access to forwarded ports", 0 },
  { "no-remote-peers", 'g' | ARG_NOT, NULL, 0, 
    "Disallow remote access to forwarded ports (default).", 0 },

  { NULL, 0, NULL, 0, "Modifiers that apply to remote execution:", 0 },
  { "stdin", OPT_STDIN, "Filename", 0, "Redirect stdin", 0},
  { "no-stdin", OPT_STDIN | ARG_NOT, NULL, 0, "Redirect stdin from /dev/null", 0}, 
  { "stdout", OPT_STDOUT, "Filename", 0, "Redirect stdout", 0},
  { "no-stdout", OPT_STDOUT | ARG_NOT, NULL, 0, "Redirect stdout to /dev/null", 0}, 
  { "stderr", OPT_STDERR, "Filename", 0, "Redirect stderr", 0},
  { "no-stderr", OPT_STDERR | ARG_NOT, NULL, 0, "Redirect stderr to /dev/null", 0}, 
#if !MACOS
  { "cvs-workaround", OPT_FORK_STDIO, "i?o?e?", OPTION_ARG_OPTIONAL,
    "Avoid setting one or more of the stdio file descriptors into "
    "non-blocking mode. If no argument is provided, the workaround is "
    "applied to all three file descriptors. By default, the workaround "
    "is applied to stderr only.", 0 },
  { "no-cvs-workaround", OPT_FORK_STDIO | ARG_NOT, NULL, 0,
    "Disable the cvs workaround.", 0 },
#endif
#if WITH_PTY_SUPPORT
  { "pty", 't', NULL, 0, "Request a remote pty (default).", 0 },
  { "no-pty", 't' | ARG_NOT, NULL, 0, "Don't request a remote pty.", 0 },
#endif /* WITH_PTY_SUPPORT */
  { NULL, 0, NULL, 0, NULL, 0 }
};


/* GABA:
   (expr
     (name make_start_session)
     (params
       (open_session object command)
       (requests object object_list))
     (expr (lambda (connection)
       ((progn requests)
         ; Create a "session" channel
         (open_session connection)))))
*/

/* Requests a shell or command, and connects the channel to our stdio. */
/* GABA:
   (expr
     (name client_start_session)
     (params
       (request object command))
     (expr
       (lambda (session)
          (client_start_io (request session)))))
*/

static struct command *
make_client_start_session(struct command *request)
{
  CAST_SUBTYPE(command, r, client_start_session(request));
  return r;
}

/* Create an interactive session */
static struct command *
client_shell_session(struct client_options *options)
{
  struct command *get_pty = NULL;
  struct command *get_shell;
  
  struct object_list *session_requests;
  struct ssh_channel *session = make_client_session(options);

  if (!session)
    return NULL;
  
#if WITH_PTY_SUPPORT
  if (options->with_pty && !options->used_pty)
    {
      options->used_pty = 1;
      
      if (options->tty && INTERACT_IS_TTY(options->tty))
	{
	  get_pty = make_pty_request(options->tty);
	  if (!get_pty)
	    {
	      werror("lsh: Can't use tty (probably getattr or atexit failed).\n");
	    }
	}
      else
	{
	  werror("lsh: No tty available.\n");
	}
    }

  get_shell = make_client_start_session(&request_shell.super);
  
  /* FIXME: We need a non-varargs constructor for lists. */
  if (get_pty)
    session_requests
      = make_object_list(2,
			 /* Ignore EXC_CHANNEL_REQUEST for the pty allocation call. */
			 make_catch_apply
			 (make_catch_handler_info(EXC_ALL, EXC_CHANNEL_REQUEST,
						  0, NULL),
			  get_pty),
			 get_shell, -1);
  else
#endif /* WITH_PTY_SUPPORT */
    session_requests = make_object_list(1, get_shell, -1);

  {
    CAST_SUBTYPE(command, r,
		 make_start_session
		 (make_open_session_command(session), session_requests));
    return r;
  }
}

/* Create a session executing a command line */
static struct command *
client_command_session(struct client_options *options,
		       struct lsh_string *command)
{
  struct ssh_channel *session = make_client_session(options);

  if (session)
    {
      CAST_SUBTYPE(command, r,
		   make_start_session
		   (make_open_session_command(session),
		    make_object_list
		    (1, make_client_start_session(make_exec_request(command)),
		     -1)));
      return r;
    }
  
  return NULL;
}

struct command *
client_add_action(struct client_options *options,
		  struct command *action)
{
  if (action)
    object_queue_add_tail(&options->actions, &action->super);

  return action;
}

/* NOTE: Some of the original quoting is lost here. */
static struct lsh_string *
rebuild_command_line(unsigned argc, char **argv)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  unsigned length;
  unsigned i;
  unsigned pos;
  struct lsh_string *r;
  unsigned *alengths = alloca(sizeof(unsigned) * argc);
  
  assert (argc);
  length = argc - 1; /* Number of separating spaces. */

  for (i = 0; i<argc; i++)
    {
      alengths[i] = strlen(argv[i]);
      length += alengths[i];
    }

  r = lsh_string_alloc(length);
  memcpy(r->data, argv[0], alengths[0]);
  pos = alengths[0];
  for (i = 1; i<argc; i++)
    {
      r->data[pos++] = ' ';
      memcpy(r->data + pos, argv[i], alengths[i]);
      pos += alengths[i];
    }

  assert(pos == r->length);

#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
  return r;
}

#if MACOS

static int
fork_input(int in)
{
  return -1;
}

static int
fork_output(int out)
{
  return -1;
}

#else

static int
fork_input(int in)
{
  /* pipe[0] for reading, pipe[1] for writing. */
  int pipe[2];

  if (!lsh_make_pipe(pipe))
    return -1;

  switch (fork())
    {
    case -1:
      /* Error */
      return -1;
    case 0:
      close(pipe[0]);
      if (lsh_copy_file(in, pipe[1]))
	_exit(EXIT_SUCCESS);
      else
	_exit(EXIT_FAILURE);
    default:
      /* Parent */
      close(pipe[1]);
      return pipe[0];
    }
}

static int
fork_output(int out)
{
  /* pipe[0] for reading, pipe[1] for writing. */
  int pipe[2];

  if (!lsh_make_pipe(pipe))
    return -1;

  switch (fork())
    {
    case -1:
      /* Error */
      return -1;
    case 0:
      close(pipe[1]);
      if (lsh_copy_file(pipe[0], out))
	_exit(EXIT_SUCCESS);
      else
	_exit(EXIT_FAILURE);
    default:
      /* Parent */
      close(pipe[0]);
      return pipe[1];
    }
}
#endif

/* Create a session object. stdout and stderr are shared (although
 * with independent lsh_fd objects). stdin can be used by only one
 * session (until something "session-control"/"job-control" is added).
 * */
static struct ssh_channel *
make_client_session(struct client_options *options)
{
  int in;
  int out;
  int err;

  debug("lsh.c: Setting up stdin\n");

  if (options->stdin_file)
    in = open(options->stdin_file, O_RDONLY);
  else
    {
      if (options->used_stdin)
#if MACOS
	in = open("dev:null", O_RDONLY);
#else
	in = open("/dev/null", O_RDONLY);
#endif
      else 
	{
	  in = (options->stdin_fork ? fork_input : dup)(STDIN_FILENO);
	  options->used_stdin = 1;
	}
    }
    
  if (in < 0)
    {
      werror("lsh: Can't dup/open stdin (errno = %i): %z!\n",
	     errno, strerror(errno));
      return NULL;
    }

  debug("lsh.c: Setting up stdout\n");

  if (options->stdout_file)
    out = open(options->stdout_file, O_WRONLY | O_CREAT, 0666);
  else if (options->stdout_fork)
    out = fork_output(STDOUT_FILENO);
  else
    out = dup(STDOUT_FILENO);

  if (out < 0)
    {
      werror("lsh: Can't dup/open stdout (errno = %i): %z!\n",
	     errno, strerror(errno));
      close(in);
      return NULL;
    }

  debug("lsh.c: Setting up stderr\n");
  
  if (options->stderr_file)
    err = open(options->stderr_file, O_WRONLY | O_CREAT, 0666);
  else if (options->stderr_fork)
    err = fork_output(STDERR_FILENO);
  else
    {
      err = dup(STDERR_FILENO);
#if MACOS
      /* always log to console */
      /*set_error_stream(STDERR_FILENO, 1);*/
      set_error_syslog("lsh");
#else
      set_error_stream(STDERR_FILENO, 1);
#endif
    }

  if (err < 0) 
    {
      werror("lsh: Can't dup/open stderr!\n");
      close(in);
      close(out);
      return NULL;
    }

  /* Clear options */
  options->stdin_file = options->stdout_file = options->stderr_file = NULL;
  
  return make_client_session_channel
    (io_read(make_lsh_fd(options->backend, in, options->handler),
	     NULL, NULL),
     io_write(make_lsh_fd(options->backend, out, options->handler),
	      BLOCK_SIZE, NULL),
     io_write(make_lsh_fd(options->backend, err, options->handler),
	      BLOCK_SIZE, NULL),
     WINDOW_SIZE,
     options->exit_code);
}

/* Parse the argument for -R and -L */
int
client_parse_forward_arg(char *arg,
			 UINT32 *listen_port,
			 struct address_info **target)
{
  char *first;
  char *second;
  char *end;
  long port;
  
  first = strchr(arg, ':');
  if (!first)
    return 0;

  second = strchr(first + 1, ':');
  if (!second || (second == first + 1))
    return 0;

  if (strchr(second + 1, ':'))
    return 0;

  port = strtol(arg, &end, 0);
  if ( (end == arg)  || (end != first)
       || (port < 0) || (port > 0xffff) )
    return 0;

  *listen_port = port;

  port = strtol(second + 1, &end, 0);
  if ( (end == second + 1) || (*end != '\0')
       || (port < 0) || (port > 0xffff) )
    return 0;

  *target = make_address_info(ssh_format("%ls", second - first - 1, first + 1), port);
  
  return 1;
}

#define CASE_ARG(opt, attr, none)		\
  case opt:					\
    if (options->not)				\
      {						\
        options->not = 0;			\
						\
      case opt | ARG_NOT:			\
        options->attr = none;			\
        break;					\
      }						\
      						\
    options->attr = arg;			\
    break

#define CASE_FLAG(opt, flag)			\
  case opt:					\
    if (options->not)				\
      {						\
        options->not = 0;			\
						\
      case opt | ARG_NOT:			\
        options->flag = 0;			\
        break;					\
      }						\
      						\
    options->flag = 1;				\
    break

static error_t
client_argp_parser(int key, char *arg, struct argp_state *state)
{
  CAST_SUBTYPE(client_options, options, state->input);

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_NO_ARGS:
      argp_usage(state);
      break;
    case ARGP_KEY_ARG:
      if (!state->arg_num)
	{
	  if (options->port)
	    options->remote = make_address_info_c(arg, options->port, 0);
	  else
	    options->remote = make_address_info_c(arg, "ssh", 22);
	  
	  if (!options->remote)
	    argp_error(state, "Invalid port or service '%s'.", options->port);

	  break;
	}
      else
	/* Let the next case parse it.  */
	return ARGP_ERR_UNKNOWN;

      break;
    case ARGP_KEY_ARGS:
      client_add_action
	(options,
	 client_command_session
	 (options, rebuild_command_line(state->argc - state->next,
				     state->argv + state->next)));
      options->start_shell = 0;
      break;

    case ARGP_KEY_END:
      if (!options->user)
	{
	  argp_error(state, "No user name given. Use the -l option, or set LOGNAME in the environment.");
	  break;
	}

#if WITH_TCP_FORWARD
      if (options->remote_forward)
	client_add_action(options,
			  make_install_fix_channel_open_handler
			  (ATOM_FORWARDED_TCPIP, &channel_open_forwarded_tcpip));
#endif /* WITH_TCP_FORWARD */
      
      /* Add shell action */
      if (options->start_shell)
	client_add_action(options, client_shell_session(options));

      break;

    case 'p':
      options->port = arg;
      break;

    case 'l':
      options->user = arg;
      break;

    case 'E':
      client_add_action(options, client_command_session(options, ssh_format("%lz", arg)));
      break;

    case 'S':
      client_add_action(options, client_shell_session(options));
      break;

    case 'L':
      {
	UINT32 listen_port;
	struct address_info *target;

	if (!client_parse_forward_arg(arg, &listen_port, &target))
	  argp_error(state, "Invalid forward specification '%s'.", arg);

	client_add_action(options, make_forward_local_port
			  (options->backend,
			   make_address_info((options->with_remote_peers
					      ? NULL
					      : ssh_format("%lz", "127.0.0.1")),
					     listen_port),
			   target));
	break;
      }      

#if 0
    case 'R':
      {
	UINT32 listen_port;
	struct address_info *target;

	if (!client_parse_forward_arg(arg, &listen_port, &target))
	  argp_error(state, "Invalid forward specification '%s'.", arg);

	client_add_action(options, make_forward_remote_port
			  (options->backend,
			   make_address_info((options->with_remote_peers
					      ? ssh_format("%lz", "0.0.0.0")
					      : ssh_format("%lz", "127.0.0.1")),
					     listen_port),
			   target));
	options->remote_forward = 1;
	break;
      }      
#endif 
    case 'N':
      options->start_shell = 0;
      break;

    CASE_FLAG('g', with_remote_peers);

#if WITH_PTY_SUPPORT
    CASE_FLAG('t', with_pty);
#endif /* WITH_PTY_SUPPORT */

#if MACOS
    CASE_ARG(OPT_STDIN, stdin_file, "dev:null");
    CASE_ARG(OPT_STDOUT, stdout_file, "dev:null"); 
    CASE_ARG(OPT_STDERR, stderr_file, "dev:null");
#else
    CASE_ARG(OPT_STDIN, stdin_file, "/dev/null");
    CASE_ARG(OPT_STDOUT, stdout_file, "/dev/null"); 
    CASE_ARG(OPT_STDERR, stderr_file, "/dev/null");
#endif

#if !defined(MACOS)
    case OPT_FORK_STDIO:
      if (options->not)
	{
	  options->not = 0;
	case OPT_FORK_STDIO | ARG_NOT:
	  options->stdin_fork = options->stdout_fork = options->stderr_fork = 0;
	  break;
	}
      else if (!arg)
	options->stdin_fork = options->stdout_fork = options->stderr_fork = 1;
      else
	{
	  int i;

	  options->stdin_fork = options->stdout_fork = options->stderr_fork = 0;

	  for (i = 0; arg[i]; i++)
	    switch(arg[i])
	      {
	      case 'i': case 'I':
		options->stdin_fork = 1;
		break;
	      case 'o': case 'O':
		options->stdout_fork = 1;
		break;
	      case 'e': case 'E':
		options->stderr_fork = 1;
		break;
	      default:
		argp_error(state, "The argument to --cvs-workaround should "
			   "be one or more of the characters 'i' (stdin), "
			   "'o' (stdout) and 'e' (stderr).");
		goto loop_done;
	      }
	loop_done:
	}
      break;
#endif
		
    case 'n':
      options->not = !options->not;
      break;
    }
  return 0;
}

const struct argp client_argp =
{
  client_options,
  client_argp_parser,
  NULL, NULL, NULL, NULL, NULL
};

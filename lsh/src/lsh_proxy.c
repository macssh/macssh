/* lsh_proxy.c
 *
 * main proxy program.
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999 Niels Möller, Balázs Scheidler
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

#include "algorithms.h"
#include "alist.h"
#include "atoms.h"
#include "channel.h"
#include "channel_commands.h"
#include "charset.h"
#include "client_keyexchange.h"
#include "compress.h"
#include "connection_commands.h"
#include "crypto.h"
#include "daemon.h"
#include "dsa.h"
#include "format.h"
#include "gateway_channel.h"
#include "handshake.h"
#include "io.h"
#include "io_commands.h"
#include "lookup_verifier.h"
#include "proxy.h"
#include "proxy_agentforward.h"
#include "proxy_session.h"
#include "proxy_tcpforward.h"
#include "proxy_userauth.h"
#include "proxy_x11forward.h"
#include "randomness.h"
#include "reaper.h"
#include "server.h"
#include "server_authorization.h"
#include "server_keyexchange.h"
#include "server_session.h"
#include "server_userauth.h"
#include "sexp.h"
#include "sexp_commands.h"
#include "spki.h"
#include "spki_commands.h"
#include "ssh.h"
#include "version.h"
#include "werror.h"
#include "xalloc.h"

#include "lsh_argp.h"

/* Forward declarations */
struct command_simple options2local;
#define OPTIONS2LOCAL (&options2local.super.super)

static struct command options2keyfile;
#define OPTIONS2KEYFILE (&options2keyfile.super)

struct command_simple options2signature_algorithms;
#define OPTIONS2SIGNATURE_ALGORITHMS \
  (&options2signature_algorithms.super.super)

struct command_simple proxy_destination;
#define PROXY_DESTINATION (&proxy_destination.super.super)


#include "lsh_proxy.c.x"

#include <assert.h>

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

/* Option parsing */

const char *argp_program_version
= "lsh_proxy-" VERSION;

const char *argp_program_bug_address = BUG_ADDRESS;

#define OPT_NO 0x400
#define OPT_SSH1_FALLBACK 0x200
#define OPT_INTERFACE 0x201
#define OPT_TCPIP_FORWARD 0x202
#define OPT_NO_TCPIP_FORWARD (OPT_TCPIP_FORWARD | OPT_NO)
#define OPT_X11_FORWARD 0x203
#define OPT_NO_X11_FORWARD (OPT_X11_FORWARD | OPT_NO)
#define OPT_AGENT_FORWARD  0x204
#define OPT_NO_AGENT_FORWARD (OPT_AGENT_FORWARD | OPT_NO)
#define OPT_DAEMONIC 0x205
#define OPT_NO_DAEMONIC (OPT_DAEMONIC | OPT_NO)
#define OPT_PIDFILE 0x206
#define OPT_NO_PIDFILE (OPT_PIDFILE | OPT_NO)
#define OPT_CORE 0x207

/* GABA:
   (class
     (name lsh_proxy_options)
     (super algorithms_options)
     (vars
       (backend object io_backend)
       (random object randomness_with_poll)
       (signature_algorithms object alist)
       (style . sexp_argp_state)
       (interface . "char *")
       (port . "char *")
       (hostkey . "char *")
       (local object address_info)
       (destination object address_info)
       (with_tcpip_forward . int)
       (with_x11_forward . int)
       (with_agent_forward . int)
       (daemonic . int)
       (corefile . int)
       (pid_file . "const char *")
       ; -1 means use pid file iff we're in daemonic mode
       (use_pid_file . int)))
*/

static struct lsh_proxy_options *
make_lsh_proxy_options(struct io_backend *backend, 
		       struct randomness_with_poll *random, 
		       struct alist *algorithms)
{
  NEW(lsh_proxy_options, self);

  init_algorithms_options(&self->super, algorithms);
  self->signature_algorithms
    = make_alist(1,
		 ATOM_DSA, make_dsa_algorithm(&random->super), -1);

  self->backend = backend;
  self->random = random;
  self->style = SEXP_TRANSPORT;
  self->interface = NULL;

  /* Default behaviour is to lookup the "ssh" service, and fall back
   * to port 22 if that fails. */
  self->port = NULL;

  /* FIXME: this should perhaps use sysconfdir */  
  self->hostkey = "/etc/lsh_host_key";
  self->local = NULL;

  self->with_tcpip_forward = 1;
  self->with_x11_forward = 1;
  self->with_agent_forward = 1;
  
  self->daemonic = 0;

  /* FIXME: Make the default a configure time option? */
  self->pid_file = "/var/run/lsh_proxy.pid";
  self->use_pid_file = -1;
  self->corefile = 0;
  
  return self;
}

/* Port to listen on */
DEFINE_COMMAND_SIMPLE(options2local, a)
{
  CAST(lsh_proxy_options, options, a);
  return &options->local->super;
}

/* alist of signature algorithms */
DEFINE_COMMAND_SIMPLE(options2signature_algorithms, a)
{
  CAST(lsh_proxy_options, options, a);
  return &options->signature_algorithms->super;
}

/* Read server's private key */
static void
do_options2keyfile(struct command *ignored UNUSED,
		   struct lsh_object *a,
		   struct command_continuation *c,
		   struct exception_handler *e)
{
  CAST(lsh_proxy_options, options, a);
  
  struct lsh_fd *f;

  f = io_read_file(options->backend, options->hostkey, e);

  if (f)
    COMMAND_RETURN(c, f);
  else
    {
      werror("Failed to open '%z' (errno = %i): %z.\n",
	     options->hostkey, errno, STRERROR(errno));
      EXCEPTION_RAISE(e, make_io_exception(EXC_IO_OPEN_READ, NULL, errno, NULL));
    }
}

static struct command options2keyfile =
STATIC_COMMAND(do_options2keyfile);

static const struct argp_option
main_options[] =
{
  /* Name, key, arg-name, flags, doc, group */
  { "interface", OPT_INTERFACE, "interface", 0,
    "Listen on this network interface", 0 }, 
  { "port", 'p', "Port", 0, "Listen on this port.", 0 },
  { "host-key", 'h', "Key file", 0, "Location of the server's private key.", 0},
  { "destination", 'D', "destination:port", 0, "Destination ssh server address (transparent if not given)", 0 },

#if WITH_TCP_FORWARD
  { "tcp-forward", OPT_TCPIP_FORWARD, NULL, 0, "Enable tcpip forwarding (default).", 0 },
  { "no-tcp-forward", OPT_NO_TCPIP_FORWARD, NULL, 0, "Disable tcpip forwarding.", 0 },
#endif /* WITH_TCP_FORWARD */
#if WITH_X11_FORWARD
  { "x11-forward", OPT_X11_FORWARD, NULL, 0, "Enable X11 forwarding (default).", 0 },
  { "no-x11-forward", OPT_NO_X11_FORWARD, NULL, 0, "Disable X11 forwarding.", 0 },
#endif
#if WITH_AGENT_FORWARD
  { "agent-forward", OPT_AGENT_FORWARD, NULL, 0, "Enable auth agent forwarding (default).", 0 },
  { "no-agent-forward", OPT_NO_AGENT_FORWARD, NULL, 0, "Disable auth agent forwarding.", 0 },
#endif

  { NULL, 0, NULL, 0, "Daemonic behaviour", 0 },
  { "daemonic", OPT_DAEMONIC, NULL, 0, "Run in the background, redirect stdio to /dev/null, and chdir to /.", 0 },
  { "no-daemonic", OPT_NO_DAEMONIC, NULL, 0, "Run in the foreground, with messages to stderr (default).", 0 },
  { "pid-file", OPT_PIDFILE, "file name", 0, "Create a pid file. When running in daemonic mode, "
    "the default is /var/run/lsh_proxy.pid.", 0 },
  { "no-pid-file", OPT_NO_PIDFILE, NULL, 0, "Don't use any pid file. Default in non-daemonic mode.", 0 },
  { "enable-core", OPT_CORE, NULL, 0, "Dump core on fatal errors (disabled by default).", 0 },
    
  { NULL, 0, NULL, 0, NULL, 0 }
};

static const struct argp_child
main_argp_children[] =
{
  { &sexp_input_argp, 0, "", 0 },
  { &algorithms_argp, 0, "", 0 },
  { &werror_argp, 0, "", 0 },
  { NULL, 0, NULL, 0}
};

static int 
parse_dest_arg(char *arg,
	       struct address_info **target)
{
  char *sep, *end;
  int port;
  
  sep = strchr(arg, ':');
  if (!sep)
    return 0;
    
  port = strtol(sep + 1, &end, 0);
  if ( (end == sep + 1) || (*end != '\0')
       || (port < 0) || (port > 0xffff) )
    return 0;

  *target = make_address_info(ssh_format("%ls", sep - arg, arg), port);
  return 1;
}

static error_t
main_argp_parser(int key, char *arg, struct argp_state *state)
{
  CAST(lsh_proxy_options, self, state->input);
  
  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_INIT:
      state->child_inputs[0] = &self->style;
      state->child_inputs[1] = &self->super;
      state->child_inputs[2] = NULL;
      break;
    case ARGP_KEY_ARG:
      argp_error(state, "Spurious arguments.");
      break;
      
    case ARGP_KEY_END:
      if (self->port)
	self->local = make_address_info_c(arg, self->port, 0);
      else
	self->local = make_address_info_c(arg, "ssh", 22);

      if (!self->local)
	argp_error(state, "Invalid interface, port or service, %s:%s'.",
		   self->interface ? self->interface : "ANY",
		   self->port);
      if (self->use_pid_file < 0)
	self->use_pid_file = self->daemonic;

      /* Start background poll */
      RANDOM_POLL_BACKGROUND(self->random->poller);
      
      break;
      
    case 'p':
      self->port = arg;
      break;

    case 'h':
      self->hostkey = arg;
      break;
      
    case 'D':
      if (!parse_dest_arg(arg, &self->destination))
        {
          argp_error(state, "Invalid destination specification '%s'.", arg);
        }
      
      break;

    case OPT_INTERFACE:
      self->interface = arg;
      break;
 
    case OPT_TCPIP_FORWARD:
      self->with_tcpip_forward = 1;
      break;

    case OPT_NO_TCPIP_FORWARD:
      self->with_tcpip_forward = 0;
      break;

    case OPT_X11_FORWARD:
      self->with_x11_forward = 1;
      break;

    case OPT_NO_X11_FORWARD:
      self->with_x11_forward = 0;
      break;

    case OPT_AGENT_FORWARD:
      self->with_agent_forward = 1;
      break;

    case OPT_NO_AGENT_FORWARD:
      self->with_agent_forward = 0;
      break;

    case OPT_DAEMONIC:
      self->daemonic = 1;
      break;

    case OPT_NO_DAEMONIC:
      self->daemonic = 0;
      break;

    case OPT_PIDFILE:
      self->pid_file = arg;
      self->use_pid_file = 1;
      break;

    case OPT_NO_PIDFILE:
      self->use_pid_file = 0;
      break;

    case OPT_CORE:
      self->corefile = 1;
      break;
    }
  return 0;
}

static const struct argp
main_argp =
{ main_options, main_argp_parser, 
  NULL,
  "Server for the ssh-2 protocol.",
  main_argp_children,
  NULL, NULL
};

/* GABA:
   (class
     (name fake_host_db)
     (super lookup_verifier)
     (vars
       ;; (algorithm object signature_algorithm)
       ))
*/

static struct verifier *
do_host_lookup(struct lookup_verifier *c UNUSED,
	       int method,
               struct lsh_user *user UNUSED,
               struct lsh_string *key)
{
  switch(method)
    {
    case ATOM_SSH_DSS:
	return make_ssh_dss_verifier(key->length, key->data);
    default:
      return NULL;
    }
}

static struct lookup_verifier *
make_fake_host_db(void)
{
  NEW(fake_host_db, res);

  res->super.lookup = do_host_lookup;

  return &res->super;
}

/* GABA:
   (class
     (name proxy_destination)
     (super command)
     (vars
       (options object lsh_proxy_options)))
*/

static void
do_proxy_destination(struct command *s,
		     struct lsh_object *x,
		     struct command_continuation *c,
		     struct exception_handler *e)
{
  CAST(proxy_destination, closure, s);
  CAST(listen_value, client_addr, x);

  if (closure->options->destination)
    COMMAND_RETURN(c, closure->options->destination);
  else
    {
      /* FIXME: Why not use client_addr->peer? /nisse*/
      struct sockaddr_in sa;
      int salen = sizeof(sa);

      /* try to be transparent */
      /* FIXME: support non AF_INET address families */
      if (getsockname(client_addr->fd->fd, (struct sockaddr *) &sa, &salen) != -1)
        {
          COMMAND_RETURN(c,
			 sockaddr2info(salen, (struct sockaddr *) &sa));
          /* a = make_address_info(ssh_format("localhost"), 1998); */
        }
      else
	{
	  struct exception *ex = 
	    make_io_exception(EXC_IO, client_addr->fd, errno, "getsockname() failed");

	  EXCEPTION_RAISE(e, ex);
	}
    }
}

static struct command *
make_proxy_destination_command(struct lsh_proxy_options *options)
{
  NEW(proxy_destination, self);

  self->super.call = do_proxy_destination;
  self->options = options;
  return &self->super;
}

DEFINE_COMMAND_SIMPLE(proxy_destination, a)
{
  CAST(lsh_proxy_options, options, a);
  return &make_proxy_destination_command(options)->super;
}

/* GABA:
   (expr
     (name lsh_proxy_listen)
     (params
       (listen object command)
       (services object command)
       (handshake_server object command)
       (handshake_client object command))
     (expr 
       (lambda (options)
         (services 
	   (let 
	     ; accept a connection
             ((client_addr (listen(options2local options)))) 
	     ; chain two connections
	     (chain_connections 
	       (handshake_server options)   ; callback to perform server side handshake
	       (handshake_client options)   ; callback to perform client side handshake
	       client_addr))))))	    ; address of accepted connection
	     
*/

/* GABA:
   (expr
     (name lsh_proxy_handshake_client)
     (params
       (handshake object handshake_info)
       (init object make_kexinit))
     (expr
       (lambda (options client_addr)
         (connection_handshake    ; handshake on the client side
            handshake init
	    (spki_read_hostkeys (options2signature_algorithms options)
	                        (options2keyfile options)) 
	    (log_peer client_addr)))))

*/

/* GABA:
   (expr
     (name lsh_proxy_handshake_server)
     (params
       (connect object command)
       (verifier object lookup_verifier)
       (handshake object handshake_info)
       (init object make_kexinit))
     (expr 
       (lambda (options client_addr)
         (init_connection_service 
	   (connection_handshake 
             handshake init
             verifier 
             (connect (proxy_destination options client_addr)))))))
*/

/* Invoked when the client requests the userauth service. */
/* GABA:
   (expr
     (name lsh_proxy_services)
     (params 
       (userauth object command))
     (expr
       (lambda (connection)
         ((userauth connection) connection))))
*/

/* Invoked when starting the ssh-connection service */
/* GABA:
   (expr
     (name lsh_proxy_connection_service)
     (params
       (login object command))
     (expr
       (lambda (user connection)
         (login user
	    ; We have to initialize the connection
	    ; before logging in.
	    (init_connection_service connection)))))
*/

#if WITH_ALF
#include "proxy_alf.h"
#endif


int main(int argc, char **argv)
{
  struct lsh_proxy_options *options;
  
  struct alist *keys;
  
  struct reap *reaper;
  
  struct randomness_with_poll *r;
  struct alist *algorithms_server, *algorithms_client;
  struct alist *signature_algorithms;
  
  struct make_kexinit *make_kexinit;
  struct exception_handler *handler;

  struct io_backend *backend;

  backend = make_io_backend();

#if WITH_ALF
  proxy_alf_init();
  proxy_alf_end();
#endif


  /* For filtering messages. Could perhaps also be used when converting
   * strings to and from UTF8. */
  setlocale(LC_CTYPE, "");
  /* FIXME: Choose character set depending on the locale */
  set_local_charset(CHARSET_LATIN1);

  handler = make_report_exception_handler
    (make_report_exception_info(EXC_IO, EXC_IO, "lsh_proxy: "),
     &default_exception_handler,
     HANDLER_CONTEXT);
  reaper = make_reaper(backend); 
  r = make_default_random(reaper, handler);

  algorithms_server = all_symmetric_algorithms();
  /* FIXME: copy algorithms_server */
  algorithms_client = all_symmetric_algorithms();
  
  signature_algorithms = all_signature_algorithms(&r->super);
  
  options = make_lsh_proxy_options(backend, r, algorithms_server);
  
  argp_parse(&main_argp, argc, argv, 0, NULL, options);

  if (!options->corefile && !daemon_disable_core())
    {
      werror("Disabling of core dumps failed.\n");
      return EXIT_FAILURE;
    }
  
  if (options->daemonic)
    {
#if HAVE_SYSLOG
      set_error_syslog("lsh_proxy");
#else /* !HAVE_SYSLOG */
      werror("lsh_proxy: No syslog. Further messages will be directed to /dev/null.\n");
#endif /* !HAVE_SYSLOG */
    }

  if (options->daemonic)
    switch (daemon_init())
      {
      case 0:
	werror("lsh_proxy: Spawning into background failed.\n");
	return EXIT_FAILURE;
      case DAEMON_INETD:
	werror("lsh_proxy: spawning from inetd not yet supported.\n");
	return EXIT_FAILURE;
      case DAEMON_INIT:
      case DAEMON_NORMAL:
	break;
      default:
	fatal("Internal error\n");
      }
  
  if (options->use_pid_file && !daemon_pidfile(options->pid_file))
    {
      werror("lsh_proxy seems to be running already.\n");
      return EXIT_FAILURE;
    }
      
  /* Read the hostkey */
  keys = make_alist(0, -1);
#if 0
  if (!(hostkey = read_spki_key_file(options->hostkey,
				     make_alist(1, ATOM_DSA, make_dsa_algorithm(r), -1),
				     &ignore_exception_handler)))
    {
      werror("lsh_proxy: Could not read hostkey.\n");
      return EXIT_FAILURE;
    }
  ALIST_SET(keys, hostkey->type, hostkey);
#endif

  /* FIXME: We should check that we have at least one host key.
   * We should also extract the host-key algorithms for which we have keys,
   * instead of hardcoding ssh-dss below. */
 
#if 0
  lookup_keys = make_alist(1, 
			   ATOM_SSH_DSS, make_fake_host_db(),
			   -1);
#endif

  ALIST_SET(algorithms_server, 
	    ATOM_DIFFIE_HELLMAN_GROUP1_SHA1,
	    &make_dh_server(make_dh1(&r->super))->super);
  ALIST_SET(algorithms_client, 
	    ATOM_DIFFIE_HELLMAN_GROUP1_SHA1,
	    &make_dh_client(make_dh1(&r->super))->super);
  
  make_kexinit
    = make_simple_kexinit(&r->super,
			  make_int_list(1, ATOM_DIFFIE_HELLMAN_GROUP1_SHA1,
					-1),
			  options->super.hostkey_algorithms,
			  options->super.crypto_algorithms,
			  options->super.mac_algorithms,
			  options->super.compression_algorithms,
			  make_int_list(0, -1));
  
  {
    /* Commands to be invoked on the connection */
    struct object_queue connection_server_hooks, connection_client_hooks;
    struct alist *server_session_requests, *client_session_requests;

    server_session_requests = make_alist
      (4,
       ATOM_SHELL, &gateway_channel_request, 
       ATOM_PTY_REQ, &gateway_channel_request,
       ATOM_EXIT_STATUS, &gateway_channel_request,
       ATOM_EXIT_SIGNAL, &gateway_channel_request,
       -1);
    client_session_requests = make_alist(0, -1);

#if WITH_X11_FORWARD
    if (options->with_x11_forward)
      {
	ALIST_SET(server_session_requests, ATOM_X11_REQ, &gateway_channel_request);
      }
#endif
#if WITH_AGENT_FORWARD
    if (options->with_agent_forward)
      {
	ALIST_SET(server_session_requests, ATOM_AUTH_AGENT_REQ, &gateway_channel_request);
      }
#endif

    object_queue_init(&connection_server_hooks);
    object_queue_init(&connection_client_hooks);

    object_queue_add_tail(&connection_server_hooks,
			  &make_install_fix_channel_open_handler
			  (ATOM_SESSION, 
			   make_proxy_open_session(server_session_requests,
						   client_session_requests))->super);

#ifdef WITH_TCP_FORWARD
    if (options->with_tcpip_forward)
      {
	object_queue_add_tail(&connection_server_hooks,
			      &make_install_fix_global_request_handler
			      (ATOM_TCPIP_FORWARD, &gateway_global_request)->super);
	object_queue_add_tail(&connection_server_hooks,
			      &make_install_fix_global_request_handler
			      (ATOM_CANCEL_TCPIP_FORWARD, &gateway_global_request)->super);
	object_queue_add_tail(&connection_server_hooks,
			      &make_install_fix_channel_open_handler
			      (ATOM_DIRECT_TCPIP,
			       make_proxy_open_direct_tcpip())->super);
	
	object_queue_add_tail(&connection_client_hooks,
			      &make_install_fix_channel_open_handler
			      (ATOM_FORWARDED_TCPIP,
			       make_proxy_open_forwarded_tcpip())->super);
      }
#endif

#if WITH_X11_FORWARD
    if (options->with_x11_forward)
      {
	object_queue_add_tail(&connection_client_hooks,
			      &make_install_fix_channel_open_handler
			      (ATOM_X11,
			       make_proxy_open_x11())->super);
      }
#endif

#if WITH_AGENT_FORWARD
    if (options->with_agent_forward)
      {
	object_queue_add_tail(&connection_client_hooks,
			      &make_install_fix_channel_open_handler
			      (ATOM_AUTH_AGENT,
			       make_proxy_open_auth_agent())->super);
      }
#endif
      
    {
      struct lsh_object *o = lsh_proxy_listen
	(make_simple_listen(backend, NULL),
	 make_proxy_offer_service
	 (make_alist(1, 
		     ATOM_SSH_USERAUTH, 
		     lsh_proxy_services
		     (make_proxy_userauth
		      (make_int_list(1, ATOM_PASSWORD, -1), 
		       make_alist(1, ATOM_PASSWORD, &proxy_password_auth, -1),
		       make_alist
		       (1, 
			ATOM_SSH_CONNECTION,
			lsh_proxy_connection_service
			(make_proxy_connection_service
			 (queue_to_list_and_kill(&connection_server_hooks),
			  queue_to_list_and_kill(&connection_client_hooks))),
			-1))),
		      -1)),

	 /* callback to call when client<->proxy handshake finished */
	 (struct command *)lsh_proxy_handshake_server(make_simple_connect(backend, NULL),
						      make_fake_host_db(),
						      make_handshake_info
						      (CONNECTION_CLIENT,
						       "lsh_proxy_client - a free ssh",
						       "proxy client",
						       SSH_MAX_PACKET,
						       &r->super,
						       algorithms_client,
						       NULL),
						      make_kexinit),
	 (struct command *)lsh_proxy_handshake_client(make_handshake_info
						      (CONNECTION_SERVER,
						       "lsh_proxy_server - a free ssh",
						       "proxy server",
						       SSH_MAX_PACKET,
						       &r->super,
						       algorithms_server,
						       NULL),
						      make_kexinit));


    
      CAST_SUBTYPE(command, server_listen, o);

      COMMAND_CALL(server_listen, options,
                   &discard_continuation,
		   handler);
    }
  }
  
  io_run(backend);

  return 0;
}

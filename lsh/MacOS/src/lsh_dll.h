/*
 * lsh_dll.h
 * (c) 2000 Jean-Pierre Stierlin.
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
#ifndef LSH_DLL_H_
#define LSH_DLL_H_

#include <Events.h>

typedef void *lshctx;

typedef void (*hdlevtfunc)(long userData, EventRecord *userEvent, long sleepTime);
typedef void (*logfunc)(long userData, const char *message);
typedef char *(*getpassfunc)(long userData, const char *prompt);
typedef int (*yesornofunc)(long userData, const char *prompt, int def);

enum {
	kLSHConvertLFs = 0x1,
	kLSHStripCRs   = 0x2
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * lsh_new
 *
 * input:
 *
 *	- argstr:    argument string (see Usage below).
 *	- hdlevt     callback to main event handler
 *	- log:       callback for lsh logs
 *	- getpass:   called upon password request
 *	- yes_or_no: called upon new host keys confirmation request
 *
 *  returns:
 *
 *  - opaque lshctx struct to use for further calls
 *
 * --------------------------------------------------------------------------------
 *
 *	Usage: lsh [OPTION...] host
 *	  or:  lsh [OPTION...] host command ...
 *	Connects to a remote machine
 *	
 *	      --capture-to=File      When a new hostkey is received, append an ACL
 *	                             expressing trust in the key. In sloppy mode, the
 *	                             default is captured_keys.
 *	      --dh-keyexchange       Enable DH support (default, unless SRP is being
 *	                             used).
 *	      --host-db=Filename     By default, known_hosts
 *	  -i, --identity=Identity key   Use this key to authenticate.
 *	      --no-dh-keyexchange    Disable DH support.
 *	      --no-publickey         Don't try publickey user authentication.
 *	      --no-srp-keyexchange   Disable experimental SRP support (default).
 *	      --no-userauth          Request the ssh-userauth service (default if SRP
 *	                             is used).
 *	      --publickey            Try publickey user authentication (default).
 *	      --sloppy-host-authentication
 *	                             Allow untrusted hostkeys.
 *	      --srp-keyexchange      Enable experimental SRP support.
 *	      --strict-host-authentication
 *	                             Never, never, ever trust an unknown hostkey.
 *	                             (default)
 *	      --userauth             Request the ssh-userauth service (default, unless
 *	                             SRP is being used).
 *	      --debug                Print huge amounts of debug information
 *	  -q, --quiet                Suppress all warnings and diagnostic messages
 *	      --trace                Detailed trace
 *	  -v, --verbose              Verbose diagnostic messages
 *	
 *	 Algorithm selection:
 *	  -c, --crypto=Algorithm
 *	      --hostkey-algorithm=Algorithm
 *	      --list-algorithms      List supported algorithms.
 *	  -m, --mac=Algorithm
 *	  -z, --compression[=Algorithm]   Default is zlib.
 *	
 *	  -l, --user=User name       Login as this user.
 *	  -p, --port=Port            Connect to this port.
 *	
 *	 Actions:
 *	  -E, --execute=command      Execute a command on the remote machine
 *	  -L, --forward-local-port=local-port:target-host:target-port
 *	  -N, --nop                  No operation (suppresses the default action, which
 *	                             is to spawn a remote shell)
 *	  -S, --shell=command        Spawn a remote shell
 *	
 *	 Universal not:
 *	  -n, --no                   Inverts the effect of the next modifier
 *	
 *	 Modifiers that apply to port forwarding:
 *	  -g, --remote-peers         Allow remote access to forwarded ports
 *	      --no-remote-peers      Disallow remote access to forwarded ports
 *	                             (default).
 *	
 *	 Modifiers that apply to remote execution:
 *	      --no-pty               Don't request a remote pty.
 *	      --no-stderr            Redirect stderr to /dev/null
 *	      --no-stdin             Redirect stdin from /dev/null
 *	      --no-stdout            Redirect stdout to /dev/null
 *	      --stderr=Filename      Redirect stderr
 *	      --stdin=Filename       Redirect stdin
 *	      --stdout=Filename      Redirect stdout
 *	  -t, --pty                  Request a remote pty (default).
 *	
 *	  -R, --forward-remote-port=remote-port:target-host:target-port
 *	
 *	  -?, --help                 Give this help list
 *	      --usage                Give a short usage message
 *	  -V, --version              Print program version
 *	
 *	Mandatory or optional arguments to long options are also mandatory or optional
 *	for any corresponding short options.
 *	
 *	Connects to the remote machine, and then performs one or more actions, i.e.
 *	command execution, various forwarding services. The default action is to start
 *	a remote interactive shell or execute a given command on the remote machine.
 *	
 * --------------------------------------------------------------------------------
 *	Sample argstr:
 *	
 *	lsh -lusername -iidentityfile --sloppy-host-authentication 192.168.1.41
 *	
 *	*IMPORTANT* all the data after the first argument (i.e. the hostname)
 *      are passed as a command to launch on the server.
 */

lshctx *lsh_new(char *argstr, hdlevtfunc hdlevt, logfunc log, getpassfunc getpass,
				yesornofunc yes_or_no, unsigned long flags, long userData);

void lsh_delete(lshctx *ctx);

void lsh_yield();

int lsh_read(lshctx *ctx, void *buffer, long inbytes);

long lsh_write(lshctx *ctx, const void *buffer, long inbytes);

int lsh_canread(lshctx *ctx);

int lsh_canwrite(lshctx *ctx);

char *lsh_getprefsd();

lshctx *lsh_current();

Boolean lsh_running(lshctx *ctx);

void lsh_init(void);

void lsh_terminate(void);

#ifdef __cplusplus
}
#endif

#endif

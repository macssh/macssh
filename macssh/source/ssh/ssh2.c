/*
 * ssh2.c
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

#include "lshprefix.h"
#include "format.h"
#include "io.h"
#include "xalloc.h"
#include "werror.h"
#include "tty.h"

#include "ssh2.h"
#include "PasswordDialog.h"
#include "base64.h"

#include "libssh2.h"
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SIOUX.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdarg.h>
#include <sched.h>
#include <termios.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <sys/stat.h>

#include "wind.h"
#include "rsdefs.h"
#include "vsdata.h"
#include "event.proto.h"
#include "netevent.proto.h"
#include "InternalEvents.h"
#include "vsinterf.proto.h"
#include "rsinterf.proto.h"
#include "parse.proto.h"
#include "errors.proto.h"
#include "Connections.proto.h"
#include "DlogUtils.proto.h"

#ifndef OUR_PREFS_VERSION_NUMBER
#include "Preferences.h"
#endif

#include <Fonts.h>
#include <LowMem.h>
#include <Gestalt.h>
#include <Resource.h>

extern Boolean			gInitialized;

extern ApplicationPrefs		*gApplicationPrefs;

extern int gMovableModal;
extern int gSetUpMovableModal;

extern int appl_main(int argc, char **argv);
extern char *applname;
extern char *defargstr;

extern RSdata *RSlocal;

extern void ssh2_init();
extern void ssh2_sched();
extern char *getprefsd(ConstStringPtr name, char *buf, size_t size, short *vRefNum, long *parID);

/*----------------------------------------------------------------------------*/

void openlog(const char *id, int flags, int type);
void syslog( int priority, const char *format, ...);
void exit(int result);
void abort(void);
int tty_getwinsize( int fd, struct terminal_dimensions *dims );
int tty_setwinsize( int fd, const struct terminal_dimensions *dims );

char *strtoxor(char *dst, const char *src, char xormask);
char *strfromxor(char *dst, const char *src, char xormask);
int strcmpxor(const char *xored, const char *src, char xormask);
void setcachedpassphrase(struct CachedPass *pass, const char *prompt, const char *password);
struct CachedPass *newcachedpassphrase(lshcontext *context, const char *prompt, const char *password);
void addcachedpassphrase(lshcontext *context, const char *prompt, const char *password);
Boolean getnextcachedpassphrase(const char *prompt, char *password, int *pindex);
void clearcachedpassphrase();

short save_once_cancel1(const char *fingerprint);
short save_once_cancel2(const char *fingerprint);
int yes_or_no(struct lsh_string *s, int def, int free);

void add_one_file(struct lshcontext *context, int fd);
void remove_one_file(struct lshcontext *context, int fd);

void KeyDestructor(void *);
void ssh2_sighandler(int sig);
void init_context(lshcontext *context, short port);
void make_args( char *argstr, char **tabargv, int *argc, char ***argv );
void make_env( lshcontext *context, WindRec *w );

int cvs_listen( int port );

void *ssh2_thread(WindRec*w);
void ssh_protocol_initial(WindRec*w);
void ssh_packet_read(struct WindRec*w, unsigned char*databuf, short datalen);
void ssh_protocol_write(struct WindRec*w, unsigned char*databuf, short datalen);
void ssh_protocol_close(WindRec*w);
void ssh_randomize(void);
void ssh_exportkey(void);
void ssh_wresize(struct WindRec* w);


/*----------------------------------------------------------------------------*/

/* this record has been stolen from my linuxppc pty */

const struct termios defaulttermios = {
	0x00000300,		/* c_iflag;               input mode flags */
	0x00000003,		/* c_oflag;               output mode flags */
	0x00000b0d,		/* c_cflag;               control mode flags */
	0x000005cf,		/* c_lflag;               local mode flags */
	0,				/* c_line;                line discipline (== c_cc[19]) */
	{
	  0x03,0x1c,0x7f,0x15,0x04,0x01,0x00,0x00,
	  0x00,0x00,0x17,0x12,0x1a,0x11,0x13,0x16,
	  0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},				/* c_cc[NCCS];            control characters */
	0,				/* c_ispeed;              input speed */
	0				/* c_ospeed;              output speed */
};


/* true if we log werror/trace... to stdio, false => log to syslog */
Boolean	gLogStdIO = 0;

int g_error_fd = -1;

pthread_key_t ssh2threadkey = NULL;

char homepath[256];
char username[256];

static char gmessage[512];

static char *envp[] = {
	"HOME",
	"LOGNAME",
	"TERM",
	"DISPLAY"
};

enum {
	kEnvHome = 0,
	kEnvLogName,
	kEnvTerm,
	kEnvDisplay
};

struct CachedPass {
	struct CachedPass	*link;
	lshcontext		*context;
	char			*prompt;
	char			password[64];
	time_t			timestamp;
};

struct CachedPass *gcachedpass = NULL;
int gcachepasscount = 0;
int gcachepassdelay = 120;	/* 2 minutes */

/*
 * getenv
 */

char *getenv(const char *var)
{
	int			i;

	lshcontext	*context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	if ( context ) {
		for ( i = 0; i < sizeof(envp) / sizeof(char *); i++ ) {
			if ( strcmp( var, envp[i] ) == 0 ) {
				return context->_envv[i];
			}
		}
	}
	return NULL;
}

/*
 * openlog
 */

void openlog(const char *id, int flags, int type)
{
}

#define BUF_SIZE 1024
/*
 * syslog
 */

void syslog( int priority, const char *format, ...)
{
  va_list		args;
  char			string_buf[BUF_SIZE];
  int			len;

  va_start( args, format );
  len = vsnprintf( string_buf, BUF_SIZE - 1, format, args );
  va_end( args );
  if ( len > 0 ) {
	putlln( string_buf, len );
  }
}

/*
 * strerror
 */

char * strerror( int errnum )
{
	static char		errstr[__max_errstr];
	
	return __strerror( errnum, errstr );
}

/*
 * __strerror
 */

char * __strerror( int errnum, char *str )
{
	switch ( errnum ) {
		case EPERM:             strcpy(str,  "Operation not permitted"); break;
		case ENOENT:            strcpy(str,  "No such file or directory"); break;
		case ESRCH:             strcpy(str,  "No such process"); break;
		case EINTR:             strcpy(str,  "Interrupted system call"); break;
		case EIO:               strcpy(str,  "Input/output error"); break;
		case ENXIO:             strcpy(str,  "Device not configured"); break;
		case E2BIG:             strcpy(str,  "Argument list too long"); break;
		case ENOEXEC:           strcpy(str,  "Exec format error"); break;
		case EBADF:             strcpy(str,  "Bad file descriptor"); break;
		case ECHILD:            strcpy(str,  "No child processes"); break;
		case EDEADLK:           strcpy(str,  "Resource deadlock avoided"); break;
					/* 11 was EAGAIN */
		case ENOMEM:            strcpy(str,  "Cannot allocate memory"); break;
		case EACCES:            strcpy(str,  "Permission denied"); break;
		case EFAULT:            strcpy(str,  "Bad address"); break;
		case ECANCELED:         strcpy(str,  "Operation cancelled"); break;
		case EBUSY:             strcpy(str,  "Device busy"); break;
		case EEXIST:            strcpy(str,  "File exists"); break;
		case EXDEV:             strcpy(str,  "Cross-device link"); break;
		case ENODEV:            strcpy(str,  "Operation not supported by device"); break;
		case ENOTDIR:           strcpy(str,  "Not a directory"); break;
		case EISDIR:            strcpy(str,  "Is a directory"); break;
		case EINVAL:            strcpy(str,  "Invalid argument"); break;
		case ENFILE:            strcpy(str,  "Too many open files in system"); break;
		case EMFILE:            strcpy(str,  "Too many open files"); break;
		case ENOTTY:            strcpy(str,  "Inappropriate ioctl for device"); break;
		case EFBIG:             strcpy(str,  "File too large"); break;
		case ENOSPC:            strcpy(str,  "No space left on device"); break;
		case ESPIPE:            strcpy(str,  "Illegal seek"); break;
		case EROFS:             strcpy(str,  "Read-only file system"); break;
		case EMLINK:            strcpy(str,  "Too many links"); break;
		case EPIPE:             strcpy(str,  "Broken pipe"); break;

/* math software */
		case EDOM:              strcpy(str,  "Numerical argument out of domain"); break;
		case ERANGE:            strcpy(str,  "Result too large"); break;

/* non-blocking and interrupt i/o */
		case EAGAIN:            strcpy(str,  "Resource temporarily unavailable"); break;
		/*case EWOULDBLOCK:       strcpy(str,  "Operation would block"); break;*/
		case EINPROGRESS:       strcpy(str,  "Operation now in progress"); break;
		case EALREADY:          strcpy(str,  "Operation already in progress"); break;

/* ipc/network software -- argument errors */
		case ENOTSOCK:          strcpy(str,  "Socket operation on non-socket"); break;
		case EDESTADDRREQ:      strcpy(str,  "Destination address required"); break;
		case EMSGSIZE:          strcpy(str,  "Message too long"); break;
		case EPROTOTYPE:        strcpy(str,  "Protocol wrong type for socket"); break;
		case ENOPROTOOPT:       strcpy(str,  "Protocol not available"); break;
		case EPROTONOSUPPORT:   strcpy(str,  "Protocol not supported"); break;
		case ESOCKTNOSUPPORT:   strcpy(str,  "Socket type not supported"); break;
		case EOPNOTSUPP:        strcpy(str,  "Operation not supported"); break;
		case EPFNOSUPPORT:      strcpy(str,  "Protocol family not supported"); break;
		case EAFNOSUPPORT:      strcpy(str,  "Address family not supported by protocol family"); break;
		case EADDRINUSE:        strcpy(str,  "Address already in use"); break;
		case EADDRNOTAVAIL:     strcpy(str,  "Can't assign requested address"); break;

/* ipc/network software -- operational errors */
		case ENETDOWN:          strcpy(str,  "Network is down"); break;
		case ENETUNREACH:       strcpy(str,  "Network is unreachable"); break;
		case ENETRESET:         strcpy(str,  "Network dropped connection on reset"); break;
		case ECONNABORTED:      strcpy(str,  "Software caused connection abort"); break;
		case ECONNRESET:        strcpy(str,  "Connection reset by peer"); break;
		case ENOBUFS:           strcpy(str,  "No buffer space available"); break;
		case EISCONN:           strcpy(str,  "Socket is already connected"); break;
		case ENOTCONN:          strcpy(str,  "Socket is not connected"); break;
		case ESHUTDOWN:         strcpy(str,  "Can't send after socket shutdown"); break;
		case ETOOMANYREFS:      strcpy(str,  "Too many references: can't splice"); break;
		case ETIMEDOUT:         strcpy(str,  "Operation timed out"); break;
		case ECONNREFUSED:      strcpy(str,  "Connection refused"); break;

		case ELOOP:             strcpy(str,  "Too many levels of symbolic links"); break;
		case ENAMETOOLONG:      strcpy(str,  "File name too long"); break;

/* should be rearranged */
		case EHOSTDOWN:         strcpy(str,  "Host is down"); break;
		case EHOSTUNREACH:      strcpy(str,  "No route to host"); break;
		case ENOTEMPTY:         strcpy(str,  "Directory not empty"); break;

		case ENOLCK:            strcpy(str,  "No locks available"); break;
		case ENOSYS:            strcpy(str,  "Function not implemented"); break;

#ifndef _POSIX_SOURCE
		/*case ELAST:             strcpy(str,  "Must be equal largest errno"); break;*/
#endif /* _POSIX_SOURCE */
		default:                sprintf(str, "Unknown Error (%d)", errnum); break;
	}

	return str;
}

/*
 * strsignal
 */

#define __max_sigstr	64

char * strsignal( int signo )
{
	static char		sigstr[__max_sigstr];

	switch (signo)
	{
		case SIGHUP:            strcpy(sigstr,  "SIGHUP, hangup"); break;
		case SIGINT:            strcpy(sigstr,  "SIGINT, interrupt"); break;
		case SIGQUIT:           strcpy(sigstr,  "SIGQUIT, quit"); break;
		case SIGILL:            strcpy(sigstr,  "SIGILL, illegal instruction"); break;
		case SIGABRT:           strcpy(sigstr,  "SIGABRT, abort"); break;
		case SIGFPE:            strcpy(sigstr,  "SIGFPE, floating point exception"); break;
		case SIGKILL:           strcpy(sigstr,  "SIGKILL, kill"); break;
		case SIGSEGV:           strcpy(sigstr,  "SIGSEGV, segmentation violation"); break;
		case SIGPIPE:           strcpy(sigstr,  "SIGPIPE, write on a pipe with no one to read it"); break;
		case SIGALRM:           strcpy(sigstr,  "SIGALRM, alarm clock"); break;
		case SIGTERM:           strcpy(sigstr,  "SIGTERM, software termination signal from kill"); break;
#ifndef _POSIX_SOURCE
		case SIGURG:            strcpy(sigstr,  "SIGURG, urgent condition on IO channel"); break;
#endif
		case SIGSTOP:           strcpy(sigstr,  "SIGSTOP, sendable stop signal not from tty"); break;
		case SIGTSTP:           strcpy(sigstr,  "SIGSTOP, stop signal from tty"); break;
		case SIGCONT:           strcpy(sigstr,  "SIGCONT, continue a stopped process"); break;
		case SIGCHLD:           strcpy(sigstr,  "SIGCHLD"); break;
		case SIGTTIN:           strcpy(sigstr,  "SIGTTIN"); break;
		case SIGTTOU:           strcpy(sigstr,  "SIGTTOU"); break;
#ifndef _POSIX_SOURCE
		case SIGIO:             strcpy(sigstr,  "SIGIO"); break;
		case SIGPROF:           strcpy(sigstr,  "SIGPROF"); break;
		case SIGWINCH:          strcpy(sigstr,  "SIGWINCH"); break;
#endif
		case SIGUSR1:           strcpy(sigstr,  "SIGUSR1, user defined signal 1"); break;
		case SIGUSR2:           strcpy(sigstr,  "SIGUSR2, user defined signal 2"); break;
		default:                sprintf(sigstr, "unknown signal no %d", signo); break;
	}
	return sigstr;
}

/*
 * getexitbuf
 */

void setexitbuf(jmp_buf *exitbuf)
{
	lshcontext	*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if ( context && context->_self == context ) {
		context->_pexitbuf = exitbuf;
	}
}

/*
 * getexitbuf
 */

jmp_buf *getexitbuf()
{
	lshcontext	*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if ( context && context->_self == context ) {
		return context->_pexitbuf;
	}
	return NULL;
}

/*
 * exit
 */

void exit(int result)
{
	jmp_buf *exitbuf;

	if (exitbuf = getexitbuf()) {
		longjmp( *exitbuf, 1 );
	}
	/* should never go here... */
	Debugger();
}

/*
 * abort
 */

void abort(void)
{
	exit( EXIT_FAILURE );
}

/*
 * __assertion_failed
 */
void __assertion_failed(char const *condition, char const *filename, int lineno)
{
	//Debugger();

	fprintf( stderr, "Assertion (%s) failed in \"%s\" on line %d\n", condition, filename, lineno );

	if ( pthread_getspecific( ssh2threadkey ) ) {
		macosabort();
	} else {
		#undef abort
		abort();
		#define abort macosabort
	}
}

/*
 * __msl_assertion_failed
 */
void __msl_assertion_failed(char const *condition, char const *filename, char const *funcname, int lineno)
{
	//Debugger();

	fprintf(stderr, "Assertion (%s) failed in \"%s\" on line %d\n", condition, filename, lineno);

	if ( pthread_getspecific(ssh2threadkey) ) {
		macosabort();
	} else {
		#undef abort
		abort();
		#define abort macosabort
	}
}

/*
 * tty_getwinsize : replaces tty_getwinsize from liblsh
 */

int
tty_getwinsize( int fd, struct terminal_dimensions *dims )
{
	WindRec		*wind = ssh2_window();

	if ( wind ) {
		dims->char_width = VSgetcols( wind->vs ) + 1;
		dims->char_height = VSgetlines( wind->vs );
		dims->pixel_width = RSlocal[wind->vs].fwidth * (dims->char_width);
		dims->pixel_height = RSlocal[wind->vs].fheight * (dims->char_height);
		return 1;
	}
	return 0;
}


/*
 * tty_setwinsize : replaces tty_setwinsize from liblsh
 */

int
tty_setwinsize( int fd, const struct terminal_dimensions *dims )
{
	WindRec		*wind = ssh2_window();

	if ( wind ) {
		if ( VSsetlines( wind->vs, dims->char_height) < 0 ) {
			OutOfMemory(-4);
		} else {
			RScalcwsize( wind->vs, dims->char_width );
	 	}
		return 1;
	}
	return 0;
}

/*
 * tcgetattr : replaces tty_getattr from liblsh
 */

int
tcgetattr(int fd, struct termios *ios)
{
	lshcontext	*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if ( context ) {
		memcpy( ios, &context->_mactermios, sizeof(struct termios) );
		return 1;
	}
	return 0;
}

/*
 * tcsetattr : replaces tty_setattr from liblsh
 */

int
tcsetattr(int fd, int optional_actions, const struct termios *ios)
{
	lshcontext	*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if ( context ) {
		memcpy( &context->_mactermios, ios, sizeof(struct termios) );
		return 1;
	}
	return 0;
}

#pragma mark -


/*
 * strtoxor
 */

char *strtoxor(char *dst, const char *src, char xormask)
{
	char		*d = dst;
	char		c;

	do {
		*d++ = ((c = *src++) ^ xormask);
	} while ( c );
	return dst;
}

/*
 * strfromxor
 */

char *strfromxor(char *dst, const char *src, char xormask)
{
	char		*d = dst;
	char		c;

	while ( *d++ = (*src++ ^ xormask) )
		;
	return dst;
}

/*
 * strcmpxor
 */

int strcmpxor(const char *xored, const char *src, char xormask)
{
	const	unsigned char * p1 = (unsigned char *) xored;
	const	unsigned char * p2 = (unsigned char *) src;
	unsigned char		c1, c2;
	
	while ((c1 = (*p1++ ^ xormask)) == (c2 = *p2++))
		if (!c1)
			return 0;
	return c1 - c2;
}

/*
 * setcachedpassphrase
 */

void setcachedpassphrase(struct CachedPass *pass, const char *prompt, const char *password)
{
	char *p;

	p = NewPtr( strlen(prompt) + 1 );
	if ( p == NULL ) {
		/* couldn't save: not fatal, forget it */
		return;
	}
	if ( pass->prompt )
		DisposePtr( pass->prompt );
	pass->prompt = p;

	strtoxor(pass->prompt, prompt, 0xAA);
	strtoxor(pass->password, password, 0xAA);
	pass->timestamp = time(NULL);
}

/*
 * newcachedpassphrase
 */

struct CachedPass *newcachedpassphrase(lshcontext *context, const char *prompt, const char *password)
{
	struct CachedPass *pass;

	pass = NewPtr(sizeof(struct CachedPass));
	if (pass) {
		pass->context = context;
		pass->prompt = NULL;
		setcachedpassphrase(pass, prompt, password);
		pass->link = NULL;
	}
	return pass;
}

/*
 * addcachedpassphrase
 */

void addcachedpassphrase(lshcontext *context, const char *prompt, const char *password)
{
	struct CachedPass *pass = gcachedpass;

	while ( pass ) {
		if ( pass->prompt && !strcmpxor(pass->prompt, prompt, 0xAA) 
		  && !strcmpxor(pass->password, password, 0xAA) ) {
			/* we already have this passphrase, restamp it */
			pass->timestamp = time(NULL);
			return;
		}

		if ( pass->context == context
		 && (!pass->prompt || !strcmpxor(pass->prompt, prompt, 0xAA)) ) {
			/* this limits to one wrong passphrase per session */
			setcachedpassphrase(pass, prompt, password);
			return;
		}

		if ( gApplicationPrefs->cacheDelay
		  && time(NULL) - pass->timestamp >= gApplicationPrefs->cacheDelay ) {
			/* timed out, reuse this one */
			setcachedpassphrase(pass, prompt, password);
			return;
		}
		if (pass->link == NULL) {
			pass->link = newcachedpassphrase(context, prompt, password);
			if ( pass->link ) {
				++gcachepasscount;
			}
			return;
		}
		pass = pass->link;
	}
	gcachedpass = newcachedpassphrase(context, prompt, password);
	if ( gcachedpass ) {
		++gcachepasscount;
	}
}

/*
 * getnextcachedpassphrase
 * prompt may differ from key to key
 * and we can have the same prompt for different keys too
 */

Boolean getnextcachedpassphrase(const char *prompt, char *password, int *pindex)
{
	int i;
	struct CachedPass *pass;

	if ( *pindex >= gcachepasscount ) {
		/* not found in cache */
		*pindex = 0;
		return 0;
	}
	/* fetch next index */
	for ( i = 0, pass = gcachedpass; i < *pindex && pass != NULL; i++ )
		pass = pass->link;
	for ( ; pass != NULL; i++ ) {
		if ( (!gApplicationPrefs->cacheDelay
		    || time(NULL) - pass->timestamp < gApplicationPrefs->cacheDelay)
		  && pass->prompt && !strcmpxor(pass->prompt, prompt, 0xAA) ) {
			/* prepare next index */
			*pindex = i + 1;
			strfromxor(password, pass->password, 0xAA);
			return 1;
		}
		pass = pass->link;
	}
	*pindex = gcachepasscount;
	return 0;
}

/*
 * clearcachedpassphrase
 */

void clearcachedpassphrase()
{
	struct CachedPass *pass;
	struct CachedPass *next;

	if ( (pass = gcachedpass ) != NULL ) {
		gcachedpass = NULL;
		do {
			next = pass->link;
			if ( pass->prompt )
				DisposePtr( pass->prompt );
			DisposePtr((Ptr)pass);
			pass = next;
		} while ( pass != NULL );
	}
}

/* let the user deal with only one dialog at a time */
pthread_mutex_t		dialock = NULL;
Boolean				gThreadModal = false;

/*
 * getpass
 */

void LockDialog()
{
	while (gMovableModal /*gSetUpMovableModal*/)
		ssh2_sched();
	pthread_mutex_lock( &dialock );
	gThreadModal = true;
}

/*
 * getpass
 */

void UnlockDialog()
{
	gThreadModal = false;
	pthread_mutex_unlock( &dialock );
}


/*
 * getpass
 */

char *getpass( const char *prompt )
{
	lshcontext		*context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	WindRec			*wind = ssh2_window();
	char			cprompt[512];
	Str255			pprompt;
	Str255			ppassword;
	char			*password;
	Boolean			valid;
	short			index;

	if ( wind && strstr(prompt, "assword for") ) {
		/* password authentication */
		password = wind->sshdata.currentpass;
		password[0] = '\0';
		/* add host name to avoid sending password to another host... */
		strcpy(cprompt, prompt);
		strncat(cprompt, (char *)wind->sshdata.host + 1, wind->sshdata.host[0]);

		index = context->_pindex;
		if ( gApplicationPrefs->cachePassphrase && !wind->sshdata.password[0]
		  && getnextcachedpassphrase(cprompt, password, &context->_pindex) ) {
			return password;
		}
		LockDialog();
		/* may need to reload the window... */
		wind = ssh2_window();
		if ( wind ) {
			context->_pindex = index;
			if ( gApplicationPrefs->cachePassphrase && !wind->sshdata.password[0]
			  && getnextcachedpassphrase(cprompt, password, &context->_pindex) ) {
				UnlockDialog();
				return password;
			}
			if ( wind->sshdata.password[0] ) {
				memcpy(password, wind->sshdata.password + 1, wind->sshdata.password[0]);
				password[wind->sshdata.password[0]] = '\0';
				wind->sshdata.password[0] = '\0';
				valid = 1;
			} else {
				ppassword[0] = 0;
				valid = SSH2PasswordDialog(prompt, ppassword, wind->wind);
				if (valid) {
					memcpy(password, ppassword + 1, ppassword[0]);
					password[ppassword[0]] = '\0';
				}
			}
			if (valid) {
				if ( gApplicationPrefs->cachePassphrase ) {
					addcachedpassphrase(context, cprompt, password);
				}
			}
		} else {
			// terminal has been closed
			valid = 0;
		}

	} else {
		/* encrypted private key */
		int plen;
		WindowPtr term;
		assert(context != NULL);
		index = context->_kindex;
		if ( gApplicationPrefs->cachePassphrase
		  && getnextcachedpassphrase(prompt, context->_kpassword, &context->_kindex) ) {
			return context->_kpassword;
		}
		LockDialog();
		if (wind == ssh2_window()) {
			context->_kindex = index;
			if ( gApplicationPrefs->cachePassphrase
			  && getnextcachedpassphrase(prompt, context->_kpassword, &context->_kindex) ) {
				UnlockDialog();
				return context->_kpassword;
			}
			term = ( wind ) ? wind->wind : NULL;
			context->_kpassword[0] = 0;
			valid = SSH2PasswordDialog(prompt, (StringPtr)context->_kpassword, term);
			if (valid) {
				plen = context->_kpassword[0];
				password = (wind != NULL) ? wind->sshdata.currentpass : context->_kpassword;
				memcpy(password, context->_kpassword + 1, plen);
				password[plen] = '\0';
				if ( gApplicationPrefs->cachePassphrase ) {
					addcachedpassphrase(context, prompt, password);
				}
			}
		} else {
			// terminal has been closed
			valid = 0;
		}
	}
	UnlockDialog();
	return (valid) ? password : NULL;
}

/*
 * setctxprompt
 */
void setctxprompt(const char *prompt)
{
	strcpy(((lshcontext *)pthread_getspecific(ssh2threadkey))->_keychainprompt, prompt);
}

/*
 * getctxprompt
 */

char *getctxprompt()
{
	return ((lshcontext *)pthread_getspecific(ssh2threadkey))->_keychainprompt;
}

/*
 * save_once_cancel1
 */

short save_once_cancel1(const char *fingerprint)
{
	short		result;

	LockDialog();
	result = SSH2SOC1Dialog(fingerprint);
	UnlockDialog();
	return result;
}

/*
 * save_once_cancel2
 */

short save_once_cancel2(const char *fingerprint)
{
	short		result;

	LockDialog();
	result = SSH2SOC2Dialog(fingerprint);
	UnlockDialog();
	return result;
}

/*
 * yes_or_no
 */

int yes_or_no(struct lsh_string *s, int def, int free)
{
	const char *prompt;

	prompt = lsh_get_cstring(s);
	if ( prompt ) {
		if (!quiet_flag) {
			Str255 pprompt;
			int i;
			pprompt[0] = strlen(prompt);
			memcpy(pprompt + 1, prompt, pprompt[0]);
			for (i = 1; i <= pprompt[0]; i++) {
				if (pprompt[i] == 0x0a) {
					pprompt[i] = 0x0d;
				}
			}
			LockDialog();
			def = YesNoDialog( pprompt );
			UnlockDialog();
		}
	}
	return def;
}

#pragma mark -

/*
 * SIOUXHandleOneEvent
 * unused, but hooked by GUSISIOUXSocket constructor
 */

short SIOUXHandleOneEvent(EventRecord *userevent)
{
	syslog( 0, "#### SIOUXHandleOneEvent : 0x%lx\n", userevent );
	return 1;
}


/*
 * ssh2_doevent
 */

volatile DialogPtr key_dlog = NULL;
volatile int key_gen = 0;
volatile int key_abt = 0;

void ssh2_doevent(long sleepTime)
{
	Boolean memOK;
	extern Boolean haveNotifiedLowMemory;

	if ( !gInitialized ) {
		// don't process GUSI events until full init.
		return;
	}

	if ( key_gen == 0 ) {
		if (!gThreadModal && !/*gSetUpMovableModal*/gMovableModal) {
			DoEvents(NULL);
		}
		if (!TelInfo->done) {
			DoNetEvents();
		}
		memOK = RecoverReserveMemory();
		if ( memOK ) {
			haveNotifiedLowMemory = false;
		} else if ( !haveNotifiedLowMemory )  {
			Alert( MemoryLowAlert, NULL );
			haveNotifiedLowMemory = true;
		}
	} else {
		if (!gThreadModal && !/*gSetUpMovableModal*/gMovableModal) {
			static unsigned long lastTicks = 0L;
			if ( (LMGetTicks() - lastTicks) >= 10 ) {
				EventRecord	myEvent;
				lastTicks = LMGetTicks();
				if ( WaitNextEvent(everyEvent, &myEvent, 0, 0L) ) {
					HandleEvent(&myEvent);
				}
			}
		}
	}
}

#pragma mark -

/*
 * KeyDestructor
 */

void KeyDestructor(void *)
{
}

/*
 * ssh2_sighandler
 */

void ssh2_sighandler(int sig)
{
	switch (sig) {
		case SIGCHLD:
		case SIGCONT:
		case SIGSTOP:
		case SIGTSTP:
		case SIGTTIN:
		case SIGTTOU:
			break;	/* Ignore */
		default:
			exit(1);
	}
}


/*
 * ssh2_window
 */

WindRec *ssh2_window()
{
	lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	if ( context && context->_port >= 0 ) {
		return FindWindByPort(context->_port);
	}
	return NULL;
}

/*
 * get_context_listener
 */

int get_context_listener()
{
	lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	if ( context ) {
		return context->_listener;
	}
	return -1;
}


/*
 * init_context
 */

void init_context(lshcontext *context, short port)
{
	context->_self = context;
	context->_port = port;
	context->_thread = pthread_self();
	context->_forward = 0;
	context->_localport = 0;
	context->_listener = -1;
	context->_socket = -1;
	/*context->_exitbuf = 0;*/
	memcpy(&context->_mactermios, &defaulttermios, sizeof(struct termios));
	context->_gConsoleInEOF = 0;
	/*context->_convertLFs = 0;*/
	context->_convertLFs = 0;
	context->_lastCR = 0;
	context->_insock = NULL;
	context->_gConsoleInBufLen = 0;
	context->_gConsoleInBufMax = CONSOLEBUFSIZE;
	context->_gConsoleOutBufLen = 0;
	context->_gConsoleOutBufMax = CONSOLEBUFSIZE;
	context->_quiet_flag = 0;
	context->_verbose_flag = 0;
	context->_trace_flag = 0;
	context->_debug_flag = 0;
	/*context->_error_fd = STDERR_FILENO;*/
	context->_error_fd = g_error_fd;
	context->_error_pos = 0;
	//context->_error_write = write_raw;
	context->_tracing = 0;
	context->_verbosing = 0;
	context->_debugging = 0;
	context->_window_changed = 0;
	context->_kpassword[0] = 0;
	context->_kindex = 0;
	context->_pindex = 0;
	context->_keychainprompt[0] = 0;
}

#if 0
/*
 * make_args
 */

void make_args( char *argstr, char **tabargv, int *argc, char ***argv )
{
	int i = 0;
	int j = 0;
	int quoted1 = 0;
	int quoted2 = 0;

	tabargv[i++] = argstr;

	#define SEPARATOR(x) ((x) == ' ' || (x) == '\t' || (x) == 10 || (x) == 13 || (x) == 0)
	#define QUOTE1(x) ((x) == '"')
	#define QUOTE2(x) ((x) == '\'')
	while (argstr[j]) {
		if (!quoted1 && !quoted2 && SEPARATOR(argstr[j])) {
			argstr[j++] = 0;
			if (!SEPARATOR(argstr[j])) {
				tabargv[i++] = argstr + j;
			}
		} else if (!quoted2 && QUOTE1(argstr[j])) {
			strcpy(argstr + j, argstr + j + 1);
			if (!QUOTE1(argstr[j]))
				quoted1 = !quoted1;
			else
				j++;
		} else if (!quoted1 && QUOTE2(argstr[j])) {
			strcpy(argstr + j, argstr + j + 1);
			if (!QUOTE2(argstr[j]))
				quoted2 = !quoted2;
			else
				j++;
		} else {
			j++;
		}
	}
	#undef SEPARATOR
	#undef QUOTE

	tabargv[i] = NULL;

	*argc = i;
	*argv = tabargv;
}
#endif

#if 0
/*
 * make_env
 */

void make_env( lshcontext *context, WindRec *w )
{
	char *buf;
	StringHandle hstr;
	int i;
	int err;
	short vRefNum;
	long dirID;
	char folderString[256];

	GetIndString((unsigned char *)folderString,MISC_STRINGS,PREFS_FOLDER_NAME);
	i = ((unsigned char *)folderString)[0];
	BlockMoveData(folderString + 1, folderString, i);
	folderString[i] = 0;

	buf = getprefsd( folderString, homepath, sizeof(homepath), &vRefNum, &dirID );
	if ( !buf ) {
 		buf = getcwd(homepath, sizeof(homepath));
	}
  	context->_envv[kEnvHome] = buf;	/* set home pathname */

	/*hstr = GetString( -16413 );*/ /* get computer name */
	hstr = GetString( -16096 ); /* get user name */
	if ( hstr && *hstr ) {
		i = **hstr;
		BlockMoveData( *hstr + 1, username, i );
		username[i] = 0;
		ReleaseResource( (Handle)hstr );
  		context->_envv[kEnvLogName] = username;	/* set user name */
  	} else {
  		context->_envv[kEnvLogName] = NULL;
  	}

	/* set TERM variable from prefs */
	if ( w && (i = w->answerback[0]) != 0 ) {
		memcpy(context->_term, (char *)w->answerback + 1, i);
		context->_term[i] = '\0';
		context->_envv[kEnvTerm] = context->_term;
  	} else {
  		context->_envv[kEnvTerm] = "vt220";
	}

	/* set DISPLAY variable from prefs (for x11 forwarding) */
	if ( w && (i = w->display[0]) != 0 ) {
		memcpy(context->_display, (char *)w->display + 1, i);
		context->_display[i] = '\0';
		context->_envv[kEnvDisplay] = context->_display;
  	} else {
  		context->_envv[kEnvDisplay] = NULL;
	}
}
#endif

#pragma mark -

/*
 * cvs_listen
 */

int cvs_listen( int port )
{
	struct sockaddr_in	local;
	int					listener;

	local.sin_family		=	AF_INET;
	local.sin_addr.s_addr	=	0;
	local.sin_port			=	htons( port );				

	listener = socket( AF_INET, SOCK_STREAM, 0 );

	if ( listener < 0 ) {
		return -1;
	}

	if ( bind( listener, (struct sockaddr *)&local, sizeof(local) ) < 0 ) {
		close( listener );
		return -1;
	}

	if ( listen( listener, 1 ) < 0 ) {
		close( listener );
		return -1;
	}

	return listener;
}


#pragma mark -

#if 0
/*
 * build_cmdline : prepare lsh client argument string
 *
 *	argstr can be up to 1500 bytes !
 */

static int build_cmdline(WindRec*w, char *argstr)
{
	struct in_addr	addr;
	char			tempstr[256];
	lshcontext		*context;

	strcpy(argstr, applname);

	if ( w->sshdata.login[0] ) {
		strcat(argstr, " -l");
		strncat(argstr, (char *)w->sshdata.login + 1, w->sshdata.login[0]);	/* 256 */
	}

	strcat(argstr, " --capture-to \"");
	strcat(argstr, getenv("HOME"));	/* 256 */
	strcat(argstr, "known_hosts\"");

	strcat(argstr, " --stdin dev:ttyin --stdout dev:ttyout --stderr dev:ttyerr");
 
	if (!w->restricted)
		strcat(argstr, " --sloppy-host-authentication");

	switch (w->encryption) {
		default: strcat(argstr, " -call"); break;
		case 1: strcat(argstr, " -c3des"); break;
		case 2: strcat(argstr, " -ctwofish"); break;
		case 3: strcat(argstr, " -ccast128"); break;
		case 4: strcat(argstr, " -cserpent"); break;
		/*case 5: strcat(argstr, " -crijndael"); break;*/
		case 5: strcat(argstr, " -caes"); break;
		/* case xx: strcat(argstr, " -cidea"); break; */
		case 6: strcat(argstr, " -cblowfish"); break;
		case 7: strcat(argstr, " -carcfour"); break;
		case 8: strcat(argstr, " -cnone"); break;
	}
	switch (w->authentication) {
		default: break; /* hmac-sha1, hmac-md5 */
		case 1: strcat(argstr, " -msha1"); break;
		case 2: strcat(argstr, " -mmd5"); break;
		case 3: strcat(argstr, " -mnone"); break;
	}
	switch (w->compression) {
		default: break; /* none, zlib */
		case 1: strcat(argstr, " -znone"); break;
		case 2: strcat(argstr, " -zzlib"); break;
	}
	if (w->verbose)
		strcat(argstr, " --verbose");
	if (w->trace)
		strcat(argstr, " --trace");
	if (w->debug)
		strcat(argstr, " --debug");
	if ( w->portNum != 22 ) {
		sprintf(tempstr, " -p%d", (unsigned short)w->portNum);
		strcat(argstr, tempstr);
	}
	if ( w->ssh2method == 1 || w->ssh2method == 2 ) {
		/* strcat(argstr, " -N"); */
		/* strcat(argstr, " --no-pty"); */
		if ( w->ssh2guests ) {
			strcat(argstr, " -g");
		}
		/* 255 */
		sprintf(tempstr, " -%c%u:%#s:%u", (w->ssh2method != 2) ? 'L' : 'R', (unsigned short)w->localport, w->remotehost, (unsigned short)w->remoteport);
		strcat(argstr, tempstr);
	} else if ( w->ssh2method == 3 ) {
		/* cvs pseudo 'port forwarding', listen to local socket */
		if (!strstr("--no-pty", w->sshdata.command))
			strcat(argstr, " --no-pty");
		context = w->sshdata.context;
		context->_convertLFs = 1;
		if ( context->_listener == -1 ) {
			context->_listener = cvs_listen( w->localport );
			if ( context->_listener == -1 ) {
				syslog( 0, "### ssh2_thread, cvs_listen failed : %s\n", strerror(errno) );
				return -1;
			}
			/* don't close the listener in close_all_files() */
			remove_one_file( context, context->_listener );
		}
	}

	if ( w->x11forward ) {
		/* x11 forwarding */
		strcat(argstr, " -x");
	}

	/* add 'command' field AFTER stdio redirection */
	if ( w->sshdata.command[0] ) {
		strcat(argstr, " ");
		strncat(argstr, (char *)w->sshdata.command + 1, w->sshdata.command[0]);	/* 256 */
	}

	/* and finally the host */
	strcat(argstr, " ");
	strncat(argstr, (char *)w->sshdata.host + 1, w->sshdata.host[0]);

	return 0;
}
#endif

static void libssh2_handler(LIBSSH2_SESSION *session, void *context, const char *msg, size_t msglen)
{
	syslog(0, "%s\n", msg);
}

extern int WriteCharsToTTY(int id, void *ctx, char *buffer, int n);

static Boolean ssh2_get_known_hosts_file(FSSpec *fsp)
{
	// Return an FSSpec to the known_hosts file in Preferences:MacSSH.
	// The old lsh-based MacSSH created MacSSH:known_hosts with creator code of CWIE. If this is found, rename the file
	// and recreate with creator code 'Ssh2' (kNCSACreatorSignature).
	// Otherwise, find the existing file
	OSErr		err;
	Str255		folderName;
	short		vRefNum;
	long		dirID;
	Boolean		create = false;

	GetIndString(folderName,MISC_STRINGS,PREFS_FOLDER_NAME);
	if( getprefsd(folderName, NULL, 0, &vRefNum, &dirID) == NULL ) {
		syslog(0, "ssh2_get_known_hosts_file(): getprefsd() failed\n");
		return false;
	}

	err = FSMakeFSSpec(vRefNum, dirID, "\pknown_hosts", fsp);

	if( err == noErr )
	{
		// File exists, check creator code
		FInfo finfo;
		err = FSpGetFInfo(fsp, &finfo);
		if( err == noErr )
		{
			if( finfo.fdCreator != kNCSACreatorSignature )
			{
				// rename, recreate file
				syslog(0, "ssh2_get_known_hosts_file(): found old known_hosts file, renaming\n");
				if( FSpRename(fsp, "\pknown_hosts-lsh-old") != noErr )
				{
					syslog(0, "ssh2_get_known_hosts_file(): FSpRename error\n");
					return false;
				}

				create = true;
			}
		}
	}
	else if( err == fnfErr )
	{
		// 'known_hosts' file doesn't exist
		create = true;
	}
	else
	{
		syslog(0, "ssh2_get_known_hosts_file(): FSMakeFSSpec returned error %d\n", err);
		return false;
	}

	if( create ) {
		short refNum;
		syslog(0, "ssh2_get_known_hosts_file(): creating new known_hosts file\n");

		err = FSpCreate(fsp, kNCSACreatorSignature, 'TEXT', smSystemScript);
		if( err != noErr)
		{
			syslog(0, "ssh2_get_known_hosts_file(): FSpCreate returned error %d\n", err);
			return false;
		}

		if( FSpOpenDF(fsp, fsCurPerm, &refNum) == noErr )
		{
			static const char header[] =
				"# known_hosts file for MacSSH 3.0\n# File uses OpenSSH format and Unix line endings ('\\n')\n\n";
			long count = sizeof(header) - 1;

			FSWrite(refNum, &count, header);
			FSClose(refNum);
		}
	}

	// At this point, file exists and has correct creator code (either empty or is our format)
	return true;
}

static Boolean ssh2_read_known_hosts_file(short refNum, LIBSSH2_KNOWNHOSTS *hosts)
{
	// Use PBReadSync to read until a Unix newline ('\n') is reached.
	ParamBlockRec	pb;
	OSErr			err;
	char			buf[2048];

	memset(&pb, 0, sizeof(pb));
	pb.ioParam.ioRefNum = refNum;
	pb.ioParam.ioBuffer = buf;
	pb.ioParam.ioReqCount = sizeof(buf);
	pb.ioParam.ioPosMode = ('\n' << 8) | newLineMask;
	pb.ioParam.ioPosOffset = 0;

	while (true)
	{
		OSErr err = PBReadSync(&pb);

		if( (err == noErr) || ((err == eofErr) && (pb.ioParam.ioActCount > 0)) )
		{
			int err = libssh2_knownhost_readline(hosts, buf, pb.ioParam.ioActCount, LIBSSH2_KNOWNHOST_FILE_OPENSSH);
			if (err != LIBSSH2_ERROR_NONE)
				syslog(0, "ssh2_read_known_hosts_file(): libssh2_knownhost_readline returned error %d\n", err);
		}
		else if( err == eofErr )
		{
			break;
		}
		else
		{
			syslog(0, "ssh2_read_known_hosts_file(): PBReadSync returned error %d\n", err);
			return false;
		}
	}
	return true;
}

static Boolean ssh2_append_host_to_file(short refNum, LIBSSH2_KNOWNHOSTS *hosts, struct libssh2_knownhost *known)
{
	char			buf[2048];
	size_t			outlen;

	{
		int err = libssh2_knownhost_writeline(hosts, known, buf, sizeof(buf), &outlen, LIBSSH2_KNOWNHOST_FILE_OPENSSH);
		if( err != 0 )
		{
			syslog(0, "ssh2_append_host_to_file(): libssh2_knownhost_writeline returned error %d\n", err);
			return false;
		}
	}

	{
		OSErr			err;
		ParamBlockRec	pb;

		memset(&pb, 0, sizeof(pb));
		pb.ioParam.ioRefNum = refNum;
		pb.ioParam.ioBuffer = buf;
		pb.ioParam.ioReqCount = outlen;
		pb.ioParam.ioPosMode = fsFromLEOF;
		pb.ioParam.ioPosOffset = 0;

		err = PBWriteSync(&pb);
		if( err == noErr )
		{
			if( pb.ioParam.ioActCount != outlen )
				syslog(0, "ssh2_append_host_to_file(): PBWrite only wrote %d bytes out of %d\n", pb.ioParam.ioActCount, outlen);
		}
		else
		{
			syslog(0, "ssh2_append_host_to_file(): PBWrite returned error %d\n", err);
			return false;
		}
	}

	return true;
}

static Boolean ssh2_hostkey_approved(LIBSSH2_SESSION *session, const char *hostname, in_port_t port)
{
	Boolean result = false, mismatch = false, error = false;
	OSErr err;
	LIBSSH2_KNOWNHOSTS *hosts = NULL;
	const char *hostkey_raw;
	size_t hostkey_raw_len;
	int hostkey_type;
	FSSpec known_hosts_fsp;
	short known_hosts_refNum = -1;
	int knownhost_type = LIBSSH2_KNOWNHOST_KEY_UNKNOWN;

	hosts = libssh2_knownhost_init(session);
	if (!hosts)
	{
		error = true;
		goto exit;
	}

	hostkey_raw = libssh2_session_hostkey(session, &hostkey_raw_len, &hostkey_type);

	switch (hostkey_type)
	{
		case LIBSSH2_HOSTKEY_TYPE_RSA:
			knownhost_type = LIBSSH2_KNOWNHOST_KEY_SSHRSA;
			break;
		case LIBSSH2_HOSTKEY_TYPE_DSS:
			knownhost_type = LIBSSH2_KNOWNHOST_KEY_SSHDSS;
			break;
	}

	// read lines in from known hosts file
	if( ssh2_get_known_hosts_file(&known_hosts_fsp) == true)
	{
		err = FSpOpenDF(&known_hosts_fsp, fsCurPerm, &known_hosts_refNum);
		if( err == noErr )
		{
			if( ssh2_read_known_hosts_file(known_hosts_refNum, hosts) )
			{
				// check hosts for current host
				switch (libssh2_knownhost_checkp(hosts, hostname, port, hostkey_raw, hostkey_raw_len, LIBSSH2_KNOWNHOST_TYPE_PLAIN | LIBSSH2_KNOWNHOST_KEYENC_RAW | knownhost_type, NULL))
				{
					case LIBSSH2_KNOWNHOST_CHECK_MATCH:
						result = true;
						break;
					case LIBSSH2_KNOWNHOST_CHECK_NOTFOUND:
						result = false;
						break;
					case LIBSSH2_KNOWNHOST_CHECK_MISMATCH:
						result = false;
						mismatch = true;
						break;
					case LIBSSH2_KNOWNHOST_CHECK_FAILURE:
						break;
				}
			}
			else
			{
				error = true;
				goto exit;
			}
		}
		else
		{
			error = true;
			goto exit;
		}
	}
	else
	{
		error = true;
		goto exit;
	}

	if (!result)
	{
		short socresult;
		// hostkey_hash is binary data (32 bytes for SHA256)
		const char *hostkey_hash = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA256);
		char hostkey_hash_ascii[7+(32*3)+1] = "SHA256:";
		base64_encode(32, hostkey_hash, sizeof(hostkey_hash_ascii)-7, hostkey_hash_ascii+7);

		/* SHA1
		int ret = sprintf(hostkey_hash_ascii,
		"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
		hostkey_hash[0], hostkey_hash[1], hostkey_hash[2], hostkey_hash[3], hostkey_hash[4], hostkey_hash[5],
		hostkey_hash[6], hostkey_hash[7], hostkey_hash[8], hostkey_hash[9], hostkey_hash[10], hostkey_hash[11],
		hostkey_hash[12], hostkey_hash[13], hostkey_hash[14], hostkey_hash[15], hostkey_hash[16], hostkey_hash[17],
		hostkey_hash[18], hostkey_hash[19]);
		*/

		if( mismatch )
			socresult = save_once_cancel2(hostkey_hash_ascii);
		else
			socresult = save_once_cancel1(hostkey_hash_ascii);

		switch (socresult)
		{
			case 1: // save and accept
			{
				// append known host line to file
				struct libssh2_knownhost *khost;

				libssh2_knownhost_addc(hosts, hostname, NULL, hostkey_raw, hostkey_raw_len, NULL, 0, LIBSSH2_KNOWNHOST_TYPE_PLAIN | LIBSSH2_KNOWNHOST_KEYENC_RAW | knownhost_type, &khost);

				if( ssh2_append_host_to_file(known_hosts_refNum, hosts, khost) )
					result = true;
				else
					error = true;

				// TODO: if mismatch, does previous line need to be removed from file?

				break;
			}
			case 2: // accept (but no save)
				result = true;
				break;

			case 3:	// cancel connection
				result = false;
				break;
		}
	}

exit:
	if (error)
	{
		// Each function will print more specific errors to the log window, and then this dialog is shown
		Str255 str;
		GetIndString(str, OPFAILED_MESSAGES_ID, KNOWN_HOSTS_ERR);
		p2cstr(str);
		SSH2ErrorDialog((char *)str);

		result = true;
	}

	if (hosts)
		libssh2_knownhost_free(hosts);
	if (known_hosts_refNum != -1)
		FSClose(known_hosts_refNum);

	return result;
}

static void kbd_callback(const char* name, int name_len, const char* instruction,
            int instruction_len, int num_prompts,
            const LIBSSH2_USERAUTH_KBDINT_PROMPT* prompts,
            LIBSSH2_USERAUTH_KBDINT_RESPONSE* responses, void **abstract)
{
	WindRec *w = (WindRec *)*abstract;
	if (name)
		syslog(0, "kbd_callback name %s\n", name);
	if (instruction)
		syslog(0, "kbd_callback instruction %s\n", instruction);
	syslog(0, "kbd_callback num_prompts %d\n", num_prompts);

	// If only one prompt, and a password was entered earlier, send it
	if ((num_prompts == 1) && (w->sshdata.password[0] > 0)) {
		responses[0].text = malloc(w->sshdata.password[0]);
		memcpy(responses[0].text, &(w->sshdata.password[1]), w->sshdata.password[0]);
		responses[0].length = w->sshdata.password[0];
		// TODO: set a flag to indicate if this has been sent yet
	}
	// TODO: if flag true, put up usual dialogs

/*
	if (num_prompts > 0)
	{
		responses[0].text = malloc(4);
		memcpy(responses[0].text, "test", 4);
		responses[0].length = 4;
	}
	*/
}

static Boolean ssh2_authentication_successful(LIBSSH2_SESSION *session, WindRec *w)
{
	Str255 username;
	char *userauthlist;
	memcpy(username, &(w->sshdata.login[1]), w->sshdata.login[0]);
	username[w->sshdata.login[0]] = '\0';

	userauthlist = libssh2_userauth_list(session, username,  w->sshdata.login[0]);
	if (userauthlist == NULL) {
		if (libssh2_userauth_authenticated(session)) {
			syslog(0, "\'none\' userauth succeeded\n");
			return true;
		} else {
			syslog(0, "Failed to retrieve authentication methods\n");
			return false;
		}
	} else {
		syslog( 0, "Authentication methods: %s\n", userauthlist);
	}


	// TODO: public key
	if (strstr(userauthlist, "keyboard-interactive") != NULL) {
		if (libssh2_userauth_keyboard_interactive_ex(session, &(w->sshdata.login[1]), w->sshdata.login[0], kbd_callback) == 0)
			return true;
	}
	if (strstr(userauthlist, "password") != NULL) {
		//SSH2LoginDialog(theScreen->sshdata.host, theScreen->sshdata.login, theScreen->sshdata.password);
		// TODO: add pwd change callback
		if (libssh2_userauth_password_ex(session, &(w->sshdata.login[1]), w->sshdata.login[0], &(w->sshdata.password[1]), w->sshdata.password[0], NULL) == 0)
			return true;
		// TODO: allow re-entering password on LIBSSH2_ERROR_AUTHENTICATION_FAILED
	}
	syslog( 0, "No supported authentication method found\n");
	return false;
}

/*
 * ssh2_thread
 */

void *ssh2_thread(WindRec*w)
{
	OSErr			err;
	lshcontext		*context;
	short			port;
	int				i;
	int				result;
	int				listener;

	port = w->port;

	{
		char hostname[256];
		int sock;
		int rc = libssh2_init(0);
		syslog(0, "libssh2 init %d\n", rc);
		sock = socket(AF_INET, SOCK_STREAM, 0);
		fcntl(sock, F_SETFL, 0);

		{
			struct sockaddr_in sin;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(w->portNum);
			{
				struct hostent *hp;

				memcpy(hostname, &(w->sshdata.host[1]), w->sshdata.host[0]);
				hostname[w->sshdata.host[0]] = '\0';
				syslog(0, "host %s\n", hostname);
				hp = gethostbyname(hostname);
				if (hp) {
					memcpy(&(sin.sin_addr.s_addr), hp->h_addr_list[0], hp->h_length);
					syslog(0, "h_name %s\n", hp->h_name);
				}
				else {
					// TODO: gethostbyname error
				}
			}
			syslog(0, "port %d portNum %d\n", port, w->portNum);
			rc = connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in));
			if (rc != 0) {
				syslog(0, "failed to connect!\n");
			}
		}
		{
			LIBSSH2_SESSION *session = libssh2_session_init_ex(NULL, NULL, NULL, w);
			//libssh2_trace(session, INT_MAX);
			libssh2_trace_sethandler(session, NULL, libssh2_handler);
			if (libssh2_session_startup(session, sock)) {
				syslog(0, "Failure establishing SSH session\n");
			}

			if (!ssh2_hostkey_approved(session, hostname, w->portNum))
				goto closesession;

			if (!ssh2_authentication_successful(session, w))
				goto closesession;

			{
				LIBSSH2_CHANNEL *channel = libssh2_channel_open_session(session);
				libssh2_channel_request_pty_ex(channel, "vt100", 5, NULL, 0,
					VSgetcols( w->vs ) + 1, VSgetlines( w->vs ),
					RSlocal[w->vs].fwidth * (VSgetcols( w->vs ) + 1),
					RSlocal[w->vs].fheight * (VSgetlines( w->vs )));
				libssh2_channel_shell(channel);

				{
					context = (lshcontext *)NewPtr(sizeof(lshcontext));
					if (context == NULL) {
						syslog( 0, "### ssh2_thread, NewPtr lshcontext failed\n" );
						goto done;
					}

					if (pthread_setspecific(ssh2threadkey, context)) {
						syslog( 0, "### ssh2_thread, pthread_setspecific failed\n" );
						goto done;
					}
					w->sshdata.context = context;

					listener = -1;
					init_context(context, port);

					if ( listener != -1 ) {
						context->_listener = listener;
					}
				}

				w->sshdata.channel = channel;
				libssh2_session_set_blocking(session, 0);

				{
					int stdinfd;
					char c;

					stdinfd = open("dev:ttyin", O_RDONLY);

					syslog( 0, "stdin %d\n", stdinfd);

					{
						char buf[64];
						ssize_t bytes;

						while ((bytes = libssh2_channel_read(channel, buf, sizeof(buf))) > 0)
						{
							syslog(0, "read %d bytes from channel\n", bytes);

							if (bytes > 0)
								WriteCharsToTTY(1, NULL, buf, bytes);
						}
					}

					while (1) {
						int directions, maxfd = 0;
						fd_set readfds, writefds, exceptfds;
						FD_ZERO(&readfds);
						FD_ZERO(&writefds);
						FD_ZERO(&exceptfds);

						directions = libssh2_session_block_directions(session);

						if (directions & LIBSSH2_SESSION_BLOCK_INBOUND)
						{
							FD_SET(sock, &readfds);
							if (sock > maxfd)
								maxfd = sock;
						}
						if (directions & LIBSSH2_SESSION_BLOCK_OUTBOUND)
						{
							//FD_SET(sock, &writefds);
							//if (sock > maxfd)
							//	maxfd = sock;
						}
						FD_SET(sock, &exceptfds);
						if (sock > maxfd)
							maxfd = sock;

						FD_SET(stdinfd, &readfds);
						if (stdinfd > maxfd)
							maxfd = stdinfd;

						//FD_SET(stdoutfd, &writefds);
						//if (stdinfd > maxfd)
						//	maxfd = stdinfd;

						rc = select(maxfd+1, &readfds, &writefds, &exceptfds, NULL);

						if (rc == -1) {
							syslog(0, "select exited with errno %d\n", errno);
							break;
						}
						else {
							syslog(0, "select returned %d\n", rc);
						}

						if (FD_ISSET(stdinfd, &readfds)) {
							char buf[64];
							int bytes;

							bytes = read(stdinfd, buf, sizeof(buf));
							syslog(0, "read %d bytes from stdin\n", bytes);

							if (bytes > 0)
								libssh2_channel_write(channel, buf, bytes);
							else if (bytes == 0)
								break;	// EOF: window was closed. Close channel/session/connection.
							else {
								syslog(0, "read() from stdin returned -1 errno %d\n", errno);
								break;
							}
						}
						//if (FD_ISSET(stdoutfd, &writefds))
						//{
						//}
						if (FD_ISSET(sock, &readfds)) {
							char buf[256];

							while (1) {
								ssize_t bytes = libssh2_channel_read(channel, buf, sizeof(buf));

								syslog(0, "libssh2_channel_read() returned %d\n", bytes);
								if (bytes > 0)
									WriteCharsToTTY(1, NULL, buf, bytes);
								else
									break;
							}

							if (libssh2_channel_eof(channel))
							{
								syslog(0, "libssh2_channel_eof() true\n");
								break;
							}
						}
						//if (FD_ISSET(sock, &writefds))
						//{
						//}
						if (FD_ISSET(sock, &exceptfds)) {
							syslog(0, "select: sock in exceptfds\n", errno);
							break;
						}
					}

					close(stdinfd);
				}

				libssh2_channel_free(channel);
			}

closesession:
			libssh2_session_disconnect(session, "Normal Shutdown");
			libssh2_session_free(session);
		}

closesocket:
		close(sock);
		libssh2_exit();
	}

done:
	/* disconnect indication */
	if ( port != -1 ) {
		netputevent(CONCLASS, CONCLOSE, port, 0);
	}

	/* window's ptr might have changed... better reload it */
	w = ssh2_window();
	if ( w ) {
		if ( w->sshdata.thread == pthread_self() ) {
			w->sshdata.thread = NULL;
			w->sshdata.context = NULL;
		}
	}

	if ( context ) {
		if ( context->_listener != -1 ) {
			listener = context->_listener;
			context->_listener = -1;
			close( listener );
		}
		context->_self = NULL;
		DisposePtr((Ptr)context);
	}
	return NULL;

#if 0
	context = (lshcontext *)NewPtr(sizeof(lshcontext));
	if (context == NULL) {
		syslog( 0, "### ssh2_thread, NewPtr lshcontext failed\n" );
		goto done;
	}

	if (pthread_setspecific(ssh2threadkey, context)) {
		syslog( 0, "### ssh2_thread, pthread_setspecific failed\n" );
		goto done;
	}
	w->sshdata.context = context;

	listener = -1;

	context->_pexitbuf = &exitbuf;
	if (!setjmp(exitbuf)) {

		do {
			init_context(context, port);

			if ( listener != -1 ) {
				context->_listener = listener;
			}

			err = MPInit(65564, &context->_gMemPool, NULL);
			if (err != noErr) {
				syslog( 0, "### ssh2_thread, MPInit failed\n" );
				goto done;
			}

			make_env( context, w );

			if ( build_cmdline(w, argstr) )
				goto done;

			listener = context->_listener;

			make_args( argstr, tabargv, &argc, &argv );

			/* we need to intercept SIGINT to fake 'exit' */
			memset(&interrupt, 0, sizeof(interrupt));
			interrupt.sa_handler = ssh2_sighandler;
			sigemptyset(&interrupt.sa_mask);
			interrupt.sa_flags = 0;
			if (sigaction(SIGINT, &interrupt, NULL) < 0) {
				werror("warning: failed to install ssh2_sighandler for SIGINT\n");
			}



		 	set_error_syslog(applname);
			result = appl_main(argc, argv);

			/* wait for empty output buffer */
			while (context->_gConsoleOutBufLen) {
				ssh2_sched();
			}

			close_all_files(context);

			if ( context->_gMemPool ) {
				mempool = context->_gMemPool;
				context->_gMemPool = NULL;
				MPDispose(mempool);
			}

			/* window's ptr might have changed... better reload it */
			w = ssh2_window();

		} while ( result == 0 && w && (w->ssh2method == 3 || w->autoreconnect != 0) );

	}

done:

	/* disconnect indication */
	if ( port != -1 ) {
		netputevent(CONCLASS, CONCLOSE, port, 0);
	}

	/* window's ptr might have changed... better reload it */
	w = ssh2_window();
	if ( w ) {
		if ( w->sshdata.thread == pthread_self() ) {
			w->sshdata.thread = NULL;
			w->sshdata.context = NULL;
		}
	}

	if ( context ) {
		if ( context->_listener != -1 ) {
			listener = context->_listener;
			context->_listener = -1;
			close( listener );
		}
		close_all_files(context);
		if ( context->_gMemPool ) {
			mempool = context->_gMemPool;
			context->_gMemPool = NULL;
			MPDispose(mempool);
		}
		context->_self = NULL;
		DisposePtr((Ptr)context);
	}

	return NULL;
#endif
}


/*
 * ssh_protocol_initial
 */

void ssh_protocol_initial(WindRec*w)
{
	pthread_attr_t		attr = NULL;
	static Boolean		sOTTested = false;
	static Boolean		sOTPresent = false;
	static Boolean		sThreadsTested = false;
	static Boolean		sThreadsPresent = false;
	Str255			scratchPstring;


	if (w->sshdata.thread != NULL) {
		syslog( 0, "### ssh_protocol_initial, ssh2thread already exists\n" );
		goto done;
	}

#if 0
	/* we need at least OpenTransport 1.1.1 */
	if ( !sOTTested ) {
		long result;
		NumVersion version;
		if ( Gestalt(gestaltOpenTpt, &result) == noErr ) {
			OSErr theErr = Gestalt(gestaltOpenTptVersions, (long*)&version);
			if (theErr == noErr && (version.majorRev >= 2 || version.minorAndBugRev >= 0x11)) {
				sOTPresent = true;
			}
		}
		sOTTested = true;
	}
	if ( !sOTPresent ) {
		GetIndString(scratchPstring, OPFAILED_MESSAGES_ID, NO_OPENTPT_ERR);
		p2cstr(scratchPstring);
		SSH2ErrorDialog((char *)scratchPstring);
		goto done;
	}
#endif

	/* we need the Thread Manager */
	if ( !sThreadsTested ) {
		long mask;
		long result;
		mask	 = (1 << gestaltThreadMgrPresent);
		// is the Thread Manager running ?
		if ( Gestalt(gestaltThreadMgrAttr, &result) == noErr
		  && (result & mask) == mask ) {
			sThreadsPresent = true;
		}
	}
	if ( !sThreadsPresent ) {
		GetIndString(scratchPstring, OPFAILED_MESSAGES_ID, NO_THREADS_ERR);
		p2cstr(scratchPstring);
		SSH2ErrorDialog((char *)scratchPstring);
		goto done;
	}


	/* we need at least 1MB free in the process manager heap */
	if ( TempFreeMem() < 1024*1024 ) {
		GetIndString(scratchPstring, OPFAILED_MESSAGES_ID, OUT_OF_MEMORY);
		p2cstr(scratchPstring);
		SSH2ErrorDialog((char *)scratchPstring);
		goto done;
	}


	ssh2_init();

	if ( ssh2threadkey == NULL ) {
		if (pthread_key_create(&ssh2threadkey, KeyDestructor)) {
			syslog( 0, "### ssh2_thread, pthread_key_create failed\n" );
			goto done;
		}
	}
	if (pthread_attr_init(&attr)) {
		syslog( 0, "### ssh_protocol_initial, pthread_attr_init failed\n" );
		goto done;
	}
	/* important ! ugly crash with default stack size. */
	if (pthread_attr_setstacksize(&attr, 65536)) {
		syslog( 0, "### ssh_protocol_initial, pthread_attr_setstacksize failed\n" );
		goto done;
	}
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
		syslog( 0, "### ssh_protocol_initial, pthread_attr_setdetachstate failed\n" );
		goto done;
	}
	if (pthread_create(&w->sshdata.thread, &attr, (GUSIPThreadProc)ssh2_thread, w)) {
		syslog( 0, "### ssh_protocol_initial, pthread_create failed\n" );
		goto done;
	}
	if ( attr != NULL ) {
		pthread_attr_destroy(&attr);
	}
	return;
done:
	netputevent(CONCLASS, CONCLOSE, w->port, 0);
	if ( attr != NULL ) {
		pthread_attr_destroy(&attr);
	}
}


/*
 * ssh_packet_read
 */

void ssh_packet_read(struct WindRec*w, unsigned char*databuf, short datalen)
{
	syslog( 0, "### ssh_packet_read, len : %d\n", datalen );
}


/*
 * ssh_protocol_write
 */

void ssh_protocol_write(struct WindRec*w, unsigned char*databuf, short datalen)
{
	syslog( 0, "### ssh_protocol_write, len : %d\n", datalen );
}


/*
 * ssh_protocol_close
 */

void ssh_protocol_close(WindRec*w)
{
	if (w->sshdata.thread) {
		lshcontext *context = (lshcontext *)w->sshdata.context;
		context->_gConsoleInEOF = true;
		pthread_kill( w->sshdata.thread, SIGINT );
		w->sshdata.thread = NULL;
		ssh2_sched();
	}
}

#pragma mark -

/*
 * ssh_randomize
 */

void ssh_randomize(void)
{
	// create an SSH keypair--not implemented and probably never will be
}

#pragma mark -

/*
 * ssh_exportkey
 */

void ssh_exportkey(void)
{
	// export an SSH keypair--not implemented and probably never will be
}

#pragma mark -

/*
 * ssh_wresize
 */

void ssh_wresize(struct WindRec* w)
{
	if (w->sshdata.thread) {
		LIBSSH2_CHANNEL *channel = (LIBSSH2_CHANNEL *)w->sshdata.channel;
		syslog(0, "width %d height %d pixels %d %d\n", VSgetcols( w->vs ) + 1, VSgetlines( w->vs ),
			RSlocal[w->vs].fwidth * (VSgetcols( w->vs ) + 1), RSlocal[w->vs].fheight * (VSgetlines( w->vs )));
		libssh2_channel_request_pty_size_ex(channel, VSgetcols( w->vs ) + 1, VSgetlines( w->vs ),
			RSlocal[w->vs].fwidth * (VSgetcols( w->vs ) + 1), RSlocal[w->vs].fheight * (VSgetlines( w->vs )));
	}
}

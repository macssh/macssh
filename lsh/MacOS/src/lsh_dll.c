/*
 * lsh_dll.c
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
#include "lsh_context.h"

#include "format.h"
#include "io.h"
#include "tty.h"
#include "xalloc.h"
#include "werror.h"

#include <assert.h>
/*#include <console.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SIOUX.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdarg.h>
#include <setjmp.h>
#include <termios.h>


#include <CodeFragments.h>
#include <TextUtils.h>



#include "MemPool.h"


extern pascal OSErr __initialize(CFragConnectionID connID);
extern pascal void __terminate(void);

pascal OSErr __lsh_initialize(const CFragInitBlock * theInitBlock);
pascal void __lsh_terminate(void);

FSSpec gDLLFileSpec;

Boolean gLogStdIO = 0;

extern int appl_main(int argc, char **argv);
extern char *applname;
extern char *defargstr;

extern void ssh2_init();
extern void ssh2_terminate();
extern void ssh2_sched();
extern void ssh2_doevent(EventRecord *theEvent, long sleepTime);

extern void close_all_files();

extern char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *parID);

extern const struct exception *write_raw(int fd, UINT32 length, const UINT8 *data);

typedef void(*hdlevtfunc)( long userData, EventRecord *userEvent, long sleepTime);
typedef void(*logfunc)( long userData, const char *message);
typedef char *(*getpassfunc)( long userData, const char *prompt);
typedef int (*yesornofunc)( long userData, const char *prompt, int def);

typedef struct lshctx {
	pthread_t		pthread;
	lshcontext		*context;
	char			*argstr;
	unsigned long	flags;
	hdlevtfunc		hdlevt;
	getpassfunc		getpass;
	logfunc			log;
	yesornofunc		yes_or_no;
	long			userData;
} lshctx;

enum {
	kLSHConvertLFs = 0x1,
	kLSHStripCRs   = 0x2
};

lshctx *lsh_new(char *argstr, hdlevtfunc hdlevt, logfunc log, getpassfunc getpass,
				yesornofunc yes_or_no, unsigned long flags, long userData);
void lsh_delete(lshctx *ctx);
void lsh_yield();
int lsh_read(lshctx *ctx, void *buffer, long inbytes);
long lsh_write(lshctx *ctx, const void *buffer, long inbytes);
char *lsh_getprefsd();
lshctx *lsh_current();
Boolean lsh_running(lshctx *ctx);


const struct termios defaulttermios = {
	0x00000300,		/* c_iflag;               input mode flags */
	0x00000003,		/* c_oflag;               output mode flags */
	0x00000b0d,		/* c_cflag;               control mode flags */
	0x000005cf,		/* c_lflag;               local mode flags */
	0,			/* c_line;                line discipline (== c_cc[19]) */
	{
	  0x03,0x1c,0x7f,0x15,0x04,0x01,0x00,0x00,
	  0x00,0x00,0x17,0x12,0x1a,0x11,0x13,0x16,
	  0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},			/* c_cc[NCCS];            control characters */
	0,			/* c_ispeed;              input speed */
	0			/* c_ospeed;              output speed */
};

static char *envp[] = {
	"HOME",
	"LOGNAME",
	"TERM"
};

static char *envv[] = {
	"",
	"",
	"vt220"
};

char homepath[256];
char username[256];


pthread_key_t ssh2threadkey = NULL;


/*
 * getenv
 */

char *getenv(const char *var)
{
	int i;
	for (i = 0; i < sizeof(envp) / sizeof(char *); i++) {
		if (strcmp(var, envp[i]) == 0) {
			return envv[i];
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
	lshctx *ctx = lsh_current();
	if ( ctx && ctx->log ) {
		va_list args;
		char string_buf[BUF_SIZE];
		int len;

		va_start(args, format);
		len = vsnprintf(string_buf, BUF_SIZE - 2, format, args);
		assert(len < BUF_SIZE - 1);
		va_end(args);
		if ( len > 0 ) {
/*
#if TRACE_IT
	    // add CR if string is not finished
	    if (string_buf[len - 1] != '\r' && string_buf[len - 1] != '\n') {
	      string_buf[len++] = '\r';
	    }
	    plnt_putbuf(string_buf, len, 'TEXT', NULL, 0);
#else
	    string_buf[len++] = 0;
	    printf("%s", string_buf);
#endif
*/
			(*ctx->log)( ctx->userData, string_buf );
		}
	}
}

/*
 * strerror
 */

char * strerror(int errnum)
{
	static char errstr[64];
	
	return(__strerror(errnum, errstr));
}

/*
 * __strerror
 */

char * __strerror(int errnum, char * str)
{
	switch (errnum)
	{
											//123456789_123456789_123456789_123456789_123456789_
		case EPERM:             strcpy(str,  "Operation not permitted");			              break;
		case ENOENT:            strcpy(str,  "No such file or directory");			              break;
		case ESRCH:             strcpy(str,  "No such process");			                      break;
		case EINTR:             strcpy(str,  "Interrupted system call");			              break;
		case EIO:               strcpy(str,  "Input/output error");			                      break;
		case ENXIO:             strcpy(str,  "Device not configured");			                  break;
		case E2BIG:             strcpy(str,  "Argument list too long");			                  break;
		case ENOEXEC:           strcpy(str,  "Exec format error");			                      break;
		case EBADF:             strcpy(str,  "Bad file descriptor");			                  break;
		case ECHILD:            strcpy(str,  "No child processes");			                      break;
		case EDEADLK:           strcpy(str,  "Resource deadlock avoided");			              break;
					/* 11 was EAGAIN */
		case ENOMEM:            strcpy(str,  "Cannot allocate memory");			                  break;
		case EACCES:            strcpy(str,  "Permission denied");			                      break;
		case EFAULT:            strcpy(str,  "Bad address");                                      break;
		case ECANCELED:         strcpy(str,  "Operation cancelled");                              break;
		case EBUSY:             strcpy(str,  "Device busy");                                      break;
		case EEXIST:            strcpy(str,  "File exists");                                      break;
		case EXDEV:             strcpy(str,  "Cross-device link");                                break;
		case ENODEV:            strcpy(str,  "Operation not supported by device");			      break;
		case ENOTDIR:           strcpy(str,  "Not a directory");                                  break;
		case EISDIR:            strcpy(str,  "Is a directory");                                   break;
		case EINVAL:            strcpy(str,  "Invalid argument");                                 break;
		case ENFILE:            strcpy(str,  "Too many open files in system");                    break;
		case EMFILE:            strcpy(str,  "Too many open files");                              break;
		case ENOTTY:            strcpy(str,  "Inappropriate ioctl for device");                   break;
		case EFBIG:             strcpy(str,  "File too large");                                   break;
		case ENOSPC:            strcpy(str,  "No space left on device");                          break;
		case ESPIPE:            strcpy(str,  "Illegal seek");                                     break;
		case EROFS:             strcpy(str,  "Read-only file system");                            break;
		case EMLINK:            strcpy(str,  "Too many links");                                   break;
		case EPIPE:             strcpy(str,  "Broken pipe");                                      break;

/* math software */
		case EDOM:              strcpy(str,  "Numerical argument out of domain");                 break;
		case ERANGE:            strcpy(str,  "Result too large");                                 break;

/* non-blocking and interrupt i/o */
		case EAGAIN:            strcpy(str,  "Resource temporarily unavailable");                 break;
		/*case EWOULDBLOCK:       strcpy(str,  "Operation would block");                            break;*/
		case EINPROGRESS:       strcpy(str,  "Operation now in progress");                        break;
		case EALREADY:          strcpy(str,  "Operation already in progress");                    break;

/* ipc/network software -- argument errors */
		case ENOTSOCK:          strcpy(str,  "Socket operation on non-socket");                   break;
		case EDESTADDRREQ:      strcpy(str,  "Destination address required");                     break;
		case EMSGSIZE:          strcpy(str,  "Message too long");                                 break;
		case EPROTOTYPE:        strcpy(str,  "Protocol wrong type for socket");                   break;
		case ENOPROTOOPT:       strcpy(str,  "Protocol not available");                           break;
		case EPROTONOSUPPORT:   strcpy(str,  "Protocol not supported");                           break;
		case ESOCKTNOSUPPORT:   strcpy(str,  "Socket type not supported");                        break;
		case EOPNOTSUPP:        strcpy(str,  "Operation not supported");                          break;
		case EPFNOSUPPORT:      strcpy(str,  "Protocol family not supported");                    break;
		case EAFNOSUPPORT:      strcpy(str,  "Address family not supported by protocol family");  break;
		case EADDRINUSE:        strcpy(str,  "Address already in use");                           break;
		case EADDRNOTAVAIL:     strcpy(str,  "Can't assign requested address");                   break;

/* ipc/network software -- operational errors */
		case ENETDOWN:          strcpy(str,  "Network is down");                                  break;
		case ENETUNREACH:       strcpy(str,  "Network is unreachable");                           break;
		case ENETRESET:         strcpy(str,  "Network dropped connection on reset");              break;
		case ECONNABORTED:      strcpy(str,  "Software caused connection abort");                 break;
		case ECONNRESET:        strcpy(str,  "Connection reset by peer");                         break;
		case ENOBUFS:           strcpy(str,  "No buffer space available");                        break;
		case EISCONN:           strcpy(str,  "Socket is already connected");                      break;
		case ENOTCONN:          strcpy(str,  "Socket is not connected");                          break;
		case ESHUTDOWN:         strcpy(str,  "Can't send after socket shutdown");                 break;
		case ETOOMANYREFS:      strcpy(str,  "Too many references: can't splice");                break;
		case ETIMEDOUT:         strcpy(str,  "Operation timed out");                              break;
		case ECONNREFUSED:      strcpy(str,  "Connection refused");                               break;

		case ELOOP:             strcpy(str,  "Too many levels of symbolic links");                break;
		case ENAMETOOLONG:      strcpy(str,  "File name too long");                               break;

/* should be rearranged */
		case EHOSTDOWN:         strcpy(str,  "Host is down");                                     break;
		case EHOSTUNREACH:      strcpy(str,  "No route to host");                                 break;
		case ENOTEMPTY:         strcpy(str,  "Directory not empty");                              break;

		case ENOLCK:            strcpy(str,  "No locks available");                               break;
		case ENOSYS:            strcpy(str,  "Function not implemented");                         break;

#ifndef _POSIX_SOURCE
		/*case ELAST:             strcpy(str,  "Must be equal largest errno");                      break;*/
#endif /* _POSIX_SOURCE */
		default:                sprintf(str, "Unknown Error (%d)", errnum);                       break;
	}

	return(str);
}

/*
 * __assertion_failed
 */
void __assertion_failed(char const *condition, char const *filename, int lineno)
{
	fprintf(stderr, "Assertion (%s) failed in \"%s\" on line %d\n", condition, filename, lineno);
	if ( pthread_getspecific(ssh2threadkey) ) {
		abort();
	} else {
		Debugger();
		#undef abort
		abort();
		#define abort macosabort
	}
}

/*
 * strsignal
 */

char * strsignal(int signo)
{
	static char errstr[__max_errstr];
	
	sprintf(errstr, "Signal no (%d)", signo);
	return 0L;
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

void exit(int result UNUSED)
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
/*
	extern int	__aborting;

	raise(SIGABRT);
	
	__aborting = 1;
*/
	exit(EXIT_FAILURE);
}

/*
 * random
 */

long random()
{
	return rand();
}


#pragma mark -

#undef malloc
#undef calloc
#undef realloc
#undef free
extern void *malloc(unsigned long size);
extern void *calloc(unsigned long items, unsigned long size);
extern void *realloc(void *addr, unsigned long size);
extern void free(void *addr);

/*
 * lshmalloc
 */
void *lshmalloc(unsigned long size)
{
	lshcontext *context = pthread_getspecific(ssh2threadkey);
	if ( context && context->_gMemPool ) {
		return MPmalloc(context->_gMemPool, size);
	} else {
		return malloc(size);
	}
}

/*
 * lshcalloc
 */
void *lshcalloc(unsigned long items, unsigned long size)
{
	lshcontext *context = pthread_getspecific(ssh2threadkey);
	if ( context && context->_gMemPool ) {
		size_t		tsize;
		void		*p;
		tsize = items * size;
		p = MPmalloc( context->_gMemPool, tsize );
		if ( p ) {
			memset(p, '\0', tsize);
		}
	} else {
		return calloc(items, size);
	}
	return NULL;
}

/*
 * lshrealloc
 */
void *lshrealloc(void *addr, unsigned long size)
{
	lshcontext *context = pthread_getspecific(ssh2threadkey);
	if ( context && context->_gMemPool ) {
		size_t orig_size;
		void* p;
		if (addr == 0)
			return MPmalloc(context->_gMemPool, size);
		orig_size = MPsize(context->_gMemPool, addr);
		p = MPmalloc(context->_gMemPool, size);
		orig_size = orig_size < size ? orig_size : size;
		memcpy(p, addr, orig_size);
		MPfree(context->_gMemPool, addr);
		return p;
	} else {
		return realloc(addr, size);
	}
	return NULL;
}

/*
 * lshfree
 */
void lshfree(void *addr)
{
	lshcontext *context = pthread_getspecific(ssh2threadkey);
	if ( context && context->_gMemPool ) {
		MPfree(context->_gMemPool, addr);
	} else {
		free(addr);
	}
}


#pragma mark -
/*
 * we need to track open()/close()/socket() calls to close files/sockets
 * upon abort/exit
 */

/*
 * add_one_file
 */

void add_one_file(struct lshcontext *context, int fd)
{
	int i;
	
	/*
	 * GUSI problem : the sockets when closed are instead pushed in a
	 * queue for later removal (GUSIProcess::Instance()->QueueForClose).
	 * However a linger value tends to correct this behavior and wait for
	 * the socket to die for a maximum of 2 seconds) (alexp)
	 */
    {
    	struct linger ling = {1, 2};
		setsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
	}

	if ( fd != -1 ) {
		for (i = 0; i < MAXFILESCOUNT; i++) {
			if (context->_filesTable[i] == -1) {
				context->_filesTable[i] = fd;
				break;
			}
		}
	}
}

/*
 * remove_one_file
 */

void remove_one_file(struct lshcontext *context, int fd)
{
	int i;

	if ( fd != -1 ) {
		for (i = 0; i < MAXFILESCOUNT; i++) {
			if (context->_filesTable[i] == fd) {
				context->_filesTable[i] = -1;
				break;
			}
		}
	}
}

/*
 * close_all_files
 */

void close_all_files(lshcontext *context)
{
	int i;

	for (i = 0; i < MAXFILESCOUNT; i++) {
		if (context->_filesTable[i] != -1) {
			close(context->_filesTable[i]);
		}
	}
}


#pragma mark -

/*
 * my PLstrcpy
 */

pascal StringPtr PLstrcpy(StringPtr d, ConstStr255Param s)
{
	int			size;

	size = s[0] + 1;
	while (size--) {
		d[size] = s[size];
	}
	return d;
}


/*
 * my PLstrrchr in CWPro6 (last MPTP release) crashes in 68k ???
 */

pascal Ptr PLstrrchr(ConstStr255Param s, short c)
{
	int			size;

	size = s[0];
	while ( size > 0 ) {
		if ( s[size] == (unsigned char)c ) {
			return (char *)s + size;
		}
		size--;
	}
	return NULL;
}


#pragma mark -

/*
 * InstallTTY
 */
int InstallTTY(int id, void *ctx, void *sock)
{
	if ( id == 0 ) {
		lshcontext *context = ctx;
		context->_insock = sock;
	}
	return 0;
}

/*
 * RemoveTTY
 */
void RemoveTTY(int id, void *ctx, void *sock)
{
	if ( id == 0 ) {
		lshcontext *context = ctx;
		context->_insock = NULL;
	}
}

/*
 * WriteCharsToTTY
 */
int WriteCharsToTTY(int id, void *ctx, char *buffer, int n)
{
	long			written = 0;
	lshcontext		*context = (lshcontext *)ctx;
	char			*buf = buffer;
	char			c;

	ssh2_doevent(NULL, 0L);
	
	if ( id == 2 ) {
		// log stderr to console
		syslog( 0, "%.*s", n, buffer );
		/*putlln( buffer, n );*/
		return n;							
	}

	if ( !context ) {
		return 0;
	}

	if ( context->_gConsoleOutBufMax ) {
		if ( context->_socket == -1 ) {
			while (n > 0) {
				if ( context->_gConsoleOutBufLen < context->_gConsoleOutBufMax ) {
					long len = n;
					if ( len > context->_gConsoleOutBufMax - context->_gConsoleOutBufLen ) {
						len = context->_gConsoleOutBufMax - context->_gConsoleOutBufLen;
					}
		        	if ( context->_stripCRs ) {
						long inlen = 0;
						long outlen = context->_gConsoleOutBufLen;
						while (inlen < len && outlen < context->_gConsoleOutBufMax - 1) {
							c = buf[inlen++];
							if ( c != 0x0d )
								context->_gConsoleOutBuf[outlen++] = c;
						}
						context->_gConsoleOutBufLen = outlen;
						buf += inlen;
						written += inlen;
						n -= inlen;
		        	} else if ( context->_convertLFs ) {
						long inlen = 0;
						long outlen = context->_gConsoleOutBufLen;
/*
						while (inlen < len && outlen < context->_gConsoleOutBufMax - 1) {
							c = buf[inlen++];
							if ( c == 0x0a ) {
								context->_gConsoleOutBuf[outlen++] = 0x0d;
							}
							context->_gConsoleOutBuf[outlen++] = c;
						}
*/
						while (inlen < len && outlen < context->_gConsoleOutBufMax - 1) {
							c = buf[inlen++];
							if ( context->_lastCR ) {
								if ( c != 0x0a && c != 0x1b )
									context->_gConsoleOutBuf[outlen++] = 0x0a;
							} else {
								if ( c == 0x0a )
									context->_gConsoleOutBuf[outlen++] = 0x0d;
							}
							context->_gConsoleOutBuf[outlen++] = c;
							context->_lastCR = (c == 0x0d);
						}

						context->_gConsoleOutBufLen = outlen;
						buf += inlen;
						written += inlen;
						n -= inlen;
		        	} else {
						BlockMoveData( buf, context->_gConsoleOutBuf + context->_gConsoleOutBufLen, len);
						/*context->_lastCR = (buf[len-1] == 0x0d);*/
						context->_gConsoleOutBufLen += len;
						buf += len;
						written += len;
						n -= len;
					}
				}
				ssh2_sched();
			}
		}
	} else {
		written = n;
	}
/*
	syslog( 0, "write\n");
	dumpln(0, 0, buffer, written);
*/
	return written;
}

/*
 * ReadCharsFromTTY
 */
int ReadCharsFromTTY(int id, void *ctx, char *buffer, int n)
{
#pragma unused (id)
	long			len = 0;
	lshcontext		*context = (lshcontext *)ctx;

	ssh2_doevent(NULL, 0L);

	if ( !context ) {
		return 0;
	}

	if ( context->_gConsoleInBufMax ) {
		while (!len && n > 0) {
			if (context->_gConsoleInEOF) {
				buffer[0] = EOF;
				return 0;
			}
			if (context->_gConsoleInBufLen && context->_socket == -1 ) {
				len = context->_gConsoleInBufLen;
				if ( len > n ) {
					len = n;
				}
				BlockMoveData( context->_gConsoleInBuf, buffer, len );
				context->_gConsoleInBufLen -= len;
				if ( context->_gConsoleInBufLen ) {
					BlockMoveData( context->_gConsoleInBuf + len, context->_gConsoleInBuf, context->_gConsoleInBufLen );
				}
			}
			ssh2_sched();
		}
	} else {
		buffer[0] = EOF;
	}
/*
	syslog( 0, "read\n");
	dumpln(0, 0, buffer, len);
*/
	return len;
}

/*
 * AvailableFromTTY
 */
int AvailableFromTTY(int id, void *ctx)
{
#pragma unused (id)
	lshcontext *context = (lshcontext *)ctx;

	ssh2_doevent(NULL, 0L);

	if ( !context ) {
		return 0;
	}
	return context->_gConsoleInBufLen || context->_gConsoleInEOF;
}

#pragma mark -

/*
 * tty_getwinsize : replaces tty_getwinsize from liblsh
 */
int
tty_getwinsize(int fd, struct terminal_dimensions *dims)
{
  dims->char_width = 80;
  dims->char_height = 24;
  dims->pixel_width = 0;
  dims->pixel_height = 0;
  return 1;
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



/*
 * getpass
 */

char *getpass( const char *prompt )
{
	lshctx *ctx = lsh_current();
	if ( ctx && ctx->getpass ) {
		return (*ctx->getpass)( ctx->userData, prompt );
	}
	return NULL;
}


/*
 * yes_or_no
 */

int yes_or_no(struct lsh_string *s, int def, int free)
{
	lshctx *ctx = lsh_current();
	if ( ctx && ctx->yes_or_no ) {
		const char *prompt = lsh_get_cstring(s);
		if ( prompt ) {
			def = (*ctx->yes_or_no)( ctx->userData, prompt, def );
		}
	}
	if ( free ) {
		lsh_string_free(s);
	}
	return def;
}


/*
 * ssh2_doevent
 */

void ssh2_doevent(EventRecord *theEvent, long sleepTime)
{
	lshctx *ctx = lsh_current();
	if ( ctx && ctx->hdlevt ) {
		(*ctx->hdlevt)( ctx->userData, theEvent, sleepTime );
	}
}


/*
 * SIOUXHandleOneEvent
 */
/* moved to lsh_evt.c
short SIOUXHandleOneEvent(EventRecord *userEvent)
{
	lshctx *ctx = lsh_current();
	if ( ctx && ctx->hdlevt ) {
		(*ctx->hdlevt)( ctx->userData, userEvent, 0L );
		return 1;
	}
	return 0;
}
*/

#pragma mark -


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

/*
 * make_env
 */

void make_env( lshcontext *context )
{
	StringHandle	hstr;
	int				i;

  	context->_envv[0] = lsh_getprefsd();	/* set home pathname */

	/*hstr = GetString( -16413 );*/ /* get computer name */
	hstr = GetString( -16096 ); /* get user name */
	if ( hstr && *hstr ) {
    	i = **hstr;
    	BlockMoveData( *hstr + 1, username, i );
    	username[i] = 0;
    	ReleaseResource( (Handle)hstr );
  		context->_envv[1] = username;	/* set user name */
  	} else {
  		context->_envv[1] = NULL;
  	}

	/* set TERM variable */
  	context->_envv[2] = "vt220";
}




#pragma mark -

/*
 * lsh_key_del
 */

void lsh_key_del(void *)
{
}

/*
 * lsh_sighandler
 */

void lsh_sighandler(int sig)
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
 * lsh_thread
 */

void *lsh_thread(lshctx *ctx)
{
	lshcontext		*context;
	jmp_buf			exitbuf;
	OSErr			err;
	char			*tabargv[64];
	int				argc;
	char			**argv;

	context = (lshcontext *)NewPtr(sizeof(lshcontext));
	if (context == NULL) {
		printf("### main_thread, NewPtr lshcontext failed\n");
		return 0L;
	}
	if (pthread_setspecific(ssh2threadkey, context)) {
		printf("### main_thread, pthread_setspecific failed\n");
		DisposePtr((Ptr)context);
		return 0L;
	}

	/* allocate memory pool */
	err = MPInit(65564, &context->_gMemPool, NULL);
	if (err != noErr) {
		printf("### main_thread, MPInit failed\n");
		DisposePtr((Ptr)context);
		return 0L;
	}

	context->_port = -1;
	context->_userdata = ctx;
	context->_thread = pthread_self();
	context->_forward = 0;
	context->_localport = 0;
	context->_listener = -1;
	context->_socket = -1;
	//context->_exitbuf = 0;
	//context->_gMemPool = NULL;
	memset(context->_filesTable, 0xff, sizeof(context->_filesTable));
	memcpy(&context->_mactermios, &defaulttermios, sizeof(struct termios));
	context->_gConsoleInEOF = 0;
	context->_convertLFs = (ctx->flags & kLSHConvertLFs) != 0;
	context->_stripCRs = (ctx->flags & kLSHStripCRs) != 0;
	context->_lastCR = 0;
	context->_insock = NULL;
	context->_gConsoleInBufLen = 0;
	context->_gConsoleInBufMax = CONSOLEBUFSIZE;
	context->_gConsoleOutBufLen = 0;
	context->_gConsoleOutBufMax = CONSOLEBUFSIZE;
	context->_all_objects = NULL;
	context->_number_of_objects = 0;
	context->_live_objects = 0;
	context->_quiet_flag = 0;
	context->_verbose_flag = 0;
	context->_trace_flag = 0;
	context->_debug_flag = 0;
	context->_error_fd = STDERR_FILENO;
	context->_error_pos = 0;
	context->_error_write = write_raw;
	context->_tracing = 0;
	context->_verbosing = 0;
	context->_debugging = 0;
	context->_window_changed = 0;
	context->_kpassword[0] = 0;
	context->_kindex = 0;
	context->_self = context;

	ctx->context = context;

	make_args( ctx->argstr, tabargv, &argc, &argv );

	make_env( context );

	context->_pexitbuf = &exitbuf;
	if ( !setjmp(exitbuf) ) {
		/* we need to intercept SIGINT to fake 'exit' */
		struct sigaction interrupt;
		memset(&interrupt, 0, sizeof(interrupt));
		interrupt.sa_handler = lsh_sighandler;
		sigemptyset(&interrupt.sa_mask);
		interrupt.sa_flags = 0;
		if (sigaction(SIGINT, &interrupt, NULL) < 0) {
			werror("warning: failed to install ssh2_sighandler for SIGINT\n");
		}
	 	set_error_syslog(applname);
		appl_main(argc, argv);
	}

	ctx->context = NULL;
	ctx->pthread = NULL;

	close_all_files(context);
	MPDispose(context->_gMemPool);
	DisposePtr((Ptr)context);
	
	return NULL;
}

#pragma mark -

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
				yesornofunc yes_or_no, unsigned long flags, long userData)
{
	pthread_attr_t	attr = NULL;
	lshctx			*ctx = NULL;

	ssh2_init();

	/* allocate new record */
	ctx = NewPtr(sizeof(lshctx));
	if (ctx == NULL) {
		goto failed;
	}

	ctx->pthread = NULL;
	ctx->context = NULL;
	ctx->flags = flags;
	ctx->userData = userData;

	/* save argument string */
	ctx->argstr = NewPtr(strlen(argstr) + 1);
	if (ctx->argstr == NULL) {
		goto failed;
	}
	strcpy(ctx->argstr, argstr);

	ctx->hdlevt = hdlevt;
	ctx->log = log;
	ctx->getpass = getpass;
	ctx->yes_or_no = yes_or_no;

	if (pthread_attr_init(&attr)) {
		printf("### main, pthread_attr_init failed\n");
		goto failed;
	}
	if (pthread_key_create(&ssh2threadkey, lsh_key_del)) {
		printf("### main, pthread_key_create failed\n");
		goto failed;
	}
	if (pthread_attr_setstacksize(&attr, 64*1024)) {
		printf("### main, pthread_attr_setstacksize failed\n");
		goto failed;
	}

	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
		printf("### main, pthread_attr_setdetachstate failed\n");
		goto failed;
	}

	if (pthread_create(&ctx->pthread, &attr, (GUSIPThreadProc)lsh_thread, ctx)) {
		printf("### main, pthread_create failed\n");
		goto failed;
	}
	pthread_attr_destroy(&attr);

	/* add this thread to local thread pool */
	/* TODO */

	return ctx;

failed:
	if ( attr != NULL ) {
		pthread_attr_destroy(&attr);
	}
	if ( ctx != NULL ) {
		if ( ctx->argstr != NULL ) {
			DisposePtr( ctx->argstr );
		}
		DisposePtr( ctx );
	}
	return NULL;
}


/*
 * lsh_delete
 */

void lsh_delete(lshctx *ctx)
{
	assert(ctx != NULL);
	if ( ctx->context != NULL && ctx->pthread != NULL ) {
		lshcontext *context = ctx->context;
		context->_gConsoleInEOF = true;
		pthread_kill( ctx->pthread, SIGINT );
		while (ctx->pthread) {
			/* wait for thread to die */
			ssh2_sched();
		}
	}
	if ( ctx->argstr != NULL ) {
		DisposePtr( ctx->argstr );
	}
	DisposePtr( ctx );
}


/*
 * lsh_yield
 */

void lsh_yield()
{
	ssh2_sched();
}


/*
 * lsh_read
 */

/* should it be blocking ? */

int lsh_read(lshctx *ctx, void *buffer, long inbytes)
{
	lshcontext *context = (lshcontext *)ctx->context;
	long n = inbytes;
	long outbytes;

	outbytes = 0;
	if ( context && context->_gConsoleOutBufLen ) {
		outbytes = context->_gConsoleOutBufLen;
		if ( outbytes > n ) {
			outbytes = n;
		}
		BlockMoveData( context->_gConsoleOutBuf, buffer, outbytes );
		context->_gConsoleOutBufLen -= outbytes;
		if ( context->_gConsoleOutBufLen ) {
			BlockMoveData( context->_gConsoleOutBuf + outbytes, context->_gConsoleOutBuf, context->_gConsoleOutBufLen );
			/*netputuev( CONCLASS, CONDATA, pnum,0);*/			/* more data to get */
		}
	}
	return outbytes;
}


int lsh_canread(lshctx *ctx)
{
	lshcontext *context = (lshcontext *)ctx->context;
	long outbytes = 0;

	if ( context && context->_gConsoleOutBufLen )
	{
		outbytes = context->_gConsoleOutBufLen;
	}
	return outbytes;
}


/*
 * lsh_write
 */

long lsh_write(lshctx *ctx, const void *buffer, long inbytes)
{
	lshcontext *context = (lshcontext *)ctx->context;
	long n = inbytes;
	long outbytes;

	outbytes = 0;
	if ( context ) {
		while ( n ) {
			if ( context->_gConsoleInBufLen < context->_gConsoleInBufMax ) {
				long len = n;
				if ( len > context->_gConsoleInBufMax - context->_gConsoleInBufLen )
					len = context->_gConsoleInBufMax - context->_gConsoleInBufLen;
				BlockMoveData( buffer, context->_gConsoleInBuf + context->_gConsoleInBufLen, len);
				context->_gConsoleInBufLen += len;
				(char *)buffer += len;
				n -= len;
				outbytes += len;

				if (context->_insock) {
					extern void GUSIWakeupTTYSocket( void *insock );
					GUSIWakeupTTYSocket(context->_insock);
				}

			} else {
				ssh2_sched();
			}
		}
	}
	return outbytes;
}

int lsh_canwrite(lshctx *ctx)
{
	lshcontext *context = (lshcontext *)ctx->context;
	long outbytes = 0;

	if ( context && context->_gConsoleInBufLen )
	{
		outbytes = context->_gConsoleInBufLen;
	}
	return outbytes;
}


/*
 * lsh_getprefsd
 */

char *lsh_getprefsd()
{
	char			*buf;
	short			vRefNum;
	long			dirID;

	buf = getprefsd( "MacSSH", homepath, sizeof(homepath), &vRefNum, &dirID );
	if ( !buf ) {
 		buf = getcwd(homepath, sizeof(homepath));
	}
	return buf;
}

/*
 * lsh_current
 */

lshctx *lsh_current()
{
	lshcontext *context = pthread_getspecific(ssh2threadkey);
	if ( context ) {
		return (lshctx *)context->_userdata;
	}
	return NULL;
}

/*
 * lsh_running
 */

Boolean lsh_running(lshctx *ctx)
{
	return ctx->pthread != NULL;
}

void lsh_init(void)
{
	ssh2_init();
}

void lsh_terminate(void)
{
	ssh2_terminate();
}

/*
 * __lsh_initialize
 */

pascal OSErr __lsh_initialize(const CFragInitBlock * theInitBlock)
{
	OSErr result = noErr;

	if(CFragHasFileLocation(theInitBlock->fragLocator.where) &&
		theInitBlock->fragLocator.u.onDisk.fileSpec != 0L)
	{
		gDLLFileSpec = *theInitBlock->fragLocator.u.onDisk.fileSpec;
	}

	/* check for OT 1.1.1 */
	/* TODO */

	/* check for Thread Manager */
	/* TODO */

	srand(time(NULL));

	result = __initialize(theInitBlock->connectionID);

	return result;
}


/*
 * __lsh_terminate
 */

pascal void __lsh_terminate(void)
{
	__terminate();

	/* kill pending threads in local thread pool */
	/* TODO */
}


/* macosmain.c
 *
 */

#include "lshprefix.h"
#include "lsh_context.h"

#include "format.h"
#include "io.h"
#include "tty.h"
#include "xalloc.h"
#include "werror.h"

#include <assert.h>
#include <console.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SIOUX.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdarg.h>
#include <setjmp.h>
#include <termios.h>

#include <Events.h>
#include <Fonts.h>
#include <LowMem.h>
#include <OSUtils.h>

#include "PasswordDialog.h"
#include "MemPool.h"

// we need atEOF to re-enable stdin after ctrl-d
extern Boolean atEOF;
extern Boolean gSIOUXTerminalMode;
extern Boolean SIOUXQuitting;

Boolean gLogStdIO = 0;

extern int appl_main(int argc, char **argv);
extern char *applname;
extern char *defargstr;

extern void ssh2_init();
extern void close_all_files();

extern const struct exception *
write_raw(int fd, UINT32 length, const UINT8 *data);


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

static char *tabargv[128];
static char argstr[1024];
static char currentdir[1024];

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
  va_list args;
  char string_buf[BUF_SIZE];
  int len;

  va_start(args, format);
  len = vsnprintf(string_buf, BUF_SIZE - 2, format, args);
  assert(len < BUF_SIZE - 1);
  va_end(args);
  if ( len > 0 ) {
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
	fprintf(stderr, "Assertion (%s) failed in \"%s\" on line %d\n", condition, filename, lineno);
	if ( pthread_getspecific(ssh2threadkey) ) {
		abort();
	} else {
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
	extern Boolean SIOUXQuitting;
	jmp_buf *exitbuf;

	fflush(stdout);
	trace("*** exit ***\n");
	fflush(stdout);

	if ( !SIOUXQuitting && (exitbuf = getexitbuf()) != NULL ) {
		longjmp( *exitbuf, 1 );
	}
	ExitToShell();
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

void ssh2_sched()
{
	sched_yield();
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
		assert(context->_self == context);
		tsize = items * size;
		p = MPmalloc( context->_gMemPool, tsize );
		if ( p ) {
			memset(p, '\0', tsize);
		}
		return p;
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

void add_one_file(int fd)
{
	if ( fd != -1 ) {
		lshcontext *context = pthread_getspecific(ssh2threadkey);
		if ( context ) {
			int i;
			for (i = 0; i < MAXFILESCOUNT; i++) {
				if (context->_filesTable[i] == -1) {
					context->_filesTable[i] = fd;
					break;
				}
			}
		}
	}
}

void remove_one_file(int fd)
{
	if ( fd != -1 ) {
		lshcontext *context = pthread_getspecific(ssh2threadkey);
		if ( context ) {
			int i;
			for (i = 0; i < MAXFILESCOUNT; i++) {
				if (context->_filesTable[i] == fd) {
					context->_filesTable[i] = -1;
					break;
				}
			}
		}
	}
}

void close_all_files()
{
	lshcontext *context = pthread_getspecific(ssh2threadkey);
	if ( context ) {
		int i;
		for (i = 0; i < MAXFILESCOUNT; i++) {
			if (context->_filesTable[i] != -1) {
				close(context->_filesTable[i]);
			}
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
int InstallTTY(int id, void *context)
{
#pragma unused (id, context)
	return 0;
}

/*
 * RemoveTTY
 */
void RemoveTTY(int id, void *context)
{
#pragma unused (id, context)
}

/*
 * WriteCharsToTTY
 */
int WriteCharsToTTY(int id, void *context, char *buffer, int n)
{
#pragma unused (id, context)
	return WriteCharsToConsole(buffer, n);
}

/*
 * ReadCharsFromTTY
 */
int ReadCharsFromTTY(int id, void *context, char *buffer, int n)
{
#pragma unused (id, context)
	return ReadCharsFromConsole(buffer, n);
}

/*
 * AvailableFromTTY
 */
int AvailableFromTTY(int id, void *context)
{
#pragma unused (id, context)
	extern short gSIOUXBufSize;
	if ( !gSIOUXBufSize ) {
		QHdrPtr eventQueue = LMGetEventQueue();
		EvQElPtr element = (EvQElPtr)eventQueue->qHead;
		// now, count the number of pending keyDown events.
		while (element != nil) {
			if (element->evtQWhat == keyDown || element->evtQWhat == autoKey)
				return true;
			element = (EvQElPtr)element->qLink;
		}
		return false;
	}
	return true;
}

#pragma mark -

/*
 * tty_getwinsize : replaces tty_getwinsize from liblsh
 */
int
tty_getwinsize(int fd, struct terminal_dimensions *dims)
{
  FontInfo	info;

  dims->char_width = SIOUXSettings.columns;
  dims->char_height = SIOUXSettings.rows;
  dims->pixel_width = 0;
  dims->pixel_height = 0;
/*
  if (!FetchFontInfo(SIOUXSettings.fontid,SIOUXSettings.fontsize,SIOUXSettings.fontface, &info)) {
  	dims->pixel_width = info.widMax;
  	dims->pixel_height = info.ascent + info.descent;
  } else {
    // just guess...
    dims->pixel_width = 12;
    dims->pixel_height = 16;
  }
*/
  return 1;
}


/*
 * tcgetattr : replaces tty_getattr from liblsh
 */
int
tcgetattr(int fd, struct termios *ios)
{
  lshcontext *context = pthread_getspecific(ssh2threadkey);
  if (context) {
  	memcpy(ios, &context->_mactermios, sizeof(struct termios));
  }
  return 1;
}

/*
 * tcsetattr : replaces tty_setattr from liblsh
 */
int
tcsetattr(int fd, int optional_actions, const struct termios *ios)
{
  lshcontext *context = pthread_getspecific(ssh2threadkey);
  if (context) {
    memcpy(&context->_mactermios, ios, sizeof(struct termios));
  }
  return 1;
}


/*
 * getpass
 */

char *getpass( const char *prompt )
{
	Str255 pprompt;
	Str255 ppassword;
	static char password[128];

	pprompt[0] = strlen(prompt);
	memcpy(pprompt + 1, prompt, pprompt[0]);
	ppassword[0] = 0;
	InternalBufferDialog(pprompt, ppassword);
	memcpy(password, ppassword + 1, ppassword[0]);
	password[ppassword[0]] = '\0';
}


/*
 * yes_or_no
 */

int yes_or_no(const struct lsh_string *s, int def, int free)
{
	if (!quiet_flag) {
		const char *prompt = lsh_get_cstring(s);
		if (prompt) {
			Str255 pprompt;
			int i;
			pprompt[0] = strlen(prompt);
			memcpy(pprompt + 1, prompt, pprompt[0]);
			for (i = 1; i <= pprompt[0]; i++) {
				if (pprompt[i] == 0x0a) {
					pprompt[i] = 0x0d;
				}
			}
			def = YesNoDialog( pprompt );
	    }
	}
	return def;
}

#pragma mark -


/*
 * make_args
 */

void make_args( char *argstr, int *argc, char ***argv )
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

	getcwd(currentdir, 1024);	// set home pathname
}

/*
 * run_app
 */

static void run_app()
{
	lshcontext		*context;
	jmp_buf			exitbuf;
	extern Boolean SIOUXQuitting;
	char			moreargs[1024];
	char			*pmoreargs;
	int				argc;
	char			**argv;
	OSErr			err;

	context = (lshcontext *)NewPtr(sizeof(lshcontext));
	if (context == NULL) {
	  printf("### main_thread, NewPtr lshcontext failed\n");
	  return;
	}
	if (pthread_setspecific(ssh2threadkey, context)) {
	  printf("### main_thread, pthread_setspecific failed\n");
	  DisposePtr((Ptr)context);
	  return;
	}

	/* allocate memory pool */
	err = MPInit(65564, &context->_gMemPool, NULL);
	if (err != noErr) {
	  printf("### main_thread, MPInit failed\n");
	  DisposePtr((Ptr)context);
	  return;
	}

	context->_port = -1;
	context->_userdata = NULL;
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
	context->_convertLFs = 0;
	context->_stripCRs = 0;
	context->_lastCR = 0;
	context->_insock = NULL;
	context->_gConsoleInBufLen = 0;
	context->_gConsoleInBufMax = CONSOLEBUFSIZE;
	context->_gConsoleOutBufLen = 0;
	context->_gConsoleOutBufMax = CONSOLEBUFSIZE;
	context->_all_objects = NULL;
	context->_number_of_objects = 0;
	context->_number_of_strings = 0;
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

	context->_pexitbuf = &exitbuf;
	if (!setjmp(&exitbuf)) {
	
	  printf("\n\nlsh shell application\nargs (--help for info): ");
	
	  fflush(stdout);
	  strcpy(argstr, applname);
	  strcat(argstr, " ");
	  atEOF = false;
	 gSIOUXTerminalMode = false;
	  fgets(moreargs, sizeof(moreargs), stdin);
	 gSIOUXTerminalMode = true;
	  moreargs[strlen(moreargs) - 1] = '\0';
	  pmoreargs = moreargs;
	  if ( moreargs[0] == '+' || !moreargs[0] ) {
	    strcat(argstr, defargstr);
	    if ( moreargs[0] ) {
	      strcat(argstr, " ");
	      pmoreargs++;
	    }
	  }
	  strcat(argstr, pmoreargs);
	
	  make_args( argstr, &argc, &argv );
	 //set_error_syslog(applname);
	  appl_main(argc, argv);
	}

	close_all_files();
	MPDispose(context->_gMemPool);
	DisposePtr((Ptr)context);
}

/*
 * main_thread
 */

void *main_thread()
{
  while (!SIOUXQuitting) {
	run_app();
  }
}

void KeyDestructor(void *)
{
}

/*
 * main
 */

int main()
{
  char moreargs[1024];
  char *pmoreargs;
  int argc;
  char **argv;
  pthread_attr_t attr;
  pthread_t mainthread;

#if TARGET_CPU_68K
  SetApplLimit(GetApplLimit() - 128000);	/* We need more stack */
  MaxApplZone();
#endif

  SIOUXSettings.autocloseonquit = 1;
  SIOUXSettings.asktosaveonclose = 0;
  SIOUXSettings.showstatusline = 1;
  SIOUXSettings.columns = 80;
  SIOUXSettings.rows = 48;
  SIOUXSettings.toppixel = 50;
  SIOUXSettings.leftpixel = 20;
  SIOUXSettings.tabspaces = 8;

  srand(time(NULL));

  ssh2_init();

#if 1

  // need to flush stdout to open the window from the main thread
  printf("\n");
  fflush(stdout);

  if (pthread_attr_init(&attr)) {
    printf("### main, pthread_attr_init failed\n");
    return 2;
  }
  if (pthread_key_create(&ssh2threadkey, KeyDestructor)) {
	printf("### main, pthread_key_create failed\n");
    return 2;
  }
  if (pthread_attr_setstacksize(&attr, 64*1024)) {
    printf("### main, pthread_attr_setstacksize failed\n");
    return 2;
  }
  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
    printf("### main, pthread_attr_setdetachstate failed\n");
    return 2;
  }
  if (pthread_create(&mainthread, &attr, (GUSIPThreadProc)main_thread, NULL)) {
    printf("### main, pthread_create failed\n");
    return 2;
  }
  pthread_attr_destroy(&attr);

  while (!SIOUXQuitting) {
  	sched_yield();
  }

#else
  while (!SIOUXQuitting) {
	run_app();
  }
#endif
  return 0;
}


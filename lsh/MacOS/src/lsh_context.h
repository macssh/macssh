#ifndef _LSH_CONTEXT_H
#define _LSH_CONTEXT_H

#include <Types.h>

#include <pthread.h>
#include <setjmp.h>
#include <termios.h>
#include <signal.h>

#include "lsh_types.h"
#include "lsh.h"
#include "exception.h"

#define MAXFILESCOUNT 64
#define CONSOLEBUFSIZE 16384
#define ERROR_BUF_SIZE 512

typedef struct lshcontext {
	int				_port;		// for BetterTelnet's networking
	void			*_userdata;	// available for callers
	pthread_t		_thread;
	int				_forward;
	int				_localport;
	int				_listener;	// for cvs
	int				_socket;	// for cvs
	int				_rcmdline;	// for cvs
	jmp_buf			*_pexitbuf;
	char			*_gMemPool;
	int				_filesTable[MAXFILESCOUNT];
	struct termios	_mactermios;

	Boolean 		_gConsoleInEOF;
	Boolean			_convertLFs;
	Boolean			_stripCRs;
	char			_lastCR;

	void			*_insock;
	long			_gConsoleInBufLen;
	long			_gConsoleInBufMax;
	char			_gConsoleInBuf[CONSOLEBUFSIZE];
	long			_gConsoleOutBufLen;
	long			_gConsoleOutBufMax;
	char			_gConsoleOutBuf[CONSOLEBUFSIZE];

	struct lsh_object *_all_objects;
	unsigned		_number_of_objects;
	unsigned		_number_of_strings;
	unsigned		_live_objects;

	int				_quiet_flag;
	int				_verbose_flag;
	int				_trace_flag;
	int				_debug_flag;

	int				_error_fd;
	UINT8			_error_buffer[ERROR_BUF_SIZE];
	UINT32			_error_pos;
	const struct exception *(*_error_write)(int fd, UINT32 length, const UINT8 *data);

	int				_tracing;
	int				_verbosing;
	int				_debugging;

	sig_atomic_t	_window_changed;

	char			*_envv[4];		// "HOME", "LOGNAME", "TERM", "DISPLAY"
	char			_term[64];		// home and logname never change
	char			_display[64];	// home and logname never change
	char			_kpassword[64];
	int				_kindex;
	int				_pindex;
	char			_keychainprompt[256];

	struct lshcontext *_self;

} lshcontext;

extern pthread_key_t ssh2threadkey;


#define all_objects (((lshcontext *)pthread_getspecific(ssh2threadkey))->_all_objects)
#define number_of_objects (((lshcontext *)pthread_getspecific(ssh2threadkey))->_number_of_objects)
#define number_of_strings (((lshcontext *)pthread_getspecific(ssh2threadkey))->_number_of_strings)

#define live_objects (((lshcontext *)pthread_getspecific(ssh2threadkey))->_live_objects)

#define quiet_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_quiet_flag)
#define verbose_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_verbose_flag)
#define trace_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_trace_flag)
#define debug_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_debug_flag)

#define error_fd (((lshcontext *)pthread_getspecific(ssh2threadkey))->_error_fd)
#define error_buffer (((lshcontext *)pthread_getspecific(ssh2threadkey))->_error_buffer)
#define error_pos (((lshcontext *)pthread_getspecific(ssh2threadkey))->_error_pos)
#define error_write (((lshcontext *)pthread_getspecific(ssh2threadkey))->_error_write)

#define tracing (((lshcontext *)pthread_getspecific(ssh2threadkey))->_tracing)
#define verbosing (((lshcontext *)pthread_getspecific(ssh2threadkey))->_verbosing)
#define debugging (((lshcontext *)pthread_getspecific(ssh2threadkey))->_debugging)
#define convertLFs (((lshcontext *)pthread_getspecific(ssh2threadkey))->_convertLFs)
#define stripCRs (((lshcontext *)pthread_getspecific(ssh2threadkey))->_stripCRs)


#define window_changed (((lshcontext *)pthread_getspecific(ssh2threadkey))->_window_changed)

#endif

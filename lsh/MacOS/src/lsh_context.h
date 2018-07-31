#ifndef _LSH_CONTEXT_H
#define _LSH_CONTEXT_H

#include <Types.h>

#include <pthread.h>
#include <setjmp.h>

#include "lsh_types.h"
#include "lsh.h"

#define CONSOLEBUFSIZE 16384

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

	int				_quiet_flag;
	int				_verbose_flag;
	int				_trace_flag;
	int				_debug_flag;

	int				_tracing;
	int				_verbosing;
	int				_debugging;

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

#define quiet_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_quiet_flag)
#define verbose_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_verbose_flag)
#define trace_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_trace_flag)
#define debug_flag (((lshcontext *)pthread_getspecific(ssh2threadkey))->_debug_flag)

#define tracing (((lshcontext *)pthread_getspecific(ssh2threadkey))->_tracing)
#define verbosing (((lshcontext *)pthread_getspecific(ssh2threadkey))->_verbosing)
#define debugging (((lshcontext *)pthread_getspecific(ssh2threadkey))->_debugging)
#define convertLFs (((lshcontext *)pthread_getspecific(ssh2threadkey))->_convertLFs)
#define stripCRs (((lshcontext *)pthread_getspecific(ssh2threadkey))->_stripCRs)


#define window_changed (((lshcontext *)pthread_getspecific(ssh2threadkey))->_window_changed)

#endif

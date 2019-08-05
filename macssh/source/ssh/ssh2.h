/*
 * ssh2.h
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

/****** moved from lshprefix.h */

#define exit macosexit
#define abort macosabort

/*******/

/****** moved from lsh_context.h */

#include <pthread.h>
#include <setjmp.h>

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

	char			_term[64];		// home and logname never change
	char			_display[64];	// home and logname never change
	char			_kpassword[64];
	int				_kindex;
	int				_pindex;
	char			_keychainprompt[256];

	struct lshcontext *_self;

} lshcontext;

/*******/

#include "wind.h"

#include <PLStringFuncs.h>

#ifdef __cplusplus
extern "C" {
#endif

void ssh2_init();
void ssh2_sched();
void GUSIHandleNextEvent(long sleepTime);

WindRec *ssh2_window();
Boolean can_read();
void ssh2_doevent(long sleepTime);
pascal Ptr PLstrrchr(ConstStr255Param s, short c);
void close_all_files(lshcontext *context);

#ifdef __cplusplus
}
#endif


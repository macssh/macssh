/*
 * console.stubs.c
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

#ifndef __CONSOLE__
#include <console.h>
#endif
#include <sched.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/*
 *	The following four functions provide the UI for the console package.
 *	Users wishing to replace SIOUX with their own console package need
 *	only provide the four functions below in a library.
 */

/*
 *	extern short InstallConsole(short fd);
 *
 *	Installs the Console package, this function will be called right
 *	before any read or write to one of the standard streams.
 *
 *	short fd:		The stream which we are reading/writing to/from.
 *	returns short:	0 no error occurred, anything else error.
 */

short InstallConsole(short fd)
{
#pragma unused (fd)

	return 0;
}

/*
 *	extern void RemoveConsole(void);
 *
 *	Removes the console package.  It is called after all other streams
 *	are closed and exit functions (installed by either atexit or _atexit)
 *	have been called.  Since there is no way to recover from an error,
 *	this function doesn't need to return any.
 */

void RemoveConsole(void)
{
}


/*
 * can_read
 */

Boolean can_read()
{
	lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if ( !context ) {
		return 0;
	}
	return context->_gConsoleInBufLen || context->_gConsoleInEOF;
}

/*
 *	extern long WriteCharsToConsole(char *buffer, long n);
 *
 *	Writes a stream of output to the Console window.  This function is
 *	called by write.
 *
 *	char *buffer:	Pointer to the buffer to be written.
 *	long n:			The length of the buffer to be written.
 *	returns short:	Actual number of characters written to the stream,
 *					-1 if an error occurred.
 */

long WriteCharsToConsole(char *buffer, long n)
{
	long			written = 0;
	lshcontext		*context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	char			*buf = buffer;
	char			c;

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
		        	if ( context->_convertLFs ) {
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
					if ( context->_port != -1 ) {
						/*netputuev(CONCLASS, CONDATA, context->_port,0);*/
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
 *	extern long ReadCharsFromConsole(char *buffer, long n);
 *
 *	Reads from the Console into a buffer.  This function is called by
 *	read.
 *
 *	char *buffer:	Pointer to the buffer which will recieve the input.
 *	long n:			The maximum amount of characters to be read (size of
 *					buffer).
 *	returns short:	Actual number of characters read from the stream,
 *					-1 if an error occurred.
 */

long ReadCharsFromConsole(char *buffer, long n)
{
	long			len = 0;
	lshcontext		*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if ( !context ) {
		return 0;
	}

	if ( context->_gConsoleInBufMax ) {
		if ( context->_socket == -1 ) {
			while (!len && n > 0) {
				if (context->_gConsoleInEOF) {
					buffer[0] = -1;
					return 0;
				}
				if (context->_gConsoleInBufLen) {
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
		}
	} else {
		buffer[0] = -1;
	}
/*
	syslog( 0, "read\n");
	dumpln(0, 0, buffer, len);
*/
	return len;
}

/*
 *	extern char *__ttyname(long fildes);
 *
 *	Return the name of the current terminal (only valid terminals are
 *	the standard stream (ie stdin, stdout, stderr).
 *
 *	long fildes:	The stream to query.
 *
 *	returns char*:	A pointer to static global data which contains a C string
 *					or NULL if the stream is not valid.
 */

extern char *__ttyname(long fildes)
{
#pragma unused (fildes)
	/* all streams have the same name */
	static char *__devicename = "null device";

	if (fildes >= 0 && fildes <= 2)
		return (__devicename);

	return (0L);
}

/* Begin mm 981218 */
/*
*
*    int kbhit()
*
*    returns true if any keyboard key is pressed without retrieving the key
*    used for stopping a loop by pressing any key
*/
int kbhit(void)
{
      return 0; 
}

/*
*
*    int getch()
*
*    returns the keyboard character pressed when an ascii key is pressed  
*    used for console style menu selections for immediate actions.
*/
int getch(void)
{
      return 0; 
}

/*
*     void clrscr()
*
*     clears screen
*/
void clrscr()
{
	return;
}
/* End mm 981218 */

/*     Change record
 *  mm 981218	Added stubs for kbhit(), getch(), and clrscr()
*/

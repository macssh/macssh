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

#ifndef __CONSOLE__
#include <console.h>
#endif
#include <sched.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "InternalEvents.h"
#include "netevent.proto.h"
#include "wind.h"
#include "Connections.proto.h"

#include "ssh2.h"
#include "GUSITTY.h"


extern void syslog( int priority, const char *format, ...);

static void accept_call(lshcontext *context);
static void check_listener(lshcontext *context);


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
	/* since we redirect stdin/out/err to our 'sockets'
	 * this shouldn't occur ! (or lsh calls printf ? (; )
	 */
//	DebugStr("\pWriteCharsToConsole");
//	return n;
	return WriteCharsToTTY(1, pthread_getspecific(ssh2threadkey), buffer, n);
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
	/* since we redirect stdin/out/err to our 'sockets'
	 * this shouldn't occur ! (or lsh calls printf ? (; )
	 */
//	DebugStr("\pReadCharsFromConsole");
//	return 0;
	return ReadCharsFromTTY(0, pthread_getspecific(ssh2threadkey), buffer, n);
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


#pragma mark -

/*
 * accept_call
 */

static void accept_call(lshcontext *context)
{
	if ( context->_listener != -1 && context->_socket == -1 ) {
		struct sockaddr peer;
		socklen_t addr_len = sizeof(peer);
		context->_socket = accept( context->_listener, &peer, &addr_len );
		if ( context->_socket != -1 ) {
			WindRec *wind = ssh2_window();
			struct sockaddr local;
			addr_len = sizeof(local);
			getsockname( context->_socket, &local, &addr_len );
			/*syslog(0, "accept_call, local : 0x%x, remote : 0x%x\n", ((struct sockaddr_in *)&local)->sin_addr.s_addr, ((struct sockaddr_in *)&peer)->sin_addr.s_addr );*/
			if ( wind == NULL || peer.sa_family != AF_INET
			 || ( !wind->ssh2guests
			   && ( ((struct sockaddr_in *)&peer)->sin_addr.s_addr
			     != ((struct sockaddr_in *)&local)->sin_addr.s_addr ) ) ) {
		   		/* reject call */
				close( context->_socket );
				context->_socket = -1;
			} else {
				/* start faked rcmd server */
				/* we wait for 4 lines terminated by '\0' */
				context->_rcmdline = 4;

			}
		}
	}
}


/*
 * check_listener
 */

static void check_listener(lshcontext *context)
{
	if ( context->_listener != -1 && context->_socket == -1 ) {
		struct fd_set rfds;
		struct timeval timeout;
		FD_ZERO( &rfds );
		FD_SET( context->_listener, &rfds );
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		if ( select( 32, &rfds, NULL, NULL, &timeout ) ) {
			if ( FD_ISSET( context->_listener, &rfds ) ) {
				accept_call(context);
			}
		}
	}
}

/*
 * read_local_socket
 */

static long read_local_socket(lshcontext *context, char *buffer, long n)
{
	if ( context->_socket != -1 ) {
		n = read( context->_socket, buffer, n );
		if ( n <= 0 ) {
			close( context->_socket );
			context->_socket = -1;
			n = 0;
		} else if ( context->_rcmdline ) {
			/* if rcmd pending, deal with data locally */
			int i;
			for ( i = 0; i < n && context->_rcmdline; i++ ) {
				if ( buffer[i] == '\0' ) {
					/* consume one line */
					--context->_rcmdline;
				}
				if ( context->_rcmdline == 0 ) {
					/* last rcmd line received, send reply */
					char c = '\0';
					write( context->_socket, &c, 1 );
				}
			}
			BlockMoveData( buffer + i, buffer, n -= i);
		}
	} else {
		n = 0;
	}
	return n;
}


#pragma mark -

/*
 * InstallTTY
 */
int InstallTTY(int id, void *ctx, void *insock)
{
	if ( id == 0 ) {
		lshcontext *context = ctx;
		context->_insock = insock;
	}
	return 0;
}

/*
 * RemoveTTY
 */
void RemoveTTY(int id, void *ctx, void *insock)
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
	lshcontext		*context = ctx;
	char			*buf = buffer;
	char			c;

	if ( !context ) {
		context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	}
	if ( id == 2 ) {
		if ( !context || context->_port != -1 ) {
			// log stderr to console
			putlln( buffer, n );
			return n;
		}
	}
	if ( !context ) {
		return 0;
	}

	if ( context->_listener != -1 ) {
		/* write data to local socket */
		check_listener(context);
		if ( context->_socket != -1 ) {
			long len = n;
			long inlen = 0;
/*
			while ( n > 0 && context->_socket != -1 ) {
				long len = n;
				long inlen = 0;
				char tmpbuf[512];
				long outlen = 0;

syslog(0, "WriteCharsToTTY\n" );
dumpln(0L, NULL, buf, len);

				while ( inlen < len && outlen < sizeof(tmpbuf) - 1 ) {
					char c = buf[inlen++];
					if ( c != 0x0d ) {
						tmpbuf[outlen++] = c;
					}
				}
				if (outlen) {

syslog(0, "write socket\n" );
dumpln(0L, NULL, tmpbuf, outlen);
					inlen = write( context->_socket, tmpbuf, outlen );
					if ( inlen <= 0 ) {
						close( context->_socket );
						context->_socket = -1;
					} else {
						buf += inlen;
						written += inlen;
						n -= inlen;
					}
				}
			}
*/
			inlen = write( context->_socket, buf, len );
			if ( inlen <= 0 ) {
				close( context->_socket );
				context->_socket = -1;
			} else {
				buf += inlen;
				written += inlen;
				n -= inlen;
			}
		}
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
						netputuev(CONCLASS, CONDATA, context->_port,0);
					}
				}
				ssh2_sched();
			}
		}
	} else {
		written = n;
	}
/*
	syslog( 0, "from ssh\n");
	dumpln(0, 0, buffer, written);
*/
	return written;
}

/*
 * ReadCharsFromTTY
 */
int ReadCharsFromTTY(int id, void *ctx, char *buffer, int n)
{
	long			len = 0;
	lshcontext		*context = ctx;

	if ( !context ) {
		return 0;
	}

	if ( context->_gConsoleInBufMax ) {
		while (!len && n > 0) {
			if ( context->_gConsoleInBufLen && context->_socket == -1 ) {
				len = context->_gConsoleInBufLen;
				if ( len > n ) {
					len = n;
				}
				BlockMoveData( context->_gConsoleInBuf, buffer, len );
				context->_gConsoleInBufLen -= len;
				if ( context->_gConsoleInBufLen ) {
					BlockMoveData( context->_gConsoleInBuf + len, context->_gConsoleInBuf, context->_gConsoleInBufLen );
				}
			} else if (context->_gConsoleInEOF) {
				buffer[0] = EOF;
				return 0;
			}
			ssh2_sched();
			if ( context->_listener != -1 ) {
				/* read data from local socket */
				check_listener(context);
				if ( context->_socket != -1 ) {
					len = read_local_socket(context, buffer, n);
					if ( context->_socket == -1 && len == 0 ) {
						/* disconnected */
						break;
					}
				}
			}
		}
	} else {
		buffer[0] = EOF;
	}
/*
	syslog( 0, "to ssh\n");
	dumpln(0, 0, buffer, len);
*/
	return len;
}

/*
 * AvailableFromTTY
 */
int AvailableFromTTY(int id, void *ctx)
{
	lshcontext		*context = ctx;

	if ( !context ) {
		return 0;
	}
	if ( context->_listener != -1 ) {
		struct fd_set rfds;
		struct fd_set efds;
		struct timeval timeout;
		FD_ZERO( &rfds );
		FD_ZERO( &efds );
		FD_SET( context->_listener, &rfds );
		if ( context->_socket != -1 ) {
			FD_SET( context->_socket, &rfds );
			FD_SET( context->_socket, &efds );
		}
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		if ( select( 32, &rfds, NULL, &efds, &timeout ) ) {
			if ( context->_socket == -1 ) {
				if ( FD_ISSET( context->_listener, &rfds ) ) {
					accept_call(context);
				}
			} else {
				if ( FD_ISSET( context->_socket, &efds ) ) {
					close( context->_socket );
					context->_socket = -1;
				} else if ( FD_ISSET( context->_socket, &rfds ) ) {
					return 1;
				}
			}
		}
	}
	return context->_gConsoleInBufLen || context->_gConsoleInEOF;
}


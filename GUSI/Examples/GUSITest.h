/*********************************************************************
File		:	GUSI				-	Grand Unified Socket Interface
File		:	GUSITest.h		-	Common testing gear
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:54  chombier
Initial import

Revision 1.2  2000/05/23 06:43:25  neeri
Separate out descriptor commands

Revision 1.1  1998/10/25 11:57:29  neeri
Ready to release 2.0a3

Revision 1.2  1994/12/31  01:18:29  neeri
Benchmark support.

Revision 1.1  1994/02/25  02:48:06  neeri
Initial revision

Revision 0.1  1992/09/08  00:00:00  neeri
Factor out more common code

*********************************************************************/

#ifndef _GUSITEST_
#define _GUSITEST_

typedef void (*TestCmd)(char ch1, char ch2, const char * restcmd);

#include "GUSITest_P.h"

#include <stdio.h>

extern FILE *  input;
extern int		inputline;

/* void COMMAND(
 				char ch1, char ch2,			Command name
				TestCmd p,						Command to be run
				char *  s,						Arguments to command
				char *  h);						Explanation for command

	Example:
		COMMAND('m', 'd', MkDir, "directory",			"Make a new directory");
*/

#define COMMAND(ch1,ch2,p,s,h)	\
	DISPATCH(ch1,ch2)	=	(p),		\
	USAGE(ch1,ch2) 	=	(s),		\
	HELPMSG(ch1,ch2)	=	(h)

/* An useful macro for dumping variables.
	
	Example:
		DUMP(statbuf.st_dev,d);
*/

#define DUMP(EXPR, MODE)	printf("#    %s = %"#MODE"\n", #EXPR, EXPR)

/* Add common commands for descriptors */

void AddDescriptorCommands();

/* Add common commands for sockets */

void AddSocketCommands();

/* Run the test. Define your commands with COMMAND and call this */

void RunTest(int argc, char ** argv);

/* Print a MPW executable location note */

void Where();

/* Print a prompt */

void Prompt();

/* Return a string of the current error number, e.g. "EINVAL" */

const char * Explain();

/* Print a usage message for a command */

void Usage(char ch1, char ch2);

/* Clean up sockets */

void CleanupSockets();

extern int sock;				/* Socket to read/write to				*/
extern int accsock;			/* Socket to accept connections on 	*/

/* Keep statistics on a series of values */

typedef struct {
	int 	count;
	long	min;
	long  max;
	long	sum;
} Sampler;

void InitSampler(Sampler * samp);
void Sample(Sampler * samp, long sample);

#endif

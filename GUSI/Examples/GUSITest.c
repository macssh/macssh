/*********************************************************************
File		:	GUSI				-	Grand Unified Socket Interface
File		:	GUSITest.c		-	Common testing gear
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:54  chombier
Initial import

Revision 1.4  2000/05/23 06:45:28  neeri
Add profiler support

Revision 1.3  1999/06/28 05:43:30  neeri
Handle signals

Revision 1.2  1999/04/10 04:26:22  neeri
Get rid of nonstandard error codes

Revision 1.1  1998/10/25 11:57:28  neeri
Ready to release 2.0a3

Revision 1.2  1994/12/31  01:15:09  neeri
ANSIfy.
Benchmark support.

Revision 1.1  1994/02/25  02:47:57  neeri
Initial revision

Revision 0.2  1992/09/20  00:00:00  neeri
Allow empty lines & comments

Revision 0.1  1992/09/08  00:00:00  neeri
Factor out more common code

*********************************************************************/

#include <Memory.h>
#include <QuickDraw.h>
#include "GUSITest.h"
#include <Types.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/errno.h>

#if __profile__
#include <Profiler.h>
#endif

#ifdef __MWERKS__
#include <SIOUX.h>
#endif

Boolean 	HellHoundOnMyTrail = true;			/* Gotta keep on moving */
char		infilename[200];
char *	inputfrom;
FILE *	input;
int		inputline;
CmdDef 	commands[NROFCMDS];
	
void Help(char ch1, char ch2, const char * cmd)
{	
	printf("Commands are:\n\n");
	
	for (ch1 = 'a'; ch1 <= 'z'; ++ch1)
		for (ch2 = 0; ch2 <= 'z'; ch2 ? ++ch2 : (ch2 = 'a'))
			if (HELPMSG(ch1,ch2))
				printf(
					"   %c%c %-25s -- %s\n", 
					ch1, 
					ch2 ? ch2 : ' ', 
					USAGE(ch1,ch2), 
					HELPMSG(ch1,ch2));
	
	printf("\n");
}

void Where()
{
	if (inputfrom)
		printf("File '%s'; Line %d\n", inputfrom, inputline);
}

void Prompt()
{
	if (!inputfrom)
		printf("[%d]%c", inputline, ' ');
}

#define CASE(code)	case code: return #code

const char * Explain()
{
	switch (errno) {
	CASE(EPERM);
	CASE(ENOENT);
	CASE(ESRCH);
	CASE(EINTR);
	CASE(EIO);
	CASE(ENXIO);
	CASE(E2BIG);
	CASE(ENOEXEC);
	CASE(EBADF);
	CASE(ECHILD);
	CASE(ENOMEM);
	CASE(EACCES);
	CASE(EFAULT);
	CASE(EBUSY);
	CASE(EEXIST);
	CASE(EXDEV);
	CASE(ENODEV);
	CASE(ENOTDIR);
	CASE(EISDIR);
	CASE(EINVAL);
	CASE(ENFILE);
	CASE(EMFILE);
	CASE(ENOTTY);
	CASE(EFBIG);
	CASE(ENOSPC);
	CASE(ESPIPE);
	CASE(EROFS);
	CASE(EMLINK);
	CASE(EPIPE);
	CASE(EDOM);
	CASE(ERANGE);
	CASE(EWOULDBLOCK);
	CASE(EINPROGRESS);
	CASE(EALREADY);
	CASE(ENOTSOCK);
	CASE(EDESTADDRREQ);
	CASE(EMSGSIZE);
	CASE(EPROTOTYPE);
	CASE(ENOPROTOOPT);
	CASE(EPROTONOSUPPORT);
	CASE(ESOCKTNOSUPPORT);
	CASE(EOPNOTSUPP);
	CASE(EPFNOSUPPORT);
	CASE(EAFNOSUPPORT);
	CASE(EADDRINUSE);
	CASE(EADDRNOTAVAIL);
	CASE(ENETDOWN);
	CASE(ENETUNREACH);
	CASE(ENETRESET);
	CASE(ECONNABORTED);
	CASE(ECONNRESET);
	CASE(ENOBUFS);
	CASE(EISCONN);
	CASE(ENOTCONN);
	CASE(ESHUTDOWN);
	CASE(ETOOMANYREFS);
	CASE(ETIMEDOUT);
	CASE(ECONNREFUSED);
	CASE(ELOOP);
	CASE(ENAMETOOLONG);
	CASE(EHOSTDOWN);
	CASE(EHOSTUNREACH);
	CASE(ENOTEMPTY);
	CASE(ENOLCK);
	CASE(ENOSYS);
	default:
		return "Unknown";
	}		
}

void Usage(char ch1, char ch2)
{
	printf("# Usage is: %c%c %s\n", ch1, ch2 ? ch2 : ' ', USAGE(ch1,ch2));
	Where();
}

void Dispatch(const char * command)
{
	char		ch1	=	command[0];
	char		ch2	=	command[1];
	TestCmd	exec;
	
	/* We are guaranteed to have at least one valid character */
	
	switch (ch1) {
	case '\n':
	case '#':
		return;
	}

	if (!ch2)
		++command;
	else {
		if (isspace(ch2))	{
			command	+=	1;
			ch2		=	0;
		} else
			command	+=	2;
		
		/* Skip rest of first word */	
		for (; *command && !isspace(*command); ++command);
		
		/* Skip whitespace */
		while (isspace(*command))
			++command;
	}
	
	if (isalpha(ch1) && (!ch2 || isalpha(ch2)) && (exec = DISPATCH(ch1,ch2)))
		exec(ch1, ch2, command);
	else {
		if (ch2)
			printf("# Unknown command: '%c%c'\n", ch1, ch2);
		else
			printf("# Unknown command: '%c'\n", ch1);
			
		printf("# Type 'h' for a list of known commands.\n"); 
		
		Where();
	}
}

void Quit(char ch1, char ch2, const char * cmd)
{
	HellHoundOnMyTrail = false;
}

void signal_handler(int sig)
{
	/* Don't do anything, just interrupt system calls */
}

void Alarm(char ch1, char ch2, const char * cmd)
{
	signal(SIGALRM, signal_handler);
	alarm(atoi(cmd));
}

void InitSampler(Sampler * samp)
{
	samp->count	= 0;
	samp->min   = 0x7FFFFFFF;
	samp->max   = -samp->min;
	samp->sum   = 0;
}

void Sample(Sampler * samp, long sample)
{
	++samp->count;
	if (sample < samp->min)
		samp->min = sample;
	if (sample > samp->max)
		samp->max = sample;
	samp->sum += sample;
}

void RunTest(int argc, char ** argv)
{
	struct sigaction	act;
	char 				cmd[80];

#if __profile__
	ProfilerInit(collectDetailed, bestTimeBase, 100, 20);
	ProfilerSetStatus(0);
#endif

	COMMAND('h',   0, Help,  "", 						"Print this list");
	COMMAND('a', 'l', Alarm, "n", 						"Raise an alarm in n seconds");
	COMMAND('q',   0, Quit,  "", 						"End the sad existence of this program");

#ifdef __MWERKS__
	SIOUXSettings.asktosaveonclose = false;
#else
	InitGraf((Ptr) &qd.thePort);
#endif

	act.sa_handler 	= signal_handler;
	act.sa_mask	  	= 0;
	act.sa_flags  	= 0;
	sigaction(SIGINT, &act, NULL);
	
	if (--argc <= 0)
		Help('h', 0, "");
	
	do {
		if (argc > 0 && strcmp(inputfrom = *++argv, "-"))	{
			printf("Executing %sÉ\n", inputfrom);
			input 	 	=	fopen(inputfrom, "r");
		} else {
			inputfrom	=	0;
			input			=	stdin;
		}

		inputline	=	1;
		
		while (HellHoundOnMyTrail && (Prompt(), fgets(cmd, 80, input)))	{
#if __profile__
			ProfilerSetStatus(1);
#endif
			Dispatch(cmd);
#if __profile__
			ProfilerSetStatus(0);
#endif
			++inputline;
		}
	} while (HellHoundOnMyTrail && --argc > 0);

#if __profile__
	ProfilerDump("\pGUSI.prof");
	ProfilerTerm();
#endif
		
	printf("So long, it's been good to know you.\n");
}

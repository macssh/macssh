/*********************************************************************
File		:	GUSI				-	Grand Unified Socket Interface
File		:	GUSITest_P.h	-	Common testing gear
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:54  chombier
Initial import

Revision 1.1  1994/02/25  02:48:27  neeri
Initial revision

*********************************************************************/

#ifndef _GUSITEST_P_
#define _GUSITEST_P_

#include <CType.h>

#define NROFCHARS				26
#define DECODE(ch)			((ch) ? (ch) - (isupper(ch) ? 'A' : 'a') + 1 : 0)
#define CMDCODE(ch1,ch2)	(DECODE(ch1)*(NROFCHARS+1)+DECODE(ch2))
#define NROFCMDS				(NROFCHARS+1)*(NROFCHARS+1)

typedef struct {
	TestCmd			proc;
	const char *	syntax;
	const char *	help;
} CmdDef;

extern CmdDef commands[NROFCMDS];

#define DISPATCH(ch1,ch2)	commands[CMDCODE(ch1,ch2)].proc
#define USAGE(ch1,ch2) 		commands[CMDCODE(ch1,ch2)].syntax
#define HELPMSG(ch1,ch2) 	commands[CMDCODE(ch1,ch2)].help

#endif
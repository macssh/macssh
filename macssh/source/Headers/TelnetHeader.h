/****************************************************************
*	NCSA Telnet for the Macintosh								*
*																*
*	National Center for Supercomputing Applications				*
*	Software Development Group									*
*	152 Computing Applications Building							*
*	605 E. Springfield Ave.										*
*	Champaign, IL  61820										*
*																*
*	Copyright (c) 1986-1994,									*
*	Board of Trustees of the University of Illinois				*
****************************************************************/

#define PATTERN(x) &x	

#if defined(powerc) || defined(__powerc)
#define __powerpc__
#endif

// Using settings of NO_UNIVERSAL and __powerpc__, set up our SIMPLE_UPP macro


#define PROTO_UPP(routine, what)		\
extern what##UPP routine##UPP

// Have universal headers, compiling PPC
#ifdef	__powerpc__
#define SIMPLE_UPP(routine,what)        \
RoutineDescriptor routine##RD =	\
BUILD_ROUTINE_DESCRIPTOR(upp##what##ProcInfo,routine);\
what##UPP routine##UPP=(what##UPP)&routine##RD	\

#else	// ifdef __powerpc__

// Have universal headers, compiling 68K
#define	SIMPLE_UPP(routine,what)		\
what##UPP routine##UPP=(what##UPP)&routine

#endif	// ifdef __powerpc__

// Some random things that used to be in typedefs.h
typedef unsigned char * BytePtr;

#define	UNUSED_ARG(x)

#include <MacTCP.h>
#include "globaldefs.h"
#include "Preferences.h"

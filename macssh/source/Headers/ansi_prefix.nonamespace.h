/*  Metrowerks Standard Library */

/*  $Date$ 
 *  $Revision$ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	ansi_prefix.mac.h
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <os_enum.h>
#define 	__dest_os		__mac_os

#ifndef _MSL_IMP_EXP    		/* cc  03152000 */  
	#define _MSL_IMP_EXP 
#endif 

#define _POSIX

#define __MSL_LONGLONG_SUPPORT__
/* #define _MSL_MALLOC_0_RETURNS_NON_NULL */
/*
	Turn on _MSL_OS_DIRECT_MALLOC for a malloc alternative that simply goes
	straight to the OS with	no pooling.  Recompile the C lib when flipping
	this switch.  This will typically cause poorer performance, but may be of
	help when debugging memory problems. */
/* #define _MSL_OS_DIRECT_MALLOC */
/* #define _MSL_PRO4_MALLOC */

/* Turn on and off namespace std here */
#if defined(__cplusplus) && __embedded_cplusplus == 0
    //#define _MSL_USING_NAMESPACE
	/* Turn on support for wchar_t as a built in type */
	/* #pragma wchar_type on */   /*  vss  not implemented yet  */
#endif

/* hh 980217 

	__ANSI_OVERLOAD__ controls whether or not the prototypes in the C++ standard
	section 26.5 get added to <cmath> and <math.h> or not.  If __ANSI_OVERLOAD__
	is defined, and a C++ compiler is used, then these functions are available,
	otherwise not.
	
	There is one exception to the above rule:  double abs(double); is available
	in <cmath> and <math.h> if the C++ compiler is used.  __ANSI_OVERLOAD__ has
	no effect on the availability of this one function.

	There is no need to recompile the C or C++ libs when this switch is flipped.

	If _MSL_INTEGRAL_MATH is defined then in addition to the prototypes added by
	__ANSI_OVERLOAD__, there are also non-standard integral versions of these
	prototypes added as well.  This is to allow client code to put integral arguments
	into math functions, and avoid ambiguous call errors.
*/
/* hh 990201 turned on */
#define __ANSI_OVERLOAD__  
#define _MSL_INTEGRAL_MATH
/*
For MacHeaders
*/

/*if you are using PP or custom precompiled headers please 
	1) set MSL_USE_PRECOMPILED_HEADERS to 0
	2) make sure OLDROUTINENAMES, OLDROUTINELOCATIONS are set to false before 
	   MacHeaders is precompiled or parts of Universal Headers are included
*/

/* hh 980727 */

#ifndef OLDROUTINENAMES
	#define OLDROUTINENAMES 0
#endif

#ifndef OLDROUTINELOCATIONS
	#define OLDROUTINELOCATIONS 0
#endif
/*
#define _MWMT 1
*/
#ifndef	MSL_USE_PRECOMPILED_HEADERS 
#define MSL_USE_PRECOMPILED_HEADERS	0	/*Set to have ansi_prefix include some form of MacHeaders*/
#endif 

#if (MSL_USE_PRECOMPILED_HEADERS == 1 )
	#ifndef USE_PRECOMPILED_MAC_HEADERS
		#define	USE_PRECOMPILED_MAC_HEADERS	1  /*Set to 0 if you don't want to use precompiled MacHeaders*/
	#endif

	#if (USE_PRECOMPILED_MAC_HEADERS != 1 )
		#include 	<MacHeaders.c>
	#else
		#ifdef __cplusplus
			#if __POWERPC__
				#include <MacHeadersPPC++>
			#elif __CFM68K__
				#include <MacHeadersCFM68K++>
			#else
				#include <MacHeaders68K++>
			#endif
		#else
			#if __POWERPC__
				#include <MacHeadersPPC>
			#elif __CFM68K__
				#include <MacHeadersCFM68K>
			#else
				#include <MacHeaders68K>
			#endif
		#endif
	#endif

#endif /*MSL_USE_PRECOMPILED_HEADERS*/
/*
For ZoneRanger
	If you want malloc to register its pools with ZoneRanger, add ZoneRanger.c
	to your project.  ZoneRanger support is no longer part of MSL.
*/
/*
For DebugNew

NB: this assumes that the only placement versions of new are in the following files:
	mmemory.h, new.h, and bstring.h. DebugNew does not currently work with the 
	placement versions of operator new

1) 	add DebugNew.cp to your project

2) 	add this to New.cp
	#ifdef DebugNew_H
	#undef new
	#endif

3) to zap memory, add gDebugNewFlags |= dnDontFreeBlocks; right before your problem code

4) put DebugNewForgetLeaks() underneath this

5) put DebugNewReportLeaks() at the end of your problem code

6) uncomment lines below

7) run
*/

/*to activate debug new, uncomment the following lines*/
/*
#define DEBUG_NEW	2
#include <DebugNew.h>
#ifdef DebugNew_H
	#define new NEW
#endif
*/

#endif /*	__ansi_prefix__	  */

/*#pragma once on*/
/* Switching this pragma on, can improve compilation speed but it may cause problems with the
   use of assert.h, which generally depends upon being actually included more than once during
   compilation. */

/* Change record
mm 970110  Changed wrapper for long long support
hh 980727  Wrapped OLDROUTINENAMES and OLDROUTINELOCATIONS to prevent changing previously defined values.
mf 980811  commented out #define __ANSI_OVERLOAD__ 
hh 990201 turned __ANSI_OVERLOAD__ on because we now have foo(int) support
hh 990227 Added flag for malloc - ZoneRanger cooperation
hh  000302  Moved the namespace flag to here from mslGlobals.h
cc  000315	added _MSL_IMP_EXP
*/


#include <PLStringFuncs.h>

#if !TARGET_RT_MAC_CFM

/*********************************************************************
File		:	PLStringFuncs.c	-- C-style manipulations of Pascal strings
Author		:	Matthias Neeracher, reimplementing Apple's interface
Language	:	Metrowerks C
*********************************************************************/

#include <string.h>

pascal short PLstrcmp(ConstStr255Param str1, ConstStr255Param str2)
{
	short res;
	
	if (*str2 < *str1) {
		if (!(res = memcmp(str1+1, str2+1, *str2)))
			return 1;
	} else if (!(res = memcmp(str1+1, str2+1, *str1)))
		return -(*str1 < *str2);
		
	return res;
}

pascal short PLstrncmp(ConstStr255Param str1, ConstStr255Param str2, short num)
{
	if (num <= *str1 && num <= *str2)
		return memcmp(str1+1, str2+1, num);
	else
		return PLstrcmp(str1, str2);
}

pascal StringPtr PLstrcpy(StringPtr str1, ConstStr255Param str2)
{
	return (StringPtr) memcpy(str1, str2, *str2+1);
}

pascal StringPtr PLstrncpy(StringPtr str1, ConstStr255Param str2, short num)
{
	if (num < *str2) {
		memcpy(str1+1, str2+1, *str1 = num);
	} else {
		memcpy(str1, str2, *str2+1);
		if (*str2 < num)
			memset(str1+*str2+1, 0, num-*str2);
	}
	return str1;
}

pascal StringPtr PLstrcat(StringPtr str1, ConstStr255Param str2)
{
	memcpy(str1+*str1+1, str2+1, *str2);
	*str1 += *str2;
	
	return str1;
}

pascal StringPtr PLstrncat(StringPtr str1, ConstStr255Param str2, short num)
{
	if (*str2 < num)
		num = *str2;

	memcpy(str1+*str1+1, str2+1, num);
	*str1 += num;
	
	return str1;
}

pascal Ptr PLstrchr(ConstStr255Param str1, short ch1)
{
	return (Ptr) memchr(str1+1, ch1, *str1);
}

pascal Ptr PLstrrchr(ConstStr255Param str1, short ch1)
{
	void * found = nil;
	void * next;
	
	if ((found = PLstrchr(str1, ch1)) != NULL)
		while ((next = memchr((char *) found+1, ch1, *str1 - ((char *)found-(char *)str1))) != NULL)
			found = next;
	
	return found;
}

pascal Ptr PLstrpbrk(ConstStr255Param str1, ConstStr255Param str2)
{
	int count = *str1;
	
	while (count--)
		if (PLstrchr(str2, *++str1))
			return (Ptr) str1;
	
	return nil;
}

pascal short PLstrspn(ConstStr255Param str1, ConstStr255Param str2)
{
	ConstStr255Param s	= str1;
	int count 				= *s;
	
	while (count && PLstrchr(str2, *++str1))
		--count;
		
	return *s-count;
}

pascal Ptr PLstrstr(ConstStr255Param str1, ConstStr255Param str2)
{
	Ptr limit = (Ptr)str1+*str1-*str2+2;
	while ((Ptr)++str1 < limit)
		if (!memcmp(str1, str2+1, *str2))
			return (Ptr)str1;
	return nil;
}

pascal short PLstrlen(ConstStr255Param str)
{
	return *str;
}

pascal short PLpos(ConstStr255Param str1, ConstStr255Param str2)
{
	Ptr found = PLstrstr(str1, str2);
	
	return found ? found - (Ptr) str1 : 0;
}

#else

/*********************************************************************
This code is adapted from some public domain code written by Matthias
Neeracher.  It allows CW on PPC to call the PLString functions which are
in StdCLib.  We can't always link directly to StdCLib because some symbols
conflict with our own ANSI libraries.
*********************************************************************/

#include <CodeFragments.h>

/*********************************************************************
 * These procedure pointers will hold the glue routines to the MPW code
 */

static pascal short		(*StdCLib_PLstrcmp)(ConstStr255Param str1, ConstStr255Param str2);
static pascal short 	(*StdCLib_PLstrncmp)(ConstStr255Param str1, ConstStr255Param str2, short num);
static pascal StringPtr (*StdCLib_PLstrcpy)(StringPtr str1, ConstStr255Param str2);
static pascal StringPtr (*StdCLib_PLstrncpy)(StringPtr str1, ConstStr255Param str2, short num);
static pascal StringPtr	(*StdCLib_PLstrcat)(StringPtr str1, ConstStr255Param str2);
static pascal StringPtr (*StdCLib_PLstrncat)(StringPtr str1, ConstStr255Param str2, short num);
static pascal Ptr 		(*StdCLib_PLstrchr)(ConstStr255Param str1, short ch1);
static pascal Ptr 		(*StdCLib_PLstrrchr)(ConstStr255Param str1, short ch1);
static pascal Ptr 		(*StdCLib_PLstrpbrk)(ConstStr255Param str1, ConstStr255Param str2);
static pascal short 	(*StdCLib_PLstrspn)(ConstStr255Param str1, ConstStr255Param str2);
static pascal Ptr 		(*StdCLib_PLstrstr)(ConstStr255Param str1, ConstStr255Param str2);
static pascal short 	(*StdCLib_PLstrlen)(ConstStr255Param str);
static pascal short		(*StdCLib_PLpos)(ConstStr255Param str1, ConstStr255Param str2);


/* 
 * Initialize glue if necessary. The test is done inline in InitStandardLib, 
 * while the actual work is done in a separate procedure DoInitStandardLib.
 */
static Boolean beenThereDoneThat = false;
	
static void DoInitStandardLib()
{
	CFragConnectionID			StdCLib;
	CFragSymbolClass			symClass;
	Ptr 						whoCares;
	Str255						error;
		
	beenThereDoneThat = true;
	
	/* 
	 * We interface to the MPW standard library by dynamically connecting to the 
	 * code fragment. This technique has been suggested to me by Charlie Reiman.
	 */
	if (GetSharedLibrary(
			(StringPtr)"\pStdCLib", kCompiledCFragArch, kLoadCFrag, &StdCLib, &whoCares, error)
	)
		return;
	
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrcmp", (Ptr *) &StdCLib_PLstrcmp, &symClass))
		goto failed_on_PLstrcmp;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrncmp", (Ptr *) &StdCLib_PLstrncmp, &symClass))
		goto failed_on_PLstrncmp;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrcpy", (Ptr *) &StdCLib_PLstrcpy, &symClass))
		goto failed_on_PLstrcpy;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrncpy", (Ptr *) &StdCLib_PLstrncpy, &symClass))
		goto failed_on_PLstrncpy;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrcat", (Ptr *) &StdCLib_PLstrcat, &symClass))
		goto failed_on_PLstrcat;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrncat", (Ptr *) &StdCLib_PLstrncat, &symClass))
		goto failed_on_PLstrncat;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrchr", (Ptr *) &StdCLib_PLstrchr, &symClass))
		goto failed_on_PLstrchr;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrrchr", (Ptr *) &StdCLib_PLstrrchr, &symClass))
		goto failed_on_PLstrrchr;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrpbrk", (Ptr *) &StdCLib_PLstrpbrk, &symClass))
		goto failed_on_PLstrpbrk;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrspn", (Ptr *) &StdCLib_PLstrspn, &symClass))
		goto failed_on_PLstrspn;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrstr", (Ptr *) &StdCLib_PLstrstr, &symClass))
		goto failed_on_PLstrstr;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLstrlen", (Ptr *) &StdCLib_PLstrlen, &symClass))
		goto failed_on_PLstrlen;
	if (FindSymbol (StdCLib, (StringPtr)"\pPLpos", (Ptr *) &StdCLib_PLpos, &symClass))
		goto failed_on_PLpos;
	
	return;
	
failed_on_PLpos:
	StdCLib_PLpos = nil;
failed_on_PLstrlen:
	StdCLib_PLstrlen = nil;
failed_on_PLstrstr:
	StdCLib_PLstrstr = nil;
failed_on_PLstrspn:
	StdCLib_PLstrspn = nil;
failed_on_PLstrpbrk:
	StdCLib_PLstrpbrk = nil;
failed_on_PLstrrchr:
	StdCLib_PLstrrchr = nil;
failed_on_PLstrchr:
	StdCLib_PLstrchr = nil;
failed_on_PLstrncat:
	StdCLib_PLstrncat = nil;
failed_on_PLstrcat:
	StdCLib_PLstrcat = nil;
failed_on_PLstrncpy:
	StdCLib_PLstrncpy = nil;
failed_on_PLstrcpy:
	StdCLib_PLstrcpy = nil;
failed_on_PLstrncmp:
	StdCLib_PLstrncmp = nil;
failed_on_PLstrcmp:
	StdCLib_PLstrcmp = nil;
}

inline void InitStandardLib()
{
	if (beenThereDoneThat)
		return;
	DoInitStandardLib();
}

pascal short PLstrcmp(ConstStr255Param str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLstrcmp != nil) {
		return StdCLib_PLstrcmp (str1, str2);
	}
	return 0;
}

pascal short PLstrncmp(ConstStr255Param str1, ConstStr255Param str2, short num)
{
	InitStandardLib ();
	if (StdCLib_PLstrncmp != nil) {
		return StdCLib_PLstrncmp (str1, str2, num);
	}
	return 0;
}

pascal StringPtr PLstrcpy(StringPtr str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLstrcpy != nil) {
		return StdCLib_PLstrcpy (str1, str2);
	}
	return nil;
}

pascal StringPtr PLstrncpy(StringPtr str1, ConstStr255Param str2, short num)
{
	InitStandardLib ();
	if (StdCLib_PLstrncpy != nil) {
		return StdCLib_PLstrncpy (str1, str2, num);
	}
	return nil;
}

pascal StringPtr PLstrcat(StringPtr str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLstrcat != nil) {
		return StdCLib_PLstrcat (str1, str2);
	}
	return nil;
}

pascal StringPtr PLstrncat(StringPtr str1, ConstStr255Param str2, short num)
{
	InitStandardLib ();
	if (StdCLib_PLstrncat != nil) {
		return StdCLib_PLstrncat (str1, str2, num);
	}
	return nil;
}

pascal Ptr PLstrchr(ConstStr255Param str1, short ch1)
{
	InitStandardLib ();
	if (StdCLib_PLstrchr != nil) {
		return StdCLib_PLstrchr (str1, ch1);
	}
	return nil;
}

pascal Ptr PLstrrchr(ConstStr255Param str1, short ch1)
{
	InitStandardLib ();
	if (StdCLib_PLstrrchr != nil) {
		return StdCLib_PLstrrchr (str1, ch1);
	}
	return nil;
}

pascal Ptr PLstrpbrk(ConstStr255Param str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLstrpbrk != nil) {
		return StdCLib_PLstrpbrk (str1, str2);
	}
	return nil;
}

pascal short PLstrspn(ConstStr255Param str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLstrspn != nil) {
		return StdCLib_PLstrspn (str1, str2);
	}
	return 0;
}

pascal Ptr PLstrstr(ConstStr255Param str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLstrstr != nil) {
		return StdCLib_PLstrstr (str1, str2);
	}
	return nil;
}

pascal short PLstrlen(ConstStr255Param str)
{
	InitStandardLib ();
	if (StdCLib_PLstrlen != nil) {
		return StdCLib_PLstrlen (str);
	}
	return 0;
}

pascal short PLpos(ConstStr255Param str1, ConstStr255Param str2)
{
	InitStandardLib ();
	if (StdCLib_PLpos != nil) {
		return StdCLib_PLpos (str1, str2);
	}
	return 0;
}

#endif

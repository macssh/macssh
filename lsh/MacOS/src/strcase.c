/*  Metrowerks Standard Library  */

/*
 *	strcase.c
 *	
 *	
 *	Routines
 *	--------
 *		strcasecmp
 *		strncasecmp
 *	
 *
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#pragma ANSI_strict off
int strcasecmp(const char * str1, const char * str2)
{
#if !__POWERPC__ && !__MIPS__
	
	const	unsigned char * p1 = (unsigned char *) str1;
	const	unsigned char * p2 = (unsigned char *) str2;
				unsigned char		c1, c2;
	
	while (toupper((c1 = *p1++)) == toupper((c2 = *p2++)))
		if (!c1)
			return(0);

	return(toupper(c1) - toupper(c2));

#elif __PPC_EABI__ || __MIPS__

	/*
	 *	strcmp routine designed to minimized the number of
	 *	loads and stores.  We make sure that all the loads and
	 *  stores are properly aligned.
	 *
	 */
	register unsigned char *left = (unsigned char *)str1;
	register unsigned char *right = (unsigned char *)str2;
	unsigned int k1, k2, align, l1, r1, x;

	/*	Check the simple case of the first byte being different. */
	l1 = toupper(*left);
	r1 = toupper(*right);
	if (l1 - r1) {
		return (l1 - r1);
	}
	
	/*
	 *	If either the destination or the source are not 
	 *	aligned on the same boundary, we do a byte copy.  
	 *	Otherwise we align them to a word boundary. 
	 */ 
	if ((align = ((int)left & 3)) != ((int)right & 3)) {
		goto bytecopy;
	}
	if (align) {
		/*	Continuation of test of first byte. */
		if (l1 == 0) {
			return (0);
		}
		for (align = 3 - align; align; align--) {
			l1 = toupper(*(++left));
			r1 = toupper(*(++right));
			if (l1 - r1) {
				return (l1 - r1);
			}
			if (l1 == 0) {
				return (0);
			}
		}
		left++;
		right++;
	}

	/*	Strings are word aligned */
	
	k1 = K1;
	k2 = K2;
<<IzNoGood>>
	/*	Load a word from each string. */
	l1 = *(int*)left;
	r1 = *(int*)right;
	/*	See comments in strcpy function. */
	x = l1 + k2;
	if (x & k1) {
		goto adjust;
	}
	while(l1 == r1) {
		/*	Load a word and increment strings. */
		l1 = *(++((int*)(left)));
		r1 = *(++((int*)(right)));
		x = l1 + k2;
		if (x & k1) {
			goto adjust;
		}
	}
	/*	l1 and r1 are not equal.  */
	if (l1 > r1)
		return (1);
	return (-1);	

adjust:
	l1 = *left;
	r1 = *right;
	if (l1 - r1) {
		return (l1 - r1);
	}
bytecopy:
	if (l1 == 0) {
		return (0);
	}
	do {
		l1 = toupper(*(++left));
		r1 = toupper(*(++right));
		if (l1 - r1) {
			return (l1 - r1);
		}
		if (l1 == 0) {
			return (0);
		}
	} while (1);

#else
	
	const	unsigned char * p1 = (unsigned char *) str1 - 1;
	const	unsigned char * p2 = (unsigned char *) str2 - 1;
			unsigned long		c1, c2;
		
	while (toupper(c1 = *++p1) == toupper(c2 = *++p2))
		if (!c1)
			return(0);

	return(toupper(c1) - toupper(c2));
#endif	
}
#pragma ANSI_strict reset
int strncasecmp(const char * str1, const char * str2, size_t n)
{
#if !__POWERPC__
	
	const	unsigned char * p1 = (unsigned char *) str1;
	const	unsigned char * p2 = (unsigned char *) str2;
				unsigned char		c1, c2;
	
	n++;
	
	while (--n)
		if (toupper(c1 = *p1++) != toupper(c2 = *p2++))
			return(toupper(c1) - toupper(c2));
		else if (!c1)
			break;
	
#else
	
	const	unsigned char * p1 = (unsigned char *) str1 - 1;
	const	unsigned char * p2 = (unsigned char *) str2 - 1;
				unsigned long		c1, c2;
	
	n++;
	
	while (--n)
		if (toupper(c1 = *++p1) != toupper(c2 = *++p2))
			return(toupper(c1) - toupper(c2));
		else if (!c1)
			break;

#endif
	
	return(0);
}

/*
 *		%W%	(NCSA)	%G%
 *
 *      Virtual Screen Kernel Keys and Attribute Definitions
 *  
 *    National Center for Supercomputing Applications
 *      by Gaige B. Paulsen
 *
 *      This file contains equates used by the program for specification of
 *  special Keyboard characters and definition of the Attribute byte.
 *
 *      Version Date    Notes
 *      ------- ------  ---------------------------------------------------
 *      0.01    861102  Initial coding -GBP
 *		2.1		871130	NCSA Telnet 2.1 -GBP
 *		2.2 	880715	NCSA Telnet 2.2 -GBP
 *
 */

#ifndef __VSKEYS_H__
#define __VSKEYS_H__

/* internal codes for identifying special keys */

#define VSF10	 96		/* Function key 10 */		/* Mac Key F5 */	/* BYU 2.4.12 */
#define VSF11	 97		/* Function key 11 */		/* Mac Key F6 */	/* BYU 2.4.12 */
#define VSF12	 98		/* Function key 12 */		/* Mac Key F7 */	/* BYU 2.4.12 */
#define VSF8	 99		/* Function key  8 */		/* Mac Key F3 */	/* BYU 2.4.12 */
#define VSF13	100		/* Function key 13 */		/* Mac Key F8 */	/* BYU 2.4.12 */
#define VSF14	101		/* Function key 14 */		/* Mac Key F9 */	/* BYU 2.4.12 */
#define VSF16	103		/* Function key 16 */		/* Mac Key F11 */	/* BYU 2.4.12 */
#define VSF18	105		/* Function key 18 */		/* Mac Key F13 */	/* BYU 2.4.12 */
#define VSF19	107		/* Function key 19 */		/* Mac Key F14 */	/* BYU 2.4.12 */
#define VSF15	109		/* Function key 15 */		/* Mac Key F10 */	/* BYU 2.4.12 */
#define VSF17	111		/* Function key 17 */		/* Mac Key F12 */	/* BYU 2.4.12 */
#define VSF20	113		/* Function key 20 */		/* Mac Key F15 */	/* BYU 2.4.12 */
#define VSHELP	114		/* Help */										/* BYU 2.4.12 */
#define VSHOME	115		/* Home */										/* BYU 2.4.12 */
#define VSPGUP	116		/* Page up */									/* BYU 2.4.12 */
#define VSDEL	117		/* Del */										/* BYU 2.4.12 */
#define VSF9	118		/* Function key  9 */		/* Mac Key F4 */	/* BYU 2.4.12 */
#define VSEND	119		/* End */										/* BYU 2.4.12 */
#define VSF7	120		/* Function key  7 */		/* Mac Key F2 */	/* BYU 2.4.12 */
#define VSPGDN	121		/* Page down */									/* BYU 2.4.12 */
#define VSF6	122		/* Function key  6 */		/* Mac Key F1 */	/* BYU 2.4.12 */

#define VSF5    123     /* Function key  5 */		/* Mac Key F5 */

/* Following group is the "Cursor Control" keys */
#define VSUP    129     /* Up Arrow */
#define VSDN    130     /* Down Arrow */
#define VSRT    131     /* Right Arrow */
#define VSLT    132     /* Left Arrow */

/* Following group is the "Auxilliary Keypad" keys */
#define VSK0    133     /* Keypad 0 */
#define VSK1    134     /* Keypad 1 */
#define VSK2    135     /* Keypad 2 */
#define VSK3    136     /* Keypad 3 */
#define VSK4    137     /* Keypad 4 */
#define VSK5    138     /* Keypad 5 */	
#define VSK6    139     /* Keypad 6 */	
#define VSK7    140     /* Keypad 7 */	
#define VSK8    141     /* Keypad 8 */	
#define VSK9    142     /* Keypad 9 */
#define VSKC    143     /* Keypad , */
#define VSKM    144     /* Keypad - */
#define VSKP    145     /* Keypad . */
#define VSKE    146     /* Keypad Enter */

#define VSPF1   147     /* Function 1 */			/* Mac Keypad Clear */	
#define VSPF2   148     /* Function 2 */			/* Mac Keypad = */	
#define VSPF3   149     /* Function 3 */			/* Mac Keypad / */	
#define VSPF4   150     /* Function 4 */			/* Mac Keypad * */	

#define VSJPBKS 151     /* backslash on japanese keyboard */
#define VSJPUND 152     /* underscore on japanese keyboard */


#ifdef VSMASTER
char VSIkpxlate[2][25] =
	{
		"ABCD0123456789,-.\15PQRS\\_",
		//"ABCDpqrstuvwxylmnMPQRS\\_"
		"ABCD0123456789,-.\15PQRS|_"
	};
//char VSIkpxlate2[] = "222122?2?3?3?2?3?3123425161";		/* BYU 2.4.12 */
//char VSIkpxlate3[] = "134956?9?2?3?8?1?4~~~~0~8~7";		/* BYU 2.4.12 */
//unsigned char VSIkplen[] = 								/* BYU 2.4.12 */
//	{ 5,5,5,5,5,5,5,5,5,5,								/* BYU 2.4.12 */
//	  5,5,5,5,5,5,5,5,4,4,								/* BYU 2.4.12 */
//	  4,4,5,4,5,4,5 };									/* BYU 2.4.12 */
#else
extern char *VSIkpxlate;
//extern char *VSIkpxlate2:
//extern unsigned char *VSIkplen;							/* BYU 2.4.12 */
#endif

/*
 *          Definition of attribute bits in the Virtual Screen
 *
 *          0   - 0x000001 -  Bold
 *          1   - 0x000002 -  faint
 *          2   - 0x000004 -  italic
 *          3   - 0x000008 -  Underline
 *          4   - 0x000010 -  slowly Blinking
 *          5   - 0x000020 -  rapidly Blinking
 *          6   - 0x000040 -  Reverse
 *          7   - 0x000080 -  Graphics character set
 *			8	-    bit 0 of ansi foreground color index
 *			9	-    bit 1 of ansi foreground color index
 *			10	-    bit 2 of ansi foreground color index
 *			11	- 0x000800 - use ansi foreground color
 *			12	-    bit 0 of ansi background color index
 *			13	-    bit 1 of ansi background color index
 *			14	-    bit 2 of ansi background color index
 *			15	- 0x008000 - use ansi background color
 *			16	- 0x010000 - use 2nd ansi foreground color
 *			17	- 0x020000 - use 2nd ansi background color
 *			18	- 0x040000 - next character is part of multi-byte
 *
 */

/* character attributes */

#define VSa(x)          (1 << ((x) - 1))

#define kVSbold			0x000001
#define kVSfaint		0x000002
#define kVSitalic		0x000004
#define kVSundl			0x000008
#define kVSblnk			0x000010
#define kVSfastblnk		0x000020
#define kVSrev			0x000040
#define kVSgrph			0x000080
#define kVSansifg		0x000800
#define kVSansibg		0x008000
#define kVSansifg2		0x010000
#define kVSansibg2		0x020000
#define kVSansi2b		0x040000
// 4 bits unused, higher bit (i.e. 0x00800000) MUST be left to 0

#define VSisbold(x)     ((x) & kVSbold)
#define VSisfaint(x)    ((x) & kVSfaint)
#define VSisitalic(x)   ((x) & kVSitalic)
#define VSisundl(x)     ((x) & kVSundl)
#define VSisblnk(x)     ((x) & kVSblnk)
#define VSisfastblnk(x) ((x) & kVSfastblnk)
#define VSisrev(x)      ((x) & kVSrev)
#define VSisgrph(x)     ((x) & kVSgrph)
#define VSisansifg(x)   ((x) & kVSansifg)
#define VSisansibg(x)   ((x) & kVSansibg)
#define VSisansi(x)     ((x) & (kVSansifg | kVSansibg))
#define VSisansifg2(x)  ((x) & kVSansifg2)
#define VSisansibg2(x)  ((x) & kVSansibg2)
#define VSisansi2b(x)   ((x) & kVSansi2b)

#define VSgraph(x)  	((x) | kVSgrph)
#define VSnotgraph(x) 	((x) & ~kVSgrph)
#define VSansifg(x)  	((x) | kVSansifg)
#define VSansibg(x) 	((x) | kVSansibg)
#define VSansifg2(x)  	((x) | (kVSansifg2 | kVSansifg))
#define VSansibg2(x) 	((x) | (kVSansibg2 | kVSansibg))
#define VSansi2b(x) 	((x) | kVSansi2b)

/* line attributes */

#define kVSdecdhlt		0x0001
#define kVSdecdhlb		0x0002
#define kVSwrap			0x0004

#define VSisdecdhlt(x)  (((x) & kVSdecdhlt) && !((x) & kVSdecdhlb))
#define VSisdecdhlb(x)  (((x) & kVSdecdhlb) && !((x) & kVSdecdhlt))
#define VSisdecdwl(x)   (((x) & (kVSdecdhlt|kVSdecdhlb)) == (kVSdecdhlt|kVSdecdhlb))
#define VSisdecdwh(x)  	(((x) & (kVSdecdhlt|kVSdecdhlb)) && !VSisdecdwl(x))
//#define VSisdecdw(x)  	(((x) & (kVSdecdhlt|kVSdecdhlb)))
#define VSiswrap(x)  	((x) & kVSwrap)

#endif

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

#define VSF1    147     /* Function 1 */			/* Mac Keypad Clear */	
#define VSF2    148     /* Function 2 */			/* Mac Keypad = */	
#define VSF3    149     /* Function 3 */			/* Mac Keypad / */	
#define VSF4    150     /* Function 4 */			/* Mac Keypad * */	





#ifdef VSMASTER
char VSIkpxlate[2][23] =
	{	"ABCD0123456789,-.\15PQRS",
		"ABCDpqrstuvwxylmnMPQRS"
	};
char VSIkpxlate2[] = "222122?2?3?3?2?3?3123425161";		/* BYU 2.4.12 */
char VSIkpxlate3[] = "134956?9?2?3?8?1?4~~~~0~8~7";		/* BYU 2.4.12 */
unsigned char VSIkplen[] = 								/* BYU 2.4.12 */
	{ 5,5,5,5,5,5,5,5,5,5,								/* BYU 2.4.12 */
	  5,5,5,5,5,5,5,5,4,4,								/* BYU 2.4.12 */
	  4,4,5,4,5,4,5 };									/* BYU 2.4.12 */
#else
extern char *VSIkpxlate,*VSIkpxlate2,*VSIkpxlate2;		/* BYU 2.4.12 */
extern unsigned char *VSIkplen;							/* BYU 2.4.12 */
#endif

/*
 *          Definition of attribute bits in the Virtual Screen
 *
 *          0   -   Bold
 *          1   -   faint
 *          2   -   italic
 *          3   -   Underline
 *          4   -   slowly Blinking
 *          5   -   rapidly Blinking
 *          6   -   Reverse
 *          7   -   Graphics character set
 *			8	-	bit 0 of ansi foreground color index
 *			9	-	bit 1 of ansi foreground color index
 *			10	-	bit 2 of ansi foreground color index
 *			11	-	use ansi foreground color
 *			12	-	bit 0 of ansi background color index
 *			13	-	bit 1 of ansi background color index
 *			14	-	bit 2 of ansi background color index
 *			15	-	use ansi background color
 *
 */
#define VSa(x)			( 1 << ((x)-1) )	
#define VSisbold(x) 	((x) & 0x01)
#define VSisfaint(x) 	((x) & 0x02)
#define VSisitalic(x) 	((x) & 0x04)
#define VSisundl(x) 	((x) & 0x08)
#define VSisblnk(x) 	((x) & 0x10)
#define VSisfastblnk(x) ((x) & 0x20)
#define VSisrev(x)  	((x) & 0x40)
#define VSisgrph(x) 	((x) & 0x80)
#define VSinattr(x) 	((x) & 0xff)	

#define VSgraph(x)  	((x) | 0x80)
#define VSnotgraph(x) 	((x) & 0x7F)

#define VSisansifg(x) 	((x) & 0x0800)	
#define VSisansibg(x) 	((x) & 0x8000)	
#define VSisansi(x)		((x) & 0x8800)	

//#define VSisbold(x) (x & 0x01)
//#define VSisundl(x) (x & 0x08)
//#define VSisblnk(x) (x & 0x10)
//#define VSisrev(x)  (x & 0x40)
//#define VSisgrph(x) (x & 0x80)
//#define VSinattr(x) (x & 0xd9)

//#define VSgraph(x)  (x | 0x80)
//#define VSnotgraph(x) (x & 0x7F)

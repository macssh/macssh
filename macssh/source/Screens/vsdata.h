/*
 *      Virtual Screen Kernel Data/Structure Definitions
 *                          (vsdata.h)
 *  
 *    National Center for Supercomputing Applications
 *      by Gaige B. Paulsen
 *
 *    This file contains the control and interface calls for the NCSA
 *  Virtual Screen Kernel.
 *
 *      Version Date    Notes
 *      ------- ------  ---------------------------------------------------
 *      0.01    861102  Initial coding -GBP
 *		2.1		871130	NCSA Telnet 2.1 -GBP
 *		2.2 	880715	NCSA Telnet 2.2 -GBP
 *
 */

#ifndef	__VSDATA__
#define __VSDATA__

/* NONO */
/*#define MAXWID 132*/      /* The absolute maximum number of chars/line */
#define MAXWID 255      /* The absolute maximum number of chars/line */
/* NONO */

typedef long VSAttrib;
//typedef short VSAttrib;

typedef struct VSline
  {
	struct VSline       /* doubly-linked list of lines */
		*next,          /* Pointer to next line */
		*prev;          /* Pointer to previous line */
	short
		lattr;			/* line attributes */
	char  	// Text for the line -- may be part of a block 
		*text;          /* of memory containing more than one line */
	VSAttrib
		*attr;          /* 16 bit attribute; 8 for normal, 8 for ansi color */
	short                /* flag for memory allocation coordination */
		mem;            /* nonzero for first line in memory block */
  } VSline,*VSlinePtr,**VSlineArray;

// RAB BetterTelnet 1.2.1 - added this again for the old scrollback code
typedef struct VSattrline	//CCP 2.7 ANSI needs shorts
  {
	struct VSattrline
	  /* doubly-linked list of lines */
		*next,          /* Pointer to next line */
		*prev;          /* Pointer to previous line */
	short
		lattr;			/* line attributes */
	VSAttrib            // macblue, now use 16 bit attribute; 8 for normal, 8 for ansi color
		*text;			/* of memory containing more than one line */
	char
		*dummy;			// keep the pointer lists the same length - RAB BetterTelnet 1.2.1
	short                /* flag for memory allocation coordination */
		mem;            /* nonzero for first line in memory block */
  } VSattrline, *VSattrlinePtr, **VSattrlineArray;

#define maxparms 16
#define DEBUG_CAPTURE
struct VSscrn {
	OSType	id;				// VSCR
	short
         vtemulation;       /* 0:vt100, 1:vt220, 2:ansi, 3:linux */
    short
    	 vteightbits;		/* true if we accept vt200 8-bit codes */
    VSlinePtr
         scrntop,           /* topmost line of the current screen (= linest[0]) */
         buftop,            /* top (i e oldest line) of scrollback buffer 	*/
         vistop;            /* topmost line within visible region (may be in screen or scrollback area) */
	VSlineArray
         linest;             /* pointer to array of screen text lines        */
    VSattrlineArray
    	 attrst;			// RAB BetterTelnet 1.2.1 - added this again, too
    short
    	 lines,				/* How many lines are in the screen arrays      */
		 maxlines,          /* maximum number of lines to save off top      */
         numlines;          /* number of lines currently saved off top      */
	short
         allwidth,          /* allocated width of screen lines              */
         maxwidth,          /* current screen width setting (<= allwidth)   */
         savelines,         /* save lines off top? 0=no                     */
 		 forcesave,			/* NCSA 2.5: always force lines to be saved off top */
         possibleForce,		//we have receive a "move to 0,0"; if next is EEOl, forcesave
         ESscroll;			/* Scroll screen when ES received				*/
	short lattrib;			/* current lineattributes                       */
    VSAttrib attrib;        /* current character writing attributes         */
    short x,y;              /* current cursor positon                       */
    short Px,Py;            /* saved cursor position and writing attributes */
    VSAttrib Pattrib;       /* current character writing attributes         */
	unsigned char lastchar; /* last character displayed */
    short DECAWM,           /* Auto Wrap Mode 0=off 						*/
         DECCKM,            /* Cursor Key Mode      						*/
         DECPAM,            /* keyPad Application Mode						*/
		 DECORG,			/* origin mode                                  */
		 DECCM,				/* Bri 970610: cursor mode (1=visible, 0=invisible) */
		 G0,G1,				/* Character set identifiers 					*/
		 charset,			/* Character set mode 							*/
         IRM,               /* Insert/Replace Mode  						*/
         escflg,            /* Current Escape level							*/
         top, bottom,       /* Vertical bounds of scrolling region 			*/
         Rtop,Rbottom,		/* Vertical bounds of visible region 			*/
         Rleft,Rright,		/* Horizontal bounds of visible region 			*/
         parmptr,           /* LU - index of current parm 					*/
		 prredirect,		/* LU - printer redirection or not				*/
		 qprint,			// RAB BetterTelnet 1.0fc3: queue print requests
		 printqueued,		// RAB BetterTelnet 1.0fc3: print queue in progress
		 ignoreBeeps,		// RAB BetterTelnet 1.0fc7: ignore beeps
		 oldScrollback,		// RAB BetterTelnet 1.2.1 - use old scrollback code
		 					// (no attribute saving)
		 linesjumped,		// RAB BetterTelnet 2.0b3 - number of jump scrolled lines
		 jumpScroll,
		 disableCursor,		// RAB BetterTelnet 2.0b4 - cursor disabled to avoid screen glitch
		 realBlink;			// RAB BetterTelnet 2.0fc1 - real blinking text
	long prbuf;				/* LU - last four chars							*/
	char prbuf2;			// RAB BetterTelnet 1.2: and another char
   short refNum;			/* LU - temporary file for printer redirection	*/
	char fname[40];			/* LU - file name for temporary file			*/
    short parms[maxparms];   /* Ansi Parameters 							*/
    char *tabs;             /* pointer to array for tab settings 			*/
                            /* contains 'x' at each tab position, blanks elsewhere */
	unsigned long escflags;
	short trincount;       /* number of chars pending in translation              */
	short trinx;           /* number of characters to draw at left of cursor */
	short trintag;         /* true if waiting for a second multi-byte character */
	unsigned char trinbuf[4];
	Str255 tempstr;        /* buffer to keep partial new window name/url in vsem */

};


typedef struct VSscrn VSscrn;

struct VSscrndata {
    VSscrn *loc;            /* Location of the Screen record for this scrn */
    short
    	captureRN,	/* capture file's RefNum                       */	/* BYU 2.4.18 */
    	stat;		/* status of this screen (0=Uninitialized,     */	/* BYU 2.4.18 */
					/*                        1=In Use             */	/* BYU 2.4.18 */
					/*                        2=Initialized, but not IU */	/* BYU 2.4.18 */
};

typedef struct VSscrndata VSscrndata;

#ifdef VSMASTER
VSscrn  *VSIw;
short      VSIwn;
#else
extern VSscrn   *VSIw;
extern short       VSIwn;
#endif

#define	VSPBOTTOM	(VSIw->lines)

#endif	// __VSDATA__
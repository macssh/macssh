/*------------------------------------------------------------------------------*/
/* RSDEFS.H																		*/
/* This is the defines file for rsmac.c.  There are enough defines and structs	*/
/* to merit putting them into a separate file, so I have done so	-SMB		*/


/*-------------------------------------------------------------------------*/
/* some defines for easy structure access */
#define FHeight RSlocal[w].fheight
#define	Fascent	RSlocal[w].fascent
#define FWidth RSlocal[w].fwidth
#define	FONT RSlocal[w].fnum
#define FSIZE RSlocal[w].fsiz
#define FRight RSlocal[w].width
#define RMAXWINDOWWIDTH (width)*FWidth+16 - (CHO)
#define RMAXWINDOWHEIGHT (lines)*FHeight+16

/* the different cursor types */
#define BLOCKCURSOR				0	/* BYU 2.4.11 */
#define UNDERSCORECURSOR		1	/* BYU 2.4.11 */
#define VERTICALCURSOR			2	/* BYU 2.4.11 */
#define NOCURSOR				3
#define LOWER_THIRDCURSOR		4
#define LOWER_HALFCURSOR		5
#define TWO_THIRDSCURSOR		6

/*  Capable of shifting the text to the right some # of pixels		*/
#define CVO 0
#define CHO (-2)

#define LOCKWIDTH				16		// width of the ssh locker

#define CURS_BLINK_PERIOD		30

#define INFINITY				32000	// Will screens ever be this large?
#define WINDCOLORSIZE			4
#define ANSICOLORSIZE			16
#define PALETTESIZE				(WINDCOLORSIZE + ANSICOLORSIZE)


/*------------------------------------------------------------------------*/
/* Now we have some externs, packed away neatly from the rest of the code */
extern TelInfoRec *TelInfo;
extern short scrn;					/* shut up, Tim */
extern char *tempspot;
extern MenuHandle myMenus[];


/*--------------------------------------------------------------------------*/
/* Time to define the big RS structure.  RSdata holds the font and screen 	*/
/* specific stuff.  Basically all the ugly stuff you wouldnt otherwise 		*/
/* care to look at															*/

struct RSdata {
	OSType		id;			// RSDA
	Rect		textrect;	/* Where the text is in the window */
	Rect		cursor;		/* Cursor rectangle */
	WindowPtr	window;		/* Window pointer */
	PaletteHandle	pal;	/* My Palette */
	char		cursorstate,/* BYU 2.4.11 - 0 is invisible, 1 is visible */
				selected;	/* BYU 2.4.11 - text is selected */
	Point		last,
				anchor;
	short		topline,
				leftmarg;	/* leftmost visible column position */
	short		rheight,	/* Real window dimensions */
							/* adjusted to not include CHO boundary - TK 12/88 */
				rwidth;
	short		height,
				width;		/* Window Dimensions rounded to the nearest character */
	Boolean		flipped;	/* Set when we've reversed normal fg and bg colors */
	ControlHandle
		left,		/* The CH for the left margin */
		scroll;		/* The CH for the scroll bar */
	short
		min,		/* Minimum vertical scrollbar value (number of lines in screen buffer and scrollback) */
		max,		/* Maximum vertical scrollbar value */
		current,	/* current vertical scrollbar value */
		lmin,		/* Minimum horizontal scrollbar value (always 0) */
		lmax,		/* Maximum horizontal scrollbar value (number of columns not visible) */
		lcurrent;	/* current horizontal scrollbar value (leftmost visible column) */
	short
		fascent,	/* Font Ascent */
		fnum,		/* Font ID */
		fsiz,		/* Font Size */
		fheight,	/* Font Height/character */
		fwidth,		/* Font Width /character */
		monospaced,	/* Font is monospaced */
		allowBold,	// RAB BetterTelnet 1.0fc4 - allow boldfacing
		colorBold,	// RAB BetterTelnet 1.0fc4 - use color for boldfacing
		bfnum,		// RAB BetterTelnet 1.0fc9 - bold font ID
		bfsiz,		// RAB BetterTelnet 1.0fc9 - bold font size
		bfstyle,	// RAB BetterTelnet 1.0fc9 - bold font style
		realbold,	// RAB BetterTelnet 1.2 - use bold font for bold
		boldislarger, // to use real bold
		keepselection; // try to keep current selection when scrolling
	Boolean
		skip;		/* TRUE if we are skipping the output */
	Boolean
		active;		/* true if window is currently active */
	short
		cursType;
	RGBColor
		savedColors[16];
	Boolean
		hideScrollBars;
};

typedef struct RSdata RSdata;

#define	MYSETRECT(a,b,c,d,e)	a.left = b; \
								a.top = c;\
								a.right = d;\
								a.bottom = e;\

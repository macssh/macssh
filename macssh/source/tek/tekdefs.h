/*--------------------------------------------------------------------------*/
/* TEKDEFS -- This file contains all of the defines, and other related 		*/
/* 		stuff for TEK, bundled up into one neat package						*/
/*		First come the #defines												*/
/*--------------------------------------------------------------------------*/

#define TEK_DEVICE_MAX 2
#define SPLASH_SQUARED	4	/* NCSA: sb - used by tekrgmac.c */

#define MAXWIND 20
#define WINXMAX 4095
#define WINYMAX 3139
#define INXMAX 4096
#define INYMAX 4096

#define NUMSIZES 6 				/* number of char sizes */
#define PREDCOUNT 50

/* MORE vgtek specific stuff */
#define MAXVG		20 /* maximum number of VG windows */

/* temporary states */
#define HIY			0	/* waiting for various pieces of coordinates */
#define EXTRA		1
#define LOY			2
#define HIX			3
#define LOX			4

#define DONE		5	/* not waiting for coordinates */
#define ENTERVEC	6	/* entering vector mode */
#define CANCEL		7	/* done but don't draw a line */
#define RS			8	/* RS - incremental plot mode */
#define ESCOUT		9	/* when you receive an escape char after a draw command */
#define CMD0		50	/* got esc, need 1st cmd letter */
#define SOMEL		51	/* got esc L, need 2nd letter */
#define IGNORE		52	/* ignore next char */
#define SOMEM		53	/* got esc M, need 2nd letter */
#define IGNORE2		54
#define INTEGER		60	/* waiting for 1st integer part */
#define INTEGER1	61	/* waiting for 2nd integer part */
#define INTEGER2	62	/* waiting for 3rd (last) integer part */
#define COLORINT	70
#define GTSIZE0		75
#define GTSIZE1		76
#define	GTEXT		77	/* TEK4105 GraphText			17jul90dsw */
#define MARKER		78	/* TEK4105 Marker select		17jul90dsw */
#define	GTPATH		79	/* TEK4105 GraphText path		17jul90dsw */
#define SOMET		80
#define JUNKARRAY	81
#define STARTDISC	82
#define DISCARDING	83
#define	FPATTERN	84	/* TEK4105 FillPattern			17jul90dsw */
#define	GTROT		85	/* TEK4105 GraphText rotation	17jul90dsw */
#define GTROT1		86
#define	GTINDEX		87	/* TEK4105 GraphText color		17jul90dsw */
#define PANEL		88	/* TEK4105 Begin Panel			23jul90dsw */
#define	SUBPANEL	89	/* TEK4105 Begin^2 Panel		25jul90dsw */
#define TERMSTAT	90	/* TEK4105 Report Term Status	24jul90dsw */
#define	SOMER		91	/* TEK4105 for ViewAttributes	10jan91dsw */
#define	VIEWAT		92	/* TEK4105 ViewAttributes		10jan91dsw */
#define VIEWAT2		93

/* output modes */
#define ALPHA		0
#define DRAW		1
#define MARK		3
#define TEMPDRAW	101
#define TEMPMOVE	102
#define TEMPMARK	103

/* stroked fonts */
#define CHARWIDE	51		/* total horz. size */
#define CHARTALL	76		/* total vert. size */
#define CHARH		10		/* horz. unit size */
#define CHARV		13		/* vert. unit size */

/*--------------------------------------------------------------------------*/
/* Next come the typedefs for the various tek structures					*/
/*--------------------------------------------------------------------------*/

typedef struct TPOINT *pointlist;

typedef	struct TPOINT {
	short		x,y;
	pointlist	next;
} point/*,*pointlist*/;		/* BYU LSC */

typedef struct {
	short
		(*newwin)(void);
	char * 
		(*devname)(void);
	void 
		(*init)(void);
	short
		(*gin)(short),
		(*pencolor)(short, short),
		(*clrscr)(short),
		(*close)(short),
		(*point)(short, short, short),
		(*drawline)(short, short, short, short, short);
	void
		(*info)(short, short, short, short, short, short),
		(*pagedone)(short),
		(*dataline)(short, short, short), 
		(*charmode)(short, short, short), 
		(*gmode)(void),
		(*tmode)(void),
		(*showcur)(void),
		(*lockcur)(void),
		(*hidecur)(void),
		(*bell)(short),
		(*uncover)(short);
} RGLINK;

/*--------------------------------------------------------------------------*/
/* VGwintype structure -- this is the main high level TEK structure, where	*/
/* 		everything happens													*/
/*--------------------------------------------------------------------------*/
struct VGWINTYPE {
	OSType	id;	// VGWN
	short	RGdevice,RGnum,theVS;
	char	mode,modesave;					/* current output mode */
	char	loy,hiy,lox,hix,ex,ey;			/* current graphics coordinates */
	char	nloy,nhiy,nlox,nhix,nex,ney;	/* new coordinates */
	short	curx,cury;						/* current composite coordinates */
	short	savx,savy;						/* save the panel's x,y */
	short	winbot,wintop,winleft,winright,wintall,winwide; 
		/* position of window in virutal space */
	short	textcol;						/* text starts in 0 or 2048 */
	short	intin;							/* integer parameter being input */
	short	pencolor;						/* current pen color */
	short	fontnum,charx,chary;			/* char size */
	short	count;							/* for temporary use in special state loops */
	char	tekclear;
	char	TEKtype;						/* 4105 or 4014?  added: 16jul90dsw */
	char	TEKMarker;						/* 4105 marker type 17jul90dsw */
	char	TEKOutline;						/* 4105 panel outline boolean */
	short	TEKPath;						/* 4105 GTPath */
	short	TEKPattern;						/* 4105 Panel Fill Pattern */
	short	TEKIndex;						/* 4105 GTIndex */
	short	TEKRot;							/* 4105 GTRotation */
	short	TEKSize;						/* 4105 GTSize */
	short	TEKBackground;					/* 4105 Background color */
	pointlist	TEKPanel;					/* 4105 Panel's list of points */
	pointlist	current;					/* current point in the list */
};

/*--------------------------------------------------------------------------*/
/* RGMwindow structure -- this is the display structure for tek stuff.  It	*/
/* 		contains all the display specific info (location, scale, etc)		*/
/*--------------------------------------------------------------------------*/

struct RGMwindows {
OSType
	id;		// RGMW
GrafPtr
	wind;
short 
	xorigin,
	yorigin,
	xscale,
	yscale,
	vg,
	vs,
	ingin;
unsigned char
	*name;
short 
	width,
	height;
ControlHandle
	zoom,
	vert,
	horiz;
	};
	/* *RGMwind[ MAXWIND ];	*//* BYU - changed from an array of structure to an array of pointers */

typedef struct {
	long	thiselnum;	/* number of currently-viewing element */
	Handle	dataHandle;	/* Handle to the data */
	}	TEKSTORE, *TEKSTOREP;

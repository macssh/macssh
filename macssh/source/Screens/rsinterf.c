// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// #pragma profile on

/* rsinterf.c */

/* A split of RSmac.c to facilitate keeping my sanity --CCP */

#include "rsdefs.h"
#include "vsdata.h"
#include "vskeys.h"
#include "wind.h"
#include "rsmac.proto.h"
#include "vsinterf.proto.h"
#include "vsintern.proto.h"
#include "rsinterf.proto.h"
#include "menuseg.proto.h"
#include "maclook.proto.h"
#include "wdefpatch.proto.h"	/* 931112, ragge, NADA, KTH */
#include "parse.proto.h"
#include "network.proto.h"
#include "DlogUtils.proto.h"
#include "url.proto.h"
#include "drag.proto.h"
#include "configure.proto.h"
#include "errors.proto.h"
#include "SmartTrackControl.h"
#include "sshglue.proto.h"
#include "general_resrcdefs.h"
#include "movableModal.h"

extern void syslog( int priority, const char *format, ...);

static void calculateWindowPosition(WindRec *theScreen,Rect *whereAt, short colsHigh, short colsWide);

/* wdefpatch.c */
extern void drawicon (short id, Rect *dest);

#if GENERATINGPOWERPC
extern Boolean gHasSetWindowContentColor;
#endif

extern WindRec *screens;

extern short MaxRS;
extern RSdata *RSlocal, *RScurrent;
extern Rect	noConst;
extern short RSw;         /* last window used */
extern VSAttrib RSa;          /* last attrib used */
extern  short **topLeftCorners;
extern short	NumberOfColorBoxes;
extern short	BoxColorItems[8];
extern RGBColor	BoxColorData[8];

long RScolors[8] =	//these are the old quickdraw constants, 
{					//only used if Telinfo->hasColorQuickDraw is false 
	blackColor,		
	redColor,		
	greenColor,			
	yellowColor,	
	blueColor,		
	magentaColor,	
	cyanColor,			
	whiteColor
};


SIMPLE_UPP(ScrollProc,ControlAction);
SIMPLE_UPP(ActiveScrollProc,ControlAction);

static void HandleDoubleClick(short w, short modifiers);
static void HandleTripleClick(short w, short modifiers);

void	RSunload(void) {}


/*------------------------------------------------------------------------------*/
/* RSselect																		*/
/* Handle the mouse down in the session window.  All we know so far is that it	*/
/* is somewhere in the content window, and it is NOT an option - click.			*/
/* Double clicking now works -- SMB												*/
// And I fixed it so it works correctly.  Grrrr... - JMB
//	WARNING: Make sure RSlocal[w].selected is 1 when doing selections.  If it is
//		zero, the autoscrolling routines will seriously hose the selection drawing.
//		Heed this advice, it took me two hours to find the cause of this bug! - JMB

  /* called on a mouse-down in the text display area of the
	active window. Creates or extends the highlighted selection
	within that window, autoscrolling as appropriate if the user
	drags outside the currently visible part of the display. */
void RSselect( short w, Point pt, EventRecord theEvent)
{
	static	long 	lastClick = 0;
	static 	Point 	lastClickLoc = {0,0};
	static 	Boolean	sDblClick = false;
	GrafPtr tempwndo;
	Point	curr, temp, lastm;
	long	clickTime;
	short	shift = (theEvent.modifiers & shiftKey);
	VSAttrib attrib;


	RSsetConst(w);
	tempwndo = RSlocal[w].window;
	
	curr = normalize(pt, w, TRUE);
	clickTime = LMGetTicks();

	if  ( (EqualPt(RSlocal[w].anchor, curr) || EqualPt(RSlocal[w].anchor, RSlocal[w].last))
	   &&  clickTime - lastClick <= GetDblTime()
	   &&  EqualPt(curr, lastClickLoc) ) {
		/* NCSA: SB - check to see if this is a special click */
		/* NCSA: SB - It has to be in the right time interval, and in the same spot */
		if ( !sDblClick ) {
			curr = RSlocal[w].anchor = RSlocal[w].last = normalize(pt, w, TRUE);
			HandleDoubleClick(w, theEvent.modifiers);
			RSlocal[w].selected = 1;
// doesn't work yet. need to change WaitMouseMoved in DragText
//			sDblClick = true;
		} else {
			// unhighlight current selection
			RSinvText(w, RSlocal[w].anchor, RSlocal[w].last, &noConst);
			curr = RSlocal[w].anchor = RSlocal[w].last = normalize(pt, w, TRUE);
			HandleTripleClick(w, theEvent.modifiers);
			RSlocal[w].selected = 1;
			sDblClick = false;
		}
		lastClick = clickTime;
		lastClickLoc = curr;
	} else if (sDblClick
		   &&  clickTime - lastClick <= GetDblTime()
		   &&  EqualPt(curr, lastClickLoc) ) {
			// unhighlight current selection
			RSinvText(w, RSlocal[w].anchor, RSlocal[w].last, &noConst);
			curr = RSlocal[w].anchor = RSlocal[w].last = normalize(pt, w, TRUE);
			HandleTripleClick(w, theEvent.modifiers);
			RSlocal[w].selected = 1;
			sDblClick = false;
	} else {
		sDblClick = false;
		if ((theEvent.modifiers & cmdKey)) {
			// a command click means we should look for a url
			if ( RSTextSelected(w) && PointInSelection(curr, w) ) {
				// we have a selection already 
				HandleURL(w);
			} else {
				// we need to find the url around this pnt
				if (FindURLAroundPoint(curr, w))
					HandleURL(w);
				else
					SysBeep(1);
			}
		} else {
			lastClick = clickTime;
			lastClickLoc = curr;

			if (RSlocal[w].selected) {
				if (!shift) {
					RSlocal[w].selected = 0;
					// unhighlight current selection
					RSinvText(w, RSlocal[w].anchor, RSlocal[w].last, &noConst);
					// start new selection
					curr = normalize(pt, w, TRUE);
					RSlocal[w].last = RSlocal[w].anchor = curr;
				} else {
					RSsortAnchors(w);
					if ((curr.v < RSlocal[w].anchor.v) || ((curr.v == RSlocal[w].anchor.v) && (curr.h < RSlocal[w].anchor.h))) {
						temp = RSlocal[w].anchor;
						RSlocal[w].anchor = RSlocal[w].last;
						RSlocal[w].last = temp;
					}
				}
			} else {
				// start new selection
				curr = normalize(pt, w, TRUE);
				RSlocal[w].anchor = RSlocal[w].last = curr;
			}

			if (EqualPt(RSlocal[w].anchor, RSlocal[w].last) && RSlocal[w].anchor.h > -1) {
				if (VSgetattr(w, RSlocal[w].anchor.h - 1, curr.v, RSlocal[w].anchor.h, curr.v, &attrib, sizeof(VSAttrib))) {
					if (VSisansi2b(attrib)) {
						--RSlocal[w].anchor.h;
						--RSlocal[w].last.h;
					}
				}
			}

			while ( StillDown() ) {
				// wait for mouse position to change
				do {
					unsigned long finalTicks;
					temp = getlocalmouse(tempwndo);

					// FIXME: I'm lazy. this could be tiny.
					if (temp.v < -30) {
						// no delay
					} else if (temp.v < -20 ) {
						Delay( 2, &finalTicks);
					} else if (temp.v < -10 ) {
						Delay( 6, &finalTicks);
					} else if (temp.v < 0 ) {
						Delay( 10, &finalTicks);
					} else if (temp.v > RSlocal[w].height + 30) {
						// no delay
					} else if (temp.v > RSlocal[w].height + 20) {
						Delay( 2, &finalTicks);
					} else if (temp.v > RSlocal[w].height + 10) {
						Delay( 6, &finalTicks);
					} else if (temp.v > RSlocal[w].height) {
						Delay( 10, &finalTicks);
					}

					curr = normalize(temp, w,TRUE);
					if ( curr.h > -1 ) {
						if (VSgetattr(w, curr.h - 1, curr.v, curr.h, curr.v, &attrib, sizeof(VSAttrib))) {
							if (VSisansi2b(attrib)) {
								++curr.h;
							}
						}
					}

				} while (StillDown() && (EqualPt(curr, RSlocal[w].last) /*|| EqualPt(pt, temp)*/));

				RSlocal[w].selected = !EqualPt(RSlocal[w].anchor, curr);

				if ( !EqualPt(pt, temp) || RSlocal[w].selected ) {
					// toggle highlight state of text between current and last mouse positions
					RSinvText(w, curr, RSlocal[w].last, &noConst);
					RSlocal[w].last = curr;
					pt = temp;
				}
			}
		}
	}

	RSlocal[w].selected = !EqualPt(RSlocal[w].anchor, RSlocal[w].last);

	SetMenusForSelection((short)RSlocal[w].selected);
} // RSselect

void FlashSelection(short w)
{
	short i;
	DELAYLONG finalTick;
	for (i = 0; i < 2; i++) {
		Delay(5, &finalTick);
   	 	RSinvText(w, RSlocal[ w].anchor, RSlocal[w].last, &noConst);
		Delay(5, &finalTick);
    	RSinvText(w, RSlocal[ w].anchor, RSlocal[w].last, &noConst);
	}
}

Boolean PointInSelection(Point curr, short w)
{
  	long beg_offset, end_offset, current_offset;
  	short columns;
  	columns = VSgetcols(w);
  	beg_offset = columns*RSlocal[w].anchor.v + RSlocal[w].anchor.h;
  	end_offset = columns*RSlocal[w].last.v + RSlocal[w].last.h;
  	if (beg_offset == end_offset)
  		return FALSE;
  	current_offset = columns*curr.v + curr.h;
	if ((current_offset >= beg_offset)&&(current_offset <= end_offset))
  		return TRUE;
  	else
  		return FALSE;
}

void RSzoom
  (
	GrafPtr window, /* window to zoom */
	short code, /* inZoomIn or inZoomOut */
	short shifted /* bring to front or not */
  )
  /* called after a click in the zoom box, to zoom a terminal window. */
  {
	WStateData	**WSDhdl;
	short		w;
	short		h, v, x1, x2, y1, y2;
	short		width, lines;			// For setting Standard State before zooming
	short		top, left;				// Ditto
	
	SetPort(window);
	w = RSfindvwind(window); /* which window is it, anyway */

	width = VSmaxwidth(w) + 1;
	lines = VSgetlines(w);
	WSDhdl = (WStateData **)((WindowPeek)window)->dataHandle;
	top = (**WSDhdl).userState.top;
	left = (**WSDhdl).userState.left;
	HLock((Handle)WSDhdl);
	SetRect(&((*WSDhdl)->stdState), left, top, RMAXWINDOWWIDTH + left,
				RMAXWINDOWHEIGHT + top);
	HUnlock((Handle)WSDhdl);
	
	/* EraseRect(&window->portRect); */
	ZoomWindow(window, code, shifted);
    EraseRect(&window->portRect);			/* BYU 2.4.15 */

	/* get new window size */
	h = window->portRect.right - window->portRect.left;
	v = window->portRect.bottom - window->portRect.top;

	RSsetsize(w, v, h, -1); /* save new size settings and update scroll bars */
	/* update the visible region of the virtual screen */
	VSgetrgn(w, &x1, &y1, &x2, &y2);
	VSsetrgn(w, x1, y1, (x1 + (h - 16 + CHO) / FWidth -1),
		(y1 + (v - 16 + CVO) / FHeight - 1));
	VSgetrgn(w, &x1, &y1, &x2, &y2);		/* Get new region */
	/* refresh the part which has been revealed, if any */
	VSredraw(w, 0, 0, x2 - x1 + 1, y2 - y1 + 1); 
	/* window contents are now completely valid */
	ValidRect(&window->portRect);
  } /* RSzoom */
  
Boolean RSisInFront(short w)
{
	if (((WindowPtr)RSlocal[w].window) == FrontWindow())
		return TRUE;
	else
		return FALSE;
}


void RSdrawlocker(short w, RgnHandle visRgn)
{
	/* draw locker icon */
	if ( RSlocal[w].left  && !RSlocal[w].hideScrollBars ) {
		short sn = findbyVS(w);
		if ( sn >= 0 && screens[sn].protocol == 4 ) {
			Rect iconRect = (**RSlocal[w].left).contrlRect;
			iconRect.top += 1;
			iconRect.right = iconRect.left;
			iconRect.left = RSlocal[w].window->portRect.left;
			iconRect.bottom = iconRect.top + LOCKWIDTH;
			if ( !visRgn || RectInRgn(&iconRect, visRgn) ) {
				PlotIconID(&iconRect, kAlignNone, kTransformNone, sshicon);
			}
		}
	}
}

short
RSupdatecontent(
	GrafPtr wind,
	RgnHandle updRgn )
{
    short x1, x2, y1, y2;

	RSregnconv /* find bounds of text area needing updating */
	  (
		updRgn,
		&x1, &y1, &x2, &y2,
		RScurrent->fheight, RScurrent->fwidth
	  );

	if (x2 > x1)
	{
		VSredraw(RSfindvwind(wind), x1, y1, x2, y2); /* draw that text */
		// We must reset, less we risk looking UGLY as sin...

		BackPat(PATTERN(qd.white));
		PenPat(PATTERN(qd.black));

		if (TelInfo->haveColorQuickDraw)
		  {
			PmForeColor(0);
			PmBackColor(1);
		  }
		else
		  {
			if (!RSlocal->flipped)
			{
				ForeColor(RScolors[0]);		/* normal foreground */
				BackColor(RScolors[7]);		/* normal Background */
		  	}
		  	else	
			{
				ForeColor(RScolors[7]);		/* normal foreground */
				BackColor(RScolors[0]);		/* normal Background */
		  	}
		  }
	    RSa = -1;

	}
	return(0);
}


short RSupdate
  (
	GrafPtr wind
  )
  /* does updating for the specified window, if it's one of mine.
	Returns zero iff it is. */
  {
    short w, x1, x2, y1, y2;

    w = RSfindvwind(wind);
    if (RSsetwind(w) < 0)
		return(-1); /* not one of mine */

    BeginUpdate(wind);
	if ( !EmptyRgn(wind->visRgn) ) {

		RSupdatecontent(wind, wind->visRgn);

		DrawGrowIcon(wind);

		if (RSlocal[w].active && !RSlocal[w].hideScrollBars) {
			UpdateControls(wind, wind->visRgn);
		} else {
			ControlHandle ctrl;
			Rect ctrlRect;
			RgnHandle eraseRgn;

			BackColor(whiteColor);

			eraseRgn = NewRgn();
			if ((ctrl = RSlocal[w].scroll) != 0L) {
				ctrlRect = (**ctrl).contrlRect;
				InsetRect(&ctrlRect, 1, 1);
				RectRgn(eraseRgn, &ctrlRect);
				SectRgn(eraseRgn, wind->visRgn, eraseRgn);
				EraseRgn(eraseRgn);
			}
			if ((ctrl = RSlocal[w].left) != 0L) {
				ctrlRect = (**ctrl).contrlRect;
				InsetRect(&ctrlRect, 1, 1);
				RectRgn(eraseRgn, &ctrlRect);
				SectRgn(eraseRgn, wind->visRgn, eraseRgn);
				EraseRgn(eraseRgn);
			}
			DisposeRgn(eraseRgn);

			if (TelInfo->haveColorQuickDraw) {
				PmBackColor(1);
			} else {
				BackColor(blackColor);
			}
		}

		RSdrawlocker(w, wind->visRgn);
	}
    EndUpdate(wind);
	return(0);

  } /* RSupdate */

short RSTextSelected(short w) {		/* BYU 2.4.11 */
  return(RSlocal[w].selected);	/* BYU 2.4.11 */
}								/* BYU 2.4.11 */

void RSskip
  (
	short w,
	Boolean on
  )
  /* sets the "skip" flag for the specified window (whether ignore
	screen updates until further notice). */
  {
	RSlocal[w].skip = on;
  } /* RSskip */


/*
 *  This routine is called when the user presses the grow icon, or when the size of
 *  the window needs to be adjusted (where==NULL, modifiers==0).
 *  It limits the size of the window to a legal range.
 */

short RSsize (GrafPtr window, long *where, long modifiers)
{
	Rect	SizRect;
	long	size;
	short	w, width, lines;
	short	tw, h, v, x1, x2, y1, y2, th;
	Boolean	changeVSSize = false;
	short	sn = 0;
	Boolean	snValid = false;
	short 	err = noErr;
	short	cwidth;
	short		oldlines;
	short		oldcols;

	if ((w = RSfindvwind(window)) < 0)	/* Not found */
		return (0);
	
	if (modifiers & cmdKey) return (0);
	
	snValid = (sn = findbyVS(w)) >= 0;

/* NONO */
	/* inverted window-resize behaviour */
	/*changeVSSize = (modifiers & optionKey) == optionKey;*/
	changeVSSize = (modifiers & optionKey) == 0;
/* NONO */
#define DONT_DEFAULT_CHANGE_VS_IF_NAWS				// JMB
	// 931112, ragge, NADA, KTH 
	// I think this is the way it should work, if there is naws available it
	// should be used by default, and option toggles behaviour.
	// Maybe it should be user configurable?
#ifndef DONT_DEFAULT_CHANGE_VS_IF_NAWS
	if(snValid && screens[sn].naws) {
/* NONO */
		/* inverted window-resize behaviour */
		/*changeVSSize = (modifiers & optionKey) != optionKey;*/
		changeVSSize = (modifiers & optionKey) == 0;
/* NONO */
	}
#endif

	SetPort(window);

	width = VSmaxwidth(w) + 1; //VSmaxwidth returns one less than number of columns
	lines = VSgetlines(w);


	if (changeVSSize) {
		th = INFINITY;
		tw = INFINITY-1;
		}
	else {
		tw = RMAXWINDOWWIDTH;
		th = RMAXWINDOWHEIGHT + 1;
		}

	SetRect(&SizRect, 48, 48, tw + 1, th);
	
	if (where)											/* grow icon actions */
		{							
		if (changeVSSize) { /* 931112, ragge, NADA, KTH */
			setupForGrow(window, 1 - CHO, 1 - CVO, FWidth, FHeight);
		}
		size = GrowWindow(window, *(Point *) where, &SizRect);	/* BYU LSC */
		if (changeVSSize) { /* 931112, ragge, NADA, KTH */
			cleanupForGrow(window);
		}

		if (size != 0L)
		  {
			SizeWindow(window, size & 0xffff, (size >> 16) & 0xffff, FALSE);
			h = window->portRect.right - window->portRect.left;
			v = window->portRect.bottom - window->portRect.top;
		  }
		else return(0);							/* user skipped growing */
	  }
	else
	  {									/* just resize the window */
		h = window->portRect.right - window->portRect.left;	/* same width */
		v = (FHeight) * (VSgetlines(w));					/* new height */
		SizeWindow(window, h, v, FALSE);					/* change it */
		} 	

	RSsetsize(w, v, h, sn); /* save new size settings and update scroll bars */

  /* update the visible region of the virtual screen */

	VSgetrgn(w, &x1, &y1, &x2, &y2);
	VSsetrgn(w, x1, y1, (short)((x1 + (h - 16 + CHO) / FWidth - 1)),
		(short)((y1 + (v - 16) / FHeight - 1)));
	VSgetrgn(w, &x1, &y1, &x2, &y2);		/* Get new region */

	DrawControls(window);


	if (changeVSSize) {
		
		oldlines = VSgetlines(w);
		oldcols = VSgetcols(w);

		switch (VSsetlines(w,y2 -y1 +1)) {


		case (-4000): //can't even get enough memory to put VS back to original size
			/* signal this to main program */
			return(-4);
			break;
		
		case (-3000): //no resize: unkown problems, but we put the VS back to original size
			return(0);
			break;
		case (-2000): //no resize: Memory problems, but we put the VS back to original size
			return(-2);
			break;
		default:	//Ok, we can resize; tell host
			cwidth = x2 - x1 + 1;
			if ( cwidth > 255 ) {
				cwidth = 255;
			}
			RScalcwsize(w,cwidth);
			if (snValid
			 && (oldlines != VSgetlines(w) || oldcols != VSgetcols(w)) ) {
				if (screens[sn].naws) {
					SendNAWSinfo(&screens[sn], cwidth, (y2-y1+1));
				}
				if (screens[sn].protocol == 4) {
					ssh_glue_wresize(&screens[sn]);
				}
			}
			return (0);
			break;
		}
	}

	VSredraw(w, 0, 0, x2 - x1 + 1, y2 - y1 + 1);		/* refresh the part which has been revealed, if any */
	ValidRect(&window->portRect);						/* window contents are now completely valid */

	return (0);
  } /* RSsize */
  
void RSshow( short w)		/* reveals a hidden terminal window. */
{
	VSscrn *theVS;
	if (RSsetwind(w) < 0)
		return;
	theVS = VSwhereis(w);
	RSa = -1;
	VSredraw(w, 0, 0, theVS->maxwidth, theVS->lines);
	ShowWindow(RScurrent->window);
}

/*
 * RSresetcolors:  back to default ANSI colors
 */

void RSresetcolors( short w )
{
	int		i;

    if ( !TelInfo->haveColorQuickDraw || RSsetwind(w) < 0 )
		return;

	for ( i = 0; i < ANSICOLORSIZE; i++ ) {
		SetEntryColor(RScurrent->pal, i + WINDCOLORSIZE, &RScurrent->savedColors[i]);
	}
}

/*
 * RSsetcolors:  change one of the window/ANSI colors
 */

Boolean RSsetcolors
	(
	short w, /* window number */
	short n, /* color entry number */
	RGBColor *color
	)
  /* sets a new value for the specified color entry of a terminal window. */
  {
    if ( !TelInfo->haveColorQuickDraw || RSsetwind(w) < 0 || n < 0 || n >= PALETTESIZE )
		return FALSE;

	SetEntryColor(RScurrent->pal, n, color);

	if ( n == 1 ) {
		/* set background color */
#if GENERATINGPOWERPC
		if (gHasSetWindowContentColor ) {
			SetWindowContentColor(RScurrent->window, color);
		} else
#endif
		{
			WCTabHandle colorTable = (WCTabHandle)myNewHandle(sizeof(WinCTab));
			if (colorTable != NULL) {
				(**colorTable).wCSeed = 0;
				(**colorTable).wCReserved = 0;
				(**colorTable).ctSize = 0;
				(**colorTable).ctTable[0].value = wContentColor;
				(**colorTable).ctTable[0].rgb = *color;
				SetWinColor(RScurrent->window, colorTable);
				/* heap becomes corrupted after CloseWindow() */
				/* if I call DisposeHandle... but no leak ? */
				/*DisposeHandle((Handle)colorTable);*/
			}
		}
	}

	SetPort(RScurrent->window);
	InvalRect(&RScurrent->window->portRect);
	return(TRUE);
  } /* RSsetcolors */
  
void RSsendstring
  (
	short w, /* which terminal window */
	char *ptr, /* pointer to data */
	short len /* length of data */
  )
  /* sends some data to the host along the connection associated
	with the specified window. */
{
	short sn;
 	WindRecPtr tw;

	sn = findbyVS(w);
	if ( sn < 0 )
		return;
	tw = &screens[sn];
	netpush(tw->port);				/* BYU 2.4.18 - for Diab systems? */		
	netwrite(tw->port, ptr, len);
} /* RSsendstring */



short RSnewwindow
  (
	RectPtr 	wDims,
	short scrollback, /* number of lines to save off top */
	short width, /* number of characters per text line (80 or 132) */
	short lines, /* number of text lines */
	StringPtr name, /* window name */
	short fnum, /* ID of font to use initially */
	short fsiz, /* size of font to use initially */
  	short screenNumber,
  	short bfnum,
  	short bfsiz,
  	short bfstyle,
  	short vtemulation,
	unsigned long flags
  )
  /* creates a virtual screen and a window to display it in. */
{
	GrafPort gp; /* temp port for getting text parameters */
	short w;

	Rect		pRect;
	short		wheight, wwidth;
	WStateData	*wstate;
	WindowPeek	wpeek;
	CTabHandle	ourColorTableHdl;
	int			i;
	WindowPeek	front;
	WindowPtr	behind;

	/* create the virtual screen */
	w = VSnewscreen(vtemulation, scrollback, (scrollback != 0), lines, width, flags);
	if (w < 0) {		/* problems opening the virtual screen -- tell us about it */
		return(-1);
	}

	RScurrent = RSlocal + w;

	RScurrent->fnum = fnum;
	RScurrent->fsiz = fsiz;
	RScurrent->bfnum = bfnum;
	RScurrent->bfsiz = bfsiz;
	RScurrent->bfstyle = bfstyle;

	OpenPort(&gp);
	RScurrent->allowBold = flags & RSWallowBold;
	RScurrent->colorBold = flags & RSWcolorBold;
	RScurrent->realbold = flags & RSWrealbold;
	RScurrent->keepselection = flags & RSWKeepSelection;
	RSTextFont(fnum,fsiz,0);	/* BYU */
	TextSize(fsiz);
	RSfontmetrics();
	ClosePort(&gp);
	 
	if (wDims->bottom == 0)
		calculateWindowPosition(&screens[screenNumber],wDims,lines,width);

	if ((wDims->right - wDims->left) > RMAXWINDOWWIDTH)
		wDims->right = wDims->left + RMAXWINDOWWIDTH;
	if ((wDims->bottom - wDims->top) > RMAXWINDOWHEIGHT)
		wDims->bottom = wDims->top + RMAXWINDOWHEIGHT;
	wwidth = wDims->right - wDims->left;
	wheight = wDims->bottom - wDims->top;

	if (!RectInRgn(wDims,TelInfo->greyRegion)) //window would be offscreen
		calculateWindowPosition(&screens[screenNumber],wDims,lines,width);

	/* leave dialogs in front */
	behind = kInFront;
	front = (WindowPeek)LMGetWindowList();
	while ( front && (!front->visible || RSfindvwind((WindowPtr)front) < 0) ) {
		behind = (WindowPtr)front;
		front = front->nextWindow;
	}

  /* create the window */
	if (!TelInfo->haveColorQuickDraw) {
		RScurrent->window = NewWindow(0L, wDims, name, (flags & RSWshowit) != 0, 8,behind, (flags & RSWgoaway) != 0, (long)w);
		RScurrent->pal = NULL;
		if (RScurrent->window == NULL) {
			VSdestroy(w);
			return(-2);
		}
	} else {
		if ( TelInfo->AnsiColors == NULL ) {
			VSdestroy(w);
			return(-2);
		}

		RScurrent->window = NewCWindow(0L, wDims, name, (flags & RSWshowit) != 0, 8,behind, (flags & RSWgoaway) != 0, (long)w);
		if (RScurrent->window == NULL) {
			VSdestroy(w);
			return(-2);
		}

		//note: the ANSI colors are in the top 8 of the palette.  The four telnet colors (settable
		//in telnet) are in the lower 4 of the palette.  These 4 are set later by a call from 
		//CreateConnectionFromParams to RSsetColor (ick, but I am not going to add 4 more params to
		//this ungodly function call (CCP 2.7)
		ourColorTableHdl = (CTabHandle) myNewHandle((long) (sizeof(ColorTable) + 
									(PALETTESIZE - 1) * sizeof(CSpecArray)));
		if (ourColorTableHdl == NULL) 
		{
			DisposeWindow(RScurrent->window);
			VSdestroy(w);
			return(-2);
		}
		HLock((Handle) ourColorTableHdl);
			
		(*ourColorTableHdl)->ctSize = PALETTESIZE - 1;		// Number of entries minus 1
		(*ourColorTableHdl)->ctFlags = 0;
		
		for (i = 0; i < WINDCOLORSIZE; i++) // set the ctTable.value field to zero for our four
			(*ourColorTableHdl)->ctTable[i].value = 0;
		
		for (i = 0; i < ANSICOLORSIZE; i++) {
			// get the ANSI colors from the palette
			GetEntryColor(TelInfo->AnsiColors, i, &(*ourColorTableHdl)->ctTable[i + WINDCOLORSIZE].rgb);
			(*ourColorTableHdl)->ctTable[i + WINDCOLORSIZE].value = 0;
			RScurrent->savedColors[i] = (*ourColorTableHdl)->ctTable[i + WINDCOLORSIZE].rgb;
		}

		RScurrent->pal = NewPalette(PALETTESIZE, ourColorTableHdl, pmCourteous, 0);

		DisposeHandle((Handle)ourColorTableHdl);

		if (RScurrent->pal == NULL) {
			DisposeWindow(RScurrent->window);
			VSdestroy(w);
			return(-2);
		}
		SetPalette(RScurrent->window, RScurrent->pal, TRUE); //copy the palette to the window
	} 

	SetPort(RScurrent->window);
	SetOrigin(CHO, CVO);			/* Cheap way to correct left margin problem */

	wpeek = (WindowPeek) RScurrent->window;

	HLock(wpeek->dataHandle);
	wstate = (WStateData *) *wpeek->dataHandle;


	BlockMoveData(&wstate->userState, wDims, 8);
	pRect.top = wDims->top;
	pRect.left = wDims->left;
	pRect.right = pRect.left + RMAXWINDOWWIDTH;
	if (pRect.right > TelInfo->screenRect.right)
		pRect.right = TelInfo->screenRect.right;

	pRect.bottom = pRect.top + RMAXWINDOWHEIGHT;

	RScurrent->hideScrollBars = ((flags & RSWHideScroll) != 0);

  /* create scroll bars for window */
	pRect.top = -1 + CVO;
	pRect.bottom = wheight - 14 + CVO;
	pRect.left = wwidth - 15 + CHO;
	pRect.right = wwidth + CHO;
	RScurrent->scroll = NewControl(RScurrent->window, &pRect, "\p", FALSE,	/* BYU LSC */
		0, 0, 0, 16, 1L);

	if (RScurrent->scroll == 0L) return(-3);

	if ( screens[screenNumber].protocol == 4 ) {
		i = LOCKWIDTH + 1;
	} else {
		i = 0;
	}
	pRect.top = wheight - 15 + CVO;
	pRect.bottom = wheight + CVO;
	pRect.left = -1 + CHO + i;
	pRect.right = wwidth - 14 - i + CHO;
	RScurrent->left = NewControl(RScurrent->window, &pRect, "\p", FALSE,		/* BYU LSC */
		0, 0, 0, 16, 1L);

	if (RScurrent->left == 0L) return(-3);

	RScurrent->skip = 0; /* not skipping output initially */
	RScurrent->max = 0; /* scroll bar settings will be properly initialized by subsequent call to VSsetrgn */
	RScurrent->min = 0;
	RScurrent->current = 0;
	RScurrent->lmax = 0;
	RScurrent->lmin = 0;
	RScurrent->lcurrent = 0;
	RScurrent->selected = 0;	/* no selection initially */
	RScurrent->cursorstate = 0;	/* BYU 2.4.11 - cursor off initially */
	RScurrent->flipped = 0;		/* Initially, the color entries are not flipped */
	RSsetsize(w, wheight, wwidth, screenNumber);

	RScurrent->active = 0;
	RScurrent->cursType = gApplicationPrefs->CursorType;

	RSTextFont(RScurrent->fnum,RScurrent->fsiz,0);	/* BYU LSC */
	TextSize(RScurrent->fsiz);				/* 9 point*/
	if (!TelInfo->haveColorQuickDraw)
		TextMode(srcXor);			/* Xor mode*/
	else
		TextMode(srcCopy);

	if (flags & RSWwrapon)
	  /* turn on autowrap */
		VSwrite(w, "\033[?7h",5);

	return(w);
  } /* RSnewwindow */

short RSmouseintext				/* Point is in global coords */
  (
	short w,
	Point myPoint
  )
  /* is myPoint within the text-display area of the specified window. */
  {
	return
		PtInRect(myPoint, &RSlocal[w].textrect); 	/* BYU LSC */
  } /* RSmouseintext */

void RSkillwindow
  (
	short w
  )
  /* closes a terminal window. */
  {
  	short sn;
 	WindRecPtr tw;
 	RSdata *temp;

 	sn = findbyVS(w);
 	if ( sn < 0 )
 		return;
	tw = &screens[sn];

 	--((*topLeftCorners)[tw->positionIndex]); //one less window at this position

 	temp = RSlocal + w;
 	if (temp->pal != NULL) {
 		DisposePalette(temp->pal);		
 		temp->pal = NULL;
		}
 
	VSdestroy(w);		/* destroy the virtual screen */
	KillControls(RSlocal[w].window);  /* Get rid of those little slidy things */
	DisposeWindow(RSlocal[w].window);	/* Get rid of the actual window */
	RSlocal[w].window = 0L;
	RSw = -1;
  }

void RSgetcolors
  (
	short w, /* window number */
	short n, /* color entry number */
	RGBColor *color /* output color */
  )
  /* gets the current value for the specified color entry of a terminal window. */
  {
	GetEntryColor(RSlocal[w].pal,n,color);  
  } /* RSgetcolors */

void	RShide( short w)		/* hides a terminal window. */
{
	if (RSsetwind(w) < 0)
		return;
	
	HideWindow(RScurrent->window);
}

GrafPtr RSgetwindow
  (
	short w
  )
  /* returns a pointer to the Mac window structure for the
	specified terminal window. */
  {
    return(RSlocal[w].window);
  } /* RSgetwindow */

char **RSGetTextSel
  (
	short w, /* window to look at */
	short table, /* nonzero for "table" mode, i e
		replace this many (or more) spaces with a single tab. */
	short clipspaces
  )
  /* returns the contents of the current selection as a handle,
	or nil if there is no selection. */
  {
	char **charh, *charp;
	short maxwid;
	long realsiz;
	Point Anchor,Last;

	if (!RSlocal[w].selected)
		return(0L);	/* No Selection */
	maxwid = VSmaxwidth(w);
	Anchor = RSlocal[w].anchor;
	Last = RSlocal[w].last;
	
	realsiz = Anchor.v - Last.v;
	if (realsiz < 0)
		realsiz = - realsiz;
	realsiz ++;								/* lines 2,3 selected can be 2 lines */
	realsiz *= (maxwid * 2 + 2);
	charh = myNewHandle(realsiz);
	if (charh == 0L)
		return((char **) -1L);				/* Boo Boo return */
	HLock((Handle)charh);
	charp = *charh;
	realsiz = VSgettext(w, Anchor.h, Anchor.v, Last.h, Last.v,
		charp, realsiz, "\015", table, clipspaces);
	HUnlock((Handle)charh);
	mySetHandleSize((Handle)charh, realsiz);
	return charh;
  }  /* RSGetTextSel */

extern void VSprintf(char *fmt, ...);

char **RSGetTextScreen
  (
	short w, /* window to look at */
	short table, /* nonzero for "table" mode, i e
		replace this many (or more) spaces with a single tab. */
	short clipspaces
  )
  /* returns the contents of the visible on-screen text as a handle */
  {
	char **charh, *charp;
	short maxwid;
	long realsiz;
	Point Anchor,Last;

	maxwid = VSmaxwidth(w);
	VSgetrgn(w, &Anchor.h, &Anchor.v, &Last.h, &Last.v);
	Anchor.h = -1;
	realsiz = Anchor.v - Last.v;
	if (realsiz < 0)
		realsiz = - realsiz;
	realsiz ++;								/* lines 2,3 selected can be 2 lines */
	realsiz *= (maxwid * 2 + 2);
	charh = myNewHandle(realsiz);
	if (charh == 0L)
		return((char **) -1L);				/* Boo Boo return */
	HLock((Handle)charh);
	charp = *charh;
	realsiz = VSgettext(w, Anchor.h, Anchor.v, Last.h, Last.v,
		charp, realsiz, "\015", table, clipspaces);
	HUnlock((Handle)charh);
	mySetHandleSize((Handle)charh, realsiz);
	return charh;
}  /* RSGetTextScreen */


RgnHandle RSGetTextSelRgn(short w)
{
	Rect	temp, temp2;
	Point	lb, ub;
	Point	curr;
	Point	last;
	RgnHandle	rgnH, tempRgn;

	rgnH = NewRgn();
	if (rgnH == nil) {
		return nil;
		}

	tempRgn = NewRgn();
	if (tempRgn == nil) {
		DisposeRgn(rgnH);
		return nil;
		}

	RSsetwind(w);
	RSsetConst(w);

	curr = RSlocal[w].anchor;
	last = RSlocal[w].last;

  /* normalize coordinates with respect to visible area of virtual screen */
	curr.v -= RScurrent->topline;
	curr.h -= RScurrent->leftmarg;
	last.v -= RScurrent->topline;
	last.h -= RScurrent->leftmarg;

	if (curr.v == last.v)
	  {
	  /* highlighted text all on one line */
		if (curr.h < last.h) /* get bounds the right way round */
		  {
			ub = curr;
			lb = last;
		  }
		else
		  {
			ub = last;
			lb = curr;
		  } /* if */
		MYSETRECT /* set up rectangle bounding area to be highlighted */
		  (
			temp,
			(ub.h + 1) * RScurrent->fwidth,
			ub.v * RScurrent->fheight,
			(lb.h + 1) * RScurrent->fwidth,
			(lb.v + 1) * RScurrent->fheight
		  );
		SectRect(&temp, &noConst, &temp2); /* clip to constraint rectangle */
		RectRgn(rgnH, &temp2);
	  }
	else
	  {
	  /* highlighting across more than one line */
		if (curr.v < last.v)
			ub = curr;
		else
			ub = last;
		if (curr.v > last.v)
			lb = curr;
		else
			lb = last;
		MYSETRECT /* bounds of first (possibly partial) line to be highlighted */
		  (
			temp,
			(ub.h + 1) * RScurrent->fwidth,
			ub.v * RScurrent->fheight,
			RScurrent->width,
			(ub.v + 1) * RScurrent->fheight
		  );
		SectRect(&temp, &noConst, &temp2); /* clip to constraint rectangle */
		RectRgn(rgnH, &temp2);

		MYSETRECT /* bounds of last (possibly partial) line to be highlighted */
		  (
			temp,
			0,
			lb.v * RScurrent->fheight,
			(lb.h + 1) * RScurrent->fwidth,
			(lb.v + 1) * RScurrent->fheight
		  );
		SectRect(&temp, &noConst, &temp2); /* clip to constraint rectangle */
		RectRgn(tempRgn, &temp2);
		UnionRgn(tempRgn, rgnH, rgnH);

		if (lb.v - ub.v > 1) /* highlight extends across more than two lines */
		  {
		  /* highlight complete in-between lines */
			SetRect
			  (
				&temp,
				0,
				(ub.v + 1) * RScurrent->fheight,
				RScurrent->width,
				lb.v * RScurrent->fheight
			  );
			SectRect(&temp, &noConst, &temp2); /* clip to constraint rectangle */
			RectRgn(tempRgn, &temp2);
			UnionRgn(tempRgn, rgnH, rgnH);

		  } /* if */
	  } /* if */

	DisposeRgn(tempRgn);
	
	return rgnH;
}

short RSfindvwind
  (
	GrafPtr wind
  )
  /* returns the number of the virtual screen associated with
	the specified window, or -4 if not found. */
  {
    short
		i = 0;
    while ((RSlocal[i].window != wind) && (i < MaxRS))
		i++;
    if ((RSlocal[i].window == 0L) || (i >= MaxRS))
		return(-4);
	else
		return(i);
  } /* RSfindvwind */


/*
 * RSactivate
 *
 * handles an activate event for the specified window
 */

void RSactivate( short w )
{
	RSsetConst(w);
	/* display the grow icon */
	DrawGrowIcon(RSlocal[w].window);
	/* and activate the scroll bars */

	if ( !RSlocal[w].hideScrollBars ) {
		if (RSlocal[w].scroll != 0L) {
			ShowControl(RSlocal[w].scroll);
		}
		if (RSlocal[w].left != 0L) {
			ShowControl(RSlocal[w].left);
		}
	}

	RSlocal[w].active = 1;

	if ( gApplicationPrefs->BlinkCursor ) {
    	TelInfo->blinktime = LMGetTicks() - CURS_BLINK_PERIOD;
	}
} /* RSactivate */


/*
 * RSdeactivate
 *
 * handles a deactivate event for the specified window
 */

void RSdeactivate( short w )
{
	GrafPtr port;
	GetPort(&port);
	SetPort(RSlocal[w].window);

	RSsetConst(w);

	RSlocal[w].active = 0;

	if ( gApplicationPrefs->BlinkCursor ) {
		RScursoff( w );
	}

	BackColor(whiteColor);

	if ( !RSlocal[w].hideScrollBars ) {
	  	/* deactivate the scroll bars */
		if (RSlocal[w].scroll != 0L) {
			HideControl(RSlocal[w].scroll);
		}
		if (RSlocal[w].left != 0L) {
			HideControl(RSlocal[w].left);
		}
	}

	/* update the appearance of the grow icon */
	DrawGrowIcon(RSlocal[w].window); 

	if (TelInfo->haveColorQuickDraw) {
		PmBackColor(1);
	} else {
		BackColor(blackColor);
	}

 	SetPort(port);
} /* RSdeactivate */



/*
 * RSsetcurstype
 */

void RSsetcurstype( short w, long cursType )
{
	if ( VSvalids(w) != 0 )
		return;
	if ( VSIcursorvisible() && RSlocal[w].cursorstate )
		RScursoff( w );
	RSlocal[w].cursType = cursType;
} /* RSsetcurstype */


/*
 * RScursblink
 */

void RScursblink( short w )
{
	unsigned long	now;

	if (!gApplicationPrefs->BlinkCursor
	 || VSvalids(w)
	 || !VSIcursorvisible()
	 || ((!RSlocal[w].active
	 ||   TelInfo->suspended)
	  && !RSlocal[w].cursorstate))
		return;
	if ( (now = LMGetTicks()) - TelInfo->blinktime >= CURS_BLINK_PERIOD ) {
		GrafPtr savePort;
		GetPort(&savePort);
		TelInfo->blinktime = now;
		RSlocal[w].cursorstate ^= 1;
		SetPort(RSlocal[w].window);
		InvertRect(&RSlocal[w].cursor);
		SetPort(savePort);
	}
} /* RScursblink */


/*
 * RScursblinkon
 */

void RScursblinkon( short w )
{
	if (!gApplicationPrefs->BlinkCursor
	 || VSvalids(w)
	 || !VSIcursorvisible()
	 || !RSlocal[w].active
	 || TelInfo->suspended)
  		return;
	TelInfo->blinktime = LMGetTicks();
  	if ( !RSlocal[w].cursorstate ) {
		GrafPtr savePort;
		GetPort(&savePort);
		RSlocal[w].cursorstate = 1;
		SetPort(RSlocal[w].window);
		InvertRect(&RSlocal[w].cursor);
		SetPort(savePort);
	}
} /* RScursblinkon */


/*
 * RScursblinkoff
 */

void RScursblinkoff( short w )
{

	if (!gApplicationPrefs->BlinkCursor || VSvalids(w) || !VSIcursorvisible())
		return;
  	if ( RSlocal[w].cursorstate ) {
		GrafPtr savePort;
		GetPort(&savePort);
		RSlocal[w].cursorstate = 0;
		TelInfo->blinktime = LMGetTicks() - CURS_BLINK_PERIOD;
		SetPort(RSlocal[w].window);
		InvertRect(&RSlocal[w].cursor);
		SetPort(savePort);
	}
} /* RScursblinkoff */


void	RScprompt(short w)
  /* puts up the dialog that lets the user examine and change the color
	settings for the specified window. */
{
	short			scratchshort, ditem;
	Point			ColorBoxPoint;
	DialogPtr		dptr;
	Boolean			UserLikesNewColor;
	RGBColor		scratchRGBcolor;
	short			itemType;
	Handle			itemHandle;
	Rect			itemRect;
	
	SetUpMovableModalMenus();
	dptr = GetNewMySmallDialog(ColorDLOG, NULL, kInFront, (void *)ThirdCenterDialog);

	for (scratchshort = 0, NumberOfColorBoxes = 4; scratchshort < NumberOfColorBoxes; scratchshort++)
	{
		RSgetcolors(w,scratchshort, &scratchRGBcolor);
		BoxColorItems[scratchshort] = ColorNF + scratchshort;
		BlockMoveData(&scratchRGBcolor,&BoxColorData[scratchshort], sizeof(RGBColor));
		UItemAssign( dptr, ColorNF + scratchshort, ColorBoxItemProcUPP);
	}
		
	ColorBoxPoint.h = 0;			// Have the color picker center the box on the main
	ColorBoxPoint.v = 0;			// screen
	
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 2);
	//SetDialogTracksCursor(dptr, 1);

	ShowWindow(dptr);

	ditem = 3;	
	while (ditem > 2) {
		/*ModalDialog(ColorBoxModalProcUPP, &ditem);*/
		movableModalDialog(ColorBoxModalProcUPP, &ditem);

		switch (ditem) {
			case	ColorNF:	
			case	ColorNB:	
			case	ColorBF:	
			case	ColorBB:	
				if (TelInfo->haveColorQuickDraw) {
					Str255 askColorString;
					GetIndString(askColorString,MISC_STRINGS,PICK_NEW_COLOR_STRING);
					UserLikesNewColor = GetColor(ColorBoxPoint, askColorString,
						 &BoxColorData[ditem-ColorNF], &scratchRGBcolor);
					if (UserLikesNewColor)
						BoxColorData[ditem-ColorNF] = scratchRGBcolor;
						// force refresh
						GetDialogItem(dptr, ditem, &itemType, &itemHandle, &itemRect);
						InvalRect(&itemRect);
					}
				break;
				
			default:
				break;
			
			} // switch
		} // while

	if (ditem == DLOGCancel) {
		DisposeDialog(dptr);
		ResetMenus();
		return;
		}
		
	for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++) 
			RSsetcolors(w,scratchshort, &BoxColorData[scratchshort]);
	
	/* force redrawing of entire window contents */
	SetPort(RSlocal[w].window);
	InvalRect(&RSlocal[w].window->portRect);

	DisposeDialog(dptr);
	ResetMenus();
} /* RScprompt */

/*------------------------------------------------------------------------------*/
/* NCSA: SB - RScalcwsize 														*/
/* 		This routine is used to switch between 80 and 132 column mode. All that	*/	
/* 		is passed in is the RS window, and the new width.  This calculates the	*/	
/* 		new window width, resizes the window, and updates everything.  - SMB	*/
/*------------------------------------------------------------------------------*/
void RScalcwsize(short w, short width)
{
	short x1,x2,y1,y2;
	short lines;
	short resizeWidth, resizeHeight;
	Rect ourContent;
	
	RSsetwind(w);
	RScursoff(w);
	VSsetcols(w,(short)(width-1));
	VSgetrgn(w, &x1, &y1, &x2, &y2); /*get current visible region */
	x2= width-1;
	lines = VSgetlines(w);				/* NCSA: SB - trust me, you need this... */
	RScurrent->rwidth =
		RScurrent->width = (x2 - x1 + 1) * RScurrent->fwidth - CHO;
	RScurrent->rheight =
		RScurrent->height= (y2 - y1 + 1) * RScurrent->fheight; 


	if (RScurrent->rwidth > RMAXWINDOWWIDTH - 16 - CHO)
	 	 RScurrent->rwidth = RMAXWINDOWWIDTH - 16 - CHO;
	if (RScurrent->rheight > RMAXWINDOWHEIGHT - 16)
	 	 RScurrent->rheight = RMAXWINDOWHEIGHT - 16;

	ourContent = (*((WindowPeek)(RScurrent->window))->contRgn)->rgnBBox;	
	RScheckmaxwind(&ourContent,RScurrent->rwidth +16,	
				RScurrent->rheight + 16, &resizeWidth, &resizeHeight);		
	RScurrent->rwidth = resizeWidth - 16;
	RScurrent->rheight = resizeHeight - 16;
	SizeWindow
	  (
		RScurrent->window,
		RScurrent->rwidth + 16, RScurrent->rheight+16,
		FALSE
	  ); 
	RSsetsize(w, RScurrent->rheight + 16, RScurrent->rwidth + 16, -1);
	VSgetrgn(w, &x1, &y1, &x2, &y2);
	VSsetrgn(w, x1, y1,
		(short) (x1 + (RScurrent->rwidth ) / RScurrent->fwidth - 1),
		(short) (y1 + (RScurrent->rheight) / RScurrent->fheight - 1));
	VSgetrgn(w, &x1, &y1, &x2, &y2);		/* Get new region */
	
	DrawGrowIcon(RScurrent->window);
	RSdrawlocker(w, RScurrent->window->visRgn);
	VSredraw(w, 0, 0, x2 - x1 + 1, y2 - y1 + 1); /* redraw newly-revealed area, if any */
	ValidRect(&RScurrent->window->portRect); /* no need to do it again */
	DrawControls(RScurrent->window);
	
//	RScursoff(w);
}

/* handles a click in a terminal window. */
short RSclick( GrafPtr window, EventRecord theEvent)
{
	ControlHandle ctrlh;
	short w, part, part2, x1, x2, y1, y2;
	Point	where = theEvent.where;
	
	short	shifted = (theEvent.modifiers & shiftKey);
	short	optioned = (theEvent.modifiers & optionKey);
	
	w = 0;
    while ((RSlocal[w].window != window) && (w < MaxRS))	//find VS 
		w++;
    if ((RSlocal[w].window == 0L) || (w >= MaxRS))
		return(-1); /* what the heck is going on here?? */


	SetPort(window);
	GlobalToLocal((Point *) &where);
	part = FindControl(where, window, &ctrlh);		/* BYU LSC */
	if (part != 0)
		switch (part)
		  {
			case inThumb:
				if (gApplicationPrefs->noLiveScrolling) // RAB BetterTelnet 2.0b2
					part2 = TrackControl(ctrlh, where, 0L);
				else part2 = SmartTrackControl(ctrlh, where, ActiveScrollProcUPP);		/* BYU LSC */
				if (part2 == inThumb)
				  {
					part = GetControlValue(ctrlh);
					if (ctrlh == RSlocal[w].scroll)
					  {
					  /* scroll visible region vertically */
						VSgetrgn(w, &x1, &y1, &x2, &y2);
						VSsetrgn(w, x1, part, x2, part + (y2 - y1));
					  }
					else
					  { /* ctrlh must be .left */
					  /* scroll visible region horizontally */
						VSgetrgn(w, &x1, &y1, &x2, &y2);
						VSsetrgn(w, part, y1, part + (x2 - x1), y2);
					  } /* if */
				  } /* if */
				break;
			case inUpButton:
			case inDownButton:
			case inPageUp:
			case inPageDown:
				part2 = TrackControl(ctrlh, where, ScrollProcUPP);	/* BYU LSC */
	/*			InvalRect(&(**RSlocal->scroll).contrlRect); */  /* cheap fix */
				break;
			default:
				break;
		  } /* switch */
	else
	{
		if ((where.h <= RSlocal[w].width)&&(where.v <= RSlocal[w].height))
		{//CCP 2.7 added the above check so that we dont do things when we are in an inactive scrollbar
			if (optioned) 
			{
			  /* send host the appropriate sequences to move the cursor
				to the specified position */
				Point x;
				x = normalize(where, w,FALSE);
				VSpossend(w, x.h, x.v, screens[scrn].echo); /* MAT--we can check here if we want to use normal */
															/* MAT--or EMACS movement. */
			}
			else if (ClickInContent(where,w)) 		/* NCSA: SB - prevent BUS error */
			{
				Boolean	dragged;
	
				(void) DragText(&theEvent, where, w, &dragged);
				if (!dragged) 
					RSselect(w, where, theEvent);
			}
		}
	  } /* if */
	return
		0;
  } /* RSclick */

/*--------------------------------------------------------------------------*/
/* HandleDoubleClick														*/
/* This is the routine that does the real dirty work.  Since it is not a	*/
/* true TextEdit window, we have to kinda "fake" the double clicking.  By	*/
/* this time, we already know that a double click has taken place, so check	*/
/* the chars to the left and right of our location, and select all chars 	*/
/* that are appropriate	-- SMB												*/
/*--------------------------------------------------------------------------*/
static	void HandleDoubleClick(short w, short modifiers)													
{																				
	Point	leftLoc, rightLoc, curr, oldcurr;
	long	mySize;
	char	theChar;
	short	mode = -1, newmode, foundEnd=0;
	Point	pt;
	Point	temp;
	VSAttrib attrib;
	short	mw;
	VSlinePtr ypt;

	RSsetConst(w);								// get window dims
	leftLoc = RSlocal[w].anchor;				// these two should be the same
	rightLoc = RSlocal[w].last;

	mw = VSgetcols(w);

	ypt = VSIGetLineStart(w, rightLoc.v);
	while ( !foundEnd ) {
		// scan to the right first
		mySize = VSgettext(w, rightLoc.h, rightLoc.v, rightLoc.h+1, rightLoc.v,	
			&theChar, (long)1, "\015", 0, 0);
		if ( mySize == 0 || isspace(theChar) ) {	// stop if not a letter
			foundEnd = 1;														
		} else if ( ++rightLoc.h > mw - 1 ) {
			if ( rightLoc.v == 0 ) {
				ypt = VSIGetLineStart(w, rightLoc.v);
			}
			if ( ypt && VSiswrap(ypt->lattr) ) {
				ypt = ypt->next;
				rightLoc.h = -1;
				++rightLoc.v;
			} else {
				foundEnd = 1;														
			}
		}
	}																		

	if ( leftLoc.h < 0 )
		leftLoc.h = 0;
	ypt = VSIGetLineStart(w, leftLoc.v);
	foundEnd = 0;																
	while( !foundEnd ) {																		
		// ...and then scan to the left
		mySize = VSgettext(w, leftLoc.h-1, leftLoc.v, leftLoc.h, leftLoc.v,		
			&theChar, (long)1, "\015", 0, 0);
		if ( mySize == 0 || isspace(theChar) ) {		// STOP!
			foundEnd = 1;
		} else if ( --leftLoc.h < 0 ) {
			if ( leftLoc.v == -1 ) {
				ypt = VSIGetLineStart(w, leftLoc.v);
			}
			if ( ypt && VSiswrap(ypt->prev->lattr) ) {
				ypt = ypt->prev;
				leftLoc.h = mw;
				--leftLoc.v;
			} else {
				foundEnd = 1;														
			}
		}
	}																		

	if (leftLoc.h != rightLoc.h) {				// we selected something

		HiliteThis(w, leftLoc, rightLoc);

		if (modifiers & cmdKey)					// Possible URL selection
			HandleURL(w);
		else {																		
	
			curr.h = 0; curr.v = 0;
	
			pt = getlocalmouse(RSlocal[w].window);
			while (StillDown()) {
				// wait for mouse position to change
				do {
					oldcurr = curr;
					temp = getlocalmouse(RSlocal[w].window);
					curr = normalize(temp, w,TRUE);
					if ( curr.h > -1 ) {
						if (VSgetattr(w, curr.h - 1, curr.v, curr.h, curr.v, &attrib, sizeof(VSAttrib))) {
							if (VSisansi2b(attrib)) {
								++curr.h;
							}
						}
					}
				} while (StillDown() && (EqualPt(curr, oldcurr) || EqualPt(pt, temp)));

				if ( !EqualPt(pt, temp) /*|| RSlocal[w].selected*/ ) {
					pt = temp;
				
					if ((curr.v < leftLoc.v) || ((curr.v == leftLoc.v) && (curr.h < leftLoc.h))) {
						newmode = 1;	// up
					} else if ((curr.v > leftLoc.v) || ((curr.v == leftLoc.v) && (curr.h > rightLoc.h))) {
						newmode = 2;	// down
					} else 
						newmode = -1;	// inside dbl-clicked word
						
					/* toggle highlight state of text between current and last mouse positions */
					if (mode == -1) {
						if (newmode == 2) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, curr, rightLoc, &noConst);
							RSlocal[w].last = curr;
						}
						if (newmode == 1) {
							RSlocal[w].anchor = rightLoc;
							RSinvText(w, curr, leftLoc, &noConst);
							RSlocal[w].last = curr;
						}
					}
		
					if (mode == 1) {
						if (newmode == 2) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, oldcurr, leftLoc, &noConst);
							RSinvText(w, rightLoc, curr, &noConst);
							RSlocal[w].last = curr;
						}
						if (newmode == -1) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, oldcurr, leftLoc, &noConst);
							RSlocal[w].last = rightLoc;
						}
						if (newmode == mode) {
							RSinvText(w, oldcurr, curr, &noConst);
							RSlocal[w].last = curr;
						}
					}
					
					if (mode == 2) {
						if (newmode == 1) {
							RSlocal[w].anchor = rightLoc;
							RSinvText(w, oldcurr, rightLoc, &noConst);
							RSinvText(w, leftLoc, curr, &noConst);
							RSlocal[w].last = curr;
						}
						if (newmode == -1) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, oldcurr, rightLoc, &noConst);
							RSlocal[w].last = rightLoc;
						}
						if (newmode == mode) {
							RSinvText(w, oldcurr, curr, &noConst);
							RSlocal[w].last = curr;
						}
					}
						
					mode = newmode;
				}
			}
		}
	}	
}

/*--------------------------------------------------------------------------*/
/* HandleTripleClick														*/
/* This is the routine that does the real dirty work.  Since it is not a	*/
/* true TextEdit window, we have to kinda "fake" the double clicking.  By	*/
/* this time, we already know that a double click has taken place, so check	*/
/* the chars to the left and right of our location, and select all chars 	*/
/* that are appropriate	-- SMB												*/
/*--------------------------------------------------------------------------*/
static	void HandleTripleClick(short w, short modifiers)													
{																				
	Point	leftLoc, rightLoc, curr, oldcurr;
	long	mySize;
	char	theChar;
	short	mode = -1, newmode, foundEnd;
	Point	pt;
	Point	temp;
	VSAttrib attrib;
	short	mw;
	VSlinePtr ypt;

	RSsetConst(w);								// get window dims
	leftLoc = RSlocal[w].anchor;				// these two should be the same
	rightLoc = RSlocal[w].last;

	mw = VSgetcols(w);

	rightLoc.h = mw;
	ypt = VSIGetLineStart(w, rightLoc.v);
	foundEnd = 0;																
	while ( !foundEnd ) {
		// scan to the right first
		if ( rightLoc.v == 0 ) {
			ypt = VSIGetLineStart(w, rightLoc.v);
		}
		if ( ypt && VSiswrap(ypt->lattr) ) {
			ypt = ypt->next;
			++rightLoc.v;
		} else {
			foundEnd = 1;														
		}
	}

	leftLoc.h = -1;
	ypt = VSIGetLineStart(w, leftLoc.v);
	foundEnd = 0;																
	while ( !foundEnd ) {																		
		// ...and then scan to the left
		if ( leftLoc.v == -1 ) {
			ypt = VSIGetLineStart(w, leftLoc.v);
		}
		if ( ypt && VSiswrap(ypt->prev->lattr) ) {
			ypt = ypt->prev;
			--leftLoc.v;
		} else {
			foundEnd = 1;														
		}
/*
		// ...and then scan to the left
		mySize = VSgettext(w, leftLoc.h-1, leftLoc.v, leftLoc.h, leftLoc.v,		
			&theChar, (long)1, "\015", 0, 0);
		if ( mySize == 0 || isspace(theChar) ) {		// STOP!
			foundEnd = 1;
		} else if ( --leftLoc.h < 0 ) {
			if ( leftLoc.v == -1 ) {
				ypt = VSIGetLineStart(w, leftLoc.v);
			}
			if ( ypt && VSiswrap(ypt->prev->lattr) ) {
				ypt = ypt->prev;
				leftLoc.h = mw;
				--leftLoc.v;
			} else {
				foundEnd = 1;														
			}
		}
*/
	}																		

syslog(0, "leftLoc.h:%d rightLoc.h:%d\n", leftLoc.h, rightLoc.h);

	if (leftLoc.h != rightLoc.h) { // we selected something

		HiliteThis(w, leftLoc, rightLoc);

		if (modifiers & cmdKey)					// Possible URL selection
			HandleURL(w);
		else {																		
	
			curr.h = 0; curr.v = 0;
	
			pt = getlocalmouse(RSlocal[w].window);
			while (StillDown()) {
				// wait for mouse position to change
				do {
					oldcurr = curr;
					temp = getlocalmouse(RSlocal[w].window);
					curr = normalize(temp, w,TRUE);
					if ( curr.h > -1 ) {
						if (VSgetattr(w, curr.h - 1, curr.v, curr.h, curr.v, &attrib, sizeof(VSAttrib))) {
							if (VSisansi2b(attrib)) {
								++curr.h;
							}
						}
					}
				} while (StillDown() && (EqualPt(curr, oldcurr) || EqualPt(pt, temp)));

				if ( !EqualPt(pt, temp) /*|| RSlocal[w].selected*/ ) {
					pt = temp;
				
					if ((curr.v < leftLoc.v) || ((curr.v == leftLoc.v) && (curr.h < leftLoc.h))) {
						newmode = 1;	// up
					} else if ((curr.v > leftLoc.v) || ((curr.v == leftLoc.v) && (curr.h > rightLoc.h))) {
						newmode = 2;	// down
					} else 
						newmode = -1;	// inside dbl-clicked word
						
					/* toggle highlight state of text between current and last mouse positions */
					if (mode == -1) {
						if (newmode == 2) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, curr, rightLoc, &noConst);
							RSlocal[w].last = curr;
						}
						if (newmode == 1) {
							RSlocal[w].anchor = rightLoc;
							RSinvText(w, curr, leftLoc, &noConst);
							RSlocal[w].last = curr;
						}
					}
		
					if (mode == 1) {
						if (newmode == 2) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, oldcurr, leftLoc, &noConst);
							RSinvText(w, rightLoc, curr, &noConst);
							RSlocal[w].last = curr;
						}
						if (newmode == -1) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, oldcurr, leftLoc, &noConst);
							RSlocal[w].last = rightLoc;
						}
						if (newmode == mode) {
							RSinvText(w, oldcurr, curr, &noConst);
							RSlocal[w].last = curr;
						}
					}
					
					if (mode == 2) {
						if (newmode == 1) {
							RSlocal[w].anchor = rightLoc;
							RSinvText(w, oldcurr, rightLoc, &noConst);
							RSinvText(w, leftLoc, curr, &noConst);
							RSlocal[w].last = curr;
						}
						if (newmode == -1) {
							RSlocal[w].anchor = leftLoc;
							RSinvText(w, oldcurr, rightLoc, &noConst);
							RSlocal[w].last = rightLoc;
						}
						if (newmode == mode) {
							RSinvText(w, oldcurr, curr, &noConst);
							RSlocal[w].last = curr;
						}
					}
						
					mode = newmode;
				}
			}
		}
	}	
}

Point getlocalmouse(GrafPtr wind)
  /* returns the current mouse position in coordinates local
	to the specified window. Leaves the current grafPort set
	to that window. */
  {
	Point temp;

	SetPort(wind);
	GetMouse(&temp);
	return(temp);
  } /* getlocalmouse */
  
  /*--------------------------------------------------------------------------*/
/* NCSA: SB - ClickInContent												*/
/*	This procedure is a quick check to see if the mouse click is in the		*/
/*	content region of the window.  Normalize the point to be a VS location	*/
/* 	and then see if that is larger than what it should be...				*/
/*	Used by RSClick to see if the click is in the scroll bars, or content..	*/
/*--------------------------------------------------------------------------*/
short ClickInContent(Point where,short w)				/* NCSA: SB */
{														/* NCSA: SB */
	Point x;											/* NCSA: SB */
	x = normalize(where, w,FALSE);							/* NCSA: SB */
	if (x.v >= VSgetlines(w)) return 0;					/* NCSA: SB */
	else return 1;										/* NCSA: SB */
}														/* NCSA: SB */

void RSchangefont(short w, short fnum,long fsiz)
   /*	Set (w) to font fnum, size fsiz; resize window */
{
	Rect pRect;
	short x1, x2, y1, y2, width, lines;
	short srw,srh;
	WStateData *wstate;
	WindowPeek wpeek;
	short resizeWidth, resizeHeight;		/* NCSA: SB */
	int cursOff;

    RSsetwind(w);

	cursOff = 0;
	if ( VSIcursorenabled() && RScursison(w) ) {
		cursOff = 1;
		VSIcuroff(w); 						// temporarily hide cursor
	}

	srw = RScurrent->rwidth;
	srh = RScurrent->rheight;

	if (fnum != -1) {
		RSTextFont(fnum,fsiz,0);	/* BYU */
		RScurrent->fnum = fnum;
	}
	if ( !fsiz ) {
		fsiz = RScurrent->fsiz;
	}

	TextSize(fsiz);
	RScurrent->fsiz = fsiz;
	/* adjust bold size too */
	RScurrent->bfsiz = fsiz;

	RSfontmetrics();

	width = VSmaxwidth(w) + 1;
	lines = VSgetlines(w);

 /* resize window to preserve its dimensions in character cell units */

	VSgetrgn(w, &x1, &y1, &x2, &y2);	/* get visible region */
	RScurrent->rwidth =
		RScurrent->width = (x2 - x1 + 1) * RScurrent->fwidth - CHO;
	RScurrent->rheight =
		RScurrent->height= (y2 - y1 + 1) * RScurrent->fheight;

	if (RScurrent->rwidth > RMAXWINDOWWIDTH - 16 - CHO)
	 	 RScurrent->rwidth = RMAXWINDOWWIDTH - 16 - CHO;
	if (RScurrent->rheight > RMAXWINDOWHEIGHT - 16)
	 	 RScurrent->rheight = RMAXWINDOWHEIGHT - 16;

	RScheckmaxwind(&RScurrent->window->portRect,RScurrent->rwidth +16,
		RScurrent->rheight + 16, &resizeWidth, &resizeHeight);

	SizeWindow
	  (
		RScurrent->window,
		RScurrent->rwidth + 16, RScurrent->rheight+16,
		FALSE
	  ); /*  TRUE if done right */
	RSsetsize(w, RScurrent->rheight + 16, RScurrent->rwidth + 16, -1);

	wpeek = (WindowPeek) RScurrent->window;

	HLock(wpeek->dataHandle);
	wstate = (WStateData *) *wpeek->dataHandle;

	BlockMoveData(&pRect, &wstate->stdState, 8);
	pRect.right = pRect.left + RMAXWINDOWWIDTH;
	if (pRect.right > TelInfo->screenRect.right)
		pRect.right = TelInfo->screenRect.right;
	pRect.bottom = pRect.top + RMAXWINDOWHEIGHT;
	BlockMoveData(&wstate->stdState, &pRect, 8);

	VSgetrgn(w, &x1, &y1, &x2, &y2);
	VSsetrgn(w, x1, y1,
		(short) (x1 + (RScurrent->rwidth ) / RScurrent->fwidth - 1),
		(short) (y1 + (RScurrent->rheight) / RScurrent->fheight - 1));
	VSgetrgn(w, &x1, &y1, &x2, &y2);		// Get new region

	if ( cursOff ) {
		VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
	} else {
		// just calcutate the new coordinates
		VScursset(w, VSIw->x, VSIw->y);
	}
/*
	DrawGrowIcon(RScurrent->window);
	RSdrawlocker(w, RScurrent->window->visRgn);
	VSredraw(w, 0, 0, x2 - x1 + 1, y2 - y1 + 1); // redraw newly-revealed area, if any
	ValidRect(&RScurrent->window->portRect); // no need to do it again
	DrawControls(RScurrent->window);
*/
	InvalRect(&RScurrent->window->portRect);

} /* RSchangefont */


void RSchangebold
	(
		short w,
		short allowBold,
		short colorBold,
		short inversebold
	)
{
	RSsetwind(w);
	RScurrent->allowBold = allowBold;
	RScurrent->colorBold = colorBold;
	RScurrent->bfstyle = inversebold;
/* NONO */
	RScurrent->realbold = allowBold;
/* NONO */
/*
	VSredraw(screens[scrn].vs,0,0,VSmaxwidth(screens[scrn].vs),VSgetlines(screens[scrn].vs)-1);
*/
	InvalRect(&RScurrent->window->portRect);
}


short RSgetfont
  (
	short w, /* which window */
	short *pfnum, /* where to return font ID */
	short *pfsiz /* where to return font size */
  )
  /* returns the current font ID and size setting for the specified window. */
  {
    if (0 > RSsetwind(w))
		return -1;
	*pfnum = RScurrent->fnum;
	*pfsiz = RScurrent->fsiz;
	return(0);
  } /* RSgetfont */

void RSfontmetrics
  (
	void
  )
  /* calculates various metrics for drawing text with selected font
	and size in current grafport into *RScurrent. */
  {
	FontInfo finforec;
	GrafPtr myGP;
	StyleField txFace;
 
	GetPort(&myGP); 
	GetFontInfo(&finforec);
	RScurrent->fascent = finforec.ascent;
	RScurrent->fheight = finforec.ascent + finforec.descent + finforec.leading /* +1 */;
	RScurrent->monospaced = (CharWidth('W') == CharWidth('i'));   
	txFace = myGP->txFace;
	TextFace( 0 );
	RScurrent->fwidth = CharWidth('W'); 
	TextFace( bold );
	RScurrent->boldislarger = ( CharWidth('W') != RScurrent->fwidth);
	TextFace( txFace );

/*	SetFontInfo(&finforec);*/
}

pascal void ScrollProc(ControlHandle control, short part)
  /* scroll-tracking routine which does continuous scrolling of visible
	 region. */
  {
	short w, kind, x1, y2, x2, y1, value;

	kind = RSfindscroll(control, &w);
	VSgetrgn(w, &x1, &y1, &x2, &y2);

	if (kind == 2)
	  { /* horizontal scroll bar */
		switch (part)
		  {
			case inUpButton:							/* Up is left */
				VSscrolleft(w, 1);
				break;
			case inDownButton:							/* Down is right */
				VSscrolright(w, 1);
				break;
			case inPageUp:
				VSscrolleft(w, x2 - x1); /* scroll a whole windowful */
				break;
			case inPageDown:
				VSscrolright(w, x2 - x1); /* scroll a whole windowful */
				break;
			default:
				break;
		  } /* switch */
	  }
	else if (kind == 1)
	  { /* vertical scroll bar */
		switch (part)
		  {
			case inUpButton:
				VSscrolback(w, 1);
				break;
			case inDownButton:
				VSscrolforward(w, 1);
				break;
			case inPageUp:
				VSscrolback(w, y2 - y1); /* scroll a whole windowful */
				break;
			case inPageDown:
				VSscrolforward(w, y2 - y1); /* scroll a whole windowful */
				break;
			default:
				break;
		  } /* switch */
	  } /* if */
  } /* ScrollProc */

pascal void ActiveScrollProc(ControlHandle control, short part)
  /* scroll-tracking routine which does continuous scrolling of visible
	 region. */
  {
	short w, kind, x1, y2, x2, y1, value;

	kind = RSfindscroll(control, &w);
	VSgetrgn(w, &x1, &y1, &x2, &y2);

	if (kind == 2)
	  { /* horizontal scroll bar */
		switch (part)
		  {
			case inThumb:
				value = GetControlValue(control);
				  /* scroll visible region horizontally */
					VSsetrgn(w, value, y1, value + (x2 - x1), y2);
					break;
			default:
				break;
		  } /* switch */
	  }
	else if (kind == 1)
	  { /* vertical scroll bar */
		switch (part)
		  {
			case inThumb:
					value = GetControlValue(control);
					  /* scroll visible region vertically */
						VSsetrgn(w, x1, value, x2, value + (y2 - y1));
						break;
			default:
				break;
		  } /* switch */
	  } /* if */
  } /* ActiveScrollProc */

void UnHiliteSelection(short w)
{
	RSinvText(w, RSlocal[ w].anchor, RSlocal[w].last, &noConst);
	RSlocal[w].selected = FALSE;
}
void HiliteThis(short w, Point begin, Point end)
{
	if (RSlocal[w].selected)
		UnHiliteSelection(w);

	RSlocal[w].anchor.v = begin.v;
	RSlocal[w].anchor.h = begin.h;
	RSlocal[w].last.v = end.v;
	RSlocal[w].last.h = end.h;
	
	RSinvText(w, RSlocal[ w].anchor, RSlocal[w].last, &noConst);
	RSlocal[w].selected = TRUE;
}

void calculateWindowPosition(WindRec *theScreen,Rect *whereAt, short colsHigh, short colsWide)
{
	
	short offset, currentCount = 0, lastIndex = 0;
	Boolean done = FALSE, tooFarRight = FALSE;
	short w = theScreen->vs;
	Boolean wideCount = 0;
	theScreen->positionIndex = 0;
	while (!done)
	{
	
		while (((*topLeftCorners)[theScreen->positionIndex] > currentCount)&& //find an empty spot
				(theScreen->positionIndex < MaxSess - 1))
			theScreen->positionIndex++;

		offset = ((gApplicationPrefs->StaggerWindows == TRUE) ? 
					gApplicationPrefs->StaggerWindowsOffset : 1) * (theScreen->positionIndex);
		whereAt->top = GetMBarHeight() + 25 + offset;
		whereAt->left  = 10 + offset;
		if (!tooFarRight)
			whereAt->left += (currentCount-wideCount)*gApplicationPrefs->StaggerWindowsOffset;
		else
			wideCount += currentCount - 1;
		whereAt->bottom= 30000 + offset;
		whereAt->right = 30000 + offset;
		tooFarRight = (whereAt->left + (colsWide + 1)*RScurrent->fwidth + 16 - CHO >
					TelInfo->screenRect.right);
		if (tooFarRight || (whereAt->top + (colsHigh + 1)*RScurrent->fwidth + 16 - CHO > 
										TelInfo->screenRect.bottom))
		{ // we are off screen
			if (theScreen->positionIndex == 0)
				return; //the window is bigger than the screensize; return;
			
			currentCount++; // go through again, pick spot with least number already at it
			lastIndex = theScreen->positionIndex;
			theScreen->positionIndex = 0; 		
		}
		else
			done = TRUE;
	}	
	((*topLeftCorners)[theScreen->positionIndex])++; //add our window to the number at this spot
}

void RSUpdatePalette(void)  //called when ANSI colors have changed, and we need to update each
{							//windows palette
	GrafPtr oldPort;
	int sn;
	WindRec	*w;
	

	GetPort(&oldPort);
	for (sn = 0; sn < TelInfo->numwindows; sn++)
	{
		w = &screens[sn];
		if ((w->active == CNXN_ACTIVE)||
			(w->active == CNXN_OPENING))
		{
			if (w->ANSIgraphics)
			{
				if (RSsetwind(w->vs) >= 0)
				{
					int i;
					for (i = 0; i < 16; i++) {
							RGBColor tempColor;
							GetEntryColor(TelInfo->AnsiColors, i, &tempColor);
							SetEntryColor(RScurrent->pal,i+4, &tempColor); //set the new color
					}
					SetPort(w->wind);
					InvalRect(&RScurrent->window->portRect); //force a redraw
				}
			}
		}
	}
	SetPort(oldPort);
}

void RSchangeboldfont(short w, short fnum)
{
    RSsetwind(w);
	RScurrent->bfnum = fnum;
	VSredraw(screens[scrn].vs,0,0,VSmaxwidth(screens[scrn].vs),VSgetlines(screens[scrn].vs)-1);
} /* RSchangeboldfont */

short RSgetboldfont
  (
	short w, /* which window */
	short *pfnum /* where to return font ID */
  )
  /* returns the current font ID and size setting for the specified window. */
  {
    if (0 > RSsetwind(w))
		return -1;
	*pfnum = RScurrent->bfnum;
	return(0);
  } /* RSgetboldfont */

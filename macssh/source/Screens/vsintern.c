// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// #pragma profile on

/*
 *
 *	  Virtual Screen Kernel Internal Routines
 *					  (vsintern.c)
 *  National Center for Supercomputing Applications
 *
 *	  by Gaige B. Paulsen
 *
 *	This file contains the private internal calls for the NCSA
 *  Virtual Screen Kernel.
 *
 *		Version Date	Notes
 *		------- ------  ---------------------------------------------------
 *		0.01	861102  Initial coding -GBP
 *		0.50	861113  First compiled edition -GBP
 *		0.70	861114  Internal operation confirmed -GBP
 *		2.1		871130	NCSA Telnet 2.1 -GBP
 *		2.2 	880715	NCSA Telnet 2.2 -GBP
 */


#include "vsdata.h"
#include "vskeys.h"
#include "vsinterf.proto.h"
#include "rsmac.proto.h"
#include "rsinterf.proto.h"
#include "maclook.proto.h"
#include "translate.proto.h"
#include "wind.h"
#include "rsdefs.h"
#include "Vers.h"

#define ScrollbackQuantum 100

//#define VSIclrattrib 0
//#define VSIclrattrib (VSIw->attrib & 0x0003ffff)

#include "vsintern.proto.h"

extern short 	TempItemsVRefNum;
extern long		TempItemsDirID;
extern WindRec	*screens;
extern RSdata	*RSlocal;



short VSIclip
  (
	 short *x1, /* starting column */
	 short *y1, /* line on which to draw (assumed to lie within visible region) */
	 short *x2, /* ending column (inclusive) (output if *n >= 0) */
	 short *y2, /* ending line (inclusive) (output if *n >= 0) */
	 short *n, /* length of text to draw (input and output) */
	 short *offset /* length of initial part of text to skip (output) */
  )
  /* clips a text string to the visible region, given the starting
	line and column in screen coordinates at which it is to be drawn.
	If the length of the string is given, will also compute the ending
	line and column. On return, these coordinates will be normalized
	to the current visible region. Returns a nonzero function result
	iff the string is completely invisible. */
  {
	if (*n >= 0)
	  {
	  /* compute ending line and column (inclusive) */
		*x2 = *x1 + *n - 1;
		*y2 = *y1;
	  }
  /* else take these as given */

	if ((*x1 > VSIw->Rright) || (*y2 < VSIw->Rtop))
		return (-1); /* nothing to draw */

	if (*x2 > VSIw->Rright)
		*x2 = VSIw->Rright;
	if (*y2 > VSIw->Rbottom)
		*y2 = VSIw->Rbottom;
  /* normalize x1, x2, y1, y2 to be relative to current visible region */
	*x1 -= VSIw->Rleft;
	*x2 -= VSIw->Rleft;
	*y1 -= VSIw->Rtop;
	*y2 -= VSIw->Rtop;
  /* clip part of text string lying outside region, if any */
	*offset = - *x1;
	if (*offset < 0)
		*offset = 0; /* text string starts within region--nothing to clip */
  /* don't draw anything outside region */
	if (*x1 < 0)
		*x1 = 0;
	if (*y1 < 0)
		*y1 = 0;

	*n = *x2 - *x1  + 1 ; /* length of string to draw (assuming it's all on one line) */
	if ((*n <= 0) || (*y2 - *y1 < 0))
		return (-1); /* nothing to draw */
	return (0);
  } /* VSIclip */

short VSIcdellines(short w, short top, short bottom, short n, short scrolled)
		  /*
			-ve => cancel current selection, if any;
			+ve => selection has moved up one line;
			0 => don't touch selection
		  */
  /* updates the display to indicate deletion of the specified
	number of lines from the top of the specified region.
	Returns 0 iff any part of the change is visible. */
  {
	short
		x1 = 0,
		x2 = VSIw->maxwidth,
		tn = -1,
		offset;

	if (VSIclip(&x1, &top, &x2, &bottom, &tn, &offset))
		return(-1); /* affected region is invisible */
	tn = bottom - top;
	if (tn < n)
		n = tn; /* don't bother scrolling more lines than scrolling region holds */
	RSdellines(w, top, bottom, n, scrolled);
	return(0);				/* I delete the whole thing! */
  } /* VSIcdellines */

short VSIcinslines(short w, short top, short bottom, short n, short scrolled) /* -ve <=> cancel current selection, if any */
  /* updates the display to indicate insertion of the specified
	number of blank lines at the top of the specified region.
	Returns 0 iff any part of the change is visible. */
  {
	short
		x1 = 0,
		x2 = VSIw->maxwidth,
		tn = -1,
		offset;

	if (VSIclip(&x1, &top, &x2, &bottom, &tn, &offset))
		return -1; /* affected region is invisible */
	tn = bottom - top;
	if (tn < n)
		n = tn; /* don't bother scrolling more lines than scrolling region holds */
	RSinslines(w, top, bottom, n, scrolled);
	return 0;
  } /* VSIcinslines */


void VScursset( short w, short x, short y )
{
	short		x2;
	short		y2;
	short		n = 1;
	short		offset;
	short		lattr;

	if (VSvalids(w) != 0)
		return;

	lattr = VSIw->linest[y]->lattr;

	if (!VSIclip(&x, &y, &x2, &y2, &n, &offset)) {
		/* cursor already lies within visible region */
		RScursset(w, lattr, x, y);
	}
}

/*
 * VSIcurson
 *
 * displays the text cursor at the specified position. If
 * ForceMove is true, I am to do any appropriate scrolling of
 * the display to ensure the cursor is within the visible region.
 * Assumes cursor isn't currently being shown.
 */
void VSIcurson( short w, short x, short y, short ForceMove )
{
	short
		x2,
		y2,
		n = 1,
		offset;
	short lattr;

	if (VSvalids(w) != 0)
		return;

	if (!VSIw->DECCM) return; // Bri 970610
	if (VSIw->disableCursor) return; // RAB BetterTelnet 2.0b4

	lattr = VSIw->linest[y]->lattr;

	if (!VSIclip(&x, &y, &x2, &y2, &n, &offset)) {
		/* cursor already lies within visible region */
		RScurson(w, lattr, x, y); /* just make it visible */
	} else if (ForceMove) {
	  /* scroll to make cursor visible */
		x2 = VSIw->Rbottom - VSIw->Rtop;
		if (x2 >= VSIw->lines) {
			/* visible region is big enough to show entire screen--
			make sure I don't scroll off the bottom of the screen.
			This call will also do any appropriate scrolling and
			redisplaying of the cursor. */
			VSsetrgn(VSIwn, VSIw->Rleft, VSIw->lines - x2,
				VSIw->Rright, VSIw->lines);
		} else {
			/* x & y have been normalized relative to left & top
			of current visible region. Just call the appropriate scroll
			routine, which will also redisplay the cursor. */
			if (y > 0)
				VSscrolforward(VSIwn, y);
			else
				VSscrolback(VSIwn, -y);
		} /* if */
	} /* if */
} /* VSIcurson */


/*
 * VSIcuroff
 *
 * hides the cursor for the specified screen. Assumes it
 * is currently being shown (or that it's on an invisible
 * part of the screen).
 */
void VSIcuroff( short w )
{
	short
		x,
		y,
		x2,
		y2,
		n = 1,
		offset;

	if (VSvalids(w) != 0)
		return;

	x = VSIw->x;
	y = VSIw->y;

	if (!VSIclip(&x, &y, &x2, &y2, &n, &offset) || !VSIw->DECCM) // Bri 970610
	  /* cursor is on visible part of screen */
		RScursoff(w);
} /* VSIcuroff */


void VSIcursdisable() // RAB BetterTelnet 2.0b4
{
	VSIw->disableCursor = 1;
}

void VSIcursenable()
{
	VSIw->disableCursor = 0;
}

short VSIcursorenabled( void )
{
	if (!VSIw->DECCM)
		return 0; // Bri 970610
	if (VSIw->disableCursor)
		return 0; // RAB BetterTelnet 2.0b4
	return 1;
}

short VSIcursorvisible( void )
{
	short
		x = VSIw->x,
		y = VSIw->y,
		x2,
		y2,
		n = 1,
		offset;

	if (!VSIcursorenabled())
		return 0;
	if (VSIclip(&x, &y, &x2, &y2, &n, &offset))
		return 0;
	return 1;
}

/*
 * VScursblink
 */

void VScursblink( short w )
{
	unsigned long	now;
	short
		x = VSIw->x,
		y = VSIw->y,
		x2,
		y2,
		n = 1,
		offset;

	if (!gApplicationPrefs->BlinkCursor
	 || VSvalids(w)
	 || !VSIcursorvisible()
	 || ((!RSlocal[w].active || TelInfo->suspended) && !RSlocal[w].cursorstate))
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
} /* VScursblink */


/*
 * VScursblinkon
 */

void VScursblinkon( short w )
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
} /* VScursblinkon */


/*
 * VScursblinkoff
 */

void VScursblinkoff( short w )
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
} /* VScursblinkoff */




VSlineArray VSInewlinearray
  (
	short nrlines
  )
  /* allocates an array to hold the specified number of pointers
	to line elements. */
  {
	return
		(VSlineArray) myNewPtrCritical(sizeof(VSlinePtr) * nrlines);
  } /* VSInewlinearray */

VSlinePtr VSInewlines
  (
    short nlines
  )
  /* allocates a doubly-linked list of the specified number of
     line elements, and returns a pointer to the head of the list,
     or NULL if ran out of memory. The maximum number of characters
     each line can hold is controlled by VSIw->allwidth.
  */
  {
	void* memoryPtr;
	VSlinePtr linePtr;
	char* textPtr;
	VSAttrib* attrPtr;
	long width;
	long i;

	/* Allocated one large block that will be broken up as follows:
	   +---------+-------+-------+
	   | VSlines | texts | attrs |
	   +---------+-------+-------+
	   This way, we can save tons of time allocating and deallocating
	   memory from the system.
	   BCS 970726
	*/
	width = VSIw->allwidth + 1;
	memoryPtr = myNewPtrCritical(nlines * (sizeof(VSline) + (width * (1 + sizeof(VSAttrib)))));
	if(memoryPtr == NULL)
		return (VSlinePtr)NULL;

	// Chop the memory up into its 3 chunks.
	linePtr = (VSlinePtr)memoryPtr;
	textPtr = (char*)(linePtr + nlines);
	attrPtr = (VSAttrib*)(textPtr + (nlines * width));

	// Loop through the elements, initializing each one.
	for(i = 0; i < nlines; i++) {
		linePtr->lattr = 0;
		linePtr->mem = 0;
		linePtr->text = textPtr;
		linePtr->attr = attrPtr;
		linePtr->prev = linePtr - 1;
		linePtr->next = linePtr + 1;
		linePtr++;
		textPtr += width;
		attrPtr += width;
	}  

	// Flag the first line as the free element.
	// This seems really strange; should be fixed (BCS)
	((VSline*)memoryPtr)->mem = 1;

	// Terminate the beginning and end nodes in the list.
	((VSline*)memoryPtr)->prev = NULL;
	(linePtr-1)->next = NULL;

	// Return the head.
	return (VSline*)memoryPtr;
  } /* VSInewlines */

VSlinePtr VSOnewlines
  (
	short nlines,
	short elementSize //CCP 2.7
  )
  /* allocates a doubly-linked list of the specified number of
	line elements, and returns a pointer to the head of the list,
	or nil if ran out of memory. The maximum number of characters
	each line can hold is controlled by VSIw->allwidth. */
  {
	VSlinePtr t2;
	char *t;
	register short i;
	
  /* allocate one block for the line list elements, and another
	block for the line contents. These blocks will be divided up
	and appropriate flags set so I will be able to call DisposePtr
	the right number of times, with the right arguments. */
	if ((t = myNewPtrCritical(nlines * (VSIw->allwidth + 1)*elementSize)) != 0L)
	  {
		if ((t2 = (VSlinePtr) myNewPtrCritical(nlines * sizeof(VSline))) != 0L)
			t2->text = t;
		else
		  {
		  /* clean up gracefully before giving up */
			DisposePtr(t);
			return(0L);
		  } /* if */
	  }
	else
	  /* sorree no memoree */
		return((VSlinePtr) 0L);

/*
*  indicate to the free routine that the first record is the one to free.
*/
	t2->mem = 1;						/* call DisposePtr on this one */
	t2->next = t2 + 1;					/* point to next one */
/*
*  Take our allocation for multiple lines and fill in the structures to 
*  point to the right text fields and connect the doubly-linked chain.
*
*/
	for (i = 1; i < nlines; i++)
	  {
		t += (VSIw->allwidth + 1)* elementSize;		/* inc to next text space for a line */
		t2[i].lattr = 0;
		t2[i].mem = 0;					/* don't DisposePtr any of these */
		t2[i].text = t;
		t2[i].prev = t2 + i - 1;		/* point back one */
		t2[i].next = t2 + i + 1;		/* point forward one */
	  } /* for */
	
	t2[0].prev = 0L;					/* first one has no prev yet */
	t2[nlines - 1].next = 0L;			/* last one has no next yet */

	return(t2);
  } /* VSOnewlines */


void VSIlistndx
  (
	register VSlinePtr ts
  )
  /* sets up the screen arrays for the current screen to point
	at the given lists of attribute and text lines. */
  {
	register short i;
	for (i = 0; i <= VSIw->lines; i++)
	  {
		VSIw->linest[i] = ts;
		ts = ts->next;
	  } /* for */
  } /* VSIlistndx */

void VSOlistndx
  (
	register VSlinePtr ts,
	register VSattrlinePtr as
  )
  /* sets up the screen arrays for the current screen to point
	at the given lists of attribute and text lines. */
  {
	register short i;
	for (i = 0; i <= VSIw->lines; i++)
	  {
		VSIw->attrst[i] = as;
		VSIw->linest[i] = ts;
		ts = ts->next;
		as = as->next;
	  } /* for */
  } /* VSOlistndx */

void VSIscroff
  (
	void
  )
  /* called to save current screen contents in scrollback buffer,
	if it is ordained that I should do so. This is called by VSIes
	(below) just before the entire screen is cleared. */
  {
	VSlinePtr tmp;
	register short i, j;

	if
	  (
			(!VSIw->savelines) /* not saving lines */
		||
			(VSIw->top != 0) || (VSIw->bottom != VSIw->lines)
			  /* scrolling region isn't exactly the entire screen */
	  )
		return; /* do nothing */

	if (VSIw->oldScrollback) {
		VSOscroff();
		return;
	}

	tmp = VSIw->linest[VSIw->lines]; /* need VSIw->lines + 1 more lines */
	for (i = 0; i <= VSIw->lines; i++)
	  {
	  /* count off the lines in the part of the scrollback buffer
		below the screen (if any), to see if there's enough to hold
		a screenful. If the scrollback list isn't circular, then
		this part contains lines that have been allocated, but not
		yet used. If the list is circular (meaning it has reached
		its full size), then this is the part that is next in line
		for reuse. */
		if (!tmp->next) 
		  { /* not enough */
			j = VSIw->maxlines - VSIw->numlines - i; /* potential unallocated scrollback */
			if (j > ScrollbackQuantum)
				j = ScrollbackQuantum; /* but don't bother allocating more than this */
			if (j <= 0)
			  {
			  /* already reached user-specified scrollback limit-- */
			  /* make the list circular to indicate no more extension. */
				tmp->next = VSIw->buftop;
				VSIw->buftop->prev = tmp;		/* connect it up */
			  }
			else
			  {
			  /* extend the scrollback buffer to make room for
				another screenful */
				if (j < VSIw->lines - i + 1)
					j = VSIw->lines - i + 1; /* need at least this many */
				if ((tmp->next = VSInewlines(j)) != 0L) 
					tmp->next->prev = tmp;		/* got some space--link it up */
				else
				  {
				  /* out of memory--no more extensions */
					tmp->next = VSIw->buftop;
					VSIw->buftop->prev = tmp;	
				  } /* if */				
			  } /* if */
			break;								/* only allocate once is enough */
		  } /* if */
		tmp = tmp->next; /* keep counting */
	  } /* for */
		
/*
*  at this point, we know we have enough memory for the whole scroll.
*  It might be wraparound (reuse of some line elements), might not.
*/
		
	for (i = 0; i <= VSIw->lines; i++)
	  {
	  /* push another screen line into the scrollback area */
// RAB BetterTelnet 1.2 - bug fix for bad linked list bug
// What was NCSA thinking when they wrote this code, anyway?
//		if (VSIw->linest[i]->next == VSIw->buftop)
		if (VSIw->linest[VSIw->lines]->next == VSIw->buftop)
			VSIw->buftop = VSIw->buftop->next;	/* reusing old space */
		else
			VSIw->numlines++;				/* using some new space */
		VSIw->scrntop = VSIw->scrntop->next; /* move another line into the scrollback buffer */
		VSIlistndx(VSIw->scrntop); /* and update screen arrays */
	  /* note that it's up to the caller to clear out the new screen text
		and attribute lines */
	  } /* for */

	VSIw->vistop = VSIw->scrntop;

// RAB BetterTelnet 2.0b3 - hopefully the very last scrollback bug
// I think this one caused quite a few crashes now and then.
// Rtop and Rbottom weren't being synced with vistop (stupid)
	VSIw->Rbottom -= VSIw->Rtop;
	VSIw->Rtop = 0;
//	VSIw->Rbottom = VSIw->lines;
	RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar */
  } /* VSIscroff */

void VSOscroff
  (
	void
  )
  /* called to save current screen contents in scrollback buffer,
	if it is ordained that I should do so. This is called by VSIes
	(below) just before the entire screen is cleared. */
  {
	VSlinePtr tmp;
	register short i, j;

	if
	  (
			(!VSIw->savelines) /* not saving lines */
		||
			(VSIw->top != 0) || (VSIw->bottom != VSIw->lines)
			  /* scrolling region isn't exactly the entire screen */
	  )
		return; /* do nothing */

	tmp = VSIw->linest[VSIw->lines]; /* need VSIw->lines + 1 more lines */
	for (i = 0; i <= VSIw->lines; i++)
	  {
	  /* count off the lines in the part of the scrollback buffer
		below the screen (if any), to see if there's enough to hold
		a screenful. If the scrollback list isn't circular, then
		this part contains lines that have been allocated, but not
		yet used. If the list is circular (meaning it has reached
		its full size), then this is the part that is next in line
		for reuse. */
		if (!tmp->next) 
		  { /* not enough */
			j = VSIw->maxlines - VSIw->numlines - i; /* potential unallocated scrollback */
			if (j > ScrollbackQuantum)
				j = ScrollbackQuantum; /* but don't bother allocating more than this */
			if (j <= 0)
			  {
			  /* already reached user-specified scrollback limit-- */
			  /* make the list circular to indicate no more extension. */
				tmp->next = VSIw->buftop;
				VSIw->buftop->prev = tmp;		/* connect it up */
			  }
			else
			  {
			  /* extend the scrollback buffer to make room for
				another screenful */
				if (j < VSIw->lines - i + 1)
					j = VSIw->lines - i + 1; /* need at least this many */
				if ((tmp->next = VSOnewlines(j,1)) != 0L) 
					tmp->next->prev = tmp;		/* got some space--link it up */
				else
				  {
				  /* out of memory--no more extensions */
					tmp->next = VSIw->buftop;
					VSIw->buftop->prev = tmp;	
				  } /* if */				
			  } /* if */
			break;								/* only allocate once is enough */
		  } /* if */
		tmp = tmp->next; /* keep counting */
	  } /* for */
		
/*
*  at this point, we know we have enough memory for the whole scroll.
*  It might be wraparound (reuse of some line elements), might not.
*/
		
	for (i = 0; i <= VSIw->lines; i++)
	  {
	  /* push another screen line into the scrollback area */
//		RAB BetterTelnet 1.2.1 - bug fix
//		if (VSIw->linest[i]->next == VSIw->buftop)
		if (VSIw->linest[VSIw->lines]->next == VSIw->buftop)
			VSIw->buftop = VSIw->buftop->next;	/* reusing old space */
		else
			VSIw->numlines++;				/* using some new space */
		VSIw->scrntop = VSIw->scrntop->next; /* move another line into the scrollback buffer */
		VSOlistndx(VSIw->scrntop, VSIw->attrst[1]); /* and update screen arrays */
	  /* note that it's up to the caller to clear out the new screen text
		and attribute lines */
	  } /* for */

	VSIw->vistop = VSIw->scrntop;
// RAB BetterTelnet 2.0b3 - bug fix
	VSIw->Rbottom -= VSIw->Rtop;
	VSIw->Rtop = 0;
//	VSIw->Rbottom = VSIw->lines;
	RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar */
  } /* VSOscroff */

void VSIelo
  (
	short s /* line to erase, -ve => line containing cursor */
  )
  /* blanks out the specified line in the screen buffer.
	Doesn't do anything to the display. */
  {
	char *tt;
	VSAttrib attrib;
	VSAttrib *ta;
	short i;

	if (s < 0)
		s = VSIw->y;

	VSIw->linest[s]->lattr = 0;
	if (VSIw->oldScrollback) {
		ta = &VSIw->attrst[s]->text[0];
	} else {
		ta = &VSIw->linest[s]->attr[0];
	}
	tt = &VSIw->linest[s]->text[0];

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

	for (i = 0; i <= VSIw->allwidth; i++)
	  {
		*ta++ = attrib;
		*tt++ = ' ';
	  } /* for */
  } /* VSIelo */

void VSIes
  (
	void
  )
  /* clears the screen, first saving its contents in the
	scrollback buffer if appropriate. Also updates the display. */
  {
	short
		i;
	short
		x1 = 0,
		y1 = 0,
		x2 = VSIw->maxwidth,
		y2 = VSIw->lines,
		n = -1,
		offset;

	VSIflush(); // RAB BetterTelnet 2.0b3

  /* save screen contents in scrollback buffer, if appropriate */
	if (VSIw->ESscroll)
		VSIscroff();
  /* clear out screen buffer */
	for (i = 0; i <= VSIw->lines; i++)
		VSIelo(i);
  /* update display to show what I've done */
	if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) 
		RSerase(VSIwn, x1, y1, x2, y2);
	VSIw->vistop = VSIw->scrntop;
// RAB BetterTelnet 2.0b3 - bug fix
	VSIw->Rtop = 0;
	VSIw->Rbottom = VSIw->lines;
	RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar */

  } /* VSIes */

void VSItabclear
  (
	void
  )
  /* clears all current tab settings. */
  {
	short
		x = 0;

	while (x <= VSIw->allwidth)
	  {
		VSIw->tabs[x] = ' ';
		x++;
	  } /* while */
  } /* VSItabclear */

void VSItabinit
  (
	void
  )
  /* sets tab settings to default (every 8 columns). */
  {
	short
		x = 0;

	VSItabclear();
	while (x <= VSIw->allwidth)
	  {
		VSIw->tabs[x] = 'x';
		x += 8;
	  }
	VSIw->tabs[VSIw->allwidth] = 'x';
  } /* VSItabinit */

void VSIreset
  (
	void
  )
  /* restores terminal mode settings to defaults and clears screen. */
  {
  	VSIflush(); // RAB BetterTelnet 2.0b3
	VSIw->top = 0;
	VSIw->bottom = VSIw->lines;
	VSIw->parmptr = 0;
	VSIw->escflg = 0;
	VSIw->DECAWM = 0;
	VSIw->DECCKM = 0;
	VSIw->DECPAM = 0;
	VSIw->DECORG = 0;		/* NCSA: SB -- is this needed? */
	VSIw->DECCM = 1; // Bri 970610
	VSIw->Pattrib = 0xffffffff;		/* NCSA: SB -- is this needed? */
	VSIw->IRM = 0;
	VSIw->attrib = 0;
	VSIw->x = 0;
	VSIw->y = 0;
	VSIw->charset = 0;
	VSIw->trincount = 0;
	VSIw->trinx = 0;
	VSIw->trintag = 0;
	VSIw->prbuf=0;										/* LU */
	if (VSIw->prredirect) {								/* LU - kill redirection */
		VSIw->prredirect=0;								/* LU */
		FSClose (VSIw->refNum);							/* LU */
		VSIw->refNum=-1;								/* LU */
		HDelete(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname);		/* LU */
	}													/* LU */
	VSIes();
	VSItabinit();
	} /* VSIreset */

void VSIlistmove(VSlinePtr TD, VSlinePtr BD, VSlinePtr TI, VSlinePtr BI)
  /* moves the lines from TD to BD inclusive from their
	present position to between TI and BI. Either of the
	latter may be nil. */
  {
  /* unlink the lines from TD to BD */
	if (TD->prev != 0L)
		TD->prev->next = BD->next;	/* Maintain circularity */
	if (BD->next != 0L)
		BD->next->prev = TD->prev;
  /* relink them between TI and BI */
	TD->prev = TI;								/* Place the node in its new home */
	BD->next = BI;
	if (TI != 0L)
		TI->next = TD;					/* Ditto prev->prev */
	if (BI != 0L)
		BI->prev = BD;
  } /* VSIlistmove */

void VSIdellines
  (
	short n, /* nr lines to delete */
	short s /* starting line to delete, -ve => line containing cursor */
  )
  /* deletes lines from the screen, scrolling up the remainder and
	inserting new blank lines at the bottom of the scrolling region. */
  {
	short i, j;
	char *tt;
	VSAttrib attrib;
	VSAttrib *ta;
	VSlinePtr  ts, TD, BD, TI, BI, itt;

	if (VSIw->oldScrollback) {
		VSOdellines(n, s);
		return;
	}

	if (s < 0)
		s = VSIw->y;
	if (s + n - 1 > VSIw->bottom)
		n = VSIw->bottom - s + 1; /* don't bother deleting more than scrolling region will hold */

  /* find new tops of screen arrays */
// RAB BetterTelnet 2.0b3
// argh, must I keep fixing bugs here?
//	if (s == 0 && n <= VSIw->lines)
	if (s == 0 && n <= VSIw->bottom)
	  {
	  /* element for line after last one being deleted */
		ts = VSIw->linest[n];
	  }
	else
	  {
	  /* top line unaffected, or entire screen is being wiped */
		ts = VSIw->linest[0];
	  } /* if */

	TD = VSIw->linest[s]; /* topmost line to delete */
	BD = VSIw->linest[s + n - 1]; /* bottommost line to delete */
	TI = VSIw->linest[VSIw->bottom]; /* insert replacement blank lines after this line */
	BI = TI->next; /* insert them before this line (might be nil) */
	itt = TD; /* start of text lines to be blanked out */
  /* the space taken by the deleted lines will be reused for
	the inserted blank lines */
	if (TD != BI && TI != BD)
	  /* insertion and deletion areas not adjacent -- move the lines to
		their new position */
		VSIlistmove(TD, BD, TI, BI);

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

	for (i = 0; i < n; i++)
	  {
	    itt->lattr = 0;
		ta = itt->attr;
		tt = itt->text;
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*tt++ = ' ';
			*ta++ = attrib;
		  } /* for */
		itt = itt->next;
	  } /* for */

	VSIw->scrntop = ts; /* new topmost line (if it's changed) */
  /* re-sync screen arrays */
	VSIlistndx(ts);
	if (VSIw->Rtop >= 0)
	  /* make sure vistop still points to same line position
		on screen that it did before */
		VSIw->vistop = VSIw->linest[VSIw->Rtop];
  /* and actually display the change on-screen */
	VSIcdellines(VSIwn, s, VSIw->bottom, n, -1); /* Cancel current selection */
  } /* VSIdellines */

void VSOdellines
  (
	short n, /* nr lines to delete */
	short s /* starting line to delete, -ve => line containing cursor */
  )
  /* deletes lines from the screen, scrolling up the remainder and
	inserting new blank lines at the bottom of the scrolling region. */
  {
	short i, j;
	char *tt;
	VSAttrib *ta;
	VSlinePtr  as, ts, TD, BD, TI, BI, itt;
	VSattrlinePtr ita;
	VSAttrib attrib;

	if (s < 0)
		s = VSIw->y;
	if (s + n - 1 > VSIw->bottom)
		n = VSIw->bottom - s + 1; /* don't bother deleting more than scrolling region will hold */

  /* find new tops of screen arrays */
// RAB BetterTelnet 2.0b3
// way way too many bugs squashed
//	if (s == 0 && n <= VSIw->lines)
	if (s == 0 && n <= VSIw->bottom)
	  {
	  /* element for line after last one being deleted */
		ts = VSIw->linest[n];
		as = (VSlinePtr)VSIw->attrst[n];
	  }
	else
	  {
	  /* top line unaffected, or entire screen is being wiped */
		ts = VSIw->linest[0];
		as = (VSlinePtr)VSIw->attrst[0];
	  } /* if */

	TD = VSIw->linest[s]; /* topmost line to delete */
	BD = VSIw->linest[s + n - 1]; /* bottommost line to delete */
	TI = VSIw->linest[VSIw->bottom]; /* insert replacement blank lines after this line */
	BI = TI->next; /* insert them before this line (might be nil) */
	itt = TD; /* start of text lines to be blanked out */
  /* the space taken by the deleted lines will be reused for
	the inserted blank lines */
	if (TD != BI && TI != BD)
	  /* insertion and deletion areas not adjacent -- move the lines to
		their new position */
		VSIlistmove(TD, BD, TI, BI);

	TD = (VSlinePtr)VSIw->attrst[s]; /* topmost line to delete */
	BD = (VSlinePtr)VSIw->attrst[s + n - 1]; /* bottommost line to delete */
	TI = (VSlinePtr)VSIw->attrst[VSIw->bottom]; /* insert new lines after this one */
	BI = TI->next; /* insert them before this line */
  /* perform same rearrangement on attribute lines as on text lines */
	if (TD != BI && TI != BD)
	  /* insertion and deletion areas not adjacent -- move the lines to
		their new position */
		VSIlistmove(TD, BD, TI, BI);

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

  /* blank out the newly-created replacement lines */
	ita = (VSattrlinePtr)TD; /* start of attribute lines to be blanked out */
	for (i = 0; i < n; i++)
	  {
		itt->lattr = 0;
		tt = itt->text;
		ta = ita->text;
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*tt++ = ' ';
			*ta++ = attrib;
		  } /* for */
		ita = ita->next;
		itt = itt->next;
	  } /* for */

	VSIw->scrntop = ts; /* new topmost line (if it's changed) */
  /* re-sync screen arrays */
	VSOlistndx(ts, (VSattrlinePtr)as);
	if (VSIw->Rtop >= 0)
	  /* make sure vistop still points to same line position
		on screen that it did before */
		VSIw->vistop = VSIw->linest[VSIw->Rtop];
  /* and actually display the change on-screen */
	VSIcdellines(VSIwn, s, VSIw->bottom, n, -1); /* Cancel current selection */
  } /* VSOdellines */

void VSIinslines
  (
	short n, /* how many to insert */
	short s /* where to insert them, -ve => line containing cursor */
  )
  /* inserts the specified number of blank lines, scrolling the
	remaining ones down, and dropping off any that fall off the
	end of the scrolling region. */
  {
	short i, j;
	char *tt;
	VSAttrib *ta;		
	VSlinePtr ts, TD, BD, TI, BI, itt;
	VSAttrib attrib;

	VSIflush(); // RAB BetterTelnet 2.0b3

	if (VSIw->oldScrollback) {
		VSOinslines(n, s);
		return;
	}

	if (s < 0)
		s = VSIw->y;
	if (s + n - 1 > VSIw->bottom)
	  /* don't bother inserting more than scrolling region can hold */
		n = VSIw->bottom - s + 1;

  /* find new tops of screen arrays */
	if (s == 0 && n <= VSIw->lines)
	  {
	  /* element for first blank line being inserted */
		ts = VSIw->linest[VSIw->bottom - n + 1];
	  }
	else
	  {
	  /* top line unaffected, or entire screen is being wiped */
		ts = VSIw->linest[0];
	  } /* if */

	BI = VSIw->linest[s]; /* insert blank lines before this one */
	TI = BI->prev; /* insert them after this one */
	TD = VSIw->linest[VSIw->bottom - n + 1]; /* topmost line to delete */
	BD = VSIw->linest[VSIw->bottom]; /* bottommost line to delete */
	itt = TD; /* start of text lines to be blanked out */
  /* the space occupied by the deleted lines will be reused for
	the new blank lines */
	if (TD != BI && TI != BD)
	  /* new and deleted lines not contiguous -- move the space
		to its new position */
		VSIlistmove(TD, BD, TI, BI);

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

  /* blank out the newly-inserted lines */
	for (i = 0; i < n; i++)
	  {
		tt = itt->text;
		ta = itt->attr;
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*tt++ = ' ';
			*ta++ = attrib;
		  }
		itt = itt->next;
	  } /* for */

	VSIw->scrntop = ts;
	VSIlistndx(ts); /* re-sync screen arrays */
	if (VSIw->Rtop >= 0)
	  /* make sure vistop still points to same line position
		on screen that it did before */
		VSIw->vistop = VSIw->linest[VSIw->Rtop];
  /* update display to match reality */
	VSIcinslines(VSIwn, s, VSIw->bottom, n, -1);  /* Destroy selection area if this is called tooo */
  } /* VSIinslines */

void VSOinslines
  (
	short n, /* how many to insert */
	short s /* where to insert them, -ve => line containing cursor */
  )
  /* inserts the specified number of blank lines, scrolling the
	remaining ones down, and dropping off any that fall off the
	end of the scrolling region. */
  {
	short i, j;
	char *tt;
	VSAttrib *ta;		
	VSlinePtr ts, TD, BD, TI, BI, itt;
	VSattrlinePtr as, aTD, aBD, aTI, aBI, ita;
	VSAttrib attrib;

	if (s < 0)
		s = VSIw->y;
	if (s + n - 1 > VSIw->bottom)
	  /* don't bother inserting more than scrolling region can hold */
		n = VSIw->bottom - s + 1;

  /* find new tops of screen arrays */
	if (s == 0 && n <= VSIw->lines)
	  {
	  /* element for first blank line being inserted */
		ts = VSIw->linest[VSIw->bottom - n + 1];
		as = VSIw->attrst[VSIw->bottom - n + 1];
	  }
	else
	  {
	  /* top line unaffected, or entire screen is being wiped */
		ts = VSIw->linest[0];
		as = VSIw->attrst[0];
	  } /* if */

	BI = VSIw->linest[s]; /* insert blank lines before this one */
	TI = BI->prev; /* insert them after this one */
	TD = VSIw->linest[VSIw->bottom - n + 1]; /* topmost line to delete */
	BD = VSIw->linest[VSIw->bottom]; /* bottommost line to delete */
	itt = TD; /* start of text lines to be blanked out */
  /* the space occupied by the deleted lines will be reused for
	the new blank lines */
	if (TD != BI && TI != BD)
	  /* new and deleted lines not contiguous -- move the space
		to its new position */
		VSIlistmove(TD, BD, TI, BI);

	aBI = VSIw->attrst[s]; /* insert new lines before this one */
	aTI = aBI->prev; /* insert them after this one */
	aTD = VSIw->attrst[VSIw->bottom - n + 1]; /* topmost line to delete */
	aBD = VSIw->attrst[VSIw->bottom]; /* bottommost line to delete */
  /* do the same rearrangement on the attribute lines */
	if (aTD != aBI && aTI != aBD)
	  /* new and deleted lines not contiguous -- move the space
		to its new position */
		VSIlistmove((VSlinePtr)aTD, (VSlinePtr)aBD, (VSlinePtr)aTI, (VSlinePtr)aBI); 

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

  /* blank out the newly-inserted lines */
	ita = aTD; /* start of attribute lines to be blanked out */
	for (i = 0; i < n; i++)
	  {
		itt->lattr = 0;
		tt = itt->text;
		ta = ita->text;
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*tt++ = ' ';
			*ta++ = attrib;
		  }
		itt = itt->next;
		ita = ita->next;
	  } /* for */

	VSIw->scrntop = ts;
	VSOlistndx(ts, as); /* re-sync screen arrays */
	if (VSIw->Rtop >= 0)
	  /* make sure vistop still points to same line position
		on screen that it did before */
		VSIw->vistop = VSIw->linest[VSIw->Rtop];
  /* update display to match reality */
	VSIcinslines(VSIwn, s, VSIw->bottom, n, -1);  /* Destroy selection area if this is called tooo */
  } /* VSOinslines */

void VSIscroll
  (
	void
  )
  /* scrolls scrolling region up one line. */
  {
	register char *temp;
	register VSAttrib *tempa;
	static VSlinePtr tmp, tmp2, tmp3, tmp4;
	register short i;
	short theBottom;				/* NCSA 2.5: the correct screen bottom */
	short scrolled;

	if (VSIw->y > VSIw->lines)		/* BYU - replaces BYU modification below */
		return;						/* BYU */

	if (VSIw->oldScrollback) {
		VSOscroll();
		return;
	}

	if ((!VSIw->savelines) /* no scrollback */    ||    (VSIw->top != 0)) 	/* NCSA 2.5 */
//			|| ((VSIw->bottom != VSIw->lines) && !VSIw->forcesave))	/* NCSA 2.5 */
			  /* region being scrolled is not entire screen */		

	  /* no saving of lines */
		VSIdellines(1, VSIw->top);
	else
	  {
	  /* scrolling region is entire screen, and lines are being saved off top */
		if (VSIw->linest[VSIw->lines]->next == 0L)
		  {
		  /* all currently-allocated scrollback lines have been used, but
			scrollback buffer isn't at its full size -- allocate some more
			space */
			i = VSIw->maxlines - VSIw->numlines; /* number of lines that can be allocated */
			if (i > ScrollbackQuantum)
				i = ScrollbackQuantum; /* don't bother allocating more than this at once */
			if ((i > 0) && (tmp = VSInewlines(i)) != 0L)
			  {
			  /* link newly-allocated lines into the list */
				VSIw->linest[VSIw->lines]->next = tmp;
				tmp->prev = VSIw->linest[VSIw->lines];
				VSIw->numlines++; /* use one of the newly-allocated scrollback lines */
				RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar accordingly */
			} else {
			  /* not enough memory to extend scrollback buffer--reuse
				oldest line and give up on future extensions */
				VSIw->linest[VSIw->lines]->next = VSIw->buftop;		/* Make it circular */
				VSIw->buftop->prev = VSIw->linest[VSIw->lines];
				VSIw->buftop = VSIw->buftop->next;	/* step one forward */
			} /* if */
		} else {
		  	/* either there's allocated, but not yet used, space at
			VSIw->linest[VSIw->lines]->next, or the text line list
			is circular. Either way, don't do any new scrollback
			allocation. */
			if (VSIw->linest[VSIw->lines]->next == VSIw->buftop) {
				/* scrollback buffer is at full size--reuse oldest line */
				VSIw->buftop = VSIw->buftop->next;
			} else {
			  /* haven't used up all the space I allocated last time */
				VSIw->numlines++;					/* count another line */
				RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar accordingly */
			} /* if */
		} /* if */

		VSIw->scrntop = VSIw->scrntop->next; /* scroll the screen buffer */
		VSIlistndx(VSIw->scrntop); /* update screen arrays */
	  /* reflect the change in the display by scrolling up the visible
		part of the on-screen area, if any */

//		if (VSIw->forcesave)
		theBottom = VSIw->bottom;	/* NCSA 2.5: get the correct scroll rgn */
//		else theBottom = VSIw->Rbottom;				/* NCSA 2.5: just use whole screen */

/*
		if (RSlocal[VSIwn].last.v < VSIw->Rtop) {
			scrolled = 1;
		} else if (RSlocal[VSIwn].anchor.v > theBottom) {
			scrolled = -1;
		} else {
			scrolled = 0;
		}
*/
		scrolled = -1;

		if (VSIcdellines(VSIwn, VSIw->Rtop, theBottom, 1, scrolled))	/* NCSA 2.5 */
		  {
		  /* no part of on-screen area is visible */
			if (VSIw->Rtop > -VSIw->numlines)
			  /* update bounds of visible region to be consistent
				with portion of scrollback buffer still being displayed */
			  {
				VSIw->Rtop--;
				VSIw->Rbottom--;
			  }
			else
			  {
			  /* displaying right from top of scrollback buffer. Topmost
				line being shown has in fact vanished. Update the display
				to show this fact. */
				VSIw->vistop = VSIw->vistop->next;
				RSdellines(VSIwn, 0, VSIw->Rbottom - VSIw->Rtop, 1, scrolled);
			  } /* if */
		  }
		else
			VSIw->vistop = VSIw->vistop->next; /* consistent with changed display */

		/* clear line attribute */
		VSIw->linest[VSIw->lines]->lattr = 0;
		/* blank out newly-revealed bottom line */
		tempa = VSIw->linest[VSIw->lines]->attr;
		temp = VSIw->linest[VSIw->lines]->text;
		for (i = 0; i <= VSIw->allwidth; i++) {
			*temp++ = ' ';
			*tempa++ = 0;
		} /* for */

// RAB BetterTelnet 1.2.1 - another bug fix for NCSA's insanity!

	if (VSIw->bottom != VSIw->lines) { // we're forcesaving here...
		tmp = VSIw->linest[VSIw->lines]->next;
		tmp2 = VSIw->linest[VSIw->lines];
		tmp3 = VSIw->linest[VSIw->bottom];
		tmp4 = VSIw->linest[VSIw->lines]->prev;
		VSIw->linest[VSIw->bottom]->prev->next = tmp2;
		tmp2->prev = VSIw->linest[VSIw->bottom]->prev;
		tmp2->next = tmp3;
		tmp3->prev = tmp2;
		tmp4->next = tmp;
		if (tmp4->next) tmp4->next->prev = tmp4;
		if (VSIw->scrntop == VSIw->linest[VSIw->bottom]) VSIw->scrntop = tmp2;
		VSIlistndx(VSIw->scrntop);
	}

	  } /* if */

  } /* VSIscroll */
  
void VSOscroll
  (
	void
  )
  /* scrolls scrolling region up one line. */
  {
	register char *temp;
	register VSAttrib *tempa;
	register short i;
	short theBottom;				/* NCSA 2.5: the correct screen bottom */
	static VSlinePtr tmp, tmp2, tmp3, tmp4;
	short scrolled;

	if (VSIw->y > VSIw->lines)		/* BYU - replaces BYU modification below */
		return;						/* BYU */


	if ((!VSIw->savelines) /* no scrollback */    ||    (VSIw->top != 0)) 	/* NCSA 2.5 */
//			|| ((VSIw->bottom != VSIw->lines) && !VSIw->forcesave))	/* NCSA 2.5 */
			  /* region being scrolled is not entire screen */		

	  /* no saving of lines */
		VSOdellines(1, VSIw->top);
	else
	  {
	  /* scrolling region is entire screen, and lines are being saved off top */
		if (VSIw->linest[VSIw->lines]->next == 0L)
		  {
		  /* all currently-allocated scrollback lines have been used, but
			scrollback buffer isn't at its full size -- allocate some more
			space */
			i = VSIw->maxlines - VSIw->numlines; /* number of lines that can be allocated */
			if (i > ScrollbackQuantum)
				i = ScrollbackQuantum; /* don't bother allocating more than this at once */
			if ((i > 0) && (tmp = VSOnewlines(i,1)) != 0L)
			  {
			  /* link newly-allocated lines into the list */
				VSIw->linest[VSIw->lines]->next = tmp;
				tmp->prev = VSIw->linest[VSIw->lines];
				VSIw->numlines++; /* use one of the newly-allocated scrollback lines */
				RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar accordingly */
			  }
			else
			  {
			  /* not enough memory to extend scrollback buffer--reuse
				oldest line and give up on future extensions */
				VSIw->linest[VSIw->lines]->next = VSIw->buftop;		/* Make it circular */
				VSIw->buftop->prev = VSIw->linest[VSIw->lines];
				VSIw->buftop = VSIw->buftop->next;	/* step one forward */
			  } /* if */
		  }	
		else
		  {
		  /* either there's allocated, but not yet used, space at
			VSIw->linest[VSIw->lines]->next, or the text line list
			is circular. Either way, don't do any new scrollback
			allocation. */
			if (VSIw->linest[VSIw->lines]->next == VSIw->buftop)
			  /* scrollback buffer is at full size--reuse oldest line */
				VSIw->buftop = VSIw->buftop->next;
			else
			  {
			  /* haven't used up all the space I allocated last time */
				VSIw->numlines++;					/* count another line */
				RSbufinfo(VSIwn, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* update vertical scroll bar accordingly */
			  } /* if */
		  } /* if */

		VSIw->scrntop = VSIw->scrntop->next; /* scroll the screen buffer */
		VSOlistndx(VSIw->scrntop, VSIw->attrst[1]); /* update screen arrays */
	  /* reflect the change in the display by scrolling up the visible
		part of the on-screen area, if any */


//		if (VSIw->forcesave)
		theBottom = VSIw->bottom;	/* NCSA 2.5: get the correct scroll rgn */
//		else theBottom = VSIw->Rbottom;				/* NCSA 2.5: just use whole screen */

/*
		if (RSlocal[VSIwn].last.v < VSIw->Rtop) {
			scrolled = 1;
		} else if (RSlocal[VSIwn].anchor.v > theBottom) {
			scrolled = -1;
		} else {
			scrolled = 0;
		}
*/
		scrolled = -1;

		if (VSIcdellines(VSIwn, VSIw->Rtop, theBottom, 1, scrolled))	/* NCSA 2.5 */
		  {
		  /* no part of on-screen area is visible */
			if (VSIw->Rtop > -VSIw->numlines)
			  /* update bounds of visible region to be consistent
				with portion of scrollback buffer still being displayed */
			  {
				VSIw->Rtop--;
				VSIw->Rbottom--;
			  }
			else
			  {
			  /* displaying right from top of scrollback buffer. Topmost
				line being shown has in fact vanished. Update the display
				to show this fact. */
				VSIw->vistop = VSIw->vistop->next;
				RSdellines(VSIwn, 0, VSIw->Rbottom - VSIw->Rtop, 1, scrolled);
			  } /* if */
		  }
		else
			VSIw->vistop = VSIw->vistop->next; /* consistent with changed display */
	  /* blank out newly-revealed bottom line */
		VSIw->linest[VSIw->lines]->lattr = 0;
		tempa = VSIw->attrst[VSIw->lines]->text;
		temp = VSIw->linest[VSIw->lines]->text;
		for (i = 0; i <= VSIw->allwidth; i++)
		  {
			*temp++ = ' ';
			*tempa++ = 0;
		  } /* for */
// RAB BetterTelnet 1.2.1 - Return of the Bug Fix!
		if (VSIw->bottom != VSIw->lines) { // we're forcesaving here...
			tmp = VSIw->linest[VSIw->lines]->next;
			tmp2 = VSIw->linest[VSIw->lines];
			tmp3 = VSIw->linest[VSIw->bottom];
			tmp4 = VSIw->linest[VSIw->lines]->prev;
			VSIw->linest[VSIw->bottom]->prev->next = tmp2;
			tmp2->prev = VSIw->linest[VSIw->bottom]->prev;
			tmp2->next = tmp3;
			tmp3->prev = tmp2;
			tmp4->next = tmp;
			if (tmp4->next) tmp4->next->prev = tmp4;
			if (VSIw->scrntop == VSIw->linest[VSIw->bottom]) VSIw->scrntop = tmp2;

			// now for the attribute lines...
			VSIw->attrst[VSIw->lines]->next->prev = VSIw->attrst[VSIw->lines]->prev;
			VSIw->attrst[VSIw->lines]->prev->next = VSIw->attrst[VSIw->lines]->next;
			VSIw->attrst[VSIw->bottom]->prev->next = VSIw->attrst[VSIw->lines];
			VSIw->attrst[VSIw->lines]->prev = VSIw->attrst[VSIw->bottom]->prev;
			VSIw->attrst[VSIw->lines]->next = VSIw->attrst[VSIw->bottom];
			VSIw->attrst[VSIw->bottom]->prev = VSIw->attrst[VSIw->lines];
			VSOlistndx(VSIw->scrntop, VSIw->attrst[0]);
		}

	  } /* if */

} /* VSOscroll */

// RAB BetterTelnet 2.0b3 - flush the jump scroller here
void VSIflush(void)
{
	if (!VSIw->linesjumped) return; // nothing to flush
	RSlocal[VSIwn].skip = 0;
	VSIcdellines(VSIwn, VSIw->top, VSIw->bottom, VSIw->linesjumped, -VSIw->linesjumped);

	if (VSIw->linesjumped > (VSIw->bottom - VSIw->top))
		VSredraw(VSIwn, 0, VSIw->top - VSIw->Rtop, VSIw->maxwidth,
			VSIw->bottom - VSIw->Rtop); // redraw the whole thing
	else // redraw just a part of the screen
		VSredraw(VSIwn, 0, VSIw->bottom - VSIw->linesjumped + 1 - VSIw->Rtop,
			VSIw->maxwidth, VSIw->bottom - VSIw->Rtop);

	VSIw->linesjumped = 0;
}

void VSIindex
  (
	void
  )
  /* moves cursor down one line, unless it's at the bottom of
	the scrolling region, in which case scrolls up one. */
	// RAB BetterTelnet 2.0b3 - added jump scrolling so it runs faster
{
	short sn = findbyVS(VSIwn);

	if (VSIw->y == VSIw->bottom) {	/* BYU - changed "==" to ">=" and back again */
		if (VSIw->jumpScroll)
		if (VSIw->linesjumped || !RSlocal[VSIwn].skip)
		/* if ((VSIw->top == 0) && (VSIw->bottom == VSIw->lines)) */ {
			RSlocal[VSIwn].skip = 1;
			VSIw->linesjumped++;
			if (VSIw->linesjumped > (VSIw->bottom - VSIw->top)) VSIflush();
		}
		VSIscroll();
	}
	else if (VSIw->y < VSIw->lines) 	/* BYU  - added "if ... " */
		VSIw->y++;

	VSIw->lattrib = 0;

	// back to default input translation ?
	if ( sn >= 0 ) {
		WindRec *tw = &screens[sn];
		switchintranslation(tw, tw->outnational, tw->outcharset);
	}

} /* VSIindex */

void VSIwrapnow(short *xp, short *yp)
  /* checks current cursor position for VSIw to see if
	it's within bounds, wrapping to next line if not.
	Returns correct cursor position in either case in *xp
	and *yp. */
  {
	short mw = VSIw->maxwidth;

	if (VSisdecdwh(VSIw->lattrib)) {
		mw >>= 1;
		if ( !(VSIw->maxwidth & 1) )
			mw -= 1;
	}
	if (VSIw->x > mw) 
	  {
		VSIw->x = 0;
		VSIindex();
	  } /* if */
	*xp = VSIw->x;
	*yp = VSIw->y;
  } /* VSIwrapnow */

void VSIeeol
  (
	void
  )
  /* erases characters to the end of the current line. */
  {
	char
		*tt;
	VSAttrib 
		*ta;
	short
		x1 = VSIw->x,
		y1 = VSIw->y,
		x2 = VSIw->maxwidth,
		y2 = VSIw->y,
		n = -1,
		offset;
	short
		i;
	VSAttrib attrib;

// RAB BetterTelnet 2.0b1 - Fix for forcesave bug

	VSlinePtr savedTextBlock, savedTextPtr;

	if ((VSIw->x == 0)&&(VSIw->y == 0)&&(VSIw->possibleForce == 1))
	{
		VSIw->possibleForce = 0;
		savedTextBlock = VSInewlines(VSIw->lines + 1);
		savedTextPtr = savedTextBlock;
		for (i = 0; i <= VSIw->lines; i++) {
			BlockMoveData(VSIw->linest[i]->text, savedTextPtr->text, VSIw->allwidth + 1);
			if (savedTextPtr->next) savedTextPtr = savedTextPtr->next;
		}
		savedTextPtr = savedTextBlock;
		for (i = 0; i <= VSIw->lines; i++) {
			savedTextPtr->lattr = VSIw->linest[i]->lattr;
			if (VSIw->oldScrollback) {
				BlockMoveData(VSIw->attrst[i]->text, savedTextPtr->attr, (VSIw->allwidth + 1) * sizeof(VSAttrib));
			} else {
				BlockMoveData(VSIw->linest[i]->attr, savedTextPtr->attr, (VSIw->allwidth + 1) * sizeof(VSAttrib));
			}
			if (savedTextPtr->next) savedTextPtr = savedTextPtr->next;
		}

		VSIscroff();

		savedTextPtr = savedTextBlock;
		for (i = 0; i <= VSIw->lines; i++) {
			BlockMoveData(savedTextPtr->text, VSIw->linest[i]->text, VSIw->allwidth + 1);
			if (savedTextPtr->next) savedTextPtr = savedTextPtr->next;
		}
		savedTextPtr = savedTextBlock;
		for (i = 0; i <= VSIw->lines; i++) {
			VSIw->linest[i]->lattr = savedTextPtr->lattr;
			if (VSIw->oldScrollback) {
				BlockMoveData(savedTextPtr->attr, VSIw->attrst[i]->text, (VSIw->allwidth + 1) * sizeof(VSAttrib));
			} else {
				BlockMoveData(savedTextPtr->attr, VSIw->linest[i]->attr, (VSIw->allwidth + 1) * sizeof(VSAttrib));
			}
			if (savedTextPtr->next) savedTextPtr = savedTextPtr->next;
		}
		DisposePtr((Ptr) savedTextBlock); // VSIfreelinelist adds un-needed overhead here
	}

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

	VSIwrapnow(&x1, &y1);
	y2 = y1;
  /* clear out screen line */
  	//VSIw->linest[y1]->lattr = 0;
  	if (VSIw->oldScrollback) {
  		ta = &VSIw->attrst[y1]->text[x1];
	} else {
		ta = &VSIw->linest[y1]->attr[x1];
	}
	tt = &VSIw->linest[y1]->text[x1];
	for (i = VSIw->allwidth - x1 + 1; i > 0; i--)
	  {
		*ta++ = attrib;
		*tt++ = ' ';
	  }
  /* update display */
	if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) {
		//RSerase(VSIwn, x1, y1, x2, y2);
		VSredraw(VSIwn, x1, y1, x2, y2);
	}
  } /* VSIeeol */

void VSIdelchars
  (
	short x /* how many characters to delete */
  )
  /* deletes characters at the current cursor position onwards,
	moving the remainder of the line to the left. */
  {
	short
		i;
	short
		x1 = VSIw->x,
		y1 = VSIw->y,
		x2 = VSIw->maxwidth,
		y2 = VSIw->y,
		n = -1,
		offset;
	char
		*temp;
	VSAttrib
		*tempa;
	short
		lattr;
	VSAttrib attrib;

	VSIwrapnow(&x1, &y1);
	y2 = y1;

	if (x > VSIw->maxwidth)
		x = VSIw->maxwidth;
	lattr = VSIw->linest[y1]->lattr;
	if (VSIw->oldScrollback) {
		tempa = VSIw->attrst[y1]->text;
	} else {
		tempa = VSIw->linest[y1]->attr;
	}
	temp = VSIw->linest[y1]->text;

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

	for (i = x1; i <= VSIw->maxwidth - x; i++)
	  {
	  /* move remainder of line to the left */
		temp[i] = temp[x + i];
		tempa[i] = tempa[x + i];
	  }
	for (i = VSIw->maxwidth - x + 1; i <= VSIw->allwidth; i++)
	  {
	  /* insert blank characters after end of line */
		temp[i] = ' ';
		tempa[i] = attrib;
	  }
  /* update display */
	if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset))
		RSdelchars(VSIwn, lattr, x1, y1, x);
  } /* VSIdelchars */

void VSIfreelinelist
  (
	VSlinePtr listhead
  )
  /* frees up the list of line elements pointed to by listhead. */
  {
	register VSlinePtr
		ThisElt, NextElt, ToFree;

	ThisElt = listhead;
	ToFree = nil;
	while (true)
	  {
		if (ThisElt == nil)
			break;
		NextElt = ThisElt->next;
		if (ThisElt->mem)
		  {
			ThisElt->next = ToFree;
			ToFree = ThisElt;
		  } /* if */
		ThisElt = NextElt;
		if (ThisElt == listhead)
			break;
	  } /* while */
	while (ToFree)
	  {
		NextElt = ToFree->next;
		DisposePtr((Ptr) ToFree);
		if (VSIw->oldScrollback)
			DisposePtr((Ptr) ToFree->text);
		ToFree = NextElt;
	  } /* while */
  } /* VSIfreelinelist */

void VSIfreelines
  (
	void
  )
  /* frees up all the memory allocated for screen and scrollback
	text lines for the current screen. */
  {
	VSIfreelinelist(VSIw->buftop);
  } /* VSIfreelines */

void VSIrindex
  (
	void
  )
  /* moves cursor up one line, unless it's at the top of
	the scrolling region, in which case scrolls down one. */
  {
  	VSIflush(); // RAB BetterTelnet 2.0b3
	if (VSIw->y == VSIw->top)
		VSIinslines(1, VSIw->top);
	else
		VSIw->y--;
  } /* VSIrindex */

void VSIebol
  (
	void
  )
  /* erases characters from beginning of line to cursor. */
  {
	char
		*tt;
	VSAttrib
		*ta;
	short
		x1 = 0,
		y1 = VSIw->y,
		x2 = VSIw->x,
		y2 = VSIw->y,
		n = -1,
		offset;
	short
		i;
	VSAttrib attrib;

	VSIwrapnow(&x2, &y1);
	y2 = y1;
  /* clear from beginning of line to cursor */
	//VSIw->linest[y1]->lattr = 0;
	if (VSIw->oldScrollback) {
		ta = &VSIw->attrst[y1]->text[0];
	} else {
		ta = &VSIw->linest[y1]->attr[0];
	}

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

	tt = &VSIw->linest[y1]->text[0];
	for (i = 0; i <= x2; i++)
	  {
		*ta++ = attrib;
		*tt++ = ' ';
	  }
  /* update display */
	if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) 
		RSerase(VSIwn, x1, y1, x2, y2);
  } /* VSIebol */

void VSIel
  (
	short s /* line to clear, -ve => line containing cursor */
  )
  /* erases the specified line. */
  {
	char *tt;
	VSAttrib *ta;
	short x1 = 0, y1 = s, x2 = VSIw->maxwidth, y2 = s, n = -1, offset;
	short i;
	VSAttrib attrib;

	if (s < 0)
	  {
		VSIwrapnow(&x1, &y1);
		s = y2 = y1;
		x1 = 0;
	  } 
  /* clear out line */
	//VSIw->linest[s]->lattr = 0;
	if (VSIw->oldScrollback) {
		ta = &VSIw->attrst[s]->text[0];
	} else {
		ta = &VSIw->linest[s]->attr[0];
	}

	if (VSIw->vtemulation == 3)
		attrib = VSIw->attrib;
	else
		// all off, keep multi-byte / graphic
		attrib = VSIw->attrib & (kVSansi2b | kVSgrph);

	tt = &VSIw->linest[s]->text[0];
	for(i = 0; i <= VSIw->allwidth; i++)
	  {
		*ta++ = attrib;
		*tt++ = ' ';
	  }
  /* update display */
	if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) 
		RSerase(VSIwn, x1, y1, x2, y2);
  } /* VSIel */

void VSIeeos
  (
	void
  )
  /* erases characters from cursor to end of screen. */
  {
	short
		i;
	short
		x1 = 0,
		y1 = VSIw->y + 1,
		x2 = VSIw->maxwidth,
		y2 = VSIw->lines,
		n = -1,
		offset;

	VSIflush(); // RAB BetterTelnet 2.0b3

	VSIwrapnow(&x1, &y1);
	y1++;
	x1 = 0;

	i = y1;
 
 	if ((VSIw->ESscroll)&&(VSIw->y == 0)&&(VSIw->x == 0)) //trashes screen unless we are in 0,0
		VSIscroff();

	VSIw->possibleForce = FALSE; //CCP better forcesave stuff
  /* erase complete lines from screen */
	if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) 
		RSerase(VSIwn, x1, y1, x2, y2);
  /* blank out current line from cursor to end */
	VSIeeol(); /* this also erases the partial line on-screen */
  /* blank out remaining lines to end of screen */
	while (i <= VSIw->lines)
	  {
		VSIelo(i);
		i++;
	  } /* while */
	if (VSIw->y < VSIw->lines && (VSIw->x <= VSIw->maxwidth))
	  /* erase the partial line (what--again??) */
		if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) 
			RSerase(VSIwn, x1, y1, x2, y2);
  } /* VSIeeos */

void VSIebos
  (
	void
  )
  /* erases characters from beginning of screen to cursor. */
  {
	short i;
	short x1, y1, x2 = VSIw->maxwidth, y2, n = -1, offset;

	VSIflush(); // RAB BetterTelnet 2.0b3

	VSIwrapnow(&x1, &y1);
	y2 = y1 - 1;
	x1 = 0;
	y1 = 0;
  /* blank out current line from beginning to cursor */
	VSIebol(); /* this also erases the partial line on-screen */
	i = 0;
  /* blank out remaining lines from beginning of screen to previous line */
	while (i < (y2 + 1))
	{
		VSIelo(i);
		i++;
	} 
	if (y2 >= 0)
	  /* erase the partial line (what--again??) */
		if (!VSIclip(&x1, &y1, &x2, &y2, &n, &offset)) 
			RSerase(VSIwn, x1, y1, x2, y2);
  } /* VSIebos */

void VSIrange
  (
	void
  )
  /* constrains cursor position to valid range (somewhere on the screen). */
  {
	short wrap = (VSIw->DECAWM) ? 1 : 0;
	short mw = VSIw->maxwidth;

	if (VSisdecdwh(VSIw->lattrib)) {
		mw >>= 1;
		if ( !(VSIw->maxwidth & 1) )
			mw -= 1;
	}
	if (VSIw->x < 0)
		VSIw->x = 0;
	if (VSIw->x > mw + wrap)
		VSIw->x = mw + wrap;
	if (VSIw->y < 0)
		VSIw->y = 0;
	if (VSIw->y > VSIw->lines)
		VSIw->y = VSIw->lines;
  } /* VSIrange */

/*
 * VTsendpos
 *
 * sends an escape sequence representing the current cursor position.
 */

void VTsendpos( void )
{
	char
		tempbuf[19];
	short
		x = VSIw->x,
		y = VSIw->y;
	short mw = VSIw->maxwidth;

	if (VSisdecdwh(VSIw->lattrib)) {
		mw >>= 1;
		if ( !(VSIw->maxwidth & 1) )
			mw -= 1;
	}

	if (x > mw) {
		/* autowrap pending */
		x = 0;
		y++;
	}
	if (y > VSIw->lines)
		/* scroll pending (because of the autowrap) */
		y = VSIw->lines;

	sprintf(tempbuf, "\033[%d;%dR", y + 1, x + 1);
	RSsendstring(VSIwn, tempbuf, strlen(tempbuf));
} /* VTsendpos */


/*
 * VTsendprintstat
 *
 * sends the printer status string.
 */

void VTsendprintstat( void )
{
	/* FIXME: always ready... */
	//RSsendstring(VSIwn, "\033[?13n", 6);	/* no printer */
	RSsendstring(VSIwn, "\033[?10n", 6);	/* The printer is ready */
	//RSsendstring(VSIwn, "\033[?11n", 6);	/* The printer is not ready */
} /* VTsendprintstat */


/*
 * VTsendudkstat
 *
 * sends the used defined key status string.
 */

void VTsendudkstat( void )
{
	/* FIXME: always locked... */
	RSsendstring(VSIwn, "\033[?21n", 6);	/* User Defined Keys are locked */
	//RSsendstring(VSIwn, "\033[?20n", 6);	/* User Defined Keys are unlocked */
} /* VTsendudkstat */


/*
 * VTsendstat
 *
 * sends the terminal status string.
 */

void VTsendstat( void )
{
	RSsendstring(VSIwn, "\033[0n", 4);
} /* VTsendstat */


/*
 * VTsendident
 *
 * sends an appropriate terminal identification sequence.
 */

void VTsendident( void )
{
	if (VSIw->vtemulation)
		//RSsendstring(VSIwn, "\033[?62;1;6c", 10);			// VT200-series
		RSsendstring(VSIwn, "\033[?62;1;2;6;7;8c", 16);		// VT200-series
	else
		RSsendstring(VSIwn, "\033[?6c", 5);					// VT102
} /* VTsendident */


/*
 * VTsendsecondaryident
 *
 * sends an appropriate terminal identification sequence.
 */

void VTsendsecondaryident( void )
{
	char identstr[64];

  	sprintf(identstr, "\033[>1;10;0c");
	RSsendstring(VSIwn, identstr, strlen(identstr));
} /* VTsendsecondaryident */


void VTalign
  (
	void
  )
  /* fills screen with uppercase "E"s, for checking screen alignment. */
  /* Yeah, right. */
  {
	char *tt;
	short i, j;

	VSIes();		/* erase the screen */
	for (j = 0; j < VSIw->lines; j++)
	  {
		VSIw->linest[j]->lattr = 0;
		tt = &VSIw->linest[j]->text[0];
		for (i = 0; i <= VSIw->maxwidth; i++)
			*tt++ = 'E';
	  } /* for */
  /* update the display */
	VSredraw(VSIwn, 0, 0,
		(VSIw->Rright - VSIw->Rleft), (VSIw->Rbottom - VSIw->Rtop));
  } /* VTalign */

void VSIapclear
  (
	void
  )
  /* initializes all the parameters for the current control
	sequence, and the current param index, to zero. */
  {
	short
		parmptr = maxparms;
	while (--parmptr >= 0)
		VSIw->parms[parmptr] = -1;
	VSIw->parmptr = 0;
  } /* VSIapclear */

void VSIsetoption
  (
	short toggle /* 1 => set, 0 => reset */
  )
  /* sets/resets various options, as specified by the parms in
	the current control sequence. Note that this implementation
	will not set/reset more than one option at a time! */
  {
	short
		WindWidth = VSIw->Rright - VSIw->Rleft;

	switch (VSIw->parms[0])
	  {
		case -2: /* DEC-private control sequence */
			switch (VSIw->parms[1])
			  {
				case 1: /* cursor-key mode */
					VSIw->DECCKM = toggle;
					break;
				case 3: /* 80/132 columns */
	//				VSIw->x = VSIw->y = 0; /* home cursor */
	//				VSIes(); /* and clear screen */
					VSIflush(); // RAB BetterTelnet 2.0b3
					if (toggle)	/* 132 column mode */
						{												/* NCSA: SB */
/* NONO */
						VSIw->maxwidth = 131;							/* NCSA: SB */
						RScalcwsize(VSIwn,132);							/* NCSA: SB */
						/*VSIw->maxwidth = 254;*/							/* NCSA: SB */
						/*RScalcwsize(VSIwn,255);*/							/* NCSA: SB */
						}												/* NCSA: SB */
					else												/* NCSA: SB */
						{												/* NCSA: SB */
						VSIw->maxwidth = 79;							/* NCSA: SB */
						RScalcwsize(VSIwn,80);							/* NCSA: SB */
						}												/* NCSA: SB */
				  /* update scroll bars */									
					RSmargininfo(VSIwn, VSIw->maxwidth, VSIw->Rleft);	/* NCSA: SB */	
					break;
					
/* NCSA: SB -  this next one will allow us to flip the foreground and		*/
/*		background colors.													*/
				case 5:											/* NCSA: SB - screen mode */
					VSIflush(); // RAB BetterTelnet 2.0b3
					RSbackground(VSIwn,toggle);
					break;
				
				case 6: /* origin mode */
					VSIw->DECORG = toggle;
					break;
				case 7: /* autowrap mode */
					VSIw->DECAWM = toggle;
					break;

				case 25: /* cursor mode */
					VSIw->DECCM = toggle;
					break;
				default:
					break;
			  } /* switch */
			break;
		case  4: /* insert/replace character writing mode */
			VSIw->IRM = toggle;
			break;
		default:
			break;
	  } /* switch */
  } /* VSIsetoption */

void VSItab //BUGG
  (
	void
  )
  /* advances VSIw->x to the next tab position. */
  {
	
	VSIw->x += VSIgetNextTabDistance();
  
  } /* VSItab */

void VSIinschar
  (
	short x, /* number of blanks to insert */
	short clear
  )
  /* inserts the specified number of blank characters at the
	current cursor position, moving the rest of the line along,
	losing any characters that fall off the right margin.
	Does not update the display. */
  {
	short i, j; 
	char  *temp;
	VSAttrib *tempa;
	VSAttrib attrib;

	VSIwrapnow(&i, &j);

	if (VSIw->oldScrollback)
		tempa = VSIw->attrst[VSIw->y]->text;
	else
		tempa = VSIw->linest[VSIw->y]->attr;
	temp = VSIw->linest[VSIw->y]->text;
	for (i = VSIw->maxwidth - x; i >= VSIw->x; i--) {
		/* move along remaining characters on line */
		temp[x + i] =temp[i];
		tempa[x + i] = tempa[i];
	}
	if ( clear ) {

		if (VSIw->vtemulation == 3)
			attrib = VSIw->attrib & (kVSansi2b | kVSgrph);
		else
			// all off, keep multi-byte / graphic
			attrib = VSIw->attrib;

		for (i = VSIw->x; i < VSIw->x + x; i++) {
			/* insert appropriate number of blanks */
			temp[i] = ' ';
			tempa[i] = attrib;
		}
	}
} /* VSIinschar */

void VSIinsstring
  (
	short len,
	char *start
  )
  /* updates the screen to show insertion of a string of characters
	at the current cursor position. The text has already been
	inserted into the screen buffer. Also, the cursor position has
	already been updated, so the part needing redrawing begins at column
	(VSIw->x - len). */
  {
		RSinsstring(VSIwn, VSIw->x - len, VSIw->y,
			VSIw->lattrib, VSIw->attrib, len, start);
  } /* VSIinsstring */

void VSIsave
  (
	void
  )
  /* saves the current cursor position and attribute settings. */
  {
	VSIw->Px = VSIw->x;
	VSIw->Py = VSIw->y;
	VSIw->Pattrib = VSIw->attrib;
  } /* VSIsave */

void VSIrestore
  (
	void
  )
  /* restores the last-saved cursor position and attribute settings. */
  {
	if (VSIw->Pattrib == 0xffffffff)
	  /* no previous save */
		return;

	VSIflush(); // RAB BetterTelnet 2.0b3

	VSIw->x = VSIw->Px;
	VSIw->y = VSIw->Py;
	VSIrange();
	// keep the graphics character set selection
	VSIw->attrib &= kVSgrph;
	VSIw->attrib |= VSIw->Pattrib;
  } /* VSIrestore */

void VSIdraw
  (
	short VSIwn, /* window number */
	short x, /* starting column */
	short y, /* line on which to draw */
	short la,
	VSAttrib a, /* text attributes */
	short len, /* length of text to draw */
	char *c /* pointer to text */
  )
  /* displays a piece of text (assumed to fit on a single line) on a
	screen, using the specified attributes, and clipping to the
	current visible region. Highlights any part of the text lying
	within the current selection. */
  {
	short x2, y2, offset;

	if (!VSIclip(&x, &y, &x2, &y2, &len, &offset))
		RSdraw(VSIwn, x, y, la, a, len, (char *) (c + offset));	/* BYU LSC */
  } /* VSIdraw */

short VSIgetblinkflag(void)
{
	return VSIw->realBlink;
}
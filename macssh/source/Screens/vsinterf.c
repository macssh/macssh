// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// #pragma profile on

/*
 *
 *	  Virtual Screen Kernel Interface
 *			  (vsinterf.c)
 *
 *	  by Gaige B. Paulsen
 *
 *	This file contains the control and interface calls for the NCSA
 *  Virtual Screen Kernal.
 *
 *	  VSinit(maxwidth)					- Initialize the VSK
 *	  VSnewscreen(maxlines,scrnsave)	- Initialize a new screen.
 *	  VSdetach(w)						- Detach screen w
 *	  VSredraw(w,x1,y1,x2,y2)			- redraw region for window w
 *	  VSwrite(w,ptr,len)				- write text @ptr, length len
 *	  VSclear(w)						- clear w's real screen
 *	  VSkbsend(w,k,echo)				- send keycode k's rep. out window w (w/echo if req.)
 *	  VSclearall(w)						- clear w's real and saved screen
 *	  VSreset(w)						- reset w's emulator (as per TERM)
 *	  VSgetline(w,y)					- get a ptr to w's line y
 *	  VSsetrgn(w,x1,y1,x2,y2)			- set local display region
 *	  VSscrolback(w,n)					- scrolls window w back n lines
 *	  VSscrolforward(w,n)				- scrolls window w forward n lines
 *	  VSscrolleft(w,n)			 		- scrolls window w left  n columns
 *	  VSscrolright(w,n)			 		- scrolls window w right n columns
 *	  VSscrolcontrol(w,scrlon,offtop)	- sets scroll vars for w
 *	  VSgetrgn(w,&x1,&y1,&x2,&y2)		- returns set region
 *	  VSsnapshot(w)				  		- takes a snapshot of w
 *	  VSgetlines(w)						- Returns current # of lines
 *	  VSsetlines(w, lines)				- Sets the current # of lines to lines
 *	
 *		Version Date	Notes
 *		------- ------  ---------------------------------------------------
 *		0.01	861102  Initial coding -GBP
 *		0.10	861113  Added some actual program to this file -GBP
 *		0.15	861114  Initiated Kludge Operation-GBP
 *		0.50	8611VSPBOTTOM  Parameters added to VSnewscreen -GBP
 *		0.90	870203	Added the kbsend routine -GBP
 *		2.1		871130	NCSA Telnet 2.1 -GBP
 *		2.2 	880715	NCSA Telnet 2.2 -GBP
 *
 */

#define VSMASTER


#include "rsinterf.proto.h"
#include "rsmac.proto.h"
#include "vsem.proto.h"
#include "vsdata.h"
#include "vskeys.h"
#include "vsinit.h"
#include "vsintern.proto.h"
#include "Wind.h"
#include "DlogUtils.proto.h"
#include "maclook.proto.h"
#include "errors.proto.h"
#include "Macros.proto.h"
#define DEBUGMAC

#include "vsinterf.proto.h"
/*
unsigned short VSIkpnums[] =	// RAB BetterTelnet 2.0b5 - macro numbers
	{ 24, 25, 26, 22, 27, 28,  0, 40,  0, 42,  0, 43,  0, 29,  0, 41,
	   0, 44, 58, 56, 54, 59, 23, 57, 21, 55, 20,  0,  0,  0,  0,  0,
	   0, 80, 81, 83, 82, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 71,
	  70, 72, 73, 50, 51, 52, 53 };
*/

// macro numbers
unsigned short VSIkpnums[] = {
  24, 25, 26, 22, 27, 28,  0, 40,  0, 42,  0,
  43,  0, 29,  0, 41,  0, 44, 58, 56, 54, 59,
  23, 57, 21, 55, 20, 27,  0,  0,  0,  0,  0,
  90, 91, 93, 92, 70, 71, 72, 73, 74, 75, 76,
  77, 78, 79, 81, 80, 82, 83, 50, 51, 52, 53
};

extern TelInfoRec *TelInfo;
extern WindRec	*screens;
extern long		TempItemsDirID;
extern short	TempItemsVRefNum;
extern short RSa;

extern void syslog( int priority, const char *format, ...);

/* Internal variables for use in managing windows */
short		VSmax = 0;			/* max nr screens allowed */
short		VSinuse = 0;		/* nr screens actually in existence */
VSscrndata	*VSscreens;

short VSinit
  (
	short max /* max nr screens to allow */
  )
  /* initializes virtual screen and window handling. */
{
	short i;

	RSinitall(max);
	VSmax = max;
	VSIwn = 0;
	if ((VSscreens = (VSscrndata *) myNewPtr(max * sizeof(VSscrndata))) == 0L)
		return -2;
	for (i = 0; i < max; i++) {
		VSscreens[i].loc = 0L;
		VSscreens[i].stat = 0;
	} /* for */
	return(0);
} /* VSinit */


short VSiscapturing(short w) {								/* BYU 2.4.18 */
	return(VSscreens[w].captureRN);					/* BYU 2.4.18 */
}														/* BYU 2.4.18 */

Boolean VSopencapture(short scrn_num, short w)
{
	UNUSED_ARG(scrn_num)
	static short	captNumber = 1;
	short			VRefNum;
	long			DirID;
	Str255			filename, tempString;
	Str32			numstring;
	Point			where = {100,100};
	SFReply			sfr;
	long			junk = 0;
	OSErr			err;
	
	NumToString(captNumber++, numstring);
	GetIndString(filename, MISC_STRINGS, CAPTFILENAME);
	filename[++(filename[0])] = ' ';
	pstrcat(filename, numstring);
	
	GetIndString(tempString,MISC_STRINGS,SAVE_CAPTURED_TEXT_STRING);
	SFPutFile(where,tempString, filename, NULL, &sfr);
	if (sfr.good) {
		(void) GetWDInfo(sfr.vRefNum, &VRefNum, &DirID, &junk);

		err = HCreate(VRefNum, DirID, sfr.fName, 
					gApplicationPrefs->CaptureFileCreator, 'TEXT');
		if (err == dupFNErr) {
			HDelete(VRefNum, DirID, sfr.fName);
			err = HCreate(VRefNum, DirID, sfr.fName,
							gApplicationPrefs->CaptureFileCreator, 'TEXT');
			}
			
		if (err != noErr)
			OperationFailedAlert(CANT_CREATE_FILE, 500, err);
		else {		
			err = HOpenDF(VRefNum, DirID, sfr.fName, fsRdWrPerm,
				&VSscreens[w].captureRN);
			if (err != noErr) OperationFailedAlert(CANT_OPEN_FILE, 501, err);
			else {
				SetEOF(VSscreens[w].captureRN, (long) 0);
				return(TRUE);
				}
			}
		}
		
	return(FALSE);
}

void VSclosecapture(short w) {									/* BYU 2.4.18 */
	FSClose(VSscreens[w].captureRN);					/* BYU 2.4.18 */
	VSscreens[w].captureRN = 0;							/* BYU 2.4.18 */
}														/* BYU 2.4.18 */

void VScapture(unsigned char *ptr, short len) {		/* BYU 2.4.18 */
	long ln = len;								/* BYU 2.4.18 */
	if (VSscreens[VSIwn].captureRN) {			/* BYU 2.4.18 */
		unsigned char captbuf[512];				/* BYU 2.4.18 */
		unsigned char *ptr2,*ptr3;				/* BYU 2.4.18 */
		ptr2 = ptr;								/* BYU 2.4.18 */
		ptr3 = &captbuf[0];						/* BYU 2.4.18 */
#ifdef DEBUG_CAPTURE // RAB BetterTelnet 2.0b1 - debug capture captures ALL control codes
		if (TelInfo->debug) {
		for (len = 0; len < ln; len++)
			*(ptr3++) = *(ptr2++);
		} else {
#endif
		for (len = 0; len < ln; len++) {		/* BYU 2.4.18 */
			if (*ptr2 >= 32 || 					/* BYU 2.4.18 */
				*ptr2 == 13 ||					/* BYU 2.4.18 */
				*ptr2 ==  9)					/* BYU 2.4.18 */
				*(ptr3++) = *(ptr2++);			/* BYU 2.4.18 */
			else {								/* BYU 2.4.18 */
				ptr2++;							/* BYU 2.4.18 */
				ln--;							/* BYU 2.4.18 */
			}									/* BYU 2.4.18 */
		}										/* BYU 2.4.18 */
#ifdef DEBUG_CAPTURE
		}
#endif
		if (ln > 0) {													/* BYU 2.4.18 */
			if (FSWrite(VSscreens[VSIwn].captureRN, &ln, captbuf)) {	/* BYU 2.4.18 */
				FSClose(VSscreens[VSIwn].captureRN);					/* BYU 2.4.18 */
				VSscreens[VSIwn].captureRN = 0;							/* BYU 2.4.18 */
			}											/* BYU 2.4.18 */
		}											/* BYU 2.4.18 */
	}												/* BYU 2.4.18 */
}													/* BYU 2.4.18 */

short	VSisprinting(short w)
{
	if ((VSscreens[w].loc)->qprint) // RAB BetterTelnet 1.0fc4
		return ((VSscreens[w].loc)->printqueued); // RAB BetterTelnet 1.0fc4
	return ((VSscreens[w].loc)->prredirect);
}

Boolean	VSisvt7bit(short w)
{
	return !(VSscreens[w].loc)->vteightbits;
}

void	ClosePrintingFile(short w)
{
	OSErr sts;
	char tmp[80];

	putln("Attempting to remove print file");
	
	if ((sts=FSClose ((VSscreens[w].loc)->refNum)) != noErr) {
		SysBeep(1);
		sprintf(tmp,"FSClose: ERROR %d",sts); putln(tmp);
		}
	if ((sts=HDelete(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname)) != noErr) {
		SysBeep(1);
		sprintf(tmp,"HDelete: ERROR %d",sts); putln(tmp);
		}
}

short VSvalids
  (
	short w
  )
  /* validates a virtual screen number and sets it as the
	current screen for subsequent operations if success.
	Returns 0 iff success. */
  {
	if (VSinuse == 0)
		return(-5); /* -5=no ports in use */
	if (VSIwn == w)
		return(0);	/* Currently set to that window */
	if ((w > VSmax) || (w < 0))
		return(-6); /* blown out the top of the stuff */
	VSIwn = w;
	if (VSscreens[w].stat != 1)
		return(-3);/* not currently active */
	VSIw = VSscreens[w].loc;
	if (VSIw == 0L)
		return(-3); /* no space allocated */
	return(0);
  } /* VSvalids */

VSscrn *VSwhereis(short i) /* screen number */
  /* returns a pointer to the structure for the specified screen. */
  {
	VSvalids(i);
	return(VSIw);
  } /* VSwhereis */

void VSIclrbuf
  (
	void
  )
  /* clears out the text and attribute buffers for the current screen.
	All text characters are set to blanks, and all attribute bytes
	are set to zero. Doesn't update the display. */
  {
	register short j, i;
	register char *tx;
	register VSAttrib *ta;
	for (i = 0; i <= VSIw->lines; i++)
	  {
	  	VSIw->linest[i]->lattr = 0;
	  	if (VSIw->oldScrollback) {
	  		ta = &VSIw->attrst[i]->text[0];
	  	} else {
			ta = &VSIw->linest[i]->attr[0];
		}
		tx = &VSIw->linest[i]->text[0];
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*ta++ = 0;
			*tx++ = ' ';
		  } /* for */
	  } /* for */
  } /* VSIclrbuf */

short VSnewscreen
  (
	short vtemulation,
	short maxlines, /* max lines to save in scrollback buffer */
	short screensave, /* whether to have a scrollback buffer */
	short numLines, //numLines initially on screen  (CCP 2.7)
	short maxwid, /* number of columns on screen */
	unsigned long flags
  )
  /* creates a new virtual screen, and returns its number. */
  {
	if (maxlines < VSDEFLINES)
		maxlines = VSDEFLINES;

	if ( VSinuse >= VSmax ) {
		// too many screens in existence
		return -1;
	}
	VSIwn = 0;
	while ( VSIwn < VSmax && VSscreens[VSIwn].stat == 1 )
		VSIwn++;
	if ( VSIwn >= VSmax ) {
		// shouldn't occur?
		return -1;
	}
	numLines -= 1;  //correct for internal use
	
	/*
	*  Fill initial scrollback buffer and screen storage space.
	*
	*  Memory allocation rules:
	*  line->mem == 0 if not a memory allocation, line->mem == 1 if it is the first
	*     VSline in a block (indeterminate size, may be size == 1)
	*  
	*  attributes array is ALWAYS allocated as one block.  Internally represented and
	*  manipulated as a linked list of lines, but only one of the lines will have 
	*  line->mem == 1.  This list is always supposed to be circular (it is never
	*  extended, as attributes are never scrolled back).
	*
	*  scrollback and screen line buffer space is allocated in large blocks.  Each
	*  block will have line->mem == 1 if the pointer to that VSline is "free"able.
	*  This list will either be circular (which means it has reached its full size),
	*  or it will have a NULL next field at the end.  During scrolling, the end may
	*  be augmented until VSIw->numlines > VSIw->maxlines or we run out of memory.
	*  Typically allocate memory 100 lines at a time in two blocks, one is the VSline
	*  list, the other is the mem for the character storage.
	*
	*/

	/* All memory allocation for this function is done at once, to help damage control in 
	low memory situations */

	if ((VSscreens[VSIwn].loc = VSIw = (VSscrn *) myNewPtr(sizeof(VSscrn))) == 0L) {
		return(-2);
	}

	VSIw->vtemulation = vtemulation;
	VSIw->vteightbits = !(flags & RSWvt7bit);

	VSIw->oldScrollback = flags & RSWoldscrollback;
	VSIw->lines = numLines;
	//VSIw->lines = 23; CCP 2.7 set this from the start


	VSIw->linest = VSInewlinearray(VSIw->lines + 1);
	if (VSIw->linest == NULL)
 	{
		DisposePtr((Ptr)VSIw);
		VSscreens[VSIwn].loc = VSIw = NULL;
		return (-2);
	}	

	if (VSIw->oldScrollback) {
		VSIw->attrst = (VSattrlinePtr *) VSInewlinearray(VSIw->lines + 1);
		if (VSIw->attrst == NULL)
	 	{
			DisposePtr((Ptr)VSIw->linest);
			DisposePtr((Ptr)VSIw);
			VSscreens[VSIwn].loc = VSIw = NULL;
			return (-2);
		}
	}

/* NONO */
/*	VSIw->tabs = (char *) myNewPtr(132);*/		/* NCSA: SB - allow 132 column mode */
	VSIw->tabs = (char *) myNewPtr(255);	/* up to 255 comumns */
/* NONO */
	if (VSIw->tabs == NULL)  /* CCP: Hey?  Why not check if we got it?! */
	{
		DisposePtr((Ptr)VSIw->linest);
		DisposePtr((Ptr)VSIw);
		VSscreens[VSIwn].loc = VSIw = NULL;
		return (-2);
	}
/* NONO */
/*	VSIw->allwidth = 131;*/				/* NCSA: SB - always allocate max lines */
	VSIw->allwidth = 255;				/* always allocate max lines */
/* NONO */
	if (!VSIw->oldScrollback) {
		if (screensave)
			VSIw->buftop = VSInewlines(VSIw->lines + 1 + VSDEFLINES); /* screen lines plus some initial preallocated scrollback space */
		else
			VSIw->buftop = VSInewlines(VSIw->lines + 1); /* screen lines, no scrollback */
		if (VSIw->buftop == NULL)
		{
			DisposePtr((Ptr)VSIw->tabs);
			DisposePtr((Ptr)VSIw->linest);
			DisposePtr((Ptr)VSIw);
			VSscreens[VSIwn].loc = VSIw = NULL;
			return(-2);
		}
		VSIw->linest[0] = VSIw->buftop;
	} else {
		if (screensave)
			VSIw->buftop = VSOnewlines(VSIw->lines + 1 + VSDEFLINES,1); /* screen lines plus some initial preallocated scrollback space */
		else
			VSIw->buftop = VSOnewlines(VSIw->lines + 1,1); /* screen lines, no scrollback */
		if (VSIw->buftop == NULL)
		{
			DisposePtr((Ptr)VSIw->tabs);
			DisposePtr((Ptr)VSIw->attrst);
			DisposePtr((Ptr)VSIw->linest);
			DisposePtr((Ptr)VSIw);
			VSscreens[VSIwn].loc = VSIw = NULL;
			return(-2);
		}
		VSIw->linest[0] = VSIw->buftop;
		VSIw->attrst[0] = (VSattrlinePtr)VSOnewlines(VSIw->lines + 1,sizeof(VSAttrib));		/* new space for attributes (these are never scrolled back) */
		if (VSIw->attrst[0] == NULL)
		{
			VSIfreelinelist(VSIw->buftop);
			DisposePtr((Ptr)VSIw->tabs);
			DisposePtr((Ptr)VSIw->attrst);
			DisposePtr((Ptr)VSIw->linest);
			DisposePtr((Ptr)VSIw);
			VSscreens[VSIwn].loc = VSIw = NULL;
			return(-2);
		}
	}

	VSIw->vistop = VSIw->scrntop = VSIw->buftop;	/* initial view = screen */

	if (VSIw->oldScrollback) {
		VSOlistndx(VSIw->scrntop, VSIw->attrst[0]);
		VSIw->attrst[0]->prev = VSIw->attrst[VSIw->lines]; /* make attribute list circular, since it is never extended */
		VSIw->attrst[VSIw->lines]->next = VSIw->attrst[0];
	} else
		VSIlistndx(VSIw->scrntop);	/* Set up screen arrays */

	if (!screensave)
	{   /* make text line list circular to indicate no extensions */
		VSIw->linest[0]->prev = VSIw->linest[VSIw->lines];
		VSIw->linest[VSIw->lines]->next = VSIw->linest[0];
	} /* if */

	VSIw->disableCursor = 0; // cursor may be visible (RAB BetterTelnet 2.0b4)
	VSIw->linesjumped = 0; // reset the jump scrolling (RAB BetterTelnet 2.0b3)
	VSIw->jumpScroll = flags & RSWjumpscroll;
	VSIw->maxlines = maxlines;
	VSIw->numlines = 0;
	VSscreens[VSIwn].captureRN = 0;		/* BYU 2.4.18 - capture file's RefNum */
	VSIw->id = 'VSCR';
	VSIw->maxwidth = maxwid - 1;
	VSIw->savelines = screensave;
	VSIw->forcesave = flags & RSWforcesave;		/* NCSA 2.5 */
	VSIw->attrib = 0;
	VSIw->Pattrib = 0xffffffff; /* initially no saved attribute */
	VSIw->x = 0;
	VSIw->y = 0;
	VSIw->charset = 0;
	VSIw->trincount = 0;
	VSIw->trinx = 0;
	VSIw->trintag = 0;
	VSIw->G0 = 0;
	VSIw->G1 = 1;
	VSIw->DECAWM = 0;
	VSIw->DECCKM = 0;
	VSIw->DECPAM = 0;
	VSIw->DECORG = 0;
	VSIw->DECCM = 1; /* Bri 970610 */
	VSIw->IRM = 0;
	VSIw->escflg = 0;
	VSIw->top = 0;
	VSIw->bottom = numLines;
	VSIw->parmptr = 0;
	VSIw->Rtop = 0;
	VSIw->Rleft = 0;
	VSIw->Rright = maxwid - 1;
	VSIw->Rbottom = numLines;
	VSIw->ESscroll = 1;
	VSIw->prredirect = 0;						/* LU */
	VSIw->qprint = 0;
	VSIw->ignoreBeeps = flags & RSWignoreBeeps;
	VSIw->prbuf = 0;							/* LU */
	VSIw->refNum = -1;							/* LU */
	VSIw->possibleForce = 0;
	VSIw->realBlink = flags & RSWrealBlink;
	VSIclrbuf();
	VSItabinit();
	VSscreens[VSIwn].stat = 1;
	VSinuse++;

	return(VSIwn);
} /* VSnewscreen */

short VSdestroy(short w) /* screen number */
  /* gets rid of a virtual screen. */
  {
	if (VSvalids(w) != 0)
		return(-3);
	VSIfreelines();
	if (VSIw->oldScrollback) {
		VSIfreelinelist((VSlinePtr)VSIw->attrst[0]);
		if (VSIw->attrst)
			DisposePtr((Ptr) VSIw->attrst);
	}
	if(VSIw->linest)
		DisposePtr((Ptr) VSIw->linest);
	if(VSIw->tabs)
		DisposePtr(VSIw->tabs);
	if(VSIw)
		DisposePtr((Ptr) VSIw);
	VSscreens[w].stat = 0;
	VSIwn = -1;
	VSinuse--;			/* SCA '87 */
	return(0);
  } /* VSdestroy */

// RAB BetterTelnet 2.0b4
// reallocates a virtual screen, keeping screen contents but nuking the scrollback buffer
void VSrealloc(short w)
{
	short i;
	VSlinePtr savedTextBlock, savedTextPtr;

	if (VSvalids(w) != 0)
		return;

	// save the current screen
	savedTextBlock = VSInewlines(VSIw->lines + 1 + VSDEFLINES);
	if (!savedTextBlock) {
		// FIXME: we should display the error here...
		return;
	}

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

	// nuke the current line structures
	VSIfreelines();
	// note: this always frees the text, but only frees the attributes if we're using
	// the "new" scrollback

	if (!VSIw->oldScrollback) {
		VSIw->buftop = savedTextBlock; // hey, we have a block with the right stuff anyway...
		VSIw->linest[0] = VSIw->buftop;
		VSIlistndx(VSIw->buftop);
	} else {
		VSIw->buftop = VSOnewlines(VSIw->lines + 1 + VSDEFLINES,1); /* screen lines plus some initial preallocated scrollback space */
/*		if (VSIw->buftop == NULL)
		{
			DisposePtr((Ptr)VSIw->tabs);
			DisposePtr((Ptr)VSIw->attrst);
			DisposePtr((Ptr)VSIw->linest);
			DisposePtr((Ptr)VSIw);
			VSscreens[VSIwn].loc = VSIw = NULL;
			return(-2);
		} */ // we really should check for errors here, but what do we do if we find one?
		VSIw->linest[0] = VSIw->buftop;
		VSOlistndx(VSIw->buftop, VSIw->attrst[0]);
		VSIw->attrst[0]->prev = VSIw->attrst[VSIw->lines]; /* make attribute list circular, since it is never extended */
		VSIw->attrst[VSIw->lines]->next = VSIw->attrst[0];
// we don't allocate attrst here because we already have it
// we do, however, need to copy in the screen text
		savedTextPtr = savedTextBlock;
		for (i = 0; i <= VSIw->lines; i++) {
			BlockMoveData(savedTextPtr->text, VSIw->linest[i]->text, VSIw->allwidth + 1);
			if (savedTextPtr->next) savedTextPtr = savedTextPtr->next;
		}
		DisposePtr((Ptr) savedTextBlock); // VSIfreelinelist adds un-needed overhead here
	}

	VSIw->vistop = VSIw->scrntop = VSIw->buftop;
	VSIw->Rbottom -= VSIw->Rtop;
	VSIw->Rtop = 0;
	VSIw->numlines = 0;
}

void VSredrawLine(short w) //redraws current line
{
	if (VSvalids(w) != 0)
		return;
	VSredraw(w, 0, VSIw->y,VSIw->maxwidth, VSIw->y); 
	if (VSIcursorvisible())
		VSIcuroff(w);
}

short VSredraw
  (
	short w, 		// window to redraw */
	short x1, 		
	short y1,
	short x2,
	short y2
  )
  /* redisplays the specified portion of a virtual screen. */
  {
	VSlinePtr ypt;
	short y;
	short tx1, tx2, ty1, ty2, tn, offset;
	short sx1;
	short sx2;
	Boolean cursOff;

	if (VSvalids(w) != 0)
		return(-3);

	if (VSIw->oldScrollback)
		return VSOredraw(w, x1, y1, x2, y2);

	x1 += VSIw->Rleft;	// Make local coords global again
	x2 += VSIw->Rleft;
	y1 += VSIw->Rtop;
	y2 += VSIw->Rtop;

	if (x1 < 0) x1 = 0;
	else if (x1 > VSIw->maxwidth) x1 = VSIw->maxwidth;
	if (x2 < 0) x2 = 0;
	else if (x2 > VSIw->maxwidth) x2 = VSIw->maxwidth;
	if (y1 < -VSIw->numlines) y1 = -VSIw->numlines;
	else if (y1 > VSIw->lines) y1 = VSIw->lines;
	if (y2 < -VSIw->numlines) y2 = -VSIw->numlines;	/* limit to amount of scrollback */
	else if (y2 > VSIw->lines) y2 = VSIw->lines;

	tx1 = x1;		// Set up to clip redraw area to visible area
	tx2 = x2;
	ty1 = y1;
	ty2 = y2;
	tn = -1;		// so we include more than 1 line

	if (!VSIclip(&tx1, &ty1, &tx2, &ty2, &tn, &offset)) {

		cursOff = 0;
		if ( VSIcursorenabled() && RScursison(w) ) {
			cursOff = 1;
			VSIcuroff(w); 						// temporarily hide cursor
		}

		//RSerase(w, tx1, ty1, tx2, ty2);		// Erase the offending area

		sx1 = tx1;
		sx2 = tx2;

		ypt = VSIGetLineStart(w, y1);

		for ( y = ty1; y <= ty2; y++ ) {
			char *pt;
			VSAttrib *pa;
			VSAttrib lasta;
			short x, lastx;
			short chw;

			if ( VSIw->Rtop + y == 0 ) {
				// on-screen buffer
				ypt = *VSIw->linest;
			}

			pt = ypt->text + VSIw->Rleft;
			pa = ypt->attr + VSIw->Rleft;

			// if double size, we must shift width
			if (VSisdecdwh(ypt->lattr)) {
				tx1 >>= 1;
				tx2 >>= 1;
				chw = 2;
			} else
				chw = 1;

			// multi-byte
			if ( tx1 > 0 && (pa[tx1-1] & kVSansi2b) )
				--tx1;
			if ( tx2 < VSIw->maxwidth && (pa[tx2] & kVSansi2b) )
				++tx2;

			lastx = tx1;
			lasta = pa[tx1] & ~kVSansi2b;
			for (x = tx1+1; x <= tx2; x++) {
				if ( (pa[x] & ~kVSansi2b) != lasta ) {
					RSdraw(w, lastx, y, ypt->lattr, lasta, x-lastx, pt + lastx);
					lastx = x;
					lasta = pa[x] & ~kVSansi2b;
				}
			}
			if (lastx <= tx2)
				RSdraw(w, lastx, y, ypt->lattr, lasta, tx2-lastx+1, pt + lastx);

			tx1 = sx1;
			tx2 = sx2;
			ypt = ypt->next;
		}

		if ( cursOff ) {
			VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
		} else {
			// just calcutate the new coordinates
			VScursset(w, VSIw->x, VSIw->y);
		}

	}
	return 0;
} /* VSredraw */


short VSOredraw
  (
	short w, 		// window to redraw */
	short x1, 		
	short y1,
	short x2,
	short y2
  )
  /* redisplays the specified portion of a virtual screen. */
  {
	VSlinePtr ypt;
	VSattrlinePtr ypa;
	short y;
	short tx1, tx2, ty1, ty2, tn, offset;
	short sx1;
	short sx2;
	Boolean cursOff;

	// this fails for multi-bytes character set...

	if (VSvalids(w) != 0)
		return(-3);

	x1 += VSIw->Rleft;	// Make local coords global again
	x2 += VSIw->Rleft;
	y1 += VSIw->Rtop;
	y2 += VSIw->Rtop;

	if (x1 < 0) x1 = 0;
	else if (x1 > VSIw->maxwidth) x1 = VSIw->maxwidth;
	if (x2 < 0) x2 = 0;
	else if (x2 > VSIw->maxwidth) x2 = VSIw->maxwidth;
	if (y1 < -VSIw->numlines) y1 = -VSIw->numlines;
	else if (y1 > VSIw->lines) y1 = VSIw->lines;
	if (y2 < -VSIw->numlines) y2 = -VSIw->numlines;	/* limit to amount of scrollback */
	else if (y2 > VSIw->lines) y2 = VSIw->lines;

	tx1 = x1;		// Set up to clip redraw area to visible area
	tx2 = x2;
	ty1 = y1;
	ty2 = y2;
	tn = -1;		// so we include more than 1 line

	cursOff = 0;
	if ( VSIcursorenabled() && RScursison(w) ) {
		cursOff = 1;
		VSIcuroff(w); 						// temporarily hide cursor
	}

	// draw visible part of scrollback buffer
	if (y1 < 0) {
	
		tx1 = x1;		// Set up to clip redraw area to visible area of scrollback buffer
		tx2 = x2;
		ty1 = y1;
		ty2 = (y2>=0) ? -1 : y2;
		tn = -1;
		
		if (!VSIclip(&tx1, &ty1, &tx2, &ty2, &tn, &offset)) {
			sx1 = tx1;
			sx2 = tx2;

			ypt = VSIw->vistop;
			for(y=VSIw->Rtop; y<y1; y++)
				ypt = ypt->next;		// Get pointer to top line we need
	
			for (y=ty1; y<=ty2; y++) {

				// if double size, we must shift width
				if (VSisdecdwh(ypt->lattr)) {
					tx1 = (tx1 & 1) ? (tx1 >> 1) - 1 : (tx1 >> 1);
					tx2 = (tx2 & 1) ? (tx2 >> 1) + 1 : (tx2 >> 1);
				}

				/* no attributes... */
				RSdraw(w, tx1, y, ypt->lattr, 0, tn, ypt->text + VSIw->Rleft +tx1);

				// if double size, we must shift width
				if (VSisdecdwh(ypt->lattr)) {
					tx1 = sx1;
					tx2 = sx2;
				}

				ypt = ypt->next;
			}
		}
		y1 = 0;			// continue with on-screen buffer, if any
	}

	// draw visible part of on-screen buffer, taking account of attributes
	if (y2 >= 0) {
	
		tx1 = x1;		// Set up to clip redraw area to visible area of on-screen buffer
		tx2 = x2;
		ty1 = y1;
		ty2 = y2;
		tn = -1;

		if (!VSIclip(&tx1, &ty1, &tx2, &ty2, &tn, &offset)) {
		
			sx1 = tx1;
			sx2 = tx2;

			ypt = VSIw->linest[VSIw->Rtop+ty1];
			ypa = VSIw->attrst[VSIw->Rtop+ty1];
			
			for (y=ty1; y<=ty2; y++) {
				char *pt;
				VSAttrib *pa;
				VSAttrib lasta;
				short x, lastx;
			
				pt = ypt->text + VSIw->Rleft;
				pa = ypa->text + VSIw->Rleft;
				
				// if double size, we must shift width
				if (VSisdecdwh(ypt->lattr)) {
					tx1 = (tx1 & 1) ? (tx1 >> 1) - 1 : (tx1 >> 1);
					tx2 = (tx2 & 1) ? (tx2 >> 1) + 1 : (tx2 >> 1);
				}

				lastx = tx1;
				lasta = pa[tx1] & ~kVSansi2b;
				for(x=tx1+1; x<=tx2; x++) {
					if ( (pa[x] & ~kVSansi2b) != lasta ) {
						if (lastx == tx1 && tx1 > 0 && (pa[tx1-1] & kVSansi2b))
							RSdraw(w, lastx-1, y, ypt->lattr, lasta, x-lastx+1, pt + lastx-1);
						else
							RSdraw(w, lastx, y, ypt->lattr, lasta, x-lastx, pt + lastx);
						lastx = x;
						lasta = pa[x] & ~kVSansi2b;
					}
				}
				if (lastx<=tx2) {
					if (lastx == tx1 && tx1 > 0 && (pa[tx1-1] & kVSansi2b))
						RSdraw(w, lastx-1, y, ypt->lattr, lasta, tx2-lastx+2, pt + lastx-1);
					else
						RSdraw(w, lastx, y, ypt->lattr, lasta, tx2-lastx+1, pt + lastx);
				}
				
				// if double size, we must shift width
				if (VSisdecdwh(ypt->lattr)) {
					tx1 = sx1;
					tx2 = sx2;
				}

				ypt = ypt->next;
				ypa = ypa->next;
			}
		}
	}

	if ( cursOff ) {
		VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
	} else {
		// just calcutate the new coordinates
		VScursset(w, VSIw->x, VSIw->y);
	}
	return(0);
  } /* VSOredraw */


short VSwrite
  (
	short w, /* screen to draw into */
	char *ptr, /* pointer to text string */
	short len  /* length of text string */
  )
  /* sends a stream of characters to the specified window. */
{
	int cursOff;

//  	_profile = 1;
	if (len == 0)
		return 0;
	if (VSvalids(w) != 0)
		return(-3);
	cursOff = 0;
	if ( VSIcursorenabled() && RScursison(w) ) {
		cursOff = 1;
		VSIcuroff(w); // hide cursor momentarily
	}
	VSIcursdisable(); // RAB BetterTelnet 2.0b4
	VSem((unsigned char *) ptr, len);	/* BYU LSC - interpret the character stream */
	VSIflush(); // RAB BetterTelnet 2.0b3
	VSIcursenable();
	if ( cursOff ) {
		VSIcurson(w, VSIw->x, VSIw->y, 1); /* restore cursor, force it to be visible. */
	} else {
		// just calcutate the new coordinates
		VScursset(w, VSIw->x, VSIw->y);
	}
//	_profile = 0;
	return(0);
} /* VSwrite */


// RAB BetterTelnet 2.0b3
// The same VSwrite you know and love, except it doesn't flush.
// This is for parse() to use so it doesn't flush every time it hits an escape.

short VSwritefast
  (
	short w, /* screen to draw into */
	char *ptr, /* pointer to text string */
	short len  /* length of text string */
  )
  /* sends a stream of characters to the specified window. */
  {
//  	_profile = 1;
	if ( len == 0 )
		return 0;
	if ( VSvalids(w) != 0 )
		return(-3);
	if ( VSIcursorenabled() ) {
		VSIcuroff(w); /* hide cursor momentarily */
	}
	VSIcursdisable();
	VSem((unsigned char *) ptr, len);	/* BYU LSC - interpret the character stream */

// We don't show the cursor again here because it interferes with the screen refresh.
// Anything that calls VSfastwrite() is supposed to call VSflushwrite() anyway once it's done.

//	_profile = 0;
	return(0);
  } /* VSwrite */

// RAB BetterTelnet 2.0b3 - interface to VSIflush() which shows the cursor once
// the screen has been flushed

void VSflushwrite(short w) {

	if (VSvalids(w) != 0) return;

	VSIflush();
	VSIcursenable();
	if ( VSIcursorenabled() ) {
		VSIcurson(w, VSIw->x, VSIw->y, 1);
	}
}

// Utility routine: BetterTelnet 1.0fc3 (RAB)

void VSsetprintmode(short w, short printMode) {

	if (VSvalids(w) != 0)
		return;
	VSIw->qprint = printMode;
	if (!printMode) {
		VSIw->prredirect = VSIw->printqueued;
		VSprOFF();
	}
	VSIw->printqueued = 0;
}

short VSIgetNextTabDistance(void)
{
	short current;
	
	if (VSIw->x >= VSIw->maxwidth)
		return(0);
	current = VSIw->x + 1;
	while ((VSIw->tabs[current] != 'x')&&(current < VSIw->maxwidth))
		current++;
	return (current - VSIw->x);
}


															// MAT--We need to define how big the Queue is
															// MAT--for sending the cursor characters.
															// MAT--Since MacTCP doesn't like 30+ individual
#define MATSbufSize	254										// MAT--SendChar requests.
void VSpossendEM											// MAT--we can change this to support EMACS
  (															// MAT--style movement commands.
	short w, /* affected screen */												// MAT--
	short x, /* column to move to */												// MAT--
	short y, /* line to move to */												// MAT--
	short echo /* local echo flag */												// MAT--
  )																				// MAT--
  /* sends a stream of VT100 cursor-movement sequences to move the				// MAT--
	cursor on the specified screen to the specified position. */				// MAT--
{																				// MAT--
	UNUSED_ARG(echo)
	static char																	// MAT--
			tt[MATSbufSize] = "";												// MAT--
		char curschar;															// MAT--
		short localCount;															// MAT--
																				// MAT--
																				// MAT--
	if (x < 0 || y < 0 || x > VSIw->maxwidth || y > VSIw->lines)				// MAT--
		return;																	// MAT--
  /* convert x and y to relative distance to move */							// MAT--
	x -= VSIw->x;																// MAT--
	y -= VSIw->y;																// MAT--
																				// MAT--
	curschar = '\002'; /* EMACS cursor left */									// MAT--
	localCount=0;
	while (x < 0 && localCount < MATSbufSize)									// MAT--
	  {																			// MAT--
		tt[localCount] = curschar;												// MAT--
		x++; localCount++;														// MAT--
	  } /* while */																// MAT--
	if(localCount)	RSsendstring(w, tt, localCount);							// MAT--

	curschar = '\016'; /* EMACS cursor down */									// MAT--
	localCount=0;
	while (y > 0 && localCount < MATSbufSize)									// MAT-- why we check to see if localCount is < MATSbufSize
	  {																			// MAT-- I dont know. But If they had a window > 254 columns
		tt[localCount] = curschar;												// MAT-- maybe it's a good idea.
		y--; localCount++;														// MAT-- but it never hurts to be safe.
	  } /* while */																// MAT--
	if(localCount)	RSsendstring(w, tt, localCount);							// MAT--

	curschar = '\020'; /* EMACS cursor up */									// MAT--
	localCount=0;
	while (y < 0 && localCount < MATSbufSize)									// MAT--
	  {																			// MAT--
		tt[localCount] = curschar;												// MAT--
		y++; localCount++;														// MAT--
	  } /* while */																// MAT--
	if(localCount)	RSsendstring(w, tt, localCount);							// MAT--

	curschar = '\006'; /* EMACS cursor right */									// MAT--
	localCount=0;
	while (x > 0 && localCount < MATSbufSize)									// MAT--
	  {																			// MAT--
		tt[localCount] = curschar;												// MAT--
		x--; localCount++;														// MAT--
	  } /* while */																// MAT--
	if(localCount)	RSsendstring(w, tt, localCount);							// MAT--
																				// MAT--
	if (0)	//(note: supposed to look for "echo" here)							// MAT--
	  {																			// MAT--
		if (VSIcursorenabled())
			VSIcuroff(w);															// MAT--
		VSIw->x = x;															// MAT--
		VSIw->y = y;															// MAT--
		if (VSIcursorenabled())
			VSIcurson(w, VSIw->x, VSIw->y, 1); /* Force Move */						// MAT--
	  } /* if */																// MAT--
  } /* VSpossendEM */					// changed comment						// MAT--
 	
void VSpossend
  (
	short w, /* affected screen */
	short x, /* column to move to */
	short y, /* line to move to */
	short echo /* local echo flag */
  )
  /* sends a stream of VT100 cursor-movement sequences to move the
	cursor on the specified screen to the specified position. */
{
	short sn;
	static char
		VSkbax[] = "\033O ",		/* prefix for auxiliary code */
		VSkban[] = "\033[ ";		/* prefix for arrows normal */
	char *vskptr;

	if (VSvalids(w) != 0)
		return;
	
	
/* NCSA: SB - This would bomb before.  You need to get the screens # from the 
				translation routine before you access the record! */  
	sn = findbyVS(w);	
	if (sn >= 0 && screens[sn].arrowmap) {    /* NCSA: SB - get the CORRECT screens # */
		VSpossendEM(w,x,y,echo);            // MAT--  call our cursor movement routine
		return;                             // MAT--  then exit
	}

	if (VSIw->DECPAM && VSIw->DECCKM)
		vskptr = VSkbax;
	else
		vskptr = VSkban;
	if (x < 0 || y < 0 || x > VSIw->maxwidth || y > VSIw->lines)
		return;
  /* convert x and y to relative distance to move */
	x -= VSIw->x;
	y -= VSIw->y;

	vskptr[2] = 'D'; /* cursor left */
	while (x < 0)
	  {
		x++;
		RSsendstring(w, vskptr, 3);
	  } /* while */

	vskptr[2] = 'B'; /* cursor down */
	while (y > 0)
	  {
		y--;
		RSsendstring(w, vskptr, 3);
	  } /* while */
	vskptr[2] = 'A'; /* cursor up */
	while (y < 0)
	  {
		y++;
		RSsendstring(w, vskptr, 3);
	  } /* while */
	vskptr[2] = 'C'; /* cursor right */
	while (x > 0)
	  {
		x--;
		RSsendstring(w, vskptr, 3);
	  } /* while */

	if (echo)
	  {
		if (VSIcursorenabled())
			VSIcuroff(w);
		VSIw->x = x;
		VSIw->y = y;
		if (VSIcursorenabled())
			VSIcurson(w, VSIw->x, VSIw->y, 1); /* Force Move */
	  } /* if */
} /* VSpossend */


char VSkbsend
  (
	short w, /* active window */
	unsigned char k, /* special key code if > 128, else ascii code */
	short echo, /* local echo flag */
	short shifted // RAB BetterTelnet 2.0b5 - shift key down
  )
  /* sends the appropriate sequence for the specified key, taking due
	account of terminal mode settings. */
  {
// RAB BetterTelnet 2.0b5 - extensively rewritten
//	static char
//		VSkbkn[] = "\033O ",		/* prefix for keypad normal */
//		VSkbax[] = "\033O ",		/* prefix for auxiliary code*/
//		VSkban[] = "\033[ ",		/* prefix for arrows normal */
//		VSkbfn[] = "\033O ",		/* prefix for function keys */		/* BYU 2.4.12 */
//		VSk220[] = "\033[  ~";		/* prefix for vt220 keys */			/* BYU 2.4.12 */
	short sn;
	char *vskptr;
	short vskplen;
	short macronum;

	if (VSvalids(w) != 0)
		return(-3);

	sn = findbyVS(w);
	if (sn < 0)
		return;
	if ( screens[sn].arrowmap && (k <= VSLT) && (k >= VSUP) )	// MAT--
										// MAT-- important...we need to check this first before
		{								// MAT-- the next if(É) statement gets its hands on the string.
			switch (k) {				// MAT--  do the mapping from arrowkeys -> EMACS ctrl keys.
				case VSLT:				// MAT-- 
					k = 0x02;			// MAT-- ^B			Question: Is there a way to find out if the option
					break;				// MAT-- 					  key was held down with this character?
				case VSRT:				// MAT-- 			I didn't want to declare myEvent an extern
					k = 0x06;			// MAT-- ^F			(I didn't know if that was a no-no)
					break;				// MAT-- 			If I can.....let me know, I want to make
				case VSUP:				// MAT-- 			option-arrowkey's do useful things too
					k = 0x10;			// MAT-- ^P
					break;				// MAT-- 			checking the keymap would be a kludge here.
				case VSDN:				// MAT-- 
					k = 0x0e;			// MAT-- ^N
					break;				// MAT-- 
			}	/* switch k */			// MAT--
			RSsendstring(w,(char *)&k,1);		// MAT-- send the character
			return(0);					// MAT--
		}								// MAT--


	if ( k < VSF10 )	{						/* BYU 2.4.12 */
	  /* 7-bit ascii code -- send as is */
		RSsendstring(w,(char *) &k, 1);		/* BYU LSC */
		return 0; // RAB BetterTelnet 2.0b5 - rest is for special keys
	}

	/* Keypad (Not Application Mode): 0-9 , - . Enter */ // was (k < VSPF1)

	if ( k >= VSJPBKS || (k > VSLT && k < VSKE && !VSIw->DECPAM) ) {
		RSsendstring(w, &VSIkpxlate[shifted][k - VSUP], 1);
		if (echo)
			VSwrite(w, &VSIkpxlate[shifted][k - VSUP], 1);
//		if (k == VSKE)
//			RSsendstring(w, "\012", 1);
		return(0);
	} /* if */

/*	if (VSIw->DECPAM && VSIw->DECCKM) {
	  // aux kp mode
	  	vskptr = VSkbax;
		vskplen = 3;
		vskptr[2] = VSIkpxlate[1][k - VSUP];	// BYU 2.4.12
	  }
	else if (k < VSUP) {						// BYU 2.4.12
		vskptr = VSk220;						// BYU 2.4.12 
		vskplen = VSIkplen[k - VSF10];			// BYU 2.4.12 
		vskptr[2] = VSIkpxlate2[k - VSF10];		// BYU 2.4.12 
		vskptr[3] = VSIkpxlate3[k - VSF10];		// BYU 2.4.12 
	} else {									// BYU 2.4.12 
		vskplen = 3;							// BYU 2.4.12 
		if (k < VSK0) {							// BYU 2.4.13 - arrow keys 
			vskptr = VSkban;					// BYU 2.4.12 
			if (VSIw->DECCKM)					// BYU 2.4.13 
				vskptr[1] = 79;					// BYU 2.4.13 
			else								// BYU 2.4.13 
				vskptr[1] = 91;					// BYU 2.4.13 
		}										// BYU 2.4.13 
		else if (k < VSPF1) 					// BYU 2.4.12 
			vskptr = VSkbkn;					// BYU 2.4.12 
		else 									// BYU 2.4.12 
			vskptr = VSkbfn;					// BYU 2.4.12 
												// BYU 2.4.12 
		vskptr[2] = VSIkpxlate[1][k - VSUP];	// BYU 2.4.12 
	}											// BYU 2.4.12

	RSsendstring(w, vskptr, vskplen);
	if (echo)
		VSwrite(w, vskptr, vskplen); */

	// now for the new - RAB BetterTelnet 2.0b5
	// first we get the number, then we mangle it depending on what other stuff is going on
	macronum = VSIkpnums[k - VSF10];

	if ( macronum >= 20 && macronum <= 29 ) {
		if ( shifted )
			macronum += 10; // Shift+F1-F10
		else if ( VSIw->vtemulation == 2 )
			macronum += 80;	// ANSI Function keys
		else if ( VSIw->vtemulation == 3 )
			macronum += 90;	// Linux Function keys
	} else if (macronum >= 40 && macronum <= 44 && shifted) // Shift+F11-F15
		macronum += 5;
	else if (macronum >= 50 && macronum <= 59 && shifted) // Shift+PF1, Home...
		macronum += 10;
	else if (macronum >= 90 && macronum <= 93 && VSIw->DECCKM) // CKM arrows
		macronum += 5;
	else if (k == VSKE && !VSIw->DECPAM) // Enter (!AM)
		macronum++;

	sendmacro(&screens[sn], macronum); // and actually send it, the NEW way!

	return 0;
  } /* VSkbsend */

short VSreset
  (
	short w
  )
  /* resets virtual terminal settings to default state, clears screen
	and homes cursor. */
  {
	if (VSvalids(w) != 0)
		return(-3);
	if (VSIcursorenabled())
		VSIcuroff(w);			/* NCSA: SB -- get rid of extraneous cursor BS */
	VSIreset(); /* causes cursor to disappear */
	if (VSIcursorenabled())
		VSIcurson(w, VSIw->x, VSIw->y, 1); /* redisplay cursor at home position */
	return(0);
  } /* VSreset */

void VSscrolright
  (
	short w,
	short n /* number of columns to scroll */
  )
  /* moves the view of the virtual screen within its window the
	specified number of columns to the right. */
  {
	short sn, lmmax;

	if (VSvalids(w) != 0)
		return;

  /* limit scroll amount against number of invisible columns */
	lmmax = VSIw->maxwidth - (VSIw->Rright - VSIw->Rleft);
	if (VSIw->Rleft + n > lmmax)
		n = lmmax - VSIw->Rleft; /* can't scroll any further right than this */
	if (n == 0)
		return;									/* Do nothing if appropriate */

	if (VSIcursorenabled())
		VSIcuroff(w); /* temporarily hide cursor */
	VSIw->Rleft += n; /* update visible region */
	VSIw->Rright += n;
	sn = VSIw->Rbottom - VSIw->Rtop;
	RSmargininfo(w, lmmax, VSIw->Rleft);	/* update horizontal scroll bar */
	RSdelcols(w, n); /* scroll the window contents */
	if (VSIcursorenabled())
		VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
  /* redraw newly-revealed portion of screen */
	VSredraw(w, (VSIw->Rright - VSIw->Rleft) - n, 0, (VSIw->Rright - VSIw->Rleft), sn);
  } /* VSscrolright */

void VSscrolleft
  (
	short w,
	short n /* number of columns to scroll */
  )
  /* moves the view of the virtual screen within its window the
	specified number of columns to the left. */
  {
	short sn, lmmax;

	if (VSvalids(w) != 0)
		return;

	lmmax = VSIw->maxwidth - (VSIw->Rright - VSIw->Rleft); /* number of invisible columns */

	if (n > VSIw->Rleft)
		n = VSIw->Rleft; /* can't scroll any further left than this */
	if (n == 0)
		return;									/* Do nothing if appropriate */

	if (VSIcursorenabled())
		VSIcuroff(w); /* temporarily hide cursor */
	VSIw->Rleft -= n; /* update visible region */
	VSIw->Rright -= n;
	sn = VSIw->Rbottom - VSIw->Rtop;
	RSmargininfo(w, lmmax, VSIw->Rleft); /* update horizontal scroll bar */
	RSinscols(w, n); /* scroll the window contents */
	if (VSIcursorenabled())
		VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
	VSredraw(w, 0, 0, n, sn); /* redraw newly-revealed portion of screen */
  } /* VSscrolleft */

short VSscroltop( short w /* which window */)					/* JMB 2.6 */
{															/* JMB 2.6 */
	if (VSvalids(w) != 0)									/* JMB 2.6 */
		return(-3);											/* JMB 2.6 */
															/* JMB 2.6 */
	return(VSscrolback(w, VSIw->Rtop + VSIw->numlines)); /* can't scroll back any further than this */
}															/* JMB 2.6 */

short VSscrolback
  (
	short w, /* which window */
	short in /* number of lines to scroll */
  )
  /* moves the view of the virtual screen within its window the
	specified number of lines upwards. */
  {
	short sn, n;

	n = in;

	if (VSvalids(w) != 0)
		return(-3);

	if (VSIw->numlines < (n - VSIw->Rtop))
		n = VSIw->Rtop + VSIw->numlines; /* can't scroll back any further than this */
	if (n <= 0)
		return(0);			/* Dont be scrollin' no lines.... */

	if (VSIcursorenabled())
		VSIcuroff(w); /* temporarily hide cursor */

	VSIw->Rtop = VSIw->Rtop - n; /* adjust the visible region */
	VSIw->Rbottom = VSIw->Rbottom - n;

  /* find the line list element for the new topmost visible line */
	sn = n;
	while (sn-- > 0)
	  {
#ifdef DEBUGMAC
		if (VSIw->vistop->prev == 0L)
			DebugStr("\pVSscrolback -- something wrong with linked list structure");
#endif DEBUGMAC
		VSIw->vistop = VSIw->vistop->prev;
	  } /* while */

	sn = VSIw->Rbottom - VSIw->Rtop;
  /* update vertical scroll bar */
	RSbufinfo(w, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom);

	if (n <= VSIw->lines)
	  {
		RSinslines(w, 0, sn, n, 0);	/* scroll, preserving current selection */
		if (VSIcursorenabled())
			VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
		VSredraw(w, 0, 0, VSIw->maxwidth, n - 1); /* redraw newly-revealed portion */
	  }
	else {
	  /* currently-visible contents scrolled completely off the screen--
		just redraw everything */
		VSredraw(w, 0, 0, VSIw->maxwidth, sn);
	}

	return(0);
  } /* VSscrolback */

short VSscrolforward
  (
	short w, /* which window */
	short n /* number of lines to scroll */
  )
  /* moves the view of the virtual screen within its window the
	specified number of lines downwards. */
  {
	short sn;

	if (VSvalids(w) != 0)
		return(-3);

	if (n > VSIw->lines - VSIw->Rbottom)
		n = VSIw->lines - VSIw->Rbottom; /* can't scroll any further forward than this */
	if (n <= 0)
		return(0);			/* Dont be scrollin' no lines.... */

	if (VSIcursorenabled())
		VSIcuroff(w); /* temporarily hide cursor */

	VSIw->Rtop = n + VSIw->Rtop; /* adjust the visible region */
	VSIw->Rbottom = n + VSIw->Rbottom;

  /* find the line list element for the new topmost visible line */
	sn = n;
	while (sn-- > 0)
	  {
#ifdef DEBUGMAC
		if (VSIw->vistop->next == nil)
			DebugStr("\pVSscrolforward -- something wrong with linked list structure");
#endif DEBUGMAC
		VSIw->vistop = VSIw->vistop->next;
	  } /* while */

	sn = VSIw->Rbottom - VSIw->Rtop;
  /* update vertical scroll bar */
	RSbufinfo(w, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom);

	if (n <= VSIw->lines)
	  {
		RSdellines(w, 0, sn, n, 0);	/* scroll, preserving current selection */
		if (VSIcursorenabled())
			VSIcurson(w, VSIw->x, VSIw->y, 0); /* restore cursor at original position */
		VSredraw(w, 0, (sn + 1) - n, VSIw->maxwidth, sn); /* redraw newly-revealed portion */
	  } /* if */
	else {
	  /* currently-visible contents scrolled completely off the screen--
		just redraw everything */
		VSredraw(w, 0, 0, VSIw->maxwidth, sn);
	}

	return(0);
  } /* VSscrolforward */

short VSsetrgn
  (
	short w,
	short x1, /* leftmost column */
	short y1, /* uppermost line */
	short x2, /* rightmost column */
	short y2 /* bottommost line */
  )
  /* sets the visible region for the specified virtual screen
	in its window, scrolling its contents as appropriate. Assumes
	that either the vertical bounds or the height of the region has
	changed, but not both, and similarly that the horizontal bounds
	or the width has changed, but not both. */
  {
	short n;

	if (VSvalids(w) != 0)
		return(-3);

	VSIw->Rbottom = VSIw->Rtop + (y2 - y1); /* make change in height of visible region first */

	if (x2 > VSIw->maxwidth)
	  {
	  /* trying to make columns visible which aren't there--
		adjust the left and right boundaries to avoid this */
		n = x2 - VSIw->maxwidth; /* how far to adjust to the left */
		if (n > x1)
			n = x1; /* but I'd rather have unused columns on the right than on the left */
		x1 -= n;							/* Adjust left	*/
		x2 -= n;							/* Adjust right */
	  } /* if */

	if (VSIw->Rleft != x1)
	  {
	  /* left margin changed -- scroll horizontally */
	  /* (assume width of region hasn't also changed) */
		n = x1 - VSIw->Rleft;
		if (n > 0)
			VSscrolright(w, n);
		else
			VSscrolleft(w, -n);
	  }
	else
	  /* just update horizontal scroll bar limits for new width of visible region */
		RSmargininfo(w, VSIw->maxwidth - (x2 - x1), x1);

	VSIw->Rleft = x1;
	VSIw->Rright = x2;

	if (VSIw->Rbottom > VSIw->lines)
	  /* don't scroll off the bottom of the screen */
		n = VSIw->Rbottom - VSIw->lines;
	else
	  /* scroll to new topmost line as specified */
		n = VSIw->Rtop - y1;

	if (n != 0)
	  /* scroll vertically (assume height of region hasn't also changed) */
		if (n > 0)
			VSscrolback(w, n);
		else
			VSscrolforward(w, -n);
	else
	  /* update vertical scroll bar limits for new height of visible region */
		RSbufinfo(w, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom);
	return(0);
  } /* VSsetrgn */

short VSscrolcontrol
  (
	short w,
	short scrolon, /* whether to save lines off top */
	short offtop /* whether to save lines on clearing entire screen */
  )
  /* changes scrollback flag settings for a virtual screen. */
  {
	if (VSvalids(w) != 0)
		return(-3);

	if (scrolon > -1)
		VSIw->savelines = scrolon;
	if (offtop > -1)
		VSIw->ESscroll = offtop;

	return(0);
  } /* VSscrolcontrol */

// RAB BetterTelnet 1.2 - set "ignore beeps"
void VSbeepcontrol
	(
		short w,
		short beep
	)
{
	if (VSvalids(w) != 0) return;

	VSIw->ignoreBeeps = beep;
}


/*
 * VSenableblink
 */

void VSenableblink
	(
		short w,
		Boolean enableBlink
	)
{
	if (VSvalids(w) != 0) return;

	VSIw->realBlink = enableBlink;
	VSPulseOne(w, 0, 0, VSIw->maxwidth, VSIw->lines);
}



short VSgetrgn
  (
	short w,
	short *x1,
	short *y1,
	short *x2,
	short *y2
  )
  /* returns the current visible region for a virtual screen. */
  {
	if (VSvalids(w) != 0)
		return(-3);

	*x1 = VSIw->Rleft;
	*y1 = VSIw->Rtop;
	*x2 = VSIw->Rright;
	*y2 = VSIw->Rbottom;
   
	return(0);
  } /* VSgetrgn */


short VSmaxwidth
  (
	short w
  )
  /* returns one less than the number of columns on a virtual screen. */
  {
	if (VSvalids(w) != 0)
		return(-3);
	return(VSIw->maxwidth);
  } /* VSmaxwidth */

VSlinePtr VSIGetLineStart(short w, short y1)
  /* returns a pointer to the specified text line (number may be
	negative for a line in the scrollback buffer). */
  {
	VSlinePtr ptr;
	short n;

	if (VSvalids(w) != 0)
		return((VSlinePtr) -3);

	if (y1 >= 0)
		return(VSIw->linest[y1]);

	n = y1 - VSIw->Rtop;				/* Number of lines from VISTOP to scroll forward */
	ptr = VSIw->vistop;
	while (n > 0)
	  {
		n--;
		if (ptr->next) ptr = ptr->next; // sanity check here - RAB BetterTelnet 2.0b1
	  } /* while */
	while (n < 0)
	  {
		n++;
		if (ptr->prev) ptr = ptr->prev; // more BetterSanityChecking (tm)
	  } /* while */
	return(ptr);
  } /* VSIGetLineStart */

char *VSIstrcopy(char *src, short len, char *dest, short table, short noClip)
  /* copies characters from *src (length len) to *dest, dropping
	trailing blanks. If table is nonzero, then this number (or more) of
	consecutive embedded blanks will be replaced with a tab. Returns a pointer
	to the position one past the last character copied to the *dest buffer. */
{
	char *p, *tempp;
	short tblck;

	p = src + len - 1;
	/* skip trailing blanks, but only if !noClip */
  	if (!noClip)
		while ((*p == ' ') && (p >= src))
			p--;
	if (p < src)
		return(dest);
	if (!table)
	  /* straight character copy */
		while (src <= p)
			*dest++ = *src++;
	else
	  /* tab-replacement copy */
		while (src <= p)
		  {
			while ((src <= p) && (*src != ' '))
				*dest++ = *src++;
			if (src < p)
			  {
				tempp = dest; /* remember start of run of spaces */
				tblck = 0; /* length of run */
				while ((src <= p) && (*src == ' '))
				  {
					*dest++ = *src++;
					tblck++;
				  } /* while */
				if (tblck >= table)
				  {
					*tempp++ = '\011'; /* replace first space with a tab */
					dest = tempp; /* drop remaining spaces */
				  } /* if */
			  } /* if */
		  } /* while */
	return(dest);
} /* VSIstrcopy */


long VSOgettext(short w, short x1, short y1, short x2, short y2, char *charp, long max,
	char *EOLS, short table, short clipspaces)
  /* copies a portion of text from the specified virtual screen into
	the *charp buffer. table, if nonzero, is the minimum length of
	runs of spaces to be replaced with single tabs. Returns the
	length of the copied text. max is supposed to be the maximum
	length to copy, but this is currently ignored!
	EOLS is the end-of-line sequence to insert at line boundaries.
	This is currently assumed to be exactly one character long. */
{
	UNUSED_ARG(max) /* !! */
	short EOLlen;
	short lx,ly,					/* Upper bounds of selection */
		  ux,uy;					/* Lower bounds of selection */
	short maxwid;
	char *origcp;
	VSlinePtr t;

	if (VSvalids(w) != 0)
		return(-3);
	EOLlen = strlen(EOLS);
	maxwid = VSIw->maxwidth;
	origcp = charp;

	// this fails for multi-bytes character set...

	/* Order the lower and upper bounds */
	if (x1 < x2) {
		ux = x1;
		lx = x2;
	} else {
		ux = x2;
		lx = x1;
	}

	if (y1 < y2) {
		uy = y1;
		ly = y2;
	} else {
		uy = y2;
		ly = y1;
	}

	if (uy < -VSIw->numlines) {
		uy = -VSIw->numlines;
		ux = -1;
	}
	if (ly < -VSIw->numlines) {
		ly = -VSIw->numlines;
	}

	if (uy == ly)
	  {
	  /* copying no more than a single line */
		t = VSIGetLineStart(w, uy);

		clipspaces = clipspaces && gApplicationPrefs->clipTrailingSpaces;

		if ((long)(lx-ux) < max)
			charp=VSIstrcopy(&t->text[ux+1], lx-ux, charp, table, !clipspaces);
		else
			charp=VSIstrcopy(&t->text[ux+1], (short)(max - (long)(charp-origcp)), charp, table, !clipspaces);
		if (lx == maxwid)
			*charp++ = *EOLS; /* assumes it's only one character! */
	  }
	else
	  {
	  /* copying more than one line */
		t = VSIGetLineStart(w, uy);
		if (((long) (maxwid-ux) < max))
			charp=VSIstrcopy(&t->text[ux+1],maxwid-ux,charp,table, !clipspaces);
		else
			charp=VSIstrcopy(&t->text[ux+1],(short) (max-(long)(charp-origcp)),charp,table, !clipspaces);
		*charp++ = *EOLS; /* assumes it's only one character! */
		uy++;
		if (t->next) t = t->next; // RAB BetterTelnet 2.0b1 - sanity checking
		while (uy < ly && uy < VSIw->lines)
		  {
			if ((long)(maxwid+1) < max)
				charp=VSIstrcopy(t->text,maxwid+1,charp, table, !clipspaces);
			else
				 charp=VSIstrcopy(t->text,(short)(max - (long) (charp-origcp)),charp, table, !clipspaces);
			*charp++=*EOLS;
			if (t->next) t=t->next; 
			uy++;
		  } /* while */
		if (ly > VSIw->lines)
			lx = maxwid;

		if ((long) (lx+1) < max)
			charp=VSIstrcopy(t->text,lx+1,charp,table, !clipspaces);
		else
			charp=VSIstrcopy(t->text,(short)(max - (long)(charp-origcp)),charp,table, !clipspaces);

		if (lx >= maxwid)
			*charp++ = *EOLS; /* assumes it's only one character! */
	  } /* if */
	return(charp - origcp);
} /* VSgettext */


long VSgettext(short w, short x1, short y1, short x2, short y2,
			char *charp, long max, char *EOLS, short table, short clipspaces)
  /* copies a portion of text from the specified virtual screen into
	the *charp buffer. table, if nonzero, is the minimum length of
	runs of spaces to be replaced with single tabs. Returns the
	length of the copied text. max is supposed to be the maximum
	length to copy, but this is currently ignored!
	EOLS is the end-of-line sequence to insert at line boundaries.
	This is currently assumed to be exactly one character long. */
{
	short mw;
	short lx,ly,					/* Upper bounds of selection */
		  ux,uy;					/* Lower bounds of selection */
	char *origcp;
	short elen;
	short outlen;
	short i;
	VSlinePtr ypt;
	char *pt;
	char *tp;
//	VSAttrib *pa;

	if (VSvalids(w) != 0)
		return(-3);

	if (VSIw->oldScrollback)
		return VSOgettext(w, x1, y1, x2, y2, charp, max, EOLS, table, clipspaces);
	
	mw = VSIw->maxwidth;

	/* limit the lower and upper bounds */
	if (x1 < -1)
		x1 = -1;
	else if (x1 > mw)
		x1 = mw;
	if (x2 < -1)
		x2 = -1;
	else if (x2 > mw)
		x2 = mw;
	if (y1 < -VSIw->numlines) {
		y1 = -VSIw->numlines;
		x1 = -1;
	} else if (y1 > VSIw->lines) {
		y1 = VSIw->lines;
		x1 = mw;
	}
	if (y2 < -VSIw->numlines) {
		y2 = -VSIw->numlines;
		x2 = -1;
	} else if (y2 > VSIw->lines) {
		y2 = VSIw->lines;
		x2 = mw;
	}

	origcp = charp;
	elen = strlen(EOLS);

	// Order the lower and upper bounds
	ux = x1;
	lx = x2;
	uy = y1;
	ly = y2;
	if ( y1 == y2 ) {
		if ( x1 > x2 ) {
			ux = x2;
			lx = x1;
		}
		// single line
		clipspaces = clipspaces && gApplicationPrefs->clipTrailingSpaces;
	} else if (y1 > y2) {
		uy = y2;
		ly = y1;
		ux = x2;
		lx = x1;
	}

	x1 = ux + 1;
	x2 = mw;
	ypt = VSIGetLineStart(w, uy);
	for ( y1 = uy; y1 <= ly && max >= elen; y1++, x1 = 0, x2 = mw, ypt = ypt->next ) {
		if ( y1 == 0 ) {
			// get text from on-screen buffer
			ypt = *VSIw->linest;
		}
		pt = ypt->text;
//		pa = ypt->attr;
		if (y1 == ly)
			x2 = lx; // end last line
		outlen = x2 - x1 + 1;
		if (outlen > max) {
			outlen = max;
		}
		// don't clip trailing spaces if this line is wrapped
		charp = VSIstrcopy(pt + x1, outlen, tp = charp, table, !clipspaces || VSiswrap(ypt->lattr) );
		max -= charp - tp;
		if (max >= elen && !VSiswrap(ypt->lattr) && y1 != ly) {
			strcpy(charp, EOLS);
			charp += elen;
			max -= elen;
		}
	}
	return charp - origcp;
}


long VSgetattr(short w, short x1, short y1, short x2, short y2,
			VSAttrib *attrp, long max)
{
	short mw;
	short lx,ly;					// Lower bounds of selection
	short ux,uy;					// Upper bounds of selection
	VSAttrib *origap;
	short outlen;
	short i;
	VSlinePtr ypt;
	VSAttrib *pa;

	if (VSvalids(w) != 0)
		return(-3);

	if (VSIw->oldScrollback)
		return 0;	// not coded...
	
	mw = VSIw->maxwidth;

	/* limit the lower and upper bounds */
	if (x1 < -1)
		x1 = -1;
	else if (x1 > mw)
		x1 = mw;
	if (x2 < -1)
		x2 = -1;
	else if (x2 > mw)
		x2 = mw;
	if (y1 < -VSIw->numlines) {
		y1 = -VSIw->numlines;
		x1 = -1;
	} else if (y1 > VSIw->lines) {
		y1 = VSIw->lines;
		x1 = mw;
	}
	if (y2 < -VSIw->numlines) {
		y2 = -VSIw->numlines;
		x2 = -1;
	} else if (y2 > VSIw->lines) {
		y2 = VSIw->lines;
		x2 = mw;
	}

	origap = attrp;

	/* Order the lower and upper bounds */
	ux = x1;
	lx = x2;
	uy = y1;
	ly = y2;
	if ( y1 == y2 ) {
		if ( x1 > x2 ) {
			ux = x2;
			lx = x1;
		}
	} else if (y1 > y2) {
		uy = y2;
		ly = y1;
		ux = x2;
		lx = x1;
	}

	x1 = ux + 1;
	x2 = mw;
	ypt = VSIGetLineStart(w, uy);
	for ( y1 = uy; y1 <= ly && max >= sizeof(VSAttrib); y1++, x1 = 0, x2 = mw, ypt = ypt->next ) {
		if (y1 == 0) {
			// get text from on-screen buffer
			ypt = *VSIw->linest;
		}
		pa = ypt->attr;
		if (y1 == ly)
			x2 = lx; // end last line
		outlen = (x2 - x1 + 1) * sizeof(VSAttrib);
		if (outlen > max / sizeof(VSAttrib)) {
			outlen = max / sizeof(VSAttrib);
		}
		memcpy(attrp, pa + x1, outlen * sizeof(VSAttrib));
		attrp += outlen;
		max -= outlen * sizeof(VSAttrib);
	}
	return attrp - origap;
}


short VSgetnumlines
  (
	short w
  )
  /* returns the number of lines in the scrollback. */
  {
	if (VSvalids(w) != 0)
		return(-2);
	return(VSIw->numlines);
  } /* VSgetlines */

short VSgetlines
  (
	short w
  )
  /* returns the number of lines in a virtual screen. */
  {
	if (VSvalids(w) != 0)
		return(-2);
	return(VSIw->lines + 1);
  } /* VSgetlines */

short VSsetlines
  (
	short w, /* window number */
	short lines /* new number of lines */
  )
  /* sets the number of lines in a virtual screen, reallocating text
	and attribute arrays accordingly. Returns the new number of lines
	on success, or an error otherwise */
  {
	VSlineArray linest;				/* For storage of old ones */
	VSlinePtr line;							/* pointer to a line */
	short i, j, oldlines;
	char *temp;
	VSAttrib *tempa;
	
	if (VSvalids(w) != 0)
		return(-3000);

	if (VSIw->oldScrollback)
		return VSOsetlines(w, lines);

	lines -= 1;								/* Correct for internal use */
	oldlines = VSIw->lines;
	if (lines == oldlines)	 				/* no change */
	 	return(0);
	
	VSIw->x = 0;
	VSIw->y = 0;
	if (VSIcursorenabled())
		VSIcurson(w, VSIw->x, VSIw->y, 1); 	/* keeps cursor from pointing outside of window */

	VSIw->vistop = VSIw->scrntop;			/* Force view to the top of the screen */
	
	linest = VSIw->linest;
 
	VSIw->linest = VSInewlinearray(lines + 1); /* allocate new screen buffers */
	if (!VSIw->linest)
	{
		VSIw->linest = linest;		/* mem problems */
		return (-2000);
	}
	
	VSIw->lines = lines; /* set new number of screen lines */
	

	VSIw->linest[0] = VSInewlines(lines + 1); /* allocate new text and attribute lines */
	if (VSIw->linest[0])
	{	/* mem is there */

		VSIlistndx(VSIw->linest[0]); /* build the new screen arrays */
		if (VSIw->savelines)
		{ /* save previous screen contents in scrollback buffer */
			line = linest[oldlines]->next;				/* save continuation */
			linest[oldlines]->next = VSIw->linest[0];
			VSIw->linest[lines]->next = line;			/* restore continuation */
			VSIw->linest[0]->prev = linest[oldlines];	/* backpointer */
			if (line)									/* if there was a follower */
				line->prev = VSIw->linest[lines];		/* new prev for it */
			VSIw->numlines += oldlines;					/* we made more scrollback */
		}
		else
		{  /* get rid of previous screen contents */
			VSIfreelinelist(linest[0]);
			DisposePtr((Ptr) linest);
		}
	}
	else
	{	/* need more mem - emergency */
		if (VSIw->linest[0])					/*if 1/2 of push for memory died, kill the other */
			VSIfreelinelist(VSIw->linest[0]);
		/* Here we should ask if we want to lose the screen buffer!!  CCP */
		VSIfreelines();							  /* release scrollback buffer */
		VSIfreelinelist(linest[0]);				  /* release current visible lines */
		DisposePtr((Ptr) linest);				  
		VSIw->linest[0] = VSInewlines(lines + 1); /* allocate new screen arrays */
		if (!VSIw->linest[0])
		{  /* still not enough memory;  Try to allocate just enough to go back to original size */
			
			if (VSIw->linest[0])					/* this gets rid of useless */
				VSIfreelinelist(VSIw->linest[0]);	/* memory, since we are giving up */
			VSIw->linest[0] = VSInewlines(oldlines + 1); /* try original size */
			
			if (!VSIw->linest[0])
			/* damage control: */
			{										  /* Nope. Give up, and signal that */
				if (VSIw->linest[0])				  /* caller should kill this screen */
					VSIfreelinelist(VSIw->linest[0]);	
				/* dont destroy everything, as this will screw up VSdestroy later */			
				return(-4000);
			}
			else
			{
				lines = oldlines;
				VSIw->lines = lines;
			}
		}
		VSIw->buftop = VSIw->linest[0];
		VSIw->numlines = 0; /* nothing in scrollback */
	  } /* if */

	VSIw->scrntop = VSIw->linest[0];			/* new top of screen */
	VSIw->vistop = VSIw->scrntop;				/* Force a scroll to the top of the screen */
	VSIlistndx(VSIw->scrntop);                  /* rebuild screen arrays */	
	if (!VSIw->savelines)
	  {
	  /* make text line list circular to indicate no extensions */
		VSIw->linest[lines]->next = VSIw->linest[0];
		VSIw->linest[0]->prev = VSIw->linest[lines];
	  } /* if */

  /* initialize the new screen lines to blank text and no attributes */
	for (i = 0; i <= lines; i++)
	  {
		tempa = VSIw->linest[i]->attr;
		temp = VSIw->linest[i]->text;
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*temp++ = ' ';
			*tempa++ = 0;
		  } /* for */
	  } /* for */
  
  /* reset scrolling region */
	VSIw->top = 0;
	VSIw->bottom = lines;
  
  /* reset visible region */
	VSIw->Rtop = 0;
	VSIw->Rbottom = lines;

	
	VSredraw(w, 0, 0, VSIw->maxwidth, lines); /* draw new blank screen */
	RSbufinfo(w, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* adjust vertical scroll bar */

	return(VSIw->lines);

} /* VSsetlines */

short VSOsetlines
  (
	short w, /* window number */
	short lines /* new number of lines */
  )
  /* sets the number of lines in a virtual screen, reallocating text
	and attribute arrays accordingly. Returns the new number of lines
	on success, or an error otherwise */
  {
	VSlineArray linest;				/* For storage of old ones */
	VSattrlineArray attrst;
	VSlinePtr line;							/* pointer to a line */
	short i, j, oldlines;
	char *temp;
	VSAttrib *tempa;
	
	if (VSvalids(w) != 0)
		return(-3000);
	
	lines -= 1;								/* Correct for internal use */
	oldlines = VSIw->lines;
	if (lines == oldlines)	 				/* no change */
	 	return(0);
	
	VSIw->x = 0;
	VSIw->y = 0;
	if (VSIcursorenabled())
		VSIcurson(w, VSIw->x, VSIw->y, 1); 	/* keeps cursor from pointing outside of window */

	VSIw->vistop = VSIw->scrntop;			/* Force view to the top of the screen */
	
	attrst = VSIw->attrst; /* save old screen arrays */
	linest = VSIw->linest;
 
	VSIw->linest = VSInewlinearray(lines + 1); /* allocate new screen buffers */
	if (!VSIw->linest)
	{
		VSIw->linest = linest;		/* mem problems */
		return (-2000);
	}
	
	VSIw->attrst = (VSattrlinePtr *)VSInewlinearray(lines + 1);
	if (!VSIw->attrst)
	{								/* mem problems */
		DisposePtr((Ptr) VSIw->linest);
		VSIw->linest = linest;
		VSIw->attrst = attrst;
		return (-2000);
	}
	
	VSIw->lines = lines; /* set new number of screen lines */
	

	VSIw->linest[0] = VSOnewlines(lines + 1,1); /* allocate new text and attribute lines */
	VSIw->attrst[0] = (VSattrlinePtr)VSOnewlines(lines + 1,sizeof(VSAttrib));
	if (VSIw->linest[0] && VSIw->attrst[0])
	{	/* mem is there */
		
		/* dispose of old attribute lines */
		VSIfreelinelist((VSlinePtr)attrst[0]);
		DisposePtr((Ptr) attrst);

		VSOlistndx(VSIw->linest[0],VSIw->attrst[0]); /* build the new screen arrays */
		if (VSIw->savelines)
		{ /* save previous screen contents in scrollback buffer */
			line = linest[oldlines]->next;				/* save continuation */
			linest[oldlines]->next = VSIw->linest[0];
			VSIw->linest[lines]->next = line;			/* restore continuation */
			VSIw->linest[0]->prev = linest[oldlines];	/* backpointer */
			if (line)									/* if there was a follower */
				line->prev = VSIw->linest[lines];		/* new prev for it */
			VSIw->numlines += oldlines;					/* we made more scrollback */
		}
		else
		{  /* get rid of previous screen contents */
			VSIfreelinelist(linest[0]);
			DisposePtr((Ptr) linest);
		}
	}
	else
	{	/* need more mem - emergency */
		if (VSIw->linest[0])					/*if 1/2 of push for memory died, kill the other */
			VSIfreelinelist(VSIw->linest[0]);
		if (VSIw->attrst[0])
			VSIfreelinelist((VSlinePtr)VSIw->attrst[0]);
		/* Here we should ask if we want to lose the screen buffer!!  CCP */
		VSIfreelines();							  /* release scrollback buffer */
		VSIfreelinelist(linest[0]);				  /* release current visible lines */
		DisposePtr((Ptr) linest);				  
		VSIfreelinelist((VSlinePtr)attrst[0]);				  /* release current visible attrib */
		DisposePtr((Ptr) attrst);
		VSIw->linest[0] = VSOnewlines(lines + 1,1); /* allocate new screen arrays */
		VSIw->attrst[0] = (VSattrlinePtr)VSOnewlines(lines + 1,sizeof(VSAttrib));
		if (!VSIw->linest[0] || !VSIw->attrst[0])
		{  /* still not enough memory;  Try to allocate just enough to go back to original size */
			
			if (VSIw->linest[0])					/* this gets rid of useless */
				VSIfreelinelist(VSIw->linest[0]);	/* memory, since we are giving up */
			if (VSIw->attrst[0])									
				VSIfreelinelist((VSlinePtr)VSIw->attrst[0]);

			VSIw->linest[0] = VSOnewlines(oldlines + 1,1); /* try original size */
			VSIw->attrst[0] = (VSattrlinePtr)VSOnewlines(oldlines + 1,sizeof(VSAttrib));
			
			if (!VSIw->linest[0] || !VSIw->attrst[0])
			/* damage control: */
			{										  /* Nope. Give up, and signal that */
				if (VSIw->linest[0])				  /* caller should kill this screen */
					VSIfreelinelist(VSIw->linest[0]);	
				if (VSIw->attrst[0])				
					VSIfreelinelist((VSlinePtr)VSIw->attrst[0]);
				/* dont destroy everything, as this will screw up VSdestroy later */			
				return(-4000);
			}
			else
			{
				lines = oldlines;
				VSIw->lines = lines;
			}
		}
		VSIw->buftop = VSIw->linest[0];
		VSIw->numlines = 0; /* nothing in scrollback */
	  } /* if */

	VSIw->scrntop = VSIw->linest[0];			/* new top of screen */
	VSIw->vistop = VSIw->scrntop;				/* Force a scroll to the top of the screen */
	VSOlistndx(VSIw->scrntop, VSIw->attrst[0]); /* rebuild screen arrays */	
	VSIw->attrst[0]->prev = VSIw->attrst[lines];	/* Make attribute list circular */
	VSIw->attrst[lines]->next = VSIw->attrst[0];
	if (!VSIw->savelines)
	  {
	  /* make text line list circular to indicate no extensions */
		VSIw->linest[lines]->next = VSIw->linest[0];
		VSIw->linest[0]->prev = VSIw->linest[lines];
	  } /* if */

  /* initialize the new screen lines to blank text and no attributes */
	for (i = 0; i <= lines; i++)
	  {
	  	VSIw->linest[i]->lattr = 0;
		tempa = VSIw->attrst[i]->text;
		temp = VSIw->linest[i]->text;
		for (j = 0; j <= VSIw->allwidth; j++)
		  {
			*temp++ = ' ';
			*tempa++ = 0;
		  } /* for */
	  } /* for */
  
  /* reset scrolling region */
	VSIw->top = 0;
	VSIw->bottom = lines;
  
  /* reset visible region */
	VSIw->Rtop = 0;
	VSIw->Rbottom = lines;

	VSOredraw(w, 0, 0, VSIw->maxwidth, lines); /* draw new blank screen */
	RSbufinfo(w, VSIw->numlines, VSIw->Rtop, VSIw->Rbottom); /* adjust vertical scroll bar */

	return(VSIw->lines);

} /* VSOsetlines */

/*--------------------------------------------------------------------------*/
/* NCSA: SB - VSsetcols 													*/
/* 	This allows external procedures to set the column width.  Used by 		*/
/*	routines in main, to allow ARBITRARY column widths						*/
/*--------------------------------------------------------------------------*/
short VSsetcols(short w, short myWidth)									/* NCSA: SB */
{																	/* NCSA: SB */
	if (VSvalids(w) != 0)											/* NCSA: SB */
		return(-1);													/* NCSA: SB */
	VSIw->maxwidth = myWidth;										/* NCSA: SB */
	return 0;
} 																	/* NCSA: SB */


/*--------------------------------------------------------------------------*/
/* NCSA: SB - VSgetcols 													*/
/* 	This returns the column width.  Used by SetScreenDimensions, when		*/
/*	the procedure needs to know the initial column width					*/
/*--------------------------------------------------------------------------*/
short VSgetcols(short w)												/* NCSA: SB */
{																	/* NCSA: SB */
	if (VSvalids(w) != 0)											/* NCSA: SB */
		return(-1);													/* NCSA: SB */
	return VSIw->maxwidth;											/* NCSA: SB */
} 																	/* NCSA: SB */

// BetterTelnet 2.0fc1 - integrated DJ's blinker code
// DJ: VSPulseAll is an attempt to JUST do the blinkers.
// VSPulseOne redraws one window with a different RS call.  Still huge overkill....but oh well.
short VSPulseOne(short w, short x1, short y1,short x2, short y2);

void VSPulseAll(void)
{
	short	i;

	for ( i = 0; i < VSmax; i++ ) {
		if ( VSscreens[i].stat == 1 && VSscreens[i].loc->realBlink )
			VSPulseOne(i, 0, 0, VSscreens[i].loc->maxwidth, VSscreens[i].loc->lines);
	}
}

short VSPulseOne
  (
	short w, 		// window to redraw
	short x1, 		
	short y1,
	short x2,
	short y2
  )
  /* redisplays the specified portion of a virtual screen. */
  {
	VSlinePtr ypt;
	VSattrlinePtr ypa;
	short y;
	short tx1, tx2, ty1, ty2, tn, offset;
	short sx1;
	short sx2;
	short cursOff;

	if (VSvalids(w) != 0)
		return(-3);

	if (VSIw->oldScrollback)
		return VSOPulseOne(w, x1, y1, x2, y2); // RAB

	x1 += VSIw->Rleft;	// Make local coords global again
	x2 += VSIw->Rleft;
	y1 += VSIw->Rtop;
	y2 += VSIw->Rtop;

	if (x1 < 0) x1 = 0;
	else if (x1 > VSIw->maxwidth) x1 = VSIw->maxwidth;
	if (x2 < 0) x2 = 0;
	else if (x2 > VSIw->maxwidth) x2 = VSIw->maxwidth;
	if (y1 < -VSIw->numlines) y1 = -VSIw->numlines;
	else if (y1 > VSIw->lines) y1 = VSIw->lines;
	if (y2 < -VSIw->numlines) y2 = -VSIw->numlines;	/* limit to amount of scrollback */
	else if (y2 > VSIw->lines) y2 = VSIw->lines;

	tx1 = x1;		// Set up to clip redraw area to visible area
	tx2 = x2;
	ty1 = y1;
	ty2 = y2;
	tn = -1;		// so we include more than 1 line
	
	if (VSIclip(&tx1, &ty1, &tx2, &ty2, &tn, &offset)!=0) return 0;		// test clip region
	
	RSa = -1;

	cursOff = 0;

	// we need to access only the visible part
	
	sx1 = tx1;
	sx2 = tx2;

	ypt = VSIw->vistop;
	for(y=VSIw->Rtop; y<y1; y++)
		ypt = ypt->next;		// Get pointer to top line we need

	for (y=ty1; y<=ty2; y++) {
		char *pt;
		VSAttrib *pa;
		VSAttrib lasta;
		short x, lastx;

		pt = ypt->text + VSIw->Rleft;
		pa = ypt->attr + VSIw->Rleft;

		// if double size, we must shift width
		if (VSisdecdwh(ypt->lattr)) {
			tx1 = (tx1 & 1) ? (tx1 >> 1) - 1 : (tx1 >> 1);
			tx2 = (tx2 & 1) ? (tx2 >> 1) + 1 : (tx2 >> 1);
		}

		lastx = tx1;
		lasta = pa[tx1];
		for( x = tx1+1; x <= tx2; x++ ) {
			if ( pa[x] != lasta ) {
				if ( VSisblnk(lasta) ) {
					if ( y == VSIw->y && lastx <= VSIw->x && VSIw->x <= x && VSIcursorvisible()) {
						// temporarily hide cursor
						cursOff = 1;
						VSIcuroff(w);
					}
					RSdraw(w, lastx, y, ypt->lattr, lasta, x-lastx, pt + lastx);
					if ( cursOff ) {
						// restore cursor at original position
						cursOff = 0;
						VSIcurson(w, VSIw->x, VSIw->y, 0);
					}
				}
				lastx = x;
				lasta = pa[x];
			}
		}
		if ( lastx <= tx2 && VSisblnk(lasta) ) {
			if ( y == VSIw->y && lastx <= VSIw->x && VSIw->x <= x && VSIcursorvisible() ) {
				// temporarily hide cursor
				cursOff = 1;
				VSIcuroff(w);
			}
			RSdraw(w, lastx, y, ypt->lattr, lasta, tx2-lastx+1, pt + lastx);
			if ( cursOff ) {
				// restore cursor at original position
				cursOff = 0;
				VSIcurson(w, VSIw->x, VSIw->y, 0);
			}
		}

		// if double size, we must shift width
		if (VSisdecdwh(ypt->lattr)) {
			tx1 = sx1;
			tx2 = sx2;
		}

		ypt = ypt->next;
	}

	/* back to default window colors */
    RSsetwind(w);
	RSsetattr( 0, 0 );

	return 0;
}

short VSOPulseOne
  (
	short w, 		// window to redraw
	short x1, 		
	short y1,
	short x2,
	short y2
  )
  /* redisplays the specified portion of a virtual screen. */
  {
	VSlinePtr ypt;
	VSattrlinePtr ypa;
	short y;
	short tx1, tx2, ty1, ty2, tn, offset;
	short cursOff;
	short sx1;
	short sx2;

	if (VSvalids(w) != 0)
		return(-3);

	x1 += VSIw->Rleft;	// Make local coords global again
	x2 += VSIw->Rleft;
	y1 += VSIw->Rtop;
	y2 += VSIw->Rtop;

	if (x1 < 0) x1 = 0;
	else if (x1 > VSIw->maxwidth) x1 = VSIw->maxwidth;
	if (x2 < 0) x2 = 0;
	else if (x2 > VSIw->maxwidth) x2 = VSIw->maxwidth;
	if (y1 < -VSIw->numlines) y1 = -VSIw->numlines;
	else if (y1 > VSIw->lines) y1 = VSIw->lines;
	if (y2 < -VSIw->numlines) y2 = -VSIw->numlines;	/* limit to amount of scrollback */
	else if (y2 > VSIw->lines) y2 = VSIw->lines;

	tx1 = x1;		// Set up to clip redraw area to visible area
	tx2 = x2;
	ty1 = y1;
	ty2 = y2;
	tn = -1;		// so we include more than 1 line
	
	if (VSIclip(&tx1, &ty1, &tx2, &ty2, &tn, &offset)!=0) return 0;		// test clip region
	
	RSa = -1;

	cursOff = 0;

	if (y1 < 0)
		y1 = 0;

	// draw visible part of on-screen buffer, taking account of attributes
	if (y2 >= 0) {

		tx1 = x1;		// Set up to clip redraw area to visible area of on-screen buffer
		tx2 = x2;
		ty1 = y1;
		ty2 = y2;
		tn = -1;

		if (!VSIclip(&tx1, &ty1, &tx2, &ty2, &tn, &offset)) {

			sx1 = tx1;
			sx2 = tx2;

			ypt = VSIw->linest[VSIw->Rtop+ty1];
			ypa = VSIw->attrst[VSIw->Rtop+ty1];

			for (y=ty1; y<=ty2; y++) {
				char *pt;
				VSAttrib *pa;
				VSAttrib lasta;
				short x, lastx;

				pt = ypt->text + VSIw->Rleft;
				pa = ypa->text + VSIw->Rleft;

				// if double size, we must shift width
				if (VSisdecdwh(ypt->lattr)) {
					tx1 = (tx1 & 1) ? (tx1 >> 1) - 1 : (tx1 >> 1);
					tx2 = (tx2 & 1) ? (tx2 >> 1) + 1 : (tx2 >> 1);
				}

				lastx = tx1;
				lasta = pa[tx1];
				for( x = tx1+1; x <= tx2; x++ ) {
					if ( pa[x] != lasta ) {
						if ( VSisblnk(lasta) ) {
							if ( y == VSIw->y && lastx <= VSIw->x && VSIw->x <= x && VSIcursorvisible() ) {
								// temporarily hide cursor
								cursOff = 1;
								VSIcuroff(w);
							}
							RSdraw(w, lastx, y, ypt->lattr, lasta, x-lastx, pt + lastx);
							if ( cursOff ) {
								// restore cursor at original position
								cursOff = 0;
								VSIcurson(w, VSIw->x, VSIw->y, 0);
							}
						}
						lastx = x;
						lasta = pa[x];
					}
				}
				if ( lastx <= tx2 && VSisblnk(lasta) ) {
					if ( y == VSIw->y && lastx <= VSIw->x && VSIw->x <= x && VSIcursorvisible() ) {
						// temporarily hide cursor
						cursOff = 1;
						VSIcuroff(w);
					}
					RSdraw(w, lastx, y, ypt->lattr, lasta, tx2-lastx+1, pt + lastx);
					if ( cursOff ) {
						// restore cursor at original position
						cursOff = 0;
						VSIcurson(w, VSIw->x, VSIw->y, 0);
					}
				}

				// if double size, we must shift width
				if (VSisdecdwh(ypt->lattr)) {
					tx1 = sx1;
					tx2 = sx2;
				}

				ypt = ypt->next;
				ypa = ypa->next;
			}
		}
	}

	/* back to default window colors */
    RSsetwind(w);
	RSsetattr( 0, 0 );

	return 0;
}



// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
 *
 *      Virtual Graphics Kernel Macintosh Real Graphics Interface
 *                          (vrrgmac.c)
 *
 *   National Center for Supercomputing Applications
 *      by Gaige B. Paulsen
 *
 *    This file contains the macintosh real screen calls for the NCSA
 *  Virtual Graphics Kernel.
 *
 *	Following are the Per Device calls:
 *
 *   MacRGraster( p,x1,y1,x2,y2,wid)- Plot raster in rect @(x1,y1,x2,y2) with wid @ p
 *   MacRGcopy( x1,y1,x2,y2,x3,y3,x4,y4)- 
 *   MacRGmap( offset,count,data)   - 
 *
 *
 *  WARNING, WARNING!
 *  Gaige has this cute idea about how to do "subwindows" of real windows by shifting
 *  the window number by 4 bits (MAC_WINDOW_SHIFT).  Then, the remainder is the
 *  sub-window number.  It will probably work, but you MUST keep the shifted and
 *  non-shifted numbers straight.  For example, MacRGdestroy() and MacRGremove() take
 *  different uses of the window number right now.  
 *
 *
 *  Macintosh only Routines:
 *
 *      Version Date    Notes
 *      ------- ------  ---------------------------------------------------
 *		0.5 	880912	Initial Coding -GBP
 *      1.0     890216  Minor fixes for 1.0 - TKK
 */


#define __ALLNU__


#include "maclook.proto.h"
#include "vdevice.h"
#include "vdevice.proto.h"
#include "vr.h"
#include "InternalEvents.h"

#define MAX_MAC_RGS	8
#define MAX_MAC_SUB 16
#define MAC_WINDOW_SHIFT	4		/* Bits shifted */
#define	MAC_SUB_MASK	0xf			/* Bits maksed */

#include "vrrgmac.proto.h"
#include "errors.proto.h"
#include "telneterrors.h"



typedef struct MacWindow {
	VDevice			vdev;			/* virtual device to draw in, has its own colors */
	WindowPtr 		window;			/* My Window  (0L if not in use ) */
	PaletteHandle 	palette;		/* My Palette */
	char			title[256];		/* Title string */
	
	
	Point			size;			/* My height and width */
	Rect			subs[MAX_MAC_SUB];/* Rectangles of my subwindows [0,0,0,0] if not in use */
	} MacWindow;

MacWindow *MacRGs;

short RGwn=0;							/* Window number in use */
short RGsub=0;						/* Sub-Window Number in use */

void MacRGinit(void)
{
	short i;
	
	MacRGs= (MacWindow *)myNewPtr( MAX_MAC_RGS * sizeof(MacWindow));
	for (i=0;i<MAX_MAC_RGS;i++)
		MacRGs[i].window=NULL;
}

/************************************************************
 * MacRGnewwindow( name, x1, y1, x2, y2)		-			*
 *		make a new subwindow to wn							*
 ************************************************************/

short MacRGnewwindow
  (
	char *name,
	short x1,
	short y1,
	short x2,
	short y2
  )
{
	short w,i ;
	Rect wDims;
	RGBColor curcol;

	
	for (w=0; w<MAX_MAC_RGS && MacRGs[w].window; w++)
		/* loop */;
	
	if (w>= MAX_MAC_RGS)
		return( -1);
		
	if ((x2 - x1) & 1)						/* odd width, must be even */
		x2++;
	
	SetRect( &wDims,  x1+40, y1+40, x2+40, y2+40);
		
	strcpy( MacRGs[w].title, name);						/* Copy of the name */
	
	if (!TelInfo->haveColorQuickDraw)		/* Borrow from RS */
		return(-1);
	else 
	{
		short i;
		Str255	scratchPstring;
		strcpy((char *)scratchPstring, name);
		CtoPstr((char *)scratchPstring);
		
		MacRGs[w].window=NewCWindow(NULL, &wDims, scratchPstring, TRUE, noGrowDocProc, kInFront,	/* BYU LSC */
			TRUE, (long) w);
		
		if (!MacRGs[w].window)
		{
			DoError(108 | MEMORY_ERRORCLASS, LEVEL2, NULL);		
			return(-1);
		}
		MacRGs[w].vdev.bounds = &MacRGs[w].window->portRect;

		
		MacRGs[w].palette = NewPalette( 256, NULL, pmTolerant, 0);
		if (!MacRGs[w].palette)
		{
			DisposeWindow(MacRGs[w].window);
			DoError(108 | MEMORY_ERRORCLASS, LEVEL2, NULL);		
			return(-1);
		}
		for (i=0; i<256; i++) {				/* load with grey-scale */
			curcol.red = i<<8;
			curcol.green = i<<8;
			curcol.blue = i<<8;
			SetEntryColor( MacRGs[w].palette, i, &curcol);
		}
		if (InitVDevice(&MacRGs[w].vdev,MacRGs[w].palette))		/* get vdevice going */
		{
			DisposePalette(MacRGs[w].palette);
			DisposeWindow(MacRGs[w].window);
			DoError(108 | MEMORY_ERRORCLASS, LEVEL2, NULL);		
			return(-1);
		}
	}
		
	((WindowPeek)MacRGs[w].window)->windowKind = WIN_ICRG;	
	MacRGs[w].size.h = x2-x1;
	MacRGs[w].size.v = y2-y1;
	for (i=0; i<MAX_MAC_SUB; i++)
		SetRect( &MacRGs[w].subs[i], 0,0,0,0);			/* Reset the subs */
	RGwn = w;
	RGsub= 0;
	return (w << MAC_WINDOW_SHIFT);
}

/****************************************
 * MacRGsubwindow(wn)		-			*
 *		make a new subwindow to wn		*
 ****************************************/

short MacRGsubwindow( short wn)
{
	return (wn);
}

/****************************************
 * MacRGsetwindow(wn)		-			*
 *		set the drawing window to wn	*
 ****************************************/
void MacRGsetwindow( short wn)
{
	short w = wn >> MAC_WINDOW_SHIFT;
	
	if (!MacRGs[w].window)
		return;
	
	SetPort( MacRGs[w].window);
	RGwn = w;
	RGsub= wn & MAC_SUB_MASK;
	
	/* Optionally set the clip region */
}

/****************************************
 * MacRGdestroy(wn)		-				*
 *		destroy window wn				*
 ****************************************/
void MacRGdestroy(short wn)
{
	
//	sprintf((char *) tempspot,"destroy: %d", wn); putln((char *) tempspot);	/* BYU LSC */
	if (!MacRGs[wn].window)
		return;
	
	VRdestroybyName((char *) &MacRGs[wn].title);
}
	
/****************************************
 * MacRGremove(wn)		-				*
 *		destroy window wn				*
 ****************************************/
void MacRGremove
  (
	short wn
  )
{
	CGrafPtr cgp;
	short w = wn>> MAC_WINDOW_SHIFT;
	
	if (!MacRGs[w].window)
		return;
	
	TrashVDevice(&MacRGs[w].vdev);
	
	
	cgp = (CGrafPtr) MacRGs[w].window;					/* unseed window color table */
	(*(*(cgp->portPixMap))->pmTable)->ctSeed = GetCTSeed();
	
	DisposeWindow( MacRGs[w].window);		/* Get rid of the actual window */

	if (MacRGs[w].palette)
		DisposePalette( MacRGs[w].palette);
		
	MacRGs[w].palette = NULL;
	MacRGs[w].window = NULL;
//	sprintf((char *) tempspot,"take away: %d", w); putln((char *) tempspot);	/* BYU LSC */
}

short MacRGfindwind(WindowPtr wind)
{
	short i=0;
	
	if (!wind)
		return(-2);
	
	while (i<MAX_MAC_RGS && wind != MacRGs[i].window)
		i++;
	if (i==MAX_MAC_RGS)
		return(-1);

	return( i);
}

/************************************************************************************/
/* MacRGcopy
*  Copybits the image window into the clipboard.
*  
*/
RGBColor	icrwhite = { 0xffff,0xffff,0xffff },
			icrblack = { 0,0,0}; 
			
void MacRGcopy(WindowPtr wind)
{
	Rect copysize,copyfrom;
	long len,wn;
	PicHandle picture;
	PixMapHandle hidep;
		
	if (( wn= MacRGfindwind( wind)) <0)
		return;						/* Couldn't do it */
 
	hidep = MacRGs[wn].vdev.whichWorld->portPixMap;
	copyfrom = *MacRGs[wn].vdev.bounds;
			
	SetPort(wind);
	
	copysize = copyfrom;						/* boundary of drawing area */

	picture= OpenPicture(&copysize);

	ClipRect(&copysize);
	
/*	RGBBackColor(&icrwhite);
	RGBForeColor(&icrblack); */

	ForeColor( blackColor);
	BackColor( whiteColor);
	LockPixels(hidep);
	CopyBits((BitMap *) (*hidep), &wind->portBits,
			&copyfrom, &copysize, srcCopy, NULL); 
			
	UnlockPixels(hidep);

	ClosePicture();
	
/*	put the PICT into the scrap manager */
	len = GetHandleSize((Handle) picture);
	HLock((Handle) picture);
	ZeroScrap();
	PutScrap( len, 'PICT', (Ptr) *picture);
	HUnlock((Handle) picture);
	KillPicture(picture);

}

short MacRGupdate( WindowPtr wind)
{
	short wn;
	GWorldPtr theWorld;
	if (( wn= MacRGfindwind( wind)) <0)
		return(-1);						/* Couldn't do it */
 
 	theWorld = MacRGs[wn].vdev.whichWorld;
 	SetPort(wind);
	ForeColor( blackColor);
	BackColor( whiteColor);
	BeginUpdate(wind);

	LockPixels(theWorld->portPixMap);
	CopyBits(&((GrafPtr)theWorld)->portBits,&wind->portBits,&theWorld->portRect,
					&wind->portRect, srcCopy, NULL);
	UnlockPixels(theWorld->portPixMap);
	EndUpdate( wind);
	return(0);
}

/**************************** Hereafter lie the graphics routines ************************/
short	MacRGraster(char *data, short x1, short y1, short x2, short y2, short rowbytes)
{
	Rect tr;
	char *p,*baseOfThisRun;
	short i;
	PixMapHandle pix;
	CGrafPtr origPort;
	GDHandle origDev;
	GWorldPtr theOffscreenWorld;
	long temp;
	if (!MacRGs[RGwn].window)
		return(-1);
	theOffscreenWorld = MacRGs[RGwn].vdev.whichWorld;
	
	GetGWorld(&origPort,&origDev); //save old settings
	
	SetGWorld(theOffscreenWorld,NULL);
	pix = GetGWorldPixMap(MacRGs[RGwn].vdev.whichWorld);
	LockPixels(pix);
	baseOfThisRun = (char *)GetPixBaseAddr(pix);
	temp = (long)((*pix)->rowBytes & 0x3fff);
	baseOfThisRun += temp*y1 + x1; 
	p = baseOfThisRun;								
	for (i=0; i<rowbytes; i++)
		*p++ = *data++;
	UnlockPixels(pix);
	SetRect(&tr, x1,y1, x2+1,y2+1);
	SetGWorld(origPort,origDev);
	InvalRect(&tr);
	return(0);
}

short	MacRGcopyrgn(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4)
{
	UNUSED_ARG(x1)
	UNUSED_ARG(y1)
	UNUSED_ARG(x2)
	UNUSED_ARG(y2)
	UNUSED_ARG(x3)
	UNUSED_ARG(y3)
	UNUSED_ARG(x4)
	UNUSED_ARG(y4)
	/* copy one region to another within wn */
	return 0;
}

short	MacRGmap(short start, short length, char *data)
{
	short i;
	RGBColor curcol;
	
	for (i=start; i<start+length; i++) {		
		curcol.red = (*data++)<<8;
		curcol.green = (*data++)<<8;
		curcol.blue = (*data++)<<8;
		SetEntryColor( MacRGs[RGwn].palette, i, &curcol);
	}
	
	SetPalette( MacRGs[RGwn].window, MacRGs[RGwn].palette, TRUE);
	ActivatePalette( MacRGs[RGwn].window);

	ColorVDevice(&MacRGs[RGwn].vdev,MacRGs[RGwn].palette);
#if 0															/* BYU LSC */
	sprintf((char *) tempspot, "Palette[%d,%d]",start,length);	/* BYU LSC */
	putln((char *) tempspot);									/* BYU LSC */
#endif
	return 0;
}
		

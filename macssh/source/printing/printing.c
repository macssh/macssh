/*
*    printing.c
*
*	This now contains the PrintPages() code from Roland Mansson, from Lund 
*	University Computing Center.  This allows for printer redirection, and also
*	printing page numbers (and other goodies).  Thanks for all of your work!
*
*   Code to handle printing.
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "wind.h"
#include "rsinterf.proto.h"	/* For RSGetSelText */
#include "netevent.proto.h"
#include "vgtek.proto.h"
#include "rgmp.proto.h"
#include "event.proto.h"
#include "tekrgmac.proto.h"



#include "vsdata.h"
#include "vsinterf.proto.h"

extern Cursor *theCursors[];
extern WindRec
	*screens;			/* The screen array from Maclook */
extern short scrn;

PROTO_UPP(printsleep, PrIdle);
extern	void printsleep(void);

THPrint	gPrRecHandle = NULL;

#define PGRECT	(*PrRecHandle)->prInfo.rPage		/* Macro for making the pagerect more accessible */
#define VMARGINS 4
#define PAPRECT (*PrRecHandle)->rPaper /* Macro for making the paperrect more accessible */
#define bDevCItoh 1 /* from Printing.h */
#define ascLF 10
#define ascFF 12
#define ascCR 13

#include "printing.proto.h"

void	PrintingUnload(void) {}

/*	userabort - check to see if the user wants to abort */
short userabort(void)
{
	EventRecord theEvent;

	while (GetNextEvent(24,&theEvent)) {
		if ((theEvent.modifiers & cmdKey) &&
			(theEvent.message & 0xff) =='.') 
			return(-1);
		}
	return(0);
}

/*	printsleep()	- Provide the network keep alive while we are in print
 *					  mode.  Also set the abort flag if necessary. */
SIMPLE_UPP(printsleep, PrIdle);
void printsleep(void)
{
	short	CurrentResFile;
	
	CurrentResFile = CurResFile();		/* Save the current resource file just in case. */
	Stask();							/* act like the postman */
	if (userabort())
		(*gPrRecHandle)->prJob.fFromUsr=TRUE;
	UseResFile(CurrentResFile);			/* Restore this in case we were bad and changed it. */
}

/*	printGraph -	Print the graphics on the current window
 *		vg - which graphics window to print */

void printGraph(short dnum)			/* Which drawing to print */
{
	short		h,v;			/* used for centering (h=horiz. , v=vert.) */
	short		wh,wv;			/* Window horiz and vert */
	TPrStatus	prStatus;		/* Printing status record */
	Rect		prRect;			/* the rectangle to print in */
	TPPrPort	prPort;			/* the printing port */
	short		j;				/* VG identifier for pass-through */
	THPrint		PrRecHandle;	/* our print record handle */
	short		vs;

	vs = VGgetVS(dnum);
	if ( vs < 0 )
		return;

	PrRecHandle = PrintSetupRecord();
		
	setLastCursor(theCursors[normcurs]);

	if (PrJobDialog(PrRecHandle)) {			/* Cancel the print if FALSE */
		prPort=PrOpenDoc(PrRecHandle,0L,0L);		/* Open Printer Port */
			if (PrError() == 0)	{				/* If we can't, then die */
				PrOpenPage(prPort, 0L);				/* Open a page */

				(*PrRecHandle)->prJob.pIdleProc = printsleepUPP;	/* our sleep/cancel*/

				prRect=PGRECT;						/* Get the page rectangle */
				h=prRect.right - prRect.left;	/* Get the width */
				v=prRect.bottom- prRect.top;	/* Get the height */
				if (3*h<4*v) {					/* Center the little bugger */
					wh = h;
					wv = (3 * h)/ 4;
					}
				else {							/* On the page rectangle */
					wv = v;
					wh = (4 * v)/ 3;
					}
		
				prRect.top  = (v- wv) /2;
				prRect.left = (h- wh) /2;
				prRect.bottom = prRect.top + wv;
				prRect.right = prRect.left + wh;
				j = VGnewwin(TEK_DEVICE_PICTURE, vs, VGgettektype(vs), VGgettekclear(vs)); 
				RGMPsize( &prRect );
				VGzcpy( dnum, j);				/* Love dat zm factr */
				VGredraw(dnum,j);				/* Copy the picture in i to j */
				VGclose(j);						/* OK, we're done, give it to someone else */
				PrClosePage(prPort);			/* Page out.... */
				}

			PrCloseDoc(prPort);					/* Done with the document */
			putln("Doc is closed... the printing begins....");

		if (PrError()==0)													/* BYU 2.4.20 - changed for HP DeskWriter*/
			PrPicFile(PrRecHandle,0L,0L,0L,&prStatus);
									/* Print the spooled file if applicable */
	}		/* if PrJobDialog */
	
	DisposeHandle((Handle)PrRecHandle);
	updateCursor(1);
}





/* LU - this is the NEW PrintPages code.  This handles page numbering, and also
		allows us to dump the redirected output to the printer.  Written by
		Roland Mansson, Lund University Computing Center.  Thank HIM for this code... */
/* Some patches had to be made to get it to compile under THINK C, and to work		*/
/* with some of the changes we have recently made.   - SMB							*/




/*	printPages(prPort,charh,title, columns) -
 *			Prints <charh> on the printer through port <prPort> with <title>
 *			using <columns>.
 */
void printPages(TPPrPort prPort, THPrint PrRecHandle, Str255 Title, short columns, char **charh, short refNum, short useTitle, short theScreen)
{
	char	*charp, *start;
	long	charlen,count=0L;
	short		v, h, scount, lines, pgcount, pgtmplen, maxlines;
	char	pagetemp[20];
	FMetricRec info;
	short		pFheight, pFwidth;
	unsigned char buf[256];		/* to build a line in from the file */
	unsigned char nextchar;		/* next unprocessed char in file */
	short		rdy;
	short	indent;				/* indent to give reasonable left marginal */
	OSErr	sts;
	long	dummyCount;
	char	tmp[100];			/* only for debugging */
	char	stupidarray[256];	/* used in menuseg for finding string widths */

	for (v=0; v<256; v++) stupidarray[v]='W';	/* Set up the width array */

	indent = ((*PrRecHandle)->prInfo.iHRes * 180)/254;	/* 1.8 centimeters left margin */
	if (-PAPRECT.left > indent)
		indent = 0;
	else
		indent += PAPRECT.left;

//	if ( ((*PrRecHandle)->prStl.wDev>>8) == bDevCItoh ) {
//					/* Think about this: put def. font in rsrc fork- STR255 */
//		TextFont(monaco);			/* gives monaco on ImageWriters É */
//		v=9;
//		}
//	else {							/* NCSA: SB - use current font */
		TextFont((*(screens[theScreen].wind)).txFont);		/* NCSA: SB */
		TextFace((*(screens[theScreen].wind)).txFace);		/* NCSA: SB */
		v = (*(screens[theScreen].wind)).txSize;			/* NCSA: SB */
//		}													/* NCSA: SB */
	
	do {
		TextSize(v);
		FontMetrics( &info );
		pFheight = FixRound( info.ascent + info.descent + info.leading);
		pFwidth  = FixRound( info.widMax);
		//pFwidth  = CharWidth('W');			/* Re-assign to largest char */
		v--;
	} while ( TextWidth(stupidarray,0,columns+1) > (PGRECT.right - PGRECT.left - indent));
	sprintf (tmp,"Fheight:%d, Fwidth:%d, TextSize:%d",pFheight,pFwidth,v+1); putln (tmp);

	if (charh!=NULL) {
		HLock(charh);
		start=charp=*charh;
		charlen= GetHandleSize(charh);
	} else {
		if ((sts=GetFPos(refNum, &charlen)) != noErr)
			{
			sprintf(tmp,"GetFPos: ERROR %d",(short)sts);
			putln(tmp);
			}
		charlen-=3;					/* skip last 3 chars, as they are part of ESC seq */
		if ((sts=SetFPos(refNum,(short) fsFromStart,0L)) != noErr)	/* rewind to beginning of file */
			{ sprintf(tmp,"SetFPos: ERROR %d",sts); putln(tmp); }
		start = (char *)buf;				/* always (the array doesn't move) */
		dummyCount=1;
		if ((sts=FSRead(refNum,&dummyCount,&nextchar)) != noErr)		/* get first char */
			{ sprintf(tmp,"FSRead: ERROR %d",sts); putln(tmp); }
	}
	
	h=PGRECT.right - PGRECT.left - indent;	/* Get the width */
	v=PGRECT.bottom- PGRECT.top;			/* Get the height */

	maxlines = v/pFheight-1;
	pgcount = 0;
	while (count<charlen) {
		pgcount++;
		lines = 1;
		PrOpenPage(prPort, 0L);		/* Open the Printer Page */
		if ((sts=PrError()) != noErr) { sprintf(tmp,"PrOpenPage: ERROR %d",sts); putln(tmp); }
		(*PrRecHandle)->prJob.pIdleProc= printsleepUPP;			/* our netsleep */
		sprintf (tmp,"printing page:%d",pgcount); putln(tmp);

		if (useTitle) {
			Str255 SessionNameString;
			GetIndString(SessionNameString,MISC_STRINGS,SESSION_NAME_STRING);
			MoveTo( indent, lines*pFheight);
			DrawString(SessionNameString);			/* BYU LSC */
	
			DrawString( Title);			/* BYU LSC */
			sprintf(pagetemp, "Page %d", pgcount);
			pgtmplen=strlen(pagetemp);
			MoveTo( PGRECT.right-(pgtmplen * pFwidth)-1, lines++*pFheight);
			DrawText( pagetemp, 0, pgtmplen);
			lines++;					/* one blank line after title line */
		}

		if ( charh != NULL ) {									/* print from handle */
			while ( lines <= maxlines && count < charlen ) {
				scount = 0;
				while ( count < charlen ) {
					if ( scount > columns ) {
						// eat next char if CR
						if (*charp == '\r') {
							++charp;
							++count;
						}
						break;
					}
					++count;
					if ( *charp++ == '\r' ) {
						break;
					}
					++scount;
				}
				MoveTo( indent, lines++ * pFheight );
				if ( scount > 0 ) {
					DrawText( start, 0, scount );
				}
				start = charp;
			}
		} else {											/* print from file */
			while (lines <= maxlines && count<charlen) {
				rdy = 0;
				scount = 0;
				while ( count < charlen && !rdy ) {
					if ( scount > 250 || scount > columns )
						nextchar = ascLF;
					switch (nextchar) {
						case ascCR:
							rdy=1;
							break;
						case ascLF:
							rdy=1;
							break;
						case ascFF:
							rdy=1;
							break;
						default:
							buf[scount++] = nextchar;
							count++;
							dummyCount = 1;
							if ((sts = FSRead (refNum,&dummyCount,&nextchar)) != noErr) {
								sprintf(tmp,"FSRead: ERROR %d",sts);
								putln(tmp);
							}
							break;
					}
				}
				MoveTo(indent, lines * pFheight);
				if (scount > 0)
					DrawText(start, 0, scount);
				if ( nextchar == ascLF )
					lines++;						/* LF -> new line */
				if ( nextchar == ascFF )
					if (screens[theScreen].ignoreff) // RAB BetterTelnet 1.0fc8
						lines++;
					else
						lines = maxlines+1;				/* FF -> new page */
				dummyCount = 1;
				if ((sts=FSRead (refNum,&dummyCount,&nextchar)) != noErr)
					{ sprintf(tmp,"FSRead: ERROR %d",sts); putln(tmp); }
				count++;
			}
		}
		PrClosePage(prPort);		/* Close the Printer page */
		if ((sts=PrError()) != noErr) { sprintf(tmp,"PrClosePage: ERROR %d",sts); putln(tmp); }
	}
	if (charh!=NULL)
		HUnlock(charh);
}

/*	printText -	Print the text selected on the screen 
 *		vs - which vs to print from */
void printText
  (
	short vs,			/* screen to use for window size */
	Handle charh,		/* text to display */
	Str255 Title,		/* The title string */
  	short scrn			/* screen to use for fonts etc... */
  )
{
	TPrStatus	prStatus;		/* Status record */
	TPPrPort	prPort;			/* the Printer port */
	THPrint		PrRecHandle;	/* our print record handle */

	PrRecHandle = PrintSetupRecord();

	setLastCursor(theCursors[normcurs]);
	
	if (PrJobDialog(PrRecHandle)) {			/* Cancel the print if FALSE */
		prPort = PrOpenDoc(PrRecHandle, 0L, 0L);
		if (PrError() == 0)	{
			printPages( prPort, PrRecHandle, Title, VSmaxwidth(vs), charh, (short) -1, TRUE, scrn);
		}
		if ( prPort ) {
			PrCloseDoc(prPort);
			if (((*PrRecHandle)->prJob.bJDocLoop == bSpoolLoop) && (PrError()==0))
				PrPicFile(PrRecHandle,0L,0L,0L,&prStatus); /* Spool the print */
		}
	}
	updateCursor(1);
	DisposeHandle((Handle)PrRecHandle);
}


/*	printScreen -	Print the current screen */
void printScreen
  (
	short vs,			/* Which screen to print */
	Str255 Title,		/* The title string */
  	short scrn
  )
{
	char **charh;

	charh = RSGetTextScreen(vs, 0, 0); /* Get the characters to print, including spaces */
	if ( charh ) {
		printText(vs, charh, Title, scrn);
		DisposeHandle(charh);
	}
}


/*	printText -	Print the text selected on the screen 
 *		vs - which vs to print from */
void printSel
  (
	short vs,			/* Which screen to print */
	Str255 Title,		/* The title string */
  	short scrn
  )
{
	char		**charh;

	charh = RSGetTextSel(vs, 0, 0);	/* Get the characters to print, including spaces */
	if ( charh ) {
		printText(vs, charh, Title, scrn);
		DisposeHandle(charh);
	}
}


void PrintPageSetup(void)
{
	THPrint		PrRecHandle;	/* our print record handle */
	
	PrOpen();

	PrRecHandle = (THPrint)GetResource('JOHN',169);					/* AISD: JSA */
	if ( PrRecHandle != NULL ) {							/* AISD: JSA */
		PrStlDialog(PrRecHandle);					/* AISD: JSA */
		ChangedResource((Handle)PrRecHandle);		/* AISD: JSA */
		UpdateResFile(TelInfo->SettingsFile);		/* AISD: JSA */
		}											/* AISD: JSA */
	else {											/* AISD: JSA */
		PrRecHandle=(THPrint)myNewHandle((long)sizeof(TPrint));	/* AISD: JSA */
		PrintDefault(PrRecHandle);					/* AISD: JSA */
		PrStlDialog(PrRecHandle);					/* AISD: JSA */
		UseResFile(TelInfo->SettingsFile);			/* AISD: JSA */
		AddResource((Handle)PrRecHandle,'JOHN',169,"\p");	/* AISD: JSA */
		UpdateResFile(TelInfo->SettingsFile);		/* AISD: JSA */
		}											/* AISD: JSA */

	ReleaseResource((Handle)PrRecHandle);		/* AISD: JSA */
	PrClose();
}

void PrintScreen(void)
{
		short	i;

		PrOpen();
			
		i=RGgetdnum(FrontWindow());
		if (i>-1) 		
			printGraph( i);						/* Print Graphics */
		else 
			if ( (i=RSfindvwind(FrontWindow())) >-1 ) {
				Str255 Title;

				GetWTitle( FrontWindow(), Title);
				printScreen(i, Title,scrn);	/* Print Text selection */
				}
		PrClose();
}

void PrintSelection(void)
{
		short	i;

		PrOpen();
			
		i=RGgetdnum(FrontWindow());
		if (i>-1) 		
			printGraph( i);						/* Print Graphics */
		else 
			if ( (i=RSfindvwind(FrontWindow())) >-1 ) {
				Str255 Title;

				GetWTitle( FrontWindow(), Title);
				printSel(i, Title,scrn);	/* Print Text selection */
				}
		PrClose();
}

THPrint PrintSetupRecord(void)
{
	THPrint	PrRecHandle;
	
	PrRecHandle = (THPrint)GetResource('JOHN',169);					/* AISD: JSA */

	if ( PrRecHandle != NULL ) {
		PrValidate(PrRecHandle);
		DetachResource((Handle)PrRecHandle);
		}
	else {
		PrRecHandle=(THPrint)myNewHandle((long)sizeof(TPrint));
		PrintDefault(PrRecHandle);
		}
		
	gPrRecHandle = PrRecHandle;
	return (PrRecHandle);
}

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// #pragma profile on

/*
 *
 *      Virtual Screen Kernel Emulation Routines
 *                      (vsem.c)
 *  
 *   National Center for Supercomputing Applications
 *      by Gaige B. Paulsen
 *
 *    This file contains the private emulation calls for the NCSA
 *  Virtual Screen Kernel.
 *
 *-----------------------------------------------------------------------------------
 *	This also now contains Roland Mansson's code for printer redirection
 *	Thanks for all of the work, Roland!
 *-----------------------------------------------------------------------------------
 *
 *      Version Date    Notes
 *      ------- ------  ---------------------------------------------------
 *      0.01    861102  Initial coding -GBP
 *      0.10    861111  Added/Modified VT emulator -GBP
 *      0.50    861113  First compiled edition -GBP
 *		2.1		871130	NCSA Telnet 2.1 -GBP
 *		2.2 	880715	NCSA Telnet 2.2 -GBP
 *		2.6		12/92	Telnet 2.6: Fixed some VS stuff, and added LU changes
 */
 

#include "wind.h"
#include "vsdata.h"
#include "vskeys.h"
#include "rsdefs.h"
#include "translate.h"
#include "rsinterf.proto.h"
#include "rsmac.proto.h"
#include "vsintern.proto.h"
#include "vsinterf.proto.h"
#include "translate.proto.h"
#include "maclook.proto.h"
#include "printing.proto.h"
#include "event.proto.h"
#include "menuseg.proto.h"
#include "url.proto.h"

#include "vsem.proto.h"

#include <stdarg.h>

extern THPrint PrRecHandle;		/* LU - our print record handle from menu.c */
extern Cursor *theCursors[];
extern long		TempItemsDirID;
extern short	TempItemsVRefNum;
extern WindRec *screens;

#ifdef DEBUG_CAPTURE
extern VSscrndata *VSscreens;
#endif

extern WindRec	*console; 
extern void syslog( int priority, const char *format, ...);

extern Boolean gInitialized;

int gVSemlogging = 0;

void VSprintf(char *fmt, ...);
void VSdump(char *p, int len);

#define CUR_DEF         0
#define CUR_NONE        1
#define CUR_UNDERLINE   2
#define CUR_LOWER_THIRD 3
#define CUR_LOWER_HALF  4
#define CUR_TWO_THIRDS  5
#define CUR_BLOCK       6
#define CUR_HWMASK      0x0f
#define CUR_SWMASK      0xfff0


void	VSunload(void) {}

/* LU - this is the start of the main LU changes for doing printer redirection 	*/
/* 	Once again I would like to thank Roland Mansson, of the Lund University 
	Computing Center (Sweden) for all of his work on Telnet.   Not only for the 
	national char support, but also for the printer redirection support, a new 
	version of PrintPages, and everything else.  Thanks a lot, and keep up the
	good work!!!		-SMB													*/
/* LU - we have gotten the escape sequence to turn on printer redirection, so do so */ 
void VSprON(void)
{
	OSErr	sts;
	long	myEOF;
	
	VSprintf("printer redirection ON\n");

	VSIw->prredirect = 1;

	if (VSIw->qprint && VSIw->printqueued) {
		GetEOF(VSIw->refNum, &myEOF);
		if (myEOF >= 3)
			SetEOF(VSIw->refNum, myEOF - 3);
		SetFPos(VSIw->refNum, 2, 0L);
		return;
	}

	if (VSIw->qprint) VSIw->printqueued = 1;

	VSIw->prbuf = 0x00000000;
	VSIw->prbuf2 = 0x00;
	sprintf (VSIw->fname,"MacSSH TempFile #%d",VSIwn);
	c2pstr(VSIw->fname);

	if ((sts = HCreate(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname, '????', 'TEXT')) != noErr) {
		if (sts != dupFNErr) {
			SysBeep(1);
			VSIw->prredirect = 0;
			VSprintf("Create: ERROR %d\n",sts);
			return;
			}
		}
	if ((sts = HOpenDF(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname, fsRdWrPerm, &(VSIw->refNum))) != noErr) {
		SysBeep(1);
		VSIw->prredirect = 0;
		VSprintf("FSOpen: ERROR %d\n",sts);
		HDelete(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname);
		return;
		}
	if (SetEOF(VSIw->refNum, 0L)) {
		SysBeep(1);
		VSIw->prredirect = 0;
		VSprintf("VSPRON:SETEOF ERROR\n");
		FSClose(VSIw->refNum);
		VSIw->refNum = -1;
		HDelete(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname);
		return;
		}
}


/* LU - we just got the escape sequence to turn OFF redirection.  Take what we have
		and dump it to the printer */

void VSprOFF(void)
{
	Str255		Title;
	TPrStatus	prStatus;	/* Status record */
	TPPrPort	prPort;		/* the Printer port */
	OSErr		sts;
	GrafPtr		savePort;
	short		sn;		/* NCSA: SB - the screen # */
	THPrint		PrRecHandle;
	
	VSprintf("printer redirection OFF\n");
	if (VSIw->prredirect==0)			/* no redirection started! */
		return;
	VSIw->prredirect = 0;
	if (VSIw->qprint) return;
	GetPort (&savePort);				/* save old port */

	PrOpen();
	
	PrRecHandle = PrintSetupRecord();
	
	GetWTitle ((GrafPtr) RSgetwindow(VSIwn),Title);
	setLastCursor(theCursors[normcurs]);
	
	if (PrJobDialog(PrRecHandle)) {			/* Cancel the print if FALSE */
		if ((sts=PrError()) != noErr){
			VSprintf("PrJobDialog: ERROR %d\n",sts);
		}
		prPort=PrOpenDoc(PrRecHandle,0L,0L);
		if ((sts=PrError()) != noErr) {
			SysBeep(1);
			VSprintf("PrOpenDoc: ERROR %d\n",sts);
		} else {
			sn = findbyVS(VSIwn);					/* NCSA: SB */
			if (sn < 0)	{
				PrClose();					/* NCSA: SB */
				DisposeHandle((Handle)PrRecHandle);
				return;							/* NCSA: SB */
			}
			printPages (prPort, PrRecHandle, Title, VSmaxwidth(VSIwn), NULL, VSIw->refNum, 0L,sn);
			PrCloseDoc(prPort);
			if ((sts=PrError()) != noErr) {
				VSprintf("PrCloseDoc: ERROR %d\n",sts);
			}
			if (((*PrRecHandle)->prJob.bJDocLoop == bSpoolLoop) && (PrError()==0)) {
				PrPicFile(PrRecHandle,0L,0L,0L,&prStatus); /* Spool if necessaryÉ */
				if ((sts=PrError()) != noErr) {
					VSprintf("PrPicFile: ERROR %d\n",sts);
				}
			}
		}
	}
	
	PrClose();
	DisposeHandle((Handle)PrRecHandle);
	
	SetPort (savePort);				/* restore old port */
	if ((sts=FSClose (VSIw->refNum)) != noErr) {
		SysBeep(1);
		VSprintf("FSClose: ERROR %d\n",sts);
	}
	VSIw->refNum = -1;

	if ((sts=HDelete(TempItemsVRefNum, TempItemsDirID, (StringPtr)VSIw->fname)) != noErr) {
		SysBeep(1);
		VSprintf("HDelete: ERROR %d\n",sts);
	}
	updateCursor(1);
}


#define ENDOFPRT 	'\033[4i'		/* <ESC>[4i   (0x1b5b3469) */
#define ENDOFPRT2	'[?4i'			// only valid if (prbuf2 == '\033')

static OSErr VStranslatewrite(char *string, long len)
{
	OSErr	theErr = 0;
	char	buf[512];
	long	buflen;
	long	count;
 	WindRecPtr tw;
 	RSdata *temp;
  	short	sn;

 	sn = findbyVS(VSIwn);
	if ( sn >= 0 ) {
		tw = &screens[sn];
		while (!theErr && len) {
			buflen = len > 256 ? 256 : len;
			count = sizeof(buf);
			trbuf_nat_mac(tw, (unsigned char *)string, &buflen, (unsigned char *)buf, &count);
			theErr = FSWrite(VSIw->refNum, &count, buf);
			len -= buflen;
			string += buflen;
		}
	} else {
		theErr = FSWrite(VSIw->refNum, &len, string);
	}
	return theErr;
}

void VSpr(unsigned char **pc, short *pctr)
{
	long count;				/* number of chars to print to file */
	char *start; 			/* original start of buffer */
	OSErr sts;
	char tmp[100];			/* only for debugging */
	short rdy;				/* true if <ESC>[4i or <ESC>[?4i */

	count = 0;				
	start = (char *)*pc;	
	rdy = 0;
	sts = 0;

	while ( *pctr > 0 && !rdy && sts == noErr ) {
		VSIw->prbuf2=(VSIw->prbuf>>24);
		VSIw->prbuf=(VSIw->prbuf<<8) + **pc;
		if (VSIw->prbuf==ENDOFPRT) {
										/* i.e. no more redirection */
			rdy=1;
			count--;					/* will be incremented again below */
		}
		if ((VSIw->prbuf==ENDOFPRT2) && (VSIw->prbuf2 == '\033')) {
										/* i.e. no more redirection */
			rdy=1;
			count--;					/* will be incremented again below */
			count--;
		}
		count++;
		if (**pc == 0) {								// RAB BetterTelnet 1.0fc7
														// We don't want NULLs here!
			count--;
			sts = VStranslatewrite(start, count);
			start += (count + 1);
			count = 0;
		}
		(*pc)++;
		(*pctr)--;
	}
	sts = VStranslatewrite(start, count);
	if ( sts != noErr ) {
		SysBeep(1);
		VSprintf("FSWrite: ERROR %d\n",sts);
	}
	if (rdy || sts)
		VSprOFF();
}


/*
 * VSprintf
 */

void VSprintf(char *fmt, ...)
{
	va_list		args;
	char		string_buf[1024];
	int			len;
	short		sx;

	if ( gInitialized && !gVSemlogging && console && console->wind && VSIwn != console->vs ) {
		++gVSemlogging;
		sx = VSIwn;
		//VSvalids(sx);
		//RSsetwind(sx);
		va_start( args, fmt );
		len = vsnprintf( string_buf, sizeof(string_buf) - 1, fmt, args );
		va_end( args );
		if ( len > 0 ) {
			putlln( string_buf, len );
		}
		//VSvalids(sx);
		VSIwn = sx;
		VSIw = VSscreens[sx].loc;
		RSsetwind(sx);
		--gVSemlogging;
	}
}

/*
 * VSdump
 */

void VSdump(char *p, int len)
{
	va_list		args;
	char		string_buf[1024];
	static		int gVSemlogging = 0;
	short		sx;

	if ( gInitialized && !gVSemlogging && console && console->wind && VSIwn != console->vs) {
		++gVSemlogging;
		sx = VSIwn;
		dumpln( (long)p, 0, p, len );
		VSIwn = sx;
		VSIw = VSscreens[sx].loc;
		--gVSemlogging;
	}
}


/*
 * VSdump
 */

static void trflush(WindRec *tw)
{
	if ( tw ) {
		trflush_nat_mac( tw );
		VSIw->trinx = 0;
		VSIw->trincount = 0;
	}
}

/* escape states for VSem */

enum {
	esc_root = 0,		/* default */
	esc_basic,			/* processing ESC character */
	esc_csi,			/* processing ESC '[' character */
	esc_decl,
	esc_descl,
	esc_G0,
	esc_G1,
	esc_G2,
	esc_G3,
	esc_G0j,
	esc_G2j,
	esc_XTerm,
	esc_url
};


/* LU - that is the end of the new routines needed for printer redirection 	*/
/* LU - now we just patch up VSem() to use this code, and were done! 		*/

void VSem
  (
	unsigned char *c, /* pointer to character string */
	short ctr /* length of character string */
  )
  /* basic routine for placing characters on a virtual screen, and
	interpreting control characters and escape sequences. Simple
	interpretation of controls & escapes is done here, while the
	harder stuff is done by calling VSIxx routines in vsintern.c. */
{
    register short sx;
    register short escflg; /* state of escape sequence interpretation */
    short escflags; /* state of escape sequence interpretation */
    VSAttrib attrib;
    short insert, extra, offend;
    char  *current, *start;
	VSAttrib *acurrent;
	short sn;
	WindRec *tw;
	Boolean tryit;
	Boolean captured;
	short savedX;
	short savedY;
	unsigned char savedChar;
	long val;
	short mw;
	Boolean needwrap;
	int i;

    escflg = VSIw->escflg;
    escflags = VSIw->escflags;
    
    captured = false;

//VSprintf( "VSem:" );
//VSdump( c, ctr );

#ifdef DEBUG_CAPTURE
	if (TelInfo->debug) {
		if (VSscreens[VSIwn].captureRN) {
			unsigned char *tc = c;
			short tctr = ctr;
			while (tctr != 0) {
				VScapture(tc, (tctr > 255) ? 255 : tctr);
				tc += ((tctr > 255) ? 255 : tctr);
				tctr -= ((tctr > 255) ? 255 : tctr);
			}
			captured = true;
		}
	}
#endif
	sn = findbyVS(VSIwn);
	tw = (sn >= 0) ? &screens[sn] : NULL;

    while (ctr > 0) {

		mw = VSIw->maxwidth;
		if (VSisdecdwh(VSIw->lattrib)) {
			mw >>= 1;
			if ( !(VSIw->maxwidth & 1) )
				mw -= 1;
		}

		if (VSIw->prredirect)	/* PR - printer redirection? */
			VSpr(&c,&ctr);		/* PR -if yes, call VSpr */
								/* PR - when we return from VSpr there may (ctr!=0) É */
								/* PR - É or may not (ctr==0) be chars left in *c to print */
		tryit = 0;
		while (escflg == esc_root && ctr > 0 && *c < 32 && !tryit) {
			switch (*c) {
				case 0x1b: /* esc */
					escflg = esc_basic;
					break;
				case 0x0e: /* Lock Shift G1 */
					if (VSIw->G1)
						VSIw->attrib = VSgraph(VSIw->attrib);
					else
						VSIw->attrib = VSnotgraph(VSIw->attrib);
					VSIw->charset = 1;
					break;
				case 0x0f: /* Lock Shift G0 */
					if (VSIw->G0)
						VSIw->attrib = VSgraph(VSIw->attrib);
					else
						VSIw->attrib = VSnotgraph(VSIw->attrib);
					VSIw->charset = 0;
					break;
				case 0x07: /* bell */
					if (!VSIw->ignoreBeeps)
						RSbell(VSIwn);
					break;
				case 0x08: /* backspace */
					if (VSIw->x > 0)
						--VSIw->x;
					trflush(tw);
					break;
				case 0x0c: /* ff */
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();
					break;
				case 0x09: /* ht */		/* Later change for versatile tabbing */
					VSItab();
					if (!captured)
						VScapture(c,1);				
					break;
				case 0x0a: /* lf */
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();
					break;
				case 0x0d: /* cr */
					VSIw->x = 0;
					if (!captured)
						VScapture(c,1);				
					trflush(tw);
					break;
				case 0x0b: /* vt */
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();
					break;
				default:
					if ( *c >= 0x10 && VSIw->vtemulation >= 2 ) {
						tryit = 1;
						c--;
						ctr++;
					}
					break;
			} 
			++c;
			--ctr;
		}
		if ( escflg == esc_root && ctr > 0
		 && (*c & 0x80) && *c < 0xA0 && VSIw->vtemulation != 0 && VSIw->vteightbits ) {
			// VT220 eightbit starts here
			switch (*c) {									
				case 0x84: /* ind */		//same as ESC D
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();		
					goto ShortCut;			
				case 0x85: /* nel */		//same as ESC E
					VSIw->x = 0;				
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();				
					goto ShortCut;			
				case 0x88: /* hts */		//same as ESC H 
					VSIw->tabs[VSIw->x] = 'x';	
					goto ShortCut;	
				case 0x8d: /* ri */			//same as ESC M 
					VSIrindex();		
					goto ShortCut;	
				case 0x9b: /* csi */		//same as ESC [ 
					VSIapclear();			
					escflg = esc_csi;			
					escflags = 0;
					++c;			//CCP			
					--ctr;					
					break;						
				case 0x86: /* ssa */			// - same as ESC 'F'
				case 0x87: /* esa */			// - same as ESC 'G'
					goto ShortCut;				
				case 0x8e: /* ss2 */			// - same as ESC 'N'
					// switch to G2 for next char only
					// FIXME
					goto ShortCut;				
				case 0x8f: /* ss3 */			// - same as ESC 'O'
					// switch to G3 for next char only
					// FIXME
					goto ShortCut;				
				case 0x90: /* dcs */			// - same as ESC 'P'
				case 0x93: /* sts */			// - same as ESC 'S'
				case 0x96: /* spa */			// - same as ESC 'V'
				case 0x97: /* epa */			// - same as ESC 'W'
					goto ShortCut;
				case 0x9d: /* osc */			// - same as ESC ']'
					if (tw && tw->Xterm) {
						escflg = esc_XTerm;
						escflags = 0;
						break;
					}
					goto ShortCut;
				case 0x9e: /* pm */				// - same as ESC ^
				case 0x9f: /* apc */			// - same as ESC _
				default:
					goto ShortCut;				
			} 					
		}//end if vt220

		while (escflg == esc_root && ctr > 0 && (*c >= 32 || tryit)
		  && !((*c & 0x80) && *c < 0xA0 && VSIw->vtemulation != 0 && VSIw->vteightbits)) {

			// loop around, printing lines of text one at a time

			// where to put next char
			current = start = &VSIw->linest[VSIw->y]->text[VSIw->x];
			// where to put corresponding attribute byte
			if (VSIw->oldScrollback) {
				acurrent = &VSIw->attrst[VSIw->y]->text[VSIw->x];
			} else {
				acurrent = &VSIw->linest[VSIw->y]->attr[VSIw->x];
			}

			attrib = VSIw->attrib;	// current writing attribute
			insert = VSIw->IRM;		// insert mode (boolean)
			offend = 0;				// wrapped to next line (boolean)
			extra = 0;				// overwriting last character of line
			sx = VSIw->x;			// starting column of area needing redrawing

			if ( VSIw->x > mw ) {
				if (VSIw->DECAWM) { // wrap to next line
					VSIw->x = 0;
					VSIw->linest[VSIw->y]->lattr |= kVSwrap;
					VSIindex();
				} else {			// stay at right margin 
					VSIw->x = mw;
				}
				current = start = &VSIw->linest[VSIw->y]->text[VSIw->x];
				if (VSIw->oldScrollback)
					acurrent = &VSIw->attrst[VSIw->y]->text[VSIw->x];
				else
					acurrent = &VSIw->linest[VSIw->y]->attr[VSIw->x];
				sx = VSIw->x;
			}

			// Write characters on a single line
			while (ctr > 0 && (*c >= 32 || tryit) && offend == 0
			  && !((*c & 0x80) && *c < 0xA0 && VSIw->vtemulation != 0 && VSIw->vteightbits)) {
				// translate to mac chars
				if ( tw ) {
					int res;
					long inlen;
					unsigned char trbuf[32];
					long trlen;
					unsigned char *pbuf;
					VSAttrib trattr;

					pbuf = VSIw->trinbuf;
					pbuf[VSIw->trincount++] = *c;
					inlen = VSIw->trincount;
					trlen = sizeof(trbuf);
//VSprintf( "itrin:" );
//VSdump( pbuf, inlen );
					res = trbuf_nat_mac( tw, pbuf, &inlen, trbuf, &trlen );
					if ( res && res != kTECPartialCharErr ) {
//VSprintf( "###itrfailed: %d\n", res );
						// translation failed, leave data as-is
						if ( VSIw->x > mw ) {
							// hit right margin, autowrap now
							if ( !VSIw->DECAWM ) {
								// just eat it
								++c;
								--ctr;
							} else {
								// autowrap to start of next line
								++VSIw->x;
								offend = 1;
								if ( VSIw->trinx ) {
									++sx;	// one less char to display on the right
								}
							}
							--VSIw->trincount;
							continue;
						}
						trlen = VSIw->trincount;
						trattr = attrib | kVSansi2b;
					} else {
						// translation ok, or no data yet
						if ( inlen ) {
							// keep a few chars
							for (i = inlen; i < VSIw->trincount; i++) {
								pbuf[i - inlen] = pbuf[i];
							}
							VSIw->trincount -= inlen;
						}
						if ( !trlen ) {
							// nothing yet
							if ( VSIw->trincount < sizeof(VSIw->trinbuf) ) {
								if ((*acurrent & kVSansi2b)) {
									// try to display next byte too ?
									extra = 1;
								}
								++VSIw->x;
								++VSIw->trinx;
								++current;
								++acurrent;
								++c;
								--ctr;
								if (insert) {
									// make room for the char(s)
									VSIinschar(1, 0);
								}
								continue;
							}
							// temp translation buffer full, unable to translate...
							trlen = VSIw->trincount;
//VSprintf( "###itrfull: %d\n", trlen );
							VSIw->trincount = 0;
							//res = trflush_nat_mac( tw );
							trattr = attrib;
						} else {
							// translation complete
							pbuf = trbuf;
							trattr = attrib | kVSansi2b;
							extra = 0;
						}
					}
					if (insert) {
						// make room for the char
						VSIinschar(1, 1);
					}
					current -= VSIw->trinx;
					acurrent -= VSIw->trinx;
					for (i = 0, --current, --acurrent; i < trlen; ++i) {
						*(++current) = pbuf[i];
						*(++acurrent) = trattr;	// tag first chars
					}
					VSIw->lastchar = *current;

					// set attribute
					if ( trlen == 1 && (*current & 0x80) && !VSIw->trintag
					  && GetTranslationIndex(tw->innational) < 0 ) {
						// tag allowed in multi-byte
						VSIw->trintag = 1;
					} else {
						// don't tag last char
						*acurrent = attrib;
						VSIw->trintag = 0;
					}
					if ( VSIw->x == sx && sx > 0 && (acurrent[-1] & kVSansi2b)) {
						// display previous byte too
						--sx;
						--start;
						*acurrent = attrib;
						VSIw->trintag = 0;
					}
					if ((*acurrent & kVSansi2b)) {
						// try to display next byte too ?
						extra = 1;
					}
					VSIw->trinx = 0;
					VSIw->trincount = 0;
//VSprintf( "itrout:" );
//VSdump( pbuf, trlen );
				} else {
					/* no window, no translation */
					VSIw->lastchar = *c;
					if (insert) // make room for the char 
						VSIinschar(1, 1);
					*current = *c;
					*acurrent = attrib;
				}
				++c;
				--ctr;
				if ( VSIw->x < mw ) {
					// advance the cursor position
					++acurrent;
					++current;
					++VSIw->x;
				} else {
					// hit right margin
					if (VSIw->DECAWM) {
						// autowrap to start of next line
						++VSIw->x;
						offend = 1; // terminate inner loop 
					} else {
						// stay at right margin
						VSIw->x = mw;
						extra = 1; // cursor position doesn't advance 
					} 
				} 
			} // we've got a line full of text in the virtual screen

		  	// now update the screen to show what we've done
		  	extra += VSIw->x - sx;
			if ( insert ) {
				RSinsstring( VSIwn, VSIw->x - extra, VSIw->y, VSIw->lattrib, VSIw->attrib, extra, start );
			} else { 
				short x2, y2, offset, cx = sx, cy = VSIw->y, extraCopy = extra;
				if (!VSIclip(&cx, &cy, &x2, &y2, &extraCopy, &offset)) {
//VSprintf( "idraw:" );
//VSdump( start + offset, extraCopy );
					RSdraw( VSIwn, cx, cy, VSIw->lattrib, VSIw->attrib, extraCopy, start + offset );
				}
			}
			if ( !captured ) {
				VScapture( (unsigned char *)start, extra );
			}
		}

		while ( escflg == esc_basic && ctr > 0 ) {
			/* basic escape sequence processing */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case '[':	// csi
					VSIapclear();
					escflg = esc_csi;
					escflags = 0;
					break;
				case '7':
					VSIsave();
					goto ShortCut;
				case '8':
					VSIrestore();
					goto ShortCut;
				case 'c':
					VSIreset();
					goto ShortCut;
				case 'D':
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();
					goto ShortCut;
				case 'E':
					VSIw->x = 0;
					VSIw->linest[VSIw->y]->lattr &= ~kVSwrap;
					VSIindex();
					goto ShortCut;
				case 'M':
					VSIrindex();
					goto ShortCut;
				case 'N':	// Single Shift G2
					// switch to G2 for next char only
					// FIXME
					goto ShortCut;
				case 'O':	// Single Shift G3
					// switch to G2 for next char only
					// FIXME
					goto ShortCut;
				case '>':
					VSIw->DECPAM = 0;
					goto ShortCut;
				case '=':
					VSIw->DECPAM = 1;
					goto ShortCut;
				case 'Z':
					VTsendident();
					goto ShortCut;
				case ' ':			// sure ?
				case '#':
					escflg = esc_decl;
					break;
				case '(':
					escflg = esc_G0;
					break;
				case ')':
					escflg = esc_G1;
					break;
				case '*':
					escflg = esc_G2;
					break;
				case '+':
					escflg = esc_G3;
					break;
				case '$':
					escflg = esc_G0j;
					escflags = 0;
					break;
				case '.':
					escflg = esc_G2j;
					break;
				case 'H':
					VSIw->tabs[VSIw->x] = 'x';
					goto ShortCut;
#ifdef CISB
				case 'I':
					bp_ESC_I();
					break;
#endif CISB
				case ']':
					if (tw && tw->Xterm) {
						escflg = esc_XTerm;
						escflags = 0;
						break;
					}
					goto ShortCut;
				case 'n':	// Lock Shift G2, Left   (VT200 mode only)
				case '}':	// Lock Shift G2, Right  (VT200 mode only)
				case 'o':	// Lock Shift G3, Left   (VT200 mode only)
				case '|':	// Lock Shift G3, Right  (VT200 mode only)
					// switch to charset
					// FIXME
					goto ShortCut;

				case '&':	// URL processing
					escflg = esc_url;
					escflags = 0;
					break;

				default:
					goto ShortCut;
			} /* switch */
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_csi && ctr > 0 ) {

			/* "control sequence" processing */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				  /* parse numeric parameter */
					if (VSIw->parms[VSIw->parmptr] < 0)
						VSIw->parms[VSIw->parmptr] = 0;
					VSIw->parms[VSIw->parmptr] = VSIw->parms[VSIw->parmptr] * 10;
					VSIw->parms[VSIw->parmptr] += *c - '0';
					break;

				case '?':
				  /* DEC-private control sequence */
					VSIw->parms[VSIw->parmptr++] = -2;
					break;

				case ';':
				  /* parameter separator */
					VSIw->parmptr++;
					break;

				case '"':
					/* start Compatibility Level (DECSCL) */
		            escflg = esc_descl;
		            break;

				case 'A': /* cursor up */
					sx = VSIw->parms[0];
					if (sx < 1) sx = 1;
					VSIw->y -= sx;
					if (VSIw->y < 0) VSIw->y = 0;			/* BYU */
					if (VSIw->y < VSIw->top)				/* NCSA: SB */
						VSIw->y = VSIw->top;				/* NCSA: SB */
					VSIrange();
					VSIflush(); // RAB BetterTelnet 2.0b3
					goto ShortCut;

				case 'X':
				 /* Erases characters at the cursor position and the next n-1 characters */
				 /* A parameter of 0 or 1 causes a single character to be erased */
				 /* Character attributes are set to normal */
				 /* The cursor remains in the same position */
					if ( VSIw->parms[0] == 0 )
						VSIw->parms[0] = 1;
					savedX = VSIw->x;
					savedY = VSIw->y;
					savedChar = VSIw->lastchar;
					VSIw->lastchar = ' ';
					/* leave current attribute as-is for linux ??? */
					attrib = VSIw->attrib;
					if (VSIw->vtemulation != 3)
      					VSIw->attrib &= (kVSansi2b | kVSgrph); // all off, keep multi-byte / graphic
      				// FALL-THROUGH
				case 'b':	/* repeat last char n times ? */
					while ( VSIw->parms[0] > 0 ) {
						int len = VSIw->parms[0];
						unsigned char repeatbuf[256];
						short savecaptureRN;

						if ( len > sizeof(repeatbuf) )
							len = sizeof(repeatbuf);
						VSIw->parms[0] -= len;
						for (i = 0; i < len; ++i)
							repeatbuf[i] = VSIw->lastchar;
						VSIw->escflg = esc_root;
						/* disable capture of already captured data */
						if ( TelInfo->debug ) {
							savecaptureRN = VSscreens[VSIwn].captureRN;
							VSscreens[VSIwn].captureRN = 0;
						}
						VSem(repeatbuf, len);
						if ( TelInfo->debug ) {
							VSscreens[VSIwn].captureRN = savecaptureRN;
						}
					}
					if (*c == 'X') {
						VSIw->x = savedX;
						VSIw->y = savedY;
						VSIw->lastchar = savedChar;
      					VSIw->attrib = attrib;
					}
					goto ShortCut;

				case 'B': /* cursor down */
					sx = VSIw->parms[0];
					if (sx < 1) sx = 1;
					VSIw->y += sx;
					if (VSIw->y > VSIw->bottom)				/* NCSA: SB */
						VSIw->y = VSIw->bottom;				/* NCSA: SB */
					VSIrange();
					VSIflush(); // RAB BetterTelnet 2.0b3
					goto ShortCut;

				case 'C': /* cursor right */
					sx = VSIw->parms[0];
					if (sx < 1) sx = 1;
					if (VSIw->DECAWM && VSIw->x >= mw) {
						// autowrap
						if (VSIw->y >= VSIw->lines)
							goto ShortCut;
						VSIw->y++;
						VSIw->x = (VSIw->x > mw) ? 0 : -1;
					}
					VSIw->x += sx;
					VSIrange();
					if (VSIw->x > mw)
						VSIw->x = mw;
					goto ShortCut;

				case 'c':
					if ( VSIw->parms[0] == -2 ) {	/* not the same meaning !!! */
						/* set cursor_type */
						val = CUR_DEF;
						if ( VSIw->parms[1] >= 0 ) {
							val = VSIw->parms[1];
							if ( VSIw->parms[2] > 0 ) {
								val = VSIw->parms[2]<<8;
								if ( VSIw->parms[3] > 0 )
									val = VSIw->parms[3]<<16;
							}
						}
						switch ( val ) {
							case CUR_DEF:			val = gApplicationPrefs->CursorType; break;
							case CUR_NONE:			val = NOCURSOR; break;
							case CUR_UNDERLINE:		val = UNDERSCORECURSOR; break;
							case CUR_LOWER_THIRD:	val = LOWER_THIRDCURSOR; break;
							case CUR_LOWER_HALF:	val = LOWER_HALFCURSOR; break;
							case CUR_TWO_THIRDS:	val = TWO_THIRDSCURSOR; break;
							case CUR_BLOCK:			val = BLOCKCURSOR; break;
							//case CUR_HWMASK:
							//case CUR_SWMASK:
							default: val = -1;
						}
						if ( val >= 0 ) {
							RSsetcurstype(VSIwn, val);
						}
					} else {
						if (!(escflags & 1)) {
							VTsendident();
						} else {
							VTsendsecondaryident();
						}
					}
					goto ShortCut;

				case 'd':	/* Y cursor position ? */
					VSIw->y = VSIw->parms[0] - 1;
					if (VSIw->DECAWM && VSIw->x > mw)
						VSIw->x = 0;
					if (VSIw->y < 0)
						VSIw->y = 0;
					if (VSIw->y > VSIw->lines)
						VSIw->y = VSIw->lines;
					goto ShortCut;

				case 'D': /* cursor left */
					sx = VSIw->parms[0];
					if (sx < 1) sx = 1;
					if (VSIw->DECAWM && VSIw->x <= 0) {
						// autowrap
						if (VSIw->y <= 0)
							goto ShortCut;
						VSIw->y--;
						VSIw->x = mw + 1;
					}
					VSIw->x -= sx;
					VSIrange();
					goto ShortCut;

				case 'G':	/* X cursor position ? */
					if (VSIw->DECAWM && VSIw->x > mw) {
						// autowrap
						if (VSIw->y >= VSIw->lines)
							goto ShortCut;
						VSIw->y++;
					}
					VSIw->x = VSIw->parms[0] - 1;
					if (VSIw->x < 0)
						VSIw->x = 0;
					if (VSIw->x > mw)
						VSIw->x = mw;
					goto ShortCut;

				case 'g':
					if (VSIw->parms[0] == 3)
					  /* clear all tabs */
						VSItabclear();
					else if (VSIw->parms[0] <= 0)
					  /* clear tab at current position */
						VSIw->tabs[VSIw->x] = ' ';
					goto ShortCut;

				case 'h':
					/* set options */
					VSIsetoption(1);
					goto ShortCut;

				case 'f':
				case 'H':
				  /* absolute cursor positioning */
					VSIw->x = VSIw->parms[1] - 1;
					if (VSIw->DECORG)
					  /* origin mode -- position relative to top of scrolling region */
						VSIw->y = VSIw->parms[0] - 1 + VSIw->top;
					else
						VSIw->y = VSIw->parms[0] - 1;
					/*	Don't use actual VSIrange 'cause it will wrap us to first column if 
						we are past screen edge.  This causes "resize" to break */
					if (VSIw->x < 0)						/* JMB 2.6 */
						VSIw->x = 0;						/* JMB 2.6 */
					if (VSIw->x > mw)			/* JMB 2.6 */
						VSIw->x = mw;			/* JMB 2.6 */
					if (VSIw->y < 0)						/* JMB 2.6 */
						VSIw->y = 0;						/* JMB 2.6 */
					if (VSIw->y > VSIw->lines)				/* JMB 2.6 */
						VSIw->y = VSIw->lines;				/* JMB 2.6 */
					if ((VSIw->forcesave)&&(VSIw->y == 0)&&(VSIw->x == 0)) //CCP better FORCESAVE
						VSIw->possibleForce = TRUE;

					if (VSIw->y != VSIw->bottom) VSIflush(); // RAB BetterTelnet 2.0b3
					goto ShortCut;

		        case 'i':											/* PR: media copy */
					if (VSIw->parms[VSIw->parmptr]==5) {			/* PR */
						/*++c; --ctr; */							/* PR */			
						VSprON();	/* PR - set status and open temp file etc */
									/* PR - chars will be redirected at top of loop É */
									/* PR - É in this procedure */
					}				/* PR */
		            escflg = esc_root;		/* PR */
		            break;			/* PR */

				case 'J':
					/* erase to beginning/end/whole of screen */
					switch ( VSIw->parms[0] ) {
						case -1:
						case  0:
							VSIeeos();
							break;
						case  1:
							VSIebos();
							break;
						case  2:
							VSIes();
							break;
					}
					goto ShortCut;

				case 'K':
					/* erase to beginning/end/whole of line */
					switch ( VSIw->parms[0] ) {
						case -1:
						case  0:
							VSIeeol();
							break;
						case  1:
							VSIebol();
							break;
						case  2:
							VSIel(-1);
							break;
					}
					goto ShortCut;

				case 'L':
					if (VSIw->parms[0] < 1)
						VSIw->parms[0] = 1;
					VSIinslines(VSIw->parms[0], -1);
					goto ShortCut;

				case 'l':
					/* reset options */
					VSIsetoption(0);
					goto ShortCut;

				case 'M':
					VSIflush(); // RAB BetterTelnet 2.0b3
					if (VSIw->parms[0] < 1)
						VSIw->parms[0] = 1;
					VSIdellines(VSIw->parms[0], -1);
					goto ShortCut;

				case 'm':
					if (VSIw->parms[0] == -2) {
						/* clear selection => unused */
						goto ShortCut;
					} else {
						/* set/clear attributes */
						short temp = 0;

						if (VSIw->parms[VSIw->parmptr] < 0)
							VSIw->parms[VSIw->parmptr] = 0;

						while (temp <= VSIw->parmptr) {
							short p = VSIw->parms[temp];
	      					if (p == 0) {
	      						VSIw->attrib &= kVSansi2b | kVSgrph; // all off
	      					} else if (p > 0 && p < 8 ) {
	      						VSIw->attrib |= VSa(p); // set an attribute
	      					} else if (p >= 10 && p < 20 ) {
								// 10 : Primary (default) font
								// 11..19 : alternate fonts
	      					} else if (p > 20 && p < 28) {
								VSIw->attrib &= ~VSa(p - 20); // clear an attribute
	              			} else if (tw && tw->ANSIgraphics) {
	 							if (p >= 30 && p < 38) {
									VSIw->attrib = VSansifg((VSIw->attrib & 0xfffcf0ff) | ((p- 30)<< 8));
								} else if (p >= 38 && p < 40) { // Turn off foreground color
									VSIw->attrib = VSIw->attrib & 0xfffef0ff;
								} else if (p >= 40 && p < 48) {
									VSIw->attrib = VSansibg((VSIw->attrib & 0xfffc0fff) | ((p- 40)<<12));
								} else if (p >= 48 && p < 50) { // Turn off background color
									VSIw->attrib = VSIw->attrib & 0xfffd0fff;
	 							} else if (p >= 90 && p < 98) {
									VSIw->attrib = VSansifg2((VSIw->attrib & 0xfffcf0ff) | ((p- 90)<< 8));
								} else if (p >= 100 && p < 108) {
									VSIw->attrib = VSansibg2((VSIw->attrib & 0xfffc0fff) | ((p-100)<<12));
								}
							}
							temp++;
						}
					}
					goto ShortCut;

				case 'n':
					if ( VSIw->parms[0] == -2 ) {
						switch ( VSIw->parms[1] ) {
							case 15:	/* (request for printer status) */
								VTsendprintstat();
								break;
							case 25:	/* request for UDK (use defined keys) status */
								VTsendudkstat();
								break;
						}
					} else {
						switch (VSIw->parms[0]) {
							case 5:
								VTsendstat();
								break;
							case 6:
								VTsendpos();
								break;
						}
					}
					goto ShortCut;

				case 'P':
					if (VSIw->parms[0] < 1)
						VSIw->parms[0] = 1;
					VSIdelchars(VSIw->parms[0]);
					goto ShortCut;

				case 'q':
				  /* flash dem LEDs. What LEDs? */
					goto ShortCut;

				case 'r':
					if ((VSIw->parms[0] - 1 == VSIw->top) && (VSIw->parms[1] - 1 == VSIw->bottom))
						goto ShortCut; // RAB BetterTelnet 2.0b3
					VSIflush(); // RAB BetterTelnet 2.0b3
				  /* set scrolling region */
					if (VSIw->parms[0] < 0)
						VSIw->top = 0;
					else
						VSIw->top = VSIw->parms[0] - 1;
					if (VSIw->parms[1] < 0)
						VSIw->bottom = VSIw->lines;
					else
						VSIw->bottom = VSIw->parms[1] - 1;
					if (VSIw->top < 0)
						VSIw->top = 0;
					if (VSIw->top > VSIw->lines - 1)
						VSIw->top = VSIw->lines - 1;
					if (VSIw->bottom < 1)
						VSIw->bottom = VSIw->lines;
					if (VSIw->bottom > VSIw->lines)
						VSIw->bottom = VSIw->lines;

					if (VSIw->top >= VSIw->bottom) {
						if (VSIw->bottom >=1)
							VSIw->top = VSIw->bottom -1;
						else VSIw->bottom = VSIw->top +1;
					}
 				
 					VSIw->x = 0;
					VSIw->y = 0;
					if (VSIw->DECORG)
						VSIw->y = VSIw->top;	/* origin mode relative */
					goto ShortCut;

				case 'S':	/* what is this one ? */
					goto ShortCut;

				case '>':
					/* maybe secondary ident request */
				  	escflags |= 1;
					break;

				case '@':
					if (VSIw->parms[0] < 1)
						VSIw->parms[0] = 1;
					VSIinschar(VSIw->parms[0], 1);
					VSredrawLine(VSIwn); //redraws current line
					goto ShortCut;
				case '!':						/* BYU 2.4.12 - More private DEC stuff? */
				case '\'':						/* BYU 2.4.12 - More private DEC stuff? */
					escflg = esc_decl;
					break;
				default:
//					VSprintf( "unknown csi : 0x%x\n", *c );
					goto ShortCut;
			} /* switch */
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_decl && ctr > 0 ) {
			/* "#" handling */
			short lattrib = -1;
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case '3':	// Double Height Line (DECDHL) Top Half - applies to the whole line
					lattrib = kVSdecdhlt;
					break;
				case '4':	// Double Height Line (DECDHL) Bottom Half - applies to the whole line
					lattrib = kVSdecdhlb;
					break;
				case '5':	// Single-Width Line (DECSWL) - applies to the whole line
					lattrib = 0;
					break;
				case '6':	// Double-Width Line (DECDWL) - applies to the whole line
					// unsupported yet...
					lattrib = kVSdecdhlt | kVSdecdhlb;
					break;
				case '8':	// alignment display
					VTalign();
					goto ShortCut;
				default:
					goto ShortCut;
			} /* switch */
			if ( lattrib != -1 ) {
				VSIw->lattrib &= ~(kVSdecdhlt | kVSdecdhlb);
				VSIw->lattrib |= lattrib;
				VSIw->linest[VSIw->y]->lattr = VSIw->lattrib;
				// refresh line with new width
				VSredrawLine(VSIwn);
				goto ShortCut;
			}
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_descl && ctr > 0 ) {
			/* '"' handling */
			short lattrib = -1;
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case 'p':
					/* set the compatibility level of the terminal */
					if ( VSIw->parms[0] == 61 ) {
						// Set terminal for level 1 compatibility (VT100 mode)
						VSIw->vtemulation = 0;
					} else if ( VSIw->parms[0] == 62 ) {
						if ( VSIw->parms[1] == -1 || VSIw->parms[1] == 0 || VSIw->parms[1] == 2 ) {
							// Set terminal for level 2 compatibility (VT200 mode, 8-bit controls).
							VSIw->vtemulation = 1;
							VSIw->vteightbits = 1;
						} else if ( VSIw->parms[1] == 1 ) {
							// Set terminal for level 2 compatibility (VT200 mode, 7-bit controls).
							VSIw->vtemulation = 1;
							VSIw->vteightbits = 0;
						}
					}
					goto ShortCut;
				default:
					goto ShortCut;
			} /* switch */
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_G0 && ctr > 0 ) {
			/* "(" handling (selection of G0 character set) single byte */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case ' ':
				case '@':
					goto ShortCut;
				case 'A': /* UK */	// FIXME: should we use the same translation as ASCII ?
				case 'B': /* US - ASCII */
				case '1': /* "soft" */
					VSIw->G0 = 0;
					if (!VSIw->charset) {
						VSIw->attrib = VSnotgraph(VSIw->attrib);
					}
					if (tw && GetTranslationIndex(tw->innational) < 0)
						switchintranslation(tw, kTRJIS, kASCII);
					goto ShortCut;
				case '0': /* DEC special graphics */
				case '2': /* "soft" */
					VSIw->G0 = 1;
					if (!VSIw->charset) {
						VSIw->attrib = VSgraph(VSIw->attrib);
					}
					goto ShortCut;
				case 'J': /* JIS X 0201-Roman */
					// The "Roman" character set of JIS X 0201 [JISX0201] is identical to
					// ASCII except for backslash () and tilde (~). The backslash is
					// replaced by the Yen sign, and the tilde is replaced by overline. This
					// set is Japan's national variant of ISO 646 [ISO646].
					if (tw && GetTranslationIndex(tw->innational) < 0)
						switchintranslation(tw, kTRJIS, kJISX0201_1976);
					goto ShortCut;
				case 'I': /* Not Std ISO-2022-JP */
					if (tw && GetTranslationIndex(tw->innational) < 0) {
						//switchintranslation(tw, kTRJIS, kJISX0201_1976Kana);
						switchintranslation(tw, kTRJISX0201_76kana, 0);
					}
					goto ShortCut;
				default:
					goto ShortCut;
			} /* switch */
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_G1 && ctr > 0 ) {
			/* ")" handling (selection of G1 character set) */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case 'A': /* UK */
				case 'B': /* US */
				case '1': /* "soft" */
					VSIw->G1 = 0;
					if (VSIw->charset) {
						VSIw->attrib = VSnotgraph(VSIw->attrib);
					}
					goto ShortCut;
				case '0': /* DEC special graphics */
				case '2': /* "soft" */
					VSIw->G1 = 1;
					if (VSIw->charset) {
						VSIw->attrib = VSgraph(VSIw->attrib);
					}
					goto ShortCut;
				default:
					goto ShortCut;
			} /* switch */
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_G2 && ctr > 0 ) {
			/* "*" handling (selection of G2 character set) */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				default:
					// FIXME...
					goto ShortCut;
			}
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_G3 && ctr > 0 ) {
			/* "+" handling (selection of G3 character set) */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				default:
					// FIXME...
					goto ShortCut;
			}
			++c;
			--ctr;
		} /* while */

		while ( escflg == esc_G0j && ctr > 0 ) {
			/* "$" handling (selection of G0 character set) multibytes */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;

				case '@': /* JIS X 0208-1978 */
					// The JIS X 0208 [JISX0208] character sets consist of Kanji, Hiragana,
					// Katakana and some other symbols and characters. Each character takes
					// up two bytes.
					if ( escflags == 0 ) {
						/* set G0 character set to this one... */
						VSIw->G0 = 0;
						if (!VSIw->charset) {
							VSIw->attrib = VSnotgraph(VSIw->attrib);
						}
						if (tw && GetTranslationIndex(tw->innational) < 0)
							switchintranslation(tw, kTRJIS, kJISX0208_1978);
					}
					goto ShortCut;

				case 'B': /* JIS X 0208-1983 */
					if ( escflags == 0 ) {
						/* set G0 character set to this one... */
						VSIw->G0 = 0;
						if (!VSIw->charset) {
							VSIw->attrib = VSnotgraph(VSIw->attrib);
						}
						if (tw && GetTranslationIndex(tw->innational) < 0)
							switchintranslation(tw, kTRJIS, kJISX0208_1983);
					}
					goto ShortCut;

				case 'A': /* GB2312-1980 */
					if ( escflags == 0 ) {
						/* set G0 character set to this one... */
						VSIw->G0 = 0;
						if (!VSIw->charset) {
							VSIw->attrib = VSnotgraph(VSIw->attrib);
						}
						// FIXME
						//if (tw && GetTranslationIndex(tw->innational) < 0)
							//switchintranslation(tw, ISO2022CN, kxxxxxxxxxxx);
					}
					goto ShortCut;

				case '(':
					if ( escflags == 0 ) {
						escflags = 1;
						break;
					}
					goto ShortCut;

				case 'C': /* KSC5601-1987 */
					goto ShortCut;

				case 'D': /* JIS X 0212-1990 */
					if ( escflags == 1 ) {
						/* set G0 character set to this one... */
						VSIw->G0 = 0;
						if (!VSIw->charset) {
							VSIw->attrib = VSnotgraph(VSIw->attrib);
						}
						if (tw && GetTranslationIndex(tw->innational) < 0)
							//switchintranslation(tw, kTRJIS, kJISX0212_1990);
							switchintranslation(tw, kTRJISX0212_1990, 0);
					}
					goto ShortCut;

				default:
					goto ShortCut;
			} /* switch */
			++c;
			--ctr;
		} /* while */
	
		while ( escflg == esc_G2j && ctr > 0 ) {
			/* "." handling (selection of G2 character set) */
			switch (*c) {
				case 0x08:
					if (VSIw->x > 0)
						--VSIw->x;
					break;
				case 'A': /* ISO8859-1 */
				case 'F': /* ISO8859-7(Greek) */
					// FIXME...
					//VSIw->G2 = 0;
					if (VSIw->charset) {
						VSIw->attrib = VSnotgraph(VSIw->attrib);
					}
					goto ShortCut;
				default:
					goto ShortCut;
			}
			++c;
			--ctr;
		} /* while */

        // Handle XTerm rename functions, code contributed by Bill Rausch
        // Modified by JMB to handle ESC]2; case as well.
		if ( escflg == esc_XTerm && ctr > 0 ) {
			if ( escflags == 0 ) {
				if ( *c == '0' || *c == '2') {
					escflags = 1;
	            	++c;
	            	--ctr;
				} else if ( *c == 'P') { /* palette escape sequence */
					escflags = 3;
	            	++c;
	            	--ctr;
				} else if ( *c == 'R') { /* reset palette */
					RSresetcolors( VSIwn );
					goto ShortCut;
				} else {
					goto ShortCut;
				}
			}
			if ( escflags == 1 && ctr > 0 && *c == ';' ) {
				++c;
				--ctr;
				escflags = 2;
				*VSIw->tempstr = 0;
			}
			while ( escflags == 2 && ctr > 0 && *c != 7 && *c != 033) {
	        	if (*VSIw->tempstr < sizeof(VSIw->tempstr) - 1) {
	        		VSIw->tempstr[++(*VSIw->tempstr)] = *c;
	            }
	            ++c;
	            --ctr;
			}
	        if ( escflags == 2 && ctr > 0 && (*c == 7 || *c == 033) ) {
	        	set_new_window_name( VSIw->tempstr, RSgetwindow(VSIwn) );
	           	if (*c != 07) {
					/* This will be undone in the ShortCut below. */
	            	c--;
	                ctr++;
				}
	            goto ShortCut;
			}
			if ( escflags == 3 && ctr > 0 ) {
				if ( (*c>='0'&&*c<='9') || (*c>='A'&&*c<='F') || (*c>='a'&&*c<='f') ) {
					VSIw->parms[VSIw->parmptr++] = (*c>'9' ? (*c&0xDF)-'A'+10 : *c-'0');
					if ( VSIw->parmptr == 7 ) {
						RGBColor newColor;
						newColor.red   = 16 * VSIw->parms[1] + VSIw->parms[2];
						newColor.green = 16 * VSIw->parms[3] + VSIw->parms[4];
						newColor.blue  = 16 * VSIw->parms[5] + VSIw->parms[6];
						RSsetcolors(VSIwn, VSIw->parms[0] + 4, &newColor);
	            		goto ShortCut;
					}
				} else {
	            	goto ShortCut;
	            }
			}
		}

		while ( escflg == esc_url && ctr > 0 ) {
			if ( (escflags == 0 && *c == '&')
			  || (escflags == 1 && *c == 'B')
			  || (escflags == 2 && *c == 'u')
			  || (escflags == 3 && *c == 'r')
			  || (escflags == 4 && *c == 'l') ) {
				++escflags;
            	if ( escflags == 5 ) {
					*VSIw->tempstr = 0;
            	}
			} else if ( escflags == 5 ) {
				if ( *c == 0x0d || *c == 0x0a ) {
					// completed. process url.
					Boolean result = (tw && tw->launchurlesc) ? HandleURLString(VSIw->tempstr) : false;
					RSsendstring( VSIwn, result ? "S" : "F", 1 );
					escflg = 0;
				} else if (*VSIw->tempstr < sizeof(VSIw->tempstr) - 1) {
	        		VSIw->tempstr[++(*VSIw->tempstr)] = *c;
	            } else {
	            	// too big...
            		goto ShortCut;
	            }
			} else {
            	goto ShortCut;
			}
			++c;
			--ctr;
		}

		if ( escflg > esc_csi && ctr > 0 ) {
ShortCut:						/* BYU 2.4.12 - well, sacrificing style for speed */
			if ( VSIw->possibleForce && *c != 'H' ) //CCP better forcesave
				VSIw->possibleForce = FALSE;
			escflg = 0;
			escflags = 0;
			++c;
			--ctr;
		}

	} /* while (ctr > 0) */
    VSIw->escflg = escflg;
    VSIw->escflags = escflags;
} /* VSem */

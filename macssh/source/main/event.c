// event.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
*    Main Event loop code for NCSA Telnet for the Macintosh
*
*	Called by:
*		maclook.c
*
*	Revisions:
*	7/92	Telnet2.6:	added 2 support for 2 global structs, put cursors into
*						an array, cleaned up defines			Scott Bulmahn						
*
*/

/*
 *	Files for inclusion.
 */


#include "VSkeys.h"
#include "wind.h"
#include "translate.h"
#include "event.h" 	//kxplate moved to its own file (CCP 2.7)
#include "errors.proto.h"
#include "network.proto.h"
#include "mydnr.proto.h"
#include "bkgr.proto.h"
#include "maclook.proto.h"
#include "InternalEvents.h"
#include "vsdata.h"
#include "vsinterf.proto.h"
#include "vsintern.proto.h"
#include "menuseg.proto.h"
#include "vrrgmac.proto.h"
#include "tekrgmac.proto.h"
#include "rsinterf.proto.h"
#include "rsmac.proto.h"
#include "event.proto.h"
#include "macros.proto.h"				/* For setmacro proto */
#include "netevent.proto.h"
#include "translate.proto.h"
#include "parse.proto.h"
#include "linemode.proto.h"
#include "Connections.proto.h"
#include "mainseg.proto.h"
#include "movableModal.h"
#include <Script.h> // for EMACS meta hack
#include <Icons.h> //For Notify User Icon Stuff

#include <Gestalt.h>
#include <OSUtils.h>
#include <Traps.h>
#include <LowMem.h>


extern short		scrn;
extern MenuHandle	myMenus[NMENUS];
extern Boolean		gKeyboardHasControlKey;
extern SysEnvRec	theWorld;		/* BYU 2.4.12 - System Environment record */
extern WindRec		*screens,
					*ftplog;
extern Cursor		*theCursors[];
extern Boolean 		gHaveDragMgr;

extern void VSprintf(char *fmt, ...);
extern void VSdump(char *p, int len);

static gHaveInstalledNotification = 0;
NMRec *nRecPtr;

OSType	gKeyboardLayoutType = kKeyboardUnknown;

extern int gMovableModal;

#include "event.proto.h"

extern void syslog( int priority, const char *format, ...);
extern long dumpln( long base, char *dest, void *src, long len );


Boolean IsKBGetLayoutTypeAvailable(void);

OSType GetKeyboardLayoutType(short deviceID);


// BetterTelnet 2.0fc1 - integrated DJ's changes for real blink (if you want it :-)
/* DJ: Blink global */
short				gBlink = 0;
// In ticks, how long between toggling the above?
#define BLINK_PERIOD	30
// RAB - DON'T extern shared routines, and DON'T define them outside of proto.h's!
//extern void VSRedrawAll(void);
//extern void VSPulseAll(void);
/* End blink */

#define switchEvt	 1		/* Switching event (suspend/resume )  for app4evt */

#define	BScode		51		/* This is the Keycode for Backspace */

//This is stuff to translate key codes to VS codes.  There are two tables, for shifted
//and unshifted.  The base is 0x41, the code for the keypad period.  The max is 0x7e, the
//code for the up arrow.  This includes everything but the main portion of the keyboard.

#define KPlowest	65		/* This is the last code before the KP */

unsigned char kpxlate[2][62] =
  {
	  {		/* virtual key code */
		VSKP,	/* $41 */					/* Keypad . */
		VSRT,	/* $42 (Mac+) */			/* Right Arrow */
		VSPF4,	/* $43 (ADB) */				/* Mac Keypad * */
		0,		/* $44 */
		VSKC,	/* $45 (ADB std) */			/* Keypad , (+) */
		VSLT,	/* $46 (Mac+) */			/* Left Arrow */
		VSPF1,	/* $47 */					/* Mac Keypad Clear */
		VSDN,	/* $48 (Mac+) */
		0,		/* $49 */
		0,		/* $4A */
		VSPF3,	/* $4B (ADB) */
		VSKE,	/* $4C */
		VSUP,	/* $4D (Mac+) */
		VSKM,	/* $4E */					/* Keypad - */
		0,		/* $4F */
		0,		/* $50 */
		VSPF2,	/* $51 (ADB) */
		VSK0,	/* $52 */
		VSK1,	/* $53 */
		VSK2,	/* $54 */
		VSK3,	/* $55 */
		VSK4,	/* $56 */
		VSK5,	/* $57 */
		VSK6,	/* $58 */
		VSK7,	/* $59 */
		0,		/* $5A */
		VSK8,	/* $5B */
		VSK9,	/* $5C */
		VSJPBKS,/* $5D */	/* '\' on japanese keyboard */
		VSJPUND,/* $5E */	/* '_' on japanese keyboard */
		VSF5,	/* $5F */
		VSF10,	/* $60 */	/* BYU 2.4.12 */
		VSF11,	/* $61 */	/* BYU 2.4.12 */
		VSF12,	/* $62 */	/* BYU 2.4.12 */
		VSF8,	/* $63 */	/* BYU 2.4.12 - was VSF3 */
		VSF13,	/* $64 */	/* BYU 2.4.12 */
		VSF14,	/* $65 */	/* BYU 2.4.12 */
		0,		/* $66 */
		VSF16,	/* $67 */	/* BYU 2.4.12 */
		0,		/* $68 */
		VSF18,	/* $69 */	/* BYU 2.4.12 */
		0,		/* $6A */
		VSF19,	/* $6B */	/* BYU 2.4.12 */
		0,		/* $6C */
		VSF15,	/* $6D */	/* BYU 2.4.12 */
		0,		/* $6E */
		VSF17,	/* $6F */	/* BYU 2.4.12 */
		0,		/* $70 */
		VSF20,	/* $71 */	/* BYU 2.4.12 */
		VSHELP,	/* $72 */	/* BYU 2.4.12 */
		VSHOME,	/* $73 */	/* BYU 2.4.12 */
		VSPGUP,	/* $74 */	/* BYU 2.4.12 */
		VSDEL,	/* $75 (ADB ext) */		/* BYU 2.4.12 - was 0x7f */
		VSF9,	/* $76 */	/* BYU 2.4.12 - was VSF4 */
		VSEND,	/* $77 */	/* BYU 2.4.12 */
		VSF7,	/* $78 */	/* BYU 2.4.12 - was VSF2 */
		VSPGDN,	/* $79 */	/* BYU 2.4.12 */
		VSF6,	/* $7A */	/* BYU 2.4.12 - was VSF1 */
		VSLT,	/* $7B */
		VSRT,	/* $7C */
		VSDN,	/* $7D */
		VSUP	/* $7E */
	  },
	  {
		VSKP,	/* $41 */
		VSPF4,	/* $42 (Mac+) */
		VSPF4,		/* $43 (ADB) */
		0,		/* $44 */
		VSKC,		/* $45 (ADB) */
		VSKC,	/* $46 (Mac+) */
		VSPF1,	/* $47 */
		VSPF2,	/* $48 */
		0,		/* $49 */
		0,		/* $4A */
		VSPF3,		/* $4B */
		VSKE,	/* $4C */
		VSPF3,	/* $4D */
		VSKM,		/* $4E */
		0,		/* $4F */
		0,		/* $50 */
		VSPF2,		/* $51 */
		VSK0,	/* $52 */
		VSK1,	/* $53 */
		VSK2,	/* $54 */
		VSK3,	/* $55 */
		VSK4,	/* $56 */
		VSK5,	/* $57 */
		VSK6,	/* $58 */
		VSK7,	/* $59 */
		0,		/* $5A */
		VSK8,	/* $5B */
		VSK9,	/* $5C */
		VSJPBKS,/* $5D */	/* '\' on japanese keyboard */
		VSJPUND,/* $5E */	/* '_' on japanese keyboard */
		VSF5,		/* $5F */
		VSF10,	/* $60 */	/* BYU 2.4.12 */
		VSF11,	/* $61 */	/* BYU 2.4.12 */
		VSF12,	/* $62 */	/* BYU 2.4.12 */
		VSF8,	/* $63 */	/* BYU 2.4.12 - was VSF3 */
		VSF13,	/* $64 */	/* BYU 2.4.12 */
		VSF14,	/* $65 */	/* BYU 2.4.12 */
		0,		/* $66 */
		VSF16,	/* $67 */	/* BYU 2.4.12 */
		0,		/* $68 */
		VSF18,	/* $69 */	/* BYU 2.4.12 */
		0,		/* $6A */
		VSF19,	/* $6B */	/* BYU 2.4.12 */
		0,		/* $6C */
		VSF15,	/* $6D */	/* BYU 2.4.12 */
		0,		/* $6E */
		VSF17,	/* $6F */	/* BYU 2.4.12 */
		0,		/* $70 */
		VSF20,	/* $71 */	/* BYU 2.4.12 */
		VSHELP,	/* $72 */	/* BYU 2.4.12 */
		VSHOME,	/* $73 */	/* BYU 2.4.12 */
		VSPGUP,	/* $74 */	/* BYU 2.4.12 */
		VSDEL,	/* $75 (ADB ext) */		/* BYU 2.4.12 - was 0x7f */
		VSF9,	/* $76 */	/* BYU 2.4.12 - was VSF4 */
		VSEND,	/* $77 */	/* BYU 2.4.12 */
		VSF7,	/* $78 */	/* BYU 2.4.12 - was VSF2 */
		VSPGDN,	/* $79 */	/* BYU 2.4.12 */
		VSF6,	/* $7A */	/* BYU 2.4.12 - was VSF1 */
		VSLT,	/* $7B */
		VSRT,	/* $7C */
		VSDN,	/* $7D */
		VSUP	/* $7E */
	  }
  };


void setLastCursor(Cursor *crsr)
{
	if ( TelInfo->lastCursor != crsr ) {
		SetCursor(TelInfo->lastCursor = crsr);
	}
}

short updateCursor(short force)
{
	static Point	lastPoint;
	static short	optwasdown;
	short			optDown;
	Point			myPoint;
	KeyMap			allthekeys;			/* Someplace to put the keymap */
	short			vs;

	if ( !TelInfo->myfrontwindow ) {
		setLastCursor(theCursors[normcurs]);
		return 0;
	}

	/* GetMouse returns local coordinates */
	SetPort( TelInfo->myfrontwindow );
	GetMouse(&myPoint);

	GetKeys(allthekeys);
	optDown = ((unsigned char *)allthekeys)[7] & 4;

	if ( !force && EqualPt(myPoint,lastPoint) && optDown == optwasdown)
		return 0;

	if (force)
		TelInfo->lastCursor = 0L;

	if (TelInfo->ginon) {
		setLastCursor(theCursors[gincurs]);
		return 1;
	}

	if (TelInfo->xferon && !optDown) {
		setLastCursor(theCursors[xfercurs]);
		return 1;
	}

	vs = TelInfo->myfrontvs;

	switch (TelInfo->myfronttype) {
		case DEC_WINDOW:
			if ( RSmouseintext(vs, myPoint) ) {
				if ( gHaveDragMgr && RSTextSelected(vs)
				  && PointInSelection(normalize(myPoint, vs, FALSE), vs) ) {
					// for Drag ability
					setLastCursor(theCursors[normcurs]);
				} else {
					if (optDown) {
						setLastCursor(theCursors[poscurs]);
					} else {
						setLastCursor(theCursors[textcurs]);
					}
				}
			} else {
				setLastCursor(theCursors[normcurs]);
			}
			break;
		case TEK_WINDOW:
			LocalToGlobal(&myPoint);
			if ( PtInRgn(myPoint, TelInfo->myfrontwindow->contRgn) ) {		/* BYU LSC */
				setLastCursor(theCursors[graphcurs]);
			} else {
				setLastCursor(theCursors[normcurs]);
			}
			break;
		case NO_WINDOW:
		default:
			setLastCursor(theCursors[normcurs]);
	}
	lastPoint = myPoint;
	optwasdown = optDown;
	return 0;
}

void NoWindow( void)
{
	if (gMovableModal) {
		return;
	}

	TelInfo->myfrontwindow=0L;
	TelInfo->myfronttype=NO_WINDOW;
	TelInfo->myfrontRgn=0L;
	updateCursor(0);
}

/* 	The following code was graciously donated by Marc Tamsky.  When are YOU going to donate
	YOUR code, eh?  We know you're reading this.  -- JMB */

Boolean CheckPageKeys(short code)											/* NCSA: SB */
{																			/* NCSA: SB */
	GrafPtr currFW;     // current front window holder              		// MAT--
	short     ourW;       // virtual screen number holder             		// MAT--kinda
	short     x1, y2, x2, y1; // coordinates from window              		// MAT--pulled from scrollproc
																			/* NCSA: SB */
	currFW = (GrafPtr)FrontWindow();                            			// MAT--
	ourW = RSfindvwind(currFW);                                 			// MAT--
																			/* NCSA: SB */	
	switch (code)															/* NCSA: SB */
		{																	/* NCSA: SB */
		case VSPGUP:														/* NCSA: SB */
			VScursblinkoff(ourW);
			VSgetrgn(ourW, &x1, &y1, &x2, &y2);                   			// MAT--
			VSscrolback(ourW, y2 - y1); /* scroll a whole windowful */  	// MAT--
			VScursblinkon(ourW);
			return TRUE;                                                   	// MAT--
			break;															/* NCSA: SB */															
																			/* NCSA: SB */
		case VSPGDN:                                                   		// MAT--121 is a PAGE DOWN.		                                                             	 
			VScursblinkoff(ourW);																// MAT--in rsmac.c
			VSgetrgn(ourW, &x1, &y1, &x2, &y2);                     		// MAT--
			VSscrolforward(ourW, y2 - y1); /* scroll a whole windowful */   // MAT--
			VScursblinkon(ourW);
			return TRUE;                                                    // MAT--
			break; 															/* NCSA: SB */
			                                                              	// MAT--
		case VSHOME:														/* NCSA: SB */
			VScursblinkoff(ourW);
			VSscroltop(ourW);												/* JMB 2.6 -- Created VSscroltop just for this purpose */
			VScursblinkon(ourW);
			return TRUE;													/* NCSA: SB */
			break;                                                     		// MAT--
		                                                               		// MAT--
		case VSEND:															/* NCSA: SB */
			VScursblinkoff(ourW);
			VSgetrgn(ourW, &x1, &y1, &x2, &y2);                     		// MAT--
			VSscrolforward(ourW, 32765); /* scroll a whole BUNCH! */    	// MAT-- kludge time again.  anyone suggest
			VScursblinkon(ourW);
			return TRUE;													/* NCSA: SB */
			break;                                                     		// MAT--  a better way to hack this part?
		}                                                               	// MAT--
																			/* NCSA: SB */
	return FALSE;			/* NCSA: SB - we didnt handle event, let us know about it */
}																			/* NCSA: SB */


/*
 * translatekeycode
 *
 * returns ascii code for input code, using the input modifiers.
 */

static unsigned char translatekeycode(unsigned char code, long modifiers)
{
	Ptr				KCHRPtr;
	unsigned long	state = 0;

	KCHRPtr = (Ptr)GetScriptManagerVariable(smKCHRCache);
	if ( KCHRPtr ) {
		return KeyTranslate(KCHRPtr, code | modifiers, &state);
	}
	return 0;
}

/*  HandleKeyDown --
		By now, we have already gotten a keypress signal from the event handler, so we
		just need to interpret it.  Get the	raw code and ascii value, and then decide
		what to do with it.	*/	

void HandleKeyDown(EventRecord theEvent,struct WindRec *tw)
{
	unsigned char	ascii, code;
	unsigned char	sendch;
	long			menuEquiv;
	short			enterkey = 0;
	Boolean			commanddown, optiondown, controldown,shifted;
	unsigned char	trbuf[32];
	unsigned char	*pbuf;
	long			inlen;
	long			trlen;
	int				res;
	int				i;
	
	ascii = theEvent.message & charCodeMask;
	code = ((theEvent.message & keyCodeMask) >> 8);
	commanddown = ((theEvent.modifiers & cmdKey) != 0);
	optiondown = ((theEvent.modifiers & optionKey) != 0);
	controldown = ((theEvent.modifiers & controlKey) != 0);
	shifted = ((theEvent.modifiers & shiftKey) != 0);

	if (DebugKeys(commanddown, ascii, tw->vs))
		return;

	ObscureCursor();

	if ( tw->emacsmeta == 2 && optiondown ) {
		// option key as emacs meta key: keep shift and control translation
		ascii = translatekeycode( code, (theEvent.modifiers & (shiftKey|controlKey)) );
		goto emacsHack;
	}


	if ( code == 0x34 && ascii == 3 )	// fix for PowerBook 540  bad KCHR
		ascii = 13; 					// (map control-c to return)
/*
	else if ( controldown && shifted && ascii == '2' )
		ascii = 0;						// fix bad KCHR control-@
	else if ( controldown && shifted && ascii == '6' )
		ascii = 0x1e;					// fix bad KCHR control-^
*/

	if ( controldown && !commanddown && gKeyboardLayoutType == kKeyboardJIS ) {
		if ( code == 0x5d ) {	// '\'
			ascii = 0x1c;
			code = 0;
		} else if ( code == 0x5e ) {	// '_'
			ascii = 0x1f;
			code = 0;
		} else if ( code == 0x21 )	// '@'
			ascii = 0;
		else if ( code == 0x1e )	// '['
			ascii = 0x1b;
		else if ( code == 0x2a )	// ']'
			ascii = 0x1d;
	}

	if ( commanddown ) {
		if ( gApplicationPrefs->CommandKeys || tw->emacsmeta == 3 ) {
			//if optioned, retranslate so we can do menu commands
			if ( tw->emacsmeta != 3 ) {
	 			if ( optiondown ) {
					ascii = translatekeycode( code, (theEvent.modifiers & shiftKey) );
				}
				menuEquiv = MenuKey(ascii); //handle menu keys first
				if ( (menuEquiv & 0xFFFF0000) != 0 ) {
					HandleMenuCommand(menuEquiv,theEvent.modifiers);
					return;
				}
			}
			if ( TelInfo->numwindows < 1 || tw->active != CNXN_ACTIVE ) 
				return;
			//	Check for EMACS meta key.  
			if ( tw->emacsmeta == 3 || (tw->emacsmeta == 1 && controldown) ) {
				if ( tw->emacsmeta != 3 ) {
					if ( ascii <= 32 ) {
						// control changed the ascii value
						ascii |= 0x40;	// move back to a non-control
					}
					if ( shifted || ascii == 0x5f ) {
						//so we can get meta -
						ascii = translatekeycode( code, (theEvent.modifiers & shiftKey) );
					}
				} else {
					ascii = translatekeycode( code, theEvent.modifiers & ~cmdKey );
				}
emacsHack:
				trbuf[0] = ESC;
				trbuf[1] = ascii;
				if ( (tw->clientflags & PASTE_IN_PROGRESS)
				  && (tw->pastemethod || tw->outlen > 32767) ) {
					// queue this
					kbwrite( tw, trbuf, 2);
				}
				kbflush( tw );
				if ( tw->echo && tw->halfdup ) 		
					parse( tw, trbuf, 2 );	
				netpush( tw->port );			
//				netwrite(tw->port,temp,2);
				// ascii is sent below
				netwrite( tw->port, trbuf, 1 ); // RAB BetterTelnet 2.0b4
				controldown = 0;
//				return;					RAB BetterTelnet 2.0b4 - deal with key below
			} else if ( ascii >= '0' && ascii <= '9' ) {
				// now look for macros
				kbflush( tw );
				sendmacro(tw, ascii - '0' + ((shifted) ? 10 : 0));
				return;
			} else if (!((ascii == '`' && gApplicationPrefs->RemapTilde) || code == BScode )) {
				// remap cmd-pgup/down
				CheckPageKeys(code);
				return;
			}
		} else {
			// no command key menus
			if ((TelInfo->numwindows < 1) || (tw->active != CNXN_ACTIVE)) 
				return;
// RAB BetterTelnet 2.0b4 - better remapping for old Macs
//			else if (!gKeyboardHasControlKey) //map command to control
//			{
/* NONO: the plain '`' char was disabled if CommandKeys is false and RemapTilde is true */
				/*ascii &= 0x1f;*/
				if ( ascii != '`' || !gApplicationPrefs->RemapTilde ) {
					ascii &= 0x1f;
					commanddown = FALSE;
				}
/* NONO */
//			}
//			else
//				return;
		}
	}

	if ((TelInfo->numwindows < 1) || (tw->active != CNXN_ACTIVE)) 
		return;

	if ( (ascii == '@' || ascii == 32) && controldown ) //this, along with the fixed KCHR that mapps a Cntl-@ to
		ascii = 0;	// a @, takes care of Apple not posting NULL key values


	//	map '`' to ESC if needed
	if (ascii == '`' && gApplicationPrefs->RemapTilde && !commanddown)
		ascii = ESC;

	// map Del to ^D if the user wants it
	if ( code == 0x75 && tw->remapCtrlD ) {
		code = 0x34;
		ascii = 4;
	}

	if ( code == BScode ) {
		// handle mapping BS to DEL, flipping on option
		if ( tw->bsdel ) {
			if ( (optiondown && tw->emacsmeta != 2) || commanddown )
				ascii = BS;
			else
				ascii = DEL;
		} else {
			if ( (optiondown && tw->emacsmeta != 2) || commanddown )
				ascii = DEL;
			else
				ascii = BS;
		}
	}
		
	if ( !tw->enabled && tw->restart_any_flow && ascii != tw->TELstop  ) {
		// if we are suspended, and we have negotiated restart_any 
		// with the host, then enable the screen on anything but an XOFF.  We will
		// eat the XON later if that is what this is. (RFC 1372 --CCP 2.7)
		tw->enabled = 1;
		changeport(scrn, scrn);
	}

	/* Remap PgUp,PgDown,Home,End if the user wants it that way */
	// RAB BetterTelnet 2.0b3 - we don't check to see if we're using vt220
	if (tw->pgupdwn && code >= KPlowest) // do page up/down on vt100
		if ( CheckPageKeys(code) )
			return;		

	if ( code >= KPlowest ) {
/*
		if (theWorld.keyBoardType == envStandADBKbd)	// standard MacII keyboard has keypad +,- 
		{												// codes switched	
			if (code == 0x45)									
				code = 0x4e;
			else if (code == 0x4e)
				code = 0x45;
		}
*/
		if ( !tw->keypadmap || code == 0x4c || code > 0x51 || code < 0x43 ) {
			// dont remap operators
			if ( (tw->clientflags & PASTE_IN_PROGRESS)
			  && (tw->pastemethod || tw->outlen > 32767) ) {
				// queue this
				trbuf[0] = 0;
				trbuf[1] = kpxlate[shifted][code - KPlowest];
				kbwrite( tw, trbuf, 2);
				return;
			}

			kbflush( tw );
			ascii = kpxlate[shifted][code - KPlowest];
			// Should we check here for ascii being zero?
			VSkbsend(tw->vs, (unsigned char) ascii, tw->echo, shifted);
			return;
		}
	}

//	VSprintf("trin: ");
//	VSdump(&ascii, 1);

	//	Handle whatever mapping is needed.
//	mac_nat(tw, &ascii); // LU/PM: Convert char from mac to nat
	if ( GetTranslationIndex(tw->outnational) != kTRJIS ) {
		if (tw->troutcount >= sizeof(tw->troutbuf)) {
			// !!!! shouldn't occur...
			Debugger();
			tw->troutcount = 0;
		}
		pbuf = tw->troutbuf;
		pbuf[tw->troutcount++] = ascii;

		if ( tw->troutcount < sizeof(tw->troutbuf) - 1 ) {
			EventRecord nextEvent;
			if ( EventAvail(keyDownMask|autoKeyMask, &nextEvent) ) {
				/* wait for next char */
				return;
			}
		}

		inlen = tw->troutcount;
		trlen = sizeof(trbuf);
		res = trbuf_mac_nat( tw, pbuf, &inlen, trbuf, &trlen );
		if ( res && res != kTECPartialCharErr ) {
			// translation failed, leave data as-is
			trlen = tw->troutcount;
		} else {
			// translation ok, or no data yet
			if ( inlen ) {
				// keep a few chars
				for (i = inlen; i < tw->troutcount; i++) {
					pbuf[i - inlen] = pbuf[i];
				}
				tw->troutcount -= inlen;
			}
			if ( !trlen ) {
				// nothing yet
				if ( tw->troutcount < sizeof(tw->troutbuf) ) {
					return;
				}
				// temp translation buffer full, unable to translate...
				// flush data ?
				trlen = tw->troutcount;
				res = trflush_mac_nat( tw );
			} else {
				// translation complete
				pbuf = trbuf;
			}
		}
		tw->troutcount = 0;
	} else {
		// send as-is ?
		if ( !tw->troutcount && !(ascii & 0x80) ) {
			pbuf = &ascii;
			trlen = 1;
		} else {
			pbuf = tw->troutbuf;
			pbuf[tw->troutcount++] = ascii;
			if ( tw->troutcount == 1 )
				// wait for second byte
				return;
			trlen = tw->troutcount;
			tw->troutcount = 0;
		}
	}

//	VSprintf("trout: ");
//	VSdump(pbuf, trlen);

	if ( (tw->clientflags & PASTE_IN_PROGRESS)
	  && (tw->pastemethod || tw->outlen > 32767) ) {
		// queue this
		kbwrite( tw, pbuf, trlen);
		return;
	} else if ( tw->lmode ) {
		for (res = 0; res < trlen; ++res) {
			// Some form of linemode is active; we dont touch it after them
			process_key( pbuf[res], tw );
		}
		return;
	}

	
	// BSD-like mapping.... if we don't want this, set chars to zero and it wont work
	//CCP 2.7: this is now AFTER the linemode stuff, so that linemode can handle it differently 
	if ( ascii == tw->TELstop ) {
		if ( tw->allow_flow ) {
			// remote flow control can turn this off
			tw->enabled = 0;
			changeport( scrn, scrn );
			return;
		}
	} else if ( ascii == tw->TELgo ) {
		if ( tw->allow_flow ) {
			// remote flow control can turn this off
			tw->enabled = 1;
			changeport( scrn, scrn );
			return;
		}
	} else if ( ascii == tw->TELip ) {
		char *tellUser = "\n\r[Interrupt Process]\n\r";
		parse( tw, (unsigned char *)tellUser, 23 );
		netpush( tw->port );
		netwrite( tw->port, "\377\364", 2 );			//IAC IP 
		netpush( tw->port );
		netwrite( tw->port, "\377\375\006" ,3 );		// send Do TM 
		tw->timing = 1;									// set emulate to TMwait 
		return;
	}

	if ( tw->echo && !tw->halfdup ) {
		// Handle klude-linemode
		if ( ascii > 31 && ascii < 127 && code < KPlowest ) {
			// printable key 
			kbwrite( tw, pbuf, trlen);
			parse(tw, pbuf, trlen);
			return;								// OK, were set...
		} else if ( code == BScode ) {
			if ( tw->kblen > 0 ) {
				--tw->kblen;
				parse( tw, (unsigned char *)"\010 \010", 3 );
			}
			return;
		} else if ( ascii == KILLCHAR ) {
			while ( tw->kblen > 0 ) {
				parse( tw, (unsigned char *)"\010 \010", 3 );
				--tw->kblen;
			}
			return;
		} else if ( code < KPlowest ) {
			// if full send buffer
			kbflush( tw );
			if ( ascii != CR ) {
				sendch = '@' + ascii;
				parse( tw, (unsigned char *)"^", 1 );
				parse( tw, &sendch, 1 );
			}
		}
	} //end if klude-linemode


	if (ascii == '\015') {
		// CR. If crmap is on, send CR-NULL instead of CR-LF.
		SendCRAsIfTyped(tw);
		return;
	}

	kbflush( tw );

	if (tw->echo && tw->halfdup) 
		parse( tw, pbuf, trlen );

	if ( ascii != 255 ) {
		netwrite( tw->port, pbuf, trlen );
	} else {
		netwrite( tw->port, "\377\377", 2 );
	}

}

void	HandleMouseDown(EventRecord myEvent)
{
	GrafPtr	whichWindow;
	short	code, myRGMnum;
	short 	growErr, i;
	short	theItem;
	DialogPtr dlogp;
	short vs;

	code = FindWindow(myEvent.where, &whichWindow);
	
	switch (code) {
		case inMenuBar:
			if ( gApplicationPrefs->BlinkCursor && (vs = RSfindvwind(FrontWindow())) >= 0 )
				VScursblinkoff(vs);
			if (myEvent.modifiers & optionKey)
			{
				switchToOptionMenus(TRUE);
				HandleMenuCommand(MenuSelect(myEvent.where),myEvent.modifiers);	
				switchToOptionMenus(FALSE);
			}
			else if (myEvent.modifiers & shiftKey)
			{
				switchToShiftMenus(TRUE);
				HandleMenuCommand(MenuSelect(myEvent.where),myEvent.modifiers);			
				switchToShiftMenus(FALSE);
			}
			else
				HandleMenuCommand(MenuSelect(myEvent.where),myEvent.modifiers);			
			if ( gApplicationPrefs->BlinkCursor && (vs = RSfindvwind(FrontWindow())) >= 0 )
				VScursblinkon(vs);
			break;
		case inSysWindow:
			SystemClick(&myEvent, whichWindow);
			break;
		case inGoAway:
			if (TrackGoAway( whichWindow, myEvent.where))
				CloseAWindow((WindowPtr)whichWindow);
			break;

		case inDrag:
			if ((whichWindow != FrontWindow()) &&
					(!(myEvent.modifiers & cmdKey)) &&
					(!(myEvent.modifiers & optionKey))) {
				SelectWindow(whichWindow);
				}
			DragWindow(whichWindow, myEvent.where, &TelInfo->dragRect);
			break;

		case inZoomIn:
		case inZoomOut:
			if (TrackBox( whichWindow, myEvent.where, code))
				RSzoom( whichWindow, code, myEvent.modifiers & shiftKey);
			break;
	
	/* NCSA: SB - Telnet now allows you to grow the TEK window, finally.		*/
	/* NCSA: SB - So check to see if the click was in a TEK window				*/
	
		case inGrow:
			growErr = RSsize( whichWindow, (long *) &myEvent.where, myEvent.modifiers);
			switch(growErr) {
			case (-4):		 /*bad mem problems, kill window, signal user */				
				OutOfMemory(-4);
				if ((i = WindowPtr2ScreenIndex(whichWindow)) >= 0) 
				{
					netclose(screens[i].port);
					removeport(&screens[i]);
				}
				break;
			case (-2):		/* no resize due to memory error, signal user */
				OutOfMemory(-2);
				break;
			default: 		/* ok resize, or window not found */
				break;
			}
			myRGMnum = RGfindbywind(whichWindow);			/* NCSA: SB - is it a TEK window click? */
			if (myRGMnum  != -1)			/* NCSA: SB - Anyone want to play some BOLO? */
				{											/* NCSA: SB */
				RGMgrowme((short)myRGMnum, whichWindow,(long *) &myEvent.where,myEvent.modifiers);	/* NCSA: TG */
				}											/* NCSA: SB */
		break;
		
		case inContent:
			if (whichWindow != FrontWindow()) {
				SelectWindow(whichWindow);
				}
			else {
				if (((WindowPeek)whichWindow)->windowKind == dialogKind) {
					if (IsDialogEvent(&myEvent)) {
						if (DialogSelect(&myEvent, &dlogp, &theItem))
							HandleModeless(&myEvent, dlogp, theItem);
					}
					break;
				}
				if (RSclick(whichWindow, myEvent) <0) {
					SetPort(whichWindow);
					GlobalToLocal(&myEvent.where);
					RGmousedown(whichWindow, &myEvent.where );
					}
			}
			break;
		
		default:
			break;
	}
}


#pragma profile off
void	DoEvents( EventRecord* theEvent)
{
	Boolean		gotOne;			/* Did we get an event */
	short		vs;
	EventRecord	myEvent;
	short		scratchshort;
	DialogPtr	dlogp;
	static unsigned long blinkTicks = 0;

	if ( theEvent == NULL ) {
		theEvent = &myEvent;
		gotOne = WaitNextEvent(everyEvent, &myEvent, gApplicationPrefs->TimeSlice, 0L);
	} else {
		gotOne = true;
	}
	if (!gotOne) { // RAB BetterTelnet 1.2 - null events to dialogs
		if (TelInfo->macrosModeless) {
			if (!CallStdFilterProc(TelInfo->macrosModeless, theEvent, &scratchshort))
				if (IsDialogEvent(theEvent))
					DialogSelect(theEvent, &dlogp, &scratchshort);
		}
	}

	if (gotOne) {
/* BYU 2.4.11 - Turn the cursor off when the human makes the slightest move. */
		if ( gApplicationPrefs->BlinkCursor ) {
			if ( (vs = RSfindvwind(FrontWindow())) >= 0 && vs == screens[scrn].vs ) {
				if (!(myEvent.modifiers & cmdKey) &&	
					(myEvent.what == keyDown || myEvent.what == autoKey))
					VScursblinkon(vs);				
				else
					VScursblinkoff(vs);
			}			
		}
		HandleEvent(theEvent);
	} else if (gApplicationPrefs->BlinkCursor && !TelInfo->suspended) {
		if ( (vs = RSfindvwind(FrontWindow())) >= 0 && vs == screens[scrn].vs )
			VScursblink(vs);
	}

	if (FrontWindow() != TelInfo->macrosModeless) // RAB BetterTelnet 1.2
		updateCursor(0);

/* DJ: Welcome to blink-land */
	if ( LMGetTicks() - blinkTicks >= BLINK_PERIOD ) {
		blinkTicks = LMGetTicks();
		gBlink ^= 1;
		VSPulseAll();
	}
}						

static long currentScript, defaultKCHR, mungeCount;
static Boolean haveChangedKCHR;
//called at startup to figure out the default roman KCHR and the active script
void scriptKbdInit(void)
{
	long	response;

	currentScript = GetScriptManagerVariable(smKeyScript);//get active script
	defaultKCHR = GetScriptVariable(smRoman, smScriptKeys); //get the smRoman default KCHR
	mungeCount = GetScriptManagerVariable(smMunged); //Get the mungeCount
	haveChangedKCHR = FALSE;
#if GENERATINGPOWERPC
	gKeyboardLayoutType = GetKeyboardLayoutType(LMGetKbdType());
#endif
}

#if GENERATINGPOWERPC
/*  Returns the keyboard layout type.  */

OSType GetKeyboardLayoutType(short deviceID)
{
    OSType keyboardLayoutType;

    switch (deviceID) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x06:
        case 0x08:
        case 0x0C:
        case 0x10:
        case 0x18:
        case 0x1B:
        case 0x1C:
        case 0xC0:
        case 0xC3:
        case 0xC6:
            keyboardLayoutType = kKeyboardANSI;
            break;
        case 0x12:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x1A:
        case 0x1E:
        case 0xC2:
        case 0xC5:
        case 0xC8:
        case 0xC9:
        case 0xCE:
            keyboardLayoutType = kKeyboardJIS;
            break;
        case 0x04:
        case 0x05:
        case 0x07:
        case 0x09:
        case 0x0D:
        case 0x11:
        case 0x14:
        case 0x19:
        case 0x1D:
        case 0xC1:
        case 0xC4:
        case 0xC7:
            keyboardLayoutType = kKeyboardISO;
            break;
        default:
            if (IsKBGetLayoutTypeAvailable ())
                keyboardLayoutType = KBGetLayoutType (deviceID);
            else
                keyboardLayoutType = kKeyboardUnknown;
            break;
    }
    return keyboardLayoutType;
}

/*  Returns true if KBGetLayoutType is available.  */

Boolean IsKBGetLayoutTypeAvailable(void)
{
    long response;

    if (Gestalt (gestaltKeyboardsLib, &response) == noErr)
        return true;
    else
        return false;
}

#endif


void SetDefaultKCHR(void)
{
	if (haveChangedKCHR)
	{
		SetScriptVariable(smRoman,smScriptKeys,defaultKCHR);
		haveChangedKCHR = FALSE;
		KeyScript(smRoman);	
	}
}
void CheckDefaultScriptSettings(void)
{
	long tempLong;
	tempLong = GetScriptManagerVariable(smMunged);
	if (tempLong > mungeCount)
	{
		currentScript = GetScriptManagerVariable(smKeyScript);//get the new active script
		if ((currentScript != smRoman)&&(haveChangedKCHR)) //roman has been switched out and we need to fix it
		{	
			SetScriptVariable(smRoman,smScriptKeys,defaultKCHR);
			currentScript = GetScriptManagerVariable(smKeyScript);//get the new active script
			haveChangedKCHR = FALSE; //we dont mess with non-roman scripts	
		}
		mungeCount = tempLong;  //update the mungeCount
	}
}


void HandleEvent(EventRecord *myEvent) //CCP split this from DoEvents so we can call this from
{										//other places
	short i,vs;
	DialogPtr dlogp;
	short theItem, tempstatus;
	short commanddown;

	switch(myEvent->what) {
	case mouseDown:
		HandleMouseDown(*myEvent);
		break;

	case updateEvt:
		switch(((WindowPeek)myEvent->message)->windowKind) {
			case WIN_CONSOLE:
			case WIN_LOG:
			case WIN_CNXN:
				if (RSupdate((GrafPtr) myEvent->message))
					putln("Ack, problem in update!");
				break;
			
			case WIN_ICRG:
				if (MacRGupdate((WindowPtr) myEvent->message))
					putln("Ack, problem in update!");
				break;
				
			case WIN_TEK:
				if (RGupdate((GrafPtr) myEvent->message) ==0) 
					TekDisable(RGgetVG((GrafPtr) myEvent->message));
				else
					putln("Ack, problem in update!");
				break;

			case dialogKind:
				CallStdFilterProc((GrafPtr) myEvent->message, myEvent, &theItem);
				if (IsDialogEvent(myEvent))
					DialogSelect(myEvent, &dlogp, &theItem);
				break;

			default:
				putln("Bad windowkind!");
				break;
			}			
		break;

	case keyDown:
	case autoKey:
		if (FrontWindow())
			if (((WindowPeek)FrontWindow())->windowKind == dialogKind) {
				tempstatus = 1;
				if (CallStdFilterProc(FrontWindow(), myEvent, &theItem)) {
					HandleModeless(myEvent, FrontWindow(), theItem);
					break;
				}
				commanddown = ((myEvent->modifiers & cmdKey) != 0);
				if (commanddown) {
					HandleKeyDown(*myEvent, &screens[scrn]);		/* All key events are processed through here */
					break;
				}
				if (IsDialogEvent(myEvent)) {
					if (DialogSelect(myEvent, &dlogp, &theItem))
						HandleModeless(myEvent, dlogp, theItem);
					else tempstatus = 0;
				}
				if (tempstatus) break;
			}
		HandleKeyDown(*myEvent, &screens[scrn]);		/* All key events are processed through here */
		break;

	case diskEvt:			/* check to see if disk needs to be initialized */
		myEvent->where.h = 100;
		myEvent->where.v = 120;
		if (noErr != (( myEvent->message >> 16 ) & 0xffff )) {	/* check hi word */
			DILoad();
			DIBadMount( myEvent->where, myEvent->message);	/* BYU LSC */
			DIUnload();
		}
		break;

	case activateEvt:
		if ((myEvent->modifiers & activeFlag)==1)  //its an activate event
		{
			if (((WindowPeek)myEvent->message)->windowKind == dialogKind) {
				CallStdFilterProc((GrafPtr) myEvent->message, myEvent, &theItem);
				if (IsDialogEvent(myEvent))
					DialogSelect(myEvent, &dlogp, &theItem);
				if (TelInfo->macrosModeless)
				if ((GrafPtr)myEvent->message == TelInfo->macrosModeless) {
					EnableItem(myMenus[Edit],EDcut);
					EnableItem(myMenus[Edit],EDcopy);
					EnableItem(myMenus[Edit],EDpaste);
					EnableItem(myMenus[Edit],EDclear);
				} else {
					DisableItem(myMenus[Edit],EDcut);
					DisableItem(myMenus[Edit],EDcopy);
					DisableItem(myMenus[Edit],EDpaste);
					DisableItem(myMenus[Edit],EDclear);
				}
				break;
			} else {
				DisableItem(myMenus[Edit],EDcut);
				DisableItem(myMenus[Edit],EDclear);
			}
			i=WindowPtr2ScreenIndex((GrafPtr) myEvent->message);	// We need to know who 
			CheckDefaultScriptSettings(); //see if someone has changed the script
			
			//this next bit takes care of setting the KCHR based on the EMACS hack pref
			if ((currentScript == smRoman)&&(screens[i].emacsmeta == 2))
			{
/* NONO */
/* why changing current script ?
   this forced keymap to 'US' whatever the keyboard is really.
   looks like I could get rid of all this haveChangedKCHR stuff, no ?
				if (!haveChangedKCHR) //if we haven't already done this
				{
					SetScriptVariable(currentScript,smScriptKeys,502); //set the KCHR for EMACS
					KeyScript(smRoman);	//Make it active
					haveChangedKCHR = TRUE;
				}
*/
/* NONO */
			}
			else if (haveChangedKCHR) //new active window doesnt want EMACS hack
			{
				SetScriptVariable(currentScript,smScriptKeys,defaultKCHR); 
				KeyScript(smRoman);	
				haveChangedKCHR = FALSE;
			}

			AdjustMenus();
			DrawMenuBar();

			i=WindowPtr2ScreenIndex((GrafPtr) myEvent->message);	/* We need to know who */
			if (i>=0) 
			{
				if ((screens[i].curgraph>-1) && (!(myEvent->modifiers & optionKey)))
					detachGraphics(screens[i].curgraph);
				changeport(scrn,i);
				scrn=i;
			}
			if ((i=RSfindvwind((GrafPtr) myEvent->message))>=0) 
			{
				if (RSTextSelected(i)) 
				{					
					EnableItem(myMenus[Fil],FLprint);		
					EnableItem(myMenus[Edit],EDcopy);	
					EnableItem(myMenus[Edit],EDcopyt);		
				} 
				else 
				{								
					DisableItem(myMenus[Fil],FLprint);	
					DisableItem(myMenus[Edit],EDcopy);		
					DisableItem(myMenus[Edit],EDcopyt);		
				}										
				RSactivate(i);
				TelInfo->myfrontwindow=(WindowPeek) myEvent->message;
				TelInfo->myfronttype=DEC_WINDOW;
				TelInfo->myfrontvs = i;
				TelInfo->myfrontRgn =0L;
				updateCursor(1);
			} 
			else 
			{					
				TelInfo->myfrontwindow=(WindowPeek) myEvent->message;
				TelInfo->myfronttype=TEK_WINDOW;
				TelInfo->myfrontRgn =0L;
				updateCursor(1);
				if ( (i = RGgetdnum((GrafPtr) myEvent->message)) >-1) 
				{
					if (( i = RGgetVS( i)) >-1) 
					{
						EnableItem(myMenus[Fil],FLprint);	// enable printing 
						EnableItem(myMenus[Edit],EDcopy);	// - enable copying 
						DisableItem(myMenus[Edit],EDcopyt);	
						i = findbyVS( i);
						changeport(scrn,i);
						scrn=i;
					}
				}
			}
		} 
		else //its a disable event
		{
			short i;
			DisableItem(myMenus[Edit],EDcut);
			DisableItem(myMenus[Edit],EDclear);
			DisableItem(myMenus[Edit],EDcopy);
			DisableItem(myMenus[Edit],EDpaste);
			AdjustMenus();
			DoTheMenuChecks();
			DrawMenuBar();
			if ((i=RSfindvwind((GrafPtr) myEvent->message))>=0)
				RSdeactivate(i);
			NoWindow();
		}
		break;
	case osEvt:
		switch(( myEvent->message >>24) &0xff) {	
			case switchEvt:
				if (myEvent->message & 0x20)
					/*Convert clipboard here if necc. (it is not)*/;

				if (myEvent->message & 0x1) {		/* Resume Event */
					GrafPtr window;

					TelInfo->suspended = FALSE;				/* We are no longer in suspension */
					if (gHaveInstalledNotification)
					{
						NMRemove(nRecPtr);
						/*DisposeHandle(nRecPtr->nmIcon, true);*/
						if (nRecPtr->nmIcon)
							DisposeIconSuite(nRecPtr->nmIcon, TRUE);
						DisposePtr(nRecPtr);
						gHaveInstalledNotification = FALSE;
					}

					window = FrontWindow();			/* Who's on first */
					if ( (vs=RSfindvwind(window)) >= 0) 
					{
						CheckDefaultScriptSettings();//update script settings
						if ((currentScript == smRoman)&&(haveChangedKCHR)) // set it back
						{
							SetScriptVariable(currentScript,smScriptKeys,502); //set the KCHR for EMACS
							KeyScript(smRoman);	//Make it active
						}
						RSactivate(vs);
						TelInfo->myfrontwindow = (WindowPeek) window;
						TelInfo->myfronttype=DEC_WINDOW;
						TelInfo->myfrontvs = vs;
						TelInfo->myfrontRgn =0L;
						updateCursor(1);
					} else if ( (long)window != 0L) {
						myEvent->message = (long) window;
						myEvent->modifiers |= activeFlag;
						myEvent->what = activateEvt;
						myEvent->when = TickCount();
						SystemEvent( myEvent);
						}
					}
				else {								/* Suspend Event */
					GrafPtr window;
					CheckDefaultScriptSettings();//update script settings
					if ((currentScript == smRoman)&&(haveChangedKCHR)) //switch out to default KCHR when while suspended
					{
						SetScriptVariable(currentScript,smScriptKeys,defaultKCHR); 
						KeyScript(smRoman);	
					}
					TelInfo->suspended=TRUE;					/* We be in waitin' */

					window = FrontWindow();			/* Who's on first */
					if ((window = FrontWindow()) != nil) {
						if ( (vs=RSfindvwind(window)) >= 0)
							RSdeactivate(vs);
						else if ( (long)window != 0L) {
							myEvent->message = (long) window;
							myEvent->modifiers &= (~activeFlag);
							myEvent->what = activateEvt;
							myEvent->when = TickCount();
							SystemEvent( myEvent);
							}
						}
					NoWindow();
					}
				break;			/* switch of myEvent->message >>24 */
			default:
				break;
			}
		break;
	case kHighLevelEvent:
		(void) AEProcessAppleEvent(myEvent);
		break;			
	}

}
//#pragma profile on

void	CloseAWindow(WindowPtr	theWindow)
{
	short	i;
	long junk;
	
	switch(((WindowPeek)theWindow)->windowKind) {
		case WIN_CONSOLE:
//			Debugger();				// Can't close the console window
			// can't close it, but we can hide it...
			DebugKeys(TRUE, 39, -1);
			break;

		case WIN_LOG:
			if (theWindow == ftplog->wind) {
				CheckItem(myMenus[Fil],FLlog,FALSE);
				TelInfo->ftplogon = FALSE;
				RShide(ftplog->vs);
			}
			break;
					
		case WIN_CNXN:
			if ((i = WindowPtr2ScreenIndex(theWindow)) >= 0) 
			{
				if ( screens[i].active == CNXN_ISCORPSE)
					destroyport(i);
				else 
				{
					if ( !ReallyClose( i) ) break;
					netclose(screens[i].port);
					removeport(&screens[i]);
				}
			}
			break;
		
		case WIN_ICRG:
			MacRGdestroy(MacRGfindwind(theWindow));
			MaxMem(&junk);
			break;
			
		case WIN_TEK:
			destroyGraphics(RGgetVG(theWindow));
			MaxMem(&junk);
			break;

		case dialogKind:
			CloseModelessDialog(theWindow);
			break;

		default:
			DebugStr("\pBad windowkind!");
			break;
		}			

		NoWindow();
}


void NotifyUser (void)
{
	OSErr err = noErr;

	if ((!TelInfo->suspended)||(!gApplicationPrefs->NotifyUser)||gHaveInstalledNotification)
		return;
	
	nRecPtr = (NMRecPtr) myNewPtr(sizeof(NMRec)); 
	if (nRecPtr == NULL)
		return;
	nRecPtr->qType = nmType;
	nRecPtr->nmMark = 1;
	err = GetIconSuite(&(nRecPtr->nmIcon), kNCSAIconFamilyId, svAllSmallData);
	if (err != noErr) nRecPtr->nmIcon = nil;
	nRecPtr->nmSound = nil;
	nRecPtr->nmStr = nil;
	nRecPtr->nmResp = nil;
	NMInstall(nRecPtr);
	gHaveInstalledNotification = TRUE;
}

void HandleModeless(EventRecord *theEvent, DialogPtr dlogp, short theItem)
{
/* NONO */
	extern DialogPtr key_dlog;
	extern int key_abt;
/* NONO */
	short result;

	if (dlogp == TelInfo->macrosModeless) {
		MacroDialog(&TelInfo->newMacros, dlogp,
					theEvent, theItem);
		if (theItem == 1) {
			updateCursor(0);
			CloseMacros(&TelInfo->newMacros, dlogp);
		} else if (theItem == 2) {
			updateCursor(0);
			CancelMacros(&TelInfo->newMacros, dlogp);
		}
/* NONO */
	} else if (dlogp == key_dlog) {
		if (theItem == 1) {
			key_abt = 1;
		}
/* NONO */
	}
}

void CloseModelessDialog(DialogPtr dlogp)
{

	if (dlogp == TelInfo->ipModeless) {
		DisposeDialog(TelInfo->ipModeless);
		TelInfo->ipModeless = 0;
	}

	if (dlogp == TelInfo->macrosModeless)
		CloseMacros(&TelInfo->newMacros, dlogp);
}

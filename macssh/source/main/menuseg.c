// menuseg.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
*    Menu Handling and initialization code.
*
*	
*	Revisions:
*	7/92 Telnet 2.6 initial version: reorganized defines, put all stray globals in a struct, and
*				put all cursors in a nice array.  Moved some routines to other places -	Scott Bulmahn			
*
*	6/94 Added support for Keypad and Function menus - Carl Bell (Baylor U.)
*/


#include "wind.h"
#include "translate.h"
#include "menuseg.proto.h"
#include "mainseg.proto.h"
#include "Sets.proto.h"				/* JMB: For Saved Sets functions */
#include "configure.proto.h"
#include "parse.proto.h"
#include "InternalEvents.h"
#include "mydnr.proto.h"
#include "rsinterf.proto.h"
#include "network.proto.h"
#include "maclook.proto.h"
#include "vrrgmac.proto.h"
#include "rgmp.proto.h"
#include "tekrgmac.proto.h"
#include "vgtek.proto.h"
#include "netevent.proto.h"
#include "Connections.proto.h"
#include "tnae.h"
#include "authencrypt.proto.h"
#include "vsdata.h"
#include "vsinterf.proto.h"
#include "otp.proto.h"
#include "sshglue.proto.h"

#include "event.proto.h"
#include "macros.proto.h"
#include "DlogUtils.proto.h"			/* For VersionNumber, OutlineItem, and DialogProc Protos */
#include "telneterrors.h"


#include <Printing.h>
#include "printing.proto.h"
#include "translate.proto.h"
#include "parse.proto.h"				// For SendNAWSinfo proto
#include "keypadmenus.proto.h"
#include "LinkedList.proto.h"
#include "movableModal.h"

MenuHandle
	myMenus[NMENUS];		/* Menu Handles .... */
static short lastMenyCommandKeys = -1; //whether last menu set had command keys

//char *tempspot;				/* temporary storage ~255 bytes malloc-ed */

extern int			gMovableModal;

extern Boolean		gTekCopying;
/*
 * External variable declarations (those which we borrow )
 *
 */
extern short scrn;
extern Cursor *theCursors[];
extern WindRec
	*screens,			/* The screen array from Maclook */
	*ftplog;					/* The FTP log screen from Maclook */
extern	short	nNational;
extern short	gTableCount;

void CloseCaptureFile(short w)
{
	VSclosecapture(w);								/* BYU 2.4.18 */
	CheckItem(myMenus[Emul], EMcapture,FALSE);		/* BYU 2.4.18 */
}

/*	portsOpen()	- Count the number of ports open. Returns 3 different answers
 *				  0= no ports at all, 1= at least active, -1= ports/none active. */
short	portsOpen(void)
{
	short pnum;

	pnum=TelInfo->numwindows-1;
	if (pnum<0)  return(0);
	while (pnum>=0) 
//		if (!screens[pnum--].active && !screens[pnum+1].corpse) return(1);
		// corpse status now part of active flag
		if (screens[pnum--].active == CNXN_ACTIVE) return(1);
	return(-1);
}

void switchToOptionMenus(Boolean useOptionedStuff)
{
	 Str255 theMenuItem;
	 short whichString;
	 if (useOptionedStuff)
	 	whichString = CLOSE_ALL_MENU_ITEM;
	 else
	 	whichString = CLOSE_MENU_ITEM;
	
	 GetIndString(theMenuItem,MISC_STRINGS,whichString);
	
	 SetMenuItemText(myMenus[Fil],FLclose,theMenuItem);
}

void switchToShiftMenus(Boolean useShiftStuff)
{
	 Str255 theMenuItem;
	 short whichString;
	 if (useShiftStuff)
	 	whichString = PREVIOUS_SESSION_STRING;
	 else
	 	whichString = NEXT_SESSION_STRING;
	
	 GetIndString(theMenuItem,MISC_STRINGS,whichString);
	 SetMenuItemText(myMenus[Conn],COnext,theMenuItem);
		 
}

void	AdjustMenus(void)
{
	short		i;
	WindowPtr	wind;

	if (gMovableModal) {
		return;
	}

	if ( (wind = FrontWindow()) != NULL
	  && ( ((WindowPeek)wind)->windowKind >= userKind
		|| ((WindowPeek)wind)->windowKind == dialogKind) )
		EnableItem( myMenus[Fil],FLclose);
	else
		DisableItem( myMenus[Fil],FLclose);
		
	if ((i=portsOpen()) <1) {
		DisableItem( myMenus[Fil],FLsave);
		DisableItem( myMenus[Fil],FLsavem);
		DisableItem( myMenus[Fil],FLprint);
		DisableItem( myMenus[Fil],FLprintScrn);
		DisableItem( myMenus[Fil],FLselect);
		DisableItem( myMenus[Fil],FLupload);
		DisableItem( myMenus[Edit],EDcut);
		DisableItem( myMenus[Edit],EDundo);
		DisableItem( myMenus[Edit],EDclear);
		DisableItem( myMenus[Edit],EDcopy);
		DisableItem( myMenus[Edit],EDretype);
		DisableItem( myMenus[Edit],EDcopyt);

#if 0	// RAB BetterTelnet 1.0fc8

		DisableItem( myMenus[Emul],EMbs);
		DisableItem( myMenus[Emul],EMdel);
		DisableItem( myMenus[Emul],EMecho);
		DisableItem( myMenus[Emul],EMwrap);
		DisableItem( myMenus[Emul],EMcrmap);
		DisableItem( myMenus[Emul],EMmapkeypad);
		DisableItem( myMenus[Emul],EMansi);
		DisableItem( myMenus[Emul],EMxterm);
		DisableItem( myMenus[Emul],EMmapd);
		DisableItem( myMenus[Emul],EMbold);
		DisableItem( myMenus[Emul],EMboldcolor);
		DisableItem( myMenus[Emul],EMbeep);
		DisableItem( myMenus[Emul],EMeightbit);
		DisableItem( myMenus[Emul],EMscroll);
		DisableItem( myMenus[Emul],EMreset);
		DisableItem ( myMenus[Emul],EMjump);
		DisableItem ( myMenus[Emul],EMpage);
		DisableItem ( myMenus[Emul],EMclear);	/* BYU 2.4.14 */
		DisableItem ( myMenus[Emul],EMscreensize);
		DisableItem ( myMenus[Emul],EMsetup);
		DisableItem ( myMenus[Emul],EMfont);
		DisableItem ( myMenus[Emul],EMsize);
		DisableItem ( myMenus[Emul],EMcolor);
		DisableItem ( myMenus[Emul],EMAnsiColor);
		DisableItem ( myMenus[Emul],EMcapture);	/* BYU 2.4.18 */
		DisableItem ( myMenus[Emul],EMqprint);

#endif
		DisableItem ( myMenus[Emul],0); // RAB BetterTelnet 1.0fc8
		DisableItem( myMenus[Net ],NEftp);
		DisableItem( myMenus[Net ],NEip);
		DisableItem( myMenus[Net ],NEayt);
		DisableItem( myMenus[Net ],NEao);
		DisableItem( myMenus[Net ],NEinter);
		DisableItem( myMenus[Net ],NEipsync);
		DisableItem( myMenus[Net ],NEsync);
		DisableItem( myMenus[Net ],NEbrk);
		DisableItem( myMenus[Net ],NEec);
		DisableItem( myMenus[Net ],NEel);
		DisableItem( myMenus[Net ],NEscroll);
		DisableItem( myMenus[Net ],NEeof);
		DisableItem( myMenus[Net ],NEabort);
		DisableItem( myMenus[Net ],NEsusp);

		if (TelInfo->ScrlLock) {
			TelInfo->ScrlLock=0;
			CheckItem(myMenus[Net ], NEscroll,FALSE);
			}
		}
	else {
		EnableItem ( myMenus[Fil],FLsave);
		EnableItem ( myMenus[Fil],FLsavem);
		EnableItem ( myMenus[Fil],FLupload);
		EnableItem ( myMenus[Fil],FLprintScrn);

		EnableItem ( myMenus[Emul],0); // RAB BetterTelnet 1.0fc8

#if 0 // RAB BetterTelnet 1.0fc8

		EnableItem ( myMenus[Emul],EMbs);
		EnableItem ( myMenus[Emul],EMdel);
		EnableItem ( myMenus[Emul],EMecho);
		EnableItem ( myMenus[Emul],EMwrap);
		EnableItem ( myMenus[Emul],EMcrmap);
		EnableItem ( myMenus[Emul],EMansi);
		EnableItem ( myMenus[Emul],EMxterm);
		EnableItem ( myMenus[Emul],EMmapd);
		EnableItem ( myMenus[Emul],EMbold);
		EnableItem ( myMenus[Emul],EMbeep);
		EnableItem ( myMenus[Emul],EMboldcolor);
		EnableItem ( myMenus[Emul],EMeightbit);
		EnableItem ( myMenus[Emul],EMmapkeypad);

#endif

		if (nNational > 0) {
			EnableItem ( myMenus[National], 0);
			}

#if 0

		EnableItem ( myMenus[Emul],EMscroll);
		EnableItem ( myMenus[Emul],EMreset);
		EnableItem ( myMenus[Emul],EMjump);
		EnableItem ( myMenus[Emul],EMpage);
		EnableItem ( myMenus[Emul],EMclear);	/* BYU 2.4.14 */
		EnableItem ( myMenus[Emul],EMscreensize);
		EnableItem ( myMenus[Emul],EMsetup);
		EnableItem ( myMenus[Emul],EMfont);
		EnableItem ( myMenus[Emul],EMsize);
		EnableItem ( myMenus[Emul],EMcapture);	/* BYU 2.4.18 */
		EnableItem ( myMenus[Emul],EMqprint);

#endif

		if (TelInfo->haveColorQuickDraw) 
		{
			EnableItem ( myMenus[Emul],EMcolor);
			EnableItem ( myMenus[Emul],EMAnsiColor);
		} else {
			DisableItem ( myMenus[Emul],EMcolor);
			DisableItem ( myMenus[Emul],EMAnsiColor);
		}
		EnableItem ( myMenus[Net ],NEftp);
		EnableItem ( myMenus[Net ],NEip);
		EnableItem ( myMenus[Net ],NEayt);
		EnableItem ( myMenus[Net ],NEao);
		EnableItem ( myMenus[Net ],NEinter);
		EnableItem ( myMenus[Net ],NEipsync);
		EnableItem ( myMenus[Net ],NEbrk);
		EnableItem ( myMenus[Net ],NEsync);
		EnableItem ( myMenus[Net ],NEec);
		EnableItem ( myMenus[Net ],NEel);
		EnableItem ( myMenus[Net ],NEscroll);
		EnableItem( myMenus[Net ],NEeof);
		EnableItem( myMenus[Net ],NEabort);
		EnableItem( myMenus[Net ],NEsusp);
	}

	if (TelInfo->macrosModeless)
	if (FrontWindow() == TelInfo->macrosModeless) {
		EnableItem(myMenus[Edit],EDcut);
		EnableItem(myMenus[Edit],EDcopy);
		EnableItem(myMenus[Edit],EDpaste);
		EnableItem(myMenus[Edit],EDclear);
	}

}

/*	switchMenus( which) - Switch from our current menus to the key menus (1)
 *						  or the normal menus (0). */
void switchMenus(short which, short force)
{
	short i;
	Str255 theMenuItem;
		
	if ((lastMenyCommandKeys != which) || force)
	{
		lastMenyCommandKeys = which;
		DeleteMenu( fileMenu);					/* Take them from the menu bar */
		DeleteMenu( editMenu);
		DeleteMenu( termMenu);
		DeleteMenu(  netMenu);
		DeleteMenu(  keyMenu);				/* Baylor */
		DeleteMenu( funcMenu);				/* Baylor */
		DeleteMenu(NfileMenu);				
		DeleteMenu(NeditMenu);
		DeleteMenu(NtermMenu);
		DeleteMenu(NnetMenu);

		if (which) 
		{
			myMenus[Fil ] = GetMenu(NfileMenu);
			myMenus[Edit] = GetMenu(NeditMenu);
			myMenus[Emul] = GetMenu(NtermMenu);
			myMenus[Net ] = GetMenu(NnetMenu );
			myMenus[Keypad] = GetMenu(keyMenu);			/* Baylor */
			myMenus[Function] = GetMenu(funcMenu);		/* Baylor */
	
			DeleteMenuItem(myMenus[Conn],COnext);
			InsertMenuItem(myMenus[Conn],(StringPtr)"\017Next Session/N\0",0);
			GetIndString(theMenuItem, MISC_STRINGS, NEXT_SESSION_STRING);
	 		SetMenuItemText(myMenus[Conn],COnext,theMenuItem);
			}
		else 
		{
			myMenus[Fil ] = GetMenu(fileMenu);
			myMenus[Edit] = GetMenu(editMenu);
			myMenus[Emul] = GetMenu(termMenu);
			myMenus[Net ] = GetMenu( netMenu);
			myMenus[Keypad] = GetMenu(keyMenu);			/* Baylor */
			myMenus[Function] = GetMenu(funcMenu);		/* Baylor */
	
			DeleteMenuItem(myMenus[Conn],COnext);
			InsertMenuItem(myMenus[Conn],(StringPtr)"\015Next Session\0",0);
			GetIndString(theMenuItem, MISC_STRINGS, NEXT_SESSION_STRING);
	 		SetMenuItemText(myMenus[Conn],COnext,theMenuItem);
		}
	}
	for(i=1; i<Conn; i++)
		InsertMenu( myMenus[i], opspecMenu);	/* Put them in the menu bar */

	if (gApplicationPrefs->KeyPadAndFuncMenus) {			/* Baylor */
		InsertMenu(myMenus[Keypad], 0);						/* Baylor */
		InsertMenu(myMenus[Function], 0);					/* Baylor */
		}

	AdjustMenus();			/* Hilite the right stuff */

	if (TelInfo->MacBinary)
		CheckItem(myMenus[Fil],FLbin,TRUE);		/* Check MacBinary ... */
	else
		CheckItem(myMenus[Fil],FLbin,FALSE);

	if (TelInfo->ftplogon)
		CheckItem(myMenus[Fil],FLlog,TRUE);		/* and the log .... */
	else
		CheckItem(myMenus[Fil],FLlog,FALSE);

	if (TelInfo->ScrlLock) 								/* and the Suspend network ... */
		CheckItem(myMenus[Net], NEscroll,TRUE);
	else 
		CheckItem(myMenus[Net], NEscroll,FALSE);

	if (TelInfo->numwindows>0)							/* and set the BSDEL flag */
		CheckItem(myMenus[Emul], EMbs+screens[scrn].bsdel,FALSE);

	DrawMenuBar();								/* Draw what we have done */

	SetupOpSpecSubmenu(myMenus[OpSpec]);

	if (!TelInfo->haveColorQuickDraw)
		DisableItem( myMenus[Emul], EMcolor);

}

/*	setupmenu - Set up (load) all menus and prepare menu bar.
 *				set default check marks. */
void setupmenu(short def, DialogPtr startupBox)
{
	short scratchshort, i;
	Str255 scratchPstring;
	static Str255 stemp;

	GetIndString(stemp, MISC_STRINGS, REBUILDING_FONT_MENU + 1);
	SetTEText(startupBox, 3, stemp);

/* NONO */
	/*lastMenyCommandKeys = def;*/
	lastMenyCommandKeys = -1;
/* NONO */
	myMenus[0] = GetMenu(appleMenu);			/* Get all of our friendly menus */
	myMenus[Conn] = GetMenu(connMenu);
	myMenus[Font] = GetMenu(fontMenu);
	myMenus[FontOther] = GetMenu(fontMenu2);	// RAB BetterTelnet 1.0.1
	myMenus[Sizem] = GetMenu(sizeMenu);
	myMenus[OpSpec] = GetMenu(opspecMenu);
	myMenus[PrefsSub] = GetMenu(prefsMenu);
	myMenus[National] = GetMenu(transMenu);
	myMenus[Keypad] = GetMenu(keyMenu);			/* Baylor */
	myMenus[Function] = GetMenu(funcMenu);		/* Baylor */
	myMenus[Fil ] = GetMenu(NfileMenu);
	myMenus[Edit] = GetMenu(NeditMenu);
	myMenus[Emul] = GetMenu(NtermMenu);
	myMenus[Net ] = GetMenu(NnetMenu );

	AppendResMenu(myMenus[0], 'DRVR');					/* Add in the DA's */

	InsertMenu(myMenus[0], 0);						/* Insert the Menus into the bar */
	InsertMenu(myMenus[OpSpec], 0);
	InsertMenu(myMenus[Conn], 0);
	if (gApplicationPrefs->KeyPadAndFuncMenus) {	/* Baylor */
		InsertMenu(myMenus[Keypad], 0);				/* Baylor */
		InsertMenu(myMenus[Function], 0);			/* Baylor */
		}
	InsertMenu(myMenus[Font], -1);
	InsertMenu(myMenus[FontOther], -1);
	InsertMenu(myMenus[Sizem], -1);
//	InsertMenu(myMenus[OpSpec], -1);
	InsertMenu(myMenus[National], -1);
	InsertMenu(myMenus[PrefsSub], -1);

	if (!gApplicationPrefs->monospacedOut)
		AppendResMenu(myMenus[Font], 'FONT');				/* Put the fonts in the font menu */
	else addMonoSpacedFonts(myMenus[Font], startupBox);
	scratchshort = CountMItems(myMenus[Font]);
	for (i = 1; i <= scratchshort; i++) {
		// RAB BetterTelnet 1.0.1
		GetMenuItemText(myMenus[Font],i,scratchPstring);
/* NONO */
		//AppendMenu(myMenus[FontOther],scratchPstring);
		AppendMenu( myMenus[FontOther], "\pempty" );
		SetMenuItemText( myMenus[FontOther], i, scratchPstring ); 
/* NONO */
	}
	SetupOpSpecSubmenu(myMenus[OpSpec]);
	switchMenus(def, 0);

	GetIndString(stemp, MISC_STRINGS, REBUILDING_FONT_MENU + 2);
	SetTEText(startupBox, 3, stemp);
}

void addMonoSpacedFonts(MenuHandle theMenu, DialogPtr startupBox)
{
	GrafPtr tempPort, savedPort;
	Boolean doItAll = FALSE;
	Handle	theGoodFonts, theBadFonts;
	short	numGoodFonts, numBadFonts,numFontsInMenu;
    short menuIndex;
	Str255 fontListName,currentFontName;
	
	//here begins the caching of fonts to save time caculating proportionality
	UseResFile(TelInfo->SettingsFile);

	theGoodFonts = Get1Resource('STR#',4000); //the good fonts
	if (!theGoodFonts) {
		UseResFile(TelInfo->ApplicationFile);
		theGoodFonts = Get1Resource('STR#',4000);
		theBadFonts = Get1Resource('STR#',5000);
		DetachResource(theGoodFonts);
		DetachResource(theBadFonts);
		UseResFile(TelInfo->SettingsFile);
		AddResource(theGoodFonts,'STR#',4000,"\pGood Fonts");
		AddResource(theBadFonts,'STR#',5000,"\pBad Fonts");
	} else
		theBadFonts = Get1Resource('STR#',5000);

redoTheList:			

	numGoodFonts = *(short *)(*theGoodFonts);
	if ( numGoodFonts == 0 )
		doItAll = TRUE; // we haven't created a list of fonts yet
	else
		numBadFonts = *(short *)(*theBadFonts);

	AppendResMenu(theMenu,'FONT'); // add them all here
	numFontsInMenu = CountMItems(theMenu);
	if ( numFontsInMenu != numBadFonts + numGoodFonts ) {
		SetHandleSize(theGoodFonts,2);
		*((short *)*theGoodFonts) = 0; 
		SetHandleSize(theBadFonts,2);
		*((short *)*theBadFonts) = 0; 
		doItAll = TRUE;
	}
	if ( !doItAll ) {
		// we have a list already
		short goodFontIndex, badFontIndex;
		goodFontIndex = badFontIndex = 1;
		
		for ( menuIndex = 1; menuIndex <= numFontsInMenu; menuIndex++ ) {
			// look at each font, see if its bad, good, or new
			GetMenuItemText(theMenu,menuIndex,currentFontName);
			GetIndString(fontListName, 5000, badFontIndex);
			if ( EqualString(currentFontName, fontListName, TRUE, FALSE) ) {
// HACK: ASLFont was improperly rejected as proportional. now
// that isMonospacedFont() works with other scripts too, rebuild
// the list if we find it there.
				if ( !memcmp("\pASLFont+", fontListName, 9) ) {
					*((short *)*theGoodFonts) = 0; 
					*((short *)*theBadFonts) = 0; 
					goto redoTheMenu;
				}
//
				DeleteMenuItem(theMenu, menuIndex); //remove the bad font from the menu
				numFontsInMenu--; 
				menuIndex--;
				badFontIndex++;
			} else {
				GetIndString(fontListName, 4000, goodFontIndex);
				if ( EqualString(currentFontName, fontListName, TRUE, FALSE) ) {
					goodFontIndex++;
				} else {
					// its a new font, and we didn't expect it; this means there were an equal
					// number of fonts removed and added.  Let's start over.
					short numberInMenu;
redoTheMenu:
					numberInMenu = CountMItems(theMenu);
					for (;numberInMenu > 0; numberInMenu--)
						DeleteMenuItem(theMenu,numberInMenu); // clear the menu
					SetHandleSize(theGoodFonts,2); // clear the lists
					SetHandleSize(theBadFonts,2);
					
					goto redoTheList;
				}
			}
		}
	} else {
		// create a new list
		Str255 stemp;
//		DialogPtr 	dtemp;
//		dtemp = GetNewMyDialog(130, NULL, kInFront, (void *)SecondThirdCenterDialog);	/* opening dialog */
//		DrawDialog(dtemp);

		// first, open a temporary port to test CharWidth
		tempPort = (GrafPtr)myNewPtrCritical(sizeof(GrafPort));
		GetPort(&savedPort);
		OpenPort(tempPort);

		GetIndString(stemp, MISC_STRINGS, REBUILDING_FONT_MENU);
		SetTEText(startupBox, 3, stemp);

		for (menuIndex = 1; menuIndex <=  numFontsInMenu; menuIndex++) {
			GetMenuItemText(theMenu,menuIndex,currentFontName);
			if (!isMonospacedFont(currentFontName)) {
				DeleteMenuItem(theMenu,menuIndex); // remove the bad font from the menu
				numFontsInMenu--; 
				menuIndex--;
				PtrAndHand(currentFontName,theBadFonts,currentFontName[0]+1);// add it to the bad list
				(*(short *)(*theBadFonts))++;
			} else {
				PtrAndHand(currentFontName,theGoodFonts,currentFontName[0]+1);// add it to the good list
				(*(short *)(*theGoodFonts))++;
			}
		}

		SetPort(savedPort);
		ClosePort(tempPort);
		DisposePtr((Ptr)tempPort);

//		DisposeDialog(dtemp);	
	}	
	ChangedResource(theGoodFonts);
	ChangedResource(theBadFonts);
	ReleaseResource(theGoodFonts);
	ReleaseResource(theBadFonts);
	UpdateResFile(TelInfo->SettingsFile);
}

Boolean isMonospacedFont(Str255 theFont)
{
	Boolean haveNonRomanScripts,thisOneIsMonospaced = FALSE, doRomanTest = FALSE;
	short fond;
	long tempLong;


	tempLong = GetScriptManagerVariable(smEnabled);	//this returns number of scripts enable
    haveNonRomanScripts = (tempLong > 1); //if there is one, its roman
	GetFNum(theFont,&fond);
	if (haveNonRomanScripts) {
		long thisScriptEnabled;
		ScriptCode scriptNumber;
		scriptNumber = FontToScript(fond);
		if ( scriptNumber != 0 ) {
			// if its non-roman
			thisScriptEnabled = GetScriptVariable(scriptNumber,smScriptEnabled);
			if (thisScriptEnabled) {
				// check if this font is the preferred monospaced font for its script
				long theSizeAndFond;
				short thePreferredFond;
				theSizeAndFond = GetScriptVariable(scriptNumber, smScriptMonoFondSize);
				thePreferredFond = theSizeAndFond >> 16; // high word is fond
				if ( !(thisOneIsMonospaced = (thePreferredFond == fond)) )
					// not the default doesn't mean it is not proportional...
					doRomanTest = TRUE;
			} else
				thisOneIsMonospaced = FALSE; //this font's script isn't enabled
		} else
			doRomanTest = TRUE;
	} else
		doRomanTest = TRUE;
		
	if (doRomanTest) {
		TextFont(fond);
		thisOneIsMonospaced = (CharWidth('W') == CharWidth('.'));
	}
	return(thisOneIsMonospaced);
} 

void CheckOpSpecSubmenu(void)
{
//	short numItems;
//	UseResFile(TelInfo->SettingsFile);
//	numItems = Count1Resources(SESSIONPREFS_RESTYPE);
//	if ((CountMItems(myMenus[OpSpec]) - 2) != numItems)
		SetupOpSpecSubmenu(myMenus[OpSpec]);
}

void SetupOpSpecSubmenu(MenuHandle theMenu)
{
	short			scratchshort,numberofitems;
	LinkedListNode	*theHead;	
	scratchshort = (CountMItems(theMenu) - 2);
	for (; scratchshort>0; scratchshort--) DeleteMenuItem(theMenu, scratchshort + 2);
	
	UseResFile(TelInfo->SettingsFile);
	numberofitems = Count1Resources(SESSIONPREFS_RESTYPE);
	if (numberofitems)
	{	
		theHead = createSortedList(SESSIONPREFS_RESTYPE,numberofitems,"\p<Default>"); //now we have a sorted linked list of the names
		addListToMenu/*2*/(theMenu, theHead, 3);
		deleteList(&theHead);
	}
}

/*	updateMenuChecks() - update the check marks for file transfer
 *						 (MacBinary) */

void updateMenuChecks( void)
{
	if (TelInfo->MacBinary)
		CheckItem(myMenus[Fil],FLbin,TRUE);		/* Check MacBinary ... */
	else
		CheckItem(myMenus[Fil],FLbin,FALSE);
}

/*	DisplayMacBinary()	- Sets the macbinary check mark according to the MacBinary flag */
void DisplayMacBinary( void)
{
	if (TelInfo->MacBinary)
		CheckItem(myMenus[Fil],FLbin,TRUE);		/* Check MacBinary ... */
	else
		CheckItem(myMenus[Fil],FLbin,FALSE);
}

/*CheckFonts()		- Place checkmarks and outlines on the appropriate
 *						  menu items for the fonts */
void CheckFonts(void)
{
	short 	i, fsiz, fnum;
	long	itemFontSize;
	Str255 	temp, itemString;

	RSgetboldfont( screens[scrn].vs, &fnum);
	GetFontName(fnum, temp);

	if (TelInfo->checkedBoldFontNum)
		CheckItem(myMenus[FontOther], TelInfo->checkedBoldFontNum, FALSE);
	if (TelInfo->checkedFontNum)
		CheckItem(myMenus[Font], TelInfo->checkedFontNum, FALSE);

	TelInfo->checkedFontNum = 0;
	TelInfo->checkedBoldFontNum = 0;

	if (screens[scrn].cachedBoldFontNum) {
		CheckItem(myMenus[FontOther], screens[scrn].cachedBoldFontNum, TRUE);
		TelInfo->checkedBoldFontNum = screens[scrn].cachedBoldFontNum;
	}
	else for(i=1; i<= CountMItems( myMenus[FontOther]); i++)
		{
		GetMenuItemText( myMenus[FontOther], i, itemString);
		if (EqualString(temp, itemString, FALSE, FALSE)) {
			TelInfo->checkedBoldFontNum = i;
			screens[scrn].cachedBoldFontNum = i;
			CheckItem( myMenus[FontOther], i, TRUE);			/* Check the current font */
		}
//		else
//			CheckItem( myMenus[FontOther], i, FALSE);
		}

	RSgetfont( screens[scrn].vs, &fnum, &fsiz);
	GetFontName(fnum, temp);

	if (screens[scrn].cachedFontNum) {
		CheckItem(myMenus[Font], screens[scrn].cachedFontNum, TRUE);
		TelInfo->checkedFontNum = screens[scrn].cachedFontNum;
	}
	else for(i=1; i<= CountMItems( myMenus[Font]); i++)
		{
		GetMenuItemText( myMenus[Font], i, itemString);
		if (EqualString(temp, itemString, FALSE, FALSE)) {
			TelInfo->checkedFontNum = i;
			screens[scrn].cachedFontNum = i;
			CheckItem( myMenus[Font], i, TRUE);			/* Check the current font */
		}
//		else
//			CheckItem( myMenus[Font], i, FALSE);
		}
	
	for(i=1; i<=CountMItems( myMenus[Sizem]); i++) {
		GetMenuItemText( myMenus[Sizem], i, itemString);			/* JMB 2.6 -- Much safer to do it */
		StringToNum(itemString, &itemFontSize);			/*				this way! */

		if (fsiz == (short)itemFontSize)
			CheckItem( myMenus[Sizem], i, TRUE);			/* Check Our Current Size */
		else
			CheckItem( myMenus[Sizem], i, FALSE);

		if (RealFont( fnum, (short)itemFontSize))		/* Outline All Available REAL Sizes */
			SetItemStyle( myMenus[Sizem], i, outline);
		else
			SetItemStyle( myMenus[Sizem], i, 0);
		}
}

/*	applAbout - display the about dialog for the application.*/
void applAbout( void)
{
/*
	DialogPtr About;
	short itemhit;

	About=GetNewMyDialog( AboutDLOG, (Ptr) 0L,(WindowPtr) -1L, (void *)ThirdCenterDialog);
	if (About) {
		UItemAssign( About, 2, VersionNumberUPP);
		ModalDialog(NULL, &itemhit);
		DisposeDialog(About);
		}
*/
	extern void DoAboutBox();
	DoAboutBox();
}

short ReallyClose( short scrn)
{
	DialogPtr	dtemp;
	short		item;
	Str255		scratchPstring;
	
	if (!gApplicationPrefs->dontWarnOnClose) {
		setLastCursor(theCursors[normcurs]);

		GetWTitle(screens[scrn].wind, scratchPstring);
		ParamText(scratchPstring, NULL, NULL, NULL);
		
		dtemp = GetNewMyDialog( CloseDLOG, NULL, kInFront, (void *)ThirdCenterDialog);

		item = DLOGCancel +1;
		while (item> DLOGCancel)
			ModalDialog(DLOGwOK_CancelUPP, &item);

		DisposeDialog( dtemp);

		updateCursor(1);
		
		if (item == DLOGCancel) return(0);
	}
	return(1);
}

PicHandle RGtoPICT(short i)
{
	GrafPtr		savePort;
	short		vs;
	short		j;
	PicHandle	tpic;
	Rect		trect;

	GetPort(&savePort);

	// get tek type and tekclear values
	vs = VGgetVS(i);
	if ( vs < 0 )
		return NULL;

	j = VGnewwin(TEK_DEVICE_PICTURE, vs, VGgettektype(vs), VGgettekclear(vs));
	if ( j >= 0 ) {
		SetRect( &trect, 0, 0, 384, 384 );
		RGMPsize( &trect );
		VGzcpy( i, j );				/* Love dat zm factr */
		RGsetwind( i );
		ClipRect(&trect);
		tpic = OpenPicture(&trect);
		gTekCopying = TRUE;
		VGredraw(i,j);
		ClosePicture();
		gTekCopying = FALSE;
		VGclose(j);
	}

	SetPort(savePort);
	return tpic;
}

/* 
 * copyGraph	- Copy the current graphics screen.
 *			dnum - the number of the drawing to copy .
 */
void copyGraph( short dnum)
{
	long tlong;							// Temporary Variable
	PicHandle tpic;						// Mental picture of the thing

	tpic = RGtoPICT(dnum);				// Get the picture
	if ( tpic != NULL ) {
		tlong=ZeroScrap();				// Nobody else can live here
		HLock((Handle) tpic);			// Lock it for Puting
		tlong=PutScrap(GetHandleSize((Handle) tpic),'PICT', (Ptr) *tpic);	// Store as a PICT
		HUnlock((Handle) tpic);			// Unlock so we can toss it
		KillPicture(tpic);				// Kill the picture.....
	}
}				

/* 
 * copyText	- Copy the current selection on the virtual screen
 *			vs - the number of the virtual screen to copy from
 */

void copyText( short vs)
{
	char **charh;			/* where to store the characters */
	long tlong;				/* Necessary temporary variable */

	tlong=ZeroScrap();		/* This Scrap aint big enough for the both of us */

	charh=RSGetTextSel(vs, 0, 1);		/* Get the text selection */

	if (charh == (char **)-1L)
		OutOfMemory(400);
	else if (charh != (char **)0L) {	/* BYU LSC - Can't do anything without characters */
		HLock(charh);				/* Lock for putting */
		tlong=PutScrap(GetHandleSize(charh),'TEXT',*charh);
									/* Put it as a TEXT resource */
		HUnlock(charh);				/* Unlock for disposal */
		DisposeHandle(charh);		/* Kill the chars */
		}
}

/* 
 * copyTable   - Copy the current selection on the virtual screen
 *			vs - the number of the virtual screen to copy from
 */

void copyTable( short vs)
{
	char **charh;			/* where to store the characters */
	long tlong;				/* Necessary temporary variable */

	tlong=ZeroScrap();		/* This Scrap aint big enough for the both of us */

	charh=RSGetTextSel(vs, gApplicationPrefs->CopyTableThresh, 1);		/* Get the text selection */

	if (charh>(char **)0L) {					/* BYU LSC - Can't do anything without characters */
		HLock(charh);				/* Lock for putting */
		tlong=PutScrap(GetHandleSize(charh),'TEXT',*charh);
									/* Put it as a TEXT resource */
		HUnlock(charh);				/* Unlock for disposal */
		DisposeHandle(charh);		/* Kill the chars */
		}
	else putln("No characters to copy darn it!");
}

/*
 *	paste - Paste the resource from the scrap into the current WIND, if
 *			and only if it is really text
 */

void paste( void)
{
	long
		off,				/* offset */
		length;				/* the lenght of what is on the Scrap */
	WindRec *tw = &screens[scrn];

	if (tw->clientflags & PASTE_IN_PROGRESS) {  // One paste at a time, please
		SysBeep(4);
		return;
	}

	if (GetScrap(0L, 'TEXT', &off)<=0L)		/* If there are no TEXT res's */
		return;						/* then we can't paste it */

	/* Flush the buffer if necessary */ //CCP fix for linemode
	kbflush(tw);

	tw->outhand = myNewHandle(0L);	/* create a handle to put chars in */
	length = GetScrap(tw->outhand, 'TEXT', &off);
	if ( GetTranslationIndex(tw->outnational) != kTRJIS ) {
		tw->outhand = htrbuf_mac_nat(tw, tw->outhand);
	}
	if ( tw->outhand && !(tw->outlen = GetHandleSize(tw->outhand)) ) {
		DisposeHandle( tw->outhand );
		tw->outhand = NULL;
	}
	if ( tw->outhand ) {
		HLock(tw->outhand);
		tw->outptr = *tw->outhand;
		tw->outlen = GetHandleSize(tw->outhand);
		tw->clientflags |= PASTE_IN_PROGRESS;
		tw->isUploading = 0;
		tw->incount = 0;
		tw->outcount = 0;
		pasteText( scrn);	/* BYU LSC - routine to paste to net, w/echo if neccessary */
	} else {
		// translation failed...
		SysBeep(5);
	}
}

void uploadFile(void) // RAB: routine added in BetterTelnet 1.0fc9
{
	long length;
	StandardFileReply sfr;
	OSErr err;
	short refNum;
	WindRec *tw = &screens[scrn];

	if (tw->clientflags & PASTE_IN_PROGRESS) {  // One paste at a time, please
		SysBeep(4);
		return;
	}

	StandardGetFile(0, -1, 0, &sfr);
	if (!sfr.sfGood) return;

	err = FSpOpenDF(&sfr.sfFile, fsCurPerm, &refNum);
	if (err) return;

	/* Flush the buffer if necessary */ //CCP fix for linemode
	kbflush(tw);

	length = 16384; // for now, upload block is 16K
	tw->outhand = myNewHandle(length);
	HLock( tw->outhand );
	FSRead(refNum, &length, *tw->outhand);
	if (length == 0) {
		FSClose(refNum);
		HUnlock(tw->outhand);
		DisposeHandle(tw->outhand);
		return;
	}
	SetHandleSize(tw->outhand, length); // now REALLY set the length
	if ( GetTranslationIndex(tw->outnational) != kTRJIS )
		tw->outhand = htrbuf_mac_nat(tw, tw->outhand);
	if ( tw->outhand && !(tw->outlen = GetHandleSize(tw->outhand)) ) {
		DisposeHandle( tw->outhand );
		tw->outhand = NULL;
	}
	if ( tw->outhand ) {
		HLock(tw->outhand);
		tw->outptr = *tw->outhand;
		tw->outlen = GetHandleSize(tw->outhand);
		tw->clientflags |= PASTE_IN_PROGRESS;
		if (length == 16384) {
			tw->isUploading = 1;
			tw->uploadRefNum = refNum;
		} else {
			FSClose(refNum);
		}
		tw->incount = 0;
		tw->outcount = 0;
		pasteText(scrn);	/* BYU LSC - routine to paste to net, w/echo if neccessary */
	} else {
		// translation failed...
		SysBeep(5);
	}
}


void autoPaste(short vs) // RAB: routine added in BetterTelnet 1.0fc6
{
	char **charh;
	WindRec *tw = &screens[scrn];

	if (tw->clientflags & PASTE_IN_PROGRESS) {  // One paste at a time, please
		SysBeep(4);
		return;
		}

	/* Flush the buffer if necessary */ //CCP fix for linemode
	kbflush(tw);

	charh = RSGetTextSel(vs, 0, 1);		/* Get the text selection */

	if (charh == (char **)-1L) {
		OutOfMemory(400);
	} else if (charh != (char **)0L) {	/* BYU LSC - Can't do anything without characters */
		if ( GetTranslationIndex(tw->outnational) != kTRJIS )
			tw->outhand = htrbuf_mac_nat(tw, charh);
		if ( tw->outhand && !(tw->outlen = GetHandleSize(tw->outhand)) ) {
			DisposeHandle( tw->outhand );
			tw->outhand = NULL;
		}
		if ( tw->outhand ) {
			HLock(tw->outhand);
			tw->outptr = *tw->outhand;
			tw->outlen = GetHandleSize(tw->outhand);
			tw->clientflags |= PASTE_IN_PROGRESS;
			tw->isUploading = 0;
			tw->incount = 0;
			tw->outcount = 0;
			pasteText( scrn);	/* BYU LSC - routine to paste to net, w/echo if neccessary */
		} else {
			// translation failed...
			SysBeep(5);
		}
	}
}


void displayStatus(short n)
{
	DialogPtr		dptr;
	short			item;
	Str255			scratchPstring,anotherString;
	
	setLastCursor(theCursors[normcurs]);
	
	switch(screens[n].active) {
		case CNXN_ISCORPSE:
			GetWTitle(screens[n].wind, scratchPstring);
			GetIndString(anotherString,MISC_STRINGS,AWAITING_DISMISSAL_STRING);
			ParamText( scratchPstring, anotherString, NULL, NULL);
			break;
		case CNXN_OPENING:
			GetIndString(anotherString,MISC_STRINGS,BEING_OPENED_STRING);
			ParamText( screens[n].machine,anotherString, NULL, NULL);
			break;
		default:
			GetIndString(anotherString,MISC_STRINGS,BEING_LOOKED_UP);
			ParamText( screens[n].machine,anotherString, NULL, NULL);
		}

	dptr = GetNewMyDialog( StatusDLOG, NULL, kInFront, (void *)ThirdCenterDialog);

	item = DLOGCancel+1;
	while (item > DLOGCancel)
		ModalDialog(DLOGwOK_CancelUPP, &item);

	updateCursor(1);
	
	if (item == DLOGCancel) {
		netclose(screens[n].port);
		destroyport( n);
		}	

	DisposeDialog(dptr);
}

/*
 *	changeport - handle the menu updates for changing from one port to another
 */
void changeport(short oldprt, short newprt)
{
	if (screens[oldprt].active == CNXN_ACTIVE) 
		CheckItem(myMenus[Conn], oldprt + FIRST_CNXN_ITEM, FALSE);		/* Adjust Conn menu */
	CheckItem(myMenus[Conn], newprt + FIRST_CNXN_ITEM, TRUE);
	CheckItem(myMenus[Emul], EMbs,FALSE);						/* Adjust BS */
	CheckItem(myMenus[Emul], EMdel,FALSE);
	CheckItem(myMenus[Emul], EMbs+screens[newprt].bsdel,TRUE);	/* and DEL */
	CheckItem(myMenus[Emul], EMcapture, VSiscapturing(screens[newprt].vs));
	CheckItem(myMenus[Emul], EMqprint, screens[newprt].qprint);
	CheckItem(myMenus[Emul], EMff, screens[newprt].ignoreff);
	CheckItem(myMenus[Emul], EMsuspend, screens[newprt].enabled);
	CheckItem(myMenus[Emul], EMecho, screens[newprt].echo);
	CheckItem(myMenus[Emul], EMwrap, screens[newprt].wrap);
	CheckItem(myMenus[Emul], EMcrmap, screens[newprt].crmap);
	CheckItem(myMenus[Emul], EMansi, screens[newprt].ANSIgraphics);
	CheckItem(myMenus[Emul], EMxterm, screens[newprt].Xterm);
	CheckItem(myMenus[Emul], EMeightbit, screens[newprt].eightbit);
	CheckItem(myMenus[Emul], EMmapkeypad, screens[newprt].keypadmap);
	CheckItem(myMenus[Emul], EMarrowmap, screens[newprt].arrowmap);
	CheckItem(myMenus[Emul], EMpgupdwn, screens[newprt].pgupdwn);
	CheckItem(myMenus[Emul], EMscroll, screens[newprt].ESscroll);
	if (screens[newprt].tektype < 0) {	// TEK is inhibited
		DisableItem(myMenus[Emul], EMclear);
		DisableItem(myMenus[Emul], EMpage);
	} else {
		EnableItem(myMenus[Emul], EMclear);
		EnableItem(myMenus[Emul], EMpage);
		CheckItem(myMenus[Emul], EMclear, screens[newprt].tekclear);
	}
	CheckItem(myMenus[Emul], EMmapd, screens[newprt].remapCtrlD);
	CheckItem(myMenus[Emul], EMbold, screens[newprt].allowBold);
	CheckItem(myMenus[Emul], EMboldcolor, screens[newprt].colorBold);
	CheckItem(myMenus[Emul], EMinverse, screens[newprt].inversebold);
	CheckItem(myMenus[Emul], EMbeep, screens[newprt].ignoreBeeps);
	CheckItem(myMenus[Emul], EMrealblink, screens[newprt].realBlink);
	
	scrn = newprt;

	CheckFonts();

	if ( screens[newprt].outnational < 0 )
		CheckNational( gTableCount - screens[newprt].outnational );
	else
		CheckNational( screens[newprt].outnational );
}

// Returns TRUE if the user cancelled the quit
Boolean	HandleQuit(void)
{
	short	i;
	Boolean liveConnections = FALSE, die = TRUE;
	short	kcount = 0;
	
	if (TelInfo->numwindows>0) 
	{
		for(i = 0; i < MaxSess; i++)
		{
			if ((screens[i].active == CNXN_ACTIVE)||(screens[i].active == CNXN_OPENING))
				liveConnections = TRUE;
		}
		if (liveConnections)
			if (!gApplicationPrefs->dontWarnOnQuit)
				die = AskUserAlert(REALLY_QUIT_QUESTION, FALSE);
		if (die) 
		{
			for (i = TelInfo->numwindows - 1; i >= 0; i--) 
			{
				if (screens[i].authenticate)
					++kcount;
				netclose(screens[i].port);
				destroyport(i);
			}
		}
		else
			return (TRUE);
	 }
	if (gApplicationPrefs->destroyKTickets && kcount)
		DestroyTickets();
	quit();
	return (FALSE);
}

/*
 *	HandleMenuCommand - preform a command denoted by the arguments.
 *		mResult - the result of the menu event
 *		modifiers- modifiers from the menu event
 */

void HandleMenuCommand( long mResult, short modifiers)
{
	register short i;
	short theItem, theMenu;
	Boolean doWrap;
	Str255		scratchPstring;

	theMenu = mResult >> 16;
	theItem = mResult & 0xffff;
	switch(theMenu) {
	case appleMenu:
		if (theItem==1)				/* About Dialog */
			applAbout();
		else
		  {
			Str255
				name;
			GetMenuItemText(myMenus[0], theItem, name);		/* Desk accessory */
			OpenDeskAcc(name);
		  }
		break;

	case fileMenu:
	case NfileMenu:
		switch(theItem) {			
			
		case FLopen:
			PresentOpenConnectionDialog();					/* Open a connection */				
			break;

		case FLclose:									/* Close a connection */
			if (!FrontWindow()) break;	// RAB BetterTelnet 1.2 - bug fix
			if (!(modifiers &  optionKey))
				CloseAWindow(FrontWindow());
			else
			{
				Boolean die = TRUE;
				if (numberLiveConnections())
					die = AskUserAlert(CLOSE_ALL_WINDOWS_Q, FALSE);
				
				if (die) 
				{
					for (i = TelInfo->numwindows - 1; i >= 0; i--) 
					{
						netclose(screens[i].port);
						destroyport(i);
					}
				}
				else
					break;
			}
			break;

		case FLload:								/* Load a set */
			LoadSet();
			break;
		case FLsave:								/* Save a set */
			if (TelInfo->numwindows<1) break;
			SaveSet(0, (modifiers & optionKey));
			break;
		case FLsavem:
			if (TelInfo->numwindows<1) break;
			SaveSet(1, (modifiers & optionKey));
			break;
		case FLbin:									/* Toggle MacBinary on/off */
			TelInfo->MacBinary = !TelInfo->MacBinary;
			if (TelInfo->MacBinary) {
				CheckItem(myMenus[Fil], FLbin,TRUE);
				}
			else {
				CheckItem(myMenus[Fil], FLbin,FALSE);
				}
			break;

		case FLlog:										/* Toggle FTP window on/off*/
			TelInfo->ftplogon=!TelInfo->ftplogon;
			if(TelInfo->ftplogon) {
				CheckItem(myMenus[Fil],FLlog,TRUE);
				RSshow(ftplog->vs);
				SelectWindow(ftplog->wind);
				}
			else {
				CheckItem(myMenus[Fil],FLlog,FALSE);
				RShide(ftplog->vs);
				}
			break;

		case FLotp:
			otpinterface(0, 0, 0, 0, 0, 0, 0);
			break;

		case FLprint:								/* Print Selection (or gr) */
			PrintSelection();
			break;

		case FLprintScrn:							/* Print Screen */
			PrintScreen();
			break;

		case FLselect:
			SaveSelectionToFile();
			break;

		case FLupload:
			if (TelInfo->numwindows<1)
				break;
			uploadFile();
			break;

		case FLpset:									/* Set up for printer */
			PrintPageSetup();
			break;

		case FLquit:
			(void) HandleQuit();
			break;

		}
		break;

	case editMenu:
	case NeditMenu:
		if (!SystemEdit(theItem-1)) {				/* Is this mine? */
			switch(theItem) {
			case EDcut:
				if (TelInfo->macrosModeless)
				if (TelInfo->macrosModeless == FrontWindow()) {
					DialogCut(TelInfo->macrosModeless);
					ZeroScrap();
					TEToScrap();
					break;
				}
				break;
			case EDclear:
				if (TelInfo->macrosModeless)
				if (TelInfo->macrosModeless == FrontWindow()) {
					DialogDelete(TelInfo->macrosModeless);
					break;
				}
				break;
			case EDcopy:							/* Copy */
				if (TelInfo->macrosModeless)
				if (TelInfo->macrosModeless == FrontWindow()) {
					DialogCopy(TelInfo->macrosModeless);
					ZeroScrap();
					TEToScrap();
					break;
				}
				i = MacRGfindwind(FrontWindow());	/* is ICR window? */
				if (i >= 0)
					MacRGcopy(FrontWindow());		/* copy the ICR window */
				else {
					i = RGgetdnum(FrontWindow());
					if (i >= 0)						/* Copy Graphics */
						copyGraph( i);
					else							/* Copy Text */
						if ( (i=RSfindvwind(FrontWindow())) >= 0)
							copyText( i);
				}
				break;

			case EDcopyt:							/* Copy Table */
				/* 
				*  tech note #180 trick to get MultiFinder to pay attention 
				*/
				if (!SystemEdit(EDcopy-1)) {		/* tell it we did a copy */
					i=RGgetdnum(FrontWindow());
					if (i>-1)			/* Copy Graphics */
						copyGraph( i);
					else				/* Copy Text */
						if ( (i=RSfindvwind(FrontWindow())) >-1)
							copyTable( i);
				}
				break;

			case EDpaste:							/* Paste */
				if (TelInfo->macrosModeless)
				if (TelInfo->macrosModeless == FrontWindow()) {
					TEFromScrap();
					DialogPaste(TelInfo->macrosModeless);
					break;
				}
				if (TelInfo->numwindows<1)
						break;
					else paste();			/* Paste if there is a wind to do to*/
				break;
			case EDretype:
				if (TelInfo->numwindows<1) break;
				i = MacRGfindwind(FrontWindow());	/* is ICR window? */
				if (i >= 0)
					break;
				else {
					i=RGgetdnum(FrontWindow());
					if (i>-1)						/* Copy Graphics */
						break;
					else							/* Copy Text */
						if ( (i=RSfindvwind(FrontWindow())) >-1)
							autoPaste( i);
				}
				break;
			case EDmacros:							/* Set them Macros */
				Macros();
				break;
			case EDmacros+2:
				Cenviron();
				break;
			case EDmacros+5:
				Cftp();
				break;
			case EDmacros+4:
				EditConfigType(SESSIONPREFS_RESTYPE, &EditSession);
				CheckOpSpecSubmenu();
				break;
			case EDmacros+3:
				EditConfigType(TERMINALPREFS_RESTYPE, &EditTerminal);
				break;
			case EDmacros+6:
				EditConfigType(FTPUSERPREFS_RESTYPE, &EditFTPUser);

			default:
				break;
			}
		}
		break;

	/* Session */
	case termMenu:
	case NtermMenu:
		switch(theItem) {

		case EMbs:								/* Backspace for backspace  */
			if (TelInfo->numwindows<1) break;
			CheckItem(myMenus[Emul], EMbs+screens[scrn].bsdel,FALSE);
			screens[scrn].bsdel=0;
			CheckItem(myMenus[Emul], EMbs+screens[scrn].bsdel,TRUE);
			break;
		
		case EMdel:								/* Delete for backspace */
			if (TelInfo->numwindows<1) break;
			CheckItem(myMenus[Emul], EMbs+screens[scrn].bsdel,FALSE);
			screens[scrn].bsdel=1;
			CheckItem(myMenus[Emul], EMbs+screens[scrn].bsdel,TRUE);
			break;

		case EMscreensize:
			if (TelInfo->numwindows<1) break;					/* NCSA: SB */
			SetScreenDimensions((short)scrn, modifiers);		/* NCSA: SB */
			break;

		case EMsetup:							/* need dialog to enter new key values */
			setupkeys();
			break;

		case EMcolor:										/* Set color */
			if (TelInfo->numwindows<1) break;
			if (TelInfo->haveColorQuickDraw)
				RScprompt(screens[scrn].vs);	
			break;
		case EMAnsiColor:
			if (TelInfo->haveColorQuickDraw) {
				AnsiPrompt(0, 0);
				RSUpdatePalette();
			}
			return;

		case EMcapture:											/* BYU 2.4.18 - Capture session to file */
			if (VSiscapturing(screens[scrn].vs)) {				/* BYU 2.4.18 */
				CloseCaptureFile(screens[scrn].vs);				/* BYU 2.4.18 */
			} else {											/* BYU 2.4.18 */
				if(VSopencapture(scrn, screens[scrn].vs))		/* BYU 2.4.18 */
					CheckItem(myMenus[Emul], theItem,TRUE);	/* BYU 2.4.18 */
			}													/* BYU 2.4.18 */
			break;												/* BYU 2.4.18 */

		case EMqprint:
			screens[scrn].qprint = !screens[scrn].qprint;
			CheckItem(myMenus[Emul], theItem, screens[scrn].qprint);
			VSsetprintmode(screens[scrn].vs, screens[scrn].qprint);
			break;

		case EMff:
			screens[scrn].ignoreff = !screens[scrn].ignoreff;
			CheckItem(myMenus[Emul], theItem, screens[scrn].ignoreff);
			break;

		case EMsuspend:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].enabled = !screens[scrn].enabled;
			CheckItem( myMenus[Emul], theItem, !screens[scrn].enabled);
			break;

		case EMclrlines: // clear saved lines
			if (TelInfo->numwindows < 1) break;
			VSrealloc(screens[scrn].vs);
			/* FIXME: need to update scroll bars */
			break;

		case EMecho:								/* Toggle Local Echo (if poss.) */
			if (TelInfo->numwindows < 1) break;
			if ( screens[scrn].echo ) {
				kbflush(&screens[scrn]);
			}
			screens[scrn].echo = !screens[scrn].echo;	/* toggle */
			if (screens[scrn].echo) {					/* LOCAL ECHO */
				if (!(modifiers & optionKey) && (screens[scrn].protocol == 0))
					send_dont(screens[scrn].port,1);
			} else {										/* REMOTE ECHO */
				if (!(modifiers & optionKey) && (screens[scrn].protocol == 0))
					send_do(screens[scrn].port,1);
			}
			CheckItem(myMenus[Emul], theItem, screens[scrn].echo);
			break;
			
		case EMwrap:								/* wrap mode */
			if (TelInfo->numwindows < 1) break;
			if (!screens[scrn].wrap) {				/* is off, turn on */
				screens[scrn].wrap = 1;
				VSwrite(screens[scrn].vs, "\033[?7h",5);	/* kick emulator */
			} else {
				screens[scrn].wrap = 0;
				VSwrite(screens[scrn].vs, "\033[?7l",5);
			}
			CheckItem( myMenus[Emul], theItem, screens[scrn].wrap);
			break;
		
		case EMcrmap:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].crmap = !screens[scrn].crmap;
			CheckItem( myMenus[Emul], theItem, screens[scrn].crmap);
			break;

		case EMansi:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].ANSIgraphics = !screens[scrn].ANSIgraphics;
			CheckItem( myMenus[Emul], theItem, screens[scrn].ANSIgraphics);
			break;

		case EMxterm:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].Xterm = !screens[scrn].Xterm;
			CheckItem( myMenus[Emul], theItem, screens[scrn].Xterm);
			break;

		case EMeightbit:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].eightbit = !screens[scrn].eightbit;
			CheckItem( myMenus[Emul], theItem, screens[scrn].eightbit);
			break;

		case EMmapkeypad:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].keypadmap = !screens[scrn].keypadmap;
			CheckItem( myMenus[Emul], theItem, screens[scrn].keypadmap);
			break;

		case EMarrowmap:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].arrowmap = !screens[scrn].arrowmap;
			CheckItem( myMenus[Emul], theItem, screens[scrn].arrowmap);
			break;

		case EMpgupdwn:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].pgupdwn = !screens[scrn].pgupdwn;
			CheckItem( myMenus[Emul], theItem, screens[scrn].pgupdwn);
			break;
			
		case EMscroll:										/* Scrollback on CLS */
			if (TelInfo->numwindows<1) break;
			screens[scrn].ESscroll = !screens[scrn].ESscroll;
			VSscrolcontrol( screens[scrn].vs, -1, screens[scrn].ESscroll);
			CheckItem(myMenus[Emul], theItem, screens[scrn].ESscroll);
			break;

		case EMreset:									/* Reset Screen */
			//RESTORE WRAP AFTER THE RESET!!! BUGG
			if (TelInfo->numwindows<1) break;
			doWrap = screens[scrn].wrap;
			VSreset(screens[scrn].vs);					/* Reset it */
			screens[scrn].timing=0;
			if (doWrap)
				VSwrite(screens[scrn].vs, "\033[?7h",5);
			else
				CheckItem( myMenus[Emul],EMwrap, FALSE);
			break;

		case EMjump:									/* Jump Scroll */
			if (TelInfo->numwindows<1) break;
			FlushNetwork(scrn);							/* Flush it */
			break;
			
		case EMpage:									/* TEK page command */
			if (TelInfo->numwindows<1) break;
			parse( &screens[scrn],  (unsigned char *) "\033\014",2);	/* BYU LSC */
			break;

		case EMclear:								/* BYU 2.4.8 - Clear on TEK page */
			if (TelInfo->numwindows<1) break;
			screens[scrn].tekclear = !screens[scrn].tekclear;
			CheckItem(myMenus[Emul], theItem, screens[scrn].tekclear);
			break;

		case EMmapd:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].remapCtrlD = !screens[scrn].remapCtrlD;
			CheckItem( myMenus[Emul], theItem, screens[scrn].remapCtrlD);
			break;

		case EMbold:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].allowBold = !screens[scrn].allowBold;
			CheckItem( myMenus[Emul], theItem, screens[scrn].allowBold);
			RSchangebold(screens[scrn].vs, screens[scrn].allowBold, screens[scrn].colorBold, screens[scrn].inversebold);
			break;

		case EMboldcolor:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].colorBold = !screens[scrn].colorBold;
			CheckItem( myMenus[Emul], theItem, screens[scrn].colorBold);
			RSchangebold(screens[scrn].vs, screens[scrn].allowBold, screens[scrn].colorBold, screens[scrn].inversebold);
			break;

		case EMinverse:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].inversebold = !screens[scrn].inversebold;
			CheckItem( myMenus[Emul], theItem, screens[scrn].inversebold);
			RSchangebold(screens[scrn].vs, screens[scrn].allowBold, screens[scrn].colorBold, screens[scrn].inversebold);
			break;

		case EMbeep:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].ignoreBeeps = !screens[scrn].ignoreBeeps;
			CheckItem( myMenus[Emul], theItem, screens[scrn].ignoreBeeps);
			VSbeepcontrol(screens[scrn].vs, screens[scrn].ignoreBeeps);
			break;

		case EMrealblink:
			if (TelInfo->numwindows < 1) break;
			screens[scrn].realBlink = !screens[scrn].realBlink;
			CheckItem( myMenus[Emul], theItem, screens[scrn].realBlink);
			VSenableblink(screens[scrn].vs, screens[scrn].realBlink);
			break;

		default:
			break;
		}
		break;

	/* Net */
	case netMenu:
	case NnetMenu:
		switch(theItem) {
		case NEftp:										/* Send FTP command */
		case NEip:										/* Send IP Number */
			if (TelInfo->numwindows<1) break;
			{	char tmpout[30];						/* Basically the same except for */
				unsigned char tmp[4];					/* The ftp -n phrase in NEftp */

				if ( screens[scrn].echo ) {
					kbflush(&screens[scrn]);
				}
				netgetip(tmp);
				if (theItem == NEftp) {
					if ((gFTPServerPrefs->ServerState == 1) && !(modifiers & shiftKey))
						sprintf(tmpout,"ftp -n %d.%d.%d.%d\015\012",tmp[0],tmp[1],tmp[2],tmp[3]);
					else
						sprintf(tmpout,"ftp %d.%d.%d.%d\015\012",tmp[0],tmp[1],tmp[2],tmp[3]);
					}
				else
					sprintf(tmpout,"%d.%d.%d.%d",tmp[0],tmp[1],tmp[2],tmp[3]);
				netwrite(screens[scrn].port,tmpout,strlen(tmpout));
				if (screens[scrn].echo)
					VSwrite(screens[scrn].vs,tmpout, strlen(tmpout));
			}
			break;

		case NEayt:								/* Send "Are You There?"*/
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\366",2);
			break;

		case NEao:								/* Send "Abort Output"*/
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\365",2);
			screens[ scrn].timing = 1;						/* set emulate to TMwait */
			netwrite(screens[scrn].port, "\377\375\006",3);		/* send TM */
			break;

		case NEinter:								/* Send "Interrupt Process"*/
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\364",2);
			screens[ scrn].timing = 1;						/* set emulate to TMwait */
			netwrite(screens[scrn].port, "\377\375\006",3);		/* send TM */
			break;

		case NEbrk:
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\363",2); // IAC BRK
			break;

		case NEsync:
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netUrgent(); // This must be sent TCP Urgent.
			netwrite(screens[scrn].port, "\377\362",2); // IAC DM
			break;

		case NEipsync:
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\364", 2); // IAC IP
			netpush(screens[scrn].port);
			netUrgent(); // This must also be sent TCP Urgent.
			netwrite(screens[scrn].port, "\377\362", 2); // IAC DM
			netpush(screens[scrn].port);
			screens[scrn].timing = 1; // set emulate to TMwait
			netwrite(screens[scrn].port, "\377\375\006", 3); // send Timing Mark
			break;

		case NEeof: // End Of File
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\354", 2);
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\375\006", 3);
			break;

		case NEsusp:
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\355", 2);
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\375\006", 3);
			break;

		case NEabort: // Abort
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\356", 2);
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\375\006", 3);
			break;

		case NEec:								/* Send "Erase Character"*/
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\367",2);
			break;

		case NEel:								/* Send "Erase Line"*/
			if (TelInfo->numwindows<1) break;
			netpush(screens[scrn].port);
			netwrite(screens[scrn].port, "\377\370",2);
			break;
			
		case NEscroll:							/* Suspend Network */
			TelInfo->ScrlLock=!TelInfo->ScrlLock;
			if (TelInfo->ScrlLock) 
				CheckItem(myMenus[Net], NEscroll,TRUE);
			else 
				CheckItem(myMenus[Net], NEscroll,FALSE);
			break;

		case NEnet:								/* Show Network Numbers */
			showNetNumbers();
			break;

		default:
			break;
		}
		break;

	/* Favorites */
	case opspecMenu:											// JMB
		switch (theItem) {
			case 1:
				EditConfigType(SESSIONPREFS_RESTYPE, &EditSession);
				CheckOpSpecSubmenu();
				break;
			default:
				OpenPortSpecial(myMenus[OpSpec], theItem);
		}
		break;													// JMB

	/* Window */
	case connMenu:
	case NconnMenu:
		if (theItem == COnext) 
		{
			if (TelInfo->numwindows >1) 
			{
				short	scratchshort;
				if (!(modifiers &  shiftKey)) //go forward
				{
					scratchshort = WindowPtr2ScreenIndex(FrontWindow()) + 1;
					// Skip over inactive connections
					while(	(screens[scratchshort].active != CNXN_ACTIVE) &&
							(screens[scratchshort].active != CNXN_ISCORPSE) &&
							(scratchshort <= TelInfo->numwindows+1))						
						scratchshort++;
					if ((scratchshort < 0) || (scratchshort >= TelInfo->numwindows))
						scratchshort = 0;
				}
				else //go backward
				{
					scratchshort = WindowPtr2ScreenIndex(FrontWindow()) - 1;
					// Skip over inactive connections
					while(	(screens[scratchshort].active != CNXN_ACTIVE) &&
							(screens[scratchshort].active != CNXN_ISCORPSE) &&
							(scratchshort >= 0))						
						scratchshort--;
					if ((scratchshort < 0) || (scratchshort >= TelInfo->numwindows))
						scratchshort = TelInfo->numwindows - 1;
				}		
				SelectWindow(screens[scratchshort].wind);
			}
			break;
		}
		
		if ( theItem == COshowlog ) {
			DebugKeys(TRUE, 39, -1);
			break;
		}

		if (theItem == COtitle)	{
			ChangeWindowName(FrontWindow());
			break;
		}
			
		if (theItem >= FIRST_CNXN_ITEM) {
			if ((theItem - FIRST_CNXN_ITEM-1)>(TelInfo->numwindows+1)) break;  /* rotten danish */
			if (screens[theItem - FIRST_CNXN_ITEM].active != CNXN_ACTIVE) {
				displayStatus(theItem - FIRST_CNXN_ITEM);	/* Tell them about it..... */
				break;
			}
			else {
				HiliteWindow(screens[scrn].wind, FALSE);
				changeport(scrn,(theItem - FIRST_CNXN_ITEM));
				if (!(modifiers &  optionKey)) SelectWindow(screens[scrn].wind);
				else HiliteWindow(screens[scrn].wind, TRUE);
			}
		}
		break;

	case fontMenu:
		if (TelInfo->numwindows>0) {
			short 	itemFontNum;
			Str255 	temp;
			
			GetMenuItemText( myMenus[Font], theItem, temp);
			GetFNum( temp, &itemFontNum);
			
			RSchangefont( screens[scrn].vs, itemFontNum, 0);
			screens[scrn].cachedFontNum = theItem;

			/* use the same font for bold */
			RSchangeboldfont( screens[scrn].vs, itemFontNum);
			screens[scrn].cachedBoldFontNum = theItem;

			CheckFonts();
			}
		break;
	case fontMenu2:
		if (TelInfo->numwindows>0) {
			short 	itemFontNum;
			Str255 	temp;
			
			GetMenuItemText( myMenus[FontOther], theItem, temp);
			GetFNum( temp, &itemFontNum);
			
			RSchangeboldfont( screens[scrn].vs, itemFontNum);
			screens[scrn].cachedBoldFontNum = theItem;
			CheckFonts();
			}
		break;
	case sizeMenu:
		if (TelInfo->numwindows>0) 
		{
			long	itemFontSize;
			short 	currentSize, junk;
			Str255 	temp;
			short 	numOfItems;
			
			numOfItems = CountMItems(myMenus[Sizem]);
			if (numOfItems == theItem) //use picked 'other...'
			{
				RSgetfont(screens[scrn].vs, &junk, &currentSize);	
				itemFontSize = SetOtherFontSize(currentSize);
			}
			else
			{
				GetMenuItemText( myMenus[Sizem], theItem, temp);	/* JMB 2.6 -- Much safer to do it */
				StringToNum(temp, &itemFontSize);		/*				this way! */
			}

			RSchangefont( screens[scrn].vs, -1, itemFontSize);
			CheckFonts();
		}
		break;
	case prefsMenu:
		switch(theItem) {
			case prfGlobal:
				Cenviron();
				break;
			case prfFTP:
				Cftp();
				break;
			case prfSess:
				EditConfigType(SESSIONPREFS_RESTYPE, &EditSession);
				CheckOpSpecSubmenu();
				break;
			case prfTerm:
				EditConfigType(TERMINALPREFS_RESTYPE, &EditTerminal);
				break;
			case prfFTPUser:
				EditConfigType(FTPUSERPREFS_RESTYPE, &EditFTPUser);
			}
		break;
	case transMenu:
		if (TelInfo->numwindows>0) {
			CheckNational(theItem-1);		// Set up the menu
			//transBuffer(screens[scrn].outnational, theItem-1);	// Translate the scrollback buffer
			switchintranslation(&screens[scrn], theItem-1, 0);
			switchouttranslation(&screens[scrn], theItem-1, 0);
			// and redraw the screen
			//VSredraw(screens[scrn].vs,0,0,VSmaxwidth(screens[scrn].vs),VSgetlines(screens[scrn].vs)-1);	/* LU */
			//screens[scrn].outnational = theItem-1;
			}
		break;
	case keyMenu:
		KeyMenu(theItem, modifiers & shiftKey);
		break;
	case funcMenu:
		FuncMenu(theItem, modifiers & shiftKey);
		break;
	default:
		break;
	
	}
	HiliteMenu(0);
  } /* HandleMenuCommand */

//	Take the user's new translation choice and make sure the proper tables exist to do 
//		the translations.  If there is a problem, return the default translation as the chouce.
void	CheckNational(short choice)
{
	short i;
			
	for(i=1; i<=(nNational+1);i++)
		if ((choice+1) == i)					/* Check the Current NatLang */
			CheckItem( myMenus[National], i, TRUE);
		else
			CheckItem( myMenus[National], i, FALSE);

}

/*
 *	adjustwindowcommands.
 */

static void adjustwindowcommands()
{
/*
	int i;
	int count = CountMenuItems(myMenus[Conn]);
	if (count > FIRST_CNXN_ITEM + 8 )
		count = FIRST_CNXN_ITEM + 8;
	for (i = FIRST_CNXN_ITEM; i <= count; ++i) {
		//SetItemCommand( myMenus[Conn], i, mark);
		SetItemCmd( myMenus[Conn], i, '1' + i - FIRST_CNXN_ITEM );
	}
*/
}

/*
 *	extractmenu - remove a connection from the menu.
 */

void extractmenu(short screen)
{
	DeleteMenuItem(myMenus[Conn], screen + FIRST_CNXN_ITEM);
	adjustwindowcommands();
	AdjustMenus();
}

/*
 *	addinmenu - add a connection's name to the menu in position pnum. (name is
 *		an str255 pointed at by temps).
 */

void addinmenu( short screen, Str255 temps, char mark)
{
	InsertMenuItem(myMenus[Conn], "\pDoh", (screen-1) + FIRST_CNXN_ITEM);
	SetMenuItemText(myMenus[Conn], screen + FIRST_CNXN_ITEM, temps);		// Avoid metas
	SetItemMark( myMenus[Conn], screen + FIRST_CNXN_ITEM, mark);
	adjustwindowcommands();
	AdjustMenus();
}


/*	Set the item mark for <scrn> to opening connection */
void SetMenuMarkToOpeningForAGivenScreen( short scrn)
{
	unsigned char c=0xa5;

	SetItemMark( myMenus[Conn], scrn + FIRST_CNXN_ITEM, c );
}

/*	Set the item mark for <scrn> to opened connection */
void SetMenuMarkToLiveForAGivenScreen( short scrn)
{
	SetItemMark( myMenus[Conn], scrn + FIRST_CNXN_ITEM, noMark);
	AdjustMenus();
}

void DoTheMenuChecks(void)
{
	short	active;
	short	windownum;

	if (gMovableModal) {
		return;
	}


	EnableItem( myMenus[Conn], 0 );
	EnableItem( myMenus[Conn], COshowlog);

	if (TelInfo->numwindows>0)
		{
//		EnableItem(myMenus[Conn], 0 );
		EnableItem( myMenus[Conn], COtitle );

		if (gApplicationPrefs->KeyPadAndFuncMenus) {			/* Baylor */
			EnableItem(myMenus[Keypad], 0);			/* Baylor */
			EnableItem(myMenus[Function], 0);		/* Baylor */
			}
		DrawMenuBar();
		}

	else 
		{
//		DisableItem(myMenus[Conn], 0 );
		DisableItem(myMenus[Conn], COtitle );

		if (gApplicationPrefs->KeyPadAndFuncMenus) {			/* Baylor */
			DisableItem(myMenus[Keypad], 0);		/* Baylor */
			DisableItem(myMenus[Function], 0);		/* Baylor */
			}
		DrawMenuBar();
		}


	active =0;
	
	for (windownum=0;windownum<TelInfo->numwindows;windownum++)
		if (screens[windownum].active == CNXN_ACTIVE) active++;
		
	if (active<2)	
		DisableItem(myMenus[Conn],COnext);
	else EnableItem(myMenus[Conn],COnext);

	if (!active)
		{
		DisableItem(myMenus[Edit],EDpaste);
		DisableItem(myMenus[Emul],0);
		DrawMenuBar();
		}
	else 
		{
		EnableItem(myMenus[Edit],EDpaste);
		EnableItem( myMenus[Emul],0);
		DrawMenuBar();
		}

	if (TelInfo->macrosModeless)
	if (FrontWindow() == TelInfo->macrosModeless) {
		EnableItem(myMenus[Edit],EDcut);
		EnableItem(myMenus[Edit],EDcopy);
		EnableItem(myMenus[Edit],EDpaste);
		EnableItem(myMenus[Edit],EDclear);
	}
}



/*--------------------------------------------------------------------------*/
/* SetupMenusForSelection													*/
/* If there is a selection on screen, then let the user copy and print.		*/
/* If not, then, oh well....just disable the menus and forget about it		*/
/* ...and to think that this good stuff USED to be in rsmac.c. 				*/
/* This is called from RSselect after the user clicks in the window, and 	*/
/* was moved here for modularity	- SMB									*/
/*--------------------------------------------------------------------------*/
void SetMenusForSelection (short selected)					/* NCSA: SB */	
{															/* NCSA: SB */
	if (!selected)											/* NCSA: SB */
		{													/* NCSA: SB */
		DisableItem(myMenus[Fil],FLprint);					/* NCSA: SB */
		DisableItem(myMenus[Fil],FLselect);
		DisableItem(myMenus[Edit],EDcopy);					/* NCSA: SB */
		DisableItem(myMenus[Edit],EDretype);				// RAB BetterTelnet 1.0fc6
		DisableItem(myMenus[Edit],EDcopyt);					/* NCSA: SB */
		}													/* NCSA: SB */
	else 													/* NCSA: SB */
		{													/* NCSA: SB */
		EnableItem(myMenus[Fil],FLprint);					/* NCSA: SB */
		EnableItem(myMenus[Fil],FLselect);
		EnableItem(myMenus[Edit],EDcopy);					/* NCSA: SB */
		EnableItem(myMenus[Edit],EDretype);					// RAB BetterTelnet 1.0fc6
		EnableItem(myMenus[Edit],EDcopyt);					/* NCSA: SB */
		}													/* NCSA: SB */
}															/* NCSA: SB */


long SetOtherFontSize(short currentSize)
{
	DialogPtr	dtemp;
	Str255 currentSizeStr, newSizeStr;
	long newSize;
	Boolean GoodValue;
	short ditem;
	
	SetUpMovableModalMenus();
	dtemp=GetNewMyDialog( OtherFontDLOG, NULL, kInFront, (void *)ThirdCenterDialog); 
	InitCursor();

	GoodValue = 0;
	
	while (!GoodValue)
	{
		GoodValue = TRUE;
		NumToString((long) currentSize, currentSizeStr);
		SetTEText(dtemp, FontSizeTE, currentSizeStr);
	
		ditem = 0;
		while(ditem != DLOGOk && ditem != DLOGCancel) {
			//ModalDialog(DLOGwOK_CancelUPP, &ditem);
			movableModalDialog(DLOGwOK_CancelUPP, &ditem);
		}

		if (ditem == DLOGCancel) {
			DisposeDialog( dtemp);
			ResetMenus();
			return currentSize;
		}
	
		GetTEText(dtemp, FontSizeTE, newSizeStr);
		StringToNum(newSizeStr, &newSize);
	
		if (newSize < 4)
			GoodValue = FALSE;
		
		if (!GoodValue) SysBeep(4);
	}
	DisposeDialog( dtemp);
	ResetMenus();
	return (newSize);
}
/*----------------------------------------------------------------------*/
/* NCSA: SB - SetColumnWidth											*/
/*	Allow the user to FINALLY pick how many columns he wants on the 	*/
/*	screen.  Set up a dialog box to pick the # of columns, and then		*/
/*	size-up the Telnet screen accordingly.  NOTE: The user still needs	*/
/* 	to do a "resize", unless he is using NAWS							*/
/*----------------------------------------------------------------------*/
void SetScreenDimensions(short scrn, short modifiers)
{
	DialogPtr	dtemp;
	Str255		ColumnsSTR, LinesSTR;
	long		columns, lines;
	short		ditem, notgood;
	short		oldlines;
	short		oldcols;
	
	SetUpMovableModalMenus();
	dtemp=GetNewMyDialog( SizeDLOG, NULL, kInFront, (void *)ThirdCenterDialog); 
	
	setLastCursor(theCursors[normcurs]);

	notgood = 1;
	lines = VSgetlines(screens[scrn].vs);
	columns = VSgetcols(screens[scrn].vs) + 1;
	
	while (notgood) {
		notgood = 0;								/* Default to good */
		NumToString(columns, ColumnsSTR);
		NumToString(lines, LinesSTR);
		SetTEText(dtemp, ColumnsNumber, ColumnsSTR);
		SetTEText(dtemp, LinesNumber, LinesSTR);
		SelectDialogItemText( dtemp, ColumnsNumber, 0, 32767);
	
		ditem = 3;
		while(ditem>2) {
			/*ModalDialog(DLOGwOK_CancelUPP, &ditem);*/
			movableModalDialog(DLOGwOK_CancelUPP, &ditem);
		}

		if (ditem == DLOGCancel) {
			DisposeDialog( dtemp);
			ResetMenus();
			return;
			}
		
		GetTEText(dtemp, ColumnsNumber, ColumnsSTR);
		StringToNum(ColumnsSTR, &columns);
		GetTEText(dtemp, LinesNumber, LinesSTR);
		StringToNum(LinesSTR, &lines);
		
		if (columns < 10) {
			columns = 10;
			notgood = 1;
			}
/* NONO */
/*
		else if (columns > 132) {
			columns = 132;
			notgood = 1;
			}
*/
		else if (columns > 255) {
			columns = 255;
			notgood = 1;
			}
/* NONO */
		if (lines < 10) {
			lines = 10;
			notgood = 1;
			}
		else if (lines > 200) {
			lines = 200;
			notgood = 1;
			}
		
		if (notgood) SysBeep(4);
	}
	DisposeDialog( dtemp);
	ResetMenus();

	oldlines = VSgetlines(screens[scrn].vs);
	oldcols = VSgetcols(screens[scrn].vs);
	
	if (VSsetlines( screens[scrn].vs, lines) < 0) 
		OutOfMemory(-4);
	else
	{
		RScalcwsize( screens[scrn].vs, columns);
		if ((screens[scrn].naws) && !(modifiers & optionKey)) {
			SendNAWSinfo(&screens[scrn], (short)columns, (short)lines);
		}
/* NONO */
		if ( oldlines != VSgetlines(screens[scrn].vs)
		  || oldcols != VSgetcols(screens[scrn].vs) ) {
			ssh_glue_wresize(&screens[scrn]);
		}
/* NONO */
 	}
	updateCursor(1);
}

void	ChangeWindowName(WindowPtr	theWindow)
{
	DialogPtr	dptr;
	short		itemHit;
	Str255		theName;

	if( theWindow != NULL) {
		InitCursor();
		SetUpMovableModalMenus();
		dptr = GetNewMySmallDialog(WinTitlDLOG, NULL, kInFront, (void *)ThirdCenterDialog );

		GetWTitle(theWindow, theName); 
		SetTEText( dptr, kWinNameTE, theName);
		SelectDialogItemText( dptr, kWinNameTE, 0, 250 );

		ShowWindow(dptr);
		itemHit = 0;
		while(itemHit != DLOGOk && itemHit != DLOGCancel) {
			//ModalDialog(DLOGwOK_CancelUPP, &itemHit);
			movableModalDialog(DLOGwOK_CancelUPP, &itemHit);
		}

		if(itemHit == DLOGOk) {
			GetTEText(dptr, kWinNameTE, theName);
			set_new_window_name(theName, theWindow);
			}
		DisposeDialog(dptr);
		ResetMenus();
		}
}

void	set_new_window_name(Str255 theName, WindowPtr theWindow)
{
    short	i;
    
	if(theName[0]) {
		i = WindowPtr2ScreenIndex(theWindow);
		if (i >= 0) {
			i += FIRST_CNXN_ITEM;
			SetWTitle(theWindow, theName);
			SetMenuItemText(myMenus[Conn], i, theName);
			}
		}
}

void OpenSpecial(short theItem) {

	OpenPortSpecial(myMenus[OpSpec], theItem);

}

void SaveSelectionToFile(void) {
	short i;

	if (TelInfo->numwindows<1) return;
	i = MacRGfindwind(FrontWindow());	/* is ICR window? */
	if (i >= 0)
		return;
	else {
		i=RGgetdnum(FrontWindow());
		if (i>-1)						/* Copy Graphics */
			return;
		else							/* Copy Text */
			if ( (i=RSfindvwind(FrontWindow())) >-1)
				SaveThisSelection( i);
	}
}

void SaveThisSelection(short vs) {
	char **charh;
	OSErr err;
	StandardFileReply sfr;
	short refNum, exist;
	long tempCount;

	charh=RSGetTextSel(vs, 0, 1);		/* Get the text selection */

	if (charh == (char **)-1L)
		OutOfMemory(400);
	else if (charh != (char **)0L) {	/* BYU LSC - Can't do anything without characters */
		HLock(charh);				/* Lock for putting */
		StandardPutFile("\p", "\puntitled", &sfr);
		if (sfr.sfGood) {

			if ((err = FSpCreate(&sfr.sfFile, gApplicationPrefs->CaptureFileCreator, 'TEXT', sfr.sfScript)) == dupFNErr)
				exist = 1;

			err = FSpOpenDF(&sfr.sfFile, fsWrPerm, &refNum);

			if (exist) 
				SetEOF(refNum, 0L);

			tempCount = GetHandleSize(charh);
			err = FSWrite(refNum, &tempCount, *charh);
			FSClose(refNum);
		}

		HUnlock(charh);				/* Unlock for disposal */
		DisposeHandle(charh);		/* Kill the chars */
	}
}
//----------------------------------------------------------------------------------
//	File		: movableModal.c
//	Date		: April 4, 1994
//	Author		: Jim Stout
//	Purpose		: Implements movableModal dialogs
//------------------------------------------------------------------------------------

// This was originally written by Jim Stout (of Jim's CDEFs fame), but was extensively
// hacked by Rolf Braun for BetterTelnet 1.2. The most important change is that
// the new version of movableModal.c now calls StdFilterProc() instead of doing
// its own filtering.

#include "movableModal.h"
#include "menuseg.proto.h"
#include "tekrgmac.proto.h"
#include "rsinterf.proto.h"
#include "event.proto.h"
#include "netevent.proto.h"

extern void ssh2_sched();

extern MenuHandle myMenus[];

int gSetUpMovableModal = 0;
int gMovableModal = 0;

pascal void movableModalDialog(ModalFilterUPP filter, short *theItem)
{
	EventRecord theEvent;
	DialogPtr	d,thisDialog;
	GrafPtr		oldPort;
	WindowPeek	front;
	WindowPtr	dialog;
	
	thisDialog = FrontWindow();
	GetPort(&oldPort);
	SetPort(thisDialog);
	
	++gMovableModal;
	for(;;) {

		/* ssh2_sched can process terminal opening... */
		if (thisDialog != FrontWindow()) {
			SelectWindow(thisDialog);
		}

		WaitNextEvent(everyEvent, &theEvent, gApplicationPrefs->TimeSlice, 0L);

		DoNetEvents();

		ssh2_sched();

		SetPort(thisDialog);

		if ((*theItem = preFilterEvent(thisDialog, &theEvent)) != 0)
			break;
		
		if (filter != nil) {
			if (CallModalFilterProc(filter, thisDialog, &theEvent, theItem))
				break;
		} else if (CallStdFilterProc(thisDialog, &theEvent, theItem))
			break;

		if (IsDialogEvent(&theEvent))
			if (DialogSelect(&theEvent, &d, theItem))
				break;
		
	}
	--gMovableModal;
	SetPort(oldPort);
}

// RAB BetterTelnet 1.2
// For some reason, StdFilterProc() doesn't seem to return a meaningful result.
// CallStdFilterProc() is a kludge that works around this.
short CallStdFilterProc(DialogPtr dptr, EventRecord *theEvent, short *ditem)
{
	*ditem = 0;
	StdFilterProc(dptr, theEvent, ditem);
	if (*ditem != 0) return -1;
	return 0;
}

short preFilterEvent(DialogPtr d, EventRecord *theEvent)
{
	short 		ret=0;
	
	switch (theEvent->what) {
		case mouseDown:
			ret = doMouseDialog(d,theEvent); 	/* handle drag etc. of dialog		*/
		break;
		case diskEvt:
			diskEvent(theEvent);
		break;
		case updateEvt:
			if ((DialogPtr)theEvent->message != d)
				HandleEvent(theEvent); // We really do need to handle update events...
		break;
		
		case keyDown:
		case autoKey:
		break;

		case kHighLevelEvent:
			/* close current dialog ??? */

			AEProcessAppleEvent(theEvent);
			
		break;

	}
	return (ret);
}

short doMouseDialog(DialogPtr d, EventRecord *theEvent)
{
	WindowPtr	theWindow;
	short		partCode, ret=0, theItem, theMenu;
	long		mResult;
	
	switch (partCode = FindWindow(theEvent->where,&theWindow)) {
		case inDrag:
			if(theWindow == d) {
				DragWindow(d, theEvent->where, &qd.screenBits.bounds);
				theEvent->what = nullEvent;
			} else SysBeep(1);
		break;

		case inMenuBar:
			mResult = MenuSelect(theEvent->where);
			HiliteMenu(0);
			theMenu = mResult >> 16;
			theItem = mResult & 0xFFFF;
			if ( theMenu == appleMenu ) {
				if (theItem == 1) {
					applAbout();
				} else {
					Str255 name;
					GetMenuItemText(myMenus[0], theItem, name);
					OpenDeskAcc(name);
				}
				break;
			}

			if ((theMenu != editMenu) && (theMenu != NeditMenu)) break;

			switch (theItem) {
				case EDcut:
				DialogCut(d);
				break;

				case EDcopy:
				DialogCopy(d);
				break;

				case EDpaste:
				DialogPaste(d);
				break;

				case EDclear:
				DialogDelete(d);
				break;

				default:
				break;
			}
		break;

		case inGoAway:
			if (TrackBox (theWindow, theEvent->where, partCode)) {
				ret = cancel;
				theEvent->what = nullEvent;
			}
		break;
		
/* add code if you need to deal with these mouseDown eventsÉ	*/

		case inGrow:
		break;
		case inZoomIn:
		case inZoomOut:
		break;
		case inContent:
			if(theWindow != d) {
				SysBeep(1);
			}
		break;
		default:
		break;
	}
	return(ret);
}

void diskEvent(EventRecord *theEvent)
{
	Point diskInitPt;

	if (HiWord (theEvent->message) != noErr) {
		diskInitPt.v = 120;
		diskInitPt.h = 100;
		DILoad ();
		(void) DIBadMount (diskInitPt, theEvent->message);
		DIUnload ();
		theEvent->what = nullEvent;
	}
}

void SetUpMovableModalMenus(void)
{
	if (++gSetUpMovableModal == 1) {
		DisableItem(myMenus[Fil], 0);
		DisableItem(myMenus[0], 1);
		DisableItem(myMenus[Emul], 0);
		DisableItem(myMenus[Conn], 0);
		DisableItem(myMenus[Net], 0);
		DisableItem(myMenus[OpSpec], 0);
		DisableItem(myMenus[Keypad], 0);
		DisableItem(myMenus[Function], 0);

		DisableItem(myMenus[Edit], EDundo);
		EnableItem(myMenus[Edit], EDcut);
		EnableItem(myMenus[Edit], EDcopy);
		EnableItem(myMenus[Edit], EDpaste);
		EnableItem(myMenus[Edit], EDclear);
		DisableItem(myMenus[Edit], EDcopyt);
		DisableItem(myMenus[Edit], EDretype);
		DisableItem(myMenus[Edit], EDmacros);
		DisableItem(myMenus[Edit], EDprefs+1);
		DisableItem(myMenus[Edit], EDprefs+2);
		DisableItem(myMenus[Edit], EDprefs+4);
		DisableItem(myMenus[Edit], EDprefs+5);

		DrawMenuBar();
		HiliteMenu(0);
	}
}

void ResetMenus(void)
{
	short i;

	if (--gSetUpMovableModal == 0) {
		EnableItem(myMenus[Fil], 0);
		EnableItem(myMenus[0], 1);
		EnableItem(myMenus[Net], 0);
		EnableItem(myMenus[OpSpec], 0);

		DisableItem(myMenus[Edit], EDcut);
		DisableItem(myMenus[Edit], EDclear);
		EnableItem(myMenus[Edit], EDcopyt);
		EnableItem(myMenus[Edit], EDretype);
		EnableItem(myMenus[Edit], EDmacros);
		EnableItem(myMenus[Edit], EDprefs+1);
		EnableItem(myMenus[Edit], EDprefs+2);
		EnableItem(myMenus[Edit], EDprefs+4);
		EnableItem(myMenus[Edit], EDprefs+5);

		AdjustMenus();
		DoTheMenuChecks();
		DrawMenuBar();

		if (!FrontWindow()) return;

		if ((i=RSfindvwind(FrontWindow()))>=0) 
		{
			if (RSTextSelected(i)) 
			{					
				EnableItem(myMenus[Edit],EDcopy);	
				EnableItem(myMenus[Edit],EDcopyt);		
			} 
			else 
			{								
				DisableItem(myMenus[Edit],EDcopy);		
				DisableItem(myMenus[Edit],EDcopyt);		
			}										
		} 
		else 
		{					
			if ( (i = RGgetdnum(FrontWindow())) >-1)
			{
				if (( i = RGgetVS( i)) >-1) 
				{
					EnableItem(myMenus[Edit],EDcopy);	// - enable copying 
					DisableItem(myMenus[Edit],EDcopyt);	
				}
			}
		}
	}
}

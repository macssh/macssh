/* Configure.c
*  New Configuration scheme (the death of config.tel)
*
*  Revisions:
*  8/92		Telnet 2.6:	Initial version.  Jim Browne
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// RAB BetterTelnet 2.0fc1 - seriously cleaned up large parts of this code
// (given that I had messed it up in previous versions :-)

#include "DlogUtils.proto.h"
#include "popup.h"
#include "popup.proto.h"
#include "configure.proto.h"
#include "netevent.proto.h"		/* For Stask proto */
#include "prefs.proto.h"
#include "bkgr.proto.h"			// For StartUpFTP proto
#include "menuseg.proto.h"
#include "macutil.proto.h"		
#include "rsinterf.proto.h"
#include "LinkedList.proto.h"
#include "Sets.proto.h"
#include "movableModal.h"
#include "wind.h"
#include "Connections.proto.h"
#include "sshglue.proto.h"
#include "errors.proto.h"
#include "macros.proto.h"
#include "translate.proto.h"

/* ssh2.c */
extern void clearcachedpassphrase();


static Boolean isACopy(Str255 theName);//looks for an occurance of "copy" in a string

static	pascal short TerminalModalProc( DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(TerminalModalProc, ModalFilter);
static	pascal short SessionModalProc(DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(SessionModalProc, ModalFilter);
static	pascal short MyDlogWListFilter( DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(MyDlogWListFilter, ModalFilter);

static	pascal void	listDitemproc(DialogPtr theDlg, short itemNo);
PROTO_UPP(listDitemproc, UserItem);

extern	FTPServerPrefs*		gFTPServerPrefs;
extern	ApplicationPrefs*	gApplicationPrefs;
extern	MenuHandle	myMenus[];
static	void ZeroNumOnly(void);
static	Boolean InNumOnly(short item);

/*static char configPassword[256], configPassword2[256];*/
static Str255 configPassword;
static Str255 configPassword2;

ConstStringPtr gDefaultName = "\p<Default>";

static LinkedListNode *currentHead;
static ListHandle currentList;
#define NUMONLYSIZE 8
short NumOnly[NUMONLYSIZE];		/* Items for which non-number keys are ignored */
								// NumOnly[0] is number of "safe" item to return

extern Handle oldMacros;
extern NewMacroInfo oldMacroIndexes;
extern short dialogPane;

extern Boolean encryptOK;				// true if des encryption routines available
extern Boolean authOK;					// true if authentication driver available


void	CONFIGUREunload(void) {}
void SetControlText(DialogPtr dptr, short item, Str255 text);			


void SetControlText(DialogPtr dptr, short item, Str255 text)			
{
	short	itemType;
	Handle	itemHandle;
	Rect	itemRect;

	GetDialogItem(dptr, item, &itemType, &itemHandle, &itemRect);
	SetControlTitle((ControlHandle)itemHandle, text);
}

void Cenviron( void)
{
	DialogPtr	dptr;
	short 		ditem, defaultBoldColor;
	Str255		scratchPstring;
	long 		scratchlong;
	OSType		newtype;
	PaletteHandle toSave;
	Boolean		tempBoolean;
	Boolean		cachePass;

	SetUpMovableModalMenus();
	dptr = GetNewMySmallStrangeDialog( PrefDLOG, NULL, kInFront, (void *)ThirdCenterDialog);	
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 2);
	SetDialogTracksCursor(dptr, 1);

	SetCntrl(dptr, PrefDClose, gApplicationPrefs->WindowsDontGoAway);
	SetCntrl(dptr, PrefStag, gApplicationPrefs->StaggerWindows);
	defaultBoldColor = gApplicationPrefs->defaultBoldColor;
	if (!gApplicationPrefs->StaggerWindows)
	{
		HideDialogItem(dptr,PrefStaggerOffset);
		HideDialogItem(dptr,PrefStaggerBY);
		tempBoolean = FALSE;
	}
	else
	{
		scratchlong = gApplicationPrefs->StaggerWindowsOffset;
		NumToString(scratchlong, scratchPstring);
		SetTEText(dptr, PrefStaggerOffset, scratchPstring);
		tempBoolean = TRUE;
	}
	if (!TelInfo->haveColorQuickDraw)
		HideDialogItem(dptr,PrefAnsiColor);
		
	SetCntrl(dptr, PrefCMDkey, gApplicationPrefs->CommandKeys);
	SetCntrl(dptr, PrefTMap, gApplicationPrefs->RemapTilde);
	SetCntrl(dptr, PrefBlink, gApplicationPrefs->BlinkCursor);
	SetCntrl(dptr, PrefKeyFuncMenus, gApplicationPrefs->KeyPadAndFuncMenus);
	SetCntrl(dptr, PrefNotifyUser, gApplicationPrefs->NotifyUser);
	SetCntrl(dptr, PrefBlockCursor, (gApplicationPrefs->CursorType == 0));
	SetCntrl(dptr, PrefUnderscoreCursor, (gApplicationPrefs->CursorType == 1));
	SetCntrl(dptr, PrefVerticalCursor, (gApplicationPrefs->CursorType == 2));
	SetCntrl(dptr, PrefDestroyTickets, gApplicationPrefs->destroyKTickets);
	SetCntrl(dptr, 30, gApplicationPrefs->autoOpenDialog);
	SetCntrl(dptr, 31, gApplicationPrefs->dontWarnOnQuit);
	SetCntrl(dptr, 42, gApplicationPrefs->dontWarnOnClose);
	SetCntrl(dptr, 32, gApplicationPrefs->autoOpenDefault);
	SetCntrl(dptr, 33, gApplicationPrefs->parseAliases);
	SetCntrl(dptr, 34, gApplicationPrefs->monospacedOut);

	SetCntrl(dptr, 35, gApplicationPrefs->clipTrailingSpaces);
	SetCntrl(dptr, 36, gApplicationPrefs->globalSavePass);
	SetCntrl(dptr, 37, !gApplicationPrefs->noLiveScrolling);

/* NONO */
	SetCntrl(dptr, 38, gApplicationPrefs->cachePassphrase);
	scratchlong = gApplicationPrefs->cacheDelay;
	NumToString(scratchlong, scratchPstring);
	SetTEText(dptr, 24, scratchPstring);
	if ( !gApplicationPrefs->cachePassphrase ) {
		GetIndString(scratchPstring, MISC_STRINGS, CACHE_PASSPHRASE_TITLE);
		SetControlText(dptr, 38, scratchPstring);
		HideDialogItem(dptr,24);
		HideDialogItem(dptr,40);
		cachePass = FALSE;
	} else {
		cachePass = TRUE;
	}
	if ( !TelInfo->haveKeyChain ) {
		HideDialogItem(dptr, 41);
	} else {
		SetCntrl(dptr, 41, gApplicationPrefs->useKeyChain);
	}
/* NONO */

	scratchPstring[0] = 4;
	BlockMoveData(&(gApplicationPrefs->CaptureFileCreator), &scratchPstring[1], sizeof(OSType));
	SetTEText(dptr, PrefCaptTE, scratchPstring);
	newtype = gApplicationPrefs->CaptureFileCreator;

	scratchlong = gApplicationPrefs->CopyTableThresh;
	NumToString(scratchlong, scratchPstring);
	SetTEText(dptr, PrefCTt, scratchPstring);
	SelectDialogItemText(dptr, PrefCTt, 0, 32767);

	NumToString(gApplicationPrefs->TimeSlice, scratchPstring);
	SetTEText(dptr, PrefTimeSlice, scratchPstring);
	
	scratchlong = (long) gApplicationPrefs->OpenTimeout;
	NumToString(scratchlong,scratchPstring);
	SetTEText(dptr,PrefOpenTime, scratchPstring);

	scratchlong = (long) gApplicationPrefs->SendTimeout;
	NumToString(scratchlong,scratchPstring);
	SetTEText(dptr,PrefSendTime, scratchPstring);

	ShowWindow(dptr);
	ditem=0;								/* initially no hits */
	while((ditem>2) || (ditem==0)) {	
		movableModalDialog(0,&ditem);
		switch(ditem) {
			case PrefDClose:
			case PrefCMDkey:
			case PrefTMap:
			case PrefBlink:
			case PrefNotifyUser:
			case PrefKeyFuncMenus:
			case PrefDestroyTickets:
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 41:
			case 42:
				FlipCheckBox( dptr, ditem);
				break;
			case PrefStag:
				FlipCheckBox( dptr, ditem);
				tempBoolean = !tempBoolean;
				if (!tempBoolean)
				{
					HideDialogItem(dptr,PrefStaggerOffset);
					HideDialogItem(dptr,PrefStaggerBY);
				}
				else
				{
					ShowDialogItem(dptr, PrefStaggerBY);
					ShowDialogItem(dptr, PrefStaggerOffset);
					scratchlong = gApplicationPrefs->StaggerWindowsOffset;
					NumToString(scratchlong, scratchPstring);
					SetTEText(dptr, PrefStaggerOffset, scratchPstring);
				}
				break;

			case 38:
				FlipCheckBox( dptr, ditem);
				cachePass = !cachePass;
				if (!cachePass)
				{
					GetIndString(scratchPstring, MISC_STRINGS, CACHE_PASSPHRASE_TITLE);
					SetControlText(dptr, 38, scratchPstring);
					HideDialogItem(dptr,24);
					HideDialogItem(dptr,40);
				}
				else
				{
					GetIndString(scratchPstring, MISC_STRINGS, CACHE_PASSPHRASE_FOR_TITLE);
					SetControlText(dptr, 38, scratchPstring);
					ShowDialogItem(dptr, 24);
					ShowDialogItem(dptr, 40);
				}
				break;

			case PrefBlockCursor:
				SetCntrl(dptr, PrefBlockCursor, 1);
				SetCntrl(dptr, PrefUnderscoreCursor, 0);
				SetCntrl(dptr, PrefVerticalCursor, 0);
				break;
			case PrefUnderscoreCursor:
				SetCntrl(dptr, PrefBlockCursor, 0);
				SetCntrl(dptr, PrefUnderscoreCursor, 1);
				SetCntrl(dptr, PrefVerticalCursor, 0);
				break;
			case PrefVerticalCursor:
				SetCntrl(dptr, PrefBlockCursor, 0);
				SetCntrl(dptr, PrefUnderscoreCursor, 0);
				SetCntrl(dptr, PrefVerticalCursor, 1);
				break;

			case PrefCaptCreat:
				GetApplicationType(&newtype);
				scratchPstring[0] = 4;
				BlockMoveData(&newtype, &scratchPstring[1], sizeof(OSType));
				SetTEText(dptr, PrefCaptTE, scratchPstring);
				break;
			case PrefAnsiColor:
				AnsiPrompt(1, &defaultBoldColor);
				break;
			default:
				break;
			} /* switch */
		}
	
	if (ditem==DLOGCancel) {
			DisposeDialog( dptr);
			ResetMenus();
			return;
			}

	gApplicationPrefs->defaultBoldColor = defaultBoldColor;

	GetTEText(dptr, PrefCTt, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	gApplicationPrefs->CopyTableThresh = (short) scratchlong;

	GetTEText(dptr, PrefTimeSlice, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
/* NONO */
	/*BoundsCheck(&scratchlong, 100, 3);*/
	BoundsCheck(&scratchlong, 100, 0);
/* NONO */
	gApplicationPrefs->TimeSlice = scratchlong;

	GetTEText(dptr,PrefOpenTime, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	gApplicationPrefs->OpenTimeout = (short) scratchlong;
	
	GetTEText(dptr,PrefSendTime, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	gApplicationPrefs->SendTimeout = (short) scratchlong;
	
	GetTEText(dptr, PrefCaptTE, scratchPstring);
	BlockMoveData(&scratchPstring[1], &(gApplicationPrefs->CaptureFileCreator), sizeof(OSType));
	GetTEText(dptr,PrefStaggerOffset, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	gApplicationPrefs->StaggerWindowsOffset = (short) scratchlong;

/* NONO */
	if ( gApplicationPrefs->cachePassphrase != cachePass ) {
		gApplicationPrefs->cachePassphrase = cachePass;
		if (!cachePass) {
			clearcachedpassphrase();
		}
	}
	GetTEText(dptr,24, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	gApplicationPrefs->cacheDelay = scratchlong;
	gApplicationPrefs->useKeyChain = GetCntlVal(dptr, 41);					
/* NONO */

	gApplicationPrefs->CommandKeys = GetCntlVal(dptr, PrefCMDkey);					
	gApplicationPrefs->WindowsDontGoAway = GetCntlVal(dptr, PrefDClose);
	gApplicationPrefs->RemapTilde =	GetCntlVal(dptr, PrefTMap);
	gApplicationPrefs->StaggerWindows = GetCntlVal(dptr, PrefStag);
	gApplicationPrefs->BlinkCursor = GetCntlVal(dptr, PrefBlink);
	gApplicationPrefs->KeyPadAndFuncMenus = GetCntlVal(dptr, PrefKeyFuncMenus);
	gApplicationPrefs->NotifyUser = GetCntlVal(dptr,PrefNotifyUser);
	gApplicationPrefs->destroyKTickets = GetCntlVal(dptr,PrefDestroyTickets);
	gApplicationPrefs->autoOpenDialog = GetCntlVal(dptr,30);
	gApplicationPrefs->dontWarnOnQuit = GetCntlVal(dptr,31);
	gApplicationPrefs->dontWarnOnClose = GetCntlVal(dptr,42);
	gApplicationPrefs->autoOpenDefault = GetCntlVal(dptr,32);
	gApplicationPrefs->parseAliases = GetCntlVal(dptr,33);
	gApplicationPrefs->monospacedOut = GetCntlVal(dptr,34);
	gApplicationPrefs->clipTrailingSpaces = GetCntlVal(dptr,35);
	gApplicationPrefs->globalSavePass = GetCntlVal(dptr,36);
	gApplicationPrefs->noLiveScrolling = !GetCntlVal(dptr,37);
	gApplicationPrefs->CursorType = (GetCntlVal(dptr, PrefUnderscoreCursor) == 1) + (2 *
									 (GetCntlVal(dptr, PrefVerticalCursor) == 1));
	
	if (TelInfo->haveColorQuickDraw)
	{
		RSUpdatePalette(); //reflects any ANSI change
									 
		UseResFile(TelInfo->SettingsFile);
		toSave = (PaletteHandle)Get1Resource('pltt', 10001);
		if (toSave == NULL)//this shouldnt happen; make a new copy
		{
			UseResFile(TelInfo->ApplicationFile);
			//toSave = (PaletteHandle)GetNewPalette(9999);
			toSave = (PaletteHandle)Get1Resource('pltt', 9999);
			DetachResource((Handle)toSave);
			UseResFile(TelInfo->SettingsFile);
			AddResource((Handle)toSave, 'pltt', 10001, "\pANSI Colors");//make the new resource
			UpdateResFile(TelInfo->SettingsFile);
		}
		CopyPalette(TelInfo->AnsiColors,toSave,0,0,16);
		ChangedResource((Handle)toSave);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)toSave);
	}
	switchMenus(gApplicationPrefs->CommandKeys, 1);
	AdjustMenus();
	DoTheMenuChecks();
	SaveAppPreferences();
	
	DisposeDialog( dptr);
	ResetMenus();
}


void Cftp(void)
{
	DialogPtr			dptr;
	short				ditem;
	Str255				scratchPstring;
	OSType				scratchOSType;
	SFReply				sfr;
	FInfo				fi;			
	Point				where = {100,100};
	
	SetUpMovableModalMenus();
	dptr = GetNewMySmallStrangeDialog( FTPDLOG, NULL, kInFront, (void *)ThirdCenterDialog);	

	SetCntrl(dptr, FTPServerOff, (gFTPServerPrefs->ServerState == 0));
	SetCntrl(dptr, FTPServerUnsecure, (gFTPServerPrefs->ServerState == 1));
	SetCntrl(dptr, FTPServerPswdPlease, (gFTPServerPrefs->ServerState == 2));
	SetCntrl(dptr, FTPShowFTPlog, gFTPServerPrefs->ShowFTPlog);
	SetCntrl(dptr, FTPrevDNS, gFTPServerPrefs->DNSlookupconnections);
	SetCntrl(dptr, FTPUseMacBinaryII, gFTPServerPrefs->UseMacBinaryII);
	SetCntrl(dptr, FTPResetMacBinary, gFTPServerPrefs->ResetMacBinary);
	SetCntrl(dptr, FTPISO, gFTPServerPrefs->DoISOtranslation);

	HideDialogItem(dptr, FTPrevDNS);		// Sometime later, perhaps.
	
	scratchPstring[0] = 4;

	BlockMoveData(&(gFTPServerPrefs->BinaryCreator), &scratchPstring[1], sizeof(OSType));
	SetTEText(dptr, FTPbincreatTE, scratchPstring);
	
	BlockMoveData(&(gFTPServerPrefs->BinaryType), &scratchPstring[1], sizeof(OSType));
	SetTEText(dptr, FTPbintypeTE, scratchPstring);
	SelectDialogItemText(dptr, FTPbintypeTE, 0, 32767);

	BlockMoveData(&(gFTPServerPrefs->TextCreator), &scratchPstring[1], sizeof(OSType));
	SetTEText(dptr, FTPtextcreatTE, scratchPstring);

	ShowWindow(dptr);
	ditem=0;									/* initially no hits */
	while((ditem>2) || (ditem==0)) {	
		/*ModalDialog(DLOGwOK_CancelUPP,&ditem);*/
		movableModalDialog(DLOGwOK_CancelUPP,&ditem);
		switch(ditem) {
			case	FTPShowFTPlog:
			case	FTPrevDNS:
			case	FTPUseMacBinaryII:
			case	FTPResetMacBinary:
			case	FTPISO:
				FlipCheckBox( dptr, ditem);
				break;

			case	FTPServerOff:
				SetCntrl(dptr, FTPServerOff, 1);
				SetCntrl(dptr, FTPServerUnsecure, 0);
				SetCntrl(dptr, FTPServerPswdPlease, 0);
				break;
			case	FTPServerUnsecure:
				SetCntrl(dptr, FTPServerOff, 0);
				SetCntrl(dptr, FTPServerUnsecure, 1);
				SetCntrl(dptr, FTPServerPswdPlease, 0);
				break;
			case	FTPServerPswdPlease:
				SetCntrl(dptr, FTPServerOff, 0);
				SetCntrl(dptr, FTPServerUnsecure, 0);
				SetCntrl(dptr, FTPServerPswdPlease, 1);
				break;

			case	FTPtextcreatorbutton:
				if (GetApplicationType(&scratchOSType)) {
					BlockMoveData(&scratchOSType, &scratchPstring[1], sizeof(OSType));
					SetTEText(dptr, FTPtextcreatTE, scratchPstring);
					}
				break;
			
			case	FTPbinexamplebutton:
				SFGetFile(where, NULL, NULL, -1, NULL, NULL, &sfr);
				if (sfr.good) {
					GetFInfo(sfr.fName, sfr.vRefNum, &fi);
					scratchPstring[0] = sizeof(OSType);
					BlockMoveData(&fi.fdCreator, &scratchPstring[1], sizeof(OSType));
					SetTEText(dptr, FTPbincreatTE, scratchPstring);
					BlockMoveData(&fi.fdType, &scratchPstring[1], sizeof(OSType));
					SetTEText(dptr, FTPbintypeTE, scratchPstring);
					}
				break;	
			default:
				break;
			} /* switch */
		} /* while */
	
	if (ditem==DLOGCancel) {
			DisposeDialog( dptr);
			ResetMenus();
			return;
			}

	gFTPServerPrefs->ServerState = GetCntlVal(dptr, FTPServerUnsecure) + 
		GetCntlVal(dptr, FTPServerPswdPlease) * 2;
	StartUpFTP();			// Make sure FTP server recognizes new mode.
	
	gFTPServerPrefs->ShowFTPlog = GetCntlVal(dptr, FTPShowFTPlog);
	gFTPServerPrefs->DNSlookupconnections = GetCntlVal(dptr, FTPrevDNS);
	gFTPServerPrefs->UseMacBinaryII = GetCntlVal(dptr, FTPUseMacBinaryII);
	gFTPServerPrefs->ResetMacBinary = GetCntlVal(dptr, FTPResetMacBinary);
	gFTPServerPrefs->DoISOtranslation = GetCntlVal(dptr, FTPISO);

	GetTEText(dptr, FTPbincreatTE, scratchPstring);
	BlockMoveData(&scratchPstring[1], &(gFTPServerPrefs->BinaryCreator), sizeof(OSType));
	
	GetTEText(dptr, FTPbintypeTE, scratchPstring);
	BlockMoveData(&scratchPstring[1], &(gFTPServerPrefs->BinaryType), sizeof(OSType));

	GetTEText(dptr, FTPtextcreatTE, scratchPstring);
	BlockMoveData(&scratchPstring[1], &(gFTPServerPrefs->TextCreator), sizeof(OSType));

	SaveFTPPreferences();
	
	DisposeDialog( dptr);
	ResetMenus();
}

Boolean GetApplicationType(OSType *type)
{
	SFTypeList types;
	SFReply sfr;
	FInfo fi;
	Point where;
	
	SetPt(&where, 100, 100);
	types[0] = 'APPL';
	SFGetFile(where, NULL, NULL, 1, types, NULL, &sfr);
	if (sfr.good) {
		GetFInfo(sfr.fName, sfr.vRefNum, &fi);
		BlockMoveData(&fi.fdCreator, type, sizeof(OSType));		// Copy the application creator type
		}
		
	return(sfr.good);
}


static Boolean GetCurrentSelection(ListHandle thelist, Point *theCell, StringPtr theString)
{
	Point 		tempCell;

	if ( theCell == NULL )
		theCell = &tempCell;
	SetPt(theCell, 0, 0);
	if ( LGetSelect(TRUE, theCell, thelist) ) {
		if ( theString != NULL ) {
			short length = 254;
			LGetCell(theString+1, &length, *theCell, thelist);
			theString[0] = (char)length;
		}
		return TRUE;
	}
	return FALSE;
}

static Boolean IsDefaultLabel(StringPtr theString)
{
	return EqualString( theString, gDefaultName, TRUE, FALSE );
}

static Boolean IsDefaultSelected(ListHandle thelist)
{
	Point 		theCell;
	Str255		ItemName;

	if (GetCurrentSelection(thelist, &theCell, ItemName)) {
		return IsDefaultLabel(ItemName);
	}
	return FALSE;
}



//	Our standard modal dialog filter with code for handling user items containing lists.
SIMPLE_UPP(MyDlogWListFilter, ModalFilter);
pascal short MyDlogWListFilter( DialogPtr dptr, EventRecord *evt, short *item)
{
	short key, iType;
	Handle iHndl;
	Rect iRect;
	Point scratchPoint;
	Cell tempCell;
	static unsigned long lastTime = 0;
	static char shortcut[] = {0,0,0,0,0,0,0,0,0,0,0};
	Str255 tempString;

	SetPort(dptr);
	if ((evt->what == keyDown)||(evt->what == autoKey)) 
	{
		key = evt->message & charCodeMask;
		//keyCode = (evt->message >>8) & 0xff ;
		if ((key == 0x0d) || (key == 0x03) || (key == 27)) { // 0x0d == return, 0x03 == enter, 27 = esc
// RAB BetterTelnet 1.2 - we let StdFilterProc handle this
//			*item = 1;				// ok
//			FlashButton(dptr, 1);
//			return(-1);
			return CallStdFilterProc(dptr, evt, item);
			}
		else if (evt->modifiers & cmdKey)
		{
			switch(evt->message & charCodeMask)
			{
				case 'e':
				case 'E':
					if (GetCurrentSelection(currentList, NULL, NULL)) {
						*item = kChange;
					} else {
						evt->what == nullEvent;
						return FALSE;
					}
					break;
				case 'r':
				case 'R':
					if ( GetCurrentSelection(currentList, NULL, tempString)
					  && !IsDefaultLabel(tempString) ) {
						*item = kRemove;
					} else {
						evt->what == nullEvent;
						return FALSE;
					}
					break;
				case 'n':
				case 'N':
					*item = kNew;
					break;
				case 'd':
				case 'D':
					if (GetCurrentSelection(currentList, NULL, NULL)) {
						*item = kDuplicate;
					} else {
						evt->what == nullEvent;
						return FALSE;
					}
					break;
				default:
					evt->what == nullEvent;
					return FALSE;
					break;
			}
			FlashButton(dptr, *item);
			return -1;

		}
		else // a normal key
		{
			char keyCode = (evt->message & keyCodeMask) >> 8;
 
			if (keyCode >=0x7B) //CCP: arrow keys
			{
				tempCell.h = tempCell.v = 0;
				switch(keyCode)
				{
					case 0x7E: //up
					case 0x7B: //left
						if (LGetSelect(TRUE,&tempCell,currentList))
						{
							LSetSelect(FALSE,tempCell,currentList);//unselect it
							tempCell.v--;
						}
						else
							tempCell.v = 0;
					break;
					case 0x7D: //down
					case 0x7C: //right
						if (LGetSelect(TRUE,&tempCell,currentList))
						{
							LSetSelect(FALSE,tempCell,currentList);//unselect it
							tempCell.v++;
						}
						else
							tempCell.v = (**currentList).dataBounds.bottom - 1;
					break;
					default:
						return (FALSE);
					break;
				}
				if (tempCell.v > (**currentList).dataBounds.bottom - 1)
					tempCell.v = (**currentList).dataBounds.bottom - 1;
				if (tempCell.v < 0)
					tempCell.v = 0;
				LSetSelect(TRUE,tempCell,currentList);//select it
				LAutoScroll(currentList);
				Hilite(dptr, kRemove, IsDefaultSelected(currentList) ? 255 : 0);
				Hilite(dptr, kChange, 0);
				Hilite(dptr, kDuplicate, 0);
				return(FALSE);
			}
			else if (currentHead)
			{
				//CCP go to right list item based on keystroke
				if ((TickCount() - lastTime) > GetCaretTime() )
					shortcut[0] = 0;
				lastTime = TickCount();
				shortcut[(shortcut[0]++) +1] = key;
				tempCell.h = tempCell.v = 0;
				if (LGetSelect(TRUE,&tempCell,currentList))
					LSetSelect(FALSE,tempCell,currentList);
				tempCell.v = findClosestNodeIndex(currentHead,(StringPtr)shortcut);
				LSetSelect(TRUE,tempCell,currentList);
				LAutoScroll(currentList);
				if (shortcut[0] == 10)
					shortcut[0] = 0;

				Hilite(dptr, kRemove, IsDefaultSelected(currentList) ? 255 : 0);
				Hilite(dptr, kChange, 0);
				Hilite(dptr, kDuplicate, 0);
			}	
		}
	}
	else if (evt->what == updateEvt) 
	{
		if ((WindowPtr)evt->message == dptr) 
		{
// RAB BetterTelnet 1.2 - we let StdFilterProc handle this
//			GetDialogItem(dptr,1,&iType,&iHndl,&iRect);	// ok
//			PenSize(3,3);
//			InsetRect(&iRect,-4,-4);
//			FrameRoundRect(&iRect,16,16);
		CallStdFilterProc(dptr, evt, item);
		return 0;
		}
	}
	else if (evt->what == mouseDown) 
	{
		GetDialogItem(dptr, kItemList, &iType, &iHndl, &iRect);
		scratchPoint = evt->where;
		GlobalToLocal(&scratchPoint);
		if (PtInRect(scratchPoint, &iRect)) 
		{
			ListHandle theList;
			*item = kItemList;
			if (LClick(scratchPoint, evt->modifiers, currentList)
			 && GetCurrentSelection(currentList, NULL, NULL)) {
				*item = kChange;
				FlashButton(dptr, kChange);
			}	
			return(-1);
		}
	}
	
	return CallStdFilterProc(dptr, evt, item);
}

Boolean isACopy(Str255 theName)
{
	Boolean yes=FALSE;
	char copyString[] = "copy";	
	p2cstr(theName);
	if (strstr((char *)theName,copyString))
		yes = TRUE;
	c2pstr((char *)theName);
	return(yes);
}
	

//	User Dialog Item Procedure for a list.  Assumes the Dialog's refcon contains the list
//	handle.
SIMPLE_UPP(listDitemproc, UserItem);
pascal void	listDitemproc(DialogPtr theDlg, short itemNo)
{
	short		itemType;
	Handle		ControlHndl;
	Rect		scratchRect;
	
	GetDialogItem(theDlg, itemNo, &itemType, &ControlHndl, &scratchRect);
	
	PenNormal();
	InsetRect(&scratchRect, -1, -1);
	FrameRect(&scratchRect);
	LUpdate(theDlg->visRgn, (ListHandle)GetWRefCon(theDlg));
}

void	BoundsCheck(long *value, long high, long low)
{
	if (*value > high) *value = high;
	if (*value < low) *value = low;
}



/*#define kSCListMods lNoNilHilite+lOnlyOne*/
#define kSCListMods (lNoNilHilite+lOnlyOne)

void	EditConfigType(ResType ConfigResourceType, Boolean (*EditFunction)(StringPtr))
{
	DialogPtr	dptr;
	Handle 		iHndl;
	short 		iType, length, ditem,numberofitems, index;
	Rect 		ListBox, ListBounds;
	Point 		cellSize, theCell;
	ListHandle	thelist;
	Handle		ItemResource;
	Str255		ItemName;
	LinkedListNode *theHead, *leader;

	SetUpMovableModalMenus();
	dptr = GetNewMySmallDialog(kItemDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 1);
	SetDialogTracksCursor(dptr, 1);

	ItemName[0] = '\0';
	switch ( ConfigResourceType ) {
		case SESSIONPREFS_RESTYPE:
			GetIndString(ItemName, MISC_STRINGS, TITLE_FAVORITES);
			break;
		case TERMINALPREFS_RESTYPE:
			GetIndString(ItemName, MISC_STRINGS, TITLE_TERMINALS);
			break;
		case FTPUSERPREFS_RESTYPE:
			GetIndString(ItemName, MISC_STRINGS, TITLE_FTPUSERS);
			break;
	}
	SetWTitle(dptr, ItemName);

	ditem = 3;

	GetDialogItem(dptr, kItemList, &iType, &iHndl, &ListBox);
	SetDialogItem(dptr, kItemList, iType, (Handle)listDitemprocUPP, &ListBox);
	
	ListBox.right -= 15;		// Make room for scrollbar
	SetRect(&ListBounds, 0,0,1,0);
	SetPt(&cellSize,(ListBox.right-ListBox.left),16);
	thelist = LNew(&ListBox, &ListBounds, cellSize, 0, (WindowPtr)dptr,0,0,0,1);
/* NONO: was wrong field */
/*	(*(thelist))->listFlags = kSCListMods;*/
	(*(thelist))->selFlags = kSCListMods;
	(*(thelist))->listFlags = lDoVAutoscroll;
/* NONO */
	currentList = thelist;
	SetWRefCon(dptr, (long)thelist);		// So the Ditem proc can find the list
	
	UseResFile(TelInfo->SettingsFile);
	numberofitems = Count1Resources(ConfigResourceType);
	if (numberofitems)
		theHead = createSortedList(ConfigResourceType,numberofitems,gDefaultName); //now we have a sorted linked list of the names
 	else
		theHead = NULL;
	leader = theHead;
	for(index = 1, theCell.v = 0, theCell.h = 0; index <= numberofitems;index++, theCell.v++) 
	{
		LAddRow(1,-1, thelist);
		HLock((Handle)leader->name);
		LSetCell(*(leader->name) +1, (*leader->name)[0], theCell, thelist);
		HUnlock((Handle)leader->name);
		leader = leader->next;
	}
	theCell.v = 0;
	theCell.h = 0;
	if (numberofitems) LSetSelect(1, theCell, thelist);
	LSetDrawingMode(1, thelist);

	/* item <default> can't be deleted */
	if ( IsDefaultSelected(thelist) ) {
		Hilite(dptr, kRemove, 255);
	}
	if (!GetCurrentSelection(thelist, NULL, NULL)) {
		Hilite(dptr, kRemove, 255);
		Hilite(dptr, kChange, 255);
		Hilite(dptr, kDuplicate, 255);
	}

	ShowWindow(dptr);

	currentHead = theHead; //let dialog filter know about the list
	while (ditem > 1) {
		movableModalDialog(MyDlogWListFilterUPP, &ditem);
		
		switch(ditem) 
		{
			case kRemove:
				if ( GetCurrentSelection(thelist, &theCell, ItemName)
				  && !IsDefaultLabel(ItemName) ) {
					deleteItem(&theHead,ItemName);//delete it from the linked list
					currentHead = theHead; //in case list is now empty
					LDelRow(1,theCell.v,thelist);
					theCell.v--;
					LSetSelect(TRUE,theCell,thelist);
					UseResFile(TelInfo->SettingsFile);
					ItemResource = Get1NamedResource(ConfigResourceType, ItemName);
					RemoveTaggedResource(ItemResource, ConfigResourceType, ItemName);
					ReleaseResource(ItemResource);
					UpdateResFile(TelInfo->SettingsFile);
				}
				break;
			
			case kChange:
				SetPt(&theCell, 0, 0);
				if (LGetSelect(TRUE, &theCell, thelist)) 
				{
					short newPosition;
					length = 254;
					LGetCell(ItemName+1, &length, theCell, thelist);
					ItemName[0] = (char)length;
					leader = findNode(theHead,ItemName);
					
					if ((*EditFunction)((StringPtr)&ItemName)) 
					{
						if (!(EqualString(ItemName,*leader->name,TRUE,FALSE)))//new name
						{
							DisposeHandle((Handle)leader->name);
							leader->name = NewString(ItemName);//set the new name
							newPosition = reSortElement(&theHead,leader);//resort
							LDelRow(1,theCell.v,thelist);//delete the old row
							theCell.v = newPosition; 
							LAddRow(1,newPosition,thelist); //add a new row
							LSetCell(&ItemName[1], ItemName[0],theCell, thelist);//write the name in
							LSetSelect(TRUE,theCell,thelist);
						}
					}
				}
			break;
			
			case kNew:		
				ItemName[0] = 0;					// Signal new shortcut
				if ((*EditFunction)((StringPtr)&ItemName)) 
				{
					short whereAt = createNodeAndSort(&theHead, ItemName);
					currentHead = theHead; //in case list was empty
					if (LGetSelect(TRUE, &theCell, thelist)) 
						LSetSelect(FALSE,theCell,thelist); //turn off old selection
					theCell.v = LAddRow(1, whereAt, thelist);
					LSetCell(ItemName+1, StrLength(ItemName), theCell, thelist);
					LSetSelect(TRUE,theCell,thelist); //turn on new selection	
				}
			break;
			
			case kDuplicate:
				if ( GetCurrentSelection(thelist, &theCell, ItemName) ) {
					Handle oldResource,newResource;
					unsigned char copyString[] = " copy";
					short resourceID;
					short index;
					LSetSelect(FALSE,theCell,thelist);
					leader = findNode(theHead,ItemName);
					UseResFile(TelInfo->SettingsFile);
					oldResource = GetNamedResource(ConfigResourceType,ItemName);
					if (!isACopy(ItemName))//add the copy extension
					{
						ItemName[0] += 5;
						BlockMoveData(copyString, &(ItemName[ItemName[0]-4]),5); 
					}
					getAUniqueName(theHead,ItemName);
					resourceID = UniqueID(ConfigResourceType);
					newResource = myNewHandle(GetHandleSize(oldResource));
					BlockMoveData(*oldResource,*newResource,GetHandleSize(newResource));
					AddResource((Handle)newResource, ConfigResourceType, resourceID, ItemName);
					UpdateResFile(TelInfo->SettingsFile);
					index = createNodeAndSort(&currentHead,ItemName);
					theCell.v = index; 
					LAddRow(1,index,thelist); //add a new row
					LSetCell(&ItemName[1], ItemName[0],theCell, thelist);//write the name in
					LSetSelect(TRUE,theCell,thelist);
					ReleaseResource((Handle)newResource);
					ReleaseResource((Handle)oldResource);
				}
			break;
			default:
			break;
		} // switch
		
		if ( GetCurrentSelection(thelist, &theCell, ItemName) ) {
			Hilite(dptr, kRemove, (IsDefaultLabel(ItemName)) ? 255 : 0);
			Hilite(dptr, kChange, 0);
			Hilite(dptr, kDuplicate, 0);
		} else {
			Hilite(dptr, kRemove, 255);
			Hilite(dptr, kChange, 255);
			Hilite(dptr, kDuplicate, 255);
		}
	} // while
	
	LDispose(thelist);
	DisposeDialog(dptr);
	ResetMenus();
	deleteList(&theHead);
}

void ZeroNumOnly(void)
{
	short i;
	
	for(i=0; i<NUMONLYSIZE; i++) NumOnly[i]=0;
}

Boolean InNumOnly(short item)
{
	short index = 1;
	
	while ((NumOnly[index] != item) && (NumOnly[index] != 0)) index++;
	
	return(NumOnly[index] == item);
}

short		NumberOfColorBoxes;
short		BoxColorItems[16];
RGBColor	BoxColorData[16];

SIMPLE_UPP(ColorBoxItemProc, UserItem);
pascal void	ColorBoxItemProc(DialogPtr theDlg, short itemNo)
{
	short		itemType, index = 0;
	Handle		ControlHndl;
	Rect		scratchRect;
	RGBColor	rgb;

	GetDialogItem(theDlg, itemNo, &itemType, &ControlHndl, &scratchRect);
	PenNormal();
	FrameRect(&scratchRect);
	InsetRect(&scratchRect, 1, 1);			// Why isn't this framing the boxes?
	
	GetForeColor(&rgb);

	while ((index < NumberOfColorBoxes) && (BoxColorItems[index] != itemNo)) index++;
	
	RGBForeColor(&BoxColorData[index]);		/* color corresponding to item */
	PaintRect(&scratchRect);

	RGBForeColor(&rgb);			/* restore previous foreground color */
}

SIMPLE_UPP(ColorBoxModalProc, ModalFilter);
pascal short ColorBoxModalProc( DialogPtr dptr, EventRecord *evt, short *item)
{
	short keyCode, key, iType, index;
	Handle	iHndl;
	Rect	iRect;
	
	if ((evt->what == keyDown) || (evt->what == autoKey)) {
		key = evt->message & charCodeMask;
		keyCode = (evt->message >>8) & 0xff;
		if ((key == 0x0d) || (key == 0x03)) { // 0x0d == return, 0x03 == enter
//			*item = DLOGOk;
//			return(-1);				/* BYU LSC - pascal doesn't like "1" as true */
			return CallStdFilterProc(dptr, evt, item);
			}
		if (((key == '.') && (evt->modifiers & cmdKey)) ||
			((key == 0x1b) && (keyCode == 0x35)) ) {	// 0x35 == escKey
//			*item = DLOGCancel;
//			return(-1);				/* BYU LSC - pascal doesn't like "1" as true */
			return CallStdFilterProc(dptr, evt, item);
			}

        if ((key < '0' || key > '9') && 
        	!(key == 0x08 || key == 0x09 || (key > 0x1B && key < 0x20))
        	&& InNumOnly(((DialogPeek)dptr)->editField + 1)) {
            	/* Reject non-numbers for this TE */
            	*item = NumOnly[0];	/* Kludge -- Return "safe" item */
				return(-1);
				}   
       	}
       	
	if (evt->what == updateEvt) {
		if ((WindowPtr)evt->message == dptr) {
// RAB BetterTelnet 1.2 - We let StdFilterProc handle this.
//			GetDialogItem(dptr,DLOGOk,&iType,&iHndl,&iRect);	// ok
//			PenSize(3,3);
//			InsetRect(&iRect,-4,-4);
//			FrameRoundRect(&iRect,16,16);
			for (index = 0; index < NumberOfColorBoxes; index++)
				ColorBoxItemProc(dptr, BoxColorItems[index]);		// update color boxes
			}
			CallStdFilterProc(dptr, evt, item);
	}
	
	Stask();
	return CallStdFilterProc(dptr, evt, item);
}

popup TPopup[] = {{TermFontPopup, (MenuHandle) 0, 1},
					{41, (MenuHandle) 0, 1},
					{0, (MenuHandle) 0, 0}};

SIMPLE_UPP(TerminalModalProc, ModalFilter);
pascal short TerminalModalProc( DialogPtr dptr, EventRecord *evt, short *item)
{
	if ( evt->what == mouseDown ) {
		short oldChoice = TPopup[0].choice;
		short result = PopupMousedown(dptr, evt, item);
		if ( result && *item == TPopup[0].item ) {
			TPopup[1].choice = TPopup[0].choice;
			DrawPopUp(dptr, TPopup[1].item);
		}
		return result;
	}
	return(ColorBoxModalProc(dptr, evt, item));
}

/* NONO */
static void CheckPortPopup( DialogPtr dptr, unsigned short port, short id )
{
	int i;
	Str255 scratchPstring;
	long scratchlong;

	i = 0;
	do {
		scratchPstring[0] = 0;
		GetIndString(scratchPstring, 2008, ++i);
		if ( scratchPstring[0] ) {
			StringToNum(scratchPstring, &scratchlong);
			if ((unsigned short)scratchlong == port) {
				break;
			}
		}
	} while (scratchPstring[0]);
	SetCntrl(dptr, id, i);
}
/* NONO */

SIMPLE_UPP(SessionModalProc, ModalFilter);
pascal short SessionModalProc(DialogPtr dptr, EventRecord *evt, short *item)
{
	short	key;
	short	tempLen, editField;
	short	start,end;
	Handle	hText;
	char	*pbuf;
	char	buffer[256];
	long	val;
	

//	if ((evt->what == keyDown) || (evt->what == autoKey))
//		if ((evt->message & charCodeMask) == ' ') {
//			*item = NumOnly[0];	/* Kludge -- Return "safe" item */
//			return(-1);
//			}

	editField = ((DialogPeek)dptr)->editField + 1;
/* NONO */
	if ((evt->what == keyDown || evt->what == autoKey) && editField > 0 ) {

		if ( editField == 51 || editField == 59 ) {
			if (!PasswordFilter(dptr, evt, (editField == 51) ? configPassword : configPassword2, 250)) {
				return TerminalModalProc(dptr, evt, item);
			}
		} else if ( editField == 81 || editField == 83 || editField == SessPort ) {
			int updt = 0;

			key = evt->message & charCodeMask;
			switch (key) {
				case '.':			// Cmd-.
					if (!(evt->modifiers & cmdKey))
						break;
				case '\r':			// Return
				case '\003':		// Enter
				case '\033':		// Esc
				case '\t':			// Tab
				case '\034':		// Left arrow
				case '\035':		// Right arrow
				case '\036':		// Up arrow
				case '\037':		// Down arrow
					return TerminalModalProc(dptr, evt, item);
				default:			// Everything else falls through to be dealt with
					break;			//	below
			}
			start = (**((DialogPeek)dptr)->textH).selStart;	// Get the current selection
			end = (**((DialogPeek)dptr)->textH).selEnd;
			hText = TEGetText(((DialogPeek)dptr)->textH);

			/* filter port number */
			buffer[0] = GetHandleSize(hText);
			BlockMoveData(*hText, buffer + 1, (unsigned char)buffer[0]);
			if (start != end) {					// If there's a selection, delete it
				DeleteRange(buffer,start,end);
				StringToNum((unsigned char *)buffer, &val);
				updt = 1;
			}
			if (key == '\010') {	// Backspace
				if (start != 0) {
					DeleteRange(buffer,start-1,start);	// Delete the character to the left
					StringToNum((unsigned char *)buffer, &val);
					updt = 1;
				}
			} else {
				if ((evt->modifiers & cmdKey)) {
					if (key == 'v' || key == 'V') {
						// get scrap
						OSErr err;
						if ((err = GetScrapHandle('TEXT', &hText)) == noErr) {
							int len = GetHandleSize(hText);
							if (len && len + *buffer <= 255) {
								int i;
								for (i = 0; i < len ; i++) {
									key = (*hText)[i];
									InsertChar(buffer,start++,key);
									StringToNum((unsigned char *)buffer, &val);
									if ( key < '0' || key > '9' || val > 65535 ) {
										SysBeep(5);
										break;
									}
									// Insert the real key into the buffer
									TEKey((*hText)[i], ((DialogPeek)dptr)->textH);
									updt = 1;
								}
							}
							DisposeHandle(hText);
							evt->what = nullEvent;
						}
					}
				} else {
					InsertChar(buffer,start,key);		// Insert the real key into the buffer
					StringToNum((unsigned char *)buffer, &val);
					if ( key < '0' || key > '9' || val > 65535 ) {
						SysBeep(5);
						evt->what = nullEvent;
					} else {
						updt = 1;
					}
				}
			}
			if (updt) {
				/* update popup */
				if (editField == 81) {
					CheckPortPopup(dptr, val, 88);
				} else if (editField == 83) {
					CheckPortPopup(dptr, val, 89);
				} else if (editField == SessPort) {
					CheckPortPopup(dptr, val, 90);
				}
			}
		}
	}
/* NONO */

	return TerminalModalProc(dptr, evt, item);
}

void ShowTermPanel(DialogPtr dptr, short panel)
{
	switch (panel) {
		case 1:
		ShowDialogItemRange(dptr, 11, 16);
		ShowDialogItemRange(dptr, 22, 23);
		ShowDialogItemRange(dptr, 25, 26);
		ShowDialogItemRange(dptr, 29, 30);
		ShowDialogItem(dptr, 32);
		ShowDialogItemRange(dptr, 45, 46);
		if ( GetCntlVal(dptr, TermType) != 1 ) {
			ShowDialogItem(dptr, 48);
		}
		//ShowDialogItem(dptr, 48);
		ShowDialogItem(dptr, 50);
		break;

		case 2:
		ShowDialogItemRange(dptr, 27, 28);
		ShowDialogItem(dptr, 31);
		ShowDialogItemRange(dptr, 33, 37);
		ShowDialogItem(dptr, TermMetaIsOff);
		break;

		case 3:
		ShowDialogItemRange(dptr, 19, 21);
		ShowDialogItemRange(dptr, 38, 42);
		DrawPopUp(dptr, 20);
		DrawPopUp(dptr, 41);
		ShowDialogItem(dptr, 44);
		break;

		case 4:
		ShowDialogItemRange(dptr, 3, 10);
		ShowDialogItem(dptr, 24);
		ShowDialogItem(dptr, 47);
		break;
	}
}

void HideTermPanel(DialogPtr dptr, short panel)
{
	switch (panel) {
		case 1:
		HideDialogItemRange(dptr, 11, 16);
		HideDialogItemRange(dptr, 22, 23);
		HideDialogItemRange(dptr, 25, 26);
		HideDialogItemRange(dptr, 29, 30);
		HideDialogItem(dptr, 32);
		HideDialogItemRange(dptr, 45, 46);
		HideDialogItem(dptr, 48);
		HideDialogItem(dptr, 50);
		break;

		case 2:
		HideDialogItemRange(dptr, 27, 28);
		HideDialogItem(dptr, 31);
		HideDialogItemRange(dptr, 33, 37);
		HideDialogItem(dptr, TermMetaIsOff);
		break;

		case 3:
		DrawBlank(dptr, 20);
		DrawBlank(dptr, 41);
		HideDialogItemRange(dptr, 19, 21);
		HideDialogItemRange(dptr, 38, 42);
		HideDialogItem(dptr, 44);
		break;

		case 4:
		HideDialogItemRange(dptr, 3, 10);
		HideDialogItem(dptr, 24);
		HideDialogItem(dptr, 47);
		break;
	}
}

Boolean EditTerminal(StringPtr PrefRecordNamePtr)
{
	DialogPtr		dptr;
	short			ditem, scratchshort, resourceID, i, currentPanel, newPanel;
	long			scratchlong;
	ResType			scratchResType;
	Boolean			IsNewPrefRecord, UserLikesNewColor,wasInAliasText;
	TerminalPrefs**	TermPrefsHdl;
	TerminalPrefs*	TermPrefsPtr;
	Str255			scratchPstring;
	RGBColor		scratchRGBcolor;
	Point			ColorBoxPoint;
	MenuHandle		WeNeedAFontMenuHandle, WeNeedAnotherFontMenuHandle;
	short			editField;

	SetUpMovableModalMenus();
	dptr = GetNewMySmallStrangeDialog(TermDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	ditem = 3;
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 2);
	SetDialogTracksCursor(dptr, 1);
	currentPanel = TelInfo->lastPanelTerminal + 1;

	SetCntrl(dptr, 43, currentPanel);

	WeNeedAFontMenuHandle = NewMenu(666, "\p");
	//get the fonts from the font menu in the menu bar
	scratchshort = CountMItems(myMenus[Font]);
	for (i = 1; i <= scratchshort; i++)
	{
		GetMenuItemText(myMenus[Font],i,scratchPstring);
/* NONO */
		//AppendMenu(WeNeedAFontMenuHandle,scratchPstring);
		AppendMenu( WeNeedAFontMenuHandle, "\pempty" );
		SetMenuItemText( WeNeedAFontMenuHandle, i, scratchPstring ); 
/* NONO */
	}

	TPopup[0].h = WeNeedAFontMenuHandle;

	WeNeedAnotherFontMenuHandle = NewMenu(667, "\p");
	//get the fonts from the font menu in the menu bar
	scratchshort = CountMItems(myMenus[Font]);
	for (i = 1; i <= scratchshort; i++)
	{
		GetMenuItemText(myMenus[Font],i,scratchPstring);
/* NONO */
		//AppendMenu(WeNeedAnotherFontMenuHandle,scratchPstring);
		AppendMenu( WeNeedAnotherFontMenuHandle, "\pempty" );
		SetMenuItemText( WeNeedAnotherFontMenuHandle, i, scratchPstring ); 
/* NONO */
	}

	TPopup[1].h = WeNeedAnotherFontMenuHandle;

	/* need to call PopupInit before ShowTermPanel(3) !!! */
	PopupInit(dptr, TPopup);

	HideTermPanel(dptr, 1);
	HideTermPanel(dptr, 2);
	HideTermPanel(dptr, 3);
	HideTermPanel(dptr, 4);

	if (PrefRecordNamePtr[0] != 0) {
		IsNewPrefRecord = FALSE;
		UseResFile(TelInfo->SettingsFile);
		TermPrefsHdl = (TerminalPrefs **)Get1NamedSizedResource(TERMINALPREFS_RESTYPE, PrefRecordNamePtr, sizeof(TerminalPrefs));
		if (IsDefaultLabel(PrefRecordNamePtr)) {
			HideDialogItem(dptr, TermNameStatText);
			HideDialogItem(dptr, TermName);
		}
	} else { //make sure we have a unique name
		TermPrefsHdl = GetDefaultTerminal();
		IsNewPrefRecord = TRUE;
		GetIndString(PrefRecordNamePtr, MISC_STRINGS, MISC_NEWTERM);
		getAUniqueName(currentHead,PrefRecordNamePtr);	
	}
	HLock((Handle) TermPrefsHdl);
	TermPrefsPtr = *TermPrefsHdl;
	SetTEText(dptr, TermName, PrefRecordNamePtr);
	SetCntrl(dptr, TermANSIEsc, TermPrefsPtr->ANSIgraphics);
	SetCntrl(dptr, TermXterm, TermPrefsPtr->Xtermsequences);
	SetCntrl(dptr, 37, TermPrefsPtr->remapCtrlD);
	SetCntrl(dptr, 38, TermPrefsPtr->allowBold);
	SetCntrl(dptr, 39, TermPrefsPtr->colorBold);
	SetCntrl(dptr, 42, TermPrefsPtr->boldFontStyle);
	SetCntrl(dptr, 44, TermPrefsPtr->realbold);
	SetCntrl(dptr, 45, TermPrefsPtr->oldScrollback);
	SetCntrl(dptr, 46, TermPrefsPtr->jumpScroll);
	SetCntrl(dptr, Termvtwrap, TermPrefsPtr->vtwrap);
//	SetCntrl(dptr, Termmeta, TermPrefsPtr->emacsmetakey);
	SetCntrl(dptr, TermMetaIsCmdCntrol, (TermPrefsPtr->emacsmetakey == 1));
	SetCntrl(dptr, TermMetaIsOption, (TermPrefsPtr->emacsmetakey == 2));
	SetCntrl(dptr, TermMetaIsCmd, (TermPrefsPtr->emacsmetakey == 3));
	SetCntrl(dptr, TermMetaIsOff, (TermPrefsPtr->emacsmetakey == 0));
	SetCntrl(dptr, Termarrow, TermPrefsPtr->emacsarrows);
	SetCntrl(dptr, TermMAT, TermPrefsPtr->MATmappings);
	SetCntrl(dptr, Termeightbit, TermPrefsPtr->eightbit);
	SetCntrl(dptr, Termclearsave, TermPrefsPtr->clearsave);
	SetCntrl(dptr, TermType, TermPrefsPtr->vtemulation + 1);

	SetCntrl(dptr, TermRemapKeypad, TermPrefsPtr->remapKeypad);
	SetCntrl(dptr, 47, TermPrefsPtr->realBlink);
	SetCntrl(dptr, 48, TermPrefsPtr->vt7bits);
	
	SetCntrl(dptr, 50, TermPrefsPtr->hideScrollBars);	
	
	scratchlong = (long)(TermPrefsPtr->vtwidth);
	NumToString(scratchlong, scratchPstring);
	SetTEText(dptr, TermWidth, scratchPstring);
	scratchlong = (short)(TermPrefsPtr->vtheight);
	NumToString(scratchlong, scratchPstring);
	SetTEText(dptr, TermHeight, scratchPstring);
	scratchlong = TermPrefsPtr->fontsize;
	NumToString(scratchlong, scratchPstring);
	SetTEText(dptr, TermFontSize, scratchPstring);
	scratchlong = (unsigned short)TermPrefsPtr->numbkscroll;
	NumToString(scratchlong, scratchPstring);
	SetTEText(dptr, TermScrollback, scratchPstring);
	SetTEText(dptr, TermAnswerback, TermPrefsPtr->AnswerBackMessage);

	for(scratchshort = CountMItems(TPopup[0].h); scratchshort; scratchshort--) {
		GetMenuItemText(TPopup[0].h, scratchshort, scratchPstring);
		if (EqualString(scratchPstring, (TermPrefsPtr->DisplayFont), TRUE, FALSE))
			TPopup[0].choice = scratchshort;
	}

	for(scratchshort = CountMItems(TPopup[1].h); scratchshort; scratchshort--) {
		GetMenuItemText(TPopup[1].h, scratchshort, scratchPstring);
		if (EqualString(scratchPstring, (TermPrefsPtr->BoldFont), TRUE, FALSE))
			TPopup[1].choice = scratchshort;
	}

	ZeroNumOnly();
	NumOnly[0] = TermSafeItem;
	NumOnly[1] = TermWidth; NumOnly[2] = TermHeight; NumOnly[3] = TermFontSize;
	NumOnly[4] = TermScrollback;
	NumOnly[5] = 0;
//	if (TermPrefsPtr->vtemulation == 0)
//	{
//		HideDialogItem(dptr,TermRemapKeypad);
//		HideDialogItem(dptr,TermMAT);
//	}

	if (TelInfo->haveColorQuickDraw) {
		for (scratchshort = 0, NumberOfColorBoxes = 4; scratchshort < NumberOfColorBoxes; scratchshort++) {
			BoxColorItems[scratchshort] = TermNFcolor + scratchshort;
			BlockMoveData(&(TermPrefsPtr->nfcolor) + scratchshort,
				&BoxColorData[scratchshort], sizeof(RGBColor));
			UItemAssign( dptr, TermNFcolor + scratchshort, ColorBoxItemProcUPP);
			}
		}
	else
		NumberOfColorBoxes = 0;		// B&W machine
		
	ColorBoxPoint.h = 0;			// Have the color picker center the box on the main
	ColorBoxPoint.v = 0;			// screen
		
	ShowTermPanel(dptr, currentPanel);

	if ( ((DialogPeek)dptr)->editField >= 0 ) {
		SelectDialogItemText(dptr, ((DialogPeek)dptr)->editField + 1, 0, 255);
	}

	ShowWindow(dptr);
	wasInAliasText = FALSE;
	while (ditem > 2) 
	{
		movableModalDialog(TerminalModalProcUPP, &ditem);
		if ((wasInAliasText)&&(ditem != TermName))
		{
			wasInAliasText = FALSE;
			GetTEText(dptr, TermName,scratchPstring);
			if (EqualString(scratchPstring,PrefRecordNamePtr,FALSE,FALSE))
				goto okayTermName;
			if (scratchPstring[0] == 0) {
				DialogPtr alertUser;
				short	alertDItem = 3;
				alertUser = GetNewMyDialog(kBadPrefNameDLOG, NULL, kInFront, (void *)ThirdCenterDialog);			
				while (alertDItem > 1)
				{
					ModalDialog(DLOGwOK_CancelUPP,&alertDItem);
					DisposeDialog(alertUser);
				}
				SetPort(dptr); //back to old dialog
				SetTEText(dptr,TermName,PrefRecordNamePtr);//set the string back to the original
				SelectDialogItemText(dptr,TermName,0,32767);
				ditem = TermName; //so that if they hit ok or cancel, it doesn't kill the window
			}
			else if(findNodeNC(currentHead,scratchPstring))  //we already have that name
			{
				DialogPtr alertUser;
				short	alertDItem = 3;
				alertUser = GetNewMyDialog(kBadPrefNameDLOG, NULL, kInFront, (void *)ThirdCenterDialog);			
				while (alertDItem > 1)
				{
					ModalDialog(DLOGwOK_CancelUPP,&alertDItem);
					DisposeDialog(alertUser);
				}
				SetPort(dptr); //back to old dialog
				SetTEText(dptr,TermName,PrefRecordNamePtr);//set the string back to the original
				SelectDialogItemText(dptr,TermName,0,32767);
				ditem = TermName; //so that if they hit ok or cancel, it doesn't kill the window
			}
			else
				goto okayTermName;
		}
		else
		{
				okayTermName:
			switch (ditem) {
				case	TermANSIEsc:
				case	TermXterm:
				case	37:
				case	38:
				case	39:
				case	42:
				case	44:
				case	45:
				case	46:
				case	47:
				case	48:
				case	50:
				case	Termvtwrap:
				case	Termarrow:
				case	TermMAT:
				case	Termeightbit:
				case	Termclearsave:
				case 	TermRemapKeypad:
					FlipCheckBox(dptr, ditem);
					break;

				case	TermType:
					switch (GetCntlVal(dptr, ditem)) {
						case 1:
							SetTEText(dptr, TermAnswerback, "\pvt100");
							HideDialogItem(dptr, 48);
							//HideDialogItem(dptr,TermRemapKeypad);
							//HideDialogItem(dptr,TermMAT);
							break;
						case 2:
							SetTEText(dptr, TermAnswerback, "\pvt220");
							ShowDialogItem(dptr, 48);
							//ShowDialogItem(dptr,TermRemapKeypad);
							//ShowDialogItem(dptr,TermMAT);
							break;
						case 3:
							SetTEText(dptr, TermAnswerback, "\pansi");
							ShowDialogItem(dptr, 48);
							//ShowDialogItem(dptr,TermRemapKeypad);
							//ShowDialogItem(dptr,TermMAT);
							break;
						case 4:
							SetTEText(dptr, TermAnswerback, "\plinux");
							ShowDialogItem(dptr, 48);
							//ShowDialogItem(dptr,TermRemapKeypad);
							//ShowDialogItem(dptr,TermMAT);
							break;
					}
					break;

				case	TermMetaIsCmdCntrol:
					SetCntrl(dptr, TermMetaIsCmdCntrol, 1);
					SetCntrl(dptr, TermMetaIsOption, 0);
					SetCntrl(dptr, TermMetaIsCmd, 0);
					SetCntrl(dptr, TermMetaIsOff, 0);
					break;
				case	TermMetaIsOption:
					SetCntrl(dptr, TermMetaIsCmdCntrol, 0);
					SetCntrl(dptr, TermMetaIsOption, 1);
					SetCntrl(dptr, TermMetaIsCmd, 0);
					SetCntrl(dptr, TermMetaIsOff, 0);
					break;
				case	TermMetaIsCmd:
					SetCntrl(dptr, TermMetaIsCmdCntrol, 0);
					SetCntrl(dptr, TermMetaIsOption, 0);
					SetCntrl(dptr, TermMetaIsCmd, 1);
					SetCntrl(dptr, TermMetaIsOff, 0);
					break;
				case	TermMetaIsOff:
					SetCntrl(dptr, TermMetaIsCmdCntrol, 0);
					SetCntrl(dptr, TermMetaIsOption, 0);
					SetCntrl(dptr, TermMetaIsCmd, 0);
					SetCntrl(dptr, TermMetaIsOff, 1);
					break;
				case	TermNFcolor:	
				case	TermNBcolor:	
				case	TermBFcolor:	
				case	TermBBcolor:	
					if (TelInfo->haveColorQuickDraw) {
						Str255 askColorString;
						GetIndString(askColorString,MISC_STRINGS,PICK_NEW_COLOR_STRING);
						UserLikesNewColor = GetColor(ColorBoxPoint, askColorString,
							 &BoxColorData[ditem-TermNFcolor], &scratchRGBcolor);
						if (UserLikesNewColor)
							BoxColorData[ditem-TermNFcolor] = scratchRGBcolor;
						}
					break;
				case	43: // switch tabs
					newPanel = GetCntlVal(dptr, ditem);
					if (newPanel == currentPanel)
						break;
					editField = ((DialogPeek)dptr)->editField + 1;
					HideTermPanel(dptr, currentPanel);
					ShowTermPanel(dptr, newPanel);
					currentPanel = newPanel;
					if (editField != ((DialogPeek)dptr)->editField + 1
					  && ((DialogPeek)dptr)->editField >= 0) {
						SelectDialogItemText(dptr, ((DialogPeek)dptr)->editField + 1, 0, 255);
					}
					break;

				case 	TermName:
					wasInAliasText = TRUE;
				default:
					break;
				
				} // switch
			}//else not wasInAliasText
		} // while

	GetMenuItemText(TPopup[1].h, TPopup[1].choice, scratchPstring);
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring, (TermPrefsPtr->BoldFont), scratchPstring[0]+1);

	GetMenuItemText(TPopup[0].h, TPopup[0].choice, scratchPstring);
	PopupCleanup();
	
	if (ditem == 2) {
		if (IsNewPrefRecord) DisposeHandle((Handle) TermPrefsHdl);
		else ReleaseResource((Handle) TermPrefsHdl);

		TelInfo->lastPanelTerminal = currentPanel - 1;
		DisposeDialog(dptr);
		ResetMenus();
		return(FALSE);			// No changes should be made.
		}
	
		
	if (TelInfo->haveColorQuickDraw) {
		for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++) {
				BlockMoveData(&BoxColorData[scratchshort], 
					&(TermPrefsPtr->nfcolor) + scratchshort, sizeof(RGBColor));
			}
		}
		
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring, (TermPrefsPtr->DisplayFont), scratchPstring[0]+1);
	
	TermPrefsPtr->ANSIgraphics = GetCntlVal(dptr, TermANSIEsc);
	TermPrefsPtr->Xtermsequences = GetCntlVal(dptr, TermXterm);
	TermPrefsPtr->remapCtrlD = GetCntlVal(dptr, 37);
	TermPrefsPtr->allowBold = GetCntlVal(dptr, 38);
	TermPrefsPtr->colorBold = GetCntlVal(dptr, 39);
	TermPrefsPtr->realbold = GetCntlVal(dptr, 44);
	TermPrefsPtr->oldScrollback = GetCntlVal(dptr, 45);
	TermPrefsPtr->jumpScroll = GetCntlVal(dptr, 46);
	TermPrefsPtr->boldFontStyle = GetCntlVal(dptr, 42);
	TermPrefsPtr->realBlink = GetCntlVal(dptr, 47);
	TermPrefsPtr->vt7bits = GetCntlVal(dptr, 48);
	TermPrefsPtr->hideScrollBars = GetCntlVal(dptr, 50);
	TermPrefsPtr->vtwrap = GetCntlVal(dptr, Termvtwrap);

	if (GetCntlVal(dptr, TermMetaIsCmdCntrol))
		TermPrefsPtr->emacsmetakey = 1;
	else if (GetCntlVal(dptr, TermMetaIsOption))
		TermPrefsPtr->emacsmetakey = 2;
	else if (GetCntlVal(dptr, TermMetaIsCmd))
		TermPrefsPtr->emacsmetakey = 3;
	else
		TermPrefsPtr->emacsmetakey = 0;
	
	TermPrefsPtr->emacsarrows = GetCntlVal(dptr, Termarrow);
	TermPrefsPtr->MATmappings = GetCntlVal(dptr, TermMAT);
	TermPrefsPtr->eightbit = GetCntlVal(dptr, Termeightbit);
	TermPrefsPtr->clearsave = GetCntlVal(dptr, Termclearsave);
	TermPrefsPtr->remapKeypad = GetCntlVal(dptr, TermRemapKeypad);

	TermPrefsPtr->vtemulation = GetCntlVal(dptr, TermType) - 1;

	GetTEText(dptr, TermWidth, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
/* NONO */
/*	BoundsCheck(&scratchlong, 133, 10);*/
	BoundsCheck(&scratchlong, 255, 1);
/* NONO */
	TermPrefsPtr->vtwidth = (short) scratchlong;
	
	GetTEText(dptr, TermHeight, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
//	BoundsCheck(&scratchlong, 80, 10);
	BoundsCheck(&scratchlong, 200, 1);
	TermPrefsPtr->vtheight = (short) scratchlong;

	GetTEText(dptr, TermFontSize, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	BoundsCheck(&scratchlong, 24, 4);
	TermPrefsPtr->fontsize = (short) scratchlong;

	GetTEText(dptr, TermScrollback, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	BoundsCheck(&scratchlong, 32767, 24);
	TermPrefsPtr->numbkscroll = (unsigned short) scratchlong;

	GetTEText(dptr, TermAnswerback, scratchPstring);
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring, TermPrefsPtr->AnswerBackMessage, scratchPstring[0]+1);
	
	GetTEText(dptr, TermName, PrefRecordNamePtr);
	
	if (IsNewPrefRecord) {
		UseResFile(TelInfo->SettingsFile);
		resourceID = UniqueID(TERMINALPREFS_RESTYPE);
		AddResource((Handle)TermPrefsHdl, TERMINALPREFS_RESTYPE, resourceID, PrefRecordNamePtr);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)TermPrefsHdl);
		}
	else {
		UseResFile(TelInfo->SettingsFile);
		GetResInfo((Handle)TermPrefsHdl, &resourceID, &scratchResType,(StringPtr) &scratchPstring);
		SetResInfo((Handle)TermPrefsHdl, resourceID, PrefRecordNamePtr);
		ChangedResource((Handle)TermPrefsHdl);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)TermPrefsHdl);
		}

	TelInfo->lastPanelTerminal = currentPanel - 1;
	DisposeDialog(dptr);
	ResetMenus();
	return(TRUE);			// A resource has changed or been added.	
}

/* NONO */
static void CheckSSH2Method(DialogPtr dptr, short method)
{
	if ( method ) {
		if ( method == 3 ) {
			ShowDialogItem(dptr, 81);
			HideDialogItemRange(dptr, 82, 83);
			ShowDialogItem(dptr, 84);
			HideDialogItemRange(dptr, 85, 86);
			ShowDialogItemRange(dptr, 87, 88);
			HideDialogItem(dptr, 89);
		} else if ( method == 4 ) {
			ShowDialogItem(dptr, 81);
			HideDialogItemRange(dptr, 82, 83);
			ShowDialogItem(dptr, 84);
			HideDialogItemRange(dptr, 85, 86);
			ShowDialogItemRange(dptr, 87, 88);
			HideDialogItem(dptr, 89);
		} else {
			ShowDialogItemRange(dptr, 81, 89);
		}
	} else {
		HideDialogItemRange(dptr, 81, 89);
	}
}
/* NONO */

void ShowSessPanel(DialogPtr dptr, short panel)
{
	switch (panel) {
		case 1:	// general
		ShowDialogItemRange(dptr, 19, 20);
		ShowDialogItemRange(dptr, 25, 27);
		ShowDialogItem(dptr, 29);
		ShowDialogItemRange(dptr, 36, 37);
		ShowDialogItemRange(dptr, 40, 41);
		DrawPopUp(dptr, 25);
		DrawPopUp(dptr, 26);
		ShowDialogItem(dptr, 72);
		ShowDialogItem(dptr, 90);
		break;

		case 2: // network
		ShowDialogItemRange(dptr, 6, 7);
		ShowDialogItemRange(dptr, 21, 24);
		ShowDialogItem(dptr, 12);
		ShowDialogItem(dptr, 14);
		ShowDialogItem(dptr, 17);
		ShowDialogItem(dptr, 31);
		ShowDialogItemRange(dptr, 33, 35);
		ShowDialogItemRange(dptr, 38, 39);
		break;

		case 3: // terminal
		ShowDialogItemRange(dptr, 3, 5);
		ShowDialogItemRange(dptr, 8, 11);
		ShowDialogItem(dptr, 13);
		ShowDialogItem(dptr, 30);
		ShowDialogItem(dptr, 32);
		ShowDialogItemRange(dptr, 42, 44);
		ShowDialogItem(dptr, 91);
		ShowDialogItem(dptr, 94);
		break;

		case 4: // security
		ShowDialogItemRange(dptr, 15, 16);
		ShowDialogItemRange(dptr, 53, 61);
		ShowDialogItem(dptr, 92);
		break;

		case 5: // otp
		ShowDialogItemRange(dptr, 46, 52);
		break;

		case 6: // ssh2
		ShowDialogItemRange(dptr, 62, 63);
/* NONO */
		ShowDialogItemRange(dptr, 73, 80);
		CheckSSH2Method( dptr, GetCntlVal(dptr, 80) - 1 );
		ShowDialogItem(dptr, 93);
/* NONO */
		break;

		case 7: // firewall
		ShowDialogItemRange(dptr, 64, 71);
		break;
	}
}

void HideSessPanel(DialogPtr dptr, short panel)
{
	switch (panel) {
		case 1:
		DrawBlank(dptr, 25);
		DrawBlank(dptr, 26);
		HideDialogItemRange(dptr, 19, 20);
		HideDialogItemRange(dptr, 25, 27);
		HideDialogItem(dptr, 29);
		HideDialogItemRange(dptr, 36, 37);
		HideDialogItemRange(dptr, 40, 41);
		HideDialogItem(dptr, 72);
		HideDialogItem(dptr, 90);
		break;

		case 2:
		HideDialogItemRange(dptr, 6, 7);
		HideDialogItemRange(dptr, 21, 24);
		HideDialogItem(dptr, 12);
		HideDialogItem(dptr, 14);
		HideDialogItem(dptr, 17);
		HideDialogItem(dptr, 31);
		HideDialogItemRange(dptr, 33, 35);
		HideDialogItemRange(dptr, 38, 39);
		break;

		case 3:
		HideDialogItemRange(dptr, 3, 5);
		HideDialogItemRange(dptr, 8, 11);
		HideDialogItem(dptr, 13);
		HideDialogItem(dptr, 30);
		HideDialogItem(dptr, 32);
		HideDialogItemRange(dptr, 42, 44);
		HideDialogItem(dptr, 91);
		HideDialogItem(dptr, 94);
		break;

		case 4:
		HideDialogItemRange(dptr, 15, 16);
		HideDialogItemRange(dptr, 53, 61);
		HideDialogItem(dptr, 92);
		break;

		case 5:
		HideDialogItemRange(dptr, 46, 52);
		break;

		case 6:
		HideDialogItemRange(dptr, 62, 63);
/* NONO */
		HideDialogItemRange(dptr, 73, 89);
		HideDialogItem(dptr, 93);
/* NONO */
		break;

		case 7:
		HideDialogItemRange(dptr, 64, 71);
	}
}

Boolean EditSession(StringPtr PrefRecordNamePtr)
{
	DialogPtr		dptr;
	short			ditem, scratchshort, resourceID, currentPanel, newPanel, i;
	short 			numberOfTerms;
	long			scratchlong;
	ResType			scratchResType;
	Boolean			IsNewPrefRecord, wasInAliasText;
	SessionPrefs**	SessPrefsHdl;
	SessionPrefs*	SessPrefsPtr;
	TerminalPrefs** tempTermPrefsHdl;
	Str255			scratchPstring, scratchPstring2;
	LinkedListNode *savedList;
	EventRecord optKeyEvtRec;
	popup SPopup[] = {{SessTermPopup, (MenuHandle) 0, 1},
						{SessTransTablePopup, (MenuHandle) 0, 1},
						{0, (MenuHandle) 0, 0}};
	short			editField;

	SetUpMovableModalMenus();
	dptr = GetNewMySmallStrangeDialog(SessionConfigDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	ditem = 3;
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 2);
	SetDialogTracksCursor(dptr, 1);
	currentPanel = TelInfo->lastPanelSession + 1;
	
	savedList = currentHead; //save the current list, so the list that got us here works
	SPopup[0].h = NewMenu(666, "\p");
	UseResFile(TelInfo->SettingsFile);
	numberOfTerms = Count1Resources(TERMINALPREFS_RESTYPE);
	currentHead  = createSortedList(TERMINALPREFS_RESTYPE,numberOfTerms,gDefaultName);
	addListToMenu(SPopup[0].h, currentHead, 1);
	EnableItem(SPopup[0].h, 0);		// Make sure the entire menu is enabled
	deleteList(&currentHead);
	
	UseResFile(TelInfo->ApplicationFile);
	SPopup[1].h = NewMenu(667, "\p");
	BuildTranslateMenu(SPopup[1].h);

	UseResFile(TelInfo->SettingsFile);
	currentHead = savedList;
	PopupInit(dptr, SPopup);

	if (PrefRecordNamePtr[0] != 0) {
		IsNewPrefRecord = FALSE;
		UseResFile(TelInfo->SettingsFile);
		SessPrefsHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, PrefRecordNamePtr, sizeof(SessionPrefs));
		if (IsDefaultLabel(PrefRecordNamePtr)) {
			HideDialogItem(dptr, SessAlias);
			HideDialogItem(dptr, SessAliasStatText);
		}
	} else {
		SessPrefsHdl = GetDefaultSession();
		IsNewPrefRecord = TRUE;
		GetIndString(PrefRecordNamePtr, MISC_STRINGS, MISC_NEWSESSION);
		getAUniqueName(currentHead,PrefRecordNamePtr);
	}	

	HLock((Handle) SessPrefsHdl);
	SessPrefsPtr = *SessPrefsHdl;
	SetCntrl(dptr, SessTEKinhib, (SessPrefsPtr->tektype == -1));
	SetCntrl(dptr, SessTEK4014, (SessPrefsPtr->tektype == 0));
	SetCntrl(dptr, SessTEK4105, (SessPrefsPtr->tektype == 1));
	SetCntrl(dptr, SessPasteQuick, (SessPrefsPtr->pastemethod == 0));
	SetCntrl(dptr, SessPasteBlock, (SessPrefsPtr->pastemethod == 1));
	SetCntrl(dptr, SessDeleteDel, (SessPrefsPtr->bksp == 1));
	SetCntrl(dptr, SessDeleteBS, (SessPrefsPtr->bksp == 0));
	SetCntrl(dptr, SessForceSave, SessPrefsPtr->forcesave);
	SetCntrl(dptr, SessBezerkeley, SessPrefsPtr->crmap);
	SetCntrl(dptr, 42, SessPrefsPtr->alwaysBSD);
	SetCntrl(dptr, 43, SessPrefsPtr->ignoreBeeps);
	SetCntrl(dptr, 44, SessPrefsPtr->ignoreff);
	SetCntrl(dptr, SessLinemode, SessPrefsPtr->linemode);
	SetCntrl(dptr, SessTEKclear, SessPrefsPtr->tekclear);
	SetCntrl(dptr, SessHalfDuplex, SessPrefsPtr->halfdup);
	SetCntrl(dptr, SessAuthenticate, SessPrefsPtr->authenticate);
	SetCntrl(dptr, SessEncrypt, SessPrefsPtr->encrypt);
	SetCntrl(dptr, 92, SessPrefsPtr->forward);
	SetCntrl(dptr, SessLocalEcho, SessPrefsPtr->localecho);
	SetCntrl(dptr, 46, SessPrefsPtr->otpauto);
	SetCntrl(dptr, 47, SessPrefsPtr->otpmulti);
	SetCntrl(dptr, 48, SessPrefsPtr->otphex);
	SetCntrl(dptr, 49, SessPrefsPtr->otpnoprompt);
	SetCntrl(dptr, 52, SessPrefsPtr->otpsavepass);
	SetCntrl(dptr, 53, SessPrefsPtr->protocol + 1);
	SetCntrl(dptr, 62, SessPrefsPtr->encryption + 1);
	SetCntrl(dptr, 64, SessPrefsPtr->usesocks);
	SetCntrl(dptr, 65, SessPrefsPtr->socks4a);

/* NONO */
	SetCntrl(dptr, 73, SessPrefsPtr->authentication + 1);
	SetCntrl(dptr, 74, SessPrefsPtr->compression + 1);
	SetCntrl(dptr, 75, SessPrefsPtr->verbose);
	SetCntrl(dptr, 76, SessPrefsPtr->trace);
	SetCntrl(dptr, 77, SessPrefsPtr->debug);
	SetCntrl(dptr, 78, SessPrefsPtr->restricted);
	SetCntrl(dptr, 80, SessPrefsPtr->ssh2method + 1);
	CheckSSH2Method(dptr, SessPrefsPtr->ssh2method);
	NumToString((unsigned short)SessPrefsPtr->localport, scratchPstring);
	SetTEText(dptr, 81, scratchPstring);
	SetTEText(dptr, 82, (unsigned char *)SessPrefsPtr->remotehost);
	NumToString((unsigned short)SessPrefsPtr->remoteport, scratchPstring);
	SetTEText(dptr, 83, scratchPstring);
	SetCntrl(dptr, 87, SessPrefsPtr->ssh2guests);
	CheckPortPopup( dptr, SessPrefsPtr->localport, 88 );
	CheckPortPopup( dptr, SessPrefsPtr->remoteport, 89 );
	CheckPortPopup( dptr, (unsigned short)SessPrefsPtr->port, 90 );
	SetCntrl(dptr, 91, SessPrefsPtr->launchurlesc);
	SetCntrl(dptr, 93, SessPrefsPtr->x11forward);
	SetCntrl(dptr, 94, SessPrefsPtr->keepselection);
//	SetCntrl(dptr, 95, SessPrefsPtr->autoreconnect);

/* NONO */

	if (!authOK) {
		Hilite( dptr, SessAuthenticate, 255);
		Hilite( dptr, SessEncrypt, 255);
		Hilite( dptr, 92, 255);
	} else if (!encryptOK) {
		Hilite( dptr, SessEncrypt, 255);
	}
	if (GetCntlVal(dptr, SessAuthenticate)) {
		Hilite(dptr, SessEncrypt, (encryptOK)? 0 : 255);
		Hilite(dptr, 92, 0);
	} else {
		Hilite(dptr, SessEncrypt, 255);
		Hilite( dptr, 92, 255);
		SetCntrl(dptr, SessEncrypt, false);
		SetCntrl(dptr, 92, false);
	}

	configPassword[0] = 0;
	if (SessPrefsPtr->otppassword[0]) {
		pstrcpy(configPassword, (unsigned char *)SessPrefsPtr->otppassword);
		scratchPstring[0] = configPassword[0];
		for (i = 1; i < 250 && i <= configPassword[0]; i++)
			scratchPstring[i] = '¥';
		SetTEText(dptr, 51, scratchPstring);
	}
	configPassword2[0] = 0;
	if (SessPrefsPtr->password[0]) {
		pstrcpy(configPassword2, (unsigned char *)SessPrefsPtr->password);
		scratchPstring[0] = configPassword2[0];
		for (i = 1; i < 250 && i <= configPassword2[0]; i++)
			scratchPstring[i] = '¥';
		SetTEText(dptr, 59, scratchPstring);
	}

	SetTEText(dptr, SessHostName, SessPrefsPtr->hostname);
	SetTEText(dptr, SessAlias, PrefRecordNamePtr);
	SetTEText(dptr, 58, (unsigned char *)SessPrefsPtr->username);
//	SetTEText(dptr, 59, (unsigned char *)SessPrefsPtr->password);
	SetTEText(dptr, 60, (unsigned char *)SessPrefsPtr->clientuser);

	if ( SessPrefsPtr->protocol != 4 ) {
		GetIndString(scratchPstring, MISC_STRINGS, MISC_COMMAND);
	} else {
		GetIndString(scratchPstring, MISC_STRINGS, MISC_ARGUMENTS);
	}
	SetTEText(dptr, 57, scratchPstring);

	SetTEText(dptr, 61, (unsigned char *)SessPrefsPtr->command);
	SetTEText(dptr, 66, (unsigned char *)SessPrefsPtr->sockshost);
	SetTEText(dptr, 71, (unsigned char *)SessPrefsPtr->socksusername);

	if (SessPrefsPtr->portNegative) {
		NumToString((unsigned short)SessPrefsPtr->port, &scratchPstring[1]);
		scratchPstring[0] = scratchPstring[1] + 1;
		scratchPstring[1] = '-';
	} else NumToString((unsigned short)SessPrefsPtr->port, scratchPstring);
	SetTEText(dptr, SessPort, scratchPstring);

	NumToString(SessPrefsPtr->pasteblocksize, scratchPstring);
	SetTEText(dptr, SessBlockSize, scratchPstring);

	if (!SessPrefsPtr->socksport) SessPrefsPtr->socksport = 1080;
	NumToString((unsigned short)SessPrefsPtr->socksport, scratchPstring);
	SetTEText(dptr, 67, scratchPstring);

	if (SessPrefsPtr->NetBlockSize <= 512)
		SessPrefsPtr->NetBlockSize = 512;
	NumToString(SessPrefsPtr->NetBlockSize, scratchPstring);
	SetTEText(dptr, SessBlockSizeTE, scratchPstring);
	
	scratchPstring[0] = 2;
	scratchPstring[1] = '^';
	if (SessPrefsPtr->ckey != -1) {
		scratchPstring[2] = SessPrefsPtr->ckey ^ 64;
		SetTEText(dptr, SessInterrupt, scratchPstring);
	}
	if (SessPrefsPtr->skey != -1) {
		scratchPstring[2] = SessPrefsPtr->skey ^ 64;
		SetTEText(dptr, SessSuspend, scratchPstring);
	}
	if (SessPrefsPtr->qkey != -1) {
		scratchPstring[2] = SessPrefsPtr->qkey ^ 64;
		SetTEText(dptr, SessResume, scratchPstring);
	}
	for(scratchshort = CountMItems(SPopup[0].h); scratchshort; scratchshort--) {
		GetMenuItemText(SPopup[0].h, scratchshort, scratchPstring);
		if (EqualString(scratchPstring, (SessPrefsPtr->TerminalEmulation), TRUE, FALSE))
			SPopup[0].choice = scratchshort;
	}
	for(scratchshort = CountMItems(SPopup[1].h); scratchshort; scratchshort--) {
		GetMenuItemText(SPopup[1].h, scratchshort, scratchPstring);
		if (EqualString(scratchPstring, (SessPrefsPtr->TranslationTable), TRUE, FALSE))
			SPopup[1].choice = scratchshort;
	}

	NumberOfColorBoxes = 0;
	ZeroNumOnly();
	NumOnly[0] = SessSafeItem;
	NumOnly[1] = SessBlockSize;

	SetCntrl(dptr, 45, currentPanel);

	for (i = 1; i < 8; i++)
		HideSessPanel(dptr, i);
	ShowSessPanel(dptr, currentPanel);

	if ( ((DialogPeek)dptr)->editField >= 0 ) {
		SelectDialogItemText(dptr, ((DialogPeek)dptr)->editField + 1, 0, 255);
	}

	ShowWindow(dptr);
	wasInAliasText = FALSE;
	while ((ditem > 2) || (ditem == 0)) {
		movableModalDialog(SessionModalProcUPP, &ditem);
		if (ditem == 0) continue;
		if ((wasInAliasText)&&(ditem != SessAlias))
		{
			wasInAliasText = FALSE;
			GetTEText(dptr, SessAlias, scratchPstring);
			if (EqualString(scratchPstring,PrefRecordNamePtr,FALSE,FALSE))
				goto okaySessName;
			if (scratchPstring[0] == 0)
			{
				DialogPtr alertUser;
				short	alertDItem = 3;
				alertUser = GetNewMyDialog(kBadPrefNameDLOG, NULL, kInFront, (void *)ThirdCenterDialog);			
				while (alertDItem > 1)
				{
					ModalDialog(DLOGwOK_CancelUPP,&alertDItem);
					DisposeDialog(alertUser);
				}
				SetPort(dptr); //back to old dialog
				SetTEText(dptr,SessAlias,PrefRecordNamePtr);//set the string back to the original
				SelectDialogItemText(dptr,SessAlias,0,32767);
				ditem = SessAlias; //so that if they hit ok or cancel, it doesn't kill the window
			}
			else if(findNodeNC(currentHead,scratchPstring))  //we already have that name
			{
				DialogPtr alertUser;
				short	alertDItem = 3;
				alertUser = GetNewMyDialog(kBadPrefNameDLOG, NULL, kInFront, (void *)ThirdCenterDialog);			
				while (alertDItem > 1)
				{
					ModalDialog(DLOGwOK_CancelUPP,&alertDItem);
					DisposeDialog(alertUser);
				}
				SetPort(dptr); //back to old dialog
				SetTEText(dptr,SessAlias,PrefRecordNamePtr);//set the string back to the original
				SelectDialogItemText(dptr,SessAlias,0,32767);
				ditem = SessAlias; //so that if they hit ok or cancel, it doesn't kill the window
			}
			else
				goto okaySessName;
		}
		else
		{
			okaySessName:
			switch (ditem) {

				case	SessAuthenticate:
					FlipCheckBox(dptr, ditem);
					if (GetCntlVal(dptr, SessAuthenticate)) {
						Hilite(dptr, SessEncrypt, (encryptOK)? 0 : 255);
						Hilite(dptr, 92, 0);
					} else {
						Hilite(dptr, SessEncrypt, 255);
						Hilite(dptr, 92, 255);
						SetCntrl(dptr, SessEncrypt, false);
						SetCntrl(dptr, 92, false);
					}
					break;

				case	SessForceSave:
				case	SessBezerkeley:
				case	SessLinemode:
				case	SessTEKclear:
				case	SessHalfDuplex:
				case	SessEncrypt:
				case	SessLocalEcho:
				case	42:
				case	43:
				case	44:
				case	46:
				case	47:
				case	48:
				case	49:
				case	52:
				case	64:
				case	65:
/* NONO */
				case	75:
				case	76:
				case	77:
				case	78:
				case	87:
				case	91:
				case	92:
				case	93:
				case	94:
/* NONO */
					FlipCheckBox(dptr, ditem);
					break;

/* NONO */
				case	79:
					if (ssh_glue_installed())
						ssh_glue_exportkey();
					else OperationFailedAlert(6, 0, 0);
					break;
				case	80:
					CheckSSH2Method( dptr, GetCntlVal(dptr, 80) - 1 );
					break;
				case	88:
				case	89:
					/* local/remote port popup */
					GetIndString(scratchPstring, 2008, GetCntlVal(dptr, ditem));
					if (scratchPstring[0]) {
						SetTEText(dptr,(ditem == 88) ? 81 : 83,scratchPstring);
						SelectDialogItemText(dptr,(ditem == 88) ? 81 : 83,0,32767);
					}
					break;
				case	90:
					GetIndString(scratchPstring, 2008, GetCntlVal(dptr, ditem));
					if (scratchPstring[0]) {
						SetTEText(dptr,SessPort,scratchPstring);
						SelectDialogItemText(dptr,SessPort,0,32767);
					}
					break;
				case 63:
					if (ssh_glue_installed()) {
						SetDialogTracksCursor(dptr, 0);
						ssh_glue_randomize();
						SetDialogTracksCursor(dptr, 1);
					}
					else OperationFailedAlert(6, 0, 0);
				break;
/* NONO */

				case 53:
				{
					short protocol, tempPort;
					protocol = GetCntlVal(dptr, 53);
					tempPort = getDefaultPort(protocol - 1);
					NumToString((unsigned short)tempPort, scratchPstring);
					SetTEText(dptr, SessPort, scratchPstring);
					CheckPortPopup( dptr, (unsigned short)tempPort, 90 );

					if ( protocol != 5 ) {
						GetIndString(scratchPstring, MISC_STRINGS, MISC_COMMAND);
					} else {
						GetIndString(scratchPstring, MISC_STRINGS, MISC_ARGUMENTS);
					}
					SetTEText(dptr, 57, scratchPstring);
				}
				break;

				case	SessTEKinhib:
					SetCntrl(dptr, SessTEKinhib, 1);
					SetCntrl(dptr, SessTEK4014, 0);
					SetCntrl(dptr, SessTEK4105, 0);
					break;
				case	SessTEK4014:
					SetCntrl(dptr, SessTEKinhib, 0);
					SetCntrl(dptr, SessTEK4014, 1);
					SetCntrl(dptr, SessTEK4105, 0);
					break;
				case	SessTEK4105:
					SetCntrl(dptr, SessTEKinhib, 0);
					SetCntrl(dptr, SessTEK4014, 0);
					SetCntrl(dptr, SessTEK4105, 1);
					break;
					
				case	SessPasteQuick:
					SetCntrl(dptr, SessPasteQuick, 1);
					SetCntrl(dptr, SessPasteBlock, 0);
					break;
		
				case	SessPasteBlock:
					SetCntrl(dptr, SessPasteQuick, 0);
					SetCntrl(dptr, SessPasteBlock, 1);
					break;
		
				case	SessDeleteDel:
					SetCntrl(dptr, SessDeleteDel, 1);
					SetCntrl(dptr, SessDeleteBS, 0);
					break;
		
				case	SessDeleteBS:
					SetCntrl(dptr, SessDeleteDel, 0);
					SetCntrl(dptr, SessDeleteBS, 1);
					break;
		
				case	SessInterrupt:
				case	SessSuspend:
				case	SessResume:
					GetTEText(dptr, ditem, scratchPstring);
					if ((scratchPstring[1] < 32) && (scratchPstring[1] > 0)) {
						scratchPstring[0] = 2;
						scratchPstring[2] = scratchPstring[1] ^ 64;
						scratchPstring[1] = '^';
						SetTEText(dptr, ditem, scratchPstring);
						}
					break;

				case 45: // tab control
				newPanel = GetCntlVal(dptr, 45);
				if (newPanel == currentPanel)
					break;
				editField = ((DialogPeek)dptr)->editField + 1;
				HideSessPanel(dptr, currentPanel);
				ShowSessPanel(dptr, newPanel);
				SelectDialogItemText(dptr,((DialogPeek)dptr)->editField + 1,0,32767);
				currentPanel = newPanel;
				if (editField != ((DialogPeek)dptr)->editField + 1
				  && ((DialogPeek)dptr)->editField >= 0) {
					SelectDialogItemText(dptr, ((DialogPeek)dptr)->editField + 1, 0, 255);
				}
				break;

				case 40: // Save as Set...
				case 41: // Add to Set...
					SetSessionData(dptr, SessPrefsPtr, (char *)SPopup, PrefRecordNamePtr);
					UseResFile(TelInfo->SettingsFile);
					tempTermPrefsHdl = (TerminalPrefs **)Get1NamedSizedResource
						(TERMINALPREFS_RESTYPE, SessPrefsPtr->TerminalEmulation, sizeof(TerminalPrefs));
					DetachResource((Handle) tempTermPrefsHdl);
					HLock((Handle) tempTermPrefsHdl);
					EventAvail(everyEvent, &optKeyEvtRec);
					SaveSetFromSession(SessPrefsPtr, *tempTermPrefsHdl, (ditem == 41), (optKeyEvtRec.modifiers & 0x0800));
					DisposeHandle((Handle) tempTermPrefsHdl);
					break;

				case 72: // Edit Session Macros
				{
					static NewMacroInfo tmpmacros, savedmacroidx;
					Handle tmpmacrohdl, savedmacrohdl;
					short macropanel, macroitem;
					long len;
					char *pos;

					savedmacrohdl = oldMacros;
					savedmacroidx = oldMacroIndexes;
					macropanel = dialogPane;

					setupNewMacros(&tmpmacros);
					UseResFile(TelInfo->SettingsFile);
					tmpmacrohdl = Get1NamedResource('uMac', PrefRecordNamePtr);
					if (tmpmacrohdl) {
						HLock(tmpmacrohdl);
						DetachResource(tmpmacrohdl);
						disposemacros(&tmpmacros);
						ParseMacrosFromHandle(&tmpmacros, tmpmacrohdl);
					}
					macroitem = ModalMacros(&tmpmacros);
					if (macroitem != 2) {
						UseResFile(TelInfo->SettingsFile);
						tmpmacrohdl = Get1NamedResource('uMac', PrefRecordNamePtr);
						if (tmpmacrohdl) RemoveResource(tmpmacrohdl);
						HLock(tmpmacros.handle);
						len = GetHandleSize(tmpmacros.handle);
						tmpmacrohdl = myNewHandle(len + 2);
						HLock(tmpmacrohdl);
						pos = *tmpmacrohdl;
						*pos = '!'; pos++;
						*pos = '\015'; pos++;
						BlockMoveData(*(tmpmacros.handle), pos, len);
						DisposeHandle(tmpmacros.handle);
						HUnlock(tmpmacrohdl);
						AddResource(tmpmacrohdl, 'uMac', UniqueID('uMac'), PrefRecordNamePtr);
						UpdateResFile(TelInfo->SettingsFile);
						ReleaseResource(tmpmacrohdl);
					} else DisposeHandle(tmpmacros.handle);

					oldMacros = savedmacrohdl;
					oldMacroIndexes = savedmacroidx;
					dialogPane = macropanel;
				}
					break;
				case	SessAlias:
					wasInAliasText = TRUE;
					break;
				default:
					break;
				} // switch
			} //else not bad alias
		} // while

	if (ditem == 2) {
		PopupCleanup();
		if (IsNewPrefRecord) DisposeHandle((Handle) SessPrefsHdl);
		else ReleaseResource((Handle) SessPrefsHdl);
		TelInfo->lastPanelSession = currentPanel - 1;
		DisposeDialog(dptr);
		ResetMenus();
		return(FALSE);			// No changes should be made.
		}

	SetSessionData(dptr, SessPrefsPtr, (char *)SPopup, PrefRecordNamePtr);
	PopupCleanup();

	if (IsNewPrefRecord) {
		UseResFile(TelInfo->SettingsFile);
		resourceID = UniqueID(SESSIONPREFS_RESTYPE);
		AddResource((Handle)SessPrefsHdl, SESSIONPREFS_RESTYPE, resourceID, PrefRecordNamePtr);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)SessPrefsHdl);
		}
	else {
		UseResFile(TelInfo->SettingsFile);
		GetResInfo((Handle)SessPrefsHdl, &resourceID, &scratchResType,(StringPtr) &scratchPstring);
		SetTaggedSesResInfo((Handle)SessPrefsHdl, resourceID, scratchPstring, PrefRecordNamePtr);
		ChangedResource((Handle)SessPrefsHdl);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)SessPrefsHdl);
		}

	TelInfo->lastPanelSession = currentPanel - 1;
	DisposeDialog(dptr);
	ResetMenus();
	return(TRUE);			// A resource has changed or been added.	
}

void SetSessionData(DialogPtr dptr, SessionPrefs *SessPrefsPtr,
					char *SPopupRef, StringPtr PrefRecordNamePtr)
{
	short scratchshort;
	long scratchlong;
	Str255 scratchPstring, scratchPstring2;
	popup *SPopup = (popup *)SPopupRef;

	GetMenuItemText(SPopup[0].h, SPopup[0].choice, scratchPstring);
	GetMenuItemText(SPopup[1].h, SPopup[1].choice, scratchPstring2);
		
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring, (SessPrefsPtr->TerminalEmulation), scratchPstring[0]+1);

	if (StrLength(scratchPstring2) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring2, (SessPrefsPtr->TranslationTable), scratchPstring2[0]+1);

	SessPrefsPtr->tektype = (-1 * GetCntlVal(dptr, SessTEKinhib)) + GetCntlVal(dptr, SessTEK4105);
	SessPrefsPtr->pastemethod = !GetCntlVal(dptr, SessPasteQuick);
	SessPrefsPtr->bksp = GetCntlVal(dptr, SessDeleteDel);
	SessPrefsPtr->forcesave = GetCntlVal(dptr, SessForceSave);
	SessPrefsPtr->crmap = GetCntlVal(dptr, SessBezerkeley);
	SessPrefsPtr->alwaysBSD = GetCntlVal(dptr, 42);
	SessPrefsPtr->ignoreBeeps = GetCntlVal(dptr, 43);
	SessPrefsPtr->ignoreff = GetCntlVal(dptr, 44);
	SessPrefsPtr->linemode = GetCntlVal(dptr, SessLinemode);
	SessPrefsPtr->tekclear = GetCntlVal(dptr, SessTEKclear);
	SessPrefsPtr->halfdup = GetCntlVal(dptr, SessHalfDuplex);
	SessPrefsPtr->authenticate = GetCntlVal(dptr, SessAuthenticate);
	SessPrefsPtr->encrypt = GetCntlVal(dptr, SessEncrypt);
	SessPrefsPtr->forward = GetCntlVal(dptr, 92);
	SessPrefsPtr->localecho = GetCntlVal(dptr, SessLocalEcho);
	SessPrefsPtr->otpauto = GetCntlVal(dptr, 46);
	SessPrefsPtr->otpmulti = GetCntlVal(dptr, 47);
	SessPrefsPtr->otphex = GetCntlVal(dptr, 48);
	SessPrefsPtr->otpnoprompt = GetCntlVal(dptr, 49);
	SessPrefsPtr->otpsavepass = GetCntlVal(dptr, 52);
	SessPrefsPtr->protocol = GetCntlVal(dptr, 53) - 1;
	SessPrefsPtr->encryption = GetCntlVal(dptr,62) - 1;
	SessPrefsPtr->usesocks = GetCntlVal(dptr, 64);
	SessPrefsPtr->socks4a = GetCntlVal(dptr, 65);

/* NONO */
	SessPrefsPtr->authentication = GetCntlVal(dptr, 73) - 1;
	SessPrefsPtr->compression = GetCntlVal(dptr, 74) - 1;
	SessPrefsPtr->verbose = GetCntlVal(dptr, 75);
	SessPrefsPtr->trace = GetCntlVal(dptr, 76);
	SessPrefsPtr->debug = GetCntlVal(dptr, 77);
	SessPrefsPtr->restricted = GetCntlVal(dptr, 78);

	SessPrefsPtr->ssh2method = GetCntlVal(dptr, 80) - 1;

	GetTEText(dptr, 81, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	BoundsCheck(&scratchlong, 65535, 0);
	SessPrefsPtr->localport = (short)scratchlong;

	GetTEText(dptr, 82, scratchPstring);
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring, SessPrefsPtr->remotehost, scratchPstring[0]+1);

	GetTEText(dptr, 83, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	BoundsCheck(&scratchlong, 65535, 0);
	SessPrefsPtr->remoteport = (short)scratchlong;

	SessPrefsPtr->ssh2guests = GetCntlVal(dptr, 87);
	SessPrefsPtr->launchurlesc = GetCntlVal(dptr, 91);
	SessPrefsPtr->x11forward = GetCntlVal(dptr, 93);
	SessPrefsPtr->keepselection = GetCntlVal(dptr, 94);
//	SessPrefsPtr->autoreconnect = GetCntlVal(dptr, 95);
/* NONO */

	memset(SessPrefsPtr->otppassword, 0, sizeof(SessPrefsPtr->otppassword));
//	c2pstr(configPassword);
	pstrcpy((unsigned char *)SessPrefsPtr->otppassword, configPassword);

	memset(SessPrefsPtr->password, 0, sizeof(SessPrefsPtr->password));
//	c2pstr(configPassword2);
	pstrcpy((unsigned char *)SessPrefsPtr->password, configPassword2);

	GetTEText(dptr, SessPort, scratchPstring);
	SessPrefsPtr->portNegative = 0;
	if (scratchPstring[1] == '-') {
		SessPrefsPtr->portNegative = 1;
		scratchPstring[1] = scratchPstring[0] - 1;
		StringToNum(&scratchPstring[1], &scratchlong);
	} else StringToNum(scratchPstring, &scratchlong);
	BoundsCheck(&scratchlong, 65530, 1);
	SessPrefsPtr->port = (short) scratchlong;

	GetTEText(dptr, 67, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	SessPrefsPtr->socksport = (short) scratchlong;

	GetTEText(dptr, SessBlockSize, scratchPstring);
	StringToNum(scratchPstring, &scratchlong);
	BoundsCheck(&scratchlong, 4097, 10);
	SessPrefsPtr->pasteblocksize = (short) scratchlong;
	
	GetTEText(dptr, SessHostName, scratchPstring);
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;

	BlockMoveData(scratchPstring, SessPrefsPtr->hostname, scratchPstring[0]+1);

	GetTEText(dptr, 66, scratchPstring);
	if (StrLength(scratchPstring) > 63) scratchPstring[0] = 63;
	BlockMoveData(scratchPstring, SessPrefsPtr->sockshost, scratchPstring[0]+1);

	GetTEText(dptr, SessAlias, PrefRecordNamePtr);
	GetTEText(dptr, 58, (unsigned char *)SessPrefsPtr->username);
//	GetTEText(dptr, 59, (unsigned char *)SessPrefsPtr->password);
	GetTEText(dptr, 60, (unsigned char *)SessPrefsPtr->clientuser);
	GetTEText(dptr, 61, (unsigned char *)SessPrefsPtr->command);
	GetTEText(dptr, 71, (unsigned char *)SessPrefsPtr->socksusername);

	GetTEText(dptr, SessInterrupt, scratchPstring);
	if (scratchPstring[0]) SessPrefsPtr->ckey = toupper(scratchPstring[2]) ^ 64;
	else SessPrefsPtr->ckey = -1;
	
	GetTEText(dptr, SessSuspend, scratchPstring);
	if (scratchPstring[0]) SessPrefsPtr->skey = toupper(scratchPstring[2]) ^ 64;
	else SessPrefsPtr->skey = -1;
	
	GetTEText(dptr, SessResume, scratchPstring);
	if (scratchPstring[0]) SessPrefsPtr->qkey = toupper(scratchPstring[2]) ^ 64;
	else SessPrefsPtr->qkey = -1;
	
	GetTEText(dptr,SessBlockSizeTE,scratchPstring);
	StringToNum(scratchPstring,&scratchlong);
	BoundsCheck(&scratchlong, 4096, 512);
	SessPrefsPtr->NetBlockSize = (short) scratchlong;
}

Boolean EditFTPUser(StringPtr PrefRecordNamePtr)
{
	DialogPtr	dptr;
	short		ditem, scratchshort, resourceID, vRefNum;
	ResType		scratchResType;
	Boolean		IsNewPrefRecord;
	FTPUserPrefs**	FTPUHdl;
	FTPUserPrefs*	FTPUptr;
	Str255		scratchPstring, scratchPstring2;

	SetUpMovableModalMenus();
	dptr = GetNewMySmallStrangeDialog(FTPUserDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	ditem = 3;

	HideDialogItem(dptr, FTPUcanchangeCWD);		// Sometime later, perhaps
		
	if (PrefRecordNamePtr[0] != 0) {
		UseResFile(TelInfo->SettingsFile);
		FTPUHdl = (FTPUserPrefs **)Get1NamedResource(FTPUSERPREFS_RESTYPE, PrefRecordNamePtr);
		if ( FTPUHdl ) {
			IsNewPrefRecord = FALSE;
			HLock((Handle) FTPUHdl);
			FTPUptr = *FTPUHdl;
			SetCntrl(dptr, FTPUcanchangeCWD, FTPUptr->UserCanCWD);
			SetTEText(dptr, FTPUusername, PrefRecordNamePtr);
			for (scratchshort = 8, scratchPstring[0] = 8; scratchshort > 0; scratchshort--)
				scratchPstring[scratchshort] = '¥';
	 		SetTEText(dptr, FTPUpassword, scratchPstring);
			vRefNum = VolumeNameToRefNum(FTPUptr->DefaultDirVolName);
		} else {
			PrefRecordNamePtr[0] = 0;
		}
	}
	if (PrefRecordNamePtr[0] == 0) {
		IsNewPrefRecord = TRUE;
		FTPUHdl = (FTPUserPrefs **)myNewHandle(sizeof(FTPUserPrefs));
		HLock((Handle) FTPUHdl);
		FTPUptr = *FTPUHdl;
		vRefNum = -1;						// Default Volume
		FTPUptr->DefaultDirDirID = 2;		// Root directory
		SetCntrl(dptr, FTPUcanchangeCWD, 0);
	}

	PathNameFromDirID(FTPUptr->DefaultDirDirID, vRefNum, scratchPstring);
	SetTEText(dptr, FTPUDfltDirDsply, scratchPstring);
	
	SelectDialogItemText(dptr, FTPUusername, 0, 32767);
	ShowWindow(dptr);
	
	while (ditem > 2) {
		/*ModalDialog(DLOGwOK_CancelUPP, &ditem);*/
		movableModalDialog(DLOGwOK_CancelUPP, &ditem);
		switch (ditem) {
			case	FTPUcanchangeCWD:
				FlipCheckBox(dptr, ditem);
				break;
			
			case	FTPUDfltDirButton:
				if (SelectDirectory(&vRefNum, &FTPUptr->DefaultDirDirID)) {
					PathNameFromDirID(FTPUptr->DefaultDirDirID, vRefNum, scratchPstring);
					SetTEText(dptr, FTPUDfltDirDsply, scratchPstring);
				}
				break;
				
			default:
				break;
			
			} // switch
		} // while
		
	if (ditem == 2) {
		if (IsNewPrefRecord) DisposeHandle((Handle) FTPUHdl);
		else ReleaseResource((Handle) FTPUHdl);
		
		DisposeDialog(dptr);
		ResetMenus();
		return(FALSE);			// No changes should be made.
		}
		
	FTPUptr->UserCanCWD = GetCntlVal(dptr, FTPUcanchangeCWD);
	GetDirectoryName(vRefNum, 2, FTPUptr->DefaultDirVolName);
	
	GetTEText(dptr, FTPUusername, PrefRecordNamePtr);
	GetTEText(dptr, FTPUpassword, scratchPstring);
	
	if (scratchPstring[1] != '¥') {
		// Encrypt the new (or possibly modified) password.
		PtoCstr(scratchPstring);
		Sencompass((char *)scratchPstring, (char *)scratchPstring2);
		CtoPstr((char *)scratchPstring2);
		BlockMoveData(scratchPstring2, FTPUptr->EncryptedPassword, StrLength(scratchPstring2)+1);
		}
			
	if (IsNewPrefRecord) {
		UseResFile(TelInfo->SettingsFile);
		resourceID = UniqueID(FTPUSERPREFS_RESTYPE);
		AddResource((Handle)FTPUHdl, FTPUSERPREFS_RESTYPE, resourceID, PrefRecordNamePtr);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)FTPUHdl);
		}
	else {
		UseResFile(TelInfo->SettingsFile);
		GetResInfo((Handle)FTPUHdl, &resourceID, &scratchResType,(StringPtr) &scratchPstring);
		SetResInfo((Handle)FTPUHdl, resourceID, PrefRecordNamePtr);
		ChangedResource((Handle)FTPUHdl);
		UpdateResFile(TelInfo->SettingsFile);
		ReleaseResource((Handle)FTPUHdl);
		}
	
	DisposeDialog(dptr);
	ResetMenus();
	return(TRUE);			// A resource has changed or been added.	
}

short AnsiPrompt(short allowDefaultBoldSelect, short *defaultBoldColor)
{
  /* puts up the dialog that lets the user examine and change the ANSI color
	settings for the specified window. */

	short		scratchshort, ditem;
	Point			ColorBoxPoint;
	DialogPtr		dptr;
	Boolean			UserLikesNewColor;
	RGBColor	 	scratchRGB;
	short			itemType;
	Handle			itemHandle;
	Rect			itemRect;

	SetUpMovableModalMenus();
	dptr = GetNewMySmallDialog(ANSIColorDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 2);
	SetDialogTracksCursor(dptr, 1);
	NumberOfColorBoxes = 16;
	if ( allowDefaultBoldSelect && defaultBoldColor ) {
		if ( *defaultBoldColor == -1 )
			SetCntrl(dptr, ANSIForegroundRadio, 1);
		else
			SetCntrl(dptr, ANSIBlackRadio + *defaultBoldColor, 1);
	} else {
		for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++)
			HideDialogItem(dptr, scratchshort + ANSIBlackRadio);
		HideDialogItem(dptr, 13);
		HideDialogItem(dptr, 14);
		HideDialogItem(dptr, 47);
	}
	for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++) 
	{
		BoxColorItems[scratchshort] = ANSIBlack + scratchshort;
		GetEntryColor(TelInfo->AnsiColors, scratchshort, &BoxColorData[scratchshort]);
		UItemAssign( dptr, scratchshort + ANSIBlack, ColorBoxItemProcUPP);
	}

	ColorBoxPoint.h = 0;			// Have the color picker center the box on the main
	ColorBoxPoint.v = 0;			// screen
	NumOnly[0] = 12; //safe item
	ditem = 3;	

	ShowWindow(dptr);

	while (ditem > 2) {
		movableModalDialog(ColorBoxModalProcUPP, &ditem);
		switch (ditem) {
			case	ANSIBlack:
			case	ANSIRed:	
			case	ANSIGreen:	
			case	ANSIYellow:	
			case	ANSIBlue:	
			case	ANSIMagenta:	
			case	ANSICyan:	
			case	ANSIWhite:
            case	ANSIBoldBlack:
			case	ANSIBoldRed:
			case	ANSIBoldGreen:
			case	ANSIBoldYellow:
			case	ANSIBoldBlue:
			case	ANSIBoldMagenta:
			case	ANSIBoldCyan:
			case	ANSIBoldWhite:
				if (TelInfo->haveColorQuickDraw) {
					Str255 askColorString;
					GetIndString(askColorString,MISC_STRINGS,PICK_NEW_COLOR_STRING);
					UserLikesNewColor = GetColor(ColorBoxPoint,askColorString,
						 &BoxColorData[ditem-ANSIBlack], &scratchRGB);
					if (UserLikesNewColor) {
						BoxColorData[ditem-ANSIBlack] = scratchRGB;
						// force refresh
						GetDialogItem(dptr, ditem, &itemType, &itemHandle, &itemRect);
						InvalRect(&itemRect);
					}
				}
				break;
			case	ANSIBlackRadio:
			case	ANSIRedRadio:
			case	ANSIGreenRadio:
			case	ANSIYellowRadio:
			case	ANSIBlueRadio:
			case	ANSIMagentaRadio:
			case	ANSICyanRadio:
			case	ANSIWhiteRadio:
            case	ANSIBoldBlackRadio:
			case	ANSIBoldRedRadio:
			case	ANSIBoldGreenRadio:
			case	ANSIBoldYellowRadio:
			case	ANSIBoldBlueRadio:
			case	ANSIBoldMagentaRadio:
			case	ANSIBoldCyanRadio:
			case	ANSIBoldWhiteRadio:
			case	ANSIForegroundRadio:
				for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++)
					SetCntrl(dptr, ANSIBlackRadio + scratchshort, 0);
				if ( ditem != ANSIForegroundRadio )
					SetCntrl(dptr, ANSIForegroundRadio, 0);
				SetCntrl(dptr, ditem, 1);
			default:
				break;
			
			} // switch
		} // while

	if (ditem == DLOGCancel)
	{
		DisposeDialog(dptr);
		ResetMenus();
		return -1;
	}
	if ( allowDefaultBoldSelect && defaultBoldColor ) {
		if ( GetCntlVal(dptr, ANSIForegroundRadio) ) {
			*defaultBoldColor = -1;
		} else {
			for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++) {
				if (GetCntlVal(dptr, ANSIBlackRadio + scratchshort)) {
					*defaultBoldColor = scratchshort;
					break;
				}
			}
		}
	}
	for (scratchshort = 0; scratchshort < NumberOfColorBoxes; scratchshort++) 
		SetEntryColor(TelInfo->AnsiColors, scratchshort, &BoxColorData[scratchshort]);

	DisposeDialog(dptr);
	ResetMenus();
}

void
DeleteRange(char *buffer,short start,short end)
{	register char	*src,*dest,*last;
	
	last = buffer + *buffer;
	
	src = buffer + end + 1;
	dest = buffer + start + 1;
	
	while (src <= last)			// Shift character to the left over the removed characters
		*(dest++) = *(src++);
	
	(*buffer) -= (end-start);	// Adjust the buffer's length
}

void
InsertChar(char *buffer,short pos,char c)
{	register short	index,len;
	
	len = *buffer;
	
	if (len == 0xFF)		// if the string is full, return
		return;
	
	for (index = len;index > pos;index--)	// Shift characters to the right to make room
		buffer[index+1] = buffer[index];
	
	buffer[pos+1] = c;		// Fill in the new character
	
	(*buffer)++;			// Add one to the length of the string
}

OSErr
GetScrapHandle( FlavorType dataFlavor, Handle *dataHandle )
{
	SInt32 scrapOffset;
	SInt32 scrapResult;
	OSErr err;

	*dataHandle = NewHandle(0);
	if ( ( err = MemError() ) != noErr )
		return err;
	if ( ( scrapResult = GetScrap( *dataHandle, dataFlavor, &scrapOffset ) ) < 0 ) {
		err = scrapResult;
		DisposeHandle( *dataHandle );
		return err;
	}
	return noErr;
}

Boolean PasswordFilter(DialogPtr dptr, EventRecord *evt, unsigned char *buf, short max)
{
	short	key;
	short	start;
	short	end;
	Handle	hText;
	char	*pbuf;

	key = evt->message & charCodeMask;
	switch (key) {
		case '.':			// Cmd-.
			if (!(evt->modifiers & cmdKey))
				break;
		case '\r':			// Return
		case '\003':		// Enter
		case '\033':		// Esc
		case '\t':			// Tab
		case '\034':		// Left arrow
		case '\035':		// Right arrow
		case '\036':		// Up arrow
		case '\037':		// Down arrow
			return 0;
		default:			// Everything else falls through to be dealt with
			break;			//	below
	}

	start = (**((DialogPeek)dptr)->textH).selStart;	// Get the current selection
	end = (**((DialogPeek)dptr)->textH).selEnd;
	hText = TEGetText(((DialogPeek)dptr)->textH);

	pbuf = (char*)buf;
	if (start != end) {					// If there's a selection, delete it
		DeleteRange(pbuf,start,end);
	}
	if (key == '\010') {	// Backspace
		if (start != 0) {
			DeleteRange(pbuf,start-1,start);	// Delete the character to the left
		}
	} else {
		if ((evt->modifiers & cmdKey)) {
			if (key == 'v' || key == 'V') {
				// get scrap
				Handle hText;
				OSErr err;
				if ((err = GetScrapHandle('TEXT', &hText)) == noErr) {
					int len = GetHandleSize(hText);
					if (len + *pbuf < max) {
						int i;
						for (i = 0; i < len; i++) {
							InsertChar(pbuf,start++,(*hText)[i]); // Insert the real key into the buffer
							TEKey('¥', ((DialogPeek)dptr)->textH);
						}
					}
					// add scrap to textedit view
					DisposeHandle(hText);
					evt->what = nullEvent;
				}
			} else if ( key == 'x' || key == 'X'
					 || key == 'c' || key == 'C' ) {
				SysBeep(5);
				evt->what = nullEvent;
			}
		} else {
			InsertChar(pbuf,start,key);		// Insert the real key into the buffer
			evt->message = '¥';				// Character to use in field
		}
	}
	return 0;
}

// Connections.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "telneterrors.h"
#include "DlogUtils.proto.h"
#include "movableModal.h"

#include "wind.h"
#include "event.proto.h"

#include "rsinterf.proto.h"
#include "vsdata.h"
#include "vskeys.h"
#include "vsinterf.proto.h"
#include "vgtek.proto.h"
#include "tekrgmac.proto.h"
#include "vr.h"
#include "vrrgmac.proto.h" 
#include "network.proto.h"
#include "mydnr.proto.h"
#include "InternalEvents.h"
#include "menuseg.proto.h"
#include "maclook.proto.h"
#include "parse.proto.h"
#include "parse.h"
#include "configure.proto.h"
#include "netevent.proto.h"
#include "linemode.proto.h"
#include "mainseg.proto.h"
#include "prefs.proto.h"
#include "popup.h"
#include "popup.proto.h"
#include "translate.proto.h"

#include "Connections.proto.h"
#include "tnae.h"
#include "authencrypt.h"
#include "authencrypt.proto.h"
#include "wdefpatch.proto.h"
#include "LinkedList.proto.h"
#include "ae.proto.h"
#include "sshglue.proto.h"
#include "macros.proto.h"

#include "PasswordDialog.h"

/*	These are all of the variables we need... */

extern	Cursor	*theCursors[NUMCURS];		/* all the cursors in a nice bundle */
extern	WindRec	*screens;
extern	short	scrn;
extern	short	nNational;				// Number of user-installed translation tables
extern  short   gTableCount;
extern	MenuHandle	myMenus[];
extern	Boolean	authOK;
extern	Boolean	encryptOK;
extern	unsigned char *gReadspace;
extern	short	gBlocksize;

extern ConstStringPtr gDefaultName;

static short	numWind = 1;
static short	stagNum = 1;

static Boolean startautocomplete = false;
static Boolean doneautocomplete = false;
static unsigned long autoTicks;

/*
extern void LockDialog();
extern void UnlockDialog();
*/

static	void setSessStates(DialogPtr dptr);
static short FindMenuItemText(MenuHandle hMenu, StringPtr itemString);
static short numberKerberosConnections(void);

static	pascal short POCdlogfilter( DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(POCdlogfilter, ModalFilter);

short getDefaultPort(short protocol)
{
	short port;
	short protocolArray[] = {23, 513, 514, 512, 22, 23};

	if ((protocol < 0) || (protocol > 5)) return 23; // default if something goes weird
	return protocolArray[protocol];
}

void OpenPortSpecial(MenuHandle menuh, short item)
{
	ConnInitParams		**theParams;
	Boolean				success;
	Str255				scratchPstring;
	Boolean				wasAlias;
	
	GetMenuItemText(menuh, item, scratchPstring);
	
	theParams = NameToConnInitParams(scratchPstring, TRUE, 0, &wasAlias);
	if (theParams == NULL) {
		OutOfMemory(1020);
		return;
	}
	// use favorite's name as window name
	if ( !(**theParams).WindowName[0] && memcmp(scratchPstring, gDefaultName, gDefaultName[0] + 1) ) {
		BlockMoveData(scratchPstring, (**theParams).WindowName, scratchPstring[0] + 1);
		NumToString(numWind++, scratchPstring);
		pstrcat((**theParams).WindowName, "\p (");
		pstrcat((**theParams).WindowName, scratchPstring);
		pstrcat((**theParams).WindowName, "\p)");
	}
	success = CreateConnectionFromParams(theParams);
}

SIMPLE_UPP(POCdlogfilter, ModalFilter);
pascal short POCdlogfilter( DialogPtr dptr, EventRecord *evt, short *item)
{
	short key;
	short result;
	short editField;
	Str255 scratch1Pstring;
	Str255 scratch2Pstring;

	if (evt->what == keyDown) {
		if (evt->modifiers & cmdKey) {
			key = evt->message & charCodeMask;
			if ( key == 'A' || key == 'a' ) {
				*item = NCauthenticate;
				return -1;
			}
			if ( key == 'E' || key == 'e' ) {
				*item = NCencrypt;
				return -1;
			}
			if ( key == 'S' || key == 's' ) {
				*item = NCssh2;
				return -1;
			}
			if ( key == 'F' || key == 'f' ) {
				*item = NCforward;
				return -1;
			}
		}
	}
	if ((evt->what == keyDown) || (evt->what == autoKey)) {
		key = evt->message & charCodeMask;
		if (key == 0x1F) {
			*item = 1000;
			return(-1);
		}
		if (key == 0x1E) {
			*item = 1001;
			return(-1);
		}
	}


//	if (evt->what == mouseDown)
//		return(PopupMousedown(dptr, evt, item));

// RAB BetterTelnet 1.2 - we let StdFilterProc handle this now
//	return(DLOGwOK_Cancel(dptr, evt, item));
	result = CallStdFilterProc(dptr, evt, item);

	editField = ((DialogPeek)dptr)->editField + 1;
	if ( startautocomplete &&
		(LMGetTicks() - autoTicks >= 30 || editField != NChostname) ) {
		startautocomplete = false;
		doneautocomplete = true;
		*item = NChostname;
		result = -1;
	}

	return result;
}

static short FindMenuItemText(MenuHandle hMenu, StringPtr itemString)
{
	short i;
	short n;
	Str255 scratchPstring;
	
	n = CountMItems(hMenu);
	for (i = 1; i <= n; i++) {
		GetMenuItemText(hMenu, i, scratchPstring);
		if (!memcmp(itemString, scratchPstring, itemString[0] + 1)) {
			return i;
		}
	}
	return 0;
}


static Str255	sLastHostName;

static void SetCurrentSession(DialogPtr dptr, Str255 scratchPstring)
{
	SessionPrefs	**tempSessHdl;
	Str31			scritchPstring;

	tempSessHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, scratchPstring, sizeof(SessionPrefs));
	if (tempSessHdl) {
		SetTEText(dptr, NCfavoritename, scratchPstring);//update the favoritename
//						TerminalIndex = findPopupMenuItem(TermPopupHdl,
//								(**tempSessHdl).TerminalEmulation);
//						TPopup[0].choice = TerminalIndex;
//						DrawPopUp(dptr, NCtermpopup); //update popup
		strcpy((char *)scratchPstring, (char *)(**tempSessHdl).hostname);

		if ((**tempSessHdl).port != getDefaultPort((**tempSessHdl).protocol)) {
				NumToString((unsigned short)(**tempSessHdl).port, scritchPstring);
				pstrcat(scratchPstring, "\p:");
				if ((**tempSessHdl).portNegative)
					pstrcat(scratchPstring, "\p-");
				pstrcat(scratchPstring, scritchPstring);
		}
		/* recall last hostname seen if none */
		if ( !scratchPstring[0] && sLastHostName[0] ) {
			SetTEText(dptr, NChostname, sLastHostName);
		} else {
			SetTEText(dptr, NChostname, scratchPstring);//update the hostname
		}
		SelectDialogItemText(dptr, NChostname, 0, 32767);
		SetCntrl(dptr, NCauthenticate, (**tempSessHdl).authenticate);//update the auth status
		SetCntrl(dptr, NCencrypt, (**tempSessHdl).encrypt);
		SetCntrl(dptr, NCforward, (**tempSessHdl).forward);
		SetCntrl(dptr, NCssh2, (**tempSessHdl).protocol == 4);
		setSessStates(dptr);//encrypt cant be on w/o authenticate
		ReleaseResource((Handle)tempSessHdl);
	}
}

Boolean PresentOpenConnectionDialog(void)
{
	ConnInitParams	**InitParams;
	DialogPtr		dptr;
	short			ditem, scratchshort, mystrpos;
	Boolean			success;
	long			scratchlong;
	Str255			hostString;
	Str255			scratchPstring;
	Str255			terminalPopupString;
	Str255			favoriteString;
	Handle			ItemHandle;
	SessionPrefs	**tempSessHdl;
	short 			numberOfTerms, sessMark, requestPort;
	MenuHandle		SessPopupHdl;
//	MenuHandle		TermPopupHdl;
	Rect			scratchRect;
	Point			SessPopupLoc;
	short			TerminalIndex, itemNumber = 1;
//	popup TPopup[] = {{NCtermpopup, (MenuHandle) 0, 1},
//						{0, (MenuHandle) 0, 0}};
	Size 			junk;
	LinkedListNode	*theHead;
	Boolean			portSet;
	Boolean			wasAlias;
	Boolean			typedHost;
	Boolean			parseAliases;

	setLastCursor(theCursors[normcurs]);


	SetUpMovableModalMenus();
	dptr = GetNewMyDialog(NewCnxnDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	if (dptr == NULL) {
		ResetMenus();
		OutOfMemory(1000);
		return;
	}
		
	SetDialogDefaultItem(dptr, 1);
	SetDialogCancelItem(dptr, 2);
	SetDialogTracksCursor(dptr, 1);
	ditem = 3;
	sessMark = 1;

	GetIndString(scratchPstring,MISC_STRINGS,SESSION_STRING);
	SessPopupHdl = NewMenu(668, scratchPstring);
	if (SessPopupHdl == NULL) {
		DisposeDialog(dptr);
		ResetMenus();
		OutOfMemory(1000);
		return;
	}
	UseResFile(TelInfo->SettingsFile);
	numberOfTerms = Count1Resources(SESSIONPREFS_RESTYPE);
	theHead  = createSortedList(SESSIONPREFS_RESTYPE,numberOfTerms,"\p<Default>");
	EnableItem(SessPopupHdl, 0);		// Make sure the entire menu is enabled
	addListToMenu(SessPopupHdl, theHead, 1);
	deleteList(&theHead);

	SetItemMark(SessPopupHdl, 1, 18);

	GetDialogItem(dptr, NCsesspopup, &scratchshort, &ItemHandle, &scratchRect);
	SessPopupLoc.h = scratchRect.left;
	SessPopupLoc.v = scratchRect.top;
	
//	TermPopupHdl = NewMenu(666, "\p");
//	if (TermPopupHdl == NULL) {
//		DisposeHandle((Handle)SessPopupHdl);
//		DisposeDialog(dptr);
//		ResetMenus();
//		OutOfMemory(1000);
//		return;
//		}
	SetPort(dptr);
	LocalToGlobal(&SessPopupLoc);

//	numberOfTerms = Count1Resources(TERMINALPREFS_RESTYPE);
//	theHead  = createSortedList(TERMINALPREFS_RESTYPE,numberOfTerms,"\p<Default>");
//	addListToMenu(TermPopupHdl, theHead);
//	deleteList(&theHead);
//	TPopup[0].h = TermPopupHdl;
//	PopupInit(dptr, TPopup);
	
	if (!authOK) {
		Hilite( dptr, NCauthenticate, 255);
		Hilite( dptr, NCencrypt, 255);
		Hilite( dptr, NCforward, 255);
	} else if (!encryptOK) {
		Hilite( dptr, NCencrypt, 255);
	}

	BlockMoveData("\p<Default>", scratchPstring, 15);
	SetCurrentSession(dptr, scratchPstring);

	GetTEText(dptr, NChostname, hostString);

//	TerminalIndex = findPopupMenuItem(TermPopupHdl,(**defaultSessHdl).TerminalEmulation);
//	TPopup[0].choice = TerminalIndex;
//	PopupInit(dptr, TPopup);

	SelectDialogItemText(dptr, NChostname, 0, 32767);

	typedHost = false;

	ShowWindow(dptr);

	startautocomplete = false;
	doneautocomplete = false;

	while (ditem != NCconnect && ditem != NCcancel) {
		movableModalDialog(POCdlogfilterUPP, &ditem);
		switch(ditem) 
		{
			case	NCauthenticate:
			case	NCencrypt:
			case	NCforward:
				GetDialogItem(dptr, ditem, &scratchshort, &ItemHandle, &scratchRect);
				if ((**(ControlHandle)ItemHandle).contrlHilite == 0) {	// if control not disabled
					FlipCheckBox(dptr, ditem);
					setSessStates(dptr);
				}
				break;

			case	NCssh2:
				GetDialogItem(dptr, ditem, &scratchshort, &ItemHandle, &scratchRect);
				FlipCheckBox(dptr, ditem);
				setSessStates(dptr);
				break;

			case    NChostname:
				if ( gApplicationPrefs->parseAliases ) {
					typedHost = true;
					GetTEText(dptr, NChostname, scratchPstring);
					if (memcmp(scratchPstring, hostString, scratchPstring[0] + 1)) {
						// host name changed
						autoTicks = LMGetTicks();
						startautocomplete = true;
						pstrcpy(hostString, scratchPstring);
					}
					if ( !startautocomplete && doneautocomplete ) {
						doneautocomplete = false;
						// check if the string matches a favorite name
						scratchshort = FindMenuItemText(SessPopupHdl, scratchPstring);
						if ( scratchshort /*&& sessMark != scratchshort*/ ) {
							short editField = ((DialogPeek)dptr)->editField + 1;
							short start = (**((DialogPeek)dptr)->textH).selStart;	// Get the current selection
							short end = (**((DialogPeek)dptr)->textH).selEnd;
							SetItemMark(SessPopupHdl, sessMark, 0);
							sessMark = scratchshort;
							SetItemMark(SessPopupHdl, sessMark, 18);
							GetMenuItemText(SessPopupHdl, scratchshort, scratchPstring);
							SetCurrentSession(dptr, scratchPstring);
							if ( editField != NChostname )
								SelectDialogItemText(dptr, editField, start, end);
						}
					}
				}
				break;

			case	NCsesspopup:
				GetDialogItem(dptr, NCsesspopup, &scratchshort, &ItemHandle, &scratchRect);
				SessPopupLoc.h = scratchRect.left;
				SessPopupLoc.v = scratchRect.top;
				SetPort(dptr);
				LocalToGlobal(&SessPopupLoc);

				InsertMenu(SessPopupHdl, hierMenu);
				CalcMenuSize(SessPopupHdl);
				scratchlong = PopUpMenuSelect(SessPopupHdl, SessPopupLoc.v,
												SessPopupLoc.h, 0);
				DeleteMenu(668);
				if (scratchlong) {
					typedHost = false;
					scratchshort = scratchlong & 0xFFFF; //	Apple sez ignore the high word
					SetItemMark(SessPopupHdl, sessMark, 0);
					sessMark = scratchshort;
					SetItemMark(SessPopupHdl, sessMark, 18);
					GetMenuItemText(SessPopupHdl, scratchshort, scratchPstring);
					SetCurrentSession(dptr, scratchPstring);
				}
				break;
			case 1001:
				typedHost = false;
				SetItemMark(SessPopupHdl, sessMark, 0);
				sessMark--;
				if (sessMark < 1)
					sessMark = CountMItems(SessPopupHdl);
				SetItemMark(SessPopupHdl, sessMark, 18);
				GetMenuItemText(SessPopupHdl, sessMark, scratchPstring);
				SetCurrentSession(dptr, scratchPstring);
				break;
			case 1000:
				typedHost = false;
				SetItemMark(SessPopupHdl, sessMark, 0);
				sessMark++;
				if (sessMark > CountMItems(SessPopupHdl))
					sessMark = 1;
				SetItemMark(SessPopupHdl, sessMark, 18);
				GetMenuItemText(SessPopupHdl, sessMark, scratchPstring);
				SetCurrentSession(dptr, scratchPstring);
				break;
			default:
				break;
		} // switch
	} // while
	
	
	if (ditem == NCcancel) {
//		PopupCleanup();
		DisposeMenu(SessPopupHdl);	// drh Ñ Bug fix: memory leak
		DisposeDialog(dptr);
		ResetMenus();
		return;
		}
	
	GetTEText(dptr, NChostname, scratchPstring);
	if (!StrLength(scratchPstring)) {
//		PopupCleanup();
		DisposeMenu(SessPopupHdl);	// drh Ñ Bug fix: memory leak
		DisposeDialog(dptr);
		ResetMenus();
		return;
		}
	
	pstrcpy(sLastHostName, scratchPstring);

//	GetMenuItemText(TPopup[0].h, TPopup[0].choice, terminalPopupString);
//	PopupCleanup();
	portSet = false;

	for (mystrpos = 0; mystrpos < StrLength(scratchPstring); mystrpos++) {
		if (scratchPstring[mystrpos + 1] == ':') {
			scratchPstring[mystrpos + 1] = ' ';
			portSet = true;
			break;
		}
	}
	mystrpos = StrLength(scratchPstring);
	while (mystrpos > 0 && scratchPstring[mystrpos] == ' ')
		--mystrpos;
	scratchshort = mystrpos;
	while (mystrpos > 0 && scratchPstring[mystrpos] >= '0' && scratchPstring[mystrpos] <= '9')
		--mystrpos;
	if ( mystrpos != scratchshort && scratchPstring[mystrpos] == ' ' ) {
		// terminated by [spc][digits]
		portSet = true;
	}

	MaxMem(&junk);
	GetMenuItemText(SessPopupHdl, sessMark, favoriteString);

	/* don't try to convert hostname to alias if nothing has been typed */
	parseAliases = gApplicationPrefs->parseAliases;
	if (!typedHost)
		gApplicationPrefs->parseAliases = false;
	InitParams = NameToConnInitParams(scratchPstring, FALSE, favoriteString, &wasAlias);
	gApplicationPrefs->parseAliases = parseAliases;
	if (InitParams == NULL)
	{
		DisposeMenu(SessPopupHdl);	// drh Ñ Bug fix: memory leak
		DisposeDialog(dptr);
		ResetMenus();
		OutOfMemory(1000);
		return;
	}

//	if ((**InitParams).terminal == NULL)  //if this is not null, then the string was an alias,
//	{										// so dont use the popup terminal
//		(**InitParams).terminal = (TerminalPrefs **)
//				Get1NamedResource(TERMINALPREFS_RESTYPE,terminalPopupString);
//		DetachResource((Handle)(**InitParams).terminal);
//		if (InitParams == NULL) {
//			OutOfMemory(1000);
//			DisposeDialog(dptr);
//			ResetMenus();
//			return;
//			}
//	}

// RAB BetterTelnet 2.0b1 - lock the handles down FIRST!

	HLock((Handle)InitParams);
	HLock((Handle)(**InitParams).session);

  	(**(**InitParams).session).authenticate = GetCntlVal(dptr, NCauthenticate);
  	(**(**InitParams).session).encrypt = GetCntlVal(dptr, NCencrypt);
  	(**(**InitParams).session).forward = GetCntlVal(dptr, NCforward);

 	if ( GetCntlVal(dptr, NCssh2) ) {
		if ((**(**InitParams).session).protocol != 4) {
			(**(**InitParams).session).protocol = 4;
			if ( !portSet ) {
				(**(**InitParams).session).port = getDefaultPort(4);
			}
		}
 	} else {
		if ((**(**InitParams).session).protocol == 4) {
			(**(**InitParams).session).protocol = 0;
			if ( !portSet ) {
				(**(**InitParams).session).port = getDefaultPort(0);
			}
		}
 	}

	GetTEText(dptr, NCwindowname, scratchPstring);

	// Copy over the user specified window name.  If blank, CreateConnectionFromParams 
	// will copy the hostname to the windowname and append a number.
	if (StrLength(scratchPstring)) 
		BlockMoveData(scratchPstring, (**InitParams).WindowName,
					(StrLength(scratchPstring) > 63) ? 64 : (StrLength(scratchPstring) + 1));

	HUnlock((Handle)(**InitParams).session);
	HUnlock((Handle)InitParams);

	DisposeMenu(SessPopupHdl);
	DisposeDialog(dptr);
	ResetMenus();
	
	// use favorite's name as window name
	if ( !(**InitParams).WindowName[0] && memcmp(favoriteString, gDefaultName, gDefaultName[0] + 1) ) {
		BlockMoveData(favoriteString, (**InitParams).WindowName, favoriteString[0] + 1);
		NumToString(numWind++, favoriteString);
		pstrcat((**InitParams).WindowName, "\p (");
		pstrcat((**InitParams).WindowName, favoriteString);
		pstrcat((**InitParams).WindowName, "\p)");
	}

	success = CreateConnectionFromParams(InitParams);
	return success;
}

// Set states of session checkboxes
static	void setSessStates (DialogPtr dptr)
{		
	if (GetCntlVal(dptr, NCauthenticate)) {
		Hilite(dptr, NCencrypt, (encryptOK)? 0 : 255);
		Hilite(dptr, NCforward, 0);
	} else {
		Hilite(dptr, NCencrypt, 255);
		SetCntrl(dptr, NCencrypt, false);
		Hilite(dptr, NCforward, 255);
		SetCntrl(dptr, NCforward, false);
	}
}

Boolean OpenConnectionFromURL(char *host, char *portstring, char *user, char *password, short ssh)
{
	ConnInitParams	**Params;
	Str255			windowName, tempString;
	short			len;
	long			port;
	Boolean			success;
	
	Params = ReturnDefaultConnInitParams();


	windowName[0] = 0;

	// Set up window name if user (and password) given
	if ( user != NULL && ssh == 0 ) {
		GetIndString(windowName, MISC_STRINGS, MISC_USERPRMPT);
		len = strlen(user);
		BlockMoveData(user,& windowName[StrLength(windowName)+1], len);
		windowName[0] += len;
		if (password != NULL) {
			GetIndString(tempString, MISC_STRINGS, MISC_PSWDPRMPT);
			BlockMoveData(&tempString[1], &windowName[StrLength(windowName)+1], tempString[0]);
			windowName[0] += tempString[0];
			len = strlen(password);
			BlockMoveData(password, &windowName[StrLength(windowName)+1], len);
			windowName[0] += len;
		}

		if (windowName[0] != 0) {
			BlockMoveData(windowName, (**Params).WindowName, StrLength(windowName)+1);
		}
	}

	CtoPstr(host);
	BlockMoveData(host, (**(**Params).session).hostname, host[0]+1);
	
	if ( ssh != 0 ) {
		(**(**Params).session).protocol = 4;
		(**(**Params).session).port = getDefaultPort(4);
		if ( user != NULL ) {
			strcpy((**(**Params).session).username, user);
			CtoPstr((**(**Params).session).username);
		}
		if ( password != NULL ) {
			strcpy((**(**Params).session).password, password);
			CtoPstr((**(**Params).session).password);
		}
	} else if ( (**(**Params).session).protocol == 4 ) {
		/* default to telnet */
		(**(**Params).session).protocol = 0;
		(**(**Params).session).port = getDefaultPort(0);
	}

	if (portstring != nil) {
		CtoPstr(portstring);
		StringToNum((StringPtr)portstring, &port);
		(**(**Params).session).port = port;
	}
		
	success = CreateConnectionFromParams(Params);
	return success;
}	

// RAB BetterTelnet 2.0fc1 - updated for SOCKS
Boolean CreateConnectionFromParams( ConnInitParams **Params)
{
	short			scratchshort, fontnumber, otherfnum;
	SessionPrefs	*SessPtr;
	TerminalPrefs	*TermPtr;
	short			cur;
	Str32			numPstring;
	Str255			scratchPstring;
	Boolean			scratchBoolean;
	WindRec			*theScreen;
	unsigned char	*hostname;
	unsigned long	flags;
	unsigned char	localIP[4];

	setLastCursor(theCursors[watchcurs]);					/* We may be here a bit */

	// Check if we have the max number of sessions open
	if (TelInfo->numwindows == MaxSess) return(FALSE);
	
	cur = TelInfo->numwindows++;		// Adjust # of windows and get this window's number
	theScreen = &screens[cur];
	
	theScreen->active = CNXN_NOTINUSE;	// Make sure it is marked as dead (in case we
										// abort screen creation after initiating DNR)
										// That way CompleteConnectionOpening will know
										// we didn't make it.
	HLockHi((Handle)Params);
	HLockHi((Handle)(**Params).terminal);
	HLockHi((Handle)(**Params).session);
	SessPtr = *((**Params).session);
	TermPtr = *((**Params).terminal);
	
	if (StrLength((**Params).WindowName) == 0) {
		BlockMoveData((**(**Params).session).hostname, (**Params).WindowName, 
					StrLength((**(**Params).session).hostname)+1);
		if (SessPtr->port != getDefaultPort(SessPtr->protocol)) {
			NumToString((unsigned short)SessPtr->port, numPstring);
			pstrcat((**Params).WindowName, "\p:");
			pstrcat((**Params).WindowName, numPstring);
		}
		NumToString(numWind++, numPstring);
		pstrcat((**Params).WindowName, "\p (");
		pstrcat((**Params).WindowName, numPstring);	// tack the number onto the end.
		pstrcat((**Params).WindowName, "\p)");
	}

	if (SessPtr->hostname[0] == 0) {
		OperationFailedAlert(5, 0, 0);
		goto failed;
	}
/* we have SSH.
	if (SessPtr->protocol == 4) // make sure we have SSH
	{
		if (!ssh_glue_installed()) {
			OperationFailedAlert(6, 0, 0);
			goto failed;
		}
	}
*/

	Mnetinit();	// RAB BetterTelnet 1.0fc4

	// RAB BetterTelnet 2.0fc1 - we look up the SOCKS firewall first
	// heck, we might even be using SOCKS 4a, in which case that's ALL we directly look up
	if (SessPtr->usesocks) {
		hostname = (unsigned char *)SessPtr->sockshost;
		theScreen->sockslookup = 1; // tell the DNR completion we need to make another look up,
									// and that this one should not be reversed (see below)
									// but the next one can be
	} else {
		hostname = SessPtr->hostname;
		theScreen->sockslookup = 0; // ok to reverse lookup after
									// the idea here is that we need the reverse DNS
									// (canonical machine name) for Kerberos, so we might
									// as well (if using Kerberos, otherwise we don't reverse)
									// get it for the remote host and not the firewall
	}



/*
	// Get the IP for the host while we set up the connection
	if (DoTheDNR(hostname, cur) != noErr) {
		OutOfMemory(1010);
		goto failed;
	}
*/
	DoTheMenuChecks();

	theScreen->sessmacros = (**Params).sessmacros;

  	theScreen->authenticate = SessPtr->authenticate;
  	theScreen->encrypt = SessPtr->encrypt;
  	theScreen->forward = SessPtr->forward;
 
    theScreen->aedata = NULL;
 	
 	for (scratchshort = 0; scratchshort < sizeof(theScreen->myopts); scratchshort++) {
		theScreen->myopts[scratchshort] = 0;
		theScreen->hisopts[scratchshort] = 0;		
	}	
	theScreen->cannon[0] = '\0';

	theScreen->usesocks = theScreen->socksinprogress = SessPtr->usesocks;
	theScreen->sockspos = 0; // for receiving socks reply
	theScreen->socks4a = SessPtr->socks4a;
	theScreen->socksport = SessPtr->socksport;
	pstrcpy((unsigned char *)theScreen->socksusername, (unsigned char *)SessPtr->socksusername);
	pstrcpy((unsigned char *)theScreen->sockshost, (unsigned char *)SessPtr->sockshost);

	theScreen->vtemulation = TermPtr->vtemulation;
	theScreen->forcesave = SessPtr->forcesave;
	theScreen->eightbit = TermPtr->eightbit;
	theScreen->portNum = SessPtr->port;
	theScreen->portNegative = SessPtr->portNegative;
	theScreen->allowBold = TermPtr->allowBold;
	theScreen->colorBold = TermPtr->colorBold;
	theScreen->realbold = TermPtr->realbold;
	theScreen->keepselection = SessPtr->keepselection;
	theScreen->inversebold = TermPtr->boldFontStyle;
	theScreen->ignoreBeeps = SessPtr->ignoreBeeps;
	theScreen->otpauto = SessPtr->otpauto;
	theScreen->otpnoprompt = SessPtr->otpnoprompt;
	theScreen->otphex = SessPtr->otphex;
	theScreen->otpmulti = SessPtr->otpmulti;
	theScreen->otpsavepass = SessPtr->otpsavepass;
	theScreen->oldScrollback = TermPtr->oldScrollback;
	theScreen->protocol = SessPtr->protocol;
	pstrcpy((unsigned char *)theScreen->otppassword, (unsigned char *)SessPtr->otppassword);
	pstrcpy((unsigned char *)theScreen->username, (unsigned char *)SessPtr->username);
	pstrcpy((unsigned char *)theScreen->password, (unsigned char *)SessPtr->password);
	pstrcpy((unsigned char *)theScreen->clientuser, (unsigned char *)SessPtr->clientuser);
	pstrcpy((unsigned char *)theScreen->command, (unsigned char *)SessPtr->command);
	theScreen->otpautostate = 0;
	theScreen->otpautobuffer[7] = 0;
	theScreen->emacsmeta = TermPtr->emacsmetakey;
	theScreen->Xterm = TermPtr->Xtermsequences;
	theScreen->remapCtrlD = TermPtr->remapCtrlD;
	theScreen->keypadmap = TermPtr->remapKeypad;
	theScreen->port = -1;				// netxopen will take care of this
	theScreen->cachedFontNum = 0;
	theScreen->cachedBoldFontNum = 0;
	theScreen->cxWeHaveAppleEvent = 0;
	theScreen->waWeHaveAppleEvent = 0;
	theScreen->waWaiting = 0;
	theScreen->encryption = SessPtr->encryption;
	theScreen->jumpScroll = TermPtr->jumpScroll;
	theScreen->jsNoFlush = 0;
	theScreen->realBlink = TermPtr->realBlink;

	BlockMoveData((Ptr)SessPtr->hostname, theScreen->machine, StrLength(SessPtr->hostname)+1);

	theScreen->lmode = 0; // RAB BetterTelnet 2.0b1 - fix for a really bizarre bug
	theScreen->lineAllow = SessPtr->linemode;
	if (SessPtr->linemode) //we allow linemode
		initLinemode(&screens[cur]);

	theScreen->launchurlesc = SessPtr->launchurlesc;
	theScreen->x11forward = SessPtr->x11forward;
	theScreen->autoreconnect = SessPtr->autoreconnect;

/* NONO */
	theScreen->authentication = SessPtr->authentication;
	theScreen->compression = SessPtr->compression;
	theScreen->verbose = SessPtr->verbose;
	theScreen->trace = SessPtr->trace;
	theScreen->debug = SessPtr->debug;
	theScreen->restricted = SessPtr->restricted;
	theScreen->ssh2method = SessPtr->ssh2method;
	theScreen->ssh2guests = SessPtr->ssh2guests;
	theScreen->localport = SessPtr->localport;
	memcpy(theScreen->remotehost, SessPtr->remotehost, SessPtr->remotehost[0] + 1);
	theScreen->remoteport = SessPtr->remoteport;

	// eXodus doesn't like loopback, but accepts local IP...
	//pstrcpy((unsigned char *)theScreen->display, "\p127.0.0.1:0.0");
	if ( !SessPtr->display[0] )
		pstrcpy((unsigned char *)SessPtr->display, "\p0.0");
	netgetip(localIP);
	sprintf((char *)theScreen->display + 1, "%d.%d.%d.%d:%#s", localIP[0], localIP[1], localIP[2], localIP[3], SessPtr->display);
	theScreen->display[0] = strlen((char *)theScreen->display + 1);

	theScreen->sshdata.thread = NULL;
	theScreen->sshdata.context = NULL;

	theScreen->vs = -1;
	theScreen->wind = NULL;

	if (SessPtr->protocol == 4) {
		memcpy(theScreen->sshdata.host, theScreen->machine, theScreen->machine[0] + 1);
		memcpy(theScreen->sshdata.login, theScreen->username, theScreen->username[0] + 1);
		memcpy(theScreen->sshdata.password, theScreen->password, theScreen->password[0] + 1);
		memcpy(theScreen->sshdata.command, theScreen->command, theScreen->command[0] + 1);

		setLastCursor(theCursors[normcurs]);

		if ( !theScreen->sshdata.login[0]
		  /*|| !theScreen->sshdata.password[0]*/ ) {
		 	if ( !SSH2LoginDialog(theScreen->sshdata.host, theScreen->sshdata.login, theScreen->sshdata.password) ) {
				goto failed;
			}
		}
	}
/* NONO */

	theScreen->active = CNXN_DNRWAIT;			// Signal we are waiting for DNR.

	// Get the IP for the host while we set up the connection
	if (DoTheDNR(hostname, cur) != noErr) {
		OutOfMemory(1010);
		goto failed;
	}

	GetFNum(TermPtr->DisplayFont, &fontnumber);
	GetFNum(TermPtr->BoldFont, &otherfnum);

	flags = 0;
	if (TermPtr->vtwrap)
		flags |= RSWwrapon;
	/*flags |= RSWshowit;*/
	flags |= RSWgoaway;
	if (SessPtr->forcesave)
		flags |= RSWforcesave;
	if (TermPtr->allowBold)
		flags |= RSWallowBold;
	if (TermPtr->colorBold)
		flags |= RSWcolorBold;
	if (SessPtr->ignoreBeeps)
		flags |= RSWignoreBeeps;
	if (TermPtr->realbold)
		flags |= RSWrealbold;
	if (TermPtr->oldScrollback)
		flags |= RSWoldscrollback;
	if (TermPtr->jumpScroll)
		flags |= RSWjumpscroll;
	if (TermPtr->realBlink)
		flags |= RSWrealBlink;
	if (TermPtr->vt7bits)
		flags |= RSWvt7bit;
	if (TermPtr->hideScrollBars)
		flags |= RSWHideScroll;
	if (SessPtr->keepselection)
		flags |= RSWKeepSelection;

	theScreen->vs = RSnewwindow(&((**Params).WindowLocation), TermPtr->numbkscroll, TermPtr->vtwidth,
									TermPtr->vtheight, (**Params).WindowName, fontnumber,
									TermPtr->fontsize, cur, otherfnum, TermPtr->boldFontSize,
									TermPtr->boldFontStyle, TermPtr->vtemulation, flags);

	if (theScreen->vs < 0 ) { 	/* we have a problem opening up the virtual screen */
		OutOfMemory(1011);
		goto failed;
	}

	theScreen->wind = RSgetwindow( theScreen->vs);
	((WindowPeek)theScreen->wind)->windowKind = WIN_CNXN;
	

	/*
	 * Attach our extra part to display encryption status
	 */
	PatchWindowWDEF(theScreen->wind, &screens[cur]);

	theScreen->arrowmap = TermPtr->emacsarrows;  		/* MAT -- save our arrow setting */
	theScreen->maxscroll= TermPtr->numbkscroll;
	theScreen->bsdel = SessPtr->bksp;
	theScreen->crmap = SessPtr->crmap;

	if (theScreen->portNum != getDefaultPort(theScreen->protocol)) // RAB BetterTelnet 1.0b1, 1.0fc4
		theScreen->crmap = SessPtr->alwaysBSD; // RAB BetterTelnet 1.0b1, 1.0fc4

	theScreen->tekclear = SessPtr->tekclear;
	theScreen->ESscroll= TermPtr->clearsave;
	theScreen->ANSIgraphics  = TermPtr->ANSIgraphics; //ANSI graphics, 2.7
	theScreen->tektype = SessPtr->tektype;
	theScreen->wrap = TermPtr->vtwrap;
	theScreen->pgupdwn = TermPtr->MATmappings;		/* JMB: map pgup/pgdwn/home/end? */
	theScreen->qprint = 0;
	theScreen->ignoreff = SessPtr->ignoreff;
	theScreen->TELstop = SessPtr->skey;
	theScreen->TELgo = SessPtr->qkey;
	theScreen->TELip = SessPtr->ckey;
	BlockMoveData(TermPtr->AnswerBackMessage, theScreen->answerback, 32);
	theScreen->termstate = VTEKTYPE;
	theScreen->naws = 0;								/* NCSA - set default NAWS to zero */
	theScreen->telstate=0;
	theScreen->timing=0;
	theScreen->curgraph=-1;				/* No graphics screen */
	theScreen->clientflags = 0;			/* BYU */
	theScreen->kblen = 0;				/* nothing in the buffer */
	theScreen->enabled = 1;			/* Gotta be enabled to start with */
	theScreen->Ittype = 0;
	theScreen->Isga = 0;				/* I suppress go ahead = no */
	theScreen->Usga = 0;				/* U suppress go ahead = no */
	theScreen->remote_flow = 0;		/* they handle toggling remote_flow */
	theScreen->allow_flow = 1;		/* initially, we allow flow control */
	theScreen->restart_any_flow = 0;	/* initially, only an XON turns flow control back on  */
	theScreen->termstate=VTEKTYPE;	/* BYU */
	theScreen->echo = 1;
	theScreen->halfdup = SessPtr->halfdup;	/* BYU */

	theScreen->innational = 0;			// Default to no translation.
	theScreen->outnational = 0;			// Default to no translation.
	theScreen->incharset = 0;
	theScreen->outcharset = 0;
	theScreen->toconverter = NULL;
	theScreen->fromconverter = NULL;
	// Now see if the desired translation is available, if not use default translation.
	for (scratchshort = 1; scratchshort <= nNational+1; scratchshort++) {
		GetMenuItemText(myMenus[National], scratchshort, scratchPstring);
		if (EqualString(SessPtr->TranslationTable, scratchPstring, TRUE, FALSE)) {
			theScreen->innational = theScreen->outnational = scratchshort - 1;
			break;
		}
	}
	inittranslation(theScreen);

	// Set up paste related variables
	theScreen->incount = 0;
	theScreen->outcount = 0;
	theScreen->outptr = NULL;
	theScreen->outhand = NULL;
	theScreen->outlen = 0;
	theScreen->pastemethod = SessPtr->pastemethod;
	theScreen->pastesize = SessPtr->pasteblocksize;
	
	scratchBoolean = RSsetcolors( theScreen->vs, 0, &TermPtr->nfcolor);
	scratchBoolean = RSsetcolors( theScreen->vs, 1, &TermPtr->nbcolor);
	scratchBoolean = RSsetcolors( theScreen->vs, 2, &TermPtr->bfcolor);
	scratchBoolean = RSsetcolors( theScreen->vs, 3, &TermPtr->bbcolor);

	addinmenu(cur, (**Params).WindowName, diamondMark);

	theScreen->myInitParams = (Handle)Params;
	HUnlock((Handle)(**Params).terminal);
	HUnlock((Handle)(**Params).session);
	// Params handle must stay locked because interrupt level DNR completion routine needs to deref it

	VSscrolcontrol( theScreen->vs, -1, theScreen->ESscroll);

	updateCursor(1);							/* Done stalling the user */
	return TRUE;

failed:

	theScreen->active = CNXN_NOTINUSE;
	disposetranslation(theScreen);
	disposemacros(&theScreen->sessmacros);
	DisposeHandle((Handle)(**Params).terminal);
	DisposeHandle((Handle)(**Params).session);
	DisposeHandle((Handle)Params);
	TelInfo->numwindows--;
	DoTheMenuChecks();
	updateCursor(1);

	return FALSE;
}

void	CompleteConnectionOpening(short dat, ip_addr the_IP, OSErr DNRerror, char *cname)
{
	ConnInitParams	**Params;
	short socks4a, len, pos;
	WindRec *tw;
	
	tw = &screens[dat];

	if (tw->active != CNXN_DNRWAIT) return;			// Something is wrong.
	
	Params = (ConnInitParams **)tw->myInitParams;
	
	if (DNRerror == noErr) {
		if (tw->sockslookup) { // RAB BetterTelnet 2.0fc1
			tw->socksIP = the_IP;
			socks4a = 0;
			if (tw->socks4a) {
				len = tw->machine[0];
				pos = 1;
				while (len) { // we still "look up" dotted quad numbers since
							  // that doesn't involve DNS
					if (((tw->machine[pos] < '0') ||
						 (tw->machine[pos] > '9')) &&
						(tw->machine[pos] != '.'))
						socks4a = 1; // not a number or period, thus it's a DNS name
					len--;
					pos++;
				}
			}
			tw->socks4a = socks4a;
			if (!socks4a) { // we need to do another lookup
				tw->sockslookup = 0;
				if (DoTheDNR(tw->machine, dat) != noErr) {
					OutOfMemory(1010);
					DisposeHandle((Handle)(**Params).terminal);
					DisposeHandle((Handle)(**Params).session);
					DisposeHandle((Handle)Params);
					TelInfo->numwindows--;
					updateCursor(1);
					return;
				}
				return; // wait for it to finish
			}
		} else if (tw->usesocks) { // ok, it finished
			tw->actualIP = the_IP;
			the_IP = tw->socksIP; // we connect to the socks server (first lookup)
										   // not the remote host (second lookup)
		}

		if (tw->usesocks) // fix the port
			(**(**Params).session).port = tw->socksport;
			// we set (**(**Params).session).port but not tw->portNum because
			// the session is disposed after the connection is open while
			// the screen remains and could be used to save a set

		HLockHi((Handle)(**Params).session);

		if ((**(**Params).session).NetBlockSize < 512)	
			(**(**Params).session).NetBlockSize = 512; //less than this can really get messy

		if (setReadBlockSize((**(**Params).session).NetBlockSize,dat) != 0) //couldnt get read buffer
			return;
		if ((tw->protocol == 1) || (tw->protocol == 2)) netfromport(768);

/* NONO */
#if 1
		if ( tw->protocol == 4 ) {
			// dummy makestream for ssh2...
			tw->port = makestream();
			tw->sshdata.ip = the_IP;
			// fake open indication for ssh2...
			netputevent(CONCLASS, CONOPEN, tw->port,0);

		} else {
			tw->port  = netxopen(the_IP,(**(**Params).session).port,/* BYU 2.4.15 - open to host name */
						gApplicationPrefs->OpenTimeout);/* CCP 2.7 allow user set-able timeouts on open */
		}

#else
		tw->port  = netxopen(the_IP,(**(**Params).session).port,/* BYU 2.4.15 - open to host name */
					gApplicationPrefs->OpenTimeout);/* CCP 2.7 allow user set-able timeouts on open */
#endif
/* NONO */

		// We need the cannonical hostname for Kerberos. Make best guess if
		// DNR did not return a cname.
		if (cname)
			strncpy(tw->cannon, cname, sizeof(tw->cannon));
		else
			strncpy(tw->cannon, (char *)(**(**Params).session).hostname, sizeof(tw->cannon));
		tw->cannon[sizeof(tw->cannon)-1] = '\0';

		DisposeHandle((Handle)(**Params).session);
		DisposeHandle((Handle)(**Params).terminal);
		DisposeHandle((Handle)Params);

		if (tw->port <0) {					/* Handle netxopen fail */
			destroyport(dat);
			}
		tw->active = CNXN_OPENING;
		SetMenuMarkToOpeningForAGivenScreen(dat);	/* Change status mark */
		}
	else
		{	// We should report the real DNR error here!
		Str255		errorString, numberString, numberString2, scratchPstring;
		DialogPtr	theDialog;
		short		message, ditem = 3;

		HLockHi((Handle)(**Params).session);
		BlockMoveData((**(**Params).session).hostname, scratchPstring, StrLength((**(**Params).session).hostname)+1);

		if (DNRerror >= -23048 && DNRerror <= -23041) message = DNRerror + 23050;
		else message = 1;

		GetIndString(errorString,DNR_MESSAGES_ID, message);
		NumToString((long)0, numberString);
		NumToString((long)DNRerror, numberString2);
		ParamText(scratchPstring, errorString, numberString, numberString2);

		theDialog = GetNewMyDialog(DNRErrorDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
		ShowWindow(theDialog);

		while (ditem > 1)	ModalDialog(DLOGwOKUPP, &ditem);
		DisposeDialog(theDialog);

		// RAB BetterTelnet 2.0b2 - we need to report the DNS error to AppleScript
		if (tw->cxWeHaveAppleEvent) {
			AEResumeTheCurrentEvent(&tw->cxAppleEvent, &tw->cxAEReply,
				MyHandleConnectUPP, 2);
			tw->cxWeHaveAppleEvent = 0;
		}

		DisposeHandle((Handle)(**Params).session);
		DisposeHandle((Handle)(**Params).terminal);
		DisposeHandle((Handle)Params);
		destroyport(dat);
		}
}

void	ConnectionOpenEvent(short port)
{
	short i, pos, pos2;
	static char buf[512];
	
	i=WindByPort(port);
	if (i<0) { 
		return;
		}

	if (screens[i].cxWeHaveAppleEvent) {
		AEResumeTheCurrentEvent(&screens[i].cxAppleEvent, &screens[i].cxAEReply,
			MyHandleConnectUPP, 1);
		screens[i].cxWeHaveAppleEvent = 0;
	}

	screens[ i].active= CNXN_ACTIVE;
	RSshow( screens[i].vs);			/* BYU */
	SelectWindow(screens[i].wind);	/* BYU */

	if (screens[i].socksinprogress) { // RAB BetterTelnet 2.0fc1
		buf[0] = 4;
		buf[1] = 1;
		buf[2] = (screens[i].portNum >> 8) & 0xFF;
		buf[3] = screens[i].portNum & 0xFF;
		if (screens[i].socks4a)
			memcpy(&buf[4], "\000\000\000\001", 4);
		else memcpy(&buf[4], &screens[i].actualIP, 4);
		pos = screens[i].socksusername[0];
		memcpy(&buf[8], &screens[i].socksusername[1], pos);
		pos += 8;
		buf[pos] = 0;
		pos++;
		if (screens[i].socks4a) {
			pos2 = screens[i].machine[0];
			memcpy(&buf[pos], &screens[i].machine[1], pos2);
			pos += pos2;
			buf[pos] = 0;
			pos++;
		}

		Rnetwrite(screens[i].port, buf, pos);
	}
	else
		telnet_send_initial_options(&screens[i]);

	changeport(scrn,i);		/* BYU */
	SetMenuMarkToLiveForAGivenScreen(scrn);			/* BYU */
	DoTheMenuChecks();		/* BYU */
}

void	ConnectionDataEvent(short port)
{
	short	i, cnt, urgent, res;
	char p[255];
	unsigned char *st;
	WindRec *tw;
	
	i=WindByPort(port);									/* BYU */
	if (i<0) {return; }					/* BYU */

	if (TelInfo->ScrlLock || (!screens[i].enabled && !screens[i].waWeHaveAppleEvent))	/* BYU LSC */
		netputuev( CONCLASS, CONDATA, port,0);
	else {
		cnt = netread(port,gReadspace,gBlocksize);	/* BYU LSC */
// urgent data isn't working right now, so this is turned off
/*		if ((screens[i].protocol >= 1) && (screens[i].protocol <= 3)) {
			urgent = getUrgentFlag(port);
			if (urgent) {
				rlogin_parse( &screens[i], gReadspace, cnt);
				screens[i].incount += cnt;
				return;
			}
		} */

// RAB BetterTelnet 2.0fc1 - stuff for processing SOCKS 4 replies

		st = gReadspace;
		tw = &screens[i];

		while (tw->socksinprogress && cnt) {
			if (tw->sockspos == 1) {
				res = *st;
				sprintf(p, "socks4: result code %d", res);
				putln(p);
			}
			if (tw->sockspos == 7) {
				tw->socksinprogress = 0;
				telnet_send_initial_options(tw);
			}
			tw->sockspos++;
			cnt--;
			st++;
		}
		if (/*screens[i].protocol != 4*/ true ) // ssh handles differently
			parse( &screens[i], st, cnt);	/* BYU LSC */
		else {
			screens[i].jsNoFlush = 1; // RAB BetterTelnet 2.0b4
									  // improve relations between jump scroller and ssh
			ssh_glue_read(&screens[i], st, cnt);
			screens[i].jsNoFlush = 0;
			VSflushwrite(screens[i].vs);
		}
		screens[i].incount += cnt;				/* BYU LSC */
		}
}

void	ConnectionFailedEvent(short port)
{
	short	i, err;
	Str255	scratchPstring;

	if (checkPortRotation(port)) return;

	netclose( port);
	i= WindByPort(port);
	if (i<0) { return; }

	if (screens[i].cxWeHaveAppleEvent) {
		AEResumeTheCurrentEvent(&screens[i].cxAppleEvent, &screens[i].cxAEReply,
			MyHandleConnectUPP, 2);
		screens[i].cxWeHaveAppleEvent = 0;
	}

	BlockMoveData((Ptr)screens[i].machine, (Ptr)scratchPstring, StrLength(screens[i].machine)+1);
	PtoCstr(scratchPstring);
	DoError(807 | NET_ERRORCLASS, LEVEL2, (char *)scratchPstring);
	
	if (screens[i].active != CNXN_ACTIVE) destroyport(i);	// JMB - 2.6
	else removeport(&screens[i]);		// JMB - 2.6
}

void	ConnectionClosedEvent(short port)
{
	short i;

	i= WindByPort(port);
	if (i<0) { 
		netclose( port);			/* We close again.... */
		return;
		}

	if (screens[i].cxWeHaveAppleEvent) {
		AEResumeTheCurrentEvent(&screens[i].cxAppleEvent, &screens[i].cxAEReply,
			MyHandleConnectUPP, 2);
		screens[i].cxWeHaveAppleEvent = 0;
	}

	FlushNetwork(i);				/* BYU */
	netclose( screens[i].port);		/* BYU */
	removeport(&screens[i]);					/* BYU */
}

short	WindByPort(short port)
{
	short i=0;

	while (i<TelInfo->numwindows &&
			(screens[i].port != port || 
				((screens[i].active != CNXN_ACTIVE) && (screens[i].active != CNXN_OPENING)))
			) i++;

	if (i>=TelInfo->numwindows) {					/* BYU */
		putln("Can't find a window for the port # in WindByPort");	/* BYU */
		if (i==0) i=999;		/* BYU */
		return(-i);				/* BYU */
		}						/* BYU */

	return(i);
}

WindRec *FindWindByPort(short port)
{
	short i=0;

	while (i<TelInfo->numwindows && screens[i].port != port) i++;
	if (i>=TelInfo->numwindows) {
		putln("Can't find a window for the port # in WindByPort");
		return NULL;
	}
	return &screens[i];
}

void destroyport(short wind)
{
	Handle	h;
	short	i,
			callNoWindow=0;
	Size 	junk;
	WindRecPtr	tw;
	
	tw = &screens[wind];

	setLastCursor(theCursors[watchcurs]);		/* We may be here a while */

	if (tw->active == CNXN_ISCORPSE) {
		if ( tw->curgraph >= 0 )
			detachGraphics( tw->curgraph);	/* Detach the Tek screen */
		if (tw->outlen>0) {
			tw->outlen=0;						/* Kill the remaining send*/
			HUnlock( tw->outhand);			/*  buffer */
			HPurge ( tw->outhand);
		}
	}

	if ( FrontWindow() == tw->wind )
		callNoWindow = 1;

	if ( tw->aedata != NULL ) {
		auth_encrypt_end((tnParams **)&tw->aedata);
 		DisposePtr((Ptr)tw->aedata);
 		tw->aedata = NULL;
	}

	/*
	 * Get handle to the WDEF patch block, kill the window, and then
	 * release the handle.
	 */
	h = ( tw->wind ) ? GetPatchStuffHandle(tw->wind, tw) : NULL;
	RSkillwindow( tw->vs);
	SetDefaultKCHR();
	if ( h )
		DisposeHandle(h);
	tw->active = CNXN_NOTINUSE;

	for (i=wind; i<TelInfo->numwindows-1; i++) {
		screens[i]=screens[i+1];		/* Bump all of the pointers */
		if (screens[i].wind)
			RePatchWindowWDEF(screens[i].wind, &screens[i]);	/* hack hack hack */
	}

	if (scrn>wind) scrn--;				/* Adjust for deleting a lower #ered screen */

	TelInfo->numwindows--;						/* There are now fewer windows */
	extractmenu( wind);					/* remove from the menu bar */

	DoTheMenuChecks();
	MaxMem(&junk);
/* BYU 2.4.11 - the call to "NoWindow()" changes "myfrontwindow", 
                which is used by "updateCursor()", so we reversed 
                the order of the following two lines. */
	if (callNoWindow) NoWindow();		/* BYU 2.4.11 - Update cursor stuff if front window */
	updateCursor(1);					/* BYU 2.4.11 - Done stalling the user */

	
} /* destroyport */

void removeport(WindRecPtr tw)
{
	Str255		scratchPstring;
	short		active;
	
	setLastCursor(theCursors[watchcurs]);				/* We may be here a while */

	disposemacros(&tw->sessmacros);

	if (tw->curgraph >= 0)
		detachGraphics( tw->curgraph);		/* Detach the Tek screen */

	if (tw->protocol == 4) {
		ssh_glue_close(tw);
	}

	disposetranslation(tw);

	if (tw->outlen>0) {
		tw->outlen=0;				/* Kill the remaining send*/
		HUnlock( tw->outhand);		/*  buffer */
		HPurge ( tw->outhand);
	}

	if (VSiscapturing(tw->vs))				/* NCSA: close up the capture */
		CloseCaptureFile(tw->vs);			/* NCSA */

	if (VSisprinting(tw->vs))
		ClosePrintingFile(tw->vs);

	active = tw->active;
	tw->active = CNXN_ISCORPSE;
	if ( tw->authenticate
	  && gApplicationPrefs->destroyKTickets
	  && numberKerberosConnections() == 0 ) {
		// if this is last window
		DestroyTickets();
	}
	tw->active = active;


	if (!gApplicationPrefs->WindowsDontGoAway) {
		if ( tw->vs >= 0 ) {
			short sn = findbyVS(tw->vs);
			if ( sn >= 0 ) {
				destroyport(sn);
			}
		}
	} else {
		Str255	temp;
		
		GetWTitle(tw->wind, scratchPstring);
		sprintf((char *)temp, "(%#s)", scratchPstring);
		CtoPstr((char *)temp);
		SetWTitle(tw->wind, temp);

		tw->port = 32700;
		tw->active = CNXN_ISCORPSE;
		}
	updateCursor(1);							/* Done stalling the user */
} /* removeport */


//	We recognize the following input string: "xxxx yyyy"
//	If "xxxx" matches a session name, that session record is used.  Otherwise, the default
//	session record is used with "xxxx" as the DNS hostname.   "yyyy", if extant, is
//	converted to a number.  If it is a valid number, it is used as the port to connect to.
//	WARNING: Do not pass this routing a blank string.  (We check this in PresentOpenConnectionDialog.)
//
//	CCP 2.7:  If second argument is true, get terminal out of session pref; otherwise set it to NULL
ConnInitParams	**NameToConnInitParams(StringPtr InputString, Boolean useDefaultTerminal, StringPtr otherPstring, Boolean *wasAlias)
{
	ConnInitParams	**theHdl;
	SessionPrefs	**sessHdl;
	TerminalPrefs	**termHdl;
	Handle			sessmacros;
	short			portRequested, portHack, portNegative;
	Boolean			foundPort;
	Boolean			alias = FALSE;
	long 			junk;

	*wasAlias = false;

	theHdl = (ConnInitParams **)myNewHandleCritical(sizeof(ConnInitParams));
	if (theHdl == NULL)
		return NULL;

	if (useDefaultTerminal) {
		foundPort = 0;
		portNegative = 0;
	} else
		foundPort = ProcessHostnameString(InputString, &portRequested, &portNegative);

	UseResFile(TelInfo->SettingsFile);

	if (useDefaultTerminal) {
		sessHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, InputString, sizeof(SessionPrefs));
		if (sessHdl == NULL)
		{				// Connect to host xxxx w/default session.
			portHack = 1;
			sessHdl = GetDefaultSession();
			if ( sessHdl == NULL ) {
				/* not found */
				DisposeHandle((Handle)theHdl);
				return NULL;
			}
			DetachResource((Handle) sessHdl);
			HLock((Handle)sessHdl);
			BlockMoveData(InputString, (**sessHdl).hostname, 64);
			sessmacros = Get1NamedResource('uMac', "\p<Default>");
		}
		else 
		{	
			portHack = 0;
			DetachResource((Handle) sessHdl);
			HLock((Handle)sessHdl);
			alias = TRUE;
			sessmacros = Get1NamedResource('uMac', InputString);
		}
	} else {
		sessHdl = NULL;
		if (gApplicationPrefs->parseAliases) {
			sessHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, InputString, sizeof(SessionPrefs));
			if (sessHdl != NULL) {
				*wasAlias = true;
			}
		}
		if (sessHdl == NULL) {
			portHack = 1;
			sessHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, otherPstring, sizeof(SessionPrefs));
			if ( sessHdl == NULL ) {
				/* not found */
				DisposeHandle((Handle)theHdl);
				return NULL;
			}
			DetachResource((Handle) sessHdl);
			HLock((Handle)sessHdl);
			BlockMoveData(InputString, (**sessHdl).hostname, 64);
			sessmacros = Get1NamedResource('uMac', otherPstring);
		} else {
			portHack = 0;
			DetachResource((Handle) sessHdl);
			HLock((Handle)sessHdl);
			sessmacros = Get1NamedResource('uMac', InputString);
		}
	}

	HLock((Handle)theHdl);
	(**theHdl).session = sessHdl;
	
	UseResFile(TelInfo->SettingsFile);

//	if ((useDefaultTerminal)||(alias))
//	if (1)
//	{	

	termHdl = (TerminalPrefs **)Get1NamedSizedResource
			(TERMINALPREFS_RESTYPE, (**sessHdl).TerminalEmulation, sizeof(TerminalPrefs));
	if (termHdl == NULL) {
		termHdl = GetDefaultTerminal();
		if ( termHdl == NULL ) {
			DisposeHandle((Handle)sessHdl);
			DisposeHandle((Handle)theHdl);
			return NULL;
		}
	}
	DetachResource((Handle) termHdl);
	(**theHdl).terminal = termHdl;

//	}
//	else
//		(**theHdl).terminal = NULL;

	UnloadSeg(&PREFSUnload);
	MaxMem(&junk);	  //swap out space so we can make the new window CCP
	
	((**theHdl).WindowName)[0] = 0;
	(**sessHdl).ip_address = 0;

	(**theHdl).sessmacros.handle = 0;

	if (sessmacros) {
		DetachResource(sessmacros);
		HLock(sessmacros);
		ParseMacrosFromHandle(&(**theHdl).sessmacros, sessmacros);
	}

	if (foundPort) { (**sessHdl).port = portRequested; (**sessHdl).portNegative = portNegative; }
	else if (portHack) (**sessHdl).port = getDefaultPort((**sessHdl).protocol);
	
	HUnlock((Handle)sessHdl);

	return(theHdl);
}

ConnInitParams	**ReturnDefaultConnInitParams(void)
{
	ConnInitParams	**theHdl;
	Handle sessmacros;

	theHdl = (ConnInitParams **)myNewHandle(sizeof(ConnInitParams));
	if ( theHdl ) {
		HLock((Handle)theHdl);
		(**theHdl).session = GetDefaultSession();
		if ( (**theHdl).session ) {
			(**(**theHdl).session).ip_address = 0;
			(**theHdl).terminal = GetDefaultTerminal();
			if ( (**theHdl).terminal ) {
				UseResFile(TelInfo->SettingsFile);
				sessmacros = Get1NamedResource('uMac', "\p<Default>");
				if (sessmacros) {
					(**theHdl).sessmacros.handle = 0;
					DetachResource(sessmacros);
					HLock(sessmacros);
					ParseMacrosFromHandle(&(**theHdl).sessmacros, sessmacros);
				}
				HUnlock((Handle)theHdl);
			} else {
				DisposeHandle((Handle)(**theHdl).session);
				DisposeHandle((Handle)theHdl);
				theHdl = NULL;
			}
		} else {
			DisposeHandle((Handle)theHdl);
			theHdl = NULL;
		}
	}
	return theHdl;
}

short numberLiveConnections(void)
{
	short i;
	short liveConnections = 0;
	for(i = 0; i < MaxSess; i++)
		if ((screens[i].active == CNXN_ACTIVE)||(screens[i].active == CNXN_OPENING))
			liveConnections++;
	return liveConnections;
}


static short numberKerberosConnections(void)
{
	short i;
	short liveConnections = 0;
	for(i = 0; i < MaxSess; i++)
		if ( screens[i].authenticate
		 && (screens[i].active == CNXN_ACTIVE || screens[i].active == CNXN_OPENING) )
			liveConnections++;
	return liveConnections;
}


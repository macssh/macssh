/*
 * PasswordDialog.c
 * (c) 2000 Jean-Pierre Stierlin.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <Types.h>
#include <Memory.h>
#include <Quickdraw.h>
#include <Menus.h>
#include <Notification.h>
#include <Dialogs.h>
#include <TextEdit.h>

#include "PasswordDialog.h"
#include "movableModal.h"
#include "event.proto.h"
#include "netevent.proto.h"
#include "DlogUtils.proto.h"

extern void ssh2_sched();

extern TelInfoRec	*TelInfo;
extern Boolean		gAEavail;

static pascal Boolean RandomizeFilter(DialogPtr dlog,EventRecord *event,short *itemHit);
OSErr InteractWithUser( Boolean isUrgent, short sicnResID, short strResID );
static pascal Boolean InternalBufferFilter(DialogPtr dlog,EventRecord *event,short *itemHit);
static void DeleteRange(char *buffer,short start,short end);
static Boolean InsertChar(char *buffer,short pos,char c,short max);
static OSErr GetScrapHandle( FlavorType dataFlavor, Handle *dataHandle );

#define kMaxPasswordLen 128

Boolean YesNoDialog(StringPtr prompt)
{
	Boolean			result = false;
	DialogPtr		dlog;
	short			item;
	/*ModalFilterUPP  yesNoFilterUPP;*/

	/*yesNoFilterUPP = NewModalFilterProc(YesNoFilter);*/
	dlog = GetNewDialog(rYesNoDialog, 0L, (WindowPtr)-1L);
	if ( dlog ) {
		ParamText(prompt, "\p", "\p", "\p");
		SetDialogDefaultItem(dlog, 1);
		//SetDialogCancelItem(dlog, 2);
		SetDialogTracksCursor(dlog, 1);
		ShowWindow(dlog);
		do {
			ModalDialog(NULL, &item);
		} while (item != 1 && item != 2);				// Until the OK button is hit
		result = (item == 2);	// NO is item 1...
		DisposeDialog(dlog);
	}
	/*DisposeRoutineDescriptor(yesNoFilterUPP);*/
	return result;
}

Boolean SSH2RandomizeDialog( long *type, long *level, long *encrypt, Str255 comment )
{
	DialogPtr		dlog;
	Boolean			result = 0;
	ModalFilterUPP  dlogFilterUPP;
	Str255 temp;

	SetUpMovableModalMenus();
	dlogFilterUPP = NewModalFilterProc(RandomizeFilter);
	//dlog = GetNewMyDialog(rSSH2RandomizeDialog, 0L, (WindowPtr)-1L, NULL);
	dlog = GetNewMyDialog(rSSH2RandomizeDialog, 0L, (WindowPtr)-1L, (void *)ThirdCenterDialog);
	if ( dlog ) {
		short item;
		SInt16 itemType;
		Handle itemHandle;
		Rect itemRect;
		SetDialogDefaultItem(dlog, 1);
		SetDialogCancelItem(dlog, 2);
		SetDialogTracksCursor(dlog, 1);

		GetDialogItem(dlog, 6, &itemType, &itemHandle, &itemRect);	// DSA
		SetControlValue((ControlHandle)itemHandle, 1);
		GetDialogItem(dlog, 7, &itemType, &itemHandle, &itemRect);	// RSA
		SetControlValue((ControlHandle)itemHandle, 0);
		HideDialogItem(dlog, 4);
		HideDialogItem(dlog, 9);
		HideDialogItem(dlog, 10);
		GetDialogItem(dlog, 3, &itemType, &itemHandle, &itemRect);	// NIST Level
		SetControlValue((ControlHandle)itemHandle, 9);
		//GetDialogItem(dlog, 9, &itemType, &itemHandle, &itemRect);	// Bits
		//SetDialogItemText(itemHandle, "\p1024");
		GetDialogItem(dlog, 11, &itemType, &itemHandle, &itemRect);	// Encrypt
		SetControlValue((ControlHandle)itemHandle, 1);
		if (comment) {
			GetDialogItem(dlog, 12, &itemType, &itemHandle, &itemRect);
			SetDialogItemText(itemHandle, comment);
		}
		ShowWindow(dlog);
		do {
			movableModalDialog(dlogFilterUPP, &item);
			if ( item == 6 ) {
				GetDialogItem(dlog, 6, &itemType, &itemHandle, &itemRect);
				SetControlValue((ControlHandle)itemHandle, 1);
				GetDialogItem(dlog, 7, &itemType, &itemHandle, &itemRect);
				SetControlValue((ControlHandle)itemHandle, 0);
				HideDialogItem(dlog, 4);
				HideDialogItem(dlog, 9);
				HideDialogItem(dlog, 10);
				ShowDialogItem(dlog, 3);
				/* arrow not refreshed ? */
				GetDialogItem(dlog, 3, &itemType, &itemHandle, &itemRect);
				itemRect.right += 40;
				InvalRect(&itemRect);
			} else if ( item == 7 ) {
				GetDialogItem(dlog, 7, &itemType, &itemHandle, &itemRect);
				SetControlValue((ControlHandle)itemHandle, 1);
				GetDialogItem(dlog, 6, &itemType, &itemHandle, &itemRect);
				SetControlValue((ControlHandle)itemHandle, 0);
				ShowDialogItem(dlog, 4);
				ShowDialogItem(dlog, 9);
				SelectDialogItemText(dlog, 9, 0, 255);
				ShowDialogItem(dlog, 10);
				HideDialogItem(dlog, 3);
			} else if ( item == 11 ) {
				GetDialogItem(dlog, 11, &itemType, &itemHandle, &itemRect);
				SetControlValue((ControlHandle)itemHandle, !GetControlValue((ControlHandle)itemHandle));
			}
		} while (item != 1 && item != 2);
		if ( item == 1 ) {
			GetDialogItem(dlog, 7, &itemType, &itemHandle, &itemRect);
			if (type) {
				*type = GetControlValue((ControlHandle)itemHandle);
			}
			if (level) {
				if (GetControlValue((ControlHandle)itemHandle)) {
					/* RSA */
					GetDialogItem(dlog, 9, &itemType, &itemHandle, &itemRect);
					GetDialogItemText(itemHandle, temp);
					StringToNum(temp, level);
				} else {
					GetDialogItem(dlog, 3, &itemType, &itemHandle, &itemRect);
					*level = GetControlValue((ControlHandle)itemHandle) - 1;
				}
			}
			if (encrypt) {
				GetDialogItem(dlog, 11, &itemType, &itemHandle, &itemRect);
				*encrypt = GetControlValue((ControlHandle)itemHandle);
			}
			if (comment) {
				GetDialogItem(dlog, 12, &itemType, &itemHandle, &itemRect);
				GetDialogItemText(itemHandle, comment);
			}
			result = 1;
		}
		DisposeDialog(dlog);
	}
	DisposeRoutineDescriptor(dlogFilterUPP);
	ResetMenus();
	return result;
}


static pascal Boolean
RandomizeFilter(DialogPtr dlog,EventRecord *event,short *itemHit)
{
	char	key;
	short	start,end;
	Handle	hText;
	char	buffer[256];
	long	val;
	
	if (event->what != keyDown && event->what != autoKey)
		return CallStdFilterProc(dlog, event, itemHit);

	key = event->message & charCodeMask;
/*
	switch (key) {
		case '\r':			// Return
		case '\003':		// Enter
			*itemHit = 1;		// OK Button
			return true;		// We handled the event
		case '.':			// Cmd-.
			if (!(event->modifiers & cmdKey))
				break;
		case '\033':			// Esc
			*itemHit = 2;		// Cancel Button
			return true;		// We handled the event
		case '\t':			// Tab
		case '\034':		// Left arrow
		case '\035':		// Right arrow
		case '\036':		// Up arrow
		case '\037':		// Down arrow
			return false;		// Let ModalDialog handle them
		default:			// Everything else falls through to be dealt with
			break;			//	below
	}
*/
	switch (key) {
		case '.':			// Cmd-.
			if (!(event->modifiers & cmdKey))
				break;
		case '\r':			// Return
		case '\003':		// Enter
		case '\033':		// Esc
		case '\t':			// Tab
		case '\034':		// Left arrow
		case '\035':		// Right arrow
		case '\036':		// Up arrow
		case '\037':		// Down arrow
			return CallStdFilterProc(dlog, event, itemHit);
		default:			// Everything else falls through to be dealt with
			break;			//	below
	}

	if (((DialogPeek)dlog)->editField == 8 ) {	// item-1 !!!
		start = (**((DialogPeek)dlog)->textH).selStart;	// Get the current selection
		end = (**((DialogPeek)dlog)->textH).selEnd;
		
		hText = TEGetText(((DialogPeek)dlog)->textH);
		buffer[0] = GetHandleSize(hText);
		BlockMoveData(*hText, buffer + 1, (unsigned char)buffer[0]);

		if (start != end) {					// If there's a selection, delete it
			DeleteRange(buffer,start,end);
		}
		if (key == '\010') {	// Backspace
			if (start != 0) {
				DeleteRange(buffer,start-1,start);	// Delete the character to the left
			}
		} else {
			if ((event->modifiers & cmdKey)) {
				if (key == 'v' || key == 'V') {
					// get scrap
					OSErr err;
					if ((err = GetScrapHandle('TEXT', &hText)) == noErr) {
						int len = GetHandleSize(hText);
						if (len + *buffer <= 255) {
							int i;
							for (i = 0; i < len ; i++) {
								key = (*hText)[i];
								if ( InsertChar(buffer,start++,key,63) ) {
									StringToNum((unsigned char *)buffer, &val);
									if ( key < '0' || key > '9' || val > 4096 ) {
										SysBeep(5);
										break;
									}
									// Insert the real key into the buffer
									TEKey((*hText)[i], ((DialogPeek)dlog)->textH);
								} else {
									break;
								}
							}
						}
						DisposeHandle(hText);
						event->what = nullEvent;
					}
				} else if ( key == 'x' || key == 'X'
						 || key == 'c' || key == 'C' ) {
					SysBeep(5);
					event->what = nullEvent;
				}
			} else {
				// Insert the real key into the buffer
				if ( InsertChar(buffer,start,key,63)) {
					StringToNum((unsigned char *)buffer, &val);
					if ( key < '0' || key > '9' || val > 4096 ) {
						SysBeep(5);
						event->what = nullEvent;
					}
				} else {
					event->what = nullEvent;
				}
			}
		}
	}
	return false; 
}



void SSH2ErrorDialog(char *mess1)
{
	DialogPtr		dlog;
	short item;
	char buf[256];

	InteractWithUser( true, 128, 128 );

	dlog = GetNewMyDialog(rSSH2ErrorDialog, 0L, (WindowPtr)-1L, (void *)ThirdCenterDialog);
	if ( dlog ) {
		buf[0]=0;		/* dont print extraneous garbage to the screen... */
		strncpy(buf, mess1, 255);
		c2pstr(buf);
		ParamText((StringPtr)buf,"\p","\p","\p");
		SetDialogDefaultItem(dlog, 1);
		//SetDialogCancelItem(dlog, 2);
		SetDialogTracksCursor(dlog, 1);
		ShowWindow(dlog);
		do {
			ModalDialog(NULL, &item);
		} while (item != 1);

		DisposeDialog(dlog);
	}
}



Boolean SSH2LoginDialog(StringPtr host, StringPtr login, StringPtr password)
{
	DialogPtr		dlog;
	short			item = 0;
	ModalFilterUPP  internalBufferFilterUPP;
	Str255			temp;

	InteractWithUser( true, 128, 128 );

	SetUpMovableModalMenus();
	internalBufferFilterUPP = NewModalFilterProc(InternalBufferFilter);
	//dlog = GetNewMyDialog(rSSH2LoginDialog, 0L, (WindowPtr)-1L, (void *)ThirdCenterDialog);
	dlog = GetNewMyDialog(rSSH2LoginDialog, 0L, (WindowPtr)-1L, NULL);
	if ( dlog ) {
		SInt16 itemType;
		Handle itemHandle;
		Rect itemRect;
		// set prompt
		GetDialogItem(dlog, 5, &itemType, &itemHandle, &itemRect);
		SetDialogItemText(itemHandle, host);
		// set login
		GetDialogItem(dlog, 4, &itemType, &itemHandle, &itemRect);
		SetDialogItemText(itemHandle, login);
		// set password
		GetDialogItem(dlog, 3, &itemType, &itemHandle, &itemRect);
		memset(temp + 1, '¥', password[0]);
		temp[0] = password[0];
		SetDialogItemText(itemHandle, temp);
		if (!login[0]) {
			SelectDialogItemText(dlog, 4, 0, 255);
		} else {
			SelectDialogItemText(dlog, 3, 0, 255);
		}
		SetDialogDefaultItem(dlog, 1);
		SetDialogCancelItem(dlog, 2);
		SetDialogTracksCursor(dlog, 1);
		ShowWindow(dlog);
		SetWRefCon(dlog, (long)password);	// Stash the buffer's address
		do {
			movableModalDialog(internalBufferFilterUPP, &item);
		} while (item != 1 && item != 2);	// Until the OK button is hit
		GetDialogItem(dlog, 4, &itemType, &itemHandle, &itemRect);
		GetDialogItemText(itemHandle, login);
		DisposeDialog(dlog);
	}
	DisposeRoutineDescriptor(internalBufferFilterUPP);
	ResetMenus();
	return item == 1;
}

Boolean SSH2PasswordDialog(StringPtr prompt, StringPtr password)
{
	DialogPtr		dlog;
	short			item = 0;
	ModalFilterUPP  internalBufferFilterUPP;

	InteractWithUser( true, 128, 128 );

	SetUpMovableModalMenus();
	internalBufferFilterUPP = NewModalFilterProc(InternalBufferFilter);
	*password = '\0';
	dlog = GetNewMyDialog(rSSH2PasswordDialog, 0L, (WindowPtr)-1L, NULL);
	if ( dlog ) {
		SInt16 itemType;
		Handle itemHandle;
		Rect itemRect;
		// set prompt
		GetDialogItem(dlog, 5, &itemType, &itemHandle, &itemRect);
		SetDialogItemText(itemHandle, prompt);
		SetDialogDefaultItem(dlog, 1);
		SetDialogCancelItem(dlog, 2);
		SetDialogTracksCursor(dlog, 1);
		ShowWindow(dlog);
		SetWRefCon(dlog, (long)password);	// Stash the buffer's address
		do {
			movableModalDialog(internalBufferFilterUPP, &item);
		} while (item != 1 && item != 2);	// Until the OK button is hit
		DisposeDialog(dlog);
	}
	DisposeRoutineDescriptor(internalBufferFilterUPP);
	ResetMenus();
	return item == 1;
}


static short SSH2SOCDialog(char *fingerprint, int id)
{
	DialogPtr		dlog;
	short			item = 3;	/* cancel */

	InteractWithUser( true, 128, 128 );

	SetUpMovableModalMenus();
	dlog = GetNewMyDialog(id, 0L, (WindowPtr)-1L, NULL);
	if ( dlog ) {
		SInt16 itemType;
		Handle itemHandle;
		Rect itemRect;
		Str255 pString;
		// set fingerprint
		GetDialogItem(dlog, 6, &itemType, &itemHandle, &itemRect);
		pString[0] = strlen(fingerprint);
		BlockMoveData(fingerprint, pString+1, pString[0]);
		SetDialogItemText(itemHandle, pString);
		if ( id == rSSH2SOC1Dialog ) {
			SetDialogDefaultItem(dlog, 1);	/* accept & save */
			//SetDialogCancelItem(dlog, 2);	/* accept once */
			SetDialogCancelItem(dlog, 3);	/* cancel */
		} else {
			/* host key changed: cancel is default */
			SetDialogDefaultItem(dlog, 3);	/* cancel */
			SetDialogCancelItem(dlog, 3);	/* cancel */
		}
		SetDialogTracksCursor(dlog, 1);
		ShowWindow(dlog);
		do {
			movableModalDialog(NULL, &item);
		} while (item != 1 && item != 2 && item != 3);	// Until button is hit
		DisposeDialog(dlog);
	}
	ResetMenus();
	return item;
}

short SSH2SOC1Dialog(char *fingerprint)
{
	return SSH2SOCDialog(fingerprint, rSSH2SOC1Dialog);
}

short SSH2SOC2Dialog(char *fingerprint)
{
	return SSH2SOCDialog(fingerprint, rSSH2SOC2Dialog);
}


// ---------------------------------------------------------------------------
//		¥ AEIdleProc
// ---------------------------------------------------------------------------

static pascal Boolean AEIdleProc(
	EventRecord			*theEvent,
	long				*sleepTime,
	RgnHandle			*mouseRgn )
{
	DoEvents(NULL);
	ssh2_sched();
	if (!TelInfo->done) {
		DoNetEvents();
	}
	ssh2_sched();
	return false;
}

// ---------------------------------------------------------------------------
//		¥ InteractWithUser
// ---------------------------------------------------------------------------

OSErr InteractWithUser(
	Boolean			isUrgent,
	short			sicnResID,
	short			strResID )
{
	static NMRec	notice;
	static AEIdleUPP sAEIdleProc;
	Handle			mySmallIcon;
	Handle			notificationString;
	char			sicnState, strState;
	OSErr			err = noErr;


	if ( TelInfo->suspended && gAEavail ) {

		if (sAEIdleProc == NULL) {
			sAEIdleProc = NewAEIdleProc( AEIdleProc );
		}

		// Set up a notification which blinks a small icon and (optionally)
		// shows a dialog to the user.
		notice.qType = nmType;
		//notice.nmFlags = 0L;
		//notice.nmPrivate = 0L;
		//notice.nmReserved = 0L;
		notice.nmMark = 1;
/*
		if ( ( mySmallIcon = GetResource( 'SICN', sicnResID ) ) != nil) {
			sicnState = HGetState( mySmallIcon );
			HNoPurge(mySmallIcon);
		}
		notice.nmIcon = mySmallIcon;
*/
		err = GetIconSuite(&mySmallIcon, kNCSAIconFamilyId, svAllSmallData);
		if ( !err )
			notice.nmIcon = mySmallIcon;
		else
			notice.nmIcon = NULL;

		//notice.nmSound = 0L;
		notice.nmSound = (Handle)-1L;	// play system alert sound
		if (isUrgent && strResID != -1 && (notificationString = (Handle)GetString(strResID)) != nil) {
			strState = HGetState(notificationString);
			MoveHHi( notificationString );
			HLock( notificationString );
			notice.nmStr = (StringPtr)*notificationString;
		} else {
			notificationString = nil;
			notice.nmStr = nil;
		}
		notice.nmResp = 0L;
		notice.nmRefCon = 0L;
		// If we're in the background, the next call will either switch us to the front,
		// call the Notification Manager, or return an error indicating that user interaction
		// is not allowed.
		// If interaction is allowed (and we're in the back), this call will wait for our
		// application to become frontmost and then return noErr.
		// If we're already in front, it returns noErr.
		//err = AEInteractWithUser(kAEDefaultTimeout, &notice, gAEIdleProc);
		//err = ::AEInteractWithUser(kNoTimeOut, &notice, gAEIdleProc);
		err = AEInteractWithUser( kNoTimeOut, &notice, sAEIdleProc );

		//if ( !err )
		//	TelInfo->suspended = false;

		//if ( mySmallIcon )
		//	HSetState( mySmallIcon, sicnState );
		if ( notificationString )
			HSetState( notificationString, strState );
	}

	return err;
}

void InternalBufferDialog(StringPtr prompt, StringPtr password)
{
	DialogPtr		dlog;
	short			item;
	ModalFilterUPP  internalBufferFilterUPP;

	internalBufferFilterUPP = NewModalFilterProc(InternalBufferFilter);
	*password = '\0';					// Zero out the buffered password
	dlog = GetNewDialog(rInternalBufferDialog, 0L, (WindowPtr)-1L);
	if ( dlog ) {
		ParamText(prompt, "\p", "\p", "\p");
		SetWRefCon(dlog, (long)password);	// Stash the buffer's address
		SetDialogDefaultItem(dlog, 1);
		SetDialogTracksCursor(dlog, 1);
		ShowWindow(dlog);
		do {
			ModalDialog(internalBufferFilterUPP, &item);
		} while (item != 1);				// Until the OK button is hit
		DisposeDialog(dlog);
	}
	DisposeRoutineDescriptor(internalBufferFilterUPP);
}

static pascal Boolean
InternalBufferFilter(DialogPtr dlog,EventRecord *event,short *itemHit)
{	char	key;
	short	start,end;
	char	*buffer;
	
	if (event->what != keyDown && event->what != autoKey)
		return CallStdFilterProc(dlog, event, itemHit);
	
	key = event->message & charCodeMask;
	
	switch (key) {
		case '.':			// Cmd-.
			if (!(event->modifiers & cmdKey))
				break;
		case '\r':			// Return
		case '\003':		// Enter
		case '\033':			// Esc
		case '\t':			// Tab
		case '\034':		// Left arrow
		case '\035':		// Right arrow
		case '\036':		// Up arrow
		case '\037':		// Down arrow
			return CallStdFilterProc(dlog, event, itemHit);
		default:			// Everything else falls through to be dealt with
			break;			//	below
	}

	if (((DialogPeek)dlog)->editField == 2 ) {	// item-1 !!! => password
		start = (**((DialogPeek)dlog)->textH).selStart;	// Get the current selection
		end = (**((DialogPeek)dlog)->textH).selEnd;
		
		buffer = (char*)GetWRefCon(dlog);		// Get the buffer's address
		
		if (start != end) {					// If there's a selection, delete it
			DeleteRange(buffer,start,end);
		}
		if (key == '\010') {	// Backspace
			if (start != 0) {
				DeleteRange(buffer,start-1,start);	// Delete the character to the left
			}
		} else {
			if ((event->modifiers & cmdKey)) {
				if (key == 'v' || key == 'V') {
					// get scrap
					Handle hText;
					OSErr err;
					if ((err = GetScrapHandle('TEXT', &hText)) == noErr) {
						int len = GetHandleSize(hText);
						if (len + *buffer <= kMaxPasswordLen) {
							int i;
							for (i = 0; i < len; i++) {
								// Insert the real key into the buffer
								if (InsertChar(buffer,start++,(*hText)[i],63)) {
									TEKey('¥', ((DialogPeek)dlog)->textH);
								} else {
									break;
								}
							}
						}
						// add scrap to textedit view
						DisposeHandle(hText);
						event->what = nullEvent;
					}
				}
			} else {
				// Insert the real key into the buffer
				if (InsertChar(buffer,start,key,63)) {
					event->message = '¥';	// Character to use in field
				} else {
					event->what = nullEvent;
				}
			}
		}
	}
	return false; 		// Let ModalDialog insert the fake char
}

static void
DeleteRange(char *buffer,short start,short end)
{	register char	*src,*dest,*last;
	
	last = buffer + *buffer;
	
	src = buffer + end + 1;
	dest = buffer + start + 1;
	
	while (src <= last)			// Shift character to the left over the removed characters
		*(dest++) = *(src++);
	
	(*buffer) -= (end-start);	// Adjust the buffer's length
}

static Boolean
InsertChar(char *buffer,short pos,char c,short max)
{	register short	index,len;
	
	len = *buffer;
	
	if (len >= max) {		// if the string is full, return
		SysBeep(5);
		return false;
	}

	for (index = len;index > pos;index--)	// Shift characters to the right to make room
		buffer[index+1] = buffer[index];
	
	buffer[pos+1] = c;		// Fill in the new character
	
	(*buffer)++;			// Add one to the length of the string
	return true;
}

static OSErr
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


/*
	File:		PasswordDialog.c

	Contains:	This file contains all the interesting password stuff

	Written by: Tim Dierks	

	Copyright:	Copyright © 1991-1999 by Apple Computer, Inc., All Rights Reserved.

				You may incorporate this Apple sample source code into your program(s) without
				restriction. This Apple sample source code has been provided "AS IS" and the
				responsibility for its operation is yours. You are not permitted to redistribute
				this Apple sample source code as "Apple sample source code" after having made
				changes. If you're going to re-distribute the source, we require that you make
				it clear in the source that the code was descended from Apple sample source
				code, but that you've made changes.

	Change History (most recent first):
				8/9/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1
				

*/

#include <Types.h>
#include <Memory.h>
#include <Resources.h>
#include <OSUtils.h>
#include <Quickdraw.h>
#include <Fonts.h>
#include <Events.h>
#include <Windows.h>
#include <Menus.h>
#include <Dialogs.h>
#include <TextEdit.h>
#include <Drag.h>
#include <Scrap.h>

#include "PasswordDialog.h"
/*
#include "movableModal.h"
#include "DlogUtils.proto.h"
*/

static pascal Boolean InternalBufferFilter(DialogPtr dlog,EventRecord *event,short *itemHit);
static void DeleteRange(char *buffer,short start,short end);
static void InsertChar(char *buffer,short pos,char c);

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
		ShowWindow(dlog);
		SetDialogDefaultItem(dlog, 1);
		SetDialogCancelItem(dlog, 2);
		do {
			ModalDialog(NULL, &item);
		} while (item != 1 && item != 2);				// Until the OK/Cancel button is hit
		result = (item == 2);
		DisposeDialog(dlog);
	}
	/*DisposeRoutineDescriptor(yesNoFilterUPP);*/
	return result;
}

/*
Boolean SSH2PasswordDialog(StringPtr prompt, StringPtr password)
{
	DialogPtr		dlog;
	short			item;
	ModalFilterUPP  internalBufferFilterUPP;

	SetUpMovableModalMenus();
	internalBufferFilterUPP = NewModalFilterProc(InternalBufferFilter);
	*password = '\0';
	//dlog = GetNewDialog(rSSH2PasswordDialog, 0L, (WindowPtr)-1L);
	dlog = GetNewMyDialog(rSSH2PasswordDialog, 0L, (WindowPtr)-1L, (void *)ThirdCenterDialog);
	if ( dlog ) {
		SInt16 itemType;
		Handle itemHandle;
		Rect itemRect;
		GetDialogItem(dlog, 5, &itemType, &itemHandle, &itemRect);
		SetDialogItemText(itemHandle, prompt);
		//ParamText(prompt, "\p", "\p", "\p");
		ShowWindow(dlog);
		SetWRefCon(dlog, (long)password);	// Stash the buffer's address
		SetDialogDefaultItem(dlog, 1);
		SetDialogCancelItem(dlog, 2);
		do {
			//ModalDialog(internalBufferFilterUPP, &item);
			movableModalDialog(internalBufferFilterUPP, &item);
		} while (item != 1 && item != 2);	// Until the OK button is hit
		DisposeDialog(dlog);
	}
	DisposeRoutineDescriptor(internalBufferFilterUPP);
	ResetMenus();
}
*/

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
		ShowWindow(dlog);
		SetWRefCon(dlog, (long)password);	// Stash the buffer's address
		SetDialogDefaultItem(dlog, 1);
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
		return false;				// We don't want to deal with them
	
	key = event->message & charCodeMask;
	
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
							InsertChar(buffer,start++,(*hText)[i]); // Insert the real key into the buffer
							TEKey('¥', ((DialogPeek)dlog)->textH);
						}
					}
					// add scrap to textedit view
					DisposeHandle(hText);
					event->what = nullEvent;
				}
			}
		} else {
			InsertChar(buffer,start,key);		// Insert the real key into the buffer
			event->message = '¥';				// Character to use in field
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

static void
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

	//	allocate a temporary handle
	if ( ( err = MemError() ) != noErr )
	{
		goto cleanup;
	}

	//	look for an item with the specified flavor on the desk scrap
	if ( ( scrapResult = GetScrap( *dataHandle, dataFlavor, &scrapOffset ) ) < 0 )
	{
		//	a negative result from GetScrap is an error code
		err = scrapResult;
		DisposeHandle( *dataHandle );
		goto cleanup;
	}

	//	clear result code
	err = noErr;

cleanup:
	//	return result code
	return err;
}


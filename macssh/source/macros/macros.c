/*
*    macros.c
*	 originally by Gaige B. Paulsen
*
*	NCSA Revisions:
*	7/92	Moved here from event.c and maclook.c by JMB
*	11/94	Rewritten to use handle based macros JMB
*/

// 11/98 - totally revised for BetterTelnet 2.0b5 (RAB)

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5


#include "network.proto.h"				/* For netwrite proto */
#include "wind.h"				/* For WindRec definition */
#include "DlogUtils.proto.h"
#include "parse.proto.h"
#include "event.proto.h"
#include "menuseg.proto.h"

#include "vsdata.h"
#include "vsinterf.proto.h"

#include "macros.proto.h"
#include "netevent.proto.h"

#include "Sets.proto.h" //for CStringToFile
#include "movableModal.h"
#include "PasswordDialog.h"

extern void ssh2_sched();

/* Macro Defines */
#define MACRO_IP		0xff	/* Send IP number here */
#define MACRO_LINES		0xfe	/* Send # of lines here */
#define	MACRO_MAX_LEN	256		// Maximum macro length

extern Cursor *theCursors[];

//Handle	gMacros[10];

Handle oldMacros;
NewMacroInfo oldMacroIndexes;
short dialogPane; // which key set are we using?
				  // (needs to be global because there's no struct)

void	MACROSunload(void) {}

void	initmacros( void)
{
	short i;

/*	for (i=0; i<10 ; i++) {
		gMacros[i] = nil;
		} */
	for (i=0; i<NUM_MACROS; i++) // RAB BetterTelnet 2.0b5 - new macros code
		TelInfo->newMacros.index[i] = 0;
	TelInfo->newMacros.handle = 0;
	setupNewMacros(&TelInfo->newMacros);
}

void disposemacros(NewMacroInfo *macrost)	// same as init, but don't put in default
											// null-macros and get rid of all the handles
{
	short i;

	if (macrost->handle != NULL)
		DisposeHandle(macrost->handle);

/*	for (i = 0; i < 10; i++)
	{
		if (gMacros[i] != NULL)
			DisposeHandle(gMacros[i]);
		gMacros[i] = NULL;
	} */

	for (i=0; i<NUM_MACROS; i++) // RAB BetterTelnet 2.0b5 - new macros code
		macrost->index[i] = 0;
	macrost->handle = 0;
}

void setupNewMacros(NewMacroInfo *macrost) // RAB BetterTelnet 2.0b5
{
	Handle macroHandle;
	Ptr pos;
	long len;
	short i;

	len = NUM_MACROS;
	macrost->handle = macroHandle = myNewHandle(len);
	HLock(macroHandle);
	pos = *macroHandle;

	while (len) {
		*pos = 0;
		pos++;
		len--;
	}

	for (i = 0; i < NUM_MACROS; i++)
		macrost->index[i] = i;

	HUnlock(macroHandle);
}

void CheckGlobalMacros(void)
{
	OSErr err;
	FSSpec macroFile;
	Boolean dummy1, dummy2;
	Str255 tempString;

// RAB BetterTelnet 2.0b5
// if there's no global file, we have to get our default from the resource fork...
// we load them first just in case we have OLD macros in the macros file, in which case
// we have to add them to the defaults rather than replace the default

	GetDefaultMacros();

	GetIndString(tempString, MISC_STRINGS, MACROS_FILE_NAME);
	err = FSMakeFSSpec(TelInfo->PrefFolder.vRefNum, TelInfo->PrefFolder.parID, tempString, &macroFile);
	if (!err) {
		ResolveAliasFile(&macroFile, FALSE, &dummy1, &dummy2);
		loadMacros(&TelInfo->newMacros, &macroFile);
	}
}

// RAB BetterTelnet 2.0b5 - added GetDefaultMacros
// note that we ASSUME the resource fork has new-style macros - don't even think
// about putting the old format there. it would be useless anyway given that the
// purpose of having defaults in the resource fork is to make sure the arrows et al. work
// right from the get-go.
void GetDefaultMacros(void)
{
	Handle resHandle;

	UseResFile(TelInfo->ApplicationFile);
	resHandle = Get1Resource('Mcro', 128);
	UseResFile(TelInfo->SettingsFile);
	if (!resHandle) {		// RAB 2.0b5 - get the blank macros, then
		setupNewMacros(&TelInfo->newMacros); // ignore the error, and let the lusers learn on their own
		return;								// not to use ResEdit unless they know what they're doing :-)
	}
	DetachResource(resHandle);
	HLock(resHandle);
	disposemacros(&TelInfo->newMacros); // we are replacing, not merging
	ParseMacrosFromHandle(&TelInfo->newMacros, resHandle);
}

void SaveGlobalMacros(NewMacroInfo *macrost)
{
	OSErr err;
	FSSpec macroFile;
	Boolean dummy1, dummy2;
	Str255 tempString;

	GetIndString(tempString, MISC_STRINGS, MACROS_FILE_NAME);
	err = FSMakeFSSpec(TelInfo->PrefFolder.vRefNum, TelInfo->PrefFolder.parID, tempString, &macroFile);
	if ((!err) || (err == -43)) {
		ResolveAliasFile(&macroFile, FALSE, &dummy1, &dummy2);
		saveMacros(macrost, &macroFile);
	}
}

void setmacro(NewMacroInfo *macrost, short n, char *s) // RAB BetterTelnet 2.0b5
{
	// note that we don't check MACRO_MAX_LEN anymore - the new structure can handle most any
	// size of macro, doesn't use 8-bit length descriptors anywhere (all 16 or 32 bit) -
	// however, the final implementation still uses Str255-sized memory spaces. It is the
	// responsibility of whoever calls getmacro() to pass a reasonable value for "room"
	// to ensure correct truncation of oversized values.
	short i;
	Handle macroHandle;
	Ptr pos;
	long offset, offset2, offset3, ohlen, nhlen;
	short oslen, nslen; // ..len: o = old, n = new, h = handle, s = string

	macroHandle = macrost->handle;
	HLock(macroHandle);
	pos = *macroHandle;
	ohlen = GetHandleSize(macroHandle); // get all the lengths and offsets calculated
	if (n)
		offset = macrost->index[n];
	else offset = 0;
	oslen = strlen(pos + offset);
	nslen = strlen(s);
	nhlen = ohlen - oslen + nslen;
	offset2 = offset + oslen + 1; // the end of the old string
	offset3 = offset + nslen + 1; // the end of the new strings
	if (nhlen > ohlen) {
		HUnlock(macroHandle);
		SetHandleSize(macroHandle, nhlen); // make sure we have enough room
		HLock(macroHandle);
		pos = *macroHandle;
	}

	if (ohlen > offset2) { // move data after the insertion position up or down
		BlockMoveData(pos + offset2, pos + offset3, ohlen - offset2);
		for (i = n + 1; i < NUM_MACROS; i++) // fix the offsets
			macrost->index[i] += offset3 - offset2;
	}

	HUnlock(macroHandle);
	SetHandleSize(macroHandle, nhlen);
	HLock(macroHandle);
	pos = *macroHandle; // dereference _yet_ _again_ (rrgh)

	strcpy(pos+offset, s);
	HUnlock(macroHandle);
}

#if 0
void	oldsetmacro(short n, char *s)			/* Set macro number <n> to the value of s */
{
	unsigned char	*p;
	short	num, pos, escape;
	short	len;
	OSErr 	memError;
	
	if (n<0  || n>9)
		return;

	// Restrict the maximum length of macros to MACRO_MAX_LEN bytes
	len = strlen(s)+1;
	if (len > (MACRO_MAX_LEN - 1)) {
		len = MACRO_MAX_LEN;
		s[MACRO_MAX_LEN - 1] = 0;
		}
	
	// If this is an empty string, remove whatever storage might have been used previously
	// by this macro.
	if (len == 1) {
		if (gMacros[n] != nil) {
			DisposeHandle(gMacros[n]);
			gMacros[n] = nil;
			}
		return;
		}
		
	// If neccessary, create storage for the macro
	if (gMacros[n] == nil) {
		gMacros[n] = myNewHandle(len);
		if (gMacros[n] == nil) {		// Memory error
			return;
			}
		}

	// Adjust the handle to the proper size (may be making an existing macro longer)
	memError = mySetHandleSize(gMacros[n], len);
	if (memError != noErr) {
		return;
		}
	
	HLock(gMacros[n]);
	p = (unsigned char *)*gMacros[n];

	num = 0;
	pos = 0;
	escape = 0;
	
	while ( *s) {
		if (escape) {
			escape = 0;
			switch (*s) {
				case 'i':
					if ( pos >0) {
						*p++=num;
						*p++=*s;
						pos=0;
						}
					*p++=MACRO_IP;
					break;
				case '#':
					if ( pos >0) {
						*p++=num;
						*p++=*s;
						pos=0;
						}
					*p++=MACRO_LINES;
					break;
				case 'n':
					if ( pos >0) {
						*p++=num;
						*p++=*s;
						pos=0;
						}
					*p++='\012';
					break;
				case 'r':
					if ( pos >0) {
						*p++=num;
						*p++=*s;
						pos=0;
						}
					*p++='\015';
					break;
				case 't':
					if ( pos >0) {
						*p++=num;
						*p++=*s;
						pos=0;
						}
					*p++='\t';
					break;
				case '"':
					if ( pos >0) {
						*p++=num;
						*p++=*s;
						pos=0;
						}
					*p++='\"';
					break;

						
				case '\\':
					if ( pos >0) {
						*p++=num;
						escape=1;
						pos=0;
						num=0;
						}
					else
						*p++='\\';
					break;
				default:
					if (*s <='9' && *s >='0' && pos <3) {
						num= num*8+( *s -'0');
						pos++;
						escape=1;
						}
					else {
						if (pos ==0 && num==0) {
							*p++='\\';
							*p++=*s;
							}
						else {
							*p++=num;
							pos= 0;
							s--;			/* back up the buffer. */
							}
						}
					break;
				}
			}
		else {
			if (*s=='\\') {
				num=0;
				pos=0;
				escape=1;
				}
			else
				*p++=*s;
			}
		s++;
		}

	if (pos >0) *p++=num;
	
	*p=0;
	
	// The resultant macro may be shorter than the input string due to escaped characters.
	// So, recalculate the length of the macro and resize than handle if neccessary.
	len = strlen(*gMacros[n])+1;
	
	HUnlock(gMacros[n]);
	mySetHandleSize(gMacros[n], len);
} /* setmacro */
#endif

short	sendmacro(struct WindRec *tw, short n)				/* send macro number n */
// RAB BetterTelnet 2.0b5 - changed to support new macro system
{
	char			temp[20];
	unsigned char	*s, *first, *p;
	unsigned char	myipnum[4];
	Handle			mh, ph;
	short			i, num, pos, escape, length;
	char			*plabel;
	Str255			password;
	unsigned long	startTicks;
	
	// Invalid number
	if (n < 0 || n >= NUM_MACROS) {
		return -1;
	}

	// first we actually have to GET the macro (2.0b5)
	// we use getmacrointohandle for this
	i = -1;
	if (tw->sessmacros.handle)
		i = getmacrointohandle(&tw->sessmacros, n, &mh);
	if (i == -1)
		i = getmacrointohandle(&TelInfo->newMacros, n, &mh);
	if (i == -1)
		return 0;

// Empty macro, so do nothing
//	if (gMacros[n] == nil) {
//		return 0;
//		}

//	HLock(gMacros[n]);
//	mp = (unsigned char *)*gMacros[n];

	s = (unsigned char *)*mh;

	ph = myNewHandle(GetHandleSize(mh) + 256);
	if (!ph) return 0; // ouch
	HLock(ph);
	first = p = (unsigned char *)*ph;

	netgetip(myipnum);

// totally revised - we parse as we go (RAB BetterTelnet 2.0b5)
// this has the pleasant side effect of getting rid of the use of 254 and 255 as special
// characters, so we can use them in macros now - you can now send telnet options from a macro!
	num = 0;
	pos = 0;
	escape = 0;

	while ( *s) {
		if (escape == 1 && (((*s < '0' || *s > '9') && pos) || pos >= 3)) {
												  // do this ONCE -
												  // it's a kludge to do this in each case
			*p++=num;
			pos = 0;
			escape = 0;
			// now the rest of the code will take care of whatever char this was
		}
		if (escape == 1) {
			escape = 0;
			switch (*s) {
				case 'i':
					SendStringAsIfTyped(tw, (char *)first, p-first);
					sprintf(temp,"%d.%d.%d.%d", myipnum[0], myipnum[1], myipnum[2], myipnum[3]);
					SendStringAsIfTyped(tw, temp, strlen(temp));
					first = p = (unsigned char *)*ph;
					break;
				case '#':
					SendStringAsIfTyped(tw, (char *)first, p-first);
					sprintf(temp,"%d", VSgetlines(tw->vs));
					SendStringAsIfTyped(tw, temp, strlen(temp));
					first = p = (unsigned char *)*ph;
					break;
				case 'n':
					*p++='\012';
					break;
				case 'r':
					*p++='\015';
					break;
				case 't':
					*p++='\t';
					break;
				case '"':
					*p++='\"';
					break;
				case '\\':
					*p++='\\';
					break;
				case 'k':
					escape = 2;
					break;
				case 'w':
					// switch to window num
					if ( s[1] >= '1' && s[1] <= '9') {
						HandleMenuCommand( (connMenu << 16 | FIRST_CNXN_ITEM) + s[1] - '1', 0 );
						++s;
					}
					break;
				default:
					if (*s <='9' && *s >='0' && pos <3) {
						num= num*8+( *s -'0');
						pos++;
						escape=1;
						}
					else {
//						if (pos ==0 && num==0) {
							*p++='\\';
							*p++=*s;
//							}
//						else {
//							*p++=num;
//							pos= 0;
//							s--;			/* back up the buffer. */
//							}
						}
					break;
				}
		} else if (escape == 2) {
			if (*s == '{') {
				escape = 3;
				plabel = NULL;
			} else {
				escape = 0;
			}
		} else if (escape == 3) {
			if (*s != '}') {
				if ( plabel == NULL )
					plabel = s;
			} else {
				*s = 0;
				if (plabel && SSH2PasswordDialog(plabel, password, NULL)) {
					SendStringAsIfTyped(tw, (char *)first, p-first);
					// better wait for echo off...
					startTicks = TickCount();
					while (TickCount() - startTicks < 30) {
						ssh2_sched();
						DoNetEvents();
					}
					first = p = (unsigned char *)*ph;
					SendStringAsIfTyped(tw, (char *)password + 1, password[0]);
				} else {
					// malformed macro, or pasword cancel
					first = p = (unsigned char *)*ph;
					break;
				}
				escape = 0;
			}
		} else {
			if (*s=='\\') {
				num=0;
				pos=0;
				escape=1;
				}
			else
				*p++=*s;
			}
		s++;
		}

	if (pos >0) *p++=num;

	SendStringAsIfTyped(tw, (char *)first, p-first);

	DisposeHandle(mh);
	DisposeHandle(ph);
	return 0;
}

// postcondition: returns _LOCKED_ macro handle in desthp
short getmacrointohandle(NewMacroInfo *macrost, short n, Handle *desthp)
{ // RAB BetterTelnet 2.0b5 - for new sendmacro
	Handle macroHandle, desth;
	Ptr pos, dest;
	long offset, length;

	if (n < 0 || n >= NUM_MACROS) {
		return -1;
		}

	// find the macro in the new structure
	if (n == 0)  // no offset for this
		offset = 0;
	else
		offset = macrost->index[n];

	macroHandle = macrost->handle;
	HLock(macroHandle);
	pos = *macroHandle;
	pos += offset;

	length = strlen(pos);
	if (!length) return -1; // don't bother if it's null
	desth = myNewHandle(length + 1);
	if (!desth) return -1; // ouch
	HLock(desth);
	*desthp = desth;
	dest = *desth;
	strcpy(dest, pos); // here we go

	HUnlock(macroHandle); // that's right, we DON'T unlock the handle into which the macro
						  // has been copied (desth), only the global macros handle!
	return 0;
}

short getmacro(NewMacroInfo *macrost, short n, char *dest,
				short room) // no more quoteflag! no more kludge!
{ // RAB BetterTelnet 2.0b5
	Handle macroHandle;
	Ptr pos;
	long offset, length;

	if (n < 0 || n >= NUM_MACROS) {
		return -1;
		}

	// find the macro in the new structure
	if (n == 0)  // no offset for this
		offset = 0;
	else
		offset = macrost->index[n];

	macroHandle = macrost->handle;
	HLock(macroHandle);
	pos = *macroHandle;
	pos += offset;

	length = strlen(pos);
	if (length >= room) { // >= compensates for null termination
		length = room - 1; // truncate if too long
		strncpy(dest, pos, length);
		pos += length;
		*pos = 0;
	} else
		strcpy(dest, pos); // or just move it in
	HUnlock(macroHandle);
	return 0;
}

#if 0
short	oldgetmacro(short n, char *dest, short room, short quoteflag)
{
	unsigned char	*s;

	// Invalid number
	if (n<0 || n>9) {
		return -1;
		}
	
	// Empty macro, so return empty string
	if (gMacros[n] == nil) {
		*dest = 0;
		return 0;
		}
		
	s = (unsigned char *)*gMacros[n];
	
	while (*s && (room >= 5)) {  // 5 = (size of \xxx) + (terminating \0)
		switch( *s) {
			case MACRO_IP :
				*dest++='\\';
				*dest++='i';
				room--;
				break;
			case MACRO_LINES :
				*dest++='\\';
				*dest++='#';
				room--;
				break;
			case '\\':
				*dest++='\\';
				*dest++='\\';
				room--;
				break;
			case '\015':
				*dest++='\\';
				*dest++='r';
				room--;
				break;
			case '\012':
				*dest++='\\';
				*dest++='n';
				room--;
				break;
			case '\t':
				*dest++='\\';
				*dest++='t';
				room--;
				break;
			default: 
				if ( isprint(*s) && ((*s != '\042') || (!quoteflag)))
								// RAB BetterTelnet 2.0b3 - fix for quote bug
					*dest++=*s;
				else {
					*dest++='\\';
					*dest++= (*s / 64) +'0';
					*dest++= ((*s % 64) / 8)+'0';
					*dest++= (*s % 8) +'0';
					room = room - 3;
					}
				break;
			}
		room--;
		s++;
		}

	*dest = 0;
	return( 0);
}
#endif

void	Macros( void)
{
	DialogPtr dtemp;
	short dItem;
	short i;
	Rect dBox;
	Str255 temp;
	Handle MacString[10], rubbish;

	setLastCursor(theCursors[normcurs]);

	if (TelInfo->macrosModeless) {
		SelectWindow(TelInfo->macrosModeless);
		return;
	}

	dtemp=GetNewMyDialog( MacroDLOG, NULL, kInFront, (void *)ThirdCenterDialog);
	SetDialogDefaultItem(dtemp, 1);
	SetDialogCancelItem(dtemp, 2);
	SetDialogTracksCursor(dtemp, 1);

	dialogPane = 0; // start with Command-[0-9]

	// RAB BetterTelnet 2.0b5 - fix cancel button so it works again
	oldMacros = TelInfo->newMacros.handle;
	HandToHand(&oldMacros);
	oldMacroIndexes = TelInfo->newMacros;

	// now fix the strings
	for (i=0; i<10; i++) {
		GetIndString(temp, 7100, i + (10 * dialogPane) + 1);
		GetDialogItem(dtemp, i+3, &dItem, &rubbish, &dBox);
		if (!temp[0]) { HideDialogItem(dtemp, i+13); HideDialogItem(dtemp, i+3); }
		else { ShowDialogItem(dtemp, i+13); ShowDialogItem(dtemp, i+3); }
		SetDialogItemText(rubbish, temp);
	}

	for (i=0; i<10; i++) {
		getmacro(&TelInfo->newMacros, i, (char *) &temp, 256); /* BYU LSC */
		c2pstr((char *)temp);								/* BYU LSC */
		GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
		SetDialogItemText( MacString[i], temp );
		}
	TelInfo->macrosModeless = dtemp;
}

short ModalMacros(NewMacroInfo *macrost)
{
	DialogPtr dtemp;
	short dItem;
	short i;
	Rect dBox;
	Str255 temp;
	Handle MacString[10], rubbish;

	SetUpMovableModalMenus();
	dtemp=GetNewMyDialog( MacroDLOG + 2, NULL, kInFront, (void *)ThirdCenterDialog);
	SetDialogDefaultItem(dtemp, 1);
	SetDialogCancelItem(dtemp, 2);
	SetDialogTracksCursor(dtemp, 1);
	HideDialogItem(dtemp, 25); // no save default in session macros dialog

	dialogPane = 0; // start with Command-[0-9]

	// RAB BetterTelnet 2.0b5 - fix cancel button so it works again
	oldMacros = macrost->handle;
	HandToHand(&oldMacros);
	oldMacroIndexes = *macrost;

	// now fix the strings
	for (i=0; i<10; i++) {
		GetIndString(temp, 7100, i + (10 * dialogPane) + 1);
		GetDialogItem(dtemp, i+3, &dItem, &rubbish, &dBox);
		if (!temp[0]) { HideDialogItem(dtemp, i+13); HideDialogItem(dtemp, i+3); }
		else { ShowDialogItem(dtemp, i+13); ShowDialogItem(dtemp, i+3); }
		SetDialogItemText(rubbish, temp);
	}

	for (i=0; i<10; i++) {
		getmacro(macrost, i, (char *) &temp, 256); /* BYU LSC */
		c2pstr((char *)temp);								/* BYU LSC */
		GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
		SetDialogItemText( MacString[i], temp );
		}

	TelInfo->macrosModeless = dtemp;

	dItem = 0;
	while ((dItem != 1) && (dItem != 2)) {
		movableModalDialog(0, &dItem);
		MacroDialog(macrost, dtemp, 0, dItem);
	}

	if (dItem == 2) CancelMacros(macrost, dtemp);
	else CloseMacros(macrost, dtemp);
	ResetMenus();
	return dItem;
}
		

// NB: As of 2.0fc1, theEvent can be NULL! It isn't used anywhere, so don't _let_ it be
// used anywhere!
void MacroDialog(NewMacroInfo *macrost, DialogPtr dtemp, EventRecord *theEvent, short dItem)
{
	short i;
	Rect dBox;
	Str255 temp;
	Handle MacString[10], rubbish;

	switch (dItem) {
		case 27: // switch keyset (pane) - RAB BetterTelnet 2.0b5
			for (i=0; i<10; i++) {
				GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
				GetDialogItemText( MacString[i], temp);
				p2cstr(temp);
				setmacro(macrost, i + (10 * dialogPane), (char *) &temp);
			}
			dialogPane = GetCntlVal(dtemp, 27) - 1;
			// now fix the strings
			for (i=0; i<10; i++) {
				GetIndString(temp, 7100, i + (10 * dialogPane) + 1);
				GetDialogItem(dtemp, i+3, &dItem, &rubbish, &dBox);
				if (!temp[0]) { HideDialogItem(dtemp, i+13); HideDialogItem(dtemp, i+3); }
				else { ShowDialogItem(dtemp, i+13); ShowDialogItem(dtemp, i+3); }
				SetDialogItemText(rubbish, temp);
			}
			for (i=0; i<10; i++) {
				getmacro(macrost, i + (10 * dialogPane), (char *) &temp, 256);		
				c2pstr((char *)temp);								
				GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
				SetDialogItemText( MacString[i], temp );
			}
		break;

		case (MacroExport):
			// we _used_ to get all the macros out, but now only the current set of 10
			for (i=0; i<10; i++) {
				GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
				GetDialogItemText( MacString[i], temp);
				p2cstr(temp);
				setmacro(macrost, i + (10 * dialogPane), (char *) &temp);
								// RAB BetterTelnet 2.0b5 (revised)
			}
			saveMacros(macrost, (FSSpec *) NULL);
			break;
		case 25:
			for (i=0; i<10; i++) {
				GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
				GetDialogItemText( MacString[i], temp);
				p2cstr(temp);
				setmacro(macrost, i + (10 * dialogPane), (char *) &temp);
			}
			SaveGlobalMacros(macrost);
			break;
		case (MacroImport):
			loadMacros(macrost, (FSSpec *) NULL);
			for (i=0; i<10; i++) {
				getmacro(macrost, i + (10 * dialogPane), (char *) &temp, 256);		
				c2pstr((char *)temp);								
				GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
				SetDialogItemText( MacString[i], temp );
			}
			break;
		case 1:
		case 2:
			break;
		default:
			if (dItem >2 && dItem <13) 
			{
				i=dItem-3;
				getmacro(macrost, i + (dialogPane * 10), (char *) &temp, 256); /* BYU LSC */
				c2pstr((char *)temp);
				GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
				SetDialogItemText( MacString[i], temp );				/* BYU LSC - Revert the mother */
				SelectDialogItemText( dtemp, i+13, 0, 32767);				/* And select it... */
			}	
			break;
	}
}

void CancelMacros(NewMacroInfo *macrost, DialogPtr dtemp)
{
	if ( TelInfo->macrosModeless == dtemp ) {
		DisposeDialog(dtemp);
		DisposeHandle(macrost->handle);

		*macrost = oldMacroIndexes;
		macrost->handle = oldMacros;

		//if ( TelInfo->macrosModeless ) {
			TelInfo->macrosModeless = 0;
			AdjustMenus();
			DoTheMenuChecks();
		//}
	}
}

void CloseMacros(NewMacroInfo *macrost, DialogPtr dtemp)
{
	short dItem;
	short i;
	Rect dBox;
	Str255 temp;
	Handle MacString[10];

	if ( TelInfo->macrosModeless == dtemp ) {
		for (i=0; i<10; i++) {
			GetDialogItem( dtemp, i+13, &dItem, &MacString[i], &dBox);
			GetDialogItemText( MacString[i], temp);
			p2cstr(temp);
			setmacro(macrost, i + (10 * dialogPane), (char *) &temp);
			}

		DisposeHandle(oldMacros);
		DisposeDialog(dtemp);
		
		//if ( TelInfo->macrosModeless ) {
			TelInfo->macrosModeless = 0;
			AdjustMenus();
			DoTheMenuChecks();
		//}
	}
}

#if 0
void oldsaveMacros(FSSpec *theFile)
{
	SFReply		whereReply;
	short 		refNum,exist;
	FSSpec		macroFile;
	long		junk;
	short 		i;
	char		temp[256], temp2[256];
	Point		where;
	OSErr		err;
	Str255		tempString,tempString2;

	where.h = 100; where.v = 100;

	GetIndString(tempString,MISC_STRINGS,SAVE_MACROS_STRING);
	GetIndString(tempString2,MISC_STRINGS,DEFAULT_MACRO_SET_NAME);

	if (theFile == 0) {
		SFPutFile( where, tempString, tempString2, 0L, &whereReply);

		if (!whereReply.good)
			return;

		BlockMoveData(&whereReply.fName, macroFile.name, (*whereReply.fName)+1); 
		GetWDInfo(whereReply.vRefNum, &macroFile.vRefNum, &macroFile.parID, &junk);
	}
	else
		macroFile = *theFile;

	if ((err = HCreate(macroFile.vRefNum, macroFile.parID, 
			macroFile.name, kNCSACreatorSignature, 'TEXT')) == dupFNErr)
		exist = 1;
	
	err = HOpenDF(macroFile.vRefNum, macroFile.parID, macroFile.name, fsWrPerm, &refNum);

	if (exist) 
		SetEOF(refNum, 0L);

	
	for (i = 0; i < 10; i++)
	{
		oldgetmacro(i, temp, sizeof(temp), 1);
		sprintf(temp2, "key%d = \"", i);
		CStringToFile(refNum,(unsigned char *) temp2);
		if (*temp)
		{
			CStringToFile(refNum,(unsigned char *) temp);
		}
		strcpy(temp2,"\"\015");
		CStringToFile(refNum,(unsigned char *) temp2);

	}
	FSClose(refNum);

}
#endif

void saveMacros(NewMacroInfo *macrost, FSSpec *theFile)
{
	SFReply		whereReply;
	short 		refNum,exist;
	FSSpec		macroFile;
	long		junk, len, len2;
	short 		i;
	Point		where;
	OSErr		err;
	Str255		tempString,tempString2;
	Handle		macroHandle;
	Ptr			pos;

	if (!macrost->handle) return; // sanity check

	where.h = 100; where.v = 100;

	GetIndString(tempString,MISC_STRINGS,SAVE_MACROS_STRING);
	GetIndString(tempString2,MISC_STRINGS,DEFAULT_MACRO_SET_NAME);

	if (theFile == 0) {
		SFPutFile( where, tempString, tempString2, 0L, &whereReply);

		if (!whereReply.good)
			return;

		BlockMoveData(&whereReply.fName, macroFile.name, (*whereReply.fName)+1); 
		GetWDInfo(whereReply.vRefNum, &macroFile.vRefNum, &macroFile.parID, &junk);
	}
	else
		macroFile = *theFile;

	if ((err = HCreate(macroFile.vRefNum, macroFile.parID, 
			macroFile.name, kNCSACreatorSignature, 'TEXT')) == dupFNErr)
		exist = 1;
	
	err = HOpenDF(macroFile.vRefNum, macroFile.parID, macroFile.name, fsWrPerm, &refNum);

	if (exist) 
		SetEOF(refNum, 0L);

// the new code - RAB BetterTelnet 2.0b5
	macroHandle = macrost->handle;
	HandToHand(&macroHandle);
	HLock(macroHandle);
	pos = *macroHandle;
	len = len2 = GetHandleSize(macroHandle);
	while (len) {
		if (*pos == 0) *pos = 13;
		pos++;
		len--;
	}

	pos = *macroHandle;
	junk = 2;
	FSWrite(refNum, &junk, "!\015");
	FSWrite(refNum, &len2, pos);
	DisposeHandle(macroHandle); // it's a copy anyway, get rid of it!

	FSClose(refNum);
}

void loadMacros(NewMacroInfo *macrost, FSSpec *theFile)
{
	SFReply		sfr;
	long		junk;
	SFTypeList	typesok = {'TEXT'};
	Point		where;
	FSSpec		macros;
	OSErr 		err;
	short 		fileRef;
	where.h=100;where.v=100;
	if (theFile == 0L)
	{
		SFGetFile( where, NULL, 0L, 1, typesok, 0L, &sfr);
		if (!sfr.good) return;
		BlockMoveData(&sfr.fName, macros.name, (*sfr.fName)+1); 
		GetWDInfo(sfr.vRefNum, &macros.vRefNum, &macros.parID, &junk);
		err = HOpenDF(macros.vRefNum, macros.parID, macros.name, fsRdPerm, &fileRef);
	}
	else
		err = HOpenDF(theFile->vRefNum, theFile->parID, theFile->name, fsRdPerm, &fileRef);
	
	if (err != noErr)
		return;
	parseMacroFile(macrost, fileRef);
	FSClose(fileRef);
}

void parseMacroFile(NewMacroInfo *macrost, short fileRef)
{
	unsigned char buffer[300],*bufferPtr;
	unsigned char newMacro[256], *newMacroPtr;
	OSErr fileErr = noErr;
	short numMacrosRead = 0;
	short totalLen,i;
	long count=1;

	bufferPtr = buffer; 


	count = 2;
	fileErr = FSRead(fileRef,&count,bufferPtr);
	if ((fileErr != eofErr) && (count == 2))
		if (!strncmp((char *)bufferPtr, "!\015", 2)) {
			disposemacros(macrost);  //sets all handles to null (but disposes them first)
			parseNewMacros(macrost, fileRef);
			return;
		}
	bufferPtr += count;
	count = 1;

	while ((fileErr != eofErr)&&(numMacrosRead < 10))
	{
		fileErr = FSRead(fileRef,&count,bufferPtr);
		while((*bufferPtr != 0x0D)&&(fileErr != eofErr))	//while not CR or EOF
		{
		
			++bufferPtr;
			fileErr = FSRead(fileRef,&count,bufferPtr);
		
		}
		
		totalLen = bufferPtr-buffer;
		bufferPtr = buffer;
		newMacroPtr = newMacro;
		while((*bufferPtr++ != '"')&&(totalLen != 0))
			--totalLen;

		while((*bufferPtr != '"')&&(totalLen != 0))
		{
			*newMacroPtr++ = *bufferPtr++;
			--totalLen;
		}		
		*newMacroPtr = NULL; //make this a C string

		setmacro(macrost, numMacrosRead,(char *)newMacro); // just set the new macro directly
		bufferPtr = buffer;
		++numMacrosRead;
	}
}

// RAB BetterTelnet 2.0b5
// handle should be locked and detached
void ParseMacrosFromHandle(NewMacroInfo *macrost, Handle theHandle)
{
	Ptr macroPtr;
	long macroLength;

	macroPtr = *theHandle;
	macroLength = GetHandleSize(theHandle) - 2;
	// format and sanity checks follow
	if ((macroLength < 1) || (*macroPtr != '!') || (*(macroPtr + 1) != '\015')) {
		DisposeHandle(theHandle);
		setupNewMacros(macrost);
		return;
	} // bah
	BlockMoveData(macroPtr + 2, macroPtr, macroLength); // get rid of !CR
	HUnlock(theHandle);
	SetHandleSize(theHandle, macroLength);
	HLock(theHandle);

// now invoke the actual parser
	parseNewMacros2(macrost, theHandle);
}

// RAB BetterTelnet 2.0b5
void parseNewMacros(NewMacroInfo *macrost, short fileRef)
{
	Handle macroHandle;
	Ptr macroPtr, pos;
	long macroLength, count, len;
	OSErr fileErr;
	short i, flag;

	fileErr = GetEOF(fileRef, &macroLength);
	macroLength -= 2; // compensate for !CR
	macroHandle = myNewHandle(macroLength);

	if (!macroHandle) { // memory error?
		disposemacros(macrost);
		setupNewMacros(macrost); // this better work, or we crash hard
		return;
	}

	HLock(macroHandle);
	macroPtr = *macroHandle;
	count = macroLength;
	FSRead(fileRef, &count, macroPtr);

	parseNewMacros2(macrost, macroHandle);
}

void parseNewMacros2(NewMacroInfo *macrost, Handle macroHandle)
{
	Ptr macroPtr, pos;
	long macroLength, len;
	short i, flag;

	macroLength = GetHandleSize(macroHandle);
	macroPtr = *macroHandle;

	len = macroLength;
	pos = macroPtr;
	i = 1; // first index is obviously always zero, so we use it as length (see below)
	flag = 0;

	while (len) {
		if (*pos == 13) *pos = 0; // for strlen
		pos++;
		len--;
	}

	len = macroLength;
	pos = macroPtr;
	
	while ((i < NUM_MACROS) && (len > 1)) { // if len = 1, this is the last char;
											// then the index points out of the handle!
		if (*pos == 0) {
			macrost->index[i] = (pos - macroPtr + 1);
			i++;
		}
		pos++;
		len--;
	}

	macrost->handle = macroHandle;
	macrost->index[0] = macroLength; // first index is length of whole shebang
	HUnlock(macroHandle);
	fixMacros(macrost); // make sure there's an entry for each macro
}

// This routine makes sure there is at least a null byte (and accompanying index)
// for every defined macro numbered n such that n < NUM_MACROS
// It lengthens the handle if necessary (backwards compatibility),
// but does NOT truncate additional data (forwards compatibility).
void fixMacros(NewMacroInfo *macrost)
{
	Handle macroHandle;
	Ptr macroPtr, pos;
	long len, len2;
	short i, num;

	macroHandle = macrost->handle;
	HLock(macroHandle);
	macroPtr = *macroHandle;
	len = macrost->index[0];
	pos = macroPtr;
	pos += (len - 1);

	if (*pos != 0) {
		len++;
		HUnlock(macroHandle);
		SetHandleSize(macroHandle, len);
		HLock(macroHandle);
		macroPtr = *macroHandle;
		pos = macroPtr + (len - 1);
		*pos = 0;
	}

	num = 0;
	for (i = 1; i < NUM_MACROS; i++) {
		if (macrost->index[i] == 0) {
			num = i;
			break;
		}
	}

	if (num) {
		len2 = len;
		len += (NUM_MACROS - num);
		HUnlock(macroHandle);
		SetHandleSize(macroHandle, len);
		HLock(macroHandle);
		macroPtr = *macroHandle;
		pos = macroPtr + len2;
		for (i = num; i < NUM_MACROS; i++, pos++) {
			*pos = 0;
			macrost->index[i] = pos - macroPtr;
		}
	}

	HUnlock(macroHandle);
	macrost->index[0] = len;
}
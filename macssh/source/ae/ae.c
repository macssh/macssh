/*
*	ae.c
*	Code to handle the AppleEvents we recognize
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "wind.h"				// For connections.proto.h
#include "ae.proto.h"
#include "connections.proto.h"	// For OpenConnectionFromURL proto
#include "Sets.proto.h"			// For readconfig proto
#include "macros.proto.h"		// For loadMacro proto
#include "menuseg.proto.h"		// For HandleQuit proto
#include "telnetdebug.h"				// For ShowDebugWindow proto
#include "parse.proto.h"
#include "linemode.proto.h"
#include "DlogUtils.proto.h"

extern WindRec *screens;
extern short scrn;

static void ProcessURLEscapeCodes (char *url, char **end);

void AEunload(void) { }

SIMPLE_UPP(MyHandleODoc, AEEventHandler);
pascal OSErr  MyHandleODoc (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	FSSpec		myFSS;
	AEDescList	docList;
	OSErr		err;
	long		index;
	long		itemsInList;
	Size		actualSize;
	AEKeyword	keywd;
	DescType	returnedType;
	FInfo		fileInfo;
	
	if ((err = AEGetParamDesc (theAppleEvent, keyDirectObject, typeAEList, &docList)) != noErr)
		return err;

	// check for missing parameters
	if ((err = MyGotRequiredParams (theAppleEvent)) != noErr)
		return err;

	// count the number of descriptor records in the list
	if ((err = AECountItems (&docList, &itemsInList)) != noErr)
		return err;

	for (index = 1; index <= itemsInList; index++) {
			err = AEGetNthPtr (&docList, index, typeFSS, &keywd, &returnedType, 
								(Ptr) &myFSS, sizeof(myFSS), &actualSize);
			if (err) {
				AEDisposeDesc (&docList);
				return err;
			}

			FSpGetFInfo(&myFSS, &fileInfo);        /* make sure it's a data file */

			if (fileInfo.fdType == kNCSASetFileType) {
				TelInfo->gotDocument = 1;
				readconfig(myFSS);		// Read the actual set
			} else if(fileInfo.fdType == 'TEXT') {
				loadMacros(&TelInfo->newMacros, &myFSS);
			} else if (fileInfo.fdType == 'DBUG') {
				TelInfo->debug = TRUE;
				ShowDebugWindow();	
			}		
	}				

	err = AEDisposeDesc (&docList);
	return noErr;
}

SIMPLE_UPP(MyHandlePDoc, AEEventHandler);
pascal OSErr  MyHandlePDoc (AppleEvent *theAppleEvent, AppleEvent *reply, long
														handlerRefCon)
{
	// We don't print files.
	return (errAEEventNotHandled);
}

SIMPLE_UPP(MyHandleOApp, AEEventHandler);
pascal OSErr  MyHandleOApp (AppleEvent *theAppleEvent, AppleEvent *reply, long
														handlerRefCon)
{
	// Don't need to do anything for OApp.
	// Well, actually, we do, because we don't want to show the Open Connection box
	// automatically if a set file was opened.

	if (gApplicationPrefs->autoOpenDialog && !TelInfo->gotDocument)
		PresentOpenConnectionDialog();

	return noErr;
}

SIMPLE_UPP(MyHandleQuit, AEEventHandler);
pascal OSErr  MyHandleQuit (AppleEvent *theAppleEvent, AppleEvent *reply, long
														handlerRefCon)
{
	if (HandleQuit())
		return(-128);	// userCancelledErr
			
	return(noErr);	
}

SIMPLE_UPP(MyHandleSendData, AEEventHandler);
pascal OSErr  MyHandleSendData (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;
	DescType	returnedType;
	Size		actualSize;
	char		DataString[255];

	if ((err = AEGetParamPtr(theAppleEvent, keyDirectObject, typeChar, &returnedType,
								DataString, sizeof(DataString)-1, &actualSize)) != noErr)
		return err;

	// check for missing parameters
	if ((err = MyGotRequiredParams(theAppleEvent)) != noErr)
		return err;

	DataString[actualSize] = 0;		// Terminate the C string

	if (actualSize == 0) return noErr;
	if (TelInfo->numwindows < 1) return noErr;

	SendStringAsIfTyped(&screens[scrn], DataString, strlen(DataString));

goodexit:
	err = noErr;
badexit:
	return err;
}

SIMPLE_UPP(MyHandleSendCR, AEEventHandler);
pascal OSErr  MyHandleSendCR (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;

	if (TelInfo->numwindows < 1) return noErr;

	if (screens[scrn].lmode) {
		process_key('\015', &screens[scrn]);
		return noErr;
	}

/*	if (screens[scrn].crmap) 
		SendStringAsIfTyped(&screens[scrn],"\015\0",2);
	else
		SendStringAsIfTyped(&screens[scrn],"\015\012",2); */

	SendCRAsIfTyped(&screens[scrn]);

goodexit:
	err = noErr;
badexit:
	return err;
}

SIMPLE_UPP(MyHandleSuspend, AEEventHandler);
pascal OSErr  MyHandleSuspend (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;

	if (TelInfo->numwindows < 1) return noErr;

	screens[scrn].enabled = 0;
	changeport(scrn, scrn);

	return noErr;
}

SIMPLE_UPP(MyHandleUnSuspend, AEEventHandler);
pascal OSErr  MyHandleUnSuspend (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;

	if (TelInfo->numwindows < 1) return noErr;

	screens[scrn].enabled = 1;
	changeport(scrn, scrn);

	return noErr;
}

SIMPLE_UPP(MyHandleConnect, AEEventHandler);
pascal OSErr  MyHandleConnect (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;
	DescType	returnedType;
	Size		actualSize;
	char		NameString[255];
	char		HostString[255];
	char		LoginString[255];
	char		PasswordString[255];
	char		TitleString[255];
	short		mystrpos, doWait, isSuspended;
	ConnInitParams **theParams;
	SessionPrefs	*SessPtr;
	Boolean		wasAlias;

	if (handlerRefCon == 1) return noErr;
	if (handlerRefCon == 2) return paramErr;

	if ((err = AEGetParamPtr(theAppleEvent, keyDirectObject, typeChar, &returnedType,
								NameString, sizeof(NameString)-1, &actualSize)) != noErr)
		return err;

	NameString[actualSize] = 0;
	if (actualSize == 0) return paramErr;

	if (AEGetParamPtr(theAppleEvent, 'host', typeChar, &returnedType, HostString,
								sizeof(HostString)-1, &actualSize))
		HostString[0] = 0; // nope
	else HostString[actualSize] = 0; // yup

	if (AEGetParamPtr(theAppleEvent, 'logi', typeChar, &returnedType, LoginString,
								sizeof(LoginString)-1, &actualSize))
		LoginString[0] = 0; // nope
	else LoginString[actualSize] = 0; // yup

	if (AEGetParamPtr(theAppleEvent, 'pass', typeChar, &returnedType, PasswordString,
								sizeof(PasswordString)-1, &actualSize))
		PasswordString[0] = 0; // nope
	else PasswordString[actualSize] = 0; // yup

	if (AEGetParamPtr(theAppleEvent, 'wait', typeChar, &returnedType, TitleString,
								sizeof(TitleString)-1, &actualSize))
		doWait = 0;
	else doWait = 1;

	if (AEGetParamPtr(theAppleEvent, 'susp', typeChar, &returnedType, TitleString,
								sizeof(TitleString)-1, &actualSize))
		isSuspended = 0;
	else isSuspended = 1;

	if (AEGetParamPtr(theAppleEvent, 'titl', typeChar, &returnedType, TitleString,
								sizeof(TitleString)-1, &actualSize))
		TitleString[0] = 0; // nope
	else TitleString[actualSize] = 0; // yup


	// check for missing parameters
	if ((err = MyGotRequiredParams(theAppleEvent)) != noErr)
		return err;

	c2pstr(NameString);
	c2pstr(HostString);
	c2pstr(LoginString);
	c2pstr(PasswordString);

	if (HostString[0]) {
		for (mystrpos = 0; mystrpos < StrLength(HostString); mystrpos++)
			if (HostString[mystrpos + 1] == ':')
				HostString[mystrpos + 1] = ' ';
		theParams = NameToConnInitParams((unsigned char *)HostString,
			FALSE, (unsigned char *)NameString, &wasAlias);
	}
	else
		theParams = NameToConnInitParams((unsigned char *)NameString, TRUE, 0, &wasAlias);
	if (theParams == NULL)
		return paramErr;

	SessPtr = *((**theParams).session);
	if ( LoginString[0] ) {
		pstrcpy((unsigned char *)SessPtr->username, (unsigned char *)LoginString);
	}
	if ( PasswordString[0] ) {
		pstrcpy((unsigned char *)SessPtr->password, (unsigned char *)PasswordString);
	}
	if (CreateConnectionFromParams(theParams)) {
		if (doWait) {
			err = AESuspendTheCurrentEvent(theAppleEvent);
			screens[TelInfo->numwindows - 1].cxWeHaveAppleEvent = 1;
			screens[TelInfo->numwindows - 1].cxAppleEvent = *theAppleEvent;
			screens[TelInfo->numwindows - 1].cxAEReply = *reply;
		}
		if (isSuspended)
			screens[TelInfo->numwindows - 1].enabled = 0;
		return noErr;
	}
	err = paramErr;

goodexit:
	err = noErr;
badexit:
	return err;
}

SIMPLE_UPP(MyHandleWait, AEEventHandler);
pascal OSErr  MyHandleWait (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;
	DescType	returnedType;
	Size		actualSize;
	char		DataString[255];
	short		i, k;

	if (handlerRefCon == 1) return noErr;
	if (handlerRefCon == 2) return paramErr;

	if (TelInfo->numwindows < 1) return noErr; // RAB BetterTelnet 2.0b2 (oops)

	if ((err = AEGetParamPtr(theAppleEvent, keyDirectObject, typeChar, &returnedType,
								DataString, sizeof(DataString)-1, &actualSize)) != noErr)
		return err;

	DataString[actualSize] = 0;
	if (actualSize == 0) return paramErr;

	// check for missing parameters
	if ((err = MyGotRequiredParams(theAppleEvent)) != noErr)
		return err;

	if (err = AESuspendTheCurrentEvent(theAppleEvent)) return err;

	screens[scrn].waWeHaveAppleEvent = 1;
	screens[scrn].waAppleEvent = *theAppleEvent;
	screens[scrn].waAEReply = *reply;
	screens[scrn].waWasEnabled = screens[scrn].enabled;
	screens[scrn].enabled = 1;
	screens[scrn].waWaitPos = 0;
	screens[scrn].waWaiting = 1;
	screens[scrn].waWaitLength = strlen(DataString);
	strcpy(screens[scrn].waWaitString, DataString);

// RAB BetterTelnet 2.0b2
// This next code computes the Knuth-Morris-Platt prefix function for the string
// search.

	screens[scrn].waPrefixFunction[0] = 0;
	k = 0;

	if (screens[scrn].waWaitLength >= 2)
		for (i = 1; i < screens[scrn].waWaitLength; i++) {
			while ((k > 0) &&
				(screens[scrn].waWaitString[k] != screens[scrn].waWaitString[i]))
					k = screens[scrn].waPrefixFunction[k - 1];
			if (screens[scrn].waWaitString[k] == screens[scrn].waWaitString[i])
				k++;
			screens[scrn].waPrefixFunction[i] = k;
		}

	changeport(scrn, scrn);

	return noErr;
}

SIMPLE_UPP(MyHandleGURL, AEEventHandler);
pascal OSErr  MyHandleGURL (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon)
{
	OSErr		err;
	DescType	returnedType;
	Size		actualSize;
	char		URLString[255];
	char		*beg, *end, *user, *password, *portstring, *host;
	short		ssh = -1;
	ConnInitParams **theParams = NULL;
	Boolean		wasAlias;
	unsigned char favorite[255];
	long		port;

	if ((err = AEGetParamPtr(theAppleEvent, keyDirectObject, typeChar, &returnedType,
								URLString, sizeof(URLString)-1, &actualSize)) != noErr)
		return err;

	// check for missing parameters
	if ((err = MyGotRequiredParams(theAppleEvent)) != noErr)
		return err;

	URLString[actualSize] = 0;		// Terminate the C string
	beg = &URLString[0];
	end = &URLString[actualSize-1];

	// Strip leading spaces
	while ((beg < end) && (*beg == ' '))
		beg++;

	// Strip trailing spaces
	while ((end < beg) && (*end == ' '))
		end--;

	// Look for (and strip) beginning and ending angle brackets
	if (*beg == '<') {
		if (*end != '>') {	// Leading angle bracket, but no trailing angle bracket
			err = paramErr;
			goto badExit;
			}
		
		// Nuke the brackets
		beg++;
		end--;
		}

	// Terminate the string we currently have (for strncmp fn's)
	*(end+1) = 0;

	// Look for (and strip) leading "URL:"
	if (!strncmp(beg, "URL:", 4)) {
		beg += 4;
		}

	// Look for (and strip) leading "telnet://"
	if (!strncmp(beg, "telnet://", 9)) {
		beg += 9;
		ssh = 0;
	} else if (!strncmp(beg, "rlogin://", 9)) {
		beg += 9;
		ssh = 0;
	} else if (!strncmp(beg, "ssh://", 6)) {
		beg += 6;
		ssh = 1;
	} else if (!strncmp(beg, "ssh2://", 7)) {
		beg += 7;
		ssh = 2;
	} else {
		err = paramErr;					
		goto badExit;
	}

	// Drop any ending slash
	if (*end == '/') {
		*end = 0;
		end--;
		}

	// Process any escaped characters
	ProcessURLEscapeCodes(beg, &end);

	// Set up for URL parsing
	password = nil;
	portstring = nil;
	host = nil;

	// Assume <user> exists
	user = beg;
	
	// Leading : or @ is a no-no
	if ((*beg == ':') || (*beg == '@')) {
		err = paramErr;
		goto badExit;
		}

	// Scan for : or @ or end of string
	while ((beg <= end) && (*beg != ':') && (*beg != '@'))
		beg++;

	// If we reached the end, only a host was given
	if (beg > end) {
		host = user;
		user = nil;
		goto goodUrl;
		}

	// If : was found, scan in the password (or port)
	if (*beg == ':') {
		*beg++ = 0;
		password = beg;

		// Scan for : or @ or end of string
		while ((beg <= end) && (*beg != ':') && (*beg != '@'))
			beg++;

		if (*beg == ':') {	// xxxx:yyyy:<anything> is a no-no
			err = paramErr;
			goto badExit;
			}

		if (*beg != '@') {	// End of string.  Must be host:port
			host = user;
			user = nil;
			portstring = password;
			password = nil;
			goto goodUrl;
			}

		// Have xxx:yyy@<something>
		*beg++ = 0;	// Terminate password string
		}		
	else {		// Found @
		*beg++ = 0;	// Terminate user string
		}
		
	// Ok at this point have xxx:yyy@<something> or xxxx@<something>
	host = beg;

	// Scan for : or @ or end of string
	while ((beg <= end) && (*beg != ':') && (*beg != '@') && (*beg != '/')) //CCP fix bad telnets
		beg++;

	if (*beg == '@') { //xxx[:yyyy]@xxx@ is a no-no
		err = paramErr;
		goto badExit;
		}

	if (*beg != ':') { // End of string, we have xxxx[:yyyy]@zzzz
		*beg = 0;
		}
	else {
		*beg++ = 0;	// Terminate host string
		portstring = beg;
		}

goodUrl:
	TelInfo->gotDocument = 1;
/* NONO */
	if ( gApplicationPrefs->parseAliases ) {
		BlockMoveData(host, favorite+1, favorite[0] = strlen(host));
		if ( (theParams = NameToConnInitParams(favorite, false, "\p", &wasAlias)) != NULL ) {
			SessionPrefs *SessPtr = *((**theParams).session);
			if ( user != nil ) {
				BlockMoveData(user, (unsigned char *)SessPtr->username + 1, ((unsigned char *)SessPtr->username)[0] = strlen(user));
				if ( password != nil ) {
					BlockMoveData(password, (unsigned char *)SessPtr->password + 1, ((unsigned char *)SessPtr->password)[0] = strlen(password));
				}
			}
			if ( portstring != nil ) {
				BlockMoveData(portstring, favorite+1, favorite[0] = strlen(portstring));
				StringToNum(favorite, &port);
				SessPtr->port = port;
			}
		}
	}
	if ( theParams && CreateConnectionFromParams(theParams) ) {
		TelInfo->gotDocument = 1;
		err = noErr;
/* NONO */
	} else if ( OpenConnectionFromURL(host, portstring, user, password, ssh) ) {
		TelInfo->gotDocument = 1;
		err = noErr;
	}

badExit:
	return err;	
}

/*----------------------------------------------------------------------------
	ProcessURLEscapeCodes
	
	Process "%xx" escape codes in a URL string (replace them by the characters
	they represent).
	
	Entry:	url = URL with escape codes.
			
	Exit:	url = URL with escape codes replaced by the characters they
				represent.

	Copyright © 1994, Northwestern University.
	Modified 12/94 Jim Browne for NCSA
----------------------------------------------------------------------------*/

static void ProcessURLEscapeCodes (char *url, char **end)
{
	char *p, *q;
	char c1, c2;
	p = q = url;
	while (*p != 0) {
		if (*p == '%') {
			c1 = tolower(*(p+1));
			c2 = tolower(*(p+2));
			if (isxdigit(c1) && isxdigit(c2)) {
				c1 = isdigit(c1) ? c1 - '0' : c1 - 'a' + 10;
				c2 = isdigit(c2) ? c2 - '0' : c2 - 'a' + 10;
				*q++ = (c1 << 4) + c2;
				p += 3;
			} else {
				*q++ = *p++;
			}
		} else {
			*q++ = *p++;
		}
	}
	*q = 0;
	*end = q-1;
}

OSErr MyGotRequiredParams (AppleEvent *theAppleEvent)
{
	DescType	returnedType;
	Size		actualSize;
	OSErr		err;

	err = AEGetAttributePtr (theAppleEvent, keyMissedKeywordAttr,
									typeWildCard, &returnedType, nil, 0,
									&actualSize);
	if (err == errAEDescNotFound)	// you got all the required parameters
			return noErr;
	else if (!err)			// you missed a required parameter
			return errAEEventNotHandled;
	else					// the call to AEGetAttributePtr failed
			return err;
}
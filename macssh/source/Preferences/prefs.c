// prefs.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*	NCSA Revisions:
*	5/93	Telnet 2.6:	Rewritten entirely -- Jim Browne
*/


#include "prefs.proto.h"
#include "mainseg.proto.h"		// For quit proto
#include "errors.proto.h"
#include "DlogUtils.proto.h"

FTPServerPrefs*		gFTPServerPrefs=NULL;
ApplicationPrefs*	gApplicationPrefs=NULL;

ResType taggedSessTypes[] = { 'uMac' };
#define NUM_TAGGED_SESS_TYPES 1

void PREFSUnload(void) { }


OSErr OpenPreferencesFile( void)
{
	Str255	**PreferencesFileNameHdl;
	OSErr 	err;
	
	PreferencesFileNameHdl = (Str255**)GetString(PREFERENCES_FILE_NAME_STR_ID);
	if (PreferencesFileNameHdl == NULL) {
		DebugStr("\pSomeone has deleted the prefs file name resource!");
		forcequit();
		}
	
	HLock((Handle)PreferencesFileNameHdl);
		
	/* look for an existing settings file. */
	/* first, try the application folder */
	TelInfo->SettingsFile=HOpenResFile(TelInfo->ApFolder.vRefNum, TelInfo->ApFolder.parID,
											(StringPtr)*PreferencesFileNameHdl,fsRdWrShPerm);

	if (TelInfo->SettingsFile == -1) /* Look in System Folder */
		{
		TelInfo->SettingsFile = HOpenResFile(TelInfo->SysFolder.vRefNum, 
												TelInfo->SysFolder.parID, 
											(StringPtr)*PreferencesFileNameHdl, fsRdWrShPerm);
		}

	if (TelInfo->SettingsFile == -1) /* Look in Preferences Folder */
		{
		TelInfo->SettingsFile = HOpenResFile(TelInfo->PrefFolder.vRefNum, 
												TelInfo->PrefFolder.parID,
											(StringPtr)*PreferencesFileNameHdl, fsRdWrShPerm);
		}

	if (TelInfo->SettingsFile == -1)
	  	{
		if ((err = HCreate(TelInfo->PrefFolder.vRefNum, TelInfo->PrefFolder.parID,
						(StringPtr)*PreferencesFileNameHdl, kNCSACreatorSignature, 'pref')) != noErr)
			FatalAlert(CANT_CREATE_PREFS, 120, err);

		HCreateResFile(TelInfo->PrefFolder.vRefNum, TelInfo->PrefFolder.parID,
					(StringPtr)*PreferencesFileNameHdl);

		if (ResError() != noErr)
			FatalAlert(CANT_CREATE_PREFS, 120, ResError());

		TelInfo->SettingsFile = HOpenResFile(TelInfo->PrefFolder.vRefNum, 
												TelInfo->PrefFolder.parID,
											(StringPtr)*PreferencesFileNameHdl, fsRdWrShPerm);

		if (TelInfo->SettingsFile == -1)
			FatalAlert(CANT_CREATE_PREFS, 120, ResError());		// Give up
			
		if ((err = NewPreferences()) != noErr) return(err);
	  	}
		
	UseResFile(TelInfo->SettingsFile);
	ReleaseResource((Handle)PreferencesFileNameHdl);

	return	noErr;
}

OSErr NewPreferences(void)
{
	short				sessPrefsID;
	short				termPrefsID;
	ApplicationPrefs	**AppPrefsHdl;
	FTPServerPrefs		**FTPPrefsHdl;
	SessionPrefs		**sessPrefs;
	TerminalPrefs		**termPrefs;
		
	// Get the master copies from the application's resource fork
	
	if ( GetScriptManagerVariable(smRegionCode) == verJapan ) {
		sessPrefsID = SESSIONPREFS_APPID;
		termPrefsID = TERMINALPREFS_APPID;
	} else {
		sessPrefsID = SESSIONPREFS_APPID;
		termPrefsID = TERMINALPREFS_APPID;
	}

	AppPrefsHdl = (ApplicationPrefs **)GetResource(APPLICATIONPREFS_RESTYPE, APPLICATIONPREFS_APPID);
	if ((ResError() != noErr) || (AppPrefsHdl == NULL)) return(ResError());
	DetachResource((Handle)AppPrefsHdl);
	
	FTPPrefsHdl = (FTPServerPrefs **)GetResource(FTPSERVERPREFS_RESTYPE, FTPSERVERPREFS_APPID);
	if ((ResError() != noErr) || (FTPPrefsHdl == NULL)) return(ResError());
	DetachResource((Handle)FTPPrefsHdl);

	sessPrefs = (SessionPrefs **)GetResource(SESSIONPREFS_RESTYPE, sessPrefsID);
	if ((ResError() != noErr) || (sessPrefs == NULL)) return(ResError());
	DetachResource((Handle)sessPrefs);
	
	termPrefs = (TerminalPrefs **)GetResource(TERMINALPREFS_RESTYPE, termPrefsID);
	if ((ResError() != noErr) || (termPrefs == NULL)) return(ResError());
	DetachResource((Handle)termPrefs);

	// Add them to the Preferences file
	
	UseResFile(TelInfo->SettingsFile);
	if (ResError() != noErr) return(ResError());
	AddResource((Handle)AppPrefsHdl, APPLICATIONPREFS_RESTYPE, APPLICATIONPREFS_ID, "\p");
	if (ResError() != noErr) return(ResError());
	AddResource((Handle)FTPPrefsHdl,FTPSERVERPREFS_RESTYPE, FTPSERVERPREFS_ID, "\p");
	if (ResError() != noErr) return(ResError());
	AddResource((Handle)sessPrefs,SESSIONPREFS_RESTYPE, SESSIONPREFS_APPID, "\p<Default>");
	if (ResError() != noErr) return(ResError());
	AddResource((Handle)termPrefs,TERMINALPREFS_RESTYPE, TERMINALPREFS_APPID, "\p<Default>");
	if (ResError() != noErr) return(ResError());

	// Update the preferences file and release the resources
	
	UpdateResFile(TelInfo->SettingsFile);
	ReleaseResource((Handle)AppPrefsHdl);
	ReleaseResource((Handle)FTPPrefsHdl);
	ReleaseResource((Handle)sessPrefs);
	ReleaseResource((Handle)termPrefs);
	return(ResError());
}

OSErr LoadPreferences(void)
{
	ApplicationPrefs	**AppPrefsHdl;
	FTPServerPrefs		**FTPPrefsHdl;
	
	UseResFile(TelInfo->SettingsFile);
	if (ResError() != noErr) return(ResError());
	
	AppPrefsHdl = (ApplicationPrefs **)GetResource(APPLICATIONPREFS_RESTYPE, APPLICATIONPREFS_ID);
	if ((ResError() != noErr) || (AppPrefsHdl == NULL)) return(ResError());

	HLock((Handle) AppPrefsHdl);

	CheckPrefsHandleSize((Handle) AppPrefsHdl, sizeof(ApplicationPrefs));

	BlockMoveData(*AppPrefsHdl, gApplicationPrefs,
			  sizeof(ApplicationPrefs));
	ReleaseResource((Handle) AppPrefsHdl);
	
	FTPPrefsHdl = (FTPServerPrefs **)GetResource(FTPSERVERPREFS_RESTYPE, FTPSERVERPREFS_ID);
	if ((ResError() != noErr) || (FTPPrefsHdl == NULL)) return(ResError());

	HLock((Handle) FTPPrefsHdl);
	BlockMoveData(*FTPPrefsHdl, gFTPServerPrefs, sizeof(FTPServerPrefs));
	ReleaseResource((Handle) FTPPrefsHdl);
	
	if (TelInfo->haveColorQuickDraw)
	{
		UseResFile(TelInfo->SettingsFile);
		TelInfo->AnsiColors = GetNewPalette(10001);
		if (TelInfo->AnsiColors == NULL) //get the Application's copy instead
		{
			UseResFile(TelInfo->ApplicationFile);
			TelInfo->AnsiColors = GetNewPalette(9999);
			DetachResource((Handle)TelInfo->AnsiColors);
			HNoPurge((Handle)TelInfo->AnsiColors);
			if (TelInfo->AnsiColors != NULL) {
				UseResFile(TelInfo->SettingsFile);
				AddResource((Handle)TelInfo->AnsiColors, 'pltt', 10001, "\pANSI Colors");//make the new resource
				UpdateResFile(TelInfo->SettingsFile);
			}
		} else {
			DetachResource((Handle)TelInfo->AnsiColors);
			HNoPurge((Handle)TelInfo->AnsiColors);
		}
	}
	CheckPrefsVersion();
	return(noErr);
}

OSErr SaveAppPreferences(void)
{
	ApplicationPrefs	**AppPrefsHdl;
	Boolean				UserHasBeenAlerted = FALSE, UserResponse;
	
	UseResFile(TelInfo->SettingsFile);
	if (ResError() != noErr) return(ResError());
	
	AppPrefsHdl = (ApplicationPrefs **)GetResource(APPLICATIONPREFS_RESTYPE, APPLICATIONPREFS_ID);
	if ((ResError() != noErr) || (AppPrefsHdl == NULL)) return(ResError());

	HLock((Handle) AppPrefsHdl);

	CheckPrefsHandleSize((Handle) AppPrefsHdl, sizeof(ApplicationPrefs));

	if (gApplicationPrefs->version < (*AppPrefsHdl)->version) 
	{
		UserResponse = AskUserAlert(PREFS_ARE_NEWER_ID, TRUE);		// Cancel is default
		if (UserResponse == TRUE) return(noErr);	// User doesn't want to destroy prefs
		UserHasBeenAlerted = TRUE;					// Don't ask the user twice!
	}
		
	BlockMoveData(gApplicationPrefs, *AppPrefsHdl, sizeof(ApplicationPrefs));
	ChangedResource((Handle) AppPrefsHdl);
	UpdateResFile(TelInfo->SettingsFile);
	ReleaseResource((Handle) AppPrefsHdl);
	return(noErr);
}
OSErr SaveFTPPreferences(void)
{
	FTPServerPrefs		**FTPPrefsHdl;
	Boolean				UserHasBeenAlerted = FALSE, UserResponse;
	
	UseResFile(TelInfo->SettingsFile);
	if (ResError() != noErr) return(ResError());
	
	FTPPrefsHdl = (FTPServerPrefs **)GetResource(FTPSERVERPREFS_RESTYPE, FTPSERVERPREFS_ID);
	if ((ResError() != noErr) || (FTPPrefsHdl == NULL)) return(ResError());

	HLock((Handle) FTPPrefsHdl);
	if ((gFTPServerPrefs->version < (*FTPPrefsHdl)->version) && (!UserHasBeenAlerted)) {
		UserResponse = AskUserAlert(PREFS_ARE_NEWER_ID, TRUE);		// Cancel is default
		if (UserResponse == TRUE) return(noErr);	// User doesn't want to destroy prefs
		}
				
	BlockMoveData(gFTPServerPrefs, *FTPPrefsHdl, sizeof(FTPServerPrefs));
	ChangedResource((Handle) FTPPrefsHdl);

	UpdateResFile(TelInfo->SettingsFile);
	ReleaseResource((Handle) FTPPrefsHdl);
	return(noErr);
}

TerminalPrefs	**GetDefaultTerminal(void)
{
	TerminalPrefs	**theTerminalHdl;
	short			scratchshort;
	
	UseResFile(TelInfo->SettingsFile);
	theTerminalHdl = (TerminalPrefs **)Get1NamedSizedResource(TERMINALPREFS_RESTYPE, "\p<Default>", sizeof(TerminalPrefs));
	
	// If there is an error here, we put up a dialog box about the prefs
	// being messed up and then we restore the default from the master copy
	// in the application's resource fork.
	
	if (theTerminalHdl == NULL) {
		FatalCancelAlert(PREFERENCES_PROBLEM, "\pRepair", 110, ResError());
		UseResFile(TelInfo->ApplicationFile);
		theTerminalHdl = (TerminalPrefs **)Get1NamedSizedResource(TERMINALPREFS_RESTYPE, "\p<Default>", sizeof(TerminalPrefs));
		
		// If the master copy is not there, it's a fatal error!
		if (theTerminalHdl == NULL) {
			FatalAlert(RESOURCE_PROBLEM, 111, ResError());	// Doesn't return
			// ... but crashes if called from an appleevent handler
			return;
		}
		DetachResource((Handle)theTerminalHdl);	
		UseResFile(TelInfo->SettingsFile);
		scratchshort = Unique1ID(TERMINALPREFS_RESTYPE);
		AddResource((Handle)theTerminalHdl, TERMINALPREFS_RESTYPE, scratchshort, "\p<Default>");

		// If an error ocurred fixing the prefs file, it's a fatal error!
		if (ResError() != noErr) {
			FatalAlert(NUKED_PREFS, 112, ResError());	// Doesn't return
			// ... but crashes if called from an appleevent handler
			DisposeHandle((Handle)theTerminalHdl);
			return;
		}
		UpdateResFile(TelInfo->SettingsFile);
	}

	DetachResource((Handle)theTerminalHdl);
	return(theTerminalHdl);
}

SessionPrefs	**GetDefaultSession(void)
{
	SessionPrefs	**theSessionHdl;
	short			scratchshort;
	
	UseResFile(TelInfo->SettingsFile);
	theSessionHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, "\p<Default>", sizeof(SessionPrefs));

	// If there is an error here, we put up a dialog box about the prefs
	// being messed up and then we restore the default from the master copy
	// in the application's resource fork.
	
	if (theSessionHdl == NULL) {
		FatalCancelAlert(PREFERENCES_PROBLEM, "\pRepair", 100, ResError());
		UseResFile(TelInfo->ApplicationFile);
		theSessionHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, "\p<Default>", sizeof(SessionPrefs));
		
		// If the master copy is not there, it's a fatal error!
		if (theSessionHdl == NULL) {
			FatalAlert(RESOURCE_PROBLEM, 101, ResError());	// Doesn't return
			// ... but crashes if called from an appleevent handler
			return;
		}

		DetachResource((Handle)theSessionHdl);
		UseResFile(TelInfo->SettingsFile);
		scratchshort = Unique1ID(SESSIONPREFS_RESTYPE);
		AddResource((Handle)theSessionHdl, SESSIONPREFS_RESTYPE, scratchshort, "\p<Default>");

		// If an error ocurred fixing the prefs file, it's a fatal error!
		if (ResError() != noErr)  {
			FatalAlert(NUKED_PREFS, 102, ResError());	// Doesn't return
			// ... but crashes if called from an appleevent handler
			DisposeHandle((Handle)theSessionHdl);
			return;
		}
			
		UpdateResFile(TelInfo->SettingsFile);
	}
		
	DetachResource((Handle)theSessionHdl);
	return(theSessionHdl);
}

void	GetHostNameFromSession(StringPtr string)
{
	SessionPrefs	**sessHdl;
	
	UseResFile(TelInfo->SettingsFile);
	sessHdl = (SessionPrefs **)Get1NamedSizedResource(SESSIONPREFS_RESTYPE, string, sizeof(SessionPrefs));
	HLock((Handle)sessHdl);
	
	BlockMoveData((**sessHdl).hostname, string, StrLength((**sessHdl).hostname)+1);
	
	ReleaseResource((Handle)sessHdl);
}


Boolean			ProcessHostnameString(StringPtr	HostnameString, short *port, short *portNegative)
{
	Str255			MungeString;
	unsigned char	shortString[16];
	short			scratchshort;
	short			i;
	long			portRequested;
	StringPtr		xxxxptr, yyyyptr;
	Boolean			foundPort = FALSE;
	short			portNeg = 0;
	
	//	Copy the whole damn thing over
	BlockMoveData(HostnameString, MungeString, 255);
	
	//	Remove leading spaces
	scratchshort = 1;
	xxxxptr = NULL;
	do {

		while((scratchshort <= StrLength(MungeString)) && (MungeString[scratchshort] == ' '))
			scratchshort++;
		if (scratchshort > StrLength(MungeString)) {
			if ( xxxxptr == NULL ) {
				HostnameString[0] = 0;
				return(FALSE);
			}
			break;
		}
		if ( xxxxptr == NULL ) {
			xxxxptr = &MungeString[scratchshort-1];
		}
		//	Now look for a port number...
		while((scratchshort <= StrLength(MungeString)) && (MungeString[scratchshort] != ' '))
			scratchshort++;
		yyyyptr = &MungeString[scratchshort];
		if (scratchshort < StrLength(MungeString)) {
			if ( MungeString[scratchshort + 1] == '-' ) {
				scratchshort++;
				portNeg = 1;
			}
			shortString[0] = StrLength(MungeString) - scratchshort;
			BlockMoveData( &MungeString[scratchshort + 1], shortString + 1, shortString[0] );
			portRequested = 0;
			for (i = 1; i <= shortString[0]; i++)
				if ( shortString[i] < '0' || shortString[i] > '9' )
					break;
			if ( i > shortString[0] )
				StringToNum( shortString, &portRequested );
			if ( portRequested > 0 && portRequested < 65535 )
				foundPort = TRUE;
			else
				yyyyptr = &MungeString[scratchshort + 1];
		}
	} while (!foundPort && scratchshort < StrLength(MungeString));
	xxxxptr[0] = yyyyptr - xxxxptr - 1;
	//	Copy parsed hostname string back
	BlockMoveData(xxxxptr, HostnameString, StrLength(xxxxptr)+1);
	
	*port = (short)portRequested;
	*portNegative = portNeg;
	
	return foundPort;
}

void CheckPrefsVersion(void)
{
	if (gApplicationPrefs->version != OUR_PREFS_VERSION_NUMBER) //need to correctly set new values
	{
		if (gApplicationPrefs->version < NO_TIMEOUTS_VERSION)
		{
		 	gApplicationPrefs->SendTimeout = 15; //before d5, we need to set these
			gApplicationPrefs->OpenTimeout = 15;
		}
		gApplicationPrefs->StaggerWindowsOffset = 10;//before b1, we need to set this
		gApplicationPrefs->version = OUR_PREFS_VERSION_NUMBER;
		SaveAppPreferences();
	}
}

void CheckPrefsHandleSize(Handle h, long requestedSize)
{
	long oldsize, i;
	char *myPointer;

	oldsize = GetHandleSize(h);

	if (requestedSize > oldsize) {
		HUnlock(h);
		SetHandleSize(h, requestedSize);
		HLock(h);
		myPointer = (char *) *h;

		for (i = oldsize; i < requestedSize; i++)
			*(myPointer + i) = 0;
	}
}

Handle Get1NamedSizedResource(ResType theType, StringPtr theName, long requestedSize)
{
	Handle h;

	h = Get1NamedResource(theType, theName);
	CheckPrefsHandleSize(h, requestedSize);
	return h;
}

void RemoveTaggedResource(Handle resHandle, ResType theType, StringPtr theName)
{
	short i;
	Handle h;

	RemoveResource(resHandle);
	UseResFile(TelInfo->SettingsFile);

	switch (theType) {
		case 'SeSn':
		for (i = 0; i < NUM_TAGGED_SESS_TYPES; i++) {
			h = Get1NamedResource(taggedSessTypes[i], theName);
			if (h) RemoveResource(h);
		}
		break;
	}
}

void SetTaggedSesResInfo(Handle resHandle, short resID, StringPtr oldName, StringPtr newName)
{
	short i;
	short id;
	ResType rt;
	Handle h;
	static Str255 tmpname;

	SetResInfo(resHandle, resID, newName);

	UseResFile(TelInfo->SettingsFile);
	for (i = 0; i < NUM_TAGGED_SESS_TYPES; i++) {
		h = Get1NamedResource(taggedSessTypes[i], oldName);
		if (h) {
			GetResInfo(h, &id, &rt, tmpname);
			SetResInfo(h, id, newName);
			ReleaseResource(h);
		}
	}
}
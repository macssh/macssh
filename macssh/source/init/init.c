// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// init.c
// Top level of init code & general initialization routines
// Called only by main()

// Revisions:
// 7/92		Telnet 2.6:	Initial version.  Jim Browne


#include "init.proto.h"

#include "mainseg.proto.h"		/* For forcequit() proto */
#include "event.proto.h"
#include "iNetwork.proto.h"		/* For initnet proto */
#include "macros.proto.h"		/* For initmacro proto */
#include "wind.h"				/* For WindRec definition */
#include "rsinterf.proto.h"		/* For RSgetwindow & RSnewwindow protos */
#include "DlogUtils.proto.h"	/* For VersionNumber, UItemAssign, GetNewMyDialog protos */
#include "Sets.proto.h"			/* For readconfig proto */
#include "prefs.proto.h"
#include "vr.h"					/* For VRinit proto */
#include "vsdata.h"
#include "vsinterf.proto.h"		/* For VSinit proto */
#include "vgtek.proto.h"		/* For VGinit proto */
#include "vrrgmac.proto.h"		/* For MacRGinit proto */
#include "menuseg.proto.h"
#include "translate.proto.h"
#include "tnae.h"
#include "authencrypt.h"
#include "authencrypt.proto.h"
#include "ae.proto.h"			/* For AppleEvent handlers */
#include "MacBinary.h"
#include "binsubs.proto.h"		/* For init_mb_files proto */
#include "drag.proto.h"			/* For MyDragInit */
#include "IConfig.proto.h"
#include "errors.proto.h"
#include "telneterrors.h"
#include "wdefpatch.proto.h" //for wdefLoad
#include "memory.proto.h"
#include "AddressXLation.h"

#include <KeyChain.h>

#define _POSIX_SOURCE 1
#include <unistd.h>
#include <sys/stat.h>

extern void ssh2_init();
extern char *getprefsd(char *name, char * buf, size_t size, short *vRefNum, long *parID);

/*--------------------------------------------------------------------------------*/
#define CurrentVersion 1		/* Last known SysEnvirons version */
#define HFSPtr ((long *)1014)

//extern	char *tempspot;		 	/* for temp storage only */

Boolean gInitialized = 0;

extern	Cursor *theCursors[];

extern	WindRec
	*screens,		/* Window Records (VS) for :	Current Sessions */
	*ftplog;		/*								FTP Log			 */
extern	MenuHandle	myMenus[];

OSErr		io;
SysEnvRec	theWorld;						/* System Environment record */
short		TempItemsVRefNum;
long		TempItemsDirID;
Boolean		gKeyboardHasControlKey, gAEavail;
short **topLeftCorners;  //CCP 2.7: Better window positioning scheme


Boolean authOK;					// true if authentication driver available
Boolean encryptOK;				// true if des encryption routines available

/*	checkCONF - check if the user clicked on a configuration file */
void checkCONF( void)
{
	short	i,mess,count;
//	AppFile	theFile;
	FSSpec	FileSpec;
	long	junk = 0;

	if (gAEavail) return;	// If AppleEvents are available, we'll get ODOCs

// System 6 users: TOO BAD. The old file code has been removed as of 2.0b2.

	return;
}

void initEvents( void)
{
	EventRecord myEvent;

	FlushEvents(everyEvent - diskEvt,0);	/* Don't Let 'em get off without seeing it. */

	WaitNextEvent(everyEvent, &myEvent, gApplicationPrefs->TimeSlice, 0L);	/* WNE always avail */
	WaitNextEvent(everyEvent, &myEvent, gApplicationPrefs->TimeSlice, 0L);
}

/* initftplog - initialize the FTP log window */
void initftplog( void)
{
	Rect prect;
	TerminalPrefs **termHdl;
	Boolean scratchBoolean;
	Str255 tempString;
	unsigned long flags;
	
	SetRect(&prect, 300,256,512,384);

	ftplog = (WindRec *) myNewPtr(sizeof(WindRec));
	
	GetIndString(tempString,MISC_STRINGS,FTP_LOG_STRING);

	flags = RSWwrapon | RSWgoaway | RSWignoreBeeps | RSWoldscrollback;
	if (gFTPServerPrefs->ShowFTPlog)
		flags |= RSWshowit;

	ftplog->vs=RSnewwindow( &prect, 50, 80, 24, tempString, DefFONT, DefSIZE, 0,
							DefFONT, DefSIZE, 0, 0, flags);

	ftplog->wind = RSgetwindow( ftplog->vs);
	((WindowPeek)ftplog->wind)->windowKind = WIN_LOG;
	
	UseResFile(TelInfo->SettingsFile);
	termHdl = (TerminalPrefs **)Get1NamedSizedResource
				(TERMINALPREFS_RESTYPE, "\p<Default>", sizeof(TerminalPrefs));
	DetachResource((Handle) termHdl);
	HLock((Handle)termHdl);
	
	scratchBoolean = RSsetcolors( ftplog->vs, 0, &(*termHdl)->nfcolor);
	scratchBoolean = RSsetcolors( ftplog->vs, 1, &(*termHdl)->nbcolor);
	scratchBoolean = RSsetcolors( ftplog->vs, 2, &(*termHdl)->bfcolor);
	scratchBoolean = RSsetcolors( ftplog->vs, 3, &(*termHdl)->bbcolor);

	DisposeHandle((Handle)termHdl);
	VSwrite(ftplog->vs,"\033[24;0H",7);		/* Move to bottom of screen */
	ftplog->active=0; 						/* Meaningless */
	ftplog->port=0;							/* As is this */
	ftplog->termstate=VTEKTYPE;
}

  /* copies a specified cursor from a resource into a nonrelocatable block
	and returns a pointer to it in *result. */
void loadcursor( short CursorID, CursPtr *result)
{
	CursHandle tempcurs;
	OSErr Err;

	*result = nil;
	do /* once */
	  {
		tempcurs = GetCursor(CursorID);
		Err = ResError();
		if (Err != noErr)
			break;
		*result = (CursPtr) myNewPtr(sizeof(Cursor));		/* BYU LSC */
		Err = MemError();
		if (Err != noErr)
			break;
		LoadResource((Handle) tempcurs); /* in case it was purged */
		Err = ResError();
		if (Err != noErr)
			break;
		BlockMoveData((Ptr) *tempcurs, (Ptr) *result, sizeof(Cursor));
	  }
	while (false);
	if (Err != noErr)
	  {
		if (*result != nil)
			DisposePtr((Ptr) *result);
		*result = &qd.arrow;
	  }
} /* loadcursor */

/* initmcurs - Set up the mouse Cursors
 *		NOTE: debug tells if the bug or the pointer is to be used as dflt.*/
void initmcurs ( short debug) 
{
	loadcursor(rWatchCursor, &theCursors[watchcurs]);
	loadcursor(rCrossCursor, &theCursors[graphcurs]);
	loadcursor(rPOSCURS, &theCursors[poscurs]);
	loadcursor(rBeamCursor, &theCursors[textcurs]);
	loadcursor(rGINCURS, &theCursors[gincurs]);
	loadcursor(rXferCURS, &theCursors[xfercurs]);
	if (debug)
		loadcursor(rDbugCURS, &theCursors[normcurs]);
	else
		theCursors[normcurs] = &qd.arrow;
	setLastCursor(theCursors[normcurs]);
}

/* 	If a file with the filetype "DBUG" was sent to us, set the debug mode. */
short checkdebug( void)
{
	short i,mess,count;
//	AppFile theFile;
	long junk = 0;

	if (gAEavail) return(FALSE);	// 	Don't use CountAppFiles if AppleEvents
									//	are available.
// removed old Sys6 stuff (RAB 2.0b2)

	return (FALSE);

}

void InquireEnvironment( void)
{
	OSErr	err;
	Boolean HFSflag;
	long	response;

										// RJZ.  12/10/97.  Got rid of test for
										// HFS.  Checking absolute memory
										// locations is bad.

// Hey, we need System 7 anyway...
//	long *HFSp=(long *)1014L;
	
//	HFSflag= (*HFSp) > 0L;
//	if (!HFSflag) FatalAlert(NEED_HFS_ERR, 0, 0);

	err = SysEnvirons(CurrentVersion, &theWorld);
	
	if (err == envVersTooBig) FatalAlert(SYS_ENVIRON_ERR, 0, 0);

// RAB BetterTelnet - we need System 7 now. (or Mac OS 8)
// Get rid of the System 6 users right here. :)
	if (theWorld.systemVersion < 0x0700) FatalAlert(SYSTEM_VERS_ERR, 0, 0);
	
	if (theWorld.machineType < 0) FatalAlert(ROM_VERS_ERR, 0, 0);
	
	// If there is a problem w/Gestalt, assume our keyboard has a Control key.
	// Otherwise, we assume we have a control key unless a Mac or MacPlus keyboard is
	// present.
		
	if ((err = Gestalt(gestaltKeyboardType, &response)) != noErr)
		gKeyboardHasControlKey = TRUE;
	else
		gKeyboardHasControlKey = !(	(response == gestaltMacKbd) ||
									(response == gestaltMacAndPad) ||
									(response == gestaltMacPlusKbd) );
	{
		short virtualCode = 0;
		Ptr KCHRPtr;
		long newStuff;
		unsigned long state = 0;
		virtualCode = 0x10;//this will be a 'y' for Querty
		KCHRPtr  = (Ptr)GetScriptManagerVariable(smKCHRCache);
		newStuff = KeyTranslate(KCHRPtr,virtualCode,&state);
		newStuff &= 0xFF; //only look at bottom byte
		TelInfo->isQuerty = ((unsigned char) newStuff == 'y');
	}

	TelInfo->haveColorQuickDraw = theWorld.hasColorQD;
#if GENERATINGCFM
	TelInfo->haveKeyChain = KeychainManagerAvailable();
#else
	TelInfo->haveKeyChain = false;
#endif
}

#define	kURLEventClass	'GURL'
#define	kGetURLEvent	'GURL'

void initmac( void)
{
	UnsignedWide usec;
	EventRecord myEvent;
	short 	i;
	long	gestaltvalue;
	OSErr	err;
	Handle	tempHandle;

	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);				/* No resume proc */
	InitCursor();

	Microseconds(&usec);
	qd.randSeed = usec.lo;

	TelInfo = (TelInfoRec *)myNewPtr(sizeof(TelInfoRec));

	tempHandle = myNewHandle(sizeof(ApplicationPrefs));
	HLockHi(tempHandle);
	gApplicationPrefs = (ApplicationPrefs *) *tempHandle;
	
	tempHandle = myNewHandle(sizeof(FTPServerPrefs));
	HLockHi(tempHandle);
	gFTPServerPrefs = (FTPServerPrefs *)*tempHandle;
	
	screens = (WindRec *)myNewPtr(MaxSess*sizeof(WindRec));
	for (i = 0; i < MaxSess; i++)
		screens[i].active = CNXN_NOTINUSE;
		
	topLeftCorners = (short **) myNewHandle(MaxSess*sizeof(short));

	InquireEnvironment();

	for (i=1;i<12;i++)
		EventAvail( 0xffff, &myEvent);
	
	err = Gestalt(gestaltAppleEventsAttr, &gestaltvalue);		// See if AppleEvents are available
	gAEavail = (!err && ((gestaltvalue >> gestaltAppleEventsPresent) & 0x0001));
	

	TelInfo->screenRect = qd.screenBits.bounds;			/* well, they have to be set */
												/* somewhere, where else ? */
	TelInfo->greyRegion = GetGrayRgn();
	SetRect(&TelInfo->dragRect, 4, 24, TelInfo->screenRect.right-4,
						TelInfo->screenRect.bottom-4);
}

void InitAEHandlers(void)
{
	OSErr		err;

	if (gAEavail) {
		if ((err = AEInstallEventHandler(kCoreEventClass,kAEOpenApplication,
											MyHandleOAppUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kCoreEventClass,kAEOpenDocuments,
											MyHandleODocUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kCoreEventClass,kAEPrintDocuments,
											MyHandlePDocUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,
											MyHandleQuitUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kURLEventClass,kGetURLEvent,
											MyHandleGURLUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'send',MyHandleSendDataUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'seCR',MyHandleSendCRUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'mkCX',MyHandleConnectUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'WaiT',MyHandleWaitUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'ReaD',MyHandleReadUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'susp',MyHandleSuspendUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		if ((err = AEInstallEventHandler(kNCSACreatorSignature,'!sus',MyHandleUnSuspendUPP,0,FALSE)) != noErr)
			FatalAlert(AE_PROBLEM_ERR, 0, 0);
		}
}

void DoTheGlobalInits(void)
{
	long junk = 0;
	ParamBlockRec pb;
	Str255 tempString;
	char folderString[256];
	int err;
	char *buf;
	
	TelInfo->ScrlLock=0;
	TelInfo->ftplogon=0;
	TelInfo->done=0;
	TelInfo->MacBinary=0;
	TelInfo->numwindows=0;
	TelInfo->CONFstate=0;
	TelInfo->CONFactive=0;
	TelInfo->SettingsFile=-1;
	TelInfo->ApplicationFile = CurResFile();
	TelInfo->ginon   = 0;
	TelInfo->xferon  = 0;
	TelInfo->graphs = 0;
	TelInfo->debug = 1;
	TelInfo->myfronttype = NO_WINDOW;
	TelInfo->myfrontvs = 0;
	TelInfo->suspended = FALSE;
	TelInfo->myfrontwindow = 0L;	
	TelInfo->myfrontRgn = 0L;	
	TelInfo->lastCursor = 0L;
	TelInfo->DefaultDirVRefNum = -1;
	TelInfo->FTPClientVRefNum = -1;
	TelInfo->DefaultDirDirID = 2;
	TelInfo->FTPClientDirID = 2;
	TelInfo->macrosModeless = 0;
	TelInfo->ipModeless = 0;
	TelInfo->otpword[0] = 0;
	TelInfo->lastPanelSession = 0;
	TelInfo->lastPanelTerminal = 0;
	TelInfo->checkedBoldFontNum = 0;
	TelInfo->checkedFontNum = 0;
	
	pb.ioParam.ioNamePtr = 0L;
	PBGetVolSync(&pb);			/* see Tech Note 140 for implications of this */

	GetWDInfo(pb.ioParam.ioVRefNum, &(TelInfo->ApFolder.vRefNum), &(TelInfo->ApFolder.parID), &junk);
	TelInfo->ApFolder.name[0] = 0;

	FindFolder( kOnSystemDisk, kSystemFolderType, kCreateFolder,
					&(TelInfo->SysFolder.vRefNum), &(TelInfo->SysFolder.parID));
	TelInfo->SysFolder.name[0]=0;

/* NONO */
	GetIndString((unsigned char *)folderString,MISC_STRINGS,PREFS_FOLDER_NAME);
	junk = folderString[0];
	BlockMoveData(folderString + 1, folderString, junk);
	folderString[junk] = 0;
	// WARNING: this initializes GUSI which in turn calls our event handler...
	buf = getprefsd(folderString, (char *)tempString, sizeof(tempString), &TelInfo->PrefFolder.vRefNum, &TelInfo->PrefFolder.parID);
	if ( !buf ) {
		FindFolder( kOnSystemDisk, kPreferencesFolderType, kCreateFolder,
						&(TelInfo->PrefFolder.vRefNum), &(TelInfo->PrefFolder.parID));
  	}
	TelInfo->PrefFolder.name[0]=0;
/* NONO */

	
	FindFolder( kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &TempItemsVRefNum, &TempItemsDirID);
}

void init(void)
{
	DialogPtr 	dtemp;		/* Used for dialog display */
	Size 		junk;
	long		junk2;

	FlushEvents(everyEvent, 0);

/* NONO */	
	/*initMemoryBuffer(5*1024,30*1024);*/ //this gives use a grow zone for emergency situations
	initMemoryBuffer(5*1024, 128*1024); //this gives use a grow zone for emergency situations
/* NONO */

	initmac();				/* initialize Macintosh stuff */
	
	InitAEHandlers();

	DoTheGlobalInits();

	OpenPreferencesFile();
	LoadPreferences();
	UnloadSeg(&LoadPreferences);

	
	dtemp = GetNewMySmallDialog(FirstDLOG, NULL, kInFront, (void *)ThirdCenterDialog);	/* opening dialog */
	UItemAssign( dtemp, 2, VersionNumberUPP);
	ShowWindow(dtemp);
	DrawDialog(dtemp);										/* while we init everything */
	scriptKbdInit(); 
	TelInfo->debug=checkdebug();	/* must return TRUE or FALSE */

	initmcurs(0);		/* init the mouse cursor */
	setLastCursor(theCursors[watchcurs]);
	setupmenu(gApplicationPrefs->CommandKeys, dtemp);
	
	trInit(myMenus[National]);	/* LU: Initialize translation routines; must be done */
								/* LU: prior to checkdebug(), since even the debug 	 */
								/* LU: output goes through the translation routines. */
	
		
	if (VSinit(MaxSess+2)) {		/* initialize the VS (which also inits RS stuff) */
		DoError(104 | MEMORY_ERRORCLASS, LEVEL3, NULL);
		forcequit();
	}
	//UnloadSeg(&VSinit);
	//MaxMem(&junk2);

	InitDebug();
//	ShowDebugWindow();
	initftplog();					/* allocate/initialize ftp log screen */

	VGinit();						/* initialize Virtual Graphics after VS so that */
									/* init messages can come on console screen */
	VRinit();						/* ICR init */
	MacRGinit();					/* ditto */
	init_mb_files();				/* Initialize file id's to closed state */

	switchMenus(gApplicationPrefs->CommandKeys, 0);

	TelInfo->done = FALSE;			// We're not quitting yet.

	setLastCursor(theCursors[normcurs]);

	initmacros();
	CheckGlobalMacros();

	ICinit();
	UnloadSegments(); //get all of them at once
	MaxMem(&junk); 	  //so DNR gets locked high in memory
	TelInfo->startedTCP = 0;
/*	io = OpenResolver(NULL);

	if (io) {								
		DialogPtr theErrDialog;
		short itemhit = 0;

		theErrDialog = GetNewDialog(RESOLVERERROR, NULL, kInFront);
		DrawDialog(theErrDialog);
		while (!itemhit)
			ModalDialog(DLOGwOKUPP,&itemhit);
		DisposeDialog(theErrDialog);
		} */

	initnet();						/* initialize network stuff */

	updateMenuChecks();
	DoTheMenuChecks();

	MyDragInit();
	
	checkCONF();	 				/* Did user click on a set? */

	DisposeDialog(dtemp);			/* Remove the splashbox...  */
	AdjustMenus(); // and adjust the menus again - RAB BetterTelnet 1.2

   	UnloadSeg(&init_mb_files);//this allows the authencrypt stuff to be locked above the FTP code segment
	UnloadSeg(&MyDragInit);
	MaxMem(&junk2); 

    encryptOK = authOK = true;
    if (!auth_encrypt_init()) {
	    encryptOK = false;
	    authOK = false;
    }
	loadWDEF(); //this just loads the WDEF code in so that it doesnt fragment the heap later
	loadErrors(); //ditto for the error code

	ssh2_init();

	gInitialized = true;
}

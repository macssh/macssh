/*  Metrowerks Standard Library  */

/*  $Date$ 
 *  $Revision$ + Carbon Changes
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*****************************************************************************/
/*  Project...: C++ and ANSI-C Compiler Environment                          */
/*  Name......: SIOUXMenus.c                                       		     */
/*  Purpose...: Menu related functions for SIOUX			                 */
/*****************************************************************************/

				  
 #include <SIOUX.h>  

#include "SIOUXGlobals.h"
#include "SIOUXMenus.h"
#include "SIOUXPrinter.h"
#include "SIOUXWindows.h"

#include "PasswordDialog.h"	/* just for GetScrapHandle() */

#if SIOUX_USE_WASTE
	#include <limits.h>
#endif

/*#include <unix.h>*/
extern long _fcreator, _ftype;

#include <Devices.h>
#include <LowMem.h>
#include <Scrap.h> /*bkoz*/
#include <StandardFile.h>
#include <ToolUtils.h>

#if TARGET_API_MAC_CARBON
	#include <AEDataModel.h>	/* ra 990612 */
	#include <Gestalt.h>
	#include <MacErrors.h>
	#include <Navigation.h>	/* ra 990612 */
#endif /* TARGET_API_MAC_CARBON */

#pragma bool on
#ifndef TRUE	/* ra 990611 Carbon */
#define TRUE true
#endif

/*	Menuhandles ...*/
static MenuHandle appleMenu;
static MenuHandle fileMenu;
static MenuHandle editMenu;

extern Boolean gSIOUXTerminalMode;
extern char gSIOUXChars[];
extern short gSIOUXBufSize;
extern const short gSIOUXMaxSize;

#if ! SIOUX_USE_WASTE

/************************************************************************/
/*	Purpose..: 	Replacement for mac TE glue								*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static OSErr MyTEFromScrap(void)
{
	long length; /* cc 991111 */
#if TARGET_API_MAC_CARBON  /* cc 991111 */
	ScrapRef theScrapRef;
#else
	long scrapOffset;  /* cc 991111 */
#endif

#if TARGET_API_MAC_CARBON
	GetCurrentScrap(&theScrapRef);
	GetScrapFlavorSize(theScrapRef, 'TEXT', &length);
#else
	if ((length = GetScrap(0L,'TEXT',&scrapOffset))<0)
		return (length);
#endif   /* cc 991111 */

	if (length>32000)
		return (teScrapSizeErr);

#if TARGET_API_MAC_CARBON  /* cc 991111 */
	HUnlock(TEScrapHandle());
	SetHandleSize(TEScrapHandle(), length);
	HLock(TEScrapHandle());
	GetScrapFlavorData(theScrapRef, 'TEXT', &length, *TEScrapHandle());
#else  /* cc 991111 */
	HLock(TEScrapHandle());
	if ((length = GetScrap(TEScrapHandle(),'TEXT',&scrapOffset))<0) {
		HUnlock(TEScrapHandle());
		return (length);
	}
#endif
	
	HUnlock(TEScrapHandle());  /* cc 991111 */
	TESetScrapLength(length);
	
	return (noErr);
}

static OSErr MyTEToScrap(void)
{
	long	error;
#if TARGET_API_MAC_CARBON  /*cc 991111 */
	ScrapRef theScrapRef;
#endif

	HLock(TEScrapHandle());
	
#if TARGET_API_MAC_CARBON
	error = ClearCurrentScrap();
	error = GetCurrentScrap(&theScrapRef);
	error = PutScrapFlavor(theScrapRef, 'TEXT', kScrapFlavorMaskNone, TEGetScrapLength(),
		*TEScrapHandle());
#else
	ZeroScrap();
	error = PutScrap(LMGetTEScrpLength(), 'TEXT', *TEScrapHandle());
#endif /* TARGET_API_MAC_CARBON */    /*cc 991111 */
	
	HUnlock(TEScrapHandle());

	return error;
}
#endif /* ! SIOUX_USE_WASTE */

/****************************************************************/
/* Purpose..: Setup SIOUX's menus  								*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXSetupMenus(void)
{
#if TARGET_API_MAC_CARBON
	long result;
#endif /* TARGET_API_MAC_CARBON */
	unsigned char appleString[2] = "\001\024";

	/*	Create the Apple menu ...*/
	appleMenu = NewMenu(APPLEID, appleString);
#if SIOUX_USE_WASTE
	AppendMenu( appleMenu, "\pAbout SIOUX-WASTE;(-");
#else
	AppendMenu(appleMenu, "\pAbout SIOUX;(-");
#endif /* SIOUX_USE_WASTE */
	AppendResMenu(appleMenu, 'DRVR');
	InsertMenu(appleMenu, 0);

	/*	Create the File menu ...*/
	fileMenu = NewMenu(FILEID, "\pFile");
#if TARGET_API_MAC_CARBON
	Gestalt(gestaltMenuMgrAttr, &result);
	if ((result & gestaltMenuMgrAquaLayoutMask) == gestaltMenuMgrAquaLayoutMask)
		/* JWW - On OS X, the Quit item is automaticaly in the application menu, not in File */
		AppendMenu(fileMenu, "\p(New...;(Open...;(Close...;Save/S;(-;Page Setup;Print.../P");
	else
		/* JWW - Be careful if you modify this area -- the else clause is after the #endif */
#endif /* TARGET_API_MAC_CARBON */
	AppendMenu(fileMenu, "\p(New...;(Open...;(Close...;Save/S;(-;Page Setup;Print.../P;(-;Quit/Q");
	InsertMenu(fileMenu, 0);

	/*	Create the Edit menu ...*/
	editMenu = NewMenu(EDITID, "\pEdit");
	AppendMenu(editMenu, "\p(Undo/Z;(-;(Cut/X;(Copy/C;Paste/V;(Clear;(-;Select All/A");
	InsertMenu(editMenu, 0);

	InvalMenuBar();	/* Instead of DrawMenuBar to allow Carbon to fill in the Apple menu */
}

/****************************************************************/
/* Purpose..: Setup SIOUX's menus  								*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXUpdateMenuItems(void)
{
#if SIOUX_USE_WASTE
	SInt32 weSelStart, weSelEnd;
#endif	
	/*	First disable all possible items ...*/

#if TARGET_API_MAC_CARBON
	DisableMenuItem(fileMenu, FILESAVE);
	DisableMenuItem(fileMenu, FILEPRINT);
	DisableMenuItem(editMenu, EDITCUT);
	DisableMenuItem(editMenu, EDITCOPY);
	DisableMenuItem(editMenu, EDITPASTE);
	DisableMenuItem(editMenu, EDITCLEAR);
	DisableMenuItem(editMenu, EDITSELECTALL);
#else
	DisableItem(fileMenu, FILESAVE);
	DisableItem(fileMenu, FILEPRINT);
	DisableItem(editMenu, EDITCUT);
	DisableItem(editMenu, EDITCOPY);
	DisableItem(editMenu, EDITPASTE);
	DisableItem(editMenu, EDITCLEAR);
	DisableItem(editMenu, EDITSELECTALL);
#endif /* TARGET_API_MAC_CARBON */

	if (SIOUXState != PRINTFING && SIOUXIsAppWindow(FrontWindow())) {
	#if TARGET_API_MAC_CARBON
		EnableMenuItem(fileMenu, FILEPRINT);
	#else
		EnableItem(fileMenu, FILEPRINT);
	#endif /* TARGET_API_MAC_CARBON */
#if SIOUX_USE_WASTE
		if ( WECanPaste( SIOUXTextWindow->edit ) ) {
#endif
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITPASTE);
		#else
			EnableItem(editMenu, EDITPASTE);
		#endif
#if SIOUX_USE_WASTE
		}
#endif
	#if TARGET_API_MAC_CARBON
		EnableMenuItem(editMenu, EDITSELECTALL);
	#else
		EnableItem(editMenu, EDITSELECTALL);
	#endif /* TARGET_API_MAC_CARBON */
		
#if SIOUX_USE_WASTE
		WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
		
		if ( weSelStart != weSelEnd ) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITCUT);
		#else
			EnableItem(editMenu, EDITCUT);
		#endif /* TARGET_API_MAC_CARBON */
		}
		if ( weSelStart != weSelEnd ) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITCOPY);
		#else
			EnableItem(editMenu, EDITCOPY);
		#endif /* TARGET_API_MAC_CARBON */
		}
		if ( weSelStart != weSelEnd ) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITCLEAR);
		#else
			EnableItem(editMenu, EDITCLEAR);
		#endif /* TARGET_API_MAC_CARBON */
		}
#else
		if ((*SIOUXTextWindow->edit)->selStart != (*SIOUXTextWindow->edit)->selEnd) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITCUT);
		#else
			EnableItem(editMenu, EDITCUT);
		#endif /* TARGET_API_MAC_CARBON */
		}
		if ((*SIOUXTextWindow->edit)->selStart != (*SIOUXTextWindow->edit)->selEnd) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITCOPY);
		#else
			EnableItem(editMenu, EDITCOPY);
		#endif /* TARGET_API_MAC_CARBON */
		}
		if ((*SIOUXTextWindow->edit)->selStart != (*SIOUXTextWindow->edit)->selEnd) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(editMenu, EDITCLEAR);
		#else
			EnableItem(editMenu, EDITCLEAR);
		#endif /* TARGET_API_MAC_CARBON */
		}
#endif /* SIOUX_USE_WASTE */
		
		if (SIOUXTextWindow->dirty) {
		#if TARGET_API_MAC_CARBON
			EnableMenuItem(fileMenu, FILESAVE);
		#else
			EnableItem(fileMenu, FILESAVE);
		#endif /* TARGET_API_MAC_CARBON */
		}
	}
}


/****************************************************************/
/* Purpose..: Setup SIOUX's menus  								*/
/* Input....: ---                       						*/
/* Returns..: noErr text saved correctly / -1 error or cancel	*/
/****************************************************************/

#if TARGET_API_MAC_CARBON
/* Prototype for routine in SIOUXWindows.c */
void SIOUXUpdateWindow(WindowPtr theWindow);

static pascal void NavEventProc2(NavEventCallbackMessage inSelector, NavCBRecPtr ioParams,
							NavCallBackUserData ioUserData)  /*  cc  991111 */
{
#pragma unused(ioParams, ioUserData)

	if (inSelector == kNavCBEvent) {
		SIOUXUpdateWindow(SIOUXTextWindow->window);
	}
}
#endif /* TARGET_API_MAC_CARBON */

short SIOUXDoSaveText(void)
{
	short error;
	HParamBlockRec pb;
	StandardFileReply sfReply;	/* ra 990612 Change to support NavServ. */
	
#if SIOUX_USE_WASTE
	WEReference theTE = SIOUXTextWindow->edit;
	long textLength = WEGetTextLength( theTE );
	Handle textHandle = WEGetText( theTE );
#else
	TEHandle theTE = SIOUXTextWindow->edit;
	long textLength = (long)(*theTE)->teLength;
#endif /* SIOUX_USE_WASTE */
	
	Point aPoint = {100, 100};

#if SIOUX_USE_WASTE
	HLock( textHandle );
#else
	HLock((*theTE)->hText);
#endif /* SIOUX_USE_WASTE */

	if (SIOUXTextWindow->dirid) {
	/*	Open the file ...*/
		pb.ioParam.ioCompletion = 0L;
		pb.ioParam.ioNamePtr = SIOUXTextWindow->fname;
		pb.ioParam.ioVRefNum = SIOUXTextWindow->vrefnum;
		pb.ioParam.ioPermssn = fsWrPerm;
		pb.fileParam.ioDirID = SIOUXTextWindow->dirid;
		if (PBHOpenDFSync(&pb) != noErr)
			/*	Couldn't open the file so we try resaving it ...*/
			goto RenameFile;
	/*	Write the text ...*/
#if SIOUX_USE_WASTE
		pb.ioParam.ioBuffer = *textHandle;
#else
		pb.ioParam.ioBuffer = *(*theTE)->hText;
#endif /* SIOUX_USE_WASTE */
		pb.ioParam.ioReqCount = textLength;
		pb.ioParam.ioPosMode = fsFromStart;
		pb.ioParam.ioPosOffset = 0;
		if (PBWriteSync((ParmBlkPtr)&pb) != noErr)
			goto SaveError;
#if SIOUX_USE_WASTE
		if (pb.ioParam.ioActCount != (long)WEGetTextLength( theTE ) )
#else
		if (pb.ioParam.ioActCount != (long)(*theTE)->teLength)
#endif /* SIOUX_USE_WASTE */
			goto SaveError;
		pb.ioParam.ioMisc = (Ptr)pb.ioParam.ioActCount;
		if (PBSetEOFSync((ParmBlkPtr)&pb) != noErr)
			goto SaveError;
	/*	Close file ...*/
		if (PBCloseSync((ParmBlkPtr)&pb) != noErr)
			goto SaveError;
		goto Saved;
	} else {
		Str255 theTitle;

RenameFile:
		GetWTitle(SIOUXTextWindow->window, theTitle);
		
		sfReply.sfGood = false;
			
	#if TARGET_API_MAC_CARBON
		{
			NavDialogOptions NavOptions;
			NavReplyRecord NavReply;
			AEDesc	specDesc;
			OSErr	err;

			NavEventUPP myEventUPP = NewNavEventUPP(NavEventProc2);

			err = NavLoad();
			if (err != noErr)
				goto NotSaved;
			
			NavGetDefaultDialogOptions(&NavOptions);
			BlockMoveData(theTitle, NavOptions.savedFileName, sizeof(sfReply.sfFile.name));
			
			err = NavPutFile(NULL, &NavReply, &NavOptions, myEventUPP, 'TEXT', 'CWIE', NULL);
			if (err != noErr)
				goto NotSaved;

			AECoerceDesc(&NavReply.selection, typeFSS, &specDesc);
		
			DisposeNavEventUPP(myEventUPP);
			
			sfReply.sfGood = NavReply.validRecord;
			if (sfReply.sfGood) {
				NavTranslationOptions Options = kNavTranslateInPlace;
				sfReply.sfFile = **(FSSpec**) specDesc.dataHandle;
				NavCompleteSave(&NavReply, Options);				
			}
			NavDisposeReply(&NavReply);
			NavUnload();
			AEDisposeDesc(&specDesc);
			
			if (err == userCanceledErr)
				goto NotSaved;
		}
	#else
		StandardPutFile("\pSave document as...", theTitle, &sfReply);	/* ra 990612 UI 3.2 */
		if (!sfReply.sfGood)
			goto NotSaved;
	#endif /* TARGET_API_MAC_CARBON */
				
	/*	Make the FSSpec ...*/
		SIOUXTextWindow->vrefnum = sfReply.sfFile.vRefNum;
	#if TARGET_API_MAC_CARBON
		SIOUXTextWindow->dirid = sfReply.sfFile.parID;
	#else
		SIOUXTextWindow->dirid = LMGetCurDirStore();	/* jd- why is this even being done??? */
	#endif /* TARGET_API_MAC_CARBON */
		BlockMoveData(sfReply.sfFile.name, SIOUXTextWindow->fname, sfReply.sfFile.name[0] + 1);

		pb.ioParam.ioCompletion = 0L;
		pb.ioParam.ioNamePtr = SIOUXTextWindow->fname;
		pb.ioParam.ioVRefNum = SIOUXTextWindow->vrefnum;
		pb.fileParam.ioFlVersNum = 0;
		pb.fileParam.ioDirID = SIOUXTextWindow->dirid;
		error = PBHCreateSync(&pb);
		if (error != noErr && error != dupFNErr)
			goto SaveError;
	/*	Set the finder info ...*/
		GetDateTime(&pb.fileParam.ioFlCrDat);  /*  cc 991111 */
		GetDateTime(&pb.fileParam.ioFlMdDat);  /*  cc 991111 */
		if ( _ftype != '\0' )
			pb.fileParam.ioFlFndrInfo.fdType = _ftype;
		else
			pb.fileParam.ioFlFndrInfo.fdType = 'TEXT';
		
		if ( _fcreator != '\0' )
			pb.fileParam.ioFlFndrInfo.fdCreator = _fcreator;
		else
			pb.fileParam.ioFlFndrInfo.fdCreator = 'CWIE';
		pb.fileParam.ioFlFndrInfo.fdFlags = 0;
		if (PBHSetFInfoSync(&pb) != noErr)
			goto SaveError;
	/*	Open the file ...*/
		pb.ioParam.ioPermssn = fsWrPerm;
		if (PBHOpenDFSync(&pb) != noErr)
			goto SaveError;
	/*	Write the text ...*/
#if SIOUX_USE_WASTE
		pb.ioParam.ioBuffer = *textHandle;
#else
		pb.ioParam.ioBuffer = *(*theTE)->hText;
#endif /* SIOUX_USE_WASTE */
		pb.ioParam.ioReqCount = textLength;
		pb.ioParam.ioPosMode = fsFromStart;
		pb.ioParam.ioPosOffset = 0;
		if (PBWriteSync((ParmBlkPtr)&pb) != noErr)
			goto SaveError;
#if SIOUX_USE_WASTE
		if (pb.ioParam.ioActCount != (long)WEGetTextLength( theTE ) )
#else
		if (pb.ioParam.ioActCount != (long)(*theTE)->teLength)
#endif /* SIOUX_USE_WASTE */
			goto SaveError;
		pb.ioParam.ioMisc = (Ptr)pb.ioParam.ioActCount;
		if (PBSetEOFSync((ParmBlkPtr)&pb) != noErr)
			goto SaveError;
	/*	Close file ...*/
		if (PBCloseSync((ParmBlkPtr)&pb) != noErr)
			goto SaveError;
	/*	Rename the window ...*/
		SetWTitle(SIOUXTextWindow->window, SIOUXTextWindow->fname);
		
		goto Saved;
	}
SaveError:
#if SIOUX_USE_WASTE
	HUnlock( textHandle );
#else
	HUnlock((*theTE)->hText);
#endif /* SIOUX_USE_WASTE */

#if TARGET_API_MAC_CARBON
	{
		Cursor theArrow;
		SetCursor(GetQDGlobalsArrow(&theArrow));
	}
#else
	SetCursor(&qd.arrow);
#endif /* TARGET_API_MAC_CARBON */
	SIOUXCantSaveAlert(SIOUXTextWindow->fname);
NotSaved:
	return (-1);
Saved:
#if SIOUX_USE_WASTE
	HUnlock( textHandle );
#else
	HUnlock((*theTE)->hText);
#endif /* SIOUX_USE_WASTE */
	SIOUXTextWindow->dirty = 0;
	return (noErr);
}

/****************************************************************/
/* Purpose..: Handle the cut menu command						*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXDoEditCut(void)
{
#if SIOUX_USE_WASTE
	SInt32 weSelStart, weSelEnd;
	OSErr err;
	
	WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
	if (SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit) && weSelStart != weSelEnd) {
		err = WECut(SIOUXTextWindow->edit);
		SIOUXTextWindow->dirty = TRUE;
		SIOUXUpdateScrollbar();
	}
#else
	if (SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit) && (*SIOUXTextWindow->edit)->selStart != (*SIOUXTextWindow->edit)->selEnd) {
		TECut(SIOUXTextWindow->edit);
		MyTEToScrap();
		SIOUXTextWindow->dirty = TRUE;
		SIOUXUpdateScrollbar();
	}
#endif /* SIOUX_USE_WASTE */
}

/****************************************************************/
/* Purpose..: Handle the copy menu command						*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXDoEditCopy(void)
{
#if SIOUX_USE_WASTE
	SInt32 weSelStart, weSelEnd;
	OSErr err;
	
	WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
	if (weSelStart != weSelEnd) {
		err = WECopy(SIOUXTextWindow->edit);
	}
#else
	if ((*SIOUXTextWindow->edit)->selStart != (*SIOUXTextWindow->edit)->selEnd) 
	{
		TECopy(SIOUXTextWindow->edit);
		MyTEToScrap();                       /* mm 970428 */
	}
#endif /* SIOUX_USE_WASTE */
}

/****************************************************************/
/* Purpose..: Handle the paste menu command						*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXDoEditPaste(void)
{
#if SIOUX_USE_WASTE
	OSErr err;
	
	if (SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit) && WECanPaste( SIOUXTextWindow->edit ) ) {
		if (!gSIOUXTerminalMode) {
			err = WEPaste(SIOUXTextWindow->edit);
			SIOUXTextWindow->dirty = 1;
			SIOUXUpdateScrollbar();
		} else {
			Handle hText = nil ;
			OSErr err;
			// look for a text flavor
			if ( ( err = GetScrapHandle ( 'TEXT', &hText ) ) == noErr ) {
				SInt32 size = GetHandleSize ( hText );
				if (size > gSIOUXMaxSize - gSIOUXBufSize) {
					size = gSIOUXMaxSize - gSIOUXBufSize;
				}
				BlockMoveData(* hText, gSIOUXChars + gSIOUXBufSize, size);
				gSIOUXBufSize += size;
				SIOUXState = IDLE;
				DisposeHandle( hText );
			}
		}
	}
#else
	if (SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit) && MyTEFromScrap() == noErr) {
		TEPaste(SIOUXTextWindow->edit);
		SIOUXTextWindow->dirty = 1;
		SIOUXUpdateScrollbar();
	}
#endif /* SIOUX_USE_WASTE */
}


/****************************************************************/
/* Purpose..: Handle the clear menu command						*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXDoEditClear(void)
{
#if SIOUX_USE_WASTE
	SInt32 weSelStart, weSelEnd;
	
	WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
	if (SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit) && weSelStart != weSelEnd) {
		WEDelete(SIOUXTextWindow->edit);
		SIOUXTextWindow->dirty = TRUE;
		SIOUXUpdateScrollbar();
	}
#else
	if (SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit) && (*SIOUXTextWindow->edit)->selStart != (*SIOUXTextWindow->edit)->selEnd) {
		TEDelete(SIOUXTextWindow->edit);
		SIOUXTextWindow->dirty = TRUE;
		SIOUXUpdateScrollbar();
	}
#endif /* SIOUX_USE_WASTE */
}

/****************************************************************/
/* Purpose..: Handle the select all menu command				*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXDoEditSelectAll(void)
{
#if SIOUX_USE_WASTE
	WESetSelection( 0, LONG_MAX, SIOUXTextWindow->edit );
#else
	TESetSelect(0, 32767, SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
	
	SIOUXUpdateScrollbar();
}

/****************************************************************/
/* Purpose..: Setup SIOUX's menus  								*/
/* Input....: ---                       						*/
/* Returns..: ---                        						*/
/****************************************************************/
void SIOUXDoMenuChoice(long menuValue)
{
	short 	theMenu 		= HiWord(menuValue);
	short 	theMenuItem 	= LoWord(menuValue);
#if TARGET_API_MAC_OS8
	Str255 	accName;
#endif /* TARGET_API_MAC_OS8 */

	if (!SIOUXIsAppWindow(FrontWindow()))
		return;

	switch(theMenu) {
		case APPLEID:
			switch (theMenuItem) {
				case APPLEABOUT:
					SIOUXDoAboutBox();
					break;
				default: /*	ie the apple menu items ...*/
				#if TARGET_API_MAC_OS8
					GetMenuItemText(appleMenu, theMenuItem, accName);
					OpenDeskAcc(accName);
				#endif /* TARGET_API_MAC_OS8 */
					break;
			}
			break;
		case FILEID:
			switch (theMenuItem) {
				case FILESAVE:
					SIOUXDoSaveText();
					break;
				case FILEPAGESETUP:
					SIOUXDoPageSetup();
					break;
				case FILEPRINT:
					SIOUXDoPrintText();
					break;
				case FILEQUIT:
					if (SIOUXSettings.standalone)
						SIOUXQuitting = TRUE;
					break;
				default:
					break;
			}
			break;
		case EDITID:
			switch (theMenuItem) {
				case EDITCUT:
					SIOUXDoEditCut();
					break;
				case EDITCOPY:
					SIOUXDoEditCopy();
					break;
				case EDITPASTE:
					SIOUXDoEditPaste();
					break;
				case EDITCLEAR:
					SIOUXDoEditClear();
					break;
				case EDITSELECTALL:
					SIOUXDoEditSelectAll();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	if (SIOUXSettings.standalone)
		HiliteMenu(0);
}

#pragma bool reset

/*  Change Record
 *	BB 01/10/93 removed all TextEdit calls which called toolbox glue
 *				so that users no longer needed to include MacOS.lib ...
 *	BB 04/10/93 rewrote savetext so that all file saving was done using
 *				inline toolbox calls therefore no MacOS.lib ...
 *  JH 30/12/95 removed uses of OLDROUTINENAMES
 *	bk 09/02/96 added Universal Headers incase macheaders not the prefix
 *  mm 970428   Replaced missing statement allowing copying to take place.
 *	cc 991108	added ra Carbon Changes done 990611
 *  cc 991109   changed TARGET_CARBON to TARGET_API_MAC_CARBON
 *  cc 991111   added ra & jww suggestions for functions no longer in carbon
 *  cc 991115 	updated and deleted outdated carbon comments
 *  cc 991116	bug fix (IR9907-1420) - changed line 398 to if (!sfReply.sfGood)
 *  ra 000114   SIOUXSetupMenus calls InvalMenuBar for Carbon
 * JWW 000531   Don't add a Quit item in the File menu if running on OS X
*/
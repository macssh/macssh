/*  Metrowerks Standard Library  */

/*  $Date$ 
 *  $Revision$ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-2000 Metrowerks, Inc.
 *		All rights reserved.
 */

/*******************************************************************************/
/*  Project...: C++ and ANSI-C Compiler Environment                     ********/
/*  Name......: SIOUX.c                                       			********/
/*  Purpose...: Interface functions to SIOUX			                ********/
/*******************************************************************************/
/*
*/

#include "lshprefix.h"
#include "lsh_context.h"

#include <SIOUX.h> 

#include "SIOUXGlobals.h"
#include "SIOUXMenus.h"
#include "SIOUXWindows.h"

#if SIOUX_USE_WASTE
	#include "LongControls.h"		/* ¥¥¥LC  */
#endif /* SIOUX_USE_WASTE */

#include <console.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
/*#include <unix.h>*/

#include <AppleEvents.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Fonts.h> 		/*bkoz*/
#include <Sound.h>      /* mm 971006 */
#include <ToolUtils.h>
#include <Traps.h>
#include <ControlDefinitions.h>
#include <LowMem.h>


/*	defined in unix.c ...*/
#ifdef __cplusplus
extern "C" {
#endif

#if ! __MSL__
	extern char	__system7present;
	extern void	__CheckForSystem7(void);
	extern void (*__RemoveConsoleHandler__)(void);
#else
	int  __system7present(void);
#endif /* ! __MSL__ */

/*extern long __myraise(long signal);   now an inline */
		__inline long __myraise(long _a) {return (__std(raise)(_a));}												

	extern void GUSIWakeupSIOUXSocket();

	extern void sched_yield();

#ifdef __cplusplus
}
#endif

/*	Local Defines*/
//#define SIOUX_BUFSIZ 512U					/*	SIOUX's internal buffer size ...*/
#define SIOUX_BUFSIZ 2048U					/*	SIOUX's internal buffer size ...*/
#define TICK_DELTA 30UL						/*	Max ticks allowed between tests for UserBreak()*/
/*#define EOF (-1L)							/*	End Of File marker */

/*	Local Globals */
static Boolean toolBoxDone     = false;		/*	set to true when the TB is setup */
static unsigned long LastTick  = 0UL;		/*	The tickcount of the last UserBreak test ...*/
static short inputBuffersize   = 0;			/*	Used by GetCharsFromSIOUX and DoOneEvent ...*/
static CursHandle iBeamCursorH = NULL;		/*	Contains the iBeamCursor ... */
/*static Boolean atEOF 		   = false;*/		/*	Is the stream at EOF? */
Boolean atEOF 		   = false;		/*	Is the stream at EOF? */
Boolean gSIOUXTerminalMode  = false;

char gSIOUXChars[SIOUX_BUFSIZ];
const short gSIOUXMaxSize = SIOUX_BUFSIZ;
short gSIOUXBufSize = 0;

typedef struct tSIOUXBuffer 
{
	char		*startpos,	/* pointer to a block of memory which will serve as the buffer */
				*curpos,	/* pointer to insertion point in the buffer */
				*endpos;	/* pointer to end of text in the buffer */
	long		tepos;		/* current offset in TEHandle (0 if at end) */
} tSIOUXBuffer;
static tSIOUXBuffer SIOUXBuffer;

#define ZEROSIOUXBUFFER()		{															\
									SIOUXBuffer.curpos =									\
										SIOUXBuffer.endpos =								\
											SIOUXBuffer.startpos;							\
								}
									/* SIOUXBuffer.tepos = -1;									\ mm 971229 */
#define CURRENTBUFSIZE()		(SIOUXBuffer.endpos - SIOUXBuffer.startpos)
#define CHECKFOROVERFLOW(c)		{															\
									if (CURRENTBUFSIZE() + (c) >= SIOUX_BUFSIZ)				\
										InsertSIOUXBuffer();								\
								}
#define DELETEFROMBUFFER(num)	{															\
									if (SIOUXBuffer.curpos != SIOUXBuffer.endpos)			\
										BlockMoveData(SIOUXBuffer.curpos,					\
												SIOUXBuffer.curpos - (num),					\
												SIOUXBuffer.endpos - SIOUXBuffer.curpos);	\
									SIOUXBuffer.endpos -= (num);							\
									SIOUXBuffer.curpos -= (num);							\
								}
/* mm 970626 begin */
#define ROLLBACKBUFFER(num)		{															\
									SIOUXBuffer.curpos = SIOUXBuffer.endpos - (num); 		\
									SIOUXBuffer.endpos = SIOUXBuffer.curpos;		 		\
								}
/* mm 970626 end */
#if SIOUX_USE_WASTE
#define INSERTCHAR(c)			{															\
									if (SIOUXBuffer.tepos == -1) {							\
										*SIOUXBuffer.curpos = (c);							\
										if (SIOUXBuffer.curpos == SIOUXBuffer.endpos)		\
											SIOUXBuffer.endpos++;							\
										SIOUXBuffer.curpos++;								\
									} else {												\
										WEReference theTEH = SIOUXTextWindow->edit;			\
										WESetSelection(SIOUXBuffer.tepos,					\
													SIOUXBuffer.tepos + 1,					\
													theTEH);								\
										WEKey(c, nil, theTEH);								\
										SIOUXBuffer.tepos++;								\
										if (SIOUXBuffer.tepos == WEGetTextLength(theTEH ))	\
											SIOUXBuffer.tepos = -1;							\
									}														\
								}
										/*if (SIOUXBuffer.tepos == WEGetTextLength( theTEH ) - 1)	\  mm 971229 */
#else
#define INSERTCHAR(c)			{															\
									if (SIOUXBuffer.tepos == -1) {							\
										*SIOUXBuffer.curpos = (c);							\
										if (SIOUXBuffer.curpos == SIOUXBuffer.endpos)		\
											SIOUXBuffer.endpos++;							\
										SIOUXBuffer.curpos++;								\
									} else {												\
										TEHandle theTEH = SIOUXTextWindow->edit;			\
										TESetSelect(SIOUXBuffer.tepos,						\
													SIOUXBuffer.tepos + 1,					\
													theTEH);								\
										TEKey(c, theTEH);									\
										SIOUXBuffer.tepos++;								\
										if (SIOUXBuffer.tepos == (*theTEH)->teLength)	    \
											SIOUXBuffer.tepos = -1;							\
									}														\
								}
										/* if (SIOUXBuffer.tepos == (*theTEH)->teLength - 1)	\    mm 971229 */
#endif /* SIOUX_USE_WASTE */
#define INSERTLINEFEED()		{															\
									*SIOUXBuffer.curpos = 0x0d;								\
									SIOUXBuffer.curpos++;									\
									SIOUXBuffer.endpos = SIOUXBuffer.curpos;				\
									SIOUXBuffer.tepos = -1;									\
								}

/************************************************************************/
/* Purpose..: Setup the toolbox			  								*/
/* Input....: ---                       								*/
/* Returns..: ---                        								*/
/************************************************************************/
static void DoSetupToolbox(void)
{
#if TARGET_API_MAC_OS8
	/*	Initialize the ToolBox (not necessary with Carbon)...*/
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	FlushEvents(everyEvent, 0L);
	
	MaxApplZone();
#endif /* TARGET_API_MAC_OS8 */
	MoreMasters();

	InitCursor();
	toolBoxDone = true;
}

#if SIOUX_USE_WASTE
static pascal void ScrollProc ( WEReference /*edit*/ )
{
	SIOUXUpdateScrollbar ( ) ;
}
#endif

/************************************************************************/
/* Purpose..: Initialize the SIOUX textWindow							*/
/* Input....: Number of columns											*/
/* Input....: Number of rows											*/
/* Returns..: TRUE the window was initialized							*/
/************************************************************************/
Boolean SIOUXSetupTextWindow(void)
{
	Rect aRect = {0,0,1,1};
	FontInfo fi;
	
#if SIOUX_USE_WASTE
	static WEScrollUPP scrollProc = nil ;
	LongRect tempLongRect;
	long lineHeight, screenHeight, screenWidth;
	UInt32 weFeatures;/*SInt32 response; */	/* ¥¥¥MP */
#else
	short lineHeight, screenHeight, screenWidth;
#endif /* SIOUX_USE_WASTE */
	
	ProcessSerialNumber psn;
	ProcessInfoRec pinfo;
	OSErr err;
	Str63	AppName = "\p";
	FSSpec	spec;
	GrafPtr saveport;
#if TARGET_API_MAC_CARBON
	BitMap	screenBitMap;
	Rect	screenBits;
	GetQDGlobalsScreenBits(&screenBitMap);
	screenBits = screenBitMap.bounds;
#else
	Rect screenBits = qd.screenBits.bounds;
#endif /* TARGET_API_MAC_CARBON */

	/*	Get process Info*/

	GetPort(&saveport);

#if ! __MSL__
	if (__system7present == -1)
		__CheckForSystem7();

	if (__system7present)
#else
	if (__system7present())
#endif /* __MSL__ */
	{
		err = GetCurrentProcess(&psn);
		pinfo.processName = AppName;
		pinfo.processInfoLength = sizeof(pinfo);
		pinfo.processAppSpec = &spec;
		err = GetProcessInformation(&psn,&pinfo);

		if (spec.name[0] <= 59) {
			spec.name[++(spec.name[0])] = '.';
			spec.name[++(spec.name[0])] = 'o';
			spec.name[++(spec.name[0])] = 'u';
			spec.name[++(spec.name[0])] = 't';
		}
		if (SIOUXSettings.userwindowtitle != NULL)   /* mm 980609 */
			BlockMoveData(SIOUXSettings.userwindowtitle, spec.name, 
											(long)SIOUXSettings.userwindowtitle[0]+1);

	}

	if (*AppName == 0)
		BlockMoveData("\pSIOUX TextWindow", AppName, 17L);

	/*	Initialize the TextWindPtr ...*/
	SIOUXTextWindow = (pSIOUXWin)NewPtr(sizeof(tSIOUXWin));
	if (SIOUXTextWindow == 0)
		return false;
/*	SIOUXTextWindow = &SIOUXTextWindowBuf*/

	/* jd 980923 */
	SIOUXTextWindow->window = NewWindow(NULL,
										&aRect,
										spec.name,
										false,
										documentProc,
										(WindowPtr)-1L,
										false,
										0L);
	if ( SIOUXTextWindow->window == NULL ) {
		return (false);
	}
	
	SetPortWindowPort(SIOUXTextWindow->window);	/* ra 990612 UI 3.2 */

	/*	Set the font information ...*/
	TextFont(SIOUXSettings.fontid);
	TextSize(SIOUXSettings.fontsize);
	TextFace(SIOUXSettings.fontface);
	GetFontInfo(&fi);
	lineHeight = fi.ascent + fi.descent + fi.leading;

	/*	Size the window ...*/
	SIOUXSettings.columns = (SIOUXSettings.columns <= 0) ? 80 : SIOUXSettings.columns;
	SIOUXSettings.rows = (SIOUXSettings.rows <= 0) ? 24 : SIOUXSettings.rows;
	if (SIOUXSettings.showstatusline)
		aRect.bottom = 2*4 + (SIOUXSettings.rows + 1)*lineHeight + 4;	/*	2*indent + ... + titlebar*/
	else
		aRect.bottom = 2*4 + (SIOUXSettings.rows)*lineHeight + 4;	/*	2*indent + ... + titlebar*/
	aRect.right = 2*4 + SIOUXSettings.columns*CharWidth('0') + 16,	/*	2*indent + ... + scrollbar*/

#if TARGET_API_MAC_CARBON
	screenHeight = screenBits.bottom - screenBits.top - GetMBarHeight() - 24;	/*	screen height ...*/
	screenWidth = screenBits.right - screenBits.left;
#else
	screenHeight = screenBits.bottom - screenBits.top - LMGetMBarHeight() - 24;	/*	screen height ...*/
	screenWidth = screenBits.right - screenBits.left;
#endif /* TARGET_API_MAC_CARBON */

	if (aRect.bottom > screenHeight)
		aRect.bottom = ((screenHeight - 2*4 - 4) / lineHeight) * lineHeight + 2*4 + 4;

	/*	Move the window to the correct place ...*/
	if (SIOUXSettings.toppixel == 0 && SIOUXSettings.leftpixel == 0) {
		if (aRect.right > screenWidth) /*	The window is wider than the screen*/
			aRect.right = ((screenWidth - 2*4 - 16 - 20) / CharWidth('0')) * CharWidth('0') + 2*4 + 16;

	#if TARGET_API_MAC_CARBON
		MoveWindow (SIOUXTextWindow->window,
					((screenBits.right - screenBits.left - aRect.right) / 2),
					((screenHeight - aRect.bottom) / 5 + GetMBarHeight() + 24),
					false);
	#else
		MoveWindow (SIOUXTextWindow->window,
					((screenBits.right - screenBits.left - aRect.right) / 2),
					((screenHeight - aRect.bottom) / 5 + LMGetMBarHeight() + 24),
					false);
	#endif /* TARGET_API_MAC_CARBON */
	} else
		MoveWindow (SIOUXTextWindow->window,
					SIOUXSettings.leftpixel, SIOUXSettings.toppixel,
					false);
	/*	Now make it the correct size ...*/
	SizeWindow(SIOUXTextWindow->window, aRect.right, aRect.bottom, true);

	/*	Create the vertical scrollbar ...*/
#if TARGET_API_MAC_CARBON
	GetWindowPortBounds(SIOUXTextWindow->window, &aRect);
#else
	aRect = SIOUXTextWindow->window->portRect;
#endif /* TARGET_API_MAC_CARBON */
	aRect.left = aRect.right - 15;
	aRect.right += 1;
	aRect.bottom -= 14;
	aRect.top -= 1;
	if (SIOUXSettings.showstatusline)
		aRect.top += lineHeight + 4;	/*	factor in the status window ...*/

	SIOUXTextWindow->vscroll = NewControl(SIOUXTextWindow->window, &aRect, "\p",
									 true, 0, 0, 0, scrollBarProc, 0L);

#if SIOUX_USE_WASTE		/* ¥¥¥LC */
	/*	Attach a LongControls record to the scrollbar */
	err = LCAttach(SIOUXTextWindow->vscroll);	/* what do I do with the error code? */
#endif

	ShowWindow(SIOUXTextWindow->window);

	/*	Create the TEHandle ...*/
#if TARGET_API_MAC_CARBON
	GetPortBounds(GetQDGlobalsThePort(),&aRect);
#else
	aRect = qd.thePort->portRect;
#endif /* TARGET_API_MAC_CARBON */
	aRect.right -= 15;
	if (SIOUXSettings.showstatusline)
		aRect.top += lineHeight + 4;
	InsetRect(&aRect, 4, 4);

#if SIOUX_USE_WASTE
	WERectToLongRect( &aRect, &tempLongRect );
	weFeatures = ( weDoAutoScroll | weDoDrawOffscreen | weDoMonoStyled );
	if ( SIOUXSettings.wasteusetempmemory )
		weFeatures |= weDoUseTempMem;
	if ( SIOUXSettings.enabledraganddrop )
		weFeatures |= ( weDoDragAndDrop | weDoOutlineHilite );
	if ( SIOUXSettings.outlinehilite )
		weFeatures |= weDoOutlineHilite;
	err = WENew(&tempLongRect, &tempLongRect, weFeatures, &SIOUXTextWindow->edit);
	
	/*	install the scroll callback */
	if ( scrollProc == nil )
	{
		scrollProc = NewWEScrollUPP( ScrollProc );  /* RA/1/9/99 NewWEScrollProc */
	}
	err = WESetInfo( weScrollProc, & scrollProc, SIOUXTextWindow->edit );

#else
	SIOUXTextWindow->edit = TENew(&aRect, &aRect);

	TEAutoView(true, SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
	
	SIOUXTextWindow->linesInFolder = SIOUXSettings.rows;	/*	Number of lines visible ...*/
	SIOUXTextWindow->dirty = 0;								/*	Window is clean ...*/
	SIOUXTextWindow->dirid = 0;								/*	Window hasn't been saved yet ...*/

	SIOUXUpdateWindow(SIOUXTextWindow->window);

	SetPort(saveport);
	return(true);
}

/************************************************************************/
/*	Purpose..: 	Determines value of current top line					*/
/*	Input....:	TextEdit Handle											*/
/*	Return...:	Position on current line								*/
/************************************************************************/
static long CurrentTopLine(
#if SIOUX_USE_WASTE
	WEReference theTEH
#else
	TEHandle theTEH
#endif /* SIOUX_USE_WASTE */
)
{
#if SIOUX_USE_WASTE
	long topLineNumber;
	long height;
	
	LongRect weViewRect, weDestRect;
	WEGetViewRect( &weViewRect, theTEH );
	WEGetDestRect( &weDestRect, theTEH );
	
	height = WEGetHeight( 0, 1, theTEH );
	topLineNumber = (weViewRect.top - weDestRect.top) /
					WEGetHeight( 0, 1, theTEH );
#else
	short topLineNumber;
	topLineNumber = ((*theTEH)->viewRect.top - (*theTEH)->destRect.top) /
					((*theTEH)->lineHeight);
#endif /* SIOUX_USE_WASTE */
	return topLineNumber;
}

/************************************************************************/
/*	Purpose..: 	Determines position on current line	(in chars)			*/
/*	Input....:	TextEdit Handle											*/
/*	Return...:	Position on current line								*/
/************************************************************************/
static long OffsetOnCurrentLine(
#if SIOUX_USE_WASTE
	WEReference theTEH,
#else
	TEHandle theTEH,
#endif /* SIOUX_USE_WASTE */
	Boolean buf
)
{
	char *ptr, *start;
	long result;
#if SIOUX_USE_WASTE
	long i, lastCharIndex;
	SInt32 weSelStart, weSelEnd;
#endif /* SIOUX_USE_WASTE */
	
	/*	Check for a CR in the buffer ...*/
	if (buf && SIOUXBuffer.endpos != SIOUXBuffer.startpos) 
	{
		for (start = SIOUXBuffer.startpos, ptr = SIOUXBuffer.endpos; ptr > start; ptr--)
			/*if (*ptr == 0x0d) return (SIOUXBuffer.endpos - ptr - 1);  /*mm 970903*/
			if (ptr[-1] == 0x0d)                    /*mm 980112 */
				return (SIOUXBuffer.endpos - ptr);  /*mm 980112*/
	}

#if SIOUX_USE_WASTE
	WEGetSelection( &weSelStart, &weSelEnd, theTEH );
	
	lastCharIndex = WEGetTextLength( theTEH ) - 1;
	i = 0;
	/*while ( ( lastCharIndex > weSelStart ) && WEGetChar( lastCharIndex - i, theTEH ) != 0x0d )  mm 971230 */
	while ( ( lastCharIndex >= i ) && WEGetChar( lastCharIndex - i, theTEH ) != 0x0d )         /* mm 971230 */
		/*i--;*/            /* mm 971230 */
		i++;                /* mm 971230 */
	
	/*result = weSelStart - i + ( SIOUXBuffer.endpos - SIOUXBuffer.startpos );*/  /* mm 971230 */
	result = i + ( SIOUXBuffer.endpos - SIOUXBuffer.startpos);                    /* mm 971230 */
#else
	HLock((Handle)theTEH);
	HLock((*theTEH)->hText);

	start = *(*theTEH)->hText;
	ptr = *(*theTEH)->hText + (*theTEH)->selStart;
	while (ptr > start && ptr[-1] != 0x0d)
		ptr--;
	
	result = *(*theTEH)->hText + (*theTEH)->selStart - ptr + SIOUXBuffer.endpos - SIOUXBuffer.startpos;
	
	HUnlock((*theTEH)->hText);
	HUnlock((Handle)theTEH);
#endif /* SIOUX_USE_WASTE */
	
	return result;
}

/************************************************************************/
/*	Purpose..:	Handle a mouseDown event								*/
/*	Input....:	pointer to an Event										*/
/*	Return...:	true/false												*/
/************************************************************************/
static Boolean HandleMouseDownEvent(EventRecord *theEvent)
{
	WindowPtr window;
	short part;
	Boolean isSIOUXwindow;
/*	Point theWhere; */
	
	part = FindWindow(theEvent->where, &window);
	isSIOUXwindow = SIOUXIsAppWindow(window);
	
	switch (part) 
	{
		case inMenuBar:
			if (SIOUXSettings.setupmenus) 
			{
				SIOUXUpdateMenuItems();
				/* jd 980923 - should update this to MenuEvent?? */
				SIOUXDoMenuChoice(MenuSelect(theEvent->where));
				return true;
			}
			break;
			
		#if TARGET_API_MAC_OS8     /*instead of !TARGET_API_MAC_CARBON*/  /* cc 991111 */
		case inSysWindow:
			if (SIOUXSettings.standalone)
				SystemClick(theEvent, window);
			break;
		#endif /* cc 991111 */
			
		case inContent:
			/*									mm 980605a 
			**	SIOUX should only select windows that it knows about so
			**	that the app embedding a SIOUx window can decide whether or
			**	not to select the window.
			*/
			if (isSIOUXwindow) 
			{

				if (window != FrontWindow())
					SelectWindow(window);
				else
				{
					if (SIOUXState == PRINTFING) 
					{
					if (StillDown())
							{
						while (WaitMouseUp())
							;	/*	Pause output while mouse is down ...*/
							}
				} else
					SIOUXDoContentClick(window, theEvent);
				}

				return true;
			}
			break;
			
		case inDrag:
			/*           mm 980605b
			**	SIOUX should select its window when the user clicks in
			**	the drag region if it's not the FrontWindow.
			*/
			if (isSIOUXwindow) 
			{
				if (window != FrontWindow())
					SelectWindow(window);
				else
				DragWindow(window, theEvent->where, &SIOUXBigRect);
				return true;
			}
			break;
			
		case inGrow:
			/*				mm 980605b
			**	SIOUX should select its window when the user clicks in
			**	the grow region if it's not the FrontWindow.
			*/
			if (isSIOUXwindow) 
			{
				if (window != FrontWindow())
					SelectWindow(window);
				else
				SIOUXMyGrowWindow(window, theEvent->where);
				return true;
			}
			break;
		}

	return false;
}

/************************************************************************/
/*	Purpose..:	Handle update and activate/deactivate events			*/
/*	Input....:	pointer to an Event										*/
/*	Return...:	true/false												*/
/************************************************************************/
static Boolean HandleUpdateActivateEvent(EventRecord *theEvent)
{
	if (SIOUXIsAppWindow((WindowPtr)theEvent->message)) 
	{
		if (theEvent->what == updateEvt)
			SIOUXUpdateWindow((WindowPtr)theEvent->message);
		else 
		{	/* must be an activate/deactivate event */
			if (theEvent->modifiers & activeFlag) 
			{
#if SIOUX_USE_WASTE
				WEActivate(SIOUXTextWindow->edit);
#else
				TEActivate(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
				ShowControl(SIOUXTextWindow->vscroll);
			} 
			else 
			{
#if SIOUX_USE_WASTE
				WEDeactivate(SIOUXTextWindow->edit);
#else
				TEDeactivate(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
				HideControl(SIOUXTextWindow->vscroll);
			}
			SIOUXDrawGrowBox(SIOUXTextWindow->window);	/* ra 990612 Use WindowPtr */
			SIOUXUpdateStatusLine(SIOUXTextWindow->window);
		}
		return true;
	}

	return false;
}

/************************************************************************/
/*	Purpose..:	Handle update and activate/deactivate events			*/
/*	Input....:	pointer to an Event										*/
/*	Return...:	true/false												*/
/************************************************************************/
static Boolean HandleOSEvents(EventRecord *theEvent)
{
	switch ((theEvent->message >> 24) & 0xff) 
	{
		case resumeFlag:
			if (theEvent->message & suspendResumeMessage) 
			{
#if SIOUX_USE_WASTE
				WEActivate(SIOUXTextWindow->edit);
#else
				TEActivate(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
				ShowControl(SIOUXTextWindow->vscroll);
			} 
			else 
			{
#if SIOUX_USE_WASTE
				WEDeactivate(SIOUXTextWindow->edit);
#else
				TEDeactivate(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
				HideControl(SIOUXTextWindow->vscroll);
			}
			SIOUXDrawGrowBox(SIOUXTextWindow->window);	/* ra 990612 Use WindowPtr */
			SIOUXUpdateStatusLine(SIOUXTextWindow->window);
			return(true);

		default:
			return(false);
	}
}

/************************************************************************/
/*	Purpose..:	Detect a user break (Command-'.')						*/
/*	Input....:	pointers an size of block								*/
/*	Return...:	true/false												*/
/************************************************************************/
static void UserBreak(void)
{
	EventRecord ev;

	if (TickCount() - LastTick < TICK_DELTA)
		return;

	LastTick = TickCount();

	if (SIOUXUseWaitNextEvent)
		WaitNextEvent(everyEvent, &ev, GetCaretTime(), NULL);
	else 
	{
	#if TARGET_API_MAC_OS8     /*instead of !TARGET_API_MAC_CARBON*/ /* cc 991111 */
		/*	Keep the system happy ...*/
		SystemTask();
	#endif  /* cc 991111 */

		GetNextEvent(everyEvent, &ev);
	}

	switch (ev.what) 
	{
		case nullEvent:		/*	ignore it ...*/
			break;

		case keyDown:
		case autoKey:		/*	check for break ...*/
			if (ev.modifiers & cmdKey) 
			{
				if ((ev.message & charCodeMask) == '.')
					__myraise(SIGINT);
				if ((ev.message & charCodeMask) == 'q' || (ev.message & charCodeMask) == 'Q') {
					SIOUXQuitting = true;
					__myraise(SIGINT);
				}
			}
			if ((ev.message & charCodeMask) == 0x03 && (ev.message & keyCodeMask) >> 8 != 0x4c)
				__myraise(SIGINT);			/*	enter and control c have same char code*/
			break;

		case mouseDown:
			HandleMouseDownEvent(&ev);
			break;

		case activateEvt:
		case updateEvt:
			HandleUpdateActivateEvent(&ev);
			break;

		case osEvt:
			HandleOSEvents(&ev);
			break;

		case kHighLevelEvent:
#if ! __MSL__
			if (__system7present == -1)
				__CheckForSystem7();
		
			if (__system7present)
#else
			if (__system7present())
#endif /* ! __MSL__ */
			{
				AEProcessAppleEvent(&ev);
			}
			break;

		#if TARGET_API_MAC_OS8     /*instead of !TARGET_API_MAC_CARBON*/  /* cc 991111 */
		case diskEvt:
			if (HiWord(ev.message) != noErr) {
				Point pt = {100, 100};
				DIBadMount(pt, ev.message);
			}
			break;
		#endif  /* cc 991111 */

		case mouseUp:
		case keyUp:
		default:
			break;
	}
}

/************************************************************************/
/*	Purpose..: 	Insert the current SIOUX buffer into the TE Handle		*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static void InsertSIOUXBuffer(void)
{
#if SIOUX_USE_WASTE
	WEReference theTEH = SIOUXTextWindow->edit;
	long teLength;
	
/* the following variables were used for the aborted error-handling code found later in this function --pcg */
/*	long weToDelete;
	SInt16 autoScrollFlag;
*/
	teLength = WEGetTextLength( theTEH );
#else
	TEHandle theTEH = SIOUXTextWindow->edit;
	short teLength;
	
	HLock((Handle)theTEH);
	teLength = (*theTEH)->teLength;
#endif /* SIOUX_USE_WASTE */
	
#if ! SIOUX_USE_WASTE
	/* this isn't necessary w/ WASTE  -pcg */
	if ((teLength + CURRENTBUFSIZE()) > 32767) 
	{
		/*	Insert will grow TEHandle past 32K so we cut out excess from top ...*/
		char *ptr;
		short todelete = (short) ((teLength + CURRENTBUFSIZE()) - 32767) + 8*SIOUX_BUFSIZ;
		
		/*	Make sure that the text to be cut ends on a CR ...*/
		HLock((*theTEH)->hText);
		for (ptr = *(*theTEH)->hText + todelete; *ptr != 0x0d; ptr++) ;

		/*	We now point at the CR, increment ptr to point after it ...*/
		todelete += ++ptr - (*(*theTEH)->hText + todelete);
		HUnlock((*theTEH)->hText);
		
		/*	We hit the fields directly to keep TE from redrawing twice*/
		(*theTEH)->selStart = 0;
		(*theTEH)->selEnd   = todelete;
		TEDelete(theTEH);

		/*	Now fix things up...*/
		teLength = (*theTEH)->teLength;
	}
#endif /* ! SIOUX_USE_WASTE */

	/*	Now insert the new text ...*/
#if SIOUX_USE_WASTE
	/* aborted code to handle WASTE buffer size limiting. I'll leave it here
	for future generations to try their hand at.  --pcg */
/*	if ( SIOUXSettings.wastemaxbuffersize > 0 )
	{
		if ( teLength + CURRENTBUFSIZE() > SIOUXSettings.wastemaxbuffersize )
		{
			autoScrollFlag = WEFeatureFlag( weFAutoScroll, weBitTest, theTEH );
			weToDelete = CURRENTBUFSIZE();
			while ( ( weToDelete < teLength ) && ( WEGetChar( weToDelete, theTEH ) != 0x0d ) )
				weToDelete++;
			WEFeatureFlag( weFAutoScroll, weBitClear, theTEH );
			WESetSelection( 0, weToDelete + 1, theTEH );
			WEDelete( theTEH );
			SIOUXUpdateScrollbar();
			LCSetValue( SIOUXTextWindow->vscroll, */								/*¥¥¥LC */
/*									LCGetMax( SIOUXTextWindow->vscroll ) );	*/	/*¥¥¥LC */
/*			AdjustText();
			WEFeatureFlag( weFAutoScroll, autoScrollFlag, theTEH );
			
			teLength = WEGetTextLength( theTEH );
		}
	}
*/
	if (!gSIOUXTerminalMode)
		WESetSelection( teLength, teLength, theTEH );
	else {
		// need to replace current line if it's not empty
		SInt32 weSelStart, weSelEnd;
		int i;
		WEGetSelection( &weSelStart, &weSelEnd, theTEH );
		i = weSelStart;
		while ( i < teLength
		     && i - weSelStart < CURRENTBUFSIZE()
		     && WEGetChar( i, theTEH ) != 0x0d )
			++i;
		weSelEnd = i;
		WESetSelection( weSelStart, weSelEnd, theTEH );
	}
	WEInsert(SIOUXBuffer.startpos, CURRENTBUFSIZE(), nil, nil, theTEH);
	
	teLength = WEGetTextLength( theTEH );
#else
	if (!gSIOUXTerminalMode)
		TESetSelect(teLength, teLength, theTEH);
	TEInsert(SIOUXBuffer.startpos, CURRENTBUFSIZE(), theTEH);
	teLength = (*theTEH)->teLength;
#endif /* SIOUX_USE_WASTE */
	
	SIOUXTextWindow->dirty = true;
	ZEROSIOUXBUFFER();

#if SIOUX_USE_WASTE
	if (!gSIOUXTerminalMode)
		WESetSelection(teLength, teLength, theTEH);
#else
	if (!gSIOUXTerminalMode)
		TESetSelect(teLength, teLength, theTEH);

	HUnlock((Handle)theTEH);
#endif /* SIOUX_USE_WASTE */

	if (SIOUXSettings.standalone)
		UserBreak();
	else
		SIOUXUpdateScrollbar();
}

/************************************************************************/
/*	Purpose..:	Determine the user's theoretical menuchoice				*/
/*	Input....:	Character typed											*/
/*	Return...:	Menuchoice												*/
/************************************************************************/
static long myMenuKey(char key)
{
	short theMenu = 0;
	short theMenuItem = 0;

	switch (key) 
	{
		/*	File menu choices*/
		case 's': case 'S':
			theMenu = FILEID;
			theMenuItem = FILESAVE;
			break;
			
		case 'p': case 'P':
			theMenu = FILEID;
			theMenuItem = FILEPRINT;
			break;
			
		case 'q': case 'Q':
			theMenu = FILEID;
			theMenuItem = FILEQUIT;
			break;
			
		case 'x': case 'X':
			theMenu = EDITID;
			theMenuItem = EDITCUT;
			break;
			
		case 'c': case 'C':
			theMenu = EDITID;
			theMenuItem = EDITCOPY;
			break;
			
		case 'v': case 'V':
			theMenu = EDITID;
			theMenuItem = EDITPASTE;
			break;
			
		case 'a': case 'A':
			theMenu = EDITID;
			theMenuItem = EDITSELECTALL;
			break;
	}

	return (((long)theMenu << 16) | theMenuItem);
}

/************************************************************************/
/*	Purpose..: 	Check if insertion range is in edit range				*/
/*	Input....:	first character in edit range							*/
/*	Input....:	Handle to textedit										*/
/*	Return...:	true/false												*/
/************************************************************************/
Boolean SIOUXisinrange(
#if SIOUX_USE_WASTE
	long first, WEReference te
#else
	short first, TEHandle te
#endif /* SIOUX_USE_WASTE */
)
{
#if SIOUX_USE_WASTE
	SInt32 weSelStart, weSelEnd;
	
	WEGetSelection( &weSelStart, &weSelEnd, te );
	if (!gSIOUXTerminalMode && ((weSelStart < first) || (weSelEnd < first) ))
#else
	if (!gSIOUXTerminalMode && (((*te)->selStart < first) || (*te)->selEnd < first))
#endif /* SIOUX_USE_WASTE */
	{
		SysBeep(10);
		return false;
	} else
		return true;
}

/************************************************************************/
/* Purpose..: Handles a single event ...  								*/
/* Input....: If non-zero then pointer to an event						*/
/* Returns..: ---           		             						*/
/************************************************************************/
short SIOUXHandleOneEvent(EventRecord *userevent)
{
	EventRecord theEvent;
	WindowPtr	window;
	char		aChar;
#if SIOUX_USE_WASTE
	LongRect weViewRect;
	long scrollAdjust, directionMultiplier;
#else
	short scrollAdjust, directionMultiplier;
#endif /* SIOUX_USE_WASTE */

	if (SIOUXState == OFF)
		return false;

	if (userevent)						/*	External call of the function ...*/
		theEvent = *userevent;
	else if (SIOUXUseWaitNextEvent)		/*	Internal with WNE allowed ...*/
		WaitNextEvent(everyEvent, &theEvent, GetCaretTime(), NULL);
	else 
	{								/*	Internal with no WNE allowed ...*/
		#if TARGET_API_MAC_OS8     /*instead of !TARGET_API_MAC_CARBON*/  /*cc 991111 */
		SystemTask();
		#endif   /*cc 991111 */
		
		GetNextEvent(everyEvent, &theEvent);
	}

	window = FrontWindow();

	switch (theEvent.what) 
	{
		case nullEvent:
			/*	Maintain the cursor*/
			if (SIOUXIsAppWindow(window))
			{
				GrafPtr savePort;Point localMouse;		/* ¥¥¥MP  */
#if SIOUX_USE_WASTE
				Rect tempRect;
				LongRect weViewRect;
#endif /* SIOUX_USE_WASTE */

				GetPort(&savePort);
				SetPortWindowPort(window); /* ra 990612 UI 3.2 */
				localMouse = theEvent.where;GlobalToLocal(&localMouse);		/* ¥¥¥MP */
#if SIOUX_USE_WASTE
				WEGetViewRect( &weViewRect, SIOUXTextWindow->edit );
				WELongRectToRect( &weViewRect, &tempRect );
				if (PtInRect(localMouse, &tempRect) && iBeamCursorH )		/* ¥¥¥MP */
#else
				if (PtInRect(localMouse, &(*SIOUXTextWindow->edit)->viewRect) &&	/* ¥¥¥MP */
					iBeamCursorH)
#endif /* SIOUX_USE_WASTE */
				{
#if SIOUX_USE_WASTE
					WEAdjustCursor( theEvent.where, nil, SIOUXTextWindow->edit );
#else
					SetCursor(*iBeamCursorH);
#endif /* SIOUX_USE_WASTE */
				} 
				else 
				{
				#if TARGET_API_MAC_CARBON
					Cursor theArrow;
					SetCursor(GetQDGlobalsArrow(&theArrow));
				#else
					SetCursor(&qd.arrow);
				#endif /* TARGET_API_MAC_CARBON */
				}
				/*LocalToGlobal(&theEvent.where);	 */	/* ¥¥¥MP */
#if SIOUX_USE_WASTE
				WEAdjustCursor( theEvent.where, nil, SIOUXTextWindow->edit );
				WEIdle( nil, SIOUXTextWindow->edit );
#else
				TEIdle(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
				SetPort(savePort);
				/*				mm 980605c
				**	SIOUX should return false so that an app embedding
				**	SIOUX that looks at the result of SIOUXHandleOneEvent()
				**	will be able to process the NULL event as well.
				*/
				return false;
			} 
			else 
			{
				if (SIOUXSettings.standalone)	/* JWW - only set cursor when standalone */
				{
				#if TARGET_API_MAC_CARBON
					Cursor theArrow;
					SetCursor(GetQDGlobalsArrow(&theArrow));
				#else
					SetCursor(&qd.arrow);
				#endif /* TARGET_API_MAC_CARBON */
				
					if (SIOUXTextWindow != NULL)
#if SIOUX_USE_WASTE
						WEIdle( nil, SIOUXTextWindow->edit );
#else
						TEIdle(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
				}
			}
			break;

		case mouseDown:
			if (HandleMouseDownEvent(&theEvent))
				return true;
			break;

		case keyDown:
		case autoKey:
			if (SIOUXIsAppWindow(window)) {
				aChar = (theEvent.message & charCodeMask);
				if ((theEvent.modifiers & cmdKey) && (aChar > 0x20)) 
				{	/*¥¥¥MP */
					/*	Check first for command - '.'*/
					if (SIOUXState != TERMINATED && aChar == '.')
						__myraise(SIGINT);
					if (SIOUXSettings.setupmenus) 
					{
						SIOUXUpdateMenuItems();
						SIOUXDoMenuChoice(MenuKey(aChar));
					} 
					else
						SIOUXDoMenuChoice(myMenuKey(aChar));
					return true;
				} else {
					if (((theEvent.message & keyCodeMask) >> 8 == 0x4c) ||                            /* mm 980413 */
													((theEvent.message & keyCodeMask) >> 8 == 0x34))  /* mm 980413 */
						aChar = 0x0d;				/*	map enter key to return key ...*/
					if (SIOUXState == SCANFING) {
						/*	If there are too many characters on the line already then just return ...*/
#if SIOUX_USE_WASTE
						if ((WEGetTextLength( SIOUXTextWindow->edit ) - SIOUXselstart + 1) >= inputBuffersize)
#else
						if (((*SIOUXTextWindow->edit)->teLength - SIOUXselstart + 1) >= inputBuffersize)
#endif /* SIOUX_USE_WASTE */
						{
							SysBeep(10);
							return false;
						}
						switch (aChar) 
						{
							case 0x1a:				/*	Control - 'z'*/
							case 0x04:				/*	Control - 'd'*/
								/*	Place in the enter key char which will become the EOF*/
								aChar = 0x03;
							case 0x0d:				/*	Carriage Return*/
								SIOUXState = IDLE;
								break;
								
							case 0x03:				/*	Control - 'c'*/
								__myraise(SIGINT);
								break;
								
							case 0x08:				/*	Delete*/
								if (!gSIOUXTerminalMode) {
									if (!SIOUXisinrange(SIOUXselstart + 1, SIOUXTextWindow->edit))
										return false;
								}
								break;
								
							default:
								break;
						}
						
						/*	if the cursor is currently outside the typeable region then move it ...*/
						if ((aChar >= ' ') && !SIOUXisinrange(SIOUXselstart, SIOUXTextWindow->edit)) {
#if SIOUX_USE_WASTE
							WESetSelection(WEGetTextLength( SIOUXTextWindow->edit ),
										WEGetTextLength( SIOUXTextWindow->edit ),
										SIOUXTextWindow->edit);
#else
							TESetSelect((*SIOUXTextWindow->edit)->teLength,
										(*SIOUXTextWindow->edit)->teLength,
										SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
						}

					}
					
					directionMultiplier = 1;
					switch ( aChar )
					{
						case 0x0C:	/* pageDown */
							directionMultiplier = -1;
						case 0x0B:	/* pageUp */
#if SIOUX_USE_WASTE
							WEGetViewRect( &weViewRect, SIOUXTextWindow->edit );
							scrollAdjust = (weViewRect.bottom - weViewRect.top) /
							   		 	   WEGetHeight( 0, 1, SIOUXTextWindow->edit );
#else
							scrollAdjust = ( (*SIOUXTextWindow->edit)->viewRect.bottom -
													(*SIOUXTextWindow->edit)->viewRect.top ) /
													(*SIOUXTextWindow->edit)->lineHeight;
#endif /* SIOUX_USE_WASTE */
							scrollAdjust *= directionMultiplier;
							MoveScrollBox(SIOUXTextWindow->vscroll, scrollAdjust);
							AdjustText();
							break;
							
						case 0x01:	/* home */
#if SIOUX_USE_WASTE
							LCSetValue( SIOUXTextWindow->vscroll,	/* ¥¥¥LC */
													LCGetMin( SIOUXTextWindow->vscroll ) );
#else
							SetControlValue( SIOUXTextWindow->vscroll,
													GetControlMinimum( SIOUXTextWindow->vscroll ) );
#endif /* SIOUX_USE_WASTE */
							AdjustText();
							break;
							
						case 0x04:	/* end */
#if SIOUX_USE_WASTE
							LCSetValue( SIOUXTextWindow->vscroll,	/* ¥¥¥LC */
													LCGetMax( SIOUXTextWindow->vscroll ) );
#else
							SetControlValue( SIOUXTextWindow->vscroll,
													GetControlMaximum( SIOUXTextWindow->vscroll ) );
#endif /* SIOUX_USE_WASTE */
							AdjustText();
							break;
							
						default:
							if ( !gSIOUXTerminalMode ) {
#if SIOUX_USE_WASTE
								WEKey(aChar, theEvent.modifiers, SIOUXTextWindow->edit);
#else
								TEKey(aChar, SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
							} else {
								gSIOUXChars[gSIOUXBufSize++] = aChar;
								GUSIWakeupSIOUXSocket();
								SIOUXState = IDLE;
							}
							SIOUXUpdateScrollbar();
							if (aChar < 0x1c || aChar > 0x1f) {
								SIOUXTextWindow->dirty = true;
							}
					}
					return true;
				}
			}
			break;

		case activateEvt:
		case updateEvt:
			if (HandleUpdateActivateEvent(&theEvent))
				return true;
			break;

		case osEvt:
			if (HandleOSEvents(&theEvent))
				/*  					mm 980605c
				**	SIOUX should return false so that an app embedding
				**	SIOUX that looks at the result of SIOUXHandleOneEvent()
				**	will be able to process the OS event as well.
				*/
				return false;
			break;

		case kHighLevelEvent:
#if ! __MSL__
			if (__system7present == -1)
				__CheckForSystem7();
			
			if (__system7present)
#else
			if (__system7present())
#endif /* ! __MSL__ */
			{
				AEProcessAppleEvent(&theEvent);
			}
			break;

		#if TARGET_API_MAC_OS8     /*instead of !TARGET_API_MAC_CARBON*/ /*cc 991111 */
		case diskEvt:
			if (HiWord(theEvent.message) != noErr) 
			{
				Point pt = {100, 100};
				DIBadMount(pt, theEvent.message);
			}
			break;
		#endif /*cc 991111 */

		case mouseUp:
		case keyUp:
		default:
			break;
	}
	return false;
}

/************************************************************************/
/* Purpose..: Cleans up the data for a quit ...							*/
/* Input....: ---                       								*/
/* Returns..: true killed everything/false user cancelled ...			*/
/************************************************************************/
static Boolean SIOUXCleanUp(void)
{
	short item;
	Str255 aString;

	if (SIOUXTextWindow) 
	{
		if (SIOUXTextWindow->dirty && SIOUXSettings.asktosaveonclose) 
		{
			GetWTitle(SIOUXTextWindow->window, aString); /* ra 990612 Use WindowPtr */
		
		#if TARGET_API_MAC_CARBON
			{
				Cursor arrowCursor;
				SetCursor(GetQDGlobalsArrow(&arrowCursor));
			}
		#else
			SetCursor(&qd.arrow);
		#endif /* TARGET_API_MAC_CARBON */
		
			item = SIOUXYesNoCancelAlert(aString);
	
			switch (item) 
			{
				case 1:		/*	Yes*/
					if (SIOUXDoSaveText() != noErr && SIOUXSettings.standalone) 
					{	/*	Save the textWindow ...*/
						SIOUXQuitting = false;
						return (false);
					}
					break;
					
				case 3:		/*	Cancel*/
					SIOUXQuitting = false;
					return (false);
					
				case 2:		/*	No*/
				default:	/*	error*/
					break;
			}
		}

#if SIOUX_USE_WASTE		/* ¥¥¥LC */
		/*	before killing the scrollbar, be sure to dispose of the LongControls record */
		LCDetach(SIOUXTextWindow->vscroll);
#endif /* SIOUX_USE_WASTE */


		/*	Kill the textWindow ...*/
	    KillControls(SIOUXTextWindow->window);
#if SIOUX_USE_WASTE
	    WEDispose(SIOUXTextWindow->edit);
#else
	    TEDispose(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */

	#if TARGET_API_MAC_OS8
		/* CloseWindow doesn't exist under Carbon since you cannot allocate
			your own storage anyways */				
	   	CloseWindow(SIOUXTextWindow->window);
	#else
		DisposeWindow(SIOUXTextWindow->window);	/* jd */
	#endif /* TARGET_API_MAC_OS8 */
		
		DisposePtr((Ptr)SIOUXTextWindow);
		SIOUXTextWindow = 0L;
		ZEROSIOUXBUFFER();
	}

	return (true);
}

/************************************************************************/
/*	Purpose..: 	Make sure all required Apple Event parameters are used	*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static OSErr GotRequiredParams(const AppleEvent *theEvent)
{
	OSErr theErr;
	DescType theType;
	Size theSize;
	
	theErr = AEGetAttributePtr(theEvent, keyMissedKeywordAttr, typeWildCard, &theType,
		NULL, 0, &theSize);
	
	if (theErr == errAEDescNotFound)
		theErr = noErr;
	else
		theErr = errAEParamMissed;
	
	return theErr;
}

/************************************************************************/
/*	Purpose..: 	Handle Apple Event for when application is launched		*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static pascal OSErr DoHandleOpenApplication(const AppleEvent *theEvent, AppleEvent *theReply,
	UInt32 refCon)
{
#pragma unused(theReply, refCon)
	OSErr theErr;
	
	theErr = GotRequiredParams(theEvent);
	
	return theErr;
}

/************************************************************************/
/*	Purpose..: 	Process open and print Apple Event						*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static OSErr DoHandleOpenPrint(const AppleEvent *theEvent, Boolean isOpen)
{
#pragma unused(isOpen)
	OSErr theErr;
	AEDescList theDocuments;
	
	theErr = AEGetParamDesc(theEvent, keyDirectObject, typeAEList, &theDocuments);
	
	if (theErr == noErr)
	{
		theErr = GotRequiredParams(theEvent);
		
		AEDisposeDesc(&theDocuments);
	}
	
	return theErr;
}

/************************************************************************/
/*	Purpose..: 	Handler for Open Document Apple Event					*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static pascal OSErr DoHandleOpenDocuments(const AppleEvent *theEvent, AppleEvent *theReply,
	UInt32 refCon)
{
#pragma unused(theReply, refCon)
	OSErr theErr;
	
	theErr = DoHandleOpenPrint(theEvent, true);
	
	return theErr;
}

/************************************************************************/
/*	Purpose..: 	Handler for Print Document Apple Event					*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static pascal OSErr DoHandlePrintDocuments(const AppleEvent *theEvent, AppleEvent *theReply,
	UInt32 refCon)
{
#pragma unused(theReply, refCon)
	OSErr theErr;
	
	theErr = DoHandleOpenPrint(theEvent, false);
	
	return theErr;
}

/************************************************************************/
/*	Purpose..: 	Handle Apple Event for quit application requests		*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
static pascal OSErr DoHandleQuit(const AppleEvent *theEvent, AppleEvent *theReply,
	UInt32 refCon)
{
#pragma unused(theReply, refCon)
	OSErr theErr;
	
	theErr = GotRequiredParams(theEvent);
	
	if (theErr == noErr)
	{
		SIOUXQuitting = true;
		
		if (SIOUXCleanUp() == false)
			theErr = userCanceledErr;
	}
	
	return theErr;
}

/************************************************************************/
/* Purpose..: Install the console package								*/
/* Input....: The stream to install (ignored)							*/
/* Returns..: 0 no error / -1 error occurred							*/
/************************************************************************/
short InstallConsole(short fd)
{

#pragma unused (fd)

	if (SIOUXQuitting || SIOUXState != OFF) return 0;

	if (SIOUXSettings.initializeTB && !toolBoxDone)
		DoSetupToolbox();

	/*	Initialize Space for the SIOUX buffer ...*/
	if ((SIOUXBuffer.startpos = (char *)NewPtr(SIOUX_BUFSIZ)) == NULL)
		return -1;
	ZEROSIOUXBUFFER();
	SIOUXBuffer.tepos = -1;    /* mm 980108 */
	/*	Setup the menus ...*/
	if (SIOUXSettings.setupmenus)
		SIOUXSetupMenus();

	/* JWW - Install Apple Event handlers for standalone SIOUX appliation */
	if (SIOUXSettings.standalone == true)
	{
#if ! __MSL__
		if (__system7present == -1)
			__CheckForSystem7();
	
		if (__system7present)
#else
		if (__system7present())
#endif /* ! __MSL__ */
		{
			AEInstallEventHandler(kCoreEventClass, kAEOpenApplication,
				NewAEEventHandlerProc(&DoHandleOpenApplication), 0, false);
			
			AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments,
				NewAEEventHandlerProc(&DoHandleOpenDocuments), 0, false);
			
			AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments,
				NewAEEventHandlerProc(&DoHandlePrintDocuments), 0, false);
			
			AEInstallEventHandler(kCoreEventClass, kAEQuitApplication,
				NewAEEventHandlerProc(&DoHandleQuit), 0, false);
		}
	}
	
	/*	Setup the textWindow ...*/
	if (SIOUXSetupTextWindow()) 
	{
		if (SIOUXSettings.standalone == false) 
		{
			SIOUXSettings.autocloseonquit = true;
		}

		SIOUXState = IDLE;

		/*	Test for WaitNextEvent ...*/
	
	#if !TARGET_API_MAC_OS8
		SIOUXUseWaitNextEvent = true;
	#else
		if (GetToolTrapAddress(_WaitNextEvent) != GetToolTrapAddress(_Unimplemented))
			SIOUXUseWaitNextEvent = true;
	#endif /* !TARGET_API_MAC_OS8 */

		iBeamCursorH = GetCursor(iBeamCursor);

		return 0;
	}

#if ! __MSL__
		__RemoveConsoleHandler__ = RemoveConsole;
#endif /* ! __MSL__ */
			
	return(-1);
}

/************************************************************************/
/* Purpose..: Remove the console package								*/
/* Input....: ---														*/
/* Returns..: ---														*/
/************************************************************************/
void RemoveConsole(void)
{
	extern int __aborting;

	if (SIOUXState == OFF || !SIOUXTextWindow)
		return;

	if (__aborting)
		SIOUXState = ABORTED;
	else
		SIOUXState = TERMINATED;
	SIOUXUpdateStatusLine(SIOUXTextWindow->window);	/* ra 990612 Use WindowPtr */

	SIOUXselstart = 0;
#if SIOUX_USE_WASTE
	WEActivate(SIOUXTextWindow->edit);
#else
	TEActivate(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
	SIOUXUpdateScrollbar();

	if (SIOUXSettings.autocloseonquit)
		SIOUXQuitting = true;

	while (!SIOUXQuitting) 
	{
BackInTheLoop:
		SIOUXHandleOneEvent(NULL);
	}

	if (!SIOUXCleanUp())
		goto BackInTheLoop;

#if ! __MSL__
	__RemoveConsoleHandler__ = NULL;
#endif /* ! __MSL__ */

	SIOUXState = OFF;
}

/************************************************************************/
/*	Purpose..: 	Write a string to the console							*/
/*	Input....:	pointer to buffer										*/
/*	Input....:	number of chars in buffer								*/
/*	Return...:	0 no error / -1 error occurred							*/
/************************************************************************/
long WriteCharsToConsole(char *buffer, long n)
{
	long counter, i, spacestoinsert;
	char aChar;
	GrafPtr saveport;
	static int mode = 1;
	static int paramindex;
	static const int maxparams = 16;
	static int params[maxparams];
	static int tempindex;
	static unsigned int attrib = 0;
	static int unknown = -1;
	char buf[256];

	if (SIOUXQuitting)
		return 0;

	GetPort(&saveport);
	SetPortWindowPort(SIOUXTextWindow->window);	/* ra 990612 UI 3.2 */

	/*plnt_dump("WriteCharsToConsole", buffer, n);*/

	SIOUXState = PRINTFING;
	SIOUXUpdateStatusLine(SIOUXTextWindow->window); /* ra 990612 Use WindowPtr */

	for(counter = n; counter > 0; counter--)
	{
		aChar = *buffer++;
		switch (mode) {
			case 1:
				switch(aChar) 
				{
					case 0x0d:	
						#if !__option(mpwc_newline)		/*	Line Feed (Mac newline)*/
							INSERTLINEFEED();
							break;
							
						#else	/*	Carriage Return (move to start of line)*/
							i = OffsetOnCurrentLine(SIOUXTextWindow->edit, 1);	
							if (i <= CURRENTBUFSIZE()) 
							{
								ROLLBACKBUFFER(i);
							} 
							else 
							{
								SIOUXBuffer.tepos = (*SIOUXTextWindow->edit)->teLength - (i - CURRENTBUFSIZE());
							}
							break;
						#endif
						
					case 0x0a:	
						#if !__option(mpwc_newline)	/*	Carriage Return (move to start of line)*/
							if (convertLFs || (counter < n && buffer[-2] != 0x0d)) {
								INSERTLINEFEED();
							}
							i = OffsetOnCurrentLine(SIOUXTextWindow->edit, 1);
							if (i <= CURRENTBUFSIZE()) 
							{
								ROLLBACKBUFFER(i);
							} 
							else 
							{
#if SIOUX_USE_WASTE
								SIOUXBuffer.tepos = WEGetTextLength( SIOUXTextWindow->edit ) - (i - CURRENTBUFSIZE());
#else
								SIOUXBuffer.tepos = (*SIOUXTextWindow->edit)->teLength - (i - CURRENTBUFSIZE());
#endif /* SIOUX_USE_WASTE */
		                    ROLLBACKBUFFER(CURRENTBUFSIZE());  /*mm 980109*/					
		                    }
							break;
						#else		/*	Line Feed (Mac newline)*/
							INSERTLINEFEED();
							break;			
						#endif
						
					case 0x1b:	/*	Start escape mode*/
					    if ( gSIOUXTerminalMode ) {
							// need to flush data
							InsertSIOUXBuffer();
							mode = 2;
							{
								int len = counter-1;
								if (len > 10)
									len = 10;
								/*plnt_printf("### Start escape mode : %.*s\n", len, buffer);*/
							}
						}
						break;

					case '\t':	/*	Tab character*/
						if (SIOUXSettings.tabspaces) 
						{
							/*	insert spaces for tabs*/
							CHECKFOROVERFLOW(SIOUXSettings.tabspaces);

							i = OffsetOnCurrentLine(SIOUXTextWindow->edit, 1);

							spacestoinsert = SIOUXSettings.tabspaces -
											 (i % SIOUXSettings.tabspaces);
							for (i = 0; i < spacestoinsert; i++) INSERTCHAR(' ');
						} 
						else
							INSERTCHAR('\t');
						break;
						
					case '\f':	/*	Form Feed*/
						CHECKFOROVERFLOW(SIOUXTextWindow->linesInFolder);
						for (i = SIOUXTextWindow->linesInFolder; i > 0; i--) INSERTLINEFEED();
						break;
						
					case '\a':	/*	Audible Alert*/
						SysBeep(1);
						break;
						
					case '\b':	/*	Backspace*/
						if (CURRENTBUFSIZE() != 0) 
						{
							/*DELETEFROMBUFFER(1);           mm 970212 */
						    SIOUXBuffer.curpos -= 1;      /* mm 970212 */
						    SIOUXBuffer.endpos -= 1;      /* mm 981210 */
						} 
						else 
						{				/*	Need to delete the last character from the TextEdit Handle*/
#if SIOUX_USE_WASTE
							long teLength = WEGetTextLength( SIOUXTextWindow->edit );
#else
							short teLength = (*SIOUXTextWindow->edit)->teLength;
#endif /* SIOUX_USE_WASTE */
							if (teLength > 0) 
							{
#if SIOUX_USE_WASTE
								OSErr err;
								WESetSelection(teLength-1, teLength, SIOUXTextWindow->edit);
								err = WEDelete(SIOUXTextWindow->edit);
#else
								TESetSelect(teLength-1, teLength, SIOUXTextWindow->edit);
								TEDelete(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
							}
						}
						break;
						
					case '\v':	/*	Vertical Tab*/
						break;
						
					default:	/*	just add it to SIOUX ...*/
						INSERTCHAR(aChar);
						break;
				}
				CHECKFOROVERFLOW(0);
				break;

			case 2:
				switch (aChar) {
					case '[':
						paramindex = maxparams;
						while (--paramindex >= 0)
							params[paramindex] = -1;
						paramindex = 0;
						mode = 3; // go to control sequence processing mode
						break;
					default:
						/*plnt_printf("### unknown escape seq : %c\n", aChar);*/
						mode = 1; // back to plain input
						break;
				}
				break;

			case 3: // control sequence processing
				if ( aChar >= '0' && aChar <= '9' ) {
					if ( params[paramindex] == -1 )
						params[paramindex] = 0;
					params[paramindex] *= 10;
					params[paramindex] += aChar - '0';
					break;
				}
				mode = 1; // back to plain input by default

				switch (aChar) {

					case '?': /* ???????????? */
						unknown = -1;
						mode = 4; // ??? 25l, 25h ???
						break;

					case ';': /* parameter separator */
						paramindex++;
						mode = 3; // remain in current mode
						break;

					case 'A': /* cursor up */
						/*plnt_printf("cursor up\n");*/
						break;

					case 'B': /* cursor down */
						/*plnt_printf("cursor down\n");*/
						break;

					case 'C': /* cursor right */
						/*plnt_printf("cursor right\n");*/
						break;

					case 'c': /* request identification */
						/*plnt_printf("request identification\n");*/
						break;

					case 'D': /* cursor left */
						/*plnt_printf("cursor left\n");*/
						break;

					case 'f':
					case 'H': /* absolute cursor positioning */
						{
							int ypos, xpos;
							SInt32 lineStart, lineEnd;
							SInt32 topLineNumber;
							SInt32 maxLineNumber;
							SInt32 length;
							SInt32 offset;
							int crcount;
							int spacecount;

							ypos = params[0] - 1;
							if ( ypos < 0 )
								ypos = 0;
							else if ( ypos >= SIOUXSettings.rows )
								ypos = SIOUXSettings.rows - 1;

							xpos = params[1] - 1;
							if ( xpos < 0 )
								xpos = 0;
							else if ( xpos >= SIOUXSettings.columns )
								xpos = SIOUXSettings.columns - 1;

							/*plnt_printf("absolute cursor positioning to (x:%d,y:%d)\n", xpos, ypos);*/

							topLineNumber = CurrentTopLine(SIOUXTextWindow->edit);
							length = WEGetTextLength( SIOUXTextWindow->edit );
							if (length > 0) {
								maxLineNumber = WECountLines(SIOUXTextWindow->edit);
								if (WEGetChar( length-1, SIOUXTextWindow->edit ) == 0x0d ) {
									maxLineNumber++;
								}
							} else {
								maxLineNumber = 0;
							}

							/*plnt_printf("topLineNumber:%d, maxLineNumber:%d\n", topLineNumber, maxLineNumber);*/
							crcount = ypos - (maxLineNumber - topLineNumber);
							if (crcount > 0) {
								/*plnt_printf("need to add %d crs\n", crcount);*/
								WESetSelection( length-1, length-1, SIOUXTextWindow->edit );
								for (i = 0; i < crcount; i++) {
									buf[i] = 0x0d;
								}
								WEInsert(buf, crcount, nil, nil, SIOUXTextWindow->edit);
								maxLineNumber += crcount;
								lineStart = length + crcount - 1;
								lineEnd = lineStart;
							} else {
								WEGetLineRange(topLineNumber + ypos, &lineStart, &lineEnd, SIOUXTextWindow->edit );
							}
							/*plnt_printf("line %d begins at %d\n", topLineNumber + ypos, lineStart);*/

							offset = 0;
							spacecount = 0;
							length = WEGetTextLength( SIOUXTextWindow->edit );
							while ( offset < xpos ) {
								if ( lineStart + offset >= length
								 || WEGetChar( lineStart + offset, SIOUXTextWindow->edit ) == 0x0d ) {
									// need to add spaces...
									spacecount = xpos - offset;
									break;
								}
								offset++;
							} 
							if (spacecount) {
/*
								if (lineEnd == length
								 && lineStart == lineEnd - 1
								 && WEGetChar( lineEnd-1, SIOUXTextWindow->edit ) == 0x0d) {
									offset++;
								}
*/
								/*plnt_printf("need to add %d spaces at %d\n", spacecount, lineStart+offset);*/
								WESetSelection( lineStart+offset, lineStart+offset, SIOUXTextWindow->edit );
								for (i = 0; i < spacecount; i++) {
									buf[i] = ' ';
								}
								WEInsert(buf, spacecount, nil, nil, SIOUXTextWindow->edit);
							} else {
								WESetSelection( lineStart+offset, lineStart+offset, SIOUXTextWindow->edit );
							}


							maxLineNumber = WECountLines(SIOUXTextWindow->edit) - 1;
							WEGetLineRange(maxLineNumber, &lineStart, &lineEnd, SIOUXTextWindow->edit );
							/*plnt_printf("line max %d begins at %d\n", maxLineNumber, lineStart);*/

						}
						break;

					case 'J': /* erase to beginning/end/whole of screen */
						switch (params[0]) {
							case -1:
							case  0: /* erases characters from cursor to end of screen */
								/*plnt_printf("erases characters from cursor to end of screen\n");*/
								{
#if SIOUX_USE_WASTE
									SInt32 weSelStart, weSelEnd, lastCharIndex;
									OSErr err;
									WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
									lastCharIndex = WEGetTextLength( SIOUXTextWindow->edit );
									WESetSelection( weSelStart, lastCharIndex, SIOUXTextWindow->edit );
									err = WEDelete(SIOUXTextWindow->edit);
#else
									SInt32 weSelStart, weSelEnd, lastCharIndex;
									weSelStart = (*SIOUXTextWindow->edit)->selStart;
									weSelEnd = (*SIOUXTextWindow->edit)->selEnd;
									lastCharIndex = (*SIOUXTextWindow->edit)->teLength;
									TESetSelect( weSelStart, lastCharIndex, SIOUXTextWindow->edit );
									TEDelete(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
								}
								break;
							case  1: /* erases characters from beginning of screen to cursor */
								/*plnt_printf("erases characters from beginning of screen to cursor\n");*/
								break;
							case  2: /* clears the screen */
								/*plnt_printf("clears the screen\n");*/
								{
#if SIOUX_USE_WASTE
									SInt32 weSelStart, weSelEnd, lastCharIndex;
									OSErr err;
									//WEGetLineRange(CurrentTopLine(SIOUXTextWindow->edit), &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
									WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
									lastCharIndex = WEGetTextLength( SIOUXTextWindow->edit );
									WESetSelection( weSelStart, lastCharIndex, SIOUXTextWindow->edit );
									err = WEDelete(SIOUXTextWindow->edit);
#else
									SInt32 weSelStart, weSelEnd, lastCharIndex;
									weSelStart = (*SIOUXTextWindow->edit)->selStart;
									weSelEnd = (*SIOUXTextWindow->edit)->selEnd;
									lastCharIndex = (*SIOUXTextWindow->edit)->teLength;
									TESetSelect( weSelStart, lastCharIndex, SIOUXTextWindow->edit );
									TEDelete(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
								}
								break;
						}
						break;

					case 'K': /* erase to beginning/end/whole of line */
						switch (params[0]) {
							case -1:
							case  0: /* erases characters to the end of the current line */
								/*plnt_printf("erases characters to the end of the current line\n");*/
								{
#if SIOUX_USE_WASTE
									SInt32 weSelStart, weSelEnd, lastCharIndex;
									OSErr err;
									WEGetSelection( &weSelStart, &weSelEnd, SIOUXTextWindow->edit );
									lastCharIndex = WEGetTextLength( SIOUXTextWindow->edit ) - 1;
									i = weSelStart;
									while ( i < lastCharIndex && WEGetChar( i, SIOUXTextWindow->edit ) != 0x0d )
										++i;
									weSelEnd = i+1;
									WESetSelection( weSelStart, weSelEnd, SIOUXTextWindow->edit );
									err = WEDelete(SIOUXTextWindow->edit);
#else
									SInt32 weSelStart, weSelEnd, lastCharIndex;
									char *ptr;
									weSelStart = (*SIOUXTextWindow->edit)->selStart;
									weSelEnd = (*SIOUXTextWindow->edit)->selEnd;
									lastCharIndex = (*SIOUXTextWindow->edit)->teLength - 1;
									i = weSelStart;
									ptr = *(*SIOUXTextWindow->edit)->hText;
									while ( i < lastCharIndex && ptr[i] != 0x0d )
										++i;
									weSelEnd = i+1;
									TESetSelect( weSelStart, weSelEnd, SIOUXTextWindow->edit );
									TEDelete(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
								}
								break;
							case  1: /* erases characters from beginning of line to cursor */
								/*plnt_printf("erases characters from beginning of line to cursor\n");*/

								break;
							case  2: /* erases the specified line */
								/*plnt_printf("erases the specified line\n");*/
								break;
						}
						break;

					case 'L': /* ???????????? */
						break;

					case 'm': /* set/clear attributes */
						tempindex = -1;
						while (++tempindex <= paramindex) {
							if ( params[tempindex] == -1 )
								params[tempindex] = 0;
							if (params[tempindex] == 0) {
								attrib &= 0x80; // all off
	      					} else if (params[tempindex] < 8) {
	      						attrib |= 1 << (params[tempindex] - 1); // set an attribute
							} else if (params[tempindex] > 21 && params[tempindex] < 28)  {
								attrib &= ~(1 << (params[tempindex] - 21)); // clear an attribute
							}
						}
						break;

					case 'q': /* flash dem LEDs. What LEDs? */
						break;

					case 'r': /* set scrolling region */
						{
							int top, bottom;
							top = params[0];
							if ( top < 1 )
								top = 1;
							else if ( top > SIOUXSettings.rows )
								top = SIOUXSettings.rows;
							bottom = params[1];
							if ( bottom < 1 || bottom > SIOUXSettings.rows )
								bottom = SIOUXSettings.rows;
							/*plnt_printf("set scrolling region to (%d,%d)\n", top, bottom);*/
						}
						break;

					default:
						/*plnt_printf("### unknown control seq : %c\n", aChar);*/
						break;
				}
				break;

			case 4: // ??? 25l, 25h ???
				if ( aChar >= '0' && aChar <= '9' ) {
					if ( unknown == -1 )
						unknown = 0;
					unknown *= 10;
					unknown += aChar - '0';
					break;
				}
				mode = 1; // back to plain input by default
				switch (aChar) {
					case 'l':
						// turn something on ???
						break;
					case 'h':
						// turn something off ???
						break;
				}
				break;

		}
	}
	InsertSIOUXBuffer();

	SIOUXState = IDLE;
	SIOUXUpdateStatusLine(SIOUXTextWindow->window); /* ra 990612 Use WindowPtr */

	SetPort(saveport);

	return n;
}

/************************************************************************/
/*	Purpose..: 	Read characters into the buffer							*/
/*	Input....:	pointer to buffer										*/
/*	Input....:	max length of buffer									*/
/*	Return...:	number of characters read / -1 error occurred			*/
/************************************************************************/
long ReadCharsFromConsole(char *buffer, long n)
{
	long charsread;
	GrafPtr saveport;
#if SIOUX_USE_WASTE
	Handle textHandle;
#endif /* SIOUX_USE_WASTE */
	OSErr err;

	if (SIOUXQuitting)
		return 0;

	if (atEOF) 
	{
		if (gSIOUXTerminalMode)
			atEOF = 0;
		buffer[0] = EOF;
		return 0;
	}

	GetPort(&saveport);
	SetPortWindowPort(SIOUXTextWindow->window);	/* ra 990612 Use UI 3.2 and WindowPtr */

	SIOUXState = SCANFING;
	inputBuffersize = n;
#if SIOUX_USE_WASTE
	SIOUXselstart = WEGetTextLength( SIOUXTextWindow->edit );
#else
	SIOUXselstart = (*SIOUXTextWindow->edit)->teLength;
#endif /* SIOUX_USE_WASTE */

	SelectWindow(SIOUXTextWindow->window);	/* ra 990612 Use WindowPtr */
	SIOUXUpdateStatusLine(SIOUXTextWindow->window);
	
	if (!gSIOUXTerminalMode) {
#if SIOUX_USE_WASTE
		WESetSelection(SIOUXselstart, SIOUXselstart, SIOUXTextWindow->edit);
		WEActivate(SIOUXTextWindow->edit);
#else
		TESetSelect(SIOUXselstart, SIOUXselstart, SIOUXTextWindow->edit);
		TEActivate(SIOUXTextWindow->edit);
#endif /* SIOUX_USE_WASTE */
	} else {

	}
	
	SIOUXUpdateScrollbar();

	while (SIOUXState == SCANFING && !SIOUXQuitting) 
	{
BackInTheLoop:
		SIOUXHandleOneEvent(NULL);
	}
		
	if (SIOUXQuitting) 
	{
		if (!SIOUXCleanUp()) 
		{
			SIOUXQuitting = false;
			goto BackInTheLoop;
		}
		SetPort(saveport);
		exit(0);
	}

	if (!gSIOUXTerminalMode) {

		/*	put the string into the buffer ...*/
#if SIOUX_USE_WASTE
		charsread = WEGetTextLength(SIOUXTextWindow->edit ) - SIOUXselstart;
		textHandle = WEGetText(SIOUXTextWindow->edit);
		HLock( textHandle );
		BlockMoveData( *textHandle + SIOUXselstart, buffer, charsread);
		HUnlock( textHandle );

#else
		charsread = (*SIOUXTextWindow->edit)->teLength - SIOUXselstart;
#ifndef __NO_WIDE_CHAR														/* mm 981020 */
		if (fwide(stdin, 0) <= 0)                                        		/* mm 980208 */
#endif /* #ifndef __NO_WIDE_CHAR */											/* mm 981020 */
		BlockMoveData((*(*SIOUXTextWindow->edit)->hText) + SIOUXselstart,
				  buffer,
				  charsread);
#ifndef __NO_WIDE_CHAR														/* mm 981020 */
		else																	/* mm 980208 */
		{																		/* mm 980208 */
			char tempbuf[256];
			long count;													        /* mm 980208 */
			BlockMoveData((*(*SIOUXTextWindow->edit)->hText) + SIOUXselstart,	/* mm 980208 */
				  tempbuf,														/* mm 980208 */
				  charsread);													/* mm 980208 */
			for (count = 0; count <= charsread; count++)						/* mm 980208 */
			{																	/* mm 980208 */
				buffer[2*count] = '\0';											/* mm 980208 */
				buffer[2*count+1] = tempbuf[count];								/* mm 980208 */
			}																	/* mm 980208 */
			
			charsread *= 2;														/* mm 980208 */
		}																		/* mm 980208 */
#endif /* #ifndef __NO_WIDE_CHAR */											/* mm 981020 */

#endif /* SIOUX_USE_WASTE */
		err = MemError();

	} else {

		/* get data from blind buffer */
		if ( gSIOUXBufSize ) {
			charsread = gSIOUXBufSize;
			if (charsread > n) {
				charsread = n;
			}
			BlockMoveData(gSIOUXChars, buffer, charsread);
			gSIOUXBufSize -= charsread;
			if ( gSIOUXBufSize ) {
				BlockMoveData( gSIOUXChars + charsread, gSIOUXChars, gSIOUXBufSize );
			}
			err = noErr;
		} else {
			err = -1;
		}
	}

	/*	if no error occurred continue else return 0 characters read ...*/
	if (err == noErr) 
	{
		if (buffer[charsread - 1L] == 0x03)	/* The user did a Control - Z or control - D (ie an EOF) */
			charsread--, atEOF = 1;
		else if (!gSIOUXTerminalMode)
			buffer[charsread - 1L] = 0x0d;
	} 
	else 
	{
		charsread = 0;
	}

	SIOUXUpdateStatusLine(SIOUXTextWindow->window);	/* ra 990612 Use WindowPtr */

	SetPort(saveport);

	/*plnt_dump("ReadCharsFromConsole", buffer, charsread);*/

	return charsread;
}

/*
 *	return the name of the current terminal ...
 */
char *__ttyname(long fildes)
{
	/*	all streams have the same name ...*/
	static char *__SIOUXDeviceName = "SIOUX";
	
	if (fildes >= 0 && fildes <= 2)
		return (__SIOUXDeviceName);

	return (NULL);
}

/*
 *	Set SIOUX's window title ...
 */
void SIOUXSetTitle(unsigned char title[256])
{

	if (SIOUXTextWindow != NULL)
		SetWTitle(SIOUXTextWindow->window, title);	/* ra 990612 Use WindowPtr */
	else   /* mm 980609 */
	{
		SIOUXSettings.userwindowtitle = malloc(title[0]+1);
		if (SIOUXSettings.userwindowtitle != NULL)
			BlockMoveData(title, SIOUXSettings.userwindowtitle, (long)title[0] + 1);
	}
}

/*
*
*    int kbhit()
*
*    returns true if any keyboard key is pressed without retrieving the key
*    used for stopping a loop by pressing any key
*/
int kbhit(void)
{
      EventRecord event;  
      return EventAvail(keyDownMask,&event); 
}

/*
*
*    int getch()
*
*    returns the keyboard character pressed when an ascii key is pressed  
*    used for console style menu selections for immediate actions.
*/
int getch(void)
{
   int c;
   EventRecord event;
   fflush(stdout);
         /* Allow SIOUX response for the mouse, drag, zoom, or resize. */
	while(!GetNextEvent(keyDownMask,&event)) 
	{
		if (GetNextEvent(updateMask | osMask | mDownMask | mUpMask, &event)) /* mm 980506 */
			SIOUXHandleOneEvent(&event);
	}
	c = event.message&charCodeMask;
	if (c == '.' && (event.modifiers&cmdKey))
		exit(1);
  
   return c;
}

/*
*     void clrscr()
*
*     clears screen  and empties buffer.
*/
void clrscr(void)                   /* mm 980427 */  /* mm 981218 */
{
	EventRecord rEvent;
 
	fflush(stdout);
	rEvent.what 	 = keyDown;
	rEvent.when 	 = TickCount( );
	rEvent.message 	 = 'a';
	rEvent.modifiers = cmdKey;	
	SIOUXHandleOneEvent(&rEvent);

	rEvent.what 	 = keyDown;
	rEvent.when 	 = TickCount( );
	rEvent.message 	 = 0x7F;	
	rEvent.modifiers = 0;
	SIOUXHandleOneEvent(&rEvent);
}



/*  Change Record
//	BB 01/10/93 removed diskEvt from switch statement since this called
//				DIBadMount which is not glue code and hence required importing
//				MacOS.lib ...
//	BB 21/01/94 removed the direct call to GrowDrawIcon and replaced it
//				with a call to a clipping function which doesn't draw the
//				lines.
//	BB 25/01/94 Added support for command - '.', also changed calls to
//				ExitToShell() to exit() which allows the ANSI libs to close
//				any open file streams.
//	BB 25/01/94 Added support for tab characters.
//	BB 22/07/94	Added support for EOF, through control - z
//	BB 11/09/94 Added support for control - c, also both control-c and command-.
//				call raise SIGABRT rather than calling exit.
//				Added support for mouse down to pause output.
//				Added support for setting tab behaviour in SIOUX.
//				Fixed support for characters >128
//	BB 20/10/94	Fixed behaviour of replacing tabs for spaces ...
//	BB 25/10/94	Added support for '\r' to move cursor to beginning of line
//	BB 26/10/94 Changed command-'.' and control - c to raise SIGINT instead of
//				SIGABRT.
//	BB 01/12/94	Extended EOF support to include control - d
//	JH 01/09/95 Modified to run with new ANSI C library
//	JH 12/10/95 Moved __system7present() back to unix.c
//	JH 29/01/96 Added missing return in SIOUXHandleOneEvent's keyDown/autoKey
//				Cmd-key handling
//  JH 19/02/96 Eliminated local definition of EOF.
//	bk 09/02/96 added Universal Headers incase macheaders not the prefix
//	bk 961228	line 752-772 switched LF/CR if __option(mpwc_newline)
//  mm 970212   Changed backspace to just move cursor when text still in buffer.
//  mm 970626   Modification to allow correct function of '\r'
//  mm 970903   Correction to calculation of OffsetOnCurrentLine  MW00396 
//  mm 971006  Added #include of Sound.h because of change in universal headers
//  mm 971229  Corrected semantics of \r in errors revealed by MW03003
//  mm 971230  Changes to OffsetOnCurrentLine for WASTE to correct semantics of \r in errors revealed by MW03003
//  mm 980108  Change to InstallConsole to correct behaviour of \r on first line of output
//  mm 980109  A further fix to \r
//  mm 980112  Yet another fix to \r
//  mm 980208  Support for reading wide characters from console for wscanf.  Not yet done for WASTE
//  mm 980331  Fix to insert linefeed to make the sequence \b\n work correctly.
//  mm 980413  Fix to make the enter key work as return on input on a PowerBook where its virtual key code is 0x34.
//  mm 980427  Added SIOUXclrscr   MW06847
//  mm 980506  Modified getch() so that the window is repainted if necessary. MW03278
//  mm 980605a Change from Michael Bishop to make SIOUX only select windows it knows about MW06855
//  mm 980605b Change from Michael Bishop to make SIOUX select windows when user clicks in drag or grow region 
//             if it's not the FrontWindow.   MW06856
//  mm 980605c Change from Michael Bishop to return false from SIOUXHandleOneEvent() so that an app that embeds
//             SIOUX will be able to process a NULL event as well.  MM06857
//  mm 980609  Changes that allow user to specify window title before the SIOUX window is created.
// vss 980629  moved variable into code block that uses it to remove warning
//  mm 981020  Added  #ifndef __NO_WIDE_CHAR wrappers
//  mm 981210  Corrected backspace behaviour for stderr (non-buffered) MW08661
//  mm 981218  Changed name SIOUXclrscr to clrscr MW08237
//	cc 991108  added ra Carbon Changes done 990611
//  cc 991109  changed TARGET_CARBON to TARGET_API_MAC_CARBON
//  cc 991111  added ra & JWW suggestions for functions no longer in carbon
//  cc 991115  updated and deleted carbon outdated comments
// JWW 000413  Call DisposeWindow or CloseWindow -- never both
//  cc 000516  __myraise is now an inline in unix.h
// JWW 000531  Added Apple Event handlers for standalone application (necessary to catch Quit on OS X, but also nice to have overall)
*/
/*
 * About.c
 */

#include <Gestalt.h>
#include <QDOffscreen.h>
#include <Sound.h>
#include <TextEdit.h>

#include "url.proto.h"

#define WHITE_ON_BLACK 0

enum  {
	ditl_about_background = 1,	// background about item
	ditl_about_creditsbox,		// credits
	ditl_about_versionbox,		// version number
	ditl_about_infobox			// info
};

#define 	rWINDAbout			1000
#define 	rDITLAbout			1000
#define		rTEXTCredits		1000
#define 	rPICTBackground 	1025
#define 	rSTR_Info		 	1000

#define 	Fade_Levels 		12

#define 	Scroll_Ticks 		2

#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=mac68k
#endif

typedef struct UserItemRec	{
	long padding1;
	Rect box;
	short padding2;
} UserItemRec;

typedef struct DITLRec	{
	short numItems;
	UserItemRec theUserItems[1];
} DITLRec, **DITLHand;


typedef struct CreditsRec  {
	GWorldPtr theGWorldP;
	Rect viewRect;
	Rect pictRect;
	short pictHeight;
} CreditsRec;

#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=reset
#endif


typedef enum AnimateState  {
	animate_Waiting,
	animate_Active,
	animate_Done
} AnimateState;

#ifdef __cplusplus
extern "C" {
#endif

void DoAboutBox();
OSErr SetUpGraphics(Rect *urlBox, Str255 urlString);
void DoAnimation(void);
void CleanUpGraphics(void);
OSErr GetItemRect(short inItem, Rect *outRect);
OSErr DrawPictIntoNewGWorld(short inPICTid, short inDepth,
						   GWorldPtr *outGWorldP);
OSErr DrawTextIntoNewGWorld(short inTEXTid, Rect *inBounds, short inDepth,
						   GWorldPtr *outGWorldP);
OSErr SetUpCredits(CreditsRec *inCreditsRec);

#ifdef __cplusplus
}
#endif

static short Pixel_Depth;
static CWindowPtr aboutBoxWindow;
static PixMapPtr windowPixMapP;
static GWorldPtr drawWorldP = NULL;
static PixMapPtr drawPixMapP = NULL;
static GWorldPtr backWorldP = NULL;
static PixMapPtr backPixMapP;

void DoAboutBox()
{
	long qdVersion;
	Rect urlBox;
	Str255 urlString;
	Point mouseLoc;

	PurgeMem(maxSize);
	Gestalt(gestaltQuickdrawVersion, &qdVersion);
	if ((qdVersion < gestalt32BitQD) || (FreeMem() < 500000)) {
		SysBeep(6);
	} else {
		GWorldPtr saveWorld;
		GDHandle saveDevice;
		GetGWorld(&saveWorld, &saveDevice);
		if (!SetUpGraphics(&urlBox, urlString)) {
			DoAnimation();
			GetMouse(&mouseLoc);
			CleanUpGraphics();
			PurgeMem(maxSize);
		}
		SetGWorld(saveWorld, saveDevice);
		if (PtInRect(mouseLoc, &urlBox)) {
			HandleURLString(urlString);
		}
	}
}

OSErr SetUpGraphics(Rect *urlBox, Str255 urlString)
{
	OSErr err;
	Rect backRect;
	PixMapHandle thePixMapH;
	Rect	versionBox;
	VersRecHndl version;
	Str255 versionString;

	Pixel_Depth = 16;

	//	Make GWorld for offscreen drawing.

	err = GetItemRect(ditl_about_background, &backRect);
	if (err)
		return err;

	err = NewGWorld(&drawWorldP, Pixel_Depth, &backRect, nil, nil, 0);
	if (err)
		return err;

	thePixMapH = GetGWorldPixMap(drawWorldP);
	HLockHi((Handle)thePixMapH);
	if ( !LockPixels(thePixMapH) ) {
		err = -1;
		goto failed;
	}
	drawPixMapP = *thePixMapH;

	//	Make GWorld for background.

	err = DrawPictIntoNewGWorld(rPICTBackground, Pixel_Depth, &backWorldP);
	if ( err )
		goto failed;

	thePixMapH = GetGWorldPixMap(backWorldP);
	HLockHi((Handle)thePixMapH);
	if ( !LockPixels(thePixMapH) ) {
		err = -1;
		goto failed;
	}
	backPixMapP = *thePixMapH;

	//	Make Window.

	aboutBoxWindow = (CWindowPtr) GetNewCWindow(rWINDAbout, nil, (WindowPtr) - 1L);
	ShowWindow((WindowPtr)aboutBoxWindow);
	
	HLockHi((Handle)aboutBoxWindow->portPixMap);
	windowPixMapP = *aboutBoxWindow->portPixMap;

	//	Draw background in Window.

	SetGWorld(aboutBoxWindow, GetMainDevice());
	CopyBits((BitMapPtr)backPixMapP, (BitMapPtr)windowPixMapP, &backRect,
			 &backRect, srcCopy, nil);

	//	Draw version string.

	GetItemRect(ditl_about_versionbox, &versionBox);
	if ((version = (VersRecHndl) GetResource('vers',1)) != nil) {
		BlockMoveData((*version)->shortVersion+*(*version)->shortVersion+1, versionString, *((*version)->shortVersion+*(*version)->shortVersion+1)+1);
		ReleaseResource((Handle)version);
	} else {
		*versionString = 0;
	}

	TextFont( 3);
	TextSize( 9);
	MoveTo( versionBox.left +((versionBox.right-versionBox.left)/2)-(StringWidth(versionString)/2), versionBox.bottom-2);
	DrawString(versionString);

	//	Draw info string.

	GetItemRect(ditl_about_infobox, urlBox);
	GetIndString(urlString, rSTR_Info, 1);
	MoveTo( urlBox->left +((urlBox->right-urlBox->left)/2)-(StringWidth(urlString)/2), urlBox->bottom-4);
	TextFace( underline);
	DrawString(urlString);
	TextFace( 0);

	return noErr;
failed:
	if (drawWorldP) {
		DisposeGWorld(drawWorldP);
		drawWorldP = NULL;
	}
	if (backWorldP) {
		DisposeGWorld(backWorldP);
		backWorldP = NULL;
	}
	if (aboutBoxWindow) {
		DisposeWindow((WindowPtr)aboutBoxWindow);
		aboutBoxWindow = NULL;
	}
	return err;
}

OSErr SetUpCredits(CreditsRec *inCreditsRec)
{
	OSErr err;
	PicHandle pictureH;
	Rect picFrame;
	PixMapHandle thePixMapH;
	Rect	creditsBox;

	//	The only animation is the credits
	//	Therefore, we can reduce the size of the offscreen worlds for the background
	//	and scratch drawing to just cover the area where the credits are drawn.

	DisposeGWorld(backWorldP);
	backWorldP = NULL;
	DisposeGWorld(drawWorldP);
	drawWorldP = NULL;

	err = GetItemRect(ditl_about_creditsbox, &inCreditsRec->viewRect);
	if (err)
		return;

	//	Make GWorld for offscreen drawing.

	err = NewGWorld(&drawWorldP, Pixel_Depth, &inCreditsRec->viewRect, nil, nil, 0);
	if (err)
		return;
	thePixMapH = GetGWorldPixMap(drawWorldP);
	HLockHi((Handle)thePixMapH);
	if ( !LockPixels(thePixMapH) ) {
		err = -1;
		goto failed;
	}
	drawPixMapP = *thePixMapH;

	//	Make GWorld for background.

	err = NewGWorld(&backWorldP, Pixel_Depth, &inCreditsRec->viewRect, nil,
			  GetGWorldDevice(drawWorldP), noNewDevice);
	if (err)
		goto failed;
	thePixMapH = GetGWorldPixMap(backWorldP);
	HLockHi((Handle)thePixMapH);
	if ( !LockPixels(thePixMapH) ) {
		err = -1;
		goto failed;
	}
	backPixMapP = *thePixMapH;
	SetGWorld(backWorldP, nil);
	pictureH = GetPicture(rPICTBackground);
	picFrame = (**pictureH).picFrame;
	DrawPicture(pictureH, &picFrame);
	ReleaseResource((Handle)pictureH);

	//	Make GWorld for credits.
	
	creditsBox = inCreditsRec->viewRect;
	err = DrawTextIntoNewGWorld(rTEXTCredits, &creditsBox,
			Pixel_Depth, &inCreditsRec->theGWorldP);
	if (err)
		goto failed;
	inCreditsRec->pictRect.left = 0;
	inCreditsRec->pictRect.top = 0;
	inCreditsRec->pictRect.right = inCreditsRec->viewRect.right -
		inCreditsRec->viewRect.left;
	inCreditsRec->pictRect.bottom = inCreditsRec->viewRect.bottom -
		inCreditsRec->viewRect.top;
		
	inCreditsRec->pictHeight = creditsBox.bottom;
	return noErr;
failed:
	if (backWorldP) {
		DisposeGWorld(backWorldP);
		backWorldP = NULL;
	}
	if (drawWorldP) {
		DisposeGWorld(drawWorldP);
		drawWorldP = NULL;
	}
	return err;
}

void DoAnimation(void)
{
	OSErr err = noErr;
	CreditsRec theCreditsRec;

	AnimateState animCreditsFade = animate_Active;
	AnimateState animCreditsScroll = animate_Waiting;

	Boolean logoHalfDrawn = false;
	Boolean logoWholeDrawn = false;

	RGBColor fadeColor;
#if WHITE_ON_BLACK
	short grayIndex = 0;
#else
	short grayIndex = Fade_Levels;
#endif
	unsigned short grayLevels[Fade_Levels] =  {
		4369, 8738, 17476, 21845, 30583, 34952, 43690, 48059, 52428, 56979,
		61166, 65535
	};

	theCreditsRec.theGWorldP = nil;

	animCreditsFade = animate_Active;

	if (SetUpCredits(&theCreditsRec))
		return;

	while (!Button() && err == noErr) {
		unsigned long ticks;
		long endTicks;
		long startTicks = TickCount();

#if WHITE_ON_BLACK
		if ((animCreditsFade == animate_Active) && (grayIndex >= Fade_Levels)) {
			animCreditsFade = animate_Done;
			Delay(90, &ticks);
			animCreditsScroll = animate_Active;
		}
#else
		if ((animCreditsFade == animate_Active) && (grayIndex <= 0)) {
			animCreditsFade = animate_Done;
			Delay(90, &ticks);
			animCreditsScroll = animate_Active;
		}
#endif
		if ((animCreditsScroll == animate_Active) &&
			(theCreditsRec.pictRect.top > theCreditsRec.pictHeight)) {
			theCreditsRec.pictRect.top = 1;
			theCreditsRec.pictRect.bottom = theCreditsRec.viewRect.bottom -
				theCreditsRec.viewRect.top + 1;
		}
		if (animCreditsFade == animate_Active) {
			PixMapHandle thePixMapH;

			Delay(Scroll_Ticks, &ticks);
			SetGWorld(drawWorldP, nil);

			thePixMapH = GetGWorldPixMap(theCreditsRec.theGWorldP);
			if ( LockPixels(thePixMapH) ) {
				CopyBits((BitMapPtr)(*thePixMapH), (BitMapPtr)drawPixMapP,
					 &theCreditsRec.pictRect, &theCreditsRec.viewRect,
					 srcCopy, nil);
				UnlockPixels(thePixMapH);
			}

#if WHITE_ON_BLACK
			fadeColor.red = fadeColor.blue = fadeColor.green =
				grayLevels[grayIndex++];
#else
			fadeColor.red = fadeColor.blue = fadeColor.green =
				grayLevels[--grayIndex];
#endif
			RGBForeColor(&fadeColor);
#if WHITE_ON_BLACK
			PenMode(adMin);
#else
			PenMode(adMax);
#endif
			PaintRect(&theCreditsRec.viewRect);
			PenMode(patCopy);
			ForeColor(blackColor);

#if WHITE_ON_BLACK
			CopyBits((BitMapPtr)backPixMapP, (BitMapPtr)drawPixMapP,
					 &theCreditsRec.viewRect, &theCreditsRec.viewRect, adMax,
					 nil);
#else
			CopyBits((BitMapPtr)backPixMapP, (BitMapPtr)drawPixMapP,
					 &theCreditsRec.viewRect, &theCreditsRec.viewRect, adMin,
					 nil);
#endif

			SetGWorld(aboutBoxWindow, GetMainDevice());
			CopyBits((BitMapPtr)drawPixMapP, (BitMapPtr)windowPixMapP,
					 &theCreditsRec.viewRect, &theCreditsRec.viewRect,
					 srcCopy, nil);
		}
		if (animCreditsScroll == animate_Active) {
			PixMapHandle thePixMapH;
			short i, j;
			short creditsWidth =
				theCreditsRec.viewRect.right -theCreditsRec.viewRect.left -1;
			Rect wrapViewRect, wrapPictRect;

			Delay(Scroll_Ticks, &ticks);
			SetGWorld(drawWorldP, nil);

			theCreditsRec.pictRect.top += 1;
			theCreditsRec.pictRect.bottom += 1;

			thePixMapH = GetGWorldPixMap(theCreditsRec.theGWorldP);
			if ( LockPixels(thePixMapH) ) {
				if (theCreditsRec.pictRect.bottom <= theCreditsRec.pictHeight) {
					CopyBits((BitMapPtr)(*thePixMapH), (BitMapPtr)drawPixMapP,
							 &theCreditsRec.pictRect, &theCreditsRec.viewRect,
							 srcCopy, nil);
				} else {
					wrapViewRect = theCreditsRec.viewRect;
					wrapViewRect.top = wrapViewRect.bottom -
						(theCreditsRec.pictRect.bottom -
						 theCreditsRec.pictHeight);
					wrapPictRect = theCreditsRec.pictRect;
					wrapPictRect.top = 0;
					wrapPictRect.bottom = wrapPictRect.top +
						(wrapViewRect.bottom - wrapViewRect.top);
					CopyBits((BitMapPtr)(*thePixMapH), (BitMapPtr)drawPixMapP,
							 &theCreditsRec.pictRect, &theCreditsRec.viewRect,
							 srcCopy, nil);
					CopyBits((BitMapPtr)(*thePixMapH), (BitMapPtr)drawPixMapP,
							 &wrapPictRect, &wrapViewRect, srcCopy, nil);
				}
				UnlockPixels(thePixMapH);
			}


#if WHITE_ON_BLACK
			PenMode(adMin);					// Fade out at top and fade in at bottom
#else
			PenMode(adMax);
#endif

			j = Fade_Levels - 1;
			for (i = 0; i <= j; i++) {
#if WHITE_ON_BLACK
				fadeColor.red = fadeColor.blue = fadeColor.green =
					grayLevels[i];
#else
				fadeColor.red = fadeColor.blue = fadeColor.green =
					grayLevels[Fade_Levels - i - 1];
#endif
				RGBForeColor(&fadeColor);
				MoveTo(theCreditsRec.viewRect.left,
					   theCreditsRec.viewRect.top + i);
				Line(creditsWidth, 0);
				MoveTo(theCreditsRec.viewRect.left,
					   theCreditsRec.viewRect.bottom - i - 1);
				Line(creditsWidth, 0);
			}
			ForeColor(blackColor);
			PenMode(patCopy);

#if WHITE_ON_BLACK
			CopyBits((BitMapPtr)backPixMapP, (BitMapPtr)drawPixMapP,
					 &theCreditsRec.viewRect, &theCreditsRec.viewRect, adMax,
					 nil);
#else
			CopyBits((BitMapPtr)backPixMapP, (BitMapPtr)drawPixMapP,
					 &theCreditsRec.viewRect, &theCreditsRec.viewRect, adMin,
					 nil);
#endif
			SetGWorld(aboutBoxWindow, GetMainDevice());
			CopyBits((BitMapPtr)drawPixMapP, (BitMapPtr)windowPixMapP,
					 &theCreditsRec.viewRect, &theCreditsRec.viewRect,
					 srcCopy, nil);
		}
		endTicks = TickCount(); //	Timing loop.
		if (endTicks == startTicks) {	//	TickCount must change during each pass thru animation loop.
			while (endTicks == TickCount())
				;			//	this puts an upper speed limit on the animation rate.
		}
	}

	// Finished animation (user clicked). Flush the mousedown events

	FlushEvents(mDownMask | mUpMask | keyDownMask | keyUpMask | autoKeyMask,
				0);

	//	Dispose of all GWorlds (if they haven't already been disposed).

	if (theCreditsRec.theGWorldP != nil)
		DisposeGWorld(theCreditsRec.theGWorldP);

}

void CleanUpGraphics(void)
{
	if (drawWorldP) {
		DisposeGWorld(drawWorldP);
		drawWorldP = NULL;
	}
	if (backWorldP) {
		DisposeGWorld(backWorldP);
		backWorldP = NULL;
	}
	if (aboutBoxWindow) {
		DisposeWindow((WindowPtr)aboutBoxWindow);
		aboutBoxWindow = NULL;
	}
}

OSErr GetItemRect(short inItem, Rect *outRect)
{
	DITLHand theDITL;

	theDITL = (DITLHand) GetResource('DITL', rDITLAbout);
	if (theDITL == NULL)
		return -1;
	*outRect = (**theDITL).theUserItems[inItem - 1].box;
	return noErr;
}

OSErr DrawPictIntoNewGWorld(short inPICTid, short inDepth,
						   GWorldPtr *outGWorldP)
{
	PicHandle thePicture;
	Rect picFrame;
	GWorldPtr saveWorld;
	GDHandle saveDevice;
	OSErr err;

	thePicture = GetPicture(inPICTid);
	picFrame = (**thePicture).picFrame;

	err = NewGWorld(outGWorldP, inDepth, &picFrame, nil,
					GetGWorldDevice(drawWorldP), noNewDevice);
	if ( err )
		return err;

	GetGWorld(&saveWorld, &saveDevice);
	SetGWorld (*outGWorldP, nil);
	if ( LockPixels(GetGWorldPixMap(*outGWorldP)) ) {
		DrawPicture(thePicture, &picFrame);
		UnlockPixels(GetGWorldPixMap(*outGWorldP));
	}
	SetGWorld(saveWorld, saveDevice);

	ReleaseResource((Handle)thePicture);
	return noErr;
}

OSErr DrawTextIntoNewGWorld(short inTEXTid, Rect *ioBounds, short inDepth,
						   GWorldPtr *outGWorldP)
{
	Handle theText;
	TEHandle	theTE;
	StScrpHandle	theStyle;
	GWorldPtr saveWorld;
	GDHandle saveDevice;
	OSErr err;
	
		// Put Text in a TERecord
	
	TextFont(applFont);	
	TextSize(9);
	theTE = TEStyleNew(ioBounds, ioBounds);
	if (theTE == NULL) {
		err = -1;
		goto failed;
	}
	theText = GetResource('TEXT', inTEXTid);
	if (theText == NULL) {
		err = -1;
		goto failed;
	}
	theStyle = (StScrpHandle) GetResource('styl', inTEXTid);
	HLock(theText);
	HidePen();
	TEStyleInsert(*theText, GetHandleSize(theText), theStyle, theTE);
	ShowPen();
	ReleaseResource(theText);
	if (theStyle != nil) {
		ReleaseResource((Handle)theStyle);
	}
#if WHITE_ON_BLACK
	TESetAlignment(teJustRight, theTE);
#else
	TESetAlignment(teJustLeft, theTE);
#endif
	TECalText(theTE);

		// Determine height of the Text

	ioBounds->right = ioBounds->right - ioBounds->left;
	ioBounds->left = ioBounds->top = 0;
	ioBounds->bottom = TEGetHeight((**theTE).nLines, 0, theTE);
	
		// Create new GWorld that is the height of the Text
	err = NewGWorld(outGWorldP, inDepth, ioBounds, nil,
					GetGWorldDevice(drawWorldP), noNewDevice);
	if ( err )
		goto failed;

	GetGWorld(&saveWorld, &saveDevice);
	SetGWorld (*outGWorldP, nil);
	if ( LockPixels(GetGWorldPixMap(*outGWorldP)) ) {
		EraseRect(ioBounds);
			// Draw Text inside GWorld
		(**theTE).viewRect = *ioBounds;
		(**theTE).destRect = *ioBounds;
		(**theTE).inPort = (GrafPtr) *outGWorldP;

		TEUpdate(ioBounds, theTE);
#if WHITE_ON_BLACK
		InvertRect(ioBounds);			// White letters on black background
#endif
		UnlockPixels(GetGWorldPixMap(*outGWorldP));
	}
	TEDispose(theTE);
	SetGWorld(saveWorld, saveDevice);
	return noErr;
failed:
	if (theTE)
		TEDispose(theTE);
	return err;
}


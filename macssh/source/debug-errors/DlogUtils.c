/*
*	DlogUtils.c
*	Very Useful Dialog Utilities Apple SHOULD have provided in the toolbox...
*
*  Revisions:
*  8/92		Telnet 2.6:	Initial version.  Jim Browne
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "DlogUtils.proto.h"
#include "netevent.proto.h"
#include "movableModal.h"

#include "Popup.h"
#include "popup.proto.h"

pascal short modalProc(DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(modalProc, ModalFilter);
pascal short FileHook(short item, DialogPtr theDialog);
PROTO_UPP(FileHook, DlgHook);
pascal Boolean	FolderFilter(FileParam *pbp);
PROTO_UPP(FolderFilter, FileFilter);

void GetTEText( DialogPtr dptr, short item, Str255 str)
{
	short type;
	Handle itemh;
	Rect theRect;

	GetDialogItem(dptr, item, &type, &itemh, &theRect);
	GetDialogItemText(itemh, str);
}

void SetTEText( DialogPtr dptr, short item, Str255 str)
{
	short type;
	Handle itemh;
	Rect theRect;

	GetDialogItem(dptr, item, &type, &itemh, &theRect);
	SetDialogItemText(itemh, str);
}

void Hilite( DialogPtr dptr, short item, short value)
{
	short Type;
	Handle Itemh;
	Rect Box;

	GetDialogItem(dptr, item, &Type, &Itemh, &Box);
	HiliteControl((ControlHandle)Itemh, value);
}

void FlipCheckBox( DialogPtr dptr, short item)
{
	short type;
	Handle itemHdl;
	Rect box;

	GetDialogItem(dptr, item, &type, &itemHdl, &box);
	SetControlValue((ControlHandle)itemHdl, (GetControlValue((ControlHandle)itemHdl) == 0) ? 1 : 0);
}

void FlipRadioButton( DialogPtr dptr, short item)
{
	short type;
	Handle itemHdl;
	Rect box;

	GetDialogItem(dptr, item, &type, &itemHdl, &box);
	SetControlValue((ControlHandle)itemHdl, (GetControlValue((ControlHandle)itemHdl) == 0) ? 1 : 0);
}

void SetCntrl( DialogPtr dptr, short item, short value)
{
	short Type;
	Handle Itemh;
	Rect Box;

	GetDialogItem(dptr, item, &Type, &Itemh, &Box);
	SetControlValue((ControlHandle)Itemh, value);
}

short GetCntlVal( DialogPtr dptr, short item)
{
	short Type;
	Handle Itemh;
	Rect Box;

	GetDialogItem(dptr, item, &Type, &Itemh, &Box);
	return GetControlValue((ControlHandle)Itemh);
}

short findPopupMenuItem(MenuHandle theHandle, Str255 name)
{
	short id;
	Str255 	tempName;
	short numItems;

	numItems = CountMItems(theHandle);
	for (id = 1; id < numItems; id++)
	{
		GetMenuItemText(theHandle,id,tempName);
		if (EqualString(tempName, name, 0, 0))
			break;
	}

	return id;
}

#ifndef NO_UNIVERSAL
void UItemAssign( DialogPtr dptr, short item, UserItemUPP proc)
#else
void UItemAssign( DialogPtr dptr, short item, ProcPtr proc)
#endif
/* sets the handler for a user item to the specified procedure. */
{
	Rect ibox;
	char **ihndl;
	short typ;

	GetDialogItem( dptr, item, &typ, &ihndl, &ibox);
	SetDialogItem( dptr, item,  typ, (Handle) proc, &ibox);
}

SIMPLE_UPP(OutlineItem, UserItem);
pascal void OutlineItem( DialogPtr dptr, short itm)
{
	Rect ibox;
	char **ihndl;
	short typ;
	short item=itm;
	
	GetDialogItem( dptr, item, &typ, &ihndl, &ibox);
	PenSize( 3,3);
	InsetRect( &ibox, -4,-4);
	FrameRoundRect( &ibox, 16,16);
}

SIMPLE_UPP(VersionNumber, UserItem);
pascal void VersionNumber( DialogPtr dptr, short itm)
{
	Rect ibox;
	char **ihndl;
	short typ;
	short item=itm;
	VersRecHndl version;
	Str63 versionString;

	if ((version = (VersRecHndl) GetResource('vers',1)) != nil)
	{
		BlockMoveData((*version)->shortVersion+*(*version)->shortVersion+1, versionString, *((*version)->shortVersion+*(*version)->shortVersion+1)+1);
		ReleaseResource((Handle)version);
	}
	else
		*versionString = 0;

	GetDialogItem( dptr, item, &typ, &ihndl, &ibox);
	TextFont( 3);
	TextSize( 9);
	MoveTo( ibox.left /* +((ibox.right-ibox.left)/2)-(StringWidth(versionString)/2) */, ibox.bottom-2);
	DrawString(versionString);
}

SIMPLE_UPP(modalProc, ModalFilter);
pascal short modalProc( DialogPtr dptr, EventRecord *evt, short *item)
{
	UNUSED_ARG(dptr)
	short keyCode, key;

	if (evt->what == keyDown) {
		key = evt->message & charCodeMask;
		keyCode = (evt->message >>8) & 0xff ;
		if ((key == 0x0d) || (key == 0x03)) { // 0x0d == return, 0x03 == enter
			*item = DLOGOk;
			return(-1);				/* BYU LSC - pascal doesn't like "1" as true */
			}
		if (((key == '.') && (evt->modifiers & cmdKey)) ||
			((key == 0x1b) && (keyCode == 0x35)) ) {	// 0x35 == escKey
			*item = DLOGCancel;
			return(-1);				/* BYU LSC - pascal doesn't like "1" as true */
			}
		}
	Stask();
	return(FALSE);
}

void FlashButton (DialogPtr theDialog, short item)
{
	short itemType;
	ControlHandle theItem;
	Rect itemRect;
	DELAYLONG myticks;
	
	if (item > 0) {
		GetDialogItem(theDialog,item,&itemType,(Handle *)&theItem,&itemRect);
		HiliteControl(theItem,1);
		Delay(8,&myticks);
		HiliteControl(theItem,0);
	}
}

SIMPLE_UPP(DLOGwOK, ModalFilter);
pascal short DLOGwOK( DialogPtr dptr, EventRecord *evt, short *item)
{
	return(MyDLOGfilter(dptr, evt, item, true, false, false));
}

SIMPLE_UPP(DLOGwOK_Cancel, ModalFilter);
pascal short DLOGwOK_Cancel( DialogPtr dptr, EventRecord *evt, short *item)
{
	return(MyDLOGfilter(dptr, evt, item, true, true, false));
}

SIMPLE_UPP(DLOGwOK_Cancel_Popup, ModalFilter);
pascal short DLOGwOK_Cancel_Popup( DialogPtr dptr, EventRecord *evt, short *item)
{
	return(MyDLOGfilter(dptr, evt, item, true, true, true));
}

// Our standard modal dialog box filter.
// RAB BetterTelnet 1.2 - we use StdFilterProc now...
SIMPLE_UPP(MyDLOGfilter, ModalFilter);
pascal short MyDLOGfilter( DialogPtr dptr, EventRecord *evt, short *item, 
							Boolean hasOK, Boolean hasCancel, Boolean hasPopup)
{
	short keyCode, key, iType;
	Handle iHndl;
	Rect iRect;

	if (hasOK) SetDialogDefaultItem(dptr, 1);
	if (hasCancel) SetDialogCancelItem(dptr, 2);
	SetDialogTracksCursor(dptr, 1);
	SetPort(dptr);
/*	if (evt->what == keyDown) {
		key = evt->message & charCodeMask;
		keyCode = (evt->message >>8) & 0xff ;
		if (((key == 0x0d) || (key == 0x03)) && hasOK) { // 0x0d == return, 0x03 == enter
			*item = DLOGOk;				// ok
			FlashButton(dptr, DLOGOk);
			return(-1);
			}
		if ((((key == '.') && (evt->modifiers & cmdKey)) ||
			((key == 0x1b) && (keyCode == 0x35))) && hasCancel) {	// 0x35 == escKey
			*item = DLOGCancel;
			FlashButton(dptr, DLOGCancel);
			return(-1);
			}
		}
	else if ((evt->what == updateEvt) && hasOK) {
		if ((WindowPtr)evt->message == dptr) {
//			GetDialogItem(dptr,DLOGOk,&iType,&iHndl,&iRect);	// ok
//			PenSize(3,3);
//			InsetRect(&iRect,-4,-4);
//			FrameRoundRect(&iRect,16,16);
			}
	} */

	if ((evt->what == mouseDown) && hasPopup)
		return(PopupMousedown(dptr, evt, item));

	Stask();
	return CallStdFilterProc(dptr, evt, item);
}

#if 0
//	Our standard modal dialog box filter with code to handle popup user items.
pascal short MyDLOGfilterWPopupFilter( DialogPtr dptr, EventRecord *evt, short *item)
{
	short keyCode, key, iType;
	Handle iHndl;
	Rect iRect;

	SetPort(dptr);
	if (evt->what == keyDown) {
		key = evt->message & charCodeMask;
		keyCode = (evt->message >>8) & 0xff ;
		if ((key == 0x0d) || (key == 0x03)) { // 0x0d == return, 0x03 == enter
			*item = DLOGOk;				// ok
			FlashButton(dptr, DLOGOk);
			return(-1);
			}
		if (((key == '.') && (evt->modifiers & cmdKey)) ||
			((key == 0x1b) && (keyCode == 0x35)) ) {	// 0x35 == escKey
			*item = DLOGCancel;
			FlashButton(dptr, DLOGCancel);
			return(-1);
			}
		}
	else if (evt->what == updateEvt) {
		if ((WindowPtr)evt->message == dptr) {
			GetDialogItem(dptr,DLOGOk,&iType,&iHndl,&iRect);	// ok
			PenSize(3,3);
			InsetRect(&iRect,-4,-4);
			FrameRoundRect(&iRect,16,16);
			}
	}
	
	if (evt->what == mouseDown) return(PopupMousedown(dptr, evt, item));
	Stask();
	return(FALSE);
}
#endif

#ifdef	NO_UNIVERSAL
#define LMSetSFSaveDisk(SFSaveDiskValue) ((* (short *) 0x0214) = (SFSaveDiskValue))
#define LMGetSFSaveDisk() (* (short *) 0x0214)
#define LMGetCurDirStore() (* (long *) 0x0398)
#define LMSetCurDirStore(CurDirStoreValue) ((* (long *) 0x0398) = (CurDirStoreValue))
#endif

Boolean	SelectDirectory(short *VRefNum, long *DirID)
{
	Point		where;
	SFReply		sfr;
	
	LMSetSFSaveDisk( - *VRefNum);
	LMSetCurDirStore( *DirID );
	
	StandardFileCenter(&where, WDSET_DLOG);
	
	TelInfo->setdir = 0;					/* clear dir found flag */
	
	SFPGetFile( where, NULL, FolderFilterUPP, -1, NULL,	/* BYU LSC */
		FileHookUPP, &sfr, WDSET_DLOG,
		modalProcUPP); 

	if (TelInfo->setdir)					/* we got a dir */
	{
		*VRefNum = - LMGetSFSaveDisk();
		*DirID = LMGetCurDirStore();
		return true;
	}
	return false;
}

SIMPLE_UPP(FileHook, DlgHook);
pascal short	FileHook( short item, DialogPtr theDialog)
{
#ifdef	MPW
#pragma unused(theDialog)
#endif
	if(item == 11) {					/* Set Directory Button */
			TelInfo->setdir = 1;
			
			return(3);		/* pretend we hit cancel, but we didn't */
		}
	else if (item == 12)
		item = 9;			/* these don't count, map to disabled item */

	return(item);
}

SIMPLE_UPP(FolderFilter, FileFilter);
pascal Boolean	FolderFilter(FileParam *pbp)
{
	if (pbp->ioFlAttrib & 0x0010) return(FALSE);
	else return(-1);
}

OSErr SetIndString(StringPtr theStr,short resID,short strIndex)
{
   	Handle theRes;               /* handle pointing to STR# resource */
   	short numStrings;            /* number of strings in STR# */
   	short ourString;             /* counter to index up to strIndex */
   	char *resStr;                /* string pointer to STR# string to replace */
   	long oldSize;                /* size of STR# resource before call */
   	long newSize;                /* size of STR# resource after call */
	unsigned long offset;        /* resource offset to str to replace*/
	OSErr memError;
   /* make sure index is in bounds */

   if (resID < 1)
   return -1;

   /* make sure resource exists */

   theRes = GetResource('STR#',resID);
   if (ResError()!=noErr)
   return ResError();
   if (!theRes || !(*theRes))
   return resNotFound;

   HLock(theRes);
   HNoPurge(theRes);

   /* get # of strings in STR# */

   BlockMoveData(*theRes,&numStrings,sizeof(short));
   if (strIndex > numStrings)
   return resNotFound;

   /* get a pointer to the string to replace */

   offset = sizeof(short);
   resStr = (char *) *theRes + sizeof(short);
   for (ourString=1; ourString<strIndex; ourString++) {
   offset += 1+resStr[0];
   resStr += 1+resStr[0];
   }

   /* grow/shrink resource handle to make room for new string */

   oldSize = GetHandleSize(theRes);
   newSize = oldSize - resStr[0] + theStr[0];
   HUnlock(theRes);
   memError = mySetHandleSize(theRes,newSize);
   if (memError!=noErr) {
   ReleaseResource(theRes);
   return -1;
   }
   HLock(theRes);
   resStr = *theRes + offset;

   /* move old data forward/backward to make room */

   BlockMoveData(resStr+resStr[0]+1, resStr+theStr[0]+1, oldSize-offset-resStr[0]-1);

   /* move new data in */

   BlockMoveData(theStr,resStr,theStr[0]+1);

   /* write resource out */

   ChangedResource(theRes);
   WriteResource(theRes);
   HPurge(theRes);
   ReleaseResource(theRes);

   return ResError();
}

/* A very special thanks to Steve Dorner (sdorner@qualcomm.com) for the following code! */

/************************************************************************
 * GetNewMyDialog - get a new dialog, with a bit extra
 ************************************************************************/
WindowPtr GetNewMyDialog(short template,Ptr wStorage,WindowPtr behind,
	void (*centerFunction)(short))
{
	WindowPtr win;
	
	if (wStorage == nil)
	{
		if ((wStorage=myNewPtrCritical(sizeof(DialogRecord)))==nil)
			return (nil);
	}
	WriteZero(wStorage, sizeof(WindowRecord));
/*
	if ( centerFunction != NULL) {
		(*centerFunction)(template);
	}
*/
	win = GetNewDialog(template, wStorage, behind);
	if (win==nil) return(nil);
	SetPort(win);
	TextFont(systemFont);
	TextSize(12);
/*	win->contR = ((GrafPtr)win)->portRect; */
/*	win->qWindow.refCon = CREATOR; */
	return(win);
}

/* Do a GetNewMyDialog, but make the text small... */
WindowPtr GetNewMySmallDialog(short template,Ptr wStorage,WindowPtr behind,
	void (*centerFunction)(short))
{
	short size;
	DialogPtr dlogptr;
	
	size = 9;
	SetDialogFont(applFont);
	dlogptr = GetNewMyDialog(template,wStorage,behind,centerFunction);
	SetDialogFont(systemFont);
	if (dlogptr==nil)
		return 0;
	SetPort(dlogptr);
	TextSize(size);
	TextFont(applFont);
	{
		TEHandle teh = ((DialogPeek)dlogptr)->textH;
		ControlHandle cntl = (ControlHandle)((WindowPeek)dlogptr)->controlList;
		(*teh)->txSize = size;
		(*teh)->lineHeight = GetLeading(applFont,size);
		(*teh)->fontAscent = GetAscent(applFont,size);
	}			

	/* 	For those dialogs with non-system font buttons, make them inititally invisible
		and then do a ShowWindow.  This keeps the Control Manager from every using Chicago
		on our turf. -- JMB */
		
	//if (! ((((DialogPeek) dlogptr)->window).visible)) ShowWindow(dlogptr);
	
	return(dlogptr);
}

/* Do a GetNewMySmallDialog, but read the item titles from a STR#... */
WindowPtr GetNewMySmallStrangeDialog(short template,Ptr wStorage,WindowPtr behind,
	void (*centerFunction)(short))
{
	short 		size, index, limit, itemType;
	DialogPtr 	dlogptr;
	Handle		resource;
	ControlHandle	itemHandle;
	Rect		scratchRect;
	Str255		scratchPstring;
	
	size = 9;
	SetDialogFont(applFont);
	dlogptr = GetNewMyDialog(template,wStorage,behind,centerFunction);
	SetDialogFont(systemFont);
	if (dlogptr==nil)
		return 0;
	SetPort(dlogptr);
	TextSize(size);
	TextFont(applFont);
	{
		TEHandle teh = ((DialogPeek)dlogptr)->textH;
		ControlHandle cntl = (ControlHandle)((WindowPeek)dlogptr)->controlList;
		(*teh)->txSize = size;
		(*teh)->lineHeight = GetLeading(applFont,size);
		(*teh)->fontAscent = GetAscent(applFont,size);
	}			

	/* 	For those dialogs with non-system font buttons, make them inititally invisible
		and then do a ShowWindow.  This keeps the Control Manager from ever using Chicago
		on our turf. -- JMB */
	
	if ((resource = GetResource('STR#', template)) != NULL) {
		short base;
		Str255 baseStr;
		
		GetIndString(baseStr, template, 1);
		base = atoi(p2cstr(baseStr));
		
		limit = *(short *)(*resource);
		ReleaseResource(resource);
		
		for(index = 2; index <= limit; index++) {
			GetDialogItem(dlogptr, index + base, &itemType, (Handle *)&itemHandle, &scratchRect);
			if ((itemType == 7) && ((**itemHandle).contrlRfCon == 1991)) {
				GetIndString(scratchPstring, template, index);
				SetControlTitle(itemHandle, scratchPstring);
				}
			}
		}
				
	return(dlogptr);
}

// RAB BetterTelnet 1.2:
// Do a GetNewMySmallStrange Dialog, but use Geneva 10, not Geneva 9...
WindowPtr GetNewMySmall10StrangeDialog(short template,Ptr wStorage,WindowPtr behind,
	void (*centerFunction)(short))
{
	short 		size, index, limit, itemType;
	DialogPtr 	dlogptr;
	Handle		resource;
	ControlHandle	itemHandle;
	Rect		scratchRect;
	Str255		scratchPstring;
	
	size = 10;
	SetDialogFont(applFont);
	dlogptr = GetNewMyDialog(template,wStorage,behind,centerFunction);
	SetDialogFont(systemFont);
	if (dlogptr==nil)
		return 0;
	SetPort(dlogptr);
	TextSize(size);
	TextFont(applFont);
	{
		TEHandle teh = ((DialogPeek)dlogptr)->textH;
		ControlHandle cntl = (ControlHandle)((WindowPeek)dlogptr)->controlList;
		(*teh)->txSize = size;
		(*teh)->lineHeight = GetLeading(applFont,size);
		(*teh)->fontAscent = GetAscent(applFont,size);
	}

	if ((resource = GetResource('STR#', template)) != NULL) {
		short base;
		Str255 baseStr;
		
		GetIndString(baseStr, template, 1);
		base = atoi(p2cstr(baseStr));
		
		limit = *(short *)(*resource);
		ReleaseResource(resource);
		
		for(index = 2; index <= limit; index++) {
			GetDialogItem(dlogptr, index + base, &itemType, (Handle *)&itemHandle, &scratchRect);
			if ((itemType == 7) && ((**itemHandle).contrlRfCon == 1991)) {
				GetIndString(scratchPstring, template, index);
				SetControlTitle(itemHandle, scratchPstring);
				}
			}
		}
				
	return(dlogptr);
}

// Hide a range of dialog items. RAB BetterTelnet 1.2
void HideDialogItemRange(DialogPtr dlogp, short startItem, short endItem)
{
	short i;

	for (i = startItem; i <= endItem; i++)
		HideDialogItem(dlogp, i);
}

// Show a range of dialog items. RAB BetterTelnet 1.2
void ShowDialogItemRange(DialogPtr dlogp, short startItem, short endItem)
{
	short i;

	for (i = startItem; i <= endItem; i++)
		ShowDialogItem(dlogp, i);
}

// RAB BetterTelnet 1.2
// Draw the background color over an item. Needed to hide NCSA's brain-dead
// fake popup menus. One of these days, I'll rip out popup.c, but until then...

void DrawBlank(DialogPtr dlogp, short theItem)
{
	Handle hdl;
	Rect box;
	short type;
	GrafPtr savePort;

	GetDialogItem(dlogp, theItem, &type, &hdl, &box);
	GetPort(&savePort);
	SetPort(dlogp);
	InsetRect(&box, -5, -5);
	EraseRect(&box);
	SetPort(savePort);
}

/**********************************************************************
 * write zeroes over an area of memory
 **********************************************************************/
void WriteZero(Ptr pointer,long size)
{
	while (size--) *pointer++ = 0;
}

/**********************************************************************
 * figure out the appropriate leading for a font
 **********************************************************************/
short GetLeading(short fontID,short fontSize)
{
	FMInput fInInfo;
	FMOutput *fOutInfo;
	
	/* set up the font input struct */
	fInInfo.family = fontID;
	fInInfo.size = fontSize;
	fInInfo.face = 0;
	fInInfo.needBits = FALSE;
	fInInfo.device = 0;
	fInInfo.numer.h = fInInfo.numer.v = 1;
	fInInfo.denom.h = fInInfo.denom.v = 1;
	
	/* get the actual info */
	fOutInfo = FMSwapFont(&fInInfo);
	
	return(((fOutInfo->leading + fOutInfo->ascent + fOutInfo->descent)*fOutInfo->numer.v)/fOutInfo->denom.v);
}

/**********************************************************************
 * find ascent font
 **********************************************************************/
short GetAscent(short fontID,short fontSize)
{
	FMInput fInInfo;
	FMOutput *fOutInfo;
	
	/*	set up the font input struct */
	fInInfo.family = fontID;
	fInInfo.size = fontSize;
	fInInfo.face = 0;
	fInInfo.needBits = FALSE;
	fInInfo.device = 0;
	fInInfo.numer.h = fInInfo.numer.v = 1;
	fInInfo.denom.h = fInInfo.denom.v = 1;
	
	/*	get the actual info */
	fOutInfo = FMSwapFont(&fInInfo);
	
	return((fOutInfo->ascent * fOutInfo->numer.v)/fOutInfo->denom.v);
}

/************************************************************************
 * CenterRectIn - center one rect in another
 ************************************************************************/
void CenterRectIn(Rect *inner,Rect *outer)
{
	OffsetRect(inner,(outer->left+outer->right-inner->left-inner->right)/2, 
									 (outer->top+outer->bottom-inner->top-inner->bottom)/2);																	
}

/************************************************************************
 * TopCenterRectIn - center one rect in (the bottom of) another
 ************************************************************************/
void TopCenterRectIn(Rect *inner,Rect *outer)
{
	OffsetRect(inner,(outer->left+outer->right-inner->left-inner->right)/2, 
									 outer->top-inner->top);																	
}

/************************************************************************
 * BottomCenterRectIn - center one rect in (the bottom of) another
 ************************************************************************/
void BottomCenterRectIn(Rect *inner,Rect *outer)
{
	OffsetRect(inner,(outer->left+outer->right-inner->left-inner->right)/2, 
									 outer->bottom-inner->bottom);																	
}

/************************************************************************
 * ThirdCenterRectIn - center one rect in (the top 1/3 of) another
 ************************************************************************/
void ThirdCenterRectIn(Rect *inner,Rect *outer)
{
	OffsetRect(inner,(outer->left+outer->right-inner->left-inner->right)/2, 
									 outer->top-inner->top +
									 (outer->bottom-outer->top-inner->bottom+inner->top)/3);																	
}

void SecondThirdCenterRectIn(Rect *inner,Rect *outer)
{
	OffsetRect(inner,(outer->left+outer->right-inner->left-inner->right)/2, 
									 outer->top-inner->top +
									 2*(outer->bottom-outer->top-inner->bottom+inner->top)/3);																	
}

void ThirdCenterDialog(short id)
{
	DialogTHndl dTempl;
	Rect r = qd.screenBits.bounds;
	r.top += GetMBarHeight();       
	
	dTempl=(DialogTHndl)GetResource('ALRT',id);
	if (!dTempl)
  		 dTempl=(DialogTHndl)GetResource('DLOG',id);
	if (dTempl)
    	ThirdCenterRectIn(&(*dTempl)->boundsRect,&r);
}
void SecondThirdCenterDialog(short id)
{
	DialogTHndl dTempl;
	Rect r = qd.screenBits.bounds;
	r.top += GetMBarHeight();       
	
	dTempl=(DialogTHndl)GetResource('ALRT',id);
	if (!dTempl)
  		 dTempl=(DialogTHndl)GetResource('DLOG',id);
	if (dTempl)
    	SecondThirdCenterRectIn(&(*dTempl)->boundsRect,&r);
}


/************************************************************************
 * StandardFileCenter - figure out where a stdfile dialog should go
 ************************************************************************/
void StandardFileCenter(Point *where, short id)
{
	Rect r,in;
	DialogTHndl dTempl; 			
	dTempl=(DialogTHndl)GetResource('ALRT',id);
	if (!dTempl)
  		 dTempl=(DialogTHndl)GetResource('DLOG',id);
	if (dTempl)
	{
		r = (*dTempl)->boundsRect;
		in = qd.screenBits.bounds;
		in.top += GetMBarHeight();
		ThirdCenterRectIn(&r,&in);
		where->h = r.left;
		where->v = r.top;
	}
	else
	{
		where->h = 100;
		where->v = 100;
	}
}

/* Pascal string utilities */

/*	pstrcat - add string 'src' to end of string 'dst' */
void pstrcat(StringPtr dst, StringPtr src)
{
	/* copy string in */
	BlockMoveData(src + 1, dst + *dst + 1, *src);
	/* adjust length byte */
	*dst += *src;
}

/*	pstrinsert - insert string 'src' at beginning of string 'dst' */
void pstrinsert(StringPtr dst, StringPtr src)
{
	/* make room for new string */
	BlockMoveData(dst + 1, dst + *src + 1, *dst);
	/* copy new string in */
	BlockMoveData(src + 1, dst + 1, *src);
	/* adjust length byte */
	*dst += *src;
}

void pstrcpy(StringPtr dst, StringPtr src)
{
	BlockMoveData(src, dst, *src + 1);
}

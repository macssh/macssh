// drag.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include <Drag.h>
#include <CodeFragments.h>

#include "wind.h"

#include "maclook.proto.h"		// For WindowPtr2WindRecPtr proto
#include "rsinterf.proto.h"
#include "parse.proto.h"
#include "drag.proto.h"

Boolean	gHaveDragMgr, gDropcanAcceptItems, gDropcursorInContent, gDropDestCanAcceptItems;

#define	kScrollbarSize	15

pascal OSErr MyReceiveDropHandler(WindowPtr theWindow, unsigned long handlerRefCon,
								  DragReference theDrag);
PROTO_UPP(MyReceiveDropHandler, DragReceiveHandler);
pascal OSErr MyTrackingHandler(short message, WindowPtr theWindow,
							   void *handlerRefCon, DragReference theDrag);
PROTO_UPP(MyTrackingHandler, DragTrackingHandler);

static	void LocalToGlobalRgn (RgnHandle rgn);
static	void OutlineRegion (RgnHandle theRgn);

void	MyDragInit(void)
{
	OSErr	err;
	long	dragMgrAttr;
	
	err = Gestalt(gestaltDragMgrAttr, &dragMgrAttr);
	gHaveDragMgr = (err == noErr) && (dragMgrAttr & (1L << gestaltDragMgrPresent)) != 0;
	#ifdef powerc
	gHaveDragMgr = gHaveDragMgr && 
		(dragMgrAttr & (1L << gestaltPPCDragLibPresent)) != 0
		&& ((long) InstallTrackingHandler) != kUnresolvedCFragSymbolAddress;


	#endif
	// If the DragManager is available, install our tracking handlers
	if (gHaveDragMgr) {
		err = InstallTrackingHandler(MyTrackingHandlerUPP, nil, nil);

		if (err == noErr) {
			// If all is ok so far, install the rcv handler
			err = InstallReceiveHandler(MyReceiveDropHandlerUPP, nil, nil);
			if (err != noErr) {
				// If an error ocurred installing the rcv handler, remove the tracking handler
				RemoveTrackingHandler(MyTrackingHandlerUPP, nil);
				}
			}
		
		// If any error occurred, turn off DragManager support	
		if (err != noErr) {
			gHaveDragMgr = false;
			}
		}
}

/*	MyReceiveDropHandler
 *	Called by the Drag Manager when a drop occurs over one of Telnet's windows. */
SIMPLE_UPP(MyReceiveDropHandler, DragReceiveHandler);
pascal OSErr MyReceiveDropHandler(WindowPtr theWindow, unsigned long handlerRefCon,
								  DragReference theDrag)
{	
	OSErr				result,memError;
	Handle				dataH;
	unsigned short		items, index;
	ItemReference		theItem;
	DragAttributes		attributes;
	Size				textSize;
	short				mouseDownModifiers, mouseUpModifiers;
	WindRecPtr			tw;

	if (!gDropcanAcceptItems || !gDropcursorInContent)
		return(dragNotAcceptedErr);

	SetPort(theWindow);

	// No text yet
	dataH = nil;
	
	GetDragAttributes(theDrag, &attributes);
	GetDragModifiers(theDrag, 0L, &mouseDownModifiers, &mouseUpModifiers);

	//	Loop through all of the drag items contained in this drag and collect the text
	//	into the accumulation handle.

	CountDragItems(theDrag, &items);

	for (index = 1; index <= items; index++) {
		//	Get the item's reference number, so we can refer to it.
		GetDragItemReferenceNumber(theDrag, index, &theItem);

		//	Try to get the flags for a 'TEXT' flavor. If this returns noErr,
		//	then we know that a 'TEXT' flavor exists in the item.
		result = GetFlavorDataSize(theDrag, theItem, 'TEXT', &textSize);

		if (result == noErr) {
			if (dataH == nil) {					// No data yet, create a new handle for accumulation
				dataH = myNewHandle(textSize);
				if (dataH == nil) {
					return memFullErr;			// Exit if there is an error
					}
				}
			else {								// Append to existing TEXT data
				memError = mySetHandleSize(dataH, GetHandleSize(dataH) + textSize);	// Grow the handle
				if (memError != noErr) {
					return memFullErr;			// Exit if there is a problem
					}
				}
				

			// Temporarily lock down the accumlation handle
			HLock(dataH);
		
			// Get the drag data	
			GetFlavorData(theDrag, theItem, 'TEXT', *dataH, &textSize, 0L);

			// Ok to unlock the accumulation handle
			HUnlock(dataH);
			}
		}

	// Get the length of all that was accumulated
	textSize = GetHandleSize(dataH);

	//	If we actually received text, insert it into the destination.
	if (textSize != 0) {
		if (attributes & kDragHasLeftSenderWindow) {
			HideDragHilite(theDrag);
			}

		HLock(dataH);

		// Send the text to the window.		
		tw = WindowPtr2WindRecPtr(theWindow);
		if (tw != nil) {
			SendStringAsIfTyped(tw, *dataH, textSize);
			}
		HUnlock(dataH);
		}

	if (dataH != nil) {
		DisposeHandle(dataH);
		}

	return(noErr);
}

/*	MyTrackingHandler
 *	This is the drag tracking handler for windows in the DragText application.*/
SIMPLE_UPP(MyTrackingHandler, DragTrackingHandler);
pascal OSErr MyTrackingHandler(short message, WindowPtr theWindow,
							   void *handlerRefCon, DragReference theDrag)
{
	short				result;
	unsigned short		count, index;
	unsigned long		flavorFlags, attributes;
	ItemReference		theItem;
	RgnHandle			theRgn, selectionRgn;
	Point				theMouse, localMouse;
	Rect				dragRect;
	GrafPtr				savePort;
	Boolean				inOriginalSelection;
	
	if ((message != kDragTrackingEnterHandler) && (!gDropcanAcceptItems))
		return(noErr);

	if ((message != kDragTrackingEnterHandler) && (message != kDragTrackingEnterWindow) 
			&& (!gDropDestCanAcceptItems))
		return(noErr);

	GetDragAttributes(theDrag, &attributes);

	switch (message) {

		case kDragTrackingEnterHandler:
			//	We get called with this message the first time that a drag enters ANY
			//	window in our application. 
			gDropcanAcceptItems = true;
										
			//	Check to see if all of the drag items contain
			//	TEXT. We only accept a drag if all of the items in the drag can be accepted.
			CountDragItems(theDrag, &count);

			for (index = 1; index <= count; index++) {
				GetDragItemReferenceNumber(theDrag, index, &theItem);

				result = GetFlavorFlags(theDrag, theItem, 'TEXT', &flavorFlags);

				if (result != noErr) {
					gDropcanAcceptItems = false;
					break;
					}
				}
			break;

		case kDragTrackingEnterWindow:
			//	We receive an EnterWindow message each time a drag enters one of our
			//	application's windows. We initialize our global variables for tracking
			//	the drag through the window.

			// Assume this window can accept the drag
			gDropDestCanAcceptItems = true;
			
			// If it is not a connection window, it cannot accept a drop.
			if (((WindowPeek)theWindow)->windowKind != WIN_CNXN) {
				gDropDestCanAcceptItems = false;
				break;
				}
			
			// Initially no blue box.
			gDropcursorInContent = false;
			break;

		case kDragTrackingInWindow:
			GetPort(&savePort);
			SetPort(theWindow);

			//	We receive InWindow messages as long as the mouse is in one of our windows
			//	during a drag. We draw the window highlighting when we get these messages.
			GetDragMouse(theDrag, &theMouse, 0L);

			//	If we are still in the sender window, check to see if we should draw a blue box.
			//	inOriginalSelection is true if the mouse is still within the confines of the
			//	selected text's original location.  This allows the user to cancel a drop by
			//	dropping the text somewhere inside of the original selection.
			if (attributes & kDragInsideSenderWindow) {
				localMouse = theMouse;
				GlobalToLocal(&localMouse);
				
				selectionRgn = RSGetTextSelRgn(WindowPtr2WindRecPtr(theWindow)->vs);
				inOriginalSelection = PtInRgn(localMouse, selectionRgn);
				DisposeRgn(selectionRgn);
				}
			else {
				inOriginalSelection = false;
				}

			//	Show or hide the window highlighting when the mouse enters or leaves the
			//	text area in our window (we don't want to show the highlighting when
			//	the mouse is over the scroll bars).
			dragRect = (*(((WindowPeek)theWindow)->contRgn))->rgnBBox;
			
			//	Subtract out the scrollbars.
			dragRect.right -= kScrollbarSize;
			dragRect.bottom -= kScrollbarSize;
			
			//	If the mouse is in the text area and not in the original selection,
			//	draw the blue box.			
			if (PtInRect(theMouse, &dragRect) && !inOriginalSelection) {
				if (!gDropcursorInContent) {

					// Set up the blue box region.
					GlobalToLocal(&topLeft(dragRect));
					GlobalToLocal(&botRight(dragRect));
					RectRgn(theRgn = NewRgn(), &dragRect);

					ShowDragHilite(theDrag, theRgn, true);

					DisposeRgn(theRgn);
					}
					
				// Remember that we are in a valid drop location.
				gDropcursorInContent = true;

				}
			else {
				// We have moved out of a valid drop region.

				if (gDropcursorInContent) {			// Hide the box if it was drawn.
					HideDragHilite(theDrag);
					}
					
				// Remember that we are outside of a valid drop region.
				gDropcursorInContent = false;

				}	

			SetPort(savePort);
			break;

		case kDragTrackingLeaveWindow:
				HideDragHilite(theDrag);
			break;

		case kDragTrackingLeaveHandler:
			break;

	}

	return(noErr);
}

OSErr DragText(EventRecord *ev, Point where, short w, Boolean *dragged)
{
	DragReference	dragRef;
	OSErr			err = noErr;
	Boolean			haveDragRef = false;
	RgnHandle		dragRgn = nil;
	Handle			textH = nil;
	long			size;
	
	*dragged = false;
	if (!gHaveDragMgr) {
		return noErr;
		}
		
	dragRgn = RSGetTextSelRgn(w);
	if (dragRgn == nil) {
		return noErr;
		}
	if (!PtInRgn(where, dragRgn)) {
		DisposeRgn(dragRgn);
		return noErr;
		}
	if (!WaitMouseMoved(ev->where)) return noErr;
	*dragged = true;

	textH = RSGetTextSel(w, 0, 1);
	
	if ((textH == (char **)-1L) || (textH == nil)) {
		textH = nil;
		goto exit;
		}
	
	HLock(textH);
	size = GetHandleSize(textH);
	
	err = NewDrag(&dragRef);
	if (err != noErr) goto exit;
	haveDragRef = true;

	err = AddDragItemFlavor(dragRef, 1, 'TEXT', *textH, size, 0);
	if (err != noErr) goto exit;

	LocalToGlobalRgn(dragRgn);
	OutlineRegion(dragRgn);

	err = TrackDrag(dragRef, ev, dragRgn);
	if (err != noErr && err != userCanceledErr) goto exit;

	DisposeRgn(dragRgn);
	DisposeDrag(dragRef);
	DisposeHandle(textH);
	return noErr;
	
exit:

	if (haveDragRef) DisposeDrag(dragRef);
	if (dragRgn != nil) DisposeRgn(dragRgn);
	if (textH != nil) DisposeHandle(textH);
	return err;
}

static	void LocalToGlobalRgn (RgnHandle rgn)
{
	Point where;
	
	SetPt(&where, 0, 0);
	LocalToGlobal(&where);
	OffsetRgn(rgn, where.h, where.v);
}

static	void OutlineRegion (RgnHandle theRgn)
{
	RgnHandle tempRgn;
	
	tempRgn = NewRgn();
	CopyRgn(theRgn, tempRgn);
	InsetRgn(tempRgn, 1, 1);
	DiffRgn(theRgn, tempRgn, theRgn);
	DisposeRgn(tempRgn);
}

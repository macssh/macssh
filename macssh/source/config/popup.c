/*
*  popup.c
*  Code for doing popup menus in our dialog boxes
*  Credit for this goes to Apple Sample Code and a conglomeration of other code I've seen.
*
*  NCSA Revisions:
*  8/92		Telnet 2.6:	Initial version.  Jim Browne
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "popup.h"
#include "popup.proto.h"

static popup *FindPopup(short item);
PROTO_UPP(DrawPopUp, UserItem);

popup *Popup;	/* Current popup structure array */

static popup *FindPopup( short item)
{
	popup *p = Popup;

	while (p->item) {
			if (p->item == item)
				return p;
			++p;
	}
	return 0;					/* Not found */
}

SIMPLE_UPP(DrawPopUp, UserItem);
pascal void DrawPopUp(DialogPtr dptr, short item)
{
	char text[256];
	char* s;
	PenState savePen;
	Rect box, title;
	FontInfo info;
	short type, width, newwidth;
	Handle hdl;
	popup *p;

	SetPort(dptr); // RAB BetterTelnet 2.0b2

	GetPenState(&savePen);
	GetFontInfo(&info);

	GetDialogItem(dptr, item, &type, &hdl, &box);
	p = FindPopup(item);

	title = box;
	title.right = box.left - 1;
	title.left = title.right - StringWidth((**(p->h)).menuData) - 2;
	box.top -= 1;
	box.left -= 1;
	box.bottom += 2;
	box.right += 2;

	EraseRect(&box);
	
	if ((**(p->h)).menuData[0] != 0)
		InvertRect(&title);

	/* Get and draw menu title. Get item text. */
	if (!p || !p->h) return;
	s = (char *)(**(p->h)).menuData;	/* Menu title */
	MoveTo(box.left - StringWidth((StringPtr)s) - 2, box.top + info.ascent);
	DrawString((StringPtr)s);
	GetMenuItemText(p->h, p->choice, (StringPtr)text);

	width = (box.right - box.left) - (CharWidth(checkMark) + 2);
	newwidth = StringWidth((StringPtr)text);

	if (newwidth > width) {
		width -= CharWidth('É');
		do {
			newwidth -= CharWidth(text[text[0]]);
			--text[0];
			} while (newwidth > width && text[0]);
		++text[0];
		text[text[0]] = 'É';
		}
	
	MoveTo(box.left + CharWidth(checkMark) + 2, box.top + info.ascent);
	DrawString((StringPtr)text);

	InsetRect(&box, -1, -1);					/* A la Apple Sample Code */
	PenSize(1, 1);
	FrameRect(&box);
	MoveTo(box.left + 2, box.bottom);
	LineTo(box.right, box.bottom);
	LineTo(box.right, box.top + 2);

	SetPenState(&savePen);
}

Boolean PopupMousedown(	DialogPtr dptr, EventRecord *event, short *i)
{
	short item, choice, type;
	popup *p;
	Point clikloc;
	long chosen;
	Handle hdl;
	Rect box, title;
	Boolean result = FALSE;

	clikloc = event->where;
	GlobalToLocal(&clikloc);
	if ((item = FindDialogItem(dptr, clikloc) + 1) < 1)
		return result;
	p = FindPopup(item);
	if (!p || !p->h)
		return result;

	GetDialogItem(dptr, item, &type, &hdl, &box);
	clikloc = topLeft(box);
	LocalToGlobal(&clikloc);
	title = box;
	title.right = box.left - 1;
	title.left = title.right - StringWidth((**(p->h)).menuData) - 2;
	box.top -= 1;
	box.left -= 1;
	box.bottom += 2;
	box.right += 2;

	EraseRect(&box);
	if ((**(p->h)).menuData[0] != 0)
		InvertRect(&title);

	InsertMenu(p->h, hierMenu);
	SetItemMark(p->h, p->choice, checkMark);
	CalcMenuSize(p->h);
	chosen = PopUpMenuSelect(p->h, clikloc.v, clikloc.h, p->choice);
	SetItemMark(p->h, p->choice, noMark);
	DeleteMenu((**(p->h)).menuID);

	if (chosen) {
		choice = chosen & 0xFFFF;			/* Apple sez ignore high word */
		if (choice != p->choice) {
			p->choice = choice;
			*i = item;
			result = TRUE;
		}
	}

	DrawPopUp(dptr, item);

	return result;
}

void PopupInit( DialogPtr dptr, popup *popups)
{
	popup *p;
	short w;
	FontInfo info;
	short type;
	Handle hdl;
	Rect box;

	Popup = p = popups;
	SetPort(dptr);
	GetFontInfo(&info);
	while (p->item) {
		if (p->h) {
			/* Correct user item box. */
			GetDialogItem(dptr, p->item, &type, &hdl, &box);
			CalcMenuSize(p->h);
			w = (**(p->h)).menuWidth;
			w += 16;
			if (w < (box.right - box.left))
				box.right = box.left + w;
			box.bottom = box.top + info.ascent + info.descent + info.leading;
			SetDialogItem(dptr, p->item, type, (Handle)DrawPopUpUPP, &box);
		}
		++p;
	}
}

void PopupCleanup(void)
{
	popup *p = Popup;

	while (p->item) {
		if (p->h)
			DisposeMenu(/*(Handle)*/p->h);	// Since we're using NewMenu's, we need
											// to do a DisposeMenu, rather than a 
											// Releaseresource
		++p;
	}
}




//   maclook.c
//   User interface code for NCSA Telnet for the Macintosh
//	 originally by Gaige B. Paulsen

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
*   User interface for basic telnet with background file transfer
*   capabilities via FTP calls from host.
*
*	This, the Macintosh version also gives each session its own window,
*	provides advanced keyboard translation, Tek emulation, printing/copying
*	of text and graphics and the pasting of text into the active session.
*
*		Requires:
*	menu.c	- Aux. file for handling menu calls
*	event.c	- Aux. file for handling events
*	switch.c- Aux. file for handling swithcher  (the J word is in Event and here )
*
*
*	NCSA Revisions:
*	7/92 Telnet 2.6: added support for the 2 global structs, and put all the 
*					cursors into one array.  Cleaned up defines			Scott Bulmahn
*/


#define PREFMASTER					/* So pref structure gets defined Scott! */


#include "netevent.proto.h"
#include "wind.h"
#include "event.proto.h"
#include "macutil.proto.h"
#include "popup.h"
#include "popup.proto.h"

#include "rsinterf.proto.h"
#include "vsdata.h"
#include "vskeys.h"
#include "vsinterf.proto.h"
#include "vgtek.proto.h"
#include "tekrgmac.proto.h"
#include "vr.h"
#include "vrrgmac.proto.h" 
#include "network.proto.h"
#include "mydnr.proto.h"
#include "menuseg.proto.h"
#include "prefs.proto.h"
#include "maclook.proto.h"
#include "DlogUtils.proto.h"
#include "telneterrors.h"
#include "movableModal.h"

extern	Cursor *theCursors[];
extern	WindRec
	*screens;		/* Window Records (VS) for :	Current Sessions */

extern	short	scrn;
//extern	char	*tempspot;				/* temporary storage ~255 bytes malloc-ed */

void MiscUnload(void) {}

short detachGraphics( short dnum)
{
	short i=0;

	while((i<TelInfo->numwindows) && (dnum!=screens[i].curgraph)) i++;
	if (i>=TelInfo->numwindows) return(-1);
	TelInfo->oldgraph[TelInfo->graphs++]=dnum;
	screens[i].curgraph=-1;
	screens[i].termstate=VTEKTYPE;
	screens[i].enabled=1;
	RGdetach(dnum);
	return(0);
}

/* Find screen index by Window */
short	WindowPtr2ScreenIndex(WindowPtr wn)
{
	short	i=0;

	while((i<TelInfo->numwindows) && (wn!= (WindowPtr)screens[i].wind)) i++;
	if (i>=TelInfo->numwindows) return(-1);
	return(i);
}

WindRecPtr	WindowPtr2WindRecPtr(WindowPtr wn)
{
	short	winNum;
	
	winNum = WindowPtr2ScreenIndex(wn);
	
	if (winNum == -1) {
		return nil;
		}
		
	return &screens[winNum];
}

/* Find screen index by VS Number */
short findbyVS( short vs)
{
	short i=0;

	while((i<TelInfo->numwindows) && (vs!=screens[i].vs)) i++;
	if (i>=TelInfo->numwindows) return(-1);
	return(i);
}

void setgraphcurs( void)				/* Called at start of gin */
{
	TelInfo->ginon=1;
	updateCursor(1);
}

void unsetgraphcurs( void)				/* Called at start of gin */
{
	TelInfo->ginon=0;
	updateCursor(1);
}

short VGalive( short dnum)
{
	short i;

	i=0;
	while((i<TelInfo->numwindows) && (dnum!=screens[i].curgraph)) i++;
	if (i<TelInfo->numwindows) {
		return(1);
		}
	else {
		i=0;
		while ((i<TelInfo->graphs) && (dnum!=TelInfo->oldgraph[i])) i++;
		if (i<TelInfo->graphs)
			return(1);
		}
	return(0);
}

void destroyGraphics( short dnum)
{
	short i;

	i=0;
	while((i<TelInfo->numwindows) && (dnum!=screens[i].curgraph)) i++;
	if (i<TelInfo->numwindows) {
		screens[i].curgraph=-1;
		screens[i].termstate=VTEKTYPE;
		screens[i].enabled=1;
		}
	else {
		i=0;
		while ((i<TelInfo->graphs) && (dnum!=TelInfo->oldgraph[i])) i++;
		while (i<(MaxGraph-1))
			TelInfo->oldgraph[i]=TelInfo->oldgraph[++i];			/* Bump everyone down one slot */
		TelInfo->graphs--;								/* One less graph to feed */
		}
	VGclose( dnum);
}

short FindByVG(short vg)
{
	short i=0;

	while((i<TelInfo->numwindows) && (vg!=screens[i].curgraph)) i++;
	if (i>=TelInfo->numwindows) return(-1);
	return(i);
}

void TekEnable( short vg)
{
	short change;

	change=FindByVG( vg);
	if (change>=0)
		screens[change].enabled=1;
}

void	TekDisable( short vg)
{
	short change;

	change=FindByVG( vg);
	if (change>=0)
		screens[change].enabled=0;
}

/* showNetNumbers	- Show the current network numbers. */
void showNetNumbers( void)
{
	char		tmpout[256];			/* IP Number */

	DialogPtr	dptr;				/* dialog box pointer */
	short		scratchshort;
	int			tmp[4];				/* Integer copy of IP Number */
	unsigned char tmp2[4];

	if (TelInfo->ipModeless) {
		SelectWindow(TelInfo->ipModeless);
		return;
	}

	Mnetinit();						// RAB BetterTelnet 1.0fc4
	netgetip(tmp2);
	for(scratchshort=0; scratchshort<4; scratchshort++)
			tmp[scratchshort] = (int)tmp2[scratchshort];		/* Get integer numbers */

	sprintf(&tmpout[0],"IP Address:\r%d.%d.%d.%d",tmp[0],tmp[1],tmp[2],tmp[3]);
										/* create Human-readable numbers */


	c2pstr(tmpout);									/* BYU LSC */

//	RAB BetterTelnet 1.2 - we can't use ParamText() for modeless dialogs
//	ParamText(0L, (StringPtr)tmpout,0L,0L);				/* BYU LSC - Put Parms in Dlog */

	dptr = GetNewDialog(MyIPDLOG, NULL, kInFront);
	SetTEText(dptr, 1, (unsigned char *)tmpout);
	DrawDialog(dptr);							/* Display Dialog */
	TelInfo->ipModeless = dptr;

//	ModalDialog(NULL, &scratchshort);			/* Wait for a click */

//	DisposeDialog(dptr);
//	Alert(MyIPDLOG, 0L);
	AdjustMenus();
	DoTheMenuChecks();
}

/*******************************************************************/
/*  setupkeys
*   Prompt the user for which keys to use for kill, stop and start.
*/

void setupkeys( void)
{
	DialogPtr dtemp;
	Rect dBox;
	short dItem,kItem,sItem,eItem;
	Handle kbox,sbox,ebox;
	
	char *tempspot;
	
	tempspot = (char *) myNewPtr(256);
	if (tempspot == NULL)
		return; //BUGG signal error here
		
	SetUpMovableModalMenus();
	dtemp=GetNewMyDialog( SetupDLOG, NULL, kInFront, (void *)ThirdCenterDialog);

	setLastCursor(theCursors[normcurs]);
	
	GetDialogItem( dtemp, killbox, &kItem, &kbox, &dBox);	
	GetDialogItem( dtemp, stopbox, &eItem, &ebox, &dBox);	
	GetDialogItem( dtemp, startbox, &sItem, &sbox, &dBox);	

	*tempspot = 0;
	if (screens[scrn].TELstop > 0) {
		sprintf(tempspot,"^%c",screens[scrn].TELstop^64);
		c2pstr(tempspot);									/* BYU LSC */
		SetDialogItemText( ebox , (StringPtr)tempspot);
		SelectDialogItemText( dtemp, stopbox, 0, 32767);
		}
	if (screens[scrn].TELgo > 0) {
		sprintf(tempspot,"^%c",screens[scrn].TELgo^64);
		c2pstr(tempspot);									/* BYU LSC */
		SetDialogItemText( sbox , (StringPtr)tempspot);
		SelectDialogItemText( dtemp, startbox, 0, 32767);
		}
	if (screens[scrn].TELip > 0) {
		sprintf(tempspot,"^%c",screens[scrn].TELip^64);
		c2pstr(tempspot);									/* BYU LSC */
		SetDialogItemText( kbox , (StringPtr)tempspot);
		SelectDialogItemText( dtemp, killbox, 0, 32767);
		}
	
		dItem=0;								/* initially no hits */
		while((dItem>3) || (dItem==0)) {		/* While we are in the loop */
			/*ModalDialog(DLOGwOK_CancelUPP,&dItem);*/
			movableModalDialog(DLOGwOK_CancelUPP,&dItem);

/*
*  intermediate check.  If they hit a key, put its number in the box.
*/
			GetDialogItemText( kbox, (StringPtr)tempspot);					/* BYU LSC - Get the string */
			p2cstr((StringPtr)tempspot);							/* BYU LSC */
			if (*tempspot < 32 && *tempspot > 0) {
				sprintf(tempspot,"^%c",*tempspot^64);
				c2pstr(tempspot);						/* BYU LSC */
				SetDialogItemText( kbox , (StringPtr)tempspot);				/* BYU LSC */
				SelectDialogItemText( dtemp, killbox, 0, 32767 );
				}
			GetDialogItemText( ebox, (StringPtr)tempspot);					/* BYU LSC - Get the string */
			p2cstr((StringPtr)tempspot);							/* BYU LSC */
			if (*tempspot < 32 && *tempspot > 0) {
				sprintf(tempspot,"^%c",*tempspot^64);
				c2pstr(tempspot);						/* BYU LSC */
				SetDialogItemText( ebox , (StringPtr)tempspot);				/* BYU LSC */
				SelectDialogItemText( dtemp, stopbox, 0, 32767);
				}
			GetDialogItemText( sbox, (StringPtr)tempspot);					/* BYU LSC - Get the string */
			p2cstr((StringPtr)tempspot);							/* BYU LSC */
			if (*tempspot < 32 && *tempspot > 0) {
				sprintf(tempspot,"^%c",*tempspot^64);
				c2pstr(tempspot);						/* BYU LSC */
				SetDialogItemText( sbox , (StringPtr)tempspot);				/* BYU LSC */
				SelectDialogItemText( dtemp, startbox, 0, 32767);
				}
				
			}
				
	
		if (dItem==DLOGCancel) {
			DisposeDialog( dtemp);
			ResetMenus();
			return;
			}
			
		GetDialogItemText( kbox, (StringPtr)tempspot);			/* BYU LSC - Get the string */
		p2cstr((StringPtr)tempspot);					/* BYU LSC */
		if (*tempspot != '^') 
			screens[scrn].TELip = -1;
		else
			screens[scrn].TELip = toupper(*(tempspot+1)) ^ 64;
			
		GetDialogItemText( ebox, (StringPtr)tempspot);			/* BYU LSC - Get the string */
		p2cstr((StringPtr)tempspot);					/* BYU LSC */
		if (*tempspot != '^') 
			screens[scrn].TELstop = -1;
		else
			screens[scrn].TELstop = toupper(*(tempspot+1)) ^ 64;

		GetDialogItemText( sbox, (StringPtr)tempspot);			/* BYU LSC - Get the string */
		p2cstr((StringPtr)tempspot);					/* BYU LSC */
		if (*tempspot != '^') 
			screens[scrn].TELgo = -1;
		else
			screens[scrn].TELgo = toupper(*(tempspot+1)) ^ 64;
			
	DisposeDialog( dtemp);
	ResetMenus();
	updateCursor(1);
}

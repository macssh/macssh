/*
*	errors.c
*	This file handles all of the error printing in one centralized place.
*	There are several classes of errors (network, memory, etc).
*	To Handle an error, just call into DoError with the appropriate
*	mask and error number.  The routine goes from there.
*
*****************************************************************
*  Revisions:
*  7/92		Telnet 2.6:	Initial version.  Scott Bulmahn
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "InternalEvents.h"
#include "DlogUtils.proto.h"
#include "mainseg.proto.h"				// for forcequit proto
#include "errors.proto.h"
#include "telneterrors.h"

void loadErrors(void) { } // so that we can load this code high and lock it early

/*================================= ERROR HANDLING CODE ============================*/
/* This is the main error handling code.  Get all the errors out of the resource	*/
/* file.  The resource number that contains the errors depends on what error		*/
/* class we are talking about														*/
void DoError(short code, short level, char *mess1)
{
	char buffer1[256],buffer2[256];
	short dItem;
	DialogPtr dtemp;

	buffer1[0]=0;		/* dont print extraneous garbage to the screen... */
	buffer2[0]=0;

	switch (code & ERRORMASK)
		{
		case NET_ERRORCLASS:
		case MEMORY_ERRORCLASS:
		case RESOURCE_ERRORCLASS:
			strncpy( buffer1, geterrstring(code), 255);
			if (mess1) strncpy(buffer2, mess1, 255);
			break;


		case NOCODE:						/* no code given, just 2 strings */
			strncpy(buffer1, mess1, 255);
		}

	if (level ==2 || level ==3)				
	{				
		c2pstr(buffer1);
		c2pstr(buffer2);
		ParamText((StringPtr) buffer1,(StringPtr) buffer2,"\p","\p");

		dtemp=GetNewDialog(OopsDLOG,(Ptr) 0L,kInFront);	/* Put up the error notice */
		ModalDialog(DLOGwOKUPP,&dItem);		/* Get an event */
		DisposeDialog(dtemp);			/* Take it off.....all off......*/
	}
}

/************************************************************************/
/*  geterrstring
*   returns the string associated with a particular error number
*	NOTE: strings moved to resource # 23237 
*   error number is formatted %4d at the beginning of the string
*/


char *geterrstring(short code)
{
	short i;
	char s[10];
	Str255 ErrorString;									/* BYU 2.4.16 */
	short errorResource, numberOfErrors;
	
	switch (code & ERRORMASK)
		{
		case NET_ERRORCLASS:
			errorResource = NETERROR_RESOURCE_ID;
			numberOfErrors = NETERROR_RESOURCE_COUNT;
			break;
		case MEMORY_ERRORCLASS:
			errorResource = MEMERROR_RESOURCE_ID;
			numberOfErrors = MEMERROR_RESOURCE_COUNT;
			break;
		case RESOURCE_ERRORCLASS:
			errorResource = RESERROR_RESOURCE_ID;
			numberOfErrors = RESERROR_RESOURCE_COUNT;
			break;
		}

	code &= FLAGMASK;	
	sprintf(s,"%4d",code);


	for (i=1; i <= numberOfErrors; i++) 
		{			
		GetIndString(ErrorString,errorResource,i);	
		p2cstr(ErrorString);							
		if (!strncmp((char *) ErrorString,s,4))			
			return((char *) ErrorString + 5);			
		}												
		
	GetIndString(ErrorString, errorResource,1);		
	return((char *) ErrorString+5);						
}

void	FatalCancelAlert(short messageID, StringPtr DefaultButtonString, short InternalID, short MacID)
{
	Str255		errorString, numberString, numberString2;
	DialogPtr	theDialog;
	short		Type, ditem = 3;
	Handle		Itemh;
	Rect		Box;

	GetIndString(errorString, GENERAL_MESSAGES_ID, messageID);
	NumToString((long)InternalID, numberString);
	NumToString((long)MacID, numberString2);
	ParamText(errorString, numberString, numberString2, NULL);
	
	theDialog = GetNewMyDialog(FATALCANCEL_ID, NULL, kInFront, (void *)ThirdCenterDialog);

	GetDialogItem(theDialog, DLOGOk, &Type, &Itemh, &Box);
	SetControlTitle((ControlHandle)Itemh, DefaultButtonString);
	ShowWindow(theDialog);

	while (ditem > 2)	ModalDialog(DLOGwOK_CancelUPP, &ditem);
	DisposeDialog(theDialog);
	
	// If the user chose quit, then do it... else return.
	if (ditem == DLOGCancel)	forcequit();
}

void	FatalAlert(short messageID, short InternalID, short MacID)
{
	Str255		errorString, numberString, numberString2;
	DialogPtr	theDialog;
	short		ditem = 3;

	GetIndString(errorString, GENERAL_MESSAGES_ID, messageID);
	NumToString((long)InternalID, numberString);
	NumToString((long)MacID, numberString2);
	ParamText(errorString, numberString, numberString2, NULL);
	
	theDialog = GetNewMyDialog(FATAL_ID, NULL, kInFront, (void *)ThirdCenterDialog);
	ShowWindow(theDialog);

	while (ditem > 1)	ModalDialog(DLOGwOKUPP, &ditem);
	
	forcequit();		// It's a fatal error, so quit
}

void	OperationFailedAlert(short messageID, short InternalID, short MacID)
{
	Str255		errorString, numberString, numberString2;
	DialogPtr	theDialog;
	short		ditem = 3;

	GetIndString(errorString, OPFAILED_MESSAGES_ID, messageID);
	NumToString((long)InternalID, numberString);
	NumToString((long)MacID, numberString2);
	ParamText(errorString, numberString, numberString2, NULL);
	
	theDialog = GetNewMyDialog(OPFAILED_ID, NULL, kInFront, (void *)ThirdCenterDialog);
	
	if (theDialog)
		ShowWindow(theDialog);
	else
		return;
	while (ditem > 1)	ModalDialog(DLOGwOKUPP, &ditem);
	DisposeDialog(theDialog);
}

void	OutOfMemory(short InternalID)
{
	OperationFailedAlert(OUT_OF_MEMORY, InternalID, 0);
}

// Returns "TRUE" if default button is hit.
Boolean	AskUserAlert(short messageID, Boolean CancelIsDefault)
{
	Str255		errorString;
	DialogPtr	theDialog;
	short		Type, ditem = 3;
	Handle		Itemh;
	Rect		Box;

	GetIndString(errorString, GENERAL_MESSAGES_ID, messageID);
	ParamText(errorString, NULL, NULL, NULL);
	
	theDialog = GetNewMyDialog(ASKUSER_ID, NULL, kInFront, (void *)ThirdCenterDialog);

	if (!CancelIsDefault) {		// We need to swap cancel and ok buttons
		Str255 temp;
		GetDialogItem(theDialog, DLOGOk, &Type, &Itemh, &Box);
		GetIndString(temp,MISC_STRINGS,OK_STRING);
		SetControlTitle((ControlHandle)Itemh, temp);
		GetDialogItem(theDialog, DLOGCancel, &Type, &Itemh, &Box);
		GetIndString(temp,MISC_STRINGS,CANCEL_STRING);
		SetControlTitle((ControlHandle)Itemh, temp);
		}
		
	ShowWindow(theDialog);

	while (ditem > 2)	ModalDialog(DLOGwOK_CancelUPP, &ditem);
	DisposeDialog(theDialog);
	
	if (ditem == 1) return (TRUE);
	else return (FALSE);
}
/*
*    netevent.c
*	 Originally by Gaige B. Paulsen
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*    Network event handler for NCSA Telnet for the Macintosh
*
*	Called by:
*		event.c
*		maclook.c
*
*	NCSA Revisions:
*	7/92	Telnet 2.6:  added the 2 global structures, and cleaned up defines -- Scott Bulmahn
*/

#include <sched.h>

#include "netevent.proto.h"
#include "InternalEvents.h"
#include "wind.h"
#include "mydnr.proto.h"
#include "bkgr.proto.h"
#include "maclook.proto.h"
#include "network.proto.h"
#include "menuseg.proto.h"
#include "rsinterf.proto.h"
#include "vrrgmac.proto.h"
#include "tekrgmac.proto.h"
#include "translate.h"
#include "vsdata.h"
#include "vskeys.h"
#include "translate.proto.h"

#include "parse.proto.h"
#include "Connections.proto.h"
#include "event.proto.h"
#include "vsinterf.proto.h"
#include "telneterrors.h"

extern WindRec
	*screens;					/* The screen array from Maclook */
//extern char *tempspot;			/* 256 bytes for temp strings */
extern short scrn;

short
	FileInTransit,
	gBlocksize[MaxSess],		/*	how large do we make the blocks when we read? */
	gQueueError = 0,			//	Set to 1 if the queue fills up.
	gQueueLength = 0,			//	Used to monitor queue length for debugging purposes.
	gQueueInUse = 0;			//		Ditto.
	
QHdr	gEventsQueue, gEventsFreeQueue;
		
unsigned char *gReadspace = 0;		/* main buffer space */

#define PFTP 1
#define PRCP 2
#define PDATA 3

#ifdef __MWERKS__
#pragma profile off
#endif

//	Our "give time" routine
void Stask( void)
{
}

//	Every time we open or close a network connection, we add or remove a few elements
//	to/from the queue to assure that there will always be some free elements laying
//	around.  This allows us to avoid Dequeue'ing at interrupt time, which causes 
//	nasty mutex problems since we walk the queue in netgetevent at non-interrupt time.
void	ChangeQueueLength(short delta)
{
	internal_event	*theEvent;
	
	if (delta > 0)
		while (delta != 0) {
			theEvent = (internal_event *) myNewPtrCritical(sizeof(internal_event));
			Enqueue((QElemPtr)theEvent, &gEventsFreeQueue);
			gQueueLength++;
			delta--;
			}
	else
		while (delta != 0) {
			theEvent = (internal_event *)gEventsFreeQueue.qHead;
			Dequeue((QElemPtr)theEvent, &gEventsFreeQueue);
			if (theEvent) DisposePtr((Ptr)theEvent);
			gQueueLength--;
			delta++;
			}
}
			
/***********************************************************************/
/*  netgetevent
*   Retrieves the next event (and clears it) which matches bits in
*   the given mask.  Returns the event number or -1 on no event present.
*   Also returns the exact class and the associated integer in reference
*   parameters.
*/

short netgetevent(
//	short	mask,
	short	*evclass,
	short	*data1,
	long	*data2
  )
{
	internal_event	*theEvent;
	short			event;
//#define	QUEUE_STATS
#ifdef	QUEUE_STATS
	char			qs[255];
	static	long	timer = 0;
	
	if (TickCount() - timer > 60*30) {
		sprintf(qs, "QueueLength: %d, InUse: %d", gQueueLength, gQueueInUse);
		timer = TickCount();
		putln(qs);
		}
#endif	QUEUE_STATS

	if (gQueueError)				// Yikes, we actually ran out of queue elements!
		DebugStr("\pOut of Queue Elements, please quit as quickly as possible!");
		
	// Spin until we hit the end of the queue or if we Dequeue an element that is not
	// already dequeued.  If we don't do this check on Dequeue, someone else could come
	// in and dequeue an element that we are about to dequeue.  Thus, WHAM! we both have
	// that queue element.
	while ((theEvent = (internal_event *)gEventsQueue.qHead) != NULL) {
		if (Dequeue((QElemPtr)theEvent, &gEventsQueue) == noErr)
			break;
		}
		
//	while ((theEvent != NULL) && !(theEvent->eclass & mask))
//		theEvent = (internal_event *)theEvent->qLink;
	
	if (theEvent == NULL) return(-1);
		
//	(void) Dequeue((QElemPtr)theEvent, &gEventsQueue);

	*evclass = theEvent->eclass;
	*data1 = theEvent->data1;
	*data2 = theEvent->data2;
	event = theEvent->event;
	
	Enqueue((QElemPtr)theEvent, &gEventsFreeQueue);
	gQueueInUse--;
	return(event);
}

/***********************************************************************/
/*  netputevent
*   add an event to the queue.
*   Will probably get the memory for the entry from the free list.
*   Returns 0 if there was room, 1 if an event was lost.
*/
short netputevent(short theclass, short what, short data1, long data2)
{
	internal_event	*theEvent;
	
	while ((theEvent = (internal_event *)gEventsFreeQueue.qHead) != NULL) {
		if (Dequeue((QElemPtr)theEvent, &gEventsFreeQueue) == noErr)
			break;
		}
			
	if (theEvent == NULL) {
		gQueueError = 1;					// Darn, we filled the queue, alert the user.
		return(-1);
		}
	else
		gQueueInUse++;
		
	theEvent->qType = 0;	
	theEvent->eclass = theclass;
	theEvent->event = what;
	theEvent->data1 = data1;
	theEvent->data2 = data2;
	
	Enqueue((QElemPtr)theEvent, &gEventsQueue);
	return(0);
}

/***************************************************************************/
/*  netputuev
*   put a unique event into the queue
*   First searches the queue for like events
*/
short netputuev(short tclass, short	what, short	data1, long	data2)
{
	internal_event	*theEvent = (internal_event *)gEventsQueue.qHead;
	
	while((theEvent != NULL) && ((theEvent->eclass != tclass) || (theEvent->event != what)
			|| (theEvent->data1 != data1) || (theEvent->data2 != data2)))
				theEvent = (internal_event *)theEvent->qLink;
				
	if (theEvent != NULL) return(0);
	
	return(netputevent(tclass, what, data1, data2));
}

void FlushNetwork(short scrn)
{
	short cnt;
	
	RSskip(screens[scrn].vs, 1);				/* Don't do any output */
	cnt = gBlocksize[scrn];						/* just needs to be positive to begin with */
	
	while (cnt>0) {
		cnt = netread(screens[scrn].port,gReadspace,gBlocksize[scrn]);
		parse( &screens[scrn ], gReadspace, cnt);
		}

	RSskip(screens[scrn].vs, 0);				/* Do output now */
	SetPort( screens[scrn].wind);
	InvalRect(&screens[scrn].wind->portRect);	/* Don't forget to redraw */
}

void pasteText(short scrn)
{
	short amount;
	long uploadLength;
	WindRec *tw = &screens[scrn];

	if (!tw->outlen)
		return;


	if (netpush(tw->port) != 0) {			/* BYU 2.4.16 - wait until not busy */
		netputevent( USERCLASS, PASTELEFT, scrn,0);	/* BYU 2.4.16 */
		return;										/* BYU 2.4.16 */
	}												/* BYU 2.4.16 */
	if (tw->incount) {					/* BYU 2.4.16 */
		tw->incount = 0;					/* BYU 2.4.16 */
		tw->outcount = 0;					/* BYU 2.4.16 */
		netputevent( USERCLASS, PASTELEFT, scrn,0);	/* BYU 2.4.16 */
		return;										/* BYU 2.4.16 */
	}												/* BYU 2.4.16 */
	if (tw->outcount < 2) {				/* BYU 2.4.16 */
		tw->outcount++;					/* BYU 2.4.16 */
		netputevent( USERCLASS, PASTELEFT, scrn,0);	/* BYU 2.4.16 */
		return;										/* BYU 2.4.16 */
	}
	if (netqlen(tw->port) > 0) {			/* BYU 2.4.16 - wait until not full */
		netputevent( USERCLASS, PASTELEFT, scrn,0);	/* BYU 2.4.16 */
		return;										/* BYU 2.4.16 */
	}												/* BYU 2.4.16 */

	// netwrite cannot send blocks larger than 32767...
	if ( !tw->pastemethod && tw->outlen < 32768 ) {	// Do this all at once?
		amount = netwrite(tw->port, tw->outptr, tw->outlen);
	} else { // Nope, do it in blocks
		if ( tw->pastemethod && tw->outlen > tw->pastesize )
			amount = tw->pastesize;
		else if ( tw->outlen > 32767 )
			amount = 32767;
		else
			amount = tw->outlen;
		amount = netwrite(tw->port, tw->outptr, amount);
	}

	if (tw->echo)	
		parse( tw,(unsigned char *) tw->outptr,amount);
	
	tw->outlen -= amount;	
	tw->outptr += (long) amount;

	if ( tw->isUploading && tw->outlen <= 0 ) {
		uploadLength = 16384;
		HUnlock( tw->outhand );
		SetHandleSize(tw->outhand, uploadLength);
		HLock( tw->outhand );
		FSRead(tw->uploadRefNum, &uploadLength, *tw->outhand);
		if ( uploadLength > 0 ) {
			SetHandleSize(tw->outhand, uploadLength);
			if ( GetTranslationIndex(tw->outnational) != kTRJIS )
				tw->outhand = htrbuf_mac_nat(tw, tw->outhand);
			if ( tw->outhand && !(tw->outlen = GetHandleSize(tw->outhand)) ) {
				DisposeHandle( tw->outhand );
				tw->outhand = NULL;
			}
			if ( tw->outhand ) {
				HLock(tw->outhand);
				tw->outptr = *tw->outhand;
				tw->outlen = GetHandleSize(tw->outhand);
				tw->clientflags |= PASTE_IN_PROGRESS;
				if (uploadLength != 16384) {
					tw->isUploading = 0;
					FSClose(tw->uploadRefNum);
				}
				tw->incount = 0;
				tw->outcount = 0;
				//pasteText(scrn);
				netputevent(USERCLASS, PASTELEFT, scrn, 0);
				return;
			} else {
				// translation failed...
				SysBeep(5);
				tw->outlen = 0;
			}
		}
	}

	if ( tw->outlen <= 0) {
		int left;
		tw->clientflags &= ~PASTE_IN_PROGRESS;
		if (tw->isUploading)
			FSClose(tw->uploadRefNum);
		if ( tw->outhand ) {
			HUnlock(tw->outhand);
			DisposeHandle(tw->outhand);
		}
		tw->outptr = (char *) 0L;	
		tw->outhand = (char **) 0L;	
		
		//this next loop is to unload the queue of stuff typed during the paste
		for (left = 0; left < tw->kblen; left++) 
		{
			if ((tw->kbbuf[left]) == 0) //this signals a VSkey
			{
				left++; //send the VS key
				VSkbsend(tw->vs, (unsigned char) tw->kbbuf[left], tw->echo, 0);
			}
			else
			{
				netpush(tw->port);
				if (tw->kbbuf[left+1] != 0x15) //not a KILLCHAR
				{	
					if (tw->kbbuf[left] == '\015') // remap CR-> CR LF
					{
						netwrite(tw->port,"\015\012",2);
						if (tw->echo)
							parse(tw,(unsigned char *) "\012\015",2);
						return;
					}
					netwrite(tw->port,&(tw->kbbuf[left]),1);
					if (tw->echo && (tw->kbbuf[left]>31) && (tw->kbbuf[left] <127))	
						parse(tw,(unsigned char *)&(tw->kbbuf[left]),1);
					else if (tw->echo)
					{
						tw->kbbuf[left] += '@';
						parse(tw,(unsigned char *) "^",1);	/* BYU LSC */
						parse(tw, (unsigned char *)&(tw->kbbuf[left]), 1);
					}
				}
				else
					left++; //user cancelled one keystroke with another
			}
		}
	}
	else
		netputevent( USERCLASS, PASTELEFT, scrn,0);
}

void DoNetEvents(void)
{
	long	ftptime = 0;
	short	event, evclass, data1;
	long	data2, pos;

	if ((event = netgetevent(&evclass, &data1, &data2)) < 0) return;

	if ( (TickCount() - ftptime > 60*2) && FileInTransit) {
		ftptime = TickCount();
		Sftpstat(&pos);					/* get transfer status */
		if (pos <= 0) 
			ftpmess("FTP Status: transferring\015\012");
		else 
			{
			char *temp;
			temp = myNewPtr(50);	//enough room for this string
			if (temp == NULL)
				return;  //Not enough memory to print status
			if (FileInTransit+2)
				sprintf((char *) temp,"FTP Status: %ld bytes remaining.\015\012", pos);		/* BYU LSC */
			else
				sprintf((char *) temp,"FTP Status: %ld bytes transferred.\015\012", pos);	/* BYU LSC */

			ftpmess((char *) temp);	/* BYU LSC */
			DisposePtr(temp);
			}
		}
		
	switch(evclass) {
		case SCLASS:
			switch (event) {
				case FTPACT:
					ftpd(0, data1);
					break;
				case CLOSEDONE:					/* Used in the drivers */
					netclose(data1);
					break;
				case CLOSEDONE+1:				/* Used in the drivers */
					netclose(data1);
					break;
				default:
					break;
			}
			break;
			
		case CONCLASS:							/* Connection type event */
			switch(GetPortType(data1)) {
				case PFTP:
					rftpd(event,data1);		/* BYU 2.4.16 */
					break;
				case PDATA:
					ftpd(event,data1);
					break;
				default:
				case CNXN_TYPE:
					switch (event) {
						case CONOPEN:				/* connection opened or closed */
							ConnectionOpenEvent(data1);
							break;
		 
						case CONDATA:				/* data arrived for me */
							ConnectionDataEvent(data1);
							break;
		
						case CONFAIL:
							ConnectionFailedEvent(data1);
							break;
		
						case CONCLOSE:
							ConnectionClosedEvent(data1);
							break;
		
						default:
							break;
						}
					break;	/* Case port type = CNXN_TYPE */
				}
			break;	/* CONCLASS */

	case USERCLASS:
			switch (event) {
				case DOMAIN_DONE:	// data1 = screen #, data2 = ptr to info structure
					HandleDomainDoneMessage(data1, data2);
					break;
				case DOMAIN_DONE2:	// data1 = screen #, data2 = ptr to info structure
					HandleDomainDone2Message(data1, data2);
					break;
				case FTPBEGIN:
					FileInTransit=data1;
					ftptime=TickCount();
					break;
				case FTPEND:
					ftpmess("FTP Transfer Concluding\015\012");
					ftpmess("\015\012");
					FileInTransit=0;
					break;
				case FTPCOPEN:
					{
						ip_addrbytes	ftpinfo;
						
						Str255	remoteMachineName;
						
						char *temp;
						if (gFTPServerPrefs->ResetMacBinary) 
							TelInfo->MacBinary = gFTPServerPrefs->UseMacBinaryII;
						updateMenuChecks();
						TelInfo->xferon=1;
						updateCursor(1);
						
						ftpmess("-----------------------------------------------------------------------------\015\012");
						ftpmess("FTP server initiated from host: ");
						temp = myNewPtr(80);
						ftpinfo.a.addr = Sftphost();

						if (temp != NULL) //enough memory to tell the log about the ftp session
						{
							if (!TranslateIPtoDNSname(ftpinfo.a.addr, remoteMachineName)) 
								sprintf((char *) &temp[4],"%u.%u.%u.%u\015\012",
									(unsigned char)ftpinfo.a.byte[0],
									(unsigned char)ftpinfo.a.byte[1],
									(unsigned char)ftpinfo.a.byte[2],
									(unsigned char)ftpinfo.a.byte[3]);
							else 
							{
								PtoCstr(remoteMachineName);
								sprintf((char *) &temp[4],"%s\015\012",remoteMachineName);
							}
						ftpmess((char *) &temp[4]);		/* BYU LSC */
						DisposePtr(temp);
						}
					}
					break;
				case FTPCLOSE:
					TelInfo->xferon=0;
					updateCursor(1);
					if (gFTPServerPrefs->ResetMacBinary) 
						TelInfo->MacBinary = gFTPServerPrefs->UseMacBinaryII;
					updateMenuChecks();
					ftpmess("FTP server ending session\015\012");
					ftpmess("-----------------------------------------------------------------------------\015\012");
					ftpmess("\015\012");

					break;
				case RG_REDRAW:			/* RGredraw event */
					if (VGalive(data1) && RGsupdate(data1))
						TekEnable(data1);
					break;
				case PASTELEFT:
					pasteText(data1);
					break;
				default:
					break;
			}
			break;	/* USERCLASS */

	default:
		break;
	} /* switch (CLASS) */
	
} /* DoNetEvents */
		
/*	setblocksize()
		Make sure that we have space for the block of data that is to be read each
		time the netread() is called.
		
		2.7: Different gBlockSize for each scrn, same gReadSpace  -- CCP */
		
short setReadBlockSize(short tosize, short scrn)
{
	short currentSize;
	gBlocksize[scrn] = tosize;							/* keep size of block */

	if (tosize < 512)							/* minimum buffer */
		gBlocksize[scrn] = 512;
	
	if (gReadspace == NULL)
	{
		gReadspace = (unsigned char *) myNewPtrCritical(tosize+100); 
		if (gReadspace == NULL)
			return -1;	//ick--lets handle this better
	}
	else
	{
		currentSize = (short) GetPtrSize((Ptr)gReadspace);
	
		if (currentSize < (tosize+100))	//increase buffer size, if possible
		{
			DisposePtr((Ptr)gReadspace);
			gReadspace = (unsigned char *)myNewPtrCritical(tosize + 100);
			if (gReadspace == NULL)
			{
				if  (NULL == (gReadspace = (unsigned char *)myNewPtrCritical(currentSize))) //try to get old size
					return(-1);				//ick--lets handle this better
				gBlocksize[scrn] = currentSize;
			}
		}
	}
	return(0);

}

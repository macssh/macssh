// network.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
*  NCSA Revisions:
*  10/87  Initial source release, Tim Krauskopf
*  2/88  typedef support for other compilers (TK)
*  8/88  Gaige Paulsen - support for MacTCP drivers
*  1/89  TK - conversion to new drivers, minor update for new calling convention
*  6/89  TK - update to MacTCP 1.0 include files and use of GetMyIPAddr()
*  7/92  Scott Bulmahn - add support for 2 global structs, put cursors into an array
*  11/92 Jim Browne - Fixed more bugs than I care to mention.					
*
*/

#include "InternalEvents.h"
#include "wind.h"
#include "maclook.proto.h"
#include "menuseg.proto.h"
#include "mainseg.proto.h"
#include "MyMacTCPstructures.h"
#include "network.proto.h"
#include "netevent.proto.h"
#include "tnae.h"
#include "authencrypt.h"
#include "authencrypt.proto.h"
#include "AddressXLation.h"
#include "event.proto.h"

#include "Connections.proto.h"
#include "sshglue.proto.h"

#include "errors.proto.h"
#include "telneterrors.h"
#ifdef THINK_C
#pragma options(!profile)
#endif

#include "ssh2.h"

#if TRACE_IT && TRACE_NET
	#define TRACEN(x)	TRACE(x)
#else
	#define TRACEN(x)
#endif

extern void ssh2_sched();

#ifdef __MWERKS__
#pragma profile off
#endif

//#define NET_DEBUG_MUCHINFO
//#define	NET_DEBUG
#ifdef	NET_DEBUG
char	net_debug_string[256];
#define	net_debug_print(x)	putln(net_debug_string)
#else
#define	net_debug_print(x)	
#endif

//PROTO_UPP(Notify, TCPNotify);
//static	TCPIOCompletionProcPtr		OpenCompleteInit(TCPiopb *pbp);
//PROTO_UPP(OpenCompleteInit, TCPIOCompletion);
//static	TCPIOCompletionProcPtr		CloseCompleteInit(TCPiopb *pbp);
//PROTO_UPP(CloseCompleteInit, TCPIOCompletion);
//static	TCPIOCompletionProcPtr		SendCompleteInit(TCPiopb *pbp);
//PROTO_UPP(SendCompleteInit, TCPIOCompletion);

static TCPIOCompletionUPP 	OpenCompleteInitUPP = nil;
static TCPIOCompletionUPP 	CloseCompleteInitUPP = nil;
static TCPIOCompletionUPP 	SendCompleteInitUPP = nil;
static TCPNotifyUPP			NotifyUPP = nil;


static	void		OpenComplete(MyTCPpbPtr pbp);
static	void		SendComplete(MyTCPpbPtr pbp);
static	void		CloseComplete(MyTCPpbPtr pbp);
short WindByPort2(short);

pascal void Notify(StreamPtr streamPtr, unsigned short code, Ptr uptr, 
						unsigned short terminReason, struct ICMPReport *icmpMsg);

static	wdsEntry	*getSWDS(void);
static	void		returnSWDS(wdsEntry *wds);
static	void		MyPBreturn(MyTCPpbPtr pbp);
static	MyTCPpb		*getPB(short driver,short call, StreamPtr stream, short usernum);
static	void		clearPB(MyTCPpbPtr pbp, short driver, short call, StreamPtr stream);
static	OSErr		xPBControlSync(MyTCPpbPtr pbp);
static	OSErr		xPBControlAsync(MyTCPpbPtr pbp, TCPIOCompletionUPP completionUPP);
/*static	short		makestream(void);*/
static	void		reclaim(StreamRPtr p);
static	short		giveback(StreamRPtr p, wdsEntry *wds);
static	void		compressfds(wdsEntry *fds);
static void NetWriteZero(Ptr pointer,long size); //put this here to keep from loading and unlaoding config segment
extern Cursor	*theCursors[];
extern WindRec	*screens;

long		MyA5;
short		TCPd = 0;					/* refnum of TCP drivers */
QHdr		gFreePBQueue;
QHdr		gFreeSWDSQueue;
short 		numSWDS=0;					/* Number of SWDS's ever alloc'd (PM Only) */
StreamRPtr	streams[NPORTS];
ip_port		nnfromport = 0;
Boolean 	nextIsUrgent = FALSE;		//CCP 2.7 (See NetUrgent ())

void NetWriteZero(Ptr pointer,long size)
{
	while (size--) *pointer++ = 0;
}

/**************************************************************************/

wdsEntry	*getSWDS(void)
{
	short		n=0;
	wdsEntry *	wds;
	
	if (gFreeSWDSQueue.qHead == NULL)
		wds = (wdsEntry *) myNewPtrCritical( sizeof(wdsEntry) * MAX_SWDS_ELEMS);	
	else
		{
		wds = (wdsEntry *) gFreeSWDSQueue.qHead;
		Dequeue(gFreeSWDSQueue.qHead, &gFreeSWDSQueue);
		}
		
	if (wds == NULL) {	/* sorry, cant allocate WDS (BIG PROBLEM) */
		DoError(508 | NET_ERRORCLASS, LEVEL3, NULL); // <- Is 508 correct?
		forcequit();
		// return ((wdsEntry*) 0L); <- ?? perhaps
		}

	return(wds);
}

/**************************************************************************/
void	returnSWDS(wdsEntry *wds)
{
	Enqueue((QElemPtr)wds, &gFreeSWDSQueue);
}

/**************************************************************************/
void MyPBreturn(MyTCPpbPtr pbp)
{
	Enqueue((QElemPtr)pbp, &gFreePBQueue);
}

/**************************************************************************/
MyTCPpb *getPB(short driver,short call, StreamPtr stream, short usernum)
{
	UNUSED_ARG(usernum)
	MyTCPpb *pbp;

	if (gFreePBQueue.qHead == NULL)
		pbp = (MyTCPpbPtr) myNewPtrCritical( sizeof(MyTCPpb) );
	else
		{
		pbp = (MyTCPpbPtr) gFreePBQueue.qHead;
		Dequeue(gFreePBQueue.qHead, &gFreePBQueue);
		}
		
	if (pbp == NULL) {	/* sorry, cant allocate TCP buffer (BIG PROBLEM) */
		DoError(508 | NET_ERRORCLASS, LEVEL3, NULL);
		forcequit();
		}
	
	memset((char *) pbp, '\0', sizeof(MyTCPpb));	
	pbp->pb.ioCRefNum = driver;
	pbp->pb.tcpStream = stream;
	pbp->pb.csCode = call;
	
	return(pbp);
}

/**************************************************************************/
void clearPB(MyTCPpbPtr pbp, short driver, short call, StreamPtr stream)
{
	NetWriteZero((Ptr)pbp, sizeof(MyTCPpb));	/* BYU LSC - Default to all zeros */	
	pbp->pb.ioCRefNum = driver;
	pbp->pb.tcpStream = stream;
	pbp->pb.csCode = call;
}

/**************************************************************************/
OSErr xPBControlSync(MyTCPpbPtr pbp) 
{ 
	pbp->pb.ioCompletion = 0L; 						/* Charlie will puke if he ever sees */
	return PBControlSync((ParmBlkPtr)(pbp)); 		/* his funtions here. -- JMB */
}

/**************************************************************************/
OSErr xPBControlAsync(MyTCPpbPtr pbp, TCPIOCompletionUPP completionUPP) 
{ 
#ifndef __powerpc__
	pbp->SavedA5 = MyA5;
#endif
	pbp->pb.ioCompletion = completionUPP; 
	return(PBControlAsync((ParmBlkPtr)(pbp)));		/* async */
}

void	SetPortType(short port, short type)
{
	if (port >= 0 && port < NPORTS && (streams[port] != NULL))
		streams[port]->portType = type;
}

short	GetPortType(short port)
{
	if (port >= 0 && port < NPORTS && (streams[port] != NULL))
		return(streams[port]->portType);
	else return(-1);
}

/**************************************************************************/
/*	Returns an empty stream */ 
short makestream(void)
{
	short		pnum, i;
	StreamRPtr	p;
	MyTCPpbPtr	pbp;

	for ( pnum=0; (streams[pnum]!= NULL) && pnum<NPORTS; pnum++)
			;	/* BYU 2.4.16 */
	
	if (pnum >= NPORTS)
		return(-2);

 	if (!(p = streams[pnum] = (StreamRPtr) myNewPtrCritical(sizeof(StreamRec))))
 		return(-1);
  
 	if ((p->buffer = (char *) myNewPtrCritical( TCPBUFSIZ)) == NULL) {
 		DisposePtr((Ptr)p);
 		streams[pnum] = 0;
  		return(-1);
 	}
 	if ((p->sbuffer = (char *) myNewPtrCritical( TCPBUFSIZ)) == NULL) {
 		DisposePtr((Ptr)p->buffer);
 		DisposePtr((Ptr)p);
 		streams[pnum] = 0;
  		return(-1);
 	}	
 
	
	for (i=0; i<MAX_FDS_ELEMS; i++) 
		{
		p->fds[ i].length =0; p->fds[ i].ptr = 0L;
		p->exFDS[ i].inuse=0; p->exFDS[ i].fds.length=0;p->exFDS[ i].fds.ptr=0L;
		}
	p->fds[0].length = TCPBUFSIZ;
	p->fds[0].ptr = p->sbuffer;
	p->maxFDSused=0;
	p->aedata = NULL;
	
	pbp=getPB( TCPd, TCPCreate, (long) 0, 7);	/* BYU LSC important - Make create call */
	pbp->pb.csParam.create.rcvBuff = p->buffer;
	pbp->pb.csParam.create.rcvBuffLen = TCPBUFSIZ;
	pbp->pb.csParam.create.notifyProc = NotifyUPP;

	if (xPBControlSync(pbp) != noError) ;
//		DoError (512 | NET_ERRORCLASS, LEVEL2, NULL);

	p->stream = pbp->pb.tcpStream;
	p->urgentMode = 0; // RAB BetterTelnet 2.0b2
	p->nextRecvUrgent = 0; // RAB BetterTelnet 2.0b2
	
	net_debug_print("Made new stream");
	MyPBreturn(pbp);
	return(pnum);
}

/**************************************************************************/
/*	reclaim( p) -
		reclaims buffer space to stream (from pointer p) into the FDS list  */
 
void reclaim(StreamRPtr p)
{
	short n=0, offset=0;
	
	while (offset < MAX_FDS_ELEMS && p->fds[offset].ptr != 0L) offset++;
	
	if (offset >= MAX_FDS_ELEMS) /* offset too large -- cant reclaim */		
		return;
		
	for (n=0 ; n<MAX_FDS_ELEMS && offset< MAX_FDS_ELEMS; n++) 
		{
		if (p->exFDS[ n].inuse) 
			{
			p->fds[ offset++]=p->exFDS[ n].fds;
			p->exFDS[ n].inuse = 0;
			}
		}
}

/**************************************************************************
	giveback( p, wds) - gives WDS entries back to the stream by putting them in the 
 		mutually exclusive buffer.
	p -> stream
	wds -> wds array			*/
short giveback(StreamRPtr p, wdsEntry *wds)
{
	short n=0, m=0;
	
	while ( n< MAX_SWDS_ELEMS && wds[n].ptr !=0L) 
		{
		while (m< MAX_FDS_ELEMS && p->exFDS[ m].inuse) m++;
		if (m >= MAX_FDS_ELEMS)									// This was off by one - JMB 2.6
			return(-1);				/* No room in the RECLAIMation center */
		else 
			{
			p->exFDS[ m].inuse =1;
			p->exFDS[ m].fds = wds[n];
			m++;
			}
		n++;
		}

	return 0;
}

/**************************************************************************/
/*	compressfds( fds)
		compress an fds data structure to make everyone happy */
void compressfds(wdsEntry *fds)
{
	short n,m,compressed;
	
	compressed = 0;
	
	while ( !compressed) {
		compressed=1;
		for (n=0; n< MAX_FDS_ELEMS; n++) 
			{				/* Slow Forwards */
			if (fds[n].ptr) 
				{								/* Do if N exists */
				for ( m = MAX_FDS_ELEMS -1; m>=0; m--) 
					{	/* Fast Backwards */
					if (fds[m].ptr && (fds[m].ptr+fds[m].length == fds[n].ptr)) {
						fds[n].length+=fds[m].length;
						fds[n].ptr = fds[m].ptr;
						fds[m].ptr=0L;
						fds[m].length=0;
						compressed=0;
						}
#ifdef CHECKBOTHWAYZ
					else 
					if (fds[n].ptr+fds[n].length == fds[m].ptr) 
						{
						fds[m].length+=fds[n].length;
						fds[n].ptr=0L;
						fds[n].length=0;
						compressed=0;
						}
#endif CHECKBOTHWAYZ
					}
				}
			}
		}
	m=0;n=0;
	
	/* Close the gaps */
	
	while (n+m < MAX_FDS_ELEMS) 
		{
		while (fds[n+m].ptr ==0L && n+m< MAX_FDS_ELEMS)
			m++;			/* increase gap to valid entry */
		if (n+m<MAX_FDS_ELEMS)
			fds[n]=fds[n+m];
		n++;
		}
	
	/* Get rid of the empty spaces */
	
	n--;		/* for the next loop */
	while (n < MAX_FDS_ELEMS)
		{
		fds[n].ptr=0;
		fds[n++].length=0;
		}
}

/***************************************************************************/
/*  Mnetread
*   Read from a connection buffer into a user buffer.  
*   Returns number of bytes read, < 0 on error
* NOTE:
*	current version very inefficient, but hopefully works.
*/
short	netread(short pnum, void *buffer, short n)
{
	short		i;
	StreamRPtr	p;
	MyTCPpbPtr	pbp;
	short		inQ, reqdamt;

	if (pnum < 0 || pnum >= NPORTS)			/* BYU 2.4.15 */
		return(-2);

/* NONO */
	i = WindByPort(pnum);									/* BYU */
	if ( i >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		lshcontext *context = (lshcontext *)wind->sshdata.context;
		reqdamt = 0;
		if (context && context->_gConsoleOutBufLen) {
			reqdamt = context->_gConsoleOutBufLen;
			if ( reqdamt > n ) {
				reqdamt = n;
			}
			BlockMoveData( context->_gConsoleOutBuf, buffer, reqdamt );
			context->_gConsoleOutBufLen -= reqdamt;
			if ( context->_gConsoleOutBufLen ) {
				BlockMoveData( context->_gConsoleOutBuf + reqdamt, context->_gConsoleOutBuf, context->_gConsoleOutBufLen );
				netputuev( CONCLASS, CONDATA, pnum,0);			/* more data to get */
			}
			TRACEN(PRINTF("netread, wind:%d, port:%d\n", i, pnum ));
			//TRACEN(DUMP("netread\n", buffer, reqdamt));
		}
		return reqdamt;
	}
/* NONO */

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPStatus, p->stream, 1);				/* Make status call */
	if (xPBControlSync(pbp) != noError) 					/* status call bombed */
//		DoError (509 | NET_ERRORCLASS, LEVEL1, "Mnetread");
;

	if (pbp->pb.csParam.status.connectionState !=8) {
#ifdef	NET_DEBUG
		sprintf(net_debug_string, "CState: %d is %d",(int)pnum, (int)pbp->pb.csParam.status.connectionState);
		net_debug_print(net_debug_string);
#endif
		MyPBreturn(pbp);
		return(-1);					  		/* Connection not established */
		}
	
	inQ = pbp->pb.csParam.status.amtUnreadData;
	reqdamt = n >inQ ? inQ : n;
	
	clearPB( pbp, TCPd, TCPRcv, p->stream);
	pbp->pb.csParam.receive.rcvBuff = buffer;
	pbp->pb.csParam.receive.rcvBuffLen = reqdamt;
	
	if (reqdamt<1) 
		{								/* Drop out if no data */
		MyPBreturn(pbp);
		return(0);
		}
	
	if ((i = xPBControlSync(pbp)) != noError) {
//		DoError (524 | NET_ERRORCLASS, LEVEL1, NULL);
		return(-1);
		}
		
	reqdamt = pbp->pb.csParam.receive.rcvBuffLen;
	if (reqdamt<inQ) 
		netputuev( CONCLASS, CONDATA, pnum,0);			/* more data to get */

	// RAB BetterTelnet 2.0b2 - handle urgent data (for rlogin options, etc.)

	p->urgentMode = 0;
	if (pbp->pb.csParam.receive.urgentFlag)
		p->urgentMode = 1;
	if (p->nextRecvUrgent) p->urgentMode = 1;
	if (p->urgentMode) {
		if (pbp->pb.csParam.receive.markFlag) p->nextRecvUrgent = 0;
		else p->nextRecvUrgent = 1;
	}

 	/* Decrypt data */
	if (p->aedata && ((tnParams *)p->aedata)->decrypting)
		kdecrypt((tnParams *)p->aedata, buffer, (long)reqdamt);
 
	MyPBreturn(pbp);
	return(reqdamt);
}

//makes the next buffer be sent as urgent data
void netUrgent(void)
{
	nextIsUrgent = TRUE;
}
/************************************************************************/
short WindByPort2(short port)
{
	short i=0;

	while (i<TelInfo->numwindows &&
			(screens[i].port != port || 
				((screens[i].active != CNXN_ACTIVE) && (screens[i].active != CNXN_OPENING)))
			) i++;

	if (i>=TelInfo->numwindows) {					/* BYU */
		putln("Can't find a window for the port # in WindByPort2");	/* BYU */
		if (i==0) i=999;		/* BYU */
		return(-i);				/* BYU */
		}						/* BYU */

	return(i);
}


short netwrite(short pnum, void *buffer, short nsend)
{
	short i;

	i = WindByPort2(pnum);
	if (i < 0) return Rnetwrite(pnum, buffer, nsend);

/* NONO */
	if ( screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		lshcontext *context = (lshcontext *)wind->sshdata.context;
		short n = nsend;
		TRACEN(PRINTF("netwrite, wind:%d, port:%d\n", i, pnum ));
		//TRACEN(DUMP("netwrite\n", buffer, nsend));
		nsend = 0;
		if (context) {
			while (n > 0) {
				if ( context->_gConsoleInBufMax - context->_gConsoleInBufLen > 0  ) {
					long len = n;
					if ( len > context->_gConsoleInBufMax - context->_gConsoleInBufLen )
						len = context->_gConsoleInBufMax - context->_gConsoleInBufLen;
					BlockMoveData( buffer, context->_gConsoleInBuf + context->_gConsoleInBufLen, len);
					context->_gConsoleInBufLen += len;
					(char *)buffer += len;
					n -= len;
					nsend += len;

					if (context->_insock) {
						extern void GUSIWakeupTTYSocket( void *insock );
						GUSIWakeupTTYSocket(context->_insock);
					}

				} else {
					ssh2_sched();
				}
			}
		}
		return nsend;
	}
/*
	if (screens[i].protocol == 4) {
		ssh_glue_write(&screens[i], buffer, nsend);
		return nsend;
	}
*/
/* NONO */


	return Rnetwrite(pnum, buffer, nsend);
}

/* netwrite
*  write something into the output queue, netsleep routine will come
*  around and send the data, etc.
*
*/
short	Rnetwrite(short pnum, void *buffer, short nsend)
{	
	StreamRPtr	p;
	wdsEntry	*swds;
	short		remaining, queued, n,m;
	MyTCPpbPtr	pbp;
	OSErr		err;
	short i;

#ifdef	NET_DEBUG_MUCHINFO
	sprintf(net_debug_string, "port: %d\tbuffer: %lx\tnsend %d", pnum,
		(unsigned int)buffer, (int)nsend);
	net_debug_print(net_debug_string);
#endif

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

	if ( (p = streams[pnum]) == NULL)
		return(-2);
	
	if ( !nsend )
		return(0);

/* NONO */
	i = WindByPort(pnum);
	if ( i >= 0 && screens[i].protocol == 4 ) {
		//TRACE(PRINTF("### Rnetwrite\n"));
		//TRACEN(DUMP("Rnetwrite\n", buffer, nsend));
		return nsend;
	}
/* NONO */


	swds = getSWDS();
	
	if (swds == NULL) return(nsend);		// Duh, Mr. Bulmahn. - JMB 2.6
	
	reclaim( p);

	compressfds( p->fds);

	n=0; remaining = nsend;

//	if (p->fds[0].ptr == 0) DebugStr("\pArgh! fds[0] is NULL!");
	if (p->fds[0].ptr == 0) return 0; // argh!
	
	while (p->fds[n].ptr !=0 && remaining>0 ) {
//		if (n >= MAX_SWDS_ELEMS) DebugStr("\pOverflowing SWDS in while loop!");
		
		swds[n].ptr = p->fds[n].ptr;
		if ( p->fds[n].length > remaining) {
			swds[n].length = remaining;
			p->fds[n].length -= remaining;
			p->fds[n].ptr += remaining;
			remaining=0;
			}
		else 
			{
			swds[n].length =  p->fds[n].length;
			remaining -= p->fds[n].length;
			p->fds[n].length = 0;
			p->fds[n].ptr = 0;
			}
		n++;
		}

	if (n>p->maxFDSused) p->maxFDSused=n;
	
	compressfds( p->fds);

	queued = nsend-remaining;
	
	for(m=0; m<n; m++) {
#ifdef	NET_DEBUG_MUCHINFO
	sprintf(net_debug_string, "swds[m].ptr: %lx\tswds[m].length: %d", (unsigned int)swds[m].ptr, (int)swds[m].length);
	net_debug_print(net_debug_string);
#endif
		}
		
	for (m=0; m<n; m++) 
		{
		BlockMoveData(buffer, swds[m].ptr, swds[m].length);	/* BYU LSC - Put data in WDS */
	
		/*
		 * encrypt if encrypting the session
		 */
		if (p->aedata && ((tnParams *)p->aedata)->encrypting)
			kencrypt((tnParams *)p->aedata, (unsigned char *)swds[m].ptr, swds[m].length);
		buffer=(void *)((char *)buffer + swds[m].length);
		}
	
	swds[m].ptr = 0L;
	swds[m].length=0;
	
	pbp=getPB( TCPd, TCPSend, p->stream, 2);			/* Make send call */
	pbp->pb.csParam.send.wdsPtr = (Ptr) swds;
	pbp->pb.csParam.send.pushFlag = p->push;
	pbp->pb.csParam.open.ulpTimeoutValue = (SInt8)gApplicationPrefs->SendTimeout;// timeout value
	pbp->pb.csParam.open.ulpTimeoutAction = 1;				// Abort
	pbp->pb.csParam.open.validityFlags = 0xC0;				// Timeout and action are valid
	if (nextIsUrgent)
	{
		nextIsUrgent = FALSE;
		pbp->pb.csParam.send.urgentFlag = TRUE;
	}

	p->push=0;

	if ((err = xPBControlAsync(pbp, SendCompleteInitUPP)) != noError) 
		{
		char temp_xvvyz[256];
		
		sprintf(temp_xvvyz, "netwrite: %d", err);
//		DoError(510 | NET_ERRORCLASS, LEVEL2, temp_xvvyz);
		return(-1);
		}

	return(queued);
}

/**************************************************************************/
/*  Mnetpush
*   attempt to push the rest of the data from the queue
*   and then return whether the queue is empty or not (0 = empty)
*   returns the number of bytes in the queue.
*/
short netpush(short pnum)
{
	StreamRPtr	p;
	MyTCPpbPtr	pbp;
	short		inQ;
	OSErr		err;
	short i;

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

/* NONO */
	i = WindByPort(pnum);
	if ( i >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		lshcontext *context = (lshcontext *)wind->sshdata.context;
		TRACEN(PRINTF("netpush\n"));
		if (context) {
			unsigned long startTicks = LMGetTicks();
			while ( context->_gConsoleInBufLen && LMGetTicks() - startTicks < 15 ) {
				ssh2_sched();
			}
			return context->_gConsoleInBufLen;
		} else {
			return 0;
		}
	}
/* NONO */

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPStatus, p->stream, 3);			/* Make status call */
//#ifdef	NET_DEBUG_MUCHINFO
//	sprintf(net_debug_string, "TCPd == %d", TCPd);
//	net_debug_print(net_debug_string);
//#endif
	if ((err = xPBControlSync(pbp)) != noError) {		/* status call bombed */
		char temp_xvvyz[256];

		sprintf(temp_xvvyz, "netpush: %d", err);
//		DoError(509 | NET_ERRORCLASS, LEVEL2, temp_xvvyz);
		}
		
	inQ = pbp->pb.csParam.status.amtUnackedData;
	MyPBreturn( pbp);
	
	p->push=1;
	return(inQ);
}	

/**************************************************************************/
/*  Mnetqlen
*   return the number of bytes waiting to be read from the incoming queue.
*/
short netqlen(short pnum)
{
	StreamRPtr	p;
	MyTCPpbPtr	pbp;
	short		inQ;
	short i;

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

/* NONO */
	i = WindByPort(pnum);
	if ( i >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		lshcontext *context = (lshcontext *)wind->sshdata.context;
		TRACEN(PRINTF("netqlen\n"));
		if ( context ) {
			return context->_gConsoleOutBufLen;
		} else {
			return 0;
		}
	}
/* NONO */

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPStatus, p->stream, 4);				/* Make status call */
	if (xPBControlSync(pbp) != noError) 					/* status call failed */
//		DoError(509 | NET_ERRORCLASS, LEVEL2, "Mnetqlen");
;

	inQ = pbp->pb.csParam.status.amtUnreadData;
	MyPBreturn( pbp);
	
	p->push = 1;
	return(inQ);
}

/**************************************************************************/
/*  Mnetroom()
*	return how much room is available in output buffer for a connection
*/
short netroom(short pnum)
{
	StreamRPtr	p;
	short		inQ, n;
	short i;

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

/* NONO */
	i = WindByPort(pnum);
	if ( i >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		lshcontext *context = (lshcontext *)wind->sshdata.context;
		TRACEN(PRINTF("netroom\n"));
		if (context) {
			return context->_gConsoleInBufMax - context->_gConsoleInBufLen;
		} else {
			return 0;
		}
	}
/* NONO */

	if (NULL == (p = streams[pnum]))
		return(-2);

	reclaim( p);
	compressfds( p->fds);

#ifdef OLDM
	pbp=getPB( TCPd, TCPStatus, p->stream, 5);			/* Make status call */
	if (xPBControlSync(pbp) != noError) ;
//		DoError(509 | NET_ERRORCLASS, LEVEL1, "Mnetroom");

	inQ = pbp->csParam.status.sendWindow -
				pbp->csParam.status.amtUnackedData;
	MyPBreturn( pbp);
#else
/*#pragma unused(pbp)	/* BYU LSC */
#endif

	inQ = n = 0;
	while (p->fds[n].ptr) {
	
		inQ += p->fds[n].length;				/* add up free list space */
		n++;
		}

	return(inQ);
}

/**************************************************************************/
void netgetip(unsigned char *st)
{
	struct GetAddrParamBlock mypb;
	/* long netmask; */

	net_debug_print("Attempting getmyipaddr");
	
	NetWriteZero((Ptr)&mypb, sizeof(struct GetAddrParamBlock));
	
	mypb.ioCRefNum = TCPd;			/* BYU LSC - TCP driver has to be open by now */
	mypb.csCode = ipctlGetAddr;

	if (PBControlSync((ParmBlkPtr)&mypb) != noError) 
		{
		DoError(511 | NET_ERRORCLASS, LEVEL2, NULL);
		return;
		}
	
	BlockMoveData(&mypb.ourAddress, st, 4);	/* BYU LSC - copy the address */
	/* netmask is here if we want it, too */
}

// chas BetterTelnet 2.0b4 - for k5
short netgetport(short pnum)
{
	StreamRPtr p;
	MyTCPpbPtr pbp;
	short port;
	short i;

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

/* NONO */
	i = WindByPort(pnum);
	if ( i >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		TRACEN(PRINTF("netgetport\n"));
		// FIXME: this is completely wrong...
		// we should return a local port to send
		// unencrypted data from the module.
		// but there's no such port...
		return wind->port;
	}
/* NONO */

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPStatus, p->stream, 4);				/* Make status call */
	if (xPBControlSync(pbp) != noError) 					/* status call failed */
		DoError(509 | NET_ERRORCLASS, LEVEL2, "Mnetgetport");

	port = pbp->pb.csParam.status.localPort;
	MyPBreturn( pbp);

	return port;
}

void netfromport			/* next "open" will use this port */
  ( short port)
{
	nnfromport = (ip_port)port;
}

/**************************************************************************/
/*  Mnetest?
*  is a particular session established yet?
*  Returns 0 if the connection is in the established state.
*/
short netest(short pnum)
{
	StreamRPtr	p;
	MyTCPpbPtr	pbp;
	short		inQ;
	short i;

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

/* NONO */
	i = WindByPort(pnum);
	if ( i >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		lshcontext *context = (lshcontext *)wind->sshdata.context;
		TRACEN(PRINTF("netest\n"));
		return context == NULL ? -1 : 0;
	}
/* NONO */

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPStatus, p->stream, 6);			/* Make status call */
	if (xPBControlSync(pbp) != noError)
		{
		DoError(509 | NET_ERRORCLASS, LEVEL2, "Mnetest");
		inQ = -1;
		}
	else 
		inQ = pbp->pb.csParam.status.connectionState !=8;
	MyPBreturn( pbp);
	
	return(inQ);
}

/**************************************************************************/
/*  Mnetlisten
*   Listen to a TCP port number and make the connection automatically when
*   the SYN packet comes in.  The TCP layer will notify the higher layers
*   with a CONOPEN event.  Save the port number returned to refer to this
*   connection.
*
*   usage:   portnum = netlisten(service);
*            short service;
*
*/

short netlisten(ip_port serv)
{
	short		pnum;
	StreamRPtr	p;
	MyTCPpbPtr	pbp;

	pnum = makestream();

	if (pnum < 0 || pnum >= NPORTS)		/* BYU 2.4.15 */
		return(-2);

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPPassiveOpen, p->stream, 8);			/* Make Listen call */
	
	pbp->pb.csParam.open.localPort = serv;
	
	if (xPBControlAsync(pbp, OpenCompleteInitUPP) != noError) 
		DoError(513 | NET_ERRORCLASS, LEVEL2, NULL);

	return(pnum);						/* BYU 2.4.16 */
}

/***********************************************************************/
/*  Mnetgetftp
*  Provides the information that ftp needs to open a stream back to the
*  originator of the command connection.  The other side's IP number
*  and the port numbers to be used to calculate the default data connection
*  number.  Returns values in an integer array for convenient use in 
*  PORT commands.
*/
short	netgetftp(short pnum, ip_addr *addr, ip_port *localPort, ip_port *remotePort)
{
	StreamRPtr p;
	MyTCPpbPtr pbp;
	
	if (pnum < 0)
		return(-2);

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPStatus, p->stream, 9);			/* Make status call */
	if (xPBControlSync(pbp) != noError) 
		DoError(514 | NET_ERRORCLASS, LEVEL2, NULL);

	*addr = pbp->pb.csParam.status.remoteHost;
	*localPort = pbp->pb.csParam.status.localPort;
	*remotePort = pbp->pb.csParam.status.remotePort;
	MyPBreturn( pbp);

	return 0;
}

/**************************************************************************/
/*  Open a network socket for the user. */
short netxopen( ip_addr machine, ip_port service,
				short	timeout	)		// in seconds
{
	short		pnum, err;
	StreamRPtr	p;
	MyTCPpbPtr	pbp;

	pnum = makestream();

	if (pnum < 0)
		return(-2);

	if (NULL == (p = streams[pnum]))
		return(-2);

	pbp=getPB( TCPd, TCPActiveOpen, p->stream, 10);			/* Make Listen call */
	
	pbp->pb.csParam.open.remoteHost = machine;				/* IP # */
	pbp->pb.csParam.open.remotePort = service;				/* Port */
	pbp->pb.csParam.open.localPort = nnfromport;			/* My Port */
	pbp->pb.csParam.open.ulpTimeoutValue = (SInt8)timeout;	// timeout value
	pbp->pb.csParam.open.ulpTimeoutAction = 1;				// Abort
	pbp->pb.csParam.open.validityFlags = 0xC0;				// Timeout and action are valid
	nnfromport=0;											/* Next one is random */

// RAB BetterTelnet 2.0b2
// we handle port rotation for privileged ports for rlogin here
// just in case the call returns synchronously

	err = -23017;

	while (err == -23017) {
		err = xPBControlAsync(pbp, OpenCompleteInitUPP);
		if (err == -23017)
			pbp->pb.csParam.open.localPort++;
	}

	if (err != noError) {
		DoError(515 | NET_ERRORCLASS, LEVEL2, NULL);
		return(-1);
		}
	
	SetPortType(pnum, NO_TYPE);				// Is allocated to the user
	
#ifdef	NET_DEBUG
	sprintf(net_debug_string, "TCPOpen on port #%d", pnum);
	net_debug_print(net_debug_string);
#endif
	return(pnum);					/* BYU 2.4.16 */
}


/**************************************************************************/
/* Mnetclose
*  Do appropriate actions to return connection state to SCLOSED which
*  enables the memory for that port to be reused.
*
*	Specifically:
*		o If status is closed, then release the data structures
*		o If status is not close, perform bkgrd close which generates CLOSEDONE,
*			which should make the session layer call us again
*/
short netclose(short pnum)
{
	StreamRPtr	p;
	MyTCPpbPtr	pbp;
	OSErr		errorCode;
	short		status;
	static		short count=0;
	short i;

	errorCode = 0;
	
	if (pnum < 0 || pnum >= NPORTS)			/* is a valid port? */
		return(-1);


	if ((p = streams[pnum]) == NULL) 			/* nothing there */
		return (1);



/* NONO */
	if ( (i = WindByPort(pnum)) >= 0 && screens[i].protocol == 4 ) {
		WindRec *wind = &screens[i];
		TRACEN(PRINTF("netclose\n"));

		pbp = getPB( TCPd, TCPStatus, p->stream, 11 );			/* Make status call */
		if (pbp) {
			clearPB( pbp, TCPd, TCPRelease, p->stream );			/* Make Release call */
			xPBControlSync( pbp );
		}

		DisposePtr( p->buffer);				/* Free Receive Buffer */
		DisposePtr( p->sbuffer);				/* Free Send Buffer */
		DisposePtr((Ptr) p);					/* Free Stream Structure */
		streams[pnum]=0L;
		netportencryptstate(pnum, 0);
		if (wind->sshdata.context) {
			ssh_glue_close(wind);
		} else {
			//netputuev(SCLASS, CLOSEDONE, pnum,0);
			netputevent(SCLASS, CLOSEDONE, pnum,0);
		}

		return 0;
	}


	pbp=getPB( TCPd, TCPStatus, p->stream, 11);			/* Make status call */
	if ((errorCode = xPBControlSync(pbp)) != noError) 
		{
		if ( errorCode == invalidStreamPtr)
		{
//			DoError (516 | NET_ERRORCLASS, LEVEL2, "Mnetclose");
			return(-1);
		}
		else
			{
			status=0;
			count =0;
			}
		}
	else 
		{
		status = pbp->pb.csParam.status.connectionState;			/* The connection Status */
		if (count++ ==10) 
			{
			status= 0;
			count =0;
			}
		}


#ifdef	NET_DEBUG
	sprintf(net_debug_string, "Mnetclose:error code=%i status=%d",(int)errorCode, status);
	net_debug_print(net_debug_string);
#endif

	if (status < 18 && status >2 ) {	/* We aren't closed yet ! */
#ifdef	NET_DEBUG
		sprintf(net_debug_string, "TCPClose being attempted state ...[%d]",status); /* Prolly because outstanding close */
		net_debug_print(net_debug_string);
#endif
		clearPB( pbp, TCPd, TCPClose, p->stream);			/* Make Close call */
		if ((errorCode=xPBControlAsync(pbp, CloseCompleteInitUPP)) != noError) 
			{			/* TCP close failed */
//			DoError(517 | NET_ERRORCLASS, LEVEL2, "Mnetclose");
			return (errorCode);
			}

		/* Go ahead and return... PB is in use by Async Close call. */
		return (0);											/* Return with OK */
		}

	/* IF we got here, we must be at closed state, so free memory */

#ifdef	NET_DEBUG
	sprintf(net_debug_string, "TCP being released, status = %d", status);
	net_debug_print(net_debug_string);
#endif

	clearPB( pbp,TCPd, TCPRelease, p->stream);			/* Make Release call */
	if (xPBControlSync(pbp) != noError) 
		{				/* TCP failed to be released.  Let us know... */
//		DoError (518 | NET_ERRORCLASS, LEVEL2, "Mnetclose");
		return (-1);
		}
	
	DisposePtr( p->buffer);				/* Free Receive Buffer */
	DisposePtr( p->sbuffer);				/* Free Send Buffer */
	DisposePtr((Ptr) p);					/* Free Stream Structure */
	streams[pnum]=0L;
	netportencryptstate(pnum, 0);

	MyPBreturn(pbp);
	return(0);
}

/**************************************************************************/
/*	Nuke the connection, NOW! */
short	netabort(short pnum)
{
	StreamRPtr	p;
	MyTCPpbPtr	pbp;
	OSErr		errorCode = noErr;
	short i;

	if (pnum < 0 || pnum >= NPORTS)			/* is a valid port? */
		return(-1);

	if ((p = streams[pnum]) != NULL) 	/* something there */
	{
		i = WindByPort(pnum);
		if ( i >= 0 && screens[i].protocol == 4 ) {
			WindRec *wind = &screens[i];
			lshcontext *context = (lshcontext *)wind->sshdata.context;
			if (context) {
				ssh_glue_close(wind);
			}
			return 0;
		} else {
			pbp=getPB( TCPd, TCPAbort, p->stream, 12);			/* Make Close call */
			if ((errorCode=xPBControlSync(pbp)) != noError) ;		/* TCP abort failed.  Bad news */
	//			DoError (519 | NET_ERRORCLASS, LEVEL1, NULL);
			clearPB( pbp,TCPd, TCPRelease, p->stream);			/* Make Close call */
			if (xPBControlSync(pbp) != noError) 
				{
	//			DoError (518 | NET_ERRORCLASS, LEVEL2, NULL);
				return(-1);
				}
			}
		}
	else
		return(1);
	
	DisposePtr( p->buffer);				/* Free Receive Buffer */
	DisposePtr( p->sbuffer);				/* Free Send Buffer */
	DisposePtr((Ptr) p);					/* Free Stream Structure */
	streams[pnum]=0L;

	MyPBreturn(pbp);
	return(0);
}


/**************************************************************************/
/*  Mnetinit
*   Calls all of the various initialization routines that set up queueing
*   variables, static values, reads configuration files, etc.
*/

void Mnetinit( void)
{
	short	i;
	OSErr	err;

	if (TelInfo->startedTCP) return; // demand-load TCP (RAB BetterTelnet 1.0fc4)
	TelInfo->startedTCP = 1; // RAB BetterTelnet 1.0fc4

#ifndef __powerpc__	
	MyA5 = SetCurrentA5();
#endif

	OpenResolver(NULL); // open DNS and discard any errors (RAB BetterTelnet 1.0fc4)

	for (i=0; i<NPORTS;i++) {
		streams[i]= (StreamRPtr) 0;
		} 
		
	gFreePBQueue.qHead = NULL;
	gFreePBQueue.qTail = NULL;
	gFreePBQueue.qFlags = 0;
	gFreeSWDSQueue.qHead = NULL;
	gFreeSWDSQueue.qTail = NULL;
	gFreeSWDSQueue.qFlags = 0;
		
	if ((err = OpenDriver("\p.IPP",&TCPd)) != noError) 		/* cant open the IP driver */
		{	
		setLastCursor(theCursors[normcurs]);
		FatalAlert(CANT_OPEN_MACTCP, 300, err);		// Doesn't return!
		}
}



/*************************************************************************/
/*  Mnetshut
*   Close all the connections and turn off the hardware.
*/
void netshut(void)
{
	short	i;

	for (i=0; i < NPORTS ; i++) 
		if (streams[i] != (StreamRPtr) NULL)
			netabort(i);						/* Prolly should abort */
}

/**************************************************************************/
short	findbystream(StreamPtr streamPtr)
{
	short	pnum=0;
	
	while (pnum<NPORTS &&
			( (streams[pnum] ==(StreamRPtr)0L) || (streamPtr !=streams[pnum]->stream)))
		pnum++;
	if (pnum >=NPORTS)
		return(-1);
	else
		return(pnum);
}


/**************************************************************************/
pascal void Notify(StreamPtr streamPtr, unsigned short code, Ptr uptr, 
						unsigned short terminReason, struct ICMPReport *icmpMsg)
{
	UNUSED_ARG(uptr)
	UNUSED_ARG(terminReason)
	UNUSED_ARG(icmpMsg)
	StreamRPtr	p;
	short		pnum;
	
	pnum = findbystream(streamPtr);
	
	if (pnum < 0 || (p = streams[pnum]) == 0L)
		return;
	
	switch( code) {
		case TCPTerminate:
		case TCPClosing:
			//netputuev(CONCLASS, CONCLOSE, pnum,0);
			netputevent(CONCLASS, CONCLOSE, pnum,0);
			break;
		case TCPULPTimeout:
			//netputuev(CONCLASS, CONFAIL, pnum,0);
			netputevent(CONCLASS, CONFAIL, pnum,0);
			break;
		case TCPDataArrival:
		case TCPUrgent:
			netputuev(CONCLASS, CONDATA, pnum,0);
			break;
		case TCPICMPReceived:
		default:
			break;
		}
	return;
}

/**************************************************************************/
static void OpenCompleteInit(TCPiopb *pbp)
{
	long		saveA5;
	MyTCPpbPtr	myptr;
	
	myptr = (MyTCPpbPtr)pbp;

#ifndef _powerpc_
	saveA5 = SetA5(myptr->SavedA5);
#endif

	OpenComplete(myptr);

#ifndef _powerpc_
	SetA5(saveA5);
#endif
}

/**************************************************************************/
void	OpenComplete(MyTCPpbPtr pbp)
{
	StreamRPtr	p;
	short		pnum;

	pnum= findbystream(pbp->pb.tcpStream);
	
	if (pnum < 0 || (p = streams[pnum]) == 0L) { 
		MyPBreturn(pbp); //BUGG:  Shouldn't we return here so we don't MyPBreturn twice? 
		return;			 //I'll assume so (CCP)
	}
	if (pbp->pb.ioResult == -23017) {
		//netputuev(CONCLASS, CONFAIL, pnum, -23017);
		netputevent(CONCLASS, CONFAIL, pnum, -23017);
		streams[pnum]->pbp = pbp; // save the params
		return;					 // and return
	} else streams[pnum]->pbp = 0;

	if (pbp->pb.ioResult !=noError) {
		//netputuev(CONCLASS, CONFAIL, pnum,pbp->pb.ioResult);	/* Failure ... */
		netputevent(CONCLASS, CONFAIL, pnum,pbp->pb.ioResult);	/* Failure ... */
	} else {
		//netputuev(CONCLASS, CONOPEN, pnum,0);			/* Success ! */
		netputevent(CONCLASS, CONOPEN, pnum,0);			/* Success ! */
	}
	
	MyPBreturn( pbp);
}

/**************************************************************************/
void	SendComplete(MyTCPpbPtr pbp)
{
	StreamRPtr	p;
	short		pnum;
	wdsEntry	*swds;

	swds = (wdsEntry *) pbp->pb.csParam.send.wdsPtr;
	
	pnum = findbystream(pbp->pb.tcpStream);
	
	if (pnum < 0 || (p = streams[pnum]) == 0L) {
		MyPBreturn(pbp);
		return;
		}
		
	MyPBreturn( pbp);
	giveback( p, (wdsEntry *) pbp->pb.csParam.send.wdsPtr);	/* BYU LSC - Give this back.... NOW */
	returnSWDS( swds);
}


/**************************************************************************/
static void SendCompleteInit(TCPiopb *pbp)
{
	long		saveA5;
	MyTCPpbPtr	myptr;
	
	myptr = (MyTCPpbPtr)pbp;

#ifndef _powerpc_
	saveA5 = SetA5(myptr->SavedA5);
#endif
	
	SendComplete(myptr);
	
#ifndef _powerpc_
	SetA5(saveA5);
#endif
}

/**************************************************************************/
void	CloseComplete(MyTCPpbPtr pbp)
{
	StreamRPtr	p;
	short		pnum;

	pnum= findbystream(pbp->pb.tcpStream);
	
	if (pnum < 0 || (p = streams[pnum]) == 0L)
	  {
		//netputuev(SCLASS, CLOSEDONE+1, pnum,0);
		netputevent(SCLASS, CLOSEDONE+1, pnum,0);
		MyPBreturn(pbp);
		return;
	  }
		
	if (pbp->pb.ioResult !=noError) {
		//netputuev(SCLASS, CLOSEDONE+1, pnum,0);
		netputevent(SCLASS, CLOSEDONE+1, pnum,0);
	} else {
		//netputuev(SCLASS, CLOSEDONE, pnum,0);			/* Success ! */
		netputevent(SCLASS, CLOSEDONE, pnum,0);			/* Success ! */
	}

	MyPBreturn( pbp);
}

/**************************************************************************/
static void CloseCompleteInit(TCPiopb *pbp)
{
	long		saveA5;
	MyTCPpbPtr	myptr;
	
	myptr = (MyTCPpbPtr)pbp;

#ifndef _powerpc_
	saveA5 = SetA5(myptr->SavedA5);
#endif
	
	CloseComplete(myptr);
	
#ifndef _powerpc_
	SetA5(saveA5);
#endif
}
 
short netportencryptstate (short port, Ptr edata)
{
	StreamRPtr	p;
 
	if (port < 0 || port >= NPORTS)
 		return(-2);
 
 	if ((p = streams[port]) == NULL)
 		return(-2);
 
	p->aedata = edata;
 
 	return 0;
}
void networkUPPinit(void)
{
	OpenCompleteInitUPP  = 	NewTCPIOCompletionProc(OpenCompleteInit);
	CloseCompleteInitUPP = 	NewTCPIOCompletionProc(CloseCompleteInit);
	SendCompleteInitUPP  = 	NewTCPIOCompletionProc(SendCompleteInit);
	NotifyUPP        	 =  NewTCPNotifyProc(Notify);
}

// This takes care of most async cases of port rotation when we need to assign
// our own range of port numbers for a privileged port for rlogin
// (RAB BetterTelnet 2.0b2)
short checkPortRotation(short port)
{
	short err;
	MyTCPpbPtr pbp;

	if (port < 0 || port >= NPORTS)
 		return 0;

	pbp = streams[port]->pbp;
	if (!pbp) return 0;

	err = -23017;

	while (err == -23017) {
		pbp->pb.csParam.open.localPort++;
		err = xPBControlAsync(pbp, OpenCompleteInitUPP);
	}

	if (err != noError) {
		DoError(515 | NET_ERRORCLASS, LEVEL2, NULL);
		return 0;
		} else return 1; // yay, we did it
}

short getUrgentFlag(short pnum)
{
	short		i;
	StreamRPtr	p;

	if (pnum < 0 || pnum >= NPORTS)			/* BYU 2.4.15 */
		return 0;

	if (NULL == (p = streams[pnum]))
		return 0;

	return (p->urgentMode);
}

/*
*   Util.c
*   utility library for use with the Network kernel
*
*   version 2, full session layer, TK started 6/17/87 */

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5


#include "InternalEvents.h"
#include "wind.h"
#include "bkgr.proto.h"
#include "netevent.proto.h"
#include "mydnr.proto.h"
#include "Connections.proto.h"
#include "AddressXlation.h"
extern long MyA5;
extern WindRec *screens;

#ifdef __MWERKS__
#pragma profile off
#endif

typedef	struct {
	short
		screen;
	long
		MyA5;
	OSErr
		theError;
	Str63
		hostname;
	struct hostInfo
		*hinfo;
}	DNRDelayStruct;

// for async name resolution
pascal void DNRDone(struct hostInfo *hostInfoPtr, DNRDelayStruct *info);
pascal void DNRDoneInit(struct hostInfo *hostInfoPtr, DNRDelayStruct *info);
PROTO_UPP(DNRDoneInit, Result);

// for reverse name lookup
pascal void DNRDone2(struct hostInfo *hostInfoPtr, DNRDelayStruct *info);
pascal void DNRDone2Init(struct hostInfo *hostInfoPtr, DNRDelayStruct *info);
PROTO_UPP(DNRDone2Init, Result);

/**********************************************************************
 * DotToNum - turn an address in dotted decimal into an internet address
 * returns True if the conversion was successful.  This routine is
 * somewhat limited, in that it will accept only four octets, and does
 * not permit the abbreviated forms for class A and B networks. <- Steve Dorner
 **********************************************************************/
Boolean DotToNum(BytePtr string,ip_addr *nPtr)
{
  unsigned long	address=0;
  short			b=0;
  BytePtr 		cp;
  short			dotcount=0;
  
  /*
   * allow leading spaces
   */
  for (cp=string+1;cp<=string+*string;cp++) if (*cp!=' ') break;
  
  /*
   * the address
   */
  for (;cp<=string+*string;cp++)
  {
    if (*cp=='.')
    {
      if (++dotcount > 3) return (FALSE); /* only 4 octets allowed */
      address <<= 8;
      address |= b;
      b=0;
    }
    else if (isdigit(*cp))
    {
      b *= 10;
      b += (*cp - '0');
      if (b>255) return (FALSE);          /* keep it under 256 */
    }
    else if (*cp==' ')                    /* allow trailing spaces */
      break;
    else
      return (FALSE);                     /* periods or digits ONLY */
  }
  
  /*
   * final checks, assignment
   */
  if (dotcount!=3) return (FALSE);
  address <<= 8;
  address |= b;
  *nPtr = (ip_addr) address;
  return(TRUE);
}

/**************************************************************************/
/*	For FTP to look up the transfer options to use when running */
Boolean	TranslateIPtoDNSname(ip_addr ipnum, StringPtr machineName)
{
	UNUSED_ARG(ipnum)
	UNUSED_ARG(machineName)
	// NEED TO IMPLEMENT THIS BUGG
	return(FALSE);

}

/*********************************************************************/
pascal void DNRDone(struct hostInfo *hostInfoPtr, DNRDelayStruct *info)
{
	UNUSED_ARG(hostInfoPtr)
	WindRec *theScreen = &screens[info->screen];

	if ( theScreen->active == CNXN_DNRWAIT ) {
		netputevent(USERCLASS,DOMAIN_DONE, info->screen, (long)info);
	} else {
		DisposePtr((Ptr)info->hinfo);
		DisposePtr((Ptr)info);
	}
}

SIMPLE_UPP(DNRDoneInit, Result);
pascal void DNRDoneInit(struct hostInfo *hostInfoPtr, DNRDelayStruct *info)
{
	long saveA5;
	DNRDelayStruct *mptr;
	struct hostInfo *hptr;
	
	mptr = info;
	hptr = hostInfoPtr;

#ifndef	__powerpc__
	saveA5 = SetA5(mptr->MyA5);
#endif

	DNRDone(hptr, mptr);
	
#ifndef	__powerpc__
	SetA5(saveA5);
#endif
}

// The following code for reverse DNS lookups was contributed by Dan Hyde. Thanks!
// (added in BetterTelnet 1.0fc9 - RAB)
/*********************************************************************/
pascal void DNRDone2(struct hostInfo *hostInfoPtr, DNRDelayStruct *info)
{
	UNUSED_ARG(hostInfoPtr)
	WindRec *theScreen = &screens[info->screen];

	if ( theScreen->active == CNXN_DNRWAIT ) {
		netputevent(USERCLASS,DOMAIN_DONE2, info->screen, (long)info);
	} else {
		DisposePtr((Ptr)info->hinfo);
		DisposePtr((Ptr)info);
	}
}

SIMPLE_UPP(DNRDone2Init, Result);
pascal void DNRDone2Init(struct hostInfo *hostInfoPtr, DNRDelayStruct *info)
{
	long saveA5;
	DNRDelayStruct *mptr;
	struct hostInfo *hptr;
	
	mptr = info;
	hptr = hostInfoPtr;

#ifndef	__powerpc__
	saveA5 = SetA5(mptr->MyA5);
#endif

	DNRDone2(hptr, mptr);
	
#ifndef	__powerpc__
	SetA5(saveA5);
#endif
}

/*********************************************************************/
//
//     On request, I'm modifying this to do reverse name lookup, too.
// The idea is that we should convert the hostname passed to an IP
// address (or series of them), and then convert the IP address to a
// name.  This gives us the particular machine name of a multihomed
// hostname, e.g., login.itd.umich.edu, and allows other parts of
// this code build the correct <hostname>.rcmd string for kerberos use.
//     Dan Hyde, October 1996 
//

OSErr	DoTheDNR(StringPtr hostname, short window)
{
	DNRDelayStruct		*Info;
	struct hostInfo		*HInfo;
	ip_addr				ip;
	
	Info = (DNRDelayStruct *)myNewPtr(sizeof(DNRDelayStruct));
	HInfo = (struct hostInfo *)myNewPtr(sizeof(struct hostInfo));
	
	if ((Info == NULL) || (HInfo == NULL)) return(memFullErr);
	
	Info->screen = window;
#ifndef __powerpc__
	Info->MyA5 = MyA5;
#endif
	Info->hinfo = HInfo;
		
	BlockMoveData(hostname, Info->hostname, StrLength(hostname)+1);
	
	if (DotToNum(hostname, &ip)) {
		Info->hinfo->rtnCode = noErr;
		Info->hinfo->addr[0] = ip;
		DNRDone(HInfo, Info);
		return(noErr);
	}

	PtoCstr(Info->hostname);
	Info->theError = StrToAddr((char *)Info->hostname, HInfo,
								DNRDoneInitUPP, (Ptr)Info);
	
	if ((Info->theError != cacheFault) && (Info->theError != inProgress))
		DNRDone(HInfo, Info);
	
	return(noErr);
}

//	The event queue routines send us the screen number that DNRDone sent.  We demangle
//	this mess of data and call CompleteConnectionOpening to do all of the port and screen
//	stuff that we shouldn't know about at this level.  We are merely a non-interrupt level
//	flow control point. (i.e. I would do this from DNRDone, but that's interrupt time)

// RAB BetterTelnet 1.0fc9 - added glue to Dan Hyde's reverse DNS code
// Note that, unlike in Dan Hyde's version, reverse DNS is only done here
// when Kerberos authentication is turned on.

void	HandleDomainDoneMessage(short screen, long data2)
{
	DNRDelayStruct	*Info = (DNRDelayStruct *)data2;
	ip_addr			the_IP;
	struct hostInfo	*HInfo;
	OSErr			theErr;		// The error, if any

	HInfo = Info->hinfo;
	the_IP = Info->hinfo->addr[0];
	theErr = Info->hinfo->rtnCode;

	if (!screens[screen].authenticate || screens[screen].sockslookup) {
		CompleteConnectionOpening(screen, the_IP, theErr, Info->hinfo->cname);

		//	We also dispose of the DNR memory allocations

		DisposePtr((Ptr)Info->hinfo);
		DisposePtr((Ptr)Info);
	} else {

		// Did things fail?
		if (theErr || (!the_IP)) {

		// pass the buck
			HandleDomainDone2Message(screen, data2);

		} else {

			/* Do reverse name lookup */
			Info->theError = AddrToName(the_IP, HInfo, DNRDone2InitUPP, (Ptr)Info);
			if ((Info->theError != cacheFault) && (Info->theError != inProgress)) {
				DNRDone2(HInfo, Info);
			}
		}
	}

}

void	HandleDomainDone2Message(short screen, long data2)
{
	DNRDelayStruct	*Info = (DNRDelayStruct *)data2;
	ip_addr			the_IP;
	OSErr			theErr;		// The error, if any
	
	the_IP = Info->hinfo->addr[0];
	theErr = Info->hinfo->rtnCode;
	
	CompleteConnectionOpening(screen, the_IP, theErr, Info->hinfo->cname);

	//	We also dispose of the DNR memory allocations
		
	DisposePtr((Ptr)Info->hinfo);
	DisposePtr((Ptr)Info);
}

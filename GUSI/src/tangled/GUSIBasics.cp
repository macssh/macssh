// <GUSIBasics.cp>=                                                        
#define GUSI_MESSAGE_LEVEL 5

#include "GUSIInternal.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>

#include <MacTypes.h>
#include <Events.h>
#include <Files.h>
#include <OSUtils.h>
#include <EPPC.h>
#include <LowMem.h>
#include <AppleEvents.h>
#include <QuickDraw.h>
#include <MacTCP.h>
#include <OpenTransport.h>

// \section{Implementation of hook handling}                               
//                                                                         
// Each hook is represented by a hook variable and a piece of code in the set and
// get routine.                                                            
//                                                                         
// <Implementation of hook handling>=                                      
__BEGIN_DECLS
// Every hook then has to add to the three above code sections. Here is the spin hook.
//                                                                         
// <Hook variables>=                                                       
GUSISpinFn gGUSISpinHook;
// Here is the exec hook. I guess you see the pattern now                  
//                                                                         
// <Hook variables>=                                                       
GUSIExecFn gGUSIExecHook;
// The event hook is somewhat different as it is a whole array of hooks.   
//                                                                         
// <Hook variables>=                                                       
const short kGUSIDefaultEventMask = mDownMask+highLevelEventMask;
short		gGUSIEventMask		  = kGUSIDefaultEventMask;
GUSIEventFn	gGUSIEventHook[16];
__END_DECLS
void GUSISetHook(OSType code, GUSIHook hook)
{
	switch (code) {
	// <Hook setter code>=                                                     
 case GUSI_SpinHook:
 	gGUSISpinHook = (GUSISpinFn) hook;
 	break;
 // <Hook setter code>=                                                     
 case GUSI_ExecHook:
 	gGUSIExecHook = (GUSIExecFn) hook;
 	break;
 // <Hook setter code>=                                                     
 case GUSI_EventHook + nullEvent:
 case GUSI_EventHook + mouseDown:
 case GUSI_EventHook + mouseUp:
 case GUSI_EventHook + keyDown:
 case GUSI_EventHook + keyUp:
 case GUSI_EventHook + autoKey:
 case GUSI_EventHook + updateEvt:
 case GUSI_EventHook + diskEvt:
 case GUSI_EventHook + activateEvt:
 case GUSI_EventHook + osEvt:
 	code -= GUSI_EventHook;
 	if (hook == (GUSIHook) -1) {
 		gGUSIEventHook[code] = (GUSIEventFn) 0;
 		gGUSIEventMask &= ~(1 << code);
 	} else {
 		gGUSIEventHook[code] = (GUSIEventFn) hook;
 		if (hook || kGUSIDefaultEventMask & (1 << code))
 			gGUSIEventMask |= 1 << code;
 		else
 			gGUSIEventMask &= ~(1 << code);
 	}
 	break;
 case GUSI_EventHook + kHighLevelEvent:
 	if (hook != (GUSIHook) -1)
 		gGUSIEventMask |= highLevelEventMask;
 	else
 		gGUSIEventMask &= ~highLevelEventMask;
 	break;
	default:
		GUSI_ASSERT_CLIENT(0, 
			(	"Illegal code %lx ('%c%c%c%c') passed to GUSISetHook\n",
				code,
				(code >> 24) & 0xff,
				(code >> 16) & 0xff,
				(code >> 8)  & 0xff,
				(code) 		 & 0xff
			));
		break;		
	}
}
GUSIHook GUSIGetHook(OSType code)
{
	switch (code) {
	// <Hook getter code>=                                                     
 case GUSI_SpinHook:
 	return (GUSIHook) gGUSISpinHook;
 // <Hook getter code>=                                                     
 case GUSI_ExecHook:
 	return (GUSIHook) gGUSIExecHook;
 // <Hook getter code>=                                                     
 case GUSI_EventHook + nullEvent:
 case GUSI_EventHook + mouseDown:
 case GUSI_EventHook + mouseUp:
 case GUSI_EventHook + keyDown:
 case GUSI_EventHook + keyUp:
 case GUSI_EventHook + autoKey:
 case GUSI_EventHook + updateEvt:
 case GUSI_EventHook + diskEvt:
 case GUSI_EventHook + activateEvt:
 case GUSI_EventHook + osEvt:
 	return (GUSIHook) gGUSIEventHook[code - GUSI_EventHook];
 case GUSI_EventHook + kHighLevelEvent:
 	return (GUSIHook) ((gGUSIEventMask & highLevelEventMask) ? 0 : -1);
	default:
		GUSI_ASSERT_CLIENT(0, 
			(	"Illegal code %lx ('%c%c%c%c') passed to GUSIGetHook\n",
				code,
				(code >> 24) & 0xff,
				(code >> 16) & 0xff,
				(code >> 8)  & 0xff,
				(code) 		 & 0xff
			));
		return (GUSIHook) nil;
	}
}
// \section{Implementation of error handling}                              
//                                                                         
//                                                                         
// <Implementation of error handling>=                                     
int GUSISetPosixError(int error)
{
	if (error > 0) {
		errno = error;
		
		return -1;
	} else if (error < 0) {
		errno = EINVAL;
		
		return -1;
	} else
		return 0;
}
// <Implementation of error handling>=                                     
inline int GUSIErrorMapping(OSErr error, int posixError)
{
	GUSI_MESSAGE(("Error MacOS %d -> POSIX %d\n", error, posixError));
	return posixError;
}

int GUSIMapMacError(OSErr error)
{
	switch (error) {
	case noErr:
		return 0;
	// The errors we handle originate mainly from four major sources. The first
 // of them is the File Manager.                                            
 //                                                                         
 // <Map File Manager errors to POSIX errors>=                              
 case opWrErr:
 case wrPermErr:
 	return GUSIErrorMapping(error, EPERM);
 case bdNamErr:
 	return GUSIErrorMapping(error, ENAMETOOLONG);
 case afpObjectTypeErr:
 	return GUSIErrorMapping(error, ENOTDIR);
 case fnfErr:
 case nsvErr:
 case dirNFErr:
 	return GUSIErrorMapping(error, ENOENT);
 case dupFNErr:
 	return GUSIErrorMapping(error, EEXIST);
 case dirFulErr:
 case dskFulErr:
 	return GUSIErrorMapping(error, ENOSPC);
 case fBsyErr:
 	return GUSIErrorMapping(error, EBUSY);
 case tmfoErr:
 	return GUSIErrorMapping(error, ENFILE);
 case fLckdErr:
 case permErr:
 case afpAccessDenied:
 case kOTAccessErr:			/* -3152 Missing access permission					*/
 	return GUSIErrorMapping(error, EACCES);
 case wPrErr:
 case vLckdErr:
 	return GUSIErrorMapping(error, EROFS);
 case badMovErr:
 	return GUSIErrorMapping(error, EINVAL);
 case diffVolErr:
 	return GUSIErrorMapping(error, EXDEV);
	// MacTCP generates another group of errors.                               
 //                                                                         
 // <Map MacTCP errors to POSIX errors>=                                    
 case openFailed:
 	return GUSIErrorMapping(error, ECONNREFUSED);
 case duplicateSocket:
 	return GUSIErrorMapping(error, EADDRINUSE);
 case connectionTerminated:
 	return GUSIErrorMapping(error, ECONNREFUSED);
 case commandTimeout:
 	return GUSIErrorMapping(error, ETIMEDOUT);		
 case ipBadLapErr:
 case ipBadCnfgErr:
 case ipNoCnfgErr:
 case ipLoadErr:
 	return GUSIErrorMapping(error, ENETDOWN);
	// Open Transport both generates its own version of many                   
 // POSIX error codes and a number of ``native'' error codes.               
 //                                                                         
 // <Map Open Transport errors to POSIX errors>=                            
 case kOTLookErr:			/* -3158 An event occurred - call Look() 			*/
 	return GUSIErrorMapping(error, ELOOK);
 case kOTAddressBusyErr:		/* -3172 Address requested is already in use		*/
 	return GUSIErrorMapping(error, EADDRINUSE);
 case kOTNoAddressErr:		/* -3154 No address was specified					*/
 case kOTOutStateErr:		/* -3155 Call issued in wrong state					*/
 	return GUSIErrorMapping(error, ENOTCONN);
 case kOTFlowErr:			/* -3161 Provider is flow-controlled				*/
 	return GUSIErrorMapping(error, EWOULDBLOCK);
 case kOTNotSupportedErr:	/* -3167 Command is not supported					*/
 	return GUSIErrorMapping(error, EOPNOTSUPP);
 case kOTCanceledErr:		/* -3180 The command was cancelled					*/
 	return GUSIErrorMapping(error, ECANCELED);
 case kEAGAINErr:		/*  Seems to be returned for refused connections	*/
 	return GUSIErrorMapping(error, ECONNREFUSED);
// A little macro provides a great service in translating the POSIXish OpenTransport errors.
//                                                                         
// <Map Open Transport POSIXish errors to POSIX errors>=                   
#define MAP_OT_POSIX_ERROR(err)	case k##err##Err: return err

MAP_OT_POSIX_ERROR(EPERM);			/*  Permission denied					*/
MAP_OT_POSIX_ERROR(ENOENT);			/*  No such file or directory			*/
MAP_OT_POSIX_ERROR(EINTR);			/*  Interrupted system service			*/
MAP_OT_POSIX_ERROR(EIO);			/*  I/O error							*/
MAP_OT_POSIX_ERROR(ENXIO);			/*  No such device or address			*/
MAP_OT_POSIX_ERROR(EBADF);			/*  Bad file number						*/
MAP_OT_POSIX_ERROR(ENOMEM);			/*  Not enough space					*/
MAP_OT_POSIX_ERROR(EACCES);			/*  Permission denied					*/
MAP_OT_POSIX_ERROR(EFAULT);			/*  Bad address							*/
MAP_OT_POSIX_ERROR(EBUSY);			/*  Device or resource busy				*/
MAP_OT_POSIX_ERROR(EEXIST);			/*  File exists							*/
MAP_OT_POSIX_ERROR(ENODEV);			/*  No such device						*/
MAP_OT_POSIX_ERROR(EINVAL);			/*  Invalid argument					*/
MAP_OT_POSIX_ERROR(ENOTTY);			/*  Not a character device				*/
MAP_OT_POSIX_ERROR(EPIPE);			/*  Broken pipe							*/
MAP_OT_POSIX_ERROR(ERANGE);			/*  Message size too large for STREAM	*/
MAP_OT_POSIX_ERROR(EWOULDBLOCK);	/* 										*/
MAP_OT_POSIX_ERROR(EALREADY);		/* 										*/
MAP_OT_POSIX_ERROR(ENOTSOCK);		/*  Socket operation on non-socket		*/
MAP_OT_POSIX_ERROR(EDESTADDRREQ);	/*  Destination address required		*/
MAP_OT_POSIX_ERROR(EMSGSIZE);		/*  Message too long					*/
MAP_OT_POSIX_ERROR(EPROTOTYPE);		/*  Protocol wrong type for socket		*/
MAP_OT_POSIX_ERROR(ENOPROTOOPT);	/*  Protocol not available				*/
MAP_OT_POSIX_ERROR(EPROTONOSUPPORT);/*  Protocol not supported				*/
MAP_OT_POSIX_ERROR(ESOCKTNOSUPPORT);/*  Socket type not supported			*/
MAP_OT_POSIX_ERROR(EOPNOTSUPP);		/*  Operation not supported on socket	*/
MAP_OT_POSIX_ERROR(EADDRINUSE);		/*  Address already in use				*/
MAP_OT_POSIX_ERROR(EADDRNOTAVAIL);	/*  Can't assign requested address		*/
MAP_OT_POSIX_ERROR(ENETDOWN);		/*  Network is down						*/
MAP_OT_POSIX_ERROR(ENETUNREACH);	/*  Network is unreachable				*/
MAP_OT_POSIX_ERROR(ENETRESET);		/*  Network dropped connection on reset	*/
MAP_OT_POSIX_ERROR(ECONNABORTED);	/*  Software caused connection abort	*/
MAP_OT_POSIX_ERROR(ECONNRESET);		/*  Connection reset by peer			*/
MAP_OT_POSIX_ERROR(ENOBUFS);		/*  No buffer space available			*/
MAP_OT_POSIX_ERROR(EISCONN);		/*  Socket is already connected			*/
MAP_OT_POSIX_ERROR(ENOTCONN);		/*  Socket is not connected				*/
MAP_OT_POSIX_ERROR(ESHUTDOWN);		/*  Can't send after socket shutdown	*/
MAP_OT_POSIX_ERROR(ETOOMANYREFS);	/*  Too many references: can't splice	*/
MAP_OT_POSIX_ERROR(ETIMEDOUT);		/*  Connection timed out				*/
MAP_OT_POSIX_ERROR(ECONNREFUSED);	/*  Connection refused					*/
MAP_OT_POSIX_ERROR(EHOSTDOWN);		/*  Host is down						*/
MAP_OT_POSIX_ERROR(EHOSTUNREACH);	/*  No route to host					*/
MAP_OT_POSIX_ERROR(EINPROGRESS);	/* 										*/
MAP_OT_POSIX_ERROR(ESRCH);			/* 										*/
	// Finally, the PPC Toolbox generates some errors.                         
 //                                                                         
 // <Map PPC Toolbox errors to POSIX errors>=                               
 case destPortErr:      		/* -906 Port does not exist at destination*/		
 	return GUSIErrorMapping(error, EADDRNOTAVAIL);
 case portNameExistsErr: 	/* -910 port is already open (perhaps in another app)*/
 	return GUSIErrorMapping(error, EISCONN);
 case userRejectErr: 		/* -912 Destination rejected the session request*/
 	return GUSIErrorMapping(error, ECONNREFUSED);
 case noResponseErr: 		/* -916 unable to contact destination*/
 	return GUSIErrorMapping(error, ENETUNREACH);
	case memFullErr:
		return GUSIErrorMapping(error, ENOMEM);
	default:
		return GUSIErrorMapping(error, EINVAL);
	}
}

int GUSISetMacError(OSErr error)
{
	return GUSISetPosixError(GUSIMapMacError(error));
}
// [[h_errno]] is defined here.                                            
//                                                                         
// <Implementation of error handling>=                                     
int h_errno;

int GUSISetHostError(int error)
{
	if (error) {
		h_errno = error;
		
		return -1;
	} else
		return 0;
}

int GUSISetMacHostError(OSErr error)
{
	switch (error) {
	case noErr: 
		return 0;
	case nameSyntaxErr:
	case noNameServer:
	case authNameErr:
		h_errno = HOST_NOT_FOUND;	
		break;
	case noResultProc:
	case dnrErr:
	default:
		h_errno = NO_RECOVERY;
		break;
	case noAnsErr:
	case outOfMemory:
		h_errno = TRY_AGAIN;
		break;
	}
	return -1;
}
// \section{Implementation of event handling}                              
//                                                                         
//                                                                         
// <Implementation of event handling>=                                     
void GUSIHandleNextEvent(long sleepTime)
{
	EventRecord event;
	
	if (WaitNextEvent(gGUSIEventMask|1, &event, sleepTime, nil))
		switch (event.what) {
#if !TARGET_API_MAC_CARBON
		case mouseDown:
			if (!gGUSIEventHook[mouseDown]) {
				WindowPtr win;
				if (FindWindow(event.where, &win) == inSysWindow)
					SystemClick(&event, win);
				return;
			}
			break;
#endif
		case kHighLevelEvent:
			AEProcessAppleEvent(&event);	// Ignore errors
				
			return;
		}

	if (gGUSIEventHook[event.what])
		gGUSIEventHook[event.what](&event);
}
// \section{Implementation of string formatting}                           
//                                                                         
//                                                                         
// <Implementation of string formatting>=                                  
int GUSI_vsprintf(char * s, const char * format, va_list args)
{
	return vsprintf(s, format, args);
}

int GUSI_sprintf(char * s, const char * format, ...)
{
	va_list	ap;
		
	va_start(ap, format);
	int len = GUSI_vsprintf(s, format, ap);
	va_end(ap);
	
	return len;
}

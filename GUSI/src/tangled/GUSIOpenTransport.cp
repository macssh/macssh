// <GUSIOpenTransport.cp>=                                                 
#define GUSI_MESSAGE_LEVEL 1

#include "GUSIInternal.h"
#include "GUSIOpenTransport.h"
#include "GUSIDiag.h"
#include "GUSITimer.h"

#include <stdlib.h>
#include <errno.h>

#include <algorithm>
#include <memory>

GUSI_USING_STD_NAMESPACE

// <Asynchronous notifier function for Open Transport>=                    
inline uint32_t CompleteMask(OTEventCode code)	
{ 	
	return 1 << (code & 0x1F); 
}

pascal void GUSIOTNotify(
	GUSIOTSocket * sock, OTEventCode code, OTResult result, void *cookie)
{
	GUSI_MESSAGE1(("GUSIOTNotify %08x %d\n", code, result));
	switch (code & 0x7f000000L) {
	case 0:
		sock->fNewEvent |= code;
		result = 0;
		break;
	case kCOMPLETEEVENT:
		if (!(code & 0x00FFFFE0))
			sock->fNewCompletion |= CompleteMask(code);
		if (code == T_OPENCOMPLETE)
			sock->fEndpoint = static_cast<EndpointRef>(cookie);
		break;
	default:
		if (code != kOTProviderWillClose
		 && code != kOTProviderIsClosed)
			result = 0;
		else if (!result) {
			result = kENETDOWNErr;
		}
	}
	sock->SetAsyncMacError(result);
	sock->Wakeup();
}
// <Member functions for class [[GUSIOTFactory]]>=                         
bool GUSIOTFactory::sOK = false;

bool GUSIOTFactory::Initialize()
{
	if (!sOK)
		sOK = !InitOpenTransport();
	return sOK;
}
// <Member functions for class [[GUSIOTStreamFactory]]>=                   
GUSISocket * GUSIOTStreamFactory::socket(int domain, int type, int protocol)
{
	GUSIOTStrategy * strategy = Strategy(domain, type, protocol);
	if (Initialize() && strategy)
		return new GUSIOTStreamSocket(strategy);
	else
		return static_cast<GUSISocket *>(0);
}
// <Member functions for class [[GUSIOTDatagramFactory]]>=                 
GUSISocket * GUSIOTDatagramFactory::socket(int domain, int type, int protocol)
{
	GUSIOTStrategy * strategy = Strategy(domain, type, protocol);
	if (Initialize() && strategy)
		return new GUSIOTDatagramSocket(strategy);
	else
		return static_cast<GUSISocket *>(0);
}
// <Member functions for class [[GUSIOTStrategy]]>=                        
OTConfiguration * GUSIOTStrategy::CreateConfiguration()
{
	if (!fConfig)
		fConfig = OTCreateConfiguration(ConfigPath());
	
	return OTCloneConfiguration(fConfig);
}

GUSIOTStrategy::~GUSIOTStrategy()
{
	if (fConfig)
		OTDestroyConfiguration(fConfig);
}
// While you might have to override this for sophisticated addresses, this should do for most.
//                                                                         
// <Member functions for class [[GUSIOTStrategy]]>=                        
int GUSIOTStrategy::CopyAddress(const TNetbuf & from, TNetbuf & to)
{
	memcpy(to.buf, from.buf, to.len = from.len);
	
	return 0;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
void GUSIOTSocket::MopupEvents()
{
	{
		Lock	lock(fEndpoint);
	 	
		fEvent 		   |= (fCurEvent	  = fNewEvent);
		fCompletion	   |= (fCurCompletion = fNewCompletion);
		fNewEvent		=	0;
		fNewCompletion	= 	0;
	}
	
	if (fCurEvent & T_UDERR) {
		GUSIOTTUDErr *	udErr = new (fEndpoint) GUSIOTTUDErr;
		
		if (!OTRcvUDErr(fEndpoint, udErr) && udErr)
			SetAsyncMacError(udErr->error);
		
		delete udErr;
	}
	if ((fCurEvent & (T_DISCONNECT | T_ORDREL)) && !(fEvent & T_LISTEN)) {
		fReadShutdown = true;
	}
}
// <Member functions for class [[GUSIOTSocket]]>=                          
GUSIOTSocket::GUSIOTSocket(GUSIOTStrategy * strategy)
{
	// <Initialize fields of [[GUSIOTSocket]]>=                                
 fNewEvent 		= 0;
 fEvent 			= 0;
 fNewCompletion	= 0;
 fCompletion		= 0;
 // <Initialize fields of [[GUSIOTSocket]]>=                                
 fStrategy		= strategy;
 fEndpoint		= nil;
 fLinger.l_onoff = false;
 fLinger.l_linger= 0;
 // <Initialize fields of [[GUSIOTSocket]]>=                                
 fSockName	= nil;
	SetAsyncMacError(
		OTAsyncOpenEndpoint(
			fStrategy->CreateConfiguration(), 
			0, fStrategy->EndpointInfo(), 
			reinterpret_cast<OTNotifyProcPtr>(GUSIOTNotify), 
			this)); 
	AddContext();
	MopupEvents();
	while (!fAsyncError && !(fCompletion & CompleteMask(T_OPENCOMPLETE))) {
		GUSIContext::Yield(kGUSIBlock);
		MopupEvents();
	}
	RemoveContext();
	if (!fEndpoint)
		GUSISetPosixError(GetAsyncError());
}
// <Member functions for class [[GUSIOTSocket]]>=                          
void GUSIOTSocket::close()
{	
	if (OTGetEndpointState(fEndpoint) != T_IDLE) {
		// Under desperate circumstances, we are prepared to employ quite a bit of 
  // violence to disconnect the socket.                                      
  //                                                                         
  // <Disconnect the [[GUSIOTSocket]], dammit>=                              
  fCompletion 	   &= ~(CompleteMask(T_DISCONNECTCOMPLETE));
  GUSIOTTCall * call 	= new (fEndpoint, 0) GUSIOTTCall;
  fAsyncError 		= 0;
  SetAsyncMacError(OTSndDisconnect(fEndpoint, call));
  delete call;
  AddContext();
  MopupEvents();
  while (!fAsyncError && !(fCompletion & CompleteMask(T_DISCONNECTCOMPLETE))) {
  	GUSIContext::Yield(kGUSIBlock);
  	MopupEvents();
  }
  RemoveContext();
  fAsyncError = 0;
	}
	
	Unbind();
	OTCloseProvider(fEndpoint);
	fEvent = 0;
	
	GUSISocket::close();
}

GUSIOTSocket::~GUSIOTSocket()
{
	delete fSockName;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int	GUSIOTSocket::bind(void * name, socklen_t namelen)
{	
	if (fSockName)
		return GUSISetPosixError(EINVAL);

	int 			res 	= -1;
	GUSIOTTBind * 	inName 	= new (fEndpoint) GUSIOTTBind;
	if (!inName)
		return GUSISetPosixError(ENOMEM);
	if (inName->Pack(fStrategy, name, namelen))
		goto freeInName;
	inName->qlen = 0;
	
	res = BindToAddress(inName);
freeInName:	
	delete inName;
	
	return res;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::BindToAddress(GUSIOTTBind * addr)
{
	fSockName = new (fEndpoint) GUSIOTTBind;
	if (!fSockName)
		return GUSISetPosixError(ENOMEM);
	fCompletion	   &= ~CompleteMask(T_BINDCOMPLETE);
	fAsyncError		= 0;
	SetAsyncMacError(OTBind(fEndpoint, addr, fSockName));
	AddContext();
	MopupEvents();
	while (!fAsyncError && !(fCompletion & CompleteMask(T_BINDCOMPLETE))) {
		GUSIContext::Yield(kGUSIBlock);
		MopupEvents();
	}
	RemoveContext();
	if (GUSISetPosixError(GetAsyncError())) {
		delete fSockName;
		fSockName = nil;
		
		return -1;
	} else
		return 0;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
void GUSIOTSocket::Unbind()
{
	fCompletion &= ~(CompleteMask(T_BINDCOMPLETE) | CompleteMask(T_UNBINDCOMPLETE));
	fAsyncError = 0;
	SetAsyncMacError(OTUnbind(fEndpoint));
	AddContext();
	MopupEvents();
	while (!fAsyncError && !(fCompletion & CompleteMask(T_UNBINDCOMPLETE))) {
		GUSIContext::Yield(kGUSIBlock);
		MopupEvents();
	}
	RemoveContext();
	fAsyncError = 0;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::getsockname(void * name, socklen_t * namelen)
{
	int res;
	
	if (!fSockName)
		BindToAddress(nil);

	if (fStrategy->EndpointInfo()->flags & T_XPG4_1) {
		GUSIOTTBind * 	otname	= new (fEndpoint) GUSIOTTBind;
		if (!otname)
			return GUSISetPosixError(ENOMEM);
		
		fCompletion &= ~CompleteMask(T_GETPROTADDRCOMPLETE);
		fAsyncError	 = 0;
		SetAsyncMacError(OTGetProtAddress(fEndpoint, otname, nil));

		AddContext();
		MopupEvents();
		while (!fAsyncError && !(fCompletion & CompleteMask(T_GETPROTADDRCOMPLETE))) {
			GUSIContext::Yield(kGUSIBlock);
			MopupEvents();
		}
		RemoveContext();
		
		res = fAsyncError 
				? GUSISetPosixError(GetAsyncError()) 
				: otname->Unpack(fStrategy, name, namelen);
		
		delete otname;
	} else
		res = fSockName->Unpack(fStrategy, name, namelen);
		
	return res;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::shutdown(int how)
{
	if (!GUSI_SASSERT_CLIENT(how >= 0 && how < 3, "shutdown: 0,1, or 2\n"))
		return GUSISetPosixError(EINVAL);
	
	switch (how) {
	case SHUT_RD:
	case SHUT_RDWR:
		Unbind();
	}
	
	GUSISMState::Shutdown(how);
	
	return 0;
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::fcntl(int cmd, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoFcntl(&result, cmd, arg))
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("fcntl: illegal request %d\n", cmd));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::ioctl(unsigned int request, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoIoctl(&result, request, arg))
		return result;
	if (request == FIONREAD) {
		size_t	res;
		if (OTCountDataBytes(fEndpoint, &res))
			res = 0;
		*va_arg(arg, size_t *) = res;
		return 0;
	}
			
	GUSI_ASSERT_CLIENT(false, ("ioctl: illegal request %d\n", request));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTSocket]]>=                          
bool GUSIOTSocket::pre_select(bool wantRead, bool wantWrite, bool wantExcept)
{
	size_t	sz;
	
	if (wantRead && OTCountDataBytes(fEndpoint, &sz) == kOTNoDataErr)
		fEvent &= ~(T_DATA|T_EXDATA);
	
	return GUSISocket::pre_select(wantRead, wantWrite, wantExcept);
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::getsockopt(int level, int optname, void *optval, socklen_t * optlen)
{
	int result;
	
	MopupEvents();
	
	if (GUSISMAsyncError::DoGetSockOpt(&result, level, optname, optval, optlen))
		return result;

	TOptMgmt 	optReq;
	UInt8	 	optBuffer[ kOTOptionHeaderSize + sizeof(struct linger) ];
	TOption*	opt 	= (TOption*)optBuffer;
	int			len;
	
	optReq.flags = T_CURRENT;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= XTI_GENERIC;
	opt->name 	= optname;
		
	switch (level) {
	case SOL_SOCKET:
		switch (optname) {
		case SO_KEEPALIVE:
			len = sizeof(struct t_kpalive);
			break;
		case SO_DEBUG:
		case SO_RCVBUF:
		case SO_SNDBUF:
		case SO_RCVLOWAT:
		case SO_SNDLOWAT:
			len = 4;
			break;
		case SO_LINGER:
			memcpy(optval, &fLinger, sizeof(struct linger));
			
			return 0;
		default:
			goto notSupported;
		}
		break;
	case IPPROTO_TCP:
		switch (optname) {
		case TCP_KEEPALIVE:
			opt->name = OPT_KEEPALIVE;
			len = sizeof(struct t_kpalive);
			break;
		default:
			goto notSupported;
		}
		break;
	default:
		goto notSupported;
	}
	optReq.opt.len = opt->len = kOTOptionHeaderSize+len;
	if (GUSISetMacError(OTOptionManagement(fEndpoint, &optReq, &optReq)))
		return -1;
	switch (optname) {
	case TCP_KEEPALIVE:
		if (level == IPPROTO_TCP)
			*(int *)optval = reinterpret_cast<t_kpalive *>(opt->value)->kp_timeout*60;
		else
			*(int *)optval = reinterpret_cast<t_kpalive *>(opt->value)->kp_onoff;
		*optlen = 4;
		break;
	default:
		memcpy(optval, opt->value, len);
		*optlen = len;
	}
	
	return 0;
notSupported:	
	GUSI_ASSERT_CLIENT(false, ("getsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTSocket]]>=                          
int GUSIOTSocket::setsockopt(int level, int optname, void *optval, socklen_t)
{
	MopupEvents();
	
	TOptMgmt 			optReq;
	UInt8	 			optBuffer[ kOTOptionHeaderSize + sizeof(struct linger) ];
	TOption*	 		opt 	= (TOption*)optBuffer;
	t_kpalive			kpal	= {1, 120};
	int					len;
		
	optReq.flags = T_NEGOTIATE;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= INET_IP;
	opt->name 	= optname;
	
	switch (level) {
	case SOL_SOCKET:
		switch (optname) {
		case SO_KEEPALIVE:
			len 			= sizeof(struct t_kpalive);
			kpal.kp_onoff 	= *reinterpret_cast<int *>(optval);
			optval			= &kpal;
			break;
		case SO_DEBUG:
		case SO_RCVBUF:
		case SO_SNDBUF:
		case SO_RCVLOWAT:
		case SO_SNDLOWAT:
			len = 4;
			break;
		case SO_LINGER:
			memcpy(&fLinger, optval, sizeof(struct linger));
			
			return 0;
		default:
			goto notSupported;
		}
		break;
	case IPPROTO_TCP:
		switch (optname) {
		case TCP_KEEPALIVE:
			len 			= sizeof(struct t_kpalive);
			kpal.kp_timeout = (*reinterpret_cast<int *>(optval) + 30) / 60;
			optval			= &kpal;
			break;
		default:
			goto notSupported;
		}
		break;
	default:
		goto notSupported;
	}
	optReq.opt.len = opt->len = kOTOptionHeaderSize+len;
	memcpy(opt->value, optval, len);

	return GUSISetMacError(OTOptionManagement(fEndpoint, &optReq, &optReq));
notSupported:	
	GUSI_ASSERT_CLIENT(false, ("setsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTSocket]]>=                          
bool GUSIOTSocket::Supports(ConfigOption config)
{
	return config == kSocketCalls;
}
// \section{Implementation of [[GUSIOTStreamSocket]]}                      
//                                                                         
// [[GUSIOTStreamSocket]]s have to be connected and send data in a         
// continuous stream.                                                      
//                                                                         
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
GUSIOTStreamSocket::GUSIOTStreamSocket(GUSIOTStrategy * strategy)
	: GUSIOTSocket(strategy)
{
	// <Initialize fields of [[GUSIOTStreamSocket]]>=                          
 fPeerName	= nil;
 // <Initialize fields of [[GUSIOTStreamSocket]]>=                          
 fNextListener	= nil;
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
GUSIOTSocket * GUSIOTStreamSocket::Clone()
{
	return new GUSIOTStreamSocket(fStrategy);
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
void GUSIOTStreamSocket::close()
{
	if (fLinger.l_onoff && !fLinger.l_linger) {
		GUSIOTSocket::close();
	} else {
		OTSndOrderlyDisconnect(fEndpoint);	
	
		if (fLinger.l_onoff) {
			if (fBlocking) {
				// <Linger on [[GUSIOTStreamSocket]]>=                                     
    GUSITimer	lingerer;

    lingerer.Sleep(fLinger.l_linger*1000+1);

    AddContext();
    MopupEvents();
    while (OTGetEndpointState(fEndpoint) > T_IDLE)
    	if (fLinger.l_onoff && lingerer.Expired()) { 
    		break;
    	} else {
    		// If there is still pending data, we'll have to read it before the disconnect
      // succeeds.                                                               
      //                                                                         
      // <Read and discard data sent to [[GUSIOTStreamSocket]]>=                 
      char 	discard[256];
      OTFlags	otflags;
      while (OTRcv(fEndpoint, discard, 256, &otflags) >= 0)
      	;
      MopupEvents();
    		if (GUSIContext::Yield(kGUSIBlock)) 
    			break;
    	} 
    RemoveContext();
    GUSIOTSocket::close();
				
				return;
			} else {
				fDeadline = LMGetTicks()+fLinger.l_linger*60;
			}
		} else {
			fDeadline	=	LMGetTicks()+3600;
		}
		if (!Close(LMGetTicks()))
			GUSIProcess::Instance()->QueueForClose(this);
	}		
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
GUSIOTStreamSocket::~GUSIOTStreamSocket()
{
	delete fPeerName;
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
bool GUSIOTStreamSocket::Close(UInt32 now)
{
	// If there is still pending data, we'll have to read it before the disconnect
 // succeeds.                                                               
 //                                                                         
 // <Read and discard data sent to [[GUSIOTStreamSocket]]>=                 
 char 	discard[256];
 OTFlags	otflags;
 while (OTRcv(fEndpoint, discard, 256, &otflags) >= 0)
 	;
 MopupEvents();
	if (now < fDeadline && OTGetEndpointState(fEndpoint) > T_IDLE) {
		return false;
	} else {
		GUSIOTSocket::close();
		
		return true;
	}
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
void GUSIOTStreamSocket::MopupEvents()
{
	GUSIOTSocket::MopupEvents();

	if (fCurEvent & T_CONNECT) {
		GUSI_MESSAGE(("Connect\n"));
		OTRcvConnect(fEndpoint, fPeerName);
		fEvent |= T_GODATA;
	}
	if (fCurEvent & T_ORDREL) {
		OTRcvOrderlyDisconnect(fEndpoint);
		GUSI_MESSAGE(("Orderly Disconnect\n"));
	}
	if (fCurEvent & T_DISCONNECT) {
		GUSIOTTDiscon *	discon = new (fEndpoint) GUSIOTTDiscon;

		OTRcvDisconnect(fEndpoint, discon);
		if (discon) {
			GUSI_MESSAGE(("Disconnect %d\n", XTI2OSStatus(discon->reason)));
			SetAsyncMacError(XTI2OSStatus(discon->reason));
			
			delete discon;
		} else {
			GUSI_MESSAGE(("Disconnect\n"));
		}
	}
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
int GUSIOTStreamSocket::listen(int queueLength)
{
	Unbind();
	// For some weird reason, BSD multiplies queue lengths with a fudge factor.
 //                                                                         
 // <Fudge [[queueLength]]>=                                                
 if (queueLength < 1)
 	queueLength = 1;
 else if (queueLength > 4)
 	queueLength = 8;
 else
 	queueLength = ((queueLength * 3) >> 1) + 1;
	fSockName->qlen = queueLength;
	fAsyncError 	= 0;
	SetAsyncMacError(OTBind(fEndpoint, fSockName, nil));
	AddContext();
	MopupEvents();
	while (!fAsyncError && !(fCompletion & CompleteMask(T_BINDCOMPLETE))) {
		GUSIContext::Yield(kGUSIBlock);
		MopupEvents();
	}
	RemoveContext();
	
	return GUSISetPosixError(GetAsyncError());
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
int GUSIOTStreamSocket::getpeername(void * name, socklen_t * namelen)
{
	MopupEvents();
	
	if (!fPeerName)
		return GUSISetPosixError(ENOTCONN);
	return fPeerName->Unpack(fStrategy, name, namelen);
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
GUSISocket * GUSIOTStreamSocket::accept(void * address, socklen_t * addrlen)
{
	MopupEvents();
	
	bool	mustListen = false;
	for (;;) {
		if (mustListen || !fNextListener) {
			// <Call [[OTListen]] and queue a candidate>=                              
   GUSIOTTCall * call = new (fEndpoint) GUSIOTTCall;
   if (!call)
   	return GUSISetPosixError(ENOMEM), static_cast<GUSISocket *>(0);
   OTResult  err;
   bool signal = false;
   AddContext();
   for (;;) {
   	err = OTListen(fEndpoint, call);
   	if (!fBlocking || (err != kOTNoDataErr))
   		break;
   	if (signal = GUSIContext::Yield(kGUSIBlock))
   		break;
   } 
   RemoveContext();
// <[[return]] an appropriate error if [[GUSIOTSocket::accept]] failed>=   
if (signal)
	return GUSISetPosixError(EINTR), static_cast<GUSISocket *>(0);
if (err) {
	delete call;
	
	if (err==kOTNoDataErr)
		GUSISetPosixError(EWOULDBLOCK);
	else
		GUSISetMacError(err);
		
	return static_cast<GUSISocket *>(0);
}
   GUSIOTStreamSocket * candidate = dynamic_cast<GUSIOTStreamSocket *>(Clone());
   candidate->fPeerName = call;
   candidate->fNextListener = fNextListener;
   fNextListener = candidate;
   GUSI_MESSAGE(("GUSIOTStreamSocket::accept queued %08x\n", candidate));
			mustListen = false;
		}
		if (fNextListener) {
			// <Call [[OTAccept]] and [[return]] if successful>=                       
   GUSIOTStreamSocket * sock = fNextListener;
   fCompletion &= ~(CompleteMask(T_ACCEPTCOMPLETE));
   fAsyncError  = 0;
   SetAsyncMacError(OTAccept(fEndpoint, sock->fEndpoint, sock->fPeerName));
   AddContext();
   MopupEvents();
   while (!fAsyncError && !(fCompletion & CompleteMask(T_ACCEPTCOMPLETE))) {
   	GUSIContext::Yield(kGUSIBlock);
   	MopupEvents();
   }
   RemoveContext();
   switch (int error = GetAsyncError()) {
   case ELOOK:
   	switch (OTLook(fEndpoint)) {
   	case T_LISTEN:				// Another connection has arrived
   		mustListen = true;	
   		GUSI_MESSAGE(("GUSIOTStreamSocket::accept T_LISTEN\n"));
   		continue;	
   	case T_DISCONNECT:			// Peer disconnected already
   		GUSI_MESSAGE(("GUSIOTStreamSocket::accept T_DISCONNECT\n"));
   		goto deleteCandidate;
   	}
   	break;
   case 0:
   	GUSI_MESSAGE(("GUSIOTStreamSocket::accept accepted %08x\n", sock));
   	fNextListener 		= sock->fNextListener;
   	sock->fNextListener = nil;
   	
   	sock->getpeername(address, addrlen);
   	sock->fSockName = new (fEndpoint) GUSIOTTBind;
   	sock->fEvent |= T_GODATA; /* Ready to write */
   	if (sock->fSockName && !fSockName->Copy(fStrategy, sock->fSockName))
   		return sock;
   	else
   		delete sock;
   	break;
   default:
   deleteCandidate:
   	GUSI_MESSAGE(("GUSIOTStreamSocket::accept async error %d\n", error));
   	fNextListener = sock->fNextListener;
   	
   	delete sock;
   }
		}
	} 
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
int GUSIOTStreamSocket::connect(void * address, socklen_t addrlen)
{
	MopupEvents();
	OTResult res = 0;

	GUSI_MESSAGE(("Connecting...\n"));
	switch (OTGetEndpointState(fEndpoint)) {
	case T_OUTCON:
		if (!fBlocking)
			return GUSISetPosixError(EALREADY);
		break;
	case T_UNBND:
		if (BindToAddress(nil))
			return -1;
		// Fall through
	case T_IDLE:
		fReadShutdown	=	false;
		fWriteShutdown	= 	false;
		fPeerName 		= new (fEndpoint) GUSIOTTCall;
		if (!fPeerName)
			return GUSISetPosixError(GetAsyncError());
		if (fPeerName->Pack(fStrategy, address, addrlen, true))
			goto freePeerName;
		fEvent &= ~T_CONNECT;
		res = OTConnect(fEndpoint, fPeerName, nil);
		MopupEvents();
		break;
	default:
		return GUSISetPosixError(EISCONN);
	}
	if (!fBlocking && !(fEvent & (T_CONNECT|T_DISCONNECT)))
		return GUSISetPosixError(EINPROGRESS);
	{
		bool signal = false;
		AddContext();
		MopupEvents();
		while (!signal && !(fEvent & (T_CONNECT|T_DISCONNECT))) {
			signal = GUSIContext::Yield(kGUSIBlock);
			MopupEvents();
		}
		RemoveContext();
		if (signal)
			return GUSISetPosixError(EINTR);
	}
	if (fEvent & T_CONNECT)
		return 0;
	fEvent &= ~T_DISCONNECT;
	GUSISetPosixError(GetAsyncError());
freePeerName:
	delete fPeerName;
	fPeerName = nil;
	
	return -1;
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
ssize_t GUSIOTStreamSocket::recvfrom(
	const GUSIScatterer & buffer, int flags, void * from, socklen_t * fromlen)
{
	MopupEvents();
	OTFlags	 otflags;
	uint16_t exp	= fEvent & T_EXDATA;
	fEvent ^= exp;
	
	AddContextInScope	scope(this);
	
	OTResult res 	= OTRcv(fEndpoint, buffer.Buffer(), buffer.Length(), &otflags);
	if (res < 0 && fReadShutdown)
		return 0;
	if (res == kOTNoDataErr) {
		if (GUSISetPosixError(GetAsyncError()))
			return -1;
		fEvent &= ~(T_DATA|T_EXDATA);
		if (!fBlocking)
			return GUSISetPosixError(EWOULDBLOCK);
		bool signal = false;
		MopupEvents();
		while (res == kOTNoDataErr && !(res = fAsyncError)) {
			signal = GUSIContext::Yield(kGUSIBlock);
			MopupEvents();
			if (signal)
				break;
			res = OTRcv(fEndpoint, buffer.Buffer(), buffer.Length(), &otflags);
		}
		if (signal)
			return GUSISetPosixError(EINTR);
	}
	if (res < 0)
		return GUSISetMacError(res);
	else
		buffer.SetLength(res);
	if (from)
		fPeerName->Unpack(fStrategy, from, fromlen);
	// When the [[T_EXDATA]] event arrives, we might first get some non-expedited data
 // [[!(otflags & T_EXPEDITED)]] and then a packet of expedited data [[otflags & T_EXPEDITED]],
 // possibly with the [[T_MORE]] flag set (although that should not happen in TCP/IP). We 
 // therefore don't reset [[T_EXDATA]] until we have seen a packet with [[T_EXPEDITED]] set 
 // and [[T_MORE]] not set.                                                 
 //                                                                         
 // <Keep [[T_EXDATA]] flag set until we finish reading expedited data>=    
 if (exp && (otflags & (T_EXPEDITED|T_MORE)) != T_EXPEDITED) 
 	fEvent |= exp;
	return res;
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
ssize_t GUSIOTStreamSocket::sendto(
	const GUSIGatherer & buffer, int flags, const void * to, socklen_t tolen)
{
	MopupEvents();
	if (GUSISetPosixError(GetAsyncError()))
		return -1;
	long		done= 0;
	char * 		buf = static_cast<char *>(buffer.Buffer());
	long		len = buffer.Length();
	
	AddContextInScope	scope(this);
	
	while (len) {
		OTResult res = OTSnd(fEndpoint, buf, len, 0);
		if (res <= 0)
			if (res == kOTFlowErr) {
				fEvent &= ~T_GODATA;
				if (!fBlocking)
					return done ? done : GUSISetPosixError(EWOULDBLOCK);
				bool signal = GUSIContext::Yield(kGUSIBlock);
				MopupEvents();
				if (signal)
					return done ? done : GUSISetPosixError(EINTR);
			} else
				return GUSISetMacError(res);
		else {
			buf += res;
			len -= res;
			done+= res;
		}
	}
	return buffer.Length();
}
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
bool GUSIOTStreamSocket::select(bool * canRead, bool * canWrite, bool * except)
{
	MopupEvents();
	
	size_t	sz;
	bool 		res		= false;
	OTResult	state 	= OTGetEndpointState(fEndpoint);
	
	if (canRead) {
		if (*canRead = 
			fReadShutdown										// EOF
		 || fAsyncError											// Asynchronous error
		 ||	fEvent & T_DATA										// Data available
		 || state == T_INCON									// Connection pending
		 || fNextListener										// Connection pending
		) 
			res = true;
	}
	if (canWrite) {
		if (fWriteShutdown || fAsyncError) {
			res = *canWrite = true;
		} else {
			switch (state) {
			case T_DATAXFER:
			case T_INREL:
				if (*canWrite = (fEvent & T_GODATA) != 0)
					res = true;
				break;
			default:
				*canWrite = false;
			}
		}
	}
	if (except) {
		if (*except = (fEvent & T_EXDATA) != 0)
			res = true;
	}
	return res;
}	
// <Member functions for class [[GUSIOTStreamSocket]]>=                    
int GUSIOTStreamSocket::shutdown(int how)
{
	switch (how) {
	case SHUT_WR:
	case SHUT_RDWR:
		OTSndOrderlyDisconnect(fEndpoint);
	}
	
	return GUSIOTSocket::shutdown(how);
}
// \section{Implementation of [[GUSIOTDatagramSocket]]}                    
//                                                                         
// [[GUSIOTDatagramSocket]]s don't need to be connected.                   
//                                                                         
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
GUSIOTDatagramSocket::GUSIOTDatagramSocket(GUSIOTStrategy * strategy)
	: GUSIOTSocket(strategy)
{
	// <Initialize fields of [[GUSIOTDatagramSocket]]>=                        
 fPeerName	= nil;
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
GUSIOTSocket * GUSIOTDatagramSocket::Clone()
{
	return new GUSIOTDatagramSocket(fStrategy);
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
GUSIOTDatagramSocket::~GUSIOTDatagramSocket()
{
	delete fPeerName;
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
int GUSIOTDatagramSocket::BindIfUnbound()
{
	if (OTGetEndpointState(fEndpoint) == T_UNBND)
		return BindToAddress(nil);
	return 0;
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
int GUSIOTDatagramSocket::getpeername(void * name, socklen_t * namelen)
{
	if (!fPeerName)
		return GUSISetPosixError(ENOTCONN);
	return fPeerName->Unpack(fStrategy, name, namelen);
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
int GUSIOTDatagramSocket::connect(void * address, socklen_t addrlen)
{
	MopupEvents();
	if (BindIfUnbound())
		return -1;
	if (!fPeerName) {
		fPeerName = new (fEndpoint) GUSIOTTBind;
		if (!fPeerName)
			return GUSISetPosixError(ENOMEM);
	}
	if (fPeerName->Pack(fStrategy, address, addrlen, true)) {
		delete fPeerName;
		fPeerName = nil;
		
		return -1;
	}
	return 0;
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
ssize_t GUSIOTDatagramSocket::recvfrom(
	const GUSIScatterer & buffer, int flags, void * from, socklen_t * fromlen)
{
	AddContextInScope	scope(this);
	
	MopupEvents();
	GUSIOTTUnitData * data = new (fEndpoint, T_ADDR|T_OPT) GUSIOTTUnitData;
	if (!data)
		return GUSISetPosixError(ENOMEM);
	data->udata.len = data->udata.maxlen = buffer.Length();
	data->udata.buf	   = static_cast<UInt8 *>(buffer.Buffer());
retry:
	OTFlags	 otflags;
	OTResult res;
	while ((res = OTRcvUData(fEndpoint, data, &otflags)) == kOTNoDataErr) {
		if (GUSISetPosixError(GetAsyncError()))
			res = -1;
		else if (fReadShutdown) 
			res = 0;
		else if (!fBlocking)
			res = GUSISetPosixError(EWOULDBLOCK);
		else {
			bool signal = GUSIContext::Yield(kGUSIBlock);
			MopupEvents();
			if (signal)
				return GUSISetPosixError(EINTR);
			continue;
		}
		goto killDataAndReturn;
	}
	if (res < 0) {
		if (fReadShutdown) {
			res = 0;
		} else {
			GUSISetMacError(res);
			res = -1;
		}
	} else {
		// If we called [[connect]] on the socket, we want to restrict peer addresses.
  //                                                                         
  // <Compare received address to stored peer address>=                      
  if (fPeerName)
  	if (memcmp(
  		fPeerName->addr.buf, data->addr.buf, 
  		min(fPeerName->addr.len, data->addr.len))
  	)
  		goto retry; // No match
		if (from)
			data->Unpack(fStrategy, from, fromlen);
	}
	if (res >= 0) {
		res = buffer.SetLength(data->udata.len);
		// Open Transport requires one to read the entire datagram, so if there was a rest,
  // we use a temporary buffer. This part may be called recursively.         
  //                                                                         
  // <Read rest of datagram if buffer allocation was too stingy>=            
  if (otflags & T_MORE) {
  	char * buf = new char[1024];
  	recvfrom(GUSIScatterer(buf, 1024), 0, nil, nil);
  }
	}
killDataAndReturn:
	data->udata.buf = nil;
	delete data;
	return res;
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
ssize_t GUSIOTDatagramSocket::sendto(
	const GUSIGatherer & buffer, int flags, const void * to, socklen_t tolen)
{
	MopupEvents();
	if (GUSISetPosixError(GetAsyncError()))
		return -1;
	if (!to && !fPeerName)
		return GUSISetPosixError(ENOTCONN);
	auto_ptr<GUSIOTTUnitData> data(new (fEndpoint, to ? T_ADDR|T_OPT : T_OPT) GUSIOTTUnitData);
	if (!data.get())
		return GUSISetPosixError(ENOMEM);
	if (to) {
		if (data->Pack(fStrategy, to, tolen, true))
			return -1;
	} else if (fPeerName) {
		data->addr = fPeerName->addr;
	} 
	data->udata.len = data->udata.maxlen = buffer.Length();
	data->udata.buf = static_cast<UInt8 *>(buffer.Buffer());
	
	AddContextInScope	scope(this);
	OTResult 			res;
	int					error = 0;
	while ((res = OTSndUData(fEndpoint, data.get())) == kOTFlowErr) {
		if (!fBlocking) {
			error = EWOULDBLOCK;
			
			break;
		}
		bool signal = GUSIContext::Yield(kGUSIBlock);
		MopupEvents();
		if (signal) {
			error = EINTR;
			
			break;
		}
	}
	data->udata.buf = nil;
	if (!to)
		data->addr.buf = nil;
	if (error)
		return GUSISetPosixError(error);
	else if (res < 0)
		return GUSISetMacError(res);
	else
		res = data->udata.len;
	
	return res;
}
// <Member functions for class [[GUSIOTDatagramSocket]]>=                  
bool GUSIOTDatagramSocket::select(bool * canRead, bool * canWrite, bool * except)
{
	MopupEvents();
	bool 		res		= false;
	OTResult	state 	= OTGetEndpointState(fEndpoint);
	
	if (canRead) {
		if (*canRead = fAsyncError || fEvent & T_DATA) 
			res = true;
	}
	if (canWrite)
		switch (state) {
		case T_IDLE:
		case T_DATAXFER:
		case T_INREL:
			*canWrite = true;
			res   	  = true;
			break;
		default:
			*canWrite = fAsyncError != 0;
		}
	if (except)
		*except = false;

	return res;
}	

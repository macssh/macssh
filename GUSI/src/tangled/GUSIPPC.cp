// <GUSIPPC.cp>=                                                           
#include "GUSIInternal.h"
#include "GUSIPPC.h"
#include "GUSIBasics.h"
#include "GUSIBuffer.h"
#include "GUSISocketMixins.h"

#include <errno.h>

#include <algorithm>

GUSI_USING_STD_NAMESPACE

// \section{Definition of [[GUSIPPCSocket]]}                               
//                                                                         
// A [[GUSIPPCSocket]] is implemented with the usual read and write        
// buffers.                                                                
//                                                                         
// <Definition of class [[GUSIPPCSocket]]>=                                
class GUSIPPCSocket : 
	public GUSISocket,
	protected 	GUSISMBlocking,
	protected	GUSISMState,
	protected	GUSISMInputBuffer,
	protected 	GUSISMOutputBuffer
{
public:
	GUSIPPCSocket();
	virtual ~GUSIPPCSocket();
	// [[bind]] is absolutely required for all forms of PPC sockets.           
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual int bind(void * addr, socklen_t namelen);
 // [[connect]] opens a connection actively.                                
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual int connect(void * address, socklen_t addrlen);
 // Most of the dirty work of [[listen]] is already handled in [[GUSIPPCListen]].
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual int listen(int queueLength);
 // [[accept]] also is able to delegate most of the hard work to [[GUSIPPCListen]].
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual GUSISocket * accept(void *from, socklen_t *fromlen);
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual bool Supports(ConfigOption config);
 // [[recvfrom]] reads from [[fInputBuffer]].                               
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual ssize_t recvfrom(const GUSIScatterer & buffer, int, void * from, socklen_t * fromlen);
 // [[sendto]] writes to [[fOutputBuffer]]. As opposed to reads, writes have to
 // be executed fully. This leads to a problem when a nonblocking write wants to
 // write more data than the total length of the buffer. In this case, GUSI disregards
 // the nonblocking flag.                                                   
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual ssize_t sendto(const GUSIGatherer & buffer, int flags, const void * to, socklen_t);
 // [[select]] checks for various conditions on the socket.                 
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual bool	select(bool * canRead, bool * canWrite, bool * exception);
 // [[fcntl]] handles the blocking support.                                 
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual int fcntl(int cmd, va_list arg);
 // [[ioctl]] deals with blocking support and with [[FIONREAD]].            
 //                                                                         
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual int ioctl(unsigned int request, va_list arg);
 // <Overridden member functions for [[GUSIPPCSocket]]>=                    
 virtual int shutdown(int how);
protected:
	// \section{Implementation of [[GUSIPPCSocket]]}                           
 //                                                                         
 // The implementation of [[GUSIPPCSocket]] is similar to many other socket classes.
 // Let's start, once more, with the interrupt level routines.              
 //                                                                         
 // \subsection{Interrupt level routines for [[GUSIPPCSocket]]}             
 //                                                                         
 // Both [[GUSIPPCSendDone]] and [[GUSIPPCRecvDone]] are always called with the 
 // same [[TCPiopb]] in a  [[GUSIPPCSocket]] so they can easily find out the address 
 // of the socket itself. [[GUSIPPCSend]] and [[GUSIPPCRecv]] set up send and receive 
 // calls.                                                                  
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 OSErr				fAsyncError;
 PPCParamBlockRec	fSendPB;
 PPCParamBlockRec	fRecvPB;
 friend void 		GUSIPPCSend(GUSIPPCSocket * sock);
 friend void 		GUSIPPCRecv(GUSIPPCSocket * sock);
 friend void 		GUSIPPCSendDone(PPCParamBlockPtr pb);
 friend void 		GUSIPPCRecvDone(PPCParamBlockPtr pb);
 static PPCCompUPP	sSendProc;
 static PPCCompUPP	sRecvProc;
 // For listens it is necessary to build a backlog if the interrupt level gets opens
 // faster than the high level routines can accept them.                    
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 struct Listener {
 	PPCSessRefNum		fSession;
 	LocationNameRec		fLocation;
 	PPCPortRec			fPort;
 	bool				fBusy;
 };
 Listener *	fListeners;
 bool		fRestartListen;
 char		fNumListeners;
 char		fCurListener;
 char		fNextListener;
 friend void GUSIPPCListenDone(PPCParamBlockPtr pb);
 friend void GUSIPPCListen(GUSIPPCSocket * sock);
 static PPCCompUPP sListenProc;
 // [[SetupListener]] prepares a [[Listener]].                              
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 void SetupListener(Listener & listener);
 // Some functions simply need to wake up the socket again. These are always called
 // on [[fSendPB]], because [[fRecvPB]] might be in use by a [[PPCInform]] call.
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 friend void GUSIPPCDone(PPCParamBlockPtr pb);
 static PPCCompUPP sDoneProc;
 // \subsection{High level interface for [[GUSIPPCSocket]]}                 
 //                                                                         
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 LocationNameRec		fLocation;
 PPCPortRec			fPort;
 LocationNameRec		fPeerLoc;
 PPCPortRec			fPeerPort;
 // The port reference number is shared between a listener and all of its   
 // clients, so we cannot simply close it at any time and have to store it  
 // in a shared data structure.                                             
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 struct PortRef {
 	short			fRefCount;
 	PPCPortRefNum	fPort;
 	
 	PortRef(PPCPortRefNum port) : fRefCount(1), fPort(port) {}
 };
 PortRef *			fPortRef;
 // [[accept]] uses a special constructor of [[GUSIPPCSocket]] which constructs
 // a socket directly from a [[Listener]].                                  
 //                                                                         
 // <Privatissima of [[GUSIPPCSocket]]>=                                    
 GUSIPPCSocket(GUSIPPCSocket * orig, Listener & listener);
};
// \section{Implementation of [[GUSIPPCFactory]]}                          
//                                                                         
// To use PPC sockets, you need to call [[GUSIwithPPCSockets]] from your [[GUSISetupFactories]] routine.
//                                                                         
// <Member functions for class [[GUSIPPCFactory]]>=                        
extern "C" void GUSIwithPPCSockets()
{
	GUSISocketDomainRegistry::Instance()->AddFactory(AF_PPC, GUSIPPCFactory::Instance());
}
// <Member functions for class [[GUSIPPCFactory]]>=                        
GUSISocketFactory * GUSIPPCFactory::sInstance = nil;
// <Member functions for class [[GUSIPPCFactory]]>=                        
GUSISocket * GUSIPPCFactory::socket(int, int, int)
{
	return new GUSIPPCSocket;
}
// The UPPs for the completion procedures are set up the first time a socket
// is constructed.                                                         
//                                                                         
// <Member functions for class [[GUSIPPCSocket]]>=                         
PPCCompUPP	GUSIPPCSocket::sSendProc	= 0;
PPCCompUPP	GUSIPPCSocket::sRecvProc	= 0;
// <Member functions for class [[GUSIPPCSocket]]>=                         
PPCCompUPP	GUSIPPCSocket::sListenProc	= 0;
// <Member functions for class [[GUSIPPCSocket]]>=                         
void GUSIPPCSocket::SetupListener(Listener & listener)
{
	listener.fBusy	= false;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
PPCCompUPP	GUSIPPCSocket::sDoneProc	= 0;
// The constructors have to initialize a rather large number of data fields.
//                                                                         
// <Member functions for class [[GUSIPPCSocket]]>=                         
GUSIPPCSocket::GUSIPPCSocket()
{
	// <Initialize fields of [[GUSIPPCSocket]]>=                               
 if (!sSendProc)
 	sSendProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCSendDone));
 if (!sRecvProc)
 	sRecvProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCRecvDone));
 // <Initialize fields of [[GUSIPPCSocket]]>=                               
 fListeners 		=	nil;
 fRestartListen	=	true;
 fNumListeners	=	0;
 fCurListener	=	0;
 fNextListener   = 	0;
 if (!sListenProc)
 	sListenProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCListenDone));
 // <Initialize fields of [[GUSIPPCSocket]]>=                               
 if (!sDoneProc)
 	sDoneProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCDone));
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
int GUSIPPCSocket::bind(void * addr, socklen_t namelen)
{
	struct sockaddr_ppc *name	=	(struct sockaddr_ppc *)addr;
	// The address to be passed must be up to a minimal standard of decency.   
 // For instance, the host address must be either the real IP number of     
 // our host or one of the two legitimate pseudo-addresses for              
 // [["localhost"]].                                                        
 //                                                                         
 // <Sanity checks for [[GUSIPPCSocket::bind]]>=                            
 if (!GUSI_ASSERT_CLIENT(
 		namelen >= sizeof(struct sockaddr_ppc),
 		("bind: address len %d < %d\n", namelen, sizeof(struct sockaddr_ppc)))
 )
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_ASSERT_CLIENT(
 		name->sppc_family == AF_PPC,
 		("bind: family %d != %d\n", name->sppc_family, AF_PPC))
 )
 	return GUSISetPosixError(EAFNOSUPPORT);
 if (!GUSI_SASSERT_CLIENT(fState==Unbound, "bind: Socket already bound\n"))
 	return GUSISetPosixError(EINVAL);
	fPort			 	= name->sppc_port;
	fLocation			= name->sppc_location;
	
	PPCOpenPBRec & openParam = fSendPB.openParam;
	openParam.ioCompletion  = sDoneProc;
	openParam.serviceType	= ppcServiceRealTime;
	openParam.resFlag		= 0;
	openParam.portName		= &fPort;
	openParam.locationName	= &fLocation;
	openParam.networkVisible= true;
	PPCOpenAsync(&openParam);
	AddContext();
	while (fSendPB.openParam.ioResult == 1)
		GUSIContext::Yield(kGUSIBlock);
	RemoveContext();
	
	switch (fSendPB.openParam.ioResult) {
	case noErr:
		fPortRef = new PortRef(openParam.portRefNum);
		fState = Unconnected;
		return 0;
	case noGlobalsErr:
		return GUSISetPosixError(ENOMEM);
	case portNameExistsErr:
	case nbpDuplicate:
		return GUSISetPosixError(EADDRINUSE);
	case nameTypeErr:
	case badReqErr:
	case badPortNameErr:
	case badLocNameErr:
	default:
		return GUSISetPosixError(EINVAL);
	}
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
int GUSIPPCSocket::connect(void * address, socklen_t addrlen)
{
	sockaddr_ppc *	addr 	= (sockaddr_ppc *) address;

	// <Sanity checks for [[GUSIPPCSocket::connect]]>=                         
 if (!GUSI_CASSERT_CLIENT(addrlen >= int(sizeof(sockaddr_ppc))))
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_CASSERT_CLIENT(addr->sppc_family == AF_PPC))
 	return GUSISetPosixError(EAFNOSUPPORT);
 switch (fState) {
 case Unbound:
 	return GUSISetPosixError(EINVAL); 			// Must be bound before connecting
 case Unconnected:
 	break;										// Go ahead
 default:
 	return GUSISetPosixError(EISCONN);			// Already connected in some form
 }
		
	fPeerPort 	= addr->sppc_port;
	fPeerLoc	= addr->sppc_location;
	
	PPCStartPBRec	& startParam = fSendPB.startParam;
	startParam.portRefNum	= fPortRef->fPort;
	startParam.resFlag		= 0;
	startParam.portName		= &fPeerPort;
	startParam.locationName	= &fPeerLoc;
	startParam.userData		= 0;
	startParam.userRefNum	= 0;
	
	Boolean	guest;
	Str32 	uname;
	uname[0] = 0;
	
	switch (StartSecureSession(&startParam, uname, true, true, &guest, (StringPtr) "\p")) {
	case userCanceledErr:
		return GUSISetPosixError(EINTR);
	default:
		return GUSISetPosixError(EINVAL);
	case noErr:
		fState = Connected;
		
		PPCWritePBRec &	writeParam = fSendPB.writeParam;
		writeParam.ioCompletion	= sSendProc;
	
		PPCReadPBRec &	readParam = fRecvPB.readParam;
		readParam.ioCompletion	= sRecvProc;
		readParam.sessRefNum	= writeParam.sessRefNum;
		
		GUSIPPCSend(this);
		GUSIPPCRecv(this);
		
		return 0;
	}
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
int GUSIPPCSocket::listen(int queueLength)
{
	// <Sanity checks for [[GUSIPPCSocket::listen]]>=                          
 if (!GUSI_CASSERT_CLIENT(fState <= Unconnected))
 	return GUSISetPosixError(EISCONN);
 if (!GUSI_CASSERT_CLIENT(fState != Unbound))
 	return GUSISetPosixError(EINVAL);
	// For some weird reason, BSD multiplies queue lengths with a fudge factor.
 //                                                                         
 // <Adjust [[queueLength]] according to BSD definition>=                   
 if (queueLength < 1)
 	queueLength = 1;
 else if (queueLength > 4)
 	queueLength = 8;
 else
 	queueLength = ((queueLength * 3) >> 1) + 1;
	
	fInputBuffer.SwitchBuffer(0);
	fOutputBuffer.SwitchBuffer(0);
	fState 		= Listening;
	fListeners 	= new Listener[fNumListeners = queueLength];
	while (queueLength--)
		SetupListener(fListeners[queueLength]);
	
	GUSIPPCListen(this);
	
	return 0;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
GUSISocket * GUSIPPCSocket::accept(void *from, socklen_t *fromlen)
{
	GUSIPPCSocket *	sock;

	// <Sanity checks for [[GUSIPPCSocket::accept]]>=                          
 if (!GUSI_CASSERT_CLIENT(fState == Listening)) {
 	GUSISetPosixError(ENOTCONN); 
 	return nil;
 }
	// Listener slots are filled one by one, so we simply check whether the next listener
 // block has been filled yet.                                              
 //                                                                         
 // <Wait for a connection to arrive for the [[GUSIPPCSocket]]>=            
 if (!fListeners[fNextListener].fBusy && !fReadShutdown) {
 	if (!fBlocking) {
 		GUSISetPosixError(EWOULDBLOCK);
 		return nil;
 	}
 	bool signal = false;
 	AddContext();
 	while (!fListeners[fNextListener].fBusy && !fReadShutdown) {
 		if (signal = GUSIContext::Yield(kGUSIBlock))
 			break;
 	}
 	RemoveContext();
 	if (!fListeners[fNextListener].fBusy && signal)
 		return GUSISetPosixError(EINTR), static_cast<GUSISocket *>(0);
 }	
 if (!fListeners[fNextListener].fBusy && fReadShutdown)
 	return GUSISetPosixError(ESHUTDOWN), static_cast<GUSISocket *>(0);
	
	sock = new GUSIPPCSocket(this, fListeners[fNextListener]);
	
	SetupListener(fListeners[fNextListener]);
	fNextListener = (fNextListener+1) % fNumListeners;

	if (fRestartListen)
		GUSIPPCListen(this);

	if (sock && from)
		sock->getpeername(from, fromlen);

	return sock;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
GUSIPPCSocket::GUSIPPCSocket(GUSIPPCSocket * orig, Listener & listener)
{
	// <Initialize fields of [[GUSIPPCSocket]]>=                               
 if (!sSendProc)
 	sSendProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCSendDone));
 if (!sRecvProc)
 	sRecvProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCRecvDone));
 // <Initialize fields of [[GUSIPPCSocket]]>=                               
 fListeners 		=	nil;
 fRestartListen	=	true;
 fNumListeners	=	0;
 fCurListener	=	0;
 fNextListener   = 	0;
 if (!sListenProc)
 	sListenProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCListenDone));
 // <Initialize fields of [[GUSIPPCSocket]]>=                               
 if (!sDoneProc)
 	sDoneProc = NewPPCCompProc(reinterpret_cast<PPCCompProcPtr>(GUSIPPCDone));
	fLocation			= orig->fLocation;
	fPort				= orig->fPort;
	fPeerLoc			= listener.fLocation;
	fPeerPort			= listener.fPort;
	fPortRef			= orig->fPortRef;
	++fPortRef->fRefCount;
	fState 				= Connected;
	
	PPCWritePBRec &	writeParam = fSendPB.writeParam;
	writeParam.ioCompletion	= sSendProc;
	writeParam.sessRefNum	= listener.fSession;
	
	PPCReadPBRec &	readParam = fRecvPB.readParam;
	readParam.ioCompletion	= sRecvProc;
	readParam.sessRefNum	= listener.fSession;
		
	GUSIPPCSend(this);
	GUSIPPCRecv(this);
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
bool GUSIPPCSocket::Supports(ConfigOption config)
{
	return config == kSocketCalls;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
ssize_t GUSIPPCSocket::recvfrom(
	const GUSIScatterer & buffer, int flags, void * from, socklen_t * fromlen)
{
	if (from)
		getpeername(from, fromlen);

	// <Sanity checks for [[GUSIPPCSocket::recvfrom]]>=                        
 if (fReadShutdown&& !fInputBuffer.Valid())
 	return 0;
 switch (fState) {
 case Unbound:
 case Unconnected:
 case Listening:
 	return GUSISetPosixError(ENOTCONN);
 case Closing:
 case Connecting:
 case Connected:
 	break;
 }
	// The socket needs to be in [[Connected]] or [[Closing]] state and the input buffer needs to be
 // nonempty before a read can succeed.                                     
 //                                                                         
 // <Wait for valid data on [[GUSIPPCSocket]]>=                             
 if (!fReadShutdown 
  && (fState == Connecting || fState == Connected || fState == Closing) 
  && !fInputBuffer.Valid()
 ) {
 	if (!fBlocking)
 		return GUSISetPosixError(EWOULDBLOCK);
 	bool signal = false;
 	AddContext();
 	while (!fReadShutdown 
 	    && (fState == Connecting || fState == Connected || fState == Closing) 
 		&& !fInputBuffer.Valid()
 	)
 		if (signal = GUSIContext::Yield(kGUSIBlock))
 			break;
 	RemoveContext();
 	if (signal && !fInputBuffer.Valid())
 		return GUSISetPosixError(EINTR);
 }

	size_t len 	= buffer.Length();
	if (flags & MSG_PEEK) 
		fInputBuffer.Peek(buffer, len);
	else
		fInputBuffer.Consume(buffer, len);
	
	return (int)len;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
ssize_t GUSIPPCSocket::sendto(
	const GUSIGatherer & buffer, int flags, const void * to, socklen_t)
{
	// <Sanity checks for [[GUSIPPCSocket::sendto]]>=                          
 if (fWriteShutdown)
 	return GUSISetPosixError(ESHUTDOWN);

 if (!GUSI_SASSERT_CLIENT(!to, "Can't sendto() on a stream socket"))
 	return GUSISetPosixError(EOPNOTSUPP);
 switch (fState) {
 case Unbound:
 case Unconnected:
 case Listening:
 	return GUSISetPosixError(ENOTCONN);
 case Closing:
 case Connecting:
 case Connected:
 	break;
 }

 if (!fBlocking && !fOutputBuffer.Free())
 	return GUSISetPosixError(EAGAIN);
	
	size_t 	rest 	= buffer.Length();
	size_t 	offset 	= 0;
	while (rest) {
		size_t 	len 	= rest;
		bool	signal	= false;
		// <Wait for free buffer space on [[GUSIPPCSocket]]>=                      
  if (!fBlocking && !fOutputBuffer.Free())
  	break;
  if (!fWriteShutdown && (fState == Connecting || fState == Connected) && !fOutputBuffer.Free()) {
  	AddContext();
  	while (!fWriteShutdown 
  	    && (fState == Connecting || fState == Connected) 
  		&& !fOutputBuffer.Free()
  	)
  		signal = GUSIContext::Yield(kGUSIBlock);
  	RemoveContext();
  }
  if (signal || (fWriteShutdown && !fOutputBuffer.Free()))
  	if (offset)
  		break;
  	else
  		return GUSISetPosixError(signal ? EINTR : ESHUTDOWN);
		fOutputBuffer.Produce(buffer, len, offset);
		rest -= len;
	}
	
	return offset;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
bool GUSIPPCSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		if (*canRead = fReadShutdown ||
			(fState == Listening 
				? fListeners[fNextListener].fBusy 
				: fInputBuffer.Valid() > 0
			)
		)
			cond = true;
	if (canWrite)
		if (*canWrite = fWriteShutdown || fOutputBuffer.Free())
			cond = true;
		
	return cond;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
int GUSIPPCSocket::fcntl(int cmd, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoFcntl(&result, cmd, arg))
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("fcntl: illegal request %d\n", cmd));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
int GUSIPPCSocket::ioctl(unsigned int request, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoIoctl(&result, request, arg)
	 || GUSISMInputBuffer::DoIoctl(&result, request, arg)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("ioctl: illegal request %d\n", request));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
int GUSIPPCSocket::shutdown(int how)
{
	if (!GUSI_SASSERT_CLIENT(how >= 0 && how < 3, "shutdown: 0,1, or 2\n"))
		return GUSISetPosixError(EINVAL);
		
	GUSISMState::Shutdown(how);
	fOutputBuffer.Lock();
	fOutputBuffer.Release();
	
	return 0;
}
// <Member functions for class [[GUSIPPCSocket]]>=                         
GUSIPPCSocket::~GUSIPPCSocket()
{	
	switch (fState) {
	case Listening: 
		shutdown(2);
		// <Shut down listening [[GUSIPPCSocket]]>=                                
  fInputBuffer.Lock();
  for (int i = 0; i<fNumListeners; i++)
  	if (fListeners[i].fBusy) {
  		fSendPB.endParam.ioCompletion 	= sDoneProc;
  		fSendPB.endParam.sessRefNum 	= fListeners[i].fSession;
  		PPCEndAsync(&fSendPB.endParam);
  		AddContext();
  		while (fSendPB.endParam.ioResult == 1)
  			GUSIContext::Yield(kGUSIBlock);
  		RemoveContext();
  	}
		break;
	case Connected:
		shutdown(2);
		AddContext();
		while (fState == Connected)
			GUSIContext::Yield(kGUSIBlock);
		RemoveContext();
		// Fall through
	case Closing:
		fSendPB.endParam.ioCompletion = sDoneProc;
		PPCEndAsync(&fSendPB.endParam);
		AddContext();
		while (fSendPB.endParam.ioResult == 1)
			GUSIContext::Yield(kGUSIBlock);
		RemoveContext();
		break;
	}
	if (fPortRef && !--fPortRef->fRefCount) {
		fSendPB.closeParam.ioCompletion = sDoneProc;
		fSendPB.closeParam.portRefNum	= fPortRef->fPort;
		PPCCloseAsync(&fSendPB.closeParam);
		AddContext();
		while (fSendPB.closeParam.ioResult == 1)
			GUSIContext::Yield(kGUSIBlock);
		RemoveContext();
		delete fPortRef;
	}	
}
// [[GUSIPPCSendDone]] does all its work between [[fSendPB]] and [[fOutputBuffer]].
// If a send fails, the whole send buffer is cleared.                      
//                                                                         
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCSendDone(PPCParamBlockPtr pb)
{
	GUSIPPCSocket * sock = 
		reinterpret_cast<GUSIPPCSocket *>((char *)pb-offsetof(GUSIPPCSocket, fSendPB));
	if (sock->fOutputBuffer.Locked())
		sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIPPCSendDone), pb);
	else {
		PPCWritePBRec & writeParam	 	=	pb->writeParam;
		
		sock->fOutputBuffer.ClearDefer();
		sock->fOutputBuffer.FreeBuffer(writeParam.bufferPtr, writeParam.actualLength);
		if (writeParam.ioResult) {
			for (long valid; valid = sock->fOutputBuffer.Valid(); )
				sock->fOutputBuffer.FreeBuffer(nil, valid);
			sock->fWriteShutdown 	= true;
		}
		GUSIPPCSend(sock);
		sock->Wakeup();
	}
}
// [[GUSIPPCSend]] starts a PPC write call if there is data to send and otherwise
// sets itself up as the deferred procedure of the output buffer (and thus is 
// guaranteed to be called the next time data is deposited in the buffer again).
// If all data has been delivered and a shutdown is requested, send one.   
//                                                                         
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCSend(GUSIPPCSocket * sock)
{
	size_t	valid = sock->fOutputBuffer.Valid();
	
	sock->fOutputBuffer.ClearDefer();
	if (!valid) {
		if (!sock->fWriteShutdown)
			sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIPPCSend), sock);
		else if (sock->fState == GUSIPPCSocket::Connected)
			sock->fState = GUSIPPCSocket::Closing;
	} else {
		PPCWritePBRec & writeParam	 	=	sock->fSendPB.writeParam;
		valid = min(valid, min((size_t)65535, sock->fOutputBuffer.Size() >> 1));

		writeParam.bufferPtr	= 	
			reinterpret_cast<char *>(sock->fOutputBuffer.ConsumeBuffer(valid));
		writeParam.bufferLength	= 	(Size) valid;
		writeParam.more			=	false;
		writeParam.userData		=	0;
		writeParam.blockCreator	=	'GU·I';
		writeParam.blockType	=	'strm';
		
		PPCWriteAsync(&writeParam);		
	}
}
// [[GUSIPPCRecvDone]] does all its work between [[fRecvPB]] and [[fInputBuffer]].
//                                                                         
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCRecvDone(PPCParamBlockPtr pb)
{
	GUSIPPCSocket * sock = 
		reinterpret_cast<GUSIPPCSocket *>((char *)pb-offsetof(GUSIPPCSocket, fRecvPB));
	if (sock->fInputBuffer.Locked())
		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIPPCRecvDone), pb);
	else {
		PPCReadPBRec & readParam	 	=	pb->readParam;
		sock->fInputBuffer.ClearDefer();
		switch (readParam.ioResult) {
		case noErr:
			sock->fInputBuffer.ValidBuffer(readParam.bufferPtr, readParam.actualLength);
			GUSIPPCRecv(sock);
			break;
		default:
			sock->fReadShutdown = true;
			break;
		}
		sock->Wakeup();
	}
}
// [[GUSIPPCRecv]] starts a PPC receive call if there is room left and otherwise
// sets itself up as the deferred procedure of the output buffer (and thus is 
// guaranteed to be called the next time there is free space in the buffer again.
//                                                                         
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCRecv(GUSIPPCSocket * sock)
{
	size_t free = sock->fInputBuffer.Free();
	if (!free)
		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIPPCRecv), sock);
	else {
		PPCReadPBRec & readParam	 	=	sock->fRecvPB.readParam;

		sock->fInputBuffer.ClearDefer();
		free = min(free, min((size_t)65535, sock->fInputBuffer.Size() >> 1));
		
		readParam.ioCompletion	= 	sock->sRecvProc;
		readParam.bufferPtr		=	
			reinterpret_cast<char *>(sock->fInputBuffer.ProduceBuffer(free));
		readParam.bufferLength	=	free;
		
		PPCReadAsync(&readParam);		
	}
}
// [[GUSIPPCListenDone]] saves the connection parameters and starts the    
// next passive open, if possible. Blocking on [[fInputBuffer]] is somewhat
// bizarre; we're not actually using the buffer, just its lock. The only   
// times this lock is used is when the socket is shutting down.            
//                                                                         
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCListenDone(PPCParamBlockPtr pb)
{
	GUSIPPCSocket * sock = 
		(GUSIPPCSocket *)((char *)pb-offsetof(GUSIPPCSocket, fRecvPB));
	if (!(sock->fAsyncError = pb->informParam.ioResult)) {
		GUSIPPCSocket::Listener & listener = sock->fListeners[sock->fCurListener];
		listener.fSession	= pb->informParam.sessRefNum;
		listener.fBusy		= true;
		sock->fCurListener = (sock->fCurListener+1) % sock->fNumListeners;
	} 
	sock->Wakeup();
	if (!sock->fAsyncError)
		if (sock->fInputBuffer.Locked())
			sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIPPCListenDone), pb);
		else {
			sock->fInputBuffer.ClearDefer();
			GUSIPPCListen(sock);
		}
}
// [[GUSIPPCListen]] initiates a passive open.                             
//                                                                         
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCListen(GUSIPPCSocket * sock)
{
	if (sock->fReadShutdown)
		return;
	if (sock->fRestartListen = sock->fListeners[sock->fCurListener].fBusy) 
		return;
	PPCInformPBRec &			informParam = sock->fRecvPB.informParam;
	GUSIPPCSocket::Listener	&	listener	= sock->fListeners[sock->fCurListener];
	informParam.ioCompletion				= GUSIPPCSocket::sListenProc;
	informParam.portRefNum					= sock->fPortRef->fPort;
	informParam.autoAccept					= true;
	informParam.portName					= &listener.fPort;
	informParam.locationName 				= &listener.fLocation;
	informParam.userName					= nil;
	
	PPCInformAsync(&informParam);
}
// <Interrupt level routines for [[GUSIPPCSocket]]>=                       
void GUSIPPCDone(PPCParamBlockPtr pb)
{
	GUSIPPCSocket * sock = 
		(GUSIPPCSocket *)((char *)pb-offsetof(GUSIPPCSocket, fSendPB));
	sock->fAsyncError = pb->endParam.ioResult;
	sock->Wakeup();
}

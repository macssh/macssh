// <GUSIMTTcp.cp>=                                                         
#include "GUSIInternal.h"
#include "GUSIMTTcp.h"
#include "GUSIMTInet.h"
#include "GUSIMTNetDB.h"
#include "GUSIInet.h"
#include "GUSIDiag.h"
#include "GUSISocketMixins.h"

#include <errno.h>

#include <Devices.h>

#include <algorithm>

GUSI_USING_STD_NAMESPACE

// \section{Definition of [[GUSIMTTcpSocket]]}                             
//                                                                         
// The only specfic data member, [[fSelf]], serves as the [[userDataPtr]] of
// the TCP notification procedure. Since [[accept]] associates a different 
// [[GUSIMTTcpSocket]] with the same TCP [[StreamPtr]], passing [[this]] for this
// purpose would be fatal.                                                 
//                                                                         
// <Definition of class [[GUSIMTTcpSocket]]>=                              
class GUSIMTTcpSocket : public GUSIMTInetSocket, public GUSISMProcess {
public:
	GUSIMTTcpSocket();
	~GUSIMTTcpSocket();
	// [[connect]] opens a connection actively.                                
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual int connect(void * address, socklen_t addrlen);
 // Most of the dirty work of [[listen]] is already handled in [[GUSIMTTListen]].
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual int listen(int queueLength);
 // [[accept]] also is able to delegate most of the hard work to [[GUSIMTTListen]].
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual GUSISocket * accept(void *from, socklen_t *fromlen);
 // [[recvfrom]] reads from [[fInputBuffer]].                               
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual ssize_t recvfrom(const GUSIScatterer & buffer, int, void * from, socklen_t * fromlen);
 // [[sendto]] writes to [[fOutputBuffer]]. As opposed to reads, writes have to
 // be executed fully. This leads to a problem when a nonblocking write wants to
 // write more data than the total length of the buffer. In this case, GUSI disregards
 // the nonblocking flag.                                                   
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual ssize_t sendto(const GUSIGatherer & buffer, int flags, const void * to, socklen_t);
 // [[select]] checks for various conditions on the socket.                 
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual bool	select(bool * canRead, bool * canWrite, bool * exception);
 // [[shutdown]] for writing sends a closing notice. [[fOutputBuffer]] is locked and
 // released so the [[TCPClose]] is sent.                                   
 //                                                                         
 // <Overridden member functions for [[GUSIMTTcpSocket]]>=                  
 virtual int shutdown(int how);
private:
	GUSIMTTcpSocket ** fSelf;
	// \section{Implementation of [[GUSIMTTcpSocket]]}                         
 //                                                                         
 // The implementation of [[GUSIMTTcpSocket]] consists of a synchronous high level
 // part which mostly deals with [[GUSIRingBuffers]] and an asynchronous low
 // level part. The low level procedures in their single-mindedness are actually
 // simpler to explain, so we start with them.                              
 //                                                                         
 // \subsection{Interrupt level routines for [[GUSIMTTcpSocket]]}           
 //                                                                         
 // Both [[GUSIMTTSendDone]] and [[GUSIMTTRecvDone]] are always called with the 
 // same [[TCPiopb]] in a  [[GUSIMTTcpSocket]] so they can easily find out the address 
 // of the socket itself. [[GUSIMTTSend]] and [[GUSIMTTRecv]] set up send and receive 
 // calls.                                                                  
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 TCPiopb						fSendPB;
 MiniWDS						fSendWDS;
 TCPiopb						fRecvPB;
 friend void 				GUSIMTTSend(GUSIMTTcpSocket * sock);
 friend void 				GUSIMTTRecv(GUSIMTTcpSocket * sock);
 friend void 				GUSIMTTSendDone(TCPiopb * pb);
 friend void 				GUSIMTTRecvDone(TCPiopb * pb);
 static TCPIOCompletionUPP	sSendProc;
 static TCPIOCompletionUPP	sRecvProc;
 // For some global events, MacTCP calls a notification procedure.          
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 friend pascal void GUSIMTTNotify(
 					StreamPtr, u_short, GUSIMTTcpSocket **, 
 					u_short, struct ICMPReport *);
 static TCPNotifyUPP	sNotifyProc;
 // When a [[connect]] completes, we can fill in the peer address. [[connect]]
 // uses [[fSendPB]].                                                       
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 friend void GUSIMTTConnectDone(TCPiopb * pb);
 static TCPIOCompletionUPP sConnectProc;
 // Passive opens work similarly, but it is necessary to build a backlog if the
 // interrupt level gets opens faster than the high level routines can accept them.
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 struct Listener {
 	StreamPtr			fTcp;
 	GUSIMTTcpSocket **	fRef;
 	sockaddr_in			fSockAddr;
 	sockaddr_in			fPeerAddr;
 	bool				fBusy;
 };
 Listener *	fListeners;
 bool		fRestartListen;
 char		fNumListeners;
 char		fCurListener;
 char		fNextListener;
 friend void GUSIMTTListenDone(TCPiopb * pb);
 friend void GUSIMTTListen(GUSIMTTcpSocket * sock);
 static TCPIOCompletionUPP sListenProc;
 // [[CreateStream]] creates a TCP stream.                                  
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 StreamPtr CreateStream(GUSIMTTcpSocket ** socketRef);
 // [[SetupListener]] prepares a [[Listener]].                              
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 void SetupListener(Listener & listener);
 // [[accept]] uses a special constructor of [[GUSIMTTcpSocket]] which constructs
 // a socket directly from a [[Listener]].                                  
 //                                                                         
 // <Privatissima of [[GUSIMTTcpSocket]]>=                                  
 GUSIMTTcpSocket(Listener & listener);
};
// [[GUSIMTTSendDone]] does all its work between [[fSendPB]] and [[fOutputBuffer]].
// If a send fails, the whole send buffer is cleared.                      
//                                                                         
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTSendDone(TCPiopb * pb)
{
	GUSIMTTcpSocket * sock = 
		reinterpret_cast<GUSIMTTcpSocket *>((char *)pb-offsetof(GUSIMTTcpSocket, fSendPB));
	GUSIProcess::A5Saver saveA5(sock->Process());	
	if (sock->fOutputBuffer.Locked())
		sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTTSendDone), pb);
	else {
		sock->fOutputBuffer.ClearDefer();
		sock->fOutputBuffer.FreeBuffer(sock->fSendWDS.fDataPtr, sock->fSendWDS.fLength);
		if (sock->SetAsyncMacError(sock->fSendPB.ioResult)) {
			for (long valid; valid = sock->fOutputBuffer.Valid(); )
				sock->fOutputBuffer.FreeBuffer(nil, valid);
			sock->fWriteShutdown 	= true;
		}
		GUSIMTTSend(sock);
		sock->Wakeup();
	}
}
// [[GUSIMTTSend]] starts a tcp send call if there is data to send and otherwise
// sets itself up as the deferred procedure of the output buffer (and thus is 
// guaranteed to be called the next time data is deposited in the buffer again).
// If all data has been delivered and a shutdown is requested, send one.   
//                                                                         
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTSend(GUSIMTTcpSocket * sock)
{
	size_t	valid = sock->fOutputBuffer.Valid();
	
	sock->fOutputBuffer.ClearDefer();
	if (!valid) {
		if (!sock->fWriteShutdown)
			sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTTSend), sock);
		else if (sock->fState == GUSIMTTcpSocket::Connected) {
			sock->fState								= GUSISMState::Closing;
			sock->fSendPB.ioCompletion					= nil;
			sock->fSendPB.csCode						= TCPClose;
			sock->fSendPB.csParam.close.validityFlags 	= timeoutValue | timeoutAction;
			sock->fSendPB.csParam.close.ulpTimeoutValue = 60 /* seconds */;
			sock->fSendPB.csParam.close.ulpTimeoutAction= 0 /* 0:abort 1:report */;

			PBControlAsync(ParmBlkPtr(&sock->fSendPB));
		} 
	} else {
		valid = min(valid, min((size_t)65535, sock->fOutputBuffer.Size() >> 1));

		sock->fSendWDS.fDataPtr	= 
			static_cast<Ptr>(sock->fOutputBuffer.ConsumeBuffer(valid));
		sock->fSendWDS.fLength	= (u_short) valid;
		
		sock->fSendPB.ioCompletion					= sock->sSendProc;
		sock->fSendPB.csCode						= TCPSend;
		sock->fSendPB.csParam.send.validityFlags 	= timeoutValue | timeoutAction;
		sock->fSendPB.csParam.send.ulpTimeoutValue 	= 60 /* seconds */;
		sock->fSendPB.csParam.send.ulpTimeoutAction = 0 /* 0:abort 1:report */;
		sock->fSendPB.csParam.send.wdsPtr 			= &sock->fSendWDS;
		sock->fSendPB.csParam.send.sendFree 		= 0;
		sock->fSendPB.csParam.send.sendLength 		= 0;
		sock->fSendPB.csParam.send.urgentFlag		= 0;
		sock->fSendPB.csParam.send.pushFlag			= 
			valid == sock->fOutputBuffer.Valid();
			
		PBControlAsync(ParmBlkPtr(&sock->fSendPB));		
	}
}
// [[GUSIMTTRecvDone]] does all its work between [[fRecvPB]] and [[fInputBuffer]].
//                                                                         
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTRecvDone(TCPiopb * pb)
{
	GUSIMTTcpSocket * sock = 
		reinterpret_cast<GUSIMTTcpSocket *>((char *)pb-offsetof(GUSIMTTcpSocket, fRecvPB));
	GUSIProcess::A5Saver saveA5(sock->Process());	
	if (sock->fInputBuffer.Locked())
		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTTRecvDone), pb);
	else {
		sock->fInputBuffer.ClearDefer();
		switch (sock->fRecvPB.ioResult) {
		case noErr:
			sock->fInputBuffer.ValidBuffer(
				sock->fRecvPB.csParam.receive.rcvBuff,
				sock->fRecvPB.csParam.receive.rcvBuffLen);
			// Fall through
		case commandTimeout:
			GUSIMTTRecv(sock);
			break;
		default:
			sock->SetAsyncMacError(sock->fRecvPB.ioResult);
		case connectionClosing:
			sock->fReadShutdown = true;
			break;
		}
		sock->Wakeup();
	}
}
// [[GUSIMTTRecv]] starts a tcp receive call if there is room left and otherwise
// sets itself up as the deferred procedure of the output buffer (and thus is 
// guaranteed to be called the next time there is free space in the buffer again).
//                                                                         
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTRecv(GUSIMTTcpSocket * sock)
{
	size_t	free = sock->fInputBuffer.Free();
	if (!free)
		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTTRecv), sock);
	else {
		sock->fInputBuffer.ClearDefer();
		free = min(free, min((size_t)65535, sock->fInputBuffer.Size() >> 1));
		
		sock->fRecvPB.ioCompletion				= sock->sRecvProc;
		sock->fRecvPB.csCode 					= TCPRcv;
		sock->fRecvPB.csParam.receive.rcvBuff	= 
			static_cast<Ptr>(sock->fInputBuffer.ProduceBuffer(free));
		sock->fRecvPB.csParam.receive.rcvBuffLen= free;
		sock->fRecvPB.csParam.receive.commandTimeoutValue	= 120; 
		
		PBControlAsync(ParmBlkPtr(&sock->fRecvPB));		
	}
}
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
pascal void GUSIMTTNotify(
	StreamPtr, 
	u_short eventCode, GUSIMTTcpSocket ** sp, u_short, struct ICMPReport *)
{
	GUSIMTTcpSocket * sock = *sp;
	
	switch (eventCode) {
	case TCPClosing:
		sock->fReadShutdown		= true;
		break;
	case TCPTerminate:
		sock->fReadShutdown		= true;
		sock->fWriteShutdown	= true;
		sock->fState 			= GUSISMState::Unconnected;
		break;
	}
	sock->Wakeup();
}
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTConnectDone(TCPiopb * pb)
{
	GUSIMTTcpSocket * sock = 
		(GUSIMTTcpSocket *)((char *)pb-offsetof(GUSIMTTcpSocket, fSendPB));
	GUSIProcess::A5Saver saveA5(sock->Process());	
	if (!sock->SetAsyncMacError(pb->ioResult)) {
		sock->fSockAddr.sin_family		= AF_INET;
		sock->fSockAddr.sin_addr.s_addr = pb->csParam.open.localHost;
		sock->fSockAddr.sin_port 		= pb->csParam.open.localPort;
		sock->fPeerAddr.sin_family		= AF_INET;
		sock->fPeerAddr.sin_addr.s_addr	= pb->csParam.open.remoteHost;
		sock->fPeerAddr.sin_port 		= pb->csParam.open.remotePort;
		sock->fState 					= GUSISMState::Connected;

		GUSIMTTSend(sock);
		GUSIMTTRecv(sock);
	} else
		sock->fState 					= GUSISMState::Unconnected;
	GUSI_MESSAGE(("Connect %x\n", sock));
	sock->Wakeup();
}
// [[GUSIMTTListenDone]] saves the connection parameters and starts the    
// next passive open, if possible. Blocking on [[fInputBuffer]] is somewhat
// bizarre; we're not actually using the buffer, just its lock. The only   
// times this lock is used is while we're waiting for a local socket number
// to be assigned to an unbound listener socket and when the socket is shutting
// down.                                                                   
//                                                                         
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTListenDone(TCPiopb * pb)
{
	bool allowRestart = true;
	GUSIMTTcpSocket * sock = 
		(GUSIMTTcpSocket *)((char *)pb-offsetof(GUSIMTTcpSocket, fRecvPB));
	GUSIProcess::A5Saver saveA5(sock->Process());	
	switch (pb->ioResult) {
		case commandTimeout:
		case openFailed:
			break;
		default:
			if (!sock->SetAsyncMacError(pb->ioResult)) {
				GUSIMTTcpSocket::Listener & listener = sock->fListeners[sock->fCurListener];
				listener.fSockAddr.sin_family		= AF_INET;
				listener.fSockAddr.sin_addr.s_addr 	= pb->csParam.open.localHost;
				listener.fSockAddr.sin_port 		= pb->csParam.open.localPort;
				listener.fPeerAddr.sin_family		= AF_INET;
				listener.fPeerAddr.sin_addr.s_addr	= pb->csParam.open.remoteHost;
				listener.fPeerAddr.sin_port 		= pb->csParam.open.remotePort;
				listener.fBusy	= true;
				sock->fCurListener = (sock->fCurListener+1) % sock->fNumListeners;
				GUSI_MESSAGE(("Listen %x\n", &listener));
			} else 
				allowRestart = false;
		}
	sock->Wakeup();
	if (allowRestart)
		if (sock->fInputBuffer.Locked())
			sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTTListenDone), pb);
		else {
			sock->fInputBuffer.ClearDefer();
			GUSIMTTListen(sock);
		}
}
// [[GUSIMTTListen]] initiates a passive open.                             
//                                                                         
// <Interrupt level routines for [[GUSIMTTcpSocket]]>=                     
void GUSIMTTListen(GUSIMTTcpSocket * sock)
{
	if (sock->fRestartListen = sock->fListeners[sock->fCurListener].fBusy) 
		return; 
	sock->fRecvPB.tcpStream		= sock->fListeners[sock->fCurListener].fTcp;
	sock->fRecvPB.ioCompletion						= sock->sListenProc;
	sock->fRecvPB.csCode 							= TCPPassiveOpen;
	sock->fRecvPB.csParam.open.validityFlags 		= timeoutValue | timeoutAction;
	sock->fRecvPB.csParam.open.ulpTimeoutValue 		= 300 /* seconds */;
	sock->fRecvPB.csParam.open.ulpTimeoutAction 	= 1 /* 1:abort 0:report */;
	sock->fRecvPB.csParam.open.commandTimeoutValue	= 0 /* infinity */;
	sock->fRecvPB.csParam.open.remoteHost 	= 0;
	sock->fRecvPB.csParam.open.remotePort 	= 0;
	sock->fRecvPB.csParam.open.localHost 	= sock->fSockAddr.sin_addr.s_addr;
	sock->fRecvPB.csParam.open.localPort 	= sock->fSockAddr.sin_port;
	sock->fRecvPB.csParam.open.dontFrag 	= 0;
	sock->fRecvPB.csParam.open.timeToLive 	= 0;
	sock->fRecvPB.csParam.open.security 	= 0;
	sock->fRecvPB.csParam.open.optionCnt 	= 0;
	
	// If we do a [[listen]] on an unbound socket, MacTCP assigns a socket number,
 // which strangely happens at some time {\em after} the asynchronous call has
 // initially returned but {\em before} the call has completed. Thus, we lock to
 // prevent further opens from starting and then do a sort-of-busy wait. Thanks
 // to Peter Lewis for his explanations on that point.                      
 //                                                                         
 // <Do the [[TCPPassiveOpen]] and pick up port number if necessary>=       
 if (!sock->fSockAddr.sin_port)  {
 	sock->fInputBuffer.Lock();
 	PBControlAsync(ParmBlkPtr(&sock->fRecvPB));
 	while (!sock->fRecvPB.csParam.open.localPort)
 		GUSIContext::Yield(kGUSIPoll);
 	sock->fSockAddr.sin_port = sock->fRecvPB.csParam.open.localPort;
 	sock->fInputBuffer.Release();
 } else 
 	PBControlAsync(ParmBlkPtr(&sock->fRecvPB));
}
// The UPPs for the completion procedures are set up the first time a socket
// is constructed.                                                         
//                                                                         
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
TCPIOCompletionUPP	GUSIMTTcpSocket::sSendProc	= 0;
TCPIOCompletionUPP	GUSIMTTcpSocket::sRecvProc	= 0;
// The UPP for the notification procedure is set up the first time a socket
// is constructed.                                                         
//                                                                         
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
TCPNotifyUPP	GUSIMTTcpSocket::sNotifyProc	= 0;
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
TCPIOCompletionUPP	GUSIMTTcpSocket::sConnectProc	= 0;
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
TCPIOCompletionUPP	GUSIMTTcpSocket::sListenProc	= 0;
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
StreamPtr GUSIMTTcpSocket::CreateStream(GUSIMTTcpSocket ** socketRef)
{
	fSendPB.ioCompletion				= nil;
	fSendPB.csCode 						= TCPCreate;
	fSendPB.csParam.create.rcvBuff 		= (char *)NewPtr(8192);
	fSendPB.csParam.create.rcvBuffLen 	= 8192;
	fSendPB.csParam.create.notifyProc 	= sNotifyProc;
	fSendPB.csParam.create.userDataPtr	= Ptr(socketRef);
	
	PBControlSync(ParmBlkPtr(&fSendPB));
	
	if (fSendPB.ioResult)
		return nil;
	else 
		return fSendPB.tcpStream;
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
void GUSIMTTcpSocket::SetupListener(Listener & listener)
{
	listener.fRef 	= new (GUSIMTTcpSocket *);
	*listener.fRef	= this;
	listener.fTcp	= CreateStream(listener.fRef);
	listener.fBusy	= false;
}
// \subsection{High level interface for [[GUSIMTTcpSocket]]}               
//                                                                         
// The constructor has to initialize a rather large number of data fields, and as a 
// side effect opens the MacTCP driver if necessary. No other interesting activity
// occurs.                                                                 
//                                                                         
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
GUSIMTTcpSocket::GUSIMTTcpSocket()
{
	// <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 if (!sSendProc)
 	sSendProc = NewTCPIOCompletionProc(GUSIMTTSendDone);
 if (!sRecvProc)
 	sRecvProc = NewTCPIOCompletionProc(GUSIMTTRecvDone);
 // The send and receive parameter blocks are highly specialized and never really
 // change during the existence of a socket.                                
 //                                                                         
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 fSendPB.ioCRefNum 							= GUSIMTInetSocket::Driver();
 fRecvPB.ioCRefNum 							= GUSIMTInetSocket::Driver();
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 fSelf = nil;
 if (!sNotifyProc)
 	sNotifyProc = NewTCPNotifyProc(TCPNotifyProcPtr(GUSIMTTNotify));
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 if (!sConnectProc)
 	sConnectProc = NewTCPIOCompletionProc(GUSIMTTConnectDone);
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 fListeners 		=	nil;
 fRestartListen	=	true;
 fNumListeners	=	0;
 fCurListener	=	0;
 fNextListener   = 	0;
 if (!sListenProc)
 	sListenProc = NewTCPIOCompletionProc(GUSIMTTListenDone);
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
int GUSIMTTcpSocket::connect(void * address, socklen_t addrlen)
{
	sockaddr_in *	addr 	= (sockaddr_in *) address;

	// <Sanity checks for [[GUSIMTTcpSocket::connect]]>=                       
 if (!GUSI_CASSERT_CLIENT(addrlen >= int(sizeof(sockaddr_in))))
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_CASSERT_CLIENT(addr->sin_family == AF_INET))
 	return GUSISetPosixError(EAFNOSUPPORT);
 if (GUSISetPosixError(GetAsyncError()))				// non-blocking connect failed
 	return -1;
 switch (fState) {
 case Connecting:
 	return GUSISetPosixError(EALREADY);			// non-blocking connect in progress
 case Unbound:
 case Unconnected:
 	break;										// Go ahead
 default:
 	return GUSISetPosixError(EISCONN);			// Already connected in some form
 }

	if (!fSelf) {
		fSelf = new (GUSIMTTcpSocket *);
		*fSelf = this;
	}
	if (!fStream)
		if (!(fStream = CreateStream(fSelf)))
			return GUSISetPosixError(ENFILE);
	
	fSendPB.tcpStream						= fStream;
	fRecvPB.tcpStream						= fStream;
	
	fSendPB.ioCompletion					= sConnectProc;
	fSendPB.csCode 							= TCPActiveOpen;
	fSendPB.csParam.open.validityFlags 		= timeoutValue | timeoutAction;
	fSendPB.csParam.open.ulpTimeoutValue 	= 60 /* seconds */;
	fSendPB.csParam.open.ulpTimeoutAction 	= 1 /* 1:abort 0:report */;
	fSendPB.csParam.open.commandTimeoutValue= 0;
	fSendPB.csParam.open.remoteHost 		= addr->sin_addr.s_addr;
	fSendPB.csParam.open.remotePort 		= addr->sin_port;
	fSendPB.csParam.open.localHost 			= fSockAddr.sin_addr.s_addr;
	fSendPB.csParam.open.localPort 			= fSockAddr.sin_port;
	fSendPB.csParam.open.dontFrag 			= 0;
	fSendPB.csParam.open.timeToLive 		= 0;
	fSendPB.csParam.open.security 			= 0;
	fSendPB.csParam.open.optionCnt 			= 0;
	fState 									= Connecting;

	SetAsyncPosixError(0);
	if (GUSISetMacError(PBControlAsync(ParmBlkPtr(&fSendPB))))
		return -1;
		
	if (!fBlocking)
		return GUSISetPosixError(EINPROGRESS);

	AddContext();
	while (fSendPB.ioResult == inProgress)
		GUSIContext::Yield(kGUSIBlock);
	RemoveContext();

	return GUSISetPosixError(GetAsyncError());
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
int GUSIMTTcpSocket::listen(int queueLength)
{
	// <Sanity checks for [[GUSIMTTcpSocket::listen]]>=                        
 if (!GUSI_CASSERT_CLIENT(fState <= Unconnected))
 	return GUSISetPosixError(EISCONN);
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
	
	GUSIMTTListen(this);
	
	return 0;
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
GUSISocket * GUSIMTTcpSocket::accept(void *from, socklen_t *fromlen)
{
	GUSIMTTcpSocket *	sock;

	// <Sanity checks for [[GUSIMTTcpSocket::accept]]>=                        
 if (!GUSI_CASSERT_CLIENT(fState == Listening)) {
 	GUSISetPosixError(ENOTCONN); 
 	return nil;
 }
	// Listener slots are filled one by one, so we simply check whether the next listener
 // block has been filled yet.                                              
 //                                                                         
 // <Wait for a connection to arrive for the [[GUSIMTTcpSocket]]>=          
 if (!fListeners[fNextListener].fBusy && !fReadShutdown) {
 	if (!fBlocking)
 		return GUSISetPosixError(EWOULDBLOCK), static_cast<GUSISocket *>(0);
 	bool signal = false;
 	AddContext();
 	while (!fListeners[fNextListener].fBusy && !fReadShutdown)
 		if (signal = GUSIContext::Yield(kGUSIBlock))
 			break;
 	RemoveContext();
 	if (signal)
 		return GUSISetPosixError(EINTR), static_cast<GUSISocket *>(0);
 }	
 if (!fListeners[fNextListener].fBusy && fReadShutdown) {
 	GUSISetPosixError(ESHUTDOWN);
 	return nil;
 }
	
	sock = new GUSIMTTcpSocket(fListeners[fNextListener]);
	
	SetupListener(fListeners[fNextListener]);
	fNextListener = (fNextListener+1) % fNumListeners;

	if (fRestartListen)
		GUSIMTTListen(this);

	if (sock && from)
		sock->getpeername(from, fromlen);

	return sock;
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
GUSIMTTcpSocket::GUSIMTTcpSocket(Listener & listener)
{
	// <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 if (!sSendProc)
 	sSendProc = NewTCPIOCompletionProc(GUSIMTTSendDone);
 if (!sRecvProc)
 	sRecvProc = NewTCPIOCompletionProc(GUSIMTTRecvDone);
 // The send and receive parameter blocks are highly specialized and never really
 // change during the existence of a socket.                                
 //                                                                         
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 fSendPB.ioCRefNum 							= GUSIMTInetSocket::Driver();
 fRecvPB.ioCRefNum 							= GUSIMTInetSocket::Driver();
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 fSelf = nil;
 if (!sNotifyProc)
 	sNotifyProc = NewTCPNotifyProc(TCPNotifyProcPtr(GUSIMTTNotify));
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 if (!sConnectProc)
 	sConnectProc = NewTCPIOCompletionProc(GUSIMTTConnectDone);
 // <Initialize fields of [[GUSIMTTcpSocket]]>=                             
 fListeners 		=	nil;
 fRestartListen	=	true;
 fNumListeners	=	0;
 fCurListener	=	0;
 fNextListener   = 	0;
 if (!sListenProc)
 	sListenProc = NewTCPIOCompletionProc(GUSIMTTListenDone);
	fSockAddr 			= listener.fSockAddr;
	fPeerAddr 			= listener.fPeerAddr;
	fSelf	  			= listener.fRef;
	*fSelf    			= this;
	fStream	  			= listener.fTcp;
	fState 				= Connected;
	fSendPB.tcpStream	= fStream;
	fRecvPB.tcpStream	= fStream;
		
	GUSIMTTSend(this);
	GUSIMTTRecv(this);
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
ssize_t GUSIMTTcpSocket::recvfrom(
	const GUSIScatterer & buffer, int flags, void * from, socklen_t * fromlen)
{
	if (from)
		getpeername(from, fromlen);

	// <Sanity checks for [[GUSIMTTcpSocket::recvfrom]]>=                      
 if (!fInputBuffer.Valid())
 	if (GUSISetPosixError(GetAsyncError()))
 		return -1;
 	else if (fReadShutdown)
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
 // <Wait for valid data on [[GUSIMTTcpSocket]]>=                           
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
 	if (signal)
 		return GUSISetPosixError(EINTR);
 }

	size_t len 	= buffer.Length();
	if (flags & MSG_PEEK) 
		fInputBuffer.Peek(buffer, len);
	else
		fInputBuffer.Consume(buffer, len);
	
	return (ssize_t)len;
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
ssize_t GUSIMTTcpSocket::sendto(
	const GUSIGatherer & buffer, int flags, const void * to, socklen_t)
{
	// <Sanity checks for [[GUSIMTTcpSocket::sendto]]>=                        
 if (GUSISetPosixError(GetAsyncError()))
 	return -1;
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

 if (!fBlocking && (fState == Connecting || !fOutputBuffer.Free()))
 	return GUSISetPosixError(EWOULDBLOCK);
	
	size_t rest 	= buffer.Length();
	size_t offset = 0;
	while (rest) {
		size_t len = rest;
		// <Wait for free buffer space on [[GUSIMTTcpSocket]]>=                    
  if (!fBlocking && !fOutputBuffer.Free())
  	break;
  if (!fWriteShutdown && (fState == Connecting || fState == Connected) && !fOutputBuffer.Free()) {
  	bool signal = false;
  	AddContext();
  	while (!fWriteShutdown && (fState == Connecting || fState == Connected) && !fOutputBuffer.Free())
  		if (signal = GUSIContext::Yield(kGUSIBlock))
  			break;
  	RemoveContext();
  	if (signal)
  		if (offset)
  			break;
  		else
  			GUSISetPosixError(EINTR);
  }
  if (fWriteShutdown && !fOutputBuffer.Free())
  	if (offset)
  		break;
  	else
  		return GUSISetPosixError(ESHUTDOWN);
		fOutputBuffer.Produce(buffer, len, offset);
		rest -= len;
	}
		
	return offset;
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
bool GUSIMTTcpSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		if (*canRead = fReadShutdown ||
			fAsyncError ||
			(fState == Listening 
				? fListeners[fNextListener].fBusy 
				: fInputBuffer.Valid() > 0
			)
		)
			cond = true;
	if (canWrite)
		if (*canWrite = fWriteShutdown || fAsyncError || fOutputBuffer.Free())
			cond = true;
	
	if (cond)
		GUSI_MESSAGE(("Select%s%s\n", 
			(canRead && *canRead ? " read" : ""),
			(canWrite && *canWrite ? " write" : "")));
		
	return cond;
}
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
int GUSIMTTcpSocket::shutdown(int how)
{
	if (GUSIMTInetSocket::shutdown(how))
		return -1;
	fOutputBuffer.Lock();
	fOutputBuffer.Release();

	return 0;
}
// MacTCP has ways to make you feel very sorry for yourself if you don't close 
// streams.                                                                
//                                                                         
// <Member functions for class [[GUSIMTTcpSocket]]>=                       
GUSIMTTcpSocket::~GUSIMTTcpSocket()
{
	TCPiopb pb;
	
	pb.ioCRefNum	= GUSIMTInetSocket::Driver();
	pb.csCode 		= TCPRelease;
	
	if (fState == Listening) {
		// <Shut down listening [[GUSIMTTcpSocket]]>=                              
  fInputBuffer.Lock();
  for (int i = 0; i<fNumListeners; i++) {
  	pb.tcpStream = fListeners[i].fTcp;
  	if (PBControlSync(ParmBlkPtr(&pb)))
  		continue;
  	DisposePtr(pb.csParam.create.rcvBuff); /* there is no release pb */
  }
	} else if (fStream) {
		pb.tcpStream	= fStream;
		switch (fState) {
		case Connecting:
		case Connected:
			shutdown(2);
		}
		AddContext();
		while (fState > Unconnected) {
			size_t consume = 0x7F000000;
			fInputBuffer.Consume(nil, consume);

			GUSIContext::Yield(kGUSIBlock);
		}
		RemoveContext();
		
		if (PBControlSync(ParmBlkPtr(&pb)))
			return;

		DisposePtr(pb.csParam.create.rcvBuff); /* there is no release pb */
	}
}
// \section{Implementation of [[GUSIMTTcpFactory]]}                        
//                                                                         
//                                                                         
// <Member functions for class [[GUSIMTTcpFactory]]>=                      
GUSISocketFactory * GUSIMTTcpFactory::instance = nil;
// <Member functions for class [[GUSIMTTcpFactory]]>=                      
GUSISocket * GUSIMTTcpFactory::socket(int, int, int)
{
	return new GUSIMTTcpSocket();
}
// <Member functions for class [[GUSIMTTcpFactory]]>=                      
void GUSIwithMTTcpSockets()
{
	gGUSIInetFactories.AddFactory(SOCK_STREAM, 0, GUSIMTTcpFactory::Instance());
	GUSIMTNetDB::Instantiate();
}

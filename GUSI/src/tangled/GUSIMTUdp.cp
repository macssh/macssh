// <GUSIMTUdp.cp>=                                                         
#include "GUSIInternal.h"
#include "GUSIMTUdp.h"
#include "GUSIMTInet.h"
#include "GUSIMTNetDB.h"
#include "GUSIInet.h"
#include "GUSIDiag.h"
#include "GUSISocketMixins.h"

#include <errno.h>

#include <algorithm>

#include <Devices.h>

GUSI_USING_STD_NAMESPACE

// \section{Definition of [[GUSIMTUdpSocket]]}                             
//                                                                         
// [[GUSIMTUdpSocket]] have no interesting data of their own.              
//                                                                         
// <Definition of class [[GUSIMTUdpSocket]]>=                              
class GUSIMTUdpSocket : public GUSIMTInetSocket, public GUSISMProcess {
public:
	GUSIMTUdpSocket();
	~GUSIMTUdpSocket();
	// [[connect]] installs a peer address.                                    
 //                                                                         
 // <Overridden member functions for [[GUSIMTUdpSocket]]>=                  
 virtual int connect(void * address, socklen_t addrlen);
 // [[recvfrom]] reads from [[fInputBuffer]].                               
 //                                                                         
 // <Overridden member functions for [[GUSIMTUdpSocket]]>=                  
 virtual ssize_t recvfrom(const GUSIScatterer & buffer, int, void * from, socklen_t * fromlen);
 // [[sendto]] writes to [[fOutputBuffer]]. It is very important that the output buffer be locked 
 // between writing the header and writing the data.                        
 //                                                                         
 // <Overridden member functions for [[GUSIMTUdpSocket]]>=                  
 virtual ssize_t sendto(const GUSIGatherer & buffer, int flags, const void * to, socklen_t addrlen);
 // [[select]] checks for various conditions on the socket.                 
 //                                                                         
 // <Overridden member functions for [[GUSIMTUdpSocket]]>=                  
 virtual bool	select(bool * canRead, bool * canWrite, bool * exception);
 // [[shutdown]] for writing sends a closing notice. [[fOutputBuffer]] is locked and
 // released so the send procedure is called.                               
 //                                                                         
 // <Overridden member functions for [[GUSIMTUdpSocket]]>=                  
 virtual int shutdown(int how);
private:
	// \section{Implementation of [[GUSIMTUdpSocket]]}                         
 //                                                                         
 // The implementation of [[GUSIMTUdpSocket]] consists of a synchronous high level
 // part which mostly deals with [[GUSIRingBuffers]] and an asynchronous low
 // level part. We'll start with the low level procedures.                  
 //                                                                         
 // \subsection{Interrupt level routines for [[GUSIMTUdpSocket]]}           
 //                                                                         
 // Both [[GUSIMTUSendDone]] and [[GUSIMTURecvDone]] are always called with the 
 // same [[UDPiopb]] in a [[GUSIMTUdpSocket]] so they can easily find out the address 
 // of the socket itself. [[GUSIMTUSend]] and [[GUSIMTURecv]] set up send and receive 
 // calls.                                                                  
 //                                                                         
 // <Privatissima of [[GUSIMTUdpSocket]]>=                                  
 UDPiopb						fSendPB;
 MidiWDS						fSendWDS;
 UDPiopb						fRecvPB;
 friend void 				GUSIMTUSend(GUSIMTUdpSocket * sock);
 friend void 				GUSIMTURecv(GUSIMTUdpSocket * sock);
 friend void 				GUSIMTUSendDone(UDPiopb * pb);
 friend void 				GUSIMTURecvDone(UDPiopb * pb);
 static UDPIOCompletionUPP	sSendProc;
 static UDPIOCompletionUPP	sRecvProc;
 // [[CreateStream]] creates a UDP stream.                                  
 //                                                                         
 // <Privatissima of [[GUSIMTUdpSocket]]>=                                  
 int CreateStream();
};
// [[GUSIMTUSendDone]] does all its work between [[fSendPB]] and [[fOutputBuffer]].
// If a send fails, the whole send buffer is cleared.                      
//                                                                         
// <Interrupt level routines for [[GUSIMTUdpSocket]]>=                     
void GUSIMTUSendDone(UDPiopb * pb)
{
	GUSIMTUdpSocket * sock = 
		reinterpret_cast<GUSIMTUdpSocket *>((char *)pb-offsetof(GUSIMTUdpSocket, fSendPB));
	GUSIProcess::A5Saver saveA5(sock->Process());	
	if (sock->fOutputBuffer.Locked())
		sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTUSendDone), pb);
	else {
		sock->fOutputBuffer.ClearDefer();
		sock->fOutputBuffer.FreeBuffer(sock->fSendWDS.fDataPtr, sock->fSendWDS.fLength);
		switch (sock->fSendPB.ioResult) {
		case noErr:
			break; /* Everything ok */
		case insufficientResources:
			break; /* Queue overflow, discard packet but proceed */
		default:
			sock->SetAsyncMacError(sock->fSendPB.ioResult);
			for (long valid; valid = sock->fOutputBuffer.Valid(); )
				sock->fOutputBuffer.FreeBuffer(nil, valid);
			sock->fWriteShutdown 	= true;
		}
		GUSIMTUSend(sock);
		sock->Wakeup();
	}
}
// Since UDP consists of distinct packets, each packet is prefixed by a header when
// stored in a GUSIBuffer.                                                 
//                                                                         
// <Interrupt level routines for [[GUSIMTUdpSocket]]>=                     
struct GUSIUDPHeader {
	in_addr_t	fPeerAddr;
	in_port_t	fPeerPort;
	uint16_t	fLength;
};
// [[GUSIMTUSend]] starts an UDP send call if there is data to send and otherwise
// sets itself up as the deferred procedure of the output buffer (and thus is 
// guaranteed to be called the next time data is deposited in the buffer again).
// If all data has been delivered and a shutdown is requested, send one.   
//                                                                         
// To avoid race conditions, we insist that a valid request consist of at least 
// 1 byte of packet data.                                                  
//                                                                         
// <Interrupt level routines for [[GUSIMTUdpSocket]]>=                     
void GUSIMTUSend(GUSIMTUdpSocket * sock)
{
	sock->fOutputBuffer.ClearDefer();
	if (!sock->fOutputBuffer.Valid() || sock->fOutputBuffer.Locked()) {
		if (!sock->fWriteShutdown)
			sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTUSend), sock);
		else
			sock->fState = GUSISMState::Closing;
	} else {
		sock->fOutputBuffer.ClearDefer();
		
		GUSIUDPHeader	header;
		size_t			len = sizeof(GUSIUDPHeader);
		
		sock->fOutputBuffer.Consume(&header, len);
		
		// Since we have to preserve packet sizes and [[GUSIBuffer]] does not, we sometimes
  // might get the packet data in two pieces.                                
  //                                                                         
  // <Set up [[fSendWDS]]>=                                                  
  len						= header.fLength;
  sock->fSendWDS.fDataPtr	= 
  	static_cast<Ptr>(sock->fOutputBuffer.ConsumeBuffer(len));
  sock->fSendWDS.fLength	= (u_short) len;
  if (len < header.fLength) {
  	len 						= header.fLength - len;
  	sock->fSendWDS.fDataPtr2	= 
  		static_cast<Ptr>(sock->fOutputBuffer.ConsumeBuffer(len));
  	sock->fSendWDS.fLength2		= (u_short) len;
  } else 
  	sock->fSendWDS.fLength2		= 0;
		
		sock->fSendPB.ioCompletion				= sock->sSendProc;
		sock->fSendPB.csCode					= UDPWrite;
		sock->fSendPB.csParam.send.remoteHost	= header.fPeerAddr;
		sock->fSendPB.csParam.send.remotePort	= header.fPeerPort;
		sock->fSendPB.csParam.send.wdsPtr 		= &sock->fSendWDS;
		sock->fSendPB.csParam.send.checkSum 	= true;
		sock->fSendPB.csParam.send.sendLength 	= 0;
			
		PBControlAsync(ParmBlkPtr(&sock->fSendPB));		
	}
}
// [[GUSIMTURecvDone]] does all its work between [[fRecvPB]] and [[fInputBuffer]].
//                                                                         
// <Interrupt level routines for [[GUSIMTUdpSocket]]>=                     
void GUSIMTURecvDone(UDPiopb * pb)
{
	GUSIMTUdpSocket * sock = 
		reinterpret_cast<GUSIMTUdpSocket *>((char *)pb-offsetof(GUSIMTUdpSocket, fRecvPB));
	GUSIProcess::A5Saver saveA5(sock->Process());	
	if (sock->fInputBuffer.Locked())
		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTURecvDone), pb);
	else {
		sock->fInputBuffer.ClearDefer();
		switch (sock->fRecvPB.ioResult) {
		case noErr: 
			// If the packet is too big, or if the socket is bound and a packet arrives from
   // a different peer, we drop it. Whether we drop or not, we must release the buffers.
   // Since we reuse the same completion procedure, we also must check whether we 
   // arrived here on a read or on a buffer return.                           
   //                                                                         
   // <Check and store received UDP packet>=                                  
   if (sock->fRecvPB.csCode == UDPRead) {
   	long needed = sock->fRecvPB.csParam.receive.rcvBuffLen+8;
   	if (sock->fInputBuffer.Size() < needed)
   		; // Drop
   	else if (sock->fInputBuffer.Free() < needed) {
   		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMTURecvDone), pb);
   		return;
   	} else if (sock->fPeerAddr.sin_port &&
   		(  sock->fRecvPB.csParam.receive.remoteHost != sock->fPeerAddr.sin_addr.s_addr
   		|| sock->fRecvPB.csParam.receive.remotePort != sock->fPeerAddr.sin_port
   		)
   	) {
   		; // Drop
   	} else {
   		GUSIUDPHeader	header;
   		size_t			len    = sizeof(GUSIUDPHeader);
   		
   		header.fPeerAddr = sock->fRecvPB.csParam.receive.remoteHost;
   		header.fPeerPort = sock->fRecvPB.csParam.receive.remotePort;
   		header.fLength   = sock->fRecvPB.csParam.receive.rcvBuffLen;
   		
   		sock->fInputBuffer.Produce(&header, len);
   		len = header.fLength;
   		sock->fInputBuffer.Produce(
   			sock->fRecvPB.csParam.receive.rcvBuff, len);
   	}
   	sock->fRecvPB.csCode 		= UDPBfrReturn;
   	PBControlAsync(ParmBlkPtr(&sock->fRecvPB));
   	
   	return;
   }
			// Fall through
		case commandTimeout:
			GUSIMTURecv(sock);
			break;
		default:
			sock->SetAsyncMacError(sock->fRecvPB.ioResult);
			sock->fReadShutdown = true;
			break;
		}
		sock->Wakeup();
	}
}
// [[GUSIMTURecv]] starts an UDP receive call. No free space is necessary in 
// the buffer.                                                             
//                                                                         
// <Interrupt level routines for [[GUSIMTUdpSocket]]>=                     
void GUSIMTURecv(GUSIMTUdpSocket * sock)
{
	sock->fRecvPB.ioCompletion						= sock->sRecvProc;
	sock->fRecvPB.csCode 							= UDPRead;
	sock->fRecvPB.csParam.receive.timeOut			= 0; 
	sock->fRecvPB.csParam.receive.secondTimeStamp	= 0; 
		
	PBControlAsync(ParmBlkPtr(&sock->fRecvPB));		
}
// The UPPs for the completion procedures are set up the first time a socket
// is constructed.                                                         
//                                                                         
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
UDPIOCompletionUPP	GUSIMTUdpSocket::sSendProc	= 0;
UDPIOCompletionUPP	GUSIMTUdpSocket::sRecvProc	= 0;
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
int GUSIMTUdpSocket::CreateStream()
{
	fSendPB.ioCompletion				= nil;
	fSendPB.csCode 						= UDPCreate;
	fSendPB.csParam.create.rcvBuff 		= (char *)NewPtr(4096);
	fSendPB.csParam.create.rcvBuffLen 	= 4096;
	fSendPB.csParam.create.notifyProc 	= nil;
	fSendPB.csParam.create.userDataPtr	= nil;
	fSendPB.csParam.create.localPort	= fSockAddr.sin_port;
	
	PBControlSync(ParmBlkPtr(&fSendPB));
	
	if (fSendPB.ioResult)
		return GUSISetMacError(fSendPB.ioResult);
	
	fState				= Connected;
	fStream				= fRecvPB.udpStream	= fSendPB.udpStream;
	fSockAddr.sin_port	= fSendPB.csParam.create.localPort;
	
	GUSIMTUSend(this);
	GUSIMTURecv(this);
	
	return 0;
}
// \subsection{High level interface for [[GUSIMTUdpSocket]]}               
//                                                                         
// The constructor has to initialize a rather large number of data fields, and as a 
// side effect opens the MacUDP driver if necessary. No other interesting activity
// occurs.                                                                 
//                                                                         
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
GUSIMTUdpSocket::GUSIMTUdpSocket()
{
	// <Initialize fields of [[GUSIMTUdpSocket]]>=                             
 if (!sSendProc)
 	sSendProc = NewUDPIOCompletionProc(GUSIMTUSendDone);
 if (!sRecvProc)
 	sRecvProc = NewUDPIOCompletionProc(GUSIMTURecvDone);
 // The send and receive parameter blocks are highly specialized and never really
 // change during the existence of a socket.                                
 //                                                                         
 // <Initialize fields of [[GUSIMTUdpSocket]]>=                             
 fSendPB.ioCRefNum 							= GUSIMTInetSocket::Driver();
 fRecvPB.ioCRefNum 							= GUSIMTInetSocket::Driver();
}
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
int GUSIMTUdpSocket::connect(void * address, socklen_t addrlen)
{
	sockaddr_in *	addr 	= (sockaddr_in *) address;

	// <Sanity checks for [[GUSIMTUdpSocket::connect]]>=                       
 if (!GUSI_CASSERT_CLIENT(addrlen >= int(sizeof(sockaddr_in))))
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_CASSERT_CLIENT(addr->sin_family == AF_INET)) {
 	memset(&fPeerAddr, 0, sizeof(sockaddr_in));
 	return GUSISetPosixError(EAFNOSUPPORT);
 }
	// <Create UDP stream if necessary>=                                       
 if (!fStream && CreateStream())
 	return -1;
		
	fPeerAddr = *addr;
	
	return 0;
}
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
ssize_t GUSIMTUdpSocket::recvfrom(
	const GUSIScatterer & buffer, int flags, void * from, socklen_t * fromlen)
{
	// <Sanity checks for [[GUSIMTUdpSocket::recvfrom]]>=                      
 if (!fInputBuffer.Valid())
 	if (GUSISetPosixError(GetAsyncError()))
 		return -1;
 	else if (fReadShutdown)
 		return 0;
 if (!GUSI_CASSERT_CLIENT(!from || *fromlen >= int(sizeof(sockaddr_in))))
 	return GUSISetPosixError(EINVAL);
	// <Create UDP stream if necessary>=                                       
 if (!fStream && CreateStream())
 	return -1;
	// The input buffer needs to be nonempty before a read can succeed.        
 //                                                                         
 // <Wait for valid data on [[GUSIMTUdpSocket]]>=                           
 if (!fReadShutdown && !fInputBuffer.Valid()) {
 	if (!fBlocking)
 		return GUSISetPosixError(EWOULDBLOCK);
 	bool signal = false;
 	AddContext();
 	while (!fReadShutdown && !fInputBuffer.Valid())
 		if (signal = GUSIContext::Yield(kGUSIBlock))
 			break;
 	RemoveContext();
 	if (signal)
 		return GUSISetPosixError(EINTR);
 }

	GUSIUDPHeader	header;
	size_t			len = sizeof(GUSIUDPHeader);

	if (flags & MSG_PEEK) {
		GUSIRingBuffer::Peeker peeker(fInputBuffer);
		
		peeker.Peek(&header, len);
		len = min(buffer.Length(), (int) header.fLength);
		peeker.Peek(buffer, len);
	} else {
		fInputBuffer.Consume(&header, len);

		len = min(buffer.Length(), (int)header.fLength);
		fInputBuffer.Consume(buffer, len);
	}
	
	if (from) {
		sockaddr_in *	addr 	= (sockaddr_in *) from;
		*fromlen				= sizeof(sockaddr_in);
		addr->sin_family		= AF_INET;
		addr->sin_addr.s_addr 	= header.fPeerAddr;
		addr->sin_port			= header.fPeerPort;
	}
	
	return (int)len;
}
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
ssize_t GUSIMTUdpSocket::sendto(
	const GUSIGatherer & buffer, int flags, const void * to, socklen_t addrlen)
{
	// <Sanity checks for [[GUSIMTUdpSocket::sendto]]>=                        
 if (GUSISetPosixError(GetAsyncError()))
 	return -1;
 if (!GUSI_CASSERT_CLIENT(!to || addrlen >= int(sizeof(sockaddr_in))))
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_CASSERT_CLIENT(buffer.Length() < 0x10000))
 	return GUSISetPosixError(EINVAL);
	// <Create UDP stream if necessary>=                                       
 if (!fStream && CreateStream())
 	return -1;
	// <Wait for free buffer space on [[GUSIMTUdpSocket]]>=                    
 size_t needed = buffer.Length()+8;
 if (!fBlocking && fOutputBuffer.Free() < needed)
 	return GUSISetPosixError(EWOULDBLOCK);
 if (!fWriteShutdown && fOutputBuffer.Free() < needed) {
 	if (fOutputBuffer.Size() < needed)
 		return GUSISetPosixError(EINVAL);
 	bool signal = false;
 	AddContext();
 	while (!fWriteShutdown && fOutputBuffer.Free() < needed)
 		if (signal = GUSIContext::Yield(kGUSIBlock))
 			break;
 	RemoveContext();
 	if (signal)
 		return GUSISetPosixError(EINTR);
 }
 if (fWriteShutdown)
 	return GUSISetPosixError(ESHUTDOWN);
	
	GUSIUDPHeader header;
	if (fPeerAddr.sin_port) {
		header.fPeerAddr = fPeerAddr.sin_addr.s_addr;
		header.fPeerPort = fPeerAddr.sin_port;
	} else if (to) {
		header.fPeerAddr = static_cast<const sockaddr_in *>(to)->sin_addr.s_addr;
		header.fPeerPort = static_cast<const sockaddr_in *>(to)->sin_port;
	} else
		return GUSISetPosixError(ENOTCONN);
	header.fLength = (uint16_t)buffer.Length();
	
	fOutputBuffer.Lock();
	size_t len    = sizeof(GUSIUDPHeader);
	fOutputBuffer.Produce(&header, len);
	len = buffer.Length();
	size_t offset = 0;
	fOutputBuffer.Produce(buffer, len, offset);
	fOutputBuffer.Release();
	
	return len;
}
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
bool GUSIMTUdpSocket::select(bool * canRead, bool * canWrite, bool *)
{
	// <Create UDP stream if necessary>=                                       
 if (!fStream && CreateStream())
 	return -1;
	
	bool cond = false;
	if (canRead)
		if (*canRead = fReadShutdown || fAsyncError || fInputBuffer.Valid() > 0)
			cond = true;
	if (canWrite)
		if (*canWrite = fWriteShutdown || fAsyncError || fOutputBuffer.Free()>1600)
			cond = true;
		
	return cond;
}
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
int GUSIMTUdpSocket::shutdown(int how)
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
// <Member functions for class [[GUSIMTUdpSocket]]>=                       
GUSIMTUdpSocket::~GUSIMTUdpSocket()
{
	if (fStream) {
		UDPiopb pb;
		
		pb.ioCRefNum	= GUSIMTInetSocket::Driver();
		pb.csCode 		= UDPRelease;
		pb.udpStream	= fStream;
	
		if (fState != Closing) {
			shutdown(2);
		
			AddContext();
			while (fState != Closing)
				GUSIContext::Yield(kGUSIBlock);
			RemoveContext();
		}
		
		if (PBControlSync(ParmBlkPtr(&pb)))
			return;

		DisposePtr(pb.csParam.create.rcvBuff); /* there is no release pb */
	}
}
// \section{Implementation of [[GUSIMTUdpFactory]]}                        
//                                                                         
//                                                                         
// <Member functions for class [[GUSIMTUdpFactory]]>=                      
GUSISocketFactory * GUSIMTUdpFactory::instance = nil;
// <Member functions for class [[GUSIMTUdpFactory]]>=                      
GUSISocket * GUSIMTUdpFactory::socket(int, int, int)
{
	return new GUSIMTUdpSocket();
}
// <Member functions for class [[GUSIMTUdpFactory]]>=                      
void GUSIwithMTUdpSockets()
{
	gGUSIInetFactories.AddFactory(SOCK_DGRAM, 0, GUSIMTUdpFactory::Instance());
	GUSIMTNetDB::Instantiate();
}

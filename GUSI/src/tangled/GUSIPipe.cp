// <GUSIPipe.cp>=                                                          
#include "GUSIInternal.h"
#include "GUSIPipe.h"
#include "GUSIBasics.h"
#include "GUSIBuffer.h"

#include <errno.h>

// \section{Definition of [[GUSIPipeSocket]]}                              
//                                                                         
// A [[GUSIPipeSocket]] is implemented with a simple [[GUSIBuffer]].       
//                                                                         
// <Definition of class [[GUSIPipeSocket]]>=                               
class GUSIPipeSocket : public GUSISocket {
public:
	GUSIPipeSocket();
	virtual ~GUSIPipeSocket();
	// Currently we only implement the simple calls.                           
 //                                                                         
 // <Overridden member functions for [[GUSIPipeSocket]]>=                   
 virtual bool Supports(ConfigOption config);
 // <Overridden member functions for [[GUSIPipeSocket]]>=                   
 virtual ssize_t read(const GUSIScatterer & buffer);
 // <Overridden member functions for [[GUSIPipeSocket]]>=                   
 virtual ssize_t	write(const GUSIGatherer & buffer);
 // Of course, we also have [[select]]. [[canWrite]] will be sort of unsatisfactory,
 // since it says nothing about the size of [[write]] possible.             
 //                                                                         
 // <Overridden member functions for [[GUSIPipeSocket]]>=                   
 virtual bool	select(bool * canRead, bool * canWrite, bool * exception);
 // Some similar functions are necessary for [[shutdown]].                  
 //                                                                         
 // <Overridden member functions for [[GUSIPipeSocket]]>=                   
 virtual int	shutdown(int how);
	// Each [[GUSIPipeSocket]] has a peer which is set with [[SetPeer]].       
 //                                                                         
 // <Peer management for [[GUSIPipeSocket]]>=                               
 void	SetPeer(GUSIPipeSocket * peer);
protected:
	// \section{Implementation of [[GUSIPipeSocket]]}                          
 //                                                                         
 //                                                                         
 // <Privatissima of [[GUSIPipeSocket]]>=                                   
 GUSIRingBuffer 		fBuffer;
 bool				fWriteShutdown;
 bool				fBlocking;
 GUSIPipeSocket *	fPeer;
 // <Privatissima of [[GUSIPipeSocket]]>=                                   
 void WakeupPeer();
 // Since there is currently no data, the simplest strategy won't work and  
 // we need something a bit more elaborate.                                 
 //                                                                         
 // <Privatissima of [[GUSIPipeSocket]]>=                                   
 bool Eof()	{  return !fPeer || fPeer->fWriteShutdown;	}
 // <Privatissima of [[GUSIPipeSocket]]>=                                   
 bool Shutdown()	{  return !fPeer || fWriteShutdown;	}
};
// We don't have much to do with peers except setting them and waking them up.
//                                                                         
// <Inline member functions for class [[GUSIPipeSocket]]>=                 
inline void	GUSIPipeSocket::SetPeer(GUSIPipeSocket * peer)  { fPeer = peer; }
// \section{Implementation of [[GUSIPipeFactory]]}                         
//                                                                         
// Normally, you use [[GUSIPipeFactory]] only via [[pipe]], in which case you don't
// need to initialize anything. If you want to use it via [[socketpair]], however,
// you need to call [[GUSIwithLocalSockets]] from your [[GUSISetupFactories]] routine.
//                                                                         
// <Member functions for class [[GUSIPipeFactory]]>=                       
extern "C" void GUSIwithLocalSockets()
{
	GUSISocketDomainRegistry::Instance()->AddFactory(AF_LOCAL, GUSIPipeFactory::Instance());
}
// <Member functions for class [[GUSIPipeFactory]]>=                       
GUSISocketFactory * GUSIPipeFactory::sInstance = nil;
// [[GUSIPipeFactory]] is odd in that [[socket]] is not meaningful.        
//                                                                         
// <Member functions for class [[GUSIPipeFactory]]>=                       
GUSISocket * GUSIPipeFactory::socket(int, int, int)
{
	return GUSISetPosixError(EOPNOTSUPP), static_cast<GUSISocket *>(nil);
}
// [[socketpair]] is meaningful, however.                                  
//                                                                         
// <Member functions for class [[GUSIPipeFactory]]>=                       
int GUSIPipeFactory::socketpair(int, int, int, GUSISocket * s[2])
{
	GUSIErrorSaver		saveError;
	GUSIPipeSocket *	sock[2];
	
	if (s[0] = sock[0] = new GUSIPipeSocket)
		if (s[1] = sock[1] = new GUSIPipeSocket) {
			sock[0]->SetPeer(sock[1]);
			sock[1]->SetPeer(sock[0]);
			
			return 0;
		} else
			s[0]->close();
	
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
GUSIPipeSocket::GUSIPipeSocket()
 : fBuffer(8192), fWriteShutdown(false), fBlocking(true), fPeer(nil)
{
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
void GUSIPipeSocket::WakeupPeer()
{
	if (fPeer)
		fPeer->Wakeup();
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
bool GUSIPipeSocket::Supports(ConfigOption config)
{
	return config == kSimpleCalls;
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
ssize_t GUSIPipeSocket::read(const GUSIScatterer & buffer)
{
	size_t	len 	= buffer.Length();
	
	if (!fBuffer.Valid()) {
		// <No more data in [[GUSIPipeSocket]], check for EOF and consider waiting>=
  if (Eof())
  	return 0;
  if (!fBlocking)
  	return GUSISetPosixError(EWOULDBLOCK);
  bool signal = false;
  AddContext();
  do {
  	if (signal = GUSIContext::Yield(kGUSIBlock))
  		break;
  } while (!Eof() && !fBuffer.Valid());
  RemoveContext();
  if (signal)
  	return GUSISetPosixError(EINTR);
	}
	fBuffer.Consume(buffer, len);
	WakeupPeer();
	
	return (int) len;
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
ssize_t GUSIPipeSocket::write(const GUSIGatherer & buffer)
{
	size_t buflen = buffer.Length();
	size_t len;
	size_t offset = 0;
	
restart:
	if (Shutdown())
		return GUSISetPosixError(EPIPE);
	if (fPeer->fBuffer.Free() < buflen) {
		// This could get painful. We write a portion and then wait for free buffer space.
  //                                                                         
  // <Too much data in [[GUSIPipeSocket]], consider writing in portions>=    
  if (!fBlocking && !fPeer->fBuffer.Free())
  	return offset ? (int) offset : GUSISetPosixError(EWOULDBLOCK);
  len = buflen;
  fPeer->fBuffer.Produce(buffer, len, offset);
  buflen -= len;
  WakeupPeer();
  bool signal = false;
  AddContext();
  while (!Shutdown() && !fPeer->fBuffer.Free()) {
  	if (signal = GUSIContext::Yield(kGUSIBlock))
  		break;
  }
  RemoveContext();
  if (signal)
  	return offset ? (int) offset : GUSISetPosixError(EINTR);
  goto restart;
	}
	fPeer->fBuffer.Produce(buffer, buflen, offset);
	WakeupPeer();
	
	return (size_t) buffer.Length();
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
bool GUSIPipeSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		if (*canRead = Eof() || fBuffer.Valid())
			cond = true;
	if (canWrite)
		if (*canWrite = Shutdown() || fPeer->fBuffer.Free())
			cond = true;
		
	return cond;
}
// When we're destroyed, we sever the links to our peer.                   
//                                                                         
// <Member functions for class [[GUSIPipeSocket]]>=                        
GUSIPipeSocket::~GUSIPipeSocket()
{
	if (fPeer)
		fPeer->fPeer = nil;
	WakeupPeer();
}
// <Member functions for class [[GUSIPipeSocket]]>=                        
int GUSIPipeSocket::shutdown(int how)
{
	if (how) 						// write
		fWriteShutdown = true;
	if (!(how & 1))				// read
		if (fPeer)
			fPeer->fWriteShutdown = true;
	
	return 0;	
}

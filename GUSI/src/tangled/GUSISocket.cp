// <GUSISocket.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSISocket.h"
#include "GUSIDiag.h"
#include "GUSIBuffer.h"

#include <errno.h>
#include <sys/stat.h>
 
// \subsection{I/O Operations}                                             
//                                                                         
// As mentioned above, there are three variants of each call. If one of them
// is not implemented, the default implementation tries to emulate it with 
// first another call.                                                     
//                                                                         
// To simplify filling in a [[msghdr]], we provide an extension class with a
// convenient constructor.                                                 
//                                                                         
// <Auxiliary data structures for class [[GUSISocket]]>=                   
struct GUSImsghdr : public msghdr {
	GUSImsghdr(const GUSIScattGath & buffer, const void * addr = nil, socklen_t addrlen = 0);
};
// The constructor of [[GUSImsghdr]] translates a simple buffer into an [[iovec]].
//                                                                         
// <Auxiliary data structures for class [[GUSISocket]]>=                   
GUSImsghdr::GUSImsghdr(const GUSIScattGath & buffer, const void * addr, socklen_t addrlen)
{
	msg_name		=	static_cast<char *>(const_cast<void *>(addr));
	msg_namelen		=	addrlen;
	msg_iov			=	const_cast<iovec *>(buffer.IOVec());
	msg_iovlen		=	buffer.Count();
	msg_control		=	nil;
	msg_controllen	=	0;
}
// <Member functions for class [[GUSISocket]]>=                            
GUSISocket::GUSISocket()
 : fRefCount(0), fQueue(0), fNextSocket(0), fPrevSocket(0)
{
	GUSIContext::Setup(false);
}

bool GUSISocket::Supports(ConfigOption)
{
	return false;
}
// <Member functions for class [[GUSISocket]]>=                            
void GUSISocket::Enqueue(GUSISocket ** queue)
{
	GUSI_MESSAGE(("GUSISocket::Enqueue %08x\n", this));
	Dequeue();
	fQueue		=	queue;
	fPrevSocket	=	0;
	fNextSocket	=	*queue;
	*queue		=	this;
	if (fNextSocket)
		fNextSocket->fPrevSocket = this;
}

void GUSISocket::Dequeue()
{
	if (!fQueue)
		return;
	GUSI_MESSAGE(("GUSISocket::Dequeue %08x\n", this));
	if (fPrevSocket)
		fPrevSocket->fNextSocket	=	fNextSocket;
	else
		*fQueue						=	fNextSocket;
	if (fNextSocket)
		fNextSocket->fPrevSocket	=	fPrevSocket;
}
// The closing mechanism is quite intricate. [[close]] is typically overridden and
// eventually calls through to either this implementation, or to [[Enqueue]].
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
void GUSISocket::close()
{
	delete this;
}

void GUSISocket::CheckClose(UInt32 now)
{
	GUSISocket *	next	= fNextSocket;
	
	Close(now);
	if (next)
		next->CheckClose(now);
}

bool GUSISocket::Close(UInt32)
{
	delete this;
	
	return true;
}

GUSISocket::~GUSISocket()
{
	Wakeup();
	Dequeue();
}
// <Member functions for class [[GUSISocket]]>=                            
void GUSISocket::AddContext(GUSIContext * context)
{
	fContexts.push_front(context ? context : GUSIContext::Current());
}

void GUSISocket::RemoveContext(GUSIContext * context)
{
	fContexts.remove(context ? context : GUSIContext::Current());
}
// <Member functions for class [[GUSISocket]]>=                            
int	GUSISocket::bind(void *, socklen_t)			
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::getsockname(void *, socklen_t *)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::getpeername(void *, socklen_t *)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::listen(int)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::connect(void *, socklen_t)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::getsockopt(int, int, void *, socklen_t *)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::setsockopt(int, int, void *, socklen_t )
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int	GUSISocket::fcntl(int, va_list)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int	GUSISocket::ioctl(unsigned int, va_list)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::ftruncate(off_t)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
int GUSISocket::shutdown(int)
	// \subsection{Operations without plausible default implementations}       
 //                                                                         
 // Many socket operations by default simply return [[EOPNOTSUPP]].         
 //                                                                         
 // <No default implementation, return [[EOPNOTSUPP]]>=                     
 {
 	return GUSISetPosixError(EOPNOTSUPP);
 }
// [[accept]] behaves similarly to the above, but returns a [[NULL]] pointer,
// not [[-1]].                                                             
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
GUSISocket * GUSISocket::accept(void *, socklen_t *)
{
	return GUSISetPosixError(EOPNOTSUPP), static_cast<GUSISocket *>(nil);
}
// [[lseek]] should, according to the POSIX standard, return [[ESPIPE]]    
// rather than [[EOPNOTSUPP]].                                             
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
off_t GUSISocket::lseek(off_t, int)
{
	return GUSISetPosixError(ESPIPE), -1;
}
// [[fsync]] returns [[EINVAL]] as in BSD.                                 
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
int GUSISocket::fsync()
{
	return GUSISetPosixError(EINVAL);
}
// \subsection{Operations with plausible default implementations}          
//                                                                         
// By default, we assume that a socket is not a console.                   
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
int	GUSISocket::isatty()
{
	return 0;
}
// [[pre_select]] and [[post_select]] often don't have to do anything. By default,
// we assume that a socket can sleep.                                      
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
bool GUSISocket::pre_select(bool, bool, bool)
{
	AddContext();
	
	return true;
}

void GUSISocket::post_select(bool, bool, bool)
{
	RemoveContext();
}
// [[select]] by default reports that nothing happens (although a socket like this
// is useless for inclusion in a [[select]] statement.                     
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
bool GUSISocket::select(bool *, bool *, bool *)
{	
	return false;
}
// [[read]] tries the more complex calls [[recvfrom]] and [[recvmsg]]. A socket
// family which supports none of the three calls must be very strange indeed. To
// keep implementors honest, an [[assert]] verifies that they don't claim support
// of a call variant and then dispatch to the default implementation anyway.
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
ssize_t	GUSISocket::read(const GUSIScatterer & buffer)
{
	GUSI_CASSERT_INTERNAL(!Supports(kSimpleCalls));
	
	if (Supports(kSocketCalls)) {
		socklen_t	fromlen = 0;
		
		return recvfrom(buffer, 0, nil, &fromlen);
	} else if (Supports(kMsgCalls)) {
		GUSImsghdr	msg(buffer);
		
		return recvmsg(&msg, 0);
	} else
		return GUSISetPosixError(EOPNOTSUPP);
}
// [[recvfrom]] can always be translated to [[recvmsg]] and sometimes to   
// [[read]].                                                               
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
ssize_t GUSISocket::recvfrom(
	const GUSIScatterer & buffer, int flags, void * from, socklen_t * fromlen)
{
	GUSI_CASSERT_INTERNAL(!Supports(kSocketCalls));
	
	if (!flags && !from && Supports(kSimpleCalls))
		return read(buffer);
	else if (Supports(kMsgCalls)) {
		GUSImsghdr	msg(buffer, from, *fromlen);
		
		int result 	= recvmsg(&msg, flags);
		*fromlen 	= msg.msg_namelen;
		
		return result;
	} else
		return GUSISetPosixError(EOPNOTSUPP);
}
// [[recvmsg]] can always (in GUSI) be translated to [[recvfrom]] and sometimes 
// to [[read]].                                                            
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
ssize_t	GUSISocket::recvmsg(msghdr * msg, int flags)
{
	GUSI_CASSERT_INTERNAL(!Supports(kMsgCalls));
	
	if (!flags && !msg->msg_name && Supports(kSimpleCalls)) {
		GUSIScatterer	scatter(msg->msg_iov, msg->msg_iovlen);
		
		return scatter.SetLength(read(scatter));
	} else if (Supports(kSocketCalls)) {
		GUSIScatterer	scatter(msg->msg_iov, msg->msg_iovlen);

		return scatter.SetLength(
			recvfrom(scatter, flags, msg->msg_name, &msg->msg_namelen));
	} else
		return GUSISetPosixError(EOPNOTSUPP);	
}
// <Member functions for class [[GUSISocket]]>=                            
ssize_t GUSISocket::write(const GUSIGatherer & buffer)
{
	GUSI_CASSERT_INTERNAL(!Supports(kSimpleCalls));
	
	if (Supports(kSocketCalls))
		return sendto(buffer, 0, nil, 0);
	else if (Supports(kMsgCalls)) {
		GUSImsghdr	msg(buffer);
		
		return sendmsg(&msg, 0);
	} else
		return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSISocket]]>=                            
ssize_t GUSISocket::sendto(
	const GUSIGatherer & buffer, int flags, const void * to, socklen_t tolen)
{
	GUSI_CASSERT_INTERNAL(!Supports(kSocketCalls));
	
	if (!flags && !to && Supports(kSimpleCalls))
		return write(buffer);
	else if (Supports(kMsgCalls)) {
		GUSImsghdr	msg(buffer, to, tolen);
		
		return sendmsg(&msg, flags);
	} else
		return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSISocket]]>=                            
ssize_t GUSISocket::sendmsg(const msghdr * msg, int flags)
{
	GUSI_CASSERT_INTERNAL(!Supports(kMsgCalls));
	
	if (!flags && !msg->msg_name && Supports(kSimpleCalls)) {
		GUSIGatherer	gather(msg->msg_iov, msg->msg_iovlen);
		
		return write(gather);
	} else if (Supports(kSocketCalls)) {
		GUSIGatherer	gather(msg->msg_iov, msg->msg_iovlen);

		return sendto(gather, flags, msg->msg_name, msg->msg_namelen);
	} else
		return GUSISetPosixError(EOPNOTSUPP);	
}
// [[fstat]] actually has a quite reasonable default.                      
//                                                                         
// <Member functions for class [[GUSISocket]]>=                            
int	GUSISocket::fstat(struct stat * buf)
{
	buf->st_dev			=	0;
	buf->st_ino			=	0;
	buf->st_mode		=	S_IFSOCK | 0666 ;
	buf->st_nlink		=	1;
	buf->st_uid			=	0;
	buf->st_gid			=	0;
	buf->st_rdev		=	0;
	buf->st_size		=	1;
	buf->st_atime		=	time(NULL);
	buf->st_mtime		=	time(NULL);
	buf->st_ctime		=	time(NULL);
	buf->st_blksize		=	1;
	buf->st_blocks		=	1;
	
	return 0;
}

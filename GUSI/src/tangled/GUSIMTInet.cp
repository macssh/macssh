// <GUSIMTInet.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSIMTInet.h"
#include "GUSIMTTcp.h"
#include "GUSIMTUdp.h"
#include "GUSIDiag.h"
#include "GUSIFSWrappers.h"

#include <stdlib.h>
#include <errno.h>
#include <algorithm>

#include <Devices.h>

GUSI_USING_STD_NAMESPACE

// <Member functions for class [[GUSIMTInetSocket]]>=                      
short 	GUSIMTInetSocket::sDrvrRefNum		=	0;
OSErr 	GUSIMTInetSocket::sDrvrState		=	1;
u_long	GUSIMTInetSocket::sHostAddress		=	0;
// [[Driver]] opens the driver if necessary and stores its refnum.         
//                                                                         
// <Member functions for class [[GUSIMTInetSocket]]>=                      
short GUSIMTInetSocket::Driver()
{
	if (sDrvrState == 1) 
		sDrvrState = GUSIFSOpenDriver("\p.IPP", &sDrvrRefNum);

	return sDrvrState ? 0 : sDrvrRefNum;
}
// [[HostAddr]] does an [[ipctlGetAddr]] control call if necessary.        
//                                                                         
// <Member functions for class [[GUSIMTInetSocket]]>=                      
u_long GUSIMTInetSocket::HostAddr()
{
	if (!sHostAddress && Driver()) {
		GUSIIOPBWrapper<GetAddrParamBlock>	ga;
		
		ga->ioCRefNum 	= Driver();
		ga->csCode 		= ipctlGetAddr;
		
		if (!ga.Control())
			sHostAddress = ga->ourAddress;
	}
	return sHostAddress;
}
// Initial values should be fairly obvious.                                
//                                                                         
// <Member functions for class [[GUSIMTInetSocket]]>=                      
GUSIMTInetSocket::GUSIMTInetSocket()
	: fStream(nil)
{
	memset(&fSockAddr, 0, sizeof(sockaddr_in));
	fSockAddr.sin_family	= AF_INET;
	
	fPeerAddr = fSockAddr;
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::bind(void * addr, socklen_t namelen)
{
	struct sockaddr_in *name	=	(struct sockaddr_in *)addr;
	// The address to be passed must be up to a minimal standard of decency.   
 // For instance, the host address must be either the real IP number of     
 // our host or one of the two legitimate pseudo-addresses for              
 // [["localhost"]].                                                        
 //                                                                         
 // <Sanity checks for [[GUSIMTInetSocket::bind]]>=                         
 if (!GUSI_ASSERT_CLIENT(
 		namelen >= sizeof(struct sockaddr_in),
 		("bind: address len %d < %d\n", namelen, sizeof(struct sockaddr_in)))
 )
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_ASSERT_CLIENT(
 		name->sin_family == AF_INET,
 		("bind: family %d != %d\n", name->sin_family, AF_INET))
 )
 	return GUSISetPosixError(EAFNOSUPPORT);
 if (!GUSI_SASSERT_CLIENT(!fSockAddr.sin_port, "bind: Socket already bound\n"))
 	return GUSISetPosixError(EINVAL);
 switch (name->sin_addr.s_addr) {
 default:
 	if (!GUSI_ASSERT_CLIENT(
 			name->sin_addr.s_addr == HostAddr(),
 			("bind: addr %08X != %08X\n", name->sin_addr.s_addr, HostAddr()))
 	)
 		return GUSISetPosixError(EADDRNOTAVAIL);
 case 0:
 case 0x7F000001:
 	break;
 }
	fSockAddr.sin_addr.s_addr 	= name->sin_addr.s_addr;
	fSockAddr.sin_port 			= name->sin_port;
	
	return 0;
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::getsockname(void *name, socklen_t *namelen)
{
	if (!GUSI_SASSERT_CLIENT(*namelen >= 0, "getsockname: passed negative length\n"))
		return GUSISetPosixError(EINVAL);

	memcpy(name, &fSockAddr, *namelen = min(*namelen, socklen_t(sizeof(sockaddr_in))));
	return 0;
}
int GUSIMTInetSocket::getpeername(void *name, socklen_t *namelen)
{
	if (!GUSI_SASSERT_CLIENT(*namelen >= 0, "getpeername: passed negative length\n"))
		return GUSISetPosixError(EINVAL);

	memcpy(name, &fPeerAddr, *namelen = min(*namelen, socklen_t(sizeof(sockaddr_in))));
	return 0;
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::shutdown(int how)
{
	if (!GUSI_SASSERT_CLIENT(how >= 0 && how < 3, "shutdown: 0,1, or 2\n"))
		return GUSISetPosixError(EINVAL);
		
	GUSISMState::Shutdown(how);
	
	return 0;
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::fcntl(int cmd, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoFcntl(&result, cmd, arg))
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("fcntl: illegal request %d\n", cmd));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::ioctl(unsigned int request, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoIoctl(&result, request, arg)
	 || GUSISMInputBuffer::DoIoctl(&result, request, arg)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("ioctl: illegal request %d\n", request));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::getsockopt(int level, int optname, void *optval, socklen_t * optlen)
{
	int	result;
	
	if (GUSISMInputBuffer::DoGetSockOpt(&result, level, optname, optval, optlen)
	 || GUSISMOutputBuffer::DoGetSockOpt(&result, level, optname, optval, optlen)
	 || GUSISMAsyncError::DoGetSockOpt(&result, level, optname, optval, optlen)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("getsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
int GUSIMTInetSocket::setsockopt(int level, int optname, void *optval, socklen_t optlen)
{
	int	result;
	
	if (GUSISMInputBuffer::DoSetSockOpt(&result, level, optname, optval, optlen)
	 || GUSISMOutputBuffer::DoSetSockOpt(&result, level, optname, optval, optlen)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("setsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
bool GUSIMTInetSocket::Supports(ConfigOption config)
{
	return config == kSocketCalls;
}
// <Member functions for class [[GUSIMTInetSocket]]>=                      
void GUSIwithMTInetSockets()
{
	GUSIwithMTTcpSockets();
	GUSIwithMTUdpSockets();
}

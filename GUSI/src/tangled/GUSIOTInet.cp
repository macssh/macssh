// <GUSIOTInet.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSIOTInet.h"
#include "GUSIOpenTransport.h"
#include "GUSIOTNetDB.h"
#include "GUSIInet.h"
#include "GUSIDiag.h"

#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <net/if.h>

// \section{Definition of [[GUSIOTTcpFactory]]}                            
//                                                                         
// The only way that the [[GUSIOTTcpFactory]] differs from the [[GUSIOTFactory]]
// is that it defines a strategy and an instantiation mechanism.           
//                                                                         
// <Definition of class [[GUSIOTTcpFactory]]>=                             
class GUSIOTTcpFactory : public GUSIOTStreamFactory {
public:
	static GUSISocketFactory * Instance();
protected:
	GUSIOTTcpFactory()				{}
	
	GUSISocket * socket(int domain, int type, int protocol);
	virtual GUSIOTStrategy *Strategy(int domain, int type, int protocol);
private:
	// \section{Implementation of [[GUSIOTTcpFactory]]}                        
 //                                                                         
 //                                                                         
 // <Privatissima of [[GUSIOTTcpFactory]]>=                                 
 static GUSISocketFactory *	sInstance;
};
// \section{Definition of [[GUSIOTUdpFactory]]}                            
//                                                                         
// The [[GUSIOTUdpFactory]] is the datagram equivalent of [[GUSIOTTcpFactory]].
// It returns its own socket typ to support multicasting.                  
//                                                                         
// <Definition of class [[GUSIOTUdpFactory]]>=                             
class GUSIOTUdpFactory : public GUSIOTDatagramFactory {
public:
	static GUSISocketFactory * Instance();
protected:
	GUSIOTUdpFactory()				{}
	
	GUSISocket * socket(int domain, int type, int protocol);
	virtual GUSIOTStrategy *Strategy(int domain, int type, int protocol);
private:
	// \section{Implementation of [[GUSIOTUdpFactory]]}                        
 //                                                                         
 //                                                                         
 // <Privatissima of [[GUSIOTUdpFactory]]>=                                 
 static GUSISocketFactory *	sInstance;
};
// \section{Definition of [[GUSIOTInetStrategy]]}                          
//                                                                         
// TCP and UDP sockets use different creation configurations, but the same 
// address packing routines.                                               
//                                                                         
// <Definition of class [[GUSIOTInetStrategy]]>=                           
class GUSIOTInetStrategy : public GUSIOTStrategy {
protected:
	virtual	int	PackAddress(
		const void * address, socklen_t len, TNetbuf & addr, bool non_null);
	virtual	int	UnpackAddress(const TNetbuf & addr, void * address, socklen_t * len);
};
// \section{Definition of [[GUSIOTMInetOptions]]}                          
//                                                                         
// The options common to TCP and UDP sockets are implemented in the mixin class
// [[GUSIOTMInetOptions]].                                                 
//                                                                         
// <Definition of class [[GUSIOTMInetOptions]]>=                           
class GUSIOTMInetOptions {
protected:
	bool	DoGetSockOpt(
						int * result, EndpointRef endpoint, int level, int optname, 
						void *optval, socklen_t * optlen);
	bool	DoSetSockOpt(
						int * result, EndpointRef endpoint, int level, int optname, 
						void *optval, socklen_t optlen);		
	bool	DoIoctl(int * result, unsigned int request, va_list arg);
};
// \section{Definition of [[GUSIOTTcpStrategy]]}                           
//                                                                         
// The [[GUSIOTTcpStrategy]] defines the configuration for TCP sockets     
//                                                                         
// <Definition of class [[GUSIOTTcpStrategy]]>=                            
class GUSIOTTcpStrategy : public GUSIOTInetStrategy {
protected:
	virtual const char *	ConfigPath();
};
// \section{Definition of [[GUSIOTTcpSocket]]}                             
//                                                                         
// A [[GUSIOTUdpSocket]] supports TCP stream services over Open Transport. 
//                                                                         
// <Definition of class [[GUSIOTTcpSocket]]>=                              
class GUSIOTTcpSocket : public GUSIOTStreamSocket, protected GUSIOTMInetOptions {
public:
	// \section{Implementation of [[GUSIOTTcpSocket]]}                         
 //                                                                         
 // [[Clone]] creates another socket of the same class.                     
 //                                                                         
 // <Overridden member functions for [[GUSIOTTcpSocket]]>=                  
 virtual GUSIOTSocket * Clone();
 // [[getsockopt]] and [[setsockopt]] for [[GUSIOTTcpSocket]] add the options
 // for multicast.                                                          
 //                                                                         
 // <Overridden member functions for [[GUSIOTTcpSocket]]>=                  
 virtual int getsockopt(int level, int optname, void *optval, socklen_t * optlen);
 // <Overridden member functions for [[GUSIOTTcpSocket]]>=                  
 virtual int setsockopt(int level, int optname, void *optval, socklen_t optlen);
 // [[ioctl]] deals with interface access.                                  
 //                                                                         
 // <Overridden member functions for [[GUSIOTTcpSocket]]>=                  
 virtual int ioctl(unsigned int request, va_list arg);
protected:
	GUSIOTTcpSocket(GUSIOTStrategy * strategy) : GUSIOTStreamSocket(strategy) {}

	friend class GUSIOTTcpFactory;
};
// \section{Definition of [[GUSIOTUdpStrategy]]}                           
//                                                                         
// The [[GUSIOTUdpStrategy]] defines the configuration for UDP sockets     
//                                                                         
// <Definition of class [[GUSIOTUdpStrategy]]>=                            
class GUSIOTUdpStrategy : public GUSIOTInetStrategy {
protected:
	virtual const char *	ConfigPath();
};
// \section{Definition of [[GUSIOTUdpSocket]]}                             
//                                                                         
// A [[GUSIOTUdpSocket]] supports UDP datagram services over Open Transport.
//                                                                         
// <Definition of class [[GUSIOTUdpSocket]]>=                              
class GUSIOTUdpSocket : public GUSIOTDatagramSocket, protected GUSIOTMInetOptions {
public:
	// \section{Implementation of [[GUSIOTUdpSocket]]}                         
 //                                                                         
 // [[Clone]] creates another socket of the same class.                     
 //                                                                         
 // <Overridden member functions for [[GUSIOTUdpSocket]]>=                  
 virtual GUSIOTSocket * Clone();
 // [[getsockopt]] and [[setsockopt]] for [[GUSIOTUdpSocket]] add the options
 // for multicast.                                                          
 //                                                                         
 // <Overridden member functions for [[GUSIOTUdpSocket]]>=                  
 virtual int getsockopt(int level, int optname, void *optval, socklen_t * optlen);
 // <Overridden member functions for [[GUSIOTUdpSocket]]>=                  
 virtual int setsockopt(int level, int optname, void *optval, socklen_t optlen);
 // [[ioctl]] deals with interface access.                                  
 //                                                                         
 // <Overridden member functions for [[GUSIOTUdpSocket]]>=                  
 virtual int ioctl(unsigned int request, va_list arg);
protected:
	GUSIOTUdpSocket(GUSIOTStrategy * strategy) : GUSIOTDatagramSocket(strategy) {}

	friend class GUSIOTUdpFactory;
};
// <Member functions for class [[GUSIOTTcpFactory]]>=                      
GUSISocketFactory *	GUSIOTTcpFactory::sInstance;

GUSISocketFactory * GUSIOTTcpFactory::Instance()
{
	if (!sInstance)
		sInstance = new GUSIOTTcpFactory;
	return sInstance;
}
// The real smarts of [[GUSIOTTcpFactory]] are in the [[Strategy]] member  
// function.                                                               
//                                                                         
// <Member functions for class [[GUSIOTTcpFactory]]>=                      
GUSIOTStrategy * GUSIOTTcpFactory::Strategy(int, int, int)
{
	static GUSIOTStrategy * tcpStrategy = new GUSIOTTcpStrategy;
		
	return tcpStrategy;
}
// OpenTransport TCP sockets have a little bit of added functionality over 
// stream sockets.                                                         
//                                                                         
// <Member functions for class [[GUSIOTTcpFactory]]>=                      
GUSISocket * GUSIOTTcpFactory::socket(int domain, int type, int protocol)
{
	GUSIOTStrategy * strategy = Strategy(domain, type, protocol);
	if (Initialize() && strategy)
		return new GUSIOTTcpSocket(strategy);
	else
		return static_cast<GUSISocket *>(0);
}
// <Member functions for class [[GUSIOTUdpFactory]]>=                      
GUSISocketFactory *	GUSIOTUdpFactory::sInstance;

GUSISocketFactory * GUSIOTUdpFactory::Instance()
{
	if (!sInstance)
		sInstance = new GUSIOTUdpFactory;
	return sInstance;
}
// The real smarts of [[GUSIOTUdpFactory]] are in the [[Strategy]] member  
// function.                                                               
//                                                                         
// <Member functions for class [[GUSIOTUdpFactory]]>=                      
GUSIOTStrategy * GUSIOTUdpFactory::Strategy(int, int, int)
{
	static GUSIOTStrategy * udpStrategy = new GUSIOTUdpStrategy;
		
	return udpStrategy;
}
// OpenTransport UDP sockets have a little bit of added functionality over 
// datagram sockets.                                                       
//                                                                         
// <Member functions for class [[GUSIOTUdpFactory]]>=                      
GUSISocket * GUSIOTUdpFactory::socket(int domain, int type, int protocol)
{
	GUSIOTStrategy * strategy = Strategy(domain, type, protocol);
	if (Initialize() && strategy)
		return new GUSIOTUdpSocket(strategy);
	else
		return static_cast<GUSISocket *>(0);
}
// \section{Implementation of [[GUSIOTInetStrategy]]}                      
//                                                                         
// [[GUSIOTInetStrategy]] defines the packing and unpacking functions.     
//                                                                         
// <Member functions for class [[GUSIOTInetStrategy]]>=                    
int	GUSIOTInetStrategy::PackAddress(
	const void * address, socklen_t len, TNetbuf & addr, bool non_null)
{
	const sockaddr_in *name	=	(const sockaddr_in *)address;
	// <Sanity checks for [[GUSIOTInetStrategy::PackAddress]]>=                
 if (!GUSI_ASSERT_CLIENT(
 		len >= sizeof(struct sockaddr_in),
 		("PackAddress: address len %d < %d\n", len, sizeof(struct sockaddr_in)))
 )
 	return GUSISetPosixError(EINVAL);
 if (!GUSI_ASSERT_CLIENT(
 		name->sin_family == AF_INET,
 		("PackAddress: family %d != %d\n", name->sin_family, AF_INET))
 )
 	return GUSISetPosixError(EAFNOSUPPORT);
 if (non_null && (!name->sin_addr.s_addr || !name->sin_port))
 	return GUSISetPosixError(EADDRNOTAVAIL);
	OTInitInetAddress(
		reinterpret_cast<InetAddress *>(addr.buf), 
		name->sin_port, name->sin_addr.s_addr);
	addr.len = 16;
	
	return 0;
}
// <Member functions for class [[GUSIOTInetStrategy]]>=                    
int	GUSIOTInetStrategy::UnpackAddress(const TNetbuf & addr, void * address, socklen_t * len)
{
	sockaddr_in *name	=	(sockaddr_in *)address;
	// <Sanity checks for [[GUSIOTInetStrategy::UnpackAddress]]>=              
 if (!GUSI_ASSERT_CLIENT(
 		*len >= sizeof(struct sockaddr_in),
 		("UnpackAddress: address len %d < %d\n", len, sizeof(struct sockaddr_in)))
 )
 	return GUSISetPosixError(EINVAL);
	const InetAddress * otaddr = reinterpret_cast<InetAddress *>(addr.buf);
	name->sin_family 		= AF_INET;
	name->sin_port   		= otaddr->fPort;
	name->sin_addr.s_addr 	= otaddr->fHost;
	*len					= sizeof(struct sockaddr_in);
	
	return 0;
}
// \section{Implementation of [[GUSIOTMInetOptions]]}                      
//                                                                         
//                                                                         
// <Member functions for class [[GUSIOTMInetOptions]]>=                    
bool GUSIOTMInetOptions::DoGetSockOpt(
					int * result, EndpointRef endpoint, int level, int optname, 
					void *optval, socklen_t * optlen)
{
	TOptMgmt 	optReq;
	UInt8	 	optBuffer[ kOTOptionHeaderSize + 50 ];
	TOption*	opt 	= (TOption*)optBuffer;
	int			len;
	
	optReq.flags = T_CURRENT;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= INET_IP;
	opt->name 	= optname;
		
	switch (level) {
	case SOL_SOCKET:
		switch (optname) {
		case SO_REUSEPORT:
			opt->name = SO_REUSEADDR;
			// Fall through
		case SO_REUSEADDR:
		case SO_DONTROUTE:
			len = 4;
			break;
		default:
			goto notSupported;
		}
		break;
	case IPPROTO_IP:
		switch (optname) {
		case IP_OPTIONS:
			len = *optlen;
			break;
		case IP_TOS:
		case IP_TTL:
			len = 1;
			break;
		default:
			goto notSupported;
		}
		break;
	default:
		goto notSupported;
	}
	optReq.opt.len 		= opt->len = kOTOptionHeaderSize+len;
	optReq.opt.maxlen 	= sizeof(optBuffer);
	if (*result = GUSISetMacError(OTOptionManagement(endpoint, &optReq, &optReq)))
		return true;
	switch (optname) {
	case IP_TOS:
	case IP_TTL:
		*reinterpret_cast<int *>(optval) = *reinterpret_cast<char *>(opt->value);
		*optlen = 4;
		break;
	case IP_OPTIONS:
		len = optReq.opt.len;
		// Fall through
	default:
		memcpy(optval, opt->value, len);
		*optlen = len;
		break;
	}
	
	return true;
notSupported:	
	return false;
}
// <Member functions for class [[GUSIOTMInetOptions]]>=                    
bool GUSIOTMInetOptions::DoSetSockOpt(
					int * result, EndpointRef endpoint, int level, int optname, 
					void *optval, socklen_t optlen)
{
	TOptMgmt 			optReq;
	UInt8	 			optBuffer[ kOTOptionHeaderSize + sizeof(struct linger) ];
	TOption*	 		opt 	= (TOption*)optBuffer;
	t_kpalive			kpal	= {1, 120};
	int					len;
	char				val;
		
	optReq.flags = T_NEGOTIATE;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= INET_IP;
	opt->name 	= optname;
	
	switch (level) {
	case SOL_SOCKET:
		switch (optname) {
		case SO_REUSEPORT:
			opt->name = SO_REUSEADDR;
			// Fall through
		case SO_REUSEADDR:
		case SO_DONTROUTE:
			len = 4;
			break;
		default:
			goto notSupported;
		}
		break;
	case IPPROTO_IP:
		switch (optname) {
		case IP_OPTIONS:
			len = optlen;
			break;
		case IP_TOS:
		case IP_TTL:
			val 	= *reinterpret_cast<int *>(optval);
			optval	= &val;
			len 	= 1;
			break;
		default:
			goto notSupported;
		}
		break;
	default:
		goto notSupported;
	}
	optReq.opt.len = opt->len = kOTOptionHeaderSize+len;
	optReq.opt.maxlen 		  = sizeof(optBuffer);
	memcpy(opt->value, optval, len);

	*result = GUSISetMacError(OTOptionManagement(endpoint, &optReq, &optReq));
	
	return true;
notSupported:	
	return false;
}
// Internet sockets have to support a fairly complex set of [[ioctl]] options to
// obtain information about hwardware interfaces. We don't support changing any
// of the information.                                                     
//                                                                         
// [[SetInterface]] fills in an [[ifreq]] structure. The name is synthesized, encoding 
// the interface index and secondary IP address for fast lookup.           
//                                                                         
// <Member functions for class [[GUSIOTMInetOptions]]>=                    
static void	SetAddress(sockaddr_in * sa, InetHost addr)
{
	sa->sin_family 		= AF_INET;
	sa->sin_port		= 0;
	sa->sin_addr.s_addr	= addr;
}

static void SetInterface(ifreq * ifr, int ifNum, int aliasNum, InetHost addr)
{
	GUSI_sprintf(ifr->ifr_name, (aliasNum ? "ot%d:%d" : "ot%d"), ifNum, aliasNum);
	SetAddress(reinterpret_cast<sockaddr_in *>(&ifr->ifr_addr), addr);
}
// [[GetInterfaceList]] gets a list of all IP addresses.                   
//                                                                         
// <Member functions for class [[GUSIOTMInetOptions]]>=                    
static int GetInterfaceList(ifconf * conf)
{
	InetInterfaceInfo	info;
	int					maxInterfaces 	= conf->ifc_len / sizeof(ifreq);
	int					numInterfaces	= 0;
	SInt32				interface 		= 0;
	ifreq *				ifr = conf->ifc_req;
	
	if (!maxInterfaces)
		return GUSISetPosixError(EINVAL);
	
	while (!OTInetGetInterfaceInfo(&info, numInterfaces)) {
		SetInterface(ifr++, numInterfaces, 0, info.fAddress);
		if (++interface == maxInterfaces)
			goto bufferFull;
		if (info.fIPSecondaryCount) {
			InetHost * secondaries = new InetHost[info.fIPSecondaryCount];
			OTInetGetSecondaryAddresses(secondaries, &info.fIPSecondaryCount, numInterfaces);
			for (int i = 0; i < info.fIPSecondaryCount; ++i) {
				SetInterface(ifr++, numInterfaces, i+1, secondaries[i]);
				if (++interface == maxInterfaces)
					goto bufferFull;
			}
			delete[] secondaries;
		}
		++numInterfaces;
	}
bufferFull:
	conf->ifc_len = interface * sizeof(ifreq);
	
	return 0;
}
// [[GetInterfaceParam]] gets a parameter for an interface. Secondary IP addresses are only 
// retrieved for [[SIOCGIFADDR]].                                          
//                                                                         
// <Member functions for class [[GUSIOTMInetOptions]]>=                    
static int GetInterfaceParam(ifreq * ifr, unsigned int request)
{
	int ifnum = atoi(ifr->ifr_name+2);
	int ifalias = 0;
	char * alias = strchr(ifr->ifr_name, ':');
	if (alias)
		ifalias = atoi(alias+1);
		
	InetInterfaceInfo	info;
	if (OTInetGetInterfaceInfo(&info, ifnum) || ifalias > info.fIPSecondaryCount)
		return GUSISetPosixError(ENOENT);
	if (ifalias && request == SIOCGIFADDR) {
		InetHost * secondaries = new InetHost[info.fIPSecondaryCount];
		OTInetGetSecondaryAddresses(secondaries, &info.fIPSecondaryCount, ifnum);
		info.fAddress = secondaries[ifalias-1];
		delete[] secondaries;
	}
	switch (request) {
	case SIOCGIFADDR:
		SetAddress(reinterpret_cast<sockaddr_in *>(&ifr->ifr_addr), info.fAddress);
		break;
	case SIOCGIFFLAGS:
		ifr->ifr_flags = IFF_UP | IFF_BROADCAST | IFF_MULTICAST;
		break;
	case SIOCGIFBRDADDR:
		SetAddress(reinterpret_cast<sockaddr_in *>(&ifr->ifr_addr), info.fBroadcastAddr);
		break;
	case SIOCGIFNETMASK:
		SetAddress(reinterpret_cast<sockaddr_in *>(&ifr->ifr_addr), info.fNetmask);
		break;
	}
	
	return 0;
}

// <Member functions for class [[GUSIOTMInetOptions]]>=                    
bool GUSIOTMInetOptions::DoIoctl(int * result, unsigned int request, va_list arg)
{
	switch (request) {
	case SIOCGIFCONF:
		*result = GetInterfaceList(va_arg(arg, ifconf *));
		
		return true;
	case SIOCGIFADDR:
	case SIOCGIFFLAGS:
	case SIOCGIFBRDADDR:
	case SIOCGIFNETMASK:
		*result = GetInterfaceParam(va_arg(arg, ifreq *), request);

		return true;
	}
	
	return false;
}
// \section{Implementation of [[GUSIOTTcpStrategy]]}                       
//                                                                         
// [[GUSIOTTcpStrategy]] merely needs to define the creation options.      
//                                                                         
// <Member functions for class [[GUSIOTTcpStrategy]]>=                     
const char * GUSIOTTcpStrategy::ConfigPath()
{
	return kTCPName;
}
// <Member functions for class [[GUSIOTTcpSocket]]>=                       
GUSIOTSocket * GUSIOTTcpSocket::Clone()
{
	return new GUSIOTTcpSocket(fStrategy);
}
// <Member functions for class [[GUSIOTTcpSocket]]>=                       
int GUSIOTTcpSocket::getsockopt(int level, int optname, void *optval, socklen_t * optlen)
{
	int result = GUSIOTSocket::getsockopt(level, optname, optval, optlen);
	
	if (!result || errno != EOPNOTSUPP 
	 || GUSIOTMInetOptions::DoGetSockOpt(&result, fEndpoint, level, optname, optval, optlen)
	)
		return result;
	
	TOptMgmt 	optReq;
	UInt8	 	optBuffer[ kOTOptionHeaderSize + sizeof(long) ];
	TOption*	opt 	= (TOption*)optBuffer;
	int			len;
		
	optReq.flags = T_CURRENT;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= INET_TCP;
	opt->name 	= optname;
	
	switch (level) {
	case IPPROTO_TCP:
		switch (optname) {
		case TCP_MAXSEG:
		case TCP_NODELAY:
			len = 4;
			break;
		default:
			goto notSupported;
		}
		break;
	default:
		goto notSupported;
	}
		
	optReq.opt.len 		= opt->len = kOTOptionHeaderSize+len;
	optReq.opt.maxlen 	= sizeof(optBuffer);
	if (GUSISetMacError(OTOptionManagement(fEndpoint, &optReq, &optReq)))
		return -1;
	memcpy(optval, opt->value, len);
	*optlen = len;
	
	return 0;
notSupported:	
	GUSI_ASSERT_CLIENT(false, ("getsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTTcpSocket]]>=                       
int GUSIOTTcpSocket::setsockopt(int level, int optname, void *optval, socklen_t optlen)
{
	int result = GUSIOTSocket::setsockopt(level, optname, optval, optlen);
	
	if (!result || errno != EOPNOTSUPP 
	 || GUSIOTMInetOptions::DoSetSockOpt(&result, fEndpoint, level, optname, optval, optlen)
	)
		return result;

	TOptMgmt 			optReq;
	UInt8	 			optBuffer[ kOTOptionHeaderSize + sizeof(TIPAddMulticast) ];
	TOption*	 		opt 	= (TOption*)optBuffer;
	int					len;
		
	optReq.flags = T_NEGOTIATE;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= INET_TCP;
	opt->name 	= optname;
	
	switch (level) {
	case IPPROTO_TCP:
		switch (optname) {
		case TCP_MAXSEG:
		case TCP_NODELAY:
			len = 4;
			break;
		default:
			goto notSupported;
		}
		break;
	default:
		goto notSupported;
	}
	optReq.opt.len 		= opt->len = kOTOptionHeaderSize+len;
	optReq.opt.maxlen 	= sizeof(optBuffer);
	memcpy(opt->value, optval, len);

	return GUSISetMacError(OTOptionManagement(fEndpoint, &optReq, &optReq));
notSupported:	
	GUSI_ASSERT_CLIENT(false, ("setsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTTcpSocket]]>=                       
int GUSIOTTcpSocket::ioctl(unsigned int request, va_list arg)
{
	int	result;
	
	if (GUSIOTMInetOptions::DoIoctl(&result, request, arg))
		return result;
	else	
		return GUSIOTSocket::ioctl(request, arg);
}
// \section{Implementation of [[GUSIOTUdpStrategy]]}                       
//                                                                         
// [[GUSIOTUdpStrategy]] merely needs to define the creation options.      
//                                                                         
// <Member functions for class [[GUSIOTUdpStrategy]]>=                     
const char * GUSIOTUdpStrategy::ConfigPath()
{
	return kUDPName;
}
// <Member functions for class [[GUSIOTUdpSocket]]>=                       
GUSIOTSocket * GUSIOTUdpSocket::Clone()
{
	return new GUSIOTUdpSocket(fStrategy);
}
// <Member functions for class [[GUSIOTUdpSocket]]>=                       
int GUSIOTUdpSocket::getsockopt(int level, int optname, void *optval, socklen_t * optlen)
{
	int result = GUSIOTSocket::getsockopt(level, optname, optval, optlen);
	
	if (!result || errno != EOPNOTSUPP 
	 || GUSIOTMInetOptions::DoGetSockOpt(&result, fEndpoint, level, optname, optval, optlen)
	)
		return result;
	
	TOptMgmt 	optReq;
	UInt8	 	optBuffer[ kOTOptionHeaderSize + sizeof(long) ];
	TOption*	opt 	= (TOption*)optBuffer;
	int			len;
		
	optReq.flags = T_CURRENT;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->name 	= optname;
	
	switch (level) {
	case SOL_SOCKET:
		opt->level 	= INET_IP;
		switch (optname) {
		case SO_BROADCAST:
			len = 4;
			break;
		default:
			goto notSupported;
		}
		break;
	case IPPROTO_IP:
		opt->level 	= INET_IP;
		switch (optname) {
		case IP_HDRINCL:
		case IP_RCVDSTADDR:
		case IP_MULTICAST_IF:
			len = 4;
			break;
		case IP_MULTICAST_TTL:
		case IP_MULTICAST_LOOP:
			len = 1;
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
	case IP_MULTICAST_TTL:
	case IP_MULTICAST_LOOP:
		*reinterpret_cast<int *>(optval) = *reinterpret_cast<char *>(opt->value);
		*optlen = 4;
		break;
	case IP_HDRINCL:
	case IP_RCVDSTADDR:
	case SO_BROADCAST:
	case IP_MULTICAST_IF:
		memcpy(optval, opt->value, len);
		*optlen = len;
		break;
	}
	
	return 0;
notSupported:	
	GUSI_ASSERT_CLIENT(false, ("getsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTUdpSocket]]>=                       
int GUSIOTUdpSocket::setsockopt(int level, int optname, void *optval, socklen_t optlen)
{
	int result = GUSIOTSocket::setsockopt(level, optname, optval, optlen);
	
	if (!result || errno != EOPNOTSUPP 
	 || GUSIOTMInetOptions::DoSetSockOpt(&result, fEndpoint, level, optname, optval, optlen)
	)
		return result;

	TOptMgmt 			optReq;
	UInt8	 			optBuffer[ kOTOptionHeaderSize + sizeof(TIPAddMulticast) ];
	TOption*	 		opt 	= (TOption*)optBuffer;
	char				val;
		
	optReq.flags = T_NEGOTIATE;
	optReq.opt.buf = (UInt8*) optBuffer;
		
	opt->level 	= INET_IP;
	opt->name 	= optname;
	
	int	len;
	switch (level) {
	case SOL_SOCKET:
		switch (optname) {
		case SO_BROADCAST:
			len = 4;
			break;
		default:
			goto notSupported;
		}
		break;
	case IPPROTO_IP:
		switch (optname) {
		case IP_HDRINCL:
		case IP_RCVDSTADDR:
		case IP_MULTICAST_IF:
			len = 4;
			break;
		case IP_MULTICAST_TTL:
		case IP_MULTICAST_LOOP:
			val = *reinterpret_cast<long *>(optval) != 0;
			optval = &val;
			len = 1;
			break;
		case IP_ADD_MEMBERSHIP:
		case IP_DROP_MEMBERSHIP:
			len = 8;
			break;
		default:
			goto notSupported;
		}
		break;
	}
	optReq.opt.len = opt->len = kOTOptionHeaderSize+len;
	memcpy(opt->value, optval, len);

	return GUSISetMacError(OTOptionManagement(fEndpoint, &optReq, &optReq));
notSupported:	
	GUSI_ASSERT_CLIENT(false, ("setsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIOTUdpSocket]]>=                       
int GUSIOTUdpSocket::ioctl(unsigned int request, va_list arg)
{
	int	result;
	
	if (GUSIOTMInetOptions::DoIoctl(&result, request, arg))
		return result;
	else	
		return GUSIOTSocket::ioctl(request, arg);
}
// \section{Implementation of Open Transport Internet hooks}               
//                                                                         
//                                                                         
// <Implementation of [[GUSIwithOTInetSockets]]>=                          
void GUSIwithOTTcpSockets()
{
	gGUSIInetFactories.AddFactory(SOCK_STREAM, 0, GUSIOTTcpFactory::Instance());
	GUSIOTNetDB::Instantiate();
}

void GUSIwithOTUdpSockets()
{
	gGUSIInetFactories.AddFactory(SOCK_DGRAM, 0, GUSIOTUdpFactory::Instance());
	GUSIOTNetDB::Instantiate();
}

void GUSIwithOTInetSockets()
{
	GUSIwithOTTcpSockets();
	GUSIwithOTUdpSockets();
}

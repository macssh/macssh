// <GUSIOTNetDB.cp>=                                                       
#include "GUSIInternal.h"
#include "GUSIOTNetDB.h"
#include "GUSIOTInet.h"

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if UNIVERSAL_INTERFACES_VERSION < 0x0340
#undef AF_INET
#endif
#undef IP_OPTIONS
#undef IP_TOS
#undef IP_TTL
#undef IP_HDRINCL
#undef IP_MULTICAST_IF
#undef IP_MULTICAST_TTL
#undef IP_MULTICAST_LOOP
#undef IP_ADD_MEMBERSHIP
#undef IP_DROP_MEMBERSHIP

#include <OpenTptInternet.h>

// <Asynchronous notifier function for [[GUSIOTNetDB]]>=                   
inline uint32_t CompleteMask(OTEventCode code)	
{ 	
	return 1 << (code & 0x1F); 
}

pascal void GUSIOTNetDBNotify(
	GUSIOTNetDB * netdb, OTEventCode code, OTResult result, void *cookie)
{
	GUSI_MESSAGE(("GUSIOTNetDBNotify %08x %d\n", code, result));
	GUSIContext *	context = netdb->fCreationContext;
	
	switch (code & 0x7F000000L) {
	case 0:
		netdb->fEvent |= code;
		result = 0;
		break;
	case kPRIVATEEVENT:
	case kCOMPLETEEVENT:
		if (!(code & 0x00FFFFE0))
			netdb->fCompletion |= CompleteMask(code);
		switch (code) {
		case T_OPENCOMPLETE:
			netdb->fSvc = static_cast<InetSvcRef>(cookie);
			break;
		case T_DNRSTRINGTOADDRCOMPLETE:
		case T_DNRADDRTONAMECOMPLETE:
			context = static_cast<GUSIContext **>(cookie)[-1];
			break;
		}
		break;
	default:
		switch (code) {
		case kOTProviderWillClose:
		case kOTProviderIsClosed:
			netdb->fCreationContext = nil;	// Close & reopen
			break;
		default: 
			result = 0;
			break;
		}
		break;
	}
	if (result)
		netdb->fAsyncError = result;
	context->Wakeup();
}
// <Member functions for class [[GUSIOTNetDB]]>=                           
GUSIOTNetDB::GUSIOTNetDB()
{
	// <Initialize fields of [[GUSIOTNetDB]]>=                                 
 fEvent 				= 0;
 fCompletion			= 0;
 fAsyncError 		= 0;
 fSvc				= 0;
 fCreationContext	= 0;
}
// The Open Transport DNR code is installed by calling [[Instantiate]], which initializes
// the singleton instance of [[GUSINetDB]].                                
//                                                                         
// <Member functions for class [[GUSIOTNetDB]]>=                           
void GUSIOTNetDB::Instantiate()
{
	if (!sInstance)
		sInstance = new GUSIOTNetDB;
}
// <Member functions for class [[GUSIOTNetDB]]>=                           
bool GUSIOTNetDB::Resolver()
{
	if (!fCreationContext) {
		if (fSvc) {
			OTCloseProvider(fSvc);
			fSvc = nil;
		}
		fCreationContext = GUSIContext::Current();
		if (!GUSIOTFactory::Initialize())
			return false;
		fAsyncError = 0;
		OSStatus syncError = OTAsyncOpenInternetServices(
			kDefaultInternetServicesPath, 
			0,
			reinterpret_cast<OTNotifyProcPtr>(GUSIOTNetDBNotify),
			this);
		if (!syncError)
			while (!fAsyncError && !(fCompletion & CompleteMask(T_OPENCOMPLETE)))
				GUSIContext::Yield(kGUSIBlock);
	}
	return fSvc != 0;
}
// Naturally, Open Transport keeps its host data in different structures than the socket
// standard.                                                               
//                                                                         
// <Member functions for class [[GUSIOTNetDB]]>=                           
static void CopyHost(InetHostInfo & otHost, GUSIhostent & unixHost)
{
	size_t len = strlen(otHost.name);
	len = (len+4) & ~3;
	
	unixHost.Alloc(len+kMaxHostAddrs*4);
	strcpy(unixHost.h_name, otHost.name);
	unixHost.h_aliases[0] 	= NULL;			// Aliases not supported
	unixHost.h_addrtype		= AF_INET;	
	unixHost.h_length		= 4;
	
	int addrs = 0;
	for (int i=0; i<kMaxHostAddrs && otHost.addrs[addrs]!=0; ++i, ++addrs) {
		unixHost.h_addr_list[addrs] = unixHost.fName + len;
		len += 4;
		memcpy(unixHost.h_addr_list[addrs], &otHost.addrs[addrs], 4);
	}
	unixHost.h_addr_list[addrs] = NULL;
}
// <Member functions for class [[GUSIOTNetDB]]>=                           
hostent * GUSIOTNetDB::gethostbyname(const char * name)
{
	// <Open Open Transport DNR or fail lookup>=                               
 if (!Resolver())
 	return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);
	if (!strcmp(name, "localhost")) {
		long ipaddr = gethostid();
		if (ipaddr)
			return gethostbyaddr((char *) &ipaddr, sizeof(in_addr), AF_INET);
		return GUSISetHostError(HOST_NOT_FOUND), static_cast<hostent *>(nil);
	}
	// <Declare and initialize [[otHost]] and [[unixHost]]>=                   
 struct	{
 	GUSIContext *	fContext;
 	InetHostInfo	fInfo;
 }				otHost;
 GUSIhostent &	unixHost 	= *fHost;
 otHost.fContext 			= GUSIContext::Current();
	fCompletion &= ~CompleteMask(T_DNRSTRINGTOADDRCOMPLETE);
	fAsyncError = 0;
	OSStatus syncError = OTInetStringToAddress(fSvc, const_cast<char *>(name), &otHost.fInfo);
	if (syncError)
		return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);
	while (!fAsyncError && !(fCompletion & CompleteMask(T_DNRSTRINGTOADDRCOMPLETE)))
		GUSIContext::Yield(kGUSIBlock);
	if (fAsyncError)
		return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);

	CopyHost(otHost.fInfo, unixHost);
	
	return &unixHost;
}
// <Member functions for class [[GUSIOTNetDB]]>=                           
hostent * GUSIOTNetDB::gethostbyaddr(const void * addrP, size_t len, int)
{
	// <Open Open Transport DNR or fail lookup>=                               
 if (!Resolver())
 	return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);
	// <Declare and initialize [[otHost]] and [[unixHost]]>=                   
 struct	{
 	GUSIContext *	fContext;
 	InetHostInfo	fInfo;
 }				otHost;
 GUSIhostent &	unixHost 	= *fHost;
 otHost.fContext 			= GUSIContext::Current();

	InetHost addr = *(InetHost *)addrP;
	if (addr == 0x7F000001)
		addr = static_cast<InetHost>(gethostid());
	
	fCompletion &= ~CompleteMask(T_DNRADDRTONAMECOMPLETE);
	fAsyncError = 0;
	OSStatus syncError = OTInetAddressToName(fSvc, addr, otHost.fInfo.name);
	if (syncError)
		return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);
	while (!fAsyncError && !(fCompletion & CompleteMask(T_DNRADDRTONAMECOMPLETE)))
		GUSIContext::Yield(kGUSIBlock);
	if (fAsyncError)
		return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);

	memset(otHost.fInfo.addrs, 0, kMaxHostAddrs*4);
	otHost.fInfo.addrs[0] = addr;
	// Apparently, Open Transport likes appaending an extra [['.']] to the domain name.
 //                                                                         
 // <Strip extra period from name>=                                         
 len = strlen(otHost.fInfo.name);
 if (otHost.fInfo.name[len-1] == '.')
 	otHost.fInfo.name[len-1] = 0;
	CopyHost(otHost.fInfo, unixHost);
	
	return &unixHost;
}
// <Member functions for class [[GUSIOTNetDB]]>=                           
char * GUSIOTNetDB::inet_ntoa(in_addr inaddr)
{
	GUSIhostent &	unixHost = *fHost;
	GUSI_sprintf(unixHost.fAddrString, "%d.%d.%d.%d", 
		(inaddr.s_addr >> 24) & 0xFF,
		(inaddr.s_addr >> 16) & 0xFF,
		(inaddr.s_addr >> 8) & 0xFF,
		inaddr.s_addr & 0xFF);
	return unixHost.fAddrString;
}
// <Member functions for class [[GUSIOTNetDB]]>=                           
long GUSIOTNetDB::gethostid()
{
	if (!Resolver())
		return GUSISetHostError(NO_RECOVERY), 0;
	InetInterfaceInfo	info;
	OTInetGetInterfaceInfo(&info, kDefaultInetInterface);
	
	return static_cast<long>(info.fAddress);
}

// <GUSIMTNetDB.cp>=                                                       
#include "GUSIInternal.h"
#include "GUSIMTNetDB.h"
#include "GUSIMTInet.h"
#include "GUSIContext.h"

#include <MacTCP.h>

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

// \section{Implementation of [[GUSIMTNetDB]]}                             
//                                                                         
// To implement this, we need to include the MacTCP DNR code, a somewhat unpleasant
// operation. If possible, we wrap within an anonymous namespace.          
//                                                                         
// <MacTCP DNR code>=                                                      
#ifdef GUSI_COMPILER_HAS_NAMESPACE
namespace {
#include <AddressXlation.h>

#include "dnr.c"
}
#else
#include "dnr.c"
#endif
// When a DNR call terminates, it wakes up the thread issuing the call.    
//                                                                         
// <MacTCP DNR code>=                                                      
static pascal void DNRDone(struct hostInfo *, GUSIContext * context)
{
	context->Wakeup();
}

#if GENERATINGCFM
RoutineDescriptor	uDNRDone = 
		BUILD_ROUTINE_DESCRIPTOR(uppResultProcInfo, DNRDone);
#else
#define uDNRDone DNRDone
#endif
// The MacTCP DNR code is installed by calling [[Instantiate]], which initializes
// the singleton instance of [[GUSINetDB]].                                
//                                                                         
// <Member functions for class [[GUSIMTNetDB]]>=                           
void GUSIMTNetDB::Instantiate()
{
	if (!sInstance)
		sInstance = new GUSIMTNetDB;
}
// <Member functions for class [[GUSIMTNetDB]]>=                           
OSErr 	GUSIMTNetDB::sResolverState	= 1;

bool GUSIMTNetDB::Resolver()
{
	GUSIMTInetSocket::Driver();
	if (sResolverState == 1)
		sResolverState = OpenResolver(nil);
	
	return !sResolverState;
}
// Naturally, MacTCP keeps its host data in different structures than the socket
// standard.                                                               
//                                                                         
// <Member functions for class [[GUSIMTNetDB]]>=                           
static void CopyHost(hostInfo & macHost, GUSIhostent & unixHost)
{
	/* for some reason there is a dot at the end of the name */
	size_t len = strlen(macHost.cname);
	if (macHost.cname[len-1] == '.')
		macHost.cname[--len] = 0;
	len = (len+4) & ~3;
	
	unixHost.Alloc(len+NUM_ALT_ADDRS*4);
	strcpy(unixHost.h_name, macHost.cname);
	unixHost.h_aliases[0] 	= NULL;			// Aliases not supported
	unixHost.h_addrtype		= AF_INET;	
	unixHost.h_length		= 4;
	
	int addrs = 0;
	for (int i=0; i<NUM_ALT_ADDRS && macHost.addr[addrs]!=0; ++i, ++addrs) {
		unixHost.h_addr_list[addrs] = unixHost.fName + len;
		len += 4;
		memcpy(unixHost.h_addr_list[addrs], &macHost.addr[addrs], 4);
	}
	unixHost.h_addr_list[addrs] = NULL;
}
// <Member functions for class [[GUSIMTNetDB]]>=                           
hostent * GUSIMTNetDB::gethostbyname(const char * name)
{
	// <Open MacTCP DNR or fail lookup>=                                       
 if (!Resolver())
 	return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);
	if (!strcmp(name, "localhost")) {
		long ipaddr = gethostid();
		if (ipaddr)
			return gethostbyaddr((char *) &ipaddr, sizeof(in_addr), AF_INET);
		return GUSISetHostError(HOST_NOT_FOUND), static_cast<hostent *>(nil);
	}
	// <Declare and initialize [[macHost]] and [[unixHost]]>=                  
 hostInfo		macHost;
 GUSIhostent &	unixHost = *fHost;

 for (int i=0; i<NUM_ALT_ADDRS; ++i)
 	macHost.addr[i] = 0;
	if (StrToAddr(const_cast<char *>(name), &macHost, ResultUPP(&uDNRDone), 
			reinterpret_cast<Ptr>(GUSIContext::Current())
	 	) == cacheFault
	)
		do {
			GUSIContext::Yield(kGUSIBlock);
		} while (macHost.rtnCode == cacheFault);
	if (macHost.rtnCode)
		return GUSISetMacHostError(macHost.rtnCode), static_cast<hostent *>(nil);
	if (macHost.cname[0] == 0)
		return GUSISetHostError(HOST_NOT_FOUND), static_cast<hostent *>(nil);

	CopyHost(macHost, unixHost);
	
	return &unixHost;
}
// <Member functions for class [[GUSIMTNetDB]]>=                           
hostent * GUSIMTNetDB::gethostbyaddr(const void * addrP, size_t, int)
{
	// <Open MacTCP DNR or fail lookup>=                                       
 if (!Resolver())
 	return GUSISetHostError(NO_RECOVERY), static_cast<hostent *>(nil);
	// <Declare and initialize [[macHost]] and [[unixHost]]>=                  
 hostInfo		macHost;
 GUSIhostent &	unixHost = *fHost;

 for (int i=0; i<NUM_ALT_ADDRS; ++i)
 	macHost.addr[i] = 0;

	ip_addr addr = *(ip_addr *)addrP;
	if (addr == 0x7F000001)
		addr = static_cast<ip_addr>(gethostid());
	
	if (AddrToName(addr, &macHost, ResultUPP(&uDNRDone), 
			reinterpret_cast<Ptr>(GUSIContext::Current())
	 	) == cacheFault
	)
		do {
			GUSIContext::Yield(kGUSIBlock);
		} while (macHost.rtnCode == cacheFault);

	if (macHost.rtnCode)
		return GUSISetMacHostError(macHost.rtnCode), static_cast<hostent *>(nil);
	/* For some reason, the IP address usually seems to be set to 0 */
	if (!macHost.addr[0])
		macHost.addr[0] = addr;
	CopyHost(macHost, unixHost);
	
	return &unixHost;
}
// <Member functions for class [[GUSIMTNetDB]]>=                           
char * GUSIMTNetDB::inet_ntoa(in_addr inaddr)
{
	GUSIhostent &	unixHost = *fHost;
	GUSI_sprintf(unixHost.fAddrString, "%d.%d.%d.%d", 
		(inaddr.s_addr >> 24) & 0xFF,
		(inaddr.s_addr >> 16) & 0xFF,
		(inaddr.s_addr >> 8) & 0xFF,
		inaddr.s_addr & 0xFF);
	return unixHost.fAddrString;
}
// <Member functions for class [[GUSIMTNetDB]]>=                           
long GUSIMTNetDB::gethostid()
{
	return static_cast<long>(GUSIMTInetSocket::HostAddr());
}

// <GUSIDCon.cp>=                                                          
#include "GUSIInternal.h"
#include "GUSIDCon.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"

#include <fcntl.h>
#include <stddef.h>

#include <DCon.h>

// \section{Definition of [[GUSIDConSocket]]}                              
//                                                                         
// A [[GUSIDConSocket]] writes to the console or to a file.                
//                                                                         
// <Definition of class [[GUSIDConSocket]]>=                               
class GUSIDConSocket : 
	public GUSISocket
{
	char *	fLog;
public:
	GUSIDConSocket(const char * log);
	~GUSIDConSocket();
	// Read always returns EOF.                                                
 //                                                                         
 // <Overridden member functions for [[GUSIDConSocket]]>=                   
 virtual ssize_t	read(const GUSIScatterer & buffer);
 // Writes get translated into their DCon equivalents.                      
 //                                                                         
 // <Overridden member functions for [[GUSIDConSocket]]>=                   
 virtual ssize_t write(const GUSIGatherer & buffer);
 // DCon sockets implement simple calls.                                    
 //                                                                         
 // <Overridden member functions for [[GUSIDConSocket]]>=                   
 virtual bool Supports(ConfigOption config);
 // As the name says, DCon sockets are consolish.                           
 //                                                                         
 // <Overridden member functions for [[GUSIDConSocket]]>=                   
 virtual int	isatty();
};
// \section{Implementation of [[GUSIDConDevice]]}                          
//                                                                         
// You can use [[GUSIDConSocket]]s directly from C++, but the usual way to use them
// is to call [[GUSIwithDConSockets]] to have [["Dev:DCon"]] and [["Dev:DCon:xxx"]]
// mapped to them.                                                         
//                                                                         
// <Member functions for class [[GUSIDConDevice]]>=                        
extern "C" void GUSIwithDConSockets()
{
	GUSIDeviceRegistry::Instance()->AddDevice(GUSIDConDevice::Instance());
}
// <Member functions for class [[GUSIDConDevice]]>=                        
GUSIDConDevice * GUSIDConDevice::sInstance = nil;
// [[GUSIDConDevice]] will handle only the open request.                   
//                                                                         
// <Member functions for class [[GUSIDConDevice]]>=                        
bool GUSIDConDevice::Want(GUSIFileToken & file)
{
	if (!file.IsDevice())
		return false;
	
	const char * path = file.Path();
	
	return file.WhichRequest() == GUSIFileToken::kWillOpen
	 &&	file.StrFragEqual(path+4, "dcon")
	 && (!path[8] || (path[8] == ':' && path[9]));
}
// Open will never fail except for lack of memory.                         
//                                                                         
// <Member functions for class [[GUSIDConDevice]]>=                        
GUSISocket * GUSIDConDevice::open(GUSIFileToken & file, int)
{
	const char * path = file.Path();

	GUSISocket * sock = 
		path[8] ? new GUSIDConSocket(path+9) : new GUSIDConSocket(nil);
	if (!sock)
		GUSISetPosixError(ENOMEM);
	return sock;
}
// \section{Implementation of [[GUSIDConSocket]]}                          
//                                                                         
// The implementation of [[GUSIDConSocket]] is trivial.                    
//                                                                         
//                                                                         
// <Member functions for class [[GUSIDConSocket]]>=                        
GUSIDConSocket::GUSIDConSocket(const char * log)
	: fLog(nil)
{
	if (log)
		fLog = strcpy(new char[strlen(log)+1], log);
}
GUSIDConSocket::~GUSIDConSocket()
{
	delete fLog;
}
// <Member functions for class [[GUSIDConSocket]]>=                        
ssize_t GUSIDConSocket::read(const GUSIScatterer &)
{
	return 0;
}
// <Member functions for class [[GUSIDConSocket]]>=                        
ssize_t GUSIDConSocket::write(const GUSIGatherer & buffer)
{	
	dfprintf(fLog, "%.*s", buffer.Length(), buffer.Buffer());
	return buffer.Length();
}
// <Member functions for class [[GUSIDConSocket]]>=                        
bool GUSIDConSocket::Supports(ConfigOption config)
{
	return config == kSimpleCalls;
}
// <Member functions for class [[GUSIDConSocket]]>=                        
int GUSIDConSocket::isatty()
{ 
	return 1;
}

// <GUSINull.cp>=                                                          
#include "GUSIInternal.h"
#include "GUSINull.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"

#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>

#include <Devices.h>

// \section{Definition of [[GUSINullSocket]]}                              
//                                                                         
//                                                                         
// <Definition of class [[GUSINullSocket]]>=                               
class GUSINullSocket : 
	public GUSISocket
{
public:
	GUSINullSocket();
	// Read always returns EOF.                                                
 //                                                                         
 // <Overridden member functions for [[GUSINullSocket]]>=                   
 virtual ssize_t	read(const GUSIScatterer & buffer);
 // Writes always succeed.                                                  
 //                                                                         
 // <Overridden member functions for [[GUSINullSocket]]>=                   
 virtual ssize_t write(const GUSIGatherer & buffer);
 // Stat returns our special stat block.                                    
 //                                                                         
 // <Overridden member functions for [[GUSINullSocket]]>=                   
 virtual int	fstat(struct stat * buf);
 // Null sockets implement simple calls.                                    
 //                                                                         
 // <Overridden member functions for [[GUSINullSocket]]>=                   
 virtual bool Supports(ConfigOption config);
};
// \section{Implementation of [[GUSINullDevice]]}                          
//                                                                         
// You can use [[GUSINullSocket]]s directly from C++, but the usual way to use them
// is to call [[GUSIwithNullSockets]] to have [["Dev:Null"]] mapped to them.
//                                                                         
// <Member functions for class [[GUSINullDevice]]>=                        
extern "C" void GUSIwithNullSockets()
{
	GUSIDeviceRegistry::Instance()->AddDevice(GUSINullDevice::Instance());
}
// <Member functions for class [[GUSINullDevice]]>=                        
GUSINullDevice * GUSINullDevice::sInstance = nil;
// [[GUSINullDevice]] will handle only the open request.                   
//                                                                         
// <Member functions for class [[GUSINullDevice]]>=                        
bool GUSINullDevice::Want(GUSIFileToken & file)
{
	if (!file.IsDevice())
		return false;
	
	const char * path = file.Path();
	
	switch (file.WhichRequest()) {
	case GUSIFileToken::kWillOpen:
	case GUSIFileToken::kWillStat:
	 	return file.StrFragEqual(path+4, "null") && !path[8];
	default:
		return false;
	}
}
// Open will never fail except for lack of memory.                         
//                                                                         
// <Member functions for class [[GUSINullDevice]]>=                        
GUSISocket * GUSINullDevice::open(GUSIFileToken &, int)
{
	GUSISocket * sock = new GUSINullSocket;
	if (!sock)
		GUSISetPosixError(ENOMEM);
	return sock;
}

GUSISocket * GUSINullDevice::open()
{
	GUSISocket * sock = new GUSINullSocket;
	if (!sock)
		GUSISetPosixError(ENOMEM);
	return sock;
}
// The only original part o a null socket's reply to [[stat]] and [[fstat]] is the device/inode.
//                                                                         
// <Member functions for class [[GUSINullDevice]]>=                        
static int GUSINullStat(struct stat * buf)
{
	buf->st_dev			=	'dev ';
	buf->st_ino			=	'null';
	buf->st_mode		=	S_IFCHR | 0666 ;
	buf->st_nlink		=	1;
	buf->st_uid			=	0;
	buf->st_gid			=	0;
	buf->st_rdev		=	0;
	buf->st_size		=	1;
	buf->st_atime		=	time(NULL);
	buf->st_mtime		=	time(NULL);
	buf->st_ctime		=	time(NULL);
	buf->st_blksize		=	0;
	buf->st_blocks		=	1;
	
	return 0;
}

int GUSINullDevice::stat(GUSIFileToken &, struct stat * buf)
{
	return GUSINullStat(buf);
}
// \section{Implementation of [[GUSINullSocket]]}                          
//                                                                         
// The implementation of [[GUSINullSocket]] is trivial.                    
//                                                                         
//                                                                         
// <Member functions for class [[GUSINullSocket]]>=                        
GUSINullSocket::GUSINullSocket()
{
}
// <Member functions for class [[GUSINullSocket]]>=                        
ssize_t GUSINullSocket::read(const GUSIScatterer &)
{
	return 0;
}
// <Member functions for class [[GUSINullSocket]]>=                        
ssize_t GUSINullSocket::write(const GUSIGatherer & buffer)
{	
	return buffer.Length();
}
// <Member functions for class [[GUSINullSocket]]>=                        
int	GUSINullSocket::fstat(struct stat * buf)
{
	return GUSINullStat(buf);
}
// <Member functions for class [[GUSINullSocket]]>=                        
bool GUSINullSocket::Supports(ConfigOption config)
{
	return config == kSimpleCalls;
}

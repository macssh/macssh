// We try to distinguish between functions mentioned in the POSIX.1 standard,
// BSD functions which later became part of UNIX 98, and a few MPW specific 
// functions.                                                              
//                                                                         
// <GUSIPOSIX.cp>=                                                         
#include "GUSIInternal.h"
#include "GUSIPOSIX.h"
#include "GUSISocket.h"
#include "GUSIFactory.h"
#include "GUSIDevice.h"
#include "GUSIDescriptor.h"
#include "GUSIPipe.h"
#include "GUSIConfig.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"
#include "GUSINetDB.h"
#include "GUSITimer.h"

#include <LowMem.h>

// \section{Implementation of POSIX wrappers}                              
//                                                                         
// [[pipe]] is in fact a special case of [[socketpair]], but we bypass the domain 
// registry because pipe sockets are not installed as a domain.            
//                                                                         
// <POSIX function wrappers>=                                              
int pipe(int * fd)
{
	GUSIErrorSaver			saveError;
	GUSISocketFactory *		factory	= GUSIPipeFactory::Instance();
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
	GUSISocket * 			sock[2];
	
	if (!factory->socketpair(0, 0, 0, sock)) {
		if ((fd[0] = table->InstallSocket(sock[0])) > -1)
			if ((fd[1] = table->InstallSocket(sock[1])) > -1) {
				shutdown(fd[0], 1);
				shutdown(fd[1], 0);
				
				return 0;
			} else
				table->RemoveSocket(fd[0]);
		sock[0]->close();
		sock[1]->close();
	}
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}
// [[fsync]] synchronizes a socket                                         
//                                                                         
// <POSIX function wrappers>=                                              
int fsync(int s)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->fsync();
}
// [[close]] closes one file descriptor associated with a socket and deletes the 
// socket if this was the last desriptor.                                  
//                                                                         
// <POSIX function wrappers>=                                              
int close(int s)
{
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();

	return table->RemoveSocket(s);
}
// <POSIX function wrappers>=                                              
ssize_t read(int s, void *buffer, size_t buflen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;
	
	return sock->read(buffer, buflen);
}
// <POSIX function wrappers>=                                              
static ssize_t HandleWriteErrors(ssize_t retval)
{
	if (retval == -1)
		switch (errno) {
		case EINTR:
		case EWOULDBLOCK:
		case EINPROGRESS:
		case EALREADY:
			break;
		default:
			GUSIConfiguration::Instance()->BrokenPipe();
			break;
		}
	
	return retval;
}

ssize_t write(int s, const void *buffer, size_t buflen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return HandleWriteErrors(sock->write(buffer, buflen));
}
// All requests to [[fcntl]] except for [[F_DUPFD]] get dispatched to the socket.
//                                                                         
// <POSIX function wrappers>=                                              
int fcntl(int s, int cmd, ...)
{
	va_list arglist;
	va_start(arglist, cmd);
	
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	if (cmd == F_DUPFD) {
		GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();

		return table->InstallSocket(sock, va_arg(arglist, int));
	} else
		return sock->fcntl(cmd, arglist);
}
// [[dup]] and [[dup2]] are slight variations of the preceding.            
//                                                                         
// <POSIX function wrappers>=                                              
int dup(int s)
{
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();

	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return table->InstallSocket(sock);
}

int dup2(int s, int s1)
{
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();

	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	table->RemoveSocket(s1);
	return table->InstallSocket(sock, s1);
}
// <POSIX function wrappers>=                                              
int fstat(int s, struct stat * buf)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->fstat(buf);
}
// MacOS soon will work with 64 bit file offsets, so [[off_t]] is defined as a 
// 64 bit integer.                                                         
//                                                                         
// <POSIX function wrappers>=                                              
off_t lseek(int s, off_t offset, int whence)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->lseek(offset, whence);
}
// We also should define [[ttyname]] but that will have to wait.           
//                                                                         
// <POSIX function wrappers>=                                              
int isatty(int s)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->isatty();
}
// Sleeping is simple, computing the rest time is not. For convenience, we also
// special case sleep(0) as a yield.                                       
//                                                                         
// <POSIX function wrappers>=                                              
u_int sleep(u_int seconds) 
{	
	if (!seconds) {
		GUSIContext::Yield(kGUSIYield);
		
		return 0;
	}
	GUSITime start(GUSITime::Now());
	GUSITimer * timer = new GUSITimer;
	GUSIContext::CreateCurrent()->ClearWakeups();
	timer->Sleep(GUSITime(static_cast<uint32_t>(seconds), GUSITime::seconds).Get(GUSITime::msecs));
	GUSIContext::Yield(kGUSIBlock);
	delete timer;
	
	seconds -= (GUSITime::Now() - start).Get(GUSITime::seconds);
	
	return (seconds & 0x80000000) ? 0 : seconds;
}
// The next calls also need the services of [[GUSIDeviceRegistry]]. [[open]] used
// both the [[GUSIDeviceRegistry]] and the [[GUSIDescriptorTable]].        
//                                                                         
// <POSIX function wrappers>=                                              
int	open(const char * path, int mode, ...)
{
	GUSIErrorSaver			saveError;
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
	GUSISocket * 			sock;
	int						fd;
	
	if (sock = factory->open(path, mode)) {
		if ((fd = table->InstallSocket(sock)) > -1)
			return fd;
		sock->close();
	}
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}
// [[creat]] is just a special case for [[open]].                          
//                                                                         
// <POSIX function wrappers>=                                              
int	creat(const char * path, ...)
{
	return open(path, O_WRONLY | O_TRUNC | O_CREAT);
}
// [[remove]] remove a file.                                               
//                                                                         
// <POSIX function wrappers>=                                              
int	remove(const char * path)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->remove(path);
}
// [[unlink]] is the same as [[remove]].                                   
//                                                                         
// <POSIX function wrappers>=                                              
int	unlink(const char * path)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->remove(path);
}
// [[rename]] renames a file.                                              
//                                                                         
// <POSIX function wrappers>=                                              
int rename(const char * oldname, const char * newname)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->rename(oldname, newname);
}
// [[stat]] and [[lstat]], which ultimately resolve to the same function,  
// return information about the status of a file.                          
//                                                                         
// <POSIX function wrappers>=                                              
int stat(const char * path, struct stat * buf)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->stat(path, buf, false);
}

int lstat(const char * path, struct stat * buf)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->stat(path, buf, true);
}
// [[chmod]] changes file protection flags as well as this is possible under MacOS.
//                                                                         
// <POSIX function wrappers>=                                              
int chmod(const char * path, mode_t mode)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->chmod(path, mode);
}
// [[utime]] modifies a file's modification time.                          
//                                                                         
// <POSIX function wrappers>=                                              
int utime(const char * path, const utimbuf * times)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->utime(path, times);
}
// [[access]] checks the access permissions for a file.                    
//                                                                         
// <POSIX function wrappers>=                                              
int access(const char * path, int mode)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->access(path, mode);
}
// [[mkdir]] and [[rmdir]] create and delete a directory, respectively.    
//                                                                         
// <POSIX function wrappers>=                                              
int	mkdir(const char * path, ...)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->mkdir(path);
}

int rmdir(const char * path)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->rmdir(path);
}
// [[opendir]] returns an instance of a derived class of [[GUSIDirectory]].
//                                                                         
// <POSIX function wrappers>=                                              
typedef GUSIDirectory * GUSIDirPtr;

DIR * opendir(const char * path)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	GUSIDirectory * dir = factory->opendir(path);
	
	return dir ? reinterpret_cast<DIR *>(new GUSIDirPtr(dir)) : 0;
}
// The other directory functions then dispatch on the [[GUSIDirectory]]    
// without needing the [[GUSIDeviceRegistry]]. [[readdir]] reads the next  
// directory entry.                                                        
//                                                                         
// <POSIX function wrappers>=                                              
dirent * readdir(DIR * dir)
{
	return GUSIDirPtr(*dir)->readdir();
}
// [[telldir]] saves the current directory position, [[seekdir]] sets it, rewinddir
// sets it to the beginning.                                               
//                                                                         
// <POSIX function wrappers>=                                              
long telldir(DIR * dir)
{
	return GUSIDirPtr(*dir)->telldir();
}

void seekdir(DIR * dir, long pos)
{
	GUSIDirPtr(*dir)->seekdir(pos);
}

void rewinddir(DIR * dir)
{
	GUSIDirPtr(*dir)->seekdir(1);
}
// [[closedir]] closes the directory stream.                               
//                                                                         
// <POSIX function wrappers>=                                              
int closedir(DIR * dir)
{
	delete GUSIDirPtr(*dir);
	delete dir;
	
	return 0;
}
// [[chdir]] changes the default directory.                                
//                                                                         
// <POSIX function wrappers>=                                              
int chdir(const char * dir)
{
	GUSIFileSpec	directory(dir);
	
	if (directory.Error())
		return GUSISetMacError(directory.Error());	
	else if (!directory.Exists())
		return GUSISetPosixError(ENOENT);
	
	(++directory).SetDefaultDirectory();
	
	return 0;
} 
// [[getcwd]] returns the path to the default directory.                   
//                                                                         
// <POSIX function wrappers>=                                              
char *getcwd(char * buf, size_t size)
{
	GUSIFileSpec	cwd;
	char * 			res;
	
	if (cwd.GetDefaultDirectory())
		return GUSISetMacError(cwd.Error()), static_cast<char *>(nil);
		
	res = (--cwd).FullPath();
	
	if (size < strlen(res)+1)
		return GUSISetPosixError(size > 0 ? ERANGE : EINVAL), 
			static_cast<char *>(nil);
	if (!buf && !(buf = (char *) malloc(size)))
		return GUSISetPosixError(ENOMEM), static_cast<char *>(nil);

	strcpy(buf, res);
	
	return buf;
}
// [[time]] returns the time in seconds since 1904. The MSL version uses 1970; the
// present version conforms more to my Mac traditionalistic outlook. You will see an
// unpleasant number of [[reinterpret_cast]] expressions in the following routines
// because some of our compilers want [[time_t]] to be [[long]] and some want it to be 
// [[unsigned long]].                                                      
//                                                                         
// <POSIX function wrappers>=                                              
time_t time(time_t * timer)
{
	time_t t;
	
	if (!timer) 
		timer = &t;
		
	GetDateTime(reinterpret_cast<unsigned long*>(timer));
	
	return *timer;
}
// [[gettimeofday]] returns a somewhat more accurate time than [[time]].   
//                                                                         
// <POSIX function wrappers>=                                              
int	gettimeofday(struct timeval * tv, struct timezone * tz)
{
	*tv = (timeval)GUSITime::Now();
	if (tz)
		*tz = GUSITime::Zone();
	
	return 0;
}
// [[localtime]] returns the local time in a broken down record, [[gmtime]] does the
// same with UTC time. Our versions measure from 1904 and are thread safe. 
//                                                                         
// <POSIX function wrappers>=                                              
extern "C" void GUSIKillTM(void * t) 
{
	delete reinterpret_cast<tm *>(t);
}

static tm * get_tm()
{
	static GUSISpecificData<tm, GUSIKillTM>	sTM;
	
	return sTM.get();
}

struct tm * localtime(const time_t * timer)
{
	tm * t = get_tm();
	*t = GUSITime(static_cast<unsigned long>(*timer), GUSITime::seconds);
	t->tm_isdst = GUSITime::Zone().tz_dsttime != 0;	
	return t;
}

struct tm * gmtime(const time_t * timer)
{
	tm * t = get_tm();
	*t = GUSITime(static_cast<unsigned long>(*timer), GUSITime::seconds).Local2GMTime();
	t->tm_isdst = 0;	
	return t;
}
// [[mktime]] parses a [[struct tm]].                                      
//                                                                         
// <POSIX function wrappers>=                                              
time_t mktime(struct tm *timeptr)
{
	return static_cast<time_t>(GUSITime(*timeptr).UGet(GUSITime::seconds));
}
// \section{Implementation of Socket wrappers}                             
//                                                                         
// [[getdtablesize]] returns the size of the descriptor table.             
//                                                                         
// <Socket function wrappers>=                                             
int getdtablesize()
{
	return GUSIDescriptorTable::SIZE;
}
// [[socket]] creates a socket and installs it in the descriptor table.    
//                                                                         
// <Socket function wrappers>=                                             
int socket(int domain, int type, int protocol)
{
	GUSIErrorSaver			saveError;
	GUSISocketFactory *		factory	= GUSISocketDomainRegistry::Instance();
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
	GUSISocket * 			sock;
	int						fd;
	
	if (sock = factory->socket(domain, type, protocol)) {
		if ((fd = table->InstallSocket(sock)) > -1)
			return fd;
		sock->close();
	}
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}
// [[socketpair]] works similar to [[socket]], although the opportunity for problems
// is a bit greater.                                                       
//                                                                         
// <Socket function wrappers>=                                             
int socketpair(int domain, int type, int protocol, int * sv)
{
	GUSIErrorSaver			saveError;
	GUSISocketFactory *		factory	= GUSISocketDomainRegistry::Instance();
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
	GUSISocket * 			sock[2];
	
	if (!factory->socketpair(domain, type, protocol, sock)) {
		if ((sv[0] = table->InstallSocket(sock[0])) > -1)
			if ((sv[1] = table->InstallSocket(sock[1])) > -1)
				return 0;
			else
				table->RemoveSocket(sv[0]);
		sock[0]->close();
		sock[1]->close();
	}
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}
// [[bind]] binds a socket to a name.                                      
//                                                                         
// <Socket function wrappers>=                                             
int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;
	return sock->bind((void *) name, namelen);
}
// [[connect]] connects a socket to a named peer.                          
//                                                                         
// <Socket function wrappers>=                                             
int connect(int s, const struct sockaddr *addr, socklen_t addrlen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;
	return sock->connect((void *) addr, addrlen);
}
// [[listen]] puts a socket in passive mode.                               
//                                                                         
// <Socket function wrappers>=                                             
int listen(int s, int qlen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;
	return sock->listen(qlen);
}
// [[accept]] accepts a connection from another socket.                    
//                                                                         
// <Socket function wrappers>=                                             
int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();

	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	if (sock = sock->accept(addr, addrlen))
		if ((s = table->InstallSocket(sock)) != -1)
			return s;
		else
			sock->close();

	return -1;
}
// <Socket function wrappers>=                                             
ssize_t readv(int s, const struct iovec *iov, int count)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->read(GUSIScatterer(iov, count));
}
// <Socket function wrappers>=                                             
ssize_t recv(int s, void *buffer, size_t buflen, int flags)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	socklen_t 	fromlen 	=	0;

	return sock->recvfrom(buffer, buflen, flags, nil, &fromlen);
}
// <Socket function wrappers>=                                             
ssize_t recvfrom(
	int s, void *buffer, size_t buflen, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->recvfrom(buffer, buflen, flags, from, fromlen);
}
// <Socket function wrappers>=                                             
ssize_t recvmsg(int s, struct msghdr *msg, int flags)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->recvmsg(msg, flags);
}
// <Socket function wrappers>=                                             
ssize_t writev(int s, const struct iovec *iov, int count)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return HandleWriteErrors(sock->write(GUSIGatherer(iov, count)));
}
// <Socket function wrappers>=                                             
ssize_t send(int s, const void *buffer, size_t buflen, int flags)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return HandleWriteErrors(sock->sendto(buffer, buflen, flags, nil, 0));
}
// <Socket function wrappers>=                                             
ssize_t sendto(
	int s, const void *buffer, size_t buflen, int flags, const struct sockaddr *to, socklen_t tolen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return HandleWriteErrors(sock->sendto(buffer, buflen, flags, to, tolen));
}
// <Socket function wrappers>=                                             
ssize_t sendmsg(int s, const struct msghdr *msg, int flags)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return HandleWriteErrors(sock->sendmsg(msg, flags));
}
// [[select]] is quite complex, so we break it up. [[select_once]] polls all 
// file descriptors once.                                                  
//                                                                         
// <Socket function wrappers>=                                             
static int select_once(int width,
	fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	fd_set *readres, fd_set *writeres, fd_set *exceptres)
{
	bool 	r, w, e;
	bool *	canRead;
	bool *	canWrite;
	bool *	canExcept;
	int		count = 0;
	
	for (int s = 0; s < width ; ++s)  {
		canRead = (readfds && FD_ISSET(s,readfds)) ? &r : nil;
		canWrite = (writefds && FD_ISSET(s,writefds)) ? &w : nil;
		canExcept = (exceptfds && FD_ISSET(s,exceptfds)) ? &e : nil;
		if (canRead || canWrite || canExcept) {
			GUSISocket * sock = GUSIDescriptorTable::LookupSocket(s);
			
			if (!GUSI_ASSERT_EXTERNAL(sock, ("Socket %d closed in select\n", s)))
				return count ? count : -1;
				
			r = w = e = false;
			if (sock->select(canRead, canWrite, canExcept))
				count += (canRead && *canRead)+(canWrite && *canWrite)+(canExcept && *canExcept);
			if (r)
				FD_SET(s,readres);
			if (w)
				FD_SET(s,writeres);
			if (e)
				FD_SET(s,exceptres);
		}
	}
	
	return count;
}
// [[select_sleep]] sleeps as long as [[canSleep]] allows.                 
//                                                                         
// <Socket function wrappers>=                                             
static bool select_sleep(bool canSleep)
{
	if (canSleep) {
		return GUSIContext::Yield(kGUSIBlock);
	} else {
		return GUSIContext::Yield(kGUSIPoll);
	}
}
// [[select_forever]] keeps calling [[select_once]] until one of the file  
// descriptors triggers.                                                   
//                                                                         
// <Socket function wrappers>=                                             
static int select_forever(bool canSleep, int width, 
	fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	fd_set *readres, fd_set *writeres, fd_set *exceptres)
{
	int 		count;
	
	for (;;) {
		count = 
			select_once(width, 
				readfds, writefds, exceptfds,
				readres, writeres, exceptres);
		if (count)
			break;
		if (select_sleep(canSleep))
			return GUSISetPosixError(EINTR);
	} 
	
	return count;			
}
// [[select_timed]] keeps calling [[select_once]] until one of the file    
// descriptors triggers or the timer runs out.                             
//                                                                         
// <Socket function wrappers>=                                             
static int select_timed(bool canSleep, int width,
	fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	fd_set *readres, fd_set *writeres, fd_set *exceptres,
	struct timeval *timeout)
{
	int 		count;
	GUSITimer	timer;
	
	timer.MicroSleep(GUSITime(*timeout).Get(GUSITime::usecs));
	for (;;) {
		count = 
			select_once(width, 
				readfds, writefds, exceptfds,
				readres, writeres, exceptres);
		if (count || timer.Expired())
			break;
		if (select_sleep(canSleep))
			return GUSISetPosixError(EINTR);
	}
	
	return count;			
}
// Even so, [[select]] is still a heavyweight.                             
//                                                                         
// <Socket function wrappers>=                                             
int select(int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	bool		canSleep	= true;
	int 		count 		= 0;
	fd_set 		readres;	FD_ZERO(&readres);
	fd_set 		writeres;	FD_ZERO(&writeres);
	fd_set 		exceptres;	FD_ZERO(&exceptres);
	
	// <Check that all specified file descriptors are valid or [[return -1]]>= 
 for (int s = 0; s < width ; ++s)
 	if (	(readfds && FD_ISSET(s,readfds))
 		||	(writefds && FD_ISSET(s,writefds))
 		||	(exceptfds && FD_ISSET(s,exceptfds))
 	)
 		if (!GUSIDescriptorTable::LookupSocket(s))
 			return -1;
	// <Call [[pre_select]] for all file descriptors and determine [[canSleep]]>=
 for (int s = 0; s < width ; ++s)
 	if (GUSISocket * sock = GUSIDescriptorTable::LookupSocket(s)) {
 		bool r = readfds && FD_ISSET(s,readfds);
 		bool w = writefds && FD_ISSET(s,writefds);
 		bool e = exceptfds && FD_ISSET(s,exceptfds);

 		if (r || w || e)
 			canSleep = sock->pre_select(r, w, e) && canSleep;
 	}
	if (!timeout) {
		count = 
			select_forever(canSleep, width,
				readfds, writefds, exceptfds,
				&readres, &writeres, &exceptres);
	} else if (timeout->tv_sec || timeout->tv_usec) {
		count = 
			select_timed(canSleep, width,
				readfds, writefds, exceptfds,
				&readres, &writeres, &exceptres,
				timeout);
	} else {
		count = 
			select_once(width,
				readfds, writefds, exceptfds,
				&readres, &writeres, &exceptres);
		GUSIContext::Yield(kGUSIYield);
	}
	// This loop is pretty much guaranteed to set [[errno]] to [[EBADF]], so we 
 // save the actual error code and restore it (this is harmless if no error 
 // occurred).                                                              
 //                                                                         
 // <Call [[post_select]] for all file descriptors>=                        
 int saveErrno = errno;
 for (int s = 0; s < width ; ++s)
 	if (GUSISocket * sock = GUSIDescriptorTable::LookupSocket(s)) {
 		bool r = readfds && FD_ISSET(s,readfds);
 		bool w = writefds && FD_ISSET(s,writefds);
 		bool e = exceptfds && FD_ISSET(s,exceptfds);

 		if (r || w || e)
 			sock->post_select(r, w, e);
 	}
 errno = saveErrno;
	// When copying back descriptor sets, we have to be careful not to copy more
 // words than covered by [[width]], because Perl depends on that by allocating
 // fake [[fd_sets]]. I personally think that is a bad idea, but staying    
 // compatible doesn't cost too much.                                       
 //                                                                         
 // <Copy internal descriptor sets to parameters>=                          
 int nwords = ((width+31) >> 3) & ~3;
 if (readfds)
 	memcpy(readfds, &readres, nwords);
 if (writefds)
 	memcpy(writefds, &writeres, nwords);
 if (exceptfds)
 	memcpy(exceptfds, &exceptres, nwords);	

	return count;
}
// <Socket function wrappers>=                                             
int getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->getsockname(name, namelen);
}

int getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->getpeername(name, namelen);
}
// <Socket function wrappers>=                                             
int shutdown(int s, int how)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->shutdown(how);
}
// Bit of trivia: [[ioctl]] is actually not a POSIX function.              
//                                                                         
// <Socket function wrappers>=                                             
int ioctl(int s, unsigned long request, ...)
{
	va_list arglist;
	va_start(arglist, request);

	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;
	
	return sock->ioctl(request, arglist);
}
// The length argument should eventually become a [[socklen_t]].           
//                                                                         
// <Socket function wrappers>=                                             
int getsockopt(int s, int level, int optname, void *optval, socklen_t * optlen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->getsockopt(level, optname, optval, optlen);
}
// <Socket function wrappers>=                                             
int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock->setsockopt(level, optname, (void *) optval, optlen);
}
// <Socket function wrappers>=                                             
int ftruncate(int s, off_t offset)
{
	// Many of the other routines are quite stereotypical: They translate their file
 // descriptor argument to a [[GUSISocket]] and dispatch the call to it.    
 //                                                                         
 // <Translate descriptor [[s]] to [[GUSISocket * sock]] or return [[-1]]>= 
 GUSISocket * sock	= GUSIDescriptorTable::LookupSocket(s);

 if (!sock)
 	return -1;

	return sock ? sock->ftruncate(offset) : -1;
}
// [[truncate]] is implemented in terms of [[ftruncate]].                  
//                                                                         
// <Socket function wrappers>=                                             
int truncate(const char * path, off_t offset)
{
	int fd = open(path, O_RDWR);
	if (fd < 0)
		return fd;
		
	int res = ftruncate(fd, offset);
	
	close(fd);
	
	return res;
}
// While [[sleep]] is a POSIX function, [[usleep]] is not.                 
//                                                                         
// <Socket function wrappers>=                                             
int usleep(u_int useconds)
{	
	if (!useconds) {
		GUSIContext::Yield(kGUSIYield);
		
		return 0;
	}
	GUSITimer * timer = new GUSITimer;
	GUSIContext::CreateCurrent()->ClearWakeups();
	timer->MicroSleep(useconds);
	GUSIContext::Yield(kGUSIBlock);
	delete timer;
	
	return 0;
}
// [[symlink]] creates a symbolic link to a file.                          
//                                                                         
// <Socket function wrappers>=                                             
int symlink(const char * path, const char * linkto)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->symlink(path, linkto);
}
// [[readlink]] reads the contents of a symbolic link.                     
//                                                                         
// <Socket function wrappers>=                                             
int readlink(const char * path, char * buf, int bufsize)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->readlink(path, buf, bufsize);
}
// [[inet_aton]] and [[inet_addr]] convert an address string to an internet address.
//                                                                         
// <Socket function wrappers>=                                             
int inet_aton(const char * addr, struct in_addr * ina)
{
	in_addr_t a = inet_addr(addr);
	
	if (a == INADDR_NONE)
		return 0;
	if (ina) 
		ina->s_addr = a;
	
	return 1;
}
// <Socket function wrappers>=                                             
in_addr_t inet_addr(const char * addr)
{
	return GUSINetDB::Instance()->inet_addr(addr);
}
// <Socket function wrappers>=                                             
char *inet_ntoa(in_addr addr)
{
	return GUSINetDB::Instance()->inet_ntoa(addr);
}
// <Socket function wrappers>=                                             
long gethostid()
{
	return GUSINetDB::Instance()->gethostid();
}
// <Socket function wrappers>=                                             
int gethostname(char *machname, int buflen)
{
	return GUSINetDB::Instance()->gethostname(machname, buflen);
}
// <Socket function wrappers>=                                             
void setprotoent(int stayopen)
{
	GUSINetDB::Instance()->setprotoent(stayopen);
}
// <Socket function wrappers>=                                             
void setservent(int stayopen)
{
	GUSINetDB::Instance()->setservent(stayopen);
}
// <Socket function wrappers>=                                             
void endprotoent()
{
	GUSINetDB::Instance()->endprotoent();
}
// <Socket function wrappers>=                                             
void endservent()
{
	GUSINetDB::Instance()->endservent();
}
// <Socket function wrappers>=                                             
hostent	*gethostbyaddr(const void * addr, size_t size, int family)
{
	return GUSINetDB::Instance()->gethostbyaddr(addr, size, family);
}
// <Socket function wrappers>=                                             
hostent	*gethostbyname(const char * name)
{
	return GUSINetDB::Instance()->gethostbyname(name);
}
// <Socket function wrappers>=                                             
protoent *getprotobyname(const char * name)
{
	return GUSINetDB::Instance()->getprotobyname(name);
}
// <Socket function wrappers>=                                             
protoent *getprotobynumber(int proto)
{
	return GUSINetDB::Instance()->getprotobynumber(proto);
}
// <Socket function wrappers>=                                             
protoent *getprotoent()
{
	return GUSINetDB::Instance()->getprotoent();
}
// <Socket function wrappers>=                                             
servent	*getservbyname(const char * name, const char * proto)
{
	return GUSINetDB::Instance()->getservbyname(name, proto);
}
// <Socket function wrappers>=                                             
servent	*getservbyport(int port, const char * proto)
{
	return GUSINetDB::Instance()->getservbyport(port, proto);
}
// <Socket function wrappers>=                                             
servent	*getservent()
{
	return GUSINetDB::Instance()->getservent();
}
// \section{Implementation of MPW wrappers}                                
//                                                                         
// [[fgetfileinfo]] and [[fsetfileinfo]] manipulate the MacOS type/creator codes.
//                                                                         
// <MPW function wrappers>=                                                
int fgetfileinfo(const char * path, OSType * creator, OSType * type)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->fgetfileinfo(path, creator, type);
}

void fsetfileinfo(const char * path, OSType creator, OSType type)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	factory->fsetfileinfo(path, creator, type);
}
// [[faccess]] manipulates MPW properties of files.                        
//                                                                         
// <MPW function wrappers>=                                                
int faccess(const char * path, unsigned * cmd, void * arg)
{
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	
	return factory->faccess(path, cmd, arg);
}

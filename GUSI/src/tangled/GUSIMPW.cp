// <GUSIMPW.cp>=                                                           
#include "GUSIInternal.h"
#include "GUSIMPW.h"
#include "GUSIDevice.h"
#include "GUSIDescriptor.h"
#include "GUSIMacFile.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"
#include "GUSITimer.h"
#include "GUSIConfig.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <CursorCtl.h>

#include <CodeFragments.h>

// \section{Interfacing to MPW library routines}                           
//                                                                         
// As mentioned above, our interface strategy differs for 68K and PPC code. We try
// to handle this with a macro.                                            
//                                                                         
// <MPW library functions>=                                                
extern "C" {
#if !defined(powerc) && !defined(__powerc)
#define DECL_stdlib(name, pname, ret, args) ret name args;
#ifdef __MWERKS__
#pragma pointers_in_D0
#endif
// Now we only have to declare the list once, and can reuse it numerous times.
//                                                                         
// <Declarations for MPW functions>=                                       
DECL_stdlib(MPW_open, "\popen", int, (const char * name, int flags))
DECL_stdlib(MPW_close, "\pclose", int, (int s))
DECL_stdlib(MPW_read, "\pread", int, (int s, char *buffer, unsigned buflen))
DECL_stdlib(MPW_write, "\pwrite", int, (int s, char *buffer, unsigned buflen))
DECL_stdlib(MPW_fcntl, "\pfcntl", int, (int s, unsigned int cmd, int arg))
DECL_stdlib(MPW_ioctl, "\pioctl", int, (int d, unsigned int request, long *argp))
DECL_stdlib(MPW_lseek, "\plseek", long, (int fd, long offset, int whence))
DECL_stdlib(MPW_faccess, "\pfaccess", int, (char *fileName, unsigned int cmd, long * arg))
DECL_stdlib(MPW_getenv, "\pgetenv", char *, (const char *env))
#ifdef __MWERKS__
#pragma pointers_in_A0
#endif
static bool ConnectToMPWLibrary()   { return true; }
#ifdef __MWERKS__
extern int _mpwerrno;
static void UpdateMPWErrno()   		{ errno = _mpwerrno; }
#else
static void UpdateMPWErrno()   		{  }
#endif
#else
#define DECL_stdlib(name, pname, ret, args) ret (*name) args;
// Now we only have to declare the list once, and can reuse it numerous times.
//                                                                         
// <Declarations for MPW functions>=                                       
DECL_stdlib(MPW_open, "\popen", int, (const char * name, int flags))
DECL_stdlib(MPW_close, "\pclose", int, (int s))
DECL_stdlib(MPW_read, "\pread", int, (int s, char *buffer, unsigned buflen))
DECL_stdlib(MPW_write, "\pwrite", int, (int s, char *buffer, unsigned buflen))
DECL_stdlib(MPW_fcntl, "\pfcntl", int, (int s, unsigned int cmd, int arg))
DECL_stdlib(MPW_ioctl, "\pioctl", int, (int d, unsigned int request, long *argp))
DECL_stdlib(MPW_lseek, "\plseek", long, (int fd, long offset, int whence))
DECL_stdlib(MPW_faccess, "\pfaccess", int, (char *fileName, unsigned int cmd, long * arg))
DECL_stdlib(MPW_getenv, "\pgetenv", char *, (const char *env))
int *	MPW_errno;
static void UpdateMPWErrno()   		{ errno = *MPW_errno; }
// On PowerPC, we have to connect to the dynamic library (which, in principle,
// can fail).                                                              
//                                                                         
// <Connecting to the StdCLib code fragment>=                              
static bool sConnected;

static void DoConnectToMPWLibrary()
{
	CFragConnectionID 	StdCLib;
	CFragSymbolClass	symClass;
	Ptr 				whoCares;
	Str255				error;

	if (GetSharedLibrary(
		StringPtr("\pStdCLib"), kPowerPCCFragArch, kLoadCFrag, &StdCLib, &whoCares, error)
	)
		return;

	if (FindSymbol(StdCLib, "\perrno", (Ptr *) &MPW_errno, &symClass))
		goto failed;
#undef DECL_stdlib
#define DECL_stdlib(name, pname, ret, args) \
	if (FindSymbol(StdCLib, pname, (Ptr *) &name, &symClass)) \
		goto failed;
	// Now we only have to declare the list once, and can reuse it numerous times.
 //                                                                         
 // <Declarations for MPW functions>=                                       
 DECL_stdlib(MPW_open, "\popen", int, (const char * name, int flags))
 DECL_stdlib(MPW_close, "\pclose", int, (int s))
 DECL_stdlib(MPW_read, "\pread", int, (int s, char *buffer, unsigned buflen))
 DECL_stdlib(MPW_write, "\pwrite", int, (int s, char *buffer, unsigned buflen))
 DECL_stdlib(MPW_fcntl, "\pfcntl", int, (int s, unsigned int cmd, int arg))
 DECL_stdlib(MPW_ioctl, "\pioctl", int, (int d, unsigned int request, long *argp))
 DECL_stdlib(MPW_lseek, "\plseek", long, (int fd, long offset, int whence))
 DECL_stdlib(MPW_faccess, "\pfaccess", int, (char *fileName, unsigned int cmd, long * arg))
 DECL_stdlib(MPW_getenv, "\pgetenv", char *, (const char *env))
	
	sConnected = true;
	
	return;
	
failed:
#undef DECL_stdlib
#define DECL_stdlib(name, pname, ret, args) name = 0;

	// Now we only have to declare the list once, and can reuse it numerous times.
 //                                                                         
 // <Declarations for MPW functions>=                                       
 DECL_stdlib(MPW_open, "\popen", int, (const char * name, int flags))
 DECL_stdlib(MPW_close, "\pclose", int, (int s))
 DECL_stdlib(MPW_read, "\pread", int, (int s, char *buffer, unsigned buflen))
 DECL_stdlib(MPW_write, "\pwrite", int, (int s, char *buffer, unsigned buflen))
 DECL_stdlib(MPW_fcntl, "\pfcntl", int, (int s, unsigned int cmd, int arg))
 DECL_stdlib(MPW_ioctl, "\pioctl", int, (int d, unsigned int request, long *argp))
 DECL_stdlib(MPW_lseek, "\plseek", long, (int fd, long offset, int whence))
 DECL_stdlib(MPW_faccess, "\pfaccess", int, (char *fileName, unsigned int cmd, long * arg))
 DECL_stdlib(MPW_getenv, "\pgetenv", char *, (const char *env))
}

static bool ConnectToMPWLibrary()
{
	if (!sConnected)
		DoConnectToMPWLibrary();
	return sConnected;
}
#endif
}
// \section{Definition of [[GUSIMPWSocket]]}                               
//                                                                         
// A [[GUSIMPWSocket]] is a proxy class for an MPW file descriptor.        
//                                                                         
//                                                                         
// <Declaration of class [[GUSIMPWSocket]]>=                               
class GUSIMPWSocket : public GUSISocket {
public:
	~GUSIMPWSocket();
	
	// Some member functions are fairly trivial wrappers.                      
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 ssize_t	read(const GUSIScatterer & buffer);
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 ssize_t write(const GUSIGatherer & buffer);
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual off_t lseek(off_t offset, int whence);
 // [[fcntl]] and [[ioctl]] have to get their final arguments from [[va_lists]].
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual int	fcntl(int cmd, va_list arg);
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual int	ioctl(unsigned int request, va_list arg);
 // [[ftruncate]] translates into an [[ioctl]] request.                     
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual int ftruncate(off_t offset);
 // Since we know we're running on a pseudodevice, we can pass on that fact.
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual int	fstat(struct stat * buf);
 // And we also know we're a TTY.                                           
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual int	isatty();
 // We have no choice but to be optimistic and claim that we are always ready for 
 // reading and writing.                                                    
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWSocket]]>=              
 virtual bool	select(bool * canRead, bool * canWrite, bool * exception);
protected:
	friend class GUSIMPWDevice;
	
	GUSIMPWSocket(int fd);
private:
	int		fFD;
};
// \section{Definition of [[GUSIMPWDevice]]}                               
//                                                                         
// A [[GUSIMPWDevice]] is a singleton class supporting the standard [[open]] and
// [[faccess]] operations and a domain specific [[stdopen]] operation to support
// opening already-open descriptors.                                       
//                                                                         
// <Declaration of class [[GUSIMPWDevice]]>=                               
class GUSIMPWDevice : public GUSIDevice {
public:
	static GUSIMPWDevice *	Instance();

	// [[GUSIMPWDevice]] is prepared to handle an [[open]] on a limited set of device names
 // and on all files.                                                       
 //                                                                         
 // <Overridden member functions for class [[GUSIMPWDevice]]>=              
 virtual bool Want(GUSIFileToken & file);
 // <Overridden member functions for class [[GUSIMPWDevice]]>=              
 virtual GUSISocket * open(GUSIFileToken &, int flags);
	
	GUSISocket * stdopen(int fd, int flags);
private:
	GUSIMPWDevice() 								{}
	
	static GUSIMPWDevice *	sInstance;
};
// \section{Implementation of [[GUSIMPWSocket]]}                           
//                                                                         
// A [[GUSIMPWSocket]] acts as a wrapper for a file descriptor from the MPW library.
//                                                                         
// <Member functions for class [[GUSIMPWSocket]]>=                         
GUSIMPWSocket::GUSIMPWSocket(int fd) 
	: fFD(fd)
{
}
GUSIMPWSocket::~GUSIMPWSocket()
{
	MPW_close(fFD);
	UpdateMPWErrno();
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
ssize_t GUSIMPWSocket::read(const GUSIScatterer & buffer)
{
	GUSIStdioFlush();
	GUSIConfiguration::Instance()->AutoSpin();
	int res = MPW_read(fFD, (char *) buffer.Buffer(), (unsigned)buffer.Length());
	if (res < 0)
		UpdateMPWErrno();
	return buffer.SetLength(res);
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
ssize_t GUSIMPWSocket::write(const GUSIGatherer & buffer)
{
	GUSIConfiguration::Instance()->AutoSpin();
	int res = MPW_write(fFD, (char *) buffer.Buffer(), (unsigned)buffer.Length());
	if (res < 0)
		UpdateMPWErrno();
	return res;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
off_t GUSIMPWSocket::lseek(off_t offset, int whence)
{
	off_t res = MPW_lseek(fFD, offset, (long)whence);
	if (res < 0)
		UpdateMPWErrno();
	return res;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
int GUSIMPWSocket::fcntl(int cmd, va_list arg)
{
	int res = MPW_fcntl(fFD, cmd, va_arg(arg, int));
	if (res < 0)
		UpdateMPWErrno();
	return res;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
int GUSIMPWSocket::ioctl(unsigned int request, va_list arg)
{
	int res = MPW_ioctl(fFD, request, va_arg(arg, long *));
	if (res < 0)
		UpdateMPWErrno();
	return res;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
int GUSIMPWSocket::ftruncate(off_t offset)
{
	int res = MPW_ioctl(fFD, FIOSETEOF, (long *) offset);
	if (res < 0)
		UpdateMPWErrno();
	return res;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
int	GUSIMPWSocket::fstat(struct stat * buf)
{
	GUSISocket::fstat(buf);
	buf->st_mode =	S_IFCHR | 0666;
	
	return 0;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
int GUSIMPWSocket::isatty()
{ 
	return 1;
}
// <Member functions for class [[GUSIMPWSocket]]>=                         
bool GUSIMPWSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		cond = *canRead = true;
	if (canWrite)
		cond = *canWrite = true;
		
	return cond;
}
// \section{Implementation of [[GUSIMPWDevice]]}                           
//                                                                         
// [[GUSIMPWDevice]] is a singleton class.                                 
//                                                                         
// <Member functions for class [[GUSIMPWDevice]]>=                         
GUSIMPWDevice * GUSIMPWDevice::sInstance;
// <Member functions for class [[GUSIMPWDevice]]>=                         
GUSIMPWDevice * GUSIMPWDevice::Instance()
{
	if (!sInstance)
		sInstance = new GUSIMPWDevice();
	return sInstance;
}
// <Member functions for class [[GUSIMPWDevice]]>=                         
bool GUSIMPWDevice::Want(GUSIFileToken & file)
{
	switch (file.WhichRequest()) {
	case GUSIFileToken::kWillOpen:
		return !file.IsDevice() || (file.StrStdStream(file.Path()) > -1);
	default:
		return false;
	}
}
// [[open]] translates the file flags, opens the file, and passes the resulting 
// descriptor to  [[stdopen]].                                             
//                                                                         
// The values of some of the flags that MPW uses differ a bit from the ones used
// in our headers.                                                         
//                                                                         
// <Member functions for class [[GUSIMPWDevice]]>=                         
#define MPW_O_RDONLY	 0 		/* Bits 0 and 1 are used internally */
#define MPW_O_WRONLY	 1 		/* Values 0..2 are historical */
#define MPW_O_RDWR 		 2		/* NOTE: it goes 0, 1, 2, *!* 8, 16, 32, ... */
#define MPW_O_APPEND	(1<< 3)	/* append (writes guaranteed at the end) */
#define MPW_O_RSRC 		(1<< 4)	/* Open the resource fork */
#define MPW_O_ALIAS		(1<< 5)	/* Open alias file */
#define MPW_O_CREAT		(1<< 8)	/* Open with file create */
#define MPW_O_TRUNC		(1<< 9)	/* Open with truncation */
#define MPW_O_EXCL 		(1<<10) /* w/ O_CREAT:  Exclusive "create-only" */
#define MPW_O_BINARY	(1<<11) /* Open as a binary stream */
#define MPW_O_NRESOLVE	(1<<14)	/* Don't resolve any aliases */
// [[TranslateOpenFlags]] translates the header flags into the MPW flags.  
//                                                                         
// <Member functions for class [[GUSIMPWDevice]]>=                         
static int TranslateOpenFlags(int mode)
{
	int mpwMode;
	
	switch (mode & 3) {
	case O_RDWR:
		mpwMode = MPW_O_RDWR;
		break;
	case O_RDONLY:
		mpwMode = MPW_O_RDONLY;
		break;
	case O_WRONLY:
		mpwMode = MPW_O_WRONLY;
		break;
	}
	if (mode & O_APPEND)
		mpwMode |= MPW_O_APPEND;
	if (mode & O_CREAT)
		mpwMode |= MPW_O_CREAT;
	if (mode & O_EXCL)
		mpwMode |= MPW_O_EXCL;
	if (mode & O_TRUNC)
		mpwMode |= MPW_O_TRUNC;
	
	return mpwMode;
}
// <Member functions for class [[GUSIMPWDevice]]>=                         
extern int StandAlone;

GUSISocket * GUSIMPWDevice::open(GUSIFileToken & file, int flags)
{
	if (!ConnectToMPWLibrary())
		return GUSISetPosixError(ENOEXEC), static_cast<GUSISocket *>(nil);

	// To ensure that our fancy path handling gets applied to all complex paths, but MPW gets
 // to do its magic on [[Dev:]] paths, we normalize paths if they contain more than 1 colon.
 //                                                                         
 // <Normalize [[path]] for MPW>=                                           
 const char * path	= file.Path();
 const char * colon	= strchr(path, ':');
 if (colon && strchr(colon, ':'))
 	path = file.RelativePath();
	int fd 	= MPW_open(path, TranslateOpenFlags(flags));
	
	if (fd == -1) {
		UpdateMPWErrno();
		return static_cast<GUSISocket *>(nil);
	} else if (!file.IsDevice() && !StandAlone && MPW_ioctl(fd, FIOINTERACTIVE, nil) == -1) {
		MPW_close(fd);
		return GUSIMacFileDevice::Instance()->open(file, flags & ~(O_CREAT | O_EXCL));
	} else
		return stdopen(fd, flags);
}
// [[stdopen]] handles the GUSI side of the opening.                       
//                                                                         
// <Member functions for class [[GUSIMPWDevice]]>=                         
GUSISocket * GUSIMPWDevice::stdopen(int fd, int flags)
{
	if (!ConnectToMPWLibrary())
		return GUSISetPosixError(ENOEXEC), static_cast<GUSISocket *>(nil);

	// Our support of MacOS files is far superiour to our MPW console support, so
 // whenever we find that in fact we're talking to a real file, we switch to
 // using a [[MacFileSocket]] instead. This whole parocedure does not apply to
 // SIOW applications: The initial sockets 0, 1, and 2 get closed right away and
 // calling anything else on them would be counterproductive.               
 //                                                                         
 // <Open and return a [[MacFileSocket]] if possible>=                      
 short	fRef;
 	
 if (!StandAlone 
  && MPW_ioctl(fd, FIOINTERACTIVE, nil) == -1
  && MPW_ioctl(fd, FIOREFNUM, (long *) &fRef) != -1
 ) {
 	static short 		sOutFRef = 0;
 	static GUSISocket *	sOutSocket;
 	
 	MPW_close(fd); 
 	if (fd == 1) {
 		sOutFRef 	= fRef;
 		return sOutSocket = GUSIMacFileDevice::Instance()->open(fRef, flags);	
 	} else if (fd == 2 && fRef == sOutFRef) {
 		// Standard output and error redirected to same file
 		return sOutSocket;
 	} else
 		return GUSIMacFileDevice::Instance()->open(fRef, flags);
 }
	
	GUSISocket * sock = new GUSIMPWSocket(fd);
	
	return sock ? sock : (GUSISetPosixError(ENOMEM), static_cast<GUSISocket *>(nil));
}
// <MPW implementation of [[GUSIGetEnv]]>=                                 
char *GUSIGetEnv(const char * name)
{
	if (!ConnectToMPWLibrary())
		return static_cast<char *>(nil);
	return MPW_getenv(name);
}
// <MPW spin hook>=                                                        
void GUSIMPWSpin(bool wait)
{
	static GUSITimer	sSpinDue(false);

	GUSIConfiguration::Instance()->AutoInitGraf();
	
	if (sSpinDue.Expired()) {
		RotateCursor(32);
		sSpinDue.Sleep(125, true);
	} else if (wait)
		GUSIHandleNextEvent(600);
}
// As opposed to an application, an MPW tool connects to the three standard
// descriptors on startup.                                                 
//                                                                         
// <MPW implementation of [[GUSISetupConsoleDescriptors]]>=                
void GUSISetupConsoleDescriptors()
{
	GUSIMPWDevice *       mpw  	= GUSIMPWDevice::Instance();
	GUSIDescriptorTable * table = GUSIDescriptorTable::Instance();
	
	GUSIDeviceRegistry::Instance()->AddDevice(mpw);
	
	if (!(*table)[0]) {
		table->InstallSocket(mpw->stdopen(0, O_RDONLY));
		table->InstallSocket(mpw->stdopen(1, O_WRONLY));
		table->InstallSocket(mpw->stdopen(2, O_WRONLY));
	}

	GUSISetHook(GUSI_EventHook+mouseDown,	(GUSIHook)-1);
	GUSISetHook(GUSI_SpinHook, 				(GUSIHook)GUSIMPWSpin);
}

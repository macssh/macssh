/*
 * GUSIPatches.cp
 * (c) 2000 Jean-Pierre Stierlin.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "wind.h"

#include "dialog_resrcdefs.h"
#include "event.proto.h"
#include "memory.proto.h"
#include "netevent.proto.h"
#include "network.proto.h"

#include "GUSITTY.h"

#include <GUSIInternal.h>
#include <GUSIBasics.h>
#include <GUSIContext.h>
#include <GUSIConfig.h>
#include <GUSIDiag.h>
#include <GUSISocket.h>
#include <GUSIPThread.h>
#include <GUSIFactory.h>
#include <GUSIDevice.h>
#include <GUSIDescriptor.h>
#include <GUSIOTNetDB.h>
#include <GUSISIOUX.h>

#include <console.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

#include <LowMem.h>

#include <MoreFilesExtras.h>
#include <FullPath.h>

/*#include "ssh2.h"*/ /* lsh source code doesn't like cplusplus... */

#ifdef __cplusplus
extern "C" {
#endif

void ssh2_init();
void ssh2_sched();

char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *parID);

int open(const char * path, int mode, ...);
int dup(int s);
int dup2(int s, int s1);
int socket(int domain, int type, int protocol);
int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int close(int s);

void ssh2_doevent(long sleepTime);
void add_one_file(struct lshcontext *context, int fd);
void remove_one_file(struct lshcontext *context, int fd);

#ifdef __cplusplus
}
#endif

extern pthread_key_t ssh2threadkey;
extern int g_error_fd;

/*
 * ssh2_init
 */

void ssh2_init()
{
	static Boolean		sGUSISetup = false;

	if ( !sGUSISetup ) {
		GUSIContext::Setup(true);

		/* this call initializes the resolver with current context */
		gethostid();

		/*GUSISetupConsole();*/
		g_error_fd = open("dev:ttyerr", O_WRONLY );

		sGUSISetup = true;
	}
}

/*
 * sched_yield cannot be called without checking
 * for GUSIContext::Current which can be called
 * only from C++, hence ssh2_sched...
 */

void ssh2_sched()
{
	if (GUSIContext::Current() != NULL) {
		sched_yield();
	}
}

/*
 * getprefsd return the full path of prefs directory
 */

char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *dirID)
{
	OSErr			err;
	FSSpec			fileSpec;
	Boolean			isFolder;
	Boolean			wasAlias;
	Boolean			isDirectory;
	short			fullPathLength;
	short			pathLength;
	Handle			fullPath;

	*vRefNum = 0;
	*dirID = 0;
	err = FindFolder(kOnSystemDisk, kPreferencesFolderType, true, &fileSpec.vRefNum, &fileSpec.parID);
	if (err) {
		return NULL;
	}
	fileSpec.name[0] = strlen(name);
	BlockMoveData(name, fileSpec.name + 1, fileSpec.name[0]);
	err = ResolveAliasFile(&fileSpec, true, &isFolder, &wasAlias);
	err = FSpGetDirectoryID(&fileSpec, dirID, &isDirectory);
	if (err == fnfErr) {
		err = FSpDirCreate(&fileSpec, smSystemScript, dirID);
		if (err != noErr) {
			return NULL;
		}
		isDirectory = true;
	}
	if (err != noErr || !isDirectory) {
		return NULL;
	}
	err = FSpGetFullPath(&fileSpec, &fullPathLength, &fullPath);
	if (err != noErr) {
		return NULL;
	}
	pathLength = fullPathLength;
	if ((*fullPath)[pathLength - 1] == ':' )
		--pathLength;
	if (pathLength > size - 2) {
		// FIXME: better error message
		DisposeHandle(fullPath);
		return NULL;
	}
	*vRefNum = fileSpec.vRefNum;
	BlockMoveData(*fullPath, buf, pathLength);
	buf[pathLength++] = ':';
	buf[pathLength] = 0;
	DisposeHandle(fullPath);
	return buf;

/*
	GUSIFileSpec	prefs(kPreferencesFolderType, kOnSystemDisk);
	char * 			res;
	char * 			out = buf;
	const GUSICatInfo *info;

	if (prefs.Error())
		return GUSISetMacError(prefs.Error()), static_cast<char *>(nil);

	prefs.SetName(name);
	
	// resolve path name in case it's an alias
	prefs.Resolve( true );

	res = prefs.FullPath();

	if (size < strlen(res)+1)
		return GUSISetPosixError(size > 0 ? ERANGE : EINVAL), 
			static_cast<char *>(nil);
	if (!out && !(out = (char *) malloc(size)))
		return GUSISetPosixError(ENOMEM), static_cast<char *>(nil);

	strcpy(out, res);

	if ( (access( out, R_OK ) == 0 || mkdir( out ) == 0) && (info = prefs.CatInfo()) != nil ) {
		*vRefNum = info->DirInfo().ioVRefNum;
		*dirID = info->DirInfo().ioDrDirID;
	} else {



		if (!buf)
			free(out);
		return static_cast<char *>(nil);
	}
	return out;
*/
}

/*
 * default GUSIHandleNextEvent is GUSI's main event loop
 * we use Telnet's own.
 */

void GUSIHandleNextEvent(long sleepTime)
{
	ssh2_doevent( sleepTime );
}


/* we don't use SIOUX event handler */
/*
GUSISIOUXSocket::GUSISIOUXSocket() 
{
	InstallConsole(0);
}
*/

/*
 * default GUSISIOUXSocket::select checks for keyDown or
 * autoKey events from MacOS's EventQueue.
 * we use an internal buffer.
 */

class GUSISIOUXSocket : public GUSISocket {
public:
	~GUSISIOUXSocket();
	
	ssize_t	read(const GUSIScatterer & buffer);
	ssize_t write(const GUSIGatherer & buffer);
	virtual int	ioctl(unsigned int request, va_list arg);
	virtual int	fstat(struct stat * buf);
	virtual int	isatty();
	bool select(bool * canRead, bool * canWrite, bool *);

	static GUSISIOUXSocket *	Instance();
private:
	static GUSISIOUXSocket *	sInstance;
	
	GUSISIOUXSocket();
};

bool GUSISIOUXSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;

	if (canRead) {
		if (*canRead = (AvailableFromTTY(0, (lshcontext *)pthread_getspecific(ssh2threadkey))))
			cond = true;
	}
	if (canWrite)
		cond = *canWrite = true;
	return cond;
}

/*
 * default GUSIProcess::Yield has 20 ticks to remain in same state
 * we use 0.
 */

/* GUSI 2.1.3
void GUSIProcess::Yield(GUSIYieldMode wait)
{
	if (wait == kGUSIBlock) {
		fWillSleep = true;
		if (fReadyThreads > 1 || fDontSleep)
			wait = kGUSIYield;
	}
	if (wait == kGUSIYield && LMGetTicks() - fResumeTicks < 0) {
		GUSI_SMESSAGE("Skip WNE\n");
		return;
	}
	if (fClosing)
		fClosing->CheckClose();
	if (gGUSISpinHook) {
		gGUSISpinHook(wait == kGUSIBlock);
	} else {
		GUSI_SMESSAGE("Suspend\n");
		GUSIHandleNextEvent(wait == kGUSIBlock ? 600 : 0);
		GUSI_SMESSAGE("Resume\n");
	} 
		
	fWillSleep 		= false;
	fDontSleep 		= false;
	fResumeTicks 	= LMGetTicks();
}
*/

/* GUSI 2.1.5 */
void GUSIProcess::Yield(GUSIYieldMode wait)
{
	if (wait == kGUSIBlock) {
		fWillSleep = true;
		if (fReadyThreads > 1 || fDontSleep) {
			GUSI_SMESSAGE("Don't Sleep\n");
			wait = kGUSIYield;
		}
	}
	if (fExistingThreads < 2) // Single threaded process skips sleep only once
		fDontSleep = false;
	if (wait == kGUSIYield && LMGetTicks() - fResumeTicks < 0) {
		fWillSleep 		= false;
		return;
	}
	if (gGUSISpinHook) {
		gGUSISpinHook(wait == kGUSIBlock);
	} else {
		GUSI_SMESSAGE("Suspend\n");
		GUSIHandleNextEvent(wait == kGUSIBlock ? 60 : 0);
		GUSI_SMESSAGE("Resume\n");
	} 
	if (fExistingThreads < 2) 		// Single threaded process skips sleep only once
		fDontSleep = false;
	fWillSleep 		= false;
	fResumeTicks 	= LMGetTicks();
	if (fClosing)
		fClosing->CheckClose();
}


/*
 * default GUSIContext::Yield has 12 ticks to remain in the same state
 * we use 0.
 */


bool GUSIContext::Yield(GUSIYieldMode wait)
{
	if (wait == kGUSIYield && LMGetTicks() - sCurrentContext->fEntryTicks < 0)
		return false;	

	bool			mainThread	= sCurrentContext->fThreadID == kApplicationThreadID;
	bool 			block		= wait == kGUSIBlock && !mainThread;
	GUSIProcess	*	process 	= GUSIProcess::Instance();
	bool 			interrupt 	= false;
	
	do {
		if (mainThread)
			process->Yield(wait);
				
		if (interrupt = Raise())
			goto done;
				
		if (sHasThreading) {
			if (block)
				SetThreadState(kCurrentThreadID, kStoppedThreadState, kNoThreadID);
			else
				YieldToAnyThread();
		}
	} while (wait == kGUSIBlock && !sCurrentContext->fWakeup);
done:
	sCurrentContext->fWakeup = false;
	
	return interrupt;
}


/*
 * The fSvc field of the GUSIOTNetDB instance is no longer valid after
 * an interface switch in the TCP/IP control panel.
 * Let's clear it upon kOTProviderWillClose message.
 */

/* moved to GUSIPatch2.cp : 68k version no longer links otherwise */
/*
// <Asynchronous notifier function for [[GUSIOTNetDB]]>=                   
inline uint32_t CompleteMask(OTEventCode code)	
{ 	
	return 1 << (code & 0x1F); 
}

// I do that cause I'm using my hacked GUSI for recent Universal Headers : GUSI
// is not yet ready to compile for this target. (alexp)
#if UNIVERSAL_INTERFACES_VERSION >= 0x0334
pascal void GUSIOTNetDBNotify(
	void *contextPtr, OTEventCode code, OTResult result, void *cookie)
{
	GUSIOTNetDB * netdb = static_cast<GUSIOTNetDB *>(contextPtr);
#else
pascal void GUSIOTNetDBNotify(
	GUSIOTNetDB * netdb, OTEventCode code, OTResult result, void *cookie)
{
#endif
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
		if (code != kOTProviderWillClose)
			result = 0;
		if (code == kOTProviderIsClosed) {
			OTCloseProvider(netdb->fSvc);
			netdb->fSvc = static_cast<InetSvcRef>(NULL);
			netdb->fCreationContext = static_cast<GUSIContext *>(NULL);
		}
		break;
	}
	if (result)
		netdb->fAsyncError = result;
	context->Wakeup();
}
*/

/*
 * we need to track open()/dup()/close()/socket() calls to close files/sockets
 * upon abort/exit
 */

/*
 * open.
 */

int	open(const char * path, int mode, ...)
{
	GUSIErrorSaver			saveError;
	GUSIDeviceRegistry *	factory	= GUSIDeviceRegistry::Instance();
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
	GUSISocket * 			sock;
	int						fd;
	
	if (sock = factory->open(path, mode)) {
		if ((fd = table->InstallSocket(sock)) > -1) {
			lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
			if ( context ) {
				add_one_file(context, fd);
			}
			return fd;
		}
		sock->close();
	}
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}

/*
 * dup.
 */

int dup(int s)
{
	GUSIDescriptorTable		*table	= GUSIDescriptorTable::Instance();
	GUSISocket				*sock	= GUSIDescriptorTable::LookupSocket(s);
	int						fd;
	lshcontext				*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if (!sock)
		return -1;

	fd = table->InstallSocket(sock);
	if ( context ) {
		add_one_file(context, fd);
	}
	return fd;
}

/*
 * dup2.
 */

int dup2(int s, int s1)
{
	GUSIDescriptorTable		*table	= GUSIDescriptorTable::Instance();
	GUSISocket				*sock	= GUSIDescriptorTable::LookupSocket(s);
	int						fd;
	lshcontext				*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if (!sock)
		return -1;

	table->RemoveSocket(s1);
	fd = table->InstallSocket(sock, s1);
	if ( context && s1 != fd ) {
		remove_one_file(context, s1);
		add_one_file(context, fd);
	}
	return fd;
}

/*
 * socket.
 */

int socket(int domain, int type, int protocol)
{
	GUSIErrorSaver			saveError;
	GUSISocketFactory *		factory	= GUSISocketDomainRegistry::Instance();
	GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
	GUSISocket * 			sock;
	int						fd;
	
	if (sock = factory->socket(domain, type, protocol)) {
		if ((fd = table->InstallSocket(sock)) > -1) {
			lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
			if ( context ) {
				add_one_file(context, fd);
			}
			return fd;
		}
		sock->close();
	}
	if (!errno)
		return GUSISetPosixError(ENOMEM);
	else
		return -1;
}

/*
 * accept.
 */

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	GUSIDescriptorTable		*table = GUSIDescriptorTable::Instance();
	GUSISocket				*sock= GUSIDescriptorTable::LookupSocket(s);
	lshcontext				*context = (lshcontext *)pthread_getspecific(ssh2threadkey);

	if (!sock)
		return -1;

	if (sock = sock->accept(addr, addrlen)) {
		if ((s = table->InstallSocket(sock)) != -1) {
			if ( context ) {
				add_one_file(context, s);
			}
			return s;
		} else {
			sock->close();
		}
	}
	return -1;
}

/*
 * close.
 */

int close(int s)
{
	if ( s > STDERR_FILENO ) {
		GUSIDescriptorTable *	table	= GUSIDescriptorTable::Instance();
		lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
		if ( context ) {
			remove_one_file(context, s);
		}
		return table->RemoveSocket(s);
	} else {
		/* we don't close stdin/stdout/stderr */
		return 0;
	}
}


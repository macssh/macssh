/*
 * GUSIPatch2.cp
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

#include <GUSIInternal.h>
#include <GUSINetDB.h>
#include <GUSIOTNetDB.h>
#include <GUSIDescriptor.h>
#include <GUSISocket.h>
#include <GUSIPOSIX.h>
#include <GUSITimer.h>

#include <pthread.h>
#include <sys/time.h>
#include <setjmp.h>

/*
 * we don't use the /etc/services files from prefs...
 */

class GUSIFileServiceDB : public GUSIServiceDB {
public:
	static GUSIFileServiceDB * Instance();
protected:
	FILE *			fFile;
	pthread_mutex_t	fLock;
	
	GUSIFileServiceDB(FILE * file) 
		: fFile(file), fLock(0) { }
	
	virtual void	Reset();
	virtual void 	Next();
};


GUSIFileServiceDB * GUSIFileServiceDB::Instance()
{
	return static_cast<GUSIFileServiceDB *>(nil);
}

/*
 * The fSvc field of the GUSIOTNetDB instance is no longer valid after
 * an interface switch in the TCP/IP control panel.
 * Let's clear it upon kOTProviderWillClose message.
 */

/* no longer needed: GUSI 2.1.6b2 fixes this */
/*
// <Asynchronous notifier function for [[GUSIOTNetDB]]>=                   
inline uint32_t CompleteMask(OTEventCode code)	
{ 	
	return 1 << (code & 0x1F); 
}

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
		if (code != kOTProviderWillClose) {
			result = 0;
		} else {
			netdb->fCreationContext = static_cast<GUSIContext *>(NULL);
		}
		break;
	}
	if (result)
		netdb->fAsyncError = result;
	context->Wakeup();
}

// <Member functions for class [[GUSIOTNetDB]]>=                           
bool GUSIOTNetDB::Resolver()
{
	if (!fCreationContext) {
		fCreationContext = GUSIContext::Current();
		if (!GUSIOTFactory::Initialize())
			return false;
		if (fSvc) {
			OTCloseProvider(fSvc);
			fSvc = static_cast<InetSvcRef>(NULL);
		}
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
*/

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

extern "C" jmp_buf *getexitbuf();
extern "C" void *setexitbuf(jmp_buf *exitbuf);

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

	// we must delete the timer before exiting current thread
	// otherwise, bad things occur.
	jmp_buf *oldexitbuf = getexitbuf();
	jmp_buf exitbuf;
	setexitbuf(&exitbuf);
	int result;
	{
		GUSITimer timer;
		if ( !(result = setjmp(exitbuf)) ) {
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
		}
	}
	setexitbuf(oldexitbuf);
	if ( result ) {
		longjmp( *oldexitbuf, result );
	}
/*
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
*/
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

	// we must call post_select before exiting current thread
	// to remove the contexts. otherwise, bad things occur.
	jmp_buf *oldexitbuf = getexitbuf();
	jmp_buf exitbuf;
	setexitbuf(&exitbuf);
	int result;
	if ( !(result = setjmp(exitbuf)) ) {

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

	}

	// <Call [[post_select]] for all file descriptors>=                        

 // this is needed because next loop always exits with errno = EBADF
 // which overwrite potential EINTR signal.
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

	setexitbuf(oldexitbuf);
	if ( result ) {
		longjmp( *oldexitbuf, result );
	}

	// <Copy internal descriptor sets to parameters>=                          
 if (readfds)
 	*readfds = readres;
 if (writefds)
 	*writefds = writeres;
 if (exceptfds)
 	*exceptfds = exceptres;	

	return count;
}

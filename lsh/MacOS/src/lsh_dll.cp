#include "lshprefix.h"
#include "lsh_context.h"

#include <GUSIInternal.h>
#include <GUSIFileSpec.h>
#include <GUSIDescriptor.h>
#include <GUSISocket.h>
#include <GUSIPOSIX.h>
#include <GUSITimer.h>

#include <pthread.h>
#include <sys/time.h>
#include <setjmp.h>
#include <sched.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *dirID);

void ssh2_sched();

void ssh2_doevent(EventRecord *theEvent, long sleepTime);

#ifdef __cplusplus
}
#endif


/*
 * getprefsd return the full path of prefs directory
 */

char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *dirID)
{
	GUSIFileSpec	prefs(kPreferencesFolderType, kOnSystemDisk);
	char * 			res;
	char * 			out = buf;
	const GUSICatInfo *info;

	if (prefs.Error())
		return GUSISetMacError(prefs.Error()), static_cast<char *>(nil);

	prefs.SetName(name);
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

/*
 * ssh2_sched
 */

void ssh2_sched()
{
	sched_yield();
}

/*
 * GUSIHandleNextEvent
 */

void GUSIHandleNextEvent(long sleepTime)
{
	ssh2_doevent(NULL, sleepTime);
}

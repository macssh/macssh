#include "lshprefix.h"
#include "lsh_context.h"

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

#include <pthread.h>
#include <sched.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef __CONSOLE__
#include <console.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void ssh2_sched();

Boolean can_read();
void ssh2_doevent(long sleepTime);
char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *dirID);

#ifdef __cplusplus
}
#endif


/*
 * default GUSIHandleNextEvent is GUSI's main event loop
 */

void GUSIHandleNextEvent(long sleepTime)
{
	ssh2_doevent( sleepTime );
}


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
		if (*canRead = can_read())
			cond = true;
	}
	if (canWrite)
		cond = *canWrite = true;
	return cond;
}

/* we don't use SIOUX event handler */
GUSISIOUXSocket::GUSISIOUXSocket() 
{
	InstallConsole(0);
}


/*
 * ssh2_sched
 */

void ssh2_sched()
{
	sched_yield();
}



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


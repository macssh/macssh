#include "lshprefix.h"
#include "lsh_context.h"

#include <GUSIInternal.h>
#include <GUSIFileSpec.h>

#include <pthread.h>
#include <sched.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

void ssh2_sched();

char *getprefsd(char *name, char *buf, size_t size, short *vRefNum, long *dirID);

#ifdef __cplusplus
}
#endif


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


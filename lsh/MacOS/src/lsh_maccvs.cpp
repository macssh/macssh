/*
 * GUSIPatches.cp
 */

#include "lshprefix.h"
#include "lsh_context.h"

#include <pthread.h>
#include <sched.h>

#include "GUSIMSLSetup.h"

#ifdef __cplusplus
extern "C" {
#endif

void ssh2_init();

void add_one_file(struct lshcontext *context, int fd);
void remove_one_file(struct lshcontext *context, int fd);

#ifdef __cplusplus
}
#endif

static void myGusiMSLAddFile(int fd)
{
	lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	if ( context ) {
		add_one_file(context, fd);
	}
}

static void myGusiMSLRemoveFile(int fd)
{
	lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
	if ( context ) {
		remove_one_file(context, fd);
	}
}

/*
 * ssh2_init
 */

void ssh2_init()
{
	static Boolean		sGUSISetup = false;

	if ( !sGUSISetup ) {
		GusiMSLSetAddFile(myGusiMSLAddFile);
		GusiMSLSetRemoveFile(myGusiMSLRemoveFile);
	}
}


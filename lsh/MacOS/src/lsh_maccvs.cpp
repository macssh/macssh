/*
 * GUSIPatches.cp
 */

#include "lshprefix.h"
#include "lsh_context.h"

#include <pthread.h>
#include <console.h>
#include <sched.h>

#include "GUSIMSLSetup.h"

#ifdef __cplusplus
extern "C" {
#endif

void GUSIwithTTYSockets();
void GUSIwithoutTTYSockets();

void ssh2_init();
void ssh2_terminate();

#ifdef __cplusplus
}
#endif

/*
 * - This project is intended to work with MacCvs (http://www.cvsgui.org)
 * - It exports the lsh program as a shared library, thanks to Jean-Pierre.
 * - The lsh API is hooked up to the cvs shared library of cvs and provide
 * the SSH2 authentication.
 *
 * Questions : alexandre parenteau (aubonbeurre@hotmail.com)
 */

/*
 * ssh2_init
 */

static Boolean		sGUSISetup = false;

void ssh2_init()
{

	if ( !sGUSISetup ) {

		GUSIwithTTYSockets();

		sGUSISetup = true;
	}
}

void ssh2_terminate()
{
	if ( sGUSISetup ) {

		GUSIwithoutTTYSockets();

		sGUSISetup = false;
	}
}

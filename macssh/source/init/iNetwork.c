// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// iNetwork.c
// General network initialization routines
// Called only by init.c

// NCSA Revisions:
// 7/92		Telnet 2.6:	Initial version.  Jim Browne

#include "iNetwork.proto.h"

#include "InternalEvents.h"
#include "network.proto.h"	        // For Mnetinit proto
#include "netevent.proto.h"	        // For Stask & setblocksize proto
#include "bkgr.proto.h"			    // For StartUpFTP proto
extern QHdr		gEventsQueue, gEventsFreeQueue;
extern short	gQueueLength;

extern void GUSISetupConfig();

/*************************************************************************/
/* neteventinit
*  load up the pointers for the event queue
*  makes a circular list to follow, required for error messages
*/
void neteventinit( void)
{
	gEventsFreeQueue.qHead = 0;
	gEventsFreeQueue.qTail = 0;
	gEventsFreeQueue.qFlags = 0;
	gEventsQueue.qHead = 0;
	gEventsQueue.qTail = 0;
	gEventsQueue.qFlags = 0;
	
	ChangeQueueLength(NEVENTS);
	gQueueLength = NEVENTS;
}

/* initnet - init all of the network stuff... */
void initnet( void)
{
	neteventinit();							/* initializes for error messages to count */
	networkUPPinit();		//initialize UPP's for TCP callbacks
//	Mnetinit();
	StartUpFTP();
}

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// mainseg.c

#define GLOBALMASTER			/* Global structures are charged to us... see below */
#include "event.proto.h"
#include "netevent.proto.h"
#include "init.proto.h"
#include "network.proto.h"		// For netshut proto
#include "wind.h"
#include "MacBinary.h"
#include "binsubs.proto.h"			// For close_mb_files proto
#include "menuseg.proto.h"
#include "Connections.proto.h"
#include "AddressXlation.h"
//the following are for the unload protos
#include "vgtek.proto.h"		
#include "vr.h"					
#include "vr.proto.h"			
#include "rsinterf.proto.h"		
#include "vsem.proto.h"			
#include "configure.proto.h"	
#include "macros.proto.h"		
#include "bkgr.proto.h"			
#include "printing.proto.h"		
#include "sets.proto.h"		
#include "rg0.proto.h"	
#include "rgmp.proto.h"		
#include "parse.proto.h"
#include "linemode.proto.h"	
#include "tnae.h"	
#include "authencrypt.proto.h"
#include "mainseg.proto.h"
#include "IConfig.proto.h"		
#include "ae.proto.h"			
#include "prefs.proto.h"
#include "Appearance.h"

//#define	PROFILER						//	Define to use MW profiler
#ifdef	PROFILER
#include 	"profiler.h"
#endif

Cursor *theCursors[NUMCURS];		/* all the cursors in a nice bundle */

WindRec
	*screens,		/* Window Records (VS) for :	Current Sessions */
	*ftplog;

short scrn=0;

TelInfoRec	*TelInfo;			/* This is defined here and externed everywhere else. */

Boolean gHasAppearance = false;
Boolean gHasAppearance11 = false;
Boolean haveNotifiedLowMemory = FALSE;

Boolean			gPresentOpenConnectionDialog;
Boolean			gAutoOpenDefault;
unsigned long	gPresentOpenConnectionTicks;

extern int gMovableModal;

extern void ssh2_sched();
extern Boolean gThreadModal;

void	main(void)
{
	short	i;
	long	dummy;
	long	lastCount = 0;
	Boolean memOK;
	MaxApplZone();
	
	for (i=0; i<9; i++)
		MoreMasters();
		
#ifdef	PERFORMANCE
	ThePGlobals = nil;
	if (!InitPerf(&ThePGlobals, 10, 8, TRUE, TRUE, "\pCODE", 0, "\p", FALSE, 0, 0, 0))
		Debugger();
	PerfControl(ThePGlobals, TRUE);
#endif
#ifdef	PROFILER
	if (ProfilerInit(collectDetailed, microsecondsTimeBase, 1000, 30) != noErr)
		DebugStr("\pError starting profiler!");
//	ProfilerSetStatus(false);
	ProfilerClear();
#endif

	if ( !Gestalt('appr', &dummy) ) {
		gHasAppearance = true;
		if ( !Gestalt(gestaltAppearanceVersion, &dummy) && dummy >= 0x110 ) {
			gHasAppearance11 = true;
		}
		RegisterAppearanceClient();
	}

	init();					/* JMB 2.6 - Call all init routines */
	UnloadSeg(&init);		/* Bye, bye to init routines... 	*/

/*
	if ( gApplicationPrefs->autoOpenDefault )
		OpenSpecial(3);
*/

	TelInfo->gotDocument = 0;

	do {						/* BYU - Do this forever */
		UnloadSegments();
		if (!gThreadModal && !gMovableModal) {
			DoEvents(NULL);
		}
		ssh2_sched();
		if (!TelInfo->done) {
			DoNetEvents();
		}
		memOK = RecoverReserveMemory();
		if ( memOK ) {
			haveNotifiedLowMemory = false;
		} else if (!haveNotifiedLowMemory) {
			Alert(MemoryLowAlert, NULL);
			haveNotifiedLowMemory = true;
		}

		if ( gPresentOpenConnectionDialog || gAutoOpenDefault ) {
			if ( !TelInfo->gotDocument ) {
				if (LMGetTicks() - gPresentOpenConnectionTicks > 30) {
					if ( gPresentOpenConnectionDialog ) {
						gPresentOpenConnectionDialog = 0;
						PresentOpenConnectionDialog();
					} else {
						gAutoOpenDefault = 0;
						OpenSpecial(3);
					}
				}
			} else {
				gPresentOpenConnectionDialog = 0;
				gAutoOpenDefault = 0;
			}
		}

	} while ( !TelInfo->done );

#ifdef	PERFORMANCE
	if (!PerfDump(ThePGlobals, "\pPerform.out", TRUE, 80))
		DebugStr("\pDump Failed");
	Debugger(); 
	TermPerf(ThePGlobals);
#endif
#ifdef	PROFILER
	if (ProfilerDump("\pProfiler.out") != noErr) {
		DebugStr("\pDump Failed");
		}
	ProfilerTerm();
#endif
}

void	quit( void)
{
	if (TelInfo->numwindows>0)
		return;

	if (TelInfo->startedTCP) { // RAB BetterTelnet 1.0fc4
		netshut();
		CloseResolver();			/* BYU 2.4.16 */
	} // RAB BetterTelnet 1.0fc4

	close_mb_files();			/* BYU - Don't leave any files open! */
	stopInternetConfig();
	TelInfo->done = 1;			/* BYU */
}

void	forcequit(void)
{
	quit();
	ExitToShell();
}

//	This is rather simple right now, but I'll make it smarter when I have the time.
//void	CheckFreeMemory(void)
//{
//	long	space, block;
	
//	PurgeSpace(&space, &block);
	
//	if (space < (30 * 1024))
//		Alert(MemoryLowAlert, NULL);
//}

//	This unloads the segments that are safe to unload.  Right now, the DNR and network
//	segments ARE NOT safe to unload.  Do _not_ add anything to this unless you really
//	know what you are doing!  (I MEAN IT!)
void	UnloadSegments(void)
{
	UnloadSeg(&TEKMAINunload);
	UnloadSeg(&ICRunload);
	UnloadSeg(&CONFIGUREunload);
	UnloadSeg(&MACROSunload);
	UnloadSeg(&FTPServerUnload);
	UnloadSeg(&PrintingUnload);
	UnloadSeg(&SETSunload);
	UnloadSeg(&TEKNULLunload);
	UnloadSeg(&TEKMacPicunload);
	UnloadSeg(&LinemodeUnload);
	UnloadSeg(&ICUnload);
	UnloadSeg(&AEunload);
	UnloadSeg(&PREFSUnload);
}
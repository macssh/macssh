// <GUSISIOW.cp>=                                                          
#include "GUSIInternal.h"
#include "GUSISIOW.h"
#include "GUSIBasics.h"
#include "GUSIDescriptor.h"

#include <Events.h>
#include <Windows.h>

extern "C" {
// The activate and suspend/resume handlers are very similar and both ultimately call
// [[_DoActivate]]. For reasons that I don't quite remember anymore, we are a bit more
// conservative about treating the current port than SIOW itself is.       
//                                                                         
// <Prototypes for internal SIOW functions>=                               
void _DoActivate(WindowPtr win, int activate);
// As a slight extra complication, not all OS events are suspend/resume events.
//                                                                         
// <Prototypes for internal SIOW functions>=                               
void _DoIdle();
// The update handler similarly calls [[_DoUpdate]].                       
//                                                                         
// <Prototypes for internal SIOW functions>=                               
void _DoUpdate(WindowPtr win);
}

// <Event handlers for SIOW>=                                              
static void GUSIDoActivate(WindowPtr win, bool activate)
{
	GrafPtr port;
	GetPort(&port);
	if (win)
		SetPort(win);
	_DoActivate(win, activate);
	SetPort(port);
}

static void GUSISIOWActivate(EventRecord * ev)
{
	GUSIDoActivate(reinterpret_cast<WindowPtr>(ev->message), ev->modifiers & activeFlag);
}
// <Event handlers for SIOW>=                                              
static void GUSISIOWSusRes(EventRecord * ev)
{	
	switch ((ev->message >> 24) & 0xFF) {
	case suspendResumeMessage:
		GUSIDoActivate(FrontWindow(), ev->message & resumeFlag);
		break;
	case mouseMovedMessage:
		_DoIdle();
		break;
	}
}
// <Event handlers for SIOW>=                                              
static void GUSISIOWUpdate(EventRecord * ev)
{
	_DoUpdate((WindowPtr) ev->message);
}
// This is one of the rare occasions where overriding [[GUSIDefaultConsole]] makes sense. This way, we can 
// reuse the MPW version of [[GUSISetupConsoleDescriptors]].               
//                                                                         
// <SIOW implementation of [[GUSIDefaultSetupConsole]]>=                   
static void GUSISetupSIOW()
{
	GUSISetHook(GUSI_EventHook+updateEvt,	(GUSIHook)GUSISIOWUpdate);
	GUSISetHook(GUSI_EventHook+activateEvt,	(GUSIHook)GUSISIOWActivate);
	GUSISetHook(GUSI_EventHook+osEvt,		(GUSIHook)GUSISIOWSusRes);
}

void GUSIDefaultSetupConsole()
{
	GUSISetupConsoleDescriptors();
	GUSISetupSIOW();
	GUSISetupConsoleStdio();
}

// <GUSIForeignThreads.cp>=                                                
#include "GUSIInternal.h"
#include "GUSIContext.h"

#include <ConditionalMacros.h>
#include <CodeFragments.h>

// \section{Interfacing to the Thread Manager routines}                    
//                                                                         
// As mentioned above, our interface strategy differs for non-CFM and CFM code. We try
// to handle this with a macro. As opposed to the MPW interface, we have to handle CFM68K.
//                                                                         
// <Declaration of thread manager functions>=                              
extern "C" {
#if !TARGET_RT_MAC_CFM
#define DECL_thread(name, pname, ret, args) ret name args;
#ifdef __MWERKS__
#pragma pointers_in_D0
#endif
// Now we only have to declare the list once, and can reuse it numerous times.
//                                                                         
// <Declarations for Threads functions>=                                   
DECL_thread(GUSIStdNewThread, "\pNewThread", OSErr, 
	(ThreadStyle threadStyle, ThreadEntryProcPtr threadEntry, 
	 void *threadParam, Size stackSize, ThreadOptions options, 
	 void **threadResult, ThreadID *threadMade))
DECL_thread(GUSIStdSetThreadSwitcher, "\pSetThreadSwitcher", OSErr, 
	(ThreadID thread, ThreadSwitchProcPtr threadSwitcher, 
	 void *switchProcParam, Boolean inOrOut))
DECL_thread(GUSIStdSetThreadTerminator, "\pSetThreadTerminator", OSErr, 
	(ThreadID thread, ThreadTerminationProcPtr threadTerminator, 
	 void *terminationProcParam))
#ifdef __MWERKS__
#pragma pointers_in_A0
#endif
static bool ConnectToThreadLibrary()	{ return true; } 
#define sGUSIThreadFailure 0
#else
#define DECL_thread(name, pname, ret, args) static ret (*name) args;
// Now we only have to declare the list once, and can reuse it numerous times.
//                                                                         
// <Declarations for Threads functions>=                                   
DECL_thread(GUSIStdNewThread, "\pNewThread", OSErr, 
	(ThreadStyle threadStyle, ThreadEntryProcPtr threadEntry, 
	 void *threadParam, Size stackSize, ThreadOptions options, 
	 void **threadResult, ThreadID *threadMade))
DECL_thread(GUSIStdSetThreadSwitcher, "\pSetThreadSwitcher", OSErr, 
	(ThreadID thread, ThreadSwitchProcPtr threadSwitcher, 
	 void *switchProcParam, Boolean inOrOut))
DECL_thread(GUSIStdSetThreadTerminator, "\pSetThreadTerminator", OSErr, 
	(ThreadID thread, ThreadTerminationProcPtr threadTerminator, 
	 void *terminationProcParam))
// On PowerPC, we have to connect to the dynamic library (which, in principle,
// can fail).                                                              
//                                                                         
// <Connecting to the ThreadsLib code fragment>=                           
static OSErr sGUSIThreadFailure = 1;

static void DoConnectToThreadLibrary()
{
	CFragConnectionID 	ThreadsLib;
	CFragSymbolClass	symClass;
	Ptr 				whoCares;
	Str255				error;

	if (sGUSIThreadFailure = GetSharedLibrary(
		StringPtr("\pThreadsLib"), kPowerPCCFragArch, kLoadCFrag, 
		&ThreadsLib, &whoCares, error)
	)
		return;

#undef DECL_thread
#define DECL_thread(name, pname, ret, args) \
	if (sGUSIThreadFailure = FindSymbol(ThreadsLib, pname, (Ptr *) &name, &symClass)) \
		goto failed;
	// Now we only have to declare the list once, and can reuse it numerous times.
 //                                                                         
 // <Declarations for Threads functions>=                                   
 DECL_thread(GUSIStdNewThread, "\pNewThread", OSErr, 
 	(ThreadStyle threadStyle, ThreadEntryProcPtr threadEntry, 
 	 void *threadParam, Size stackSize, ThreadOptions options, 
 	 void **threadResult, ThreadID *threadMade))
 DECL_thread(GUSIStdSetThreadSwitcher, "\pSetThreadSwitcher", OSErr, 
 	(ThreadID thread, ThreadSwitchProcPtr threadSwitcher, 
 	 void *switchProcParam, Boolean inOrOut))
 DECL_thread(GUSIStdSetThreadTerminator, "\pSetThreadTerminator", OSErr, 
 	(ThreadID thread, ThreadTerminationProcPtr threadTerminator, 
 	 void *terminationProcParam))
	
	return;
	
failed:
#undef DECL_thread
#define DECL_thread(name, pname, ret, args) name = 0;
	// Now we only have to declare the list once, and can reuse it numerous times.
 //                                                                         
 // <Declarations for Threads functions>=                                   
 DECL_thread(GUSIStdNewThread, "\pNewThread", OSErr, 
 	(ThreadStyle threadStyle, ThreadEntryProcPtr threadEntry, 
 	 void *threadParam, Size stackSize, ThreadOptions options, 
 	 void **threadResult, ThreadID *threadMade))
 DECL_thread(GUSIStdSetThreadSwitcher, "\pSetThreadSwitcher", OSErr, 
 	(ThreadID thread, ThreadSwitchProcPtr threadSwitcher, 
 	 void *switchProcParam, Boolean inOrOut))
 DECL_thread(GUSIStdSetThreadTerminator, "\pSetThreadTerminator", OSErr, 
 	(ThreadID thread, ThreadTerminationProcPtr threadTerminator, 
 	 void *terminationProcParam))
}

static bool ConnectToThreadLibrary()
{
	if (sGUSIThreadFailure == 1)
		DoConnectToThreadLibrary();
	return !sGUSIThreadFailure;
}
#endif
}
// \section{Redirecting thread manager calls to their GUSI equivalents}    
//                                                                         
// People naively using thread manager calls will find themselves redirected to GUSI routines instead. 
//                                                                         
// <Override implementations of thread manager functions>=                 
pascal OSErr NewThread(
	ThreadStyle style, ThreadEntryProcPtr entry, void *param, 
	Size stack, ThreadOptions options, void **result, ThreadID *made)
{
	return GUSINewThread(style, entry, param, stack, options, result, made);
}

pascal OSErr SetThreadSwitcher(
	ThreadID thread, ThreadSwitchProcPtr switcher, void *param, Boolean inOrOut)
{
	return GUSISetThreadSwitcher(thread, switcher, param, inOrOut);
}

pascal OSErr SetThreadTerminator(
	ThreadID thread, ThreadTerminationProcPtr terminator, void *terminatorParam)
{
	return GUSISetThreadTerminator(thread, terminator, terminatorParam);
}
// \section{Installing our GUSI thread manager hooks}                      
//                                                                         
// [[GUSIThreadManagerForeignProxy]] is now quite straighforward.          
//                                                                         
// <Declaration of class [[GUSIThreadManagerForeignProxy]]>=               
class GUSIThreadManagerForeignProxy : public GUSIThreadManagerProxy {
public:
	virtual OSErr NewThread(
		ThreadStyle style, ThreadEntryProcPtr entry, void *param, 
		Size stack, ThreadOptions options, void **result, ThreadID *made);
	virtual OSErr SetThreadSwitcher(ThreadID thread,
		ThreadSwitchProcPtr switcher, void *switchProcParam, Boolean inOrOut);
	virtual OSErr SetThreadTerminator(ThreadID thread, 	
		ThreadTerminationProcPtr terminator, void *terminatorParam);
};
// Finally, to make sure that these hooks are called, we sneak in and replace [[GUSIThreadManagerProxy::MakeInstance]].
//                                                                         
// <Override implementation of [[GUSIThreadManagerProxy::MakeInstance]]>=  
GUSIThreadManagerProxy * GUSIThreadManagerProxy::MakeInstance()
{
	return new GUSIThreadManagerForeignProxy;
}
// Note that the calls below resolve to a statically linked procedure on non-CFM, but call through an indirect procedure pointer on
// CFM.                                                                    
//                                                                         
// <Member functions for class [[GUSIThreadManagerForeignProxy]]>=         
OSErr GUSIThreadManagerForeignProxy::NewThread(
	ThreadStyle style, ThreadEntryProcPtr entry, void *param, 
	Size stack, ThreadOptions options, void **result, ThreadID *made)
{
	if (!ConnectToThreadLibrary())
		return sGUSIThreadFailure;
	return GUSIStdNewThread(style, entry, param, stack, options, result, made);
}

OSErr GUSIThreadManagerForeignProxy::SetThreadSwitcher(
	ThreadID thread, ThreadSwitchProcPtr switcher, void *param, Boolean inOrOut)
{
	if (!ConnectToThreadLibrary())
		return sGUSIThreadFailure;
	return GUSIStdSetThreadSwitcher(thread, switcher, param, inOrOut);
}

OSErr GUSIThreadManagerForeignProxy::SetThreadTerminator(
	ThreadID thread, ThreadTerminationProcPtr terminator, void *terminatorParam)
{
	if (!ConnectToThreadLibrary())
		return sGUSIThreadFailure;
	return GUSIStdSetThreadTerminator(thread, terminator, terminatorParam);
}

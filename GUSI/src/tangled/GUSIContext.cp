// <GUSIContext.cp>=                                                       
#include "GUSIInternal.h"
#include "GUSIContext.h"
#include "GUSIDiag.h"
#include "GUSISignal.h"
#include "GUSIConfig.h"
#include "GUSIDescriptor.h"
#include "GUSITimer.h"

#include <errno.h>
#include <signal.h>

#include <EPPC.h>
#include <LowMem.h>
#include <AppleEvents.h>
#include <QuickDraw.h>
#include <Devices.h>

#include <utility>
#include <memory>

GUSI_USING_STD_NAMESPACE

// <Implementation of completion handling>=                                
GUSIProcess * GUSIProcess::sInstance;
// <Implementation of completion handling>=                                
GUSI_NEEDS_QD

GUSIProcess::GUSIProcess(bool threading)
{
	GetCurrentProcess(&fProcess);
	fA5 = (long) LMGetCurrentA5();
	if (threading)
		AcquireTaskRef();
	else
		fTaskRef = 0;
	GUSIConfiguration::Instance()->AutoInitGraf();
	fReadyThreads 	= 0;
	fExistingThreads= 0;
	fResumeTicks  	= 0;
	fWillSleep 		= false;
	fDontSleep 		= false;
	fSigProcess		= GUSISigFactory::Instance()->CreateSigProcess();
	fClosing		= 0;
}
// We can't do this inline as this would create a circular dependency.     
//                                                                         
// <Implementation of completion handling>=                                
void GUSIProcess::QueueForClose(GUSISocket * sock)
{
	sock->Enqueue(&fClosing);
}
// On termination of the [[GUSIProcess]], we accelerate the closings of all
// pending sockets.                                                        
//                                                                         
// <Implementation of completion handling>=                                
GUSIProcess::~GUSIProcess()
{
	UInt32 	start = LMGetTicks();
	UInt32	now	  = start;
	
	while (fClosing)	{ 
		fClosing->CheckClose(now);
		if (now < start+300)	// Normal speed for 5 seconds
			now = LMGetTicks();
		else					
			now += 300;			// Accelerate
	}
}
// [[GUSIContext::Setup]] initializes the default context. We have to employ 
// [[sCreatingCurrentContext]] to avoid nasty recursions.                  
//                                                                         
// <Implementation of completion handling>=                                
void GUSIContext::Setup(bool threading)
{
	bool	wasThreading = sHasThreading;
	if (threading)
		sHasThreading = true;
	if (!sCurrentContext && !sCreatingCurrentContext) {
		MaxApplZone();		// It's about time, too!
		
		sCreatingCurrentContext = true;
		sCurrentContext = 
			GUSIContextFactory::Instance()->CreateContext(kApplicationThreadID);
		sCreatingCurrentContext = false;
	} else if (!wasThreading && threading) {
		// Sometimes we only recognize that we need threading after the application context
  // has already been created. Assuming a disciplined use of threads, we can assume
  // that the current context is still the application context, so all we have to do
  // is to set the thread switcher for it.                                   
  //                                                                         
  // <Upgrade application context to threading>=                             
  GUSIThreadManagerProxy::Instance()->SetThreadSwitcher(
  	kApplicationThreadID, (ThreadSwitchProcPtr)GUSIThreadSwitchIn, sCurrentContext, true);
  GUSIThreadManagerProxy::Instance()->SetThreadSwitcher(
  	kApplicationThreadID, (ThreadSwitchProcPtr)GUSIThreadSwitchOut, sCurrentContext, false);
  GUSIProcess::Instance()->AcquireTaskRef();
	}
}
// <Implementation of completion handling>=                                
GUSIContext::Queue 		GUSIContext::sContexts;
GUSIContext * 			GUSIContext::sCurrentContext;
bool					GUSIContext::sCreatingCurrentContext;
bool					GUSIContext::sHasThreading;
OSErr					GUSIContext::sError;
// <Implementation of completion handling>=                                
extern int h_errno;

void GUSIContext::StartSetup()
{
	fSwitchInProc	=	0;
	fSwitchOutProc	=	0;
	fTerminateProc	=	0;
	fErrno			=	errno;
	fHostErrno		=	h_errno;
	fJoin			=	nil;
	fFlags			=	0;
	fWakeup			= 	false;
}

void GUSIContext::FinishSetup()
{
	fProcess		= GUSIProcess::Instance();
	fSigContext		= 
		GUSISigFactory::Instance()->CreateSigContext(
			sCurrentContext ? sCurrentContext->fSigContext : 0);
	++fProcess->fReadyThreads;
	++fProcess->fExistingThreads;
	sContexts.push(this);
	if (sHasThreading) {
		GUSIThreadManagerProxy::Instance()->SetThreadSwitcher(
			fThreadID, (ThreadSwitchProcPtr)GUSIThreadSwitchIn, this, true);
		GUSIThreadManagerProxy::Instance()->SetThreadSwitcher(
			fThreadID, (ThreadSwitchProcPtr)GUSIThreadSwitchOut, this, false);
		if (fThreadID != kApplicationThreadID)
			GUSIThreadManagerProxy::Instance()->SetThreadTerminator(
				fThreadID, (ThreadTerminationProcPtr)GUSIThreadTerminator, this);
	}
	GUSI_MESSAGE(("Create #%d\n", fThreadID));
}
// The preexisting thread constructor is now simple to define.             
//                                                                         
// <Implementation of completion handling>=                                
GUSIContext::GUSIContext(ThreadID id)
{
	StartSetup();
	fThreadID = id;
	sError	  = 0;
	FinishSetup();
}
// And so is the creation contructor, come to think of it.                 
//                                                                         
// <Implementation of completion handling>=                                
GUSIContext::GUSIContext(ThreadEntryProcPtr threadEntry, void *threadParam, 
        Size stackSize, ThreadOptions options, void ** result, ThreadID * thread)
{
    StartSetup();
	if (!result)
		result = &fResult;
    sError = GUSIThreadManagerProxy::Instance()->NewThread(
				kCooperativeThread, threadEntry, threadParam, 
        		stackSize, options, result, &fThreadID);
	if (thread)
    	*thread = fThreadID;
	if (sError)
        return;
    FinishSetup();
}
// [[GUSIContext::Lookup]] does a linear search.                           
//                                                                         
// <Implementation of completion handling>=                                
GUSIContext * GUSIContext::Lookup(ThreadID id)
{
	for (GUSIContextQueue::iterator context = begin(); context != end(); ++context)
		if (context->fThreadID == id)
			return *context;
	return nil;
}
// <Implementation of completion handling>=                                
GUSIContext::~GUSIContext()
{
	sContexts.remove(this);
	
	delete fSigContext;
}
// Furthermore, at the end of an application, we need some global cleanup. This
// is especially true for MPW tools. Furthermore, we shouldn't start throwing away
// contexts before all sockets are closed. Afterwards, we kill the [[GUSIProcess]]
// instance as well.                                                       
//                                                                         
// <Implementation of completion handling>=                                
void GUSIContext::Queue::LiquidateAll()
{
	GUSIContextFactory::DeleteInstance();
	GUSIDescriptorTable::CloseAllDescriptors();
	while (!empty())
		front()->Liquidate();
	GUSIProcess::DeleteInstance();
}

void GUSIContext::Liquidate()
{
	GUSI_MESSAGE4(("GUSIContext::Liquidate %08x\n", fThreadID));
	switch (fThreadID) {
	case kApplicationThreadID:	// Main thread, restore switchers
		if (sHasThreading) {
			SetThreadSwitcher(fThreadID, fSwitchInProc,  fSwitchInParam,  true);
			SetThreadSwitcher(fThreadID, fSwitchOutProc, fSwitchOutParam, false);
		}
		break;
	default:					// Other thread, terminate
		if (!(fFlags & done)) {
			fFlags &= ~detached;	// Will destroy context ourselves
			DisposeThread(fThreadID, nil, false);
		}
		break;
	}
	delete this;
}
// [[GUSIContext::Wakeup]] awakens a context. In every asynchronous call, there 
// is a risk of race conditions, i.e., the call completes before the       
// thread/process making it goes to sleep. For threads, we can handle this case 
// by checking for a wakeup in the switch out procedure (an idea due to Quinn). 
//                                                                         
// <Implementation of completion handling>=                                
void GUSIContext::Wakeup()
{
	if (fWakeup) {
		GUSI_MESSAGE(("Duplicate wakeup #%d\n", fThreadID));
	} else {
		GUSI_MESSAGE(("Wakeup #%d\n", fThreadID));
		fWakeup = true;
		if (fThreadID && fThreadID != kApplicationThreadID)
			SetThreadReadyGivenTaskRef(Process()->GetTaskRef(), fThreadID);
		Process()->Wakeup();
	}
}
// The code for processes used to be quite complex. However, it turned out that there are
// in fact no race cvonditions with [[WaitNextEvent]] and it was my attempted countermeasures
// which caused trouble instead. Thanks to Keith Stattenfield for finally setting me straight.
//                                                                         
// \begin{itemize}                                                         
// \item If [[fDontSleep]] is set, the process will not go to sleep in the first place (presumably because
// 	a wakeup is already pending).                                          
// \item Otherwise, if [[fWillSleep]] is not set, we can prevent the process from going to sleep by setting 
// 	[[fDontSleep]].                                                        
// \item Otherwise, we can wake it up by calling [[WakeUpProcess]], whether or not [[WaitNextEvent]]
// 	has already been called.                                               
// \end{itemize}                                                           
//                                                                         
//                                                                         
// <Implementation of completion handling>=                                
void GUSIProcess::Wakeup()
{
	if (fDontSleep) {
		GUSI_SMESSAGE("Duplicate WakeUpProcess\n");
	} else {
		fDontSleep 			= true;
		
		if (!fWillSleep) {
			GUSI_SMESSAGE("Caught sleep in time\n");
		} else {		
			GUSI_SMESSAGE("WakeUpProcess\n");
			
			WakeUpProcess(&fProcess);
		}
	}
}
// The thread wrappers are fairly trivial.                                 
//                                                                         
// <Implementation of completion handling>=                                
OSErr GUSINewThread(
	ThreadStyle, ThreadEntryProcPtr threadEntry, void *threadParam, 
	Size stackSize, ThreadOptions options, void **threadResult, ThreadID *threadMade)
{
	GUSIContext * context = GUSIContextFactory::Instance()->CreateContext(
				threadEntry, threadParam, stackSize, options, 
				threadResult, threadMade);
	OSErr err = context->Error();
	if (err)
		context->Liquidate();
	return err;
}
// <Implementation of completion handling>=                                
#ifdef __MRC__
#pragma noinline_func GUSISetupContextFactory
#endif
#ifdef __MWERKS__
#pragma dont_inline on
#endif

void GUSISetupContextFactory()
{
}

#ifdef __MWERKS__
#pragma dont_inline reset
#endif
// <Implementation of completion handling>=                                
static GUSIContextFactory *	sGUSIContextFactory;
static bool					sGUSIContextFactorySetup;

GUSIContextFactory * GUSIContextFactory::Instance()
{
	if (!sGUSIContextFactorySetup) {
		sGUSIContextFactorySetup = true;
		GUSISetupContextFactory();
	}

	if (!sGUSIContextFactory) 
		SetInstance(new GUSIContextFactory());
	
	return sGUSIContextFactory;
}

void GUSIContextFactory::SetInstance(GUSIContextFactory * instance)
{
	sGUSIContextFactory = instance;
}

void GUSIContextFactory::DeleteInstance()
{
	delete sGUSIContextFactory;
	sGUSIContextFactory = 0;
}

GUSIContextFactory::GUSIContextFactory()
{
}

GUSIContextFactory::~GUSIContextFactory()
{
}
// <Implementation of completion handling>=                                
GUSIContext * GUSIContextFactory::CreateContext(
	ThreadEntryProcPtr threadEntry, void *threadParam, 
	Size stackSize, ThreadOptions options, void **threadResult, ThreadID *threadMade
) 
{
	GUSIContext::Setup(true);

	return new GUSIContext(threadEntry, threadParam, stackSize, options,
					threadResult, threadMade);
}

GUSIContext * GUSIContextFactory::CreateContext(ThreadID threadMade)
{
	GUSIContext::Setup(true);
	
	return new GUSIContext(threadMade);
}
// The thread switcher dispatches to the appropriate member function.      
//                                                                         
// <Implementation of completion handling>=                                
OSErr GUSISetThreadSwitcher(
	ThreadID thread, ThreadSwitchProcPtr threadSwitcher, void *switchProcParam, Boolean inOrOut)
{
	GUSIContext * context;
	if (!(context = GUSIContext::Lookup(thread)))
		return GUSIThreadManagerProxy::Instance()->SetThreadSwitcher(
			thread, threadSwitcher, switchProcParam, inOrOut);
	if (inOrOut) 
		context->SetSwitchIn(threadSwitcher, switchProcParam);
	else
		context->SetSwitchOut(threadSwitcher, switchProcParam);
	return noErr;
}

void GUSIContext::SetSwitchIn(ThreadSwitchProcPtr switcher, void *switchParam)
{
	fSwitchInProc = switcher;
	fSwitchInParam= switchParam;
}

void GUSIContext::SetSwitchOut(ThreadSwitchProcPtr switcher, void *switchParam)
{
	fSwitchOutProc = switcher;
	fSwitchOutParam= switchParam;
}
// Similar for the thread terminator.                                      
//                                                                         
// <Implementation of completion handling>=                                
OSErr GUSISetThreadTerminator(
	ThreadID thread, ThreadTerminationProcPtr threadTerminator, void *terminationProcParam)
{
	GUSIContext * context;
	if (!(context = GUSIContext::Lookup(thread)))
		return GUSIThreadManagerProxy::Instance()->SetThreadTerminator(
			thread, threadTerminator, terminationProcParam);
	context->SetTerminator(threadTerminator, terminationProcParam);
	
	return noErr;
}

void GUSIContext::SetTerminator(ThreadTerminationProcPtr terminator, void *terminationParam)	
{
	fTerminateProc = terminator;
	fTerminateParam= terminationParam;
}
// The hooks, after having performed their task, call through to user defined hooks.
//                                                                         
// <Implementation of completion handling>=                                
#if GENERATING68K && GENERATINGCFM
#define CallThreadSwitchProc(userRoutine, thread, context)	\
	CallUniversalProc((userRoutine), uppThreadSwitchProcInfo, (thread), (context))
#define CallThreadTerminationProc(userRoutine, thread, context)	\
	CallUniversalProc((userRoutine), uppThreadTerminationProcInfo, (thread), (context))
#else
#define CallThreadSwitchProc(userRoutine, thread, context)	\
	(*userRoutine)((thread), (context))
#define CallThreadTerminationProc(userRoutine, thread, context)	\
	(*userRoutine)((thread), (context))
#endif
// <Implementation of completion handling>=                                
pascal void GUSIThreadSwitchIn(ThreadID, GUSIContext * context)
{
	context->SwitchIn();
}

void GUSIContext::SwitchIn()
{
	if (sCurrentContext != this) {
		GUSI_MESSAGE(("Yield #%d -> #%d\n", sCurrentContext->fThreadID, fThreadID));
		
		fEntryTicks = LMGetTicks();
	}
	sCurrentContext	= 	this;
	errno			=	fErrno;
	h_errno			=	fHostErrno;
	
	if (fFlags & asleep) {
		fFlags &= ~asleep;
		++fProcess->fReadyThreads;
		fProcess->fDontSleep = false; // We're back to at least 2 threads
	}
	
	if (fSwitchInProc)
		CallThreadSwitchProc(fSwitchInProc, fThreadID, fSwitchInParam);
}
// When we are switched out, we check whether we should wake up again right away.
// Pleasantly enough, there are no race conditions between this code and   
// [[GUSIContext::Wakeup()]]:                                              
//                                                                         
// \begin{Itemize}                                                         
// \item By the time [[GUSIThreadSwitchOut]] is called, the thread state is 
// 	already set to stopped, so the [[GetThreadStateGivenTaskRef]] call in  
// 	[[GUSIContext::Wakeup]] will take effect.                              
// \item If [[GUSIContext::Wakeup]] is called before [[GUSIThreadSwitchOut]],
// 	it will be able to set [[fWakeup]] in time for the switchout procedure 
// 	to notice and set the thread to ready again.                           
// \end{itemize}                                                           
//                                                                         
//                                                                         
// <Implementation of completion handling>=                                
pascal void GUSIThreadSwitchOut(ThreadID, GUSIContext * context)
{
	context->SwitchOut();
}

void GUSIContext::SwitchOut()
{
	if (fSwitchOutProc)
		CallThreadSwitchProc(fSwitchOutProc, fThreadID, fSwitchOutParam);
	fErrno		=	errno;
	fHostErrno	=	h_errno;
	
	ThreadTaskRef	taskRef = Process()->GetTaskRef();
	ThreadState		state;
	if (!GetThreadStateGivenTaskRef(taskRef, fThreadID, &state))
		if (state == kStoppedThreadState)
			if (fWakeup) {
				SetThreadReadyGivenTaskRef(taskRef, fThreadID);
			} else {
				GUSI_MESSAGE(("Sleep #%d\n", fThreadID));
				fFlags |= asleep;
				--fProcess->fReadyThreads;
			}
}
// <Implementation of completion handling>=                                
pascal void GUSIThreadTerminator(ThreadID, GUSIContext * context)
{
	context->Terminate();
}

void GUSIContext::Terminate()
{
	--fProcess->fExistingThreads;
	if (!(fFlags & asleep))
		--fProcess->fReadyThreads;
	if (fTerminateProc)
		CallThreadTerminationProc(fTerminateProc, fThreadID, fTerminateParam);
	if (fFlags & detached) {
		delete this;
	} else {
		fFlags |= done;
		if (fJoin)
			fJoin->Wakeup();
	}
	GUSI_MESSAGE(("Terminate #%d\n", fThreadID));
}
// [[Done]] is simple unless [[join]] is set. Otherwise, we set [[fJoin]] and 
// wait. If some other process is already joining, we bail out.            
//                                                                         
// <Implementation of completion handling>=                                
bool GUSIContext::Done(bool join)
{
	if ((fFlags & done) || !join || fJoin)
		return (fFlags & done);
	fJoin = GUSIContext::sCurrentContext;
	while (!(fFlags & done))
		Yield(kGUSIBlock);
	return (fFlags & done);
}
// [[Yield]] tries to do the smart thing in all contexts. The basic idea is that 
// both in threading and non-threading contexts, everyone gets their fair turn.
// We'll start with some constants determining scheduling policy (which we possibly 
// should make user configurable).                                         
//                                                                         
// <Implementation of completion handling>=                                
const int kThreadTimeSliceTicks		=	12;
const int kProcessTimeSliceTicks	= 	20;
const int kProcessSleepTicks		= 	60;
// <Implementation of completion handling>=                                
bool GUSIContext::Yield(GUSIYieldMode wait)
{
	// <Determine process and blocking parameters>=                            
 bool			mainThread	= CreateCurrent()->fThreadID == kApplicationThreadID;
 bool 			block		= wait == kGUSIBlock && !mainThread;
 GUSIProcess	*	process 	= GUSIProcess::Instance();
	// <Check thread timeslice before doing voluntary switch>=                 
 if (wait == kGUSIYield && LMGetTicks() - sCurrentContext->fEntryTicks < kThreadTimeSliceTicks)
 	return false;	
	
	bool 			interrupt 	= false;
	
	do {
		// <Suspend the current process if necessary>=                             
  if (mainThread)
  	process->Yield(wait);
		// <Check for eligible signals>=                                           
  if (interrupt = Raise())
  	goto done;
		// <Suspend the current thread if possible>=                               
  if (sHasThreading) {
  	if (block)
  		SetThreadState(kCurrentThreadID, kStoppedThreadState, kNoThreadID);
  	else
  		YieldToAnyThread();
  }
	} while (wait == kGUSIBlock && !sCurrentContext->fWakeup);
done:
	sCurrentContext->fWakeup = false;
	
	return interrupt;
}
// [[SigWait]] and [[SigSuspend]] are similar to [[Yield]]. The former waits for
// one of the specified signals to become pending, while the latter waits for
// any signal to become executed.                                          
//                                                                         
// <Implementation of completion handling>=                                
void GUSIContext::SigWait(sigset_t sigs)
{
	GUSIYieldMode 	wait		= kGUSIBlock;	
	// <Determine process and blocking parameters>=                            
 bool			mainThread	= CreateCurrent()->fThreadID == kApplicationThreadID;
 bool 			block		= wait == kGUSIBlock && !mainThread;
 GUSIProcess	*	process 	= GUSIProcess::Instance();
	
	for (;;) {
		// <Suspend the current process if necessary>=                             
  if (mainThread)
  	process->Yield(wait);
		if (Pending() & sigs)
			break;
		Raise();
		// <Suspend the current thread if possible>=                               
  if (sHasThreading) {
  	if (block)
  		SetThreadState(kCurrentThreadID, kStoppedThreadState, kNoThreadID);
  	else
  		YieldToAnyThread();
  }
	}
	sCurrentContext->fWakeup = false;
}

void GUSIContext::SigSuspend()
{
	GUSIYieldMode 	wait		= kGUSIBlock;	
	// <Determine process and blocking parameters>=                            
 bool			mainThread	= CreateCurrent()->fThreadID == kApplicationThreadID;
 bool 			block		= wait == kGUSIBlock && !mainThread;
 GUSIProcess	*	process 	= GUSIProcess::Instance();
	
	for (;;) {
		// <Suspend the current process if necessary>=                             
  if (mainThread)
  	process->Yield(wait);
		if (Raise(true))
			break;
		// <Suspend the current thread if possible>=                               
  if (sHasThreading) {
  	if (block)
  		SetThreadState(kCurrentThreadID, kStoppedThreadState, kNoThreadID);
  	else
  		YieldToAnyThread();
  }
	}
	sCurrentContext->fWakeup = false;
}
// [[GUSIProcess::Yield]] tries to narrow down the critical time for race conditions,
// so we rarely have to force a wakeup. This is done using the [[fWillSleep]] and [[fDontSleep]]
// flags. The former signals the processes willingness to block, the latter is set by wakeup
// routines to try stopping it from doing so. According to Andreas Grosam, the thread manager 
// may need a number of calls to [[YieldToAnyThread]] to reorganize its queues, so we leave the
// [[fDontSleep]] flag set until the number of ready threads increases above 2 or the number of
// threads in the process drops to 1 -- the application thread itself.     
//                                                                         
// <Implementation of completion handling>=                                
void GUSIProcess::Yield(GUSIYieldMode wait)
{
	// By definition, an interrupt key press is only intended for us if we are in front.
 // Applications can check this condition relatively easily by keeping track of
 // suspend and resume events, but as a library, the only reliable way is to compare
 // PSNs.                                                                   
 //                                                                         
 // <Check for interrupts if we are the front process>=                     
 ProcessSerialNumber	front;
 Boolean				same;
 	
 if (!GetFrontProcess(&front) && !SameProcess(&front, &fProcess, &same) && same)
 	GUSIConfiguration::Instance()->CheckInterrupt();
	if (wait == kGUSIBlock) {
		fWillSleep = true;
		if (fReadyThreads > 1 || fDontSleep) {
			GUSI_SMESSAGE("Don't Sleep\n");
			wait = kGUSIYield;
		}
	}
	if (fExistingThreads < 2) // Single threaded process skips sleep only once
		fDontSleep = false;
	if (wait == kGUSIYield && LMGetTicks() - fResumeTicks < kProcessTimeSliceTicks) {
		fWillSleep 		= false;
		return;
	}
	if (gGUSISpinHook) {
		gGUSISpinHook(wait == kGUSIBlock);
	} else {
		GUSI_SMESSAGE("Suspend\n");
		GUSIHandleNextEvent(wait == kGUSIBlock ? kProcessSleepTicks : 0);
		GUSI_SMESSAGE("Resume\n");
	} 
	if (fExistingThreads < 2) 		// Single threaded process skips sleep only once
		fDontSleep = false;
	fWillSleep 		= false;
	fResumeTicks 	= LMGetTicks();
	if (fClosing)
		fClosing->CheckClose();
}
// [[Raise]] raises all eligible signals. [[Pending]] returns the pending signals.
// [[Blocked]] returns the blocked signals.                                
//                                                                         
// <Implementation of completion handling>=                                
bool GUSIContext::Raise(bool allSigs)
{
	return sCurrentContext->SigContext()->Raise(
		GUSIProcess::Instance()->SigProcess(), allSigs);
}

sigset_t GUSIContext::Pending()
{
	return sCurrentContext->SigContext()->Pending(
		GUSIProcess::Instance()->SigProcess());
}

sigset_t GUSIContext::Blocked()
{
	return sCurrentContext->SigContext()->GetBlocked();
}
// Many different asynchronous calls can be handled by [[GUSIIODone]].     
//                                                                         
// <Implementation of completion handling>=                                
inline GUSIContext *& Context(IOParam * pb)
{
	return reinterpret_cast<GUSIContext **>(pb)[-1];
}
static void GUSIIODone(IOParam * pb)
{
	if (Context(pb)) 
		Context(pb)->Wakeup();
}

GUSI_COMPLETION_PROC_A0(GUSIIODone, IOParam)
// Since we (or at least I) never quite know when a call is executed synchronously,
// we set the context field to [[nil]] until after the call. This avoids having to 
// wake up a running context, which is a fairly costly operation.          
//                                                                         
// <Implementation of completion handling>=                                
void GUSIStartIO(IOParam * pb)
{
	static IOCompletionUPP sIODone = 0;
	
	if (!sIODone)
		sIODone = NewIOCompletionProc(reinterpret_cast<IOCompletionProcPtr>(GUSIIODoneEntry));
	Context(pb) 		= nil;
	pb->ioCompletion 	= sIODone;
}

OSErr GUSIFinishIO(IOParam * pb)
{
	Context(pb) 		= GUSIContext::CreateCurrent();
	while (pb->ioResult > 0)
		GUSIContext::Yield(kGUSIBlock);
	return pb->ioResult;
}

OSErr GUSIControl(IOParam * pb)
{
	GUSIStartIO(pb);
	PBControlAsync(reinterpret_cast<ParmBlkPtr>(pb));
	return GUSIFinishIO(pb);
}
// The default implementation of [[GUSIThreadManagerProxy]] is trivial.    
//                                                                         
// <Implementation of completion handling>=                                
static auto_ptr<GUSIThreadManagerProxy> sGUSIThreadManagerProxy;

OSErr GUSIThreadManagerProxy::NewThread(
		ThreadStyle threadStyle, ThreadEntryProcPtr threadEntry, void *threadParam, 
		Size stackSize, ThreadOptions options, 
		void **threadResult, ThreadID *threadMade)
{
	return ::NewThread(
				threadStyle, threadEntry, threadParam, stackSize, options, 
				threadResult, threadMade);
}

OSErr GUSIThreadManagerProxy::SetThreadSwitcher(ThreadID thread, 
		ThreadSwitchProcPtr threadSwitcher, void *switchProcParam, Boolean inOrOut)
{
	return ::SetThreadSwitcher(thread, threadSwitcher, switchProcParam, inOrOut);
}

OSErr GUSIThreadManagerProxy::SetThreadTerminator(ThreadID thread, 
		ThreadTerminationProcPtr threadTerminator, void *terminationProcParam)
{
	return ::SetThreadTerminator(thread, threadTerminator, terminationProcParam);
}

GUSIThreadManagerProxy * GUSIThreadManagerProxy::Instance()
{ 
	if (!sGUSIThreadManagerProxy.get()) 
		sGUSIThreadManagerProxy = auto_ptr<GUSIThreadManagerProxy>(MakeInstance()); 
	return sGUSIThreadManagerProxy.get(); 
}

GUSIThreadManagerProxy * GUSIThreadManagerProxy::MakeInstance()
{
	return new GUSIThreadManagerProxy;
}

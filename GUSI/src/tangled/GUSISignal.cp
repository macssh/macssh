// <GUSISignal.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSISignal.h"
#include "GUSIDiag.h"
#include "GUSITimer.h"
#include "GUSIPThread.h"

#include <stdlib.h>
#include <unistd.h>
#include <memory>

GUSI_USING_STD_NAMESPACE

// <Member functions for class [[GUSISigContext]]>=                        
GUSISigContext::GUSISigContext(const GUSISigContext * parent)
 : fPending(0), fBlocked(parent ? parent->fBlocked : 0)
{
}

GUSISigContext::~GUSISigContext()
{
}
// <Member functions for class [[GUSISigContext]]>=                        
inline sigset_t SigMask(int signal) { return 1 << (signal-1); }

sigset_t GUSISigContext::CantBlock()
{
	return SigMask(SIGKILL)
		|  SigMask(SIGSTOP)
		|  SigMask(SIGILL)
		|  SigMask(SIGFPE)
		|  SigMask(SIGSEGV);
}
// [[GetBlocked]] and [[SetBlocked]] take care of the rest.                
//                                                                         
// <Member functions for class [[GUSISigContext]]>=                        
sigset_t GUSISigContext::GetBlocked() const
{
	return fBlocked;
}

void GUSISigContext::SetBlocked(sigset_t sigs)
{
	fBlocked = sigs & ~CantBlock();
}
// [[Pending]] and [[Post]] deal with signals pending against a thread (as a result of 
// [[pthread_kill]]).                                                      
//                                                                         
// <Member functions for class [[GUSISigContext]]>=                        
sigset_t GUSISigContext::Pending() const
{
	return fPending;
}

void GUSISigContext::ClearPending(sigset_t clear)
{
	fPending &= ~clear;
}

void GUSISigContext::Post(int sig)
{
	sigaddset(&fPending, sig);
}
// [[Raise]] initiates a series of signal executions and returns [[true]] if any of these should
// interrupt a slow system call.                                           
//                                                                         
// <Member functions for class [[GUSISigContext]]>=                        
sigset_t GUSISigContext::Pending(GUSISigProcess * proc) const
{
	return proc->Pending() | Pending();
}

sigset_t GUSISigContext::Ready(GUSISigProcess * proc)
{
	return Pending(proc) & ~GetBlocked();
}

bool GUSISigContext::Raise(GUSISigProcess * proc, bool allSigs)
{
	bool		interrupt = false;
	
	for (;;) {
		sigset_t	todo = Ready(proc);

		if (!todo)
			return interrupt;

		proc->ClearPending(todo);
		ClearPending(todo);
		
		for (int sig = 1; todo; ++sig)
			if (sigismember(&todo, sig)) {
				sigdelset(&todo, sig);
				interrupt = proc->Raise(sig, this) || interrupt || allSigs;
			}
	}
}
// <Member functions for class [[GUSISigProcess]]>=                        
GUSISigProcess::GUSISigProcess()
 : fPending(0)
{
	memset(&fAction, 0, sizeof(fAction));
}

GUSISigProcess::~GUSISigProcess()
{
}
// [[GetAction]] returns a signal action.                                  
//                                                                         
// <Member functions for class [[GUSISigProcess]]>=                        
struct sigaction & GUSISigProcess::GetAction(int sig)
{
	return fAction[sig-1];
}
// <Member functions for class [[GUSISigProcess]]>=                        
bool GUSISigProcess::CantCatch(int sig)
{
	return (sig == SIGKILL) 
		|| (sig == SIGSTOP);
}

bool GUSISigProcess::CantIgnore(int sig)
{
	return (sig == SIGKILL) 
		|| (sig == SIGSTOP);
}

int GUSISigProcess::SetAction(int sig, const struct sigaction & act)
{
	if (act.sa_handler == SIG_IGN) {
		if (CantIgnore(sig))
			return GUSISetPosixError(EINVAL);
		sigdelset(&fPending, sig);
	} else if (act.sa_handler != SIG_DFL) {
		if (CantCatch(sig))
			return GUSISetPosixError(EINVAL);
	}
	fAction[sig-1] = act;
	
	return 0;
}
// [[Pending]] and [[Post]] deal with signals pending against a process (as a result of 
// [[kill]] or [[raise]]).                                                 
//                                                                         
// <Member functions for class [[GUSISigProcess]]>=                        
sigset_t GUSISigProcess::Pending() const
{
	return fPending;
}

void GUSISigProcess::ClearPending(sigset_t clear)
{
	fPending &= ~clear;
}

void GUSISigProcess::Post(int sig)
{
	sigaddset(&fPending, sig);
	// To wake up contexts, we make two passes: In the first pass, we look for contexts which have the signal
 // unblocked. If we don't find any, we wake up all contexts in the hope that one of them is waiting in a
 // [[sigwait]]. This corresponds more or less to the typical nonthreaded and threaded models of thread 
 // handling, respectively.                                                 
 //                                                                         
 // <Wake up appropriate contexts to deliver signal>=                       
 sigset_t	sigmask = SigMask(sig);
 bool		found   = false;
 for (GUSIContextQueue::iterator context = GUSIContext::begin(); 
 	context != GUSIContext::end(); 
 	++context
 )
 	if (!(context->SigContext()->GetBlocked() & sigmask)) {
 		context->Wakeup();
 		found = true;
 	}
 if (!found)
 	for (GUSIContextQueue::iterator context = GUSIContext::begin(); 
 		context != GUSIContext::end(); 
 		++context
 	)
 		context->Wakeup();
}
// [[Raise]], finally, is the function for which the whole rest of the engine exists. 
//                                                                         
// <Member functions for class [[GUSISigProcess]]>=                        
bool GUSISigProcess::Raise(int sig, GUSISigContext * context)
{
	struct sigaction & act = GetAction(sig);
	
	if (act.sa_handler == SIG_IGN)
		return false; // Ignore signal
	else if (act.sa_handler == SIG_DFL)
		return DefaultAction(sig, act) && !(act.sa_flags & SA_RESTART);
	
	// Executing an user defined signal handler involves a rather complicated dance of blocking and
 // unblocking signals.                                                     
 //                                                                         
 // <Execute the signal handler>=                                           
 __sig_handler 	handler = act.sa_handler;
 sigset_t		blockset= act.sa_mask;

 if (act.sa_flags & SA_RESETHAND)
 	act.sa_handler = SIG_DFL;
 else if (!(act.sa_flags & SA_NODEFER))
 	sigaddset(&blockset, sig);

 sigset_t	blocksave = context->GetBlocked();
 context->SetBlocked(blocksave | blockset);
 (*handler)(sig);
 sigset_t	blocknew  = context->GetBlocked();
 context->SetBlocked(blocksave | (blocknew & ~blockset));

 return !(act.sa_flags & SA_RESTART);
}
// <Member functions for class [[GUSISigProcess]]>=                        
extern "C" void _exit(int status);

bool GUSISigProcess::DefaultAction(int sig, const struct sigaction &)
{
	switch (sig) {
	case SIGCHLD:
	case SIGCONT:
	case SIGSTOP:
	case SIGTSTP:
	case SIGTTIN:
	case SIGTTOU:
		break;	// Ignore
	default:
		_exit(1);
	}
	
	return false;
}
// \section{Implementation of the signal handling engine}                  
//                                                                         
// The [[GUSISigFactory]] is a straighforward overridable singleton.       
//                                                                         
// <Member functions for class [[GUSISigFactory]]>=                        
static auto_ptr<GUSISigFactory> sGUSISigFactory;

GUSISigFactory * GUSISigFactory::Instance()
{
	if (!sGUSISigFactory.get())
		SetInstance(new GUSISigFactory);
		
	return sGUSISigFactory.get();
}

void GUSISigFactory::SetInstance(GUSISigFactory * instance)
{
	sGUSISigFactory = auto_ptr<GUSISigFactory>(instance);
}

GUSISigFactory::~GUSISigFactory()
{
}
// To support more signals, override these creators to make them return subclasses of the required
// classes.                                                                
//                                                                         
// <Member functions for class [[GUSISigFactory]]>=                        
GUSISigProcess * GUSISigFactory::CreateSigProcess()
{
	return new GUSISigProcess;
}

GUSISigContext * GUSISigFactory::CreateSigContext(const GUSISigContext * parent)
{
	return new GUSISigContext(parent);
}

// First of all, we define out of line versions of the signal set manipulators, giving them the
// added benefit of a range check.                                         
//                                                                         
// <POSIX functions for signal handling>=                                  
int (sigaddset)(sigset_t *set, int signo)			
{ 
	// \section{Definition of the signal handling POSIX functions}             
 //                                                                         
 // A considerable number of POSIX functions is concerned with signal handling. Many of them 
 // can profit from range checking on signal numbers. Additionally, we have to remap some of
 // the signals.                                                            
 //                                                                         
 // <Perform range check on [[signo]]>=                                     
 if (!GUSI_CASSERT_CLIENT(signo>0 && signo<NSIG))
 	return GUSISetPosixError(EINVAL);
 if (signo == 1)			// SIGABRT in MPW and CW
 	signo = SIGABRT;
 else if (signo == 4) 	// SIGINT in CW
 	signo = SIGINT;
	return sigaddset(set, signo);		
}

int (sigdelset)(sigset_t *set, int signo)			
{ 
	// \section{Definition of the signal handling POSIX functions}             
 //                                                                         
 // A considerable number of POSIX functions is concerned with signal handling. Many of them 
 // can profit from range checking on signal numbers. Additionally, we have to remap some of
 // the signals.                                                            
 //                                                                         
 // <Perform range check on [[signo]]>=                                     
 if (!GUSI_CASSERT_CLIENT(signo>0 && signo<NSIG))
 	return GUSISetPosixError(EINVAL);
 if (signo == 1)			// SIGABRT in MPW and CW
 	signo = SIGABRT;
 else if (signo == 4) 	// SIGINT in CW
 	signo = SIGINT;
	return sigdelset(set, signo);		
}

int (sigemptyset)(sigset_t *set)					
{ 
	return sigemptyset(set);			
}

int (sigfillset)(sigset_t *set)						
{ 
	return sigfillset(set);			
}

int (sigismember)(const sigset_t *set, int signo)	
{ 
	// \section{Definition of the signal handling POSIX functions}             
 //                                                                         
 // A considerable number of POSIX functions is concerned with signal handling. Many of them 
 // can profit from range checking on signal numbers. Additionally, we have to remap some of
 // the signals.                                                            
 //                                                                         
 // <Perform range check on [[signo]]>=                                     
 if (!GUSI_CASSERT_CLIENT(signo>0 && signo<NSIG))
 	return GUSISetPosixError(EINVAL);
 if (signo == 1)			// SIGABRT in MPW and CW
 	signo = SIGABRT;
 else if (signo == 4) 	// SIGINT in CW
 	signo = SIGINT;
	return sigismember(set, signo);	
}
// [[raise]] posts a signal against the current process.                   
//                                                                         
// <POSIX functions for signal handling>=                                  
int	raise(int signo)
{
	// \section{Definition of the signal handling POSIX functions}             
 //                                                                         
 // A considerable number of POSIX functions is concerned with signal handling. Many of them 
 // can profit from range checking on signal numbers. Additionally, we have to remap some of
 // the signals.                                                            
 //                                                                         
 // <Perform range check on [[signo]]>=                                     
 if (!GUSI_CASSERT_CLIENT(signo>0 && signo<NSIG))
 	return GUSISetPosixError(EINVAL);
 if (signo == 1)			// SIGABRT in MPW and CW
 	signo = SIGABRT;
 else if (signo == 4) 	// SIGINT in CW
 	signo = SIGINT;
	GUSIProcess::Instance()->SigProcess()->Post(signo);
	
	return 0;
}
// [[pthread_kill]] posts a signal against the specified thread.           
//                                                                         
// <POSIX functions for signal handling>=                                  
int pthread_kill(pthread_t thread, int signo)
{
	if (!signo)
		return thread ? 0 : ESRCH;
	// <Perform pthreads style range check on [[signo]]>=                      
 if (!GUSI_CASSERT_CLIENT(signo>0 && signo<NSIG))
 	return EINVAL;
	thread->SigContext()->Post(signo);
	thread->Wakeup();
	
	return 0;
}
// [[sigaction]] manipulates the signal action table.                      
//                                                                         
// <POSIX functions for signal handling>=                                  
int	sigaction(int signo, const struct sigaction * act, struct sigaction * oact)
{
	// \section{Definition of the signal handling POSIX functions}             
 //                                                                         
 // A considerable number of POSIX functions is concerned with signal handling. Many of them 
 // can profit from range checking on signal numbers. Additionally, we have to remap some of
 // the signals.                                                            
 //                                                                         
 // <Perform range check on [[signo]]>=                                     
 if (!GUSI_CASSERT_CLIENT(signo>0 && signo<NSIG))
 	return GUSISetPosixError(EINVAL);
 if (signo == 1)			// SIGABRT in MPW and CW
 	signo = SIGABRT;
 else if (signo == 4) 	// SIGINT in CW
 	signo = SIGINT;
	GUSISigProcess * proc = GUSIProcess::Instance()->SigProcess();
	if (oact)
		*oact = proc->GetAction(signo);
	if (act)
		if (proc->SetAction(signo, *act))
			return -1;
		else if (act->sa_handler==SIG_IGN)
			GUSIContext::CreateCurrent()->SigContext()->ClearPending(SigMask(signo));
	return 0;
}
// [[signal]] is the historical and rather inconvenient API for [[sigaction]].
//                                                                         
// <POSIX functions for signal handling>=                                  
__sig_handler signal(int signo, __sig_handler newhandler)
{
	struct sigaction oact;
	struct sigaction nact;
	
	nact.sa_handler	=	newhandler;
	nact.sa_mask	=	0;
	nact.sa_flags	= 	SA_RESETHAND;
	
	if (sigaction(signo, &nact, &oact))
		return reinterpret_cast<__sig_handler>(0);
	else
		return oact.sa_handler;
}
// [[sigpending]] returns the list of pending signals.                     
//                                                                         
// <POSIX functions for signal handling>=                                  
int	sigpending(sigset_t * pending)
{
	if (pending)
		*pending = GUSIProcess::Instance()->SigProcess()->Pending() 
			| GUSIContext::CreateCurrent()->SigContext()->Pending();
	return 0;
}
// [[pthread_sigmask]] and [[sigprocmask]] manipulate the signal mask.     
//                                                                         
// <POSIX functions for signal handling>=                                  
int pthread_sigmask(int how, const sigset_t * mask, sigset_t * omask)
{
	GUSISigContext * context = GUSIContext::CreateCurrent(true)->SigContext();
	if (omask)
		*omask = context->GetBlocked();
	if (mask)
		switch (how) {
		case SIG_BLOCK:
			context->SetBlocked(context->GetBlocked() | *mask);
			break;
		case SIG_SETMASK:
			context->SetBlocked(*mask);
			break;
		case SIG_UNBLOCK:
			context->SetBlocked(context->GetBlocked() & ~*mask);
			break;
		default:
			return EINVAL;
		}
	GUSIContext::Raise();
	return 0;
}

int	sigprocmask(int how, const sigset_t * mask, sigset_t * omask)
{
	return GUSISetPosixError(pthread_sigmask(how, mask, omask));
}
// [[sigsuspend]] waits for a signal to arrive. This is one of the few POSIX functions which always returns
// an error.                                                               
//                                                                         
// <POSIX functions for signal handling>=                                  
int	sigsuspend(const sigset_t * mask)
{
	GUSISigContext * context = GUSIContext::CreateCurrent()->SigContext();
	sigset_t	blocksave = context->GetBlocked();
	context->SetBlocked(*mask);
	GUSIContext::SigSuspend();
	sigset_t	blocknew  = context->GetBlocked();
	context->SetBlocked(blocksave | (blocknew & ~*mask));
	
	return GUSISetPosixError(EINTR);
}
// [[pause]] is a simpler (and less useful) form of [[sigsuspend]].        
//                                                                         
// <POSIX functions for signal handling>=                                  
int	pause()
{
	GUSIContext::CreateCurrent()->SigSuspend();
	
	return GUSISetPosixError(EINTR);
}
// [[sigwait]] waits for a blocked signal.                                 
//                                                                         
// <POSIX functions for signal handling>=                                  
int sigwait(const sigset_t * sigs, int * signo)
{
	if (!GUSI_CASSERT_CLIENT(sigs && signo && !(*sigs & ~GUSIContext::Blocked())))
		return GUSISetPosixError(EINVAL);
	GUSIContext::SigWait(*sigs);
	const sigset_t cursigs = *sigs & GUSIContext::Pending();
	for (*signo = 1; !sigismember(&cursigs, *signo); ++*signo)
		;
	GUSIContext::CreateCurrent()->SigContext()->ClearPending(SigMask(*signo));
	GUSIProcess::Instance()->SigProcess()->ClearPending(SigMask(*signo));
	
	return 0;
}
// [[abort]] raises [[SIGABRT]] and calls [[_exit]].                       
//                                                                         
// <POSIX functions for signal handling>=                                  
void abort(void)
{
	raise(SIGABRT);
	
	_exit(2);
}
// [[_exit]] is similar to [[exit]], but doesn't call handlers established with
// [[atexit]]. MPW already has a working implementation for [[_exit]].     
//                                                                         
// <POSIX functions for signal handling>=                                  
#ifdef __MWERKS__
extern int __aborting;

extern "C" void _exit(int code)
{
	__aborting = 1;
	
	exit(code);
}
#endif
// To handle alarms, we define the auxiliary class [[GUSIAlarm]].          
//                                                                         
// <POSIX functions for signal handling>=                                  
class GUSIAlarm : public GUSITimer {
public:
	GUSIAlarm(long interval = 0) : GUSITimer(true), fInterval(interval) {}
	
	virtual void	Wakeup();
	long			Restart(long interval = 0);
private:	
	long	fInterval;
};

void GUSIAlarm::Wakeup()
{
	GUSIProcess::Instance()->SigProcess()->Post(SIGALRM);
	if (fInterval)
		Sleep(fInterval, true);
}

long GUSIAlarm::Restart(long interval)
{
	fInterval = interval;
	RmvTime(Elem());
	long rest = tmCount;
	if (rest < 0)
		rest = -rest;
	else
		rest *= 1000;
	tmCount		= 0;
	tmWakeUp		= 0;
	tmReserved	= 0;
	InsXTime(Elem());
	
	return rest;
}

static auto_ptr<GUSIAlarm>	sGUSIAlarm;
// [[alarm]] raises a SIGALRM after a specified number of seconds has elapsed.
//                                                                         
// <POSIX functions for signal handling>=                                  
unsigned int alarm(unsigned int delay)
{
	unsigned int togo = 0;
	
	GUSIAlarm * a = sGUSIAlarm.get();
	if (a) 
		togo = static_cast<unsigned int>(a->Restart() / 1000000);
	else
		sGUSIAlarm = auto_ptr<GUSIAlarm>(a = new GUSIAlarm);
	if (a && delay)
		a->Sleep(delay*1000);
	
	return togo;
}
// [[ualarm]] provides a finer resolution and optionally offers the possibility to 
// generate repeated signals.                                              
//                                                                         
// <POSIX functions for signal handling>=                                  
useconds_t ualarm(useconds_t delay, useconds_t interval)
{
	useconds_t togo = 0;
	
	GUSIAlarm * a = sGUSIAlarm.get();
	if (a) 
		togo = static_cast<useconds_t>(a->Restart(static_cast<long>(interval)));
	else
		sGUSIAlarm = auto_ptr<GUSIAlarm>(a = new GUSIAlarm(interval));
	if (a && delay)
		a->MicroSleep(delay);
	
	return togo;
}

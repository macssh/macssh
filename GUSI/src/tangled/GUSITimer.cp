// <GUSITimer.cp>=                                                         
#include "GUSIInternal.h"
#include "GUSITimer.h"
#include "GUSIDiag.h"
#include "GUSIContext.h"

#include <errno.h>

// \section{Implementation of [[GUSITime]]}                                
//                                                                         
// The constructors need various multiplicators.                           
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
#if !TYPE_LONGLONG
GUSITime::GUSITime(int32_t val, Format format)
{
	Construct(S64Set(val), format);
}

GUSITime::GUSITime(uint32_t val, Format format)
{
	Construct(S64SetU(val), format);
}
#endif

void GUSITime::Construct(int64_t val, Format format)
{
	switch (format) {
	case seconds:
		fTime = S64Multiply(val, S64Set(1000000000));
		break;
	case ticks:
		fTime = S64Multiply(val, S64Set(16666667));
		break;
	case msecs:
		fTime = S64Multiply(val, S64Set(1000000));
		break;
	case usecs:
		fTime = S64Multiply(val, S64Set(1000));
		break;
	case nsecs:
		fTime = val;
		break;
	}
}
// The struct constructors are simple arithmetic.                          
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
GUSITime::GUSITime(const timeval & tv)
{
	*this = GUSITime(tv.tv_sec, seconds) + GUSITime(tv.tv_usec, usecs);
}

GUSITime::GUSITime(const timespec & ts)
{
	*this = GUSITime(ts.tv_sec, seconds) + GUSITime(ts.tv_nsec, nsecs);
}
// The basic get operation is the one returning an [[int64_t]].            
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
int64_t GUSITime::Get64(Format format)
{
	switch (format) {
	case seconds:
		return S64Div(fTime, S64Set(1000000000));
	case ticks:
		return S64Div(fTime, S64Set(16666667));
	case msecs:
		return S64Div(fTime, S64Set(1000000));
	case usecs:
		return S64Div(fTime, S64Set(1000));
	default:
	case nsecs:
		return fTime;
	}
}
// The struct casts break down the value into seconds and a remainder. For some
// reason, S64Divide is not defined for native functions.                  
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
time_t GUSITime::Deconstruct(int64_t & rem)
{
#if TYPE_LONGLONG
	rem = S64Mod(fTime, S64Set(1000000000));
	return static_cast<time_t>(U32SetU(SInt64ToUInt64(S64Div(fTime, S64Set(1000000000)))));
#else
	return static_cast<time_t>(U32SetU(SInt64ToUInt64(S64Divide(fTime, S64Set(1000000000), &rem))));
#endif
}

GUSITime::operator timeval()
{
	timeval	tv;
	int64_t	rem;
	
	tv.tv_sec	= Deconstruct(rem);
	tv.tv_usec	= S32Set(S64Div(rem, S64Set(1000)));
	
	return tv;
}

GUSITime::operator timespec()
{
	timespec	ts;
	int64_t		rem;
	
	ts.tv_sec	= Deconstruct(rem);
	ts.tv_nsec	= S32Set(rem);
	
	return ts;
}
// We construct a [[struct tm]] via [[LongSecondsToDate]].                 
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
GUSITime::operator tm()
{
	tm				t;
	LongDateRec		ldr;
	LongDateTime	ldt;
	
	ldt = Get64(seconds);
	LongSecondsToDate(&ldt, &ldr);
	t.tm_sec	=	ldr.ld.second;
	t.tm_min	=	ldr.ld.minute;
	t.tm_hour	=	ldr.ld.hour;
	t.tm_mday	=	ldr.ld.day;
	t.tm_mon	=	ldr.ld.month-1;
	t.tm_year	=	ldr.ld.year-1900;	
	t.tm_wday	=	ldr.ld.dayOfWeek-1;
	t.tm_yday	=	ldr.ld.dayOfYear-1;
	t.tm_isdst	=	-1;
	
	return t;
}
// Constructing a [[GUSITime]] from a [[struct tm]] is a bit trickier because
// field values are not guaranteed to be in range and [[LongDateToSeconds]] is
// not infinitely tolerant.                                                
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
GUSITime::GUSITime(const tm & t)
{
	LongDateRec		ldr;
	LongDateTime	ldt;
	
	ldr.ld.era 		= 	0;
	ldr.ld.year		=	t.tm_year+1900;
	ldr.ld.month	=	t.tm_mon+1;
	ldr.ld.day		=	t.tm_mday ? t.tm_mday : 1;
	ldr.ld.hour		=	t.tm_hour>=0 ? t.tm_hour : 0;
	ldr.ld.minute	=	t.tm_min>=0 ? t.tm_min : 0;
	ldr.ld.second	=	t.tm_sec>=0 ? t.tm_sec : 0;
	
	LongDateToSeconds(&ldr, &ldt);
	if (!t.tm_mday)
		ldt = S64Subtract(ldt, S64Set(86400));
	if (t.tm_hour < 0)
		ldt = S64Add(ldt, S64Multiply(S64Set(t.tm_hour), S64Set(3600)));
	if (t.tm_min < 0)
		ldt = S64Add(ldt, S64Multiply(S64Set(t.tm_min), S64Set(60)));
	if (t.tm_sec < 0)
		ldt = S64Add(ldt, S64Set(t.tm_sec));
		
	Construct(ldt, seconds);
}
// To calculate the current time, we need to combine [[GetDateTime]], which gives
// seconds since 1904, with [[Microseconds]], which gives microseconds since startup.
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
int64_t		GUSITime::sTimeOffset;
timezone 	GUSITime::sTimeZone;

GUSITime GUSITime::Now()
{
	if (S64Not(sTimeOffset)) {
		GUSITime	s;
		GUSITime	zone;
		GUSITime	us;
		
		{
			uint32_t	secs;
			GetDateTime(&secs);
			s = GUSITime(secs, seconds);
		}
		{
			UnsignedWide	u;
			Microseconds(&u);
			us = GUSITime(UInt64ToSInt64(UnsignedWideToUInt64(u)), usecs);
		}
		{
			MachineLocation loc;
			ReadLocation(&loc);
			int32_t delta = loc.u.gmtDelta & 0x00FFFFFF;
			if (delta & 0x00800000)
				delta |= 0xFF000000;
			zone = GUSITime(delta, seconds);
			sTimeZone.tz_minuteswest = zone.Get(seconds) / 60;
			sTimeZone.tz_dsttime	 = (loc.u.gmtDelta & 0xFF000000) != 0;
		}
		s -= zone;
		sTimeOffset =  s - us;
		
		return s;
	} else {
		UnsignedWide	us;
		Microseconds(&us);
		GUSITime	t(UInt64ToSInt64(UnsignedWideToUInt64(us)), usecs);
		
		return S64Add(int64_t(t), sTimeOffset);
	}
}
// The time zone is cached by [[Now]].                                     
//                                                                         
// <Member functions for class [[GUSITime]]>=                              
timezone & GUSITime::Zone()
{
	Now();
	return sTimeZone;
}

GUSITime GUSITime::Local2GMTime()
{
	Now();
	
	GUSITime zone(static_cast<int32_t>(60*sTimeZone.tz_minuteswest), seconds);
	
	return *this - zone;
}

GUSITime GUSITime::GM2LocalTime()
{
	Now();
	
	GUSITime zone(static_cast<int32_t>(60*sTimeZone.tz_minuteswest), seconds);
	
	return *this+zone;
}
// \section{Implementation of [[GUSITimer]]}                               
//                                                                         
// [[GUSITimerProc]] wakes up the context.                                 
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
static void GUSITimerProc(TMTask * tm)
{
	GUSITimer * timer = static_cast<GUSITimer *>(tm);
	
	GUSIProcess::A5Saver	saveA5(timer->Context());
	
	timer->Wakeup();
}

GUSI_COMPLETION_PROC_A1(GUSITimerProc, TMTask)
// The default wakeup action simply wakes up the associated context.       
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
void GUSITimer::Wakeup()
{ 
	Context()->Wakeup();					
}
// When a process terminates, all remaining timers get destroyed.          
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
GUSITimer::TimerQueue::~TimerQueue()
{
	GUSIContext::LiquidateAll();
}

void GUSIKillTimers(void * timers)
{
	GUSITimer::Queue * q = static_cast<GUSITimer::Queue *>(timers);
	
	while (q->fTimer)
		q->fTimer->Kill();
		
	delete q;
}
// On constructing the first [[GUSITimer]] in a program, we initialize [[sTimerProc]].
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
TimerUPP 				GUSITimer::sTimerProc = (TimerUPP)0;
GUSITimer::TimerQueue	GUSITimer::sTimerQueue;

GUSITimer::GUSITimer(bool wakeup, GUSIContext * context)
{
	if (!context)
		context 	= GUSIContext::CreateCurrent();
	if (wakeup) {
		if (!sTimerProc)
			sTimerProc 	= NewTimerProc(reinterpret_cast<TimerProcPtr>(GUSITimerProcEntry));
		tmAddr 			= sTimerProc;
	} else
		tmAddr  		= 0;
	tmCount				= 0;
	tmWakeUp			= 0;
	tmReserved			= 0;
	InsXTime(Elem());
	fQueue 				= sTimerQueue.get(context);
	fQueue->fContext	= context;
	fNext 				= fQueue->fTimer;
	fQueue->fTimer 		= this;
}
// We support both normal timing and drift free timers.                    
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
void GUSITimer::Sleep(long ms, bool driftFree)
{
	if (!driftFree)
		tmWakeUp = 0;
	PrimeTime(Elem(), ms);
}
// To reset the timer, we remove it and insert it again.                   
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
void GUSITimer::Reset()
{
	RmvTime(Elem());
	InsXTime(Elem());
}
// Upon destruction, we remove the time manager task. The [[fQueue]] element serves as a
// flag so we don't get killed twice.                                      
//                                                                         
// <Member functions for class [[GUSITimer]]>=                             
void GUSITimer::Kill()
{
	if (!fQueue)	// We are dead already
		return;

	RmvTime(Elem());
	GUSITimer ** queue = &fQueue->fTimer;
	
	while (*queue)
		if (*queue == this) {
			*queue = fNext; 
			
			break;
		} else
			queue = &(*queue)->fNext;
	fNext 	= 0;
	fQueue	= 0;
}

GUSITimer::~GUSITimer()
{
	Kill();
}

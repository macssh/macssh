/*********************************************************************
File		:	GUSI				-	Grand Unified Socket Interface
File		:	GUSIThreadTest	-	Testthread related features
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:54  chombier
Initial import

Revision 1.1  2000/10/29 20:31:53  neeri
Releasing 2.1.3

*********************************************************************/

#include "GUSITest.h"

#include <pthread.h>
#include <sched.h>

#include <LowMem.h>

typedef struct {
	pthread_mutex_t	fMutex;
	int				fCounter;
	int				fLoops;
	char			fType;
} Shared;

void *
MutexPerformanceProc(void *arg)
{
	Shared *	shared = (Shared *)arg;
	
	int		i;

	if (shared->fType == 'm') {
		for (i = 0; i < shared->fLoops; i++) {
			pthread_mutex_lock(&shared->fMutex);
			shared->fCounter++;
			pthread_mutex_unlock(&shared->fMutex);
		}
	} else {
		pthread_mutex_lock(&shared->fMutex);
		pthread_mutex_unlock(&shared->fMutex);
		for (i = 0; i < shared->fLoops; i++) {
			shared->fCounter++;
			if (shared->fType == 'y')
				sched_yield();
		}
	}
	return(NULL);
}
/* end incr */

void MutexPerformance(char ch1, char ch2, const char * cmd)
{
	int 			nloops;
	int 			nthreads;
	pthread_t		tid[20];
	Shared			shared;
	int				i;
	int				fairness;
	unsigned long 	start_time;
	unsigned long 	stop_time;
	
	if (sscanf(cmd, "%d %d", &shared.fLoops, &nthreads) != 2)
		Usage(ch1, ch2);
	else {
		if (nthreads < 1) {
			printf("Too few threads specified, using 1\n");
			nthreads = 1;
		} else if (nthreads > 20) {
			printf("Too many threads specified, using 20\n");
			nthreads = 20;
		}
		shared.fCounter = 0;
		shared.fType	= ch1;
		pthread_mutex_init(&shared.fMutex, NULL);
		pthread_mutex_lock(&shared.fMutex);

		for (i = 0; i < nthreads; i++) 
			pthread_create(&tid[i], NULL, MutexPerformanceProc, &shared);

		start_time = LMGetTicks();
		
		pthread_mutex_unlock(&shared.fMutex);

		/* 4wait for all the threads */
		pthread_join(tid[0], NULL);
		fairness = shared.fCounter;
		for (i = 1; i < nthreads; i++) 
			pthread_join(tid[i], NULL);
		
		stop_time = LMGetTicks();
		
		printf("Time: %f seconds, Fairness: %.2f%\n", 
			(stop_time - start_time) / 60.0,
			(fairness * 100.0) / ((shared.fLoops-1) * nthreads + 1));
		if (shared.fCounter != shared.fLoops * nthreads)
			printf("error: counter = %ld\n", shared.fCounter);
		
		pthread_mutex_destroy(&shared.fMutex);
	}
}

void Sleep(char ch1, char ch2, const char * cmd)
{
	int 			seconds;
	
	if (sscanf(cmd, "%d", &seconds) != 1)
		Usage(ch1, ch2);
	else 
		printf("Remaining: %ds\n", sleep(seconds));
}

void Times(char ch1, char ch2, const char * cmd)
{
	time_t		lmk, gmk;
	time_t 		now = time(NULL);
	struct tm *	t	= localtime(&now);
	printf("Localtime %d/%d/%d %d:%02d:%02d %s\n", 
		t->tm_year+1900, t->tm_mon+1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec, t->tm_isdst ? "DST" : "");
	lmk = mktime(t);
	
	t	= gmtime(&now);
	printf("GMtime    %d/%d/%d %d:%02d:%02d %s\n", 
		t->tm_year+1900, t->tm_mon+1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec, t->tm_isdst ? "DST" : "");
	gmk = mktime(t);
	
	printf("Now %u Local %u GM %u\n", now, lmk, gmk);
}

main(int argc, char ** argv)
{
	printf("GUSIFileTest		MN 25OCt00\n\n");

	COMMAND('m', 'p', MutexPerformance,  "#loops #threads", "Test mutex performance");
	COMMAND('y', 'p', MutexPerformance,  "#loops #threads", "Test yield performance");
	COMMAND('n', 'p', MutexPerformance,  "#loops #threads", "Test noyield performance");
	COMMAND('s', 'l', Sleep,  			 "seconds", "sleep");
	COMMAND('t', 'm', Times,  			 "", "Test time related functions");
	
	RunTest(argc, argv);
}

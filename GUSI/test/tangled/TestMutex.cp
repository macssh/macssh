// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestMutex.nw		-	Test pthreads mutex facilities                
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                                
// % Revision 1.5  2000/05/23 07:26:10  neeri                              
// % Tweak tests                                                           
// %                                                                       
// % Revision 1.4  1999/08/02 07:03:55  neeri                              
// % Getting ready for 2.0b6                                               
// %                                                                       
// % Revision 1.3  1999/05/30 03:09:35  neeri                              
// % Added support for MPW compilers                                       
// %                                                                       
// % Revision 1.2  1999/03/17 09:05:16  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.1  1998/01/25 21:02:55  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing the Pthreads Mutex Facilities}                         
//                                                                         
// This program tests mutexes and condition variables.                     
//                                                                         
//                                                                         
// <TestMutex.cp>=                                                         
#include <Events.h>

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// We test a mutex and one condition variable. To pretend they make sense, 
// we add a global, too.                                                   
//                                                                         
// <Mutex and condition for [[TestMutex]]>=                                
pthread_mutex_t	market 		= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	apple		= PTHREAD_COND_INITIALIZER;

int gApples = 20;
// And so are the piggies and the big bad wolf.                            
//                                                                         
// <Thread procedures for [[TestMutex]]>=                                  
extern "C" void * GoToMarket(void * s)
{
	int apples = 0;
	
	for (int leave = 0; !leave;) {
		usleep((rand() % 20) * 10000);
		cout << TickCount() << ": Piggie " << (int)s << " goes to market\n";
		pthread_mutex_lock(&market);
		cout << TickCount() << ": Piggie " << (int)s << " in market\n";
		if (!(gApples % 7)) {
			cout << TickCount() << ": Piggie " << (int)s << " wakes up wolves\n";
			pthread_cond_broadcast(&apple);
		}
		if (gApples) {
			++apples;
			--gApples;
			cout << TickCount() << ": Piggie " << (int)s << " got an apple\n";
		} else {
			cout << TickCount() << ": Piggie " << (int)s << " got no apple\n";
			leave = 1;
		}
		cout << TickCount() << ": Piggie " << (int)s << " leaves market.\n";
		pthread_mutex_unlock(&market);
	}
	cout << TickCount() << ": Piggie " << (int)s << " quits with " 
		<< apples << " apples\n";
	
	return 0;
}

extern "C" void * HuffAndPuff(void * s)
{
	for (int leave = 0; !leave;) {
		usleep((rand() % 5) * 10000);
		cout << TickCount() << ": Wolf " << (int)s << " starts huffing\n";
		pthread_mutex_lock(&market);
		cout << TickCount() << ": Wolf " << (int)s << " starts puffing\n";
		pthread_cond_wait(&apple, &market);
		cout << TickCount() << ": Wolf " << (int)s << " stops huffing and puffing with "
			<< gApples << " apples left\n";
		leave = !gApples;
		pthread_mutex_unlock(&market);
	}
	
	return 0;
}
// The main program is quite simple.                                       
//                                                                         
// <Main program for [[TestMutex]]>=                                       
void main()
{
	pthread_t	piggie1;
	pthread_t	piggie2;
	pthread_t	piggie3;
	pthread_t	wolf1;
	pthread_t	wolf2;
	void *		whocares;
	
	srand(time(NULL));
	
	pthread_create(&piggie1, 0, GoToMarket, (void *)1);
	pthread_create(&piggie2, 0, GoToMarket, (void *)2);
	pthread_create(&piggie3, 0, GoToMarket, (void *)3);
	pthread_create(&wolf1, 0, HuffAndPuff, (void *)1);
	pthread_create(&wolf2, 0, HuffAndPuff, (void *)2);
	
	pthread_join(piggie1, &whocares);
	pthread_join(piggie2, &whocares);
	pthread_join(piggie3, &whocares);
	
	pthread_cond_broadcast(&apple);
	pthread_join(wolf1, &whocares);
	pthread_join(wolf2, &whocares);
	
	cout <<flush;
}

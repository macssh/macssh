// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestTimers.nw		-	Test the GUSITiimer class                    
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                               
// % Revision 1.8  2000/03/15 07:08:59  neeri                              
// % New yield model                                                       
// %                                                                       
// % Revision 1.7  1999/05/30 03:09:36  neeri                              
// % Added support for MPW compilers                                       
// %                                                                       
// % Revision 1.6  1999/03/17 09:05:17  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.5  1998/01/25 20:54:02  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// % Revision 1.4  1997/11/13 21:12:15  neeri                              
// % Fall 1997                                                             
// %                                                                       
// % Revision 1.3  1996/11/24  13:00:30  neeri                             
// % Fix comment leaders                                                   
// %                                                                       
// % Revision 1.2  1996/11/24  12:52:12  neeri                             
// % Added GUSIPipeSockets                                                 
// %                                                                       
// % Revision 1.1  1996/11/18  00:53:47  neeri                             
// % TestTimers (basic threading/timer test) works                         
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing the [[GUSITimer]] class}                               
//                                                                         
// This is a test program for the [[GUSITimer]] class. We test it by solving
// the Hamming Problem: Printing all multiples of 3, 5, and 7, in ascending
// order, without duplicates. We solve this with three threads sleeping 300
// 500, and 700 milliseconds at a time.                                    
//                                                                         
//                                                                         
// <TestTimers.cp>=                                                        
#define GUSI_SOURCE

#include "GUSIBasics.h"
#include "GUSIContext.h"
#include "GUSITimer.h"

#include <iostream.h>
#include <iomanip.h>

// The only global status we need is [[gLastNumber]], which serves both to eliminate
// duplicates and to terminate the program                                 
//                                                                         
// <Global status for [[TestTimers]]>=                                     
long gLastNumber	=	0;
// And so is the multiplier thread itself.                                 
//                                                                         
// <Thread procedure for [[TestTimers]]>=                                  
pascal void * MultiplierThread(void * inc)
{
	long	cur			= 0;
	long	incr		= (long) inc;
	
	GUSITimer * timer 	= new GUSITimer();
	
	for (;;) {
		timer->Sleep(incr*200, true);
		GUSIContext::Yield(kGUSIBlock);
		cur += incr;
		if (gLastNumber > cur)
			cout << "Oops... wanted to print " << cur
				 << " but we are already at " << gLastNumber << "." << endl;
		else if (gLastNumber == cur)
			;
		else 
			cout << setw(5) << (gLastNumber = cur) << setw(12) << TickCount() << endl;
	}
	
	return 0;
}
// The main program is quite simple.                                       
//                                                                         
// <Main program for [[TestTimers]]>=                                      
void main()
{
	void * 		dontCare;
	ThreadID	mul3;
	ThreadID	mul5;
	ThreadID	mul7;
	
	GUSINewThread(kCooperativeThread, MultiplierThread, (void *) 3, 20480, kCreateIfNeeded, &dontCare, &mul3);
	GUSINewThread(kCooperativeThread, MultiplierThread, (void *) 5, 20480, kCreateIfNeeded, &dontCare, &mul5);
	GUSINewThread(kCooperativeThread, MultiplierThread, (void *) 7, 20480, kCreateIfNeeded, &dontCare, &mul7);

	while (gLastNumber < 50) {
		GUSIContext::Yield(kGUSIPoll);
	}
	
	DisposeThread(mul3, 0, false);
	DisposeThread(mul5, 0, false);
	DisposeThread(mul7, 0, false);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestPipes.nw		-	Test the GUSIPipeSocket class                 
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                                
// % Revision 1.8  2000/05/23 07:26:11  neeri                              
// % Tweak tests                                                           
// %                                                                       
// % Revision 1.7  2000/03/15 07:08:59  neeri                              
// % New yield model                                                       
// %                                                                       
// % Revision 1.6  1999/08/02 07:03:55  neeri                              
// % Getting ready for 2.0b6                                               
// %                                                                       
// % Revision 1.5  1999/03/17 09:05:17  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.4  1998/01/25 20:54:01  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// % Revision 1.3  1997/11/13 21:12:14  neeri                              
// % Fall 1997                                                             
// %                                                                       
// % Revision 1.2  1996/11/24  13:00:29  neeri                             
// % Fix comment leaders                                                   
// %                                                                       
// % Revision 1.1  1996/11/24  12:52:11  neeri                             
// % Added GUSIPipeSockets                                                 
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing the [[GUSIPipeSocket]] class}                          
//                                                                         
// This is a test program for the [[GUSIPipeSocket]] class.                
//                                                                         
//                                                                         
// <TestPipes.cp>=                                                         
#define GUSI_SOURCE

#include "GUSISocket.h"

#define GUSI_INTERNAL

#include "GUSIPipe.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>

// And so are the producer and consumer threads.                           
//                                                                         
// <Thread procedures for [[TestPipes]]>=                                  
pascal void * ProducerThread(void * s)
{
	char 			buffer[200];
	GUSISocket * 	sock = (GUSISocket *) s;
	unsigned long	written = 0;
	
	while (cin.getline(buffer, 200)) {
		int len = strlen(buffer);
		if (sock->write(buffer, len) < 0) {
			cout << "Write Error: " << errno << "\n";
			
			break;
		}
		cout << "Written " << (written += len) << " " << TickCount() << "\n";
	}
	
	sock->close();
	
	return (void *) 1;
}
pascal void * ConsumerThread(void * s)
{
	char 			buffer[1000];
	GUSISocket * 	sock = (GUSISocket *) s;
	unsigned long	read = 0;
	int				len;
	
	while (len = sock->read(buffer, 1000)) {
		if (len < 0) {
			cout << "Read Error: " << errno << "\n";
			
			break;
		}
		cout << "Read " << (read += len) << " " << TickCount() << "\n";
	}
	
	sock->close();

	return (void *) 1;
}
// The main program is quite simple.                                       
//                                                                         
// <Main program for [[TestPipes]]>=                                       
void main()
{
	GUSISocket *	s[2];
	
	GUSIPipeFactory::Instance()->socketpair(0, 0, 0, s);

	ThreadID	producer;
	ThreadID	consumer;
	void * 		producerDone = 0;
	void * 		consumerDone = 0;
	
	GUSINewThread(kCooperativeThread, ProducerThread, (void *) s[0], 20480, kCreateIfNeeded, &producerDone, &producer);
	GUSINewThread(kCooperativeThread, ConsumerThread, (void *) s[1], 20480, kCreateIfNeeded, &consumerDone, &consumer);

	while (!producerDone || !consumerDone) {
		cout << "Main Yielding " << TickCount() << "\n";
		GUSIContext::Yield(kGUSIPoll);
		cout << "Main Back " << TickCount() << "\n";
	}
	cout << flush;
}

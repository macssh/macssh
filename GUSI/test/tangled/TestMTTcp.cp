// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestMTTcp.nw		-	Test the GUSIMTTcpSocket class                
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                                
// % Revision 1.9  2000/10/16 04:15:27  neeri                              
// % Use s->close() instead of delete s                                    
// %                                                                       
// % Revision 1.8  2000/03/15 07:08:58  neeri                              
// % New yield model                                                       
// %                                                                       
// % Revision 1.7  1999/08/02 07:03:55  neeri                              
// % Getting ready for 2.0b6                                               
// %                                                                       
// % Revision 1.6  1999/05/30 03:09:35  neeri                              
// % Added support for MPW compilers                                       
// %                                                                       
// % Revision 1.5  1999/03/17 09:05:16  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.4  1998/10/11 16:45:28  neeri                              
// % Ready to release 2.0a2                                                
// %                                                                       
// % Revision 1.3  1998/01/25 20:54:00  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// % Revision 1.2  1996/12/22 19:57:59  neeri                              
// % TCP streams work                                                      
// %                                                                       
// % Revision 1.1  1996/12/16 02:12:43  neeri                              
// % TCP Sockets sort of work                                              
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing the [[GUSIMTTcpSocket]] class}                         
//                                                                         
// This is a test program for the [[GUSIMTTcpSocket]] class. Because we lack
// a lot of infrastructure right now, we'll just connect to a hardcoded echo
// port.                                                                   
//                                                                         
//                                                                         
// <TestMTTcp.cp>=                                                         
#define GUSI_SOURCE
#define GUSI_INTERNAL

#include "GUSIMTTcp.h"
#include "GUSIDiag.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

// And so are the producer and consumer threads.                           
//                                                                         
// <Thread procedures for [[TestMTTcp]]>=                                  
pascal void * ProducerThread(void * s)
{
	char 			buffer[8000];
	GUSISocket * 	sock = (GUSISocket *) s;
	unsigned long	written = 0;
	int				len;
	
	while (len = read(0, buffer, 8000)) {
		if (sock->write(buffer, len) < 0) {
			cout << "Write Error: " << errno << "\n";
			
			break;
		}
		written += len;
		cout << "Written " << written << " " << TickCount() << "\n";
	}
	sock->shutdown(1);
	
	return (void *) 1;
}
pascal void * ConsumerThread(void * s)
{
	char 			buffer[9000];
	GUSISocket * 	sock = (GUSISocket *) s;
	unsigned long	read = 0;
	int				len;
	
	while (len = sock->read(buffer, 9000)) {
		if (len < 0) {
			cout << "Read Error: " << errno << "\n";
			
			break;
		}
		cout << "Read " << (read += len) << " " << TickCount() << "\n";
	}
	
	return (void *) 1;
}
pascal void * ListenerThread(void * s)
{
	GUSISocket * 	sock = (GUSISocket *) s;
	GUSISocket * 	client;
	sockaddr_in		addr;
	socklen_t		len = sizeof(addr);
	
	if (client = sock->accept(&addr, &len)) {
		cout << "Accepted " 
			 << hex << addr.sin_addr.s_addr << dec
			 << ":" << addr.sin_port << " "<< TickCount() << "\n";
		
		client->close();
	}
	return (void *) 1;
}
// The main program is quite simple.                                       
//                                                                         
// <Main program for [[TestMTTcp]]>=                                       
void main()
{
	GUSISocket *	s;
	GUSISocket *	l;
	sockaddr_in		addr;
	
	addr.sin_family		=	AF_INET;
	addr.sin_addr.s_addr=	0x7F000001; 	// localhost
	addr.sin_port		=	7;				// Echo port
	s 					= 	
		GUSIMTTcpFactory::Instance()->socket(AF_INET, SOCK_STREAM, 0);
		
	if (s->connect(&addr, sizeof(addr))) {
		cerr << "Error connecting: " << errno << endl;
		cerr << "You should run MacTCPWatcher during this test" << endl;
		
		goto killS;
	}

	addr.sin_family		=	AF_INET;
	addr.sin_addr.s_addr=	0;
	addr.sin_port		=	7000;				
	l 					= 
		GUSIMTTcpFactory::Instance()->socket(AF_INET, SOCK_STREAM, 0);

	if (l->bind(&addr, sizeof(addr))) {
		cerr << "Error binding: " << errno << endl;
		
		goto killL;
	}
	if (l->listen(5)) {
		cerr << "Error listening: " << errno << endl;
		
		goto killL;
	}
	
	{
		ThreadID	producer;
		ThreadID	consumer;
		ThreadID	listener;
		void * 		producerDone = 0;
		void * 		consumerDone = 0;
		void * 		listenerDone = 0;
		
		GUSINewThread(kCooperativeThread, ProducerThread, (void *) s, 50000, kCreateIfNeeded, &producerDone, &producer);
		GUSINewThread(kCooperativeThread, ConsumerThread, (void *) s, 50000, kCreateIfNeeded, &consumerDone, &consumer);
		GUSINewThread(kCooperativeThread, ListenerThread, (void *) l, 50000, kCreateIfNeeded, &listenerDone, &listener);
	
		while (!producerDone || !consumerDone || !listenerDone) {
			cout << "Main Yielding " << TickCount() << "\n";
			GUSIContext::Yield(kGUSIPoll);
			cout << "Main Back " << TickCount() << "\n";
		}
		cout << flush;
	}

killL:	
	l->close();
killS:
	s->close();
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestDaytime.nw		-	Test files/UDP with a daytime               
// %										request                                                      
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
// % Revision 1.4  1999/08/05 05:57:05  neeri                              
// % Getting ready for 2.0b7                                               
// %                                                                       
// % Revision 1.3  1999/05/30 03:07:08  neeri                              
// % Added timeout since UDP packets can get lost                          
// %                                                                       
// % Revision 1.2  1999/03/17 09:05:15  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.1  1998/10/25 11:57:41  neeri                              
// % Ready to release 2.0a3                                                
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{A Daytime client}                                              
//                                                                         
// To test UDP, we send a daytime request. In blatant abuse of good programming
// practice, we simply assume that the request won't get lost.             
//                                                                         
//                                                                         
// <TestDaytime.cp>=                                                       
#include <GUSIMTUdp.h>

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>

// This program does no multithreading, so it just consists of a single loop.
//                                                                         
// <Main program for [[TestDaytime]]>=                                     
void main(int, char ** argv)
{
	sockaddr_in		addr;
	addr.sin_family		=	AF_INET;
	hostent * host 		=	gethostbyname(argv[1]); 
	if (!host) {
		cerr << "Host not found: " << h_errno << endl;
		
		exit(1);
	}
	memcpy(&addr.sin_addr, host->h_addr, sizeof(in_addr));
	servent * serv 		= 	getservbyname("daytime", "udp");
	addr.sin_port		=	serv ? serv->s_port : 13;				
	int daytime			= 	socket(AF_INET, SOCK_DGRAM, 0);

	if (sendto(daytime, "hi", 2, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
		cerr << "Error sending: " << errno << endl;
		
		exit(1);
	}

	fd_set fd_daytime;
	FD_ZERO(&fd_daytime);
	FD_SET(daytime, &fd_daytime);
	timeval	tv;
	tv.tv_sec = 10;
	tv.tv_usec= 0;
	
	switch (select(32, &fd_daytime, NULL, NULL, &tv)) {
	case -1:
		cout << "Select Error: " << errno << endl;
		
		exit(1);
	case 0:
		cout << "Select Timed Out\n" << endl;
		
		exit(1);
	}
	
	char 	buffer[120];

	if (recv(daytime, buffer, 120, 0) < 0) {
		cout << "Read Error: " << errno << endl;
		
		exit(1);
	} else {
		*strchr(buffer, '\012') = 0;
		
		cerr << buffer << endl;
	}
}

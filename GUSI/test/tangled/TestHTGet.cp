// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestHTGet.nw		-	Test files/TCP with a HTTP Get                
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                                
// % Revision 1.6  2001/01/22 04:30:13  neeri                              
// % Fix if no output file is specified                                    
// %                                                                       
// % Revision 1.5  2000/10/16 04:15:27  neeri                              
// % Use s->close() instead of delete s                                    
// %                                                                       
// % Revision 1.4  1999/05/30 03:09:34  neeri                              
// % Added support for MPW compilers                                       
// %                                                                       
// % Revision 1.3  1999/03/17 09:05:16  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.2  1998/10/11 16:45:27  neeri                              
// % Ready to release 2.0a2                                                
// %                                                                       
// % Revision 1.1  1998/01/25 21:02:54  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{A HTTP retriever}                                              
//                                                                         
// To test TCP and file system correctness and performance, we try to      
// write a HTTP page into a file.                                          
//                                                                         
//                                                                         
// <TestHTGet.cp>=                                                         
#define GUSI_SOURCE
#define GUSI_INTERNAL

#include "GUSIMTTcp.h"
#include "GUSIMTNetDB.h"
#include "GUSIMacFile.h"
#include "GUSIDiag.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

// This program does no multithreading, so it just consists of a single loop.
//                                                                         
// <Main program for [[TestHTGet]]>=                                       
void main(int, char ** argv)
{
	GUSISocket *  	f;
	GUSISocket *	s;

	GUSIMTNetDB::Instantiate();
	
	sockaddr_in		addr;
	addr.sin_family		=	AF_INET;
	hostent * host 		=	GUSINetDB::Instance()->gethostbyname(argv[1]); 
	if (!host) {
		cerr << "Host not found: " << h_errno << endl;
		
		exit(1);
	}
	memcpy(&addr.sin_addr, host->h_addr, sizeof(in_addr));
	servent * serv 		= 	GUSINetDB::Instance()->getservbyname("http", "tcp");
	addr.sin_port		=	serv ? serv->s_port : 80;				
	s 					=
		GUSIMTTcpFactory::Instance()->socket(AF_INET, SOCK_STREAM, 0);

	if (s->connect(&addr, sizeof(addr))) {
		cerr << "Error connecting: " << errno << endl;
		
		goto killS;
	}

	s->write("GET ", 4);
	s->write(argv[2], strlen(argv[2]));
	s->write("\015\012\015\012", 4);	
		
	if (argv[3]) {
		if (!(f = GUSIDeviceRegistry::Instance()->open(argv[3], O_WRONLY|O_CREAT|O_TRUNC))) {
			cerr << "Error opening file: " << errno << endl;
			
			goto killS;
		}
	} else
		f = 0;

	int 	len;
	char 	buffer[8000];
	while (len = s->read(buffer, 8000)) {
		if (len < 0) {
			cout << "Read Error: " << errno << "\n";
			
			break;
		}
		if (f && f->write(buffer, len) < 0) {
			cout << "Write Error: " << errno << "\n";
			
			break;
		}
	}
	
killF:
	if (f)
		f->close();
killS:
	s->close();
}

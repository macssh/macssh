// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestDuplicate.nw	-	Test files with a copy command             
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                            
// % Revision 1.6  2001/01/17 08:56:10  neeri                              
// % Spin cursor, write diagnostics to file                                
// %                                                                       
// % Revision 1.5  2000/10/16 04:15:27  neeri                              
// % Use s->close() instead of delete s                                    
// %                                                                       
// % Revision 1.4  1999/05/30 03:09:34  neeri                              
// % Added support for MPW compilers                                       
// %                                                                       
// % Revision 1.3  1999/03/17 09:05:15  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.2  1998/08/01 21:32:14  neeri                              
// % About ready for 2.0a1                                                 
// %                                                                       
// % Revision 1.1  1998/01/25 21:02:53  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{A File Duplicator}                                             
//                                                                         
// To test file system socket correctness and performance, we try to       
// duplicate a file.                                                       
//                                                                         
//                                                                         
// <TestDuplicate.cp>=                                                     
#define GUSI_SOURCE

#include "GUSIDevice.h"
#include "GUSISocket.h"
#include "GUSIDiag.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <CursorCtl.h>

// This program does no multithreading, so it just consists of a single loop.
//                                                                         
// <Main program for [[TestDuplicate]]>=                                   
void main(int, char ** argv)
{
	GUSI_diag_log	= "TestDuplicate";

	long bigbuf = argv[3]?atol(argv[3]):0;
	
	GUSISocket * src = 
		GUSIDeviceRegistry::Instance()->open(argv[1], O_RDONLY);
	if (!src) {
		cerr << "Error opening source file: " << errno << endl;
			
		exit(1);
	}
	if (bigbuf && src->setsockopt(SOL_SOCKET, SO_RCVBUF, &bigbuf, sizeof(long)) < 0)
		cerr << "setsockopt(src) returned error: " << errno << endl;
	GUSISocket * dst = 
		GUSIDeviceRegistry::Instance()->open(argv[2], O_WRONLY|O_CREAT|O_TRUNC);
	if (!dst) {
		cerr << "Error opening destination file: " << errno << endl;
			
		goto killS;
	}
	if (bigbuf && dst->setsockopt(SOL_SOCKET, SO_SNDBUF, &bigbuf, sizeof(long)) < 0)
		cerr << "setsockopt(dst) returned error: " << errno << endl;

	bigbuf = bigbuf ? (bigbuf >> 1) : 4096;
	
	{
		long	total = 0;
		int 	len;
		char *	buffer = new char[bigbuf];
		while (len = src->read(buffer, bigbuf)) {
			if (len < 0) {
				cout << "Read Error: " << errno << "\n";
				
				break;
			}
			if (dst->write(buffer, len) < 0) {
				cout << "Write Error: " << errno << "\n";
				
				break;
			}
			total += len;
			SpinCursor(1);
		}
		cout << "Total read: " << (unsigned long)total << endl;
	}
	
	dst->close();
killS:
	src->close();
	
	exit(0);
}

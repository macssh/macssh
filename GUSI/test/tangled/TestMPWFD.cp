// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestMPWFD.nw		-	Test file descriptors with MPW.               
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                                
// % Revision 1.3  1999/07/19 06:25:14  neeri                              
// % Getting ready for 2.0b4                                               
// %                                                                       
// % Revision 1.2  1999/05/30 03:09:35  neeri                              
// % Added support for MPW compilers                                       
// %                                                                       
// % Revision 1.1  1998/08/01 21:32:15  neeri                              
// % About ready for 2.0a1                                                 
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing standard MPW descriptors}                              
//                                                                         
// Now we test the standard MPW file descriptors.                          
//                                                                         
// <TestMPWFD.cp>=                                                         
#include <unistd.h>
#include <fcntl.h>
#include <MacTypes.h>

// <Main program for [[TestMPWFD]]>=                                       
void main(int, char ** argv)
{
	write(1, "Hello, world!\n", 14);
	write(2, "Errare humanum est.\n", 20);
	
	int fd = open("TestMPWFD.out", O_WRONLY|O_CREAT);
	write(fd, "See me roar.\n", 13);	
}

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
// % Revision 1.2  1999/06/08 04:31:32  neeri                              
// % Getting ready for 2.0b2                                               
// %                                                                       
// % Revision 1.1  1998/08/01 21:32:16  neeri                              
// % About ready for 2.0a1                                                 
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing MSL based stdio}                                       
//                                                                         
// Now we test interaction with the MSL stdio library.                     
//                                                                         
// <TestMSLTee.cp>=                                                        
#include <stdio.h>
#include <MacTypes.h>

// <Main program for [[TestMSLTee]]>=                                      
void main(int, char ** argv)
{
	char    line[1000];
	FILE *	copies[20];
	int     copycount = 0;
	
	while (*++argv)
		if (copies[copycount] = fopen(*argv, "w"))
			++copycount;
		else 
			fprintf(stderr, "Couldn't open `%s'\n", *argv);
			
	while (fgets(line, 1000, stdin)) {
		fputs(line, stdout);
		for (int i = 0; i<copycount; ++i) 
			fputs(line, copies[i]);
	}
}

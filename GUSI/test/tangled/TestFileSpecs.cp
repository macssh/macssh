// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestFileSpecs.nw	-	Test the GUSIFileSpec class                
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                            
// % Revision 1.1  1998/01/25 21:02:53  neeri                              
// % Engine implemented, except for signals & scheduling                   
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{Testing the [[GUSIFileSpec]] class}                            
//                                                                         
// This is a test program for the [[GUSIFileSpec]] class.                  
//                                                                         
//                                                                         
// <TestFileSpecs.cp>=                                                     
#define GUSI_SOURCE

#include "GUSIFileSpec.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>

// <Print a file specification>=                                           
void PrintFileSpec(ostream & s, const GUSIFileSpec & spec)
{
	s << spec->vRefNum << " " << spec->parID << " ";
	s.write(reinterpret_cast<const char *>(spec->name+1), *spec->name);
	s << endl;
}
// Currently, we only list the ":" directory.                              
//                                                                         
// <Main program for [[TestFileSpecs]]>=                                   
void main()
{
	GUSIFileSpec	dir(":");
	if (dir.Error()) {
		cout << "Error constructing: " << dir.Error() << endl;
		exit(1);
	}
	PrintFileSpec(cout, dir);
	++dir;
	if (dir.Error()) {
		cout << "Error descending: " << dir.Error() << endl;
		exit(1);
	}
	PrintFileSpec(cout, dir);
	for (short i = 1; !!dir[i]; ++i) 
		PrintFileSpec(cout, dir);
}

// <GUSIDiag.cp>=                                                          
#include "GUSIInternal.h"
#include "GUSIDiag.h"
#include "GUSIBasics.h"

#include <DCon.h>
#include <LowMem.h>

// \section{Implementation of diagnostics}                                 
//                                                                         
// Ultimately, the whole macro jungle above expands into only three rather simple
// routines. The simplest of them is [[GUSI_pos]].                         
//                                                                         
// <Implementation of diagnostics>=                                        
char * GUSI_diag_log;

bool GUSI_pos(const char * file, int line)
{
	dfprintf(GUSI_diag_log, "File '%s'; Line %d # ", file, line);
	
	return false;
}
// [[GUSI_log]] has to do arglist juggling.                                
//                                                                         
// <Implementation of diagnostics>=                                        
bool GUSI_log(const char * format, ...)
{
	va_list	ap;
		
	va_start(ap, format);
	
	char f[100];
	GUSI_sprintf(f, "%d: %s", LMGetTicks(), format);
	vdfprintf(GUSI_diag_log, f, ap);
		
	va_end(ap);

	return false;
}
// [[GUSI_break]] does a [[DebugStr]].                                     
//                                                                         
// <Implementation of diagnostics>=                                        
bool GUSI_break(const char * format, ...)
{
	va_list	ap;
	char 	breakmsgbuf[250];
	
	va_start(ap, format);
	
	strcpy(breakmsgbuf+1, "GUSI: ");
	breakmsgbuf[0] = GUSI_vsprintf(breakmsgbuf+7, format, ap)+6;
		
	va_end(ap);
	
	DebugStr((StringPtr) breakmsgbuf);
	
	return false;
}
// To offer additional flexibility, we define stub functions for the DCon functions
// we use, so even with logging enabled, we can get rid of debug messages simply
// by linking without DCon.                                                
//                                                                         
// <Implementation of diagnostics>=                                        
void dfprintf(const char *,const char *,...)
{
}

void vdfprintf(const char *,const char *,va_list)
{
}

#ifndef _DCON_
#define _DCON_

#include <MacTypes.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


void dopen(const char *log);
void dprintf(const char *format,...);
void vdprintf(const char *format,va_list arg);
void dprintmem(const void *data,size_t len);

void dfprintf(const char *log,const char *format,...);
void vdfprintf(const char *log,const char *format,va_list arg);
void dfprintmem(const char *log,const void *data,size_t len);


#ifndef DCON
	#define DCON		1
#endif

#if DCON
	#define dAssertPrint(condition)				dprintf(NULL,"Assert failed: " condition "  %s:%d\n",__FILE__,__LINE__)
	#define dAssertIfTrue(condition)			((condition) ? dAssertPrint(#condition) : ((void)0))
	#define dAssertIfFalse(condition)			((condition) ? ((void)0) : dAssertPrint(#condition))
	#define dAssert(condition)					dAssertIfFalse(condition)
#else
	#define dAssertPrint(condition)				((void)0)
	#define dAssertIfTrue(condition)			((condition) ? ((void)0) : ((void)0))
	#define dAssertIfFalse(condition)			((condition) ? ((void)0) : ((void)0))
	#define dAssert(condition)					dAssertIfFalse(condition)
	#define dopen								while(0)
	#define dprintf								while(0)
	#define vdprintf							while(0)
	#define dprintmem							while(0)
	#define dfprintf							while(0)
	#define vfdprintf							while(0)
	#define dfprintmem							while(0)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _DCON_ */

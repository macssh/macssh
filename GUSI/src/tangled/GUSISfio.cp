// <GUSISfio.cp>=                                                          
#ifdef __MWERKS__
#define __cstdio__	1
#else
#define __STDIO__	1
#endif

#include "GUSIInternal.h"
#include <sfhdr.h>
#include "GUSIBasics.h"
#include "GUSIDescriptor.h"

// \section{Implementation of internal GUSI functions for Sfio}            
//                                                                         
// [[GUSISfioFlushClose]] was adapted from the Sfio internal [[_sfall]] function.
//                                                                         
// <Implementation of internal GUSI functions for Sfio>=                   
static int GUSISfioFlushClose(bool close)
{
	reg Sfpool_t	*p, *next;
	reg Sfio_t*	f;
	reg int		n, rv;
	reg int		nsync, count, loop;
#define MAXLOOP 3

	for(loop = 0; loop < MAXLOOP; ++loop)
	{	rv = nsync = count = 0;
		for(p = &_Sfpool; p; p = next)
		{	/* find the next legitimate pool */
			for(next = p->next; next; next = next->next)
				if(next->n_sf > 0)
					break;

			/* walk the streams for _Sfpool only */
			for(n = 0; n < ((p == &_Sfpool) ? p->n_sf : 1); ++n)
			{	count += 1;
				f = p->sf[n];

				if(f->flags&SF_STRING )
					goto did_sync;
				if(SFFROZEN(f))
					continue;
				if( (close ? sfclose(f) : sfsync(f)) < 0)
					rv = -1;

			did_sync:
				nsync += 1;
			}
		}

		if(nsync == count)
			break;
	}
	return rv;
}

void GUSIStdioClose() { GUSISfioFlushClose(true);  	}
void GUSIStdioFlush() { GUSISfioFlushClose(false);	}
// Sfio already does everything to get started, no need for us to get in the act.
//                                                                         
// <Implementation of internal GUSI functions for Sfio>=                   
void GUSISetupConsoleStdio()
{
}
// We need to use the sfio function for string formatting, not the stdio version.
//                                                                         
// <Implementation of internal GUSI functions for Sfio>=                   
int GUSI_vsprintf(char * s, const char * format, va_list args)
{
	return sfvsprintf(s, 4096, format, args);
}

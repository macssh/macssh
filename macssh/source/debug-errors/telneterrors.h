/*--------------------------------- Error Classes ------------------------------------*/
#define NOCODE			0				/* no code given, just error strings */
#define FLAGMASK		1023			/* mask all of my flag crap */
#define ERRORMASK		64512			/* mask off all the error crap */
#define NET_ERRORCLASS 2048				/* this bit used for network errors */
#define MEMORY_ERRORCLASS 4096			/* memory allocation errors */
#define RESOURCE_ERRORCLASS 8192		/* problem getting resources */
/*--------------------------------- Error Levels -------------------------------------*/
#define LEVEL1	1						/* informative error */
#define LEVEL2	2						/* tell us about it */
#define LEVEL3	3						/* tell us about it, and hammer Telnet */

#include "errors.proto.h"

#ifndef _GUSITTY_
#define _GUSITTY_

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

extern pthread_key_t ssh2threadkey;

extern void GUSIWakeupTTYSocket( void *sock );

extern int InstallTTY(int id, void *context, void *sock);
extern void RemoveTTY(int id, void *context, void *sock);
extern int ReadCharsFromTTY(int id, void *context, char * buffer, int length);
extern int WriteCharsToTTY(int id, void *context, char * buffer, int length);
extern int AvailableFromTTY(int id, void *context);

#ifdef __cplusplus
}
#endif

#endif /* _GUSITTY_ */

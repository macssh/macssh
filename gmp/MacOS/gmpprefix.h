#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define exit macosexit
#define abort macosabort

#define malloc lshmalloc
#define calloc lshcalloc
#define realloc lshrealloc
#define free lshfree

#ifdef __cplusplus
extern "C" {
#endif

void *lshmalloc(unsigned long size);
void *lshcalloc(unsigned long items, unsigned long size);
void *lshrealloc(void *addr, unsigned long size);
void lshfree(void *addr);

#ifdef __cplusplus
}
#endif

#define VERSION "3.1"

/* xmalloc.h
 */

#ifndef SFTP_XMALLOC_H_INCLUDED
#define SFTP_XMALLOC_H_INCLUDED

#include <stdlib.h>

void *
xmalloc(size_t size);

char *
xstrdup(const char *s);

void *
xrealloc(void *old, size_t size);

#endif /* SFTP_XMALLOC_H_INCLUDED */

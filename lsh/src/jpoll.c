/* jpoll.c
 *
 * This code emulates a minimal poll function, and can be used as a drop-in
 * replacement for the SVID3 implementation.  Note that at this time it only
 * emulates the POLLIN and POLLOUT events.
 *
 * $id:$ */
 
/*
 * AUTHOR:     Sean Reifschneider <jafo@tummy.com>
 * DATE:       1998-10-10
 * COPYING:    Free for unlimited use.   No warranty expressed or implied.
 *					If it breaks, you get to keep both parts.
 */

#include "jpoll.h"

#include <sys/time.h>
#include <sys/types.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

int poll(struct pollfd *fdlist, nfds_t count, int timeoutInMS)
{
  struct timeval timeout, *to;
  fd_set readfdset, writefdset;
  nfds_t i;
  int ret, fdcount = 0;
  int tsize = getdtablesize();


  if (timeoutInMS < 0)
    to = NULL;
  else {
    to = &timeout;
    timeout.tv_sec = timeoutInMS / 1000;
    timeout.tv_usec = (timeoutInMS % 1000) * 1000;
  }

  FD_ZERO(&readfdset);
  FD_ZERO(&writefdset);
  for (i = 0; i < count; i++) {
    if (fdlist[i].fd < 0) continue;
    if (fdlist[i].events & POLLIN) FD_SET(fdlist[i].fd, &readfdset);
    if (fdlist[i].events & POLLOUT) FD_SET(fdlist[i].fd, &writefdset);
    fdcount++;
  }

  /*  spec says that if all FDs are negative, then *ONLY* return zero  */
  if (fdcount == 0)
    return(0);

  /*  clear all events  */
  for (i = 0; i < count; i++) fdlist[i].revents = 0;

  if ((ret = select(tsize, &readfdset, &writefdset, NULL, to)) == -1)
    return(-1);

  for (i = 0; i < count; i++) {
    if (FD_ISSET(fdlist[i].fd, &readfdset)) fdlist[i].revents |= POLLIN;
    if (FD_ISSET(fdlist[i].fd, &writefdset)) fdlist[i].revents |= POLLOUT;
    /*trace("jpoll.c: Set FD %i = %i\n", i, fdlist[i].revents);*/
  }
  /*trace("jpoll.c: Returning %i\n", ret);*/
  return(ret);
}

/* unix_random.c
 *
 * $Id$
 *
 * Randomness polling on unix, using ideas from Peter Gutmann's
 * cryptlib. */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "randomness.h"

#include "crypto.h"
#include "reaper.h"
#include "xalloc.h"
#include "werror.h"

#include <assert.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>

#ifdef HAVE_POLL
# if HAVE_POLL_H
#  include <poll.h>
# elif HAVE_SYS_POLL_H
#  include <sys/poll.h>
# endif
#else
# include "jpoll.h"
#endif

/* Workaround for some version of FreeBSD. */
#ifdef POLLRDNORM
# define MY_POLLIN (POLLIN | POLLRDNORM)
#else /* !POLLRDNORM */
# define MY_POLLIN POLLIN
#endif /* !POLLRDNORM */

#include <sys/time.h>

enum poll_status { POLL_NO_POLL, POLL_RUNNING, POLL_FINISHED, POLL_FAILED };

#include "unix_random.c.x"


#ifdef MACOS
#include <Events.h>
#include <LowMem.h>
#include <Processes.h>
#include <Timer.h>
#include <Windows.h>
#ifdef __powerc
// The timebase is available through mfspr on 601, mftb on later chips.
// Motorola recommends that an 601 implementation map mftb to mfspr
// through an exception, but I haven't tested to see if MacOS actually
// does this. We only sample the lower 32 bits of the timer (i.e. a
// few minutes of resolution)
unsigned long GetPPCTimer();
asm unsigned long GetPPCTimer()
{
	mftb  	r3		// Available on 603 and later.
}
#endif
#endif


#define HASH_OBJECT(h, x) HASH_UPDATE((h), sizeof(x), (UINT8 *) &(x))


static unsigned long
GetUSecLow()
{
#ifdef __powerc
  return GetPPCTimer();
#else
  UnsignedWide usec;
  Microseconds(&usec);
  return usec.lo;
#endif
}

#if TARGET_API_MAC_CARBON
typedef struct
{
  unsigned *count;
  struct hash_instance *hash;
} flattenTravInfos;

static pascal OSErr collectionFlattenProc(SInt32 size, void *data, void *refCon)
{
	flattenTravInfos *infos = (flattenTravInfos *)refCon;
	HASH_UPDATE(infos->hash, size, (UINT8 *)data);
     *infos->count += size / 8;
   return noErr;
}
#endif



static unsigned
do_unix_random_slow(struct random_poll *s, struct hash_instance *hash)
{
  CAST(unix_random, self, s);

  unsigned count;
  time_t now; 
  unsigned long tick;
  Point mouseLoc;
  ProcessSerialNumber psn;
  ProcessInfoRec pinfo;
  Str255 pname;
  FSSpec pfspec;
#if TARGET_API_MAC_CARBON
	WindowPtr window;
	Collection collection = NewCollection();
	CollectionFlattenUPP coltraversal = NewCollectionFlattenUPP(collectionFlattenProc);
	flattenTravInfos flattenInfos;
#else
  const WindowRecord *window;
  const VCB *vcb;
#endif

  now = time( NULL ); 
  tick = GetUSecLow();
  GetMouse( &mouseLoc );

  count = 0;

  HASH_OBJECT(hash, now);
  HASH_OBJECT(hash, tick);
  HASH_OBJECT(hash, mouseLoc);
  count += 12;

  psn.highLongOfPSN = 0;
  psn.lowLongOfPSN = kNoProcess;
  if ( GetNextProcess( &psn ) == noErr ) {
    do {
      pinfo.processInfoLength = sizeof(ProcessInfoRec);
      pinfo.processName = pname;
      pinfo.processAppSpec = &pfspec;
      if ( GetProcessInformation(&psn, &pinfo) == noErr ) {
        HASH_OBJECT(hash, pinfo);
        HASH_UPDATE(hash, *pname + 1, (UINT8 *)pname);
        HASH_OBJECT(hash, pfspec);
        count += (sizeof(pinfo) + *pname + 1 + sizeof(pfspec)) / 8;
      }
    } while ( GetNextProcess(&psn) == noErr );
  }

#if TARGET_API_MAC_CARBON
  flattenInfos.count = &count;
  flattenInfos.hash = hash;
  window = GetWindowList();
  while ( window && collection && coltraversal ) {
  	EmptyCollection(collection);
  	StoreWindowIntoCollection(window, collection);
  	FlattenCollection(collection, coltraversal, &flattenInfos);
    window = GetNextWindow(window);
  }
  
  if(collection)
  	DisposeCollection(collection);
  if(coltraversal)
  	DisposeCollectionFlattenUPP(coltraversal);
#else
  window = (const WindowRecord *)LMGetWindowList();
  while ( window ) {
    HASH_OBJECT(hash, *window);
    count += sizeof(WindowRecord) / 8;
    window = window->nextWindow;
  }

  vcb = (const VCB *) GetVCBQHdr()->qHead;
  while ( vcb ) {
    HASH_OBJECT(hash, *vcb);
    count += sizeof(VCB) / 8;
    vcb = (const VCB *)vcb->qLink;
  }
#endif

  return count;
}

static unsigned
do_unix_random_fast(struct random_poll *s, struct hash_instance *hash)
{
  CAST(unix_random, self, s);
  unsigned count = 0;

#if HAVE_GETRUSAGE
  {
    struct rusage rusage;
    if (getrusage(RUSAGE_SELF, &rusage) < 0)
      fatal("do_unix_random_fast: getrusage() failed: (errno = %i) %z\n",
	    errno, STRERROR(errno));
    
    HASH_OBJECT(hash, rusage);
    count += 1;
  }
#endif /* HAVE_GETRUSAGE */
#if HAVE_GETTIMEOFDAY
  {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
      fatal("do_unix_random_fast: gettimeofday failed(errno = %i) %z\n",
	    errno, STRERROR(errno));

    HASH_OBJECT(hash, tv);
  }
#endif /* HAVE_GETTIMEOFDAY */

  {
    /* Fallback that is useful if nothing else works. Count the number
     * of slow polls between time() ticks, and count one bit of
     * entropy if we have more than 2 calls or more than two seconds
     * between calls. */
    
    time_t now = time(NULL);
    unsigned long tick = GetUSecLow();
    self->time_count++;
    if (now != self->previous_time)
      {
	if ( (self->time_count > 2) || ((now - self->previous_time) > 2))
	  count++;
	
	HASH_OBJECT(hash, now);
	HASH_OBJECT(hash, tick);
	HASH_OBJECT(hash, self->time_count);
	
	self->time_count = 0;
	self->previous_time = now;
      }
  }

  return count;
}

static void do_unix_random_background(struct random_poll *s)
{
  CAST(unix_random, self, s);

}

/* Using a NULL reaper argument is ok. It must be supplied only if the
 * application is using a reaper, as that may get to our child process
 * before we can waitpid it. */

struct random_poll *
make_unix_random(struct reap *reaper)
{
  NEW(unix_random, self);

  self->super.slow = do_unix_random_slow;
  self->super.fast = do_unix_random_fast;
  self->super.background = do_unix_random_background;
  
  self->reaper = reaper;

  self->status = POLL_NO_POLL;
  self->previous_time = time(NULL);
  self->time_count = 0;

  return &self->super;
}

// tekstor.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/* This has been rewritten to use one damn handle to hold all of the data. -- JMB */

#include "vgtek.proto.h"
#include "rg0.proto.h"
#include "rgmp.proto.h"
#include "tekrgmac.proto.h"
#include "tekdefs.h"	/* NCSA: sb - all defines are now here, for easy access */
#include "tekstor.proto.h"

//	thiselnum runs from 0 to (handlesize-1), which is in accordance with C style
//	arrays.  The handle is indexed 0...(handlesize-1) as well.  This is why you see
//	the various +-1's. -- JMB 8/93

TEKSTOREP	newTEKstore(void)
{
	TEKSTOREP	s;
	
	s = (TEKSTOREP) myNewPtr(sizeof(TEKSTORE));
	if (s == NULL) return (NULL);
	
	if (!(s->dataHandle = myNewHandle(0L)))
		return (NULL);
	
	s->thiselnum = 0;
	
	return(s);
}

void freeTEKstore(TEKSTOREP s)
{
	DisposeHandle(s->dataHandle);
	DisposePtr((Ptr) s);
}

short	addTEKstore(TEKSTOREP s, char d)
{
	Size	handlesize;
	OSErr	memErr;
	handlesize = GetHandleSize(s->dataHandle);
	memErr = mySetHandleSize(s->dataHandle, ++handlesize);
	if (memErr) return (-1);
	
	(*(s->dataHandle))[handlesize-1] = d;	// Save in last position
	
	return(0);
}

void	topTEKstore(TEKSTOREP s)
{
	s->thiselnum = 0;
}

short	nextTEKitem(TEKSTOREP s)
{
	if (s->thiselnum == (GetHandleSize(s->dataHandle))) return(-1);	// At end of data
	
	return((short) ((*(s->dataHandle))[(s->thiselnum)++]) );
}

short	TEKunstore(TEKSTOREP s)
{
	Size	handlesize;
	OSErr 	memErr;
	if ((handlesize = GetHandleSize(s->dataHandle)) == 0) return (-1);	// Nothing in store
		
	memErr = mySetHandleSize(s->dataHandle, --handlesize);
	if (memErr) return(-1);
	
	if (s->thiselnum >= handlesize) s->thiselnum = handlesize - 1;
	return(0);
}

#ifdef	AARON_CONTORER_HAS_A_BRAIN		// Never true
#define MINPOOL 0x0200	/* smallest allowable pool 	*/
#define MAXPOOL 0x2000	/* largest allowable pool 	*/

STOREP newstore(void)
/* 
	create a new, empty store and return a pointer to it.
	Returns NULL if not enough memory to create a new store.
*/
{
	STOREP s;
	
	s=(STOREP) myNewPtr((long) sizeof(STORE));			/* BYU LSC */
	if (s==NULL) {
		return(NULL);
	}
	else {
		s->lasth = s->thish = s->firsth =
			(HANDLEP) myNewPtr((long) sizeof(HANDLE));	/* BYU LSC */
		if (s->firsth==NULL) {
			DisposePtr((Ptr) s);
			return(NULL);
		}
		else {
			s->firsth->pool = myNewPtr((long) MINPOOL);	/* BYU LSC */
			if (s->firsth->pool==NULL) {
				DisposePtr((Ptr) s->firsth);
				DisposePtr((Ptr) s);
				return(NULL);
			}
			else {
				s->lastelnum = s->thiselnum = -1;
				s->firsth->poolsize = MINPOOL;
				s->firsth->next = NULL;
			}
		}
	}
	return(s);
}

void freestore(STOREP s)
/*
	Frees all pools and other memory space associated with store s.
*/
{
	HANDLEP h,h2;
	h = s->firsth;
	while (h != NULL) {
		h2 = h;
		DisposePtr(h->pool);
		h = h->next;
		DisposePtr((Ptr) h2);
	}
	DisposePtr((Ptr) s);
}


int addstore(STOREP s, char d)
/*
	Adds character d to the end of store s.
	Returns 0 if successful, -1 if unable to add character (no memory).
*/
{
	int n; /* temp storage */
	long size;					/* BYU LSC */
	HANDLEP h;

	n = ++(s->lastelnum);
	size = s->lasth->poolsize;
	if (n < s->lasth->poolsize) {
		s->lasth->pool[n] = d;
	}
	else {
		/* Pool full; allocate a new one. */
		if (size<MAXPOOL) size <<= 1;
		h = (HANDLEP)myNewPtr((long) sizeof(HANDLE));		/* BYU LSC */
		if (h==NULL) {
			(s->lastelnum)--;
			return(-1);
		}
		else {
			h->pool = myNewPtr(size);
			if (h->pool==NULL) {
				DisposePtr((Ptr) h);
				(s->lastelnum)--;
				return(-1);
			}
			else {
				h->poolsize = size;
				h->next = NULL;
				s->lasth->next = h;
				s->lasth = h;
				s->lastelnum = 0;
				h->pool[0] = d;
			}
		}
		} /* end of new pool allocation */
	return(0);
} /* end addstore() */

topstore(STOREP s)
/*
	Reset stats so that a call to nextitem(s) will be retrieving the
	first item in store s.
*/
{
	s->thish = s->firsth;
	s->thiselnum = -1;
}

int nextitem(STOREP s)
/*
	Increment the current location in store s.  Then return the
	character at that location.  Returns -1 if no more characters.
*/
{
	HANDLEP h;

	if (s->thish==s->lasth && s->thiselnum==s->lastelnum) return(-1);
	else {
		h = s->thish;
		if (++(s->thiselnum) < s->thish->poolsize) {
			return((int)(s->thish->pool[s->thiselnum]));
			}
		else {
			/* move to next pool */
			h = h->next;
			s->thish = h;
			s->thiselnum = 0;
			return((int)(h->pool[0]));
			}
		}
} /* end nextitem() */

int unstore(STOREP s)
/*
	Removes ("pops") the last item from the specified store.
	Returns that item (in range 0-255), or returns -1 if there
	are no items in the store.
*/
{
	HANDLEP nextolast;

	if (s->lastelnum > -1) { /* last pool not empty */
		return((int)(s->lasth->pool[(s->lastelnum)--]));
	} else { /* last pool empty */
		if (s->lasth == s->firsth) return(-1);

		else { /* move back one pool */
			nextolast = s->firsth;
			while (nextolast->next != s->lasth)
				nextolast = nextolast->next;
			DisposePtr((Ptr) nextolast->next);
			s->lasth = nextolast;
			s->lastelnum = nextolast->poolsize - 2;

			if (s->thish == nextolast->next) {
				s->thish = nextolast;
				s->thiselnum = s->lastelnum;
			}

			nextolast->next = NULL;
			return((int)(nextolast->pool[s->lastelnum+1]));
		}
	}
}
#endif

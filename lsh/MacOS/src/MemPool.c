/*
 * MemPool.c
 * (c) 2000 Jean-Pierre Stierlin.
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

#define XTERN_POOL		1
#define GROW_POOL		1
#define USENEEDED		1
#define GETSTATS		1
#define MASK_INT		0
#define DEBUG_MEM		1

/*--------------- CIncludes --------------------------------------------------*/

#ifndef __MEMORY__
#include <Memory.h>
#endif
#ifndef __ERRORS__
#include <Errors.h>
#endif
#include <Assert.h>

/*--------------- Includes ---------------------------------------------------*/

#ifndef	__MEMPOOL__
#include "MemPool.h"
#endif

/*--------------- Local Declarations -----------------------------------------*/

#if DEBUG_MEM
#define DEBUGSTRING(x)	DebugStr("\p"x)
#else
#define DEBUGSTRING(x)
#endif

#if MASK_INT

#pragma parameter __D0 DisInt
unsigned short DisInt(void) = {0x40c0,0x007C,0x0700};	// move.w sr,d0       ori.w #$700,sr

#pragma parameter EnInt(__D0)
void EnInt(unsigned short) = {0x46C0};					// move.w d0,sr

#else

#define DisInt()	0
#define EnInt(x)

#endif

struct mem {
	unsigned long	size;
#if USENEEDED
	unsigned long	neededSize;
#endif
	struct mem		*nextBlock;
};

#define	MEMHSIZE 	(sizeof(struct mem))

#if USENEEDED
#define SETUSED(mp, wanted)     mp->neededSize = wanted
#define GETUSED(mp)             mp->neededSize
#else
#define SETUSED(mp, wanted)
#define GETUSED(mp)
#endif

#if GETSTATS
#define ADDSIZE(memPool, mp)    memPool->f_CurrSize += mp->size; \
                                if (memPool->f_PeakSize < memPool->f_CurrSize) \
                                    memPool->f_PeakSize = memPool->f_CurrSize
#define REMOVESIZE(memPool, mp) memPool->f_CurrSize -= mp->size
#else
#define ADDSIZE(memPool, mp)
#define REMOVESIZE(memPool, mp)
#endif

#define BADBLOCK(memPool, mp) \
	((mp) < memPool->f_LoMem || (mp) > memPool->f_HiMem - MEMHSIZE \
	 || !(mp)->size || (mp)->size > memPool->f_PoolSize)


//------------------------------------------------------------------------------

OSErr MPInit(long allocsize, Ptr *priv, Ptr prev)
{
#if XTERN_POOL
	OSErr			err;
#endif
	Handle			privH;
	MemPool			*memPool;
	Handle			poolH;

	privH = NewHandle(sizeof(MemPool));
	if ( privH == nil ) {
		//return MemError();
		return memFullErr;
	}
	MoveHHi(privH);
	HLock(privH);

	memPool = (MemPool *)*privH;

#if XTERN_POOL
	poolH = TempNewHandle((Size)allocsize + MEMHSIZE, &err);
#else
	poolH = NewHandle((Size)allocsize + MEMHSIZE);
#endif
	if ( poolH == nil ) {
		DisposeHandle(privH);
		//return MemError();
		return memFullErr;
	}
#if !XTERN_POOL
	MoveHHi(poolH);
#endif
	HLock(poolH);

	memPool->f_LoMem = *poolH;
	memPool->f_Top = memPool->f_LoMem;
	memPool->f_Free = (char *)0L;
	memPool->f_HiMem = memPool->f_LoMem + allocsize + MEMHSIZE;
	memPool->f_PoolSize = allocsize;
	memPool->f_HandleRec = privH;
	memPool->f_HandleMem = poolH;
	memPool->next = nil;
	memPool->prev = (MemPool *)prev;
#if GETSTATS
	memPool->f_CurrSize = 0L;
	memPool->f_PeakSize = 0L;
#endif
	*priv = (Ptr)memPool;

	return noErr;
}

//------------------------------------------------------------------------------

void MPDispose(Ptr priv)
{
#if XTERN_POOL
	OSErr			err;
#endif
	MemPool			*memPool = (MemPool *)priv;

#if DEBUG_MEM
	assert(memPool == (MemPool *)*memPool->f_HandleRec);
	assert(memPool->f_LoMem == *memPool->f_HandleMem);
#endif
#if GROW_POOL
	if (memPool->next != nil) {
		MPDispose(memPool->next);
		memPool->next = nil;
	}
#endif
	if (memPool->f_LoMem != nil) {
#if XTERN_POOL
		TempDisposeHandle(memPool->f_HandleMem, &err);
#else
		DisposeHandle(memPool->f_HandleMem);
#endif
		memPool->f_LoMem = nil;
		DisposeHandle(memPool->f_HandleRec);
	}
}

//------------------------------------------------------------------------------
// to always clear the buffers upon allocation, replace this bzero by the real one

#define bzero(pointer, size)	pointer

//------------------------------------------------------------------------------

void *MPmalloc(Ptr priv, long size)
{
	MemPool			*memPool = (MemPool *)priv;
    register struct mem	*mp, *tmp;
	long			wanted;
	unsigned short	oldLevel;

#if DEBUG_MEM
	assert(memPool == (MemPool *)*memPool->f_HandleRec);
	assert(memPool->f_LoMem == *memPool->f_HandleMem);
#endif
    if ((wanted = size) == 0)
		return (char *)0;
	else {
		// align to four bytes
		if (size % 4)
			size += 4 - (size % 4);
		size += MEMHSIZE;
	}
	oldLevel = DisInt();
	// search into the free blocks list if size needed is found
    for (tmp = 0, mp = (struct mem *)memPool->f_Free; mp; tmp = mp, mp = mp->nextBlock) {
		assert(!BADBLOCK(memPool, mp));
        if (mp && mp->size >= size) {
			// this block is at least big enough for the size needed
            if (mp->size >= size + MEMHSIZE + 4) {	// free the begining
				// this block is much bigger, let's create a new free block
                tmp = (struct mem *)((unsigned)mp + mp->size - size);
                tmp->size = size;
                mp->size -= size;
				SETUSED(tmp, wanted);
				ADDSIZE(memPool, tmp);
				EnInt(oldLevel);
                return bzero((char *)tmp + MEMHSIZE, wanted);
            }
            if (tmp) {
                tmp->nextBlock = mp->nextBlock;
			}
            else
                memPool->f_Free = (char *)mp->nextBlock;
			SETUSED(mp, wanted);
			ADDSIZE(memPool, mp);
			EnInt(oldLevel);
            return bzero((char *)mp + MEMHSIZE, wanted);
        }
    }
    if (memPool->f_Top + size > memPool->f_HiMem) {
#if GROW_POOL
		if (memPool->next == nil) {
			OSErr err;
			long poolsize = size > memPool->f_PoolSize ? size : memPool->f_PoolSize;
			err = MPInit(poolsize, &memPool->next, (Ptr)memPool);
			if (err == noErr) {
				EnInt(oldLevel);
				return MPmalloc((Ptr)memPool->next, wanted);
			}
		} else {
			EnInt(oldLevel);
			return MPmalloc((Ptr)memPool->next, wanted);
		}
#endif
		EnInt(oldLevel);
        return nil;
	}
    mp = (struct mem *)memPool->f_Top;
    memPool->f_Top += size;
    mp->size = size;
	SETUSED(mp, wanted);
	ADDSIZE(memPool, mp);
	EnInt(oldLevel);
    return bzero((char *)mp + MEMHSIZE, wanted);
}

//------------------------------------------------------------------------------

void MPfree(Ptr priv, void *addr)
{
	MemPool			*memPool = (MemPool *)priv;
	register struct mem *memAddr;
    register struct mem *mp, *tmp;
	unsigned short	oldLevel;

#if DEBUG_MEM
	assert(memPool == (MemPool *)*memPool->f_HandleRec);
	assert(memPool->f_LoMem == *memPool->f_HandleMem);
#endif
	// check the addr
	if (addr < memPool->f_LoMem + MEMHSIZE || addr >= memPool->f_HiMem) {
#if GROW_POOL
		if (memPool->next != nil) {
			MPfree(memPool->next, addr);
		} else {
#endif
			DEBUGSTRING("### MPfree : outbounds");
#if GROW_POOL
		}
#endif
		return;
	}
    memAddr = (struct mem *)((char *)addr - MEMHSIZE);
	if (!memAddr->size || memAddr->size > memPool->f_PoolSize) {
		DEBUGSTRING("### MPfree : bad size");
		return;
	}

#if USENEEDED
	if (!GETUSED(memAddr) || GETUSED(memAddr) > memPool->f_PoolSize) {
		DEBUGSTRING("### MPfree : bad user size");
		return;
	}
	SETUSED(memAddr, 0);
#endif

	REMOVESIZE(memPool, memAddr);

	oldLevel = DisInt();

	// insert the block in the free list
    if (memPool->f_Free == nil) {
        memPool->f_Free = (char *)memAddr;
        memAddr->nextBlock = nil;
    } else {
		// get previous free block
	    for (tmp = nil, mp = (struct mem *)memPool->f_Free; mp; tmp = mp, mp = mp->nextBlock) {
			assert(!BADBLOCK(memPool, mp));
			// this block is between memPool->f_LoMem and previous free block
	        if (tmp < memAddr && memAddr < mp)
	            break;
		}

		// hook mp block to the just freed "memAddr" one
	    memAddr->nextBlock = mp;

		// if we had previous blocks
	    if (tmp) {
	        tmp->nextBlock = memAddr;
		} else {
	        memPool->f_Free = (char *)memAddr;
		}
	}
	// now compact the free blocks list
	// try assembling adjacent free blocks
    for (tmp = (struct mem *)memPool->f_Free,
		mp = ((struct mem *)memPool->f_Free)->nextBlock; mp; tmp=mp, mp=mp->nextBlock) {
		assert(!BADBLOCK(memPool, mp));
        if ((char *)tmp + tmp->size == (char *)mp) {
            tmp->size += mp->size;
            tmp->nextBlock = mp->nextBlock;
            mp = tmp;
		}
	}
	// now try to free blocks in the list
	// tmp points to the last free block
    if (memPool->f_Top == (char *)tmp + tmp->size) {
		// memPool->f_Top is just after the last free block
		// remove the last free block off the list and ajust memPool->f_Top
        memPool->f_Top = (char *)tmp;
        if (memPool->f_Free == (char *)tmp) {
			// last free block was the first of the list : the list is empty
            memPool->f_Free = nil;
        } else {
			// remove the last block off the list
            for (mp = (struct mem *)memPool->f_Free; mp->nextBlock != tmp; mp = mp->nextBlock)
                ;
            mp->nextBlock = nil;
        }
    }
#if GROW_POOL
	/* don't kill first mempool: it's the only one known by the caller... */
	if ( memPool->prev && memPool->f_Free == nil && memPool->f_Top == memPool->f_LoMem ) {
		if ( memPool->prev )
			memPool->prev->next = memPool->next;
		if ( memPool->next )
			memPool->next->prev = memPool->prev;
		memPool->next = nil;
		MPDispose(priv);
	}
#endif
	EnInt(oldLevel);
}

//------------------------------------------------------------------------------
#if USENEEDED
long MPsize(Ptr priv, void *addr)
{
	MemPool			*memPool = (MemPool *)priv;
	register struct mem *memAddr;

	// check the addr
	if (addr < memPool->f_LoMem + MEMHSIZE || addr >= memPool->f_HiMem) {
#if GROW_POOL
		if (memPool->next != nil) {
			return MPsize((Ptr)memPool->next, addr);
		} else {
#endif
			DEBUGSTRING("### MPsize : outbounds");
#if GROW_POOL
		}
#endif
		return NULL;
	}
    memAddr = (struct mem *)((char *)addr - MEMHSIZE);
	if (!memAddr->size || memAddr->size > memPool->f_PoolSize) {
		DEBUGSTRING("### MPsize : bad size");
		return NULL;
	}
	if (!GETUSED(memAddr) || GETUSED(memAddr) > memPool->f_PoolSize) {
		DEBUGSTRING("### MPsize : bad user size");
		return NULL;
	}
	return GETUSED(memAddr);
}
#endif

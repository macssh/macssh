// memory.c 

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "memory.proto.h"

static Size gCushion;				/* size of memory cushion */
static Size gReserve;				/* size of memory reserve */
static Handle gReserveBlock = NULL;	/* reserve block */
static Boolean gCritical = true;	/* true if current memory request is critical */

void initMemoryBuffer(Size cushion, Size reserve)
{
	OSErr err = noErr;
	static GrowZoneUPP myGrowZoneUPP;

	gCushion = cushion;
	gReserve = reserve;
	gReserveBlock = NewHandle(reserve);
	myGrowZoneUPP = NewGrowZoneProc(MyGrowZone);
	SetGrowZone(myGrowZoneUPP);
	return;
}

Boolean RecoverReserveMemory (void)
{
	if (gReserveBlock == nil) return true;
	if (*gReserveBlock != nil) return true;
	ReallocateHandle(gReserveBlock, gReserve);
	if (MemError() != noErr) return false;
	return MemoryAvailable(0);
}

Boolean MemoryAvailable (Size len)
{
	long total, contig;
	
	if (gReserveBlock == nil || *gReserveBlock == nil) {
		if (!RecoverReserveMemory()) return false;
	}
	PurgeSpace(&total, &contig);
	return len + gCushion < contig;
}

static pascal long MyGrowZone (Size cbNeeded)
{
	long theA5, result;
	
	theA5 = SetCurrentA5();
	if (gCritical && gReserveBlock != nil && 
		*gReserveBlock != nil && gReserveBlock != GZSaveHnd()) 
	{
		EmptyHandle(gReserveBlock);
		result = gReserve;
	} else {
		result = 0;
	}
	SetA5(theA5);
	return result;
}


Handle myNewHandle(Size len)
{
	Handle tempHandle;
	if (!MemoryAvailable(len)) return NULL;
	gCritical = FALSE;
	tempHandle = NewHandleClear(len);
	gCritical = TRUE;
	return tempHandle;
}

Handle myNewHandleCritical(Size len)
{
	Handle tempHandle;
	tempHandle = NewHandleClear(len);
	return tempHandle;
}


Ptr myNewPtr(Size len)
{
	Ptr tempPtr;
	if (!MemoryAvailable(len)) return NULL;
	gCritical = FALSE;
	tempPtr = NewPtrClear(len);
	gCritical = TRUE;
	return tempPtr;
}

Ptr myNewPtrCritical(Size len)
{
	Ptr tempPtr;
	tempPtr = NewPtrClear(len);
	return tempPtr;
}

OSErr mySetHandleSize (Handle handle, Size len)
{
	Size oldLen;
	OSErr err = noErr;

	oldLen = GetHandleSize(handle);
	if (oldLen < len) {
		if (!MemoryAvailable(len-oldLen)) return memFullErr;
	}
	gCritical = FALSE;
	SetHandleSize(handle, len);
	gCritical = TRUE;
	err = MemError();
	if (err == memFullErr) {
		/* The Memory Manager is too stupid to do this itself. */
		MoveHHi(handle);
		CompactMem(maxSize);
		gCritical = FALSE;
		SetHandleSize(handle, len);
		gCritical = TRUE;
		err = MemError();
	}
	if (err != noErr) return err;
	if (oldLen >= len) return noErr;
	if (MemoryAvailable(0)) return noErr;
	SetHandleSize(handle, oldLen);
	return memFullErr;
}

OSErr MySetHandleSizeCritical (Handle handle, Size len)
{
	SetHandleSize(handle, len);
	if (MemError() == memFullErr) {
		/* The Memory Manager is too stupid to do this itself. */
		MoveHHi(handle);
		CompactMem(maxSize);
		SetHandleSize(handle, len);
	}
	return MemError();
}

// <GUSISpecific.cp>=                                                      
#include "GUSIInternal.h"
#include "GUSISpecific.h"
#include "GUSIContext.h"
#include "GUSIDiag.h"

#include <utility>

GUSI_USING_STD_NAMESPACE

#include <TextUtils.h>

// \section{Implementation of Thread Specific Variables}                   
//                                                                         
// [[GUSISpecific]] is trivial to implement, so we keep all of the members inline.
//                                                                         
// <Member functions for class [[GUSISpecific]]>=                          
unsigned GUSISpecific::sNextID = 0;
// <Member functions for class [[GUSISpecificTable]]>=                     
GUSISpecificTable::~GUSISpecificTable()
{
	void * data;
	
	if (fValues) {
		for (unsigned id = 0; id < fAlloc; ++id) 
			while ((data = fValues[0][id]) && sKeys[0][id]) {
				fValues[0][id] = 0;
				sKeys[0][id]->Destruct(data);
			}
		DisposeHandle((Handle)fValues);
	}
}
// <Member functions for class [[GUSISpecificTable]]>=                     
void GUSISpecificTable::SetSpecific(const GUSISpecific * key, void * value)
{
	if (!key)
		return;
	
	if (key->fID >= fAlloc) {
		unsigned newAlloc 	= (key->fID & ~7) + 8;
		size_t   allocSize	= newAlloc*sizeof(void *);
	
		if (!fValues)
			fValues  = (void ***)NewHandle(allocSize);
		else
			SetHandleSize((Handle) fValues, allocSize);			

		while (fAlloc < newAlloc)
			fValues[0][fAlloc++] = nil;
	}
	fValues[0][key->fID] = value;
}
// <Member functions for class [[GUSISpecificTable]]>=                     
void GUSISpecificTable::DeleteSpecific(const GUSISpecific * key)
{
	if (fValues && Valid(key)) {
		void * data = fValues[0][key->fID];
		
		if (data && sKeys[0][key->fID]) {
			fValues[0][key->fID] = nil;
			sKeys[0][key->fID]->Destruct(data);
		}
	}
}
// <Member functions for class [[GUSISpecificTable]]>=                     
GUSISpecific ***	GUSISpecificTable::sKeys 		= nil;
unsigned			GUSISpecificTable::sKeyAlloc 	= 0;
// <Member functions for class [[GUSISpecificTable]]>=                     
void GUSISpecificTable::Register(GUSISpecific * key)
{
	if (key->fID >= sKeyAlloc) {
		unsigned newAlloc 	= (key->fID & ~7) + 8;
		size_t   allocSize	= newAlloc*sizeof(GUSISpecific *);
		
		if (!sKeys)
			sKeys = (GUSISpecific ***) NewHandle(allocSize);
		else
			SetHandleSize((Handle)sKeys, allocSize);
			
		while (sKeyAlloc < newAlloc)
			sKeys[0][sKeyAlloc++] = nil;
	}
	sKeys[0][key->fID] = key;
}
// Note that this doesn't call any destructors.                            
//                                                                         
// <Member functions for class [[GUSISpecificTable]]>=                     
void GUSISpecificTable::Destruct(GUSISpecific * key)
{
	sKeys[0][key->fID] = nil;
}

// <GUSIContextQueue.cp>=                                                  
#include "GUSIInternal.h"

#define GUSI_DIAG GUSI_DIAG_CAUTIOUS
#include "GUSIDiag.h"

#include "GUSIContextQueue.h"
#include "GUSIContext.h"

#include <string.h>

#include <Memory.h>

// <Implementation of context queues>=                                     
GUSIContextQueue::element::header * GUSIContextQueue::element::sBlocks = 0;
// [[operator new]] must be prepared to allocate a new block.              
//                                                                         
// <Implementation of context queues>=                                     
void * GUSIContextQueue::element::operator new(size_t)
{
	header * b;
	
	for (b = sBlocks; b; b = b->fNext)
		if (b->fFree) 
			break;
	
	if (!b) {
		short max = sBlocks ? (sBlocks->fMax << 1) : 64;
		b = reinterpret_cast<header *>(NewPtr(max << 3));
		if (!b)
			return 0;
		memset(b, 0, max << 3);
		b->fMax = max;
		b->fFree= max-1; 
		b->fNext= sBlocks;
		sBlocks = b;
	}
	
	element * e = reinterpret_cast<element *>(b);
	while ((++e)->fContext)
		;
	--b->fFree;
			
	return e;
}
// To avoid borderline cases, [[operator delete]] only deletes a block if there
// are enough other free elements.                                         
//                                                                         
// <Implementation of context queues>=                                     
void GUSIContextQueue::element::operator delete(void * elem, size_t)
{
	header * h = static_cast<header *>(elem);
	header * b;
	header * p = 0;
	
	for (b = sBlocks; b; b = b->fNext)
		if (h > b && h < b+b->fMax) {
			memset(h, 0, sizeof(header));
			if (++b->fFree == b->fMax-1) {
				// <Delete [[b]] if enough free elements remain>=                          
    int sum = 0;
    for (header * s = sBlocks; s; s = s->fNext)
    	if (s != b)
    		if ((sum += s->fFree) > 32) {
    			if (p)
    				p->fNext = b->fNext;
    			else
    				sBlocks = b->fNext;
    			DisposePtr(reinterpret_cast<Ptr>(b));
    			
    			break;
    		}
			}
			return;
		}			
	// Can't reach
}

// <Implementation of context queues>=                                     
GUSIContextQueue::~GUSIContextQueue()
{
	while (!empty())
		pop_front();
}
// Making [[remove]] inline as well would probably push things too far, though.
//                                                                         
// <Implementation of context queues>=                                     
void GUSIContextQueue::remove(GUSIContext * context)
{
	if (fFirst) 
		if (fFirst->fContext == context)
			pop_front();
		else {
			element * prev = fFirst;
			for (element * cur = prev->fNext; cur; cur = cur->fNext) 
				if (cur->fContext == context) {
					if (!(prev->fNext = cur->fNext)) // Delete last element
						fLast = prev;
					delete cur;
					
					return;
				} else 
					prev = cur;
		}
}
// I'd love to have [[push_back]] inline, but it doesn't seem to work.     
//                                                                         
// <Implementation of context queues>=                                     
void GUSIContextQueue::push_back(GUSIContext * context)
{
	if (element * e = new element(context)) {
		if (fLast)
			fLast->fNext = e;
		else 
			fFirst = e;
		fLast = e;
	}
}
// [[Wakeup]] is the only context specific operation.                      
//                                                                         
// <Implementation of context queues>=                                     
void GUSIContextQueue::Wakeup()
{
	GUSI_MESSAGE(("Wakeup #%h\n", this));
	for (element * cur = fFirst; cur; cur = cur->fNext) 
		cur->fContext->Wakeup();
}

// <GUSIBuffer.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSIBuffer.h"

#include <algorithm>
#include <stdlib.h>
#include <Memory.h>

GUSI_USING_STD_NAMESPACE

// The constructor of [[GUSIScattGath]] does a case distinction between an empty
// IO vector, a vector with only one element, and a vector with several elements.
//                                                                         
// <Member functions for class [[GUSIScattGath]]>=                         
GUSIScattGath::GUSIScattGath(const iovec *iov, int cnt, bool gather)
	:	fIo(iov), fCount(cnt), fScratch(nil), fGather(gather)
{
	if (fCount < 1)	{
		// The first two cases are obvious.                                        
  //                                                                         
  // <[[GUSIScattGath]] constructed with empty IO vector>=                   
  fBuf	=	nil;
  fLen	=	0;
	} else if (fCount == 1)	{	
		// <[[GUSIScattGath]] constructed with contiguous IO vector>=              
  fBuf	=	(void *) iov->iov_base;
  fLen	=	(int)	iov->iov_len;
	} else {
		// Only the third case is of any interest. We construct a scratch area. Note that
  // [[cnt]] and [[iov]] are used; we don't want to alter the member fields. Since 
  // [[GUSIScattGath]] usually persist only for short times, [[fScratch]] is allocated 
  // as a locked handle rather than a pointer.                               
  //                                                                         
  // <[[GUSIScattGath]] constructed with sparse IO vector>=                  
  fBuf     = nil;
  for (fLen = 0; cnt--; ++iov)
  	fLen += (int) iov->iov_len;
	}
}
// The backwards compatible constructor of [[GUSIScattGath]] uses the [[fTrivialIo]]
// field to simulate the [[iovec]] array.                                  
//                                                                         
// <Member functions for class [[GUSIScattGath]]>=                         
GUSIScattGath::GUSIScattGath(void * buffer, size_t length, bool gather)
	:	fIo(&fTrivialIo), fCount(1), fScratch(nil), fGather(gather)
{
	fTrivialIo.iov_base = static_cast<caddr_t>(fBuf = buffer);
	fTrivialIo.iov_len	= fLen = length;
}
// A call to [[Buffer]] is an indication that the caller prefers for the data to
// be contiguous, so we allocate a scratch buffer and gather, if necessary.
//                                                                         
// <Member functions for class [[GUSIScattGath]]>=                         
void * GUSIScattGath::Buffer() const
{
	if (!fScratch && fCount > 1) {
		if (GUSI_MUTABLE(GUSIScattGath, fScratch) = NewHandle(fLen))	{
			HLock(fScratch);
			GUSI_MUTABLE(GUSIScattGath, fBuf)	=	(void *) *fScratch;
			if (fGather) {
				// <Gather [[fIo]] contents in [[fBuf]]>=                                  
    char *			buffer = static_cast<char *>(fBuf);
    const iovec *	io 	   = fIo;
    for (int count = fCount; count--; ++io)	{
    	memcpy(buffer, io->iov_base, io->iov_len);
    	
    	buffer	+=	io->iov_len;
    }
			}
		} else {
			GUSI_MUTABLE(GUSIScattGath, fCount)	= 	0;
			GUSI_MUTABLE(GUSIScattGath, fBuf) 	=	nil;
		}
	}
	
	return fBuf;
}
// The destructor of [[GUSIScattGath]] gets rid of the scratch area if one was 
// allocated. If it works in scatter mode, the data is first scattered.    
//                                                                         
// <Member functions for class [[GUSIScattGath]]>=                         
GUSIScattGath::~GUSIScattGath()
{
	if (fScratch) {
		if (!fGather) {
			// <Scatter [[fBuf]] contents to [[fIo]]>=                                 
   char *			buffer = static_cast<char *>(fBuf);
   const iovec *	io 	   = fIo;
   int				length = fLen;
   for (int count = fCount; count-- && length; ++io)	{
   	int sect = min(length, int(io->iov_len));
   	
   	memcpy(io->iov_base, buffer, sect);
   	
   	buffer	+=	sect;
   	length 	-=	sect;
   }
		}
		DisposeHandle(fScratch);
	}
}
// The copy constructor and the assignment operator both have to ensure that the
// two objects don't end up sharing the same scratch area.                 
//                                                                         
// <Member functions for class [[GUSIScattGath]]>=                         
void GUSIScattGath::operator=(const GUSIScattGath & other)
{
	if (fScratch)
		DisposeHandle(fScratch);
	// <Copy uncontroversial fields of [[GUSIScattGath]]>=                     
 fIo 	= other.fIo;
 fCount	= other.fCount;
 fLen	= other.fLen;
 fGather = other.fGather;
	// <Copy [[fBuf]] unless it points to a scratch area>=                     
 fScratch = nil;
 fBuf 	 = other.fScratch ? nil : other.fBuf;
}

GUSIScattGath::GUSIScattGath(const GUSIScattGath & other)
{
	// <Copy uncontroversial fields of [[GUSIScattGath]]>=                     
 fIo 	= other.fIo;
 fCount	= other.fCount;
 fLen	= other.fLen;
 fGather = other.fGather;
	// <Copy [[fBuf]] unless it points to a scratch area>=                     
 fScratch = nil;
 fBuf 	 = other.fScratch ? nil : other.fBuf;
}
// <Member functions for class [[GUSIRingBuffer]]>=                        
size_t GUSIRingBuffer::Distance(Ptr from, Ptr to)
{
	if (from > to)
		return (fEnd - from) + (to - fBuffer);
	else
		return to-from;	
}
// <Member functions for class [[GUSIRingBuffer]]>=                        
bool GUSIRingBuffer::Invariant()
{
	Lock();
		
	bool invariant = 
		GUSI_CASSERT_INTERNAL(fProduce >= fBuffer && fProduce < fEnd)
	 &&	GUSI_CASSERT_INTERNAL(fConsume >= fBuffer && fConsume < fEnd)
	 &&	GUSI_CASSERT_INTERNAL(fFree + fValid + fSpare == fEnd - fBuffer)
	 &&	GUSI_CASSERT_INTERNAL(Distance(fConsume, fProduce) == (fValid + fSpare) % (fEnd - fBuffer))
	 &&	GUSI_CASSERT_INTERNAL(Distance(fProduce, fConsume) == fFree % (fEnd - fBuffer));

	Release();
	
	return invariant;
}
// The constructor initializes everything.                                 
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
GUSIRingBuffer::GUSIRingBuffer(size_t bufsiz)
{
	// <Initialize buffer of [[GUSIRingBuffer]]>=                              
 fBuffer			=	fConsume	=	fProduce	=	bufsiz ? NewPtr(bufsiz) : 0;
 fEnd			=	fBuffer+bufsiz;

 fValid			=	fSpare		=	0;
 fFree			=	bufsiz;
	// <Initialize fields of [[GUSIRingBuffer]]>=                              
 fInUse			=	false;
 // <Initialize fields of [[GUSIRingBuffer]]>=                              
 fLocked			=	0;
 fDeferred		=	nil;
 fDeferredArg	=	nil;
 // <Initialize fields of [[GUSIRingBuffer]]>=                              
 fNewBuffer	=	nil;
 fOldBuffer	=	nil;	
	
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::GUSIRingBuffer()!\n");
}
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::SwitchBuffer(size_t bufsiz)
{
	PurgeBuffers();
	Lock();
	if (fNewBuffer)
		fNewBuffer->SwitchBuffer(bufsiz);
	else if (bufsiz == fEnd-fBuffer)		// No change
		return;
	else if (!fInUse && !fValid) {
		if (fBuffer)
			DisposePtr(fBuffer);
		// <Initialize buffer of [[GUSIRingBuffer]]>=                              
  fBuffer			=	fConsume	=	fProduce	=	bufsiz ? NewPtr(bufsiz) : 0;
  fEnd			=	fBuffer+bufsiz;

  fValid			=	fSpare		=	0;
  fFree			=	bufsiz;
	} else 
		fNewBuffer = new GUSIRingBuffer(bufsiz);
	Release();
}
void GUSIRingBuffer::PurgeBuffers()
{
	if (fOldBuffer) {
		delete fOldBuffer;
		
		fOldBuffer = nil;
	}
}
// [[ObsoleteBuffer]] swaps the data structures of [[fNewBuffer]] and the current
// buffer and finally prepends [[fNewBuffer]] to [[fOldBuffer]].           
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::ObsoleteBuffer()
{
	Ptr oldBuffer = fBuffer;
	fBuffer 		= fNewBuffer->fBuffer;
	fEnd	 		= fNewBuffer->fEnd;
	fConsume 		= fNewBuffer->fConsume;
	fProduce 		= fNewBuffer->fProduce;
	fFree 			= fNewBuffer->fFree;
	fValid 			= fNewBuffer->fValid;
	fSpare 			= fNewBuffer->fSpare;
	fInUse 			= fNewBuffer->fInUse;
	fNewBuffer->fOldBuffer  = fOldBuffer;
	fOldBuffer		= fNewBuffer;
	fNewBuffer		= fOldBuffer->fNewBuffer;
	fOldBuffer->fBuffer		= oldBuffer;
	fOldBuffer->fNewBuffer	= nil;
}
// The destructor cleans up deferred procedures and allocated memory.      
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
GUSIRingBuffer::~GUSIRingBuffer()
{
	Lock();
	Release();
	if (fBuffer)
		DisposePtr(fBuffer);
	PurgeBuffers();
	if (fNewBuffer)
		delete fNewBuffer;
}
// Since the direct interface is built on the indirect interface, we deal with the latter 
// first. [[ProduceBuffer]] is the most complicated member function, as it has the most
// freedom in how to do its job.                                           
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void * GUSIRingBuffer::ProduceBuffer(size_t & len)
{
	Lock();
	// If a new buffer exists, [[ProduceBuffer]] requests get directed there.  
 //                                                                         
 // <Forward [[ProduceBuffer]] to [[fNewBuffer]]>=                          
 while (fNewBuffer) {
 	if (!fValid)
 		ObsoleteBuffer();
 	else {
 		void * buf = fNewBuffer->ProduceBuffer(len);
 		Release();
 		return buf;
 	}
 }
	size_t	requested_length	=	len;

	// To keep free space as contiguous as possible, we reset the production and consumption
 // pointers whenever the buffer becomes empty. Since this code is only called from 
 // [[ProduceBuffer]], we know that there is no outstanding data producing call. This
 // is also a good opportunity to switch buffers.                           
 //                                                                         
 // <Reset pointers if [[GUSIRingBuffer]] is empty>=                        
 if (!fValid) {
 	fProduce 	= 	fConsume = fBuffer;
 	fSpare   	= 	0;
 	fFree		=	fEnd - fBuffer;
 }	
	// At the core of [[ProduceBuffer]] is the code to calculate the best possible free block
 // in [[streak]] and adjust [[len]].                                       
 //                                                                         
 // <Calculate the size of a free block in [[GUSIRingBuffer]] and adjust [[len]]>=
 size_t	streak	=	fEnd - fProduce;
 if (streak >= fFree)
 	streak = fFree;
 else 
 	// Now we turn to the secret of [[fSpare]]: If a large produce buffer is asked for, the free
  // area wraps around, and most of the free area is at the beginning of the buffer, [[fProduce]]
  // skips the rest of the buffer and the number of skipped bytes is noted in [[fSpare]] so 
  // [[fConsume]] will skip them later, too.                                 
  //                                                                         
  // <Avoid silly windows in [[GUSIRingBuffer]]>=                            
  if (streak < (fFree >> 1) && streak < len) {
  	fSpare 		=	streak;
  	fProduce	=	fBuffer;
  	fFree		-=	fSpare;
  	streak		=	fFree;
  }
 if (len > streak)
 	len	= streak;
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::ProduceBuffer()!\n");
	GUSI_CASSERT_INTERNAL(len <= requested_length);
	void *	result 	= fProduce;
	fInUse		= true;
	Release();
	return result;
}
// Compared to [[ProduceBuffer]], [[ConsumeBuffer]] is quite simple as we don't have the 
// option of skipping anything.                                            
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void * GUSIRingBuffer::ConsumeBuffer(size_t & len)
{
	Lock();
	size_t	requested_length	=	len;
	// At this point, empty buffers can be thrown away unless they are in use  
 //                                                                         
 // <[[ObsoleteBuffer]] if possible>=                                       
 while (fNewBuffer && !fValid && !fInUse)
 	ObsoleteBuffer();
	size_t	streak	=	fEnd - fConsume - fSpare;
	if (streak > fValid)
		streak = fValid;
	if (len > streak)
		len	=	streak;
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::ConsumeBuffer()!\n");
	GUSI_CASSERT_INTERNAL(len <= requested_length);
	void *	result = fConsume;
	Release();
	return result;
}
// [[ValidBuffer]] concludes an action started by [[ProduceBuffer]] by advancing [[fProduce]].
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::ValidBuffer(void * buffer, size_t len)
{
	Lock();
	if (fNewBuffer && (buffer < fBuffer || buffer >= fEnd)) {
		fNewBuffer->ValidBuffer(buffer, len);
		Release();
		return;
	}
	GUSI_CASSERT_INTERNAL(len <= fFree);
	GUSI_CASSERT_INTERNAL(fProduce + len <= fEnd);
	fInUse		=	false;
	fValid 		+= 	len;
	fFree		-=	len;
	fProduce	+=	len;
	if (fProduce == fEnd)
		fProduce	=	fBuffer;
	// At this point, empty buffers can be thrown away unless they are in use  
 //                                                                         
 // <[[ObsoleteBuffer]] if possible>=                                       
 while (fNewBuffer && !fValid && !fInUse)
 	ObsoleteBuffer();
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::ValidBuffer()!\n");
	Release();
}
// [[FreeBuffer]] concludes an action started by [[ConsumeBuffer]] by advancing [[fConsume]].
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::FreeBuffer(void *, size_t len)
{
	Lock();
	fFree 		+=	len;
	fValid		-=	len;
	fConsume	+=	len;
	
	if (fConsume == fEnd-fSpare) {
		fConsume	=	fBuffer;
		fFree		+=	fSpare;
		fSpare		=	0;
	}
	// At this point, empty buffers can be thrown away unless they are in use  
 //                                                                         
 // <[[ObsoleteBuffer]] if possible>=                                       
 while (fNewBuffer && !fValid && !fInUse)
 	ObsoleteBuffer();
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::FreeBuffer()!\n");
	Release();
}
// Now for the direct interface. [[Produce]] combines [[ProduceBuffer]] with [[ValidBuffer]].
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::Produce(void * from, size_t & len)
{
	size_t	part;
	size_t	rest;
	void *	buf;
	
	PurgeBuffers();
	for (rest = len; (part = rest) && fFree; rest -= part)	{
		buf	=	ProduceBuffer(part);
		BlockMoveData(from, buf, part);
		ValidBuffer(buf, part);
		
		from	= static_cast<char *>(from)+part;
	}
	len	-=	rest;
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::Produce()!\n");
}
// [[Consume]] combines [[ConsumeBuffer]] with [[FreeBuffer]].             
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::Consume(void * to, size_t & len)
{
	size_t	part;
	size_t	rest;
	void *	buf;
	
	PurgeBuffers();
	for (rest = len; (part = rest) && fValid; rest -= part)	{
		buf	= ConsumeBuffer(part);
		if (to) {
			BlockMoveData(buf, to, part);
			to	= static_cast<char *>(to)+part;
		}
		FreeBuffer(buf, part);
	}
	
	len	-=	rest;
	GUSI_SASSERT_INTERNAL(
		Invariant(), "Invariant violated in GUSIRingBuffer::Consume()!\n");
}
// <Member functions for class [[GUSIRingBuffer]]>=                        
void GUSIRingBuffer::IterateIOVec(
	const GUSIScattGath & sg, size_t & len, size_t & offset, bool produce)
{
	const iovec *	vec = sg.IOVec();
	iovec 			io  = vec[0];

	if (!len)	// Surprising as it is, nobody notices this any sooner
		return;	
	
	Lock();
	// <Skip [[offset]] bytes from beginning of [[vec]]>=                      
 size_t		off = offset;

 while (off >= io.iov_len) {
 	off -= io.iov_len;
 	do {
 		io = *++vec;
 	} while (!io.iov_len);
 }
 io.iov_base	= static_cast<char *>(io.iov_base)+off;
 io.iov_len  = io.iov_len-off;
	size_t part;
	size_t rest = len;
	while (part = min(rest, io.iov_len)) {
		size_t donepart = part;
		if (produce)
			Produce(io.iov_base, donepart);
		else
			Consume(io.iov_base, donepart);
		rest   -= donepart;
		if (donepart != part)
			break;
		do {
			io = *++vec;
		} while (!io.iov_len);
	}
	len 	-= rest;
	offset	+= len;
	Release();
}
// [[Free]] returns the free buffer space of the most recent buffer.       
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
size_t GUSIRingBuffer::Free()		
{ 
	if (fNewBuffer)
		return fNewBuffer->Free();
	else
		return fFree;		
}	
// [[Free]] returns the sum of the valid bytes of all buffers.             
//                                                                         
// <Member functions for class [[GUSIRingBuffer]]>=                        
size_t GUSIRingBuffer::Valid()
{ 
	if (fNewBuffer)
		return fNewBuffer->Valid()+fValid;
	else
		return fValid;		
}	
// <Member functions for class [[GUSIRingBuffer::Peeker]]>=                
GUSIRingBuffer::Peeker::Peeker(GUSIRingBuffer & buffer)
 : fTopBuffer(buffer)
{
	fTopBuffer.Lock();
	for (fCurBuffer = &fTopBuffer; fCurBuffer && !fCurBuffer->fValid; )
		fCurBuffer = fCurBuffer->fNewBuffer;
	if (fCurBuffer)
		fPeek = fCurBuffer->fConsume;
}
GUSIRingBuffer::Peeker::~Peeker()
{
	fTopBuffer.Release();
}
// <Member functions for class [[GUSIRingBuffer::Peeker]]>=                
void *	GUSIRingBuffer::Peeker::PeekBuffer(size_t & len)
{
	size_t	streak;
	
	if (!fCurBuffer)
		return nil;
		
	if (fPeek < fCurBuffer->fConsume)
		streak = fCurBuffer->fConsume - fPeek;
	else
		streak = fCurBuffer->fEnd - fPeek - fCurBuffer->fSpare;
	if (streak > fCurBuffer->fValid)
		streak = fCurBuffer->fValid;
	if (len > streak)
		len	=	streak;
	void *	result = fPeek;
	// <Advance [[fPeek]] by [[len]]>=                                         
 fPeek	+=	len;
 if (fPeek == fCurBuffer->fEnd-fCurBuffer->fSpare)
 	fPeek = fCurBuffer->fBuffer;
 if (fPeek == fCurBuffer->fConsume) {
 	while ((fCurBuffer = fCurBuffer->fNewBuffer) && !fCurBuffer->fValid)
 		;
 	if (fCurBuffer)
 		fPeek = fCurBuffer->fConsume;
 }
	
	return result;
}
// The implementation of [[Peek]] itself is then fairly simple.            
//                                                                         
// <Member functions for class [[GUSIRingBuffer::Peeker]]>=                
void GUSIRingBuffer::Peeker::Peek(void * to, size_t & len)
{
	size_t	part;
	size_t	rest;
	void *	buf;
	
	for (rest = len; (part = rest) && (buf = PeekBuffer(part)); rest -= part) {
		BlockMoveData(buf, to, part);
		to	= static_cast<char *>(to)+part;
	}
	
	len	-=	rest;
}
// <Member functions for class [[GUSIRingBuffer::Peeker]]>=                
void GUSIRingBuffer::Peeker::Peek(const GUSIScatterer & scatter, size_t & len)
{
	const iovec *	vec = scatter.IOVec();
	iovec 			io  = vec[0];

	while (!io.iov_len)
		io = *++vec;

	size_t part;
	size_t rest = len;
	while (part = min(rest, io.iov_len)) {
		size_t donepart = part;
		Peek(io.iov_base, donepart);
		rest   -= donepart;
		if (donepart != part)
			break;
		do {
			io = *++vec;
		} while (!io.iov_len);
	}
	len 	-= rest;
}

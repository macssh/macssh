// <GUSIMacFile.cp>=                                                       
#define GUSI_MESSAGE_LEVEL 5

#include "GUSIInternal.h"
#include "GUSIMacFile.h"
#include "GUSIFSWrappers.h"
#include "GUSISocketMixins.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"
#include "GUSIConfig.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <stddef.h>
#include <unistd.h>
#include <algorithm>
#include <memory>

#include <Aliases.h>
#include <Devices.h>
#include <TextUtils.h>
#include <Resources.h>

GUSI_USING_STD_NAMESPACE

// \subsection{Interrupt level routines for [[GUSIMacFileSocket]]}         
//                                                                         
//                                                                         
// <Prototypes for [[GUSIMacFileSocket]] interrupt level routines>=        
class GUSIMacFileSocket;

static void GUSIMFRead(GUSIMacFileSocket * sock);
static void GUSIMFReadDone(ParamBlockRec * pb);
static void GUSIMFWrite(GUSIMacFileSocket * sock);
static void GUSIMFWriteDone(ParamBlockRec * pb);
static void GUSIMFWakeup(ParamBlockRec * pb);
// \section{Definition of [[GUSIMacFileSocket]]}                           
//                                                                         
//                                                                         
// <Definition of class [[GUSIMacFileSocket]]>=                            
class GUSIMacFileSocket : 
	public 		GUSISocket,
	public 		GUSISMProcess,
	protected	GUSISMAsyncError,
	protected 	GUSISMBlocking,
	protected	GUSISMState,
	protected	GUSISMInputBuffer,
	protected 	GUSISMOutputBuffer
{
public:
	GUSIMacFileSocket(short fileRef, bool append, int mode);
	~GUSIMacFileSocket();
	// <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual ssize_t	read(const GUSIScatterer & buffer);
 // To speed up writes, they go into a write-behind buffer and are flushed out
 // asynchronously. This causes errors to be reported with some delay.      
 //                                                                         
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual ssize_t write(const GUSIGatherer & buffer);
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual bool	select(bool * canRead, bool * canWrite, bool * exception);
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int	fsync();
 // [[fcntl]] handles the blocking support.                                 
 //                                                                         
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int fcntl(int cmd, va_list arg);
 // [[ioctl]] deals with blocking support and with [[FIONREAD]].            
 //                                                                         
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int ioctl(unsigned int request, va_list arg);
 // [[getsockopt]] and [[setsockopt]] are available for setting buffer sizes and getting
 // asynchronous errors.                                                    
 //                                                                         
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int getsockopt(int level, int optname, void *optval, socklen_t * optlen);
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int setsockopt(int level, int optname, void *optval, socklen_t optlen);
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual off_t lseek(off_t offset, int whence);
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int ftruncate(off_t offset);
 // [[fstat]] is implemented via [[stat]] (early implementations had it the other 
 // way around, but this failed with directories).                          
 //                                                                         
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual int	fstat(struct stat * buf);
 // File sockets implement simple calls.                                    
 //                                                                         
 // <Overridden member functions for [[GUSIMacFileSocket]]>=                
 virtual bool Supports(ConfigOption config);
private:
	// \section{Implementation of [[GUSIMacFileSocket]]}                       
 //                                                                         
 // The implementation of [[GUSIMacFileSocket]] consists of a synchronous high level
 // part which mostly deals with [[GUSIRingBuffers]] and an asynchronous low
 // level part. Since file sockets have some different properties than other sockets,
 // it is helpful to start with the high level routines.                    
 //                                                                         
 // \subsection{High level interface for [[GUSIMacFileSocket]]}             
 //                                                                         
 // The constructor has to initialize a rather large number of data fields. 
 //                                                                         
 // <Privatissima of [[GUSIMacFileSocket]]>=                                
 short	fFileRef;
 bool	fAppend;
 bool	fWriteBehind;
 // Under some circumstances, [[read]] uses a readahead buffer. Since this  
 // optimization can actually pessimize applications, we're very conservative
 // by only turning it on for readonly files and by turning it off as soon as
 // [[lseek]] is called. [[SyncRead]] puts the socket into a predictable state
 // afterwards.                                                             
 //                                                                         
 // [[fPosition]] contains the next position where operations apply, not counting
 // read-ahead data, but counting write-behind. [[fReadAhead]] indicates that a
 // read-ahead should be started on the first read operation. [[fBeyondEOF]] indicates
 // that [[fPosition]] resulted from a seek beyond the current end of file position.
 //                                                                         
 // <Privatissima of [[GUSIMacFileSocket]]>=                                
 bool	fReadAhead;
 bool	fBeyondEOF;
 long	fPosition;
 // <Privatissima of [[GUSIMacFileSocket]]>=                                
 void SyncRead();
 // As opposed to [[SyncRead]], [[SyncWrite]] doesn't change the long-term strategy.
 // By temporarily turning off [[fWriteBehind]], it signals that the write strategy
 // should write the remaining data even if it does not align to cache blocks.
 //                                                                         
 // <Privatissima of [[GUSIMacFileSocket]]>=                                
 void SyncWrite();
 // It is often advantageous to keep I/O requests a multiple of the allocation
 // block size.                                                             
 //                                                                         
 // <Privatissima of [[GUSIMacFileSocket]]>=                                
 size_t		fBlockSize;
 // <Privatissima of [[GUSIMacFileSocket]]>=                                
 ParamBlockRec				fReadPB;
 ParamBlockRec				fWritePB;
 friend void 				GUSIMFRead(GUSIMacFileSocket * sock);
 friend void 				GUSIMFReadDone(ParamBlockRec * pb);
 friend void 				GUSIMFWrite(GUSIMacFileSocket * sock);
 friend void 				GUSIMFWriteDone(ParamBlockRec * pb);
 friend void 				GUSIMFWakeup(ParamBlockRec * pb);
 static IOCompletionUPP		sReadProc;
 static IOCompletionUPP		sWriteProc;
 static IOCompletionUPP		sWakeupProc;
};
// \section{Definition of [[GUSIMacDirectory]]}                            
//                                                                         
//                                                                         
// <Definition of class [[GUSIMacDirectory]]>=                             
class GUSIMacDirectory : public GUSIDirectory {
public:
	GUSIMacDirectory(const FSSpec & spec);
	
	// <Overridden member functions for [[GUSIMacDirectory]]>=                 
 virtual dirent    *	readdir();
 // <Overridden member functions for [[GUSIMacDirectory]]>=                 
 virtual long 		telldir();
 // <Overridden member functions for [[GUSIMacDirectory]]>=                 
 virtual void 		seekdir(long pos);
 // <Overridden member functions for [[GUSIMacDirectory]]>=                 
 virtual void 		rewinddir();
private:
	// \section{Implementation of [[GUSIMacDirectory]]}                        
 //                                                                         
 //                                                                         
 // <Privatissima of [[GUSIMacDirectory]]>=                                 
 GUSIFileSpec	fDir;
 short			fPos;
 dirent          fEnt;
};
// [[GUSIMFWriteDone]] does all its work between [[fWritePB]] and [[fOutputBuffer]].
// If a write fails, the whole write buffer is cleared.                    
//                                                                         
// <Interrupt level routines for [[GUSIMacFileSocket]]>=                   
static void GUSIMFWriteDone(ParamBlockRec * pb)
{
	GUSIMacFileSocket * sock = 
		reinterpret_cast<GUSIMacFileSocket *>((char *)pb-offsetof(GUSIMacFileSocket, fWritePB));
	// To atone for the risky practice of [[reinterpret_cast<>]], we at least check for 
 // plausibility by comparing the two fields storing the file reference number.
 //                                                                         
 // <Perform sanity cross-check between [[sock]] and [[pb]]>=               
 GUSI_CASSERT_INTERNAL(sock->fFileRef == pb->ioParam.ioRefNum);
	GUSIProcess::A5Saver saveA5(sock->Process());	
	GUSI_SMESSAGE("Write done.\n");
	if (sock->fOutputBuffer.Locked())
		sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMFWriteDone), pb);
	else {
		sock->fOutputBuffer.ClearDefer();
		sock->fOutputBuffer.FreeBuffer(
			sock->fWritePB.ioParam.ioBuffer, sock->fWritePB.ioParam.ioReqCount);
		if (sock->SetAsyncMacError(sock->fWritePB.ioParam.ioResult)) {
			for (long valid; valid = sock->fOutputBuffer.Valid(); )
				sock->fOutputBuffer.FreeBuffer(nil, valid);
			sock->fWriteShutdown 	= true;
		}
		GUSIMFWrite(sock);
		sock->Wakeup();
	}
}
// [[GUSIMFWrite]] starts a file write call if there is data to write and otherwise
// sets itself up as the deferred procedure of the output buffer (and thus is 
// guaranteed to be called the next time data is deposited in the buffer again).
// If all data has been delivered and a shutdown is requested, terminate.  
//                                                                         
// <Interrupt level routines for [[GUSIMacFileSocket]]>=                   
static void GUSIMFWrite(GUSIMacFileSocket * sock)
{
	size_t	valid = sock->fOutputBuffer.Valid();
	if (sock->fWriteShutdown && !valid)
		sock->fOutputBuffer.ClearDefer();
	else if (!valid)
		sock->fOutputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMFWrite), sock);
	else {
		sock->fOutputBuffer.ClearDefer();
		valid = min(valid, sock->fOutputBuffer.Size() >> 1);
		sock->fWritePB.ioParam.ioBuffer		= 
			static_cast<Ptr>(sock->fOutputBuffer.ConsumeBuffer(valid));
		// Whenever possible, we try to maintain alignment with allocation block   
  // boundaries. If write to a write-only file is aligned, we request that it
  // not be cached, which improves performance.                              
  //                                                                         
  // <Align write size with [[fBlockSize]] and file position>=               
  long pos 		= sock->fPosition - sock->fOutputBuffer.Valid();
  size_t align	= pos % sock->fBlockSize;
  if (align)
  	valid = min(valid, sock->fBlockSize-align);
  else if (valid > sock->fBlockSize)
  	valid -= valid % sock->fBlockSize;
  sock->fWritePB.ioParam.ioPosMode = fsAtMark +
  	(sock->fWriteBehind && valid >= sock->fBlockSize) ? noCacheMask : 0;
		sock->fWritePB.ioParam.ioReqCount	= valid;
		sock->fWritePB.ioParam.ioActCount	= 0;
			
		GUSI_MESSAGE(("Async write (0x%x bytes).\n", valid));
		PBWriteAsync(&sock->fWritePB);		
	}
}
// [[GUSIMFReadDone]] does all its work between [[fReadPB]] and [[fInputBuffer]].
// If a read fails, the whole write buffer is cleared.                     
//                                                                         
// <Interrupt level routines for [[GUSIMacFileSocket]]>=                   
static void GUSIMFReadDone(ParamBlockRec * pb)
{
	GUSIMacFileSocket * sock = 
		reinterpret_cast<GUSIMacFileSocket *>((char *)pb-offsetof(GUSIMacFileSocket, fReadPB));
	// To atone for the risky practice of [[reinterpret_cast<>]], we at least check for 
 // plausibility by comparing the two fields storing the file reference number.
 //                                                                         
 // <Perform sanity cross-check between [[sock]] and [[pb]]>=               
 GUSI_CASSERT_INTERNAL(sock->fFileRef == pb->ioParam.ioRefNum);
	GUSIProcess::A5Saver saveA5(sock->Process());	
	GUSI_MESSAGE(("Read done (0x%x/%d).\n", pb->ioParam.ioActCount, pb->ioParam.ioResult));
	if (sock->fInputBuffer.Locked())
		sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMFReadDone), pb);
	else {
		sock->fInputBuffer.ClearDefer();
		sock->fInputBuffer.ValidBuffer(
			sock->fReadPB.ioParam.ioBuffer, sock->fReadPB.ioParam.ioActCount);
		if (sock->SetAsyncMacError(sock->fReadPB.ioParam.ioResult))
			sock->fReadShutdown = true;
		else if (!sock->fReadShutdown)
			GUSIMFRead(sock);
		sock->Wakeup();
	}
}
// [[GUSIMFRead]] starts a file read call.                                 
//                                                                         
// <Interrupt level routines for [[GUSIMacFileSocket]]>=                   
static void GUSIMFRead(GUSIMacFileSocket * sock)
{
	size_t	free = sock->fInputBuffer.Free();
	if (free < (sock->fInputBuffer.Size() >> 2)) {
		if (!sock->fReadShutdown)
			sock->fInputBuffer.Defer(GUSIRingBuffer::Deferred(GUSIMFRead), sock);
		else 
			sock->fInputBuffer.ClearDefer();
	} else {
		sock->fInputBuffer.ClearDefer();
		free = min(free, sock->fInputBuffer.Size() >> 1);
		// Whenever possible, we try to maintain alignment with allocation block   
  // boundaries.                                                             
  //                                                                         
  // <Align read size with [[fBlockSize]] and file position>=                
  long pos 		= sock->fPosition + sock->fInputBuffer.Valid();
  size_t align	= pos % sock->fBlockSize;
  if (align)
  	free = min(free, sock->fBlockSize-align);
  else if (free > sock->fBlockSize)
  	free -= free % sock->fBlockSize;

		sock->fReadPB.ioParam.ioBuffer		= 
			static_cast<Ptr>(sock->fInputBuffer.ProduceBuffer(free));
		sock->fReadPB.ioParam.ioReqCount	= free;
		sock->fReadPB.ioParam.ioActCount	= 0;
			
		GUSI_MESSAGE(("Async read 0x%x 0x%x (0x%x bytes).\n", sock, &sock->fReadPB, free));
		PBReadAsync(&sock->fReadPB);		
	}
}
// [[GUSIMFWakeup]] simply wakes up its socket.                            
//                                                                         
// <Interrupt level routines for [[GUSIMacFileSocket]]>=                   
static void GUSIMFWakeup(ParamBlockRec * pb)
{
	GUSIMacFileSocket * sock = 
		reinterpret_cast<GUSIMacFileSocket *>((char *)pb-offsetof(GUSIMacFileSocket, fReadPB));
	// To atone for the risky practice of [[reinterpret_cast<>]], we at least check for 
 // plausibility by comparing the two fields storing the file reference number.
 //                                                                         
 // <Perform sanity cross-check between [[sock]] and [[pb]]>=               
 GUSI_CASSERT_INTERNAL(sock->fFileRef == pb->ioParam.ioRefNum);
	sock->Wakeup();
}
// On 68K machines, completion procedures get called with the argument in the
// A0 register, which really cramps our style. Therefore, we wrap each     
// procedure.                                                              
//                                                                         
// <Interrupt routine wrappers for [[GUSIMacFileSocket]]>=                 
GUSI_COMPLETION_PROC_A0(GUSIMFReadDone, ParamBlockRec)
GUSI_COMPLETION_PROC_A0(GUSIMFWriteDone, ParamBlockRec)
GUSI_COMPLETION_PROC_A0(GUSIMFWakeup, ParamBlockRec)
// \section{Implementation of [[GUSIMacFileDevice]]}                       
//                                                                         
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
static auto_ptr<GUSIMacFileDevice>	sGUSIMacFileDevice;
// <Member functions for class [[GUSIMacFileDevice]]>=                     
GUSIMacFileDevice * GUSIMacFileDevice::Instance()
{
	if (!sGUSIMacFileDevice.get())
		sGUSIMacFileDevice = auto_ptr<GUSIMacFileDevice>(new GUSIMacFileDevice);
	return sGUSIMacFileDevice.get();
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
GUSIMacFileDevice::~GUSIMacFileDevice()
{
	CleanupTemporaries(true);
}
// [[GUSIMacFileDevice]] will handle no device requests. It will always fail
// if any parent directory of the file does not exist, and for most calls  
// fails unless the file itself exists.                                    
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
bool GUSIMacFileDevice::Want(GUSIFileToken & file)
{
	if (!file.IsFile())
		return false;
	if (!file.Error() && file.Exists())
		return true;
	if (file.Error() == fnfErr) 
		switch (file.WhichRequest()) {
		case GUSIFileToken::kWillOpen:
		case GUSIFileToken::kWillMkdir:
		case GUSIFileToken::kWillSymlink:	// A borderline case
			return true;
		}
	
	return false;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
GUSISocket * GUSIMacFileDevice::open(GUSIFileToken & file, int flags)
{
	OSErr			err;
	char 			permission;
	short			fileRef;
	
	// First, we need to translate the POSIX open flags into a Mac file permission.
 //                                                                         
 // <Translate [[flags]] into [[permission]]>=                              
 switch (flags & 3) {
 case O_RDONLY:		
 	permission = fsRdPerm; 		
 	break;
 case O_WRONLY:
 case O_RDWR:		
 	permission = 
 		GUSIConfiguration::Instance()->fSharedOpen ? fsRdWrShPerm : fsRdWrPerm; 	
 	break;
 }
tryOpen:
	// Now, we attempt to open the data or resource fork of the file.          
 //                                                                         
 // <Attempt [[Open]] into [[fileRef]]>=                                    
 if (flags & O_RSRC)
 	err = GUSIFSOpenRF(&file, permission, &fileRef);
 else
 	err = GUSIFSOpenDF(&file, permission, &fileRef);
	// If the file did not exist, we try to create it. We don't need to worry  
 // about special casing for [[O_RSRC]] here, I think.                      
 //                                                                         
 // <If file didn't exist, try to create it and [[goto tryOpen]] if successful>=
 if (err == fnfErr && (flags & O_CREAT)) {
 	if (!(err = GUSIFSCreate(&file))) {
 		GUSIConfiguration::Instance()->SetDefaultFType(file);
 		file.TouchFolder();
 	}
 	if (!err || err == dupFNErr) {
 		flags &= ~(O_CREAT | O_EXCL); 			// Avoid loop or false positive
 		goto tryOpen;							// Retry
 	}
 }
	// Now we check whether the [[Open]] ultimately succeeded.                 
 //                                                                         
 // <Handle errors from [[Open]]>=                                          
 switch (err) {
 case afpObjectTypeErr:
 	return GUSISetPosixError(EISDIR), static_cast<GUSIMacFileSocket *>(nil);
 default:		
 	return GUSISetMacError(err), static_cast<GUSIMacFileSocket *>(nil);
 case noErr:
 	break;
 }
	// If we got to this point with [[O_EXCL]] still set, the file must have existed,
 // but it shouldn't have. If [[O_TRUNC]] is set, truncate the file.        
 //                                                                         
 // <Handle [[O_EXCL]] and [[O_TRUNC]]>=                                    
 if (flags & O_EXCL) {
 	FSClose(fileRef);
 	return GUSISetPosixError(EEXIST), static_cast<GUSIMacFileSocket *>(nil);
 }
 		
 if (flags & O_TRUNC)
 	SetEOF(fileRef, 0);
	
	return open(fileRef, flags);
}
// There is also a special case backdoor [[open]], available only for      
// [[GUSIMacFileDevice]] to handle files which are already open somehow.   
// This open will never fail except for lack of memory.                    
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
GUSISocket * GUSIMacFileDevice::open(short fileRef, int flags)
{
	GUSISocket * sock;
	
	if (!(sock = 
		new GUSIMacFileSocket(fileRef, (flags & O_APPEND) != 0, flags & 3)
	)) {
		FSClose(fileRef);
		GUSISetPosixError(ENOMEM);
	}
	
	return sock;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::remove(GUSIFileToken & file)
{
	OSErr err;
	
	switch (err = GUSIFSDelete(&file)) {
	case noErr:
		return 0;
	case fBsyErr:
		return MarkTemporary(file);
	default:
		return GUSISetMacError(err);
	}
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::MarkTemporary(const FSSpec & file)
{
	OSErr				err;
	GUSITempFileSpec	temp(file.vRefNum, file.name);
	
	if (err = GUSIFSMoveRename(&file, &temp))
		return GUSISetMacError(err);
	TempQueue * q 	= new TempQueue;
	q->fNext 		= fTemporaries;
	q->fSpec 		= temp;
	fTemporaries 	= q;
	
	return 0;
}
// [[CleanupTemporaries]] tries at promising times to delete as many of the temporary
// files as possible.                                                      
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
void GUSIMacFileDevice::CleanupTemporaries(bool giveup)
{
	TempQueue ** p = &fTemporaries;
	
	for (TempQueue * q = *p; q; q = *p) 
		if (GUSIFSDelete(&q->fSpec) != fBsyErr || giveup) {
			// Delete entry
			*p = q->fNext;
			delete q;
		} else {
			// Keep entry
			p = &q->fNext;
		}
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::rename(GUSIFileToken & file, const char * newname)
{
	OSErr			err;
	GUSIFileSpec	newfile(newname, true);
	
	// We can only move a file around a volume.                                
 //                                                                         
 // <Ensure that file to rename exists and new name resides on same volume>=
 if (!file.Exists())
 	return GUSISetMacError(file.Error());
 if (file->vRefNum != newfile->vRefNum)
 	return GUSISetPosixError(EXDEV);
	bool	samename = EqualString(file->name, newfile->name, true, true);
	bool    samedir  = file->parID == newfile->parID;
	bool    newexists= newfile.Exists();
	// If one of the two files is locked, or if the two names are distinct only by case,
 // we call ourselves recursively, wrapped in the special case code. These conditions
 // are rare, so some inefficiency is not all that tragic.                  
 //                                                                         
 // <Handle special renaming cases by recursing>=                           
 if (samename && samedir)
 	return 0;
 if (file.CatInfo()->Locked()) {
 	GUSIFSRstFLock(&file);
 	int res = rename(file, newname);
 	GUSIFSSetFLock(res ? &file : &newfile);
 	return res;
 }
 if (newexists && file.CatInfo()->Locked()) {
 	GUSIFSRstFLock(&newfile);
 	int res = rename(file, newname);
 	if (res)
 		GUSIFSSetFLock(&newfile);
 	return res;
 }
 if (samedir && !samename && EqualString(file->name, newfile->name, false, true))
 	newexists = false;
	// If the target name refers to an existing file, we stash it away until we're
 // sure the rename succeeded.                                              
 //                                                                         
 // <Move existing [[target]] out of the way>=                              
 GUSIFileSpec	target;
 if (newexists) {
 	target = GUSITempFileSpec(newfile->vRefNum, newfile->parID);
 	if (err = GUSIFSRename(&newfile, target->name))
 		return GUSISetMacError(err);
 }
	if (samedir)
		err = GUSIFSRename(&file, newfile->name);
	else if (samename) 
		err = GUSIFSCatMove(&file, newfile->parID);
	else 
		err = GUSIFSMoveRename(&file, &newfile);

cleanuptarget:
	// <Clean up existing target>=                                             
 if (newexists)
 	if (!err)
 		GUSIFSDelete(&target);
 	else
 		GUSIFSRename(&target, newfile->name);
		
	return GUSISetMacError(err);
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::stat(GUSIFileToken & file, struct stat * buf)
{
	if (file.IsDevice()) {
		// Any existing device should return an OK result, so give just a set of defaults.
  //                                                                         
  // <[[return]] an alibi [[stat]] buffer>=                                  
  buf->st_dev			=	'dev ';
  buf->st_ino			=	'????';
  buf->st_mode		=	S_IFCHR | 0666 ;
  buf->st_nlink		=	1;
  buf->st_uid			=	0;
  buf->st_gid			=	0;
  buf->st_rdev		=	0;
  buf->st_size		=	1;
  buf->st_atime		=	time(NULL);
  buf->st_mtime		=	time(NULL);
  buf->st_ctime		=	time(NULL);
  buf->st_blksize		=	0;
  buf->st_blocks		=	1;

  return 0;
	} else if (!file.Exists())
		return GUSISetPosixError(ENOENT);
		
	const GUSICatInfo & 			cb = *file.CatInfo();
	GUSIIOPBWrapper<XVolumeParam>	pb;
	Str63							vName;
	
	pb->ioNamePtr	=	vName;
	pb->ioVRefNum	=	file->vRefNum;
	pb->ioVolIndex	=	0;

	if (GUSIFSXGetVolInfo(&pb))
		return GUSISetPosixError(ENOENT);
		
	buf->st_dev		=	pb->ioVRefNum;
	buf->st_ino		=	cb.DirInfo().ioDrDirID;
	buf->st_nlink	=	1;
	buf->st_uid		=	0;
	buf->st_gid		=	0;
	buf->st_rdev		=	0;
	buf->st_atime	=	cb.FileInfo().ioFlMdDat;
	buf->st_mtime	=	cb.FileInfo().ioFlMdDat;
	buf->st_ctime	=	cb.FileInfo().ioFlCrDat;
	buf->st_blksize	=	pb->ioVAlBlkSiz;

	if (!cb.IsFile())	{
		// Depending on our preference settings, we employ a faster or a slower method
  // in determining the link count. The \emph{accurate} method sets the link 
  // count to 2 plus the number of subdirectories, while the fast method establishes
  // an upper boundary on that number as 2 plus the number of subitems.      
  //                                                                         
  // <Determine the link count for a directory>=                             
  ++buf->st_nlink; 
  if (GUSIConfiguration::Instance()->fAccurateStat) {
  	GUSIFileSpec	spec;

  	spec.SetVRef(pb->ioVRefNum);
  	spec.SetParID(cb.DirInfo().ioDrDirID);
  	for (int i = 0; i++ < cb.DirInfo().ioDrNmFls;) {
  		spec	=	spec[i];
  		if (!spec.Error() && spec.CatInfo() && !spec.CatInfo()->IsFile())
  			++buf->st_nlink;
  	}
  } else {
  	buf->st_nlink	+=	cb.DirInfo().ioDrNmFls;
  }
		buf->st_mode	=	S_IFDIR | 0666;
		if (!gGUSIExecHook || gGUSIExecHook(&file))
			buf->st_mode	|= 0111;

		buf->st_size	=	cb.DirInfo().ioDrNmFls;
	} else if (cb.IsAlias()) {
		buf->st_mode	=	S_IFLNK | 0777;
		buf->st_size	=	cb.FileInfo().ioFlRLgLen;		/* Data fork is ignored	*/
	} else if (cb.FInfo().fdType == '·OCK') {
		buf->st_mode	=	S_IFSOCK | 0666;
		buf->st_size	=	cb.FileInfo().ioFlRLgLen;		/* Data fork is ignored	*/
	} else {
		buf->st_mode		=	S_IFREG | 0666;
		if (cb.FileInfo().ioFlAttrib & 0x01)
			buf->st_mode &=	~0222;
			
		if (gGUSIExecHook) {
			if (gGUSIExecHook(&file))
				buf->st_mode |= 0111;
		} else 
			switch (cb.FInfo().fdType) {
			case 'APPL':
			case 'appe':
				buf->st_mode |= 0111;
			}				

		buf->st_size	=	cb.FileInfo().ioFlLgLen;		/* Resource fork is ignored	*/
	}

	buf->st_blocks	=	(buf->st_size + buf->st_blksize - 1) / buf->st_blksize;
	
	return 0;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::chmod(GUSIFileToken & file, mode_t mode)
{
	if (file.IsDevice())
		return GUSISetPosixError(EINVAL);
	if (!file.Exists())
		return GUSISetPosixError(ENOENT);
	if (file.IsFile()) 
		if (mode & S_IWUSR) {
			if (file.CatInfo()->Locked())
				GUSIFSRstFLock(&file);
		} else {
			if (!file.CatInfo()->Locked())
				GUSIFSSetFLock(&file);
		}
	
	return 0;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::utime(GUSIFileToken & file, const utimbuf * times)
{
	if (file.IsDevice())
		return GUSISetPosixError(EINVAL);
	if (!file.Exists())
		return GUSISetPosixError(ENOENT);
	GUSIIOPBWrapper<GUSICatInfo>	info;
	info.fPB = *file.CatInfo();
	info->FileInfo().ioDirID = file->parID;
	if (times)
		info->FileInfo().ioFlMdDat = times->modtime;
	return GUSISetMacError(GUSIFSSetCatInfo(&info));
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::access(GUSIFileToken & file, int mode)
{
	if (file.IsDevice())
		return GUSISetPosixError(EINVAL);
		
	const GUSICatInfo * info	= file.CatInfo();

	if (!info)
		switch (file.Error()) {
			case afpAccessDenied:
				return GUSISetPosixError(EACCES);
			case ioErr:
				return GUSISetPosixError(EIO);
			case bdNamErr:
			case fnfErr:
			case nsvErr:
			case paramErr:
			case dirNFErr:
			case afpObjectTypeErr:
				/* ENOENT */
			default:
				return GUSISetPosixError(ENOENT);		/* Don't know what the error is. */
		}
	
	// 
	// Under our simplifying assumptions, we don't check AppleShare permissions,
	// so if we managed to do a GetCatInfo, R_OK and F_OK are already assumed
	// to be true
	//
	if (mode & W_OK) {
		//
		// Check if volume is locked
		//
		GUSIIOPBWrapper<HParamBlockRec>	vol;
		
		vol->volumeParam.ioNamePtr	= nil;
		vol->volumeParam.ioVolIndex	= 0;
		vol->volumeParam.ioVRefNum	= file->vRefNum;

		if (GUSIFSHGetVInfo(&vol))
			return GUSISetPosixError(EINVAL);	// Should never happen
			
		if (vol->volumeParam.ioVAtrb & 0x8080)
			return GUSISetPosixError(EROFS);	// Yup, volume is locked
		
		//
		// Check if file is locked
		//
		if (info->Locked())
			return GUSISetPosixError(EACCES); // Yup, file is locked
	}

	if (mode & X_OK)
		if (gGUSIExecHook) {
			if (!gGUSIExecHook(&file))
				return GUSISetPosixError(EACCES);
		} else 
			switch (info->FInfo().fdType) {
			case 'APPL':
			case 'appe':
				break;
			default:
				return GUSISetPosixError(EACCES);
			}				

	return 0;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::mkdir(GUSIFileToken & file)
{
	return GUSISetMacError(GUSIFSDirCreate(&file));
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::rmdir(GUSIFileToken & file)
{
	return GUSISetMacError(GUSIFSDelete(&file));
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
GUSIDirectory * GUSIMacFileDevice::opendir(GUSIFileToken & file)
{
	GUSIDirectory * dir = 0;
	
	++file;
	if (!file.Error()) 
		dir = new GUSIMacDirectory(file);
	else
		GUSISetPosixError(ENOTDIR);
		
	return dir;
}
// [[VRef2Icon]] finds the driver icon for a volume.                       
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
static OSErr VRef2Icon(short vRef, Handle * icon)
{
	OSErr	err;
	short 	cRef;

	{
		// <Get volume information into [[vRef]] and [[cRef]]>=                    
  GUSIIOPBWrapper<HParamBlockRec>	hpb;
  hpb->volumeParam.ioVRefNum	=	vRef;
  hpb->volumeParam.ioNamePtr	=	nil;
  hpb->volumeParam.ioVolIndex	=	0;
  if (err = GUSIFSHGetVInfo(&hpb))
  	return err;
  vRef 						=	hpb->volumeParam.ioVDrvInfo;
  cRef						=	hpb->volumeParam.ioVDRefNum;
	}
	{
		// <Get volume icon>=                                                      
  GUSIIOPBWrapper<ParamBlockRec>	pb;
  pb->cntrlParam.ioVRefNum	=	vRef;
  pb->cntrlParam.ioCRefNum	=	cRef;
  pb->cntrlParam.csCode	=	21;

  if (err = pb.Control())
  	return err;

  PtrToHand(*(Ptr *) pb->cntrlParam.csParam, icon, 256);
	}
	
	return noErr;
}
// [[AliasTypeExpert]] finds the right file type, creator, icon location and icon
// ID for the alias. This is complicated by many special cases.            
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
typedef OSType	AliasTypeMap[2];

static AliasTypeMap sMap[]	=	{
	// This is the current list of special folders. I suppose it should be revised 
 // occasionally. In fact, it is probably already out of date.              
 //                                                                         
 // <Special folder types and their alias types>=                           
 {'amnu', 'faam'},
 {'ctrl', 'fact'},
 {'extn', 'faex'},
 {'pref', 'fapf'},
 {'prnt', 'fapn'},
 {'empt', 'trsh'},
 {'trsh', 'trsh'},
 {'strt', 'fast'},
 {'macs', 'fasy'},
	{     0,      0}
};

static void AliasTypeExpert(
	const GUSIFileSpec & file, 
	OSType * 			fCreator, 
	OSType * 			fType,
	GUSIFileSpec * 		iconFile,
	short * 			iconID)
{
	const GUSICatInfo *		info = file.CatInfo();
	
	*fCreator 	= 'MACS';
	*iconFile	= file;
	*iconID		= kCustomIconResource;
	
	if (!info->IsFile())	{
		// One of the complex issues is the various AppleShare related aliases, but those
  // should not change too much.                                             
  //                                                                         
  // <Determine [[fType]] for folder and volume aliases>=                    
  GetVolParmsInfoBuffer				volParms;

  if (!GUSIFSGetVolParms(file->vRefNum, &volParms) && volParms.vMServerAdr)	
  	if (file->parID == fsRtParID)
  		*fType	=	'srvr';
  	else if (!info->HasRdPerm())
  		*fType 	=	'fadr';
  	else
  		*fType	=	'faet';
  else if (file->parID == fsRtParID)
  	*fType	=	'hdsk';
  else if (info->DirIsMounted())
  	*fType = 'famn';
  else if (info->DirIsExported())
  	*fType = 'fash';
  else if (info->DirIsShared())
  	*fType = 'faet';
  else 
  	*fType = 'fdrp';
// <Check for special folder>=                                             
if (file->parID != fsRtParID && info->DirInfo().ioDrDirID < 9) {
	short vRef;
	long  dirID;
	
	for (AliasTypeMap * mapp = sMap; **mapp; ++mapp)
		if (FindFolder(file->vRefNum, (*mapp)[0], false, &vRef, &dirID))
			continue;
		else if (dirID == info->DirInfo().ioDrDirID) {
			*fType = (*mapp)[1];

			break;
		}
}
		if (file->parID == fsRtParID)	{
			iconFile->SetParID(fsRtDirID);
			iconFile->SetName("\pIcon\n");
		} else {
			*iconFile += "\pIcon\n";
		}
	} else {
		*fType	=	info->FInfo().fdType;
		*fCreator=	info->FInfo().fdCreator;
	}

	return;
error:
	*fType		=	0;
	*fCreator	=	0;
}
// [[AddIconsToFile]] adds the appropriate icons to the alias file as the custom
// icon. The mechanical part of the work is performed by [[CopyIconFamily]]
//                                                                         
// <Member functions for class [[GUSIMacFileDevice]]>=                     
static OSType sIconTypes[]	=	{
	'ICN#',
	'ics#',
	'icl4',
	'ics4',
	'icl8', 
	'ics8',
	0
};

static bool CopyIconFamily(
	short srcResFile, short srcID, short dstResFile, short dstID)
{
	Handle	icon;
	bool	success	=	false;

	for (OSType * types = sIconTypes; *types; ++types)	{
		UseResFile(srcResFile);
		if (icon = Get1Resource(*types, srcID))	{
			UseResFile(dstResFile);
			DetachResource(icon);
			AddResource(icon, *types, dstID, "\p");
		
			success = success || !ResError();
		}
	}
	
	return success;
}

static bool AddIconsToFile(
	const GUSIFileSpec &	origFile, 	short aliasFile, 
	const GUSIFileSpec & 	iconFile,  	short iconID, 
	bool  plainDisk)
{
	bool 	hasCustomIcons 	= false;
	short	iFile 			= FSpOpenResFile(&iconFile, fsRdPerm);
	
	if (iFile != -1) {
		hasCustomIcons = 
			CopyIconFamily(iFile, iconID, aliasFile, kCustomIconResource);
		CloseResFile(iFile);
	}
	if (!hasCustomIcons && plainDisk) {
		Handle icon;
		if (!VRef2Icon(origFile->vRefNum, &icon))	{
			AddResource(icon, 'ICN#', kCustomIconResource, "\p");
			
			hasCustomIcons = !ResError();
		}
	}
				
	return hasCustomIcons;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::symlink(GUSIFileToken & to, const char * newlink)
{
	if (!to.Exists())
		return GUSISetPosixError(EIO);
	if (to.IsDevice())
		return GUSISetPosixError(EINVAL);

	OSType			fileType;
	OSType			fileCreator;
	short			iconID;
	short			aliasFile;
	AliasHandle		alias;
	bool			customIcon;
	GUSIFileSpec	iconFile;
	FInfo			info;
			
	// <Create and open [[aliasFile]]>=                                        
 GUSIFileSpec	newnm(newlink, true);

 if (newnm.Error())
 	return GUSISetPosixError(EIO);
 if (newnm.Exists())
 	return GUSISetPosixError(EEXIST);

 AliasTypeExpert(to, &fileCreator, &fileType, &iconFile, &iconID);
 FSpCreateResFile(&newnm, fileCreator, fileType, smSystemScript);

 if (ResError())
 	return GUSISetPosixError(EIO);

 aliasFile = FSpOpenResFile(&newnm, fsRdWrPerm);
 if (aliasFile == -1)
 	goto deleteFile;
		
	if (NewAlias(nil, &to, &alias))
		goto closeFile;
	
	AddResource((Handle) alias, 'alis', 0, to->name);
	if (ResError())
		goto deleteAlias;
	
	customIcon = AddIconsToFile(to, aliasFile, iconFile, iconID,
		fileType == 'hdsk' && fileCreator == 'MACS');
		
	CloseResFile(aliasFile);

	GUSIFSGetFInfo(&newnm, &info);
	info.fdFlags	|=	kIsAlias | (customIcon ? kHasCustomIcon : 0);
	info.fdFlags	&= ~kHasBeenInited;
	GUSIFSSetFInfo(&newnm, &info);
	newnm.TouchFolder();
	
	return 0;

deleteAlias:
	DisposeHandle((Handle) alias);
closeFile:
	CloseResFile(aliasFile);
deleteFile:
	GUSIFSDelete(&newnm);	
	
	return GUSISetPosixError(EIO);
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::readlink(GUSIFileToken & link, char * buf, int bufsize)
{
	if (link.IsDevice())
		return GUSISetPosixError(EINVAL);
	
	char * resPath = link.AliasPath();	

	if (!(resPath))
		if (link.Error() == resFNotFound)
			return GUSISetPosixError(EINVAL);
		else
			return GUSISetMacError(link.Error());
		
	int len = strlen(resPath);
	strncpy(buf, resPath, bufsize);
	
	if (len >= bufsize)
		return GUSISetPosixError(ENAMETOOLONG);
	else
		return len;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::fgetfileinfo(GUSIFileToken & file, OSType * creator, OSType * type)
{
	FInfo	info;	

	if (file.IsDevice())
		return GUSISetPosixError(EINVAL);		
	if (file.Error() || !file.Exists() || GUSIFSGetFInfo(&file, &info))
		return GUSISetPosixError(EIO);
	if (creator)
		*creator	= info.fdCreator;
	if (type)
		*type 		= info.fdType;
	
	return 0;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::fsetfileinfo(GUSIFileToken & file, OSType creator, OSType type)
{
	FInfo	info;	

	if (file.IsDevice())
		return GUSISetPosixError(EINVAL);		
	if (file.Error() || !file.Exists() || GUSIFSGetFInfo(&file, &info))
		return GUSISetPosixError(EIO);
		
	info.fdType 	=	type;
	info.fdCreator	=	creator;
	info.fdFlags	&= ~kHasBeenInited;
	
	if (GUSIFSSetFInfo(&file, &info))
		return GUSISetPosixError(EIO);
	return 0;
}
// <Member functions for class [[GUSIMacFileDevice]]>=                     
int GUSIMacFileDevice::faccess(GUSIFileToken &, unsigned *, void *)
{
	return GUSISetPosixError(EINVAL);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
GUSIMacFileSocket::GUSIMacFileSocket(short fileRef, bool append, int mode)
	: fFileRef(fileRef), fAppend(append), fWriteBehind(false)
{
	switch (mode) {
	case O_RDONLY:
		fWriteShutdown				= true;
		fWritePB.ioParam.ioResult	= wrPermErr;
		break;
	case O_WRONLY:
		fWriteBehind = true;
		// Fall through
	case O_RDWR:
		fReadShutdown = true;
		break;
	}
	// <Initialize fields of [[GUSIMacFileSocket]]>=                           
 fPosition	= 0;
 fReadAhead	= !fReadShutdown;
 fBeyondEOF	= false;
 // <Initialize fields of [[GUSIMacFileSocket]]>=                           
 fBlockSize =	4096;

 GUSIIOPBWrapper<FCBPBRec>	fcb;
 Str63						name;

 fcb->ioNamePtr	= name;
 fcb->ioRefNum	= fFileRef;
 fcb->ioFCBIndx	= 0;
 if (!GUSIFSGetFCBInfo(&fcb)) {
 	GUSIIOPBWrapper<XVolumeParam>	info;
 	info->ioNamePtr	=	name;
 	info->ioVRefNum	=	fcb->ioVRefNum;
 	info->ioVolIndex=	0;
 	
 	if (!GUSIFSXGetVolInfo(&info))
 		fBlockSize = info->ioVAlBlkSiz;
 }
 // <Initialize fields of [[GUSIMacFileSocket]]>=                           
 if (!fReadShutdown && !sReadProc)
 	sReadProc = NewIOCompletionProc(reinterpret_cast<IOCompletionProcPtr>(GUSIMFReadDoneEntry));
 if (!fWriteShutdown && !sWriteProc)
 	sWriteProc = NewIOCompletionProc(reinterpret_cast<IOCompletionProcPtr>(GUSIMFWriteDoneEntry));
 if (!sWakeupProc)
 	sWakeupProc = NewIOCompletionProc(reinterpret_cast<IOCompletionProcPtr>(GUSIMFWakeupEntry));
 // The write and read parameter blocks are highly specialized and never really
 // change during the existence of a socket.                                
 //                                                                         
 // <Initialize fields of [[GUSIMacFileSocket]]>=                           
 fReadPB.ioParam.ioResult	= 0;
 fReadPB.ioParam.ioRefNum	= fFileRef;
 fReadPB.ioParam.ioPosMode	= fsAtMark;
 if (!fWriteShutdown) {
 	fWritePB.ioParam.ioCompletion	= sWriteProc;
 	fWritePB.ioParam.ioResult		= 0;
 	fWritePB.ioParam.ioRefNum		= fFileRef;
 	fWritePB.ioParam.ioPosMode		= fsAtMark;

 	GUSIMFWrite(this);
 }
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
GUSIMacFileSocket::~GUSIMacFileSocket()
{
	fsync();
	if (fFileRef)
		FSClose(fFileRef);
	GUSIMacFileDevice::Instance()->CleanupTemporaries(false);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
void GUSIMacFileSocket::SyncRead()
{
	fReadAhead    = false;
	fReadShutdown = true;
	// As soon as [[fReadShutdown]] is set, no new reads will be started, so just wait
 // for the current ones to complete.                                       
 //                                                                         
 // <Wait for pending read operations on [[GUSIMacFileSocket]] to complete>=
 if (fReadPB.ioParam.ioResult == 1) {
 	AddContext();
 	while (fReadPB.ioParam.ioResult == 1)
 		GUSIContext::Yield(kGUSIBlock);
 	RemoveContext();
 }
	// If there was data in the buffer, we need to move the file position back by
 // the amount of data.                                                     
 //                                                                         
 // <Discard data in input buffer of [[GUSIMacFileSocket]]>=                
 size_t consume = 0x7F000000;
 fInputBuffer.Consume(nil, consume);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
void GUSIMacFileSocket::SyncWrite()
{
	fOutputBuffer.Lock();
	bool saveWriteBehind = fWriteBehind;
	fWriteBehind = false;
	fOutputBuffer.Release();
	AddContext();
	while (!fWriteShutdown && fOutputBuffer.Valid())
		GUSIContext::Yield(kGUSIBlock);
	RemoveContext();
	fOutputBuffer.Lock();
	fWriteBehind = saveWriteBehind;
	fOutputBuffer.Release();	
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
ssize_t GUSIMacFileSocket::read(const GUSIScatterer & buffer)
{
	size_t	rest 	= buffer.Length();
	size_t	offset	= 0;
	
	GUSI_MESSAGE(("pos: %d len: %d\n", fPosition, rest));
	
	SyncWrite();
	if (fBeyondEOF)
		if (GUSISetPosixError(GetAsyncError()))
			return -1;
		else
			return 0;

	// As long as there has been no [[fReadShutdown]], we use the read-ahead buffer.
 //                                                                         
 // <Read from read-ahead buffer of [[GUSIMacFileSocket]] while possible>=  
 while (!fReadAhead && (!fReadShutdown || fInputBuffer.Valid()) && rest) {
 	if (fInputBuffer.Valid()) {
 		size_t len = rest;
 		fInputBuffer.Lock();
 		fInputBuffer.Consume(buffer, len, offset);
 		fPosition += len;
 		fInputBuffer.Release();
 		rest 	-= len;
 	} else {
 		if (GUSISetPosixError(GetAsyncError()))
 			return -1;
 		if (!fBlocking)
 			if (!offset) // Nothing read yet
 				return GUSISetPosixError(EWOULDBLOCK);
 			else
 				return offset;
 		AddContext();
 		while (!fReadShutdown && !fInputBuffer.Valid())
 			GUSIContext::Yield(kGUSIBlock);
 		RemoveContext();
 	}
 }
	// If there is still data to be read, read it now.                         
 //                                                                         
 // <Read directly from file into [[buffer]] if necessary>=                 
 if (!rest)
 	return buffer.Length();
 do {
 	size_t len = rest;
 	fReadPB.ioParam.ioCompletion 	= sWakeupProc;
 	fReadPB.ioParam.ioBuffer		= 
 		buffer.Count()==1 ? 
 			static_cast<Ptr>(buffer.Buffer())+offset 
 		  : static_cast<Ptr>(fInputBuffer.ProduceBuffer(len));
 	fReadPB.ioParam.ioReqCount		= len;
 	GUSI_MESSAGE(("Sync read (0x%x bytes).\n", len));
 	PBReadAsync(&fReadPB);
 	// As soon as [[fReadShutdown]] is set, no new reads will be started, so just wait
  // for the current ones to complete.                                       
  //                                                                         
  // <Wait for pending read operations on [[GUSIMacFileSocket]] to complete>=
  if (fReadPB.ioParam.ioResult == 1) {
  	AddContext();
  	while (fReadPB.ioParam.ioResult == 1)
  		GUSIContext::Yield(kGUSIBlock);
  	RemoveContext();
  }
 	GUSI_MESSAGE(("Read done (0x%x/%d).\n", fReadPB.ioParam.ioActCount, fReadPB.ioParam.ioResult));
 	switch (fReadPB.ioParam.ioResult) {
 	case eofErr:
 	case noErr:
 		rest 		-= fReadPB.ioParam.ioActCount;
 		fPosition	+= fReadPB.ioParam.ioActCount;
 		if (buffer.Count() == 1) {
 			offset		+= fReadPB.ioParam.ioActCount;
 		} else {
 			fInputBuffer.ValidBuffer(fReadPB.ioParam.ioBuffer, fReadPB.ioParam.ioActCount);
 			fInputBuffer.Consume(
 				buffer, *reinterpret_cast<size_t *>(&fReadPB.ioParam.ioActCount), offset);
 		}
 		if (rest && len == fReadPB.ioParam.ioActCount)
 			continue;
 		break;
 	default:	
 		return GUSISetMacError(fReadPB.ioParam.ioResult);
 	}
 } while (0);
	size_t done = buffer.Length() - rest;
	// We wait for the first [[read]] (and handle it synchronously) before deciding that
 // file access is sequential enough for read-ahead.                        
 //                                                                         
 // <Start read-ahead if necessary>=                                        
 if (fReadAhead && !fReadShutdown) {
 	fReadAhead 						= false;
 	fReadPB.ioParam.ioCompletion	= sReadProc;
 	GUSIMFRead(this);
 }

	return int(done);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
ssize_t GUSIMacFileSocket::write(const GUSIGatherer & buffer)
{
	if (GUSISetPosixError(GetAsyncError()))
		return -1;
	if (fWriteShutdown)	// Background error
		return GUSISetMacError(fWritePB.ioParam.ioResult);
		
	if (fAppend)
		lseek(0, SEEK_END);
	else if (fBeyondEOF) {
		// If a [[seek]] operation went beyond EOF and we try to do a [[write]] the space
  // in-between has to be filled with null bytes.                            
  //                                                                         
  // <Insert null bytes beyond EOF>=                                         
  long	targetPos = fPosition;

  for (lseek(0, SEEK_END); fPosition < targetPos; ) {
  	// <Wait for free buffer space on [[GUSIMacFileSocket]]>=                  
   if (!fWriteShutdown && !fOutputBuffer.Free()) {
   	AddContext();
   	while (!fWriteShutdown && !fOutputBuffer.Free())
   		GUSIContext::Yield(kGUSIBlock);
   	RemoveContext();
   }
   if (fWriteShutdown)
   	return GUSISetMacError(fWritePB.ioParam.ioResult);
  	size_t	size = targetPos - fPosition;
  	Ptr		p = static_cast<Ptr>(fOutputBuffer.ProduceBuffer(size));
  	memset(p, 0, size);
  	fPosition += size;
  	fOutputBuffer.ValidBuffer(p, size);
  }
	}
	
	size_t rest   = buffer.Length();
	size_t offset = 0;
	if (!fBlocking && fOutputBuffer.Free() < rest)
		return GUSISetPosixError(EWOULDBLOCK);
	while (rest) {
		size_t len = rest;
		// <Wait for free buffer space on [[GUSIMacFileSocket]]>=                  
  if (!fWriteShutdown && !fOutputBuffer.Free()) {
  	AddContext();
  	while (!fWriteShutdown && !fOutputBuffer.Free())
  		GUSIContext::Yield(kGUSIBlock);
  	RemoveContext();
  }
  if (fWriteShutdown)
  	return GUSISetMacError(fWritePB.ioParam.ioResult);
		fOutputBuffer.Lock();
		fOutputBuffer.Produce(buffer, len, offset);
		fPosition += len;
		fOutputBuffer.Release();
		rest -= len;
	}	
	
	return buffer.Length();
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
bool GUSIMacFileSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		if (*canRead = fBeyondEOF || fAsyncError || fReadAhead || fReadShutdown 
		 || fInputBuffer.Valid()
		)
			cond = true;
	if (canWrite)
		if (*canWrite = fBeyondEOF || fAsyncError || fWriteShutdown || fOutputBuffer.Free())
			cond = true;
		
	return cond;
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int GUSIMacFileSocket::fsync()
{
	SyncWrite();
	if (fWriteShutdown)
		return GUSISetMacError(fWritePB.ioParam.ioResult);
	else
		return 0;
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int GUSIMacFileSocket::fcntl(int cmd, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoFcntl(&result, cmd, arg))
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("fcntl: illegal request %d\n", cmd));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int GUSIMacFileSocket::ioctl(unsigned int request, va_list arg)
{
	int	result;
	
	if (GUSISMBlocking::DoIoctl(&result, request, arg)
	 || GUSISMInputBuffer::DoIoctl(&result, request, arg)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("ioctl: illegal request %d\n", request));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int GUSIMacFileSocket::getsockopt(int level, int optname, void *optval, socklen_t * optlen)
{
	int	result;
	
	if (GUSISMInputBuffer::DoGetSockOpt(&result, level, optname, optval, optlen)
	 || GUSISMOutputBuffer::DoGetSockOpt(&result, level, optname, optval, optlen)
	 || GUSISMAsyncError::DoGetSockOpt(&result, level, optname, optval, optlen)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("getsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int GUSIMacFileSocket::setsockopt(int level, int optname, void *optval, socklen_t optlen)
{
	int	result;
	
	if (GUSISMInputBuffer::DoSetSockOpt(&result, level, optname, optval, optlen)
	 || GUSISMOutputBuffer::DoSetSockOpt(&result, level, optname, optval, optlen)
	)
		return result;
	
	GUSI_ASSERT_CLIENT(false, ("setsockopt: illegal request %d\n", optname));
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
off_t GUSIMacFileSocket::lseek(off_t offset, int whence)
{
	if (!fReadShutdown || fInputBuffer.Valid())
		SyncRead();
	else if (!fWriteShutdown)
		SyncWrite();
		
	long	eof;
	long	pos;
	
	switch (whence) {
	case SEEK_CUR:
		pos = fPosition; 
		break;
	case SEEK_END:
		if (GUSISetMacError(GetEOF(fFileRef, &eof)))
			return -1; 
		pos = eof;
		break;
	case SEEK_SET:
		pos = 0;
		break;
	default:
		return GUSISetPosixError(EINVAL);
	}
	GUSI_MESSAGE(("pos: %d  off: %d eof: %d whence: %d\n", pos, offset, eof, whence)); 
	pos += offset;
	
	if (fPosition == pos)
		return fPosition;

	if (whence != SEEK_END && GUSISetMacError(GetEOF(fFileRef, &eof)))
		return -1; 
	if (pos > eof)
		fBeyondEOF = true;
	else if (pos < 0)
		return GUSISetPosixError(EINVAL);
	else if (GUSISetMacError(SetFPos(fFileRef, fsFromStart, pos)))
		return -1;
	else
		fBeyondEOF = false;
	
	return fPosition = pos;
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int GUSIMacFileSocket::ftruncate(off_t offset)
{	
	if (lseek(offset, SEEK_SET) == -1)
		return -1;
	if (fBeyondEOF) 
  		return lseek(0, SEEK_END) == -1 ? -1 : 0;
	else
		return GUSISetMacError(SetEOF(fFileRef, fPosition));
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
int	GUSIMacFileSocket::fstat(struct stat * buf)
{
	fsync();

	GUSIFileSpec	spec(fFileRef);
	
	if (!spec.Exists())
		return GUSISetPosixError(ENOENT);
	
	GUSIFileToken	token(spec, GUSIFileToken::kWillStat);
	
	return token.Device()->stat(token, buf);
}
// <Member functions for class [[GUSIMacFileSocket]]>=                     
bool GUSIMacFileSocket::Supports(ConfigOption config)
{
	return config == kSimpleCalls;
}
// The UPPs for the completion procedures are set up the first time a socket
// is constructed.                                                         
//                                                                         
// <Member functions for class [[GUSIMacFileSocket]]>=                     
IOCompletionUPP	GUSIMacFileSocket::sReadProc	= 0;
IOCompletionUPP	GUSIMacFileSocket::sWriteProc	= 0;
IOCompletionUPP	GUSIMacFileSocket::sWakeupProc	= 0;
// <Member functions for class [[GUSIMacDirectory]]>=                      
GUSIMacDirectory::GUSIMacDirectory(const FSSpec & spec)
 : fDir(spec)
{	
	fPos         = 1;
}
// <Member functions for class [[GUSIMacDirectory]]>=                      
dirent * GUSIMacDirectory::readdir()
{
	fDir[fPos];
	if (OSErr err = fDir.Error())
		return GUSISetMacError(err == fnfErr ? 0 : err), static_cast<dirent *>(nil);
		
	++fPos;
	const GUSICatInfo * info(fDir.CatInfo());
	fEnt.d_ino 		= info->DirInfo().ioDrDirID;	
	memcpy(fEnt.d_name, (char *)fDir->name+1, fDir->name[0]);
	fEnt.d_name[fDir->name[0]] = 0;
	
	return &fEnt;
}
// <Member functions for class [[GUSIMacDirectory]]>=                      
long GUSIMacDirectory::telldir()
{
	return fPos;
}
// <Member functions for class [[GUSIMacDirectory]]>=                      
void GUSIMacDirectory::seekdir(long pos)
{
	fPos = pos;
}
// <Member functions for class [[GUSIMacDirectory]]>=                      
void GUSIMacDirectory::rewinddir()
{
	fPos = 1;
}

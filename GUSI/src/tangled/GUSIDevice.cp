// <GUSIDevice.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSIDevice.h"
#include "GUSIMacFile.h"
#include "GUSINull.h"
#include "GUSIDiag.h"

#include <fcntl.h>
#include <utility>

GUSI_USING_STD_NAMESPACE

// \section{Implementation of [[GUSIFileToken]]}                           
//                                                                         
// Identifying a name starting with "Dev:" as a file or a device is a dangerous job.
// Currently, there is no possibility to entirely disable device interpretation, so 
// we're using the following heuristics in order to minimize conflicts with real file
// names:                                                                  
//                                                                         
// \begin{itemize}                                                         
// \item Any name corresponding to an existing file is a file              
// \item Any name not recognized by any device domain is a file            
// \end{itemize}                                                           
//                                                                         
// We need many case insensitive comparisons. Since we always compare to fixed
// strings withing the ASCII set, we can do a cheap implementation.        
//                                                                         
// <Member functions for class [[GUSIFileToken]]>=                         
bool GUSIFileToken::StrFragEqual(const char * name, const char * frag)
{
	do {
		if ((*name++ | 0x20) != *frag++)
			return false;
	} while (*frag);
	
	return true;
}

GUSIFileToken::GUSIFileToken(const char * path, Request request, bool useAlias)
	: fPath(path), GUSIFileSpec(path, useAlias), fDevice(nil), fRequest(request), fIsFile(false)
{
	if (!StrFragEqual(path, "dev:") || (!Error() && Exists()))
		goto treatAsFile;

	if (fDevice = GUSIDeviceRegistry::Instance()->Lookup(*this))
		return;

treatAsFile:	
	fIsFile = true;
	fDevice = GUSIDeviceRegistry::Instance()->Lookup(*this);
}
// Some names need to be detected frequently. We've already established that the
// name starts with [["dev:"]].                                            
//                                                                         
// <Member functions for class [[GUSIFileToken]]>=                         
GUSIFileToken::StdStream GUSIFileToken::StrStdStream(const char * name)
{
	if (StrFragEqual(name+4, "console") && !name[11])
		return kConsole;
	if (StrFragEqual(name+4, "std"))
		if (StrFragEqual(name+7, "in") && !name[9])
			return kStdin;
		else if (StrFragEqual(name+7, "out") && !name[10])
			return kStdout;
		else if (StrFragEqual(name+7, "err") && !name[10])
			return kStderr;
	return kNoStdStream;
}
// The other constructors of [[GUSIFileToken]] are always called for real files
// and thus present no ambiguities.                                        
//                                                                         
// <Member functions for class [[GUSIFileToken]]>=                         
GUSIFileToken::GUSIFileToken(const GUSIFileSpec & spec, Request request)
	: fPath(nil), GUSIFileSpec(spec), fDevice(nil), fRequest(request), fIsFile(true)
{
	fDevice = GUSIDeviceRegistry::Instance()->Lookup(*this);
}

GUSIFileToken::GUSIFileToken(short fRefNum, Request request)
	: fPath(nil), GUSIFileSpec(fRefNum), fDevice(nil), fRequest(request), fIsFile(true)
{
	fDevice = GUSIDeviceRegistry::Instance()->Lookup(*this);
}
// \section{Implementation of [[GUSIDevice]]}                              
//                                                                         
// By default, a device wants no files.                                    
//                                                                         
// <Member functions for class [[GUSIDevice]]>=                            
bool	GUSIDevice::Want(GUSIFileToken &)
{
	return false;
}
// <Member functions for class [[GUSIDevice]]>=                            
GUSISocket * GUSIDevice::open(GUSIFileToken &, int)
// <This should not happen, fail assertion and return [[nil]]>=            
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	GUSISetPosixError(EOPNOTSUPP);
	
	return nil;
}

int GUSIDevice::remove(GUSIFileToken &)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::rename(GUSIFileToken &, const char *)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::stat(GUSIFileToken &, struct stat *)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::chmod(GUSIFileToken &, mode_t)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::utime(GUSIFileToken &, const utimbuf *)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::access(GUSIFileToken &, int)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::mkdir(GUSIFileToken &)
// <This should not happen, fail assertion and return [[nil]]>=            
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	GUSISetPosixError(EOPNOTSUPP);
	
	return nil;
}

int GUSIDevice::rmdir(GUSIFileToken &)
// <This should not happen, fail assertion and return [[nil]]>=            
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	GUSISetPosixError(EOPNOTSUPP);
	
	return nil;
}

GUSIDirectory * GUSIDevice::opendir(GUSIFileToken &)
// <This should not happen, fail assertion and return [[nil]]>=            
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	GUSISetPosixError(EOPNOTSUPP);
	
	return nil;
}

int GUSIDevice::symlink(GUSIFileToken &, const char *)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::readlink(GUSIFileToken &, char *, int)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::fgetfileinfo(GUSIFileToken &, OSType *, OSType *)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::fsetfileinfo(GUSIFileToken &, OSType, OSType)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}

int GUSIDevice::faccess(GUSIFileToken &, unsigned *, void *)
// Since individual devices are free to opt out of every operation, we have to 
// implement them all as empty functions here. Calling any of these functions is
// an internal error.                                                      
//                                                                         
// <This should not happen, fail assertion and return [[EOPNOTSUPP]]>=     
{
	GUSI_SASSERT_INTERNAL(false, "Undefined device operation");
	
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
GUSIDeviceRegistry *	GUSIDeviceRegistry::sInstance;
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
GUSIDeviceRegistry::GUSIDeviceRegistry()
	: fFirstDevice(nil)
{
	AddDevice(GUSIMacFileDevice::Instance());
}
// [[AddDevice]] and [[RemoveDevice]] add and remove a [[GUSIDevice]].     
//                                                                         
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
void GUSIDeviceRegistry::AddDevice(GUSIDevice * device)
{
	device->fNextDevice = fFirstDevice;
	fFirstDevice		= device;
}

void GUSIDeviceRegistry::RemoveDevice(GUSIDevice * device)
{
	if (fFirstDevice == device)
		fFirstDevice = device->fNextDevice;
	else
		for (iterator dev = begin(); dev != end(); ++dev)
			if (dev->fNextDevice == device) {
				dev->fNextDevice = device->fNextDevice;
				
				break;
			}
}
// To look up, we iterate through all devices.                             
//                                                                         
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
GUSIDevice * GUSIDeviceRegistry::Lookup(GUSIFileToken & file)
{
	for (iterator dev = begin(); dev != end(); ++dev)
		if (dev->Want(file))
			return &(*dev);
	
	return static_cast<GUSIDevice *>(nil);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
GUSISocket * GUSIDeviceRegistry::open(const char * path, int flags)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillOpen, (flags & O_ALIAS) != 0);
	
	if (GUSIDevice * device = file.Device())
		return device->open(file, flags);
	else
		return GUSISetPosixError(ENOENT), static_cast<GUSISocket *>(nil);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::remove(const char * path)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillRemove, true);
	
	if (GUSIDevice * device = file.Device())
		return device->remove(file);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::rename(const char * oldname, const char * newname)
{
	GUSIFileToken	oldfile(oldname, GUSIFileToken::kWillRename, true);
	
	if (GUSIDevice * device = oldfile.Device())
		return device->rename(oldfile, newname);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::stat(const char * path, struct stat * buf, bool useAlias)
{
	GUSIFileToken 	file(path, GUSIFileToken::kWillStat, useAlias);
	
	if (GUSIDevice * device = file.Device())
		return device->stat(file, buf);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::chmod(const char * path, mode_t mode)
{
	GUSIFileToken 	file(path, GUSIFileToken::kWillChmod, false);
	
	if (GUSIDevice * device = file.Device())
		return device->chmod(file, mode);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::utime(const char * path, const utimbuf * times)
{
	GUSIFileToken 	file(path, GUSIFileToken::kWillUtime, false);
	
	if (GUSIDevice * device = file.Device())
		return device->utime(file, times);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::access(const char * path, int mode)
{
	GUSIFileToken 	file(path, GUSIFileToken::kWillAccess, false);
	
	if (GUSIDevice * device = file.Device())
		return device->access(file, mode);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::mkdir(const char * path)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillMkdir);
	
	if (GUSIDevice * device = file.Device())
		return device->mkdir(file);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::rmdir(const char * path)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillRmdir);
	
	if (GUSIDevice * device = file.Device())
		return device->rmdir(file);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
GUSIDirectory * GUSIDeviceRegistry::opendir(const char * path)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillOpendir);
	
	if (GUSIDevice * device = file.Device())
		return device->opendir(file);
	else
		return GUSISetPosixError(ENOENT), static_cast<GUSIDirectory *>(nil);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::symlink(const char * target, const char * newlink)
{
	GUSIFileToken	file(target, GUSIFileToken::kWillSymlink, true);
	
	if (GUSIDevice * device = file.Device())
		return device->symlink(file, newlink);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::readlink(const char * path, char * buf, int bufsize)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillReadlink, true);
	
	if (GUSIDevice * device = file.Device())
		return device->readlink(file, buf, bufsize);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::fgetfileinfo(const char * path, OSType * creator, OSType * type)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillGetfileinfo, true);
	
	if (GUSIDevice * device = file.Device())
		return device->fgetfileinfo(file, creator, type);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::fsetfileinfo(const char * path, OSType creator, OSType type)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillSetfileinfo, true);
	
	if (GUSIDevice * device = file.Device())
		return device->fsetfileinfo(file, creator, type);
	else
		return GUSISetPosixError(ENOENT);
}
// <Member functions for class [[GUSIDeviceRegistry]]>=                    
int GUSIDeviceRegistry::faccess(const char * path, unsigned * cmd, void * arg)
{
	GUSIFileToken	file(path, GUSIFileToken::kWillFaccess, true);
	
	if (GUSIDevice * device = file.Device())
		return device->faccess(file, cmd, arg);
	else
		return GUSISetPosixError(ENOENT);
}

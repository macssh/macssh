// <GUSIFileSpec.cp>=                                                      
#include "GUSIInternal.h"
#include "GUSIFileSpec.h"
#include "GUSIFSWrappers.h"

#include <PLStringFuncs.h>
#include <Errors.h>
#include <TextUtils.h>
#include <Resources.h>
#include <Memory.h>
#include <Aliases.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

// First, we test whether the path represents an encoded [FSSpec]]. This is potentially
// ambiguous, but no sane person would start a disk name with DC1. The [[ReadHex]] function,
// which only works on big endian machines, reads a specified number of hex digits.
//                                                                         
// <Auxiliary functions for class [[GUSIFileSpec]]>=                       
bool ReadHex(const char * path, int bytes, char * result) 
{
	char hexbyte[3];
	hexbyte[2] = 0;
	while (bytes--) {
		hexbyte[0] = *path++; hexbyte[1] = *path++;
		if (isxdigit(hexbyte[0]) && isxdigit(hexbyte[1]))
			*result++ = (char) strtol(hexbyte, nil, 16);
		else
			return false;
	}
	return true;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
char *	GUSIFileSpec::sScratch;
long	GUSIFileSpec::sScratchSize;
// <Member functions for class [[GUSIFileSpec]]>=                          
char * GUSIFileSpec::CScratch(bool extend)
{
	if (extend) {
		char * newScratch = NewPtr(sScratchSize + 64);
		if (!newScratch)
			return nil;
		BlockMoveData(sScratch, newScratch+64, sScratchSize);
		sScratchSize += 64;
		DisposePtr(sScratch);
		
		return sScratch = newScratch;
	} else if (!sScratchSize)
		if (sScratch = NewPtr(256))
			sScratchSize = 256;

	return sScratch;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(const GUSIFileSpec & spec)
	: fValidInfo(false), fSpec(spec.fSpec), fError(spec.fError)
{
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(const FSSpec & spec, bool useAlias)							
	: fValidInfo(false), fSpec(spec), fError(noErr)
{
	if (!useAlias)
		Resolve();
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(short vRefNum, long parID, ConstStr31Param name, bool useAlias)
	: fValidInfo(false)
{
	OSErr	err;
	
	if ((err = FSMakeFSSpec(vRefNum, parID, name, &fSpec)) && (err != fnfErr)) {
		fSpec.vRefNum	=	vRefNum;
		fSpec.parID		=	parID;
		memcpy(fSpec.name, name, *name+1);
	}
	fError	= noErr;
	
	if (!useAlias)
		Resolve();	
		
	if (EqualString(fSpec.name, name, false, true))
		memcpy(fSpec.name, name, *name+1);		
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(short wd, ConstStr31Param name, bool useAlias)
	: fValidInfo(false)
{
	if ((fError = FSMakeFSSpec(wd, 0, name, &fSpec)) && (fError != fnfErr))
		return;
	
	if (!useAlias)
		Resolve();
		
	if (EqualString(fSpec.name, name, false, true))
		memcpy(fSpec.name, name, *name+1);		
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(OSType object, short vol)
	: fValidInfo(false)
{
	fError = FindFolder(vol, object, true, &fSpec.vRefNum, &fSpec.parID);
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(short fRefNum)
	: fValidInfo(false)
{
	GUSIIOPBWrapper<FCBPBRec>	fcb;

	fcb->ioNamePtr	= 	fSpec.name;
	fcb->ioRefNum	=	fRefNum;
	fcb->ioFCBIndx	= 	0;

	if (fError = GUSIFSGetFCBInfo(&fcb))
		return;

	fSpec.vRefNum 	=	fcb->ioFCBVRefNum;
	fSpec.parID		=	fcb->ioFCBParID;
}
// The pathname constructor is by far the most complex constructor. This code is also
// probably not portable to Rhapsody, whatever and whenever that will be.  
//                                                                         
// To minimize the number of [[CatInfo]] calls necessary, the [[fullSpec]] variable
// reflects whether the entire [[FSSpec]] under construction or only the [[vRefNum]]
// and [[parID]] are valid.                                                
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec::GUSIFileSpec(const char * path, bool useAlias)
	: fValidInfo(false)
{
	const char *	nextPath;
	bool			fullSpec	= 	false;
	
	fSpec.vRefNum 	= 0;
	fSpec.parID		= 0;
	
	// <Try decoding the path as an encoded [[FSSpec]]>=                       
 if (*path == 0x11 && path[13] == ':')			// Magic DC1 character
 	if (
 		!ReadHex(path+1, 2, (char *)&fSpec.vRefNum) || !ReadHex(path+5, 4, (char *)&fSpec.parID)
 	) {
 		fSpec.vRefNum = 0;
 		fSpec.parID   = 0;
 	} else
 		path += 13;
	// <Try converting the path with [[FSMakeFSSpec]] and [[return]]>=         
 if (!fSpec.vRefNum && !fSpec.parID)
 	GetDefaultDirectory();
 int 		pathLen = (int)strlen(path);
 StringPtr	ppath  	= PScratch();
 if (pathLen < 256 && ppath) {
 	memcpy(ppath+1, path, ppath[0] = pathLen);
 	
 	short 	saveVRefNum	=	fSpec.vRefNum;
 	long	saveParID	=	fSpec.parID;
 	
 	switch (fError = FSMakeFSSpec(fSpec.vRefNum, fSpec.parID, ppath, &fSpec)) {
 	case fnfErr:
 		fError = noErr;
 		
 		return;
 	case noErr:
 		if (!useAlias)
 			Resolve();

 		while ((nextPath = strchr(path, ':')) && nextPath[1])
 			path = nextPath+1;
 		memcpy(ppath+1, path, ppath[0] = strlen(path) - (nextPath != 0));
 		if (EqualString(fSpec.name, ppath, false, true))
 			memcpy(fSpec.name, ppath, ppath[0]+1);		
 		
 		return;
 	default:
 		fSpec.vRefNum	=	saveVRefNum;
 		fSpec.parID		= 	saveParID;
 		break;
 	}
 }
	// For relative paths, we now skip a leading colon. For absolute paths, we get 
 // the volume information. [[fullSpec]] is true for absolute paths, but not for 
 // relative paths.                                                         
 //                                                                         
 // <Determine the starting directory of the path>=                         
 if (path[0] == ':') {
 	++path;
 } else if (nextPath = strchr(path, ':')) {
 	if (nextPath - (char *) path > 62) {
 		fError = bdNamErr;
 		
 		return;
 	}

 	memcpy(fSpec.name+1, (char *) path, fSpec.name[0] = nextPath - path);
 		
 	if (GetVolume())
 		return;
 	
 	path 		= nextPath + 1;
 	fullSpec	= true;
 }

	fError = noErr;
	while (*path && !fError) {
		if (*path == ':')	{
			// Going upwards is a bit more complicated than might seem necessary at first, because
   // if [["a:b"]] is an alias pointing to [["c:d:e:"]], we want a:b::"]] to be the same as 
   // [["c:d:"]].                                                             
   //                                                                         
   // <Walk directories upwards>=                                             
   if (!fullSpec)
   	--(*this);
   if (!fError)
   	Resolve();
   fullSpec = false;
   while (!fError && *++path == ':')
   	--(*this);
		} else {
			if (nextPath = strchr(path, ':')) {
				AddPathComponent(path, nextPath-path, fullSpec);
				fullSpec 	= true;
				path 		= nextPath+1;
			} else {
				AddPathComponent(path, strlen(path), fullSpec);
				fullSpec 	= true;
				break;
			}
		}
	}
	if (!fError) {
		if (!fullSpec)
			--(*this);
		if (!useAlias)
			Resolve();
	} else if (*path && fError == fnfErr) {
		fError = dirNFErr;
	}
}
// If [[name]] is used, we have to make sure that it ends with a colon.    
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
OSErr GUSIFileSpec::GetVolume(short index)
{
	fValidInfo	=	false;
	if (fSpec.name[0] || index>=0) {
		GUSIIOPBWrapper<ParamBlockRec>	vol;
		
		vol->volumeParam.ioVRefNum	=	fSpec.vRefNum;
		vol->volumeParam.ioNamePtr	=	fSpec.name;
		vol->volumeParam.ioVolIndex	=	index;
		
		if (index < 0 && fSpec.name[fSpec.name[0]] != ':')
			fSpec.name[++fSpec.name[0]] = ':';
	
		if (fError = GUSIFSGetVInfo(&vol))
			return fError;
		
		fSpec.vRefNum	=	vol->volumeParam.ioVRefNum;
	} else {
		fError 			= 	noErr;
		fSpec.vRefNum	= 	0;
	}
	fSpec.parID			=	fsRtParID;
	
	return fError;
}
// [[operator--]] replaces file specifications with their parent directory, volumes with
// the root pseudo directory.                                              
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec & GUSIFileSpec::operator--()
{
	if (fSpec.parID == fsRtParID) {
		fSpec.vRefNum 	= ROOT_MAGIC_COOKIE;
		fSpec.parID   	= 0;
		fSpec.name[0] 	= 0;
		fError			= noErr;
		fValidInfo		= false;
	} else 
		DirInfo();
	
	return *this;
}
// [[operator++]] walks down one path component.                           
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec & GUSIFileSpec::operator++()
{
	if (!fSpec.parID && fSpec.vRefNum == ROOT_MAGIC_COOKIE) {
		fSpec.vRefNum	=	0;
		fSpec.parID		=	fsRtParID;
		fSpec.name[0]	=	0;
		fValidInfo		= 	false;
		
		goto punt;
	} 
			
   	if (!CatInfo())
		goto punt;
	if (fInfo->IsAlias())
		if (Resolve() || !CatInfo())
			goto punt;
	if (fInfo->IsFile()) {
		fError = afpObjectTypeErr;
		
		goto punt;
	}
	
	fSpec.parID		= fInfo->DirInfo().ioDrDirID;
	fSpec.name[0] 	= 0;
	fValidInfo		= false;

punt:	
	return *this;
}
// [AddPathComponent]] is the basic operation for descending a directory hierarchy.
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec & GUSIFileSpec::AddPathComponent(const char * name, int length, bool fullSpec)
{
	if (length > 63) {
		fError = bdNamErr;
		
		goto punt;
	}

	if (fullSpec) 
		if (!++(*this))
			goto punt;
	
	memcpy(fSpec.name+1, name, fSpec.name[0] = length);
	fValidInfo = false;
		
	if (fSpec.parID == fsRtParID)
		GetVolume();

punt:	
	return *this;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec operator+(const FSSpec & spec, ConstStr31Param name)
{
	GUSIFileSpec	s(spec);
	
	return s += name;
}

GUSIFileSpec operator+(const FSSpec & spec, const char * name)
{
	GUSIFileSpec	s(spec);
	
	return s += name;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSIFileSpec & GUSIFileSpec::operator[](short index)
{
	if (fSpec.parID == fsRtParID) 
		GetVolume(index);
	else 
		CatInfo(index);
	
	return *this;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
void GUSIFileSpec::SetVRef(short vref)
{
	fSpec.vRefNum 	= vref;
	fValidInfo		= false;
}

void GUSIFileSpec::SetParID(long parid)
{
	fSpec.parID	 	= parid;
	fValidInfo		= false;
}

void GUSIFileSpec::SetName(ConstStr63Param name)
{
	PLstrcpy(fSpec.name, name);
	fValidInfo	= false;
}

void GUSIFileSpec::SetName(const char * name)
{
	memcpy(fSpec.name+1, name, fSpec.name[0] = strlen(name));
	fValidInfo	= false;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
OSErr GUSIFileSpec::TouchFolder()
{
	GUSIFileSpec	folder(*this, true);
	
	if (!folder.DirInfo())
		return fError = folder.Error();
	
	GetDateTime(&folder.fInfo->DirInfo().ioDrMdDat);
	folder.fInfo->DirInfo().ioDrDirID = folder.fInfo->DirInfo().ioDrParID;
	
	return fError = GUSIFSSetCatInfo(&folder.fInfo);
}
// The sort of information obtained depends on the [[index]] parameter.    
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
const GUSICatInfo * GUSIFileSpec::CatInfo(short index)
{
	if (fValidInfo && !index)	// Valid already
		return &fInfo.fPB;
		
	fInfo->DirInfo().ioVRefNum 		= fSpec.vRefNum;
	fInfo->DirInfo().ioDrDirID 		= fSpec.parID;
	fInfo->DirInfo().ioNamePtr 		= (StringPtr) fSpec.name;
	fInfo->DirInfo().ioFDirIndex 	= index;
	fInfo->DirInfo().ioACUser 		= 0;
		
	fValidInfo = !(fError = GUSIFSGetCatInfo(&fInfo)) && index>=0;
		
	return fError ? nil : &fInfo.fPB;
}
// We start the path getting functions with the full path. The full and relative
// path functions are quite similar, iterating a file specification [[current]] 
// upward, accumulating the path in [[path]].                              
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
char * GUSIFileSpec::FullPath() const
{
	char * 					path = CScratch();
	GUSIFileSpec 			current(*this);
	const GUSICatInfo * 	info 		= current.CatInfo();
	bool					directory	= info && !info->IsFile();

	if (!path) 
		return nil;
	*(path += sScratchSize-1) = 0;
	
	for (;;) {
		if (PrependPathComponent(path, current.fSpec.name, directory))
			return nil;
		directory = current.fSpec.name[0] != 0;
		if (current.fSpec.parID == fsRtParID)
			return path;
		if (!--current)
			return nil;
	}
}
// <Member functions for class [[GUSIFileSpec]]>=                          
OSErr GUSIFileSpec::PrependPathComponent(char *&path, ConstStr63Param component, bool colon) const
{
	if (colon)
		*--path = ':';
	if (path-sScratch < *component+1) {
		long length = sScratch+sScratchSize-path;
		if (!CScratch(true))
			return GUSI_MUTABLE(GUSIFileSpec, fError) = -108;
		path = sScratch+sScratchSize-length;
	}
	memcpy(path -= *component, component+1, *component);
	return noErr;
}
// [[RelativePath]] works similarly, but has to compare with the stop directory
// at each step. [[GetVolume]] is called to translate all drive numbers to real
// volume numbers.                                                         
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
char *	GUSIFileSpec::RelativePath(const FSSpec & dir) const
{
	GUSIFileSpec current(dir);
	if (current.fSpec.name[0])
		++current;
	long	relDirID= current.fSpec.parID;
	if (current.GetVolume(0))
		return FullPath();
	short	relVRef = current.fSpec.vRefNum;
	current = *this;
	if (current.GetVolume(0) || current.fSpec.vRefNum != relVRef)
		return FullPath();

	current = *this;
		
	char * 					path 		= CScratch();
	const GUSICatInfo * 	info 		= current.CatInfo();
	bool					directory	= info && !info->IsFile();

	if (!path) 
		return nil;
	if (directory && info->DirInfo().ioDrDirID == relDirID)
		return strcpy(path, ":");
	*(path += sScratchSize-1) = 0;
	
	for (;;) {
		if (PrependPathComponent(path, current.fSpec.name, directory))
			return nil;
		if (current.fSpec.parID == relDirID) {
			if (directory)
				*--path = ':';
			return path;
		}
		directory = current.fSpec.name[0] != 0;
		if (current.fSpec.parID == fsRtParID)
			return path;
			if (!--current)
			return nil;
	}
}
// <Member functions for class [[GUSIFileSpec]]>=                          
char *	GUSIFileSpec::RelativePath() const
{
	GUSIFileSpec	here;
	here.GetDefaultDirectory();
	return RelativePath(here);
}
// Encoding is simple. Note the use of the [["%#s"]] Metrowerks specific extension.
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
char * GUSIFileSpec::EncodedPath() const
{
	if (!CScratch())
		return nil;

	GUSI_sprintf(sScratch, "\021%04hX%08X:%#s", fSpec.vRefNum, fSpec.parID, fSpec.name);
	
	return sScratch;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
OSErr GUSIFileSpec::Resolve(bool gently)
{
	const GUSICatInfo * info = CatInfo();
	
	if (!info || (!info->IsAlias() && (fError = resFNotFound)))
		if (gently)
			return fError = noErr;
		else
			return fError;

	Boolean		isFolder;
	Boolean		wasAlias;
	
	fValidInfo	=	false;
	
	return fError = ResolveAliasFile(&fSpec, true, &isFolder, &wasAlias);
}
// Getting the target of an alias without resolving it is quite tricky. We have to guess
// whether the target is a file or a directory.                            
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
char * GUSIFileSpec::AliasPath() const
{
	const GUSICatInfo * info = CatInfo();
	if (!info || (!info->IsAlias() && (GUSI_MUTABLE(GUSIFileSpec, fError) = resFNotFound)))
		return nil;

	char * 			path 		= CScratch();
	if (!path) 
		return nil;
	*(path += sScratchSize-1) = 0;

	AliasHandle	alias;
	// <Get the [[AliasHandle]] for the alias file and detach it>=             
 short	oldRes	=	CurResFile();
 short	res 	= 	FSpOpenResFile(&fSpec, fsRdPerm);
 if (res == -1) {
 	GUSI_MUTABLE(GUSIFileSpec, fError) 	= 	ResError();
 	alias	=	nil;
 } else {
 	if (alias = (AliasHandle) Get1Resource('alis', 0))
 		DetachResource((Handle) alias);
 	else
 		GUSI_MUTABLE(GUSIFileSpec, fError) = ResError();
 	
 	CloseResFile(res);
 }
 UseResFile(oldRes);

 if (!alias)
 	return nil;
	bool directory;
	// We have to guess whether the alias points at a file or a directory, but if the alias
 // was constructed by GUSI or by the Finder, this guess should be correct (though not
 // necessarily up to date).                                                
 //                                                                         
 // <Find out if alias points at a file or a directory>=                    
 directory = false;
 if (info->FInfo().fdCreator == 'MACS') 
 	switch (info->FInfo().fdType) {
 	case 'srvr':
 	case 'fadr':
 	case 'faet':
 	case 'hdsk':
 	case 'famn':
 	case 'fash':
 	case 'fdrp':
 		directory = true;
 	}
	Str63 component;
	
	/*
		Build path from target up to root.  Separate with colons.
	*/
	for (short index = 0; 
		!(GUSI_MUTABLE(GUSIFileSpec, fError) = GetAliasInfo(alias, index, component));
		++index
	)
		if (!*component || PrependPathComponent(path, component, directory)) 
			break;
		else
			directory = true;
	
	if (!fError 
	 && !(GUSI_MUTABLE(GUSIFileSpec, fError) = GetAliasInfo(alias, asiVolumeName, component))
	)
		PrependPathComponent(path, component, true);

	DisposeHandle((Handle) alias);
	
	return fError ? nil : path;
}
// Comparisons are again a bit tricky because of the volume numbers.       
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
bool operator==(const GUSIFileSpec & one, const GUSIFileSpec & other)
{
	if (one.fSpec.parID != other.fSpec.parID || !EqualString(one.fSpec.name, other.fSpec.name, false, true))
		return false;
	if (one.fSpec.vRefNum == other.fSpec.vRefNum)
		return true;
	GUSIFileSpec current;
	current = one;
	current.GetVolume(0);
	short vRef1 = current.fSpec.vRefNum;
	current = other;
	current.GetVolume(0);
	short vRef2 = current.fSpec.vRefNum;
	
	return vRef1 == vRef2;
}
// <Member functions for class [[GUSIFileSpec]]>=                          
bool	GUSIFileSpec::IsParentOf(const GUSIFileSpec & other) const
{
	for (GUSIFileSpec current(other); !(--current).Error();)
		if (current == *this)
			return true;
	
	return false;
}
// The constructors for [[GUSITempFileSpec]] differ in the base            
// constructors they call.                                                 
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSITempFileSpec::GUSITempFileSpec(short vRefNum)
 : GUSIFileSpec(kTemporaryFolderType, vRefNum)
{
	TempName();
}
GUSITempFileSpec::GUSITempFileSpec(short vRefNum, long parID)
{
	SetVRef(vRefNum);
	SetParID(parID);
	
	TempName();
}
// Optionally, a base name can be specified for a temporary name.          
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
GUSITempFileSpec::GUSITempFileSpec(ConstStr63Param basename)
 : GUSIFileSpec(kTemporaryFolderType, kOnSystemDisk)
{
	TempName(basename);
}
GUSITempFileSpec::GUSITempFileSpec(short vRefNum, ConstStr31Param basename)
 : GUSIFileSpec(kTemporaryFolderType, vRefNum)
{
	TempName(basename);
}
GUSITempFileSpec::GUSITempFileSpec(short vRefNum, long parID, ConstStr31Param basename)
{
	SetVRef(vRefNum);
	SetParID(parID);
	
	TempName(basename);
}
// The names are searched by [[TempName]]. To avoid excessive searching, each 
// search starts at a new location.                                        
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
int GUSITempFileSpec::sID = 0;

void GUSITempFileSpec::TempName()
{
	for (;;) {
		char	name[8];

		GUSI_sprintf(name, "tmp%04d", sID++);		
		SetName(name);
		
		sID 		%= 10000;
		fValidInfo   = false;
		if (!Exists()) {
			if (fError == fnfErr)
				fError = noErr;
			return;
		}
	}
}
// The search with an existing base name is a slight variation on the above.
//                                                                         
// <Member functions for class [[GUSIFileSpec]]>=                          
void GUSITempFileSpec::TempName(ConstStr31Param basename)
{
	for (int id = 0;;++id) {
		Str32	name;
		int 	len = 2;

		if (id < 10)
			;
		else if (id < 100)
			len = 3;
		else if (id < 1000)
			len = 4;
		else if (id < 10000)
			len = 5;
		else {
			fError = fnfErr;
			return;
		}
		
		// I think it looks a bit prettier if the number is stuck before the first dot, instead of at the end.
  //                                                                         
  // <Insert [[id]] at the appropriate place into [[basename]]>=             
  if (!id) {
  	memcpy(name, basename, *basename+1);
  } else if (*basename + len > 31) {
  	name[0] = 30;
  	memcpy(name+1, basename+1, 31-len);
  	GUSI_sprintf(reinterpret_cast<char *>(name+32-len), "%d", id);
  } else {
  	name[0] = *basename + len;
  	unsigned char * period = reinterpret_cast<unsigned char *>(memchr(basename+1, '.', *basename));
  	int prelen = period ? (period-basename)-1 : *basename;
  	memcpy(name+1, basename+1, prelen);
  	GUSI_sprintf(reinterpret_cast<char *>(name+1+prelen), ".%d", id);
  	memcpy(name+1+prelen+len, period, *basename-prelen);
  }
		SetName(name);
		
		fValidInfo   = false;
		if (!Exists()) {
			if (fError == fnfErr)
				fError = noErr;
			return;
		}
	}
}
// \section{Implementation of C APIs to [[GUSIFileSpec]]}                  
//                                                                         
// The C API is fairly easy to implement. We move the common parts of the [[FSSpec]] constructors
// into an auxiliary procedure.                                            
//                                                                         
// <C API functions for [[GUSIFileSpec]]>=                                 
static OSErr GUSIFileSpec2FSp(const GUSIFileSpec & spec, FSSpec * desc)
{
	if (!spec.Error())
		*desc = spec;
	
	return spec.Error();
}
// The constructors now turn into one liners.                              
//                                                                         
// <C API functions for [[GUSIFileSpec]]>=                                 
OSErr GUSIFRefNum2FSp(short fRefNum, FSSpec * desc)	
	{ return GUSIFileSpec2FSp(GUSIFileSpec(fRefNum), desc); 		}
OSErr GUSIWD2FSp(short wd, ConstStr31Param name, FSSpec * desc)
	{ return GUSIFileSpec2FSp(GUSIFileSpec(wd, name, true), desc); 	}
OSErr GUSIPath2FSp(const char * path, FSSpec * desc)
	{ return GUSIFileSpec2FSp(GUSIFileSpec(path, true), desc); 		}
OSErr GUSISpecial2FSp(OSType object, short vol, FSSpec * desc)
	{ return GUSIFileSpec2FSp(GUSIFileSpec(object, vol), desc); 	}
// The temporary constructor is only a little bit trickier.                
//                                                                         
// <C API functions for [[GUSIFileSpec]]>=                                 
OSErr GUSIMakeTempFSp(short vol, long dirID, FSSpec * desc)
	{ return GUSIFileSpec2FSp((dirID ? GUSITempFileSpec(vol, dirID) : GUSITempFileSpec(vol)), desc); 		} 
// The manipulators look a bit like constructors.                          
//                                                                         
//                                                                         
// <C API functions for [[GUSIFileSpec]]>=                                 
OSErr GUSIFSpUp(FSSpec * desc)
	{ return GUSIFileSpec2FSp(--GUSIFileSpec(*desc, true), desc); 			}
OSErr GUSIFSpDown(FSSpec * desc, ConstStr31Param name)
	{ return GUSIFileSpec2FSp(*desc+name, desc); 							}			
OSErr GUSIFSpIndex(FSSpec * desc, short n)
	{ GUSIFileSpec spec(*desc);  return GUSIFileSpec2FSp(spec[n], desc);	}
OSErr GUSIFSpResolve(FSSpec * desc)
	{ return GUSIFileSpec2FSp(GUSIFileSpec(*desc), desc); 					}
// The deconstructors are two liners anyway.                               
//                                                                         
// <C API functions for [[GUSIFileSpec]]>=                                 
char * GUSIFSp2FullPath(const FSSpec * desc)
{
	GUSIFileSpec spec(*desc, true);
	
	return spec.FullPath();
}

char * GUSIFSp2RelPath(const FSSpec * desc)
{
	GUSIFileSpec spec(*desc, true);
	
	return spec.RelativePath();
}

char * GUSIFSp2DirRelPath(const FSSpec * desc, const FSSpec * dir)
{
	GUSIFileSpec spec(*desc, true);
	
	return spec.RelativePath(*dir);
}

char * GUSIFSp2Encoding(const FSSpec * desc)
{
	GUSIFileSpec spec(*desc, true);
	
	return spec.EncodedPath();
}
// <C API functions for [[GUSIFileSpec]]>=                                 
OSErr GUSIFSpTouchFolder(const FSSpec * desc)
{
	GUSIFileSpec spec(*desc);
	
	return spec.TouchFolder();
}

OSErr GUSIFSpGetCatInfo(FSSpec * desc, CInfoPBRec * info)
{
	GUSIFileSpec 		spec(*desc);
	const GUSICatInfo *	gci = spec.CatInfo();
	
	if (gci) {
		*info						= gci->Info();
		*desc 						= spec;
		info->hFileInfo.ioNamePtr 	= desc->name;
	}
		
	return spec.Error();
}

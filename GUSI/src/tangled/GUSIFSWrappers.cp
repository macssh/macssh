// <GUSIFSWrappers.cp>=                                                    
#include "GUSIInternal.h"
#include "GUSIFSWrappers.h"
#include "GUSIContext.h"

#include <PLStringFuncs.h>
#include <Devices.h>
#include <Script.h>
#include <StringCompare.h>

// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSGetCatInfo(GUSIIOPBWrapper<GUSICatInfo> * info)
{
	info->StartIO();
	PBGetCatInfoAsync(&info->fPB.Info());
	return info->FinishIO();
}

OSErr GUSIFSSetCatInfo(GUSIIOPBWrapper<GUSICatInfo> * info)
{
	info->StartIO();
	PBSetCatInfoAsync(&info->fPB.Info());
	return info->FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSGetFCBInfo(GUSIIOPBWrapper<FCBPBRec> * fcb)
{
	fcb->StartIO();
	PBGetFCBInfoAsync(&fcb->fPB);
	return fcb->FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSGetVInfo(GUSIIOPBWrapper<ParamBlockRec> * pb)
{
	pb->StartIO();
	PBGetVInfoAsync(&pb->fPB);
	return pb->FinishIO();
}
OSErr GUSIFSHGetVInfo(GUSIIOPBWrapper<HParamBlockRec> * pb)
{
	pb->StartIO();
	PBHGetVInfoAsync(&pb->fPB);
	return pb->FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSOpen(GUSIIOPBWrapper<ParamBlockRec> * pb)
{
	pb->StartIO();
	PBOpenSync(&pb->fPB);
	return pb->FinishIO();
}
OSErr GUSIFSOpenDriver(StringPtr name, short * refNum)
{
	GUSIIOPBWrapper<ParamBlockRec> 	pb; 

	pb->ioParam.ioNamePtr 	= name; 
	pb->ioParam.ioPermssn 	= fsCurPerm;
	
	OSErr err 	= GUSIFSOpen(&pb);
	*refNum = pb->ioParam.ioRefNum; 
	
	return err;
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSHGetFInfo(GUSIIOPBWrapper<HParamBlockRec> * pb)
{
	pb->StartIO();
	PBHGetFInfoAsync(&pb->fPB);
	return pb->FinishIO();
}
OSErr GUSIFSHSetFInfo(GUSIIOPBWrapper<HParamBlockRec> * pb)
{
	pb->StartIO();
	PBHSetFInfoAsync(&pb->fPB);
	return pb->FinishIO();
}
OSErr GUSIFSGetFInfo(const FSSpec * spec, FInfo * info)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum		=	spec->vRefNum;
	pb->fileParam.ioDirID		=	spec->parID;
	pb->fileParam.ioNamePtr		=	const_cast<StringPtr>(spec->name);
	pb->fileParam.ioFDirIndex	=	0;
	
	OSErr err = GUSIFSHGetFInfo(&pb);
	if (!err)
		*info = pb->fileParam.ioFlFndrInfo;
		
	return err;
}
OSErr GUSIFSSetFInfo(const FSSpec * spec, const FInfo * info)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum		=	spec->vRefNum;
	pb->fileParam.ioDirID		=	spec->parID;
	pb->fileParam.ioNamePtr		=	const_cast<StringPtr>(spec->name);
	pb->fileParam.ioFDirIndex	=	0;
	
	OSErr err = GUSIFSHGetFInfo(&pb);
	if (!err) {
		pb->fileParam.ioDirID		= spec->parID; /* Gets overwritten by PBHGetInfo */
		pb->fileParam.ioFlFndrInfo 	= *info;
		err = GUSIFSHSetFInfo(&pb);
	}
	
	return err;
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSOpenDF(const FSSpec * spec, char permission, short * refNum)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	pb->ioParam.ioPermssn	= 	permission;
	
	pb.StartIO();
	PBHOpenDFSync(&pb.fPB);
	OSErr err = pb.FinishIO();
	if (!err)
		*refNum = pb->ioParam.ioRefNum;
	
	return err;
}

OSErr GUSIFSOpenRF(const FSSpec * spec, char permission, short * refNum)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	pb->ioParam.ioPermssn	= 	permission;
	
	pb.StartIO();
	PBHOpenRFSync(&pb.fPB);
	OSErr err = pb.FinishIO();
	if (!err)
		*refNum = pb->ioParam.ioRefNum;
	
	return err;
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSHGetVolParms(GUSIIOPBWrapper<HParamBlockRec> * pb)
{
	pb->StartIO();
	PBHGetVolParmsAsync(&pb->fPB);
	return pb->FinishIO();
}
OSErr GUSIFSGetVolParms(short vRefNum, GetVolParmsInfoBuffer * volParms)
{
	GUSIIOPBWrapper<HParamBlockRec>	pb;
	
	pb->ioParam.ioNamePtr	=	nil;
	pb->ioParam.ioVRefNum	=	vRefNum;
	pb->ioParam.ioBuffer	=	Ptr(&volParms);
	pb->ioParam.ioReqCount	=	sizeof(GetVolParmsInfoBuffer);

	return GUSIFSHGetVolParms(&pb);
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSCreate(const FSSpec * spec)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	
	pb.StartIO();
	PBHCreateAsync(&pb.fPB);
	return pb.FinishIO();
}

OSErr GUSIFSCreate(const FSSpec * spec, OSType creator, OSType type, ScriptCode script)
{
	OSErr err;
	
	if (err = GUSIFSCreate(spec))
		return err;
	
	GUSIIOPBWrapper<GUSICatInfo>	info;
	info->FileInfo().ioVRefNum	=	spec->vRefNum;
	info->FileInfo().ioDirID		=	spec->parID;
	info->FileInfo().ioNamePtr	=	const_cast<StringPtr>(spec->name);
	
	if (err = GUSIFSGetCatInfo(&info))
		goto nuke;
	
	info->FInfo().fdCreator		=	creator;
	info->FInfo().fdType		=	type;
	
	if (script == smSystemScript) 
		info->FXInfo().fdScript	=	0;
	else
		info->FXInfo().fdScript	=	script | 0x80;

	if (err = GUSIFSSetCatInfo(&info))
		goto nuke;
	
	return noErr;
nuke:
	GUSIFSDelete(spec);
	
	return err;
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSDelete(const FSSpec * spec)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	
	pb.StartIO();
	PBHDeleteAsync(&pb.fPB);
	return pb.FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSDirCreate(const FSSpec * spec)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	
	pb.StartIO();
	PBDirCreateAsync(&pb.fPB);
	return pb.FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSSetFLock(const FSSpec * spec)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	
	pb.StartIO();
	PBHSetFLockAsync(&pb.fPB);
	return pb.FinishIO();
}
OSErr GUSIFSRstFLock(const FSSpec * spec)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	
	pb.StartIO();
	PBHRstFLockAsync(&pb.fPB);
	return pb.FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
OSErr GUSIFSRename(const FSSpec * spec, ConstStr255Param newname)
{
	GUSIIOPBWrapper<HParamBlockRec> 	pb; 
	
	pb->fileParam.ioVRefNum	=	spec->vRefNum;
	pb->fileParam.ioDirID	=	spec->parID;
	pb->fileParam.ioNamePtr	=	const_cast<StringPtr>(spec->name);
	pb->ioParam.ioMisc		=	(Ptr)const_cast<StringPtr>(newname);
	
	pb.StartIO();
	PBHRenameAsync(&pb.fPB);
	return pb.FinishIO();
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
static OSErr GUSIFSCatMove1(GUSIIOPBWrapper<CMovePBRec> * pb)
{
	pb->StartIO();
	PBCatMoveAsync(&pb->fPB);
	return pb->FinishIO();
}
OSErr GUSIFSCatMove(const FSSpec * spec, long dest)
{
	GUSIIOPBWrapper<CMovePBRec>	pb;
	
	pb->ioVRefNum	=	spec->vRefNum;
	pb->ioDirID		=	spec->parID;
	pb->ioNamePtr	=	const_cast<StringPtr>(spec->name);
	pb->ioNewName	=	nil;
	pb->ioNewDirID	=	dest;
	
	return GUSIFSCatMove1(&pb);
}
OSErr GUSIFSCatMove(const FSSpec * spec, const FSSpec * dest)
{
	GUSIIOPBWrapper<CMovePBRec>	pb;
	
	pb->ioVRefNum	=	spec->vRefNum;
	pb->ioDirID		=	spec->parID;
	pb->ioNamePtr	=	const_cast<StringPtr>(spec->name);
	pb->ioNewName	=	const_cast<StringPtr>(dest->name);
	pb->ioNewDirID	=	dest->parID;
	
	return GUSIFSCatMove1(&pb);
}
// <Implementation of [[GUSIFSWrappers]]>=                                 
static OSErr GUSIFSMoveRename1(const FSSpec * spec, const FSSpec * dest)
{
	OSErr err;
	
	if (err = GUSIFSCatMove(spec, dest->parID))
		return err;
		
	FSSpec corner(*spec);
	corner.parID = dest->parID;
	if (err = GUSIFSRename(&corner, dest->name))
		GUSIFSCatMove(&corner, spec->parID);

	return err;
}
OSErr GUSIFSMoveRename(const FSSpec * spec, const FSSpec * dest)
{
	OSErr							err;
	GUSIIOPBWrapper<HParamBlockRec> pb; 
	
	// Sometimes we get away with a simple call where a complex call would do  
 // the wrong thing.                                                        
 //                                                                         
 // <Try [[GUSIFSCatMove]] or [[GUSIFSRename]] if appropriate>=             
 if (spec->vRefNum != dest->vRefNum)
 	return GUSISetPosixError(EXDEV);
 if (spec->parID == dest->parID)
 	return GUSIFSRename(spec, dest->name);
 else if (EqualString(spec->name, dest->name, true, true))
 	return GUSIFSCatMove(spec, dest->parID);
	// If possible, we use the combined call.                                  
 //                                                                         
 // <Try [[PBMoveRename]] if available>=                                    
 GetVolParmsInfoBuffer			volParms;

 if (!GUSIFSGetVolParms(spec->vRefNum, &volParms) 
  && (volParms.vMAttrib & (1 << bHasMoveRename))
 ) {	
 	pb->copyParam.ioVRefNum		=	spec->vRefNum;
 	pb->copyParam.ioDirID		=	spec->parID;
 	pb->copyParam.ioNamePtr		=	const_cast<StringPtr>(spec->name);
 	pb->copyParam.ioNewName		= 	nil;
 	pb->copyParam.ioNewDirID	= 	dest->parID;
 	pb->copyParam.ioCopyName	=	const_cast<StringPtr>(dest->name);
 	
 	pb.StartIO();
 	PBHMoveRenameAsync(&pb.fPB);
 	err = pb.FinishIO();
 	
 	if (err != paramErr)
 		return err;
 }
	// <Move and rename, moving the corner file out of the way if necessary>=  
 GUSIFileSpec corner(*spec);
 corner.SetParID(dest->parID);
 if (corner.Exists()) {
 	GUSITempFileSpec temp(dest->vRefNum, dest->parID);
 	err = GUSIFSRename(&corner, temp->name);
 	if (!err) {
 		err = GUSIFSMoveRename1(spec, dest);
 		GUSIFSRename(&temp, corner->name);
 	}
 } else
 	err = GUSIFSMoveRename1(spec, dest);
	
	return err;
}

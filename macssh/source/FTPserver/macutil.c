// macutil.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// April, 1993	Entirely rewritten by Jim Browne to use HFS.



#include "maclook.proto.h"
#include "telneterrors.h"
#include "DlogUtils.proto.h"
#include "macutil.proto.h"


/* Some globals for file lookup */
char FileName[256];

//	Get the name of the directory identified by vRefNum & dirID
//	(works for volumes too... [dirID = 2])
void GetDirectoryName(short vRefNum, long dirID, Str32 name)
{
	CInfoPBRec	theInfo;
	
	WriteZero((Ptr)&theInfo, sizeof(CInfoPBRec));
	theInfo.dirInfo.ioVRefNum = vRefNum;
	theInfo.dirInfo.ioDrDirID = dirID;
	theInfo.dirInfo.ioNamePtr = name;
	theInfo.dirInfo.ioFDirIndex = -1;		// Only give me the Directory Info
	PBGetCatInfoSync(&theInfo);
}

//	Convert the given volume name to a reference number.  Returns the default vRefNum
//	if the volume specified is not found.
short VolumeNameToRefNum(Str32 volumeName)
{
	short	retval;
	
	if (SetVol(volumeName, 0) != noErr) return(-1);
	GetVol(NULL, &retval);
	
	return(retval);
}
	
void PathNameFromDirID(long dirID, short vRefNum, StringPtr fullPathName)
{
	CInfoPBRec	block;
	Str255		directoryName;
	OSErr		err;

	fullPathName[0] = '\0';

	block.dirInfo.ioDrParID = dirID;
	block.dirInfo.ioNamePtr = directoryName;
	do {
			block.dirInfo.ioVRefNum = vRefNum;
			block.dirInfo.ioFDirIndex = -1;
			block.dirInfo.ioDrDirID = block.dirInfo.ioDrParID;
			if ((err = PBGetCatInfoSync(&block)) != noErr) return;
			pstrcat(directoryName, (StringPtr)"\p/");
			pstrinsert(fullPathName, directoryName);
	} while (block.dirInfo.ioDrDirID != 2);

	pstrinsert(fullPathName, "\p/");
}

//	ChangeDirectory will change the directory relative from the provided dirID and 
//		vRefNum.  It handles preceding ".." modifiers as well as absolute pathnames.
short ChangeDirectory(long *dirID, short *vRefNum,char *pathname)
{
	CInfoPBRec		theDirInfo;
	char		tempst[256], *nSlash, *start;
	short 		NumberOfLevelsToGoUp = 0, temp, localvRefNum;
	long		junk1, junk2, localdirID;
	OSErr		err;
	
	start = pathname;
	localvRefNum = *vRefNum;
	localdirID = *dirID;
	
	while ( (nSlash = strchr(pathname,'/') ) !=0L) {
		if (( (nSlash - pathname) == 2) && (*pathname=='.') && (*(pathname+1) =='.') ) {
			pathname += 3L;
			start = pathname;
			NumberOfLevelsToGoUp++;
			}
		else {
			*nSlash=':';
			pathname=nSlash + 1L;
			}
		}
	
	if ( strcmp("..",pathname)==0) {
		start[0]=0;
		NumberOfLevelsToGoUp++;
		}

	// Now start points to the beginning of a relative pathname devoid of ".."'s, and colons
	//		for seperators rather than slashes.
	
	if ( *start==':') {							// This is a fully qualified pathname
		if (NumberOfLevelsToGoUp) return(1);	// Someone did a "../..//blah"...
		nSlash = strchr(start+1L, ':');			// Seperate out volume name
		if (strlen(start) == 1)					// Someone did a "cd /"
			localdirID = 2;						//		Use top level directory
		else {
			if (nSlash != NULL) *nSlash = 0;
			strcpy(tempst, start+1);			// Remove leading colon
			strcat(tempst, ":");				// Add trailing colon
			CtoPstr(tempst);
			SetVol((StringPtr)tempst, 0);
			GetVol(NULL, &temp);
			GetWDInfo(temp, &localvRefNum, &junk1, &junk2);
			localdirID = 2;

			if (nSlash != NULL) {				// There's more than a volume name
				*nSlash = ':';					// Restore the leading slash
				strcpy( tempst, nSlash);
				CtoPstr(tempst);				// tempst now contains relative pathname
				
				WriteZero((Ptr)&theDirInfo, (long)sizeof(CInfoPBRec));
				theDirInfo.dirInfo.ioVRefNum = localvRefNum;		// Now find the directory on the volume
				theDirInfo.dirInfo.ioDrDirID = localdirID;
				theDirInfo.dirInfo.ioNamePtr = (StringPtr)tempst;
				theDirInfo.dirInfo.ioFDirIndex = 0;
				if ((err = PBGetCatInfoSync(&theDirInfo)) != noErr) return(1);
				localdirID = theDirInfo.dirInfo.ioDrDirID;
				}		
			}
		}
	else
		{	// This is a relative pathname, start by taking care of any ".."s...
			WriteZero((Ptr)&theDirInfo, (long)sizeof(CInfoPBRec));
			while (NumberOfLevelsToGoUp && (localdirID != 2)) {
				theDirInfo.dirInfo.ioVRefNum = localvRefNum;
				theDirInfo.dirInfo.ioDrDirID = localdirID;
				theDirInfo.dirInfo.ioNamePtr = 0;
				theDirInfo.dirInfo.ioFDirIndex = -1;
				if ((err = PBGetCatInfoSync(&theDirInfo)) != noErr) return(1);
				localdirID = theDirInfo.dirInfo.ioDrParID;
				NumberOfLevelsToGoUp--;
				}
		
			// Now use the relative pathname to find out the actual directory ID
			// Relative pathnames must begin with a colon, so put on on the beginning
			
			if(start[0]) {
				strcpy(tempst+1, start);
				tempst[0] = ':';
				CtoPstr(tempst);
				WriteZero((Ptr)&theDirInfo, (long)sizeof(CInfoPBRec));
				theDirInfo.dirInfo.ioVRefNum = localvRefNum;
				theDirInfo.dirInfo.ioDrDirID = localdirID;
				theDirInfo.dirInfo.ioNamePtr = (StringPtr)tempst;
				theDirInfo.dirInfo.ioFDirIndex = 0;
				if ((err = PBGetCatInfoSync(&theDirInfo)) != noErr) return(1);
				localdirID = theDirInfo.dirInfo.ioDrDirID;
				}
		}
	
	*dirID = localdirID;		// Everything went ok, change the dirID and vRefNum
	*vRefNum = localvRefNum;		
	return(0);
}

short wccheck(char *file, char *template)	/* BYU - routine now returns (short) for recursion. */
{
	while(*template) {
		if (*template=='*') {
			template++;
			if (*template) {
				while((*file) && !wccheck(file,template)) file++;	/* BYU */
				if ((*file)==0) return(0);
				}
			else return(1);
			continue;
			}
		else
			if ((*template!='?') && (*template!=*file)) return(0);
		template++;file++;
		}
	if (*file)			/* BYU */
		return(0);		/* BYU */
	else				/* BYU */
		return(1);		/* BYU */
}

/* getFileName
*  find the name in the given directory which matches the wildcard
*  specification	*/
char *getFileName(char *spec, CInfoPBRec *finfo)
{
	CInfoPBRec *localfinfo;
	Boolean done = FALSE;
	localfinfo = (CInfoPBRec *) myNewPtr(sizeof(CInfoPBRec));	
	
	finfo->hFileInfo.ioNamePtr=(StringPtr) FileName;
	
	while (!done)
	{
		BlockMoveData(finfo, localfinfo, sizeof(CInfoPBRec));
		if (PBGetCatInfoSync(localfinfo)!=0) {
			DisposePtr((Ptr)localfinfo);
			return(0L);
			}
	
		FileName[FileName[0]+1]=0;
		if ((!wccheck(&FileName[1],spec))|| //it doesnt match
			(((localfinfo->hFileInfo.ioFlAttrib & 0x10)&& //its an invisible directory
			(localfinfo->dirInfo.ioDrUsrWds.frFlags & (fInvisible | 0x1000)))||
			(!(localfinfo->hFileInfo.ioFlAttrib & 0x10)&&//its an invisible file
			(localfinfo->hFileInfo.ioFlFndrInfo.fdFlags)&(fInvisible | 0x1000))))

			finfo->hFileInfo.ioFDirIndex++; //go to next one
		else	
			done = TRUE;
		}

	if (localfinfo->hFileInfo.ioFlAttrib & 0x10) //its a directory
	{
		FileName[++FileName[0]]='/';
		FileName[FileName[0]+1]=0;
	}

	DisposePtr((Ptr)localfinfo);
	return(&FileName[1]);
}

//	MakeDirectory will add a new directory relative from the provided dirID and 
//		vRefNum.  It handles preceding ".." modifiers as well as absolute pathnames.
short MakeDirectory(long *dirID, short *vRefNum,char *pathname) // KFG
{
	OSErr		oserr;
	char		tstr[256];
	Str255		pstr;
	FSSpec		fsspec;
	long		newDirID;
	int			n, o;

	if (pathname[0] == '/')
		BlockMoveData(&pathname[1], tstr, strlen(pathname));
	else
		{
		tstr[0] = ':';
		BlockMoveData(pathname, &tstr[1], strlen(pathname) + 1);
		}
	for (n = 0, o = strlen(tstr); n < o; n++)
		if (tstr[n] == '/') tstr[n] = ':';

	BlockMoveData(tstr, &pstr[1], (*pstr = (Byte) strlen(tstr)));
	if (tstr[0] != ':')
		oserr = FSMakeFSSpec(0x8000, 0, pstr, &fsspec);
	else
		oserr = FSMakeFSSpec(*vRefNum, *dirID, pstr, &fsspec);

	if (oserr != -43) return 1;

	return FSpDirCreate(&fsspec, 0, &newDirID);
}

//	RemoveDirectory will remove an existing directory relative from the provided dirID and 
//		vRefNum.  It handles preceding ".." modifiers as well as absolute pathnames.
short RemoveDirectory(long *dirID, short *vRefNum,char *pathname) // KFG
{
	OSErr		oserr;
	char		tstr[256];
	Str255		pstr;
	FSSpec		fsspec;
	int			n, o;

	if (pathname[0] == '/')
		BlockMoveData(&pathname[1], tstr, strlen(pathname));
	else
		{
		tstr[0] = ':';
		BlockMoveData(pathname, &tstr[1], strlen(pathname) + 1);
		}
	for (n = 0, o = strlen(tstr); n < o; n++)
		if (tstr[n] == '/') tstr[n] = ':';

	BlockMoveData(tstr, &pstr[1], (*pstr = (Byte) strlen(tstr)));
	if (tstr[0] != ':')
		oserr = FSMakeFSSpec(0x8000, 0, pstr, &fsspec);
	else
		oserr = FSMakeFSSpec(*vRefNum, *dirID, pstr, &fsspec);

	if (! oserr) oserr = FSpDelete(&fsspec);
	return oserr;
}

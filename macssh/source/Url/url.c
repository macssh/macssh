// url.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "rsinterf.proto.h"
#include "IConfig.proto.h"
#include "url.proto.h"
#include "vsdata.h"//because the damn prototype on the next line needs it  
#include "vsinterf.proto.h"
#include "ICTypes.h"
#include "ICAPI.h"
#include "ICKeys.h"

// Revised 5/97 to use IC 1.1's new ICLaunchURL routine. (RAB BetterTelnet 1.0b2c2)

extern void VSprintf(char *fmt, ...);

extern ICInstance inst;
TURLKind ParseURL(Handle, short*);

extern long dumpln(long base, char *dest, void *src, long len);

Boolean MyStrNEqual (char *s1, char *s2, short n);
OSErr FindAppOnVolume (OSType sig, short vRefNum, FSSpec *file);
OSErr FindAppFromSig (OSType sig, FSSpec *fSpec, Boolean *running,
	 ProcessSerialNumber *psn);
OSErr FindRunningAppBySignature (OSType sig, FSSpec *fSpec,
	ProcessSerialNumber *psn);
OSErr LaunchAppWithEventAndString (Boolean running, FSSpec *appSpec, ProcessSerialNumber *psn,
	OSType eventClass, OSType eventID, OSType keyword, char *str, 
	unsigned short launchFileFlags, unsigned short launchControlFlags);
OSErr OpenHelperWithURL (OSType sig, char *url);
OSErr GetLastModDateTime(FSSpec *fSpec, unsigned long *lastModDateTime);
OSErr VolHasDesktopDB (short vRefNum, Boolean *hasDesktop);
OSErr GetIndVolume (short index, short *vRefNum);

#define kGetURLEventClass				'GURL'
#define kGetURLEventID					'GURL'

static unsigned char gTable[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x41, 0x41, 0x43, 0x45, 0x4E, 0x4F, 0x55, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x43, 0x45, 0x45,
	0x45, 0x45, 0x49, 0x49, 0x49, 0x49, 0x4E, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x55, 0x55, 0x55, 0x55,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0x4F,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xAE, 0x4F,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0x22, 0x22, 0xC9, 0x20, 0x41, 0x41, 0x4F, 0xAE, 0xAE,
	0xD0, 0xD1, 0x22, 0x22, 0x27, 0x27, 0xD6, 0xD7, 0x59, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

char *gURLSchemeNames[] = {
	":",		/* kNotURL */
	"mailto:",	
	"news:",
	"nntp:",
	"ftp:",
	"http:",
	"gopher:",
	"wais:",
	"telnet:",
	"rlogin:",
	"tn3270:",
	"finger:",
	"whois:",
	"ssh:",
	"ssh2:",
	nil
};

void UnloadURL(void) { }

Boolean MyStrNEqual (char *s1, char *s2, short n)
{
	unsigned char *t = gTable;
	unsigned char *us1 = (unsigned char *)s1;
	unsigned char *us2 = (unsigned char *)s2;

	while (--n >= 0 && t[*us1++] == t[*us2++]) /* do nothing */;
	return n < 0;
}

#define CR			0x0d	/* the carriage return character */


#define isLWSP(a) ((a) == ' ' || (a) == '\t')
#define isLWSPorCR(a) (isLWSP(a) || (a) == CR)

static Boolean LaunchURL(Handle urlH, short w)
{
	TURLKind urlKind;
	OSErr err;
	OSType sig;
	short returnedSize;
	long fakeSelBegin, fakeSelEnd, handleSize;
	unsigned short launchControlFlags;
	
	HLock(urlH);

	urlKind = ParseURL(urlH, &returnedSize);

	if (urlKind != 100) {
		if ( w >= 0 )
			FlashSelection(w);
		if (urlKind == kTelnetURL || urlKind == kSSHURL || urlKind == kSSH2URL) {
			//we handle this, send apple event to ourselves
			ProcessSerialNumber psn;
			launchControlFlags = launchContinue | launchNoFileFlags | launchDontSwitch;
			GetCurrentProcess( &psn );
			err = LaunchAppWithEventAndString(TRUE, NULL, &psn,
					kGetURLEventClass, kGetURLEventID,
					keyDirectObject, *urlH, 0, launchControlFlags);
		} else if (urlKind != kNotURL) {
			fakeSelBegin = 0;
			fakeSelEnd = returnedSize;
			ICFindConfigFile(inst, 0, 0);
			err = ICLaunchURL(inst, aligned_pstring("\p"), *urlH, returnedSize, &fakeSelBegin, &fakeSelEnd);
		} else {
			// a local application to launch ?			
			Str32 nullString;
			AliasHandle	alias;
			FSSpec fileSpec;
			Boolean wasChanged;
			char *p = *urlH;
			if ( *p == '/' || *p == '\\' ) {
				char *q = p + returnedSize;
				while ( ++p < q ) {
					if ( *p == '/' )
						*p = ':';
				}
				p = *urlH + 1;
				--returnedSize;
			}
			nullString[0] = '\0';
			err = NewAliasMinimalFromFullPath(returnedSize, p, nullString, nullString, &alias);
			if ( err == noErr ) {
				err = ResolveAlias(NULL, alias, &fileSpec, &wasChanged);
				DisposeHandle((Handle)alias);
			}
			if ( err == noErr ) {
				// launch this application
				launchControlFlags = launchContinue | launchNoFileFlags /*| launchDontSwitch*/;
				err = LaunchAppWithEventAndString(FALSE, &fileSpec, NULL,
						kCoreEventClass, kAEOpenApplication,
						keyDirectObject, "\p", 0, launchControlFlags);
			}
			if ( err != noErr ) {
				/* try using IC */
				fakeSelBegin = 0;
				fakeSelEnd = returnedSize;
				ICFindConfigFile(inst, 0, 0);
				err = ICLaunchURL(inst, aligned_pstring("\p"), *urlH, returnedSize, &fakeSelBegin, &fakeSelEnd);
			}
		}
		return err == noErr;
	}
	return false;
//		sig = GetHelperInfo(urlKind);
//	if (sig == NULL)
//		return;
//	err = OpenHelperWithURL(sig, *urlH);
}


Boolean HandleURLString(ConstStr255Param urlString)
{
	Handle urlH;
	Boolean result;
	short len = urlString[0];

	urlH = NewHandle(len + 1);
	if (MemError() != noErr) {
		return false;
	}
	HLock(urlH);
	BlockMoveData(urlString + 1, *urlH, len);
	(*urlH)[len] = ' ';
	result = LaunchURL(urlH, -1);
	DisposeHandle(urlH);
	return result;
}

Boolean HandleURL(short w)
{
	Boolean result;
	Handle urlH;
	TURLKind urlKind;
	OSErr err;
	OSType sig;
	short returnedSize;
	long fakeSelBegin, fakeSelEnd, handleSize;
	
	urlH = RSGetTextSel(w, 0, 1);
	if ((urlH == (char **)-1L) || (urlH == nil)) {
		return false;
	}
	// 12/10/97, RJZ.  Increase size by one
	// so there is room for a NULL
	// terminator.
	handleSize = GetHandleSize(urlH);
	SetHandleSize(urlH, handleSize + 1);
	if (MemError() != noErr) {
		DisposeHandle(urlH);
		return false;
	}
	HLock(urlH);
	(*urlH)[handleSize] = ' ';
	result = LaunchURL(urlH, w);
	DisposeHandle(urlH);
	return result;
}


TURLKind ParseURL(Handle urlH, short *returnedSize)
{
	short size;
	char *textBegin, *textEnd, *p;
	TURLKind urlKind;
	char *schemeName;


	size = GetHandleSize(urlH);

	p = *urlH;

	textBegin = p;
	textEnd = p + size - 1;

	/* strip off leading white space and lagging white space */
	while (isLWSPorCR(*textBegin) && textBegin < textEnd) textBegin++;

										// RJZ.  12/10/97  Start search at end!

	while (isLWSPorCR(*textEnd) && textBegin < textEnd) textEnd--;

		
	/* look for < and > on each end, strip em off */
	if (*textBegin == '<') 
		textBegin++;
	if (*textEnd   == '>') 
		textEnd--;
	
	/* look for URL:, strip it*/
	
	if ((MyStrNEqual("URL:", textBegin, 4)))
		textBegin += 4;
	
	size = textEnd - textBegin + 1;
	
	if (size <= 0)
		return 100;		
	
	/* strip CR inside */
	
	p = textBegin;
	while (p < textEnd)
	{
		if (*p == CR)
		{
			BlockMoveData(p+1,  p,size - (p-textBegin + 1));
			size--;
			textEnd--;
		}
		else
			p++;
	}
			
	/* Clean up and NULL terminate */
	p = *urlH;
	BlockMoveData(textBegin, p, size);
	p[size] = 0;

	*returnedSize = size;

	/* get url type */
	while ( *p != ':' && p < *urlH + size - 1 )
		p++;

	if ( p - *urlH >= size - 1 ) {
		// colon not found
		/* might be mail address */
		//if (isMailAddress(urlH))
		//	return kMailtoURL;
	//	else
		// pathname ?
		return ( **urlH == '/' ) ? kNotURL : 100;
	}


	for (urlKind = kNotURL; ; urlKind++) {
		schemeName = gURLSchemeNames[urlKind];
		if (schemeName == nil) return kNotURL;
		if (MyStrNEqual(*urlH, schemeName, strlen(schemeName))) return urlKind;
	}
}

OSErr FindAppOnVolume (OSType sig, short vRefNum, FSSpec *file)
{
	DTPBRec pb;
	OSErr err = noErr;
	short ioDTRefNum, i;
	FInfo fInfo;
	FSSpec candidate;
	unsigned long lastModDateTime, maxLastModDateTime;

	memset(&pb, 0, sizeof(DTPBRec));
	pb.ioCompletion = nil;
	pb.ioVRefNum = vRefNum;
	pb.ioNamePtr = nil;
	err = PBDTGetPath(&pb);
	if (err != noErr) return err;
	ioDTRefNum = pb.ioDTRefNum;

	memset(&pb, 0, sizeof(DTPBRec));
	pb.ioCompletion = nil;
	pb.ioIndex = 0;
	pb.ioFileCreator = sig;
	pb.ioNamePtr = file->name;
	pb.ioDTRefNum = ioDTRefNum;
	err = PBDTGetAPPLSync(&pb);
	
	if (err == fnfErr || err == paramErr) return afpItemNotFound;
	if (err != noErr) return err;

	file->vRefNum = vRefNum;
	file->parID = pb.ioAPPLParID;
	
	err = FSpGetFInfo(file, &fInfo);
	if (err == noErr) return noErr;
	
	i = 1;
	maxLastModDateTime = 0;
	while (true) {
		memset(&pb, 0, sizeof(DTPBRec)); 
		pb.ioCompletion = nil;
		pb.ioIndex = i;
		pb.ioFileCreator = sig;
		pb.ioNamePtr = candidate.name;
		pb.ioDTRefNum = ioDTRefNum;
		err = PBDTGetAPPLSync(&pb);
		if (err != noErr) break;
		candidate.vRefNum = vRefNum;
		candidate.parID = pb.ioAPPLParID;
		err = GetLastModDateTime(file, &lastModDateTime);
		if (err == noErr) {
			if (lastModDateTime > maxLastModDateTime) {
				maxLastModDateTime = lastModDateTime;
				*file = candidate;
			}
		}
		i++;
	}
	
	return maxLastModDateTime > 0 ? noErr : afpItemNotFound;
}
/*----------------------------------------------------------------------------
	GetLastModDateTime
	
	Get the last mod date and time of a file.
	
	Entry:	fSpec = pointer to file spec.
	
	Exit:	function result = error code.
			*lastModDateTime = last mod date and time.
----------------------------------------------------------------------------*/


OSErr GetLastModDateTime(FSSpec *fSpec, unsigned long *lastModDateTime)
{
	CInfoPBRec pBlock;
	OSErr err = noErr;
	
	pBlock.hFileInfo.ioNamePtr = fSpec->name;
	pBlock.hFileInfo.ioVRefNum = fSpec->vRefNum;
	pBlock.hFileInfo.ioFDirIndex = 0;
	pBlock.hFileInfo.ioDirID = fSpec->parID;
	err = PBGetCatInfoSync(&pBlock);
	if (err != noErr) return err;
	*lastModDateTime = pBlock.hFileInfo.ioFlMdDat;
	return noErr;
}

/*----------------------------------------------------------------------------
	VolHasDesktopDB 
	
	Check to see if a volume supports the new desktop database.
	
	Entry:	vRefNum = vol ref num of volumn
			
	Exit:	function result = error code.
			*hasDesktop = true if volume has the new desktop database.
----------------------------------------------------------------------------*/

OSErr VolHasDesktopDB (short vRefNum, Boolean *hasDesktop)
{
	HParamBlockRec pb;
	GetVolParmsInfoBuffer info;
	OSErr err = noErr;
	
	pb.ioParam.ioCompletion = nil;
	pb.ioParam.ioNamePtr = nil;
	pb.ioParam.ioVRefNum = vRefNum;
	pb.ioParam.ioBuffer = (Ptr)&info;
	pb.ioParam.ioReqCount = sizeof(info);
	err = PBHGetVolParmsSync(&pb);
	*hasDesktop = err == noErr && (info.vMAttrib & (1L << bHasDesktopMgr)) != 0;
	return err;
}

OSErr FindAppFromSig (OSType sig, FSSpec *fSpec, Boolean *running,
	 ProcessSerialNumber *psn)
{
	OSErr err = noErr;
	short sysVRefNum, vRefNum, index;
	Boolean hasDesktopDB;
	long junk;
	
	if (running != nil) {
		err = FindRunningAppBySignature(sig, fSpec, psn);
		*running = true;
		if (err == noErr) return noErr;
		*running = false;
		if (err != procNotFound) return err;
	}
	err = FindFolder(kOnSystemDisk, kSystemFolderType, false, &sysVRefNum, &junk);
	if (err != noErr) return err;
	vRefNum = sysVRefNum;
	index = 0;
	while (true) {
		if (index == 0 || vRefNum != sysVRefNum) {
			err = VolHasDesktopDB(vRefNum, &hasDesktopDB);
			if (err != noErr) return err;
			if (hasDesktopDB) {
				err = FindAppOnVolume(sig, vRefNum, fSpec);
				if (err != afpItemNotFound) return err;
			}
		}
		index++;
		err = GetIndVolume(index, &vRefNum);
		if (err == nsvErr) return fnfErr;
		if (err != noErr) return err;
	}
}

OSErr GetIndVolume (short index, short *vRefNum)
{
	ParamBlockRec pb;
	OSErr err = noErr;
	
	pb.volumeParam.ioCompletion = nil;
	pb.volumeParam.ioNamePtr = nil;
	pb.volumeParam.ioVolIndex = index;
	
	err = PBGetVInfoSync(&pb);
	
	*vRefNum = pb.volumeParam.ioVRefNum;
	return err;
}

/*----------------------------------------------------------------------------
	FindRunningAppBySignature 
	
	Find a running app given its signature.
	
	Entry:	sig = signature of app.
			
	Exit:	function result = error code.
				= procNotFound if not running.
			*fSpec = file spec of app. 
			*psn = process serial number of running app.
----------------------------------------------------------------------------*/

OSErr FindRunningAppBySignature (OSType sig, FSSpec *fSpec,
	ProcessSerialNumber *psn)
{
	OSErr err = noErr;
	ProcessInfoRec info;
	
	psn->highLongOfPSN = 0;
	psn->lowLongOfPSN  = kNoProcess;
	while (true) {
		err = GetNextProcess(psn);
		if (err != noErr) return err;
		info.processInfoLength = sizeof(info);
		info.processName = nil;
		info.processAppSpec = fSpec;
		err = GetProcessInformation(psn, &info);
		if (err != noErr) return err;
		if (info.processSignature == sig) return noErr;
	}
}

/*----------------------------------------------------------------------------
	LaunchAppWithEventAndString 
	
	Launch an application with an initial event with a string parameter.
	
	Entry:	running = true if application is running, in which case
				it is sent the odoc event.
			appSpec = file spec of application.
			*psn = process serial number of app if it is running.
			eventClass = event class.
			eventID = event id.
			keyword = parameter keyword (keyDirectObject if string is the
				direct object).
			str = the string parameter for the event.
			launchFileFlags = file flags.
			launchControlFlags = control flags.
			
	Exit:	function result = error code.
----------------------------------------------------------------------------*/

OSErr LaunchAppWithEventAndString (Boolean running, FSSpec *appSpec, ProcessSerialNumber *psn,
	OSType eventClass, OSType eventID, OSType keyword, char *str, 
	unsigned short launchFileFlags, unsigned short launchControlFlags)
{
	ProcessSerialNumber thePSN;
	LaunchParamBlockRec launchThis;
	AEDesc target = {0, nil};
	AEDesc stringDesc = {0, nil};
	AEDesc launchDesc = {0, nil};
	AppleEvent theEvent = {0, nil};
	AppleEvent theReply = {0, nil};
	OSErr err = noErr;
	Boolean autoParamValue = false;

	if (running) thePSN = *psn;
	err = AECreateDesc(typeProcessSerialNumber, &thePSN, sizeof(thePSN), &target); 
	if (err != noErr) goto exit;
	err = AECreateAppleEvent(eventClass, eventID, &target,
		kAutoGenerateReturnID, kAnyTransactionID, &theEvent);
	if (err != noErr) goto exit;
	err = AECreateDesc(typeChar, str, strlen(str), &stringDesc);
	if (err != noErr) goto exit;
	err = AEPutParamDesc(&theEvent, keyword, &stringDesc);
	if (err != noErr) goto exit;
	if (running) {
		err = AESend(&theEvent, &theReply, kAENoReply, kAENormalPriority, kNoTimeOut,
			nil, nil);
		if (err != noErr) goto exit;
		if ((launchControlFlags & launchDontSwitch) == 0) {
			err = SetFrontProcess(psn);
			if (err != noErr) goto exit;
		}
	} else {
		err = AECoerceDesc(&theEvent, typeAppParameters, &launchDesc);
		if (err != noErr) goto exit;
		HLock(theEvent.dataHandle);
		launchThis.launchAppSpec = appSpec;
		launchThis.launchAppParameters = (AppParametersPtr)*launchDesc.dataHandle;
		launchThis.launchBlockID = extendedBlock;
		launchThis.launchEPBLength = extendedBlockLen;
		launchThis.launchFileFlags = launchFileFlags;
		launchThis.launchControlFlags = launchControlFlags;
		err = LaunchApplication(&launchThis);
	}
	
exit:

	if (target.dataHandle != nil) AEDisposeDesc(&target);
	if (stringDesc.dataHandle != nil) AEDisposeDesc(&stringDesc);
	if (launchDesc.dataHandle != nil) AEDisposeDesc(&launchDesc);
	if (theEvent.dataHandle != nil) AEDisposeDesc(&theEvent);
	if (theReply.dataHandle != nil) AEDisposeDesc(&theReply);
	return err;
}
/*----------------------------------------------------------------------------
	OpenHelperWithURL
	
	Open a helper program and pass it a URL string.
	
	Entry:	helperInfo = pointer to helper info.
			url = URL string.
	
	Exit:	function result = error code.
----------------------------------------------------------------------------*/
OSErr OpenHelperWithURL (OSType sig, char *url)
{
	OSErr err = noErr;
	ProcessSerialNumber psn;
	FSSpec appSpec;
	Boolean running;
	unsigned short launchControlFlags;
	
	launchControlFlags = launchContinue | launchNoFileFlags;
		
	err = FindAppFromSig(sig, &appSpec, &running, &psn);
	if (err != noErr) return err;			
			
	err = LaunchAppWithEventAndString(running, &appSpec, &psn,
				kGetURLEventClass, kGetURLEventID,
				keyDirectObject, url, 0, launchControlFlags);
	return err;
}
#define isurlschemechar(c)				(isalnum((c)) || c == '+' || c == '.' || c == '-')


/*
 * FindURLAroundPoint
 *
 * called by RSSelect when Command Click has occured outside any selection.
 * This routine looks for urls around the current point, makes that the selected area if
 * it finds one, and returns TRUE.  Otherwise leaves the current selection area untouched
 * and returns FALSE.
 */

Boolean FindURLAroundPoint(Point curr, short w)
{
	Boolean found;
	short columns;
	short blockSize;
	Handle lblock;
	Handle rblock;
	Point topleft;
	Point botright;
	short lblockSize;
	short rblockSize;
	char *p, *q, cp, cq;

	found = FALSE;
	columns = VSgetcols(w) + 1; // VSgetcols returns one less than the number of columns

	if ( curr.h < 0 )
		curr.h = 0;
	if ( curr.h >= columns )
		curr.h = columns - 1;

	topleft.h = curr.h - 255;
	topleft.v = curr.v;
	while ( topleft.h < 0 ) {
		topleft.h += columns;
		--topleft.v;
	}
	blockSize = 255 + (curr.v - topleft.v) + 1;
	lblock = myNewHandle(blockSize);
	HLock(lblock);
	lblockSize = VSgettext(w, topleft.h, topleft.v, curr.h, curr.v, *lblock, blockSize, "\015", 0, 0);

	botright.h = curr.h + 255;
	botright.v = curr.v;
	while ( botright.h >= columns ) {
		botright.h -= columns;
		++botright.v;
	}
	blockSize = 255 + (botright.v - curr.v) + 1;
	rblock = myNewHandle(blockSize);
	HLock(rblock);
	rblockSize = VSgettext(w, curr.h, curr.v, botright.h, botright.v, *rblock, blockSize, "\015", 0, 0);

	blockSize = lblockSize + rblockSize;
	HUnlock(lblock);
	SetHandleSize(lblock, blockSize);
	HLock(lblock);
	BlockMoveData( *rblock, *lblock + lblockSize, rblockSize );
	DisposeHandle(rblock);
	p = *lblock + lblockSize;
	if ( !isLWSPorCR(*p) ) {
		topleft.h = curr.h;
		topleft.v = curr.v;
		while ( p > *lblock && *p != '<' && !isLWSPorCR(p[-1]) && p[-1] != '"') {
			--p;
			if ( --topleft.h < 0 ) {
				topleft.h += columns;
				--topleft.v;
			}
		}
		q = *lblock + lblockSize;	// check first char of second block
		botright.h = curr.h + 1;
		botright.v = curr.v;
		while ( q < *lblock + blockSize && *q != '>' && !isLWSPorCR(q[1]) && q[1] != '"' ) {
			++q;
			if ( ++botright.h >= columns ) {
				botright.h -= columns;
				++botright.v;
			}
		}
		if ( q < *lblock + blockSize ) {
			if (p < q && q - p < 256 && ((*p == '<' && *q == '>') || (*p != '<' && *q != '>'))) {
				// found something
				HiliteThis(w, topleft, botright);
				found = TRUE;
			}
		}
	}
	DisposeHandle(lblock);
	return found;
}

#if 0

Boolean FindURLAroundPoint(Point curr, short w)
{
	/* called by RSSelect when Command Click has occured outside any selection.
   This routine looks for urls around the current point, makes that the selected area if
   it finds one, and returns TRUE.  Otherwise leaves the current selection area untouched
   and returns FALSE. */
/* Note: the point we receive is already normalized to a character position */
	short columns;
	Handle block;
	char *original, *start, *end, *textEnd, *p, *q;
	short i, numLines = 1;
	short neededLines, endLine = 0, startLine = 0;
	short blockSize;
	Point startPoint, endPoint;
	short  currentLine, currentLength;
	char *tempBlockPtr;
	short tempCounter, lineLength[20];
	Boolean found = FALSE, allowSpaces=FALSE;
	short firstLine, lastLine;
	short numPreviousLines;

	columns = VSgetcols(w) + 1; //VSgetcols returns one less than the number of columns

	/* we need enough space to find a 255 char URL on either side of the cursor */
	neededLines = 510/columns;
	if (255%columns != 0)
		neededLines += 2;

	/* want it to be symmetric about this line */
	if (neededLines%2 == 0)
		neededLines += 1;
										// RJZ 12/11/97.  Make sure the first
										// line is on the screen.

	firstLine = curr.v - (neededLines - 1)/2;
	lastLine = curr.v + (neededLines - 1)/2;
	if (firstLine < -VSgetnumlines(w)) {
		neededLines += (firstLine + VSgetnumlines(w));
		firstLine = -VSgetnumlines(w);
	}
	numPreviousLines = curr.v - firstLine;

	blockSize = neededLines*(columns + 1); //we need space for the \r

	block = myNewHandle((long)blockSize);

	HLock(block);
										// RJZ.  12/11/97.  Get pointer to end
										// of text *after* locking the handle.
	textEnd = *block + blockSize;

	/* get the lines we need */
	VSgettext(w, 0, firstLine, columns-1, lastLine, *block, blockSize, "\015", 0, 0);

	original = *block;
										// RJZ 12/12/97.  Fixed so that after
										// this loop, original points to the
										// start of the line where click
										// occurred.

//	numPreviousLines = (neededLines - 1)/2;
//	if (numPreviousLines > curr.v)
//		numPreviousLines = curr.v;
	for (i = 0; i < numPreviousLines; i++)
	{
		while (*original != CR)
			original++;
		original++;
	}
	original+= curr.h + 1;

	tempBlockPtr = *block;
	for (i = 0; i < neededLines; i++)
	{
		tempCounter = 0;
		while (*tempBlockPtr++ != CR) tempCounter++;
		lineLength[i] = tempCounter;
	}

	p = original;

	q = original - 1;
										// RJZ 12/11/97
	currentLine = numPreviousLines;
	while ((!found)&&(p > *block))
	{
		while (p >= *block && !isLWSPorCR(*p) && *p != '<' && *p != '"') p--;
		if (*p == CR)
		{
			if (currentLine > 0) currentLength = lineLength[currentLine - 1];
			else currentLength = 0;
			if (currentLength == columns)
			{
				p--; //this is just a wrapped line
				if (currentLine > 0) // RAB BetterTelnet 2.0b1 - sanity check
					currentLine--;
			}
			else
				found = TRUE;
		}
		else
			found = TRUE;
	}

	startLine = currentLine;
	if (*p != '<')
		p++;
	else
		allowSpaces = TRUE;

										// RJZ 12/11/97
	currentLine = numPreviousLines;

	found = FALSE;
	while (!found)
	{
		while (q < textEnd && (*q != CR) &&(!isLWSP(*q) || allowSpaces)
						&& *q != '>' && *q != '"') q++;
		if (*q == CR)
		{
										// RJZ 12/10/97

			if (lineLength[currentLine] == columns || allowSpaces)
			{
				q++; //this is just a wrapped line
				currentLine++;
			}
			else
				found = TRUE;
		}
		else
			found = TRUE;
	}
	endLine = currentLine;
	if (*q != '>') q--;
										// RJZ 12/10/97.  Dispose of block
										// before returning.
	if (p >= q) {
		DisposeHandle(block);
		return FALSE;
	}

	while (p < q && (isLWSPorCR(*p))) p++;
	while (p < q && (isLWSPorCR(*q))) q--;

										// RJZ.  12/10/97.  Dispose block
										// before returning.
	if (*p == '<') {
		if (*q != '>') {
			DisposeHandle(block);
			return FALSE;
		}
	} else {
		if (*q == '>') {
			DisposeHandle(block);
			return FALSE;
		}
	}

	start = p;
	end = q;

	HUnlock(block);

										// RJZ 12/11/97

	startPoint.v = curr.v - (numPreviousLines - startLine);
	endPoint.v   = curr.v + endLine - numPreviousLines;

	if (startPoint.v == curr.v)
		startPoint.h = curr.h - (original - start);
	else
	{
		long numChars = 0;
										// RJZ 12/11/97
		for (i = 0; i < startLine; i++)
			numChars += lineLength[i] + 1;
		startPoint.h = (start - *block - numChars - 1);
	}
	if (endPoint.v == curr.v)
		endPoint.h = curr.h + (end - original + 1);
	else
	{
		long numChars = 0;
										// RJZ 12/11/97
		for (i = 0; i < endLine; i++)
			numChars += lineLength[i] + 1;
		endPoint.h = (end - *block - numChars);
	}

	HiliteThis(w, startPoint, endPoint);
										// RJZ 12/10/97.  Dispose  block
										// before returning.
	DisposeHandle(block);
	return TRUE;
}

#endif
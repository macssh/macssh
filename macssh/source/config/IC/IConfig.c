/* IConfig.c */

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "IConfig.proto.h"
#include "ICTypes.h"
#include "ICAPI.h"
#include "ICKeys.h"


#define kICftpHelper "\pHelper¥ftp"
#define kIChttpHelper "\pHelper¥http"
#define kICgopherHelper "\pHelper¥gopher"
#define kICwaisHelper "\pHelper¥wais"
#define kICtelnetHelper "\pHelper¥telnet"
#define kICtn3270Helper "\pHelper¥tn3270"
#define kICfingerHelper "\pHelper¥finger"
#define kICwhoisHelper "\pHelper¥whois"

ICInstance inst;
Boolean gHaveIC;

char * gICHelperKeys[]  = {
	NULL, /* kNotURL*/
	(char *)"\pHelper¥mailto",
	(char *)"\pHelper¥nntp",
	(char *)"\pHelper¥nntp",
	(char *)"\pHelper¥ftp",
	(char *)"\pHelper¥http",
	(char *)"\pHelper¥gopher",
	(char *)"\pHelper¥wais",
	NULL, /* we handle telnet URL */
	NULL, /* we handle rlogin URL */
	(char *)"\pHelper¥tn3270",
	(char *)"\pHelper¥finger",
	(char *)"\pHelper¥whois"
};

void ICUnload(void) { }
OSType GetHelperInfo(TURLKind urlKind)
{
	StringPtr key;
	ICError icErr;
	ICAttr attr;
	long size;
	ICAppSpec icAppSpec;

	if (!gHaveIC)
		return NULL;
	
	key = (unsigned char *)gICHelperKeys[urlKind];
	if (key == NULL)
		return NULL;
		
	size = sizeof(icAppSpec);
	icErr = ICBegin(inst, icReadOnlyPerm); //we dont touch

	icErr |= ICGetPref(inst, aligned_pstring(key), &attr, (Ptr)&icAppSpec, &size);
	
	icErr |= ICEnd(inst);
	if (icErr == noErr)
		return (icAppSpec.fCreator);		
	else
		return (NULL);
}
void ICinit(void)
{
	ICError ICerr;
	ICDirSpecArray folder_spec;
	
	ICerr = ICStart(&inst, 'Ssh2');			/* tell it our creator */

	folder_spec[0].vRefNum = -1;			/* search for prefs in system */
	folder_spec[0].dirID = 2;						
	ICerr = ICFindConfigFile(inst, 1, (ICDirSpecArrayPtr) &folder_spec);
	gHaveIC = (ICerr == noErr);
}

void stopInternetConfig(void)
{
	ICError ICerr;
	if (gHaveIC)
		ICerr = ICStop(inst);
}

// This hack was written by Jim Matthews, of Fetch fame. Thanks!
// It works around a bug in CodeWarrior that doesn't word-align Pascal strings.
// Apparently, Internet Config is very sensitive to this.

/*
 * aligned_pstring() --
 *	Passed a possibly-aligned pstring, we return the address of an aligned
 *	one (either the original or a copy).
 *
 *	Note: we use static data for the copy.
 */
StringPtr aligned_pstring(StringPtr pstr)
{
	static Str255	aligned;
	
	if (((long) pstr) & 0x1)
	{
		memcpy(aligned, pstr, pstr[0] + 1);
		return aligned;
	}
	else
		return pstr;
} /* aligned_pstring() */

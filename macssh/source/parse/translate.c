/*
*	translate.c
*	written by Roland MŒnsson, Lund University Computing Center, Sweden
*	roland_m@ldc.lu.se
*	July 1992
*
*	Modified by Pascal Maes
*	UCL/ELEC
*	Place du Levant, 3
*	B-1348 Louvain-la-Neuve
*	Modified 7/93 by Jim Browne for NCSA.
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "wind.h"
#include "vsdata.h"
#include "translate.h"
#include "translate.proto.h"
#include "vsinterf.proto.h"
#include "LinkedList.proto.h"

#include "TextCommon.h"
#include "TextEncodingConverter.h"


extern void VSprintf(char *fmt, ...);
extern void VSdump(char *p, int len);

static void trTECInit();

//#define	DEBUG_TRANSLATION
/*************** external variables ***************/

extern 	WindRec *screens;		/* The screen array from maclook.c */
extern	short scrn;				/* The current screen from maclook.c */

extern int gVSemlogging;

/*************** global variables ***************/

BytePtr	DefaultTable,
		FTPinTable,
		FTPoutTable;
		
Handle			transTablesHdl;
short			nNational = 0;
short			gTableCount = 0;
TextEncoding	gLocalEncoding = 0;

short	transBuffer(short oldtable, short newtable)		/* translate entire buffer */
{
	VSscrn	*vsscreen;
	VSline	*vslin,*p;
	short		lineNo,maxLineNo;
	short		width;				/* allocated witdth of window (80/132) */
	char		tmp[80];				/* only for debugging */
	
	vsscreen = VSwhereis (screens[scrn].vs);
	vslin = vsscreen->buftop;

#ifdef DEBUG_TRANSLATION
	putln ("in transBuffer, well and alive");
	sprintf (tmp,"VSgetlines(screens[scrn].vs):%d", VSgetlines(screens[scrn].vs)); putln (tmp);
	sprintf (tmp,"VSmaxwidth(screens[scrn].vs):%d", VSmaxwidth(screens[scrn].vs)); putln (tmp);
	sprintf (tmp,"vsscreen->lines:%d\n",vsscreen->lines); putln (tmp);
	sprintf (tmp,"vsscreen->maxlines:%d\n",vsscreen->maxlines); putln (tmp);
	sprintf (tmp,"vsscreen->numlines:%d\n",vsscreen->numlines); putln (tmp);
	sprintf (tmp,"vsscreen->allwidth:%d\n",vsscreen->allwidth); putln (tmp);
	sprintf (tmp,"vsscreen->maxwidth:%d\n",vsscreen->maxwidth); putln (tmp);
	sprintf (tmp,"vsscreen:%08x\n",vsscreen);  putln (tmp);
	sprintf (tmp,"vslin:%08x\n",vslin);  putln (tmp);
	sprintf (tmp,"next:%08x\n",vslin->next);  putln (tmp);
	sprintf (tmp,"prev:%08x\n",vslin->prev);  putln (tmp);
	sprintf (tmp,"text:%08x\n\n",vslin->text);  putln (tmp);
#endif

	width = VSmaxwidth(screens[scrn].vs)+1;	/* VSmaxwidth returns 79 or 131 */
	p = vslin;
	maxLineNo = vsscreen->numlines+VSgetlines(screens[scrn].vs); /* VSgetlines returns 24 or whatever */
	for (lineNo=1; lineNo<=maxLineNo; lineNo++) {
		/*sprintf(tmp,"lineNo:%d, p:%08x, starts with:%c%c%c%c",lineNo,p,*(p->text),*(p->text+1),*(p->text+2),*(p->text+3)); putln(tmp);*/
		if (p==NULL) { putln ("p is NULL"); return (-1); }
		if (p->text==NULL) { putln ("p->text is NULL"); return (-1); }

		// First convert the line back to Mac US format, and then to the new format.
		// FIXME: cashes with 2 bytes characters...
		// switchintranslation
		//trbuf_mac_nat(tw, (unsigned char *)p->text, width, (unsigned char *)p->text, &outlen);
		// switchintranslation
		//trbuf_nat_mac((unsigned char *)p->text, width, (unsigned char *)p->text, &outlen);

		p = p->next;
	}
	sprintf (tmp, "transBuffer:did convert %d lines", lineNo-1); putln (tmp);
	return (0);
}


short GetTranslationIndex(short table)
{
	if ( table > gTableCount ) {
		return gTableCount - table;
	}
	return table;
}


BytePtr GetTranslationResource(short id)
{
	Handle	h;

	h = GetResource(MY_TRSL, id);
	if ( h == NULL || ResError() != noErr ) {
		return NULL;
	}
	DetachResource(h);
	HLockHi(h);
	return (BytePtr)*h;
}


//	table #'s 1...n correspond to tables in our master array, table #0 is the default table
BytePtr ReturnTablePtr(short table, Boolean out)
{
	BytePtr ptable;

	if ( table < 1 || table > gTableCount ) {
		ptable = DefaultTable;
	} else {
		ptable = ((BytePtr)*transTablesHdl) + ((table - 1) * 512);
	}
	if ( out && ptable ) {
		ptable += 256;
	}
	return ptable;
}


//	The Default table (i.e. no translation) and the two FTP tables are stored in the 
//	 Application's resource fork as resources of type TRSL.  The
//	 tables added and removed by the user are stored in the prefs file as resources of
//	 type taBL.  This routine loads the default table and the two FTP tables into memory.
//	 Failures can only be caused by an incorrect application resource fork.
void	Setup_Default_Tables(void)
{
	DefaultTable = GetTranslationResource(TRSL_DEFAULT_TABLE);
	FTPinTable = GetTranslationResource(TRSL_FTP_TABLE);
	FTPoutTable = FTPinTable + 256;
}


void BuildTranslateMenu(MenuHandle whichMenu)
{
	Str255		scratchPstring;

	short numberOfTerms = CountResources(USER_TRSL);
	LinkedListNode *theHead = createSortedList2(USER_TRSL, numberOfTerms, NULL);
	GetIndString(scratchPstring, MISC_STRINGS, NONE_STRING); //"None" string
	AppendMenu(whichMenu, scratchPstring);
	addListToMenu/*3*/(whichMenu, theHead, 2);
	EnableItem(whichMenu, 0);		// Make sure the entire menu is enabled
	deleteList(&theHead);
	if ( gTableCount < nNational ) {
		// append hard-coded translations
		// WARNING: this string  must match the one in NewPreferences
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 2, "\pJIS (ISO-2022-JP)");
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 3, "\pEUC-JP");
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 4, "\pShift-JIS");
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 5, "\pBig-5");
		/* tests */
/*
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 5, "\pJISX0208 1978");
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 6, "\pJISX0208 1983");
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 7, "\pJISX0212 1990");
		AppendMenu(whichMenu, "\p ");
		SetMenuItemText(whichMenu, gTableCount + 8, "\pJISX0201 1976 kana");
*/
	}
}

#pragma mark -
/* 
 *	Be very careful with calling putln from this procedure, since
 *	putln uses the translation tables. If the tables are not setup
 *	garbage output will appear. This is not harmful, but very
 *	annoying.
 */

void trInit(MenuHandle whichMenu)
{
	short	i, numTables;		
	Handle	h;
	LinkedListNode *theHead;
		
	trTECInit();

	Setup_Default_Tables();
	UseResFile(TelInfo->ApplicationFile);
	gTableCount = CountResources(USER_TRSL);
	if (gTableCount) {
		transTablesHdl = myNewHandle(512 * gTableCount);
		HLockHi(transTablesHdl);
		nNational += gTableCount;
		BuildTranslateMenu(whichMenu);
		for (i = 0; i < gTableCount; i++) {
			// start adding things from the second menu item (first is none)
			Str255 menuItemName;
			GetMenuItemText(whichMenu, i + 2, menuItemName);
			h = GetNamedResource(USER_TRSL, menuItemName);
			if (h != NULL && ResError() == noErr) {
				if ( GetHandleSize(h) == 512 ) {
					HLock(h);
					BlockMoveData(*h, *transTablesHdl + i * 512, 512);
				}
				ReleaseResource(h);
			}
		}
	}
	UseResFile(TelInfo->SettingsFile);
}


/*
 * trTECInit
 */
static void trTECInit()
{
#if GENERATINGPOWERPC
	CFragConnectionID connID;
#endif
	OSStatus 		theErr = noErr;
	OSStatus		res = noErr;
	TextEncoding	outputEncoding;
	TextEncoding	inputEncoding;

#if GENERATINGPOWERPC
	if (GetSharedLibrary("\pTextCommon", kPowerPCCFragArch, kFindCFrag, &connID, NULL, NULL))
		return;
	if (FindSymbol( connID, "\pCreateTextEncoding", NULL, NULL ) != noErr)
		return;
	if (GetSharedLibrary("\pTextEncodingConverter", kPowerPCCFragArch, kFindCFrag, &connID, NULL, NULL))
		return;
	if (FindSymbol( connID, "\pTECCreateConverter", NULL, NULL ) != noErr)
		return;
 	nNational = 4;	// 4 hard-coded translations actually
 	//nNational = 7;	// tests

	// use current system settings for local encoding
	UpgradeScriptInfoToTextEncoding(smSystemScript, kTextLanguageDontCare,
									kTextRegionDontCare, NULL, &gLocalEncoding);
#endif
}


/*
 * inittranslation
 */
void inittranslation(WindRec *tw)
{
	short national = tw->innational;

	tw->fromconverter = NULL;
	tw->toconverter = NULL;
	tw->innational = 0x7fff;	// force converter to load
	tw->incharset = -1;
	tw->outnational = 0x7fff;	// force converter to load
	tw->outcharset = -1;
	switchintranslation(tw, national, kASCII);
	switchouttranslation(tw, national, kASCII);
}

/*
 * disposetranslation
 */
void disposetranslation(WindRec *tw)
{
#if GENERATINGPOWERPC
	if ( tw->fromconverter ) {
		TECDisposeConverter(tw->fromconverter);
		tw->fromconverter = NULL;
	}
	if ( tw->toconverter ) {
		TECDisposeConverter(tw->toconverter);
		tw->toconverter = NULL;
	}
#endif
}

/*
 * switchintranslation
 */
void switchintranslation(WindRec *tw, short national, short charset)
{
#if GENERATINGPOWERPC
	short			table;
	OSStatus		res;
	TextEncoding	outputEncoding;
	TextEncoding	inputEncoding;
	char			*cname ;

	if (gVSemlogging)
		return;

	if ( national < 0 ) {
		national = gTableCount - national;
	}
	if ( charset < 0 ) {
		charset = 0;
	}
	table = GetTranslationIndex(national);

	if ( tw->innational != national ) {
		switch ( table ) {
			case kTRJIS:
				cname = "kTRJIS";
				inputEncoding  = kTextEncodingISO_2022_JP;
				outputEncoding = kTextEncodingMacJapanese;
				break;
			case kTREUC_JP:
				cname = "kTREUC_JP";
				inputEncoding  = kTextEncodingEUC_JP;
				outputEncoding = kTextEncodingMacJapanese;
				break;
			case kTRShiftJIS:
				cname = "kTRShiftJIS";
				inputEncoding  = kTextEncodingShiftJIS;
				outputEncoding = kTextEncodingMacJapanese;
				break;
			case kTRBig5:
				cname = "kTRBig5";
				inputEncoding  = kTextEncodingBig5;
				outputEncoding = (gLocalEncoding == 0) ? kTextEncodingMacChineseTrad : gLocalEncoding;
				break;

			case kTRJISX0208_1978:
				cname = "kTRJISX0208_1978";
				inputEncoding  = kTextEncodingISO_2022_JP;
				charset = kJISX0208_1978;
				outputEncoding = kTextEncodingMacJapanese;
				break;
			case kTRJISX0208_1983:
				cname = "kTRJISX0208_1983";
				//inputEncoding  = kTextEncodingJIS_X0208_83;
				inputEncoding  = kTextEncodingISO_2022_JP;
				charset = kJISX0208_1983;
				outputEncoding = kTextEncodingMacJapanese;
				break;

			case kTRJISX0212_1990:
				cname = "kTRJISX0212_1990";
				inputEncoding  = kTextEncodingJIS_X0212_90;
				outputEncoding = kTextEncodingMacJapanese;
				break;
			case kTRJISX0201_76kana:
				cname = "kTRJISX0201_76kana";
				inputEncoding  = kTextEncodingJIS_X0201_76;
				outputEncoding = kTextEncodingMacJapanese;
				break;
			default:
				inputEncoding = 0;
				break;
		}
		if ( tw->fromconverter ) {
			TECDisposeConverter(tw->fromconverter);
			tw->fromconverter = NULL;
		}

		VSIw->trincount = 0;
		VSIw->trinx = 0;
		VSIw->trintag = 0;

		if ( inputEncoding ) {
			outputEncoding = CreateTextEncoding(outputEncoding, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
			res = TECCreateConverter(&tw->fromconverter, inputEncoding, outputEncoding);
			if ( res ) {
				VSprintf("### switchintranslation %s failed : %d\n", cname, res);
			}
		}
		tw->innational = national;
		tw->incharset = -1;
	} else {
		// force charset for subsets of ISO_2022_JP
		if ( national == kTRJISX0208_1978 )
			charset = kJISX0208_1978;
		else if ( national == kTRJISX0208_1983 )
			charset = kJISX0208_1983;
	}

	switch ( table ) {
		case kTRJIS:
		case kTRJISX0208_1978:
		case kTRJISX0208_1983:
			if ( tw->incharset != charset ) {
				tw->incharset = charset;
				if ( tw->fromconverter ) {
					ByteCount srclen;
					ByteCount dstlen;
					unsigned char buf[4];
					dstlen = 4;
					res = TECFlushText(tw->fromconverter, buf, dstlen, &dstlen);
					switch (charset) {
						case kASCII:
							cname = "kASCII";
							buf[0] = 0x1b;
							buf[1] = '(';
							buf[2] = 'B';
							break;
						case kJISX0201_1976:
							cname = "kJISX0201_1976";
							buf[0] = 0x1b;
							buf[1] = '(';
							buf[2] = 'J';
							break;
						case kJISX0208_1978:		// 	‹ŒJIS ?
							cname = "kJISX0208_1978";
							buf[0] = 0x1b;
							buf[1] = '$';
							buf[2] = '@';
							break;
						case kJISX0208_1983:		//  VJIS ?
							cname = "kJISX0208_1983";
							buf[0] = 0x1b;
							buf[1] = '$';
							buf[2] = 'B';
							break;
						case kJISX0201_1976Kana:	// [Not Std ISO-2022-JP]
							cname = "kJISX0201_1976Kana";
							buf[0] = 0x1b;
							buf[1] = '(';
							buf[2] = 'I';
							break;
						case kJISX0212_1990:		// [Not Std ISO-2022-JP]
							cname = "kJISX0212_1990";
							buf[0] = 0x1b;
							buf[1] = '$';
							buf[2] = 'D';
							break;
						default:
							return;
					}

					srclen = 3;
					dstlen = 4;
					res = TECConvertText(tw->fromconverter, buf, srclen, &srclen, buf, dstlen, &dstlen);
					if ( res && res != kTECPartialCharErr ) {
						VSprintf("### switchintranslation %s failed : %d\n", cname, res);
					}
				}
			}
			break;
	}
#else
	if (gVSemlogging)
		return;
	tw->innational = national;
	tw->incharset = charset;
#endif
}

/*
 * switchouttranslation
 */
void switchouttranslation(WindRec *tw, short national, short charset)
{
#if GENERATINGPOWERPC
	short			table;
	OSStatus		res;
	TextEncoding	outputEncoding;
	TextEncoding	inputEncoding;
	char			*cname;

	if (gVSemlogging)
		return;

	if ( national < 0 ) {
		national = gTableCount - national;
	}
	table = GetTranslationIndex(national);

	if ( tw->outnational != national ) {
		switch ( table ) {
			case kTRJIS:
				cname = "kTRJIS";
				inputEncoding  = kTextEncodingMacJapanese;
				outputEncoding = kTextEncodingISO_2022_JP;
				break;
			case kTREUC_JP:
				cname = "kTREUC_JP";
				inputEncoding  = kTextEncodingMacJapanese;
				outputEncoding = kTextEncodingEUC_JP;
				break;
			case kTRShiftJIS:
				cname = "kTRShiftJIS";
				inputEncoding  = kTextEncodingMacJapanese;
				outputEncoding = kTextEncodingShiftJIS;
				break;
			case kTRBig5:
				cname = "kTRBig5";
				inputEncoding = (gLocalEncoding == 0) ? kTextEncodingMacChineseTrad : gLocalEncoding;
				outputEncoding = kTextEncodingBig5;
				break;

			case kTRJISX0208_1978:
				cname = "kTRJISX0208_1978";
				inputEncoding  = kTextEncodingMacJapanese;
				outputEncoding = kTextEncodingISO_2022_JP;
				charset = kJISX0208_1978;
				break;
			case kTRJISX0208_1983:
				cname = "kTRJISX0208_1983";
				inputEncoding  = kTextEncodingMacJapanese;
				//outputEncoding = kTextEncodingJIS_X0208_83;
				outputEncoding = kTextEncodingISO_2022_JP;
				charset = kJISX0208_1983;
				break;

			case kTRJISX0212_1990:
				cname = "kTRJISX0212_1990";
				inputEncoding  = kTextEncodingMacJapanese;
				outputEncoding = kTextEncodingJIS_X0212_90;
				break;
			case kTRJISX0201_76kana:
				cname = "kTRJISX0201_76kana";
				inputEncoding  = kTextEncodingMacJapanese;
				outputEncoding = kTextEncodingJIS_X0201_76;
				break;
			default:
				inputEncoding = 0;
				break;
		}
		if ( tw->toconverter ) {
			TECDisposeConverter(tw->toconverter);
			tw->toconverter = NULL;
		}
		if ( inputEncoding ) {
			tw->troutcount = 0;
			inputEncoding = CreateTextEncoding(inputEncoding, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
			res = TECCreateConverter(&tw->toconverter, inputEncoding, outputEncoding);
			if ( res ) {
				VSprintf("### switchouttranslation %s failed : %d\n", cname, res);
			}
		}
		tw->outnational = national;
		tw->outcharset = -1;
	} else {
		// force charset for subsets of ISO_2022_JP
		if ( national == kTRJISX0208_1978 )
			charset = kJISX0208_1978;
		else if ( national == kTRJISX0208_1983 )
			charset = kJISX0208_1983;
	}

/*
	if ( charset < 0 ) {
		charset = 0;
	}

	switch ( table ) {
		case kTRJIS:
		case kTRJISX0208_1978:
		case kTRJISX0208_1983:
			if ( tw->outcharset != charset ) {
				tw->outcharset = charset;
				if ( tw->toconverter ) {
					ByteCount srclen;
					ByteCount dstlen;
					unsigned char buf[4];
					dstlen = 4;
					res = TECFlushText(tw->toconverter, buf, dstlen, &dstlen);
					switch (charset) {
						case kASCII:
							cname = "kASCII";
							buf[0] = 0x1b;
							buf[1] = '(';
							buf[2] = 'B';
							break;
						case kJISX0201_1976:
							cname = "kJISX0201_1976";
							buf[0] = 0x1b;
							buf[1] = '(';
							buf[2] = 'J';
							break;
						case kJISX0208_1978:		// 	‹ŒJIS ?
							cname = "kJISX0208_1978";
							buf[0] = 0x1b;
							buf[1] = '$';
							buf[2] = '@';
							break;
						case kJISX0208_1983:		//  VJIS ?
							cname = "kJISX0208_1983";
							buf[0] = 0x1b;
							buf[1] = '$';
							buf[2] = 'B';
							break;
						case kJISX0201_1976Kana:	// [Not Std ISO-2022-JP]
							cname = "kJISX0201_1976Kana";
							buf[0] = 0x1b;
							buf[1] = '(';
							buf[2] = 'I';
							break;
						case kJISX0212_1990:		// [Not Std ISO-2022-JP]
							cname = "kJISX0212_1990";
							buf[0] = 0x1b;
							buf[1] = '$';
							buf[2] = 'D';
							break;
						default:
							return;
					}
					srclen = 3;
					dstlen = 4;
					res = TECConvertText(tw->toconverter, buf, srclen, &srclen, buf, dstlen, &dstlen);
					if ( res && res != kTECPartialCharErr ) {
						VSprintf("### switchouttranslation %s failed : %d\n", cname, res);
					}
				}
			}
			break;
	}
*/
#else
	if (gVSemlogging)
		return;
	tw->outnational = national;
	tw->outcharset = charset;
#endif
}

/*	flush input translation */
int trflush_nat_mac(WindRec *tw)
{
	OSStatus	res = 0;
#if GENERATINGPOWERPC
	short		table;
	char		buf[32];
	ByteCount	len;
	short		charset;
	
	//tw->trincount = 0;
	//tw->trinx = 0;
	//tw->trintag = 0;
	table = GetTranslationIndex(tw->innational);
	if ( table < 0 ) {
		if ( tw->fromconverter ) {
			len = sizeof(buf);
			res = TECFlushText(tw->fromconverter, buf, len, &len);
/*
			charset = tw->incharset;
			tw->incharset = -1;
			switchintranslation(tw, tw->innational, charset);
*/
		}
	}
#endif
	return res;
}

/*	flush output translation */
int trflush_mac_nat(WindRec *tw)
{
	OSStatus	res = 0;
#if GENERATINGPOWERPC
	short		table;
	char		buf[32];
	ByteCount	len;
	short		charset;
	
	tw->troutcount = 0;
	table = GetTranslationIndex(tw->outnational);
	if ( table < 0 ) {
		if ( tw->toconverter ) {
			len = sizeof(buf);
			res = TECFlushText(tw->toconverter, buf, len, &len);
/*
			charset = tw->outcharset;
			tw->outcharset = -1;
			switchouttranslation(tw, tw->outnational, charset);
*/
		}
	}
#endif
	return res;
}


/*	Converts a buffer from 8/16-bit National to 8/16-bit Macintosh */
/* WARNING: ouptut size can be twice as big as input */
int trbuf_nat_mac(WindRec *tw, unsigned char *buf, long *len, unsigned char *out, long *outlen)
{
	OSStatus		res = 0;
	short			table;
	long			i = *len;
	BytePtr			table_data;

	table = GetTranslationIndex(tw->innational);
	if ( table >= 0 ) {
		table_data = ReturnTablePtr(table, FALSE);
		if ( table_data ) {
			while (i--)
				*out++ = table_data[*buf++];
		} else {
			memcpy(out, buf, *len);
		}
		*outlen = *len;
#if GENERATINGPOWERPC
	} else {
		switch ( table ) {
			case kTRJIS:
			case kTRJISX0208_1978:
			case kTRJISX0208_1983:
				switch (tw->incharset) {
				    case kASCII:
				    case kJISX0201_1976:
				    case kJISX0208_1978:
				    case kJISX0208_1983:
				    case kJISX0212_1990:
						if ( tw->fromconverter ) {
							res = TECConvertText(tw->fromconverter, buf, *len, len, out, *outlen, outlen);
						} else {
							memcpy(out, buf, *len);
							*outlen = *len;
						}
						break;
				    case kJISX0201_1976Kana:
				    	// FIXME
						memcpy(out, buf, *len);
						*outlen = *len;
						break;
					default:
						memcpy(out, buf, *len);
						*outlen = *len;
						break;
				}
				break;
			case kTREUC_JP:
			case kTRShiftJIS:
			case kTRBig5:
			//case kTRJISX0208_1978:
			//case kTRJISX0208_1983:
			case kTRJISX0212_1990:
			case kTRJISX0201_76kana:
				if ( tw->fromconverter ) {
					res = TECConvertText(tw->fromconverter, buf, *len, len, out, *outlen, outlen);
					break;
				}
				// FALL THROUGH
			default:
				memcpy(out, buf, *len);
				*outlen = *len;
				break;
		}
#endif
	}
	return res;
}

/*	Converts a buffer from 8/16-bit Macintosh to 8/16-bit National */
/* WARNING: ouptut size can be twice as big as input */
int trbuf_mac_nat(WindRec *tw, unsigned char *buf, long *len, unsigned char *out, long *outlen)
{
	OSStatus		res = 0;
	short			table;
	long			i = *len;
	BytePtr			table_data;

	table = GetTranslationIndex(tw->outnational);
	if ( table >= 0 ) {
		table_data = ReturnTablePtr(table, TRUE);
		if ( table_data ) {
			while (i--)
				*out++ = table_data[*buf++];
		} else {
			memcpy(out, buf, *len);
		}
		*outlen = *len;
#if GENERATINGPOWERPC
	} else {
		switch ( table ) {
			case kTRJIS:
			case kTREUC_JP:
			case kTRShiftJIS:
			case kTRBig5:
			case kTRJISX0208_1978:
			case kTRJISX0208_1983:
			case kTRJISX0212_1990:
			case kTRJISX0201_76kana:
				if ( tw->toconverter ) {
					res = TECConvertText(tw->toconverter, buf, *len, len, out, *outlen, outlen);
					break;
				}
				// FALL THROUGH
			default:
				memcpy(out, buf, *len);
				*outlen = *len;
				break;
		}
#endif
	}
	return res;
}


/*	Converts a handle from 8/16-bit National to 8/16-bit Macintosh */
Handle htrbuf_nat_mac(WindRec *tw, Handle hin)
{
	long len = GetHandleSize(hin);
	long outlen = len * 2;
	Handle hout = NewHandle(outlen);
	if (hout) {
		HLock(hin);
		HLock(hout);
		trbuf_nat_mac(tw, (unsigned char *)*hin, &len, *hout, &outlen);
		DisposeHandle(hin);
		HUnlock(hout);
		SetHandleSize(hout, outlen);
		return hout;
	} else {
		/* failed ... */
		return hin;
	}
}


/*	Converts a handle from 8/16-bit Macintosh to 8/16-bit National */
Handle htrbuf_mac_nat(WindRec *tw, Handle hin)
{
	long len = GetHandleSize(hin);
	long outlen = len * 2;
	Handle hout = NewHandle(outlen);
	if (hout) {
		HLock(hin);
		HLock(hout);
		trbuf_mac_nat(tw, (unsigned char *)*hin, &len, *hout, &outlen);
		DisposeHandle(hin);
		HUnlock(hout);
		SetHandleSize(hout, outlen);
		return hout;
	} else {
		/* failed ... */
		return hin;
	}
}


unsigned char ftp_iso_mac(unsigned char *ascii)
{
	if ( FTPinTable ) {
		*ascii = FTPinTable[*ascii];
	}
	return *ascii;
}


unsigned char ftp_mac_iso(unsigned char *ascii)
{
	if ( FTPoutTable ) {
		*ascii = FTPoutTable[*ascii];
	}
	return *ascii;
}


void trbuf_ftp_mac(unsigned char *buf, short len)
{
	if ( FTPinTable )
		while (len--)
			*buf++ = FTPinTable[*buf];
}

void trbuf_mac_ftp(unsigned char *buf, short len)
{

	if ( FTPoutTable )
		while (len--)
			*buf++ = FTPoutTable[*buf];
}

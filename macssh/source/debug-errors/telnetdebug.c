// debug.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "vsdata.h"
#include "vsinterf.proto.h"		// For VSwrite proto 
#include "wind.h"			    // For WindRec structure
#include "parse.proto.h"		// For DemangleLinemodeShort
#include "rsinterf.proto.h"		// For RSshow proto
#include "linemode.proto.h"
#include "prefs.proto.h"
WindRec	*console;		//	Window Record (VS) for	console Window   
extern WindRec	*screens;

#include <stdio.h>

#define DEBUG_FACILITIES

extern MenuHandle myMenus[NMENUS];		/* Menu Handles .... */

void InitDebug(void)
{
#ifdef DEBUG_FACILITIES
	Rect pRect;
	TerminalPrefs **termHdl;
	Boolean scratchBoolean;
	unsigned long flags;

	console = (WindRec *) myNewPtr(sizeof(WindRec));
	console->vs = -1;
	
//	SetRect(&pRect, 50, 150, 700, 350);		// Need to make this a resource!
	SetRect(&pRect, 50, 150, 0, 0);
	
	flags = RSWwrapon | RSWgoaway | RSWignoreBeeps | RSWoldscrollback;
	if (TelInfo->debug)
		flags |= RSWshowit;

	console->vs=RSnewwindow( &pRect, 2000, 80, 24, "\p<console>", DefFONT, DefSIZE, 0,
							DefFONT, DefSIZE, 0, 0, flags);

	console->wind = RSgetwindow( console->vs);
	((WindowPeek)console->wind)->windowKind = WIN_CONSOLE;

//	VSwrite(console->vs,"\033[24;0H",7);
	console->active=0;
	console->port=-1;
	console->termstate=VTEKTYPE;
	console->innational = 0;			/* LU: no translation */
	console->incharset = -1;
	console->outnational = 0;
	console->outcharset = -1;
	UseResFile(TelInfo->SettingsFile);
	termHdl = (TerminalPrefs **)Get1NamedSizedResource
				(TERMINALPREFS_RESTYPE, "\p<Default>", sizeof(TerminalPrefs));
	DetachResource((Handle) termHdl);
	HLock((Handle)termHdl);

	RSsetcolors( console->vs, 0, &(*termHdl)->nfcolor);
	RSsetcolors( console->vs, 1, &(*termHdl)->nbcolor);
	RSsetcolors( console->vs, 2, &(*termHdl)->bfcolor);
	RSsetcolors( console->vs, 3, &(*termHdl)->bbcolor);

	DisposeHandle((Handle)termHdl);
#else
	console = NULL;
#endif
}

void putln( char *cp)
{
#ifdef DEBUG_FACILITIES
	short temp;
	if (!TelInfo->debug)
		return;

	temp=strlen(cp);
	if (temp>80) return;
	VSwrite(console->vs,cp,temp);
	VSwrite(console->vs,"\015\012",2);
#endif
}

void putlln( char *cp, short len )								
{
#ifdef DEBUG_FACILITIES
	short size;
	char buf[256];
	short temp;

	size = VSgetcols(console->vs) + 1;
	while ( size > 0 && len > 0 ) {
		short inlen = 0;
		short outlen = 0;
		temp = len;
		if ( temp > size ) {
			temp = size;
		}
		while (inlen < temp && outlen < size - 1) {
			if ( cp[inlen] == 0x0a ) {
				buf[outlen++] = 0x0d;
			}
			buf[outlen++] = cp[inlen++];
		}
		VSwrite(console->vs,buf,outlen);
		cp += inlen;
		len -= inlen;
	}
#endif
}

long dumpln(long base, char *dest, void *src, long len)
{
	unsigned long	baseAddr;
	register long	i;
	register long	j;
	long			offset;
	unsigned char	*bufSrc;
	char			*bufDst;
	char			hexBuf[39];		// temp buffer for hexa part of dump
	unsigned char	ascBuf[17];		// temp buffer for ascii part of dump
	char			tmpHex[5];  	// after conversion tmpHex[0 and 1] = code hexa
	char			fmtString[] = "  0x%08lx : %s  >%s<\n";
	char			outString[75];
	int				theByte;

	bufDst = (char *)dest;
	baseAddr = base;

	memset(hexBuf, ' ', 38);		// clear hexa buffer
	hexBuf[38] = ascBuf[16] = tmpHex[4] = '\0';
	bufSrc = (unsigned char *)src;
	j = 0;
	for (i = 0; i < len; i++) {
		if (j == 16) {
			if ( bufDst ) {
				bufDst += sprintf(bufDst, fmtString, baseAddr, hexBuf, ascBuf);
			} else {
				/*printf(fmtString, baseAddr, hexBuf, ascBuf);*/
				putlln( outString, sprintf(outString, fmtString, baseAddr, hexBuf, ascBuf) );
			}
			memset(hexBuf, ' ', 38);
			bufSrc += 16;
			baseAddr += 16;
			j = 0;
		}
		theByte = bufSrc[j];
		sprintf(tmpHex, "%02x", theByte);
		if (isprint(theByte)) {
			ascBuf[j] = theByte;
		} else {
			if (theByte == ' ')
				ascBuf[j] = theByte;
			else
				ascBuf[j] = '.';
		}
		if (j < 4)
			offset = j;
		else if (j > 3 && j < 8)
			offset = j + 1;
		else if (j > 7 && j < 12)
			offset = j + 2;
		else if (j > 11)
			offset = j + 3;
		hexBuf[2*offset]     = tmpHex[0];
		hexBuf[2*offset + 1] = tmpHex[1];
		j += 1;
	}
	// last line
	for (i = j; i < 16; i++)
		ascBuf[i] =  ' ';
	if ( bufDst ) {
		bufDst += sprintf(bufDst, fmtString, baseAddr, hexBuf, ascBuf);
	} else {
		/*printf(fmtString, baseAddr, hexBuf, ascBuf);*/
		putlln( outString, sprintf(outString, fmtString, baseAddr, hexBuf, ascBuf) );
	}
	return bufDst - (char *)dest;
}



// Called by HandleKeyDown.  Allows me to insert debug info keys all in one place
//	that can be easily #defined out for release versions.  Returns TRUE if
//	HandleKeyDown should do an immediate return after calling us.
Boolean	DebugKeys(Boolean cmddwn, unsigned char ascii, short s)
{
	Str255	scratchPstring;
#ifdef DEBUG_FACILITIES
	if (cmddwn && (ascii == ';')) {	// 2.6b16.1
		char hackhackhack[80];
		
		strcpy(hackhackhack, "Linemode: ");
		DemangleLineModeShort(hackhackhack, screens[s].lmodeBits);
		putln(hackhackhack);
		return(FALSE);
		}
	if (cmddwn && (ascii == 39)) { //single quote
		if (TelInfo->debug) HideDebugWindow();
		else ShowDebugWindow();

		GetIndString( scratchPstring, MISC_STRINGS, (TelInfo->debug) ? HIDE_LOG : SHOW_LOG);
		SetMenuItemText( myMenus[Conn], COshowlog, scratchPstring);

		return(FALSE);
	}
#endif
	return (FALSE);
}

void	ShowDebugWindow(void)
{
#ifdef DEBUG_FACILITIES
	
	if (console != NULL)
	{
		TelInfo->debug = TRUE;
		RSshow(console->vs);
	}
#endif
}

void	HideDebugWindow(void)
{
#ifdef DEBUG_FACILITIES
	
	if (console != NULL)
	{
		TelInfo->debug = FALSE;
		RShide(console->vs);
	}
#endif
}

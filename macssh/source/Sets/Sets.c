/*
*	Sets.c
*	All code relating to loading, parsing, and saving of sets
*
*****************************************************************
*  Revisions:
*  7/92		Telnet 2.6:	Code moved here from config.c and maclook.c...  Jim Browne
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#define SAVE_SET_STRINGS_COUNT 96

#include "macros.proto.h"
#include "wind.h"			/* For WindRec definition */
#include "Sets.proto.h"
#include "telneterrors.h"
#include "prefs.proto.h"
#include "Connections.proto.h"
#include "rsinterf.proto.h"
#include "rsmac.proto.h"
#include "vsdata.h"
#include "vsinterf.proto.h"
#include "rsdefs.h"
#include "DlogUtils.proto.h"

extern RSdata *RScurrent;

//#define	DEBUG_SETS
#ifdef	DEBUG_SETS
#define	sets_debug_print(x) putln(x)
#else
#define sets_debug_print(x)
#endif

ConnInitParams	**SetParams;
SessionPrefs	*SetSessionPtr;
TerminalPrefs	*SetTerminalPtr;

#define			kMaxSetLine 300

char			*Cspace;

extern WindRec
	*screens;		/* Window Records (VS) for :	Current Sessions */


extern MenuHandle
	myMenus[NMENUS];		/* Menu Handles .... */

#define PORTNUM 37			/* NCSA 2.5: the port variable */

void	SETSunload(void)	{}

/* affirmative() checks a token to see if it is a valid Affirmation string.
	We now get the affirmative strings from the resource */
	
Boolean	affirmative( char *s)
{
	short	i;
	Str255	AffWords;		/* Telnet 2.6: get string resources */

	for (i=0; i<TelInfo->position; i++)
				s[i] = tolower(s[i]);

	for (i=1; i<= AFF_WORDS_COUNT ;i++) 
		{
		GetIndString(AffWords,AFF_WORDS_ID,i);
		p2cstr(AffWords);
		if (!ncstrcmp((char *)AffWords,s))				
		break;										
		}
		
	if (i <= AFF_WORDS_COUNT)
		return(TRUE);
	else
		return(FALSE);
}

/**************************************************************************/
/*  Sissep
*   is the character a valid separator for the hosts file?
*   separators are white space, special chars and :;=
*
*/
Boolean Sissep( char c)
{
	if (c < 33)
		return(1);
	if (c == ':' || c == ';' || c == '=')
		return(TRUE);
	return(FALSE);
}

/************************************************************************/
/*  ncstrcmp
*   No case string compare.
*   Only returns 0=match, 1=no match, does not compare greater or less
*   There is a tiny bit of overlap with the | 32 trick, but shouldn't be
*   a problem.  It causes some different symbols to match.
*/
short ncstrcmp(char *sa, char *sb)
{
	while (*sa && *sa < 33)		/* don't compare leading spaces */
		sa++;
	while (*sb && *sb < 33)
		sb++;

	while (*sa && *sb) {
		if ((*sa != *sb) && ((*sa | 32) != (*sb | 32)))
			return(1);
		sa++;sb++;
	}
	if (!*sa && !*sb)		/* if both at end of string */
		return(0);
	else
		return(1);
}

/*
 * PStrFromC copies a C string into a pascal one
 */
static StringPtr PStrFromC( StringPtr destPString, const char *srcCString )
{
	int			size = 0;
	char		theChar;

	theChar = *srcCString++;
	while ( theChar != '\0' && size < 255 ) {
		char tempChar = *srcCString++;
		destPString[++size] = theChar;
		theChar = tempChar;
	}
	destPString[0] = size;
	return destPString;
}

/*
 * PStrFromNC copies a C string into a pascal one
 * limit the max output size
 */
static StringPtr PStrFromNC( StringPtr destPString, const char *srcCString, unsigned char n  )
{
	int			size = 0;
	char		theChar;

	theChar = *srcCString++;
	while ( theChar != '\0' && size < 255 && n--) {
		char tempChar = *srcCString++;
		destPString[++size] = theChar;
		theChar = tempChar;
	}
	destPString[0] = size;
	return destPString;
}



/* confile() now gets all of its keywords from the string resources, for greater
	ease, flexibility, and overall coolness.  */
short confile( char *s)
{
	short		i, port, portNeg;
	short		confState;
	Boolean		success;
	unsigned	int a,b,c,d;
	int			signedint;
	Str255		Ckeyw;
	char		tempCstring[256];
	char		*p;

	sets_debug_print(s);
	if ( !*s )
		return 0;

	confState = TelInfo->CONFstate;
	TelInfo->CONFstate = 0;

	switch ( confState) {
		case 0:				/* No keyword yet! */
			for ( i = 0; i < TelInfo->position; i++ )
				s[i] = tolower(s[i]);
			for ( i = 1; i <= SAVE_SET_STRINGS_COUNT ;i++ )  {
				GetIndString(Ckeyw, SAVE_SET_STRINGS_ID, i);
				p2cstr(Ckeyw);
				if (!ncstrcmp((char *)Ckeyw,s))
					break;
			}
			if ( i > SAVE_SET_STRINGS_COUNT ) {
				// ignore unknown labels
				//OperationFailedAlert(BAD_SET_ERR, 0, 0);
				//return(1);
				break;
			}
			if ( i == 5 ) {
				SetSessionPtr->bksp = 0;
			} else if ( i == 6 ) {
				SetSessionPtr->bksp = 1;
			} else {
				TelInfo->CONFstate = i;
			}
			break;

		case 1:				/* NAME */
			/* is there enough memory? */
			if ((p = myNewPtr(40000)) == NULL) {
				DoError(107 | MEMORY_ERRORCLASS, LEVEL2, NULL);	/* register the error */
				return -1;
			} else {
				DisposePtr(p);
			}
			if (TelInfo->CONFactive) {
				success = CreateConnectionFromParams(SetParams);
				SetParams = NULL;
				SetSessionPtr = NULL;
				SetTerminalPtr = NULL;
				TelInfo->CONFactive = 0;
				if (!success) {
					sets_debug_print("ERROR IN OPENING!! ");
					return 42;
				}
			}
			if (SetParams == NULL) {
				SetParams = ReturnDefaultConnInitParams();
				if ( SetParams ) {
					HLockHi((Handle)SetParams);
					HLockHi((Handle)(**SetParams).session);
					SetSessionPtr = *(**SetParams).session;
					HLockHi((Handle)(**SetParams).terminal);
					SetTerminalPtr = *(**SetParams).terminal;
				} else {
					DoError(107 | MEMORY_ERRORCLASS, LEVEL2, NULL);	/* register the error */
					return -1;
				}
			}
			PStrFromC(tempCstring, s);
			if ((tempCstring[0] == 1) && (tempCstring[1] == ' '))
				tempCstring[0] = 0;
			BlockMoveData(tempCstring, (**SetParams).WindowName, tempCstring[0]+1);
			TelInfo->CONFactive = 1;
			break;

		case 2:				/* HOST */
			PStrFromNC(tempCstring, s, sizeof(SetSessionPtr->hostname) - 1);
			//	Process the hosname string.
			if (ProcessHostnameString((StringPtr)tempCstring, &port, &portNeg))
				SetSessionPtr->port = port;
			
			BlockMoveData(tempCstring, SetSessionPtr->hostname, tempCstring[0]+1);

			break;

		case 3:				/* SIZE */
			if ( 4 != sscanf(s, "{%d,%d,%d,%d}", &a, &b, &c,&d) ) {
				sets_debug_print("Error in window size");
				return(2);
				}
			
			(**SetParams).WindowLocation.top=a;
			(**SetParams).WindowLocation.left=b;
			(**SetParams).WindowLocation.bottom=c;
			(**SetParams).WindowLocation.right=d;
			break;

		case 4:
			if ( 1 != sscanf(s,"%d", &a) ) {
				sets_debug_print("Scrollback needs parameter");
				return(1);
				}
			SetTerminalPtr->numbkscroll = a;
			break;

		case 5:
			SetSessionPtr->bksp = 0;			
			break;

		case 6:
			SetSessionPtr->bksp = 1;
			break;


/* NONO */
/* NONO: save all macros
		case 7:
			setmacro(&TelInfo->newMacros, 0, s);
			break;
		case 8:
			setmacro(&TelInfo->newMacros, 1, s);
			break;
		case 9:
			setmacro(&TelInfo->newMacros, 2, s);
			break;
		case 10:
			setmacro(&TelInfo->newMacros, 3, s);
			break;
		case 11:
			setmacro(&TelInfo->newMacros, 4, s);
			break;
		case 12:
			setmacro(&TelInfo->newMacros, 5, s);
			break;
		case 13:
			setmacro(&TelInfo->newMacros, 6, s);
			break;
		case 14:
			setmacro(&TelInfo->newMacros, 7, s);
			break;
		case 15:
			setmacro(&TelInfo->newMacros, 8, s);
			break;
		case 16:
			setmacro(&TelInfo->newMacros, 9, s);
			break;
*/
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			break;
/* NONO */

		case 17: /* commandkeys unused ? */
			break;
		case 18:
			if (!strcmp(s,"backspace") )
				SetSessionPtr->bksp = 0;
			else
				SetSessionPtr->bksp = 1;
			break;
		case 19:
		case 21:
			if ( 1 == sscanf(s,"%d", &a) ) 
				SetTerminalPtr->vtwidth = a;
			break;
		case 20:
			SetSessionPtr->tekclear = affirmative(s);
			break;
		case 22:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->nfcolor.red = a;
			SetTerminalPtr->nfcolor.green = b;
			SetTerminalPtr->nfcolor.blue = c;
			break;			
		case 23:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->nbcolor.red = a;
			SetTerminalPtr->nbcolor.green = b;
			SetTerminalPtr->nbcolor.blue = c;
			break;			
		case 24:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->bfcolor.red = a;
			SetTerminalPtr->bfcolor.green = b;
			SetTerminalPtr->bfcolor.blue = c;
			break;			
		case 25:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->bbcolor.red = a;
			SetTerminalPtr->bbcolor.green = b;
			SetTerminalPtr->bbcolor.blue = c;
			break;
		case 26:		/* Font Name */
			PStrFromNC(SetTerminalPtr->DisplayFont, s, sizeof(SetTerminalPtr->DisplayFont) - 1);
			break;
		case 27:		/* Font Size */
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->fontsize = a;
			break;
		case 28:		/* number of lines to use for window's editable region */
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->vtheight = a;
			break;
		case 29:		/* keystop, XOFF key */
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->skey = a;
			break;
		case 30:		/* keygo, XON key */
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->qkey = a;
			break;
		case 31:		/* keyip, kill key */
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ckey = a;
			break;
		case 32:		/* cr-map */
			if ((1 == sscanf( s, "%d", &a)) && (a !=0))
				SetSessionPtr->crmap = TRUE;
			else
				SetSessionPtr->crmap = FALSE;
			break;
		case 33:					/* BYU 2.4.9 */
			if ((1 == sscanf( s, "%d", &a)) && (a !=0))		/* BYU 2.4.9 */
				SetSessionPtr->linemode = TRUE;	/* BYU 2.4.9 */
			else					/* BYU 2.4.9 */
				SetSessionPtr->linemode = FALSE;	/* BYU 2.4.9 */
			break;					/* BYU 2.4.9 */
		case 34:					/* BYU 2.4.9 */
			if ((1 == sscanf( s, "%d", &a)) && (a !=0))
				SetTerminalPtr->eightbit = TRUE;	/* BYU 2.4.9 */
			else					/* BYU 2.4.9 */
				SetTerminalPtr->eightbit = FALSE;	/* BYU 2.4.9 */
			break;					/* BYU 2.4.9 */
		case 35:					/* BYU */
//			(**SetParams).ftpstate = 1;		/* BYU */
			break;					/* BYU */
		case 36:	// ignored
			break;
		case PORTNUM:							/* NCSA 2.5: get the real port # */
			if (1 == sscanf( s, "%d", &a))		/* NCSA */
				SetSessionPtr->port = a;				/* NCSA */
			break;								/* NCSA */
		case 38:	// translation
			PStrFromNC(SetSessionPtr->TranslationTable, s, sizeof(SetSessionPtr->TranslationTable) - 1);
			break;
		case 39:	// tekem
			if (1 == sscanf(s, "%d", &signedint))
				SetSessionPtr->tektype = signedint;
			break;
		case 40:	// answerback
			PStrFromNC(SetTerminalPtr->AnswerBackMessage, s, sizeof(SetTerminalPtr->AnswerBackMessage) - 1);
			break;
		case 41: // authenticate
			SetSessionPtr->authenticate = affirmative(s);
			break;
		case 42: // encrypt
			SetSessionPtr->encrypt = affirmative(s);		
			break;
		case 43: // pageup/down map
			SetTerminalPtr->MATmappings = affirmative(s);
			break;
		case 44: // keypad map
			SetTerminalPtr->remapKeypad = affirmative(s);
			break;
		case 45: // ansicolor
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->ANSIgraphics = a;
			break;
		case 46: // arrowmap
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->emacsarrows = a;
			break;
		case 47: // emacsmeta
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->emacsmetakey = a;
			break;
		case 48: // pastemethod
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->pastemethod = a;
			break;
		case 49: // pastesize
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->pasteblocksize = a;
			break;
		case 50: // saveclear
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->clearsave = a;
			break;
		case 51: // vtemulation
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->vtemulation = a;
			break;
		case 52: // forcesave
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->forcesave = a;
			break;
		case 53: // vtwrap
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->vtwrap = a;
			break;
		case 54: // xterm
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->Xtermsequences = a;
			break;
		case 55: // localecho
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->localecho = a;
			break;
		case 56: // halfdup
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->halfdup = a;
			break;
		case 57: // remapctrld
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->remapCtrlD = a;
			break;
		case 58: // negative
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->portNegative = a;
			break;
		case 59: // allowbold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->allowBold = a;
			break;
		case 60: // colorbold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->colorBold = a;
			break;
		case 61: // ignorebeeps
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ignoreBeeps = a;
			break;
		case 62: // ignoreff
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ignoreff = a;
			break;
		case 63: // boldfont
			PStrFromNC(SetTerminalPtr->BoldFont, s, sizeof(SetTerminalPtr->BoldFont) - 1);
			break;
		case 64: // inversebold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->boldFontStyle = a;
			break;
		case 65: // otpauto
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpauto = a;
			break;
		case 66: // otpmulti
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpmulti = a;
			break;
		case 67: // otphex
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otphex = a;
			break;
		case 68: // otpnoprompt
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpnoprompt = a;
			break;
		case 69: // otppassword
			PStrFromNC(SetSessionPtr->otppassword, s, sizeof(SetSessionPtr->otppassword) - 1);
			break;
		case 70: // realbold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->realbold = a;
			break;
		case 71: // otpsavepass
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpsavepass = a;
			break;
		case 72: // oldscrollback
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->oldScrollback = a;
			break;
/* NONO */
		case 73: // protocol
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->protocol = a;
			break;
		case 74: // encryption
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->encryption = a;
			break;
		case 75: // authentication
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->authentication = a;
			break;
		case 76: // compression
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->compression = a;
			break;
		case 77: // verbose
			SetSessionPtr->verbose = affirmative(s);
			break;
		case 78: // trace
			SetSessionPtr->trace = affirmative(s);
			break;
		case 79: // debug
			SetSessionPtr->debug = affirmative(s);
			break;
		case 80: // restricted
			SetSessionPtr->restricted = affirmative(s);
			break;
		case 81: // ssh2method
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ssh2method = a;
			break;
		case 82: // ssh2guests
			SetSessionPtr->ssh2guests = affirmative(s);
			break;
		case 83: // localport
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->localport = a;
			break;
		case 84: // remoteport
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->remoteport = a;
			break;
		case 85: // remotehost
			strncpy(SetSessionPtr->remotehost, s, sizeof(SetSessionPtr->remotehost) - 1);
			CtoPstr(SetSessionPtr->remotehost);
			break;
		case 86: // username
			PStrFromNC(SetSessionPtr->username, s, sizeof(SetSessionPtr->username) - 1);
			break;
/*
		case 87: // password
			PStrFromNC(SetSessionPtr->password, s, sizeof(SetSessionPtr->password) - 1);
			break;
*/
		case 88: // command
			PStrFromNC(SetSessionPtr->command, s, sizeof(SetSessionPtr->command) - 1);
			break;
/* NONO */
/* NONO: save all macros */
		case 89: // macro
			if (1 == sscanf( s, "%d", &a)) {
				while ((*s >= '0' && *s <= '9'))
					++s;
				if (*s) {
					++s;
					if (*s) {
						setmacro(&TelInfo->newMacros, a, s);
					}
				}
			}
			break;
		case 90: // jumpScroll
			SetTerminalPtr->jumpScroll = affirmative(s);
			break;
		case 91: // realBlink
			SetTerminalPtr->realBlink = affirmative(s);
			break;
		case 92: // vt7bits
			SetTerminalPtr->vt7bits = affirmative(s);
			break;
		case 93: // x11forward
			SetSessionPtr->x11forward = affirmative(s);
			break;
		case 94:
			SetSessionPtr->autoreconnect = affirmative(s);
			break;
		case 95: // forward
			SetSessionPtr->forward = affirmative(s);
			break;
		case 96: // keepselection
			SetSessionPtr->keepselection = affirmative(s);
			break;
			
/* NONO */
	}
	return(0);
} /* confile */


/************************************************************************/
/* contoken
*  tokenize the strings which get passed to confile.
*  Handles quotes and uses separators:  <33, ;:=
*/ 
short contoken(char c)
{
	short		retval;
	Boolean		success;

	if (c == EOF) {
		if ( TelInfo->position && Cspace[TelInfo->position - 1] == '"' )
			TelInfo->position--;
		Cspace[TelInfo->position++] = '\0';
		sets_debug_print("Eof handler called");
		confile( Cspace );
		if (TelInfo->CONFactive) {
			success = CreateConnectionFromParams(SetParams);
			if (!success) {
				sets_debug_print("ERROR IN OPENING!! ");
				return 42;
			}
		}
		return EOF;
	}
	
	if ( !TelInfo->position && !TelInfo->inquote && Sissep(c) )
		return 0;

	if ( TelInfo->position >= kMaxSetLine ) {
		sets_debug_print("Out of bounds error!");
		return 1;
	}

	if ( TelInfo->inquote || !Sissep(c) ) {
		/*
		*  check for quotes, a little mixed up here, could be reorganized
		*/
		if ( c == '"' ) {
			if ( !TelInfo->inquote ) {			/* beginning of quotes */
				TelInfo->inquote = 1;
			} else {
				Cspace[TelInfo->position++] = c;
			}
			return 0;
		} else if ( c != '\012' && c != '\015' ) {
			/* include in current string */
			Cspace[TelInfo->position++] = c;
			return 0;
		}
	}

	if ( TelInfo->position && Cspace[TelInfo->position - 1] == '"' )
		TelInfo->position--;

	Cspace[TelInfo->position++] = '\0';

	retval = confile(Cspace);			/* pass the token along */

	TelInfo->position = 0;
	TelInfo->inquote = 0;
	Cspace[0] = '\0';

	return retval;
}

/************************************************************************/
/*  readconfig
*   read the saved set file into our in-memory data structure.
*   Handle everything by keyword (stored in resources).
*/
void readconfig(FSSpec theSet)
{
	short c,retval;
	short fn;
	OSErr err;

	Cspace = myNewPtr(kMaxSetLine + 1);
	if ( Cspace == NULL ) {
		DoError(107 | MEMORY_ERRORCLASS, LEVEL2, NULL);	
		return;
	}

	SetParams = ReturnDefaultConnInitParams();
	if ( SetParams ) {
		HLockHi((Handle)SetParams);
		HLockHi((Handle)(**SetParams).session);
		SetSessionPtr = *(**SetParams).session;
		HLockHi((Handle)(**SetParams).terminal);
		SetTerminalPtr = *(**SetParams).terminal;
	} else {
		DisposePtr( (Ptr)Cspace );
		DoError(107 | MEMORY_ERRORCLASS, LEVEL2, NULL);	
		return;
	}

	TelInfo->position = TelInfo->CONFstate = TelInfo->CONFactive = TelInfo->inquote = TelInfo->lineno = 0;   /* state vars */	

	retval = 0;

	err = HOpenDF(theSet.vRefNum, theSet.parID, theSet.name, fsRdPerm, &fn);
	if ( err == noErr ) {
		while (!retval) {
			c = Myfgetc(fn);
			if (c == '#' && !TelInfo->inquote) {
				while (c != EOF && c != '\012' && c != '\015')		/* skip to EOL */	/* BYU 2.4.18 - changed \n to \015 and added \012*/
					c = Myfgetc(fn);
			}
			if (c == '\012' || c == '\015')			/* BYU 2.4.18 - changed \n to \015 and added \012 */
				TelInfo->lineno++;
			retval = contoken(c);
		}
		FSClose(fn);
	}
	DisposePtr((Ptr) Cspace);

	if (retval == EOF) {				/* EOF is normal end */
		sets_debug_print("EOF termination");
	} else {
		sets_debug_print("NON-EOF termination");
	}
		
	return;
}

void LoadSet( void)
{
	SFReply		sfr;
	long		junk;
	SFTypeList	typesok = {'CONF'};
	Point		where;
	FSSpec		set;
	Str255		tempString;
	where.h=100;where.v=100;

	GetIndString(tempString,MISC_STRINGS,SET_TO_LOAD_STRING);
	SFGetFile( where,tempString, 0L, 1, typesok, 0L, &sfr);

	if (! sfr.good) return;

	BlockMoveData(&sfr.fName, set.name, *sfr.fName + 1);
	GetWDInfo(sfr.vRefNum, &set.vRefNum, &set.parID, &junk);
	readconfig(set);
}

char Myfgetc(short myfile)
{
	OSErr err;
	long count;
	unsigned char buffer;
	
	count = 1;
	if ((err = FSRead(myfile, &count, &buffer)) == eofErr)
		buffer = EOF;
	
	return buffer;
}

/*
 * BoolItemToFile
 */
static OSErr BoolItemToFile(short refNum, short index, Boolean value) 
{	
	long		count;
	OSErr		theErr;
	Str255		tempStr;

	GetIndString( tempStr, SAVE_SET_STRINGS_ID, index );
	pstrcat( tempStr, (value) ? "\p = yes\015" : "\p = no\015" );
	count = tempStr[0];
	theErr = FSWrite( refNum, &count, (Ptr)tempStr + 1 );
	return theErr;
}

/*
 * IntItemToFile
 */
static OSErr IntItemToFile(short refNum, short index, int value) 
{	
	long		count;
	OSErr		theErr;
	Str255		tempStr;
	Str32		numStr;

	GetIndString( tempStr, SAVE_SET_STRINGS_ID, index );
	NumToString( value, numStr );
	pstrcat( tempStr, "\p = " );
	pstrcat( tempStr, numStr );
	pstrcat( tempStr, "\p\015" );
	count = tempStr[0];
	theErr = FSWrite( refNum, &count, (Ptr)tempStr + 1 );
	return theErr;
}

/*
 * CStringItemToFile
 */
static OSErr CStringItemToFile(short refNum, short index, char *value) 
{	
	long		len;
	long		count;
	OSErr		theErr;
	Str255		tempStr;
	Str32		numStr;

	len = strlen(value);
	if ( len ) {
		GetIndString( tempStr, SAVE_SET_STRINGS_ID, index );
		pstrcat( tempStr, "\p = \"" );
		count = tempStr[0];
		theErr = FSWrite( refNum, &count, (Ptr)tempStr + 1 );
		if ( !theErr ) {
			count = len;
			theErr = FSWrite( refNum, &count, value );
			if ( !theErr ) {
				count = 2;
				theErr = FSWrite( refNum, &count, "\"\015" );
			}
		}
	} else {
		theErr = noErr;
	}
	return theErr;
}

/*
 * PStringItemToFile
 */
static OSErr PStringItemToFile(short refNum, short index, StringPtr value) 
{	
	long		count;
	OSErr		theErr;
	Str255		tempStr;
	Str32		numStr;

	if ( value[0] ) {
		GetIndString( tempStr, SAVE_SET_STRINGS_ID, index );
		pstrcat( tempStr, "\p = \"" );
		count = tempStr[0];
		theErr = FSWrite( refNum, &count, (Ptr)tempStr + 1 );
		if ( !theErr ) {
			count = value[0];
			theErr = FSWrite( refNum, &count, (Ptr)value + 1 );
			if ( !theErr ) {
				count = 2;
				theErr = FSWrite( refNum, &count, "\"\015" );
			}
		}
	} else {
		theErr = noErr;
	}
	return theErr;
}


void SaveSet(short doSaveMacros, short dontSaveTitle)
{
	Point		where;
	Str255		scratchPstring;
	Str255		scratchPstring2;
	SFReply		sfr;
	FSSpec		set;
	short		fn;
	short		truncate;
	OSErr		err;
	WindowPeek	wpeek;
	Rect		rect;
	long		junk;
	char		temp[256];
	char		temp2[300];
	short		fnum,fsiz;
	short		i;
	
	where.h = 100; where.v = 100;
	GetIndString(scratchPstring, MISC_STRINGS, SAVE_SET_STRING);
	GetIndString(scratchPstring2, MISC_STRINGS, DEFAULT_SAVE_SET_NAME);

	SFPutFile(where, scratchPstring, scratchPstring2, 0L, &sfr);

	if (!sfr.good)
		return;

	BlockMoveData(&sfr.fName, set.name, *sfr.fName + 1);
	GetWDInfo(sfr.vRefNum, &set.vRefNum, &set.parID, &junk);

	if ((err = HCreate(set.vRefNum, set.parID, set.name, kNCSACreatorSignature, kNCSASetFileType)) == dupFNErr)
		truncate = 1;

	err = HOpenDF(set.vRefNum, set.parID, set.name, fsWrPerm, &fn);
	if ( err ) {
		// FIXME: add error message
		return;
	}

	if (truncate) 
		SetEOF(fn, 0L);

	BoolItemToFile(fn, 17, gApplicationPrefs->CommandKeys);

	if (doSaveMacros) {
		// save all macros
		for (i = 0; i < NUM_MACROS; i++) {
			getmacro(&TelInfo->newMacros, i, temp, sizeof(temp));
			if (*temp) {
				sprintf(temp2, "%d %s", i, temp);
				CStringItemToFile(fn, 89, temp2);			// macro
			}
		}
	}

	for (i = 0; i < TelInfo->numwindows; i++) {
		WindRec *tw = &screens[i];
		scratchPstring[0] = '\0';
	  	if ( !dontSaveTitle )
		  	GetWTitle( tw->wind, scratchPstring );
		PStringItemToFile(fn, 1, scratchPstring);			// name
		PStringItemToFile(fn, 2, tw->machine);				// host
		IntItemToFile(fn, 37, tw->portNum);					// port
		IntItemToFile(fn, 4, tw->maxscroll);				// scrollback
		CStringItemToFile(fn, 18, (tw->bsdel) ? "delete" : "backspace"); // erase
		if ( !dontSaveTitle ) {
			wpeek = (WindowPeek) tw->wind;
			rect = (*wpeek->contRgn)->rgnBBox;
			sprintf( temp2, "{%d,%d,%d,%d}", rect.top, rect.left, rect.bottom, rect.right );
			CStringItemToFile(fn, 3, temp2);				// size
		}
		IntItemToFile(fn, 21, VSmaxwidth(tw->vs) + 1);		// vtwidth
		BoolItemToFile(fn, 20, tw->tekclear);				// tekclear
		if (TelInfo->haveColorQuickDraw) {
			RGBColor col;
			short j;
			for (j = 0; j < 4; j++) {
				RSgetcolors( tw->vs, j, &col);
				sprintf( temp2, "{%u,%u,%u}", col.red, col.green, col.blue );
				CStringItemToFile(fn, 22 + j, temp2);		// rgb0..3
			}
		}
		RSgetfont( tw->vs, &fnum, &fsiz );
		GetFontName( fnum, scratchPstring );
		PStringItemToFile(fn, 26, scratchPstring);			// font
		IntItemToFile(fn, 27, fsiz);						// fsize
		fnum = RScurrent->bfnum;
		GetFontName( fnum, scratchPstring );
		PStringItemToFile(fn, 63, scratchPstring);			// boldfont
		IntItemToFile(fn, 28, VSgetlines(tw->vs));			// nlines
		IntItemToFile(fn, 29, tw->TELstop);					// keystop
		IntItemToFile(fn, 30, tw->TELgo);					// keygo
		IntItemToFile(fn, 31, tw->TELip);					// keyip
		IntItemToFile(fn, 32, tw->crmap);					// crmap
		IntItemToFile(fn, 39, tw->tektype);					// tekem

// All this stuff was added in BetterTelnet 1.0b3... (RAB)

		IntItemToFile(fn, 51, tw->vtemulation);				// vtemulation
		IntItemToFile(fn, 52, tw->forcesave);				// forcesave
		IntItemToFile(fn, 34, tw->eightbit);				// eightbit
		IntItemToFile(fn, 53, tw->wrap);					// vtwrap
		IntItemToFile(fn, 54, tw->Xterm);					// xterm
		IntItemToFile(fn, 57, tw->remapCtrlD);				// remapctrld
		IntItemToFile(fn, 45, tw->ANSIgraphics);			// ansicolor
		IntItemToFile(fn, 46, tw->arrowmap);				// arrowmap
		IntItemToFile(fn, 47, tw->emacsmeta);				// emacsmeta
		IntItemToFile(fn, 48, tw->pastemethod);				// pastemethod
		IntItemToFile(fn, 49, tw->pastesize);				// pastesize
		IntItemToFile(fn, 50, tw->ESscroll);				// saveclear
		IntItemToFile(fn, 33, tw->lineAllow);				// linemode

// And all this stuff was added a bit later... (RAB BetterTelnet 1.0 and later)

		IntItemToFile(fn, 58, tw->portNegative);			// negative
		IntItemToFile(fn, 59, tw->allowBold);				// allowbold
		IntItemToFile(fn, 60, tw->colorBold);				// colorbold
		IntItemToFile(fn, 61, tw->ignoreBeeps);				// ignorebeeps
		IntItemToFile(fn, 62, tw->ignoreff);				// ignoreff
		IntItemToFile(fn, 64, tw->inversebold);				// inversebold
		IntItemToFile(fn, 65, tw->otpauto);					// otpauto
		IntItemToFile(fn, 66, tw->otpmulti);				// otpmulti
		IntItemToFile(fn, 67, tw->otphex);					// otphex
		IntItemToFile(fn, 68, tw->otpnoprompt);				// otpnoprompt
		IntItemToFile(fn, 70, tw->realbold);				// realbold
		IntItemToFile(fn, 72, tw->oldScrollback);			// oldscrollback
		IntItemToFile(fn, 71, tw->otpsavepass);				// otpsavepass

// end of "all this stuff"

		PStringItemToFile(fn, 69, tw->otppassword);			// otppassword
		GetMenuItemText( myMenus[National], tw->outnational + 1, scratchPstring);
		PStringItemToFile(fn, 38, scratchPstring );			// translation
		PStringItemToFile(fn, 40, tw->answerback );			// answerback
		BoolItemToFile(fn, 41, tw->authenticate);			// authenticate
		BoolItemToFile(fn, 42, tw->encrypt);				// encrypt
		BoolItemToFile(fn, 43, tw->pgupdwn);				// pageup
		BoolItemToFile(fn, 44, tw->keypadmap);				// keypad
/* NONO */
		BoolItemToFile(fn, 90, tw->jumpScroll);				// jumpscroll
		BoolItemToFile(fn, 91, tw->realBlink);				// realblink
		BoolItemToFile(fn, 92, VSisvt7bit(tw->vs));			// vt7bits

		IntItemToFile(fn, 73, tw->protocol);				// protocol
		IntItemToFile(fn, 74, tw->encryption);				// encryption
		IntItemToFile(fn, 75, tw->authentication);			// authentication
		IntItemToFile(fn, 76, tw->compression);				// compression
		BoolItemToFile(fn, 77, tw->verbose);				// verbose
		BoolItemToFile(fn, 78, tw->trace);					// trace
		BoolItemToFile(fn, 79, tw->debug);					// debug
		BoolItemToFile(fn, 80, tw->restricted);				// restricted
		IntItemToFile(fn, 81, tw->ssh2method);				// ssh2method
		BoolItemToFile(fn, 82, tw->ssh2guests);				// ssh2guests
		IntItemToFile(fn, 83, tw->localport);				// localport
		IntItemToFile(fn, 84, tw->remoteport);				// remoteport
		PStringItemToFile(fn, 85, tw->remotehost );			// remotehost
		PStringItemToFile(fn, 86, tw->username );			// username
		//PStringItemToFile(fn, 87, tw->password );			// password
		PStringItemToFile(fn, 88, tw->command );			// command
		BoolItemToFile(fn, 93, tw->x11forward );			// x11forward
		BoolItemToFile(fn, 94, tw->autoreconnect );			// autoreconnect
		BoolItemToFile(fn, 95, tw->forward );				// forward
		BoolItemToFile(fn, 96, tw->keepselection );			// keepselection

/* NONO */

	} /* for i */

	FSClose(fn);
}

// This is new for BetterTelnet 1.0b4. It saves sessions directly from the Favorites. (RAB)
void SaveSetFromSession(SessionPrefs* setSession, TerminalPrefs* setTerminal, short doWeAppend, short doSaveMacros)
{
	Point		where;
	Str255		scratchPstring;
	Str255		scratchPstring2;
	SFTypeList	typesok = {'CONF'};
	SFReply		sfr;
	FSSpec		set;
	short		fn;
	short		truncate;
	OSErr		err;
	WindowPeek	wpeek;
	Rect		rect;
	long		junk;
	char		temp[256];
	char		temp2[300];
	short		fnum,fsiz;
	short		i;

	where.h = 100; where.v = 100;
	if (doWeAppend) {
		GetIndString( scratchPstring, MISC_STRINGS, SAVE_SET_STRING );
		SFGetFile( where,scratchPstring, 0L, 1, typesok, 0L, &sfr);
	} else {
		GetIndString( scratchPstring, MISC_STRINGS, SAVE_SET_STRING );
		GetIndString( scratchPstring2, MISC_STRINGS, DEFAULT_SAVE_SET_NAME);
		SFPutFile( where,scratchPstring, scratchPstring2, 0L, &sfr );
	}

	if (!sfr.good)
		return;

	BlockMoveData(&sfr.fName, set.name, *sfr.fName + 1);
	GetWDInfo(sfr.vRefNum, &set.vRefNum, &set.parID, &junk);

	if (!doWeAppend)
		if ((err = HCreate(set.vRefNum, set.parID, set.name, kNCSACreatorSignature, kNCSASetFileType)) == dupFNErr)
			truncate = 1;

	err = HOpenDF(set.vRefNum, set.parID, set.name, fsWrPerm, &fn);
	if ( err ) {
		// FIXME: add error message
		return;
	}

	if (!doWeAppend && truncate) 
		SetEOF(fn, 0L);

	if (doWeAppend)
		SetFPos(fn, 2, 0L);

	if (doSaveMacros) {
		// save all macros
		for (i = 0; i < NUM_MACROS; i++) {
			getmacro(&TelInfo->newMacros, i, temp, sizeof(temp));
			if (*temp) {
				sprintf(temp2, "%d %s", i, temp);
				CStringItemToFile(fn, 89, temp2);			// macro
			}
		}
	}

	PStringItemToFile(fn, 1, "\p ");						// name
	PStringItemToFile(fn, 2, setSession->hostname);			// host
	IntItemToFile(fn, 37, setSession->port);				// port
	IntItemToFile(fn, 4, setTerminal->numbkscroll);			// scrollback
	CStringItemToFile(fn, 18, (setSession->bksp) ? "delete" : "backspace"); // erase
	IntItemToFile(fn, 21, setTerminal->vtwidth);			// vtwidth
	BoolItemToFile(fn, 20, setSession->tekclear);			// tekclear
	if ( TelInfo->haveColorQuickDraw ) {
		sprintf(temp2, "{%u,%u,%u}", setTerminal->nfcolor.red,
			setTerminal->nfcolor.green, setTerminal->nfcolor.blue);
		CStringItemToFile(fn, 22, temp2);					// rgb0
		sprintf(temp2, "{%u,%u,%u}", setTerminal->nbcolor.red,
			setTerminal->nbcolor.green, setTerminal->nbcolor.blue);
		CStringItemToFile(fn, 23, temp2);					// rgb1
		sprintf(temp2, "{%u,%u,%u}", setTerminal->bfcolor.red,
			setTerminal->bfcolor.green, setTerminal->bfcolor.blue);
		CStringItemToFile(fn, 24, temp2);					// rgb2
		sprintf(temp2, "{%u,%u,%u}", setTerminal->bbcolor.red,
			setTerminal->bbcolor.green, setTerminal->bbcolor.blue);
		CStringItemToFile(fn, 25, temp2);					// rgb3
	}

	PStringItemToFile(fn, 26, setTerminal->DisplayFont);	// font
	IntItemToFile(fn, 27, setTerminal->fontsize);			// fsize
	PStringItemToFile(fn, 63, setTerminal->BoldFont);		// boldfont
	IntItemToFile(fn, 28, setTerminal->vtheight);		// nlines
	IntItemToFile(fn, 29, setSession->skey);				// keystop
	IntItemToFile(fn, 30, setSession->qkey);				// keygo
	IntItemToFile(fn, 31, setSession->ckey);				// keyip
	IntItemToFile(fn, 32, setSession->crmap);				// crmap
	IntItemToFile(fn, 39, setSession->tektype);				// tekem

// All this stuff was added in BetterTelnet 1.0b3... (RAB)

	IntItemToFile(fn, 51, setTerminal->vtemulation);		// vtemulation
	IntItemToFile(fn, 52, setSession->forcesave);			// forcesave
	IntItemToFile(fn, 34, setTerminal->eightbit);			// eightbit
	IntItemToFile(fn, 53, setTerminal->vtwrap);				// vtwrap
	IntItemToFile(fn, 54, setTerminal->Xtermsequences);		// xterm
	IntItemToFile(fn, 57, setTerminal->remapCtrlD);			// remapctrld
	IntItemToFile(fn, 45, setTerminal->ANSIgraphics);		// ansicolor
	IntItemToFile(fn, 46, setTerminal->emacsarrows);		// arrowmap
	IntItemToFile(fn, 47, setTerminal->emacsmetakey);		// emacsmeta
	IntItemToFile(fn, 48, setSession->pastemethod);			// pastemethod
	IntItemToFile(fn, 49, setSession->pasteblocksize);		// pastesize
	IntItemToFile(fn, 50, setTerminal->clearsave);			// saveclear
	IntItemToFile(fn, 33, setSession->linemode);			// linemode
	IntItemToFile(fn, 58, setSession->portNegative);		// negative
	IntItemToFile(fn, 59, setTerminal->allowBold);			// allowbold
	IntItemToFile(fn, 60, setTerminal->colorBold);			// colorbold
	IntItemToFile(fn, 61, setSession->ignoreBeeps);			// ignorebeeps
	IntItemToFile(fn, 62, setSession->ignoreff);			// ignoreff
	IntItemToFile(fn, 64, setTerminal->boldFontStyle);		// inversebold

// And all this stuff was added in 1.0b5... (RAB)
	IntItemToFile(fn, 55, setSession->localecho);			// localecho
	IntItemToFile(fn, 56, setSession->halfdup);				// halfdup

// end of "all this stuff" (RAB)

	IntItemToFile(fn, 65, setSession->otpauto);				// otpauto
	IntItemToFile(fn, 66, setSession->otpmulti);			// otpmulti
	IntItemToFile(fn, 67, setSession->otphex);				// otphex
	IntItemToFile(fn, 68, setSession->otpnoprompt);			// otpnoprompt
	IntItemToFile(fn, 70, setTerminal->realbold);			// realbold
	IntItemToFile(fn, 72, setTerminal->oldScrollback);		// oldscrollback
	IntItemToFile(fn, 71, setSession->otpsavepass);			// otpsavepass
	if ( setSession->otppassword[0] ) {
		PStringItemToFile(fn, 69, setSession->otppassword);	// otppassword
	}
	PStringItemToFile(fn, 38, setSession->TranslationTable ); // translation
	PStringItemToFile(fn, 40, setTerminal->AnswerBackMessage ); // answerback
	BoolItemToFile(fn, 41, setSession->authenticate);		// authenticate
	BoolItemToFile(fn, 42, setSession->encrypt);			// encrypt
	BoolItemToFile(fn, 43, setTerminal->MATmappings);		// pageup
	BoolItemToFile(fn, 44, setTerminal->remapKeypad);		// keypad
/* NONO */
	BoolItemToFile(fn, 90, setTerminal->jumpScroll);		// jumpscroll
	BoolItemToFile(fn, 91, setTerminal->realBlink);			// realblink
	BoolItemToFile(fn, 92, setTerminal->vt7bits);			// vt7bits

	IntItemToFile(fn, 73, setSession->protocol);			// protocol
	IntItemToFile(fn, 74, setSession->encryption);			// encryption
	IntItemToFile(fn, 75, setSession->authentication);		// authentication
	IntItemToFile(fn, 76, setSession->compression);			// compression
	BoolItemToFile(fn, 77, setSession->verbose);			// verbose
	BoolItemToFile(fn, 78, setSession->trace);				// trace
	BoolItemToFile(fn, 79, setSession->debug);				// debug
	BoolItemToFile(fn, 80, setSession->restricted);			// restricted
	IntItemToFile(fn, 81, setSession->ssh2method);			// ssh2method
	BoolItemToFile(fn, 82, setSession->ssh2guests);			// ssh2guests
	IntItemToFile(fn, 83, setSession->localport);			// localport
	IntItemToFile(fn, 84, setSession->remoteport);			// remoteport
	PStringItemToFile(fn, 85, setSession->remotehost );		// remotehost
	PStringItemToFile(fn, 86, setSession->username );		// username
	//PStringItemToFile(fn, 87, setSession->password );		// password
	PStringItemToFile(fn, 88, setSession->command );		// command
	BoolItemToFile(fn, 93, setSession->x11forward );		// x11forward
	BoolItemToFile(fn, 94, setSession->autoreconnect );		// autoreconnect
	BoolItemToFile(fn, 95, setSession->forward );			// forward
	BoolItemToFile(fn, 96, setSession->keepselection );	// keepselection

/* NONO */

  	FSClose(fn);
}

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

#define SAVE_SET_STRINGS_COUNT 89

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

/* confile() now gets all of its keywords from the string resources, for greater
	ease, flexibility, and overall coolness.  */
short confile( char *s)
{
	short		i, port, portNeg;
	Boolean		success;
	unsigned	int a,b,c,d;
	int			signedint;
	Str255		Ckeyw;
	char		tempCstring[256];

	sets_debug_print(s);
	if (!(*s) )
		return(0);

	switch( TelInfo->CONFstate) {
		case 0:				/* No keyword yet! */
			for (i=0; i<TelInfo->position; i++)
				s[i] = tolower(s[i]);
			for (i=1; i<= SAVE_SET_STRINGS_COUNT ;i++) 
				{
				GetIndString(Ckeyw,SAVE_SET_STRINGS_ID,i);
				p2cstr(Ckeyw);
				if (!ncstrcmp((char *)Ckeyw,s))				
				break;										
				}

			if ( i > SAVE_SET_STRINGS_COUNT ) 
				{
//				OperationFailedAlert(BAD_SET_ERR, 0, 0);
				TelInfo->CONFstate = 0;
				break;
//				return(1);
				}

			TelInfo->CONFstate=i;

			if (TelInfo->CONFstate==5) {
				SetSessionPtr->bksp = 0;
				TelInfo->CONFstate=0;
				}
			if (TelInfo->CONFstate==6) {
				SetSessionPtr->bksp = 1;
				TelInfo->CONFstate=0;
				}
			break;

		case 1:				/* NAME */
			{ char *p;
			if (NULL == (p = myNewPtr(40000))) 		/* is there enough memory? */
				{					/* NOT enough memory for the set! */
				DoError(107 | MEMORY_ERRORCLASS, LEVEL2, NULL);	/* register the error */
				return(-1);
				}
			else
				DisposePtr(p);
			}
			if (TelInfo->CONFactive) {
				success = CreateConnectionFromParams(SetParams);
				SetParams = NULL;
				if (!success) {
					sets_debug_print("ERROR IN OPENING!! ");
					return(42);
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
					return(-1);
				}
			}
			strncpy(tempCstring, s, 255);			/* Move name in */
			CtoPstr(tempCstring);
			if ((tempCstring[0] == 1) && (tempCstring[1] == ' '))
				tempCstring[0] = 0;
			BlockMoveData(tempCstring, (**SetParams).WindowName, tempCstring[0]+1);
			TelInfo->CONFstate=0;
			TelInfo->CONFactive=1;
			break;

		case 2:				/* HOST */
			strncpy(tempCstring, s, 63);			/* Move name in */
			CtoPstr(tempCstring);

			//	Process the hosname string.
			if (ProcessHostnameString((StringPtr)tempCstring, &port, &portNeg))
				SetSessionPtr->port = port;
			
			BlockMoveData(tempCstring, SetSessionPtr->hostname, tempCstring[0]+1);

			TelInfo->CONFstate=0;
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
			TelInfo->CONFstate=0;
			break;

		case 4:
			if ( 1 != sscanf(s,"%d", &a) ) {
				sets_debug_print("Scrollback needs parameter");
				return(1);
				}
			SetTerminalPtr->numbkscroll = a;
			TelInfo->CONFstate=0;
			break;

		case 5:
			SetSessionPtr->bksp = 0;			
			TelInfo->CONFstate=0;
			break;

		case 6:
			SetSessionPtr->bksp = 1;
			TelInfo->CONFstate=0;
			break;


/* NONO */
/* NONO: save all macros
		case 7:
			setmacro(&TelInfo->newMacros, 0, s);
			TelInfo->CONFstate=0;
			break;
		case 8:
			setmacro(&TelInfo->newMacros, 1, s);
			TelInfo->CONFstate=0;
			break;
		case 9:
			setmacro(&TelInfo->newMacros, 2, s);
			TelInfo->CONFstate=0;
			break;
		case 10:
			setmacro(&TelInfo->newMacros, 3, s);
			TelInfo->CONFstate=0;
			break;
		case 11:
			setmacro(&TelInfo->newMacros, 4, s);
			TelInfo->CONFstate=0;
			break;
		case 12:
			setmacro(&TelInfo->newMacros, 5, s);
			TelInfo->CONFstate=0;
			break;
		case 13:
			setmacro(&TelInfo->newMacros, 6, s);
			TelInfo->CONFstate=0;
			break;
		case 14:
			setmacro(&TelInfo->newMacros, 7, s);
			TelInfo->CONFstate=0;
			break;
		case 15:
			setmacro(&TelInfo->newMacros, 8, s);
			TelInfo->CONFstate=0;
			break;
		case 16:
			setmacro(&TelInfo->newMacros, 9, s);
			TelInfo->CONFstate=0;
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
			TelInfo->CONFstate=0;		// Now ignored (was macros 0 to 9)
			break;
/* NONO */

		case 17:
			TelInfo->CONFstate=0;		// Now ignored (was commandkeys)
			break;
		case 18:
			if (!strcmp(s,"backspace") )
				SetSessionPtr->bksp = 0;
			else
				SetSessionPtr->bksp = 1;
			TelInfo->CONFstate=0;
			break;
		case 19:
		case 21:
			if ( 1 == sscanf(s,"%d", &a) ) 
				SetTerminalPtr->vtwidth = a;
			TelInfo->CONFstate=0;
			break;
		case 20:
			if (affirmative(s))
				SetSessionPtr->tekclear = 1;
			else
				SetSessionPtr->tekclear = 0;
			TelInfo->CONFstate = 0;
			break;
		case 22:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->nfcolor.red = a;
			SetTerminalPtr->nfcolor.green = b;
			SetTerminalPtr->nfcolor.blue = c;
			TelInfo->CONFstate = 0;
			break;			
		case 23:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->nbcolor.red = a;
			SetTerminalPtr->nbcolor.green = b;
			SetTerminalPtr->nbcolor.blue = c;
			TelInfo->CONFstate = 0;
			break;			
		case 24:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->bfcolor.red = a;
			SetTerminalPtr->bfcolor.green = b;
			SetTerminalPtr->bfcolor.blue = c;
			TelInfo->CONFstate = 0;
			break;			
		case 25:
			if ( 3 != sscanf(s, "{%u,%u,%u}", &a, &b, &c)) {	/* BYU LSC - "%d" changed to "%u" */
				sets_debug_print("Bad Parms to rgb");
				return(2);
				}
			SetTerminalPtr->bbcolor.red = a;
			SetTerminalPtr->bbcolor.green = b;
			SetTerminalPtr->bbcolor.blue = c;
			TelInfo->CONFstate = 0;
			break;
		case 26:		/* Font Name */
			strncpy(tempCstring, s, 63);			/* Move name in */
			CtoPstr(tempCstring);
			BlockMoveData(tempCstring, &(SetTerminalPtr->DisplayFont[0]), tempCstring[0]+1);
			TelInfo->CONFstate = 0;
			break;
		case 27:		/* Font Size */
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->fontsize = a;
			TelInfo->CONFstate = 0;
			break;
		case 28:		/* number of lines to use for window's editable region */
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->vtheight = a;
			TelInfo->CONFstate = 0;
			break;
		case 29:		/* keystop, XOFF key */
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->skey = a;
			TelInfo->CONFstate = 0;
			break;
		case 30:		/* keygo, XON key */
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->qkey = a;
			TelInfo->CONFstate = 0;
			break;
		case 31:		/* keyip, kill key */
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ckey = a;
			TelInfo->CONFstate = 0;
			break;
		case 32:		/* cr-map */
			if ((1 == sscanf( s, "%d", &a)) && (a !=0))
				SetSessionPtr->crmap = TRUE;
			else
				SetSessionPtr->crmap = FALSE;
			TelInfo->CONFstate = 0;
			break;
		case 33:					/* BYU 2.4.9 */
			if ((1 == sscanf( s, "%d", &a)) && (a !=0))		/* BYU 2.4.9 */
				SetSessionPtr->linemode = TRUE;	/* BYU 2.4.9 */
			else					/* BYU 2.4.9 */
				SetSessionPtr->linemode = FALSE;	/* BYU 2.4.9 */
			TelInfo->CONFstate=0;			/* BYU 2.4.9 */
			break;					/* BYU 2.4.9 */
		case 34:					/* BYU 2.4.9 */
			if ((1 == sscanf( s, "%d", &a)) && (a !=0))
				SetTerminalPtr->eightbit = TRUE;	/* BYU 2.4.9 */
			else					/* BYU 2.4.9 */
				SetTerminalPtr->eightbit = FALSE;	/* BYU 2.4.9 */
			TelInfo->CONFstate=0;			/* BYU 2.4.9 */
			break;					/* BYU 2.4.9 */
		case 35:					/* BYU */
//			(**SetParams).ftpstate = 1;		/* BYU */
			TelInfo->CONFstate=0;			/* BYU */
			break;					/* BYU */
		case 36:	// ignored
			TelInfo->CONFstate=0;
			break;
		case PORTNUM:							/* NCSA 2.5: get the real port # */
			if (1 == sscanf( s, "%d", &a))		/* NCSA */
				SetSessionPtr->port = a;				/* NCSA */
			TelInfo->CONFstate = 0;				/* NCSA */
			break;								/* NCSA */
		case 38:	// translation
			strncpy((char *) SetSessionPtr->TranslationTable, s, 32);
			CtoPstr((char *) SetSessionPtr->TranslationTable);
			TelInfo->CONFstate=0;
			break;
		case 39:	// tekem
			if (1 == sscanf(s, "%d", &signedint))
				SetSessionPtr->tektype = signedint;
			TelInfo->CONFstate=0;
			break;
		case 40:	// answerback
			strncpy((char *) SetTerminalPtr->AnswerBackMessage, s, 32);
			CtoPstr((char *) SetTerminalPtr->AnswerBackMessage);
			TelInfo->CONFstate=0;
			break;
		case 41: //authenticate
			if (affirmative(s))		
				SetSessionPtr->authenticate = TRUE;
			else				
				SetSessionPtr->authenticate = FALSE;	
			TelInfo->CONFstate=0;		
			break;
		case 42: //encrypt
			if (affirmative(s))		
				SetSessionPtr->encrypt = TRUE;	
			else					
				SetSessionPtr->encrypt = FALSE;	
			TelInfo->CONFstate=0;			
			break;
		
		case 43: //pageup/down map
			if (affirmative(s))
				SetTerminalPtr->MATmappings = TRUE;
			else					
				SetTerminalPtr->MATmappings = FALSE;
			TelInfo->CONFstate=0;	
			break;
		case 44: //keypad map
			if (affirmative(s))
				SetTerminalPtr->remapKeypad = TRUE;
			else					
				SetTerminalPtr->remapKeypad = FALSE;
			TelInfo->CONFstate=0;	
			break;
		case 45: // ansicolor
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->ANSIgraphics = a;
			TelInfo->CONFstate=0;
			break;
		case 46: // arrowmap
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->emacsarrows = a;
			TelInfo->CONFstate=0;
			break;
		case 47: // emacsmeta
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->emacsmetakey = a;
			TelInfo->CONFstate=0;
			break;
		case 48: // pastemethod
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->pastemethod = a;
			TelInfo->CONFstate=0;
			break;
		case 49: // pastesize
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->pasteblocksize = a;
			TelInfo->CONFstate=0;
			break;
		case 50: // saveclear
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->clearsave = a;
			TelInfo->CONFstate=0;
			break;
		case 51: // vtemulation
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->vtemulation = a;
			TelInfo->CONFstate=0;
			break;
		case 52: // forcesave
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->forcesave = a;
			TelInfo->CONFstate=0;
			break;
		case 53: // vtwrap
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->vtwrap = a;
			TelInfo->CONFstate=0;
			break;
		case 54: // xterm
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->Xtermsequences = a;
			TelInfo->CONFstate=0;
			break;
		case 55: // localecho
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->localecho = a;
			TelInfo->CONFstate=0;
			break;
		case 56: // halfdup
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->halfdup = a;
			TelInfo->CONFstate=0;
			break;
		case 57: // remapctrld
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->remapCtrlD = a;
			TelInfo->CONFstate=0;
			break;
		case 58: // negative
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->portNegative = a;
			TelInfo->CONFstate=0;
			break;
		case 59: // allowbold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->allowBold = a;
			TelInfo->CONFstate=0;
			break;
		case 60: // colorbold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->colorBold = a;
			TelInfo->CONFstate=0;
			break;
		case 61: // ignorebeeps
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ignoreBeeps = a;
			TelInfo->CONFstate=0;
			break;
		case 62: // ignoreff
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ignoreff = a;
			TelInfo->CONFstate=0;
			break;
		case 63: // boldfont
			strncpy(tempCstring, s, 63);			/* Move name in */
			CtoPstr(tempCstring);
			BlockMoveData(tempCstring, &(SetTerminalPtr->BoldFont[0]), tempCstring[0]+1);
			TelInfo->CONFstate = 0;
			break;
		case 64: // inversebold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->boldFontStyle = a;
			TelInfo->CONFstate = 0;
			break;
		case 65: // otpauto
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpauto = a;
			TelInfo->CONFstate = 0;
			break;
		case 66: // otpmulti
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpmulti = a;
			TelInfo->CONFstate = 0;
			break;
		case 67: // otphex
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otphex = a;
			TelInfo->CONFstate = 0;
			break;
		case 68: // otpnoprompt
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpnoprompt = a;
			TelInfo->CONFstate = 0;
			break;
		case 69: // otppassword
			strncpy(tempCstring, s, 63);			/* Move name in */
			CtoPstr(tempCstring);
			BlockMoveData(tempCstring, &(SetSessionPtr->otppassword[0]), tempCstring[0]+1);
			TelInfo->CONFstate = 0;
			break;
		case 70: // realbold
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->realbold = a;
			TelInfo->CONFstate = 0;
			break;
		case 71: // otpsavepass
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->otpsavepass = a;
			TelInfo->CONFstate = 0;
			break;
		case 72: // oldscrollback
			if (1 == sscanf( s, "%d", &a))
				SetTerminalPtr->oldScrollback = a;
			TelInfo->CONFstate = 0;
			break;
/* NONO */
		case 73: // protocol
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->protocol = a;
			TelInfo->CONFstate = 0;
			break;
		case 74: // encryption
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->encryption = a;
			TelInfo->CONFstate = 0;
			break;
		case 75: // authentication
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->authentication = a;
			TelInfo->CONFstate = 0;
			break;
		case 76: // compression
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->compression = a;
			TelInfo->CONFstate = 0;
			break;
		case 77: // verbose
			SetSessionPtr->verbose = (affirmative(s)) ? TRUE : FALSE;	
			TelInfo->CONFstate=0;			
			break;
		case 78: // trace
			SetSessionPtr->trace = (affirmative(s)) ? TRUE : FALSE;	
			TelInfo->CONFstate=0;			
			break;
		case 79: // debug
			SetSessionPtr->debug = (affirmative(s)) ? TRUE : FALSE;	
			TelInfo->CONFstate=0;			
			break;
		case 80: // restricted
			SetSessionPtr->restricted = (affirmative(s)) ? TRUE : FALSE;	
			TelInfo->CONFstate=0;			
			break;
		case 81: // ssh2method
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->ssh2method = a;
			TelInfo->CONFstate = 0;
			break;
		case 82: // ssh2guests
			SetSessionPtr->ssh2guests = (affirmative(s)) ? TRUE : FALSE;	
			TelInfo->CONFstate=0;			
			break;
		case 83: // localport
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->localport = a;
			TelInfo->CONFstate = 0;
			break;
		case 84: // remoteport
			if (1 == sscanf( s, "%d", &a))
				SetSessionPtr->remoteport = a;
			TelInfo->CONFstate = 0;
			break;
		case 85: // remotehost
			strncpy(SetSessionPtr->remotehost, s, sizeof(SetSessionPtr->remotehost) - 1);
			CtoPstr(SetSessionPtr->remotehost);
			TelInfo->CONFstate = 0;
			break;
		case 86: // username
			strncpy(SetSessionPtr->username, s, sizeof(SetSessionPtr->username) - 1);
			CtoPstr(SetSessionPtr->username);
			TelInfo->CONFstate = 0;
			break;
/*
		case 87: // password
			strncpy(SetSessionPtr->password, s, sizeof(SetSessionPtr->password) - 1);
			CtoPstr(SetSessionPtr->password);
			TelInfo->CONFstate = 0;
			break;
*/
		case 88: // command
			strncpy(SetSessionPtr->command, s, sizeof(SetSessionPtr->command) - 1);
			CtoPstr(SetSessionPtr->command);
			TelInfo->CONFstate = 0;
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
			TelInfo->CONFstate = 0;
			break;
/* NONO */
		default:
			TelInfo->CONFstate=0;
		}
	return(0);
  } /* confile */

/************************************************************************/
/* contoken
*  tokenize the strings which get passed to confile.
*  Handles quotes and uses separators:  <33, ;:=
*/ 
short contoken( char c)
{
	short		retval;
	Boolean		success;

	if (c == EOF) {
		Cspace[TelInfo->position++] = '\0';
		sets_debug_print("Eof handler called");
		confile(Cspace);
		if (TelInfo->CONFactive) {
				success = CreateConnectionFromParams(SetParams);
				if (!success) {
					sets_debug_print("ERROR IN OPENING!! ");
					return(42);
					}
				}
		return(-1);
	}
	
	if (!TelInfo->position && !TelInfo->inquote && Sissep(c))
	/*if (!TelInfo->position && Sissep(c))	*/	/* old_skip over junk before keyword */
		return(0);

	if (TelInfo->inquote || !Sissep(c)) {

		if (TelInfo->position > 200) {
			sets_debug_print("Out of bounds error!");
			return(1);
		}
/*
*  check for quotes, a little mixed up here, could be reorganized
*/
		if (c == '"' ) {
			if (!TelInfo->inquote) {			/* beginning of quotes */
				TelInfo->inquote = 1;
				return(0);
			}
			 Cspace[TelInfo->position++] =c;
			return(0);
		}
		else 
			{						/* include in current string */
			if (c != '\012' && c != '\015')		/* BYU 2.4.18 - changed \n to \015 and added \012 */
				{
				Cspace[TelInfo->position++] = c;
				return(0);
				}
			}
				
		}

	if (Cspace[TelInfo->position-1] == '"') TelInfo->position--;
	Cspace[TelInfo->position++] = '\0';

	retval = confile(Cspace);			/* pass the token along */

	TelInfo->position = 0;
	TelInfo->inquote = 0;
	Cspace[0] = '\0';

	return(retval);
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

	Cspace = myNewPtr(256);				/* BYU LSC - get room for gathering stuff */

	if (NULL == Cspace) 				/* no memory left for the set to load in */
		{								/* we're out of memory */
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
		DisposePtr((Ptr) Cspace);
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

	BlockMoveData(&sfr.fName, set.name, (*sfr.fName)+1); // pstring copy sfr.fName -> set.name
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
	
	return (buffer);
}

void CStringToFile(short myfile, unsigned char *mystring) 
{	
	long mycount;										/* BYU LSC */
	short fstatus;										/* BYU LSC */
  
  mycount = strlen((char *) mystring);				/* BYU LSC */
  fstatus = FSWrite(myfile,&mycount,mystring);		/* BYU LSC */
}


void SaveSet(short doSaveMacros, short dontSaveTitle)
{
	SFReply		sfr;
	short		fn, truncate;
	WindowPeek	wpeek;
	Rect		rect;
	Point		where;
	long		junk;
	char		temp[256], temp2[256];			/* BYU LSC */
	short			fnum,fsiz;
	short			i;
	FSSpec		set;
	OSErr		err;
	Str255		scratchPstring,scratchPstring2;
	
	where.h = 100; where.v = 100;
	GetIndString(scratchPstring,MISC_STRINGS,SAVE_SET_STRING);
	GetIndString(scratchPstring2,MISC_STRINGS,DEFAULT_SAVE_SET_NAME);
	SFPutFile(where,scratchPstring,scratchPstring2, 0L, &sfr);	/* BYU LSC */

	if (!sfr.good)
		return;

	BlockMoveData(&sfr.fName, set.name, (*sfr.fName)+1); // pstring copy sfr.fName -> set.name
	GetWDInfo(sfr.vRefNum, &set.vRefNum, &set.parID, &junk);

	if ((err = HCreate(set.vRefNum, set.parID, set.name, kNCSACreatorSignature, kNCSASetFileType)) == dupFNErr)
		truncate = 1;
		
	err = HOpenDF(set.vRefNum, set.parID, set.name, fsWrPerm, &fn);

	if (truncate) 
		SetEOF(fn, 0L);

	if (gApplicationPrefs->CommandKeys)
		CStringToFile(fn,(unsigned char *) "commandkeys = yes\015");	/* BYU LSC */
	else
		CStringToFile(fn,(unsigned char *) "commandkeys = no\015");		/* BYU LSC */

	if (doSaveMacros)
/* NONO: save all macros */
		for (i = 0; i < 110; i++)
		  {
			getmacro(&TelInfo->newMacros,i, temp, sizeof(temp));			/* BYU LSC */
			if (*temp) {									/* BYU LSC */
				sprintf(temp2, "macro= \"%d ", i);			/* BYU 2.4.16 */
				CStringToFile(fn,(unsigned char *) temp2);	/* BYU LSC */
				CStringToFile(fn,(unsigned char *) temp);	/* BYU LSC */
				strcpy(temp2,"\"\015");						/* BYU LSC */
				CStringToFile(fn,(unsigned char *) temp2);	/* BYU LSC */
			}												/* BYU LSC */
		  } /* for */
/* NONO */
#if 0													/* BYU LSC */
	for (i = 0; i < TelInfo->numwindows; i++)
	  {
		short j;
		j = RSgetfont(screens[i].vs, &fnum, &fsiz);
	  } /* for */
#endif													/* BYU LSC */

	for (i = 0; i < TelInfo->numwindows; i++)
	  {
	  	if (!dontSaveTitle) {
		  	GetWTitle(screens[i].wind, scratchPstring);
		  	PtoCstr(scratchPstring);
			sprintf(temp2, "name= \"%s\"\015", scratchPstring);
		}
		else
			sprintf(temp2, "name= \" \"\015", scratchPstring);

		CStringToFile(fn,(unsigned char *) temp2);				/* BYU LSC */

		BlockMoveData((Ptr)screens[i].machine, (Ptr)scratchPstring, StrLength(screens[i].machine)+1);
		PtoCstr(scratchPstring);
		sprintf(temp2, "host= \"%s\"\015", scratchPstring);
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */

		sprintf (temp2,"port= %d\015",screens[i].portNum);	/* NCSA: save port # */
		CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */

		sprintf(temp2, "scrollback= %d\015", (screens[i].maxscroll));	/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */

		if (screens[i].bsdel)
			CStringToFile(fn,(unsigned char *)  "erase = delete\015");		/* BYU LSC */
		else
			CStringToFile(fn,(unsigned char *)  "erase = backspace\015");	/* BYU LSC */

		wpeek = (WindowPeek) screens[i].wind;
		rect = (*wpeek->contRgn)->rgnBBox;

		if (!dontSaveTitle) {
			sprintf(temp2, "size = {%d,%d,%d,%d}\015", rect.top, rect.left,	/* BYU LSC */
						rect.bottom, rect.right);
			CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */
		}

		sprintf(temp2, "vtwidth = %d\015", VSmaxwidth(screens[i].vs) + 1);			/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */

		if (screens[i].tekclear)
			CStringToFile(fn,(unsigned char *) "tekclear = yes\015");	/* BYU LSC */
		else
			CStringToFile(fn,(unsigned char *) "tekclear = no\015");	/* BYU LSC */

		if (TelInfo->haveColorQuickDraw)
		{
			RGBColor theColor;
			short j;
			for (j = 0; j < 4; j++)
			{
				RSgetcolor( screens[i].vs, j, &theColor);
				sprintf(temp2, "rgb%d = {%u,%u,%u}\015",
					j, theColor.red, theColor.green, theColor.blue);
				CStringToFile(fn,(unsigned char *) temp2);
			} 
		} 
		RSgetfont( screens[i].vs, &fnum, &fsiz);
		GetFontName( fnum, (StringPtr)temp);									/* BYU LSC */
#ifndef MPW
		p2cstr((unsigned char *) temp);								/* BYU LSC */
#endif

		sprintf( temp2, "font = \"%s\"\015", temp);					/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
		sprintf( temp2, "fsize= %d\015", fsiz);						/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */

		RSsetwind(screens[i].vs);
		fnum = RScurrent->bfnum;
		GetFontName( fnum, (StringPtr)temp);									/* BYU LSC */
#ifndef MPW
		p2cstr((unsigned char *) temp);								/* BYU LSC */
#endif

		sprintf( temp2, "boldfont = \"%s\"\015", temp);
		CStringToFile(fn,(unsigned char *) temp2);

		sprintf( temp2, "nlines= %d\015", VSgetlines(screens[i].vs));/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
		sprintf( temp2, "keystop= %d\015", screens[i].TELstop);		/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
		sprintf( temp2, "keygo= %d\015", screens[i].TELgo);			/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
		sprintf( temp2, "keyip= %d\015", screens[i].TELip);			/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
		sprintf( temp2, "crmap= %d\015", screens[i].crmap);		/* BYU LSC */
		CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
		sprintf( temp2, "tekem= %d\015", screens[i].tektype);
		CStringToFile(fn,(unsigned char *) temp2);

// All this stuff was added in BetterTelnet 1.0b3... (RAB)

		sprintf( temp2, "vtemulation = %d\015", screens[i].vtemulation);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "forcesave = %d\015", screens[i].forcesave);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "eightbit = %d\015", screens[i].eightbit);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "vtwrap = %d\015", screens[i].wrap);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "xterm = %d\015", screens[i].Xterm);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "remapctrld = %d\015", screens[i].remapCtrlD);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "ansicolor = %d\015", screens[i].ANSIgraphics);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "arrowmap = %d\015", screens[i].arrowmap);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "emacsmeta = %d\015", screens[i].emacsmeta);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "pastemethod = %d\015", screens[i].pastemethod);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "pastesize = %d\015", screens[i].pastesize);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "saveclear = %d\015", screens[i].ESscroll);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "linemode = %d\015", screens[i].lineAllow);
		CStringToFile(fn,(unsigned char *) temp2);

// And all this stuff was added a bit later... (RAB BetterTelnet 1.0 and later)

		sprintf( temp2, "negative = %d\015", screens[i].portNegative);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "allowbold = %d\015", screens[i].allowBold);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "colorbold = %d\015", screens[i].colorBold);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "ignorebeeps = %d\015", screens[i].ignoreBeeps);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "ignoreff = %d\015", screens[i].ignoreff);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "inversebold = %d\015", screens[i].inversebold);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpauto = %d\015", screens[i].otpauto);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpmulti = %d\015", screens[i].otpmulti);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otphex = %d\015", screens[i].otphex);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpnoprompt = %d\015", screens[i].otpnoprompt);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "realbold = %d\015", screens[i].realbold);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "oldscrollback = %d\015", screens[i].oldScrollback);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpsavepass = %d\015", screens[i].otpsavepass);
		CStringToFile(fn,(unsigned char *) temp2);

// end of "all this stuff"

		if (screens[i].otppassword[0]) { // RAB BetterTelnet 1.2
			pstrcpy(scratchPstring, (unsigned char *)screens[i].otppassword);
			p2cstr(scratchPstring);
			sprintf(temp2, "otppassword = \"%s\"\015", scratchPstring);
			CStringToFile(fn, (unsigned char *)temp2);
		}

		if (screens[i].national) {						// Don't do this if using default translation table
			GetMenuItemText(myMenus[National], screens[i].national+1, scratchPstring);
			PtoCstr(scratchPstring);
			sprintf(temp2, "translation= \"%s\"\015", scratchPstring);
			CStringToFile(fn, (unsigned char *)temp2);
			}
		BlockMoveData(screens[i].answerback, scratchPstring, *(screens[i].answerback)+1);
		PtoCstr(scratchPstring);
		sprintf(temp2, "answerback= \"%s\"\015", scratchPstring);
		CStringToFile(fn, (unsigned char *)temp2);

		if (screens[i].authenticate)
			CStringToFile(fn,(unsigned char *) "authenticate = yes\015");	//CCP added next four lines for authencrypt
		else
			CStringToFile(fn,(unsigned char *) "authenticate = no\015");	

		if (screens[i].encrypt)
			CStringToFile(fn,(unsigned char *) "encrypt = yes\015");	
		else
			CStringToFile(fn,(unsigned char *) "encrypt = no\015");	

		if (screens[i].pgupdwn)
			CStringToFile(fn,(unsigned char *) "pageup = yes\015");	//CCP 2.7
		else
			CStringToFile(fn,(unsigned char *) "pageup = no\015");	

		if (screens[i].keypadmap)
			CStringToFile(fn,(unsigned char *) "keypad = yes\015");	//CCP 2.7
		else
			CStringToFile(fn,(unsigned char *) "keypad = no\015");

/* NONO */
		sprintf( temp2, "protocol = %d\015", screens[i].protocol);
		CStringToFile(fn,(unsigned char *) temp2);

		sprintf( temp2, "encryption = %d\015", screens[i].encryption);
		CStringToFile(fn,(unsigned char *) temp2);

		sprintf( temp2, "authentication = %d\015", screens[i].authentication);
		CStringToFile(fn,(unsigned char *) temp2);

		sprintf( temp2, "compression = %d\015", screens[i].compression);
		CStringToFile(fn,(unsigned char *) temp2);

		if (screens[i].verbose)
			CStringToFile(fn,(unsigned char *) "verbose = yes\015");
		else
			CStringToFile(fn,(unsigned char *) "verbose = no\015");

		if (screens[i].trace)
			CStringToFile(fn,(unsigned char *) "trace = yes\015");
		else
			CStringToFile(fn,(unsigned char *) "trace = no\015");

		if (screens[i].debug)
			CStringToFile(fn,(unsigned char *) "debug = yes\015");
		else
			CStringToFile(fn,(unsigned char *) "debug = no\015");

		if (screens[i].restricted)
			CStringToFile(fn,(unsigned char *) "restricted = yes\015");
		else
			CStringToFile(fn,(unsigned char *) "restricted = no\015");

		sprintf( temp2, "ssh2method = %d\015", screens[i].ssh2method);
		CStringToFile(fn,(unsigned char *) temp2);

		if (screens[i].ssh2guests)
			CStringToFile(fn,(unsigned char *) "ssh2guests = yes\015");
		else
			CStringToFile(fn,(unsigned char *) "ssh2guests = no\015");

		sprintf( temp2,"localport = %d\015",screens[i].localport );
		CStringToFile(fn,(unsigned char *) temp2);

		sprintf( temp2,"remoteport = %d\015", (unsigned short)screens[i].remoteport );
		CStringToFile(fn,(unsigned char *) temp2);

		if (screens[i].remotehost[0]) {
			sprintf( temp2, "remotehost = \"%#s\"\015", screens[i].remotehost );
			CStringToFile(fn,(unsigned char *) temp2);
		}
		if (screens[i].username[0]) {
			sprintf( temp2, "username = \"%#s\"\015", screens[i].username );
			CStringToFile(fn,(unsigned char *) temp2);
		}
/*
		if (screens[i].password[0]) {
			sprintf( temp2, "password = \"%#s\"\015", screens[i].password );
			CStringToFile(fn,(unsigned char *) temp2);
		}
*/
		if (screens[i].command[0]) {
			sprintf( temp2, "command = \"%#s\"\015", screens[i].command );
			CStringToFile(fn,(unsigned char *) temp2);
		}
/* NONO */

	} /* for i */

	FSClose(fn);						/* BYU LSC */
}

// This is new for BetterTelnet 1.0b4. It saves sessions directly from the Favorites. (RAB)
void SaveSetFromSession(SessionPrefs* setSession, TerminalPrefs* setTerminal, short doWeAppend, short doSaveMacros) {
	SFReply		sfr;
	short		fn, truncate;
	WindowPeek	wpeek;
	Rect		rect;
	Point		where;
	long		junk;
	char		temp[256], temp2[256];			/* BYU LSC */
	short			fnum,fsiz, i;
	FSSpec		set;
	OSErr		err;
	Str255		scratchPstring,scratchPstring2;
	SFTypeList	typesok = {'CONF'};

	where.h = 100; where.v = 100;
	if (doWeAppend) {
		GetIndString(scratchPstring,MISC_STRINGS,SAVE_SET_STRING);
		SFGetFile( where,scratchPstring, 0L, 1, typesok, 0L, &sfr);
	} else {
		GetIndString(scratchPstring,MISC_STRINGS,SAVE_SET_STRING);
		GetIndString(scratchPstring2,MISC_STRINGS,DEFAULT_SAVE_SET_NAME);
		SFPutFile(where,scratchPstring,scratchPstring2, 0L, &sfr);	/* BYU LSC */
	}

	if (!sfr.good)
		return;

	BlockMoveData(&sfr.fName, set.name, (*sfr.fName)+1); // pstring copy sfr.fName -> set.name
	GetWDInfo(sfr.vRefNum, &set.vRefNum, &set.parID, &junk);

	if (!doWeAppend)
		if ((err = HCreate(set.vRefNum, set.parID, set.name, kNCSACreatorSignature, kNCSASetFileType)) == dupFNErr)
			truncate = 1;
		
	err = HOpenDF(set.vRefNum, set.parID, set.name, fsWrPerm, &fn);

	if ((!doWeAppend) && truncate) 
		SetEOF(fn, 0L);

	if (doWeAppend)
		SetFPos(fn, 2, 0L);

	if (doSaveMacros)
/* NONO: save all macros */
		for (i = 0; i < 110; i++)
		  {
			getmacro(&TelInfo->newMacros,i, temp, sizeof(temp));			/* BYU LSC */
			if (*temp) {									/* BYU LSC */
				sprintf(temp2, "macro= \"%d ", i);			/* BYU 2.4.16 */
				CStringToFile(fn,(unsigned char *) temp2);	/* BYU LSC */
				CStringToFile(fn,(unsigned char *) temp);	/* BYU LSC */
				strcpy(temp2,"\"\015");						/* BYU LSC */
				CStringToFile(fn,(unsigned char *) temp2);	/* BYU LSC */
			}												/* BYU LSC */
		  } /* for */
/* NONO */

	sprintf(temp2, "name= \" \"\015");
	CStringToFile(fn,(unsigned char *) temp2);				/* BYU LSC */

	BlockMoveData(setSession->hostname, (Ptr)scratchPstring, StrLength(setSession->hostname)+1);
	PtoCstr(scratchPstring);
	sprintf(temp2, "host= \"%s\"\015", scratchPstring);
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */

	sprintf (temp2,"port= %d\015",setSession->port);	/* NCSA: save port # */
	CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */

	sprintf(temp2, "scrollback= %d\015", setTerminal->numbkscroll);	/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */

	if (setSession->bksp)
		CStringToFile(fn,(unsigned char *)  "erase = delete\015");		/* BYU LSC */
	else
		CStringToFile(fn,(unsigned char *)  "erase = backspace\015");	/* BYU LSC */

	sprintf(temp2, "vtwidth = %d\015", setTerminal->vtwidth);			/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);						/* BYU LSC */

	if (setSession->tekclear)
		CStringToFile(fn,(unsigned char *) "tekclear = yes\015");	/* BYU LSC */
	else
		CStringToFile(fn,(unsigned char *) "tekclear = no\015");	/* BYU LSC */

	if (TelInfo->haveColorQuickDraw)
	{
		sprintf(temp2, "rgb%d = {%u,%u,%u}\015",
			0, setTerminal->nfcolor.red, setTerminal->nfcolor.green, setTerminal->nfcolor.blue);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf(temp2, "rgb%d = {%u,%u,%u}\015",
			1, setTerminal->nbcolor.red, setTerminal->nbcolor.green, setTerminal->nbcolor.blue);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf(temp2, "rgb%d = {%u,%u,%u}\015",
			2, setTerminal->bfcolor.red, setTerminal->bfcolor.green, setTerminal->bfcolor.blue);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf(temp2, "rgb%d = {%u,%u,%u}\015",
			3, setTerminal->bbcolor.red, setTerminal->bbcolor.green, setTerminal->bbcolor.blue);
		CStringToFile(fn,(unsigned char *) temp2);
	}

	BlockMoveData(setTerminal->DisplayFont, temp, StrLength(setTerminal->DisplayFont) + 1);
#ifndef MPW
	p2cstr((unsigned char *) temp);								/* BYU LSC */
#endif

	sprintf( temp2, "font = \"%s\"\015", temp);					/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
	sprintf( temp2, "fsize= %d\015", setTerminal->fontsize);						/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */

	BlockMoveData(setTerminal->BoldFont, temp, StrLength(setTerminal->BoldFont) + 1);
#ifndef MPW
	p2cstr((unsigned char *) temp);
#endif
	sprintf( temp2, "boldfont = \"%s\"\015", temp);
	CStringToFile(fn,(unsigned char *) temp2);

	sprintf( temp2, "nlines= %d\015", setTerminal->vtheight);/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
	sprintf( temp2, "keystop= %d\015", setSession->skey);		/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
	sprintf( temp2, "keygo= %d\015", setSession->qkey);			/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
	sprintf( temp2, "keyip= %d\015", setSession->ckey);			/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
	sprintf( temp2, "crmap= %d\015", setSession->crmap);		/* BYU LSC */
	CStringToFile(fn,(unsigned char *) temp2);					/* BYU LSC */
	sprintf( temp2, "tekem= %d\015", setSession->tektype);
	CStringToFile(fn,(unsigned char *) temp2);

// All this stuff was added in BetterTelnet 1.0b3... (RAB)

	sprintf( temp2, "vtemulation = %d\015", setTerminal->vtemulation);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "forcesave = %d\015", setSession->forcesave);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "eightbit = %d\015", setTerminal->eightbit);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "vtwrap = %d\015", setTerminal->vtwrap);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "xterm = %d\015", setTerminal->Xtermsequences);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "remapctrld = %d\015", setTerminal->remapCtrlD);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "ansicolor = %d\015", setTerminal->ANSIgraphics);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "arrowmap = %d\015", setTerminal->emacsarrows);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "emacsmeta = %d\015", setTerminal->emacsmetakey);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "pastemethod = %d\015", setSession->pastemethod);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "pastesize = %d\015", setSession->pasteblocksize);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "saveclear = %d\015", setTerminal->clearsave);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "linemode = %d\015", setSession->linemode);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "negative = %d\015", setSession->portNegative);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "allowbold = %d\015", setTerminal->allowBold);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "colorbold = %d\015", setTerminal->colorBold);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "ignorebeeps = %d\015", setSession->ignoreBeeps);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "ignoreff = %d\015", setSession->ignoreff);
	CStringToFile(fn,(unsigned char *) temp2);

// And all this stuff was added in 1.0b5... (RAB)

	sprintf( temp2, "localecho = %d\015", setSession->localecho);
	CStringToFile(fn,(unsigned char *) temp2);
	sprintf( temp2, "halfdup = %d\015", setSession->halfdup);
	CStringToFile(fn,(unsigned char *) temp2);

// end of "all this stuff" (RAB)

		sprintf( temp2, "otpauto = %d\015", setSession->otpauto);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpmulti = %d\015", setSession->otpmulti);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otphex = %d\015", setSession->otphex);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpnoprompt = %d\015", setSession->otpnoprompt);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "realbold = %d\015", setTerminal->realbold);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "oldscrollback = %d\015", setTerminal->oldScrollback);
		CStringToFile(fn,(unsigned char *) temp2);
		sprintf( temp2, "otpsavepass = %d\015", setSession->otpsavepass);
		CStringToFile(fn,(unsigned char *) temp2);

		/* NONO bug fix : screens[i] was used instead of setSession */
		if (setSession->otppassword[0]) { // RAB BetterTelnet 1.2
			pstrcpy(scratchPstring, (unsigned char *)setSession->otppassword);
			p2cstr(scratchPstring);
			sprintf(temp2, "otppassword = \"%s\"\015", scratchPstring);
			CStringToFile(fn, (unsigned char *)temp2);
		}


	BlockMoveData(setSession->TranslationTable, scratchPstring, StrLength(setSession->TranslationTable) + 1);
	PtoCstr(scratchPstring);
	sprintf(temp2, "translation= \"%s\"\015", scratchPstring);
	CStringToFile(fn, (unsigned char *)temp2);

	BlockMoveData(setTerminal->AnswerBackMessage, scratchPstring, *(setTerminal->AnswerBackMessage)+1);
	PtoCstr(scratchPstring);
	sprintf(temp2, "answerback= \"%s\"\015", scratchPstring);
	CStringToFile(fn, (unsigned char *)temp2);

	if (setSession->authenticate)
		CStringToFile(fn,(unsigned char *) "authenticate = yes\015");	//CCP added next four lines for authencrypt
	else
		CStringToFile(fn,(unsigned char *) "authenticate = no\015");	

	if (setSession->encrypt)
		CStringToFile(fn,(unsigned char *) "encrypt = yes\015");	
	else
		CStringToFile(fn,(unsigned char *) "encrypt = no\015");	

	if (setTerminal->MATmappings)
		CStringToFile(fn,(unsigned char *) "pageup = yes\015");	//CCP 2.7
	else
		CStringToFile(fn,(unsigned char *) "pageup = no\015");	

	if (setTerminal->remapKeypad)
		CStringToFile(fn,(unsigned char *) "keypad = yes\015");	//CCP 2.7
	else
		CStringToFile(fn,(unsigned char *) "keypad = no\015");

/* NONO */
	sprintf( temp2, "protocol = %d\015", setSession->protocol);
	CStringToFile(fn,(unsigned char *) temp2);

	sprintf( temp2, "encryption = %d\015", setSession->encryption);
	CStringToFile(fn,(unsigned char *) temp2);

	sprintf( temp2, "authentication = %d\015", setSession->authentication);
	CStringToFile(fn,(unsigned char *) temp2);

	sprintf( temp2, "compression = %d\015", setSession->compression);
	CStringToFile(fn,(unsigned char *) temp2);

	if (setSession->verbose)
		CStringToFile(fn,(unsigned char *) "verbose = yes\015");
	else
		CStringToFile(fn,(unsigned char *) "verbose = no\015");

	if (setSession->trace)
		CStringToFile(fn,(unsigned char *) "trace = yes\015");
	else
		CStringToFile(fn,(unsigned char *) "trace = no\015");

	if (setSession->debug)
		CStringToFile(fn,(unsigned char *) "debug = yes\015");
	else
		CStringToFile(fn,(unsigned char *) "debug = no\015");

	if (setSession->restricted)
		CStringToFile(fn,(unsigned char *) "restricted = yes\015");
	else
		CStringToFile(fn,(unsigned char *) "restricted = no\015");

	sprintf( temp2, "ssh2method = %d\015", setSession->ssh2method);
	CStringToFile(fn,(unsigned char *) temp2);

	if (setSession->ssh2guests)
		CStringToFile(fn,(unsigned char *) "ssh2guests = yes\015");
	else
		CStringToFile(fn,(unsigned char *) "ssh2guests = no\015");

	sprintf( temp2,"localport = %d\015",setSession->localport );
	CStringToFile(fn,(unsigned char *) temp2);

	sprintf( temp2,"remoteport = %d\015", (unsigned short)setSession->remoteport );
	CStringToFile(fn,(unsigned char *) temp2);

	if (setSession->remotehost[0]) {
		sprintf( temp2, "remotehost = \"%#s\"\015", setSession->remotehost );
		CStringToFile(fn,(unsigned char *) temp2);
	}
	if (setSession->username[0]) {
		sprintf( temp2, "username = \"%#s\"\015", setSession->username );
		CStringToFile(fn,(unsigned char *) temp2);
	}
/*
	if (setSession->password[0]) {
		sprintf( temp2, "password = \"%#s\"\015", setSession->password );
		CStringToFile(fn,(unsigned char *) temp2);
	}
*/
	if (setSession->command[0]) {
		sprintf( temp2, "command = \"%#s\"\015", setSession->command );
		CStringToFile(fn,(unsigned char *) temp2);
	}
/* NONO */

  	FSClose(fn);						/* BYU LSC */
}

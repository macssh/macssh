// linemode.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "wind.h"
#include "parse.h"

#include "linemode.proto.h"
#include "network.proto.h"
#include "parse.proto.h"
#include "vsdata.h"
#include "vsinterf.proto.h"
#include "menuseg.proto.h"

//#define	OPTS_DEBUG
#ifdef	OPTS_DEBUG
extern char *telstates[];
extern char *teloptions[];
extern char *LMoptions[];
extern char *LMflags[];
extern char *LMmodes[];
extern char *LMmodeBits[];
#define	opts_debug_print(x)	putln(x)	
#else
#define	opts_debug_print(x)
#endif

extern short scrn;

static	void	DemangleLineMode(char *s, short mode);
static void setForwardMask(struct WindRec *tw); //set the keys on which we forward on
static void negotiateSLC(struct WindRec *tw);
static void respondSLC(short optionNumber,short *alreadySentSB,struct WindRec *tw, Boolean AWK);



static	unsigned char str_lm[] = { TEL_IAC, TEL_SB, N_LINEMODE, 0, 0, TEL_IAC, TEL_SE };
static	unsigned char str_lm_mode[] = { TEL_IAC, TEL_SB, N_LINEMODE, L_MODE, 0, TEL_IAC, TEL_SE };

static unsigned char default_slc[] = 
	{ 255, 255, DEFAULT_SLC_BRK, DEFAULT_SLC_IP,DEFAULT_SLC_AO,0,0,DEFAULT_SLC_ABORT,DEFAULT_SLC_EOF,
		DEFAULT_SLC_SUSP,DEFAULT_SLC_EC,DEFAULT_SLC_EL,DEFAULT_SLC_EW,DEFAULT_SLC_RP,
		DEFAULT_SLC_LNEXT,DEFAULT_SLC_XON,DEFAULT_SLC_XOFF,DEFAULT_SLC_FORW1,DEFAULT_SLC_FORW2,255,255,255,255,255,255,255,255,255,255,255,255 };

#ifdef OPTS_DEBUG
	static char munger[255]; 
#endif

void	LinemodeUnload(void) {}

void initLinemode(struct WindRec *tw)
{
	short temp;
	for (temp=0; temp<= SLC_MAX; temp++)
	{
		tw->slc[temp] = default_slc[temp];
		if (default_slc[temp] == 255)
			tw->slcLevel[temp] = SLC_NOSUPPORT;
		else
			tw->slcLevel[temp] = SLC_VALUE;
	}
	tw->lmodeBits = 0;
	tw->litNext = 0;
	tw->lmode = 0;
}

void process_key(unsigned char ascii,struct WindRec *tw)
{
	if (tw->litNext) {
		//do no processing on next key
		tw->litNext = FALSE;

		kbwrite(tw, &ascii, 1);

		if (tw->echo) {
			if (ascii>31 && ascii <127) {
				/* add these chars to buffer */
				parse(tw, &ascii, 1);
			}
			return;
		}
	}

	if (tw->lmodeBits & 2) {		
		// TRAPSIG mode active
		unsigned char toSend[2] = {IAC,0};
		short whichSignal = 0;
		if (ascii == tw->slc[SLC_IP]) {
			whichSignal = SLC_IP;
			toSend[1] = TEL_IP;
		} else if (ascii == tw->slc[SLC_SUSP]) {
			whichSignal = SLC_SUSP;
			toSend[1] = TEL_SUSP;
		}
// RAB BetterTelnet 2.0b1
// The defaults in parse.h are to send ABORT on ^\ which is usually correct, but
// feel free to hack this (in parse.h, not here) if BRK is better in your configuration.
		else if (ascii == tw->slc[SLC_ABORT])
		{
			whichSignal = SLC_ABORT;
			toSend[1] = TEL_ABORT;
		}
		else if (ascii == tw->slc[SLC_BRK]) // RAB BetterTelnet 2.0b1
		{
			whichSignal = SLC_BRK;
			toSend[1] = TEL_BREAK;
		}
		if (toSend[1]) //if we have a signal to catch
		{
//			if (tw->echo)
//				parse(tw, &ascii, 1); // echo if we should
			tw->kblen=0; //zero out the buffer
			netpush(tw->port);
			netwrite(tw->port,toSend,2); //send IAC whatever
			if (tw->slcLevel[whichSignal] & SLC_FLUSHIN)
			{
				unsigned char dm[2] = {IAC,TEL_DM};
				netpush(tw->port);
				netUrgent();//send next as urgent data
				netwrite(tw->port,dm,2);//send IAC DM
			}
			if (tw->slcLevel[whichSignal] & SLC_FLUSHOUT)
			{
				unsigned char tm[3] = {IAC,TEL_DOTEL,N_TIMING};
				tw->timing = 1;	//tell ourselves to wait for WILL TIMING
				netpush(tw->port);
				netwrite(tw->port,tm,3);//send DO TIMING
			}
			return;
		}
	}
	
	if ((tw->lmodeBits & L_SOFT_TAB)&&(ascii == 0x09)) // SOFT_TAB mode active; expand tab into spaces
	{
		short numSpaces = VSIgetNextTabDistance();
		unsigned char spacechar = ' ';
		while (numSpaces-- > 0) {
			kbwrite(tw, &spacechar, 1);
		}
		if (tw->echo)
			parse(tw, &ascii, 1);
		return;
	}
	


	if (tw->lmodeBits & L_EDIT) //handle editing functions
	{
			

		if (ascii == '\015') //CR
		{ //since we are in edit, send the buffer and CR-LF
			kbflush(tw);
			netpush(tw->port);
			netwrite(tw->port,"\015\012",2);
			if (tw->echo)
				parse(tw,(unsigned char *) "\012\015",2);
			return;
		}
		
		if (ascii == tw->slc[SLC_EC]) //kill the character
		{
			if (tw->echo)
				parse(tw,(unsigned char *) "\010 \010",3);	
			tw->kblen--;
			return;
		}
		else if (ascii == tw->slc[SLC_AO]) //kill the line
		{
			while (tw->kblen >0) 
			{
				if (tw->echo)
					parse(tw,(unsigned char *) "\010 \010",3);
				tw->kblen--;
			}
			return;
		}
		else if (ascii == tw->slc[SLC_EL]) //kill the line
		{
			while (tw->kblen >0) 
			{
				if (tw->echo)
					parse(tw,(unsigned char *) "\010 \010",3);
				tw->kblen--;
			}
			return;
		}
		else if ((ascii == tw->slc[SLC_EOF]) && (tw->lmodeBits & 2))
		{ //push the buffer, send IAC EOF (RAB BetterTelnet 2.0b1 - only under TRAPSIG)
			char eofString[2] = { IAC, TEL_EOF };
			kbflush(tw);
// RAB BetterTelnet 2.0b1 - BAD! BAD! BAD!
// Fix for *BSD (and probably others):
// Putting ^D into Telnet's key buffer after sending an EOF could make it pop up later, so
// hitting some keys (arrows seemed to do the trick) that flushed Telnet's key buffer
// after a cat command which terminated with ^D caused a logout. Yuck.
//			tw->kbbuf[0]=ascii;
//			tw->kblen=1;
			netpush(tw->port);
			netwrite(tw->port,eofString, 2);
			return;
		}
// RAB BetterTelnet 2.0b1
// We don't need to do this down here if we're already handling TRAPSIG mode
// separately. The SUSP (usually ^Z) char is part of TRAPSIG, not EDIT.
//		else if (ascii == tw->slc[SLC_SUSP])
//		{
//			char eofString[2] = { IAC, TEL_SUSP };
//			if (tw->kblen > 0)
//				netwrite(tw->port, tw->kbbuf, tw->kblen);
//			tw->kblen = 0;
//			netpush(tw->port);
//			netwrite(tw->port,eofString, 2);
//			return;
//		}
		else if (ascii == tw->slc[SLC_EW])
		{
			while ((tw->kbbuf[tw->kblen-1] != 0x20)&&(tw->kblen >= 0)) //while its not a space
			{
				if (tw->echo)
					parse(tw,(unsigned char *)"\010 \010",3);
				tw->kblen--;
			}
		}
		else if (ascii == tw->slc[SLC_RP])
		{
			VSredrawLine(tw->vs);
			return;
		}
		else if (ascii == tw->slc[SLC_LNEXT])
		{
			tw->litNext = TRUE;
			return;
		}
		else if (ascii == tw->slc[SLC_XON])
		{
			if (tw->allow_flow) { //remote flow control can turn this off
				tw->enabled = 1; // oops small bug (RAB BetterTelnet 2.0b1)
				changeport(scrn, scrn);
			}
			return;
		}	
		else if (ascii == tw->slc[SLC_XOFF])
		{
			if (tw->allow_flow) { //remote flow control can turn this off
				tw->enabled = 0;
				changeport(scrn, scrn);
			}
			return;
		}
		else if ((ascii == tw->slc[SLC_FORW1])||(ascii == tw->slc[SLC_FORW1]))
		{
			kbflush(tw);
			netwrite(tw->port,&ascii,1);
			return;

		}
		//ok, at this point, we are past all local editing functions.  Now, add the character to the buffer.
		else
		{
			kbwrite(tw, &ascii, 1);
		}

	}
	else if (ascii == '\015') //CR; map this to CR-LF
	{
		unsigned char toSend[2] = {0x0D,0x00};
		netpush(tw->port);
		netwrite(tw->port,toSend,2);
		if (tw->echo)
			parse(tw,(unsigned char *) "\012\015",2);
		return;	
	}
	else //not editing; send it
	{
		netpush(tw->port);
		netwrite( tw->port, &ascii, 1);	// if full send buffer 
	}

	if (tw->echo)	/* Handle local ECHOs */
	{
		if (ascii>31 && ascii <127)	/* add these chars to buffer */
			parse(tw, &ascii, 1);
		else			/* not printable char */
		{
			if (!(tw->lmodeBits & L_LIT_ECHO)) //don't echo if this is set
			{		
				ascii='@'+ascii;
				parse(tw,(unsigned char *) "^",1);
				parse(tw, &ascii, 1);
			}
		}
	}
}


void	linemode_suboption(struct WindRec *tw)
{
	switch(tw->parsedat[1]) {
		char s[80];

		case L_MODE:	/* change mode */
#ifdef OPTS_DEBUG
			strcpy(s, "RECV: SB LINEMODE MODE => ");
			DemangleLineModeShort(s, tw->parsedat[2]);
			opts_debug_print(s);
#endif
			if (tw->lineAllow) {			// First make sure we allowed linemode in the first place.
				// RFC 1184 says client should ignore MODE negotiations with the MODE_ACK bit set, and should not
				//	generate a response if the negotiated MODE matches the current MODE
				if (!((tw->parsedat[2] & L_MODE_ACK) || ((tw->parsedat[2] & L_MODE_MASK) == tw->lmodeBits))) 
				{
					tw->lmodeBits = tw->parsedat[2];	// Accept the mode
					tw->parsedat[2] |= L_MODE_ACK;  // Set the MODE_ACK bit
 
					sprintf(s,"%c%c%c%c",IAC,TEL_SB,N_LINEMODE,L_MODE);
					netwrite(tw->port,s,4);
					sprintf(s,"%c%c%c",tw->parsedat[2],IAC,TEL_SE);
					netwrite(tw->port,s,3); 
#ifdef OPTS_DEBUG
					opts_debug_print("SENT: IAC SB");
					strcpy(s, "SENT: LM MODE = ");
					DemangleLineModeShort(s, tw->parsedat[2]);
					opts_debug_print(s);
					opts_debug_print("SENT: IAC SE");
#endif
				}
#ifdef OPTS_DEBUG
				else {
					strcpy(s, "LINEMODE MODE = ");
					DemangleLineModeShort(s, tw->parsedat[2]);
					opts_debug_print(s);
					if (tw->parsedat[2] & L_MODE_ACK) 
						opts_debug_print("\tignored because MODE_ACK was set.");
					else
						opts_debug_print("\tIgnored because we are already at that mode.");
					strcpy(s, "Curr Linemode = ");
					DemangleLineModeShort(s, tw->lmodeBits);
					opts_debug_print(s);
				}
#endif
				
			}
			break;
		
		case TEL_DOTEL:	
#ifdef OPTS_DEBUG
			sprintf(munger,"RECV: SB LINEMODE DO %c", tw->parsedat[2]);
			opts_debug_print(munger);
#endif
			tw->forwardMask = TRUE;
			setForwardMask(tw);
			str_lm[3] = TEL_WILLTEL;
			str_lm[4] = L_FORWARDMASK;
			netpush(tw->port);
			netwrite(tw->port, str_lm, sizeof(str_lm));
#ifdef OPTS_DEBUG
			sprintf(munger,"SENT: IAC SB LINEMODE WILL %c IAC SE", tw->parsedat[2]);
			opts_debug_print(munger);
#endif
			break;

		case TEL_DONTTEL:	
#ifdef OPTS_DEBUG
			sprintf(munger,"RECV: SB LINEMODE DO %c", tw->parsedat[2]);
			opts_debug_print(munger);
#endif
			tw->forwardMask = FALSE;
			str_lm[3] = TEL_WONTTEL;
			str_lm[4] = L_FORWARDMASK;
			netpush(tw->port);
			netwrite(tw->port, str_lm, sizeof(str_lm));
#ifdef OPTS_DEBUG
			sprintf(munger,"SENT: IAC SB LINEMODE WONT %c IAC SE", tw->parsedat[2]);
			opts_debug_print(munger);
#endif
			break;

		case L_SLC:		/* set local chars */
			negotiateSLC(tw);
		break;
			
		default:
#ifdef OPTS_DEBUG
		sprintf(munger, "RECV: SB LINEMODE ?? (?? = %c)", tw->parsedat[1]);
		opts_debug_print(munger);
#endif
		break;

	}//switch parsedat
}


void setForwardMask(struct WindRec *tw) //set the keys on which we forward on
{
	short j, i = 3;
	Boolean anotherByte = TRUE;
	tw->numForwardKeys = 1;
	while((tw->parsedat[i] != 0)&&(i < 35))
	{
		if (tw->parsedat[i] == 255)
		{
			i++; //could be doubled IAC
			if(tw->parsedat[i+1] == TEL_SE)
			{
				anotherByte = FALSE; //but its not
				i--;
			}
		}
		if (anotherByte)
			for(j = 0; j < 8; j++)
			{
				if (tw->parsedat[i] & (j*2))
				{
					tw->forwardKeys[tw->numForwardKeys-1] = (i-3)*8 + j;
					tw->numForwardKeys++;
				}
			}
		else
			break;
		i++;//next byte
	}		
	for (i = tw->numForwardKeys; i < 32; i++)  //zero the rest of the array
		tw->forwardKeys[i] = 0;
}			

void respondSLC(short optionNumber,short *alreadySentSB,struct WindRec *tw, Boolean AWK)
{
	unsigned char stuffToSend[3];
	unsigned char subBeginSeq[4] = {IAC,TEL_SB,N_LINEMODE,L_SLC};
	stuffToSend[0] = optionNumber;
	stuffToSend[1] = tw->slcLevel[optionNumber];
	if (AWK) 
		stuffToSend[1] |= SLC_AWK;	
	stuffToSend[2] = tw->slc[optionNumber];	
	
	if (!(*alreadySentSB))
	{
		*alreadySentSB = TRUE;
		netwrite(tw->port, subBeginSeq, 4);
		opts_debug_print("SENT: IAC SB LINEMODE SLC");
	}
	if (stuffToSend[2] == IAC)
	{
		netwrite(tw->port, stuffToSend,2);
		netwrite(tw->port, subBeginSeq,1); //double the IAC
		netwrite(tw->port, subBeginSeq,1);
	}
	else
		netwrite(tw->port, stuffToSend, 3);
#ifdef OPTS_DEBUG
	if (AWK)
	{
		if ((tw->slcLevel[optionNumber] & (SLC_FLUSHIN | SLC_FLUSHOUT)) == (SLC_FLUSHIN | SLC_FLUSHOUT))
			sprintf(munger,"\t %s %s | AWK | FLUSHIN | FLUSHOUT %d",LMoptions[optionNumber],LMflags[SLC_LEVELBITS & tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
		else if (tw->slcLevel[optionNumber] & SLC_FLUSHIN)
			sprintf(munger,"\t %s %s | AWK | FLUSHIN %d",LMoptions[optionNumber],LMflags[SLC_LEVELBITS & tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
		else if (tw->slcLevel[optionNumber] & SLC_FLUSHOUT)
			sprintf(munger,"\t %s %s | AWK | FLUSHOUT %d",LMoptions[optionNumber],LMflags[SLC_LEVELBITS & tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
		else
			sprintf(munger,"\t %s %s | AWK %d",LMoptions[optionNumber],LMflags[tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
	}
	else
	{
		if ((tw->slcLevel[optionNumber] & (SLC_FLUSHIN | SLC_FLUSHOUT)) == (SLC_FLUSHIN | SLC_FLUSHOUT))
			sprintf(munger,"\t %s %s | AWK | FLUSHIN | FLUSHOUT %d",LMoptions[optionNumber],LMflags[SLC_LEVELBITS & tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
		else if (tw->slcLevel[optionNumber] & SLC_FLUSHIN)
			sprintf(munger,"\t %s %s | FLUSHIN %d",LMoptions[optionNumber],LMflags[SLC_LEVELBITS & tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
		else if (tw->slcLevel[optionNumber] & SLC_FLUSHOUT)
			sprintf(munger,"\t %s %s | FLUSHOUT %d",LMoptions[optionNumber],LMflags[SLC_LEVELBITS & tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
		else
			sprintf(munger,"\t %s %s %d",LMoptions[optionNumber],LMflags[tw->slcLevel[optionNumber]],tw->slc[optionNumber]);
	}
	opts_debug_print(munger);
#endif
}

void negotiateSLC(struct WindRec *tw)
{
	short	lmslcflag = 0;
	unsigned char *ourValues = tw->slc;
	unsigned char *ourLevels = tw->slcLevel;
	short i;
	
#ifdef OPTS_DEBUG
	sprintf(munger,"RECV: IAC SB LINEMODE SLC");
	opts_debug_print(munger);
	for(i=2;i <= tw->parseIndex - 3; i+=3) 
	{
		if (tw->parsedat[i] > 30) break; // RAB BetterTelnet 2.0b3
										 // we shouldn't panic on bad data
//		if (tw->parsedat[i] > 30)
//			DebugStr("\pAck! Bad option number");
		if(tw->parsedat[i+1] & SLC_AWK)
			sprintf(munger,"     %s %s|AWK %d",LMoptions[tw->parsedat[i]],LMflags[tw->parsedat[i+1] & SLC_LEVELBITS],tw->parsedat[i+2]);
		else
			sprintf(munger,"     %s %s %d",LMoptions[tw->parsedat[i]],LMflags[tw->parsedat[i+1] & SLC_LEVELBITS],tw->parsedat[i+2]);
		opts_debug_print(munger);
	}	
	sprintf(munger,"RECV: IAC SE");
	opts_debug_print(munger);

#endif


	for (i=2, lmslcflag=0; i <= tw->parseIndex - 3; i+=3) 
	{
		short optionNumber = tw->parsedat[i];
		Boolean awked = (tw->parsedat[i+1] & SLC_AWK);
		unsigned char requestedValue = tw->parsedat[i+2];
		unsigned char requestedLevel = tw->parsedat[i+1] & SLC_LEVELBITS;
		Boolean flushin = tw->parsedat[i+1] & SLC_FLUSHIN;
		Boolean flushout = tw->parsedat[i+1] & SLC_FLUSHOUT;

		if ((ourValues[optionNumber] != requestedValue)||(ourLevels[optionNumber] != tw->parsedat[i+1])) //if we are sent what we already have, ignore it
		{
			if (requestedLevel == SLC_NOSUPPORT)
			{
				if (ourLevels[optionNumber] & SLC_LEVELBITS != SLC_NOSUPPORT)
				{
				 	ourValues[optionNumber] = 255;
					ourLevels[optionNumber] = SLC_NOSUPPORT;//ok, we wont support this
					if (!awked)
						respondSLC(optionNumber,&lmslcflag, tw, TRUE);//awk only if he didnt awk
#ifdef OPTS_DEBUG
					else
					{
						sprintf(munger,"\t %s %s %d accepted; no response sent because it was AWKED",
							LMoptions[optionNumber],LMflags[requestedLevel],requestedValue);				
						opts_debug_print(munger);
					}
#endif
				}
#ifdef OPTS_DEBUG
				else
				{
					sprintf(munger, "\t %s %s %d Ignored because we are already at nosupport",
						LMoptions[optionNumber],LMflags[requestedLevel],ourValues[optionNumber]);
					opts_debug_print(munger);
				}
#endif
			}
			else if (requestedLevel == SLC_DEFAULT)
			{
				ourValues[optionNumber] = default_slc[optionNumber]; //get our default
				if (ourValues[optionNumber] == 255)
					ourLevels[optionNumber] = SLC_NOSUPPORT;
				else
					ourLevels[optionNumber] = SLC_VALUE;
				respondSLC(optionNumber, &lmslcflag, tw, FALSE);	//tell him about our choice
			}
			else if (requestedLevel > ourLevels[optionNumber])
				respondSLC(optionNumber, &lmslcflag, tw, FALSE);  //keep lower setting
			else
			{
				ourValues[optionNumber] = requestedValue;
				ourLevels[optionNumber] = requestedLevel;
				if (flushin)
					ourLevels[optionNumber] |= SLC_FLUSHIN;
				if (flushout)
					ourLevels[optionNumber] |= SLC_FLUSHOUT;
				if (!awked)
					respondSLC(optionNumber, &lmslcflag, tw, TRUE); //agree to his new setting
			}
		}
#ifdef OPTS_DEBUG
		else
		{
			sprintf(munger, "\t %s %s Ignored because %d is already the current value",
				LMoptions[optionNumber],LMflags[requestedLevel],ourValues[optionNumber]);
			opts_debug_print(munger);
		}
#endif
		
	}//end for on parseindex
	if (lmslcflag) 
	{
		unsigned char toSend[2] = {IAC,TEL_SE};
		netpush(tw->port);
		netwrite(tw->port,toSend,2);
		opts_debug_print("SENT: IAC SE");	
	}
}
			

void doLinemode(struct WindRec *tw)
{
	short i;
	unsigned char subBeginSeq[4] = {IAC,TEL_SB,N_LINEMODE,L_SLC};
	unsigned char subEndSeq[2] = {IAC,TEL_SE};
	unsigned char toSend[3];
	
	
	tw->lmodeBits = 0;
	tw->lmode = TRUE;
	tw->litNext = FALSE;

	netwrite(tw->port,subBeginSeq,4);
	opts_debug_print("SENT: IAC SB LINEMODE SLC");
	
	for (i=1; i<= SLC_MAX; i++)
	{
		if (tw->slc[i]==255) 
		{
			toSend[0] = i;
			toSend[1] = SLC_NOSUPPORT;
			toSend[2] = 0;
#ifdef OPTS_DEBUG
			sprintf(munger,"     %s NO_SUPPORT 0",LMoptions[i]);
#endif
		}
		else
		{
			toSend[0] = i;
			toSend[1] = SLC_VALUE;
			toSend[2] = tw->slc[i];
#ifdef OPTS_DEBUG
			sprintf(munger,"     %s VALUE %d",LMoptions[i],(short)tw->slc[i]);
#endif
		}
		opts_debug_print(munger);
		netwrite(tw->port,toSend,3);
	}
	opts_debug_print("SENT: IAC SE");
	netpush(tw->port);
	netwrite(tw->port,subEndSeq,2); 
} 

static	void	DemangleLineMode(char *s, short mode)
{
#ifdef OPTS_DEBUG
	short 	i;
	
	for (i=0; i<5; i++) {
		if (mode & (1 << i))
			strcat(s, LMmodes[i]);
	}
#endif
}

void	DemangleLineModeShort(char *s, short mode)
{
#ifdef OPTS_DEBUG
	short 	i;
	
	for (i=0; i<5; i++) {
		if (mode & (1 << i))
			strcat(s, LMmodeBits[i]);
		else
			strcat(s," ");
	}
#endif
}

// parse.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "parse.h"				// For our #defines
#include "wind.h"				/* For WindRec structure */
#include "translate.h"
				/* For putln proto */

#include "ae.proto.h"
#include "network.proto.h"				/* For net functions */
#include "linemode.proto.h"
#include "vgtek.proto.h"
#include "vsdata.h"
#include "vsinterf.proto.h"
#include "maclook.proto.h"
#include "tekrgmac.proto.h"
#include "menuseg.proto.h"
#include "translate.proto.h"
#include "vr.h"
#include "vr.proto.h"
#include "tnae.h"
#include "authencrypt.h"
#include "authencrypt.proto.h"
#include "wdefpatch.proto.h"
#include "otp.proto.h"
#include "sshglue.proto.h"

//#define	OPTS_DEBUG
#ifdef	OPTS_DEBUG
#include "optsdebug.h"
#define	opts_debug_print(x)	putln(x)	
static char munger[255];
#else
#define	opts_debug_print(x)
#endif

/*#include <Profiler.h>*/

extern void syslog( int priority, const char *format, ...);

extern short 	scrn;
extern WindRec	*screens;

extern Boolean authOK;					// true if authentication driver available
extern Boolean encryptOK;				// true if des encryption routines available


#include "parse.proto.h"

extern void VSprintf(char *fmt, ...);
extern void VSdump(char *p, int len);

static	char	*getcname(struct WindRec *tw);
static	void	encryptStatechange(struct WindRec *tw);
static	void	process_suboption(struct WindRec *tw, unsigned char *start, unsigned char *end);
static	void	send_will(short port, short option);
static	void	send_wont(short port, short option);
static	void	telnet_do(struct WindRec *tw, short option);
static	void	telnet_dont(struct WindRec *tw, short option);
static	void	telnet_will(struct WindRec *tw, short option);
static	void	telnet_wont(struct WindRec *tw, short option);
static void		env_opt_start(void);
static void		env_opt_add(char *, char *);
static void		env_opt_end(void);
static int		opt_welldefined(char *);

void	Parseunload(void) {}

/*
 * kbflush
 */
void kbflush(struct WindRec *tw)
{
	if ( tw->kblen ) {
		netwrite( tw->port, tw->kbbuf, tw->kblen);
		tw->kblen = 0;	
	}
}

/*
 * kbwrite
 */
void kbwrite(struct WindRec *tw, unsigned char *string, short len)
{
	int i;

	for (i = 0; i < len; i++) {
		if ( tw->kblen == MAXKB ) {
			kbflush(tw);
		}
		tw->kbbuf[tw->kblen++] = string[i];
	}
}

void	SendStringAsIfTyped(struct WindRec *tw, char *string, short len)
{
	unsigned char	outbuf[256];
	unsigned char	trbuf[32];
	unsigned char	*pbuf;
	long			inlen;
	long			outlen;
	long			buflen;
	long			trlen;
	int				i;
	int				j;
	int				res;

	kbflush( tw );

	trflush_mac_nat(tw);

	while (len) {
		buflen = len > sizeof(outbuf) ? sizeof(outbuf) : len;
		for ( i = 0, outlen = 0; i < buflen && outlen < sizeof(outbuf); ++i ) {
			if ( /*string[i] != ESC &&*/ GetTranslationIndex(tw->outnational) != kTRJIS ) {
				pbuf = tw->troutbuf;
				if ( tw->troutcount >= sizeof(tw->troutbuf) ) {
					// !!!! shouldn't occur...
					Debugger();
					tw->troutcount = 0;
				}
				pbuf[tw->troutcount++] = string[i];
				inlen = tw->troutcount;
				trlen = sizeof(trbuf);
				res = trbuf_mac_nat( tw, pbuf, &inlen, trbuf, &trlen );
				if ( res && res != kTECPartialCharErr ) {
					// translation failed, leave data as-is
					trlen = tw->troutcount;
				} else {
					// translation ok, or no data yet
					if ( inlen ) {
						// keep a few chars
						for (j = inlen; j < tw->troutcount; j++)
							pbuf[j - inlen] = pbuf[j];
						tw->troutcount -= inlen;
					}
					if ( !trlen ) {
						// nothing yet
						if ( tw->troutcount < sizeof(tw->troutbuf) )
							continue;
						// temp translation buffer full, unable to translate...
						// flush data ?
						trlen = tw->troutcount;
						res = trflush_mac_nat( tw );
					} else {
						// translation complete
						pbuf = trbuf;
					}
				}
				tw->troutcount = 0;
				for ( j = 0; j < trlen; ++j )
					outbuf[outlen++] = pbuf[j];
			} else {
				outbuf[outlen++] = string[i];
			}
		}
		netpush(tw->port);
		kbflush( tw );
		netwrite(tw->port, outbuf, outlen);
		if (tw->echo)
			parse(tw, (unsigned char *)outbuf, outlen);
		len -= buflen;
		string += buflen;
	}

	tw->troutcount = 0;
}

// RAB BetterTelnet 2.0b3
// I added this routine because I was sick of special-casing the CR code
// all over the place.
void SendCRAsIfTyped(struct WindRec *tw)
{
	kbflush( tw );

	if (tw->echo) 
		parse(tw,(unsigned char *) "\012\015",2);	/* BYU LSC */

	if ((tw->protocol >= 1) && (tw->protocol <= 4)) { // for rlogin/rsh, ssh
		netwrite(tw->port,"\015",1);
		return;
	}

	if (tw->crmap) 
		netwrite(tw->port,"\015\0",2);
	else
		netwrite(tw->port,"\015\012",2); //UNIVAC fix
}

// RAB BetterTelnet 2.0b2 - rlogin_parse()
// parses urgent data for rlogin commands

void rlogin_parse (struct WindRec *tw, unsigned char *st, short cnt)
{
	short i;
	unsigned char *ptr;

	ptr = st;
	i = 0;

	while (i < cnt) {
		sprintf(munger, "rlogin: RECV urgent data <%d>", *ptr);
		opts_debug_print(munger);
		i++;
		ptr++;
	}
}

/*********************************************************************/
/*  parse
*   look at the string which has just come in from outside and
*   check for special sequences that we are interested in.
*
*   Tries to pass through routine strings immediately, waiting for special
*   characters ESC and 255 to change modes.
*/

void parse (struct WindRec *tw, unsigned char *st, short cnt)
{
	short i,temptw;
	unsigned char *mark,*orig;

//	ProfilerSetStatus(true);

	orig = st;				/* remember beginning point */
	mark = st + cnt;		/* set to end of input string */

/*
* raw mode for debugging, passes through escape sequences and other
* special characters as <27> symbols
*/
	if (tw->termstate == DUMBTYPE)
		{
		for (i=0; i < cnt; i++,st++)			/* put on screen raw */
			if (*st == 27 || *st > 126)
				{
				sprintf((char *)tw->parsedat,"<%d>",*st);
				VSwrite(tw->vs,(char *)tw->parsedat,strlen((char *)tw->parsedat));	/* send to virtual screen */
				}
			else
				VSwrite(tw->vs,(char *) st,1);		/* BYU LSC */
		}
	else
		{

/*
*  traverse string, looking for any special characters which indicate that
*  we need to change modes.
*/
	while (st < mark) 
		{
		switch (tw->telstate)
			{
			case GS:
			case ESCFOUND:
				if (tw->tektype >= 0) {	/* we'll allow the TEK */
					if ((*st == 12) || (tw->telstate == GS)) {	/* esc-FF */
						if ((*st == 12) && 
							((tw->termstate == VTEKTYPE) || (!tw->tekclear))) {
							if (tw->termstate == VTEKTYPE)
								putln("Entering Tek mode");
							else if (tw->curgraph > -1)
								detachGraphics(tw->curgraph);

							if (tw->curgraph <=  -1) {		// No current TEK window
								temptw = VGnewwin(TEK_DEVICE_WINDOW, tw->vs, tw->tektype, tw->tekclear);

								if (temptw > -1) {
									Str255	scratchPstring;
									
									tw->curgraph = temptw;
	
									VGgiveinfo(temptw);
									GetWTitle(tw->wind, scratchPstring);
									PtoCstr(scratchPstring);
									RGattach(temptw,tw->vs,(char *)scratchPstring, tw->tektype);
								}
								else
									tw->telstate = TS_NORM;  // Can't create TEK window
							}

							if (tw->telstate != TS_NORM)
								tw->termstate = TEKTYPE;
						}

						if (tw->telstate == GS) {
							st++;
							VGwrite(tw->curgraph,"\035",1);
						}
						else if (tw->telstate != TS_NORM)
							VGwrite(tw->curgraph,"\037\033\014",3);

						tw->telstate = TS_NORM;
						break;
					} // FF or GS
				} // tw->tektype >= 0
	
				if (*st == '^')	{			/* ESC- ^ */
					tw->termstate = RASTYPE;
					tw->telstate = TS_NORM;
					VRwrite("\033^",2);		/* Put it through */
					orig = ++st;
					break;
				}
				
				if (tw->termstate == TEKTYPE)
					VGwrite(tw->curgraph,"\033",1);
				else if (tw->termstate  == RASTYPE)
					VRwrite("\033",1);
				else
					VSwrite(tw->vs,"\033",1);	/* send the missing ESC */
				tw->telstate = TS_NORM;
				break;


/*------------------------------------------------------------------------------*
 * 	IACFOUND:  This is the start of the Telnet option negotiation.  If Telnet	*
 *	gets an IAC character, then negotiation data follows, and is ready to be	*
 * 	parsed. 																	*
 *------------------------------------------------------------------------------*/
			case IACFOUND: 		/* telnet option negotiation- START */
				if (*st == TEL_IAC) {	/* real data = 255 */
					orig = st;			// MP: translation mod (break will make it miss
										// this assignment later on in the case, Jim!)
					st++;				/* real 255 will get sent */
					tw->telstate = TS_NORM;
					break;
					}
				else {
					switch (*st++) {
						case TEL_EOF:		/* BYU LSC */
						case TEL_EL:				/* thanx Quincey!!! */
						case TEL_EC:
						case TEL_AYT:
						case TEL_AO:
						case TEL_IP:
						case TEL_BREAK:
						case TEL_DM:
						case TEL_NOP:
						case TEL_SE:
						case TEL_GA:
			#ifdef OPTS_DEBUG
							sprintf(munger,"RECV: %s",telstates[tw->telstate]);
							opts_debug_print(munger); 
			#endif
							tw->telstate = TS_NORM;
							orig=st;
							break;

						case TEL_DOTEL:
							tw->telstate = TS_DO;
							orig = st;
							break;

						case TEL_WILLTEL:
							tw->telstate = TS_WILL;
							orig = st;
							break;

						case TEL_DONTTEL:
							tw->telstate = TS_DONT;
							orig = st;
							break;

						case TEL_WONTTEL:
							tw->telstate = TS_WONT;
							orig = st;
							break;

			/*------------------------------------------------------------------------------*
			 * 	SUBNEGOTIATION -- If there is an SB token found, then Telnet needs to do 	*
			 *	some subnegotiating.  The subnegotiation data follows, and needs to be put	*
			 *	somewhere safe.  Make sure and update the state, so that we know that		*
			 *	Telnet is doing some subnegotiations and not just horsing around			*
			 *------------------------------------------------------------------------------*/
						case TEL_SB:
							tw->telstate = TS_SB;		/* Guess what I'm doing... */
							orig=st;
							tw->parseIndex = 0;		/* No data yet! Clear any possible garbage */
							break;
							
						default:		// Means we got IAC <xx> where xx isn't recognized
							orig = st;
							tw->telstate = TS_NORM;
#ifdef OPTS_DEBUG
							sprintf(munger,"RECVD: IAC %c",*(st-1));
							opts_debug_print(munger);
#endif							
							break;
					}	// switch (*st++)	
				} // else from it (*st == TEL_IAC)
			break; // case IACFOUND

            case TS_DO:
            	telnet_do(tw, *st++);
            	orig = st;
                tw->telstate = TS_NORM;
                break;

            case TS_DONT:
            	telnet_dont(tw, *st++);
            	orig = st;
				tw->telstate = TS_NORM;
            	break;

            case TS_WILL:
            	telnet_will(tw, *st++);
            	orig = st;
				tw->telstate = TS_NORM;
                break;

            case TS_WONT:
            	telnet_wont(tw, *st++);
            	orig = st;
                tw->telstate = TS_NORM;
                break;
			
			case TS_SB:
				if (*st == TEL_IAC) 
				{
					st++;
					if (*st == TEL_SE) //subnegotiation over 
					{
						st++;
						process_suboption(tw, st, mark);
						orig = st;
						tw->telstate = TS_NORM;
					}	
					else
					{
						if (*st == TEL_IAC) //doubled IAC
							tw->parsedat[tw->parseIndex++] = *st++;
						else
						{	//process this, then go IAC found
							
							tw->parsedat[tw->parseIndex++] = TEL_IAC; //why do we add this to 
							tw->parsedat[tw->parseIndex++] = *st;	 //the parsedat?   We don't 
							process_suboption(tw, st, mark);		//need it for the options
							tw->substat = IACFOUND;
						}
					}
				}
				else 	// Collect subnegotiation data
					tw->parsedat[tw->parseIndex++] = *st++;
				break;
										
            default:
                tw->telstate = TS_NORM;
                break;
		} // switch(tw->telstate)

/*
* quick scan of the remaining string, skip chars while they are
* uninteresting
*/
		if (tw->telstate == TS_NORM) {
/*
*  skip along as fast as possible until an interesting character is found
*/

			if (!tw->eightbit) {											/* BYU 2.4.10 */
				while (st < mark) {											/* BYU 2.4.10 */
					if ((*st == IAC) && (tw->protocol == 0)) // RAB BetterTelnet 2.0b2
						break;												/* BYU 2.4.10 */
					else {													/* BYU 2.4.10 */
						*st &= 0x7f; 										/* BYU 2.4.10 */
						if (*st == ESC  || *st == GS) 						/* BYU 2.4.10 */
							break;											/* BYU 2.4.10 */
						st++;												/* BYU 2.4.10 */
				}	}														/* BYU 2.4.10 */
			} else 															/* BYU 2.4.10 */
				while (st < mark && *st != ESC && ((*st < 255) || (tw->protocol != 0)) && *st != GS)
																			// MP: translation mod
																			// RAB BetterTelnet 2.0b2
					st++; 													/* BYU 2.4.10 */
/*
*  send the string where it belongs
*/
			if (!tw->timing) {
				if (tw->termstate == TEKTYPE) {
					
					i = VGwrite( tw->curgraph,(char *) orig,  st-orig);	/* BYU LSC */
					if (i < (st - orig)) {
						detachGraphics(tw->curgraph);
						st = orig + i;
						}
					}
				else if (tw->termstate == RASTYPE) {
					
					i= VRwrite((char *) orig, st-orig);		/* BYU LSC */
					if (i <(st-orig)) {
						tw->termstate = VTEKTYPE;
						st = orig +i;				/* Next char to parse */
						}
					}
				else {
					if (tw->otpauto)
						otpauto(tw, (char *) orig, st-orig);
					if (tw->waWaiting)
						handlewait(tw, (char *) orig, st-orig);
/* NONO */
					else if (tw->waWeHaveAppleEvent)
						handleread(tw, (char *) orig, st-orig);
/* NONO */


					VSwritefast( tw->vs,(char *) orig,st-orig);	/* BYU LSC - send to virtual VT102 */
					}
				}

			orig = st;				/* forget what we have sent already */

			if (st < mark)
				switch (*st) {
					case TEL_IAC:			/* telnet IAC */
						if (tw->protocol == 0) tw->telstate = IACFOUND; // RAB BetterTelnet 2.0b2
						st++;
						break;
					case GS:
						if (tw->telstate != GS) {
							tw->telstate = GS;
							}
						else
							tw->telstate = TS_NORM;
						st++;
						break;
						
					case ESC:			/* ESCape code */
						if (st == mark-1 || *(st+1) == 12 || *(st+1) == '^' ) {
							tw->telstate = ESCFOUND;
						}
						st++;			/* strip or accept ESC char */
						break;
	
					default:
						if (*st++ > 127) {
							if (st==mark)							/*new addition */
								VSwrite(tw->vs,(char *) orig,1);	/* BYU LSC */
	         				}
						break;
				}	// switch(*st)
			} // tw->telstate == TS_NORM
	} // while (st < mark)

	if (!tw->jsNoFlush)
		VSflushwrite(tw->vs);

	} // tw->termstate != DUMBTYPE
	
//	ProfilerSetStatus(false);
	
} /* parse */

void	SendNAWSinfo(WindRec *s, short horiz, short vert)
{
	char			blah[20];
	unsigned char	height, width;

	if (s->protocol != 0) return;

	height = vert & 0xff;
	width = horiz & 0xff;
	
    /* 931112, ragge, NADA, KTH, ugly patch to not send IAC as window size  */
	if(height == 0xFF) height = 0xFE;
	if(width == 0xFF) width = 0xFE;

	netpush (s->port);

/* NCSA: syntax for command is:  IAC SB NAWS widthHI widthLO heightHI heightLO IAC SE */

	netwrite(s->port,"\377\372\037\000",4);
	sprintf(blah,"%c\000", width);
	netwrite(s->port,blah,2);
	sprintf(blah,"%c\377\360", height);
	netwrite(s->port,blah,3);
	opts_debug_print("SENT: IAC SB NAWS <data> IAC SE");		
}

/*
 * telnet environment passing routines
 */

#define OPT_REPLY_SIZE  256
unsigned char opt_reply[OPT_REPLY_SIZE];
unsigned char *opt_replyp;

void
env_opt_start(void)
{
        opt_replyp = opt_reply;

        *opt_replyp++ = IAC;
        *opt_replyp++ = SB;
        *opt_replyp++ = N_NEW_ENVIRON;
        *opt_replyp++ = TNQ_IS;
}

void
env_opt_add(char *ep, char *vp)
{
        unsigned char c;

        if (opt_welldefined(ep))
                *opt_replyp++ = NEW_ENV_VAR;
        else
                *opt_replyp++ = ENV_USERVAR;

        for (;;) {
                while (c = *ep++) {
                        switch(c&0xff) {
                        case IAC:
                                *opt_replyp++ = IAC;
                                break;
                        case NEW_ENV_VAR:
                        case NEW_ENV_VALUE:
                        case ENV_ESC:
                        case ENV_USERVAR:
                                *opt_replyp++ = ENV_ESC;
                                break;
                        }
                        *opt_replyp++ = c;
                }

                if (ep = vp) {
                        *opt_replyp++ = NEW_ENV_VALUE;
                        vp = NULL;
                } else
                        break;
        }
}

int
opt_welldefined(char *ep)
{
        if ((strcmp(ep, "USER") == 0) ||
            (strcmp(ep, "DISPLAY") == 0) ||
            (strcmp(ep, "PRINTER") == 0) ||
            (strcmp(ep, "SYSTEMTYPE") == 0) ||
            (strcmp(ep, "JOB") == 0) ||
            (strcmp(ep, "ACCT") == 0))
                return(1);
        return(0);
}

void
env_opt_end()
{
        *opt_replyp++ = IAC;
        *opt_replyp++ = SE;
}

/*
 * Implementation specific Kerberos routines
 */

/*
 * getcname
 * Return a pointer to the cannonical host name
 */
static char	*getcname (WindRec *tw)
{
	char *cp;
	static char *b, buf[100];

	cp = 0;
	if (tw->cannon[0])
		cp = tw->cannon;

//	Doing the following is bad because we disposed of our init params!
//	else if ((*(*(ConnInitParams **)(tw->myInitParams))->session)->hostname)
//		cp = (char *)(*(*(ConnInitParams **)(tw->myInitParams))->session)->hostname;

	/* make a local copy to avoid locking handles */
	if (cp) {
		b = buf;		
		while (*cp)
			*b++ = *cp++;			
		*b++ = '\0';
		return buf;
	}

	return cp;
}


/*
 * encryptStatechange
 * Called when the encryption state changes
 */
#define kOurHit	32
static	void encryptStatechange (struct WindRec *tw)
{
	MyWDEFPatch(zoomDocProc, tw->wind, wDraw, kOurHit);
}

void	telnet_send_initial_options(WindRec *tw)
{

// RAB BetterTelnet 2.0b2 - revised to support multiple protocols

	if ((tw->protocol >= 1) && (tw->protocol <= 3)) { // initial rlogin stuff
		netwrite(tw->port, "\000", 1);
		if ((tw->protocol == 3) || (!tw->clientuser[0]))
			// rexec sends username, rlogin/rsh need client username
			// but we use server username if we don't have a client
			// username...
			netwrite(tw->port, &tw->username[1], tw->username[0]);
		else
			netwrite(tw->port, &tw->clientuser[1], tw->clientuser[0]);
		netwrite(tw->port, "\000", 1);
		if (tw->protocol == 3) // rexec sends password, rlogin/rsh send server username
			netwrite(tw->port, &tw->password[1], tw->password[0]);
		else
			netwrite (tw->port, &tw->username[1], tw->username[0]);
		netwrite(tw->port, "\000", 1);
		if (tw->protocol == 1) { // rlogin sends terminal type & speed, rsh/rexec send command
			netwrite(tw->port, &tw->answerback[1], tw->answerback[0]);
			netwrite(tw->port, "/9600\000", 6);
		} else {
			netwrite(tw->port, &tw->command[1], tw->command[0]);
			netwrite(tw->port, "\000", 1);
		}
		tw->echo = 0; // NO local echo for rlogin!
		tw->Usga = 1;
		return;
	}

	if (tw->protocol == 4) { // initial ssh stuff
		ssh_glue_initial(tw);
		tw->echo = 0; // NO local echo for ssh either!
		tw->Usga = 1;
//		return;
	} else if (tw->protocol != 0) {
		tw->echo = 1;
		tw->Usga = 1;
		return;
	}

	if (tw->authenticate && authOK) {
		send_will(tw->port, OPT_AUTHENTICATION);
		(tw->myopts)[OPT_AUTHENTICATION-MHOPTS_BASE] = 1;

		if (tw->encrypt && encryptOK) {
			send_will(tw->port, OPT_ENCRYPT);		/* will encrypt */
			(tw->myopts)[OPT_ENCRYPT-MHOPTS_BASE] = 1;
		}
	}

	if (tw->protocol == 4) {
		return;
	}

	if ((tw->portNum == 23) || tw->portNegative) {
		send_do(tw->port, N_ECHO);		// Do ECHO
		tw->echo = 1;

		send_do(tw->port, N_SGA);		// Do SGA
		tw->Usga=1;
	
		send_wont(tw->port, N_XDISPLOC);
	
		send_will(tw->port, N_NAWS);
	} else {
		tw->echo = 1;
		tw->Usga = 1;
	}
}
	
static	void	process_suboption(struct WindRec *tw, unsigned char *start, unsigned char *end)
{	
	unsigned long	sendlength;
	unsigned char	sendbuffer[1500];
	short			s;
	char			IACSB[] = { TEL_IAC, TEL_SB, 0, 0};
	char			IACSE[] = { TEL_IAC, TEL_SE};

	switch(tw->parsedat[0]) {
	/*------------------------------------------------------------------------------*
	* 	SUBNegotiate Termtype:  send the term type data now							*
	*------------------------------------------------------------------------------*/
		case N_TERMTYP:			
			if (tw->parsedat[1]==1) {
				char s[60], termtmp[40];
				BlockMoveData(tw->answerback, termtmp, 32);
				PtoCstr((StringPtr)termtmp);
#ifdef OPTS_DEBUG
				sprintf(munger,"RECV: SB TERMINAL-TYPE SEND\r\nSEND: SB TERMINAL-TYPE IS %s\r\n",termtmp);
				opts_debug_print(munger); 
#endif
				IACSB[2] = '\030';
				IACSB[3] = 0;
				netpush(tw->port);
				netwrite(tw->port,IACSB,4);
				netpush(tw->port);
				sprintf(s,"%s%c%c",termtmp, TEL_IAC, TEL_SE);
				netwrite(tw->port, s, strlen(s));
				}
			break;
				
/*------------------------------------------------------------------------------*
* 	SUBNegotiate ENCRYPTION:                                     		        *
*------------------------------------------------------------------------------*/
			case N_ENCRYPT:
				sendlength = sizeof(sendbuffer);
				s = encrypt_suboption((tnParams **)&tw->aedata, tw->parsedat,
									  tw->parseIndex,
									  sendbuffer, &sendlength, getcname(tw), 
									  tw->hisopts[OPT_ENCRYPT - MHOPTS_BASE], 
									  tw->myopts[OPT_ENCRYPT - MHOPTS_BASE]);
				netportencryptstate(tw->port, (Ptr)tw->aedata);
				
				/*
				 * If we turned on encryption, we must decrypt the rest of the buffer.
				 */
				if (s == TNREP_START_DECRYPT) {
					kdecrypt((tnParams *)tw->aedata, start, (long)(end-start));
				}

				if (sizeof(sendbuffer) - sendlength)
					netwrite(tw->port, sendbuffer, sizeof(sendbuffer)-sendlength);
				
				/*
				 * transfer the encrypting flag here, after the buffer 
				 * with encrypt-start has gone out.
				 */
				if (((tnParams *)tw->aedata)->startencrypting) {
					((tnParams *)tw->aedata)->startencrypting = false;
					((tnParams *)tw->aedata)->encrypting = true;
					}
					
				encryptStatechange(tw);
			break;
				
	/*------------------------------------------------------------------------------*
	* 	SUBNegotiate Authentication:  send the authentication data now     		    *
	*------------------------------------------------------------------------------*/
		case N_AUTHENTICATION:			
			sendlength = sizeof(sendbuffer);
			auth_suboption((tnParams **)&tw->aedata, tw->parsedat,
						   tw->parseIndex,
						   sendbuffer, &sendlength, getcname(tw), 
						   tw->hisopts[OPT_ENCRYPT-MHOPTS_BASE], 
						   tw->myopts[OPT_ENCRYPT-MHOPTS_BASE],
						   tw->port, tw->forward, tw->username);
			if (sizeof(sendbuffer) - sendlength) {
				netwrite(tw->port, sendbuffer, sizeof(sendbuffer)-sendlength);
				}
			break;
			
	/*------------------------------------------------------------------------------*
	* 	SUBNegotiate Linemode:  set up local characters, modes, and such			*
	*------------------------------------------------------------------------------*/
		case N_LINEMODE:
			linemode_suboption(tw);
			break;

	/*------------------------------------------------------------------------------*
	* 	SUBNegotiate REMOTE_FLOW_CONTROL:  determine whether we control flow, and   *
	*									   what restarts flow			 			*
	*------------------------------------------------------------------------------*/
		case N_REMOTEFLOW:
			switch (tw->parsedat[1])
			{

				case FLOW_OFF:
#ifdef OPTS_DEBUG
					sprintf(munger,"RECV: SB REMOTE_FLOW FLOW_OFF");
					opts_debug_print(munger); 
#endif
					tw->allow_flow = FALSE;
				break;
				
				case FLOW_ON:
#ifdef OPTS_DEBUG
					sprintf(munger,"RECV: SB REMOTE_FLOW FLOW_ON");
					opts_debug_print(munger); 
#endif	
					tw->allow_flow = TRUE;
				break;
				
				case FLOW_RESTART_ANY:
#ifdef OPTS_DEBUG
					sprintf(munger,"RECV: SB REMOTE_FLOW FLOW_RESTART_ANY");
					opts_debug_print(munger); 
#endif	
					tw->restart_any_flow = TRUE;
				break;
				
				case FLOW_RESTART_XON:
#ifdef OPTS_DEBUG
					sprintf(munger,"RECV: SB REMOTE_FLOW FLOW_RESTART_XON");
					opts_debug_print(munger); 
#endif	
					tw->restart_any_flow = FALSE;
				break;
				
				default:
				break;	
			}
 			break;
         /*------------------------------------------------------------------------------*
         *       SUBNegotiate Environment:  pass username as USER
         *
         *------------------------------------------------------------------------------*/
                 case N_NEW_ENVIRON:
                         switch (tw->parsedat[1])
                         {
                                 case TNQ_SEND:
#ifdef OPTS_DEBUG
                                         sprintf(munger, "RECV: SB NEW_ENVIRON SEND");
                                         opts_debug_print(munger);
#endif
                                         env_opt_start();
 
 										p2cstr((unsigned char *)tw->username);
 										env_opt_add("USER", tw->username);
 										c2pstr(tw->username);
 
                                         env_opt_end();
 
                                         netwrite(tw->port, opt_reply, opt_replyp - opt_reply);
                                         netpush(tw->port);
#ifdef OPTS_DEBUG
                                         sprintf(munger, "SENT: SB NEW_ENVIRON IS USER <value>");
                                         opts_debug_print(munger);
#endif
                                         break;
                                 default:
#ifdef OPTS_DEBUG
                                         sprintf(munger, "RECV: SB NEW_ENVIRON unsupported suboption");
                                         opts_debug_print(munger);
#endif
                         }
                         break;
		
		default: //dont know this subnegotiation!!
			break;
	}
}			

static	void	telnet_do(struct WindRec *tw, short option)
{
#ifdef OPTS_DEBUG
	sprintf(munger,"RECV: %s %s",telstates[TEL_DOTEL-TEL_SE],teloptions[option]);
	opts_debug_print(munger); 
#endif
	switch(option) {
		case  N_SGA:		/* Sure we'll supress GA */
			if (!tw->Isga) {
				tw->Isga=1;
				send_will(tw->port, N_SGA);
				}
			break;
		
		case N_TERMTYP:		/* And we'll even tell you about ourselves */
			if (!tw->Ittype) {
				tw->Ittype=1;
				send_will(tw->port, N_TERMTYP);
				}
			break;

		case  N_NAWS:			/* NCSA: sure, I like changing the window size! */
			tw->naws =1;		/* NCSA: this session is now NAWS */
			send_will(tw->port, N_NAWS);

			SendNAWSinfo(tw, VSmaxwidth(tw->vs) + 1, VSgetlines(tw->vs));
#ifdef OPTS_DEBUG
			opts_debug_print("SENT: IAC TEL_SB N_NAWS <data> IAC TEL_SE"); 
#endif
			break;											/* NCSA */

		case N_LINEMODE:  /* Sure I'll do line mode... */
			if (tw->lineAllow)  
			{
				send_will(tw->port, N_LINEMODE);
				doLinemode(tw);
			}
			else
				send_wont(tw->port, N_LINEMODE);
			break;

		case N_AUTHENTICATION:		/* do auth */
			if (!tw->myopts[OPT_AUTHENTICATION-MHOPTS_BASE]) {
				if (tw->authenticate && authOK) {
					(tw->myopts)[OPT_AUTHENTICATION-MHOPTS_BASE] = 1;
					send_will(tw->port, N_AUTHENTICATION);
					}
				else {
					send_wont(tw->port, N_AUTHENTICATION);
					}
			}
			break;

		case N_ENCRYPT: 			/* do encrypt */
			if (!tw->myopts[OPT_ENCRYPT-MHOPTS_BASE]) {
				if (tw->encrypt && encryptOK && tw->authenticate && authOK) {
					(tw->myopts)[OPT_ENCRYPT-MHOPTS_BASE] = 1;
					send_will(tw->port, N_ENCRYPT);
				} else {
					send_wont(tw->port, N_ENCRYPT);
				}
			}
			break;
		
 		case N_NEW_ENVIRON:
 			if (tw->username[0])
 				send_will(tw->port, N_NEW_ENVIRON);
 			else
 				send_wont(tw->port, option);
 			break;
 
		case N_REMOTEFLOW:
			if (!tw->remote_flow)
			{
				tw->remote_flow = TRUE;
				send_will(tw->port, option); 
			}			
			break;
			
		default:				/* But, we won't do .... */
			send_wont(tw->port, option);
			break;
		}
}

static	void	telnet_dont(struct WindRec *tw, short option)
{
#ifdef OPTS_DEBUG
	sprintf(munger,"RECV: %s %s",telstates[TEL_DONTTEL-TEL_SE],teloptions[option]);
	opts_debug_print(munger); 
#endif

    switch (option) {
		case N_ENCRYPT:					/* dont encrypt */
		case N_AUTHENTICATION:			/* dont authenticate */
			tw->myopts[option - MHOPTS_BASE] = 0;
			send_wont(tw->port, option);
            break;
		case N_LINEMODE:  /* Ok. turn it off if its on */
			if (tw->lmode)  
			{
				send_wont(tw->port, N_LINEMODE);
				kbflush( tw );
				tw->lmode = 0;
				tw->lmodeBits = 0;
				tw->litNext = 0;
			}
			break;


	}
}

static	void	telnet_will(struct WindRec *tw, short option)
{
#ifdef OPTS_DEBUG
	sprintf(munger,"RECV: %s %s",telstates[TEL_WILLTEL-TEL_SE],teloptions[option]);
	opts_debug_print(munger); 
#endif
    switch(option) 
    	{
        case N_ECHO:             /* Echo on the other end*/
#ifdef	OPTS_DEBUG
			if (!tw->echo)
				opts_debug_print("tw->echo is False.");
#endif
			if (!tw->echo)
				break;
			tw->echo = 0;	/* Ok, in that case they can echo... */
			changeport(scrn,scrn);
			send_do(tw->port, N_ECHO);
			break;
		
		case N_SGA:				/* Supress GA */
#ifdef	OPTS_DEBUG
			if (tw->Usga)
				opts_debug_print("tw->Usga is True.");
#endif
			if (tw->Usga)
				break;
			tw->Usga = 1;	/* Go Ahead and supress GA */
			send_do(tw->port, N_SGA);
			break;
		
		case N_TIMING:				/* Timing Mark */
			tw->timing = 0;
			break;
        
		case N_AUTHENTICATION:		/* will auth */
			if (!tw->hisopts[OPT_AUTHENTICATION-MHOPTS_BASE]) {
				if (tw->authenticate && authOK) {
					(tw->hisopts)[OPT_AUTHENTICATION-MHOPTS_BASE] = 1;
					send_do(tw->port, N_AUTHENTICATION);
				} else {
					send_dont(tw->port, N_AUTHENTICATION);
				}
			}
			break;

		case N_ENCRYPT: 			/* will encrypt */
			if (!tw->hisopts[OPT_ENCRYPT-MHOPTS_BASE]) {
				if (tw->encrypt && encryptOK && tw->authenticate && authOK) {
					(tw->hisopts)[OPT_ENCRYPT-MHOPTS_BASE] = 1;
					send_do(tw->port, N_ENCRYPT);
				} else {
					send_dont(tw->port, N_ENCRYPT);
				}
			}
			break;

		case N_REMOTEFLOW: 	/* they want to toggle flow control */
			if (!tw->remote_flow)
			{
				tw->remote_flow = 1;
#ifdef	OPTS_DEBUG
				opts_debug_print("tw->remote_flow is True.");
#endif
				send_do(tw->port, N_REMOTEFLOW);
			}

        default:
        	send_dont(tw->port, option);
    	}
}

static	void	telnet_wont(struct WindRec *tw, short option)
{
#ifdef OPTS_DEBUG
	sprintf(munger,"RECV: %s %s",telstates[TEL_WONTTEL-TEL_SE],teloptions[option]);
	opts_debug_print(munger); 
#endif
    switch(option) 
    	{         /* which option? */
        case N_ECHO:             /* echo */
			if (tw->echo)
				break;
            tw->echo = 1;	/* Ok, I will echo if I have to... */
			changeport(scrn,scrn);
            send_dont(tw->port,N_ECHO);
            break;
		
		case N_SGA:
			if (!tw->Usga)
				break;
			tw->Usga = 0;
           	send_dont(tw->port,N_SGA);
			break;
		
		case N_TIMING:				/* Timing Mark */
			tw->timing = 0;
			break;
        
		case N_ENCRYPT:					/* wont encrypt */
		case N_AUTHENTICATION:			/* wont authenticate */
			tw->hisopts[option-MHOPTS_BASE] = 0;
            send_dont(tw->port, option);
            break;

        default:
            break;
    	}
}

void	send_do(short port, short option)
{
	char	data[] = { IAC, TEL_DOTEL, 0};
	
	data[2] = option;
	netpush(port);
	netwrite(port, data, 3);
#ifdef OPTS_DEBUG
	sprintf(munger,"SENT: DO %s",teloptions[option]);
	opts_debug_print(munger); 
#endif
}

void	send_dont(short port, short option)
{
	char	data[] = { IAC, TEL_DONTTEL, 0};

	data[2] = option;
	netpush(port);
	netwrite(port, data, 3);
#ifdef OPTS_DEBUG
	sprintf(munger,"SENT: DONT %s",teloptions[option]);
	opts_debug_print(munger); 
#endif
}

static	void	send_will(short port, short option)
{
	char	data[] = { IAC, TEL_WILLTEL, 0};

	data[2] = option;
	netpush(port);
	netwrite(port, data, 3);
#ifdef OPTS_DEBUG
	sprintf(munger,"SENT: WILL %s",teloptions[option]);
	opts_debug_print(munger); 
#endif
}

static	void	send_wont(short port, short option)
{
	char	data[] = { IAC, TEL_WONTTEL, 0};
	
	data[2] = option;
	netpush(port);
	netwrite(port, data, 3);
#ifdef OPTS_DEBUG
	sprintf(munger,"SENT: WONT %s",teloptions[option]);
	opts_debug_print(munger); 
#endif
}

void otpauto(struct WindRec *tw, char *string, short len)
{
	short i;
	short templen;
	char *tempstring;

	templen = len;
	tempstring = string;

	while (templen) {
		switch (tw->otpautostate) {
			case 0: // waiting for otp prompt
			for (i = 0; i < 8; i++)
				tw->otpautobuffer[i] = tw->otpautobuffer[i + 1];
			tw->otpautobuffer[8] = *tempstring;
			if (!strncmp(tw->otpautobuffer, "otp-sha1 ", 9)) {
				tw->otpautostate = 1;
				strncpy(tw->otpautostring, "otp-sha1 ", 9);
				tw->otpautolength = 9;
			} else if (!strncmp(tw->otpautobuffer + 1, "otp-md5 ", 8)) {
				tw->otpautostate = 1;
				strncpy(tw->otpautostring, "otp-md5 ", 8);
				tw->otpautolength = 8;
			} else if (!strncmp(tw->otpautobuffer + 1, "otp-md4 ", 8)) {
				tw->otpautostate = 1;
				strncpy(tw->otpautostring, "otp-md4 ", 8);
				tw->otpautolength = 8;
			} else if (!strncmp(tw->otpautobuffer + 3, "s/key ", 6)) {
				tw->otpautostate = 1;
				strncpy(tw->otpautostring, "s/key ", 6);
				tw->otpautolength = 6;
			}
			break;

			case 1: // checking for sequence number
			if ((*tempstring >= '0') && (*tempstring <= '9')) {
				tw->otpautostate = 2;
				tw->otpautostring[tw->otpautolength] = *tempstring;
				tw->otpautolength++;
			}
			else tw->otpautostate = 0;
			break;

			case 2: // moving through sequence number
			if (tw->otpautolength > 250) {
				tw->otpautostate = 0;
				break;
			}
			if ((*tempstring >= '0') && (*tempstring <= '9')) {
				tw->otpautostring[tw->otpautolength] = *tempstring;
				tw->otpautolength++;
				break;
			}
			if (*tempstring == ' '){
				tw->otpautostate = 3;
				tw->otpautostring[tw->otpautolength] = ' ';
				tw->otpautolength++;
				break;
			}
			tw->otpautostate = 0;
			break;

			case 3: // moving through string
			if (tw->otpautolength > 250) {
				tw->otpautostate = 0;
				break;
			}
			if ((*tempstring <= ' ') || (*tempstring == ']')) {
				tw->otpautostring[tw->otpautolength] = 0;
				tw->otpautostate = 0;
				tw->otpauto = tw->otpmulti;
				otpinterface(1, tw->otpautostring, tw, tw->otphex, tw->otpnoprompt, tw->otppassword, tw->otpsavepass);
			} else {
				tw->otpautostring[tw->otpautolength] = *tempstring;
				tw->otpautolength++;
			}
			break;
		}
	templen--;
	tempstring++;
	}
}

// RAB BetterTelnet 2.0b1 - added handlewait() for scripting
// RAB BetterTelnet 2.0b2 - implemented Knuth-Morris-Platt plaintext search algorithm

void handlewait(struct WindRec *tw, char *string, short len)
{
	short templen;
	char *tempstring;

	templen = len;
	tempstring = string;

	while (templen) {
		while ((tw->waWaitPos > 0) && (tw->waWaitString[tw->waWaitPos] != *tempstring))
			tw->waWaitPos = tw->waPrefixFunction[tw->waWaitPos - 1]; // Knuth-Morris-Platt
		if (*tempstring == tw->waWaitString[tw->waWaitPos]) {
			tw->waWaitPos++;
			if (tw->waWaitPos >= tw->waWaitLength) {
				tw->waWaiting = 0;
				tw->enabled = tw->waWasEnabled;
				changeport(scrn, scrn);
				if (tw->waWeHaveAppleEvent) {
					tw->waWeHaveAppleEvent = 0;
					AEResumeTheCurrentEvent(&tw->waAppleEvent, &tw->waAEReply,
						MyHandleWaitUPP, 1);
				}
			}
		} else tw->waWaitPos = 0;
		templen--;
		tempstring++;
	}
}

void handleread(struct WindRec *tw, char *string, short len)
{
	/* add 'string' to AppleEvent reply */
	OSErr err = AEPutParamPtr(&tw->waAEReply, keyDirectObject, typeChar, string, len);
	tw->waWeHaveAppleEvent = 0;
	tw->enabled = tw->waWasEnabled;
	AEResumeTheCurrentEvent(&tw->waAppleEvent, &tw->waAEReply,
		MyHandleReadUPP, (err == noErr) ? 1 : 2);
}

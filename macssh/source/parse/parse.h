#define OPTS_DEBUG

/* These are the different Telnet option negotiation tags */
#define TEL_EOF			236			/* BYU LSC */
#define TEL_SUSP		237
#define TEL_ABORT 		238			//CCP for linemode rfc
#define	TEL_SE			240			/* end sub-negitiation */				// 360
#define	TEL_NOP			241			/* No Operation */
#define TEL_DM			242			/* data mark */
#define TEL_BREAK		243			
#define TEL_IP			244			/* the ole' Interrupt Process */
#define TEL_AO			245			/* Abort Output */
#define	TEL_AYT			246			/* Are You There???? */
#define	TEL_EC			247			/* Erase Char */
#define TEL_EL			248			/* Erase Line */
#define TEL_GA			249			/* Telnet Go-Ahead */
#define	TEL_SB			250			/* start a Subnegotion */				// 372
#define TEL_WILLTEL 	251			/* Negotiate:  Will do this option */	// 373
#define TEL_WONTTEL 	252			/* Negotiate:  Wont do this option */	// 374
#define TEL_DOTEL		253			/* Negotiate:  Do this option */		// 375
#define TEL_DONTTEL 	254			/* Negotiate:  Dont do this option */	// 376
#define	TEL_IAC			255

/* these are the rest of the defines needed for the option negitiations */
#define TS_NORM		0			/* parsing data normal */
#define	TS_SB		1			/* Subnegotiation data follows */
#define	TS_SE		2
#define	TS_WILL		3
#define	TS_WONT		4
#define	TS_DO		7
#define	TS_DONT		8
#define ESCFOUND 			5			/* we have found an ESC sequence */
#define IACFOUND 			6			/* negotiation data follows */
#define	SEXECUTE			200
#define	IAC					255			/* signals negotiation data is coming */
#define GS					29			/* we can drop into TEK from here */


/* these are some of the various options that we can negotiate about */
#define N_BINARY 			0			/* binary data transfer */
#define N_ECHO				1			/* for local echo stuff */
#define N_SGA				3			/* Go ahead */
#define N_STATUS			5		
#define N_TIMING			6			/* timing mark */
#define	N_TERMTYP			24			/* set the terminal type */
#define N_NAWS				31			/* Negotiate About Window Size */
#define	N_TERMSPEED			32			/* how fast can we go? */	
#define N_REMOTEFLOW		33			/* do Remote Flow Control */
 //flow control suboptions
	#define		FLOW_OFF            0	// dont allow flow control  
    #define    	FLOW_ON             1	// allow flow control
    #define    	FLOW_RESTART_ANY    2	// restart flow on any character
    #define    	FLOW_RESTART_XON    3	// restart flow only on XON


#define N_LINEMODE 			34			/* Ah yes, the infamous Line Mode option */
	#define L_MODE 		1
	#define L_FORWARDMASK 2
	#define L_SLC 		3
#define	N_XDISPLOC			35
#define N_AUTHENTICATION	37			/* Authentication */
#define N_ENCRYPT			38			/* Encryption */
#define N_NEW_ENVIRON		39

/* values for NEW ENVIRON */
#define NEW_ENV_VAR    0
#define NEW_ENV_VALUE  1
#define ENV_ESC        2
#define ENV_USERVAR    3

/* Values for LINEMODE MODE */
#define L_EDIT    	1
#define L_TRAPSIG  	2
#define L_MODE_ACK	4
#define	L_SOFT_TAB	8
#define	L_LIT_ECHO	16
#define	L_MODE_MASK	31

/* these are the local keys, defined right before our very eyes!! */
/* used for line-mode negotiations */
#define SLC_NOSUPPORT	0
#define SLC_CANTCHANGE	1
#define SLC_VALUE		2
#define SLC_DEFAULT		3
#define SLC_LEVELBITS	3
#define SLC_FLUSHOUT	32
#define SLC_FLUSHIN		64
#define SLC_AWK			128

#define	SLC_MAX			30

#define SLC_SYNCH		1
#define SLC_BRK			2
#define SLC_IP			3
#define SLC_AO			4
#define SLC_AYT			5
#define SLC_EOR			6
#define SLC_ABORT		7
#define SLC_EOF			8
#define SLC_SUSP		9
#define SLC_EC			10
#define SLC_EL   		11
#define SLC_EW   		12
#define SLC_RP			13
#define SLC_LNEXT		14
#define SLC_XON			15
#define SLC_XOFF		16
#define SLC_FORW1		17
#define SLC_FORW2		18

#define DEFAULT_SLC_AO 		21
#define DEFAULT_SLC_IP 		3
#define DEFAULT_SLC_EC 		127
#define DEFAULT_SLC_EL 		21
#define DEFAULT_SLC_EOF 	4
#define DEFAULT_SLC_ABORT   28
#define DEFAULT_SLC_SUSP 	26
#define DEFAULT_SLC_EW 		23
#define DEFAULT_SLC_RP 		18
#define DEFAULT_SLC_LNEXT 	22
#define DEFAULT_SLC_XON 	17
#define DEFAULT_SLC_XOFF 	19
#define DEFAULT_SLC_FORW1 	22
#define DEFAULT_SLC_FORW2 	254
#define DEFAULT_SLC_BRK		255
#define	substate	tw->substat

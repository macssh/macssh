//
//		MacLook Window Management Data Structure Definition
//

#ifndef __WIND__
#define __WIND__

#include <pthread.h>
#include <TextEncodingConverter.h>

#define	MAXKB	256			// BYU mod 

#define PASTE_IN_PROGRESS	0x4000 // BYU LSC 
#define PASTE_DONE			0x0040 //CCP 2.7
//	The following are for the "active" field.
#define	CNXN_NOTINUSE	-2
#define	CNXN_DNRWAIT	-1
#define	CNXN_ACTIVE		 0
#define	CNXN_OPENING	 1
#define	CNXN_ISCORPSE	 2
#define MHOPTS_BASE		37	// Base option for {my,his}opts (Authenticate)
							//  {my,his} opts should only be used for telnet options
							//  in the range starting at MHOPTS_BASE and limited
							//  by MHOPTS_SIZE. This saves memory.
#define MHOPTS_SIZE		2	// Number of options supported in {my,his}opts

#define SLC_ARRAY_SIZE	30	// This should match the value of SLC_MAX from parse.h

// RAB BetterTelnet 2.0b2 - the struct for ssh stuff

typedef struct sshinfo {
	Str255		host;
	Str255		login;
	Str255		password;
	Str255		command;
	char		currentpass[256];
	UInt32		ip;			// no v6...
	pthread_t	thread;
	void		*context;
} sshinfo;

struct WindRec {
short	
	vs,				// virtual screen number
	port,			// TCP/IP port number 
	active,			// See above for definition of possible values 
	enabled,		// Are we currently enabled for receive? 

	vtemulation,	// 0 = VT100, 1 = VT 220 
	bsdel,			// backspace or delete is default 
	eightbit,		// eight bit font displayed (false is seven bit display
	innational,		// translation table to use for this connection for input
	outnational,	// translation table to use for this connection for output
	arrowmap,		// MAT: should we allow the arrow keys to be mapped?? 
	showErrors,		// show ALL errors if this is set 
	pgupdwn,		// JMB/MAT: should we have page up/down do local window movement? 
	keypadmap,		// CCP 2.7: should we have numeric keypad operators work like regular operators? 
	emacsmeta,		// JMB/SMB:	should option key work as EMACS meta key? 
	Xterm,			// JMB/WNR:	should Xterm sequences be recognized? 
	remapCtrlD,		// RAB BetterTelnet 1.0fc4: remap Del to ^D
	halfdup,		// If true then half-duplex mode 
	forcesave,		// NCSA 2.5: force lines to be saved 
	crmap,			// BYU mod - CR's second byte is ...

	tekclear,		// Does tektronix (1) clear screen or (0) add window 
	tektype,		// -1 = TEK not allowed, 0 = 4014, 1 = 4105 
	curgraph,		// Associated Tek drawing 
	qprint,			// Queue print requests
	ignoreff,		// RAB BetterTelnet 1.0fc8 - ignore FF

	maxscroll,		// Requested amount of scroll back 
	wrap,			// whether vtwrap is set or not for this screen 
	echo,			// NOTE: this is backwards - I am echoing your chars 
	ESscroll,		// will Clear Screen Save Lines? 

	ANSIgraphics,	// ANSI color sequences (CCP 2.7)	
	termstate,		// Emulation State (-1 is none, 0 is VT100) 
					// = TEKTYPE, send data to VGwrite 
					// = RASTYPE, send data to VRwrite 
	positionIndex,	// CCP 2.7:  for determining window placement
	naws,			// NCSA: will negotiate NAWS 
	lineAllow,		// allow linemode? 
	Isga,			// I am supressing go ahead 
	Usga,			// You are supressing go ahead 
	Ittype,			// I am sending terminal type negotiations.. 
	telstate,		// Telnet State 
	timing,			// True if waiting for timing mark 
	remote_flow,	// true if they are dictating flow_control 
	allow_flow,  	// true unless prior is true AND the host has told us to turn flow control off 
	restart_any_flow,// true if any character (except XOFF) will restart flow 
	substat,		// Telnet Subnegot. State 
	parseIndex,		// save an index into the subnegotiation parsed data 

	portNum,		// port number from the application save set
	portNegative,	// port number is negative: RAB BetterTelnet 1.0fc4
	allowBold,		// allow font boldfacing: RAB BetterTelnet 1.0fc4
	colorBold,		// use color for bold: RAB BetterTelnet 1.0fc4
	realbold,
	ignoreBeeps,	// ignore beeps: RAB BetterTelnet 1.0fc7
	inversebold,	// use inverse for bold: RAB BetterTelnet 1.0fc9
	kblen,			// Pointer to next char in buffer to be used 
	clientflags,	// BYU mod - boolean flags for ftp client
	jsNoFlush,		// RAB BetterTelnet 2.0b4 - don't flush jump scroller at all
	realBlink;

short
	otpautostate,
	otpauto,
	otpnoprompt,
	otpmulti,
	otphex,
	otpautolength,
	otpsavepass,
	oldScrollback;

char
	otpautobuffer[10],
	otppassword[64],
	otpautostring[256];

short
	cachedFontNum,
	cachedBoldFontNum;

short cxWeHaveAppleEvent;
AppleEvent cxAppleEvent, cxAEReply;

short
	waWeHaveAppleEvent,
	waWaiting,
	waWaitPos,
	waWasEnabled,
	waWaitLength;

char
	waWaitString[256],
	waPrefixFunction[256];

AppleEvent
	waAppleEvent, waAEReply;

char
	username[256],
	password[256],
	clientuser[256],
	command[256];

short
	protocol, encryption;

sshinfo sshdata;

short
	jumpScroll;

NewMacroInfo newMacros;

short
	usesocks,
	socksinprogress,
	socks4a,
	socksport,
	sockspos,
	sockslookup;

char
	sockshost[256],
	socksusername[256];

ip_addr
	actualIP,
	socksIP;

Str63
	machine;		// Name of machine we are connecting to

Str32
	answerback;		// Message to send when server sends TERMTYPE Telnet option 

// 	The following are used during paste operations 
long
	outlen;			// Length of text remaining to be pasted (can be > 32K)
short
	pastemethod,	// 0 = All at once, 1 = in blocks of size pasteblock 
	pastesize;		// Size of paste "blocks" 
	
char
	*outptr,		// Pointer to next char to send 
	**outhand;		// Handle to text from the Paste 

long   
	incount,		// BYU LSC - Count of bytes into this port 
	outcount;		// BYU LSC - Count of bytes out this port 

short
	isUploading,	// RAB BetterTelnet 1.0fc9 - file upload in progress
	uploadRefNum;	// RAB BetterTelnet 1.0fc9 - refNum for file upload

//linemode options
short
	litNext,		// next key should be echo'd literal
	lmodeBits,		// Current linemode MODE.  Currently support EDIT and TRAPSIG 
	lmode,			// linemode is active 
	forwardMask,	// should we forward on certain characters
	numForwardKeys; // how many keys to forward on
unsigned char	
	slcLevel[SLC_ARRAY_SIZE+1], //levelBits
	forwardKeys[32],// which keys to forward on 
	slc[SLC_ARRAY_SIZE+1]; //actual values

char
	TELstop,		// Character for scrolling to stop 
	TELgo,			// Character for scrolling to go 
	TELip,			// Character for interrupt process 
	kbbuf[MAXKB];	// The keyboard buffer (echo mode ) 
	
GrafPtr 
	wind;
	
Handle
	myInitParams;	//	So we can look up stuff after we have DNS'd.

Ptr
    aedata;

unsigned char
	myopts[MHOPTS_SIZE],	// My telnet options. Warning: see MHOPTS_BASE
	hisopts[MHOPTS_SIZE],	// His telnet options. Warning: see MHOPTS_BASE
	parsedat[450];			// save the subnegotiation data here
						    //   must be big enough for a Kerberos AP message

char
	cannon[80];		// Cannonical hostname

Boolean
	authenticate,	// true if authenticating connection wanted
	encrypt,		// true if encrypting connection wanted
	forward;                // true if fowarding wanted

NewMacroInfo sessmacros;

/* NONO */
short
	authentication,
	compression,
	verbose,
	trace,
	debug,
	restricted,
	ssh2method,
	ssh2guests,
	localport,
	remoteport;
char
	remotehost[180];
char
	display[64];	// x11 display

short
	incharset,		// current translation state for multi-byte translation
	outcharset,		// current translation state for multi-byte translation
	troutcount;

unsigned char
	troutbuf[16];	// chars pending in translation

TECObjectRef
	fromconverter,
	toconverter;

Boolean
	launchurlesc,	// true if we accept to process 'launchurl' escape sequences
	x11forward,
	autoreconnect,
	keepselection;
/* NONO */

};

typedef struct WindRec WindRec, * WindRecPtr;

#endif

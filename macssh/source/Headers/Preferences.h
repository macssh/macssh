#define	PREFERENCES_FILE_NAME_STR_ID	1991	// STR resource containing "BetterTelnet
												//	Prefs2" or translated name
#define OUR_PREFS_VERSION_NUMBER 2
#define NO_TIMEOUTS_VERSION		 1

/*

HEY!
READ THIS BEFORE YOU EVEN _THINK_ ABOUT CHANGING THE PREFS FILE FORMAT...
BetterTelnet 2.0b2 changes the way the prefs work. It used to be that we had a certain
amount of padding which had to be reduced each time we added a prefs option. Once we
were out of padding, too bad, tough noogies, we had to scrap the whole prefs file and make
a new one with a longer resource of that particular type. This happened once during
the BetterTelnet 1.0 beta cycle, and thus the file is named "BetterTelnet Prefs2" and
the terminals resource is 'TeR2' and not 'TeRm'. But no more. So, here are the guidelines
for modifying the NEW prefs system:

1. Padding stays at 100 words, or 200 bytes. This is only for backward compatibility, but
   DO NOT reduce the padding when adding new options, because we WANT the resource to expand,
   and BetterTelnet 2.0b2 and later will take care of enlarging it for us.

2. Change the size of the master resource in the application to match this.

3. Don't worry about backwards compatibility. All versions of BetterTelnet prior to 2.0b1
   did all memory copies to and from the prefs resources using the size of their prefs
   definition and not the actual size of the resource. Thus, they'll simply ignore the larger
   resource size, no problem.

4. Don't worry about forwards compatibility, either. 2.0b2 and later will automatically
   enlarge the resource if needed to fix their prefs definition but will never shrink it.
   Since all memory copies are done with the size of the prefs definition, there's still
   no problem.

5. 2.0b1 may crash with the 2.0b2 resource format, due to a bug. Tell users not to use 2.0b1
   anymore and there won't be a problem. Just use 2.0b2, or later.

6. When adding a prefs option, remember that ALL ZEROES are the default for new options.
   DON'T assume that what you put in the master resource will automatically be loaded into
   the user's resources, because it won't unless the user deletes his/her prefs file. So,
   structure your prefs code so that all zeroes in a field will always invoke the default
   option.

7. This applies to all prefs resources except those for the FTP server.

8. And remember, always add new prefs at the end of the old ones, before the padding but
   after everything else.

*/

#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=mac68k
#endif

// Name of each instance is the resource name.  Resource type TeR2
typedef	struct {
	short
		version;				// Version of this structure
		
	RGBColor
		nfcolor,				// Normal foreground RGB
		nbcolor,				// Normal background RGB
		bfcolor,				// Blinking foreground RGB
		bbcolor,				// Blinking background RGB
		reserved1,				// Reserved by JMB - NCSA
		reserved2;				// Reserved by JMB - NCSA
		
	short
		vtemulation,			// 0 = VT100, 1 = VT220 (supposedly)
		vtwidth,				// Width of the terminal screen in characters
		vtheight,				// Height of the terminal screen in characters
		fontsize,				// Size of DisplayFont to use to display text
		numbkscroll;			// Number of lines to save in scroll buffer
		
	Boolean
		ANSIgraphics,			// Recognize ANSI color sequences
		Xtermsequences,			// Recognize Xterm sequences
		vtwrap,					// Use VT wrap mode
		emacsmetakey,			// 0 = off, 1 = cmd-control, 2 =  option
		emacsarrows,			// Arrow keys and mouse position are EMACS flavor
		MATmappings,			// Map PageUp, PageDown, Home, End. (MAT == Mark Tamsky)
		eightbit,				// Don't strip the high bit
		clearsave;				// Save cleared lines
		
	Str63
		DisplayFont;			// Font to use to display text
	
	Str32
		AnswerBackMessage;		// Response to send when asked what terminal we are
	Boolean 
		remapKeypad;			//remap keypad (2.7 CCP)
	short
		remapCtrlD,				// remap Del to ^D (RAB BetterTelnet 1.0fc4)
		allowBold,				// allow boldface font (RAB BetterTelnet 1.0fc4)
		colorBold;				// use color for boldface (RAB BetterTelnet 1.0fc4)
	Str63
		BoldFont;
	short
		boldFontSize,
		boldFontStyle,
		realbold,
		oldScrollback,
		jumpScroll,
		realBlink,
		vt7bits,
		hideScrollBars;
	short
		padding[98];			// So I'm generous....
}	TerminalPrefs;
#define	TERMINALPREFS_RESTYPE	'TeR2'
#define	TERMINALPREFS_APPID		1991		// The "<Default>" terminal seed copy
#define	JPTERMINALPREFS_APPID	1992		// The "<Default>" terminal seed for japan

// Name of each instance is the resource name.  Resource type SeSn
typedef	struct {
	short
		version;				// Version of this structure
		
	short
		port,					// Port to connect to
		tektype,				// -1 = inhibit, 0 = 4014, 1 = 4105
		pastemethod,			// 0 = quick, 1 = block
		pasteblocksize;			// Size of blocks to paste if using block mode
	unsigned long
		ip_address;				// IP address of the host

	Boolean
		forcesave,				//
		crmap,					// Strange Bezerkley 4.3 CR mode needed
		linemode,				// Allow linemode to be used
		showlowlevelerrors,		// Not Used anymore
		tekclear,				// Clear TEK window vs. create new one
		halfdup;				// Half duplex required
		
	char
		bksp,					// 0 DELETE = BKSP, 1 DELETE = DELETE
		ckey,
		skey,
		qkey;

	Str32
		TerminalEmulation,		// Name of terminal emulator to use
		TranslationTable;		// Name of translation table to use by default

	Str63
		hostname;				// The DNS name of the host
		
	Boolean
		authenticate,			// Kerberos authentication
		encrypt,				// Encrypted session
		localecho,				// Force local echo on?
		forward;				// Reserved by JMB - NCSA
	
	short
		NetBlockSize,			// Size of read buffer (2.7 CCP)
		portNegative,			// Port number is negative (force use of initial Telnet codes)
								// (RAB BetterTelnet 1.0fc4)
		alwaysBSD,				// always use BSD 4.3 CR mode, even on non-port-23
								// (RAB BetterTelnet 1.0fc4)
		ignoreBeeps,			// Ignore Beeps (RAB BetterTelnet 1.0fc7)
		ignoreff,				// Ignore Form Feeds (RAB BetterTelnet 1.0fc8)
		otpauto,				// Automatic OTP - RAB BetterTelnet 1.2
		otpmulti,				// RAB BetterTelnet 1.2
		otpnoprompt,			// RAB BetterTelnet 1.2
		otphex;					// RAB BetterTelnet 1.2

	char
		otppassword[64];		// RAB BetterTelnet 1.2
	short
		otpsavepass;			// RAB BetterTelnet 1.2.1

// Now we bring you a whole bunch of NEW stuff for BetterTelnet 2.0b2... (RAB)

	char
		username[256],
		password[256],
		clientuser[256],
		command[256];

	short
		protocol; // Telnet = 0, rlogin = 1, rsh = 2, rexec = 3, ssh = 4, raw TCP = 5

	short
		encryption; // for SSH: 3DES = 0, DES = 1, Blowfish = 2, IDEA = 3, none = 4

// and now for BetterTelnet 2.0fc1 (SOCKS stuff)...

	short
		usesocks, // boolean - do we use SOCKS 4 for firewalling connections?
		socks4a, // boolean - do we use SOCKS 4a for firewalling DNS lookups?
		socksport; // the socks port (what else?) - default of 0 = actually 1080
	char
		socksusername[256], // and now more gratuitous disk wastage for strings :P
		sockshost[256];

/* NONO */
/* added fields below... */
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

	Boolean
		launchurlesc,
		x11forward;

	char
		display[64];			// x11 display name (usually "0.0")

	Boolean
		autoreconnect;
	Boolean
		keepselection;
	short
		padding[99];			// Disk space is cheap.... (used to be [98])
/* NONO */

}	SessionPrefs;
#define	SESSIONPREFS_RESTYPE	'SeSn'
#define SESSIONPREFS_APPID		1991		// The "<Default>" session seed copy
#define JPSESSIONPREFS_APPID	1992		// The "<Default>" session seed for japan


// Only one instance of this resource type.  Resource type PrEf
typedef	struct	{
	short
		version;				// Version of this resource
		
	short
		CursorType,				// 0 = block, 1 = underscore, 2 = vertical line
		CopyTableThresh;		// Copy table threshold
		
	long
		TimeSlice;				// Timeslice passed to WaitNextEvent
		
	ResType
		CaptureFileCreator;		// Creator signature for session capture files
		
	Boolean
		WindowsDontGoAway,		// Connection's windows remain open after connection close
		StaggerWindows,			// Stagger connection windows
		CommandKeys,			// Use command key shortcuts for menus
		RemapTilde,				// Remap Ò~Ó to escape
		BlinkCursor,			// Blink the cursor
		KeyPadAndFuncMenus;		// Show Keypad and Function key menus

	short
		OpenTimeout,			// Time to Open Connections in seconds (CCP 2.7)
		SendTimeout,			// Time to Send Data (and receive ACK) (CCP 2.7)
		StaggerWindowsOffset,   // Allow user to specify how much windows are staggered
		NotifyUser,
		destroyKTickets,		//destroy kerberos tickets when window closes
		autoOpenDialog,
		dontWarnOnQuit,
		autoOpenDefault,
		parseAliases,
		monospacedOut,
		clipTrailingSpaces,
		defaultBoldColor,
		globalSavePass,
		noLiveScrolling;

/* NONO */
	Boolean
		cachePassphrase,		// 
		useKeyChain,			// 
		dontWarnOnClose,
		unused1;
	long
		cacheDelay;				// delay in seconds before passphrase expires
/* NONO */

	short
		padding[96];			// Memory is getting cheap too.... (used to be [96])
}	ApplicationPrefs;

#define	APPLICATIONPREFS_RESTYPE	'PrEf'
#define	APPLICATIONPREFS_ID			1991
#define	APPLICATIONPREFS_APPID		1990

extern	ApplicationPrefs	*gApplicationPrefs;

// Only one instance of this structure.  Resource FTPs
typedef	struct {
	short
		version;				// Version of this resource

	short
		ServerState;			// 0 = off, 1 = no protection, 2 = password protection
		
	OSType
		BinaryCreator,			// Default creator for files transferred in binary mode
		BinaryType,				// Default type for files transferred in binary mode
		TextCreator;			// Default creator for file transferred in ASCII mode
		
	Boolean
		ShowFTPlog,				// FTP log defaults to visible
		DNSlookupconnections,	// Translate IP of connecting hosts to DNS names for log
		UseMacBinaryII,			// Allow MacBinary II
		unused1,				// Unused
		unused2,				// Unused
		ResetMacBinary,			// Reset MacBinary after each transfer
		DoISOtranslation;		// Use ISO translation

	short
		padding[100];
}	FTPServerPrefs;
#define	FTPSERVERPREFS_RESTYPE	'FTPs'
#define	FTPSERVERPREFS_ID 		1991
#define	FTPSERVERPREFS_APPID 	1990

extern	FTPServerPrefs	*gFTPServerPrefs;

// Username is the resource name.  Resource type FTPu
typedef	struct {
	short
		version;				// Version of this resource
	Str32
		EncryptedPassword,		// Encrypted form of user's password
		DefaultDirVolName;		// Name of the volume containing user's default directory
		
	long
		DefaultDirDirID;		// DirID of the user's default directory

	Boolean
		UserCanCWD;				// User is allowed to change directories
	Boolean
		usused1;				// User is allowed to change directories
		
	short
		padding[20];			// Some padding for the future
}	FTPUserPrefs;
#define	FTPUSERPREFS_RESTYPE	'FTPu'
#define FTPUSERPREFS_APPID		1991		// The "<Default>" session seed copy

// >>>> FTP Client Prefs? <<<	

typedef	struct {
	TerminalPrefs
		**terminal;
	SessionPrefs
		**session;
	Str255
		WindowName;
	short
		ftpstate;
	Rect
		WindowLocation;
	NewMacroInfo
		sessmacros;
}	ConnInitParams;


#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=reset
#endif

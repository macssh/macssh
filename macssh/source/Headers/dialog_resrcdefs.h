/*================================================================================*/
/*================================================================================*/
/*	#defines for ALRT and DITL resources */

#define	MemoryLowAlert	200	// Preloaded, unpurgable alert to tell the user memory is tight

/*	#defines for the DLOG and DITL resources */

#define	DLOGOk		1
#define DLOGCancel	2

#define RESOLVERERROR	128
 
//list modal dialog
#define	kItemDLOG	150
#define	kRemove		2
#define	kChange		3
#define	kNew		4
#define	kItemList	5
#define	kDuplicate	6

#define kBadPrefNameDLOG 151	//when a name for a session or terminal is a duplicate
#define	FATALCANCEL_ID	160		// Fatal Cancel Alert
#define	FATAL_ID		170		// Fatal Alert
#define OPFAILED_ID		175		// Operation Failed Alert
#define	ASKUSER_ID		180		// Ask user an OK/CANCEL question ALERT

#define	WinTitlDLOG		190		// Change window title DLOG
#define	kWinNameTE		3		// Window Name TextEdit

#define	DNRErrorDLOG	195		// DNR error occurred.

#define	MyIPDLOG		259

#define	StatusDLOG		270		// "Machine" is currently being "opened/looked up"

#define SetupDLOG		273		/*  Setup keys interface */
#define killbox			4
#define stopbox			5
#define startbox		6

#define	NewCnxnDLOG		280
#define	NCconnect		1
#define	NCcancel		2
#define	NChostname		4
#define	NCwindowname	6
#define	NCsesspopup		7
#define NCauthenticate	8
#define NCencrypt		9
#define NCfavoritename	10
#define NCssh2			11
//#define NCtermpopup		12
#define NCforward		12
#define NCusername		13

#define TermPopupMenu	1004	
#define SessPopupMenu	1005	

#define	ConnFalDLOG		303

#define MacroDLOG		500		/* The Macros dialog */
#define MacroImport		23
#define MacroExport		24

#define	AboutDLOG		501
#define OopsDLOG		640
#define	FirstDLOG		1023

#define	ColorDLOG		1001	/* Connection color selection dialog */
#define	ColorNF			3
#define	ColorNB			4
#define	ColorBF			5
#define	ColorBB			6

#define ANSIColorDLOG   1002


#define	ANSIBlack		15
#define	ANSIRed			16
#define	ANSIGreen		17
#define	ANSIYellow		18
#define	ANSIBlue		19
#define	ANSIMagenta		20
#define	ANSICyan		21
#define	ANSIWhite		22
#define	ANSIBoldBlack	23
#define	ANSIBoldRed		24
#define	ANSIBoldGreen	25
#define	ANSIBoldYellow	26
#define	ANSIBoldBlue	27
#define	ANSIBoldMagenta	28
#define	ANSIBoldCyan	29
#define	ANSIBoldWhite	30

#define	ANSIBlackRadio	31
#define	ANSIRedRadio	32
#define	ANSIGreenRadio	33
#define	ANSIYellowRadio	34
#define	ANSIBlueRadio	35
#define	ANSIMagentaRadio 36
#define	ANSICyanRadio	37
#define	ANSIWhiteRadio	38
#define	ANSIBoldBlackRadio 39
#define	ANSIBoldRedRadio 40
#define	ANSIBoldGreenRadio 41
#define	ANSIBoldYellowRadio 42
#define	ANSIBoldBlueRadio 43
#define	ANSIBoldMagentaRadio 44
#define	ANSIBoldCyanRadio 45
#define	ANSIBoldWhiteRadio 46
#define	ANSIForegroundRadio 47



#define	CloseDLOG		2001	/* The Close Confirmation Dialog */

#define WDSET_DLOG		4001

#define	SizeDLOG		5500	/* The number of lines config dialog ----------- */
#define	ColumnsNumber	4		/* NCSA: SB - User given # of columns */
#define	LinesNumber		6		/* User given # of lines */

#define OtherFontDLOG	290
#define FontSizeTE		4

#define	PrefDLOG	7001			/* The Preferences Dialog		---------------- */
#define PrefDClose				3	/* Don't close the window on "Close" */
#define PrefStag    			4	/* Staggered Windows? */
#define PrefCMDkey				5	/* Command keys */
#define PrefTMap				6	/* Shall we map the tilde? */
#define	PrefKeyFuncMenus		7	/* Show Keypad and Function menus? */
#define PrefBlink   			8	/* Blink the cursor? */
#define PrefBlockCursor   		9	/* Display a block cursor? */
#define PrefUnderscoreCursor	10	/* Display an underscore cursor? */
#define PrefVerticalCursor		11	/* Display a vertical line cursor? */
#define PrefCTt					14	/* Copy table threshold */
#define PrefTimeSlice			20	/* Multifinder Time Slice TxtEdit */
#define PrefCaptCreat			16	/* Capture File Creator Button */
#define	PrefCaptTE				39	/* Capture File Creator TE */
#define PrefAnsiColor			23  // Select ANSI Color Button
#define PrefStaggerOffset		21  //TE box for amount to stagger
#define PrefStaggerBY			25  //static text 'by'
#define PrefNotifyUser			12	//Notify user CheckBox
#define PrefOpenTime			18	/* Open Connection Timeout TE (in seconds) */
#define PrefSendTime			15  /* Send Connection Timeout TE (in seconds) */
#define PrefDestroyTickets		13  //destroy kerberos tickets checkbox
#define FTPDLOG		7002			/* The FTP config Dialog		---------------- */
#define	FTPServerOff			3	/* FTP server off button */
#define FTPServerUnsecure		4	/* FTP server ON Unsecure button */
#define FTPServerPswdPlease		5	/* FTP server Passwords Required */
#define FTPShowFTPlog			6	/* FTP show FTP log on startup */
#define FTPrevDNS				7	/* FTP reverse DNS IP of connection hosts */
#define FTPUseMacBinaryII		8	/* FTP Use Macbinary II */
#define FTPResetMacBinary		9	/* FTP Reset Macbinary after each ftp */
#define FTPISO					10	/* FTP ISO checkbox */
#define FTPbintypeTE			11	/* FTP binary filetype TextEdit */
#define FTPbincreatTE			12	/* FTP binary creator type TextEdit */
#define FTPbinexamplebutton		13	/* FTP binary example file selection button */
#define FTPtextcreatTE			14	/* FTP text creator type TextEdit */ 
#define FTPtextcreatorbutton	15	/* FTP TEXT file creator selection button */

#define TermDLOG	7003			/* The Terminal Config Dialog	---------------- */
#define TermANSIEsc				24	/* ANSI escape sequences checkbox */
#define TermXterm				25	/* Xterm escape sequences checkbox */
#define Termvtwrap				26	/* Use vtwrap mode checkbox*/
#define TermMetaIsCmdCntrol		33	/* Use control-cmd as meta */
#define TermMetaIsOption		34	// use option as meta
#define TermMetaIsCmd			35	// use cmd as meta
#define TermMetaIsOff			49	// no meta
#define Termarrow				27	/* Use emacs arrow keys checkbox */
#define TermMAT					28	/* Map pgup, etc. checkbox */
#define Termeightbit			29	/* Eight bit connection */
#define Termclearsave			30	/* Clear screen saves lines */
#define TermRemapKeypad			31
#define TermType				32	/* Terminal type popup */
#define TermName				18	/* Terminal configuration name TE */
#define TermWidth				14	/* Terminal width TE */
#define TermHeight				16	/* Terminal height TE */
#define TermFontSize			21	/* Font size TE */
#define TermScrollback			23	/* Scrollback TE */
#define TermAnswerback			12	/* Answerback TE */
#define TermNFcolor				3	/* NF color user item */
#define TermNBcolor				4	/* NB color user item */
#define TermBFcolor				5	/* BF color user item */
#define TermBBcolor				6	/* BB color user item */
//#define TermUFcolor				23	/* UF color user item */
//#define TermUBcolor				24	/* UB color user item */
#define	TermFontPopup			20	/* Font menu popup item */
#define	TermNameStatText		17
#define	TermSafeItem			17

#define SessionConfigDLOG	7004	/* The Session Config Dialog	---------------- */
#define	SessTEKinhib			3	/* Inhibit TEK radio button */
#define SessTEK4014				4	/* TEK 4014 radio button */
#define	SessTEK4105				5	/* TEK 4105 radio button */
#define	SessPasteQuick			6
#define	SessPasteBlock			7
#define	SessDeleteDel			8
#define	SessDeleteBS			9
#define	SessForceSave			10
#define	SessBezerkeley			11
#define	SessLinemode			12
#define	SessTEKclear			13
#define	SessHalfDuplex			14
#define	SessAuthenticate		15
#define SessEncrypt				16
#define SessLocalEcho			17
#define	SessAlias				18
#define SessHostName			19
#define	SessPort				20
#define SessBlockSize			21
#define	SessInterrupt			22
#define	SessSuspend				23
#define	SessResume				24
#define	SessTermPopup			25
#define	SessTransTablePopup		26
#define	SessSafeItem			27
#define	SessAliasStatText		28
#define SessBlockSizeTE			39

#define FTPUserDLOG			7005	/* The FTP User Config Dialog   ---------------- */
#define	FTPUcanchangeCWD		3	/* User can change WD checkbox */
#define FTPUusername			4	/* Username TE */
#define FTPUpassword			5	/* User's password */
#define	FTPUDfltDirDsply		6	/* User's default dir path display */
#define	FTPUDfltDirButton		7	/* Button to change user's default dir */

/*================================================================================*/
/* Defines for the Menu related resources */

#define	NMENUS	14			/*	We have NMENUS menus right now */

#define	appleMenu	1		/*	Apple Menu ID */
#define	NfileMenu	512		/*	File Menu Resource ID */
#define	NeditMenu	513		/*	Edit Menu Resource ID */
#define NtermMenu	514		/*	Emulation Menu Resource ID */
#define	NconnMenu	515		/*	Connection Menu Resource ID */
#define NnetMenu	516		/*	Network Menu Resource ID */

#define	fileMenu	256		/*	File Menu ID */
#define	editMenu	257		/*	Edit Menu ID */
#define termMenu	258		/*	Emulation Menu ID */
#define	connMenu	259		/*	Connection Menu ID */
#define netMenu		260		/*	Network Menu Resource ID */
#define fontMenu	128		/*  Font Menu Resource ID */
#define sizeMenu	129		/*	Size Menu Resource ID */
#define opspecMenu	130		/*	Open Special Sub-Menu Resource ID */
#define prefsMenu	131		/*	Preferences Sub-Menu Resource ID */
#define	transMenu	132		/*	Translation Sub-Menu Resource ID */
#define fontMenu2	133		// Bold Font Sub-Menu: RAB BetterTelnet 1.0.1
#define keyMenu		1000	/*	Baylor -- Keypad menu */
#define funcMenu	1002	/*	Baylor -- Function key menu */

#define	Fil			1		/*	File Menu's Position */
#define Edit		2		/*  Edit Menu's Position */
#define	Emul		3		/*	Emulation Menu's position */
#define Net			4		/*	Network Menu's position */
#define Conn		5		/*	Connection Menu's position */
#define Font		6		/*	Font Menu's position */
#define Sizem		7		/*  Size Menu's position */
#define OpSpec		8		/*	Open Special Sub-Menu's position */
#define PrefsSub	9		/*	Preferences Sub-Menu position */
#define	National	10		/*	Translation Sub-Menu's postition */
#define Keypad		11		/*	Baylor -- Keypad menu's position */
#define Function	12		/*	Baylor -- Function menu's position */
#define FontOther	13		// RAB BetterTelnet 1.0.1 - the bold font submenu

/* ----------------- The File Menu ---------------------- */
#define FLopen		1		/* File Menu: Open Connection */
#define FLopenspec	2		/* File Menu: Open Special */
#define	FLclose		3		/* File Menu: Close Connection */
#define	FLload		5		/* File Menu: Load Set */
#define	FLsave		6		/* File Menu: Save Set */
#define FLsavem		7		/* File Menu: Save Set with Macros */
#define FLbin		9		/* File Menu: MacBinary on */
#define FLlog		11		/* File Menu: ftp log on */
#define FLotp		12		// RAB BetterTelnet 1.2 - OTP
#define FLprint		14		/* File Menu: Print Selection */
#define FLprintScrn	15		/* File Menu: Print Selection */
#define FLpset		16		/* File Menu: Page Setup */
#define FLselect	17		// RAB BetterTelnet 1.0fc9 - Save Selection to File
#define FLupload	18		// RAB BetterTelnet 1.0fc9 - Upload File
#define FLquit		20		/* File Menu: Quit */

/* ----------------- The Edit Menu ---------------------- */
#define	EDundo		1
#define EDcut		3
#define EDcopy		4		/* Edit Menu: Copy */
#define EDpaste		5		/* Edit Menu: Paste */
#define EDclear		6		/* Edit Menu: Clear */
#define EDcopyt		7		/* Edit Menu: Copy Table */
#define EDretype	8		// RAB BetterTelnet 1.0fc6: retype selection
#define EDmacros	10		/* Edit Menu: Macros */
#define EDprefs		11		/* Edit Menu: Preferences */

/* ----------------- The Session Menu ---------------------- */
#define EMbs		1		/* Emulation Menu: backspace */
#define EMdel		2		/* Emulation Menu: delete */
#define EMscreensize 4		/* NCSA: SB - new screen dimensions dialog */
#define EMsetup     6		/* BYU 2.4.8 - Emulation Menu: Setup keys */
#define EMfont		7		/* BYU 2.4.8 - Emulation Menu: Font */
#define EMfont2		8		// RAB BetterTelnet 1.0.1 - Bold Font submenu
#define EMsize		9		/* BYU 2.4.8 - Emulation Menu: Size */
#define EMcolor		10		/* BYU 2.4.8 - Emulation Menu: Color */
#define EMAnsiColor 11
#define EMcapture	13		/* BYU 2.4.18 - Emulation Menu: Capture to file */
#define EMqprint	14		// RAB BetterTelnet 1.0fc3 - Emulation Menu: Queue Print Jobs
#define EMff		15		// RAB BetterTelnet 1.0fc8 - Emulation Menu: Ignore FF
#define EMsuspend	16		// RAB BetterTelnet 2.0b1 - Suspend Session
#define EMclrlines	17		// RAB BetterTelnet 2.0b4 - clear saved lines
#define EMecho		19		/* Emulation Menu: echo */
#define EMwrap      20      /* Emulation Menu: Wrap mode on/off */
#define EMcrmap		21		/* RAB BetterTelnet 1.0b2c2 - Emulation Menu: BSD 4.3 CR Mode */
#define EMansi		22		/* RAB BetterTelnet - Emulation Menu: ANSI color */
#define EMxterm		23		/* RAB BetterTelnet - Emulation Menu: Xterm */
#define EMeightbit	24		/* RAB BetterTelnet - Emulation Menu: 8-bit */
#define EMmapkeypad	25		/* RAB BetterTelnet - Emulation Menu: remap keypad */
#define EMarrowmap	26		/* JMB - Emulation Menu: Turn EMACS mapping off/on */
#define EMpgupdwn	27		/* JMB - Emulation Menu: Turn local PgUp/PgDwn/etc. on/off */
#define EMnational	28		/* LU */
#define EMscroll	29		/* Emulation Menu: Clear Screen Saves Lines */
#define EMreset 	30		/* Emulation Menu: Reset Terminal */
#define EMjump		31		/* Emulation Menu: Jump Scroll */
#define EMpage		32		/* Emulation Menu: TEK Page command */
#define EMclear		33		/* BYU 2.4.8 - Emulation Menu: TEK Page clears screen */
#define EMmapd		34		// RAB BetterTelnet 1.0fc4: map Del to ^D
#define EMbold		35		// RAB BetterTelnet 1.0fc6: allow boldfacing
#define EMboldcolor 36		// RAB BetterTelnet 1.0fc6: allow color for boldfacing
#define EMinverse	37		// RAB BetterTelnet 1.0fc9: use inverse for bold
#define EMbeep		38		// RAB BetterTelnet 1.0fc8: ignore beeps
#define EMrealblink	39		// use real blinking

/* ----------------- The Network Menu ---------------------- */
/* #define NEcommand	19	Emulation Menu: Command Keys */
#define	NEftp		1		/* Emulation Menu: Send ftp command */
#define	NEip		2		/* Emulation Menu: Send IP # */
#define NEayt		4		/* Emulation Menu: Send "Are You There" */
#define NEao		5		/* Emulation Menu: Send "Abort Output" */
#define NEinter		6		/* Emulation Menu: Send "Interrupt Process" */
#define NEec		7		/* Emulation Menu: Send "Erase Character" */
#define NEel		8		/* Emulation Menu: Send "Erase Line" */
#define NEsync		9		/* Network Menu: Send SYNC (Urgent) */
#define NEbrk		10		/* Network Menu: Send BRK (Break) */
#define NEipsync	11		// RAB BetterTelnet 1.0fc9 - IP + SYNC (RFC compliant)
#define NEeof		12		// RAB BetterTelnet 2.0b1 - End of File
#define NEsusp		13		// RAB BetterTelnet 2.0b1 - SUSPend Process
#define NEabort		14		// RAB BetterTelnet 2.0b1 - Abort
#define NEscroll	16		/* Network Menu: Scroll Lock */
#define NEnet		18		/* Network Menu: Show Net #'s */

/* ----------------- The Connection Menu ---------------------- */
#define COnext			1	/* Connection Menu: Next Session... */
#define	COtitle			2	/* Change window title */
#define	COshowlog		3	/* Show/Hide Log window */

#define	FIRST_CNXN_ITEM	5	/* The first slot for connections in the menu */

/* ----------------- The Preferences Menu --------------------- */
#define	prfGlobal	1
#define	prfTerm		2
#define	prfSess		3
#define	prfFTP		4
#define prfFTPUser	5

/* Baylor ----------------- The Keypad Menu ---------------------		*/
#define	KPFind			1		/*	Baylor -- Edit Key "Find"			*/
#define	KPInsertHere	2		/*	Baylor -- Edit Key "Insert here"	*/
#define	KPRemove		3		/*	Baylor -- Edit Key "Remove"			*/
#define	KPSelect		4		/*	Baylor -- Edit Key "Select"			*/
#define KPPrevScreen	5		/*	Baylor -- Edit Key "Prev Screen"	*/
#define	KPNextScreen	6		/*	Baylor -- Edit Key "Next Screen"	*/
#define KPUp			7		/*	Baylor -- Edit Key "Up arrow"		*/
#define KPLeft			8		/*	Baylor -- Edit Key "Left arrow"		*/
#define	KPDown			9		/*	Baylor -- Edit Key "Down arrow"		*/
#define KPRight			10		/*	Baylor -- Edit Key "Right arrow"	*/
#define KPPF1			11		/*	Baylor -- Keypad "PF1"				*/
#define KPPF2			12		/*	Baylor -- Keypad "PF2"				*/
#define KPPF3			13		/*	Baylor -- Keypad "PF3"				*/
#define KPPF4			14		/*	Baylor -- Keypad "PF4"				*/
#define KPKP7			15		/*	Baylor -- Keypad "7"				*/
#define KPKP8			16		/*	Baylor -- Keypad "8"				*/
#define KPKP9			17		/*	Baylor -- Keypad "9"				*/
#define KPKPMinus		18		/*	Baylor -- Keypad "Minus"			*/
#define KPKP4			19		/*	Baylor -- Keypad "4"				*/
#define KPKP5			20		/*	Baylor -- Keypad "5"				*/
#define KPKP6			21		/*	Baylor -- Keypad "6"				*/
#define KPKPComma		22		/*	Baylor -- Keypad "Comma"			*/
#define KPKP1			23		/*	Baylor -- Keypad "1"				*/
#define KPKP2			24		/*	Baylor -- Keypad "2"				*/
#define KPKP3			25		/*	Baylor -- Keypad "3"				*/
#define KPKEnter		26		/*	Baylor -- Keypad "Enter"			*/
#define KPKP0			27		/*	Baylor -- Keypad "0"				*/
#define KPKPPeriod		28		/*	Baylor -- Keypad "Period"			*/

/* Baylor ----------------- The Keypad Menu ---------------------		*/
#define	FKF6			1		/*	Baylor -- Function Key "F6"			*/
#define	FKF7			2		/*	Baylor -- Function Key "F7"			*/
#define	FKF8			3		/*	Baylor -- Function Key "F8"			*/
#define	FKF9			4		/*	Baylor -- Function Key "F9"			*/
#define	FKF10			5		/*	Baylor -- Function Key "F10"		*/
#define	FKF11			6		/*	Baylor -- Function Key "F11"		*/
#define	FKF12			7		/*	Baylor -- Function Key "F12"		*/
#define	FKF13			8		/*	Baylor -- Function Key "F13"		*/
#define	FKF14			9		/*	Baylor -- Function Key "F14"		*/
#define	FKHelp			10		/*	Baylor -- Function Key "Help"		*/
#define	FKDo			11		/*	Baylor -- Function Key "Do"			*/
#define	FKF17			12		/*	Baylor -- Function Key "F17"		*/
#define	FKF18			13		/*	Baylor -- Function Key "F18"		*/
#define	FKF19			14		/*	Baylor -- Function Key "F19"		*/
#define	FKF20			15		/*	Baylor -- Function Key "F20"		*/

/*	Resource Defines */

#define	kNCSACreatorSignature 	'Ssh2'		/* Creator signature for "saved set" files */

#define	kNCSASetFileType		'CONF'		/* Filetype for "saved set" files */
#define kNCSAIconFamilyId		128
/* ----------------- Defines for translation routines -------------------------------*/
#define	MY_TRSL				'TRSL'	/* resource type for translation */
#define	TRSL_DEFAULT_TABLE	256		/* The default table */
#define	TRSL_FTP_TABLE		257		/* The MAC->ISO, ISO->MAC table for FTP */

#define	USER_TRSL	'taBL'	/* User translation tables resource type */
							/* 256 bytes of in table followed by 256 bytes of out table */
							
#define GetStrRes(x)	(Str255 **)GetResource('STR ',(x));		/* Get string resource */

/*================================================================================*/
/*	Cursor resource related defines */

#define	rPOSCURS		256
#define rBeamCursor		1
#define rCrossCursor 	2
#define rPlusCursor  	3
#define rWatchCursor  	4
#define	rGINCURS		257
#define	rDbugCURS		268
#define	rXferCURS		290
#define leftcrsr		130		/* cursors to display encryption state */
#define rightcrsr		129		/* cursors to display encryption state */
#define lockcrsr		131		/* cursors to display encryption state */
#define sshicon			135		/* ics# to display ssh2 state */
/*================================================================================*/
/*	WIND resources for remembering the position of our windows */
#define	FTPlogWIND	257


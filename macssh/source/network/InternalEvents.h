/* first we have all of the neat network defines.  Most of them are used by
	the DoNetEvents loop, as the custom network events */

/* the different types of network events... */	
#define USERCLASS	1				/* user defined events */
#define SCLASS		8
#define CONCLASS	16				/* this has all of the connection events */

/* these are all of the connection events */
#define CONOPEN		1		/* connection has opened, CONCLASS */
#define CONDATA 	2       /* there is data available on this connection */
#define CONCLOSE	3		/* the other side has closed its side of the connection */
#define CONFAIL		4		/* connection open attempt has failed */

/* all of the user-defined network events */
#define UDPDATA		1		/* UDP data has arrived on listening port, USERCLASS */
#define DOMAIN_DONE	2		/* domain name ready/failed */
#define DOMAIN_DONE2 3		// for reverse DNS lookups - thanks to Dan Hyde
#define FTPCOPEN	20		/* FTP command connection has opened */
#define FTPCLOSE	21		/* FTP command connection has closed */
#define FTPBEGIN	22
#define FTPEND		23		/* FTP transfer ending */

#define FTPACT		2		/* FTP transfer is active, keep sending */
#define RCPACT		4		/* rcp is active, needs CPU time */
#define RETRYCON	5		/* retry connection packet, might be lost */
#define CLOSEDONE	6		/* Close completion routine has been called (for Mac Drivers only) */
#define UDATA		7		/* BYU mod - client ftp active */

#define	RG_REDRAW	128

/* next two are for keeping track of pasting to session */
#define PASTELEFT	130		/* UserEvent, PASTELEFT causes pasteText */

#define HFTP		21		/* BYU mod: for ftp stuff */

//	Port types (determines what routine should be called when data arrives on port)
#define	NO_TYPE		-1
#define	CNXN_TYPE	0
#define	UDATA_TYPE	1
#define	PDATA_TYPE	2
#define	PFTP_TYPE	3

/*************************************************************************/
/*  event queue
*   records what happens, especially errors, and keeps them for any
*   routines that poll, looking for what happened.
*   Eight event classes are masked in the event class byte.
*	There can be 256 event types per class.
*   The data field is handled differently by each event type.
*/
typedef struct internal_event {
	Ptr				qLink;
	short			qType;
	short			eclass;		/* class, defined in netevent.h */
	short			event;		/* which event */
	short			data1;
	long			data2;		
}	internal_event;

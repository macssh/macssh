/*
	KrbDriver.h -- Application interface for Kerberos Client (mac system extension)
	This interface can be used as is or it can be accessed at a higher level using
	the c++ interface (Kerberos.h).
	
	Set the compiler def KRB_DRIVER only if you are including krb.h in your code
	(this is only necessary for the driver itself)
	
	
	© Copyright 1992 by Cornell University
	
	Initial coding 1/92 by Peter Bosanko.
*/

#ifndef _KrbDriver_	
#define	_KrbDriver_

#ifndef __CONDITIONALMACROS__	
#include <ConditionalMacros.h>
#endif

#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=mac68k
#endif

/* csCodes for Control Calls */
enum {
	cKrbKillIO = 1,

	/* Low level routines, here for compatability with Brown Driver */
	cKrbGetLocalRealm,
	cKrbSetLocalRealm,
	cKrbGetRealm,
	cKrbAddRealmMap,
	cKrbDeleteRealmMap,
	cKrbGetNthRealmMap,
	cKrbGetNthServer,
	cKrbAddServerMap,
	cKrbDeleteServerMap,
	cKrbGetNthServerMap,
	cKrbGetNumSessions,
	cKrbGetNthSession,
	cKrbDeleteSession,
	cKrbGetCredentials,
	cKrbAddCredentials,
	cKrbDeleteCredentials,
	cKrbGetNumCredentials,
	cKrbGetNthCredentials,
			
	/* High Level routines */
	cKrbDeleteAllSessions,
	/* Removes all credentials from storage.                   
	   The user will be asked to enter user name and password the next time a ticket is requested */ 

	cKrbGetTicketForService,
	/* Gets a ticket and returns it to application in buf
		  -> service				Formal Kerberos name of service
		  -> buf					Buffer to receive ticket
		  -> checksum				checksum for this service
		 <-> buflen					length of ticket buffer (must be at least 1258 bytes)
		 <-  sessionKey				for internal use
		 <-  schedule				for internal use								*/

	cKrbGetAuthForService,
	/* Similiar to cKrbGetTicketForService except it builds a kerberos "SendAuth" style
	   request (with SendAuth and application version numbers preceeding the actual ticket)
		  -> service				Formal Kerberos name of service
		  -> buf					Buffer to receive ticket
		  -> checksum				checksum for this service
		  -> applicationVersion		version number of the application (8 byte string)
	 	 <-> buflen					length of ticket buffer (must be at least 1258 bytes)
		 <-  sessionKey				for internal use
		 <-  schedule				for internal use                                */

	// Use the same krbHiParmBlock for the routines below that you used to get the ticket for the service.
	// That way the session key and schedule will get passed back to the driver.
	
	cKrbCheckServiceResponse,
	/* Use the return code from this call to determine if the client is properly authenticated
		  -> buf					points to the begining of the server response
		  -> buflen				length of the server response
		  -> sessionKey			this was returned from cKrbGetTicketForService call	
		  -> schedule				"	
		  -> checksum				left over from cKrbGetTicketForService call
		  -> lAddr				addresses used for service validation...
		  -> lPort				"
		  -> fAddr				"
		  -> fPort				"                                             */

	cKrbEncrypt,
	/* Encrypt stream, High level version of cKrbMakePrivate
		  -> buf					points to the begining of stream buffer
		  -> buflen				length of the stream buffer
		  -> sessionKey			this was returned from cKrbGetTicketForService call	
		  -> schedule				"	
		  -> lAddr				server uses addresses to confirm who we are...
		  -> lPort				"
		  -> fAddr				"
		  -> fPort				"
		  -> encryptBuf			output buffer, allow 26 more bytes than input data			
		  <- encryptLength		actual length of output data                            */				

	cKrbDecrypt,
	/* Decrypt stream, High level version of cKrbReadPrivate
		  -> buf					points to the begining of stream buffer
		  -> buflen				length of the stream buffer
		  -> sessionKey			this was returned from cKrbGetTicketForService call	
		  -> schedule				"	
		  -> lAddr				addresses used to confirm source of message...
		  -> lPort				"
		  -> fAddr				"
		  -> fPort				"
		  <- decryptOffset		offset in buf to beginning of application data			
		  <- decryptLength		actual length of decrypted data	                       */
		  
	cKrbCacheInitialTicket,
	/* Gets a ticket for the ticket granting service and optionally another service that you specify.
	   This call always prompts the user for a password.  The ticket(s) are placed in the ticket cache
	   but are not returned.  Use cKrbGetTicketForService to receive the ticket.
	   NOTE: This call is useful for getting a ticket for the password changing service or any other
	   service that requires that the user be reauthenticated ( that needs an initial ticket ).
	   
		  -> service			Formal Kerberos name of service
		  						( NULL service is OK if you just want a ticket granting ticket ) */
								
	cKrbGetUserName,
	/* Get the kerberos name of the user.  If the user is not logged in, returns error cKrbNotLoggedIn.
	   
		  <- user				Name that user typed in loggin dialog			 */

	cKrbSetUserName,
	/* Set the kerberos name of the user.  If the user is logged in, cKrbSetUserName logged the user out.
	   
		  -> user				Name that will be used as default in loggin dialog */

	cKrbSetPassword,
	/* Sets the password which will be used the next time a password is needed
		This can be used to bypass the loggin dialog.
		NOTE: Password is cleared from memory after it is used once or
		whenever a cKrbSetUserName or cKrbDeleteAllSessions call is made.
		  -> user					contains password (of current user)			*/

	cKrbGetDesPointers
	/* Returns a block of pointers to DES routines so the routines can be
	    called directly. */
	};

/* Password changing service */

#define KRB_PASSWORD_SERVICE  "changepw.kerberos"

// Error codes
enum {
	cKrbCorruptedFile = -1024,	/* couldn't find a needed resource */
	cKrbNoKillIO,				/* can't killIO because all calls sync */
	cKrbBadSelector,			/* csCode passed doesn't select a recognized function */
	cKrbCantClose,				/* we must always remain open */
	cKrbMapDoesntExist,			/* tried to access a map that doesn't exist (index too large,
									or criteria doesn't match anything) */
	cKrbSessDoesntExist,		/* tried to access a session that doesn't exist */
	cKrbCredsDontExist,			/* tried to access credentials that don't exist */
	cKrbTCPunavailable,			/* couldn't open MacTCP driver */
	cKrbUserCancelled,			/* user cancelled a log in operation */
	cKrbConfigurationErr,		/* Kerberos Preference file is not configured properly */
	cKrbServerRejected,			/* A server rejected our ticket */
	cKrbServerImposter,			/* Server appears to be a phoney */
	cKrbServerRespIncomplete,	/* Server response is not complete */
	cKrbNotLoggedIn,			/* Returned by cKrbGetUserName if user is not logged in */
	cKrbOldDriver,				/* old version of the driver */
	
	cKrbKerberosErrBlock = -20000	/* start of block of 256 kerberos errors */
	};

// Parameter block for high level calls
struct krbHiParmBlock	{
			char 			*service;		/* full name -- combined service, instance, realm */
			char  			*buf;
			unsigned long  	buflen;
			long 			checksum;
			unsigned long	lAddr;
			unsigned short	lPort;
			unsigned long	fAddr;
			unsigned short	fPort;
			unsigned long	decryptOffset;
			unsigned long	decryptLength;
			char 			*encryptBuf;
			unsigned long	encryptLength;
			char	 		*applicationVersion;	/* Version string must be 8 bytes long!	 */
			char	 		sessionKey[8];			/* for internal use                      */
			char			schedule[128];			/* for internal use                      */
			char 			*user;
};
typedef struct krbHiParmBlock krbHiParmBlock;
typedef krbHiParmBlock *KrbParmPtr;
typedef KrbParmPtr *KrbParmHandle;

/* ********************************************************* */
/* The rest of these defs are for low level calls
/* ********************************************************* */
#ifndef KRB_DRIVER
/* First some kerberos defs */

#ifdef notdef
typedef unsigned char des_cblock[8];	/* crypto-block size */

/* Key schedule */
typedef struct des_ks_struct { des_cblock _; } des_key_schedule[16];

#define C_Block des_cblock
#define Key_schedule des_key_schedule
#endif

/* The maximum sizes for aname, realm, sname, and instance +1 */
#define 	ANAME_SZ	40
#define		REALM_SZ	40
#define		SNAME_SZ	40
#define		INST_SZ		40

/* Definition of text structure used to pass text around */
#define		MAX_KTXT_LEN	1250

struct ktext {
    long     length;		/* Length of the text */
    unsigned char dat[MAX_KTXT_LEN];	/* The data itself */
    unsigned long mbz;		/* zero to catch runaway strings */
};

typedef struct ktext *KTEXT;
typedef struct ktext KTEXT_ST;


/* Structure definition for rd_private_msg and rd_safe_msg */

struct msg_dat {
    unsigned char *app_data;	/* pointer to appl data */
    unsigned long app_length;	/* length of appl data */
    unsigned long hash;		/* hash to lookup replay */
    long     swap;		/* swap bytes? */
    long    time_sec;		/* msg timestamp seconds */
    unsigned char time_5ms;	/* msg timestamp 5ms units */
};

typedef struct msg_dat MSG_DAT;

typedef unsigned long u_long;
typedef unsigned short u_short;

#endif


/* Parameter block for low level calls */		
struct krbParmBlock	{
			char	*uName;
			char	*uInstance;
			char	*uRealm;			/* also where local realm or mapping realm passed */
			char	*sName;
			char	*sInstance;
			char	*sRealm;
			char	*host;				/* also netorhost */
			long	admin;				/* isadmin, mustadmin */
			long	*itemNumber;
			long	*adminReturn;		/* when it needs to be passed back */
			void *cred;
};
typedef struct krbParmBlock krbParmBlock;

#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=reset
#endif

#endif


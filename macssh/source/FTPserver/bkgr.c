// Background procedures for rcp and ftp

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
* NCSA Revisions:
* 11/86          Tim Krauskopf
* 7/92	Telnet 2.6: added global structures, cleaned up defines   Scott Bulmahn
*
*
*/


#include "InternalEvents.h"
#include "netevent.proto.h"
#include "menuseg.proto.h"
#include "network.proto.h"
#include "mydnr.proto.h"
#include "macutil.proto.h"
#include "macbinary.h"
#include "binsubs.proto.h"
#include "maclook.proto.h"
#include "bkgr.proto.h"
#include "telneterrors.h"
#include "wind.h"
#include "mainseg.proto.h"
#include "translate.proto.h"
#include "DlogUtils.proto.h"		// For WriteZero proto

#include "vsdata.h"
#include "vsinterf.proto.h"

extern WindRec *ftplog;
extern TelInfoRec *TelInfo;

#define PFTP 1
#define PRCP 2
#define PDATA 3
#define HTELNET 23
#define HRSHD 514
#define HFTP 21
#define BUFFERS 8000
#define PATHLEN 256
#define MACBINARY
#define RCPSEGSIZE 512
#define O_RAW O_RDONLY
#define EOLCHAR 13
#define FASCII 0
#define FIMAGE O_RAW
#define FAMODE 0
#define FIMODE 1
#define FMMODE 2			/* Mac Binary, when ready */

MBFile
	*mbfp=NULL;

char *nextfile;

static Handle mungbuf = 0;

static short fcnt=0;
static  short
	ftpenable=0,				/* is file transfer enabled? */
	rcpenable=0,				/* is rcp enabled? */
	rsnum=-1,					/* port number for incoming rshell */
	rserr=-1;					/* port number for rshd() stderr */

static  unsigned char *xs=0L,	/* buffer space for file transfer */
		*pathname=0L,			/* space to keep path names */
		*newfile=0L,			/* current file being received */
		myuser[17],					/* user name on my machine */
		waitchar;					/* character waiting for from net */

static short 
	curstate = -1,			/* state machine for background processes */
	retstate = 200,			/* to emulate a subroutine call */
	ftpstate = 0,			/* state of the ftp data transfer */
	isdir=0,				/* flag for rcp target pathname */
	waitpos=0,				/* marker for gathering strings from net */
	cnt=0,					/* number of characters from last netread() */
	fh=0,					/* file handle when transfer file is open */
	ftpfh=0,				/* file handle for ftp data */
	rc=0,					/* telnet flag */
	xp=0,					/* general pointer */
	towrite=0,				/* file transfer pointer */
	len=0;					/* file transfer length */

static long
	filelen=0L;				/* length of current file for transfer */

short mainport[NPORTS];		/* BYU 2.4.16 - map FTP data port to main port */
short dataport[NPORTS];		/* BYU 2.4.16 - map FTP main port to data port */
short mainstate[NPORTS];	/* BYU 2.4.16 */
short datastate[NPORTS];	/* BYU 2.4.16 */
short retnstate[NPORTS];

ip_port	fileport[NPORTS];

static char crfound=0;

static short rfstate,
	ftptmode=FAMODE;			/* how to transfer the file on net */

static	ip_addr	portaddr[NPORTS],
				hishostaddr;
static	ip_port	portremoteport[NPORTS],
				portlocalport[NPORTS];
				
unsigned char messs[100];		/* BYU */

char	ListingTemplate[256];
CInfoPBRec	theDirectoryState;

static	void CRESP(short fnum, short msg_number);
static	short ftpgo( short fnum);

void	FTPServerUnload(void) {}

void CRESP(short fnum, short msg_number)
{
  GetIndString(messs,MSG_RESOURCE_ID,msg_number+1);
  netpush(fnum);
  Rnetwrite(fnum, &messs[1], (short)messs[0]);
}

void ftpmess( char *cp)
{
	short temp;
	temp=strlen(cp);
	if (temp>80 || temp < 1) return;
	VSwrite(ftplog->vs,cp,temp);
}

/***********************************************************************/
void StartUpFTP(void)
{
	if (gFTPServerPrefs->ServerState != 0)
		setftp();
	else
		unsetftp();
}

void setftp( void)
{
	short fnum,i1;							/* BYU 2.4.16 */
	short size = 1024;

	Mnetinit();								// RAB BetterTelnet 1.0fc4

/* set up to receive a telnet connection for ftp commands */
	rfstate = 0;
	ftpstate = 0;
/* Listen for FTP if we're not listening already */

	for (i1 = 0; i1 < NPORTS; i1++) 										/* BYU 2.4.16 */
		if (GetPortType(i1) == PFTP)
			break;															/* BYU 2.4.16 */

	if (i1 >= NPORTS) {								/* BYU 2.4.16 - non-SLIP listen */
		fnum = netlisten(HFTP);		/* BYU 2.4.16 - MacTCP listen */
		ftpenable = 1;								/* BYU 2.4.16 */

		if (fnum >= 0) {							/* BYU 2.4.16 - signal that events should be caught */
			SetPortType(fnum, PFTP);
			dataport[fnum] = -1;					/* BYU 2.4.16 - no data needed yet */
			mainstate[fnum] = 0;					/* BYU 2.4.16 */
			datastate[fnum] = 0;					/* BYU 2.4.16*/
			retnstate[fnum] = 200;					/* BYU 2.4.16 */
		}											/* BYU 2.4.16 */
	}												/* BYU 2.4.16 */

	strcpy((char *) myuser,"unknown");				/* BYU LSC - set unknown user name */
	
	if (mungbuf == NULL)
		mungbuf = myNewHandle(size);
	}

void unsetftp( void)
{
	short i1;							/* BYU 2.4.16 */

	rfstate = 0;
	ftpstate = 0;
	for (i1 = 0; i1 < NPORTS; i1++) {	/* BYU 2.4.16 */
		if (GetPortType(i1) == PFTP) {		/* BYU 2.4.16 */
			netclose(i1);				/* BYU 2.4.16 */
			SetPortType(i1, NO_TYPE);			/* BYU 2.4.16 */
			mainstate[i1] = 0;			/* BYU 2.4.16 */
			datastate[i1] = 0;			/* BYU 2.4.16*/
		}								/* BYU 2.4.16 */
	}									/* BYU 2.4.16 */
	ftpenable = 0;
	if (mungbuf != NULL)
		DisposeHandle(mungbuf);
}


#if 0		/* BYU - Converted to STR# resource number 23227 */
/***********************************************************************/
/*
*  resident ftp server -- enables initiation of ftp without a username
*  and password, as long as this telnet is active at the same time
*  Now checks for the need of passwords.
*/

static char *messs[] = {
						"220 Macintosh Resident FTP server, ready \015\012",
						"451 Error in processing list command \015\012",
						"221 Goodbye \015\012",						/*2*/
						"200 This space intentionally left blank <   > \015\012",
						"150 Opening connection \015\012",
						"226 Transfer complete \015\012",			/*5*/
						"200 Type set to A, ASCII transfer mode \015\012",
						"200 Type set to I, binary transfer mode \015\012",
						"500 Command not understood \015\012",		/*8*/
						"200 Okay \015\012",
						"230 User logged in \015\012",
						"550 File not found \015\012",				/*11*/
						"501 Directory not present or syntax error\015\012",
						"250 Chdir okay\015\012",
						"257 \"",
						"\" is the current directory \015\012",		/*15*/
						"501 File not found \015\012",
						"504 Parameter not accepted, not implemented\015\012",
						"200 Stru F, file structure\015\012",
						"200 Mode S, stream mode\015\012",		/*19*/
						"202 Allocate and Account not required for this server\015\012",
						"501 Cannot open file to write, check for valid name\015\012",
						"530 USER and PASS required to activate me\015\012",
						"331 Password required\015\012",      /*23 */
						"530 Login failed\015\012",
						"200 MacBinary Mode enabled\015\012",
						"200 MacBinary Mode disabled\015\012",  /*26 */
						"552 Disk write error, probably disk full\015\012",
						"214-NCSA Telnet FTP server, supported commands:\015\012",
						"    USER  PORT  RETR  ALLO  PASS  STOR  CWD  XCWD  XPWD  LIST  NLST\015\012",
						"    HELP  QUIT  MODE  TYPE  STRU  ACCT  NOOP  MACB\015\012",  /*30*/
						"    MACB is MacBinary and must be done with TYPE I\015\012",
						"214 Direct comments and bugs to telbug@ncsa.uiuc.edu\015\012",
						"200 Type set to I, binary transfer mode [MACBINARY ENABLED]\015\012",				/* 33 */
						"200 Type set to I, binary transfer mode [macbinary disabled]\015\012",
				""};

#endif

/* open the FTP data connection to the remote host */
short ftpgo( short fnum)
{
	short	ftpdata;

	netfromport(20);				 /* ftp data port */
	ftpdata = netxopen(portaddr[fnum],fileport[fnum], 15);
	mainport[ftpdata] = fnum;
	dataport[fnum] = ftpdata;
	return(ftpdata);
}

void rftpd
  (
	short code,								/* BYU 2.4.16 */
	short fnum								/* BYU 2.4.16 */
  )
{
	ip_port	fdport;
	short	ftpdata, i, append = 0;
	char	tempp[256], theDir[256];
	char updir[4];						/* JMB 2.6 */
	
	if (!ftpenable)
		return;

	netpush(fnum);

	rfstate = mainstate[fnum];				/* BYU 2.4.16 */
	ftpstate = datastate[fnum];				/* BYU 2.4.16 */
	retstate = retnstate[fnum];				/* BYU 2.4.16 */
	fdport = fileport[fnum];				/* BYU 2.4.16 */

	switch (rfstate) {
		case 0:
			if (code != CONOPEN) 
				break;
			ftptmode = FAMODE;
			netputevent(USERCLASS,FTPCOPEN,-1,0);
			if (!xs) xs=(unsigned char *)myNewPtr(BUFFERS+10);	/* BYU LSC */
			if (!pathname) pathname=(unsigned char *)myNewPtr(PATHLEN);
			if (!newfile) newfile=(unsigned char *)myNewPtr(PATHLEN);
			if (!xs || !pathname || !newfile)	/* no memory for ftp names */
				{
				DoError(114 | MEMORY_ERRORCLASS, LEVEL3, NULL);
				quit();					// Should we really quit?
				}	
		
			rfstate = 1;				/* drop through */
		case 1:
			CRESP(fnum,0);							/* BYU 2.4.16 */
			netgetftp(fnum, &portaddr[fnum], &portlocalport[fnum], &portremoteport[fnum]);		/* BYU 2.4.16 - get default ftp information */
			hishostaddr = portaddr[fnum];
			fdport = portremoteport[fnum];

			waitpos = 0; waitchar = '\012';
			rfstate = 50;       		/* note skips over */
			if ((gFTPServerPrefs->ServerState == 2)) 
				retstate = 3;				/* check pass */
			else
				retstate = 5;				/* who needs one ? */
			break;
		case 3:				/* check for passwords */
		case 4:
			waitpos = 0;  waitchar = '\012';
			rfstate = 50;  
			if (!strncmp("USER",(char *) xs,4)) {	/* BYU LSC */
				if (strlen((char *) xs) < 6)		/* BYU LSC - make sure blank name stays blank */
					xs[5] = myuser[0] = 0;
				strncpy((char *) myuser,(char *) &xs[5],16);		/* BYU LSC - keep user name */
				FTPLogUserLoggingIn();
				CRESP(fnum,23);										/* BYU 2.4.16 */
				retstate = 4;		/* wait for password */
				break;
			}
			if (!strncmp("PASS",(char *) xs,4)) {					/* BYU LSC */
				if (Scheckpass((char *) myuser,(char *) &xs[5])) {	/* BYU LSC */
					ftpmess("FTP password OK - logged in\015\012");
					CRESP(fnum,10);									/* BYU 2.4.16 */
					retstate = 5;
				}
				else {
					ftpmess("FTP password failed - access denied\015\012");
					CRESP(fnum,24);									/* BYU 2.4.16 */
					retstate = 3;
				}
				break;
			}
			if (!strncmp("QUIT",(char *) xs,4)) {	/* BYU LSC */
				CRESP(fnum,2);						/* BYU 2.4.16 */
				cnt = -1;
			}
			else {
				CRESP(fnum,22);		/* BYU 2.4.16 */
			}
			retstate = 3;			/* must have password first */
			break;				
				
/* interpret commands that are received from the other side */
			
		case 5:

/* set to a safe state to handle recursion, wait for another command line from client */
			rfstate = 50;
			retstate = 5;
			waitchar = '\012';
			waitpos = 0;

			if (!strncmp((char *) xs,"LIST",4) || !strncmp((char *) xs,"NLST",4)) {	/* BYU LSC */
				if ((strlen((char *) xs) < 6) || xs[5] == '.')		/* BYU LSC */
					strcpy((char *) xs,"LIST *");					/* BYU LSC */
				strcpy(ListingTemplate, (char *) &xs[5]);

				theDirectoryState.hFileInfo.ioFDirIndex=1;
				theDirectoryState.hFileInfo.ioVRefNum = TelInfo->DefaultDirVRefNum;
				theDirectoryState.hFileInfo.ioDirID = TelInfo->DefaultDirDirID;

				nextfile = getFileName(ListingTemplate, &theDirectoryState);	/* find first name */
				if (nextfile == NULL) {
					CRESP(fnum,16);				/* BYU 2.4.16 */
				}
				else {
					Str255	tempstr;
					char	tempstr2[256];
					ftpdata = ftpgo(fnum);				/* BYU 2.4.16 - open the connection */
					fdport = portremoteport[fnum]; /* BYU 2.4.16 - reset to def */

					if (ftpdata >= 0)
						SetPortType(ftpdata, PDATA);
					ftpstate = 40;		/* ready to transmit */
					CRESP(fnum,4);							/* BYU 2.4.16 */
					PathNameFromDirID(TelInfo->DefaultDirDirID, TelInfo->DefaultDirVRefNum, tempstr);
					PtoCstr(tempstr);
					sprintf(tempstr2, "FTP Sending Directory %s\015\012", (char *)tempstr);
					ftpmess(tempstr2);
				}
			}
			else if (!strncmp((char *) xs,"CWD",3)) {	/* BYU LSC */
				if (ChangeDirectory(&(TelInfo->DefaultDirDirID), &(TelInfo->DefaultDirVRefNum), (char *) &xs[4])) {
					CRESP(fnum,12);						/* BYU 2.4.16 */
				}
				else {						/* success */
					CRESP(fnum,13);			/* BYU 2.4.16 */
				}
			}
// KFG BetterTelnet 2.0fc1 - integrated KFG's patch for mkdir/rmdir
			else if (!strncmp((char *) xs,"MKD",3)) {	/* KFG */
				if (MakeDirectory(&(TelInfo->DefaultDirDirID), &(TelInfo->DefaultDirVRefNum), (char *) &xs[4])) {
					CRESP(fnum,37);
				}
				else {						/* success */
					CRESP(fnum,36);
				}
			}
			else if (!strncmp((char *) xs,"RMD",3)) {	/* KFG */
				if (RemoveDirectory(&(TelInfo->DefaultDirDirID), &(TelInfo->DefaultDirVRefNum), (char *) &xs[4])) {
					CRESP(fnum,39);
				}
				else {						/* success */
					CRESP(fnum,38);
				}
			}
			else if (!strncmp((char *) xs,"XMKD",4)) {	/* KFG */
				if (MakeDirectory(&(TelInfo->DefaultDirDirID), &(TelInfo->DefaultDirVRefNum), (char *) &xs[5])) {
					CRESP(fnum,37);
				}
				else {						/* success */
					CRESP(fnum,36);
				}
			}
			else if (!strncmp((char *) xs,"XRMD",4)) {	/* KFG */
				if (RemoveDirectory(&(TelInfo->DefaultDirDirID), &(TelInfo->DefaultDirVRefNum), (char *) &xs[5])) {
					CRESP(fnum,39);
				}
				else {						/* success */
					CRESP(fnum,38);
				}
			}
			else if (!strncmp((char *) xs,"CDUP",4)) {		/* JMB 2.6 */
				strcpy(updir, "..");						/* JMB 2.6 */
				if (ChangeDirectory(&(TelInfo->DefaultDirDirID), &(TelInfo->DefaultDirVRefNum), updir)) {				/* JMB 2.6 */
					CRESP(fnum,12);							/* JMB 2.6 */
				}											/* JMB 2.6 */
				else {										/* JMB 2.6 */
					CRESP(fnum,13);							/* JMB 2.6 */
				}											/* JMB 2.6 */
			}												/* JMB 2.6 */
			else if (!strncmp((char *) xs,"STOR",4) || !strncmp((char *) xs,"APPE", 4))
			  {
				short	MBflag;

				if (!strncmp((char *) xs,"APPE", 4)) {
					if (ftptmode != FASCII) {
						CRESP(fnum, 35);
						break;
						}
					else
						append = 1;
					}
					
				/* had to simplify this, else MPW C 3.0/3.1 generates bad code! */
				if ((!TelInfo->MacBinary) || (ftptmode == FAMODE))
					MBflag = MB_DISABLE;
				else
					MBflag = 0;
					
				if (ftptmode == FAMODE)	MBflag |= MB_ISASCII;
				if (append) MBflag |= MB_APPEND;
				append = 0;
				
				mbfp = MBopen((char *)&xs[5], TelInfo->DefaultDirVRefNum, TelInfo->DefaultDirDirID, (short)(MB_WRITE | MBflag));
				if (mbfp == 0L)
				  {
					CRESP(fnum,21);			/* BYU 2.4.16 */
					break;
				  }
				else
					ftpfh = 12;

				ftpstate = 0;

				strncpy((char *) newfile,(char *) &xs[5],PATHLEN-1);	/* BYU LSC */
				

				ftpdata = ftpgo(fnum);				/* BYU 2.4.16 - open connection */
				fdport = portremoteport[fnum]; /* BYU 2.4.16 - reset to def */

				if (ftpdata >= 0)
					SetPortType(ftpdata, PDATA);

				CRESP(fnum,4);		/* BYU 2.4.16 */

				ftpstate = 30;		/* ready for data */
			  }

			else if (!strncmp((char *) xs,"RETR",4))	/* BYU LSC */
			  {
			  /* had to simplify this, else MPW C 3.0/3.1 generates bad code! */
				short
					MBflag;
				if ((!TelInfo->MacBinary) || (ftptmode == FAMODE))
					MBflag = MB_DISABLE;
				else
					MBflag = 0;
					
				if (ftptmode == FAMODE)	MBflag |= MB_ISASCII;
				
				mbfp = MBopen((char *) &xs[5], TelInfo->DefaultDirVRefNum, TelInfo->DefaultDirDirID, MB_READ | MBflag);
				
				if (mbfp == 0L) {
					CRESP(fnum,11);						/* BYU 2.4.16 */
					break;
				}
				ftpfh = 12;

				strncpy((char *) newfile,(char *) &xs[5],PATHLEN-1);	/* BYU LSC */

				PathNameFromDirID(TelInfo->DefaultDirDirID, TelInfo->DefaultDirVRefNum, (StringPtr) theDir);
				PtoCstr((StringPtr) theDir);
				sprintf(tempp,"remote <-- %s/%s\015\012",theDir,newfile);
				ftpmess(tempp);

				ftpdata = ftpgo(fnum);				/* BYU 2.4.16 - open connection */
				fdport = portremoteport[fnum];

				ftpstate = 20;		/* ready for data */
				if (ftpdata >= 0)
					SetPortType(ftpdata, PDATA);

				CRESP(fnum,4);							/* BYU 2.4.16 */
			  }
			else if (!strncmp((char *) xs,"TYPE",4)) {	/* BYU LSC */
				if (toupper(xs[5]) == 'I') {
					ftptmode = FIMODE;
					if (TelInfo->MacBinary) {
						CRESP(fnum,33);					/* BYU 2.4.16 - Binary on, MACB ON */
						}
					else {
						CRESP(fnum,34);					/* BYU 2.4.16 - Binary on, MACB off */
						}
				}
				else if (toupper(xs[5]) == 'A') {
					ftptmode = FAMODE;
					CRESP(fnum,6);					/* BYU 2.4.16 */
					}
				else {
					CRESP(fnum,17);					/* BYU 2.4.16 */
				}

			}
			else if (!strncmp((char *) xs,"MACB",4)) {	/* BYU LSC */
				if (toupper(xs[5]) == 'E') {
					TelInfo->MacBinary = 1;
					CRESP(fnum,25);					/* BYU 2.4.16 */
				}
				else {
					TelInfo->MacBinary = 0;
					CRESP(fnum,26);					/* BYU 2.4.16 */
				}
				DisplayMacBinary();			/* post an event ? */
			}
			else if (!strncmp((char *) xs,"PORT",4)) {	
/*	get the requested port number from the command given */
				ip_addrbytes	tempaddr;
				unsigned char 	*stringPtr, *tempPtr, storage[10];
				long			numbers[6];
				short index;

				for (tempPtr = stringPtr = &xs[5], index = 0; (*stringPtr != '\0') && (index < 6);index++) {
    				while ((*tempPtr != '\0') && (*tempPtr != ',')) { tempPtr++; };
     				BlockMoveData(stringPtr, &(storage[1]), tempPtr-stringPtr);
     				storage[0] = tempPtr-stringPtr;
     				StringToNum(storage, &numbers[index]);
     				tempPtr++;
     				stringPtr = tempPtr;
     			}
				
				tempaddr.a.byte[0] = (char) numbers[0];
				tempaddr.a.byte[1] = (char) numbers[1];
				tempaddr.a.byte[2] = (char) numbers[2];
				tempaddr.a.byte[3] = (char) numbers[3];
				portaddr[fnum] = tempaddr.a.addr;
				portlocalport[fnum] = numbers[4]*256 + numbers[5];
				fdport = portlocalport[fnum];
				CRESP(fnum,3);							
			}
			else if (!strncmp((char *) xs,"QUIT",4)) {	/* BYU LSC */
				CRESP(fnum,2);							/* BYU 2.4.16 */
				rfstate = 60;
				netputuev(CONCLASS,CONDATA,fnum,0);	/* post back to me */
			}
			else if (!strncmp((char *) xs,"XPWD",4) || !strncmp((char *) xs,"PWD",3)) {	/* BYU LSC */
				CRESP(fnum,14);						/* BYU 2.4.16 - start reply */
				PathNameFromDirID(TelInfo->DefaultDirDirID, TelInfo->DefaultDirVRefNum, (StringPtr) xs);
				PtoCstr((StringPtr) xs);
				Rnetwrite(fnum,(char *) xs,strlen((char *) xs));	/* BYU LSC - write dir name */
				CRESP(fnum,15);						/* BYU 2.4.16 - finish reply */
			}
			else if (!strncmp((char *) xs,"USER",4)) {	/* BYU LSC */
				if (strlen((char *) xs) < 6)			/* BYU LSC - make sure blank name stays blank */
					xs[5] = myuser[0] = 0;
				strncpy((char *) myuser,(char *) &xs[5],16);	/* BYU LSC - keep user name */
				FTPLogUserLoggingIn();
				/* confirm log in without password */
				CRESP(fnum,10);							/* BYU 2.4.16 */
			}
			else if (!strncmp((char *) xs,"STRU",4)) {	/* BYU LSC - only one stru allowed */
				if (xs[5] == 'F') {
					CRESP(fnum,18); }					/* BYU 2.4.16 */
				else {
					CRESP(fnum,17); }					/* BYU 2.4.16 */
			}
			else if (!strncmp((char *) xs,"MODE",4)) {	/* BYU LSC - only one mode allowed */
				if (xs[5] == 'S') {
					CRESP(fnum,19); }					/* BYU 2.4.16 */
				else {
					CRESP(fnum,17); }					/* BYU 2.4.16 */
			}
			else if (!strncmp((char *) xs,"ALLO",4) || !strncmp((char *) xs,"ACCT",4)) {	/* BYU LSC */
				CRESP(fnum,20); }						/* BYU 2.4.16 */
			else if (!strncmp((char *) xs,"HELP",4)) {	/* BYU LSC */
				for (i=28; i<33; i++) {
					CRESP(fnum,i); }					/* BYU 2.4.16 */
			}
			else if (!strncmp((char *) xs,"NOOP",4)) {		/* BYU LSC */
				CRESP(fnum,9); }							/* BYU 2.4.16 */
			else if (!strncmp((char *) xs,"SYST",4)) {	// JMB 2.6
				  netpush(fnum);						// JMB 2.6
				  Rnetwrite(fnum, "225 MACOS System Version:\015\012", 27);	// JMB 2.6
					}	// JMB 2.6
			else {			/* command not understood */
				CRESP(fnum,8); 								/* BYU 2.4.16 */
			}

			break;

/*	subroutine to wait for a particular character */
		case 50:
			while (0 < (cnt = netread(fnum,&xs[waitpos],1))) {
				if (xs[waitpos] == waitchar) {
					rfstate = retstate;

					while (xs[waitpos] < 33)		/* find end of string */
						waitpos--;
					xs[++waitpos] = '\0';			/* put in terminator */

					for (i=0; i<4; i++)				/* want upper case */
						xs[i] = toupper(xs[i]);

					break;
				}
				else
					waitpos += cnt;

			}
			break;

		case 60:					/* wait for message to get through */
									/* or connection is broken */
/*			printf("                  %d,%d",netpush(fnum),netest(fnum));*/
			if (!netpush(fnum) || netest(fnum))
				cnt = -1;
			else
				netputuev(CONCLASS,CONDATA,fnum,0);	/* post back to me */
			break;

		default:
			break;

	}

	if (cnt < 0) {
		if (mbfp) {
			MBclose( mbfp );
			mbfp = NULL;
		}
		ftpdata = dataport[fnum];					/* BYU 2.4.16 */
		if (ftpdata > 0) {
			netclose(ftpdata);
			netputevent(USERCLASS,FTPEND,-1,0);
			dataport[fnum] = -1;					/* BYU 2.4.16 */
			mainport[ftpdata] = -1;					/* BYU 2.4.16 */
		}
		rfstate = 100;
		ftpstate = 0;
		cnt = 0;
		netclose(fnum);
		SetPortType(fnum, NO_TYPE);
		netputevent(USERCLASS,FTPCLOSE,-1,0);
#if 0												/* BYU 2.4.16 */
		fnum = -1;
		ftpdata = -1;
#endif												/* BYU 2.4.16 */
		setftp();				/* reset it */
	}

	mainstate[fnum] = rfstate;						/* BYU 2.4.16 */
	datastate[fnum] = ftpstate;						/* BYU 2.4.16 */
	retnstate[fnum] = retstate;						/* BYU 2.4.16 */
	fileport[fnum] = fdport;						/* BYU 2.4.16 */

}

/*	important note:  for Sfread, nwant must be 256 bytes LARGER than the amount
	which will probably be read from the connection.
	Sfread will stop anywhere from 0 to 256 bytes short of filling nwant
	number of bytes. */
	
long Sfread( short pnum, char *buf, long nwant)
{
	long	i, ndone, lim;
	char	*p, *q;

	if (nwant < 1024)
		return(-1);

	ndone = 0;

	HLock(mungbuf);
	while (ndone < nwant - 1024) {

		if (0 >= (lim = netread(pnum,*mungbuf,1024))) {
			if (ndone || !lim)			/* if this read is valid, but no data */
				return(ndone);
			else
				return(-1);				/* if connection is closed for good */
		}

		p = *mungbuf;
		q = (char *) buf + ndone;

/*		printf("\012 lim=%d done=%d want=%d",lim,ndone,nwant);
		n_row();
*/
		for (i=0; i < lim; i++) {

			if (crfound) {
				if (*p == 10)
					*q++ = EOLCHAR;
				else if (*p == 0)
					*q++ = 13;			/* CR-NUL means CR */
				crfound = 0;
			}
			else if (*p == 13)
				crfound = 1;
			else
				if (gFTPServerPrefs->DoISOtranslation)
				{
					*q++ = (char) ftp_iso_mac((unsigned char *)p);		/* transform to mac */
				}
				else
					*q++ = *p;				/* copy the char */

			p++;
		}

		ndone = q-buf;					/* count chars ready */
	}
	
	HUnlock(mungbuf);
	
	return(ndone);
}

/***************************************************************************/
/*  Sfwrite
*   Write an EOL translated buffer into Rnetwrite.
*   Returns the number of bytes which were processed from the incoming
*   buffer.  Uses its own 1024 byte buffer for the translation (with Sfread).
*/

long Sfwrite( short pnum, void *buf, long nsrc)
{
	long	i,ndone,nout,lim;
	char	*p,*q;

	ndone = 0;
	HLock (mungbuf);
	while (ndone < nsrc) {

		if (0 > ( i = netroom(pnum)))
			return(-1);

		if (i < 1024)					/* not enough room to work with */
			return(ndone);

/*	process up to 512 source bytes for output (could produce 1K bytes out) */
		if (nsrc - ndone > 512)
			lim = 512;
		else
			lim = nsrc-ndone;

		p = (char *) buf + ndone;				/* where to start this block */
		q = *mungbuf;					/* where munged stuff goes */
		for (i=0; i < lim; i++) {
			if (*p == EOLCHAR) {
				*q++ = 13;
				*q++ = 10;
				p++;
			}
			else
				if (gFTPServerPrefs->DoISOtranslation)
				{
					*q++ = (char) ftp_mac_iso((unsigned char *)p);		/* transform to ISO */
					p++;
				}
				else
					*q++ = *p++;
		}
		ndone += lim;					/* # of chars processed */
		nout = q-*mungbuf;				/* # of chars new */

		
		if ( nout != Rnetwrite(pnum,*mungbuf,nout) )		/* Some error in Sfwrite */
			DoError (201 | RESOURCE_ERRORCLASS, LEVEL1, NULL);
		
	}
	HUnlock (mungbuf);
	return(ndone);
}


/*	FTP receive and send file functions */

void ftpd
  (
	short code,
	short ftpdata							/* BYU 2.4.16 */
  )
{
	short fnum = mainport[ftpdata];		/* BYU 2.4.16 */
	short i,a;
	char tempp[256];
	char theDir[256];


	ftpstate = datastate[fnum];			/* BYU 2.4.16 */

	switch (ftpstate) {
		default:
			break;

		case 40:				/* list file names in current dir */

			if (code == CONFAIL)	/* something went wrong */
				fcnt = -1;
			if (code != CONOPEN) 	/* waiting for connection to open */
				break;
			
			ftpstate = 41;

/*
*  send the "nextfile" string and then see if there is another file
*  name to send
*/
		case 41:
			netputuev(SCLASS,FTPACT,ftpdata,0);
			netpush(ftpdata);
			strcpy(tempp,nextfile);					/* BYU 2.4.13 */
			strcat(tempp,"\015\012");				/* BYU 2.4.13 */
			i = strlen(tempp);						/* BYU 2.4.13 */
			if (gFTPServerPrefs->DoISOtranslation)
				trbuf_mac_ftp((unsigned char *)tempp, i);		/* transform to ISO */ //PETERL?
			if (i != Rnetwrite(ftpdata,tempp,i)) {	/* BYU 2.4.13 */
				CRESP(fnum,1);						/* BYU 2.4.16 */
				fcnt = -1;
				break;
			}
			theDirectoryState.hFileInfo.ioFDirIndex++;
			if (NULL == (nextfile = getFileName(ListingTemplate, &theDirectoryState))) {	/* normal end */
				ftpstate = 22;   			/* push data through */
			}
			break;
			
		case 30:
			if (code == CONFAIL)	/* something went wrong */
				fcnt = -1;
			if (code != CONOPEN)	/* waiting for connection to open */
				break;
			ftpstate = 31;
			crfound = 0;
			len = xp = 0;
			filelen = 0L;
			PathNameFromDirID(TelInfo->DefaultDirDirID, TelInfo->DefaultDirVRefNum, (StringPtr) theDir);
			PtoCstr((StringPtr) theDir);
			sprintf(tempp,"remote --> %s/%s\015\012",theDir,newfile);
			ftpmess(tempp);
			netputevent(USERCLASS,FTPBEGIN,-2,0);
			
		case 31:
/*
* file has already been opened, take everything from the connection
* and place into the open file: ftpfh
*/
			do {
			/* wait until xs is full before writing to disk */
				if (len <= 2000) {

					if (xp) {
						if (0 > MBwrite(mbfp, xs, xp)) {
							DoError (202 | RESOURCE_ERRORCLASS, LEVEL1, NULL);

							MBclose( mbfp);			/* Close on Disk Full Error */
							ftpstate=22;
							CRESP(fnum,27);				/* BYU 2.4.16 */
							ftpmess((char *) messs);	/* BYU LSC */
							if( (a = netclose(ftpdata)) !=0 ) 
								DoError (108 | NET_ERRORCLASS, LEVEL1, NULL);
							fcnt = -1;
							mbfp=NULL;
							break;
						}
						xp = 0;
					}
					len = BUFFERS;		/* expected or desired len to go */
				}

				if (ftptmode == FAMODE)
					fcnt = Sfread(ftpdata,(char *) &xs[xp],len);	/* BYU LSC */
				else
					fcnt = netread(ftpdata,&xs[xp],len);

				if (fcnt >= 0) {
					len -= fcnt;
					xp += fcnt;
					filelen += fcnt;
				}

				if (fcnt < 0) {
					if (0 > MBwrite( mbfp, xs, xp)) {
						CRESP(fnum,27);			/* BYU 2.4.16 */
						MBclose( mbfp);			/* Close file on error */
						break;
					}
					MBclose( mbfp );
					ftpfh = 0;
					CRESP(fnum,5);					/* BYU 2.4.16 */
				}

			} while (fcnt > 0);
			break;

		case 20:

			if (code == CONFAIL)	/* something went wrong */
				fcnt = -1;
			if (code != CONOPEN)	/* waiting for connection to open */
				break;
			ftpstate = 21;
			filelen = MBsize( mbfp );
			towrite = 0;
			xp = 0;
			netputevent(USERCLASS,FTPBEGIN,-1,0);

		case 21:
/*
*  transfer file(s) to the other host via ftp request
*  file is already open = ftpfh
*/
			netputuev(SCLASS,FTPACT,ftpdata,0);
		
			if (towrite <= xp) {

				i = BUFFERS;
				towrite = MBread( mbfp, xs, i);
				xp = 0;
			}

			if (towrite <= 0 || netest(ftpdata)) {		/* we are done */
				ftpstate = 22;
				break;
			}

			if (ftptmode == FAMODE)
				i = Sfwrite(ftpdata,&xs[xp],towrite-xp);
			else
				i = Rnetwrite(ftpdata,&xs[xp],towrite-xp);



			if (i > 0) {
				xp += i;
				filelen -= i;
				if (filelen < 0L)
					filelen = 0L;
			}

			break;

		case 22:		/* wait for data to be accepted */
			netputuev(SCLASS,FTPACT,ftpdata,0);

			fcnt = netpush(ftpdata);		/* will go negative on err */
			if (!fcnt || netest(ftpdata))
				fcnt = -1;
			if (fcnt < 0) {
				CRESP(fnum,5);				/* BYU 2.4.16 */
			}
			break;

		case 0:
			break;

	}  /* end of switch */

/*	after reading from connection, if the connection is closed, reset up shop. */
	if (fcnt < 0) {
		if (mbfp) {
			MBclose( mbfp );
			mbfp = NULL;
		}
		ftpstate = 0;
		fcnt = 0;
		if (ftpdata >= 0) {
			netclose(ftpdata);
			netputevent(USERCLASS,FTPEND,-1,0);
			mainport[ftpdata] = -1;				/* BYU 2.4.16 */
			dataport[fnum] = -1;				/* BYU 2.4.16 */
		}
	}

	datastate[fnum] = ftpstate;			/* BYU 2.4.16 */

}


/* Sftpname and Sftpuser and Sftphost
*  record the name of the file being transferred, to use in the status
*  line updates
*/

void Sftpname(char *s)
{
	strcpy(s, (char *) newfile);	/* BYU LSC */
}

ip_addr Sftphost(void)
{
	return(hishostaddr);
}

void Sftpstat(long *byt)
{
	*byt = filelen;
}

void FTPLogUserLoggingIn(void)
{
	ftpmess("FTP user ");
	ftpmess((char *) myuser);
	ftpmess(" logging in\015\012");
}

/****************************************************************************/
/* Sencompass - Compute the encrypted password */
void Sencompass(char *ps, char *en)
{
	short	i,ck;
	char	*p,c;

	ck = 0;
	p = ps;
	while (*p)				/* checksum the string */
		ck += *p++;

	c = ck;

	for (i=0; i<10; i++) {
		*en =  (((*ps ^ c) | 32) & 127); 	/* XOR with checksum */
		if (*ps)
			ps++;
		else
			c++;		/* to hide length */
		en++;
	}

	*en = 0;
} /* Sencompass */

/****************************************************************************/
/* Scompass - compute and check the encrypted password */
Boolean Scompass(char *ps, char *en)
{
	short	ck;
	char	*p,c;

	ck = 0;
	p = ps;
	while (*p)				/* checksum the string */
		ck += *p++;

	c = ck;

	while (*en) {
		if ((((*ps ^ c) | 32) & 127) != *en)	/* XOR with checksum */
			return(FALSE);
		if (*ps)
			ps++;
		else
			c++;			/* increment checksum to hide length */
		en++;
	}

	return(TRUE);
}

/****************************************************************************/
/*  Scheckpass - Check the password file for the user, password combination */
Boolean Scheckpass(char *us, char *ps)
{
	FTPUserPrefs		**FTPUhdl;
	Str255		scratchPstring;
	CInfoPBRec	theInfo;
	
	strcpy((char *)scratchPstring, us);
	CtoPstr((char *)scratchPstring);
	
	UseResFile(TelInfo->SettingsFile);
	FTPUhdl = (FTPUserPrefs **)Get1NamedResource(FTPUSERPREFS_RESTYPE, scratchPstring);
	if (ResError()) return(FALSE);		// User not found
	
	HLock((Handle)FTPUhdl);
	BlockMoveData((**FTPUhdl).EncryptedPassword, scratchPstring,
				StrLength((**FTPUhdl).EncryptedPassword)+1);
	PtoCstr(scratchPstring);
	
	if (Scompass(ps, (char *)scratchPstring)) {		// Password matches
		TelInfo->DefaultDirDirID = (**FTPUhdl).DefaultDirDirID;
		TelInfo->DefaultDirVRefNum = VolumeNameToRefNum((**FTPUhdl).DefaultDirVolName);
		WriteZero((Ptr)&theInfo, sizeof(CInfoPBRec));
		theInfo.dirInfo.ioVRefNum = TelInfo->DefaultDirVRefNum;
		theInfo.dirInfo.ioDrDirID = TelInfo->DefaultDirDirID;
		theInfo.dirInfo.ioNamePtr = scratchPstring;
		theInfo.dirInfo.ioFDirIndex = -1;		// Only give me the Directory Info
		if (PBGetCatInfoSync(&theInfo) != noErr) {
			TelInfo->DefaultDirDirID = 2;
			TelInfo->DefaultDirVRefNum = -1;
			}
		ReleaseResource((Handle)FTPUhdl);
		return(TRUE);
		}
	
	ReleaseResource((Handle)FTPUhdl);
	return(FALSE);									// Password was no good
}

/*
 * Authencrypt.c
 * Authentication module for NCSA/Telnet and Brown tn3270.
 */

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "tnae.h"
#include "authencrypt.h"
#include "authencrypt.proto.h"
#include "KrbDriver.h"
#include "DlogUtils.proto.h"
#include "network.proto.h"
#include "MixedMode.h"

#define NAUTHTYPES	10					/* Max number of auth types */

struct codemodule *authmodules = 0;	/* list of authentication code modules */

static char nullbuf[] = {IAC, SB, OPT_AUTHENTICATION, TNQ_IS, AUTH_NULL, 
							 AUTH_CLIENT_TO_SERVER|AUTH_HOW_ONE_WAY, IAC, SE};

#ifdef powerc
enum {
    uppModule = kCStackBased
		| RESULT_SIZE(SIZE_CODE(sizeof(long)))
	    | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(long)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Ptr)))
};

#define PluginProc(userRoutine, code, pointer)		\
			CallUniversalProc((UniversalProcPtr)(userRoutine), uppModule, code, pointer)
#else /* powerc */

#define PluginProc(userRoutine, code, pointer)		\
		(*(userRoutine))(code, pointer)
#endif /* powerc */

#ifdef __MWERKS__
#pragma profile off
#endif

static	void	scanFolder(short vRef, long dirID);
static void loadCode(HParamBlockRec *pb, long dirid, Str255 name, OSType type, codemodule **header);
short hicall (long cscode, krbHiParmBlock *khipb, short kdriver);


/*
 * auth_encrypt_init
 * Initialize processing for auth/encrypt options.
 * Find all the modules that support auth/encrypt and add them to our
 * typepairs and modules table. Load those modules and initialize them.
 *
 * Return true if any auth module found.
 */
Boolean auth_encrypt_init ()
{
	short vref;
	long dirid = 0, fold;
	SysEnvRec theWorld;

	/*
	 * Find and load/initialize files with code modules.
	 * First, Try to find the Extensions folder, else use the system folder.
	 */
	if (Gestalt('fold', &fold)  || 
		((fold & 1) != 1) ||
		FindFolder(kOnSystemDisk, kExtensionFolderType, false, &vref, &dirid)) {
		if (SysEnvirons (1, &theWorld) == 0)
			vref = theWorld.sysVRefNum;
		else
			vref = -1;
	}

	scanFolder(vref, dirid);
	
	// Now scan the folder that Telnet lives in
	scanFolder(TelInfo->ApFolder.vRefNum, TelInfo->ApFolder.parID);
	
	if (authmodules)
		return true;
	return false;
}

/*
 * auth_encrypt_end
 * Allow authentication plugins to free memory, etc
 */
void auth_encrypt_end(tnParams **aedata)
{
	int i;
	OSErr s;
	tnParams *tn = *aedata;		

	if (tn->entry)
		s = PluginProc(tn->entry, TNFUNC_END_SESSION, tn);
}

static	void	scanFolder(short vRef, long dirID)
{
	short			i;
	HParamBlockRec	pb;
	Str255			name;
	OSErr			s;
	
	for (i = 1; ; i++ ) {
		pb.fileParam.ioCompletion = 0;
		pb.fileParam.ioVRefNum = vRef;
		pb.fileParam.ioFVersNum = 0;
		pb.fileParam.ioFDirIndex = i;
		pb.fileParam.ioDirID = dirID;
		pb.fileParam.ioNamePtr = name;
		if ((s = PBHGetFInfoSync(&pb)) != noErr)
			break;

		/*	Check for module types that we are interested in. */
		if (pb.fileParam.ioFlFndrInfo.fdType == moduleType) {
			OSType *authType = authTypes;
			
			while(*authType != 0)
			{
				loadCode(&pb, dirID, name, *authType, &authmodules);
				++authType;
			}
		}
	}
}

void loadCode (HParamBlockRec *pb, long dirid, Str255 name, OSType type, codemodule **header)
{
	int i;
	short rf, oldrf;
	struct codemodule *code = 0;
	Handle h;

	oldrf = CurResFile();
	rf = HOpenResFile(pb->fileParam.ioVRefNum, dirid, name, fsRdPerm);	
	if (rf == -1)
		return;
	SetResLoad(true);
	
	/*
	 * Find all resources of indicated type.
	 */
	for (i = 1; ; i++) {
		if (!((h = Get1IndResource(type, i))))
			break;

		code = (struct codemodule *)myNewPtr(sizeof(struct codemodule));
		if (code) {
			DetachResource(h); 
			HNoPurge(h);
			HLockHi(h);
			code->entry = (module)*h;
			code->authType = type;
			/*
			 * Initialize the module.
			 * It should preset the type/pairs list and return the number of
			 * pairs entered.
			 */

			code->npairs = PluginProc(code->entry, TNFUNC_INIT_CODE, &code->pairs);
			code->encryptok = PluginProc(code->entry, TNFUNC_QUERY_ENCRYPT, 0);

			qlink((void **)header, code);
		} else
			ReleaseResource(h);
	}
	CloseResFile(rf);  /* ddd for debugging with The Debugger ***/
	UseResFile(oldrf);
}


/*
 * auth_suboption
 * Called by the Telnet client when an authentication sub-option is received.
 * The reply option (if any) is placed into sendbuffer and *sendlength adjusted
 * by the amount of data placed into sendbuffer.
 */
void auth_suboption (tnParams **aedata, unsigned char *subbuffer, long sublength,
	unsigned char *sendbuffer, unsigned long *sendlength, char *cname, Boolean hisencrypt,
	Boolean myencrypt, unsigned short port, Boolean forward, char *username)
{
	int i;
	OSErr s;
	unsigned short pair;
	unsigned char *cp, *buflimit;
	struct codemodule *code = 0;
	tnParams *tn;						/* temp params struct */
	
	buflimit = subbuffer + sublength;

	/*
	 * Initialize session's tnParams if not initialized yet.
	 * Return null auth if no memory and TNQ_SEND.
	 */
	if (!(*aedata)) {
		*aedata = (tnParams *)myNewPtr(sizeof(tnParams));
		if (!(*aedata)) {
			if (subbuffer[SB_SUBOPTION] == TNQ_SEND) {
				BlockMoveData((Ptr)nullbuf, (Ptr)sendbuffer, sizeof(nullbuf));
				*sendlength -= sizeof(nullbuf);
			}
			return;
		}
	}
	tn = *aedata;
	
	switch (subbuffer[SB_SUBOPTION]) {
	case TNQ_IS:
	case TNQ_NAME:
		/* The client should not get one of these */
		break;

	case TNQ_SEND:
		/*
		 * For telnet clients, the buffer contains:
		 * AUTHENTICATION SEND type modifier [type modifier] [...] IAC SE
		 * DDD is IAC SE in the buffer???
		 * We scan the type/modifier pairs until we find one we can do. 
		 * Since they are are in priority order, the  first one we
		 * find that we can do wins.
		 */
		for (cp = &subbuffer[SB_TYPE]; cp < buflimit; cp += 2) {
			pair = (cp[0] << 8) | cp[1];
			for (code = authmodules; code; code = code->next) {
				for (i = 0; i < code->npairs; i++)  {
					if (pair == code->pairs[i])
						goto brk;
				}
			}
		}
	brk:
		if (!code) {
			/*
			 * If no methods match, send null authentication.
			 */
			BlockMoveData((Ptr)nullbuf, (Ptr)sendbuffer, sizeof(nullbuf));
			*sendlength -= sizeof(nullbuf);
			return;
		}

		/*
		 * If no auth data, initialize it now.
		 */
		if (!(tn->authdata)) {
			switch (code->authType) {
			case 'TNae':
				s = PluginProc(code->entry, TNFUNC_INIT_SESSION_AUTH, &tn->authdata);
				if ((s == 0) && !tn->encryptdata) {
					s = PluginProc(code->entry, TNFUNC_INIT_SESSION_ENCRYPT, &tn->encryptdata);
					tn->encrType = code->authType;
				}
				break;
			default:
				netgetip(tn->ipaddr);
				tn->port = netgetport(port);
				tn->username = username;
				s = PluginProc(code->entry, TNFUNC_INIT_SESSION_AUTH, tn);
				if ((s == 0) && !tn->encryptdata) {
					s = PluginProc(code->entry, TNFUNC_INIT_SESSION_ENCRYPT, tn);
					tn->encrType = code->authType;
				}
			}

			if (s) {					/* if no memory, etc */
				BlockMoveData((Ptr)nullbuf, (Ptr)sendbuffer, sizeof(nullbuf));
				*sendlength -= sizeof(nullbuf);
				return;
			}
			tn->entry = code->entry;
		}

		/*
		 * Process the SEND option
		 */
		tn->subbuffer = subbuffer;
		tn->sublength = sublength;
		tn->sendbuffer = sendbuffer;
		tn->sendlength = sendlength;
		tn->cname = cname;
		tn->hisencrypt = hisencrypt;
		tn->myencrypt = myencrypt;
		s = PluginProc(tn->entry, TNFUNC_AUTH_SEND, tn);		
		if (s) {
			/* ddd null probably wrong here ??? */
			BlockMoveData((Ptr)nullbuf, (Ptr)sendbuffer, sizeof(nullbuf));
			*sendlength -= sizeof(nullbuf);
		}
		return;

	case TNQ_REPLY:
		/*
		 * Process the reply.
		 */
		if (!(tn->authdata)) {
			DebugStr("\pauth_suboption: tnq_reply no authdata");
			return;
		}
		tn->subbuffer = subbuffer;
		tn->sublength = sublength;
		tn->sendbuffer = sendbuffer;
		tn->sendlength = sendlength;
		tn->cname = cname;
		tn->forward = forward ? 1 : -1;
		tn->hisencrypt = hisencrypt;
		tn->myencrypt = myencrypt;
		s = PluginProc(tn->entry, TNFUNC_AUTH_REPLY, tn);
		switch (s) {
		case TNREP_OK:
			return;

		case TNREP_AUTH_OK:
			break;

		case TNREP_AUTH_ERR:
			break;
		}
		return;
	}
}


/*
 * encrypt_suboption
 * Called by the Telnet client when an encryption sub-option is received.
 * The reply option (if any) is placed into sendbuffer and *sendlength adjusted
 * by the amount of data placed into sendbuffer.
 */
short encrypt_suboption (tnParams **aedata, unsigned char *subbuffer, long sublength, unsigned char *sendbuffer, unsigned long *sendlength, char *cname, Boolean hisencrypt, Boolean myencrypt)
{
	short s;
	tnParams *tn;
	struct codemodule *code = 0;
			
	/*
	 * Initialize session's tnParams if not there. 
	 * Return null auth if no memory and TNQ_SEND.
	 */
	if (!(*aedata)) {
		*aedata = (tnParams *)myNewPtr(sizeof(tnParams));
		if (!(*aedata)) {
			return 0;
		}
	}
	tn = *aedata;

	if (!tn->encryptdata) {
		for (code = authmodules; code; code = code->next) {
			if (code->encryptok)
				break;
		}
		if (!code) {
			//DisposePtr(*aedata);
			//*aedata = NULL;
			return 0;
		}

		switch (code->authType)
		{
		case 'TNae':
			s = PluginProc(code->entry, TNFUNC_INIT_SESSION_ENCRYPT, &tn->encryptdata);
			break;
		default:
			s = PluginProc(code->entry, TNFUNC_INIT_SESSION_ENCRYPT, tn);
		}

		if (s)
			return TNREP_ERROR;

		tn->entry = code->entry;
		tn->encrType = code->authType;
	}

	tn->subbuffer = subbuffer;
	tn->sublength = sublength;
	tn->sendbuffer = sendbuffer;
	tn->sendlength = sendlength;
	tn->cname = cname;
	tn->hisencrypt = hisencrypt;
	tn->myencrypt = myencrypt;
	s = PluginProc(tn->entry, TNFUNC_ENCRYPT_SB, tn);
	return s;
}


void kdecrypt (tnParams *tn, unsigned char *buf, long len)
{
	short s;
	
	tn->data = len;
	tn->ebuf = buf;
	if (tn->encrType == 'TNae')
		s = TNREP_ERROR;
	else
	s = PluginProc(tn->entry, TNFUNC_DECRYPT2, tn);
	if (s == TNREP_ERROR)		// plugin must not support TNFUNC_DECRYPT2
	{
		while (len-- > 0) {
 			tn->data = (long)*buf;
 			PluginProc(tn->entry, TNFUNC_DECRYPT, tn);
 			*buf = (unsigned char)tn->data;
			buf++;
		}
	}
}


void kencrypt (tnParams *tn, unsigned char *buf, long len)
{
	tn->data = len;
	tn->ebuf = buf;
	PluginProc(tn->entry, TNFUNC_ENCRYPT, tn);
}


/*
 * qlink
 * Add an entry to the end of a linked list
 */
void qlink (void **flist, void *fentry)
{
    struct dummy {
		struct dummy *next;
    } **list, *entry;

    list = (struct dummy **)flist;
    entry = (struct dummy *)fentry;
    
    /*
     * Find address of last entry in the list.
     */
    while (*list)
	list = &(*list)->next;

    /*
     * Link entry
     */
    *list = entry;
    entry->next = 0;
}


/*
 * qunlink
 * Remove an entry from linked list
 * Returns the entry or NULL if not found.
 */
void *qunlink (void **flist, void *fentry)
{
    struct dummy {
		struct dummy *next;
    } **list, *entry;

    list = (struct dummy **)flist;
    entry = (struct dummy *)fentry;
    
    /*
     * Find entry and unlink it
     */
    while (*list) {
		if ((*list) == entry) {
			*list = entry->next;
			return entry;
		}
	
		list = &(*list)->next;
    }
    return NULL;
}


short hicall (long cscode, krbHiParmBlock *khipb, short kdriver)
{
	short s;
	ParamBlockRec pb;
	
	WriteZero((Ptr)&pb, sizeof(ParamBlockRec));
	*(long *)pb.cntrlParam.csParam = (long)khipb;
	pb.cntrlParam.ioCompletion = nil;
	pb.cntrlParam.ioCRefNum = kdriver;

	pb.cntrlParam.csCode = cscode;
	if (s = PBControlSync(&pb))
		return s;
	if (s = pb.cntrlParam.ioResult)
		return s;
	return 0;
}

void	DestroyTickets(void)
{
	struct codemodule *code = NULL;
	OSErr			err;
	//short authRefNumkrb;
	krbHiParmBlock khpb, *khipb = &khpb;
	short kdriver;

	for (code = authmodules; code; code = code->next)
	{
		switch(code->authType)
		{
			case 'TNae':
			
				if (!(err = OpenDriver("\p.Kerberos", &kdriver)))
				{
					WriteZero((Ptr)khipb, sizeof(krbHiParmBlock));
					if (err = hicall(cKrbDeleteAllSessions, khipb, kdriver))
					return;
				}
				//else if (!(err=openAuthMan(&authRefNum,&authAPIversion)))
				//	if (err=expireV4Ticket(authRefNum,NULL,NULL,NULL))
				//		return;
				break;
			default:
				PluginProc(code->entry, TNFUNC_DESTROY_CREDS, 0);
		}
	}
}

/*********************************************************************
File		:	GUSI				-	Grand Unified Socket Interface
File		:	GUSIPPCTest		-	Test PPC sockets
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:39  chombier
Initial import

Revision 1.1  2000/10/29 20:31:53  neeri
Releasing 2.1.3

Revision 1.2  1994/12/31  01:05:05  neeri
ANSIfy (omitting parameter names was illegal).

Revision 1.1  1994/02/25  02:47:36  neeri
Initial revision

Revision 0.3  1993/06/20  00:00:00  neeri
New sa_constr

Revision 0.2  1992/10/14  00:00:00  neeri
Fix NBP type, usage messages

Revision 0.1  1992/09/08  00:00:00  neeri
Factor out common socket routines

*********************************************************************/

#include "GUSITest.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ppc.h>

#include <Events.h>
#include <TextUtils.h>
#include <PLStringFuncs.h>

void Socket(char ch1, char ch2, const char * line)
{
	sock	=	socket(AF_PPC, SOCK_STREAM, 0);
	
	if (sock == -1)	{
		printf("# socket() returned error %s\n", Explain());
		Where();
	}
}

void Bind(char ch1, char ch2, const char * cmd)
{
	struct sockaddr_ppc	addr;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}

	addr.sppc_family	=	AF_PPC;
	addr.sppc_port.nameScript						=	smRoman;
	addr.sppc_port.portKindSelector				=	ppcByString;
	addr.sppc_location.locationKindSelector	=	ppcNBPTypeLocation;
	
	if (sscanf(cmd, "%s %s", addr.sppc_port.name+1, addr.sppc_location.u.nbpType+1) != 2) {
		Usage(ch1, ch2);
			
		return;
	}
	
	addr.sppc_port.name[0] 				= strlen(((char *)addr.sppc_port.name)+1);
	addr.sppc_location.u.nbpType[0] = strlen(((char *)&addr.sppc_location.u.nbpType)+1);

	PLstrcpy((StringPtr) &addr.sppc_port.u.portTypeStr, "\pGUSIPPCTest");

	if (bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_ppc)))	{
		printf("bind() returned error %s\n", Explain());
		Where();
	}
}

void Accept(char ch1, char ch2, const char * line)
{
	socklen_t				len;
	struct sockaddr_ppc	addr;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}
	if (accsock != -1)	{
		printf("# can't accept more than one connection\n");
		Where();
		
		return;
	}

	len	=	sizeof(struct sockaddr_ppc);
	sock	=	accept(accsock = sock, (struct sockaddr *) &addr, &len);
	
	if (sock < 0)	{
		printf("# accept() returned error %s\n", Explain());
		sock		=	accsock;
		accsock	=	-1;
	} else {
		addr.sppc_port.name[addr.sppc_port.name[0]+1] = 0;
		addr.sppc_port.u.portTypeStr[addr.sppc_port.u.portTypeStr[0]+1] = 0;
		printf(
			"# accepted connection from %s[%s]", 
			((char *)addr.sppc_port.name)+1, 
			((char *)addr.sppc_port.u.portTypeStr)+1);
		
		switch (addr.sppc_location.locationKindSelector) {
		case ppcNBPLocation:
			addr.sppc_location.u.nbpEntity.objStr[addr.sppc_location.u.nbpEntity.objStr[0]+1] = 0;
			addr.sppc_location.u.nbpEntity.typeStr[addr.sppc_location.u.nbpEntity.typeStr[0]+1] = 0;
			addr.sppc_location.u.nbpEntity.zoneStr[addr.sppc_location.u.nbpEntity.zoneStr[0]+1] = 0;
			printf(
				"@%s:%s:%s\n", 
				((char *)addr.sppc_location.u.nbpEntity.objStr)+1, 
				((char *)addr.sppc_location.u.nbpEntity.typeStr)+1, 
				((char *)addr.sppc_location.u.nbpEntity.zoneStr)+1);
			break;
		case ppcNBPTypeLocation:
			addr.sppc_location.u.nbpType[addr.sppc_location.u.nbpType[0]+1] = 0;
			printf("@%s\n", ((char *)addr.sppc_location.u.nbpType)+1);
			break;
		default:
			printf("\n");
			break;
		}
	}
	
	Where();
}

Boolean BrowseForName(struct sockaddr_ppc * name)
{
	PortInfoRec				info;
	
	if (PPCBrowser(
			"\pConnect where?", 
			(StringPtr) "\p", 
			false, 
			&name->sppc_location, 
			&info, 
			(PPCFilterUPP)nil,
			"\pGUSIPPCTest"))
		return false;
		
	name->sppc_family	=	AF_PPC;
	name->sppc_port	=	info.name;

	return true;
}

void Connect(char ch1, char ch2, const char * cmd)
{
	socklen_t				len;
	struct sockaddr_ppc	addr;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}
	
	if (!BrowseForName(&addr))
		return;

	if (connect(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_ppc)))	{
		printf("# connect() returned error %s\n", Explain());
		Where();
	}
}	

main(int argc, char ** argv)
{
	printf("GUSIPPCTest		MN 10Jan98\n\n");

	COMMAND('s', 's', Socket,  "", 				"Create a stream socket");
	COMMAND('b', 'd', Bind,  	"Name Type", 	"Bind to address");
	COMMAND('c', 'o', Connect, "", 				"Connect to address");
	COMMAND('a', 'c', Accept,  "", 				"Accept a connection");
	
	AddSocketCommands();
	
	RunTest(argc, argv);
	CleanupSockets();
}
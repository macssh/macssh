/*********************************************************************
Project	:	GUSI				-	Grand Unified Socket Interface
File		:	GUSIINETTest	-	Test unix domain sockets
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:39  chombier
Initial import

Revision 1.4  2000/10/15 21:49:20  neeri
Add multiple connection test

Revision 1.3  2000/05/23 06:40:03  neeri
Add Linger, Getsockname tests

Revision 1.2  2000/03/06 08:28:32  neeri
Releasing 2.0.5

Revision 1.1  1998/10/25 11:57:26  neeri
Ready to release 2.0a3

Revision 1.1  1994/12/31  00:50:27  neeri
Initial revision

Revision 0.6  1993/08/25  00:00:00  neeri
gethostbyaddr()

Revision 0.5  1993/07/29  00:00:00  neeri
servent tests

Revision 0.4  1993/03/03  00:00:00  neeri
Added performance test

Revision 0.3  1992/09/14  00:00:00  neeri
Misinterpreted hostent structure

Revision 0.2  1992/09/08  00:00:00  neeri
Factor out common socket routines

Revision 0.1  1992/09/08  00:00:00  neeri
First attempt

*********************************************************************/

#include "GUSITest.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "Events.h"

char	addrstr[100];

void Socket(char ch1, char ch2, const char * line)
{
	sock	=	socket(AF_INET, (ch1 == 's') ? SOCK_STREAM : SOCK_DGRAM, 0);
	
	if (sock == -1)	{
		printf("# socket() returned error %d [%s]\n", errno, Explain());
		Where();
	}
}

void Bind(char ch1, char ch2, const char * cmd)
{
	int						len;
	struct sockaddr_in	addr;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}

	if (sscanf(cmd, "%s %hd", addrstr, &addr.sin_port) == 2) {
		addr.sin_addr.s_addr	=	inet_addr(addrstr);
		addr.sin_family		=	AF_INET;
		len						=	sizeof(struct sockaddr_in);
	} else {
		Usage(ch1, ch2);
		return;
	}
	
	if (bind(sock, (struct sockaddr *) &addr, len))	{
		printf(
			"bind(%s:%d) returned error %d [%s]\n",  
			inet_ntoa(addr.sin_addr),
			addr.sin_port, 
			errno, Explain());
			
		Where();
	}
}

void NoDelay(char , char , const char * cmd)
{
	int one	= 1;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		return;
	}

	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(int)))
		printf(
			"setsockopt(IPPROTO_TCP, TCP_NODELAY) returned error %d [%s]\n",  
			errno, Explain());
}

void Linger(char ch1, char ch2, const char * cmd)
{
	struct linger	l;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}

	if (sscanf(cmd, "%d", &l.l_linger) == 1) {
		l.l_onoff = l.l_linger > -1;
	} else {
		Usage(ch1, ch2);
		return;
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof(struct linger)))
		printf(
			"setsockopt(SOL_SOCKET, SO_LINGER) returned error %d [%s]\n",  
			errno, Explain());
}

void Accept(char ch1, char ch2, const char * line)
{
	socklen_t				len;
	struct sockaddr_in	addr;

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

	len	=	sizeof(struct sockaddr_in);
	sock	=	accept(accsock = sock, (struct sockaddr *) &addr, &len);
	
	if (sock < 0)	{
		printf("# accept() returned error %d [%s]\n", errno, Explain());
		sock		=	accsock;
		accsock	=	-1;
	} else {
		printf(
			"# accepted connection from %s port %d\n", 
			inet_ntoa(addr.sin_addr), 
			addr.sin_port);
	}
	
	Where();
}

void Getsockname(char ch1, char ch2, const char * line)
{
	socklen_t			len;
	struct sockaddr_in	addr;
	int					err;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}

	len	=	sizeof(struct sockaddr_in);
	
	if (ch2 == 's')
		err = getsockname(accsock = sock, (struct sockaddr *) &addr, &len);
	else
		err = getpeername(accsock = sock, (struct sockaddr *) &addr, &len);
	if (err < 0)	{
		printf("# get%sname() returned error %d [%s]\n", 
			(ch2 == 's' ? "sock" : "peer"), errno, Explain());
	} else {
		printf(
			"# %s are %s port %d\n", (ch2 == 's' ? "We" : "They"), 
			inet_ntoa(addr.sin_addr), 
			addr.sin_port);
	}
	
	Where();
}
	
void Connect(char ch1, char ch2, const char * cmd)
{
	int						len;
	struct sockaddr_in	addr;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}
	
	if (sscanf(cmd, "%s %hd", addrstr, &addr.sin_port) == 2) {
		addr.sin_addr.s_addr	=	inet_addr(addrstr);
		addr.sin_family		=	AF_INET;
		len						=	sizeof(struct sockaddr_in);
	} else {
		Usage(ch1, ch2);
		return;
	}

	if (connect(sock, (struct sockaddr *) &addr, len))	{
		printf(
			"connect(%s:%d) returned error %s\n", 
			inet_ntoa(addr.sin_addr),
			addr.sin_port, 
			Explain());
		Where();
	}
}	

static int sSock[1000];
static int sSockUsed;
	
void MultiConn(char ch1, char ch2, const char * cmd)
{
	int					len;
	int					count;
	int					i;
	struct sockaddr_in	addr;

	if (sscanf(cmd, "%d %s %hd", &count, addrstr, &addr.sin_port) == 3) {
		addr.sin_addr.s_addr	=	inet_addr(addrstr);
		addr.sin_family		=	AF_INET;
		len						=	sizeof(struct sockaddr_in);
	} else {
		Usage(ch1, ch2);
		return;
	}

	if (count > 1000-sSockUsed)
		count = 1000-sSockUsed;
	for (i=0; i<count; ++i) {
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(sock, (struct sockaddr *) &addr, len))	{
			printf(
				"#%d: connect(%s:%d) returned error %s\n", 
				i,
				inet_ntoa(addr.sin_addr),
				addr.sin_port, 
				Explain());
			Where();
			close(sock);
			if (!i)
				break;
		} else {
			struct sockaddr_in	addr;
			socklen_t			len;
			
			len =	sizeof(struct sockaddr_in);
			getsockname(sock, (struct sockaddr *) &addr, &len);
			printf("#%d connected from port %d\n", i, addr.sin_port);
			sSock[sSockUsed++] = sock;
		}
	}
}	

void MultiDis(char, char, const char * cmd)
{
	while (sSockUsed > 0)
		close(sSock[--sSockUsed]);
}

int ReadWhileUCan()
{
	int				res;
	char *			outline;
	fd_set			rdfds;
	fd_set			exfds;
	struct timeval	delay;
	char 				out[500];

	for (;;) {
		FD_ZERO(&rdfds);
		FD_ZERO(&exfds);
		
		FD_SET(sock, &rdfds);
		FD_SET(sock, &exfds);
		
		delay.tv_sec	=	1;
		delay.tv_usec	=	0;
		
		res = select(sock+1, &rdfds, nil, &exfds, &delay);
		
		if (res < 0)	{
			printf("# select() returned error %d [%s]\n", errno, Explain());
			
			return -1;
		} else if (!res)
			return 0;
		else if (res && FD_ISSET(sock, &exfds)) {
			printf("# select() returned an exception\n");
			
			return -1;
		} else if (res && FD_ISSET(sock, &rdfds)) {
			res = read(sock, out, 500);
			
			if (res < 0) {
				printf("# read() returned error %d [%s]\n", errno, Explain());
			
				return -1;
			}
			
			out[res] = 0;
			
			for (outline = strtok(out, "\n\r"); outline; outline = strtok(nil, "\n\r"))
				printf("%s\n", outline);
		}
	}
}

void Telnet(char ch1, char ch2, const char * cmd)
{
	int				len;
	int 				part;
	int				res;
	char *			line;
	char *			outline;
	fd_set			rdfds;
	fd_set			wrfds;
	fd_set			exfds;
	struct timeval	delay;
	char 				in[100];
	char 				out[500];
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}

	printf("# Entering Poor Man's Telnet mode\n");
	
	for (;;) {
		if (ReadWhileUCan())
			break;
		
		Prompt();
		
		if (!fgets(in, 100, input))
			break;
			
		++inputline;
		
		if (!strcmp(in, ".\n"))
			break;
		
		if (!strcmp(in, "?\n"))
			continue;

		len 			= strlen(in);
		in[len++]	= '\r';
		in[len]		= 0;
		
		for (line = in; len; len -= part, line += part) {		
			part = 0;

			FD_ZERO(&rdfds);
			FD_ZERO(&wrfds);
			FD_ZERO(&exfds);
			
			FD_SET(sock, &rdfds);
			FD_SET(sock, &wrfds);
			FD_SET(sock, &exfds);
			
			delay.tv_sec	=	10;
			delay.tv_usec	=	0;
			
			res = select(sock+1, &rdfds, &wrfds, &exfds, &delay);
			
			if (res < 0)	{
				printf("# select() returned error %d [%s]\n", errno, Explain());
				
				return;
			} else if (!res) {
				printf("# select() timed out\n");
				
				return;
			} else if (FD_ISSET(sock, &exfds)) {
				printf("# select() returned an exception\n");
				
				return;
			}
			
			if (FD_ISSET(sock, &rdfds)) {
				res = read(sock, out, 500);
				
				if (res < 0) {
					printf("# read() returned error %d [%s]\n", errno, Explain());
				
					return;
				}
				
				out[res] = 0;
				
				for (outline = strtok(out, "\n\r"); outline; outline = strtok(nil, "\n\r"))
					printf("%s\n", outline);
			} else if (FD_ISSET(sock, &wrfds)) {
				res = write(sock, line, len);
			
				if (res < 0) {
					line[strlen(line) - 2] = 0;
					printf("# write(\"%s\") returned error %s\n", line, Explain());
				
					return;
				}
				
				part = res;
			}
		}
	}
	
	printf("# Leaving Poor Man's Telnet mode\n");
}

void N2Addr(char ch1, char ch2, const char * cmd)
{
	struct in_addr		addr;
	struct hostent *	ent;
	
	if (sscanf(cmd, "%s", addrstr) == 1)
		if (ent = gethostbyname(addrstr)) {
			memcpy(&addr, ent->h_addr, sizeof(struct in_addr));
			printf("# gethostbyname(%s) returned %s\n", addrstr, inet_ntoa(addr));
		} else
			printf("# gethostbyname(%s) failed.\n", addrstr);
	else
		Usage(ch1, ch2);
}

void A2Name(char ch1, char ch2, const char * cmd)
{
	in_addr_t			addr;
	struct hostent *	ent;
	
	if (sscanf(cmd, "%s", addrstr) == 1) {
		addr = inet_addr(addrstr);
		if (ent = gethostbyaddr((char *) &addr, 0, 0)) {
			printf("# gethostbyaddr(%s) returned %s\n", addrstr, ent->h_name);
		} else
			printf("# gethostbyaddr(%s) failed.\n", addrstr);
	} else
		Usage(ch1, ch2);
}

void PerfTest(char ch1, char ch2, const char * cmd)
{
	int 	count;
	int 	i;
	long	ticks;
	char	buf[1024];
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}

	if (sscanf(cmd, "%d", &count) == 1) {
		ticks = TickCount();
		for (i=0; i<count; i++) {
			write(sock, buf, 1);
			read(sock, buf, 1024);
		} 
		printf("# Did %d iterations in %d ticks.\n", count, TickCount()-ticks);
	} else
		Usage(ch1, ch2);
}

void ServByName(char ch1, char ch2, const char * cmd)
{
	struct servent *	ent;
	char					proto[6];
	
	if (sscanf(cmd, "%s %s", addrstr, proto) == 2)
		if (ent = getservbyname(addrstr, proto))
			printf("# getservbyname(%s %s) returned %d\n", addrstr, proto, ent->s_port);
		else
			printf("# getservbyname(%s %s) failed.\n", addrstr, proto);
	else if (sscanf(cmd, "%s", addrstr) == 1)
		if (ent = getservbyname(addrstr, NULL))
			printf("# getservbyname(%s) returned %d\n", addrstr, ent->s_port);
		else
			printf("# getservbyname(%s) failed.\n", addrstr);
	else
		Usage(ch1, ch2);
}

void PrintServEnt(struct servent * ent, Boolean full)
{
	char	** al;
	
	printf("%s", ent->s_name);
	
	if (full)
	 	printf(" %d/%s", ent->s_port, ent->s_proto);
		
	for (al = ent->s_aliases; *al; ++al)
		printf(" %s", *al);
	
	putchar('\n');
}

void ServByPort(char ch1, char ch2, const char * cmd)
{
	int					port;
	struct servent *	ent;
	char					proto[6];
	
	if (sscanf(cmd, "%d %s", &port, proto) == 2)
		if (ent = getservbyport(port, proto)) {
			printf("# getservbyport(%d %s) returned ", port, proto);
			
			PrintServEnt(ent, false);
		} else
			printf("# getservbyport(%d %s) failed.\n", addrstr, proto);
	else if (sscanf(cmd, "%d %s", &port) == 1)
		if (ent = getservbyport(port, NULL)) {
			printf("# getservbyport(%d) returned ", port);
			
			PrintServEnt(ent, false);
		} else
			printf("# getservbyport(%d) failed.\n", addrstr);
	else
		Usage(ch1, ch2);
}

void ServList(char ch1, char ch2, const char * line)
{
	struct servent *	ent;
	
	setservent(0);
	
	while (ent = getservent()) {
		printf("# ");
		PrintServEnt(ent, true);
	}
}

main(int argc, char ** argv)
{
	printf("GUSIINETTest		MN 14May00\n\n");

	COMMAND('s', 's', Socket,  "", 				"Create a stream socket");
	COMMAND('d', 's', Socket,  "", 				"Create a datagram socket");
	COMMAND('b', 'd', Bind,  	"addr port", 	"Bind to address");
	COMMAND('c', 'o', Connect, "addr port", 	"Connect to address");
	COMMAND('m', 'c', MultiConn, "# addr port", "Connect multiple times to address");
	COMMAND('m', 'd', MultiDis, "",				"Disconnect the connections");
	COMMAND('a', 'c', Accept,  "", 				"Accept a connection");
	COMMAND('t', 'n', Telnet,  "",				"Play telnet");
	COMMAND('n', 'a', N2Addr,	"name",			"Convert name to address");
	COMMAND('a', 'n', A2Name,	"addr",			"Convert address to name");
	COMMAND('p', 't', PerfTest,"count",			"Do a performance test");
	COMMAND('s', 'n', ServByName, "name",		"Convert service name to port number");
	COMMAND('s', 'p', ServByPort, "port",		"Convert port number to service name");
	COMMAND('s', 'l', ServList, "",				"List services");
	COMMAND('n', 'd', NoDelay, "",				"Turn on NODELAY TCP option");
	COMMAND('l', 'g', Linger, "",				"Set LINGER socket option");
	COMMAND('g', 's', Getsockname, "",			"Get socket name");
	COMMAND('g', 'p', Getsockname, "",			"Get peer name");
	
	AddSocketCommands();
	
	RunTest(argc, argv);
	CleanupSockets();
}
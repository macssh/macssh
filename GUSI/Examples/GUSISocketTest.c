/*********************************************************************
File		:	GUSI					-	Grand Unified Socket Interface
File		:	GUSISocketTest.c	-	Socket testing gear
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:39  chombier
Initial import

Revision 1.6  2000/05/23 06:41:40  neeri
Separate out descriptor commands

Revision 1.5  1999/12/13 03:07:24  neeri
Releasing 2.0.2

Revision 1.4  1999/11/15 07:52:30  neeri
Last Call for 2.0.1

Revision 1.3  1999/08/02 06:59:24  neeri
Added support for asynchronous errors

Revision 1.2  1999/04/10 04:25:54  neeri
Change to more standard compliant headers

Revision 1.1  1998/10/25 11:57:27  neeri
Ready to release 2.0a3

Revision 1.2  1994/12/31  01:12:21  neeri
ANSIfy.
Roundtrip benchmark.

Revision 1.1  1994/02/25  02:47:47  neeri
Initial revision

Revision 0.1  1992/09/08  00:00:00  neeri
Factor out more common code

*********************************************************************/

#include "GUSITest.h"
#include <Types.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <LowMem.h>

int	sock		=	-1;
int	accsock	=	-1;

void Close(char ch1, char ch2, const char * cmd)
{
	if (close(sock))	{
		printf("# close() returned error %s\n", Explain());
		Where();
	}
	
	sock		=	accsock;
	accsock	=	-1;
}

void Listen(char ch1, char ch2, const char * cmd)
{
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
		
		return;
	}

	if (listen(sock, 5))	{
		printf("# listen() returned error %s\n", Explain());
		Where();
	}
}

void Write(char ch1, char ch2, const char * line)
{
	int len	=	strlen(line);
	int part;

	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}
	
	for (; len; len -= part, line += part) {
		part = write(sock, line, len);
		if (part < 0)	{
			printf("# write(\"%s\") returned error %s\n", line, Explain());
			Where();
			
			break;
		}		
	}
}

void Read(char ch1, char ch2, const char * cmd)
{
	int		len;
	char 		buf[500];
	char *	line;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}

	len	=	read(sock, buf, 500);
	
	if (len < 0)	{
		printf("# read() returned error %s\n", Explain());
	} else {
		buf[len] = 0;
		printf("# read() returned:\n");
		
		for (line = strtok(buf, "\n\r"); line; line = strtok(nil, "\n\r"))
			printf("# %s\n", line);
	}
	
	Where();
}

void Select(char ch1, char ch2, const char * cmd)
{
	int				res;
	fd_set			rdfds;
	fd_set			wrfds;
	fd_set			exfds;
	struct timeval	delay;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}
	
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
		printf("# select() returned error %s\n", Explain());
	} else if (!res) {
		printf("# select() timed out\n");
	} else {
		printf(
			"# select() returned %s%s%s\n", 
			FD_ISSET(sock, &rdfds) ? "canRead " : "",
			FD_ISSET(sock, &wrfds) ? "canWrite " : "",
			FD_ISSET(sock, &exfds) ? "exception " : "");
	}
	
	Where();
}

void TogBlk(char ch1, char ch2, const char * cmd)
{
	int	block;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}
	
	switch (fcntl(sock, F_GETFL, 0))	{
	case 0:
		block	=	1;
		break;
	default:
		block = 	0;
		break;
	case -1:
		printf("# fcntl(F_GETFL) returned error %s\n", Explain());
		Where();
		
		return;
	}

	if (fcntl(sock, F_SETFL, block ? O_NONBLOCK : 0))
		printf(
			"# ioctl(FIONBIO, %s) returned error %s\n", 
			block ? "true" : "false", 
			Explain());
	else
		printf("# Socket is now %s\n", block ? "nonblocking" : "blocking");
	
	Where();
}

void NRead(char ch1, char ch2, const char * cmd)
{
	int	nread;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}
	
	if (ioctl(sock, FIONREAD, (long *) &nread))
		printf("# ioctl(FIONREAD) returned error %s\n", Explain());
	else
		printf("# %d bytes waiting to be read\n", nread);
	
	Where();
}

void GetError(char ch1, char ch2, const char * cmd)
{
	int err;
	socklen_t len = sizeof(int);
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}
	
	if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len))
		printf("# getsockopt(SOL_SOCKET, SO_ERROR) returned error %s\n", Explain());
	else {
		errno = err;
		printf("# Asynchronous error was %d (%s)\n", err, Explain());
	}
	
	Where();
}

static char BenchBuf[8192];

void BenchSer(char ch1, char ch2, const char * cmd)
{
	char *	at;
	int		requestSize;
	int		responseSize;
	int		packetSize;
	int		count;
	int		i;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}
	
	at = BenchBuf;
	
	do {
		at += read(sock, at, 1024);
		*at = 0;
	} while (!strchr(BenchBuf, '\015'));
	
	sscanf(BenchBuf, "%d %d %d\015", &requestSize, &responseSize, &count);
	
	write(sock, BenchBuf, 1);
	
	for (i=0; i++<count; ) {
		packetSize = 0;
		do { 
			packetSize += read(sock, BenchBuf, 8192);
		} while (packetSize < requestSize);
		write(sock, BenchBuf, responseSize);
	}
}

void BenchCli(char ch1, char ch2, const char * cmd)
{
	int		requestSize;
	int		responseSize;
	int		packetSize;
	int		contribSize;
	int		count;
	int		i;
	long 	startTime;
	long	transTime;
	Sampler	readSamp;
	Sampler	writeSamp;
	Sampler	sizeSamp;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}

	if (sscanf(cmd, "%d %d %d", &requestSize, &responseSize, &count) != 3) {
		Usage(ch1, ch2);
		return;
	}
	
	InitSampler(&readSamp);
	InitSampler(&writeSamp);
	InitSampler(&sizeSamp);
	
	write(sock, cmd, strlen(cmd)-1);
	write(sock, "\015", 1);
	read(sock, BenchBuf, 1);
	
	startTime = LMGetTicks();
	
	for (i=0; i++<count; ) {
		transTime = LMGetTicks();
		write(sock, BenchBuf, requestSize);
		Sample(&writeSamp, LMGetTicks()-transTime);
		packetSize = 0;
		transTime = LMGetTicks();
		do { 
			contribSize = read(sock, BenchBuf, 8192);
			packetSize += contribSize;
			Sample(&sizeSamp, contribSize);
		} while (packetSize < responseSize);
		Sample(&readSamp, LMGetTicks()-transTime);
	}
	
	printf("# Test took %d ticks.\n", LMGetTicks() - startTime);
	printf("# Read  min: %2d max: %2d avg: %2.1f\n", 
		readSamp.min, readSamp.max, ((double)readSamp.sum) / readSamp.count);
	printf("# Size  min: %2d max: %2d avg: %2.1f\n", 
		sizeSamp.min, sizeSamp.max, ((double)sizeSamp.sum) / sizeSamp.count);
	printf("# Write min: %2d max: %2d avg: %2.1f\n", 
		writeSamp.min, writeSamp.max, ((double)writeSamp.sum) / writeSamp.count);
}

void Shutdown(char ch1, char ch2, const char * cmd)
{
	int		what;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}

	if (sscanf(cmd, "%d", &what) != 1) {
		Usage(ch1, ch2);
		return;
	}
	
	if (shutdown(sock, what))
		printf("# shutdown(%d) returned error %s\n", what, Explain());
}

void AddDescriptorCommands()
{
	COMMAND('c', 'l', Close,  	"", 				"Close socket");
	COMMAND('w', 'r', Write, 	"text", 			"Write a line");
	COMMAND('r', 'd', Read,  	"", 				"Read");
	COMMAND('s', 'e', Select,  "", 				"Select a socket");
	COMMAND('t', 'b', TogBlk,  "", 				"Toggle blocking status");
	COMMAND('n', 'r', NRead,	"",				"Number of bytes to be read");
}

void AddSocketCommands()
{
	AddDescriptorCommands();
	COMMAND('l', 'i', Listen,  "", 				"Listen to socket");
	COMMAND('b', 's', BenchSer,"",				"Benchmark, server side");
	COMMAND('b', 'c', BenchCli,"reqSz resSz ct","Benchmark, client side");
	COMMAND('g', 'e', GetError,"",				"Get asynchronous error");
	COMMAND('s', 'd', Shutdown,"",				"Shutdown one side of socket");
}

void CleanupSockets()
{
	if (sock != -1)
		close(sock);
	if (accsock != -1)
		close(accsock);
}

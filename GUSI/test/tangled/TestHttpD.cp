// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
// % Project	:	GUSI				-	Grand Unified Socket Interface                    
// % File		:	TestHttpD.nw		-	A primitive HTTP daemon                       
// % Author	:	Matthias Neeracher                                           
// % Language	:	C++                                                        
// %                                                                       
// % $Log$
// % Revision 1.1.1.1  2001/03/03 21:51:12  chombier
// % Initial import
// %                                                
// % Revision 1.5  1999/06/08 04:31:32  neeri                              
// % Getting ready for 2.0b2                                               
// %                                                                       
// % Revision 1.4  1999/03/17 09:05:16  neeri                              
// % Added GUSITimer, expanded docs                                        
// %                                                                       
// % Revision 1.3  1998/10/25 11:57:42  neeri                              
// % Ready to release 2.0a3                                                
// %                                                                       
// % Revision 1.2  1998/10/11 16:45:28  neeri                              
// % Ready to release 2.0a2                                                
// %                                                                       
// % Revision 1.1  1998/08/01 21:32:14  neeri                              
// % About ready for 2.0a1                                                 
// %                                                                       
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
//                                                                         
// \documentstyle[noweb]{article}                                          
// \begin{document}                                                        
// \section{A primitive HTTP daemon}                                       
//                                                                         
// This program implements an extremely primitive HTTP daemon.             
//                                                                         
//                                                                         
// <TestHttpD.cp>=                                                         
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <iostream.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>

#include <GUSIDiag.h>

// The daemon runs until something is written to [[gDone]] and [[gPending]] is 0.
//                                                                         
// <Global state for [[TestHttpD]]>=                                       
int				gDone               = 0;
int				gPending            = 0;
pthread_mutex_t	gPendingMutex 		= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	gPendingCond		= PTHREAD_COND_INITIALIZER;
// All our replies use the same header                                     
//                                                                         
// <Global state for [[TestHttpD]]>=                                       
const char * gReplyHeader = 
	"HTTP/1.0 200 OK\015\012"
	"Content-Type: text/plain\015\012\015\012";
const char * gErrorHeader = 
	"HTTP/1.0 404 Not Found\015\012"
	"Content-Type: text/plain\015\012\015\012";
// <Auxiliary procedures for [[TestHttpD]]>=                               
void HandleCGI(int socket, char * request)
{
	if (!strcmp(request, "quit")) {
		write(socket, gReplyHeader, strlen(gReplyHeader));
		write(socket, "So long.", 8);
		pthread_mutex_lock(&gPendingMutex);
		write(gDone, "Game Over", 9);
		pthread_mutex_unlock(&gPendingMutex);
	} else if (!strcmp(request, "slow")) {
		write(socket, gReplyHeader, strlen(gReplyHeader));
		for (int i = 0; i<120; ++i) {
			char buf[20];
			sprintf(buf, "%d\015\012", i);
			write(socket, buf, strlen(buf));
			sleep(1);
		}
	}
}
// <Auxiliary procedures for [[TestHttpD]]>=                               
void HandleFile(int socket, char * request)
{
	for (char * slash = request; slash = strchr(slash, '/'); )
		*slash = ':';
	FILE * file = fopen(request, "r");
	FILE * sock = fdopen(socket, "w");
	
	if (file) {
		fputs(gReplyHeader, sock);
		// <Copy [[file]] to [[sock]] with EOL translation>=                       
  int    ch;

  while ((ch = fgetc(file)) != EOF) {
  	fputc(ch, sock);
  	if (ch == '\015') {
  		fputc('\012', sock);
  		if (ferror(sock))
  			break;
  	}
  }
	} else {
		fputs(gErrorHeader, sock);
		// <Report an error>=                                                      
  fputs("Yo! That URL is busted!\015\012", sock);
	}
	fclose(file);
	fclose(sock);
}
// <Request handler for [[TestHttpD]]>=                                    
extern "C" void * Handler(void * s)
{
	char buffer[1000];
	int  size  = 999;
	char * request;
	
	int		socket = reinterpret_cast<int>(s);
	int		sz;
	bool 	foundEnd = false;
	
	for (request = buffer; !foundEnd && (sz = read(socket, request, size)) > 0; ) {
		// <Translate CRLF and set [[foundEnd]] for end of request>=               
  char * in = request;
  char * out= request;
  bool   nl = false;
  while (sz--) {
  	switch (*in) {
  	case 0x0A:
  		if (out != request && out[-1] == 0x0D)
  			break; // Skip
  		*in = 0x0D;
  		// Fall through
  	case 0x0D:
  		if (nl) {
  			foundEnd = true;
  			sz = 0;
  		}
  		// Fall through
  	default:
  		nl = (*out++ = *in) == 0x0D;
  	}
  	++in;
  }
  sz = out-request;
		request += sz;
		size    -= sz;
	}
	
	// The technique employed here should definitely not be confused with a real
 // WWW server. All we're interested here is demonstrating the I/O library. 
 //                                                                         
 // <Isolate URL>=                                                          
 for (request = strchr(buffer, ' '); isspace(*++request); )
 	;
 *strchr(request, ' ') = 0;
	// We handle a few special URLs, the rest is translated as files.          
 //                                                                         
 // <Handle URL>=                                                           
 if (!strncmp(request, "/cgi/", 5)) 
 	HandleCGI(socket, request+5);
 else
 	HandleFile(socket, request);
 close(socket);
	
	pthread_mutex_lock(&gPendingMutex);
	--gPending;
	pthread_cond_signal(&gPendingCond);
	pthread_mutex_unlock(&gPendingMutex);
	
	return 0;
}
// <Main program for [[TestHttpD]]>=                                       
extern char * GUSI_diag_log;

void main()
{
	GUSI_diag_log = "TestHTTPd";
	// First we create a listener on port 80.                                  
 //                                                                         
 // <Create listener socket>=                                               
 sockaddr_in		addr;
 	
 addr.sin_family		=	AF_INET;
 addr.sin_addr.s_addr=	0;
 addr.sin_port		=	80;				

 int listener = socket(AF_INET, SOCK_STREAM, 0);

 bind(listener, (sockaddr *)&addr, sizeof(addr));
 listen(listener, 5);
	// To make it possible to wait on either a socket connection or a quit event, we
 // transmit the latter on a pipe. Cool, isn't it?                          
 //                                                                         
 // <Create completion pipe>=                                               
 int done[2];
 pipe(done);
 gDone = done[1];
	// The main event loop is a rather vanilla [[select]] based loop.          
 //                                                                         
 // <Handle events>=                                                        
 fd_set fd_interesting;
 FD_ZERO(&fd_interesting);
 FD_SET(done[0], &fd_interesting);
 FD_SET(listener, &fd_interesting);

 for (;;) {
 	fd_set fd = fd_interesting;
 	
 	if (select(32, &fd, NULL, NULL, NULL)) {
 		if (FD_ISSET(listener, &fd)) {
 			// Each incoming connection is immediately delegated to a new thread.      
    //                                                                         
    // <Create a new client>=                                                  
    int 		client = accept(listener, NULL, NULL);
    pthread_t	thread;

    pthread_create(&thread, 0, Handler, (void *)client);
    pthread_detach(thread);

    pthread_mutex_lock(&gPendingMutex);
    ++gPending;
    pthread_mutex_unlock(&gPendingMutex);
 			// Looking up the IP number is quite slow, so we only do that after starting the 
    // thread.                                                                 
    //                                                                         
    // <Report connection>=                                                    
    sockaddr_in	peer;
    socklen_t	len = sizeof(sockaddr_in);

    if (getpeername(client, (sockaddr *)&peer, &len)) {
    	cerr << "getpeername failed: " << h_errno << endl;
    } else {
    	hostent * host = gethostbyaddr(&peer.sin_addr, sizeof(in_addr), AF_INET);
    	if (host)
    		GUSI_MESSAGE(("Connection from %s [%s]\n", host->h_name, inet_ntoa(peer.sin_addr)));
    	else
    		GUSI_MESSAGE(("Connection from %s\n", inet_ntoa(peer.sin_addr)));
    }
 		}
 		if (FD_ISSET(done[0], &fd)) 
 			break;
 	}
 }
	// Finally, we wait for [[gPending]] to reach zero again, at which point all
 // clients will have completed.                                            
 //                                                                         
 // <Wait for all clients to complete>=                                     
 pthread_mutex_lock(&gPendingMutex);
 while (gPending) 
 	pthread_cond_wait(&gPendingCond, &gPendingMutex);
 pthread_mutex_unlock(&gPendingMutex);
}

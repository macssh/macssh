// <GUSIInet.cp>=                                                          
#include "GUSIInternal.h"
#include "GUSIInet.h"
#include "GUSIMTInet.h"
#include "GUSIOpenTransport.h"
#include "GUSIOTInet.h"

#include <sys/types.h>
#include <sys/socket.h>

GUSISocketTypeRegistry gGUSIInetFactories(AF_INET, 8);

// <Implementation of [[GUSIwithInetSockets]]>=                            
void GUSIwithInetSockets()
{
	if (GUSIOTFactory::Initialize())
		GUSIwithOTInetSockets();
	else
		GUSIwithMTInetSockets();
}

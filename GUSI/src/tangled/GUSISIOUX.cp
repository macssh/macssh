// <GUSISIOUX.cp>=                                                         
#include "GUSIInternal.h"
#include "GUSISIOUX.h"
#include "GUSIDevice.h"
#include "GUSIDescriptor.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"

#include <LowMem.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <console.h>

// \section{Definition of [[GUSISIOUXSocket]]}                             
//                                                                         
// A [[GUSISIOUXSocket]] is another singleton class: There is only one SIOUX
// console per application.                                                
//                                                                         
//                                                                         
// <Declaration of class [[GUSISIOUXSocket]]>=                             
class GUSISIOUXSocket : public GUSISocket {
public:
	~GUSISIOUXSocket();
	
	// Some member functions are fairly trivial wrappers.                      
 //                                                                         
 // <Overridden member functions for class [[GUSISIOUXSocket]]>=            
 ssize_t	read(const GUSIScatterer & buffer);
 // <Overridden member functions for class [[GUSISIOUXSocket]]>=            
 ssize_t write(const GUSIGatherer & buffer);
 // <Overridden member functions for class [[GUSISIOUXSocket]]>=            
 virtual int	ioctl(unsigned int request, va_list arg);
 // Since we know we're running on a pseudodevice, we can pass on that fact.
 //                                                                         
 // <Overridden member functions for class [[GUSISIOUXSocket]]>=            
 virtual int	fstat(struct stat * buf);
 // And we also know we're a TTY.                                           
 //                                                                         
 // <Overridden member functions for class [[GUSISIOUXSocket]]>=            
 virtual int	isatty();
 // [[select]] requires a walk of the event queue.                          
 //                                                                         
 // <Overridden member functions for class [[GUSISIOUXSocket]]>=            
 bool select(bool * canRead, bool * canWrite, bool *);

	static GUSISIOUXSocket *	Instance();
private:
	static GUSISIOUXSocket *	sInstance;
	
	GUSISIOUXSocket();
};
// \section{Definition of [[GUSISIOUXDevice]]}                             
//                                                                         
// A [[GUSISIOUXDevice]] is a singleton class supporting the standard [[open]]
// operation.                                                              
//                                                                         
// <Declaration of class [[GUSISIOUXDevice]]>=                             
class GUSISIOUXDevice : public GUSIDevice {
public:
	static GUSISIOUXDevice *	Instance();

	// [[GUSISIOUXDevice]] is prepared to handle an [[open]] on a limited set of names.
 //                                                                         
 // <Overridden member functions for class [[GUSISIOUXDevice]]>=            
 virtual bool Want(GUSIFileToken & file);
 // [[open]] returns the sole instance of [[GUSISIOUXSocket]].              
 //                                                                         
 // <Overridden member functions for class [[GUSISIOUXDevice]]>=            
 virtual GUSISocket * open(GUSIFileToken &, int flags);
private:
	GUSISIOUXDevice() 								{}
	
	static GUSISIOUXDevice *	sInstance;
};
// \section{Implementation of [[GUSISIOUXSocket]]}                         
//                                                                         
// A [[GUSISIOUXSocket]] is a dataless wrapper for the SIOUX               
// library. To protect the sole instance from ever being deleted           
// again, we artificially raise its reference count.                       
//                                                                         
// <Member functions for class [[GUSISIOUXSocket]]>=                       
GUSISIOUXSocket * GUSISIOUXSocket::sInstance;

GUSISIOUXSocket * GUSISIOUXSocket::Instance()
{
	if (!sInstance)
		if (sInstance = new GUSISIOUXSocket)
			sInstance->AddReference();

	return sInstance;
}
// On construction, we make sure to install the SIOUX event handler for all
// relevant elements.                                                      
//                                                                         
// <Member functions for class [[GUSISIOUXSocket]]>=                       
// This declaration lies about the return type
extern "C" void SIOUXHandleOneEvent(EventRecord *userevent);

GUSISIOUXSocket::GUSISIOUXSocket() 
{
	InstallConsole(0);
	GUSISetHook(GUSI_EventHook+nullEvent, 	(GUSIHook)SIOUXHandleOneEvent);
	GUSISetHook(GUSI_EventHook+mouseDown, 	(GUSIHook)SIOUXHandleOneEvent);
	GUSISetHook(GUSI_EventHook+mouseUp, 	(GUSIHook)SIOUXHandleOneEvent);
	GUSISetHook(GUSI_EventHook+updateEvt, 	(GUSIHook)SIOUXHandleOneEvent);
	GUSISetHook(GUSI_EventHook+diskEvt, 	(GUSIHook)SIOUXHandleOneEvent);
	GUSISetHook(GUSI_EventHook+activateEvt, (GUSIHook)SIOUXHandleOneEvent);
	GUSISetHook(GUSI_EventHook+osEvt, 		(GUSIHook)SIOUXHandleOneEvent);
}
GUSISIOUXSocket::~GUSISIOUXSocket()
{
	RemoveConsole();
}
// <Member functions for class [[GUSISIOUXSocket]]>=                       
ssize_t GUSISIOUXSocket::read(const GUSIScatterer & buffer)
{
	GUSIStdioFlush();
	return buffer.SetLength(
		ReadCharsFromConsole((char *) buffer.Buffer(), (int)buffer.Length()));
}
// <Member functions for class [[GUSISIOUXSocket]]>=                       
ssize_t GUSISIOUXSocket::write(const GUSIGatherer & buffer)
{
	return WriteCharsToConsole((char *) buffer.Buffer(), (int)buffer.Length());
}
// <Member functions for class [[GUSISIOUXSocket]]>=                       
int GUSISIOUXSocket::ioctl(unsigned int request, va_list)
{
	switch (request)	{
	case FIOINTERACTIVE:
		return 0;
	default:
		return GUSISetPosixError(EOPNOTSUPP);
	}
}
// <Member functions for class [[GUSISIOUXSocket]]>=                       
int	GUSISIOUXSocket::fstat(struct stat * buf)
{
	GUSISocket::fstat(buf);
	buf->st_mode =	S_IFCHR | 0666;
	
	return 0;
}
// <Member functions for class [[GUSISIOUXSocket]]>=                       
int GUSISIOUXSocket::isatty()
{ 
	return 1;
}
// <Member functions for class [[GUSISIOUXSocket]]>=                       
static bool input_pending()
{
	QHdrPtr eventQueue = LMGetEventQueue();
	EvQElPtr element = (EvQElPtr)eventQueue->qHead;
	
	// now, count the number of pending keyDown events.
	while (element != nil) {
		if (element->evtQWhat == keyDown || element->evtQWhat == autoKey)
			return true;
		element = (EvQElPtr)element->qLink;
	}
	
	return false;
}

bool GUSISIOUXSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		if (*canRead = input_pending())
			cond = true;
	if (canWrite)
		cond = *canWrite = true;
		
	return cond;
}
// \section{Implementation of [[GUSISIOUXDevice]]}                         
//                                                                         
// [[GUSISIOUXDevice]] is a singleton class.                               
//                                                                         
// <Member functions for class [[GUSISIOUXDevice]]>=                       
GUSISIOUXDevice * GUSISIOUXDevice::sInstance;
// <Member functions for class [[GUSISIOUXDevice]]>=                       
GUSISIOUXDevice * GUSISIOUXDevice::Instance()
{
	if (!sInstance)
		sInstance = new GUSISIOUXDevice();
	return sInstance;
}
// <Member functions for class [[GUSISIOUXDevice]]>=                       
bool GUSISIOUXDevice::Want(GUSIFileToken & file)
{
	switch (file.WhichRequest()) {
	case GUSIFileToken::kWillOpen:
		return file.IsDevice() && (file.StrStdStream(file.Path()) > -1);
	default:
		return false;
	}
}
// <Member functions for class [[GUSISIOUXDevice]]>=                       
GUSISocket * GUSISIOUXDevice::open(GUSIFileToken &, int)
{
	return GUSISIOUXSocket::Instance();
}
// A SIOUX application connects to the three standard descriptors on startup.
//                                                                         
// <SIOUX implementation of [[GUSISetupConsoleDescriptors]]>=              
void GUSISetupConsoleDescriptors()
{
	GUSIDescriptorTable * table = GUSIDescriptorTable::Instance();
	GUSISIOUXSocket *     SIOUX = GUSISIOUXSocket::Instance();
	
	table->InstallSocket(SIOUX);
	table->InstallSocket(SIOUX);
	table->InstallSocket(SIOUX);
}

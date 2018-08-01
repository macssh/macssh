/*
 * GUSITTY.cp
 * (c) 2000 Jean-Pierre Stierlin.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "GUSIInternal.h"
#include "GUSITTY.h"
#include "GUSIDevice.h"
#include "GUSIDescriptor.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"

#include <LowMem.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>


class GUSITTYDevice : public GUSIDevice {
public:
	static GUSITTYDevice *	Instance();

	// [[GUSITTYDevice]] is prepared to handle an [[open]] on a limited set of names.
 //                                                                         
 // <Overridden member functions for class [[GUSITTYDevice]]>=            
 virtual bool Want(GUSIFileToken & file);
 // [[open]] returns instance of [[GUSITTYSocket]].              
 //                                                                         
 // <Overridden member functions for class [[GUSITTYDevice]]>=            
 virtual GUSISocket * open(GUSIFileToken &, int flags);
private:
	GUSITTYDevice() {}

	static GUSITTYDevice *	sInstance;
};



class GUSITTYSocket : public GUSISocket {
public:
	~GUSITTYSocket();
	
	// Some member functions are fairly trivial wrappers.                      
 //                                                                         
 // <Overridden member functions for class [[GUSITTYSocket]]>=            
 ssize_t	read(const GUSIScatterer & buffer);
 // <Overridden member functions for class [[GUSITTYSocket]]>=            
 ssize_t write(const GUSIGatherer & buffer);
 // <Overridden member functions for class [[GUSITTYSocket]]>=            
 virtual int	ioctl(unsigned int request, va_list arg);
 // Since we know we're running on a pseudodevice, we can pass on that fact.
 //                                                                         
 // <Overridden member functions for class [[GUSITTYSocket]]>=            
 virtual int	fstat(struct stat * buf);
 // And we also know we're a TTY.                                           
 //                                                                         
 // <Overridden member functions for class [[GUSITTYSocket]]>=            
 virtual int	isatty();
 // [[select]] requires a walk of the event queue.                          
 //                                                                         
 // <Overridden member functions for class [[GUSITTYSocket]]>=            
 virtual bool pre_select(bool wantRead, bool wantWrite, bool wantExcept);
 virtual bool select(bool * canRead, bool * canWrite, bool * exception);
 virtual void post_select(bool wantRead, bool wantWrite, bool wantExcept);

protected:
	GUSITTYSocket(int id, void *context);

  int mId;
  void *mContext;

 friend class GUSITTYDevice;
};



/*
 * GUSIwithTTYSockets
 */
extern "C" void GUSIwithTTYSockets()
{
	GUSIDeviceRegistry::Instance()->AddDevice(GUSITTYDevice::Instance());
}

/*
 * GUSIwithoutTTYSockets
 */
extern "C" void GUSIwithoutTTYSockets()
{
	GUSIDeviceRegistry::Instance()->RemoveDevice(GUSITTYDevice::Instance());
}

/*
 * GUSIWakeupTTYSocket
 */
extern "C" void GUSIWakeupTTYSocket( void *insock )
{
	GUSISocket *sock = (GUSISocket *)insock;
	sock->Wakeup();
}


/*
 * GUSITTYDevice::Instance
 */

GUSITTYDevice * GUSITTYDevice::sInstance;

GUSITTYDevice * GUSITTYDevice::Instance()
{
	if (!sInstance)
		sInstance = new GUSITTYDevice();
	return sInstance;
}

/*
 * GUSITTYDevice::Want
 */
bool GUSITTYDevice::Want(GUSIFileToken &file)
{
	const char *path = file.Path();

	switch (file.WhichRequest()) {
	case GUSIFileToken::kWillOpen:
		return file.IsDevice() && file.StrFragEqual(path+4, "tty")
		 && (!path[7]
		  || (file.StrFragEqual(path+7, "in") && !path[9])
		  || (file.StrFragEqual(path+7, "out") && !path[10])
		  || (file.StrFragEqual(path+7, "err") && !path[10]));
	default:
		return false;
	}
}

/*
 * GUSITTYDevice::open
 */
GUSISocket * GUSITTYDevice::open(GUSIFileToken &file, int flags)
{
	GUSISocket *sock;
	const char *path = file.Path();
	int        id;

	if (!path[7]) {
		id = 3;
	} else if (!path[9]) {
		id = 0;
	} else if (file.StrFragEqual(path+7, "out")) {
		id = 1;
	} else /*if (file.StrFragEqual(path+7, "err"))*/ {
		id = 2;
	}
	if ( !(sock = new GUSITTYSocket(id, pthread_getspecific(ssh2threadkey)) )) {
		GUSISetPosixError(ENOMEM);
	}
	return sock;
}



/*
 * GUSITTYSocket::GUSITTYSocket
 */
GUSITTYSocket::GUSITTYSocket(int id, void *context) : mId(id), mContext(context)
{
	InstallTTY(id, context, this);
}


/*
 * GUSITTYSocket::~GUSITTYSocket
 */
GUSITTYSocket::~GUSITTYSocket()
{
	RemoveTTY(mId, mContext, this);
}

/*
 * GUSITTYSocket::read
 */
ssize_t GUSITTYSocket::read(const GUSIScatterer & buffer)
{
	/* FIXME: flush pending output */
	/*FlushTTY(mId);*/
	return buffer.SetLength(
		ReadCharsFromTTY(mId, mContext, (char *) buffer.Buffer(), (int)buffer.Length()));
}

/*
 * GUSITTYSocket::write
 */
ssize_t GUSITTYSocket::write(const GUSIGatherer & buffer)
{
	return WriteCharsToTTY(mId, mContext, (char *) buffer.Buffer(), (int)buffer.Length());
}

/*
 * GUSITTYSocket::ioctl
 */
int GUSITTYSocket::ioctl(unsigned int request, va_list)
{
	switch (request)	{
	case FIOINTERACTIVE:
		return 0;
	default:
		return GUSISetPosixError(EOPNOTSUPP);
	}
}

/*
 * GUSITTYSocket::fstat
 */
int	GUSITTYSocket::fstat(struct stat * buf)
{
	GUSISocket::fstat(buf);
	buf->st_mode =	S_IFCHR | 0666;
	
	return 0;
}

/*
 * GUSITTYSocket::isatty
 */
int GUSITTYSocket::isatty()
{ 
	return 1;
}

extern "C" int get_context_listener();

/*
 * GUSITTYSocket::pre_select
 */
bool GUSITTYSocket::pre_select(bool wantRead, bool wantWrite, bool wantExcept)
{
	/* we can't sleep when in pseudo-port forwarding mode */
	/* because the select functions polls for the connection */
	if (get_context_listener() == -1) {
		return GUSISocket::pre_select(wantRead, wantWrite, wantExcept);
	}
	return false;
}

/*
 * GUSITTYSocket::post_select
 */
void GUSITTYSocket::post_select(bool wantRead, bool wantWrite, bool wantExcept)
{
	if (get_context_listener() == -1) {
		GUSISocket::post_select(wantRead, wantWrite, wantExcept);
	}
}

/*
 * GUSITTYSocket::select
 */
bool GUSITTYSocket::select(bool * canRead, bool * canWrite, bool *)
{
	bool cond = false;
	if (canRead)
		if (*canRead = AvailableFromTTY(mId, mContext))
			cond = true;
	if (canWrite)
		cond = *canWrite = true;
		
	return cond;
}

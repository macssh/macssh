/*
 * GUSIPatch2.cp
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

#include <GUSIInternal.h>
#include <GUSINetDB.h>
#include <GUSIOTNetDB.h>

#include <pthread.h>

/*
 * we don't use the /etc/services files from prefs...
 */

class GUSIFileServiceDB : public GUSIServiceDB {
public:
	static GUSIFileServiceDB * Instance();
protected:
	FILE *			fFile;
	pthread_mutex_t	fLock;
	
	GUSIFileServiceDB(FILE * file) 
		: fFile(file), fLock(0) { }
	
	virtual void	Reset();
	virtual void 	Next();
};


GUSIFileServiceDB * GUSIFileServiceDB::Instance()
{
	return static_cast<GUSIFileServiceDB *>(nil);
}

/*
 * The fSvc field of the GUSIOTNetDB instance is no longer valid after
 * an interface switch in the TCP/IP control panel.
 * Let's clear it upon kOTProviderWillClose message.
 */

/* no longer needed: GUSI 2.1.6b2 fixes this */
/*
// <Asynchronous notifier function for [[GUSIOTNetDB]]>=                   
inline uint32_t CompleteMask(OTEventCode code)	
{ 	
	return 1 << (code & 0x1F); 
}

pascal void GUSIOTNetDBNotify(
	GUSIOTNetDB * netdb, OTEventCode code, OTResult result, void *cookie)
{
	GUSI_MESSAGE(("GUSIOTNetDBNotify %08x %d\n", code, result));
	GUSIContext *	context = netdb->fCreationContext;
	
	switch (code & 0x7F000000L) {
	case 0:
		netdb->fEvent |= code;
		result = 0;
		break;
	case kPRIVATEEVENT:
	case kCOMPLETEEVENT:
		if (!(code & 0x00FFFFE0))
			netdb->fCompletion |= CompleteMask(code);
		switch (code) {
		case T_OPENCOMPLETE:
			netdb->fSvc = static_cast<InetSvcRef>(cookie);
			break;
		case T_DNRSTRINGTOADDRCOMPLETE:
		case T_DNRADDRTONAMECOMPLETE:
			context = static_cast<GUSIContext **>(cookie)[-1];
			break;
		}
		break;
	default:
		if (code != kOTProviderWillClose) {
			result = 0;
		} else {
			netdb->fCreationContext = static_cast<GUSIContext *>(NULL);
		}
		break;
	}
	if (result)
		netdb->fAsyncError = result;
	context->Wakeup();
}

// <Member functions for class [[GUSIOTNetDB]]>=                           
bool GUSIOTNetDB::Resolver()
{
	if (!fCreationContext) {
		fCreationContext = GUSIContext::Current();
		if (!GUSIOTFactory::Initialize())
			return false;
		if (fSvc) {
			OTCloseProvider(fSvc);
			fSvc = static_cast<InetSvcRef>(NULL);
		}
		fAsyncError = 0;
		OSStatus syncError = OTAsyncOpenInternetServices(
			kDefaultInternetServicesPath, 
			0,
			reinterpret_cast<OTNotifyProcPtr>(GUSIOTNetDBNotify),
			this);
		if (!syncError)
			while (!fAsyncError && !(fCompletion & CompleteMask(T_OPENCOMPLETE)))
				GUSIContext::Yield(kGUSIBlock);
	}
	return fSvc != 0;
}
*/

// <GUSIFactory.cp>=                                                       
#include "GUSIInternal.h"
#include "GUSIFactory.h"
#include "GUSIDiag.h"
#include "GUSIInet.h"

// \section{Implementation of [[GUSISocketFactory]]}                       
//                                                                         
// We define this so compilers know where to place the vtable. Furthermore, very few
// domains provide a definition for [[socketpair]], while defining [[socket]] is
// pretty much mandatory.                                                  
//                                                                         
// <Member functions for class [[GUSISocketFactory]]>=                     
int GUSISocketFactory::socketpair(int, int, int, GUSISocket * [2])
{
	return GUSISetPosixError(EOPNOTSUPP);
}
// <Member functions for class [[GUSISocketDomainRegistry]]>=              
GUSISocketDomainRegistry *	GUSISocketDomainRegistry::sInstance;
// <Member functions for class [[GUSISocketDomainRegistry]]>=              
GUSISocketDomainRegistry::GUSISocketDomainRegistry()
{
	for (int i = 0; i<AF_MAX; ++i)
		factory[i] = nil;
}
// The [[socket]] call is swiftly delegated to the registered domain.      
//                                                                         
// <Member functions for class [[GUSISocketDomainRegistry]]>=              
GUSISocket * GUSISocketDomainRegistry::socket(int domain, int type, int protocol)
{
	if (!GUSI_CASSERT_CLIENT(domain >= 0 && domain < AF_MAX) || !factory[domain])
		return GUSISetPosixError(EAFNOSUPPORT), static_cast<GUSISocket *>(nil);
	return factory[domain]->socket(domain, type, protocol);
}
// So is [[socketpair]].                                                   
//                                                                         
// <Member functions for class [[GUSISocketDomainRegistry]]>=              
int GUSISocketDomainRegistry::socketpair(int domain, int type, int protocol, GUSISocket * s[2])
{
	if (!GUSI_CASSERT_CLIENT(domain >= 0 && domain < AF_MAX) || !factory[domain])
		return GUSISetPosixError(EAFNOSUPPORT);
	return factory[domain]->socketpair(domain, type, protocol, s);
}
// [[AddFactory]] and [[RemoveFactory]] add and remove a [[GUSISocketFactory]] 
// for a given domain number. Both return the previous registrant. Out of range
// errors are considered internal errors rather than merely client errors. 
//                                                                         
// <Member functions for class [[GUSISocketDomainRegistry]]>=              
GUSISocketFactory * GUSISocketDomainRegistry::AddFactory(int domain, GUSISocketFactory * f)
{
	if (!GUSI_CASSERT_INTERNAL(domain >= 0 && domain < AF_MAX))
		return nil;
	GUSISocketFactory * old = factory[domain];
	factory[domain] = f;
	
	return old;
}
// <Member functions for class [[GUSISocketTypeRegistry]]>=                
void GUSISocketTypeRegistry::Initialize()
{
	if (!factory) {
		factory = new Entry[maxfactory];
		GUSISocketDomainRegistry::Instance()->AddFactory(domain, this);
	}
}
// <Member functions for class [[GUSISocketTypeRegistry]]>=                
bool GUSISocketTypeRegistry::Find(int type, int protocol, bool exact, Entry *&found)
{
	Initialize();
	for (Entry * ent = factory; ent<factory+maxfactory; ++ent)
		if (!ent->factory) {
			found = ent;
			return false;
		} else if (
			// [[protocol]] may be specified as [[0]] in the call, but not [[type]].   
   //                                                                         
   // <Socket [[type]] matches [[ent->type]]>=                                
   (ent->type == type || (!exact && !ent->type))
		 && // <Socket [[protocol]] matches [[ent->protocol]]>=                        
      (ent->protocol == protocol || (!exact && (!ent->protocol || !protocol)))
		 ) {
			found = ent;
			return true;
		}
	found = nil;
	return false;
}
// The [[socket]] and [[socketpair]] calls are swiftly delegated to some registered 
// domain.                                                                 
//                                                                         
// <Member functions for class [[GUSISocketTypeRegistry]]>=                
GUSISocket * GUSISocketTypeRegistry::socket(int domain, int type, int protocol)
{
	Entry * ent;
	bool found = Find(type, protocol, false, ent);
	if (!GUSI_CASSERT_CLIENT(found))
		return GUSISetPosixError(EPROTONOSUPPORT), static_cast<GUSISocket *>(nil);
	return ent->factory->socket(domain, type, protocol);
}
 
int GUSISocketTypeRegistry::socketpair(int domain, int type, int protocol, GUSISocket * s[2])
{
	Entry * ent;
	bool found = Find(type, protocol, false, ent);
	if (!GUSI_CASSERT_CLIENT(found))
		return GUSISetPosixError(EPROTONOSUPPORT);
	return ent->factory->socketpair(domain, type, protocol, s);
}
// [[AddFactory]] and [[RemoveFactory]] add and remove a [[GUSISocketFactory]] 
// Both return the previous registrant. Table overflow errors are considered 
// internal errors rather than merely client errors.                       
//                                                                         
// <Member functions for class [[GUSISocketTypeRegistry]]>=                
GUSISocketFactory * GUSISocketTypeRegistry::AddFactory(int type, int protocol, GUSISocketFactory * f)
{
	Entry * ent;
	bool	previous = Find(type, protocol, true, ent);
	if (!GUSI_CASSERT_INTERNAL(ent))
		return nil;
	GUSISocketFactory * old = previous ? ent->factory : nil;
	ent->type		= type;
	ent->protocol	= protocol;
	ent->factory 	= f;
	
	return old;
}
// [[RemoveFactory]] has to take care of keeping all valid entries together.
//                                                                         
// <Member functions for class [[GUSISocketTypeRegistry]]>=                
GUSISocketFactory * GUSISocketTypeRegistry::RemoveFactory(int type, int protocol)
{
	Entry * ent;
	if (!Find(type, protocol, true, ent))
		return nil;
	GUSISocketFactory * old = ent->factory;
	while (++ent - factory < maxfactory && ent->factory)
		ent[-1] = ent[0];
	ent[-1].factory = nil;
	
	return old;
}

// <GUSIConfig.cp>=                                                        
#include "GUSIInternal.h"
#include "GUSIConfig.h"
#include "GUSIDiag.h"
#include "GUSIContext.h"
#include "GUSIBasics.h"
#include "GUSIFSWrappers.h"

#include <signal.h>
#include <sys/signal.h>

#include <PLStringFuncs.h>
#include <Resources.h>
#include <QuickDraw.h>
#include <LowMem.h>
#include <Script.h>

// The configuration resource format of GUSI is quite old by now and has grown 
// over the years. We have to be careful to keep it compatible across versions.
//                                                                         
// <Definition of struct [[GUSIConfigRsrc]]>=                              
struct GUSIConfigRsrc {
	OSType			fDefaultType;
	OSType			fDefaultCreator;
	
	bool			fAutoSpin;
	unsigned char	fFlags;
	
	OSType			fVersion;
	
	short							fNumSuffices;
	GUSIConfiguration::FileSuffix 	fSuffices[1];
};
// <Member functions for class [[GUSIConfiguration]]>=                     
GUSIConfiguration * GUSIConfiguration::sInstance = nil;
// The default implementation of [[GUSISetupConfig]] simply creates the instance.
// Feel free to override it.                                               
//                                                                         
// <Member functions for class [[GUSIConfiguration]]>=                     
//
// Prevent inlining to allow override
//
#ifdef __MWERKS__
#pragma dont_inline on
#endif

void GUSISetupConfig()
{
	GUSIConfiguration::CreateInstance();
}

#ifdef __MWERKS__
#pragma dont_inline reset
#endif
// The [[GUSIConfiguration]] constructor initializes all fields to reasonable defaults
// and then looks for a configuration resources, if one is specified.      
//                                                                         
// <Member functions for class [[GUSIConfiguration]]>=                     
GUSIConfiguration::GUSIConfiguration(short resourceID)
{
	// We show all fields with their three initialization methods tracks:      
 //                                                                         
 // \begin{itemize}                                                         
 // \item How they are initialized.                                         
 // \item How they are read from the configuration resource.                
 // \item How they are set via a member function.                           
 // \end{itemize}                                                           
 //                                                                         
 // After that, we show how they are used.                                  
 //                                                                         
 // [[ConfigureDefaultTypeCreator]] sets the default type and creator.      
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fDefaultType	=	'TEXT';
 fDefaultCreator	=	'MPS ';
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fNumSuffices 	= 	0;
 fSuffices   	= 	nil;
 fWeOwnSuffices	=	false;
 // [[ConfigureAutoSpin]] sets the autospin flag, which is enabled by default.
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fAutoSpin = true;
 // [[ConfigureAutoInitGraf]] controls automatic initialization of QuickDraw.
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fAutoInitGraf		= true;
 // [[ConfigureSigPipe]] controls whether [[SIGPIPE]] signals are generated.
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fSigPipe		= false;
 // [[ConfigureSigInt] controls whether [[SIGINT]] signals are generated.   
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fSigInt		= true;
 // [[fAccurateStat]] and [[fSharedOpen]] are tested from client code.      
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fAccurateStat		= false;
 fSharedOpen			= false;
 // [[ConfigureHandleAppleEvents]] is somewhat unusual in that it calls out into
 // client code instead of the other way around.                            
 //                                                                         
 // <Set defaults for [[GUSIConfiguration]] members>=                       
 fHandleAppleEvents 	= true;
	
	if (resourceID == kNoResource)
		return;
	
	GUSIConfigRsrc ** 	rsrc = 
		reinterpret_cast<GUSIConfigRsrc **>(Get1Resource('GU·I', resourceID));
	long				size = GetHandleSize(Handle(rsrc));
	
	if (!rsrc || !size)
		return;
		
	OSType version = '0102';
	if (size >= 14)
		version = rsrc[0]->fVersion;
		
	// <Read configuration resource into [[GUSIConfiguration]]>=               
 if (size >= 4 && rsrc[0]->fDefaultType)
 	fDefaultType = rsrc[0]->fDefaultType;
 if (size >= 8 && rsrc[0]->fDefaultCreator)
 	fDefaultType = rsrc[0]->fDefaultCreator;
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 if (version >= '0120' && size >= 16)
 	fNumSuffices = rsrc[0]->fNumSuffices;
 if (fNumSuffices && (fSuffices = new FileSuffix[fNumSuffices])) {
 	fWeOwnSuffices = true;
 	memcpy(fSuffices, rsrc[0]->fSuffices, fNumSuffices*sizeof(FileSuffix));
 	for (int i=0; i<fNumSuffices; i++)
 		for (int j=0; j<4; j++)
 			if (fSuffices[i].suffix[j] == ' ')
 				fSuffices[i].suffix[j] = 0;
 }
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 if (size >= 9)
 	fAutoSpin = (rsrc[0]->fAutoSpin != 0);
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 fAutoInitGraf		= version <  '0174' || (rsrc[0]->fFlags & 0x04) == 0;
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 fSigPipe		= version >= '0174' && (rsrc[0]->fFlags & 0x01) != 0;
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 fSigInt		= true;
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 fAccurateStat		= (rsrc[0]->fFlags & 0x80) != 0;
 fSharedOpen			= version >= '0174' && (rsrc[0]->fFlags & 0x02) != 0;
 // <Read configuration resource into [[GUSIConfiguration]]>=               
 ConfigureHandleAppleEvents(version < '0181' || (rsrc[0]->fFlags & 0x08) == 0);
}
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::ConfigureSuffices(short numSuffices, FileSuffix * suffices)
{
	if (fWeOwnSuffices) {
		delete fSuffices;
		fWeOwnSuffices = false;
	}
	fNumSuffices = numSuffices;
	fSuffices = suffices;
}
// [[SetDefaultFType]] assigns the preferred file type to a file.          
//                                                                         
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::SetDefaultFType(const GUSIFileSpec & name) const
{
	FInfo	info;	

	if (GUSIFSGetFInfo(&name, &info))
		return;
	info.fdType 	=	fDefaultType;
	info.fdCreator	=	fDefaultCreator;

	// <Try matching file suffix rules>=                                       
 Ptr dot = PLstrrchr(name->name, '.');
 if (dot && (name->name[0] - (dot-Ptr(name->name))) <= 4) {
 	char searchsuffix[5];
 	memset(searchsuffix, 0, 5);
 	strncpy(searchsuffix, dot+1, name->name[0] - (dot-Ptr(name->name)));
 	
 	for (int i = 0; i<fNumSuffices; i++)
 		if (!strncmp(fSuffices[i].suffix, searchsuffix, 4)) {
 			info.fdType 	=	fSuffices[i].suffType;
 			info.fdCreator	=	fSuffices[i].suffCreator;
 			
 			goto determined;
 		}
 }
determined:	
	GUSIFSSetFInfo(&name, &info);
}
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::DoAutoSpin() const
{
	GUSIContext::Yield(kGUSIYield);
}
// To make sure that A5 doesn't point into an overly wild location, we perform some sanity
// checks before getting to the point.                                     
//                                                                         
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::DoAutoInitGraf()
{
#if !TARGET_API_MAC_CARBON
	Ptr curA5 = LMGetCurrentA5();
	
	if (!(reinterpret_cast<long>(curA5) & 1) 
	 && curA5 > reinterpret_cast<Ptr>(ApplicationZone()) 
	 && curA5 < LMGetBufPtr()
	)
		if (*reinterpret_cast<GrafPtr **>(curA5) != &qd.thePort)
			InitGraf(&qd.thePort);
#endif
	fAutoInitGraf	=	false;	
}
// [[BrokenPipe]] raises a [[SIGPIPE]] signal if desired.                  
//                                                                         
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::BrokenPipe()
{
	if (fSigPipe)
		raise(SIGPIPE);
}
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::CheckInterrupt()
{
	if (fSigInt) {
#if !TARGET_API_MAC_CARBON
		EvQElPtr		eventQ;

		for (eventQ = (EvQElPtr) LMGetEventQueue()->qHead; eventQ; )
			if (CmdPeriod(reinterpret_cast<EventRecord *>(&eventQ->evtQWhat))) {
				raise(SIGINT);
				FlushEvents(-1, 0);
				break; 
			} else
				eventQ = (EvQElPtr)eventQ->qLink;
#else
		if ( CheckEventQueueForUserCancel() ) {
			raise(SIGINT);
			FlushEvents(-1, 0);
		}
#endif
	}
}
// Checking for the Command-Period key combination is rather complex. Our technique is copied
// straight from tech note 263.                                            
//                                                                         
// <Member functions for class [[GUSIConfiguration]]>=                     
bool GUSIConfiguration::CmdPeriod(const EventRecord * event)
{
	// <Constants for [[CmdPeriod]]>=                                          
 const long kMaskModifiers 	= 0xFE00;  		// we need the modifiers without the
                                    			// command key for KeyTrans
 const long kMaskVirtualKey 	= 0x0000FF00; 	// get virtual key from event message
                                    			// for KeyTrans
 const long kUpKeyMask      	= 0x0080;
 const long kShiftWord      	= 8;          	// we shift the virtual key to mask it
                                    			// into the keyCode for KeyTrans
 const long kMaskASCII1     	= 0x00FF0000; 	// get the key out of the ASCII1 byte
 const long kMaskASCII2     	= 0x000000FF; 	// get the key out of the ASCII2 byte
	
	if ((event->what == keyDown) || (event->what == autoKey)) {
		// see if the command key is down.  If it is, find out the ASCII
		// equivalent for the accompanying key.

		if (event->modifiers & cmdKey ) {
			// <Find ASCII equivalent of virtual key>=                                 
   short 	virtualKey 	= (event->message & kMaskVirtualKey) >> kShiftWord;
   short 	keyCode    	= (event->modifiers & kMaskModifiers) | virtualKey;
   UInt32	state      	= 0;
   Handle	hKCHR 		= nil; 
   Ptr		KCHRPtr 	= reinterpret_cast<Ptr>(GetScriptManagerVariable(smKCHRCache));
   if (!KCHRPtr) {
   	short script 	= GetScriptManagerVariable(smKeyScript);
   	short kcID 		= GetScriptVariable(script, smScriptKeys);
   	hKCHR   		= GetResource('KCHR', kcID);
   	KCHRPtr 		= *hKCHR;
   }
   short	keyInfo;
   if (KCHRPtr) {
   	keyInfo = KeyTranslate(KCHRPtr, keyCode, &state);
   	if (hKCHR)
   		ReleaseResource(hKCHR);
   } else
   	keyInfo = event->message;
			
			if ((keyInfo & kMaskASCII2) == '.' || (keyInfo & kMaskASCII1) == ('.' << 16))
				return true;
		}  // end the command key is down
	}  // end key down event	
	
	return false;
}
// <Member functions for class [[GUSIConfiguration]]>=                     
void GUSIConfiguration::ConfigureHandleAppleEvents(bool handleAppleEvents)
{
	if (fHandleAppleEvents != handleAppleEvents) {
		fHandleAppleEvents = handleAppleEvents;
		GUSISetHook(GUSI_EventHook+kHighLevelEvent, (GUSIHook) (fHandleAppleEvents ? 0 : -1));
	}
}

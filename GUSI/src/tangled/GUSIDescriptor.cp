// <GUSIDescriptor.cp>=                                                    
#include "GUSIInternal.h"
#include "GUSIDescriptor.h"
#include "GUSIBasics.h"
#include "GUSIDiag.h"
#include "GUSINull.h"

#include <errno.h>
#include <fcntl.h>
#include <utility>
#include <memory>

GUSI_USING_STD_NAMESPACE

// <Member functions for class [[GUSIDescriptorTable]]>=                   
GUSIDescriptorTable::GUSIDescriptorTable()
	: fInvalidDescriptor(0)
{
}
// <Member functions for class [[GUSIDescriptorTable]]>=                   
#ifdef __MRC__
#pragma noinline_func GUSISetupConsole, GUSISetupConsoleDescriptors, GUSISetupConsoleStdio
#pragma noinline_func GUSIStdioClose, GUSIStdioFlush, GUSISetupDescriptorTable
#endif
// <Member functions for class [[GUSIDescriptorTable]]>=                   
GUSIDescriptorTable	* GUSIDescriptorTable::sGUSIDescriptorTable;

// Application programmers may elect to override [[GUSISetupConsole]] to do their
// own initializations (notably to add further socket factories and devices), but 
// if they chose to do so, they generally should include a call to         
// [[GUSIDefaultSetupConsole]] in their version.                           
//                                                                         
// <Default implementation of [[GUSISetupConsole]]>=                       
//
// Prevent inlining to allow override
//
#ifdef __MWERKS__
#pragma dont_inline on
#endif

void GUSISetupConsole()
{
	GUSIDefaultSetupConsole();
}

#ifdef __MWERKS__
#pragma dont_inline reset
#endif
// The file descriptor part and the stdio part of the initialization functionality
// are usually handled in different GUSI sublibraries, therefore we separate them
// into different hooks. There never should be a reason to override        
// [[GUSIDefaultSetupConsole]] itself.                                     
//                                                                         
// <Default implementation of [[GUSISetupConsole]]>=                       
void GUSIDefaultSetupConsole()
{
	GUSISetupConsoleDescriptors();
	GUSISetupConsoleStdio();
}
// It may be necessary to override this function (notably for MPW support), but this
// version covers the basic case. To avoid any dependence on Null devices being
// installed in the device table, we call the instance directly.           
//                                                                         
// <Default implementation of [[GUSISetupConsole]]>=                       
//
// Prevent inlining to allow override
//
#ifdef __MWERKS__
#pragma dont_inline on
#endif

void GUSISetupConsoleDescriptors()
{
	GUSIDescriptorTable * table = GUSIDescriptorTable::Instance();
	GUSINullDevice *      null  = GUSINullDevice::Instance();
	
	if (open("dev:console", O_RDONLY) < 0)
		table->InstallSocket(null->open());
	if (open("dev:console", O_WRONLY) < 0)
		table->InstallSocket(null->open());
	if (open("dev:console", O_WRONLY) < 0)
		table->InstallSocket(null->open());
}

#ifdef __MWERKS__
#pragma dont_inline reset
#endif
// If, for any reason, the default descriptor table does not do the job for you, override
// [[GUSISetupDescriptorTable]].                                           
//                                                                         
// <Default implementation of [[GUSISetupDescriptorTable]]>=               
//
// Prevent inlining to allow override
//
#ifdef __MWERKS__
#pragma dont_inline on
#endif

void GUSISetupDescriptorTable()
{
}

#ifdef __MWERKS__
#pragma dont_inline reset
#endif

GUSIDescriptorTable * GUSIDescriptorTable::Instance()
{
	static bool sNeedConsoleSetup = true;
	
	if (!sGUSIDescriptorTable) {
		GUSISetupDescriptorTable();
		
		if (!sGUSIDescriptorTable)
			sGUSIDescriptorTable = new GUSIDescriptorTable();			
	}
	if (sNeedConsoleSetup) {
		sNeedConsoleSetup = false;
		GUSISetupConsole();
	}
	return sGUSIDescriptorTable;
}

void GUSIDescriptorTable::SetInstance(GUSIDescriptorTable * table)
{
	sGUSIDescriptorTable = table;
}

void GUSIDescriptorTable::CloseAllDescriptors()
{
	delete sGUSIDescriptorTable;
	sGUSIDescriptorTable	= nil;
}
// <Member functions for class [[GUSIDescriptorTable]]>=                   
//
// Prevent inlining to allow override
//
#ifdef __MWERKS__
#pragma dont_inline on
#endif

void GUSIStdioClose() { }
void GUSIStdioFlush() { }
void GUSISetupConsoleStdio() { }

#ifdef __MWERKS__
#pragma dont_inline reset
#endif

GUSIDescriptorTable::~GUSIDescriptorTable()
{
	GUSIStdioFlush();
	GUSIStdioClose();	
	
	for (iterator i = begin(); i != end(); ++i) 
		RemoveSocket(*i);
}
// To keep the range of descriptor slots to search as small as possible, only 
// descriptors smaller than [[fInvalidDescriptor]] are potentially valid.  
//                                                                         
// <Member functions for class [[GUSIDescriptorTable]]>=                   
int GUSIDescriptorTable::InstallSocket(GUSISocket * sock, int start)
{
	if (start<0 || start >= SIZE)
		return GUSISetPosixError(EINVAL);

	while (start<fInvalidDescriptor)
		if (!fSocket[start])
			goto found;
		else
			++start;
	while (start > fInvalidDescriptor)
		fSocket[fInvalidDescriptor++] = static_cast<GUSISocket *>(nil);
	if (start < SIZE)
		++fInvalidDescriptor;
	else
		return GUSISetPosixError(EMFILE);

found:
	fSocket[start] = sock;
		
	sock->AddReference();
	
	return start;
}
// <Member functions for class [[GUSIDescriptorTable]]>=                   
int GUSIDescriptorTable::RemoveSocket(int fd)
{
	GUSISocket *	sock;

	if (fd<0 || fd >= fInvalidDescriptor || !(sock = fSocket[fd]))
		return GUSISetPosixError(EBADF);

	fSocket[fd] 	=	nil;

	sock->RemoveReference();

	return 0;
}
// <Member functions for class [[GUSIDescriptorTable]]>=                   
GUSISocket * GUSIDescriptorTable::operator[](int fd)
{
	GUSISocket * sock;
	
	if (fd<0 || fd >= fInvalidDescriptor || !(sock = fSocket[fd]))
		return GUSISetPosixError(EBADF), static_cast<GUSISocket *>(nil);
	else
		return sock;
}
// <Member functions for class [[GUSIDescriptorTable]]>=                   
GUSISocket * GUSIDescriptorTable::LookupSocket(int fd)
{
	GUSIDescriptorTable * 	table = Instance();
	GUSISocket * 			sock;
	
	if (fd<0 || fd >= table->fInvalidDescriptor || !(sock = table->fSocket[fd]))
		return GUSISetPosixError(EBADF), static_cast<GUSISocket *>(nil);
	else
		return sock;
}
// The copy constructor has to increment the reference counts of all sockets in the table.
//                                                                         
// <Member functions for class [[GUSIDescriptorTable]]>=                   
GUSIDescriptorTable::GUSIDescriptorTable(const GUSIDescriptorTable & parent)
 : fInvalidDescriptor(parent.fInvalidDescriptor)
{
	memcpy(fSocket, parent.fSocket, fInvalidDescriptor*sizeof(GUSISocket *));

	iterator e = end();
	for (iterator i = begin(); i != e; ++i)
		if (GUSISocket * s = fSocket[*i])
			s->AddReference();
}

// <GUSIMSL.cp>=                                                           
#include "GUSIInternal.h"
#include "GUSIMSL.h"
#include "GUSIDescriptor.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>

GUSI_USING_STD_NAMESPACE

// <Prototypes for internal MSL variables>=                                
extern __file_modes __temp_file_mode;
extern "C" {
// <Prototypes for internal MSL functions>=                                
void __temp_file_name(char * name_str, void * fsspec);
// \section{Implementation of ANSI library specific public GUSI functions} 
//                                                                         
// [[fdopen]] is an operation inherently tied to the ANSI library used.    
//                                                                         
// <Prototypes for internal MSL functions>=                                
FILE * __find_unopened_file();
// __handle_reopen is declared in <stdio.h>
// \section{Implementation of ANSI library specific internal GUSI functions}
//                                                                         
// While there is a function [[__close_all]], it turns out that calling it is
// problematic, at least under CodeWarrior Pro 4, because it may fail      
// catastrophically upon being called a second time.                       
//                                                                         
// <Prototypes for internal MSL functions>=                                
void __flush_all(void);
// \section{Implementation of MSL override functions}                      
//                                                                         
// All opens from the ANSI C and C++ libraries eventually call [[__open_file]], so
// that's one of our most important override candidates. We translate the file mode
// and call [[open]].                                                      
//                                                                         
// <Prototypes for MSL override functions>=                                
int __open_file(const char * name, __file_modes mode, __file_handle * handle);
// [[__open_temp_file]] is used to open files that get deleted after they are closed.
// We simply use the underlying GUSI support for the POSIXish [[open() / unlink]]
// idiom.                                                                  
//                                                                         
// <Prototypes for MSL override functions>=                                
int __open_temp_file(__file_handle * handle);
// I/O operations in MSL lead to calls to [[__read_file]], [[__write_file]],
// [[__position_file]], and [[__close_file]], so we'll reimplement those.  
//                                                                         
// <Prototypes for MSL override functions>=                                
int __read_file(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc idle_proc);
// We treat the console variations almost identically to their file counterparts,
// except that we have to try to guess the correct handle.                 
//                                                                         
// <Prototypes for MSL override functions>=                                
int __read_console(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc idle_proc);
// <Prototypes for MSL override functions>=                                
int __write_file(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc idle_proc);
// <Prototypes for MSL override functions>=                                
int __write_console(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc idle_proc);
// The only one of these presenting us with some subtlety is [[__position_file]].
// Unless we catch the case of positioning at a place where the position already is,
// MSL will be unable to [[fdopen]] any sockets.                           
//                                                                         
// <Prototypes for MSL override functions>=                                
int __position_file(
	__file_handle handle, unsigned long * position, int mode, __idle_proc idle_proc);
// <Prototypes for MSL override functions>=                                
int __close_file(__file_handle handle);
// [[__close_console]] is an undecidable function unless you build with MPW 
// MSL libraries.                                                          
//                                                                         
// <Prototypes for MSL override functions>=                                
int __close_console(__file_handle handle);
}

// <Implementation of MSL override functions>=                             
int __open_file(const char * name, __file_modes mode, __file_handle * handle)
{
	int	fd;
	int	posixmode;
	
	// Translation of the constants is pretty obvious.                         
 //                                                                         
 // <Translate [[mode]] to [[posixmode]]>=                                  
 switch (mode.open_mode) {
 case __must_exist:
 	posixmode = 0;
 	break;
 case __create_if_necessary:
 	posixmode = O_CREAT;
 	break;
 case __create_or_truncate:
 	posixmode = O_CREAT | O_TRUNC;
 	break;
 }

 switch (mode.io_mode) {
 case __read:
 	posixmode |= O_RDONLY;
 	break;
 case __write:
 	posixmode |= O_WRONLY;
 	break;
 case __read_write:
 	posixmode |= O_RDWR;
 	break;
 case __write | __append:
 	posixmode |= O_WRONLY | O_APPEND;
 	break;
 case __read_write | __append:
 	posixmode |= O_RDWR | O_APPEND;
 	break;
 }
	
	fd = open(name, posixmode);
	
	if (fd == -1)
		return __io_error;
	
	*handle = (unsigned) fd;
	
	return __no_io_error;
}
// <Implementation of MSL override functions>=                             
int __open_temp_file(__file_handle * handle)
{
	char	temp_name[L_tmpnam];
	FSSpec	spec;
	int		ioresult;
	
	__temp_file_name(temp_name, &spec);

	ioresult = __open_file(temp_name, __temp_file_mode, handle);
	
	if (ioresult == __no_io_error)
	{
		unlink(temp_name);
	}
	
	return(ioresult);
}
// <Implementation of MSL override functions>=                             
int __read_file(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc)
{
	int result = read((int) handle, (char *) buffer, (int) *count);
	
	if (result < 0) {
		*count = 0;
		
		return __io_error;
	} else {
		*count = result;
		
		return result ? __no_io_error : __io_EOF;
	}
}
// <Implementation of MSL override functions>=                             
int __read_console(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc idle_proc)
{
	return __read_file(handle, buffer, count, idle_proc);
}
// <Implementation of MSL override functions>=                             
int __write_file(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc)
{
	int result = write((int) handle, (char *) buffer, (int) *count);
	
	if (result < 0) {
		*count = 0;
		
		return __io_error;
	} else {
		*count = result;
		
		return __no_io_error;
	}
}
// <Implementation of MSL override functions>=                             
static __file_handle GuessHandle(__file_handle handle, unsigned char * buffer)
{
	if (handle)
		return handle;
	if (buffer > stderr->buffer && (buffer - stderr->buffer) < stderr->buffer_size)
		return 2;
	return 1;
}

int __write_console(
	__file_handle handle, unsigned char * buffer, size_t * count, __idle_proc idle_proc)
{
	return __write_file(GuessHandle(handle, buffer), buffer, count, idle_proc);
}
// <Implementation of MSL override functions>=                             
int __position_file(
	__file_handle handle, unsigned long * position, int mode, __idle_proc)
{
	long result = lseek((int) handle, *position, mode);
	
	if (result < 0)
		if (errno == ESPIPE && !*position && mode != SEEK_END)
			*position = 0;
		else
			return __io_error;
	else
		*position = result;
	
	return __no_io_error;
}
// <Implementation of MSL override functions>=                             
int __close_file(__file_handle handle)
{
	return close((int) handle) < 0 ? __io_error : __no_io_error;
}
// <Implementation of MSL override functions>=                             
int __close_console(__file_handle handle)
{
	return __close_file(handle);
}
// <Implementation of ANSI library specific public GUSI functions>=        
#if __MSL__ >= 0x6000 && __MSL__ < 0x7001
#define fdopen _fdopen
#endif
#if defined(__MWERKS__) && __MWERKS__ < 0x2401 && !defined(_SFSTDIO_H)
FILE * fdopen(int fildes, char *type)
#else
FILE * fdopen(int fildes, const char *type)
#endif
{
	FILE			*str;

	if (!(str = __find_unopened_file()))
		return(0);
	
	return(__handle_reopen(fildes, type, str));
}
// <Implementation of ANSI library specific internal GUSI functions>=      
void GUSIStdioClose() { 				 	 	}
void GUSIStdioFlush() { __flush_all(); 			}
// <Implementation of ANSI library specific internal GUSI functions>=      
static void MSLSetupStdio(int fd, FILE * f)
{
	f->handle 			= fd;
	f->position_proc	= __position_file;
	f->read_proc 		= __read_file;
	f->write_proc 		= __write_file;
	f->close_proc 		= __close_file;
}

void GUSISetupConsoleStdio()
{
	MSLSetupStdio(0, stdin);
	MSLSetupStdio(1, stdout);
	MSLSetupStdio(2, stderr);
}

// <GUSIMPWStdio.cp>=                                                      
#include "GUSIInternal.h"
#include "GUSIDescriptor.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// \section{Implementation of MPW ANSI library specific public GUSI functions}
//                                                                         
// We have to reimplement a substantial portion of the MPW stdio library   
// because of shared library problems and incompatible macro definitions for
// [[open]] flags.                                                         
//                                                                         
// We'll start with a couple of internals. [[findfile]] searches for a free
// [[FILE]] structure.                                                     
//                                                                         
// <Implementation of MPW ANSI library specific public GUSI functions>=    
static FILE * findfile()
{
	FILE * 		stream;
	
	for (stream = _iob; stream < _iob+_NFILE; ++stream)
		if (!(stream->_flag & (_IOREAD | _IOWRT | _IORW)))
			return stream;
	
	return NULL;
}
// [[fdreopen]] initializes a [[FILE]] for use, which is actually quite simple.
//                                                                         
// <Implementation of MPW ANSI library specific public GUSI functions>=    
static FILE *fdreopen(int fd, short flags, FILE* stream)
{
	stream->_cnt 	= 0;
	stream->_ptr 	= NULL;
	stream->_base 	= NULL;
	stream->_end 	= NULL;
	stream->_size 	= NULL;
	stream->_flag 	= flags;
	stream->_file 	= fd;
	
	return stream;
}
// [[mode2flags]] parses the mode string and translates it to [[open]] and stdio
// flags.                                                                  
//                                                                         
// <Implementation of MPW ANSI library specific public GUSI functions>=    
static int mode2flags(const char * mode, int * openflags, int * stdioflags)
{
	bool	read_write 	= mode[1] == '+' || mode[2] == '+';
	
	*openflags	= 0;
	*stdioflags	= 0;
	
	switch (mode[0]) {
	case 'r':
		*openflags	|= read_write ? O_RDWR : O_RDONLY;
		*stdioflags	|= read_write ? _IORW : _IOREAD;
		break;
	case 'w':
		*openflags	|= (read_write ? O_RDWR : O_WRONLY) | O_CREAT | O_TRUNC;
		*stdioflags	|= read_write ? _IORW : _IOWRT;
		break;
	case 'a':
		*openflags	|= (read_write ? O_RDWR : O_WRONLY) | O_CREAT | O_APPEND;
		*stdioflags	|= read_write ? _IORW : _IOWRT;
		break;
	default:
		return -1;
	}
	
	return 0;
}
// [[fopen]], [[freopen]], and [[fdopen]] can then be composed from the above.
//                                                                         
// <Implementation of MPW ANSI library specific public GUSI functions>=    
FILE *fopen(const char *filename, const char *mode)
{
	FILE *	stream;
	int	 	flags;
	int		ioflags;
	int 	fd;
	
	if ((stream = findfile()) 
	 && mode2flags(mode, &flags, &ioflags) >= 0 
	 && (fd = open(filename, flags)) >= 0
	) 
		return fdreopen(fd, ioflags, stream);
	else
		return NULL;
}
// <Implementation of MPW ANSI library specific public GUSI functions>=    
FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
	int	 	flags;
	int		ioflags;
	int 	fd;
	
	flags = errno;
	fclose(stream);
	errno = flags;
	
	if (mode2flags(mode, &flags, &ioflags) >= 0 
	 && (fd = open(filename, flags)) >= 0
	) 
		return fdreopen(fd, ioflags, stream);
	else
		return NULL;
}
// <Implementation of MPW ANSI library specific public GUSI functions>=    
FILE *fdopen(int fd, const char *mode)
{
	FILE *	stream;
	int	 	flags;
	int		ioflags;

	if ((stream = findfile()) 
	 && mode2flags(mode, &flags, &ioflags) >= 0  
	)
		return fdreopen(fd, ioflags, stream);
	else
		return NULL;
}
// \section{Implementation of internal GUSI functions for MPW Stdio}       
//                                                                         
//                                                                         
// <Implementation of internal GUSI functions for MPW Stdio>=              
void GUSIStdioClose()
{
	for (FILE * f = _iob; f<_iob+_NFILE; ++f)
		if (f->_flag & (_IOREAD|_IOWRT))
			fclose(f);
}

void GUSIStdioFlush() { fflush(NULL);	}
// MPW Stdio already does everything to get started, no need for us to get in the 
// act. With MrC, however, we have to pay attention that ioctl does not get stripped,
// otherwise the library will call a bad version internally.               
//                                                                         
// <Implementation of internal GUSI functions for MPW Stdio>=              
static void * sDontStrip;

void GUSISetupConsoleStdio()
{
	sDontStrip = ioctl;
}

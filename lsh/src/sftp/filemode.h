#ifndef PARAMS
# if defined PROTOTYPES || (defined __STDC__ && __STDC__)
#  define PARAMS(Args) Args
# else
#  define PARAMS(Args) ()
# endif
#endif

#include <sys/types.h>
#include <sys/stat.h>

#define MODE_STRING_LENGTH 10
void mode_string PARAMS ((short unsigned int mode, char *str));
void filemodestring PARAMS ((struct stat *statp, char *str));

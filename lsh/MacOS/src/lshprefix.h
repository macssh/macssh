/* prefix.h */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define to support non-standard packets of ssh2 */
#define DATAFELLOWS_WORKAROUNDS 1

/* Define to enable sanity checking on memory allocation and casting */
#define DEBUG_ALLOC 0

/* Define to enable tracing */
#define DEBUG_TRACE 1

/* Define if the `getpgrp' function takes no argument.  */
/* #undef GETPGRP_VOID */

/* Define if AI_NUMERICHOST exists */
/* #undef HAVE_AI_NUMERICHOST */

/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if you have the <argp.h> header file.  */
/* #undef HAVE_ARGP_H */

/* Define if you have the cfmakeraw function.  */
/* #undef HAVE_CFMAKERAW */

/* Define if you have the <crypt.h> header file.  */
/* #undef HAVE_CRYPT_H */

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the gai_strerror function.  */
/* #undef HAVE_GAI_STRERROR */

/* Define if the compiler understands __attribute__ */
/* #undef HAVE_GCC_ATTRIBUTE */

/* Define if the compiler understands __FUNCTION__ */
/* #define HAVE_GCC_FUNCTION 1 */

/* Define if you have the getaddrinfo function.  */
/* #undef HAVE_GETADDRINFO */

/* Define if you have the getnameinfo function.  */
/* #undef HAVE_GETNAMEINFO */

/* Define if you have the getrusage function.  */
/* #undef HAVE_GETRUSAGE */

/* Define if you have the getspnam function.  */
/* #undef HAVE_GETSPNAM */

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the <gmp2/gmp.h> header file.  */
/* #undef HAVE_GMP2_GMP_H */

/* Define if you have the <gmp.h> header file.  */
#define HAVE_GMP_H 1

/* Define if you have the inet_aton function.  */
#define HAVE_INET_ATON 1

/* Define if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */

/* Define if you have the <krb5.h> header file.  */
/* #undef HAVE_KRB5_H */

/* Define if you have the crypt library (-lcrypt).  */
/* #undef HAVE_LIBCRYPT */

/* Define if you have the nsl library (-lnsl).  */
/* #undef HAVE_LIBNSL */

/* Define if you have the socket library (-lsocket).  */
/* #undef HAVE_LIBSOCKET */

/* Define if you have the util library (-lutil).  */
/* #undef HAVE_LIBUTIL */

/* Define if you have the <libutil.h> header file.  */
/* #undef HAVE_LIBUTIL_H */

/* Define if you have the `Xau' library (-lXau). */
/* #undef HAVE_LIBXAU */

/* Define if you have the `xnet' library (-lxnet). */
/* #undef HAVE_LIBXNET */

/* Define if you have the z library (-lz).  */
/* #undef HAVE_LIBZ */

/* Define if you have the login function.  */
/* #undef HAVE_LOGIN */

/* Define if you have the logout function.  */
/* #undef HAVE_LOGOUT */

/* Define if you have the logwtmp function.  */
/* #undef HAVE_LOGWTMP */

/* Define if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* Define if you have the memxor function.  */
/* #undef HAVE_MEMXOR */

/* Define if you have the openpty function.  */
/* #undef HAVE_OPENPTY */

/* Define if you have the poll function.  */
/* #undef HAVE_POLL */

/* Define if you have the <poll.h> header file.  */
/* #undef HAVE_POLL_H */

/* Define if you have the <pty.h> header file.  */
/* #undef HAVE_PTY_H */

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the <shadow.h> header file. */
/* #undef HAVE_SHADOW_H */

/* Define if you have the socket function.  */
#define HAVE_SOCKET 1

/* Define if you have the strerror function.  */
/* #undef HAVE_STDLIB_H */

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the <strings.h> header file.  */
/* #undef HAVE_STRINGS_H */

/* Define if you have the <string.h> header file.  */
/* #undef HAVE_STRING_H */

/* Define if you have the <stropts.h> header file.  */
/* #undef HAVE_STROPTS_H */

/* Define if you have the strsignal function.  */
#define HAVE_STRSIGNAL 1

/* Define if you have the strtol function.  */
#define HAVE_STRTOL 1

/* Define if `ut_addr' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_ADDR */

/* Define if `ut_addr_v6' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_ADDR_V6 */

/* Define if `ut_host' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_HOST */

/* Define if `ut_name' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_NAME */

/* Define if `ut_user' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_USER */

/* syslog() available? */
#define HAVE_SYSLOG 1

/* Define if you have the <syslog.h> header file.  */
/* #define HAVE_SYSLOG_H 1 */

/* Define if you have the <sys/poll.h> header file.  */
/* #undef HAVE_SYS_POLL_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if a we have working UNIX98 pty handling */
/* #undef HAVE_UNIX98_PTYS */

/* Define if you have the <utmpx.h> header file.  */
/* #undef HAVE_UTMPX_H */

/* Define if you have the <utmp.h> header file.  */
/* #undef HAVE_UTMP_H */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if you have the vsnprintf function.  */
#define HAVE_VSNPRINTF 1

/* Define if you have the <X11/Xauth.h> header file. */
/* #undef HAVE_X11_XAUTH_H */

/* Define if you have the <zlib.h> header file.  */
#define HAVE_ZLIB_H 1

/* Name of package */
#define PACKAGE "lsh"

/* The installation prefix. */
/* #undef PREFIX */

/* Traditional BSD pty handling */
/* #undef PTY_BSD_SCHEME */

/* Possible first characters in a /dev/ptyXX name */
/* #undef PTY_BSD_SCHEME_FIRST_CHARS */

/* Possible second characters in a /dev/ptyXX name */
/* #undef PTY_BSD_SCHEME_SECOND_CHARS */

/* sbin directory */
/* #undef SBINDIR */

/* Define to indicate that shutdown seems to work properly */
#define SHUTDOWN_WORKS_WITH_UNIX_SOCKETS 1

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* Location of the SSH1 daemon */
/* #undef SSHD1 */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
        STACK_DIRECTION > 0 => grows toward higher addresses
        STACK_DIRECTION < 0 => grows toward lower addresses
        STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Version number of package */
#define VERSION "1.3.4"

/* Define to enable authentication agent forwarding */
/* #undef WITH_AGENT_FORWARD */

/* Use gcov */
/* #undef WITH_GCOV */

/* Define if IDEA should be used */
/* #define WITH_IDEA 1 */

/* For Ipv6 support */
/* #undef WITH_IPV6 */

/* For kerberos */
/* #undef WITH_KERBEROS */

/* Define to enable pty support */
#define WITH_PTY_SUPPORT 1

/* Define if SRP should be supported */
#define WITH_SRP 1

/* Define to enable fallback to SSH1 */
/* #undef WITH_SSH1_FALLBACK */

/* Define to enable tcp forwarding */
#define WITH_TCP_FORWARD 1

/* For utmp support */
/* #undef WITH_UTMP */

/* Define to enable x11 forwarding */
#define WITH_X11_FORWARD 1

/* Define if zlib should be used */
#define WITH_ZLIB 1

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
#define WORDS_BIGENDIAN 1


/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define as __inline if that's what the C compiler calls it.  */
/* #undef inline */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Length type used by getsockopt */
/* #undef socklen_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */


/* MacOS Specific */

#define MACOS 1
#define LSH 1
/*#define HAVE_STDTTY_FILENO 1*/
/*#define STDTTY_FILENO 0*/
#define SIOUX_USE_WASTE 1

#define exit macosexit
#define abort macosabort

#define zlib_alloc ssh_xcalloc
#define zlib_free ssh_xfree

#define malloc lshmalloc
#define calloc lshcalloc
#define realloc lshrealloc
#define free lshfree

#include <sys/types.h>
#include <string.h>

#include "version.h"

/*
#if defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#endif
*/

extern char *strsignal(int signo);

/* bug in metrowerks 68k alloca function */
#ifndef powerc
#define ALLOCA_68K_BUG 1
//#if __MWERKS__
#if __MC68K__										/* begin mm 980512 */
#include <stddef.h>
extern void *__alloca(size_t size:__d0) : __d0;
#endif												/*  end  mm 980512 */
//#endif
#endif

#if ALLOCA_68K_BUG
inline asm long GetSP() {
	move.l sp,d0
}
#define ALLOCA_START(x) register long x = GetSP()
#define ALLOCA_FREE(x) asm { move.l x,sp }
#else
#define ALLOCA_START(x)
#define ALLOCA_FREE(x)
#endif

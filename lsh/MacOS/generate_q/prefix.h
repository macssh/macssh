/* prefix.h */

/* Define if using alloca.c.  */
#undef C_ALLOCA

/* Define to empty if the keyword does not work.  */
#undef const

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
#undef CRAY_STACKSEG_END

/* Define if the `getpgrp' function takes no argument.  */
#undef GETPGRP_VOID

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you have alloca, as a function or macro.  */
#undef HAVE_ALLOCA

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#undef HAVE_ALLOCA_H

/* Define if you don't have vprintf but do have _doprnt.  */
#undef HAVE_DOPRNT

/* Define if you have the vprintf function.  */
#undef HAVE_VPRINTF

/* Define as __inline if that's what the C compiler calls it.  */
#undef inline

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
 STACK_DIRECTION > 0 => grows toward higher addresses
 STACK_DIRECTION < 0 => grows toward lower addresses
 STACK_DIRECTION = 0 => direction of growth unknown
 */
#undef STACK_DIRECTION

/* Define if you have the ANSI C header files.  */
#undef STDC_HEADERS

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#undef TIME_WITH_SYS_TIME

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define to enable sanity checking on memory allocation and casting */
#undef DEBUG_ALLOC

/* Define to enable tracing */
#undef DEBUG_TRACE

/* Define if zlib should be used */
#undef WITH_ZLIB

/* Define to enable pty support */
#undef WITH_PTY_SUPPORT

/* Define to enable fallback to SSH1 */
#undef WITH_SSH1_FALLBACK

/* Location of the SSH1 daemon */
#undef SSHD1

/* Define if IDEA should be used */
#undef WITH_IDEA

/* Define if SRP should be supported */
#undef WITH_SRP

/* Define to enable tcp forwarding */
#undef WITH_TCP_FORWARD

/* For Ipv6 support */
#undef WITH_IPV6

/* Define if AI_NUMERICHOST exists */
#undef HAVE_AI_NUMERICHOST

/* For utmp support */
#undef WITH_UTMP

/* Define to indicate that shutdown seems to work properly */
#undef SHUTDOWN_WORKS_WITH_UNIX_SOCKETS

/* Define if the compiler understands __attribute__ */
#undef HAVE_GCC_ATTRIBUTE

/* Define if the compiler understands __FUNCTION__ */
#undef HAVE_GCC_FUNCTION

/* Define if the compiler understands offsetof(TYPE, MEMBER) */
#define HAVE_C_OFFSETOF 1

/* Define if a we have working UNIX98 pty handling */
#undef HAVE_UNIX98_PTYS

/* For BSD-style pty allocation */
#undef PTY_BSD_SCHEME_FIRST_CHARS
#undef PTY_BSD_SCHEME_SECOND_CHARS

#undef PTY_BSD_SCHEME

/* syslog() available? */
#undef HAVE_SYSLOG

/* struct utmp has a ut_host member */
#undef HAVE_UT_HOST

/* struct utmp has a ut_name member */
#undef HAVE_UT_NAME

/* struct utmp has a ut_user member */
#undef HAVE_UT_USER

/* struct utmp has a ut_addr member */
#undef HAVE_UT_ADDR

/* struct utmp has a ut_addr_v6 member */
#undef HAVE_UT_ADDR_V6

/* Length type used by getsockopt() */
/* #undef socklen_t */

/* Define to support non-standard packets of ssh2 */
#undef DATAFELLOWS_WORKAROUNDS

/* Global variables needed by argp */
#undef HAVE_PROGRAM_INVOCATION_NAME
#undef HAVE_PROGRAM_INVOCATION_SHORT_NAME

/* Define to enable x11 forwarding */
#undef WITH_X11_FORWARD

/* Define to enable authentication agent forwarding */
#undef WITH_AGENT_FORWARD

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* Define if you have the argp_parse function.  */
#undef HAVE_ARGP_PARSE

/* Define if you have the cfmakeraw function.  */
#undef HAVE_CFMAKERAW

/* Define if you have the flockfile function.  */
#undef HAVE_FLOCKFILE

/* Define if you have the fputs_unlocked function.  */
#undef HAVE_FPUTS_UNLOCKED

/* Define if you have the fwrite_unlocked function.  */
#undef HAVE_FWRITE_UNLOCKED

/* Define if you have the gai_strerror function.  */
#undef HAVE_GAI_STRERROR

/* Define if you have the getaddrinfo function.  */
#undef HAVE_GETADDRINFO

/* Define if you have the getnameinfo function.  */
#undef HAVE_GETNAMEINFO

/* Define if you have the getopt_long function.  */
#undef HAVE_GETOPT_LONG

/* Define if you have the getspnam function.  */
#undef HAVE_GETSPNAM

/* Define if you have the inet_aton function.  */
#undef HAVE_INET_ATON

/* Define if you have the login function.  */
#undef HAVE_LOGIN

/* Define if you have the logout function.  */
#undef HAVE_LOGOUT

/* Define if you have the logwtmp function.  */
#undef HAVE_LOGWTMP

/* Define if you have the mempcpy function.  */
#undef HAVE_MEMPCPY

/* Define if you have the memxor function.  */
#undef HAVE_MEMXOR

/* Define if you have the openpty function.  */
#undef HAVE_OPENPTY

/* Define if you have the poll function.  */
#undef HAVE_POLL

/* Define if you have the putc_unlocked function.  */
#undef HAVE_PUTC_UNLOCKED

/* Define if you have the select function.  */
#undef HAVE_SELECT

/* Define if you have the socket function.  */
#undef HAVE_SOCKET

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strndup function.  */
#undef HAVE_STRNDUP

/* Define if you have the strsignal function.  */
#define HAVE_STRSIGNAL 1
#define strsignal(x) (sys_signame[x])

/* Define if you have the strtol function.  */
#undef HAVE_STRTOL

/* Define if you have the syslog function.  */
#undef HAVE_SYSLOG

/* Define if you have the vsnprintf function.  */
#undef HAVE_VSNPRINTF

/* Define if you have the <argp.h> header file.  */
#undef HAVE_ARGP_H

/* Define if you have the <crypt.h> header file.  */
#undef HAVE_CRYPT_H

/* Define if you have the <fcntl.h> header file.  */
#undef HAVE_FCNTL_H

/* Define if you have the <gmp.h> header file.  */
#define HAVE_GMP_H 1

/* Define if you have the <gmp2/gmp.h> header file.  */
#undef HAVE_GMP2_GMP_H

/* Define if you have the <libutil.h> header file.  */
#undef HAVE_LIBUTIL_H

/* Define if you have the <poll.h> header file.  */
#undef HAVE_POLL_H

/* Define if you have the <pty.h> header file.  */
#undef HAVE_PTY_H

/* Define if you have the <shadow.h> header file.  */
#undef HAVE_SHADOW_H

/* Define if you have the <strings.h> header file.  */
#undef HAVE_STRINGS_H

/* Define if you have the <stropts.h> header file.  */
#undef HAVE_STROPTS_H

/* Define if you have the <sys/poll.h> header file.  */
#undef HAVE_SYS_POLL_H

/* Define if you have the <sys/time.h> header file.  */
#undef HAVE_SYS_TIME_H

/* Define if you have the <syslog.h> header file.  */
#undef HAVE_SYSLOG_H

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <utmp.h> header file.  */
#undef HAVE_UTMP_H

/* Define if you have the <utmpx.h> header file.  */
#undef HAVE_UTMPX_H

/* Define if you have the <zlib.h> header file.  */
#undef HAVE_ZLIB_H

/* Define if you have the crypt library (-lcrypt).  */
#undef HAVE_LIBCRYPT

/* Define if you have the nsl library (-lnsl).  */
#undef HAVE_LIBNSL

/* Define if you have the socket library (-lsocket).  */
#undef HAVE_LIBSOCKET

/* Define if you have the util library (-lutil).  */
#undef HAVE_LIBUTIL

/* Define if you have the xnet library (-lxnet).  */
#undef HAVE_LIBXNET

/* Define if you have the z library (-lz).  */
#undef HAVE_LIBZ

/* Name of package */
#define PACKAGE "lsh"

/* Version number of package */
#define VERSION "1.0"

/* MacOS Library */

#define MACOS 1

#define LSH 1


/* lsh_types.h
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Niels Möller
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

#ifndef LSH_TYPES_H_INCLUDED
#define LSH_TYPES_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>

#ifndef alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
#  define HAVE_ALLOCA 1
# else
#  if defined HAVE_ALLOCA_H || defined _LIBC
#   include <alloca.h>
#  else
#   ifdef _AIX
 #pragma alloca
#   else
#    ifndef alloca
char *alloca ();
#    endif
#   endif
#  endif
# endif
#endif

#if SIZEOF_SHORT >= 4
# define UINT32 unsigned short
#elif SIZEOF_INT >= 4
# define UINT32 unsigned int
#elif SIZEOF_LONG >= 4
# define UINT32 unsigned long
#else
# error No suitable type found to use for UINT32
#endif /* UINT32 */
 
#if SIZEOF_SHORT >= 2
# define UINT16 unsigned short
#elif SIZEOF_INT >= 2
# define UINT16 unsigned int
#else
# error No suitable type found to use for UINT16
#endif  /* UINT16 */
 
#define UINT8 unsigned char

#if __GNUC__ && HAVE_GCC_ATTRIBUTE
# define NORETURN __attribute__ ((__noreturn__))
# define PRINTF_STYLE(f, a) __attribute__ ((__format__ (__printf__, f, a)))
# define UNUSED __attribute__ ((__unused__))
#else
# define NORETURN
# define PRINTF_STYLE(f, a)
# define UNUSED
#endif

#if HAVE_GCC_FUNCTION
# define FUNCTION_NAME __FUNCTION__
#else
# define FUNCTION_NAME "Unknown"
#endif

/* Stringizing macros */
#define STRINGIZE1(x) #x
#define STRINGIZE(x) STRINGIZE1(x)
#define STRING_LINE STRINGIZE(__LINE__)

#if HAVE_STRERROR
#define STRERROR strerror
#else
#define STRERROR(x) (sys_errlist[x])
#endif

#if HAVE_STRSIGNAL
# define STRSIGNAL strsignal
#else /* !HAVE_STRSIGNAL */
# ifdef _sgi
#  define STRSIGNAL(x) (_sys_siglist[x])
# else /* !_sgi */
#  define STRSIGNAL(x) (sys_siglist[x])
# endif /* !_sgi */
#endif /* !HAVE_STRSIGNAL */

/* Some macros */

/* Reads a 32-bit integer, in network byte order */
#define READ_UINT32(p)				\
(  (((UINT32) (p)[0]) << 24)			\
 | (((UINT32) (p)[1]) << 16)			\
 | (((UINT32) (p)[2]) << 8)			\
 |  ((UINT32) (p)[3]))

#define WRITE_UINT32(p, i)			\
do {						\
  (p)[0] = ((i) >> 24) & 0xff;			\
  (p)[1] = ((i) >> 16) & 0xff;			\
  (p)[2] = ((i) >> 8) & 0xff;			\
  (p)[3] = (i) & 0xff;				\
} while(0)

/* Same, for 24-bit quantities */
#define READ_UINT24(p)				\
(  (((UINT32) (p)[0]) << 24)			\
 | (((UINT32) (p)[1]) << 16)			\
 |  ((UINT32) (p)[2]))

/* Same, for 16-bit quantities */
#define READ_UINT16(p)				\
(  (((UINT32) (p)[0]) << 8)			\
 |  ((UINT32) (p)[1]))

#define WRITE_UINT16(p, i)			\
do {						\
  (p)[0] = ((i) >> 8) & 0xff;			\
  (p)[1] = (i) & 0xff;				\
} while(0)

/* Useful macros. */
#define MIN(a, b) (((a)>(b)) ? (b) : (a))
#define MAX(a, b) (((a)<(b)) ? (b) : (a))
#define SQR(x) ((x)*(x))
     

#endif /* LSH_TYPES_H_INCLUDED */

/* werror.c
 *
 *
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

#include "lshprefix.h"
#include "lsh_context.h"

#include "werror.h"

#include "charset.h"
#include "format.h"  /* For format_size_in_decimal() */
#include "gc.h"
#include "io.h"
#include "parse.h"
#include "xalloc.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_SYSLOG_H
#include <syslog.h>
#endif

#ifdef MACOS
extern void abort();
#if !HAVE_SYSLOG_H
extern void openlog( const char *id, int flags, int type );
extern void syslog( int priority, const char *format, ... );
#define LOG_NOTICE 1
#define LOG_PID 1
#define LOG_CONS 2
#define LOG_DAEMON 1
#endif
#endif

/* Global flags */
#ifndef MACOS
int trace_flag = 0;
int debug_flag = 0;
int quiet_flag = 0;
int verbose_flag = 0;
int syslog_flag = 0;
#else
extern Boolean gLogStdIO;
extern int g_error_fd;
#endif

#define WERROR_TRACE -1
#define WERROR_DEBUG -2

static const struct argp_option
werror_options[] =
{
  { "quiet", 'q', NULL, 0, "Suppress all warnings and diagnostic messages", 0 },
  { "verbose", 'v', NULL, 0, "Verbose diagnostic messages", 0},
  { "trace", WERROR_TRACE, NULL, 0, "Detailed trace", 0 },
  { "debug", WERROR_DEBUG, NULL, 0, "Print huge amounts of debug information", 0 },
  { NULL, 0, NULL, 0, NULL, 0 }
};

static error_t
werror_argp_parser(int key, char *arg UNUSED,
		   struct argp_state *state UNUSED)
{
  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case 'q':
      quiet_flag = 1;
      break;
    case 'v':
      verbose_flag = 1;
      break;
    case WERROR_TRACE:
      trace_flag = 1;
      break;
    case WERROR_DEBUG:
      debug_flag = 1;
      break;
    }
  return 0;
}

const struct argp werror_argp =
{
  werror_options,
  werror_argp_parser,
  NULL, NULL, NULL, NULL, NULL
};

#ifndef MACOS
static int error_fd = STDERR_FILENO;

#define BUF_SIZE 500
static UINT8 error_buffer[BUF_SIZE];
static UINT32 error_pos = 0;

static const struct exception *
(*error_write)(int fd, UINT32 length, const UINT8 *data) = write_raw;
#else
#define BUF_SIZE ERROR_BUF_SIZE
#endif

#if HAVE_SYSLOG
static const struct exception *
write_syslog(int fd UNUSED, UINT32 length, const UINT8 *data)
{
  struct lsh_string *s = make_cstring_l(length, data);

  /* NOTE: make_cstring fails if the data contains any NUL characters. */
  assert(s);

#if 0
  /* Make sure the message is properly terminated with \0. */
  snprintf(string_buffer, MIN(BUF_SIZE, length), "%s", data);
#endif
  /* FIXME: Should we use different log levels for werror, verbose and
   * debug? */
  
  syslog(LOG_NOTICE, "%s", s->data);
  lsh_string_free(s);
  
  return NULL;
}

void
set_error_syslog(const char *id)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  openlog(id, LOG_PID | LOG_CONS, LOG_DAEMON);
  error_write = write_syslog;
  error_fd = -1;
}
#endif /* HAVE_SYSLOG */

static const struct exception *
write_ignore(int fd UNUSED,
	     UINT32 length UNUSED, const UINT8 *data UNUSED)
{ return NULL; }

void
set_error_stream(int fd, int with_poll)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  error_fd = fd;

  error_write = with_poll ? write_raw_with_poll : write_raw;
}

int
dup_error_stream(void)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return -1;
  }
  if (error_fd < 0)
    /* We're not writing error messages on any file; there's no
     * problem. */
    return 1;
  else
    {
      int fd = dup(error_fd);
      if (fd > STDERR_FILENO)
	{
	  io_set_close_on_exec(fd);
	  error_fd = fd;
	  return 1;
	}

      if (fd >= 0)
	close(fd);
      
      return 0;
    }
}

void
set_error_ignore(void)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  context->_error_write = write_ignore;
  context->_error_fd = -1;
}

/*#define WERROR(c, l, d) (c->_error_write(c->_error_fd, (l), (d)))*/
static void
WERROR(lshcontext *context, int l, const UINT8 *d)
{
  int oldConvertLFs = context->_convertLFs;
  if (gLogStdIO || (!context->_tracing && !context->_verbosing && !context->_debugging))
    context->_convertLFs = true;
  context->_error_write(context->_error_fd, l, d);
  if (gLogStdIO || (!context->_tracing && !context->_verbosing && !context->_debugging))
    context->_convertLFs = oldConvertLFs;
}

static void
werror_flush(void)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (context->_error_pos)
    {
      WERROR(context, context->_error_pos, context->_error_buffer);
      context->_error_pos = 0;
    }
}

static void
werror_putc(UINT8 c)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (context->_error_pos == BUF_SIZE)
    werror_flush();

  context->_error_buffer[context->_error_pos++] = c;
}

static void
werror_write(UINT32 length, const UINT8 *msg)
{
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (context->_error_pos + length <= BUF_SIZE)
    {
      memcpy(context->_error_buffer + context->_error_pos, msg, length);
      context->_error_pos += length;
    }
  else
    {
      werror_flush();
      WERROR(context, length, msg);
    }
}

static void
werror_cstring(char *s) { werror_write(strlen(s), (const unsigned char *)s); }

static void
werror_bignum(mpz_t n, int base)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  char *s = alloca(mpz_sizeinbase(n, base) + 2);
  mpz_get_str(s, 16, n);

  werror_cstring(s);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

static void
werror_decimal(UINT32 n)
{
#if ALLOCA_68K_BUG
  ALLOCA_START(alloca_ref);
#endif
  unsigned length = format_size_in_decimal(n);
  UINT8 *buffer = alloca(length);

  format_decimal(length, buffer, n);

  werror_write(length, buffer);
#if ALLOCA_68K_BUG
  ALLOCA_FREE(alloca_ref);
#endif
}

static unsigned format_size_in_hex(UINT32 n);

static void
werror_hex_digit(unsigned digit)
{
  werror_putc("0123456789abcdef"[digit]);
}

static void
werror_hex_putc(UINT8 c)
{
  werror_hex_digit(c / 16);
  werror_hex_digit(c % 16);
}

static void
werror_hex(UINT32 n)
{
  unsigned left = 8;
  
  while ( (left > 1)
	  && !(n & 0xf0000000UL))
    {
      left --;
      n <<= 4;
    }
		    
  while (left--)
    {
      werror_hex_digit((n >> 28) & 0xf);
      n <<= 4;
    }
}

static void
werror_hexdump(UINT32 length, UINT8 *data)
{
  UINT32 i = 0;
  
  werror("(size %i = 0x%xi)\n", length, length);

  for (i = 0; i<length; i+= 16)
    {
      unsigned j = format_size_in_hex(i);
      unsigned r = length - i;
      
      for ( ; j < 8; j++)
	werror_putc('0');

      werror_hex(i);
      werror_cstring(": ");

      if (r > 16)
	r = 16;

      for (j = 0; j<r; j++)
	werror_hex_putc(data[i+j]);

      for (; j<17; j++)
	werror_cstring("  ");

      for (j = 0; j<r; j++)
	{
	  UINT8 c = data[i+j];
	  if ( (c < 32) || (c > 126) )
	    c = '.';
	  werror_putc(c);
	}

      werror_cstring("\n");
    }
}

static void
werror_paranoia_putc(UINT8 c)
{
  switch (c)
    {
    case '\\':
      werror_cstring("\\\\");
      break;
    case '\r':
      /* Ignore */
      break;
    default:
      if (!isprint(c))
	{
	  werror_putc('\\');
	  werror_hex_putc(c);
	  break;
	}
      /* Fall through */
    case '\n':
      werror_putc(c);
      break;
    }
}

void
werror_vformat(const char *f, va_list args)
{
  while (*f)
    {
      if (*f == '%')
	{
	  int do_hex = 0;
	  int do_free = 0;
	  int do_paranoia = 0;
	  int do_utf8 = 0;

	  while (*++f)
	    switch (*f)
	      {
	      case 'x':
		do_hex = 1;
		break;
	      case 'f':
		do_free = 1;
		break;
	      case 'p':
		do_paranoia = 1;
		break;
	      case 'u':
		do_utf8 = 1;
		break;
	      default:
		goto end_options;
	      }
	end_options:
	  switch(*f++)
	    {
	    case '%':
	      werror_putc(*f);
	      break;
	    case 'i':
	      (do_hex ? werror_hex : werror_decimal)(va_arg(args, UINT32));
	      break;
	    case 'c':
	      werror_putc(va_arg(args, int));
	      break;
	    case 'n':
	      werror_bignum(va_arg(args, MP_INT *), do_hex ? 16 : 10);
	      break;
	    case 'z':
	      {
		char *s = va_arg(args, char *);

		if (do_hex)
		  werror_hexdump(strlen(s), (unsigned char *)s);

		while (*s)
		  (do_paranoia ? werror_paranoia_putc : werror_putc)(*s++);

		break;
	      }
	    case 'a':
	      {
		int atom = va_arg(args, int);
		
		assert(atom);

		werror_write(get_atom_length(atom), get_atom_name(atom));
		
		break;
	      }
	    case 's':
	      {
		UINT32 length = va_arg(args, UINT32);
		UINT8 *s = va_arg(args, UINT8 *);

		struct lsh_string *u = NULL; 

		if (do_utf8 && !local_is_utf8())
		  {
		    u = low_utf8_to_local(length, s, 0);
		    if (!u)
		      {
			werror_cstring("<Invalid utf-8 string>");
			break;
		      }
		    length = u->length;
		    s = u->data;
		  }
		if (do_hex)
		  {
		    assert(!do_paranoia);
		    werror_hexdump(length, s);
		  }
		else if (do_paranoia)
		  {
		    UINT32 i;
		    for (i=0; i<length; i++)
		      werror_paranoia_putc(*s++);
		  }
		else
		  werror_write(length, s);

		if (u)
		  lsh_string_free(u);
	      }
	      break;
	    case 'S':
	      {
		struct lsh_string *s = va_arg(args, struct lsh_string *);

		if (do_utf8)
		  {
		    s = utf8_to_local(s, 0, do_free);
		    if (!s)
		      {
			werror_cstring("<Invalid utf-8 string>");
			break;
		      }
		    do_free = 1;
		  }
		if (do_hex)
		  {
		    assert(!do_paranoia);
		    werror_hexdump(s->length, s->data);
		  }
		else if (do_paranoia)
		  {
		    UINT32 i;
		    for (i=0; i<s->length; i++)
		      werror_paranoia_putc(s->data[i]);
		  }
		else
		  werror_write(s->length, s->data);

		if (do_free)
		  lsh_string_free(s);
	      }
	      break;

	    default:
	      fatal("werror_vformat: bad format string!\n");
	      break;
	    }
	}
      else
	werror_putc(*f++);
    }
}

void
werror(const char *format, ...) 
{
  va_list args;
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (!context->_quiet_flag)
    {
      if (gLogStdIO || (!context->_tracing && !context->_verbosing && !context->_debugging)) {
      	if (!gLogStdIO) {
          werror_flush();
      	  /*set_error_stream(STDOUT_FILENO, 0);*/
      	  set_error_stream(g_error_fd, 0);
      	}
      }
      va_start(args, format);
      werror_vformat(format, args);
      va_end(args);
      werror_flush();
      if (gLogStdIO || (!context->_tracing && !context->_verbosing && !context->_debugging)) {
      	if (!gLogStdIO) {
      	  set_error_syslog("");
      	}
      }
    }
}

void
trace(const char *format, ...) 
{
  va_list args;
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (context->_trace_flag)
    {
      werror_flush();
      context->_tracing = true;
      va_start(args, format);
      werror_vformat(format, args);
      va_end(args);
      werror_flush();
      context->_tracing = false;
    }
}

void
debug(const char *format, ...) 
{
  va_list args;
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (context->_debug_flag)
    {
      werror_flush();
      context->_debugging = true;
      va_start(args, format);
      werror_vformat(format, args);
      va_end(args);
      werror_flush();
      context->_debugging = false;
    }
}

void
verbose(const char *format, ...) 
{
  va_list args;
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if ( !context ) {
    return;
  }
  if (context->_verbose_flag)
    {
      werror_flush();
      context->_verbosing = true;
      va_start(args, format);
      werror_vformat(format, args);
      va_end(args);
      werror_flush();
      context->_verbosing = false;
    }
}

void
fatal(const char *format, ...) 
{
  va_list args;
  lshcontext *context = (lshcontext *)pthread_getspecific(ssh2threadkey);
  if (context)
    {
      werror_flush();
      /*set_error_stream(STDOUT_FILENO, 0);*/
      set_error_stream(g_error_fd, 0);
      va_start(args, format);
      werror_vformat(format, args);
      va_end(args);
      werror_flush();
      set_error_syslog("");
    }
  abort();
}

static unsigned
format_size_in_hex(UINT32 n)
{
  int i;
  int e;
  
  /* Table of 16^(2^n) */
  static const UINT32 powers[] = { 0x10UL, 0x100UL, 0x10000UL };

#define SIZE (sizeof(powers) / sizeof(powers[0])) 

  /* Determine the smallest e such that n < 16^e */
  for (i = SIZE - 1 , e = 0; i >= 0; i--)
    {
      if (n >= powers[i])
	{
	  e += 1UL << i;
	  n /= powers[i];
	}
    }

#undef SIZE
  
  return e+1;
}


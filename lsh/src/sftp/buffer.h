/* buffer.h
 *
 * Buffering for sftp.
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2001 Niels Möller
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */

#ifndef SFTP_BUFFER_H_INCLUDED
#define SFTP_BUFFER_H_INCLUDED

/* Basic configuration */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* For off_t */
#include <sys/types.h>

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


/* Abstract input and output functions */
#include <time.h>

struct sftp_input;
struct sftp_output;

/* Input */

/* Returns 1 of all was well, 0 on error, and -1 on EOF */
int
sftp_read_packet(struct sftp_input *i);

int
sftp_get_data(struct sftp_input *i, UINT32 length, UINT8 *data);

int
sftp_get_uint8(struct sftp_input *i, UINT8 *value);

int
sftp_get_uint32(struct sftp_input *i, UINT32 *value);

int
sftp_get_uint64(struct sftp_input *i, off_t *value);

/* Allocates storage. Caller must deallocate using
 * sftp_free_string. */
UINT8 *
sftp_get_string(struct sftp_input *i, UINT32 *length);

void
sftp_free_string(UINT8 *s);

/* Like sftp_get_string, but the data is deallocated automatically by
 * sftp_read_packet and sftp_input_clear_strings. */
UINT8 *
sftp_get_string_auto(struct sftp_input *i, UINT32 *length);

void
sftp_input_clear_strings(struct sftp_input *i);

int
sftp_get_eod(struct sftp_input *i);

/* Output */

void
sftp_set_msg(struct sftp_output *o, UINT8 msg);

void
sftp_set_id(struct sftp_output *o, UINT32 id);

int
sftp_write_packet(struct sftp_output *o);

void
sftp_put_data(struct sftp_output *o, UINT32 length, const UINT8 *data);

void
sftp_put_uint8(struct sftp_output *o, UINT8 value);

void
sftp_put_uint32(struct sftp_output *o, UINT32 value);

void
sftp_put_uint64(struct sftp_output *o, off_t value);

void
sftp_put_string(struct sftp_output *o, UINT32 length, const UINT8 *data);

/* Returns index. */
UINT32
sftp_put_reserve_length(struct sftp_output *o);

void
sftp_put_final_length(struct sftp_output *o,
		      UINT32 index);

void
sftp_put_length(struct sftp_output *o,
		UINT32 index,
		UINT32 length);

void
sftp_put_reset(struct sftp_output *o,
	       UINT32 index);

UINT32
sftp_put_printf(struct sftp_output *o, const char *format, ...)
     PRINTF_STYLE(2,3);
     
UINT32
sftp_put_strftime(struct sftp_output *o, UINT32 size,
		  const char *format,
		  const struct tm *tm);

     
/* Constructed types. */

struct sftp_attrib
{
  UINT32 flags;
  off_t size;
  UINT32 uid;
  UINT32 gid;
  UINT32 permissions;

  /* NOTE: The representations of times is about to change. */
  UINT32 atime;
  UINT32 mtime;
};

void
sftp_clear_attrib(struct sftp_attrib *a);

int
sftp_get_attrib(struct sftp_input *i, struct sftp_attrib *a);

void
sftp_put_attrib(struct sftp_output *o, const struct sftp_attrib *a);

int
sftp_skip_extension(struct sftp_input *i);


/* Simple input and output objects based on FILE * */
#include <stdio.h>

struct sftp_input *
sftp_make_input(FILE *f);

struct sftp_output *
sftp_make_output(FILE *f);


#endif /* SFTP_BUFFER_H_INCLUDED */

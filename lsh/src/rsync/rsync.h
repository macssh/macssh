/* rsync.h
 *
 * $Id$
 */

/* 
   Copyright (C) Andrew Tridgell 1996
   Copyright (C) Paul Mackerras 1996
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* Hacked by Niels Möller */
#ifndef RSYNC_H_INCLUDED
#define RSYNC_H_INCLUDED

#if LSH
# include "lsh_types.h"
#else
# if HAVE_CONFIG_H
#  include "config.h"
# endif
#endif

#include <inttypes.h>

#include <nettle/md5.h>

#include <stdlib.h>

/* FIXME: replace with proper autoconf check */
#define OFF_T size_t

#define RSYNC_SUM_SIZE MD5_DIGEST_SIZE

/* Size of block count, block size, tail */
#define RSYNC_HEADER_SIZE 12

/* Size of weak sum, md5 sum */
#define RSYNC_ENTRY_SIZE 20

#define RSYNC_TOKEN_SIZE 4

/* Initial checksum calculations (by the receiver) */

/* NOTE: Unlike zlib, we want to know the file size before we start.
 * This could be relaxed, but requires some modifications to the
 * protocol. */
struct rsync_generate_state
{
  /* Public fields */
  uint8_t *next_in;
  uint32_t avail_in;
  uint8_t *next_out;
  uint32_t avail_out;

  uint32_t block_size;
  uint32_t total_length;
  uint32_t offset; /* Current offset in input file. */

  /* Weak check sum */
  unsigned a_sum;
  unsigned c_sum;
  
  struct md5_ctx block_sum;

  /* Internal state */
  uint8_t buf[RSYNC_ENTRY_SIZE];
  uint8_t buf_length; /* Zero means no buffered data. */
  uint8_t buf_pos;

  uint32_t left; /* Amount left of current block */
};

/* Return values */
enum rsync_result_t
{
  /* Things are working fine */
  RSYNC_PROGRESS,
  
  /* All data is flushed to the output */
 RSYNC_DONE,
  
  /* No progress possible */
  RSYNC_BUF_ERROR,
  
  /* Invalid input */
  RSYNC_INPUT_ERROR,
  
  /* Out of memory (can happen only for rsync_read_table and rsync_send_init) */
  RSYNC_MEMORY
};

enum rsync_result_t
rsync_generate(struct rsync_generate_state *state);

enum rsync_result_t
rsync_generate_init(struct rsync_generate_state *state,
		    uint32_t block_size,
		    uint32_t size);


/* Receiving a file. */

/* The receiver calls this function to copy at most LENGTH octets of
 * local data to the output buffer.
 *
 * OPAQUE is state private to the lookup function. DST and LENGTH give
 * the location of the destination buffer. INDEX is the block to read,
 * and OFFSET is a location within that block.
 *
 * The function should return
 *
 * -1 on failure (and it has to check INDEX and OFFSET for validity).
 * 0 if copying succeeds, but not all of the block was copied.
 * 1 if copying succeeds, and the final octet of the data was copied.
 *
 * On success, the function should set *DONE to the amount of data copied.
 */

typedef int (*rsync_lookup_read_t)(void *opaque,
				   uint8_t *dst, uint32_t length,
				   uint32_t index, uint32_t offset, uint32_t *done);

struct rsync_receive_state
{
  /* Public fields */
  uint8_t *next_in;
  uint32_t avail_in;
  uint8_t *next_out;
  uint32_t avail_out;

  uint32_t block_size;
  /* uint32_t offset; */ /* Current offset in output file. */

  rsync_lookup_read_t lookup;
  void *opaque;
  
  struct md5_ctx sum_md5; /* Sum of all the output data */

  /* Private state */

  enum {
    /* Reading a partial token */
    RSYNC_READ_TOKEN,

    /* Reading a literal */
    RSYNC_READ_LITERAL,

    /* Copying a local block */
    RSYNC_READ_LOOKUP,

    /* Reading final md5 sum */
    RSYNC_READ_CHECKSUM,

    /* Results in error */
    RSYNC_READ_INVALID
  } state;
  
  uint32_t token; 
  uint32_t i;

  uint8_t buf[RSYNC_SUM_SIZE];
};

enum rsync_result_t
rsync_receive(struct rsync_receive_state *state);

void
rsync_receive_init(struct rsync_receive_state *state);

/* Sending files */

struct rsync_table;
struct rsync_node;

struct rsync_read_table_state
{
  /* Public fields */
  struct rsync_table *table;

  /* Limits */
  uint32_t max_count;
  uint32_t max_block_size;
    
  uint32_t count; /* Block count */
  uint32_t block_size;
  uint32_t remainder;

  /* Private state */
  uint8_t buf[RSYNC_ENTRY_SIZE];
  unsigned pos;
};

enum rsync_result_t
rsync_read_table(struct rsync_read_table_state *state,
		 uint32_t length, uint8_t *input);

/* For reading the list of checksums. */
struct rsync_send_state
{
  /* Public fields */
  uint8_t *next_in;
  uint32_t avail_in;
  uint8_t *next_out;
  uint32_t avail_out;

  struct rsync_table *table;
  
  /* Internal state */

  enum { RSYNC_SEND_READING, RSYNC_SEND_WRITING } state;

  /* Non-zero if the final EOF-token has been buffered for output. */
  int final;
  
  /* The next block that we expect */
  struct rsync_node *guess;

  /* The input and output buffer. */
  
  uint32_t buf_size;
  /* The allocated size includes space for header and trailer,
   * besides the input buffer of BUD_SIZE octets. */
  uint8_t *buf;
  
  /* In writing mode, we copy the data from I to OUT_END */
  uint32_t i;
  uint32_t out_end;

  /* Size of buffer (relevant in primarily in read mode).
   * It does not include the buffer header. */

  uint32_t size;
  
  unsigned sum_a;
  unsigned sum_b;

  struct md5_ctx sum_md5; /* Sum of all the input data */

};

enum rsync_result_t
rsync_send_init(struct rsync_send_state *state,
		struct rsync_table *table);
		     
enum rsync_result_t
rsync_send(struct rsync_send_state *state, int flush);

void rsync_send_free(struct rsync_send_state *state); 

void
rsync_update_1(unsigned *ap, unsigned *cp,
	       uint32_t length, uint8_t *data);

struct rsync_node *
rsync_search(unsigned *ap, unsigned *bp, unsigned block_size,
	     uint32_t length, uint8_t *start, uint8_t *end,
	     uint32_t *found, struct rsync_node **hash);

#endif /* RSYNC_H_INCLUDED */

/* send.c
 *
 * The sending end of the rsync algorithm. 
 *
 * $Id$ */

#include "rsync.h"

#include <assert.h>
#include <string.h>

#define HASH_SIZE 0x10000
#define HASH_SUM(a, b) (((a) ^ (b)) & 0xffff)
#define COMBINE_SUM(a, b) ((a) | ((b) << 16))

struct rsync_node
{
  struct rsync_node *next;
  UINT32 index;
  UINT32 length;
  
  UINT32 sum_weak; /* a | b << 16*/

  UINT8 sum_md5[MD5_DIGESTSIZE];
};

struct rsync_table
{
  struct rsync_node *hash[HASH_SIZE];
  UINT32 alloc_size;
  UINT32 size;
  UINT32 block_size;

  struct rsync_node all_nodes[1];
};

static struct rsync_table *
make_rsync_table(UINT32 count, UINT32 block_size)
{
  unsigned i;
  
  struct rsync_table *table =
    malloc(sizeof(struct rsync_table) - sizeof(struct rsync_node)
	   + count * sizeof(struct rsync_node));
  
  if (!table)
    return NULL;

  for (i = 0; i<HASH_SIZE; i++)
    table->hash[i] = NULL;

  table->alloc_size = count;
  table->size = 0;

  table->block_size = block_size;

  return table;
}

static struct rsync_node *
rsync_add_entry(struct rsync_table *table,
		UINT8 *input)
{
  struct rsync_node *node;
  unsigned a, b;
  unsigned h;

  assert(table->size < table->alloc_size);
  node = table->all_nodes + table->size;

  node->index = table->size++;

  /* NOTE: Length field is left uninitialized for now */
  
  a = READ_UINT16(input);
  b = READ_UINT16(input + 2);

  node->sum_weak = COMBINE_SUM(a, b);
  memcpy(node->sum_md5, input + 4, RSYNC_SUM_SIZE);

  h = HASH_SUM(a, b);
  node->next = table->hash[h];
  table->hash[h] = node;

  return node;
}

static struct rsync_node *
rsync_lookup_1(struct rsync_node *n, UINT32 weak)
{
  while (n && (n->sum_weak != weak))
    n = n->next;

  return n;
}

static struct rsync_node *
rsync_lookup_2(struct rsync_node *n, UINT32 weak,
	       const UINT8 *digest)
{
  /* FIXME: This could be speeded up slightly if the hash lists were
   * kept sorted on weak_sum. */
  while (n && ( (n->sum_weak != weak)
		|| memcmp(n->sum_md5, digest, RSYNC_SUM_SIZE)))
      n = n->next;

  return n;
}

static struct rsync_node *
rsync_lookup_block(struct rsync_send_state *s,
		   UINT32 start, UINT32 size)

{
  struct rsync_node *n;
  
  assert(size);

  if (size == s->table->block_size)
    {
      n = s->table->hash[HASH_SUM(s->sum_a, s->sum_b)];
      if (n)
	{
	  /* The first block might match. */
	  UINT32 weak = COMBINE_SUM(s->sum_a, s->sum_b);
	  struct md5_ctx m;
	  UINT8 digest[MD5_DIGESTSIZE];

	  /* First check our guess. */
	  if (s->guess && (s->guess->sum_weak == weak))
	    {
	      md5_init(&m);
	      md5_update(&m, s->buf + start, s->table->block_size);
	      md5_final(&m);
	      md5_digest(&m, digest);

	      if (!memcmp(s->guess->sum_md5, digest, RSYNC_SUM_SIZE))
		{
		  /* Correct guess! */
		  n = s->guess;
		}
	      else
		n = rsync_lookup_2(n, weak, digest);
	    }
	  else
	    {
	      n = rsync_lookup_1(n, weak);
	      if (n)
		{
		  md5_init(&m);
		  md5_update(&m, s->buf + start, s->table->block_size);
		  md5_final(&m);
		  md5_digest(&m, digest);

		  n = rsync_lookup_2(n, weak, digest);
		}
	    }
	}
      if (n)
	{
	  /* Guess for the next block. */
	  s->guess = n + 1;

	  /* Does it make sense? */
	  if ( (s->guess == (s->table->all_nodes + s->table->alloc_size))
	       || (s->guess->length < s->table->block_size) )
	    s->guess = NULL;
	}
    }
  else
    {
      /* A smaller block. It could only match the final block. */
      s->guess = NULL;
      n = s->table->all_nodes + s->table->alloc_size - 1;
      if (size == n->length)
	{
	  UINT32 weak = COMBINE_SUM(s->sum_a, s->sum_b);

	  if (weak == n->sum_weak)
	    {
	      struct md5_ctx m;
	      UINT8 digest[MD5_DIGESTSIZE];

	      md5_init(&m);
	      md5_update(&m, s->buf + start, size);
	      md5_final(&m);
	      md5_digest(&m, digest);

	      if (!memcmp(n->sum_md5, digest, RSYNC_SUM_SIZE))
		return n;
	    }
	}
    }
  return NULL;
}


enum rsync_result_t
rsync_read_table(struct rsync_read_table_state *s,
		 UINT32 length, UINT8 *input)
{
  while (length)
    if (!s->table)
      {
	UINT32 left = RSYNC_HEADER_SIZE - s->pos;
	if (length < left)
	  {
	    memcpy(s->buf + s->pos, input, length);
	    s->pos += length;
	    return RSYNC_PROGRESS;
	  }
	else
	  {
	    UINT32 block_size;
	    
	    memcpy(s->buf + s->pos, input, left);
	    input += left;
	    length -= left;
	    s->pos = 0;
	    
	    s->count = READ_UINT32(s->buf);
	    block_size = READ_UINT32(s->buf + 4);
	    s->remainder = READ_UINT32(s->buf + 8);

	    if ( (s->count > s->max_count)
		 || (s->block_size > s->max_block_size)
		 || (s->remainder >= s->block_size))
	      return RSYNC_INPUT_ERROR;

	    s->table = make_rsync_table(s->count, block_size);
	    
	    return (s->table) ? RSYNC_PROGRESS : RSYNC_MEMORY;
	  }
      }
    else
      {
	struct rsync_node *node;

	if (s->pos)
	  {
	    /* Do partial entries */
	    UINT32 left = RSYNC_ENTRY_SIZE - s->pos;
	    if (length < left)
	      {
		memcpy(s->buf + s->pos, input, length);
		s->pos += length;
		return RSYNC_PROGRESS;
	      }
	    else
	      {
		memcpy(s->buf + s->pos, input, left);
		input += left;
		length -= left;
		s->pos = 0;

		node = rsync_add_entry(s->table, s->buf);
	      }
	  }
	else if (length < RSYNC_ENTRY_SIZE)
	  {
	    /* New partial block */
	    memcpy(s->buf, input, length);
	    s->pos = length;
	    return RSYNC_PROGRESS;
	  }
	else
	  {
	    node = rsync_add_entry(s->table, input);
	    length -= RSYNC_ENTRY_SIZE;
	    input += RSYNC_ENTRY_SIZE;
	  }
	
	node->length = s->block_size;
	
	if (s->table->size == s->table->alloc_size)
	  {
	    if (s->remainder)
	      node->length = s->remainder;
	    
	    return length ? RSYNC_INPUT_ERROR : RSYNC_DONE;
	  }
      }
  return RSYNC_PROGRESS;
}

/* While searching, we have to keep a buffer of previous block of
 * data. Our buffer BUF consists of SIZE octets starting after the header space.
 *
 * We may have less than one block of data available, in that case we
 * must collect more data before we can start searching. If we collect
 * more than buf_size (usually twice the block size), we output a
 * literal. */

#define BUFFER_HEADER RSYNC_TOKEN_SIZE
#define BUFFER_TRAILER (RSYNC_TOKEN_SIZE + RSYNC_SUM_SIZE)

enum rsync_result_t
rsync_send_init(struct rsync_send_state *s,
		struct rsync_table *table)
{
  assert(table->block_size <= 0xffffffffU/2);

  /* The buffer must be at least twice the block size. */
  s->buf_size = table->block_size * 3;
  s->table = table;

  s->buf = malloc(s->buf_size + BUFFER_HEADER + BUFFER_TRAILER);
  
  if (!s->buf)
    return RSYNC_MEMORY;
  
  s->size = 0;
  
  s->sum_a = s->sum_b = 0;

  md5_init(&s->sum_md5);
  
  s->state = RSYNC_SEND_READING;
  s->final = 0;
  
  return RSYNC_PROGRESS;
}

static void
rsync_send_eof(struct rsync_send_state *s)
{
  /* FIXME: Try matching the final block. */
  UINT32 end = 0;

  /* If buffer is non-empty, add a literal. */  
  if (s->size)
    {
      WRITE_UINT32(s->buf, s->size);
      end = s->size + 4;
    }

  /* Write EOF marker */
  WRITE_UINT32(s->buf + end, 0);

  end += 4;

  /* And final hash of the entire file */
  md5_final(&s->sum_md5);
  md5_digest(&s->sum_md5, s->buf + end);

  end += RSYNC_SUM_SIZE;

  s->size = 0;
  s->i = 0;
  s->out_end = end;
  s->final = 1;
  s->state = RSYNC_SEND_WRITING;
}

/* The start of the input buffer */
#define BUF (s->buf + BUFFER_HEADER)

/* Copy from input buffer, and update md5 sum. */
static void
rsync_send_copy_in(struct rsync_send_state *s,
		   UINT32 length)
{
  assert(length <= s->avail_in);

  md5_update(&s->sum_md5, s->next_in, length);
  memcpy(BUF + s->size, s->next_in, length);
  s->next_in += length;
  s->avail_in -= length;
  s->size += length;
}

static void
rsync_send_read(struct rsync_send_state *s, UINT32 left)
{
  /* The current hash does not include a complete block. We need more data. */
  struct rsync_node *n;
  UINT32 avail = MIN(left, s->avail_in);

  assert(avail);
  
  /* Update weak sum */ 
  rsync_update_1(&s->sum_a, &s->sum_b, avail, s->next_in);
  rsync_send_copy_in(s, avail);
  
  if (s->size < s->table->block_size)
    return;

  /* We have a complete block. Check if we have a match already. */
  n = rsync_lookup_block(s, 0, s->size);

  if (n)
    {
      /* We have a match! */
      UINT32 token = ~(n - s->table->all_nodes);
      
      WRITE_UINT32(s->buf, token);

      s->size = 0;
      s->i = 0;
      s->out_end = RSYNC_TOKEN_SIZE;
      s->state = RSYNC_SEND_WRITING;
    }
}

static void
rsync_send_search(struct rsync_send_state *s)
{
  UINT32 avail;
  UINT32 done;
  struct rsync_node *n;
  
  assert(s->size >= s->table->block_size);
  assert(s->size < s->buf_size);
  
  avail = MIN(s->avail_in, s->buf_size - s->size);

  n = rsync_search(&s->sum_a, &s->sum_b, s->table->block_size,
		   avail,
		   BUF + s->size - s->table->block_size,
		   s->next_in,
		   &done, s->table->hash);

  /* done should be non-zero */
  assert(done);

  if (n)
    {
      /* The block
       *
       *   BUF[size + done - block_size...size] + next_in[0...done]
       *
       * might match */
      
      UINT32 weak = COMBINE_SUM(s->sum_a, s->sum_b);
            
      n = rsync_lookup_1(n, weak);

      if (n)
	{
	  struct md5_ctx m;
	  UINT8 digest[MD5_DIGESTSIZE];
	  UINT32 start = s->size + done - s->table->block_size;

	  assert(start);
	  
	  /* NOTE: Don't bother examining our guess. */

	  md5_init(&m);
	  md5_update(&m, BUF + start,
		     s->table->block_size - done);
	  md5_update(&m, s->next_in, done);
	  md5_final(&m);
	  md5_digest(&m, digest);

	  n = rsync_lookup_2(n, weak, digest);

	  if (n)
	    {
	      /* Match found! */

	      /* Token is one-complement of the index */
	      UINT32 token = ~(n - s->table->all_nodes);
	      
	      /* Hash input before we discard it */
	      md5_update(&s->sum_md5, s->next_in, done);
	      s->next_in += done;
	      s->avail_in -= done;
	      
	      /* Length of literal */
	      WRITE_UINT32(s->buf, start);
	      
	      /* Block reference, written after the literal data. */
	      WRITE_UINT32(BUF + start, token);

	      s->size = 0;
	      s->i = 0;
	      s->out_end = start + (2 * RSYNC_TOKEN_SIZE);
	      s->state = RSYNC_SEND_WRITING;

	      return;
	    }
	}
    }

  /* No match so far. Copy the data up to the apparent match, or all
   * available if there were no apparent match. */
  rsync_send_copy_in(s, done);
}

static void
rsync_send_flush(struct rsync_send_state *s)
{
  /* Entire buffer filled, but no match. Make a literal
   * out of all but the last block in the buffer */

  UINT32 length = s->size - s->table->block_size;
  WRITE_UINT32(s->buf, length);

  s->size = s->table->block_size;
  s->i = 0;
  s->out_end = length + RSYNC_TOKEN_SIZE;
  s->state = RSYNC_SEND_WRITING;
}

static int
rsync_send_write(struct rsync_send_state *s)
{
  /* Trassmits octets between I and OUT_END */

  UINT32 left = s->out_end - s->i;
  if (left <= s->avail_out)
    {
      memcpy(s->next_out, s->buf + s->i, left);
      s->next_out += left;
      s->avail_out -= left;
      s->state = RSYNC_SEND_READING;

      return 1;
    }
  else
    {
      memcpy(s->next_out, s->buf + s->i, s->avail_out);
      s->next_out += s->avail_out;
      s->i += s->avail_out;
      s->avail_out = 0;

      return 0;
    }
}

enum rsync_result_t
rsync_send(struct rsync_send_state *s, int flush)
{
  enum rsync_result_t result = RSYNC_BUF_ERROR;

  for (;;)
    {
      switch (s->state)
	{
	case RSYNC_SEND_READING:
	  assert(!s->final);
	  assert(s->size <= s->buf_size);
	  
	  if (!s->avail_in)
	    {
	      if (!flush)
		return result;

	      rsync_send_eof(s);
	      continue;
	    }

	  if (s->size == s->buf_size)
	    rsync_send_flush(s);
	  else
	    {
	      result = RSYNC_PROGRESS;
	  
	      if (s->size < s->table->block_size)
		rsync_send_read(s, s->table->block_size - s->size);
	      else
		rsync_send_search(s);
	    }
	  break;
	  
	case RSYNC_SEND_WRITING:
	  if (!s->avail_out)
	    return result;
	  
	  if (rsync_send_write(s) && s->final)
	    return RSYNC_DONE;
	  else
	    result = RSYNC_PROGRESS;
	}
    }
}

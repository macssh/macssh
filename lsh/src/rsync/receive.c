/* receive.c
 *
 * The receive end of the rsync algorithm. 
 *
 * $Id$ */

#include "rsync.h"

#include <assert.h>
#include <string.h>

/* Hash the data after it is copied to the output buffer. */
static void
rsync_update(struct rsync_receive_state *s,
	     UINT32 length)
{
  md5_update(&s->sum_md5, s->next_out, length);
  s->next_out += length;
  s->avail_out -= length;
}

#define GET() (assert(s->avail_in), s->avail_in--, *s->next_in++)

enum rsync_result_t
rsync_receive(struct rsync_receive_state *s)
{
  int progress = 0;
  
  for (;;)
    /* FIXME: Cast to enum rsync_receive_mode, for better type checking? */
    switch (s->state)
      {
      do_token:
	/* Here, i is octets read */
	s->token = 0;
	s->i = 0;
	s->state = RSYNC_READ_TOKEN;
      case RSYNC_READ_TOKEN:
	if (!s->avail_in)
	  return progress ? RSYNC_PROGRESS : RSYNC_BUF_ERROR;
	
	s->token = (s->token << 8) | GET();
	s->i++;

	progress = 1;
	
	if (s->i == 4)
	  {
	    if (!s->token)
	      goto do_checksum;
	    
	    else if (! (s->token & 0x80000000))
	      {
		s->i = s->token;
		goto do_literal;
	      }
	    else
	      {
		/* Index is one's complement */ 
		s->token = ~s->token;
		goto do_lookup;
	      }
	  }
	break;

      do_literal:
	/* Here, i is the number of octets to read. */
	s->state = RSYNC_READ_LITERAL;
      case RSYNC_READ_LITERAL:
	{
	  UINT32 avail = MIN(s->avail_in, s->avail_out);
	  if (!avail)
	    return progress ? RSYNC_PROGRESS : RSYNC_BUF_ERROR;

	  if (avail < s->i)
	    {
	      memcpy(s->next_out, s->next_in, avail);
	      rsync_update(s, avail);
	      s->next_in += avail;
	      s->avail_in -= avail;
	      s->i -= avail;
	    }
	  else
	    {
	      memcpy(s->next_out, s->next_in, s->i);
	      rsync_update(s, s->i);
	      s->next_in += s->i;
	      s->avail_in -= s->i;
	      goto do_token;
	    }
	}
	break;

      do_lookup:
	s->state = RSYNC_READ_LOOKUP;
	s->i = 0;
      case RSYNC_READ_LOOKUP:
	{
	  UINT32 done;

	  if (!s->avail_out)
	    return progress ? RSYNC_PROGRESS : RSYNC_BUF_ERROR;
	  
	  switch (s->lookup(s->opaque, s->next_out, s->avail_out,
			    s->token, s->i, &done))
	    {
	    case 1:
	      rsync_update(s, done);
	      goto do_token;
	    case 0:
	      rsync_update(s, done);
	      s->i += done;
	      break;
	    case -1:
	      return RSYNC_INPUT_ERROR;
	    default:
	      abort();
	    }
	}
	break;
	  
      do_checksum:
	/* i is number of octets read */
	s->i = 0;
	md5_final(&s->sum_md5);
	md5_digest(&s->sum_md5, s->buf);
	s->state = RSYNC_READ_CHECKSUM;
      case RSYNC_READ_CHECKSUM:
	if (!s->avail_in)
	  return progress ? RSYNC_PROGRESS : RSYNC_BUF_ERROR;

	if (GET() != s->buf[s->i++])
	  return RSYNC_INPUT_ERROR;

	if (s->i == RSYNC_SUM_SIZE)
	  {
	    s->state = RSYNC_READ_INVALID;
	    return RSYNC_DONE;
	  }
	break;

      default:
	abort();
      }
}

void
rsync_receive_init(struct rsync_receive_state *s)
{
  s->state = RSYNC_READ_TOKEN;
  s->i = 0;
}

/* checksum.c
 *
 * Calculate rsync specific checksums.
 *
 * $Id$
 */

#include "rsync.h"

/* NOTE: We do calculations module 2^16. We don't need to care about
 * overflow, as calculation with unsigned C variables is arithmetic
 * module 2^n for some n >= 16. */

#define CHAR_OFFSET 0

/* Update a checksum of a prefix of a block
 *
 * For input x_i, i = 0, ..., l-1, we calculate (modulo 2^16)
 *
 *   a_k = \sum_0^k x_i,
 *   b_k = \sum_0^k (l-i) x_i
 *
 * But in fact, we don't calculate all b_k, only the final
 * value b_{l-1}, and we have the identity (by changing order of summation)
 *
 *   b_{l-1} = \sum_0^{l-1} (l-i) x_i = \sum_0^{l-1} a_i
 *
 * So we keep track of the numbers c_k = \sum_0^k a_k rather than b_k. */

void
rsync_update_1(unsigned *ap, unsigned *cp,
	       UINT32 length, UINT8 *data)
{
  unsigned a = *ap;
  unsigned c = *cp;
  UINT32 i;
  
  for (i = 0; i<length; i++)
  {
    a += data[i] + CHAR_OFFSET;
    c += a;
  }

  *ap = a & 0xffff;
  *cp = c & 0xffff;
}

/* Updates checksum of a full block. START points to the beginning of the block,
 * END points to new data to be added. HASH points to a hash table.
 *
 * The function returns the hash node at a hash-tag hit, in which case
 * DONE is the number of octets that the hashed region was advanced (0
 * < *found <= length).
 *
 * If no tag hit occurs, return NULL and set DONE = LENGTH. This way,
 * *DONE will always reflect how far the region was advanced.
 *
 * Note that the region is always advanced at least one octet. */

struct rsync_node *
rsync_search(unsigned *ap, unsigned *bp, unsigned block_size,
	     UINT32 length, UINT8 *start, UINT8 *end,
	     UINT32 *done, struct rsync_node **hash)
{
  unsigned a = *ap;
  unsigned b = *bp;
  UINT32 i;
  struct rsync_node *n;

#if 0
  n = hash[(a ^ b) & 0xffff];
  if (n)
    {
      *found = 0;
      return n;
    }
#endif
  
  for (i = 0; i<length; )
    {
      /* Any non-zero CHAR_OFFSET cancels out here. I think. */

      a += end[i] - start[i];
      b += a - block_size * start[i];

      i++;
      n = hash[(a ^ b) & 0xffff];

      if (n)
	{
	  *done = i;
	  *ap = a & 0xffff;
	  *bp = a & 0xffff;
	  return n;
	}
    }
  *done = length;
  return NULL;
}

/*
 * MemPool.c
 * (c) 2000 Jean-Pierre Stierlin.
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

#ifndef	__MEMPOOL__
#define	__MEMPOOL__

#ifndef __MEMORY__
#include <Memory.h>
#endif

typedef struct MemPool {
	char					*f_Top;
	char					*f_Free;
	char					*f_HiMem;
	char					*f_LoMem;
	long					f_PoolSize;
	Handle					f_HandleRec;
	Handle					f_HandleMem;
	struct MemPool			*next;
	struct MemPool			*prev;
#if GETSTATS
	long					f_CurrSize;
	long					f_PeakSize;
#endif
} MemPool;

#ifdef __cplusplus
extern "C" {
#endif

	OSErr	MPInit(long allocsize, Ptr *priv, Ptr prev);
	void	MPDispose(Ptr priv);

	void	*MPmalloc(Ptr priv, long size);
	void	MPfree(Ptr priv, void *addr);
	long	MPsize(Ptr priv, void *addr);

#ifdef __cplusplus
}
#endif

#endif

/* resource.h
 *
 * External resources associated with a connection, for instance
 * processes and ports. Used to kill or release the resource in
 * question when the connection dies.
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

#ifndef LSH_RESOURCE_H_INCLUDED
#define LSH_RESOURCE_H_INCLUDED

#include "lsh.h"

#define GABA_DECLARE
#include "resource.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name resource)
     (vars
       ; Hack to check liveness before the resource gets gc:ed.
       ; Live resources should never be forgotten.
       (alive special int #f dont_free_live_resource)
       
       (kill method void)))
*/

#define KILL_RESOURCE(r) ((r)->kill((r)))

void
resource_init(struct resource *self,
	      void (*k)(struct resource *));

/* For the resource list. */

/* GABA:
   (class
     (name resource_list)
     (super resource)
     (vars
       (remember method void "struct resource *r")))
*/

#define REMEMBER_RESOURCE(l, r) ((l)->remember((l), (r)))
#define KILL_RESOURCE_LIST(l) KILL_RESOURCE(&(l)->super)

/* Allocates an empty list. */
struct resource_list *empty_resource_list(void);

#endif /* LSH_RESOURCE_H_INCLUDED */

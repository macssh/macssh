/* handshake.c
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, 2000 Niels Möller
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

#ifndef HANDSHAKE_H_INCLUDED
#define HANDSHAKE_H_INCLUDED

#include "alist.h"
#include "command.h"
#include "ssh1_fallback.h"

#define GABA_DECLARE
#include "handshake.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name handshake_info)
     (vars
       ; Initial value for the connection flags. Usually
       ; CONNECTION_SERVER or CONNECTION_CLIENT
       (flags . UINT32)
       
       (block_size . UINT32)
       (id_comment . "const char *")
       (debug_comment . "const char *")

       (random object randomness)
       (algorithms object alist)
       
       ; Used only on the server
       (fallback object ssh1_fallback)))
*/

struct handshake_info *
make_handshake_info(UINT32 flags,
		    const char *id_comment,
		    const char *debug_comment,
		    UINT32 block_size,
		    struct randomness *r,
		    struct alist *algorithms,
		    struct ssh1_fallback *fallback);

extern struct command_4 handshake_command;
#define CONNECTION_HANDSHAKE (&handshake_command.super.super)

#endif /* HANDSHAKE_H_INCLUDED */

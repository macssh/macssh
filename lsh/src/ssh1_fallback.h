/* ssh1_fallback.h
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

#ifndef LSH_SSH1_FALLBACK_H_INCLUDED
#define LSH_SSH1_FALLBACK_H_INCLUDED

#include "exception.h"

#define GABA_DECLARE
#include "ssh1_fallback.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name ssh1_fallback)
     (vars
       (fallback method void
                 "int fd" "UINT32 length" "const UINT8 *line"
		 "struct exception_handler *e")))
*/

#define SSH1_FALLBACK(f, fd, length, line, e) \
((f)->fallback((f), (fd), (length), (line), (e)))
     
struct ssh1_fallback *make_ssh1_fallback(char *sshd1);

#endif /* LSH_SSH1_FALLBACK_H_INCLUDED */

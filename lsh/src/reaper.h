/* reaper.h
 *
 * Handle child processes.
 *
 * $Id$
 */

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

#ifndef LSH_REAPER_H_INCLUDED
#define LSH_REAPER_H_INCLUDED

#include "io.h"

#define GABA_DECLARE
#include "reaper.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name exit_callback)
     (vars
       (exit method void "int signaled" "int core" "int value")))
*/

#define EXIT_CALLBACK(e, s, c, v) ((e)->exit((e), (s), (c), (v)))

/* GABA:
   (class
     (name reap)
     (vars
       ; Use callback == NULL to cancel a previous callback.
       (reap method void "pid_t pid" "struct exit_callback *callback")))
*/

#define REAP(r, p, c) ((r)->reap((r), (p), (c)))

struct reap *make_reaper(struct io_backend *b);

#endif /* LSH_REAPER_H_INCLUDED */

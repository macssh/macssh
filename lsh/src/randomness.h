/* randomness.h
 *
 *
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

#ifndef LSH_RANDOMNESS_H_INCLUDED
#define LSH_RANDOMNESS_H_INCLUDED

#include "abstract_crypto.h"

#include "exception.h"

#define GABA_DECLARE
#include "randomness.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name randomness)
     (vars
       (quality . int)
       (random method void "UINT32 length" "UINT8 *dst")))
*/

#define RANDOM(r, length, dst) ((r)->random((r), length, dst))

/* A class polling the environment for randomness. Following Peter
 * Gutmann's ideas, there are two methods for a slow more thorough
 * poll done at startup, and a faster poll performed from time to time
 * as a generator is used. */

/* GABA:
   (class
     (name random_poll)
     (vars
       ; Both functions return an entropy estimate, and adds the
       ; randomness to the given hash instance.
       (slow method unsigned "struct hash_instance *")
       (fast method unsigned "struct hash_instance *")
       (background method void)))
*/

#define RANDOM_POLL_SLOW(p, h) ((p)->slow((p), (h)))
#define RANDOM_POLL_FAST(p, h) ((p)->fast((p), (h)))
#define RANDOM_POLL_BACKGROUND(p) ((p)->background((p)))

/* GABA:
   (class
     (name randomness_with_poll)
     (super randomness)
     (vars
       ; Object that gets randomness from the environment
       (poller object random_poll)))
*/

/* Consumes the init string (which may be NULL). */
struct randomness *make_poor_random(struct hash_algorithm *hash,
				    struct lsh_string *init);

struct randomness *make_bad_random(void);

struct randomness *make_device_random(const char *device);
struct randomness *make_reasonably_random(void);

struct randomness_with_poll *
make_arcfour_random(struct random_poll *poller,
		    struct hash_algorithm *hash,
		    struct exception_handler *e);

struct randomness_with_poll *
make_default_random(struct reap *reaper,
		    struct exception_handler *e);

struct random_poll *
make_unix_random(struct reap *reaper);

#endif /* LSH_RANDOMNESS_H_INCLUDED */

/* xalloc.h
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

#ifndef LSH_XALLOC_H_INCLUDED
#define LSH_XALLOC_H_INCLUDED

#include "gc.h"

#include <stdlib.h>

/* Allocation */

/* The memory allocation model (for strings) is as follows:
 *
 * Packets are allocated when they are needed. A packet may be passed
 * through a chain of processing functions, until it is finally
 * discarded or transmitted, at which time it is deallocated.
 * Processing functions may deallocate their input packets and
 * allocate fresh packets to pass on; therefore, any data from a
 * packet that is needed later must be copied into some other storage.
 *
 * At any time, each packet is own by a a particular processing
 * function. Pointers into a packet are valid only while you own it.
 * */

#if DEBUG_ALLOC
extern struct lsh_string *all_strings;

struct lsh_string *lsh_string_alloc_clue(UINT32 size, const char *clue);

#define lsh_string_alloc(size) \
  (lsh_string_alloc_clue((size), (__FILE__ ":" STRING_LINE ": " FUNCTION_NAME)))

#else /* !DEBUG_ALLOC */
struct lsh_string *lsh_string_alloc(UINT32 size);
#endif /* !DEBUG_ALLOC */

void
lsh_string_free(const struct lsh_string *packet);

struct lsh_object *lsh_object_alloc(struct lsh_class *class);

/* FIXME: Should take a const struct lsh_object. */
void lsh_object_free(struct lsh_object *o);

/* NOTE: This won't work for if there are strings or other instance
 * variables that can't be shared. */
struct lsh_object *lsh_object_clone(struct lsh_object *o);

void *lsh_space_alloc(size_t size);
void lsh_space_free(const void *p);

#if DEBUG_ALLOC

struct lsh_object *lsh_object_check(struct lsh_class *class,
				    struct lsh_object *instance);
struct lsh_object *lsh_object_check_subtype(struct lsh_class *class,
					    struct lsh_object *instance);

#define CHECK_TYPE(c, i) \
  lsh_object_check(&CLASS(c), (struct lsh_object *) (i))
#define CHECK_SUBTYPE(c, i) \
  lsh_object_check_subtype(&CLASS(c), (struct lsh_object *) (i))

#define CAST(class, var, o) \
  struct class *(var) = (struct class *) CHECK_TYPE(class, o)

#define CAST_SUBTYPE(class, var, o) \
  struct class *(var) = (struct class *) CHECK_SUBTYPE(class, o)
   
extern unsigned number_of_strings;

#else   /* !DEBUG_ALLOC */

#define CHECK_TYPE(c, o) ((struct lsh_object *)(o))
#define CHECK_SUBTYPE(c, o) ((struct lsh_object *)(o))
     
#define CAST(class, var, o) \
   struct class *(var) = (struct class *) (o)

#define CAST_SUBTYPE(class, var, o) CAST(class, var, o)

     
#endif  /* !DEBUG_ALLOC */

#define NEW(class, var) \
  struct class *(var) = (struct class *) lsh_object_alloc(&CLASS(class))
#define NEW_SPACE(x) ((x) = lsh_space_alloc(sizeof(*(x))))

#define CLONE(class, i) \
  ((struct class *) lsh_object_clone(CHECK_TYPE(class, (i))))

#define CLONED(class, var, i) \
  struct class *(var) = CLONE(class, i)
     
#define KILL(x) gc_kill((struct lsh_object *) (x))

#endif /* LSH_XALLOC_H_INCLUDED */

/* gc.c
 *
 * Simple mark&sweep garbage collector.
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

#include "gc.h"

#include "werror.h"
#include "xalloc.h"

#include <assert.h>

/* Global variables */
#ifndef MACOS
static struct lsh_object *all_objects = NULL;
static unsigned number_of_objects = 0;
static unsigned live_objects = 0;
#else
#include "lsh_context.h"
#endif

#if DEBUG_ALLOC
static void sanity_check_object_list(void)
{
  unsigned i = 0;
  struct lsh_object *o;

#if 0
  wwrite("sanity_check_object_list: Objects on list:\n");
  for(o = all_objects; o; o = o->next)
    werror("  %xi, class: %z\n", (UINT32) o, o->isa ? o->isa->name : "UNKNOWN");
#endif
  
  for(o = all_objects; o; o = o->next)
    i++;

  if (i != number_of_objects)
    fatal("sanity_check_object_list: Found %i objects, expected %i.\n",
	  i, number_of_objects);
}
#else
#define sanity_check_object_list()
#endif

/* FIXME: This function recurses heavily. One could use some trickery
 * to emulate tail recursion, which would help marking linked lists.
 * Or one could use some more efficient datastructures than the C
 * stack for keeping track of the marked but not yet traced objects.
 * Or use something more sophisticated, like pointer reversal. */
static void gc_mark(struct lsh_object *o)
{
  if (!o)
    return;
  
  switch(o->alloc_method)
    {
    case LSH_ALLOC_STACK:
      fatal("gc_mark: Unexpected stack object!\n");

    case LSH_ALLOC_HEAP:
      if (o->marked)
	return;
      o->marked = 1;
      /* Fall through */
    case LSH_ALLOC_STATIC:
      /* Can't use mark bit on static objects, as there's no way to
       * reset all the bits */
      assert(!o->dead);
      {
	struct lsh_class *class;

#if 0
	debug("gc_mark: Marking object of class '%z'\n",
	      o->isa ? o->isa->name : "UNKNOWN");
#endif
	
	for (class = o->isa; class; class = class->super_class)
	  {
	    if (class->mark_instance)
	      MARK_INSTANCE(class, o, gc_mark);
	  }
      }
      break;
    default:
      fatal("gc_mark: Memory corrupted!\n");
    }
}

static void gc_sweep(void)
{
  struct lsh_object *o;
  struct lsh_object **o_p;

  live_objects = 0;
  
  for(o_p = &all_objects; (o = *o_p); )
    {
      if (o->marked)
	{
	  /* Paralyze the living... */
	  live_objects++;
	  o->marked = 0;
	}
      else
	{
	  /* ... and resurrect the dead. */
	  struct lsh_class *class;

#if 0
	  debug("gc_sweep: Freeing object of class '%z'\n",
		o->isa->name);
#endif  
	  for (class = o->isa; class; class = class->super_class)
	    if (class->free_instance)
	      FREE_INSTANCE(class, o);

	  *o_p = o->next;
	  number_of_objects--;
	  
	  lsh_object_free(o);
	  continue;
	}
      o_p = &o->next;
    }
  assert(live_objects == number_of_objects);
}

void gc_register(struct lsh_object *o)
{
  sanity_check_object_list();

  o->marked = o->dead = 0;
  o->next = all_objects;
  all_objects = o;

  number_of_objects ++;

  sanity_check_object_list();
}

/* NOTE: If the object is close to the start of the object list, it is
 * deallocated and forgotten immedietely. If the object is not found,
 * we don't search the entire list, but instead defer deallocation to
 * gc_sweep(). */
void gc_kill(struct lsh_object *o)
{
  sanity_check_object_list();

  if (!o)
    return;

  assert(!o->dead);

  o->dead = 1;

#if 0
  debug("gc_kill: Killing object of type %z.\n",
	o->isa ? o->isa->name : "UNKNOWN");
#endif
  
  if (o == all_objects)
    {
      struct lsh_class *class;
      
#if 0
      debug("gc_kill:   Deallocating immediately.\n");
#endif
      
      for (class = o->isa; class; class = class->super_class)
	if (class->free_instance)
	  FREE_INSTANCE(class, o);

      all_objects = o->next;
      number_of_objects--;
      lsh_object_free(o);
    }
  else
    {
#if 0
      debug("gc_kill:   Deferring deallocation to gc_sweep()\n");
#endif
    }
  
  sanity_check_object_list();
}

void gc(struct lsh_object *root)
{
  unsigned before = number_of_objects;

  gc_mark(root);  
  gc_sweep();
  
  verbose("Objects alive: %i, garbage collected: %i\n", live_objects,
	  before - live_objects);
}

void gc_maybe(struct lsh_object *root, int busy)
{
  sanity_check_object_list();

  if (number_of_objects > (100 + live_objects*(2+busy)))
    {
      verbose("Garbage collecting while %z...\n", busy ? "busy" : "idle");
      gc(root);
    }
}

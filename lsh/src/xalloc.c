/* xalloc.c
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

#include "xalloc.h"

#include "list.h"
#include "werror.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#if DEBUG_ALLOC

#define lsh_free debug_free
#define lsh_malloc debug_malloc

/* There are two sets of allocation functions: Low level allocation *
 * that can allocate memory for any purpose, and object allocators
 * that assume that the allocated object begins with a type field. */

/* UNIT should be a type of a size that is a multiple of the alignment
 * requirement of the machine. */

/* NOTE: The code breaks horribly if UNIT is of the wrong size. But it
 * doesn't matter much if we guess wrong on some platforms, as this
 * affects only optionalal debug code. */

#define UNIT unsigned long 

#define SIZE_IN_UNITS(x) (((x) + sizeof(UNIT)-1) / sizeof(UNIT))

static void *debug_malloc(size_t real_size)
{
  static int count = 4711;
  UNIT *res;
  UNIT size = SIZE_IN_UNITS(real_size);
  
  res = malloc((size + 3)*sizeof(UNIT));

  if (!res)
    return NULL;

  res += 2;
  
  res[-2] = count;
  res[-1] = real_size;
  res[size] = ~count;
  count++;

  return (void *) res;
}

static void
debug_free(const void *m)
{
  if (m)
    {
      UNIT *p = (UNIT *) m;
      UNIT real_size = p[-1];
      UNIT size = SIZE_IN_UNITS(real_size);
      
      if (~p[-2] != p[size])
	fatal("Memory corrupted!\n");
      
      p[-2] = p[size] = 0;
      
      free(p-2);
    }
}

#else  /* !DEBUG_ALLOC */

/* ANSI-C free doesn't allow const pointers to be freed. (That is one
 * of the few things that C++ gets right). */
#define lsh_free(p) free((void *) (p))
#define lsh_malloc malloc

#endif /* !DEBUG_ALLOC */

static void *xalloc(size_t size)
{
  void *res = lsh_malloc(size);
  if (!res)
    fatal("Virtual memory exhausted");

  /* FIXME: The gc can't handle uninitialized pointers. The simple way
   * is to zero-fill all memory as it is allocated. But initialization
   * is only necessary for objects, strings need no initialization. By
   * moving initializing to some higher level, we could avoid
   * unnecessary clearing, and also initialize mpz objects
   * automatically. */
  memset(res, 0, size);

  return res;
}

#if DEBUG_ALLOC
#undef lsh_string_alloc
static
#endif

struct lsh_string *
lsh_string_alloc(UINT32 length)
{
  /* NOTE: The definition of the struct contains a char array of
   * length 1, so the below includes space for a terminating NUL. */
  
  struct lsh_string *s
    = xalloc(sizeof(struct lsh_string) + length);

  s->length = length;
  s->data[length] = '\0';
  s->sequence_number = 0;
  
  return s;
}

#if DEBUG_ALLOC

unsigned number_of_strings = 0;
struct lsh_string *all_strings = NULL;

static void sanity_check_string_list(void)
{
  unsigned i = 0;
  struct lsh_string *s;

  if (!all_strings)
    {
      assert(!number_of_strings);
      return;
    }
  assert(!all_strings->header.prev);
  
  for(i = 0, s = all_strings; s; s = s->header.next, i++)
    {
      if (s->header.next)
	{
	  assert(s->header.next->header.prev = s);
	}
    }
  assert (i == number_of_strings);
}

struct lsh_string *
lsh_string_alloc_clue(UINT32 length, const char *clue)
{
  struct lsh_string *s = lsh_string_alloc(length);

  sanity_check_string_list();
  
  s->header.magic = -1717;
  number_of_strings++;

  s->header.clue = clue;
  s->header.next = all_strings;
  s->header.prev = NULL;
  if (s->header.next)
    s->header.next->header.prev = s;
  all_strings = s;

  sanity_check_string_list();

  return s;
}
#endif

void
lsh_string_free(const struct lsh_string *s)
{
  if (!s)
    return;

#if DEBUG_ALLOC
  sanity_check_string_list();

  assert(number_of_strings);
  number_of_strings--;

  if (s->header.magic != -1717)
    fatal("lsh_string_free: Not string!\n");
  if (s->data[s->length])
    fatal("lsh_string_free: String not NUL-terminated.\n");

  if (s->header.next)
    s->header.next->header.prev = s->header.prev;
  
  if (s->header.prev)
    s->header.prev->header.next = s->header.next;
  else
    {
      assert (all_strings == s);
      all_strings = s->header.next;
    }
  
  sanity_check_string_list();	
#endif

#if 0
  debug("lsh_string_free: freeing %xi,\n", (UINT32) s);
#endif
  
  lsh_free(s);
}

struct lsh_object *
lsh_object_alloc(struct lsh_class *class)
{
  struct lsh_object *instance = xalloc(class->size);
  instance->isa = class;
  instance->alloc_method = LSH_ALLOC_HEAP;

  gc_register(instance);

  return instance;
}

struct lsh_object *
lsh_object_clone(struct lsh_object *o)
{
  struct lsh_object *i = xalloc(o->isa->size);

  /* Copy header and all instance variables. Note that the header is
   * now invalid, as the next pointer can't be copied directly. This
   * is fixed by the gc_register call below. */
  memcpy(i, o, o->isa->size);

  i->alloc_method = LSH_ALLOC_HEAP;
  gc_register(i);

  return i;
}

struct list_header *
lsh_list_alloc(struct lsh_class *class,
				   unsigned length, size_t element_size)
{
  struct list_header *list = xalloc(class->size
				    + element_size * length
				    - element_size);

  assert(element_size < 1024);
  /* assert(length < 65536); */
  
  list->super.isa = class;
  list->super.alloc_method = LSH_ALLOC_HEAP;

  list->length = length;
  
  gc_register(&list->super);

  return list;
}
    
/* Should be called *only* by the gc */
void
lsh_object_free(struct lsh_object *o)
{
  if (!o)
    return;
  
  if (o->alloc_method != LSH_ALLOC_HEAP)
    fatal("lsh_object_free: Object not allocated on the heap!\n");

#if 0
  if (o->isa->free_instance)
    o->isa->free_instance(o);
#endif
  
  lsh_free(o);
}

#if DEBUG_ALLOC
struct lsh_object *lsh_object_check(struct lsh_class *class,
				    struct lsh_object *instance)
{
  if (!instance)
    return instance;
  
  if (instance->marked)
    fatal("lsh_object_check: Unexpected marked object!\n");

  if (instance->dead)
    fatal("lsh_object_check: Reference to dead object!\n");

  if ( (instance->alloc_method == LSH_ALLOC_HEAP)
       && (instance->isa != class))
    fatal("lsh_object_check: Type error!\n");

  return instance;
}

struct lsh_object *lsh_object_check_subtype(struct lsh_class *class,
					    struct lsh_object *instance)
{
  struct lsh_class *type;
  
  if (!instance)
    return instance;

  if (instance->marked)
    fatal("lsh_object_check: Unexpected marked object!\n");

  if (instance->dead)
    fatal("lsh_object_check: Reference to dead object!\n");

  /* Only heap allocated objects have a valid isa-pointer */
  switch (instance->alloc_method)
    {
    case LSH_ALLOC_STATIC:
    case LSH_ALLOC_STACK:
      return instance;
    case LSH_ALLOC_HEAP:
      break;
    default:
      fatal("lsh_object_check_subtype: Memory corrupted!\n");
    }
  
  for (type = instance->isa; type; type=type->super_class)
    if (type == class)
      return instance;

  fatal("lsh_object_check_subtype: Type error!\n");
}
#endif /* DEBUG_ALLOC */

#if DEBUG_ALLOC
void *lsh_space_alloc(size_t size)
{
  UNIT *p = xalloc(size + sizeof(UNIT));

  *p = -1919;

  return (void *) (p + 1);
}

void lsh_space_free(const void *p)
{
  UNIT *m;
  
  if (!p)
    return;
  
  m = (UNIT *) p;
  
  if (m[-1] != (UNIT) -1919)
    fatal("lsh_free_space: Type error!\n");

  lsh_free(m-1);
}

#else /* !DEBUG_ALLOC */

/* FIXME: Why not use macros for this? */
void *lsh_space_alloc(size_t size)
{
  return lsh_malloc(size);
}

void lsh_space_free(const void *p)
{
  lsh_free(p);
}

#endif /* !DEBUG_ALLOC */

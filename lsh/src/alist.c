/* alist.c
 *
 * Associations are implemented as linear tables.
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
 

#include "alist.h"

#include "atoms.h"
#include "list.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#define GABA_DEFINE
#include "alist.h.x"
#undef GABA_DEFINE

struct alist *
alist_addv(struct alist *a, unsigned n, va_list args)
{
  unsigned i;
  
  for (i=0; i<n; i++)
    {
      int atom = va_arg(args, int);
      struct lsh_object *value = va_arg(args, struct lsh_object *);

      if (atom < 0)
	fatal("Internal error!\n");
      ALIST_SET(a, atom, value);
    }

  assert(va_arg(args, int) == -1);

  return a;
}

struct alist
*alist_add(struct alist *a, unsigned n, ...)
{
  va_list args;

  va_start(args, n);
  alist_addv(a, n, args);
  va_end(args);

  return a;
}

struct alist_node
{
  struct alist_node *next;
  int atom;
  struct lsh_object *value;
};

/* Prototypes */

static struct lsh_object *
do_linear_get(struct alist *c, int atom);
static void
do_linear_set(struct alist *c, int atom, struct lsh_object *value);

static void
do_mark_list(struct alist_node *n,
	     void (*mark)(struct lsh_object *o));
static void
do_free_list(struct alist_node *n);

static struct lsh_object *
do_linked_get(struct alist *c, int atom);
static void
do_linked_set(struct alist *c, int atom, struct lsh_object *value);

#include "alist.c.x"

/* NOTE: For a linear alist, all keys must be non-negative and less
 * than NUMBER_OF_ATOMS. */

/* GABA:
   (class
     (name alist_linear)
     (super alist)
     (meta alist)
     (vars
       (table array (object lsh_object) NUMBER_OF_ATOMS))
     (methods do_linear_get do_linear_set))
*/

static struct lsh_object *
do_linear_get(struct alist *c, int atom)
{
  CAST(alist_linear, self, c);

  assert(atom >= 0);
  assert(atom < NUMBER_OF_ATOMS);
  
  return self->table[atom];
}

static void
do_linear_set(struct alist *c, int atom, struct lsh_object *value)
{
  CAST(alist_linear, self, c);
  
  assert(atom >= 0);
  assert(atom < NUMBER_OF_ATOMS);
  
  self->super.size += !self->table[atom] - !value; 
  
  self->table[atom] = value;
}

struct alist *
make_linear_alist(unsigned n, ...)
{
  int i;
  va_list args;
  
  NEW(alist_linear, res);

  res->super.size = 0;
  
  for (i = 0; i<NUMBER_OF_ATOMS; i++)
    res->table[i] = NULL;

  va_start(args, n);

  alist_addv(&res->super, n, args);
  va_end(args);
  
  return &res->super;
}

/* NOTE: A linked alist does not have any limit on the size of its keys. */

/* GABA:
   (class
     (name alist_linked)
     (super alist)
     (meta alist)
     (vars
       (head special "struct alist_node *"
             do_mark_list do_free_list))
     (methods do_linked_get do_linked_set))
*/

static void
do_mark_list(struct alist_node *n,
	     void (*mark)(struct lsh_object *o))
{
  while(n)
    {
      mark(n->value);
      n = n->next;
    }
}

static void
do_free_list(struct alist_node *n)
{
  while(n)
    {
      struct alist_node *old = n;
      n = n->next;
      lsh_space_free(old);
    }
}

static struct lsh_object *
do_linked_get(struct alist *c, int atom)
{
  CAST(alist_linked, self, c);
  struct alist_node *p;
  
  assert(atom >= 0);
  
  for (p = self->head; p; p = p->next)
    if (p->atom == atom)
      return p->value;
  
  return NULL;
}

static void
do_linked_set(struct alist *c, int atom, struct lsh_object *value)
{
  CAST(alist_linked, self, c);
  
  if (value)
    {
      struct alist_node *p;

      for (p = self->head; p; p = p->next)
	if (p->atom == atom)
	  {
	    p->value = value;
	    return;
	  }

      NEW_SPACE(p);
      p->next = self->head;
      p->atom = atom;
      p->value = value;

      self->head = p;

      self->super.size++;
    }
  else
    { /* Remove atom */
      struct alist_node **p;

      for(p = &self->head; *p; )
	{
	  struct alist_node *o = *p;
	  if (o->atom == atom)
	    {
	      *p = o->next;
	      lsh_space_free(o);

	      self->super.size--;
	      return;
	    }
	  p = &o->next;
	}
      /* Not found */
    }
}

struct alist *
make_linked_alist(unsigned n, ...)
{
  va_list args;
  
  struct alist *res;
  
  NEW(alist_linked, self);
  res = &self->super;

  res->size = 0;

  self->head = NULL;

  va_start(args, n);

  alist_addv(res, n, args);
  va_end(args);
    
  return res;
}

/* Copies selected elements from one alist to another. */

unsigned
alist_select(struct alist *dst, struct alist *src,
	     struct int_list *names)
{
  unsigned selected;
  unsigned i;
  for (i = 0, selected = 0; i < LIST_LENGTH(names); i++)
    {
      int atom = LIST(names)[i];
      struct lsh_object *o = ALIST_GET(src, atom);
      if (o)
	{
	  ALIST_SET(dst, atom, o);
	  selected ++;
	}
    }
  return selected;
}

unsigned
alist_select_l(struct alist *dst, struct alist *src,
	       unsigned n, ...)
{
  va_list args;
  struct int_list *names;
  unsigned selected;
  
  va_start(args, n);
  names = make_int_listv(n, args);
  va_end(args);

  selected = alist_select(dst, src, names);

  KILL(names);

  return selected;
}
  

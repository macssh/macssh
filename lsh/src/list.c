/* list.h
 *
 * Variable length lists of atoms (or other integers).
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

#include "list.h"

#include <assert.h>
#include <stdarg.h>

#define GABA_DEFINE
#include "list.h.x"
#undef GABA_DEFINE

#include "xalloc.h"

struct int_list *make_int_listv(unsigned n, va_list args)
{
  unsigned i;
  
  struct int_list *l = alloc_int_list(n);

  for (i=0; i<n; i++)
    {
      int atom = va_arg(args, int);
      assert(atom >= 0);
      LIST(l)[i] = atom;
    }

  assert(va_arg(args, int) == -1);

  return l;
}

struct int_list *make_int_list(unsigned n, ...)
{
  va_list args;
  struct int_list *l;
  
  va_start(args, n);
  l = make_int_listv(n, args);
  va_end(args);

  return l;
}

struct object_list *make_object_listv(unsigned n, va_list args)
{
  unsigned i;
  
  struct object_list *l = alloc_object_list(n);

  /* FIXME: Add sanity type checking */
  for (i=0; i<n; i++)
    LIST(l)[i] = va_arg(args, struct lsh_object *);

  assert(va_arg(args, int) == -1);

  return l;
}


struct object_list *make_object_list(unsigned n, ...)
{
  va_list args;
  struct object_list *l;

  va_start(args, n);
  l = make_object_listv(n, args);
  va_end(args);

  return l;
}


/* queue.c 
 *
 * $Id$
 *
 * Generic doubly linked list. */

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

#include "queue.h"

#include "list.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

/* Prototypes */
static void do_object_queue_mark(struct lsh_queue *q,
				 void (*mark)(struct lsh_object *o));
static void do_object_queue_free(struct lsh_queue *q);
static void do_string_queue_free(struct lsh_queue *q);

#define GABA_DEFINE
#include "queue.h.x"
#undef GABA_DEFINE

/* Short cuts */
#define next np_links[LSH_QUEUE_NEXT]
#define prev np_links[LSH_QUEUE_PREV]

#define head ht_links[LSH_QUEUE_HEAD]
#define tail ht_links[LSH_QUEUE_TAIL]
#define tailprev ht_links[LSH_QUEUE_TAILPREV]

#define EMPTYP(q) ((q)->tailprev == (struct lsh_queue_node *) &(q)->head)

/* #define EMPTYP(q) (!(q)->length) */

#if DEBUG_ALLOC
static void sanity_check_queue(struct lsh_queue *q)
{
  struct lsh_queue_node *n;

#if 0
  UINT32 count;
#endif
  
#if 0
  debug("sanity_check_queue: q = %xi\n", (UINT32) q);
#endif

#if 0
  if (EMPTYP(q) != EMPTYP_1(q))
    fatal("inconsistent emptyness!\n");

  count = 0;
#endif
  
  if (q->tail)
    fatal("sanity_check_queue: q->tail not NULL!\n");

  n = q->head;

  if (n->prev != (struct lsh_queue_node *) &q->head)
    fatal("sanity_check_queue: head->prev != &q->head !\n");

  while (n->next)
    {
#if 0
      debug("  n = %xi\n", (UINT32) n);
#endif 
      if (n->prev->next != n)
	fatal("n->prev->next != n !\n");

      n = n->next;
#if 0
      count++;
#endif
    }
  if (n != (struct lsh_queue_node *) &(q->tail))
    fatal("n != n &t->tail!\n");

#if 0
  if (count != (q->length + 1))
    fatal("incorrect length!\n");
#endif
  
}
#else
#define sanity_check_queue(x)
#endif

void lsh_queue_init(struct lsh_queue *q)
{
  q->head = (struct lsh_queue_node *) &(q->tail);
  q->tail = NULL;
  q->tailprev = (struct lsh_queue_node *) &(q->head);
  sanity_check_queue(q);
}

int lsh_queue_is_empty(struct lsh_queue *q)
{
  sanity_check_queue(q);
  return EMPTYP(q);
}

void lsh_queue_add_head(struct lsh_queue *q, struct lsh_queue_node *n)
{
  sanity_check_queue(q);
  n->next = q->head;
  n->prev = (struct lsh_queue_node *) &(q->head);
  n->prev->next = n;
  n->next->prev = n;

  sanity_check_queue(q);
}

void lsh_queue_add_tail(struct lsh_queue *q, struct lsh_queue_node *n)
{
  sanity_check_queue(q);
  n->next = (struct lsh_queue_node *) &(q->tail);
  n->prev = q->tailprev;
  n->prev->next = n;
  n->next->prev = n;

  sanity_check_queue(q);
}

void lsh_queue_remove(struct lsh_queue_node *n)
{
  assert(n->next);
  assert(n->prev);
  n->next->prev = n->prev;
  n->prev->next = n->next;
}

struct lsh_queue_node *lsh_queue_remove_head(struct lsh_queue *q)
{
  struct lsh_queue_node *n = q->head;

  sanity_check_queue(q);
  assert(!EMPTYP(q));
  lsh_queue_remove(n);
  sanity_check_queue(q);

  return n;
}

struct lsh_queue_node *lsh_queue_remove_tail(struct lsh_queue *q)
{
  struct lsh_queue_node *n = q->tailprev;
  
  sanity_check_queue(q);
  assert(!EMPTYP(q));
  lsh_queue_remove(n);
  sanity_check_queue(q);

  return n;
}

struct lsh_queue_node *lsh_queue_peek_head(struct lsh_queue *q)
{
  return EMPTYP(q) ? NULL : q->head;
}

struct lsh_queue_node *lsh_queue_peek_tail(struct lsh_queue *q)
{
  return EMPTYP(q) ? NULL : q->tailprev;
}


/* object_queue */
static struct object_queue_node *
make_object_queue_node(struct lsh_object *o)
{
  struct object_queue_node *n;

  NEW_SPACE(n);
  n->o = o;

  return n;
}

void object_queue_init(struct object_queue *q)
{
  lsh_queue_init(&q->q);
  q->length = 0;
}

int object_queue_is_empty(struct object_queue *q)
{
  assert(EMPTYP(&q->q) == !q->length);

  return !q->length;
}

void object_queue_add_head(struct object_queue *q, struct lsh_object *o)
{
  lsh_queue_add_head(&q->q, &make_object_queue_node(o)->header);
  q->length++;
}

void object_queue_add_tail(struct object_queue *q, struct lsh_object *o)
{
  lsh_queue_add_tail(&q->q, &make_object_queue_node(o)->header);
  q->length++;
}

static struct lsh_object *
object_queue_get_contents(struct lsh_queue_node *l)
{
  struct object_queue_node *n = (struct object_queue_node *) l;
  
  struct lsh_object *res = n->o;
  lsh_space_free(n);

  return res;
}

static struct lsh_object *
object_queue_peek(struct lsh_queue_node *n)
{
  return ( (struct object_queue_node *) n)->o;
}

struct lsh_object *object_queue_remove_head(struct object_queue *q)
{
  q->length--;
  return object_queue_get_contents(lsh_queue_remove_head(&q->q));
}

struct lsh_object *object_queue_remove_tail(struct object_queue *q)
{
  q->length--;
  return object_queue_get_contents(lsh_queue_remove_tail(&q->q));
}

struct lsh_object *object_queue_peek_head(struct object_queue *q)
{
  return EMPTYP(&q->q) ? NULL : object_queue_peek(q->q.head);
}

struct lsh_object *object_queue_peek_tail(struct object_queue *q)
{
  return EMPTYP(&q->q) ? NULL : object_queue_peek(q->q.tailprev);
}

struct object_list *queue_to_list(struct object_queue *q)
{
  struct object_list *l = alloc_object_list(q->length);
  UINT32 i = 0;

  FOR_OBJECT_QUEUE(q, n)
    {
      LIST(l)[i++] = n;
    }
  assert(i == q->length);

  return l;
}

struct object_list *queue_to_list_and_kill(struct object_queue *q)
{
  struct object_list *l = queue_to_list(q);
  object_queue_kill(q);

  return l;
}

/* For gc */
static void do_object_queue_mark(struct lsh_queue *q,
				 void (*mark)(struct lsh_object *o))
{
  FOR_QUEUE(q, struct object_queue_node *, n)
    mark(n->o);
}

static void do_object_queue_free(struct lsh_queue *q)
{
  FOR_QUEUE(q, struct object_queue_node *, n)
    lsh_space_free(n);
}

void object_queue_kill(struct object_queue *q)
{
  do_object_queue_free(&q->q);
}


/* String queues */

static struct string_queue_node *
make_string_queue_node(struct lsh_string *s)
{
  struct string_queue_node *n;

  NEW_SPACE(n);
  n->s = s;

  return n;
}

void string_queue_init(struct string_queue *q)
{
  lsh_queue_init(&q->q);
  q->length = 0;
}

int string_queue_is_empty(struct string_queue *q)
{
  assert(EMPTYP(&q->q) == !q->length);

  return !q->length;
}

void string_queue_add_head(struct string_queue *q, struct lsh_string *s)
{
  lsh_queue_add_head(&q->q, &make_string_queue_node(s)->header);
  q->length++;
}

void string_queue_add_tail(struct string_queue *q, struct lsh_string *s)
{
  lsh_queue_add_tail(&q->q, &make_string_queue_node(s)->header);
  q->length++;
}

static struct lsh_string *
string_queue_get_contents(struct lsh_queue_node *l)
{
  struct string_queue_node *n = (struct string_queue_node *) l;
  
  struct lsh_string *res = n->s;
  lsh_space_free(n);

  return res;
}

static struct lsh_string *
string_queue_peek(struct lsh_queue_node *n)
{
  return ( (struct string_queue_node *) n)->s;
}

struct lsh_string *string_queue_remove_head(struct string_queue *q)
{
  q->length--;
  return string_queue_get_contents(lsh_queue_remove_head(&q->q));
}

struct lsh_string *string_queue_remove_tail(struct string_queue *q)
{
  q->length--;
  return string_queue_get_contents(lsh_queue_remove_tail(&q->q));
}

struct lsh_string *string_queue_peek_head(struct string_queue *q)
{
  return EMPTYP(&q->q) ? NULL : string_queue_peek(q->q.head);
}

struct lsh_string *string_queue_peek_tail(struct string_queue *q)
{
  return EMPTYP(&q->q) ? NULL : string_queue_peek(q->q.tailprev);
}

static void do_string_queue_free(struct lsh_queue *q)
{
  FOR_QUEUE(q, struct string_queue_node *, n)
    {
      lsh_string_free(n->s);
      lsh_space_free(n);
    }
}


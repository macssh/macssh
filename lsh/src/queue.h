/* queue.h
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

#ifndef LSH_QUEUE_H_INCLUDED
#define LSH_QUEUE_H_INCLUDED

#include "lsh.h"

/* Layout taken from AmigaOS lists... The first node uses a prev
 * pointer that points to the queue's HEAD. The last node uses a next
 * pointer that points to the queue's TAIL field. The TAIL field is
 * always NULL; TAILPREV points to the last node in the queue. */
struct lsh_queue_node
{
  struct lsh_queue_node *np_links[2];
};
#define LSH_QUEUE_NEXT 0
#define LSH_QUEUE_PREV 1

struct lsh_queue
{
  struct lsh_queue_node *ht_links[3];
};
#define LSH_QUEUE_HEAD 0
#define LSH_QUEUE_TAIL 1
#define LSH_QUEUE_TAILPREV 2

/* This macro must be used at the start of a block, to make the
 * declarations legal. It is allowed to free n inside the loop. */

#define FOR_QUEUE(q, type, n)					\
  struct lsh_queue_node *n##_this, *n##_next;			\
  type n;							\
  for ( n##_this = (q)->ht_links[LSH_QUEUE_HEAD];		\
	( n = (type) n##_this,					\
	  (n##_next = n##_this->np_links[LSH_QUEUE_NEXT]));	\
	n##_this = n##_next)

void lsh_queue_init(struct lsh_queue *q);
int lsh_queue_is_empty(struct lsh_queue *q);
void lsh_queue_add_head(struct lsh_queue *q, struct lsh_queue_node *n);
void lsh_queue_add_tail(struct lsh_queue *q, struct lsh_queue_node *n);
void lsh_queue_remove(struct lsh_queue_node *n);
struct lsh_queue_node *lsh_queue_remove_head(struct lsh_queue *q);
struct lsh_queue_node *lsh_queue_remove_tail(struct lsh_queue *q);

struct lsh_queue_node *lsh_queue_peek_head(struct lsh_queue *q);
struct lsh_queue_node *lsh_queue_peek_tail(struct lsh_queue *q);

#define GABA_DECLARE
#include "queue.h.x"
#undef GABA_DECLARE

/* Object queue */
struct object_queue_node
{
  struct lsh_queue_node header;
  struct lsh_object *o;
};

/* GABA:
   (struct
     (name object_queue)
     (vars
       (length . UINT32)
       (q indirect-special "struct lsh_queue"
          do_object_queue_mark do_object_queue_free)))
*/

void object_queue_init(struct object_queue *q);
int object_queue_is_empty(struct object_queue *q);

void object_queue_add_head(struct object_queue *q, struct lsh_object *o);
void object_queue_add_tail(struct object_queue *q, struct lsh_object *o);
struct lsh_object *object_queue_remove_head(struct object_queue *q);
struct lsh_object *object_queue_remove_tail(struct object_queue *q);

struct lsh_object *object_queue_peek_head(struct object_queue *q);
struct lsh_object *object_queue_peek_tail(struct object_queue *q);

/* For explicitly allocated object queues, which are not included in a
 * garbage collected object. */
void object_queue_kill(struct object_queue *q);

#define KILL_OBJECT_QUEUE(q) object_queue_kill((q))

struct object_list *queue_to_list(struct object_queue *q);
struct object_list *queue_to_list_and_kill(struct object_queue *q);

#define FOR_OBJECT_QUEUE(oq, n)				\
  struct lsh_queue_node *n##_this, *n##_next;			\
  struct lsh_object *n;						\
  for ( n##_this = (oq)->q.ht_links[LSH_QUEUE_HEAD];		\
	( n = ((struct object_queue_node *) n##_this)->o,	\
	  (n##_next = n##_this->np_links[LSH_QUEUE_NEXT]));	\
	n##_this = n##_next)

/* NOTE: You should probably use break or perhaps continue after
 * removing the current node. */
/* FIXME: This name is rather ugly. */

#define FOR_OBJECT_QUEUE_REMOVE(q, n) \
do { (q)->length--; lsh_queue_remove(n##_this); } while(0)

/* String queue */
struct string_queue_node
{
  struct lsh_queue_node header;
  struct lsh_string *s;
};

/* GABA:
   (struct
     (name string_queue)
     (vars
       (length . UINT32)
       (q indirect-special "struct lsh_queue"
          #f do_string_queue_free)))
*/

void string_queue_init(struct string_queue *q);
int string_queue_is_empty(struct string_queue *q);

void string_queue_add_head(struct string_queue *q, struct lsh_string *o);
void string_queue_add_tail(struct string_queue *q, struct lsh_string *o);
struct lsh_string *string_queue_remove_head(struct string_queue *q);
struct lsh_string *string_queue_remove_tail(struct string_queue *q);

struct lsh_string *string_queue_peek_head(struct string_queue *q);
struct lsh_string *string_queue_peek_tail(struct string_queue *q);

#endif /* LSH_QUEUE_H_INCLUDED */

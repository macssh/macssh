/* write_buffer.h
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

#ifndef LSH_WRITE_BUFFER_H_INCLUDED
#define LSH_WRITE_BUFFER_H_INCLUDED

#include "abstract_io.h"
#include "queue.h"

#define GABA_DECLARE
#include "write_buffer.h.x"
#undef GABA_DECLARE

/* For flow control, we use this callback to report that data
 * has been flushed from a write_buffer. */
/* GABA:
   (class
     (name flow_controlled)
     (vars
       (report method void UINT32)))
*/

#define FLOW_CONTROL_REPORT(o, written) ((o)->report((o), written))

/* GABA:
   (class
     (name write_buffer)
     (super abstract_write)
     (vars
       (block_size . UINT32)
       (buffer space UINT8)        ; Size is twice the blocksize 
       (empty . int)

       ; Total amount of data currently in the buffer
       (length . UINT32)

       (report object flow_controlled)
       
       ; If non-zero, don't accept any more data. The i/o-channel
       ; should be closed once the current buffers are flushed. 
       (closed . int)

       ;; (try_write . int)

       (q struct string_queue)

       (pos . UINT32)        ; Partial packet
       (partial string)

       (start . UINT32)
       (end . UINT32)))
*/

struct write_buffer *write_buffer_alloc(UINT32 size);
int write_buffer_pre_write(struct write_buffer *buffer);
void write_buffer_consume(struct write_buffer *buffer, UINT32 size);
void write_buffer_close(struct write_buffer *buffer);

#endif /* LSH_WRITE_BUFFER_H_INCLUDED */

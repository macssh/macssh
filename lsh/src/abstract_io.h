/* abstract_io.h
 *
 * This is the layer separating protocol processing from actual io.
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

#ifndef LSH_ABSTRACT_IO_H_INCLUDED
#define LSH_ABSTRACT_IO_H_INCLUDED

#include "exception.h"

#define GABA_DECLARE
#include "abstract_io.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name abstract_write)
     (vars
       (write method void "struct lsh_string *packet")))
*/

#define A_WRITE(f, packet) ((f)->write((f), (packet) ))

/* A handler that passes packets on to another handler */
/* GABA:
   (class
     (name abstract_write_pipe)
     (super abstract_write)
     (vars
       (next object abstract_write)))
*/


/* GABA:
   (class
     (name read_handler)
     (vars
       (handler indirect-method

		; The method returns the amount of data processed. 
		; On errors, it can set self to NULL, but only for
		; debugging purposes; it's the responibility of the the
		; exception handler to close the corresponding fd and
		; make sure that handler is not called again. '

		; Available is 0 iff we have got EOF.
		UINT32 "UINT32 available" "UINT8 *data")))
		"struct exception_handler *e")))
*/

/* May store a new handler into *h. */
#define READ_HANDLER(h, l, d) ((h)->handler(&(h), (l), (d)))

#endif /*LSH_ABSTRACT_IO_H_INCLUDED */

/* read_line.h
 *
 * Read-handler processing a line at a time.
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

#ifndef  LSH_READ_HANDLER_H_INCLUDED
#define  LSH_READ_HANDLER_H_INCLUDED

#include "abstract_io.h"

#define GABA_DECLARE
#include "read_line.h.x"
#undef GABA_DECLARE

/* This limit follows the ssh specification */
#define MAX_LINE 255

/* May store a new handler into *h. */
/* GABA:
   (class
     (name line_handler)
     (vars
       (handler indirect-method void "struct read_handler **r"
		"UINT32 length" "UINT8 *line"
		"struct exception_handler *e")))
*/

#define PROCESS_LINE(h, r, length, line, e) \
((h)->handler(&(h), (r), (length), (line), (e)))

struct read_handler *make_read_line(struct line_handler *handler,
				    struct exception_handler *e);

#endif /* LSH_READ_HANDLER_H_INCLUDED */

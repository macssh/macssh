/* lsh_argp.h
 *
 * Include the right argp header.
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

#ifndef LSH_LSH_ARGP_H_INCLUDED
#define LSH_LSH_ARGP_H_INCLUDED

#include "lsh_types.h"

#if HAVE_ARGP_PARSE
/* We're using the system's argp implementation */
# if HAVE_ARGP_H
#  include <argp.h>
# endif
#else
# include "argp/argp.h"
#endif

#endif /* LSH_LSH_ARGP_H_INCLUDED */

/* proxy_userauth.h
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1999 Balázs Scheidler
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef LSH_PROXY_USERAUTH_H_INCLUDED
#define LSH_PROXY_USERAUTH_H_INCLUDED

#include "xalloc.h"
#include "alist.h"
#include "list.h"
#include "userauth.h"

#define GABA_DECLARE
#include "proxy_userauth.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name proxy_user)
     (vars
       (name string)))
*/

struct proxy_userauth;

extern struct proxy_userauth proxy_password_auth;

struct command *make_proxy_userauth(struct int_list *allowed_methods,
				    struct alist *methods,
				    struct alist *services);

#endif /* LSH_PROXY_USERAUTH_H_INCLUDED */

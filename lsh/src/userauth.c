/* userauth.h
 *
 */

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

#include "userauth.h"

#include "format.h"
#include "ssh.h"
#include "xalloc.h"

#define GABA_DEFINE
#include "userauth.h.x"
#undef GABA_DEFINE

struct exception *
make_userauth_special_exception(struct lsh_string *reply, const char *msg)
{
  NEW(userauth_special_exception, self);
  self->super.type = EXC_USERAUTH_SPECIAL;
  self->super.msg = msg ? msg : "userauth special reply";
  self->reply = reply;

  return &self->super;
}


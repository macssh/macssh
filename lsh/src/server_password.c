/* server_password.c
 *
 * System dependant password related functions.
 *
 * $Id$
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

#include "charset.h"
#include "format.h"
#include "parse.h"
#include "ssh.h"
#include "server_userauth.h"
#include "werror.h"
#include "xalloc.h"

#include "server_password.c.x"

/* GABA:
   (class
     (name userauth_password)
     (super userauth)
     (vars
       (db object user_db)))
*/

static void
do_authenticate(struct userauth *s,
		struct ssh_connection *connection UNUSED,
		struct lsh_string *username,
		UINT32 service UNUSED,
		struct simple_buffer *args,
		struct command_continuation *c,
		struct exception_handler *e)
{
  CAST(userauth_password, self, s);
  
  int change_passwd;
  
  username = utf8_to_local(username, 1, 1);
  if (!username)
    {
      PROTOCOL_ERROR(e, "Invalid utf8 in username.");
      return;
    }

  if (parse_boolean(args, &change_passwd))
    {
      struct lsh_string *password = NULL;
      
      if (change_passwd)
	{
	  static const struct exception passwd_change_not_implemented
	    = STATIC_EXCEPTION(EXC_USERAUTH,
			       "Password change not implemented.");
	  
	  lsh_string_free(username);
	  EXCEPTION_RAISE(e, &passwd_change_not_implemented);
			  
	  return;
	}
      if ( (password = parse_string_copy(args))
	   && parse_eod(args))
	{
	  struct lsh_user *user;

	  password = utf8_to_local(password, 1, 1);

	  if (!password)
	    {
	      lsh_string_free(username);
	      PROTOCOL_ERROR(e, "Invalid utf8 in password.");
	      return;
	    }
       
	  user = USER_LOOKUP(self->db, username, 1);

	  if (!user)
	    {
	      static const struct exception no_such_user
		= STATIC_EXCEPTION(EXC_USERAUTH, "No such user");
	      
	      lsh_string_free(password);
	      EXCEPTION_RAISE(e, &no_such_user);
	      return;
	    }

	  USER_VERIFY_PASSWORD(user, password, c, e);
	  return;
	}
      if (password)
	lsh_string_free(password);
    }
  
  /* Request was invalid */
  lsh_string_free(username);
  
  PROTOCOL_ERROR(e, "Invalid password USERAUTH message.");
}

struct userauth *
make_userauth_password(struct user_db *db)
{
  NEW(userauth_password, self);
  self->super.authenticate = do_authenticate;
  self->db = db;

  return &self->super;
}


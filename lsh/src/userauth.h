/* userauth.h
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

#ifndef LSH_USERAUTH_H_INCLUDED
#define LSH_USERAUTH_H_INCLUDED

#include "command.h"
#include "exception.h"
#include "resource.h"

/* For uid_t */
#include <unistd.h>

/* Forward declaration */
struct env_value;

#define GABA_DECLARE
#include "userauth.h.x"
#undef GABA_DECLARE

/* GABA:
   (class
     (name userauth_special_exception)
     (super exception)
     (vars
       (reply string)))
*/

/* FIXME: Perhaps it's better to use a const char * for the value? */
struct env_value
{
  const char *name;
  struct lsh_string *value;
};

struct exception *
make_userauth_special_exception(struct lsh_string *reply,
				const char *msg);

/* GABA:
   (class
     (name lsh_process)
     (super resource)
     (vars
       (signal method int int)))
*/

#define SIGNAL_PROCESS(p, s) ((p)->signal((p), (s)))

/* GABA:
   (class
     (name lsh_user)
     (vars
       ; This string include a terminating NUL-character, for
       ; compatibility with library and system calls.
       (name string)
       (uid . uid_t)
       
       ; Verify a password. Consumes the password string.
       (verify_password method void
                        "struct lsh_string *pw"
                        "struct command_continuation *c"
			"struct exception_handler *e")

       ; Check if a file in the user's home directory exists.
       ; Used by the current publickey userauth method.
       (file_exists method int "struct lsh_string *name" "int free")

       ; Open a file in the user's "~/.lsh" directory. File must be
       ; owned by the user and not writable for other users. If SECRET is 1,
       ; it must also not be readable by other users.
       (read_file method void
                  "const char *name" "int secret"
                  "struct command_continuation *c" "struct exception_handler *e")
		  
       ; chdir to the user's home directory
       (chdir_home method int)

       ; Fork a user process.

       ; FIXME: For non-UN*X-systems, we need a method
       ; that combines fork() and exec(). It would have lots of
       ; input arguments, perhaps a tag list, and return
       ; an object containing the stdin file objects and perhaps some
       ; other info.

       ; This function also returns the pid.

       ; The tty argument is for utmp/wtmp logging
       (fork_process method int "struct lsh_process **child"
                     "struct exit_callback *c"
                     "struct address_info * peer" "struct lsh_string *tty")

       ; This modifies the argv vector, in particular its first
       ; element. So the vector must have at least two elements,
       ; (argv[0], NULL). 

       (exec_shell method void "int login"
                   "const char **argv"
		   "unsigned env_length"
		   "const struct env_value *env")))
*/

#define USER_VERIFY_PASSWORD(u, p, c, e) ((u)->verify_password((u), (p), (c), (e)))
#define USER_FILE_EXISTS(u, n, f) ((u)->file_exists((u), (n), (f)))
#define USER_READ_FILE(u, n, s, x, e) ((u)->read_file((u), (n), (s), (x), (e)))
#define USER_CHDIR_HOME(u) ((u)->chdir_home((u)))
#define USER_FORK(u, c, e, p, t) \
  ((u)->fork_process((u), (c), (e), (p), (t)))
#define USER_EXEC(u, m, a, l, e) ((u)->exec_shell((u), (m), (a), (l), (e)))

#endif /* LSH_USERAUTH_H_INCLUDED */

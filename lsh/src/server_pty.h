/* server_pty.h
 *
 * $Id$ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998, 1999, Niels Möller, Balázs Scheidler
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

#ifndef LSH_SERVER_PTY_H_INCLUDED
#define LSH_SERVER_PTY_H_INCLUDED

#include "lsh.h"

#include "resource.h"

#include <termios.h>
#include <sys/types.h>

#define MAX_TTY_NAME	32

#define GABA_DECLARE
#include "server_pty.h.x"
#undef GABA_DECLARE


/* GABA:
   (class 
     (name pty_info)
     (super resource)
       (vars
         (master . int)
	 (slave . int)

	 ; Name of slave tty.
	 ; Needed for SysV pty-handling (where opening the tty
	 ; makes it the controlling terminal). Perhaps handy also for
	 ; writing accurate utmp-entries.
	 ; This string should be NUL-terminated
	 (tty_name string)))
*/

struct pty_info *make_pty_info(void);
int pty_allocate(struct pty_info *pty, uid_t user);

/* NOTE: This function also makes the current process a process group
 * leader. */
int tty_setctty(struct pty_info *pty);


#endif /* LSH_SERVER_PTY_H_INCLUDED */

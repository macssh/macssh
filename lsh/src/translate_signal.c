/* translate_signal.c
 *
 * Translate local signal numbers to canonical numbers, and vice versa.
 * The value of "canonical" is rather arbitrary.
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

#include "translate_signal.h"

#include <signal.h>

/* The signal numbering found on the sparc-linux box I'm sitting at
 * seems quite wide-spread. From now on, I consider it the canonical
 * signal numbering. Signal numbers beyond 31 may be defined later. */

struct signal_assoc
{
  int network;
  int local;
};


static const struct signal_assoc translate[] =
{
#ifdef SIGHUP
  { 1, SIGHUP },
#endif
#ifdef SIGINT
  { 2, SIGINT },
#endif
#ifdef SIGQUIT
  { 3, SIGQUIT },
#endif
#ifdef SIGILL
  { 4, SIGILL },
#endif
#ifdef SIGTRAP
  { 5, SIGTRAP },
#endif
#ifdef SIGABRT
  { 6, SIGABRT },
#endif
  /* IOT is the mnemonic for the PDP-11 instruction used by the abort function */
#ifdef SIGIOT
  { 6, SIGIOT },
#endif
#ifdef SIGEMT
  { 7, SIGEMT },
#endif
#ifdef SIGFPE
  { 8, SIGFPE },
#endif
#ifdef SIGKILL
  { 9, SIGKILL },
#endif
#ifdef SIGBUS
  { 10, SIGBUS },
#endif
#ifdef SIGSEGV
  { 11, SIGSEGV },
#endif
#ifdef SIGSYS
  { 12, SIGSYS },
#endif
#ifdef SIGPIPE
  { 13, SIGPIPE },
#endif
#ifdef SIGALRM
  { 14, SIGALRM },
#endif
#ifdef SIGTERM
  { 15, SIGTERM },
#endif
  /* Signals beyond 15 does not have the same meaning everywhere */
#ifdef SIGURG
  { 16, SIGURG },
#endif
#ifdef SIGSTOP
  { 17, SIGSTOP },
#endif
#ifdef SIGTSTP
  { 18, SIGTSTP },
#endif
#ifdef SIGCONT
  { 19, SIGCONT },
#endif
#ifdef SIGCHLD
  { 20, SIGCHLD },
#endif
#ifdef SIGTTIN
  { 21, SIGTTIN },
#endif
#ifdef SIGTTOU
  { 22, SIGTTOU },
#endif
#ifdef SIGIO
  { 23, SIGIO },
#endif
#ifdef SIGPOLL
  { 23, SIGPOLL }, /* SysV name for SIGIO */
#endif
#ifdef SIGXCPU
  { 24, SIGXCPU },
#endif
#ifdef SIGXFSZ
  { 25, SIGXFSZ },
#endif
#ifdef SIGVTALRM
  { 26, SIGVTALRM },
#endif
#ifdef SIGPROF
  { 27, SIGPROF },
#endif
#ifdef SIGWINCH
  { 28, SIGWINCH },
#endif
#ifdef SIGLOST
  { 29, SIGLOST },
#endif
#ifdef SIGUSR1
  { 30, SIGUSR1 },
#endif
#ifdef SIGUSR2
  { 31, SIGUSR2 },
#endif
  { -1, -1 }
};
  
/* Returns 0 if the signal has no network number */
int signal_local_to_network(int signal)
{
  int i;

  if (!signal)
    return 0;
  
  for (i = 0; ; i++)
    {
      if (translate[i].local < 0)
	return 0;
      if (translate[i].local == signal)
	return translate[i].network;
    }
}

/* Returns 0 if the signal has no local number */
int signal_network_to_local(int signal)
{
  int i;

  if (!signal)
    return 0;

  for (i = 0; ; i++)
    {
      if (translate[i].local < 0)
	return 0;
      if (translate[i].network == signal)
	return translate[i].local;
    }
}

/*
 * ssh2.h
 * (c) 2000 Jean-Pierre Stierlin.
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

#include "lshprefix.h"
#include "lsh_context.h"
#include "wind.h"

#include <PLStringFuncs.h>

#ifdef __cplusplus
extern "C" {
#endif

void ssh2_init();
void ssh2_sched();
void GUSIHandleNextEvent(long sleepTime);

WindRec *ssh2_window();
Boolean can_read();
void ssh2_doevent(long sleepTime);
pascal Ptr PLstrrchr(ConstStr255Param s, short c);
void close_all_files(lshcontext *context);

#ifdef __cplusplus
}
#endif


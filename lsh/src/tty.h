/* tty.h
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

#ifndef LSH_TTY_H_INCLUDED
#define LSH_TTY_H_INCLUDED

#include "lsh_types.h"

#include <termios.h>

struct terminal_dimensions
{
  UINT32 char_width;
  UINT32 char_height;
  UINT32 pixel_width;
  UINT32 pixel_height;
};

int tty_getattr(int fd, struct termios *ios);
int tty_setattr(int fd, struct termios *ios);

int tty_getwinsize(int fd, struct terminal_dimensions *dims);
int tty_setwinsize(int fd, const struct terminal_dimensions *dims);

struct lsh_string *
tty_encode_term_mode(struct termios *ios);
int
tty_decode_term_mode(struct termios *ios, UINT32 t_len, const UINT8 *t_modes);

#if HAVE_CFMAKERAW
#define CFMAKERAW cfmakeraw
#else /* !HAVE_CFMAKERAW */
/* The flags part definition is probably from the linux cfmakeraw man
 * page. We also set the MIN and TIME attributes (note that these use
 * the same fields as VEOF and VEOL). */

#define CFMAKERAW(ios) do {						   \
  (ios)->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON); \
  (ios)->c_oflag &= ~OPOST;						   \
  (ios)->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);			   \
  (ios)->c_cflag &= ~(CSIZE|PARENB); (ios)->c_cflag |= CS8;		   \
  (ios)->c_cc[VMIN] = 3; (ios)->c_cc[VTIME] = 2;			   \
} while(0)
#endif /* !HAVE_CFMAKERAW */

#endif /* LSH_TTY_H_INCLUDED */

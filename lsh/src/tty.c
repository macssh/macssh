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

#include "tty.h"

#include "format.h"
#include "parse.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#if WITH_PTY_SUPPORT

int
tty_getattr(int fd, struct termios *ios)
{
  return tcgetattr(fd, ios) != -1 ? 1 : 0;
}

int
tty_setattr(int fd, struct termios *ios)
{
  return tcsetattr(fd, TCSADRAIN, ios) != -1 ? 1 : 0;
}

int
tty_getwinsize(int fd, struct terminal_dimensions *dims)
{
  struct winsize ws;
  int rc;
  
  rc = ioctl(fd, TIOCGWINSZ, &ws);
  if (rc != -1)
    {
      dims->char_width = ws.ws_col;
      dims->char_height = ws.ws_row;
      dims->pixel_width = ws.ws_xpixel;
      dims->pixel_height = ws.ws_ypixel;
      return 1;
    }
  return 0;
}

int
tty_setwinsize(int fd, const struct terminal_dimensions *dims)
{
  struct winsize ws;
  
  ws.ws_row = dims->char_width;
  ws.ws_col = dims->char_height;
  ws.ws_xpixel = dims->pixel_width;
  ws.ws_ypixel = dims->pixel_height;
  
  return ioctl(fd, TIOCSWINSZ, &ws) == -1 ? 0 : 1;
}

static int cc_ndx[] = 
{
#ifdef VINTR
  VINTR, 
#else
  -1,
#endif
#ifdef VQUIT
  VQUIT, 
#else
  -1,
#endif
#ifdef VINTR
  VERASE, 
#else
  -1,
#endif
#ifdef VKILL
  VKILL, 
#else
  -1,
#endif
#ifdef VEOF
  VEOF, 
#else
  -1,
#endif
#ifdef VEOL
  VEOL, 
#else
  -1,
#endif
#ifdef VEOL2
  VEOL2, 
#else
  -1,
#endif
#ifdef VSTART
  VSTART, 
#else
  -1,
#endif
#ifdef VSTOP
  VSTOP, 
#else
  -1,
#endif
#ifdef VSUSP
  VSUSP, 
#else
  -1,
#endif
#ifdef VDSUSP
  VDSUSP, 
#else
  -1,
#endif
#ifdef VREPRINT
  VREPRINT, 
#else
  -1,
#endif
#ifdef VWERASE
  VWERASE, 
#else
  -1,
#endif
#ifdef VLNEXT
  VLNEXT, 
#else
  -1,
#endif
#ifdef VFLUSH
  VFLUSH, 
#else
  -1,
#endif
#ifdef VSWTCH
  VSWTCH, 
#else
  -1,
#endif
#ifdef VSTATUS
  VSTATUS, 
#else
  -1,
#endif
#ifdef VDISCARD
  VDISCARD 
#else
  -1,
#endif
};

static int cc_iflags[] = {
#ifdef IGNPAR
  IGNPAR,
#else
  0,
#endif
#ifdef PARMRK
  PARMRK,
#else
  0,
#endif
#ifdef INPCK
  INPCK,
#else
  0,
#endif
#ifdef ISTRIP
  ISTRIP,
#else
  0,
#endif
#ifdef INLCR
  INLCR,
#else
  0,
#endif
#ifdef IGNCR
  IGNCR,
#else
  0,
#endif
#ifdef ICRNL
  ICRNL,
#else
  0,
#endif
#ifdef IUCLC
  IUCLC,
#else
  0,
#endif
#ifdef IXON
  IXON,
#else
  0,
#endif
#ifdef IXANY
  IXANY,
#else
  0,
#endif
#ifdef IXOFF
  IXOFF,
#else
  0,
#endif
#ifdef IMAXBEL
  IMAXBEL
#else
  0
#endif
};

static int cc_oflags[] = {
#ifdef OPOST
  OPOST,
#else
  0,
#endif    
#ifdef OLCUC
  OLCUC,
#else
  0,
#endif    
#ifdef ONLCR
  ONLCR,
#else
  0,
#endif    
#ifdef OCRNL
  OCRNL,
#else
  0,
#endif    
#ifdef ONOCR
  ONOCR,
#else
  0,
#endif    
#ifdef ONLRET
  ONLRET
#else
  0
#endif    
};

static int cc_cflags[] = {
#ifdef CS7
  CS7,
#else
  0,
#endif    
#ifdef CS8
  CS8,
#else
  0,
#endif    
#ifdef PARENB
  PARENB,
#else
  0,
#endif    
#ifdef PARODD
  PARODD
#else
  0
#endif    
};

static int cc_lflags[] = {
#ifdef ISIG
  ISIG,
#else
  0,
#endif
#ifdef ICANON
  ICANON,
#else
  0,
#endif
#ifdef XCASE
  XCASE,
#else
  0,
#endif
#ifdef ECHO
  ECHO,
#else
  0,
#endif
#ifdef ECHOE
  ECHOE,
#else
  0,
#endif
#ifdef ECHOK
  ECHOK,
#else
  0,
#endif
#ifdef ECHONL
  ECHONL,
#else
  0,
#endif
#ifdef NOFLSH
  NOFLSH,
#else
  0,
#endif
#ifdef TOSTOP
  TOSTOP,
#else
  0,
#endif
#ifdef IEXTEN
  IEXTEN,
#else
  0,
#endif
#ifdef ECHOCTL
  ECHOCTL,
#else
  0,
#endif
#ifdef ECHOKE
  ECHOKE,
#else
  0,
#endif
#ifdef PENDIN
  PENDIN
#else
  0,
#endif
};

#define SIZE(x) (sizeof((x)) / sizeof((x)[0]))

/* FIXME: TTY_?SPEED not handled */

#define PARSE_FLAGS(cc, bits, offset) do {			\
  debug("tty_encode_term_mode: termios bits %xi (offset %i)\n",	\
        (bits), (offset));					\
  for (i=0; i<SIZE(cc); i++)					\
    if (cc[i])							\
      {								\
      	UINT32 r;						\
      	if ( (new->length - p) < 5)				\
      	  goto fail;						\
								\
      	r = ((bits) & (cc)[i]) ? 1 : 0;				\
      	new->data[p++] = i + (offset);				\
      	WRITE_UINT32(new->data + p, r);				\
      	p += 4;							\
      }								\
} while(0)
     
struct lsh_string *
tty_encode_term_mode(struct termios *ios)
{
  unsigned int i;
  struct lsh_string *new;
  UINT32 p = 0;
  
  new = lsh_string_alloc(650);

  for (i=0; i<SIZE(cc_ndx); i++)
    {
      if (cc_ndx[i] != -1)
	{
	  if ( (new->length - p) < 5)
	    goto fail;
	  
	  new->data[p++] = i + 1;
	  WRITE_UINT32(new->data + p, ios->c_cc[cc_ndx[i]]);
	  p += 4;
	}
    }
  PARSE_FLAGS(cc_iflags, ios->c_iflag, 30);
  PARSE_FLAGS(cc_lflags, ios->c_lflag, 50);
  PARSE_FLAGS(cc_oflags, ios->c_oflag, 70);
  PARSE_FLAGS(cc_cflags, ios->c_cflag, 90);

  if ( (new->length - p) < 1)
    goto fail;
  
  new->data[p++] = 0;
  new->length = p;
  
  return new;

fail:
  lsh_string_free(new);
  return NULL;
}

#define TTY_SET_VALUE(target, param, table, index)	\
do {							\
  int _value;						\
  if ((index) < (sizeof(table) / sizeof((table)[0]))	\
      && ((_value = (table)[index]) >= 0))		\
    target[_value] = param;				\
} while(0)

#define TTY_SET_FLAG(target, flag, table, index)	\
do {							\
  int _mask;						\
  if ((index) < (sizeof(table) / sizeof((table)[0]))	\
      && ((_mask = (table)[index]) >= 0))		\
    {							\
      if (flag)						\
	(target) |= _mask;				\
      else						\
	(target) &= ~_mask;				\
    }							\
} while(0)
      
/* Interpret ssh:s terminal description */
int
tty_decode_term_mode(struct termios *ios, UINT32 t_len, const UINT8 *t_modes)
{
  struct simple_buffer buffer;
  
  simple_buffer_init(&buffer, t_len, t_modes);
  
  for (;;)
    {
      unsigned opcode;
      
      if (!parse_uint8(&buffer, &opcode))
	return 0;
	  
      {
	UINT32 param;
	
	if ( (opcode == SSH_TTY_OP_END)
	     || (opcode > SSH_TTY_OP_RESERVED))
	  break;

	if (!parse_uint32(&buffer, &param))
	  return 0;
	{
	  /* FIXME: This code might be simplified a little. I
	   * think some table lookups (mapping each opcode to some
	   * "action class", including ranges to check as well as
	   * further tables... Something like 

	   struct action = at[opcode];
	   switch (action->type)
	   {
	   case ACTION_FLAG:
	   ...
	  */
	  if (opcode < 30) 
	    TTY_SET_VALUE(ios->c_cc, param, cc_ndx, opcode - 1);
	  else if (opcode < 50)
	    TTY_SET_FLAG(ios->c_iflag, param, cc_iflags, opcode - 30);
	  else if (opcode < 70)
	    TTY_SET_FLAG(ios->c_lflag, param, cc_lflags, opcode - 50);
	  else if (opcode < 90)
	    TTY_SET_FLAG(ios->c_oflag, param, cc_oflags, opcode - 70);
	  else if (opcode < 128)
	    TTY_SET_FLAG(ios->c_cflag, param, cc_cflags, opcode - 90);
	  else
	    {
	      switch(opcode)
		{
		case SSH_TTY_OP_ISPEED:
		case SSH_TTY_OP_OSPEED:
		  /* FIXME: set input/output speed there's no point
		   * in setting the speed of a pseudo tty IMHO */
		  /* The speed of the user's terminal could be
		   * useful for programs like emacs that try to
		   * optimize the redraw algorithm (slower
		   * terminals gets partial updates. But that is
		   * pretty useless as long as 9600 bps is
		   * considered fast... */
		  break;
		default:
		  werror("Unsupported terminal mode: %i\n", opcode);
		}
	    }
	}
      }
    }
  return 1;
}

#endif /* WITH_PTY_SUPPORT */

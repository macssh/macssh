// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
rgmp.c by Gaige B. Paulsen
  spawned from rgp.c by Aaron Contorer for NCSA
Copyright 1987, board of trustees, University of Illinois

Routines for Macintosh Picture output.  Only one Real device is available
*/


#define INXMAX 4096
#define INYMAX 4096

#include "rgmp.proto.h"

void	TEKMacPicunload(void) {}

char	RGMPbusy; /* is device already in use */
short	RGMPwidth, RGMPheight, RGMPxoffset=0, RGMPyoffset=0;
int RGMPcolor[]=
	{ 30,			/* black */
	  33,			/* white */
	  205,			/* red */
	  341,			/* green */
	  409,			/* blue */
	  273,			/* cyan */
	  137,			/* magenta */
	  69			/* yellow */
	  };

short	RGMPnewwin(void)
{

	RGMPbusy=1;

/*	RGMPwidth=4096;
	RGMPheight=4096;	 */
	RGMPxoffset=0;
	RGMPyoffset=0;

	return(0);
}

char	*RGMPdevname(void)
{
	return("Macintosh PICTURE output");
}

void	RGMPinit(void)
{
	RGMPbusy=0;

/*	RGMPwidth=4096;
	RGMPheight=4096; */
	RGMPxoffset=0;
	RGMPyoffset=0;
}

short	RGMPpencolor(short w, short color)
{
	UNUSED_ARG(w)
	ForeColor( (long) RGMPcolor[color] );
	return 0;
}

short	RGMPclose(short w)
{
	UNUSED_ARG(w)
	RGMPbusy=0;
	return 0;
}

short	RGMPpoint(short w, short x, short y)
{
	UNUSED_ARG(w)
	MoveTo(x,y);
	LineTo(x,y);
	return 0;
}

short	RGMPdrawline(short w, short x0, short y0, short x1, short y1)
{
	UNUSED_ARG(w)
	x0 = RGMPxoffset + (short) ((long) x0 * RGMPwidth / INXMAX);
	y0 = RGMPyoffset + RGMPheight - (short) ((long) y0 * RGMPheight / INYMAX);
	x1 = RGMPxoffset + (short) ((long) x1 * RGMPwidth/INXMAX);
	y1 = RGMPyoffset + RGMPheight - (short) ((long) y1 * RGMPheight / INYMAX);

	MoveTo(x0,y0);
	LineTo(x1,y1);
	return 0;
}

void	RGMPinfo(short w, short v, short a, short b, short c, short d)
{
	UNUSED_ARG(w)
	UNUSED_ARG(v)
	UNUSED_ARG(a)
	UNUSED_ARG(b)
	UNUSED_ARG(c)
	UNUSED_ARG(d)
}

void	RGMPdataline(short blah, short blam, short bluh)
{
	UNUSED_ARG(blah)
	UNUSED_ARG(blam)
	UNUSED_ARG(bluh)
}

void	RGMPcharmode(short w, short rotation, short size)
{
	UNUSED_ARG(w)
	UNUSED_ARG(rotation)
	UNUSED_ARG(size)
}

short	RGMPsize(Rect *incoming)
{
	RGMPheight= incoming->bottom-incoming->top;
	RGMPwidth = incoming->right - incoming->left;
	RGMPyoffset= incoming->top;
	RGMPxoffset= incoming->left; 

	return(0);
}

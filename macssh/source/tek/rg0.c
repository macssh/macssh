// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*

rgnull.c by Aaron Contorer for NCSA
Copyright 1987, board of trustees, University of Illinois

routines for "null" device -- calling these routines
	has no effect, but they are compatible with all RG calls.

*/


#include "rg0.proto.h"

void	TEKNULLunload(void){}

short	RG0newwin(void)
{
	return(0);
}

char *RG0devname(void)
{
	return(nil);
}

void	RG0oneshort(short x)
{
	UNUSED_ARG(x)
}

void	RG0void(void)
{}

short	RG0returnshort(short x)
{
	UNUSED_ARG(x)
	return 0;
}

short	RG0pencolor(short w, short color)
{
	UNUSED_ARG(w)
	UNUSED_ARG(color)
	return 0;
}

short	RG0point(short w, short x, short y)
{
	UNUSED_ARG(w)
	UNUSED_ARG(x)
	UNUSED_ARG(y)
	return 0;
}

short	RG0drawline(short w, short a, short b, short c, short d)
{
	UNUSED_ARG(w)
	UNUSED_ARG(a)
	UNUSED_ARG(b)
	UNUSED_ARG(c)
	UNUSED_ARG(d)
	return 0;
}

void	RG0dataline(short w, short data, short count)
{
	UNUSED_ARG(w)
	UNUSED_ARG(data)
	UNUSED_ARG(count)
}

void	RG0charmode(short w, short rotation, short size)
{
	UNUSED_ARG(w)
	UNUSED_ARG(rotation)
	UNUSED_ARG(size)
}

void	RG0info(short w, short a, short b, short c, short d, short v)
{
	UNUSED_ARG(w)
	UNUSED_ARG(a)
	UNUSED_ARG(b)
	UNUSED_ARG(c)
	UNUSED_ARG(d)
	UNUSED_ARG(v)
}

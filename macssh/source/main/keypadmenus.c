// keypadmenus.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "VSkeys.h"						/* Baylor - for keypad menu support */
#include "wind.h"
#include "vsdata.h"

#include "vsinterf.proto.h"
#include "keypadmenus.proto.h"

extern WindRec
	*screens;
extern	short	scrn;

static	unsigned char KeypadMenuMap[] = {				/* Baylor - which menu items map to which key */
/* 	no zero item */
	0,
/*	find,	insert,	remove,	select,	prev,	next,	*/
	VSHELP,	VSHOME, VSPGUP,	VSDEL,	VSEND,	VSPGDN,
/*	up,		left,	down,	right,	*/
	VSUP,	VSLT,	VSDN,	VSRT,
/*	pf1,	pf2,	pf3,	pf4		*/
	VSPF1,	VSPF2,	VSPF3,	VSPF4,
/*	kp7,	kp8,	kp9,	minus,	*/
	VSK7,	VSK8,	VSK9,	VSKM,
/*	kp4,	kp5,	kp6,	comma,	*/
	VSK4,	VSK5,	VSK6,	VSKC,
/*	kp1,	kp2,	kp3,	enter,	*/
	VSK1,	VSK2,	VSK3,	VSKE,
/*	kp0,	period */
	VSK0,	VSKP
};

/* NONO: changed F1-F15 */
#if 0
static	unsigned char FuncKeyMenuMap[] = {				/* Baylor - which menu items map to which key */
/* 	no zero item */
	0,
/*	f5,		f6,		f7,		f8,		f9,		*/
	VSF6,	VSF7,	VSF8,	VSF9,	VSF10,
/*	f10,	f11,	f12,	f13,	f14,	*/
	VSF11,	VSF12,	VSF13,	VSF14,	VSF15,
/*	help,	do	*/
	VSF15,	VSF16,
/*	f17,	f18,	f19,	f20		*/
	VSF17,	VSF18,	VSF19,	VSF20
};
#endif

static	unsigned char FuncKeyMenuMap[] = {				/* Baylor - which menu items map to which key */
/* 	no zero item */
	0,
/*	f1,		f2,		f3,		f4,		f5,		*/
	VSF6,	VSF7,	VSF8,	VSF9,	VSF10,
/*	f6,		f7,		f8,		f9,		f10,	*/
	VSF11,	VSF12,	VSF13,	VSF14,	VSF15,
/*	f11,	f12,	f13,	f14,	f15		*/
	VSF16,	VSF17,	VSF18,	VSF19,	VSF20
};
/* NONO */

void	KeyMenu(short theItem, short shifted)
{
	VSkbsend(screens[scrn].vs, KeypadMenuMap[theItem], screens[scrn].echo,
		shifted);		/* Baylor */
}

void	FuncMenu(short theItem, short shifted)
{
	VSkbsend(screens[scrn].vs, FuncKeyMenuMap[theItem], screens[scrn].echo,
		shifted);	/* Baylor */
}
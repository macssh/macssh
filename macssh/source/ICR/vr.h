#ifndef	__VRDEFS__
#define __VRDEFS__

/*	saved arg values in parser */
union arg {
	short	a_num;				/* integer number */
	char	a_ptr[100];			/* string pointer */
};

typedef	struct VRwin VRW;
typedef	struct RRwin RRW;

struct RRwin {
	short device;				/* Device Number */
	short wn;					/* Window Number */
};

/*	format of a window entry */
struct VRwin {
	char	w_name[100];		/* window's name, assigned on creation */
	char	w_used;				/* flag - is this name an old duplicate? */
	short	w_left;				/* left edge */
	short	w_top;				/* top edge */
	short	w_width;			/* width */
	short	w_height;			/* height */
	short	w_display;			/* hardware display number of window */
	RRW		w_rr;				/* machine dep info for window */
	VRW		*w_next;			/* next pointer */
};

/*	variables for linked list management */

#ifdef MASTERDEF
struct VRwin VRhead;
#else
extern struct VRwin VRhead;
#endif

#include "vr.proto.h"

#endif	// __VRDEFS__
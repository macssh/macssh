// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

/*
rgmac.c by Gaige B. Paulsen
  spawned from rgp.c by Aaron Contorer for NCSA
Copyright 1987, board of trustees, University of Illinois

Routines for Macintosh Window output.  
*/

#include "tekrgmac.proto.h"
#include "maclook.proto.h"
#include "vgtek.proto.h"
#include "mydnr.proto.h"
#include "rsinterf.proto.h"
#include "netevent.proto.h"
#include "rg0.proto.h"
#include "rgmp.proto.h"
#include "InternalEvents.h"
#include "tekdefs.h"		/* NCSA: sb - all the TEK defines are now here */

//#define TEK_DEBUG1
#ifdef	TEK_DEBUG1

#define	TEK_DEBUG_PRINT(x)	putln(x)
char	TEK_DEBUG_STRING	[512];
#else
#define	TEK_DEBUG_PRINT(x)
#endif

/*--------------------------------------------------------------------------*/
/* Some variable definitions are needed before we give the code				*/
/* 																			*/
/*--------------------------------------------------------------------------*/

long	RGMlastclick;
short	RGMcolor[]=
	{ 30,			/* black */
	  33,			/* white */
	  205,			/* red */
	  341,			/* green */
	  409,			/* blue */
	  273,			/* cyan */
	  137,			/* magenta */
	  69			/* yellow */
	  };

struct	RGMwindows *RGMwind[ MAXWIND ];

char	*RGMdevname(void)
{
	return("Macintosh Windows");
}

short	RGMnewwin(void)
{
	short	i = 0;

	while ( i < MAXWIND && RGMwind[i] != nil )
		i++;

	if (i >= MAXWIND)
		return -1;

	RGMwind[i] = (struct RGMwindows *) myNewPtr(sizeof(struct RGMwindows));
	if (RGMwind[i] == nil) {
		return -1;
	}

	RGMwind[i]->id = 'RGMW';

	RGMwind[i]->wind = GetNewWindow(256, NULL, kInFront);
	if (RGMwind[i]->wind == 0L) {
		DisposePtr((Ptr)RGMwind[i]);
		RGMwind[i] = nil;
		return -1;
	}

	((WindowPeek)RGMwind[i]->wind)->windowKind = WIN_TEK;
	
	RGMwind[i]->vg = -1;				/* BYU */
	RGMwind[i]->vs = -1;				/* BYU */
	RGMwind[i]->xorigin = 0;
	RGMwind[i]->yorigin = 0;
	RGMwind[i]->xscale  = WINXMAX;	/* BYU LSC - need the "W" here */
	RGMwind[i]->yscale  = WINYMAX;	/* BYU LSC - need the "W" here */
	RGMwind[i]->width   = 400;		/* BYU LSC - was 256 (size of old window resource */
	RGMwind[i]->height  = 300;		/* BYU LSC - was 342 (size of old window resource */
	RGMwind[i]->ingin   = 0;		/* BYU LSC */

	// avoid unnecessary update
	ValidRect(&RGMwind[i]->wind->portRect);  

	return i;
}

void	RGMinit(void)
{
	short	i;
	for (i = 0; i < MAXWIND; i++)
		RGMwind[i] = nil;
}

short	RGMgin(short w)
{
	if (RGsetwind(w))
		return -1;

	setgraphcurs();
	RGMwind[w]->ingin=1;
}

short	RGMpencolor(short w, short color)
{
	if (RGsetwind(w))
		return -1;

	ForeColor((long) RGMcolor[color] );
#ifdef	TEK_DEBUG_1
	sprintf(TEK_DEBUG_STRING, "TEK: Setting pen color to %d", color);
	putln(TEK_DEBUG_STRING);
#endif

}

short	RGMclrscr(short w)
{	
	if (RGsetwind(w))
		return -1;

	PaintRect(&RGMwind[w]->wind->portRect);
	TEK_DEBUG_PRINT("TEK: Clearing screen");					
}	

short	RGMclose(short w)
{
	if (RGsetwind(w))
		return -1;

	DisposeWindow(RGMwind[w]->wind);
	DisposePtr((Ptr)RGMwind[w]->name);
	DisposePtr((Ptr)RGMwind[w]);
	RGMwind[w] = nil;
}

short	RGMpoint(short w, short x, short y)
{
	if (RGsetwind(w))
		return -1;

	MoveTo(x,y);
	LineTo(x,y);
} 

short	RGMdrawline(short w, short x0, short y0, short x1, short y1)
{
	long	xl0, yl0, xl1, yl1;
	
	if (RGsetwind(w))
		return -1;

	xl0 = ((long)x0 * RGMwind[w]->width) / INXMAX;
	yl0 = (long)RGMwind[w]->height - (((long)y0 * RGMwind[w]->height) / INYMAX);
	xl1 = ((long)x1 * RGMwind[w]->width) / INXMAX;
	yl1 = (long)RGMwind[w]->height - (((long)y1 * RGMwind[w]->height) / INYMAX);

	MoveTo((short)xl0, (short)yl0);
	LineTo((short)xl1, (short)yl1);
}

void	RGMinfo(short w, short v, short a, short b, short c, short d)
{
	UNUSED_ARG(a)
	UNUSED_ARG(b)
	UNUSED_ARG(c)
	UNUSED_ARG(d)
	RGMwind[w]->vg=v;
}

void	RGMpagedone(short w)
{
	UNUSED_ARG(w)
}

void	RGMdataline(short w, short data, short count)
{
	UNUSED_ARG(w)
	UNUSED_ARG(data)
	UNUSED_ARG(count)
}


void	RGMcharmode(short w, short rotation, short size)
{
	UNUSED_ARG(w)
	UNUSED_ARG(rotation)
	UNUSED_ARG(size)
}

void	RGMgmode(void)
{}

void	RGMtmode(void)
{}

void	RGMshowcur(void)
{
}

void	RGMlockcur(void)
{
}

void	RGMhidecur(void)
{
}

void	RGMbell(short w)
{
	UNUSED_ARG(w)
}

void	RGMuncover(short w)
{
	UNUSED_ARG(w)
}

short	RGMoutfunc(short (*f )())
{
	UNUSED_ARG(f)
	return(0);	// Unused
}

short	RGsetwind(short dnum)
{
	if ( dnum < 0 || dnum >= MAXWIND || RGMwind[dnum] == nil )
		return -1;
	SetPort( RGMwind[dnum]->wind );
	return 0;
}

short	RGfindbyVG( short vg )
{
	short	i = -1;

	while ( ++i < MAXWIND )
		if ( RGMwind[i] != nil && RGMwind[i]->vg == vg )
			return i;
	return -1;
}

short	RGattach(short vg, short virt, char *name, short TEKtype)
{
	short			dnum;
	unsigned long	time;
	char			myname[256],ts[256];

	if ((dnum = RGfindbyVG(vg)) < 0) return(-1);

	RGMwind[dnum]->vs = virt;
	RGMwind[dnum]->name = (unsigned char *) myNewPtr((long) 256);		/* BYU LSC */

	if (RGMwind[dnum]->name == 0L) return(-2);

	myname[0] = '¥';
	if (TEKtype)
		strcpy(&myname[1], "[4105] ");
	else
		strcpy(&myname[1], "[4014] ");
		
	strncpy(&myname[strlen(myname)],name,120);

	GetDateTime(&time);
	IUTimeString(time,FALSE,(StringPtr) ts);		/* BYU LSC - Put time in the temp string */
	p2cstr((StringPtr)ts);											/* BYU LSC */

	strncat(myname,"  ",2);				/* BYU - was 4 - Space, please */
	strncat(myname,ts,64);				/* BYU - Place the time string at the end */
	strcpy((char *) RGMwind[dnum]->name,myname);	/* BYU LSC */

	if (RGMwind[dnum]->wind != (GrafPtr) 0) {	/* BYU LSC */
		c2pstr(myname);							/* BYU LSC */
		SetWTitle(RGMwind[dnum]->wind,(StringPtr)myname);	/* BYU LSC */
	}

	return(0);
}


short	RGdetach( short vg)
{
		short	dnum;			/* BYU */
		char	myname[256];			/* BYU LSC */

	if ((dnum = RGfindbyVG(vg)) < 0) return(-1);		/* BYU */

	if (RGMwind[dnum]->vs != -1) { 									/* BYU */
		if (RGMwind[dnum]->wind != (GrafPtr) 0) {					/* BYU */
#if 1
			strncpy((char *) RGMwind[dnum]->name,				/* BYU LSC */
			        (char *) &RGMwind[dnum]->name[1],255);		/* BYU LSC */
			strncpy(myname, (char *) RGMwind[dnum]->name,256);	/* BYU LSC */
			c2pstr(myname);										/* BYU LSC */
			SetWTitle(RGMwind[dnum]->wind,(StringPtr)myname);				/* BYU LSC */
#else
			strncpy((char *) RGMwind[dnum]->name,				/* BYU LSC */
			        (char *) &RGMwind[dnum]->name[1],255);		/* BYU LSC */
			SetWTitle(RGMwind[dnum]->wind,RGMwind[dnum]->name);	/* BYU LSC */
#endif
		}							/* BYU */
	}								/* BYU */
	return(0);						/* BYU */
}


short	RGfindbywind( GrafPtr wind)
{
	short	i = -1;

	if ( wind ) {
		i = -1;
		while ( ++i < MAXWIND )
			if ( RGMwind[i]->wind == wind )
				return i;
	}
	return -1;
}


short	RGupdate( GrafPtr wind )
{
	GrafPtr		savePort;
	short		rg ;
	short		done;

	if ((rg = RGfindbywind(wind)) < 0)
		return -1;

	GetPort(&savePort);

	SetPort(wind);

	BeginUpdate(wind);

	VGstopred(RGMwind[rg]->vg);
	VGpage(RGMwind[rg]->vg);
	done = VGpred(RGMwind[rg]->vg, RGMwind[rg]->vg);

	EndUpdate(wind);

	if (!done)
		netputevent(USERCLASS, RG_REDRAW, RGMwind[rg]->vg, 0);

	SetPort(savePort);

	return done;
}

short	RGsupdate( short i)
{
	GrafPtr		savePort;
	short		rg;
	short		result;

	if ( (rg = RGfindbyVG(i)) < 0 )
		return 0;

	GetPort(&savePort);
	SetPort(RGMwind[rg]->wind);
	if (!VGpred(RGMwind[rg]->vg,RGMwind[rg]->vg)) {
		netputevent(USERCLASS,RG_REDRAW,i,0);
		result = 0;
	} else
		result = 1;
	SetPort(savePort);
	return result;
}

short	RGgetVG(GrafPtr wind)
{
	short	i;

	i = RGfindbywind(wind);

	return(RGMwind[i]->vg);
}

short	RGgetdnum(GrafPtr wind)
{
	return(RGfindbywind(wind));
}

short	RGgetVS(short dnum)
{
	return(RGMwind[dnum]->vs);
}

short	inSplash(Point *p1, Point *p2)
{
	if ((p1->h - p2->h > 3) || (p2->h - p1->h > 3))
		return(0);
	if ((p1->v - p2->v > 3) || (p2->v - p1->v > 3))
		return(0);

	return(1);
}

void VidSync( void)
{
	long	a;
	a = TickCount();			/* BYU LSC */
	while (a == TickCount())
		/* loop*/;	/* BYU LSC */
}

void RGmousedown
  (
	GrafPtr wind,
	Point *wherein
  )
{
	unsigned long	lx,ly;		/* NCSA: SB */
	char			thispaceforent[6];
	short			i;
	Point			where;
	char			dum[32];

	where = *wherein;
	if ((i = RGfindbywind(wind)) < 0)
		return;

	if (!RGMwind[i]->ingin)
	{
	Point	anchor,current,last;
#if 0							/* BYU LSC - was "#ifndef MPW" */
	long	TickCount();
#endif MPW
	long	tc;
	short	x0,y0,x1,y1;
	Rect	rect;
	
		SetPort(wind);
	
		last  = where;
		current = where;
		anchor = where;
	
		PenPat(PATTERN(qd.gray));    
		PenMode(patXor);
	
		SetRect(&rect,0,0,0,0);
	
		while (StillDown())
		{
			GetMouse(&current);
			if (inSplash(&current,&anchor)) continue;
			tc = TickCount();
			while(TickCount() == tc)
				/* loop */;
			VidSync();
			FrameRect(&rect);
	
			if (anchor.v < current.v)
			{
				rect.top = anchor.v;
				rect.bottom = current.v;
			}
			else
			{
				rect.top = current.v;
				rect.bottom = anchor.v;
			}
	
			if (anchor.h < current.h)
			{
				rect.left = anchor.h;
				rect.right = current.h;
			}
			else
			{
				rect.right = anchor.h;
				rect.left = current.h;
			}
	
			FrameRect(&rect);
			last = current;
		}
	
		FrameRect(&rect);

		PenPat(PATTERN(qd.black));    
		PenMode(patCopy);

		if (!inSplash(&anchor,&current))
		{
			x0 = (short) ((long) rect.left * RGMwind[i]->xscale / RGMwind[i]->width );
			y0 = (short) (RGMwind[i]->yscale -
					(long) rect.top * RGMwind[i]->yscale / RGMwind[i]->height);
			x1 = (short) ((long) rect.right * RGMwind[i]->xscale / RGMwind[i]->width);
			y1 = (short) (RGMwind[i]->yscale -
					(long) rect.bottom * RGMwind[i]->yscale / RGMwind[i]->height);
			x1 = (x1 < x0+2) ? x0 + 4 : x1;
			y0 = (y0 < y1+2) ? y1 + 4 : y0;

			VGzoom( i,
					x0 + RGMwind[i]->xorigin,
					y1 + RGMwind[i]->yorigin,
					x1 + RGMwind[i]->xorigin,
					y0 + RGMwind[i]->yorigin);

			VGpage(RGMwind[i]->vg);

			RGMwind[i]->xscale  = x1 - x0;
			RGMwind[i]->yscale  = y0 - y1;
			RGMwind[i]->xorigin = x0 + RGMwind[i]->xorigin;
			RGMwind[i]->yorigin = y1 + RGMwind[i]->yorigin;

			while(!VGpred(RGMwind[i]->vg,RGMwind[i]->vg))
				Stask();
			RGMlastclick = 0L;
		}
		else
		{
			if (RGMlastclick && ((RGMlastclick + GetDblTime()) > TickCount()))
			{
				RGMwind[i]->xscale  = WINXMAX;	/* BYU LSC - need the "W" here */
				RGMwind[i]->yscale  = WINYMAX;	/* BYU LSC - need the "W" here */
				RGMwind[i]->xorigin = 0;
				RGMwind[i]->yorigin = 0;

				VGzoom(i,0,0,WINXMAX-1,WINYMAX-1);	/* BYU LSC - need the "W" here */
				VGpage( RGMwind[i]->vg);
				while(!VGpred(RGMwind[i]->vg,RGMwind[i]->vg))
					Stask();
				RGMlastclick = 0L;
			}
			else RGMlastclick = TickCount();
		}
		return;
	
	}
	
	/* NCSA: SB */
	/* NCSA: SB - These computations are being truncated and turned into signed ints. */
	/* NCSA: SB - Just make sure everything is cast correctly, and were fine */
	
	lx = ((unsigned long)RGMwind[i]->xscale * (unsigned long)where.h) 	/* NCSA: SB */
			/ (unsigned long)RGMwind[i]->width;						 	/* NCSA: SB */
	ly = (unsigned long)RGMwind[i]->yscale - 							/* NCSA: SB */
		((unsigned long)RGMwind[i]->yscale * (unsigned long)where.v) / (unsigned long)RGMwind[i]->height; /* NCSA: SB */

	VGgindata(RGMwind[i]->vg,(unsigned short) lx,(unsigned short)ly,' ',thispaceforent);	/* NCSA: SB */

#if 0													/* BYU LSC - not needed */
sprintf(dum,"GIN i: %d, vs: %d\015",i,RGMwind[i]->vs);	/* BYU 2.4.18 - changed \n to \015 */
putln(dum);
#endif													/* BYU LSC - not needed */

	RSsendstring(RGMwind[i]->vs,thispaceforent,5);
	sprintf(dum," \r\n");
	RSsendstring(RGMwind[i]->vs,dum,3);
	

    /*	RGMwind[i]->ingin = 0; */
	unsetgraphcurs();
	RGMlastclick = TickCount();
}


/*--------------------------------------------------------------------------*/
/* NCSA: SB - RGMgrowme														*/
/* This allows the TEK window to be grown like any other real window.		*/
/* The grow box is not shown on the screen, but it acts accordingly	 - SMB	*/
/*--------------------------------------------------------------------------*/
void	RGMgrowme(short myRGMnum,GrafPtr window, long *where, short modifiers)
{
	UNUSED_ARG(modifiers)
	long size;
	Rect SizRect;

	SetRect(&SizRect, 50, 50, 1500, 1500);			/* NCSA: SB - max size - arbitrary */

	if (where)
	{		
		size = GrowWindow(window, *(Point *) where, &SizRect);	
		if (size != 0L)
			{
			SizeWindow(window, size & 0xffff, (size >> 16) & 0xffff, FALSE);
			RGMwind[myRGMnum]->width   = window->portRect.right - window->portRect.left;		
			RGMwind[myRGMnum]->height  = window->portRect.bottom - window->portRect.top;		 
			VGpage(RGMwind[myRGMnum]->vg);
			InvalRect(&window->portRect);
			RGupdate(window);
			}
		else
			return;							/* user skipped growing */
	}
}


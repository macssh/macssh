/*
** Raster Virtual Kernel (vr.c)
*/

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#define MASTERDEF



#include "vr.h"
#include "vr.proto.h"
#include "vrrgmac.proto.h"

/*	defines for raster states and variables */

/*	states	*/
#define	DONE		0		/* we've been called */
#define	ESCFOUND	1		/* found escape */
#define	WANTCMD		2		/* want a command char */
#define	WANTDEL		3		/* want that first delimiter */
#define	IARG		4		/* looking for integer arg */
#define	SARG		5		/* looking for string arg */
#define	CARG		6		/* looking for count arg */
#define	DATA		7		/* all args parsed, found ^ */

/*	commands */
#define ESC		0x1b			/* start of a sequence */
#define	ESCCMD	' '				/* escape to next level of commands */
#define	DELIM	';'				/* argument delimiter */
#define	CMDTRM	'^'				/* terminator, but also prefix w/ ESC */
#define	WINCMD	'W'				/* create a window */
#define	DESCMD	'D'				/* destroy a window */
#define	MAPCMD	'M'				/* change color map entries */
#define	RLECMD	'R'				/* run-length encoded data */
#define	PIXCMD	'P'				/* standard pixel data */
#define IMPCMD  'I'             /* IMCOMP compressed data */
#define	FILCMD	'F'				/* save to file command */
#define	CLKCMD	'C'				/* click the slide camera */
#define	SAVCMD	'S'				/* save a color map to a file */

/*	command parameter types*/
#define	MAXARGS	7				/* maximum args to a command */
#define INT		1				/* integer argument */
#define	STRING	2				/* string (character) argument */
#define	COUNT	3				/* data count argument */
#define LINEMAX 1024			/* longest width for window */

/*
** flag values
*/

#define FL_NORMAL	1		/* command needs no data */
#define	FL_DATA		2		/* command takes data */

/*
** structure of command table
*/

struct cmd {
	char	c_name;
	short	c_flags;
	short	(*c_func)(union arg av[], char *);
	short	c_args[MAXARGS];
};

static	struct cmd cmdtab[] = {
	WINCMD, FL_NORMAL, VRwindow, {INT, INT, INT, INT, INT, STRING, 0},
	DESCMD, FL_NORMAL, VRdestroy, {STRING, 0, 0, 0, 0, 0, 0},
	MAPCMD, FL_DATA, VRmap, {INT, INT, COUNT, STRING, 0, 0, 0},
	FILCMD, FL_NORMAL, VRfile, {INT, INT, INT, INT, INT, STRING, STRING},
	PIXCMD, FL_DATA, VRpixel, {INT, INT, INT, COUNT, STRING, 0, 0},
	RLECMD, FL_DATA, VRrle, {INT, INT, INT, COUNT, STRING, 0, 0},
	IMPCMD, FL_DATA, VRimp, {INT, INT, INT, COUNT, STRING, 0, 0},
	CLKCMD, FL_NORMAL, VRclick, {STRING, 0, 0, 0, 0, 0, 0},
	SAVCMD, FL_NORMAL, VRmsave, {STRING, STRING, 0, 0, 0, 0, 0}
};

#define NCMDS	(sizeof(cmdtab) / sizeof(struct cmd))

/*
** global vars
*/

static short			VRstate = DONE;		/* current state */
static short			VRcmdnum = -1;		/* current command */
static short			VRargcount = 0;		/* number of args for command */
static short			VRdatalen = 0;		/* length of expected data */
static short			VRbufpos = 0;		/* current pointer in tempdata */
static union arg	VRargs[MAXARGS];	/* argument vector */
static char			VRtempdata[256];	/* temporary storage while parsing */
static char         *VRspace;			/* storage for incoming data */
static char         *VRsp2;				/* storage for pixel expansion */
static char			*VRptr;				/* pointer for data buffer */
static char			*VRptr2;			/* copy of above */

/***********************************************************************/
/* decode0 and decode1
*  start and continue the decoding.
*
*  Returns real characters, 0 if in the middle of an escape sequence.
*/

#define FRSKIP 0
#define FRIN 1
#define FRSPECIAL 2
#define FROVER 3
#define FRDONE 4

void	ICRunload(void)	{}

static short VRnextargstate(short ac, short c);

/*
** VRinit	-- initialize the VR system
**
** Arguments:
**
**	None.
**
** Returns:
**
**	short		-- status, 1 == succsessful, 0 == failed
*/

short	VRinit( void)
{
	VRhead.w_next = NULL;
	VRsp2 = (char *)myNewPtr(4*LINEMAX+10);
	VRspace = (char *)myNewPtr(LINEMAX+10);

	if (VRspace)
		return 1;
	else
		return 0;
}

/*
** VRwrite -- parse a string of VR commands
** 
** Arguments:
**
**	char *b;	-- buffer pointer
**	short	len;	-- buffer length
**
** Returns:
**
**	short			-- Number of characters processed.  0 tells
**				-- the upper level to switch out of raster mode;
**				-- usually on error, but also at completion of
**				-- command processing.
**
*/

short	VRwrite( char *b, short len)
{
	short		count = 0;
	char		*p = b;
	char		c;
	short		i;

	/*	loop 'til no more chars */

	while (count < len) {
		c = *p;

		switch (VRstate) {
			case DONE:
				if (c == ESC)
					VRstate = ESCFOUND;
				else
					return count;
				break;

			case ESCFOUND:
				if (c == CMDTRM)
					VRstate = WANTCMD;
				else {
					VRstate = DONE;
					return count;
				}
				break;

			/*	looking for a valid command char */
			case WANTCMD:
				for (i = 0; i < NCMDS; i++) {
					if (cmdtab[i].c_name == c)
						break;
					}

				VRcmdnum = i;

				if (VRcmdnum == NCMDS) {
					VRstate = DONE;
					return 0;
					}

				/*	set up for this command */
				VRargcount = 0;
				VRbufpos = 0;

				VRstate = WANTDEL;
				break;

			/*	look for that first ; */
			case WANTDEL:
				if (c == DELIM)
					VRstate = VRnextargstate(VRargcount, VRcmdnum);
				else
					VRstate = DONE;
				break;

			/*	looking for an integer arg */
			case IARG:
				switch (c) {

					/*	we've found the end of the argument, so
						try to put into the vector for later use */
					case DELIM:
					case CMDTRM:
						VRtempdata[VRbufpos] = '\0';

						/*	copy into argument union */
						(void)sscanf(VRtempdata,"%d",&VRargs[VRargcount].a_num);
						VRbufpos = 0;

						VRargcount++;
						if (c == DELIM)
							VRstate = VRnextargstate(VRargcount, VRcmdnum);
						else
							if (cmdtab[VRcmdnum].c_flags & FL_DATA)
								VRstate = DATA;
							else {	/*	run the command */
								(*cmdtab[VRcmdnum].c_func)(VRargs, (char *)0L);
								VRstate = DONE;
							}
						break;

					/*	copy over characters for later */
					default:
						VRtempdata[VRbufpos++] = c;
				}
				break;

			/*	looking for string arg */
			case SARG:
				switch (c) {
					/*	put string into argument vector */
					case DELIM:
					case CMDTRM:
						
						VRtempdata[VRbufpos] = '\0';
/*
						VRargs[VRargcount].a_ptr = myNewPtr((unsigned)VRbufpos+1);
						
						if (VRargs[VRargcount].a_ptr == (char *)0L) {
							VRstate = DONE;
							break;
						}
*/

						(void)strcpy(VRargs[VRargcount].a_ptr, VRtempdata);
						VRbufpos = 0;
						VRargcount++;
						if (c == DELIM)
							VRstate = VRnextargstate(VRargcount, VRcmdnum);
						else
							if (cmdtab[VRcmdnum].c_flags & FL_DATA)
								VRstate = DATA;
							else {	/*	run the command */
								(*cmdtab[VRcmdnum].c_func)(VRargs, (char *)0L);
								VRstate = DONE;
							}
						break;

					/*	save string for later */
					default:
						VRtempdata[VRbufpos++] = c;
				}
				break;

			/*	looking for a count argument */
			case CARG:
				switch (c) {
					/*	we've found the end of the argument, so
						try to put into the vector for later use */
					case DELIM:
					case CMDTRM:
						VRtempdata[VRbufpos] = '\0';

						/*	copy into argument union */
						(void)sscanf(VRtempdata,"%d",&VRdatalen);
						(void)sscanf(VRtempdata,"%d",&VRargs[VRargcount].a_num);

						if (VRdatalen > LINEMAX)
							VRdatalen = LINEMAX;
						VRargcount++;
						VRbufpos = 0;

						if (c == DELIM)
							VRstate = VRnextargstate(VRargcount, VRcmdnum);
						else
							if (cmdtab[VRcmdnum].c_flags & FL_DATA)
								VRstate = DATA;
							else {	/*	run the command */
								(*cmdtab[VRcmdnum].c_func)(VRargs, (char *)0L);
								VRstate = DONE;
							}

						/*	allocate storage for data */
						VRptr = VRspace;

						if (VRptr == (char *)0L) {
							VRstate = DONE;
							return 0;
							}
	
						VRptr2 = VRptr;
						decode0();			/* reset decoder */
						break;

					/*	copy over characters for later */
					default:
						VRtempdata[VRbufpos++] = c;
				}
				break;

			/*	retrieve a line of data */
			case DATA:
				/*	store bytes until done */
				if (0 <= (i = decode1(c))) {
					*VRptr2++ = i;
					--VRdatalen;
					}

				if (!VRdatalen) {	/*	we've got all of the data */
					(*cmdtab[VRcmdnum].c_func)(VRargs, VRptr);
					VRstate = DONE;
					}
				break;

		} /* end switch(VRstate)*/

		p++;
		count++;
	} /* end while loop */

	return count;
}

/*
** VRnextargstate -- return the next state based on where we're
**				  -- at already.
**
** Arguments:
**
** short ac         -- current argument count
** short state 	  -- current state
** short c    	  -- current command
**
** Returns:
**
** short       	  -- next state to move to
**
*/

static short VRnextargstate(short ac, short c)
{

	switch (cmdtab[c].c_args[ac]) {
		case INT:
			return IARG;
		case STRING:
			return SARG;
		case COUNT:
			return CARG;
		case 0:
			return DATA;

		/*	in case of error*/
		default:
			return DONE;
	}
}

/*
** VRwindow -- create a new raster window
**
** Arguments:
**
**	union arg av[];	-- the argument vector
**
**	short		av[0, 1]; 	-- upper left;
**	short		av[2, 3]; 	-- width, height
**	short		av[4];	 	-- window hardware display number
**	char	*av[5];   	-- title
**
** Returns:
**
**	None.  No provision has been made for error returns in any of
**	these routines because I don't know what to do if an error
**	occurs.  Perhaps a better man than I can figure out how to 
**	deal with this.
**	N.B -- these functions are declared as short, in the event
**	that an error return is added.
*/

short	VRwindow(union arg av[], char *unused)
{
	UNUSED_ARG(unused)
	VRW	*w = VRhead.w_next;
	short	ret;
	
	/*	search list, and if needed, myNewPtr some space for a new window thing */

	while (w) {
		if (!strcmp(w->w_name,av[5].a_ptr)
			&& w->w_width == av[2].a_num 
			&& w->w_height == av[3].a_num)		/* don't re-allocate win */
			return(1);
		if (!strcmp(w->w_name,av[5].a_ptr))		/* duplicate, different size */
			w->w_used = 1;
		w = w->w_next;
	}

	w = (VRW *)myNewPtr(sizeof(VRW));				/* new element for list */
	if (!w)
		return(-1);
		
	w->w_next = VRhead.w_next;					/* set next equal to current head */
	VRhead.w_next = w;							/* set head of list equal to me */
	
	/*
	** fill in the new window area
	*/

	w->w_left = av[0].a_num;
	w->w_top = av[1].a_num;
	w->w_width = av[2].a_num;
	w->w_height = av[3].a_num;
	w->w_display = av[4].a_num;
	w->w_used = 0;
	strncpy(w->w_name,av[5].a_ptr,100);

	if (w->w_width > LINEMAX)			/* have to be SOME limits */
		w->w_width = LINEMAX;

	if (0 <= (ret = MacRGnewwindow( w->w_name, w->w_left, w->w_top,
					w->w_left+w->w_width, w->w_top+w->w_height )))
		MacRGsetwindow( ret);

	w->w_rr.wn = ret;

	return(ret);
}

/*
** VRdestroy -- destroy a window by name
**
** Arguments:
**
**   union arg av[]; -- the argument vector
**
**   char *av[0] -- the name of the window
**
** Returns:
**
**   None.
**
*/

short	VRdestroy(union arg av[], char *unused)
{
	UNUSED_ARG(unused)
	VRW		*w,*ow;

	ow = &VRhead;
	w = ow->w_next;

	while (w) {
		if (!strcmp(w->w_name, av[0].a_ptr) ) {
			MacRGremove( w->w_rr.wn);
			ow->w_next = w->w_next;
			DisposePtr((char *)w);
		}
		else
			ow = ow->w_next;

		w = ow->w_next;
	}

	return 0;
}

void	VRdestroybyName(char *name)
{
	union arg blah;
	
	strncpy(blah.a_ptr, name, 100);
	VRdestroy(&blah, NULL);
}

/*
** VRmap -- take a color map command and set the palette
**
** Arguments:
**
**   union arg av[]; -- the argument vector
**
**   short 	av[0, 1];  -- start & length of map segment
**	 short	av[2];     -- count of data needed (info only)
**	 char	*av[3];    -- window name
**   char	*data;     -- pointer to the data  
**
** Returns:
**
**   None.
**
*/

short	VRmap(union arg av[], char *data)
{
	VRW			*w;

	w = VRlookup(av[3].a_ptr);

	if (!w)
		return 0;

	return MacRGmap(av[0].a_num, av[1].a_num, data);
}

/*
** VRpixel -- display a line of pixel data
** 
** Arugments:
**
**	union arg av[]; -- the argument vector
**
**	short		av[0];  -- x coordinate
**	short	 	av[1];  -- y coordinate
**  short     av[2];  -- pixel expansion factor
**	short	 	av[3];  -- length of data
**	char	*av[4]; -- window name
**	char	*data;  -- pointer to data
**
** Returns:
**
**	None.
**
*/

short	VRpixel(union arg av[], char *data)
{
	VRW		*w;
	short	i,lim;
	char	*p,*q;

	/*	find the right window */
	w = VRlookup(av[4].a_ptr);

	if (w == (VRW *)0L)
		return 0;

	lim = av[3].a_num*av[2].a_num;		/* total number of expanded pixels */
	if (lim > w->w_width)
		lim = w->w_width;

	if (av[2].a_num > 1) {
		p = data;
		q = VRsp2;
		for (i=0; i < lim; i++) {
			*q++ = *p;
			if (!((i+1) % av[2].a_num))
				p++;
		}
		for (i=0; i<av[2].a_num; i++)
			MacRGraster( VRsp2, av[0].a_num, av[1].a_num+i,
							 av[0].a_num+lim, av[1].a_num+i,lim);

	}
	else
		return 	MacRGraster( data, av[0].a_num, av[1].a_num,
					av[0].a_num+av[3].a_num, av[1].a_num,lim);

	return 0;
}

/*
** VRimp -- display a line of IMPCOMP encoded data
**   One line of IMPCOMP data gives 4 lines output.
** 
** Arugments:
**
**	union arg av[]; -- the argument vector
**
**	short		av[0];  -- x coordinate
**	short	 	av[1];  -- y coordinate
**  short     av[2];  -- pixel expansion
**	short	 	av[3];  -- length of data
**	char	*av[4]; -- window name
**	char	*data;  -- pointer to data
**
** Returns:
**
**	None.
**
*/

short	VRimp(union arg av[], char *data)
{
	VRW		*w;
	short	i,lim,j;
	char	*p,*q;

	/*	find the right window */
	w = VRlookup(av[4].a_ptr);

	if (w == (VRW *)0L)
		return 0;

	unimcomp((unsigned char *) data,(unsigned char *) VRsp2,av[3].a_num,w->w_width);   /* BYU LSC - decompress it */
	/* gives four lines in the VRsp2 buffer, now pixel expand */

	i = av[3].a_num;

	lim = i*av[2].a_num;		/* total number of expanded pixels on a line*/
	if (lim > w->w_width)
		lim = w->w_width;
	if (i > w->w_width)
		i = w->w_width;

	p = VRsp2;								/* from this buffer */
	for (j=0; j<4; j++) {
		if (av[2].a_num > 1) {
			q = VRspace;					/* to here */
			for (i=0; i < lim; i++) {
				*q++ = *p;
				if (!((i+1) % av[2].a_num))
					p++;
			}
			p++;
			for (i=0; i<av[2].a_num; i++)
				MacRGraster(VRspace, av[0].a_num, av[1].a_num+i+j*av[2].a_num,
					av[0].a_num + av[3].a_num,    av[1].a_num+i+j*av[2].a_num, lim );

		}
		else {
			MacRGraster(p, av[0].a_num,        av[1].a_num+j,
					av[0].a_num + av[3].a_num, av[1].a_num+j, i);
			p += av[3].a_num;			/* increment to next line */
		}

	}

	return 0;
}

/************************************************************************/
/*  Function	: unimcomp						*/
/*  Purpose	: 'Decompresses' the compressed image			*/
/*  Parameter	:							*/
/*    xdim       - x dimensions of image				*/
/*    lines      - number of lines of compressed image                  */
/*    in, out    - Input buffer and output buffer. Size of input buffer */
/*		   is xdim*lines. Size of output buffer is 4 times      */
/*		   that. It 'restores' images into seq-type files       */
/*  Returns  	: none							*/
/*  Called by   : External routines					*/
/*  Calls       : none							*/
/************************************************************************/

void unimcomp(unsigned char in[], unsigned char out[], short xdim, short xmax)
{
  short				bitmap, temp, lines=1;
  register short 	i, j, k, x;
  unsigned char		hi_color, lo_color;

	if (xmax < xdim)		/* don't go over */
		xdim = xmax;

  /* go over the compressed image */
    for (x=0; x<xdim; x=x+4)
    {
      k = x;
      hi_color = in[k+2]; 
      lo_color = in[k+3];

      bitmap = (in[k] << 8) | in[k+1];

      /* store in out buffer */
      for (i=0; i<4; i++)
      {
        temp = bitmap >> (3 - i)*4;
        for (j=x; j<(x+4); j++)
        {
 	  if ((temp & 8) == 8)
	    out[i*xdim+j] = hi_color;
	  else
	    out[i*xdim+j] = lo_color;
	  temp = temp << 1;
	}
      }
    } /* end of for x */
} /* end of unimcomp */

/*  unrleit - Decompress run length encoding. */
short	unrleit(unsigned char *buf, unsigned char *bufto, short inlen, short outlen)
{
	register short			cnt;
	register unsigned char	*p,*q;
	unsigned char			*endp,*endq;
	
	p = buf;
	endp = buf + inlen;
	q = bufto;
	endq = bufto + outlen;
	while (p < endp && q < endq) {	/* go 'til p or q hits end */
		cnt = *p++;					/* count field */
		if (!(cnt & 128)) {			/* is set of uniques */
			while (cnt-- && q < endq)
				*q++ = *p++;		/* copy unmodified */
			}
		else {
			cnt &= 127;				/* strip high bit */
			while (cnt-- && q < endq)
				*q++ = *p;			/* copy same character */
			p++;					/* skip that character */
		}
	} /* while */

	return((short)(q-bufto));
}

/***************************************************************************/
/*
** VRrle -- display a line of run-length encoded data
** 
** Arugments:
**
**	union arg av[]; -- the argument vector
**
**	short		av[0];  -- x coordinate
**	short	 	av[1];  -- y coordinate
**  short     av[2];  -- pixel expansion
**	short	 	av[3];  -- length of data
**	char	*av[4]; -- window name
**	char	*data;  -- pointer to data
**
** Returns:
**
**	None.
**
*/
short	VRrle(union arg av[], char *data)
{
	VRW 	*w;
	short	i,lim;
	char	*p,*q;

	/*	find the right window */

	w = VRlookup(av[4].a_ptr);

	if (w == (VRW *)0L)
		return 0;

	i = unrleit((unsigned char *) data,(unsigned char *) VRsp2,av[3].a_num,w->w_width);   /* BYU LSC - decompress it */


	lim = i*av[2].a_num;				/* total number of expanded pixels */
	if (lim > w->w_width)
		lim = w->w_width;

	if (av[2].a_num > 1) {
		p = VRsp2;						/* from this buffer */
		q = VRspace;					/* to here */
		for (i=0; i < lim; i++) {
			*q++ = *p;
			if (!((i+1) % av[2].a_num))
				p++;
		}
		for (i=0; i<av[2].a_num; i++)
			MacRGraster( VRspace, av[0].a_num, av[1].a_num+i,
							 av[0].a_num+lim, av[1].a_num+i,lim);

	}
	else
		return 	MacRGraster( VRsp2, av[0].a_num, av[1].a_num,
					av[0].a_num+i, av[1].a_num,i);

	return 0;

}

/*
** VRfile	-- cause the named window to be dumped to a file
**
** Arguments:
**
**	union arg av[];		-- the argument vector
**
**	short		av[0];		-- start x coordinate
**	short		av[1];		-- start y coordinate
**	short		av[2];		-- width of region to dump
**	short		av[3];		-- height of region to dump
**	short		av[4];		-- format of file -- machine dependent
**	char	*av[5];		-- file name to use
**	char	*av[6];		-- window name to dump
**
** Returns:
**
**	short;				-- 1 if success, 0 if failure
**
*/

short	VRfile(union arg av[], char *unused)
{
#ifdef IMPLEMENTED
	#pragma unused (unused)
	VRW	*w;

	/*	Look up the window */

	w = VRlookup(av[6].a_ptr);

	if (w == (VRW *)0L)
		return 0;

	/*	call the low level file routine */

	return RRfile(w, av[0].a_num, av[1].a_num, av[2].a_num, av[3].a_num,
		av[4].a_num, av[5].a_ptr);
#else 
	UNUSED_ARG(av)
	UNUSED_ARG(unused)
	return 0;
#endif IMPLEMENTED
}

/*
** VRclick	-- Click the Slide Camera -- very machine dependent
**
** Arguments:
**
**	union arg	av[];		-- the argument vector
**
**	char	*av[0];			-- window name
**
** Returns:
**
**	short;					-- 1 if success, 0 if failure
*/

short	VRclick(union arg av[], char *unused)
{
#ifdef IMPLEMENTED
	#pragma unused (unused)
	VRW	*w;

	/*	look up the window */

	w = VRlookup(av[0].a_ptr);

	if (w == (VRW *)0L)
		return 0;

	return RRclick(w);
#else 
	UNUSED_ARG(av)
	UNUSED_ARG(unused)
	return 0;
#endif IMPLEMENTED
}

/*
** VRmsave	-- Save the named colormap to the file -- saves the whole thing
**
** Arguments:
**
**	union arg	av[];		-- the argument vector
**
**	char	*av[0];			-- file name
**	char	*av[1];			-- window name
**
** Returns:
**
**	short;					-- 1 if success, 0 if failure
*/

short	VRmsave(union arg av[], char *unused)
{
#ifdef IMPLEMENTED
	#pragma unused (unused)
	VRW	*w;

	/*	look up the window */
	w = VRlookup(av[1].a_ptr);

	if (w == (VRW *)0L)
		return 0;

	return RRmsave(w, av[0].a_ptr);
#else 
	UNUSED_ARG(av)
	UNUSED_ARG(unused)
	return 0;
#endif IMPLEMENTED
}

/*
** VRlookup			-- find an entry in the list by name
**
** Arguments:
**
**	char	*name;	-- the name of the window
**
** Returns:
**
**	VRW	*w;			-- pointer to window structure, (VRW *)0 if not found
**
*/

VRW	*VRlookup(char *name)
{
	VRW	*w = VRhead.w_next;

	while (w) {
		if (!strcmp(w->w_name, name) && !w->w_used) {	/* same name, not old dup */
			if (w->w_rr.wn < 0)							/* maybe window don't work */
				return(NULL);
			MacRGsetwindow(w->w_rr.wn);
			return(w);
			}
		w = w->w_next;
	}

	return(NULL);
}

/*
** VRcleanup -- remove all windows from the screen
**
** Arguments:
**
**	None.
**
** Returns:
**
**	short;	1	-- always successful, or there's nothing you can do
**				-- about it anyways...
**
*/

short	VRcleanup(void)
{
	VRW *w = VRhead.w_next;
	VRW *x;

	while (w != (VRW *)0L) {
		x = w->w_next;
		MacRGremove( w->w_rr.wn);
		w = x;
	}

	return 1;
}

/************************************************************************/
/*  decoding
*   handle the special ASCII printable character encoding of data bytes.
*/

/***********************************************************************/
/*
*  123 precedes #'s 0-63 
*  124 precedes #'s 64-127
*  125 precedes #'s 128-191
*  126 precedes #'s 192-255
*  overall:  realchar = (specialchar - 123)*64 + (char-32) 
*            specialchar = r div 64 + 123
*            char = r mod 64 + 32
*/
/***********************************************************************/
static short dstate=FRSKIP,dspec=0;

/*	set up receive */
void	decode0(void)
{
	dstate = FRIN;
}

short	decode1(char c)
{
	switch (dstate) {
		case FRSKIP:
			return(-1);
	
		case FRIN:						/* decoding */
			if (c > 31 && c < 123)
				return(c);
			else {
				dspec = c;				/* save special character */
				dstate = FRSPECIAL;		/* doing special character */
			}
			return(-1);
			
		case FRSPECIAL:
			switch (dspec) {
				case 123:
				case 124:
				case 125:
				case 126:			/* encoded character */
					dstate = FRIN;
					return(((dspec - 123)<< 6) - 32 + c);
					
				default:			/* mistaken character in stream */
					dstate = FRIN;  /* assume not special */
					return(decode1(c));  /* check for sure */
					break;
				}
		break;
	} // switch (dstate)
}

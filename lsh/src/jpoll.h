/* jpoll.h
 *
 * Header file for my poll() SVID3 emulation function.
 *
 * $Id$ */

/*
 * AUTHOR:     Sean Reifschneider <jafo@tummy.com>
 * DATE:       1998-10-10
 * Copyright (c) 1998 Sean Reifschneider
 *
 */

#ifndef LSH_POLL_H_INCLUDED
#define LSH_POLL_H_INCLUDED

#define POLLIN		0x0001		/*  check for input  */
#define POLLOUT	0x0004		/*  check for output  */

struct pollfd {
	int fd;				/*  file descriptor to poll  */
	short events;		/*  events we are interested in  */
	short revents;		/*  events that occured  */
	};

typedef unsigned int nfds_t;

int poll(struct pollfd *fdlist, nfds_t count, int timeoutInMS);

#endif /* LSH_POLL_H_INCLUDED */

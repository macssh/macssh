/* client.h
 *
 * Utility functions for the client side of the protocol.
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2001 Niels Möller
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */

#ifndef SFTP_CLIENT_H_INCLUDED
#define SFTP_CLIENT_H_INCLUDED

#include "buffer.h"

struct client_ctx
{
  struct sftp_input *i;
  struct sftp_output *o;

  /* Status from latest message. */
  UINT32 status;
};

/* Handles are strings, choosen by the server. */
struct client_handle
{
  UINT32 length;
  UINT8 *data;
};

/* Creates a file handle */
struct client_handle *
sftp_open(struct client_ctx *ctx,
	  const char *name,
	  UINT32 flags,
	  const struct sftp_attrib *a);

/* Destroys a file or directory handle */
int
sftp_close(struct client_ctx *ctx,
	   struct client_handle *handle);


#endif /* SFTP_CLIENT_H_INCLUDED */

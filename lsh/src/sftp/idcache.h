/* idcache.h
 *
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

#ifndef SFTP_IDCACHE_H_INCLUDED
#define SFTP_IDCACHE_H_INCLUDED

struct sftp_user_info;

int
sftp_cache_assoc(struct sftp_user_info **cache,
		 int id,
		 const char **name);

const char *
sftp_cache_push(struct sftp_user_info **cache,
		int id, const char *name);

#endif /* SFTP_IDCACHE_H_INCLUDED */

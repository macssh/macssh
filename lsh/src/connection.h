/* connection.h
 *
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Niels Möller
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

#ifndef LSH_CONNECTION_H_INCLUDED
#define LSH_CONNECTION_H_INCLUDED

#include "abstract_io.h"
#include "abstract_compress.h"
#include "queue.h"
#include "resource.h"
#include "randomness.h"


#define GABA_DECLARE
#include "connection.h.x"
#undef GABA_DECLARE

/* This is almost a write handler; difference is that it gets an extra
 * argument with a connection object. */

/* GABA:
   (class
     (name packet_handler)
     (vars
       (handler method void
               "struct ssh_connection *connection"
	       "struct lsh_string *packet")))
*/

#define HANDLE_PACKET(closure, connection, packet) \
((closure)->handler((closure), (connection), (packet)))

#define DEFINE_PACKET_HANDLER(SPEC, NAME, CARG, PARG)	\
static void						\
do_##NAME(struct packet_handler *,			\
	  struct ssh_connection *,			\
	  struct lsh_string *);				\
							\
SPEC struct packet_handler NAME =			\
{ STATIC_HEADER, do_##NAME };				\
							\
static void						\
do_##NAME(struct packet_handler *s UNUSED,		\
	  struct ssh_connection *CARG,			\
	  struct lsh_string *PARG)


/* NOTE: These are used both for indexing the two-element arrays in
 * the connection object. But they are also used in the flags field,
 * to indicate our role in the protocol.
 * For instance,
 *
 *   connection->versions[connection->flags & CONNECTION_MODE]
 *
 * is the version string we sent. Furthermore, install_keys() depends
 * on the numerical values of CONNECTION_SERVER and CONNECTION_CLIENT. */

#define CONNECTION_MODE 1

#define CONNECTION_CLIENT 0
#define CONNECTION_SERVER 1

/* Set if SRP keyexchange was used. */
#define CONNECTION_SRP 2

/* Use a different encoding of ssh-dss signatures, for compatibility with
 * SSH Inc's ssh version 2.0.x and 2.1.0 */
#define PEER_SSH_DSS_KLUDGE           0x00000001

/* Support SSH_MSG_SERVICE_ACCEPT with omitted service name, for
 * compatibility with SSH Inc's ssh version 2.0.x */
#define PEER_SERVICE_ACCEPT_KLUDGE    0x00000002

/* Replace the service name with the string "ssh-userauth" in
 * publickey userauth requests, for compatibility with SSH Inc's ssh
 * version 2.0.x and 2.1.0 */
#define PEER_USERAUTH_REQUEST_KLUDGE  0x00000004

/* Never send a debug message after successful keyexchange, as SSH
 * Inc's ssh version 2.0.x and 2.1 can't handle that. */
#define PEER_SEND_NO_DEBUG            0x00000008

/* Don't include the originator port in X11 channel open messages, for
 * compatibility with SSH Inc's ssh version 2.0.x */
#define PEER_X11_OPEN_KLUDGE          0x00000010

/* ssh version 2.0.13 workaround */
#define PEER_WRONG_PK_OK              0x00000020

/* GABA:
   (class
     (name ssh_connection)
     (super abstract_write)
     (vars
       ; Where to pass errors
       (e object exception_handler)

       ; Connection flags
       (flags . UINT32)

       ; Sent and received version strings
       (versions array (string) 2)
       (session_id string)
       
       ; Connection description, used for debug messages.
       (debug_comment . "const char *")

       ; Features or bugs peculiar to the peer
       (peer_flags . UINT32)
       
       ; Information about a logged in user. NULL unless some kind of
       ; user authentication has been performed.
       (user object lsh_user)

       ; The chained connection in the proxy, or gateway.
       (chain object ssh_connection)

       ; Cleanup
       (resources object resource_list)

       ; Connected peer
       ; FIXME: Perhaps this should be a sockaddr or some other object
       ; that facilitates reverse lookups?
       (peer object address_info);
       
       ; Receiving
       (rec_max_packet . UINT32)
       (rec_mac    object mac_instance)
       (rec_crypto object crypto_instance)
       (rec_compress object compress_instance)

       ; Sending 
       (raw   object abstract_write)  ; Socket connected to the other end 
       (write object abstract_write)  ; Where to send packets through the
                                      ; pipeline.

       (send_mac object mac_instance)
       (send_crypto object crypto_instance)
       (send_compress object compress_instance)

       ; For operations that require serialization. In particular
       ; the server side of user authentication. 
       
       (paused . int)
       (pending struct string_queue)
       
       ; Key exchange 
       (kex_state . int)

       ; What to do once the connection is established
       (established object command_continuation)
       
       (kexinits array (object kexinit) 2)
       (literal_kexinits array (string) 2)

       ; Table of all known message types 
       (dispatch array (object packet_handler) "0x100")
       
       ; Table of all opened channels
       (table object channel_table)
       
       ; (provides_privacy . int)
       ; (provides_integrity . int)
       ))
*/

#define C_WRITE(c, s) A_WRITE((c)->write, (s) )

struct ssh_connection *
make_ssh_connection(UINT32 flags,
		    struct address_info *peer,
		    const char *id_comment,
		    struct command_continuation *c,
		    struct exception_handler *e);

void connection_init_io(struct ssh_connection *connection,
			struct abstract_write *raw,
			struct randomness *r);

struct lsh_callback *
make_connection_close_handler(struct ssh_connection *c);

/* Serialization */
void connection_lock(struct ssh_connection *self);
void connection_unlock(struct ssh_connection *self);

/* Table of packet types */
extern const char *packet_types[0x100];

/* Simple packet handlers. */
extern struct packet_handler connection_ignore_handler;
extern struct packet_handler connection_unimplemented_handler;
extern struct packet_handler connection_fail_handler;
extern struct packet_handler connection_forward_handler;

#endif /* LSH_CONNECTION_H_INCLUDED */

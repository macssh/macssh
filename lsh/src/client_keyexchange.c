/* client_keyexchange.c
 *
 * $Id$ */

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

#include "client_keyexchange.h"

#include "atoms.h"
#include "command.h"
#include "debug.h"
#include "dsa.h"
#include "format.h"
#include "lookup_verifier.h"

#include "srp.h"
#include "ssh.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>

#include "client_keyexchange.c.x"

/* GABA:
   (class
     (name dh_client_exchange)
     (super keyexchange_algorithm)
     (vars
       (dh object dh_method)))
*/

/* Handler for the kex_dh_reply message */
/* GABA:
   (class
     (name dh_client)
     (super packet_handler)
     (vars
       (dh struct dh_instance)
       (hostkey_algorithm . int)
       (verifier object lookup_verifier)
       (algorithms object object_list)))
*/
    
static void
do_handle_dh_reply(struct packet_handler *c,
		   struct ssh_connection *connection,
		   struct lsh_string *packet)
{
  CAST(dh_client, closure, c);
  struct lsh_string *server_key;
  int algorithm;
  struct lsh_string *signature;
  struct verifier *v;
  int res;
  
  trace("handle_dh_reply\n");

  server_key = dh_process_server_msg(&closure->dh, &signature, packet);
  lsh_string_free(packet);
  if (!server_key)
    {
      disconnect_kex_failed(connection, "Bad dh-reply\r\n");
      return;
    }
    
  v = LOOKUP_VERIFIER(closure->verifier, closure->hostkey_algorithm,
		      NULL, server_key);
  lsh_string_free(server_key);

  if (!v)
    {
      /* FIXME: Use a more appropriate error code? */
      disconnect_kex_failed(connection, "Bad server host key\r\n");
      lsh_string_free(signature);
      return;
    }

#if DATAFELLOWS_WORKAROUNDS
  if (closure->hostkey_algorithm == ATOM_SSH_DSS && 
      (connection->peer_flags & PEER_SSH_DSS_KLUDGE))
    {
      algorithm = ATOM_SSH_DSS_KLUDGE_LOCAL;
    }
  else
#endif
    algorithm = closure->hostkey_algorithm;
  
  res = VERIFY(v, algorithm,
	       closure->dh.exchange_hash->length, closure->dh.exchange_hash->data,
	       signature->length, signature->data);
  lsh_string_free(signature);

  if (!res)
    {
      werror("Invalid server signature. Disconnecting.\n");
      /* FIXME: Use a more appropriate error code? */
      disconnect_kex_failed(connection, "Invalid server signature\r\n");
      return;
    }
  
  /* Key exchange successful! */

  connection->dispatch[SSH_MSG_KEXDH_REPLY] = &connection_fail_handler;

  keyexchange_finish(connection, closure->algorithms,
		     closure->dh.method->H,
		     closure->dh.exchange_hash,
		     closure->dh.K);

  /* For gc */
  closure->dh.K = NULL;
  closure->dh.exchange_hash = NULL;  
}

static void
do_init_client_dh(struct keyexchange_algorithm *c,
		  struct ssh_connection *connection,
		  int hostkey_algorithm_atom,
		  struct lsh_object *extra,
		  struct object_list *algorithms)
{
  CAST(dh_client_exchange, closure, c);
  CAST_SUBTYPE(lookup_verifier, verifier, extra);
  
  NEW(dh_client, dh);

  assert(verifier);
  
  /* Initialize */
  dh->super.handler = do_handle_dh_reply;
  init_dh_instance(closure->dh, &dh->dh, connection);

  dh->verifier = verifier;
  dh->hostkey_algorithm = hostkey_algorithm_atom;

  dh->algorithms = algorithms;
  
  /* Send client's message */
  C_WRITE(connection, dh_make_client_msg(&dh->dh));

  /* Install handler */
  connection->dispatch[SSH_MSG_KEXDH_REPLY] = &dh->super;
}


struct keyexchange_algorithm *
make_dh_client(struct dh_method *dh)
{
  NEW(dh_client_exchange, self);

  self->super.init = do_init_client_dh;
  self->dh = dh;
  
  return &self->super;
}


#if WITH_SRP
/* SRP key exchange */

/* GABA:
   (class
     (name srp_client_instance)
     (vars
       (dh struct dh_instance)
       (tty object interact)
       (name string)
       (m2 string)
       (algorithms object object_list)))
*/

/* GABA:
   (class
     (name srp_client_handler)
     (super packet_handler)
     (vars
       (srp object srp_client_instance)))
*/

static void
do_srp_client_proof_handler(struct packet_handler *s,
			    struct ssh_connection *connection,
			    struct lsh_string *packet)
{
  CAST(srp_client_handler, self, s);

  int res = srp_process_server_proof(self->srp->m2,
				     packet);
  lsh_string_free(packet);

  connection->dispatch[SSH_MSG_KEXSRP_PROOF] = &connection_fail_handler;
  
  if (res)
    {
      connection->flags |= CONNECTION_SRP;
      
      keyexchange_finish(connection, self->srp->algorithms,
			 self->srp->dh.method->H,
			 self->srp->dh.exchange_hash,
			 self->srp->dh.K);

      /* For gc */
      self->srp->dh.K = NULL;
      self->srp->dh.exchange_hash = NULL;
    }
  else
    PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_KEXSRP_PROOF message");

  /* Try to purge information about the key exchange */
  KILL(self->srp);
  KILL(self);
}

static struct packet_handler *
make_srp_client_proof_handler(struct srp_client_instance *srp)
{
  NEW(srp_client_handler, self);
  self->super.handler = do_srp_client_proof_handler;
  self->srp = srp;

  return &self->super;
}

#define MAX_PASSWD 100

static void
do_handle_srp_reply(struct packet_handler *s,
		    struct ssh_connection *connection,
		    struct lsh_string *packet)
{
  CAST(srp_client_handler, self, s);
  struct lsh_string *salt = srp_process_reply_msg(&self->srp->dh, packet);
  struct lsh_string *passwd;
  struct lsh_string *response;
  
  mpz_t x;
  
  connection->dispatch[SSH_MSG_KEXSRP_REPLY] = &connection_fail_handler;
    
  if (!salt)
    {
      PROTOCOL_ERROR(connection->e, "Invalid SSH_MSG_KEXSRP_REPLY message");
      return;
    }

  /* Ask for SRP password */
  passwd = INTERACT_READ_PASSWORD(self->srp->tty, MAX_PASSWD,
				  ssh_format("SRP password for %lS: ",
					     self->srp->name), 1);
  if (!passwd)
    {
      lsh_string_free(salt);
      disconnect_kex_failed(connection, "Bye");
    }
  
  mpz_init(x);
  srp_hash_password(x, self->srp->dh.method->H,
		    salt, self->srp->name,
		    passwd);

  lsh_string_free(salt);
  lsh_string_free(passwd);

  response = srp_make_client_proof(&self->srp->dh, &self->srp->m2, x);
  mpz_clear(x);

  if (!response)
    PROTOCOL_ERROR(connection->e,
		   "SRP failure: Invalid public value from server.");
  
  C_WRITE(connection, response);
  
  connection->dispatch[SSH_MSG_KEXSRP_PROOF]
    = make_srp_client_proof_handler(self->srp);
}

static struct packet_handler *
make_srp_reply_handler(struct srp_client_instance *srp)
{
  NEW(srp_client_handler, self);
  self->super.handler = do_handle_srp_reply;
  self->srp = srp;

  return &self->super;
}

/* GABA:
   (class
     (name srp_client_exchange)
     (super keyexchange_algorithm)
     (vars
       (dh object dh_method)
       (tty object interact)
       (name string)))
*/

static void
do_init_client_srp(struct keyexchange_algorithm *s,
		   struct ssh_connection *connection,
		   int hostkey_algorithm_atom UNUSED,
		   struct lsh_object *extra UNUSED,
		   struct object_list *algorithms)
{
  CAST(srp_client_exchange, self, s);
  
  NEW(srp_client_instance, srp);

  /* Initialize */
  init_dh_instance(self->dh, &srp->dh, connection);

  srp->tty = self->tty;
  srp->algorithms = algorithms;
  srp->name = self->name;
  srp->m2 = NULL;
  
  /* Send client's message */
  C_WRITE(connection, srp_make_init_msg(&srp->dh, self->name));

  /* Install handler */
  connection->dispatch[SSH_MSG_KEXSRP_REPLY] = make_srp_reply_handler(srp);
}

struct keyexchange_algorithm *
make_srp_client(struct dh_method *dh, struct interact *tty,
		struct lsh_string *name)
{
  NEW(srp_client_exchange, self);

  assert(dh->G->add);
  assert(dh->G->subtract);

  self->super.init = do_init_client_srp;
  self->dh = dh;
  self->tty = tty;
  self->name = name;
  
  return &self->super;
}

#endif /* WITH_SRP */

/* lsh-krb-checkpw.c
 *
 * $Id$
 *
 * This is a helper program to verify a name and password against the
 * local kerberos infrastructure. It is loosely based on Heimdal's
 * implementation of "su". */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "lsh_types.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <krb5.h>

static void usage(void) NORETURN;
static void die(char *msg) NORETURN;

/* NOTE: It is essential that this program never ever exits successfully unless
 * the the user was accepted by kerberos. */
static void
usage(void)
{
  printf("Usage: lsh-krb-checkpw user < password\n");
  exit(EXIT_FAILURE);
}

static void
die(char *msg) 
{
  fprintf(stderr, "lsh-krb-checkpw: %s\n", msg);
  exit(EXIT_FAILURE);
}

#define PW_MAX_SIZE 40

static char *
read_pw(FILE *f)
{
  /* An extra byte for the terminating NUL. */
  char *buf = malloc(PW_MAX_SIZE+1);
  size_t length;
  
  if (!buf)
    return NULL;

  length = fread(buf, 1, PW_MAX_SIZE, f);

  if (!length || !feof(f))
    {
      free(buf);
      return NULL;
    }

  assert(length <= PW_MAX_SIZE);
  buf[length] = '\0';

  return buf;
}

int
main(int argc, char **argv)
{
  krb5_context context;
  krb5_ccache ccache;
  krb5_principal p;
  char *name;
  char *pw;
  
  if (argc != 2)
    usage();

  name = argv[1];
  pw = read_pw(stdin);

  if (!pw)
    die("No proper password provided.");

  if (!strcmp(name, "root"))
    /* In this case, heimdal's su.c creates a principal for the
     * current uid, but I don't quite understand why. */
    die("Won't log in root.");
  
  if (krb5_init_context (&context))
    die("krb5_init_context failed.");

  if (krb5_make_principal(context, &p, NULL, name, NULL))
    die("krb5_init_context failed.");
  
  if (!krb5_kuserok(context, p, name))
    die("krb5_kuserok doesn't know the user.");

  if (krb5_cc_gen_new(context, &krb5_mcc_ops, &ccache))
    die("krb5_cc_gen_new failed.");

  if (krb5_verify_user_lrealm(context, p, ccache, pw, TRUE, NULL))
    die("krb5_verify_user_lrealm failed.");

  /* Authentication successful. */

  /* TODO: Keep the credential cache in some way. Perhaps write it to
   * disk, and, write the file name used to stdout. */

  return EXIT_SUCCESS;
}





















    













#include "lsh.h"

#include "algorithms.h"
#include "crypto.h"
#include "digits.h"
#include "format.h"
#include "randomness.h"
#include "sexp.h"
#include "spki.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
  argp_parse(&werror_argp, argc, argv, 0, NULL, NULL);

/* Examples taken from RFC-2693 */

{
  struct spki_tag *tag = spki_sexp_to_tag(string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "(3:ftp13:ftp.clark.net3:cme(1:*3:set4:read5:write))") , 1), 17);
  struct sexp *access = string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "(3:ftp13:ftp.clark.net3:cme4:read)") , 1);
  fputs("Granting access 1 ... ", stderr);
  assert(tag);
  assert(access);
  
  if (SPKI_TAG_MATCH(tag, access))
    fputs("ok.\n", stderr);
  else
    { fputs("failed.\n", stderr);; exit(1); }
  KILL(tag);
  KILL(access);
}


{
  struct spki_tag *tag = spki_sexp_to_tag(string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "(3:ftp13:ftp.clark.net3:cme(1:*3:set4:read5:write))") , 1), 17);
  struct sexp *access = string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "(3:ftp13:ftp.clark.net3:cme6:delete)") , 1);
  fputs("Denying access 2 ... ", stderr);
  assert(tag);
  assert(access);
  
  if (!SPKI_TAG_MATCH(tag, access))
    fputs("ok.\n", stderr);
  else
    { fputs("failed.\n", stderr);; exit(1); }
  KILL(tag);
  KILL(access);
}


{
  struct spki_tag *tag = spki_sexp_to_tag(string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "(3:ftp13:ftp.clark.net3:cme(1:*3:set4:read5:write))") , 1), 17);
  struct sexp *access = string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "(3:ftp13:ftp.clark.net3:cme)") , 1);
  fputs("Denying access 3 ... ", stderr);
  assert(tag);
  assert(access);
  
  if (!SPKI_TAG_MATCH(tag, access))
    fputs("ok.\n", stderr);
  else
    { fputs("failed.\n", stderr);; exit(1); }
  KILL(tag);
  KILL(access);
}


  return 0;
}





















    













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
fputs("lsh_get_cstring 1 ... ", stderr);
{
	struct lsh_string *s = ssh_format("%lz", "foo") ;
	const char *p = lsh_get_cstring(s);
	if (p && !strcmp(p, "foo")) fputs("ok.\n", stderr); else { fputs("failed.\n", stderr);; exit(1); } 
}
fputs("lsh_get_cstring 2 ... ", stderr);
{
	struct lsh_string *s = simple_decode_hex("66006f") ;
	if (!lsh_get_cstring(s)) fputs("ok.\n", stderr); else { fputs("failed.\n", stderr);; exit(1); }
}
fputs("lsh_get_cstring 3 ... ", stderr);
{
	struct lsh_string *s = simple_decode_hex("6600") ;
	if (!lsh_get_cstring(s)) fputs("ok.\n", stderr); else { fputs("failed.\n", stderr);; exit(1); }
}
fputs("lsh_get_cstring 3 ... ", stderr);
{
	struct lsh_string *s = simple_decode_hex("") ;
	const char *p = lsh_get_cstring(s);
	if (p && !*p) fputs("ok.\n", stderr); else { fputs("failed.\n", stderr);; exit(1); }
}

  return 0;
}

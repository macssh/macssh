#line 9 "macros.m4"

#line 26


#line 1 "arcfour-test.m4"
#include "arcfour.h"


#line 3
#include "testutils.h"
#line 3

#line 3
#include <string.h>
#line 3
#include <stdlib.h>
#line 3

#line 3
int main (int argc, char **argv)
#line 3
{
#line 3


struct arcfour_ctx ctx;
const uint8_t *clear = decode_hex_dup("01234567 89ABCDEF");
uint8_t cipher[8];
arcfour_set_key(&ctx, 16, decode_hex_dup("01234567 89ABCDEF 00000000 00000000"));
arcfour_crypt(&ctx, 8, cipher, clear);
if (!(!memcmp (cipher, decode_hex_dup("69723659 1B5242B1"), 8)))
  abort();
#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

#line 9 "macros.m4"

#line 26


#line 1 "cast128-test.m4"
#include "cast128.h"


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


struct cast128_ctx ctx;

uint8_t msg[CAST128_BLOCK_SIZE];
uint8_t cipher[CAST128_BLOCK_SIZE];
uint8_t clear[CAST128_BLOCK_SIZE];

#line 12
/* Test vectors from B.1. Single Plaintext-Key-Ciphertext Sets, RFC
#line 12
 * 2144 */

/* 128 bit key */
decode_hex(msg, "01 23 45 67 89 AB CD EF");

cast128_set_key(&ctx, 16,  decode_hex_dup("01 23 45 67 12 34 56 78"
			     "23 45 67 89 34 56 78 9A"));
cast128_encrypt(&ctx, CAST128_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("23 8B 4F E5 84 7E 44 B2"), CAST128_BLOCK_SIZE)))
  abort();

cast128_decrypt(&ctx, CAST128_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, CAST128_BLOCK_SIZE)))
  abort();

/* 80 bit key */
decode_hex(msg, "01 23 45 67 89 AB CD EF");

cast128_set_key(&ctx, 10,  decode_hex_dup("01 23 45 67 12 34 56 78 23 45"));
cast128_encrypt(&ctx, CAST128_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("EB 6A 71 1A 2C 02 27 1B"), CAST128_BLOCK_SIZE)))
  abort();

cast128_decrypt(&ctx, CAST128_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, CAST128_BLOCK_SIZE)))
  abort();

/* 40 bit key */
decode_hex(msg, "01 23 45 67 89 AB CD EF");

cast128_set_key(&ctx, 5,  decode_hex_dup("01 23 45 67 12"));
cast128_encrypt(&ctx, CAST128_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("7A C8 16 D1 6E 9B 30 2E"), CAST128_BLOCK_SIZE)))
  abort();

cast128_decrypt(&ctx, CAST128_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, CAST128_BLOCK_SIZE)))
  abort();

#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

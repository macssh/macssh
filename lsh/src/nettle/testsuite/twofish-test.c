#line 9 "macros.m4"

#line 26


#line 1 "twofish-test.m4"
#include "twofish.h"


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


struct twofish_ctx ctx;

uint8_t msg[TWOFISH_BLOCK_SIZE];
uint8_t cipher[TWOFISH_BLOCK_SIZE];
uint8_t clear[TWOFISH_BLOCK_SIZE];

/* 128 bit key */
decode_hex(msg, "0000000000000000 0000000000000000");

twofish_set_key(&ctx, 16, decode_hex_dup("0000000000000000 0000000000000000"));
twofish_encrypt(&ctx, TWOFISH_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("9F589F5CF6122C32 B6BFEC2F2AE8C35A"), TWOFISH_BLOCK_SIZE)))
#line 19
  abort();

twofish_decrypt(&ctx, TWOFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, TWOFISH_BLOCK_SIZE)))
  abort();

/* 192 bit key */

twofish_set_key(&ctx, 24, decode_hex_dup("0123456789ABCDEF FEDCBA9876543210"
			    "0011223344556677"));
twofish_encrypt(&ctx, TWOFISH_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("CFD1D2E5A9BE9CDF 501F13B892BD2248"), TWOFISH_BLOCK_SIZE)))
#line 33
  abort();

twofish_decrypt(&ctx, TWOFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, TWOFISH_BLOCK_SIZE)))
  abort();

/* 256 bit key */
twofish_set_key(&ctx, 32, decode_hex_dup("0123456789ABCDEF FEDCBA9876543210"
			    "0011223344556677 8899AABBCCDDEEFF"));
twofish_encrypt(&ctx, TWOFISH_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("37527BE0052334B8 9F0CFCCAE87CFA20"), TWOFISH_BLOCK_SIZE)))
#line 46
  abort();

twofish_decrypt(&ctx, TWOFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, TWOFISH_BLOCK_SIZE)))
  abort();
#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

#line 9 "macros.m4"

#line 26


#line 1 "serpent-test.m4"
#include "serpent.h"


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


struct serpent_ctx ctx;

uint8_t msg[SERPENT_BLOCK_SIZE];
uint8_t cipher[SERPENT_BLOCK_SIZE];
uint8_t clear[SERPENT_BLOCK_SIZE];

#line 12
/* The first test for each key size from the ecb_vk.txt and ecb_vt.txt
#line 12
 * files in the serpent package. */

/* 128 bit key */

/* vk, 1 */
decode_hex(msg, "0000000000000000 0000000000000000");

serpent_set_key(&ctx, 16, decode_hex_dup("8000000000000000 0000000000000000"));
serpent_encrypt(&ctx, SERPENT_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("49AFBFAD9D5A3405 2CD8FFA5986BD2DD"), SERPENT_BLOCK_SIZE)))
#line 24
  abort();

serpent_decrypt(&ctx, SERPENT_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, SERPENT_BLOCK_SIZE)))
  abort();

/* vt, 1 */
decode_hex(msg, "8000000000000000 0000000000000000");

serpent_set_key(&ctx, 16, decode_hex_dup("0000000000000000 0000000000000000"));
serpent_encrypt(&ctx, SERPENT_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("10B5FFB720B8CB90 02A1142B0BA2E94A"), SERPENT_BLOCK_SIZE)))
#line 38
  abort();

serpent_decrypt(&ctx, SERPENT_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, SERPENT_BLOCK_SIZE)))
  abort();

/* 192 bit key */

/* vk, 1 */
decode_hex(msg, "0000000000000000 0000000000000000");

serpent_set_key(&ctx, 24, decode_hex_dup("8000000000000000 0000000000000000"
			    "0000000000000000"));
serpent_encrypt(&ctx, SERPENT_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("E78E5402C7195568 AC3678F7A3F60C66"), SERPENT_BLOCK_SIZE)))
#line 55
  abort();

serpent_decrypt(&ctx, SERPENT_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, SERPENT_BLOCK_SIZE)))
  abort();

/* vt, 1 */
decode_hex(msg, "8000000000000000 0000000000000000");

serpent_set_key(&ctx, 24, decode_hex_dup("0000000000000000 0000000000000000"
			    "0000000000000000"));
serpent_encrypt(&ctx, SERPENT_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("B10B271BA25257E1 294F2B51F076D0D9"), SERPENT_BLOCK_SIZE)))
#line 70
  abort();

serpent_decrypt(&ctx, SERPENT_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, SERPENT_BLOCK_SIZE)))
  abort();

/* 256 bit key */

/* vk, 1 */
decode_hex(msg, "0000000000000000 0000000000000000");

serpent_set_key(&ctx, 32, decode_hex_dup("8000000000000000 0000000000000000"
			    "0000000000000000 0000000000000000"));
serpent_encrypt(&ctx, SERPENT_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("ABED96E766BF28CB C0EBD21A82EF0819"), SERPENT_BLOCK_SIZE)))
#line 87
  abort();

serpent_decrypt(&ctx, SERPENT_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, SERPENT_BLOCK_SIZE)))
  abort();

/* vt, 1 */
decode_hex(msg, "8000000000000000 0000000000000000");

serpent_set_key(&ctx, 32, decode_hex_dup("0000000000000000 0000000000000000"
			    "0000000000000000 0000000000000000"));
serpent_encrypt(&ctx, SERPENT_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("DA5A7992B1B4AE6F 8C004BC8A7DE5520"), SERPENT_BLOCK_SIZE)))
#line 102
  abort();

serpent_decrypt(&ctx, SERPENT_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, SERPENT_BLOCK_SIZE)))
  abort();

#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

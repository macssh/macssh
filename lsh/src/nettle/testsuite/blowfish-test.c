#line 9 "macros.m4"

#line 26


#line 1 "blowfish-test.m4"
#include "blowfish.h"


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


struct blowfish_ctx ctx;

uint8_t msg[BLOWFISH_BLOCK_SIZE];
uint8_t cipher[BLOWFISH_BLOCK_SIZE];
uint8_t clear[BLOWFISH_BLOCK_SIZE];

/* 208 bit key. Test from GNUPG. */
blowfish_set_key(&ctx, 26, "abcdefghijklmnopqrstuvwxyz");
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, "BLOWFISH");
if (!(!memcmp (cipher, decode_hex_dup("32 4E D0 FE F4 13 A2 03"), BLOWFISH_BLOCK_SIZE)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp ("BLOWFISH", clear, BLOWFISH_BLOCK_SIZE)))
  abort();

/* FIXME: All values below are bogus. */
#if 0

/* 128 bit keys */
decode_hex(msg, "506812A45F08C889 B97F5980038B8359");

blowfish_set_key(&ctx, 16,  decode_hex_dup("0001020305060708 0A0B0C0D0F101112"));
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("D8F532538289EF7D 06B506A4FD5BE9C9"), 16)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, 16)))
  abort();

decode_hex(msg, "5C6D71CA30DE8B8B 00549984D2EC7D4B");

blowfish_set_key(&ctx, 16,  decode_hex_dup("14151617191A1B1C 1E1F202123242526"));
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("59AB30F4D4EE6E4F F9907EF65B1FB68C"), 16)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, 16)))
  abort();

decode_hex(msg, "53F3F4C64F8616E4 E7C56199F48F21F6");

blowfish_set_key(&ctx, 16,  decode_hex_dup("28292A2B2D2E2F30 323334353738393A"));
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("BF1ED2FCB2AF3FD4 1443B56D85025CB1"), 16)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, 16)))
  abort();

decode_hex(msg, "F5F4F7F684878689 A6A7A0A1D2CDCCCF");

blowfish_set_key(&ctx, 16,  decode_hex_dup("A0A1A2A3A5A6A7A8 AAABACADAFB0B1B2"));
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("CE52AF650D088CA5 59425223F4D32694"), 16)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, 16)))
  abort();

/* 192 bit keys */
decode_hex(msg, "2D33EEF2C0430A8A 9EBF45E809C40BB6");

blowfish_set_key(&ctx, 24,  decode_hex_dup("0001020305060708 0A0B0C0D0F101112"
			 "14151617191A1B1C"));
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("DFF4945E0336DF4C 1C56BC700EFF837F"), 16)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, 16)))
  abort();

/* 256 bit keys */
decode_hex(msg, "834EADFCCAC7E1B30664B1ABA44815AB");

blowfish_set_key(&ctx, 32,  decode_hex_dup("0001020305060708 0A0B0C0D0F101112"
			 "14151617191A1B1C 1E1F202123242526"));
blowfish_encrypt(&ctx, BLOWFISH_BLOCK_SIZE, cipher, msg);
if (!(!memcmp (cipher, decode_hex_dup("1946DABF6A03A2A2 C3D0B05080AED6FC"), 16)))
  abort();

blowfish_decrypt(&ctx, BLOWFISH_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, 16)))
  abort();
#endif
#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

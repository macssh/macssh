#line 9 "macros.m4"

#line 26


#line 1 "des-test.m4"
#include "des.h"


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


struct des_ctx ctx;

uint8_t msg[DES_BLOCK_SIZE];
uint8_t cipher[DES_BLOCK_SIZE];
uint8_t clear[DES_BLOCK_SIZE];

decode_hex(msg, "00 00 00 00 00 00 00 00");

if (!des_set_key(&ctx, decode_hex_dup("01 01 01 01 01 01 01 80")))
  abort();

des_encrypt(&ctx, DES_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("9C C6 2D F4 3B 6E ED 74"), DES_BLOCK_SIZE)))
#line 20
  abort();

des_decrypt(&ctx, DES_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, DES_BLOCK_SIZE)))
  abort();

decode_hex(msg, "00 00 00 00 00 00 00 40");

if (!des_set_key(&ctx, decode_hex_dup("80 01 01 01 01 01 01 01")))
  abort();

des_encrypt(&ctx, DES_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("A3 80 E0 2A 6B E5 46 96"), DES_BLOCK_SIZE)))
#line 35
  abort();

des_decrypt(&ctx, DES_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, DES_BLOCK_SIZE)))
  abort();

decode_hex(msg, "00 00 00 00 00 00 00 00");

if (!des_set_key(&ctx, decode_hex_dup("08 19 2A 3B 4C 5D 6E 7F")))
  abort();

des_encrypt(&ctx, DES_BLOCK_SIZE, cipher, msg);

if (!(!memcmp (cipher, decode_hex_dup("25 DD AC 3E 96 17 64 67"), DES_BLOCK_SIZE)))
#line 50
  abort();

des_decrypt(&ctx, DES_BLOCK_SIZE, clear, cipher);
if (!(!memcmp (msg, clear, DES_BLOCK_SIZE)))
  abort();

if (!des_set_key(&ctx, decode_hex_dup("01 23 45 67 89 AB CD EF")))
  abort();

des_encrypt(&ctx, DES_BLOCK_SIZE, cipher, "Now is t");

if (!(!memcmp (cipher, decode_hex_dup("3F A4 0E 8A 98 4D 48 15"), DES_BLOCK_SIZE)))
#line 63
  abort();

des_decrypt(&ctx, DES_BLOCK_SIZE, clear, cipher);
if (!(!memcmp ("Now is t", clear, DES_BLOCK_SIZE)))
  abort();

/* Parity check */
if (des_set_key(&ctx, decode_hex_dup("01 01 01 01 01 01 01 00"))
    || (ctx.status != DES_BAD_PARITY))
  abort();

/* Weak key check */
if (des_set_key(&ctx, decode_hex_dup("01 01 01 01 01 01 01 01"))
    || (ctx.status != DES_WEAK_KEY))
  abort();

if (des_set_key(&ctx, decode_hex_dup("01 FE 01 FE 01 FE 01 FE"))
    || (ctx.status != DES_WEAK_KEY))
  abort();

if (des_set_key(&ctx, decode_hex_dup("FE E0 FE E0 FE F1 FE F1"))
    || (ctx.status != DES_WEAK_KEY))
  abort();

#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

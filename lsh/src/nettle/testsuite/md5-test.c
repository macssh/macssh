#line 9 "macros.m4"

#line 26


#line 1 "md5-test.m4"
#include "md5.h"


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


struct md5_ctx ctx;
uint8_t digest[MD5_DIGEST_SIZE];

md5_init(&ctx);
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("D41D8CD98F00B204 E9800998ECF8427E"), MD5_DIGEST_SIZE)))
  abort();

memset(digest, 0, MD5_DIGEST_SIZE);
md5_digest(&ctx, MD5_DIGEST_SIZE - 1, digest);

if (!(!memcmp (digest, decode_hex_dup("D41D8CD98F00B204 E9800998ECF84200"), MD5_DIGEST_SIZE)))
  abort();

md5_init(&ctx);
md5_update(&ctx, 1, "a");
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("0CC175B9C0F1B6A8 31C399E269772661"), MD5_DIGEST_SIZE)))
  abort();

md5_init(&ctx);
md5_update(&ctx, 3, "abc");
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("900150983cd24fb0 D6963F7D28E17F72"), MD5_DIGEST_SIZE)))
  abort();

md5_init(&ctx);
md5_update(&ctx, 14, "message digest");
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("F96B697D7CB7938D 525A2F31AAF161D0"), MD5_DIGEST_SIZE)))
  abort();

md5_init(&ctx);
md5_update(&ctx, 26, "abcdefghijklmnopqrstuvwxyz");
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("C3FCD3D76192E400 7DFB496CCA67E13B"), MD5_DIGEST_SIZE)))
  abort();

md5_init(&ctx);
md5_update(&ctx, 62, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("D174AB98D277D9F5 A5611C2C9F419D9F"), MD5_DIGEST_SIZE)))
  abort();

md5_init(&ctx);
md5_update(&ctx, 80, "1234567890123456789012345678901234567890"
	            "1234567890123456789012345678901234567890");
md5_final(&ctx);
md5_digest(&ctx, MD5_DIGEST_SIZE, digest);

if (!(!memcmp (digest, decode_hex_dup("57EDF4A22BE3C955 AC49DA2E2107B67A"), MD5_DIGEST_SIZE)))
  abort();

#line 5 "macros.m4"
  /* Avoid warnings for argc and argv unused */
  (void) argc; (void) argv;
  return 0;
}

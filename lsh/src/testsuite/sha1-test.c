



















    













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

  {
    struct lsh_string *a, *b;
    fputs("SHA1 ... ", stderr);
    a = hash_string(&sha1_algorithm, ssh_format("%lz", "abc") , 1);
    b = simple_decode_hex("A9993E364706816ABA3E25717850C26C9CD0D89D") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }


/* Test vectors for sha1 from RFC-2202, and some more */


  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-0 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Hi There") , 1);
    b = simple_decode_hex("675B0B3A1B4DDF4E124872DA6C2F632BFED957E9") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-1 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Hi There") , 1);
    b = simple_decode_hex("B617318655057264E28BC0B6FB378C8EF146BE00") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = ssh_format("%lz", "Jefe") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-2 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "what do ya want for nothing?") , 1);
    b = simple_decode_hex("EFFCDF6AE5EB2FA2D27416D5F184DF9C259A7C79") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-3a ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       simple_decode_hex("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD") , 1);
    b = simple_decode_hex("D730594D167E35D5956FD8003D0DB3D3F46DC7BB") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-3 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       simple_decode_hex("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD") , 1);
    b = simple_decode_hex("125D7342B9AC11CD91A39AF48AA17B4F63F175D3") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("0102030405060708090A0B0C0D0E0F10111213141516171819") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-4 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       simple_decode_hex("CDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCD") , 1);
    b = simple_decode_hex("4C9007F4026250C6BC8414F9BF50C86C2D7235DA") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-5 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Test With Truncation") , 1);
    b = simple_decode_hex("4C1A03424B55E07FE7F27BE1D58BB9324A9A5A04") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-6 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Test Using Larger Than Block-Size Key - Hash Key First") , 1);
    b = simple_decode_hex("AA4AE5E15272D00E95705637CE8A3B55ED402112") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&sha1_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-SHA1-7 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Test Using Larger Than Block-Size Key an"
             "d Larger Than One Block-Size Data") , 1);
    b = simple_decode_hex("E8E99D0F45237D786D6BBAA7965C7808BBFF1A91") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  return 0;
}





















    













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
/* Test vectors from RFC 1321 */

  {
    struct lsh_string *a, *b;
    fputs("MD5-1 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "") , 1);
    b = simple_decode_hex("D41D8CD98F00B204E9800998ECF8427E") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }



  {
    struct lsh_string *a, *b;
    fputs("MD5-2 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "a") , 1);
    b = simple_decode_hex("0CC175B9C0F1B6A831C399E269772661") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }



  {
    struct lsh_string *a, *b;
    fputs("MD5-3 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "abc") , 1);
    b = simple_decode_hex("900150983cd24fb0D6963F7D28E17F72") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }



  {
    struct lsh_string *a, *b;
    fputs("MD5-4 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "message digest") , 1);
    b = simple_decode_hex("F96B697D7CB7938D525A2F31AAF161D0") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }



  {
    struct lsh_string *a, *b;
    fputs("MD5-5 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "abcdefghijklmnopqrstuvwxyz") , 1);
    b = simple_decode_hex("C3FCD3D76192E4007DFB496CCA67E13B") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }



  {
    struct lsh_string *a, *b;
    fputs("MD5-6 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") , 1);
    b = simple_decode_hex("D174AB98D277D9F5A5611C2C9F419D9F") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }



  {
    struct lsh_string *a, *b;
    fputs("MD5-7 ... ", stderr);
    a = hash_string(&md5_algorithm, ssh_format("%lz", "1234567890123456789012345678901234567890"
             "1234567890123456789012345678901234567890") , 1);
    b = simple_decode_hex("57EDF4A22BE3C955AC49DA2E2107B67A") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }


/* Test vectors for md5 from RFC-2202 */

  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = simple_decode_hex("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-1 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Hi There") , 1);
    b = simple_decode_hex("9294727A3638BB1C13F48EF8158BFC9D") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = ssh_format("%lz", "Jefe") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-2 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "what do ya want for nothing?") , 1);
    b = simple_decode_hex("750C783E6AB0B503EAA86E310A5DB738") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-3 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       simple_decode_hex("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD") , 1);
    b = simple_decode_hex("56BE34521D144C88DBB8C733F0E8B3F6") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = simple_decode_hex("0102030405060708090A0B0C0D0E0F10111213141516171819") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-4 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       simple_decode_hex("CDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCD") , 1);
    b = simple_decode_hex("697EAF0ACA3A3AEA3A75164746FFAA79") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = simple_decode_hex("0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-5 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Test With Truncation") , 1);
    b = simple_decode_hex("56461EF2342EDC00F9BAB995690EFD4C") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-6 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Test Using Larger Than Block-Size Key - Hash Key First") , 1);
    b = simple_decode_hex("6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd") ;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }
;
  }



  {
    struct mac_algorithm *hmac = make_hmac_algorithm(&md5_algorithm);
    struct lsh_string *key = simple_decode_hex("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ;

    
  {
    struct lsh_string *a, *b;
    fputs("HMAC-MD5-7 ... ", stderr);
    a = mac_string(hmac, key, 1,
                                       ssh_format("%lz", "Test Using Larger Than Block-Size Key an"
             "d Larger Than One Block-Size Data") , 1);
    b = simple_decode_hex("6f630fad67cda0ee1fb1f562db3aa53e") ;
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

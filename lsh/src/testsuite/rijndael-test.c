



















    













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
    struct crypto_algorithm *algorithm = &rijndael128_algorithm;
    struct lsh_string *key = simple_decode_hex("00010203050607080A0B0C0D0F101112") ;
    struct lsh_string *plain = simple_decode_hex("506812A45F08C889B97F5980038B8359") ;
    struct lsh_string *cipher = simple_decode_hex("D8F532538289EF7D06B506A4FD5BE9C9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-128 1 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-128 1 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael128_algorithm;
    struct lsh_string *key = simple_decode_hex("14151617191A1B1C1E1F202123242526") ;
    struct lsh_string *plain = simple_decode_hex("5C6D71CA30DE8B8B00549984D2EC7D4B") ;
    struct lsh_string *cipher = simple_decode_hex("59AB30F4D4EE6E4FF9907EF65B1FB68C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-128 2 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-128 2 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael128_algorithm;
    struct lsh_string *key = simple_decode_hex("28292A2B2D2E2F30323334353738393A") ;
    struct lsh_string *plain = simple_decode_hex("53F3F4C64F8616E4E7C56199F48F21F6") ;
    struct lsh_string *cipher = simple_decode_hex("BF1ED2FCB2AF3FD41443B56D85025CB1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-128 3 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-128 3 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }


/* Skipped lots of tests */


  {
    struct crypto_algorithm *algorithm = &rijndael128_algorithm;
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2") ;
    struct lsh_string *plain = simple_decode_hex("F5F4F7F684878689A6A7A0A1D2CDCCCF") ;
    struct lsh_string *cipher = simple_decode_hex("CE52AF650D088CA559425223F4D32694") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-128 128 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-128 128 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael192_algorithm;
    struct lsh_string *key = simple_decode_hex("00010203050607080A0B0C0D0F10111214151617191A1B1C") ;
    struct lsh_string *plain = simple_decode_hex("2D33EEF2C0430A8A9EBF45E809C40BB6") ;
    struct lsh_string *cipher = simple_decode_hex("DFF4945E0336DF4C1C56BC700EFF837F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-192 1 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-192 1 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael192_algorithm;
    struct lsh_string *key = simple_decode_hex("1E1F20212324252628292A2B2D2E2F30323334353738393A") ;
    struct lsh_string *plain = simple_decode_hex("6AA375D1FA155A61FB72353E0A5A8756") ;
    struct lsh_string *cipher = simple_decode_hex("B6FDDEF4752765E347D5D2DC196D1252") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-192 2 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-192 2 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael192_algorithm;
    struct lsh_string *key = simple_decode_hex("3C3D3E3F41424344464748494B4C4D4E5051525355565758") ;
    struct lsh_string *plain = simple_decode_hex("BC3736518B9490DCB8ED60EB26758ED4") ;
    struct lsh_string *cipher = simple_decode_hex("D23684E3D963B3AFCF1A114ACA90CBD6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-192 3 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-192 3 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }


/* Skipped lots of tests */


  {
    struct crypto_algorithm *algorithm = &rijndael192_algorithm;
    struct lsh_string *key = simple_decode_hex("868788898B8C8D8E90919293959697989A9B9C9D9FA0A1A2") ;
    struct lsh_string *plain = simple_decode_hex("D3D2DDDCAAADACAF9C9D9E9FE8EBEAE5") ;
    struct lsh_string *cipher = simple_decode_hex("9ADB3D4CCA559BB98C3E2ED73DBF1154") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-192 128 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-192 128 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael256_algorithm;
    struct lsh_string *key = simple_decode_hex("00010203050607080A0B0C0D0F10111214151617191A1B1C1E1F202123242526") ;
    struct lsh_string *plain = simple_decode_hex("834EADFCCAC7E1B30664B1ABA44815AB") ;
    struct lsh_string *cipher = simple_decode_hex("1946DABF6A03A2A2C3D0B05080AED6FC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-256 1 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-256 1 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael256_algorithm;
    struct lsh_string *key = simple_decode_hex("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E") ;
    struct lsh_string *plain = simple_decode_hex("D9DC4DBA3021B05D67C0518F72B62BF1") ;
    struct lsh_string *cipher = simple_decode_hex("5ED301D747D3CC715445EBDEC62F2FB4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-256 2 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-256 2 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  {
    struct crypto_algorithm *algorithm = &rijndael256_algorithm;
    struct lsh_string *key = simple_decode_hex("50515253555657585A5B5C5D5F60616264656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("A291D86301A4A739F7392173AA3C604C") ;
    struct lsh_string *cipher = simple_decode_hex("6585C8F43D13A6BEAB6419FC5935B9D0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-256 3 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-256 3 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }


/* Skipped lots of tests */


  {
    struct crypto_algorithm *algorithm = &rijndael256_algorithm;
    struct lsh_string *key = simple_decode_hex("50515253555657585A5B5C5D5F60616264656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("050407067477767956575051221D1C1F") ;
    struct lsh_string *cipher = simple_decode_hex("7444527095838FE080FC2BCDD30847EB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael-256 128 ... ", stderr);
    a = crypt_string(c, plain, 0);
    b = lsh_string_dup(cipher);
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    c = MAKE_DECRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Decrypting with rijndael-256 128 ... ", stderr);
    a = crypt_string(c, cipher, 0);
    b = plain;
    if (!lsh_string_eq(a, b))
      { fputs("failed.\n", stderr);; exit(1); }
    fputs("ok.\n", stderr);
    lsh_string_free(a);
    lsh_string_free(b);
  }

    KILL(c);
    
    lsh_string_free(key);
    lsh_string_free(cipher);
  }



  return 0;
}

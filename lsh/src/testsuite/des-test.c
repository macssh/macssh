



















    













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
/* From Applied Cryptography, 2:nd edition */


  {
    struct crypto_algorithm *algorithm = &crypto_des_algorithm;
    struct lsh_string *key = simple_decode_hex("0123456789ABCDEF") ;
    struct lsh_string *plain = simple_decode_hex("0123456789ABCDE7") ;
    struct lsh_string *cipher = simple_decode_hex("C95744256A5ED31D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with DES AC ... ", stderr);
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
    fputs("Decrypting with DES AC ... ", stderr);
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


/* From Dana How's DEScore */

  {
    struct crypto_algorithm *algorithm = &crypto_des_algorithm;
    struct lsh_string *key = simple_decode_hex("0101010101010180") ;
    struct lsh_string *plain = simple_decode_hex("0000000000000000") ;
    struct lsh_string *cipher = simple_decode_hex("9CC62DF43B6EED74") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with DES 1 ... ", stderr);
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
    fputs("Decrypting with DES 1 ... ", stderr);
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
    struct crypto_algorithm *algorithm = &crypto_des_algorithm;
    struct lsh_string *key = simple_decode_hex("8001010101010101") ;
    struct lsh_string *plain = simple_decode_hex("0000000000000040") ;
    struct lsh_string *cipher = simple_decode_hex("A380E02A6BE54696") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with DES 2 ... ", stderr);
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
    fputs("Decrypting with DES 2 ... ", stderr);
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
    struct crypto_algorithm *algorithm = &crypto_des_algorithm;
    struct lsh_string *key = simple_decode_hex("08192A3B4C5D6E7F") ;
    struct lsh_string *plain = simple_decode_hex("0000000000000000") ;
    struct lsh_string *cipher = simple_decode_hex("25DDAC3E96176467") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with DES 3 ... ", stderr);
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
    fputs("Decrypting with DES 3 ... ", stderr);
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
    struct crypto_algorithm *algorithm = &crypto_des_algorithm;
    struct lsh_string *key = simple_decode_hex("0123456789ABCDEF") ;
    struct lsh_string *plain = ssh_format("%lz", "Now is t") ;
    struct lsh_string *cipher = simple_decode_hex("3FA40E8A984D4815") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with DES 4 ... ", stderr);
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
    fputs("Decrypting with DES 4 ... ", stderr);
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





















    













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
    struct crypto_algorithm *algorithm = &crypto_arcfour_algorithm;
    struct lsh_string *key = simple_decode_hex("0123456789ABCDEF0000000000000000") ;
    struct lsh_string *plain = simple_decode_hex("0123456789ABCDEF") ;
    struct lsh_string *cipher = simple_decode_hex("697236591B5242B1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with Arcfour ... ", stderr);
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
    fputs("Decrypting with Arcfour ... ", stderr);
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

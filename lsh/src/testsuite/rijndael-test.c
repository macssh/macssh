



















    













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
    fputs("Encrypting with rijndael 1 ... ", stderr);
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
    fputs("Decrypting with rijndael 1 ... ", stderr);
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
    fputs("Encrypting with rijndael 2 ... ", stderr);
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
    fputs("Decrypting with rijndael 2 ... ", stderr);
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
    fputs("Encrypting with rijndael 3 ... ", stderr);
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
    fputs("Decrypting with rijndael 3 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("3C3D3E3F41424344464748494B4C4D4E") ;
    struct lsh_string *plain = simple_decode_hex("A1EB65A3487165FB0F1C27FF9959F703") ;
    struct lsh_string *cipher = simple_decode_hex("7316632D5C32233EDCB0780560EAE8B2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 4 ... ", stderr);
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
    fputs("Decrypting with rijndael 4 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("50515253555657585A5B5C5D5F606162") ;
    struct lsh_string *plain = simple_decode_hex("3553ECF0B1739558B08E350A98A39BFA") ;
    struct lsh_string *cipher = simple_decode_hex("408C073E3E2538072B72625E68B8364B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 5 ... ", stderr);
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
    fputs("Decrypting with rijndael 5 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("64656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("67429969490B9711AE2B01DC497AFDE8") ;
    struct lsh_string *cipher = simple_decode_hex("E1F94DFA776597BEACA262F2F6366FEA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 6 ... ", stderr);
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
    fputs("Decrypting with rijndael 6 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A") ;
    struct lsh_string *plain = simple_decode_hex("93385C1F2AEC8BED192F5A8E161DD508") ;
    struct lsh_string *cipher = simple_decode_hex("F29E986C6A1C27D7B29FFD7EE92B75F1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 7 ... ", stderr);
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
    fputs("Decrypting with rijndael 7 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("B5BF946BE19BEB8DB3983B5F4C6E8DDB") ;
    struct lsh_string *cipher = simple_decode_hex("131C886A57F8C2E713ABA6955E2B55B5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 8 ... ", stderr);
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
    fputs("Decrypting with rijndael 8 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2") ;
    struct lsh_string *plain = simple_decode_hex("41321EE10E21BD907227C4450FF42324") ;
    struct lsh_string *cipher = simple_decode_hex("D2AB7662DF9B8C740210E5EEB61C199D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 9 ... ", stderr);
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
    fputs("Decrypting with rijndael 9 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("00A82F59C91C8486D12C0A80124F6089") ;
    struct lsh_string *cipher = simple_decode_hex("14C10554B2859C484CAB5869BBE7C470") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 10 ... ", stderr);
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
    fputs("Decrypting with rijndael 10 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DA") ;
    struct lsh_string *plain = simple_decode_hex("7CE0FD076754691B4BBD9FAF8A1372FE") ;
    struct lsh_string *cipher = simple_decode_hex("DB4D498F0A49CF55445D502C1F9AB3B5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 11 ... ", stderr);
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
    fputs("Decrypting with rijndael 11 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("DCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("23605A8243D07764541BC5AD355B3129") ;
    struct lsh_string *cipher = simple_decode_hex("6D96FEF7D66590A77A77BB2056667F7F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 12 ... ", stderr);
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
    fputs("Decrypting with rijndael 12 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE010002") ;
    struct lsh_string *plain = simple_decode_hex("12A8CFA23EA764FD876232B4E842BC44") ;
    struct lsh_string *cipher = simple_decode_hex("316FB68EDBA736C53E78477BF913725C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 13 ... ", stderr);
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
    fputs("Decrypting with rijndael 13 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("04050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("BCAF32415E8308B3723E5FDD853CCC80") ;
    struct lsh_string *cipher = simple_decode_hex("6936F2B93AF8397FD3A771FC011C8C37") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 14 ... ", stderr);
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
    fputs("Decrypting with rijndael 14 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("89AFAE685D801AD747ACE91FC49ADDE0") ;
    struct lsh_string *cipher = simple_decode_hex("F3F92F7A9C59179C1FCC2C2BA0B082CD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 15 ... ", stderr);
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
    fputs("Decrypting with rijndael 15 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F505152") ;
    struct lsh_string *plain = simple_decode_hex("F521D07B484357C4A69E76124A634216") ;
    struct lsh_string *cipher = simple_decode_hex("6A95EA659EE3889158E7A9152FF04EBC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 16 ... ", stderr);
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
    fputs("Decrypting with rijndael 16 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("54555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("3E23B3BC065BCC152407E23896D77783") ;
    struct lsh_string *cipher = simple_decode_hex("1959338344E945670678A5D432C90B93") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 17 ... ", stderr);
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
    fputs("Decrypting with rijndael 17 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A") ;
    struct lsh_string *plain = simple_decode_hex("79F0FBA002BE1744670E7E99290D8F52") ;
    struct lsh_string *cipher = simple_decode_hex("E49BDDD2369B83EE66E6C75A1161B394") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 18 ... ", stderr);
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
    fputs("Decrypting with rijndael 18 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("DA23FE9D5BD63E1D72E3DAFBE21A6C2A") ;
    struct lsh_string *cipher = simple_decode_hex("D3388F19057FF704B70784164A74867D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 19 ... ", stderr);
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
    fputs("Decrypting with rijndael 19 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("E3F5698BA90B6A022EFD7DB2C7E6C823") ;
    struct lsh_string *cipher = simple_decode_hex("23AA03E2D5E4CD24F3217E596480D1E1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 20 ... ", stderr);
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
    fputs("Decrypting with rijndael 20 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2") ;
    struct lsh_string *plain = simple_decode_hex("BDC2691D4F1B73D2700679C3BCBF9C6E") ;
    struct lsh_string *cipher = simple_decode_hex("C84113D68B666AB2A50A8BDB222E91B9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 21 ... ", stderr);
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
    fputs("Decrypting with rijndael 21 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A") ;
    struct lsh_string *plain = simple_decode_hex("BA74E02093217EE1BA1B42BD5624349A") ;
    struct lsh_string *cipher = simple_decode_hex("AC02403981CD4340B507963DB65CB7B6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 22 ... ", stderr);
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
    fputs("Decrypting with rijndael 22 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("B5C593B5851C57FBF8B3F57715E8F680") ;
    struct lsh_string *cipher = simple_decode_hex("8D1299236223359474011F6BF5088414") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 23 ... ", stderr);
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
    fputs("Decrypting with rijndael 23 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F909192") ;
    struct lsh_string *plain = simple_decode_hex("3DA9BD9CEC072381788F9387C3BBF4EE") ;
    struct lsh_string *cipher = simple_decode_hex("5A1D6AB8605505F7977E55B9A54D9B90") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 24 ... ", stderr);
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
    fputs("Decrypting with rijndael 24 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("94959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("4197F3051121702AB65D316B3C637374") ;
    struct lsh_string *cipher = simple_decode_hex("72E9C2D519CF555E4208805AABE3B258") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 25 ... ", stderr);
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
    fputs("Decrypting with rijndael 25 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BA") ;
    struct lsh_string *plain = simple_decode_hex("9F46C62EC4F6EE3F6E8C62554BC48AB7") ;
    struct lsh_string *cipher = simple_decode_hex("A8F3E81C4A23A39EF4D745DFFE026E80") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 26 ... ", stderr);
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
    fputs("Decrypting with rijndael 26 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("BCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCE") ;
    struct lsh_string *plain = simple_decode_hex("0220673FE9E699A4EBC8E0DBEB6979C8") ;
    struct lsh_string *cipher = simple_decode_hex("546F646449D31458F9EB4EF5483AEE6C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 27 ... ", stderr);
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
    fputs("Decrypting with rijndael 27 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2") ;
    struct lsh_string *plain = simple_decode_hex("B2B99171337DED9BC8C2C23FF6F18867") ;
    struct lsh_string *cipher = simple_decode_hex("4DBE4BC84AC797C0EE4EFB7F1A07401C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 28 ... ", stderr);
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
    fputs("Decrypting with rijndael 28 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("A7FACF4E301E984E5EFEEFD645B23505") ;
    struct lsh_string *cipher = simple_decode_hex("25E10BFB411BBD4D625AC8795C8CA3B3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 29 ... ", stderr);
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
    fputs("Decrypting with rijndael 29 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F8F9FAFBFDFEFE00020304050708090A") ;
    struct lsh_string *plain = simple_decode_hex("F7C762E4A9819160FD7ACFB6C4EEDCDD") ;
    struct lsh_string *cipher = simple_decode_hex("315637405054EC803614E43DEF177579") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 30 ... ", stderr);
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
    fputs("Decrypting with rijndael 30 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("0C0D0E0F11121314161718191B1C1D1E") ;
    struct lsh_string *plain = simple_decode_hex("9B64FC21EA08709F4915436FAA70F1BE") ;
    struct lsh_string *cipher = simple_decode_hex("60C5BC8A1410247295C6386C59E572A8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 31 ... ", stderr);
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
    fputs("Decrypting with rijndael 31 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("20212223252627282A2B2C2D2F303132") ;
    struct lsh_string *plain = simple_decode_hex("52AF2C3DE07EE6777F55A4ABFC100B3F") ;
    struct lsh_string *cipher = simple_decode_hex("01366FC8CA52DFE055D6A00A76471BA6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 32 ... ", stderr);
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
    fputs("Decrypting with rijndael 32 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("34353637393A3B3C3E3F404143444546") ;
    struct lsh_string *plain = simple_decode_hex("2FCA001224386C57AA3F968CBE2C816F") ;
    struct lsh_string *cipher = simple_decode_hex("ECC46595516EC612449C3F581E7D42FF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 33 ... ", stderr);
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
    fputs("Decrypting with rijndael 33 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("48494A4B4D4E4F50525354555758595A") ;
    struct lsh_string *plain = simple_decode_hex("4149C73658A4A9C564342755EE2C132F") ;
    struct lsh_string *cipher = simple_decode_hex("6B7FFE4C602A154B06EE9C7DAB5331C9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 34 ... ", stderr);
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
    fputs("Decrypting with rijndael 34 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("5C5D5E5F61626364666768696B6C6D6E") ;
    struct lsh_string *plain = simple_decode_hex("AF60005A00A1772F7C07A48A923C23D2") ;
    struct lsh_string *cipher = simple_decode_hex("7DA234C14039A240DD02DD0FBF84EB67") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 35 ... ", stderr);
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
    fputs("Decrypting with rijndael 35 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F808182") ;
    struct lsh_string *plain = simple_decode_hex("6FCCBC28363759914B6F0280AFAF20C6") ;
    struct lsh_string *cipher = simple_decode_hex("C7DC217D9E3604FFE7E91F080ECD5A3A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 36 ... ", stderr);
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
    fputs("Decrypting with rijndael 36 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("84858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("7D82A43DDF4FEFA2FC5947499884D386") ;
    struct lsh_string *cipher = simple_decode_hex("37785901863F5C81260EA41E7580CDA5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 37 ... ", stderr);
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
    fputs("Decrypting with rijndael 37 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AA") ;
    struct lsh_string *plain = simple_decode_hex("5D5A990EAAB9093AFE4CE254DFA49EF9") ;
    struct lsh_string *cipher = simple_decode_hex("A07B9338E92ED105E6AD720FCCCE9FE4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 38 ... ", stderr);
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
    fputs("Decrypting with rijndael 38 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("ACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("4CD1E2FD3F4434B553AAE453F0ED1A02") ;
    struct lsh_string *cipher = simple_decode_hex("AE0FB9722418CC21A7DA816BBC61322C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 39 ... ", stderr);
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
    fputs("Decrypting with rijndael 39 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2") ;
    struct lsh_string *plain = simple_decode_hex("5A2C9A9641D4299125FA1B9363104B5E") ;
    struct lsh_string *cipher = simple_decode_hex("C826A193080FF91FFB21F71D3373C877") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 40 ... ", stderr);
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
    fputs("Decrypting with rijndael 40 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("B517FE34C0FA217D341740BFD4FE8DD4") ;
    struct lsh_string *cipher = simple_decode_hex("1181B11B0E494E8D8B0AA6B1D5AC2C48") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 41 ... ", stderr);
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
    fputs("Decrypting with rijndael 41 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FA") ;
    struct lsh_string *plain = simple_decode_hex("014BAF2278A69D331D5180103643E99A") ;
    struct lsh_string *cipher = simple_decode_hex("6743C3D1519AB4F2CD9A78AB09A511BD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 42 ... ", stderr);
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
    fputs("Decrypting with rijndael 42 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("FCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("B529BD8164F20D0AA443D4932116841C") ;
    struct lsh_string *cipher = simple_decode_hex("DC55C076D52BACDF2EEFD952946A439D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 43 ... ", stderr);
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
    fputs("Decrypting with rijndael 43 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F202122") ;
    struct lsh_string *plain = simple_decode_hex("2E596DCBB2F33D4216A1176D5BD1E456") ;
    struct lsh_string *cipher = simple_decode_hex("711B17B590FFC72B5C8E342B601E8003") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 44 ... ", stderr);
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
    fputs("Decrypting with rijndael 44 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("24252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("7274A1EA2B7EE2424E9A0E4673689143") ;
    struct lsh_string *cipher = simple_decode_hex("19983BB0950783A537E1339F4AA21C75") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 45 ... ", stderr);
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
    fputs("Decrypting with rijndael 45 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A") ;
    struct lsh_string *plain = simple_decode_hex("AE20020BD4F13E9D90140BEE3B5D26AF") ;
    struct lsh_string *cipher = simple_decode_hex("3BA7762E15554169C0F4FA39164C410C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 46 ... ", stderr);
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
    fputs("Decrypting with rijndael 46 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("BAAC065DA7AC26E855E79C8849D75A02") ;
    struct lsh_string *cipher = simple_decode_hex("A0564C41245AFCA7AF8AA2E0E588EA89") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 47 ... ", stderr);
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
    fputs("Decrypting with rijndael 47 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F707172") ;
    struct lsh_string *plain = simple_decode_hex("7C917D8D1D45FAB9E2540E28832540CC") ;
    struct lsh_string *cipher = simple_decode_hex("5E36A42A2E099F54AE85ECD92E2381ED") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 48 ... ", stderr);
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
    fputs("Decrypting with rijndael 48 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("74757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("BDE6F89E16DAADB0E847A2A614566A91") ;
    struct lsh_string *cipher = simple_decode_hex("770036F878CD0F6CA2268172F106F2FE") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 49 ... ", stderr);
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
    fputs("Decrypting with rijndael 49 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A") ;
    struct lsh_string *plain = simple_decode_hex("C9DE163725F1F5BE44EBB1DB51D07FBC") ;
    struct lsh_string *cipher = simple_decode_hex("7E4E03908B716116443CCF7C94E7C259") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 50 ... ", stderr);
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
    fputs("Decrypting with rijndael 50 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("3AF57A58F0C07DFFA669572B521E2B92") ;
    struct lsh_string *cipher = simple_decode_hex("482735A48C30613A242DD494C7F9185D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 51 ... ", stderr);
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
    fputs("Decrypting with rijndael 51 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2") ;
    struct lsh_string *plain = simple_decode_hex("3D5EBAC306DDE4604F1B4FBBBFCDAE55") ;
    struct lsh_string *cipher = simple_decode_hex("B4C0F6C9D4D7079ADDF9369FC081061D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 52 ... ", stderr);
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
    fputs("Decrypting with rijndael 52 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("C2DFA91BCEB76A1183C995020AC0B556") ;
    struct lsh_string *cipher = simple_decode_hex("D5810FE0509AC53EDCD74F89962E6270") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 53 ... ", stderr);
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
    fputs("Decrypting with rijndael 53 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EA") ;
    struct lsh_string *plain = simple_decode_hex("C70F54305885E9A0746D01EC56C8596B") ;
    struct lsh_string *cipher = simple_decode_hex("03F17A16B3F91848269ECDD38EBB2165") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 54 ... ", stderr);
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
    fputs("Decrypting with rijndael 54 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("ECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("C4F81B610E98012CE000182050C0C2B2") ;
    struct lsh_string *cipher = simple_decode_hex("DA1248C3180348BAD4A93B4D9856C9DF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 55 ... ", stderr);
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
    fputs("Decrypting with rijndael 55 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("00010203050607080A0B0C0D0F101112") ;
    struct lsh_string *plain = simple_decode_hex("EAAB86B1D02A95D7404EFF67489F97D4") ;
    struct lsh_string *cipher = simple_decode_hex("3D10D7B63F3452C06CDF6CCE18BE0C2C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 56 ... ", stderr);
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
    fputs("Decrypting with rijndael 56 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("7C55BDB40B88870B52BEC3738DE82886") ;
    struct lsh_string *cipher = simple_decode_hex("4AB823E7477DFDDC0E6789018FCB6258") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 57 ... ", stderr);
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
    fputs("Decrypting with rijndael 57 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("BA6EAA88371FF0A3BD875E3F2A975CE0") ;
    struct lsh_string *cipher = simple_decode_hex("E6478BA56A77E70CFDAA5C843ABDE30E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 58 ... ", stderr);
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
    fputs("Decrypting with rijndael 58 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("3C3D3E3F41424344464748494B4C4D4E") ;
    struct lsh_string *plain = simple_decode_hex("08059130C4C24BD30CF0575E4E0373DC") ;
    struct lsh_string *cipher = simple_decode_hex("1673064895FBEAF7F09C5429FF75772D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 59 ... ", stderr);
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
    fputs("Decrypting with rijndael 59 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("50515253555657585A5B5C5D5F606162") ;
    struct lsh_string *plain = simple_decode_hex("9A8EAB004EF53093DFCF96F57E7EDA82") ;
    struct lsh_string *cipher = simple_decode_hex("4488033AE9F2EFD0CA9383BFCA1A94E9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 60 ... ", stderr);
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
    fputs("Decrypting with rijndael 60 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("64656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("0745B589E2400C25F117B1D796C28129") ;
    struct lsh_string *cipher = simple_decode_hex("978F3B8C8F9D6F46626CAC3C0BCB9217") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 61 ... ", stderr);
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
    fputs("Decrypting with rijndael 61 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A") ;
    struct lsh_string *plain = simple_decode_hex("2F1777781216CEC3F044F134B1B92BBE") ;
    struct lsh_string *cipher = simple_decode_hex("E08C8A7E582E15E5527F1D9E2EECB236") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 62 ... ", stderr);
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
    fputs("Decrypting with rijndael 62 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("353A779FFC541B3A3805D90CE17580FC") ;
    struct lsh_string *cipher = simple_decode_hex("CEC155B76AC5FFDA4CF4F9CA91E49A7A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 63 ... ", stderr);
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
    fputs("Decrypting with rijndael 63 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2") ;
    struct lsh_string *plain = simple_decode_hex("1A1EAE4415CEFCF08C4AC1C8F68BEA8F") ;
    struct lsh_string *cipher = simple_decode_hex("D5AC7165763225DD2A38CDC6862C29AD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 64 ... ", stderr);
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
    fputs("Decrypting with rijndael 64 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("E6E7E4E5B0B3B2B5D4D5AAAB16111013") ;
    struct lsh_string *cipher = simple_decode_hex("03680FE19F7CE7275452020BE70E8204") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 65 ... ", stderr);
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
    fputs("Decrypting with rijndael 65 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DA") ;
    struct lsh_string *plain = simple_decode_hex("F8F9FAFBFBF8F9E677767170EFE0E1E2") ;
    struct lsh_string *cipher = simple_decode_hex("461DF740C9781C388E94BB861CEB54F6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 66 ... ", stderr);
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
    fputs("Decrypting with rijndael 66 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("DCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("63626160A1A2A3A445444B4A75727370") ;
    struct lsh_string *cipher = simple_decode_hex("451BD60367F96483042742219786A074") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 67 ... ", stderr);
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
    fputs("Decrypting with rijndael 67 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE010002") ;
    struct lsh_string *plain = simple_decode_hex("717073720605040B2D2C2B2A05FAFBF9") ;
    struct lsh_string *cipher = simple_decode_hex("E4DFA42671A02E57EF173B85C0EA9F2B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 68 ... ", stderr);
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
    fputs("Decrypting with rijndael 68 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("04050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("78797A7BEAE9E8EF3736292891969794") ;
    struct lsh_string *cipher = simple_decode_hex("ED11B89E76274282227D854700A78B9E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 69 ... ", stderr);
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
    fputs("Decrypting with rijndael 69 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("18191A1B1D1E1F20222324252728292A") ;
    struct lsh_string *plain = simple_decode_hex("838281803231300FDDDCDBDAA0AFAEAD") ;
    struct lsh_string *cipher = simple_decode_hex("433946EAA51EA47AF33895F2B90B3B75") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 70 ... ", stderr);
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
    fputs("Decrypting with rijndael 70 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("18191A1BBFBCBDBA75747B7A7F78797A") ;
    struct lsh_string *cipher = simple_decode_hex("6BC6D616A5D7D0284A5910AB35022528") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 71 ... ", stderr);
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
    fputs("Decrypting with rijndael 71 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F505152") ;
    struct lsh_string *plain = simple_decode_hex("848586879B989996A3A2A5A4849B9A99") ;
    struct lsh_string *cipher = simple_decode_hex("D2A920ECFE919D354B5F49EAE9719C98") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 72 ... ", stderr);
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
    fputs("Decrypting with rijndael 72 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("54555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("0001020322212027CACBF4F551565754") ;
    struct lsh_string *cipher = simple_decode_hex("3A061B17F6A92885EFBD0676985B373D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 73 ... ", stderr);
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
    fputs("Decrypting with rijndael 73 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A") ;
    struct lsh_string *plain = simple_decode_hex("CECFCCCDAFACADB2515057564A454447") ;
    struct lsh_string *cipher = simple_decode_hex("FADEEC16E33EA2F4688499D157E20D8F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 74 ... ", stderr);
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
    fputs("Decrypting with rijndael 74 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("92939091CDCECFC813121D1C80878685") ;
    struct lsh_string *cipher = simple_decode_hex("5CDEFEDE59601AA3C3CDA36FA6B1FA13") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 75 ... ", stderr);
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
    fputs("Decrypting with rijndael 75 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("90919293959697989A9B9C9D9FA0A1A2") ;
    struct lsh_string *plain = simple_decode_hex("D2D3D0D16F6C6D6259585F5ED1EEEFEC") ;
    struct lsh_string *cipher = simple_decode_hex("9574B00039844D92EBBA7EE8719265F8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 76 ... ", stderr);
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
    fputs("Decrypting with rijndael 76 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("ACADAEAF878485820F0E1110D5D2D3D0") ;
    struct lsh_string *cipher = simple_decode_hex("9A9CF33758671787E5006928188643FA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 77 ... ", stderr);
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
    fputs("Decrypting with rijndael 77 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CA") ;
    struct lsh_string *plain = simple_decode_hex("9091929364676619E6E7E0E1757A7B78") ;
    struct lsh_string *cipher = simple_decode_hex("2CDDD634C846BA66BB46CBFEA4A674F9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 78 ... ", stderr);
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
    fputs("Decrypting with rijndael 78 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("CCCDCECFD1D2D3D4D6D7D8D9DBDCDDDE") ;
    struct lsh_string *plain = simple_decode_hex("BABBB8B98A89888F74757A7B92959497") ;
    struct lsh_string *cipher = simple_decode_hex("D28BAE029393C3E7E26E9FAFBBB4B98F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 79 ... ", stderr);
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
    fputs("Decrypting with rijndael 79 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2") ;
    struct lsh_string *plain = simple_decode_hex("8D8C8F8E6E6D6C633B3A3D3CCAD5D4D7") ;
    struct lsh_string *cipher = simple_decode_hex("EC27529B1BEE0A9AB6A0D73EBC82E9B7") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 80 ... ", stderr);
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
    fputs("Decrypting with rijndael 80 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F4F5F6F7F9FAFBFCFEFE010103040506") ;
    struct lsh_string *plain = simple_decode_hex("86878485010203040808F7F767606162") ;
    struct lsh_string *cipher = simple_decode_hex("3CB25C09472AFF6EE7E2B47CCD7CCB17") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 81 ... ", stderr);
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
    fputs("Decrypting with rijndael 81 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A") ;
    struct lsh_string *plain = simple_decode_hex("8E8F8C8D656667788A8B8C8D010E0F0C") ;
    struct lsh_string *cipher = simple_decode_hex("DEE33103A7283370D725E44CA38F8FE5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 82 ... ", stderr);
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
    fputs("Decrypting with rijndael 82 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("1C1D1E1F21222324262728292B2C2D2E") ;
    struct lsh_string *plain = simple_decode_hex("C8C9CACB858687807A7B7475E7E0E1E2") ;
    struct lsh_string *cipher = simple_decode_hex("27F9BCD1AAC64BFFC11E7815702C1A69") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 83 ... ", stderr);
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
    fputs("Decrypting with rijndael 83 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("30313233353637383A3B3C3D3F404142") ;
    struct lsh_string *plain = simple_decode_hex("6D6C6F6E5053525D8C8D8A8BADD2D3D0") ;
    struct lsh_string *cipher = simple_decode_hex("5DF534FFAD4ED0749A9988E9849D0021") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 84 ... ", stderr);
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
    fputs("Decrypting with rijndael 84 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("44454647494A4B4C4E4F505153545556") ;
    struct lsh_string *plain = simple_decode_hex("28292A2B393A3B3C0607181903040506") ;
    struct lsh_string *cipher = simple_decode_hex("A48BEE75DB04FB60CA2B80F752A8421B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 85 ... ", stderr);
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
    fputs("Decrypting with rijndael 85 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("58595A5B5D5E5F60626364656768696A") ;
    struct lsh_string *plain = simple_decode_hex("A5A4A7A6B0B3B28DDBDADDDCBDB2B3B0") ;
    struct lsh_string *cipher = simple_decode_hex("024C8CF70BC86EE5CE03678CB7AF45F9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 86 ... ", stderr);
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
    fputs("Decrypting with rijndael 86 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("323330316467666130313E3F2C2B2A29") ;
    struct lsh_string *cipher = simple_decode_hex("3C19AC0F8A3A3862CE577831301E166B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 87 ... ", stderr);
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
    fputs("Decrypting with rijndael 87 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F909192") ;
    struct lsh_string *plain = simple_decode_hex("27262524080B0A05171611100B141516") ;
    struct lsh_string *cipher = simple_decode_hex("C5E355B796A57421D59CA6BE82E73BCA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 88 ... ", stderr);
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
    fputs("Decrypting with rijndael 88 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("94959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("040506074142434435340B0AA3A4A5A6") ;
    struct lsh_string *cipher = simple_decode_hex("D94033276417ABFB05A69D15B6E386E2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 89 ... ", stderr);
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
    fputs("Decrypting with rijndael 89 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BA") ;
    struct lsh_string *plain = simple_decode_hex("242526271112130C61606766BDB2B3B0") ;
    struct lsh_string *cipher = simple_decode_hex("24B36559EA3A9B9B958FE6DA3E5B8D85") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 90 ... ", stderr);
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
    fputs("Decrypting with rijndael 90 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("BCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCE") ;
    struct lsh_string *plain = simple_decode_hex("4B4A4948252627209E9F9091CEC9C8CB") ;
    struct lsh_string *cipher = simple_decode_hex("20FD4FEAA0E8BF0CCE7861D74EF4CB72") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 91 ... ", stderr);
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
    fputs("Decrypting with rijndael 91 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2") ;
    struct lsh_string *plain = simple_decode_hex("68696A6B6665646B9F9E9998D9E6E7E4") ;
    struct lsh_string *cipher = simple_decode_hex("350E20D5174277B9EC314C501570A11D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 92 ... ", stderr);
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
    fputs("Decrypting with rijndael 92 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("34353637C5C6C7C0F0F1EEEF7C7B7A79") ;
    struct lsh_string *cipher = simple_decode_hex("87A29D61B7C604D238FE73045A7EFD57") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 93 ... ", stderr);
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
    fputs("Decrypting with rijndael 93 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F8F9FAFBFDFEFE00020304050708090A") ;
    struct lsh_string *plain = simple_decode_hex("32333031C2C1C13F0D0C0B0A050A0B08") ;
    struct lsh_string *cipher = simple_decode_hex("2C3164C1CC7D0064816BDC0FAA362C52") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 94 ... ", stderr);
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
    fputs("Decrypting with rijndael 94 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("0C0D0E0F11121314161718191B1C1D1E") ;
    struct lsh_string *plain = simple_decode_hex("CDCCCFCEBEBDBCBBABAAA5A4181F1E1D") ;
    struct lsh_string *cipher = simple_decode_hex("195FE5E8A05A2ED594F6E4400EEE10B3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 95 ... ", stderr);
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
    fputs("Decrypting with rijndael 95 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("20212223252627282A2B2C2D2F303132") ;
    struct lsh_string *plain = simple_decode_hex("212023223635343BA0A1A6A7445B5A59") ;
    struct lsh_string *cipher = simple_decode_hex("E4663DF19B9A21A5A284C2BD7F905025") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 96 ... ", stderr);
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
    fputs("Decrypting with rijndael 96 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("34353637393A3B3C3E3F404143444546") ;
    struct lsh_string *plain = simple_decode_hex("0E0F0C0DA8ABAAAD2F2E515002050407") ;
    struct lsh_string *cipher = simple_decode_hex("21B88714CFB4E2A933BD281A2C4743FD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 97 ... ", stderr);
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
    fputs("Decrypting with rijndael 97 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("48494A4B4D4E4F50525354555758595A") ;
    struct lsh_string *plain = simple_decode_hex("070605042A2928378E8F8889BDB2B3B0") ;
    struct lsh_string *cipher = simple_decode_hex("CBFC3980D704FD0FC54378AB84E17870") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 98 ... ", stderr);
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
    fputs("Decrypting with rijndael 98 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("5C5D5E5F61626364666768696B6C6D6E") ;
    struct lsh_string *plain = simple_decode_hex("CBCAC9C893909196A9A8A7A6A5A2A3A0") ;
    struct lsh_string *cipher = simple_decode_hex("BC5144BAA48BDEB8B63E22E03DA418EF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 99 ... ", stderr);
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
    fputs("Decrypting with rijndael 99 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F808182") ;
    struct lsh_string *plain = simple_decode_hex("80818283C1C2C3CC9C9D9A9B0CF3F2F1") ;
    struct lsh_string *cipher = simple_decode_hex("5A1DBAEF1EE2984B8395DA3BDFFA3CCC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 100 ... ", stderr);
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
    fputs("Decrypting with rijndael 100 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("84858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("1213101125262720FAFBE4E5B1B6B7B4") ;
    struct lsh_string *cipher = simple_decode_hex("F0B11CD0729DFCC80CEC903D97159574") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 101 ... ", stderr);
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
    fputs("Decrypting with rijndael 101 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AA") ;
    struct lsh_string *plain = simple_decode_hex("7F7E7D7C3033320D97969190222D2C2F") ;
    struct lsh_string *cipher = simple_decode_hex("9F95314ACFDDC6D1914B7F19A9CC8209") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 102 ... ", stderr);
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
    fputs("Decrypting with rijndael 102 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("ACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("4E4F4C4D484B4A4D81808F8E53545556") ;
    struct lsh_string *cipher = simple_decode_hex("595736F6F0F70914A94E9E007F022519") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 103 ... ", stderr);
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
    fputs("Decrypting with rijndael 103 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2") ;
    struct lsh_string *plain = simple_decode_hex("DCDDDEDFB0B3B2BD15141312A1BEBFBC") ;
    struct lsh_string *cipher = simple_decode_hex("1F19F57892CAE586FCDFB4C694DEB183") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 104 ... ", stderr);
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
    fputs("Decrypting with rijndael 104 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("93929190282B2A2DC4C5FAFB92959497") ;
    struct lsh_string *cipher = simple_decode_hex("540700EE1F6F3DAB0B3EDDF6CAEE1EF5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 105 ... ", stderr);
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
    fputs("Decrypting with rijndael 105 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FA") ;
    struct lsh_string *plain = simple_decode_hex("F5F4F7F6C4C7C6D9373631307E717073") ;
    struct lsh_string *cipher = simple_decode_hex("14A342A91019A331687A2254E6626CA2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 106 ... ", stderr);
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
    fputs("Decrypting with rijndael 106 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("FCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("93929190B6B5B4B364656A6B05020300") ;
    struct lsh_string *cipher = simple_decode_hex("7B25F3C3B2EEA18D743EF283140F29FF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 107 ... ", stderr);
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
    fputs("Decrypting with rijndael 107 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F202122") ;
    struct lsh_string *plain = simple_decode_hex("BABBB8B90D0E0F00A4A5A2A3043B3A39") ;
    struct lsh_string *cipher = simple_decode_hex("46C2587D66E5E6FA7F7CA6411AD28047") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 108 ... ", stderr);
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
    fputs("Decrypting with rijndael 108 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("24252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("D8D9DADB7F7C7D7A10110E0F787F7E7D") ;
    struct lsh_string *cipher = simple_decode_hex("09470E72229D954ED5EE73886DFEEBA9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 109 ... ", stderr);
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
    fputs("Decrypting with rijndael 109 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A") ;
    struct lsh_string *plain = simple_decode_hex("FEFFFCFDEFECED923B3A3D3C6768696A") ;
    struct lsh_string *cipher = simple_decode_hex("D77C03DE92D4D0D79EF8D4824EF365EB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 110 ... ", stderr);
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
    fputs("Decrypting with rijndael 110 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("D6D7D4D58A89888F96979899A5A2A3A0") ;
    struct lsh_string *cipher = simple_decode_hex("1D190219F290E0F1715D152D41A23593") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 111 ... ", stderr);
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
    fputs("Decrypting with rijndael 111 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F707172") ;
    struct lsh_string *plain = simple_decode_hex("18191A1BA8ABAAA5303136379B848586") ;
    struct lsh_string *cipher = simple_decode_hex("A2CD332CE3A0818769616292E87F757B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 112 ... ", stderr);
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
    fputs("Decrypting with rijndael 112 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("74757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("6B6A6968A4A7A6A1D6D72829B0B7B6B5") ;
    struct lsh_string *cipher = simple_decode_hex("D54AFA6CE60FBF9341A3690E21385102") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 113 ... ", stderr);
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
    fputs("Decrypting with rijndael 113 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A") ;
    struct lsh_string *plain = simple_decode_hex("000102038A89889755545352A6A9A8AB") ;
    struct lsh_string *cipher = simple_decode_hex("06E5C364DED628A3F5E05E613E356F46") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 114 ... ", stderr);
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
    fputs("Decrypting with rijndael 114 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("2D2C2F2EB3B0B1B6B6B7B8B9F2F5F4F7") ;
    struct lsh_string *cipher = simple_decode_hex("EAE63C0E62556DAC85D221099896355A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 115 ... ", stderr);
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
    fputs("Decrypting with rijndael 115 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2") ;
    struct lsh_string *plain = simple_decode_hex("979695943536373856575051E09F9E9D") ;
    struct lsh_string *cipher = simple_decode_hex("1FED060E2C6FC93EE764403A889985A2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 116 ... ", stderr);
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
    fputs("Decrypting with rijndael 116 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("A4A5A6A7989B9A9DB1B0AFAE7A7D7C7F") ;
    struct lsh_string *cipher = simple_decode_hex("C25235C1A30FDEC1C7CB5C5737B2A588") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 117 ... ", stderr);
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
    fputs("Decrypting with rijndael 117 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EA") ;
    struct lsh_string *plain = simple_decode_hex("C1C0C3C2686B6A55A8A9AEAFEAE5E4E7") ;
    struct lsh_string *cipher = simple_decode_hex("796DBEF95147D4D30873AD8B7B92EFC0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 118 ... ", stderr);
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
    fputs("Decrypting with rijndael 118 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("ECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("C1C0C3C2141716118C8D828364636261") ;
    struct lsh_string *cipher = simple_decode_hex("CBCF0FB34D98D0BD5C22CE37211A46BF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 119 ... ", stderr);
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
    fputs("Decrypting with rijndael 119 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("00010203050607080A0B0C0D0F101112") ;
    struct lsh_string *plain = simple_decode_hex("93929190CCCFCEC196979091E0FFFEFD") ;
    struct lsh_string *cipher = simple_decode_hex("94B44DA6466126CAFA7C7FD09063FC24") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 120 ... ", stderr);
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
    fputs("Decrypting with rijndael 120 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("B4B5B6B7F9FAFBFC25241B1A6E69686B") ;
    struct lsh_string *cipher = simple_decode_hex("D78C5B5EBF9B4DBDA6AE506C5074C8FE") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 121 ... ", stderr);
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
    fputs("Decrypting with rijndael 121 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("868784850704051AC7C6C1C08788898A") ;
    struct lsh_string *cipher = simple_decode_hex("6C27444C27204B043812CF8CF95F9769") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 122 ... ", stderr);
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
    fputs("Decrypting with rijndael 122 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("3C3D3E3F41424344464748494B4C4D4E") ;
    struct lsh_string *plain = simple_decode_hex("F4F5F6F7AAA9A8AFFDFCF3F277707172") ;
    struct lsh_string *cipher = simple_decode_hex("BE94524EE5A2AA50BBA8B75F4C0AEBCF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 123 ... ", stderr);
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
    fputs("Decrypting with rijndael 123 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("50515253555657585A5B5C5D5F606162") ;
    struct lsh_string *plain = simple_decode_hex("D3D2D1D00605040BC3C2C5C43E010003") ;
    struct lsh_string *cipher = simple_decode_hex("A0AEAAE91BA9F31F51AEB3588CF3A39E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 124 ... ", stderr);
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
    fputs("Decrypting with rijndael 124 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("64656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("73727170424140476A6B74750D0A0B08") ;
    struct lsh_string *cipher = simple_decode_hex("275297779C28266EF9FE4C6A13C08488") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 125 ... ", stderr);
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
    fputs("Decrypting with rijndael 125 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A") ;
    struct lsh_string *plain = simple_decode_hex("C2C3C0C10A0908F754555253A1AEAFAC") ;
    struct lsh_string *cipher = simple_decode_hex("86523D92BB8672CB01CF4A77FD725882") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 126 ... ", stderr);
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
    fputs("Decrypting with rijndael 126 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("6D6C6F6EF8FBFAFD82838C8DF8FFFEFD") ;
    struct lsh_string *cipher = simple_decode_hex("4B8327640E9F33322A04DD96FCBF9A36") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 127 ... ", stderr);
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
    fputs("Decrypting with rijndael 127 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2") ;
    struct lsh_string *plain = simple_decode_hex("F5F4F7F684878689A6A7A0A1D2CDCCCF") ;
    struct lsh_string *cipher = simple_decode_hex("CE52AF650D088CA559425223F4D32694") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 128 ... ", stderr);
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
    fputs("Decrypting with rijndael 128 ... ", stderr);
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
    fputs("Encrypting with rijndael 1 ... ", stderr);
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
    fputs("Decrypting with rijndael 1 ... ", stderr);
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
    fputs("Encrypting with rijndael 2 ... ", stderr);
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
    fputs("Decrypting with rijndael 2 ... ", stderr);
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
    fputs("Encrypting with rijndael 3 ... ", stderr);
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
    fputs("Decrypting with rijndael 3 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("5A5B5C5D5F60616264656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("AA214402B46CFFB9F761EC11263A311E") ;
    struct lsh_string *cipher = simple_decode_hex("3A7AC027753E2A18C2CEAB9E17C11FD0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 4 ... ", stderr);
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
    fputs("Decrypting with rijndael 4 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A8C8D8E8F91929394") ;
    struct lsh_string *plain = simple_decode_hex("02AEA86E572EEAB66B2C3AF5E9A46FD6") ;
    struct lsh_string *cipher = simple_decode_hex("8F6786BD007528BA26603C1601CDD0D8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 5 ... ", stderr);
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
    fputs("Decrypting with rijndael 5 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("969798999B9C9D9EA0A1A2A3A5A6A7A8AAABACADAFB0B1B2") ;
    struct lsh_string *plain = simple_decode_hex("E2AEF6ACC33B965C4FA1F91C75FF6F36") ;
    struct lsh_string *cipher = simple_decode_hex("D17D073B01E71502E28B47AB551168B3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 6 ... ", stderr);
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
    fputs("Decrypting with rijndael 6 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6C8C9CACBCDCECFD0") ;
    struct lsh_string *plain = simple_decode_hex("0659DF46427162B9434865DD9499F91D") ;
    struct lsh_string *cipher = simple_decode_hex("A469DA517119FAB95876F41D06D40FFA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 7 ... ", stderr);
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
    fputs("Decrypting with rijndael 7 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("49A44239C748FEB456F59C276A5658DF") ;
    struct lsh_string *cipher = simple_decode_hex("6091AA3B695C11F5C0B6AD26D3D862FF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 8 ... ", stderr);
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
    fputs("Decrypting with rijndael 8 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE01000204050607090A0B0C") ;
    struct lsh_string *plain = simple_decode_hex("66208F6E9D04525BDEDB2733B6A6BE37") ;
    struct lsh_string *cipher = simple_decode_hex("70F9E67F9F8DF1294131662DC6E69364") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 9 ... ", stderr);
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
    fputs("Decrypting with rijndael 9 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("0E0F10111314151618191A1B1D1E1F20222324252728292A") ;
    struct lsh_string *plain = simple_decode_hex("3393F8DFC729C97F5480B950BC9666B0") ;
    struct lsh_string *cipher = simple_decode_hex("D154DCAFAD8B207FA5CBC95E9996B559") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 10 ... ", stderr);
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
    fputs("Decrypting with rijndael 10 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("2C2D2E2F31323334363738393B3C3D3E4041424345464748") ;
    struct lsh_string *plain = simple_decode_hex("606834C8CE063F3234CF1145325DBD71") ;
    struct lsh_string *cipher = simple_decode_hex("4934D541E8B46FA339C805A7AEB9E5DA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 11 ... ", stderr);
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
    fputs("Decrypting with rijndael 11 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("4A4B4C4D4F50515254555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("FEC1C04F529BBD17D8CECFCC4718B17F") ;
    struct lsh_string *cipher = simple_decode_hex("62564C738F3EFE186E1A127A0C4D3C61") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 12 ... ", stderr);
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
    fputs("Decrypting with rijndael 12 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A7C7D7E7F81828384") ;
    struct lsh_string *plain = simple_decode_hex("32DF99B431ED5DC5ACF8CAF6DC6CE475") ;
    struct lsh_string *cipher = simple_decode_hex("07805AA043986EB23693E23BEF8F3438") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 13 ... ", stderr);
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
    fputs("Decrypting with rijndael 13 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("868788898B8C8D8E90919293959697989A9B9C9D9FA0A1A2") ;
    struct lsh_string *plain = simple_decode_hex("7FDC2B746F3F665296943B83710D1F82") ;
    struct lsh_string *cipher = simple_decode_hex("DF0B4931038BADE848DEE3B4B85AA44B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 14 ... ", stderr);
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
    fputs("Decrypting with rijndael 14 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6B8B9BABBBDBEBFC0") ;
    struct lsh_string *plain = simple_decode_hex("8FBA1510A3C5B87E2EAA3F7A91455CA2") ;
    struct lsh_string *cipher = simple_decode_hex("592D5FDED76582E4143C65099309477C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 15 ... ", stderr);
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
    fputs("Decrypting with rijndael 15 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C2C3C4C5C7C8C9CACCCDCECFD1D2D3D4D6D7D8D9DBDCDDDE") ;
    struct lsh_string *plain = simple_decode_hex("2C9B468B1C2EED92578D41B0716B223B") ;
    struct lsh_string *cipher = simple_decode_hex("C9B8D6545580D3DFBCDD09B954ED4E92") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 16 ... ", stderr);
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
    fputs("Decrypting with rijndael 16 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2F4F5F6F7F9FAFBFC") ;
    struct lsh_string *plain = simple_decode_hex("0A2BBF0EFC6BC0034F8A03433FCA1B1A") ;
    struct lsh_string *cipher = simple_decode_hex("5DCCD5D6EB7C1B42ACB008201DF707A0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 17 ... ", stderr);
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
    fputs("Decrypting with rijndael 17 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("FEFE01010304050608090A0B0D0E0F10121314151718191A") ;
    struct lsh_string *plain = simple_decode_hex("25260E1F31F4104D387222E70632504B") ;
    struct lsh_string *cipher = simple_decode_hex("A2A91682FFEB6ED1D34340946829E6F9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 18 ... ", stderr);
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
    fputs("Decrypting with rijndael 18 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("1C1D1E1F21222324262728292B2C2D2E3031323335363738") ;
    struct lsh_string *plain = simple_decode_hex("C527D25A49F08A5228D338642AE65137") ;
    struct lsh_string *cipher = simple_decode_hex("E45D185B797000348D9267960A68435D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 19 ... ", stderr);
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
    fputs("Decrypting with rijndael 19 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("3A3B3C3D3F40414244454647494A4B4C4E4F505153545556") ;
    struct lsh_string *plain = simple_decode_hex("3B49FC081432F5890D0E3D87E884A69E") ;
    struct lsh_string *cipher = simple_decode_hex("45E060DAE5901CDA8089E10D4F4C246B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 20 ... ", stderr);
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
    fputs("Decrypting with rijndael 20 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("58595A5B5D5E5F60626364656768696A6C6D6E6F71727374") ;
    struct lsh_string *plain = simple_decode_hex("D173F9ED1E57597E166931DF2754A083") ;
    struct lsh_string *cipher = simple_decode_hex("F6951AFACC0079A369C71FDCFF45DF50") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 21 ... ", stderr);
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
    fputs("Decrypting with rijndael 21 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("767778797B7C7D7E80818283858687888A8B8C8D8F909192") ;
    struct lsh_string *plain = simple_decode_hex("8C2B7CAFA5AFE7F13562DAEAE1ADEDE0") ;
    struct lsh_string *cipher = simple_decode_hex("9E95E00F351D5B3AC3D0E22E626DDAD6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 22 ... ", stderr);
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
    fputs("Decrypting with rijndael 22 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("94959697999A9B9C9E9FA0A1A3A4A5A6A8A9AAABADAEAFB0") ;
    struct lsh_string *plain = simple_decode_hex("AAF4EC8C1A815AEB826CAB741339532C") ;
    struct lsh_string *cipher = simple_decode_hex("9CB566FF26D92DAD083B51FDC18C173C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 23 ... ", stderr);
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
    fputs("Decrypting with rijndael 23 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2E4E5E6E7E9EAEBEC") ;
    struct lsh_string *plain = simple_decode_hex("40BE8C5D9108E663F38F1A2395279ECF") ;
    struct lsh_string *cipher = simple_decode_hex("C9C82766176A9B228EB9A974A010B4FB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 24 ... ", stderr);
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
    fputs("Decrypting with rijndael 24 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("2A2B2C2D2F30313234353637393A3B3C3E3F404143444546") ;
    struct lsh_string *plain = simple_decode_hex("0C8AD9BC32D43E04716753AA4CFBE351") ;
    struct lsh_string *cipher = simple_decode_hex("D8E26AA02945881D5137F1C1E1386E88") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 25 ... ", stderr);
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
    fputs("Decrypting with rijndael 25 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("48494A4B4D4E4F50525354555758595A5C5D5E5F61626364") ;
    struct lsh_string *plain = simple_decode_hex("1407B1D5F87D63357C8DC7EBBAEBBFEE") ;
    struct lsh_string *cipher = simple_decode_hex("C0E024CCD68FF5FFA4D139C355A77C55") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 26 ... ", stderr);
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
    fputs("Decrypting with rijndael 26 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("84858687898A8B8C8E8F90919394959698999A9B9D9E9FA0") ;
    struct lsh_string *plain = simple_decode_hex("E62734D1AE3378C4549E939E6F123416") ;
    struct lsh_string *cipher = simple_decode_hex("0B18B3D16F491619DA338640DF391D43") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 27 ... ", stderr);
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
    fputs("Decrypting with rijndael 27 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A2A3A4A5A7A8A9AAACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("5A752CFF2A176DB1A1DE77F2D2CDEE41") ;
    struct lsh_string *cipher = simple_decode_hex("DBE09AC8F66027BF20CB6E434F252EFC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 28 ... ", stderr);
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
    fputs("Decrypting with rijndael 28 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2D4D5D6D7D9DADBDC") ;
    struct lsh_string *plain = simple_decode_hex("A9C8C3A4EABEDC80C64730DDD018CD88") ;
    struct lsh_string *cipher = simple_decode_hex("6D04E5E43C5B9CBE05FEB9606B6480FE") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 29 ... ", stderr);
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
    fputs("Decrypting with rijndael 29 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("1A1B1C1D1F20212224252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("EE9B3DBBDB86180072130834D305999A") ;
    struct lsh_string *cipher = simple_decode_hex("DD1D6553B96BE526D9FEE0FBD7176866") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 30 ... ", stderr);
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
    fputs("Decrypting with rijndael 30 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A4C4D4E4F51525354") ;
    struct lsh_string *plain = simple_decode_hex("A7FA8C3586B8EBDE7568EAD6F634A879") ;
    struct lsh_string *cipher = simple_decode_hex("0260CA7E3F979FD015B0DD4690E16D2A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 31 ... ", stderr);
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
    fputs("Decrypting with rijndael 31 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("929394959798999A9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("37E0F4A87F127D45AC936FE7AD88C10A") ;
    struct lsh_string *cipher = simple_decode_hex("9893734DE10EDCC8A67C3B110B8B8CC6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 32 ... ", stderr);
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
    fputs("Decrypting with rijndael 32 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("464748494B4C4D4E50515253555657585A5B5C5D5F606162") ;
    struct lsh_string *plain = simple_decode_hex("3F77D8B5D92BAC148E4E46F697A535C5") ;
    struct lsh_string *cipher = simple_decode_hex("93B30B750516B2D18808D710C2EE84EF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 33 ... ", stderr);
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
    fputs("Decrypting with rijndael 33 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("828384858788898A8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("D25EBB686C40F7E2C4DA1014936571CA") ;
    struct lsh_string *cipher = simple_decode_hex("16F65FA47BE3CB5E6DFE7C6C37016C0E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 34 ... ", stderr);
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
    fputs("Decrypting with rijndael 34 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2B4B5B6B7B9BABBBC") ;
    struct lsh_string *plain = simple_decode_hex("4F1C769D1E5B0552C7ECA84DEA26A549") ;
    struct lsh_string *cipher = simple_decode_hex("F3847210D5391E2360608E5ACB560581") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 35 ... ", stderr);
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
    fputs("Decrypting with rijndael 35 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("BEBFC0C1C3C4C5C6C8C9CACBCDCECFD0D2D3D4D5D7D8D9DA") ;
    struct lsh_string *plain = simple_decode_hex("8548E2F882D7584D0FAFC54372B6633A") ;
    struct lsh_string *cipher = simple_decode_hex("8754462CD223366D0753913E6AF2643D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 36 ... ", stderr);
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
    fputs("Decrypting with rijndael 36 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("DCDDDEDFE1E2E3E4E6E7E8E9EBECEDEEF0F1F2F3F5F6F7F8") ;
    struct lsh_string *plain = simple_decode_hex("87D7A336CB476F177CD2A51AF2A62CDF") ;
    struct lsh_string *cipher = simple_decode_hex("1EA20617468D1B806A1FD58145462017") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 37 ... ", stderr);
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
    fputs("Decrypting with rijndael 37 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("FAFBFCFDFE01000204050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("03B1FEAC668C4E485C1065DFC22B44EE") ;
    struct lsh_string *cipher = simple_decode_hex("3B155D927355D737C6BE9DDA60136E2E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 38 ... ", stderr);
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
    fputs("Decrypting with rijndael 38 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("18191A1B1D1E1F20222324252728292A2C2D2E2F31323334") ;
    struct lsh_string *plain = simple_decode_hex("BDA15E66819FA72D653A6866AA287962") ;
    struct lsh_string *cipher = simple_decode_hex("26144F7B66DAA91B6333DBD3850502B3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 39 ... ", stderr);
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
    fputs("Decrypting with rijndael 39 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("363738393B3C3D3E40414243454647484A4B4C4D4F505152") ;
    struct lsh_string *plain = simple_decode_hex("4D0C7A0D2505B80BF8B62CEB12467F0A") ;
    struct lsh_string *cipher = simple_decode_hex("E4F9A4AB52CED8134C649BF319EBCC90") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 40 ... ", stderr);
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
    fputs("Decrypting with rijndael 40 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("54555657595A5B5C5E5F60616364656668696A6B6D6E6F70") ;
    struct lsh_string *plain = simple_decode_hex("626D34C9429B37211330986466B94E5F") ;
    struct lsh_string *cipher = simple_decode_hex("B9DDD29AC6128A6CAB121E34A4C62B36") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 41 ... ", stderr);
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
    fputs("Decrypting with rijndael 41 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("727374757778797A7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("333C3E6BF00656B088A17E5FF0E7F60A") ;
    struct lsh_string *cipher = simple_decode_hex("6FCDDAD898F2CE4EFF51294F5EAAF5C9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 42 ... ", stderr);
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
    fputs("Decrypting with rijndael 42 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("90919293959697989A9B9C9D9FA0A1A2A4A5A6A7A9AAABAC") ;
    struct lsh_string *plain = simple_decode_hex("687ED0CDC0D2A2BC8C466D05EF9D2891") ;
    struct lsh_string *cipher = simple_decode_hex("C9A6FE2BF4028080BEA6F7FC417BD7E3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 43 ... ", stderr);
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
    fputs("Decrypting with rijndael 43 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("AEAFB0B1B3B4B5B6B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CA") ;
    struct lsh_string *plain = simple_decode_hex("487830E78CC56C1693E64B2A6660C7B6") ;
    struct lsh_string *cipher = simple_decode_hex("6A2026846D8609D60F298A9C0673127F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 44 ... ", stderr);
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
    fputs("Decrypting with rijndael 44 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("CCCDCECFD1D2D3D4D6D7D8D9DBDCDDDEE0E1E2E3E5E6E7E8") ;
    struct lsh_string *plain = simple_decode_hex("7A48D6B7B52B29392AA2072A32B66160") ;
    struct lsh_string *cipher = simple_decode_hex("2CB25C005E26EFEA44336C4C97A4240B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 45 ... ", stderr);
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
    fputs("Decrypting with rijndael 45 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("EAEBECEDEFF0F1F2F4F5F6F7F9FAFBFCFEFE010103040506") ;
    struct lsh_string *plain = simple_decode_hex("907320E64C8C5314D10F8D7A11C8618D") ;
    struct lsh_string *cipher = simple_decode_hex("496967AB8680DDD73D09A0E4C7DCC8AA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 46 ... ", stderr);
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
    fputs("Decrypting with rijndael 46 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A1C1D1E1F21222324") ;
    struct lsh_string *plain = simple_decode_hex("B561F2CA2D6E65A4A98341F3ED9FF533") ;
    struct lsh_string *cipher = simple_decode_hex("D5AF94DE93487D1F3A8C577CB84A66A4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 47 ... ", stderr);
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
    fputs("Decrypting with rijndael 47 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("262728292B2C2D2E30313233353637383A3B3C3D3F404142") ;
    struct lsh_string *plain = simple_decode_hex("DF769380D212792D026F049E2E3E48EF") ;
    struct lsh_string *cipher = simple_decode_hex("84BDAC569CAE2828705F267CC8376E90") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 48 ... ", stderr);
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
    fputs("Decrypting with rijndael 48 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("44454647494A4B4C4E4F50515354555658595A5B5D5E5F60") ;
    struct lsh_string *plain = simple_decode_hex("79F374BC445BDABF8FCCB8843D6054C6") ;
    struct lsh_string *cipher = simple_decode_hex("F7401DDA5AD5AB712B7EB5D10C6F99B6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 49 ... ", stderr);
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
    fputs("Decrypting with rijndael 49 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("626364656768696A6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("4E02F1242FA56B05C68DBAE8FE44C9D6") ;
    struct lsh_string *cipher = simple_decode_hex("1C9D54318539EBD4C3B5B7E37BF119F0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 50 ... ", stderr);
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
    fputs("Decrypting with rijndael 50 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F90919294959697999A9B9C") ;
    struct lsh_string *plain = simple_decode_hex("CF73C93CBFF57AC635A6F4AD2A4A1545") ;
    struct lsh_string *cipher = simple_decode_hex("ACA572D65FB2764CFFD4A6ECA090EA0D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 51 ... ", stderr);
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
    fputs("Decrypting with rijndael 51 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("9E9FA0A1A3A4A5A6A8A9AAABADAEAFB0B2B3B4B5B7B8B9BA") ;
    struct lsh_string *plain = simple_decode_hex("9923548E2875750725B886566784C625") ;
    struct lsh_string *cipher = simple_decode_hex("36D9C627B8C2A886A10CCB36EAE3DFBB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 52 ... ", stderr);
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
    fputs("Decrypting with rijndael 52 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("BCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCED0D1D2D3D5D6D7D8") ;
    struct lsh_string *plain = simple_decode_hex("4888336B723A022C9545320F836A4207") ;
    struct lsh_string *cipher = simple_decode_hex("010EDBF5981E143A81D646E597A4A568") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 53 ... ", stderr);
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
    fputs("Decrypting with rijndael 53 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("DADBDCDDDFE0E1E2E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("F84D9A5561B0608B1160DEE000C41BA8") ;
    struct lsh_string *cipher = simple_decode_hex("8DB44D538DC20CC2F40F3067FD298E60") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 54 ... ", stderr);
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
    fputs("Decrypting with rijndael 54 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F8F9FAFBFDFEFE00020304050708090A0C0D0E0F11121314") ;
    struct lsh_string *plain = simple_decode_hex("C23192A0418E30A19B45AE3E3625BF22") ;
    struct lsh_string *cipher = simple_decode_hex("930EB53BC71E6AC4B82972BDCD5AAFB3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 55 ... ", stderr);
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
    fputs("Decrypting with rijndael 55 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("161718191B1C1D1E20212223252627282A2B2C2D2F303132") ;
    struct lsh_string *plain = simple_decode_hex("B84E0690B28B0025381AD82A15E501A7") ;
    struct lsh_string *cipher = simple_decode_hex("6C42A81EDCBC9517CCD89C30C95597B4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 56 ... ", stderr);
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
    fputs("Decrypting with rijndael 56 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("34353637393A3B3C3E3F40414344454648494A4B4D4E4F50") ;
    struct lsh_string *plain = simple_decode_hex("ACEF5E5C108876C4F06269F865B8F0B0") ;
    struct lsh_string *cipher = simple_decode_hex("DA389847AD06DF19D76EE119C71E1DD3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 57 ... ", stderr);
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
    fputs("Decrypting with rijndael 57 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("525354555758595A5C5D5E5F61626364666768696B6C6D6E") ;
    struct lsh_string *plain = simple_decode_hex("0F1B3603E0F5DDEA4548246153A5E064") ;
    struct lsh_string *cipher = simple_decode_hex("E018FDAE13D3118F9A5D1A647A3F0462") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 58 ... ", stderr);
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
    fputs("Decrypting with rijndael 58 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F80818284858687898A8B8C") ;
    struct lsh_string *plain = simple_decode_hex("FBB63893450D42B58C6D88CD3C1809E3") ;
    struct lsh_string *cipher = simple_decode_hex("2AA65DB36264239D3846180FABDFAD20") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 59 ... ", stderr);
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
    fputs("Decrypting with rijndael 59 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("8E8F90919394959698999A9B9D9E9FA0A2A3A4A5A7A8A9AA") ;
    struct lsh_string *plain = simple_decode_hex("4BEF736DF150259DAE0C91354E8A5F92") ;
    struct lsh_string *cipher = simple_decode_hex("1472163E9A4F780F1CEB44B07ECF4FDB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 60 ... ", stderr);
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
    fputs("Decrypting with rijndael 60 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("ACADAEAFB1B2B3B4B6B7B8B9BBBCBDBEC0C1C2C3C5C6C7C8") ;
    struct lsh_string *plain = simple_decode_hex("7D2D46242056EF13D3C3FC93C128F4C7") ;
    struct lsh_string *cipher = simple_decode_hex("C8273FDC8F3A9F72E91097614B62397C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 61 ... ", stderr);
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
    fputs("Decrypting with rijndael 61 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("CACBCCCDCFD0D1D2D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("E9C1BA2DF415657A256EDB33934680FD") ;
    struct lsh_string *cipher = simple_decode_hex("66C8427DCD733AAF7B3470CB7D976E3F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 62 ... ", stderr);
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
    fputs("Decrypting with rijndael 62 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FAFCFDFEFF01020304") ;
    struct lsh_string *plain = simple_decode_hex("E23EE277B0AA0A1DFB81F7527C3514F1") ;
    struct lsh_string *cipher = simple_decode_hex("146131CB17F1424D4F8DA91E6F80C1D0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 63 ... ", stderr);
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
    fputs("Decrypting with rijndael 63 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("060708090B0C0D0E10111213151617181A1B1C1D1F202122") ;
    struct lsh_string *plain = simple_decode_hex("3E7445B0B63CAAF75E4A911E12106B4C") ;
    struct lsh_string *cipher = simple_decode_hex("2610D0AD83659081AE085266A88770DC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 64 ... ", stderr);
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
    fputs("Decrypting with rijndael 64 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("24252627292A2B2C2E2F30313334353638393A3B3D3E3F40") ;
    struct lsh_string *plain = simple_decode_hex("767774752023222544455A5BE6E1E0E3") ;
    struct lsh_string *cipher = simple_decode_hex("38A2B5A974B0575C5D733917FB0D4570") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 65 ... ", stderr);
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
    fputs("Decrypting with rijndael 65 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("424344454748494A4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("72737475717E7F7CE9E8EBEA696A6B6C") ;
    struct lsh_string *cipher = simple_decode_hex("E21D401EBC60DE20D6C486E4F39A588B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 66 ... ", stderr);
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
    fputs("Decrypting with rijndael 66 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F70717274757677797A7B7C") ;
    struct lsh_string *plain = simple_decode_hex("DFDEDDDC25262728C9C8CFCEF1EEEFEC") ;
    struct lsh_string *cipher = simple_decode_hex("E51D5F88C670B079C0CA1F0C2C4405A2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 67 ... ", stderr);
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
    fputs("Decrypting with rijndael 67 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("7E7F80818384858688898A8B8D8E8F90929394959798999A") ;
    struct lsh_string *plain = simple_decode_hex("FFFE0100707776755F5E5D5C7675746B") ;
    struct lsh_string *cipher = simple_decode_hex("246A94788A642FB3D1B823C8762380C8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 68 ... ", stderr);
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
    fputs("Decrypting with rijndael 68 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("9C9D9E9FA1A2A3A4A6A7A8A9ABACADAEB0B1B2B3B5B6B7B8") ;
    struct lsh_string *plain = simple_decode_hex("E0E1E2E3424140479F9E9190292E2F2C") ;
    struct lsh_string *cipher = simple_decode_hex("B80C391C5C41A4C3B30C68E0E3D7550F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 69 ... ", stderr);
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
    fputs("Decrypting with rijndael 69 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("BABBBCBDBFC0C1C2C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("2120272690EFEEED3B3A39384E4D4C4B") ;
    struct lsh_string *cipher = simple_decode_hex("B77C4754FC64EB9A1154A9AF0BB1F21C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 70 ... ", stderr);
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
    fputs("Decrypting with rijndael 70 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EAECEDEEEFF1F2F3F4") ;
    struct lsh_string *plain = simple_decode_hex("ECEDEEEF5350516EA1A0A7A6A3ACADAE") ;
    struct lsh_string *cipher = simple_decode_hex("FB554DE520D159A06BF219FC7F34A02F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 71 ... ", stderr);
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
    fputs("Decrypting with rijndael 71 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F6F7F8F9FBFCFDFE00010203050607080A0B0C0D0F101112") ;
    struct lsh_string *plain = simple_decode_hex("32333C3D25222320E9E8EBEACECDCCC3") ;
    struct lsh_string *cipher = simple_decode_hex("A89FBA152D76B4927BEED160DDB76C57") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 72 ... ", stderr);
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
    fputs("Decrypting with rijndael 72 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("14151617191A1B1C1E1F20212324252628292A2B2D2E2F30") ;
    struct lsh_string *plain = simple_decode_hex("40414243626160678A8BB4B511161714") ;
    struct lsh_string *cipher = simple_decode_hex("5676EAB4A98D2E8473B3F3D46424247C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 73 ... ", stderr);
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
    fputs("Decrypting with rijndael 73 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("323334353738393A3C3D3E3F41424344464748494B4C4D4E") ;
    struct lsh_string *plain = simple_decode_hex("94959293F5FAFBF81F1E1D1C7C7F7E79") ;
    struct lsh_string *cipher = simple_decode_hex("4E8F068BD7EDE52A639036EC86C33568") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 74 ... ", stderr);
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
    fputs("Decrypting with rijndael 74 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("50515253555657585A5B5C5D5F60616264656667696A6B6C") ;
    struct lsh_string *plain = simple_decode_hex("BEBFBCBD191A1B14CFCEC9C8546B6A69") ;
    struct lsh_string *cipher = simple_decode_hex("F0193C4D7AFF1791EE4C07EB4A1824FC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 75 ... ", stderr);
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
    fputs("Decrypting with rijndael 75 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("6E6F70717374757678797A7B7D7E7F80828384858788898A") ;
    struct lsh_string *plain = simple_decode_hex("2C2D3233898E8F8CBBBAB9B8333031CE") ;
    struct lsh_string *cipher = simple_decode_hex("AC8686EECA9BA761AFE82D67B928C33F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 76 ... ", stderr);
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
    fputs("Decrypting with rijndael 76 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("8C8D8E8F91929394969798999B9C9D9EA0A1A2A3A5A6A7A8") ;
    struct lsh_string *plain = simple_decode_hex("84858687BFBCBDBA37363938FDFAFBF8") ;
    struct lsh_string *cipher = simple_decode_hex("5FAF8573E33B145B6A369CD3606AB2C9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 77 ... ", stderr);
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
    fputs("Decrypting with rijndael 77 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("AAABACADAFB0B1B2B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("828384857669686B909192930B08090E") ;
    struct lsh_string *cipher = simple_decode_hex("31587E9944AB1C16B844ECAD0DF2E7DA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 78 ... ", stderr);
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
    fputs("Decrypting with rijndael 78 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4") ;
    struct lsh_string *plain = simple_decode_hex("BEBFBCBD9695948B707176779E919093") ;
    struct lsh_string *cipher = simple_decode_hex("D017FECD91148ABA37F6F3068AA67D8A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 79 ... ", stderr);
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
    fputs("Decrypting with rijndael 79 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E6E7E8E9EBECEDEEF0F1F2F3F5F6F7F8FAFBFCFDFE010002") ;
    struct lsh_string *plain = simple_decode_hex("8B8A85846067666521202322D0D3D2DD") ;
    struct lsh_string *cipher = simple_decode_hex("788EF2F021A73CBA2794B616078A8500") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 80 ... ", stderr);
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
    fputs("Decrypting with rijndael 80 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("04050607090A0B0C0E0F10111314151618191A1B1D1E1F20") ;
    struct lsh_string *plain = simple_decode_hex("76777475F1F2F3F4F8F9E6E777707172") ;
    struct lsh_string *cipher = simple_decode_hex("5D1EF20DCED6BCBC12131AC7C54788AA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 81 ... ", stderr);
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
    fputs("Decrypting with rijndael 81 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("222324252728292A2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("A4A5A2A34F404142B4B5B6B727242522") ;
    struct lsh_string *cipher = simple_decode_hex("B3C8CF961FAF9EA05FDDE6D1E4D8F663") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 82 ... ", stderr);
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
    fputs("Decrypting with rijndael 82 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F50515254555657595A5B5C") ;
    struct lsh_string *plain = simple_decode_hex("94959697E1E2E3EC16171011839C9D9E") ;
    struct lsh_string *cipher = simple_decode_hex("143075C70605861C7FAC6526199E459F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 83 ... ", stderr);
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
    fputs("Decrypting with rijndael 83 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("5E5F60616364656668696A6B6D6E6F70727374757778797A") ;
    struct lsh_string *plain = simple_decode_hex("03023D3C06010003DEDFDCDDFFFCFDE2") ;
    struct lsh_string *cipher = simple_decode_hex("A5AE12EADE9A87268D898BFC8FC0252A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 84 ... ", stderr);
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
    fputs("Decrypting with rijndael 84 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("7C7D7E7F81828384868788898B8C8D8E9091929395969798") ;
    struct lsh_string *plain = simple_decode_hex("10111213F1F2F3F4CECFC0C1DBDCDDDE") ;
    struct lsh_string *cipher = simple_decode_hex("0924F7CF2E877A4819F5244A360DCEA9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 85 ... ", stderr);
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
    fputs("Decrypting with rijndael 85 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("9A9B9C9D9FA0A1A2A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("67666160724D4C4F1D1C1F1E73707176") ;
    struct lsh_string *cipher = simple_decode_hex("3D9E9635AFCC3E291CC7AB3F27D1C99A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 86 ... ", stderr);
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
    fputs("Decrypting with rijndael 86 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CACCCDCECFD1D2D3D4") ;
    struct lsh_string *plain = simple_decode_hex("E6E7E4E5A8ABAAD584858283909F9E9D") ;
    struct lsh_string *cipher = simple_decode_hex("9D80FEEBF87510E2B8FB98BB54FD788C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 87 ... ", stderr);
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
    fputs("Decrypting with rijndael 87 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D6D7D8D9DBDCDDDEE0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2") ;
    struct lsh_string *plain = simple_decode_hex("71707F7E565150537D7C7F7E6162636C") ;
    struct lsh_string *cipher = simple_decode_hex("5F9D1A082A1A37985F174002ECA01309") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 88 ... ", stderr);
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
    fputs("Decrypting with rijndael 88 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F4F5F6F7F9FAFBFCFEFE01010304050608090A0B0D0E0F10") ;
    struct lsh_string *plain = simple_decode_hex("64656667212223245555AAAA03040506") ;
    struct lsh_string *cipher = simple_decode_hex("A390EBB1D1403930184A44B4876646E4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 89 ... ", stderr);
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
    fputs("Decrypting with rijndael 89 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("121314151718191A1C1D1E1F21222324262728292B2C2D2E") ;
    struct lsh_string *plain = simple_decode_hex("9E9F9899ABA4A5A6CFCECDCC2B28292E") ;
    struct lsh_string *cipher = simple_decode_hex("700FE918981C3195BB6C4BCB46B74E29") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 90 ... ", stderr);
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
    fputs("Decrypting with rijndael 90 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("30313233353637383A3B3C3D3F40414244454647494A4B4C") ;
    struct lsh_string *plain = simple_decode_hex("C7C6C5C4D1D2D3DC626364653A454447") ;
    struct lsh_string *cipher = simple_decode_hex("907984406F7BF2D17FB1EB15B673D747") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 91 ... ", stderr);
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
    fputs("Decrypting with rijndael 91 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("4E4F50515354555658595A5B5D5E5F60626364656768696A") ;
    struct lsh_string *plain = simple_decode_hex("F6F7E8E9E0E7E6E51D1C1F1E5B585966") ;
    struct lsh_string *cipher = simple_decode_hex("C32A956DCFC875C2AC7C7CC8B8CC26E1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 92 ... ", stderr);
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
    fputs("Decrypting with rijndael 92 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("6C6D6E6F71727374767778797B7C7D7E8081828385868788") ;
    struct lsh_string *plain = simple_decode_hex("BCBDBEBF5D5E5F5868696667F4F3F2F1") ;
    struct lsh_string *cipher = simple_decode_hex("02646E2EBFA9B820CF8424E9B9B6EB51") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 93 ... ", stderr);
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
    fputs("Decrypting with rijndael 93 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("8A8B8C8D8F90919294959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("40414647B0AFAEAD9B9A99989B98999E") ;
    struct lsh_string *cipher = simple_decode_hex("621FDA3A5BBD54C6D3C685816BD4EAD8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 94 ... ", stderr);
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
    fputs("Decrypting with rijndael 94 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BABCBDBEBFC1C2C3C4") ;
    struct lsh_string *plain = simple_decode_hex("69686B6A0201001F0F0E0908B4BBBAB9") ;
    struct lsh_string *cipher = simple_decode_hex("D4E216040426DFAF18B152469BC5AC2F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 95 ... ", stderr);
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
    fputs("Decrypting with rijndael 95 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C6C7C8C9CBCCCDCED0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2") ;
    struct lsh_string *plain = simple_decode_hex("C7C6C9C8D8DFDEDD5A5B5859BEBDBCB3") ;
    struct lsh_string *cipher = simple_decode_hex("9D0635B9D33B6CDBD71F5D246EA17CC8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 96 ... ", stderr);
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
    fputs("Decrypting with rijndael 96 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6F8F9FAFBFDFEFE00") ;
    struct lsh_string *plain = simple_decode_hex("DEDFDCDD787B7A7DFFFEE1E0B2B5B4B7") ;
    struct lsh_string *cipher = simple_decode_hex("10ABAD1BD9BAE5448808765583A2CC1A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 97 ... ", stderr);
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
    fputs("Decrypting with rijndael 97 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("020304050708090A0C0D0E0F11121314161718191B1C1D1E") ;
    struct lsh_string *plain = simple_decode_hex("4D4C4B4A606F6E6DD0D1D2D3FBF8F9FE") ;
    struct lsh_string *cipher = simple_decode_hex("6891889E16544E355FF65A793C39C9A8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 98 ... ", stderr);
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
    fputs("Decrypting with rijndael 98 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("20212223252627282A2B2C2D2F30313234353637393A3B3C") ;
    struct lsh_string *plain = simple_decode_hex("B7B6B5B4D7D4D5DAE5E4E3E2E1FEFFFC") ;
    struct lsh_string *cipher = simple_decode_hex("CC735582E68072C163CD9DDF46B91279") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 99 ... ", stderr);
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
    fputs("Decrypting with rijndael 99 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("3E3F40414344454648494A4B4D4E4F50525354555758595A") ;
    struct lsh_string *plain = simple_decode_hex("CECFB0B1F7F0F1F2AEAFACAD3E3D3C23") ;
    struct lsh_string *cipher = simple_decode_hex("C5C68B9AEEB7F878DF578EFA562F9574") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 100 ... ", stderr);
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
    fputs("Decrypting with rijndael 100 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("5C5D5E5F61626364666768696B6C6D6E7071727375767778") ;
    struct lsh_string *plain = simple_decode_hex("CACBC8C9CDCECFC812131C1D494E4F4C") ;
    struct lsh_string *cipher = simple_decode_hex("5F4764395A667A47D73452955D0D2CE8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 101 ... ", stderr);
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
    fputs("Decrypting with rijndael 101 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("7A7B7C7D7F80818284858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("9D9C9B9AD22D2C2FB1B0B3B20C0F0E09") ;
    struct lsh_string *cipher = simple_decode_hex("701448331F66106CEFDDF1EB8267C357") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 102 ... ", stderr);
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
    fputs("Decrypting with rijndael 102 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AAACADAEAFB1B2B3B4") ;
    struct lsh_string *plain = simple_decode_hex("7A7B787964676659959493924F404142") ;
    struct lsh_string *cipher = simple_decode_hex("CB3EE56D2E14B4E1941666F13379D657") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 103 ... ", stderr);
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
    fputs("Decrypting with rijndael 103 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B6B7B8B9BBBCBDBEC0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2") ;
    struct lsh_string *plain = simple_decode_hex("AAABA4A5CEC9C8CB1F1E1D1CABA8A9A6") ;
    struct lsh_string *cipher = simple_decode_hex("9FE16EFD18AB6E1981191851FEDB0764") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 104 ... ", stderr);
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
    fputs("Decrypting with rijndael 104 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6E8E9EAEBEDEEEFF0") ;
    struct lsh_string *plain = simple_decode_hex("93929190282B2A2DC4C5FAFB92959497") ;
    struct lsh_string *cipher = simple_decode_hex("3DC9BA24E1B223589B147ADCEB4C8E48") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 105 ... ", stderr);
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
    fputs("Decrypting with rijndael 105 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F2F3F4F5F7F8F9FAFCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("EFEEE9E8DED1D0D339383B3A888B8A8D") ;
    struct lsh_string *cipher = simple_decode_hex("1C333032682E7D4DE5E5AFC05C3E483C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 106 ... ", stderr);
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
    fputs("Decrypting with rijndael 106 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F20212224252627292A2B2C") ;
    struct lsh_string *plain = simple_decode_hex("7F7E7D7CA2A1A0AF78797E7F112E2F2C") ;
    struct lsh_string *cipher = simple_decode_hex("D593CC99A95AFEF7E92038E05A59D00A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 107 ... ", stderr);
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
    fputs("Decrypting with rijndael 107 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("2E2F30313334353638393A3B3D3E3F40424344454748494A") ;
    struct lsh_string *plain = simple_decode_hex("84859A9B2B2C2D2E868784852625245B") ;
    struct lsh_string *cipher = simple_decode_hex("51E7F96F53B4353923452C222134E1EC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 108 ... ", stderr);
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
    fputs("Decrypting with rijndael 108 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("4C4D4E4F51525354565758595B5C5D5E6061626365666768") ;
    struct lsh_string *plain = simple_decode_hex("B0B1B2B3070405026869666710171615") ;
    struct lsh_string *cipher = simple_decode_hex("4075B357A1A2B473400C3B25F32F81A4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 109 ... ", stderr);
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
    fputs("Decrypting with rijndael 109 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("6A6B6C6D6F70717274757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("ACADAAABBDA2A3A00D0C0F0E595A5B5C") ;
    struct lsh_string *cipher = simple_decode_hex("302E341A3EBCD74F0D55F61714570284") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 110 ... ", stderr);
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
    fputs("Decrypting with rijndael 110 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A9C9D9E9FA1A2A3A4") ;
    struct lsh_string *plain = simple_decode_hex("121310115655544B5253545569666764") ;
    struct lsh_string *cipher = simple_decode_hex("57ABDD8231280DA01C5042B78CF76522") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 111 ... ", stderr);
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
    fputs("Decrypting with rijndael 111 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A6A7A8A9ABACADAEB0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2") ;
    struct lsh_string *plain = simple_decode_hex("DEDFD0D166616063EAEBE8E94142434C") ;
    struct lsh_string *cipher = simple_decode_hex("17F9EA7EEA17AC1ADF0E190FEF799E92") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 112 ... ", stderr);
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
    fputs("Decrypting with rijndael 112 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6D8D9DADBDDDEDFE0") ;
    struct lsh_string *plain = simple_decode_hex("DBDAD9D81417161166677879E0E7E6E5") ;
    struct lsh_string *cipher = simple_decode_hex("2E1BDD563DD87EE5C338DD6D098D0A7A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 113 ... ", stderr);
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
    fputs("Decrypting with rijndael 113 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E2E3E4E5E7E8E9EAECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("6A6B6C6DE0EFEEED2B2A2928C0C3C2C5") ;
    struct lsh_string *cipher = simple_decode_hex("EB869996E6F8BFB2BFDD9E0C4504DBB2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 114 ... ", stderr);
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
    fputs("Decrypting with rijndael 114 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("B1B0B3B21714151A1A1B1C1D5649484B") ;
    struct lsh_string *cipher = simple_decode_hex("C2E01549E9DECF317468B3E018C61BA8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 115 ... ", stderr);
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
    fputs("Decrypting with rijndael 115 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("39380706A3A4A5A6C4C5C6C77271706F") ;
    struct lsh_string *cipher = simple_decode_hex("8DA875D033C01DD463B244A1770F4A22") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 116 ... ", stderr);
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
    fputs("Decrypting with rijndael 116 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("5C5D5E5F1013121539383736E2E5E4E7") ;
    struct lsh_string *cipher = simple_decode_hex("8BA0DCF3A186844F026D022F8839D696") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 117 ... ", stderr);
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
    fputs("Decrypting with rijndael 117 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("5A5B5C5D5F60616264656667696A6B6C6E6F707173747576") ;
    struct lsh_string *plain = simple_decode_hex("43424544EAD5D4D72E2F2C2D64676661") ;
    struct lsh_string *cipher = simple_decode_hex("E9691FF9A6CC6970E51670A0FD5B88C1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 118 ... ", stderr);
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
    fputs("Decrypting with rijndael 118 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A8C8D8E8F91929394") ;
    struct lsh_string *plain = simple_decode_hex("55545756989B9A65F8F9FEFF18171615") ;
    struct lsh_string *cipher = simple_decode_hex("F2BAEC06FAEED30F88EE63BA081A6E5B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 119 ... ", stderr);
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
    fputs("Decrypting with rijndael 119 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("969798999B9C9D9EA0A1A2A3A5A6A7A8AAABACADAFB0B1B2") ;
    struct lsh_string *plain = simple_decode_hex("05040B0A525554573C3D3E3F4A494847") ;
    struct lsh_string *cipher = simple_decode_hex("9C39D4C459AE5753394D6094ADC21E78") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 120 ... ", stderr);
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
    fputs("Decrypting with rijndael 120 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6C8C9CACBCDCECFD0") ;
    struct lsh_string *plain = simple_decode_hex("14151617595A5B5C8584FBFA8E89888B") ;
    struct lsh_string *cipher = simple_decode_hex("6345B532A11904502EA43BA99C6BD2B2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 121 ... ", stderr);
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
    fputs("Decrypting with rijndael 121 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("7C7D7A7BFDF2F3F029282B2A51525354") ;
    struct lsh_string *cipher = simple_decode_hex("5FFAE3061A95172E4070CEDCE1E428C8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 122 ... ", stderr);
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
    fputs("Decrypting with rijndael 122 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE01000204050607090A0B0C") ;
    struct lsh_string *plain = simple_decode_hex("38393A3B1E1D1C1341404746C23D3C3E") ;
    struct lsh_string *cipher = simple_decode_hex("0A4566BE4CDF9ADCE5DEC865B5AB34CD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 123 ... ", stderr);
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
    fputs("Decrypting with rijndael 123 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("0E0F10111314151618191A1B1D1E1F20222324252728292A") ;
    struct lsh_string *plain = simple_decode_hex("8D8C939240474645818083827C7F7E41") ;
    struct lsh_string *cipher = simple_decode_hex("CA17FCCE79B7404F2559B22928F126FB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 124 ... ", stderr);
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
    fputs("Decrypting with rijndael 124 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("2C2D2E2F31323334363738393B3C3D3E4041424345464748") ;
    struct lsh_string *plain = simple_decode_hex("3B3A39381A19181F32333C3D45424340") ;
    struct lsh_string *cipher = simple_decode_hex("97CA39B849ED73A6470A97C821D82F58") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 125 ... ", stderr);
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
    fputs("Decrypting with rijndael 125 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("4A4B4C4D4F50515254555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("F0F1F6F738272625828380817F7C7D7A") ;
    struct lsh_string *cipher = simple_decode_hex("8198CB06BC684C6D3E9B7989428DCF7A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 126 ... ", stderr);
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
    fputs("Decrypting with rijndael 126 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A7C7D7E7F81828384") ;
    struct lsh_string *plain = simple_decode_hex("89888B8A0407061966676061141B1A19") ;
    struct lsh_string *cipher = simple_decode_hex("F53C464C705EE0F28D9A4C59374928BD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 127 ... ", stderr);
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
    fputs("Decrypting with rijndael 127 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("868788898B8C8D8E90919293959697989A9B9C9D9FA0A1A2") ;
    struct lsh_string *plain = simple_decode_hex("D3D2DDDCAAADACAF9C9D9E9FE8EBEAE5") ;
    struct lsh_string *cipher = simple_decode_hex("9ADB3D4CCA559BB98C3E2ED73DBF1154") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 128 ... ", stderr);
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
    fputs("Decrypting with rijndael 128 ... ", stderr);
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
    fputs("Encrypting with rijndael 1 ... ", stderr);
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
    fputs("Decrypting with rijndael 1 ... ", stderr);
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
    fputs("Encrypting with rijndael 2 ... ", stderr);
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
    fputs("Decrypting with rijndael 2 ... ", stderr);
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
    fputs("Encrypting with rijndael 3 ... ", stderr);
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
    fputs("Decrypting with rijndael 3 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("4264B2696498DE4DF79788A9F83E9390") ;
    struct lsh_string *cipher = simple_decode_hex("2A5B56A596680FCC0E05F5E0F151ECAE") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 4 ... ", stderr);
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
    fputs("Decrypting with rijndael 4 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("EE9932B3721804D5A83EF5949245B6F6") ;
    struct lsh_string *cipher = simple_decode_hex("F5D6FF414FD2C6181494D20C37F2B8C4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 5 ... ", stderr);
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
    fputs("Decrypting with rijndael 5 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("E6248F55C5FDCBCA9CBBB01C88A2EA77") ;
    struct lsh_string *cipher = simple_decode_hex("85399C01F59FFFB5204F19F8482F00B8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 6 ... ", stderr);
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
    fputs("Decrypting with rijndael 6 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE01000204050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("B8358E41B9DFF65FD461D55A99266247") ;
    struct lsh_string *cipher = simple_decode_hex("92097B4C88A041DDF98144BC8D22E8E7") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 7 ... ", stderr);
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
    fputs("Decrypting with rijndael 7 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("18191A1B1D1E1F20222324252728292A2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("F0E2D72260AF58E21E015AB3A4C0D906") ;
    struct lsh_string *cipher = simple_decode_hex("89BD5B73B356AB412AEF9F76CEA2D65C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 8 ... ", stderr);
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
    fputs("Decrypting with rijndael 8 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F50515254555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("475B8B823CE8893DB3C44A9F2A379FF7") ;
    struct lsh_string *cipher = simple_decode_hex("2536969093C55FF9454692F2FAC2F530") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 9 ... ", stderr);
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
    fputs("Decrypting with rijndael 9 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("688F5281945812862F5F3076CF80412F") ;
    struct lsh_string *cipher = simple_decode_hex("07FC76A872843F3F6E0081EE9396D637") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 10 ... ", stderr);
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
    fputs("Decrypting with rijndael 10 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("90919293959697989A9B9C9D9FA0A1A2A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("08D1D2BC750AF553365D35E75AFACEAA") ;
    struct lsh_string *cipher = simple_decode_hex("E38BA8EC2AA741358DCC93E8F141C491") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 11 ... ", stderr);
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
    fputs("Decrypting with rijndael 11 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CACCCDCECFD1D2D3D4D6D7D8D9DBDCDDDE") ;
    struct lsh_string *plain = simple_decode_hex("8707121F47CC3EFCECA5F9A8474950A1") ;
    struct lsh_string *cipher = simple_decode_hex("D028EE23E4A89075D0B03E868D7D3A42") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 12 ... ", stderr);
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
    fputs("Decrypting with rijndael 12 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2F4F5F6F7F9FAFBFCFEFE010103040506") ;
    struct lsh_string *plain = simple_decode_hex("E51AA0B135DBA566939C3B6359A980C5") ;
    struct lsh_string *cipher = simple_decode_hex("8CD9423DFC459E547155C5D1D522E540") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 13 ... ", stderr);
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
    fputs("Decrypting with rijndael 13 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A1C1D1E1F21222324262728292B2C2D2E") ;
    struct lsh_string *plain = simple_decode_hex("069A007FC76A459F98BAF917FEDF9521") ;
    struct lsh_string *cipher = simple_decode_hex("080E9517EB1677719ACF728086040AE3") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 14 ... ", stderr);
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
    fputs("Decrypting with rijndael 14 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("30313233353637383A3B3C3D3F40414244454647494A4B4C4E4F505153545556") ;
    struct lsh_string *plain = simple_decode_hex("726165C1723FBCF6C026D7D00B091027") ;
    struct lsh_string *cipher = simple_decode_hex("7C1700211A3991FC0ECDED0AB3E576B0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 15 ... ", stderr);
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
    fputs("Decrypting with rijndael 15 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("58595A5B5D5E5F60626364656768696A6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("D7C544DE91D55CFCDE1F84CA382200CE") ;
    struct lsh_string *cipher = simple_decode_hex("DABCBCC855839251DB51E224FBE87435") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 16 ... ", stderr);
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
    fputs("Decrypting with rijndael 16 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F90919294959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("FED3C9A161B9B5B2BD611B41DC9DA357") ;
    struct lsh_string *cipher = simple_decode_hex("68D56FAD0406947A4DD27A7448C10F1D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 17 ... ", stderr);
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
    fputs("Decrypting with rijndael 17 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BABCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCE") ;
    struct lsh_string *plain = simple_decode_hex("4F634CDC6551043409F30B635832CF82") ;
    struct lsh_string *cipher = simple_decode_hex("DA9A11479844D1FFEE24BBF3719A9925") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 18 ... ", stderr);
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
    fputs("Decrypting with rijndael 18 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("109CE98DB0DFB36734D9F3394711B4E6") ;
    struct lsh_string *cipher = simple_decode_hex("5E4BA572F8D23E738DA9B05BA24B8D81") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 19 ... ", stderr);
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
    fputs("Decrypting with rijndael 19 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F80818284858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("4EA6DFABA2D8A02FFDFFA89835987242") ;
    struct lsh_string *cipher = simple_decode_hex("A115A2065D667E3F0B883837A6E903F8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 20 ... ", stderr);
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
    fputs("Decrypting with rijndael 20 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AAACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("5AE094F54AF58E6E3CDBF976DAC6D9EF") ;
    struct lsh_string *cipher = simple_decode_hex("3E9E90DC33EAC2437D86AD30B137E66E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 21 ... ", stderr);
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
    fputs("Decrypting with rijndael 21 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("764D8E8E0F29926DBE5122E66354FDBE") ;
    struct lsh_string *cipher = simple_decode_hex("01CE82D8FBCDAE824CB3C48E495C3692") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 22 ... ", stderr);
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
    fputs("Decrypting with rijndael 22 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FAFCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("3F0418F888CDF29A982BF6B75410D6A9") ;
    struct lsh_string *cipher = simple_decode_hex("0C9CFF163CE936FAAF083CFD3DEA3117") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 23 ... ", stderr);
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
    fputs("Decrypting with rijndael 23 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F20212224252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("E4A3E7CB12CDD56AA4A75197A9530220") ;
    struct lsh_string *cipher = simple_decode_hex("5131BA9BD48F2BBA85560680DF504B52") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 24 ... ", stderr);
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
    fputs("Decrypting with rijndael 24 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("211677684AAC1EC1A160F44C4EBF3F26") ;
    struct lsh_string *cipher = simple_decode_hex("9DC503BBF09823AEC8A977A5AD26CCB2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 25 ... ", stderr);
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
    fputs("Decrypting with rijndael 25 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F70717274757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("D21E439FF749AC8F18D6D4B105E03895") ;
    struct lsh_string *cipher = simple_decode_hex("9A6DB0C0862E506A9E397225884041D7") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 26 ... ", stderr);
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
    fputs("Decrypting with rijndael 26 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("D9F6FF44646C4725BD4C0103FF5552A7") ;
    struct lsh_string *cipher = simple_decode_hex("430BF9570804185E1AB6365FC6A6860C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 27 ... ", stderr);
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
    fputs("Decrypting with rijndael 27 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("0B1256C2A00B976250CFC5B0C37ED382") ;
    struct lsh_string *cipher = simple_decode_hex("3525EBC02F4886E6A5A3762813E8CE8A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 28 ... ", stderr);
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
    fputs("Decrypting with rijndael 28 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EAECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("B056447FFC6DC4523A36CC2E972A3A79") ;
    struct lsh_string *cipher = simple_decode_hex("07FA265C763779CCE224C7BAD671027B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 29 ... ", stderr);
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
    fputs("Decrypting with rijndael 29 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("5E25CA78F0DE55802524D38DA3FE4456") ;
    struct lsh_string *cipher = simple_decode_hex("E8B72B4E8BE243438C9FFF1F0E205872") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 30 ... ", stderr);
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
    fputs("Decrypting with rijndael 30 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("A5BCF4728FA5EAAD8567C0DC24675F83") ;
    struct lsh_string *cipher = simple_decode_hex("109D4F999A0E11ACE1F05E6B22CBCB50") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 31 ... ", stderr);
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
    fputs("Decrypting with rijndael 31 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("814E59F97ED84646B78B2CA022E9CA43") ;
    struct lsh_string *cipher = simple_decode_hex("45A5E8D4C3ED58403FF08D68A0CC4029") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 32 ... ", stderr);
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
    fputs("Decrypting with rijndael 32 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("15478BEEC58F4775C7A7F5D4395514D7") ;
    struct lsh_string *cipher = simple_decode_hex("196865964DB3D417B6BD4D586BCB7634") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 33 ... ", stderr);
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
    fputs("Decrypting with rijndael 33 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("253548FFCA461C67C8CBC78CD59F4756") ;
    struct lsh_string *cipher = simple_decode_hex("60436AD45AC7D30D99195F815D98D2AE") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 34 ... ", stderr);
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
    fputs("Decrypting with rijndael 34 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("FD7AD8D73B9B0F8CC41600640F503D65") ;
    struct lsh_string *cipher = simple_decode_hex("BB07A23F0B61014B197620C185E2CD75") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 35 ... ", stderr);
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
    fputs("Decrypting with rijndael 35 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE01000204050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("06199DE52C6CBF8AF954CD65830BCD56") ;
    struct lsh_string *cipher = simple_decode_hex("5BC0B2850129C854423AFF0751FE343B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 36 ... ", stderr);
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
    fputs("Decrypting with rijndael 36 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("18191A1B1D1E1F20222324252728292A2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("F17C4FFE48E44C61BD891E257E725794") ;
    struct lsh_string *cipher = simple_decode_hex("7541A78F96738E6417D2A24BD2BECA40") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 37 ... ", stderr);
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
    fputs("Decrypting with rijndael 37 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F50515254555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("9A5B4A402A3E8A59BE6BF5CD8154F029") ;
    struct lsh_string *cipher = simple_decode_hex("B0A303054412882E464591F1546C5B9E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 38 ... ", stderr);
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
    fputs("Decrypting with rijndael 38 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("79BD40B91A7E07DC939D441782AE6B17") ;
    struct lsh_string *cipher = simple_decode_hex("778C06D8A355EEEE214FCEA14B4E0EEF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 39 ... ", stderr);
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
    fputs("Decrypting with rijndael 39 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("90919293959697989A9B9C9D9FA0A1A2A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("D8CEAAF8976E5FBE1012D8C84F323799") ;
    struct lsh_string *cipher = simple_decode_hex("09614206D15CBACE63227D06DB6BEEBB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 40 ... ", stderr);
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
    fputs("Decrypting with rijndael 40 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CACCCDCECFD1D2D3D4D6D7D8D9DBDCDDDE") ;
    struct lsh_string *plain = simple_decode_hex("3316E2751E2E388B083DA23DD6AC3FBE") ;
    struct lsh_string *cipher = simple_decode_hex("41B97FB20E427A9FDBBB358D9262255D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 41 ... ", stderr);
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
    fputs("Decrypting with rijndael 41 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2F4F5F6F7F9FAFBFCFEFE010103040506") ;
    struct lsh_string *plain = simple_decode_hex("8B7CFBE37DE7DCA793521819242C5816") ;
    struct lsh_string *cipher = simple_decode_hex("C1940F703D845F957652C2D64ABD7ADF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 42 ... ", stderr);
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
    fputs("Decrypting with rijndael 42 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A1C1D1E1F21222324262728292B2C2D2E") ;
    struct lsh_string *plain = simple_decode_hex("F23F033C0EEBF8EC55752662FD58CE68") ;
    struct lsh_string *cipher = simple_decode_hex("D2D44FCDAE5332343366DB297EFCF21B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 43 ... ", stderr);
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
    fputs("Decrypting with rijndael 43 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("30313233353637383A3B3C3D3F40414244454647494A4B4C4E4F505153545556") ;
    struct lsh_string *plain = simple_decode_hex("59EB34F6C8BDBACC5FC6AD73A59A1301") ;
    struct lsh_string *cipher = simple_decode_hex("EA8196B79DBE167B6AA9896E287EED2B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 44 ... ", stderr);
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
    fputs("Decrypting with rijndael 44 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("58595A5B5D5E5F60626364656768696A6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("DCDE8B6BD5CF7CC22D9505E3CE81261A") ;
    struct lsh_string *cipher = simple_decode_hex("D6B0B0C4BA6C7DBE5ED467A1E3F06C2D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 45 ... ", stderr);
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
    fputs("Decrypting with rijndael 45 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F90919294959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("E33CF7E524FED781E7042FF9F4B35DC7") ;
    struct lsh_string *cipher = simple_decode_hex("EC51EB295250C22C2FB01816FB72BCAE") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 46 ... ", stderr);
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
    fputs("Decrypting with rijndael 46 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BABCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCE") ;
    struct lsh_string *plain = simple_decode_hex("27963C8FACDF73062867D164DF6D064C") ;
    struct lsh_string *cipher = simple_decode_hex("ADED6630A07CE9C7408A155D3BD0D36F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 47 ... ", stderr);
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
    fputs("Decrypting with rijndael 47 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("77B1CE386B551B995F2F2A1DA994EEF8") ;
    struct lsh_string *cipher = simple_decode_hex("697C9245B9937F32F5D1C82319F0363A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 48 ... ", stderr);
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
    fputs("Decrypting with rijndael 48 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F8F9FAFBFDFEFE00020304050708090A0C0D0E0F11121314161718191B1C1D1E") ;
    struct lsh_string *plain = simple_decode_hex("F083388B013679EFCF0BB9B15D52AE5C") ;
    struct lsh_string *cipher = simple_decode_hex("AAD5AD50C6262AAEC30541A1B7B5B19C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 49 ... ", stderr);
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
    fputs("Decrypting with rijndael 49 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("20212223252627282A2B2C2D2F30313234353637393A3B3C3E3F404143444546") ;
    struct lsh_string *plain = simple_decode_hex("C5009E0DAB55DB0ABDB636F2600290C8") ;
    struct lsh_string *cipher = simple_decode_hex("7D34B893855341EC625BD6875AC18C0D") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 50 ... ", stderr);
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
    fputs("Decrypting with rijndael 50 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("48494A4B4D4E4F50525354555758595A5C5D5E5F61626364666768696B6C6D6E") ;
    struct lsh_string *plain = simple_decode_hex("7804881E26CD532D8514D3683F00F1B9") ;
    struct lsh_string *cipher = simple_decode_hex("7EF05105440F83862F5D780E88F02B41") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 51 ... ", stderr);
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
    fputs("Decrypting with rijndael 51 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F80818284858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("46CDDCD73D1EB53E675CA012870A92A3") ;
    struct lsh_string *cipher = simple_decode_hex("C377C06403382061AF2C9C93A8E70DF6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 52 ... ", stderr);
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
    fputs("Decrypting with rijndael 52 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AAACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("A9FB44062BB07FE130A8E8299EACB1AB") ;
    struct lsh_string *cipher = simple_decode_hex("1DBDB3FFDC052DACC83318853ABC6DE5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 53 ... ", stderr);
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
    fputs("Decrypting with rijndael 53 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("2B6FF8D7A5CC3A28A22D5A6F221AF26B") ;
    struct lsh_string *cipher = simple_decode_hex("69A6EAB00432517D0BF483C91C0963C7") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 54 ... ", stderr);
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
    fputs("Decrypting with rijndael 54 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FAFCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("1A9527C29B8ADD4B0E3E656DBB2AF8B4") ;
    struct lsh_string *cipher = simple_decode_hex("0797F41DC217C80446E1D514BD6AB197") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 55 ... ", stderr);
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
    fputs("Decrypting with rijndael 55 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F20212224252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("7F99CF2C75244DF015EB4B0C1050AEAE") ;
    struct lsh_string *cipher = simple_decode_hex("9DFD76575902A637C01343C58E011A03") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 56 ... ", stderr);
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
    fputs("Decrypting with rijndael 56 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("E84FF85B0D9454071909C1381646C4ED") ;
    struct lsh_string *cipher = simple_decode_hex("ACF4328AE78F34B9FA9B459747CC2658") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 57 ... ", stderr);
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
    fputs("Decrypting with rijndael 57 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F70717274757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("89AFD40F99521280D5399B12404F6DB4") ;
    struct lsh_string *cipher = simple_decode_hex("B0479AEA12BAC4FE2384CF98995150C6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 58 ... ", stderr);
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
    fputs("Decrypting with rijndael 58 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("A09EF32DBC5119A35AB7FA38656F0329") ;
    struct lsh_string *cipher = simple_decode_hex("9DD52789EFE3FFB99F33B3DA5030109A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 59 ... ", stderr);
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
    fputs("Decrypting with rijndael 59 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("61773457F068C376C7829B93E696E716") ;
    struct lsh_string *cipher = simple_decode_hex("ABBB755E4621EF8F1214C19F649FB9FD") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 60 ... ", stderr);
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
    fputs("Decrypting with rijndael 60 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EAECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("A34F0CAE726CCE41DD498747D891B967") ;
    struct lsh_string *cipher = simple_decode_hex("DA27FB8174357BCE2BED0E7354F380F9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 61 ... ", stderr);
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
    fputs("Decrypting with rijndael 61 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("856F59496C7388EE2D2B1A27B7697847") ;
    struct lsh_string *cipher = simple_decode_hex("C59A0663F0993838F6E5856593BDC5EF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 62 ... ", stderr);
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
    fputs("Decrypting with rijndael 62 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("CB090C593EF7720BD95908FB93B49DF4") ;
    struct lsh_string *cipher = simple_decode_hex("ED60B264B5213E831607A99C0CE5E57E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 63 ... ", stderr);
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
    fputs("Decrypting with rijndael 63 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("A0AC75CD2F1923D460FC4D457AD95BAF") ;
    struct lsh_string *cipher = simple_decode_hex("E50548746846F3EB77B8C520640884ED") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 64 ... ", stderr);
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
    fputs("Decrypting with rijndael 64 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("2A2B282974777689E8E9EEEF525D5C5F") ;
    struct lsh_string *cipher = simple_decode_hex("28282CC7D21D6A2923641E52D188EF0C") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 65 ... ", stderr);
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
    fputs("Decrypting with rijndael 65 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("909192939390919E0F0E09089788898A") ;
    struct lsh_string *cipher = simple_decode_hex("0DFA5B02ABB18E5A815305216D6D4F8E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 66 ... ", stderr);
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
    fputs("Decrypting with rijndael 66 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("777675748D8E8F907170777649464744") ;
    struct lsh_string *cipher = simple_decode_hex("7359635C0EECEFE31D673395FB46FB99") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 67 ... ", stderr);
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
    fputs("Decrypting with rijndael 67 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE01000204050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("717073720605040B2D2C2B2A05FAFBF9") ;
    struct lsh_string *cipher = simple_decode_hex("73C679F7D5AEF2745C9737BB4C47FB36") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 68 ... ", stderr);
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
    fputs("Decrypting with rijndael 68 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("18191A1B1D1E1F20222324252728292A2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("64656667FEFDFCC31B1A1D1CA5AAABA8") ;
    struct lsh_string *cipher = simple_decode_hex("B192BD472A4D2EAFB786E97458967626") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 69 ... ", stderr);
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
    fputs("Decrypting with rijndael 69 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F50515254555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("DBDAD9D86A696867B5B4B3B2C8D7D6D5") ;
    struct lsh_string *cipher = simple_decode_hex("0EC327F6C8A2B147598CA3FDE61DC6A4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 70 ... ", stderr);
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
    fputs("Decrypting with rijndael 70 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("5C5D5E5FE3E0E1FE31303736333C3D3E") ;
    struct lsh_string *cipher = simple_decode_hex("FC418EB3C41B859B38D4B6F646629729") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 71 ... ", stderr);
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
    fputs("Decrypting with rijndael 71 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("90919293959697989A9B9C9D9FA0A1A2A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("545556574B48494673727574546B6A69") ;
    struct lsh_string *cipher = simple_decode_hex("30249E5AC282B1C981EA64B609F3A154") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 72 ... ", stderr);
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
    fputs("Decrypting with rijndael 72 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CACCCDCECFD1D2D3D4D6D7D8D9DBDCDDDE") ;
    struct lsh_string *plain = simple_decode_hex("ECEDEEEFC6C5C4BB56575051F5FAFBF8") ;
    struct lsh_string *cipher = simple_decode_hex("5E6E08646D12150776BB43C2D78A9703") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 73 ... ", stderr);
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
    fputs("Decrypting with rijndael 73 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2F4F5F6F7F9FAFBFCFEFE010103040506") ;
    struct lsh_string *plain = simple_decode_hex("464744452724252AC9C8CFCED2CDCCCF") ;
    struct lsh_string *cipher = simple_decode_hex("FAEB3D5DE652CD3447DCEB343F30394A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 74 ... ", stderr);
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
    fputs("Decrypting with rijndael 74 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A1C1D1E1F21222324262728292B2C2D2E") ;
    struct lsh_string *plain = simple_decode_hex("E6E7E4E54142435C878681801C131211") ;
    struct lsh_string *cipher = simple_decode_hex("A8E88706823F6993EF80D05C1C7B2CF0") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 75 ... ", stderr);
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
    fputs("Decrypting with rijndael 75 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("30313233353637383A3B3C3D3F40414244454647494A4B4C4E4F505153545556") ;
    struct lsh_string *plain = simple_decode_hex("72737071CFCCCDC2F9F8FFFE710E0F0C") ;
    struct lsh_string *cipher = simple_decode_hex("8CED86677E6E00A1A1B15968F2D3CCE6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 76 ... ", stderr);
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
    fputs("Decrypting with rijndael 76 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("58595A5B5D5E5F60626364656768696A6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("505152537370714EC3C2C5C4010E0F0C") ;
    struct lsh_string *cipher = simple_decode_hex("9FC7C23858BE03BDEBB84E90DB6786A9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 77 ... ", stderr);
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
    fputs("Decrypting with rijndael 77 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F90919294959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("A8A9AAAB5C5F5E51AEAFA8A93D222320") ;
    struct lsh_string *cipher = simple_decode_hex("B4FBD65B33F70D8CF7F1111AC4649C36") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 78 ... ", stderr);
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
    fputs("Decrypting with rijndael 78 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BABCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCE") ;
    struct lsh_string *plain = simple_decode_hex("DEDFDCDDF6F5F4EB10111617FEF1F0F3") ;
    struct lsh_string *cipher = simple_decode_hex("C5C32D5ED03C4B53CC8C1BD0EF0DBBF6") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 79 ... ", stderr);
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
    fputs("Decrypting with rijndael 79 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("BDBCBFBE5E5D5C530B0A0D0CFAC5C4C7") ;
    struct lsh_string *cipher = simple_decode_hex("D1A7F03B773E5C212464B63709C6A891") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 80 ... ", stderr);
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
    fputs("Decrypting with rijndael 80 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F8F9FAFBFDFEFE00020304050708090A0C0D0E0F11121314161718191B1C1D1E") ;
    struct lsh_string *plain = simple_decode_hex("8A8B8889050606F8F4F5F2F3636C6D6E") ;
    struct lsh_string *cipher = simple_decode_hex("6B7161D8745947AC6950438EA138D028") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 81 ... ", stderr);
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
    fputs("Decrypting with rijndael 81 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("20212223252627282A2B2C2D2F30313234353637393A3B3C3E3F404143444546") ;
    struct lsh_string *plain = simple_decode_hex("A6A7A4A54D4E4F40B2B3B4B539262724") ;
    struct lsh_string *cipher = simple_decode_hex("FD47A9F7E366EE7A09BC508B00460661") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 82 ... ", stderr);
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
    fputs("Decrypting with rijndael 82 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("48494A4B4D4E4F50525354555758595A5C5D5E5F61626364666768696B6C6D6E") ;
    struct lsh_string *plain = simple_decode_hex("9C9D9E9FE9EAEBF40E0F08099B949596") ;
    struct lsh_string *cipher = simple_decode_hex("00D40B003DC3A0D9310B659B98C7E416") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 83 ... ", stderr);
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
    fputs("Decrypting with rijndael 83 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F80818284858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("2D2C2F2E1013121DCCCDCACBED121310") ;
    struct lsh_string *cipher = simple_decode_hex("EEA4C79DCC8E2BDA691F20AC48BE0717") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 84 ... ", stderr);
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
    fputs("Decrypting with rijndael 84 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AAACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("F4F5F6F7EDEEEFD0EAEBECEDF7F8F9FA") ;
    struct lsh_string *cipher = simple_decode_hex("E78F43B11C204403E5751F89D05A2509") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 85 ... ", stderr);
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
    fputs("Decrypting with rijndael 85 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("3D3C3F3E282B2A2573727574150A0B08") ;
    struct lsh_string *cipher = simple_decode_hex("D0F0E3D1F1244BB979931E38DD1786EF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 86 ... ", stderr);
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
    fputs("Decrypting with rijndael 86 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FAFCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("B6B7B4B5F8FBFAE5B4B5B2B3A0AFAEAD") ;
    struct lsh_string *cipher = simple_decode_hex("042E639DC4E1E4DDE7B75B749EA6F765") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 87 ... ", stderr);
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
    fputs("Decrypting with rijndael 87 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F20212224252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("B7B6B5B4989B9A95878681809BA4A5A6") ;
    struct lsh_string *cipher = simple_decode_hex("BC032FDD0EFE29503A980A7D07AB46A8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 88 ... ", stderr);
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
    fputs("Decrypting with rijndael 88 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("A8A9AAABE5E6E798E9E8EFEE4748494A") ;
    struct lsh_string *cipher = simple_decode_hex("0C93AC949C0DA6446EFFB86183B6C910") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 89 ... ", stderr);
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
    fputs("Decrypting with rijndael 89 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F70717274757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("ECEDEEEFD9DADBD4B9B8BFBE657A7B78") ;
    struct lsh_string *cipher = simple_decode_hex("E0D343E14DA75C917B4A5CEC4810D7C2") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 90 ... ", stderr);
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
    fputs("Decrypting with rijndael 90 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("7F7E7D7C696A6B74CACBCCCD929D9C9F") ;
    struct lsh_string *cipher = simple_decode_hex("0EAFB821748408279B937B626792E619") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 91 ... ", stderr);
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
    fputs("Decrypting with rijndael 91 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("08090A0B0605040BFFFEF9F8B9C6C7C4") ;
    struct lsh_string *cipher = simple_decode_hex("FA1AC6E02D23B106A1FEF18B274A553F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 92 ... ", stderr);
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
    fputs("Decrypting with rijndael 92 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EAECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("08090A0BF1F2F3CCFCFDFAFB68676665") ;
    struct lsh_string *cipher = simple_decode_hex("0DADFE019CD12368075507DF33C1A1E9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 93 ... ", stderr);
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
    fputs("Decrypting with rijndael 93 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("CACBC8C93A393837050403020D121310") ;
    struct lsh_string *cipher = simple_decode_hex("3A0879B414465D9FFBAF86B33A63A1B9") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 94 ... ", stderr);
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
    fputs("Decrypting with rijndael 94 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("E9E8EBEA8281809F8F8E8988343B3A39") ;
    struct lsh_string *cipher = simple_decode_hex("62199FADC76D0BE1805D3BA0B7D914BF") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 95 ... ", stderr);
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
    fputs("Decrypting with rijndael 95 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("515053524645444BD0D1D6D7340B0A09") ;
    struct lsh_string *cipher = simple_decode_hex("1B06D6C5D333E742730130CF78E719B4") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 96 ... ", stderr);
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
    fputs("Decrypting with rijndael 96 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("78797A7B7D7E7F80828384858788898A8C8D8E8F91929394969798999B9C9D9E") ;
    struct lsh_string *plain = simple_decode_hex("42434041ECEFEE1193929594C6C9C8CB") ;
    struct lsh_string *cipher = simple_decode_hex("F1F848824C32E9DCDCBF21580F069329") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 97 ... ", stderr);
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
    fputs("Decrypting with rijndael 97 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A0A1A2A3A5A6A7A8AAABACADAFB0B1B2B4B5B6B7B9BABBBCBEBFC0C1C3C4C5C6") ;
    struct lsh_string *plain = simple_decode_hex("EFEEEDECC2C1C0CF76777071455A5B58") ;
    struct lsh_string *cipher = simple_decode_hex("1A09050CBD684F784D8E965E0782F28A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 98 ... ", stderr);
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
    fputs("Decrypting with rijndael 98 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C8C9CACBCDCECFD0D2D3D4D5D7D8D9DADCDDDEDFE1E2E3E4E6E7E8E9EBECEDEE") ;
    struct lsh_string *plain = simple_decode_hex("5F5E5D5C3F3C3D221D1C1B1A19161714") ;
    struct lsh_string *cipher = simple_decode_hex("79C2969E7DED2BA7D088F3F320692360") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 99 ... ", stderr);
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
    fputs("Decrypting with rijndael 99 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F0F1F2F3F5F6F7F8FAFBFCFDFE01000204050607090A0B0C0E0F101113141516") ;
    struct lsh_string *plain = simple_decode_hex("000102034142434C1C1D1A1B8D727371") ;
    struct lsh_string *cipher = simple_decode_hex("091A658A2F7444C16ACCB669450C7B63") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 100 ... ", stderr);
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
    fputs("Decrypting with rijndael 100 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("18191A1B1D1E1F20222324252728292A2C2D2E2F31323334363738393B3C3D3E") ;
    struct lsh_string *plain = simple_decode_hex("8E8F8C8DB1B2B38C56575051050A0B08") ;
    struct lsh_string *cipher = simple_decode_hex("97C1E3A72CCA65FA977D5ED0E8A7BBFC") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 101 ... ", stderr);
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
    fputs("Decrypting with rijndael 101 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("40414243454647484A4B4C4D4F50515254555657595A5B5C5E5F606163646566") ;
    struct lsh_string *plain = simple_decode_hex("A7A6A5A4E8EBEAE57F7E7978CAD5D4D7") ;
    struct lsh_string *cipher = simple_decode_hex("70C430C6DB9A17828937305A2DF91A2A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 102 ... ", stderr);
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
    fputs("Decrypting with rijndael 102 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("68696A6B6D6E6F70727374757778797A7C7D7E7F81828384868788898B8C8D8E") ;
    struct lsh_string *plain = simple_decode_hex("8A8B888994979689454443429F909192") ;
    struct lsh_string *cipher = simple_decode_hex("629553457FBE2479098571C7C903FDE8") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 103 ... ", stderr);
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
    fputs("Decrypting with rijndael 103 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("90919293959697989A9B9C9D9FA0A1A2A4A5A6A7A9AAABACAEAFB0B1B3B4B5B6") ;
    struct lsh_string *plain = simple_decode_hex("8C8D8E8FE0E3E2ED45444342F1CECFCC") ;
    struct lsh_string *cipher = simple_decode_hex("A25B25A61F612669E7D91265C7D476BA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 104 ... ", stderr);
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
    fputs("Decrypting with rijndael 104 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B8B9BABBBDBEBFC0C2C3C4C5C7C8C9CACCCDCECFD1D2D3D4D6D7D8D9DBDCDDDE") ;
    struct lsh_string *plain = simple_decode_hex("FFFEFDFC4C4F4E31D8D9DEDFB6B9B8BB") ;
    struct lsh_string *cipher = simple_decode_hex("EB7E4E49B8AE0F024570DDA293254FED") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 105 ... ", stderr);
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
    fputs("Decrypting with rijndael 105 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E0E1E2E3E5E6E7E8EAEBECEDEFF0F1F2F4F5F6F7F9FAFBFCFEFE010103040506") ;
    struct lsh_string *plain = simple_decode_hex("FDFCFFFECCCFCEC12F2E29286679787B") ;
    struct lsh_string *cipher = simple_decode_hex("38FE15D61CCA84516E924ADCE5014F67") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 106 ... ", stderr);
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
    fputs("Decrypting with rijndael 106 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("08090A0B0D0E0F10121314151718191A1C1D1E1F21222324262728292B2C2D2E") ;
    struct lsh_string *plain = simple_decode_hex("67666564BAB9B8A77071767719161714") ;
    struct lsh_string *cipher = simple_decode_hex("3AD208492249108C9F3EBEB167AD0583") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 107 ... ", stderr);
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
    fputs("Decrypting with rijndael 107 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("30313233353637383A3B3C3D3F40414244454647494A4B4C4E4F505153545556") ;
    struct lsh_string *plain = simple_decode_hex("9A9B98992D2E2F2084858283245B5A59") ;
    struct lsh_string *cipher = simple_decode_hex("299BA9F9BF5AB05C3580FC26EDD1ED12") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 108 ... ", stderr);
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
    fputs("Decrypting with rijndael 108 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("58595A5B5D5E5F60626364656768696A6C6D6E6F71727374767778797B7C7D7E") ;
    struct lsh_string *plain = simple_decode_hex("A4A5A6A70B0809365C5D5A5B2C232221") ;
    struct lsh_string *cipher = simple_decode_hex("19DC705B857A60FB07717B2EA5717781") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 109 ... ", stderr);
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
    fputs("Decrypting with rijndael 109 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("80818283858687888A8B8C8D8F90919294959697999A9B9C9E9FA0A1A3A4A5A6") ;
    struct lsh_string *plain = simple_decode_hex("464744455754555AF3F2F5F4AFB0B1B2") ;
    struct lsh_string *cipher = simple_decode_hex("FFC8AEB885B5EFCAD06B6DBEBF92E76B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 110 ... ", stderr);
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
    fputs("Decrypting with rijndael 110 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("A8A9AAABADAEAFB0B2B3B4B5B7B8B9BABCBDBEBFC1C2C3C4C6C7C8C9CBCCCDCE") ;
    struct lsh_string *plain = simple_decode_hex("323330317675746B7273747549464744") ;
    struct lsh_string *cipher = simple_decode_hex("F58900C5E0B385253FF2546250A0142B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 111 ... ", stderr);
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
    fputs("Decrypting with rijndael 111 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D0D1D2D3D5D6D7D8DADBDCDDDFE0E1E2E4E5E6E7E9EAEBECEEEFF0F1F3F4F5F6") ;
    struct lsh_string *plain = simple_decode_hex("A8A9AAAB181B1A15808186872B141516") ;
    struct lsh_string *cipher = simple_decode_hex("2EE67B56280BC462429CEE6E3370CBC1") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 112 ... ", stderr);
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
    fputs("Decrypting with rijndael 112 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("F8F9FAFBFDFEFE00020304050708090A0C0D0E0F11121314161718191B1C1D1E") ;
    struct lsh_string *plain = simple_decode_hex("E7E6E5E4202323DDAAABACAD343B3A39") ;
    struct lsh_string *cipher = simple_decode_hex("20DB650A9C8E9A84AB4D25F7EDC8F03F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 113 ... ", stderr);
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
    fputs("Decrypting with rijndael 113 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("20212223252627282A2B2C2D2F30313234353637393A3B3C3E3F404143444546") ;
    struct lsh_string *plain = simple_decode_hex("A8A9AAAB2221202FEDECEBEA1E010003") ;
    struct lsh_string *cipher = simple_decode_hex("3C36DA169525CF818843805F25B78AE5") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 114 ... ", stderr);
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
    fputs("Decrypting with rijndael 114 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("48494A4B4D4E4F50525354555758595A5C5D5E5F61626364666768696B6C6D6E") ;
    struct lsh_string *plain = simple_decode_hex("F9F8FBFA5F5C5D42424344450E010003") ;
    struct lsh_string *cipher = simple_decode_hex("9A781D960DB9E45E37779042FEA51922") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 115 ... ", stderr);
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
    fputs("Decrypting with rijndael 115 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("70717273757677787A7B7C7D7F80818284858687898A8B8C8E8F909193949596") ;
    struct lsh_string *plain = simple_decode_hex("57565554F5F6F7F89697909120DFDEDD") ;
    struct lsh_string *cipher = simple_decode_hex("6560395EC269C672A3C288226EFDBA77") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 116 ... ", stderr);
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
    fputs("Decrypting with rijndael 116 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("98999A9B9D9E9FA0A2A3A4A5A7A8A9AAACADAEAFB1B2B3B4B6B7B8B9BBBCBDBE") ;
    struct lsh_string *plain = simple_decode_hex("F8F9FAFBCCCFCEF1DDDCDBDA0E010003") ;
    struct lsh_string *cipher = simple_decode_hex("8C772B7A189AC544453D5916EBB27B9A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 117 ... ", stderr);
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
    fputs("Decrypting with rijndael 117 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("C0C1C2C3C5C6C7C8CACBCCCDCFD0D1D2D4D5D6D7D9DADBDCDEDFE0E1E3E4E5E6") ;
    struct lsh_string *plain = simple_decode_hex("D9D8DBDA7073727D80818687C2DDDCDF") ;
    struct lsh_string *cipher = simple_decode_hex("77CA5468CC48E843D05F78EED9D6578F") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 118 ... ", stderr);
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
    fputs("Decrypting with rijndael 118 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FAFCFDFEFF01020304060708090B0C0D0E") ;
    struct lsh_string *plain = simple_decode_hex("C5C4C7C6080B0A1588898E8F68676665") ;
    struct lsh_string *cipher = simple_decode_hex("72CDCC71DC82C60D4429C9E2D8195BAA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 119 ... ", stderr);
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
    fputs("Decrypting with rijndael 119 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("10111213151617181A1B1C1D1F20212224252627292A2B2C2E2F303133343536") ;
    struct lsh_string *plain = simple_decode_hex("83828180DCDFDED186878081F0CFCECD") ;
    struct lsh_string *cipher = simple_decode_hex("8080D68CE60E94B40B5B8B69EEB35AFA") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 120 ... ", stderr);
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
    fputs("Decrypting with rijndael 120 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("38393A3B3D3E3F40424344454748494A4C4D4E4F51525354565758595B5C5D5E") ;
    struct lsh_string *plain = simple_decode_hex("98999A9BDDDEDFA079787F7E0A050407") ;
    struct lsh_string *cipher = simple_decode_hex("44222D3CDE299C04369D58AC0EBA1E8E") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 121 ... ", stderr);
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
    fputs("Decrypting with rijndael 121 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("60616263656667686A6B6C6D6F70717274757677797A7B7C7E7F808183848586") ;
    struct lsh_string *plain = simple_decode_hex("CECFCCCD4F4C4D429F9E9998DFC0C1C2") ;
    struct lsh_string *cipher = simple_decode_hex("9B8721B0A8DFC691C5BC5885DBFCB27A") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 122 ... ", stderr);
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
    fputs("Decrypting with rijndael 122 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("88898A8B8D8E8F90929394959798999A9C9D9E9FA1A2A3A4A6A7A8A9ABACADAE") ;
    struct lsh_string *plain = simple_decode_hex("404142436665647B29282F2EABA4A5A6") ;
    struct lsh_string *cipher = simple_decode_hex("0DC015CE9A3A3414B5E62EC643384183") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 123 ... ", stderr);
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
    fputs("Decrypting with rijndael 123 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("B0B1B2B3B5B6B7B8BABBBCBDBFC0C1C2C4C5C6C7C9CACBCCCECFD0D1D3D4D5D6") ;
    struct lsh_string *plain = simple_decode_hex("33323130E6E5E4EB23222524DEA1A0A3") ;
    struct lsh_string *cipher = simple_decode_hex("705715448A8DA412025CE38345C2A148") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 124 ... ", stderr);
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
    fputs("Decrypting with rijndael 124 ... ", stderr);
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
    struct lsh_string *key = simple_decode_hex("D8D9DADBDDDEDFE0E2E3E4E5E7E8E9EAECEDEEEFF1F2F3F4F6F7F8F9FBFCFDFE") ;
    struct lsh_string *plain = simple_decode_hex("CFCECDCCF6F5F4CBE6E7E0E199969794") ;
    struct lsh_string *cipher = simple_decode_hex("C32B5B0B6FBAE165266C569F4B6ECF0B") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 125 ... ", stderr);
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
    fputs("Decrypting with rijndael 125 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("BABBB8B97271707FDCDDDADB29363734") ;
    struct lsh_string *cipher = simple_decode_hex("4DCA6C75192A01DDCA9476AF2A521E87") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 126 ... ", stderr);
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
    fputs("Decrypting with rijndael 126 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("C9C8CBCA4447465926272021545B5A59") ;
    struct lsh_string *cipher = simple_decode_hex("058691E627ECBC36AC07B6DB423BD698") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 127 ... ", stderr);
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
    fputs("Decrypting with rijndael 127 ... ", stderr);
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
    struct lsh_string *plain = simple_decode_hex("050407067477767956575051221D1C1F") ;
    struct lsh_string *cipher = simple_decode_hex("7444527095838FE080FC2BCDD30847EB") ;
    struct crypto_instance *c;

    assert(key->length == algorithm->key_size);
    assert(!algorithm->iv_size);

    c = MAKE_ENCRYPT(algorithm, key->data, NULL);
    
  {
    struct lsh_string *a, *b;
    fputs("Encrypting with rijndael 128 ... ", stderr);
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
    fputs("Decrypting with rijndael 128 ... ", stderr);
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

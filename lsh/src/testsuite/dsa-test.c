



















    













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
  struct alist *algorithms = all_signature_algorithms(make_bad_random());
  struct sexp *key = string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "{KDM6ZHNhKDE6cDEyODqD2afCziqRefQ82zv/594PDu8m3V365E1TG8DeRWNN"
 	      "LAfLkpsNvhDaWABw5qv7uEFcRL/1cLitd532U6rZfce964FdfogQPmFgbtPY"
 	      "opX7/TQNLUniIIM+us5VEeIsTwKX7TUemUj6hI6cj623tHvMR970JVteHV4Q"
 	      "IVs7VaC4XykoMTpxMjA6gmbg3q9GAgukjUEMpYDzqXhim10pKDE6ZzEyODow"
 	      "00u583a+yUcVSv5Adrx9NZydMvVHHdu+jWqUHEf6ncTzJXMVHbtKpZ65ibdK"
 	      "w2u2MQpei1gFAWVdkfOT2qGTrhMDBJuH/rsJPcBAS1O0xdokYzAPnFsVbXiM"
 	      "Ss6Oy7ndAMGNmVN/JVrAJdB02JSmB8vjAjoSdu9VaRajP33lQykoMTp5MTI4"
 	      "OmRAIEiyfzn0BKVGqEkJycDp4t0VOoSZRhBiiSWY0wryeuPO/CtwD7bQdzkK"
 	      "g73K14oSmUh8liO7Yq8MhaPfnvHuLA1mZY4f0yg7VAf2zTDufmFU+tQaaosP"
 	      "XIbFrMwRJ798ml1rrcsBIYDLYqVcXhfW01KM2+ACzO4THBuGhn96KSgxOngy"
 	      "MDpWxu+vh40G7vIdwHD6tx2m7B4wpikp}") , 1);
  struct lsh_string *msg = ssh_format("%lz", "Needs some randomness.") ;
  struct sexp *sign;
  struct signer *s = spki_make_signer(algorithms, key, NULL);
  struct verifier *v;

  fputs("DSA signing ... ", stderr);;
  if (!s)
    /* Invalid key. */
    { fputs("failed.\n", stderr);; exit(1); }; 

  sign = SIGN_SPKI(s, msg->length, msg->data);

  
  v = SIGNER_GET_VERIFIER(s);
  if (!v)
    /* Can't create verifier */
    { fputs("failed.\n", stderr);; exit(1); }

  if (!VERIFY_SPKI(v, msg->length, msg->data, sign))
    /* Unexpected verification failure. */
    { fputs("failed.\n", stderr);; exit(1); };

  /* Modify message slightly. */
  assert(msg->length > 10);

  msg->data[5] ^= 0x40;

  if (VERIFY_SPKI(v, msg->length, msg->data, sign))
    /* Unexpected verification success. */
    { fputs("failed.\n", stderr);; exit(1); };

  fputs("ok.\n", stderr);
}

  return 0;
}

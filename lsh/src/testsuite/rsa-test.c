



















    













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
  struct sexp *key = string_to_sexp(SEXP_TRANSPORT, ssh_format("%lz", "{KDE0OnJzYS1wa2NzMS1zaGExKDE6bjI1MzoBFRr6hWOxSyj4UOuRSv2TgnQT"
	      "RIbT+XhBCCuC0delL8hCt9SKN8QYGn9eSfJaMknWXZdbiATciNWXtC6BInC5"
    	      "PiIyCwZP21u4x/UYNq7EpYRWfOr2GeFtgqAgjLCPiNcDmaje5RI6HltB1t69"
    	      "gyRlgmTEGedjfCVUksR9HCa/YHO4Qj6nfGL43suz0qzbw90fo52pJQAFVsob"
    	      "8uNXcIkKEIUF6iaCcFxz5chlmUDDuWZsu0CN2hwTVcNEODo2h4K9UZSJ1q5r"
    	      "flh3Yd2hKeQDY4g1em/4faS43lvhkV+PqXNyaV5/9Oo5PfPhuVbppOvhV3fU"
    	      "6GsPNH2rmaSnKSgxOmU0Og8eiqcpKDE6ZDI1MzoBALlbGgNWW/Iobv5Zm17I"
    	      "lj8Vwg3DgYucIlNkKs4z+G5I1J5wSLlsSJp5mDixgy2meph8Zo7kRM8DXNbQ"
    	      "FxPrx184sRHI+zQlcd61ldDSiy+U55cw99k+AW9xPGiSlIwfGFJb8+ihimFr"
    	      "LD4bpJTjZmWFMza4W4EqovJZar2Lk8j1ahEClrVZxmClMwGViQf6sezat9eD"
    	      "ccA3D1J7lbCqhhZ4/zO7M97VifRfcvzJCRsQUgf3fdsY6BFp0/jkx+7y0ZgC"
    	      "IfBx7JJPPAe90wNIG7s+XXE4A0M3txusqFE3gpIZ+H84xD/as/h/CAkDnZDK"
    	      "xTjaeL9KEMXaJDlXKSgxOnAxMjY6wOsl7emcAJwKRkNPGY2J+P/XTvCBSzKo"
    	      "JfT3uwfCREpGfg4YliDVXyh1IgFeIZU70e8OoRkrBlDtyTzcgdq9okgSdPLu"
    	      "+tIffBLfHrQ+ZAcg2Wp3cKckhIgqy8GdQutTw/z3aDGpOmoIS+mijVmdDNmp"
    	      "g2IB/mSPjtOok3aXKSgxOnExMjc6AW+29CAmn3PY4n6MlE9jfE/ErXCTKZFh"
    	      "IEASNJPF+zsKJSNDtx1ZeVmgX8/00ZFVASs2SScWlUQLYyHWqkjDjA3URL9n"
    	      "nhdOCyGSJufb7C0OkPRWPmvlLdQi1rqIKff1IlsBRBK4y64suPmPAJ+rx39X"
    	      "sFpTto7nzbji9wWUcSkoMTphMTI2OiIbnv4UD+cPiMag8ejks81LF5wWYLxi"
    	      "UL2vQBPMQHfEVbqDupjxBjDaH/OZqtSo7kpBUPEfCT6QnbgEiozEPKoV2GOv"
    	      "tzduDuNIG/cypIZ81a79rChDmt98o8Mo3xPNaOtQkdgpAGM4N/OZm65YCwd4"
    	      "Rvi7sTgrx/c0uzXicykoMTpiMTI2Oi65so8tg+/ELd2PhaVfBGF9GAPYX8OT"
    	      "ZPTR4TImy38sscW81Dwfn+IHUWZQZZVxkCF5g0WnZHM1Tb4yg6CyQoVAnipW"
    	      "pf8D2Yf0FQuIRcwbUwLuEVPQ7/DZsUHPp0W2lvnSc987T0n8b4M43VOeg6Mj"
    	      "EnREvLlSrjmPH288ZykoMTpjMTI2OrNtDOjzHIbPm3Tf4YlXcw9DxuXjhQQo"
    	      "0dbVG80b4XAPc0Vtc3tKMw8hdWbqiKp6J4hzM+U5uz/bMCIqDTYK1srP0pNZ"
    	      "8j3Q8Os5BI/JrVtkVMl2/BSvq4LRPVI08E49oGRLI/E4zxOVrmmoASntirUP"
    	      "zrjLGBu2zAA3XTEb+Ckp}") , 1);
  struct lsh_string *msg = ssh_format("%lz", "The magic words are squeamish ossifrage") ;
  struct sexp *sign;
  struct signer *s = spki_make_signer(algorithms, key, NULL);
  struct verifier *v;

  fputs("RSA signing ... ", stderr);;
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

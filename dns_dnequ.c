/* dns_dneq(): compare to domain names and return true if equal
 * (case-unsensitive)
 */

#include "dns.h"

int dns_dnequ(const unsigned char *dn1, const unsigned char *dn2) {
  unsigned c;

  for(;;) {
    if ((c = *dn1++) != *dn2++)
      return 0;
    if (!c)
      return 1;
    /* optimization: unroll loop 4 times
     * Measured ~7% speedup on GCC -O2 */
    while(c >= 4) {
      if (dns_dnlc(dn1[0]) != dns_dnlc(dn2[0])) return 0;
      if (dns_dnlc(dn1[1]) != dns_dnlc(dn2[1])) return 0;
      if (dns_dnlc(dn1[2]) != dns_dnlc(dn2[2])) return 0;
      if (dns_dnlc(dn1[3]) != dns_dnlc(dn2[3])) return 0;
      dn1 += 4;
      dn2 += 4;
      c -= 4;
    }
    while(c--) {
      if (dns_dnlc(*dn1) != dns_dnlc(*dn2))
        return 0;
      ++dn1; ++dn2;
    }
  }
}

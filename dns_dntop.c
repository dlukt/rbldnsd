/* dns_dntop() produces printable ascii representation (external form)
 * of a domain name
 */

#include "dns.h"

static const unsigned char char_type[] = {
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

unsigned dns_dntop(const unsigned char *dn, char *dst, unsigned dstsiz) {
  char *d = dst;
  char *m = dst + dstsiz - 1;
  unsigned n;
  unsigned char c;

  if (!(n = *dn++)) {
    if (dstsiz < 2)
      return 0;
    *d++ = '.';
    *d++ = '\0';
    return 1;
  }
  do {
    if (d >= m)
      return 0;
    if (dst != d) *d++ = '.';
    do {
      c = *dn++;
      switch(char_type[c]) {
      case 0: /* safe */
        if (d >= m)
          return 0;
        *d++ = c;
        break;
      case 1: /* special */
        if (d + 1 >= m)
          return 0;
        *d++ = '\\';
        *d++ = c;
        break;
      default: /* non-printable */
        if (d + 3 >= m)
          return 0;
        *d++ = '\\';
        *d++ = '0' + (c / 100);
        *d++ = '0' + ((c % 100) / 10);
        *d++ = '0' + (c % 10);
        break;
      }
    } while(--n);
  } while((n = *dn++) != 0);
  *d = '\0';
  return d - dst;
}

// Unrounded scaling float-to-string conversion.
// From Russ Cox, "Floating-Point Printing and Parsing Can Be Simple And Fast"
// https://research.swtch.com/fp
// Source: https://github.com/rsc/fpfmt commit 6255750 (19 Jan 2026)
// License: BSD-3-Clause (see LICENSE in this directory)

//#include "uscale.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pow10.h"

static uint64_t rotateRight64(uint64_t x, int s) {
  return (x >> s) | (x << (64 - s));
}

static void trimZeros(uint64_t x, int p, uint64_t* xp, int* pp) {
  const uint64_t maxUint64 = ~0ULL;
  const uint64_t div1e8m = 0xc767074b22e90e21ULL;
  const uint64_t div1e4m = 0xd288ce703afb7e91ULL;
  const uint64_t div1e2m = 0x8f5c28f5c28f5c29ULL;
  const uint64_t div1e1m = 0xcccccccccccccccdULL;
  const uint64_t div1e8le = maxUint64 / 100000000;
  const uint64_t div1e4le = maxUint64 / 10000;
  const uint64_t div1e2le = maxUint64 / 100;
  const uint64_t div1e1le = maxUint64 / 10;

  uint64_t d;

  // Cut 1 zero, or else return.
  if ((d = rotateRight64(x * div1e1m, 1)) <= div1e1le) {
    x = d;
    p++;
  } else {
    *xp = x;
    *pp = p;
    return;
  }

  // Cut 8 zeros, then 4, then 2, then 1.
  if ((d = rotateRight64(x * div1e8m, 8)) <= div1e8le) {
    x = d;
    p += 8;
  }
  if ((d = rotateRight64(x * div1e4m, 4)) <= div1e4le) {
    x = d;
    p += 4;
  }
  if ((d = rotateRight64(x * div1e2m, 2)) <= div1e2le) {
    x = d;
    p += 2;
  }
  if ((d = rotateRight64(x * div1e1m, 1)) <= div1e1le) {
    x = d;
    p += 1;
  }
  *xp = x;
  *pp = p;
}

static void unpack64(double f, uint64_t* mp, int* ep) {
  uint64_t b;
  memcpy(&b, &f, sizeof f);
  uint64_t m = (b & ((1ULL << 52) - 1)) << 11;
  int e = ((b >> 52) & ((1 << 11) - 1)) - 1086;
  if (e == -1086) {
    e = -1085;
    int s = __builtin_clzll(m);
    m <<= s;
    e -= s;
  } else {
    m |= 1ULL << 63;
  }
  *mp = m;
  *ep = e;
}

typedef uint64_t unrounded;

static uint64_t ufloor(unrounded u) { return (u + 0) >> 2; }
static uint64_t uceil(unrounded u) { return (u + 3) >> 2; }
static uint64_t unudge(unrounded u, int64_t d) { return u + d; }
static uint64_t uround(unrounded u) { return (u + 1 + ((u >> 2) & 1)) >> 2; }

static uint64_t udiv(unrounded u, uint64_t d) {
  return (u / d) | (u & 1) | (u % d != 0);
}

static int log10Pow2(int x) { return (x * 78913) >> 18; }

static int log2Pow10(int x) { return (x * 108853) >> 15; }

static int skewed(int e) { return (e * 631305 - 261663) >> 21; }

typedef struct Scalers Scalers;
struct Scalers {
  uint64_t pmHi;
  uint64_t pmLo;
  int s;
};

static Scalers prescale(int e, int p, int lp) {
  if (p < pow10Min || p > pow10Max) abort();
  int s = -(e + lp + 3);
  Scalers pre;
  p -= pow10Min;
  pre.pmHi = pow10Tab[p][0];
  pre.pmLo = pow10Tab[p][1];
  pre.s = s;
  return pre;
}

static unrounded uscale_impl(uint64_t x, Scalers c) {
  unsigned __int128 full = (unsigned __int128)x * c.pmHi;
  uint64_t hi = full >> 64;
  uint64_t mid1 = full;
  uint64_t sticky = 1;
  if ((hi & ((1ULL << c.s) - 1)) == 0) {
    uint64_t mid2 = ((unsigned __int128)x * c.pmLo) >> 64;
    sticky = (mid1 - mid2 > 1);
    hi -= mid1 < mid2;
  }
  return (hi >> c.s) | sticky;
}

// static uint64_t uint64pow10[] = {
//     1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
//     1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
// };
static const uint64_t uint64pow10[] = {
	1,//0
	10,//1
	100,//2
	1000,//3
	10000,//4
	100000,//5
	1000000,//6
	10000000,//7
	100000000,//8
	1000000000,//9
	10000000000,//10
	100000000000,//11
	1000000000000,//12
	10000000000000,//13
	100000000000000,//14
	1000000000000000,//15
	10000000000000000,//16
	100000000000000000,//17
	1000000000000000000ull,//18
	10000000000000000000ull,//19
};

static int bitsLen64(uint64_t x) { return 64 - __builtin_clzll(x); }

static int countDigits(uint64_t d) {
  int nd = log10Pow2(bitsLen64(d));
  return nd + (d >= uint64pow10[nd]);
}

static void Short(double f, uint64_t* dp, int* pp) {
  uint64_t m;
  int e;
  unpack64(f, &m, &e);

  int p;
  uint64_t min;
  int b = 11;
  if (m == 1ULL << 63 && e > -1085) {
    p = -skewed(e + b);
    min = m - (1ULL << (b - 2));
  } else {
    if (e < -1085) {
      b = 11 + (-1085 - e);
    }
    p = -log10Pow2(e + b);
    min = m - (1ULL << (b - 1));
  }
  uint64_t max = m + (1ULL << (b - 1));

  int odd = (m >> b) & 1;
  Scalers pre = prescale(e, p, log2Pow10(p));
  uint64_t dmin = uceil(unudge(uscale_impl(min, pre), +odd));
  uint64_t dmax = ufloor(unudge(uscale_impl(max, pre), -odd));

  uint64_t d0 = dmax / 10 * 10;
  if (d0 >= dmin) {
    trimZeros(dmax / 10, -(p - 1), dp, pp);
    return;
  }
  uint64_t d = dmin;
  if (d < dmax) d = uround(uscale_impl(m, pre));
  *dp = d;
  *pp = -p;
}
static void Short_opt(double f, uint64_t* dp, int* pp) {
  // require f > 0 ; not 0,inf,nan; return shotest decimal representation
  uint64_t u;
  memcpy(&u, &f, sizeof(uint64_t));
  uint64_t m = (u << 11) | (1ULL << 63);
  uint64_t exp = ((u >> 52) & ((1 << 11) - 1));
  uint64_t subnormal = exp == 0;
  uint64_t irregular = (u << 12) == 0;
  int e = exp - 1086;
  int p = -log10Pow2(e + 11);
  uint64_t min = m - (1ULL << (11 - 1));
  uint64_t max = m + (1ULL << (11 - 1));
  if (subnormal) [[unlikely]] {
    int s = __builtin_clzll(u << 11);
    m <<= s;
    e = -1085 - s;
    int b = 11 + s;
    p = -log10Pow2(e + b);
    min = m - (1ULL << (b - 1));
    max = m + (1ULL << (b - 1));
  }
  if (irregular) [[unlikely]] {
    p = -skewed(e + 11);
    min = m - (1ULL << (11 - 2));
  }
  int64_t odd = m & 1;
  Scalers pre = prescale(e, p, log2Pow10(p));
  uint64_t dmin = uceil(unudge(uscale_impl(min, pre), +odd));
  uint64_t dmax = ufloor(unudge(uscale_impl(max, pre), -odd));

  uint64_t d0 = dmax / 10 * 10;
  if (d0 >= dmin) {
    trimZeros(dmax / 10, -(p - 1), dp, pp);
    return;
  }
  uint64_t d = dmin;
  if (d < dmax) d = uround(uscale_impl(m, pre));
  *dp = d;
  *pp = -p;
}

static const char i2a[] =
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899";

static void formatBase10(char* dst, uint64_t d64, int nd) {
  while ((d64 >> 32) != 0) {
    uint32_t x = d64 % 100000000;
    d64 /= 100000000;
    uint32_t y = x % 10000;
    x /= 10000;
    uint32_t x1 = (x / 100) * 2;
    uint32_t x0 = (x % 100) * 2;
    uint32_t y1 = (y / 100) * 2;
    uint32_t y0 = (y % 100) * 2;
    memcpy(dst + nd - 8, i2a + x1, 2);
    memcpy(dst + nd - 6, i2a + x0, 2);
    memcpy(dst + nd - 4, i2a + y1, 2);
    memcpy(dst + nd - 2, i2a + y0, 2);
    nd -= 8;
  }

  uint32_t d = d64;
  while (d >= 10000) {
    uint32_t x = d % 10000;
    d /= 10000;
    uint32_t x1 = (x / 100) * 2;
    uint32_t x0 = (x % 100) * 2;
    memcpy(dst + nd - 4, i2a + x1, 2);
    memcpy(dst + nd - 2, i2a + x0, 2);
    nd -= 4;
  }
  if (d >= 100) {
    uint32_t x = d % 100;
    d /= 100;
    memcpy(dst + nd - 2, i2a + 2 * x, 2);
    nd -= 2;
  }
  if (d >= 10) {
    memcpy(dst + nd - 2, i2a + 2 * d, 2);
    return;
  }
  dst[nd - 1] = '0' + d;
}

static char* Format(char* dst, uint64_t d, int p, int nd) {
  formatBase10(dst + 1, d, nd);
  p += nd - 1;
  dst[0] = dst[1];
  int n = nd;
  if (n > 1) {
    dst[1] = '.';
    n++;
  }
  dst[n] = 'e';
  if (p < 0) {
    dst[n + 1] = '-';
    p = -p;
  } else {
    dst[n + 1] = '+';
  }
  if (p < 100) {
    memcpy(dst + n + 2, i2a + 2 * p, 2);
    dst[n + 4] = 0;
    return dst + n + 4;
  }
  dst[n + 2] = '0' + p / 100;
  memcpy(dst + n + 3, i2a + 2 * (p % 100), 2);
  dst[n + 5] = 0;
  return dst + n + 5;
}

// void uscale_short_old(double f, char* dst) {
//   uint64_t bits;
//   memcpy(&bits, &f, sizeof f);

//   // Handle negative.
//   if (bits >> 63) {
//     *dst++ = '-';
//     f = -f;
//   }

//   // Handle zero.
//   if ((bits & 0x7FFFFFFFFFFFFFFFULL) == 0) {
//     dst[0] = '0';
//     dst[1] = 0;
//     return;
//   }

//   // Handle NaN and infinity.
//   if ((bits & 0x7FFFFFFFFFFFFFFFULL) >> 52 == 0x7FF) {
//     memcpy(dst, bits & ((1ULL << 52) - 1) == 0 ? "inf" : "nan", 4);
//     return;
//   }

//   uint64_t d;
//   int p;
//   Short(f, &d, &p);
//   Format(dst, d, p, countDigits(d));
// }

char* uscale_short(double f, char* dst) {
	uint64_t bits;
	memcpy(&bits, &f, sizeof f);
  
	// Handle negative.
	dst[0] = '-';
	dst += bits >> 63;

	// abs
    bits &= 0x7FFFFFFFFFFFFFFFULL;

	// Handle zero.
	if (bits == 0) {
	  memcpy(dst, "0", 2);
	  return dst + 1;
	}
  
	// Handle NaN and infinity.
	if ((bits >> 52) == 0x7FF) {
	  memcpy(dst, ((bits & ((1ULL << 52) - 1)) == 0) ? "inf" : "nan", 4);
	  return dst + 3;
	}
  
	uint64_t d;
	int p;
	Short(f, &d, &p);
	return Format(dst, d, p, countDigits(d));
  }
  
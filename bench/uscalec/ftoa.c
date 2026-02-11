#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include "../bench.h"
#include "pow10.h"

static uint64_t rotateRight64(uint64_t x, int s) {
	return (x>>s) | (x<<(64-s));
}

static void trimZeros(uint64_t x, int p, uint64_t *xp, int *pp) {
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
	if ((d = rotateRight64(x*div1e1m, 1)) <= div1e1le) {
		x = d;
		p++;
	} else {
		*xp = x;
		*pp = p;
		return;
	}

	// Cut 8 zeros, then 4, then 2, then 1.
	if ((d = rotateRight64(x*div1e8m, 8)) <= div1e8le) {
		x = d;
		p += 8;
	}
	if ((d = rotateRight64(x*div1e4m, 4)) <= div1e4le) {
		x = d;
		p += 4;
	}
	if ((d = rotateRight64(x*div1e2m, 2)) <= div1e2le) {
		x = d;
		p += 2;
	}
	if ((d = rotateRight64(x*div1e1m, 1)) <= div1e1le) {
		x = d;
		p += 1;
	}
	*xp = x;
	*pp = p;
}

void unpack64(double f, uint64_t *mp, int *ep) {
	uint64_t b;
	memcpy(&b, &f, sizeof f);
	uint64_t m = (b&((1ULL<<52)-1))<<11;
	int e = ((b>>52)&((1<<11)-1)) - 1086;
	if (e == -1086) {
		e = -1085;
		int s = __builtin_clzll(m);
		m <<= s;
		e -= s;
	} else {
		m |= 1ULL<< 63;
	}
	*mp = m;
	*ep = e;
}

typedef uint64_t unrounded;

static uint64_t ufloor(unrounded u) { return (u+0) >> 2; }
static uint64_t uceil(unrounded u) { return (u+3) >> 2; }
static uint64_t unudge(unrounded u, int d) { return u + d; }
static uint64_t uround(unrounded u) { return (u + 1 + ((u>>2)&1)) >> 2; }

static uint64_t udiv(unrounded u, uint64_t d) {
	return (u/d) | (u&1) | (u%d != 0);
}

static uint64_t ursh(unrounded u, int s) {
	return (u>>s) | (u&1) | ((u&((1<<s)-1)) != 0);
}

static int log10Pow2_uc(int x) {
	return (x * 78913) >> 18;
}

static int log2Pow10_uc(int x) {
	return (x * 108853) >> 15;
}

static int skewed(int e) {
	return (e*631305 - 261663) >> 21;
}


typedef struct Digits Digits;
struct Digits {
	uint64_t d;
	int p;
};

typedef struct Scalers Scalers;
struct Scalers {
	uint64_t pmHi;
	uint64_t pmLo;
	int s;
};

static Scalers prescale(int e, int p, int lp) {
	// if(p < pow10Min || p > pow10Max)
	// 	abort();
	int s = -(e + lp + 3);
	Scalers pre;
	p -= pow10Min;
	pre.pmHi = pow10Tab[p][0];
	pre.pmLo = pow10Tab[p][1];
	pre.s = s;
	return pre;
}

static unrounded uscale(uint64_t x, Scalers c) {
	//printf("uscale x=%#llx c=%#llx %#llx %d\n", x, c.pmHi, c.pmLo, c.s);
	unsigned __int128 full = (unsigned __int128)x * c.pmHi;
	uint64_t hi = full>>64;
	uint64_t mid1 = full;
	//printf("hi=%#llx mid1=%#llx\n", hi, mid1);
	uint64_t sticky = 1;
	if ((hi & ((1ULL<<c.s)-1)) == 0) {
		uint64_t mid2 = ((unsigned __int128)x * c.pmLo) >> 64;
		//printf("mid2=%#llx\n", mid2);
		sticky = (mid1-mid2 > 1);
		hi -= mid1 < mid2;
	}
	return (hi>>c.s) | sticky;
}

uint64_t uint64pow10[] = {
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

void FixedWidth(double f, int n, uint64_t *dp, int *pp) {
	uint64_t m;
	int e;
	unpack64(f, &m, &e);
	//printf("unpack %.17g %#llx %d n=%d\n", f, m, e, n);
	int p = n - 1 - log10Pow2_uc(e+63);
	//printf("p = %d\n", p);
	unrounded u = uscale(m, prescale(e, p, log2Pow10_uc(p)));
	//printf("u = %#llx\n", u);
	uint64_t d = uround(u);
	//printf("d = %lld\n", d);
	if(d >= uint64pow10[n]) {
		d = uround(udiv(u, 10));
		p--;
	}
	*dp = d;
	*pp = -p;
}

static int bitsLen64(uint64_t x) {
	return 64 - __builtin_clzll(x);
}

double pack64(uint64_t m, int e) {
	if((m & (1ULL<<52)) != 0)
		m = (m&~(1ULL<<52)) | ((uint64_t)(1075+e)<<52);
	double f;
	memcpy(&f, &m, sizeof f);
	return f;
}

double Parse(uint64_t d, int p) {
	int b = bitsLen64(d);
	int lp = log2Pow10_uc(p);
	int e = 53 - b - lp;
	if (e > 1074)
		e = 1074;
	unrounded u = uscale(d<<(64-b), prescale(e-(64-b), p, lp));
	int adj = (u >= (1ULL<<55)-2);
	u = (u>>adj) | u&1;
	e -= adj;
	uint64_t m = uround(u);
	return pack64(m, -e);
}

static void Short(double f, uint64_t *dp, int *pp) {
	uint64_t m;
	int e;
	unpack64(f, &m, &e);

	int p;
	uint64_t min;
	int b = 11;
	if (m == 1ULL<<63 && e > -1085) {
		p = -skewed(e+b);
		min = m - (1ULL<<(b-2));
	} else {
		if (e < -1085) {
			b = 11 + (-1085 - e);
		}
		p = -log10Pow2_uc(e+b);
		min = m - (1ULL<<(b-1));
	}
	uint64_t max = m + (1ULL<<(b-1));

	int odd = (m>>b) & 1;
	Scalers pre = prescale(e, p, log2Pow10_uc(p));
	uint64_t dmin = uceil(unudge(uscale(min, pre), +odd));
	uint64_t dmax = ufloor(unudge(uscale(max, pre), -odd));

	//printf("cshort f=%.17g m=%#llx e=%d p=%d b=%d odd=%d\nmin=%#llx max=%#llx dmin=%lld dmax=%lld d0=%lld\n",
	//	f, m, e, p, b, odd, min, max, dmin, dmax, d0);

	uint64_t d0 = dmax / 10 * 10;
	if (d0 >= dmin) {
		trimZeros(dmax/10, -(p-1), dp, pp);
		return;
	}
	uint64_t d = dmin;
	if (d < dmax)
		d = uround(uscale(m, pre));
	*dp = d;
	*pp = -p;
}

static const char i2a[] = "00010203040506070809"
	"10111213141516171819"
	"20212223242526272829"
	"30313233343536373839"
	"40414243444546474849"
	"50515253545556575859"
	"60616263646566676869"
	"70717273747576777879"
	"80818283848586878889"
	"90919293949596979899";

static void
formatBase10(char *dst, uint64_t d64, int nd)
{
	while ((d64>>32) != 0) {
		uint32_t x = d64 % 100000000;
		d64 /= 100000000;
		uint32_t y = x % 10000;
		x /= 10000;
		uint32_t x1 = (x / 100) * 2;
		uint32_t x0 = (x % 100) * 2;
		uint32_t y1 = (y / 100) * 2;
		uint32_t y0 = (y % 100) * 2;
		memcpy(dst+nd-8, i2a+x1, 2);
		memcpy(dst+nd-6, i2a+x0, 2);
		memcpy(dst+nd-4, i2a+y1, 2);
		memcpy(dst+nd-2, i2a+y0, 2);
		nd -= 8;
	}

	uint32_t d = d64;
	while(d >= 10000) {
		uint32_t x = d % 10000;
		d /= 10000;
		uint32_t x1 = (x / 100) * 2;
		uint32_t x0 = (x % 100) * 2;
		memcpy(dst+nd-4, i2a+x1, 2);
		memcpy(dst+nd-2, i2a+x0, 2);
		nd -= 4;
	}
	if(d >= 100) {
		uint32_t x = d % 100;
		d /= 100;
		memcpy(dst+nd-2, i2a+2*x, 2);
		nd -= 2;
	}
	if(d >= 10) {
		memcpy(dst+nd-2, i2a+2*d, 2);
		return;
	}
	dst[nd-1] = '0' + d;
}

static int countDigits(uint64_t d) {
	int nd = log10Pow2_uc(bitsLen64(d));
	return nd + (d >= uint64pow10[nd]);
}

int Format(char *dst, uint64_t d, int p, int nd) {
	formatBase10(dst+1, d, nd);
	p += nd - 1;
	dst[0] = dst[1];
	int n = nd;
	if(n > 1) {
		dst[1] = '.';
		n++;
	}
	dst[n] = 'e';
	if(p<0) {
		dst[n+1] = '-';
		p = -p;
	} else {
		dst[n+1] = '+';
	}
	if (p < 100) {
		memcpy(dst+n+2, i2a+2*p, 2);
		dst[n+4] = 0;
		return n+4;
	}
	dst[n+2] = '0' + p/100;
	memcpy(dst+n+3, i2a+2*(p%100), 2);
	dst[n+5] = 0;
	return n+5;
}

static void fixed(char *dst, double f, int n) {
	uint64_t d;
	int p;
	FixedWidth(f, n, &d, &p);
	Format(dst, d, p, n);
}

static int short1(char *dst, double f) {
	dst[0] = '-';
	uint64_t u;
	memcpy(&u, &f, 8);
	uint64_t idx = u>>63;
	dst += idx;
	uint64_t u_abs = (u<<1)>>1;
	if(u_abs == 0){
		memcpy(dst, "0.0", 4);
		return idx + 3;
	}
	if(u_abs >= 2047ull<<52){
		memcpy(dst, (u_abs == 2047ull<52) ? "inf" : "nan", 4);
		return idx + 3;
	}


	uint64_t d;
	int p;
	Short(f, &d, &p);
	//printf("short1 f=%.17g d=%#llx p=%d\n", f, d, p);
	int len = Format(dst, d, p, countDigits(d));
	return len + idx;
}

static void shortRaw(uint64_t *dp, int64_t *pp, double f) {
	uint64_t d;
	int p;
	Short(f, &d, &p);
	*dp = d;
	*pp = p;
}

static int isDigit(char c) {
	return (unsigned char)(c-'0') <= 9;
}

static double uscalecStrtod(char *s, char *e) {
	// Read digits.
	const int maxDigits = 19;
	uint64_t d = 0;
	int frac = 0;
	int i = 0;
	int len = e - s;
	for(; i < len && isDigit(s[i]); i++)
		d = d*10 + s[i] - '0';
	if(i > maxDigits)
		return 0;
	if(i < len && s[i] == '.') {
		i++;
		for(; i < len && isDigit(s[i]); i++) {
			d = d*10 + s[i] - '0';
			frac++;
		}
		if(i == 1 || i > maxDigits+1)
			return 0;
	}
	if(i == 0)
		return 0;

	// Read exponent.
	int p = 0;
	if(i < len && (s[i] == 'e' || s[i] == 'E')) {
		i++;
		int sign = +1;
		if(i < len) {
			if(s[i] == '-') {
				sign = -1;
				i++;
			} else if(s[i] == '+') {
				i++;
			}
		}
		if(i >= len || len-i > 3)
			return 0;
		for(; i < len && isDigit(s[i]); i++)
			p = p*10 + s[i] - '0';
		p *= sign;
	}
	if(i != len)
		return 0;
	return Parse(d, p-frac);
}

// void uscalecBenchFixed(char *dst, int count, double *f, int nf, int digits) {
// 	benchFixed(dst, count, f, nf, digits, fixed);
// }

// void uscalecBenchShort(char *dst, int count, double *f, int nf) {
// 	benchShort(dst, count, f, nf, short1);
// }

// void uscalecBenchShortRaw(uint64_t *dp, int64_t *pp, int count, double *f, int nf) {
// 	benchShortRaw(dp, pp, count, f, nf, shortRaw);
// }

// double uscalecBenchParseRaw(int count, int64_t *raw, int nraw) {
// 	return benchParseRaw(count, raw, nraw, Parse);
// }

// double uscalecBenchParse(int count, char *s) {
// 	return benchParse(count, s, uscalecStrtod);
// }

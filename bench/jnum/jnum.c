/*******************************************
* SPDX-License-Identifier: MIT             *
* Copyright (C) 2019-.... Jing Leng        *
* Contact: Jing Leng <lengjingzju@163.com> *
* URL: https://github.com/lengjingzju/json *
*******************************************/
#include <string.h>
#include <stdlib.h>
#include "jnum.h"
#if defined(_MSC_VER)
#include <intrin.h>
#endif

#if defined(__GNUC__) || defined(__clang__)
#define UNUSED_ATTR                __attribute__((unused))
#define FALLTHROUGH_ATTR           __attribute__((fallthrough))
#define likely(cond)               __builtin_expect(!!(cond), 1)
#define unlikely(cond)             __builtin_expect(!!(cond), 0)
#else
#define UNUSED_ATTR
#define FALLTHROUGH_ATTR
#define likely(cond)               (cond)
#define unlikely(cond)             (cond)
#endif

#define DIY_SIGNIFICAND_SIZE        64                  /* Symbol: 1 bit, Exponent, 11 bits, Mantissa, 52 bits */
#define DP_SIGNIFICAND_SIZE         52                  /* Mantissa, 52 bits */
#define DP_EXPONENT_OFFSET          0x3FF               /* Exponent offset is 0x3FF */
#define DP_EXPONENT_MAX             0x7FF               /* Max Exponent value */
#define DP_EXPONENT_MASK            0x7FF0000000000000  /* Exponent Mask, 11 bits */
#define DP_SIGNIFICAND_MASK         0x000FFFFFFFFFFFFF  /* Mantissa Mask, 52 bits */
#define DP_HIDDEN_BIT               0x0010000000000000  /* Integer bit for Mantissa */

#if (__WORDSIZE == 64) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __clang_major__ >= 9)
#define USING_U128_CALC             1
#else
#define USING_U128_CALC             0
#endif
#if USING_U128_CALC
__extension__ typedef unsigned __int128 u128;
#endif

/* Whether to use manual loop unfolding */
#ifndef JNUM_MANUAL_LOOP_UNFOLD
#if defined(_MSC_VER)
/* MSVC's performance deteriorates after using unfolding */
#define JNUM_MANUAL_LOOP_UNFOLD     0
#else
#define JNUM_MANUAL_LOOP_UNFOLD     1
#endif
#endif

/* Whether to use small lut to convert double to string */
#ifndef USING_SMALL_LUT
#define USING_SMALL_LUT             0
#endif

typedef struct {
    uint64_t f;
    int32_t e;
} diy_fp_t;

typedef struct {
    uint64_t hi;
    uint64_t lo;
} u64x2_t;

#define FAST_DIV100(n)      (((n) * 5243) >> 19)                            /* 0 <= n < 10000 */
#define FAST_DIV10000(n)    ((uint32_t)(((uint64_t)(n) * 109951163) >> 40)) /* 0 <= n < 100000000 */

static inline int32_t u64_pz_get(uint64_t n)
{
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanReverse64(&index, n);
    return 63 - index;
#elif defined(__GNUC__) || defined(__clang__)
    return __builtin_clzll(n);
#else
    int c = 0;
    if (n & 0xFFFFFFFF00000000) n >>= 32; else c += 32;
    if (n & 0xFFFF0000)         n >>= 16; else c += 16;
    if (n & 0xFF00)             n >>= 8 ; else c += 8;
    if (n & 0xF0)               n >>= 4 ; else c += 4;
    if (n & 0xC)                n >>= 2 ; else c += 2;
    return c + (n & 0x1);
#endif
}

static inline u64x2_t u128_mul(uint64_t x, uint64_t y)
{
    u64x2_t ret;
#if defined(_MSC_VER)
    ret.lo = _umul128(x, y, &ret.hi);
#elif USING_U128_CALC
    const u128 p = (u128)x * (u128)y;
    ret.hi = (uint64_t)(p >> 64);
    ret.lo = (uint64_t)p;
#else
    const uint64_t M32 = 0XFFFFFFFF;
    const uint64_t a = x >> 32;
    const uint64_t b = x & M32;
    const uint64_t c = y >> 32;
    const uint64_t d = y & M32;

    const uint64_t ac = a * c;
    const uint64_t bc = b * c;
    const uint64_t ad = a * d;
    const uint64_t bd = b * d;

    const uint64_t mid1 = ad + (bd >> 32);
    const uint64_t mid2 = bc + (mid1 & M32);

    ret.hi = ac + (mid1 >> 32) + (mid2 >> 32);
    ret.lo = (bd & M32) | (mid2 & M32) << 32;
#endif
    return ret;
}

static const char ch_100_lut[200] = {
    '0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
    '1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
    '2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
    '3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
    '4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
    '5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
    '6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
    '7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
    '8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
    '9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9',
};

static const uint8_t tz_100_lut[100] = {
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static inline int32_t fill_t_4_digits(char *buffer, uint32_t digits, int32_t *ptz)
{
    char *s = buffer;
    uint32_t q = FAST_DIV100(digits);
    uint32_t r = digits - q * 100;

    memcpy(s, &ch_100_lut[q<<1], 2);
    memcpy(s + 2, &ch_100_lut[r<<1], 2);
    *ptz = r ? tz_100_lut[r] : tz_100_lut[q] + 2;
    return 4;
}

static inline int32_t fill_t_8_digits(char *buffer, uint32_t digits, int32_t *ptz)
{
    char *s = buffer;

    if (digits < 10000) {
        memset(s, '0', 4);
        fill_t_4_digits(s + 4, digits, ptz);
    } else {
        uint32_t q = FAST_DIV10000(digits);
        uint32_t r = digits - q * 10000;

        fill_t_4_digits(s, q, ptz);
        if (!r) {
            *ptz += 4;
            memset(s + 4, '0', 4);
        } else {
            fill_t_4_digits(s + 4, r, ptz);
        }
    }

    return 8;
}

static inline int32_t fill_t_16_digits(char *buffer, uint64_t digits, int32_t *ptz)
{
    char *s = buffer;

    if (digits < 100000000llu) {
        memset(s, '0', 8);
        fill_t_8_digits(s + 8, (uint32_t)digits, ptz);
    } else {
        uint32_t q = (uint32_t)(digits / 100000000);
        uint32_t r = (uint32_t)(digits - (uint64_t)q * 100000000);

        fill_t_8_digits(s, q, ptz);
        if (!r) {
            *ptz += 8;
            memset(s + 8, '0', 8);
        } else {
            fill_t_8_digits(s + 8, r, ptz);
        }
    }

    return 16;
}

static inline int32_t fill_1_4_digits(char *buffer, uint32_t digits, int32_t *ptz)
{
    char *s = buffer;

    if (digits < 100) {
        if (digits >= 10) {
            *ptz = tz_100_lut[digits];
            memcpy(s, &ch_100_lut[digits<<1], 2);
            s += 2;
        } else {
            *ptz = 0;
            *s++ = digits + '0';
        }
    } else {
        uint32_t q = FAST_DIV100(digits);
        uint32_t r = digits - q * 100;

        if (q >= 10) {
            *ptz = tz_100_lut[q];
            memcpy(s, &ch_100_lut[q<<1], 2);
            s += 2;
        } else {
            *ptz = 0;
            *s++ = q + '0';
        }

        if (!r) {
            *ptz += 2;
            memset(s, '0', 2);
            s += 2;
        } else {
            *ptz = tz_100_lut[r];
            memcpy(s, &ch_100_lut[r<<1], 2);
            s += 2;
        }
    }

    return (int32_t)(s - buffer);
}

static inline int32_t fill_1_8_digits(char *buffer, uint32_t digits, int32_t *ptz)
{
    char *s = buffer;

    if (digits < 10000) {
        return fill_1_4_digits(s, digits, ptz);
    } else {
        uint32_t q = FAST_DIV10000(digits);
        uint32_t r = digits - q * 10000;

        s += fill_1_4_digits(s, q, ptz);
        if (!r) {
            *ptz += 4;
            memset(s, '0', 4);
            s += 4;
        } else {
            s += fill_t_4_digits(s, r, ptz);
        }
    }

    return (int32_t)(s - buffer);
}

static inline int32_t fill_1_16_digits(char *buffer, uint64_t digits, int32_t *ptz)
{
    char *s = buffer;

    if (digits < 100000000llu) {
        return fill_1_8_digits(s, (uint32_t)digits, ptz);
    } else {
        uint32_t q = (uint32_t)(digits / 100000000);
        uint32_t r = (uint32_t)(digits - (uint64_t)q * 100000000);

        s += fill_1_8_digits(s, q, ptz);
        if (!r) {
            *ptz += 8;
            memset(s, '0', 8);
            s += 8;
        } else {
            s += fill_t_8_digits(s, r, ptz);
        }
    }

    return (int32_t)(s - buffer);
}

static inline int32_t fill_1_20_digits(char *buffer, uint64_t digits, int32_t *ptz)
{
    char *s = buffer;

    if (digits < 10000000000000000llu) {
        return fill_1_16_digits(s, digits, ptz);
    } else {
        uint32_t q = (uint32_t)(digits / 10000000000000000llu);
        uint64_t r = (digits - (uint64_t)q * 10000000000000000llu);

        s += fill_1_4_digits(s, q, ptz);
        if (!r) {
            *ptz += 16;
            memset(s, '0', 16);
            s += 16;
        } else {
            s += fill_t_16_digits(s, r, ptz);
        }
    }

    return (int32_t)(s - buffer);
}

int jnum_itoa(int32_t num, char *buffer)
{
    char *s = buffer;
    uint32_t n = 0;
    int32_t tz = 0;

    if (num == 0) {
        memcpy(s, "0", 2);
        return 1;
    }

    if (num < 0) {
        n = -num;
        *s++ = '-';
    } else {
        n = num;
    }

    if (n < 100000000) {
        s += fill_1_8_digits(s, n, &tz);
    } else {
        uint32_t q = (uint32_t)(((uint64_t)n * 1441151881) >> 57); /* n / 100000000 */
        uint32_t r = n - q * 100000000;

        if (q >= 10) {
            tz = tz_100_lut[q];
            memcpy(s, &ch_100_lut[q<<1], 2);
            s += 2;
        } else {
            tz = 0;
            *s++ = q + '0';
        }

        if (!r) {
            tz += 8;
            memset(s, '0', 8);
            s += 8;
        } else {
            s += fill_t_8_digits(s, r, &tz);
        }
    }

    *s = '\0';
    return (int)(s - buffer);
}

int jnum_ltoa(int64_t num, char *buffer)
{
    char *s = buffer;
    uint64_t n = 0;
    int32_t tz = 0;

    if (num == 0) {
        memcpy(s, "0", 2);
        return 1;
    }

    if (num < 0) {
        n = -num;
        *s++ = '-';
    } else {
        n = num;
    }

    s += fill_1_20_digits(s, n, &tz);

    *s = '\0';
    return (int)(s - buffer);
}

static const char hex_char_lut[] = {
    '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
};

static inline int fill_t_2_hexs(char *buffer, uint32_t num)
{
    char *s = buffer;
    *s++ = hex_char_lut[num >> 4];
    *s++ = hex_char_lut[num & 0xf];
    return 2;
}

static inline int fill_t_4_hexs(char *buffer, uint32_t num)
{
    char *s = buffer;

    fill_t_2_hexs(s, num >> 8);
    fill_t_2_hexs(s + 2, num & 0xff);
    return 4;
}

static inline int fill_t_8_hexs(char *buffer, uint32_t num)
{
    char *s = buffer;

    fill_t_4_hexs(s, num >> 16);
    fill_t_4_hexs(s + 4, num & 0xffff);
    return 8;
}

static inline int fill_1_2_hexs(char *buffer, uint32_t num)
{
    char *s = buffer;
    uint32_t q = num >> 4;
    uint32_t r = num & 0xf;

    if (q)
        *s++ = hex_char_lut[q];
    *s++ = hex_char_lut[r];
    return (int)(s - buffer);
}

static inline int fill_1_4_hexs(char *buffer, uint32_t num)
{
    char *s = buffer;
    uint32_t q = num >> 8;
    uint32_t r = num & 0xff;

    if (!q) {
        return fill_1_2_hexs(s, num);
    } else {
        s += fill_1_2_hexs(s, q);
        s += fill_t_2_hexs(s, r);
    }
    return (int)(s - buffer);
}

static inline int fill_1_8_hexs(char *buffer, uint32_t num)
{
    char *s = buffer;
    uint32_t q = num >> 16;
    uint32_t r = num & 0xffff;

    if (!q) {
        return fill_1_4_hexs(s, num);
    } else {
        s += fill_1_4_hexs(s, q);
        s += fill_t_4_hexs(s, r);
    }
    return (int)(s - buffer);
}

int jnum_htoa(uint32_t num, char *buffer)
{
    char *s = buffer;

    *s++ = '0';
    *s++ = 'x';
    s += fill_1_8_hexs(s, num);

    *s = '\0';
    return (int)(s - buffer);
}

int jnum_lhtoa(uint64_t num, char *buffer)
{
    char *s = buffer;
    uint32_t q = (uint32_t)(num >> 32);
    uint32_t r = (uint32_t)(num & 0xffffffff);

    *s++ = '0';
    *s++ = 'x';
    if (!q) {
        s += fill_1_8_hexs(s, r);
    } else {
        s += fill_1_8_hexs(s, q);
        s += fill_t_8_hexs(s, r);
    }

    *s = '\0';
    return (int)(s - buffer);
}

/*
# python to get lut

def print_pow10(step):
    minv = 0
    nunit = 0
    nexpm = 0

    maxv = 0
    punit = 0
    pexpm = 0

    if step == 8:
        # compute negative array
        # 1 * (2 ** -8) = 0.0390625 * (10 ** -1)
        # 390625 = 0.0390625 * (10 ** 7)
        # 136 = (1022 + 52 + 11) / 8 + 1
        minv = -136
        nunit = 390625
        nexpm = 7
        # compute positive array
        # 1 * (2 ** 8) = 25.6 * (10 ** 1)
        # 256 = 25.6 * (10 ** 1)
        # 122 = (1023 - 52) / 8 + 1
        maxv = 122
        punit = 256
        pexpm = 1
    elif step == 4:
        # compute negative array
        # 1 * (2 ** -4) = 0.625 * (10 ** -1)
        # 625 = 0.625 * (10 ** 3)
        # 272 = (1022 + 52 + 11) / 4 + 1
        minv = -272
        nunit = 625
        nexpm = 3
        # compute positive array
        # 1 * (2 ** 4) = 1.6 * (10 ** 1)
        # 16 = 1.6 * (10 ** 1)
        # 243 = (1023 - 52) / 4 + 1
        maxv = 243
        punit = 16
        pexpm = 1
    elif step == 2:
        # compute negative array
        # 1 * (2 ** -2) = 2.5 * (10 ** -1)
        # 25 = 2.5 * (10 ** 1)
        # 543 = (1022 + 52 + 11) / 2 + 1
        minv = -543
        nunit = 25
        nexpm = 1
        # compute positive array
        # 1 * (2 ** 2) = 0.4 * (10 ** 1)
        # 4 = 0.4 * (10 ** 1)
        # 486 = (1023 - 52) / 2 + 1
        maxv = 486
        punit = 4
        pexpm = 1
    else:
        print('Only step 8 or 4 or 2 are supported.')
        return -1

    mul_lut = []
    exp_lut = []
    pos = 0
    for i in range(minv, maxv + 1):
        if i < 0:
            j = -i
            unit = nunit
            expm = nexpm
        else:
            j = i
            unit = punit
            expm = pexpm

        val = unit ** j
        bit = len(str(val))
        mul = (1 << 64) * val // (10 ** bit)
        rem = (1 << 64) * val % (10 ** bit)
        if rem:
            mul += 1
        exp = j * expm - bit

        mul_lut.append(mul)
        if step == 8:
            if exp < 0:
                exp_lut.append(-exp)
                if pos == 0:
                    pos = i - minv
                    print('#define POW10_EXP_LUT_POS    %d // From the pos, the value of pow10_exp_lut is negitive.' % (pos))
            else:
                exp_lut.append(exp)

        elif step == 4:
            exp_lut.append(exp)
        elif step == 2:
            if exp < 0:
                exp_lut.append(-exp)
            else:
                exp_lut.append(exp)
                if pos == 0:
                    pos = i - minv
                    print('#define POW10_EXP_LUT_POS    %d // From the pos, the value of pow10_exp_lut is positive.' % (pos))

    print('#define POW10_LUT_MIN_IDX    %d' % (minv))
    print('#define POW10_LUT_MAX_IDX    %d' % (maxv))
    last_idx = maxv - minv

    print('static const uint64_t pow10_mul_lut[POW10_LUT_MAX_IDX - POW10_LUT_MIN_IDX + 1] = {', end='')
    for i in range(last_idx + 1):
        if i % 5 == 0:
            print()
            print('    ', end='')
        print('0x%016x' % (mul_lut[i]), end='')
        if i != last_idx:
            print(', ', end='')
        else:
            print()
            print('};')

    print()
    print('static const %s pow10_exp_lut[POW10_LUT_MAX_IDX - POW10_LUT_MIN_IDX + 1] = {' % ('int8_t' if step == 4 else 'uint8_t'), end='')
    for i in range(last_idx + 1):
        if i % 20 == 0:
            print()
            print('    ', end='')
        print('%-3d' % (exp_lut[i]), end='')

        if i != last_idx:
            print(', ', end='')
        else:
            print()
            print('};')

print('#if USING_SMALL_LUT')
print_pow10(8)
print('\n#else\n')
print_pow10(4)
print('#endif')
*/

#if USING_SMALL_LUT
#define POW10_EXP_LUT_POS    136 // From the pos, the value of pow10_exp_lut is negitive.
#define POW10_LUT_MIN_IDX    -136
#define POW10_LUT_MAX_IDX    122
static const uint64_t pow10_mul_lut[POW10_LUT_MAX_IDX - POW10_LUT_MIN_IDX + 1] = {
    0x4d32a036a252e482, 0xc5a05277621be294, 0x3297a26c62d808db, 0x81842f29f2cce376, 0x2127fbb0a075fccb,
    0x54e13ca571d1e34e, 0xd94ad8b1c7380875, 0x37a0790280d2f3d4, 0x8e679c2f5e44ff90, 0x2474a2dd05b374a0,
    0x5d538c7341cb67ff, 0xeeea5d5004981479, 0x3d2990b8531898b3, 0x9c935e00d4b9d8d3, 0x2815578d865470da,
    0x669d0918621fd938, 0x1a44df832b8d45f2, 0x433facd4ea5f6b61, 0xac2820d9623bf42a, 0x2c1277005aaf1798,
    0x70d31c29dde93229, 0x1ce2137f74338194, 0x49f0d5c129799da3, 0xbd49d14aa79dbc83, 0x30753387d8fd5f5d,
    0x7c0d50b7ee0dc0ee, 0x1fc1df6a7a61ba9b, 0x514c796280fa2fa2, 0xd01fef10a657842d, 0x3547a9bea15e158d,
    0x8865899617fb1872, 0x22eae3bbed902707, 0x59638eade54811fd, 0xe4d5e82392a40516, 0x3a94f7d7f4635545,
    0x95f83d0a1fb69cda, 0x266469bcf5afc5da, 0x6248bcc5045156a8, 0xfb9b7cd9a4a7443d, 0x40695741f4e73c7a,
    0xa4e4b66b68b65d61, 0x2a367454d738ece6, 0x6c1085f7e9877d2e, 0x1baa1e332d728ea4, 0x46d238d4ef39bf13,
    0xb54d5e4a127f59c9, 0x2e69d2818df38bb9, 0x76d1770e38320987, 0x1e6adefd7f06aa60, 0x4dde63d0a158be66,
    0xc75809c42c684dd2, 0x330833a6f4d71e2a, 0x82a45b450226b39d, 0x2171c159589d5d16, 0x559e17eef755692e,
    0xdb2e51bfe9d0696b, 0x381c3de34e49d55b, 0x8fa475791a569d11, 0x24c5bfdd7761f2f7, 0x5e2332dacb38308b,
    0xf0fdf2d3f3c30ba0, 0x3db1a69ca8e627d7, 0x9defbf01b061adac, 0x286e86e9e7858cb8, 0x678159610903f798,
    0x1a7f5245e5a2cebf, 0x43d54d7fbc821144, 0xada72ccc20054aea, 0x2c7486591eb9acc8, 0x71ce24bb2fefcecb,
    0x1d22573a28f19d63, 0x4a955a2e7d4bd05a, 0xbeeefb584aff8604, 0x30e104f3c978b5c4, 0x7d21545b9d5dfa47,
    0x200888489af9569a, 0x52015ce2d469d374, 0xd1ef0244af236500, 0x35be35d424a4b581, 0x899504ae72497ebb,
    0x233894a789cd2ec8, 0x5a2a7250bcee8c3c, 0xe6d3102ad96cec1e, 0x3b174fea33909ec0, 0x9745eb4d50ce6333,
    0x26b9d5d65a5181d8, 0x63236b1a80d0a88f, 0xfdcb4fa002162a64, 0x40f8a7d70ac62fb8, 0xa6539930bf6bff46,
    0x2a94608f8d29fbb8, 0x6d00f7320d3846f5, 0x1be7abd3781eca7d, 0x476fcc5acd1b9ff7, 0xb6e0c377cfa2e12f,
    0x2ed1176a72984a08, 0x77d9d58b62cd8a52, 0x1eae8caef261aca1, 0x4e8ba596e760723e, 0xc913936dd571c84d,
    0x3379bf57826af3ca, 0x83c7088e1aab65dc, 0x21bc2b266d3a36c0, 0x565c976c9cbdfccc, 0xdd15fe86affad913,
    0x3899162693736ac6, 0x90e40fbeea1d3a4b, 0x25179157c93ec73f, 0x5ef4a74721e86477, 0xf316271c7fc3908b,
    0x3e3aeb4ae1383563, 0x9f4f2726179a2246, 0x28c87cb5c89a2572, 0x6867a5a867f103b3, 0x1aba4714957d300e,
    0x446c3b15f9926688, 0xaf298d050e4395d7, 0x2cd76fe086b93ce3, 0x72cb5bd86321e38d, 0x1d6329f1c35ca4c0,
    0x4b3b4ca85a86c47b, 0xc097ce7bc90715b4, 0x314dc6448d9338c2, 0x7e37be2022c0914c, 0x204fce5e3e250262,
    0x52b7d2dcc80cd2e4, 0xd3c21bcecceda100, 0x3635c9adc5dea000, 0x8ac7230489e80000, 0x2386f26fc1000000,
    0x5af3107a40000000, 0xe8d4a51000000000, 0x3b9aca0000000000, 0x9896800000000000, 0x2710000000000000,
    0x6400000000000000, 0x199999999999999a, 0x4189374bc6a7ef9e, 0xa7c5ac471b478424, 0x2af31dc4611873c0,
    0x6df37f675ef6eae0, 0x1c25c268497681c3, 0x480ebe7b9d58566d, 0xb877aa3236a4b44a, 0x2f394219248446bb,
    0x78e480405d7b9659, 0x1ef2d0f5da7dd8ab, 0x4f3a68dbc8f03f25, 0xcad2f7f5359a3b3f, 0x33ec47ab514e652f,
    0x84ec3c97da624ab5, 0x22073a8515171d5e, 0x571cbec554b60dbc, 0xdf01e85f912e37a4, 0x391704310a8acec2,
    0x9226712162ab070e, 0x256a18dd89e626ac, 0x5fc7edbc424d2fcc, 0xf53304714d9265e0, 0x3ec56164af81a34c,
    0xa0b19d2ab70e6ed7, 0x29233aaaadd6a38b, 0x694ff258c7443208, 0x1af5bf109550f22f, 0x4504787c5f878ab6,
    0xb0af48ec79ace838, 0x2d3b357c0692aa0b, 0x73cac65c39c96162, 0x1da48ce468e7c703, 0x4be2b05d35848cd3,
    0xc24452da229b021c, 0x31bb798fe8174c51, 0x7f50935bebc0c35f, 0x2097b309321cde0c, 0x536fdecfdc72dc48,
    0xd59944a37c0752a3, 0x36ae679665622172, 0x8bfbea76c619ef37, 0x23d5fe9530aa7aae, 0x5bbd6d030bf1dde6,
    0xeadab0aba3b2dbe6, 0x3c1f689ea0b0dc23, 0x99ea0196163fa42f, 0x2766e9e0ca4dbb71, 0x64de7fb01a60982a,
    0x19d28f47b4d524e8, 0x421b0865a5f8b066, 0xa93af6c6c79b5d2e, 0x2b52adc44bace4a8, 0x6ee8233e325e7251,
    0x1c6463225ab7ec1d, 0x48af1243779966b1, 0xba121a4650e4ddec, 0x2fa2548ce182451a, 0x79f17c49ef61f894,
    0x1f37ad21436d0c70, 0x4feab0f8fe87cdea, 0xcc963fee10b7d1b4, 0x345fced47f28e9e9, 0x8613fd0145877586,
    0x2252f0e5b39769dd, 0x57de91a832277568, 0xe0f218b8d25088b9, 0x39960a6cc11ac2bf, 0x936b9fcebb25c996,
    0x25bd5803c569edfa, 0x609d0a4718196b74, 0xf7549530e188c129, 0x3f510b91a22f4c13, 0xa21727db38cb0030,
    0x297ec285f1ddf3c3, 0x6a3a43e642383296, 0x1b31bb5dc320d18f, 0x459e089e1c7cf9c0, 0xb23867fb2a35b28e,
    0x2d9fd9154a4b2fc3, 0x74cc692c434fd66c, 0x1de6815302e5559d, 0x4c8b888296c5f9e3, 0xc3f490aa77bd60fd,
    0x322a20f03f6bdf7d, 0x806bd9714632dff7, 0x20e037aa4f692f19, 0x542984435aa6def6, 0xd77485cb25823ac8,
    0x372811ddfd50715a, 0x8d3360f09cf6e4be, 0x2425ba9bce122614, 0x5c898bcc4cfb42c3, 0xece53cec4a314ebe,
    0x3ca52e50f85e8af2, 0x9b407691d7fc44f9, 0x27be952349b969b9, 0x65beee6ed136d135, 0x1a0c03b1df8af612,
    0x42ae1df050bfe694, 0xaab37fd7d8f58179, 0x2bb31264501e14dc, 0x6fdee76733803565, 0x1ca38f350b22de91,
    0x4950cac53b3a8bb0, 0xbbb01b9283253ca3, 0x300c50c958de864f, 0x7b00ced03faa4d96, 0x1f7d228322baf525,
    0x509c814fb511cfba, 0xce5d73ff402d98e4, 0x34d4570a0c5566a1, 0x873e4f75e2224e69, 0x229f4fbbdfc73f15,
    0x58a213cc7a4ffda6, 0xe2e69915b3fff9fa, 0x3a162b4923d708b3, 0x94b3a202eb1c3f3a, 0x261150630d159136,
    0x617400fd9222bb6b, 0xf97ae3d0d2446f26, 0x3fddec7f2faf3714, 0xa37fce126597973d
};

static const uint8_t pow10_exp_lut[POW10_LUT_MAX_IDX - POW10_LUT_MIN_IDX + 1] = {
    191, 190, 188, 187, 185, 184, 183, 181, 180, 178, 177, 176, 174, 173, 171, 170, 168, 167, 166, 164,
    163, 161, 160, 159, 157, 156, 154, 153, 152, 150, 149, 147, 146, 145, 143, 142, 140, 139, 138, 136,
    135, 133, 132, 130, 129, 128, 126, 125, 123, 122, 121, 119, 118, 116, 115, 114, 112, 111, 109, 108,
    107, 105, 104, 102, 101, 99 , 98 , 97 , 95 , 94 , 92 , 91 , 90 , 88 , 87 , 85 , 84 , 83 , 81 , 80 ,
    78 , 77 , 76 , 74 , 73 , 71 , 70 , 69 , 67 , 66 , 64 , 63 , 61 , 60 , 59 , 57 , 56 , 54 , 53 , 52 ,
    50 , 49 , 47 , 46 , 45 , 43 , 42 , 40 , 39 , 38 , 36 , 35 , 33 , 32 , 30 , 29 , 28 , 26 , 25 , 23 ,
    22 , 21 , 19 , 18 , 16 , 15 , 14 , 12 , 11 , 9  , 8  , 7  , 5  , 4  , 2  , 1  , 1  , 2  , 3  , 5  ,
    6  , 8  , 9  , 10 , 12 , 13 , 15 , 16 , 17 , 19 , 20 , 22 , 23 , 24 , 26 , 27 , 29 , 30 , 31 , 33 ,
    34 , 36 , 37 , 39 , 40 , 41 , 43 , 44 , 46 , 47 , 48 , 50 , 51 , 53 , 54 , 55 , 57 , 58 , 60 , 61 ,
    62 , 64 , 65 , 67 , 68 , 70 , 71 , 72 , 74 , 75 , 77 , 78 , 79 , 81 , 82 , 84 , 85 , 86 , 88 , 89 ,
    91 , 92 , 93 , 95 , 96 , 98 , 99 , 100, 102, 103, 105, 106, 108, 109, 110, 112, 113, 115, 116, 117,
    119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 134, 136, 137, 139, 140, 141, 143, 144, 146,
    147, 148, 150, 151, 153, 154, 155, 157, 158, 160, 161, 162, 164, 165, 167, 168, 169, 171, 172
};

#else

#define POW10_LUT_MIN_IDX    -272
#define POW10_LUT_MAX_IDX    243
static const uint64_t pow10_mul_lut[POW10_LUT_MAX_IDX - POW10_LUT_MIN_IDX + 1] = {
    0x4d32a036a252e482, 0x7b84338a9d516d9d, 0xc5a05277621be294, 0x1f9ec583bdc70589, 0x3297a26c62d808db,
    0x50f29d7a37c00e2a, 0x81842f29f2cce376, 0xcf39e50feae16bf0, 0x2127fbb0a075fccb, 0x350cc5e767232e11,
    0x54e13ca571d1e34e, 0x87cec76f1c830549, 0xd94ad8b1c7380875, 0x22c44ba19083d865, 0x37a0790280d2f3d4,
    0x5900c19d9aeb1fba, 0x8e679c2f5e44ff90, 0xe3d8f9e563a198e6, 0x2474a2dd05b374a0, 0x3a5437c8091f2100,
    0x5d538c7341cb67ff, 0x95527a5202df0ccc, 0xeeea5d5004981479, 0x2639fa7333ef5f70, 0x3d2990b8531898b3,
    0x61dc1ac084f42784, 0x9c935e00d4b9d8d3, 0xfa856334878fc151, 0x2815578d865470da, 0x402225af3d53e7c3,
    0x669d0918621fd938, 0xa42e74f3d032f526, 0x1a44df832b8d45f2, 0x2a07cc05127ba31d, 0x433facd4ea5f6b61,
    0x6b991487dd65789a, 0xac2820d9623bf42a, 0x1b8b8a6038ad6ebf, 0x2c1277005aaf1798, 0x4683f19a2ab1bf5a,
    0x70d31c29dde93229, 0xb484f9dc9641e9db, 0x1ce2137f74338194, 0x2e368598b9ec0286, 0x49f0d5c129799da3,
    0x764e22cea8c295d2, 0xbd49d14aa79dbc83, 0x1e494034e79e5b9a, 0x30753387d8fd5f5d, 0x4d885272f4c89895,
    0x7c0d50b7ee0dc0ee, 0xc67bb4597ce2ce49, 0x1fc1df6a7a61ba9b, 0x32cfcbdd909c5dc5, 0x514c796280fa2fa2,
    0x8213f56a67f6b29c, 0xd01fef10a657842d, 0x214cca1724dacd78, 0x3547a9bea15e158d, 0x553f75fdcefcef47,
    0x8865899617fb1872, 0xda3c0f568cc4f3e9, 0x22eae3bbed902707, 0x37de392caf4d0b3e, 0x59638eade54811fd,
    0x8f05b1163ba6832e, 0xe4d5e82392a40516, 0x249d1ae6f8be154c, 0x3a94f7d7f4635545, 0x5dbb262653d22208,
    0x95f83d0a1fb69cda, 0xeff394dcff8a948f, 0x266469bcf5afc5da, 0x3d6d75fb22b2d629, 0x6248bcc5045156a8,
    0x9d412e0806e88aa6, 0xfb9b7cd9a4a7443d, 0x2841d689391085cd, 0x40695741f4e73c7a, 0x670ef2032171fa5d,
    0xa4e4b66b68b65d61, 0x1a6208b506839410, 0x2a367454d738ece6, 0x438a53baf1f4ae3d, 0x6c1085f7e9877d2e,
    0xace73cbfdc0bfb7c, 0x1baa1e332d728ea4, 0x2c4363851584176c, 0x46d238d4ef39bf13, 0x71505aee4b8f981e,
    0xb54d5e4a127f59c9, 0x1d022390f8b83754, 0x2e69d2818df38bb9, 0x4a42ea68e31f45f4, 0x76d1770e38320987,
    0xbe1bf1b059e9a8d7, 0x1e6adefd7f06aa60, 0x30aafe6264d77700, 0x4dde63d0a158be66, 0x7c97061a9bc130a3,
    0xc75809c42c684dd2, 0x1fe52048590672da, 0x330833a6f4d71e2a, 0x51a6b90b21583043, 0x82a45b450226b39d,
    0xd106f86e69d785c8, 0x2171c159589d5d16, 0x3582cef55a9561bd, 0x559e17eef755692e, 0x88fcf317f22241e3,
    0xdb2e51bfe9d0696b, 0x2311a6ae10ee2559, 0x381c3de34e49d55b, 0x59c6c96bb076222b, 0x8fa475791a569d11,
    0xe5d3ef282a242e82, 0x24c5bfdd7761f2f7, 0x3ad5ffc8bf031e57, 0x5e2332dacb38308b, 0x969eb7c47859e744,
    0xf0fdf2d3f3c30ba0, 0x268f0821e98fd8e7, 0x3db1a69ca8e627d7, 0x62b5d7610e3d0c8c, 0x9defbf01b061adac,
    0xfcb2cb35e702af79, 0x286e86e9e7858cb8, 0x40b0d7dca5a27abf, 0x678159610903f798, 0xa59bc234db398c26,
    0x1a7f5245e5a2cebf, 0x2a65506fd5d14acb, 0x43d54d7fbc821144, 0x6c887bff94034ed3, 0xada72ccc20054aea,
    0x1bc8d3f7b3340bfd, 0x2c7486591eb9acc8, 0x4720d6f4fdf5e13f, 0x71ce24bb2fefcecb, 0xb616a12b7fe617ab,
    0x1d22573a28f19d63, 0x2e9d585d0e4f6238, 0x4a955a2e7d4bd05a, 0x77555d172edfb3c3, 0xbeeefb584aff8604,
    0x1e8ca3185deb719b, 0x30e104f3c978b5c4, 0x4e34d4b9425abc6c, 0x7d21545b9d5dfa47, 0xc83553c5c8965d3e,
    0x200888489af9569a, 0x3340da0dc4c22429, 0x52015ce2d469d374, 0x8335616aed761f20, 0xd1ef0244af236500,
    0x2196e1a496e6f171, 0x35be35d424a4b581, 0x55fd22ed076def35, 0x899504ae72497ebb, 0xdc21a1171d42645e,
    0x233894a789cd2ec8, 0x385a8772761517a6, 0x5a2a7250bcee8c3c, 0x9043ea1ac7e41393, 0xe6d3102ad96cec1e,
    0x24ee91f2603a6338, 0x3b174fea33909ec0, 0x5e8bb3105280fe00, 0x9745eb4d50ce6333, 0xf209787bb47d6b85,
    0x26b9d5d65a5181d8, 0x3df622f09082695a, 0x63236b1a80d0a88f, 0x9e9f11c4014dda7f, 0xfdcb4fa002162a64,
    0x289b68e666bbddd3, 0x40f8a7d70ac62fb8, 0x67f43fbe77a37f8c, 0xa6539930bf6bff46, 0x1a9cbc59b83a3d53,
    0x2a94608f8d29fbb8, 0x44209a7f48432c5a, 0x6d00f7320d3846f5, 0xae67f1e9aec07188, 0x1be7abd3781eca7d,
    0x2ca5dfb8c03143fa, 0x476fcc5acd1b9ff7, 0x724c7a2ae1c5ccbe, 0xb6e0c377cfa2e12f, 0x1d42aea2879f2e45,
    0x2ed1176a72984a08, 0x4ae825771dc07673, 0x77d9d58b62cd8a52, 0xbfc2ef456ae276e9, 0x1eae8caef261aca1,
    0x3117477e509c4767, 0x4e8ba596e760723e, 0x7dac3c24a5671d30, 0xc913936dd571c84d, 0x202c1796b182d85f,
    0x3379bf57826af3ca, 0x525c6558d0ab1faa, 0x83c7088e1aab65dc, 0xd2d80db02aabd62c, 0x21bc2b266d3a36c0,
    0x35f9dea3e1f6bdff, 0x565c976c9cbdfccc, 0x8a2dbf142dfcc7ac, 0xdd15fe86affad913, 0x235fadd81c2822bc,
    0x3899162693736ac6, 0x5a8e89d75252446f, 0x90e40fbeea1d3a4b, 0xe7d34c64a9c85d45, 0x25179157c93ec73f,
    0x3b58e88c75313eca, 0x5ef4a74721e86477, 0x97edd871cfda3a57, 0xf316271c7fc3908b, 0x26e4d30eccc3215e,
    0x3e3aeb4ae1383563, 0x63917877cec0556c, 0x9f4f2726179a2246, 0xfee50b7025c36a09, 0x28c87cb5c89a2572,
    0x4140c78940f6a250, 0x6867a5a867f103b3, 0xa70c3c40a64e6c52, 0x1aba4714957d300e, 0x2ac3a4edbbfb8015,
    0x446c3b15f9926688, 0x6d79f82328ea3da7, 0xaf298d050e4395d7, 0x1c06a5ec5433c60e, 0x2cd76fe086b93ce3,
    0x47bf19673df52e38, 0x72cb5bd86321e38d, 0xb7abc627050305ae, 0x1d6329f1c35ca4c0, 0x2f050fe938943acd,
    0x4b3b4ca85a86c47b, 0x785ee10d5da46d91, 0xc097ce7bc90715b4, 0x1ed09bead87c0379, 0x314dc6448d9338c2,
    0x4ee2d6d415b85acf, 0x7e37be2022c0914c, 0xc9f2c9cd04674edf, 0x204fce5e3e250262, 0x33b2e3c9fd0803cf,
    0x52b7d2dcc80cd2e4, 0x84595161401484a0, 0xd3c21bcecceda100, 0x21e19e0c9bab2400, 0x3635c9adc5dea000,
    0x56bc75e2d6310000, 0x8ac7230489e80000, 0xde0b6b3a76400000, 0x2386f26fc1000000, 0x38d7ea4c68000000,
    0x5af3107a40000000, 0x9184e72a00000000, 0xe8d4a51000000000, 0x2540be4000000000, 0x3b9aca0000000000,
    0x5f5e100000000000, 0x9896800000000000, 0xf424000000000000, 0x2710000000000000, 0x3e80000000000000,
    0x6400000000000000, 0xa000000000000000, 0x199999999999999a, 0x28f5c28f5c28f5c3, 0x4189374bc6a7ef9e,
    0x68db8bac710cb296, 0xa7c5ac471b478424, 0x1ad7f29abcaf4858, 0x2af31dc4611873c0, 0x44b82fa09b5a52cc,
    0x6df37f675ef6eae0, 0xafebff0bcb24aaff, 0x1c25c268497681c3, 0x2d09370d42573604, 0x480ebe7b9d58566d,
    0x734aca5f6226f0ae, 0xb877aa3236a4b44a, 0x1d83c94fb6d2ac35, 0x2f394219248446bb, 0x4b8ed0283a6d3df8,
    0x78e480405d7b9659, 0xc16d9a0095928a28, 0x1ef2d0f5da7dd8ab, 0x318481895d962777, 0x4f3a68dbc8f03f25,
    0x7ec3daf941806507, 0xcad2f7f5359a3b3f, 0x2073accb12d0ff3e, 0x33ec47ab514e652f, 0x5313a5dee87d6eb1,
    0x84ec3c97da624ab5, 0xd4ad2dbfc3d07788, 0x22073a8515171d5e, 0x3671f73b54f1c896, 0x571cbec554b60dbc,
    0x8b61313bbabce2c7, 0xdf01e85f912e37a4, 0x23ae629ea696c139, 0x391704310a8acec2, 0x5b5806b4ddaae469,
    0x9226712162ab070e, 0xe9d71b689dde71b0, 0x256a18dd89e626ac, 0x3bdcf495a9703de0, 0x5fc7edbc424d2fcc,
    0x993fe2c6d07b7fac, 0xf53304714d9265e0, 0x273b5cdeedb10610, 0x3ec56164af81a34c, 0x646f023ab2690546,
    0xa0b19d2ab70e6ed7, 0x19b604aaaca62637, 0x29233aaaadd6a38b, 0x41d1f7777c8a9f45, 0x694ff258c7443208,
    0xa87fea27a539e9a6, 0x1af5bf109550f22f, 0x2b22cb4dbbb4b6b2, 0x4504787c5f878ab6, 0x6e6d8d93cc0c1123,
    0xb0af48ec79ace838, 0x1c45016d841baa47, 0x2d3b357c0692aa0b, 0x485ebbf9a41ddcdd, 0x73cac65c39c96162,
    0xb94470938fa89bcf, 0x1da48ce468e7c703, 0x2f6dae3a4172d804, 0x4be2b05d35848cd3, 0x796ab3c855a0e152,
    0xc24452da229b021c, 0x1f152bf9f10e8fb3, 0x31bb798fe8174c51, 0x4f925c1973587a1c, 0x7f50935bebc0c35f,
    0xcbb41ef979346bcb, 0x2097b309321cde0c, 0x3425eb41e9c7c9ad, 0x536fdecfdc72dc48, 0x857fcae62d8493a6,
    0xd59944a37c0752a3, 0x222d00bdff5d54e7, 0x36ae679665622172, 0x577d728a3bd03582, 0x8bfbea76c619ef37,
    0xdff9772470297ebe, 0x23d5fe9530aa7aae, 0x39566421e7772ab0, 0x5bbd6d030bf1dde6, 0x92c8ae6b464fc970,
    0xeadab0aba3b2dbe6, 0x2593a163246e8996, 0x3c1f689ea0b0dc23, 0x603240fdcde7c69d, 0x99ea0196163fa42f,
    0xf64335bcf065d37e, 0x2766e9e0ca4dbb71, 0x3f0b0fce107c5f1a, 0x64de7fb01a60982a, 0xa163ff802a3426a9,
    0x19d28f47b4d524e8, 0x2950e53f87bb6e40, 0x421b0865a5f8b066, 0x69c4da3c3cc11a3d, 0xa93af6c6c79b5d2e,
    0x1b13ac9aaf4c0ee9, 0x2b52adc44bace4a8, 0x45511606df7b0773, 0x6ee8233e325e7251, 0xb1736b96b6fd83b4,
    0x1c6463225ab7ec1d, 0x2d6d6b6a2abfe02f, 0x48af1243779966b1, 0x744b506bf28f0ab4, 0xba121a4650e4ddec,
    0x1dc574d80cf16b30, 0x2fa2548ce182451a, 0x4c36edae359d3b5c, 0x79f17c49ef61f894, 0xc31bfa0fe5698db9,
    0x1f37ad21436d0c70, 0x31f2ae9b9f14e0b3, 0x4feab0f8fe87cdea, 0x7fdde7f4ca72e310, 0xcc963fee10b7d1b4,
    0x20bbe144cf799232, 0x345fced47f28e9e9, 0x53cc7e20cb74a974, 0x8613fd0145877586, 0xd686619ba27255a3,
    0x2252f0e5b39769dd, 0x36eb1b091f58a961, 0x57de91a832277568, 0x8c974f7383725574, 0xe0f218b8d25088b9,
    0x23fdc683f8b0b9b8, 0x39960a6cc11ac2bf, 0x5c2343e134f79dfe, 0x936b9fcebb25c996, 0xebdf661791d60f57,
    0x25bd5803c569edfa, 0x3c62266c6f0fe329, 0x609d0a4718196b74, 0x9a94dd3e8cf578ba, 0xf7549530e188c129,
    0x2792a73b055d8f8c, 0x3f510b91a22f4c13, 0x654e78e9037ee01e, 0xa21727db38cb0030, 0x19ef3993b72ab85a,
    0x297ec285f1ddf3c3, 0x42646a6fe9631f9e, 0x6a3a43e642383296, 0xa9f6d30a038d1dbd, 0x1b31bb5dc320d18f,
    0x2b82c562d1ce1c18, 0x459e089e1c7cf9c0, 0x6f6340fcfa618f99, 0xb23867fb2a35b28e, 0x1c83e7ad4e6efdda,
    0x2d9fd9154a4b2fc3, 0x48ffc1bbaa11e604, 0x74cc692c434fd66c, 0xbae0a846d2195713, 0x1de6815302e5559d,
    0x2fd735519e3bbc2e, 0x4c8b888296c5f9e3, 0x7a78da6a8ad65c9e, 0xc3f490aa77bd60fd, 0x1f5a549627a36bae,
    0x322a20f03f6bdf7d, 0x504367e6cbdfcbfa, 0x806bd9714632dff7, 0xcd795be870516657, 0x20e037aa4f692f19,
    0x3499f2aa18a84b5a, 0x542984435aa6def6, 0x86a8d39ef77164bd, 0xd77485cb25823ac8, 0x22790b2abe5246d9,
    0x372811ddfd50715a, 0x58401c96621a4ef7, 0x8d3360f09cf6e4be, 0xe1ebce4dc7f16dfc, 0x2425ba9bce122614,
    0x39d5f75fb01d09ba, 0x5c898bcc4cfb42c3, 0x940f4613ae5ed137, 0xece53cec4a314ebe, 0x25e73cf29b3b16d7,
    0x3ca52e50f85e8af2, 0x61084a1b26fdab1c, 0x9b407691d7fc44f9, 0xf867241c8cc6d4c1, 0x27be952349b969b9,
    0x3f97550542c242c1, 0x65beee6ed136d135, 0xa2cb1717b52481ee, 0x1a0c03b1df8af612, 0x29acd2b63277f01c,
    0x42ae1df050bfe694, 0x6ab02fe6e79970ec, 0xaab37fd7d8f58179, 0x1b4feb7eb212cd0a, 0x2bb31264501e14dc,
    0x45eb50a08030215f, 0x6fdee76733803565, 0xb2fe3f0b8599ef08, 0x1ca38f350b22de91, 0x2dd27ebb4504974e,
    0x4950cac53b3a8bb0, 0x754e113b91f745e6, 0xbbb01b9283253ca3, 0x1e07b27dd78b13f2, 0x300c50c958de864f,
    0x4ce0814227ca707e, 0x7b00ced03faa4d96, 0xc4ce17b399107c23, 0x1f7d228322baf525, 0x3261d0d1d12b21d4,
    0x509c814fb511cfba, 0x80fa687f881c7f8f, 0xce5d73ff402d98e4, 0x2104b66647b56025, 0x34d4570a0c5566a1,
    0x5486f1a9ad557102, 0x873e4f75e2224e69, 0xd863b256369d4a41, 0x229f4fbbdfc73f15, 0x37654c5fcc71fe88,
    0x58a213cc7a4ffda6, 0x8dd01fad907ffc3c, 0xe2e69915b3fff9fa, 0x244ddb0db6666570, 0x3a162b4923d708b3,
    0x5cf04541d2f1a784, 0x94b3a202eb1c3f3a, 0xedec366b11c6cb90, 0x261150630d159136, 0x3ce8809e7b55b523,
    0x617400fd9222bb6b, 0x9becce62836ac578, 0xf97ae3d0d2446f26, 0x27eab3cf7dcd826d, 0x3fddec7f2faf3714,
    0x662fe0cb7f7ebe87
};

static const int8_t pow10_exp_lut[POW10_LUT_MAX_IDX - POW10_LUT_MIN_IDX + 1] = {
    55 , 55 , 55 , 54 , 54 , 54 , 54 , 54 , 53 , 53 , 53 , 53 , 53 , 52 , 52 , 52 , 52 , 52 , 51 , 51 ,
    51 , 51 , 51 , 50 , 50 , 50 , 50 , 50 , 49 , 49 , 49 , 49 , 48 , 48 , 48 , 48 , 48 , 47 , 47 , 47 ,
    47 , 47 , 46 , 46 , 46 , 46 , 46 , 45 , 45 , 45 , 45 , 45 , 44 , 44 , 44 , 44 , 44 , 43 , 43 , 43 ,
    43 , 43 , 42 , 42 , 42 , 42 , 42 , 41 , 41 , 41 , 41 , 41 , 40 , 40 , 40 , 40 , 40 , 39 , 39 , 39 ,
    39 , 38 , 38 , 38 , 38 , 38 , 37 , 37 , 37 , 37 , 37 , 36 , 36 , 36 , 36 , 36 , 35 , 35 , 35 , 35 ,
    35 , 34 , 34 , 34 , 34 , 34 , 33 , 33 , 33 , 33 , 33 , 32 , 32 , 32 , 32 , 32 , 31 , 31 , 31 , 31 ,
    31 , 30 , 30 , 30 , 30 , 30 , 29 , 29 , 29 , 29 , 28 , 28 , 28 , 28 , 28 , 27 , 27 , 27 , 27 , 27 ,
    26 , 26 , 26 , 26 , 26 , 25 , 25 , 25 , 25 , 25 , 24 , 24 , 24 , 24 , 24 , 23 , 23 , 23 , 23 , 23 ,
    22 , 22 , 22 , 22 , 22 , 21 , 21 , 21 , 21 , 21 , 20 , 20 , 20 , 20 , 20 , 19 , 19 , 19 , 19 , 18 ,
    18 , 18 , 18 , 18 , 17 , 17 , 17 , 17 , 17 , 16 , 16 , 16 , 16 , 16 , 15 , 15 , 15 , 15 , 15 , 14 ,
    14 , 14 , 14 , 14 , 13 , 13 , 13 , 13 , 13 , 12 , 12 , 12 , 12 , 12 , 11 , 11 , 11 , 11 , 11 , 10 ,
    10 , 10 , 10 , 10 , 9  , 9  , 9  , 9  , 8  , 8  , 8  , 8  , 8  , 7  , 7  , 7  , 7  , 7  , 6  , 6  ,
    6  , 6  , 6  , 5  , 5  , 5  , 5  , 5  , 4  , 4  , 4  , 4  , 4  , 3  , 3  , 3  , 3  , 3  , 2  , 2  ,
    2  , 2  , 2  , 1  , 1  , 1  , 1  , 1  , 0  , 0  , 0  , 0  , -1 , -1 , -1 , -1 , -1 , -2 , -2 , -2 ,
    -2 , -2 , -3 , -3 , -3 , -3 , -3 , -4 , -4 , -4 , -4 , -4 , -5 , -5 , -5 , -5 , -5 , -6 , -6 , -6 ,
    -6 , -6 , -7 , -7 , -7 , -7 , -7 , -8 , -8 , -8 , -8 , -8 , -9 , -9 , -9 , -9 , -9 , -10, -10, -10,
    -10, -11, -11, -11, -11, -11, -12, -12, -12, -12, -12, -13, -13, -13, -13, -13, -14, -14, -14, -14,
    -14, -15, -15, -15, -15, -15, -16, -16, -16, -16, -16, -17, -17, -17, -17, -17, -18, -18, -18, -18,
    -18, -19, -19, -19, -19, -19, -20, -20, -20, -20, -21, -21, -21, -21, -21, -22, -22, -22, -22, -22,
    -23, -23, -23, -23, -23, -24, -24, -24, -24, -24, -25, -25, -25, -25, -25, -26, -26, -26, -26, -26,
    -27, -27, -27, -27, -27, -28, -28, -28, -28, -28, -29, -29, -29, -29, -29, -30, -30, -30, -30, -31,
    -31, -31, -31, -31, -32, -32, -32, -32, -32, -33, -33, -33, -33, -33, -34, -34, -34, -34, -34, -35,
    -35, -35, -35, -35, -36, -36, -36, -36, -36, -37, -37, -37, -37, -37, -38, -38, -38, -38, -38, -39,
    -39, -39, -39, -39, -40, -40, -40, -40, -41, -41, -41, -41, -41, -42, -42, -42, -42, -42, -43, -43,
    -43, -43, -43, -44, -44, -44, -44, -44, -45, -45, -45, -45, -45, -46, -46, -46, -46, -46, -47, -47,
    -47, -47, -47, -48, -48, -48, -48, -48, -49, -49, -49, -49, -49, -50, -50, -50
};
#endif

static inline int32_t fill_significand(char *buffer, uint64_t digits, int32_t *ptz)
{
    char *s = buffer;

    uint32_t q = (uint32_t)(digits / 100000000);
    uint32_t r = (uint32_t)(digits - (uint64_t)q * 100000000);

#if USING_SMALL_LUT
    if (q < 10000)
    {
        s += fill_t_4_digits(s, q, ptz);
    }
    else
#endif
    {
        uint32_t q1 = FAST_DIV10000(q);
        uint32_t r1 = q - q1 * 10000;
        if (q1 < 100) {
            if (q1 >= 10) {
                *ptz = tz_100_lut[q1];
                memcpy(s, &ch_100_lut[q1<<1], 2);
                s += 2;
            } else {
                *ptz = 0;
                *s++ = q1 + '0';
            }
        } else {
            uint32_t q2 = FAST_DIV100(q1);
            uint32_t r2 = q1 - q2 * 100;

            *ptz = tz_100_lut[r2];
            *s++ = q2 + '0';
            memcpy(s, &ch_100_lut[r2<<1], 2);
            s += 2;
        }

        if (!r1) {
            *ptz += 4;
            memset(s, '0', 4);
            s += 4;
        } else {
            s += fill_t_4_digits(s, r1, ptz);
        }
    }

    if (!r) {
        *ptz += 8;
        memset(s, '0', 8);
        s += 8;
    } else {
        s += fill_t_8_digits(s, r, ptz);
    }

    return (int32_t)(s - buffer);
}

static inline int32_t ldouble_convert(diy_fp_t *v, char *buffer, int32_t *vnum_digits)
{
#if USING_SMALL_LUT
    const uint8_t s_lut[8] = {8, 9, 10, 3, 4, 5, 6, 7};
    int32_t s = s_lut[v->e & 0x7];
    v->f = (v->f << s) + ((uint64_t)1 << (s - 1));
    v->e -= s;
    v->e >>= 3;
#else
    const uint8_t s_lut[4] = {8, 9, 6, 7};
    int32_t s = s_lut[v->e & 0x3];
    v->f = (v->f << s) + ((uint64_t)1 << (s - 1));
    v->e -= s;
    v->e >>= 2;
#endif

    uint32_t index = v->e - POW10_LUT_MIN_IDX;
    uint64_t pow10 = pow10_mul_lut[index];
    int32_t exp10 = pow10_exp_lut[index];
#if USING_SMALL_LUT
    exp10 = index < POW10_EXP_LUT_POS ? exp10 : -exp10;
#endif

    v->e -= exp10;
    v->f = u128_mul(pow10, v->f).hi;
    uint32_t delta = (uint32_t)(pow10 >> (64 - s));
    uint32_t remainder = 0;
    uint64_t orig = v->f;

    v->f = orig / 10000;
    remainder = (uint32_t)(orig - v->f * 10000);
    v->e += 4;

    int32_t num_digits, trailing_zeros;
    num_digits = fill_significand(buffer, v->f, &trailing_zeros);
    *vnum_digits = num_digits - trailing_zeros;

    if (remainder > delta) {
        uint32_t q = FAST_DIV100(remainder);
        uint32_t r = remainder - q * 100;
        char *tails = buffer + num_digits;

        memcpy(tails, &ch_100_lut[q<<1], 2);
        memcpy(tails + 2, &ch_100_lut[r<<1], 2);
        trailing_zeros =  ((uint32_t)(tails[1] - '0') * 100 + r <= delta) ? 3 : \
                          ((r <= delta) ? 2 : (((uint32_t)(tails[3] - '0') <= delta) ? 1 : 0));
        v->e -= 4 - trailing_zeros;
        num_digits += 4 - trailing_zeros;
        *vnum_digits = num_digits;
    }

    return num_digits;
}

/*
# python to get define

def print_define():
    base_shift = 6
    base_exp = -1074 - base_shift

    val = 5 ** (-base_exp)
    bit = len(str(val))

    smul = (1 << 32) * val // (10 ** bit)
    srem = (1 << 32) * val % (10 ** bit)
    if srem:
        smul += 1

    lmul = (1 << 64) * val // (10 ** bit)
    lrem = (1 << 64) * val % (10 ** bit)
    if lrem:
        lmul += 1

    delta = (1 << base_shift) * smul >> 32
    exp = base_exp + bit
    split = 64 - base_shift - (len(bin(smul)) - 2)

    print('#define MIM_POW10_EXP      %d'   % (exp))
    print('#define MIM_POW10_DELTA    %d'   % (delta))
    print('#define MIM_POW10_SPLIT    %d'   % (split))
    print('#define MIM_POW10_SHIFT    %d'   % (base_shift))
    print('#define MIM_POW10_SMALL_M  0x%x' % (smul))
    print('#define MIM_POW10_LARGE_M  0x%x' % (lmul))

print_define()
*/

static inline int32_t ldouble_convert_n(diy_fp_t *v, char *buffer, int32_t *vnum_digits)
{
#define MIM_POW10_EXP      -325
#define MIM_POW10_DELTA    49
#define MIM_POW10_SPLIT    26
#define MIM_POW10_SHIFT    6
#define MIM_POW10_SMALL_M  0xc5a05278
#define MIM_POW10_LARGE_M  0xc5a05277621be294

    v->f = (v->f << MIM_POW10_SHIFT) + (1 << (MIM_POW10_SHIFT - 1));
    v->e = MIM_POW10_EXP;

    if (v->f <= (1 << MIM_POW10_SPLIT)) {
        v->f = v->f * MIM_POW10_SMALL_M >> 32;
    } else {
        v->f = u128_mul(v->f, MIM_POW10_LARGE_M).hi;
    }

    uint64_t orig = v->f;
    uint64_t remainder;
    v->f = orig / 100;
    remainder = orig - v->f * 100;

    if (v->f == 0) {
        v->e += 1;
        remainder = ch_100_lut[remainder << 1] - '0';
    } else {
        if (remainder <= MIM_POW10_DELTA) {
            v->e += 2;
            remainder = 0;
        } else {
            v->e += 1;
            remainder = ch_100_lut[remainder << 1] - '0';
        }
    }

    int32_t num_digits = 0, trailing_zeros = 0;
    if (v->f) {
        num_digits = fill_1_16_digits(buffer, v->f, &trailing_zeros);
    }
    if (remainder) {
        buffer[num_digits++] = (char)(remainder + '0');
        trailing_zeros = 0;
    }
    *vnum_digits = num_digits - trailing_zeros;
    return num_digits;
}

static inline int32_t fill_exponent(int32_t K, char *buffer)
{
    int32_t i = 0, k = 0;

    if (K < 0) {
        buffer[i++] = '-';
        K = -K;
    } else {
        buffer[i++] = '+';
    }

    if (K < 100) {
        if (K < 10) {
            buffer[i++] = '0' + K;
        } else {
            memcpy(&buffer[i], &ch_100_lut[K<<1], 2);
            i += 2;
        }
    } else {
        k = FAST_DIV100(K);
        K -= k * 100;
        buffer[i++] = '0' + k;
        memcpy(&buffer[i], &ch_100_lut[K<<1], 2);
        i += 2;
    }

    return i;
}

static inline char* ldouble_format(char *buffer, int32_t num_digits, int32_t vnum_digits, int32_t decimal_point)
{
    switch (decimal_point) {
    case -6: case -5: case -4: case -3: case -2: case -1: case 0:
         /* 0.[000]digits */
        memmove(buffer + 2 - decimal_point, buffer + 1, vnum_digits);
        memset(buffer, '0', 2 - decimal_point);
        buffer[1] = '.';
        buffer += 2 - decimal_point + vnum_digits;
        break;

    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
    case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
        if (decimal_point < vnum_digits) {
            /* dig.its */
            memmove(buffer, buffer + 1, decimal_point);
            buffer[decimal_point] = '.';
            buffer += vnum_digits + 1;
        } else {
            /* digits[000] */
            if (decimal_point > num_digits) {
                memmove(buffer, buffer + 1, num_digits);
                memset(buffer + num_digits, '0', decimal_point - num_digits);
            } else {
                memmove(buffer, buffer + 1, decimal_point);
            }
            buffer += decimal_point;
            memcpy(buffer, ".0", 2);
            buffer += 2;
        }
        break;

    default:
        buffer[0] = buffer[1];
        ++buffer;
        if (vnum_digits != 1) {
            /* d.igitsE+123 */
            *buffer = '.';
            buffer += vnum_digits;
        } else {
            /* dE+123 */
        }
        *buffer++ = 'e';
        buffer += fill_exponent(decimal_point - 1, buffer);
        break;
    }

    *buffer = '\0';
    return buffer;
}

int jnum_dtoa(double num, char *buffer)
{
    diy_fp_t v;
    char *s = buffer;
    union {double d; uint64_t n;} u = {.d = num};
    int32_t signbit = u.n >> (DIY_SIGNIFICAND_SIZE - 1);
    int32_t exponent = (u.n & DP_EXPONENT_MASK) >> DP_SIGNIFICAND_SIZE; /* Exponent */
    uint64_t significand = u.n & DP_SIGNIFICAND_MASK; /* Mantissa */
    int32_t num_digits, vnum_digits;

    if (signbit) {
        *s++ = '-';
    }

    switch (exponent) {
    case DP_EXPONENT_MAX:
        if (significand) {
            memcpy(buffer, "nan", 4);
            return 3;
        } else {
            memcpy(s, "inf", 4);
            return signbit + 3;
        }

    case 0:
        if (significand) {
            /* no-normalized double */
            v.f = significand; /* Non-normalized double doesn't have a extra integer bit for Mantissa */
            v.e = 1 - DP_EXPONENT_OFFSET - DP_SIGNIFICAND_SIZE; /* Fixed Exponent: -1022, divisor of Mantissa: pow(2,52) */
            num_digits = ldouble_convert_n(&v, s + 1, &vnum_digits);
        } else {
            memcpy(s, "0.0", 4);
            return signbit + 3;
        }
        break;

    default:
        /* normalized double */
        v.f = significand + DP_HIDDEN_BIT; /* Normalized double has a extra integer bit for Mantissa */
        v.e = exponent - DP_EXPONENT_OFFSET - DP_SIGNIFICAND_SIZE; /* Exponent offset: -1023, divisor of Mantissa: pow(2,52) */

        if (v.e >= -DP_SIGNIFICAND_SIZE && v.e <= 0 \
            && (v.f & (((uint64_t)1 << -v.e) - 1)) == 0) {
            /* small integer. */
            int32_t tz, n;
            n = fill_1_16_digits(s, v.f >> -v.e, &tz);
            memcpy(s + n, ".0", 3);
            return n + 2 + signbit;
        }
        if (v.e > 0 && v.e <= (63 - DP_SIGNIFICAND_SIZE)) {
            /* big integer. */
            int32_t tz, n;
            n = fill_1_20_digits(s, v.f << v.e, &tz);
            memcpy(s + n, ".0", 3);
            return n + 2 + signbit;
        }
        /* (-1022 - 52) <= e <= (1023 - 52) */
        num_digits = ldouble_convert(&v, s + 1, &vnum_digits);
        break;
    }

    s = ldouble_format(s, num_digits, vnum_digits, num_digits + v.e);
    *s = '\0';
    return (int)(s - buffer);
}

static int jnum_parse_hex(const char *str, jnum_type_t *type, jnum_value_t *value)
{
    const char *s = str;
    char c;
    uint64_t m = 0;

    s += 2;
    while ((s - str) < 18) {
        switch ((c = *s)) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            c -= '0';
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            c -= 'a' - 10;
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            c -= 'A' - 10;
            break;
        default:
            goto end;
        }
        m = (m << 4) + c;
        ++s;
    }

end:
    if (m > 4294967295U /*UINT_MAX*/) {
        *type = JNUM_LHEX;
        value->vlhex = m;
    } else {
        *type = JNUM_HEX;
        value->vhex = (uint32_t)m;
    }

    return (int)(s - str);
}

/*
# python to get lut

def print_lut(step):
    minv = -324 - 20  # 1e-324 = 0.0
    maxv = 309        # 1e309 = inf

    if step == 1:
        pass
    elif step == 2:
        minv = (minv // 2) - 1
        maxv = (maxv // 2) + 1
    else:
        print('Only step 2 or 1 are supported.')
        return

    mul_lut = []
    exp_lut = []
    mask = 1 << 64

    for i in range(minv, maxv + 1):
        exp = i * step
        val = 0
        if i < 0:
            val = 5 ** (-i * step)
            j = 0
            while (1 << j) // val < mask:
                j += 1
            j -= 1
            exp -= j
            val = (1 << j) // val
        else:
            val = 5 ** (i * step)
            if val < mask:
                while val < mask:
                    val <<= 1
                    exp -= 1
                val >>= 1
                exp += 1
            else:
                while val >= mask:
                    val >>= 1
                    exp += 1
        mul_lut.append(val)
        exp_lut.append(exp)

    print('#define POW2_LUT_MIN_IDX    %d' % (minv))
    print('#define POW2_LUT_MAX_IDX    %d' % (maxv))
    last_idx = maxv - minv

    print('static const uint64_t pow2_mul_lut[POW2_LUT_MAX_IDX - POW2_LUT_MIN_IDX + 1] = {', end='')
    for i in range(last_idx + 1):
        if i % 5 == 0:
            print()
            print('    ', end='')
        print('0x%016x' % (mul_lut[i]), end='')
        if i != last_idx:
            print(', ', end='')
        else:
            print()
            print('};')

    print()
    print('static const int16_t pow2_exp_lut[POW2_LUT_MAX_IDX - POW2_LUT_MIN_IDX + 1] = {', end='')
    for i in range(last_idx + 1):
        if i % 15 == 0:
            print()
            print('    ', end='')
        print('%-5d' % (exp_lut[i]), end='')

        if i != last_idx:
            print(', ', end='')
        else:
            print()
            print('};')

print('#if USING_SMALL_LUT')
print_lut(2)
print('\n#else\n')
print_lut(1)
print('#endif')
*/

#if USING_SMALL_LUT
#define POW2_LUT_MIN_IDX    -173
#define POW2_LUT_MAX_IDX    155
static const uint64_t pow2_mul_lut[POW2_LUT_MAX_IDX - POW2_LUT_MIN_IDX + 1] = {
    0xc3c05ee50655e1fa, 0x98ee4a22ecf3188b, 0xeef453d6923bd65a, 0xbaaee17fa23ebf76, 0x91d8a02bb6c10594,
    0xe3e27a444d8d98b7, 0xb208ef855c969f4f, 0x8b16fb203055ac76, 0xd953e8624b85dd78, 0xa9c98d8ccb009506,
    0x84a57695fe98746d, 0xcf42894a5dce35ea, 0xa1ebfb4219491a1f, 0xfd00b897478238d0, 0xc5a890362fddbc62,
    0x9a6bb0aa55653b2d, 0xf148440a256e2c76, 0xbc807527ed3e12bc, 0x93445b8731587ea3, 0xe61acf033d1a45df,
    0xb3c4f1ba87bc8696, 0x8c71dcd9ba0b4925, 0xdb71e91432b1a24a, 0xab70fe17c79ac6ca, 0x85f0468293f0eb4e,
    0xd1476e2c07286faa, 0xa37fce126597973c, 0xff77b1fcbebcdc4f, 0xc795830d75038c1d, 0x9becce62836ac577,
    0xf3a20279ed56d48a, 0xbe5691ef416bd60c, 0x94b3a202eb1c3f39, 0xe858ad248f5c22c9, 0xb58547448ffffb2d,
    0x8dd01fad907ffc3b, 0xdd95317f31c7fa1d, 0xad1c8eab5ee43b66, 0x873e4f75e2224e68, 0xd3515c2831559a83,
    0xa5178fff668ae0b6, 0x80fa687f881c7f8e, 0xc987434744ac874e, 0x9d71ac8fada6c9b5, 0xf6019da07f549b2b,
    0xc0314325637a1939, 0x96267c7535b763b5, 0xea9c227723ee8bcb, 0xb749faed14125d36, 0x8f31cc0937ae58d2,
    0xdfbdcece67006ac9, 0xaecc49914078536d, 0x888f99797a5e012d, 0xd5605fcdcf32e1d6, 0xa6b34ad8c9dfc06f,
    0x823c12795db6ce57, 0xcb7ddcdda26da268, 0x9efa548d26e5a6e1, 0xf867241c8cc6d4c0, 0xc21094364dfb5636,
    0x979cf3ca6cec5b5a, 0xece53cec4a314ebd, 0xb913179899f68584, 0x9096ea6f3848984f, 0xe1ebce4dc7f16dfb,
    0xb080392cc4349dec, 0x89e42caaf9491b60, 0xd77485cb25823ac7, 0xa8530886b54dbdeb, 0x8380dea93da4bc60,
    0xcd795be870516656, 0xa086cfcd97bf97f3, 0xfad2a4b13d1b5d6c, 0xc3f490aa77bd60fc, 0x991711052d8bf3c5,
    0xef340a98172aace4, 0xbae0a846d2195712, 0x91ff83775423cc06, 0xe41f3d6a7377eeca, 0xb23867fb2a35b28d,
    0x8b3c113c38f9f37e, 0xd98ddaee19068c76, 0xa9f6d30a038d1dbc, 0x84c8d4dfd2c63f3b, 0xcf79cc9db955c2cc,
    0xa21727db38cb002f, 0xfd442e4688bd304a, 0xc5dd44271ad3cdba, 0x9a94dd3e8cf578b9, 0xf18899b1bc3f8ca1,
    0xbcb2b812db11a5de, 0x936b9fcebb25c995, 0xe65829b3046b0afa, 0xb3f4e093db73a093, 0x8c974f7383725573,
    0xdbac6c247d62a583, 0xab9eb47c81f5114f, 0x8613fd0145877585, 0xd17f3b51fca3a7a0, 0xa3ab66580d5fdaf5,
    0xffbbcfe994e5c61f, 0xc7caba6e7c5382c8, 0x9c1661a651213e2d, 0xf3e2f893dec3f126, 0xbe89523386091465,
    0x94db483840b717ef, 0xe896a0d7e51e1566, 0xb5b5ada8aaff80b8, 0x8df5efabc5979c8f, 0xddd0467c64bce4a0,
    0xad4ab7112eb3929d, 0x87625f056c7c4a8b, 0xd389b47879823479, 0xa54394fe1eedb8fe, 0x811ccc668829b887,
    0xc9bcff6034c13052, 0x9d9ba7832936edc0, 0xf64335bcf065d37d, 0xc06481fb9bcf8d39, 0x964e858c91ba2655,
    0xeadab0aba3b2dbe5, 0xb77ada0617e3bbcb, 0x8f57fa54c2a9eab6, 0xdff9772470297ebd, 0xaefae51477a06b03,
    0x88b402f7fd75539b, 0xd59944a37c0752a2, 0xa6dfbd9fb8e5b88e, 0x825ecc24c873782f, 0xcbb41ef979346bca,
    0x9f24b832e6b0f436, 0xf8a95fcf88747d94, 0xc24452da229b021b, 0x97c560ba6b0919a5, 0xed246723473e3813,
    0xb94470938fa89bce, 0x90bd77f3483bb9b9, 0xe2280b6c20dd5232, 0xb0af48ec79ace837, 0x8a08f0f8bf0f156b,
    0xd7adf884aa879177, 0xa87fea27a539e9a5, 0x83a3eeeef9153e89, 0xcdb02555653131b6, 0xa0b19d2ab70e6ed6,
    0xfb158592be068d2e, 0xc428d05aa4751e4c, 0x993fe2c6d07b7fab, 0xef73d256a5c0f77c, 0xbb127c53b17ec159,
    0x9226712162ab070d, 0xe45c10c42a2b3b05, 0xb267ed1940f1c61c, 0x8b61313bbabce2c6, 0xd9c7dced53c72255,
    0xaa242499697392d2, 0x84ec3c97da624ab4, 0xcfb11ead453994ba, 0xa2425ff75e14fc31, 0xfd87b5f28300ca0d,
    0xc612062576589dda, 0x9abe14cd44753b52, 0xf1c90080baf72cb1, 0xbce5086492111aea, 0x9392ee8e921d5d07,
    0xe69594bec44de15b, 0xb424dc35095cd80f, 0x8cbccc096f5088cb, 0xdbe6fecebdedd5be, 0xabcc77118461cefc,
    0x8637bd05af6c69b5, 0xd1b71758e219652b, 0xa3d70a3d70a3d70a, 0x8000000000000000, 0xc800000000000000,
    0x9c40000000000000, 0xf424000000000000, 0xbebc200000000000, 0x9502f90000000000, 0xe8d4a51000000000,
    0xb5e620f480000000, 0x8e1bc9bf04000000, 0xde0b6b3a76400000, 0xad78ebc5ac620000, 0x878678326eac9000,
    0xd3c21bcecceda100, 0xa56fa5b99019a5c8, 0x813f3978f8940984, 0xc9f2c9cd04674ede, 0x9dc5ada82b70b59d,
    0xf684df56c3e01bc6, 0xc097ce7bc90715b3, 0x96769950b50d88f4, 0xeb194f8e1ae525fd, 0xb7abc627050305ad,
    0x8f7e32ce7bea5c6f, 0xe0352f62a19e306e, 0xaf298d050e4395d6, 0x88d8762bf324cd0f, 0xd5d238a4abe98068,
    0xa70c3c40a64e6c51, 0x82818f1281ed449f, 0xcbea6f8ceb02bb39, 0x9f4f2726179a2245, 0xf8ebad2b84e0d58b,
    0xc2781f49ffcfa6d5, 0x97edd871cfda3a56, 0xed63a231d4c4fb27, 0xb975d6b6ee39e436, 0x90e40fbeea1d3a4a,
    0xe264589a4dcdab14, 0xb0de65388cc8ada8, 0x8a2dbf142dfcc7ab, 0xd7e77a8f87daf7fb, 0xa8acd7c0222311bc,
    0x83c7088e1aab65db, 0xcde6fd5e09abcf26, 0xa0dc75f1778e39d6, 0xfb5878494ace3a5f, 0xc45d1df942711d9a,
    0x9968bf6abbe85f20, 0xefb3ab16c59b14a2, 0xbb445da9ca61281f, 0x924d692ca61be758, 0xe498f455c38b997a,
    0xb2977ee300c50fe7, 0x8b865b215899f46c, 0xda01ee641a708de9, 0xaa51823e34a7eede, 0x850fadc09923329e,
    0xcfe87f7cef46ff16, 0xa26da3999aef7749, 0xfdcb4fa002162a63, 0xc646d63501a1511d, 0x9ae757596946075f,
    0xf209787bb47d6b84, 0xbd176620a501fbff, 0x93ba47c980e98cdf, 0xe6d3102ad96cec1d, 0xb454e4a179dd1877,
    0x8ce2529e2734bb1d, 0xdc21a1171d42645d, 0xabfa45da0edbde69, 0x865b86925b9bc5c2, 0xd1ef0244af2364ff,
    0xa402b9c5a8d3a6e7, 0x802221226be55a64, 0xc83553c5c8965d3d, 0x9c69a97284b578d7, 0xf46518c2ef5b8cd1,
    0xbeeefb584aff8603, 0x952ab45cfa97a0b2, 0xe912b9d1478ceb17, 0xb616a12b7fe617aa, 0x8e41ade9fbebc27d,
    0xde469fbd99a05fe3, 0xada72ccc20054ae9, 0x87aa9aff79042286, 0xd3fa922f2d1675f2, 0xa59bc234db398c25,
    0x8161afb94b44f57d, 0xca28a291859bbf93, 0x9defbf01b061adab, 0xf6c69a72a3989f5b, 0xc0cb28a98fcf3c7f,
    0x969eb7c47859e743, 0xeb57ff22fc0c7959, 0xb7dcbf5354e9bece, 0x8fa475791a569d10, 0xe070f78d3927556a,
    0xaf58416654a6babb, 0x88fcf317f22241e2, 0xd60b3bd56a5586f1, 0xa738c6bebb12d16c, 0x82a45b450226b39c,
    0xcc20ce9bd35c78a5, 0x9f79a169bd203e41, 0xf92e0c3537826145, 0xc2abf989935ddbfe, 0x98165af37b2153de,
    0xeda2ee1c7064130c, 0xb9a74a0637ce2ee1, 0x910ab1d4db9914a0, 0xe2a0b5dc971f303a, 0xb10d8e1456105dad,
    0x8a5296ffe33cc92f, 0xd8210befd30efa5a, 0xa8d9d1535ce3b396, 0x83ea2b892091e44d, 0xce1de40642e3f4b9,
    0xa1075a24e4421730, 0xfb9b7cd9a4a7443c, 0xc491798a08a2ad4e, 0x9991a6f3d6bf1765, 0xeff394dcff8a948e,
    0xbb764c4ca7a4440f, 0x92746b9be2f8552c, 0xe4d5e82392a40515, 0xb2c71d5bca9023f8, 0x8bab8eefb6409c1a,
    0xda3c0f568cc4f3e8, 0xaa7eebfb9df9de8d, 0x8533285c936b35de, 0xd01fef10a657842c, 0xa298f2c501f45f42,
    0xfe0efb53d30dd4d7, 0xc67bb4597ce2ce48, 0x9b10a4e5e9913128, 0xf24a01a73cf2dccf, 0xbd49d14aa79dbc82,
    0x93e1ab8252f33b45, 0xe7109bfba19c0c9d, 0xb484f9dc9641e9da, 0x8d07e33455637eb2, 0xdc5c5301c56b75f7,
    0xac2820d9623bf429, 0x867f59a9d4bed6c0, 0xd226fc195c6a2f8c, 0xa42e74f3d032f525, 0x80444b5e7aa7cf85,
    0xc86ab5c39fa63440, 0x9c935e00d4b9d8d2, 0xf4a642e14c6262c8, 0xbf21e44003acdd2c, 0x95527a5202df0ccb,
    0xe950df20247c83fd, 0xb6472e511c81471d, 0x8e679c2f5e44ff8f, 0xde81e40a034bcf4f
};

static const int16_t pow2_exp_lut[POW2_LUT_MAX_IDX - POW2_LUT_MIN_IDX + 1] = {
    -1213, -1206, -1200, -1193, -1186, -1180, -1173, -1166, -1160, -1153, -1146, -1140, -1133, -1127, -1120,
    -1113, -1107, -1100, -1093, -1087, -1080, -1073, -1067, -1060, -1053, -1047, -1040, -1034, -1027, -1020,
    -1014, -1007, -1000, -994 , -987 , -980 , -974 , -967 , -960 , -954 , -947 , -940 , -934 , -927 , -921 ,
    -914 , -907 , -901 , -894 , -887 , -881 , -874 , -867 , -861 , -854 , -847 , -841 , -834 , -828 , -821 ,
    -814 , -808 , -801 , -794 , -788 , -781 , -774 , -768 , -761 , -754 , -748 , -741 , -735 , -728 , -721 ,
    -715 , -708 , -701 , -695 , -688 , -681 , -675 , -668 , -661 , -655 , -648 , -642 , -635 , -628 , -622 ,
    -615 , -608 , -602 , -595 , -588 , -582 , -575 , -568 , -562 , -555 , -549 , -542 , -535 , -529 , -522 ,
    -515 , -509 , -502 , -495 , -489 , -482 , -475 , -469 , -462 , -455 , -449 , -442 , -436 , -429 , -422 ,
    -416 , -409 , -402 , -396 , -389 , -382 , -376 , -369 , -362 , -356 , -349 , -343 , -336 , -329 , -323 ,
    -316 , -309 , -303 , -296 , -289 , -283 , -276 , -269 , -263 , -256 , -250 , -243 , -236 , -230 , -223 ,
    -216 , -210 , -203 , -196 , -190 , -183 , -176 , -170 , -163 , -157 , -150 , -143 , -137 , -130 , -123 ,
    -117 , -110 , -103 , -97  , -90  , -83  , -77  , -70  , -63  , -57  , -50  , -44  , -37  , -30  , -24  ,
    -17  , -10  , -4   , 3    , 10   , 16   , 23   , 30   , 36   , 43   , 49   , 56   , 63   , 69   , 76   ,
    83   , 89   , 96   , 103  , 109  , 116  , 123  , 129  , 136  , 142  , 149  , 156  , 162  , 169  , 176  ,
    182  , 189  , 196  , 202  , 209  , 216  , 222  , 229  , 235  , 242  , 249  , 255  , 262  , 269  , 275  ,
    282  , 289  , 295  , 302  , 309  , 315  , 322  , 328  , 335  , 342  , 348  , 355  , 362  , 368  , 375  ,
    382  , 388  , 395  , 402  , 408  , 415  , 422  , 428  , 435  , 441  , 448  , 455  , 461  , 468  , 475  ,
    481  , 488  , 495  , 501  , 508  , 515  , 521  , 528  , 534  , 541  , 548  , 554  , 561  , 568  , 574  ,
    581  , 588  , 594  , 601  , 608  , 614  , 621  , 627  , 634  , 641  , 647  , 654  , 661  , 667  , 674  ,
    681  , 687  , 694  , 701  , 707  , 714  , 720  , 727  , 734  , 740  , 747  , 754  , 760  , 767  , 774  ,
    780  , 787  , 794  , 800  , 807  , 813  , 820  , 827  , 833  , 840  , 847  , 853  , 860  , 867  , 873  ,
    880  , 887  , 893  , 900  , 907  , 913  , 920  , 926  , 933  , 940  , 946  , 953  , 960  , 966
};

#else

#define POW2_LUT_MIN_IDX    -344
#define POW2_LUT_MAX_IDX    309
static const uint64_t pow2_mul_lut[POW2_LUT_MAX_IDX - POW2_LUT_MIN_IDX + 1] = {
    0x98ee4a22ecf3188b, 0xbf29dcaba82fdeae, 0xeef453d6923bd65a, 0x9558b4661b6565f8, 0xbaaee17fa23ebf76,
    0xe95a99df8ace6f53, 0x91d8a02bb6c10594, 0xb64ec836a47146f9, 0xe3e27a444d8d98b7, 0x8e6d8c6ab0787f72,
    0xb208ef855c969f4f, 0xde8b2b66b3bc4723, 0x8b16fb203055ac76, 0xaddcb9e83c6b1793, 0xd953e8624b85dd78,
    0x87d4713d6f33aa6b, 0xa9c98d8ccb009506, 0xd43bf0effdc0ba48, 0x84a57695fe98746d, 0xa5ced43b7e3e9188,
    0xcf42894a5dce35ea, 0x818995ce7aa0e1b2, 0xa1ebfb4219491a1f, 0xca66fa129f9b60a6, 0xfd00b897478238d0,
    0x9e20735e8cb16382, 0xc5a890362fddbc62, 0xf712b443bbd52b7b, 0x9a6bb0aa55653b2d, 0xc1069cd4eabe89f8,
    0xf148440a256e2c76, 0x96cd2a865764dbca, 0xbc807527ed3e12bc, 0xeba09271e88d976b, 0x93445b8731587ea3,
    0xb8157268fdae9e4c, 0xe61acf033d1a45df, 0x8fd0c16206306bab, 0xb3c4f1ba87bc8696, 0xe0b62e2929aba83c,
    0x8c71dcd9ba0b4925, 0xaf8e5410288e1b6f, 0xdb71e91432b1a24a, 0x892731ac9faf056e, 0xab70fe17c79ac6ca,
    0xd64d3d9db981787d, 0x85f0468293f0eb4e, 0xa76c582338ed2621, 0xd1476e2c07286faa, 0x82cca4db847945ca,
    0xa37fce126597973c, 0xcc5fc196fefd7d0c, 0xff77b1fcbebcdc4f, 0x9faacf3df73609b1, 0xc795830d75038c1d,
    0xf97ae3d0d2446f25, 0x9becce62836ac577, 0xc2e801fb244576d5, 0xf3a20279ed56d48a, 0x9845418c345644d6,
    0xbe5691ef416bd60c, 0xedec366b11c6cb8f, 0x94b3a202eb1c3f39, 0xb9e08a83a5e34f07, 0xe858ad248f5c22c9,
    0x91376c36d99995be, 0xb58547448ffffb2d, 0xe2e69915b3fff9f9, 0x8dd01fad907ffc3b, 0xb1442798f49ffb4a,
    0xdd95317f31c7fa1d, 0x8a7d3eef7f1cfc52, 0xad1c8eab5ee43b66, 0xd863b256369d4a40, 0x873e4f75e2224e68,
    0xa90de3535aaae202, 0xd3515c2831559a83, 0x8412d9991ed58091, 0xa5178fff668ae0b6, 0xce5d73ff402d98e3,
    0x80fa687f881c7f8e, 0xa139029f6a239f72, 0xc987434744ac874e, 0xfbe9141915d7a922, 0x9d71ac8fada6c9b5,
    0xc4ce17b399107c22, 0xf6019da07f549b2b, 0x99c102844f94e0fb, 0xc0314325637a1939, 0xf03d93eebc589f88,
    0x96267c7535b763b5, 0xbbb01b9283253ca2, 0xea9c227723ee8bcb, 0x92a1958a7675175f, 0xb749faed14125d36,
    0xe51c79a85916f484, 0x8f31cc0937ae58d2, 0xb2fe3f0b8599ef07, 0xdfbdcece67006ac9, 0x8bd6a141006042bd,
    0xaecc49914078536d, 0xda7f5bf590966848, 0x888f99797a5e012d, 0xaab37fd7d8f58178, 0xd5605fcdcf32e1d6,
    0x855c3be0a17fcd26, 0xa6b34ad8c9dfc06f, 0xd0601d8efc57b08b, 0x823c12795db6ce57, 0xa2cb1717b52481ed,
    0xcb7ddcdda26da268, 0xfe5d54150b090b02, 0x9efa548d26e5a6e1, 0xc6b8e9b0709f109a, 0xf867241c8cc6d4c0,
    0x9b407691d7fc44f8, 0xc21094364dfb5636, 0xf294b943e17a2bc4, 0x979cf3ca6cec5b5a, 0xbd8430bd08277231,
    0xece53cec4a314ebd, 0x940f4613ae5ed136, 0xb913179899f68584, 0xe757dd7ec07426e5, 0x9096ea6f3848984f,
    0xb4bca50b065abe63, 0xe1ebce4dc7f16dfb, 0x8d3360f09cf6e4bd, 0xb080392cc4349dec, 0xdca04777f541c567,
    0x89e42caaf9491b60, 0xac5d37d5b79b6239, 0xd77485cb25823ac7, 0x86a8d39ef77164bc, 0xa8530886b54dbdeb,
    0xd267caa862a12d66, 0x8380dea93da4bc60, 0xa46116538d0deb78, 0xcd795be870516656, 0x806bd9714632dff6,
    0xa086cfcd97bf97f3, 0xc8a883c0fdaf7df0, 0xfad2a4b13d1b5d6c, 0x9cc3a6eec6311a63, 0xc3f490aa77bd60fc,
    0xf4f1b4d515acb93b, 0x991711052d8bf3c5, 0xbf5cd54678eef0b6, 0xef340a98172aace4, 0x9580869f0e7aac0e,
    0xbae0a846d2195712, 0xe998d258869facd7, 0x91ff83775423cc06, 0xb67f6455292cbf08, 0xe41f3d6a7377eeca,
    0x8e938662882af53e, 0xb23867fb2a35b28d, 0xdec681f9f4c31f31, 0x8b3c113c38f9f37e, 0xae0b158b4738705e,
    0xd98ddaee19068c76, 0x87f8a8d4cfa417c9, 0xa9f6d30a038d1dbc, 0xd47487cc8470652b, 0x84c8d4dfd2c63f3b,
    0xa5fb0a17c777cf09, 0xcf79cc9db955c2cc, 0x81ac1fe293d599bf, 0xa21727db38cb002f, 0xca9cf1d206fdc03b,
    0xfd442e4688bd304a, 0x9e4a9cec15763e2e, 0xc5dd44271ad3cdba, 0xf7549530e188c128, 0x9a94dd3e8cf578b9,
    0xc13a148e3032d6e7, 0xf18899b1bc3f8ca1, 0x96f5600f15a7b7e5, 0xbcb2b812db11a5de, 0xebdf661791d60f56,
    0x936b9fcebb25c995, 0xb84687c269ef3bfb, 0xe65829b3046b0afa, 0x8ff71a0fe2c2e6dc, 0xb3f4e093db73a093,
    0xe0f218b8d25088b8, 0x8c974f7383725573, 0xafbd2350644eeacf, 0xdbac6c247d62a583, 0x894bc396ce5da772,
    0xab9eb47c81f5114f, 0xd686619ba27255a2, 0x8613fd0145877585, 0xa798fc4196e952e7, 0xd17f3b51fca3a7a0,
    0x82ef85133de648c4, 0xa3ab66580d5fdaf5, 0xcc963fee10b7d1b3, 0xffbbcfe994e5c61f, 0x9fd561f1fd0f9bd3,
    0xc7caba6e7c5382c8, 0xf9bd690a1b68637b, 0x9c1661a651213e2d, 0xc31bfa0fe5698db8, 0xf3e2f893dec3f126,
    0x986ddb5c6b3a76b7, 0xbe89523386091465, 0xee2ba6c0678b597f, 0x94db483840b717ef, 0xba121a4650e4ddeb,
    0xe896a0d7e51e1566, 0x915e2486ef32cd60, 0xb5b5ada8aaff80b8, 0xe3231912d5bf60e6, 0x8df5efabc5979c8f,
    0xb1736b96b6fd83b3, 0xddd0467c64bce4a0, 0x8aa22c0dbef60ee4, 0xad4ab7112eb3929d, 0xd89d64d57a607744,
    0x87625f056c7c4a8b, 0xa93af6c6c79b5d2d, 0xd389b47879823479, 0x843610cb4bf160cb, 0xa54394fe1eedb8fe,
    0xce947a3da6a9273e, 0x811ccc668829b887, 0xa163ff802a3426a8, 0xc9bcff6034c13052, 0xfc2c3f3841f17c67,
    0x9d9ba7832936edc0, 0xc5029163f384a931, 0xf64335bcf065d37d, 0x99ea0196163fa42e, 0xc06481fb9bcf8d39,
    0xf07da27a82c37088, 0x964e858c91ba2655, 0xbbe226efb628afea, 0xeadab0aba3b2dbe5, 0x92c8ae6b464fc96f,
    0xb77ada0617e3bbcb, 0xe55990879ddcaabd, 0x8f57fa54c2a9eab6, 0xb32df8e9f3546564, 0xdff9772470297ebd,
    0x8bfbea76c619ef36, 0xaefae51477a06b03, 0xdab99e59958885c4, 0x88b402f7fd75539b, 0xaae103b5fcd2a881,
    0xd59944a37c0752a2, 0x857fcae62d8493a5, 0xa6dfbd9fb8e5b88e, 0xd097ad07a71f26b2, 0x825ecc24c873782f,
    0xa2f67f2dfa90563b, 0xcbb41ef979346bca, 0xfea126b7d78186bc, 0x9f24b832e6b0f436, 0xc6ede63fa05d3143,
    0xf8a95fcf88747d94, 0x9b69dbe1b548ce7c, 0xc24452da229b021b, 0xf2d56790ab41c2a2, 0x97c560ba6b0919a5,
    0xbdb6b8e905cb600f, 0xed246723473e3813, 0x9436c0760c86e30b, 0xb94470938fa89bce, 0xe7958cb87392c2c2,
    0x90bd77f3483bb9b9, 0xb4ecd5f01a4aa828, 0xe2280b6c20dd5232, 0x8d590723948a535f, 0xb0af48ec79ace837,
    0xdcdb1b2798182244, 0x8a08f0f8bf0f156b, 0xac8b2d36eed2dac5, 0xd7adf884aa879177, 0x86ccbb52ea94baea,
    0xa87fea27a539e9a5, 0xd29fe4b18e88640e, 0x83a3eeeef9153e89, 0xa48ceaaab75a8e2b, 0xcdb02555653131b6,
    0x808e17555f3ebf11, 0xa0b19d2ab70e6ed6, 0xc8de047564d20a8b, 0xfb158592be068d2e, 0x9ced737bb6c4183d,
    0xc428d05aa4751e4c, 0xf53304714d9265df, 0x993fe2c6d07b7fab, 0xbf8fdb78849a5f96, 0xef73d256a5c0f77c,
    0x95a8637627989aad, 0xbb127c53b17ec159, 0xe9d71b689dde71af, 0x9226712162ab070d, 0xb6b00d69bb55c8d1,
    0xe45c10c42a2b3b05, 0x8eb98a7a9a5b04e3, 0xb267ed1940f1c61c, 0xdf01e85f912e37a3, 0x8b61313bbabce2c6,
    0xae397d8aa96c1b77, 0xd9c7dced53c72255, 0x881cea14545c7575, 0xaa242499697392d2, 0xd4ad2dbfc3d07787,
    0x84ec3c97da624ab4, 0xa6274bbdd0fadd61, 0xcfb11ead453994ba, 0x81ceb32c4b43fcf4, 0xa2425ff75e14fc31,
    0xcad2f7f5359a3b3e, 0xfd87b5f28300ca0d, 0x9e74d1b791e07e48, 0xc612062576589dda, 0xf79687aed3eec551,
    0x9abe14cd44753b52, 0xc16d9a0095928a27, 0xf1c90080baf72cb1, 0x971da05074da7bee, 0xbce5086492111aea,
    0xec1e4a7db69561a5, 0x9392ee8e921d5d07, 0xb877aa3236a4b449, 0xe69594bec44de15b, 0x901d7cf73ab0acd9,
    0xb424dc35095cd80f, 0xe12e13424bb40e13, 0x8cbccc096f5088cb, 0xafebff0bcb24aafe, 0xdbe6fecebdedd5be,
    0x89705f4136b4a597, 0xabcc77118461cefc, 0xd6bf94d5e57a42bc, 0x8637bd05af6c69b5, 0xa7c5ac471b478423,
    0xd1b71758e219652b, 0x83126e978d4fdf3b, 0xa3d70a3d70a3d70a, 0xcccccccccccccccc, 0x8000000000000000,
    0xa000000000000000, 0xc800000000000000, 0xfa00000000000000, 0x9c40000000000000, 0xc350000000000000,
    0xf424000000000000, 0x9896800000000000, 0xbebc200000000000, 0xee6b280000000000, 0x9502f90000000000,
    0xba43b74000000000, 0xe8d4a51000000000, 0x9184e72a00000000, 0xb5e620f480000000, 0xe35fa931a0000000,
    0x8e1bc9bf04000000, 0xb1a2bc2ec5000000, 0xde0b6b3a76400000, 0x8ac7230489e80000, 0xad78ebc5ac620000,
    0xd8d726b7177a8000, 0x878678326eac9000, 0xa968163f0a57b400, 0xd3c21bcecceda100, 0x84595161401484a0,
    0xa56fa5b99019a5c8, 0xcecb8f27f4200f3a, 0x813f3978f8940984, 0xa18f07d736b90be5, 0xc9f2c9cd04674ede,
    0xfc6f7c4045812296, 0x9dc5ada82b70b59d, 0xc5371912364ce305, 0xf684df56c3e01bc6, 0x9a130b963a6c115c,
    0xc097ce7bc90715b3, 0xf0bdc21abb48db20, 0x96769950b50d88f4, 0xbc143fa4e250eb31, 0xeb194f8e1ae525fd,
    0x92efd1b8d0cf37be, 0xb7abc627050305ad, 0xe596b7b0c643c719, 0x8f7e32ce7bea5c6f, 0xb35dbf821ae4f38b,
    0xe0352f62a19e306e, 0x8c213d9da502de45, 0xaf298d050e4395d6, 0xdaf3f04651d47b4c, 0x88d8762bf324cd0f,
    0xab0e93b6efee0053, 0xd5d238a4abe98068, 0x85a36366eb71f041, 0xa70c3c40a64e6c51, 0xd0cf4b50cfe20765,
    0x82818f1281ed449f, 0xa321f2d7226895c7, 0xcbea6f8ceb02bb39, 0xfee50b7025c36a08, 0x9f4f2726179a2245,
    0xc722f0ef9d80aad6, 0xf8ebad2b84e0d58b, 0x9b934c3b330c8577, 0xc2781f49ffcfa6d5, 0xf316271c7fc3908a,
    0x97edd871cfda3a56, 0xbde94e8e43d0c8ec, 0xed63a231d4c4fb27, 0x945e455f24fb1cf8, 0xb975d6b6ee39e436,
    0xe7d34c64a9c85d44, 0x90e40fbeea1d3a4a, 0xb51d13aea4a488dd, 0xe264589a4dcdab14, 0x8d7eb76070a08aec,
    0xb0de65388cc8ada8, 0xdd15fe86affad912, 0x8a2dbf142dfcc7ab, 0xacb92ed9397bf996, 0xd7e77a8f87daf7fb,
    0x86f0ac99b4e8dafd, 0xa8acd7c0222311bc, 0xd2d80db02aabd62b, 0x83c7088e1aab65db, 0xa4b8cab1a1563f52,
    0xcde6fd5e09abcf26, 0x80b05e5ac60b6178, 0xa0dc75f1778e39d6, 0xc913936dd571c84c, 0xfb5878494ace3a5f,
    0x9d174b2dcec0e47b, 0xc45d1df942711d9a, 0xf5746577930d6500, 0x9968bf6abbe85f20, 0xbfc2ef456ae276e8,
    0xefb3ab16c59b14a2, 0x95d04aee3b80ece5, 0xbb445da9ca61281f, 0xea1575143cf97226, 0x924d692ca61be758,
    0xb6e0c377cfa2e12e, 0xe498f455c38b997a, 0x8edf98b59a373fec, 0xb2977ee300c50fe7, 0xdf3d5e9bc0f653e1,
    0x8b865b215899f46c, 0xae67f1e9aec07187, 0xda01ee641a708de9, 0x884134fe908658b2, 0xaa51823e34a7eede,
    0xd4e5e2cdc1d1ea96, 0x850fadc09923329e, 0xa6539930bf6bff45, 0xcfe87f7cef46ff16, 0x81f14fae158c5f6e,
    0xa26da3999aef7749, 0xcb090c8001ab551c, 0xfdcb4fa002162a63, 0x9e9f11c4014dda7e, 0xc646d63501a1511d,
    0xf7d88bc24209a565, 0x9ae757596946075f, 0xc1a12d2fc3978937, 0xf209787bb47d6b84, 0x9745eb4d50ce6332,
    0xbd176620a501fbff, 0xec5d3fa8ce427aff, 0x93ba47c980e98cdf, 0xb8a8d9bbe123f017, 0xe6d3102ad96cec1d,
    0x9043ea1ac7e41392, 0xb454e4a179dd1877, 0xe16a1dc9d8545e94, 0x8ce2529e2734bb1d, 0xb01ae745b101e9e4,
    0xdc21a1171d42645d, 0x899504ae72497eba, 0xabfa45da0edbde69, 0xd6f8d7509292d603, 0x865b86925b9bc5c2,
    0xa7f26836f282b732, 0xd1ef0244af2364ff, 0x8335616aed761f1f, 0xa402b9c5a8d3a6e7, 0xcd036837130890a1,
    0x802221226be55a64, 0xa02aa96b06deb0fd, 0xc83553c5c8965d3d, 0xfa42a8b73abbf48c, 0x9c69a97284b578d7,
    0xc38413cf25e2d70d, 0xf46518c2ef5b8cd1, 0x98bf2f79d5993802, 0xbeeefb584aff8603, 0xeeaaba2e5dbf6784,
    0x952ab45cfa97a0b2, 0xba756174393d88df, 0xe912b9d1478ceb17, 0x91abb422ccb812ee, 0xb616a12b7fe617aa,
    0xe39c49765fdf9d94, 0x8e41ade9fbebc27d, 0xb1d219647ae6b31c, 0xde469fbd99a05fe3, 0x8aec23d680043bee,
    0xada72ccc20054ae9, 0xd910f7ff28069da4, 0x87aa9aff79042286, 0xa99541bf57452b28, 0xd3fa922f2d1675f2,
    0x847c9b5d7c2e09b7, 0xa59bc234db398c25, 0xcf02b2c21207ef2e, 0x8161afb94b44f57d, 0xa1ba1ba79e1632dc,
    0xca28a291859bbf93, 0xfcb2cb35e702af78, 0x9defbf01b061adab, 0xc56baec21c7a1916, 0xf6c69a72a3989f5b,
    0x9a3c2087a63f6399, 0xc0cb28a98fcf3c7f, 0xf0fdf2d3f3c30b9f, 0x969eb7c47859e743, 0xbc4665b596706114,
    0xeb57ff22fc0c7959, 0x9316ff75dd87cbd8, 0xb7dcbf5354e9bece, 0xe5d3ef282a242e81, 0x8fa475791a569d10,
    0xb38d92d760ec4455, 0xe070f78d3927556a, 0x8c469ab843b89562, 0xaf58416654a6babb, 0xdb2e51bfe9d0696a,
    0x88fcf317f22241e2, 0xab3c2fddeeaad25a, 0xd60b3bd56a5586f1, 0x85c7056562757456, 0xa738c6bebb12d16c,
    0xd106f86e69d785c7, 0x82a45b450226b39c, 0xa34d721642b06084, 0xcc20ce9bd35c78a5, 0xff290242c83396ce,
    0x9f79a169bd203e41, 0xc75809c42c684dd1, 0xf92e0c3537826145, 0x9bbcc7a142b17ccb, 0xc2abf989935ddbfe,
    0xf356f7ebf83552fe, 0x98165af37b2153de, 0xbe1bf1b059e9a8d6, 0xeda2ee1c7064130c, 0x9485d4d1c63e8be7,
    0xb9a74a0637ce2ee1, 0xe8111c87c5c1ba99, 0x910ab1d4db9914a0, 0xb54d5e4a127f59c8, 0xe2a0b5dc971f303a,
    0x8da471a9de737e24, 0xb10d8e1456105dad, 0xdd50f1996b947518, 0x8a5296ffe33cc92f, 0xace73cbfdc0bfb7b,
    0xd8210befd30efa5a, 0x8714a775e3e95c78, 0xa8d9d1535ce3b396, 0xd31045a8341ca07c, 0x83ea2b892091e44d,
    0xa4e4b66b68b65d60, 0xce1de40642e3f4b9, 0x80d2ae83e9ce78f3, 0xa1075a24e4421730, 0xc94930ae1d529cfc,
    0xfb9b7cd9a4a7443c, 0x9d412e0806e88aa5, 0xc491798a08a2ad4e, 0xf5b5d7ec8acb58a2, 0x9991a6f3d6bf1765,
    0xbff610b0cc6edd3f, 0xeff394dcff8a948e, 0x95f83d0a1fb69cd9, 0xbb764c4ca7a4440f, 0xea53df5fd18d5513,
    0x92746b9be2f8552c, 0xb7118682dbb66a77, 0xe4d5e82392a40515, 0x8f05b1163ba6832d, 0xb2c71d5bca9023f8,
    0xdf78e4b2bd342cf6, 0x8bab8eefb6409c1a, 0xae9672aba3d0c320, 0xda3c0f568cc4f3e8, 0x8865899617fb1871,
    0xaa7eebfb9df9de8d, 0xd51ea6fa85785631, 0x8533285c936b35de, 0xa67ff273b8460356, 0xd01fef10a657842c,
    0x8213f56a67f6b29b, 0xa298f2c501f45f42, 0xcb3f2f7642717713, 0xfe0efb53d30dd4d7, 0x9ec95d1463e8a506,
    0xc67bb4597ce2ce48, 0xf81aa16fdc1b81da, 0x9b10a4e5e9913128, 0xc1d4ce1f63f57d72, 0xf24a01a73cf2dccf,
    0x976e41088617ca01, 0xbd49d14aa79dbc82, 0xec9c459d51852ba2, 0x93e1ab8252f33b45, 0xb8da1662e7b00a17,
    0xe7109bfba19c0c9d, 0x906a617d450187e2, 0xb484f9dc9641e9da, 0xe1a63853bbd26451, 0x8d07e33455637eb2,
    0xb049dc016abc5e5f, 0xdc5c5301c56b75f7, 0x89b9b3e11b6329ba, 0xac2820d9623bf429, 0xd732290fbacaf133,
    0x867f59a9d4bed6c0, 0xa81f301449ee8c70, 0xd226fc195c6a2f8c, 0x83585d8fd9c25db7, 0xa42e74f3d032f525,
    0xcd3a1230c43fb26f, 0x80444b5e7aa7cf85, 0xa0555e361951c366, 0xc86ab5c39fa63440, 0xfa856334878fc150,
    0x9c935e00d4b9d8d2, 0xc3b8358109e84f07, 0xf4a642e14c6262c8, 0x98e7e9cccfbd7dbd, 0xbf21e44003acdd2c,
    0xeeea5d5004981478, 0x95527a5202df0ccb, 0xbaa718e68396cffd, 0xe950df20247c83fd, 0x91d28b7416cdd27e,
    0xb6472e511c81471d, 0xe3d8f9e563a198e5, 0x8e679c2f5e44ff8f, 0xb201833b35d63f73
};

static const int16_t pow2_exp_lut[POW2_LUT_MAX_IDX - POW2_LUT_MIN_IDX + 1] = {
    -1206, -1203, -1200, -1196, -1193, -1190, -1186, -1183, -1180, -1176, -1173, -1170, -1166, -1163, -1160,
    -1156, -1153, -1150, -1146, -1143, -1140, -1136, -1133, -1130, -1127, -1123, -1120, -1117, -1113, -1110,
    -1107, -1103, -1100, -1097, -1093, -1090, -1087, -1083, -1080, -1077, -1073, -1070, -1067, -1063, -1060,
    -1057, -1053, -1050, -1047, -1043, -1040, -1037, -1034, -1030, -1027, -1024, -1020, -1017, -1014, -1010,
    -1007, -1004, -1000, -997 , -994 , -990 , -987 , -984 , -980 , -977 , -974 , -970 , -967 , -964 , -960 ,
    -957 , -954 , -950 , -947 , -944 , -940 , -937 , -934 , -931 , -927 , -924 , -921 , -917 , -914 , -911 ,
    -907 , -904 , -901 , -897 , -894 , -891 , -887 , -884 , -881 , -877 , -874 , -871 , -867 , -864 , -861 ,
    -857 , -854 , -851 , -847 , -844 , -841 , -838 , -834 , -831 , -828 , -824 , -821 , -818 , -814 , -811 ,
    -808 , -804 , -801 , -798 , -794 , -791 , -788 , -784 , -781 , -778 , -774 , -771 , -768 , -764 , -761 ,
    -758 , -754 , -751 , -748 , -744 , -741 , -738 , -735 , -731 , -728 , -725 , -721 , -718 , -715 , -711 ,
    -708 , -705 , -701 , -698 , -695 , -691 , -688 , -685 , -681 , -678 , -675 , -671 , -668 , -665 , -661 ,
    -658 , -655 , -651 , -648 , -645 , -642 , -638 , -635 , -632 , -628 , -625 , -622 , -618 , -615 , -612 ,
    -608 , -605 , -602 , -598 , -595 , -592 , -588 , -585 , -582 , -578 , -575 , -572 , -568 , -565 , -562 ,
    -558 , -555 , -552 , -549 , -545 , -542 , -539 , -535 , -532 , -529 , -525 , -522 , -519 , -515 , -512 ,
    -509 , -505 , -502 , -499 , -495 , -492 , -489 , -485 , -482 , -479 , -475 , -472 , -469 , -465 , -462 ,
    -459 , -455 , -452 , -449 , -446 , -442 , -439 , -436 , -432 , -429 , -426 , -422 , -419 , -416 , -412 ,
    -409 , -406 , -402 , -399 , -396 , -392 , -389 , -386 , -382 , -379 , -376 , -372 , -369 , -366 , -362 ,
    -359 , -356 , -353 , -349 , -346 , -343 , -339 , -336 , -333 , -329 , -326 , -323 , -319 , -316 , -313 ,
    -309 , -306 , -303 , -299 , -296 , -293 , -289 , -286 , -283 , -279 , -276 , -273 , -269 , -266 , -263 ,
    -259 , -256 , -253 , -250 , -246 , -243 , -240 , -236 , -233 , -230 , -226 , -223 , -220 , -216 , -213 ,
    -210 , -206 , -203 , -200 , -196 , -193 , -190 , -186 , -183 , -180 , -176 , -173 , -170 , -166 , -163 ,
    -160 , -157 , -153 , -150 , -147 , -143 , -140 , -137 , -133 , -130 , -127 , -123 , -120 , -117 , -113 ,
    -110 , -107 , -103 , -100 , -97  , -93  , -90  , -87  , -83  , -80  , -77  , -73  , -70  , -67  , -63  ,
    -60  , -57  , -54  , -50  , -47  , -44  , -40  , -37  , -34  , -30  , -27  , -24  , -20  , -17  , -14  ,
    -10  , -7   , -4   , 0    , 3    , 6    , 10   , 13   , 16   , 20   , 23   , 26   , 30   , 33   , 36   ,
    39   , 43   , 46   , 49   , 53   , 56   , 59   , 63   , 66   , 69   , 73   , 76   , 79   , 83   , 86   ,
    89   , 93   , 96   , 99   , 103  , 106  , 109  , 113  , 116  , 119  , 123  , 126  , 129  , 132  , 136  ,
    139  , 142  , 146  , 149  , 152  , 156  , 159  , 162  , 166  , 169  , 172  , 176  , 179  , 182  , 186  ,
    189  , 192  , 196  , 199  , 202  , 206  , 209  , 212  , 216  , 219  , 222  , 226  , 229  , 232  , 235  ,
    239  , 242  , 245  , 249  , 252  , 255  , 259  , 262  , 265  , 269  , 272  , 275  , 279  , 282  , 285  ,
    289  , 292  , 295  , 299  , 302  , 305  , 309  , 312  , 315  , 319  , 322  , 325  , 328  , 332  , 335  ,
    338  , 342  , 345  , 348  , 352  , 355  , 358  , 362  , 365  , 368  , 372  , 375  , 378  , 382  , 385  ,
    388  , 392  , 395  , 398  , 402  , 405  , 408  , 412  , 415  , 418  , 422  , 425  , 428  , 431  , 435  ,
    438  , 441  , 445  , 448  , 451  , 455  , 458  , 461  , 465  , 468  , 471  , 475  , 478  , 481  , 485  ,
    488  , 491  , 495  , 498  , 501  , 505  , 508  , 511  , 515  , 518  , 521  , 524  , 528  , 531  , 534  ,
    538  , 541  , 544  , 548  , 551  , 554  , 558  , 561  , 564  , 568  , 571  , 574  , 578  , 581  , 584  ,
    588  , 591  , 594  , 598  , 601  , 604  , 608  , 611  , 614  , 617  , 621  , 624  , 627  , 631  , 634  ,
    637  , 641  , 644  , 647  , 651  , 654  , 657  , 661  , 664  , 667  , 671  , 674  , 677  , 681  , 684  ,
    687  , 691  , 694  , 697  , 701  , 704  , 707  , 711  , 714  , 717  , 720  , 724  , 727  , 730  , 734  ,
    737  , 740  , 744  , 747  , 750  , 754  , 757  , 760  , 764  , 767  , 770  , 774  , 777  , 780  , 784  ,
    787  , 790  , 794  , 797  , 800  , 804  , 807  , 810  , 813  , 817  , 820  , 823  , 827  , 830  , 833  ,
    837  , 840  , 843  , 847  , 850  , 853  , 857  , 860  , 863  , 867  , 870  , 873  , 877  , 880  , 883  ,
    887  , 890  , 893  , 897  , 900  , 903  , 907  , 910  , 913  , 916  , 920  , 923  , 926  , 930  , 933  ,
    936  , 940  , 943  , 946  , 950  , 953  , 956  , 960  , 963
};
#endif

static inline double ldouble_rconvert(uint64_t f, int32_t e)
{
/*
 * why 22? The bits of significand is 53, 1 << 53 = 9007199254740992, then 5 ** 23 = 11920928955078125,
 * 5 ** 22 = 2384185791015625. (5 ** 23) > (1 << 53), (5 ** 22) < (1 << 53), so max index is 22.
 */
#define POW10_LUT_EXACT_IDX     22
    if (e >= -POW10_LUT_EXACT_IDX && e <= POW10_LUT_EXACT_IDX) {
        static const double pow10_lut[POW10_LUT_EXACT_IDX + 1] = {
            1e0 , 1e1 , 1e2 , 1e3 , 1e4 , 1e5 , 1e6 , 1e7 , 1e8 , 1e9 ,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
            1e20, 1e21, 1e22
        };
        return (e < 0) ? (f / pow10_lut[-e]) : (f * pow10_lut[e]);
    }

    double d = 0;
    uint64_t *v = (uint64_t *)&d;

#if USING_SMALL_LUT
    if (e & 1) {
        if (f <= 1844674407370955161llu) { /* ((uint64_t)1 << 64) / 10 */
            f *= 10;
            e -= 1;
        } else {
            f /= 10;
            e += 1;
        }
    }
    e >>= 1;
#endif

    if (e <= POW2_LUT_MIN_IDX || e >= POW2_LUT_MAX_IDX) {
        *v = e <= POW2_LUT_MIN_IDX ? 0 : DP_EXPONENT_MASK;
        return d;
    }

    const int32_t index = e - POW2_LUT_MIN_IDX;
    const uint64_t pow2 = pow2_mul_lut[index];
    int32_t exponent = pow2_exp_lut[index];
    uint64_t significand = 0;

    int32_t bitn = u64_pz_get(f);
    f <<= bitn;
    exponent -= bitn;
    significand = u128_mul(f, pow2).hi;
    exponent += 64;
    bitn = 64 - u64_pz_get(significand) - (DP_SIGNIFICAND_SIZE + 1);
    exponent += bitn;

    if (significand & ((uint64_t)1 << (bitn - 1))) {
        significand >>= bitn;
        significand += 1;
        if (significand == ((uint64_t)1 << (DP_SIGNIFICAND_SIZE + 1))) {
            significand >>= 1;
            exponent += 1;
        }
    } else {
        significand >>= bitn;
    }

    exponent += DP_EXPONENT_OFFSET + DP_SIGNIFICAND_SIZE;
    if (exponent > 1) {
        if (exponent < DP_EXPONENT_MAX) {
            *v = (significand & DP_SIGNIFICAND_MASK) | ((uint64_t)exponent << DP_SIGNIFICAND_SIZE);
        } else {
            *v = DP_EXPONENT_MASK;
        }
    } else {
        if (exponent > -DP_SIGNIFICAND_SIZE) {
            exponent -= 1;
            *v = (significand >> -exponent) + !!(significand & ((uint64_t)1 << (-exponent - 1)));
        } else {
            *v = 0;
        }
    }
    return d;
}

#define IS_DIGIT(c)     ((c) >= '0' && (c) <= '9')

#if JNUM_MANUAL_LOOP_UNFOLD
static inline const char *_parse_19_digits(const char *str, uint64_t *value)
{
    const char *s = str;
    uint64_t m = *value;

    do {
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');

        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');

        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');

        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');

        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
        if (!IS_DIGIT(*s))
            break;
        m = (m << 3) + (m << 1) + (*s++ - '0');
    } while (0);

    *value = m;
    return s;
}
#endif

int jnum_parse_num(const char *str, jnum_type_t *type, jnum_value_t *value)
{
    const char *s = str, *t = NULL;
    int32_t eneg = 0, e = 0, k = 0, i = 0;
    uint64_t neg = 0, m = 0;
#if JNUM_MANUAL_LOOP_UNFOLD
    uint64_t n = 0;
#else
    int32_t idx = 0, max = 0;
#endif

    switch (*s) {
    case '-':
        neg = 1;
        FALLTHROUGH_ATTR;
    case '+':
        ++s;
        break;
    case '0':
        if (*(s + 1) == 'x' || *(s + 1) == 'X') {
            int len = jnum_parse_hex(s, type, value);
            if (len == 2) {
                *type = JNUM_INT;
                value->vint = 0;
                return 1;
            }
            return len;
        }
        break;
    default:
        break;
    }

    if (!IS_DIGIT(*s) && (*s != '.')) {
        *type = JNUM_NULL;
        value->vint = 0;
        return 0;
    }

    /* ---------- Parse integer ---------- */
    while (*s == '0')
        ++s;
#if JNUM_MANUAL_LOOP_UNFOLD
    t = _parse_19_digits(s, &m);
#else
    t = s;
    for (idx = 0; IS_DIGIT(*t) && (idx < 19); ++idx) {
        m = (m << 3) + (m << 1) + (*t++ - '0');
    }
#endif
    k = (int)(t - s);
    s = t;

    switch (*s) {
    case '.':
        /* ---------- Parse decimal ---------- */
        ++s;
        if (m == 0) {
            while (*s == '0') {
                ++s;
                --i;
            }
        }

#if JNUM_MANUAL_LOOP_UNFOLD
        n = m;
        t = _parse_19_digits(s, &m);
        if (k + (int)(t - s) < 20 && !IS_DIGIT(*t)) {
            k += (int)(t - s);
            i -= (int)(t - s);
            s = t;
        } else {
            m = n;
            while (IS_DIGIT(*s) && (k != 19)) {
                m = (m << 3) + (m << 1) + (*s++ - '0');
                ++k;
                --i;
            }
            while (IS_DIGIT(*s))
                ++s;
        }
#else
        t = s;
        max = 19 - k;
        for (idx = 0; IS_DIGIT(*t) && (idx < max); ++idx) {
            m = (m << 3) + (m << 1) + (*t++ - '0');
        }
        k += (int)(t - s);
        i -= (int)(t - s);
        s = t;

        if (unlikely(IS_DIGIT(*s))) {
            while (IS_DIGIT(*s))
                ++s;
        }
#endif
        break;

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        /* ---------- Parse very large integer ---------- */
        while (IS_DIGIT(*s)) {
            ++s;
            ++i;
        }
        if (*s == '.') {
            ++s;
            while (IS_DIGIT(*s))
                ++s;
        }
        break;
    case 'e': case 'E':
        break;
    default:
        goto end1;
    }

    if (*s == 'e' || *s == 'E') {
        /* ---------- Parse exponent ---------- */
        t = s;
        ++t;
        eneg = *t == '-';
        if (*t == '-' || *t == '+')
            ++t;
        if (unlikely(!IS_DIGIT(*t))) {
            if (i)
                goto end2;
            else
                goto end1;
        }
        s = t;

        while (*s == '0')
            ++s;

        do {
            if (!IS_DIGIT(*s))
                break;
            e = (e << 3) + (e << 1) + (*s++ - '0');
            if (!IS_DIGIT(*s))
                break;
            e = (e << 3) + (e << 1) + (*s++ - '0');
            if (!IS_DIGIT(*s))
                break;
            e = (e << 3) + (e << 1) + (*s++ - '0');
            if (!IS_DIGIT(*s))
                break;
            e = (e << 3) + (e << 1) + (*s++ - '0');
            while (IS_DIGIT(*s))
                ++s;
        } while (0);
        i += eneg == 0 ? e : -e;
    }

end2:
    *type = JNUM_DOUBLE;
    if (m == 0) {
        value->vlhex = neg << 63;
    } else {
        value->vdbl = ldouble_rconvert(m, i);
        value->vlhex |= neg << 63;
    }
    return (int)(s - str);

end1:
    if (m <= 2147483647U /*INT_MAX*/) {
        *type = JNUM_INT;
        value->vint = neg == 0 ? (int32_t)m : -((int32_t)m);
    } else if (m <= 9223372036854775807U /*LLONG_MAX*/) {
        *type = JNUM_LINT;
        value->vlint = neg == 0 ? (int64_t)m : -((int64_t)m);
    } else {
        if (m == 9223372036854775808U && neg == 1) {
            *type = JNUM_LINT;
#if defined(_MSC_VER)
            value->vlint = -9223372036854775807 - 1;
#else
            value->vlint = -m;
#endif
        } else {
            *type = JNUM_DOUBLE;
            value->vdbl = neg == 0 ? (double)m : -((double)m);
        }
    }
    return (int)(s - str);
}

#if defined(_MSC_VER)
jnum_to_func(int32_t, jnum_atoi)
jnum_to_func(int64_t, jnum_atol)
jnum_to_func(uint32_t, jnum_atoh)
jnum_to_func(uint64_t, jnum_atolh)
jnum_to_func(double, jnum_atod)
#endif


// Copyright 2026 xjb714 and contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ftoa.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

/*==============================================================================
 * Macros
 *============================================================================*/

// Auto inducted: platform detect
#if defined(__aarch64__) || defined(_M_ARM64)
#    define XJB_IS_AARCH64 1
#    define XJB_IS_X64 0
#elif defined(__x86_64__) || defined(_M_X64)
#    define XJB_IS_AARCH64 0
#    define XJB_IS_X64 1
#else
#    define XJB_IS_AARCH64 0
#    define XJB_IS_X64 0
#endif

/* Whether to use SIMD. */
#ifndef XJB_USE_SIMD
#    define XJB_USE_SIMD 1
#endif

/* Whether to use neon (aarch64). */
#ifndef XJB_USE_NEON
#    if XJB_USE_SIMD && XJB_IS_AARCH64 && (defined(__ARM_NEON__) || defined(__ARM_NEON))
#        include <arm_neon.h>
#        define XJB_USE_NEON 1
#    else
#        define XJB_USE_NEON 0
#    endif
#endif

/* Whether to use sse2 (x64). */
#ifndef XJB_USE_SSE2
#    if XJB_USE_SIMD && (defined(__GNUC__) && defined(__SSE2__) && defined(__amd64__))
#        include <immintrin.h>
#        define XJB_USE_SSE2 1
#    else
#        define XJB_USE_SSE2 0
#    endif
#endif

// Auto inducted: XJB_USE_NEON_OR_SSE2
#if XJB_USE_SSE2 || XJB_USE_NEON
#    define XJB_USE_NEON_OR_SSE2 1
#else
#    define XJB_USE_NEON_OR_SSE2 0
#endif

/* Whether to use ssse3 (x64). */
#ifndef XJB_USE_SSSE3
#    if XJB_USE_SSE2 && defined(__SSSE3__) && __SSSE3__
#        define XJB_USE_SSSE3 1
#    else
#        define XJB_USE_SSSE3 0
#    endif
#endif

/* Whether to use sse4.1 (x64). */
#ifndef XJB_USE_SSE4_1
#    if XJB_USE_SSE2 && defined(__SSE4_1__) && __SSE4_1__
#        define XJB_USE_SSE4_1 1
#    else
#        define XJB_USE_SSE4_1 0
#    endif
#endif

/* Whether to enable avx512ifma & avx512vbmi. */
#ifndef XJB_USE_AVX512IFMA_VBMI
#    if XJB_USE_SSE2 && defined(__AVX512F__) && defined(__AVX512IFMA__) && defined(__AVX512VBMI__) && __AVX512F__ && \
        __AVX512IFMA__ && __AVX512VBMI__
#        define XJB_USE_AVX512IFMA_VBMI 1
#    else
#        define XJB_USE_AVX512IFMA_VBMI 0
#    endif
#endif

/* Whether to enable "no memmove" feature. */
#ifndef XJB_NO_MEMMOVE
#    if XJB_USE_SSE2 && (XJB_USE_AVX512IFMA_VBMI || XJB_USE_SSSE3 || XJB_USE_SSE4_1)
// On x86-64 processors, xjb's memmove can trigger store-to-load forwarding stalls.
#        define XJB_NO_MEMMOVE 1
#    elif XJB_USE_NEON && defined(__APPLE__) && defined(__arm64__)
// On Apple Silicon processors, xjb's memmove can be regarded as a free performance gain.
#        define XJB_NO_MEMMOVE 0
#    elif XJB_USE_NEON
// On other aarch64 implementations the performance characteristics of memmove may differ.
#        define XJB_NO_MEMMOVE 1
#    else
#        define XJB_NO_MEMMOVE 0
#    endif
#endif

// Auto inducted: XJB_NOT_REMOVE_FIRST_ZERO.
#if XJB_NO_MEMMOVE || XJB_USE_NEON || XJB_USE_SSSE3
#    define XJB_NOT_REMOVE_FIRST_ZERO 1
#else
#    define XJB_NOT_REMOVE_FIRST_ZERO 0
#endif

/* Some compiler checks. */

// Is the compiler really GCC.
#ifndef XJB_IS_REAL_GCC
#    if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__clang__) && !defined(__llvm__) && \
        !defined(__INTEL_COMPILER) && !defined(__ICC)
#        define XJB_IS_REAL_GCC 1
#    endif
#endif

/* "assume" macro for better optimization. */
#ifdef NDEBUG
#    if defined(__GNUC__) || defined(__clang__)
#        define xjb_assume(cond)             \
            do {                             \
                if (!(cond))                 \
                    __builtin_unreachable(); \
            } while (0)
#    elif defined(_MSC_VER)
#        define xjb_assume(cond) __assume(cond)
#    else
#        define xjb_assume(cond) ((void)0)
#    endif
#else
#    define xjb_assume(cond) ((void)0)  // not use
#endif

#ifdef _MSC_VER
#    include <intrin.h>  // __lzcnt64/_umul128/__umulh
#endif

/*==============================================================================
 * Typedefs
 *============================================================================*/

#if defined(__SIZEOF_INT128__)
typedef __uint128_t u128;  // msvc not support
#endif

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;

/*==============================================================================
 * Utility
 *============================================================================*/

static inline constexpr uint64_t umul128_hi64_fallback(uint64_t x, uint64_t y) {
    // constexpr function for all compiler and hardware. just for const value
    // precalc , not a Runtime function.
    uint64_t a = x >> 32;
    uint64_t b = uint32_t(x);
    uint64_t c = y >> 32;
    uint64_t d = uint32_t(y);

    uint64_t ac = a * c;
    uint64_t bc = b * c;
    uint64_t ad = a * d;
    uint64_t bd = b * d;

    uint64_t cs = (bd >> 32) + uint32_t(ad) + uint32_t(bc);
    return ac + (ad >> 32) + (bc >> 32) + (cs >> 32);
}

static inline constexpr void umul128_hi64_lo64_fallback(uint64_t x, uint64_t y, uint64_t& hi, uint64_t& lo) {
    // constexpr function for all compiler and hardware. just for const value
    // precalc , not a Runtime function.
    uint64_t a = x >> 32;
    uint64_t b = uint32_t(x);
    uint64_t c = y >> 32;
    uint64_t d = uint32_t(y);

    uint64_t ac = a * c;
    uint64_t bc = b * c;
    uint64_t ad = a * d;
    uint64_t bd = b * d;

    uint64_t cs = (bd >> 32) + uint32_t(ad) + uint32_t(bc);
    hi = ac + (ad >> 32) + (bc >> 32) + (cs >> 32);
    lo = (cs << 32) + uint32_t(bd);
}

static inline void mul_u128_u64_high128(uint64_t a_high, uint64_t a_low, uint64_t b, uint64_t* result_high,
                                        uint64_t* result_mid) {
    uint64_t low_high, low_low;
    uint64_t high_high, high_low;
#if defined(_MSC_VER) && defined(_M_X64)
    low_low = _umul128(a_low, b, &low_high);
    high_low = _umul128(a_high, b, &high_high);
    unsigned char carry1 = _addcarry_u64(0, low_high, high_low, result_mid);
    _addcarry_u64(carry1, high_high, 0, result_high);
#elif defined(_MSC_VER) && defined(_M_ARM64)
    low_high = __umulh(a_low, b);
    high_high = __umulh(a_high, b);
    high_low = a_high * b;
    *result_mid = low_high + high_low;
    *result_high = high_high + (*result_mid < low_high);
#elif defined(__SIZEOF_INT128__)
    u128 hi128 = ((u128)b * a_high + (((u128)b * a_low) >> 64));
    *result_high = hi128 >> 64;
    *result_mid = (uint64_t)hi128;
#else
    umul128_hi64_lo64_fallback(a_low, b, low_high, low_low);
    umul128_hi64_lo64_fallback(a_high, b, high_high, high_low);
    *result_mid = low_high + high_low;
    *result_high = high_high + (*result_mid < low_high);
#endif
}

static inline int u64_lz_bits(uint64_t x) {
#if defined(__has_builtin) && __has_builtin(__builtin_clzll)
    return __builtin_clzll(x);
#elif defined(_MSC_VER) && defined(__AVX2__)
    // Use lzcnt only on AVX2-capable CPUs that have this BMI instruction.
    return __lzcnt64(x);
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanReverse64(&idx, x);  // Fallback to the BSR instruction.
    return 63 - idx;
#else
    int n = 64;
    for (; x > 0; x >>= 1)
        --n;
    return n;
#endif
}

static inline int u64_tz_bits(uint64_t x) {
#if defined(__has_builtin) && __has_builtin(__builtin_ctzll)
    return __builtin_ctzll(x);
#elif defined(_MSC_VER) && defined(__AVX2__)
    // Use lzcnt only on AVX2-capable CPUs that have this BMI instruction.
    return __tzcnt64(x);
#elif defined(_MSC_VER)
    unsigned long idx;
    if (_BitScanForward64(&idx, x)) {
        return (int)idx;
    } else {
        return 64;
    }
#else
    int n = 64;
    for (; x > 0; x <<= 1)
        --n;
    return n;
#endif
}

static inline uint64_t umul128_hi64_xjb(uint64_t a, uint64_t b) {
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM64))
    uint64_t hi = __umulh(a, b);
    return hi;
#elif defined(__SIZEOF_INT128__)
    return ((u128)a * b) >> 64;
#else
    uint64_t hi = umul128_hi64_fallback(a, b);
    return hi;
#endif
}

static inline uint64_t u128_madd_hi64(uint64_t a, uint64_t b, uint64_t c) {
    // this function return (a * b + c) >> 64;
#if defined(_MSC_VER) && defined(_M_X64)
    uint64_t hi, lo, null_value;
    lo = _umul128(a, b, &hi);
    unsigned char carry1 = _addcarry_u64(0, lo, c, &null_value);
    return hi + carry1;
#elif defined(_MSC_VER) && defined(_M_ARM64)
    uint64_t hi = __umulh(a, b);
    uint64_t lo = a * b;
    return hi + ((lo + c) < c);
#elif defined(__SIZEOF_INT128__)
    return ((u128)a * b + c) >> 64;
#else
    uint64_t hi, lo;
    umul128_hi64_lo64_fallback(a, b, hi, lo);
    return hi + ((lo + c) < c);
#endif
}

static inline uint64_t is_little_endian() {
    const int n = 1;
    return *(const char*)(&n) == 1;
}

typedef struct {
#if XJB_USE_NEON
    uint64x2_t ascii16;
#elif XJB_USE_SSE2
    __m128i ascii16;
#else
    uint64_t hi;
    uint64_t lo;
#endif
    uint64_t dec_sig_len;  // range : [1,17] - 1 = [0,16]
#if XJB_NO_MEMMOVE
    int32_t trailing_byte;
#endif
} shortest_ascii16;

typedef struct {
    uint64_t ascii;
    uint64_t dec_sig_len;  // range : [1,9] - 1 = [0,8]
} shortest_ascii8;

struct const_value_float {
    uint64_t c1;
    uint64_t div10000;
    uint64_t m;
    uint32_t e7;
    uint32_t e6;
    // uint32_t e5;
#if XJB_USE_NEON
    int32x4_t m32_4;
#else
    int32_t m32_4[4];
#endif
};

static const struct const_value_float constants_float = {
#if XJB_IS_AARCH64
    .c1 = (((u64)('0' + '0' * 256) << (36)) + (((u64)1 << (36 - 1)) - 7)),
#else
    .c1 = (((u64)('0' + '0' * 256) << (36 - 1)) + (((u64)1 << (36 - 2)) - 7)),
#endif
    .div10000 = 1844674407370956,
    .m = (1ULL << 32) - 10000,
    .e7 = 10000000,
    .e6 = 1000000,
    .m32_4 = {0x147b000, -100 + 0x10000, 0xce0, -10 + 0x100},
};

// size: 17728, align: 64
struct double_table_t {
    static constexpr int e10_DN = -4;
    static constexpr int e10_UP = 15;
    static constexpr int max_dec_sig_len = 17;
    static constexpr int num_pow10 = 323 - (-293) + 1;
    uint64_t pow10_double[(323 - (-293) + 1) * 2] = {};  // 1234 * 8 = 9872 bytes
    uint64_t exp_result_double[324 + 308 + 1] = {};      // 633 * 8 = 5064 bytes
    alignas(64) unsigned char e10_variable_data[e10_UP - e10_DN + 1 + 1][XJB_NO_MEMMOVE ? 64 : 32] = {};
    unsigned char h7[2048] = {};

    /* Assert size of per line in e10_variable_data is enough. */
    static_assert(32 >= max_dec_sig_len + 5, "");

    constexpr double_table_t() {
        struct uint192 {
            uint64_t w0, w1, w2;
        };
        uint192 current = {0xb2e28cedd086d011, 0x1e53ed49a96272c8, 0xcc5fc196fefd7d0c};  // e10 = -293
        constexpr uint64_t ten = 0xa000000000000000;
        for (int i = 0; i < num_pow10; ++i) {
            int e10 = i - 293;
            pow10_double[(num_pow10 - 1 - i) * 2 + 0] = e10 == 0 ? 1ULL << 63 : current.w2 + (e10 >= 0 && e10 <= 27);
            pow10_double[(num_pow10 - 1 - i) * 2 + 1] = current.w1 + 1;
            uint64_t h0 = umul128_hi64_fallback(current.w0, ten);
            uint64_t h1 = umul128_hi64_fallback(current.w1, ten);
            uint64_t c0 = h0 + current.w1 * ten;
            uint64_t c1 = (c0 < h0) + h1 + current.w2 * ten;
            uint64_t c2 = (c1 < h1) + umul128_hi64_fallback(current.w2, ten);
            if (c2 >> 63)
                current = {c0, c1, c2};
            else
                current = {c0 << 1, c1 << 1 | c0 >> 63, c2 << 1 | c1 >> 63};  // left shift 1 bit
        }
        for (int e10 = -324; e10 <= 308; ++e10) {
            u64 e = e10 < 0 ? ('e' + '-' * 256) : 'e' + '+' * 256;
            u64 e10_abs = e10 < 0 ? -e10 : e10;
            u64 a = e10_abs / 100;
            u64 bc = e10_abs - a * 100;
            u64 b = bc / 10;
            u64 c = bc - b * 10;
            u64 exp_len = 4 + (e10_abs >= 100);
            u64 e10_abs_ascii =
                (e10_abs >= 100) ? (a + '0') + ((b + '0') << 8) + ((c + '0') << 16) : (b + '0') + ((c + '0') << 8);
            u64 exp_res = e + (e10_abs_ascii << 16) + (exp_len << 56);
            exp_res = (e10 >= e10_DN && e10 <= e10_UP) ? 0 : exp_res;
            exp_result_double[e10 + 324] = exp_res;
        }
        for (int e10 = e10_DN; e10 <= e10_UP + 1; e10++) {
            int tmp_data_ofs = e10 - e10_DN;
            u64 first_sig_pos = (e10_DN <= e10 && e10 <= -1) ? 1 - e10 : 0;
            // For code audit.
            if (first_sig_pos > 5)
                throw "first_sig_pos is not larger than 5";
            u64 dot_pos = (0 <= e10 && e10 <= e10_UP) ? 1 + e10 : 1;
            u64 move_pos = dot_pos + (0 <= e10 || e10 < e10_DN);
            e10_variable_data[tmp_data_ofs][max_dec_sig_len + 0] = first_sig_pos;
            e10_variable_data[tmp_data_ofs][max_dec_sig_len + 1] = dot_pos;
            e10_variable_data[tmp_data_ofs][max_dec_sig_len + 2] = move_pos;
            for (uint8_t D17 = 0; D17 <= 1; D17++) {
                unsigned char one_offset = 15 + D17 + (move_pos > dot_pos && dot_pos <= 15 + D17);
                e10_variable_data[tmp_data_ofs][max_dec_sig_len + 3 + D17] = one_offset;
                // For code audit.
                if (one_offset > 17)
                    throw "one_offset is not larger than 17";
            }
            for (int dec_sig_len = 1; dec_sig_len <= max_dec_sig_len; dec_sig_len++) {
                u64 exp_pos = (e10_DN <= e10 && e10 <= -1)
                                  ? dec_sig_len
                                  : (0 <= e10 && e10 <= e10_UP ? (e10 + 3 > dec_sig_len + 1 ? e10 + 3 : dec_sig_len + 1)
                                                               : (dec_sig_len + 1 - (dec_sig_len == 1)));
                e10_variable_data[tmp_data_ofs][dec_sig_len - 1] = exp_pos;
            }
            if (XJB_NO_MEMMOVE) {
                uint8_t v = 0xf;
                for (uint64_t j = 0; j < 0x10; ++j)
                    e10_variable_data[tmp_data_ofs][0x20 + 0x10 + j] = v--;
                if (move_pos > dot_pos) {
                    for (uint64_t j = 0xf; j > dot_pos && j > 0; --j)
                        e10_variable_data[tmp_data_ofs][j + 0x20 + 0x10] =
                            e10_variable_data[tmp_data_ofs][j + 0x20 + 0x10 - 1];
                }
                for (uint64_t j = 0; j < 0x10; ++j) {
                    auto v = e10_variable_data[tmp_data_ofs][j + 0x20 + 0x10];
                    e10_variable_data[tmp_data_ofs][j + 0x20] = v ? (v - 1) : 0xf;
                }
            }
        }
        for (int exp = 0; exp < 2048; ++exp) {
            const int offset = 9;
            int q = exp - 1075 + (exp == 0);
            int k = (q * 78913) >> 18;
            int h = q + (((-k - 1) * 217707) >> 16);
            h7[exp] = (unsigned char)(h + 1 + offset);
        }
    }
};

/* const value table for float/double to string : begin */
struct const_value_double {
    static constexpr int e10_DN = double_table_t::e10_DN;
    static constexpr int e10_UP = double_table_t::e10_UP;
    static constexpr int max_dec_sig_len = double_table_t::max_dec_sig_len;
    static constexpr int num_pow10 = double_table_t::num_pow10;
    uint64_t c1 = 78913ull << (64 - 18);
    uint64_t c2 = (uint64_t)-217707;
    uint64_t c3 = (uint64_t)1e15 - 1;
    uint64_t c4 = (1ULL << 63) + 6;
    uint64_t c5 = (uint64_t)-131072;
    uint64_t c6 = (1 << 9) - 1;
    uint64_t mul_const = 0xabcc77118461cefd;
    int64_t hundred_million = (int64_t)-100000000;
    uint64_t div10000 = 1844674407370956;
    uint64_t div10000_m = 0x100000000 - 10000;
    double div10000_2_d = (double)(-10000 + 0x100000000);
    int64_t div10000_2 = 0xd1b7176000;
#if XJB_USE_NEON
    int32x4_t multipliers32 = {0x68db8bb, -10000 + 0x10000, 0x147b000, -100 + 0x10000};  // 16
    int16x8_t multipliers16 = {0xce0, -10 + 0x100, '0' + '0' * 256};                     // 16
#else
    int32_t multipliers32[4] = {0x68db8bb, -10000 + 0x10000, 0x147b000, -100 + 0x10000};  // 16
    int16_t multipliers16[8] = {0xce0, -10 + 0x100, '0' + '0' * 256};                     // 16
#endif
#if XJB_USE_NEON
    uint8_t shuffle_table_neon[32] = {
        6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8,
    };
    uint8_t shuffle_table_memmove[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0,
                                         0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  10, 11, 12, 13, 14, 15};
#endif
#if XJB_USE_NEON && XJB_NO_MEMMOVE
    uint8_t reverse_shuffle_table[17] = {0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
#endif
#if (XJB_NOT_REMOVE_FIRST_ZERO && XJB_USE_SSSE3) || XJB_USE_NEON
    uint8_t shuffle_table[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
#endif
};

struct float_table_t {
    static const int e10_DN = -3;
    static const int e10_UP = 6;
    static const int max_dec_sig_len = 9;
    static const int num_pow10 = 44 - (-32) + 1;
    uint64_t pow10_float_reverse[44 - (-32) + 1] = {};
    uint32_t exp_result_float[45 + 38 + 1] = {};
    unsigned char e10_variable_data[e10_UP - (e10_DN) + 1 + 1][16] = {};
    unsigned char h37[256] = {};

    /* Assert size of per line in e10_variable_data is enough. */
    static_assert(16 >= max_dec_sig_len + 3, "");

    struct const_value_float constants_float = {
#if XJB_IS_AARCH64
        .c1 = (((u64)('0' + '0' * 256) << (36)) + (((u64)1 << (36 - 1)) - 7)),
#else
        .c1 = (((u64)('0' + '0' * 256) << (36 - 1)) + (((u64)1 << (36 - 2)) - 7)),
#endif
        .div10000 = 1844674407370956,
        .m = (1ULL << 32) - 10000,
        .e7 = 10000000,
        .e6 = 1000000,
        .m32_4 = {0x147b000, -100 + 0x10000, 0xce0, -10 + 0x100},
    };
    constexpr float_table_t() {
        struct uint128_xjb {
            uint64_t w0, w1;
        };
        uint128_xjb current = {0x67de18eda5814af3, 0xcfb11ead453994ba};  // e10 = -32
        uint64_t ten = 0xa000000000000000;
        for (int i = 0; i < num_pow10; ++i) {
            int e10 = i - 32;
            pow10_float_reverse[num_pow10 - i - 1] = e10 == 0 ? 1ULL << 63 : current.w1 + 1;
            uint64_t h0 = umul128_hi64_fallback(current.w0, ten);
            uint64_t c0 = h0 + current.w1 * ten;
            uint64_t c1 = (c0 < h0) + umul128_hi64_fallback(current.w1, ten);
            if (c1 >> 63)
                current = {c0, c1};
            else
                current = {c0 << 1, c1 << 1 | c0 >> 63};  // left shift 1 bit
        }
        for (int e10 = -45; e10 <= 38; e10++) {
            u64 e = e10 < 0 ? ('e' + '-' * 256) : ('e' + '+' * 256);
            u64 e10_abs = e10 < 0 ? -e10 : e10;
            u64 a = e10_abs / 10;
            u64 b = e10_abs - a * 10;
            u64 e10_abs_ascii = (a + '0') + ((b + '0') << 8);
            u64 exp_res = e + (e10_abs_ascii << 16);
            exp_res = (e10 >= e10_DN && e10 <= e10_UP) ? 0 : exp_res;
            exp_result_float[e10 + 45] = (uint32_t)exp_res;
        }
        for (int e10 = e10_DN; e10 <= e10_UP + 1; e10++) {
            int tmp_data_ofs = e10 - e10_DN;
            u64 first_sig_pos = (e10_DN <= e10 && e10 <= -1) ? 1 - e10 : 0;
            u64 dot_pos = (0 <= e10 && e10 <= e10_UP) ? 1 + e10 : 1;
            u64 move_pos = dot_pos + ((0 <= e10 || e10 < e10_DN));
            e10_variable_data[tmp_data_ofs][max_dec_sig_len + 0] = first_sig_pos;
            e10_variable_data[tmp_data_ofs][max_dec_sig_len + 1] = dot_pos;
            e10_variable_data[tmp_data_ofs][max_dec_sig_len + 2] = move_pos;
            for (int dec_sig_len = 1; dec_sig_len <= max_dec_sig_len; dec_sig_len++) {
                u64 exp_pos = (e10_DN <= e10 && e10 <= -1)
                                  ? dec_sig_len
                                  : (0 <= e10 && e10 <= e10_UP ? (e10 + 3 > dec_sig_len + 1 ? e10 + 3 : dec_sig_len + 1)
                                                               : (dec_sig_len + 1 - (dec_sig_len == 1)));
                e10_variable_data[tmp_data_ofs][dec_sig_len - 1] = exp_pos;
            }
        }
        for (int exp = 0; exp < 256; exp++) {
            int exp_bin = exp - 150 + (exp == 0);
            int k = (exp_bin * 1233) >> 12;
            int h37_precalc = (36 + 1) + exp_bin + ((k * -1701 + (-1701)) >> 9);
            h37[exp] = (unsigned char)h37_precalc;
        }
    }
};

static inline uint64_t byteswap64_xjb(uint64_t x) {
#if defined(__has_builtin) && __has_builtin(__builtin_bswap64)
    return __builtin_bswap64(x);
#elif defined(_MSC_VER)
    return _byteswap_uint64(x);
#else
    return ((x & 0xff00000000000000) >> 56) | ((x & 0x00ff000000000000) >> 40) | ((x & 0x0000ff0000000000) >> 24) |
           ((x & 0x000000ff00000000) >> 8) | ((x & 0x00000000ff000000) << 8) | ((x & 0x0000000000ff0000) << 24) |
           ((x & 0x000000000000ff00) << 40) | ((x & 0x00000000000000ff) << 56);
#endif
}

static inline uint64_t cmov_branchless(uint64_t condition, uint64_t true_value, uint64_t false_value) {
#if !XJB_IS_REAL_GCC || !defined(__amd64__)
    return condition ? true_value : false_value;
#else
    // only amd64 , gcc compiler use this branchless cmov method
    asm volatile(
        "test %2, %2\n\t"
        "cmovne %1, %0\n\t"
        : "+r"(false_value)
        : "r"(true_value), "r"(condition)
        : "cc");
    return false_value;
#endif
}

static inline uint64_t compute_double_dec_sig_len(uint64_t up_down, int tz, uint64_t D17) {
    uint64_t ret = cmov_branchless(up_down, (XJB_NOT_REMOVE_FIRST_ZERO ? 14 + D17 : 15) - (tz), 15 + D17);
    xjb_assume(ret <= 16);
    return ret;
}

static inline uint64_t compute_double_dec_sig_len_ssse3(uint64_t up_down, int tz, uint64_t D17) {
    uint64_t ret = cmov_branchless(up_down, (XJB_NOT_REMOVE_FIRST_ZERO ? 14 + D17 : 15) - tz, 15 + D17);
    xjb_assume(ret <= 16);
    return ret;
}

static inline uint64_t compute_double_dec_sig_len_sse2(uint64_t up_down, int tz_add_48, uint64_t D17) {
    uint64_t ret = cmov_branchless(up_down, (0 ? 14 + D17 : 15) + 48 - tz_add_48, 15 + D17);
    xjb_assume(ret <= 16);
    return ret;
}

static inline uint64_t compute_float_dec_sig_len(uint64_t up_down, int tz, uint64_t lz) {
    uint64_t ret = cmov_branchless(up_down, (7 - lz) - tz, 8 - lz);
    xjb_assume(ret <= 8);
    return ret;
}

/*==============================================================================
 * Constants
 *============================================================================*/
static const struct const_value_double constants_double;
alignas(64) constexpr double_table_t double_table;
alignas(64) constexpr float_table_t float_table;

/*==============================================================================
 * Implementations
 *============================================================================*/

#if XJB_NO_MEMMOVE

#    if XJB_USE_SSE4_1
static inline shortest_ascii16 to_ascii16_no_memmove_sse41(char* buf, uint64_t up_down, uint64_t D17, __m128i y,
                                                           __m128i move_shuffler) {
    __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1 << 16) - 100),
                                                 _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3)));
    __m128i bcd_swapped =
        _mm_add_epi16(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a))));
    __m128i ascii16_swapped = _mm_add_epi8(bcd_swapped, _mm_set1_epi8('0'));
    //_mm_storeu_si128((__m128i*)buf, _mm_set1_epi8('0'));
    __m128i ascii16 = _mm_shuffle_epi8(ascii16_swapped, move_shuffler);
    int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(bcd_swapped, _mm_setzero_si128()));
    int tz = u64_tz_bits(mask);
    return {ascii16, compute_double_dec_sig_len_ssse3(up_down, tz, D17), _mm_cvtsi128_si32(ascii16_swapped)};
}
#    endif

#    if XJB_USE_SSSE3
static inline shortest_ascii16 to_ascii16_no_memmove_ssse3(uint64_t up_down, uint64_t D17, __m128i y,
                                                           __m128i move_shuffler) {
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
    __m128i bcd_swapped =
        _mm_add_epi16(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a))));
    __m128i ascii16_swapped = _mm_add_epi8(bcd_swapped, _mm_set1_epi8('0'));
    __m128i ascii16 = _mm_shuffle_epi8(ascii16_swapped, move_shuffler);
    int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(bcd_swapped, _mm_setzero_si128()));
    int tz = u64_tz_bits(mask);
    return {ascii16, compute_double_dec_sig_len_ssse3(up_down, tz, D17), _mm_cvtsi128_si32(ascii16_swapped)};
}
#    endif
#endif

#if XJB_USE_SSE2 && !XJB_NO_MEMMOVE

static inline shortest_ascii16 to_ascii16_memmove_sse2(char* buf, uint64_t up_down, uint64_t D17,
                                                       const struct const_value_double* cv, __m128i z) {
    __m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
    __m128i bcd_swapped = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
    __m128i little_endian_bcd = _mm_shuffle_epi32(bcd_swapped, _MM_SHUFFLE(0, 1, 2, 3));

#    if XJB_NOT_REMOVE_FIRST_ZERO && XJB_USE_SSSE3
    little_endian_bcd =
        _mm_shuffle_epi8(little_endian_bcd,
                         // D17?_mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
                         // 0)
                         //    :_mm_set_epi8(0, 15, 14, 13, 12, 11, 10, 9, 8, 7,  6,  5,  4,  3,
                         //    2, 1));
                         _mm_loadu_si128((const __m128i*)(&(cv->shuffle_table[D17 ? 0 : 1]))));  // remove left zero
#    endif

    int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
    int tz = u64_lz_bits(mask);
    __m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
    _mm_storeu_si128((__m128i*)buf, _mm_set1_epi8('0'));  // write 32bte '0'
    _mm_storeu_si128((__m128i*)(buf + 16), _mm_set1_epi8('0'));
    return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
}

static inline shortest_ascii16 to_ascii16_sse_fallback(char* buf, const uint64_t up_down, const uint64_t D17,
                                                       const struct const_value_double* cv, __m128i y) {
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));

    return to_ascii16_memmove_sse2(buf, up_down, D17, cv, z);
}

#endif

#if XJB_USE_SSE2
static inline shortest_ascii16 to_ascii16_no_avx512(char* buf, const uint64_t m, const uint64_t up_down,
                                                    const uint64_t D17, const struct const_value_double* cv,
                                                    uint32_t abcdefgh, uint32_t ijklmnop
#    if XJB_NO_MEMMOVE
                                                    ,
                                                    const u8* move_shuffler
#    endif
) {

    __m128i x = _mm_unpacklo_epi64(_mm_cvtsi32_si128(ijklmnop), _mm_cvtsi32_si128(abcdefgh));
    __m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ULL << 32) - 10000),
                                               _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#    if XJB_USE_SSE4_1
#        if XJB_NO_MEMMOVE
    return to_ascii16_no_memmove_sse41(buf, up_down, D17, y, _mm_loadu_si128((__m128i*)move_shuffler));
#        else
    __m128i z = _mm_sub_epi32(
        _mm_slli_epi32(y, 16),
        _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
    return to_ascii16_memmove_sse2(buf, up_down, D17, cv, z);
#        endif
#    elif XJB_USE_SSSE3
#        if XJB_NO_MEMMOVE
    return to_ascii16_no_memmove_ssse3(up_down, D17, y, _mm_loadu_si128((__m128i*)move_shuffler));
#        else
    return to_ascii16_sse_fallback(buf, up_down, D17, cv, y);
#        endif

#    else  // SSE2
    return to_ascii16_sse_fallback(buf, up_down, D17, cv, y);

#    endif  // SSE2
}
#endif  // XJB_USE_SSE2
static inline uint64_t rotateRight64(uint64_t x, int s) {
    return (x >> s) | (x << (64 - s));
}
static inline uint32_t countZeros(uint64_t x) {
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
    uint32_t tz = 0;
    // Cut 8 zeros, then 4, then 2, then 1.
    if ((d = rotateRight64(x * div1e8m, 8)) <= div1e8le) {
        x = d;
        tz += 8;
    }
    if ((d = rotateRight64(x * div1e4m, 4)) <= div1e4le) {
        x = d;
        tz += 4;
    }
    if ((d = rotateRight64(x * div1e2m, 2)) <= div1e2le) {
        x = d;
        tz += 2;
    }
    if ((d = rotateRight64(x * div1e1m, 1)) <= div1e1le) {
        x = d;
        tz += 1;
    }
    return tz;
}

static inline shortest_ascii16 to_ascii16(char* buf, const uint64_t m, const uint64_t up_down, const uint64_t D17,
                                          const struct const_value_double* cv
#if XJB_NO_MEMMOVE
                                          ,
                                          const u8* move_shuffler
#endif
) {
    // m range : [1, 1e16 - 1] ; m = abcdefgh * 10^8 + ijklmnop
    const uint64_t ZERO = 0x3030303030303030ull;
    uint32_t abcdefgh = umul128_hi64_xjb(m, cv->mul_const) >> (90 - 64);
    int64_t hundred_million = cv->hundred_million;
#if XJB_IS_AARCH64 && (defined(__clang__) || defined(__GNUC__))
    asm("" : "+r"(hundred_million));
#endif
    uint32_t ijklmnop = m + abcdefgh * hundred_million;

    //  src from:
    //  https://gist.github.com/dougallj/b4f600ab30ef79bb6789bc3f86cd597a#file-convert-neon-cpp-L144-L169
    //  blog:
    //  https://dougallj.wordpress.com/2022/04/01/converting-integers-to-fixed-width-strings-faster-with-neon-simd-on-the-apple-m1/
    //  author : https://github.com/dougallj
#if XJB_USE_NEON

#    if XJB_NO_MEMMOVE
    uint64x1_t hundredmillions = {ijklmnop | ((uint64_t)abcdefgh << 32)};
#    else
    uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
#    endif
    int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(cv->multipliers32[0])), 9);
    int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(cv->multipliers32[1]));
    int32x4_t extended = vshll_n_u16(tenthousands, 0);
#    if XJB_IS_AARCH64 && (defined(__clang__) || defined(__GNUC__))
    asm("" : "+w"(extended));
#    endif
    int32x4_t high_100 = vqdmulhq_s32(extended, vdupq_n_s32(cv->multipliers32[2]));
    int32x4_t hundreds = vmlaq_s32(extended, high_100, vdupq_n_s32(cv->multipliers32[3]));
    int16x8_t high_10 = vqdmulhq_s16(hundreds, vdupq_n_s16(cv->multipliers16[0]));
    int16x8_t BCD_big_endian = vmlaq_s16(hundreds, high_10, vdupq_n_s16(cv->multipliers16[1]));
#    if XJB_NO_MEMMOVE
    // BCD_big_endian -> is_not_zero -> zeroes -> tz (rbit and lz)
    int8x16_t ascii16_swapped = vorrq_u8(BCD_big_endian, vdupq_n_s8('0'));
    uint16x8_t is_not_zero = vcgtzq_s8(BCD_big_endian);
    int8x16_t ascii16 = vqtbl1q_u8(ascii16_swapped, vld1q_u8(move_shuffler));
    uint64_t zeroes = vget_lane_u64(vreinterpret_u64_u8(vshrn_n_u16(is_not_zero, 4)), 0);
    int tz = u64_tz_bits(zeroes) >> 2;  // tz is slow on arm64 . tz = rbit and lz; two instruction.
    uint64_t dec_sig_len = up_down ? (XJB_NOT_REMOVE_FIRST_ZERO ? (14 + D17) - tz : 15 - tz) : 15 + D17;
    xjb_assume(dec_sig_len <= 16);
    return {ascii16, dec_sig_len, vgetq_lane_s32(ascii16_swapped, 0)};
#    else

    // int8x16_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    // int16x8_t ascii16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    // ascii16 = vqtbl1q_u8(vreinterpretq_u8_u16(ascii16),
    //                      vld1q_u8(&cv->shuffle_table_memmove[(15 + D17) & 16]));  // remove left zero

    int8x16_t BCD_little_endian = vqtbl1q_u8(vreinterpretq_u8_u16(BCD_big_endian),
                                             vld1q_u8(&cv->shuffle_table_neon[(15 + D17) & 16]));  // remove left zero
    int16x8_t ascii16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    vst1q_s8((int8_t*)buf, vdupq_n_s8('0'));  // write 32byte '0'
    uint16x8_t is_not_zero = vcgtzq_s8(BCD_little_endian);
    uint64_t zeroes = vget_lane_u64(vreinterpret_u64_u8(vshrn_n_u16(is_not_zero, 4)), 0);  // zeros != 0
    u32 tz = u64_lz_bits(zeroes) >> 2;
    uint32_t dec_sig_len = up_down ? ((0) ? (14 + D17) - tz : 15 - tz) : 15 + D17;
    xjb_assume(dec_sig_len <= 16);
    return {ascii16, dec_sig_len};
#    endif  // endif XJB_USE_NEON
#endif

#if XJB_USE_SSE2
    // method 1 : AVX512IFMA, AVX512VBMI
#    if XJB_USE_AVX512IFMA_VBMI
    const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
    const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
    const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
    const __m512i zmmTen = _mm512_set1_epi64(10);
    const __m512i zero = _mm512_set1_epi64(0);
    const __m512i asciizero = _mm512_set1_epi8('0');
    const __m512i ifma_const =
        _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
                          0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
    __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
    __m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);

#        if XJB_NO_MEMMOVE
    const __m512i permb_const_reverse = _mm512_castsi128_si512(
        _mm_setr_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
    __m512i bcd_r = _mm512_permutex2var_epi8(highbits_h, permb_const_reverse, highbits_l);
    __m128i bcd_swapped = _mm512_castsi512_si128(bcd_r);
    __m128i ascii16_swapped = _mm_add_epi8(bcd_swapped, _mm_set1_epi8('0'));
    __m128i ascii16 = _mm_shuffle_epi8(ascii16_swapped, _mm_loadu_si128((__m128i*)move_shuffler));
    int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(bcd_swapped, _mm_setzero_si128()));
    int tz = u64_tz_bits(mask);
    return {ascii16, compute_double_dec_sig_len_ssse3(up_down, tz, D17), _mm_cvtsi128_si32(ascii16_swapped)};
#        else
    const __m512i permb_const = _mm512_castsi128_si512(
        _mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
    __m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
    __m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
#            if XJB_NOT_REMOVE_FIRST_ZERO && XJB_USE_SSSE3
    little_endian_bcd =
        _mm_shuffle_epi8(little_endian_bcd,
                         _mm_loadu_si128((const __m128i*)(&(cv->shuffle_table[D17 ? 0 : 1]))));  // remove left zero
#            endif

    __m128i little_endian_ascii = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
    _mm_storeu_si128((__m128i*)buf, _mm_set1_epi8('0'));  // write 32byte '0'
    _mm_storeu_si128((__m128i*)(buf + 16), _mm_set1_epi8('0'));
    int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm512_castsi512_si128(zero)));
    int tz = u64_lz_bits(mask);
    return {little_endian_ascii, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#        endif

#    else  // x86-64, no AVX512
    return to_ascii16_no_avx512(buf, m, up_down, D17, cv, abcdefgh, ijklmnop
#        if XJB_NO_MEMMOVE
                                ,
                                move_shuffler
#        endif
    );

#    endif  // XJB_USE_SSE2
#endif      // endif XJB_USE_SSE2

#if !XJB_USE_NEON_OR_SSE2
    uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bbull) >> 40);
    uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bbull) >> 40);
    uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f);
    uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f);
    int abcdefgh_tz = u64_tz_bits(a_b_c_d_e_f_g_h);
    int ijklmnop_tz = u64_tz_bits(i_j_k_l_m_n_o_p);
    uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64_xjb(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
    uint64_t ijklmnop_bcd = is_little_endian() ? byteswap64_xjb(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;
    int tz = (ijklmnop == 0) ? 64 + abcdefgh_tz : ijklmnop_tz;
    tz = tz / 8;
    memcpy(buf, &ZERO, 8);  // write 32bte '0'
    memcpy(buf + 8, &ZERO, 8);
    memcpy(buf + 16, &ZERO, 8);
    memcpy(buf + 24, &ZERO, 8);
    return {abcdefgh_bcd | ZERO, ijklmnop_bcd | ZERO, compute_double_dec_sig_len(up_down, tz, D17)};
#endif
}

static inline shortest_ascii8 to_ascii8(const uint64_t m, const uint32_t up_down, uint32_t& lz,
                                        const struct const_value_float* c = nullptr) {
    // m range : [0, 1e8 - 1] ; m = abcdefgh
    const uint64_t ZERO = 0x3030303030303030;
#if XJB_USE_NEON
    int32x2_t tenthousands = vcreate_u64(m + c->m * ((m * (u128)c->div10000) >> 64));
    int32x2_t hundreds = vmla_n_s32(tenthousands, vqdmulh_s32(tenthousands, vdup_n_s32(c->m32_4[0])), c->m32_4[1]);
    // int16x4_t BCD_big_endian = vmla_n_s16(hundreds, vqdmulh_s16(hundreds,
    // vdup_n_s16(0xce0)), -10 + 0x100);
    int16x4_t BCD_big_endian = vmla_n_s16(hundreds, vqdmulh_s16(hundreds, vdup_n_s16(c->m32_4[2])),
                                          c->m32_4[3]);  // fewer instructions but slower,why?
    u64 hgfedcba_BCD = vget_lane_u64(BCD_big_endian, 0);
    u64 abcdefgh_BCD =
        byteswap64_xjb(vget_lane_u64(BCD_big_endian, 0));  // big_endian to little_endian , reverse 8 bytes
#endif

#if XJB_USE_SSE2

#    if XJB_USE_AVX512IFMA_VBMI
    __m512i bcstq_l = _mm512_set1_epi64(m);
    const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
    const __m512i ifma_const =
        _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
                          0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
    const u64 idx7 =
        7 + (15 << 8) + (23 << 16) + (31 << 24) + (39ull << 32) + (47ull << 40) + (55ull << 48) + (63ull << 56);
    const __m512i permb_const7 = _mm512_set1_epi64(idx7);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_l7 = _mm512_mullo_epi64(lowbits_l, _mm512_set1_epi64(10 << 4));
    __m512i bcd = _mm512_permutexvar_epi8(permb_const7, highbits_l7);
    u64 abcdefgh_BCD = _mm_cvtsi128_si64(_mm512_castsi512_si128(bcd));
#    else
    u64 aabb_ccdd_merge = (m << 32) + (1 - (10000ull << 32)) * ((m * (u128)1844674407370956) >> 64);
    __m128i y = _mm_set1_epi64x(aabb_ccdd_merge);
#        if XJB_USE_SSE4_1
    __m128i z = _mm_sub_epi32(
        _mm_slli_epi32(y, 16),
        _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#        else
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epi16(y, _mm_set1_epi16(0x147b)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#        endif
    __m128i z_div_10 = _mm_mulhi_epi16(z, _mm_set1_epi16(0x199a));
    __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
    u64 abcdefgh_BCD = _mm_cvtsi128_si64(tmp);
#    endif

#endif  // endif XJB_USE_SSE2

#if !XJB_USE_NEON_OR_SSE2
    i64 aabb_ccdd_merge = (m << 32) + (1 - (10000ull << 32)) * ((m * 109951163) >> 40);
    i64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) +
                            (1 - (100ull << 16)) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 abcdefgh_BCD =
        (aa_bb_cc_dd_merge << 8) + (1 - (10ull << 8)) * (((aa_bb_cc_dd_merge * 103) >> 10) &
                                                         ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
#endif
    u32 tz = u64_lz_bits(abcdefgh_BCD) >> 3;
    abcdefgh_BCD = abcdefgh_BCD >> (lz << 3);
    abcdefgh_BCD = is_little_endian() ? abcdefgh_BCD : byteswap64_xjb(abcdefgh_BCD);
    // when m = 0, abcdefgh_BCD = 0, tz = u64_lz_bits(0) >> 3, this is an UB,
    // but up_down is always 0, so we can dont care about it. when up_down = 0,
    // the tz is not used. so next line is always correct.
    return {abcdefgh_BCD | ZERO, cmov_branchless(up_down, (7 ^ lz) - tz, 8 - lz)};  // 7^lz == 7-lz
}
static inline char* write_1_to_16_digit(u64 x, char* buf, const struct const_value_double* cv) {
    // require 1 <= x < 1e16
    const u64 ZERO = 0x3030303030303030;
    const u64 mask = (0x7FULL << 32) | 0x7FULL;
    const u64 mask2 = (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
    i64 xi = x;
    if (x < 100000000) {
        // write 1-8 digit
#if XJB_USE_NEON
        u64 abcd_efgh_u64 = xi + cv->div10000_m * ((xi * (u128)cv->div10000) >> 64);  // xi = abcdefgh
        int32x2_t abcd_efgh = vld1_u64((uint64_t const*)&abcd_efgh_u64);              // (abcd << 32) + efgh
        int32x2_t ab_cd_ef_gh = vmla_s32(abcd_efgh, vqdmulh_s32(abcd_efgh, vdup_n_s32(cv->multipliers32[2])),
                                         vdup_n_s32(cv->multipliers32[3]));
        int16x4_t a_b_c_d_e_f_g_h =
            vmla_s16(ab_cd_ef_gh, vqdmulh_s16(ab_cd_ef_gh, vdup_n_s16(0xce0)), vdup_n_s16(-10 + 0x100));
        u64 bcd_big_endian = vget_lane_u64(a_b_c_d_e_f_g_h, 0);
        u64 lz = u64_lz_bits(bcd_big_endian) / 8;  // lz max is 7 , bcd_big_endian = 0 is impossible
        u64 abcdefgh_bcd = is_little_endian() ? byteswap64_xjb(bcd_big_endian) : bcd_big_endian;
        u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
        abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz);
        memcpy(buf, &abcdefgh_ascii, 8);
        buf = buf + 8 - lz;
        memcpy(buf, ".0\0", 4);
        buf += 2;
        return buf;
#elif XJB_USE_SSE2

#    if XJB_USE_AVX512IFMA_VBMI
        __m512i bcstq_l = _mm512_set1_epi64(xi);
        const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
        const __m512i ifma_const =
            _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
                              0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
        const u64 idx7 =
            7 + (15 << 8) + (23 << 16) + (31 << 24) + (39ull << 32) + (47ull << 40) + (55ull << 48) + (63ull << 56);
        const __m512i permb_const7 = _mm512_set1_epi64(idx7);
        __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
        __m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7));
        __m512i bcd = _mm512_permutexvar_epi8(permb_const7, highbits_l7);
        u64 abcdefgh_BCD = _mm_cvtsi128_si64(_mm512_castsi512_si128(bcd));
#    else
        u64 aabb_ccdd_merge = (xi << 32) + (1 - (10000ull << 32)) * ((xi * (u128)1844674407370956) >> 64);
        __m128i y = _mm_set1_epi64x(aabb_ccdd_merge);
#        if XJB_USE_SSE4_1
        __m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16),
                                  _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1),
                                                  _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#        else
        __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epi16(y, _mm_set1_epi16(0x147b)), 3);
        __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
        __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#        endif
        __m128i z_div_10 = _mm_mulhi_epi16(z, _mm_set1_epi16(0x199a));
        __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
        u64 abcdefgh_BCD = _mm_cvtsi128_si64(tmp);
#    endif
        u64 lz = u64_tz_bits(abcdefgh_BCD) / 8;
        uint64_t abcdefgh_bcd = !is_little_endian() ? byteswap64_xjb(abcdefgh_BCD) : abcdefgh_BCD;
        uint64_t abcdefgh_ascii = abcdefgh_bcd | ZERO;
        abcdefgh_ascii =
            is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz);  // remove leading zeros
        memcpy(buf, &abcdefgh_ascii, 8);
        buf = buf + 8 - lz;
        memcpy(buf, ".0\0", 4);
        buf += 2;
        return buf;
#else
        int64_t abcd_efgh = xi + (0x100000000 - 10000) * ((xi * 0x68db8bb) >> 40);
        int64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
        int64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f);
        u64 lz = u64_lz_bits(a_b_c_d_e_f_g_h) / 8;
        uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64_xjb(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
        uint64_t abcdefgh_ascii = abcdefgh_bcd | ZERO;
        abcdefgh_ascii =
            is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz);  // remove leading zeros
        memcpy(buf, &abcdefgh_ascii, 8);
        buf = buf + 8 - lz;
        memcpy(buf, ".0\0", 4);
        buf += 2;
        return buf;
#endif
    } else {
        // write 9-16 digit
#if XJB_USE_NEON
        // src from :
        // https://gist.github.com/dougallj/b4f600ab30ef79bb6789bc3f86cd597a#file-convert-neon-cpp-L144-L169
        uint32_t abcdefgh = ((__uint128_t)xi * cv->mul_const) >> 90;
        uint32_t ijklmnop = xi + abcdefgh * cv->hundred_million;
        uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
        int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(cv->multipliers32[0])), 9);
        int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(cv->multipliers32[1]));
        int32x4_t extended = vshll_n_u16(tenthousands, 0);
#    if XJB_IS_AARCH64 && (defined(__clang__) || defined(__GNUC__))
        // asm ("" : "+w"(extended));
#    endif
        int32x4_t hundreds = vmlaq_s32(extended, vqdmulhq_s32(extended, vdupq_n_s32(cv->multipliers32[2])),
                                       vdupq_n_s32(cv->multipliers32[3]));
        int16x8_t BCD_big_endian = vmlaq_s16(hundreds, vqdmulhq_s16(hundreds, vdupq_n_s16(cv->multipliers16[0])),
                                             vdupq_n_s16(cv->multipliers16[1]));
        int8x16_t BCD_little_endian = is_little_endian()
                                          ? vrev64q_u8(BCD_big_endian)
                                          : vreinterpretq_u8_u16(BCD_big_endian);  // big_endian to little_endian
                                                                                   // , reverse 8 bytes
        u64 abcdefgh_bcd = vgetq_lane_u64(BCD_little_endian, 0);                   // hi
        u64 ijklmnop_bcd = vgetq_lane_u64(BCD_little_endian, 1);                   // lo
        u64 abcdefgh_lz = u64_lz_bits(vgetq_lane_u64(BCD_big_endian, 0)) / 8;
        u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
        u64 ijklmnop_ascii = ijklmnop_bcd | ZERO;
        abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * abcdefgh_lz) : abcdefgh_ascii << (8 * abcdefgh_lz);
        memcpy(buf, &abcdefgh_ascii, 8);
        buf = buf + 8 - abcdefgh_lz;
        memcpy(buf, &ijklmnop_ascii, 8);
        buf += 8;
#elif XJB_USE_SSE2 && (false)
        uint64_t abcdefgh = ((__uint128_t)xi * cv->mul_const) >> 90;
        uint64_t ijklmnop = xi + abcdefgh * cv->hundred_million;
#    if XJB_USE_AVX512IFMA_VBMI
        const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
        const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
        const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
        const __m512i zmmTen = _mm512_set1_epi64(10);
        const __m512i zero = _mm512_set1_epi64(0);
        const __m512i asciizero = _mm512_set1_epi8('0');
        const __m512i ifma_const =
            _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
                              0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
        const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(
            0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
        __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
        __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
        __m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
        __m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
        __m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
        __m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
#    elif XJB_USE_SSSE3 && (false)
        __m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
        __m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ULL << 32) - 10000),
                                                   _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#        if XJB_USE_SSE4_1
        __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ULL << 16) - 100),
                                                     _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)),
                                                                    3)));  //_mm_mullo_epi32 : sse4.1
#        else
        __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
        __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
        __m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#        endif
        __m128i big_endian_bcd = _mm_add_epi64(
            z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a))));
        __m128i little_endian_bcd =
            _mm_shuffle_epi8(big_endian_bcd, _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7));
#    else  // sse2
        __m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
        __m128i x_div_10000 = _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi32(0xd1b71759)), 45);
        __m128i x_mod_10000 = _mm_sub_epi32(x, _mm_mul_epu32(x_div_10000, _mm_set1_epi32(10000)));
        __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));
#        if XJB_USE_SSE4_1
        __m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16),
                                  _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1),
                                                  _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#        else
        __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
        __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
        __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#        endif
        __m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
        __m128i little_endian_bcd =
            _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
#    endif
        uint64_t abcdefgh_bcd = _mm_extract_epi64(little_endian_bcd, 0);
        uint64_t ijklmnop_bcd = _mm_extract_epi64(little_endian_bcd, 1);
        u64 abcdefgh_lz = u64_tz_bits(abcdefgh_bcd) / 8;
        u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
        u64 ijklmnop_ascii = ijklmnop_bcd | ZERO;
        abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * abcdefgh_lz) : abcdefgh_ascii << (8 * abcdefgh_lz);
        memcpy(buf, &abcdefgh_ascii, 8);
        buf = buf + 8 - abcdefgh_lz;
        memcpy(buf, &ijklmnop_ascii, 8);
        buf += 8;
#else
        uint64_t abcdefgh = xi / 100000000;
        uint64_t ijklmnop = xi + abcdefgh * (-100000000);

        uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
        uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bb) >> 40);

        uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
        uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);

        uint64_t a_b_c_d_e_f_g_h =
            ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f);  //+ 0x3030303030303030;
        uint64_t i_j_k_l_m_n_o_p =
            ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f);  //+ 0x3030303030303030;
        u64 abcdefgh_lz = u64_lz_bits(a_b_c_d_e_f_g_h) / 8;
        uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64_xjb(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
        uint64_t ijklmnop_bcd = is_little_endian() ? byteswap64_xjb(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;
        u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
        u64 ijklmnop_ascii = ijklmnop_bcd | ZERO;
        abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * abcdefgh_lz) : abcdefgh_ascii << (8 * abcdefgh_lz);
        memcpy(buf, &abcdefgh_ascii, 8);
        buf = buf + 8 - abcdefgh_lz;
        memcpy(buf, &ijklmnop_ascii, 8);
        buf += 8;

#endif
        memcpy(buf, ".0\0", 4);
        buf += 2;
        return buf;
    }
}
static inline char* process_special_double(const u64 sig, const u64 exp, i64& q, u64& c, bool& is_exit,
                                           char* const buf) {
    // 0,5e-324,inf,nan
    // if( ((exp + 1) & 2047) <= 1)[[unlikely]]
    {
        if (exp == 0) [[unlikely]] {
            if (sig <= 1) {
                is_exit = true;
                return (char*)memcpy(buf, sig ? "5e-324\0" : "0.0\0\0\0\0", 8) + (sig ? 6 : 3);
            }
            c = sig;
            q = 1 - 1075;  // -1074
        }
        if (exp == 2047) [[unlikely]] {
            is_exit = true;
            return (char*)memcpy(buf, sig ? "nan" : "inf", 4) + 3;
        }
    }
    return buf;
}
static inline char* process_small_int_double(const u64 c, const i64 q, const struct const_value_double* cv,
                                             bool& is_exit, char* const buf) {
#define use_fast_path_for_integer_xjb 0
#if use_fast_path_for_integer_xjb
#    if XJB_IS_REAL_GCC
    u64 nq = -q;
    if (nq <= u64_tz_bits(c)) [[unlikely]]  // use unlikely will generate jmp instruction
#    else
    if (nq <= u64_tz_bits(c))  //[[unlikely]]
#    endif
    {
        is_exit = true;
        return write_1_to_16_digit(c >> nq, buf, cv);  // fast path for integer
    }
#endif
    return buf;
}
static inline i64 compute_k_double(i64 q) {
    // return floor(q*log10(2));
#if defined(__SIZEOF_INT128__) && XJB_IS_AARCH64
    // arm64 : smulh ; x64 : imul
    return ((i64)q * (u128)(78913ull << (64 - 18))) >> 64;
#else
    return ((i64)q * 78913) >> 18;
#endif
}
static inline u64* get_pow10_ptr(const struct double_table_t* t, const i64 k) {
    // return pointer of 10**(-k-1)
    const u64* pow10_ptr = t->pow10_double + 323 * 2 + 2;
    return (u64*)&pow10_ptr[k * 2];
}
namespace xjb {
static inline char* xjb64(double v, char* buf) {
    // require buf size >= 33 byte
    const struct const_value_double* cv = &constants_double;
    const struct double_table_t* t = &double_table;
#if XJB_IS_AARCH64 && (defined(__clang__) || defined(__GNUC__))  // for arm64 processor , fewer instructions
                                                                 // MSVC not support inline asm
    asm("" : "+r"(cv));                                          // read constant values from memory to register
#endif
    u64 vi;
    memcpy(&vi, &v, sizeof(v));
    *buf = '-';
    buf += vi >> 63;
    u64 sig = vi & ((1ULL << 52) - 1);
    u64 exp = (vi << 1) >> 53;
    i64 q = (i64)exp - 1075;
    u64 c = ((1ULL << 52) | sig);
#if XJB_IS_X64
    if (((exp + 1) & 2047) <= 1) [[unlikely]]
#endif
    {
        if (exp == 0) [[unlikely]] {
            if (sig <= 1) {
                const char* str = sig ? "5e-324\0" : "0.0\0\0\0\0";
                return (char*)memcpy(buf, str, 8) + (sig ? 6 : 3);
            }
            c = sig;
            q = 1 - 1075;
        }
        if (exp == 2047) [[unlikely]]
            return (char*)memcpy(buf, sig ? "nan" : "inf", 4) + 3;
    }
    // process_small_int_double(c, q, cv, is_exit_small_int, buf);
    unsigned char h7_precalc = t->h7[exp];
    const int offset = 9;  //  offset in range [3,10] has same result.
    u32 irregular = sig == 0;
    i64 k = compute_k_double((i64)exp - 1075);
    u64* p10 = get_pow10_ptr(t, k);
    u64 cb = c << h7_precalc;
    u64 pow10_hi = p10[0], pow10_lo = p10[1];  // 128bit pow10
    u64 hi64, lo64;
    mul_u128_u64_high128(pow10_hi, pow10_lo, cb, &hi64, &lo64);
    u64 dot_one = (hi64 << (64 - offset)) | (lo64 >> offset);
    u64 half_ulp = (pow10_hi >> ((1 + offset) - h7_precalc)) + ((c + 1) & 1);
    bool up = half_ulp > ~0 - dot_one;
    bool down = half_ulp > dot_one;
    u64 m_up = (hi64 >> offset) + up;  // m + up
    u32 up_down = up + down;
    u32 one = (u128_madd_hi64(dot_one, 10, dot_one == (1ULL << 62) ? 0 : cv->c4));  // round to nearest,even
    if (irregular) [[unlikely]] {
        // irregular case : c is 2**52 , exp range is [1,2046];
        k = (i64)(q * 315653 - 131072) >> 20;
        i64 h = q + ((k * -217707 - 217707) >> 16);
        // u64 pow10_hi = t->pow10_double[293 * 2 - 2 + k * -2];
        u64 pow10_hi = t->pow10_double[323 * 2 + 2 + k * 2];
        u64 half_ulp = pow10_hi >> (-h);
        u64 dot_one = (pow10_hi << (53 + h));
        u64 up = (half_ulp > ~0 - dot_one);
        u64 down = ((half_ulp >> 1) > dot_one);
        m_up = (pow10_hi >> (11 - h)) + up;
        up_down = up + down;
        one = ((dot_one >> (53 + h)) * 5 + (1 << (9 - h))) >> (10 - h);
        if ((((dot_one >> 54) * 5) & ((1 << 9) - 1)) > (((half_ulp >> 55) * 5)))
            one = ((((dot_one >> 54) * 5) >> 9) + 1);
        if (dot_one == (1ULL << 62))  // round to even
            one = 2;
    }
    // if (dot_one == (1ULL << 62)) [[unlikely]]  // round to even
    //     one = 2;                               // 0.25 * 10 = 2.5 -> 2
    u64 D17 = m_up > (u64)cv->c3;     // (m_up >= (u64)1e15);
    u64 mr = D17 ? m_up : m_up * 10;  // remove the first digit zero

    i64 e10 = k + (15 + D17);
    const i64 e10_DN = t->e10_DN;
    const i64 e10_UP = t->e10_UP;
    const u64 interval = e10_UP - e10_DN + 1;

#if XJB_IS_AARCH64 || (XJB_IS_X64 && XJB_USE_AVX512IFMA_VBMI && XJB_NO_MEMMOVE)
    u32 e10_3 = (i32)e10 + (-e10_DN);
    u32 e10_data_ofs = e10_3 < interval ? e10_3 : interval;
#else
    u64 e10_3 = e10 + (-e10_DN);
    u64 e10_data_ofs = e10_3 < interval ? e10_3 : interval;
#endif

#if XJB_NO_MEMMOVE
#    if XJB_USE_NEON
    // In case the compiler fails to find the optimization chance
    vst1_s8((int8_t*)buf, vdup_n_s8('0'));
#    else
    memset(buf, '0', 8);
#    endif
#endif
    shortest_ascii16 s = to_ascii16(buf, XJB_NOT_REMOVE_FIRST_ZERO ? m_up : mr, up_down, D17, cv
#if XJB_NO_MEMMOVE
                                    ,
                                    &(t->e10_variable_data[e10_data_ofs][32 + D17 * 16])
#endif
    );
    u64 first_sig_pos = t->e10_variable_data[e10_data_ofs][17 + 0];
    u64 dot_pos = t->e10_variable_data[e10_data_ofs][17 + 1];
    u64 move_pos = t->e10_variable_data[e10_data_ofs][17 + 2];
#if XJB_NO_MEMMOVE
    u64 one_offset = t->e10_variable_data[e10_data_ofs][17 + 3 + D17];
#endif
    u64 exp_pos = t->e10_variable_data[e10_data_ofs][s.dec_sig_len];
    char* const buf_origin = buf;
    buf += first_sig_pos;

#if XJB_USE_NEON_OR_SSE2
    memcpy(buf, &(s.ascii16), 16);  // write m+up to buffer
#else
    memcpy(buf + 0, &(s.hi), 8);
    memcpy(buf + 8, &(s.lo), 8);
#endif

#if XJB_NO_MEMMOVE && XJB_IS_AARCH64
    // aarch64: 0x30302e30 is not a valid immediate
    one |= 0x30303030;  // "0000"
    // first_sig_pos max = 5, one_offset max = 17, requires at least 21 bytes
    memcpy(buf + 16, &s.trailing_byte, 4);
    memcpy(buf + one_offset, &one, 4);
    buf_origin[dot_pos] = '.';
#elif XJB_NO_MEMMOVE
    one |= 0x30302e30;  // "0.00"
    // first_sig_pos max = 5, one_offset max = 17, requires at least 21 bytes
    memcpy(buf + 16, &s.trailing_byte, 4);  // [16, 20)
    buf_origin[dot_pos] = '.';
    memcpy(buf + one_offset, &one, 4);  // [17, 21)
#else
    one |= 0x30303030;                // "0000"
    memcpy(&buf[15 + D17], &one, 4);  // write one to buffer
    memmove(&buf[move_pos], &buf[dot_pos], 16);
    // dot_pos+first_sig_pos+sign max = 16+1 = 17; require 17+16=33 bytes
    buf_origin[dot_pos] = '.';
#endif

#if XJB_IS_AARCH64
    if (exp == 0) [[unlikely]]
#endif
    {
        // some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
        // if (buf[0] == '0')
        if (m_up < (u64)1e14) [[unlikely]] {
            u64 lz = 0;
            while (buf[2 + lz] == '0')
                lz++;
            lz += 2;
            e10 -= lz - 1;
            buf[0] = buf[lz];
            memmove(&buf[2], &buf[lz + 1], 16);
            exp_pos = exp_pos - lz + (exp_pos - lz != 1);
        }
    }
    u64 exp_result = t->exp_result_double[e10 + 324];
    buf += exp_pos;
    memcpy(buf, &exp_result, 8);
    u64 exp_len = exp_result >> 56;
    return buf + exp_len;
}

static inline char* xjb32(float v, char* buf) {
    // require buf size >= 17 byte
    const struct float_table_t* t = &float_table;
    const struct const_value_float* c = &t->constants_float;
#if XJB_IS_AARCH64 && (defined(__clang__) || defined(__GNUC__))  // for arm64 processor , fewer instructions ,
                                                                 // MSVC not support inline asm
    asm("" : "+r"(c));                                           // read constant values from memory to register
#endif
    u32 vi;
    memcpy(&vi, &v, 4);
    buf[0] = '-';
    buf += vi >> 31;
    u32 sig = vi & ((1 << 23) - 1);
    u64 exp = (vi << 1) >> 24;
    u64 sig_bin = sig | (1 << 23);
    i64 exp_bin = (i64)exp - 150;
    if (exp == 0) [[unlikely]] {
        if (sig == 0)
            return (char*)memcpy(buf, "0.0", 4) + 3;
        exp_bin = 1 - 150;
        sig_bin = sig;
    }
    if (exp == 255) [[unlikely]]
        return (char*)memcpy(buf, sig ? "nan" : "inf", 4) + 3;
    u32 h37_precalc = t->h37[exp];
    bool irregular = sig == 0;
    const int BIT = 36;
#if defined(__SIZEOF_INT128__) && XJB_IS_AARCH64           // for arm64 processor , fewer instructions
    i64 k = ((i64)exp_bin * (u128)(1233ull << 52)) >> 64;  // signed multiplication
#else
    i64 k = (exp_bin * 1233) >> 12;
    // exp_bin range : [-149,104] ; k range : [-45,31]
#endif
    if (irregular) [[unlikely]] {
        k = (i64)(exp_bin * 1233 - 512) >> 12;
        h37_precalc = (BIT + 1) + exp_bin + ((k * -1701 + (-1701)) >> 9);  // 37+h
    }
    u64 pow10_hi = t->pow10_float_reverse[45 + k];
    u64 cb = sig_bin << h37_precalc;
    u64 hi64 = umul128_hi64_xjb(cb, pow10_hi);
    u64 half_ulp = (pow10_hi >> (65 - h37_precalc)) + ((sig + 1) & 1);
    u64 dot_one_36bit = hi64 & (((u64)1 << BIT) - 1);
    u32 m_up = (hi64 + half_ulp) >> BIT;
    u32 up_down = m_up > (u32)((hi64 - (half_ulp >> 0)) >> BIT);
#if XJB_IS_AARCH64
    u32 one =
        (dot_one_36bit * 10 + c->c1 + (dot_one_36bit >> (BIT - 4))) >> (BIT);  // for arm64 : madd instruction faster.
#else
    u32 one = (dot_one_36bit * 5 + c->c1 + (dot_one_36bit >> (BIT - 4))) >> (BIT - 1);  // for x64.
#endif
    if (irregular) [[unlikely]] {
        if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150))  // branch instruction
            ++one;
        up_down = m_up > ((hi64 - (half_ulp >> 1)) >> BIT);
    }
    u32 lz = ((u32)m_up < (u32)c->e7) + ((u32)m_up < (u32)c->e6);
    memset(buf, '0', 16);
    shortest_ascii8 s = to_ascii8(m_up, up_down, lz, c);
    i32 e10 = (i32)k + (8 - lz);
    const i64 e10_DN = t->e10_DN, e10_UP = t->e10_UP;
    const u64 interval = e10_UP - e10_DN + 1;  // 6 + 3 + 1 = 10
    u32 e10_3 = e10 + (-e10_DN);
    u32 e10_data_ofs = e10_3 < interval ? e10_3 : interval;
    u64 first_sig_pos = t->e10_variable_data[e10_data_ofs][9 + 0];
    u64 dot_pos = t->e10_variable_data[e10_data_ofs][9 + 1];
    u64 move_pos = t->e10_variable_data[e10_data_ofs][9 + 2];
    u64 exp_pos = t->e10_variable_data[e10_data_ofs][s.dec_sig_len];
    char* buf_origin = (char*)buf;
    buf += first_sig_pos;
    memcpy(buf, &(s.ascii), 8);
    memcpy(&buf[8 - lz], &one, 4);
    memmove(&buf[move_pos], &buf[dot_pos],
            8);  // the index (first_sig_pos + dot_pos + sign) max = 7+1=8,
    buf_origin[dot_pos] = '.';
#if XJB_IS_AARCH64  // for arm64 processor , fewer instructions
    if (exp == 0) [[unlikely]]
#endif
        if (m_up < 100000) [[unlikely]] {
            u64 lz = 0;
            u64 u;
            memcpy(&u, &buf[2], 8);
            u = is_little_endian() ? u : byteswap64_xjb(u);
            lz = u64_tz_bits(u & 0x0f0f0f0f0f0f0f0f) / 8;
            lz += 2;
            e10 -= lz - 1;
            buf[0] = buf[lz];
            memmove(&buf[2], &buf[lz + 1], 8);
            exp_pos = exp_pos - lz + (exp_pos - lz != 1);
        }
    u32 exp_result_u32 = t->exp_result_float[45 + e10];
    if (!is_little_endian())
        exp_result_u32 = ((exp_result_u32 & 0xff000000) >> 24) | ((exp_result_u32 & 0x00ff0000) >> 8) |
                         ((exp_result_u32 & 0x0000ff00) << 8) | ((exp_result_u32 & 0x000000ff) << 24);
    u64 exp_result_u64 = is_little_endian() ? exp_result_u32 : (u64)exp_result_u32 << 32;
    buf += exp_pos;
    memcpy(buf, &exp_result_u64, 8);
    return buf + (exp_result_u64 & 4);  // 'e' is 0b01100101
}
}  // end of namespace xjb

/*==============================================================================
 * Export
 *============================================================================*/

char* xjb_ftoa(float v, char* buf) {
    return xjb::xjb32(v, buf);
}
char* xjb_ftoa(double v, char* buf) {
    return xjb::xjb64(v, buf);
}

// author : xjb
// src : github.com/xjb714/xjb
// date : 2026.2.2

// todo : satisfy https://tc39.es/ecma262/#sec-numeric-types-number-tostring
// when input double in (1e-7,1e21), use %lf to format, otherwise use %le.
// example : 123.456 -> "123.456", 0.000000123456 -> "1.23456e-7", 1e22 -> "1e+22"

// todo : big-endian support, msvc support, optimize for performance, add comments, reduce code size, etc.

//#pragma once

#include <stdint.h>
#include <string.h>

#define USE_NEON_SSE2 1





#if USE_NEON_SSE2

#if defined(__aarch64__) && (defined(__ARM_NEON__) || defined(__ARM_NEON))
#include <arm_neon.h>
#ifndef HAS_NEON_OR_SSE2
#define HAS_NEON_OR_SSE2 1
#endif
#ifndef HAS_NEON
#define HAS_NEON 1
#endif
#endif

#if defined(__GNUC__) && defined(__SSE2__) && defined(__amd64__)
#include <immintrin.h>
#ifndef HAS_NEON_OR_SSE2
#define HAS_NEON_OR_SSE2 1
#endif
#ifndef HAS_SSE2
#define HAS_SSE2 1
#endif
#endif

#endif // endif USE_NEON_SSE2

#ifdef __aarch64__
#  define NOT_REMOVE_FIRST_ZERO 1
#else
#  define NOT_REMOVE_FIRST_ZERO 0
#endif

#ifndef is_real_gcc
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
	!defined(__clang__) && !defined(__llvm__) &&    \
	!defined(__INTEL_COMPILER) && !defined(__ICC)
#define is_real_gcc 1
#endif
#endif

#ifdef _MSC_VER
#include <intrin.h> // __lzcnt64/_umul128/__umulh
#endif

#if defined(__SIZEOF_INT128__)
typedef __uint128_t u128; // msvc not support
#endif

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;

static inline constexpr uint64_t umul128_hi64_fallback(uint64_t x, uint64_t y)
{
	// constexpr function for all compiler and hardware. just for const value precalc , not a Runtime function.
	uint64_t a = x >> 32;
	uint64_t b = uint32_t(x);
	uint64_t c = y >> 32;
	uint64_t d = uint32_t(y);

	uint64_t ac = a * c;
	uint64_t bc = b * c;
	uint64_t ad = a * d;
	uint64_t bd = b * d;

	uint64_t cs = (bd >> 32) + uint32_t(ad) + uint32_t(bc); // cross sum
	return ac + (ad >> 32) + (bc >> 32) + (cs >> 32);
}
static inline constexpr void umul128_hi64_lo64_fallback(uint64_t x, uint64_t y, uint64_t &hi, uint64_t &lo)
{
	// constexpr function for all compiler and hardware. just for const value precalc , not a Runtime function.
	uint64_t a = x >> 32;
	uint64_t b = uint32_t(x);
	uint64_t c = y >> 32;
	uint64_t d = uint32_t(y);

	uint64_t ac = a * c;
	uint64_t bc = b * c;
	uint64_t ad = a * d;
	uint64_t bd = b * d;

	uint64_t cs = (bd >> 32) + uint32_t(ad) + uint32_t(bc); // cross sum
	hi = ac + (ad >> 32) + (bc >> 32) + (cs >> 32);
	lo = (cs << 32) + uint32_t(bd);
}
// static inline uint64_t umul128_hi64_lo64(uint64_t x,uint64_t y,uint64_t)
//{
//
// }
static inline void mul_u128_u64_high128(
	uint64_t a_high,
	uint64_t a_low,
	uint64_t b,
	uint64_t *result_high,
	uint64_t *result_mid)
{
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

static inline int u64_lz_bits(uint64_t x)
{
#if defined(__has_builtin) && __has_builtin(__builtin_clzll)
	return __builtin_clzll(x);
#elif defined(_MSC_VER) && defined(__AVX2__)
	// Use lzcnt only on AVX2-capable CPUs that have this BMI instruction.
	return __lzcnt64(x);
#elif defined(_MSC_VER)
	unsigned long idx;
	_BitScanReverse64(&idx, x); // Fallback to the BSR instruction.
	return 63 - idx;
#else
	int n = 64;
	for (; x > 0; x >>= 1)
		--n;
	return n;
#endif
}
static inline int u64_tz_bits(uint64_t x)
{
#if defined(__has_builtin) && __has_builtin(__builtin_ctzll)
	return __builtin_ctzll(x);
#elif defined(_MSC_VER) && defined(__AVX2__)
	// Use lzcnt only on AVX2-capable CPUs that have this BMI instruction.
	return __tzcnt64(x);
#elif defined(_MSC_VER)
	unsigned long idx;
	if (_BitScanForward64(&idx, x))
	{
		return (int)idx;
	}
	else
	{
		return 64;
	}
#else
	int n = 64;
	for (; x > 0; x <<= 1)
		--n;
	return n;
#endif
}
static inline uint64_t umul128_hi64_xjb(uint64_t a, uint64_t b)
{
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

static inline uint64_t u128_madd_hi64(uint64_t a, uint64_t b, uint64_t c)
{
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
static inline const uint64_t is_little_endian()
{
	const int n = 1;
	return *(const char *)(&n) == 1;
}
typedef struct
{
#if HAS_NEON
	uint64x2_t ascii16;
#elif HAS_SSE2
	__m128i ascii16;
#else
	uint64_t hi;
	uint64_t lo;
#endif
	uint64_t dec_sig_len; // range : [1,17] - 1 = [0,16]
} shortest_ascii16;

typedef struct
{
	uint64_t ascii;
	uint64_t dec_sig_len; // range : [1,9] - 1 = [0,8]
} shortest_ascii8;

/* const value table for double to string : begin */
// todo : use constexpr to reduce code size
struct const_value_double
{
	uint64_t c1;
	uint64_t c2;
	uint64_t c3;
	uint64_t c4;
	uint64_t c5;
	uint64_t c6;
	uint64_t mul_const;
	int64_t hundred_million;
	uint64_t div10000;
	uint64_t div10000_m;
	double div10000_2_d;
	int64_t div10000_2;
#if HAS_NEON
	int32x4_t multipliers32; // 16
	int16x8_t multipliers16; // 16
#else
	int32_t multipliers32[4]; // 16
	int16_t multipliers16[8]; // 16
#endif
};

struct const_value_float
{
	int64_t c1;
	uint64_t div10000;
	uint32_t e7;
	uint32_t e6;
	uint32_t e5;
	uint32_t m;
#if HAS_NEON
	int32x4_t m32_4;
#else
	int32_t m32_4[4];
#endif
};

static const struct const_value_double constants_double = {
	.c1 = 78913ull << (64 - 18),
	.c2 = (uint64_t)-217707,
	.c3 = (uint64_t)1e15 - 1,
	//.c4 = 0x7ffffffffffffff9ull,
	.c4 = (1ull << 63) + 6,
	.c5 = (uint64_t)-131072,
	.c6 = (1 << 9) - 1,
	.mul_const = 0xabcc77118461cefd,
	.hundred_million = (int64_t)-100000000,
	.div10000 = 1844674407370956,
	.div10000_m = 0x100000000 - 10000,
	.div10000_2_d = (double)(-10000 + 0x100000000),
	.div10000_2 = 0xd1b7176000,
	.multipliers32 = {0x68db8bb, -10000 + 0x10000, 0x147b000, -100 + 0x10000},
	.multipliers16 = {0xce0, -10 + 0x100, '0' + '0' * 256},
};
static const struct const_value_float constants_float = {
	.c1 = (((u64)('0' + '0' * 256) << (36 - 1)) + (((u64)1 << (36 - 2)) - 7)),
	.div10000 = 1844674407370956,
	.e7 = 10000000,
	.e6 = 1000000,
	.e5 = 100000,
	.m = (1ull << 32) - 10000,
	.m32_4 = {0x147b000, -100 + 0x10000, 0xce0, -10 + 0x100},
};

struct double_table_t
{
	static const int e10_DN = -3;
	static const int e10_UP = 15;
	static const int max_dec_sig_len = 17;
	static const int num_pow10 = 323 - (-293) + 1;
	uint64_t pow10_double[(323 - (-293) + 1) * 2] = {};
	uint64_t exp_result_double[324 + 308 + 1] = {};
	unsigned char e10_variable_data[e10_UP - (e10_DN) + 1 + 1][max_dec_sig_len + 3] = {};
	unsigned char h7[2048]={};
	constexpr double_table_t()
	{
		struct uint192
		{
			uint64_t w0, w1, w2;
		};
		uint192 current = {0xb2e28cedd086d011, 0x1e53ed49a96272c8, 0xcc5fc196fefd7d0c}; // e10 = -293
		constexpr uint64_t ten = 0xa000000000000000;
		for (int i = 0; i < num_pow10; ++i)
		{
			int e10 = i - 293;
			pow10_double[i * 2 + 0] = e10 == 0 ? 1ull << 63 : current.w2 + (e10 >= 0 && e10 <= 27);
			pow10_double[i * 2 + 1] = current.w1 + 1;
			uint64_t h0 = umul128_hi64_fallback(current.w0, ten);
			uint64_t h1 = umul128_hi64_fallback(current.w1, ten);
			uint64_t c0 = h0 + current.w1 * ten;
			uint64_t c1 = (c0 < h0) + h1 + current.w2 * ten;
			uint64_t c2 = (c1 < h1) + umul128_hi64_fallback(current.w2, ten);
			if (c2 >> 63)
				current = {c0, c1, c2};
			else
				current = {c0 << 1, c1 << 1 | c0 >> 63, c2 << 1 | c1 >> 63}; // left shift 1 bit
		}
		for (int e10 = -324; e10 <= 308; ++e10)
		{
			u64 e = e10 < 0 ? ('e' + '-' * 256) : 'e' + '+' * 256;
			u64 e10_abs = e10 < 0 ? -e10 : e10;
			u64 a = e10_abs / 100;
			u64 bc = e10_abs - a * 100;
			u64 b = bc / 10;
			u64 c = bc - b * 10;
			u64 exp_len = 4 + (e10_abs >= 100);
			u64 e10_abs_ascii = (e10_abs >= 100) ? (a + '0') + ((b + '0') << 8) + ((c + '0') << 16) : (b + '0') + ((c + '0') << 8);
			u64 exp_res = e + (e10_abs_ascii << 16) + (exp_len << 56);
			exp_res = (e10 >= e10_DN && e10 <= e10_UP) ? 0 : exp_res;
			exp_result_double[e10 + 324] = exp_res;
		}
		for (int e10 = e10_DN; e10 <= e10_UP + 1; e10++)
		{
			int tmp_data_ofs = e10 - e10_DN;
			u64 first_sig_pos = (e10_DN <= e10 && e10 <= -1) ? 1 - e10 : 0;
			u64 dot_pos = (0 <= e10 && e10 <= e10_UP) ? 1 + e10 : 1;
			u64 move_pos = dot_pos + ((0 <= e10 || e10 < e10_DN));
			e10_variable_data[tmp_data_ofs][max_dec_sig_len + 0] = first_sig_pos;
			e10_variable_data[tmp_data_ofs][max_dec_sig_len + 1] = dot_pos;
			e10_variable_data[tmp_data_ofs][max_dec_sig_len + 2] = move_pos;
			for (int dec_sig_len = 1; dec_sig_len <= max_dec_sig_len; dec_sig_len++)
			{
				u64 exp_pos = (e10_DN <= e10 && e10 <= -1) ? dec_sig_len : (0 <= e10 && e10 <= e10_UP ? (e10 + 3 > dec_sig_len + 1 ? e10 + 3 : dec_sig_len + 1) : (dec_sig_len + 1 - (dec_sig_len == 1)));
				e10_variable_data[tmp_data_ofs][dec_sig_len - 1] = exp_pos;
			}
		}
		for(int exp=0;exp<2048;++exp)
		{
			const int offset = 6;
			int q = exp - 1075 + (exp == 0);
			int k = (q * 78913) >> 18;
			int h = q + (((-k-1) * 217707) >> 16);
			h7[exp] = (unsigned char)(h + 1 + offset);
		}
	}
};

struct float_table_t
{
	static const int e10_DN = -3;
	static const int e10_UP = 6;
	static const int max_dec_sig_len = 9;
	const int num_pow10 = 44 - (-32) + 1;
	uint64_t pow10_float_reverse[44 - (-32) + 1] = {};
	uint32_t exp_result_float[45 + 38 + 1] = {};
	unsigned char e10_variable_data[e10_UP - (e10_DN) + 1 + 1][max_dec_sig_len + 3] = {};
	unsigned char h37[256] = {};
	constexpr float_table_t()
	{
		struct uint128_xjb
		{
			uint64_t w0, w1;
		};
		uint128_xjb current = {0x67de18eda5814af3, 0xcfb11ead453994ba}; // e10 = -32
		uint64_t ten = 0xa000000000000000;
		for (int i = 0; i < num_pow10; ++i)
		{
			int e10 = i - 32;
			pow10_float_reverse[num_pow10 - i - 1] = e10 == 0 ? 1ull << 63 : current.w1 + 1;
			uint64_t h0 = umul128_hi64_fallback(current.w0, ten);
			uint64_t c0 = h0 + current.w1 * ten;
			uint64_t c1 = (c0 < h0) + umul128_hi64_fallback(current.w1, ten);
			if (c1 >> 63)
				current = {c0, c1};
			else
				current = {c0 << 1, c1 << 1 | c0 >> 63}; // left shift 1 bit
		}
		for (int e10 = -45; e10 <= 38; e10++)
		{
			u64 e = e10 < 0 ? ('e' + '-' * 256) : ('e' + '+' * 256);
			u64 e10_abs = e10 < 0 ? -e10 : e10;
			u64 a = e10_abs / 10;
			u64 b = e10_abs - a * 10;
			u64 e10_abs_ascii = (a + '0') + ((b + '0') << 8);
			u64 exp_res = e + (e10_abs_ascii << 16);
			exp_res = (e10 >= e10_DN && e10 <= e10_UP) ? 0 : exp_res;
			exp_result_float[e10 + 45] = (uint32_t)exp_res;
		}
		for (int e10 = e10_DN; e10 <= e10_UP + 1; e10++)
		{
			int tmp_data_ofs = e10 - e10_DN;
			u64 first_sig_pos = (e10_DN <= e10 && e10 <= -1) ? 1 - e10 : 0;
			u64 dot_pos = (0 <= e10 && e10 <= e10_UP) ? 1 + e10 : 1;
			u64 move_pos = dot_pos + ((0 <= e10 || e10 < e10_DN));
			e10_variable_data[tmp_data_ofs][max_dec_sig_len + 0] = first_sig_pos;
			e10_variable_data[tmp_data_ofs][max_dec_sig_len + 1] = dot_pos;
			e10_variable_data[tmp_data_ofs][max_dec_sig_len + 2] = move_pos;
			for (int dec_sig_len = 1; dec_sig_len <= max_dec_sig_len; dec_sig_len++)
			{
				u64 exp_pos = (e10_DN <= e10 && e10 <= -1) ? dec_sig_len : (0 <= e10 && e10 <= e10_UP ? (e10 + 3 > dec_sig_len + 1 ? e10 + 3 : dec_sig_len + 1) : (dec_sig_len + 1 - (dec_sig_len == 1)));
				e10_variable_data[tmp_data_ofs][dec_sig_len - 1] = exp_pos;
			}
		}
		for (int exp = 0; exp < 256; exp++)
		{
			int exp_bin = exp - 150 + (exp == 0);
			int k = (exp_bin * 1233) >> 12;
			int h37_precalc = (36 + 1) + exp_bin + ((k * -1701 + (-1701)) >> 9);
			h37[exp] = (unsigned char)h37_precalc;
		}
	}
};
alignas(64) constexpr double_table_t double_table;
alignas(64) constexpr float_table_t float_table;
/* const value table for double to string : end */

static inline uint64_t byteswap64(uint64_t x)
{
#if defined(__has_builtin) && __has_builtin(__builtin_bswap64)
	return __builtin_bswap64(x);
#elif defined(_MSC_VER)
	return _byteswap_uint64(x);
#else
	return ((x & 0xff00000000000000) >> 56) | ((x & 0x00ff000000000000) >> 40) |
		   ((x & 0x0000ff0000000000) >> 24) | ((x & 0x000000ff00000000) >> 8) |
		   ((x & 0x00000000ff000000) << 8) | ((x & 0x0000000000ff0000) << 24) |
		   ((x & 0x000000000000ff00) << 40) | ((x & 0x00000000000000ff) << 56);
#endif
}

static inline uint64_t cmov_branchless(uint64_t condition, uint64_t true_value, uint64_t false_value)
{
	//     // if up_down == 1 return true_value
	//     // if up_down == 0 return false_value
	// #if is_real_gcc
	//     // prevent the gcc compiler generating branch instructions
	//     return ((~(up_down - 1)) & true_value) | ((up_down - 1) & false_value); // only up_down = 1 or 0 can correctly execute.
	// #else
	//     return up_down ? true_value : b;
	// #endif

#if !is_real_gcc || !defined(__amd64__)
	return condition ? true_value : false_value;
#else
	// only amd64 , gcc compiler use this branchless cmov method
	asm volatile("test %2, %2\n\t"
				 "cmovne %1, %0\n\t" : "+r"(false_value) : "r"(true_value),
														   "r"(condition) : "cc");
	return false_value;
#endif
}

static inline uint64_t compute_double_dec_sig_len(uint64_t up_down, int tz, uint64_t D17)
{
	//return (15 + D17) + cmov_branchless(up_down, -1 - tz, up_down);
	return cmov_branchless(up_down, (NOT_REMOVE_FIRST_ZERO ? 14 + D17 :  15) - (tz), 15 + D17);
	//return cmov_branchless(up_down, 15 - tz, 15 + D17);
}
static inline uint64_t compute_double_dec_sig_len_sse2(uint64_t up_down, int tz_add_48, uint64_t D17)
{
	return cmov_branchless(up_down, (NOT_REMOVE_FIRST_ZERO ? 14 + D17 :  15) + 48 - tz_add_48, 15 + D17);
}
static inline uint64_t compute_float_dec_sig_len(uint64_t up_down, int tz, uint64_t lz)
{
	return cmov_branchless(up_down, 7 - tz, 8 - lz);
}

static inline shortest_ascii16 to_ascii16(char* buf, const uint64_t m, const uint64_t up_down, const uint64_t D17, const struct const_value_double *cv)
{
	// m range : [1, 1e16 - 1] ; m = abcdefgh * 10^8 + ijklmnop
	const uint64_t ZERO = 0x3030303030303030ull;
	// uint32_t abcdefgh = ((__uint128_t)m * cv->mul_const) >> 90;
	uint32_t abcdefgh = umul128_hi64_xjb(m, cv->mul_const) >> (90 - 64);
	int64_t hundred_million = cv->hundred_million;
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__))
	asm("" : "+r"(hundred_million));
#endif
	uint32_t ijklmnop = m + abcdefgh * hundred_million;
#if HAS_NEON
	//  src from : https://gist.github.com/dougallj/b4f600ab30ef79bb6789bc3f86cd597a#file-convert-neon-cpp-L144-L169
	//  bolg : https://dougallj.wordpress.com/2022/04/01/converting-integers-to-fixed-width-strings-faster-with-neon-simd-on-the-apple-m1/
	//  author : https://github.com/dougallj

	uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
	int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(cv->multipliers32[0])), 9);
	int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(cv->multipliers32[1]));
	int32x4_t extended = vshll_n_u16(tenthousands, 0);
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__))
	asm("" : "+w"(extended));
#endif
	int32x4_t high_100 = vqdmulhq_s32(extended, vdupq_n_s32(cv->multipliers32[2]));
	int32x4_t hundreds = vmlaq_s32(extended, high_100, vdupq_n_s32(cv->multipliers32[3]));
	int16x8_t high_10 = vqdmulhq_s16(hundreds, vdupq_n_s16(cv->multipliers16[0]));
	int16x8_t BCD_big_endian = vmlaq_s16(hundreds, high_10, vdupq_n_s16(cv->multipliers16[1]));
	int8x16_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
	int16x8_t ascii16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
	vst1q_s8((int8_t *)buf, vdupq_n_s8('0'));
	uint16x8_t is_not_zero = vcgtzq_s8(BCD_little_endian);
	uint64_t zeroes = vget_lane_u64(vreinterpret_u64_u8(vshrn_n_u16(is_not_zero, 4)), 0);// zeros != 0
	int tz = u64_lz_bits(zeroes) >> 2;
	return {ascii16 , cmov_branchless(up_down, NOT_REMOVE_FIRST_ZERO ? (14 + D17) - (tz) : 15 - tz , 15 + D17)};
	//return {ascii16 , cmov_branchless(up_down, NOT_REMOVE_FIRST_ZERO ? (-1 ) - (tz) : 15 - tz , 0 ) + (15 + D17) };
	//return {ascii16 , (15 + D17) - (up_down ? (tz+1) : up_down)};
	//return {ascii16, compute_double_dec_sig_len(up_down, tz, D17)};


	//uint64_t abcd_efgh = abcdefgh + ((1ull << 32) - 10000) * ((abcdefgh * (__uint128_t)1844674407370956) >> 64); // (abcd << 32) + efgh
	//uint64_t ijkl_mnop = ijklmnop + ((1ull << 32) - 10000) * ((ijklmnop * (__uint128_t)1844674407370956) >> 64); // (ijkl << 32) + mnop
	// uint64_t abcd_efgh = abcdefgh + cv->div10000_m * ((abcdefgh * (__uint128_t)cv->div10000) >> 64); // (abcd << 32) + efgh
	// uint64_t ijkl_mnop = ijklmnop + cv->div10000_m * ((ijklmnop * (__uint128_t)cv->div10000) >> 64); // (ijkl << 32) + mnop
	// uint64x2_t merge4 = vcombine_u64(vcreate_u64(abcd_efgh), vcreate_u64(ijkl_mnop));
	// //uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
	// uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(cv->multipliers32[2])), cv->multipliers32[3]);
	// //uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
	// uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(cv->multipliers16[0])), cv->multipliers16[1]);
	// uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
	// int16x8_t ascii16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
	// vst1q_s8((int8_t *)buf, vdupq_n_s8('0'));
	// uint16x8_t is_not_zero = vcgtzq_s8(BCD_little_endian);
	// uint64_t zeroes = vget_lane_u64(vreinterpret_u64_u8(vshrn_n_u16(is_not_zero, 4)), 0);// zeros != 0
	// int tz = u64_lz_bits(zeroes) >> 2;
	// return {ascii16 , cmov_branchless(up_down, NOT_REMOVE_FIRST_ZERO ? (14 + D17) - (tz) : 15 - tz , 15 + D17)};

#endif

#if HAS_SSE2
	// method 1 : AVX512IFMA, AVX512VBMI
	// method 2 : SSE4.1
	// method 3 : SSSE3 (maybe deleted , not enough performance than SSE2)
	// method 4 : SSE2

	// method 1 : AVX512IFMA, AVX512VBMI
#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
	const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
	const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
	const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
	const __m512i zmmTen = _mm512_set1_epi64(10);
	const __m512i zero = _mm512_set1_epi64(0);
	const __m512i asciizero = _mm512_set1_epi8('0');
	const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
												 0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
	const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
	// const __m512i permb_const7 = _mm512_castsi128_si512(_mm_set_epi8(0x7f, 0x77, 0x6f, 0x67, 0x5f, 0x57, 0x4f, 0x47, 0x3f, 0x37, 0x2f, 0x27, 0x1f, 0x17, 0x0f, 0x07));
	__m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
	__m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
	__m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
	__m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
	//__m512i highbits_h7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_h, 5), _mm512_slli_epi64(lowbits_h, 7));
	//__m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7));
	//__m512i bcd = _mm512_permutex2var_epi8(highbits_h7, permb_const7, highbits_l7);//another way to permute , but slower.
	__m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
	__m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
	//__m128i little_endian_ascii = _mm512_castsi512_si128(ascii);
	__m128i little_endian_ascii = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
	_mm_storeu_si128((__m128i*)buf, _mm_set1_epi8('0'));
	int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm512_castsi512_si128(zero)));
	int tz = u64_lz_bits(mask);
	return {little_endian_ascii, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#elif defined(__SSSE3__) && (false)
	__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
	__m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#ifdef __SSE4_1__
	__m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3))); //_mm_mullo_epi32 : sse4.1
#else
	__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
	__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	__m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#endif
	__m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a))));
	__m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)); // ssse3
	int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
	int tz = u64_lz_bits(mask);
	__m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
	return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#else // sse2

	//     __m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
	//     __m128i x_div_10000 = _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi32(0xd1b71759)), 45);
	//     __m128i x_mod_10000 = _mm_sub_epi32(x, _mm_mul_epu32(x_div_10000, _mm_set1_epi32(10000)));
	//     __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));
	// #if defined(__SSE4_1__)
	//     __m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
	// #else
	//     __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
	//     __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	//     __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
	// #endif
	//     __m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
	//     __m128i little_endian_bcd = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));

	__m128i x = _mm_unpacklo_epi64(_mm_cvtsi32_si128(abcdefgh), _mm_cvtsi32_si128(ijklmnop));
	//__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
	__m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#if defined(__SSE4_1__)
	__m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
	__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
	__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	__m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
	__m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
	__m128i bcd_swapped = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
	__m128i little_endian_bcd = _mm_shuffle_epi32(bcd_swapped, _MM_SHUFFLE(2, 3, 0, 1));

	int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
	int tz = u64_lz_bits(mask);
	__m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
	_mm_storeu_si128((__m128i*)buf, _mm_set1_epi8('0'));
	return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#endif

#endif // endif HAS_SSE2

#if !HAS_NEON_OR_SSE2
	uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bbull) >> 40);
	uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bbull) >> 40);
	uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
	uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);
	uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f);
	uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f);
	int abcdefgh_tz = u64_tz_bits(a_b_c_d_e_f_g_h);
	int ijklmnop_tz = u64_tz_bits(i_j_k_l_m_n_o_p);
	uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
	uint64_t ijklmnop_bcd = is_little_endian() ? byteswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;
	int tz = (ijklmnop == 0) ? 64 + abcdefgh_tz : ijklmnop_tz;
	tz = tz / 8;
	memcpy(buf, &ZERO, 8);
	return {abcdefgh_bcd | ZERO, ijklmnop_bcd | ZERO, compute_double_dec_sig_len(up_down, tz, D17)};
#endif
}

static inline shortest_ascii8 to_ascii8(const uint64_t m, const uint64_t up_down, uint64_t lz, const struct const_value_float *c = nullptr)
{
	// m range : [0, 1e8 - 1] ; m = abcdefgh
	const uint64_t ZERO = 0x3030303030303030;
#if HAS_NEON
	// u64 abcd_efgh = m + c->m * ((m * (u128)c->div10000) >> 64);
	int32x2_t tenthousands = vcreate_u64(m + c->m * ((m * (u128)c->div10000) >> 64));
	// int32x2_t tenthousands = vcreate_u64(m + ((1ull << 32) - 10000) * ((m * (u128)c->div10000) >> 64));
	int32x2_t hundreds = vmla_n_s32(tenthousands, vqdmulh_s32(tenthousands, vdup_n_s32(c->m32_4[0])), c->m32_4[1]);
	//int16x4_t BCD_big_endian = vmla_n_s16(hundreds, vqdmulh_s16(hundreds, vdup_n_s16(0xce0)), -10 + 0x100);
	int16x4_t BCD_big_endian = vmla_n_s16(hundreds, vqdmulh_s16(hundreds, vdup_n_s16(c->m32_4[2])), c->m32_4[3]);//fewer instructions but slower,why?
	u64 abcdefgh_BCD = byteswap64(vget_lane_u64(BCD_big_endian, 0)); // big_endian to little_endian , reverse 8 bytes
#endif

#if HAS_SSE2

#if defined(__AVX512F__) && defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
	__m512i bcstq_l = _mm512_set1_epi64(m);
	const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
	const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b,
												 0x0000000010c6f7a0c, 0x00000000a7c5ac472, 0x000000068db8bac72, 0x0000004189374bc6b,
												 0x0000028f5c28f5c29, 0x0000199999999999a);
	// const u64 idx = 0 + (8 << 8) + (16 << 16) + (24 << 24) + (32ull << 32) + (40ull << 40) + (48ull << 48) + (56ull << 56);
	// const __m512i permb_const = _mm512_castsi128_si512(_mm_cvtsi64_si128(idx));
	// __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
	// __m512i highbits_l = _mm512_madd52hi_epu64(_mm512_set1_epi64(0), lowbits_l, _mm512_set1_epi64(10));
	// //__m512i highbits_l = _mm512_srli_epi64(_mm512_add_epi64(lowbits_l, _mm512_slli_epi64(lowbits_l, 2)), 51);
	// __m512i bcd = _mm512_permutexvar_epi8(permb_const, highbits_l);

	const u64 idx7 = 7 + (15 << 8) + (23 << 16) + (31 << 24) + (39ull << 32) + (47ull << 40) + (55ull << 48) + (63ull << 56);
	const __m512i permb_const7 = _mm512_set1_epi64(idx7);
	__m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
	__m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7)); // old CPU
	//__m512i highbits_l7 = _mm512_mullo_epi64(lowbits_l , _mm512_set1_epi64(10<<4));//new CPU may faster
	__m512i bcd = _mm512_permutexvar_epi8(permb_const7, highbits_l7);

	u64 abcdefgh_BCD = _mm_cvtsi128_si64(_mm512_castsi512_si128(bcd));
	// #elif defined(__SSSE3__) //&& (false) // endif avx512ifma avx512vbmi
	//     u64 abcd_efgh = m + ((1ull << 32) - 10000) * ((m * (u128)1844674407370956) >> 64);
	//     __m128i y = _mm_set1_epi64x(abcd_efgh);
	// #ifdef __SSE4_1__
	//     __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
	// #else
	//     __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
	//     __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	//     __m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
	// #endif
	//     __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
	//     const u64 idx = 7 + (6 << 8) + (5 << 16) + (4 << 24) + (3ull << 32) + (2ull << 40) + (1ull << 48) + (0ull << 56);
	//     __m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set1_epi64x(idx));
	//     u64 abcdefgh_BCD = _mm_cvtsi128_si64(little_endian_bcd);
#else

	// method 1 : compute little endian bcd
	u64 aabb_ccdd_merge = (m << 32) + (1 - (10000ull << 32)) * ((m * (u128)1844674407370956) >> 64);
	// u64 aabb_ccdd_merge = (m << 32) - ((10000ull << 32) - 1) * ((m * 109951163) >> 40);
	__m128i y = _mm_set1_epi64x(aabb_ccdd_merge);
#if defined(__SSE4_1__)
	__m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
	__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epi16(y, _mm_set1_epi16(0x147b)), 3);
	__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	__m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
	__m128i z_div_10 = _mm_mulhi_epi16(z, _mm_set1_epi16(0x199a));
	__m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
	u64 abcdefgh_BCD = _mm_cvtsi128_si64(tmp);

	// method 2 : compute big endian bcd , performance worse than method 1
	//      u64 abcd_efgh = m + ((1ull << 32) - 10000) * ((m * (u128)1844674407370956) >> 64);
	//      __m128i y = _mm_set1_epi64x(abcd_efgh);
	//  #ifdef __SSE4_1__
	//      __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
	//  #else
	//      __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
	//      __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	//      __m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
	//  #endif
	//      __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
	//      u64 abcdefgh_BCD = byteswap64(_mm_cvtsi128_si64(big_endian_bcd));

#endif

#endif // endif HAS_SSE2

#if !HAS_NEON_OR_SSE2
	i64 aabb_ccdd_merge = (m << 32) + (1 - (10000ull << 32)) * ((m * 109951163) >> 40);
	i64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) + (1 - (100ull << 16)) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
	u64 abcdefgh_BCD = (aa_bb_cc_dd_merge << 8) + (1 - (10ull << 8)) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
#endif

	abcdefgh_BCD = abcdefgh_BCD >> (lz << 3);
	int tz = u64_lz_bits(abcdefgh_BCD) / 8;
	abcdefgh_BCD = is_little_endian() ? abcdefgh_BCD : byteswap64(abcdefgh_BCD);
	return {abcdefgh_BCD | ZERO, compute_float_dec_sig_len(up_down, tz, lz)};
}

#if HAS_SSE2
// not used
static inline char *write_1_to_16_digit_sse2(u64 m, char *buf, const struct const_value_double *cv)
{
	uint64_t abcdefgh = ((__uint128_t)m * cv->mul_const) >> 90;
	uint64_t ijklmnop = m + abcdefgh * cv->hundred_million;
#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
	const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
	const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
	const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
	const __m512i zmmTen = _mm512_set1_epi64(10);
	const __m512i zero = _mm512_set1_epi64(0);
	const __m512i asciizero = _mm512_set1_epi8('0');
	const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
												 0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
	const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
	// const __m512i permb_const7 = _mm512_castsi128_si512(_mm_set_epi8(0x7f, 0x77, 0x6f, 0x67, 0x5f, 0x57, 0x4f, 0x47, 0x3f, 0x37, 0x2f, 0x27, 0x1f, 0x17, 0x0f, 0x07));
	__m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
	__m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
	__m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
	__m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
	//__m512i highbits_h7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_h, 5), _mm512_slli_epi64(lowbits_h, 7));
	//__m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7));
	//__m512i bcd = _mm512_permutex2var_epi8(highbits_h7, permb_const7, highbits_l7);//another way to permute , but slower.
	__m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
	__m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
	//__m128i little_endian_ascii = _mm512_castsi512_si128(ascii);
	// int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_ascii, _mm512_castsi512_si128(asciizero)));
	// int tz = u64_lz_bits(mask);
	// return {little_endian_ascii, compute_double_dec_sig_len_sse2(up_down, tz, D17)};

#elif defined(__SSSE3__) && (false)
	__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
	__m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#ifdef __SSE4_1__
	__m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3))); //_mm_mullo_epi32 : sse4.1
#else
	__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
	__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	__m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#endif
	__m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a))));
	__m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)); // ssse3
																																	  // int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
																																	  // int tz = u64_lz_bits(mask);
																																	  // __m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
																																	  // return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#else // sse2
	__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
	__m128i x_div_10000 = _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi32(0xd1b71759)), 45);
	__m128i x_mod_10000 = _mm_sub_epi32(x, _mm_mul_epu32(x_div_10000, _mm_set1_epi32(10000)));
	__m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));
#if defined(__SSE4_1__)
	__m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
	__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
	__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
	__m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
	__m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
	__m128i little_endian_bcd = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
	// int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
	// int tz = u64_lz_bits(mask);
	// __m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
	// return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#endif

	int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
	int lz = u64_tz_bits(mask);
	__m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
	static char buf_tmp[32];
	_mm_storeu_si128((__m128i *)buf_tmp, ascii16);
	memcpy(buf, buf_tmp + lz, 16);
	memcpy(buf + 16 - lz, ".0\0", 4);
	return buf + 16 - lz + 2;
}
#endif

static inline char *write_1_to_16_digit(u64 x, char *buf, const struct const_value_double *cv)
{
	// require 1 <= x < 1e16
	const u64 ZERO = 0x3030303030303030;
	const u64 mask = (0x7FULL << 32) | 0x7FULL;
	const u64 mask2 = (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
	i64 xi = x;
	if (x < 100000000)
	{
		// write 1-8 digit
#if HAS_NEON
		// u64 abcd_efgh_u64 = xi + (0x100000000 - 10000) * ((xi * (u128)1844674407370956) >> 64); // xi = abcdefgh
		u64 abcd_efgh_u64 = xi + cv->div10000_m * ((xi * (u128)cv->div10000) >> 64); // xi = abcdefgh
		int32x2_t abcd_efgh = vld1_u64((uint64_t const *)&abcd_efgh_u64);			 // (abcd << 32) + efgh
		// int32x2_t ab_cd_ef_gh = vmla_s32(abcd_efgh, vqdmulh_s32(abcd_efgh, vdup_n_s32(0x147b000)), vdup_n_s32(-100 + 0x10000));
		int32x2_t ab_cd_ef_gh = vmla_s32(abcd_efgh, vqdmulh_s32(abcd_efgh, vdup_n_s32(cv->multipliers32[2])), vdup_n_s32(cv->multipliers32[3]));
		int16x4_t a_b_c_d_e_f_g_h = vmla_s16(ab_cd_ef_gh, vqdmulh_s16(ab_cd_ef_gh, vdup_n_s16(0xce0)), vdup_n_s16(-10 + 0x100));
		u64 bcd_big_endian = vget_lane_u64(a_b_c_d_e_f_g_h, 0);
		u64 lz = u64_lz_bits(bcd_big_endian) / 8; // lz max is 7 , bcd_big_endian = 0 is impossible
		u64 abcdefgh_bcd = is_little_endian() ? byteswap64(bcd_big_endian) : bcd_big_endian;
		u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
		abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz);
		memcpy(buf, &abcdefgh_ascii, 8);
		buf = buf + 8 - lz;
		memcpy(buf, ".0\0", 4);
		buf += 2;
		return buf;
#elif HAS_SSE2

#if defined(__AVX512F__) && defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
		__m512i bcstq_l = _mm512_set1_epi64(xi);
		const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
		const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b,
													 0x0000000010c6f7a0c, 0x00000000a7c5ac472, 0x000000068db8bac72, 0x0000004189374bc6b,
													 0x0000028f5c28f5c29, 0x0000199999999999a);
		// const u64 idx = 0 + (8 << 8) + (16 << 16) + (24 << 24) + (32ull << 32) + (40ull << 40) + (48ull << 48) + (56ull << 56);
		// const __m512i permb_const = _mm512_castsi128_si512(_mm_cvtsi64_si128(idx));
		// __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
		// __m512i highbits_l = _mm512_madd52hi_epu64(_mm512_set1_epi64(0), lowbits_l, _mm512_set1_epi64(10));
		// //__m512i highbits_l = _mm512_srli_epi64(_mm512_add_epi64(lowbits_l, _mm512_slli_epi64(lowbits_l, 2)), 51);
		// __m512i bcd = _mm512_permutexvar_epi8(permb_const, highbits_l);

		const u64 idx7 = 7 + (15 << 8) + (23 << 16) + (31 << 24) + (39ull << 32) + (47ull << 40) + (55ull << 48) + (63ull << 56);
		const __m512i permb_const7 = _mm512_set1_epi64(idx7);
		__m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
		__m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7));
		//__m512i highbits_l7 = _mm512_mullo_epi64(lowbits_l , _mm512_set1_epi64(10<<4));
		__m512i bcd = _mm512_permutexvar_epi8(permb_const7, highbits_l7);

		u64 abcdefgh_BCD = _mm_cvtsi128_si64(_mm512_castsi512_si128(bcd));
		// #elif defined(__SSSE3__) //&& (false) // endif avx512ifma avx512vbmi
		//     u64 abcd_efgh = m + ((1ull << 32) - 10000) * ((m * (u128)1844674407370956) >> 64);
		//     __m128i y = _mm_set1_epi64x(abcd_efgh);
		// #ifdef __SSE4_1__
		//     __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
		// #else
		//     __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
		//     __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
		//     __m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
		// #endif
		//     __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
		//     const u64 idx = 7 + (6 << 8) + (5 << 16) + (4 << 24) + (3ull << 32) + (2ull << 40) + (1ull << 48) + (0ull << 56);
		//     __m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set1_epi64x(idx));
		//     u64 abcdefgh_BCD = _mm_cvtsi128_si64(little_endian_bcd);
#else

		// method 1 : compute little endian bcd
		u64 aabb_ccdd_merge = (xi << 32) + (1 - (10000ull << 32)) * ((xi * (u128)1844674407370956) >> 64);
		// u64 aabb_ccdd_merge = (m << 32) - ((10000ull << 32) - 1) * ((m * 109951163) >> 40);
		__m128i y = _mm_set1_epi64x(aabb_ccdd_merge);
#if defined(__SSE4_1__)
		__m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
		__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epi16(y, _mm_set1_epi16(0x147b)), 3);
		__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
		__m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
		__m128i z_div_10 = _mm_mulhi_epi16(z, _mm_set1_epi16(0x199a));
		__m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
		u64 abcdefgh_BCD = _mm_cvtsi128_si64(tmp);

		// method 2 : compute big endian bcd , performance worse than method 1
		//      u64 abcd_efgh = m + ((1ull << 32) - 10000) * ((m * (u128)1844674407370956) >> 64);
		//      __m128i y = _mm_set1_epi64x(abcd_efgh);
		//  #ifdef __SSE4_1__
		//      __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
		//  #else
		//      __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
		//      __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
		//      __m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
		//  #endif
		//      __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
		//      u64 abcdefgh_BCD = byteswap64(_mm_cvtsi128_si64(big_endian_bcd));

#endif

		u64 lz = u64_tz_bits(abcdefgh_BCD) / 8;
		uint64_t abcdefgh_bcd = !is_little_endian() ? byteswap64(abcdefgh_BCD) : abcdefgh_BCD;
		uint64_t abcdefgh_ascii = abcdefgh_bcd | ZERO;
		abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz); // remove leading zeros
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
		uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
		uint64_t abcdefgh_ascii = abcdefgh_bcd | ZERO;
		abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz); // remove leading zeros
		memcpy(buf, &abcdefgh_ascii, 8);
		buf = buf + 8 - lz;
		memcpy(buf, ".0\0", 4);
		buf += 2;
		return buf;
#endif
	}
	else
	{
		// write 9-16 digit
#if HAS_NEON
		// src from : https://gist.github.com/dougallj/b4f600ab30ef79bb6789bc3f86cd597a#file-convert-neon-cpp-L144-L169
		uint32_t abcdefgh = ((__uint128_t)xi * cv->mul_const) >> 90;
		uint32_t ijklmnop = xi + abcdefgh * cv->hundred_million;
		uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
		int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(cv->multipliers32[0])), 9);
		int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(cv->multipliers32[1]));
		int32x4_t extended = vshll_n_u16(tenthousands, 0);
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__))
		// asm ("" : "+w"(extended));
#endif
		int32x4_t hundreds = vmlaq_s32(extended, vqdmulhq_s32(extended, vdupq_n_s32(cv->multipliers32[2])), vdupq_n_s32(cv->multipliers32[3]));
		int16x8_t BCD_big_endian = vmlaq_s16(hundreds, vqdmulhq_s16(hundreds, vdupq_n_s16(cv->multipliers16[0])), vdupq_n_s16(cv->multipliers16[1]));
		// u64 abcdefgh_BCD_big_endian = vgetq_lane_u64(BCD_big_endian, 0);
		// u64 ijklmnop_BCD_big_endian = vgetq_lane_u64(BCD_big_endian, 1);
		// u64 abcdefgh_lz = u64_lz_bits(abcdefgh_BCD_big_endian) / 8;
		// u64 abcdefgh_bcd = is_little_endian() ? bswap64(abcdefgh_BCD_big_endian) : abcdefgh_BCD_big_endian;
		// u64 ijklmnop_bcd = is_little_endian() ? bswap64(ijklmnop_BCD_big_endian) : ijklmnop_BCD_big_endian;
		// u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
		// u64 ijklmnop_ascii = ijklmnop_bcd | ZERO;
		// abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * abcdefgh_lz) : abcdefgh_ascii << (8 * abcdefgh_lz);
		// memcpy(buf, &abcdefgh_ascii, 8);
		// buf = buf + 8 - abcdefgh_lz;
		// memcpy(buf, &ijklmnop_ascii, 8);
		// buf += 8;
		int8x16_t BCD_little_endian = is_little_endian() ? vrev64q_u8(BCD_big_endian) : vreinterpretq_u8_u16(BCD_big_endian); // big_endian to little_endian , reverse 8 bytes
		u64 abcdefgh_bcd = vgetq_lane_u64(BCD_little_endian, 0);															  // hi
		u64 ijklmnop_bcd = vgetq_lane_u64(BCD_little_endian, 1);															  // lo
		// u64 abcdefgh_lz = u64_tz_bits(abcdefgh_bcd) / 8;          // aabbccdd_BCD != 0
		u64 abcdefgh_lz = u64_lz_bits(vgetq_lane_u64(BCD_big_endian, 0)) / 8;
		u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
		u64 ijklmnop_ascii = ijklmnop_bcd | ZERO;
		abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * abcdefgh_lz) : abcdefgh_ascii << (8 * abcdefgh_lz);
		memcpy(buf, &abcdefgh_ascii, 8);
		buf = buf + 8 - abcdefgh_lz;
		memcpy(buf, &ijklmnop_ascii, 8);
		buf += 8;
#elif HAS_SSE2 && (false)

		uint64_t abcdefgh = ((__uint128_t)xi * cv->mul_const) >> 90;
		uint64_t ijklmnop = xi + abcdefgh * cv->hundred_million;
#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
		const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
		const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
		const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
		const __m512i zmmTen = _mm512_set1_epi64(10);
		const __m512i zero = _mm512_set1_epi64(0);
		const __m512i asciizero = _mm512_set1_epi8('0');
		const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
													 0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
		const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
		// const __m512i permb_const7 = _mm512_castsi128_si512(_mm_set_epi8(0x7f, 0x77, 0x6f, 0x67, 0x5f, 0x57, 0x4f, 0x47, 0x3f, 0x37, 0x2f, 0x27, 0x1f, 0x17, 0x0f, 0x07));
		__m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
		__m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
		__m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
		__m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
		//__m512i highbits_h7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_h, 5), _mm512_slli_epi64(lowbits_h, 7));
		//__m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7));
		//__m512i bcd = _mm512_permutex2var_epi8(highbits_h7, permb_const7, highbits_l7);//another way to permute , but slower.
		__m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
		__m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
		//__m128i little_endian_ascii = _mm512_castsi512_si128(ascii);
		// int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_ascii, _mm512_castsi512_si128(asciizero)));
		// int tz = u64_lz_bits(mask);
		// return {little_endian_ascii, compute_double_dec_sig_len_sse2(up_down, tz, D17)};

#elif defined(__SSSE3__) && (false)
		__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
		__m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#ifdef __SSE4_1__
		__m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3))); //_mm_mullo_epi32 : sse4.1
#else
		__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
		__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
		__m128i z = _mm_or_si128(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#endif
		__m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a))));
		__m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)); // ssse3
																																		  // int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
																																		  // int tz = u64_lz_bits(mask);
																																		  // __m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
																																		  // return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#else // sse2
		__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
		__m128i x_div_10000 = _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi32(0xd1b71759)), 45);
		__m128i x_mod_10000 = _mm_sub_epi32(x, _mm_mul_epu32(x_div_10000, _mm_set1_epi32(10000)));
		__m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));
#if defined(__SSE4_1__)
		__m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
		__m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
		__m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
		__m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
		__m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
		__m128i little_endian_bcd = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
		// int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
		// int tz = u64_lz_bits(mask);
		// __m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
		// return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#endif

		uint64_t abcdefgh_bcd = _mm_extract_epi64(little_endian_bcd, 0);
		uint64_t ijklmnop_bcd = _mm_extract_epi64(little_endian_bcd, 1);
		u64 abcdefgh_lz = u64_tz_bits(abcdefgh_bcd) / 8;
		// abcdefgh_bcd = !is_little_endian() ? byteswap64(abcdefgh_bcd) : abcdefgh_bcd;
		// ijklmnop_bcd = !is_little_endian() ? byteswap64(ijklmnop_bcd) : ijklmnop_bcd;
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

		uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f); //+ 0x3030303030303030;
		uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f); //+ 0x3030303030303030;
		u64 abcdefgh_lz = u64_lz_bits(a_b_c_d_e_f_g_h) / 8;
		uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
		uint64_t ijklmnop_bcd = is_little_endian() ? byteswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;
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

namespace xjb
{
	// void get_pow10_128bit(i64 k,u64 *hi,u64 *lo)
	// {
	// 	// get 10**(-k-1);

	// }
	// static inline
	char *xjb64(double v, char *buf)
	{
		const struct const_value_double *cv = &constants_double;
		const struct double_table_t *t = &double_table;
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // for arm64 processor , fewer instructions , MSVC not support inline asm
		asm("" : "+r"(cv));											  // read constant values from memory to register
#endif
		u64 vi;
		memcpy(&vi, &v, sizeof(v));
		buf[0] = '-';
		buf += vi >> 63;
		u64 ieee_significand = vi & ((1ull << 52) - 1);
		u64 ieee_exponent = (vi << 1) >> 53;
		

#if defined(__amd64__) && !is_real_gcc
	#define amd64_not_gcc
#endif

#ifdef amd64_not_gcc // for x86_64 processor , if not use this code , the performance will be very poor on icpx compiler. 9ns -> 12.5ns. that's why we use this code.
		u64 vi_abs = (vi << 1) >> 1;
		if ((u64)(vi_abs - 2) >= (u64)((2047ull << 52) - 2)) [[unlikely]]
		{
			// generate branch instruction
			if (vi_abs == 0)
				memcpy(buf, "0.0\0\0\0\0", 4);
			if (vi_abs == 1)
				memcpy(buf, "5e-324\0", 8);
			if (vi_abs == (2047ull << 52))
				memcpy(buf, "inf\0\0\0\0", 4);
			if (vi_abs > (2047ull << 52))
				memcpy(buf, "nan\0\0\0\0", 4);
			return buf + (vi_abs == 1 ? 6 : 3);
		}
#endif
		i64 q = (i64)ieee_exponent - 1075;
		u64 nq = -q;
		u64 c = ((1ull << 52) | ieee_significand);


#define use_fast_path_for_integer_xjb 0

#if use_fast_path_for_integer_xjb
#if is_real_gcc
		if (nq <= u64_tz_bits(c)) [[unlikely]] // use unlikely will generate jmp instruction
#else
		if (nq <= u64_tz_bits(c)) //[[unlikely]]
#endif
			return write_1_to_16_digit(c >> nq, buf, cv); // fast path for integer
#endif



		if (ieee_exponent == 0) [[unlikely]]
		{
#ifndef amd64_not_gcc
			if (ieee_significand <= 1)
			{
				return (char *)memcpy(buf, ieee_significand ? "5e-324\0" : "0.0\0\0\0\0", 8) + (ieee_significand ? 6 : 3);
			}
#endif
			c = ieee_significand;
			q = 1 - 1075; // -1074
		}
#ifndef amd64_not_gcc // for arm64 processor , better performance
		if (ieee_exponent == 2047) [[unlikely]]
			return (char *)memcpy(buf, ieee_significand ? "nan" : "inf", 4) + 3;
#endif
		unsigned char h7_precalc = t->h7[ieee_exponent];
		i64 k;
		const int offset = 6;
		u64 regular = ieee_significand > 0;
		u64 irregular = (ieee_significand == 0);

		// 		if (!irregular)
		// 		{
		// #if defined(__SIZEOF_INT128__) && defined(__aarch64__)
		// 			// arm64 : smulh ; x64 : imul
		// 			k = ((i64)(ieee_exponent - 1075) * (u128)(78913ull << (64 - 18))) >> 64;
		// #else
		// 			k = ((i64)(ieee_exponent - 1075) * 78913) >> 18;
		// #endif
		// 		}
		// 		else
		// 		{
		// 			k = ((i64)(ieee_exponent - 1075) * 315653 - 131072) >> 20;
		// 		}

		// #ifdef __amd64__
		// 		i64 get_e10 = -1 - k;
		// 		i64 h = q + ((get_e10 * 217707) >> 16);
		// 		const u64 *pow10_ptr = t->pow10_double + 293 * 2;
		// 		u64 *p10 = (u64 *)&pow10_ptr[get_e10 * 2]; // get 10**(-k-1)
		// 												   // u64 *p10 = (u64*)&pow10_double[293*2 + get_e10*2];
		// #else
		// 		// i64 h = q + ((k * -217707 - 217707) >> 16);
		// 		i64 h = q + ((k * (i64)cv->c2 + (i64)cv->c2) >> 16); // madd instruction , more efficient than mul and add
		// 		const u64 *pow10_ptr = t->pow10_double + 293 * 2 - 2;
		// 		u64 *p10 = (u64 *)&pow10_ptr[k * -2]; // get 10**(-k-1)
		// #endif
		// 		u64 cb = c << (h + (1 + offset));
		// 		u64 pow10_hi = p10[0];
		// 		u64 pow10_lo = p10[1];
		// 		u64 hi64, lo64;
		// 		mul_u128_u64_high128(pow10_hi, pow10_lo, cb, &hi64, &lo64);
		// 		// u128 hi128 = (cb * pow10_hi + ((cb * pow10_lo) >> 64));
		// 		// u64 dot_one = hi128 >> offset;
		// 		u64 dot_one = (hi64 << (64 - offset)) | (lo64 >> offset);
		// 		u64 half_ulp = (pow10_hi >> (-h)) + ((c + 1) & 1);
		// 		u64 up = (half_ulp > ~0 - dot_one);
		// 		u64 down = ((half_ulp >> (irregular)) > dot_one);
		// 		u64 m = (u64)(hi64 >> offset) + up;
		// 		u64 up_down = up + down;
		// 		// u64 D17 = (m >= (u64)1e15);
		// 		u64 D17 = m > (u64)cv->c3;
		// 		u64 mr = D17 ? m : m * 10;
		// 		memcpy(buf, "00000000", 8);
		// 		// memcpy(buf, "0000", 4);
		// 		shortest_ascii16 s = to_ascii16(mr, up_down, D17, cv);
		// 		i64 e10 = k + (15 + D17);
		// #ifdef __amd64__
		// 		// u64 offset_num = (dot_one == (1ull << 62)) ? 0 : (1ull << 63) + 6;
		// 		u64 offset_num = (1ull << 63) + 6;
		// 		if (dot_one == (1ull << 62)) [[unlikely]]
		// 			offset_num = 0;
		// 		// u64 one = ((dot_one * (u128)10 + offset_num) >> 64) + (u64)('0' + '0' * 256);
		// 		u64 one = (u128_madd_hi64(dot_one, 10, offset_num)) + (u64)('0' + '0' * 256);
		// 		if (irregular) [[unlikely]]
		// 			if ((((dot_one >> 54) * 5) & ((1 << 9) - 1)) > (((half_ulp >> 55) * 5)))
		// 				one = (((dot_one >> 54) * 5) >> 9) + 1 + (u64)('0' + '0' * 256);
		// #else
		// 		// u64 one = ((dot_one * (u128)10 + cv->c4) >> 64) + (u64)('0' + '0' * 256);
		// 		u64 one = (u128_madd_hi64(dot_one, 10, cv->c4)) + (u64)('0' + '0' * 256);
		// 		if (dot_one == (1ull << 62)) [[unlikely]]
		// 			one = (u64)('2' + '0' * 256);
		// 		if (irregular) [[unlikely]] // This is a cold code, so it is more efficient to have the compiler automatically generate branch instructions.
		// 		{							// Since the compiler tries to prevent access to memory, it generates branch instructions.
		// 			u64 mask = cv->c6;		// read constant values from memory to register , so this code will be more fast.
		// 			if ((((dot_one >> 54) * 5) & mask) > (((half_ulp >> 55) * 5)))
		// 				one = (((dot_one >> 54) * 5) >> 9) + 1 + (u64)('0' + '0' * 256);
		// 		}
		// #endif

		const u64 ZERO_DIGIT = 0x3030303030303030ull; // "00000000"
		u64 m, one, up_down;
#if defined(__SIZEOF_INT128__) && defined(__aarch64__)
		// arm64 : smulh ; x64 : imul
		k = ((i64)(ieee_exponent - 1075) * (u128)(78913ull << (64 - 18))) >> 64;
#else
		k = ((i64)(ieee_exponent - 1075) * 78913) >> 18;
#endif

#ifdef __amd64__
		i64 get_e10 = -1 - k;
		//i64 h = q + ((get_e10 * 217707) >> 16);
		const u64 *pow10_ptr = t->pow10_double + 293 * 2;
		u64 *p10 = (u64 *)&pow10_ptr[get_e10 * 2]; // get 10**(-k-1)
#else
		//i64 h = q + ((k * (i64)cv->c2 + (i64)cv->c2) >> 16);
		const u64 *pow10_ptr = t->pow10_double + 293 * 2 - 2;
		u64 *p10 = (u64 *)&pow10_ptr[k * -2]; // get 10**(-k-1)
#endif
		//u64 cb = c << (h + (1 + offset));// precalc
		u64 cb = c << h7_precalc;
		u64 pow10_hi = p10[0], pow10_lo = p10[1];
		u64 hi64, lo64;
		mul_u128_u64_high128(pow10_hi, pow10_lo, cb, &hi64, &lo64);
		u64 dot_one = (hi64 << (64 - offset)) | (lo64 >> offset);
		//u64 half_ulp = (pow10_hi >> (-h)) + ((c + 1) & 1);
		u64 half_ulp = (pow10_hi >> (7 - h7_precalc)) + ((c + 1) & 1);
		u64 up = half_ulp > ~0 - dot_one;
		u64 down = half_ulp > dot_one;
		m = (u64)(hi64 >> offset) + up;
		up_down = up + down;
		one = (u128_madd_hi64(dot_one, 10, cv->c4));
		if (irregular) [[unlikely]]
		{
			// irregular case : c is 2**52 , exp range is [1,2046] , only 2046 values are possible. easy to compute
			k = (i64)((ieee_exponent - 1075) * 315653 - 131072) >> 20;
			i64 h = q + ((k * -217707 - 217707) >> 16);
			u64 pow10_hi = t->pow10_double[293 * 2 - 2 + k * -2];
			u64 half_ulp = pow10_hi >> (-h);
			u64 dot_one = (pow10_hi << (53 + h));
			u64 up = (half_ulp > ~0 - dot_one);
			u64 down = ((half_ulp >> 1) > dot_one);
			m = (pow10_hi >> (11 - h)) + up;
			up_down = up + down;
			one = ((dot_one >> (53 + h)) * 5 + (1 << (9 - h))) >> (10 - h);
			if ((((dot_one >> 54) * 5) & ((1 << 9) - 1)) > (((half_ulp >> 55) * 5)))
				one = ((((dot_one >> 54) * 5) >> 9) + 1);
		}
		if (dot_one == (1ull << 62)) [[unlikely]] // branch instruction
			one = 2;
		// D17 = 1 : has 17 digits
		// D17 = 0 : has 16 digits
		u64 D17 = m > (u64)cv->c3; // (m >= (u64)1e15);
		u64 mr = D17 ? m : m * 10;
		// if arm64 : not remove left zero , better performance, high ipc
		shortest_ascii16 s = to_ascii16(buf, NOT_REMOVE_FIRST_ZERO ? m : mr, up_down, D17, cv);
		i64 e10 = k + (15 + D17);

		const i64 e10_DN = t->e10_DN;
		const i64 e10_UP = t->e10_UP;
		u64 e10_3 = e10 + (-e10_DN);
		u64 e10_data_ofs = e10_3 < e10_UP - e10_DN + 1 ? e10_3 : e10_UP - e10_DN + 1; // compute offset , min(e10_3,19)
		u64 first_sig_pos = t->e10_variable_data[e10_data_ofs][17 + 0];
		u64 dot_pos = t->e10_variable_data[e10_data_ofs][17 + 1];
		u64 move_pos = t->e10_variable_data[e10_data_ofs][17 + 2];
		//u64 move_pos = dot_pos + (u64(e10) < (u64)e10_DN);
		u64 exp_pos = t->e10_variable_data[e10_data_ofs][s.dec_sig_len];
		char *buf_origin = buf;
		buf += first_sig_pos;
#if HAS_NEON_OR_SSE2
		memcpy(buf, &(s.ascii16), 16);
#else
		memcpy(buf + 0, &(s.hi), 8);
		memcpy(buf + 8, &(s.lo), 8);
#endif

#if NOT_REMOVE_FIRST_ZERO
		memmove(buf, &buf[16 - (15 + D17)], 16);
#endif

		one |= 0x3030;
		memcpy(&buf[15 + D17], &one, 8);
		//if((u64(e10) < (u64)e10_DN))memmove(&buf[dot_pos + 1], &buf[dot_pos], 16);
		memmove(&buf[move_pos], &buf[dot_pos], 16); // dot_pos+first_sig_pos+sign max = 16+1 = 17; require 17+16=33 byte buffer
		buf_origin[dot_pos] = '.';
		if (ieee_exponent == 0) [[unlikely]]
		{
			// some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
			//if (buf[0] == '0')
			if (m < (u64)1e14)
			{
				u64 lz = 0;
				while (buf[2 + lz] == '0')
					lz++;
				lz += 2;
				e10 -= lz - 1;
				buf[0] = buf[lz];
				// byte_move_16(&buf[2], &buf[lz + 1]);
				memmove(&buf[2], &buf[lz + 1], 16);
				exp_pos = exp_pos - lz + (exp_pos - lz != 1);
			}
		}
		u64 exp_result = t->exp_result_double[e10 + 324];
		// exp_result = is_little_endian() ? exp_result : byteswap64(exp_result);
		buf += exp_pos;
		memcpy(buf, &exp_result, 8);
		u64 exp_len = exp_result >> 56;
		return buf + exp_len;
	}
	static inline
	char *xjb32(float v, char *buf)
	{
		const struct const_value_float *c = &constants_float;
		const struct float_table_t *t = &float_table;
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // for arm64 processor , fewer instructions , MSVC not support inline asm
		asm("" : "+r"(c));											  // read constant values from memory to register
#endif
		u32 vi;
		memcpy(&vi, &v, 4);
		buf[0] = '-';
		buf += vi >> 31;
		u64 sig = vi & ((1 << 23) - 1);
		u64 exp = (vi << 1) >> 24;
		u64 sig_bin = sig | (1 << 23);
		i64 exp_bin = (i64)exp - 150;
		if (exp == 0) [[unlikely]]
		{
			if (sig == 0)
				return (char *)memcpy(buf, "0.0", 4) + 3;
			exp_bin = 1 - 150;
			sig_bin = sig;
		}
		if (exp == 255) [[unlikely]]
			return (char *)memcpy(buf, sig ? "nan" : "inf", 4) + 3;
		unsigned char h37_precalc = t->h37[exp];
		u64 irregular = sig == 0;
		const int BIT = 36;
#if defined(__SIZEOF_INT128__) // for arm64 processor , fewer instructions
		// arm64 : single smulh instruction can be used to calculate high 64 bits of multiplication
		// x64 : gcc can not optimize this to a single imul instruction on x86_64 processor , but clang and icpx can optimize it
		i64 k = ((i64)exp_bin * (u128)(1233ull << 52)) >> 64; // signed multiplication
#else
		i64 k = (exp_bin * 1233) >> 12; // exp_bin range : [-149,104] ; k range : [-45,31]
#endif
		// if (irregular) [[unlikely]]
		// {
		// 	k = (i64)(exp_bin * 1233 - 512) >> 12;
		// 	h37_precalc = (BIT + 1) + exp_bin + ((k * -1701 + (-1701)) >> 9);
		// }
		u64 pow10_hi = t->pow10_float_reverse[45 + k];
		u64 even = (sig + 1) & 1; // or (sig_bin + 1) & 1
		u64 cb = sig_bin << h37_precalc;
		// u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
		u64 sig_hi = umul128_hi64_xjb(cb, pow10_hi);
		memcpy(buf, "00000000", 8);
		u64 half_ulp = (pow10_hi >> (65 - h37_precalc)) + even;
		u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1);
#ifdef __amd64__
		u64 up = (half_ulp + dot_one_36bit) >> BIT;
#else
		//u64 up = half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit;
		u64 up = dot_one_36bit > (((u64)1 << BIT) - 1) - half_ulp;
#endif
		u64 down = half_ulp > dot_one_36bit;
		u64 up_down = up + down;
		u64 m = (sig_hi >> BIT) + up;
		u64 offset_num = c->c1 + (dot_one_36bit >> (BIT - 4));
		u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
		if (irregular) [[unlikely]]
		{
			if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150)) // branch instruction
				++one;
			u64 down = (half_ulp >> 1) > dot_one_36bit;
			up_down = up + down;
			if(exp_bin == 24 - 150)return (char*)memcpy(buf,"9.8607613e-32\0\0",16) + 13;
			if(exp_bin == 57 - 150)return (char*)memcpy(buf,"8.4703295e-22\0\0",16) + 13;
			if(exp_bin == 67 - 150)return (char*)memcpy(buf,"8.6736174e-19\0\0",16) + 13;
			if(exp_bin == 220 - 150)return (char*)memcpy(buf,"9.9035203e+27\0\0",16) + 13;
		}
		
		
		
		// u64 lz = (m < (u32)1e7) + (m < (u32)1e6); // 0, 1, 2
		u64 lz = (m < c->e6) + (m < c->e7);
		//u64 lz = (m < c->e6) ? 2 : (m < c->e7);
		shortest_ascii8 s = to_ascii8(m, up_down, lz, c);
		i64 e10 = k + (8 - lz);
		// u64 offset_num = (((u64)('0' + '0' * 256) << (BIT - 1)) + (((u64)1 << (BIT - 2)) - 7)) + (dot_one_36bit >> (BIT - 4));
		// u64 offset_num = c->c1 + (dot_one_36bit >> (BIT - 4));
		// u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
		// // one = cmov_branchless(up_down, '0' + '0' * 256, one); // prevent gcc generate branch instruction
		// if (irregular) [[unlikely]]
		// {
		// 	if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150)) // branch instruction
		// 		++one;
		// }

		const i64 e10_DN = t->e10_DN, e10_UP = t->e10_UP;
		u64 e10_3 = e10 + (-e10_DN);
		u64 e10_data_ofs = e10_3 < e10_UP - e10_DN + 1 ? e10_3 : e10_UP - e10_DN + 1;
		u64 exp_len = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : 4;
		u64 first_sig_pos = t->e10_variable_data[e10_data_ofs][9 + 0];
		u64 dot_pos = t->e10_variable_data[e10_data_ofs][9 + 1];
		u64 move_pos = t->e10_variable_data[e10_data_ofs][9 + 2];
		u64 exp_pos = t->e10_variable_data[e10_data_ofs][s.dec_sig_len];
		char *buf_origin = (char *)buf;
		buf += first_sig_pos;
		memcpy(buf, &(s.ascii), 8);
		memcpy(&buf[8 - lz], &one, 8);
		memmove(&buf[move_pos], &buf[dot_pos], 8);
		buf_origin[dot_pos] = '.';
#if defined(__aarch64__) // for arm64 processor , fewer instructions
		if (exp == 0) [[unlikely]]
#endif
			if (m < 100000) [[unlikely]] // some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
			{
				u64 lz = 0;
				// while (buf[2 + lz] == '0')
				//     lz++;
				u64 u;
				memcpy(&u, &buf[2], 8);
				u = is_little_endian() ? u : byteswap64(u);
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
		return buf + exp_len;
	}
	char *to_string(float v, char *buf) { return xjb32(v, buf); }
	char *to_string(double v, char *buf) { return xjb64(v, buf); }
} // end of namespace xjb
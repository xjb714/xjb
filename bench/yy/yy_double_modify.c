#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
//#include <immintrin.h>

/*==============================================================================
 * Compiler Macros
 *============================================================================*/

/* compiler builtin check (since gcc 10.0, clang 2.6, icc 2021) */
#ifndef yy_has_builtin
#ifdef __has_builtin
#define yy_has_builtin(x) __has_builtin(x)
#else
#define yy_has_builtin(x) 0
#endif
#endif

/* compiler attribute check (since gcc 5.0, clang 2.9, icc 17) */
#ifndef yy_has_attribute
#ifdef __has_attribute
#define yy_has_attribute(x) __has_attribute(x)
#else
#define yy_has_attribute(x) 0
#endif
#endif

/* include check (since gcc 5.0, clang 2.7, icc 16, msvc 2017 15.3) */
#ifndef yy_has_include
#ifdef __has_include
#define yy_has_include(x) __has_include(x)
#else
#define yy_has_include(x) 0
#endif
#endif

/* inline for compiler */
#ifndef yy_inline
#if _MSC_VER >= 1200
#define yy_inline __forceinline
#elif defined(_MSC_VER)
#define yy_inline __inline
#elif yy_has_attribute(always_inline) || __GNUC__ >= 4
#define yy_inline __inline__ __attribute__((always_inline))
#elif defined(__clang__) || defined(__GNUC__)
#define yy_inline __inline__
#elif defined(__cplusplus) || (__STDC__ >= 1 && __STDC_VERSION__ >= 199901L)
#define yy_inline inline
#else
#define yy_inline
#endif
#endif

/* noinline for compiler */
#ifndef yy_noinline
#if _MSC_VER >= 1200
#define yy_noinline __declspec(noinline)
#elif yy_has_attribute(noinline) || __GNUC__ >= 4
#define yy_noinline __attribute__((noinline))
#else
#define yy_noinline
#endif
#endif

/* align for compiler */
#ifndef yy_align
#if defined(_MSC_VER)
#define yy_align(x) __declspec(align(x))
#elif yy_has_attribute(aligned) || defined(__GNUC__)
#define yy_align(x) __attribute__((aligned(x)))
#elif __cplusplus >= 201103L
#define yy_align(x) alignas(x)
#else
#define yy_align(x)
#endif
#endif

/* likely for compiler */
#ifndef yy_likely
#if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#define yy_likely(expr) __builtin_expect(!!(expr), 1)
#else
#define yy_likely(expr) (expr)
#endif
#endif

/* unlikely for compiler */
#ifndef yy_unlikely
#if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#define yy_unlikely(expr) __builtin_expect(!!(expr), 0)
#else
#define yy_unlikely(expr) (expr)
#endif
#endif

/* compiler version (GCC) */
#ifndef yy_gcc_available
#define yy_gcc_available(major, minor, patch)                           \
    ((__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= \
     (major * 10000 + minor * 100 + patch))
#endif

/* real gcc check */
#ifndef yy_is_real_gcc
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    !defined(__clang__) && !defined(__llvm__) &&    \
    !defined(__INTEL_COMPILER) && !defined(__ICC)
#define yy_is_real_gcc 1
#endif
#endif

/*
 Compiler barriers for single variable.

 These macros informs GCC that a read/write access to the given memory location
 will occur, preventing certain compiler optimizations or reordering around
 the access to 'val'. It does not emit any actual instructions.

 Useful when GCC's default optimization strategies are suboptimal and need
 precise control over memory access patterns.
 */
#if defined(yy_is_real_gcc)
#define gcc_load_barrier(val) __asm__ volatile("" ::"m"(val))
#define gcc_store_barrier(val) __asm__ volatile("" : "=m"(val))
#define gcc_full_barrier(val) __asm__ volatile("" : "=m"(val) : "m"(val))
#else
#define gcc_load_barrier(val)
#define gcc_store_barrier(val)
#define gcc_full_barrier(val)
#endif

/* msvc intrinsic */
#if _MSC_VER >= 1400
#include <intrin.h>
#if defined(_M_AMD64) || defined(_M_ARM64)
#define MSC_HAS_BIT_SCAN_64 1
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#endif
#if defined(_M_AMD64) || defined(_M_ARM64) || \
    defined(_M_IX86) || defined(_M_ARM)
#define MSC_HAS_BIT_SCAN 1
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#endif
#if defined(_M_AMD64)
#define MSC_HAS_UMUL128 1
#pragma intrinsic(_umul128)
#endif
#endif

/* gcc builtin */
#if yy_has_builtin(__builtin_clzll) || yy_gcc_available(3, 4, 0)
#define GCC_HAS_CLZLL 1
#endif

#if yy_has_builtin(__builtin_ctzll) || yy_gcc_available(3, 4, 0)
#define GCC_HAS_CTZLL 1
#endif

/* int128 type */
#ifndef YY_HAS_INT128
#if (__SIZEOF_INT128__ == 16) && \
    (defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER))
#define YY_HAS_INT128 1
#else
#define YY_HAS_INT128 0
#endif
#endif

/* IEEE 754 floating-point binary representation */
#ifndef YY_HAS_IEEE_754
#if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
#define YY_HAS_IEEE_754 1
#elif FLT_RADIX == 2 &&                            \
    DBL_MANT_DIG == 53 && DBL_DIG == 15 &&         \
    DBL_MIN_EXP == -1021 && DBL_MAX_EXP == 1024 && \
    DBL_MIN_10_EXP == -307 && DBL_MAX_10_EXP == 308
#define YY_HAS_IEEE_754 1
#else
#define YY_HAS_IEEE_754 0
#warning IEEE 754 not detected
#if __FAST_MATH__ || __USE_FAST_MATH__
#warning "-ffast-math" may break the nan/inf check
#endif
#endif
#endif

/*
 Correct rounding in double number computations.

 On the x86 architecture, some compilers may use x87 FPU instructions for
 floating-point arithmetic. The x87 FPU loads all floating point number as
 80-bit double-extended precision internally, then rounds the result to original
 precision, which may produce inaccurate results. For a more detailed
 explanation, see the paper: https://arxiv.org/abs/cs/0701192

 Here are some examples of double precision calculation error:

     2877.0 / 1e6   == 0.002877,  but x87 returns 0.0028770000000000002
     43683.0 * 1e21 == 4.3683e25, but x87 returns 4.3683000000000004e25

 Here are some examples of compiler flags to generate x87 instructions on x86:

     clang -m32 -mno-sse
     gcc/icc -m32 -mfpmath=387
     msvc /arch:SSE or /arch:IA32

 If we are sure that there's no similar error described above, we can define the
 YY_DOUBLE_MATH_CORRECT as 1 to enable the fast path calculation. This is
 not an accurate detection, it's just try to avoid the error at compile-time.
 An accurate detection can be done at run-time:

     bool is_double_math_correct(void) {
         volatile double r = 43683.0;
         r *= 1e21;
         return r == 4.3683e25;
     }

 See also: utils.h in https://github.com/google/double-conversion/
 */
#if !defined(FLT_EVAL_METHOD) && defined(__FLT_EVAL_METHOD__)
#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif

#if defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD != 0 && FLT_EVAL_METHOD != 1
#define YY_DOUBLE_MATH_CORRECT 0
#elif defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(_X86_) || defined(__X86__) || defined(_M_IX86) ||  \
    defined(__I86__) || defined(__IA32__) || defined(__THW_INTEL)
#if (defined(_MSC_VER) && defined(_M_IX86_FP) && _M_IX86_FP == 2) || \
    (defined(__SSE2_MATH__) && __SSE2_MATH__)
#define YY_DOUBLE_MATH_CORRECT 1
#else
#define YY_DOUBLE_MATH_CORRECT 0
#endif
#elif defined(__mc68000__) || defined(__pnacl__) || defined(__native_client__)
#define YY_DOUBLE_MATH_CORRECT 0
#else
#define YY_DOUBLE_MATH_CORRECT 1
#endif

/*==============================================================================
 * Macros
 *============================================================================*/

/* Macros used for loop unrolling and other purpose. */
#define repeat2(x) {x x}
#define repeat3(x) {x x x}
#define repeat4(x) {x x x x}
#define repeat8(x) {x x x x x x x x}
#define repeat16(x) {x x x x x x x x x x x x x x x x}

#define repeat4_incr(x) {x(0) x(1) x(2) x(3)}

#define repeat8_incr(x) {x(0) x(1) x(2) x(3) x(4) x(5) x(6) x(7)}

#define repeat16_incr(x) {x(0) x(1) x(2) x(3) x(4) x(5) x(6) x(7) \
                              x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15)}

#define repeat_in_1_18(x) {x(1) x(2) x(3) x(4) x(5) x(6) x(7)                \
                               x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15) \
                                   x(16) x(17) x(18)}

/* Macros used to provide branch prediction information for compiler. */
#undef likely
#define likely(x) yy_likely(x)
#undef unlikely
#define unlikely(x) yy_unlikely(x)

/* Macros used to provide inline information for compiler. */
#undef static_inline
#define static_inline static yy_inline
#undef static_noinline
#define static_noinline static yy_noinline

/* Macros for min and max. */
#undef yy_min
#define yy_min(x, y) ((x) < (y) ? (x) : (y))
#undef yy_max
#define yy_max(x, y) ((x) > (y) ? (x) : (y))

/* Used to write u64 literal for C89 which doesn't support "ULL" suffix. */
#undef U64
#define U64(hi, lo) ((((u64)hi##UL) << 32U) + lo##UL)

/*==============================================================================
 * Integer Constants
 *============================================================================*/

/* U64 constant values */
#undef U64_MAX
#define U64_MAX U64(0xFFFFFFFF, 0xFFFFFFFF)
#undef I64_MAX
#define I64_MAX U64(0x7FFFFFFF, 0xFFFFFFFF)
#undef USIZE_MAX
#define USIZE_MAX ((usize)(~(usize)0))

/* Maximum number of digits for reading u64 safety. */
#undef U64_SAFE_DIG
#define U64_SAFE_DIG 19

/*==============================================================================
 * IEEE-754 Double Number Constants
 *============================================================================*/

/* Inf raw value (positive) */
#define F64_RAW_INF U64(0x7FF00000, 0x00000000)

/* NaN raw value (positive, without payload) */
#define F64_RAW_NAN U64(0x7FF80000, 0x00000000)

/* double number bits */
#define F64_BITS 64

/* double number exponent part bits */
#define F64_EXP_BITS 11

/* double number significand part bits */
#define F64_SIG_BITS 52

/* double number significand part bits (with 1 hidden bit) */
#define F64_SIG_FULL_BITS 53

/* double number significand bit mask */
#define F64_SIG_MASK U64(0x000FFFFF, 0xFFFFFFFF)

/* double number exponent bit mask */
#define F64_EXP_MASK U64(0x7FF00000, 0x00000000)

/* double number exponent bias */
#define F64_EXP_BIAS 1023

/* double number significant digits count in decimal */
#define F64_DEC_DIG 17

/* max significant digits count in decimal when reading double number */
#define F64_MAX_DEC_DIG 768

/* maximum decimal power of double number (1.7976931348623157e308) */
#define F64_MAX_DEC_EXP 308

/* minimum decimal power of double number (4.9406564584124654e-324) */
#define F64_MIN_DEC_EXP (-324)

/* maximum binary power of double number */
#define F64_MAX_BIN_EXP 1024

/* minimum binary power of double number */
#define F64_MIN_BIN_EXP (-1021)

/*==============================================================================
 * Types
 *============================================================================*/

/** Type define for primitive types. */
typedef float f32;
typedef double f64;
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef size_t usize;
#if YY_HAS_INT128
__extension__ typedef __int128 i128;
__extension__ typedef unsigned __int128 u128;
#endif

/*==============================================================================
 * Load/Store Utils
 *============================================================================*/

static_inline void byte_copy_2(void *dst, const void *src)
{
    memcpy(dst, src, 2);
}

static_inline void byte_copy_4(void *dst, const void *src)
{
    memcpy(dst, src, 4);
}

static_inline void byte_copy_8(void *dst, const void *src)
{
    memcpy(dst, src, 8);
}

static_inline void byte_move_16(void *dst, const void *src)
{
    char *pdst = (char *)dst;
    const char *psrc = (const char *)src;
    u64 tmp1, tmp2;
    memcpy(&tmp1, psrc, 8);
    memcpy(&tmp2, psrc + 8, 8);
    memcpy(pdst, &tmp1, 8);
    memcpy(pdst + 8, &tmp2, 8);
}

/*==============================================================================
 * Number Utils
 *============================================================================*/

/** Convert raw binary to double. */
static_inline f64 f64_from_raw(u64 u)
{
    f64 f;
    memcpy(&f, &u, sizeof(u));
    return f;
}

/** Convert double to raw binary. */
static_inline u64 f64_to_raw(f64 f)
{
    u64 u;
    memcpy(&u, &f, sizeof(u));
    return u;
}

/*==============================================================================
 * Bits Utils
 *============================================================================*/

/** Returns the number of leading 0-bits in value (input should not be 0). */
static_inline
    u32
    u64_lz_bits(u64 v)
{
#if GCC_HAS_CLZLL
    return (u32)__builtin_clzll(v);
#elif MSC_HAS_BIT_SCAN_64
    unsigned long r;
    _BitScanReverse64(&r, v);
    return (u32)63 - (u32)r;
#elif MSC_HAS_BIT_SCAN
    unsigned long hi, lo;
    bool hi_set = _BitScanReverse(&hi, (u32)(v >> 32)) != 0;
    _BitScanReverse(&lo, (u32)v);
    hi |= 32;
    return (u32)63 - (u32)(hi_set ? hi : lo);
#else
    /* branchless, use de Bruijn sequences */
    const u8 table[64] = {
        63, 16, 62, 7, 15, 36, 61, 3, 6, 14, 22, 26, 35, 47, 60, 2,
        9, 5, 28, 11, 13, 21, 42, 19, 25, 31, 34, 40, 46, 52, 59, 1,
        17, 8, 37, 4, 23, 27, 48, 10, 29, 12, 43, 20, 32, 41, 53, 18,
        38, 24, 49, 30, 44, 33, 54, 39, 50, 45, 55, 51, 56, 57, 58, 0};
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return table[(v * U64(0x03F79D71, 0xB4CB0A89)) >> 58];
#endif
}

/** Returns the number of trailing 0-bits in value (input should not be 0). */
static_inline u32 u64_tz_bits(u64 v)
{
#if GCC_HAS_CTZLL
    return (u32)__builtin_ctzll(v);
#elif MSC_HAS_BIT_SCAN_64
    unsigned long r;
    _BitScanForward64(&r, v);
    return (u32)r;
#elif MSC_HAS_BIT_SCAN
    unsigned long lo, hi;
    bool lo_set = _BitScanForward(&lo, (u32)(v)) != 0;
    _BitScanForward(&hi, (u32)(v >> 32));
    hi += 32;
    return lo_set ? lo : hi;
#else
    /* branchless, use de Bruijn sequences */
    const u8 table[64] = {
        0, 1, 2, 53, 3, 7, 54, 27, 4, 38, 41, 8, 34, 55, 48, 28,
        62, 5, 39, 46, 44, 42, 22, 9, 24, 35, 59, 56, 49, 18, 29, 11,
        63, 52, 6, 26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12};
    return table[((v & (~v + 1)) * U64(0x022FDD63, 0xCC95386D)) >> 58];
#endif
}

/*==============================================================================
 * 128-bit Integer Utils
 *============================================================================*/

/** Multiplies two 64-bit unsigned integers (a * b),
    returns the 128-bit result as 'hi' and 'lo'. */
static_inline void u128_mul(u64 a, u64 b, u64 *hi, u64 *lo)
{
#if YY_HAS_INT128
    u128 m = (u128)a * b;
    *hi = (u64)(m >> 64);
    *lo = (u64)(m);
#elif MSC_HAS_UMUL128
    *lo = _umul128(a, b, hi);
#else
    u32 a0 = (u32)(a), a1 = (u32)(a >> 32);
    u32 b0 = (u32)(b), b1 = (u32)(b >> 32);
    u64 p00 = (u64)a0 * b0, p01 = (u64)a0 * b1;
    u64 p10 = (u64)a1 * b0, p11 = (u64)a1 * b1;
    u64 m0 = p01 + (p00 >> 32);
    u32 m00 = (u32)(m0), m01 = (u32)(m0 >> 32);
    u64 m1 = p10 + m00;
    u32 m10 = (u32)(m1), m11 = (u32)(m1 >> 32);
    *hi = p11 + m01 + m11;
    *lo = ((u64)m10 << 32) | (u32)p00;
#endif
}

/** Multiplies two 64-bit unsigned integers and add a value (a * b + c),
    returns the 128-bit result as 'hi' and 'lo'. */
static_inline void u128_mul_add(u64 a, u64 b, u64 c, u64 *hi, u64 *lo)
{
#if YY_HAS_INT128
    u128 m = (u128)a * b + c;
    *hi = (u64)(m >> 64);
    *lo = (u64)(m);
#else
    u64 h, l, t;
    u128_mul(a, b, &h, &l);
    t = l + c;
    h += ((t < l) | (t < c));
    *hi = h;
    *lo = t;
#endif
}

/*==============================================================================
 * Power10 Lookup Table
 *============================================================================*/

/** Minimum decimal exponent in pow10_sig_table. */
#define POW10_SIG_TABLE_MIN_EXP -343

/** Maximum decimal exponent in pow10_sig_table. */
#define POW10_SIG_TABLE_MAX_EXP 324

/** Minimum exact decimal exponent in pow10_sig_table */
#define POW10_SIG_TABLE_MIN_EXACT_EXP 0

/** Maximum exact decimal exponent in pow10_sig_table */
#define POW10_SIG_TABLE_MAX_EXACT_EXP 55

/** Normalized significant 128 bits of pow10, no rounded up (size: 10.4KB).
    This lookup table is used by both the double number reader and writer.
    (generate with misc/make_tables.c) */
static const u64 pow10_sig_table[] = {
    U64(0xBF29DCAB, 0xA82FDEAE), U64(0x7432EE87, 0x3880FC33),                /* ~= 10^-343 */
    U64(0xEEF453D6, 0x923BD65A), U64(0x113FAA29, 0x06A13B3F),                /* ~= 10^-342 */
    U64(0x9558B466, 0x1B6565F8), U64(0x4AC7CA59, 0xA424C507),                /* ~= 10^-341 */
    U64(0xBAAEE17F, 0xA23EBF76), U64(0x5D79BCF0, 0x0D2DF649),                /* ~= 10^-340 */
    U64(0xE95A99DF, 0x8ACE6F53), U64(0xF4D82C2C, 0x107973DC),                /* ~= 10^-339 */
    U64(0x91D8A02B, 0xB6C10594), U64(0x79071B9B, 0x8A4BE869),                /* ~= 10^-338 */
    U64(0xB64EC836, 0xA47146F9), U64(0x9748E282, 0x6CDEE284),                /* ~= 10^-337 */
    U64(0xE3E27A44, 0x4D8D98B7), U64(0xFD1B1B23, 0x08169B25),                /* ~= 10^-336 */
    U64(0x8E6D8C6A, 0xB0787F72), U64(0xFE30F0F5, 0xE50E20F7),                /* ~= 10^-335 */
    U64(0xB208EF85, 0x5C969F4F), U64(0xBDBD2D33, 0x5E51A935),                /* ~= 10^-334 */
    U64(0xDE8B2B66, 0xB3BC4723), U64(0xAD2C7880, 0x35E61382),                /* ~= 10^-333 */
    U64(0x8B16FB20, 0x3055AC76), U64(0x4C3BCB50, 0x21AFCC31),                /* ~= 10^-332 */
    U64(0xADDCB9E8, 0x3C6B1793), U64(0xDF4ABE24, 0x2A1BBF3D),                /* ~= 10^-331 */
    U64(0xD953E862, 0x4B85DD78), U64(0xD71D6DAD, 0x34A2AF0D),                /* ~= 10^-330 */
    U64(0x87D4713D, 0x6F33AA6B), U64(0x8672648C, 0x40E5AD68),                /* ~= 10^-329 */
    U64(0xA9C98D8C, 0xCB009506), U64(0x680EFDAF, 0x511F18C2),                /* ~= 10^-328 */
    U64(0xD43BF0EF, 0xFDC0BA48), U64(0x0212BD1B, 0x2566DEF2),                /* ~= 10^-327 */
    U64(0x84A57695, 0xFE98746D), U64(0x014BB630, 0xF7604B57),                /* ~= 10^-326 */
    U64(0xA5CED43B, 0x7E3E9188), U64(0x419EA3BD, 0x35385E2D),                /* ~= 10^-325 */
    U64(0xCF42894A, 0x5DCE35EA), U64(0x52064CAC, 0x828675B9),                /* ~= 10^-324 */
    U64(0x818995CE, 0x7AA0E1B2), U64(0x7343EFEB, 0xD1940993),                /* ~= 10^-323 */
    U64(0xA1EBFB42, 0x19491A1F), U64(0x1014EBE6, 0xC5F90BF8),                /* ~= 10^-322 */
    U64(0xCA66FA12, 0x9F9B60A6), U64(0xD41A26E0, 0x77774EF6),                /* ~= 10^-321 */
    U64(0xFD00B897, 0x478238D0), U64(0x8920B098, 0x955522B4),                /* ~= 10^-320 */
    U64(0x9E20735E, 0x8CB16382), U64(0x55B46E5F, 0x5D5535B0),                /* ~= 10^-319 */
    U64(0xC5A89036, 0x2FDDBC62), U64(0xEB2189F7, 0x34AA831D),                /* ~= 10^-318 */
    U64(0xF712B443, 0xBBD52B7B), U64(0xA5E9EC75, 0x01D523E4),                /* ~= 10^-317 */
    U64(0x9A6BB0AA, 0x55653B2D), U64(0x47B233C9, 0x2125366E),                /* ~= 10^-316 */
    U64(0xC1069CD4, 0xEABE89F8), U64(0x999EC0BB, 0x696E840A),                /* ~= 10^-315 */
    U64(0xF148440A, 0x256E2C76), U64(0xC00670EA, 0x43CA250D),                /* ~= 10^-314 */
    U64(0x96CD2A86, 0x5764DBCA), U64(0x38040692, 0x6A5E5728),                /* ~= 10^-313 */
    U64(0xBC807527, 0xED3E12BC), U64(0xC6050837, 0x04F5ECF2),                /* ~= 10^-312 */
    U64(0xEBA09271, 0xE88D976B), U64(0xF7864A44, 0xC633682E),                /* ~= 10^-311 */
    U64(0x93445B87, 0x31587EA3), U64(0x7AB3EE6A, 0xFBE0211D),                /* ~= 10^-310 */
    U64(0xB8157268, 0xFDAE9E4C), U64(0x5960EA05, 0xBAD82964),                /* ~= 10^-309 */
    U64(0xE61ACF03, 0x3D1A45DF), U64(0x6FB92487, 0x298E33BD),                /* ~= 10^-308 */
    U64(0x8FD0C162, 0x06306BAB), U64(0xA5D3B6D4, 0x79F8E056),                /* ~= 10^-307 */
    U64(0xB3C4F1BA, 0x87BC8696), U64(0x8F48A489, 0x9877186C),                /* ~= 10^-306 */
    U64(0xE0B62E29, 0x29ABA83C), U64(0x331ACDAB, 0xFE94DE87),                /* ~= 10^-305 */
    U64(0x8C71DCD9, 0xBA0B4925), U64(0x9FF0C08B, 0x7F1D0B14),                /* ~= 10^-304 */
    U64(0xAF8E5410, 0x288E1B6F), U64(0x07ECF0AE, 0x5EE44DD9),                /* ~= 10^-303 */
    U64(0xDB71E914, 0x32B1A24A), U64(0xC9E82CD9, 0xF69D6150),                /* ~= 10^-302 */
    U64(0x892731AC, 0x9FAF056E), U64(0xBE311C08, 0x3A225CD2),                /* ~= 10^-301 */
    U64(0xAB70FE17, 0xC79AC6CA), U64(0x6DBD630A, 0x48AAF406),                /* ~= 10^-300 */
    U64(0xD64D3D9D, 0xB981787D), U64(0x092CBBCC, 0xDAD5B108),                /* ~= 10^-299 */
    U64(0x85F04682, 0x93F0EB4E), U64(0x25BBF560, 0x08C58EA5),                /* ~= 10^-298 */
    U64(0xA76C5823, 0x38ED2621), U64(0xAF2AF2B8, 0x0AF6F24E),                /* ~= 10^-297 */
    U64(0xD1476E2C, 0x07286FAA), U64(0x1AF5AF66, 0x0DB4AEE1),                /* ~= 10^-296 */
    U64(0x82CCA4DB, 0x847945CA), U64(0x50D98D9F, 0xC890ED4D),                /* ~= 10^-295 */
    U64(0xA37FCE12, 0x6597973C), U64(0xE50FF107, 0xBAB528A0),                /* ~= 10^-294 */
    U64(0xCC5FC196, 0xFEFD7D0C), U64(0x1E53ED49, 0xA96272C8),                /* ~= 10^-293 */
    U64(0xFF77B1FC, 0xBEBCDC4F), U64(0x25E8E89C, 0x13BB0F7A),                /* ~= 10^-292 */
    U64(0x9FAACF3D, 0xF73609B1), U64(0x77B19161, 0x8C54E9AC),                /* ~= 10^-291 */
    U64(0xC795830D, 0x75038C1D), U64(0xD59DF5B9, 0xEF6A2417),                /* ~= 10^-290 */
    U64(0xF97AE3D0, 0xD2446F25), U64(0x4B057328, 0x6B44AD1D),                /* ~= 10^-289 */
    U64(0x9BECCE62, 0x836AC577), U64(0x4EE367F9, 0x430AEC32),                /* ~= 10^-288 */
    U64(0xC2E801FB, 0x244576D5), U64(0x229C41F7, 0x93CDA73F),                /* ~= 10^-287 */
    U64(0xF3A20279, 0xED56D48A), U64(0x6B435275, 0x78C1110F),                /* ~= 10^-286 */
    U64(0x9845418C, 0x345644D6), U64(0x830A1389, 0x6B78AAA9),                /* ~= 10^-285 */
    U64(0xBE5691EF, 0x416BD60C), U64(0x23CC986B, 0xC656D553),                /* ~= 10^-284 */
    U64(0xEDEC366B, 0x11C6CB8F), U64(0x2CBFBE86, 0xB7EC8AA8),                /* ~= 10^-283 */
    U64(0x94B3A202, 0xEB1C3F39), U64(0x7BF7D714, 0x32F3D6A9),                /* ~= 10^-282 */
    U64(0xB9E08A83, 0xA5E34F07), U64(0xDAF5CCD9, 0x3FB0CC53),                /* ~= 10^-281 */
    U64(0xE858AD24, 0x8F5C22C9), U64(0xD1B3400F, 0x8F9CFF68),                /* ~= 10^-280 */
    U64(0x91376C36, 0xD99995BE), U64(0x23100809, 0xB9C21FA1),                /* ~= 10^-279 */
    U64(0xB5854744, 0x8FFFFB2D), U64(0xABD40A0C, 0x2832A78A),                /* ~= 10^-278 */
    U64(0xE2E69915, 0xB3FFF9F9), U64(0x16C90C8F, 0x323F516C),                /* ~= 10^-277 */
    U64(0x8DD01FAD, 0x907FFC3B), U64(0xAE3DA7D9, 0x7F6792E3),                /* ~= 10^-276 */
    U64(0xB1442798, 0xF49FFB4A), U64(0x99CD11CF, 0xDF41779C),                /* ~= 10^-275 */
    U64(0xDD95317F, 0x31C7FA1D), U64(0x40405643, 0xD711D583),                /* ~= 10^-274 */
    U64(0x8A7D3EEF, 0x7F1CFC52), U64(0x482835EA, 0x666B2572),                /* ~= 10^-273 */
    U64(0xAD1C8EAB, 0x5EE43B66), U64(0xDA324365, 0x0005EECF),                /* ~= 10^-272 */
    U64(0xD863B256, 0x369D4A40), U64(0x90BED43E, 0x40076A82),                /* ~= 10^-271 */
    U64(0x873E4F75, 0xE2224E68), U64(0x5A7744A6, 0xE804A291),                /* ~= 10^-270 */
    U64(0xA90DE353, 0x5AAAE202), U64(0x711515D0, 0xA205CB36),                /* ~= 10^-269 */
    U64(0xD3515C28, 0x31559A83), U64(0x0D5A5B44, 0xCA873E03),                /* ~= 10^-268 */
    U64(0x8412D999, 0x1ED58091), U64(0xE858790A, 0xFE9486C2),                /* ~= 10^-267 */
    U64(0xA5178FFF, 0x668AE0B6), U64(0x626E974D, 0xBE39A872),                /* ~= 10^-266 */
    U64(0xCE5D73FF, 0x402D98E3), U64(0xFB0A3D21, 0x2DC8128F),                /* ~= 10^-265 */
    U64(0x80FA687F, 0x881C7F8E), U64(0x7CE66634, 0xBC9D0B99),                /* ~= 10^-264 */
    U64(0xA139029F, 0x6A239F72), U64(0x1C1FFFC1, 0xEBC44E80),                /* ~= 10^-263 */
    U64(0xC9874347, 0x44AC874E), U64(0xA327FFB2, 0x66B56220),                /* ~= 10^-262 */
    U64(0xFBE91419, 0x15D7A922), U64(0x4BF1FF9F, 0x0062BAA8),                /* ~= 10^-261 */
    U64(0x9D71AC8F, 0xADA6C9B5), U64(0x6F773FC3, 0x603DB4A9),                /* ~= 10^-260 */
    U64(0xC4CE17B3, 0x99107C22), U64(0xCB550FB4, 0x384D21D3),                /* ~= 10^-259 */
    U64(0xF6019DA0, 0x7F549B2B), U64(0x7E2A53A1, 0x46606A48),                /* ~= 10^-258 */
    U64(0x99C10284, 0x4F94E0FB), U64(0x2EDA7444, 0xCBFC426D),                /* ~= 10^-257 */
    U64(0xC0314325, 0x637A1939), U64(0xFA911155, 0xFEFB5308),                /* ~= 10^-256 */
    U64(0xF03D93EE, 0xBC589F88), U64(0x793555AB, 0x7EBA27CA),                /* ~= 10^-255 */
    U64(0x96267C75, 0x35B763B5), U64(0x4BC1558B, 0x2F3458DE),                /* ~= 10^-254 */
    U64(0xBBB01B92, 0x83253CA2), U64(0x9EB1AAED, 0xFB016F16),                /* ~= 10^-253 */
    U64(0xEA9C2277, 0x23EE8BCB), U64(0x465E15A9, 0x79C1CADC),                /* ~= 10^-252 */
    U64(0x92A1958A, 0x7675175F), U64(0x0BFACD89, 0xEC191EC9),                /* ~= 10^-251 */
    U64(0xB749FAED, 0x14125D36), U64(0xCEF980EC, 0x671F667B),                /* ~= 10^-250 */
    U64(0xE51C79A8, 0x5916F484), U64(0x82B7E127, 0x80E7401A),                /* ~= 10^-249 */
    U64(0x8F31CC09, 0x37AE58D2), U64(0xD1B2ECB8, 0xB0908810),                /* ~= 10^-248 */
    U64(0xB2FE3F0B, 0x8599EF07), U64(0x861FA7E6, 0xDCB4AA15),                /* ~= 10^-247 */
    U64(0xDFBDCECE, 0x67006AC9), U64(0x67A791E0, 0x93E1D49A),                /* ~= 10^-246 */
    U64(0x8BD6A141, 0x006042BD), U64(0xE0C8BB2C, 0x5C6D24E0),                /* ~= 10^-245 */
    U64(0xAECC4991, 0x4078536D), U64(0x58FAE9F7, 0x73886E18),                /* ~= 10^-244 */
    U64(0xDA7F5BF5, 0x90966848), U64(0xAF39A475, 0x506A899E),                /* ~= 10^-243 */
    U64(0x888F9979, 0x7A5E012D), U64(0x6D8406C9, 0x52429603),                /* ~= 10^-242 */
    U64(0xAAB37FD7, 0xD8F58178), U64(0xC8E5087B, 0xA6D33B83),                /* ~= 10^-241 */
    U64(0xD5605FCD, 0xCF32E1D6), U64(0xFB1E4A9A, 0x90880A64),                /* ~= 10^-240 */
    U64(0x855C3BE0, 0xA17FCD26), U64(0x5CF2EEA0, 0x9A55067F),                /* ~= 10^-239 */
    U64(0xA6B34AD8, 0xC9DFC06F), U64(0xF42FAA48, 0xC0EA481E),                /* ~= 10^-238 */
    U64(0xD0601D8E, 0xFC57B08B), U64(0xF13B94DA, 0xF124DA26),                /* ~= 10^-237 */
    U64(0x823C1279, 0x5DB6CE57), U64(0x76C53D08, 0xD6B70858),                /* ~= 10^-236 */
    U64(0xA2CB1717, 0xB52481ED), U64(0x54768C4B, 0x0C64CA6E),                /* ~= 10^-235 */
    U64(0xCB7DDCDD, 0xA26DA268), U64(0xA9942F5D, 0xCF7DFD09),                /* ~= 10^-234 */
    U64(0xFE5D5415, 0x0B090B02), U64(0xD3F93B35, 0x435D7C4C),                /* ~= 10^-233 */
    U64(0x9EFA548D, 0x26E5A6E1), U64(0xC47BC501, 0x4A1A6DAF),                /* ~= 10^-232 */
    U64(0xC6B8E9B0, 0x709F109A), U64(0x359AB641, 0x9CA1091B),                /* ~= 10^-231 */
    U64(0xF867241C, 0x8CC6D4C0), U64(0xC30163D2, 0x03C94B62),                /* ~= 10^-230 */
    U64(0x9B407691, 0xD7FC44F8), U64(0x79E0DE63, 0x425DCF1D),                /* ~= 10^-229 */
    U64(0xC2109436, 0x4DFB5636), U64(0x985915FC, 0x12F542E4),                /* ~= 10^-228 */
    U64(0xF294B943, 0xE17A2BC4), U64(0x3E6F5B7B, 0x17B2939D),                /* ~= 10^-227 */
    U64(0x979CF3CA, 0x6CEC5B5A), U64(0xA705992C, 0xEECF9C42),                /* ~= 10^-226 */
    U64(0xBD8430BD, 0x08277231), U64(0x50C6FF78, 0x2A838353),                /* ~= 10^-225 */
    U64(0xECE53CEC, 0x4A314EBD), U64(0xA4F8BF56, 0x35246428),                /* ~= 10^-224 */
    U64(0x940F4613, 0xAE5ED136), U64(0x871B7795, 0xE136BE99),                /* ~= 10^-223 */
    U64(0xB9131798, 0x99F68584), U64(0x28E2557B, 0x59846E3F),                /* ~= 10^-222 */
    U64(0xE757DD7E, 0xC07426E5), U64(0x331AEADA, 0x2FE589CF),                /* ~= 10^-221 */
    U64(0x9096EA6F, 0x3848984F), U64(0x3FF0D2C8, 0x5DEF7621),                /* ~= 10^-220 */
    U64(0xB4BCA50B, 0x065ABE63), U64(0x0FED077A, 0x756B53A9),                /* ~= 10^-219 */
    U64(0xE1EBCE4D, 0xC7F16DFB), U64(0xD3E84959, 0x12C62894),                /* ~= 10^-218 */
    U64(0x8D3360F0, 0x9CF6E4BD), U64(0x64712DD7, 0xABBBD95C),                /* ~= 10^-217 */
    U64(0xB080392C, 0xC4349DEC), U64(0xBD8D794D, 0x96AACFB3),                /* ~= 10^-216 */
    U64(0xDCA04777, 0xF541C567), U64(0xECF0D7A0, 0xFC5583A0),                /* ~= 10^-215 */
    U64(0x89E42CAA, 0xF9491B60), U64(0xF41686C4, 0x9DB57244),                /* ~= 10^-214 */
    U64(0xAC5D37D5, 0xB79B6239), U64(0x311C2875, 0xC522CED5),                /* ~= 10^-213 */
    U64(0xD77485CB, 0x25823AC7), U64(0x7D633293, 0x366B828B),                /* ~= 10^-212 */
    U64(0x86A8D39E, 0xF77164BC), U64(0xAE5DFF9C, 0x02033197),                /* ~= 10^-211 */
    U64(0xA8530886, 0xB54DBDEB), U64(0xD9F57F83, 0x0283FDFC),                /* ~= 10^-210 */
    U64(0xD267CAA8, 0x62A12D66), U64(0xD072DF63, 0xC324FD7B),                /* ~= 10^-209 */
    U64(0x8380DEA9, 0x3DA4BC60), U64(0x4247CB9E, 0x59F71E6D),                /* ~= 10^-208 */
    U64(0xA4611653, 0x8D0DEB78), U64(0x52D9BE85, 0xF074E608),                /* ~= 10^-207 */
    U64(0xCD795BE8, 0x70516656), U64(0x67902E27, 0x6C921F8B),                /* ~= 10^-206 */
    U64(0x806BD971, 0x4632DFF6), U64(0x00BA1CD8, 0xA3DB53B6),                /* ~= 10^-205 */
    U64(0xA086CFCD, 0x97BF97F3), U64(0x80E8A40E, 0xCCD228A4),                /* ~= 10^-204 */
    U64(0xC8A883C0, 0xFDAF7DF0), U64(0x6122CD12, 0x8006B2CD),                /* ~= 10^-203 */
    U64(0xFAD2A4B1, 0x3D1B5D6C), U64(0x796B8057, 0x20085F81),                /* ~= 10^-202 */
    U64(0x9CC3A6EE, 0xC6311A63), U64(0xCBE33036, 0x74053BB0),                /* ~= 10^-201 */
    U64(0xC3F490AA, 0x77BD60FC), U64(0xBEDBFC44, 0x11068A9C),                /* ~= 10^-200 */
    U64(0xF4F1B4D5, 0x15ACB93B), U64(0xEE92FB55, 0x15482D44),                /* ~= 10^-199 */
    U64(0x99171105, 0x2D8BF3C5), U64(0x751BDD15, 0x2D4D1C4A),                /* ~= 10^-198 */
    U64(0xBF5CD546, 0x78EEF0B6), U64(0xD262D45A, 0x78A0635D),                /* ~= 10^-197 */
    U64(0xEF340A98, 0x172AACE4), U64(0x86FB8971, 0x16C87C34),                /* ~= 10^-196 */
    U64(0x9580869F, 0x0E7AAC0E), U64(0xD45D35E6, 0xAE3D4DA0),                /* ~= 10^-195 */
    U64(0xBAE0A846, 0xD2195712), U64(0x89748360, 0x59CCA109),                /* ~= 10^-194 */
    U64(0xE998D258, 0x869FACD7), U64(0x2BD1A438, 0x703FC94B),                /* ~= 10^-193 */
    U64(0x91FF8377, 0x5423CC06), U64(0x7B6306A3, 0x4627DDCF),                /* ~= 10^-192 */
    U64(0xB67F6455, 0x292CBF08), U64(0x1A3BC84C, 0x17B1D542),                /* ~= 10^-191 */
    U64(0xE41F3D6A, 0x7377EECA), U64(0x20CABA5F, 0x1D9E4A93),                /* ~= 10^-190 */
    U64(0x8E938662, 0x882AF53E), U64(0x547EB47B, 0x7282EE9C),                /* ~= 10^-189 */
    U64(0xB23867FB, 0x2A35B28D), U64(0xE99E619A, 0x4F23AA43),                /* ~= 10^-188 */
    U64(0xDEC681F9, 0xF4C31F31), U64(0x6405FA00, 0xE2EC94D4),                /* ~= 10^-187 */
    U64(0x8B3C113C, 0x38F9F37E), U64(0xDE83BC40, 0x8DD3DD04),                /* ~= 10^-186 */
    U64(0xAE0B158B, 0x4738705E), U64(0x9624AB50, 0xB148D445),                /* ~= 10^-185 */
    U64(0xD98DDAEE, 0x19068C76), U64(0x3BADD624, 0xDD9B0957),                /* ~= 10^-184 */
    U64(0x87F8A8D4, 0xCFA417C9), U64(0xE54CA5D7, 0x0A80E5D6),                /* ~= 10^-183 */
    U64(0xA9F6D30A, 0x038D1DBC), U64(0x5E9FCF4C, 0xCD211F4C),                /* ~= 10^-182 */
    U64(0xD47487CC, 0x8470652B), U64(0x7647C320, 0x0069671F),                /* ~= 10^-181 */
    U64(0x84C8D4DF, 0xD2C63F3B), U64(0x29ECD9F4, 0x0041E073),                /* ~= 10^-180 */
    U64(0xA5FB0A17, 0xC777CF09), U64(0xF4681071, 0x00525890),                /* ~= 10^-179 */
    U64(0xCF79CC9D, 0xB955C2CC), U64(0x7182148D, 0x4066EEB4),                /* ~= 10^-178 */
    U64(0x81AC1FE2, 0x93D599BF), U64(0xC6F14CD8, 0x48405530),                /* ~= 10^-177 */
    U64(0xA21727DB, 0x38CB002F), U64(0xB8ADA00E, 0x5A506A7C),                /* ~= 10^-176 */
    U64(0xCA9CF1D2, 0x06FDC03B), U64(0xA6D90811, 0xF0E4851C),                /* ~= 10^-175 */
    U64(0xFD442E46, 0x88BD304A), U64(0x908F4A16, 0x6D1DA663),                /* ~= 10^-174 */
    U64(0x9E4A9CEC, 0x15763E2E), U64(0x9A598E4E, 0x043287FE),                /* ~= 10^-173 */
    U64(0xC5DD4427, 0x1AD3CDBA), U64(0x40EFF1E1, 0x853F29FD),                /* ~= 10^-172 */
    U64(0xF7549530, 0xE188C128), U64(0xD12BEE59, 0xE68EF47C),                /* ~= 10^-171 */
    U64(0x9A94DD3E, 0x8CF578B9), U64(0x82BB74F8, 0x301958CE),                /* ~= 10^-170 */
    U64(0xC13A148E, 0x3032D6E7), U64(0xE36A5236, 0x3C1FAF01),                /* ~= 10^-169 */
    U64(0xF18899B1, 0xBC3F8CA1), U64(0xDC44E6C3, 0xCB279AC1),                /* ~= 10^-168 */
    U64(0x96F5600F, 0x15A7B7E5), U64(0x29AB103A, 0x5EF8C0B9),                /* ~= 10^-167 */
    U64(0xBCB2B812, 0xDB11A5DE), U64(0x7415D448, 0xF6B6F0E7),                /* ~= 10^-166 */
    U64(0xEBDF6617, 0x91D60F56), U64(0x111B495B, 0x3464AD21),                /* ~= 10^-165 */
    U64(0x936B9FCE, 0xBB25C995), U64(0xCAB10DD9, 0x00BEEC34),                /* ~= 10^-164 */
    U64(0xB84687C2, 0x69EF3BFB), U64(0x3D5D514F, 0x40EEA742),                /* ~= 10^-163 */
    U64(0xE65829B3, 0x046B0AFA), U64(0x0CB4A5A3, 0x112A5112),                /* ~= 10^-162 */
    U64(0x8FF71A0F, 0xE2C2E6DC), U64(0x47F0E785, 0xEABA72AB),                /* ~= 10^-161 */
    U64(0xB3F4E093, 0xDB73A093), U64(0x59ED2167, 0x65690F56),                /* ~= 10^-160 */
    U64(0xE0F218B8, 0xD25088B8), U64(0x306869C1, 0x3EC3532C),                /* ~= 10^-159 */
    U64(0x8C974F73, 0x83725573), U64(0x1E414218, 0xC73A13FB),                /* ~= 10^-158 */
    U64(0xAFBD2350, 0x644EEACF), U64(0xE5D1929E, 0xF90898FA),                /* ~= 10^-157 */
    U64(0xDBAC6C24, 0x7D62A583), U64(0xDF45F746, 0xB74ABF39),                /* ~= 10^-156 */
    U64(0x894BC396, 0xCE5DA772), U64(0x6B8BBA8C, 0x328EB783),                /* ~= 10^-155 */
    U64(0xAB9EB47C, 0x81F5114F), U64(0x066EA92F, 0x3F326564),                /* ~= 10^-154 */
    U64(0xD686619B, 0xA27255A2), U64(0xC80A537B, 0x0EFEFEBD),                /* ~= 10^-153 */
    U64(0x8613FD01, 0x45877585), U64(0xBD06742C, 0xE95F5F36),                /* ~= 10^-152 */
    U64(0xA798FC41, 0x96E952E7), U64(0x2C481138, 0x23B73704),                /* ~= 10^-151 */
    U64(0xD17F3B51, 0xFCA3A7A0), U64(0xF75A1586, 0x2CA504C5),                /* ~= 10^-150 */
    U64(0x82EF8513, 0x3DE648C4), U64(0x9A984D73, 0xDBE722FB),                /* ~= 10^-149 */
    U64(0xA3AB6658, 0x0D5FDAF5), U64(0xC13E60D0, 0xD2E0EBBA),                /* ~= 10^-148 */
    U64(0xCC963FEE, 0x10B7D1B3), U64(0x318DF905, 0x079926A8),                /* ~= 10^-147 */
    U64(0xFFBBCFE9, 0x94E5C61F), U64(0xFDF17746, 0x497F7052),                /* ~= 10^-146 */
    U64(0x9FD561F1, 0xFD0F9BD3), U64(0xFEB6EA8B, 0xEDEFA633),                /* ~= 10^-145 */
    U64(0xC7CABA6E, 0x7C5382C8), U64(0xFE64A52E, 0xE96B8FC0),                /* ~= 10^-144 */
    U64(0xF9BD690A, 0x1B68637B), U64(0x3DFDCE7A, 0xA3C673B0),                /* ~= 10^-143 */
    U64(0x9C1661A6, 0x51213E2D), U64(0x06BEA10C, 0xA65C084E),                /* ~= 10^-142 */
    U64(0xC31BFA0F, 0xE5698DB8), U64(0x486E494F, 0xCFF30A62),                /* ~= 10^-141 */
    U64(0xF3E2F893, 0xDEC3F126), U64(0x5A89DBA3, 0xC3EFCCFA),                /* ~= 10^-140 */
    U64(0x986DDB5C, 0x6B3A76B7), U64(0xF8962946, 0x5A75E01C),                /* ~= 10^-139 */
    U64(0xBE895233, 0x86091465), U64(0xF6BBB397, 0xF1135823),                /* ~= 10^-138 */
    U64(0xEE2BA6C0, 0x678B597F), U64(0x746AA07D, 0xED582E2C),                /* ~= 10^-137 */
    U64(0x94DB4838, 0x40B717EF), U64(0xA8C2A44E, 0xB4571CDC),                /* ~= 10^-136 */
    U64(0xBA121A46, 0x50E4DDEB), U64(0x92F34D62, 0x616CE413),                /* ~= 10^-135 */
    U64(0xE896A0D7, 0xE51E1566), U64(0x77B020BA, 0xF9C81D17),                /* ~= 10^-134 */
    U64(0x915E2486, 0xEF32CD60), U64(0x0ACE1474, 0xDC1D122E),                /* ~= 10^-133 */
    U64(0xB5B5ADA8, 0xAAFF80B8), U64(0x0D819992, 0x132456BA),                /* ~= 10^-132 */
    U64(0xE3231912, 0xD5BF60E6), U64(0x10E1FFF6, 0x97ED6C69),                /* ~= 10^-131 */
    U64(0x8DF5EFAB, 0xC5979C8F), U64(0xCA8D3FFA, 0x1EF463C1),                /* ~= 10^-130 */
    U64(0xB1736B96, 0xB6FD83B3), U64(0xBD308FF8, 0xA6B17CB2),                /* ~= 10^-129 */
    U64(0xDDD0467C, 0x64BCE4A0), U64(0xAC7CB3F6, 0xD05DDBDE),                /* ~= 10^-128 */
    U64(0x8AA22C0D, 0xBEF60EE4), U64(0x6BCDF07A, 0x423AA96B),                /* ~= 10^-127 */
    U64(0xAD4AB711, 0x2EB3929D), U64(0x86C16C98, 0xD2C953C6),                /* ~= 10^-126 */
    U64(0xD89D64D5, 0x7A607744), U64(0xE871C7BF, 0x077BA8B7),                /* ~= 10^-125 */
    U64(0x87625F05, 0x6C7C4A8B), U64(0x11471CD7, 0x64AD4972),                /* ~= 10^-124 */
    U64(0xA93AF6C6, 0xC79B5D2D), U64(0xD598E40D, 0x3DD89BCF),                /* ~= 10^-123 */
    U64(0xD389B478, 0x79823479), U64(0x4AFF1D10, 0x8D4EC2C3),                /* ~= 10^-122 */
    U64(0x843610CB, 0x4BF160CB), U64(0xCEDF722A, 0x585139BA),                /* ~= 10^-121 */
    U64(0xA54394FE, 0x1EEDB8FE), U64(0xC2974EB4, 0xEE658828),                /* ~= 10^-120 */
    U64(0xCE947A3D, 0xA6A9273E), U64(0x733D2262, 0x29FEEA32),                /* ~= 10^-119 */
    U64(0x811CCC66, 0x8829B887), U64(0x0806357D, 0x5A3F525F),                /* ~= 10^-118 */
    U64(0xA163FF80, 0x2A3426A8), U64(0xCA07C2DC, 0xB0CF26F7),                /* ~= 10^-117 */
    U64(0xC9BCFF60, 0x34C13052), U64(0xFC89B393, 0xDD02F0B5),                /* ~= 10^-116 */
    U64(0xFC2C3F38, 0x41F17C67), U64(0xBBAC2078, 0xD443ACE2),                /* ~= 10^-115 */
    U64(0x9D9BA783, 0x2936EDC0), U64(0xD54B944B, 0x84AA4C0D),                /* ~= 10^-114 */
    U64(0xC5029163, 0xF384A931), U64(0x0A9E795E, 0x65D4DF11),                /* ~= 10^-113 */
    U64(0xF64335BC, 0xF065D37D), U64(0x4D4617B5, 0xFF4A16D5),                /* ~= 10^-112 */
    U64(0x99EA0196, 0x163FA42E), U64(0x504BCED1, 0xBF8E4E45),                /* ~= 10^-111 */
    U64(0xC06481FB, 0x9BCF8D39), U64(0xE45EC286, 0x2F71E1D6),                /* ~= 10^-110 */
    U64(0xF07DA27A, 0x82C37088), U64(0x5D767327, 0xBB4E5A4C),                /* ~= 10^-109 */
    U64(0x964E858C, 0x91BA2655), U64(0x3A6A07F8, 0xD510F86F),                /* ~= 10^-108 */
    U64(0xBBE226EF, 0xB628AFEA), U64(0x890489F7, 0x0A55368B),                /* ~= 10^-107 */
    U64(0xEADAB0AB, 0xA3B2DBE5), U64(0x2B45AC74, 0xCCEA842E),                /* ~= 10^-106 */
    U64(0x92C8AE6B, 0x464FC96F), U64(0x3B0B8BC9, 0x0012929D),                /* ~= 10^-105 */
    U64(0xB77ADA06, 0x17E3BBCB), U64(0x09CE6EBB, 0x40173744),                /* ~= 10^-104 */
    U64(0xE5599087, 0x9DDCAABD), U64(0xCC420A6A, 0x101D0515),                /* ~= 10^-103 */
    U64(0x8F57FA54, 0xC2A9EAB6), U64(0x9FA94682, 0x4A12232D),                /* ~= 10^-102 */
    U64(0xB32DF8E9, 0xF3546564), U64(0x47939822, 0xDC96ABF9),                /* ~= 10^-101 */
    U64(0xDFF97724, 0x70297EBD), U64(0x59787E2B, 0x93BC56F7),                /* ~= 10^-100 */
    U64(0x8BFBEA76, 0xC619EF36), U64(0x57EB4EDB, 0x3C55B65A),                /* ~= 10^-99 */
    U64(0xAEFAE514, 0x77A06B03), U64(0xEDE62292, 0x0B6B23F1),                /* ~= 10^-98 */
    U64(0xDAB99E59, 0x958885C4), U64(0xE95FAB36, 0x8E45ECED),                /* ~= 10^-97 */
    U64(0x88B402F7, 0xFD75539B), U64(0x11DBCB02, 0x18EBB414),                /* ~= 10^-96 */
    U64(0xAAE103B5, 0xFCD2A881), U64(0xD652BDC2, 0x9F26A119),                /* ~= 10^-95 */
    U64(0xD59944A3, 0x7C0752A2), U64(0x4BE76D33, 0x46F0495F),                /* ~= 10^-94 */
    U64(0x857FCAE6, 0x2D8493A5), U64(0x6F70A440, 0x0C562DDB),                /* ~= 10^-93 */
    U64(0xA6DFBD9F, 0xB8E5B88E), U64(0xCB4CCD50, 0x0F6BB952),                /* ~= 10^-92 */
    U64(0xD097AD07, 0xA71F26B2), U64(0x7E2000A4, 0x1346A7A7),                /* ~= 10^-91 */
    U64(0x825ECC24, 0xC873782F), U64(0x8ED40066, 0x8C0C28C8),                /* ~= 10^-90 */
    U64(0xA2F67F2D, 0xFA90563B), U64(0x72890080, 0x2F0F32FA),                /* ~= 10^-89 */
    U64(0xCBB41EF9, 0x79346BCA), U64(0x4F2B40A0, 0x3AD2FFB9),                /* ~= 10^-88 */
    U64(0xFEA126B7, 0xD78186BC), U64(0xE2F610C8, 0x4987BFA8),                /* ~= 10^-87 */
    U64(0x9F24B832, 0xE6B0F436), U64(0x0DD9CA7D, 0x2DF4D7C9),                /* ~= 10^-86 */
    U64(0xC6EDE63F, 0xA05D3143), U64(0x91503D1C, 0x79720DBB),                /* ~= 10^-85 */
    U64(0xF8A95FCF, 0x88747D94), U64(0x75A44C63, 0x97CE912A),                /* ~= 10^-84 */
    U64(0x9B69DBE1, 0xB548CE7C), U64(0xC986AFBE, 0x3EE11ABA),                /* ~= 10^-83 */
    U64(0xC24452DA, 0x229B021B), U64(0xFBE85BAD, 0xCE996168),                /* ~= 10^-82 */
    U64(0xF2D56790, 0xAB41C2A2), U64(0xFAE27299, 0x423FB9C3),                /* ~= 10^-81 */
    U64(0x97C560BA, 0x6B0919A5), U64(0xDCCD879F, 0xC967D41A),                /* ~= 10^-80 */
    U64(0xBDB6B8E9, 0x05CB600F), U64(0x5400E987, 0xBBC1C920),                /* ~= 10^-79 */
    U64(0xED246723, 0x473E3813), U64(0x290123E9, 0xAAB23B68),                /* ~= 10^-78 */
    U64(0x9436C076, 0x0C86E30B), U64(0xF9A0B672, 0x0AAF6521),                /* ~= 10^-77 */
    U64(0xB9447093, 0x8FA89BCE), U64(0xF808E40E, 0x8D5B3E69),                /* ~= 10^-76 */
    U64(0xE7958CB8, 0x7392C2C2), U64(0xB60B1D12, 0x30B20E04),                /* ~= 10^-75 */
    U64(0x90BD77F3, 0x483BB9B9), U64(0xB1C6F22B, 0x5E6F48C2),                /* ~= 10^-74 */
    U64(0xB4ECD5F0, 0x1A4AA828), U64(0x1E38AEB6, 0x360B1AF3),                /* ~= 10^-73 */
    U64(0xE2280B6C, 0x20DD5232), U64(0x25C6DA63, 0xC38DE1B0),                /* ~= 10^-72 */
    U64(0x8D590723, 0x948A535F), U64(0x579C487E, 0x5A38AD0E),                /* ~= 10^-71 */
    U64(0xB0AF48EC, 0x79ACE837), U64(0x2D835A9D, 0xF0C6D851),                /* ~= 10^-70 */
    U64(0xDCDB1B27, 0x98182244), U64(0xF8E43145, 0x6CF88E65),                /* ~= 10^-69 */
    U64(0x8A08F0F8, 0xBF0F156B), U64(0x1B8E9ECB, 0x641B58FF),                /* ~= 10^-68 */
    U64(0xAC8B2D36, 0xEED2DAC5), U64(0xE272467E, 0x3D222F3F),                /* ~= 10^-67 */
    U64(0xD7ADF884, 0xAA879177), U64(0x5B0ED81D, 0xCC6ABB0F),                /* ~= 10^-66 */
    U64(0x86CCBB52, 0xEA94BAEA), U64(0x98E94712, 0x9FC2B4E9),                /* ~= 10^-65 */
    U64(0xA87FEA27, 0xA539E9A5), U64(0x3F2398D7, 0x47B36224),                /* ~= 10^-64 */
    U64(0xD29FE4B1, 0x8E88640E), U64(0x8EEC7F0D, 0x19A03AAD),                /* ~= 10^-63 */
    U64(0x83A3EEEE, 0xF9153E89), U64(0x1953CF68, 0x300424AC),                /* ~= 10^-62 */
    U64(0xA48CEAAA, 0xB75A8E2B), U64(0x5FA8C342, 0x3C052DD7),                /* ~= 10^-61 */
    U64(0xCDB02555, 0x653131B6), U64(0x3792F412, 0xCB06794D),                /* ~= 10^-60 */
    U64(0x808E1755, 0x5F3EBF11), U64(0xE2BBD88B, 0xBEE40BD0),                /* ~= 10^-59 */
    U64(0xA0B19D2A, 0xB70E6ED6), U64(0x5B6ACEAE, 0xAE9D0EC4),                /* ~= 10^-58 */
    U64(0xC8DE0475, 0x64D20A8B), U64(0xF245825A, 0x5A445275),                /* ~= 10^-57 */
    U64(0xFB158592, 0xBE068D2E), U64(0xEED6E2F0, 0xF0D56712),                /* ~= 10^-56 */
    U64(0x9CED737B, 0xB6C4183D), U64(0x55464DD6, 0x9685606B),                /* ~= 10^-55 */
    U64(0xC428D05A, 0xA4751E4C), U64(0xAA97E14C, 0x3C26B886),                /* ~= 10^-54 */
    U64(0xF5330471, 0x4D9265DF), U64(0xD53DD99F, 0x4B3066A8),                /* ~= 10^-53 */
    U64(0x993FE2C6, 0xD07B7FAB), U64(0xE546A803, 0x8EFE4029),                /* ~= 10^-52 */
    U64(0xBF8FDB78, 0x849A5F96), U64(0xDE985204, 0x72BDD033),                /* ~= 10^-51 */
    U64(0xEF73D256, 0xA5C0F77C), U64(0x963E6685, 0x8F6D4440),                /* ~= 10^-50 */
    U64(0x95A86376, 0x27989AAD), U64(0xDDE70013, 0x79A44AA8),                /* ~= 10^-49 */
    U64(0xBB127C53, 0xB17EC159), U64(0x5560C018, 0x580D5D52),                /* ~= 10^-48 */
    U64(0xE9D71B68, 0x9DDE71AF), U64(0xAAB8F01E, 0x6E10B4A6),                /* ~= 10^-47 */
    U64(0x92267121, 0x62AB070D), U64(0xCAB39613, 0x04CA70E8),                /* ~= 10^-46 */
    U64(0xB6B00D69, 0xBB55C8D1), U64(0x3D607B97, 0xC5FD0D22),                /* ~= 10^-45 */
    U64(0xE45C10C4, 0x2A2B3B05), U64(0x8CB89A7D, 0xB77C506A),                /* ~= 10^-44 */
    U64(0x8EB98A7A, 0x9A5B04E3), U64(0x77F3608E, 0x92ADB242),                /* ~= 10^-43 */
    U64(0xB267ED19, 0x40F1C61C), U64(0x55F038B2, 0x37591ED3),                /* ~= 10^-42 */
    U64(0xDF01E85F, 0x912E37A3), U64(0x6B6C46DE, 0xC52F6688),                /* ~= 10^-41 */
    U64(0x8B61313B, 0xBABCE2C6), U64(0x2323AC4B, 0x3B3DA015),                /* ~= 10^-40 */
    U64(0xAE397D8A, 0xA96C1B77), U64(0xABEC975E, 0x0A0D081A),                /* ~= 10^-39 */
    U64(0xD9C7DCED, 0x53C72255), U64(0x96E7BD35, 0x8C904A21),                /* ~= 10^-38 */
    U64(0x881CEA14, 0x545C7575), U64(0x7E50D641, 0x77DA2E54),                /* ~= 10^-37 */
    U64(0xAA242499, 0x697392D2), U64(0xDDE50BD1, 0xD5D0B9E9),                /* ~= 10^-36 */
    U64(0xD4AD2DBF, 0xC3D07787), U64(0x955E4EC6, 0x4B44E864),                /* ~= 10^-35 */
    U64(0x84EC3C97, 0xDA624AB4), U64(0xBD5AF13B, 0xEF0B113E),                /* ~= 10^-34 */
    U64(0xA6274BBD, 0xD0FADD61), U64(0xECB1AD8A, 0xEACDD58E),                /* ~= 10^-33 */
    U64(0xCFB11EAD, 0x453994BA), U64(0x67DE18ED, 0xA5814AF2),                /* ~= 10^-32 */
    U64(0x81CEB32C, 0x4B43FCF4), U64(0x80EACF94, 0x8770CED7),                /* ~= 10^-31 */
    U64(0xA2425FF7, 0x5E14FC31), U64(0xA1258379, 0xA94D028D),                /* ~= 10^-30 */
    U64(0xCAD2F7F5, 0x359A3B3E), U64(0x096EE458, 0x13A04330),                /* ~= 10^-29 */
    U64(0xFD87B5F2, 0x8300CA0D), U64(0x8BCA9D6E, 0x188853FC),                /* ~= 10^-28 */
    U64(0x9E74D1B7, 0x91E07E48), U64(0x775EA264, 0xCF55347D),                /* ~= 10^-27 */
    U64(0xC6120625, 0x76589DDA), U64(0x95364AFE, 0x032A819D),                /* ~= 10^-26 */
    U64(0xF79687AE, 0xD3EEC551), U64(0x3A83DDBD, 0x83F52204),                /* ~= 10^-25 */
    U64(0x9ABE14CD, 0x44753B52), U64(0xC4926A96, 0x72793542),                /* ~= 10^-24 */
    U64(0xC16D9A00, 0x95928A27), U64(0x75B7053C, 0x0F178293),                /* ~= 10^-23 */
    U64(0xF1C90080, 0xBAF72CB1), U64(0x5324C68B, 0x12DD6338),                /* ~= 10^-22 */
    U64(0x971DA050, 0x74DA7BEE), U64(0xD3F6FC16, 0xEBCA5E03),                /* ~= 10^-21 */
    U64(0xBCE50864, 0x92111AEA), U64(0x88F4BB1C, 0xA6BCF584),                /* ~= 10^-20 */
    U64(0xEC1E4A7D, 0xB69561A5), U64(0x2B31E9E3, 0xD06C32E5),                /* ~= 10^-19 */
    U64(0x9392EE8E, 0x921D5D07), U64(0x3AFF322E, 0x62439FCF),                /* ~= 10^-18 */
    U64(0xB877AA32, 0x36A4B449), U64(0x09BEFEB9, 0xFAD487C2),                /* ~= 10^-17 */
    U64(0xE69594BE, 0xC44DE15B), U64(0x4C2EBE68, 0x7989A9B3),                /* ~= 10^-16 */
    U64(0x901D7CF7, 0x3AB0ACD9), U64(0x0F9D3701, 0x4BF60A10),                /* ~= 10^-15 */
    U64(0xB424DC35, 0x095CD80F), U64(0x538484C1, 0x9EF38C94),                /* ~= 10^-14 */
    U64(0xE12E1342, 0x4BB40E13), U64(0x2865A5F2, 0x06B06FB9),                /* ~= 10^-13 */
    U64(0x8CBCCC09, 0x6F5088CB), U64(0xF93F87B7, 0x442E45D3),                /* ~= 10^-12 */
    U64(0xAFEBFF0B, 0xCB24AAFE), U64(0xF78F69A5, 0x1539D748),                /* ~= 10^-11 */
    U64(0xDBE6FECE, 0xBDEDD5BE), U64(0xB573440E, 0x5A884D1B),                /* ~= 10^-10 */
    U64(0x89705F41, 0x36B4A597), U64(0x31680A88, 0xF8953030),                /* ~= 10^-9 */
    U64(0xABCC7711, 0x8461CEFC), U64(0xFDC20D2B, 0x36BA7C3D),                /* ~= 10^-8 */
    U64(0xD6BF94D5, 0xE57A42BC), U64(0x3D329076, 0x04691B4C),                /* ~= 10^-7 */
    U64(0x8637BD05, 0xAF6C69B5), U64(0xA63F9A49, 0xC2C1B10F),                /* ~= 10^-6 */
    U64(0xA7C5AC47, 0x1B478423), U64(0x0FCF80DC, 0x33721D53),                /* ~= 10^-5 */
    U64(0xD1B71758, 0xE219652B), U64(0xD3C36113, 0x404EA4A8),                /* ~= 10^-4 */
    U64(0x83126E97, 0x8D4FDF3B), U64(0x645A1CAC, 0x083126E9),                /* ~= 10^-3 */
    U64(0xA3D70A3D, 0x70A3D70A), U64(0x3D70A3D7, 0x0A3D70A3),                /* ~= 10^-2 */
    U64(0xCCCCCCCC, 0xCCCCCCCC), U64(0xCCCCCCCC, 0xCCCCCCCC),                /* ~= 10^-1 */
    U64(0x80000000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^0 */  // no erro
    U64(0xA0000000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^1 */
    U64(0xC8000000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^2 */
    U64(0xFA000000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^3 */
    U64(0x9C400000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^4 */
    U64(0xC3500000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^5 */
    U64(0xF4240000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^6 */
    U64(0x98968000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^7 */
    U64(0xBEBC2000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^8 */
    U64(0xEE6B2800, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^9 */
    U64(0x9502F900, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^10 */
    U64(0xBA43B740, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^11 */
    U64(0xE8D4A510, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^12 */
    U64(0x9184E72A, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^13 */
    U64(0xB5E620F4, 0x80000000), U64(0x00000000, 0x00000000),                /* == 10^14 */
    U64(0xE35FA931, 0xA0000000), U64(0x00000000, 0x00000000),                /* == 10^15 */
    U64(0x8E1BC9BF, 0x04000000), U64(0x00000000, 0x00000000),                /* == 10^16 */
    U64(0xB1A2BC2E, 0xC5000000), U64(0x00000000, 0x00000000),                /* == 10^17 */
    U64(0xDE0B6B3A, 0x76400000), U64(0x00000000, 0x00000000),                /* == 10^18 */
    U64(0x8AC72304, 0x89E80000), U64(0x00000000, 0x00000000),                /* == 10^19 */
    U64(0xAD78EBC5, 0xAC620000), U64(0x00000000, 0x00000000),                /* == 10^20 */
    U64(0xD8D726B7, 0x177A8000), U64(0x00000000, 0x00000000),                /* == 10^21 */
    U64(0x87867832, 0x6EAC9000), U64(0x00000000, 0x00000000),                /* == 10^22 */
    U64(0xA968163F, 0x0A57B400), U64(0x00000000, 0x00000000),                /* == 10^23 */
    U64(0xD3C21BCE, 0xCCEDA100), U64(0x00000000, 0x00000000),                /* == 10^24 */
    U64(0x84595161, 0x401484A0), U64(0x00000000, 0x00000000),                /* == 10^25 */
    U64(0xA56FA5B9, 0x9019A5C8), U64(0x00000000, 0x00000000),                /* == 10^26 */
    U64(0xCECB8F27, 0xF4200F3A), U64(0x00000000, 0x00000000),                /* == 10^27 */
    U64(0x813F3978, 0xF8940984), U64(0x40000000, 0x00000000),                /* == 10^28 */
    U64(0xA18F07D7, 0x36B90BE5), U64(0x50000000, 0x00000000),                /* == 10^29 */
    U64(0xC9F2C9CD, 0x04674EDE), U64(0xA4000000, 0x00000000),                /* == 10^30 */
    U64(0xFC6F7C40, 0x45812296), U64(0x4D000000, 0x00000000),                /* == 10^31 */
    U64(0x9DC5ADA8, 0x2B70B59D), U64(0xF0200000, 0x00000000),                /* == 10^32 */
    U64(0xC5371912, 0x364CE305), U64(0x6C280000, 0x00000000),                /* == 10^33 */
    U64(0xF684DF56, 0xC3E01BC6), U64(0xC7320000, 0x00000000),                /* == 10^34 */
    U64(0x9A130B96, 0x3A6C115C), U64(0x3C7F4000, 0x00000000),                /* == 10^35 */
    U64(0xC097CE7B, 0xC90715B3), U64(0x4B9F1000, 0x00000000),                /* == 10^36 */
    U64(0xF0BDC21A, 0xBB48DB20), U64(0x1E86D400, 0x00000000),                /* == 10^37 */
    U64(0x96769950, 0xB50D88F4), U64(0x13144480, 0x00000000),                /* == 10^38 */
    U64(0xBC143FA4, 0xE250EB31), U64(0x17D955A0, 0x00000000),                /* == 10^39 */
    U64(0xEB194F8E, 0x1AE525FD), U64(0x5DCFAB08, 0x00000000),                /* == 10^40 */
    U64(0x92EFD1B8, 0xD0CF37BE), U64(0x5AA1CAE5, 0x00000000),                /* == 10^41 */
    U64(0xB7ABC627, 0x050305AD), U64(0xF14A3D9E, 0x40000000),                /* == 10^42 */
    U64(0xE596B7B0, 0xC643C719), U64(0x6D9CCD05, 0xD0000000),                /* == 10^43 */
    U64(0x8F7E32CE, 0x7BEA5C6F), U64(0xE4820023, 0xA2000000),                /* == 10^44 */
    U64(0xB35DBF82, 0x1AE4F38B), U64(0xDDA2802C, 0x8A800000),                /* == 10^45 */
    U64(0xE0352F62, 0xA19E306E), U64(0xD50B2037, 0xAD200000),                /* == 10^46 */
    U64(0x8C213D9D, 0xA502DE45), U64(0x4526F422, 0xCC340000),                /* == 10^47 */
    U64(0xAF298D05, 0x0E4395D6), U64(0x9670B12B, 0x7F410000),                /* == 10^48 */
    U64(0xDAF3F046, 0x51D47B4C), U64(0x3C0CDD76, 0x5F114000),                /* == 10^49 */
    U64(0x88D8762B, 0xF324CD0F), U64(0xA5880A69, 0xFB6AC800),                /* == 10^50 */
    U64(0xAB0E93B6, 0xEFEE0053), U64(0x8EEA0D04, 0x7A457A00),                /* == 10^51 */
    U64(0xD5D238A4, 0xABE98068), U64(0x72A49045, 0x98D6D880),                /* == 10^52 */
    U64(0x85A36366, 0xEB71F041), U64(0x47A6DA2B, 0x7F864750),                /* == 10^53 */
    U64(0xA70C3C40, 0xA64E6C51), U64(0x999090B6, 0x5F67D924),                /* == 10^54 */
    U64(0xD0CF4B50, 0xCFE20765), U64(0xFFF4B4E3, 0xF741CF6D), /* == 10^55 */ // no error
    U64(0x82818F12, 0x81ED449F), U64(0xBFF8F10E, 0x7A8921A4),                /* ~= 10^56 */
    U64(0xA321F2D7, 0x226895C7), U64(0xAFF72D52, 0x192B6A0D),                /* ~= 10^57 */
    U64(0xCBEA6F8C, 0xEB02BB39), U64(0x9BF4F8A6, 0x9F764490),                /* ~= 10^58 */
    U64(0xFEE50B70, 0x25C36A08), U64(0x02F236D0, 0x4753D5B4),                /* ~= 10^59 */
    U64(0x9F4F2726, 0x179A2245), U64(0x01D76242, 0x2C946590),                /* ~= 10^60 */
    U64(0xC722F0EF, 0x9D80AAD6), U64(0x424D3AD2, 0xB7B97EF5),                /* ~= 10^61 */
    U64(0xF8EBAD2B, 0x84E0D58B), U64(0xD2E08987, 0x65A7DEB2),                /* ~= 10^62 */
    U64(0x9B934C3B, 0x330C8577), U64(0x63CC55F4, 0x9F88EB2F),                /* ~= 10^63 */
    U64(0xC2781F49, 0xFFCFA6D5), U64(0x3CBF6B71, 0xC76B25FB),                /* ~= 10^64 */
    U64(0xF316271C, 0x7FC3908A), U64(0x8BEF464E, 0x3945EF7A),                /* ~= 10^65 */
    U64(0x97EDD871, 0xCFDA3A56), U64(0x97758BF0, 0xE3CBB5AC),                /* ~= 10^66 */
    U64(0xBDE94E8E, 0x43D0C8EC), U64(0x3D52EEED, 0x1CBEA317),                /* ~= 10^67 */
    U64(0xED63A231, 0xD4C4FB27), U64(0x4CA7AAA8, 0x63EE4BDD),                /* ~= 10^68 */
    U64(0x945E455F, 0x24FB1CF8), U64(0x8FE8CAA9, 0x3E74EF6A),                /* ~= 10^69 */
    U64(0xB975D6B6, 0xEE39E436), U64(0xB3E2FD53, 0x8E122B44),                /* ~= 10^70 */
    U64(0xE7D34C64, 0xA9C85D44), U64(0x60DBBCA8, 0x7196B616),                /* ~= 10^71 */
    U64(0x90E40FBE, 0xEA1D3A4A), U64(0xBC8955E9, 0x46FE31CD),                /* ~= 10^72 */
    U64(0xB51D13AE, 0xA4A488DD), U64(0x6BABAB63, 0x98BDBE41),                /* ~= 10^73 */
    U64(0xE264589A, 0x4DCDAB14), U64(0xC696963C, 0x7EED2DD1),                /* ~= 10^74 */
    U64(0x8D7EB760, 0x70A08AEC), U64(0xFC1E1DE5, 0xCF543CA2),                /* ~= 10^75 */
    U64(0xB0DE6538, 0x8CC8ADA8), U64(0x3B25A55F, 0x43294BCB),                /* ~= 10^76 */
    U64(0xDD15FE86, 0xAFFAD912), U64(0x49EF0EB7, 0x13F39EBE),                /* ~= 10^77 */
    U64(0x8A2DBF14, 0x2DFCC7AB), U64(0x6E356932, 0x6C784337),                /* ~= 10^78 */
    U64(0xACB92ED9, 0x397BF996), U64(0x49C2C37F, 0x07965404),                /* ~= 10^79 */
    U64(0xD7E77A8F, 0x87DAF7FB), U64(0xDC33745E, 0xC97BE906),                /* ~= 10^80 */
    U64(0x86F0AC99, 0xB4E8DAFD), U64(0x69A028BB, 0x3DED71A3),                /* ~= 10^81 */
    U64(0xA8ACD7C0, 0x222311BC), U64(0xC40832EA, 0x0D68CE0C),                /* ~= 10^82 */
    U64(0xD2D80DB0, 0x2AABD62B), U64(0xF50A3FA4, 0x90C30190),                /* ~= 10^83 */
    U64(0x83C7088E, 0x1AAB65DB), U64(0x792667C6, 0xDA79E0FA),                /* ~= 10^84 */
    U64(0xA4B8CAB1, 0xA1563F52), U64(0x577001B8, 0x91185938),                /* ~= 10^85 */
    U64(0xCDE6FD5E, 0x09ABCF26), U64(0xED4C0226, 0xB55E6F86),                /* ~= 10^86 */
    U64(0x80B05E5A, 0xC60B6178), U64(0x544F8158, 0x315B05B4),                /* ~= 10^87 */
    U64(0xA0DC75F1, 0x778E39D6), U64(0x696361AE, 0x3DB1C721),                /* ~= 10^88 */
    U64(0xC913936D, 0xD571C84C), U64(0x03BC3A19, 0xCD1E38E9),                /* ~= 10^89 */
    U64(0xFB587849, 0x4ACE3A5F), U64(0x04AB48A0, 0x4065C723),                /* ~= 10^90 */
    U64(0x9D174B2D, 0xCEC0E47B), U64(0x62EB0D64, 0x283F9C76),                /* ~= 10^91 */
    U64(0xC45D1DF9, 0x42711D9A), U64(0x3BA5D0BD, 0x324F8394),                /* ~= 10^92 */
    U64(0xF5746577, 0x930D6500), U64(0xCA8F44EC, 0x7EE36479),                /* ~= 10^93 */
    U64(0x9968BF6A, 0xBBE85F20), U64(0x7E998B13, 0xCF4E1ECB),                /* ~= 10^94 */
    U64(0xBFC2EF45, 0x6AE276E8), U64(0x9E3FEDD8, 0xC321A67E),                /* ~= 10^95 */
    U64(0xEFB3AB16, 0xC59B14A2), U64(0xC5CFE94E, 0xF3EA101E),                /* ~= 10^96 */
    U64(0x95D04AEE, 0x3B80ECE5), U64(0xBBA1F1D1, 0x58724A12),                /* ~= 10^97 */
    U64(0xBB445DA9, 0xCA61281F), U64(0x2A8A6E45, 0xAE8EDC97),                /* ~= 10^98 */
    U64(0xEA157514, 0x3CF97226), U64(0xF52D09D7, 0x1A3293BD),                /* ~= 10^99 */
    U64(0x924D692C, 0xA61BE758), U64(0x593C2626, 0x705F9C56),                /* ~= 10^100 */
    U64(0xB6E0C377, 0xCFA2E12E), U64(0x6F8B2FB0, 0x0C77836C),                /* ~= 10^101 */
    U64(0xE498F455, 0xC38B997A), U64(0x0B6DFB9C, 0x0F956447),                /* ~= 10^102 */
    U64(0x8EDF98B5, 0x9A373FEC), U64(0x4724BD41, 0x89BD5EAC),                /* ~= 10^103 */
    U64(0xB2977EE3, 0x00C50FE7), U64(0x58EDEC91, 0xEC2CB657),                /* ~= 10^104 */
    U64(0xDF3D5E9B, 0xC0F653E1), U64(0x2F2967B6, 0x6737E3ED),                /* ~= 10^105 */
    U64(0x8B865B21, 0x5899F46C), U64(0xBD79E0D2, 0x0082EE74),                /* ~= 10^106 */
    U64(0xAE67F1E9, 0xAEC07187), U64(0xECD85906, 0x80A3AA11),                /* ~= 10^107 */
    U64(0xDA01EE64, 0x1A708DE9), U64(0xE80E6F48, 0x20CC9495),                /* ~= 10^108 */
    U64(0x884134FE, 0x908658B2), U64(0x3109058D, 0x147FDCDD),                /* ~= 10^109 */
    U64(0xAA51823E, 0x34A7EEDE), U64(0xBD4B46F0, 0x599FD415),                /* ~= 10^110 */
    U64(0xD4E5E2CD, 0xC1D1EA96), U64(0x6C9E18AC, 0x7007C91A),                /* ~= 10^111 */
    U64(0x850FADC0, 0x9923329E), U64(0x03E2CF6B, 0xC604DDB0),                /* ~= 10^112 */
    U64(0xA6539930, 0xBF6BFF45), U64(0x84DB8346, 0xB786151C),                /* ~= 10^113 */
    U64(0xCFE87F7C, 0xEF46FF16), U64(0xE6126418, 0x65679A63),                /* ~= 10^114 */
    U64(0x81F14FAE, 0x158C5F6E), U64(0x4FCB7E8F, 0x3F60C07E),                /* ~= 10^115 */
    U64(0xA26DA399, 0x9AEF7749), U64(0xE3BE5E33, 0x0F38F09D),                /* ~= 10^116 */
    U64(0xCB090C80, 0x01AB551C), U64(0x5CADF5BF, 0xD3072CC5),                /* ~= 10^117 */
    U64(0xFDCB4FA0, 0x02162A63), U64(0x73D9732F, 0xC7C8F7F6),                /* ~= 10^118 */
    U64(0x9E9F11C4, 0x014DDA7E), U64(0x2867E7FD, 0xDCDD9AFA),                /* ~= 10^119 */
    U64(0xC646D635, 0x01A1511D), U64(0xB281E1FD, 0x541501B8),                /* ~= 10^120 */
    U64(0xF7D88BC2, 0x4209A565), U64(0x1F225A7C, 0xA91A4226),                /* ~= 10^121 */
    U64(0x9AE75759, 0x6946075F), U64(0x3375788D, 0xE9B06958),                /* ~= 10^122 */
    U64(0xC1A12D2F, 0xC3978937), U64(0x0052D6B1, 0x641C83AE),                /* ~= 10^123 */
    U64(0xF209787B, 0xB47D6B84), U64(0xC0678C5D, 0xBD23A49A),                /* ~= 10^124 */
    U64(0x9745EB4D, 0x50CE6332), U64(0xF840B7BA, 0x963646E0),                /* ~= 10^125 */
    U64(0xBD176620, 0xA501FBFF), U64(0xB650E5A9, 0x3BC3D898),                /* ~= 10^126 */
    U64(0xEC5D3FA8, 0xCE427AFF), U64(0xA3E51F13, 0x8AB4CEBE),                /* ~= 10^127 */
    U64(0x93BA47C9, 0x80E98CDF), U64(0xC66F336C, 0x36B10137),                /* ~= 10^128 */
    U64(0xB8A8D9BB, 0xE123F017), U64(0xB80B0047, 0x445D4184),                /* ~= 10^129 */
    U64(0xE6D3102A, 0xD96CEC1D), U64(0xA60DC059, 0x157491E5),                /* ~= 10^130 */
    U64(0x9043EA1A, 0xC7E41392), U64(0x87C89837, 0xAD68DB2F),                /* ~= 10^131 */
    U64(0xB454E4A1, 0x79DD1877), U64(0x29BABE45, 0x98C311FB),                /* ~= 10^132 */
    U64(0xE16A1DC9, 0xD8545E94), U64(0xF4296DD6, 0xFEF3D67A),                /* ~= 10^133 */
    U64(0x8CE2529E, 0x2734BB1D), U64(0x1899E4A6, 0x5F58660C),                /* ~= 10^134 */
    U64(0xB01AE745, 0xB101E9E4), U64(0x5EC05DCF, 0xF72E7F8F),                /* ~= 10^135 */
    U64(0xDC21A117, 0x1D42645D), U64(0x76707543, 0xF4FA1F73),                /* ~= 10^136 */
    U64(0x899504AE, 0x72497EBA), U64(0x6A06494A, 0x791C53A8),                /* ~= 10^137 */
    U64(0xABFA45DA, 0x0EDBDE69), U64(0x0487DB9D, 0x17636892),                /* ~= 10^138 */
    U64(0xD6F8D750, 0x9292D603), U64(0x45A9D284, 0x5D3C42B6),                /* ~= 10^139 */
    U64(0x865B8692, 0x5B9BC5C2), U64(0x0B8A2392, 0xBA45A9B2),                /* ~= 10^140 */
    U64(0xA7F26836, 0xF282B732), U64(0x8E6CAC77, 0x68D7141E),                /* ~= 10^141 */
    U64(0xD1EF0244, 0xAF2364FF), U64(0x3207D795, 0x430CD926),                /* ~= 10^142 */
    U64(0x8335616A, 0xED761F1F), U64(0x7F44E6BD, 0x49E807B8),                /* ~= 10^143 */
    U64(0xA402B9C5, 0xA8D3A6E7), U64(0x5F16206C, 0x9C6209A6),                /* ~= 10^144 */
    U64(0xCD036837, 0x130890A1), U64(0x36DBA887, 0xC37A8C0F),                /* ~= 10^145 */
    U64(0x80222122, 0x6BE55A64), U64(0xC2494954, 0xDA2C9789),                /* ~= 10^146 */
    U64(0xA02AA96B, 0x06DEB0FD), U64(0xF2DB9BAA, 0x10B7BD6C),                /* ~= 10^147 */
    U64(0xC83553C5, 0xC8965D3D), U64(0x6F928294, 0x94E5ACC7),                /* ~= 10^148 */
    U64(0xFA42A8B7, 0x3ABBF48C), U64(0xCB772339, 0xBA1F17F9),                /* ~= 10^149 */
    U64(0x9C69A972, 0x84B578D7), U64(0xFF2A7604, 0x14536EFB),                /* ~= 10^150 */
    U64(0xC38413CF, 0x25E2D70D), U64(0xFEF51385, 0x19684ABA),                /* ~= 10^151 */
    U64(0xF46518C2, 0xEF5B8CD1), U64(0x7EB25866, 0x5FC25D69),                /* ~= 10^152 */
    U64(0x98BF2F79, 0xD5993802), U64(0xEF2F773F, 0xFBD97A61),                /* ~= 10^153 */
    U64(0xBEEEFB58, 0x4AFF8603), U64(0xAAFB550F, 0xFACFD8FA),                /* ~= 10^154 */
    U64(0xEEAABA2E, 0x5DBF6784), U64(0x95BA2A53, 0xF983CF38),                /* ~= 10^155 */
    U64(0x952AB45C, 0xFA97A0B2), U64(0xDD945A74, 0x7BF26183),                /* ~= 10^156 */
    U64(0xBA756174, 0x393D88DF), U64(0x94F97111, 0x9AEEF9E4),                /* ~= 10^157 */
    U64(0xE912B9D1, 0x478CEB17), U64(0x7A37CD56, 0x01AAB85D),                /* ~= 10^158 */
    U64(0x91ABB422, 0xCCB812EE), U64(0xAC62E055, 0xC10AB33A),                /* ~= 10^159 */
    U64(0xB616A12B, 0x7FE617AA), U64(0x577B986B, 0x314D6009),                /* ~= 10^160 */
    U64(0xE39C4976, 0x5FDF9D94), U64(0xED5A7E85, 0xFDA0B80B),                /* ~= 10^161 */
    U64(0x8E41ADE9, 0xFBEBC27D), U64(0x14588F13, 0xBE847307),                /* ~= 10^162 */
    U64(0xB1D21964, 0x7AE6B31C), U64(0x596EB2D8, 0xAE258FC8),                /* ~= 10^163 */
    U64(0xDE469FBD, 0x99A05FE3), U64(0x6FCA5F8E, 0xD9AEF3BB),                /* ~= 10^164 */
    U64(0x8AEC23D6, 0x80043BEE), U64(0x25DE7BB9, 0x480D5854),                /* ~= 10^165 */
    U64(0xADA72CCC, 0x20054AE9), U64(0xAF561AA7, 0x9A10AE6A),                /* ~= 10^166 */
    U64(0xD910F7FF, 0x28069DA4), U64(0x1B2BA151, 0x8094DA04),                /* ~= 10^167 */
    U64(0x87AA9AFF, 0x79042286), U64(0x90FB44D2, 0xF05D0842),                /* ~= 10^168 */
    U64(0xA99541BF, 0x57452B28), U64(0x353A1607, 0xAC744A53),                /* ~= 10^169 */
    U64(0xD3FA922F, 0x2D1675F2), U64(0x42889B89, 0x97915CE8),                /* ~= 10^170 */
    U64(0x847C9B5D, 0x7C2E09B7), U64(0x69956135, 0xFEBADA11),                /* ~= 10^171 */
    U64(0xA59BC234, 0xDB398C25), U64(0x43FAB983, 0x7E699095),                /* ~= 10^172 */
    U64(0xCF02B2C2, 0x1207EF2E), U64(0x94F967E4, 0x5E03F4BB),                /* ~= 10^173 */
    U64(0x8161AFB9, 0x4B44F57D), U64(0x1D1BE0EE, 0xBAC278F5),                /* ~= 10^174 */
    U64(0xA1BA1BA7, 0x9E1632DC), U64(0x6462D92A, 0x69731732),                /* ~= 10^175 */
    U64(0xCA28A291, 0x859BBF93), U64(0x7D7B8F75, 0x03CFDCFE),                /* ~= 10^176 */
    U64(0xFCB2CB35, 0xE702AF78), U64(0x5CDA7352, 0x44C3D43E),                /* ~= 10^177 */
    U64(0x9DEFBF01, 0xB061ADAB), U64(0x3A088813, 0x6AFA64A7),                /* ~= 10^178 */
    U64(0xC56BAEC2, 0x1C7A1916), U64(0x088AAA18, 0x45B8FDD0),                /* ~= 10^179 */
    U64(0xF6C69A72, 0xA3989F5B), U64(0x8AAD549E, 0x57273D45),                /* ~= 10^180 */
    U64(0x9A3C2087, 0xA63F6399), U64(0x36AC54E2, 0xF678864B),                /* ~= 10^181 */
    U64(0xC0CB28A9, 0x8FCF3C7F), U64(0x84576A1B, 0xB416A7DD),                /* ~= 10^182 */
    U64(0xF0FDF2D3, 0xF3C30B9F), U64(0x656D44A2, 0xA11C51D5),                /* ~= 10^183 */
    U64(0x969EB7C4, 0x7859E743), U64(0x9F644AE5, 0xA4B1B325),                /* ~= 10^184 */
    U64(0xBC4665B5, 0x96706114), U64(0x873D5D9F, 0x0DDE1FEE),                /* ~= 10^185 */
    U64(0xEB57FF22, 0xFC0C7959), U64(0xA90CB506, 0xD155A7EA),                /* ~= 10^186 */
    U64(0x9316FF75, 0xDD87CBD8), U64(0x09A7F124, 0x42D588F2),                /* ~= 10^187 */
    U64(0xB7DCBF53, 0x54E9BECE), U64(0x0C11ED6D, 0x538AEB2F),                /* ~= 10^188 */
    U64(0xE5D3EF28, 0x2A242E81), U64(0x8F1668C8, 0xA86DA5FA),                /* ~= 10^189 */
    U64(0x8FA47579, 0x1A569D10), U64(0xF96E017D, 0x694487BC),                /* ~= 10^190 */
    U64(0xB38D92D7, 0x60EC4455), U64(0x37C981DC, 0xC395A9AC),                /* ~= 10^191 */
    U64(0xE070F78D, 0x3927556A), U64(0x85BBE253, 0xF47B1417),                /* ~= 10^192 */
    U64(0x8C469AB8, 0x43B89562), U64(0x93956D74, 0x78CCEC8E),                /* ~= 10^193 */
    U64(0xAF584166, 0x54A6BABB), U64(0x387AC8D1, 0x970027B2),                /* ~= 10^194 */
    U64(0xDB2E51BF, 0xE9D0696A), U64(0x06997B05, 0xFCC0319E),                /* ~= 10^195 */
    U64(0x88FCF317, 0xF22241E2), U64(0x441FECE3, 0xBDF81F03),                /* ~= 10^196 */
    U64(0xAB3C2FDD, 0xEEAAD25A), U64(0xD527E81C, 0xAD7626C3),                /* ~= 10^197 */
    U64(0xD60B3BD5, 0x6A5586F1), U64(0x8A71E223, 0xD8D3B074),                /* ~= 10^198 */
    U64(0x85C70565, 0x62757456), U64(0xF6872D56, 0x67844E49),                /* ~= 10^199 */
    U64(0xA738C6BE, 0xBB12D16C), U64(0xB428F8AC, 0x016561DB),                /* ~= 10^200 */
    U64(0xD106F86E, 0x69D785C7), U64(0xE13336D7, 0x01BEBA52),                /* ~= 10^201 */
    U64(0x82A45B45, 0x0226B39C), U64(0xECC00246, 0x61173473),                /* ~= 10^202 */
    U64(0xA34D7216, 0x42B06084), U64(0x27F002D7, 0xF95D0190),                /* ~= 10^203 */
    U64(0xCC20CE9B, 0xD35C78A5), U64(0x31EC038D, 0xF7B441F4),                /* ~= 10^204 */
    U64(0xFF290242, 0xC83396CE), U64(0x7E670471, 0x75A15271),                /* ~= 10^205 */
    U64(0x9F79A169, 0xBD203E41), U64(0x0F0062C6, 0xE984D386),                /* ~= 10^206 */
    U64(0xC75809C4, 0x2C684DD1), U64(0x52C07B78, 0xA3E60868),                /* ~= 10^207 */
    U64(0xF92E0C35, 0x37826145), U64(0xA7709A56, 0xCCDF8A82),                /* ~= 10^208 */
    U64(0x9BBCC7A1, 0x42B17CCB), U64(0x88A66076, 0x400BB691),                /* ~= 10^209 */
    U64(0xC2ABF989, 0x935DDBFE), U64(0x6ACFF893, 0xD00EA435),                /* ~= 10^210 */
    U64(0xF356F7EB, 0xF83552FE), U64(0x0583F6B8, 0xC4124D43),                /* ~= 10^211 */
    U64(0x98165AF3, 0x7B2153DE), U64(0xC3727A33, 0x7A8B704A),                /* ~= 10^212 */
    U64(0xBE1BF1B0, 0x59E9A8D6), U64(0x744F18C0, 0x592E4C5C),                /* ~= 10^213 */
    U64(0xEDA2EE1C, 0x7064130C), U64(0x1162DEF0, 0x6F79DF73),                /* ~= 10^214 */
    U64(0x9485D4D1, 0xC63E8BE7), U64(0x8ADDCB56, 0x45AC2BA8),                /* ~= 10^215 */
    U64(0xB9A74A06, 0x37CE2EE1), U64(0x6D953E2B, 0xD7173692),                /* ~= 10^216 */
    U64(0xE8111C87, 0xC5C1BA99), U64(0xC8FA8DB6, 0xCCDD0437),                /* ~= 10^217 */
    U64(0x910AB1D4, 0xDB9914A0), U64(0x1D9C9892, 0x400A22A2),                /* ~= 10^218 */
    U64(0xB54D5E4A, 0x127F59C8), U64(0x2503BEB6, 0xD00CAB4B),                /* ~= 10^219 */
    U64(0xE2A0B5DC, 0x971F303A), U64(0x2E44AE64, 0x840FD61D),                /* ~= 10^220 */
    U64(0x8DA471A9, 0xDE737E24), U64(0x5CEAECFE, 0xD289E5D2),                /* ~= 10^221 */
    U64(0xB10D8E14, 0x56105DAD), U64(0x7425A83E, 0x872C5F47),                /* ~= 10^222 */
    U64(0xDD50F199, 0x6B947518), U64(0xD12F124E, 0x28F77719),                /* ~= 10^223 */
    U64(0x8A5296FF, 0xE33CC92F), U64(0x82BD6B70, 0xD99AAA6F),                /* ~= 10^224 */
    U64(0xACE73CBF, 0xDC0BFB7B), U64(0x636CC64D, 0x1001550B),                /* ~= 10^225 */
    U64(0xD8210BEF, 0xD30EFA5A), U64(0x3C47F7E0, 0x5401AA4E),                /* ~= 10^226 */
    U64(0x8714A775, 0xE3E95C78), U64(0x65ACFAEC, 0x34810A71),                /* ~= 10^227 */
    U64(0xA8D9D153, 0x5CE3B396), U64(0x7F1839A7, 0x41A14D0D),                /* ~= 10^228 */
    U64(0xD31045A8, 0x341CA07C), U64(0x1EDE4811, 0x1209A050),                /* ~= 10^229 */
    U64(0x83EA2B89, 0x2091E44D), U64(0x934AED0A, 0xAB460432),                /* ~= 10^230 */
    U64(0xA4E4B66B, 0x68B65D60), U64(0xF81DA84D, 0x5617853F),                /* ~= 10^231 */
    U64(0xCE1DE406, 0x42E3F4B9), U64(0x36251260, 0xAB9D668E),                /* ~= 10^232 */
    U64(0x80D2AE83, 0xE9CE78F3), U64(0xC1D72B7C, 0x6B426019),                /* ~= 10^233 */
    U64(0xA1075A24, 0xE4421730), U64(0xB24CF65B, 0x8612F81F),                /* ~= 10^234 */
    U64(0xC94930AE, 0x1D529CFC), U64(0xDEE033F2, 0x6797B627),                /* ~= 10^235 */
    U64(0xFB9B7CD9, 0xA4A7443C), U64(0x169840EF, 0x017DA3B1),                /* ~= 10^236 */
    U64(0x9D412E08, 0x06E88AA5), U64(0x8E1F2895, 0x60EE864E),                /* ~= 10^237 */
    U64(0xC491798A, 0x08A2AD4E), U64(0xF1A6F2BA, 0xB92A27E2),                /* ~= 10^238 */
    U64(0xF5B5D7EC, 0x8ACB58A2), U64(0xAE10AF69, 0x6774B1DB),                /* ~= 10^239 */
    U64(0x9991A6F3, 0xD6BF1765), U64(0xACCA6DA1, 0xE0A8EF29),                /* ~= 10^240 */
    U64(0xBFF610B0, 0xCC6EDD3F), U64(0x17FD090A, 0x58D32AF3),                /* ~= 10^241 */
    U64(0xEFF394DC, 0xFF8A948E), U64(0xDDFC4B4C, 0xEF07F5B0),                /* ~= 10^242 */
    U64(0x95F83D0A, 0x1FB69CD9), U64(0x4ABDAF10, 0x1564F98E),                /* ~= 10^243 */
    U64(0xBB764C4C, 0xA7A4440F), U64(0x9D6D1AD4, 0x1ABE37F1),                /* ~= 10^244 */
    U64(0xEA53DF5F, 0xD18D5513), U64(0x84C86189, 0x216DC5ED),                /* ~= 10^245 */
    U64(0x92746B9B, 0xE2F8552C), U64(0x32FD3CF5, 0xB4E49BB4),                /* ~= 10^246 */
    U64(0xB7118682, 0xDBB66A77), U64(0x3FBC8C33, 0x221DC2A1),                /* ~= 10^247 */
    U64(0xE4D5E823, 0x92A40515), U64(0x0FABAF3F, 0xEAA5334A),                /* ~= 10^248 */
    U64(0x8F05B116, 0x3BA6832D), U64(0x29CB4D87, 0xF2A7400E),                /* ~= 10^249 */
    U64(0xB2C71D5B, 0xCA9023F8), U64(0x743E20E9, 0xEF511012),                /* ~= 10^250 */
    U64(0xDF78E4B2, 0xBD342CF6), U64(0x914DA924, 0x6B255416),                /* ~= 10^251 */
    U64(0x8BAB8EEF, 0xB6409C1A), U64(0x1AD089B6, 0xC2F7548E),                /* ~= 10^252 */
    U64(0xAE9672AB, 0xA3D0C320), U64(0xA184AC24, 0x73B529B1),                /* ~= 10^253 */
    U64(0xDA3C0F56, 0x8CC4F3E8), U64(0xC9E5D72D, 0x90A2741E),                /* ~= 10^254 */
    U64(0x88658996, 0x17FB1871), U64(0x7E2FA67C, 0x7A658892),                /* ~= 10^255 */
    U64(0xAA7EEBFB, 0x9DF9DE8D), U64(0xDDBB901B, 0x98FEEAB7),                /* ~= 10^256 */
    U64(0xD51EA6FA, 0x85785631), U64(0x552A7422, 0x7F3EA565),                /* ~= 10^257 */
    U64(0x8533285C, 0x936B35DE), U64(0xD53A8895, 0x8F87275F),                /* ~= 10^258 */
    U64(0xA67FF273, 0xB8460356), U64(0x8A892ABA, 0xF368F137),                /* ~= 10^259 */
    U64(0xD01FEF10, 0xA657842C), U64(0x2D2B7569, 0xB0432D85),                /* ~= 10^260 */
    U64(0x8213F56A, 0x67F6B29B), U64(0x9C3B2962, 0x0E29FC73),                /* ~= 10^261 */
    U64(0xA298F2C5, 0x01F45F42), U64(0x8349F3BA, 0x91B47B8F),                /* ~= 10^262 */
    U64(0xCB3F2F76, 0x42717713), U64(0x241C70A9, 0x36219A73),                /* ~= 10^263 */
    U64(0xFE0EFB53, 0xD30DD4D7), U64(0xED238CD3, 0x83AA0110),                /* ~= 10^264 */
    U64(0x9EC95D14, 0x63E8A506), U64(0xF4363804, 0x324A40AA),                /* ~= 10^265 */
    U64(0xC67BB459, 0x7CE2CE48), U64(0xB143C605, 0x3EDCD0D5),                /* ~= 10^266 */
    U64(0xF81AA16F, 0xDC1B81DA), U64(0xDD94B786, 0x8E94050A),                /* ~= 10^267 */
    U64(0x9B10A4E5, 0xE9913128), U64(0xCA7CF2B4, 0x191C8326),                /* ~= 10^268 */
    U64(0xC1D4CE1F, 0x63F57D72), U64(0xFD1C2F61, 0x1F63A3F0),                /* ~= 10^269 */
    U64(0xF24A01A7, 0x3CF2DCCF), U64(0xBC633B39, 0x673C8CEC),                /* ~= 10^270 */
    U64(0x976E4108, 0x8617CA01), U64(0xD5BE0503, 0xE085D813),                /* ~= 10^271 */
    U64(0xBD49D14A, 0xA79DBC82), U64(0x4B2D8644, 0xD8A74E18),                /* ~= 10^272 */
    U64(0xEC9C459D, 0x51852BA2), U64(0xDDF8E7D6, 0x0ED1219E),                /* ~= 10^273 */
    U64(0x93E1AB82, 0x52F33B45), U64(0xCABB90E5, 0xC942B503),                /* ~= 10^274 */
    U64(0xB8DA1662, 0xE7B00A17), U64(0x3D6A751F, 0x3B936243),                /* ~= 10^275 */
    U64(0xE7109BFB, 0xA19C0C9D), U64(0x0CC51267, 0x0A783AD4),                /* ~= 10^276 */
    U64(0x906A617D, 0x450187E2), U64(0x27FB2B80, 0x668B24C5),                /* ~= 10^277 */
    U64(0xB484F9DC, 0x9641E9DA), U64(0xB1F9F660, 0x802DEDF6),                /* ~= 10^278 */
    U64(0xE1A63853, 0xBBD26451), U64(0x5E7873F8, 0xA0396973),                /* ~= 10^279 */
    U64(0x8D07E334, 0x55637EB2), U64(0xDB0B487B, 0x6423E1E8),                /* ~= 10^280 */
    U64(0xB049DC01, 0x6ABC5E5F), U64(0x91CE1A9A, 0x3D2CDA62),                /* ~= 10^281 */
    U64(0xDC5C5301, 0xC56B75F7), U64(0x7641A140, 0xCC7810FB),                /* ~= 10^282 */
    U64(0x89B9B3E1, 0x1B6329BA), U64(0xA9E904C8, 0x7FCB0A9D),                /* ~= 10^283 */
    U64(0xAC2820D9, 0x623BF429), U64(0x546345FA, 0x9FBDCD44),                /* ~= 10^284 */
    U64(0xD732290F, 0xBACAF133), U64(0xA97C1779, 0x47AD4095),                /* ~= 10^285 */
    U64(0x867F59A9, 0xD4BED6C0), U64(0x49ED8EAB, 0xCCCC485D),                /* ~= 10^286 */
    U64(0xA81F3014, 0x49EE8C70), U64(0x5C68F256, 0xBFFF5A74),                /* ~= 10^287 */
    U64(0xD226FC19, 0x5C6A2F8C), U64(0x73832EEC, 0x6FFF3111),                /* ~= 10^288 */
    U64(0x83585D8F, 0xD9C25DB7), U64(0xC831FD53, 0xC5FF7EAB),                /* ~= 10^289 */
    U64(0xA42E74F3, 0xD032F525), U64(0xBA3E7CA8, 0xB77F5E55),                /* ~= 10^290 */
    U64(0xCD3A1230, 0xC43FB26F), U64(0x28CE1BD2, 0xE55F35EB),                /* ~= 10^291 */
    U64(0x80444B5E, 0x7AA7CF85), U64(0x7980D163, 0xCF5B81B3),                /* ~= 10^292 */
    U64(0xA0555E36, 0x1951C366), U64(0xD7E105BC, 0xC332621F),                /* ~= 10^293 */
    U64(0xC86AB5C3, 0x9FA63440), U64(0x8DD9472B, 0xF3FEFAA7),                /* ~= 10^294 */
    U64(0xFA856334, 0x878FC150), U64(0xB14F98F6, 0xF0FEB951),                /* ~= 10^295 */
    U64(0x9C935E00, 0xD4B9D8D2), U64(0x6ED1BF9A, 0x569F33D3),                /* ~= 10^296 */
    U64(0xC3B83581, 0x09E84F07), U64(0x0A862F80, 0xEC4700C8),                /* ~= 10^297 */
    U64(0xF4A642E1, 0x4C6262C8), U64(0xCD27BB61, 0x2758C0FA),                /* ~= 10^298 */
    U64(0x98E7E9CC, 0xCFBD7DBD), U64(0x8038D51C, 0xB897789C),                /* ~= 10^299 */
    U64(0xBF21E440, 0x03ACDD2C), U64(0xE0470A63, 0xE6BD56C3),                /* ~= 10^300 */
    U64(0xEEEA5D50, 0x04981478), U64(0x1858CCFC, 0xE06CAC74),                /* ~= 10^301 */
    U64(0x95527A52, 0x02DF0CCB), U64(0x0F37801E, 0x0C43EBC8),                /* ~= 10^302 */
    U64(0xBAA718E6, 0x8396CFFD), U64(0xD3056025, 0x8F54E6BA),                /* ~= 10^303 */
    U64(0xE950DF20, 0x247C83FD), U64(0x47C6B82E, 0xF32A2069),                /* ~= 10^304 */
    U64(0x91D28B74, 0x16CDD27E), U64(0x4CDC331D, 0x57FA5441),                /* ~= 10^305 */
    U64(0xB6472E51, 0x1C81471D), U64(0xE0133FE4, 0xADF8E952),                /* ~= 10^306 */
    U64(0xE3D8F9E5, 0x63A198E5), U64(0x58180FDD, 0xD97723A6),                /* ~= 10^307 */
    U64(0x8E679C2F, 0x5E44FF8F), U64(0x570F09EA, 0xA7EA7648),                /* ~= 10^308 */
    U64(0xB201833B, 0x35D63F73), U64(0x2CD2CC65, 0x51E513DA),                /* ~= 10^309 */
    U64(0xDE81E40A, 0x034BCF4F), U64(0xF8077F7E, 0xA65E58D1),                /* ~= 10^310 */
    U64(0x8B112E86, 0x420F6191), U64(0xFB04AFAF, 0x27FAF782),                /* ~= 10^311 */
    U64(0xADD57A27, 0xD29339F6), U64(0x79C5DB9A, 0xF1F9B563),                /* ~= 10^312 */
    U64(0xD94AD8B1, 0xC7380874), U64(0x18375281, 0xAE7822BC),                /* ~= 10^313 */
    U64(0x87CEC76F, 0x1C830548), U64(0x8F229391, 0x0D0B15B5),                /* ~= 10^314 */
    U64(0xA9C2794A, 0xE3A3C69A), U64(0xB2EB3875, 0x504DDB22),                /* ~= 10^315 */
    U64(0xD433179D, 0x9C8CB841), U64(0x5FA60692, 0xA46151EB),                /* ~= 10^316 */
    U64(0x849FEEC2, 0x81D7F328), U64(0xDBC7C41B, 0xA6BCD333),                /* ~= 10^317 */
    U64(0xA5C7EA73, 0x224DEFF3), U64(0x12B9B522, 0x906C0800),                /* ~= 10^318 */
    U64(0xCF39E50F, 0xEAE16BEF), U64(0xD768226B, 0x34870A00),                /* ~= 10^319 */
    U64(0x81842F29, 0xF2CCE375), U64(0xE6A11583, 0x00D46640),                /* ~= 10^320 */
    U64(0xA1E53AF4, 0x6F801C53), U64(0x60495AE3, 0xC1097FD0),                /* ~= 10^321 */
    U64(0xCA5E89B1, 0x8B602368), U64(0x385BB19C, 0xB14BDFC4),                /* ~= 10^322 */
    U64(0xFCF62C1D, 0xEE382C42), U64(0x46729E03, 0xDD9ED7B5),                /* ~= 10^323 */
    U64(0x9E19DB92, 0xB4E31BA9), U64(0x6C07A2C2, 0x6A8346D1)                 /* ~= 10^324 */
};
//-343,324
static const u64 pow10_sig_table_round[] = {
    U64(0xBF29DCAB, 0xA82FDEAE), U64(0x7432EE87, 0x3880FC33),                /* == 10^-343 */
    U64(0xEEF453D6, 0x923BD65A), U64(0x113FAA29, 0x06A13B40),                /* == 10^-342 */
    U64(0x9558B466, 0x1B6565F8), U64(0x4AC7CA59, 0xA424C508),                /* == 10^-341 */
    U64(0xBAAEE17F, 0xA23EBF76), U64(0x5D79BCF0, 0x0D2DF64A),                /* == 10^-340 */
    U64(0xE95A99DF, 0x8ACE6F53), U64(0xF4D82C2C, 0x107973DC),                /* == 10^-339 */
    U64(0x91D8A02B, 0xB6C10594), U64(0x79071B9B, 0x8A4BE86A),                /* == 10^-338 */
    U64(0xB64EC836, 0xA47146F9), U64(0x9748E282, 0x6CDEE284),                /* == 10^-337 */
    U64(0xE3E27A44, 0x4D8D98B7), U64(0xFD1B1B23, 0x08169B25),                /* == 10^-336 */
    U64(0x8E6D8C6A, 0xB0787F72), U64(0xFE30F0F5, 0xE50E20F7),                /* == 10^-335 */
    U64(0xB208EF85, 0x5C969F4F), U64(0xBDBD2D33, 0x5E51A935),                /* == 10^-334 */
    U64(0xDE8B2B66, 0xB3BC4723), U64(0xAD2C7880, 0x35E61382),                /* == 10^-333 */
    U64(0x8B16FB20, 0x3055AC76), U64(0x4C3BCB50, 0x21AFCC31),                /* == 10^-332 */
    U64(0xADDCB9E8, 0x3C6B1793), U64(0xDF4ABE24, 0x2A1BBF3E),                /* == 10^-331 */
    U64(0xD953E862, 0x4B85DD78), U64(0xD71D6DAD, 0x34A2AF0D),                /* == 10^-330 */
    U64(0x87D4713D, 0x6F33AA6B), U64(0x8672648C, 0x40E5AD68),                /* == 10^-329 */
    U64(0xA9C98D8C, 0xCB009506), U64(0x680EFDAF, 0x511F18C2),                /* == 10^-328 */
    U64(0xD43BF0EF, 0xFDC0BA48), U64(0x0212BD1B, 0x2566DEF3),                /* == 10^-327 */
    U64(0x84A57695, 0xFE98746D), U64(0x014BB630, 0xF7604B58),                /* == 10^-326 */
    U64(0xA5CED43B, 0x7E3E9188), U64(0x419EA3BD, 0x35385E2E),                /* == 10^-325 */
    U64(0xCF42894A, 0x5DCE35EA), U64(0x52064CAC, 0x828675B9),                /* == 10^-324 */
    U64(0x818995CE, 0x7AA0E1B2), U64(0x7343EFEB, 0xD1940994),                /* == 10^-323 */
    U64(0xA1EBFB42, 0x19491A1F), U64(0x1014EBE6, 0xC5F90BF9),                /* == 10^-322 */
    U64(0xCA66FA12, 0x9F9B60A6), U64(0xD41A26E0, 0x77774EF7),                /* == 10^-321 */
    U64(0xFD00B897, 0x478238D0), U64(0x8920B098, 0x955522B5),                /* == 10^-320 */
    U64(0x9E20735E, 0x8CB16382), U64(0x55B46E5F, 0x5D5535B1),                /* == 10^-319 */
    U64(0xC5A89036, 0x2FDDBC62), U64(0xEB2189F7, 0x34AA831D),                /* == 10^-318 */
    U64(0xF712B443, 0xBBD52B7B), U64(0xA5E9EC75, 0x01D523E4),                /* == 10^-317 */
    U64(0x9A6BB0AA, 0x55653B2D), U64(0x47B233C9, 0x2125366F),                /* == 10^-316 */
    U64(0xC1069CD4, 0xEABE89F8), U64(0x999EC0BB, 0x696E840A),                /* == 10^-315 */
    U64(0xF148440A, 0x256E2C76), U64(0xC00670EA, 0x43CA250D),                /* == 10^-314 */
    U64(0x96CD2A86, 0x5764DBCA), U64(0x38040692, 0x6A5E5728),                /* == 10^-313 */
    U64(0xBC807527, 0xED3E12BC), U64(0xC6050837, 0x04F5ECF2),                /* == 10^-312 */
    U64(0xEBA09271, 0xE88D976B), U64(0xF7864A44, 0xC633682F),                /* == 10^-311 */
    U64(0x93445B87, 0x31587EA3), U64(0x7AB3EE6A, 0xFBE0211D),                /* == 10^-310 */
    U64(0xB8157268, 0xFDAE9E4C), U64(0x5960EA05, 0xBAD82965),                /* == 10^-309 */
    U64(0xE61ACF03, 0x3D1A45DF), U64(0x6FB92487, 0x298E33BE),                /* == 10^-308 */
    U64(0x8FD0C162, 0x06306BAB), U64(0xA5D3B6D4, 0x79F8E057),                /* == 10^-307 */
    U64(0xB3C4F1BA, 0x87BC8696), U64(0x8F48A489, 0x9877186C),                /* == 10^-306 */
    U64(0xE0B62E29, 0x29ABA83C), U64(0x331ACDAB, 0xFE94DE87),                /* == 10^-305 */
    U64(0x8C71DCD9, 0xBA0B4925), U64(0x9FF0C08B, 0x7F1D0B15),                /* == 10^-304 */
    U64(0xAF8E5410, 0x288E1B6F), U64(0x07ECF0AE, 0x5EE44DDA),                /* == 10^-303 */
    U64(0xDB71E914, 0x32B1A24A), U64(0xC9E82CD9, 0xF69D6150),                /* == 10^-302 */
    U64(0x892731AC, 0x9FAF056E), U64(0xBE311C08, 0x3A225CD2),                /* == 10^-301 */
    U64(0xAB70FE17, 0xC79AC6CA), U64(0x6DBD630A, 0x48AAF407),                /* == 10^-300 */
    U64(0xD64D3D9D, 0xB981787D), U64(0x092CBBCC, 0xDAD5B108),                /* == 10^-299 */
    U64(0x85F04682, 0x93F0EB4E), U64(0x25BBF560, 0x08C58EA5),                /* == 10^-298 */
    U64(0xA76C5823, 0x38ED2621), U64(0xAF2AF2B8, 0x0AF6F24E),                /* == 10^-297 */
    U64(0xD1476E2C, 0x07286FAA), U64(0x1AF5AF66, 0x0DB4AEE2),                /* == 10^-296 */
    U64(0x82CCA4DB, 0x847945CA), U64(0x50D98D9F, 0xC890ED4D),                /* == 10^-295 */
    U64(0xA37FCE12, 0x6597973C), U64(0xE50FF107, 0xBAB528A1),                /* == 10^-294 */
    U64(0xCC5FC196, 0xFEFD7D0C), U64(0x1E53ED49, 0xA96272C9),                /* == 10^-293 */
    U64(0xFF77B1FC, 0xBEBCDC4F), U64(0x25E8E89C, 0x13BB0F7B),                /* == 10^-292 */
    U64(0x9FAACF3D, 0xF73609B1), U64(0x77B19161, 0x8C54E9AD),                /* == 10^-291 */
    U64(0xC795830D, 0x75038C1D), U64(0xD59DF5B9, 0xEF6A2418),                /* == 10^-290 */
    U64(0xF97AE3D0, 0xD2446F25), U64(0x4B057328, 0x6B44AD1E),                /* == 10^-289 */
    U64(0x9BECCE62, 0x836AC577), U64(0x4EE367F9, 0x430AEC33),                /* == 10^-288 */
    U64(0xC2E801FB, 0x244576D5), U64(0x229C41F7, 0x93CDA73F),                /* == 10^-287 */
    U64(0xF3A20279, 0xED56D48A), U64(0x6B435275, 0x78C1110F),                /* == 10^-286 */
    U64(0x9845418C, 0x345644D6), U64(0x830A1389, 0x6B78AAAA),                /* == 10^-285 */
    U64(0xBE5691EF, 0x416BD60C), U64(0x23CC986B, 0xC656D554),                /* == 10^-284 */
    U64(0xEDEC366B, 0x11C6CB8F), U64(0x2CBFBE86, 0xB7EC8AA9),                /* == 10^-283 */
    U64(0x94B3A202, 0xEB1C3F39), U64(0x7BF7D714, 0x32F3D6AA),                /* == 10^-282 */
    U64(0xB9E08A83, 0xA5E34F07), U64(0xDAF5CCD9, 0x3FB0CC54),                /* == 10^-281 */
    U64(0xE858AD24, 0x8F5C22C9), U64(0xD1B3400F, 0x8F9CFF69),                /* == 10^-280 */
    U64(0x91376C36, 0xD99995BE), U64(0x23100809, 0xB9C21FA2),                /* == 10^-279 */
    U64(0xB5854744, 0x8FFFFB2D), U64(0xABD40A0C, 0x2832A78A),                /* == 10^-278 */
    U64(0xE2E69915, 0xB3FFF9F9), U64(0x16C90C8F, 0x323F516D),                /* == 10^-277 */
    U64(0x8DD01FAD, 0x907FFC3B), U64(0xAE3DA7D9, 0x7F6792E4),                /* == 10^-276 */
    U64(0xB1442798, 0xF49FFB4A), U64(0x99CD11CF, 0xDF41779D),                /* == 10^-275 */
    U64(0xDD95317F, 0x31C7FA1D), U64(0x40405643, 0xD711D584),                /* == 10^-274 */
    U64(0x8A7D3EEF, 0x7F1CFC52), U64(0x482835EA, 0x666B2572),                /* == 10^-273 */
    U64(0xAD1C8EAB, 0x5EE43B66), U64(0xDA324365, 0x0005EECF),                /* == 10^-272 */
    U64(0xD863B256, 0x369D4A40), U64(0x90BED43E, 0x40076A83),                /* == 10^-271 */
    U64(0x873E4F75, 0xE2224E68), U64(0x5A7744A6, 0xE804A292),                /* == 10^-270 */
    U64(0xA90DE353, 0x5AAAE202), U64(0x711515D0, 0xA205CB36),                /* == 10^-269 */
    U64(0xD3515C28, 0x31559A83), U64(0x0D5A5B44, 0xCA873E04),                /* == 10^-268 */
    U64(0x8412D999, 0x1ED58091), U64(0xE858790A, 0xFE9486C2),                /* == 10^-267 */
    U64(0xA5178FFF, 0x668AE0B6), U64(0x626E974D, 0xBE39A873),                /* == 10^-266 */
    U64(0xCE5D73FF, 0x402D98E3), U64(0xFB0A3D21, 0x2DC81290),                /* == 10^-265 */
    U64(0x80FA687F, 0x881C7F8E), U64(0x7CE66634, 0xBC9D0B9A),                /* == 10^-264 */
    U64(0xA139029F, 0x6A239F72), U64(0x1C1FFFC1, 0xEBC44E80),                /* == 10^-263 */
    U64(0xC9874347, 0x44AC874E), U64(0xA327FFB2, 0x66B56220),                /* == 10^-262 */
    U64(0xFBE91419, 0x15D7A922), U64(0x4BF1FF9F, 0x0062BAA8),                /* == 10^-261 */
    U64(0x9D71AC8F, 0xADA6C9B5), U64(0x6F773FC3, 0x603DB4A9),                /* == 10^-260 */
    U64(0xC4CE17B3, 0x99107C22), U64(0xCB550FB4, 0x384D21D4),                /* == 10^-259 */
    U64(0xF6019DA0, 0x7F549B2B), U64(0x7E2A53A1, 0x46606A48),                /* == 10^-258 */
    U64(0x99C10284, 0x4F94E0FB), U64(0x2EDA7444, 0xCBFC426D),                /* == 10^-257 */
    U64(0xC0314325, 0x637A1939), U64(0xFA911155, 0xFEFB5309),                /* == 10^-256 */
    U64(0xF03D93EE, 0xBC589F88), U64(0x793555AB, 0x7EBA27CB),                /* == 10^-255 */
    U64(0x96267C75, 0x35B763B5), U64(0x4BC1558B, 0x2F3458DF),                /* == 10^-254 */
    U64(0xBBB01B92, 0x83253CA2), U64(0x9EB1AAED, 0xFB016F16),                /* == 10^-253 */
    U64(0xEA9C2277, 0x23EE8BCB), U64(0x465E15A9, 0x79C1CADC),                /* == 10^-252 */
    U64(0x92A1958A, 0x7675175F), U64(0x0BFACD89, 0xEC191ECA),                /* == 10^-251 */
    U64(0xB749FAED, 0x14125D36), U64(0xCEF980EC, 0x671F667C),                /* == 10^-250 */
    U64(0xE51C79A8, 0x5916F484), U64(0x82B7E127, 0x80E7401B),                /* == 10^-249 */
    U64(0x8F31CC09, 0x37AE58D2), U64(0xD1B2ECB8, 0xB0908811),                /* == 10^-248 */
    U64(0xB2FE3F0B, 0x8599EF07), U64(0x861FA7E6, 0xDCB4AA15),                /* == 10^-247 */
    U64(0xDFBDCECE, 0x67006AC9), U64(0x67A791E0, 0x93E1D49A),                /* == 10^-246 */
    U64(0x8BD6A141, 0x006042BD), U64(0xE0C8BB2C, 0x5C6D24E0),                /* == 10^-245 */
    U64(0xAECC4991, 0x4078536D), U64(0x58FAE9F7, 0x73886E19),                /* == 10^-244 */
    U64(0xDA7F5BF5, 0x90966848), U64(0xAF39A475, 0x506A899F),                /* == 10^-243 */
    U64(0x888F9979, 0x7A5E012D), U64(0x6D8406C9, 0x52429603),                /* == 10^-242 */
    U64(0xAAB37FD7, 0xD8F58178), U64(0xC8E5087B, 0xA6D33B84),                /* == 10^-241 */
    U64(0xD5605FCD, 0xCF32E1D6), U64(0xFB1E4A9A, 0x90880A65),                /* == 10^-240 */
    U64(0x855C3BE0, 0xA17FCD26), U64(0x5CF2EEA0, 0x9A55067F),                /* == 10^-239 */
    U64(0xA6B34AD8, 0xC9DFC06F), U64(0xF42FAA48, 0xC0EA481F),                /* == 10^-238 */
    U64(0xD0601D8E, 0xFC57B08B), U64(0xF13B94DA, 0xF124DA27),                /* == 10^-237 */
    U64(0x823C1279, 0x5DB6CE57), U64(0x76C53D08, 0xD6B70858),                /* == 10^-236 */
    U64(0xA2CB1717, 0xB52481ED), U64(0x54768C4B, 0x0C64CA6E),                /* == 10^-235 */
    U64(0xCB7DDCDD, 0xA26DA268), U64(0xA9942F5D, 0xCF7DFD0A),                /* == 10^-234 */
    U64(0xFE5D5415, 0x0B090B02), U64(0xD3F93B35, 0x435D7C4C),                /* == 10^-233 */
    U64(0x9EFA548D, 0x26E5A6E1), U64(0xC47BC501, 0x4A1A6DB0),                /* == 10^-232 */
    U64(0xC6B8E9B0, 0x709F109A), U64(0x359AB641, 0x9CA1091B),                /* == 10^-231 */
    U64(0xF867241C, 0x8CC6D4C0), U64(0xC30163D2, 0x03C94B62),                /* == 10^-230 */
    U64(0x9B407691, 0xD7FC44F8), U64(0x79E0DE63, 0x425DCF1D),                /* == 10^-229 */
    U64(0xC2109436, 0x4DFB5636), U64(0x985915FC, 0x12F542E5),                /* == 10^-228 */
    U64(0xF294B943, 0xE17A2BC4), U64(0x3E6F5B7B, 0x17B2939E),                /* == 10^-227 */
    U64(0x979CF3CA, 0x6CEC5B5A), U64(0xA705992C, 0xEECF9C43),                /* == 10^-226 */
    U64(0xBD8430BD, 0x08277231), U64(0x50C6FF78, 0x2A838353),                /* == 10^-225 */
    U64(0xECE53CEC, 0x4A314EBD), U64(0xA4F8BF56, 0x35246428),                /* == 10^-224 */
    U64(0x940F4613, 0xAE5ED136), U64(0x871B7795, 0xE136BE99),                /* == 10^-223 */
    U64(0xB9131798, 0x99F68584), U64(0x28E2557B, 0x59846E3F),                /* == 10^-222 */
    U64(0xE757DD7E, 0xC07426E5), U64(0x331AEADA, 0x2FE589CF),                /* == 10^-221 */
    U64(0x9096EA6F, 0x3848984F), U64(0x3FF0D2C8, 0x5DEF7622),                /* == 10^-220 */
    U64(0xB4BCA50B, 0x065ABE63), U64(0x0FED077A, 0x756B53AA),                /* == 10^-219 */
    U64(0xE1EBCE4D, 0xC7F16DFB), U64(0xD3E84959, 0x12C62894),                /* == 10^-218 */
    U64(0x8D3360F0, 0x9CF6E4BD), U64(0x64712DD7, 0xABBBD95D),                /* == 10^-217 */
    U64(0xB080392C, 0xC4349DEC), U64(0xBD8D794D, 0x96AACFB4),                /* == 10^-216 */
    U64(0xDCA04777, 0xF541C567), U64(0xECF0D7A0, 0xFC5583A1),                /* == 10^-215 */
    U64(0x89E42CAA, 0xF9491B60), U64(0xF41686C4, 0x9DB57245),                /* == 10^-214 */
    U64(0xAC5D37D5, 0xB79B6239), U64(0x311C2875, 0xC522CED6),                /* == 10^-213 */
    U64(0xD77485CB, 0x25823AC7), U64(0x7D633293, 0x366B828B),                /* == 10^-212 */
    U64(0x86A8D39E, 0xF77164BC), U64(0xAE5DFF9C, 0x02033197),                /* == 10^-211 */
    U64(0xA8530886, 0xB54DBDEB), U64(0xD9F57F83, 0x0283FDFD),                /* == 10^-210 */
    U64(0xD267CAA8, 0x62A12D66), U64(0xD072DF63, 0xC324FD7C),                /* == 10^-209 */
    U64(0x8380DEA9, 0x3DA4BC60), U64(0x4247CB9E, 0x59F71E6D),                /* == 10^-208 */
    U64(0xA4611653, 0x8D0DEB78), U64(0x52D9BE85, 0xF074E609),                /* == 10^-207 */
    U64(0xCD795BE8, 0x70516656), U64(0x67902E27, 0x6C921F8B),                /* == 10^-206 */
    U64(0x806BD971, 0x4632DFF6), U64(0x00BA1CD8, 0xA3DB53B7),                /* == 10^-205 */
    U64(0xA086CFCD, 0x97BF97F3), U64(0x80E8A40E, 0xCCD228A5),                /* == 10^-204 */
    U64(0xC8A883C0, 0xFDAF7DF0), U64(0x6122CD12, 0x8006B2CE),                /* == 10^-203 */
    U64(0xFAD2A4B1, 0x3D1B5D6C), U64(0x796B8057, 0x20085F81),                /* == 10^-202 */
    U64(0x9CC3A6EE, 0xC6311A63), U64(0xCBE33036, 0x74053BB1),                /* == 10^-201 */
    U64(0xC3F490AA, 0x77BD60FC), U64(0xBEDBFC44, 0x11068A9D),                /* == 10^-200 */
    U64(0xF4F1B4D5, 0x15ACB93B), U64(0xEE92FB55, 0x15482D44),                /* == 10^-199 */
    U64(0x99171105, 0x2D8BF3C5), U64(0x751BDD15, 0x2D4D1C4B),                /* == 10^-198 */
    U64(0xBF5CD546, 0x78EEF0B6), U64(0xD262D45A, 0x78A0635D),                /* == 10^-197 */
    U64(0xEF340A98, 0x172AACE4), U64(0x86FB8971, 0x16C87C35),                /* == 10^-196 */
    U64(0x9580869F, 0x0E7AAC0E), U64(0xD45D35E6, 0xAE3D4DA1),                /* == 10^-195 */
    U64(0xBAE0A846, 0xD2195712), U64(0x89748360, 0x59CCA109),                /* == 10^-194 */
    U64(0xE998D258, 0x869FACD7), U64(0x2BD1A438, 0x703FC94B),                /* == 10^-193 */
    U64(0x91FF8377, 0x5423CC06), U64(0x7B6306A3, 0x4627DDCF),                /* == 10^-192 */
    U64(0xB67F6455, 0x292CBF08), U64(0x1A3BC84C, 0x17B1D543),                /* == 10^-191 */
    U64(0xE41F3D6A, 0x7377EECA), U64(0x20CABA5F, 0x1D9E4A94),                /* == 10^-190 */
    U64(0x8E938662, 0x882AF53E), U64(0x547EB47B, 0x7282EE9C),                /* == 10^-189 */
    U64(0xB23867FB, 0x2A35B28D), U64(0xE99E619A, 0x4F23AA43),                /* == 10^-188 */
    U64(0xDEC681F9, 0xF4C31F31), U64(0x6405FA00, 0xE2EC94D4),                /* == 10^-187 */
    U64(0x8B3C113C, 0x38F9F37E), U64(0xDE83BC40, 0x8DD3DD05),                /* == 10^-186 */
    U64(0xAE0B158B, 0x4738705E), U64(0x9624AB50, 0xB148D446),                /* == 10^-185 */
    U64(0xD98DDAEE, 0x19068C76), U64(0x3BADD624, 0xDD9B0957),                /* == 10^-184 */
    U64(0x87F8A8D4, 0xCFA417C9), U64(0xE54CA5D7, 0x0A80E5D6),                /* == 10^-183 */
    U64(0xA9F6D30A, 0x038D1DBC), U64(0x5E9FCF4C, 0xCD211F4C),                /* == 10^-182 */
    U64(0xD47487CC, 0x8470652B), U64(0x7647C320, 0x0069671F),                /* == 10^-181 */
    U64(0x84C8D4DF, 0xD2C63F3B), U64(0x29ECD9F4, 0x0041E073),                /* == 10^-180 */
    U64(0xA5FB0A17, 0xC777CF09), U64(0xF4681071, 0x00525890),                /* == 10^-179 */
    U64(0xCF79CC9D, 0xB955C2CC), U64(0x7182148D, 0x4066EEB4),                /* == 10^-178 */
    U64(0x81AC1FE2, 0x93D599BF), U64(0xC6F14CD8, 0x48405531),                /* == 10^-177 */
    U64(0xA21727DB, 0x38CB002F), U64(0xB8ADA00E, 0x5A506A7D),                /* == 10^-176 */
    U64(0xCA9CF1D2, 0x06FDC03B), U64(0xA6D90811, 0xF0E4851C),                /* == 10^-175 */
    U64(0xFD442E46, 0x88BD304A), U64(0x908F4A16, 0x6D1DA663),                /* == 10^-174 */
    U64(0x9E4A9CEC, 0x15763E2E), U64(0x9A598E4E, 0x043287FE),                /* == 10^-173 */
    U64(0xC5DD4427, 0x1AD3CDBA), U64(0x40EFF1E1, 0x853F29FE),                /* == 10^-172 */
    U64(0xF7549530, 0xE188C128), U64(0xD12BEE59, 0xE68EF47D),                /* == 10^-171 */
    U64(0x9A94DD3E, 0x8CF578B9), U64(0x82BB74F8, 0x301958CE),                /* == 10^-170 */
    U64(0xC13A148E, 0x3032D6E7), U64(0xE36A5236, 0x3C1FAF02),                /* == 10^-169 */
    U64(0xF18899B1, 0xBC3F8CA1), U64(0xDC44E6C3, 0xCB279AC2),                /* == 10^-168 */
    U64(0x96F5600F, 0x15A7B7E5), U64(0x29AB103A, 0x5EF8C0B9),                /* == 10^-167 */
    U64(0xBCB2B812, 0xDB11A5DE), U64(0x7415D448, 0xF6B6F0E8),                /* == 10^-166 */
    U64(0xEBDF6617, 0x91D60F56), U64(0x111B495B, 0x3464AD21),                /* == 10^-165 */
    U64(0x936B9FCE, 0xBB25C995), U64(0xCAB10DD9, 0x00BEEC35),                /* == 10^-164 */
    U64(0xB84687C2, 0x69EF3BFB), U64(0x3D5D514F, 0x40EEA742),                /* == 10^-163 */
    U64(0xE65829B3, 0x046B0AFA), U64(0x0CB4A5A3, 0x112A5113),                /* == 10^-162 */
    U64(0x8FF71A0F, 0xE2C2E6DC), U64(0x47F0E785, 0xEABA72AC),                /* == 10^-161 */
    U64(0xB3F4E093, 0xDB73A093), U64(0x59ED2167, 0x65690F57),                /* == 10^-160 */
    U64(0xE0F218B8, 0xD25088B8), U64(0x306869C1, 0x3EC3532C),                /* == 10^-159 */
    U64(0x8C974F73, 0x83725573), U64(0x1E414218, 0xC73A13FC),                /* == 10^-158 */
    U64(0xAFBD2350, 0x644EEACF), U64(0xE5D1929E, 0xF90898FB),                /* == 10^-157 */
    U64(0xDBAC6C24, 0x7D62A583), U64(0xDF45F746, 0xB74ABF39),                /* == 10^-156 */
    U64(0x894BC396, 0xCE5DA772), U64(0x6B8BBA8C, 0x328EB784),                /* == 10^-155 */
    U64(0xAB9EB47C, 0x81F5114F), U64(0x066EA92F, 0x3F326565),                /* == 10^-154 */
    U64(0xD686619B, 0xA27255A2), U64(0xC80A537B, 0x0EFEFEBE),                /* == 10^-153 */
    U64(0x8613FD01, 0x45877585), U64(0xBD06742C, 0xE95F5F37),                /* == 10^-152 */
    U64(0xA798FC41, 0x96E952E7), U64(0x2C481138, 0x23B73704),                /* == 10^-151 */
    U64(0xD17F3B51, 0xFCA3A7A0), U64(0xF75A1586, 0x2CA504C5),                /* == 10^-150 */
    U64(0x82EF8513, 0x3DE648C4), U64(0x9A984D73, 0xDBE722FB),                /* == 10^-149 */
    U64(0xA3AB6658, 0x0D5FDAF5), U64(0xC13E60D0, 0xD2E0EBBA),                /* == 10^-148 */
    U64(0xCC963FEE, 0x10B7D1B3), U64(0x318DF905, 0x079926A9),                /* == 10^-147 */
    U64(0xFFBBCFE9, 0x94E5C61F), U64(0xFDF17746, 0x497F7053),                /* == 10^-146 */
    U64(0x9FD561F1, 0xFD0F9BD3), U64(0xFEB6EA8B, 0xEDEFA634),                /* == 10^-145 */
    U64(0xC7CABA6E, 0x7C5382C8), U64(0xFE64A52E, 0xE96B8FC1),                /* == 10^-144 */
    U64(0xF9BD690A, 0x1B68637B), U64(0x3DFDCE7A, 0xA3C673B1),                /* == 10^-143 */
    U64(0x9C1661A6, 0x51213E2D), U64(0x06BEA10C, 0xA65C084F),                /* == 10^-142 */
    U64(0xC31BFA0F, 0xE5698DB8), U64(0x486E494F, 0xCFF30A62),                /* == 10^-141 */
    U64(0xF3E2F893, 0xDEC3F126), U64(0x5A89DBA3, 0xC3EFCCFB),                /* == 10^-140 */
    U64(0x986DDB5C, 0x6B3A76B7), U64(0xF8962946, 0x5A75E01D),                /* == 10^-139 */
    U64(0xBE895233, 0x86091465), U64(0xF6BBB397, 0xF1135824),                /* == 10^-138 */
    U64(0xEE2BA6C0, 0x678B597F), U64(0x746AA07D, 0xED582E2D),                /* == 10^-137 */
    U64(0x94DB4838, 0x40B717EF), U64(0xA8C2A44E, 0xB4571CDC),                /* == 10^-136 */
    U64(0xBA121A46, 0x50E4DDEB), U64(0x92F34D62, 0x616CE413),                /* == 10^-135 */
    U64(0xE896A0D7, 0xE51E1566), U64(0x77B020BA, 0xF9C81D18),                /* == 10^-134 */
    U64(0x915E2486, 0xEF32CD60), U64(0x0ACE1474, 0xDC1D122F),                /* == 10^-133 */
    U64(0xB5B5ADA8, 0xAAFF80B8), U64(0x0D819992, 0x132456BB),                /* == 10^-132 */
    U64(0xE3231912, 0xD5BF60E6), U64(0x10E1FFF6, 0x97ED6C69),                /* == 10^-131 */
    U64(0x8DF5EFAB, 0xC5979C8F), U64(0xCA8D3FFA, 0x1EF463C2),                /* == 10^-130 */
    U64(0xB1736B96, 0xB6FD83B3), U64(0xBD308FF8, 0xA6B17CB2),                /* == 10^-129 */
    U64(0xDDD0467C, 0x64BCE4A0), U64(0xAC7CB3F6, 0xD05DDBDF),                /* == 10^-128 */
    U64(0x8AA22C0D, 0xBEF60EE4), U64(0x6BCDF07A, 0x423AA96B),                /* == 10^-127 */
    U64(0xAD4AB711, 0x2EB3929D), U64(0x86C16C98, 0xD2C953C6),                /* == 10^-126 */
    U64(0xD89D64D5, 0x7A607744), U64(0xE871C7BF, 0x077BA8B8),                /* == 10^-125 */
    U64(0x87625F05, 0x6C7C4A8B), U64(0x11471CD7, 0x64AD4973),                /* == 10^-124 */
    U64(0xA93AF6C6, 0xC79B5D2D), U64(0xD598E40D, 0x3DD89BCF),                /* == 10^-123 */
    U64(0xD389B478, 0x79823479), U64(0x4AFF1D10, 0x8D4EC2C3),                /* == 10^-122 */
    U64(0x843610CB, 0x4BF160CB), U64(0xCEDF722A, 0x585139BA),                /* == 10^-121 */
    U64(0xA54394FE, 0x1EEDB8FE), U64(0xC2974EB4, 0xEE658829),                /* == 10^-120 */
    U64(0xCE947A3D, 0xA6A9273E), U64(0x733D2262, 0x29FEEA33),                /* == 10^-119 */
    U64(0x811CCC66, 0x8829B887), U64(0x0806357D, 0x5A3F5260),                /* == 10^-118 */
    U64(0xA163FF80, 0x2A3426A8), U64(0xCA07C2DC, 0xB0CF26F8),                /* == 10^-117 */
    U64(0xC9BCFF60, 0x34C13052), U64(0xFC89B393, 0xDD02F0B6),                /* == 10^-116 */
    U64(0xFC2C3F38, 0x41F17C67), U64(0xBBAC2078, 0xD443ACE3),                /* == 10^-115 */
    U64(0x9D9BA783, 0x2936EDC0), U64(0xD54B944B, 0x84AA4C0E),                /* == 10^-114 */
    U64(0xC5029163, 0xF384A931), U64(0x0A9E795E, 0x65D4DF11),                /* == 10^-113 */
    U64(0xF64335BC, 0xF065D37D), U64(0x4D4617B5, 0xFF4A16D6),                /* == 10^-112 */
    U64(0x99EA0196, 0x163FA42E), U64(0x504BCED1, 0xBF8E4E46),                /* == 10^-111 */
    U64(0xC06481FB, 0x9BCF8D39), U64(0xE45EC286, 0x2F71E1D7),                /* == 10^-110 */
    U64(0xF07DA27A, 0x82C37088), U64(0x5D767327, 0xBB4E5A4D),                /* == 10^-109 */
    U64(0x964E858C, 0x91BA2655), U64(0x3A6A07F8, 0xD510F870),                /* == 10^-108 */
    U64(0xBBE226EF, 0xB628AFEA), U64(0x890489F7, 0x0A55368C),                /* == 10^-107 */
    U64(0xEADAB0AB, 0xA3B2DBE5), U64(0x2B45AC74, 0xCCEA842F),                /* == 10^-106 */
    U64(0x92C8AE6B, 0x464FC96F), U64(0x3B0B8BC9, 0x0012929D),                /* == 10^-105 */
    U64(0xB77ADA06, 0x17E3BBCB), U64(0x09CE6EBB, 0x40173745),                /* == 10^-104 */
    U64(0xE5599087, 0x9DDCAABD), U64(0xCC420A6A, 0x101D0516),                /* == 10^-103 */
    U64(0x8F57FA54, 0xC2A9EAB6), U64(0x9FA94682, 0x4A12232E),                /* == 10^-102 */
    U64(0xB32DF8E9, 0xF3546564), U64(0x47939822, 0xDC96ABF9),                /* == 10^-101 */
    U64(0xDFF97724, 0x70297EBD), U64(0x59787E2B, 0x93BC56F7),                /* == 10^-100 */
    U64(0x8BFBEA76, 0xC619EF36), U64(0x57EB4EDB, 0x3C55B65B),                /* == 10^-99 */
    U64(0xAEFAE514, 0x77A06B03), U64(0xEDE62292, 0x0B6B23F1),                /* == 10^-98 */
    U64(0xDAB99E59, 0x958885C4), U64(0xE95FAB36, 0x8E45ECED),                /* == 10^-97 */
    U64(0x88B402F7, 0xFD75539B), U64(0x11DBCB02, 0x18EBB414),                /* == 10^-96 */
    U64(0xAAE103B5, 0xFCD2A881), U64(0xD652BDC2, 0x9F26A11A),                /* == 10^-95 */
    U64(0xD59944A3, 0x7C0752A2), U64(0x4BE76D33, 0x46F04960),                /* == 10^-94 */
    U64(0x857FCAE6, 0x2D8493A5), U64(0x6F70A440, 0x0C562DDC),                /* == 10^-93 */
    U64(0xA6DFBD9F, 0xB8E5B88E), U64(0xCB4CCD50, 0x0F6BB953),                /* == 10^-92 */
    U64(0xD097AD07, 0xA71F26B2), U64(0x7E2000A4, 0x1346A7A8),                /* == 10^-91 */
    U64(0x825ECC24, 0xC873782F), U64(0x8ED40066, 0x8C0C28C9),                /* == 10^-90 */
    U64(0xA2F67F2D, 0xFA90563B), U64(0x72890080, 0x2F0F32FB),                /* == 10^-89 */
    U64(0xCBB41EF9, 0x79346BCA), U64(0x4F2B40A0, 0x3AD2FFBA),                /* == 10^-88 */
    U64(0xFEA126B7, 0xD78186BC), U64(0xE2F610C8, 0x4987BFA8),                /* == 10^-87 */
    U64(0x9F24B832, 0xE6B0F436), U64(0x0DD9CA7D, 0x2DF4D7C9),                /* == 10^-86 */
    U64(0xC6EDE63F, 0xA05D3143), U64(0x91503D1C, 0x79720DBB),                /* == 10^-85 */
    U64(0xF8A95FCF, 0x88747D94), U64(0x75A44C63, 0x97CE912A),                /* == 10^-84 */
    U64(0x9B69DBE1, 0xB548CE7C), U64(0xC986AFBE, 0x3EE11ABA),                /* == 10^-83 */
    U64(0xC24452DA, 0x229B021B), U64(0xFBE85BAD, 0xCE996169),                /* == 10^-82 */
    U64(0xF2D56790, 0xAB41C2A2), U64(0xFAE27299, 0x423FB9C3),                /* == 10^-81 */
    U64(0x97C560BA, 0x6B0919A5), U64(0xDCCD879F, 0xC967D41A),                /* == 10^-80 */
    U64(0xBDB6B8E9, 0x05CB600F), U64(0x5400E987, 0xBBC1C921),                /* == 10^-79 */
    U64(0xED246723, 0x473E3813), U64(0x290123E9, 0xAAB23B69),                /* == 10^-78 */
    U64(0x9436C076, 0x0C86E30B), U64(0xF9A0B672, 0x0AAF6521),                /* == 10^-77 */
    U64(0xB9447093, 0x8FA89BCE), U64(0xF808E40E, 0x8D5B3E6A),                /* == 10^-76 */
    U64(0xE7958CB8, 0x7392C2C2), U64(0xB60B1D12, 0x30B20E04),                /* == 10^-75 */
    U64(0x90BD77F3, 0x483BB9B9), U64(0xB1C6F22B, 0x5E6F48C3),                /* == 10^-74 */
    U64(0xB4ECD5F0, 0x1A4AA828), U64(0x1E38AEB6, 0x360B1AF3),                /* == 10^-73 */
    U64(0xE2280B6C, 0x20DD5232), U64(0x25C6DA63, 0xC38DE1B0),                /* == 10^-72 */
    U64(0x8D590723, 0x948A535F), U64(0x579C487E, 0x5A38AD0E),                /* == 10^-71 */
    U64(0xB0AF48EC, 0x79ACE837), U64(0x2D835A9D, 0xF0C6D852),                /* == 10^-70 */
    U64(0xDCDB1B27, 0x98182244), U64(0xF8E43145, 0x6CF88E66),                /* == 10^-69 */
    U64(0x8A08F0F8, 0xBF0F156B), U64(0x1B8E9ECB, 0x641B5900),                /* == 10^-68 */
    U64(0xAC8B2D36, 0xEED2DAC5), U64(0xE272467E, 0x3D222F40),                /* == 10^-67 */
    U64(0xD7ADF884, 0xAA879177), U64(0x5B0ED81D, 0xCC6ABB10),                /* == 10^-66 */
    U64(0x86CCBB52, 0xEA94BAEA), U64(0x98E94712, 0x9FC2B4EA),                /* == 10^-65 */
    U64(0xA87FEA27, 0xA539E9A5), U64(0x3F2398D7, 0x47B36224),                /* == 10^-64 */
    U64(0xD29FE4B1, 0x8E88640E), U64(0x8EEC7F0D, 0x19A03AAD),                /* == 10^-63 */
    U64(0x83A3EEEE, 0xF9153E89), U64(0x1953CF68, 0x300424AC),                /* == 10^-62 */
    U64(0xA48CEAAA, 0xB75A8E2B), U64(0x5FA8C342, 0x3C052DD7),                /* == 10^-61 */
    U64(0xCDB02555, 0x653131B6), U64(0x3792F412, 0xCB06794D),                /* == 10^-60 */
    U64(0x808E1755, 0x5F3EBF11), U64(0xE2BBD88B, 0xBEE40BD0),                /* == 10^-59 */
    U64(0xA0B19D2A, 0xB70E6ED6), U64(0x5B6ACEAE, 0xAE9D0EC4),                /* == 10^-58 */
    U64(0xC8DE0475, 0x64D20A8B), U64(0xF245825A, 0x5A445275),                /* == 10^-57 */
    U64(0xFB158592, 0xBE068D2E), U64(0xEED6E2F0, 0xF0D56713),                /* == 10^-56 */
    U64(0x9CED737B, 0xB6C4183D), U64(0x55464DD6, 0x9685606C),                /* == 10^-55 */
    U64(0xC428D05A, 0xA4751E4C), U64(0xAA97E14C, 0x3C26B887),                /* == 10^-54 */
    U64(0xF5330471, 0x4D9265DF), U64(0xD53DD99F, 0x4B3066A8),                /* == 10^-53 */
    U64(0x993FE2C6, 0xD07B7FAB), U64(0xE546A803, 0x8EFE4029),                /* == 10^-52 */
    U64(0xBF8FDB78, 0x849A5F96), U64(0xDE985204, 0x72BDD033),                /* == 10^-51 */
    U64(0xEF73D256, 0xA5C0F77C), U64(0x963E6685, 0x8F6D4440),                /* == 10^-50 */
    U64(0x95A86376, 0x27989AAD), U64(0xDDE70013, 0x79A44AA8),                /* == 10^-49 */
    U64(0xBB127C53, 0xB17EC159), U64(0x5560C018, 0x580D5D52),                /* == 10^-48 */
    U64(0xE9D71B68, 0x9DDE71AF), U64(0xAAB8F01E, 0x6E10B4A7),                /* == 10^-47 */
    U64(0x92267121, 0x62AB070D), U64(0xCAB39613, 0x04CA70E8),                /* == 10^-46 */
    U64(0xB6B00D69, 0xBB55C8D1), U64(0x3D607B97, 0xC5FD0D22),                /* == 10^-45 */
    U64(0xE45C10C4, 0x2A2B3B05), U64(0x8CB89A7D, 0xB77C506B),                /* == 10^-44 */
    U64(0x8EB98A7A, 0x9A5B04E3), U64(0x77F3608E, 0x92ADB243),                /* == 10^-43 */
    U64(0xB267ED19, 0x40F1C61C), U64(0x55F038B2, 0x37591ED3),                /* == 10^-42 */
    U64(0xDF01E85F, 0x912E37A3), U64(0x6B6C46DE, 0xC52F6688),                /* == 10^-41 */
    U64(0x8B61313B, 0xBABCE2C6), U64(0x2323AC4B, 0x3B3DA015),                /* == 10^-40 */
    U64(0xAE397D8A, 0xA96C1B77), U64(0xABEC975E, 0x0A0D081B),                /* == 10^-39 */
    U64(0xD9C7DCED, 0x53C72255), U64(0x96E7BD35, 0x8C904A21),                /* == 10^-38 */
    U64(0x881CEA14, 0x545C7575), U64(0x7E50D641, 0x77DA2E55),                /* == 10^-37 */
    U64(0xAA242499, 0x697392D2), U64(0xDDE50BD1, 0xD5D0B9EA),                /* == 10^-36 */
    U64(0xD4AD2DBF, 0xC3D07787), U64(0x955E4EC6, 0x4B44E864),                /* == 10^-35 */
    U64(0x84EC3C97, 0xDA624AB4), U64(0xBD5AF13B, 0xEF0B113F),                /* == 10^-34 */
    U64(0xA6274BBD, 0xD0FADD61), U64(0xECB1AD8A, 0xEACDD58E),                /* == 10^-33 */
    U64(0xCFB11EAD, 0x453994BA), U64(0x67DE18ED, 0xA5814AF2),                /* == 10^-32 */
    U64(0x81CEB32C, 0x4B43FCF4), U64(0x80EACF94, 0x8770CED7),                /* == 10^-31 */
    U64(0xA2425FF7, 0x5E14FC31), U64(0xA1258379, 0xA94D028D),                /* == 10^-30 */
    U64(0xCAD2F7F5, 0x359A3B3E), U64(0x096EE458, 0x13A04330),                /* == 10^-29 */
    U64(0xFD87B5F2, 0x8300CA0D), U64(0x8BCA9D6E, 0x188853FC),                /* == 10^-28 */
    U64(0x9E74D1B7, 0x91E07E48), U64(0x775EA264, 0xCF55347E),                /* == 10^-27 */
    U64(0xC6120625, 0x76589DDA), U64(0x95364AFE, 0x032A819D),                /* == 10^-26 */
    U64(0xF79687AE, 0xD3EEC551), U64(0x3A83DDBD, 0x83F52205),                /* == 10^-25 */
    U64(0x9ABE14CD, 0x44753B52), U64(0xC4926A96, 0x72793543),                /* == 10^-24 */
    U64(0xC16D9A00, 0x95928A27), U64(0x75B7053C, 0x0F178294),                /* == 10^-23 */
    U64(0xF1C90080, 0xBAF72CB1), U64(0x5324C68B, 0x12DD6338),                /* == 10^-22 */
    U64(0x971DA050, 0x74DA7BEE), U64(0xD3F6FC16, 0xEBCA5E03),                /* == 10^-21 */
    U64(0xBCE50864, 0x92111AEA), U64(0x88F4BB1C, 0xA6BCF584),                /* == 10^-20 */
    U64(0xEC1E4A7D, 0xB69561A5), U64(0x2B31E9E3, 0xD06C32E5),                /* == 10^-19 */
    U64(0x9392EE8E, 0x921D5D07), U64(0x3AFF322E, 0x62439FCF),                /* == 10^-18 */
    U64(0xB877AA32, 0x36A4B449), U64(0x09BEFEB9, 0xFAD487C3),                /* == 10^-17 */
    U64(0xE69594BE, 0xC44DE15B), U64(0x4C2EBE68, 0x7989A9B4),                /* == 10^-16 */
    U64(0x901D7CF7, 0x3AB0ACD9), U64(0x0F9D3701, 0x4BF60A10),                /* == 10^-15 */
    U64(0xB424DC35, 0x095CD80F), U64(0x538484C1, 0x9EF38C94),                /* == 10^-14 */
    U64(0xE12E1342, 0x4BB40E13), U64(0x2865A5F2, 0x06B06FBA),                /* == 10^-13 */
    U64(0x8CBCCC09, 0x6F5088CB), U64(0xF93F87B7, 0x442E45D4),                /* == 10^-12 */
    U64(0xAFEBFF0B, 0xCB24AAFE), U64(0xF78F69A5, 0x1539D749),                /* == 10^-11 */
    U64(0xDBE6FECE, 0xBDEDD5BE), U64(0xB573440E, 0x5A884D1B),                /* == 10^-10 */
    U64(0x89705F41, 0x36B4A597), U64(0x31680A88, 0xF8953031),                /* == 10^-9 */
    U64(0xABCC7711, 0x8461CEFC), U64(0xFDC20D2B, 0x36BA7C3D),                /* == 10^-8 */
    U64(0xD6BF94D5, 0xE57A42BC), U64(0x3D329076, 0x04691B4D),                /* == 10^-7 */
    U64(0x8637BD05, 0xAF6C69B5), U64(0xA63F9A49, 0xC2C1B110),                /* == 10^-6 */
    U64(0xA7C5AC47, 0x1B478423), U64(0x0FCF80DC, 0x33721D54),                /* == 10^-5 */
    U64(0xD1B71758, 0xE219652B), U64(0xD3C36113, 0x404EA4A9),                /* == 10^-4 */
    U64(0x83126E97, 0x8D4FDF3B), U64(0x645A1CAC, 0x083126E9),                /* == 10^-3 */
    U64(0xA3D70A3D, 0x70A3D70A), U64(0x3D70A3D7, 0x0A3D70A4),                /* == 10^-2 */
    U64(0xCCCCCCCC, 0xCCCCCCCC), U64(0xCCCCCCCC, 0xCCCCCCCD),                /* == 10^-1 */
    U64(0x80000000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^0 */  // no error
    U64(0xA0000000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^1 */
    U64(0xC8000000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^2 */
    U64(0xFA000000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^3 */
    U64(0x9C400000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^4 */
    U64(0xC3500000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^5 */
    U64(0xF4240000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^6 */
    U64(0x98968000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^7 */
    U64(0xBEBC2000, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^8 */
    U64(0xEE6B2800, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^9 */
    U64(0x9502F900, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^10 */
    U64(0xBA43B740, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^11 */
    U64(0xE8D4A510, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^12 */
    U64(0x9184E72A, 0x00000000), U64(0x00000000, 0x00000000),                /* == 10^13 */
    U64(0xB5E620F4, 0x80000000), U64(0x00000000, 0x00000000),                /* == 10^14 */
    U64(0xE35FA931, 0xA0000000), U64(0x00000000, 0x00000000),                /* == 10^15 */
    U64(0x8E1BC9BF, 0x04000000), U64(0x00000000, 0x00000000),                /* == 10^16 */
    U64(0xB1A2BC2E, 0xC5000000), U64(0x00000000, 0x00000000),                /* == 10^17 */
    U64(0xDE0B6B3A, 0x76400000), U64(0x00000000, 0x00000000),                /* == 10^18 */
    U64(0x8AC72304, 0x89E80000), U64(0x00000000, 0x00000000),                /* == 10^19 */
    U64(0xAD78EBC5, 0xAC620000), U64(0x00000000, 0x00000000),                /* == 10^20 */
    U64(0xD8D726B7, 0x177A8000), U64(0x00000000, 0x00000000),                /* == 10^21 */
    U64(0x87867832, 0x6EAC9000), U64(0x00000000, 0x00000000),                /* == 10^22 */
    U64(0xA968163F, 0x0A57B400), U64(0x00000000, 0x00000000),                /* == 10^23 */
    U64(0xD3C21BCE, 0xCCEDA100), U64(0x00000000, 0x00000000),                /* == 10^24 */
    U64(0x84595161, 0x401484A0), U64(0x00000000, 0x00000000),                /* == 10^25 */
    U64(0xA56FA5B9, 0x9019A5C8), U64(0x00000000, 0x00000000),                /* == 10^26 */
    U64(0xCECB8F27, 0xF4200F3A), U64(0x00000000, 0x00000000),                /* == 10^27 */
    U64(0x813F3978, 0xF8940984), U64(0x40000000, 0x00000000),                /* == 10^28 */
    U64(0xA18F07D7, 0x36B90BE5), U64(0x50000000, 0x00000000),                /* == 10^29 */
    U64(0xC9F2C9CD, 0x04674EDE), U64(0xA4000000, 0x00000000),                /* == 10^30 */
    U64(0xFC6F7C40, 0x45812296), U64(0x4D000000, 0x00000000),                /* == 10^31 */
    U64(0x9DC5ADA8, 0x2B70B59D), U64(0xF0200000, 0x00000000),                /* == 10^32 */
    U64(0xC5371912, 0x364CE305), U64(0x6C280000, 0x00000000),                /* == 10^33 */
    U64(0xF684DF56, 0xC3E01BC6), U64(0xC7320000, 0x00000000),                /* == 10^34 */
    U64(0x9A130B96, 0x3A6C115C), U64(0x3C7F4000, 0x00000000),                /* == 10^35 */
    U64(0xC097CE7B, 0xC90715B3), U64(0x4B9F1000, 0x00000000),                /* == 10^36 */
    U64(0xF0BDC21A, 0xBB48DB20), U64(0x1E86D400, 0x00000000),                /* == 10^37 */
    U64(0x96769950, 0xB50D88F4), U64(0x13144480, 0x00000000),                /* == 10^38 */
    U64(0xBC143FA4, 0xE250EB31), U64(0x17D955A0, 0x00000000),                /* == 10^39 */
    U64(0xEB194F8E, 0x1AE525FD), U64(0x5DCFAB08, 0x00000000),                /* == 10^40 */
    U64(0x92EFD1B8, 0xD0CF37BE), U64(0x5AA1CAE5, 0x00000000),                /* == 10^41 */
    U64(0xB7ABC627, 0x050305AD), U64(0xF14A3D9E, 0x40000000),                /* == 10^42 */
    U64(0xE596B7B0, 0xC643C719), U64(0x6D9CCD05, 0xD0000000),                /* == 10^43 */
    U64(0x8F7E32CE, 0x7BEA5C6F), U64(0xE4820023, 0xA2000000),                /* == 10^44 */
    U64(0xB35DBF82, 0x1AE4F38B), U64(0xDDA2802C, 0x8A800000),                /* == 10^45 */
    U64(0xE0352F62, 0xA19E306E), U64(0xD50B2037, 0xAD200000),                /* == 10^46 */
    U64(0x8C213D9D, 0xA502DE45), U64(0x4526F422, 0xCC340000),                /* == 10^47 */
    U64(0xAF298D05, 0x0E4395D6), U64(0x9670B12B, 0x7F410000),                /* == 10^48 */
    U64(0xDAF3F046, 0x51D47B4C), U64(0x3C0CDD76, 0x5F114000),                /* == 10^49 */
    U64(0x88D8762B, 0xF324CD0F), U64(0xA5880A69, 0xFB6AC800),                /* == 10^50 */
    U64(0xAB0E93B6, 0xEFEE0053), U64(0x8EEA0D04, 0x7A457A00),                /* == 10^51 */
    U64(0xD5D238A4, 0xABE98068), U64(0x72A49045, 0x98D6D880),                /* == 10^52 */
    U64(0x85A36366, 0xEB71F041), U64(0x47A6DA2B, 0x7F864750),                /* == 10^53 */
    U64(0xA70C3C40, 0xA64E6C51), U64(0x999090B6, 0x5F67D924),                /* == 10^54 */
    U64(0xD0CF4B50, 0xCFE20765), U64(0xFFF4B4E3, 0xF741CF6D), /* == 10^55 */ // no error
    U64(0x82818F12, 0x81ED449F), U64(0xBFF8F10E, 0x7A8921A4),                /* == 10^56 */
    U64(0xA321F2D7, 0x226895C7), U64(0xAFF72D52, 0x192B6A0D),                /* == 10^57 */
    U64(0xCBEA6F8C, 0xEB02BB39), U64(0x9BF4F8A6, 0x9F764490),                /* == 10^58 */
    U64(0xFEE50B70, 0x25C36A08), U64(0x02F236D0, 0x4753D5B5),                /* == 10^59 */
    U64(0x9F4F2726, 0x179A2245), U64(0x01D76242, 0x2C946591),                /* == 10^60 */
    U64(0xC722F0EF, 0x9D80AAD6), U64(0x424D3AD2, 0xB7B97EF5),                /* == 10^61 */
    U64(0xF8EBAD2B, 0x84E0D58B), U64(0xD2E08987, 0x65A7DEB2),                /* == 10^62 */
    U64(0x9B934C3B, 0x330C8577), U64(0x63CC55F4, 0x9F88EB2F),                /* == 10^63 */
    U64(0xC2781F49, 0xFFCFA6D5), U64(0x3CBF6B71, 0xC76B25FB),                /* == 10^64 */
    U64(0xF316271C, 0x7FC3908A), U64(0x8BEF464E, 0x3945EF7A),                /* == 10^65 */
    U64(0x97EDD871, 0xCFDA3A56), U64(0x97758BF0, 0xE3CBB5AC),                /* == 10^66 */
    U64(0xBDE94E8E, 0x43D0C8EC), U64(0x3D52EEED, 0x1CBEA317),                /* == 10^67 */
    U64(0xED63A231, 0xD4C4FB27), U64(0x4CA7AAA8, 0x63EE4BDD),                /* == 10^68 */
    U64(0x945E455F, 0x24FB1CF8), U64(0x8FE8CAA9, 0x3E74EF6A),                /* == 10^69 */
    U64(0xB975D6B6, 0xEE39E436), U64(0xB3E2FD53, 0x8E122B45),                /* == 10^70 */
    U64(0xE7D34C64, 0xA9C85D44), U64(0x60DBBCA8, 0x7196B616),                /* == 10^71 */
    U64(0x90E40FBE, 0xEA1D3A4A), U64(0xBC8955E9, 0x46FE31CE),                /* == 10^72 */
    U64(0xB51D13AE, 0xA4A488DD), U64(0x6BABAB63, 0x98BDBE41),                /* == 10^73 */
    U64(0xE264589A, 0x4DCDAB14), U64(0xC696963C, 0x7EED2DD2),                /* == 10^74 */
    U64(0x8D7EB760, 0x70A08AEC), U64(0xFC1E1DE5, 0xCF543CA3),                /* == 10^75 */
    U64(0xB0DE6538, 0x8CC8ADA8), U64(0x3B25A55F, 0x43294BCC),                /* == 10^76 */
    U64(0xDD15FE86, 0xAFFAD912), U64(0x49EF0EB7, 0x13F39EBF),                /* == 10^77 */
    U64(0x8A2DBF14, 0x2DFCC7AB), U64(0x6E356932, 0x6C784337),                /* == 10^78 */
    U64(0xACB92ED9, 0x397BF996), U64(0x49C2C37F, 0x07965405),                /* == 10^79 */
    U64(0xD7E77A8F, 0x87DAF7FB), U64(0xDC33745E, 0xC97BE906),                /* == 10^80 */
    U64(0x86F0AC99, 0xB4E8DAFD), U64(0x69A028BB, 0x3DED71A4),                /* == 10^81 */
    U64(0xA8ACD7C0, 0x222311BC), U64(0xC40832EA, 0x0D68CE0D),                /* == 10^82 */
    U64(0xD2D80DB0, 0x2AABD62B), U64(0xF50A3FA4, 0x90C30190),                /* == 10^83 */
    U64(0x83C7088E, 0x1AAB65DB), U64(0x792667C6, 0xDA79E0FA),                /* == 10^84 */
    U64(0xA4B8CAB1, 0xA1563F52), U64(0x577001B8, 0x91185939),                /* == 10^85 */
    U64(0xCDE6FD5E, 0x09ABCF26), U64(0xED4C0226, 0xB55E6F87),                /* == 10^86 */
    U64(0x80B05E5A, 0xC60B6178), U64(0x544F8158, 0x315B05B4),                /* == 10^87 */
    U64(0xA0DC75F1, 0x778E39D6), U64(0x696361AE, 0x3DB1C721),                /* == 10^88 */
    U64(0xC913936D, 0xD571C84C), U64(0x03BC3A19, 0xCD1E38EA),                /* == 10^89 */
    U64(0xFB587849, 0x4ACE3A5F), U64(0x04AB48A0, 0x4065C724),                /* == 10^90 */
    U64(0x9D174B2D, 0xCEC0E47B), U64(0x62EB0D64, 0x283F9C76),                /* == 10^91 */
    U64(0xC45D1DF9, 0x42711D9A), U64(0x3BA5D0BD, 0x324F8394),                /* == 10^92 */
    U64(0xF5746577, 0x930D6500), U64(0xCA8F44EC, 0x7EE36479),                /* == 10^93 */
    U64(0x9968BF6A, 0xBBE85F20), U64(0x7E998B13, 0xCF4E1ECC),                /* == 10^94 */
    U64(0xBFC2EF45, 0x6AE276E8), U64(0x9E3FEDD8, 0xC321A67F),                /* == 10^95 */
    U64(0xEFB3AB16, 0xC59B14A2), U64(0xC5CFE94E, 0xF3EA101E),                /* == 10^96 */
    U64(0x95D04AEE, 0x3B80ECE5), U64(0xBBA1F1D1, 0x58724A13),                /* == 10^97 */
    U64(0xBB445DA9, 0xCA61281F), U64(0x2A8A6E45, 0xAE8EDC98),                /* == 10^98 */
    U64(0xEA157514, 0x3CF97226), U64(0xF52D09D7, 0x1A3293BE),                /* == 10^99 */
    U64(0x924D692C, 0xA61BE758), U64(0x593C2626, 0x705F9C56),                /* == 10^100 */
    U64(0xB6E0C377, 0xCFA2E12E), U64(0x6F8B2FB0, 0x0C77836C),                /* == 10^101 */
    U64(0xE498F455, 0xC38B997A), U64(0x0B6DFB9C, 0x0F956447),                /* == 10^102 */
    U64(0x8EDF98B5, 0x9A373FEC), U64(0x4724BD41, 0x89BD5EAC),                /* == 10^103 */
    U64(0xB2977EE3, 0x00C50FE7), U64(0x58EDEC91, 0xEC2CB658),                /* == 10^104 */
    U64(0xDF3D5E9B, 0xC0F653E1), U64(0x2F2967B6, 0x6737E3ED),                /* == 10^105 */
    U64(0x8B865B21, 0x5899F46C), U64(0xBD79E0D2, 0x0082EE74),                /* == 10^106 */
    U64(0xAE67F1E9, 0xAEC07187), U64(0xECD85906, 0x80A3AA11),                /* == 10^107 */
    U64(0xDA01EE64, 0x1A708DE9), U64(0xE80E6F48, 0x20CC9496),                /* == 10^108 */
    U64(0x884134FE, 0x908658B2), U64(0x3109058D, 0x147FDCDE),                /* == 10^109 */
    U64(0xAA51823E, 0x34A7EEDE), U64(0xBD4B46F0, 0x599FD415),                /* == 10^110 */
    U64(0xD4E5E2CD, 0xC1D1EA96), U64(0x6C9E18AC, 0x7007C91A),                /* == 10^111 */
    U64(0x850FADC0, 0x9923329E), U64(0x03E2CF6B, 0xC604DDB0),                /* == 10^112 */
    U64(0xA6539930, 0xBF6BFF45), U64(0x84DB8346, 0xB786151D),                /* == 10^113 */
    U64(0xCFE87F7C, 0xEF46FF16), U64(0xE6126418, 0x65679A64),                /* == 10^114 */
    U64(0x81F14FAE, 0x158C5F6E), U64(0x4FCB7E8F, 0x3F60C07E),                /* == 10^115 */
    U64(0xA26DA399, 0x9AEF7749), U64(0xE3BE5E33, 0x0F38F09E),                /* == 10^116 */
    U64(0xCB090C80, 0x01AB551C), U64(0x5CADF5BF, 0xD3072CC5),                /* == 10^117 */
    U64(0xFDCB4FA0, 0x02162A63), U64(0x73D9732F, 0xC7C8F7F7),                /* == 10^118 */
    U64(0x9E9F11C4, 0x014DDA7E), U64(0x2867E7FD, 0xDCDD9AFA),                /* == 10^119 */
    U64(0xC646D635, 0x01A1511D), U64(0xB281E1FD, 0x541501B9),                /* == 10^120 */
    U64(0xF7D88BC2, 0x4209A565), U64(0x1F225A7C, 0xA91A4227),                /* == 10^121 */
    U64(0x9AE75759, 0x6946075F), U64(0x3375788D, 0xE9B06958),                /* == 10^122 */
    U64(0xC1A12D2F, 0xC3978937), U64(0x0052D6B1, 0x641C83AE),                /* == 10^123 */
    U64(0xF209787B, 0xB47D6B84), U64(0xC0678C5D, 0xBD23A49A),                /* == 10^124 */
    U64(0x9745EB4D, 0x50CE6332), U64(0xF840B7BA, 0x963646E0),                /* == 10^125 */
    U64(0xBD176620, 0xA501FBFF), U64(0xB650E5A9, 0x3BC3D898),                /* == 10^126 */
    U64(0xEC5D3FA8, 0xCE427AFF), U64(0xA3E51F13, 0x8AB4CEBE),                /* == 10^127 */
    U64(0x93BA47C9, 0x80E98CDF), U64(0xC66F336C, 0x36B10137),                /* == 10^128 */
    U64(0xB8A8D9BB, 0xE123F017), U64(0xB80B0047, 0x445D4185),                /* == 10^129 */
    U64(0xE6D3102A, 0xD96CEC1D), U64(0xA60DC059, 0x157491E6),                /* == 10^130 */
    U64(0x9043EA1A, 0xC7E41392), U64(0x87C89837, 0xAD68DB30),                /* == 10^131 */
    U64(0xB454E4A1, 0x79DD1877), U64(0x29BABE45, 0x98C311FC),                /* == 10^132 */
    U64(0xE16A1DC9, 0xD8545E94), U64(0xF4296DD6, 0xFEF3D67B),                /* == 10^133 */
    U64(0x8CE2529E, 0x2734BB1D), U64(0x1899E4A6, 0x5F58660D),                /* == 10^134 */
    U64(0xB01AE745, 0xB101E9E4), U64(0x5EC05DCF, 0xF72E7F90),                /* == 10^135 */
    U64(0xDC21A117, 0x1D42645D), U64(0x76707543, 0xF4FA1F74),                /* == 10^136 */
    U64(0x899504AE, 0x72497EBA), U64(0x6A06494A, 0x791C53A8),                /* == 10^137 */
    U64(0xABFA45DA, 0x0EDBDE69), U64(0x0487DB9D, 0x17636892),                /* == 10^138 */
    U64(0xD6F8D750, 0x9292D603), U64(0x45A9D284, 0x5D3C42B7),                /* == 10^139 */
    U64(0x865B8692, 0x5B9BC5C2), U64(0x0B8A2392, 0xBA45A9B2),                /* == 10^140 */
    U64(0xA7F26836, 0xF282B732), U64(0x8E6CAC77, 0x68D7141F),                /* == 10^141 */
    U64(0xD1EF0244, 0xAF2364FF), U64(0x3207D795, 0x430CD927),                /* == 10^142 */
    U64(0x8335616A, 0xED761F1F), U64(0x7F44E6BD, 0x49E807B8),                /* == 10^143 */
    U64(0xA402B9C5, 0xA8D3A6E7), U64(0x5F16206C, 0x9C6209A6),                /* == 10^144 */
    U64(0xCD036837, 0x130890A1), U64(0x36DBA887, 0xC37A8C10),                /* == 10^145 */
    U64(0x80222122, 0x6BE55A64), U64(0xC2494954, 0xDA2C978A),                /* == 10^146 */
    U64(0xA02AA96B, 0x06DEB0FD), U64(0xF2DB9BAA, 0x10B7BD6C),                /* == 10^147 */
    U64(0xC83553C5, 0xC8965D3D), U64(0x6F928294, 0x94E5ACC7),                /* == 10^148 */
    U64(0xFA42A8B7, 0x3ABBF48C), U64(0xCB772339, 0xBA1F17F9),                /* == 10^149 */
    U64(0x9C69A972, 0x84B578D7), U64(0xFF2A7604, 0x14536EFC),                /* == 10^150 */
    U64(0xC38413CF, 0x25E2D70D), U64(0xFEF51385, 0x19684ABB),                /* == 10^151 */
    U64(0xF46518C2, 0xEF5B8CD1), U64(0x7EB25866, 0x5FC25D69),                /* == 10^152 */
    U64(0x98BF2F79, 0xD5993802), U64(0xEF2F773F, 0xFBD97A62),                /* == 10^153 */
    U64(0xBEEEFB58, 0x4AFF8603), U64(0xAAFB550F, 0xFACFD8FA),                /* == 10^154 */
    U64(0xEEAABA2E, 0x5DBF6784), U64(0x95BA2A53, 0xF983CF39),                /* == 10^155 */
    U64(0x952AB45C, 0xFA97A0B2), U64(0xDD945A74, 0x7BF26184),                /* == 10^156 */
    U64(0xBA756174, 0x393D88DF), U64(0x94F97111, 0x9AEEF9E4),                /* == 10^157 */
    U64(0xE912B9D1, 0x478CEB17), U64(0x7A37CD56, 0x01AAB85E),                /* == 10^158 */
    U64(0x91ABB422, 0xCCB812EE), U64(0xAC62E055, 0xC10AB33B),                /* == 10^159 */
    U64(0xB616A12B, 0x7FE617AA), U64(0x577B986B, 0x314D6009),                /* == 10^160 */
    U64(0xE39C4976, 0x5FDF9D94), U64(0xED5A7E85, 0xFDA0B80B),                /* == 10^161 */
    U64(0x8E41ADE9, 0xFBEBC27D), U64(0x14588F13, 0xBE847307),                /* == 10^162 */
    U64(0xB1D21964, 0x7AE6B31C), U64(0x596EB2D8, 0xAE258FC9),                /* == 10^163 */
    U64(0xDE469FBD, 0x99A05FE3), U64(0x6FCA5F8E, 0xD9AEF3BB),                /* == 10^164 */
    U64(0x8AEC23D6, 0x80043BEE), U64(0x25DE7BB9, 0x480D5855),                /* == 10^165 */
    U64(0xADA72CCC, 0x20054AE9), U64(0xAF561AA7, 0x9A10AE6A),                /* == 10^166 */
    U64(0xD910F7FF, 0x28069DA4), U64(0x1B2BA151, 0x8094DA05),                /* == 10^167 */
    U64(0x87AA9AFF, 0x79042286), U64(0x90FB44D2, 0xF05D0843),                /* == 10^168 */
    U64(0xA99541BF, 0x57452B28), U64(0x353A1607, 0xAC744A54),                /* == 10^169 */
    U64(0xD3FA922F, 0x2D1675F2), U64(0x42889B89, 0x97915CE9),                /* == 10^170 */
    U64(0x847C9B5D, 0x7C2E09B7), U64(0x69956135, 0xFEBADA11),                /* == 10^171 */
    U64(0xA59BC234, 0xDB398C25), U64(0x43FAB983, 0x7E699096),                /* == 10^172 */
    U64(0xCF02B2C2, 0x1207EF2E), U64(0x94F967E4, 0x5E03F4BB),                /* == 10^173 */
    U64(0x8161AFB9, 0x4B44F57D), U64(0x1D1BE0EE, 0xBAC278F5),                /* == 10^174 */
    U64(0xA1BA1BA7, 0x9E1632DC), U64(0x6462D92A, 0x69731732),                /* == 10^175 */
    U64(0xCA28A291, 0x859BBF93), U64(0x7D7B8F75, 0x03CFDCFF),                /* == 10^176 */
    U64(0xFCB2CB35, 0xE702AF78), U64(0x5CDA7352, 0x44C3D43F),                /* == 10^177 */
    U64(0x9DEFBF01, 0xB061ADAB), U64(0x3A088813, 0x6AFA64A7),                /* == 10^178 */
    U64(0xC56BAEC2, 0x1C7A1916), U64(0x088AAA18, 0x45B8FDD1),                /* == 10^179 */
    U64(0xF6C69A72, 0xA3989F5B), U64(0x8AAD549E, 0x57273D45),                /* == 10^180 */
    U64(0x9A3C2087, 0xA63F6399), U64(0x36AC54E2, 0xF678864B),                /* == 10^181 */
    U64(0xC0CB28A9, 0x8FCF3C7F), U64(0x84576A1B, 0xB416A7DE),                /* == 10^182 */
    U64(0xF0FDF2D3, 0xF3C30B9F), U64(0x656D44A2, 0xA11C51D5),                /* == 10^183 */
    U64(0x969EB7C4, 0x7859E743), U64(0x9F644AE5, 0xA4B1B325),                /* == 10^184 */
    U64(0xBC4665B5, 0x96706114), U64(0x873D5D9F, 0x0DDE1FEF),                /* == 10^185 */
    U64(0xEB57FF22, 0xFC0C7959), U64(0xA90CB506, 0xD155A7EA),                /* == 10^186 */
    U64(0x9316FF75, 0xDD87CBD8), U64(0x09A7F124, 0x42D588F3),                /* == 10^187 */
    U64(0xB7DCBF53, 0x54E9BECE), U64(0x0C11ED6D, 0x538AEB2F),                /* == 10^188 */
    U64(0xE5D3EF28, 0x2A242E81), U64(0x8F1668C8, 0xA86DA5FB),                /* == 10^189 */
    U64(0x8FA47579, 0x1A569D10), U64(0xF96E017D, 0x694487BD),                /* == 10^190 */
    U64(0xB38D92D7, 0x60EC4455), U64(0x37C981DC, 0xC395A9AC),                /* == 10^191 */
    U64(0xE070F78D, 0x3927556A), U64(0x85BBE253, 0xF47B1417),                /* == 10^192 */
    U64(0x8C469AB8, 0x43B89562), U64(0x93956D74, 0x78CCEC8E),                /* == 10^193 */
    U64(0xAF584166, 0x54A6BABB), U64(0x387AC8D1, 0x970027B2),                /* == 10^194 */
    U64(0xDB2E51BF, 0xE9D0696A), U64(0x06997B05, 0xFCC0319F),                /* == 10^195 */
    U64(0x88FCF317, 0xF22241E2), U64(0x441FECE3, 0xBDF81F03),                /* == 10^196 */
    U64(0xAB3C2FDD, 0xEEAAD25A), U64(0xD527E81C, 0xAD7626C4),                /* == 10^197 */
    U64(0xD60B3BD5, 0x6A5586F1), U64(0x8A71E223, 0xD8D3B075),                /* == 10^198 */
    U64(0x85C70565, 0x62757456), U64(0xF6872D56, 0x67844E49),                /* == 10^199 */
    U64(0xA738C6BE, 0xBB12D16C), U64(0xB428F8AC, 0x016561DB),                /* == 10^200 */
    U64(0xD106F86E, 0x69D785C7), U64(0xE13336D7, 0x01BEBA52),                /* == 10^201 */
    U64(0x82A45B45, 0x0226B39C), U64(0xECC00246, 0x61173473),                /* == 10^202 */
    U64(0xA34D7216, 0x42B06084), U64(0x27F002D7, 0xF95D0190),                /* == 10^203 */
    U64(0xCC20CE9B, 0xD35C78A5), U64(0x31EC038D, 0xF7B441F4),                /* == 10^204 */
    U64(0xFF290242, 0xC83396CE), U64(0x7E670471, 0x75A15271),                /* == 10^205 */
    U64(0x9F79A169, 0xBD203E41), U64(0x0F0062C6, 0xE984D387),                /* == 10^206 */
    U64(0xC75809C4, 0x2C684DD1), U64(0x52C07B78, 0xA3E60868),                /* == 10^207 */
    U64(0xF92E0C35, 0x37826145), U64(0xA7709A56, 0xCCDF8A83),                /* == 10^208 */
    U64(0x9BBCC7A1, 0x42B17CCB), U64(0x88A66076, 0x400BB692),                /* == 10^209 */
    U64(0xC2ABF989, 0x935DDBFE), U64(0x6ACFF893, 0xD00EA436),                /* == 10^210 */
    U64(0xF356F7EB, 0xF83552FE), U64(0x0583F6B8, 0xC4124D43),                /* == 10^211 */
    U64(0x98165AF3, 0x7B2153DE), U64(0xC3727A33, 0x7A8B704A),                /* == 10^212 */
    U64(0xBE1BF1B0, 0x59E9A8D6), U64(0x744F18C0, 0x592E4C5D),                /* == 10^213 */
    U64(0xEDA2EE1C, 0x7064130C), U64(0x1162DEF0, 0x6F79DF74),                /* == 10^214 */
    U64(0x9485D4D1, 0xC63E8BE7), U64(0x8ADDCB56, 0x45AC2BA8),                /* == 10^215 */
    U64(0xB9A74A06, 0x37CE2EE1), U64(0x6D953E2B, 0xD7173693),                /* == 10^216 */
    U64(0xE8111C87, 0xC5C1BA99), U64(0xC8FA8DB6, 0xCCDD0437),                /* == 10^217 */
    U64(0x910AB1D4, 0xDB9914A0), U64(0x1D9C9892, 0x400A22A2),                /* == 10^218 */
    U64(0xB54D5E4A, 0x127F59C8), U64(0x2503BEB6, 0xD00CAB4B),                /* == 10^219 */
    U64(0xE2A0B5DC, 0x971F303A), U64(0x2E44AE64, 0x840FD61E),                /* == 10^220 */
    U64(0x8DA471A9, 0xDE737E24), U64(0x5CEAECFE, 0xD289E5D3),                /* == 10^221 */
    U64(0xB10D8E14, 0x56105DAD), U64(0x7425A83E, 0x872C5F47),                /* == 10^222 */
    U64(0xDD50F199, 0x6B947518), U64(0xD12F124E, 0x28F77719),                /* == 10^223 */
    U64(0x8A5296FF, 0xE33CC92F), U64(0x82BD6B70, 0xD99AAA70),                /* == 10^224 */
    U64(0xACE73CBF, 0xDC0BFB7B), U64(0x636CC64D, 0x1001550C),                /* == 10^225 */
    U64(0xD8210BEF, 0xD30EFA5A), U64(0x3C47F7E0, 0x5401AA4F),                /* == 10^226 */
    U64(0x8714A775, 0xE3E95C78), U64(0x65ACFAEC, 0x34810A71),                /* == 10^227 */
    U64(0xA8D9D153, 0x5CE3B396), U64(0x7F1839A7, 0x41A14D0D),                /* == 10^228 */
    U64(0xD31045A8, 0x341CA07C), U64(0x1EDE4811, 0x1209A051),                /* == 10^229 */
    U64(0x83EA2B89, 0x2091E44D), U64(0x934AED0A, 0xAB460432),                /* == 10^230 */
    U64(0xA4E4B66B, 0x68B65D60), U64(0xF81DA84D, 0x5617853F),                /* == 10^231 */
    U64(0xCE1DE406, 0x42E3F4B9), U64(0x36251260, 0xAB9D668F),                /* == 10^232 */
    U64(0x80D2AE83, 0xE9CE78F3), U64(0xC1D72B7C, 0x6B426019),                /* == 10^233 */
    U64(0xA1075A24, 0xE4421730), U64(0xB24CF65B, 0x8612F820),                /* == 10^234 */
    U64(0xC94930AE, 0x1D529CFC), U64(0xDEE033F2, 0x6797B628),                /* == 10^235 */
    U64(0xFB9B7CD9, 0xA4A7443C), U64(0x169840EF, 0x017DA3B1),                /* == 10^236 */
    U64(0x9D412E08, 0x06E88AA5), U64(0x8E1F2895, 0x60EE864F),                /* == 10^237 */
    U64(0xC491798A, 0x08A2AD4E), U64(0xF1A6F2BA, 0xB92A27E3),                /* == 10^238 */
    U64(0xF5B5D7EC, 0x8ACB58A2), U64(0xAE10AF69, 0x6774B1DB),                /* == 10^239 */
    U64(0x9991A6F3, 0xD6BF1765), U64(0xACCA6DA1, 0xE0A8EF29),                /* == 10^240 */
    U64(0xBFF610B0, 0xCC6EDD3F), U64(0x17FD090A, 0x58D32AF3),                /* == 10^241 */
    U64(0xEFF394DC, 0xFF8A948E), U64(0xDDFC4B4C, 0xEF07F5B0),                /* == 10^242 */
    U64(0x95F83D0A, 0x1FB69CD9), U64(0x4ABDAF10, 0x1564F98E),                /* == 10^243 */
    U64(0xBB764C4C, 0xA7A4440F), U64(0x9D6D1AD4, 0x1ABE37F2),                /* == 10^244 */
    U64(0xEA53DF5F, 0xD18D5513), U64(0x84C86189, 0x216DC5EE),                /* == 10^245 */
    U64(0x92746B9B, 0xE2F8552C), U64(0x32FD3CF5, 0xB4E49BB5),                /* == 10^246 */
    U64(0xB7118682, 0xDBB66A77), U64(0x3FBC8C33, 0x221DC2A2),                /* == 10^247 */
    U64(0xE4D5E823, 0x92A40515), U64(0x0FABAF3F, 0xEAA5334A),                /* == 10^248 */
    U64(0x8F05B116, 0x3BA6832D), U64(0x29CB4D87, 0xF2A7400E),                /* == 10^249 */
    U64(0xB2C71D5B, 0xCA9023F8), U64(0x743E20E9, 0xEF511012),                /* == 10^250 */
    U64(0xDF78E4B2, 0xBD342CF6), U64(0x914DA924, 0x6B255417),                /* == 10^251 */
    U64(0x8BAB8EEF, 0xB6409C1A), U64(0x1AD089B6, 0xC2F7548E),                /* == 10^252 */
    U64(0xAE9672AB, 0xA3D0C320), U64(0xA184AC24, 0x73B529B2),                /* == 10^253 */
    U64(0xDA3C0F56, 0x8CC4F3E8), U64(0xC9E5D72D, 0x90A2741E),                /* == 10^254 */
    U64(0x88658996, 0x17FB1871), U64(0x7E2FA67C, 0x7A658893),                /* == 10^255 */
    U64(0xAA7EEBFB, 0x9DF9DE8D), U64(0xDDBB901B, 0x98FEEAB8),                /* == 10^256 */
    U64(0xD51EA6FA, 0x85785631), U64(0x552A7422, 0x7F3EA565),                /* == 10^257 */
    U64(0x8533285C, 0x936B35DE), U64(0xD53A8895, 0x8F87275F),                /* == 10^258 */
    U64(0xA67FF273, 0xB8460356), U64(0x8A892ABA, 0xF368F137),                /* == 10^259 */
    U64(0xD01FEF10, 0xA657842C), U64(0x2D2B7569, 0xB0432D85),                /* == 10^260 */
    U64(0x8213F56A, 0x67F6B29B), U64(0x9C3B2962, 0x0E29FC73),                /* == 10^261 */
    U64(0xA298F2C5, 0x01F45F42), U64(0x8349F3BA, 0x91B47B90),                /* == 10^262 */
    U64(0xCB3F2F76, 0x42717713), U64(0x241C70A9, 0x36219A74),                /* == 10^263 */
    U64(0xFE0EFB53, 0xD30DD4D7), U64(0xED238CD3, 0x83AA0111),                /* == 10^264 */
    U64(0x9EC95D14, 0x63E8A506), U64(0xF4363804, 0x324A40AB),                /* == 10^265 */
    U64(0xC67BB459, 0x7CE2CE48), U64(0xB143C605, 0x3EDCD0D5),                /* == 10^266 */
    U64(0xF81AA16F, 0xDC1B81DA), U64(0xDD94B786, 0x8E94050A),                /* == 10^267 */
    U64(0x9B10A4E5, 0xE9913128), U64(0xCA7CF2B4, 0x191C8327),                /* == 10^268 */
    U64(0xC1D4CE1F, 0x63F57D72), U64(0xFD1C2F61, 0x1F63A3F0),                /* == 10^269 */
    U64(0xF24A01A7, 0x3CF2DCCF), U64(0xBC633B39, 0x673C8CEC),                /* == 10^270 */
    U64(0x976E4108, 0x8617CA01), U64(0xD5BE0503, 0xE085D814),                /* == 10^271 */
    U64(0xBD49D14A, 0xA79DBC82), U64(0x4B2D8644, 0xD8A74E19),                /* == 10^272 */
    U64(0xEC9C459D, 0x51852BA2), U64(0xDDF8E7D6, 0x0ED1219F),                /* == 10^273 */
    U64(0x93E1AB82, 0x52F33B45), U64(0xCABB90E5, 0xC942B503),                /* == 10^274 */
    U64(0xB8DA1662, 0xE7B00A17), U64(0x3D6A751F, 0x3B936244),                /* == 10^275 */
    U64(0xE7109BFB, 0xA19C0C9D), U64(0x0CC51267, 0x0A783AD5),                /* == 10^276 */
    U64(0x906A617D, 0x450187E2), U64(0x27FB2B80, 0x668B24C5),                /* == 10^277 */
    U64(0xB484F9DC, 0x9641E9DA), U64(0xB1F9F660, 0x802DEDF6),                /* == 10^278 */
    U64(0xE1A63853, 0xBBD26451), U64(0x5E7873F8, 0xA0396974),                /* == 10^279 */
    U64(0x8D07E334, 0x55637EB2), U64(0xDB0B487B, 0x6423E1E8),                /* == 10^280 */
    U64(0xB049DC01, 0x6ABC5E5F), U64(0x91CE1A9A, 0x3D2CDA63),                /* == 10^281 */
    U64(0xDC5C5301, 0xC56B75F7), U64(0x7641A140, 0xCC7810FB),                /* == 10^282 */
    U64(0x89B9B3E1, 0x1B6329BA), U64(0xA9E904C8, 0x7FCB0A9D),                /* == 10^283 */
    U64(0xAC2820D9, 0x623BF429), U64(0x546345FA, 0x9FBDCD44),                /* == 10^284 */
    U64(0xD732290F, 0xBACAF133), U64(0xA97C1779, 0x47AD4095),                /* == 10^285 */
    U64(0x867F59A9, 0xD4BED6C0), U64(0x49ED8EAB, 0xCCCC485D),                /* == 10^286 */
    U64(0xA81F3014, 0x49EE8C70), U64(0x5C68F256, 0xBFFF5A75),                /* == 10^287 */
    U64(0xD226FC19, 0x5C6A2F8C), U64(0x73832EEC, 0x6FFF3112),                /* == 10^288 */
    U64(0x83585D8F, 0xD9C25DB7), U64(0xC831FD53, 0xC5FF7EAB),                /* == 10^289 */
    U64(0xA42E74F3, 0xD032F525), U64(0xBA3E7CA8, 0xB77F5E56),                /* == 10^290 */
    U64(0xCD3A1230, 0xC43FB26F), U64(0x28CE1BD2, 0xE55F35EB),                /* == 10^291 */
    U64(0x80444B5E, 0x7AA7CF85), U64(0x7980D163, 0xCF5B81B3),                /* == 10^292 */
    U64(0xA0555E36, 0x1951C366), U64(0xD7E105BC, 0xC3326220),                /* == 10^293 */
    U64(0xC86AB5C3, 0x9FA63440), U64(0x8DD9472B, 0xF3FEFAA8),                /* == 10^294 */
    U64(0xFA856334, 0x878FC150), U64(0xB14F98F6, 0xF0FEB952),                /* == 10^295 */
    U64(0x9C935E00, 0xD4B9D8D2), U64(0x6ED1BF9A, 0x569F33D3),                /* == 10^296 */
    U64(0xC3B83581, 0x09E84F07), U64(0x0A862F80, 0xEC4700C8),                /* == 10^297 */
    U64(0xF4A642E1, 0x4C6262C8), U64(0xCD27BB61, 0x2758C0FA),                /* == 10^298 */
    U64(0x98E7E9CC, 0xCFBD7DBD), U64(0x8038D51C, 0xB897789C),                /* == 10^299 */
    U64(0xBF21E440, 0x03ACDD2C), U64(0xE0470A63, 0xE6BD56C3),                /* == 10^300 */
    U64(0xEEEA5D50, 0x04981478), U64(0x1858CCFC, 0xE06CAC74),                /* == 10^301 */
    U64(0x95527A52, 0x02DF0CCB), U64(0x0F37801E, 0x0C43EBC9),                /* == 10^302 */
    U64(0xBAA718E6, 0x8396CFFD), U64(0xD3056025, 0x8F54E6BB),                /* == 10^303 */
    U64(0xE950DF20, 0x247C83FD), U64(0x47C6B82E, 0xF32A2069),                /* == 10^304 */
    U64(0x91D28B74, 0x16CDD27E), U64(0x4CDC331D, 0x57FA5442),                /* == 10^305 */
    U64(0xB6472E51, 0x1C81471D), U64(0xE0133FE4, 0xADF8E952),                /* == 10^306 */
    U64(0xE3D8F9E5, 0x63A198E5), U64(0x58180FDD, 0xD97723A7),                /* == 10^307 */
    U64(0x8E679C2F, 0x5E44FF8F), U64(0x570F09EA, 0xA7EA7648),                /* == 10^308 */
    U64(0xB201833B, 0x35D63F73), U64(0x2CD2CC65, 0x51E513DA),                /* == 10^309 */
    U64(0xDE81E40A, 0x034BCF4F), U64(0xF8077F7E, 0xA65E58D1),                /* == 10^310 */
    U64(0x8B112E86, 0x420F6191), U64(0xFB04AFAF, 0x27FAF783),                /* == 10^311 */
    U64(0xADD57A27, 0xD29339F6), U64(0x79C5DB9A, 0xF1F9B563),                /* == 10^312 */
    U64(0xD94AD8B1, 0xC7380874), U64(0x18375281, 0xAE7822BC),                /* == 10^313 */
    U64(0x87CEC76F, 0x1C830548), U64(0x8F229391, 0x0D0B15B6),                /* == 10^314 */
    U64(0xA9C2794A, 0xE3A3C69A), U64(0xB2EB3875, 0x504DDB23),                /* == 10^315 */
    U64(0xD433179D, 0x9C8CB841), U64(0x5FA60692, 0xA46151EC),                /* == 10^316 */
    U64(0x849FEEC2, 0x81D7F328), U64(0xDBC7C41B, 0xA6BCD333),                /* == 10^317 */
    U64(0xA5C7EA73, 0x224DEFF3), U64(0x12B9B522, 0x906C0800),                /* == 10^318 */
    U64(0xCF39E50F, 0xEAE16BEF), U64(0xD768226B, 0x34870A00),                /* == 10^319 */
    U64(0x81842F29, 0xF2CCE375), U64(0xE6A11583, 0x00D46640),                /* == 10^320 */
    U64(0xA1E53AF4, 0x6F801C53), U64(0x60495AE3, 0xC1097FD0),                /* == 10^321 */
    U64(0xCA5E89B1, 0x8B602368), U64(0x385BB19C, 0xB14BDFC4),                /* == 10^322 */
    U64(0xFCF62C1D, 0xEE382C42), U64(0x46729E03, 0xDD9ED7B5),                /* == 10^323 */
    U64(0x9E19DB92, 0xB4E31BA9), U64(0x6C07A2C2, 0x6A8346D1)                 /* == 10^324 */
};

static const u64 pow10_sig_table_round_reverse[]={
U64(0x9E19DB92, 0xB4E31BA9), U64(0x6C07A2C2, 0x6A8346D1),/* == 10^324 */
U64(0xFCF62C1D, 0xEE382C42), U64(0x46729E03, 0xDD9ED7B5),/* == 10^323 */
U64(0xCA5E89B1, 0x8B602368), U64(0x385BB19C, 0xB14BDFC4),/* == 10^322 */
U64(0xA1E53AF4, 0x6F801C53), U64(0x60495AE3, 0xC1097FD0),/* == 10^321 */
U64(0x81842F29, 0xF2CCE375), U64(0xE6A11583, 0x00D46640),/* == 10^320 */
U64(0xCF39E50F, 0xEAE16BEF), U64(0xD768226B, 0x34870A00),/* == 10^319 */
U64(0xA5C7EA73, 0x224DEFF3), U64(0x12B9B522, 0x906C0800),/* == 10^318 */
U64(0x849FEEC2, 0x81D7F328), U64(0xDBC7C41B, 0xA6BCD333),/* == 10^317 */
U64(0xD433179D, 0x9C8CB841), U64(0x5FA60692, 0xA46151EC),/* == 10^316 */
U64(0xA9C2794A, 0xE3A3C69A), U64(0xB2EB3875, 0x504DDB23),/* == 10^315 */
U64(0x87CEC76F, 0x1C830548), U64(0x8F229391, 0x0D0B15B6),/* == 10^314 */
U64(0xD94AD8B1, 0xC7380874), U64(0x18375281, 0xAE7822BC),/* == 10^313 */
U64(0xADD57A27, 0xD29339F6), U64(0x79C5DB9A, 0xF1F9B563),/* == 10^312 */
U64(0x8B112E86, 0x420F6191), U64(0xFB04AFAF, 0x27FAF783),/* == 10^311 */
U64(0xDE81E40A, 0x034BCF4F), U64(0xF8077F7E, 0xA65E58D1),/* == 10^310 */
U64(0xB201833B, 0x35D63F73), U64(0x2CD2CC65, 0x51E513DA),/* == 10^309 */
U64(0x8E679C2F, 0x5E44FF8F), U64(0x570F09EA, 0xA7EA7648),/* == 10^308 */
U64(0xE3D8F9E5, 0x63A198E5), U64(0x58180FDD, 0xD97723A7),/* == 10^307 */
U64(0xB6472E51, 0x1C81471D), U64(0xE0133FE4, 0xADF8E952),/* == 10^306 */
U64(0x91D28B74, 0x16CDD27E), U64(0x4CDC331D, 0x57FA5442),/* == 10^305 */
U64(0xE950DF20, 0x247C83FD), U64(0x47C6B82E, 0xF32A2069),/* == 10^304 */
U64(0xBAA718E6, 0x8396CFFD), U64(0xD3056025, 0x8F54E6BB),/* == 10^303 */
U64(0x95527A52, 0x02DF0CCB), U64(0x0F37801E, 0x0C43EBC9),/* == 10^302 */
U64(0xEEEA5D50, 0x04981478), U64(0x1858CCFC, 0xE06CAC74),/* == 10^301 */
U64(0xBF21E440, 0x03ACDD2C), U64(0xE0470A63, 0xE6BD56C3),/* == 10^300 */
U64(0x98E7E9CC, 0xCFBD7DBD), U64(0x8038D51C, 0xB897789C),/* == 10^299 */
U64(0xF4A642E1, 0x4C6262C8), U64(0xCD27BB61, 0x2758C0FA),/* == 10^298 */
U64(0xC3B83581, 0x09E84F07), U64(0x0A862F80, 0xEC4700C8),/* == 10^297 */
U64(0x9C935E00, 0xD4B9D8D2), U64(0x6ED1BF9A, 0x569F33D3),/* == 10^296 */
U64(0xFA856334, 0x878FC150), U64(0xB14F98F6, 0xF0FEB952),/* == 10^295 */
U64(0xC86AB5C3, 0x9FA63440), U64(0x8DD9472B, 0xF3FEFAA8),/* == 10^294 */
U64(0xA0555E36, 0x1951C366), U64(0xD7E105BC, 0xC3326220),/* == 10^293 */
U64(0x80444B5E, 0x7AA7CF85), U64(0x7980D163, 0xCF5B81B3),/* == 10^292 */
U64(0xCD3A1230, 0xC43FB26F), U64(0x28CE1BD2, 0xE55F35EB),/* == 10^291 */
U64(0xA42E74F3, 0xD032F525), U64(0xBA3E7CA8, 0xB77F5E56),/* == 10^290 */
U64(0x83585D8F, 0xD9C25DB7), U64(0xC831FD53, 0xC5FF7EAB),/* == 10^289 */
U64(0xD226FC19, 0x5C6A2F8C), U64(0x73832EEC, 0x6FFF3112),/* == 10^288 */
U64(0xA81F3014, 0x49EE8C70), U64(0x5C68F256, 0xBFFF5A75),/* == 10^287 */
U64(0x867F59A9, 0xD4BED6C0), U64(0x49ED8EAB, 0xCCCC485D),/* == 10^286 */
U64(0xD732290F, 0xBACAF133), U64(0xA97C1779, 0x47AD4095),/* == 10^285 */
U64(0xAC2820D9, 0x623BF429), U64(0x546345FA, 0x9FBDCD44),/* == 10^284 */
U64(0x89B9B3E1, 0x1B6329BA), U64(0xA9E904C8, 0x7FCB0A9D),/* == 10^283 */
U64(0xDC5C5301, 0xC56B75F7), U64(0x7641A140, 0xCC7810FB),/* == 10^282 */
U64(0xB049DC01, 0x6ABC5E5F), U64(0x91CE1A9A, 0x3D2CDA63),/* == 10^281 */
U64(0x8D07E334, 0x55637EB2), U64(0xDB0B487B, 0x6423E1E8),/* == 10^280 */
U64(0xE1A63853, 0xBBD26451), U64(0x5E7873F8, 0xA0396974),/* == 10^279 */
U64(0xB484F9DC, 0x9641E9DA), U64(0xB1F9F660, 0x802DEDF6),/* == 10^278 */
U64(0x906A617D, 0x450187E2), U64(0x27FB2B80, 0x668B24C5),/* == 10^277 */
U64(0xE7109BFB, 0xA19C0C9D), U64(0x0CC51267, 0x0A783AD5),/* == 10^276 */
U64(0xB8DA1662, 0xE7B00A17), U64(0x3D6A751F, 0x3B936244),/* == 10^275 */
U64(0x93E1AB82, 0x52F33B45), U64(0xCABB90E5, 0xC942B503),/* == 10^274 */
U64(0xEC9C459D, 0x51852BA2), U64(0xDDF8E7D6, 0x0ED1219F),/* == 10^273 */
U64(0xBD49D14A, 0xA79DBC82), U64(0x4B2D8644, 0xD8A74E19),/* == 10^272 */
U64(0x976E4108, 0x8617CA01), U64(0xD5BE0503, 0xE085D814),/* == 10^271 */
U64(0xF24A01A7, 0x3CF2DCCF), U64(0xBC633B39, 0x673C8CEC),/* == 10^270 */
U64(0xC1D4CE1F, 0x63F57D72), U64(0xFD1C2F61, 0x1F63A3F0),/* == 10^269 */
U64(0x9B10A4E5, 0xE9913128), U64(0xCA7CF2B4, 0x191C8327),/* == 10^268 */
U64(0xF81AA16F, 0xDC1B81DA), U64(0xDD94B786, 0x8E94050A),/* == 10^267 */
U64(0xC67BB459, 0x7CE2CE48), U64(0xB143C605, 0x3EDCD0D5),/* == 10^266 */
U64(0x9EC95D14, 0x63E8A506), U64(0xF4363804, 0x324A40AB),/* == 10^265 */
U64(0xFE0EFB53, 0xD30DD4D7), U64(0xED238CD3, 0x83AA0111),/* == 10^264 */
U64(0xCB3F2F76, 0x42717713), U64(0x241C70A9, 0x36219A74),/* == 10^263 */
U64(0xA298F2C5, 0x01F45F42), U64(0x8349F3BA, 0x91B47B90),/* == 10^262 */
U64(0x8213F56A, 0x67F6B29B), U64(0x9C3B2962, 0x0E29FC73),/* == 10^261 */
U64(0xD01FEF10, 0xA657842C), U64(0x2D2B7569, 0xB0432D85),/* == 10^260 */
U64(0xA67FF273, 0xB8460356), U64(0x8A892ABA, 0xF368F137),/* == 10^259 */
U64(0x8533285C, 0x936B35DE), U64(0xD53A8895, 0x8F87275F),/* == 10^258 */
U64(0xD51EA6FA, 0x85785631), U64(0x552A7422, 0x7F3EA565),/* == 10^257 */
U64(0xAA7EEBFB, 0x9DF9DE8D), U64(0xDDBB901B, 0x98FEEAB8),/* == 10^256 */
U64(0x88658996, 0x17FB1871), U64(0x7E2FA67C, 0x7A658893),/* == 10^255 */
U64(0xDA3C0F56, 0x8CC4F3E8), U64(0xC9E5D72D, 0x90A2741E),/* == 10^254 */
U64(0xAE9672AB, 0xA3D0C320), U64(0xA184AC24, 0x73B529B2),/* == 10^253 */
U64(0x8BAB8EEF, 0xB6409C1A), U64(0x1AD089B6, 0xC2F7548E),/* == 10^252 */
U64(0xDF78E4B2, 0xBD342CF6), U64(0x914DA924, 0x6B255417),/* == 10^251 */
U64(0xB2C71D5B, 0xCA9023F8), U64(0x743E20E9, 0xEF511012),/* == 10^250 */
U64(0x8F05B116, 0x3BA6832D), U64(0x29CB4D87, 0xF2A7400E),/* == 10^249 */
U64(0xE4D5E823, 0x92A40515), U64(0x0FABAF3F, 0xEAA5334A),/* == 10^248 */
U64(0xB7118682, 0xDBB66A77), U64(0x3FBC8C33, 0x221DC2A2),/* == 10^247 */
U64(0x92746B9B, 0xE2F8552C), U64(0x32FD3CF5, 0xB4E49BB5),/* == 10^246 */
U64(0xEA53DF5F, 0xD18D5513), U64(0x84C86189, 0x216DC5EE),/* == 10^245 */
U64(0xBB764C4C, 0xA7A4440F), U64(0x9D6D1AD4, 0x1ABE37F2),/* == 10^244 */
U64(0x95F83D0A, 0x1FB69CD9), U64(0x4ABDAF10, 0x1564F98E),/* == 10^243 */
U64(0xEFF394DC, 0xFF8A948E), U64(0xDDFC4B4C, 0xEF07F5B0),/* == 10^242 */
U64(0xBFF610B0, 0xCC6EDD3F), U64(0x17FD090A, 0x58D32AF3),/* == 10^241 */
U64(0x9991A6F3, 0xD6BF1765), U64(0xACCA6DA1, 0xE0A8EF29),/* == 10^240 */
U64(0xF5B5D7EC, 0x8ACB58A2), U64(0xAE10AF69, 0x6774B1DB),/* == 10^239 */
U64(0xC491798A, 0x08A2AD4E), U64(0xF1A6F2BA, 0xB92A27E3),/* == 10^238 */
U64(0x9D412E08, 0x06E88AA5), U64(0x8E1F2895, 0x60EE864F),/* == 10^237 */
U64(0xFB9B7CD9, 0xA4A7443C), U64(0x169840EF, 0x017DA3B1),/* == 10^236 */
U64(0xC94930AE, 0x1D529CFC), U64(0xDEE033F2, 0x6797B628),/* == 10^235 */
U64(0xA1075A24, 0xE4421730), U64(0xB24CF65B, 0x8612F820),/* == 10^234 */
U64(0x80D2AE83, 0xE9CE78F3), U64(0xC1D72B7C, 0x6B426019),/* == 10^233 */
U64(0xCE1DE406, 0x42E3F4B9), U64(0x36251260, 0xAB9D668F),/* == 10^232 */
U64(0xA4E4B66B, 0x68B65D60), U64(0xF81DA84D, 0x5617853F),/* == 10^231 */
U64(0x83EA2B89, 0x2091E44D), U64(0x934AED0A, 0xAB460432),/* == 10^230 */
U64(0xD31045A8, 0x341CA07C), U64(0x1EDE4811, 0x1209A051),/* == 10^229 */
U64(0xA8D9D153, 0x5CE3B396), U64(0x7F1839A7, 0x41A14D0D),/* == 10^228 */
U64(0x8714A775, 0xE3E95C78), U64(0x65ACFAEC, 0x34810A71),/* == 10^227 */
U64(0xD8210BEF, 0xD30EFA5A), U64(0x3C47F7E0, 0x5401AA4F),/* == 10^226 */
U64(0xACE73CBF, 0xDC0BFB7B), U64(0x636CC64D, 0x1001550C),/* == 10^225 */
U64(0x8A5296FF, 0xE33CC92F), U64(0x82BD6B70, 0xD99AAA70),/* == 10^224 */
U64(0xDD50F199, 0x6B947518), U64(0xD12F124E, 0x28F77719),/* == 10^223 */
U64(0xB10D8E14, 0x56105DAD), U64(0x7425A83E, 0x872C5F47),/* == 10^222 */
U64(0x8DA471A9, 0xDE737E24), U64(0x5CEAECFE, 0xD289E5D3),/* == 10^221 */
U64(0xE2A0B5DC, 0x971F303A), U64(0x2E44AE64, 0x840FD61E),/* == 10^220 */
U64(0xB54D5E4A, 0x127F59C8), U64(0x2503BEB6, 0xD00CAB4B),/* == 10^219 */
U64(0x910AB1D4, 0xDB9914A0), U64(0x1D9C9892, 0x400A22A2),/* == 10^218 */
U64(0xE8111C87, 0xC5C1BA99), U64(0xC8FA8DB6, 0xCCDD0437),/* == 10^217 */
U64(0xB9A74A06, 0x37CE2EE1), U64(0x6D953E2B, 0xD7173693),/* == 10^216 */
U64(0x9485D4D1, 0xC63E8BE7), U64(0x8ADDCB56, 0x45AC2BA8),/* == 10^215 */
U64(0xEDA2EE1C, 0x7064130C), U64(0x1162DEF0, 0x6F79DF74),/* == 10^214 */
U64(0xBE1BF1B0, 0x59E9A8D6), U64(0x744F18C0, 0x592E4C5D),/* == 10^213 */
U64(0x98165AF3, 0x7B2153DE), U64(0xC3727A33, 0x7A8B704A),/* == 10^212 */
U64(0xF356F7EB, 0xF83552FE), U64(0x0583F6B8, 0xC4124D43),/* == 10^211 */
U64(0xC2ABF989, 0x935DDBFE), U64(0x6ACFF893, 0xD00EA436),/* == 10^210 */
U64(0x9BBCC7A1, 0x42B17CCB), U64(0x88A66076, 0x400BB692),/* == 10^209 */
U64(0xF92E0C35, 0x37826145), U64(0xA7709A56, 0xCCDF8A83),/* == 10^208 */
U64(0xC75809C4, 0x2C684DD1), U64(0x52C07B78, 0xA3E60868),/* == 10^207 */
U64(0x9F79A169, 0xBD203E41), U64(0x0F0062C6, 0xE984D387),/* == 10^206 */
U64(0xFF290242, 0xC83396CE), U64(0x7E670471, 0x75A15271),/* == 10^205 */
U64(0xCC20CE9B, 0xD35C78A5), U64(0x31EC038D, 0xF7B441F4),/* == 10^204 */
U64(0xA34D7216, 0x42B06084), U64(0x27F002D7, 0xF95D0190),/* == 10^203 */
U64(0x82A45B45, 0x0226B39C), U64(0xECC00246, 0x61173473),/* == 10^202 */
U64(0xD106F86E, 0x69D785C7), U64(0xE13336D7, 0x01BEBA52),/* == 10^201 */
U64(0xA738C6BE, 0xBB12D16C), U64(0xB428F8AC, 0x016561DB),/* == 10^200 */
U64(0x85C70565, 0x62757456), U64(0xF6872D56, 0x67844E49),/* == 10^199 */
U64(0xD60B3BD5, 0x6A5586F1), U64(0x8A71E223, 0xD8D3B075),/* == 10^198 */
U64(0xAB3C2FDD, 0xEEAAD25A), U64(0xD527E81C, 0xAD7626C4),/* == 10^197 */
U64(0x88FCF317, 0xF22241E2), U64(0x441FECE3, 0xBDF81F03),/* == 10^196 */
U64(0xDB2E51BF, 0xE9D0696A), U64(0x06997B05, 0xFCC0319F),/* == 10^195 */
U64(0xAF584166, 0x54A6BABB), U64(0x387AC8D1, 0x970027B2),/* == 10^194 */
U64(0x8C469AB8, 0x43B89562), U64(0x93956D74, 0x78CCEC8E),/* == 10^193 */
U64(0xE070F78D, 0x3927556A), U64(0x85BBE253, 0xF47B1417),/* == 10^192 */
U64(0xB38D92D7, 0x60EC4455), U64(0x37C981DC, 0xC395A9AC),/* == 10^191 */
U64(0x8FA47579, 0x1A569D10), U64(0xF96E017D, 0x694487BD),/* == 10^190 */
U64(0xE5D3EF28, 0x2A242E81), U64(0x8F1668C8, 0xA86DA5FB),/* == 10^189 */
U64(0xB7DCBF53, 0x54E9BECE), U64(0x0C11ED6D, 0x538AEB2F),/* == 10^188 */
U64(0x9316FF75, 0xDD87CBD8), U64(0x09A7F124, 0x42D588F3),/* == 10^187 */
U64(0xEB57FF22, 0xFC0C7959), U64(0xA90CB506, 0xD155A7EA),/* == 10^186 */
U64(0xBC4665B5, 0x96706114), U64(0x873D5D9F, 0x0DDE1FEF),/* == 10^185 */
U64(0x969EB7C4, 0x7859E743), U64(0x9F644AE5, 0xA4B1B325),/* == 10^184 */
U64(0xF0FDF2D3, 0xF3C30B9F), U64(0x656D44A2, 0xA11C51D5),/* == 10^183 */
U64(0xC0CB28A9, 0x8FCF3C7F), U64(0x84576A1B, 0xB416A7DE),/* == 10^182 */
U64(0x9A3C2087, 0xA63F6399), U64(0x36AC54E2, 0xF678864B),/* == 10^181 */
U64(0xF6C69A72, 0xA3989F5B), U64(0x8AAD549E, 0x57273D45),/* == 10^180 */
U64(0xC56BAEC2, 0x1C7A1916), U64(0x088AAA18, 0x45B8FDD1),/* == 10^179 */
U64(0x9DEFBF01, 0xB061ADAB), U64(0x3A088813, 0x6AFA64A7),/* == 10^178 */
U64(0xFCB2CB35, 0xE702AF78), U64(0x5CDA7352, 0x44C3D43F),/* == 10^177 */
U64(0xCA28A291, 0x859BBF93), U64(0x7D7B8F75, 0x03CFDCFF),/* == 10^176 */
U64(0xA1BA1BA7, 0x9E1632DC), U64(0x6462D92A, 0x69731732),/* == 10^175 */
U64(0x8161AFB9, 0x4B44F57D), U64(0x1D1BE0EE, 0xBAC278F5),/* == 10^174 */
U64(0xCF02B2C2, 0x1207EF2E), U64(0x94F967E4, 0x5E03F4BB),/* == 10^173 */
U64(0xA59BC234, 0xDB398C25), U64(0x43FAB983, 0x7E699096),/* == 10^172 */
U64(0x847C9B5D, 0x7C2E09B7), U64(0x69956135, 0xFEBADA11),/* == 10^171 */
U64(0xD3FA922F, 0x2D1675F2), U64(0x42889B89, 0x97915CE9),/* == 10^170 */
U64(0xA99541BF, 0x57452B28), U64(0x353A1607, 0xAC744A54),/* == 10^169 */
U64(0x87AA9AFF, 0x79042286), U64(0x90FB44D2, 0xF05D0843),/* == 10^168 */
U64(0xD910F7FF, 0x28069DA4), U64(0x1B2BA151, 0x8094DA05),/* == 10^167 */
U64(0xADA72CCC, 0x20054AE9), U64(0xAF561AA7, 0x9A10AE6A),/* == 10^166 */
U64(0x8AEC23D6, 0x80043BEE), U64(0x25DE7BB9, 0x480D5855),/* == 10^165 */
U64(0xDE469FBD, 0x99A05FE3), U64(0x6FCA5F8E, 0xD9AEF3BB),/* == 10^164 */
U64(0xB1D21964, 0x7AE6B31C), U64(0x596EB2D8, 0xAE258FC9),/* == 10^163 */
U64(0x8E41ADE9, 0xFBEBC27D), U64(0x14588F13, 0xBE847307),/* == 10^162 */
U64(0xE39C4976, 0x5FDF9D94), U64(0xED5A7E85, 0xFDA0B80B),/* == 10^161 */
U64(0xB616A12B, 0x7FE617AA), U64(0x577B986B, 0x314D6009),/* == 10^160 */
U64(0x91ABB422, 0xCCB812EE), U64(0xAC62E055, 0xC10AB33B),/* == 10^159 */
U64(0xE912B9D1, 0x478CEB17), U64(0x7A37CD56, 0x01AAB85E),/* == 10^158 */
U64(0xBA756174, 0x393D88DF), U64(0x94F97111, 0x9AEEF9E4),/* == 10^157 */
U64(0x952AB45C, 0xFA97A0B2), U64(0xDD945A74, 0x7BF26184),/* == 10^156 */
U64(0xEEAABA2E, 0x5DBF6784), U64(0x95BA2A53, 0xF983CF39),/* == 10^155 */
U64(0xBEEEFB58, 0x4AFF8603), U64(0xAAFB550F, 0xFACFD8FA),/* == 10^154 */
U64(0x98BF2F79, 0xD5993802), U64(0xEF2F773F, 0xFBD97A62),/* == 10^153 */
U64(0xF46518C2, 0xEF5B8CD1), U64(0x7EB25866, 0x5FC25D69),/* == 10^152 */
U64(0xC38413CF, 0x25E2D70D), U64(0xFEF51385, 0x19684ABB),/* == 10^151 */
U64(0x9C69A972, 0x84B578D7), U64(0xFF2A7604, 0x14536EFC),/* == 10^150 */
U64(0xFA42A8B7, 0x3ABBF48C), U64(0xCB772339, 0xBA1F17F9),/* == 10^149 */
U64(0xC83553C5, 0xC8965D3D), U64(0x6F928294, 0x94E5ACC7),/* == 10^148 */
U64(0xA02AA96B, 0x06DEB0FD), U64(0xF2DB9BAA, 0x10B7BD6C),/* == 10^147 */
U64(0x80222122, 0x6BE55A64), U64(0xC2494954, 0xDA2C978A),/* == 10^146 */
U64(0xCD036837, 0x130890A1), U64(0x36DBA887, 0xC37A8C10),/* == 10^145 */
U64(0xA402B9C5, 0xA8D3A6E7), U64(0x5F16206C, 0x9C6209A6),/* == 10^144 */
U64(0x8335616A, 0xED761F1F), U64(0x7F44E6BD, 0x49E807B8),/* == 10^143 */
U64(0xD1EF0244, 0xAF2364FF), U64(0x3207D795, 0x430CD927),/* == 10^142 */
U64(0xA7F26836, 0xF282B732), U64(0x8E6CAC77, 0x68D7141F),/* == 10^141 */
U64(0x865B8692, 0x5B9BC5C2), U64(0x0B8A2392, 0xBA45A9B2),/* == 10^140 */
U64(0xD6F8D750, 0x9292D603), U64(0x45A9D284, 0x5D3C42B7),/* == 10^139 */
U64(0xABFA45DA, 0x0EDBDE69), U64(0x0487DB9D, 0x17636892),/* == 10^138 */
U64(0x899504AE, 0x72497EBA), U64(0x6A06494A, 0x791C53A8),/* == 10^137 */
U64(0xDC21A117, 0x1D42645D), U64(0x76707543, 0xF4FA1F74),/* == 10^136 */
U64(0xB01AE745, 0xB101E9E4), U64(0x5EC05DCF, 0xF72E7F90),/* == 10^135 */
U64(0x8CE2529E, 0x2734BB1D), U64(0x1899E4A6, 0x5F58660D),/* == 10^134 */
U64(0xE16A1DC9, 0xD8545E94), U64(0xF4296DD6, 0xFEF3D67B),/* == 10^133 */
U64(0xB454E4A1, 0x79DD1877), U64(0x29BABE45, 0x98C311FC),/* == 10^132 */
U64(0x9043EA1A, 0xC7E41392), U64(0x87C89837, 0xAD68DB30),/* == 10^131 */
U64(0xE6D3102A, 0xD96CEC1D), U64(0xA60DC059, 0x157491E6),/* == 10^130 */
U64(0xB8A8D9BB, 0xE123F017), U64(0xB80B0047, 0x445D4185),/* == 10^129 */
U64(0x93BA47C9, 0x80E98CDF), U64(0xC66F336C, 0x36B10137),/* == 10^128 */
U64(0xEC5D3FA8, 0xCE427AFF), U64(0xA3E51F13, 0x8AB4CEBE),/* == 10^127 */
U64(0xBD176620, 0xA501FBFF), U64(0xB650E5A9, 0x3BC3D898),/* == 10^126 */
U64(0x9745EB4D, 0x50CE6332), U64(0xF840B7BA, 0x963646E0),/* == 10^125 */
U64(0xF209787B, 0xB47D6B84), U64(0xC0678C5D, 0xBD23A49A),/* == 10^124 */
U64(0xC1A12D2F, 0xC3978937), U64(0x0052D6B1, 0x641C83AE),/* == 10^123 */
U64(0x9AE75759, 0x6946075F), U64(0x3375788D, 0xE9B06958),/* == 10^122 */
U64(0xF7D88BC2, 0x4209A565), U64(0x1F225A7C, 0xA91A4227),/* == 10^121 */
U64(0xC646D635, 0x01A1511D), U64(0xB281E1FD, 0x541501B9),/* == 10^120 */
U64(0x9E9F11C4, 0x014DDA7E), U64(0x2867E7FD, 0xDCDD9AFA),/* == 10^119 */
U64(0xFDCB4FA0, 0x02162A63), U64(0x73D9732F, 0xC7C8F7F7),/* == 10^118 */
U64(0xCB090C80, 0x01AB551C), U64(0x5CADF5BF, 0xD3072CC5),/* == 10^117 */
U64(0xA26DA399, 0x9AEF7749), U64(0xE3BE5E33, 0x0F38F09E),/* == 10^116 */
U64(0x81F14FAE, 0x158C5F6E), U64(0x4FCB7E8F, 0x3F60C07E),/* == 10^115 */
U64(0xCFE87F7C, 0xEF46FF16), U64(0xE6126418, 0x65679A64),/* == 10^114 */
U64(0xA6539930, 0xBF6BFF45), U64(0x84DB8346, 0xB786151D),/* == 10^113 */
U64(0x850FADC0, 0x9923329E), U64(0x03E2CF6B, 0xC604DDB0),/* == 10^112 */
U64(0xD4E5E2CD, 0xC1D1EA96), U64(0x6C9E18AC, 0x7007C91A),/* == 10^111 */
U64(0xAA51823E, 0x34A7EEDE), U64(0xBD4B46F0, 0x599FD415),/* == 10^110 */
U64(0x884134FE, 0x908658B2), U64(0x3109058D, 0x147FDCDE),/* == 10^109 */
U64(0xDA01EE64, 0x1A708DE9), U64(0xE80E6F48, 0x20CC9496),/* == 10^108 */
U64(0xAE67F1E9, 0xAEC07187), U64(0xECD85906, 0x80A3AA11),/* == 10^107 */
U64(0x8B865B21, 0x5899F46C), U64(0xBD79E0D2, 0x0082EE74),/* == 10^106 */
U64(0xDF3D5E9B, 0xC0F653E1), U64(0x2F2967B6, 0x6737E3ED),/* == 10^105 */
U64(0xB2977EE3, 0x00C50FE7), U64(0x58EDEC91, 0xEC2CB658),/* == 10^104 */
U64(0x8EDF98B5, 0x9A373FEC), U64(0x4724BD41, 0x89BD5EAC),/* == 10^103 */
U64(0xE498F455, 0xC38B997A), U64(0x0B6DFB9C, 0x0F956447),/* == 10^102 */
U64(0xB6E0C377, 0xCFA2E12E), U64(0x6F8B2FB0, 0x0C77836C),/* == 10^101 */
U64(0x924D692C, 0xA61BE758), U64(0x593C2626, 0x705F9C56),/* == 10^100 */
U64(0xEA157514, 0x3CF97226), U64(0xF52D09D7, 0x1A3293BE),/* == 10^99 */
U64(0xBB445DA9, 0xCA61281F), U64(0x2A8A6E45, 0xAE8EDC98),/* == 10^98 */
U64(0x95D04AEE, 0x3B80ECE5), U64(0xBBA1F1D1, 0x58724A13),/* == 10^97 */
U64(0xEFB3AB16, 0xC59B14A2), U64(0xC5CFE94E, 0xF3EA101E),/* == 10^96 */
U64(0xBFC2EF45, 0x6AE276E8), U64(0x9E3FEDD8, 0xC321A67F),/* == 10^95 */
U64(0x9968BF6A, 0xBBE85F20), U64(0x7E998B13, 0xCF4E1ECC),/* == 10^94 */
U64(0xF5746577, 0x930D6500), U64(0xCA8F44EC, 0x7EE36479),/* == 10^93 */
U64(0xC45D1DF9, 0x42711D9A), U64(0x3BA5D0BD, 0x324F8394),/* == 10^92 */
U64(0x9D174B2D, 0xCEC0E47B), U64(0x62EB0D64, 0x283F9C76),/* == 10^91 */
U64(0xFB587849, 0x4ACE3A5F), U64(0x04AB48A0, 0x4065C724),/* == 10^90 */
U64(0xC913936D, 0xD571C84C), U64(0x03BC3A19, 0xCD1E38EA),/* == 10^89 */
U64(0xA0DC75F1, 0x778E39D6), U64(0x696361AE, 0x3DB1C721),/* == 10^88 */
U64(0x80B05E5A, 0xC60B6178), U64(0x544F8158, 0x315B05B4),/* == 10^87 */
U64(0xCDE6FD5E, 0x09ABCF26), U64(0xED4C0226, 0xB55E6F87),/* == 10^86 */
U64(0xA4B8CAB1, 0xA1563F52), U64(0x577001B8, 0x91185939),/* == 10^85 */
U64(0x83C7088E, 0x1AAB65DB), U64(0x792667C6, 0xDA79E0FA),/* == 10^84 */
U64(0xD2D80DB0, 0x2AABD62B), U64(0xF50A3FA4, 0x90C30190),/* == 10^83 */
U64(0xA8ACD7C0, 0x222311BC), U64(0xC40832EA, 0x0D68CE0D),/* == 10^82 */
U64(0x86F0AC99, 0xB4E8DAFD), U64(0x69A028BB, 0x3DED71A4),/* == 10^81 */
U64(0xD7E77A8F, 0x87DAF7FB), U64(0xDC33745E, 0xC97BE906),/* == 10^80 */
U64(0xACB92ED9, 0x397BF996), U64(0x49C2C37F, 0x07965405),/* == 10^79 */
U64(0x8A2DBF14, 0x2DFCC7AB), U64(0x6E356932, 0x6C784337),/* == 10^78 */
U64(0xDD15FE86, 0xAFFAD912), U64(0x49EF0EB7, 0x13F39EBF),/* == 10^77 */
U64(0xB0DE6538, 0x8CC8ADA8), U64(0x3B25A55F, 0x43294BCC),/* == 10^76 */
U64(0x8D7EB760, 0x70A08AEC), U64(0xFC1E1DE5, 0xCF543CA3),/* == 10^75 */
U64(0xE264589A, 0x4DCDAB14), U64(0xC696963C, 0x7EED2DD2),/* == 10^74 */
U64(0xB51D13AE, 0xA4A488DD), U64(0x6BABAB63, 0x98BDBE41),/* == 10^73 */
U64(0x90E40FBE, 0xEA1D3A4A), U64(0xBC8955E9, 0x46FE31CE),/* == 10^72 */
U64(0xE7D34C64, 0xA9C85D44), U64(0x60DBBCA8, 0x7196B616),/* == 10^71 */
U64(0xB975D6B6, 0xEE39E436), U64(0xB3E2FD53, 0x8E122B45),/* == 10^70 */
U64(0x945E455F, 0x24FB1CF8), U64(0x8FE8CAA9, 0x3E74EF6A),/* == 10^69 */
U64(0xED63A231, 0xD4C4FB27), U64(0x4CA7AAA8, 0x63EE4BDD),/* == 10^68 */
U64(0xBDE94E8E, 0x43D0C8EC), U64(0x3D52EEED, 0x1CBEA317),/* == 10^67 */
U64(0x97EDD871, 0xCFDA3A56), U64(0x97758BF0, 0xE3CBB5AC),/* == 10^66 */
U64(0xF316271C, 0x7FC3908A), U64(0x8BEF464E, 0x3945EF7A),/* == 10^65 */
U64(0xC2781F49, 0xFFCFA6D5), U64(0x3CBF6B71, 0xC76B25FB),/* == 10^64 */
U64(0x9B934C3B, 0x330C8577), U64(0x63CC55F4, 0x9F88EB2F),/* == 10^63 */
U64(0xF8EBAD2B, 0x84E0D58B), U64(0xD2E08987, 0x65A7DEB2),/* == 10^62 */
U64(0xC722F0EF, 0x9D80AAD6), U64(0x424D3AD2, 0xB7B97EF5),/* == 10^61 */
U64(0x9F4F2726, 0x179A2245), U64(0x01D76242, 0x2C946591),/* == 10^60 */
U64(0xFEE50B70, 0x25C36A08), U64(0x02F236D0, 0x4753D5B5),/* == 10^59 */
U64(0xCBEA6F8C, 0xEB02BB39), U64(0x9BF4F8A6, 0x9F764490),/* == 10^58 */
U64(0xA321F2D7, 0x226895C7), U64(0xAFF72D52, 0x192B6A0D),/* == 10^57 */
U64(0x82818F12, 0x81ED449F), U64(0xBFF8F10E, 0x7A8921A4),/* == 10^56 */
U64(0xD0CF4B50, 0xCFE20765), U64(0xFFF4B4E3, 0xF741CF6D),/* == 10^55 */
U64(0xA70C3C40, 0xA64E6C51), U64(0x999090B6, 0x5F67D924),/* == 10^54 */
U64(0x85A36366, 0xEB71F041), U64(0x47A6DA2B, 0x7F864750),/* == 10^53 */
U64(0xD5D238A4, 0xABE98068), U64(0x72A49045, 0x98D6D880),/* == 10^52 */
U64(0xAB0E93B6, 0xEFEE0053), U64(0x8EEA0D04, 0x7A457A00),/* == 10^51 */
U64(0x88D8762B, 0xF324CD0F), U64(0xA5880A69, 0xFB6AC800),/* == 10^50 */
U64(0xDAF3F046, 0x51D47B4C), U64(0x3C0CDD76, 0x5F114000),/* == 10^49 */
U64(0xAF298D05, 0x0E4395D6), U64(0x9670B12B, 0x7F410000),/* == 10^48 */
U64(0x8C213D9D, 0xA502DE45), U64(0x4526F422, 0xCC340000),/* == 10^47 */
U64(0xE0352F62, 0xA19E306E), U64(0xD50B2037, 0xAD200000),/* == 10^46 */
U64(0xB35DBF82, 0x1AE4F38B), U64(0xDDA2802C, 0x8A800000),/* == 10^45 */
U64(0x8F7E32CE, 0x7BEA5C6F), U64(0xE4820023, 0xA2000000),/* == 10^44 */
U64(0xE596B7B0, 0xC643C719), U64(0x6D9CCD05, 0xD0000000),/* == 10^43 */
U64(0xB7ABC627, 0x050305AD), U64(0xF14A3D9E, 0x40000000),/* == 10^42 */
U64(0x92EFD1B8, 0xD0CF37BE), U64(0x5AA1CAE5, 0x00000000),/* == 10^41 */
U64(0xEB194F8E, 0x1AE525FD), U64(0x5DCFAB08, 0x00000000),/* == 10^40 */
U64(0xBC143FA4, 0xE250EB31), U64(0x17D955A0, 0x00000000),/* == 10^39 */
U64(0x96769950, 0xB50D88F4), U64(0x13144480, 0x00000000),/* == 10^38 */
U64(0xF0BDC21A, 0xBB48DB20), U64(0x1E86D400, 0x00000000),/* == 10^37 */
U64(0xC097CE7B, 0xC90715B3), U64(0x4B9F1000, 0x00000000),/* == 10^36 */
U64(0x9A130B96, 0x3A6C115C), U64(0x3C7F4000, 0x00000000),/* == 10^35 */
U64(0xF684DF56, 0xC3E01BC6), U64(0xC7320000, 0x00000000),/* == 10^34 */
U64(0xC5371912, 0x364CE305), U64(0x6C280000, 0x00000000),/* == 10^33 */
U64(0x9DC5ADA8, 0x2B70B59D), U64(0xF0200000, 0x00000000),/* == 10^32 */
U64(0xFC6F7C40, 0x45812296), U64(0x4D000000, 0x00000000),/* == 10^31 */
U64(0xC9F2C9CD, 0x04674EDE), U64(0xA4000000, 0x00000000),/* == 10^30 */
U64(0xA18F07D7, 0x36B90BE5), U64(0x50000000, 0x00000000),/* == 10^29 */
U64(0x813F3978, 0xF8940984), U64(0x40000000, 0x00000000),/* == 10^28 */
U64(0xCECB8F27, 0xF4200F3A), U64(0x00000000, 0x00000000),/* == 10^27 */
U64(0xA56FA5B9, 0x9019A5C8), U64(0x00000000, 0x00000000),/* == 10^26 */
U64(0x84595161, 0x401484A0), U64(0x00000000, 0x00000000),/* == 10^25 */
U64(0xD3C21BCE, 0xCCEDA100), U64(0x00000000, 0x00000000),/* == 10^24 */
U64(0xA968163F, 0x0A57B400), U64(0x00000000, 0x00000000),/* == 10^23 */
U64(0x87867832, 0x6EAC9000), U64(0x00000000, 0x00000000),/* == 10^22 */
U64(0xD8D726B7, 0x177A8000), U64(0x00000000, 0x00000000),/* == 10^21 */
U64(0xAD78EBC5, 0xAC620000), U64(0x00000000, 0x00000000),/* == 10^20 */
U64(0x8AC72304, 0x89E80000), U64(0x00000000, 0x00000000),/* == 10^19 */
U64(0xDE0B6B3A, 0x76400000), U64(0x00000000, 0x00000000),/* == 10^18 */
U64(0xB1A2BC2E, 0xC5000000), U64(0x00000000, 0x00000000),/* == 10^17 */
U64(0x8E1BC9BF, 0x04000000), U64(0x00000000, 0x00000000),/* == 10^16 */
U64(0xE35FA931, 0xA0000000), U64(0x00000000, 0x00000000),/* == 10^15 */
U64(0xB5E620F4, 0x80000000), U64(0x00000000, 0x00000000),/* == 10^14 */
U64(0x9184E72A, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^13 */
U64(0xE8D4A510, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^12 */
U64(0xBA43B740, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^11 */
U64(0x9502F900, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^10 */
U64(0xEE6B2800, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^9 */
U64(0xBEBC2000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^8 */
U64(0x98968000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^7 */
U64(0xF4240000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^6 */
U64(0xC3500000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^5 */
U64(0x9C400000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^4 */
U64(0xFA000000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^3 */
U64(0xC8000000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^2 */
U64(0xA0000000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^1 */
U64(0x80000000, 0x00000000), U64(0x00000000, 0x00000000),/* == 10^0 */
U64(0xCCCCCCCC, 0xCCCCCCCC), U64(0xCCCCCCCC, 0xCCCCCCCD),/* == 10^-1 */
U64(0xA3D70A3D, 0x70A3D70A), U64(0x3D70A3D7, 0x0A3D70A4),/* == 10^-2 */
U64(0x83126E97, 0x8D4FDF3B), U64(0x645A1CAC, 0x083126E9),/* == 10^-3 */
U64(0xD1B71758, 0xE219652B), U64(0xD3C36113, 0x404EA4A9),/* == 10^-4 */
U64(0xA7C5AC47, 0x1B478423), U64(0x0FCF80DC, 0x33721D54),/* == 10^-5 */
U64(0x8637BD05, 0xAF6C69B5), U64(0xA63F9A49, 0xC2C1B110),/* == 10^-6 */
U64(0xD6BF94D5, 0xE57A42BC), U64(0x3D329076, 0x04691B4D),/* == 10^-7 */
U64(0xABCC7711, 0x8461CEFC), U64(0xFDC20D2B, 0x36BA7C3D),/* == 10^-8 */
U64(0x89705F41, 0x36B4A597), U64(0x31680A88, 0xF8953031),/* == 10^-9 */
U64(0xDBE6FECE, 0xBDEDD5BE), U64(0xB573440E, 0x5A884D1B),/* == 10^-10 */
U64(0xAFEBFF0B, 0xCB24AAFE), U64(0xF78F69A5, 0x1539D749),/* == 10^-11 */
U64(0x8CBCCC09, 0x6F5088CB), U64(0xF93F87B7, 0x442E45D4),/* == 10^-12 */
U64(0xE12E1342, 0x4BB40E13), U64(0x2865A5F2, 0x06B06FBA),/* == 10^-13 */
U64(0xB424DC35, 0x095CD80F), U64(0x538484C1, 0x9EF38C94),/* == 10^-14 */
U64(0x901D7CF7, 0x3AB0ACD9), U64(0x0F9D3701, 0x4BF60A10),/* == 10^-15 */
U64(0xE69594BE, 0xC44DE15B), U64(0x4C2EBE68, 0x7989A9B4),/* == 10^-16 */
U64(0xB877AA32, 0x36A4B449), U64(0x09BEFEB9, 0xFAD487C3),/* == 10^-17 */
U64(0x9392EE8E, 0x921D5D07), U64(0x3AFF322E, 0x62439FCF),/* == 10^-18 */
U64(0xEC1E4A7D, 0xB69561A5), U64(0x2B31E9E3, 0xD06C32E5),/* == 10^-19 */
U64(0xBCE50864, 0x92111AEA), U64(0x88F4BB1C, 0xA6BCF584),/* == 10^-20 */
U64(0x971DA050, 0x74DA7BEE), U64(0xD3F6FC16, 0xEBCA5E03),/* == 10^-21 */
U64(0xF1C90080, 0xBAF72CB1), U64(0x5324C68B, 0x12DD6338),/* == 10^-22 */
U64(0xC16D9A00, 0x95928A27), U64(0x75B7053C, 0x0F178294),/* == 10^-23 */
U64(0x9ABE14CD, 0x44753B52), U64(0xC4926A96, 0x72793543),/* == 10^-24 */
U64(0xF79687AE, 0xD3EEC551), U64(0x3A83DDBD, 0x83F52205),/* == 10^-25 */
U64(0xC6120625, 0x76589DDA), U64(0x95364AFE, 0x032A819D),/* == 10^-26 */
U64(0x9E74D1B7, 0x91E07E48), U64(0x775EA264, 0xCF55347E),/* == 10^-27 */
U64(0xFD87B5F2, 0x8300CA0D), U64(0x8BCA9D6E, 0x188853FC),/* == 10^-28 */
U64(0xCAD2F7F5, 0x359A3B3E), U64(0x096EE458, 0x13A04330),/* == 10^-29 */
U64(0xA2425FF7, 0x5E14FC31), U64(0xA1258379, 0xA94D028D),/* == 10^-30 */
U64(0x81CEB32C, 0x4B43FCF4), U64(0x80EACF94, 0x8770CED7),/* == 10^-31 */
U64(0xCFB11EAD, 0x453994BA), U64(0x67DE18ED, 0xA5814AF2),/* == 10^-32 */
U64(0xA6274BBD, 0xD0FADD61), U64(0xECB1AD8A, 0xEACDD58E),/* == 10^-33 */
U64(0x84EC3C97, 0xDA624AB4), U64(0xBD5AF13B, 0xEF0B113F),/* == 10^-34 */
U64(0xD4AD2DBF, 0xC3D07787), U64(0x955E4EC6, 0x4B44E864),/* == 10^-35 */
U64(0xAA242499, 0x697392D2), U64(0xDDE50BD1, 0xD5D0B9EA),/* == 10^-36 */
U64(0x881CEA14, 0x545C7575), U64(0x7E50D641, 0x77DA2E55),/* == 10^-37 */
U64(0xD9C7DCED, 0x53C72255), U64(0x96E7BD35, 0x8C904A21),/* == 10^-38 */
U64(0xAE397D8A, 0xA96C1B77), U64(0xABEC975E, 0x0A0D081B),/* == 10^-39 */
U64(0x8B61313B, 0xBABCE2C6), U64(0x2323AC4B, 0x3B3DA015),/* == 10^-40 */
U64(0xDF01E85F, 0x912E37A3), U64(0x6B6C46DE, 0xC52F6688),/* == 10^-41 */
U64(0xB267ED19, 0x40F1C61C), U64(0x55F038B2, 0x37591ED3),/* == 10^-42 */
U64(0x8EB98A7A, 0x9A5B04E3), U64(0x77F3608E, 0x92ADB243),/* == 10^-43 */
U64(0xE45C10C4, 0x2A2B3B05), U64(0x8CB89A7D, 0xB77C506B),/* == 10^-44 */
U64(0xB6B00D69, 0xBB55C8D1), U64(0x3D607B97, 0xC5FD0D22),/* == 10^-45 */
U64(0x92267121, 0x62AB070D), U64(0xCAB39613, 0x04CA70E8),/* == 10^-46 */
U64(0xE9D71B68, 0x9DDE71AF), U64(0xAAB8F01E, 0x6E10B4A7),/* == 10^-47 */
U64(0xBB127C53, 0xB17EC159), U64(0x5560C018, 0x580D5D52),/* == 10^-48 */
U64(0x95A86376, 0x27989AAD), U64(0xDDE70013, 0x79A44AA8),/* == 10^-49 */
U64(0xEF73D256, 0xA5C0F77C), U64(0x963E6685, 0x8F6D4440),/* == 10^-50 */
U64(0xBF8FDB78, 0x849A5F96), U64(0xDE985204, 0x72BDD033),/* == 10^-51 */
U64(0x993FE2C6, 0xD07B7FAB), U64(0xE546A803, 0x8EFE4029),/* == 10^-52 */
U64(0xF5330471, 0x4D9265DF), U64(0xD53DD99F, 0x4B3066A8),/* == 10^-53 */
U64(0xC428D05A, 0xA4751E4C), U64(0xAA97E14C, 0x3C26B887),/* == 10^-54 */
U64(0x9CED737B, 0xB6C4183D), U64(0x55464DD6, 0x9685606C),/* == 10^-55 */
U64(0xFB158592, 0xBE068D2E), U64(0xEED6E2F0, 0xF0D56713),/* == 10^-56 */
U64(0xC8DE0475, 0x64D20A8B), U64(0xF245825A, 0x5A445275),/* == 10^-57 */
U64(0xA0B19D2A, 0xB70E6ED6), U64(0x5B6ACEAE, 0xAE9D0EC4),/* == 10^-58 */
U64(0x808E1755, 0x5F3EBF11), U64(0xE2BBD88B, 0xBEE40BD0),/* == 10^-59 */
U64(0xCDB02555, 0x653131B6), U64(0x3792F412, 0xCB06794D),/* == 10^-60 */
U64(0xA48CEAAA, 0xB75A8E2B), U64(0x5FA8C342, 0x3C052DD7),/* == 10^-61 */
U64(0x83A3EEEE, 0xF9153E89), U64(0x1953CF68, 0x300424AC),/* == 10^-62 */
U64(0xD29FE4B1, 0x8E88640E), U64(0x8EEC7F0D, 0x19A03AAD),/* == 10^-63 */
U64(0xA87FEA27, 0xA539E9A5), U64(0x3F2398D7, 0x47B36224),/* == 10^-64 */
U64(0x86CCBB52, 0xEA94BAEA), U64(0x98E94712, 0x9FC2B4EA),/* == 10^-65 */
U64(0xD7ADF884, 0xAA879177), U64(0x5B0ED81D, 0xCC6ABB10),/* == 10^-66 */
U64(0xAC8B2D36, 0xEED2DAC5), U64(0xE272467E, 0x3D222F40),/* == 10^-67 */
U64(0x8A08F0F8, 0xBF0F156B), U64(0x1B8E9ECB, 0x641B5900),/* == 10^-68 */
U64(0xDCDB1B27, 0x98182244), U64(0xF8E43145, 0x6CF88E66),/* == 10^-69 */
U64(0xB0AF48EC, 0x79ACE837), U64(0x2D835A9D, 0xF0C6D852),/* == 10^-70 */
U64(0x8D590723, 0x948A535F), U64(0x579C487E, 0x5A38AD0E),/* == 10^-71 */
U64(0xE2280B6C, 0x20DD5232), U64(0x25C6DA63, 0xC38DE1B0),/* == 10^-72 */
U64(0xB4ECD5F0, 0x1A4AA828), U64(0x1E38AEB6, 0x360B1AF3),/* == 10^-73 */
U64(0x90BD77F3, 0x483BB9B9), U64(0xB1C6F22B, 0x5E6F48C3),/* == 10^-74 */
U64(0xE7958CB8, 0x7392C2C2), U64(0xB60B1D12, 0x30B20E04),/* == 10^-75 */
U64(0xB9447093, 0x8FA89BCE), U64(0xF808E40E, 0x8D5B3E6A),/* == 10^-76 */
U64(0x9436C076, 0x0C86E30B), U64(0xF9A0B672, 0x0AAF6521),/* == 10^-77 */
U64(0xED246723, 0x473E3813), U64(0x290123E9, 0xAAB23B69),/* == 10^-78 */
U64(0xBDB6B8E9, 0x05CB600F), U64(0x5400E987, 0xBBC1C921),/* == 10^-79 */
U64(0x97C560BA, 0x6B0919A5), U64(0xDCCD879F, 0xC967D41A),/* == 10^-80 */
U64(0xF2D56790, 0xAB41C2A2), U64(0xFAE27299, 0x423FB9C3),/* == 10^-81 */
U64(0xC24452DA, 0x229B021B), U64(0xFBE85BAD, 0xCE996169),/* == 10^-82 */
U64(0x9B69DBE1, 0xB548CE7C), U64(0xC986AFBE, 0x3EE11ABA),/* == 10^-83 */
U64(0xF8A95FCF, 0x88747D94), U64(0x75A44C63, 0x97CE912A),/* == 10^-84 */
U64(0xC6EDE63F, 0xA05D3143), U64(0x91503D1C, 0x79720DBB),/* == 10^-85 */
U64(0x9F24B832, 0xE6B0F436), U64(0x0DD9CA7D, 0x2DF4D7C9),/* == 10^-86 */
U64(0xFEA126B7, 0xD78186BC), U64(0xE2F610C8, 0x4987BFA8),/* == 10^-87 */
U64(0xCBB41EF9, 0x79346BCA), U64(0x4F2B40A0, 0x3AD2FFBA),/* == 10^-88 */
U64(0xA2F67F2D, 0xFA90563B), U64(0x72890080, 0x2F0F32FB),/* == 10^-89 */
U64(0x825ECC24, 0xC873782F), U64(0x8ED40066, 0x8C0C28C9),/* == 10^-90 */
U64(0xD097AD07, 0xA71F26B2), U64(0x7E2000A4, 0x1346A7A8),/* == 10^-91 */
U64(0xA6DFBD9F, 0xB8E5B88E), U64(0xCB4CCD50, 0x0F6BB953),/* == 10^-92 */
U64(0x857FCAE6, 0x2D8493A5), U64(0x6F70A440, 0x0C562DDC),/* == 10^-93 */
U64(0xD59944A3, 0x7C0752A2), U64(0x4BE76D33, 0x46F04960),/* == 10^-94 */
U64(0xAAE103B5, 0xFCD2A881), U64(0xD652BDC2, 0x9F26A11A),/* == 10^-95 */
U64(0x88B402F7, 0xFD75539B), U64(0x11DBCB02, 0x18EBB414),/* == 10^-96 */
U64(0xDAB99E59, 0x958885C4), U64(0xE95FAB36, 0x8E45ECED),/* == 10^-97 */
U64(0xAEFAE514, 0x77A06B03), U64(0xEDE62292, 0x0B6B23F1),/* == 10^-98 */
U64(0x8BFBEA76, 0xC619EF36), U64(0x57EB4EDB, 0x3C55B65B),/* == 10^-99 */
U64(0xDFF97724, 0x70297EBD), U64(0x59787E2B, 0x93BC56F7),/* == 10^-100 */
U64(0xB32DF8E9, 0xF3546564), U64(0x47939822, 0xDC96ABF9),/* == 10^-101 */
U64(0x8F57FA54, 0xC2A9EAB6), U64(0x9FA94682, 0x4A12232E),/* == 10^-102 */
U64(0xE5599087, 0x9DDCAABD), U64(0xCC420A6A, 0x101D0516),/* == 10^-103 */
U64(0xB77ADA06, 0x17E3BBCB), U64(0x09CE6EBB, 0x40173745),/* == 10^-104 */
U64(0x92C8AE6B, 0x464FC96F), U64(0x3B0B8BC9, 0x0012929D),/* == 10^-105 */
U64(0xEADAB0AB, 0xA3B2DBE5), U64(0x2B45AC74, 0xCCEA842F),/* == 10^-106 */
U64(0xBBE226EF, 0xB628AFEA), U64(0x890489F7, 0x0A55368C),/* == 10^-107 */
U64(0x964E858C, 0x91BA2655), U64(0x3A6A07F8, 0xD510F870),/* == 10^-108 */
U64(0xF07DA27A, 0x82C37088), U64(0x5D767327, 0xBB4E5A4D),/* == 10^-109 */
U64(0xC06481FB, 0x9BCF8D39), U64(0xE45EC286, 0x2F71E1D7),/* == 10^-110 */
U64(0x99EA0196, 0x163FA42E), U64(0x504BCED1, 0xBF8E4E46),/* == 10^-111 */
U64(0xF64335BC, 0xF065D37D), U64(0x4D4617B5, 0xFF4A16D6),/* == 10^-112 */
U64(0xC5029163, 0xF384A931), U64(0x0A9E795E, 0x65D4DF11),/* == 10^-113 */
U64(0x9D9BA783, 0x2936EDC0), U64(0xD54B944B, 0x84AA4C0E),/* == 10^-114 */
U64(0xFC2C3F38, 0x41F17C67), U64(0xBBAC2078, 0xD443ACE3),/* == 10^-115 */
U64(0xC9BCFF60, 0x34C13052), U64(0xFC89B393, 0xDD02F0B6),/* == 10^-116 */
U64(0xA163FF80, 0x2A3426A8), U64(0xCA07C2DC, 0xB0CF26F8),/* == 10^-117 */
U64(0x811CCC66, 0x8829B887), U64(0x0806357D, 0x5A3F5260),/* == 10^-118 */
U64(0xCE947A3D, 0xA6A9273E), U64(0x733D2262, 0x29FEEA33),/* == 10^-119 */
U64(0xA54394FE, 0x1EEDB8FE), U64(0xC2974EB4, 0xEE658829),/* == 10^-120 */
U64(0x843610CB, 0x4BF160CB), U64(0xCEDF722A, 0x585139BA),/* == 10^-121 */
U64(0xD389B478, 0x79823479), U64(0x4AFF1D10, 0x8D4EC2C3),/* == 10^-122 */
U64(0xA93AF6C6, 0xC79B5D2D), U64(0xD598E40D, 0x3DD89BCF),/* == 10^-123 */
U64(0x87625F05, 0x6C7C4A8B), U64(0x11471CD7, 0x64AD4973),/* == 10^-124 */
U64(0xD89D64D5, 0x7A607744), U64(0xE871C7BF, 0x077BA8B8),/* == 10^-125 */
U64(0xAD4AB711, 0x2EB3929D), U64(0x86C16C98, 0xD2C953C6),/* == 10^-126 */
U64(0x8AA22C0D, 0xBEF60EE4), U64(0x6BCDF07A, 0x423AA96B),/* == 10^-127 */
U64(0xDDD0467C, 0x64BCE4A0), U64(0xAC7CB3F6, 0xD05DDBDF),/* == 10^-128 */
U64(0xB1736B96, 0xB6FD83B3), U64(0xBD308FF8, 0xA6B17CB2),/* == 10^-129 */
U64(0x8DF5EFAB, 0xC5979C8F), U64(0xCA8D3FFA, 0x1EF463C2),/* == 10^-130 */
U64(0xE3231912, 0xD5BF60E6), U64(0x10E1FFF6, 0x97ED6C69),/* == 10^-131 */
U64(0xB5B5ADA8, 0xAAFF80B8), U64(0x0D819992, 0x132456BB),/* == 10^-132 */
U64(0x915E2486, 0xEF32CD60), U64(0x0ACE1474, 0xDC1D122F),/* == 10^-133 */
U64(0xE896A0D7, 0xE51E1566), U64(0x77B020BA, 0xF9C81D18),/* == 10^-134 */
U64(0xBA121A46, 0x50E4DDEB), U64(0x92F34D62, 0x616CE413),/* == 10^-135 */
U64(0x94DB4838, 0x40B717EF), U64(0xA8C2A44E, 0xB4571CDC),/* == 10^-136 */
U64(0xEE2BA6C0, 0x678B597F), U64(0x746AA07D, 0xED582E2D),/* == 10^-137 */
U64(0xBE895233, 0x86091465), U64(0xF6BBB397, 0xF1135824),/* == 10^-138 */
U64(0x986DDB5C, 0x6B3A76B7), U64(0xF8962946, 0x5A75E01D),/* == 10^-139 */
U64(0xF3E2F893, 0xDEC3F126), U64(0x5A89DBA3, 0xC3EFCCFB),/* == 10^-140 */
U64(0xC31BFA0F, 0xE5698DB8), U64(0x486E494F, 0xCFF30A62),/* == 10^-141 */
U64(0x9C1661A6, 0x51213E2D), U64(0x06BEA10C, 0xA65C084F),/* == 10^-142 */
U64(0xF9BD690A, 0x1B68637B), U64(0x3DFDCE7A, 0xA3C673B1),/* == 10^-143 */
U64(0xC7CABA6E, 0x7C5382C8), U64(0xFE64A52E, 0xE96B8FC1),/* == 10^-144 */
U64(0x9FD561F1, 0xFD0F9BD3), U64(0xFEB6EA8B, 0xEDEFA634),/* == 10^-145 */
U64(0xFFBBCFE9, 0x94E5C61F), U64(0xFDF17746, 0x497F7053),/* == 10^-146 */
U64(0xCC963FEE, 0x10B7D1B3), U64(0x318DF905, 0x079926A9),/* == 10^-147 */
U64(0xA3AB6658, 0x0D5FDAF5), U64(0xC13E60D0, 0xD2E0EBBA),/* == 10^-148 */
U64(0x82EF8513, 0x3DE648C4), U64(0x9A984D73, 0xDBE722FB),/* == 10^-149 */
U64(0xD17F3B51, 0xFCA3A7A0), U64(0xF75A1586, 0x2CA504C5),/* == 10^-150 */
U64(0xA798FC41, 0x96E952E7), U64(0x2C481138, 0x23B73704),/* == 10^-151 */
U64(0x8613FD01, 0x45877585), U64(0xBD06742C, 0xE95F5F37),/* == 10^-152 */
U64(0xD686619B, 0xA27255A2), U64(0xC80A537B, 0x0EFEFEBE),/* == 10^-153 */
U64(0xAB9EB47C, 0x81F5114F), U64(0x066EA92F, 0x3F326565),/* == 10^-154 */
U64(0x894BC396, 0xCE5DA772), U64(0x6B8BBA8C, 0x328EB784),/* == 10^-155 */
U64(0xDBAC6C24, 0x7D62A583), U64(0xDF45F746, 0xB74ABF39),/* == 10^-156 */
U64(0xAFBD2350, 0x644EEACF), U64(0xE5D1929E, 0xF90898FB),/* == 10^-157 */
U64(0x8C974F73, 0x83725573), U64(0x1E414218, 0xC73A13FC),/* == 10^-158 */
U64(0xE0F218B8, 0xD25088B8), U64(0x306869C1, 0x3EC3532C),/* == 10^-159 */
U64(0xB3F4E093, 0xDB73A093), U64(0x59ED2167, 0x65690F57),/* == 10^-160 */
U64(0x8FF71A0F, 0xE2C2E6DC), U64(0x47F0E785, 0xEABA72AC),/* == 10^-161 */
U64(0xE65829B3, 0x046B0AFA), U64(0x0CB4A5A3, 0x112A5113),/* == 10^-162 */
U64(0xB84687C2, 0x69EF3BFB), U64(0x3D5D514F, 0x40EEA742),/* == 10^-163 */
U64(0x936B9FCE, 0xBB25C995), U64(0xCAB10DD9, 0x00BEEC35),/* == 10^-164 */
U64(0xEBDF6617, 0x91D60F56), U64(0x111B495B, 0x3464AD21),/* == 10^-165 */
U64(0xBCB2B812, 0xDB11A5DE), U64(0x7415D448, 0xF6B6F0E8),/* == 10^-166 */
U64(0x96F5600F, 0x15A7B7E5), U64(0x29AB103A, 0x5EF8C0B9),/* == 10^-167 */
U64(0xF18899B1, 0xBC3F8CA1), U64(0xDC44E6C3, 0xCB279AC2),/* == 10^-168 */
U64(0xC13A148E, 0x3032D6E7), U64(0xE36A5236, 0x3C1FAF02),/* == 10^-169 */
U64(0x9A94DD3E, 0x8CF578B9), U64(0x82BB74F8, 0x301958CE),/* == 10^-170 */
U64(0xF7549530, 0xE188C128), U64(0xD12BEE59, 0xE68EF47D),/* == 10^-171 */
U64(0xC5DD4427, 0x1AD3CDBA), U64(0x40EFF1E1, 0x853F29FE),/* == 10^-172 */
U64(0x9E4A9CEC, 0x15763E2E), U64(0x9A598E4E, 0x043287FE),/* == 10^-173 */
U64(0xFD442E46, 0x88BD304A), U64(0x908F4A16, 0x6D1DA663),/* == 10^-174 */
U64(0xCA9CF1D2, 0x06FDC03B), U64(0xA6D90811, 0xF0E4851C),/* == 10^-175 */
U64(0xA21727DB, 0x38CB002F), U64(0xB8ADA00E, 0x5A506A7D),/* == 10^-176 */
U64(0x81AC1FE2, 0x93D599BF), U64(0xC6F14CD8, 0x48405531),/* == 10^-177 */
U64(0xCF79CC9D, 0xB955C2CC), U64(0x7182148D, 0x4066EEB4),/* == 10^-178 */
U64(0xA5FB0A17, 0xC777CF09), U64(0xF4681071, 0x00525890),/* == 10^-179 */
U64(0x84C8D4DF, 0xD2C63F3B), U64(0x29ECD9F4, 0x0041E073),/* == 10^-180 */
U64(0xD47487CC, 0x8470652B), U64(0x7647C320, 0x0069671F),/* == 10^-181 */
U64(0xA9F6D30A, 0x038D1DBC), U64(0x5E9FCF4C, 0xCD211F4C),/* == 10^-182 */
U64(0x87F8A8D4, 0xCFA417C9), U64(0xE54CA5D7, 0x0A80E5D6),/* == 10^-183 */
U64(0xD98DDAEE, 0x19068C76), U64(0x3BADD624, 0xDD9B0957),/* == 10^-184 */
U64(0xAE0B158B, 0x4738705E), U64(0x9624AB50, 0xB148D446),/* == 10^-185 */
U64(0x8B3C113C, 0x38F9F37E), U64(0xDE83BC40, 0x8DD3DD05),/* == 10^-186 */
U64(0xDEC681F9, 0xF4C31F31), U64(0x6405FA00, 0xE2EC94D4),/* == 10^-187 */
U64(0xB23867FB, 0x2A35B28D), U64(0xE99E619A, 0x4F23AA43),/* == 10^-188 */
U64(0x8E938662, 0x882AF53E), U64(0x547EB47B, 0x7282EE9C),/* == 10^-189 */
U64(0xE41F3D6A, 0x7377EECA), U64(0x20CABA5F, 0x1D9E4A94),/* == 10^-190 */
U64(0xB67F6455, 0x292CBF08), U64(0x1A3BC84C, 0x17B1D543),/* == 10^-191 */
U64(0x91FF8377, 0x5423CC06), U64(0x7B6306A3, 0x4627DDCF),/* == 10^-192 */
U64(0xE998D258, 0x869FACD7), U64(0x2BD1A438, 0x703FC94B),/* == 10^-193 */
U64(0xBAE0A846, 0xD2195712), U64(0x89748360, 0x59CCA109),/* == 10^-194 */
U64(0x9580869F, 0x0E7AAC0E), U64(0xD45D35E6, 0xAE3D4DA1),/* == 10^-195 */
U64(0xEF340A98, 0x172AACE4), U64(0x86FB8971, 0x16C87C35),/* == 10^-196 */
U64(0xBF5CD546, 0x78EEF0B6), U64(0xD262D45A, 0x78A0635D),/* == 10^-197 */
U64(0x99171105, 0x2D8BF3C5), U64(0x751BDD15, 0x2D4D1C4B),/* == 10^-198 */
U64(0xF4F1B4D5, 0x15ACB93B), U64(0xEE92FB55, 0x15482D44),/* == 10^-199 */
U64(0xC3F490AA, 0x77BD60FC), U64(0xBEDBFC44, 0x11068A9D),/* == 10^-200 */
U64(0x9CC3A6EE, 0xC6311A63), U64(0xCBE33036, 0x74053BB1),/* == 10^-201 */
U64(0xFAD2A4B1, 0x3D1B5D6C), U64(0x796B8057, 0x20085F81),/* == 10^-202 */
U64(0xC8A883C0, 0xFDAF7DF0), U64(0x6122CD12, 0x8006B2CE),/* == 10^-203 */
U64(0xA086CFCD, 0x97BF97F3), U64(0x80E8A40E, 0xCCD228A5),/* == 10^-204 */
U64(0x806BD971, 0x4632DFF6), U64(0x00BA1CD8, 0xA3DB53B7),/* == 10^-205 */
U64(0xCD795BE8, 0x70516656), U64(0x67902E27, 0x6C921F8B),/* == 10^-206 */
U64(0xA4611653, 0x8D0DEB78), U64(0x52D9BE85, 0xF074E609),/* == 10^-207 */
U64(0x8380DEA9, 0x3DA4BC60), U64(0x4247CB9E, 0x59F71E6D),/* == 10^-208 */
U64(0xD267CAA8, 0x62A12D66), U64(0xD072DF63, 0xC324FD7C),/* == 10^-209 */
U64(0xA8530886, 0xB54DBDEB), U64(0xD9F57F83, 0x0283FDFD),/* == 10^-210 */
U64(0x86A8D39E, 0xF77164BC), U64(0xAE5DFF9C, 0x02033197),/* == 10^-211 */
U64(0xD77485CB, 0x25823AC7), U64(0x7D633293, 0x366B828B),/* == 10^-212 */
U64(0xAC5D37D5, 0xB79B6239), U64(0x311C2875, 0xC522CED6),/* == 10^-213 */
U64(0x89E42CAA, 0xF9491B60), U64(0xF41686C4, 0x9DB57245),/* == 10^-214 */
U64(0xDCA04777, 0xF541C567), U64(0xECF0D7A0, 0xFC5583A1),/* == 10^-215 */
U64(0xB080392C, 0xC4349DEC), U64(0xBD8D794D, 0x96AACFB4),/* == 10^-216 */
U64(0x8D3360F0, 0x9CF6E4BD), U64(0x64712DD7, 0xABBBD95D),/* == 10^-217 */
U64(0xE1EBCE4D, 0xC7F16DFB), U64(0xD3E84959, 0x12C62894),/* == 10^-218 */
U64(0xB4BCA50B, 0x065ABE63), U64(0x0FED077A, 0x756B53AA),/* == 10^-219 */
U64(0x9096EA6F, 0x3848984F), U64(0x3FF0D2C8, 0x5DEF7622),/* == 10^-220 */
U64(0xE757DD7E, 0xC07426E5), U64(0x331AEADA, 0x2FE589CF),/* == 10^-221 */
U64(0xB9131798, 0x99F68584), U64(0x28E2557B, 0x59846E3F),/* == 10^-222 */
U64(0x940F4613, 0xAE5ED136), U64(0x871B7795, 0xE136BE99),/* == 10^-223 */
U64(0xECE53CEC, 0x4A314EBD), U64(0xA4F8BF56, 0x35246428),/* == 10^-224 */
U64(0xBD8430BD, 0x08277231), U64(0x50C6FF78, 0x2A838353),/* == 10^-225 */
U64(0x979CF3CA, 0x6CEC5B5A), U64(0xA705992C, 0xEECF9C43),/* == 10^-226 */
U64(0xF294B943, 0xE17A2BC4), U64(0x3E6F5B7B, 0x17B2939E),/* == 10^-227 */
U64(0xC2109436, 0x4DFB5636), U64(0x985915FC, 0x12F542E5),/* == 10^-228 */
U64(0x9B407691, 0xD7FC44F8), U64(0x79E0DE63, 0x425DCF1D),/* == 10^-229 */
U64(0xF867241C, 0x8CC6D4C0), U64(0xC30163D2, 0x03C94B62),/* == 10^-230 */
U64(0xC6B8E9B0, 0x709F109A), U64(0x359AB641, 0x9CA1091B),/* == 10^-231 */
U64(0x9EFA548D, 0x26E5A6E1), U64(0xC47BC501, 0x4A1A6DB0),/* == 10^-232 */
U64(0xFE5D5415, 0x0B090B02), U64(0xD3F93B35, 0x435D7C4C),/* == 10^-233 */
U64(0xCB7DDCDD, 0xA26DA268), U64(0xA9942F5D, 0xCF7DFD0A),/* == 10^-234 */
U64(0xA2CB1717, 0xB52481ED), U64(0x54768C4B, 0x0C64CA6E),/* == 10^-235 */
U64(0x823C1279, 0x5DB6CE57), U64(0x76C53D08, 0xD6B70858),/* == 10^-236 */
U64(0xD0601D8E, 0xFC57B08B), U64(0xF13B94DA, 0xF124DA27),/* == 10^-237 */
U64(0xA6B34AD8, 0xC9DFC06F), U64(0xF42FAA48, 0xC0EA481F),/* == 10^-238 */
U64(0x855C3BE0, 0xA17FCD26), U64(0x5CF2EEA0, 0x9A55067F),/* == 10^-239 */
U64(0xD5605FCD, 0xCF32E1D6), U64(0xFB1E4A9A, 0x90880A65),/* == 10^-240 */
U64(0xAAB37FD7, 0xD8F58178), U64(0xC8E5087B, 0xA6D33B84),/* == 10^-241 */
U64(0x888F9979, 0x7A5E012D), U64(0x6D8406C9, 0x52429603),/* == 10^-242 */
U64(0xDA7F5BF5, 0x90966848), U64(0xAF39A475, 0x506A899F),/* == 10^-243 */
U64(0xAECC4991, 0x4078536D), U64(0x58FAE9F7, 0x73886E19),/* == 10^-244 */
U64(0x8BD6A141, 0x006042BD), U64(0xE0C8BB2C, 0x5C6D24E0),/* == 10^-245 */
U64(0xDFBDCECE, 0x67006AC9), U64(0x67A791E0, 0x93E1D49A),/* == 10^-246 */
U64(0xB2FE3F0B, 0x8599EF07), U64(0x861FA7E6, 0xDCB4AA15),/* == 10^-247 */
U64(0x8F31CC09, 0x37AE58D2), U64(0xD1B2ECB8, 0xB0908811),/* == 10^-248 */
U64(0xE51C79A8, 0x5916F484), U64(0x82B7E127, 0x80E7401B),/* == 10^-249 */
U64(0xB749FAED, 0x14125D36), U64(0xCEF980EC, 0x671F667C),/* == 10^-250 */
U64(0x92A1958A, 0x7675175F), U64(0x0BFACD89, 0xEC191ECA),/* == 10^-251 */
U64(0xEA9C2277, 0x23EE8BCB), U64(0x465E15A9, 0x79C1CADC),/* == 10^-252 */
U64(0xBBB01B92, 0x83253CA2), U64(0x9EB1AAED, 0xFB016F16),/* == 10^-253 */
U64(0x96267C75, 0x35B763B5), U64(0x4BC1558B, 0x2F3458DF),/* == 10^-254 */
U64(0xF03D93EE, 0xBC589F88), U64(0x793555AB, 0x7EBA27CB),/* == 10^-255 */
U64(0xC0314325, 0x637A1939), U64(0xFA911155, 0xFEFB5309),/* == 10^-256 */
U64(0x99C10284, 0x4F94E0FB), U64(0x2EDA7444, 0xCBFC426D),/* == 10^-257 */
U64(0xF6019DA0, 0x7F549B2B), U64(0x7E2A53A1, 0x46606A48),/* == 10^-258 */
U64(0xC4CE17B3, 0x99107C22), U64(0xCB550FB4, 0x384D21D4),/* == 10^-259 */
U64(0x9D71AC8F, 0xADA6C9B5), U64(0x6F773FC3, 0x603DB4A9),/* == 10^-260 */
U64(0xFBE91419, 0x15D7A922), U64(0x4BF1FF9F, 0x0062BAA8),/* == 10^-261 */
U64(0xC9874347, 0x44AC874E), U64(0xA327FFB2, 0x66B56220),/* == 10^-262 */
U64(0xA139029F, 0x6A239F72), U64(0x1C1FFFC1, 0xEBC44E80),/* == 10^-263 */
U64(0x80FA687F, 0x881C7F8E), U64(0x7CE66634, 0xBC9D0B9A),/* == 10^-264 */
U64(0xCE5D73FF, 0x402D98E3), U64(0xFB0A3D21, 0x2DC81290),/* == 10^-265 */
U64(0xA5178FFF, 0x668AE0B6), U64(0x626E974D, 0xBE39A873),/* == 10^-266 */
U64(0x8412D999, 0x1ED58091), U64(0xE858790A, 0xFE9486C2),/* == 10^-267 */
U64(0xD3515C28, 0x31559A83), U64(0x0D5A5B44, 0xCA873E04),/* == 10^-268 */
U64(0xA90DE353, 0x5AAAE202), U64(0x711515D0, 0xA205CB36),/* == 10^-269 */
U64(0x873E4F75, 0xE2224E68), U64(0x5A7744A6, 0xE804A292),/* == 10^-270 */
U64(0xD863B256, 0x369D4A40), U64(0x90BED43E, 0x40076A83),/* == 10^-271 */
U64(0xAD1C8EAB, 0x5EE43B66), U64(0xDA324365, 0x0005EECF),/* == 10^-272 */
U64(0x8A7D3EEF, 0x7F1CFC52), U64(0x482835EA, 0x666B2572),/* == 10^-273 */
U64(0xDD95317F, 0x31C7FA1D), U64(0x40405643, 0xD711D584),/* == 10^-274 */
U64(0xB1442798, 0xF49FFB4A), U64(0x99CD11CF, 0xDF41779D),/* == 10^-275 */
U64(0x8DD01FAD, 0x907FFC3B), U64(0xAE3DA7D9, 0x7F6792E4),/* == 10^-276 */
U64(0xE2E69915, 0xB3FFF9F9), U64(0x16C90C8F, 0x323F516D),/* == 10^-277 */
U64(0xB5854744, 0x8FFFFB2D), U64(0xABD40A0C, 0x2832A78A),/* == 10^-278 */
U64(0x91376C36, 0xD99995BE), U64(0x23100809, 0xB9C21FA2),/* == 10^-279 */
U64(0xE858AD24, 0x8F5C22C9), U64(0xD1B3400F, 0x8F9CFF69),/* == 10^-280 */
U64(0xB9E08A83, 0xA5E34F07), U64(0xDAF5CCD9, 0x3FB0CC54),/* == 10^-281 */
U64(0x94B3A202, 0xEB1C3F39), U64(0x7BF7D714, 0x32F3D6AA),/* == 10^-282 */
U64(0xEDEC366B, 0x11C6CB8F), U64(0x2CBFBE86, 0xB7EC8AA9),/* == 10^-283 */
U64(0xBE5691EF, 0x416BD60C), U64(0x23CC986B, 0xC656D554),/* == 10^-284 */
U64(0x9845418C, 0x345644D6), U64(0x830A1389, 0x6B78AAAA),/* == 10^-285 */
U64(0xF3A20279, 0xED56D48A), U64(0x6B435275, 0x78C1110F),/* == 10^-286 */
U64(0xC2E801FB, 0x244576D5), U64(0x229C41F7, 0x93CDA73F),/* == 10^-287 */
U64(0x9BECCE62, 0x836AC577), U64(0x4EE367F9, 0x430AEC33),/* == 10^-288 */
U64(0xF97AE3D0, 0xD2446F25), U64(0x4B057328, 0x6B44AD1E),/* == 10^-289 */
U64(0xC795830D, 0x75038C1D), U64(0xD59DF5B9, 0xEF6A2418),/* == 10^-290 */
U64(0x9FAACF3D, 0xF73609B1), U64(0x77B19161, 0x8C54E9AD),/* == 10^-291 */
U64(0xFF77B1FC, 0xBEBCDC4F), U64(0x25E8E89C, 0x13BB0F7B),/* == 10^-292 */
U64(0xCC5FC196, 0xFEFD7D0C), U64(0x1E53ED49, 0xA96272C9),/* == 10^-293 */
U64(0xA37FCE12, 0x6597973C), U64(0xE50FF107, 0xBAB528A1),/* == 10^-294 */
U64(0x82CCA4DB, 0x847945CA), U64(0x50D98D9F, 0xC890ED4D),/* == 10^-295 */
U64(0xD1476E2C, 0x07286FAA), U64(0x1AF5AF66, 0x0DB4AEE2),/* == 10^-296 */
U64(0xA76C5823, 0x38ED2621), U64(0xAF2AF2B8, 0x0AF6F24E),/* == 10^-297 */
U64(0x85F04682, 0x93F0EB4E), U64(0x25BBF560, 0x08C58EA5),/* == 10^-298 */
U64(0xD64D3D9D, 0xB981787D), U64(0x092CBBCC, 0xDAD5B108),/* == 10^-299 */
U64(0xAB70FE17, 0xC79AC6CA), U64(0x6DBD630A, 0x48AAF407),/* == 10^-300 */
U64(0x892731AC, 0x9FAF056E), U64(0xBE311C08, 0x3A225CD2),/* == 10^-301 */
U64(0xDB71E914, 0x32B1A24A), U64(0xC9E82CD9, 0xF69D6150),/* == 10^-302 */
U64(0xAF8E5410, 0x288E1B6F), U64(0x07ECF0AE, 0x5EE44DDA),/* == 10^-303 */
U64(0x8C71DCD9, 0xBA0B4925), U64(0x9FF0C08B, 0x7F1D0B15),/* == 10^-304 */
U64(0xE0B62E29, 0x29ABA83C), U64(0x331ACDAB, 0xFE94DE87),/* == 10^-305 */
U64(0xB3C4F1BA, 0x87BC8696), U64(0x8F48A489, 0x9877186C),/* == 10^-306 */
U64(0x8FD0C162, 0x06306BAB), U64(0xA5D3B6D4, 0x79F8E057),/* == 10^-307 */
U64(0xE61ACF03, 0x3D1A45DF), U64(0x6FB92487, 0x298E33BE),/* == 10^-308 */
U64(0xB8157268, 0xFDAE9E4C), U64(0x5960EA05, 0xBAD82965),/* == 10^-309 */
U64(0x93445B87, 0x31587EA3), U64(0x7AB3EE6A, 0xFBE0211D),/* == 10^-310 */
U64(0xEBA09271, 0xE88D976B), U64(0xF7864A44, 0xC633682F),/* == 10^-311 */
U64(0xBC807527, 0xED3E12BC), U64(0xC6050837, 0x04F5ECF2),/* == 10^-312 */
U64(0x96CD2A86, 0x5764DBCA), U64(0x38040692, 0x6A5E5728),/* == 10^-313 */
U64(0xF148440A, 0x256E2C76), U64(0xC00670EA, 0x43CA250D),/* == 10^-314 */
U64(0xC1069CD4, 0xEABE89F8), U64(0x999EC0BB, 0x696E840A),/* == 10^-315 */
U64(0x9A6BB0AA, 0x55653B2D), U64(0x47B233C9, 0x2125366F),/* == 10^-316 */
U64(0xF712B443, 0xBBD52B7B), U64(0xA5E9EC75, 0x01D523E4),/* == 10^-317 */
U64(0xC5A89036, 0x2FDDBC62), U64(0xEB2189F7, 0x34AA831D),/* == 10^-318 */
U64(0x9E20735E, 0x8CB16382), U64(0x55B46E5F, 0x5D5535B1),/* == 10^-319 */
U64(0xFD00B897, 0x478238D0), U64(0x8920B098, 0x955522B5),/* == 10^-320 */
U64(0xCA66FA12, 0x9F9B60A6), U64(0xD41A26E0, 0x77774EF7),/* == 10^-321 */
U64(0xA1EBFB42, 0x19491A1F), U64(0x1014EBE6, 0xC5F90BF9),/* == 10^-322 */
U64(0x818995CE, 0x7AA0E1B2), U64(0x7343EFEB, 0xD1940994),/* == 10^-323 */
U64(0xCF42894A, 0x5DCE35EA), U64(0x52064CAC, 0x828675B9),/* == 10^-324 */
U64(0xA5CED43B, 0x7E3E9188), U64(0x419EA3BD, 0x35385E2E),/* == 10^-325 */
U64(0x84A57695, 0xFE98746D), U64(0x014BB630, 0xF7604B58),/* == 10^-326 */
U64(0xD43BF0EF, 0xFDC0BA48), U64(0x0212BD1B, 0x2566DEF3),/* == 10^-327 */
U64(0xA9C98D8C, 0xCB009506), U64(0x680EFDAF, 0x511F18C2),/* == 10^-328 */
U64(0x87D4713D, 0x6F33AA6B), U64(0x8672648C, 0x40E5AD68),/* == 10^-329 */
U64(0xD953E862, 0x4B85DD78), U64(0xD71D6DAD, 0x34A2AF0D),/* == 10^-330 */
U64(0xADDCB9E8, 0x3C6B1793), U64(0xDF4ABE24, 0x2A1BBF3E),/* == 10^-331 */
U64(0x8B16FB20, 0x3055AC76), U64(0x4C3BCB50, 0x21AFCC31),/* == 10^-332 */
U64(0xDE8B2B66, 0xB3BC4723), U64(0xAD2C7880, 0x35E61382),/* == 10^-333 */
U64(0xB208EF85, 0x5C969F4F), U64(0xBDBD2D33, 0x5E51A935),/* == 10^-334 */
U64(0x8E6D8C6A, 0xB0787F72), U64(0xFE30F0F5, 0xE50E20F7),/* == 10^-335 */
U64(0xE3E27A44, 0x4D8D98B7), U64(0xFD1B1B23, 0x08169B25),/* == 10^-336 */
U64(0xB64EC836, 0xA47146F9), U64(0x9748E282, 0x6CDEE284),/* == 10^-337 */
U64(0x91D8A02B, 0xB6C10594), U64(0x79071B9B, 0x8A4BE86A),/* == 10^-338 */
U64(0xE95A99DF, 0x8ACE6F53), U64(0xF4D82C2C, 0x107973DC),/* == 10^-339 */
U64(0xBAAEE17F, 0xA23EBF76), U64(0x5D79BCF0, 0x0D2DF64A),/* == 10^-340 */
U64(0x9558B466, 0x1B6565F8), U64(0x4AC7CA59, 0xA424C508),/* == 10^-341 */
U64(0xEEF453D6, 0x923BD65A), U64(0x113FAA29, 0x06A13B40),/* == 10^-342 */
U64(0xBF29DCAB, 0xA82FDEAE), U64(0x7432EE87, 0x3880FC33),/* == 10^-343 */
};

static const unsigned short kh[] = {2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 49, 50, 51, 52, 53, 54, 55, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 85, 86, 87, 88, 89, 90, 91, 93, 94, 95, 97, 98, 99, 100, 101, 102, 103, 105, 106, 107, 109, 110, 111, 112, 113, 114, 115, 117, 118, 119, 121, 122, 123, 125, 126, 127, 128, 129, 130, 131, 133, 134, 135, 137, 138, 139, 140, 141, 142, 143, 145, 146, 147, 149, 150, 151, 152, 153, 154, 155, 157, 158, 159, 161, 162, 163, 164, 165, 166, 167, 169, 170, 171, 173, 174, 175, 176, 177, 178, 179, 181, 182, 183, 185, 186, 187, 188, 189, 190, 191, 193, 194, 195, 197, 198, 199, 200, 201, 202, 203, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 217, 218, 219, 221, 222, 223, 224, 225, 226, 227, 229, 230, 231, 233, 234, 235, 236, 237, 238, 239, 241, 242, 243, 245, 246, 247, 249, 250, 251, 252, 253, 254, 255, 257, 258, 259, 261, 262, 263, 264, 265, 266, 267, 269, 270, 271, 273, 274, 275, 276, 277, 278, 279, 281, 282, 283, 285, 286, 287, 288, 289, 290, 291, 293, 294, 295, 297, 298, 299, 300, 301, 302, 303, 305, 306, 307, 309, 310, 311, 312, 313, 314, 315, 317, 318, 319, 321, 322, 323, 324, 325, 326, 327, 329, 330, 331, 333, 334, 335, 336, 337, 338, 339, 341, 342, 343, 345, 346, 347, 348, 349, 350, 351, 353, 354, 355, 357, 358, 359, 361, 362, 363, 364, 365, 366, 367, 369, 370, 371, 373, 374, 375, 376, 377, 378, 379, 381, 382, 383, 385, 386, 387, 388, 389, 390, 391, 393, 394, 395, 397, 398, 399, 400, 401, 402, 403, 405, 406, 407, 409, 410, 411, 412, 413, 414, 415, 417, 418, 419, 421, 422, 423, 424, 425, 426, 427, 429, 430, 431, 433, 434, 435, 436, 437, 438, 439, 441, 442, 443, 445, 446, 447, 448, 449, 450, 451, 453, 454, 455, 457, 458, 459, 460, 461, 462, 463, 465, 466, 467, 469, 470, 471, 472, 473, 474, 475, 477, 478, 479, 481, 482, 483, 485, 486, 487, 488, 489, 490, 491, 493, 494, 495, 497, 498, 499, 500, 501, 502, 503, 505, 506, 507, 509, 510, 511, 512, 513, 514, 515, 517, 518, 519, 521, 522, 523, 524, 525, 526, 527, 529, 530, 531, 533, 534, 535, 536, 537, 538, 539, 541, 542, 543, 545, 546, 547, 548, 549, 550, 551, 553, 554, 555, 557, 558, 559, 560, 561, 562, 563, 565, 566, 567, 569, 570, 571, 572, 573, 574, 575, 577, 578, 579, 581, 582, 583, 584, 585, 586, 587, 589, 590, 591, 593, 594, 595, 597, 598, 599, 600, 601, 602, 603, 605, 606, 607, 609, 610, 611, 612, 613, 614, 615, 617, 618, 619, 621, 622, 623, 624, 625, 626, 627, 629, 630, 631, 633, 634, 635, 636, 637, 638, 639, 641, 642, 643, 645, 646, 647, 648, 649, 650, 651, 653, 654, 655, 657, 658, 659, 660, 661, 662, 663, 665, 666, 667, 669, 670, 671, 672, 673, 674, 675, 677, 678, 679, 681, 682, 683, 684, 685, 686, 687, 689, 690, 691, 693, 694, 695, 696, 697, 698, 699, 701, 702, 703, 705, 706, 707, 709, 710, 711, 712, 713, 714, 715, 717, 718, 719, 721, 722, 723, 724, 725, 726, 727, 729, 730, 731, 733, 734, 735, 736, 737, 738, 739, 741, 742, 743, 745, 746, 747, 748, 749, 750, 751, 753, 754, 755, 757, 758, 759, 760, 761, 762, 763, 765, 766, 767, 769, 770, 771, 772, 773, 774, 775, 777, 778, 779, 781, 782, 783, 784, 785, 786, 787, 789, 790, 791, 793, 794, 795, 796, 797, 798, 799, 801, 802, 803, 805, 806, 807, 808, 809, 810, 811, 813, 814, 815, 817, 818, 819, 820, 821, 822, 823, 825, 826, 827, 829, 830, 831, 833, 834, 835, 836, 837, 838, 839, 841, 842, 843, 845, 846, 847, 848, 849, 850, 851, 853, 854, 855, 857, 858, 859, 860, 861, 862, 863, 865, 866, 867, 869, 870, 871, 872, 873, 874, 875, 877, 878, 879, 881, 882, 883, 884, 885, 886, 887, 889, 890, 891, 893, 894, 895, 896, 897, 898, 899, 901, 902, 903, 905, 906, 907, 908, 909, 910, 911, 913, 914, 915, 917, 918, 919, 920, 921, 922, 923, 925, 926, 927, 929, 930, 931, 932, 933, 934, 935, 937, 938, 939, 941, 942, 943, 945, 946, 947, 948, 949, 950, 951, 953, 954, 955, 957, 958, 959, 960, 961, 962, 963, 965, 966, 967, 969, 970, 971, 972, 973, 974, 975, 977, 978, 979, 981, 982, 983, 984, 985, 986, 987, 989, 990, 991, 993, 994, 995, 996, 997, 998, 999, 1001, 1002, 1003, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1013, 1014, 1015, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1025, 1026, 1027, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1037, 1038, 1039, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1049, 1050, 1051, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1061, 1062, 1063, 1065, 1066, 1067, 1069, 1070, 1071, 1072, 1073, 1074, 1075, 1077, 1078, 1079, 1081, 1082, 1083, 1084, 1085, 1086, 1087, 1089, 1090, 1091, 1093, 1094, 1095, 1096, 1097, 1098, 1099, 1101, 1102, 1103, 1105, 1106, 1107, 1108, 1109, 1110, 1111, 1113, 1114, 1115, 1117, 1118, 1119, 1120, 1121, 1122, 1123, 1125, 1126, 1127, 1129, 1130, 1131, 1132, 1133, 1134, 1135, 1137, 1138, 1139, 1141, 1142, 1143, 1144, 1145, 1146, 1147, 1149, 1150, 1151, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1161, 1162, 1163, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1173, 1174, 1175, 1177, 1178, 1179, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 1189, 1190, 1191, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1201, 1202, 1203, 1205, 1206, 1207, 1208, 1209, 1210, 1211, 1213, 1214, 1215, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1225, 1226, 1227, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1237, 1238, 1239, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1249, 1250, 1251, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1261, 1262, 1263, 1265, 1266, 1267, 1268, 1269, 1270, 1271, 1273, 1274, 1275, 1277, 1278, 1279, 1280, 1281, 1282, 1283, 1285, 1286, 1287, 1289, 1290, 1291, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1301, 1302, 1303, 1305, 1306, 1307, 1308, 1309, 1310, 1311, 1313, 1314, 1315, 1317, 1318, 1319, 1320, 1321, 1322, 1323, 1325, 1326, 1327, 1329, 1330, 1331, 1332, 1333, 1334, 1335, 1337, 1338, 1339, 1341, 1342, 1343, 1344, 1345, 1346, 1347, 1349, 1350, 1351, 1353, 1354, 1355, 1356, 1357, 1358, 1359, 1361, 1362, 1363, 1365, 1366, 1367, 1368, 1369, 1370, 1371, 1373, 1374, 1375, 1377, 1378, 1379, 1380, 1381, 1382, 1383, 1385, 1386, 1387, 1389, 1390, 1391, 1392, 1393, 1394, 1395, 1397, 1398, 1399, 1401, 1402, 1403, 1404, 1405, 1406, 1407, 1409, 1410, 1411, 1413, 1414, 1415, 1417, 1418, 1419, 1420, 1421, 1422, 1423, 1425, 1426, 1427, 1429, 1430, 1431, 1432, 1433, 1434, 1435, 1437, 1438, 1439, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1449, 1450, 1451, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1461, 1462, 1463, 1465, 1466, 1467, 1468, 1469, 1470, 1471, 1473, 1474, 1475, 1477, 1478, 1479, 1480, 1481, 1482, 1483, 1485, 1486, 1487, 1489, 1490, 1491, 1492, 1493, 1494, 1495, 1497, 1498, 1499, 1501, 1502, 1503, 1504, 1505, 1506, 1507, 1509, 1510, 1511, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 1521, 1522, 1523, 1525, 1526, 1527, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1537, 1538, 1539, 1541, 1542, 1543, 1544, 1545, 1546, 1547, 1549, 1550, 1551, 1553, 1554, 1555, 1556, 1557, 1558, 1559, 1561, 1562, 1563, 1565, 1566, 1567, 1568, 1569, 1570, 1571, 1573, 1574, 1575, 1577, 1578, 1579, 1580, 1581, 1582, 1583, 1585, 1586, 1587, 1589, 1590, 1591, 1592, 1593, 1594, 1595, 1597, 1598, 1599, 1601, 1602, 1603, 1604, 1605, 1606, 1607, 1609, 1610, 1611, 1613, 1614, 1615, 1616, 1617, 1618, 1619, 1621, 1622, 1623, 1625, 1626, 1627, 1628, 1629, 1630, 1631, 1633, 1634, 1635, 1637, 1638, 1639, 1640, 1641, 1642, 1643, 1645, 1646, 1647, 1649, 1650, 1651, 1653, 1654, 1655, 1656, 1657, 1658, 1659, 1661, 1662, 1663, 1665, 1666, 1667, 1668, 1669, 1670, 1671, 1673, 1674, 1675, 1677, 1678, 1679, 1680, 1681, 1682, 1683, 1685, 1686, 1687, 1689, 1690, 1691, 1692, 1693, 1694, 1695, 1697, 1698, 1699, 1701, 1702, 1703, 1704, 1705, 1706, 1707, 1709, 1710, 1711, 1713, 1714, 1715, 1716, 1717, 1718, 1719, 1721, 1722, 1723, 1725, 1726, 1727, 1728, 1729, 1730, 1731, 1733, 1734, 1735, 1737, 1738, 1739, 1740, 1741, 1742, 1743, 1745, 1746, 1747, 1749, 1750, 1751, 1752, 1753, 1754, 1755, 1757, 1758, 1759, 1761, 1762, 1763, 1765, 1766, 1767, 1768, 1769, 1770, 1771, 1773, 1774, 1775, 1777, 1778, 1779, 1780, 1781, 1782, 1783, 1785, 1786, 1787, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1797, 1798, 1799, 1801, 1802, 1803, 1804, 1805, 1806, 1807, 1809, 1810, 1811, 1813, 1814, 1815, 1816, 1817, 1818, 1819, 1821, 1822, 1823, 1825, 1826, 1827, 1828, 1829, 1830, 1831, 1833, 1834, 1835, 1837, 1838, 1839, 1840, 1841, 1842, 1843, 1845, 1846, 1847, 1849, 1850, 1851, 1852, 1853, 1854, 1855, 1857, 1858, 1859, 1861, 1862, 1863, 1864, 1865, 1866, 1867, 1869, 1870, 1871, 1873, 1874, 1875, 1876, 1877, 1878, 1879, 1881, 1882, 1883, 1885, 1886, 1887, 1889, 1890, 1891, 1892, 1893, 1894, 1895, 1897, 1898, 1899, 1901, 1902, 1903, 1904, 1905, 1906, 1907, 1909, 1910, 1911, 1913, 1914, 1915, 1916, 1917, 1918, 1919, 1921, 1922, 1923, 1925, 1926, 1927, 1928, 1929, 1930, 1931, 1933, 1934, 1935, 1937, 1938, 1939, 1940, 1941, 1942, 1943, 1945, 1946, 1947, 1949, 1950, 1951, 1952, 1953, 1954, 1955, 1957, 1958, 1959, 1961, 1962, 1963, 1964, 1965, 1966, 1967, 1969, 1970, 1971, 1973, 1974, 1975, 1976, 1977, 1978, 1979, 1981, 1982, 1983, 1985, 1986, 1987, 1988, 1989, 1990, 1991, 1993, 1994, 1995, 1997, 1998, 1999, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2009, 2010, 2011, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2021, 2022, 2023, 2025, 2026, 2027, 2028, 2029, 2030, 2031, 2033, 2034, 2035, 2037, 2038, 2039, 2040, 2041, 2042, 2043, 2045, 2046, 2047, 2049, 2050, 2051, 2052, 2053, 2054, 2055, 2057, 2058, 2059, 2061, 2062, 2063, 2064, 2065, 2066, 2067, 2069, 2070, 2071, 2073, 2074, 2075, 2076, 2077, 2078, 2079, 2081, 2082, 2083, 2085, 2086, 2087, 2088, 2089, 2090, 2091, 2093, 2094, 2095, 2097, 2098, 2099, 2100, 2101, 2102, 2103, 2105, 2106, 2107, 2109, 2110, 2111, 2113, 2114, 2115, 2116, 2117, 2118, 2119, 2121, 2122, 2123, 2125, 2126, 2127, 2128, 2129, 2130, 2131, 2133, 2134, 2135, 2137, 2138, 2139, 2140, 2141, 2142, 2143, 2145, 2146, 2147, 2149, 2150, 2151, 2152, 2153, 2154, 2155, 2157, 2158, 2159, 2161, 2162, 2163, 2164, 2165, 2166, 2167, 2169, 2170, 2171, 2173, 2174, 2175, 2176, 2177, 2178, 2179, 2181, 2182, 2183, 2185, 2186, 2187, 2188, 2189, 2190, 2191, 2193, 2194, 2195, 2197, 2198, 2199, 2200, 2201, 2202, 2203, 2205, 2206, 2207, 2209, 2210, 2211, 2212, 2213, 2214, 2215, 2217, 2218, 2219, 2221, 2222, 2223, 2224, 2225, 2226, 2227, 2229, 2230, 2231, 2233, 2234, 2235, 2237, 2238, 2239, 2240, 2241, 2242, 2243, 2245, 2246, 2247, 2249, 2250, 2251, 2252, 2253, 2254, 2255, 2257, 2258, 2259, 2261, 2262, 2263, 2264, 2265, 2266, 2267, 2269, 2270, 2271, 2273, 2274, 2275, 2276, 2277, 2278, 2279, 2281, 2282, 2283, 2285, 2286, 2287, 2288, 2289, 2290, 2291, 2293, 2294, 2295, 2297, 2298, 2299, 2300, 2301, 2302, 2303, 2305, 2306, 2307, 2309, 2310, 2311, 2312, 2313, 2314, 2315, 2317, 2318, 2319, 2321, 2322, 2323, 2324, 2325, 2326, 2327, 2329, 2330, 2331, 2333, 2334, 2335, 2336, 2337, 2338, 2339, 2341, 2342, 2343, 2345, 2346, 2347, 2349, 2350, 2351, 2352, 2353, 2354, 2355, 2357, 2358, 2359, 2361, 2362, 2363, 2364, 2365, 2366, 2367, 2369, 2370, 2371, 2373, 2374, 2375, 2376, 2377, 2378, 2379, 2381, 2382, 2383, 2385, 2386, 2387, 2388, 2389, 2390, 2391, 2393, 2394, 2395, 2397, 2398, 2399, 2400, 2401, 2402, 2403, 2405, 2406, 2407, 2409, 2410, 2411, 2412, 2413, 2414, 2415, 2417, 2418, 2419, 2421, 2422, 2423, 2424, 2425, 2426, 2427, 2429, 2430, 2431, 2433, 2434, 2435, 2436, 2437, 2438, 2439, 2441, 2442, 2443, 2445, 2446, 2447, 2448, 2449, 2450, 2451, 2453, 2454, 2455, 2457, 2458, 2459, 2460, 2461, 2462, 2463, 2465};
/**
 Get the cached pow10 value from pow10_sig_table.
 @param exp10 The exponent of pow(10, e). This value must in range
              POW10_SIG_TABLE_MIN_EXP to POW10_SIG_TABLE_MAX_EXP.
 @param hi    The highest 64 bits of pow(10, e).
 @param lo    The lower 64 bits after `hi`.
 */
static_inline void pow10_table_get_sig(i32 exp10, u64 *hi, u64 *lo)
{
    i32 idx = exp10 - (POW10_SIG_TABLE_MIN_EXP);
    const u64 *sig = pow10_sig_table + idx * 2;
    //const u64 *sig = pow10_sig_table_round + idx * 2;

    // const u64 * offset =  pow10_sig_table - POW10_SIG_TABLE_MIN_EXP * 2 ;
    // const u64 * sig = &offset[exp10*2];
    *hi = sig[0];
    *lo = sig[1];
}

/**
 Get the exponent (base 2) for highest 64 bits significand in pow10_sig_table.
 */
static_inline void pow10_table_get_exp(i32 exp10, i32 *exp2)
{
    /* e2 = floor(log2(pow(10, e))) - 64 + 1 */
    /*    = floor(e * log2(10) - 63)         */
    *exp2 = (exp10 * 217706 - 4128768) >> 16;
}

/*==============================================================================
 * Digit Character Matcher
 *============================================================================*/

/** Digit type */
typedef u8 digi_type;

/** Digit: '0'. */
static const digi_type DIGI_TYPE_ZERO = 1 << 0;

/** Digit: [1-9]. */
static const digi_type DIGI_TYPE_NONZERO = 1 << 1;

/** Plus sign (positive): '+'. */
static const digi_type DIGI_TYPE_POS = 1 << 2;

/** Minus sign (negative): '-'. */
static const digi_type DIGI_TYPE_NEG = 1 << 3;

/** Decimal point: '.' */
static const digi_type DIGI_TYPE_DOT = 1 << 4;

/** Exponent sign: 'e, 'E'. */
static const digi_type DIGI_TYPE_EXP = 1 << 5;

/** Digit type table (generate with misc/make_tables.c) */
static const digi_type digi_table[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x10, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/** Match a character with specified type. */
static_inline bool digi_is_type(u8 d, digi_type type)
{
    return (digi_table[d] & type) != 0;
}

/** Match a sign: '+', '-' */
static_inline bool digi_is_sign(u8 d)
{
    return digi_is_type(d, DIGI_TYPE_POS | DIGI_TYPE_NEG);
}

/** Match a none zero digit: [1-9] */
static_inline bool digi_is_nonzero(u8 d)
{
    return digi_is_type(d, DIGI_TYPE_NONZERO);
}

/** Match a digit: [0-9] */
static_inline bool digi_is_digit(u8 d)
{
    return digi_is_type(d, DIGI_TYPE_ZERO | DIGI_TYPE_NONZERO);
}

/** Match an exponent sign: 'e', 'E'. */
static_inline bool digi_is_exp(u8 d)
{
    return digi_is_type(d, DIGI_TYPE_EXP);
}

/** Match a floating point indicator: '.', 'e', 'E'. */
static_inline bool digi_is_fp(u8 d)
{
    return digi_is_type(d, DIGI_TYPE_DOT | DIGI_TYPE_EXP);
}

/** Match a digit or floating point indicator: [0-9], '.', 'e', 'E'. */
static_inline bool digi_is_digit_or_fp(u8 d)
{
    return digi_is_type(d, DIGI_TYPE_ZERO | DIGI_TYPE_NONZERO |
                               DIGI_TYPE_DOT | DIGI_TYPE_EXP);
}

/** Read 'Inf' or 'Infinity' literal (ignoring case). */
static_inline bool read_inf(bool sign, u8 *cur, u8 **end, double *val)
{
    if ((cur[0] == 'I' || cur[0] == 'i') &&
        (cur[1] == 'N' || cur[1] == 'n') &&
        (cur[2] == 'F' || cur[2] == 'f'))
    {
        if ((cur[3] == 'I' || cur[3] == 'i') &&
            (cur[4] == 'N' || cur[4] == 'n') &&
            (cur[5] == 'I' || cur[5] == 'i') &&
            (cur[6] == 'T' || cur[6] == 't') &&
            (cur[7] == 'Y' || cur[7] == 'y'))
        {
            *end = cur + 8;
        }
        else
        {
            *end = cur + 3;
        }
        *val = sign ? -INFINITY : INFINITY;
        return true;
    }
    return false;
}

/** Read 'NaN' literal (ignoring case). */
static_inline bool read_nan(bool sign, u8 *cur, u8 **end, double *val)
{
    if ((cur[0] == 'N' || cur[0] == 'n') &&
        (cur[1] == 'A' || cur[1] == 'a') &&
        (cur[2] == 'N' || cur[2] == 'n'))
    {
        *end = cur + 3;
        *val = NAN;
        return true;
    }
    return false;
}

/** Read 'Inf', 'Infinity' or 'NaN' literal (ignoring case). */
static_inline bool read_inf_or_nan(bool sign, u8 *cur, u8 **end, double *val)
{
    if (read_inf(sign, cur, end, val))
        return true;
    if (read_nan(sign, cur, end, val))
        return true;
    return false;
}

/*==============================================================================
 * BigInt For Floating Point Number Reader
 *
 * The bigint algorithm is used by floating-point number parser to get correctly
 * rounded result for numbers with lots of digits. This part of code is rarely
 * used for normal JSON.
 *============================================================================*/

/** Maximum exponent of exact pow10 */
#define U64_POW10_MAX_EXP 19

/** Table: [ 10^0, ..., 10^19 ] (generate with misc/make_tables.c) */
static const u64 u64_pow10_table[U64_POW10_MAX_EXP + 1] = {
    U64(0x00000000, 0x00000001), U64(0x00000000, 0x0000000A),
    U64(0x00000000, 0x00000064), U64(0x00000000, 0x000003E8),
    U64(0x00000000, 0x00002710), U64(0x00000000, 0x000186A0),
    U64(0x00000000, 0x000F4240), U64(0x00000000, 0x00989680),
    U64(0x00000000, 0x05F5E100), U64(0x00000000, 0x3B9ACA00),
    U64(0x00000002, 0x540BE400), U64(0x00000017, 0x4876E800),
    U64(0x000000E8, 0xD4A51000), U64(0x00000918, 0x4E72A000),
    U64(0x00005AF3, 0x107A4000), U64(0x00038D7E, 0xA4C68000),
    U64(0x002386F2, 0x6FC10000), U64(0x01634578, 0x5D8A0000),
    U64(0x0DE0B6B3, 0xA7640000), U64(0x8AC72304, 0x89E80000)};

/** Maximum numbers of chunks used by a bigint (58 is enough here). */
#define BIGINT_MAX_CHUNKS 64

/** Unsigned arbitrarily large integer */
typedef struct bigint
{
    u32 used;                    /* used chunks count, should not be 0 */
    u64 bits[BIGINT_MAX_CHUNKS]; /* chunks */
} bigint;

/**
 Evaluate 'big += val'.
 @param big A big number (can be 0).
 @param val An unsigned integer (can be 0).
 */
static_inline void bigint_add_u64(bigint *big, u64 val)
{
    u32 idx, max;
    u64 num = big->bits[0];
    u64 add = num + val;
    big->bits[0] = add;
    if (likely((add >= num) || (add >= val)))
        return;
    for ((void)(idx = 1), max = big->used; idx < max; idx++)
    {
        if (likely(big->bits[idx] != U64_MAX))
        {
            big->bits[idx] += 1;
            return;
        }
        big->bits[idx] = 0;
    }
    big->bits[big->used++] = 1;
}

/**
 Evaluate 'big *= val'.
 @param big A big number (can be 0).
 @param val An unsigned integer (cannot be 0).
 */
static_inline void bigint_mul_u64(bigint *big, u64 val)
{
    u32 idx = 0, max = big->used;
    u64 hi, lo, carry = 0;
    for (; idx < max; idx++)
    {
        if (big->bits[idx])
            break;
    }
    for (; idx < max; idx++)
    {
        u128_mul_add(big->bits[idx], val, carry, &hi, &lo);
        big->bits[idx] = lo;
        carry = hi;
    }
    if (carry)
        big->bits[big->used++] = carry;
}

/**
 Evaluate 'big *= 2^exp'.
 @param big A big number (can be 0).
 @param exp An exponent integer (can be 0).
 */
static_inline void bigint_mul_pow2(bigint *big, u32 exp)
{
    u32 shft = exp % 64;
    u32 move = exp / 64;
    u32 idx = big->used;
    if (unlikely(shft == 0))
    {
        for (; idx > 0; idx--)
        {
            big->bits[idx + move - 1] = big->bits[idx - 1];
        }
        big->used += move;
        while (move)
            big->bits[--move] = 0;
    }
    else
    {
        big->bits[idx] = 0;
        for (; idx > 0; idx--)
        {
            u64 num = big->bits[idx] << shft;
            num |= big->bits[idx - 1] >> (64 - shft);
            big->bits[idx + move] = num;
        }
        big->bits[move] = big->bits[0] << shft;
        big->used += move + (big->bits[big->used + move] > 0);
        while (move)
            big->bits[--move] = 0;
    }
}

/**
 Evaluate 'big *= 10^exp'.
 @param big A big number (can be 0).
 @param exp An exponent integer (cannot be 0).
 */
static_inline void bigint_mul_pow10(bigint *big, i32 exp)
{
    for (; exp >= U64_POW10_MAX_EXP; exp -= U64_POW10_MAX_EXP)
    {
        bigint_mul_u64(big, u64_pow10_table[U64_POW10_MAX_EXP]);
    }
    if (exp)
    {
        bigint_mul_u64(big, u64_pow10_table[exp]);
    }
}

/**
 Compare two bigint.
 @return -1 if 'a < b', +1 if 'a > b', 0 if 'a == b'.
 */
static_inline i32 bigint_cmp(bigint *a, bigint *b)
{
    u32 idx = a->used;
    if (a->used < b->used)
        return -1;
    if (a->used > b->used)
        return +1;
    while (idx-- > 0)
    {
        u64 av = a->bits[idx];
        u64 bv = b->bits[idx];
        if (av < bv)
            return -1;
        if (av > bv)
            return +1;
    }
    return 0;
}

/**
 Evaluate 'big = val'.
 @param big A big number (can be 0).
 @param val An unsigned integer (can be 0).
 */
static_inline void bigint_set_u64(bigint *big, u64 val)
{
    big->used = 1;
    big->bits[0] = val;
}

/** Set a bigint with floating point number string. */
static_noinline void bigint_set_buf(bigint *big, u64 sig, i32 *exp,
                                    u8 *sig_cut, u8 *sig_end, u8 *dot_pos)
{

    if (unlikely(!sig_cut))
    {
        /* no digit cut, set significant part only */
        bigint_set_u64(big, sig);
        return;
    }
    else
    {
        /* some digits was cut, read them from 'sig_cut' to 'sig_end' */
        u8 *hdr = sig_cut;
        u8 *cur = hdr;
        u32 len = 0;
        u64 val = 0;
        bool dig_big_cut = false;
        bool has_dot = (hdr < dot_pos) & (dot_pos < sig_end);
        u32 dig_len_total = U64_SAFE_DIG + (u32)(sig_end - hdr) - has_dot;

        sig -= (*sig_cut >= '5'); /* sig was rounded before */
        if (dig_len_total > F64_MAX_DEC_DIG)
        {
            dig_big_cut = true;
            sig_end -= dig_len_total - (F64_MAX_DEC_DIG + 1);
            sig_end -= (dot_pos + 1 == sig_end);
            dig_len_total = (F64_MAX_DEC_DIG + 1);
        }
        *exp -= (i32)dig_len_total - U64_SAFE_DIG;

        big->used = 1;
        big->bits[0] = sig;
        while (cur < sig_end)
        {
            if (likely(cur != dot_pos))
            {
                val = val * 10 + (*cur++ - '0');
                len++;
                if (unlikely(cur == sig_end && dig_big_cut))
                {
                    /* The last digit must be non-zero,    */
                    /* set it to '1' for correct rounding. */
                    val = val - (val % 10) + 1;
                }
                if (len == U64_SAFE_DIG || cur == sig_end)
                {
                    bigint_mul_pow10(big, (i32)len);
                    bigint_add_u64(big, val);
                    val = 0;
                    len = 0;
                }
            }
            else
            {
                cur++;
            }
        }
    }
}

/*==============================================================================
 * Diy Floating Point
 *============================================================================*/

/** "Do It Yourself Floating Point" struct. */
typedef struct diy_fp
{
    u64 sig; /* significand */
    i32 exp; /* exponent, base 2 */
} diy_fp;

/** Get cached rounded diy_fp with pow(10, e) The input value must in range
    [POW10_SIG_TABLE_MIN_EXP, POW10_SIG_TABLE_MAX_EXP]. */
static_inline diy_fp diy_fp_get_cached_pow10(i32 exp10)
{
    diy_fp fp;
    u64 sig_ext;
    pow10_table_get_sig(exp10, &fp.sig, &sig_ext);
    pow10_table_get_exp(exp10, &fp.exp);
    fp.sig += (sig_ext >> 63);
    return fp;
}

/** Returns fp * fp2. */
static_inline diy_fp diy_fp_mul(diy_fp fp, diy_fp fp2)
{
    u64 hi, lo;
    u128_mul(fp.sig, fp2.sig, &hi, &lo);
    fp.sig = hi + (lo >> 63);
    fp.exp += fp2.exp + 64;
    return fp;
}

/** Convert diy_fp to IEEE-754 raw value. */
static_inline u64 diy_fp_to_ieee_raw(diy_fp fp)
{
    u64 sig = fp.sig;
    i32 exp = fp.exp;
    u32 lz_bits;
    if (unlikely(fp.sig == 0))
        return 0;

    lz_bits = u64_lz_bits(sig);
    sig <<= lz_bits;
    sig >>= F64_BITS - F64_SIG_FULL_BITS;
    exp -= lz_bits;
    exp += F64_BITS - F64_SIG_FULL_BITS;
    exp += F64_SIG_BITS;

    if (unlikely(exp >= F64_MAX_BIN_EXP))
    {
        /* overflow */
        return F64_RAW_INF;
    }
    else if (likely(exp >= F64_MIN_BIN_EXP - 1))
    {
        /* normal */
        exp += F64_EXP_BIAS;
        return ((u64)exp << F64_SIG_BITS) | (sig & F64_SIG_MASK);
    }
    else if (likely(exp >= F64_MIN_BIN_EXP - F64_SIG_FULL_BITS))
    {
        /* subnormal */
        return sig >> (F64_MIN_BIN_EXP - exp - 1);
    }
    else
    {
        /* underflow */
        return 0;
    }
}

/*==============================================================================
 * JSON Number Reader (IEEE-754)
 *============================================================================*/

/** Maximum exact pow10 exponent for double value. */
#define F64_POW10_EXP_MAX_EXACT 22

/** Cached pow10 table. */
static const f64 f64_pow10_table[] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12,
    1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};

#if 0
double yy_string_to_double(const char *str, char **endptr)
{

#define return_err()               \
    do                             \
    {                              \
        if (endptr)                \
            *endptr = (char *)str; \
        return 0.0;                \
    } while (false)

#define return_f64(_v)                        \
    do                                        \
    {                                         \
        if (endptr)                           \
            *endptr = (char *)cur;            \
        return sign ? -(f64)(_v) : (f64)(_v); \
    } while (false)

#define return_f64_raw(_v)                       \
    do                                           \
    {                                            \
        u64 ret = ((u64)sign << 63) | (u64)(_v); \
        if (endptr)                              \
            *endptr = (char *)cur;               \
        return f64_from_raw(ret);                \
    } while (false)

    u8 *sig_cut = NULL; /* significant part cutting position for long number */
    u8 *sig_end = NULL; /* significant part ending position */
    u8 *dot_pos = NULL; /* decimal point position */

    u64 sig = 0; /* significant part of the number */
    i32 exp = 0; /* exponent part of the number */

    bool exp_sign = false; /* temporary exponent sign from literal part */
    i64 exp_sig = 0;       /* temporary exponent number from significant part */
    i64 exp_lit = 0;       /* temporary exponent number from exponent literal part */
    u64 num;               /* temporary number for reading */
    u8 *tmp;               /* temporary cursor for reading */

    u8 *cur = (u8 *)str;
    u8 *hdr = cur;
    bool sign = (*hdr == '-');
    cur += sign;

    /* begin with a leading zero or non-digit */
    if (unlikely(!digi_is_nonzero(*cur)))
    { /* 0 or non-digit char */
        if (unlikely(*cur != '0'))
        { /* non-digit char */
            double ret;
            if (read_inf_or_nan(sign, cur, (u8 **)endptr, &ret))
                return ret;
            return_err(); /* no digit after minus sign */
        }
        /* begin with 0 */
        if (likely(!digi_is_digit_or_fp(*++cur)))
            return_f64(0);
        if (likely(*cur == '.'))
        {
            dot_pos = cur++;
            if (unlikely(!digi_is_digit(*cur)))
            {
                return_err(); /* no digit after decimal point */
            }
            while (unlikely(*cur == '0'))
                cur++;
            if (likely(digi_is_digit(*cur)))
            {
                /* first non-zero digit after decimal point */
                sig = (u64)(*cur - '0'); /* read first digit */
                cur--;
                goto digi_frac_1; /* continue read fraction part */
            }
        }
        if (unlikely(digi_is_digit(*cur)))
        {
            return_err(); /* number with leading zero is not allowed */
        }
        if (unlikely(digi_is_exp(*cur)))
        { /* 0 with any exponent is still 0 */
            cur += (usize)1 + digi_is_sign(cur[1]);
            if (unlikely(!digi_is_digit(*cur)))
            {
                return_err(); /* no digit after exponent sign */
            }
            while (digi_is_digit(*++cur))
                ;
        }
        return_f64_raw(0);
    }

    /* begin with non-zero digit */
    sig = (u64)(*cur - '0');

    /*
     Read integral part, same as the following code.

         for (int i = 1; i <= 18; i++) {
            num = cur[i] - '0';
            if (num <= 9) sig = num + sig * 10;
            else goto digi_sepr_i;
         }
     */
#define expr_intg(i)                                  \
    if (likely((num = (u64)(cur[i] - (u8)'0')) <= 9)) \
        sig = num + sig * 10;                         \
    else                                              \
    {                                                 \
        goto digi_sepr_##i;                           \
    }
    repeat_in_1_18(expr_intg)
#undef expr_intg

        cur += 19; /* skip continuous 19 digits */
    if (!digi_is_digit_or_fp(*cur))
    {
        return_f64(sig);
    }
    goto digi_intg_more; /* read more digits in integral part */

    /* process first non-digit character */
#define expr_sepr(i)                                 \
    digi_sepr_##i : if (likely(!digi_is_fp(cur[i]))) \
    {                                                \
        cur += i;                                    \
        return_f64(sig);                             \
    }                                                \
    dot_pos = cur + i;                               \
    if (likely(cur[i] == '.'))                       \
        goto digi_frac_##i;                          \
    cur += i;                                        \
    sig_end = cur;                                   \
    goto digi_exp_more;
    repeat_in_1_18(expr_sepr)
#undef expr_sepr

    /* read fraction part */
#define expr_frac(i)                                                      \
    digi_frac_##i : if (likely((num = (u64)(cur[i + 1] - (u8)'0')) <= 9)) \
                        sig = num + sig * 10;                             \
    else                                                                  \
    {                                                                     \
        goto digi_stop_##i;                                               \
    }
        repeat_in_1_18(expr_frac)
#undef expr_frac

            cur += 20; /* skip 19 digits and 1 decimal point */
    if (!digi_is_digit(*cur))
        goto digi_frac_end; /* fraction part end */
    goto digi_frac_more;    /* read more digits in fraction part */

    /* significant part end */
#define expr_stop(i)              \
    digi_stop_##i : cur += i + 1; \
    goto digi_frac_end;
    repeat_in_1_18(expr_stop)
#undef expr_stop

        /* read more digits in integral part */
        digi_intg_more : if (digi_is_digit(*cur))
    {
        if (!digi_is_digit_or_fp(cur[1]))
        {
            /* this number is an integer with 20 digits */
            num = (u64)(*cur - '0');
            if ((sig < (U64_MAX / 10)) ||
                (sig == (U64_MAX / 10) && num <= (U64_MAX % 10)))
            {
                sig = num + sig * 10;
                cur++;
                /* convert to double if overflow */
                return_f64(sig);
            }
        }
    }

    if (digi_is_exp(*cur))
    {
        dot_pos = cur;
        goto digi_exp_more;
    }

    if (*cur == '.')
    {
        dot_pos = cur++;
        if (!digi_is_digit(*cur))
        {
            return_err(); /* no digit after decimal point */
        }
    }

    /* read more digits in fraction part */
digi_frac_more:
    sig_cut = cur;        /* too large to fit in u64, excess digits need to be cut */
    sig += (*cur >= '5'); /* round */
    while (digi_is_digit(*++cur))
        ;
    if (!dot_pos)
    {
        dot_pos = cur;
        if (*cur == '.')
        {
            if (!digi_is_digit(*++cur))
            {
                return_err(); /* no digit after decimal point */
            }
            while (digi_is_digit(*cur))
                cur++;
        }
    }
    exp_sig = (i64)(dot_pos - sig_cut);
    exp_sig += (dot_pos < sig_cut);

    /* ignore trailing zeros */
    tmp = cur - 1;
    while (*tmp == '0' || *tmp == '.')
        tmp--;
    if (tmp < sig_cut)
    {
        sig_cut = NULL;
    }
    else
    {
        sig_end = cur;
    }

    if (digi_is_exp(*cur))
        goto digi_exp_more;
    goto digi_exp_finish;

    /* fraction part end */
digi_frac_end:
    if (unlikely(dot_pos + 1 == cur))
    {
        return_err(); /* no digit after decimal point */
    }
    sig_end = cur;
    exp_sig = -(i64)((u64)(cur - dot_pos) - 1);
    if (likely(!digi_is_exp(*cur)))
    {
        if (unlikely(exp_sig < F64_MIN_DEC_EXP - 19))
        {
            return_f64_raw(0); /* underflow */
        }
        exp = (i32)exp_sig;
        goto digi_finish;
    }
    else
    {
        goto digi_exp_more;
    }

    /* read exponent part */
digi_exp_more:
    exp_sign = (*++cur == '-');
    cur += digi_is_sign(*cur);
    if (unlikely(!digi_is_digit(*cur)))
    {
        return_err(); /* no digit after exponent sign */
    }
    while (*cur == '0')
        cur++;

    /* read exponent literal */
    tmp = cur;
    while (digi_is_digit(*cur))
    {
        exp_lit = (i64)((u8)(*cur++ - '0') + (u64)exp_lit * 10);
    }
    if (unlikely(cur - tmp >= U64_SAFE_DIG))
    {
        if (exp_sign)
        {
            return_f64_raw(0); /* underflow */
        }
        else
        {
            return_f64_raw(F64_RAW_INF); /* overflow */
        }
    }
    exp_sig += exp_sign ? -exp_lit : exp_lit;

    /* validate exponent value */
digi_exp_finish:
    if (unlikely(exp_sig < F64_MIN_DEC_EXP - 19))
    {
        return_f64_raw(0); /* underflow */
    }
    if (unlikely(exp_sig > F64_MAX_DEC_EXP))
    {
        return_f64_raw(F64_RAW_INF); /* overflow */
    }
    exp = (i32)exp_sig;

    /* all digit read finished */
digi_finish:

    /*
     Fast path 1:

     1. The floating-point number calculation should be accurate, see the
        comments of macro `YY_DOUBLE_MATH_CORRECT`.
     2. Correct rounding should be performed (fegetround() == FE_TONEAREST).
     3. The input of floating point number calculation does not lose precision,
        which means: 64 - leading_zero(input) - trailing_zero(input) < 53.

     We don't check all available inputs here, because that would make the code
     more complicated, and not friendly to branch predictor.
     */
#if YY_DOUBLE_MATH_CORRECT
    if (sig < ((u64)1 << 53) &&
        exp >= -F64_POW10_EXP_MAX_EXACT &&
        exp <= +F64_POW10_EXP_MAX_EXACT)
    {
        f64 dbl = (f64)sig;
        if (exp < 0)
        {
            dbl /= f64_pow10_table[-exp];
        }
        else
        {
            dbl *= f64_pow10_table[+exp];
        }
        return_f64(dbl);
    }
#endif

    /*
     Fast path 2:

     To keep it simple, we only accept normal number here,
     let the slow path to handle subnormal and infinity number.
     */
    if (likely(!sig_cut &&
               exp > -F64_MAX_DEC_EXP + 1 &&
               exp < +F64_MAX_DEC_EXP - 20))
    {
        /*
         The result value is exactly equal to (sig * 10^exp),
         the exponent part (10^exp) can be converted to (sig2 * 2^exp2).

         The sig2 can be an infinite length number, only the highest 128 bits
         is cached in the pow10_sig_table.

         Now we have these bits:
         sig1 (normalized 64bit)        : aaaaaaaa
         sig2 (higher 64bit)            : bbbbbbbb
         sig2_ext (lower 64bit)         : cccccccc
         sig2_cut (extra unknown bits)  : dddddddddddd....

         And the calculation process is:
         ----------------------------------------
                 aaaaaaaa *
                 bbbbbbbbccccccccdddddddddddd....
         ----------------------------------------
         abababababababab +
                 acacacacacacacac +
                         adadadadadadadadadad....
         ----------------------------------------
         [hi____][lo____] +
                 [hi2___][lo2___] +
                         [unknown___________....]
         ----------------------------------------

         The addition with carry may affect higher bits, but if there is a 0
         in higher bits, the bits higher than 0 will not be affected.

         `lo2` + `unknown` may get a carry bit and may affect `hi2`, the max
         value of `hi2` is 0xFFFFFFFFFFFFFFFE, so `hi2` will not overflow.

         `lo` + `hi2` may alse get a carry bit and may affect `hi`, but only
         the highest significant 53 bits of `hi` is needed. If there is a 0
         in the lower bits of `hi`, then all the following bits can be dropped.

         To convert the result to IEEE-754 double number, we need to perform
         correct rounding:
         1. if bit 54 is 0, round down,
         2. if bit 54 is 1 and any bit beyond bit 54 is 1, round up,
         3. if bit 54 is 1 and all bits beyond bit 54 are 0, round to even,
            as the extra bits is unknown, this case will not be handled here.
         */

        u64 raw;
        u64 sig1, sig2, sig2_ext, hi, lo, hi2, lo2, add, bits;
        i32 exp2;
        u32 lz;
        bool exact = false, carry, round_up;

        /* convert (10^exp) to (sig2 * 2^exp2) */
        pow10_table_get_sig(exp, &sig2, &sig2_ext);
        pow10_table_get_exp(exp, &exp2);

        /* normalize and multiply */
        lz = u64_lz_bits(sig);
        sig1 = sig << lz;
        exp2 -= lz;
        u128_mul(sig1, sig2, &hi, &lo);

        /*
         The `hi` is in range [0x4000000000000000, 0xFFFFFFFFFFFFFFFE],
         To get normalized value, `hi` should be shifted to the left by 0 or 1.

         The highest significant 53 bits is used by IEEE-754 double number,
         and the bit 54 is used to detect rounding direction.

         The lowest (64 - 54 - 1) bits is used to check whether it contains 0.
         */
        bits = hi & (((u64)1 << (64 - 54 - 1)) - 1);
        if (bits - 1 < (((u64)1 << (64 - 54 - 1)) - 2))
        {
            /*
             (bits != 0 && bits != 0x1FF) => (bits - 1 < 0x1FF - 1)
             The `bits` is not zero, so we don't need to check `round to even`
             case. The `bits` contains bit `0`, so we can drop the extra bits
             after `0`.
             */
            exact = true;
        }
        else
        {
            /*
             (bits == 0 || bits == 0x1FF)
             The `bits` is filled with all `0` or all `1`, so we need to check
             more lower bits with another 64-bit multiplication.
             */
            u128_mul(sig1, sig2_ext, &hi2, &lo2);

            add = lo + hi2;
            if (add + 1 > (u64)1)
            {
                /*
                 (add != 0 && add != U64_MAX) => (add + 1 > 1)
                 The `add` is not zero, so we don't need to check `round to
                 even` case. The `add` contains bit `0`, so we can drop the
                 extra bits after `0`. The `hi` cannot be U64_MAX, so it will
                 not overflow.
                 */
                carry = add < lo || add < hi2;
                hi += carry;
                exact = true;
            }
        }

        if (exact)
        {
            /* normalize */
            lz = hi < ((u64)1 << 63);
            hi <<= lz;
            exp2 -= lz;
            exp2 += 64;

            /* test the bit 54 and get rounding direction */
            round_up = (hi & ((u64)1 << (64 - 54))) > (u64)0;
            hi += (round_up ? ((u64)1 << (64 - 54)) : (u64)0);

            /* test overflow */
            if (hi < ((u64)1 << (64 - 54)))
            {
                hi = ((u64)1 << 63);
                exp2 += 1;
            }

            /* This is a normal number, convert it to IEEE-754 format. */
            hi >>= F64_BITS - F64_SIG_FULL_BITS;
            exp2 += F64_BITS - F64_SIG_FULL_BITS + F64_SIG_BITS;
            exp2 += F64_EXP_BIAS;
            raw = ((u64)exp2 << F64_SIG_BITS) | (hi & F64_SIG_MASK);
            return_f64_raw(raw);
        }
    }

    /*
     Slow path: read double number exactly with diyfp.
     1. Use cached diyfp to get an approximation value.
     2. Use bigcomp to check the approximation value if needed.

     This algorithm refers to google's double-conversion project:
     https://github.com/google/double-conversion
     */
    {
        const i32 ERR_ULP_LOG = 3;
        const i32 ERR_ULP = 1 << ERR_ULP_LOG;
        const i32 ERR_CACHED_POW = ERR_ULP / 2;
        const i32 ERR_MUL_FIXED = ERR_ULP / 2;
        const i32 DIY_SIG_BITS = 64;
        const i32 EXP_BIAS = F64_EXP_BIAS + F64_SIG_BITS;
        const i32 EXP_SUBNORMAL = -EXP_BIAS + 1;

        u64 fp_err;
        u32 bits;
        i32 order_of_magnitude;
        i32 effective_significand_size;
        i32 precision_digits_count;
        u64 precision_bits;
        u64 half_way;

        u64 raw;
        diy_fp fp, fp_upper;
        bigint big_full, big_comp;
        i32 cmp;

        fp.sig = sig;
        fp.exp = 0;
        fp_err = sig_cut ? (u64)(ERR_ULP / 2) : (u64)0;

        /* normalize */
        bits = u64_lz_bits(fp.sig);
        fp.sig <<= bits;
        fp.exp -= bits;
        fp_err <<= bits;

        /* multiply and add error */
        fp = diy_fp_mul(fp, diy_fp_get_cached_pow10(exp));
        fp_err += (u64)ERR_CACHED_POW + (fp_err != 0) + (u64)ERR_MUL_FIXED;

        /* normalize */
        bits = u64_lz_bits(fp.sig);
        fp.sig <<= bits;
        fp.exp -= bits;
        fp_err <<= bits;

        /* effective significand */
        order_of_magnitude = DIY_SIG_BITS + fp.exp;
        if (likely(order_of_magnitude >= EXP_SUBNORMAL + F64_SIG_FULL_BITS))
        {
            effective_significand_size = F64_SIG_FULL_BITS;
        }
        else if (order_of_magnitude <= EXP_SUBNORMAL)
        {
            effective_significand_size = 0;
        }
        else
        {
            effective_significand_size = order_of_magnitude - EXP_SUBNORMAL;
        }

        /* precision digits count */
        precision_digits_count = DIY_SIG_BITS - effective_significand_size;
        if (unlikely(precision_digits_count + ERR_ULP_LOG >= DIY_SIG_BITS))
        {
            i32 shr = (precision_digits_count + ERR_ULP_LOG) - DIY_SIG_BITS + 1;
            fp.sig >>= shr;
            fp.exp += shr;
            fp_err = (fp_err >> shr) + 1 + ERR_ULP;
            precision_digits_count -= shr;
        }

        /* half way */
        precision_bits = fp.sig & (((u64)1 << precision_digits_count) - 1);
        precision_bits *= ERR_ULP;
        half_way = (u64)1 << (precision_digits_count - 1);
        half_way *= ERR_ULP;

        /* rounding */
        fp.sig >>= precision_digits_count;
        fp.sig += (precision_bits >= half_way + fp_err);
        fp.exp += precision_digits_count;

        /* get IEEE double raw value */
        raw = diy_fp_to_ieee_raw(fp);
        if (unlikely(raw == F64_RAW_INF))
            return_f64_raw(raw);
        if (likely(precision_bits <= half_way - fp_err ||
                   precision_bits >= half_way + fp_err))
        {
            return_f64_raw(raw); /* number is accurate */
        }
        /* now the number is the correct value, or the next lower value */

        /* upper boundary */
        if (raw & F64_EXP_MASK)
        {
            fp_upper.sig = (raw & F64_SIG_MASK) + ((u64)1 << F64_SIG_BITS);
            fp_upper.exp = (i32)((raw & F64_EXP_MASK) >> F64_SIG_BITS);
        }
        else
        {
            fp_upper.sig = (raw & F64_SIG_MASK);
            fp_upper.exp = 1;
        }
        fp_upper.exp -= F64_EXP_BIAS + F64_SIG_BITS;
        fp_upper.sig <<= 1;
        fp_upper.exp -= 1;
        fp_upper.sig += 1; /* add half ulp */

        /* compare with bigint */
        bigint_set_buf(&big_full, sig, &exp, sig_cut, sig_end, dot_pos);
        bigint_set_u64(&big_comp, fp_upper.sig);
        if (exp >= 0)
        {
            bigint_mul_pow10(&big_full, +exp);
        }
        else
        {
            bigint_mul_pow10(&big_comp, -exp);
        }
        if (fp_upper.exp > 0)
        {
            bigint_mul_pow2(&big_comp, (u32) + fp_upper.exp);
        }
        else
        {
            bigint_mul_pow2(&big_full, (u32)-fp_upper.exp);
        }
        cmp = bigint_cmp(&big_full, &big_comp);
        if (likely(cmp != 0))
        {
            /* round down or round up */
            raw += (cmp > 0);
        }
        else
        {
            /* falls midway, round to even */
            raw += (raw & 1);
        }
        return_f64_raw(raw);
    }
    return 0.0;

#undef return_err
#undef return_f64
#undef return_f64_raw
}
#endif

/*==============================================================================
 * Integer Writer
 *
 * The maximum value of uint32_t is 4294967295 (10 digits),
 * these digits are named as 'aabbccddee' here.
 *
 * Although most compilers may convert the "division by constant value" into
 * "multiply and shift", manual conversion can still help some compilers
 * generate fewer and better instructions.
 *============================================================================*/

/** Digit table from 00 to 99. */
yy_align(2) static const unsigned char digit_table[200] = {
    '0', '0', '0', '1', '0', '2', '0', '3', '0', '4',
    '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
    '1', '0', '1', '1', '1', '2', '1', '3', '1', '4',
    '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
    '2', '0', '2', '1', '2', '2', '2', '3', '2', '4',
    '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
    '3', '0', '3', '1', '3', '2', '3', '3', '3', '4',
    '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
    '4', '0', '4', '1', '4', '2', '4', '3', '4', '4',
    '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
    '5', '0', '5', '1', '5', '2', '5', '3', '5', '4',
    '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
    '6', '0', '6', '1', '6', '2', '6', '3', '6', '4',
    '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
    '7', '0', '7', '1', '7', '2', '7', '3', '7', '4',
    '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
    '8', '0', '8', '1', '8', '2', '8', '3', '8', '4',
    '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
    '9', '0', '9', '1', '9', '2', '9', '3', '9', '4',
    '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'};
static const int digit10000[10000] = {0x30303030, 0x31303030, 0x32303030, 0x33303030, 0x34303030, 0x35303030, 0x36303030, 0x37303030, 0x38303030, 0x39303030, 0x30313030, 0x31313030, 0x32313030, 0x33313030, 0x34313030, 0x35313030, 0x36313030, 0x37313030, 0x38313030, 0x39313030, 0x30323030, 0x31323030, 0x32323030, 0x33323030, 0x34323030, 0x35323030, 0x36323030, 0x37323030, 0x38323030, 0x39323030, 0x30333030, 0x31333030, 0x32333030, 0x33333030, 0x34333030, 0x35333030, 0x36333030, 0x37333030, 0x38333030, 0x39333030, 0x30343030, 0x31343030, 0x32343030, 0x33343030, 0x34343030, 0x35343030, 0x36343030, 0x37343030, 0x38343030, 0x39343030, 0x30353030, 0x31353030, 0x32353030, 0x33353030, 0x34353030, 0x35353030, 0x36353030, 0x37353030, 0x38353030, 0x39353030, 0x30363030, 0x31363030, 0x32363030, 0x33363030, 0x34363030, 0x35363030, 0x36363030, 0x37363030, 0x38363030, 0x39363030, 0x30373030, 0x31373030, 0x32373030, 0x33373030, 0x34373030, 0x35373030, 0x36373030, 0x37373030, 0x38373030, 0x39373030, 0x30383030, 0x31383030, 0x32383030, 0x33383030, 0x34383030, 0x35383030, 0x36383030, 0x37383030, 0x38383030, 0x39383030, 0x30393030, 0x31393030, 0x32393030, 0x33393030, 0x34393030, 0x35393030, 0x36393030, 0x37393030, 0x38393030, 0x39393030, 0x30303130, 0x31303130, 0x32303130, 0x33303130, 0x34303130, 0x35303130, 0x36303130, 0x37303130, 0x38303130, 0x39303130, 0x30313130, 0x31313130, 0x32313130, 0x33313130, 0x34313130, 0x35313130, 0x36313130, 0x37313130, 0x38313130, 0x39313130, 0x30323130, 0x31323130, 0x32323130, 0x33323130, 0x34323130, 0x35323130, 0x36323130, 0x37323130, 0x38323130, 0x39323130, 0x30333130, 0x31333130, 0x32333130, 0x33333130, 0x34333130, 0x35333130, 0x36333130, 0x37333130, 0x38333130, 0x39333130, 0x30343130, 0x31343130, 0x32343130, 0x33343130, 0x34343130, 0x35343130, 0x36343130, 0x37343130, 0x38343130, 0x39343130, 0x30353130, 0x31353130, 0x32353130, 0x33353130, 0x34353130, 0x35353130, 0x36353130, 0x37353130, 0x38353130, 0x39353130, 0x30363130, 0x31363130, 0x32363130, 0x33363130, 0x34363130, 0x35363130, 0x36363130, 0x37363130, 0x38363130, 0x39363130, 0x30373130, 0x31373130, 0x32373130, 0x33373130, 0x34373130, 0x35373130, 0x36373130, 0x37373130, 0x38373130, 0x39373130, 0x30383130, 0x31383130, 0x32383130, 0x33383130, 0x34383130, 0x35383130, 0x36383130, 0x37383130, 0x38383130, 0x39383130, 0x30393130, 0x31393130, 0x32393130, 0x33393130, 0x34393130, 0x35393130, 0x36393130, 0x37393130, 0x38393130, 0x39393130, 0x30303230, 0x31303230, 0x32303230, 0x33303230, 0x34303230, 0x35303230, 0x36303230, 0x37303230, 0x38303230, 0x39303230, 0x30313230, 0x31313230, 0x32313230, 0x33313230, 0x34313230, 0x35313230, 0x36313230, 0x37313230, 0x38313230, 0x39313230, 0x30323230, 0x31323230, 0x32323230, 0x33323230, 0x34323230, 0x35323230, 0x36323230, 0x37323230, 0x38323230, 0x39323230, 0x30333230, 0x31333230, 0x32333230, 0x33333230, 0x34333230, 0x35333230, 0x36333230, 0x37333230, 0x38333230, 0x39333230, 0x30343230, 0x31343230, 0x32343230, 0x33343230, 0x34343230, 0x35343230, 0x36343230, 0x37343230, 0x38343230, 0x39343230, 0x30353230, 0x31353230, 0x32353230, 0x33353230, 0x34353230, 0x35353230, 0x36353230, 0x37353230, 0x38353230, 0x39353230, 0x30363230, 0x31363230, 0x32363230, 0x33363230, 0x34363230, 0x35363230, 0x36363230, 0x37363230, 0x38363230, 0x39363230, 0x30373230, 0x31373230, 0x32373230, 0x33373230, 0x34373230, 0x35373230, 0x36373230, 0x37373230, 0x38373230, 0x39373230, 0x30383230, 0x31383230, 0x32383230, 0x33383230, 0x34383230, 0x35383230, 0x36383230, 0x37383230, 0x38383230, 0x39383230, 0x30393230, 0x31393230, 0x32393230, 0x33393230, 0x34393230, 0x35393230, 0x36393230, 0x37393230, 0x38393230, 0x39393230, 0x30303330, 0x31303330, 0x32303330, 0x33303330, 0x34303330, 0x35303330, 0x36303330, 0x37303330, 0x38303330, 0x39303330, 0x30313330, 0x31313330, 0x32313330, 0x33313330, 0x34313330, 0x35313330, 0x36313330, 0x37313330, 0x38313330, 0x39313330, 0x30323330, 0x31323330, 0x32323330, 0x33323330, 0x34323330, 0x35323330, 0x36323330, 0x37323330, 0x38323330, 0x39323330, 0x30333330, 0x31333330, 0x32333330, 0x33333330, 0x34333330, 0x35333330, 0x36333330, 0x37333330, 0x38333330, 0x39333330, 0x30343330, 0x31343330, 0x32343330, 0x33343330, 0x34343330, 0x35343330, 0x36343330, 0x37343330, 0x38343330, 0x39343330, 0x30353330, 0x31353330, 0x32353330, 0x33353330, 0x34353330, 0x35353330, 0x36353330, 0x37353330, 0x38353330, 0x39353330, 0x30363330, 0x31363330, 0x32363330, 0x33363330, 0x34363330, 0x35363330, 0x36363330, 0x37363330, 0x38363330, 0x39363330, 0x30373330, 0x31373330, 0x32373330, 0x33373330, 0x34373330, 0x35373330, 0x36373330, 0x37373330, 0x38373330, 0x39373330, 0x30383330, 0x31383330, 0x32383330, 0x33383330, 0x34383330, 0x35383330, 0x36383330, 0x37383330, 0x38383330, 0x39383330, 0x30393330, 0x31393330, 0x32393330, 0x33393330, 0x34393330, 0x35393330, 0x36393330, 0x37393330, 0x38393330, 0x39393330, 0x30303430, 0x31303430, 0x32303430, 0x33303430, 0x34303430, 0x35303430, 0x36303430, 0x37303430, 0x38303430, 0x39303430, 0x30313430, 0x31313430, 0x32313430, 0x33313430, 0x34313430, 0x35313430, 0x36313430, 0x37313430, 0x38313430, 0x39313430, 0x30323430, 0x31323430, 0x32323430, 0x33323430, 0x34323430, 0x35323430, 0x36323430, 0x37323430, 0x38323430, 0x39323430, 0x30333430, 0x31333430, 0x32333430, 0x33333430, 0x34333430, 0x35333430, 0x36333430, 0x37333430, 0x38333430, 0x39333430, 0x30343430, 0x31343430, 0x32343430, 0x33343430, 0x34343430, 0x35343430, 0x36343430, 0x37343430, 0x38343430, 0x39343430, 0x30353430, 0x31353430, 0x32353430, 0x33353430, 0x34353430, 0x35353430, 0x36353430, 0x37353430, 0x38353430, 0x39353430, 0x30363430, 0x31363430, 0x32363430, 0x33363430, 0x34363430, 0x35363430, 0x36363430, 0x37363430, 0x38363430, 0x39363430, 0x30373430, 0x31373430, 0x32373430, 0x33373430, 0x34373430, 0x35373430, 0x36373430, 0x37373430, 0x38373430, 0x39373430, 0x30383430, 0x31383430, 0x32383430, 0x33383430, 0x34383430, 0x35383430, 0x36383430, 0x37383430, 0x38383430, 0x39383430, 0x30393430, 0x31393430, 0x32393430, 0x33393430, 0x34393430, 0x35393430, 0x36393430, 0x37393430, 0x38393430, 0x39393430, 0x30303530, 0x31303530, 0x32303530, 0x33303530, 0x34303530, 0x35303530, 0x36303530, 0x37303530, 0x38303530, 0x39303530, 0x30313530, 0x31313530, 0x32313530, 0x33313530, 0x34313530, 0x35313530, 0x36313530, 0x37313530, 0x38313530, 0x39313530, 0x30323530, 0x31323530, 0x32323530, 0x33323530, 0x34323530, 0x35323530, 0x36323530, 0x37323530, 0x38323530, 0x39323530, 0x30333530, 0x31333530, 0x32333530, 0x33333530, 0x34333530, 0x35333530, 0x36333530, 0x37333530, 0x38333530, 0x39333530, 0x30343530, 0x31343530, 0x32343530, 0x33343530, 0x34343530, 0x35343530, 0x36343530, 0x37343530, 0x38343530, 0x39343530, 0x30353530, 0x31353530, 0x32353530, 0x33353530, 0x34353530, 0x35353530, 0x36353530, 0x37353530, 0x38353530, 0x39353530, 0x30363530, 0x31363530, 0x32363530, 0x33363530, 0x34363530, 0x35363530, 0x36363530, 0x37363530, 0x38363530, 0x39363530, 0x30373530, 0x31373530, 0x32373530, 0x33373530, 0x34373530, 0x35373530, 0x36373530, 0x37373530, 0x38373530, 0x39373530, 0x30383530, 0x31383530, 0x32383530, 0x33383530, 0x34383530, 0x35383530, 0x36383530, 0x37383530, 0x38383530, 0x39383530, 0x30393530, 0x31393530, 0x32393530, 0x33393530, 0x34393530, 0x35393530, 0x36393530, 0x37393530, 0x38393530, 0x39393530, 0x30303630, 0x31303630, 0x32303630, 0x33303630, 0x34303630, 0x35303630, 0x36303630, 0x37303630, 0x38303630, 0x39303630, 0x30313630, 0x31313630, 0x32313630, 0x33313630, 0x34313630, 0x35313630, 0x36313630, 0x37313630, 0x38313630, 0x39313630, 0x30323630, 0x31323630, 0x32323630, 0x33323630, 0x34323630, 0x35323630, 0x36323630, 0x37323630, 0x38323630, 0x39323630, 0x30333630, 0x31333630, 0x32333630, 0x33333630, 0x34333630, 0x35333630, 0x36333630, 0x37333630, 0x38333630, 0x39333630, 0x30343630, 0x31343630, 0x32343630, 0x33343630, 0x34343630, 0x35343630, 0x36343630, 0x37343630, 0x38343630, 0x39343630, 0x30353630, 0x31353630, 0x32353630, 0x33353630, 0x34353630, 0x35353630, 0x36353630, 0x37353630, 0x38353630, 0x39353630, 0x30363630, 0x31363630, 0x32363630, 0x33363630, 0x34363630, 0x35363630, 0x36363630, 0x37363630, 0x38363630, 0x39363630, 0x30373630, 0x31373630, 0x32373630, 0x33373630, 0x34373630, 0x35373630, 0x36373630, 0x37373630, 0x38373630, 0x39373630, 0x30383630, 0x31383630, 0x32383630, 0x33383630, 0x34383630, 0x35383630, 0x36383630, 0x37383630, 0x38383630, 0x39383630, 0x30393630, 0x31393630, 0x32393630, 0x33393630, 0x34393630, 0x35393630, 0x36393630, 0x37393630, 0x38393630, 0x39393630, 0x30303730, 0x31303730, 0x32303730, 0x33303730, 0x34303730, 0x35303730, 0x36303730, 0x37303730, 0x38303730, 0x39303730, 0x30313730, 0x31313730, 0x32313730, 0x33313730, 0x34313730, 0x35313730, 0x36313730, 0x37313730, 0x38313730, 0x39313730, 0x30323730, 0x31323730, 0x32323730, 0x33323730, 0x34323730, 0x35323730, 0x36323730, 0x37323730, 0x38323730, 0x39323730, 0x30333730, 0x31333730, 0x32333730, 0x33333730, 0x34333730, 0x35333730, 0x36333730, 0x37333730, 0x38333730, 0x39333730, 0x30343730, 0x31343730, 0x32343730, 0x33343730, 0x34343730, 0x35343730, 0x36343730, 0x37343730, 0x38343730, 0x39343730, 0x30353730, 0x31353730, 0x32353730, 0x33353730, 0x34353730, 0x35353730, 0x36353730, 0x37353730, 0x38353730, 0x39353730, 0x30363730, 0x31363730, 0x32363730, 0x33363730, 0x34363730, 0x35363730, 0x36363730, 0x37363730, 0x38363730, 0x39363730, 0x30373730, 0x31373730, 0x32373730, 0x33373730, 0x34373730, 0x35373730, 0x36373730, 0x37373730, 0x38373730, 0x39373730, 0x30383730, 0x31383730, 0x32383730, 0x33383730, 0x34383730, 0x35383730, 0x36383730, 0x37383730, 0x38383730, 0x39383730, 0x30393730, 0x31393730, 0x32393730, 0x33393730, 0x34393730, 0x35393730, 0x36393730, 0x37393730, 0x38393730, 0x39393730, 0x30303830, 0x31303830, 0x32303830, 0x33303830, 0x34303830, 0x35303830, 0x36303830, 0x37303830, 0x38303830, 0x39303830, 0x30313830, 0x31313830, 0x32313830, 0x33313830, 0x34313830, 0x35313830, 0x36313830, 0x37313830, 0x38313830, 0x39313830, 0x30323830, 0x31323830, 0x32323830, 0x33323830, 0x34323830, 0x35323830, 0x36323830, 0x37323830, 0x38323830, 0x39323830, 0x30333830, 0x31333830, 0x32333830, 0x33333830, 0x34333830, 0x35333830, 0x36333830, 0x37333830, 0x38333830, 0x39333830, 0x30343830, 0x31343830, 0x32343830, 0x33343830, 0x34343830, 0x35343830, 0x36343830, 0x37343830, 0x38343830, 0x39343830, 0x30353830, 0x31353830, 0x32353830, 0x33353830, 0x34353830, 0x35353830, 0x36353830, 0x37353830, 0x38353830, 0x39353830, 0x30363830, 0x31363830, 0x32363830, 0x33363830, 0x34363830, 0x35363830, 0x36363830, 0x37363830, 0x38363830, 0x39363830, 0x30373830, 0x31373830, 0x32373830, 0x33373830, 0x34373830, 0x35373830, 0x36373830, 0x37373830, 0x38373830, 0x39373830, 0x30383830, 0x31383830, 0x32383830, 0x33383830, 0x34383830, 0x35383830, 0x36383830, 0x37383830, 0x38383830, 0x39383830, 0x30393830, 0x31393830, 0x32393830, 0x33393830, 0x34393830, 0x35393830, 0x36393830, 0x37393830, 0x38393830, 0x39393830, 0x30303930, 0x31303930, 0x32303930, 0x33303930, 0x34303930, 0x35303930, 0x36303930, 0x37303930, 0x38303930, 0x39303930, 0x30313930, 0x31313930, 0x32313930, 0x33313930, 0x34313930, 0x35313930, 0x36313930, 0x37313930, 0x38313930, 0x39313930, 0x30323930, 0x31323930, 0x32323930, 0x33323930, 0x34323930, 0x35323930, 0x36323930, 0x37323930, 0x38323930, 0x39323930, 0x30333930, 0x31333930, 0x32333930, 0x33333930, 0x34333930, 0x35333930, 0x36333930, 0x37333930, 0x38333930, 0x39333930, 0x30343930, 0x31343930, 0x32343930, 0x33343930, 0x34343930, 0x35343930, 0x36343930, 0x37343930, 0x38343930, 0x39343930, 0x30353930, 0x31353930, 0x32353930, 0x33353930, 0x34353930, 0x35353930, 0x36353930, 0x37353930, 0x38353930, 0x39353930, 0x30363930, 0x31363930, 0x32363930, 0x33363930, 0x34363930, 0x35363930, 0x36363930, 0x37363930, 0x38363930, 0x39363930, 0x30373930, 0x31373930, 0x32373930, 0x33373930, 0x34373930, 0x35373930, 0x36373930, 0x37373930, 0x38373930, 0x39373930, 0x30383930, 0x31383930, 0x32383930, 0x33383930, 0x34383930, 0x35383930, 0x36383930, 0x37383930, 0x38383930, 0x39383930, 0x30393930, 0x31393930, 0x32393930, 0x33393930, 0x34393930, 0x35393930, 0x36393930, 0x37393930, 0x38393930, 0x39393930, 0x30303031, 0x31303031, 0x32303031, 0x33303031, 0x34303031, 0x35303031, 0x36303031, 0x37303031, 0x38303031, 0x39303031, 0x30313031, 0x31313031, 0x32313031, 0x33313031, 0x34313031, 0x35313031, 0x36313031, 0x37313031, 0x38313031, 0x39313031, 0x30323031, 0x31323031, 0x32323031, 0x33323031, 0x34323031, 0x35323031, 0x36323031, 0x37323031, 0x38323031, 0x39323031, 0x30333031, 0x31333031, 0x32333031, 0x33333031, 0x34333031, 0x35333031, 0x36333031, 0x37333031, 0x38333031, 0x39333031, 0x30343031, 0x31343031, 0x32343031, 0x33343031, 0x34343031, 0x35343031, 0x36343031, 0x37343031, 0x38343031, 0x39343031, 0x30353031, 0x31353031, 0x32353031, 0x33353031, 0x34353031, 0x35353031, 0x36353031, 0x37353031, 0x38353031, 0x39353031, 0x30363031, 0x31363031, 0x32363031, 0x33363031, 0x34363031, 0x35363031, 0x36363031, 0x37363031, 0x38363031, 0x39363031, 0x30373031, 0x31373031, 0x32373031, 0x33373031, 0x34373031, 0x35373031, 0x36373031, 0x37373031, 0x38373031, 0x39373031, 0x30383031, 0x31383031, 0x32383031, 0x33383031, 0x34383031, 0x35383031, 0x36383031, 0x37383031, 0x38383031, 0x39383031, 0x30393031, 0x31393031, 0x32393031, 0x33393031, 0x34393031, 0x35393031, 0x36393031, 0x37393031, 0x38393031, 0x39393031, 0x30303131, 0x31303131, 0x32303131, 0x33303131, 0x34303131, 0x35303131, 0x36303131, 0x37303131, 0x38303131, 0x39303131, 0x30313131, 0x31313131, 0x32313131, 0x33313131, 0x34313131, 0x35313131, 0x36313131, 0x37313131, 0x38313131, 0x39313131, 0x30323131, 0x31323131, 0x32323131, 0x33323131, 0x34323131, 0x35323131, 0x36323131, 0x37323131, 0x38323131, 0x39323131, 0x30333131, 0x31333131, 0x32333131, 0x33333131, 0x34333131, 0x35333131, 0x36333131, 0x37333131, 0x38333131, 0x39333131, 0x30343131, 0x31343131, 0x32343131, 0x33343131, 0x34343131, 0x35343131, 0x36343131, 0x37343131, 0x38343131, 0x39343131, 0x30353131, 0x31353131, 0x32353131, 0x33353131, 0x34353131, 0x35353131, 0x36353131, 0x37353131, 0x38353131, 0x39353131, 0x30363131, 0x31363131, 0x32363131, 0x33363131, 0x34363131, 0x35363131, 0x36363131, 0x37363131, 0x38363131, 0x39363131, 0x30373131, 0x31373131, 0x32373131, 0x33373131, 0x34373131, 0x35373131, 0x36373131, 0x37373131, 0x38373131, 0x39373131, 0x30383131, 0x31383131, 0x32383131, 0x33383131, 0x34383131, 0x35383131, 0x36383131, 0x37383131, 0x38383131, 0x39383131, 0x30393131, 0x31393131, 0x32393131, 0x33393131, 0x34393131, 0x35393131, 0x36393131, 0x37393131, 0x38393131, 0x39393131, 0x30303231, 0x31303231, 0x32303231, 0x33303231, 0x34303231, 0x35303231, 0x36303231, 0x37303231, 0x38303231, 0x39303231, 0x30313231, 0x31313231, 0x32313231, 0x33313231, 0x34313231, 0x35313231, 0x36313231, 0x37313231, 0x38313231, 0x39313231, 0x30323231, 0x31323231, 0x32323231, 0x33323231, 0x34323231, 0x35323231, 0x36323231, 0x37323231, 0x38323231, 0x39323231, 0x30333231, 0x31333231, 0x32333231, 0x33333231, 0x34333231, 0x35333231, 0x36333231, 0x37333231, 0x38333231, 0x39333231, 0x30343231, 0x31343231, 0x32343231, 0x33343231, 0x34343231, 0x35343231, 0x36343231, 0x37343231, 0x38343231, 0x39343231, 0x30353231, 0x31353231, 0x32353231, 0x33353231, 0x34353231, 0x35353231, 0x36353231, 0x37353231, 0x38353231, 0x39353231, 0x30363231, 0x31363231, 0x32363231, 0x33363231, 0x34363231, 0x35363231, 0x36363231, 0x37363231, 0x38363231, 0x39363231, 0x30373231, 0x31373231, 0x32373231, 0x33373231, 0x34373231, 0x35373231, 0x36373231, 0x37373231, 0x38373231, 0x39373231, 0x30383231, 0x31383231, 0x32383231, 0x33383231, 0x34383231, 0x35383231, 0x36383231, 0x37383231, 0x38383231, 0x39383231, 0x30393231, 0x31393231, 0x32393231, 0x33393231, 0x34393231, 0x35393231, 0x36393231, 0x37393231, 0x38393231, 0x39393231, 0x30303331, 0x31303331, 0x32303331, 0x33303331, 0x34303331, 0x35303331, 0x36303331, 0x37303331, 0x38303331, 0x39303331, 0x30313331, 0x31313331, 0x32313331, 0x33313331, 0x34313331, 0x35313331, 0x36313331, 0x37313331, 0x38313331, 0x39313331, 0x30323331, 0x31323331, 0x32323331, 0x33323331, 0x34323331, 0x35323331, 0x36323331, 0x37323331, 0x38323331, 0x39323331, 0x30333331, 0x31333331, 0x32333331, 0x33333331, 0x34333331, 0x35333331, 0x36333331, 0x37333331, 0x38333331, 0x39333331, 0x30343331, 0x31343331, 0x32343331, 0x33343331, 0x34343331, 0x35343331, 0x36343331, 0x37343331, 0x38343331, 0x39343331, 0x30353331, 0x31353331, 0x32353331, 0x33353331, 0x34353331, 0x35353331, 0x36353331, 0x37353331, 0x38353331, 0x39353331, 0x30363331, 0x31363331, 0x32363331, 0x33363331, 0x34363331, 0x35363331, 0x36363331, 0x37363331, 0x38363331, 0x39363331, 0x30373331, 0x31373331, 0x32373331, 0x33373331, 0x34373331, 0x35373331, 0x36373331, 0x37373331, 0x38373331, 0x39373331, 0x30383331, 0x31383331, 0x32383331, 0x33383331, 0x34383331, 0x35383331, 0x36383331, 0x37383331, 0x38383331, 0x39383331, 0x30393331, 0x31393331, 0x32393331, 0x33393331, 0x34393331, 0x35393331, 0x36393331, 0x37393331, 0x38393331, 0x39393331, 0x30303431, 0x31303431, 0x32303431, 0x33303431, 0x34303431, 0x35303431, 0x36303431, 0x37303431, 0x38303431, 0x39303431, 0x30313431, 0x31313431, 0x32313431, 0x33313431, 0x34313431, 0x35313431, 0x36313431, 0x37313431, 0x38313431, 0x39313431, 0x30323431, 0x31323431, 0x32323431, 0x33323431, 0x34323431, 0x35323431, 0x36323431, 0x37323431, 0x38323431, 0x39323431, 0x30333431, 0x31333431, 0x32333431, 0x33333431, 0x34333431, 0x35333431, 0x36333431, 0x37333431, 0x38333431, 0x39333431, 0x30343431, 0x31343431, 0x32343431, 0x33343431, 0x34343431, 0x35343431, 0x36343431, 0x37343431, 0x38343431, 0x39343431, 0x30353431, 0x31353431, 0x32353431, 0x33353431, 0x34353431, 0x35353431, 0x36353431, 0x37353431, 0x38353431, 0x39353431, 0x30363431, 0x31363431, 0x32363431, 0x33363431, 0x34363431, 0x35363431, 0x36363431, 0x37363431, 0x38363431, 0x39363431, 0x30373431, 0x31373431, 0x32373431, 0x33373431, 0x34373431, 0x35373431, 0x36373431, 0x37373431, 0x38373431, 0x39373431, 0x30383431, 0x31383431, 0x32383431, 0x33383431, 0x34383431, 0x35383431, 0x36383431, 0x37383431, 0x38383431, 0x39383431, 0x30393431, 0x31393431, 0x32393431, 0x33393431, 0x34393431, 0x35393431, 0x36393431, 0x37393431, 0x38393431, 0x39393431, 0x30303531, 0x31303531, 0x32303531, 0x33303531, 0x34303531, 0x35303531, 0x36303531, 0x37303531, 0x38303531, 0x39303531, 0x30313531, 0x31313531, 0x32313531, 0x33313531, 0x34313531, 0x35313531, 0x36313531, 0x37313531, 0x38313531, 0x39313531, 0x30323531, 0x31323531, 0x32323531, 0x33323531, 0x34323531, 0x35323531, 0x36323531, 0x37323531, 0x38323531, 0x39323531, 0x30333531, 0x31333531, 0x32333531, 0x33333531, 0x34333531, 0x35333531, 0x36333531, 0x37333531, 0x38333531, 0x39333531, 0x30343531, 0x31343531, 0x32343531, 0x33343531, 0x34343531, 0x35343531, 0x36343531, 0x37343531, 0x38343531, 0x39343531, 0x30353531, 0x31353531, 0x32353531, 0x33353531, 0x34353531, 0x35353531, 0x36353531, 0x37353531, 0x38353531, 0x39353531, 0x30363531, 0x31363531, 0x32363531, 0x33363531, 0x34363531, 0x35363531, 0x36363531, 0x37363531, 0x38363531, 0x39363531, 0x30373531, 0x31373531, 0x32373531, 0x33373531, 0x34373531, 0x35373531, 0x36373531, 0x37373531, 0x38373531, 0x39373531, 0x30383531, 0x31383531, 0x32383531, 0x33383531, 0x34383531, 0x35383531, 0x36383531, 0x37383531, 0x38383531, 0x39383531, 0x30393531, 0x31393531, 0x32393531, 0x33393531, 0x34393531, 0x35393531, 0x36393531, 0x37393531, 0x38393531, 0x39393531, 0x30303631, 0x31303631, 0x32303631, 0x33303631, 0x34303631, 0x35303631, 0x36303631, 0x37303631, 0x38303631, 0x39303631, 0x30313631, 0x31313631, 0x32313631, 0x33313631, 0x34313631, 0x35313631, 0x36313631, 0x37313631, 0x38313631, 0x39313631, 0x30323631, 0x31323631, 0x32323631, 0x33323631, 0x34323631, 0x35323631, 0x36323631, 0x37323631, 0x38323631, 0x39323631, 0x30333631, 0x31333631, 0x32333631, 0x33333631, 0x34333631, 0x35333631, 0x36333631, 0x37333631, 0x38333631, 0x39333631, 0x30343631, 0x31343631, 0x32343631, 0x33343631, 0x34343631, 0x35343631, 0x36343631, 0x37343631, 0x38343631, 0x39343631, 0x30353631, 0x31353631, 0x32353631, 0x33353631, 0x34353631, 0x35353631, 0x36353631, 0x37353631, 0x38353631, 0x39353631, 0x30363631, 0x31363631, 0x32363631, 0x33363631, 0x34363631, 0x35363631, 0x36363631, 0x37363631, 0x38363631, 0x39363631, 0x30373631, 0x31373631, 0x32373631, 0x33373631, 0x34373631, 0x35373631, 0x36373631, 0x37373631, 0x38373631, 0x39373631, 0x30383631, 0x31383631, 0x32383631, 0x33383631, 0x34383631, 0x35383631, 0x36383631, 0x37383631, 0x38383631, 0x39383631, 0x30393631, 0x31393631, 0x32393631, 0x33393631, 0x34393631, 0x35393631, 0x36393631, 0x37393631, 0x38393631, 0x39393631, 0x30303731, 0x31303731, 0x32303731, 0x33303731, 0x34303731, 0x35303731, 0x36303731, 0x37303731, 0x38303731, 0x39303731, 0x30313731, 0x31313731, 0x32313731, 0x33313731, 0x34313731, 0x35313731, 0x36313731, 0x37313731, 0x38313731, 0x39313731, 0x30323731, 0x31323731, 0x32323731, 0x33323731, 0x34323731, 0x35323731, 0x36323731, 0x37323731, 0x38323731, 0x39323731, 0x30333731, 0x31333731, 0x32333731, 0x33333731, 0x34333731, 0x35333731, 0x36333731, 0x37333731, 0x38333731, 0x39333731, 0x30343731, 0x31343731, 0x32343731, 0x33343731, 0x34343731, 0x35343731, 0x36343731, 0x37343731, 0x38343731, 0x39343731, 0x30353731, 0x31353731, 0x32353731, 0x33353731, 0x34353731, 0x35353731, 0x36353731, 0x37353731, 0x38353731, 0x39353731, 0x30363731, 0x31363731, 0x32363731, 0x33363731, 0x34363731, 0x35363731, 0x36363731, 0x37363731, 0x38363731, 0x39363731, 0x30373731, 0x31373731, 0x32373731, 0x33373731, 0x34373731, 0x35373731, 0x36373731, 0x37373731, 0x38373731, 0x39373731, 0x30383731, 0x31383731, 0x32383731, 0x33383731, 0x34383731, 0x35383731, 0x36383731, 0x37383731, 0x38383731, 0x39383731, 0x30393731, 0x31393731, 0x32393731, 0x33393731, 0x34393731, 0x35393731, 0x36393731, 0x37393731, 0x38393731, 0x39393731, 0x30303831, 0x31303831, 0x32303831, 0x33303831, 0x34303831, 0x35303831, 0x36303831, 0x37303831, 0x38303831, 0x39303831, 0x30313831, 0x31313831, 0x32313831, 0x33313831, 0x34313831, 0x35313831, 0x36313831, 0x37313831, 0x38313831, 0x39313831, 0x30323831, 0x31323831, 0x32323831, 0x33323831, 0x34323831, 0x35323831, 0x36323831, 0x37323831, 0x38323831, 0x39323831, 0x30333831, 0x31333831, 0x32333831, 0x33333831, 0x34333831, 0x35333831, 0x36333831, 0x37333831, 0x38333831, 0x39333831, 0x30343831, 0x31343831, 0x32343831, 0x33343831, 0x34343831, 0x35343831, 0x36343831, 0x37343831, 0x38343831, 0x39343831, 0x30353831, 0x31353831, 0x32353831, 0x33353831, 0x34353831, 0x35353831, 0x36353831, 0x37353831, 0x38353831, 0x39353831, 0x30363831, 0x31363831, 0x32363831, 0x33363831, 0x34363831, 0x35363831, 0x36363831, 0x37363831, 0x38363831, 0x39363831, 0x30373831, 0x31373831, 0x32373831, 0x33373831, 0x34373831, 0x35373831, 0x36373831, 0x37373831, 0x38373831, 0x39373831, 0x30383831, 0x31383831, 0x32383831, 0x33383831, 0x34383831, 0x35383831, 0x36383831, 0x37383831, 0x38383831, 0x39383831, 0x30393831, 0x31393831, 0x32393831, 0x33393831, 0x34393831, 0x35393831, 0x36393831, 0x37393831, 0x38393831, 0x39393831, 0x30303931, 0x31303931, 0x32303931, 0x33303931, 0x34303931, 0x35303931, 0x36303931, 0x37303931, 0x38303931, 0x39303931, 0x30313931, 0x31313931, 0x32313931, 0x33313931, 0x34313931, 0x35313931, 0x36313931, 0x37313931, 0x38313931, 0x39313931, 0x30323931, 0x31323931, 0x32323931, 0x33323931, 0x34323931, 0x35323931, 0x36323931, 0x37323931, 0x38323931, 0x39323931, 0x30333931, 0x31333931, 0x32333931, 0x33333931, 0x34333931, 0x35333931, 0x36333931, 0x37333931, 0x38333931, 0x39333931, 0x30343931, 0x31343931, 0x32343931, 0x33343931, 0x34343931, 0x35343931, 0x36343931, 0x37343931, 0x38343931, 0x39343931, 0x30353931, 0x31353931, 0x32353931, 0x33353931, 0x34353931, 0x35353931, 0x36353931, 0x37353931, 0x38353931, 0x39353931, 0x30363931, 0x31363931, 0x32363931, 0x33363931, 0x34363931, 0x35363931, 0x36363931, 0x37363931, 0x38363931, 0x39363931, 0x30373931, 0x31373931, 0x32373931, 0x33373931, 0x34373931, 0x35373931, 0x36373931, 0x37373931, 0x38373931, 0x39373931, 0x30383931, 0x31383931, 0x32383931, 0x33383931, 0x34383931, 0x35383931, 0x36383931, 0x37383931, 0x38383931, 0x39383931, 0x30393931, 0x31393931, 0x32393931, 0x33393931, 0x34393931, 0x35393931, 0x36393931, 0x37393931, 0x38393931, 0x39393931, 0x30303032, 0x31303032, 0x32303032, 0x33303032, 0x34303032, 0x35303032, 0x36303032, 0x37303032, 0x38303032, 0x39303032, 0x30313032, 0x31313032, 0x32313032, 0x33313032, 0x34313032, 0x35313032, 0x36313032, 0x37313032, 0x38313032, 0x39313032, 0x30323032, 0x31323032, 0x32323032, 0x33323032, 0x34323032, 0x35323032, 0x36323032, 0x37323032, 0x38323032, 0x39323032, 0x30333032, 0x31333032, 0x32333032, 0x33333032, 0x34333032, 0x35333032, 0x36333032, 0x37333032, 0x38333032, 0x39333032, 0x30343032, 0x31343032, 0x32343032, 0x33343032, 0x34343032, 0x35343032, 0x36343032, 0x37343032, 0x38343032, 0x39343032, 0x30353032, 0x31353032, 0x32353032, 0x33353032, 0x34353032, 0x35353032, 0x36353032, 0x37353032, 0x38353032, 0x39353032, 0x30363032, 0x31363032, 0x32363032, 0x33363032, 0x34363032, 0x35363032, 0x36363032, 0x37363032, 0x38363032, 0x39363032, 0x30373032, 0x31373032, 0x32373032, 0x33373032, 0x34373032, 0x35373032, 0x36373032, 0x37373032, 0x38373032, 0x39373032, 0x30383032, 0x31383032, 0x32383032, 0x33383032, 0x34383032, 0x35383032, 0x36383032, 0x37383032, 0x38383032, 0x39383032, 0x30393032, 0x31393032, 0x32393032, 0x33393032, 0x34393032, 0x35393032, 0x36393032, 0x37393032, 0x38393032, 0x39393032, 0x30303132, 0x31303132, 0x32303132, 0x33303132, 0x34303132, 0x35303132, 0x36303132, 0x37303132, 0x38303132, 0x39303132, 0x30313132, 0x31313132, 0x32313132, 0x33313132, 0x34313132, 0x35313132, 0x36313132, 0x37313132, 0x38313132, 0x39313132, 0x30323132, 0x31323132, 0x32323132, 0x33323132, 0x34323132, 0x35323132, 0x36323132, 0x37323132, 0x38323132, 0x39323132, 0x30333132, 0x31333132, 0x32333132, 0x33333132, 0x34333132, 0x35333132, 0x36333132, 0x37333132, 0x38333132, 0x39333132, 0x30343132, 0x31343132, 0x32343132, 0x33343132, 0x34343132, 0x35343132, 0x36343132, 0x37343132, 0x38343132, 0x39343132, 0x30353132, 0x31353132, 0x32353132, 0x33353132, 0x34353132, 0x35353132, 0x36353132, 0x37353132, 0x38353132, 0x39353132, 0x30363132, 0x31363132, 0x32363132, 0x33363132, 0x34363132, 0x35363132, 0x36363132, 0x37363132, 0x38363132, 0x39363132, 0x30373132, 0x31373132, 0x32373132, 0x33373132, 0x34373132, 0x35373132, 0x36373132, 0x37373132, 0x38373132, 0x39373132, 0x30383132, 0x31383132, 0x32383132, 0x33383132, 0x34383132, 0x35383132, 0x36383132, 0x37383132, 0x38383132, 0x39383132, 0x30393132, 0x31393132, 0x32393132, 0x33393132, 0x34393132, 0x35393132, 0x36393132, 0x37393132, 0x38393132, 0x39393132, 0x30303232, 0x31303232, 0x32303232, 0x33303232, 0x34303232, 0x35303232, 0x36303232, 0x37303232, 0x38303232, 0x39303232, 0x30313232, 0x31313232, 0x32313232, 0x33313232, 0x34313232, 0x35313232, 0x36313232, 0x37313232, 0x38313232, 0x39313232, 0x30323232, 0x31323232, 0x32323232, 0x33323232, 0x34323232, 0x35323232, 0x36323232, 0x37323232, 0x38323232, 0x39323232, 0x30333232, 0x31333232, 0x32333232, 0x33333232, 0x34333232, 0x35333232, 0x36333232, 0x37333232, 0x38333232, 0x39333232, 0x30343232, 0x31343232, 0x32343232, 0x33343232, 0x34343232, 0x35343232, 0x36343232, 0x37343232, 0x38343232, 0x39343232, 0x30353232, 0x31353232, 0x32353232, 0x33353232, 0x34353232, 0x35353232, 0x36353232, 0x37353232, 0x38353232, 0x39353232, 0x30363232, 0x31363232, 0x32363232, 0x33363232, 0x34363232, 0x35363232, 0x36363232, 0x37363232, 0x38363232, 0x39363232, 0x30373232, 0x31373232, 0x32373232, 0x33373232, 0x34373232, 0x35373232, 0x36373232, 0x37373232, 0x38373232, 0x39373232, 0x30383232, 0x31383232, 0x32383232, 0x33383232, 0x34383232, 0x35383232, 0x36383232, 0x37383232, 0x38383232, 0x39383232, 0x30393232, 0x31393232, 0x32393232, 0x33393232, 0x34393232, 0x35393232, 0x36393232, 0x37393232, 0x38393232, 0x39393232, 0x30303332, 0x31303332, 0x32303332, 0x33303332, 0x34303332, 0x35303332, 0x36303332, 0x37303332, 0x38303332, 0x39303332, 0x30313332, 0x31313332, 0x32313332, 0x33313332, 0x34313332, 0x35313332, 0x36313332, 0x37313332, 0x38313332, 0x39313332, 0x30323332, 0x31323332, 0x32323332, 0x33323332, 0x34323332, 0x35323332, 0x36323332, 0x37323332, 0x38323332, 0x39323332, 0x30333332, 0x31333332, 0x32333332, 0x33333332, 0x34333332, 0x35333332, 0x36333332, 0x37333332, 0x38333332, 0x39333332, 0x30343332, 0x31343332, 0x32343332, 0x33343332, 0x34343332, 0x35343332, 0x36343332, 0x37343332, 0x38343332, 0x39343332, 0x30353332, 0x31353332, 0x32353332, 0x33353332, 0x34353332, 0x35353332, 0x36353332, 0x37353332, 0x38353332, 0x39353332, 0x30363332, 0x31363332, 0x32363332, 0x33363332, 0x34363332, 0x35363332, 0x36363332, 0x37363332, 0x38363332, 0x39363332, 0x30373332, 0x31373332, 0x32373332, 0x33373332, 0x34373332, 0x35373332, 0x36373332, 0x37373332, 0x38373332, 0x39373332, 0x30383332, 0x31383332, 0x32383332, 0x33383332, 0x34383332, 0x35383332, 0x36383332, 0x37383332, 0x38383332, 0x39383332, 0x30393332, 0x31393332, 0x32393332, 0x33393332, 0x34393332, 0x35393332, 0x36393332, 0x37393332, 0x38393332, 0x39393332, 0x30303432, 0x31303432, 0x32303432, 0x33303432, 0x34303432, 0x35303432, 0x36303432, 0x37303432, 0x38303432, 0x39303432, 0x30313432, 0x31313432, 0x32313432, 0x33313432, 0x34313432, 0x35313432, 0x36313432, 0x37313432, 0x38313432, 0x39313432, 0x30323432, 0x31323432, 0x32323432, 0x33323432, 0x34323432, 0x35323432, 0x36323432, 0x37323432, 0x38323432, 0x39323432, 0x30333432, 0x31333432, 0x32333432, 0x33333432, 0x34333432, 0x35333432, 0x36333432, 0x37333432, 0x38333432, 0x39333432, 0x30343432, 0x31343432, 0x32343432, 0x33343432, 0x34343432, 0x35343432, 0x36343432, 0x37343432, 0x38343432, 0x39343432, 0x30353432, 0x31353432, 0x32353432, 0x33353432, 0x34353432, 0x35353432, 0x36353432, 0x37353432, 0x38353432, 0x39353432, 0x30363432, 0x31363432, 0x32363432, 0x33363432, 0x34363432, 0x35363432, 0x36363432, 0x37363432, 0x38363432, 0x39363432, 0x30373432, 0x31373432, 0x32373432, 0x33373432, 0x34373432, 0x35373432, 0x36373432, 0x37373432, 0x38373432, 0x39373432, 0x30383432, 0x31383432, 0x32383432, 0x33383432, 0x34383432, 0x35383432, 0x36383432, 0x37383432, 0x38383432, 0x39383432, 0x30393432, 0x31393432, 0x32393432, 0x33393432, 0x34393432, 0x35393432, 0x36393432, 0x37393432, 0x38393432, 0x39393432, 0x30303532, 0x31303532, 0x32303532, 0x33303532, 0x34303532, 0x35303532, 0x36303532, 0x37303532, 0x38303532, 0x39303532, 0x30313532, 0x31313532, 0x32313532, 0x33313532, 0x34313532, 0x35313532, 0x36313532, 0x37313532, 0x38313532, 0x39313532, 0x30323532, 0x31323532, 0x32323532, 0x33323532, 0x34323532, 0x35323532, 0x36323532, 0x37323532, 0x38323532, 0x39323532, 0x30333532, 0x31333532, 0x32333532, 0x33333532, 0x34333532, 0x35333532, 0x36333532, 0x37333532, 0x38333532, 0x39333532, 0x30343532, 0x31343532, 0x32343532, 0x33343532, 0x34343532, 0x35343532, 0x36343532, 0x37343532, 0x38343532, 0x39343532, 0x30353532, 0x31353532, 0x32353532, 0x33353532, 0x34353532, 0x35353532, 0x36353532, 0x37353532, 0x38353532, 0x39353532, 0x30363532, 0x31363532, 0x32363532, 0x33363532, 0x34363532, 0x35363532, 0x36363532, 0x37363532, 0x38363532, 0x39363532, 0x30373532, 0x31373532, 0x32373532, 0x33373532, 0x34373532, 0x35373532, 0x36373532, 0x37373532, 0x38373532, 0x39373532, 0x30383532, 0x31383532, 0x32383532, 0x33383532, 0x34383532, 0x35383532, 0x36383532, 0x37383532, 0x38383532, 0x39383532, 0x30393532, 0x31393532, 0x32393532, 0x33393532, 0x34393532, 0x35393532, 0x36393532, 0x37393532, 0x38393532, 0x39393532, 0x30303632, 0x31303632, 0x32303632, 0x33303632, 0x34303632, 0x35303632, 0x36303632, 0x37303632, 0x38303632, 0x39303632, 0x30313632, 0x31313632, 0x32313632, 0x33313632, 0x34313632, 0x35313632, 0x36313632, 0x37313632, 0x38313632, 0x39313632, 0x30323632, 0x31323632, 0x32323632, 0x33323632, 0x34323632, 0x35323632, 0x36323632, 0x37323632, 0x38323632, 0x39323632, 0x30333632, 0x31333632, 0x32333632, 0x33333632, 0x34333632, 0x35333632, 0x36333632, 0x37333632, 0x38333632, 0x39333632, 0x30343632, 0x31343632, 0x32343632, 0x33343632, 0x34343632, 0x35343632, 0x36343632, 0x37343632, 0x38343632, 0x39343632, 0x30353632, 0x31353632, 0x32353632, 0x33353632, 0x34353632, 0x35353632, 0x36353632, 0x37353632, 0x38353632, 0x39353632, 0x30363632, 0x31363632, 0x32363632, 0x33363632, 0x34363632, 0x35363632, 0x36363632, 0x37363632, 0x38363632, 0x39363632, 0x30373632, 0x31373632, 0x32373632, 0x33373632, 0x34373632, 0x35373632, 0x36373632, 0x37373632, 0x38373632, 0x39373632, 0x30383632, 0x31383632, 0x32383632, 0x33383632, 0x34383632, 0x35383632, 0x36383632, 0x37383632, 0x38383632, 0x39383632, 0x30393632, 0x31393632, 0x32393632, 0x33393632, 0x34393632, 0x35393632, 0x36393632, 0x37393632, 0x38393632, 0x39393632, 0x30303732, 0x31303732, 0x32303732, 0x33303732, 0x34303732, 0x35303732, 0x36303732, 0x37303732, 0x38303732, 0x39303732, 0x30313732, 0x31313732, 0x32313732, 0x33313732, 0x34313732, 0x35313732, 0x36313732, 0x37313732, 0x38313732, 0x39313732, 0x30323732, 0x31323732, 0x32323732, 0x33323732, 0x34323732, 0x35323732, 0x36323732, 0x37323732, 0x38323732, 0x39323732, 0x30333732, 0x31333732, 0x32333732, 0x33333732, 0x34333732, 0x35333732, 0x36333732, 0x37333732, 0x38333732, 0x39333732, 0x30343732, 0x31343732, 0x32343732, 0x33343732, 0x34343732, 0x35343732, 0x36343732, 0x37343732, 0x38343732, 0x39343732, 0x30353732, 0x31353732, 0x32353732, 0x33353732, 0x34353732, 0x35353732, 0x36353732, 0x37353732, 0x38353732, 0x39353732, 0x30363732, 0x31363732, 0x32363732, 0x33363732, 0x34363732, 0x35363732, 0x36363732, 0x37363732, 0x38363732, 0x39363732, 0x30373732, 0x31373732, 0x32373732, 0x33373732, 0x34373732, 0x35373732, 0x36373732, 0x37373732, 0x38373732, 0x39373732, 0x30383732, 0x31383732, 0x32383732, 0x33383732, 0x34383732, 0x35383732, 0x36383732, 0x37383732, 0x38383732, 0x39383732, 0x30393732, 0x31393732, 0x32393732, 0x33393732, 0x34393732, 0x35393732, 0x36393732, 0x37393732, 0x38393732, 0x39393732, 0x30303832, 0x31303832, 0x32303832, 0x33303832, 0x34303832, 0x35303832, 0x36303832, 0x37303832, 0x38303832, 0x39303832, 0x30313832, 0x31313832, 0x32313832, 0x33313832, 0x34313832, 0x35313832, 0x36313832, 0x37313832, 0x38313832, 0x39313832, 0x30323832, 0x31323832, 0x32323832, 0x33323832, 0x34323832, 0x35323832, 0x36323832, 0x37323832, 0x38323832, 0x39323832, 0x30333832, 0x31333832, 0x32333832, 0x33333832, 0x34333832, 0x35333832, 0x36333832, 0x37333832, 0x38333832, 0x39333832, 0x30343832, 0x31343832, 0x32343832, 0x33343832, 0x34343832, 0x35343832, 0x36343832, 0x37343832, 0x38343832, 0x39343832, 0x30353832, 0x31353832, 0x32353832, 0x33353832, 0x34353832, 0x35353832, 0x36353832, 0x37353832, 0x38353832, 0x39353832, 0x30363832, 0x31363832, 0x32363832, 0x33363832, 0x34363832, 0x35363832, 0x36363832, 0x37363832, 0x38363832, 0x39363832, 0x30373832, 0x31373832, 0x32373832, 0x33373832, 0x34373832, 0x35373832, 0x36373832, 0x37373832, 0x38373832, 0x39373832, 0x30383832, 0x31383832, 0x32383832, 0x33383832, 0x34383832, 0x35383832, 0x36383832, 0x37383832, 0x38383832, 0x39383832, 0x30393832, 0x31393832, 0x32393832, 0x33393832, 0x34393832, 0x35393832, 0x36393832, 0x37393832, 0x38393832, 0x39393832, 0x30303932, 0x31303932, 0x32303932, 0x33303932, 0x34303932, 0x35303932, 0x36303932, 0x37303932, 0x38303932, 0x39303932, 0x30313932, 0x31313932, 0x32313932, 0x33313932, 0x34313932, 0x35313932, 0x36313932, 0x37313932, 0x38313932, 0x39313932, 0x30323932, 0x31323932, 0x32323932, 0x33323932, 0x34323932, 0x35323932, 0x36323932, 0x37323932, 0x38323932, 0x39323932, 0x30333932, 0x31333932, 0x32333932, 0x33333932, 0x34333932, 0x35333932, 0x36333932, 0x37333932, 0x38333932, 0x39333932, 0x30343932, 0x31343932, 0x32343932, 0x33343932, 0x34343932, 0x35343932, 0x36343932, 0x37343932, 0x38343932, 0x39343932, 0x30353932, 0x31353932, 0x32353932, 0x33353932, 0x34353932, 0x35353932, 0x36353932, 0x37353932, 0x38353932, 0x39353932, 0x30363932, 0x31363932, 0x32363932, 0x33363932, 0x34363932, 0x35363932, 0x36363932, 0x37363932, 0x38363932, 0x39363932, 0x30373932, 0x31373932, 0x32373932, 0x33373932, 0x34373932, 0x35373932, 0x36373932, 0x37373932, 0x38373932, 0x39373932, 0x30383932, 0x31383932, 0x32383932, 0x33383932, 0x34383932, 0x35383932, 0x36383932, 0x37383932, 0x38383932, 0x39383932, 0x30393932, 0x31393932, 0x32393932, 0x33393932, 0x34393932, 0x35393932, 0x36393932, 0x37393932, 0x38393932, 0x39393932, 0x30303033, 0x31303033, 0x32303033, 0x33303033, 0x34303033, 0x35303033, 0x36303033, 0x37303033, 0x38303033, 0x39303033, 0x30313033, 0x31313033, 0x32313033, 0x33313033, 0x34313033, 0x35313033, 0x36313033, 0x37313033, 0x38313033, 0x39313033, 0x30323033, 0x31323033, 0x32323033, 0x33323033, 0x34323033, 0x35323033, 0x36323033, 0x37323033, 0x38323033, 0x39323033, 0x30333033, 0x31333033, 0x32333033, 0x33333033, 0x34333033, 0x35333033, 0x36333033, 0x37333033, 0x38333033, 0x39333033, 0x30343033, 0x31343033, 0x32343033, 0x33343033, 0x34343033, 0x35343033, 0x36343033, 0x37343033, 0x38343033, 0x39343033, 0x30353033, 0x31353033, 0x32353033, 0x33353033, 0x34353033, 0x35353033, 0x36353033, 0x37353033, 0x38353033, 0x39353033, 0x30363033, 0x31363033, 0x32363033, 0x33363033, 0x34363033, 0x35363033, 0x36363033, 0x37363033, 0x38363033, 0x39363033, 0x30373033, 0x31373033, 0x32373033, 0x33373033, 0x34373033, 0x35373033, 0x36373033, 0x37373033, 0x38373033, 0x39373033, 0x30383033, 0x31383033, 0x32383033, 0x33383033, 0x34383033, 0x35383033, 0x36383033, 0x37383033, 0x38383033, 0x39383033, 0x30393033, 0x31393033, 0x32393033, 0x33393033, 0x34393033, 0x35393033, 0x36393033, 0x37393033, 0x38393033, 0x39393033, 0x30303133, 0x31303133, 0x32303133, 0x33303133, 0x34303133, 0x35303133, 0x36303133, 0x37303133, 0x38303133, 0x39303133, 0x30313133, 0x31313133, 0x32313133, 0x33313133, 0x34313133, 0x35313133, 0x36313133, 0x37313133, 0x38313133, 0x39313133, 0x30323133, 0x31323133, 0x32323133, 0x33323133, 0x34323133, 0x35323133, 0x36323133, 0x37323133, 0x38323133, 0x39323133, 0x30333133, 0x31333133, 0x32333133, 0x33333133, 0x34333133, 0x35333133, 0x36333133, 0x37333133, 0x38333133, 0x39333133, 0x30343133, 0x31343133, 0x32343133, 0x33343133, 0x34343133, 0x35343133, 0x36343133, 0x37343133, 0x38343133, 0x39343133, 0x30353133, 0x31353133, 0x32353133, 0x33353133, 0x34353133, 0x35353133, 0x36353133, 0x37353133, 0x38353133, 0x39353133, 0x30363133, 0x31363133, 0x32363133, 0x33363133, 0x34363133, 0x35363133, 0x36363133, 0x37363133, 0x38363133, 0x39363133, 0x30373133, 0x31373133, 0x32373133, 0x33373133, 0x34373133, 0x35373133, 0x36373133, 0x37373133, 0x38373133, 0x39373133, 0x30383133, 0x31383133, 0x32383133, 0x33383133, 0x34383133, 0x35383133, 0x36383133, 0x37383133, 0x38383133, 0x39383133, 0x30393133, 0x31393133, 0x32393133, 0x33393133, 0x34393133, 0x35393133, 0x36393133, 0x37393133, 0x38393133, 0x39393133, 0x30303233, 0x31303233, 0x32303233, 0x33303233, 0x34303233, 0x35303233, 0x36303233, 0x37303233, 0x38303233, 0x39303233, 0x30313233, 0x31313233, 0x32313233, 0x33313233, 0x34313233, 0x35313233, 0x36313233, 0x37313233, 0x38313233, 0x39313233, 0x30323233, 0x31323233, 0x32323233, 0x33323233, 0x34323233, 0x35323233, 0x36323233, 0x37323233, 0x38323233, 0x39323233, 0x30333233, 0x31333233, 0x32333233, 0x33333233, 0x34333233, 0x35333233, 0x36333233, 0x37333233, 0x38333233, 0x39333233, 0x30343233, 0x31343233, 0x32343233, 0x33343233, 0x34343233, 0x35343233, 0x36343233, 0x37343233, 0x38343233, 0x39343233, 0x30353233, 0x31353233, 0x32353233, 0x33353233, 0x34353233, 0x35353233, 0x36353233, 0x37353233, 0x38353233, 0x39353233, 0x30363233, 0x31363233, 0x32363233, 0x33363233, 0x34363233, 0x35363233, 0x36363233, 0x37363233, 0x38363233, 0x39363233, 0x30373233, 0x31373233, 0x32373233, 0x33373233, 0x34373233, 0x35373233, 0x36373233, 0x37373233, 0x38373233, 0x39373233, 0x30383233, 0x31383233, 0x32383233, 0x33383233, 0x34383233, 0x35383233, 0x36383233, 0x37383233, 0x38383233, 0x39383233, 0x30393233, 0x31393233, 0x32393233, 0x33393233, 0x34393233, 0x35393233, 0x36393233, 0x37393233, 0x38393233, 0x39393233, 0x30303333, 0x31303333, 0x32303333, 0x33303333, 0x34303333, 0x35303333, 0x36303333, 0x37303333, 0x38303333, 0x39303333, 0x30313333, 0x31313333, 0x32313333, 0x33313333, 0x34313333, 0x35313333, 0x36313333, 0x37313333, 0x38313333, 0x39313333, 0x30323333, 0x31323333, 0x32323333, 0x33323333, 0x34323333, 0x35323333, 0x36323333, 0x37323333, 0x38323333, 0x39323333, 0x30333333, 0x31333333, 0x32333333, 0x33333333, 0x34333333, 0x35333333, 0x36333333, 0x37333333, 0x38333333, 0x39333333, 0x30343333, 0x31343333, 0x32343333, 0x33343333, 0x34343333, 0x35343333, 0x36343333, 0x37343333, 0x38343333, 0x39343333, 0x30353333, 0x31353333, 0x32353333, 0x33353333, 0x34353333, 0x35353333, 0x36353333, 0x37353333, 0x38353333, 0x39353333, 0x30363333, 0x31363333, 0x32363333, 0x33363333, 0x34363333, 0x35363333, 0x36363333, 0x37363333, 0x38363333, 0x39363333, 0x30373333, 0x31373333, 0x32373333, 0x33373333, 0x34373333, 0x35373333, 0x36373333, 0x37373333, 0x38373333, 0x39373333, 0x30383333, 0x31383333, 0x32383333, 0x33383333, 0x34383333, 0x35383333, 0x36383333, 0x37383333, 0x38383333, 0x39383333, 0x30393333, 0x31393333, 0x32393333, 0x33393333, 0x34393333, 0x35393333, 0x36393333, 0x37393333, 0x38393333, 0x39393333, 0x30303433, 0x31303433, 0x32303433, 0x33303433, 0x34303433, 0x35303433, 0x36303433, 0x37303433, 0x38303433, 0x39303433, 0x30313433, 0x31313433, 0x32313433, 0x33313433, 0x34313433, 0x35313433, 0x36313433, 0x37313433, 0x38313433, 0x39313433, 0x30323433, 0x31323433, 0x32323433, 0x33323433, 0x34323433, 0x35323433, 0x36323433, 0x37323433, 0x38323433, 0x39323433, 0x30333433, 0x31333433, 0x32333433, 0x33333433, 0x34333433, 0x35333433, 0x36333433, 0x37333433, 0x38333433, 0x39333433, 0x30343433, 0x31343433, 0x32343433, 0x33343433, 0x34343433, 0x35343433, 0x36343433, 0x37343433, 0x38343433, 0x39343433, 0x30353433, 0x31353433, 0x32353433, 0x33353433, 0x34353433, 0x35353433, 0x36353433, 0x37353433, 0x38353433, 0x39353433, 0x30363433, 0x31363433, 0x32363433, 0x33363433, 0x34363433, 0x35363433, 0x36363433, 0x37363433, 0x38363433, 0x39363433, 0x30373433, 0x31373433, 0x32373433, 0x33373433, 0x34373433, 0x35373433, 0x36373433, 0x37373433, 0x38373433, 0x39373433, 0x30383433, 0x31383433, 0x32383433, 0x33383433, 0x34383433, 0x35383433, 0x36383433, 0x37383433, 0x38383433, 0x39383433, 0x30393433, 0x31393433, 0x32393433, 0x33393433, 0x34393433, 0x35393433, 0x36393433, 0x37393433, 0x38393433, 0x39393433, 0x30303533, 0x31303533, 0x32303533, 0x33303533, 0x34303533, 0x35303533, 0x36303533, 0x37303533, 0x38303533, 0x39303533, 0x30313533, 0x31313533, 0x32313533, 0x33313533, 0x34313533, 0x35313533, 0x36313533, 0x37313533, 0x38313533, 0x39313533, 0x30323533, 0x31323533, 0x32323533, 0x33323533, 0x34323533, 0x35323533, 0x36323533, 0x37323533, 0x38323533, 0x39323533, 0x30333533, 0x31333533, 0x32333533, 0x33333533, 0x34333533, 0x35333533, 0x36333533, 0x37333533, 0x38333533, 0x39333533, 0x30343533, 0x31343533, 0x32343533, 0x33343533, 0x34343533, 0x35343533, 0x36343533, 0x37343533, 0x38343533, 0x39343533, 0x30353533, 0x31353533, 0x32353533, 0x33353533, 0x34353533, 0x35353533, 0x36353533, 0x37353533, 0x38353533, 0x39353533, 0x30363533, 0x31363533, 0x32363533, 0x33363533, 0x34363533, 0x35363533, 0x36363533, 0x37363533, 0x38363533, 0x39363533, 0x30373533, 0x31373533, 0x32373533, 0x33373533, 0x34373533, 0x35373533, 0x36373533, 0x37373533, 0x38373533, 0x39373533, 0x30383533, 0x31383533, 0x32383533, 0x33383533, 0x34383533, 0x35383533, 0x36383533, 0x37383533, 0x38383533, 0x39383533, 0x30393533, 0x31393533, 0x32393533, 0x33393533, 0x34393533, 0x35393533, 0x36393533, 0x37393533, 0x38393533, 0x39393533, 0x30303633, 0x31303633, 0x32303633, 0x33303633, 0x34303633, 0x35303633, 0x36303633, 0x37303633, 0x38303633, 0x39303633, 0x30313633, 0x31313633, 0x32313633, 0x33313633, 0x34313633, 0x35313633, 0x36313633, 0x37313633, 0x38313633, 0x39313633, 0x30323633, 0x31323633, 0x32323633, 0x33323633, 0x34323633, 0x35323633, 0x36323633, 0x37323633, 0x38323633, 0x39323633, 0x30333633, 0x31333633, 0x32333633, 0x33333633, 0x34333633, 0x35333633, 0x36333633, 0x37333633, 0x38333633, 0x39333633, 0x30343633, 0x31343633, 0x32343633, 0x33343633, 0x34343633, 0x35343633, 0x36343633, 0x37343633, 0x38343633, 0x39343633, 0x30353633, 0x31353633, 0x32353633, 0x33353633, 0x34353633, 0x35353633, 0x36353633, 0x37353633, 0x38353633, 0x39353633, 0x30363633, 0x31363633, 0x32363633, 0x33363633, 0x34363633, 0x35363633, 0x36363633, 0x37363633, 0x38363633, 0x39363633, 0x30373633, 0x31373633, 0x32373633, 0x33373633, 0x34373633, 0x35373633, 0x36373633, 0x37373633, 0x38373633, 0x39373633, 0x30383633, 0x31383633, 0x32383633, 0x33383633, 0x34383633, 0x35383633, 0x36383633, 0x37383633, 0x38383633, 0x39383633, 0x30393633, 0x31393633, 0x32393633, 0x33393633, 0x34393633, 0x35393633, 0x36393633, 0x37393633, 0x38393633, 0x39393633, 0x30303733, 0x31303733, 0x32303733, 0x33303733, 0x34303733, 0x35303733, 0x36303733, 0x37303733, 0x38303733, 0x39303733, 0x30313733, 0x31313733, 0x32313733, 0x33313733, 0x34313733, 0x35313733, 0x36313733, 0x37313733, 0x38313733, 0x39313733, 0x30323733, 0x31323733, 0x32323733, 0x33323733, 0x34323733, 0x35323733, 0x36323733, 0x37323733, 0x38323733, 0x39323733, 0x30333733, 0x31333733, 0x32333733, 0x33333733, 0x34333733, 0x35333733, 0x36333733, 0x37333733, 0x38333733, 0x39333733, 0x30343733, 0x31343733, 0x32343733, 0x33343733, 0x34343733, 0x35343733, 0x36343733, 0x37343733, 0x38343733, 0x39343733, 0x30353733, 0x31353733, 0x32353733, 0x33353733, 0x34353733, 0x35353733, 0x36353733, 0x37353733, 0x38353733, 0x39353733, 0x30363733, 0x31363733, 0x32363733, 0x33363733, 0x34363733, 0x35363733, 0x36363733, 0x37363733, 0x38363733, 0x39363733, 0x30373733, 0x31373733, 0x32373733, 0x33373733, 0x34373733, 0x35373733, 0x36373733, 0x37373733, 0x38373733, 0x39373733, 0x30383733, 0x31383733, 0x32383733, 0x33383733, 0x34383733, 0x35383733, 0x36383733, 0x37383733, 0x38383733, 0x39383733, 0x30393733, 0x31393733, 0x32393733, 0x33393733, 0x34393733, 0x35393733, 0x36393733, 0x37393733, 0x38393733, 0x39393733, 0x30303833, 0x31303833, 0x32303833, 0x33303833, 0x34303833, 0x35303833, 0x36303833, 0x37303833, 0x38303833, 0x39303833, 0x30313833, 0x31313833, 0x32313833, 0x33313833, 0x34313833, 0x35313833, 0x36313833, 0x37313833, 0x38313833, 0x39313833, 0x30323833, 0x31323833, 0x32323833, 0x33323833, 0x34323833, 0x35323833, 0x36323833, 0x37323833, 0x38323833, 0x39323833, 0x30333833, 0x31333833, 0x32333833, 0x33333833, 0x34333833, 0x35333833, 0x36333833, 0x37333833, 0x38333833, 0x39333833, 0x30343833, 0x31343833, 0x32343833, 0x33343833, 0x34343833, 0x35343833, 0x36343833, 0x37343833, 0x38343833, 0x39343833, 0x30353833, 0x31353833, 0x32353833, 0x33353833, 0x34353833, 0x35353833, 0x36353833, 0x37353833, 0x38353833, 0x39353833, 0x30363833, 0x31363833, 0x32363833, 0x33363833, 0x34363833, 0x35363833, 0x36363833, 0x37363833, 0x38363833, 0x39363833, 0x30373833, 0x31373833, 0x32373833, 0x33373833, 0x34373833, 0x35373833, 0x36373833, 0x37373833, 0x38373833, 0x39373833, 0x30383833, 0x31383833, 0x32383833, 0x33383833, 0x34383833, 0x35383833, 0x36383833, 0x37383833, 0x38383833, 0x39383833, 0x30393833, 0x31393833, 0x32393833, 0x33393833, 0x34393833, 0x35393833, 0x36393833, 0x37393833, 0x38393833, 0x39393833, 0x30303933, 0x31303933, 0x32303933, 0x33303933, 0x34303933, 0x35303933, 0x36303933, 0x37303933, 0x38303933, 0x39303933, 0x30313933, 0x31313933, 0x32313933, 0x33313933, 0x34313933, 0x35313933, 0x36313933, 0x37313933, 0x38313933, 0x39313933, 0x30323933, 0x31323933, 0x32323933, 0x33323933, 0x34323933, 0x35323933, 0x36323933, 0x37323933, 0x38323933, 0x39323933, 0x30333933, 0x31333933, 0x32333933, 0x33333933, 0x34333933, 0x35333933, 0x36333933, 0x37333933, 0x38333933, 0x39333933, 0x30343933, 0x31343933, 0x32343933, 0x33343933, 0x34343933, 0x35343933, 0x36343933, 0x37343933, 0x38343933, 0x39343933, 0x30353933, 0x31353933, 0x32353933, 0x33353933, 0x34353933, 0x35353933, 0x36353933, 0x37353933, 0x38353933, 0x39353933, 0x30363933, 0x31363933, 0x32363933, 0x33363933, 0x34363933, 0x35363933, 0x36363933, 0x37363933, 0x38363933, 0x39363933, 0x30373933, 0x31373933, 0x32373933, 0x33373933, 0x34373933, 0x35373933, 0x36373933, 0x37373933, 0x38373933, 0x39373933, 0x30383933, 0x31383933, 0x32383933, 0x33383933, 0x34383933, 0x35383933, 0x36383933, 0x37383933, 0x38383933, 0x39383933, 0x30393933, 0x31393933, 0x32393933, 0x33393933, 0x34393933, 0x35393933, 0x36393933, 0x37393933, 0x38393933, 0x39393933, 0x30303034, 0x31303034, 0x32303034, 0x33303034, 0x34303034, 0x35303034, 0x36303034, 0x37303034, 0x38303034, 0x39303034, 0x30313034, 0x31313034, 0x32313034, 0x33313034, 0x34313034, 0x35313034, 0x36313034, 0x37313034, 0x38313034, 0x39313034, 0x30323034, 0x31323034, 0x32323034, 0x33323034, 0x34323034, 0x35323034, 0x36323034, 0x37323034, 0x38323034, 0x39323034, 0x30333034, 0x31333034, 0x32333034, 0x33333034, 0x34333034, 0x35333034, 0x36333034, 0x37333034, 0x38333034, 0x39333034, 0x30343034, 0x31343034, 0x32343034, 0x33343034, 0x34343034, 0x35343034, 0x36343034, 0x37343034, 0x38343034, 0x39343034, 0x30353034, 0x31353034, 0x32353034, 0x33353034, 0x34353034, 0x35353034, 0x36353034, 0x37353034, 0x38353034, 0x39353034, 0x30363034, 0x31363034, 0x32363034, 0x33363034, 0x34363034, 0x35363034, 0x36363034, 0x37363034, 0x38363034, 0x39363034, 0x30373034, 0x31373034, 0x32373034, 0x33373034, 0x34373034, 0x35373034, 0x36373034, 0x37373034, 0x38373034, 0x39373034, 0x30383034, 0x31383034, 0x32383034, 0x33383034, 0x34383034, 0x35383034, 0x36383034, 0x37383034, 0x38383034, 0x39383034, 0x30393034, 0x31393034, 0x32393034, 0x33393034, 0x34393034, 0x35393034, 0x36393034, 0x37393034, 0x38393034, 0x39393034, 0x30303134, 0x31303134, 0x32303134, 0x33303134, 0x34303134, 0x35303134, 0x36303134, 0x37303134, 0x38303134, 0x39303134, 0x30313134, 0x31313134, 0x32313134, 0x33313134, 0x34313134, 0x35313134, 0x36313134, 0x37313134, 0x38313134, 0x39313134, 0x30323134, 0x31323134, 0x32323134, 0x33323134, 0x34323134, 0x35323134, 0x36323134, 0x37323134, 0x38323134, 0x39323134, 0x30333134, 0x31333134, 0x32333134, 0x33333134, 0x34333134, 0x35333134, 0x36333134, 0x37333134, 0x38333134, 0x39333134, 0x30343134, 0x31343134, 0x32343134, 0x33343134, 0x34343134, 0x35343134, 0x36343134, 0x37343134, 0x38343134, 0x39343134, 0x30353134, 0x31353134, 0x32353134, 0x33353134, 0x34353134, 0x35353134, 0x36353134, 0x37353134, 0x38353134, 0x39353134, 0x30363134, 0x31363134, 0x32363134, 0x33363134, 0x34363134, 0x35363134, 0x36363134, 0x37363134, 0x38363134, 0x39363134, 0x30373134, 0x31373134, 0x32373134, 0x33373134, 0x34373134, 0x35373134, 0x36373134, 0x37373134, 0x38373134, 0x39373134, 0x30383134, 0x31383134, 0x32383134, 0x33383134, 0x34383134, 0x35383134, 0x36383134, 0x37383134, 0x38383134, 0x39383134, 0x30393134, 0x31393134, 0x32393134, 0x33393134, 0x34393134, 0x35393134, 0x36393134, 0x37393134, 0x38393134, 0x39393134, 0x30303234, 0x31303234, 0x32303234, 0x33303234, 0x34303234, 0x35303234, 0x36303234, 0x37303234, 0x38303234, 0x39303234, 0x30313234, 0x31313234, 0x32313234, 0x33313234, 0x34313234, 0x35313234, 0x36313234, 0x37313234, 0x38313234, 0x39313234, 0x30323234, 0x31323234, 0x32323234, 0x33323234, 0x34323234, 0x35323234, 0x36323234, 0x37323234, 0x38323234, 0x39323234, 0x30333234, 0x31333234, 0x32333234, 0x33333234, 0x34333234, 0x35333234, 0x36333234, 0x37333234, 0x38333234, 0x39333234, 0x30343234, 0x31343234, 0x32343234, 0x33343234, 0x34343234, 0x35343234, 0x36343234, 0x37343234, 0x38343234, 0x39343234, 0x30353234, 0x31353234, 0x32353234, 0x33353234, 0x34353234, 0x35353234, 0x36353234, 0x37353234, 0x38353234, 0x39353234, 0x30363234, 0x31363234, 0x32363234, 0x33363234, 0x34363234, 0x35363234, 0x36363234, 0x37363234, 0x38363234, 0x39363234, 0x30373234, 0x31373234, 0x32373234, 0x33373234, 0x34373234, 0x35373234, 0x36373234, 0x37373234, 0x38373234, 0x39373234, 0x30383234, 0x31383234, 0x32383234, 0x33383234, 0x34383234, 0x35383234, 0x36383234, 0x37383234, 0x38383234, 0x39383234, 0x30393234, 0x31393234, 0x32393234, 0x33393234, 0x34393234, 0x35393234, 0x36393234, 0x37393234, 0x38393234, 0x39393234, 0x30303334, 0x31303334, 0x32303334, 0x33303334, 0x34303334, 0x35303334, 0x36303334, 0x37303334, 0x38303334, 0x39303334, 0x30313334, 0x31313334, 0x32313334, 0x33313334, 0x34313334, 0x35313334, 0x36313334, 0x37313334, 0x38313334, 0x39313334, 0x30323334, 0x31323334, 0x32323334, 0x33323334, 0x34323334, 0x35323334, 0x36323334, 0x37323334, 0x38323334, 0x39323334, 0x30333334, 0x31333334, 0x32333334, 0x33333334, 0x34333334, 0x35333334, 0x36333334, 0x37333334, 0x38333334, 0x39333334, 0x30343334, 0x31343334, 0x32343334, 0x33343334, 0x34343334, 0x35343334, 0x36343334, 0x37343334, 0x38343334, 0x39343334, 0x30353334, 0x31353334, 0x32353334, 0x33353334, 0x34353334, 0x35353334, 0x36353334, 0x37353334, 0x38353334, 0x39353334, 0x30363334, 0x31363334, 0x32363334, 0x33363334, 0x34363334, 0x35363334, 0x36363334, 0x37363334, 0x38363334, 0x39363334, 0x30373334, 0x31373334, 0x32373334, 0x33373334, 0x34373334, 0x35373334, 0x36373334, 0x37373334, 0x38373334, 0x39373334, 0x30383334, 0x31383334, 0x32383334, 0x33383334, 0x34383334, 0x35383334, 0x36383334, 0x37383334, 0x38383334, 0x39383334, 0x30393334, 0x31393334, 0x32393334, 0x33393334, 0x34393334, 0x35393334, 0x36393334, 0x37393334, 0x38393334, 0x39393334, 0x30303434, 0x31303434, 0x32303434, 0x33303434, 0x34303434, 0x35303434, 0x36303434, 0x37303434, 0x38303434, 0x39303434, 0x30313434, 0x31313434, 0x32313434, 0x33313434, 0x34313434, 0x35313434, 0x36313434, 0x37313434, 0x38313434, 0x39313434, 0x30323434, 0x31323434, 0x32323434, 0x33323434, 0x34323434, 0x35323434, 0x36323434, 0x37323434, 0x38323434, 0x39323434, 0x30333434, 0x31333434, 0x32333434, 0x33333434, 0x34333434, 0x35333434, 0x36333434, 0x37333434, 0x38333434, 0x39333434, 0x30343434, 0x31343434, 0x32343434, 0x33343434, 0x34343434, 0x35343434, 0x36343434, 0x37343434, 0x38343434, 0x39343434, 0x30353434, 0x31353434, 0x32353434, 0x33353434, 0x34353434, 0x35353434, 0x36353434, 0x37353434, 0x38353434, 0x39353434, 0x30363434, 0x31363434, 0x32363434, 0x33363434, 0x34363434, 0x35363434, 0x36363434, 0x37363434, 0x38363434, 0x39363434, 0x30373434, 0x31373434, 0x32373434, 0x33373434, 0x34373434, 0x35373434, 0x36373434, 0x37373434, 0x38373434, 0x39373434, 0x30383434, 0x31383434, 0x32383434, 0x33383434, 0x34383434, 0x35383434, 0x36383434, 0x37383434, 0x38383434, 0x39383434, 0x30393434, 0x31393434, 0x32393434, 0x33393434, 0x34393434, 0x35393434, 0x36393434, 0x37393434, 0x38393434, 0x39393434, 0x30303534, 0x31303534, 0x32303534, 0x33303534, 0x34303534, 0x35303534, 0x36303534, 0x37303534, 0x38303534, 0x39303534, 0x30313534, 0x31313534, 0x32313534, 0x33313534, 0x34313534, 0x35313534, 0x36313534, 0x37313534, 0x38313534, 0x39313534, 0x30323534, 0x31323534, 0x32323534, 0x33323534, 0x34323534, 0x35323534, 0x36323534, 0x37323534, 0x38323534, 0x39323534, 0x30333534, 0x31333534, 0x32333534, 0x33333534, 0x34333534, 0x35333534, 0x36333534, 0x37333534, 0x38333534, 0x39333534, 0x30343534, 0x31343534, 0x32343534, 0x33343534, 0x34343534, 0x35343534, 0x36343534, 0x37343534, 0x38343534, 0x39343534, 0x30353534, 0x31353534, 0x32353534, 0x33353534, 0x34353534, 0x35353534, 0x36353534, 0x37353534, 0x38353534, 0x39353534, 0x30363534, 0x31363534, 0x32363534, 0x33363534, 0x34363534, 0x35363534, 0x36363534, 0x37363534, 0x38363534, 0x39363534, 0x30373534, 0x31373534, 0x32373534, 0x33373534, 0x34373534, 0x35373534, 0x36373534, 0x37373534, 0x38373534, 0x39373534, 0x30383534, 0x31383534, 0x32383534, 0x33383534, 0x34383534, 0x35383534, 0x36383534, 0x37383534, 0x38383534, 0x39383534, 0x30393534, 0x31393534, 0x32393534, 0x33393534, 0x34393534, 0x35393534, 0x36393534, 0x37393534, 0x38393534, 0x39393534, 0x30303634, 0x31303634, 0x32303634, 0x33303634, 0x34303634, 0x35303634, 0x36303634, 0x37303634, 0x38303634, 0x39303634, 0x30313634, 0x31313634, 0x32313634, 0x33313634, 0x34313634, 0x35313634, 0x36313634, 0x37313634, 0x38313634, 0x39313634, 0x30323634, 0x31323634, 0x32323634, 0x33323634, 0x34323634, 0x35323634, 0x36323634, 0x37323634, 0x38323634, 0x39323634, 0x30333634, 0x31333634, 0x32333634, 0x33333634, 0x34333634, 0x35333634, 0x36333634, 0x37333634, 0x38333634, 0x39333634, 0x30343634, 0x31343634, 0x32343634, 0x33343634, 0x34343634, 0x35343634, 0x36343634, 0x37343634, 0x38343634, 0x39343634, 0x30353634, 0x31353634, 0x32353634, 0x33353634, 0x34353634, 0x35353634, 0x36353634, 0x37353634, 0x38353634, 0x39353634, 0x30363634, 0x31363634, 0x32363634, 0x33363634, 0x34363634, 0x35363634, 0x36363634, 0x37363634, 0x38363634, 0x39363634, 0x30373634, 0x31373634, 0x32373634, 0x33373634, 0x34373634, 0x35373634, 0x36373634, 0x37373634, 0x38373634, 0x39373634, 0x30383634, 0x31383634, 0x32383634, 0x33383634, 0x34383634, 0x35383634, 0x36383634, 0x37383634, 0x38383634, 0x39383634, 0x30393634, 0x31393634, 0x32393634, 0x33393634, 0x34393634, 0x35393634, 0x36393634, 0x37393634, 0x38393634, 0x39393634, 0x30303734, 0x31303734, 0x32303734, 0x33303734, 0x34303734, 0x35303734, 0x36303734, 0x37303734, 0x38303734, 0x39303734, 0x30313734, 0x31313734, 0x32313734, 0x33313734, 0x34313734, 0x35313734, 0x36313734, 0x37313734, 0x38313734, 0x39313734, 0x30323734, 0x31323734, 0x32323734, 0x33323734, 0x34323734, 0x35323734, 0x36323734, 0x37323734, 0x38323734, 0x39323734, 0x30333734, 0x31333734, 0x32333734, 0x33333734, 0x34333734, 0x35333734, 0x36333734, 0x37333734, 0x38333734, 0x39333734, 0x30343734, 0x31343734, 0x32343734, 0x33343734, 0x34343734, 0x35343734, 0x36343734, 0x37343734, 0x38343734, 0x39343734, 0x30353734, 0x31353734, 0x32353734, 0x33353734, 0x34353734, 0x35353734, 0x36353734, 0x37353734, 0x38353734, 0x39353734, 0x30363734, 0x31363734, 0x32363734, 0x33363734, 0x34363734, 0x35363734, 0x36363734, 0x37363734, 0x38363734, 0x39363734, 0x30373734, 0x31373734, 0x32373734, 0x33373734, 0x34373734, 0x35373734, 0x36373734, 0x37373734, 0x38373734, 0x39373734, 0x30383734, 0x31383734, 0x32383734, 0x33383734, 0x34383734, 0x35383734, 0x36383734, 0x37383734, 0x38383734, 0x39383734, 0x30393734, 0x31393734, 0x32393734, 0x33393734, 0x34393734, 0x35393734, 0x36393734, 0x37393734, 0x38393734, 0x39393734, 0x30303834, 0x31303834, 0x32303834, 0x33303834, 0x34303834, 0x35303834, 0x36303834, 0x37303834, 0x38303834, 0x39303834, 0x30313834, 0x31313834, 0x32313834, 0x33313834, 0x34313834, 0x35313834, 0x36313834, 0x37313834, 0x38313834, 0x39313834, 0x30323834, 0x31323834, 0x32323834, 0x33323834, 0x34323834, 0x35323834, 0x36323834, 0x37323834, 0x38323834, 0x39323834, 0x30333834, 0x31333834, 0x32333834, 0x33333834, 0x34333834, 0x35333834, 0x36333834, 0x37333834, 0x38333834, 0x39333834, 0x30343834, 0x31343834, 0x32343834, 0x33343834, 0x34343834, 0x35343834, 0x36343834, 0x37343834, 0x38343834, 0x39343834, 0x30353834, 0x31353834, 0x32353834, 0x33353834, 0x34353834, 0x35353834, 0x36353834, 0x37353834, 0x38353834, 0x39353834, 0x30363834, 0x31363834, 0x32363834, 0x33363834, 0x34363834, 0x35363834, 0x36363834, 0x37363834, 0x38363834, 0x39363834, 0x30373834, 0x31373834, 0x32373834, 0x33373834, 0x34373834, 0x35373834, 0x36373834, 0x37373834, 0x38373834, 0x39373834, 0x30383834, 0x31383834, 0x32383834, 0x33383834, 0x34383834, 0x35383834, 0x36383834, 0x37383834, 0x38383834, 0x39383834, 0x30393834, 0x31393834, 0x32393834, 0x33393834, 0x34393834, 0x35393834, 0x36393834, 0x37393834, 0x38393834, 0x39393834, 0x30303934, 0x31303934, 0x32303934, 0x33303934, 0x34303934, 0x35303934, 0x36303934, 0x37303934, 0x38303934, 0x39303934, 0x30313934, 0x31313934, 0x32313934, 0x33313934, 0x34313934, 0x35313934, 0x36313934, 0x37313934, 0x38313934, 0x39313934, 0x30323934, 0x31323934, 0x32323934, 0x33323934, 0x34323934, 0x35323934, 0x36323934, 0x37323934, 0x38323934, 0x39323934, 0x30333934, 0x31333934, 0x32333934, 0x33333934, 0x34333934, 0x35333934, 0x36333934, 0x37333934, 0x38333934, 0x39333934, 0x30343934, 0x31343934, 0x32343934, 0x33343934, 0x34343934, 0x35343934, 0x36343934, 0x37343934, 0x38343934, 0x39343934, 0x30353934, 0x31353934, 0x32353934, 0x33353934, 0x34353934, 0x35353934, 0x36353934, 0x37353934, 0x38353934, 0x39353934, 0x30363934, 0x31363934, 0x32363934, 0x33363934, 0x34363934, 0x35363934, 0x36363934, 0x37363934, 0x38363934, 0x39363934, 0x30373934, 0x31373934, 0x32373934, 0x33373934, 0x34373934, 0x35373934, 0x36373934, 0x37373934, 0x38373934, 0x39373934, 0x30383934, 0x31383934, 0x32383934, 0x33383934, 0x34383934, 0x35383934, 0x36383934, 0x37383934, 0x38383934, 0x39383934, 0x30393934, 0x31393934, 0x32393934, 0x33393934, 0x34393934, 0x35393934, 0x36393934, 0x37393934, 0x38393934, 0x39393934, 0x30303035, 0x31303035, 0x32303035, 0x33303035, 0x34303035, 0x35303035, 0x36303035, 0x37303035, 0x38303035, 0x39303035, 0x30313035, 0x31313035, 0x32313035, 0x33313035, 0x34313035, 0x35313035, 0x36313035, 0x37313035, 0x38313035, 0x39313035, 0x30323035, 0x31323035, 0x32323035, 0x33323035, 0x34323035, 0x35323035, 0x36323035, 0x37323035, 0x38323035, 0x39323035, 0x30333035, 0x31333035, 0x32333035, 0x33333035, 0x34333035, 0x35333035, 0x36333035, 0x37333035, 0x38333035, 0x39333035, 0x30343035, 0x31343035, 0x32343035, 0x33343035, 0x34343035, 0x35343035, 0x36343035, 0x37343035, 0x38343035, 0x39343035, 0x30353035, 0x31353035, 0x32353035, 0x33353035, 0x34353035, 0x35353035, 0x36353035, 0x37353035, 0x38353035, 0x39353035, 0x30363035, 0x31363035, 0x32363035, 0x33363035, 0x34363035, 0x35363035, 0x36363035, 0x37363035, 0x38363035, 0x39363035, 0x30373035, 0x31373035, 0x32373035, 0x33373035, 0x34373035, 0x35373035, 0x36373035, 0x37373035, 0x38373035, 0x39373035, 0x30383035, 0x31383035, 0x32383035, 0x33383035, 0x34383035, 0x35383035, 0x36383035, 0x37383035, 0x38383035, 0x39383035, 0x30393035, 0x31393035, 0x32393035, 0x33393035, 0x34393035, 0x35393035, 0x36393035, 0x37393035, 0x38393035, 0x39393035, 0x30303135, 0x31303135, 0x32303135, 0x33303135, 0x34303135, 0x35303135, 0x36303135, 0x37303135, 0x38303135, 0x39303135, 0x30313135, 0x31313135, 0x32313135, 0x33313135, 0x34313135, 0x35313135, 0x36313135, 0x37313135, 0x38313135, 0x39313135, 0x30323135, 0x31323135, 0x32323135, 0x33323135, 0x34323135, 0x35323135, 0x36323135, 0x37323135, 0x38323135, 0x39323135, 0x30333135, 0x31333135, 0x32333135, 0x33333135, 0x34333135, 0x35333135, 0x36333135, 0x37333135, 0x38333135, 0x39333135, 0x30343135, 0x31343135, 0x32343135, 0x33343135, 0x34343135, 0x35343135, 0x36343135, 0x37343135, 0x38343135, 0x39343135, 0x30353135, 0x31353135, 0x32353135, 0x33353135, 0x34353135, 0x35353135, 0x36353135, 0x37353135, 0x38353135, 0x39353135, 0x30363135, 0x31363135, 0x32363135, 0x33363135, 0x34363135, 0x35363135, 0x36363135, 0x37363135, 0x38363135, 0x39363135, 0x30373135, 0x31373135, 0x32373135, 0x33373135, 0x34373135, 0x35373135, 0x36373135, 0x37373135, 0x38373135, 0x39373135, 0x30383135, 0x31383135, 0x32383135, 0x33383135, 0x34383135, 0x35383135, 0x36383135, 0x37383135, 0x38383135, 0x39383135, 0x30393135, 0x31393135, 0x32393135, 0x33393135, 0x34393135, 0x35393135, 0x36393135, 0x37393135, 0x38393135, 0x39393135, 0x30303235, 0x31303235, 0x32303235, 0x33303235, 0x34303235, 0x35303235, 0x36303235, 0x37303235, 0x38303235, 0x39303235, 0x30313235, 0x31313235, 0x32313235, 0x33313235, 0x34313235, 0x35313235, 0x36313235, 0x37313235, 0x38313235, 0x39313235, 0x30323235, 0x31323235, 0x32323235, 0x33323235, 0x34323235, 0x35323235, 0x36323235, 0x37323235, 0x38323235, 0x39323235, 0x30333235, 0x31333235, 0x32333235, 0x33333235, 0x34333235, 0x35333235, 0x36333235, 0x37333235, 0x38333235, 0x39333235, 0x30343235, 0x31343235, 0x32343235, 0x33343235, 0x34343235, 0x35343235, 0x36343235, 0x37343235, 0x38343235, 0x39343235, 0x30353235, 0x31353235, 0x32353235, 0x33353235, 0x34353235, 0x35353235, 0x36353235, 0x37353235, 0x38353235, 0x39353235, 0x30363235, 0x31363235, 0x32363235, 0x33363235, 0x34363235, 0x35363235, 0x36363235, 0x37363235, 0x38363235, 0x39363235, 0x30373235, 0x31373235, 0x32373235, 0x33373235, 0x34373235, 0x35373235, 0x36373235, 0x37373235, 0x38373235, 0x39373235, 0x30383235, 0x31383235, 0x32383235, 0x33383235, 0x34383235, 0x35383235, 0x36383235, 0x37383235, 0x38383235, 0x39383235, 0x30393235, 0x31393235, 0x32393235, 0x33393235, 0x34393235, 0x35393235, 0x36393235, 0x37393235, 0x38393235, 0x39393235, 0x30303335, 0x31303335, 0x32303335, 0x33303335, 0x34303335, 0x35303335, 0x36303335, 0x37303335, 0x38303335, 0x39303335, 0x30313335, 0x31313335, 0x32313335, 0x33313335, 0x34313335, 0x35313335, 0x36313335, 0x37313335, 0x38313335, 0x39313335, 0x30323335, 0x31323335, 0x32323335, 0x33323335, 0x34323335, 0x35323335, 0x36323335, 0x37323335, 0x38323335, 0x39323335, 0x30333335, 0x31333335, 0x32333335, 0x33333335, 0x34333335, 0x35333335, 0x36333335, 0x37333335, 0x38333335, 0x39333335, 0x30343335, 0x31343335, 0x32343335, 0x33343335, 0x34343335, 0x35343335, 0x36343335, 0x37343335, 0x38343335, 0x39343335, 0x30353335, 0x31353335, 0x32353335, 0x33353335, 0x34353335, 0x35353335, 0x36353335, 0x37353335, 0x38353335, 0x39353335, 0x30363335, 0x31363335, 0x32363335, 0x33363335, 0x34363335, 0x35363335, 0x36363335, 0x37363335, 0x38363335, 0x39363335, 0x30373335, 0x31373335, 0x32373335, 0x33373335, 0x34373335, 0x35373335, 0x36373335, 0x37373335, 0x38373335, 0x39373335, 0x30383335, 0x31383335, 0x32383335, 0x33383335, 0x34383335, 0x35383335, 0x36383335, 0x37383335, 0x38383335, 0x39383335, 0x30393335, 0x31393335, 0x32393335, 0x33393335, 0x34393335, 0x35393335, 0x36393335, 0x37393335, 0x38393335, 0x39393335, 0x30303435, 0x31303435, 0x32303435, 0x33303435, 0x34303435, 0x35303435, 0x36303435, 0x37303435, 0x38303435, 0x39303435, 0x30313435, 0x31313435, 0x32313435, 0x33313435, 0x34313435, 0x35313435, 0x36313435, 0x37313435, 0x38313435, 0x39313435, 0x30323435, 0x31323435, 0x32323435, 0x33323435, 0x34323435, 0x35323435, 0x36323435, 0x37323435, 0x38323435, 0x39323435, 0x30333435, 0x31333435, 0x32333435, 0x33333435, 0x34333435, 0x35333435, 0x36333435, 0x37333435, 0x38333435, 0x39333435, 0x30343435, 0x31343435, 0x32343435, 0x33343435, 0x34343435, 0x35343435, 0x36343435, 0x37343435, 0x38343435, 0x39343435, 0x30353435, 0x31353435, 0x32353435, 0x33353435, 0x34353435, 0x35353435, 0x36353435, 0x37353435, 0x38353435, 0x39353435, 0x30363435, 0x31363435, 0x32363435, 0x33363435, 0x34363435, 0x35363435, 0x36363435, 0x37363435, 0x38363435, 0x39363435, 0x30373435, 0x31373435, 0x32373435, 0x33373435, 0x34373435, 0x35373435, 0x36373435, 0x37373435, 0x38373435, 0x39373435, 0x30383435, 0x31383435, 0x32383435, 0x33383435, 0x34383435, 0x35383435, 0x36383435, 0x37383435, 0x38383435, 0x39383435, 0x30393435, 0x31393435, 0x32393435, 0x33393435, 0x34393435, 0x35393435, 0x36393435, 0x37393435, 0x38393435, 0x39393435, 0x30303535, 0x31303535, 0x32303535, 0x33303535, 0x34303535, 0x35303535, 0x36303535, 0x37303535, 0x38303535, 0x39303535, 0x30313535, 0x31313535, 0x32313535, 0x33313535, 0x34313535, 0x35313535, 0x36313535, 0x37313535, 0x38313535, 0x39313535, 0x30323535, 0x31323535, 0x32323535, 0x33323535, 0x34323535, 0x35323535, 0x36323535, 0x37323535, 0x38323535, 0x39323535, 0x30333535, 0x31333535, 0x32333535, 0x33333535, 0x34333535, 0x35333535, 0x36333535, 0x37333535, 0x38333535, 0x39333535, 0x30343535, 0x31343535, 0x32343535, 0x33343535, 0x34343535, 0x35343535, 0x36343535, 0x37343535, 0x38343535, 0x39343535, 0x30353535, 0x31353535, 0x32353535, 0x33353535, 0x34353535, 0x35353535, 0x36353535, 0x37353535, 0x38353535, 0x39353535, 0x30363535, 0x31363535, 0x32363535, 0x33363535, 0x34363535, 0x35363535, 0x36363535, 0x37363535, 0x38363535, 0x39363535, 0x30373535, 0x31373535, 0x32373535, 0x33373535, 0x34373535, 0x35373535, 0x36373535, 0x37373535, 0x38373535, 0x39373535, 0x30383535, 0x31383535, 0x32383535, 0x33383535, 0x34383535, 0x35383535, 0x36383535, 0x37383535, 0x38383535, 0x39383535, 0x30393535, 0x31393535, 0x32393535, 0x33393535, 0x34393535, 0x35393535, 0x36393535, 0x37393535, 0x38393535, 0x39393535, 0x30303635, 0x31303635, 0x32303635, 0x33303635, 0x34303635, 0x35303635, 0x36303635, 0x37303635, 0x38303635, 0x39303635, 0x30313635, 0x31313635, 0x32313635, 0x33313635, 0x34313635, 0x35313635, 0x36313635, 0x37313635, 0x38313635, 0x39313635, 0x30323635, 0x31323635, 0x32323635, 0x33323635, 0x34323635, 0x35323635, 0x36323635, 0x37323635, 0x38323635, 0x39323635, 0x30333635, 0x31333635, 0x32333635, 0x33333635, 0x34333635, 0x35333635, 0x36333635, 0x37333635, 0x38333635, 0x39333635, 0x30343635, 0x31343635, 0x32343635, 0x33343635, 0x34343635, 0x35343635, 0x36343635, 0x37343635, 0x38343635, 0x39343635, 0x30353635, 0x31353635, 0x32353635, 0x33353635, 0x34353635, 0x35353635, 0x36353635, 0x37353635, 0x38353635, 0x39353635, 0x30363635, 0x31363635, 0x32363635, 0x33363635, 0x34363635, 0x35363635, 0x36363635, 0x37363635, 0x38363635, 0x39363635, 0x30373635, 0x31373635, 0x32373635, 0x33373635, 0x34373635, 0x35373635, 0x36373635, 0x37373635, 0x38373635, 0x39373635, 0x30383635, 0x31383635, 0x32383635, 0x33383635, 0x34383635, 0x35383635, 0x36383635, 0x37383635, 0x38383635, 0x39383635, 0x30393635, 0x31393635, 0x32393635, 0x33393635, 0x34393635, 0x35393635, 0x36393635, 0x37393635, 0x38393635, 0x39393635, 0x30303735, 0x31303735, 0x32303735, 0x33303735, 0x34303735, 0x35303735, 0x36303735, 0x37303735, 0x38303735, 0x39303735, 0x30313735, 0x31313735, 0x32313735, 0x33313735, 0x34313735, 0x35313735, 0x36313735, 0x37313735, 0x38313735, 0x39313735, 0x30323735, 0x31323735, 0x32323735, 0x33323735, 0x34323735, 0x35323735, 0x36323735, 0x37323735, 0x38323735, 0x39323735, 0x30333735, 0x31333735, 0x32333735, 0x33333735, 0x34333735, 0x35333735, 0x36333735, 0x37333735, 0x38333735, 0x39333735, 0x30343735, 0x31343735, 0x32343735, 0x33343735, 0x34343735, 0x35343735, 0x36343735, 0x37343735, 0x38343735, 0x39343735, 0x30353735, 0x31353735, 0x32353735, 0x33353735, 0x34353735, 0x35353735, 0x36353735, 0x37353735, 0x38353735, 0x39353735, 0x30363735, 0x31363735, 0x32363735, 0x33363735, 0x34363735, 0x35363735, 0x36363735, 0x37363735, 0x38363735, 0x39363735, 0x30373735, 0x31373735, 0x32373735, 0x33373735, 0x34373735, 0x35373735, 0x36373735, 0x37373735, 0x38373735, 0x39373735, 0x30383735, 0x31383735, 0x32383735, 0x33383735, 0x34383735, 0x35383735, 0x36383735, 0x37383735, 0x38383735, 0x39383735, 0x30393735, 0x31393735, 0x32393735, 0x33393735, 0x34393735, 0x35393735, 0x36393735, 0x37393735, 0x38393735, 0x39393735, 0x30303835, 0x31303835, 0x32303835, 0x33303835, 0x34303835, 0x35303835, 0x36303835, 0x37303835, 0x38303835, 0x39303835, 0x30313835, 0x31313835, 0x32313835, 0x33313835, 0x34313835, 0x35313835, 0x36313835, 0x37313835, 0x38313835, 0x39313835, 0x30323835, 0x31323835, 0x32323835, 0x33323835, 0x34323835, 0x35323835, 0x36323835, 0x37323835, 0x38323835, 0x39323835, 0x30333835, 0x31333835, 0x32333835, 0x33333835, 0x34333835, 0x35333835, 0x36333835, 0x37333835, 0x38333835, 0x39333835, 0x30343835, 0x31343835, 0x32343835, 0x33343835, 0x34343835, 0x35343835, 0x36343835, 0x37343835, 0x38343835, 0x39343835, 0x30353835, 0x31353835, 0x32353835, 0x33353835, 0x34353835, 0x35353835, 0x36353835, 0x37353835, 0x38353835, 0x39353835, 0x30363835, 0x31363835, 0x32363835, 0x33363835, 0x34363835, 0x35363835, 0x36363835, 0x37363835, 0x38363835, 0x39363835, 0x30373835, 0x31373835, 0x32373835, 0x33373835, 0x34373835, 0x35373835, 0x36373835, 0x37373835, 0x38373835, 0x39373835, 0x30383835, 0x31383835, 0x32383835, 0x33383835, 0x34383835, 0x35383835, 0x36383835, 0x37383835, 0x38383835, 0x39383835, 0x30393835, 0x31393835, 0x32393835, 0x33393835, 0x34393835, 0x35393835, 0x36393835, 0x37393835, 0x38393835, 0x39393835, 0x30303935, 0x31303935, 0x32303935, 0x33303935, 0x34303935, 0x35303935, 0x36303935, 0x37303935, 0x38303935, 0x39303935, 0x30313935, 0x31313935, 0x32313935, 0x33313935, 0x34313935, 0x35313935, 0x36313935, 0x37313935, 0x38313935, 0x39313935, 0x30323935, 0x31323935, 0x32323935, 0x33323935, 0x34323935, 0x35323935, 0x36323935, 0x37323935, 0x38323935, 0x39323935, 0x30333935, 0x31333935, 0x32333935, 0x33333935, 0x34333935, 0x35333935, 0x36333935, 0x37333935, 0x38333935, 0x39333935, 0x30343935, 0x31343935, 0x32343935, 0x33343935, 0x34343935, 0x35343935, 0x36343935, 0x37343935, 0x38343935, 0x39343935, 0x30353935, 0x31353935, 0x32353935, 0x33353935, 0x34353935, 0x35353935, 0x36353935, 0x37353935, 0x38353935, 0x39353935, 0x30363935, 0x31363935, 0x32363935, 0x33363935, 0x34363935, 0x35363935, 0x36363935, 0x37363935, 0x38363935, 0x39363935, 0x30373935, 0x31373935, 0x32373935, 0x33373935, 0x34373935, 0x35373935, 0x36373935, 0x37373935, 0x38373935, 0x39373935, 0x30383935, 0x31383935, 0x32383935, 0x33383935, 0x34383935, 0x35383935, 0x36383935, 0x37383935, 0x38383935, 0x39383935, 0x30393935, 0x31393935, 0x32393935, 0x33393935, 0x34393935, 0x35393935, 0x36393935, 0x37393935, 0x38393935, 0x39393935, 0x30303036, 0x31303036, 0x32303036, 0x33303036, 0x34303036, 0x35303036, 0x36303036, 0x37303036, 0x38303036, 0x39303036, 0x30313036, 0x31313036, 0x32313036, 0x33313036, 0x34313036, 0x35313036, 0x36313036, 0x37313036, 0x38313036, 0x39313036, 0x30323036, 0x31323036, 0x32323036, 0x33323036, 0x34323036, 0x35323036, 0x36323036, 0x37323036, 0x38323036, 0x39323036, 0x30333036, 0x31333036, 0x32333036, 0x33333036, 0x34333036, 0x35333036, 0x36333036, 0x37333036, 0x38333036, 0x39333036, 0x30343036, 0x31343036, 0x32343036, 0x33343036, 0x34343036, 0x35343036, 0x36343036, 0x37343036, 0x38343036, 0x39343036, 0x30353036, 0x31353036, 0x32353036, 0x33353036, 0x34353036, 0x35353036, 0x36353036, 0x37353036, 0x38353036, 0x39353036, 0x30363036, 0x31363036, 0x32363036, 0x33363036, 0x34363036, 0x35363036, 0x36363036, 0x37363036, 0x38363036, 0x39363036, 0x30373036, 0x31373036, 0x32373036, 0x33373036, 0x34373036, 0x35373036, 0x36373036, 0x37373036, 0x38373036, 0x39373036, 0x30383036, 0x31383036, 0x32383036, 0x33383036, 0x34383036, 0x35383036, 0x36383036, 0x37383036, 0x38383036, 0x39383036, 0x30393036, 0x31393036, 0x32393036, 0x33393036, 0x34393036, 0x35393036, 0x36393036, 0x37393036, 0x38393036, 0x39393036, 0x30303136, 0x31303136, 0x32303136, 0x33303136, 0x34303136, 0x35303136, 0x36303136, 0x37303136, 0x38303136, 0x39303136, 0x30313136, 0x31313136, 0x32313136, 0x33313136, 0x34313136, 0x35313136, 0x36313136, 0x37313136, 0x38313136, 0x39313136, 0x30323136, 0x31323136, 0x32323136, 0x33323136, 0x34323136, 0x35323136, 0x36323136, 0x37323136, 0x38323136, 0x39323136, 0x30333136, 0x31333136, 0x32333136, 0x33333136, 0x34333136, 0x35333136, 0x36333136, 0x37333136, 0x38333136, 0x39333136, 0x30343136, 0x31343136, 0x32343136, 0x33343136, 0x34343136, 0x35343136, 0x36343136, 0x37343136, 0x38343136, 0x39343136, 0x30353136, 0x31353136, 0x32353136, 0x33353136, 0x34353136, 0x35353136, 0x36353136, 0x37353136, 0x38353136, 0x39353136, 0x30363136, 0x31363136, 0x32363136, 0x33363136, 0x34363136, 0x35363136, 0x36363136, 0x37363136, 0x38363136, 0x39363136, 0x30373136, 0x31373136, 0x32373136, 0x33373136, 0x34373136, 0x35373136, 0x36373136, 0x37373136, 0x38373136, 0x39373136, 0x30383136, 0x31383136, 0x32383136, 0x33383136, 0x34383136, 0x35383136, 0x36383136, 0x37383136, 0x38383136, 0x39383136, 0x30393136, 0x31393136, 0x32393136, 0x33393136, 0x34393136, 0x35393136, 0x36393136, 0x37393136, 0x38393136, 0x39393136, 0x30303236, 0x31303236, 0x32303236, 0x33303236, 0x34303236, 0x35303236, 0x36303236, 0x37303236, 0x38303236, 0x39303236, 0x30313236, 0x31313236, 0x32313236, 0x33313236, 0x34313236, 0x35313236, 0x36313236, 0x37313236, 0x38313236, 0x39313236, 0x30323236, 0x31323236, 0x32323236, 0x33323236, 0x34323236, 0x35323236, 0x36323236, 0x37323236, 0x38323236, 0x39323236, 0x30333236, 0x31333236, 0x32333236, 0x33333236, 0x34333236, 0x35333236, 0x36333236, 0x37333236, 0x38333236, 0x39333236, 0x30343236, 0x31343236, 0x32343236, 0x33343236, 0x34343236, 0x35343236, 0x36343236, 0x37343236, 0x38343236, 0x39343236, 0x30353236, 0x31353236, 0x32353236, 0x33353236, 0x34353236, 0x35353236, 0x36353236, 0x37353236, 0x38353236, 0x39353236, 0x30363236, 0x31363236, 0x32363236, 0x33363236, 0x34363236, 0x35363236, 0x36363236, 0x37363236, 0x38363236, 0x39363236, 0x30373236, 0x31373236, 0x32373236, 0x33373236, 0x34373236, 0x35373236, 0x36373236, 0x37373236, 0x38373236, 0x39373236, 0x30383236, 0x31383236, 0x32383236, 0x33383236, 0x34383236, 0x35383236, 0x36383236, 0x37383236, 0x38383236, 0x39383236, 0x30393236, 0x31393236, 0x32393236, 0x33393236, 0x34393236, 0x35393236, 0x36393236, 0x37393236, 0x38393236, 0x39393236, 0x30303336, 0x31303336, 0x32303336, 0x33303336, 0x34303336, 0x35303336, 0x36303336, 0x37303336, 0x38303336, 0x39303336, 0x30313336, 0x31313336, 0x32313336, 0x33313336, 0x34313336, 0x35313336, 0x36313336, 0x37313336, 0x38313336, 0x39313336, 0x30323336, 0x31323336, 0x32323336, 0x33323336, 0x34323336, 0x35323336, 0x36323336, 0x37323336, 0x38323336, 0x39323336, 0x30333336, 0x31333336, 0x32333336, 0x33333336, 0x34333336, 0x35333336, 0x36333336, 0x37333336, 0x38333336, 0x39333336, 0x30343336, 0x31343336, 0x32343336, 0x33343336, 0x34343336, 0x35343336, 0x36343336, 0x37343336, 0x38343336, 0x39343336, 0x30353336, 0x31353336, 0x32353336, 0x33353336, 0x34353336, 0x35353336, 0x36353336, 0x37353336, 0x38353336, 0x39353336, 0x30363336, 0x31363336, 0x32363336, 0x33363336, 0x34363336, 0x35363336, 0x36363336, 0x37363336, 0x38363336, 0x39363336, 0x30373336, 0x31373336, 0x32373336, 0x33373336, 0x34373336, 0x35373336, 0x36373336, 0x37373336, 0x38373336, 0x39373336, 0x30383336, 0x31383336, 0x32383336, 0x33383336, 0x34383336, 0x35383336, 0x36383336, 0x37383336, 0x38383336, 0x39383336, 0x30393336, 0x31393336, 0x32393336, 0x33393336, 0x34393336, 0x35393336, 0x36393336, 0x37393336, 0x38393336, 0x39393336, 0x30303436, 0x31303436, 0x32303436, 0x33303436, 0x34303436, 0x35303436, 0x36303436, 0x37303436, 0x38303436, 0x39303436, 0x30313436, 0x31313436, 0x32313436, 0x33313436, 0x34313436, 0x35313436, 0x36313436, 0x37313436, 0x38313436, 0x39313436, 0x30323436, 0x31323436, 0x32323436, 0x33323436, 0x34323436, 0x35323436, 0x36323436, 0x37323436, 0x38323436, 0x39323436, 0x30333436, 0x31333436, 0x32333436, 0x33333436, 0x34333436, 0x35333436, 0x36333436, 0x37333436, 0x38333436, 0x39333436, 0x30343436, 0x31343436, 0x32343436, 0x33343436, 0x34343436, 0x35343436, 0x36343436, 0x37343436, 0x38343436, 0x39343436, 0x30353436, 0x31353436, 0x32353436, 0x33353436, 0x34353436, 0x35353436, 0x36353436, 0x37353436, 0x38353436, 0x39353436, 0x30363436, 0x31363436, 0x32363436, 0x33363436, 0x34363436, 0x35363436, 0x36363436, 0x37363436, 0x38363436, 0x39363436, 0x30373436, 0x31373436, 0x32373436, 0x33373436, 0x34373436, 0x35373436, 0x36373436, 0x37373436, 0x38373436, 0x39373436, 0x30383436, 0x31383436, 0x32383436, 0x33383436, 0x34383436, 0x35383436, 0x36383436, 0x37383436, 0x38383436, 0x39383436, 0x30393436, 0x31393436, 0x32393436, 0x33393436, 0x34393436, 0x35393436, 0x36393436, 0x37393436, 0x38393436, 0x39393436, 0x30303536, 0x31303536, 0x32303536, 0x33303536, 0x34303536, 0x35303536, 0x36303536, 0x37303536, 0x38303536, 0x39303536, 0x30313536, 0x31313536, 0x32313536, 0x33313536, 0x34313536, 0x35313536, 0x36313536, 0x37313536, 0x38313536, 0x39313536, 0x30323536, 0x31323536, 0x32323536, 0x33323536, 0x34323536, 0x35323536, 0x36323536, 0x37323536, 0x38323536, 0x39323536, 0x30333536, 0x31333536, 0x32333536, 0x33333536, 0x34333536, 0x35333536, 0x36333536, 0x37333536, 0x38333536, 0x39333536, 0x30343536, 0x31343536, 0x32343536, 0x33343536, 0x34343536, 0x35343536, 0x36343536, 0x37343536, 0x38343536, 0x39343536, 0x30353536, 0x31353536, 0x32353536, 0x33353536, 0x34353536, 0x35353536, 0x36353536, 0x37353536, 0x38353536, 0x39353536, 0x30363536, 0x31363536, 0x32363536, 0x33363536, 0x34363536, 0x35363536, 0x36363536, 0x37363536, 0x38363536, 0x39363536, 0x30373536, 0x31373536, 0x32373536, 0x33373536, 0x34373536, 0x35373536, 0x36373536, 0x37373536, 0x38373536, 0x39373536, 0x30383536, 0x31383536, 0x32383536, 0x33383536, 0x34383536, 0x35383536, 0x36383536, 0x37383536, 0x38383536, 0x39383536, 0x30393536, 0x31393536, 0x32393536, 0x33393536, 0x34393536, 0x35393536, 0x36393536, 0x37393536, 0x38393536, 0x39393536, 0x30303636, 0x31303636, 0x32303636, 0x33303636, 0x34303636, 0x35303636, 0x36303636, 0x37303636, 0x38303636, 0x39303636, 0x30313636, 0x31313636, 0x32313636, 0x33313636, 0x34313636, 0x35313636, 0x36313636, 0x37313636, 0x38313636, 0x39313636, 0x30323636, 0x31323636, 0x32323636, 0x33323636, 0x34323636, 0x35323636, 0x36323636, 0x37323636, 0x38323636, 0x39323636, 0x30333636, 0x31333636, 0x32333636, 0x33333636, 0x34333636, 0x35333636, 0x36333636, 0x37333636, 0x38333636, 0x39333636, 0x30343636, 0x31343636, 0x32343636, 0x33343636, 0x34343636, 0x35343636, 0x36343636, 0x37343636, 0x38343636, 0x39343636, 0x30353636, 0x31353636, 0x32353636, 0x33353636, 0x34353636, 0x35353636, 0x36353636, 0x37353636, 0x38353636, 0x39353636, 0x30363636, 0x31363636, 0x32363636, 0x33363636, 0x34363636, 0x35363636, 0x36363636, 0x37363636, 0x38363636, 0x39363636, 0x30373636, 0x31373636, 0x32373636, 0x33373636, 0x34373636, 0x35373636, 0x36373636, 0x37373636, 0x38373636, 0x39373636, 0x30383636, 0x31383636, 0x32383636, 0x33383636, 0x34383636, 0x35383636, 0x36383636, 0x37383636, 0x38383636, 0x39383636, 0x30393636, 0x31393636, 0x32393636, 0x33393636, 0x34393636, 0x35393636, 0x36393636, 0x37393636, 0x38393636, 0x39393636, 0x30303736, 0x31303736, 0x32303736, 0x33303736, 0x34303736, 0x35303736, 0x36303736, 0x37303736, 0x38303736, 0x39303736, 0x30313736, 0x31313736, 0x32313736, 0x33313736, 0x34313736, 0x35313736, 0x36313736, 0x37313736, 0x38313736, 0x39313736, 0x30323736, 0x31323736, 0x32323736, 0x33323736, 0x34323736, 0x35323736, 0x36323736, 0x37323736, 0x38323736, 0x39323736, 0x30333736, 0x31333736, 0x32333736, 0x33333736, 0x34333736, 0x35333736, 0x36333736, 0x37333736, 0x38333736, 0x39333736, 0x30343736, 0x31343736, 0x32343736, 0x33343736, 0x34343736, 0x35343736, 0x36343736, 0x37343736, 0x38343736, 0x39343736, 0x30353736, 0x31353736, 0x32353736, 0x33353736, 0x34353736, 0x35353736, 0x36353736, 0x37353736, 0x38353736, 0x39353736, 0x30363736, 0x31363736, 0x32363736, 0x33363736, 0x34363736, 0x35363736, 0x36363736, 0x37363736, 0x38363736, 0x39363736, 0x30373736, 0x31373736, 0x32373736, 0x33373736, 0x34373736, 0x35373736, 0x36373736, 0x37373736, 0x38373736, 0x39373736, 0x30383736, 0x31383736, 0x32383736, 0x33383736, 0x34383736, 0x35383736, 0x36383736, 0x37383736, 0x38383736, 0x39383736, 0x30393736, 0x31393736, 0x32393736, 0x33393736, 0x34393736, 0x35393736, 0x36393736, 0x37393736, 0x38393736, 0x39393736, 0x30303836, 0x31303836, 0x32303836, 0x33303836, 0x34303836, 0x35303836, 0x36303836, 0x37303836, 0x38303836, 0x39303836, 0x30313836, 0x31313836, 0x32313836, 0x33313836, 0x34313836, 0x35313836, 0x36313836, 0x37313836, 0x38313836, 0x39313836, 0x30323836, 0x31323836, 0x32323836, 0x33323836, 0x34323836, 0x35323836, 0x36323836, 0x37323836, 0x38323836, 0x39323836, 0x30333836, 0x31333836, 0x32333836, 0x33333836, 0x34333836, 0x35333836, 0x36333836, 0x37333836, 0x38333836, 0x39333836, 0x30343836, 0x31343836, 0x32343836, 0x33343836, 0x34343836, 0x35343836, 0x36343836, 0x37343836, 0x38343836, 0x39343836, 0x30353836, 0x31353836, 0x32353836, 0x33353836, 0x34353836, 0x35353836, 0x36353836, 0x37353836, 0x38353836, 0x39353836, 0x30363836, 0x31363836, 0x32363836, 0x33363836, 0x34363836, 0x35363836, 0x36363836, 0x37363836, 0x38363836, 0x39363836, 0x30373836, 0x31373836, 0x32373836, 0x33373836, 0x34373836, 0x35373836, 0x36373836, 0x37373836, 0x38373836, 0x39373836, 0x30383836, 0x31383836, 0x32383836, 0x33383836, 0x34383836, 0x35383836, 0x36383836, 0x37383836, 0x38383836, 0x39383836, 0x30393836, 0x31393836, 0x32393836, 0x33393836, 0x34393836, 0x35393836, 0x36393836, 0x37393836, 0x38393836, 0x39393836, 0x30303936, 0x31303936, 0x32303936, 0x33303936, 0x34303936, 0x35303936, 0x36303936, 0x37303936, 0x38303936, 0x39303936, 0x30313936, 0x31313936, 0x32313936, 0x33313936, 0x34313936, 0x35313936, 0x36313936, 0x37313936, 0x38313936, 0x39313936, 0x30323936, 0x31323936, 0x32323936, 0x33323936, 0x34323936, 0x35323936, 0x36323936, 0x37323936, 0x38323936, 0x39323936, 0x30333936, 0x31333936, 0x32333936, 0x33333936, 0x34333936, 0x35333936, 0x36333936, 0x37333936, 0x38333936, 0x39333936, 0x30343936, 0x31343936, 0x32343936, 0x33343936, 0x34343936, 0x35343936, 0x36343936, 0x37343936, 0x38343936, 0x39343936, 0x30353936, 0x31353936, 0x32353936, 0x33353936, 0x34353936, 0x35353936, 0x36353936, 0x37353936, 0x38353936, 0x39353936, 0x30363936, 0x31363936, 0x32363936, 0x33363936, 0x34363936, 0x35363936, 0x36363936, 0x37363936, 0x38363936, 0x39363936, 0x30373936, 0x31373936, 0x32373936, 0x33373936, 0x34373936, 0x35373936, 0x36373936, 0x37373936, 0x38373936, 0x39373936, 0x30383936, 0x31383936, 0x32383936, 0x33383936, 0x34383936, 0x35383936, 0x36383936, 0x37383936, 0x38383936, 0x39383936, 0x30393936, 0x31393936, 0x32393936, 0x33393936, 0x34393936, 0x35393936, 0x36393936, 0x37393936, 0x38393936, 0x39393936, 0x30303037, 0x31303037, 0x32303037, 0x33303037, 0x34303037, 0x35303037, 0x36303037, 0x37303037, 0x38303037, 0x39303037, 0x30313037, 0x31313037, 0x32313037, 0x33313037, 0x34313037, 0x35313037, 0x36313037, 0x37313037, 0x38313037, 0x39313037, 0x30323037, 0x31323037, 0x32323037, 0x33323037, 0x34323037, 0x35323037, 0x36323037, 0x37323037, 0x38323037, 0x39323037, 0x30333037, 0x31333037, 0x32333037, 0x33333037, 0x34333037, 0x35333037, 0x36333037, 0x37333037, 0x38333037, 0x39333037, 0x30343037, 0x31343037, 0x32343037, 0x33343037, 0x34343037, 0x35343037, 0x36343037, 0x37343037, 0x38343037, 0x39343037, 0x30353037, 0x31353037, 0x32353037, 0x33353037, 0x34353037, 0x35353037, 0x36353037, 0x37353037, 0x38353037, 0x39353037, 0x30363037, 0x31363037, 0x32363037, 0x33363037, 0x34363037, 0x35363037, 0x36363037, 0x37363037, 0x38363037, 0x39363037, 0x30373037, 0x31373037, 0x32373037, 0x33373037, 0x34373037, 0x35373037, 0x36373037, 0x37373037, 0x38373037, 0x39373037, 0x30383037, 0x31383037, 0x32383037, 0x33383037, 0x34383037, 0x35383037, 0x36383037, 0x37383037, 0x38383037, 0x39383037, 0x30393037, 0x31393037, 0x32393037, 0x33393037, 0x34393037, 0x35393037, 0x36393037, 0x37393037, 0x38393037, 0x39393037, 0x30303137, 0x31303137, 0x32303137, 0x33303137, 0x34303137, 0x35303137, 0x36303137, 0x37303137, 0x38303137, 0x39303137, 0x30313137, 0x31313137, 0x32313137, 0x33313137, 0x34313137, 0x35313137, 0x36313137, 0x37313137, 0x38313137, 0x39313137, 0x30323137, 0x31323137, 0x32323137, 0x33323137, 0x34323137, 0x35323137, 0x36323137, 0x37323137, 0x38323137, 0x39323137, 0x30333137, 0x31333137, 0x32333137, 0x33333137, 0x34333137, 0x35333137, 0x36333137, 0x37333137, 0x38333137, 0x39333137, 0x30343137, 0x31343137, 0x32343137, 0x33343137, 0x34343137, 0x35343137, 0x36343137, 0x37343137, 0x38343137, 0x39343137, 0x30353137, 0x31353137, 0x32353137, 0x33353137, 0x34353137, 0x35353137, 0x36353137, 0x37353137, 0x38353137, 0x39353137, 0x30363137, 0x31363137, 0x32363137, 0x33363137, 0x34363137, 0x35363137, 0x36363137, 0x37363137, 0x38363137, 0x39363137, 0x30373137, 0x31373137, 0x32373137, 0x33373137, 0x34373137, 0x35373137, 0x36373137, 0x37373137, 0x38373137, 0x39373137, 0x30383137, 0x31383137, 0x32383137, 0x33383137, 0x34383137, 0x35383137, 0x36383137, 0x37383137, 0x38383137, 0x39383137, 0x30393137, 0x31393137, 0x32393137, 0x33393137, 0x34393137, 0x35393137, 0x36393137, 0x37393137, 0x38393137, 0x39393137, 0x30303237, 0x31303237, 0x32303237, 0x33303237, 0x34303237, 0x35303237, 0x36303237, 0x37303237, 0x38303237, 0x39303237, 0x30313237, 0x31313237, 0x32313237, 0x33313237, 0x34313237, 0x35313237, 0x36313237, 0x37313237, 0x38313237, 0x39313237, 0x30323237, 0x31323237, 0x32323237, 0x33323237, 0x34323237, 0x35323237, 0x36323237, 0x37323237, 0x38323237, 0x39323237, 0x30333237, 0x31333237, 0x32333237, 0x33333237, 0x34333237, 0x35333237, 0x36333237, 0x37333237, 0x38333237, 0x39333237, 0x30343237, 0x31343237, 0x32343237, 0x33343237, 0x34343237, 0x35343237, 0x36343237, 0x37343237, 0x38343237, 0x39343237, 0x30353237, 0x31353237, 0x32353237, 0x33353237, 0x34353237, 0x35353237, 0x36353237, 0x37353237, 0x38353237, 0x39353237, 0x30363237, 0x31363237, 0x32363237, 0x33363237, 0x34363237, 0x35363237, 0x36363237, 0x37363237, 0x38363237, 0x39363237, 0x30373237, 0x31373237, 0x32373237, 0x33373237, 0x34373237, 0x35373237, 0x36373237, 0x37373237, 0x38373237, 0x39373237, 0x30383237, 0x31383237, 0x32383237, 0x33383237, 0x34383237, 0x35383237, 0x36383237, 0x37383237, 0x38383237, 0x39383237, 0x30393237, 0x31393237, 0x32393237, 0x33393237, 0x34393237, 0x35393237, 0x36393237, 0x37393237, 0x38393237, 0x39393237, 0x30303337, 0x31303337, 0x32303337, 0x33303337, 0x34303337, 0x35303337, 0x36303337, 0x37303337, 0x38303337, 0x39303337, 0x30313337, 0x31313337, 0x32313337, 0x33313337, 0x34313337, 0x35313337, 0x36313337, 0x37313337, 0x38313337, 0x39313337, 0x30323337, 0x31323337, 0x32323337, 0x33323337, 0x34323337, 0x35323337, 0x36323337, 0x37323337, 0x38323337, 0x39323337, 0x30333337, 0x31333337, 0x32333337, 0x33333337, 0x34333337, 0x35333337, 0x36333337, 0x37333337, 0x38333337, 0x39333337, 0x30343337, 0x31343337, 0x32343337, 0x33343337, 0x34343337, 0x35343337, 0x36343337, 0x37343337, 0x38343337, 0x39343337, 0x30353337, 0x31353337, 0x32353337, 0x33353337, 0x34353337, 0x35353337, 0x36353337, 0x37353337, 0x38353337, 0x39353337, 0x30363337, 0x31363337, 0x32363337, 0x33363337, 0x34363337, 0x35363337, 0x36363337, 0x37363337, 0x38363337, 0x39363337, 0x30373337, 0x31373337, 0x32373337, 0x33373337, 0x34373337, 0x35373337, 0x36373337, 0x37373337, 0x38373337, 0x39373337, 0x30383337, 0x31383337, 0x32383337, 0x33383337, 0x34383337, 0x35383337, 0x36383337, 0x37383337, 0x38383337, 0x39383337, 0x30393337, 0x31393337, 0x32393337, 0x33393337, 0x34393337, 0x35393337, 0x36393337, 0x37393337, 0x38393337, 0x39393337, 0x30303437, 0x31303437, 0x32303437, 0x33303437, 0x34303437, 0x35303437, 0x36303437, 0x37303437, 0x38303437, 0x39303437, 0x30313437, 0x31313437, 0x32313437, 0x33313437, 0x34313437, 0x35313437, 0x36313437, 0x37313437, 0x38313437, 0x39313437, 0x30323437, 0x31323437, 0x32323437, 0x33323437, 0x34323437, 0x35323437, 0x36323437, 0x37323437, 0x38323437, 0x39323437, 0x30333437, 0x31333437, 0x32333437, 0x33333437, 0x34333437, 0x35333437, 0x36333437, 0x37333437, 0x38333437, 0x39333437, 0x30343437, 0x31343437, 0x32343437, 0x33343437, 0x34343437, 0x35343437, 0x36343437, 0x37343437, 0x38343437, 0x39343437, 0x30353437, 0x31353437, 0x32353437, 0x33353437, 0x34353437, 0x35353437, 0x36353437, 0x37353437, 0x38353437, 0x39353437, 0x30363437, 0x31363437, 0x32363437, 0x33363437, 0x34363437, 0x35363437, 0x36363437, 0x37363437, 0x38363437, 0x39363437, 0x30373437, 0x31373437, 0x32373437, 0x33373437, 0x34373437, 0x35373437, 0x36373437, 0x37373437, 0x38373437, 0x39373437, 0x30383437, 0x31383437, 0x32383437, 0x33383437, 0x34383437, 0x35383437, 0x36383437, 0x37383437, 0x38383437, 0x39383437, 0x30393437, 0x31393437, 0x32393437, 0x33393437, 0x34393437, 0x35393437, 0x36393437, 0x37393437, 0x38393437, 0x39393437, 0x30303537, 0x31303537, 0x32303537, 0x33303537, 0x34303537, 0x35303537, 0x36303537, 0x37303537, 0x38303537, 0x39303537, 0x30313537, 0x31313537, 0x32313537, 0x33313537, 0x34313537, 0x35313537, 0x36313537, 0x37313537, 0x38313537, 0x39313537, 0x30323537, 0x31323537, 0x32323537, 0x33323537, 0x34323537, 0x35323537, 0x36323537, 0x37323537, 0x38323537, 0x39323537, 0x30333537, 0x31333537, 0x32333537, 0x33333537, 0x34333537, 0x35333537, 0x36333537, 0x37333537, 0x38333537, 0x39333537, 0x30343537, 0x31343537, 0x32343537, 0x33343537, 0x34343537, 0x35343537, 0x36343537, 0x37343537, 0x38343537, 0x39343537, 0x30353537, 0x31353537, 0x32353537, 0x33353537, 0x34353537, 0x35353537, 0x36353537, 0x37353537, 0x38353537, 0x39353537, 0x30363537, 0x31363537, 0x32363537, 0x33363537, 0x34363537, 0x35363537, 0x36363537, 0x37363537, 0x38363537, 0x39363537, 0x30373537, 0x31373537, 0x32373537, 0x33373537, 0x34373537, 0x35373537, 0x36373537, 0x37373537, 0x38373537, 0x39373537, 0x30383537, 0x31383537, 0x32383537, 0x33383537, 0x34383537, 0x35383537, 0x36383537, 0x37383537, 0x38383537, 0x39383537, 0x30393537, 0x31393537, 0x32393537, 0x33393537, 0x34393537, 0x35393537, 0x36393537, 0x37393537, 0x38393537, 0x39393537, 0x30303637, 0x31303637, 0x32303637, 0x33303637, 0x34303637, 0x35303637, 0x36303637, 0x37303637, 0x38303637, 0x39303637, 0x30313637, 0x31313637, 0x32313637, 0x33313637, 0x34313637, 0x35313637, 0x36313637, 0x37313637, 0x38313637, 0x39313637, 0x30323637, 0x31323637, 0x32323637, 0x33323637, 0x34323637, 0x35323637, 0x36323637, 0x37323637, 0x38323637, 0x39323637, 0x30333637, 0x31333637, 0x32333637, 0x33333637, 0x34333637, 0x35333637, 0x36333637, 0x37333637, 0x38333637, 0x39333637, 0x30343637, 0x31343637, 0x32343637, 0x33343637, 0x34343637, 0x35343637, 0x36343637, 0x37343637, 0x38343637, 0x39343637, 0x30353637, 0x31353637, 0x32353637, 0x33353637, 0x34353637, 0x35353637, 0x36353637, 0x37353637, 0x38353637, 0x39353637, 0x30363637, 0x31363637, 0x32363637, 0x33363637, 0x34363637, 0x35363637, 0x36363637, 0x37363637, 0x38363637, 0x39363637, 0x30373637, 0x31373637, 0x32373637, 0x33373637, 0x34373637, 0x35373637, 0x36373637, 0x37373637, 0x38373637, 0x39373637, 0x30383637, 0x31383637, 0x32383637, 0x33383637, 0x34383637, 0x35383637, 0x36383637, 0x37383637, 0x38383637, 0x39383637, 0x30393637, 0x31393637, 0x32393637, 0x33393637, 0x34393637, 0x35393637, 0x36393637, 0x37393637, 0x38393637, 0x39393637, 0x30303737, 0x31303737, 0x32303737, 0x33303737, 0x34303737, 0x35303737, 0x36303737, 0x37303737, 0x38303737, 0x39303737, 0x30313737, 0x31313737, 0x32313737, 0x33313737, 0x34313737, 0x35313737, 0x36313737, 0x37313737, 0x38313737, 0x39313737, 0x30323737, 0x31323737, 0x32323737, 0x33323737, 0x34323737, 0x35323737, 0x36323737, 0x37323737, 0x38323737, 0x39323737, 0x30333737, 0x31333737, 0x32333737, 0x33333737, 0x34333737, 0x35333737, 0x36333737, 0x37333737, 0x38333737, 0x39333737, 0x30343737, 0x31343737, 0x32343737, 0x33343737, 0x34343737, 0x35343737, 0x36343737, 0x37343737, 0x38343737, 0x39343737, 0x30353737, 0x31353737, 0x32353737, 0x33353737, 0x34353737, 0x35353737, 0x36353737, 0x37353737, 0x38353737, 0x39353737, 0x30363737, 0x31363737, 0x32363737, 0x33363737, 0x34363737, 0x35363737, 0x36363737, 0x37363737, 0x38363737, 0x39363737, 0x30373737, 0x31373737, 0x32373737, 0x33373737, 0x34373737, 0x35373737, 0x36373737, 0x37373737, 0x38373737, 0x39373737, 0x30383737, 0x31383737, 0x32383737, 0x33383737, 0x34383737, 0x35383737, 0x36383737, 0x37383737, 0x38383737, 0x39383737, 0x30393737, 0x31393737, 0x32393737, 0x33393737, 0x34393737, 0x35393737, 0x36393737, 0x37393737, 0x38393737, 0x39393737, 0x30303837, 0x31303837, 0x32303837, 0x33303837, 0x34303837, 0x35303837, 0x36303837, 0x37303837, 0x38303837, 0x39303837, 0x30313837, 0x31313837, 0x32313837, 0x33313837, 0x34313837, 0x35313837, 0x36313837, 0x37313837, 0x38313837, 0x39313837, 0x30323837, 0x31323837, 0x32323837, 0x33323837, 0x34323837, 0x35323837, 0x36323837, 0x37323837, 0x38323837, 0x39323837, 0x30333837, 0x31333837, 0x32333837, 0x33333837, 0x34333837, 0x35333837, 0x36333837, 0x37333837, 0x38333837, 0x39333837, 0x30343837, 0x31343837, 0x32343837, 0x33343837, 0x34343837, 0x35343837, 0x36343837, 0x37343837, 0x38343837, 0x39343837, 0x30353837, 0x31353837, 0x32353837, 0x33353837, 0x34353837, 0x35353837, 0x36353837, 0x37353837, 0x38353837, 0x39353837, 0x30363837, 0x31363837, 0x32363837, 0x33363837, 0x34363837, 0x35363837, 0x36363837, 0x37363837, 0x38363837, 0x39363837, 0x30373837, 0x31373837, 0x32373837, 0x33373837, 0x34373837, 0x35373837, 0x36373837, 0x37373837, 0x38373837, 0x39373837, 0x30383837, 0x31383837, 0x32383837, 0x33383837, 0x34383837, 0x35383837, 0x36383837, 0x37383837, 0x38383837, 0x39383837, 0x30393837, 0x31393837, 0x32393837, 0x33393837, 0x34393837, 0x35393837, 0x36393837, 0x37393837, 0x38393837, 0x39393837, 0x30303937, 0x31303937, 0x32303937, 0x33303937, 0x34303937, 0x35303937, 0x36303937, 0x37303937, 0x38303937, 0x39303937, 0x30313937, 0x31313937, 0x32313937, 0x33313937, 0x34313937, 0x35313937, 0x36313937, 0x37313937, 0x38313937, 0x39313937, 0x30323937, 0x31323937, 0x32323937, 0x33323937, 0x34323937, 0x35323937, 0x36323937, 0x37323937, 0x38323937, 0x39323937, 0x30333937, 0x31333937, 0x32333937, 0x33333937, 0x34333937, 0x35333937, 0x36333937, 0x37333937, 0x38333937, 0x39333937, 0x30343937, 0x31343937, 0x32343937, 0x33343937, 0x34343937, 0x35343937, 0x36343937, 0x37343937, 0x38343937, 0x39343937, 0x30353937, 0x31353937, 0x32353937, 0x33353937, 0x34353937, 0x35353937, 0x36353937, 0x37353937, 0x38353937, 0x39353937, 0x30363937, 0x31363937, 0x32363937, 0x33363937, 0x34363937, 0x35363937, 0x36363937, 0x37363937, 0x38363937, 0x39363937, 0x30373937, 0x31373937, 0x32373937, 0x33373937, 0x34373937, 0x35373937, 0x36373937, 0x37373937, 0x38373937, 0x39373937, 0x30383937, 0x31383937, 0x32383937, 0x33383937, 0x34383937, 0x35383937, 0x36383937, 0x37383937, 0x38383937, 0x39383937, 0x30393937, 0x31393937, 0x32393937, 0x33393937, 0x34393937, 0x35393937, 0x36393937, 0x37393937, 0x38393937, 0x39393937, 0x30303038, 0x31303038, 0x32303038, 0x33303038, 0x34303038, 0x35303038, 0x36303038, 0x37303038, 0x38303038, 0x39303038, 0x30313038, 0x31313038, 0x32313038, 0x33313038, 0x34313038, 0x35313038, 0x36313038, 0x37313038, 0x38313038, 0x39313038, 0x30323038, 0x31323038, 0x32323038, 0x33323038, 0x34323038, 0x35323038, 0x36323038, 0x37323038, 0x38323038, 0x39323038, 0x30333038, 0x31333038, 0x32333038, 0x33333038, 0x34333038, 0x35333038, 0x36333038, 0x37333038, 0x38333038, 0x39333038, 0x30343038, 0x31343038, 0x32343038, 0x33343038, 0x34343038, 0x35343038, 0x36343038, 0x37343038, 0x38343038, 0x39343038, 0x30353038, 0x31353038, 0x32353038, 0x33353038, 0x34353038, 0x35353038, 0x36353038, 0x37353038, 0x38353038, 0x39353038, 0x30363038, 0x31363038, 0x32363038, 0x33363038, 0x34363038, 0x35363038, 0x36363038, 0x37363038, 0x38363038, 0x39363038, 0x30373038, 0x31373038, 0x32373038, 0x33373038, 0x34373038, 0x35373038, 0x36373038, 0x37373038, 0x38373038, 0x39373038, 0x30383038, 0x31383038, 0x32383038, 0x33383038, 0x34383038, 0x35383038, 0x36383038, 0x37383038, 0x38383038, 0x39383038, 0x30393038, 0x31393038, 0x32393038, 0x33393038, 0x34393038, 0x35393038, 0x36393038, 0x37393038, 0x38393038, 0x39393038, 0x30303138, 0x31303138, 0x32303138, 0x33303138, 0x34303138, 0x35303138, 0x36303138, 0x37303138, 0x38303138, 0x39303138, 0x30313138, 0x31313138, 0x32313138, 0x33313138, 0x34313138, 0x35313138, 0x36313138, 0x37313138, 0x38313138, 0x39313138, 0x30323138, 0x31323138, 0x32323138, 0x33323138, 0x34323138, 0x35323138, 0x36323138, 0x37323138, 0x38323138, 0x39323138, 0x30333138, 0x31333138, 0x32333138, 0x33333138, 0x34333138, 0x35333138, 0x36333138, 0x37333138, 0x38333138, 0x39333138, 0x30343138, 0x31343138, 0x32343138, 0x33343138, 0x34343138, 0x35343138, 0x36343138, 0x37343138, 0x38343138, 0x39343138, 0x30353138, 0x31353138, 0x32353138, 0x33353138, 0x34353138, 0x35353138, 0x36353138, 0x37353138, 0x38353138, 0x39353138, 0x30363138, 0x31363138, 0x32363138, 0x33363138, 0x34363138, 0x35363138, 0x36363138, 0x37363138, 0x38363138, 0x39363138, 0x30373138, 0x31373138, 0x32373138, 0x33373138, 0x34373138, 0x35373138, 0x36373138, 0x37373138, 0x38373138, 0x39373138, 0x30383138, 0x31383138, 0x32383138, 0x33383138, 0x34383138, 0x35383138, 0x36383138, 0x37383138, 0x38383138, 0x39383138, 0x30393138, 0x31393138, 0x32393138, 0x33393138, 0x34393138, 0x35393138, 0x36393138, 0x37393138, 0x38393138, 0x39393138, 0x30303238, 0x31303238, 0x32303238, 0x33303238, 0x34303238, 0x35303238, 0x36303238, 0x37303238, 0x38303238, 0x39303238, 0x30313238, 0x31313238, 0x32313238, 0x33313238, 0x34313238, 0x35313238, 0x36313238, 0x37313238, 0x38313238, 0x39313238, 0x30323238, 0x31323238, 0x32323238, 0x33323238, 0x34323238, 0x35323238, 0x36323238, 0x37323238, 0x38323238, 0x39323238, 0x30333238, 0x31333238, 0x32333238, 0x33333238, 0x34333238, 0x35333238, 0x36333238, 0x37333238, 0x38333238, 0x39333238, 0x30343238, 0x31343238, 0x32343238, 0x33343238, 0x34343238, 0x35343238, 0x36343238, 0x37343238, 0x38343238, 0x39343238, 0x30353238, 0x31353238, 0x32353238, 0x33353238, 0x34353238, 0x35353238, 0x36353238, 0x37353238, 0x38353238, 0x39353238, 0x30363238, 0x31363238, 0x32363238, 0x33363238, 0x34363238, 0x35363238, 0x36363238, 0x37363238, 0x38363238, 0x39363238, 0x30373238, 0x31373238, 0x32373238, 0x33373238, 0x34373238, 0x35373238, 0x36373238, 0x37373238, 0x38373238, 0x39373238, 0x30383238, 0x31383238, 0x32383238, 0x33383238, 0x34383238, 0x35383238, 0x36383238, 0x37383238, 0x38383238, 0x39383238, 0x30393238, 0x31393238, 0x32393238, 0x33393238, 0x34393238, 0x35393238, 0x36393238, 0x37393238, 0x38393238, 0x39393238, 0x30303338, 0x31303338, 0x32303338, 0x33303338, 0x34303338, 0x35303338, 0x36303338, 0x37303338, 0x38303338, 0x39303338, 0x30313338, 0x31313338, 0x32313338, 0x33313338, 0x34313338, 0x35313338, 0x36313338, 0x37313338, 0x38313338, 0x39313338, 0x30323338, 0x31323338, 0x32323338, 0x33323338, 0x34323338, 0x35323338, 0x36323338, 0x37323338, 0x38323338, 0x39323338, 0x30333338, 0x31333338, 0x32333338, 0x33333338, 0x34333338, 0x35333338, 0x36333338, 0x37333338, 0x38333338, 0x39333338, 0x30343338, 0x31343338, 0x32343338, 0x33343338, 0x34343338, 0x35343338, 0x36343338, 0x37343338, 0x38343338, 0x39343338, 0x30353338, 0x31353338, 0x32353338, 0x33353338, 0x34353338, 0x35353338, 0x36353338, 0x37353338, 0x38353338, 0x39353338, 0x30363338, 0x31363338, 0x32363338, 0x33363338, 0x34363338, 0x35363338, 0x36363338, 0x37363338, 0x38363338, 0x39363338, 0x30373338, 0x31373338, 0x32373338, 0x33373338, 0x34373338, 0x35373338, 0x36373338, 0x37373338, 0x38373338, 0x39373338, 0x30383338, 0x31383338, 0x32383338, 0x33383338, 0x34383338, 0x35383338, 0x36383338, 0x37383338, 0x38383338, 0x39383338, 0x30393338, 0x31393338, 0x32393338, 0x33393338, 0x34393338, 0x35393338, 0x36393338, 0x37393338, 0x38393338, 0x39393338, 0x30303438, 0x31303438, 0x32303438, 0x33303438, 0x34303438, 0x35303438, 0x36303438, 0x37303438, 0x38303438, 0x39303438, 0x30313438, 0x31313438, 0x32313438, 0x33313438, 0x34313438, 0x35313438, 0x36313438, 0x37313438, 0x38313438, 0x39313438, 0x30323438, 0x31323438, 0x32323438, 0x33323438, 0x34323438, 0x35323438, 0x36323438, 0x37323438, 0x38323438, 0x39323438, 0x30333438, 0x31333438, 0x32333438, 0x33333438, 0x34333438, 0x35333438, 0x36333438, 0x37333438, 0x38333438, 0x39333438, 0x30343438, 0x31343438, 0x32343438, 0x33343438, 0x34343438, 0x35343438, 0x36343438, 0x37343438, 0x38343438, 0x39343438, 0x30353438, 0x31353438, 0x32353438, 0x33353438, 0x34353438, 0x35353438, 0x36353438, 0x37353438, 0x38353438, 0x39353438, 0x30363438, 0x31363438, 0x32363438, 0x33363438, 0x34363438, 0x35363438, 0x36363438, 0x37363438, 0x38363438, 0x39363438, 0x30373438, 0x31373438, 0x32373438, 0x33373438, 0x34373438, 0x35373438, 0x36373438, 0x37373438, 0x38373438, 0x39373438, 0x30383438, 0x31383438, 0x32383438, 0x33383438, 0x34383438, 0x35383438, 0x36383438, 0x37383438, 0x38383438, 0x39383438, 0x30393438, 0x31393438, 0x32393438, 0x33393438, 0x34393438, 0x35393438, 0x36393438, 0x37393438, 0x38393438, 0x39393438, 0x30303538, 0x31303538, 0x32303538, 0x33303538, 0x34303538, 0x35303538, 0x36303538, 0x37303538, 0x38303538, 0x39303538, 0x30313538, 0x31313538, 0x32313538, 0x33313538, 0x34313538, 0x35313538, 0x36313538, 0x37313538, 0x38313538, 0x39313538, 0x30323538, 0x31323538, 0x32323538, 0x33323538, 0x34323538, 0x35323538, 0x36323538, 0x37323538, 0x38323538, 0x39323538, 0x30333538, 0x31333538, 0x32333538, 0x33333538, 0x34333538, 0x35333538, 0x36333538, 0x37333538, 0x38333538, 0x39333538, 0x30343538, 0x31343538, 0x32343538, 0x33343538, 0x34343538, 0x35343538, 0x36343538, 0x37343538, 0x38343538, 0x39343538, 0x30353538, 0x31353538, 0x32353538, 0x33353538, 0x34353538, 0x35353538, 0x36353538, 0x37353538, 0x38353538, 0x39353538, 0x30363538, 0x31363538, 0x32363538, 0x33363538, 0x34363538, 0x35363538, 0x36363538, 0x37363538, 0x38363538, 0x39363538, 0x30373538, 0x31373538, 0x32373538, 0x33373538, 0x34373538, 0x35373538, 0x36373538, 0x37373538, 0x38373538, 0x39373538, 0x30383538, 0x31383538, 0x32383538, 0x33383538, 0x34383538, 0x35383538, 0x36383538, 0x37383538, 0x38383538, 0x39383538, 0x30393538, 0x31393538, 0x32393538, 0x33393538, 0x34393538, 0x35393538, 0x36393538, 0x37393538, 0x38393538, 0x39393538, 0x30303638, 0x31303638, 0x32303638, 0x33303638, 0x34303638, 0x35303638, 0x36303638, 0x37303638, 0x38303638, 0x39303638, 0x30313638, 0x31313638, 0x32313638, 0x33313638, 0x34313638, 0x35313638, 0x36313638, 0x37313638, 0x38313638, 0x39313638, 0x30323638, 0x31323638, 0x32323638, 0x33323638, 0x34323638, 0x35323638, 0x36323638, 0x37323638, 0x38323638, 0x39323638, 0x30333638, 0x31333638, 0x32333638, 0x33333638, 0x34333638, 0x35333638, 0x36333638, 0x37333638, 0x38333638, 0x39333638, 0x30343638, 0x31343638, 0x32343638, 0x33343638, 0x34343638, 0x35343638, 0x36343638, 0x37343638, 0x38343638, 0x39343638, 0x30353638, 0x31353638, 0x32353638, 0x33353638, 0x34353638, 0x35353638, 0x36353638, 0x37353638, 0x38353638, 0x39353638, 0x30363638, 0x31363638, 0x32363638, 0x33363638, 0x34363638, 0x35363638, 0x36363638, 0x37363638, 0x38363638, 0x39363638, 0x30373638, 0x31373638, 0x32373638, 0x33373638, 0x34373638, 0x35373638, 0x36373638, 0x37373638, 0x38373638, 0x39373638, 0x30383638, 0x31383638, 0x32383638, 0x33383638, 0x34383638, 0x35383638, 0x36383638, 0x37383638, 0x38383638, 0x39383638, 0x30393638, 0x31393638, 0x32393638, 0x33393638, 0x34393638, 0x35393638, 0x36393638, 0x37393638, 0x38393638, 0x39393638, 0x30303738, 0x31303738, 0x32303738, 0x33303738, 0x34303738, 0x35303738, 0x36303738, 0x37303738, 0x38303738, 0x39303738, 0x30313738, 0x31313738, 0x32313738, 0x33313738, 0x34313738, 0x35313738, 0x36313738, 0x37313738, 0x38313738, 0x39313738, 0x30323738, 0x31323738, 0x32323738, 0x33323738, 0x34323738, 0x35323738, 0x36323738, 0x37323738, 0x38323738, 0x39323738, 0x30333738, 0x31333738, 0x32333738, 0x33333738, 0x34333738, 0x35333738, 0x36333738, 0x37333738, 0x38333738, 0x39333738, 0x30343738, 0x31343738, 0x32343738, 0x33343738, 0x34343738, 0x35343738, 0x36343738, 0x37343738, 0x38343738, 0x39343738, 0x30353738, 0x31353738, 0x32353738, 0x33353738, 0x34353738, 0x35353738, 0x36353738, 0x37353738, 0x38353738, 0x39353738, 0x30363738, 0x31363738, 0x32363738, 0x33363738, 0x34363738, 0x35363738, 0x36363738, 0x37363738, 0x38363738, 0x39363738, 0x30373738, 0x31373738, 0x32373738, 0x33373738, 0x34373738, 0x35373738, 0x36373738, 0x37373738, 0x38373738, 0x39373738, 0x30383738, 0x31383738, 0x32383738, 0x33383738, 0x34383738, 0x35383738, 0x36383738, 0x37383738, 0x38383738, 0x39383738, 0x30393738, 0x31393738, 0x32393738, 0x33393738, 0x34393738, 0x35393738, 0x36393738, 0x37393738, 0x38393738, 0x39393738, 0x30303838, 0x31303838, 0x32303838, 0x33303838, 0x34303838, 0x35303838, 0x36303838, 0x37303838, 0x38303838, 0x39303838, 0x30313838, 0x31313838, 0x32313838, 0x33313838, 0x34313838, 0x35313838, 0x36313838, 0x37313838, 0x38313838, 0x39313838, 0x30323838, 0x31323838, 0x32323838, 0x33323838, 0x34323838, 0x35323838, 0x36323838, 0x37323838, 0x38323838, 0x39323838, 0x30333838, 0x31333838, 0x32333838, 0x33333838, 0x34333838, 0x35333838, 0x36333838, 0x37333838, 0x38333838, 0x39333838, 0x30343838, 0x31343838, 0x32343838, 0x33343838, 0x34343838, 0x35343838, 0x36343838, 0x37343838, 0x38343838, 0x39343838, 0x30353838, 0x31353838, 0x32353838, 0x33353838, 0x34353838, 0x35353838, 0x36353838, 0x37353838, 0x38353838, 0x39353838, 0x30363838, 0x31363838, 0x32363838, 0x33363838, 0x34363838, 0x35363838, 0x36363838, 0x37363838, 0x38363838, 0x39363838, 0x30373838, 0x31373838, 0x32373838, 0x33373838, 0x34373838, 0x35373838, 0x36373838, 0x37373838, 0x38373838, 0x39373838, 0x30383838, 0x31383838, 0x32383838, 0x33383838, 0x34383838, 0x35383838, 0x36383838, 0x37383838, 0x38383838, 0x39383838, 0x30393838, 0x31393838, 0x32393838, 0x33393838, 0x34393838, 0x35393838, 0x36393838, 0x37393838, 0x38393838, 0x39393838, 0x30303938, 0x31303938, 0x32303938, 0x33303938, 0x34303938, 0x35303938, 0x36303938, 0x37303938, 0x38303938, 0x39303938, 0x30313938, 0x31313938, 0x32313938, 0x33313938, 0x34313938, 0x35313938, 0x36313938, 0x37313938, 0x38313938, 0x39313938, 0x30323938, 0x31323938, 0x32323938, 0x33323938, 0x34323938, 0x35323938, 0x36323938, 0x37323938, 0x38323938, 0x39323938, 0x30333938, 0x31333938, 0x32333938, 0x33333938, 0x34333938, 0x35333938, 0x36333938, 0x37333938, 0x38333938, 0x39333938, 0x30343938, 0x31343938, 0x32343938, 0x33343938, 0x34343938, 0x35343938, 0x36343938, 0x37343938, 0x38343938, 0x39343938, 0x30353938, 0x31353938, 0x32353938, 0x33353938, 0x34353938, 0x35353938, 0x36353938, 0x37353938, 0x38353938, 0x39353938, 0x30363938, 0x31363938, 0x32363938, 0x33363938, 0x34363938, 0x35363938, 0x36363938, 0x37363938, 0x38363938, 0x39363938, 0x30373938, 0x31373938, 0x32373938, 0x33373938, 0x34373938, 0x35373938, 0x36373938, 0x37373938, 0x38373938, 0x39373938, 0x30383938, 0x31383938, 0x32383938, 0x33383938, 0x34383938, 0x35383938, 0x36383938, 0x37383938, 0x38383938, 0x39383938, 0x30393938, 0x31393938, 0x32393938, 0x33393938, 0x34393938, 0x35393938, 0x36393938, 0x37393938, 0x38393938, 0x39393938, 0x30303039, 0x31303039, 0x32303039, 0x33303039, 0x34303039, 0x35303039, 0x36303039, 0x37303039, 0x38303039, 0x39303039, 0x30313039, 0x31313039, 0x32313039, 0x33313039, 0x34313039, 0x35313039, 0x36313039, 0x37313039, 0x38313039, 0x39313039, 0x30323039, 0x31323039, 0x32323039, 0x33323039, 0x34323039, 0x35323039, 0x36323039, 0x37323039, 0x38323039, 0x39323039, 0x30333039, 0x31333039, 0x32333039, 0x33333039, 0x34333039, 0x35333039, 0x36333039, 0x37333039, 0x38333039, 0x39333039, 0x30343039, 0x31343039, 0x32343039, 0x33343039, 0x34343039, 0x35343039, 0x36343039, 0x37343039, 0x38343039, 0x39343039, 0x30353039, 0x31353039, 0x32353039, 0x33353039, 0x34353039, 0x35353039, 0x36353039, 0x37353039, 0x38353039, 0x39353039, 0x30363039, 0x31363039, 0x32363039, 0x33363039, 0x34363039, 0x35363039, 0x36363039, 0x37363039, 0x38363039, 0x39363039, 0x30373039, 0x31373039, 0x32373039, 0x33373039, 0x34373039, 0x35373039, 0x36373039, 0x37373039, 0x38373039, 0x39373039, 0x30383039, 0x31383039, 0x32383039, 0x33383039, 0x34383039, 0x35383039, 0x36383039, 0x37383039, 0x38383039, 0x39383039, 0x30393039, 0x31393039, 0x32393039, 0x33393039, 0x34393039, 0x35393039, 0x36393039, 0x37393039, 0x38393039, 0x39393039, 0x30303139, 0x31303139, 0x32303139, 0x33303139, 0x34303139, 0x35303139, 0x36303139, 0x37303139, 0x38303139, 0x39303139, 0x30313139, 0x31313139, 0x32313139, 0x33313139, 0x34313139, 0x35313139, 0x36313139, 0x37313139, 0x38313139, 0x39313139, 0x30323139, 0x31323139, 0x32323139, 0x33323139, 0x34323139, 0x35323139, 0x36323139, 0x37323139, 0x38323139, 0x39323139, 0x30333139, 0x31333139, 0x32333139, 0x33333139, 0x34333139, 0x35333139, 0x36333139, 0x37333139, 0x38333139, 0x39333139, 0x30343139, 0x31343139, 0x32343139, 0x33343139, 0x34343139, 0x35343139, 0x36343139, 0x37343139, 0x38343139, 0x39343139, 0x30353139, 0x31353139, 0x32353139, 0x33353139, 0x34353139, 0x35353139, 0x36353139, 0x37353139, 0x38353139, 0x39353139, 0x30363139, 0x31363139, 0x32363139, 0x33363139, 0x34363139, 0x35363139, 0x36363139, 0x37363139, 0x38363139, 0x39363139, 0x30373139, 0x31373139, 0x32373139, 0x33373139, 0x34373139, 0x35373139, 0x36373139, 0x37373139, 0x38373139, 0x39373139, 0x30383139, 0x31383139, 0x32383139, 0x33383139, 0x34383139, 0x35383139, 0x36383139, 0x37383139, 0x38383139, 0x39383139, 0x30393139, 0x31393139, 0x32393139, 0x33393139, 0x34393139, 0x35393139, 0x36393139, 0x37393139, 0x38393139, 0x39393139, 0x30303239, 0x31303239, 0x32303239, 0x33303239, 0x34303239, 0x35303239, 0x36303239, 0x37303239, 0x38303239, 0x39303239, 0x30313239, 0x31313239, 0x32313239, 0x33313239, 0x34313239, 0x35313239, 0x36313239, 0x37313239, 0x38313239, 0x39313239, 0x30323239, 0x31323239, 0x32323239, 0x33323239, 0x34323239, 0x35323239, 0x36323239, 0x37323239, 0x38323239, 0x39323239, 0x30333239, 0x31333239, 0x32333239, 0x33333239, 0x34333239, 0x35333239, 0x36333239, 0x37333239, 0x38333239, 0x39333239, 0x30343239, 0x31343239, 0x32343239, 0x33343239, 0x34343239, 0x35343239, 0x36343239, 0x37343239, 0x38343239, 0x39343239, 0x30353239, 0x31353239, 0x32353239, 0x33353239, 0x34353239, 0x35353239, 0x36353239, 0x37353239, 0x38353239, 0x39353239, 0x30363239, 0x31363239, 0x32363239, 0x33363239, 0x34363239, 0x35363239, 0x36363239, 0x37363239, 0x38363239, 0x39363239, 0x30373239, 0x31373239, 0x32373239, 0x33373239, 0x34373239, 0x35373239, 0x36373239, 0x37373239, 0x38373239, 0x39373239, 0x30383239, 0x31383239, 0x32383239, 0x33383239, 0x34383239, 0x35383239, 0x36383239, 0x37383239, 0x38383239, 0x39383239, 0x30393239, 0x31393239, 0x32393239, 0x33393239, 0x34393239, 0x35393239, 0x36393239, 0x37393239, 0x38393239, 0x39393239, 0x30303339, 0x31303339, 0x32303339, 0x33303339, 0x34303339, 0x35303339, 0x36303339, 0x37303339, 0x38303339, 0x39303339, 0x30313339, 0x31313339, 0x32313339, 0x33313339, 0x34313339, 0x35313339, 0x36313339, 0x37313339, 0x38313339, 0x39313339, 0x30323339, 0x31323339, 0x32323339, 0x33323339, 0x34323339, 0x35323339, 0x36323339, 0x37323339, 0x38323339, 0x39323339, 0x30333339, 0x31333339, 0x32333339, 0x33333339, 0x34333339, 0x35333339, 0x36333339, 0x37333339, 0x38333339, 0x39333339, 0x30343339, 0x31343339, 0x32343339, 0x33343339, 0x34343339, 0x35343339, 0x36343339, 0x37343339, 0x38343339, 0x39343339, 0x30353339, 0x31353339, 0x32353339, 0x33353339, 0x34353339, 0x35353339, 0x36353339, 0x37353339, 0x38353339, 0x39353339, 0x30363339, 0x31363339, 0x32363339, 0x33363339, 0x34363339, 0x35363339, 0x36363339, 0x37363339, 0x38363339, 0x39363339, 0x30373339, 0x31373339, 0x32373339, 0x33373339, 0x34373339, 0x35373339, 0x36373339, 0x37373339, 0x38373339, 0x39373339, 0x30383339, 0x31383339, 0x32383339, 0x33383339, 0x34383339, 0x35383339, 0x36383339, 0x37383339, 0x38383339, 0x39383339, 0x30393339, 0x31393339, 0x32393339, 0x33393339, 0x34393339, 0x35393339, 0x36393339, 0x37393339, 0x38393339, 0x39393339, 0x30303439, 0x31303439, 0x32303439, 0x33303439, 0x34303439, 0x35303439, 0x36303439, 0x37303439, 0x38303439, 0x39303439, 0x30313439, 0x31313439, 0x32313439, 0x33313439, 0x34313439, 0x35313439, 0x36313439, 0x37313439, 0x38313439, 0x39313439, 0x30323439, 0x31323439, 0x32323439, 0x33323439, 0x34323439, 0x35323439, 0x36323439, 0x37323439, 0x38323439, 0x39323439, 0x30333439, 0x31333439, 0x32333439, 0x33333439, 0x34333439, 0x35333439, 0x36333439, 0x37333439, 0x38333439, 0x39333439, 0x30343439, 0x31343439, 0x32343439, 0x33343439, 0x34343439, 0x35343439, 0x36343439, 0x37343439, 0x38343439, 0x39343439, 0x30353439, 0x31353439, 0x32353439, 0x33353439, 0x34353439, 0x35353439, 0x36353439, 0x37353439, 0x38353439, 0x39353439, 0x30363439, 0x31363439, 0x32363439, 0x33363439, 0x34363439, 0x35363439, 0x36363439, 0x37363439, 0x38363439, 0x39363439, 0x30373439, 0x31373439, 0x32373439, 0x33373439, 0x34373439, 0x35373439, 0x36373439, 0x37373439, 0x38373439, 0x39373439, 0x30383439, 0x31383439, 0x32383439, 0x33383439, 0x34383439, 0x35383439, 0x36383439, 0x37383439, 0x38383439, 0x39383439, 0x30393439, 0x31393439, 0x32393439, 0x33393439, 0x34393439, 0x35393439, 0x36393439, 0x37393439, 0x38393439, 0x39393439, 0x30303539, 0x31303539, 0x32303539, 0x33303539, 0x34303539, 0x35303539, 0x36303539, 0x37303539, 0x38303539, 0x39303539, 0x30313539, 0x31313539, 0x32313539, 0x33313539, 0x34313539, 0x35313539, 0x36313539, 0x37313539, 0x38313539, 0x39313539, 0x30323539, 0x31323539, 0x32323539, 0x33323539, 0x34323539, 0x35323539, 0x36323539, 0x37323539, 0x38323539, 0x39323539, 0x30333539, 0x31333539, 0x32333539, 0x33333539, 0x34333539, 0x35333539, 0x36333539, 0x37333539, 0x38333539, 0x39333539, 0x30343539, 0x31343539, 0x32343539, 0x33343539, 0x34343539, 0x35343539, 0x36343539, 0x37343539, 0x38343539, 0x39343539, 0x30353539, 0x31353539, 0x32353539, 0x33353539, 0x34353539, 0x35353539, 0x36353539, 0x37353539, 0x38353539, 0x39353539, 0x30363539, 0x31363539, 0x32363539, 0x33363539, 0x34363539, 0x35363539, 0x36363539, 0x37363539, 0x38363539, 0x39363539, 0x30373539, 0x31373539, 0x32373539, 0x33373539, 0x34373539, 0x35373539, 0x36373539, 0x37373539, 0x38373539, 0x39373539, 0x30383539, 0x31383539, 0x32383539, 0x33383539, 0x34383539, 0x35383539, 0x36383539, 0x37383539, 0x38383539, 0x39383539, 0x30393539, 0x31393539, 0x32393539, 0x33393539, 0x34393539, 0x35393539, 0x36393539, 0x37393539, 0x38393539, 0x39393539, 0x30303639, 0x31303639, 0x32303639, 0x33303639, 0x34303639, 0x35303639, 0x36303639, 0x37303639, 0x38303639, 0x39303639, 0x30313639, 0x31313639, 0x32313639, 0x33313639, 0x34313639, 0x35313639, 0x36313639, 0x37313639, 0x38313639, 0x39313639, 0x30323639, 0x31323639, 0x32323639, 0x33323639, 0x34323639, 0x35323639, 0x36323639, 0x37323639, 0x38323639, 0x39323639, 0x30333639, 0x31333639, 0x32333639, 0x33333639, 0x34333639, 0x35333639, 0x36333639, 0x37333639, 0x38333639, 0x39333639, 0x30343639, 0x31343639, 0x32343639, 0x33343639, 0x34343639, 0x35343639, 0x36343639, 0x37343639, 0x38343639, 0x39343639, 0x30353639, 0x31353639, 0x32353639, 0x33353639, 0x34353639, 0x35353639, 0x36353639, 0x37353639, 0x38353639, 0x39353639, 0x30363639, 0x31363639, 0x32363639, 0x33363639, 0x34363639, 0x35363639, 0x36363639, 0x37363639, 0x38363639, 0x39363639, 0x30373639, 0x31373639, 0x32373639, 0x33373639, 0x34373639, 0x35373639, 0x36373639, 0x37373639, 0x38373639, 0x39373639, 0x30383639, 0x31383639, 0x32383639, 0x33383639, 0x34383639, 0x35383639, 0x36383639, 0x37383639, 0x38383639, 0x39383639, 0x30393639, 0x31393639, 0x32393639, 0x33393639, 0x34393639, 0x35393639, 0x36393639, 0x37393639, 0x38393639, 0x39393639, 0x30303739, 0x31303739, 0x32303739, 0x33303739, 0x34303739, 0x35303739, 0x36303739, 0x37303739, 0x38303739, 0x39303739, 0x30313739, 0x31313739, 0x32313739, 0x33313739, 0x34313739, 0x35313739, 0x36313739, 0x37313739, 0x38313739, 0x39313739, 0x30323739, 0x31323739, 0x32323739, 0x33323739, 0x34323739, 0x35323739, 0x36323739, 0x37323739, 0x38323739, 0x39323739, 0x30333739, 0x31333739, 0x32333739, 0x33333739, 0x34333739, 0x35333739, 0x36333739, 0x37333739, 0x38333739, 0x39333739, 0x30343739, 0x31343739, 0x32343739, 0x33343739, 0x34343739, 0x35343739, 0x36343739, 0x37343739, 0x38343739, 0x39343739, 0x30353739, 0x31353739, 0x32353739, 0x33353739, 0x34353739, 0x35353739, 0x36353739, 0x37353739, 0x38353739, 0x39353739, 0x30363739, 0x31363739, 0x32363739, 0x33363739, 0x34363739, 0x35363739, 0x36363739, 0x37363739, 0x38363739, 0x39363739, 0x30373739, 0x31373739, 0x32373739, 0x33373739, 0x34373739, 0x35373739, 0x36373739, 0x37373739, 0x38373739, 0x39373739, 0x30383739, 0x31383739, 0x32383739, 0x33383739, 0x34383739, 0x35383739, 0x36383739, 0x37383739, 0x38383739, 0x39383739, 0x30393739, 0x31393739, 0x32393739, 0x33393739, 0x34393739, 0x35393739, 0x36393739, 0x37393739, 0x38393739, 0x39393739, 0x30303839, 0x31303839, 0x32303839, 0x33303839, 0x34303839, 0x35303839, 0x36303839, 0x37303839, 0x38303839, 0x39303839, 0x30313839, 0x31313839, 0x32313839, 0x33313839, 0x34313839, 0x35313839, 0x36313839, 0x37313839, 0x38313839, 0x39313839, 0x30323839, 0x31323839, 0x32323839, 0x33323839, 0x34323839, 0x35323839, 0x36323839, 0x37323839, 0x38323839, 0x39323839, 0x30333839, 0x31333839, 0x32333839, 0x33333839, 0x34333839, 0x35333839, 0x36333839, 0x37333839, 0x38333839, 0x39333839, 0x30343839, 0x31343839, 0x32343839, 0x33343839, 0x34343839, 0x35343839, 0x36343839, 0x37343839, 0x38343839, 0x39343839, 0x30353839, 0x31353839, 0x32353839, 0x33353839, 0x34353839, 0x35353839, 0x36353839, 0x37353839, 0x38353839, 0x39353839, 0x30363839, 0x31363839, 0x32363839, 0x33363839, 0x34363839, 0x35363839, 0x36363839, 0x37363839, 0x38363839, 0x39363839, 0x30373839, 0x31373839, 0x32373839, 0x33373839, 0x34373839, 0x35373839, 0x36373839, 0x37373839, 0x38373839, 0x39373839, 0x30383839, 0x31383839, 0x32383839, 0x33383839, 0x34383839, 0x35383839, 0x36383839, 0x37383839, 0x38383839, 0x39383839, 0x30393839, 0x31393839, 0x32393839, 0x33393839, 0x34393839, 0x35393839, 0x36393839, 0x37393839, 0x38393839, 0x39393839, 0x30303939, 0x31303939, 0x32303939, 0x33303939, 0x34303939, 0x35303939, 0x36303939, 0x37303939, 0x38303939, 0x39303939, 0x30313939, 0x31313939, 0x32313939, 0x33313939, 0x34313939, 0x35313939, 0x36313939, 0x37313939, 0x38313939, 0x39313939, 0x30323939, 0x31323939, 0x32323939, 0x33323939, 0x34323939, 0x35323939, 0x36323939, 0x37323939, 0x38323939, 0x39323939, 0x30333939, 0x31333939, 0x32333939, 0x33333939, 0x34333939, 0x35333939, 0x36333939, 0x37333939, 0x38333939, 0x39333939, 0x30343939, 0x31343939, 0x32343939, 0x33343939, 0x34343939, 0x35343939, 0x36343939, 0x37343939, 0x38343939, 0x39343939, 0x30353939, 0x31353939, 0x32353939, 0x33353939, 0x34353939, 0x35353939, 0x36353939, 0x37353939, 0x38353939, 0x39353939, 0x30363939, 0x31363939, 0x32363939, 0x33363939, 0x34363939, 0x35363939, 0x36363939, 0x37363939, 0x38363939, 0x39363939, 0x30373939, 0x31373939, 0x32373939, 0x33373939, 0x34373939, 0x35373939, 0x36373939, 0x37373939, 0x38373939, 0x39373939, 0x30383939, 0x31383939, 0x32383939, 0x33383939, 0x34383939, 0x35383939, 0x36383939, 0x37383939, 0x38383939, 0x39383939, 0x30393939, 0x31393939, 0x32393939, 0x33393939, 0x34393939, 0x35393939, 0x36393939, 0x37393939, 0x38393939, 0x39393939}; // 40KB
static const unsigned int digit1000[1000] = {0x30303030, 0x30313030, 0x30323030, 0x30333030, 0x30343030, 0x30353030, 0x30363030, 0x30373030, 0x30383030, 0x30393030, 0x30303130, 0x30313130, 0x30323130, 0x30333130, 0x30343130, 0x30353130, 0x30363130, 0x30373130, 0x30383130, 0x30393130, 0x30303230, 0x30313230, 0x30323230, 0x30333230, 0x30343230, 0x30353230, 0x30363230, 0x30373230, 0x30383230, 0x30393230, 0x30303330, 0x30313330, 0x30323330, 0x30333330, 0x30343330, 0x30353330, 0x30363330, 0x30373330, 0x30383330, 0x30393330, 0x30303430, 0x30313430, 0x30323430, 0x30333430, 0x30343430, 0x30353430, 0x30363430, 0x30373430, 0x30383430, 0x30393430, 0x30303530, 0x30313530, 0x30323530, 0x30333530, 0x30343530, 0x30353530, 0x30363530, 0x30373530, 0x30383530, 0x30393530, 0x30303630, 0x30313630, 0x30323630, 0x30333630, 0x30343630, 0x30353630, 0x30363630, 0x30373630, 0x30383630, 0x30393630, 0x30303730, 0x30313730, 0x30323730, 0x30333730, 0x30343730, 0x30353730, 0x30363730, 0x30373730, 0x30383730, 0x30393730, 0x30303830, 0x30313830, 0x30323830, 0x30333830, 0x30343830, 0x30353830, 0x30363830, 0x30373830, 0x30383830, 0x30393830, 0x30303930, 0x30313930, 0x30323930, 0x30333930, 0x30343930, 0x30353930, 0x30363930, 0x30373930, 0x30383930, 0x30393930, 0x30303031, 0x30313031, 0x30323031, 0x30333031, 0x30343031, 0x30353031, 0x30363031, 0x30373031, 0x30383031, 0x30393031, 0x30303131, 0x30313131, 0x30323131, 0x30333131, 0x30343131, 0x30353131, 0x30363131, 0x30373131, 0x30383131, 0x30393131, 0x30303231, 0x30313231, 0x30323231, 0x30333231, 0x30343231, 0x30353231, 0x30363231, 0x30373231, 0x30383231, 0x30393231, 0x30303331, 0x30313331, 0x30323331, 0x30333331, 0x30343331, 0x30353331, 0x30363331, 0x30373331, 0x30383331, 0x30393331, 0x30303431, 0x30313431, 0x30323431, 0x30333431, 0x30343431, 0x30353431, 0x30363431, 0x30373431, 0x30383431, 0x30393431, 0x30303531, 0x30313531, 0x30323531, 0x30333531, 0x30343531, 0x30353531, 0x30363531, 0x30373531, 0x30383531, 0x30393531, 0x30303631, 0x30313631, 0x30323631, 0x30333631, 0x30343631, 0x30353631, 0x30363631, 0x30373631, 0x30383631, 0x30393631, 0x30303731, 0x30313731, 0x30323731, 0x30333731, 0x30343731, 0x30353731, 0x30363731, 0x30373731, 0x30383731, 0x30393731, 0x30303831, 0x30313831, 0x30323831, 0x30333831, 0x30343831, 0x30353831, 0x30363831, 0x30373831, 0x30383831, 0x30393831, 0x30303931, 0x30313931, 0x30323931, 0x30333931, 0x30343931, 0x30353931, 0x30363931, 0x30373931, 0x30383931, 0x30393931, 0x30303032, 0x30313032, 0x30323032, 0x30333032, 0x30343032, 0x30353032, 0x30363032, 0x30373032, 0x30383032, 0x30393032, 0x30303132, 0x30313132, 0x30323132, 0x30333132, 0x30343132, 0x30353132, 0x30363132, 0x30373132, 0x30383132, 0x30393132, 0x30303232, 0x30313232, 0x30323232, 0x30333232, 0x30343232, 0x30353232, 0x30363232, 0x30373232, 0x30383232, 0x30393232, 0x30303332, 0x30313332, 0x30323332, 0x30333332, 0x30343332, 0x30353332, 0x30363332, 0x30373332, 0x30383332, 0x30393332, 0x30303432, 0x30313432, 0x30323432, 0x30333432, 0x30343432, 0x30353432, 0x30363432, 0x30373432, 0x30383432, 0x30393432, 0x30303532, 0x30313532, 0x30323532, 0x30333532, 0x30343532, 0x30353532, 0x30363532, 0x30373532, 0x30383532, 0x30393532, 0x30303632, 0x30313632, 0x30323632, 0x30333632, 0x30343632, 0x30353632, 0x30363632, 0x30373632, 0x30383632, 0x30393632, 0x30303732, 0x30313732, 0x30323732, 0x30333732, 0x30343732, 0x30353732, 0x30363732, 0x30373732, 0x30383732, 0x30393732, 0x30303832, 0x30313832, 0x30323832, 0x30333832, 0x30343832, 0x30353832, 0x30363832, 0x30373832, 0x30383832, 0x30393832, 0x30303932, 0x30313932, 0x30323932, 0x30333932, 0x30343932, 0x30353932, 0x30363932, 0x30373932, 0x30383932, 0x30393932, 0x30303033, 0x30313033, 0x30323033, 0x30333033, 0x30343033, 0x30353033, 0x30363033, 0x30373033, 0x30383033, 0x30393033, 0x30303133, 0x30313133, 0x30323133, 0x30333133, 0x30343133, 0x30353133, 0x30363133, 0x30373133, 0x30383133, 0x30393133, 0x30303233, 0x30313233, 0x30323233, 0x30333233, 0x30343233, 0x30353233, 0x30363233, 0x30373233, 0x30383233, 0x30393233, 0x30303333, 0x30313333, 0x30323333, 0x30333333, 0x30343333, 0x30353333, 0x30363333, 0x30373333, 0x30383333, 0x30393333, 0x30303433, 0x30313433, 0x30323433, 0x30333433, 0x30343433, 0x30353433, 0x30363433, 0x30373433, 0x30383433, 0x30393433, 0x30303533, 0x30313533, 0x30323533, 0x30333533, 0x30343533, 0x30353533, 0x30363533, 0x30373533, 0x30383533, 0x30393533, 0x30303633, 0x30313633, 0x30323633, 0x30333633, 0x30343633, 0x30353633, 0x30363633, 0x30373633, 0x30383633, 0x30393633, 0x30303733, 0x30313733, 0x30323733, 0x30333733, 0x30343733, 0x30353733, 0x30363733, 0x30373733, 0x30383733, 0x30393733, 0x30303833, 0x30313833, 0x30323833, 0x30333833, 0x30343833, 0x30353833, 0x30363833, 0x30373833, 0x30383833, 0x30393833, 0x30303933, 0x30313933, 0x30323933, 0x30333933, 0x30343933, 0x30353933, 0x30363933, 0x30373933, 0x30383933, 0x30393933, 0x30303034, 0x30313034, 0x30323034, 0x30333034, 0x30343034, 0x30353034, 0x30363034, 0x30373034, 0x30383034, 0x30393034, 0x30303134, 0x30313134, 0x30323134, 0x30333134, 0x30343134, 0x30353134, 0x30363134, 0x30373134, 0x30383134, 0x30393134, 0x30303234, 0x30313234, 0x30323234, 0x30333234, 0x30343234, 0x30353234, 0x30363234, 0x30373234, 0x30383234, 0x30393234, 0x30303334, 0x30313334, 0x30323334, 0x30333334, 0x30343334, 0x30353334, 0x30363334, 0x30373334, 0x30383334, 0x30393334, 0x30303434, 0x30313434, 0x30323434, 0x30333434, 0x30343434, 0x30353434, 0x30363434, 0x30373434, 0x30383434, 0x30393434, 0x30303534, 0x30313534, 0x30323534, 0x30333534, 0x30343534, 0x30353534, 0x30363534, 0x30373534, 0x30383534, 0x30393534, 0x30303634, 0x30313634, 0x30323634, 0x30333634, 0x30343634, 0x30353634, 0x30363634, 0x30373634, 0x30383634, 0x30393634, 0x30303734, 0x30313734, 0x30323734, 0x30333734, 0x30343734, 0x30353734, 0x30363734, 0x30373734, 0x30383734, 0x30393734, 0x30303834, 0x30313834, 0x30323834, 0x30333834, 0x30343834, 0x30353834, 0x30363834, 0x30373834, 0x30383834, 0x30393834, 0x30303934, 0x30313934, 0x30323934, 0x30333934, 0x30343934, 0x30353934, 0x30363934, 0x30373934, 0x30383934, 0x30393934, 0x30303035, 0x30313035, 0x30323035, 0x30333035, 0x30343035, 0x30353035, 0x30363035, 0x30373035, 0x30383035, 0x30393035, 0x30303135, 0x30313135, 0x30323135, 0x30333135, 0x30343135, 0x30353135, 0x30363135, 0x30373135, 0x30383135, 0x30393135, 0x30303235, 0x30313235, 0x30323235, 0x30333235, 0x30343235, 0x30353235, 0x30363235, 0x30373235, 0x30383235, 0x30393235, 0x30303335, 0x30313335, 0x30323335, 0x30333335, 0x30343335, 0x30353335, 0x30363335, 0x30373335, 0x30383335, 0x30393335, 0x30303435, 0x30313435, 0x30323435, 0x30333435, 0x30343435, 0x30353435, 0x30363435, 0x30373435, 0x30383435, 0x30393435, 0x30303535, 0x30313535, 0x30323535, 0x30333535, 0x30343535, 0x30353535, 0x30363535, 0x30373535, 0x30383535, 0x30393535, 0x30303635, 0x30313635, 0x30323635, 0x30333635, 0x30343635, 0x30353635, 0x30363635, 0x30373635, 0x30383635, 0x30393635, 0x30303735, 0x30313735, 0x30323735, 0x30333735, 0x30343735, 0x30353735, 0x30363735, 0x30373735, 0x30383735, 0x30393735, 0x30303835, 0x30313835, 0x30323835, 0x30333835, 0x30343835, 0x30353835, 0x30363835, 0x30373835, 0x30383835, 0x30393835, 0x30303935, 0x30313935, 0x30323935, 0x30333935, 0x30343935, 0x30353935, 0x30363935, 0x30373935, 0x30383935, 0x30393935, 0x30303036, 0x30313036, 0x30323036, 0x30333036, 0x30343036, 0x30353036, 0x30363036, 0x30373036, 0x30383036, 0x30393036, 0x30303136, 0x30313136, 0x30323136, 0x30333136, 0x30343136, 0x30353136, 0x30363136, 0x30373136, 0x30383136, 0x30393136, 0x30303236, 0x30313236, 0x30323236, 0x30333236, 0x30343236, 0x30353236, 0x30363236, 0x30373236, 0x30383236, 0x30393236, 0x30303336, 0x30313336, 0x30323336, 0x30333336, 0x30343336, 0x30353336, 0x30363336, 0x30373336, 0x30383336, 0x30393336, 0x30303436, 0x30313436, 0x30323436, 0x30333436, 0x30343436, 0x30353436, 0x30363436, 0x30373436, 0x30383436, 0x30393436, 0x30303536, 0x30313536, 0x30323536, 0x30333536, 0x30343536, 0x30353536, 0x30363536, 0x30373536, 0x30383536, 0x30393536, 0x30303636, 0x30313636, 0x30323636, 0x30333636, 0x30343636, 0x30353636, 0x30363636, 0x30373636, 0x30383636, 0x30393636, 0x30303736, 0x30313736, 0x30323736, 0x30333736, 0x30343736, 0x30353736, 0x30363736, 0x30373736, 0x30383736, 0x30393736, 0x30303836, 0x30313836, 0x30323836, 0x30333836, 0x30343836, 0x30353836, 0x30363836, 0x30373836, 0x30383836, 0x30393836, 0x30303936, 0x30313936, 0x30323936, 0x30333936, 0x30343936, 0x30353936, 0x30363936, 0x30373936, 0x30383936, 0x30393936, 0x30303037, 0x30313037, 0x30323037, 0x30333037, 0x30343037, 0x30353037, 0x30363037, 0x30373037, 0x30383037, 0x30393037, 0x30303137, 0x30313137, 0x30323137, 0x30333137, 0x30343137, 0x30353137, 0x30363137, 0x30373137, 0x30383137, 0x30393137, 0x30303237, 0x30313237, 0x30323237, 0x30333237, 0x30343237, 0x30353237, 0x30363237, 0x30373237, 0x30383237, 0x30393237, 0x30303337, 0x30313337, 0x30323337, 0x30333337, 0x30343337, 0x30353337, 0x30363337, 0x30373337, 0x30383337, 0x30393337, 0x30303437, 0x30313437, 0x30323437, 0x30333437, 0x30343437, 0x30353437, 0x30363437, 0x30373437, 0x30383437, 0x30393437, 0x30303537, 0x30313537, 0x30323537, 0x30333537, 0x30343537, 0x30353537, 0x30363537, 0x30373537, 0x30383537, 0x30393537, 0x30303637, 0x30313637, 0x30323637, 0x30333637, 0x30343637, 0x30353637, 0x30363637, 0x30373637, 0x30383637, 0x30393637, 0x30303737, 0x30313737, 0x30323737, 0x30333737, 0x30343737, 0x30353737, 0x30363737, 0x30373737, 0x30383737, 0x30393737, 0x30303837, 0x30313837, 0x30323837, 0x30333837, 0x30343837, 0x30353837, 0x30363837, 0x30373837, 0x30383837, 0x30393837, 0x30303937, 0x30313937, 0x30323937, 0x30333937, 0x30343937, 0x30353937, 0x30363937, 0x30373937, 0x30383937, 0x30393937, 0x30303038, 0x30313038, 0x30323038, 0x30333038, 0x30343038, 0x30353038, 0x30363038, 0x30373038, 0x30383038, 0x30393038, 0x30303138, 0x30313138, 0x30323138, 0x30333138, 0x30343138, 0x30353138, 0x30363138, 0x30373138, 0x30383138, 0x30393138, 0x30303238, 0x30313238, 0x30323238, 0x30333238, 0x30343238, 0x30353238, 0x30363238, 0x30373238, 0x30383238, 0x30393238, 0x30303338, 0x30313338, 0x30323338, 0x30333338, 0x30343338, 0x30353338, 0x30363338, 0x30373338, 0x30383338, 0x30393338, 0x30303438, 0x30313438, 0x30323438, 0x30333438, 0x30343438, 0x30353438, 0x30363438, 0x30373438, 0x30383438, 0x30393438, 0x30303538, 0x30313538, 0x30323538, 0x30333538, 0x30343538, 0x30353538, 0x30363538, 0x30373538, 0x30383538, 0x30393538, 0x30303638, 0x30313638, 0x30323638, 0x30333638, 0x30343638, 0x30353638, 0x30363638, 0x30373638, 0x30383638, 0x30393638, 0x30303738, 0x30313738, 0x30323738, 0x30333738, 0x30343738, 0x30353738, 0x30363738, 0x30373738, 0x30383738, 0x30393738, 0x30303838, 0x30313838, 0x30323838, 0x30333838, 0x30343838, 0x30353838, 0x30363838, 0x30373838, 0x30383838, 0x30393838, 0x30303938, 0x30313938, 0x30323938, 0x30333938, 0x30343938, 0x30353938, 0x30363938, 0x30373938, 0x30383938, 0x30393938, 0x30303039, 0x30313039, 0x30323039, 0x30333039, 0x30343039, 0x30353039, 0x30363039, 0x30373039, 0x30383039, 0x30393039, 0x30303139, 0x30313139, 0x30323139, 0x30333139, 0x30343139, 0x30353139, 0x30363139, 0x30373139, 0x30383139, 0x30393139, 0x30303239, 0x30313239, 0x30323239, 0x30333239, 0x30343239, 0x30353239, 0x30363239, 0x30373239, 0x30383239, 0x30393239, 0x30303339, 0x30313339, 0x30323339, 0x30333339, 0x30343339, 0x30353339, 0x30363339, 0x30373339, 0x30383339, 0x30393339, 0x30303439, 0x30313439, 0x30323439, 0x30333439, 0x30343439, 0x30353439, 0x30363439, 0x30373439, 0x30383439, 0x30393439, 0x30303539, 0x30313539, 0x30323539, 0x30333539, 0x30343539, 0x30353539, 0x30363539, 0x30373539, 0x30383539, 0x30393539, 0x30303639, 0x30313639, 0x30323639, 0x30333639, 0x30343639, 0x30353639, 0x30363639, 0x30373639, 0x30383639, 0x30393639, 0x30303739, 0x30313739, 0x30323739, 0x30333739, 0x30343739, 0x30353739, 0x30363739, 0x30373739, 0x30383739, 0x30393739, 0x30303839, 0x30313839, 0x30323839, 0x30333839, 0x30343839, 0x30353839, 0x30363839, 0x30373839, 0x30383839, 0x30393839, 0x30303939, 0x30313939, 0x30323939, 0x30333939, 0x30343939, 0x30353939, 0x30363939, 0x30373939, 0x30383939, 0x30393939};
const unsigned int digit_00_99[100] = {0x302e30, 0x312e30, 0x322e30, 0x332e30, 0x342e30, 0x352e30, 0x362e30, 0x372e30, 0x382e30, 0x392e30, 0x302e31, 0x312e31, 0x322e31, 0x332e31, 0x342e31, 0x352e31, 0x362e31, 0x372e31, 0x382e31, 0x392e31, 0x302e32, 0x312e32, 0x322e32, 0x332e32, 0x342e32, 0x352e32, 0x362e32, 0x372e32, 0x382e32, 0x392e32, 0x302e33, 0x312e33, 0x322e33, 0x332e33, 0x342e33, 0x352e33, 0x362e33, 0x372e33, 0x382e33, 0x392e33, 0x302e34, 0x312e34, 0x322e34, 0x332e34, 0x342e34, 0x352e34, 0x362e34, 0x372e34, 0x382e34, 0x392e34, 0x302e35, 0x312e35, 0x322e35, 0x332e35, 0x342e35, 0x352e35, 0x362e35, 0x372e35, 0x382e35, 0x392e35, 0x302e36, 0x312e36, 0x322e36, 0x332e36, 0x342e36, 0x352e36, 0x362e36, 0x372e36, 0x382e36, 0x392e36, 0x302e37, 0x312e37, 0x322e37, 0x332e37, 0x342e37, 0x352e37, 0x362e37, 0x372e37, 0x382e37, 0x392e37, 0x302e38, 0x312e38, 0x322e38, 0x332e38, 0x342e38, 0x352e38, 0x362e38, 0x372e38, 0x382e38, 0x392e38, 0x302e39, 0x312e39, 0x322e39, 0x332e39, 0x342e39, 0x352e39, 0x362e39, 0x372e39, 0x382e39, 0x392e39};
const unsigned int digit_000_999[1000] = {808463920, 825241136, 842018352, 858795568, 875572784, 892350000, 909127216, 925904432, 942681648, 959458864, 808529456, 825306672, 842083888, 858861104, 875638320, 892415536, 909192752, 925969968, 942747184, 959524400, 808594992, 825372208, 842149424, 858926640, 875703856, 892481072, 909258288, 926035504, 942812720, 959589936, 808660528, 825437744, 842214960, 858992176, 875769392, 892546608, 909323824, 926101040, 942878256, 959655472, 808726064, 825503280, 842280496, 859057712, 875834928, 892612144, 909389360, 926166576, 942943792, 959721008, 808791600, 825568816, 842346032, 859123248, 875900464, 892677680, 909454896, 926232112, 943009328, 959786544, 808857136, 825634352, 842411568, 859188784, 875966000, 892743216, 909520432, 926297648, 943074864, 959852080, 808922672, 825699888, 842477104, 859254320, 876031536, 892808752, 909585968, 926363184, 943140400, 959917616, 808988208, 825765424, 842542640, 859319856, 876097072, 892874288, 909651504, 926428720, 943205936, 959983152, 809053744, 825830960, 842608176, 859385392, 876162608, 892939824, 909717040, 926494256, 943271472, 960048688, 808463921, 825241137, 842018353, 858795569, 875572785, 892350001, 909127217, 925904433, 942681649, 959458865, 808529457, 825306673, 842083889, 858861105, 875638321, 892415537, 909192753, 925969969, 942747185, 959524401, 808594993, 825372209, 842149425, 858926641, 875703857, 892481073, 909258289, 926035505, 942812721, 959589937, 808660529, 825437745, 842214961, 858992177, 875769393, 892546609, 909323825, 926101041, 942878257, 959655473, 808726065, 825503281, 842280497, 859057713, 875834929, 892612145, 909389361, 926166577, 942943793, 959721009, 808791601, 825568817, 842346033, 859123249, 875900465, 892677681, 909454897, 926232113, 943009329, 959786545, 808857137, 825634353, 842411569, 859188785, 875966001, 892743217, 909520433, 926297649, 943074865, 959852081, 808922673, 825699889, 842477105, 859254321, 876031537, 892808753, 909585969, 926363185, 943140401, 959917617, 808988209, 825765425, 842542641, 859319857, 876097073, 892874289, 909651505, 926428721, 943205937, 959983153, 809053745, 825830961, 842608177, 859385393, 876162609, 892939825, 909717041, 926494257, 943271473, 960048689, 808463922, 825241138, 842018354, 858795570, 875572786, 892350002, 909127218, 925904434, 942681650, 959458866, 808529458, 825306674, 842083890, 858861106, 875638322, 892415538, 909192754, 925969970, 942747186, 959524402, 808594994, 825372210, 842149426, 858926642, 875703858, 892481074, 909258290, 926035506, 942812722, 959589938, 808660530, 825437746, 842214962, 858992178, 875769394, 892546610, 909323826, 926101042, 942878258, 959655474, 808726066, 825503282, 842280498, 859057714, 875834930, 892612146, 909389362, 926166578, 942943794, 959721010, 808791602, 825568818, 842346034, 859123250, 875900466, 892677682, 909454898, 926232114, 943009330, 959786546, 808857138, 825634354, 842411570, 859188786, 875966002, 892743218, 909520434, 926297650, 943074866, 959852082, 808922674, 825699890, 842477106, 859254322, 876031538, 892808754, 909585970, 926363186, 943140402, 959917618, 808988210, 825765426, 842542642, 859319858, 876097074, 892874290, 909651506, 926428722, 943205938, 959983154, 809053746, 825830962, 842608178, 859385394, 876162610, 892939826, 909717042, 926494258, 943271474, 960048690, 808463923, 825241139, 842018355, 858795571, 875572787, 892350003, 909127219, 925904435, 942681651, 959458867, 808529459, 825306675, 842083891, 858861107, 875638323, 892415539, 909192755, 925969971, 942747187, 959524403, 808594995, 825372211, 842149427, 858926643, 875703859, 892481075, 909258291, 926035507, 942812723, 959589939, 808660531, 825437747, 842214963, 858992179, 875769395, 892546611, 909323827, 926101043, 942878259, 959655475, 808726067, 825503283, 842280499, 859057715, 875834931, 892612147, 909389363, 926166579, 942943795, 959721011, 808791603, 825568819, 842346035, 859123251, 875900467, 892677683, 909454899, 926232115, 943009331, 959786547, 808857139, 825634355, 842411571, 859188787, 875966003, 892743219, 909520435, 926297651, 943074867, 959852083, 808922675, 825699891, 842477107, 859254323, 876031539, 892808755, 909585971, 926363187, 943140403, 959917619, 808988211, 825765427, 842542643, 859319859, 876097075, 892874291, 909651507, 926428723, 943205939, 959983155, 809053747, 825830963, 842608179, 859385395, 876162611, 892939827, 909717043, 926494259, 943271475, 960048691, 808463924, 825241140, 842018356, 858795572, 875572788, 892350004, 909127220, 925904436, 942681652, 959458868, 808529460, 825306676, 842083892, 858861108, 875638324, 892415540, 909192756, 925969972, 942747188, 959524404, 808594996, 825372212, 842149428, 858926644, 875703860, 892481076, 909258292, 926035508, 942812724, 959589940, 808660532, 825437748, 842214964, 858992180, 875769396, 892546612, 909323828, 926101044, 942878260, 959655476, 808726068, 825503284, 842280500, 859057716, 875834932, 892612148, 909389364, 926166580, 942943796, 959721012, 808791604, 825568820, 842346036, 859123252, 875900468, 892677684, 909454900, 926232116, 943009332, 959786548, 808857140, 825634356, 842411572, 859188788, 875966004, 892743220, 909520436, 926297652, 943074868, 959852084, 808922676, 825699892, 842477108, 859254324, 876031540, 892808756, 909585972, 926363188, 943140404, 959917620, 808988212, 825765428, 842542644, 859319860, 876097076, 892874292, 909651508, 926428724, 943205940, 959983156, 809053748, 825830964, 842608180, 859385396, 876162612, 892939828, 909717044, 926494260, 943271476, 960048692, 808463925, 825241141, 842018357, 858795573, 875572789, 892350005, 909127221, 925904437, 942681653, 959458869, 808529461, 825306677, 842083893, 858861109, 875638325, 892415541, 909192757, 925969973, 942747189, 959524405, 808594997, 825372213, 842149429, 858926645, 875703861, 892481077, 909258293, 926035509, 942812725, 959589941, 808660533, 825437749, 842214965, 858992181, 875769397, 892546613, 909323829, 926101045, 942878261, 959655477, 808726069, 825503285, 842280501, 859057717, 875834933, 892612149, 909389365, 926166581, 942943797, 959721013, 808791605, 825568821, 842346037, 859123253, 875900469, 892677685, 909454901, 926232117, 943009333, 959786549, 808857141, 825634357, 842411573, 859188789, 875966005, 892743221, 909520437, 926297653, 943074869, 959852085, 808922677, 825699893, 842477109, 859254325, 876031541, 892808757, 909585973, 926363189, 943140405, 959917621, 808988213, 825765429, 842542645, 859319861, 876097077, 892874293, 909651509, 926428725, 943205941, 959983157, 809053749, 825830965, 842608181, 859385397, 876162613, 892939829, 909717045, 926494261, 943271477, 960048693, 808463926, 825241142, 842018358, 858795574, 875572790, 892350006, 909127222, 925904438, 942681654, 959458870, 808529462, 825306678, 842083894, 858861110, 875638326, 892415542, 909192758, 925969974, 942747190, 959524406, 808594998, 825372214, 842149430, 858926646, 875703862, 892481078, 909258294, 926035510, 942812726, 959589942, 808660534, 825437750, 842214966, 858992182, 875769398, 892546614, 909323830, 926101046, 942878262, 959655478, 808726070, 825503286, 842280502, 859057718, 875834934, 892612150, 909389366, 926166582, 942943798, 959721014, 808791606, 825568822, 842346038, 859123254, 875900470, 892677686, 909454902, 926232118, 943009334, 959786550, 808857142, 825634358, 842411574, 859188790, 875966006, 892743222, 909520438, 926297654, 943074870, 959852086, 808922678, 825699894, 842477110, 859254326, 876031542, 892808758, 909585974, 926363190, 943140406, 959917622, 808988214, 825765430, 842542646, 859319862, 876097078, 892874294, 909651510, 926428726, 943205942, 959983158, 809053750, 825830966, 842608182, 859385398, 876162614, 892939830, 909717046, 926494262, 943271478, 960048694, 808463927, 825241143, 842018359, 858795575, 875572791, 892350007, 909127223, 925904439, 942681655, 959458871, 808529463, 825306679, 842083895, 858861111, 875638327, 892415543, 909192759, 925969975, 942747191, 959524407, 808594999, 825372215, 842149431, 858926647, 875703863, 892481079, 909258295, 926035511, 942812727, 959589943, 808660535, 825437751, 842214967, 858992183, 875769399, 892546615, 909323831, 926101047, 942878263, 959655479, 808726071, 825503287, 842280503, 859057719, 875834935, 892612151, 909389367, 926166583, 942943799, 959721015, 808791607, 825568823, 842346039, 859123255, 875900471, 892677687, 909454903, 926232119, 943009335, 959786551, 808857143, 825634359, 842411575, 859188791, 875966007, 892743223, 909520439, 926297655, 943074871, 959852087, 808922679, 825699895, 842477111, 859254327, 876031543, 892808759, 909585975, 926363191, 943140407, 959917623, 808988215, 825765431, 842542647, 859319863, 876097079, 892874295, 909651511, 926428727, 943205943, 959983159, 809053751, 825830967, 842608183, 859385399, 876162615, 892939831, 909717047, 926494263, 943271479, 960048695, 808463928, 825241144, 842018360, 858795576, 875572792, 892350008, 909127224, 925904440, 942681656, 959458872, 808529464, 825306680, 842083896, 858861112, 875638328, 892415544, 909192760, 925969976, 942747192, 959524408, 808595000, 825372216, 842149432, 858926648, 875703864, 892481080, 909258296, 926035512, 942812728, 959589944, 808660536, 825437752, 842214968, 858992184, 875769400, 892546616, 909323832, 926101048, 942878264, 959655480, 808726072, 825503288, 842280504, 859057720, 875834936, 892612152, 909389368, 926166584, 942943800, 959721016, 808791608, 825568824, 842346040, 859123256, 875900472, 892677688, 909454904, 926232120, 943009336, 959786552, 808857144, 825634360, 842411576, 859188792, 875966008, 892743224, 909520440, 926297656, 943074872, 959852088, 808922680, 825699896, 842477112, 859254328, 876031544, 892808760, 909585976, 926363192, 943140408, 959917624, 808988216, 825765432, 842542648, 859319864, 876097080, 892874296, 909651512, 926428728, 943205944, 959983160, 809053752, 825830968, 842608184, 859385400, 876162616, 892939832, 909717048, 926494264, 943271480, 960048696, 808463929, 825241145, 842018361, 858795577, 875572793, 892350009, 909127225, 925904441, 942681657, 959458873, 808529465, 825306681, 842083897, 858861113, 875638329, 892415545, 909192761, 925969977, 942747193, 959524409, 808595001, 825372217, 842149433, 858926649, 875703865, 892481081, 909258297, 926035513, 942812729, 959589945, 808660537, 825437753, 842214969, 858992185, 875769401, 892546617, 909323833, 926101049, 942878265, 959655481, 808726073, 825503289, 842280505, 859057721, 875834937, 892612153, 909389369, 926166585, 942943801, 959721017, 808791609, 825568825, 842346041, 859123257, 875900473, 892677689, 909454905, 926232121, 943009337, 959786553, 808857145, 825634361, 842411577, 859188793, 875966009, 892743225, 909520441, 926297657, 943074873, 959852089, 808922681, 825699897, 842477113, 859254329, 876031545, 892808761, 909585977, 926363193, 943140409, 959917625, 808988217, 825765433, 842542649, 859319865, 876097081, 892874297, 909651513, 926428729, 943205945, 959983161, 809053753, 825830969, 842608185, 859385401, 876162617, 892939833, 909717049, 926494265, 943271481, 960048697};
static u64 exp_result[308 + 324 + 1] = {0x53432332d65, 0x53332332d65, 0x53232332d65, 0x53132332d65, 0x53032332d65, 0x53931332d65, 0x53831332d65, 0x53731332d65, 0x53631332d65, 0x53531332d65, 0x53431332d65, 0x53331332d65, 0x53231332d65, 0x53131332d65, 0x53031332d65, 0x53930332d65, 0x53830332d65, 0x53730332d65, 0x53630332d65, 0x53530332d65, 0x53430332d65, 0x53330332d65, 0x53230332d65, 0x53130332d65, 0x53030332d65, 0x53939322d65, 0x53839322d65, 0x53739322d65, 0x53639322d65, 0x53539322d65, 0x53439322d65, 0x53339322d65, 0x53239322d65, 0x53139322d65, 0x53039322d65, 0x53938322d65, 0x53838322d65, 0x53738322d65, 0x53638322d65, 0x53538322d65, 0x53438322d65, 0x53338322d65, 0x53238322d65, 0x53138322d65, 0x53038322d65, 0x53937322d65, 0x53837322d65, 0x53737322d65, 0x53637322d65, 0x53537322d65, 0x53437322d65, 0x53337322d65, 0x53237322d65, 0x53137322d65, 0x53037322d65, 0x53936322d65, 0x53836322d65, 0x53736322d65, 0x53636322d65, 0x53536322d65, 0x53436322d65, 0x53336322d65, 0x53236322d65, 0x53136322d65, 0x53036322d65, 0x53935322d65, 0x53835322d65, 0x53735322d65, 0x53635322d65, 0x53535322d65, 0x53435322d65, 0x53335322d65, 0x53235322d65, 0x53135322d65, 0x53035322d65, 0x53934322d65, 0x53834322d65, 0x53734322d65, 0x53634322d65, 0x53534322d65, 0x53434322d65, 0x53334322d65, 0x53234322d65, 0x53134322d65, 0x53034322d65, 0x53933322d65, 0x53833322d65, 0x53733322d65, 0x53633322d65, 0x53533322d65, 0x53433322d65, 0x53333322d65, 0x53233322d65, 0x53133322d65, 0x53033322d65, 0x53932322d65, 0x53832322d65, 0x53732322d65, 0x53632322d65, 0x53532322d65, 0x53432322d65, 0x53332322d65, 0x53232322d65, 0x53132322d65, 0x53032322d65, 0x53931322d65, 0x53831322d65, 0x53731322d65, 0x53631322d65, 0x53531322d65, 0x53431322d65, 0x53331322d65, 0x53231322d65, 0x53131322d65, 0x53031322d65, 0x53930322d65, 0x53830322d65, 0x53730322d65, 0x53630322d65, 0x53530322d65, 0x53430322d65, 0x53330322d65, 0x53230322d65, 0x53130322d65, 0x53030322d65, 0x53939312d65, 0x53839312d65, 0x53739312d65, 0x53639312d65, 0x53539312d65, 0x53439312d65, 0x53339312d65, 0x53239312d65, 0x53139312d65, 0x53039312d65, 0x53938312d65, 0x53838312d65, 0x53738312d65, 0x53638312d65, 0x53538312d65, 0x53438312d65, 0x53338312d65, 0x53238312d65, 0x53138312d65, 0x53038312d65, 0x53937312d65, 0x53837312d65, 0x53737312d65, 0x53637312d65, 0x53537312d65, 0x53437312d65, 0x53337312d65, 0x53237312d65, 0x53137312d65, 0x53037312d65, 0x53936312d65, 0x53836312d65, 0x53736312d65, 0x53636312d65, 0x53536312d65, 0x53436312d65, 0x53336312d65, 0x53236312d65, 0x53136312d65, 0x53036312d65, 0x53935312d65, 0x53835312d65, 0x53735312d65, 0x53635312d65, 0x53535312d65, 0x53435312d65, 0x53335312d65, 0x53235312d65, 0x53135312d65, 0x53035312d65, 0x53934312d65, 0x53834312d65, 0x53734312d65, 0x53634312d65, 0x53534312d65, 0x53434312d65, 0x53334312d65, 0x53234312d65, 0x53134312d65, 0x53034312d65, 0x53933312d65, 0x53833312d65, 0x53733312d65, 0x53633312d65, 0x53533312d65, 0x53433312d65, 0x53333312d65, 0x53233312d65, 0x53133312d65, 0x53033312d65, 0x53932312d65, 0x53832312d65, 0x53732312d65, 0x53632312d65, 0x53532312d65, 0x53432312d65, 0x53332312d65, 0x53232312d65, 0x53132312d65, 0x53032312d65, 0x53931312d65, 0x53831312d65, 0x53731312d65, 0x53631312d65, 0x53531312d65, 0x53431312d65, 0x53331312d65, 0x53231312d65, 0x53131312d65, 0x53031312d65, 0x53930312d65, 0x53830312d65, 0x53730312d65, 0x53630312d65, 0x53530312d65, 0x53430312d65, 0x53330312d65, 0x53230312d65, 0x53130312d65, 0x53030312d65, 0x40039392d65, 0x40038392d65, 0x40037392d65, 0x40036392d65, 0x40035392d65, 0x40034392d65, 0x40033392d65, 0x40032392d65, 0x40031392d65, 0x40030392d65, 0x40039382d65, 0x40038382d65, 0x40037382d65, 0x40036382d65, 0x40035382d65, 0x40034382d65, 0x40033382d65, 0x40032382d65, 0x40031382d65, 0x40030382d65, 0x40039372d65, 0x40038372d65, 0x40037372d65, 0x40036372d65, 0x40035372d65, 0x40034372d65, 0x40033372d65, 0x40032372d65, 0x40031372d65, 0x40030372d65, 0x40039362d65, 0x40038362d65, 0x40037362d65, 0x40036362d65, 0x40035362d65, 0x40034362d65, 0x40033362d65, 0x40032362d65, 0x40031362d65, 0x40030362d65, 0x40039352d65, 0x40038352d65, 0x40037352d65, 0x40036352d65, 0x40035352d65, 0x40034352d65, 0x40033352d65, 0x40032352d65, 0x40031352d65, 0x40030352d65, 0x40039342d65, 0x40038342d65, 0x40037342d65, 0x40036342d65, 0x40035342d65, 0x40034342d65, 0x40033342d65, 0x40032342d65, 0x40031342d65, 0x40030342d65, 0x40039332d65, 0x40038332d65, 0x40037332d65, 0x40036332d65, 0x40035332d65, 0x40034332d65, 0x40033332d65, 0x40032332d65, 0x40031332d65, 0x40030332d65, 0x40039322d65, 0x40038322d65, 0x40037322d65, 0x40036322d65, 0x40035322d65, 0x40034322d65, 0x40033322d65, 0x40032322d65, 0x40031322d65, 0x40030322d65, 0x40039312d65, 0x40038312d65, 0x40037312d65, 0x40036312d65, 0x40035312d65, 0x40034312d65, 0x40033312d65, 0x40032312d65, 0x40031312d65, 0x40030312d65, 0x30000392d65, 0x30000382d65, 0x30000372d65, 0x30000362d65, 0x30000352d65, 0x30000342d65, 0x30000332d65, 0x30000322d65, 0x30000312d65, 0x20000003065, 0x20000003165, 0x20000003265, 0x20000003365, 0x20000003465, 0x20000003565, 0x20000003665, 0x20000003765, 0x20000003865, 0x20000003965, 0x30000303165, 0x30000313165, 0x30000323165, 0x30000333165, 0x30000343165, 0x30000353165, 0x30000363165, 0x30000373165, 0x30000383165, 0x30000393165, 0x30000303265, 0x30000313265, 0x30000323265, 0x30000333265, 0x30000343265, 0x30000353265, 0x30000363265, 0x30000373265, 0x30000383265, 0x30000393265, 0x30000303365, 0x30000313365, 0x30000323365, 0x30000333365, 0x30000343365, 0x30000353365, 0x30000363365, 0x30000373365, 0x30000383365, 0x30000393365, 0x30000303465, 0x30000313465, 0x30000323465, 0x30000333465, 0x30000343465, 0x30000353465, 0x30000363465, 0x30000373465, 0x30000383465, 0x30000393465, 0x30000303565, 0x30000313565, 0x30000323565, 0x30000333565, 0x30000343565, 0x30000353565, 0x30000363565, 0x30000373565, 0x30000383565, 0x30000393565, 0x30000303665, 0x30000313665, 0x30000323665, 0x30000333665, 0x30000343665, 0x30000353665, 0x30000363665, 0x30000373665, 0x30000383665, 0x30000393665, 0x30000303765, 0x30000313765, 0x30000323765, 0x30000333765, 0x30000343765, 0x30000353765, 0x30000363765, 0x30000373765, 0x30000383765, 0x30000393765, 0x30000303865, 0x30000313865, 0x30000323865, 0x30000333865, 0x30000343865, 0x30000353865, 0x30000363865, 0x30000373865, 0x30000383865, 0x30000393865, 0x30000303965, 0x30000313965, 0x30000323965, 0x30000333965, 0x30000343965, 0x30000353965, 0x30000363965, 0x30000373965, 0x30000383965, 0x30000393965, 0x40030303165, 0x40031303165, 0x40032303165, 0x40033303165, 0x40034303165, 0x40035303165, 0x40036303165, 0x40037303165, 0x40038303165, 0x40039303165, 0x40030313165, 0x40031313165, 0x40032313165, 0x40033313165, 0x40034313165, 0x40035313165, 0x40036313165, 0x40037313165, 0x40038313165, 0x40039313165, 0x40030323165, 0x40031323165, 0x40032323165, 0x40033323165, 0x40034323165, 0x40035323165, 0x40036323165, 0x40037323165, 0x40038323165, 0x40039323165, 0x40030333165, 0x40031333165, 0x40032333165, 0x40033333165, 0x40034333165, 0x40035333165, 0x40036333165, 0x40037333165, 0x40038333165, 0x40039333165, 0x40030343165, 0x40031343165, 0x40032343165, 0x40033343165, 0x40034343165, 0x40035343165, 0x40036343165, 0x40037343165, 0x40038343165, 0x40039343165, 0x40030353165, 0x40031353165, 0x40032353165, 0x40033353165, 0x40034353165, 0x40035353165, 0x40036353165, 0x40037353165, 0x40038353165, 0x40039353165, 0x40030363165, 0x40031363165, 0x40032363165, 0x40033363165, 0x40034363165, 0x40035363165, 0x40036363165, 0x40037363165, 0x40038363165, 0x40039363165, 0x40030373165, 0x40031373165, 0x40032373165, 0x40033373165, 0x40034373165, 0x40035373165, 0x40036373165, 0x40037373165, 0x40038373165, 0x40039373165, 0x40030383165, 0x40031383165, 0x40032383165, 0x40033383165, 0x40034383165, 0x40035383165, 0x40036383165, 0x40037383165, 0x40038383165, 0x40039383165, 0x40030393165, 0x40031393165, 0x40032393165, 0x40033393165, 0x40034393165, 0x40035393165, 0x40036393165, 0x40037393165, 0x40038393165, 0x40039393165, 0x40030303265, 0x40031303265, 0x40032303265, 0x40033303265, 0x40034303265, 0x40035303265, 0x40036303265, 0x40037303265, 0x40038303265, 0x40039303265, 0x40030313265, 0x40031313265, 0x40032313265, 0x40033313265, 0x40034313265, 0x40035313265, 0x40036313265, 0x40037313265, 0x40038313265, 0x40039313265, 0x40030323265, 0x40031323265, 0x40032323265, 0x40033323265, 0x40034323265, 0x40035323265, 0x40036323265, 0x40037323265, 0x40038323265, 0x40039323265, 0x40030333265, 0x40031333265, 0x40032333265, 0x40033333265, 0x40034333265, 0x40035333265, 0x40036333265, 0x40037333265, 0x40038333265, 0x40039333265, 0x40030343265, 0x40031343265, 0x40032343265, 0x40033343265, 0x40034343265, 0x40035343265, 0x40036343265, 0x40037343265, 0x40038343265, 0x40039343265, 0x40030353265, 0x40031353265, 0x40032353265, 0x40033353265, 0x40034353265, 0x40035353265, 0x40036353265, 0x40037353265, 0x40038353265, 0x40039353265, 0x40030363265, 0x40031363265, 0x40032363265, 0x40033363265, 0x40034363265, 0x40035363265, 0x40036363265, 0x40037363265, 0x40038363265, 0x40039363265, 0x40030373265, 0x40031373265, 0x40032373265, 0x40033373265, 0x40034373265, 0x40035373265, 0x40036373265, 0x40037373265, 0x40038373265, 0x40039373265, 0x40030383265, 0x40031383265, 0x40032383265, 0x40033383265, 0x40034383265, 0x40035383265, 0x40036383265, 0x40037383265, 0x40038383265, 0x40039383265, 0x40030393265, 0x40031393265, 0x40032393265, 0x40033393265, 0x40034393265, 0x40035393265, 0x40036393265, 0x40037393265, 0x40038393265, 0x40039393265, 0x40030303365, 0x40031303365, 0x40032303365, 0x40033303365, 0x40034303365, 0x40035303365, 0x40036303365, 0x40037303365, 0x40038303365}; // 5064 byte
const unsigned short digit100[100] = {12336, 12592, 12848, 13104, 13360, 13616, 13872, 14128, 14384, 14640,
                                      12337, 12593, 12849, 13105, 13361, 13617, 13873, 14129, 14385, 14641,
                                      12338, 12594, 12850, 13106, 13362, 13618, 13874, 14130, 14386, 14642,
                                      12339, 12595, 12851, 13107, 13363, 13619, 13875, 14131, 14387, 14643,
                                      12340, 12596, 12852, 13108, 13364, 13620, 13876, 14132, 14388, 14644,
                                      12341, 12597, 12853, 13109, 13365, 13621, 13877, 14133, 14389, 14645,
                                      12342, 12598, 12854, 13110, 13366, 13622, 13878, 14134, 14390, 14646,
                                      12343, 12599, 12855, 13111, 13367, 13623, 13879, 14135, 14391, 14647,
                                      12344, 12600, 12856, 13112, 13368, 13624, 13880, 14136, 14392, 14648,
                                      12345, 12601, 12857, 13113, 13369, 13625, 13881, 14137, 14393, 14649};

static inline unsigned long long dec_length(const unsigned long long v)
{
    // return value [1,16]
    return (v >= 0) +               // 0
           (v >= 10) +              // 1
           (v >= 100) +             // 2
           (v >= 1000) +            // 3
           (v >= 10000) +           // 4
           (v >= 100000) +          // 5
           (v >= 1000000) +         // 6
           (v >= 10000000) +        // 7
           (v >= 100000000) +       // 8
           (v >= 1000000000) +      // 9
           (v >= 10000000000) +     // 10
           (v >= 100000000000) +    // 11
           (v >= 1000000000000) +   // 12
           (v >= 10000000000000) +  // 13
           (v >= 100000000000000) + // 14
           (v >= 1000000000000000); // 15
}
static_inline u8 *write_u32_len_8(u32 val, u8 *buf)
{
    if (1)
    {
        u64 aa = ((u128)(val) * (18446744073710)) >> 64;
        u64 bbbccc = ((u128)(val) * (18446744073710));
        u64 bbb = ((u128)(bbbccc) * (1000)) >> 64;
        u64 ccc = ((u64)((u128)(bbbccc) * (1000)) * (u128)(1000)) >> 64;
        u64 aaw = *(u16 *)&digit_table[aa * 2];
        u64 bbbw = digit1000[bbb];
        u64 cccw = digit1000[ccc];
        u64 high8_write = ((aaw) | ((bbbw) << 16) | (cccw << 40));
        byte_copy_8(buf, &high8_write);
        return buf + 8;
    }
    /* 8 digits: aabbccdd */
    u32 aa, bb, cc, dd, aabb, ccdd;
    aabb = (u32)(((u64)val * 109951163) >> 40); /* (val / 10000) */
    ccdd = val - aabb * 10000;                  /* (val % 10000) */
    aa = (aabb * 5243) >> 19;                   /* (aabb / 100) */
    cc = (ccdd * 5243) >> 19;                   /* (ccdd / 100) */
    bb = aabb - aa * 100;                       /* (aabb % 100) */
    dd = ccdd - cc * 100;                       /* (ccdd % 100) */
    byte_copy_2(buf + 0, digit_table + aa * 2);
    byte_copy_2(buf + 2, digit_table + bb * 2);
    byte_copy_2(buf + 4, digit_table + cc * 2);
    byte_copy_2(buf + 6, digit_table + dd * 2);
    return buf + 8;
}

static_inline u8 *write_u32_len_1_to_8(u32 val, u8 *buf)
{
    if (1)
    {
        u64 aa = ((u128)(val) * (18446744073710)) >> 64;
        u64 bbbccc = ((u128)(val) * (18446744073710));
        u64 bbb = ((u128)(bbbccc) * (1000)) >> 64;
        u64 ccc = ((u64)((u128)(bbbccc) * (1000)) * (u128)(1000)) >> 64;
        u64 aaw = *(u16 *)&digit_table[aa * 2];
        u64 bbbw = digit1000[bbb];
        u64 cccw = digit1000[ccc];
        u64 high8_write = ((aaw) | ((bbbw) << 16) | (cccw << 40));
        u64 h8 = high8_write & 0xff0f0f0f0f0f0f0f;
        u64 lz = __builtin_clzll(h8) >> 3;
        high8_write >>= (lz << 3);
        byte_copy_8(buf, &high8_write); // write high8/low16 digit
        return buf + 8 - lz;
        // u32 aa, bb, cc, dd, aabb, ccdd;
        // aabb = (u32)(((u64)val * 109951163) >> 40); /* (val / 10000) */
        // ccdd = val - aabb * 10000;                  /* (val % 10000) */
        // aa = (aabb * 5243) >> 19;                   /* (aabb / 100) */
        // cc = (ccdd * 5243) >> 19;                   /* (ccdd / 100) */
        // bb = aabb - aa * 100;                       /* (aabb % 100) */
        // dd = ccdd - cc * 100;                       /* (ccdd % 100) */
        // char buf_tmp[16];
        // byte_copy_2(buf_tmp + 0, digit_table + aa * 2);
        // byte_copy_2(buf_tmp + 2, digit_table + bb * 2);
        // byte_copy_2(buf_tmp + 4, digit_table + cc * 2);
        // byte_copy_2(buf_tmp + 6, digit_table + dd * 2);
        // const u64 MASK = 0x0f0f0f0f0f0f0f0full;
        // u64 num8_1 = *(u64 *)(buf_tmp + 0) & MASK;
        // u64 lz1 = __builtin_ctzll(num8_1) >> 3;
        // byte_copy_8(buf, &buf_tmp[lz1]);
        // return buf + 8 - lz1;
    }

    u32 aa, bb, cc, dd, aabb, bbcc, ccdd, lz;

    if (val < 100)
    { /* 1-2 digits: aa */
        lz = val < 10;
        byte_copy_2(buf + 0, digit_table + val * 2 + lz);
        buf -= lz;
        return buf + 2;
    }
    else if (val < 10000)
    {                            /* 3-4 digits: aabb */
        aa = (val * 5243) >> 19; /* (val / 100) */
        bb = val - aa * 100;     /* (val % 100) */
        lz = aa < 10;
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        return buf + 4;
    }
    else if (val < 1000000)
    {                                          /* 5-6 digits: aabbcc */
        aa = (u32)(((u64)val * 429497) >> 32); /* (val / 10000) */
        bbcc = val - aa * 10000;               /* (val % 10000) */
        bb = (bbcc * 5243) >> 19;              /* (bbcc / 100) */
        cc = bbcc - bb * 100;                  /* (bbcc % 100) */
        lz = aa < 10;
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        byte_copy_2(buf + 4, digit_table + cc * 2);
        return buf + 6;
    }
    else
    {                                               /* 7-8 digits: aabbccdd */
        aabb = (u32)(((u64)val * 109951163) >> 40); /* (val / 10000) */
        ccdd = val - aabb * 10000;                  /* (val % 10000) */
        aa = (aabb * 5243) >> 19;                   /* (aabb / 100) */
        cc = (ccdd * 5243) >> 19;                   /* (ccdd / 100) */
        bb = aabb - aa * 100;                       /* (aabb % 100) */
        dd = ccdd - cc * 100;                       /* (ccdd % 100) */
        lz = aa < 10;
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        byte_copy_2(buf + 4, digit_table + cc * 2);
        byte_copy_2(buf + 6, digit_table + dd * 2);
        return buf + 8;
    }
}

/*==============================================================================
 * Number Writer
 *============================================================================*/

/** Trailing zero count table for number 0 to 99.
    (generate with misc/make_tables.c) */
static const u8 dec_tz_table[] = {
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/** Write an unsigned integer with a length of 1 to 16. */
static_inline u8 *write_u64_len_1_to_16(u64 val, u8 *buf)
{
    if (0)
    {
        static const u64 pow10_table[] = {
            // 0,
            (u64)(1e1),
            (u64)(1e2),
            (u64)(1e3),
            (u64)(1e4),
            (u64)(1e5),
            (u64)(1e6),
            (u64)(1e7),
            (u64)(1e8),
            (u64)(1e9),
            (u64)(1e10),
            (u64)(1e11),
            (u64)(1e12),
            (u64)(1e13),
            (u64)(1e14),
            (u64)(1e15),
            (u64)(1e16)};
        static const u8 e10_tmp_table[65] = {18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0};

        // for(int i=1;i<64;++i)e10_tmp_table[i-1]=((63-i)*78913)>>18;
        // e10_tmp_table[63]=0;
        // e10_tmp_table[64]=0;

        // static bool is_first=true;
        // if(is_first){
        //     for(int i=1;i<64;++i)//0->63
        //     {
        //         e10_tmp_table[i-1]=((63-i)*78913)>>18;
        //         //printf("%d,",e10_tmp_table[i-1]);
        //     }
        //     e10_tmp_table[63]=0;
        //     e10_tmp_table[64]=0;
        //     for(int i=0;i<65;++i){
        //         //e10_tmp_table[i]++;
        //         printf("%d,",e10_tmp_table[i]);
        //     }
        //     is_first=false;
        // }
        char buf_tmp[32]; // 16+16
        bool is_digit17 = false;
        if (0)
            if (val >= (u64)(1e16)) // digit 17
            {
                u64 a = val / (u64)(1e16);
                buf[0] = a | '0';
                buf++;
                val = val - a * (u64)(1e16);
                is_digit17 = true;
            }

        const int use_200B_lut = 0;
        const int use_4KB_lut = 1;  // L1 dcache == 32KB, use this method can faster than use_200B_lut.
        const int use_40KB_lut = 0; // L1 dcache == 64KB , may fast

        // calc sig_len
        u64 e10_tmp = e10_tmp_table[__builtin_clzll(val | 1)]; // clz=[0,64],val=0,set val=1
        // u64 e10_tmp = ((63 - __builtin_clzll(val)) * 78913) >> 18;
        u64 sig_len = e10_tmp + 1 + (val >= pow10_table[e10_tmp]);
        // u64 sig_len = is_digit17 ? 16 : e10_tmp + 1 + (val >= pow10_table[e10_tmp]);
        if (is_digit17)
            sig_len = 16;
        u64 lz = 16 - sig_len;

        if (use_200B_lut | use_40KB_lut)
        {
            u64 aabbccdd = val / (u64)(1e8);
            u64 eeffgghh = val - aabbccdd * (u64)(1e8);
            u64 aabb = (aabbccdd * 112589990685ull) >> 50; // 1e8 / 1e4
            u64 ccdd = aabbccdd - aabb * 10000;
            u64 eeff = (eeffgghh * 112589990685ull) >> 50;
            u64 gghh = eeffgghh - eeff * 10000;

            u64 aa = (aabb * 10486) >> 20;
            u64 bb = aabb - aa * 100;
            u64 cc = (ccdd * 10486) >> 20;
            u64 dd = ccdd - cc * 100;
            u64 ee = (eeff * 10486) >> 20;
            u64 ff = eeff - ee * 100;
            u64 gg = (gghh * 10486) >> 20;
            u64 hh = gghh - gg * 100;

            char *buf_tmp_ptr = buf_tmp;

            if (use_200B_lut)
            {
                byte_copy_2(buf_tmp_ptr + 0, digit_table + aa * 2);
                byte_copy_2(buf_tmp_ptr + 2, digit_table + bb * 2); // or  bb*256 - 2559*((bb*103)>>10) + 0x3030
                byte_copy_2(buf_tmp_ptr + 4, digit_table + cc * 2);
                byte_copy_2(buf_tmp_ptr + 6, digit_table + dd * 2);

                byte_copy_2(buf_tmp_ptr + 8, digit_table + ee * 2);
                byte_copy_2(buf_tmp_ptr + 10, digit_table + ff * 2);
                byte_copy_2(buf_tmp_ptr + 12, digit_table + gg * 2);
                byte_copy_2(buf_tmp_ptr + 14, digit_table + hh * 2);
            }
            else if (use_40KB_lut)
            {
                // byte_copy_4(buf_tmp_ptr + 0, &digit10000[aabb]);
                // byte_copy_4(buf_tmp_ptr + 4, &digit10000[ccdd]);
                // byte_copy_4(buf_tmp_ptr + 8, &digit10000[eeff]);
                // byte_copy_4(buf_tmp_ptr + 12, &digit10000[gghh]);

                *(u64 *)(buf_tmp_ptr + 0) = (u64)digit10000[aabb] | ((u64)digit10000[ccdd] << 32);
                *(u64 *)(buf_tmp_ptr + 8) = (u64)digit10000[eeff] | ((u64)digit10000[gghh] << 32);
            }
            byte_move_16(buf, &buf_tmp[lz]);
            return buf + sig_len;
        }
        else if (use_4KB_lut)
        {
            // use 4KB lut
            u64 aabbbccc = val / (u64)(1e8);            // 8 digit = 2+3+3
            u64 eefffggg = val - aabbbccc * (u64)(1e8); // 8 digit = 2+3+3

            // u64 aa = (aabbbccc * 1125899907) >> 50;     // aabbbccc / 1e6  ; 1125899907=ceil(2**50/1e6)
            // u64 ee = (eefffggg * 1125899907) >> 50;     // eefffggg / 1e6
            // u64 bbbccc = aabbbccc - aa * (u64)(1e6);
            // u64 fffggg = eefffggg - ee * (u64)(1e6);
            // u64 bbb = (bbbccc * 4294968) >> 32; // bbbccc / 1000  4294968=ceil(2**32/1e3)
            // u64 ccc = bbbccc - bbb * (u64)(1e3);
            // u64 fff = (fffggg * 4294968) >> 32; // fffggg / 1000
            // u64 ggg = fffggg - fff * (u64)(1e3);
            u64 aa = ((u128)(aabbbccc) * (18446744073710)) >> 64;
            u64 bbbccc = ((u128)(aabbbccc) * (18446744073710));
            u64 bbb = ((u128)(bbbccc) * (1000)) >> 64;
            u64 ccc = ((u64)((u128)(bbbccc) * (1000)) * (u128)(1000)) >> 64;
            u64 ee = ((u128)(eefffggg) * (18446744073710)) >> 64;
            u64 fffggg = ((u128)(eefffggg) * (18446744073710));
            u64 fff = ((u128)(fffggg) * (1000)) >> 64;
            u64 ggg = ((u64)((u128)(fffggg) * (1000)) * (u128)(1000)) >> 64;

            u64 aaw = *(u16 *)&digit_table[aa * 2];
            u64 bbbw = digit1000[bbb];
            u64 cccw = digit1000[ccc];
            u64 eew = *(u16 *)&digit_table[ee * 2];
            u64 fffw = digit1000[fff];
            u64 gggw = digit1000[ggg];
            u64 high8_write = ((aaw) | ((bbbw) << 16) | (cccw << 40));
            u64 low8_write = (eew | ((fffw) << 16) | (gggw << 40));

            char *buf_tmp_ptr = buf_tmp;
            // byte_copy_2(buf_tmp_ptr + 0, digit_table + aa * 2); // write 2 byte
            // byte_copy_4(buf_tmp_ptr + 2, &digit1000[bbb]);      // write 3 byte
            // byte_copy_4(buf_tmp_ptr + 5, &digit1000[ccc]);      // write 3 byte
            // byte_copy_2(buf_tmp_ptr + 8, digit_table + ee * 2); // write 2 byte
            // byte_copy_4(buf_tmp_ptr + 10, &digit1000[fff]);     // write 3 byte
            // byte_copy_4(buf_tmp_ptr + 13, &digit1000[ggg]);     // write 3 byte
            byte_copy_8(buf_tmp_ptr + 0, &high8_write); // write high8/low16 digit
            byte_copy_8(buf_tmp_ptr + 8, &low8_write);  // write low8/low16 digit
            byte_move_16(buf, &buf_tmp[lz]);
            return buf + sig_len;
        }
    }

    u64 hgh;
    u32 low;
    if (val < 100000000)
    { /* 1-8 digits */
        buf = write_u32_len_1_to_8((u32)val, buf);
        return buf;
    }
    else
    { /* 9-16 digits */
        hgh = val / 100000000;
        low = (u32)(val - hgh * 100000000); /* (val % 100000000) */
        buf = write_u32_len_1_to_8((u32)hgh, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    }
}

/** Write an unsigned integer with a length of 1 to 17. */
static_inline u8 *write_u64_len_1_to_17(u64 val, u8 *buf)
{

    if (0)
    {
        const u64 pow10_table[] = {
            // 0,
            (u64)(1e1),
            (u64)(1e2),
            (u64)(1e3),
            (u64)(1e4),
            (u64)(1e5),
            (u64)(1e6),
            (u64)(1e7),
            (u64)(1e8),
            (u64)(1e9),
            (u64)(1e10),
            (u64)(1e11),
            (u64)(1e12),
            (u64)(1e13),
            (u64)(1e14),
            (u64)(1e15),
            (u64)(1e16)};
        static u8 e10_tmp_table[65] = {18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0};

        // for(int i=1;i<64;++i)e10_tmp_table[i-1]=((63-i)*78913)>>18;
        // e10_tmp_table[63]=0;
        // e10_tmp_table[64]=0;

        // static bool is_first=true;
        // if(is_first){
        //     for(int i=1;i<64;++i)//0->63
        //     {
        //         e10_tmp_table[i-1]=((63-i)*78913)>>18;
        //         //printf("%d,",e10_tmp_table[i-1]);
        //     }
        //     e10_tmp_table[63]=0;
        //     e10_tmp_table[64]=0;
        //     for(int i=0;i<65;++i){
        //         e10_tmp_table[i]++;
        //         printf("%d,",e10_tmp_table[i]);
        //     }
        //     is_first=false;
        // }
        char buf_tmp[32]; // 16+16
        bool is_digit17 = false;
        if (val >= (u64)(1e16)) // digit 17
        {
            // printf("val = %lld\n",val);
            u64 a = val / (u64)(1e16);
            buf[0] = a + '0';
            buf++;
            val = val - a * (u64)(1e16);
            is_digit17 = true;
        }
        u64 aabbccdd = val / (u64)(1e8);
        u64 eeffgghh = val - aabbccdd * (u64)(1e8);
        u64 aabb = (aabbccdd * 112589990685ull) >> 50; // 1e8 / 1e4
        u64 ccdd = aabbccdd - aabb * 10000;
        u64 eeff = (eeffgghh * 112589990685ull) >> 50;
        u64 gghh = eeffgghh - eeff * 10000;

        // u64 e10_tmp = ((63 - __builtin_clzll(val)) * 78913) >> 18;

#if 1
        u64 aa = (aabb * 10486) >> 20;
        u64 bb = aabb - aa * 100;
        u64 cc = (ccdd * 10486) >> 20;
        u64 dd = ccdd - cc * 100;
        u64 ee = (eeff * 10486) >> 20;
        u64 ff = eeff - ee * 100;
        u64 gg = (gghh * 10486) >> 20;
        u64 hh = gghh - gg * 100;

        byte_copy_2(buf_tmp + 0, digit_table + aa * 2);
        byte_copy_2(buf_tmp + 2, digit_table + bb * 2); // or  bb*256 - 2559*((bb*103)>>10) + 0x3030
        byte_copy_2(buf_tmp + 4, digit_table + cc * 2);
        byte_copy_2(buf_tmp + 6, digit_table + dd * 2);

        byte_copy_2(buf_tmp + 8, digit_table + ee * 2);
        byte_copy_2(buf_tmp + 10, digit_table + ff * 2);
        byte_copy_2(buf_tmp + 12, digit_table + gg * 2);
        byte_copy_2(buf_tmp + 14, digit_table + hh * 2);
#else // use large lut
      // char * buf_tmp = buf;
        byte_copy_4(buf_tmp + 0, &digit10000[aabb]);
        byte_copy_4(buf_tmp + 4, &digit10000[ccdd]);
        byte_copy_4(buf_tmp + 8, &digit10000[eeff]);
        byte_copy_4(buf_tmp + 12, &digit10000[gghh]);

        // u64 e10_tmp = e10_tmp_table[__builtin_clzll(val)];// clz=[0,64]
        // //u64 e10_tmp = ((63 - __builtin_clzll(val)) * 78913) >> 18;
        // u64 sig_len = e10_tmp + 1 + (val >= pow10_table[e10_tmp]);
        // //u64 sig_len = is_digit17 ? 16 : e10_tmp + 1 + (val >= pow10_table[e10_tmp]);
        // if( is_digit17 ) sig_len = 16;
        // u64 lz = 16 - sig_len;
        // // if(is_digit17){sig_len=16;}
        // // u64 sig_len = dec_length(val);//floor(log10(val)) + 1
        // // byte_move_16(buf, &buf_tmp[0]);
        // // return buf + 16;
        // byte_move_16(buf, &buf_tmp[lz]);
        // return buf + sig_len;
#endif
        const u64 MASK1 = 0x0f0f0f0f0f0f0f0full;
        const u64 MASK2 = 0x0f0f0f0f0f0f0fffull;
        u64 num8_1 = *(u64 *)(buf_tmp + 0) & MASK1;
        u64 num8_2 = *(u64 *)(buf_tmp + 8) & MASK1;
        u64 tz1 = __builtin_clzll(num8_1) >> 3;
        u64 tz2 = __builtin_clzll(num8_2) >> 3;
        u64 lz1 = __builtin_ctzll(num8_1) >> 3;
        u64 lz2 = __builtin_ctzll(num8_2) >> 3;
        u64 tz = ((num8_2 == 0) ? (tz1 + 8) : tz2);
        u64 lz = ((num8_1 == 0) ? (8 + lz2) : lz1);
        if (is_digit17)
            lz = 0;
        byte_move_16(buf, &buf_tmp[lz]);
        return buf + 16 - lz;
        // return buf + 16 - lz - tz + is_digit17 ;
        // return buf + 17 - lz - tz ;
    }

    u64 hgh;
    u32 mid, low, one;
    if (val >= (u64)100000000 * 10000000)
    { /* len: 16 to 17 */
        hgh = val / 100000000;
        low = (u32)(val - hgh * 100000000); /* (val % 100000000) */
        one = (u32)(hgh / 100000000);
        mid = (u32)(hgh - (u64)one * 100000000); /* (hgh % 100000000) */
        *buf = (u8)one + (u8)'0';
        buf += one > 0;
        buf = write_u32_len_8(mid, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    }
    else if (val >= (u64)100000000)
    { /* len: 9 to 15 */
        hgh = val / 100000000;
        low = (u32)(val - hgh * 100000000); /* (val % 100000000) */
        buf = write_u32_len_1_to_8((u32)hgh, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    }
    else
    { /* len: 1 to 8 */
        buf = write_u32_len_1_to_8((u32)val, buf);
        return buf;
    }
}
u64 generalized_granlund_montgomery_branchless(u64 n)
{
    u64 s = 0;

    u64 r = (n * (28999941890838049));
    u64 b = r < (184467440969);
    s = s * 2 + b;
    n = b ? (r >> 8) : n;

    r = (n * (182622766329724561));
    b = r < (1844674407370971);
    s = s * 2 + b;
    n = b ? (r >> 4) : n;

    r = (n * (14941862699704736809ull));
    b = r < (184467440737095517);
    s = s * 2 + b;
    n = b ? (r >> 2) : n;

    r = (n * (5534023222112865485));
    b = r < (1844674407370955163);
    s = s * 2 + b;
    n = b ? (r >> 1) : n;

    return s;
}
/**
 Write an unsigned integer with a length of 16 or 17 with trailing zero trimmed.
 These digits are named as "abbccddeeffgghhii" here.
 For example, input 1234567890123000, output "1234567890123".
 */
static_inline u8 *write_u64_len_16_to_17_trim(u64 val, u8 *buf)
{
    // static int digit1000[1000];   // one page size, 4KB , large lut  , 000 -> 999
    static bool is_first = true;
    if (0)
        if (is_first) // init , run only once
        {
            // for(int i=0;i<1000;++i){
            //     digit1000[i] =  ( i/100 + '0') +
            //                 ( ((i%100)/10 + '0') << 8) +
            //                 ( (i%10 + '0') << 16) +
            //                 ( (int)('0') << 24);
            //     printf("0x%x,",digit1000[i]);
            // }
            // for (int i = 0; i < 10000; ++i)
            // {
            //     digit10000[i] = (i / 1000 + '0') +
            //                     (((i % 1000) / 100 + '0') << 8) +
            //                     ((i % 100 / 10 + '0') << 16) +
            //                     ((i % 100 % 10 + '0') << 24);
            //     //printf("0x%x,",digit10000[i]);
            // }
            is_first = false;
        }
    // choose one of the three.
    // Random
    // 40KB : 35.8 cycle
    // 4KB  :  38  cycle
    // 200B : 42.2 cycle
    const int use_200B_lut = 0;
    const int use_4KB_lut = 1;  // L1 dcache == 32KB, use this method can faster than use_200B_lut.
    const int use_40KB_lut = 0; // L1 dcache == 64KB , may fast
    if (use_200B_lut | use_40KB_lut)
    {
        //  1 + 8 + 8  = 17 digit
        u64 a = val / (u64)(1e16);
        u64 high9 = val / (u64)(1e8);
        buf[0] = a | '0'; // = a + '0' ;
        buf += (a != 0);
        u64 aabbccdd = high9 - a * (u64)(1e8);         // aa bb cc dd
        u64 eeffgghh = val - high9 * (u64)(1e8);       // ee ff gg hh
        u64 aabb = (aabbccdd * 112589990685ull) >> 50; // 1e8 / 1e4
        u64 ccdd = aabbccdd - aabb * 10000;
        u64 eeff = (eeffgghh * 112589990685ull) >> 50;
        u64 gghh = eeffgghh - eeff * 10000;

        u64 aa = (aabb * 10486) >> 20;
        u64 bb = aabb - aa * 100;
        u64 cc = (ccdd * 10486) >> 20;
        u64 dd = ccdd - cc * 100;
        u64 ee = (eeff * 10486) >> 20;
        u64 ff = eeff - ee * 100;
        u64 gg = (gghh * 10486) >> 20;
        u64 hh = gghh - gg * 100;

        if (use_200B_lut)
        {
            byte_copy_2(buf + 0, digit_table + aa * 2);
            byte_copy_2(buf + 2, digit_table + bb * 2); // or  bb*256 - 2559*((bb*103)>>10) + 0x3030
            byte_copy_2(buf + 4, digit_table + cc * 2);
            byte_copy_2(buf + 6, digit_table + dd * 2);

            byte_copy_2(buf + 8, digit_table + ee * 2);
            byte_copy_2(buf + 10, digit_table + ff * 2);
            byte_copy_2(buf + 12, digit_table + gg * 2);
            byte_copy_2(buf + 14, digit_table + hh * 2);
        }
        else if (use_40KB_lut)
        {
            // byte_copy_4(buf + 0, &digit10000[aabb]);
            // byte_copy_4(buf + 4, &digit10000[ccdd]);
            // byte_copy_4(buf + 8, &digit10000[eeff]);
            // byte_copy_4(buf + 12, &digit10000[gghh]);

            *(u64 *)(buf + 0) = (u64)digit10000[aabb] | ((u64)digit10000[ccdd] << 32);
            *(u64 *)(buf + 8) = (u64)digit10000[eeff] | ((u64)digit10000[gghh] << 32);
        }
        const u64 MASK = 0x0f0f0f0f0f0f0f0full;
        u64 num8_1 = *(u64 *)(buf + 0) & MASK;
        u64 num8_2 = *(u64 *)(buf + 8) & MASK;
        u64 tz1 = __builtin_clzll(num8_1) >> 3;
        u64 tz2 = __builtin_clzll(num8_2) >> 3;
        if (num8_1 == 0)
            tz1 = 8;                                // when num8_1 is 0 set tz1 = 8
        u64 tz = ((num8_2 == 0) ? (tz1 + 8) : tz2); // when num8_2 is 0 set tz2 = 8
        return buf + 16 - tz;
    }
    else if (use_4KB_lut)
    {
        // use 4KB lut
        u64 aaabbbccc = val / (u64)(1e8);            // 8 or 9 digit = 2+3+3 or 3+3+3
        u64 eefffggg = val - aaabbbccc * (u64)(1e8); // 8 digit = 2+3+3
        // u64 aaa = (aaabbbccc * 1125899907) >> 50;    // aaabbbccc / 1e6  1125899907=ceil(2**50/1e6)
        // u64 ee = (eefffggg * 1125899907) >> 50;      // eefffggg / 1e6
        // u64 bbbccc = aaabbbccc - aaa * (u64)(1e6);
        // u64 fffggg = eefffggg - ee * (u64)(1e6);
        // u64 bbb = (bbbccc * 4294968) >> 32; // bbbccc / 1000  4294968=ceil(2**32/1e3)
        // u64 ccc = bbbccc - bbb * (u64)(1e3);
        // u64 fff = (fffggg * 4294968) >> 32; // fffggg / 1000
        // u64 ggg = fffggg - fff * (u64)(1e3);

        u64 aaa = ((u128)(aaabbbccc) * (18446744073710)) >> 64;
        u32 aaa_write = (aaa >= 100) ? digit1000[aaa] : digit1000[aaa] >> 8;
        byte_copy_4(buf + 0, &aaa_write); // write 1 digit
        buf += (aaa >= 100);              // 17 digit
        u64 num456_789 = ((u128)(aaabbbccc) * (18446744073710));
        u64 bbb = ((u128)(num456_789) * (1000)) >> 64;
        u64 ccc = ((u64)((u128)(num456_789) * (1000)) * (u128)(1000)) >> 64;

        u64 ee = ((u128)(eefffggg) * (18446744073710)) >> 64;
        u64 num345_678 = ((u128)(eefffggg) * (18446744073710));
        u64 fff = ((u128)(num345_678) * (1000)) >> 64;
        u64 ggg = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;

        const u64 MASK = 0x0f0f0f0f0f0f0f0full;
        u64 aaw = digit1000[aaa] >> 8;
        u64 bbbw = digit1000[bbb];
        u64 cccw = digit1000[ccc];
        u64 eew = *(u16 *)&digit_table[ee * 2];
        u64 fffw = digit1000[fff];
        u64 gggw = digit1000[ggg];
        u64 high8_write = ((aaw) | ((bbbw) << 16) | (cccw << 40));
        u64 low8_write = (eew | ((fffw) << 16) | (gggw << 40));
        byte_copy_8(buf + 0, &high8_write); // write high8/low16 digit
        byte_copy_8(buf + 8, &low8_write);  // write low8/low16 digit
        u64 high8 = high8_write & MASK;
        u64 low8 = low8_write & MASK;
        u64 tz1 = high8 ? (__builtin_clzll(high8)) >> 3 : 8;
        u64 tz2 = __builtin_clzll(low8) >> 3;
        u64 tz = ((low8 == 0) ? (tz1 + 8) : tz2);
        return buf + 16 - tz;

        // //u64 tz = generalized_granlund_montgomery_branchless(val);
        // return buf + 16 - generalized_granlund_montgomery_branchless(val);

        // u64 high9 = val / (u64)(1e8);
        // {
        // u64 num12 = ((u128)(high9) * (18446744073710)) >> 64;
        // const char *digit1000_ptr = (char*)digit1000;
        // i32 aaa_write = *(i32 *)(digit1000_ptr + num12 * 4 + (num12 < 100)); // when aaa==999 is also right (999<100)=0
        // byte_copy_4(buf + 0, &aaa_write);           // write 2 or 3 byte
        // buf += (num12 >= 100);                        // 17 digit
        // u64 num345_678 = ((u128)(high9) * (18446744073710));
        // u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
        // *(int *)&buf[2] = digit1000[num345]; // 3
        // u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
        // *(int *)&buf[5] = digit1000[num678]; // 3
        // }
        // {
        // u64 low8 = val - high9 * (u64)(1e8);
        // u64 num12 = ((u128)(low8) * (18446744073710)) >> 64;
        // //*(short *)&buf[8] = *(short)digit_table[num12];  // 2
        // byte_copy_2(buf + 8, digit_table + num12 * 2);
        // u64 num345_678 = ((u128)(low8) * (18446744073710));
        // u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
        // *(int *)&buf[10] = digit1000[num345]; // 3
        // u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
        // *(int *)&buf[13] = digit1000[num678]; // 3
        // return buf + 16 - generalized_granlund_montgomery_branchless(val);
        // }

        // u64 num8_1 = *(u64 *)(buf + 0) & MASK;
        // u64 num8_2 = *(u64 *)(buf + 8) & MASK;
        // u64 tz1 = num8_1 ? __builtin_clzll(num8_1) >> 3:8;
        // u64 tz2 = __builtin_clzll(num8_2) >> 3;
        // u64 tz = ((num8_2 == 0) ? (tz1 + 8) : tz2); // when num8_2 is 0 set tz2 = 8
        // return buf + 16 - tz;
    }

#if 0
#if 0
    //use 4KB lut
    u64 aaabbbccc = val / (u64)(1e8);//8 or 9 digit = 2+3+3 or 3+3+3
    u64 eefffggg = val - aaabbbccc * (u64)(1e8);//8 digit = 2+3+3
    u64 aaa = (aaabbbccc * 1125899907) >> 50; //aaabbbccc / 1e6  1125899907=ceil(2**50/1e6)
    u64 ee = (eefffggg * 1125899907) >> 50;  // eefffggg / 1e6
    u64 bbbccc = aaabbbccc - aaa * (u64)(1e6);
    u64 fffggg = eefffggg - ee * (u64)(1e6);
    u64 bbb = (bbbccc * 4294968)>>32; // bbbccc / 1000  4294968=ceil(2**32/1e3)
    u64 ccc = bbbccc - bbb * (u64)(1e3);
    u64 fff = (fffggg * 4294968)>>32; // fffggg / 1000
    u64 ggg = fffggg - fff * (u64)(1e3);
    const char * digit1000_ptr = digit1000;
    int aaa_write = *(int*)(digit1000_ptr + aaa * 4 + (aaa < 100));//when aaa==999 is also right (999<100)=0
    //int aaa_write =  digit1000[aaa] >> (8 * (aaa < 100) );  // 
    //int aaa_write = (aaa>=100) ? digit1000[aaa] :  *(short*)&digit_table[aaa*2] ;  // slow
    byte_copy_4(buf + 0 , &aaa_write );//2 or 3
    buf += (aaa >= 100);// 17 digit
    //buf += (val>=(u64)(1e16));
    byte_copy_4(buf + 2 , &digit1000[bbb]);     //write 3byte
    byte_copy_4(buf + 5 , &digit1000[ccc]);     //write 3byte
    byte_copy_2(buf + 8 , digit_table + ee * 2);//write 2byte
    byte_copy_4(buf + 10 , &digit1000[fff]);    //write 3byte
    byte_copy_4(buf + 13 , &digit1000[ggg]);    //write 3byte
    const u64 MASK = 0x0f0f0f0f0f0f0f0full;
    u64 num8_1 = *(u64 *)(buf + 0) & MASK;
    u64 num8_2 = *(u64 *)(buf + 8) & MASK;
    u64 tz1 = __builtin_clzll(num8_1) >> 3;
    u64 tz2 = __builtin_clzll(num8_2) >> 3;
    if(num8_1 == 0) tz1 = 8;// when num8_1 is 0 set tz1 = 8
    u64 tz = ((num8_2 == 0) ? (tz1 + 8) : tz2);//when num8_2 is 0 set tz2 = 8
    return buf + 16 - tz;
#else
    //  1 + 8 + 8  = 17 digit
    u64 a = val / (u64)(1e16);
    u64 high9 = val / (u64)(1e8);
    // u64 low16 = val - a * (u64)(1e16);
    // u64 a = (high9*1441151880)>>57;
    buf[0] = a | '0'; // = a | '0' ;
    buf += (a != 0);
    // return buf;
    // buf++;
    u64 aabbccdd = high9 - a * (u64)(1e8);   // aa bb cc dd
    u64 eeffgghh = val - high9 * (u64)(1e8); // ee ff gg hh
    // u64 aabbccdd = low16 / (u64)(1e8);
    // u64 eeffgghh = low16 - aabbccdd*(u64)(1e8);

    // u64 aabb = aabbccdd / 10000;
    // u64 ccdd = aabbccdd % 10000;
    // u64 eeff = eeffgghh / 10000;
    // u64 gghh = eeffgghh % 10000;
    // u64 aa = aabb / 100;
    // u64 bb = aabb % 100;
    // u64 cc = ccdd / 100;
    // u64 dd = ccdd % 100;
    // u64 ee = eeff / 100;
    // u64 ff = eeff % 100;
    // u64 gg = gghh / 100;
    // u64 hh = gghh % 100;

    // 112589990685 = ceil(2**50 / 1e4);
    u64 aabb = (aabbccdd * 112589990685ull) >> 50; // 1e8 / 1e4
    u64 ccdd = aabbccdd - aabb * 10000;
    u64 eeff = (eeffgghh * 112589990685ull) >> 50;
    u64 gghh = eeffgghh - eeff * 10000;

    u64 aa = (aabb * 10486) >> 20;
    u64 bb = aabb - aa * 100;
    u64 cc = (ccdd * 10486) >> 20;
    u64 dd = ccdd - cc * 100;
    u64 ee = (eeff * 10486) >> 20;
    u64 ff = eeff - ee * 100;
    u64 gg = (gghh * 10486) >> 20;
    u64 hh = gghh - gg * 100;

    {
        u64 aa = (aabbccdd * 1125899907ull) >> 50;
        u64 bbccdd = aabbccdd - aa * (u64)(1e6);
        // u64 bbc =
    }

    // u64 aa = (aabbccdd * 1125899907ull) >> 50;
    // u64 bbccdd = aabbccdd - aa * (u64)(1e6);
    // u64 bb = (bbccdd * 109951163) >> 40;
    // u64 ccdd = bbccdd - bb * (u64)(1e4);
    // u64 cc = (ccdd * 10486) >> 20;
    // u64 dd = ccdd - cc * 100;

    // u64 ee =  (eeffgghh * 1125899907ull) >> 50;
    // u64 ffgghh = eeffgghh - ee * (u64)(1e6);
    // u64 ff = (ffgghh * 109951163) >> 40;
    // u64 gghh = ffgghh - ff * (u64)(1e4);
    // u64 gg = (gghh * 10486) >> 20;
    // u64 hh = gghh - gg * 100;

    // u32 aabb = (aabbccdd * 112589990685ull) >> 50;
    // u32 ccdd = aabbccdd - aabb * 10000;
    // u32 eeff = (eeffgghh * 112589990685ull) >> 50;
    // u32 gghh = eeffgghh - eeff * 10000;
    // u32 aa = (aabb * 10486) >> 20;
    // u32 bb = aabb - aa * 100;
    // u32 cc = (ccdd * 10486) >> 20;
    // u32 dd = ccdd - cc * 100;
    // u32 ee = (eeff * 10486) >> 20;
    // u32 ff = eeff - ee * 100;
    // u32 gg = (gghh * 10486) >> 20;
    // u32 hh = gghh - gg * 100;

    // const u64 M57 = ((u64)1<<57)-1;
    // u64 aa_bbccdd = (aabbccdd * 144115188076) ;
    // u64 aa = aa_bbccdd >> 57;
    // u64 _bbccdd = aa_bbccdd & M57;
    // u64 bb_ccdd = _bbccdd * 100;
    // u64 bb = bb_ccdd >> 57;
    // u64 _ccdd=bb_ccdd & M57;
    // u64 cc_dd = _ccdd * 100;
    // u64 cc = cc_dd >> 57;
    // u64 _dd = cc_dd & M57;
    // u64 dd = (_dd * 100) >>57;

    // u64 ee_ffgghh = (eeffgghh * 144115188076) ;
    // u64 ee = ee_ffgghh >> 57;
    // u64 _ffgghh = ee_ffgghh & M57;
    // u64 ff_gghh = _ffgghh * 100;
    // u64 ff = ff_gghh >> 57;
    // u64 _gghh = ff_gghh & M57;
    // u64 gg_hh = _gghh * 100;
    // u64 gg = gg_hh >> 57;
    // u64 _hh = gg_hh & M57;
    // u64 hh = (_hh * 100) >>57;

    // 18446744073710; ceil(2**64 / 1e6);
    //  u64 aa = ((u128)aabbccdd * (u128)(18446744073710)) >> 64 ;
    //  u64 bbccdd = ((u128)aabbccdd * (u128)(18446744073710));
    //  u64 bb = ((u128)bbccdd * (u128)(100)) >> 64;
    //  u64 ccdd = ((u128)bbccdd * (u128)(100));
    //  u64 cc = ((u128)ccdd * (u128)(100)) >> 64;
    //  u64 _dd = ((u128)ccdd * (u128)(100)) ;
    //  u64 dd = ((u128)_dd * (u128)(100)) >> 64;

    const u64 ZERO = 0x3030303030303030ull;
    const u64 MASK = 0x0f0f0f0f0f0f0f0full;

// two way
#if 0
    // u64 num8_1 = ((u64)(*(short *)&digit_table[aa * 2]) << 0) |
    //              ((u64)(*(short *)&digit_table[bb * 2]) << 16) |
    //              ((u64)(*(short *)&digit_table[cc * 2]) << 32) |
    //              ((u64)(*(short *)&digit_table[dd * 2]) << 48);
    // *(u64 *)(buf + 0) = num8_1;
    // u64 num8_2 = ((u64)(*(short *)&digit_table[ee * 2]) << 0) |
    //              ((u64)(*(short *)&digit_table[ff * 2]) << 16) |
    //              ((u64)(*(short *)&digit_table[gg * 2]) << 32) |
    //              ((u64)(*(short *)&digit_table[hh * 2]) << 48);
    // *(u64 *)(buf + 8) = num8_2;
    // num8_1 -= ZERO;
    // num8_2 -= ZERO;
    // u64 tz1 = __builtin_clzll(num8_1) >> 3;
    // u64 tz2 = __builtin_clzll(num8_2) >> 3;
#else

#if 1
    byte_copy_2(buf + 0, digit_table + aa * 2);
    byte_copy_2(buf + 2, digit_table + bb * 2); // or  bb*256 - 2559*((bb*103)>>10) + 0x3030
    byte_copy_2(buf + 4, digit_table + cc * 2);
    byte_copy_2(buf + 6, digit_table + dd * 2);

    byte_copy_2(buf + 8, digit_table + ee * 2);
    byte_copy_2(buf + 10, digit_table + ff * 2);
    byte_copy_2(buf + 12, digit_table + gg * 2);
    byte_copy_2(buf + 14, digit_table + hh * 2);
#else
    // this way fast; apple M1
    // large lut
    // int aabb_write = digit10000[aabb]|0x30303030;
    // int ccdd_write = digit10000[ccdd]|0x30303030;
    // int eeff_write = digit10000[eeff]|0x30303030;
    // int gghh_write = digit10000[gghh]|0x30303030;
    // byte_copy_4(buf + 0, &aabb_write);
    // byte_copy_4(buf + 4, &ccdd_write);
    // byte_copy_4(buf + 8, &eeff_write);
    // byte_copy_4(buf + 12, &gghh_write);
    byte_copy_4(buf + 0, &digit10000[aabb]);
    byte_copy_4(buf + 4, &digit10000[ccdd]);
    byte_copy_4(buf + 8, &digit10000[eeff]);
    byte_copy_4(buf + 12, &digit10000[gghh]);
    // return buf + 16;
    // *(u64*)(buf+0) = (u64)digit10000[aabb] | ((u64)digit10000[ccdd]<<32);
    // *(u64*)(buf+8) = (u64)digit10000[eeff] | ((u64)digit10000[gghh]<<32);
#endif
    // write finish , count tz
    // u64 num8_1 = *(u64 *)(buf + 0) - ZERO;
    // u64 num8_2 = *(u64 *)(buf + 8) - ZERO;
    u64 num8_1 = *(u64 *)(buf + 0) & MASK;
    u64 num8_2 = *(u64 *)(buf + 8) & MASK;
    u64 tz;
    // u64 num8_1 = ((u64)digit10000[aabb]  | ((u64)digit10000[ccdd]<<32)) & MASK;
    // u64 num8_2 = ((u64)digit10000[eeff]  | ((u64)digit10000[gghh]<<32)) & MASK;
    // u64 tz1 = __builtin_clzll(num8_1) >> 3;
    // u64 tz2 = __builtin_clzll(num8_2) >> 3;
    // u64 tz = ((num8_2 == 0) ? (tz1 + 8) : tz2);
    // if(__builtin_clz(digit10000[gghh]&MASK))
    // if(yy_likely(digit10000[gghh] & 0x0f0f0000))
    // {
    //     tz = (digit10000[gghh] & 0x0f000000) == 0;
    //     return buf + 16 - tz;
    // }
    // else
    {
        // u64 num8_1 = ((u64)digit10000[aabb]  | ((u64)digit10000[ccdd]<<32)) & MASK;
        // u64 num8_2 = ((u64)digit10000[eeff]  | ((u64)digit10000[gghh]<<32)) & MASK;
        u64 tz1 = __builtin_clzll(num8_1) >> 3;
        u64 tz2 = __builtin_clzll(num8_2) >> 3;
        if (num8_1 == 0)
            tz1 = 8;
        u64 tz = ((num8_2 == 0) ? (tz1 + 8) : tz2);
        return buf + 16 - tz;
    }
#endif
    // return buf + 16 ;
    // return buf + 16 - tz;
    // return buf + 16 - ctz;
#endif
#endif

#if 1
    u32 tz, tz1, tz2;

    u32 abbccddee = (u32)(val / 100000000);
    u32 ffgghhii = (u32)(val - (u64)abbccddee * 100000000);

    u32 abbcc = abbccddee / 10000;
    u32 ddee = abbccddee - abbcc * 10000;

    u32 abb = (u32)(((u64)abbcc * 167773) >> 24); /* (abbcc / 100) */
    u32 a = (abb * 41) >> 12;                     /* (abb / 100) */
    u32 bb = abb - a * 100;                       /* (abb % 100) */
    u32 cc = abbcc - abb * 100;                   /* (abbcc % 100) */
    buf[0] = (u8)(a + '0');
    buf += a > 0;
    byte_copy_2(buf + 0, digit_table + bb * 2);
    byte_copy_2(buf + 2, digit_table + cc * 2);

    if (ffgghhii)
    {
        u32 dd = (ddee * 5243) >> 19;                        /* (ddee / 100) */
        u32 ee = ddee - dd * 100;                            /* (ddee % 100) */
        u32 ffgg = (u32)(((u64)ffgghhii * 109951163) >> 40); /* (val / 10000) */
        u32 hhii = ffgghhii - ffgg * 10000;                  /* (val % 10000) */
        u32 ff = (ffgg * 5243) >> 19;                        /* (aabb / 100) */
        u32 gg = ffgg - ff * 100;                            /* (aabb % 100) */
        byte_copy_2(buf + 4, digit_table + dd * 2);
        byte_copy_2(buf + 6, digit_table + ee * 2);
        byte_copy_2(buf + 8, digit_table + ff * 2);
        byte_copy_2(buf + 10, digit_table + gg * 2);
        if (hhii)
        {
            u32 hh = (hhii * 5243) >> 19; /* (ccdd / 100) */
            u32 ii = hhii - hh * 100;     /* (ccdd % 100) */
            byte_copy_2(buf + 12, digit_table + hh * 2);
            byte_copy_2(buf + 14, digit_table + ii * 2);
            tz1 = dec_tz_table[hh];
            tz2 = dec_tz_table[ii];
            gcc_load_barrier(tz2); /* let gcc emit cmov */
            tz = tz2 + (ii ? 0 : tz1);
            return buf + 16 - tz;
        }
        else
        {
            tz1 = dec_tz_table[ff];
            tz2 = dec_tz_table[gg];
            gcc_load_barrier(tz2); /* let gcc emit cmov */
            tz = tz2 + (gg ? 0 : tz1);
            return buf + 12 - tz;
        }
    }
    else
    {
        if (ddee)
        {
            u32 dd = (ddee * 5243) >> 19; /* (ddee / 100) */
            u32 ee = ddee - dd * 100;     /* (ddee % 100) */
            byte_copy_2(buf + 4, digit_table + dd * 2);
            byte_copy_2(buf + 6, digit_table + ee * 2);
            tz1 = dec_tz_table[dd];
            tz2 = dec_tz_table[ee];
            gcc_load_barrier(tz2); /* let gcc emit cmov */
            tz = tz2 + (ee ? 0 : tz1);
            return buf + 8 - tz;
        }
        else
        {
            tz1 = dec_tz_table[bb];
            tz2 = dec_tz_table[cc];
            gcc_load_barrier(tz2); /* let gcc emit cmov */
            tz = tz2 + (cc ? 0 : tz1);
            return buf + 4 - tz;
        }
    }
#endif
}

/** Write exponent part in the range `e-324` to `e308`. */
static_inline u8 *write_f64_exp(i32 exp, u8 *buf)
{
    // use lut to storage the exp result;
    // last byte is the length;
    // 5064 byte

    // static u8 exp_result2[(308 + 324 + 1) * 5];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     // use 40 byte , 'e-324' is 40byte
    // static u32 exp_result3[308 + 324 + 1] = {0xf432332d, 0xf332332d, 0xf232332d, 0xf132332d, 0xf032332d, 0xf931332d, 0xf831332d, 0xf731332d, 0xf631332d, 0xf531332d, 0xf431332d, 0xf331332d, 0xf231332d, 0xf131332d, 0xf031332d, 0xf930332d, 0xf830332d, 0xf730332d, 0xf630332d, 0xf530332d, 0xf430332d, 0xf330332d, 0xf230332d, 0xf130332d, 0xf030332d, 0xf939322d, 0xf839322d, 0xf739322d, 0xf639322d, 0xf539322d, 0xf439322d, 0xf339322d, 0xf239322d, 0xf139322d, 0xf039322d, 0xf938322d, 0xf838322d, 0xf738322d, 0xf638322d, 0xf538322d, 0xf438322d, 0xf338322d, 0xf238322d, 0xf138322d, 0xf038322d, 0xf937322d, 0xf837322d, 0xf737322d, 0xf637322d, 0xf537322d, 0xf437322d, 0xf337322d, 0xf237322d, 0xf137322d, 0xf037322d, 0xf936322d, 0xf836322d, 0xf736322d, 0xf636322d, 0xf536322d, 0xf436322d, 0xf336322d, 0xf236322d, 0xf136322d, 0xf036322d, 0xf935322d, 0xf835322d, 0xf735322d, 0xf635322d, 0xf535322d, 0xf435322d, 0xf335322d, 0xf235322d, 0xf135322d, 0xf035322d, 0xf934322d, 0xf834322d, 0xf734322d, 0xf634322d, 0xf534322d, 0xf434322d, 0xf334322d, 0xf234322d, 0xf134322d, 0xf034322d, 0xf933322d, 0xf833322d, 0xf733322d, 0xf633322d, 0xf533322d, 0xf433322d, 0xf333322d, 0xf233322d, 0xf133322d, 0xf033322d, 0xf932322d, 0xf832322d, 0xf732322d, 0xf632322d, 0xf532322d, 0xf432322d, 0xf332322d, 0xf232322d, 0xf132322d, 0xf032322d, 0xf931322d, 0xf831322d, 0xf731322d, 0xf631322d, 0xf531322d, 0xf431322d, 0xf331322d, 0xf231322d, 0xf131322d, 0xf031322d, 0xf930322d, 0xf830322d, 0xf730322d, 0xf630322d, 0xf530322d, 0xf430322d, 0xf330322d, 0xf230322d, 0xf130322d, 0xf030322d, 0xf939312d, 0xf839312d, 0xf739312d, 0xf639312d, 0xf539312d, 0xf439312d, 0xf339312d, 0xf239312d, 0xf139312d, 0xf039312d, 0xf938312d, 0xf838312d, 0xf738312d, 0xf638312d, 0xf538312d, 0xf438312d, 0xf338312d, 0xf238312d, 0xf138312d, 0xf038312d, 0xf937312d, 0xf837312d, 0xf737312d, 0xf637312d, 0xf537312d, 0xf437312d, 0xf337312d, 0xf237312d, 0xf137312d, 0xf037312d, 0xf936312d, 0xf836312d, 0xf736312d, 0xf636312d, 0xf536312d, 0xf436312d, 0xf336312d, 0xf236312d, 0xf136312d, 0xf036312d, 0xf935312d, 0xf835312d, 0xf735312d, 0xf635312d, 0xf535312d, 0xf435312d, 0xf335312d, 0xf235312d, 0xf135312d, 0xf035312d, 0xf934312d, 0xf834312d, 0xf734312d, 0xf634312d, 0xf534312d, 0xf434312d, 0xf334312d, 0xf234312d, 0xf134312d, 0xf034312d, 0xf933312d, 0xf833312d, 0xf733312d, 0xf633312d, 0xf533312d, 0xf433312d, 0xf333312d, 0xf233312d, 0xf133312d, 0xf033312d, 0xf932312d, 0xf832312d, 0xf732312d, 0xf632312d, 0xf532312d, 0xf432312d, 0xf332312d, 0xf232312d, 0xf132312d, 0xf032312d, 0xf931312d, 0xf831312d, 0xf731312d, 0xf631312d, 0xf531312d, 0xf431312d, 0xf331312d, 0xf231312d, 0xf131312d, 0xf031312d, 0xf930312d, 0xf830312d, 0xf730312d, 0xf630312d, 0xf530312d, 0xf430312d, 0xf330312d, 0xf230312d, 0xf130312d, 0xf030312d, 0x8039392d, 0x8038392d, 0x8037392d, 0x8036392d, 0x8035392d, 0x8034392d, 0x8033392d, 0x8032392d, 0x8031392d, 0x8030392d, 0x8039382d, 0x8038382d, 0x8037382d, 0x8036382d, 0x8035382d, 0x8034382d, 0x8033382d, 0x8032382d, 0x8031382d, 0x8030382d, 0x8039372d, 0x8038372d, 0x8037372d, 0x8036372d, 0x8035372d, 0x8034372d, 0x8033372d, 0x8032372d, 0x8031372d, 0x8030372d, 0x8039362d, 0x8038362d, 0x8037362d, 0x8036362d, 0x8035362d, 0x8034362d, 0x8033362d, 0x8032362d, 0x8031362d, 0x8030362d, 0x8039352d, 0x8038352d, 0x8037352d, 0x8036352d, 0x8035352d, 0x8034352d, 0x8033352d, 0x8032352d, 0x8031352d, 0x8030352d, 0x8039342d, 0x8038342d, 0x8037342d, 0x8036342d, 0x8035342d, 0x8034342d, 0x8033342d, 0x8032342d, 0x8031342d, 0x8030342d, 0x8039332d, 0x8038332d, 0x8037332d, 0x8036332d, 0x8035332d, 0x8034332d, 0x8033332d, 0x8032332d, 0x8031332d, 0x8030332d, 0x8039322d, 0x8038322d, 0x8037322d, 0x8036322d, 0x8035322d, 0x8034322d, 0x8033322d, 0x8032322d, 0x8031322d, 0x8030322d, 0x8039312d, 0x8038312d, 0x8037312d, 0x8036312d, 0x8035312d, 0x8034312d, 0x8033312d, 0x8032312d, 0x8031312d, 0x8030312d, 0x4000392d, 0x4000382d, 0x4000372d, 0x4000362d, 0x4000352d, 0x4000342d, 0x4000332d, 0x4000322d, 0x4000312d, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40003031, 0x40003131, 0x40003231, 0x40003331, 0x40003431, 0x40003531, 0x40003631, 0x40003731, 0x40003831, 0x40003931, 0x40003032, 0x40003132, 0x40003232, 0x40003332, 0x40003432, 0x40003532, 0x40003632, 0x40003732, 0x40003832, 0x40003932, 0x40003033, 0x40003133, 0x40003233, 0x40003333, 0x40003433, 0x40003533, 0x40003633, 0x40003733, 0x40003833, 0x40003933, 0x40003034, 0x40003134, 0x40003234, 0x40003334, 0x40003434, 0x40003534, 0x40003634, 0x40003734, 0x40003834, 0x40003934, 0x40003035, 0x40003135, 0x40003235, 0x40003335, 0x40003435, 0x40003535, 0x40003635, 0x40003735, 0x40003835, 0x40003935, 0x40003036, 0x40003136, 0x40003236, 0x40003336, 0x40003436, 0x40003536, 0x40003636, 0x40003736, 0x40003836, 0x40003936, 0x40003037, 0x40003137, 0x40003237, 0x40003337, 0x40003437, 0x40003537, 0x40003637, 0x40003737, 0x40003837, 0x40003937, 0x40003038, 0x40003138, 0x40003238, 0x40003338, 0x40003438, 0x40003538, 0x40003638, 0x40003738, 0x40003838, 0x40003938, 0x40003039, 0x40003139, 0x40003239, 0x40003339, 0x40003439, 0x40003539, 0x40003639, 0x40003739, 0x40003839, 0x40003939, 0x80303031, 0x80313031, 0x80323031, 0x80333031, 0x80343031, 0x80353031, 0x80363031, 0x80373031, 0x80383031, 0x80393031, 0x80303131, 0x80313131, 0x80323131, 0x80333131, 0x80343131, 0x80353131, 0x80363131, 0x80373131, 0x80383131, 0x80393131, 0x80303231, 0x80313231, 0x80323231, 0x80333231, 0x80343231, 0x80353231, 0x80363231, 0x80373231, 0x80383231, 0x80393231, 0x80303331, 0x80313331, 0x80323331, 0x80333331, 0x80343331, 0x80353331, 0x80363331, 0x80373331, 0x80383331, 0x80393331, 0x80303431, 0x80313431, 0x80323431, 0x80333431, 0x80343431, 0x80353431, 0x80363431, 0x80373431, 0x80383431, 0x80393431, 0x80303531, 0x80313531, 0x80323531, 0x80333531, 0x80343531, 0x80353531, 0x80363531, 0x80373531, 0x80383531, 0x80393531, 0x80303631, 0x80313631, 0x80323631, 0x80333631, 0x80343631, 0x80353631, 0x80363631, 0x80373631, 0x80383631, 0x80393631, 0x80303731, 0x80313731, 0x80323731, 0x80333731, 0x80343731, 0x80353731, 0x80363731, 0x80373731, 0x80383731, 0x80393731, 0x80303831, 0x80313831, 0x80323831, 0x80333831, 0x80343831, 0x80353831, 0x80363831, 0x80373831, 0x80383831, 0x80393831, 0x80303931, 0x80313931, 0x80323931, 0x80333931, 0x80343931, 0x80353931, 0x80363931, 0x80373931, 0x80383931, 0x80393931, 0x80303032, 0x80313032, 0x80323032, 0x80333032, 0x80343032, 0x80353032, 0x80363032, 0x80373032, 0x80383032, 0x80393032, 0x80303132, 0x80313132, 0x80323132, 0x80333132, 0x80343132, 0x80353132, 0x80363132, 0x80373132, 0x80383132, 0x80393132, 0x80303232, 0x80313232, 0x80323232, 0x80333232, 0x80343232, 0x80353232, 0x80363232, 0x80373232, 0x80383232, 0x80393232, 0x80303332, 0x80313332, 0x80323332, 0x80333332, 0x80343332, 0x80353332, 0x80363332, 0x80373332, 0x80383332, 0x80393332, 0x80303432, 0x80313432, 0x80323432, 0x80333432, 0x80343432, 0x80353432, 0x80363432, 0x80373432, 0x80383432, 0x80393432, 0x80303532, 0x80313532, 0x80323532, 0x80333532, 0x80343532, 0x80353532, 0x80363532, 0x80373532, 0x80383532, 0x80393532, 0x80303632, 0x80313632, 0x80323632, 0x80333632, 0x80343632, 0x80353632, 0x80363632, 0x80373632, 0x80383632, 0x80393632, 0x80303732, 0x80313732, 0x80323732, 0x80333732, 0x80343732, 0x80353732, 0x80363732, 0x80373732, 0x80383732, 0x80393732, 0x80303832, 0x80313832, 0x80323832, 0x80333832, 0x80343832, 0x80353832, 0x80363832, 0x80373832, 0x80383832, 0x80393832, 0x80303932, 0x80313932, 0x80323932, 0x80333932, 0x80343932, 0x80353932, 0x80363932, 0x80373932, 0x80383932, 0x80393932, 0x80303033, 0x80313033, 0x80323033, 0x80333033, 0x80343033, 0x80353033, 0x80363033, 0x80373033, 0x80383033};
    //  last
    static bool is_first = true;
    if (0)
        if (is_first)
        {
            is_first = false;
            // for (int exp = -324; exp <= 308; exp++)
            // {
            //     // length is [2,5] = 2 + [0,3]
            //     i32 neg = exp < 0;
            //     i32 exp_abs = neg ? -exp : exp;
            //     i32 dec_length = 1 + (exp_abs >= 10) + (exp_abs >= 100); //[1,3]
            //     u64 result = neg ? (u64)('e') + ((u64)('-') << 8) : (u64)('e');
            //     u64 result_ne = neg ? ((u64)('-')) : 0;
            //     u64 a = exp_abs / 100 + '0';
            //     u64 b = exp_abs % 100 / 10 + '0';
            //     u64 c = exp_abs % 100 % 10 + '0';
            //     u64 exp_ascii;
            //     if (exp_abs >= 100)
            //     {
            //         exp_ascii = a + (b << 8) + (c << 16);
            //     }
            //     else if (exp_abs >= 10)
            //     {
            //         exp_ascii = b + (c << 8);
            //     }
            //     else
            //     {
            //         exp_ascii = c;
            //     }
            //     u64 exp_res = result + (exp_ascii << (neg ? 16 : 8)) + ((u64)(1 + neg + dec_length) << (5 * 8));
            //     u64 exp_res2 = result + (exp_ascii << (neg ? 16 : 8)) + ((u64)(neg + dec_length - 1) << (4 * 8 + 6));
            //     u32 exp_res3 = result_ne + (exp_ascii << (neg ? 8 : 0)) + ((u32)(neg + dec_length - 1) << (30));
            //     exp_result[exp + 324] = exp_res;
            //     *(u64 *)&exp_result2[(exp + 324) * 5] = exp_res2;
            //     exp_result3[exp + 324] = exp_res3;
            //     // printf("0x%llx,",exp_res3);
            // }
        }

    const int use_large_lut = 1;
    if (use_large_lut)
    {
        *(u64 *)(buf + 0) = exp_result[324 + exp];
        return buf + (exp_result[324 + exp] >> (5 * 8));
    }
    // const u32 M30 = ((u32)1<<30)-1;
    // buf[0]='e';
    // *(u32*)(buf+1) = exp_result3[exp+324] & M30;
    // return buf + (exp_result3[exp+324] >> (30)) + 2;

    // const u64 M38 = ((u64)1<<38)-1;
    // *(u64*)buf = *(u64*)&exp_result2[(exp+324)*5] & M38;
    // return buf + ((*(u64*)&exp_result2[(exp+324)*5] >> 38) & 3) + 2;

    u32 neg, lz, a, bb;
    /* write the exponent notation and sign */
    neg = exp < 0;
    exp = neg ? -exp : exp;
    byte_copy_2(buf, "e-");
    buf += 1 + neg;

    /* write the exponent value */
    a = ((u32)exp * 656) >> 16; /* exp / 100 */
    bb = (u32)exp - a * 100;    /* exp % 100 */
    buf[0] = (u8)((u8)a + (u8)'0');
    buf += a > 0;
    lz = exp < 10;
    byte_copy_2(buf, digit_table + bb * 2 + lz);
    return buf + 2 - lz;
}

/**
 Convert double number from binary to decimal.
 The output significand is shortest decimal but may have trailing zeros.

 @param sig_bin The input significand in binary format.
 @param exp_bin The input exponent in binary format.
 @param sig_dec The output significand in decimal format.
 @param exp_dec The output exponent in decimal format.
 @warning The input double number should not be 0, inf or nan.
 */
static_inline void f64_bin_to_dec(u64 sig_bin, i32 exp_bin,
                                  u64 *sig_dec, i32 *exp_dec)
{
    /*
     For regular spacing significand 'c', there are 4 candidates for the result:

             d0             d1 c  u1                            u0
     ----|----|----|----|----|-*--|----|----|----|----|----|----|----|----
         9    0    1    2    3    4    5    6    7    8    9    0    1
           |___________________|___________________|
                             1ulp

     The `1ulp` is in the range [1.0, 10.0).
     If (c < x.5), round down to nearest 1.
     If (c > x.5), round up to nearest 1.
     If (c - 0.5ulp < u0), trim the last digit and round down to nearest 10.
     If (c + 0.5ulp > w0), trim the last digit and round up to nearest 10.
     */
    bool irregular, round_d0, round_d1, round_u0, round_u1;
    u64 cb, p10_hi, p10_lo, sig_hi, sig_lo;
    u64 one, ten, c, half_ulp, t0, t1, dec, dec_one, dec_ten;
    i32 k, h, trim, add_ten;

    /* fast path */
    while (likely((sig_bin != 0x10000000000000ull)))
    {
        k = (i32)(exp_bin * 315653) >> 20;       /* floor(exp_bin * log10(2)) */
        h = exp_bin + ((k * -217707) >> 16) + 1; /* exp_bin - k * floor(log2(10)) */
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        cb = sig_bin << (h); /* h = 0/1/2/3 */
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
        //ten = (sig_hi / 10) * 10;
        ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10;
        one = sig_hi - ten;
        /* right shift 4 to fit in u64 */
        c = (one << (64 - 4)) | (sig_lo >> 4);
        half_ulp = p10_hi >> (5 - h);
        t0 = ((u64)10 << (64 - 4));//lower
        t1 = c + half_ulp;//upper
        if (unlikely(sig_lo == ((u64)1 << 63)))break;//6641/12034   , k<0
        if (unlikely(half_ulp == c))break;//2703/12034 , k>=0
        if (unlikely(t0 - t1 <= (u64)1))break;//2688/12034, k>=0
        //round_u1 = (sig_lo > ((u64)1 << 63));
        round_u1 = sig_lo >> 63;
        round_u0 = (t1 > t0);
        round_d0 = (half_ulp > c);
        /* round result */

        trim = (round_d0 + round_u0);
        add_ten = (round_u0 ? 10 : 0);
        dec_one = sig_hi + round_u1;
        dec_ten = ten + add_ten;
        dec = trim ? dec_ten : dec_one;
        gcc_load_barrier(dec_one); /* let gcc emit cmov */

        // dec = sig_hi + (sig_lo >> 63);
        // if(t1 > t0) dec = ten + 10;
        // if(half_ulp > c) dec = ten;

        *sig_dec = dec;
        *exp_dec = k;
        return;
    }
    //full path
    {
        irregular = (sig_bin == 0x10000000000000ull);
        if (likely(!irregular))
            k = (exp_bin * 315653) >> 20; // branch
        else
            k = (exp_bin * 315653 - 131237) >> 20;
        h = exp_bin + ((k * -217707) >> 16) + 1;//[1,4]
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        cb = sig_bin << (h);
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
        ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10; // slow
        one = sig_hi - ten;
        /* right shift 4 to fit in u64 */
        c = (one << (64 - 4)) | (sig_lo >> 4);
        half_ulp = p10_hi >> (5 - h);//[1,4]
        t0 = ((u64)10 << (64 - 4)); // const
        t1 = c + half_ulp;
        /* check range */
        if (likely(!irregular)) // high branch
        {
            round_u1 = (sig_lo >= ((u64)1 << 63));
            if (unlikely(sig_lo == ((u64)1 << 63))) // low branch
            {
                round_u1 = (sig_hi & 1);
                //static int sum = 0;
                // printf("sig_lo=1<<63 sum=%d k=%d\n",sum++,k);
            }
            round_d0 = (half_ulp > c);
            if (unlikely(half_ulp == c)) // low branch
            {
                round_d0 = !(sig_bin & 1);
                //static int sum = 0;
                // printf("half_ulp==c sum=%d k=%d\n",sum++,k);
            }
        }
        else // low branch
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            // round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
            // if (!round_d1)round_u1 = true;
            if ((half_ulp / 2 < (sig_lo >> 4))) round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 > t0);
        if (unlikely(t0 - t1 <= (u64)1)) // low branch
        {
            // static int sum = 0;
            // printf("t0-t1<=1 sum=%d k=%d\n",sum++,k);
            // if( unlikely((k == 0) | (t0 - 1 == t1)))round_u0 = !(sig_bin & 1);
            if ((t0 - t1) <= *(u64 *)&k)
                round_u0 = !(sig_bin & 1);
        }
        *exp_dec = k;

        *sig_dec = sig_hi + round_u1;
        if (round_d0)
            *sig_dec = ten;
        if (round_u0)
            *sig_dec = ten + 10;
    }
}

static_inline void f64_bin_to_dec_full(u64 sig_bin, i32 exp_bin,
                                  u64 *sig_dec, i32 *exp_dec)
{
    /*
     For regular spacing significand 'c', there are 4 candidates for the result:

             d0             d1 c  u1                            u0
     ----|----|----|----|----|-*--|----|----|----|----|----|----|----|----
         9    0    1    2    3    4    5    6    7    8    9    0    1
           |___________________|___________________|
                             1ulp

     The `1ulp` is in the range [1.0, 10.0).
     If (c < x.5), round down to nearest 1.
     If (c > x.5), round up to nearest 1.
     If (c - 0.5ulp < u0), trim the last digit and round down to nearest 10.
     If (c + 0.5ulp > w0), trim the last digit and round up to nearest 10.
     */
    bool irregular, round_d0, round_d1, round_u0, round_u1;
    u64 cb, p10_hi, p10_lo, sig_hi, sig_lo;
    u64 one, ten, c, half_ulp, t0, t1, dec, dec_one, dec_ten;
    i32 k, h, trim, add_ten;

    /* fast path */
    // if(0)
    // while (likely((sig_bin != 0x10000000000000ull)))
    // {
    //     // unsigned long long vi = (sig_bin & ((1ull<<52)-1)) | (( (exp_bin+1075) & 2047ull)<<52);
    //     // double v = *(double*)&vi;
    //     k = (i32)(exp_bin * 315653) >> 20;       /* floor(exp_bin * log10(2)) */
    //     h = exp_bin + ((k * -217707) >> 16) + 1; /* exp_bin - k * floor(log2(10)) */
    //     pow10_table_get_sig(-k, &p10_hi, &p10_lo);
    //     cb = sig_bin << (h); /* h = 0/1/2/3 */
    //     u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
    //     u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
    //     //ten = (sig_hi / 10) * 10;
    //     ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10;
    //     one = sig_hi - ten;
    //     /* right shift 4 to fit in u64 */
    //     c = (one << (64 - 4)) | (sig_lo >> 4);
    //     half_ulp = p10_hi >> (5 - h);
    //     t0 = ((u64)10 << (64 - 4));//lower
    //     t1 = c + half_ulp;//upper
    //     // if ((unlikely(sig_lo == ((u64)1 << 63))) ){
    //     //     static int sum=0;
    //     //     static int maxk=-1000;
    //     //     static int mink=10000;
    //     //     maxk=maxk>k?maxk:k;
    //     //     mink=mink<k?mink:k;
    //     //     printf("sum=%d,sig_lo = 1<<63, k==%d mink=%d, maxk=%d v=%.17g v=%.16le %llx \n",sum++,k,mink,maxk,v,v,vi);//k
    //     // }
    //     // if ( (unlikely(half_ulp == c)) ){
    //     //     static int sum=0;
    //     //     static int maxk=-1000;
    //     //     static int mink=10000;
    //     //     static double minv=1e308;
    //     //     static double maxv=0;
    //     //     maxk=maxk>k?maxk:k;
    //     //     mink=mink<k?mink:k;
    //     //     maxv=maxv>v?maxv:v;
    //     //     minv=minv<v?minv:v;
    //     //     printf("sum=%d,half_ulp==c,  k==%d mink=%d, maxk=%d v=%.17g v=%.16le %llx minv=%.16le maxv=%.16le\n",sum++,k,mink,maxk,v,v,vi,minv,maxv);//k
    //     // }
    //     // if ((unlikely(t0 - t1 <= (u64)1)))
    //     // {
    //     //     static int sum=0;
    //     //     static int maxk=-1000;
    //     //     static int mink=10000;
    //     //     static double minv=1e308;
    //     //     static double maxv=0;
    //     //     maxk=maxk>k?maxk:k;
    //     //     mink=mink<k?mink:k;
    //     //     maxv=maxv>v?maxv:v;
    //     //     minv=minv<v?minv:v;
    //     //     printf("sum=%d,t0 - t1 <= 1,  k==%d mink=%d, maxk=%d v=%.17g v=%.16le %llx minv=%.16le maxv=%.16le\n",sum++,k,mink,maxk,v,v,vi,minv,maxv);//k
    //     // }
    //     // if (   (unlikely(sig_lo == ((u64)1 << 63)))  | (unlikely(half_ulp == c)) | (unlikely(t0 - t1 <= (u64)1))){
    //     //     static int sum=0;
    //     //     printf("sum=%d\n",sum++);
    //     // }

    //     /* check range */
    //     // if (unlikely(sig_lo == ((u64)1 << 63)))break;//6641/12034   , k<0
    //     // if (unlikely(half_ulp == c))break;//2703/12034 , k>=0
    //     // if (unlikely(t0 - t1 <= (u64)1))break;//2688/12034, k>=0

    //     if ((sig_lo == ((u64)1 << 63)))break;//6641/12034   , k<0
    //     if ((half_ulp == c))break;//2703/12034 , k>=0
    //     if ((t0 - t1 <= (u64)1))break;//2688/12034, k>=0

    //     //round_u1 = (sig_lo > ((u64)1 << 63));
    //     round_u1 = sig_lo >> 63;
    //     round_u0 = (t1 > t0);
    //     round_d0 = (half_ulp > c);
    //     /* round result */

    //     // trim = (round_d0 + round_u0);
    //     // add_ten = (round_u0 ? 10 : 0);
    //     // dec_one = sig_hi + round_u1;
    //     // //dec_one = sig_hi;
    //     // dec_ten = ten + add_ten;
    //     // dec = trim ? dec_ten : dec_one;
    //     // gcc_load_barrier(dec_one); /* let gcc emit cmov */

    //     dec = sig_hi + (sig_lo >> 63);
    //     if(t1 > t0) dec = ten + 10;
    //     if(half_ulp > c) dec=ten;

    //     *sig_dec = dec;
    //     *exp_dec = k;
    //     return;
    // }

    // bool state = false;
    // if(sig_bin != 0x10000000000000ull)
    // if(0)
    // while (likely((sig_bin != 0x10000000000000ull)))
    // {
    //     k = (i32)(exp_bin * 315653) >> 20;       /* floor(exp_bin * log10(2)) */
    //     h = exp_bin + ((k * -217707) >> 16) + 1; /* exp_bin - k * floor(log2(10)) */
    //     pow10_table_get_sig(-k, &p10_hi, &p10_lo);
    //     cb = sig_bin << (h); /* h = 0/1/2/3 */
    //     u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
    //     u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
    //     ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10;
    //     one = sig_hi - ten;
    //     c = (one << (64 - 4)) | (sig_lo >> 4);
    //     half_ulp = p10_hi >> (5 - h);
    //     t0 = ((u64)10 << (64 - 4)); // 1010<<60
    //     t1 = c + half_ulp;          // upper
    //     if (unlikely(sig_lo == ((u64)1 << 63)))break; // 6641/12034   , k<0
    //     if (unlikely(half_ulp == c))break; // 2703/12034 , k>=0
    //     if (unlikely(t0 - t1 <= (u64)1))break; // 2688/12034, k>=0
    //     // if ((sig_lo == ((u64)1 << 63)))state= true;//6641/12034   , k<0
    //     // if ((half_ulp == c))state= true;//2703/12034 , k>=0
    //     // if ((t0 - t1<= (u64)1))state= true;//2688/12034, k>=0
    //     // if (   ((sig_lo == ((u64)1 << 63)))  | ((half_ulp == c)) | ((t0 - t1 <= (u64)1))) state= true ;
    //     dec = sig_hi + (sig_lo >> 63);
    //     if (t1 >= t0)dec = ten + 10;
    //     if (half_ulp >= c)dec = ten;
    //     *sig_dec = dec;
    //     *exp_dec = k;
    //     return;
    // }
#if 0
    if (0)
    /* full path */
    {
        /* k = floor(exp_bin * log10(2) + (irregular ? log10(3.0 / 4.0) : 0)) */
        irregular = (sig_bin == 0x10000000000000ull);
        k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
        h = exp_bin + ((k * -217707) >> 16) + 1;
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);

        cb = sig_bin << (h);
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
        // one = sig_hi % 10;
        // ten = sig_hi - one;

        // uint64_t sig_hi_div10 = (sig_hi * (__uint128_t)1844674407370955162) >> 64;
        ten = sig_hi / 10 * 10;
        one = sig_hi - ten;

        /* right shift 4 to fit in u64 */
        c = (one << (64 - 4)) | (sig_lo >> 4);
        half_ulp = p10_hi >> (5 - h);
        t0 = ((u64)10 << (64 - 4));
        t1 = c + half_ulp;

        /* check range */
        if (!irregular) // likely
        {
            round_u1 = (sig_lo >= ((u64)1 << 63));
            if (sig_lo == ((u64)1 << 63))
                round_u1 = (sig_hi & 1);
            // sig_hi & (sig_lo>>63);
            round_d0 = (half_ulp >= c);
            if (half_ulp == c)
                round_d0 = !(sig_bin & 1);
        }
        else
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
            if (!round_d1)
                round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 >= t0);
        if (t0 - t1 <= (u64)1)
        {
            if (k == 0 || (t0 - t1 == 1))
                round_u0 = !(sig_bin & 1);
        }
        /* round result */
        trim = (round_d0 + round_u0);
        add_ten = (round_u0 ? 10 : 0);
        dec_one = sig_hi + round_u1;
        dec_ten = ten + add_ten;
        dec = trim ? dec_ten : dec_one;
        gcc_load_barrier(dec_one); /* let gcc emit cmov */
        *sig_dec = dec;
        *exp_dec = k;
    }
#endif

    // if(0)
    // {
    //     /* k = floor(exp_bin * log10(2) + (irregular ? log10(3.0 / 4.0) : 0)) */
    //     irregular = (sig_bin == 0x10000000000000ull);
    //     k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
    //     h = exp_bin + ((k * -217707) >> 16) + 1;
    //     pow10_table_get_sig(-k, &p10_hi, &p10_lo);

    //     cb = sig_bin << (h);
    //     u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
    //     u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);

    //     // *sig_dec = sig_hi;
    //     // *exp_dec = k;
    //     // return ;

    //     // one = sig_hi % 10;
    //     // ten = sig_hi - one;

    //     // uint64_t sig_hi_div10 = (sig_hi * (__uint128_t)1844674407370955162) >> 64;
    //     // ten = (sig_hi / 10) * 10;
    //     ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10;
    //     one = sig_hi - ten;

    //     /* right shift 4 to fit in u64 */
    //     c = (one << (64 - 4)) | (sig_lo >> 4);
    //     half_ulp = p10_hi >> (5 - h);
    //     t0 = ((u64)10 << (64 - 4)); // const
    //     t1 = c + half_ulp;

    //     /* check range */
    //     if (likely(!irregular))
    //     {
    //         round_u1 = (sig_lo >= ((u64)1 << 63));
    //         //round_u1 = sig_lo >> 63;
    //         if (unlikely(sig_lo == ((u64)1 << 63))) round_u1 = (sig_hi & 1);
    //         round_d0 = (half_ulp >= c);
    //         if (unlikely(half_ulp == c))round_d0 = !(sig_bin & 1);
    //     }
    //     else
    //     {
    //         round_u1 = (sig_lo > ((u64)1 << 63));
    //         round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
    //         if (!round_d1)
    //             round_u1 = true;
    //         round_d0 = (half_ulp / 2 >= c);
    //     }
    //     round_u0 = (t1 >= t0);
    //     if ( unlikely(t0 - t1 <= (u64)1))
    //     {
    //         // if (k == 0 || (t0 - t1 == 1))
    //         //     round_u0 = !(sig_bin & 1);
    //         if (k == 0 || (t0 - 1 == t1))
    //             round_u0 = !(sig_bin & 1);
    //     }
    //     // t1>t0 , 1
    //     // t1==t0 , k==0 && !(sig_bin & 1)
    //     // t1=t0-1 , !(sig_bin & 1)
    //     // u32 m = !(sig_bin & 1);//0 or 1
    //     // round_u0 = (t1 >= t0);
    //     // if(t1==t0)round_u0 = m > (*(u32*)&k) ;// k==0 && m==1
    //     // if(t1==t0-1)round_u0 = m;

    //     /* round result */
    //     // trim = (round_d0 + round_u0);
    //     // add_ten = (round_u0 ? 10 : 0);
    //     // // dec_one = sig_hi ;
    //     // dec_one = sig_hi + round_u1;
    //     // dec_ten = ten + add_ten;
    //     // dec = trim ? dec_ten : dec_one;
    //     // gcc_load_barrier(dec_one); /* let gcc emit cmov */

    //     dec = sig_hi + round_u1;
    //     if(round_d0) dec = ten;
    //     if(round_u0) dec = ten + 10;

    //     *sig_dec = dec;
    //     *exp_dec = k;
    // }

    //if (1)
    {
        irregular = (sig_bin == 0x10000000000000ull);
        if (likely(!irregular))
            k = (exp_bin * 315653) >> 20; // branch
        else
            k = (exp_bin * 315653 - 131237) >> 20;
        h = exp_bin + ((k * -217707) >> 16) + 1;//[1,4]
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        cb = sig_bin << (h);
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
        ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10; // slow
        one = sig_hi - ten;
        /* right shift 4 to fit in u64 */
        c = (one << (64 - 4)) | (sig_lo >> 4);
        half_ulp = p10_hi >> (5 - h);//[1,4]
        t0 = ((u64)10 << (64 - 4)); // const
        t1 = c + half_ulp;
        /* check range */
        if (likely(!irregular)) // high branch
        {
            round_u1 = (sig_lo >= ((u64)1 << 63));
            if (unlikely(sig_lo == ((u64)1 << 63))) // low branch
            {
                round_u1 = (sig_hi & 1);
                //static int sum = 0;
                // printf("sig_lo=1<<63 sum=%d k=%d\n",sum++,k);
            }
            round_d0 = (half_ulp > c);
            if (unlikely(half_ulp == c)) // low branch
            {
                round_d0 = !(sig_bin & 1);
                //static int sum = 0;
                // printf("half_ulp==c sum=%d k=%d\n",sum++,k);
            }
        }
        else // low branch
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            // round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
            // if (!round_d1)round_u1 = true;
            if ((half_ulp / 2 < (sig_lo >> 4))) round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 > t0);
        if (unlikely(t0 - t1 <= (u64)1)) // low branch
        {
            // static int sum = 0;
            // printf("t0-t1<=1 sum=%d k=%d\n",sum++,k);
            // if( unlikely((k == 0) | (t0 - 1 == t1)))round_u0 = !(sig_bin & 1);
            if ((t0 - t1) <= *(u64 *)&k)
                round_u0 = !(sig_bin & 1);
        }
        *exp_dec = k;

        *sig_dec = sig_hi + round_u1;
        if (round_d0)
            *sig_dec = ten;
        if (round_u0)
            *sig_dec = ten + 10;
    }
}
static_inline uint64_t encode_ten_thousands(uint64_t hi, uint64_t lo)
{
    uint64_t merged = hi | (lo << 32);

    uint64_t top = ((merged * 10486ULL) >> 20) & ((0x7FULL << 32) | 0x7FULL);
    /* Trailing 2 digits in the 1e4 chunks. */
    uint64_t bot = merged - 100ULL * top;
    uint64_t hundreds;
    uint64_t tens;

    /*
     * We now have 4 radix-100 digits in little-endian order, each
     * in its own 16 bit area.
     */
    hundreds = (bot << 16) + top;

    /* Divide and mod by 10 all 4 radix-100 digits in parallel. */
    tens = (hundreds * 103ULL) >> 10;
    tens &= (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
    tens += (hundreds - 10ULL * tens) << 8;

    return tens;
}
static_inline char *xjb_dtoa(double v, char *buf)
{
    const u64 M52 = (1ull << 52) - 1;
    const u64 M63 = (1ull << 63) - 1;
    u64 vi = *(u64 *)&v;
    u64 va = vi & M63;
    buf[0] = '-';
    buf += vi >> 63;
    u64 sig = vi & M52;
    u64 exp = va >> 52;
    if (unlikely(exp == 2047))
    {
        *(int *)buf = sig ? *(int *)"nan" : *(int *)"inf";
        return buf + 3;
    }
    u64 sig_bin = sig | (1ull << 52);
    ;
    int exp_bin = exp - 1075;
    if (unlikely(exp == 0))
    {
        sig_bin = sig;
        exp_bin = -1074; // 1-1075
        if (sig == 0)
        {
            buf[0] = '0';
            return buf + 1;
            //*(int*)buf = *(int*)"0.0";
            // return buf + 3;
        }
    }
    //*(u64 *)buf = *(u64 *)"0.000000"; // write 8 byte
    bool irregular, round_d0, round_d1, round_u0, round_u1;
    u64 cb, p10_hi, p10_lo, sig_hi, sig_lo;
    u64 one, ten, c, half_ulp, t0, t1, dec, dec_one, dec_ten;
    i32 k, h, trim, add_ten;
    u64 D16, D17, D17_2;
    u64 sig_dec;
    i32 exp_dec;
    u64 high9_sig_hi;
    int e10;
    // printf("run here 1\n");
    if (1)
    {
        // this core algorithm from https://github.com/ibireme/c_numconv_benchmark/blob/master/vendor/yy_double/yy_double.c
        // origin author : yy  , https://github.com/ibireme
        // modify author : xjb , 1302035400@qq.com
        // the correctness awaits verification by the origin author.
        // irregular = (sig_bin == 0x10000000000000ull);
        irregular = (sig == 0);
        if (likely(!irregular))
            k = (exp_bin * 315653) >> 20; // high branch
        else
            k = (exp_bin * 315653 - 131237) >> 20;
        h = exp_bin + ((k * -217707) >> 16) + 1;
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        cb = sig_bin << (h);
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);

        // {
        //     D17 = sig_hi >= (u64)(1e16);//
        //     const unsigned int * digit_ptr = D17 ? digit_000_999 : digit_00_99;
        //     high9_sig_hi = sig_hi / (u64)(1e8);
        //     u64 num123 = ((u128)(high9_sig_hi) * (18446744073710)) >> 64;
        //     //if(!D17) num123 *= 10;
        //     *(u32 *)&buf[0] = digit_ptr[num123]; // 4 byte : "x.xx"
        //     u64 num456_789 = ((u128)(high9_sig_hi) * (18446744073710));
        //     u64 num456 = ((u128)(num456_789) * (1000)) >> 64;
        //     //*(u32 *)&buf[4- (!D17)] = digit1000[num456];    // 3
        //     u64 num789 = ((u64)((u128)(num456_789) * (1000)) * (u128)(1000)) >> 64;
        //     //*(u32 *)&buf[7- (!D17)] = digit1000[num789];    // 3
        //     *(u64*)&buf[4 - (!D17)] = (u64)digit1000[num456] | ((u64)digit1000[num789]<<24);// write 6 byte
        // }
        ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10; // == (sig_hi / 10) * 10
        one = sig_hi - ten;
        c = (one << (64 - 4)) | (sig_lo >> 4);
        half_ulp = p10_hi >> (5 - h);
        t0 = ((u64)10 << (64 - 4));
        t1 = c + half_ulp;
        if (likely(!irregular)) // high branch
        {
            round_u1 = (sig_lo >= ((u64)1 << 63));
            if (unlikely(sig_lo == ((u64)1 << 63))) // low branch
            {
                round_u1 = (sig_hi & 1);
            }
            round_d0 = (half_ulp > c);
            if (unlikely(half_ulp == c)) // low branch
            {
                round_d0 = !(sig_bin & 1);
            }
        }
        else // low branch
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            if ((half_ulp / 2 < (sig_lo >> 4)))
                round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 > t0);
        if (unlikely(t0 - t1 <= (u64)1)) // low branch
        {
            // if( unlikely((k == 0) | (t0 - 1 == t1)))round_u0 = !(sig_bin & 1);
            if ((t0 - t1) <= *(u64 *)&k)
                round_u0 = !(sig_bin & 1);
        }
        exp_dec = k;
        sig_dec = sig_hi + round_u1;
        if (round_d0)
            sig_dec = ten;
        if (round_u0)
            sig_dec = ten + 10;
    }
    else
    {
        // author : xjb , 1302035400@qq.com
        // more fast but may error 0.038%;
        u64 odd = sig_bin & 1;
        const int offset = 11;                                        // range [3,11]
        exp_dec = (exp_bin * 315653) >> 20;                           // exp_bin [-1074,972]    k:[-324,292]
        h = exp_bin + (((-(exp_dec)-1) * 217707) >> 16) + 1 + offset; // [-3,0]
        const u64 *pow10_ptr = pow10_sig_table_round - POW10_SIG_TABLE_MIN_EXP * 2;
        u64 *sig = (u64 *)&pow10_ptr[(-(exp_dec)-1) * 2];
        p10_hi = sig[0];
        p10_lo = sig[1];
        cb = sig_bin << (h);
        sig_hi = (cb * (u128)p10_lo) >> 64;
        sig_lo = (cb * (u128)p10_hi + sig_hi);
        sig_hi = (cb * (u128)p10_hi + sig_hi) >> 64;
        ten = (sig_hi >> offset) * 10;
        half_ulp = p10_hi >> (1 + offset - h); // 1-h1 = [1,4]
        sig_lo = (sig_hi << (64 - offset)) + (sig_lo >> offset);
        sig_dec = ten + (((sig_lo + 0x0ccccccccccccccdull) * (u128)10) >> 64); // round to nearst
        sig_dec = (half_ulp - odd > (~0 - sig_lo)) ? ten + 10 : sig_dec;       // round up
        sig_dec = (half_ulp >= sig_lo + odd) ? ten : sig_dec;                  // round down
    }
    // printf("run here 2\n");
    u64 sig_fin = sig_dec;
    D17_2 = sig_fin >= (u64)(1e16); // has 17 digit
    // D16 = sig_fin < (u64)(1e16);
    e10 = exp_dec + 15 + D17_2;
    // e10 = exp_dec + 16 - D16;

    // if( !D17_2 ) sig_dec *= 10;//format to 17 digit
    //  if(!D17){
    //      //printf("sig_dec=%llu,e10=%d\n",sig_dec,e10);
    //  }
    //  if(D17 != D17_2){
    //      printf("error\n");
    //  }
    //  u64 dot_pos = (1 <= e10 && e10 <= 8) ? e10 + 1 : 1;
    //  u64 first_digit_write_pos = (-7 <= e10 && e10 <= -1) ? 1 - e10 : 0;
    //  u64 second_digit_write_pos = first_digit_write_pos + 2 - (-7<= e10 && e10 <= 8);
    u64 rz; // range : [0,15] and 17
    u64 sig_len;
    {
        // const u64 ZERO = 0x3030303030303030ull;
        //  u64 high1 = sig_dec / (u64)(1e16);
        //  *(unsigned short*)buf = high1 | ('0' + '.'*256);
        //  u64 high9 = sig_dec / (u64)(1e8); // 1e7 <= high9 < 1e9
        //  u64 high2_9 = high9 - high1 * (u64)(1e8);
        //  u64 low8 = sig_dec - high9 * (u64)(1e8); // 0 <= low8 < 1e8
        //  u64 a = (high2_9 * 28147497672ull) >> 48;
        //  u64 b = high2_9 - a * 10000;
        //  u64 c = (low8 * 28147497672ull) >> 48;
        //  u64 d = low8 - c * 10000;
        //  u64 ab = encode_ten_thousands(a,b);
        //  *(u64*)&buf[2] = ab | ZERO;
        //  u64 cd = encode_ten_thousands(c,d);
        //  *(u64*)&buf[10] = cd | ZERO;
        //  if(likely(low8!=0)){
        //      rz = __builtin_clzll(cd) >> 3;
        //  }else{
        //      rz = 8 + (__builtin_clzll(ab) >> 3 ) ;
        //      if(ab == 0) rz = 17;
        //  }
        u64 high9 = sig_fin / (u64)(1e8);        // 1e7 <= high9 < 1e9 . one mul and right shift
        u64 low8 = sig_fin - high9 * (u64)(1e8); // 0 <= low8 < 1e8   one mul and sub
        // if(unlikely(high9 != high9_sig_hi))
        {
            const unsigned int *digit_ptr = D17_2 ? digit_000_999 : digit_00_99;
            u64 num123 = ((u128)(high9) * (18446744073710)) >> 64;
            *(u32 *)&buf[0] = digit_ptr[num123]; // 4 byte : "x.xx"
            u64 num456_789 = ((u128)(high9) * (18446744073710));
            u64 num456 = ((u128)(num456_789) * (1000)) >> 64;
            //*(u32 *)&buf[4] = digit1000[num456];    // 3
            u64 num789 = ((u64)((u128)(num456_789) * (1000)) * (u128)(1000)) >> 64;
            //*(u32 *)&buf[7] = digit1000[num789];    // 3
            *(u64 *)&buf[4 - !D17_2] = (u64)digit1000[num456] | ((u64)digit1000[num789] << 24); // write 6 byte
            u64 h8d = (*(u64 *)&buf[2]) & 0x0f0f0f0f0f0f0f0full;
            // buf -= !D17_2;
            u64 num12 = ((u128)(low8) * (18446744073710)) >> 64;
            //*(u16 *)&buf[10] = digit100[num12];  // 2
            u64 num345_678 = ((u128)(low8) * (18446744073710));
            u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
            //*(u32 *)&buf[12] = digit1000[num345]; // 3
            u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
            //*(u32 *)&buf[15] = digit1000[num678]; // 3
            u64 low8w = (u64)digit100[num12] | ((u64)digit1000[num345] << 16) | ((u64)digit1000[num678] << 40);
            *(u64 *)&buf[10 - !D17_2] = low8w; // write 8 byte
            u64 low8d = low8w & 0x0f0f0f0f0f0f0f0full;
            rz = __builtin_clzll(low8d) >> 3; // low8d != 0
            u64 rz2 = (__builtin_clzll(h8d) >> 3);
            if (h8d == 0)
                rz2 = 9;
            if (low8 == 0)
                rz = 8 + rz2 - !D17_2;
            // if(h8d == 0) rz = 17;
        }
        // if(1)
        // {
        //     u64 h8w,l8w;
        //     {
        //         u64 num12 = ((u128)(high2_9) * (18446744073710)) >> 64;
        //         u64 num345_678 = ((u128)(high2_9) * (18446744073710));
        //         u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
        //         u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
        //         h8w = (u64)digit100[num12] | ((u64)digit1000[num345]<<16) | ((u64)digit1000[num678]<<40);
        //     }
        //     {
        //         u64 num12 = ((u128)(low8) * (18446744073710)) >> 64;
        //         u64 num345_678 = ((u128)(low8) * (18446744073710));
        //         u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
        //         u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
        //         l8w = (u64)digit100[num12] | ((u64)digit1000[num345]<<16) | ((u64)digit1000[num678]<<40);
        //     }
        //     *(u64*)&buf[2] = h8w;
        //     *(u64*)&buf[10] = l8w;
        //     // *(u64*)&buf[second_digit_write_pos] = h8w;
        //     // *(u64*)&buf[dot_pos + 1] = h8w >> ((dot_pos-1) * 8);
        //     // *(u64*)&buf[second_digit_write_pos + 9] = l8w;
        //     // buf[dot_pos] = '.';
        // }else{

        // }

        // u64 num123 = ((u128)(high9) * (18446744073710)) >> 64;
        // if(num123 < 100) num123 *= 10;
        // *(u32 *)&buf[0] = digit_000_999[num123]; // 4 byte : "x.xx"
        // buf -= !D17;
        // u64 num456_789 = ((u128)(high9) * (18446744073710));
        // u64 num456 = ((u128)(num456_789) * (1000)) >> 64;
        // //*(u32 *)&buf[4] = digit1000[num456];    // 3
        // u64 num789 = ((u64)((u128)(num456_789) * (1000)) * (u128)(1000)) >> 64;
        // //*(u32 *)&buf[7] = digit1000[num789];    // 3
        // *(u64*)&buf[4] = (u64)digit1000[num456] | ((u64)digit1000[num789]<<24);// write 6 byte
        if (likely(low8 != 0))
        {
            // u64 num12 = ((u128)(low8) * (18446744073710)) >> 64;
            // //*(u16 *)&buf[10] = digit100[num12];  // 2
            // u64 num345_678 = ((u128)(low8) * (18446744073710));
            // u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
            // //*(u32 *)&buf[12] = digit1000[num345]; // 3
            // u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
            // //*(u32 *)&buf[15] = digit1000[num678]; // 3
            // u64 low8w = (u64)digit100[num12] | ((u64)digit1000[num345]<<16) | ((u64)digit1000[num678]<<40);
            // *(u64*)&buf[10] = low8w;//write 8 byte
            // u64 low8d = low8w & 0x0f0f0f0f0f0f0f0full;
            // rz = __builtin_clzll(low8d) >> 3;//low8d != 0
        }
        else
        {
            // u64 h8d = (*(u64*)&buf[2]) & 0x0f0f0f0f0f0f0f0full;
            // rz = 8 + (__builtin_clzll(h8d) >> 3 ) ;
            // if(h8d == 0) rz = 17;
        }
        // sig_len = 18 - rz;
        const u64 *exp_ptr = &exp_result[324];

        // *(u64 *)&buf[18 - rz] = e10!=0 ? exp_ptr[e10] : 0;
        // return buf + 18 - rz + (e10!=0 ?(exp_ptr[e10] >> 40):0);
        *(u64 *)&buf[17 - rz + D17_2] = exp_ptr[e10];
        //*(u64 *)&buf[sig_len] =  exp_ptr[e10];
        return buf + 17 - rz + D17_2 + (exp_ptr[e10] >> 40);
    }

    // return buf + 18 + 2 + 1 - rz + (e10 < 0) + ((e10 >= 100) | (e10 <= -100)) ;
}
static_inline char *xjb_dtoa_v2(double v, char *buf)
{
    // 8.4ns / 32cycle per double;
    // AMD R7 7840H ; icpx 2025.0.4 -O3
    buf[0] = '-';
    u64 vi = *(u64 *)&v;
    buf += vi >> 63;
    u64 sig = vi & ((1ull << 52) - 1);
    u64 exp = ((vi << 1) >> 53);
    if (unlikely(exp == 2047))
    {
        *(int *)buf = sig ? *(int *)"nan" : *(int *)"inf";
        return buf + 3;
    }
    u64 sig_bin = sig | (1ull << 52);
    int exp_bin = exp - 1075;
    if (unlikely(exp == 0))
    {
        sig_bin = sig;
        exp_bin = -1074; // 1-1075
        if (sig == 0)
        {
            buf[0] = '0';
            return buf + 1;
        }
    }
    bool irregular, round_d0, round_d1, round_u0, round_u1;
    u64 cb, p10_hi, p10_lo, sig_hi, sig_lo;
    u64 one, ten, c, half_ulp, t0, t1, dec, dec_one, dec_ten;
    i32 k, h, trim, add_ten;
    u64 sig_dec, D17;
    i32 exp_dec, e10;
    u64 sig_dec_before;
    irregular = (sig == 0);
    // if(1)
    if (unlikely(irregular))
    {
        // this core algorithm from https://github.com/ibireme/c_numconv_benchmark/blob/master/vendor/yy_double/yy_double.c
        // origin author : yy  , https://github.com/ibireme
        // modify author : xjb , 1302035400@qq.com
        // the correctness awaits verification by the origin author.
        // all the irregular number is correct;
        // irregular = (sig == 0);
        if (likely(!irregular))
            k = (exp_bin * 315653) >> 20; // high branch
        else
            k = (exp_bin * 315653 - 131237) >> 20;
        h = exp_bin + ((k * -217707) >> 16) + 1;
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        cb = sig_bin << (h);
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);
        ten = (((__uint128_t)sig_hi * 1844674407370955162) >> 64) * 10; // == (sig_hi / 10) * 10
        one = sig_hi - ten;
        c = (one << (64 - 4)) | (sig_lo >> 4);
        half_ulp = p10_hi >> (5 - h);
        t0 = ((u64)10 << (64 - 4));
        t1 = c + half_ulp;
        if (likely(!irregular)) // high branch
        {
            round_u1 = (sig_lo >= ((u64)1 << 63));
            if (unlikely(sig_lo == ((u64)1 << 63))) // low branch
            {
                round_u1 = (sig_hi & 1);
            }
            round_d0 = (half_ulp > c);
            if (unlikely(half_ulp == c)) // low branch
            {
                round_d0 = !(sig_bin & 1);
            }
        }
        else // low branch
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            if ((half_ulp / 2 < (sig_lo >> 4)))
                round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 > t0);
        if (unlikely(t0 - t1 <= (u64)1)) // low branch
        {
            // if( unlikely((k == 0) | (t0 - 1 == t1)))round_u0 = !(sig_bin & 1);
            if ((t0 - t1) <= *(u64 *)&k)
                round_u0 = !(sig_bin & 1);
        }
        exp_dec = k;
        // one = one + round_u1;
        // if (round_d0)one = 0;
        // if (round_u0)one = 10;
        // sig_dec = ten + one;
        sig_dec = sig_hi + round_u1;
        if (round_d0)
            sig_dec = ten;
        if (round_u0)
            sig_dec = ten + 10;
        // if(ten == (((u64)1e16) - 10) || ten == (((u64)1e16) )){
        //     static int sum=0;
        //     printf("ten= %llu,one = %llu,ten+one=%llu,sum=%d,v=%.16le\n",ten,one,ten+one,sum++,v);
        // }
        // D17 = sig_dec >= (u64)(1e16);
        // e10 = exp_dec + 15 + D17;
    }
    else
    {
        // author : xjb , 1302035400@qq.com
        // more fast but may generate 26 error for irregular double;
        // so if irregular we call other algorithm;
        u64 odd = sig_bin & 1;
        const int offset = 4;                                         // range [3,11]
        exp_dec = (exp_bin * 315653) >> 20;                           // exp_bin [-1074,971]    k:[-324,292]
        // if (likely(!irregular))
        //     exp_dec = (exp_bin * 315653) >> 20; // high branch
        // else
        //     exp_dec = (exp_bin * 315653 - 131237) >> 20;

        h = exp_bin + (((-(exp_dec)-1) * 217707) >> 16) + 1 + offset; // [-3,0]
        const u64 *pow10_ptr = pow10_sig_table - POW10_SIG_TABLE_MIN_EXP * 2;
        u64 *sig = (u64 *)&pow10_ptr[(-(exp_dec)-1) * 2];
        p10_hi = sig[0];
        p10_lo = sig[1];
        cb = sig_bin << (h);
#if 1
        // compute u128 * u64
        sig_hi = (cb * (u128)p10_lo) >> 64;
        sig_lo = (cb * (u128)p10_hi + sig_hi);
        sig_hi = (cb * (u128)p10_hi + sig_hi) >> 64;

        // u64 bch,ach,acl;
        // //_mulx_u64(cb,p10_lo,(unsigned long long*)&bch);
        // //acl = _mulx_u64(cb,p10_hi,(unsigned long long*)&ach);
        // bch = (cb * (u128)p10_lo) >> 64;
        // ach = (cb * (u128)p10_hi) >> 64;
        // acl = (cb * (u128)p10_hi) ;
        // sig_lo = (acl + bch);
        // sig_hi = ach + (sig_lo < bch);
#else
        sig_hi = (cb * (u128)p10_hi) >> 64;
        sig_lo = (cb * (u128)p10_hi);
#endif
        ten = (sig_hi >> offset) * 10;
        half_ulp = p10_hi >> (1 + offset - h); // 1-h1 = [1,4]
        u64 dot_one = (sig_hi << (64 - offset)) + (sig_lo >> offset);
        u64 one;//
        // u64 one = (((dot_one ) * (u128)10) >> 64) + (((u64)((dot_one ) * (u128)10)) >> 63);
        {
            // round to nearest
            one = (((dot_one) * (u128)10) >> 64) + (((u64)((dot_one) * (u128)10)) >> 63);
            // round to even
            if (unlikely((u64)((dot_one) * (u128)10) == (1ull << 63))) // generate branch instruction
                one -= ((!((((dot_one) * (u128)10) >> 64) & 1)));      // if(even) one--;
            // if (unlikely((u64)((dot_one) * (u128)10) == (1ull << 63) && ((((dot_one) * (u128)10) >> 64) & 1)==0  )) // generate branch instruction
            //     one --;      // if(even) one--;
            // adjust to correct result
            // if(unlikely(irregular)){
            // static const short special_exp[]={
            //     -1069,-1059,-1009,-843,-840,-830,-747,-541,-418,-348,-192,-149,-129,37,130,223,253,293,326,346,353,429,522,542,811,924
            // };
            // for(int i=0;i<26;++i){
            //     one+=(*(short*)&exp_bin==special_exp[i]);
            // }
            // }
        
        }
        // round up
        if (unlikely(half_ulp == ((~0) - dot_one)))// generate branch instruction
        {
            if (!odd)//cmov
                one = 10;
        } 
        if (half_ulp > (~0 - dot_one))// generate cmov instruction
            one = 10; 
        // round down
        //if (likely(!irregular)) // always true
        {
            if (unlikely((half_ulp == dot_one)))// generate branch instruction
            {
                if (!odd)//cmov
                    one = 0;
            } 
            if (half_ulp > dot_one)// generate cmov instruction
                one = 0; 
        }
        // else // not exec 
        // {
        //     if (half_ulp / 2 >= dot_one)
        //         one = 0; // generate cmov instruction
        // }
        sig_dec = ten + one;
    }
    D17 = sig_dec >= (u64)(1e16); // has 17 digit
    e10 = exp_dec + 15 + D17;
    // printf("sig_dec=%llu,ten=%llu,one=%llu,h=%d,12-h=%d,e10=%d\n",sig_dec,ten,one,h,12-h,e10);
    //  u64 rz;// range : [0,15] and 17
    u64 high9 = sig_dec / (u64)(1e8);        // 1e7 <= high9 < 1e9 . one mul and right shift
    u64 low8 = sig_dec - high9 * (u64)(1e8); // 0 <= low8 < 1e8   one mul and sub
    const unsigned int *digit_ptr = D17 ? digit_000_999 : digit_00_99;
    u64 num123 = ((u128)(high9) * (18446744073710)) >> 64;
    *(u32 *)&buf[0] = digit_ptr[num123]; // 4 byte : "x.xx" or 3 byte : "x.x"
    u64 num456_789 = ((u128)(high9) * (18446744073710));
    u64 num456 = ((u128)(num456_789) * (1000)) >> 64;
    u64 num789 = ((u64)((u128)(num456_789) * (1000)) * (u128)(1000)) >> 64;
    *(u64 *)&buf[4 - !D17] = (u64)digit1000[num456] | ((u64)digit1000[num789] << 24); // write 6 byte
    // u64 h8d = (*(u64 *)&buf[2]) & 0x0f0f0f0f0f0f0f0full;
    u64 num12 = ((u128)(low8) * (18446744073710)) >> 64;
    //*(u16 *)&buf[10] = digit100[num12];  // 2
    u64 num345_678 = ((u128)(low8) * (18446744073710));
    u64 num345 = ((u128)(num345_678) * (1000)) >> 64;
    //*(u32 *)&buf[12] = digit1000[num345]; // 3
    u64 num678 = ((u64)((u128)(num345_678) * (1000)) * (u128)(1000)) >> 64;
    //*(u32 *)&buf[15] = digit1000[num678]; // 3
    u64 low8w = (u64)digit100[num12] | ((u64)digit1000[num345] << 16) | ((u64)digit1000[num678] << 40);
    *(u64 *)&buf[10 - !D17] = low8w; // write 8 byte
    u64 rz;
    u64 low8d = low8w & 0x0f0f0f0f0f0f0f0full;
    rz = __builtin_clzll(low8d) >> 3; // low8d != 0
    u64 h8d = (*(u64 *)&buf[2]) & 0x0f0f0f0f0f0f0f0full;
    u64 rz2 = (__builtin_clzll(h8d) >> 3);
    if (h8d == 0)
        rz2 = 9;
    if (low8d == 0)
        rz = 8 + rz2 - !D17;
    // if(low8 != 0){
    //     u64 low8d = low8w & 0x0f0f0f0f0f0f0f0full;
    //     rz = __builtin_clzll(low8d) >> 3;
    // }else{
    //     u64 h8d = (*(u64 *)&buf[2]) & 0x0f0f0f0f0f0f0f0full;
    //     u64 rz2 = (__builtin_clzll(h8d) >> 3);
    //     if (h8d == 0) rz2 = 9;
    //     rz = 8 + rz2- !D17_2;
    // }

    // printf("rz=%llu\n",rz);

    const u64 *exp_ptr = &exp_result[324];
    *(u64 *)&buf[17 - rz + D17] = exp_ptr[e10];
    return buf + 17 - rz + D17 + (exp_ptr[e10] >> 40);
}

static_inline void f64_bin_to_dec_xjb(u64 sig_bin, i32 exp_bin, u64 *sig_dec, i32 *exp_dec)
{
    bool irregular, round_d0, round_d1, round_u0, round_u1;
    bool round_d01, round_d11, round_u01, round_u11;
    u64 cb, p10_hi, p10_lo, sig_hi, sig_lo;
    u64 cb1, p10_hi1, p10_lo1, sig_hi1, sig_lo1;
    u64 one, ten, c, half_ulp, t0, t1, dec, dec_one, dec_ten;
    u64 ten1, one1, c1, half_ulp1, t01, t11, dec1, dec_one1, dec_ten1;
    i32 k, h, trim, add_ten;
    i32 h1;

#if 0
    {
        int odd = sig_bin & 1;
        const double halfd = 0.05;
        const u64 half = 0x0ccccccccccccccdull;

        const int offset = 11; // range [3,11]
        irregular = (sig_bin == 0x10000000000000ull);
        k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;

        h = exp_bin + (((-k) * 217707) >> 16) + 1;//[1,4]
        //printf("h=%d\n",h);
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        cb = sig_bin << (h);
        u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);

        h1 = exp_bin + (((-k - 1) * 217707) >> 16) + 1;// [-3,0]
        //printf("h1=%d\n",h1);
        pow10_table_get_sig(-k - 1, &p10_hi1, &p10_lo1);
        // printf("h=%d,h1=%d,h1+4=%d ",h,h1,h1+4);
        cb1 = sig_bin << (h1 + offset);
        u128_mul(cb1, p10_lo1, &sig_hi1, &sig_lo1);
        u128_mul_add(cb1, p10_hi1, sig_hi1, &sig_hi1, &sig_lo1);

        // *sig_dec = sig_hi;
        // *exp_dec = k;
        // return ;

        ten = (sig_hi / 10) * 10;
        one = sig_hi - ten;

        ten1 = (sig_hi1 >> offset) * 10;
        u64 sig_lo2 = (sig_hi1<<(64 - offset) | (sig_lo1 >> offset));
        one1 = ( sig_lo2 * (__uint128_t)(10)) >> 64;
        one1 = ((sig_hi1 & ((1ull<<offset)-1)) * 10 + ((sig_lo1 * (__uint128_t)10)>>64))  >> 11 ; 
        // if(k==0)printf("k==0");
        if (ten != ten1)
        {
            // static int sum = 0;
            // printf("sum=%d,h=%d,h1=%d,h1+4=%d ", sum++, h, h1, h1 + 4);
            // printf("ten =%lu,ten1=%lu,k=%d\n", ten, ten1, k);
        }
        if(sig_hi != ten1 + one1){
            static int sum =0;
            //printf("diffsum=%d sig_hi=%llu,ten1+one1=%llu k=%d\n",sum++,sig_hi,ten1+one1,k);
        }
        // if(k==0&&ten==ten1){
        //     static int sum=0;
        //     printf("sum=%d,k==0&&ten==ten1\n",sum++);
        // }

        c = (one << (64 - 4)) | (sig_lo >> 4);
        c1 = (one1 << (64 - 4)) | (sig_lo2 >> 4);
        half_ulp = p10_hi >> (5 - h);// 5-h = [1,4]
        half_ulp1 = p10_hi1 >> (1 - h1);//1-h1 = [] 
        t0 = ((u64)10 << (64 - 4)); // const
        t1 = c + half_ulp;
        t11 = c1 + half_ulp1;
        if (!irregular) // likely
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            if (sig_lo == ((u64)1 << 63))
                round_u1 = (sig_hi & 1);
            round_d0 = (half_ulp >= c);
            if (half_ulp == c)
                round_d0 = !(sig_bin & 1);
        }
        else
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
            if (!round_d1)
                round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 >= t0);
        if (t0 - t1 <= (u64)1)
        {
            if (k == 0 || (t0 - 1 == t1))
                round_u0 = !(sig_bin & 1);
        }

        round_u01 = (half_ulp1 + sig_lo2) <= sig_lo2;
        static int allsumu0 = 0;
        if(round_u0)allsumu0++;
        if(round_u0 != round_u01){
            static int sum=0;
            //printf("  up 0 sum=%d,allsumu0=%d,u0=%d,u01=%d,k=%d\n",sum++,allsumu0,round_u0,round_u01,k);
        }

        round_d01 = half_ulp1 >= sig_lo2;
        static int allsumd0 = 0;
        if(round_d0)allsumd0++;
        if(round_d0 != round_d01){
            static int sum=0;
            //printf("down 0 sum=%d,allsumd0=%d,d0=%d,d01=%d,k=%d\n",sum++,allsumd0,round_d0,round_d01,k);
        }
        
        round_u11 = sig_lo2 >> 63;

        u64 one11 = ((sig_lo2+half) * (__uint128_t)10) >> 64;
        
 

        trim = (round_d0 + round_u0);
        add_ten = (round_u0 ? 10 : 0);
        dec_one = sig_hi + round_u1;
        dec_ten = ten + add_ten;
        dec = trim ? dec_ten : dec_one;
        gcc_load_barrier(dec_one); /* let gcc emit cmov */


        static int allsumu1=0;
        if(!trim){
            allsumu1++;
            if(dec_one != (ten1 + one11)){
                static int sum=0;
                //printf("sum=%d,allsumu1=%d,dec_one=%llu,else=%llu,diff=%llu,k=%d\n",sum++,allsumu1,dec_one,ten1 + one11,dec_one-(ten1 + one11),k);
            }
        }

        dec = ten1 + one11;
        if(round_u01)dec = ten1+10;
        if(round_d01)dec = ten1;


        *sig_dec = dec;
        *exp_dec = k;
    }
#endif

#if 0
    {
        //int odd = sig_bin & 1;
        const u64 half = 0x0ccccccccccccccdull;//9,223,372,036,854,775,810  -> 2**63 == 9,223,372,036,854,775,808

        const int offset = 3; // range [3,11]
        //irregular = (sig_bin == 0x10000000000000ull);
        //irregular = 0;
        //k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
        k = (i32)(exp_bin * 315653 ) >> 20;

        // h = exp_bin + (((-k) * 217707) >> 16) + 1;//[1,4]
        // //printf("h=%d\n",h);
        // pow10_table_get_sig(-k, &p10_hi, &p10_lo);
        // cb = sig_bin << (h);
        // u128_mul(cb, p10_lo, &sig_hi, &sig_lo);
        // u128_mul_add(cb, p10_hi, sig_hi, &sig_hi, &sig_lo);

        h1 = exp_bin + (((-k - 1) * 217707) >> 16) + 1;// [-3,0]
        //printf("h1=%d\n",h1);
        pow10_table_get_sig(-k - 1, &p10_hi1, &p10_lo1);
        // printf("h=%d,h1=%d,h1+4=%d ",h,h1,h1+4);
        cb1 = sig_bin << (h1 + offset);
        u128_mul(cb1, p10_lo1, &sig_hi1, &sig_lo1);
        u128_mul_add(cb1, p10_hi1, sig_hi1, &sig_hi1, &sig_lo1);

        // *sig_dec = sig_hi;
        // *exp_dec = k;
        // return ;

        //ten = (sig_hi / 10) * 10;
        //one = sig_hi - ten;

        ten1 = (sig_hi1 >> offset) * 10;
        u64 sig_lo2 = (sig_hi1<<(64 - offset) | (sig_lo1 >> offset));
        //one1 = ( sig_lo2 * (__uint128_t)(10)) >> 64;
        //one1 = ((sig_hi1 & ((1ull<<offset)-1)) * 10 + ((sig_lo1 * (__uint128_t)10)>>64))  >> 11 ; 
        // if(k==0)printf("k==0");
        //if (ten != ten1)
        {
            // static int sum = 0;
            // printf("sum=%d,h=%d,h1=%d,h1+4=%d ", sum++, h, h1, h1 + 4);
            // printf("ten =%lu,ten1=%lu,k=%d\n", ten, ten1, k);
        }
        //if(sig_hi != ten1 + one1)
        {
            //static int sum =0;
            //printf("diffsum=%d sig_hi=%llu,ten1+one1=%llu k=%d\n",sum++,sig_hi,ten1+one1,k);
        }
        // if(k==0&&ten==ten1){
        //     static int sum=0;
        //     printf("sum=%d,k==0&&ten==ten1\n",sum++);
        // }

        //c = (one << (64 - 4)) | (sig_lo >> 4);
        //c1 = (one1 << (64 - 4)) | (sig_lo2 >> 4);
        //half_ulp = p10_hi >> (5 - h);// 5-h = [1,4]
        half_ulp1 = p10_hi1 >> (1 - h1);//1-h1 = [] 
        //t0 = ((u64)10 << (64 - 4)); // const
        //t1 = c + half_ulp;
        //t11 = c1 + half_ulp1;
        // if (!irregular) // likely
        // {
        //     round_u1 = (sig_lo > ((u64)1 << 63));
        //     if (sig_lo == ((u64)1 << 63))
        //         round_u1 = (sig_hi & 1);
        //     round_d0 = (half_ulp >= c);
        //     if (half_ulp == c)
        //         round_d0 = !(sig_bin & 1);
        // }
        // else
        // {
        //     round_u1 = (sig_lo > ((u64)1 << 63));
        //     round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
        //     if (!round_d1)
        //         round_u1 = true;
        //     round_d0 = (half_ulp / 2 >= c);
        // }
        // round_u0 = (t1 >= t0);
        // if (t0 - t1 <= (u64)1)
        // {
        //     if (k == 0 || (t0 - 1 == t1))
        //         round_u0 = !(sig_bin & 1);
        // }

        round_u01 = (half_ulp1 + sig_lo2) <= sig_lo2;
        // static int allsumu0 = 0;
        // if(round_u0)allsumu0++;
        // if(round_u0 != round_u01){
        //     static int sum=0;
        //     //printf("  up 0 sum=%d,allsumu0=%d,u0=%d,u01=%d,k=%d\n",sum++,allsumu0,round_u0,round_u01,k);
        // }

        round_d01 = half_ulp1 >= sig_lo2;
        // static int allsumd0 = 0;
        // if(round_d0)allsumd0++;
        // if(round_d0 != round_d01){
        //     static int sum=0;
        //     //printf("down 0 sum=%d,allsumd0=%d,d0=%d,d01=%d,k=%d\n",sum++,allsumd0,round_d0,round_d01,k);
        // }
        
        round_u11 = sig_lo2 >> 63;

        u64 one11 = ((sig_lo2 + half) * (__uint128_t)10) >> 64;
        
 

        // trim = (round_d0 + round_u0);
        // add_ten = (round_u0 ? 10 : 0);
        // dec_one = sig_hi + round_u1;
        // dec_ten = ten + add_ten;
        // dec = trim ? dec_ten : dec_one;
        // gcc_load_barrier(dec_one); /* let gcc emit cmov */


        // static int allsumu1=0;
        // if(!trim){
        //     allsumu1++;
        //     if(dec_one != (ten1 + one11)){
        //         static int sum=0;
        //         //printf("sum=%d,allsumu1=%d,dec_one=%llu,else=%llu,diff=%llu,k=%d\n",sum++,allsumu1,dec_one,ten1 + one11,dec_one-(ten1 + one11),k);
        //     }
        // }

        dec = ten1 + one11;
        if(round_u01)dec = ten1+10;
        if(round_d01)dec = ten1;


        *sig_dec = dec;
        *exp_dec = k;
    }
#endif
    // 0.00038% error

    typedef __uint128_t u128;
    // const u64 half = 0x0ccccccccccccccdull;//9,223,372,036,854,775,810  -> 2**63 == 9,223,372,036,854,775,808
    // u64 odd = (sig_bin & 1); // 0 or 1
    u64 odd = 0;
    const int offset = 11; // range [3,11]

    // irregular = (sig_bin == 0x10000000000000ull);
    // if (likely(!irregular))*exp_dec = (exp_bin * 315653) >> 20;//branch
    // else *exp_dec = (exp_bin * 315653 - 131237) >> 20;

    *exp_dec = (exp_bin * 315653) >> 20; // exp_bin : [-1074,972]    k:[-324,292]
    // -k : [-292,324]
    // -k-1 : [-291,323]

    h1 = exp_bin + (((-(*exp_dec) - 1) * 217707) >> 16) + 1 + offset; // [-3,0]

    // const unsigned short *khp = &kh[1074];
    // unsigned short kh1 = khp[exp_bin];
    // h1 = kh1&3;
    // *exp_dec = (int)(kh1>>2) - 324;

    // pow10_table_get_sig(-(*exp_dec) - 1, &p10_hi1, &p10_lo1);
    const u64 *pow10_ptr = pow10_sig_table_round - POW10_SIG_TABLE_MIN_EXP * 2;
    u64 *sig = (u64 *)&pow10_ptr[(-(*exp_dec) - 1) * 2];
    p10_hi1 = sig[0];
    p10_lo1 = sig[1];

    cb1 = sig_bin << (h1); // 3+h1 = [0,3]
    // u64 cb3 = sig_bin;

    // u128_mul(cb1, p10_lo1, &sig_hi1, &sig_lo1);
    // u128_mul_add(cb1, p10_hi1, sig_hi1, &sig_hi1, &sig_lo1);

    // sig_lo1 = cb1 * (u128)p10_lo1 ;
    sig_hi1 = (cb1 * (u128)p10_lo1) >> 64;
    sig_lo1 = (cb1 * (u128)p10_hi1 + sig_hi1);
    sig_hi1 = (cb1 * (u128)p10_hi1 + sig_hi1) >> 64;

    // u64 sig_hi3 = (cb3 * (u128)p10_lo1) >> 64;
    // u64 sig_lo3 = (cb3 * (u128)p10_hi1 + sig_hi3 );
    //     sig_hi3 = (cb3 * (u128)p10_hi1 + sig_hi3 ) >> 64;

    ten1 = (sig_hi1 >> offset) * 10;
    // ten =  (sig_hi1 >> offset);
    //  ten1 = ((sig_hi1 >> offset) << 3 ) + ((sig_hi1 >> offset)) + ((sig_hi1 >> offset));
    //   u64 ten3 = (sig_hi3 >> (offset - h1)) * 10;//
    half_ulp1 = p10_hi1 >> (1 + offset - h1); // 1-h1 = [1,4]
    // u64 half_ulp3 = p10_hi1 >> (1 + offset - h1);
    // u64 one1 = 0;

    u64 sig_lo2 = (sig_hi1 << (64 - offset)) + (sig_lo1 >> offset);

    // one1 = (((sig_lo2 + 0x0ccccccccccccccdull) * (u128)10) >> 64);
    one1 = (((sig_lo) * (u128)10) >> 64) + (((u64)((sig_lo) * (u128)10)) >> 63);

    dec = ten1 + one1; // round to nearst

    // if(half_ulp1 - odd > (~0 - sig_lo2)) one1 = 10;
    // if(half_ulp1 > sig_lo2 + odd) one1=0;
    // *sig_dec = ten1 + one1;
    // one1 = (sig_lo2 * (u128)(10)) >> 64;
    // u64 rest = (sig_lo2 * (u128)(10));
    // round_u1 = rest>>63;
    // if(unlikely(rest == (1ull<<63))){
    //     //printf("r=1<<63,k=%d\n",*exp_dec);
    //     round_u1 = one & 1;
    // }
    // *sig_dec = ten1 + one1 + round_u1;
    // u64 sig_lo2 = (sig_hi1 << (64 - offset)) ;
    // *sig_dec = ten1 + (((sig_lo2 + 0x0ccccccccccccccdull) * (u128)10) >> 64);//round to nearst

    //*sig_dec = ten1 + ((  ((sig_lo2 + 0x0ccccccccccccccdull) >> 4) * 10) >> 60);
    //*sig_dec = ten3 + (((sig_lo3 + 0x0ccccccccccccccdull) * (u128)10) >> 64);//round to nearst
    // if ((half_ulp1 + sig_lo2 ) < sig_lo2 - odd) *sig_dec = ten1 + 10; // round up

    // use cmov , not branch;

    // if ((half_ulp1 - odd > (~0 - sig_lo2))) *sig_dec = ten1 + 10; // round up
    // if ((half_ulp1 > sig_lo2 + odd)) *sig_dec = ten1; // round down

    // if (half_ulp1 - odd > (~0 - sig_lo2)) ten ++;
    // if (half_ulp1 - odd > (~0 - sig_lo2)) one1=0;
    // if (half_ulp1 > sig_lo2 + odd) one1=0;
    // *sig_dec = ten*10 + one1;
    //*sig_dec=dec;
    //*exp_dec=k;
    // return dec;
    // *sig_dec = (half_ulp1 - odd > (~0 - sig_lo2)) ? ten1 + 10 : *sig_dec;
    // *sig_dec = (half_ulp1 >= sig_lo2 + odd) ? ten1 : *sig_dec;
    // dec = (half_ulp1 - odd > (~0 - sig_lo2)) ? ten1 + 10 : dec;
    // dec = (half_ulp1 >= sig_lo2 + odd) ? ten1 : dec;
    dec = (half_ulp1 - odd > (~0 - sig_lo2)) ? ten1 + 10 : dec;
    dec = (half_ulp1 >= sig_lo2 + odd) ? ten1 : dec;

    // u64 condition_up = (half_ulp1 - odd > (~0 - sig_lo2));
    // u64 condition_down = (half_ulp1 >= sig_lo2 + odd);
    // u64 true_mask_up = -condition_up ;
    // u64 true_mask_down = -condition_down ;
    // dec = ten1 + one1 + ((10 - one1) & true_mask_up) - (one1 & true_mask_down);

    *sig_dec = dec;

    // *sig_dec = (ten1 + 10) *  (half_ulp1 - odd > (~0 - sig_lo2)) + (*sig_dec ) * (half_ulp1 - odd <= (~0 - sig_lo2));
    // *sig_dec = (ten1) *   (half_ulp1 >= sig_lo2 + odd) + (*sig_dec ) *  (half_ulp1 < sig_lo2 + odd);

    // *sig_dec = ( half_ulp3 - odd > (~0 - sig_lo3)) ? ten3 + 10 : *sig_dec;
    // *sig_dec = ( half_ulp3  >= sig_lo3 + odd ) ? ten3 : *sig_dec;
    // return 0;
}

//__always_inline
//__attribute__((optimize("if-conversion2"),always_inline))
void f64_bin_to_dec_xjb_v2(u64 sig_bin, i32 exp_bin, u64 *sig_dec, i32 *exp_dec)
{
    typedef __uint128_t u128;
    int odd = sig_bin & 1;
    const int offset = 10 ; // range [3,10] all right
    const u64 maskL = (1ull<<offset) - 1;
    u64 irregular = (sig_bin == 0x10000000000000ull);
    if (likely(!irregular))
        *exp_dec = (exp_bin * 315653) >> 20; // branch
    else
        *exp_dec = (exp_bin * 315653 - 131237) >> 20;
    // if (unlikely(irregular))
    // {
    //     // only exp_bin range [1,2046] , all 2046 double , generate correct result
    //     *exp_dec = (exp_bin * 315653 - 131237) >> 20;
    //     int h = exp_bin + ((*exp_dec * -217707) >> 16) + 1;
    //     const u64 *pow10_ptr = pow10_sig_table - POW10_SIG_TABLE_MIN_EXP * 2;
    //     u64 *sig = (u64 *)&pow10_ptr[(-(*exp_dec)) * 2];
    //     u64 p10_hi = sig[0];
    //     u64 p10_lo = sig[1];
    //     u64 cb = sig_bin << (h);
    //     u64 sig_hi = (cb * (u128)p10_lo) >> 64;
    //     u64 sig_lo = (cb * (u128)p10_hi + sig_hi);
    //     sig_hi = (cb * (u128)p10_hi + sig_hi) >> 64;
    //     u64 ten = (((u128)sig_hi * 1844674407370955162) >> 64) * 10; // sig_hi / 10 * 10
    //     u64 one = sig_hi - ten;
    //     u64 c = (one << (64 - 4)) | (sig_lo >> 4);
    //     u64 half_ulp = p10_hi >> (5 - h);
    //     const u64 t0 = ((u64)10 << (64 - 4)); // const
    //     u64 t1 = c + half_ulp;
    //     u64 round_u1 = (sig_lo > ((u64)1 << 63));
    //     if ((half_ulp < (sig_lo / 8))) round_u1 = true;
    //     u64 round_d0 = (half_ulp / 2 >= c);
    //     u64 round_u0 = (t1 > t0);
    //     *sig_dec = sig_hi + round_u1;
    //     if (round_d0)
    //         *sig_dec = ten;
    //     if (round_u0)
    //         *sig_dec = ten + 10;
    //     return;
    // }
    //*exp_dec = (exp_bin * 315653) >> 20; // exp_bin : [-1074,972]    k:[-324,292]
    int h1 = exp_bin + (((-(*exp_dec) - 1) * 217707) >> 16) + 1 + offset; // [-3,0] + offset
    // get 10^(-(*exp_dec) - 1)  from  pow10 u128 lut
    const u64 *pow10_ptr = pow10_sig_table - POW10_SIG_TABLE_MIN_EXP * 2;
    u64 *sig = (u64 *)&pow10_ptr[(-(*exp_dec) - 1) * 2];
    u64 p10_hi = sig[0];
    u64 p10_lo = sig[1];
    // u128 * u64 = u192 ----> get high 128 bit
    u64 cb = sig_bin << (h1); // h1 = [0,3]
    u64 sig_hi = (cb * (u128)p10_lo) >> 64;
    u64 sig_lo = (cb * (u128)p10_hi + sig_hi);
    sig_hi = (cb * (u128)p10_hi + sig_hi) >> 64;
    //select  from  ten , ten + one , ten + one + 1 , ten + 10
    u64 ten = (sig_hi >> offset) * 10;
    u64 half_ulp = p10_hi >> (1 + offset - h1); // 1+offset-h1 -> range [1,4]
    u64 dot_one = (sig_hi << (64 - offset)) + (sig_lo >> offset);
    u64 one ;
    // u64 one = (((dot_one ) * (u128)10) >> 64) + (((u64)((dot_one ) * (u128)10)) >> 63);
    {
        //round to nearest
        u64 rest = ((sig_lo & maskL)*10) >> offset;
        one = (((dot_one) * (u128)10) >> 64) + (((u64)((dot_one) * (u128)10) + rest) >> 63);
        //round to even
        if (unlikely((u64)((dot_one) * (u128)10) + rest == (1ull << 63))) // generate branch instruction
            one -= ((!((((dot_one) * (u128)10) >> 64) & 1)));      // if(even) one--;
        //adjust to correct result
        if(unlikely(irregular)){
            static const short special_exp[]={
                -1069,-1059,-1009,-843,-840,-830,-747,-541,-418,-348,-192,-149,-129,37,130,223,253,293,326,346,353,429,522,542,811,924
            };
            for(int i=0;i<26;++i){
                one+=(*(short*)&exp_bin==special_exp[i]);
            }
        }
    }
    // round up
    if (unlikely(half_ulp == ((~0) - dot_one)))// generate branch instruction
    {
        if (!odd)
            one = 10;
    } 
    if (half_ulp > (~0 - dot_one))// generate cmov instruction
        one = 10; 
    // round down
    if (likely(!irregular))
    {
        if (unlikely((half_ulp == dot_one)))// generate branch instruction
        {
            if (!odd)
                one = 0;
        } 
        if (half_ulp > dot_one)// generate cmov instruction
            one = 0; 
    }
    else
    {
        if (half_ulp / 2 >= dot_one)// generate cmov instruction
            one = 0;
    }
    
    u64 dec = ten + one;
    *sig_dec = dec;
}

/**
 Write a double number (require 40 bytes).

 We follows the ECMAScript specification to print floating point numbers,
 but with the following changes:
 1. Keep the negative sign of 0.0 to preserve input information.
 2. Keep decimal point to indicate the number is floating point.
 3. Remove positive sign of exponent part.
*/
static_inline u8 *write_f64_raw(u8 *buf, u64 raw)
{
    u64 sig_bin, sig_dec, sig_raw;
    i32 exp_bin, exp_dec, exp_raw, sig_len, dot_ofs;
    u8 *end;

    buf[0] = '-';
    buf += (raw >> (F64_BITS - 1));

    const u64 M63 = ((u64)(1) << 63) - 1;
    u64 raw_abs = raw & M63;
    double val_abs = *(double *)&raw_abs;
    u64 val_cvt_u64 = val_abs;           // cvt to u64
    double u64_cvt_double = val_cvt_u64; // cvt to double
    u64 val_abs_cvt_u64 = *(u64 *)&u64_cvt_double;
    // if( (val_abs_cvt_u64 == raw_abs) & (val_cvt_u64 <= (((u64)(1)<<53) - 1) ) )
    if ((val_abs_cvt_u64 == raw_abs) & (val_cvt_u64 < (u64)(1e16)))
    // if ((val_abs_cvt_u64 == raw_abs) & (val_cvt_u64 < (u64)(1e17))) // max digit 17
    {
        // when digit is 17 may not shortest , right but not shortest;

        // printf("val=%.16le int = %lld\n",val_abs,val_cvt_u64);
        //  if(raw_abs==0){
        //      byte_copy_4(buf, "0.0");
        //      return buf + 3;
        //  }
        // return write_u64_len_1_to_16(val_cvt_u64, buf);
        buf = write_u64_len_1_to_16(val_cvt_u64, buf);
        byte_copy_2(buf, ".0");
        return buf + 2;
    }

    /* decode raw bytes from IEEE-754 double format. */
    sig_raw = raw & F64_SIG_MASK;
    exp_raw = raw_abs >> F64_SIG_BITS;
    // exp_raw = (u32)((raw & F64_EXP_MASK) >> F64_SIG_BITS);

    /* return inf or nan */
    if (unlikely(exp_raw == ((u32)1 << F64_EXP_BITS) - 1))
    {
        if (sig_raw == 0)
        {
            // buf[0] = '-';
            // buf += (raw >> (F64_BITS - 1));
            byte_copy_8(buf, "Infinity");
            return buf + 8;
        }
        else
        {
            byte_copy_4(buf, "NaN");
            return buf + 3;
        }
    }

    /* add sign for all finite number */
    // buf[0] = '-';
    // buf += (raw >> (F64_BITS - 1));

    // /* return zero */
    // //if (unlikely((raw << 1) == 0))
    // if(raw_abs==0)
    // {
    //     byte_copy_4(buf, "0.0");
    //     return buf + 3;
    // }
    memset(buf, '0', 16);

    if (likely(exp_raw != 0))
    {
        /* normal number */
        sig_bin = sig_raw | ((u64)1 << F64_SIG_BITS);
        exp_bin = (i32)exp_raw - F64_EXP_BIAS - F64_SIG_BITS;

        /* fast path for small integer number without fraction */
        if (0)
            if (((-F64_SIG_BITS <= exp_bin) && (exp_bin <= 0)) &&
                (u64_tz_bits(sig_bin) >= (u32)-exp_bin))
            {
                sig_dec = sig_bin >> -exp_bin; /* range: [1, 0x1FFFFFFFFFFFFF] */
                buf = write_u64_len_1_to_16(sig_dec, buf);
                byte_copy_2(buf, ".0");
                return buf + 2;
            }

        /* binary to decimal */
        f64_bin_to_dec(sig_bin, exp_bin, &sig_dec, &exp_dec);

        /* the significand length is 16 or 17 */
        sig_len = 16 + (sig_dec >= (u64)100000000 * 100000000);

        /* the decimal point offset relative to the first digit */
        dot_ofs = sig_len + exp_dec;

        // if (likely(-6 < dot_ofs && dot_ofs <= 21))
        if (likely(-6 <= dot_ofs && dot_ofs <= 8))
        {
            /* write without scientific notation, e.g. 0.001234, 123400.0 */
            i32 no_pre_zero, pre_ofs, num_sep_pos, dot_set_pos;
            u8 *num_hdr, *num_end, *num_sep, *dot_end;

            /* fill zeros */
            // memset(buf, '0', 32);
            // memset(buf, '0', 16);

            /* calculate the offset before the number */
            no_pre_zero = (dot_ofs > 0);
            pre_ofs = no_pre_zero ? 0 : (2 - dot_ofs);
            gcc_full_barrier(no_pre_zero); /* let gcc emit cmov */
            gcc_full_barrier(pre_ofs);

            /* write the number as digits */
            num_hdr = buf + pre_ofs;
            num_end = write_u64_len_16_to_17_trim(sig_dec, num_hdr);

            /* seperate these digits to leave a space for the dot */
            num_sep_pos = no_pre_zero ? dot_ofs : 0;
            num_sep = num_hdr + num_sep_pos;
            byte_move_16(num_sep + no_pre_zero, num_sep);
            num_end += no_pre_zero;

            /* write the dot */
            dot_set_pos = yy_max(dot_ofs, 1);
            buf[dot_set_pos] = '.';

            /* return the ending */
            dot_end = buf + dot_ofs + 2;
            return yy_max(dot_end, num_end);
        }
        else
        {
            /* write with scientific notation, e.g. 1.234e56 */
            end = write_u64_len_16_to_17_trim(sig_dec, buf + 1);
            end -= (end == buf + 2); /* remove '.0', e.g. 2.0e34 -> 2e34 */
            exp_dec += sig_len - 1;
            buf[0] = buf[1];
            buf[1] = '.';
            return write_f64_exp(exp_dec, end);
        }
    }
    else
    {
        /* subnormal number */
        sig_bin = sig_raw;
        exp_bin = 1 - F64_EXP_BIAS - F64_SIG_BITS; // const

        /* binary to decimal */
        f64_bin_to_dec(sig_bin, exp_bin, &sig_dec, &exp_dec);

        /* write significand part */
        end = write_u64_len_1_to_17(sig_dec, buf + 1);
        buf[0] = buf[1];
        buf[1] = '.';
        exp_dec += (i32)(end - buf) - 2;

        /* trim trailing zeros */
        end -= *(end - 1) == '0'; /* branchless for last zero */
        end -= *(end - 1) == '0'; /* branchless for second last zero */
        while (*(end - 1) == '0')
            end--;                /* for unlikely more zeros */
        end -= *(end - 1) == '.'; /* remove dot, e.g. 2.e-321 -> 2e-321 */

        /* write exponent part */
        return write_f64_exp(exp_dec, end);
    }
}

#if 0
typedef struct u64x2
{
    u64 hi;
    u64 lo;
} u64x2;
static inline u64 RoundToOdd(u64x2 g, u64 cp)
{
    //__extension__ using uint128_t = unsigned __int128;

    const u128 x = (u128)(cp) * (u128)g.lo;
    const u128 y = (u128)(cp) * (u128)g.hi + (u128)(x >> 64);

    const uint64_t y0 = (u64)(y);
    const uint64_t y1 = (u64)(y >> 64);
    return y1 | (y0 > 1);
}


static inline int d2s_xjb(double v, char *buffer)
{
    static const long long exp_result3[324 + 308 + 1] = {0x3432332d65, 0x3332332d65, 0x3232332d65, 0x3132332d65, 0x3032332d65, 0x3931332d65, 0x3831332d65, 0x3731332d65, 0x3631332d65, 0x3531332d65, 0x3431332d65, 0x3331332d65, 0x3231332d65, 0x3131332d65, 0x3031332d65, 0x3930332d65, 0x3830332d65, 0x3730332d65, 0x3630332d65, 0x3530332d65, 0x3430332d65, 0x3330332d65, 0x3230332d65, 0x3130332d65, 0x3030332d65, 0x3939322d65, 0x3839322d65, 0x3739322d65, 0x3639322d65, 0x3539322d65, 0x3439322d65, 0x3339322d65, 0x3239322d65, 0x3139322d65, 0x3039322d65, 0x3938322d65, 0x3838322d65, 0x3738322d65, 0x3638322d65, 0x3538322d65, 0x3438322d65, 0x3338322d65, 0x3238322d65, 0x3138322d65, 0x3038322d65, 0x3937322d65, 0x3837322d65, 0x3737322d65, 0x3637322d65, 0x3537322d65, 0x3437322d65, 0x3337322d65, 0x3237322d65, 0x3137322d65, 0x3037322d65, 0x3936322d65, 0x3836322d65, 0x3736322d65, 0x3636322d65, 0x3536322d65, 0x3436322d65, 0x3336322d65, 0x3236322d65, 0x3136322d65, 0x3036322d65, 0x3935322d65, 0x3835322d65, 0x3735322d65, 0x3635322d65, 0x3535322d65, 0x3435322d65, 0x3335322d65, 0x3235322d65, 0x3135322d65, 0x3035322d65, 0x3934322d65, 0x3834322d65, 0x3734322d65, 0x3634322d65, 0x3534322d65, 0x3434322d65, 0x3334322d65, 0x3234322d65, 0x3134322d65, 0x3034322d65, 0x3933322d65, 0x3833322d65, 0x3733322d65, 0x3633322d65, 0x3533322d65, 0x3433322d65, 0x3333322d65, 0x3233322d65, 0x3133322d65, 0x3033322d65, 0x3932322d65, 0x3832322d65, 0x3732322d65, 0x3632322d65, 0x3532322d65, 0x3432322d65, 0x3332322d65, 0x3232322d65, 0x3132322d65, 0x3032322d65, 0x3931322d65, 0x3831322d65, 0x3731322d65, 0x3631322d65, 0x3531322d65, 0x3431322d65, 0x3331322d65, 0x3231322d65, 0x3131322d65, 0x3031322d65, 0x3930322d65, 0x3830322d65, 0x3730322d65, 0x3630322d65, 0x3530322d65, 0x3430322d65, 0x3330322d65, 0x3230322d65, 0x3130322d65, 0x3030322d65, 0x3939312d65, 0x3839312d65, 0x3739312d65, 0x3639312d65, 0x3539312d65, 0x3439312d65, 0x3339312d65, 0x3239312d65, 0x3139312d65, 0x3039312d65, 0x3938312d65, 0x3838312d65, 0x3738312d65, 0x3638312d65, 0x3538312d65, 0x3438312d65, 0x3338312d65, 0x3238312d65, 0x3138312d65, 0x3038312d65, 0x3937312d65, 0x3837312d65, 0x3737312d65, 0x3637312d65, 0x3537312d65, 0x3437312d65, 0x3337312d65, 0x3237312d65, 0x3137312d65, 0x3037312d65, 0x3936312d65, 0x3836312d65, 0x3736312d65, 0x3636312d65, 0x3536312d65, 0x3436312d65, 0x3336312d65, 0x3236312d65, 0x3136312d65, 0x3036312d65, 0x3935312d65, 0x3835312d65, 0x3735312d65, 0x3635312d65, 0x3535312d65, 0x3435312d65, 0x3335312d65, 0x3235312d65, 0x3135312d65, 0x3035312d65, 0x3934312d65, 0x3834312d65, 0x3734312d65, 0x3634312d65, 0x3534312d65, 0x3434312d65, 0x3334312d65, 0x3234312d65, 0x3134312d65, 0x3034312d65, 0x3933312d65, 0x3833312d65, 0x3733312d65, 0x3633312d65, 0x3533312d65, 0x3433312d65, 0x3333312d65, 0x3233312d65, 0x3133312d65, 0x3033312d65, 0x3932312d65, 0x3832312d65, 0x3732312d65, 0x3632312d65, 0x3532312d65, 0x3432312d65, 0x3332312d65, 0x3232312d65, 0x3132312d65, 0x3032312d65, 0x3931312d65, 0x3831312d65, 0x3731312d65, 0x3631312d65, 0x3531312d65, 0x3431312d65, 0x3331312d65, 0x3231312d65, 0x3131312d65, 0x3031312d65, 0x3930312d65, 0x3830312d65, 0x3730312d65, 0x3630312d65, 0x3530312d65, 0x3430312d65, 0x3330312d65, 0x3230312d65, 0x3130312d65, 0x3030312d65, 0x39392d65, 0x38392d65, 0x37392d65, 0x36392d65, 0x35392d65, 0x34392d65, 0x33392d65, 0x32392d65, 0x31392d65, 0x30392d65, 0x39382d65, 0x38382d65, 0x37382d65, 0x36382d65, 0x35382d65, 0x34382d65, 0x33382d65, 0x32382d65, 0x31382d65, 0x30382d65, 0x39372d65, 0x38372d65, 0x37372d65, 0x36372d65, 0x35372d65, 0x34372d65, 0x33372d65, 0x32372d65, 0x31372d65, 0x30372d65, 0x39362d65, 0x38362d65, 0x37362d65, 0x36362d65, 0x35362d65, 0x34362d65, 0x33362d65, 0x32362d65, 0x31362d65, 0x30362d65, 0x39352d65, 0x38352d65, 0x37352d65, 0x36352d65, 0x35352d65, 0x34352d65, 0x33352d65, 0x32352d65, 0x31352d65, 0x30352d65, 0x39342d65, 0x38342d65, 0x37342d65, 0x36342d65, 0x35342d65, 0x34342d65, 0x33342d65, 0x32342d65, 0x31342d65, 0x30342d65, 0x39332d65, 0x38332d65, 0x37332d65, 0x36332d65, 0x35332d65, 0x34332d65, 0x33332d65, 0x32332d65, 0x31332d65, 0x30332d65, 0x39322d65, 0x38322d65, 0x37322d65, 0x36322d65, 0x35322d65, 0x34322d65, 0x33322d65, 0x32322d65, 0x31322d65, 0x30322d65, 0x39312d65, 0x38312d65, 0x37312d65, 0x36312d65, 0x35312d65, 0x34312d65, 0x33312d65, 0x32312d65, 0x31312d65, 0x30312d65, 0x39302d65, 0x38302d65, 0x37302d65, 0x36302d65, 0x35302d65, 0x34302d65, 0x33302d65, 0x32302d65, 0x31302d65, 0x30302b65, 0x31302b65, 0x32302b65, 0x33302b65, 0x34302b65, 0x35302b65, 0x36302b65, 0x37302b65, 0x38302b65, 0x39302b65, 0x30312b65, 0x31312b65, 0x32312b65, 0x33312b65, 0x34312b65, 0x35312b65, 0x36312b65, 0x37312b65, 0x38312b65, 0x39312b65, 0x30322b65, 0x31322b65, 0x32322b65, 0x33322b65, 0x34322b65, 0x35322b65, 0x36322b65, 0x37322b65, 0x38322b65, 0x39322b65, 0x30332b65, 0x31332b65, 0x32332b65, 0x33332b65, 0x34332b65, 0x35332b65, 0x36332b65, 0x37332b65, 0x38332b65, 0x39332b65, 0x30342b65, 0x31342b65, 0x32342b65, 0x33342b65, 0x34342b65, 0x35342b65, 0x36342b65, 0x37342b65, 0x38342b65, 0x39342b65, 0x30352b65, 0x31352b65, 0x32352b65, 0x33352b65, 0x34352b65, 0x35352b65, 0x36352b65, 0x37352b65, 0x38352b65, 0x39352b65, 0x30362b65, 0x31362b65, 0x32362b65, 0x33362b65, 0x34362b65, 0x35362b65, 0x36362b65, 0x37362b65, 0x38362b65, 0x39362b65, 0x30372b65, 0x31372b65, 0x32372b65, 0x33372b65, 0x34372b65, 0x35372b65, 0x36372b65, 0x37372b65, 0x38372b65, 0x39372b65, 0x30382b65, 0x31382b65, 0x32382b65, 0x33382b65, 0x34382b65, 0x35382b65, 0x36382b65, 0x37382b65, 0x38382b65, 0x39382b65, 0x30392b65, 0x31392b65, 0x32392b65, 0x33392b65, 0x34392b65, 0x35392b65, 0x36392b65, 0x37392b65, 0x38392b65, 0x39392b65, 0x3030312b65, 0x3130312b65, 0x3230312b65, 0x3330312b65, 0x3430312b65, 0x3530312b65, 0x3630312b65, 0x3730312b65, 0x3830312b65, 0x3930312b65, 0x3031312b65, 0x3131312b65, 0x3231312b65, 0x3331312b65, 0x3431312b65, 0x3531312b65, 0x3631312b65, 0x3731312b65, 0x3831312b65, 0x3931312b65, 0x3032312b65, 0x3132312b65, 0x3232312b65, 0x3332312b65, 0x3432312b65, 0x3532312b65, 0x3632312b65, 0x3732312b65, 0x3832312b65, 0x3932312b65, 0x3033312b65, 0x3133312b65, 0x3233312b65, 0x3333312b65, 0x3433312b65, 0x3533312b65, 0x3633312b65, 0x3733312b65, 0x3833312b65, 0x3933312b65, 0x3034312b65, 0x3134312b65, 0x3234312b65, 0x3334312b65, 0x3434312b65, 0x3534312b65, 0x3634312b65, 0x3734312b65, 0x3834312b65, 0x3934312b65, 0x3035312b65, 0x3135312b65, 0x3235312b65, 0x3335312b65, 0x3435312b65, 0x3535312b65, 0x3635312b65, 0x3735312b65, 0x3835312b65, 0x3935312b65, 0x3036312b65, 0x3136312b65, 0x3236312b65, 0x3336312b65, 0x3436312b65, 0x3536312b65, 0x3636312b65, 0x3736312b65, 0x3836312b65, 0x3936312b65, 0x3037312b65, 0x3137312b65, 0x3237312b65, 0x3337312b65, 0x3437312b65, 0x3537312b65, 0x3637312b65, 0x3737312b65, 0x3837312b65, 0x3937312b65, 0x3038312b65, 0x3138312b65, 0x3238312b65, 0x3338312b65, 0x3438312b65, 0x3538312b65, 0x3638312b65, 0x3738312b65, 0x3838312b65, 0x3938312b65, 0x3039312b65, 0x3139312b65, 0x3239312b65, 0x3339312b65, 0x3439312b65, 0x3539312b65, 0x3639312b65, 0x3739312b65, 0x3839312b65, 0x3939312b65, 0x3030322b65, 0x3130322b65, 0x3230322b65, 0x3330322b65, 0x3430322b65, 0x3530322b65, 0x3630322b65, 0x3730322b65, 0x3830322b65, 0x3930322b65, 0x3031322b65, 0x3131322b65, 0x3231322b65, 0x3331322b65, 0x3431322b65, 0x3531322b65, 0x3631322b65, 0x3731322b65, 0x3831322b65, 0x3931322b65, 0x3032322b65, 0x3132322b65, 0x3232322b65, 0x3332322b65, 0x3432322b65, 0x3532322b65, 0x3632322b65, 0x3732322b65, 0x3832322b65, 0x3932322b65, 0x3033322b65, 0x3133322b65, 0x3233322b65, 0x3333322b65, 0x3433322b65, 0x3533322b65, 0x3633322b65, 0x3733322b65, 0x3833322b65, 0x3933322b65, 0x3034322b65, 0x3134322b65, 0x3234322b65, 0x3334322b65, 0x3434322b65, 0x3534322b65, 0x3634322b65, 0x3734322b65, 0x3834322b65, 0x3934322b65, 0x3035322b65, 0x3135322b65, 0x3235322b65, 0x3335322b65, 0x3435322b65, 0x3535322b65, 0x3635322b65, 0x3735322b65, 0x3835322b65, 0x3935322b65, 0x3036322b65, 0x3136322b65, 0x3236322b65, 0x3336322b65, 0x3436322b65, 0x3536322b65, 0x3636322b65, 0x3736322b65, 0x3836322b65, 0x3936322b65, 0x3037322b65, 0x3137322b65, 0x3237322b65, 0x3337322b65, 0x3437322b65, 0x3537322b65, 0x3637322b65, 0x3737322b65, 0x3837322b65, 0x3937322b65, 0x3038322b65, 0x3138322b65, 0x3238322b65, 0x3338322b65, 0x3438322b65, 0x3538322b65, 0x3638322b65, 0x3738322b65, 0x3838322b65, 0x3938322b65, 0x3039322b65, 0x3139322b65, 0x3239322b65, 0x3339322b65, 0x3439322b65, 0x3539322b65, 0x3639322b65, 0x3739322b65, 0x3839322b65, 0x3939322b65, 0x3030332b65, 0x3130332b65, 0x3230332b65, 0x3330332b65, 0x3430332b65, 0x3530332b65, 0x3630332b65, 0x3730332b65, 0x3830332b65};

    //============ README start===========

    // this code base on Schubafach algorithm
    // sse implementation
    // I have limited time, so the performance may not be optimal.

    // performance test result
    // when input double is random
    // CPU : AMD R7 7840H ; use icpx 2025.0.4 -O3 compile this func
    // performance: this func cost 12.1ns per double , 1.82x faster than dragonbox , 2x faster than schubafach
    // dragonbox  cost 22ns per double
    // schubafach cost 25ns per double
    // ryu        cost 27ns per double

    // return write buffer length
    // recommend set buffer length is 32 byte;

    // input double v range and print result ;
    // range       : double v ->   print result
    // 1. v<1e-7   : 1.23e-8  ->   1.23e-08  ,  1.23e-11 -> 1.23e-11  , 1.23e-103  -> 1.23e-103
    // 2. [1e-7,1) : 1.23e-3  ->   0.00123   ,  1.23e-1  -> 0.123
    // 3. [1,1e9)  : 1.23e2   ->   123       ,  1.23e5   -> 12300
    // 4. >=1e9    : 1.23e10  ->   1.23e+10  ,  1.23e9   -> 1.23e+09  , 1.23e103   -> 1.23e+103

    // special value : 0 -> 0 , nan -> nan , inf -> inf

    // The code may not be the final version , may optimize performance
    //============ README end===========

    // caller-saved registers
    // rax, rcx, rdx , r8-r11 , xmm0-xmm5
    // 7 + 6 = 13 registers
    // how to reduce register use , how to use xmm registers to improve performance
    typedef unsigned long long ull;
    typedef long long ll;

    const int kMin = -292;
    const int kMax = 324;
    static const u64x2 g[kMax - kMin + 1] = {
        {0xFF77B1FCBEBCDC4F, 0x25E8E89C13BB0F7B}, // -292
        {0x9FAACF3DF73609B1, 0x77B191618C54E9AD}, // -291
        {0xC795830D75038C1D, 0xD59DF5B9EF6A2418}, // -290
        {0xF97AE3D0D2446F25, 0x4B0573286B44AD1E}, // -289
        {0x9BECCE62836AC577, 0x4EE367F9430AEC33}, // -288
        {0xC2E801FB244576D5, 0x229C41F793CDA740}, // -287
        {0xF3A20279ED56D48A, 0x6B43527578C11110}, // -286
        {0x9845418C345644D6, 0x830A13896B78AAAA}, // -285
        {0xBE5691EF416BD60C, 0x23CC986BC656D554}, // -284
        {0xEDEC366B11C6CB8F, 0x2CBFBE86B7EC8AA9}, // -283
        {0x94B3A202EB1C3F39, 0x7BF7D71432F3D6AA}, // -282
        {0xB9E08A83A5E34F07, 0xDAF5CCD93FB0CC54}, // -281
        {0xE858AD248F5C22C9, 0xD1B3400F8F9CFF69}, // -280
        {0x91376C36D99995BE, 0x23100809B9C21FA2}, // -279
        {0xB58547448FFFFB2D, 0xABD40A0C2832A78B}, // -278
        {0xE2E69915B3FFF9F9, 0x16C90C8F323F516D}, // -277
        {0x8DD01FAD907FFC3B, 0xAE3DA7D97F6792E4}, // -276
        {0xB1442798F49FFB4A, 0x99CD11CFDF41779D}, // -275
        {0xDD95317F31C7FA1D, 0x40405643D711D584}, // -274
        {0x8A7D3EEF7F1CFC52, 0x482835EA666B2573}, // -273
        {0xAD1C8EAB5EE43B66, 0xDA3243650005EED0}, // -272
        {0xD863B256369D4A40, 0x90BED43E40076A83}, // -271
        {0x873E4F75E2224E68, 0x5A7744A6E804A292}, // -270
        {0xA90DE3535AAAE202, 0x711515D0A205CB37}, // -269
        {0xD3515C2831559A83, 0x0D5A5B44CA873E04}, // -268
        {0x8412D9991ED58091, 0xE858790AFE9486C3}, // -267
        {0xA5178FFF668AE0B6, 0x626E974DBE39A873}, // -266
        {0xCE5D73FF402D98E3, 0xFB0A3D212DC81290}, // -265
        {0x80FA687F881C7F8E, 0x7CE66634BC9D0B9A}, // -264
        {0xA139029F6A239F72, 0x1C1FFFC1EBC44E81}, // -263
        {0xC987434744AC874E, 0xA327FFB266B56221}, // -262
        {0xFBE9141915D7A922, 0x4BF1FF9F0062BAA9}, // -261
        {0x9D71AC8FADA6C9B5, 0x6F773FC3603DB4AA}, // -260
        {0xC4CE17B399107C22, 0xCB550FB4384D21D4}, // -259
        {0xF6019DA07F549B2B, 0x7E2A53A146606A49}, // -258
        {0x99C102844F94E0FB, 0x2EDA7444CBFC426E}, // -257
        {0xC0314325637A1939, 0xFA911155FEFB5309}, // -256
        {0xF03D93EEBC589F88, 0x793555AB7EBA27CB}, // -255
        {0x96267C7535B763B5, 0x4BC1558B2F3458DF}, // -254
        {0xBBB01B9283253CA2, 0x9EB1AAEDFB016F17}, // -253
        {0xEA9C227723EE8BCB, 0x465E15A979C1CADD}, // -252
        {0x92A1958A7675175F, 0x0BFACD89EC191ECA}, // -251
        {0xB749FAED14125D36, 0xCEF980EC671F667C}, // -250
        {0xE51C79A85916F484, 0x82B7E12780E7401B}, // -249
        {0x8F31CC0937AE58D2, 0xD1B2ECB8B0908811}, // -248
        {0xB2FE3F0B8599EF07, 0x861FA7E6DCB4AA16}, // -247
        {0xDFBDCECE67006AC9, 0x67A791E093E1D49B}, // -246
        {0x8BD6A141006042BD, 0xE0C8BB2C5C6D24E1}, // -245
        {0xAECC49914078536D, 0x58FAE9F773886E19}, // -244
        {0xDA7F5BF590966848, 0xAF39A475506A899F}, // -243
        {0x888F99797A5E012D, 0x6D8406C952429604}, // -242
        {0xAAB37FD7D8F58178, 0xC8E5087BA6D33B84}, // -241
        {0xD5605FCDCF32E1D6, 0xFB1E4A9A90880A65}, // -240
        {0x855C3BE0A17FCD26, 0x5CF2EEA09A550680}, // -239
        {0xA6B34AD8C9DFC06F, 0xF42FAA48C0EA481F}, // -238
        {0xD0601D8EFC57B08B, 0xF13B94DAF124DA27}, // -237
        {0x823C12795DB6CE57, 0x76C53D08D6B70859}, // -236
        {0xA2CB1717B52481ED, 0x54768C4B0C64CA6F}, // -235
        {0xCB7DDCDDA26DA268, 0xA9942F5DCF7DFD0A}, // -234
        {0xFE5D54150B090B02, 0xD3F93B35435D7C4D}, // -233
        {0x9EFA548D26E5A6E1, 0xC47BC5014A1A6DB0}, // -232
        {0xC6B8E9B0709F109A, 0x359AB6419CA1091C}, // -231
        {0xF867241C8CC6D4C0, 0xC30163D203C94B63}, // -230
        {0x9B407691D7FC44F8, 0x79E0DE63425DCF1E}, // -229
        {0xC21094364DFB5636, 0x985915FC12F542E5}, // -228
        {0xF294B943E17A2BC4, 0x3E6F5B7B17B2939E}, // -227
        {0x979CF3CA6CEC5B5A, 0xA705992CEECF9C43}, // -226
        {0xBD8430BD08277231, 0x50C6FF782A838354}, // -225
        {0xECE53CEC4A314EBD, 0xA4F8BF5635246429}, // -224
        {0x940F4613AE5ED136, 0x871B7795E136BE9A}, // -223
        {0xB913179899F68584, 0x28E2557B59846E40}, // -222
        {0xE757DD7EC07426E5, 0x331AEADA2FE589D0}, // -221
        {0x9096EA6F3848984F, 0x3FF0D2C85DEF7622}, // -220
        {0xB4BCA50B065ABE63, 0x0FED077A756B53AA}, // -219
        {0xE1EBCE4DC7F16DFB, 0xD3E8495912C62895}, // -218
        {0x8D3360F09CF6E4BD, 0x64712DD7ABBBD95D}, // -217
        {0xB080392CC4349DEC, 0xBD8D794D96AACFB4}, // -216
        {0xDCA04777F541C567, 0xECF0D7A0FC5583A1}, // -215
        {0x89E42CAAF9491B60, 0xF41686C49DB57245}, // -214
        {0xAC5D37D5B79B6239, 0x311C2875C522CED6}, // -213
        {0xD77485CB25823AC7, 0x7D633293366B828C}, // -212
        {0x86A8D39EF77164BC, 0xAE5DFF9C02033198}, // -211
        {0xA8530886B54DBDEB, 0xD9F57F830283FDFD}, // -210
        {0xD267CAA862A12D66, 0xD072DF63C324FD7C}, // -209
        {0x8380DEA93DA4BC60, 0x4247CB9E59F71E6E}, // -208
        {0xA46116538D0DEB78, 0x52D9BE85F074E609}, // -207
        {0xCD795BE870516656, 0x67902E276C921F8C}, // -206
        {0x806BD9714632DFF6, 0x00BA1CD8A3DB53B7}, // -205
        {0xA086CFCD97BF97F3, 0x80E8A40ECCD228A5}, // -204
        {0xC8A883C0FDAF7DF0, 0x6122CD128006B2CE}, // -203
        {0xFAD2A4B13D1B5D6C, 0x796B805720085F82}, // -202
        {0x9CC3A6EEC6311A63, 0xCBE3303674053BB1}, // -201
        {0xC3F490AA77BD60FC, 0xBEDBFC4411068A9D}, // -200
        {0xF4F1B4D515ACB93B, 0xEE92FB5515482D45}, // -199
        {0x991711052D8BF3C5, 0x751BDD152D4D1C4B}, // -198
        {0xBF5CD54678EEF0B6, 0xD262D45A78A0635E}, // -197
        {0xEF340A98172AACE4, 0x86FB897116C87C35}, // -196
        {0x9580869F0E7AAC0E, 0xD45D35E6AE3D4DA1}, // -195
        {0xBAE0A846D2195712, 0x8974836059CCA10A}, // -194
        {0xE998D258869FACD7, 0x2BD1A438703FC94C}, // -193
        {0x91FF83775423CC06, 0x7B6306A34627DDD0}, // -192
        {0xB67F6455292CBF08, 0x1A3BC84C17B1D543}, // -191
        {0xE41F3D6A7377EECA, 0x20CABA5F1D9E4A94}, // -190
        {0x8E938662882AF53E, 0x547EB47B7282EE9D}, // -189
        {0xB23867FB2A35B28D, 0xE99E619A4F23AA44}, // -188
        {0xDEC681F9F4C31F31, 0x6405FA00E2EC94D5}, // -187
        {0x8B3C113C38F9F37E, 0xDE83BC408DD3DD05}, // -186
        {0xAE0B158B4738705E, 0x9624AB50B148D446}, // -185
        {0xD98DDAEE19068C76, 0x3BADD624DD9B0958}, // -184
        {0x87F8A8D4CFA417C9, 0xE54CA5D70A80E5D7}, // -183
        {0xA9F6D30A038D1DBC, 0x5E9FCF4CCD211F4D}, // -182
        {0xD47487CC8470652B, 0x7647C32000696720}, // -181
        {0x84C8D4DFD2C63F3B, 0x29ECD9F40041E074}, // -180
        {0xA5FB0A17C777CF09, 0xF468107100525891}, // -179
        {0xCF79CC9DB955C2CC, 0x7182148D4066EEB5}, // -178
        {0x81AC1FE293D599BF, 0xC6F14CD848405531}, // -177
        {0xA21727DB38CB002F, 0xB8ADA00E5A506A7D}, // -176
        {0xCA9CF1D206FDC03B, 0xA6D90811F0E4851D}, // -175
        {0xFD442E4688BD304A, 0x908F4A166D1DA664}, // -174
        {0x9E4A9CEC15763E2E, 0x9A598E4E043287FF}, // -173
        {0xC5DD44271AD3CDBA, 0x40EFF1E1853F29FE}, // -172
        {0xF7549530E188C128, 0xD12BEE59E68EF47D}, // -171
        {0x9A94DD3E8CF578B9, 0x82BB74F8301958CF}, // -170
        {0xC13A148E3032D6E7, 0xE36A52363C1FAF02}, // -169
        {0xF18899B1BC3F8CA1, 0xDC44E6C3CB279AC2}, // -168
        {0x96F5600F15A7B7E5, 0x29AB103A5EF8C0BA}, // -167
        {0xBCB2B812DB11A5DE, 0x7415D448F6B6F0E8}, // -166
        {0xEBDF661791D60F56, 0x111B495B3464AD22}, // -165
        {0x936B9FCEBB25C995, 0xCAB10DD900BEEC35}, // -164
        {0xB84687C269EF3BFB, 0x3D5D514F40EEA743}, // -163
        {0xE65829B3046B0AFA, 0x0CB4A5A3112A5113}, // -162
        {0x8FF71A0FE2C2E6DC, 0x47F0E785EABA72AC}, // -161
        {0xB3F4E093DB73A093, 0x59ED216765690F57}, // -160
        {0xE0F218B8D25088B8, 0x306869C13EC3532D}, // -159
        {0x8C974F7383725573, 0x1E414218C73A13FC}, // -158
        {0xAFBD2350644EEACF, 0xE5D1929EF90898FB}, // -157
        {0xDBAC6C247D62A583, 0xDF45F746B74ABF3A}, // -156
        {0x894BC396CE5DA772, 0x6B8BBA8C328EB784}, // -155
        {0xAB9EB47C81F5114F, 0x066EA92F3F326565}, // -154
        {0xD686619BA27255A2, 0xC80A537B0EFEFEBE}, // -153
        {0x8613FD0145877585, 0xBD06742CE95F5F37}, // -152
        {0xA798FC4196E952E7, 0x2C48113823B73705}, // -151
        {0xD17F3B51FCA3A7A0, 0xF75A15862CA504C6}, // -150
        {0x82EF85133DE648C4, 0x9A984D73DBE722FC}, // -149
        {0xA3AB66580D5FDAF5, 0xC13E60D0D2E0EBBB}, // -148
        {0xCC963FEE10B7D1B3, 0x318DF905079926A9}, // -147
        {0xFFBBCFE994E5C61F, 0xFDF17746497F7053}, // -146
        {0x9FD561F1FD0F9BD3, 0xFEB6EA8BEDEFA634}, // -145
        {0xC7CABA6E7C5382C8, 0xFE64A52EE96B8FC1}, // -144
        {0xF9BD690A1B68637B, 0x3DFDCE7AA3C673B1}, // -143
        {0x9C1661A651213E2D, 0x06BEA10CA65C084F}, // -142
        {0xC31BFA0FE5698DB8, 0x486E494FCFF30A63}, // -141
        {0xF3E2F893DEC3F126, 0x5A89DBA3C3EFCCFB}, // -140
        {0x986DDB5C6B3A76B7, 0xF89629465A75E01D}, // -139
        {0xBE89523386091465, 0xF6BBB397F1135824}, // -138
        {0xEE2BA6C0678B597F, 0x746AA07DED582E2D}, // -137
        {0x94DB483840B717EF, 0xA8C2A44EB4571CDD}, // -136
        {0xBA121A4650E4DDEB, 0x92F34D62616CE414}, // -135
        {0xE896A0D7E51E1566, 0x77B020BAF9C81D18}, // -134
        {0x915E2486EF32CD60, 0x0ACE1474DC1D122F}, // -133
        {0xB5B5ADA8AAFF80B8, 0x0D819992132456BB}, // -132
        {0xE3231912D5BF60E6, 0x10E1FFF697ED6C6A}, // -131
        {0x8DF5EFABC5979C8F, 0xCA8D3FFA1EF463C2}, // -130
        {0xB1736B96B6FD83B3, 0xBD308FF8A6B17CB3}, // -129
        {0xDDD0467C64BCE4A0, 0xAC7CB3F6D05DDBDF}, // -128
        {0x8AA22C0DBEF60EE4, 0x6BCDF07A423AA96C}, // -127
        {0xAD4AB7112EB3929D, 0x86C16C98D2C953C7}, // -126
        {0xD89D64D57A607744, 0xE871C7BF077BA8B8}, // -125
        {0x87625F056C7C4A8B, 0x11471CD764AD4973}, // -124
        {0xA93AF6C6C79B5D2D, 0xD598E40D3DD89BD0}, // -123
        {0xD389B47879823479, 0x4AFF1D108D4EC2C4}, // -122
        {0x843610CB4BF160CB, 0xCEDF722A585139BB}, // -121
        {0xA54394FE1EEDB8FE, 0xC2974EB4EE658829}, // -120
        {0xCE947A3DA6A9273E, 0x733D226229FEEA33}, // -119
        {0x811CCC668829B887, 0x0806357D5A3F5260}, // -118
        {0xA163FF802A3426A8, 0xCA07C2DCB0CF26F8}, // -117
        {0xC9BCFF6034C13052, 0xFC89B393DD02F0B6}, // -116
        {0xFC2C3F3841F17C67, 0xBBAC2078D443ACE3}, // -115
        {0x9D9BA7832936EDC0, 0xD54B944B84AA4C0E}, // -114
        {0xC5029163F384A931, 0x0A9E795E65D4DF12}, // -113
        {0xF64335BCF065D37D, 0x4D4617B5FF4A16D6}, // -112
        {0x99EA0196163FA42E, 0x504BCED1BF8E4E46}, // -111
        {0xC06481FB9BCF8D39, 0xE45EC2862F71E1D7}, // -110
        {0xF07DA27A82C37088, 0x5D767327BB4E5A4D}, // -109
        {0x964E858C91BA2655, 0x3A6A07F8D510F870}, // -108
        {0xBBE226EFB628AFEA, 0x890489F70A55368C}, // -107
        {0xEADAB0ABA3B2DBE5, 0x2B45AC74CCEA842F}, // -106
        {0x92C8AE6B464FC96F, 0x3B0B8BC90012929E}, // -105
        {0xB77ADA0617E3BBCB, 0x09CE6EBB40173745}, // -104
        {0xE55990879DDCAABD, 0xCC420A6A101D0516}, // -103
        {0x8F57FA54C2A9EAB6, 0x9FA946824A12232E}, // -102
        {0xB32DF8E9F3546564, 0x47939822DC96ABFA}, // -101
        {0xDFF9772470297EBD, 0x59787E2B93BC56F8}, // -100
        {0x8BFBEA76C619EF36, 0x57EB4EDB3C55B65B}, //  -99
        {0xAEFAE51477A06B03, 0xEDE622920B6B23F2}, //  -98
        {0xDAB99E59958885C4, 0xE95FAB368E45ECEE}, //  -97
        {0x88B402F7FD75539B, 0x11DBCB0218EBB415}, //  -96
        {0xAAE103B5FCD2A881, 0xD652BDC29F26A11A}, //  -95
        {0xD59944A37C0752A2, 0x4BE76D3346F04960}, //  -94
        {0x857FCAE62D8493A5, 0x6F70A4400C562DDC}, //  -93
        {0xA6DFBD9FB8E5B88E, 0xCB4CCD500F6BB953}, //  -92
        {0xD097AD07A71F26B2, 0x7E2000A41346A7A8}, //  -91
        {0x825ECC24C873782F, 0x8ED400668C0C28C9}, //  -90
        {0xA2F67F2DFA90563B, 0x728900802F0F32FB}, //  -89
        {0xCBB41EF979346BCA, 0x4F2B40A03AD2FFBA}, //  -88
        {0xFEA126B7D78186BC, 0xE2F610C84987BFA9}, //  -87
        {0x9F24B832E6B0F436, 0x0DD9CA7D2DF4D7CA}, //  -86
        {0xC6EDE63FA05D3143, 0x91503D1C79720DBC}, //  -85
        {0xF8A95FCF88747D94, 0x75A44C6397CE912B}, //  -84
        {0x9B69DBE1B548CE7C, 0xC986AFBE3EE11ABB}, //  -83
        {0xC24452DA229B021B, 0xFBE85BADCE996169}, //  -82
        {0xF2D56790AB41C2A2, 0xFAE27299423FB9C4}, //  -81
        {0x97C560BA6B0919A5, 0xDCCD879FC967D41B}, //  -80
        {0xBDB6B8E905CB600F, 0x5400E987BBC1C921}, //  -79
        {0xED246723473E3813, 0x290123E9AAB23B69}, //  -78
        {0x9436C0760C86E30B, 0xF9A0B6720AAF6522}, //  -77
        {0xB94470938FA89BCE, 0xF808E40E8D5B3E6A}, //  -76
        {0xE7958CB87392C2C2, 0xB60B1D1230B20E05}, //  -75
        {0x90BD77F3483BB9B9, 0xB1C6F22B5E6F48C3}, //  -74
        {0xB4ECD5F01A4AA828, 0x1E38AEB6360B1AF4}, //  -73
        {0xE2280B6C20DD5232, 0x25C6DA63C38DE1B1}, //  -72
        {0x8D590723948A535F, 0x579C487E5A38AD0F}, //  -71
        {0xB0AF48EC79ACE837, 0x2D835A9DF0C6D852}, //  -70
        {0xDCDB1B2798182244, 0xF8E431456CF88E66}, //  -69
        {0x8A08F0F8BF0F156B, 0x1B8E9ECB641B5900}, //  -68
        {0xAC8B2D36EED2DAC5, 0xE272467E3D222F40}, //  -67
        {0xD7ADF884AA879177, 0x5B0ED81DCC6ABB10}, //  -66
        {0x86CCBB52EA94BAEA, 0x98E947129FC2B4EA}, //  -65
        {0xA87FEA27A539E9A5, 0x3F2398D747B36225}, //  -64
        {0xD29FE4B18E88640E, 0x8EEC7F0D19A03AAE}, //  -63
        {0x83A3EEEEF9153E89, 0x1953CF68300424AD}, //  -62
        {0xA48CEAAAB75A8E2B, 0x5FA8C3423C052DD8}, //  -61
        {0xCDB02555653131B6, 0x3792F412CB06794E}, //  -60
        {0x808E17555F3EBF11, 0xE2BBD88BBEE40BD1}, //  -59
        {0xA0B19D2AB70E6ED6, 0x5B6ACEAEAE9D0EC5}, //  -58
        {0xC8DE047564D20A8B, 0xF245825A5A445276}, //  -57
        {0xFB158592BE068D2E, 0xEED6E2F0F0D56713}, //  -56
        {0x9CED737BB6C4183D, 0x55464DD69685606C}, //  -55
        {0xC428D05AA4751E4C, 0xAA97E14C3C26B887}, //  -54
        {0xF53304714D9265DF, 0xD53DD99F4B3066A9}, //  -53
        {0x993FE2C6D07B7FAB, 0xE546A8038EFE402A}, //  -52
        {0xBF8FDB78849A5F96, 0xDE98520472BDD034}, //  -51
        {0xEF73D256A5C0F77C, 0x963E66858F6D4441}, //  -50
        {0x95A8637627989AAD, 0xDDE7001379A44AA9}, //  -49
        {0xBB127C53B17EC159, 0x5560C018580D5D53}, //  -48
        {0xE9D71B689DDE71AF, 0xAAB8F01E6E10B4A7}, //  -47
        {0x9226712162AB070D, 0xCAB3961304CA70E9}, //  -46
        {0xB6B00D69BB55C8D1, 0x3D607B97C5FD0D23}, //  -45
        {0xE45C10C42A2B3B05, 0x8CB89A7DB77C506B}, //  -44
        {0x8EB98A7A9A5B04E3, 0x77F3608E92ADB243}, //  -43
        {0xB267ED1940F1C61C, 0x55F038B237591ED4}, //  -42
        {0xDF01E85F912E37A3, 0x6B6C46DEC52F6689}, //  -41
        {0x8B61313BBABCE2C6, 0x2323AC4B3B3DA016}, //  -40
        {0xAE397D8AA96C1B77, 0xABEC975E0A0D081B}, //  -39
        {0xD9C7DCED53C72255, 0x96E7BD358C904A22}, //  -38
        {0x881CEA14545C7575, 0x7E50D64177DA2E55}, //  -37
        {0xAA242499697392D2, 0xDDE50BD1D5D0B9EA}, //  -36
        {0xD4AD2DBFC3D07787, 0x955E4EC64B44E865}, //  -35
        {0x84EC3C97DA624AB4, 0xBD5AF13BEF0B113F}, //  -34
        {0xA6274BBDD0FADD61, 0xECB1AD8AEACDD58F}, //  -33
        {0xCFB11EAD453994BA, 0x67DE18EDA5814AF3}, //  -32
        {0x81CEB32C4B43FCF4, 0x80EACF948770CED8}, //  -31
        {0xA2425FF75E14FC31, 0xA1258379A94D028E}, //  -30
        {0xCAD2F7F5359A3B3E, 0x096EE45813A04331}, //  -29
        {0xFD87B5F28300CA0D, 0x8BCA9D6E188853FD}, //  -28
        {0x9E74D1B791E07E48, 0x775EA264CF55347E}, //  -27
        {0xC612062576589DDA, 0x95364AFE032A819E}, //  -26
        {0xF79687AED3EEC551, 0x3A83DDBD83F52205}, //  -25
        {0x9ABE14CD44753B52, 0xC4926A9672793543}, //  -24
        {0xC16D9A0095928A27, 0x75B7053C0F178294}, //  -23
        {0xF1C90080BAF72CB1, 0x5324C68B12DD6339}, //  -22
        {0x971DA05074DA7BEE, 0xD3F6FC16EBCA5E04}, //  -21
        {0xBCE5086492111AEA, 0x88F4BB1CA6BCF585}, //  -20
        {0xEC1E4A7DB69561A5, 0x2B31E9E3D06C32E6}, //  -19
        {0x9392EE8E921D5D07, 0x3AFF322E62439FD0}, //  -18
        {0xB877AA3236A4B449, 0x09BEFEB9FAD487C3}, //  -17
        {0xE69594BEC44DE15B, 0x4C2EBE687989A9B4}, //  -16
        {0x901D7CF73AB0ACD9, 0x0F9D37014BF60A11}, //  -15
        {0xB424DC35095CD80F, 0x538484C19EF38C95}, //  -14
        {0xE12E13424BB40E13, 0x2865A5F206B06FBA}, //  -13
        {0x8CBCCC096F5088CB, 0xF93F87B7442E45D4}, //  -12
        {0xAFEBFF0BCB24AAFE, 0xF78F69A51539D749}, //  -11
        {0xDBE6FECEBDEDD5BE, 0xB573440E5A884D1C}, //  -10
        {0x89705F4136B4A597, 0x31680A88F8953031}, //   -9
        {0xABCC77118461CEFC, 0xFDC20D2B36BA7C3E}, //   -8
        {0xD6BF94D5E57A42BC, 0x3D32907604691B4D}, //   -7
        {0x8637BD05AF6C69B5, 0xA63F9A49C2C1B110}, //   -6
        {0xA7C5AC471B478423, 0x0FCF80DC33721D54}, //   -5
        {0xD1B71758E219652B, 0xD3C36113404EA4A9}, //   -4
        {0x83126E978D4FDF3B, 0x645A1CAC083126EA}, //   -3
        {0xA3D70A3D70A3D70A, 0x3D70A3D70A3D70A4}, //   -2
        {0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCCCCCCD}, //   -1
        {0x8000000000000000, 0x0000000000000000}, //    0
        {0xA000000000000000, 0x0000000000000000}, //    1
        {0xC800000000000000, 0x0000000000000000}, //    2
        {0xFA00000000000000, 0x0000000000000000}, //    3
        {0x9C40000000000000, 0x0000000000000000}, //    4
        {0xC350000000000000, 0x0000000000000000}, //    5
        {0xF424000000000000, 0x0000000000000000}, //    6
        {0x9896800000000000, 0x0000000000000000}, //    7
        {0xBEBC200000000000, 0x0000000000000000}, //    8
        {0xEE6B280000000000, 0x0000000000000000}, //    9
        {0x9502F90000000000, 0x0000000000000000}, //   10
        {0xBA43B74000000000, 0x0000000000000000}, //   11
        {0xE8D4A51000000000, 0x0000000000000000}, //   12
        {0x9184E72A00000000, 0x0000000000000000}, //   13
        {0xB5E620F480000000, 0x0000000000000000}, //   14
        {0xE35FA931A0000000, 0x0000000000000000}, //   15
        {0x8E1BC9BF04000000, 0x0000000000000000}, //   16
        {0xB1A2BC2EC5000000, 0x0000000000000000}, //   17
        {0xDE0B6B3A76400000, 0x0000000000000000}, //   18
        {0x8AC7230489E80000, 0x0000000000000000}, //   19
        {0xAD78EBC5AC620000, 0x0000000000000000}, //   20
        {0xD8D726B7177A8000, 0x0000000000000000}, //   21
        {0x878678326EAC9000, 0x0000000000000000}, //   22
        {0xA968163F0A57B400, 0x0000000000000000}, //   23
        {0xD3C21BCECCEDA100, 0x0000000000000000}, //   24
        {0x84595161401484A0, 0x0000000000000000}, //   25
        {0xA56FA5B99019A5C8, 0x0000000000000000}, //   26
        {0xCECB8F27F4200F3A, 0x0000000000000000}, //   27
        {0x813F3978F8940984, 0x4000000000000000}, //   28
        {0xA18F07D736B90BE5, 0x5000000000000000}, //   29
        {0xC9F2C9CD04674EDE, 0xA400000000000000}, //   30
        {0xFC6F7C4045812296, 0x4D00000000000000}, //   31
        {0x9DC5ADA82B70B59D, 0xF020000000000000}, //   32
        {0xC5371912364CE305, 0x6C28000000000000}, //   33
        {0xF684DF56C3E01BC6, 0xC732000000000000}, //   34
        {0x9A130B963A6C115C, 0x3C7F400000000000}, //   35
        {0xC097CE7BC90715B3, 0x4B9F100000000000}, //   36
        {0xF0BDC21ABB48DB20, 0x1E86D40000000000}, //   37
        {0x96769950B50D88F4, 0x1314448000000000}, //   38
        {0xBC143FA4E250EB31, 0x17D955A000000000}, //   39
        {0xEB194F8E1AE525FD, 0x5DCFAB0800000000}, //   40
        {0x92EFD1B8D0CF37BE, 0x5AA1CAE500000000}, //   41
        {0xB7ABC627050305AD, 0xF14A3D9E40000000}, //   42
        {0xE596B7B0C643C719, 0x6D9CCD05D0000000}, //   43
        {0x8F7E32CE7BEA5C6F, 0xE4820023A2000000}, //   44
        {0xB35DBF821AE4F38B, 0xDDA2802C8A800000}, //   45
        {0xE0352F62A19E306E, 0xD50B2037AD200000}, //   46
        {0x8C213D9DA502DE45, 0x4526F422CC340000}, //   47
        {0xAF298D050E4395D6, 0x9670B12B7F410000}, //   48
        {0xDAF3F04651D47B4C, 0x3C0CDD765F114000}, //   49
        {0x88D8762BF324CD0F, 0xA5880A69FB6AC800}, //   50
        {0xAB0E93B6EFEE0053, 0x8EEA0D047A457A00}, //   51
        {0xD5D238A4ABE98068, 0x72A4904598D6D880}, //   52
        {0x85A36366EB71F041, 0x47A6DA2B7F864750}, //   53
        {0xA70C3C40A64E6C51, 0x999090B65F67D924}, //   54
        {0xD0CF4B50CFE20765, 0xFFF4B4E3F741CF6D}, //   55
        {0x82818F1281ED449F, 0xBFF8F10E7A8921A5}, //   56
        {0xA321F2D7226895C7, 0xAFF72D52192B6A0E}, //   57
        {0xCBEA6F8CEB02BB39, 0x9BF4F8A69F764491}, //   58
        {0xFEE50B7025C36A08, 0x02F236D04753D5B5}, //   59
        {0x9F4F2726179A2245, 0x01D762422C946591}, //   60
        {0xC722F0EF9D80AAD6, 0x424D3AD2B7B97EF6}, //   61
        {0xF8EBAD2B84E0D58B, 0xD2E0898765A7DEB3}, //   62
        {0x9B934C3B330C8577, 0x63CC55F49F88EB30}, //   63
        {0xC2781F49FFCFA6D5, 0x3CBF6B71C76B25FC}, //   64
        {0xF316271C7FC3908A, 0x8BEF464E3945EF7B}, //   65
        {0x97EDD871CFDA3A56, 0x97758BF0E3CBB5AD}, //   66
        {0xBDE94E8E43D0C8EC, 0x3D52EEED1CBEA318}, //   67
        {0xED63A231D4C4FB27, 0x4CA7AAA863EE4BDE}, //   68
        {0x945E455F24FB1CF8, 0x8FE8CAA93E74EF6B}, //   69
        {0xB975D6B6EE39E436, 0xB3E2FD538E122B45}, //   70
        {0xE7D34C64A9C85D44, 0x60DBBCA87196B617}, //   71
        {0x90E40FBEEA1D3A4A, 0xBC8955E946FE31CE}, //   72
        {0xB51D13AEA4A488DD, 0x6BABAB6398BDBE42}, //   73
        {0xE264589A4DCDAB14, 0xC696963C7EED2DD2}, //   74
        {0x8D7EB76070A08AEC, 0xFC1E1DE5CF543CA3}, //   75
        {0xB0DE65388CC8ADA8, 0x3B25A55F43294BCC}, //   76
        {0xDD15FE86AFFAD912, 0x49EF0EB713F39EBF}, //   77
        {0x8A2DBF142DFCC7AB, 0x6E3569326C784338}, //   78
        {0xACB92ED9397BF996, 0x49C2C37F07965405}, //   79
        {0xD7E77A8F87DAF7FB, 0xDC33745EC97BE907}, //   80
        {0x86F0AC99B4E8DAFD, 0x69A028BB3DED71A4}, //   81
        {0xA8ACD7C0222311BC, 0xC40832EA0D68CE0D}, //   82
        {0xD2D80DB02AABD62B, 0xF50A3FA490C30191}, //   83
        {0x83C7088E1AAB65DB, 0x792667C6DA79E0FB}, //   84
        {0xA4B8CAB1A1563F52, 0x577001B891185939}, //   85
        {0xCDE6FD5E09ABCF26, 0xED4C0226B55E6F87}, //   86
        {0x80B05E5AC60B6178, 0x544F8158315B05B5}, //   87
        {0xA0DC75F1778E39D6, 0x696361AE3DB1C722}, //   88
        {0xC913936DD571C84C, 0x03BC3A19CD1E38EA}, //   89
        {0xFB5878494ACE3A5F, 0x04AB48A04065C724}, //   90
        {0x9D174B2DCEC0E47B, 0x62EB0D64283F9C77}, //   91
        {0xC45D1DF942711D9A, 0x3BA5D0BD324F8395}, //   92
        {0xF5746577930D6500, 0xCA8F44EC7EE3647A}, //   93
        {0x9968BF6ABBE85F20, 0x7E998B13CF4E1ECC}, //   94
        {0xBFC2EF456AE276E8, 0x9E3FEDD8C321A67F}, //   95
        {0xEFB3AB16C59B14A2, 0xC5CFE94EF3EA101F}, //   96
        {0x95D04AEE3B80ECE5, 0xBBA1F1D158724A13}, //   97
        {0xBB445DA9CA61281F, 0x2A8A6E45AE8EDC98}, //   98
        {0xEA1575143CF97226, 0xF52D09D71A3293BE}, //   99
        {0x924D692CA61BE758, 0x593C2626705F9C57}, //  100
        {0xB6E0C377CFA2E12E, 0x6F8B2FB00C77836D}, //  101
        {0xE498F455C38B997A, 0x0B6DFB9C0F956448}, //  102
        {0x8EDF98B59A373FEC, 0x4724BD4189BD5EAD}, //  103
        {0xB2977EE300C50FE7, 0x58EDEC91EC2CB658}, //  104
        {0xDF3D5E9BC0F653E1, 0x2F2967B66737E3EE}, //  105
        {0x8B865B215899F46C, 0xBD79E0D20082EE75}, //  106
        {0xAE67F1E9AEC07187, 0xECD8590680A3AA12}, //  107
        {0xDA01EE641A708DE9, 0xE80E6F4820CC9496}, //  108
        {0x884134FE908658B2, 0x3109058D147FDCDE}, //  109
        {0xAA51823E34A7EEDE, 0xBD4B46F0599FD416}, //  110
        {0xD4E5E2CDC1D1EA96, 0x6C9E18AC7007C91B}, //  111
        {0x850FADC09923329E, 0x03E2CF6BC604DDB1}, //  112
        {0xA6539930BF6BFF45, 0x84DB8346B786151D}, //  113
        {0xCFE87F7CEF46FF16, 0xE612641865679A64}, //  114
        {0x81F14FAE158C5F6E, 0x4FCB7E8F3F60C07F}, //  115
        {0xA26DA3999AEF7749, 0xE3BE5E330F38F09E}, //  116
        {0xCB090C8001AB551C, 0x5CADF5BFD3072CC6}, //  117
        {0xFDCB4FA002162A63, 0x73D9732FC7C8F7F7}, //  118
        {0x9E9F11C4014DDA7E, 0x2867E7FDDCDD9AFB}, //  119
        {0xC646D63501A1511D, 0xB281E1FD541501B9}, //  120
        {0xF7D88BC24209A565, 0x1F225A7CA91A4227}, //  121
        {0x9AE757596946075F, 0x3375788DE9B06959}, //  122
        {0xC1A12D2FC3978937, 0x0052D6B1641C83AF}, //  123
        {0xF209787BB47D6B84, 0xC0678C5DBD23A49B}, //  124
        {0x9745EB4D50CE6332, 0xF840B7BA963646E1}, //  125
        {0xBD176620A501FBFF, 0xB650E5A93BC3D899}, //  126
        {0xEC5D3FA8CE427AFF, 0xA3E51F138AB4CEBF}, //  127
        {0x93BA47C980E98CDF, 0xC66F336C36B10138}, //  128
        {0xB8A8D9BBE123F017, 0xB80B0047445D4185}, //  129
        {0xE6D3102AD96CEC1D, 0xA60DC059157491E6}, //  130
        {0x9043EA1AC7E41392, 0x87C89837AD68DB30}, //  131
        {0xB454E4A179DD1877, 0x29BABE4598C311FC}, //  132
        {0xE16A1DC9D8545E94, 0xF4296DD6FEF3D67B}, //  133
        {0x8CE2529E2734BB1D, 0x1899E4A65F58660D}, //  134
        {0xB01AE745B101E9E4, 0x5EC05DCFF72E7F90}, //  135
        {0xDC21A1171D42645D, 0x76707543F4FA1F74}, //  136
        {0x899504AE72497EBA, 0x6A06494A791C53A9}, //  137
        {0xABFA45DA0EDBDE69, 0x0487DB9D17636893}, //  138
        {0xD6F8D7509292D603, 0x45A9D2845D3C42B7}, //  139
        {0x865B86925B9BC5C2, 0x0B8A2392BA45A9B3}, //  140
        {0xA7F26836F282B732, 0x8E6CAC7768D7141F}, //  141
        {0xD1EF0244AF2364FF, 0x3207D795430CD927}, //  142
        {0x8335616AED761F1F, 0x7F44E6BD49E807B9}, //  143
        {0xA402B9C5A8D3A6E7, 0x5F16206C9C6209A7}, //  144
        {0xCD036837130890A1, 0x36DBA887C37A8C10}, //  145
        {0x802221226BE55A64, 0xC2494954DA2C978A}, //  146
        {0xA02AA96B06DEB0FD, 0xF2DB9BAA10B7BD6D}, //  147
        {0xC83553C5C8965D3D, 0x6F92829494E5ACC8}, //  148
        {0xFA42A8B73ABBF48C, 0xCB772339BA1F17FA}, //  149
        {0x9C69A97284B578D7, 0xFF2A760414536EFC}, //  150
        {0xC38413CF25E2D70D, 0xFEF5138519684ABB}, //  151
        {0xF46518C2EF5B8CD1, 0x7EB258665FC25D6A}, //  152
        {0x98BF2F79D5993802, 0xEF2F773FFBD97A62}, //  153
        {0xBEEEFB584AFF8603, 0xAAFB550FFACFD8FB}, //  154
        {0xEEAABA2E5DBF6784, 0x95BA2A53F983CF39}, //  155
        {0x952AB45CFA97A0B2, 0xDD945A747BF26184}, //  156
        {0xBA756174393D88DF, 0x94F971119AEEF9E5}, //  157
        {0xE912B9D1478CEB17, 0x7A37CD5601AAB85E}, //  158
        {0x91ABB422CCB812EE, 0xAC62E055C10AB33B}, //  159
        {0xB616A12B7FE617AA, 0x577B986B314D600A}, //  160
        {0xE39C49765FDF9D94, 0xED5A7E85FDA0B80C}, //  161
        {0x8E41ADE9FBEBC27D, 0x14588F13BE847308}, //  162
        {0xB1D219647AE6B31C, 0x596EB2D8AE258FC9}, //  163
        {0xDE469FBD99A05FE3, 0x6FCA5F8ED9AEF3BC}, //  164
        {0x8AEC23D680043BEE, 0x25DE7BB9480D5855}, //  165
        {0xADA72CCC20054AE9, 0xAF561AA79A10AE6B}, //  166
        {0xD910F7FF28069DA4, 0x1B2BA1518094DA05}, //  167
        {0x87AA9AFF79042286, 0x90FB44D2F05D0843}, //  168
        {0xA99541BF57452B28, 0x353A1607AC744A54}, //  169
        {0xD3FA922F2D1675F2, 0x42889B8997915CE9}, //  170
        {0x847C9B5D7C2E09B7, 0x69956135FEBADA12}, //  171
        {0xA59BC234DB398C25, 0x43FAB9837E699096}, //  172
        {0xCF02B2C21207EF2E, 0x94F967E45E03F4BC}, //  173
        {0x8161AFB94B44F57D, 0x1D1BE0EEBAC278F6}, //  174
        {0xA1BA1BA79E1632DC, 0x6462D92A69731733}, //  175
        {0xCA28A291859BBF93, 0x7D7B8F7503CFDCFF}, //  176
        {0xFCB2CB35E702AF78, 0x5CDA735244C3D43F}, //  177
        {0x9DEFBF01B061ADAB, 0x3A0888136AFA64A8}, //  178
        {0xC56BAEC21C7A1916, 0x088AAA1845B8FDD1}, //  179
        {0xF6C69A72A3989F5B, 0x8AAD549E57273D46}, //  180
        {0x9A3C2087A63F6399, 0x36AC54E2F678864C}, //  181
        {0xC0CB28A98FCF3C7F, 0x84576A1BB416A7DE}, //  182
        {0xF0FDF2D3F3C30B9F, 0x656D44A2A11C51D6}, //  183
        {0x969EB7C47859E743, 0x9F644AE5A4B1B326}, //  184
        {0xBC4665B596706114, 0x873D5D9F0DDE1FEF}, //  185
        {0xEB57FF22FC0C7959, 0xA90CB506D155A7EB}, //  186
        {0x9316FF75DD87CBD8, 0x09A7F12442D588F3}, //  187
        {0xB7DCBF5354E9BECE, 0x0C11ED6D538AEB30}, //  188
        {0xE5D3EF282A242E81, 0x8F1668C8A86DA5FB}, //  189
        {0x8FA475791A569D10, 0xF96E017D694487BD}, //  190
        {0xB38D92D760EC4455, 0x37C981DCC395A9AD}, //  191
        {0xE070F78D3927556A, 0x85BBE253F47B1418}, //  192
        {0x8C469AB843B89562, 0x93956D7478CCEC8F}, //  193
        {0xAF58416654A6BABB, 0x387AC8D1970027B3}, //  194
        {0xDB2E51BFE9D0696A, 0x06997B05FCC0319F}, //  195
        {0x88FCF317F22241E2, 0x441FECE3BDF81F04}, //  196
        {0xAB3C2FDDEEAAD25A, 0xD527E81CAD7626C4}, //  197
        {0xD60B3BD56A5586F1, 0x8A71E223D8D3B075}, //  198
        {0x85C7056562757456, 0xF6872D5667844E4A}, //  199
        {0xA738C6BEBB12D16C, 0xB428F8AC016561DC}, //  200
        {0xD106F86E69D785C7, 0xE13336D701BEBA53}, //  201
        {0x82A45B450226B39C, 0xECC0024661173474}, //  202
        {0xA34D721642B06084, 0x27F002D7F95D0191}, //  203
        {0xCC20CE9BD35C78A5, 0x31EC038DF7B441F5}, //  204
        {0xFF290242C83396CE, 0x7E67047175A15272}, //  205
        {0x9F79A169BD203E41, 0x0F0062C6E984D387}, //  206
        {0xC75809C42C684DD1, 0x52C07B78A3E60869}, //  207
        {0xF92E0C3537826145, 0xA7709A56CCDF8A83}, //  208
        {0x9BBCC7A142B17CCB, 0x88A66076400BB692}, //  209
        {0xC2ABF989935DDBFE, 0x6ACFF893D00EA436}, //  210
        {0xF356F7EBF83552FE, 0x0583F6B8C4124D44}, //  211
        {0x98165AF37B2153DE, 0xC3727A337A8B704B}, //  212
        {0xBE1BF1B059E9A8D6, 0x744F18C0592E4C5D}, //  213
        {0xEDA2EE1C7064130C, 0x1162DEF06F79DF74}, //  214
        {0x9485D4D1C63E8BE7, 0x8ADDCB5645AC2BA9}, //  215
        {0xB9A74A0637CE2EE1, 0x6D953E2BD7173693}, //  216
        {0xE8111C87C5C1BA99, 0xC8FA8DB6CCDD0438}, //  217
        {0x910AB1D4DB9914A0, 0x1D9C9892400A22A3}, //  218
        {0xB54D5E4A127F59C8, 0x2503BEB6D00CAB4C}, //  219
        {0xE2A0B5DC971F303A, 0x2E44AE64840FD61E}, //  220
        {0x8DA471A9DE737E24, 0x5CEAECFED289E5D3}, //  221
        {0xB10D8E1456105DAD, 0x7425A83E872C5F48}, //  222
        {0xDD50F1996B947518, 0xD12F124E28F7771A}, //  223
        {0x8A5296FFE33CC92F, 0x82BD6B70D99AAA70}, //  224
        {0xACE73CBFDC0BFB7B, 0x636CC64D1001550C}, //  225
        {0xD8210BEFD30EFA5A, 0x3C47F7E05401AA4F}, //  226
        {0x8714A775E3E95C78, 0x65ACFAEC34810A72}, //  227
        {0xA8D9D1535CE3B396, 0x7F1839A741A14D0E}, //  228
        {0xD31045A8341CA07C, 0x1EDE48111209A051}, //  229
        {0x83EA2B892091E44D, 0x934AED0AAB460433}, //  230
        {0xA4E4B66B68B65D60, 0xF81DA84D56178540}, //  231
        {0xCE1DE40642E3F4B9, 0x36251260AB9D668F}, //  232
        {0x80D2AE83E9CE78F3, 0xC1D72B7C6B42601A}, //  233
        {0xA1075A24E4421730, 0xB24CF65B8612F820}, //  234
        {0xC94930AE1D529CFC, 0xDEE033F26797B628}, //  235
        {0xFB9B7CD9A4A7443C, 0x169840EF017DA3B2}, //  236
        {0x9D412E0806E88AA5, 0x8E1F289560EE864F}, //  237
        {0xC491798A08A2AD4E, 0xF1A6F2BAB92A27E3}, //  238
        {0xF5B5D7EC8ACB58A2, 0xAE10AF696774B1DC}, //  239
        {0x9991A6F3D6BF1765, 0xACCA6DA1E0A8EF2A}, //  240
        {0xBFF610B0CC6EDD3F, 0x17FD090A58D32AF4}, //  241
        {0xEFF394DCFF8A948E, 0xDDFC4B4CEF07F5B1}, //  242
        {0x95F83D0A1FB69CD9, 0x4ABDAF101564F98F}, //  243
        {0xBB764C4CA7A4440F, 0x9D6D1AD41ABE37F2}, //  244
        {0xEA53DF5FD18D5513, 0x84C86189216DC5EE}, //  245
        {0x92746B9BE2F8552C, 0x32FD3CF5B4E49BB5}, //  246
        {0xB7118682DBB66A77, 0x3FBC8C33221DC2A2}, //  247
        {0xE4D5E82392A40515, 0x0FABAF3FEAA5334B}, //  248
        {0x8F05B1163BA6832D, 0x29CB4D87F2A7400F}, //  249
        {0xB2C71D5BCA9023F8, 0x743E20E9EF511013}, //  250
        {0xDF78E4B2BD342CF6, 0x914DA9246B255417}, //  251
        {0x8BAB8EEFB6409C1A, 0x1AD089B6C2F7548F}, //  252
        {0xAE9672ABA3D0C320, 0xA184AC2473B529B2}, //  253
        {0xDA3C0F568CC4F3E8, 0xC9E5D72D90A2741F}, //  254
        {0x8865899617FB1871, 0x7E2FA67C7A658893}, //  255
        {0xAA7EEBFB9DF9DE8D, 0xDDBB901B98FEEAB8}, //  256
        {0xD51EA6FA85785631, 0x552A74227F3EA566}, //  257
        {0x8533285C936B35DE, 0xD53A88958F872760}, //  258
        {0xA67FF273B8460356, 0x8A892ABAF368F138}, //  259
        {0xD01FEF10A657842C, 0x2D2B7569B0432D86}, //  260
        {0x8213F56A67F6B29B, 0x9C3B29620E29FC74}, //  261
        {0xA298F2C501F45F42, 0x8349F3BA91B47B90}, //  262
        {0xCB3F2F7642717713, 0x241C70A936219A74}, //  263
        {0xFE0EFB53D30DD4D7, 0xED238CD383AA0111}, //  264
        {0x9EC95D1463E8A506, 0xF4363804324A40AB}, //  265
        {0xC67BB4597CE2CE48, 0xB143C6053EDCD0D6}, //  266
        {0xF81AA16FDC1B81DA, 0xDD94B7868E94050B}, //  267
        {0x9B10A4E5E9913128, 0xCA7CF2B4191C8327}, //  268
        {0xC1D4CE1F63F57D72, 0xFD1C2F611F63A3F1}, //  269
        {0xF24A01A73CF2DCCF, 0xBC633B39673C8CED}, //  270
        {0x976E41088617CA01, 0xD5BE0503E085D814}, //  271
        {0xBD49D14AA79DBC82, 0x4B2D8644D8A74E19}, //  272
        {0xEC9C459D51852BA2, 0xDDF8E7D60ED1219F}, //  273
        {0x93E1AB8252F33B45, 0xCABB90E5C942B504}, //  274
        {0xB8DA1662E7B00A17, 0x3D6A751F3B936244}, //  275
        {0xE7109BFBA19C0C9D, 0x0CC512670A783AD5}, //  276
        {0x906A617D450187E2, 0x27FB2B80668B24C6}, //  277
        {0xB484F9DC9641E9DA, 0xB1F9F660802DEDF7}, //  278
        {0xE1A63853BBD26451, 0x5E7873F8A0396974}, //  279
        {0x8D07E33455637EB2, 0xDB0B487B6423E1E9}, //  280
        {0xB049DC016ABC5E5F, 0x91CE1A9A3D2CDA63}, //  281
        {0xDC5C5301C56B75F7, 0x7641A140CC7810FC}, //  282
        {0x89B9B3E11B6329BA, 0xA9E904C87FCB0A9E}, //  283
        {0xAC2820D9623BF429, 0x546345FA9FBDCD45}, //  284
        {0xD732290FBACAF133, 0xA97C177947AD4096}, //  285
        {0x867F59A9D4BED6C0, 0x49ED8EABCCCC485E}, //  286
        {0xA81F301449EE8C70, 0x5C68F256BFFF5A75}, //  287
        {0xD226FC195C6A2F8C, 0x73832EEC6FFF3112}, //  288
        {0x83585D8FD9C25DB7, 0xC831FD53C5FF7EAC}, //  289
        {0xA42E74F3D032F525, 0xBA3E7CA8B77F5E56}, //  290
        {0xCD3A1230C43FB26F, 0x28CE1BD2E55F35EC}, //  291
        {0x80444B5E7AA7CF85, 0x7980D163CF5B81B4}, //  292
        {0xA0555E361951C366, 0xD7E105BCC3326220}, //  293
        {0xC86AB5C39FA63440, 0x8DD9472BF3FEFAA8}, //  294
        {0xFA856334878FC150, 0xB14F98F6F0FEB952}, //  295
        {0x9C935E00D4B9D8D2, 0x6ED1BF9A569F33D4}, //  296
        {0xC3B8358109E84F07, 0x0A862F80EC4700C9}, //  297
        {0xF4A642E14C6262C8, 0xCD27BB612758C0FB}, //  298
        {0x98E7E9CCCFBD7DBD, 0x8038D51CB897789D}, //  299
        {0xBF21E44003ACDD2C, 0xE0470A63E6BD56C4}, //  300
        {0xEEEA5D5004981478, 0x1858CCFCE06CAC75}, //  301
        {0x95527A5202DF0CCB, 0x0F37801E0C43EBC9}, //  302
        {0xBAA718E68396CFFD, 0xD30560258F54E6BB}, //  303
        {0xE950DF20247C83FD, 0x47C6B82EF32A206A}, //  304
        {0x91D28B7416CDD27E, 0x4CDC331D57FA5442}, //  305
        {0xB6472E511C81471D, 0xE0133FE4ADF8E953}, //  306
        {0xE3D8F9E563A198E5, 0x58180FDDD97723A7}, //  307
        {0x8E679C2F5E44FF8F, 0x570F09EAA7EA7649}, //  308
        {0xB201833B35D63F73, 0x2CD2CC6551E513DB}, //  309
        {0xDE81E40A034BCF4F, 0xF8077F7EA65E58D2}, //  310
        {0x8B112E86420F6191, 0xFB04AFAF27FAF783}, //  311
        {0xADD57A27D29339F6, 0x79C5DB9AF1F9B564}, //  312
        {0xD94AD8B1C7380874, 0x18375281AE7822BD}, //  313
        {0x87CEC76F1C830548, 0x8F2293910D0B15B6}, //  314
        {0xA9C2794AE3A3C69A, 0xB2EB3875504DDB23}, //  315
        {0xD433179D9C8CB841, 0x5FA60692A46151EC}, //  316
        {0x849FEEC281D7F328, 0xDBC7C41BA6BCD334}, //  317
        {0xA5C7EA73224DEFF3, 0x12B9B522906C0801}, //  318
        {0xCF39E50FEAE16BEF, 0xD768226B34870A01}, //  319
        {0x81842F29F2CCE375, 0xE6A1158300D46641}, //  320
        {0xA1E53AF46F801C53, 0x60495AE3C1097FD1}, //  321
        {0xCA5E89B18B602368, 0x385BB19CB14BDFC5}, //  322
        {0xFCF62C1DEE382C42, 0x46729E03DD9ED7B6}, //  323
        {0x9E19DB92B4E31BA9, 0x6C07A2C26A8346D2}, //  324
    };

    u64 M52 = (1ull << 52) - 1;
    u64 M63 = (1ull << 63) - 1;
    u64 vi = *(u64 *)&v;
    i64 vi64 = *(i64 *)&v;
    u64 vi_abs = vi & M63;
    double v_abs = *(double *)&vi_abs; // abs(v)
    u64 v_to_u64 = v_abs;              // cvt to unsigned long long
    double u64_to_v = v_to_u64;        // cvt to double
    i64 v_to_i64 = v;                  // cvt to long long
    double i64_to_v = v_to_i64;        // cvt to double
    u64 sign = (vi >> 63);
    i64 exp = vi_abs >> 52;
    u64 frac = vi & M52;
#if 1 // print integer
    // if( (vi64 == *(i64*)&i64_to_v) & ( abs(v_to_i64) <= (i64)( ((1ll<<53) - 1) ) )  ) // branch miss may high in random data
    if ((vi_abs == *(u64 *)&u64_to_v) & (v_to_u64 <= (u64)(((1ull << 53) - 1))))
    {
        buffer[0] = '-';
        char *end_buf = write_u64_len_1_to_16(v_to_u64, (u8 *)(buffer + sign));
        return end_buf - buffer;
        // small_int = 1;
        // printf("small integer v_to_i64 = %lld \n", v_to_u64);
#if 0
        buffer[0] = '-';
        // int len = static_cast<int>(itoa_i64_yy((int64_t)v_to_i64,buffer) - buffer);
        int len = (itoa_u64_impl((uint64_t)v_to_u64, buffer + sign) - buffer);
        buffer[len] = '\0';
        return len;
#else
        // return i64toa_sse2(v_to_i64,buffer);
        // buffer[0] = '-';
        // return sign + u64toa_sse2(v_to_u64, buffer + sign);
#endif
    }
#endif
    buffer[0] = '-';
    buffer += sign;
    if (exp == 0x7ff) //[[unlikely]]
    {
        *(int *)buffer = frac ? *(int *)"nan" : *(int *)"inf";
        return sign + 3;
    }
    // if (vi_abs == 0)
    // {
    //     *(short *)buffer = *(short *)"0";
    //     return sign + 1;
    // }
    u64 normal = (exp > 0);
    u64 c;
    ll q;
    if (normal) // [[likely]]
    {
        c = frac | (1ull << 52);
        q = exp - 1075;
    }
    else
    {
        c = frac;
        q = -1074;
    }
    *(u64 *)buffer = *(u64 *)"0.000000"; // 8byte
    u64 cbl;
    u64 cb = c << 2;
    u64 cbr = cb | 2;
    ll k;
    bool lower_boundary_is_closer = (frac == 0);
    if (lower_boundary_is_closer) //[[unlikely]]
    {
        cbl = cb - 1;
        k = (q * 1262611 - 524031) >> 22;
    }
    else
    {
        cbl = cb - 2;
        k = (q * 1262611) >> 22;
    }

#if 0
    ll h = q + 1 + (((k) * (-1741647)) >> 19); //[1,4]
    const u64x2 *pow10_ptr = &g[-kMin];
    u64x2 pow10 = pow10_ptr[-k];
    u64 lo = pow10.lo;
    u64 hi = pow10.hi;
    u64 x1_l, x1_m, x1_r;
    u64 y1_l, y1_m, y1_r, y0_l, y0_m, y0_r;

    // _mulx_u64(lo, cbl << h, &x1_l); // x0 not use
    // y0_l = _mulx_u64(hi, cbl << h, &y1_l);
    // _addcarry_u64(_addcarry_u64(0, y0_l, x1_l, &y0_l), y1_l, 0, &y1_l);
    // u64 vbl = y1_l | (y0_l > 1);

    // _mulx_u64(lo, cb << h,  &x1_m); // x0 not use
    // y0_m = _mulx_u64(hi, cb << h,  &y1_m);
    // _addcarry_u64(_addcarry_u64(0, y0_m, x1_m, &y0_m), y1_m, 0, &y1_m);
    // u64 vb  = y1_m | (y0_m > 1);

    // _mulx_u64(lo, cbr << h, &x1_r); // x0 not use
    // y0_r = _mulx_u64(hi, cbr << h, &y1_r);
    // _addcarry_u64(_addcarry_u64(0, y0_r, x1_r, &y0_r), y1_r, 0, &y1_r);
    // u64 vbr = y1_r | (y0_r > 1);

    // u64 r1 = hi >> (63 - h);
    // u64 r2 = ((hi << h) << 1) | (lo >> (63 - h));
    // u64 r2_vbr = r2 + y0;
    // u64 r1_vbr = r1 + y1 + (r2_vbr < r2);
    // u64 vbr = r1_vbr | (r2_vbr > 1);
    u64 vbl = RoundToOdd(pow10, cbl << h);
    u64 vb = RoundToOdd(pow10, cb << h);
    u64 vbr = RoundToOdd(pow10, cbr << h);
    u64 lower = vbl + (c & 1);
    u64 upper = vbr - (c & 1);
    u64 s = vb >> 2;
    u64 sp = s / 10;
    //u64 sp = ( (u128)(s) * (u128)(1844674407370955162ull) ) >> 64;
    //_mulx_u64(1844674407370955162ull, s, &sp); // sp = s/10
    u64 sp10 = sp * 10;
    u64 digit_out = s;
    digit_out += (((vb & -4) < lower) |
                  ((vb | 3) < upper) |
                  ((vb & 3) == 3) |
                  ((vb & 7) == 6)); // s or s + 1
    if (lower <= sp10 * 4)
        digit_out = sp10;
    if (sp10 * 4 + 40 <= upper)
        digit_out = sp10 + 10;
#else
    u64 sig;
    i32 exp_dec;
    f64_bin_to_dec(c, q, &sig, &exp_dec);
    u64 digit_out = sig;
    ll e10 = exp_dec;

#endif

    // compute digit_out and k end; then print to buffer
    // result = digit_out * 10^k
    // ll e10 = k;
    if (((u64)(1e15) <= digit_out) & (digit_out < (u64)(1e16))) // 16 digit
    {
        digit_out *= 10; // format to 17 digit
        e10 += 15;
    }
    else
    {
        e10 += 16;
    }
    int flag = ((-7 <= e10) & (e10 <= -1));
    int flag2 = ((-7 <= e10) & (e10 <= 0));
    uint32_t high1 = digit_out / (ll)(1e16);       // 1<= high1 <=9
    uint32_t high9 = digit_out / (ll)(1e8);        // 1e8 <= high9 < 1e9
    uint32_t high2_9 = high9 - high1 * (ll)(1e8);  // 0<= high2_9 < 1e8
    uint32_t low8 = digit_out - high9 * (ll)(1e8); // 0<= low8 < 1e8
    int ctz;                                       // count tail zero
    // const __m128i a0 = Convert8DigitsSSE2((uint32_t)high2_9);
    // const __m128i a1 = Convert8DigitsSSE2((uint32_t)low8);
    //  Convert to bytes, add '0' , or '0'
    // const __m128i va = _mm_or_si128(_mm_packus_epi16(a0, a1), _mm_set1_epi8('0'));
    //__m128i num_low16_print = va;
    //  ll high2_9_print = _mm_extract_epi64(num_low16_print, 0);
    //  ll low8_print = _mm_extract_epi64(num_low16_print, 1);
    //  Count tail zero
    // const unsigned mask = _mm_movemask_epi8(_mm_cmpeq_epi8(va, _mm_set1_epi8('0')));
    // #ifdef _MSC_VER
    //     unsigned long ctz_t;
    //     _BitScanForward(&ctz_t, ((~mask) << 16) | (0x4000 << flag));
    //     ctz = ctz_t;
    // #else
    //     ctz = __builtin_clz(((~mask) << 16) | (0x4000 << flag)); // when ctz = 16 and flag==0 set ctz=17
    // #endif
    char *ptr = buffer;
    ll start_write_pos = flag ? (1 - e10) : 0; // when e10 range in [-7,-1] , start_pos = 1-e10
    *(short *)(ptr + start_write_pos) = high1 | ('.' * 256 + '0');
    //_mm_storeu_si128((__m128i *)(ptr + start_write_pos + 2 - flag), num_low16_print);
    u64 aabbccdd = high2_9;
    u64 eeffgghh = low8;
    u64 aabb = (aabbccdd * 112589990685ull) >> 50; // 1e8 / 1e4
    u64 ccdd = aabbccdd - aabb * 10000;
    u64 eeff = (eeffgghh * 112589990685ull) >> 50;
    u64 gghh = eeffgghh - eeff * 10000;
    u64 aa = (aabb * 10486) >> 20;
    u64 bb = aabb - aa * 100;
    u64 cc = (ccdd * 10486) >> 20;
    u64 dd = ccdd - cc * 100;
    u64 ee = (eeff * 10486) >> 20;
    u64 ff = eeff - ee * 100;
    u64 gg = (gghh * 10486) >> 20;
    u64 hh = gghh - gg * 100;
    const u64 MASK = 0x0f0f0f0f0f0f0f0full;
    char *buf = (ptr + start_write_pos + 2 - flag);
    byte_copy_2(buf + 0, digit_table + aa * 2);
    byte_copy_2(buf + 2, digit_table + bb * 2); // or  bb*256 - 2559*((bb*103)>>10) + 0x3030
    byte_copy_2(buf + 4, digit_table + cc * 2);
    byte_copy_2(buf + 6, digit_table + dd * 2);
    byte_copy_2(buf + 8, digit_table + ee * 2);
    byte_copy_2(buf + 10, digit_table + ff * 2);
    byte_copy_2(buf + 12, digit_table + gg * 2);
    byte_copy_2(buf + 14, digit_table + hh * 2);
    u64 num8_1 = *(u64 *)(buf + 0) & MASK;
    u64 num8_2 = *(u64 *)(buf + 8) & MASK;
    u64 tz1 = __builtin_clzll(num8_1) >> 3;
    u64 tz2 = __builtin_clzll(num8_2) >> 3;
    ctz = ((tz2 == 8) ? (tz1 + 8) : tz2);
    if ((flag == 0) && (ctz == 16))
        ctz = 17;
#if 1
    if (((1 <= e10) & (e10 <= 8))) // not exec this code the print result also right
    {
        // v range in [1e1 ,1e9)
        // 23 -> 23
        // 100 -> 100
        // 12.34 -> 12.34
        ll float_point_pos = e10 + 1;
        ll dot_right_value = *(ll *)(ptr + float_point_pos + 1);
        // ll dot_left_value = _mm_extract_epi64(num_low16_print, 0); // high2_9 print result; sse4.1
        ll dot_left_value = *(i64 *)(buf + 0);
        *(ll *)(ptr + 1) = dot_left_value; // move high2_9 print result to orgin dot pos

        // origin code ; when print integer not open
        // *(ptr + float_point_pos) = (ctz + e10 < 16) ? '.' : 0;                     // xx.00 ; if dot right value = all 0 , not print dot
        // *(ll *)(ptr + float_point_pos + 1) = dot_right_value;                      //
        // int end_pos = (18 - ctz > float_point_pos) ? (18 - ctz) : float_point_pos; // max(18-ctz,float_point_pos)
        // *(ptr + end_pos) = '\0';                                                   // remove tail zero
        // return sign + ((ctz + e10 < 16) ? end_pos : float_point_pos);              // length = if(ctz + e10 < 16) end_pos else float_point_pos

        // when print integer open,use this code
        *(ptr + float_point_pos) = '.';
        *(ll *)(ptr + float_point_pos + 1) = dot_right_value;
        *(ptr + 18 - ctz) = '\0';
        return sign + 18 - ctz;
    }
#endif
    const ll *exp_ptr = &exp_result3[324];
    *(ll *)(ptr + 18 - flag + start_write_pos - ctz) = flag2 ? 0 : exp_ptr[e10]; // remove tail zero
    // when double value < 1e-309 ; equal digit_out < 1e15
    // also 5e-324 <= v < 1e-309 ;
#if 1
    if (digit_out < (ull)(1e15)) // [[unlikely]]
    {
        char *buf_ptr = ptr;
        u64 len = dec_length(digit_out); // 1<= len <= 15
        u64 tz_num = ctz;
        u64 lz_num = 17 - len; // left zero num: lz_num >= 2
        // such as 00000001234500000 ; significant = 5,tz = 5,lz= 7
        // u64 signficant = 17 - tz_num - lz_num;// also = len - ctz
        u64 signficant = len - tz_num;
        u64 start_pos = lz_num + 1; // 0.00xxx ; first x pos is 3 + 1
        if (signficant > 1ull)      // [[likely]]
        {
            buf_ptr[0] = buf_ptr[start_pos];
            if (signficant <= 9) // move 8 byte
                *(i64 *)&buf_ptr[2] = *(i64 *)&buf_ptr[start_pos + 1];
            else
            { // move 16 byte
                *(i64 *)&buf_ptr[2] = *(i64 *)&buf_ptr[start_pos + 1];
                *(i64 *)&buf_ptr[2 + 8] = *(i64 *)&buf_ptr[start_pos + 1 + 8];
            }
            *(i64 *)&buf_ptr[signficant + 1] = exp_ptr[e10 - lz_num]; // write exp10 ASCII
            return sign + 1 + signficant + 5;                         // 1.2e-310 , [1.2] -> 1+signficant , [e-310] -> 5  ; sign is 1, negative double
        }
        else
        {
            *(char *)&buf_ptr[0] = buf_ptr[start_pos];
            *(i64 *)&buf_ptr[1] = exp_ptr[e10 - lz_num];
            return sign + 1 + 5; // example : 5e-324 , 1e-310 , 1e-323 ; sign is 1, negative double
        }
    }
#endif
    int exp10_length = 4 | (e10 >= 100) | (e10 <= -100);                          // 4 + (abs(e10)>=100) ; e+10 = 4 , e+100 = 5
    return sign + start_write_pos + 18 - ctz - flag + (flag2 ? 0 : exp10_length); // write buf length
}
#endif
extern "C" char *yy_double_to_string(double val, char *buf)
{
    u64 raw = f64_to_raw(val);
    return (char *)write_f64_raw((u8 *)buf, raw);
}

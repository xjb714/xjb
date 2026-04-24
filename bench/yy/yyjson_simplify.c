/*==============================================================================
 Copyright (c) 2020 YaoYuan <ibireme@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 *============================================================================*/

#include "yyjson.h"
#include <math.h> /* for `HUGE_VAL/INFINIY/NAN` macros, no libm required */



/*==============================================================================
 * MARK: - Warning Suppress (Private)
 *============================================================================*/

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wunused-label"
#   pragma clang diagnostic ignored "-Wunused-macros"
#   pragma clang diagnostic ignored "-Wunused-variable"
#elif YYJSON_IS_REAL_GCC
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Wunused-label"
#   pragma GCC diagnostic ignored "-Wunused-macros"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#elif defined(_MSC_VER)
#   pragma warning(disable:4100) /* unreferenced formal parameter */
#   pragma warning(disable:4101) /* unreferenced variable */
#   pragma warning(disable:4102) /* unreferenced label */
#   pragma warning(disable:4127) /* conditional expression is constant */
#   pragma warning(disable:4702) /* unreachable code */
#   pragma warning(disable:4706) /* assignment within conditional expression */
#endif



/*==============================================================================
 * MARK: - Version (Public)
 *============================================================================*/

uint32_t yyjson_version(void) {
    return YYJSON_VERSION_HEX;
}



/*==============================================================================
 * MARK: - Flags (Private)
 *============================================================================*/

/* msvc intrinsic */
#if YYJSON_MSC_VER >= 1400
#   include <intrin.h>
#   if defined(_M_AMD64) || defined(_M_ARM64)
#       define MSC_HAS_BIT_SCAN_64 1
#       pragma intrinsic(_BitScanForward64)
#       pragma intrinsic(_BitScanReverse64)
#   else
#       define MSC_HAS_BIT_SCAN_64 0
#   endif
#   if defined(_M_AMD64) || defined(_M_ARM64) || \
        defined(_M_IX86) || defined(_M_ARM)
#       define MSC_HAS_BIT_SCAN 1
#       pragma intrinsic(_BitScanForward)
#       pragma intrinsic(_BitScanReverse)
#   else
#       define MSC_HAS_BIT_SCAN 0
#   endif
#   if defined(_M_AMD64)
#       define MSC_HAS_UMUL128 1
#       pragma intrinsic(_umul128)
#   else
#       define MSC_HAS_UMUL128 0
#   endif
#else
#   define MSC_HAS_BIT_SCAN_64 0
#   define MSC_HAS_BIT_SCAN 0
#   define MSC_HAS_UMUL128 0
#endif

/* gcc builtin */
#if yyjson_has_builtin(__builtin_clzll) || yyjson_gcc_available(3, 4, 0)
#   define GCC_HAS_CLZLL 1
#else
#   define GCC_HAS_CLZLL 0
#endif

#if yyjson_has_builtin(__builtin_ctzll) || yyjson_gcc_available(3, 4, 0)
#   define GCC_HAS_CTZLL 1
#else
#   define GCC_HAS_CTZLL 0
#endif

/* int128 type */
#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16) && \
    (defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER))
#    define YYJSON_HAS_INT128 1
#else
#    define YYJSON_HAS_INT128 0
#endif

/* IEEE 754 floating-point binary representation */
#if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
#   define YYJSON_HAS_IEEE_754 1
#elif FLT_RADIX == 2 && \
    FLT_MANT_DIG == 24 && FLT_DIG == 6 && \
    FLT_MIN_EXP == -125 && FLT_MAX_EXP == 128 && \
    FLT_MIN_10_EXP == -37 && FLT_MAX_10_EXP == 38 && \
    DBL_MANT_DIG == 53 && DBL_DIG == 15 && \
    DBL_MIN_EXP == -1021 && DBL_MAX_EXP == 1024 && \
    DBL_MIN_10_EXP == -307 && DBL_MAX_10_EXP == 308
#   define YYJSON_HAS_IEEE_754 1
#else
#   define YYJSON_HAS_IEEE_754 0
#   undef  YYJSON_DISABLE_FAST_FP_CONV
#   define YYJSON_DISABLE_FAST_FP_CONV 1
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
 YYJSON_DOUBLE_MATH_CORRECT as 1 to enable the fast path calculation. This is
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
#    define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif

#if defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD != 0 && FLT_EVAL_METHOD != 1
#    define YYJSON_DOUBLE_MATH_CORRECT 0
#elif defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(_X86_) || defined(__X86__) || defined(_M_IX86) || \
    defined(__I86__) || defined(__IA32__) || defined(__THW_INTEL)
#   if (defined(_MSC_VER) && defined(_M_IX86_FP) && _M_IX86_FP == 2) || \
        (defined(__SSE2_MATH__) && __SSE2_MATH__)
#       define YYJSON_DOUBLE_MATH_CORRECT 1
#   else
#       define YYJSON_DOUBLE_MATH_CORRECT 0
#   endif
#elif defined(__mc68000__) || defined(__pnacl__) || defined(__native_client__)
#   define YYJSON_DOUBLE_MATH_CORRECT 0
#else
#   define YYJSON_DOUBLE_MATH_CORRECT 1
#endif

/*
 Detect the endianness at compile-time.
 YYJSON_ENDIAN == YYJSON_BIG_ENDIAN
 YYJSON_ENDIAN == YYJSON_LITTLE_ENDIAN
 */
#define YYJSON_BIG_ENDIAN       4321
#define YYJSON_LITTLE_ENDIAN    1234

#if yyjson_has_include(<sys/types.h>)
#    include <sys/types.h> /* POSIX */
#endif
#if yyjson_has_include(<endian.h>)
#    include <endian.h> /* Linux */
#elif yyjson_has_include(<sys/endian.h>)
#    include <sys/endian.h> /* BSD, Android */
#elif yyjson_has_include(<machine/endian.h>)
#    include <machine/endian.h> /* BSD, Darwin */
#endif

#if defined(BYTE_ORDER) && BYTE_ORDER
#   if defined(BIG_ENDIAN) && (BYTE_ORDER == BIG_ENDIAN)
#       define YYJSON_ENDIAN YYJSON_BIG_ENDIAN
#   elif defined(LITTLE_ENDIAN) && (BYTE_ORDER == LITTLE_ENDIAN)
#       define YYJSON_ENDIAN YYJSON_LITTLE_ENDIAN
#   endif
#elif defined(__BYTE_ORDER) && __BYTE_ORDER
#   if defined(__BIG_ENDIAN) && (__BYTE_ORDER == __BIG_ENDIAN)
#       define YYJSON_ENDIAN YYJSON_BIG_ENDIAN
#   elif defined(__LITTLE_ENDIAN) && (__BYTE_ORDER == __LITTLE_ENDIAN)
#       define YYJSON_ENDIAN YYJSON_LITTLE_ENDIAN
#   endif
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__
#   if defined(__ORDER_BIG_ENDIAN__) && \
        (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#       define YYJSON_ENDIAN YYJSON_BIG_ENDIAN
#   elif defined(__ORDER_LITTLE_ENDIAN__) && \
        (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#       define YYJSON_ENDIAN YYJSON_LITTLE_ENDIAN
#   endif
#elif (defined(__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__ == 1) || \
    defined(__i386) || defined(__i386__) || \
    defined(_X86_) || defined(__X86__) || \
    defined(_M_IX86) || defined(__THW_INTEL__) || \
    defined(__x86_64) || defined(__x86_64__) || \
    defined(__amd64) || defined(__amd64__) || \
    defined(_M_AMD64) || defined(_M_X64) || \
    defined(_M_ARM) || defined(_M_ARM64) || \
    defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
    defined(__EMSCRIPTEN__) || defined(__wasm__) || \
    defined(__loongarch__)
#   define YYJSON_ENDIAN YYJSON_LITTLE_ENDIAN
#elif (defined(__BIG_ENDIAN__) && __BIG_ENDIAN__ == 1) || \
    defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
    defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__) || \
    defined(__or1k__) || defined(__OR1K__)
#   define YYJSON_ENDIAN YYJSON_BIG_ENDIAN
#else
#   define YYJSON_ENDIAN 0 /* unknown endian, detect at run-time */
#endif

/*
 This macro controls how yyjson handles unaligned memory accesses.

 By default, yyjson uses `memcpy()` for memory copying. This allows the compiler
 to optimize the code and emit unaligned memory access instructions when
 supported by the target architecture.

 However, on some older compilers or architectures where `memcpy()` is not
 well-optimized and may result in unnecessary function calls, defining this
 macro as 1 may help. In such cases, yyjson switches to manual byte-by-byte
 access, which can potentially improve performance.

 An example of the generated assembly code for ARM can be found here:
 https://godbolt.org/z/334jjhxPT

 This flag is already enabled for common architectures in the following code,
 so manual configuration is usually unnecessary. If unsure, you can check the
 generated assembly or run benchmarks to make an informed decision.
 */
#ifndef YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
#   if defined(__ia64) || defined(_IA64) || defined(__IA64__) ||  \
        defined(__ia64__) || defined(_M_IA64) || defined(__itanium__)
#       define YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS 1 /* Itanium */
#   elif (defined(__arm__) || defined(__arm64__) || defined(__aarch64__)) && \
        (defined(__GNUC__) || defined(__clang__)) && \
        (!defined(__ARM_FEATURE_UNALIGNED) || !__ARM_FEATURE_UNALIGNED)
#       define YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS 1 /* ARM */
#   elif defined(__sparc) || defined(__sparc__)
#       define YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS 1 /* SPARC */
#   elif defined(__mips) || defined(__mips__) || defined(__MIPS__)
#       define YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS 1 /* MIPS */
#   elif defined(__m68k__) || defined(M68000)
#       define YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS 1 /* M68K */
#   else
#       define YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS 0
#   endif
#endif

/*
 Estimated initial ratio of the JSON data (data_size / value_count).
 For example:

    data:        {"id":12345678,"name":"Harry"}
    data_size:   30
    value_count: 5
    ratio:       6

 yyjson uses dynamic memory with a growth factor of 1.5 when reading and writing
 JSON, the ratios below are used to determine the initial memory size.

 A too large ratio will waste memory, and a too small ratio will cause multiple
 memory growths and degrade performance. Currently, these ratios are generated
 with some commonly used JSON datasets.
 */
#define YYJSON_READER_ESTIMATED_PRETTY_RATIO 16
#define YYJSON_READER_ESTIMATED_MINIFY_RATIO 6
#define YYJSON_WRITER_ESTIMATED_PRETTY_RATIO 32
#define YYJSON_WRITER_ESTIMATED_MINIFY_RATIO 18

/* The initial and maximum size of the memory pool's chunk in yyjson_mut_doc. */
#define YYJSON_MUT_DOC_STR_POOL_INIT_SIZE   0x100
#define YYJSON_MUT_DOC_STR_POOL_MAX_SIZE    0x10000000
#define YYJSON_MUT_DOC_VAL_POOL_INIT_SIZE   (0x10 * sizeof(yyjson_mut_val))
#define YYJSON_MUT_DOC_VAL_POOL_MAX_SIZE    (0x1000000 * sizeof(yyjson_mut_val))

/* The minimum size of the dynamic allocator's chunk. */
#define YYJSON_ALC_DYN_MIN_SIZE             0x1000

/* Default value for compile-time options. */
#ifndef YYJSON_DISABLE_READER
#define YYJSON_DISABLE_READER 0
#endif
#ifndef YYJSON_DISABLE_WRITER
#define YYJSON_DISABLE_WRITER 0
#endif
#ifndef YYJSON_DISABLE_INCR_READER
#define YYJSON_DISABLE_INCR_READER 0
#endif
#ifndef YYJSON_DISABLE_UTILS
#define YYJSON_DISABLE_UTILS 0
#endif
#ifndef YYJSON_DISABLE_FAST_FP_CONV
#define YYJSON_DISABLE_FAST_FP_CONV 0
#endif
#ifndef YYJSON_DISABLE_NON_STANDARD
#define YYJSON_DISABLE_NON_STANDARD 0
#endif
#ifndef YYJSON_DISABLE_UTF8_VALIDATION
#define YYJSON_DISABLE_UTF8_VALIDATION 0
#endif
#ifndef YYJSON_READER_DEPTH_LIMIT
#define YYJSON_READER_DEPTH_LIMIT 0
#endif

/*==============================================================================
 * MARK: - Macros (Private)
 *============================================================================*/

/* Macros used for loop unrolling and other purpose. */
#define repeat2(x)  { x x }
#define repeat4(x)  { x x x x }
#define repeat8(x)  { x x x x x x x x }
#define repeat16(x) { x x x x x x x x x x x x x x x x }

#define repeat2_incr(x)   { x(0)  x(1) }
#define repeat4_incr(x)   { x(0)  x(1)  x(2)  x(3) }
#define repeat8_incr(x)   { x(0)  x(1)  x(2)  x(3)  x(4)  x(5)  x(6)  x(7)  }
#define repeat16_incr(x)  { x(0)  x(1)  x(2)  x(3)  x(4)  x(5)  x(6)  x(7)  \
                            x(8)  x(9)  x(10) x(11) x(12) x(13) x(14) x(15) }
#define repeat_in_1_18(x) { x(1)  x(2)  x(3)  x(4)  x(5)  x(6)  x(7)  x(8)  \
                            x(9)  x(10) x(11) x(12) x(13) x(14) x(15) x(16) \
                            x(17) x(18) }

/* Macros used to provide branch prediction information for compiler. */
#undef  likely
#define likely(x)       yyjson_likely(x)
#undef  unlikely
#define unlikely(x)     yyjson_unlikely(x)

/* Macros used to provide inline information for compiler. */
#undef  static_inline
#define static_inline   static yyjson_inline
#undef  static_noinline
#define static_noinline static yyjson_noinline

/* Macros for min and max. */
#undef  yyjson_min
#define yyjson_min(x, y) ((x) < (y) ? (x) : (y))
#undef  yyjson_max
#define yyjson_max(x, y) ((x) > (y) ? (x) : (y))

/* Used to write u64 literal for C89 which doesn't support "ULL" suffix. */
#undef  U64
#define U64(hi, lo) ((((u64)hi##UL) << 32U) + lo##UL)
#undef  U32
#define U32(hi) ((u32)(hi##UL))

/* Used to cast away (remove) const qualifier. */
#define constcast(type) (type)(void *)(size_t)(const void *)

/*
 Compiler barriers for single variables.

 These macros inform GCC that a read or write access to the given memory
 location will occur, preventing certain compiler optimizations or reordering
 around the access to 'val'. They do not emit any actual instructions.

 This is useful when GCC's default optimization strategies are suboptimal and
 precise control over memory access patterns is required.
 These barriers are not needed when using Clang or MSVC.
 */
#if YYJSON_IS_REAL_GCC
#   define gcc_load_barrier(val)   __asm__ volatile(""::"m"(val))
#   define gcc_store_barrier(val)  __asm__ volatile("":"=m"(val))
#   define gcc_full_barrier(val)   __asm__ volatile("":"=m"(val):"m"(val))
#else
#   define gcc_load_barrier(val)
#   define gcc_store_barrier(val)
#   define gcc_full_barrier(val)
#endif



/*==============================================================================
 * MARK: - Constants (Private)
 *============================================================================*/

/* Common error messages. */
#define MSG_FOPEN       "failed to open file"
#define MSG_FREAD       "failed to read file"
#define MSG_FWRITE      "failed to write file"
#define MSG_FCLOSE      "failed to close file"
#define MSG_MALLOC      "failed to allocate memory"
#define MSG_CHAR_T      "invalid literal, expected 'true'"
#define MSG_CHAR_F      "invalid literal, expected 'false'"
#define MSG_CHAR_N      "invalid literal, expected 'null'"
#define MSG_CHAR        "unexpected character, expected a JSON value"
#define MSG_ARR_END     "unexpected character, expected ',' or ']'"
#define MSG_OBJ_KEY     "unexpected character, expected a string key"
#define MSG_OBJ_SEP     "unexpected character, expected ':' after key"
#define MSG_OBJ_END     "unexpected character, expected ',' or '}'"
#define MSG_GARBAGE     "unexpected content after document"
#define MSG_NOT_END     "unexpected end of data"
#define MSG_COMMENT     "unclosed multiline comment"
#define MSG_COMMA       "trailing comma is not allowed"
#define MSG_NAN_INF     "nan or inf number is not allowed"
#define MSG_ERR_TYPE    "invalid JSON value type"
#define MSG_ERR_BOM     "UTF-8 byte order mark (BOM) is not supported"
#define MSG_ERR_UTF8    "invalid utf-8 encoding in string"
#define MSG_ERR_UTF16   "UTF-16 encoding is not supported"
#define MSG_ERR_UTF32   "UTF-32 encoding is not supported"
#define MSG_DEPTH       "depth limit exceeded"

/* U64 constant values */
#undef  U64_MAX
#define U64_MAX         U64(0xFFFFFFFF, 0xFFFFFFFF)
#undef  I64_MAX
#define I64_MAX         U64(0x7FFFFFFF, 0xFFFFFFFF)
#undef  USIZE_MAX
#define USIZE_MAX       ((usize)(~(usize)0))

/* Maximum number of digits for reading u32/u64/usize safety (not overflow). */
#undef  U32_SAFE_DIG
#define U32_SAFE_DIG    9   /* u32 max is 4294967295, 10 digits */
#undef  U64_SAFE_DIG
#define U64_SAFE_DIG    19  /* u64 max is 18446744073709551615, 20 digits */
#undef  USIZE_SAFE_DIG
#define USIZE_SAFE_DIG  (sizeof(usize) == 8 ? U64_SAFE_DIG : U32_SAFE_DIG)

/* Inf bits (positive) */
#define F64_BITS_INF U64(0x7FF00000, 0x00000000)

/* NaN bits (quiet NaN, no payload, no sign) */
#if defined(__hppa__) || (defined(__mips__) && !defined(__mips_nan2008))
#define F64_BITS_NAN U64(0x7FF7FFFF, 0xFFFFFFFF)
#else
#define F64_BITS_NAN U64(0x7FF80000, 0x00000000)
#endif

/* maximum significant digits count in decimal when reading double number */
#define F64_MAX_DEC_DIG 768

/* maximum decimal power of double number (1.7976931348623157e308) */
#define F64_MAX_DEC_EXP 308

/* minimum decimal power of double number (4.9406564584124654e-324) */
#define F64_MIN_DEC_EXP (-324)

/* maximum binary power of double number */
#define F64_MAX_BIN_EXP 1024

/* minimum binary power of double number */
#define F64_MIN_BIN_EXP (-1021)

/* float/double number bits */
#define F32_BITS 32
#define F64_BITS 64

/* float/double number exponent part bits */
#define F32_EXP_BITS 8
#define F64_EXP_BITS 11

/* float/double number significand part bits */
#define F32_SIG_BITS 23
#define F64_SIG_BITS 52

/* float/double number significand part bits (with 1 hidden bit) */
#define F32_SIG_FULL_BITS 24
#define F64_SIG_FULL_BITS 53

/* float/double number significand bit mask */
#define F32_SIG_MASK U32(0x007FFFFF)
#define F64_SIG_MASK U64(0x000FFFFF, 0xFFFFFFFF)

/* float/double number exponent bit mask */
#define F32_EXP_MASK U32(0x7F800000)
#define F64_EXP_MASK U64(0x7FF00000, 0x00000000)

/* float/double number exponent bias */
#define F32_EXP_BIAS 127
#define F64_EXP_BIAS 1023

/* float/double number significant digits count in decimal */
#define F32_DEC_DIG 9
#define F64_DEC_DIG 17

/* buffer length required for float/double number writer */
#define FP_BUF_LEN 40

/* maximum length of a number in incremental parsing */
#define INCR_NUM_MAX_LEN 1024



/*==============================================================================
 * MARK: - Types (Private)
 *============================================================================*/

/** Type define for primitive types. */
typedef float       f32;
typedef double      f64;
typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;
typedef size_t      usize;

/** 128-bit integer, used by floating-point number reader and writer. */
#if YYJSON_HAS_INT128
__extension__ typedef __int128          i128;
__extension__ typedef unsigned __int128 u128;
#endif

/** 16/32/64-bit vector */
typedef struct v16 { char c[2]; } v16;
typedef struct v32 { char c[4]; } v32;
typedef struct v64 { char c[8]; } v64;

/** 16/32/64-bit vector union */
typedef union v16_uni { v16 v; u16 u; } v16_uni;
typedef union v32_uni { v32 v; u32 u; } v32_uni;
typedef union v64_uni { v64 v; u64 u; } v64_uni;



/*==============================================================================
 * MARK: - Load/Store Utils (Private)
 *============================================================================*/

#define byte_move_idx(x) ((char *)dst)[x] = ((const char *)src)[x];
#define byte_move_src(x) ((char *)tmp)[x] = ((const char *)src)[x];
#define byte_move_dst(x) ((char *)dst)[x] = ((const char *)tmp)[x];

/** Same as `memcpy(dst, src, 2)`, no overlap. */
static_inline void byte_copy_2(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(dst, src, 2);
#else
    repeat2_incr(byte_move_idx)
#endif
}

/** Same as `memcpy(dst, src, 4)`, no overlap. */
static_inline void byte_copy_4(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(dst, src, 4);
#else
    repeat4_incr(byte_move_idx)
#endif
}

/** Same as `memcpy(dst, src, 8)`, no overlap. */
static_inline void byte_copy_8(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(dst, src, 8);
#else
    repeat8_incr(byte_move_idx)
#endif
}

/** Same as `memcpy(dst, src, 16)`, no overlap. */
static_inline void byte_copy_16(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(dst, src, 16);
#else
    repeat16_incr(byte_move_idx)
#endif
}

/** Same as `memmove(dst, src, 2)`, allows overlap. */
static_inline void byte_move_2(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    u16 tmp;
    memcpy(&tmp, src, 2);
    memcpy(dst, &tmp, 2);
#else
    char tmp[2];
    repeat2_incr(byte_move_src)
    repeat2_incr(byte_move_dst)
#endif
}

/** Same as `memmove(dst, src, 4)`, allows overlap. */
static_inline void byte_move_4(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    u32 tmp;
    memcpy(&tmp, src, 4);
    memcpy(dst, &tmp, 4);
#else
    char tmp[4];
    repeat4_incr(byte_move_src)
    repeat4_incr(byte_move_dst)
#endif
}

/** Same as `memmove(dst, src, 8)`, allows overlap. */
static_inline void byte_move_8(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    u64 tmp;
    memcpy(&tmp, src, 8);
    memcpy(dst, &tmp, 8);
#else
    char tmp[8];
    repeat8_incr(byte_move_src)
    repeat8_incr(byte_move_dst)
#endif
}

/** Same as `memmove(dst, src, 16)`, allows overlap. */
static_inline void byte_move_16(void *dst, const void *src) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    char *pdst = (char *)dst;
    const char *psrc = (const char *)src;
    u64 tmp1, tmp2;
    memcpy(&tmp1, psrc, 8);
    memcpy(&tmp2, psrc + 8, 8);
    memcpy(pdst, &tmp1, 8);
    memcpy(pdst + 8, &tmp2, 8);
#else
    char tmp[16];
    repeat16_incr(byte_move_src)
    repeat16_incr(byte_move_dst)
#endif
}

/** Same as `memmove(dst, src, n)`, but only `dst <= src` and `n <= 16`. */
static_inline void byte_move_forward(void *dst, void *src, usize n) {
    char *d = (char *)dst, *s = (char *)src;
    n += (n % 2); /* round up to even */
    if (n == 16) { byte_move_16(d, s); return; }
    if (n >= 8) { byte_move_8(d, s); n -= 8; d += 8; s += 8; }
    if (n >= 4) { byte_move_4(d, s); n -= 4; d += 4; s += 4; }
    if (n >= 2) { byte_move_2(d, s); }
}

/** Same as `memcmp(buf, pat, 2) == 0`. */
static_inline bool byte_match_2(void *buf, const char *pat) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    v16_uni u1, u2;
    memcpy(&u1, buf, 2);
    memcpy(&u2, pat, 2);
    return u1.u == u2.u;
#else
    return ((char *)buf)[0] == ((const char *)pat)[0] &&
           ((char *)buf)[1] == ((const char *)pat)[1];
#endif
}

/** Same as `memcmp(buf, pat, 4) == 0`. */
static_inline bool byte_match_4(void *buf, const char *pat) {
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    v32_uni u1, u2;
    memcpy(&u1, buf, 4);
    memcpy(&u2, pat, 4);
    return u1.u == u2.u;
#else
    return ((char *)buf)[0] == ((const char *)pat)[0] &&
           ((char *)buf)[1] == ((const char *)pat)[1] &&
           ((char *)buf)[2] == ((const char *)pat)[2] &&
           ((char *)buf)[3] == ((const char *)pat)[3];
#endif
}

/** Loads 2 bytes from `src` as a u16 (native-endian). */
static_inline u16 byte_load_2(const void *src) {
    v16_uni uni;
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(&uni, src, 2);
#else
    uni.v.c[0] = ((const char *)src)[0];
    uni.v.c[1] = ((const char *)src)[1];
#endif
    return uni.u;
}

/** Loads 3 bytes from `src` as a u32 (native-endian). */
static_inline u32 byte_load_3(const void *src) {
    v32_uni uni;
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(&uni, src, 2);
    uni.v.c[2] = ((const char *)src)[2];
    uni.v.c[3] = 0;
#else
    uni.v.c[0] = ((const char *)src)[0];
    uni.v.c[1] = ((const char *)src)[1];
    uni.v.c[2] = ((const char *)src)[2];
    uni.v.c[3] = 0;
#endif
    return uni.u;
}

/** Loads 4 bytes from `src` as a u32 (native-endian). */
static_inline u32 byte_load_4(const void *src) {
    v32_uni uni;
#if !YYJSON_DISABLE_UNALIGNED_MEMORY_ACCESS
    memcpy(&uni, src, 4);
#else
    uni.v.c[0] = ((const char *)src)[0];
    uni.v.c[1] = ((const char *)src)[1];
    uni.v.c[2] = ((const char *)src)[2];
    uni.v.c[3] = ((const char *)src)[3];
#endif
    return uni.u;
}



/*==============================================================================
 * MARK: - Character Utils (Private)
 * These lookup tables were generated by `misc/make_tables.c`.
 *============================================================================*/

/* char_table1 */
#define CHAR_TYPE_ASCII     (1 << 0) /* Except: ["\], [0x00-0x1F, 0x80-0xFF] */
#define CHAR_TYPE_ASCII_SQ  (1 << 1) /* Except: ['\], [0x00-0x1F, 0x80-0xFF] */
#define CHAR_TYPE_SPACE     (1 << 2) /* Whitespace: [ \t\n\r] */
#define CHAR_TYPE_SPACE_EXT (1 << 3) /* Whitespace: [ \t\n\r\v\f], JSON5 */
#define CHAR_TYPE_NUM       (1 << 4) /* Number: [.-+0-9] */
#define CHAR_TYPE_COMMENT   (1 << 5) /* Comment: [/] */

/* char_table2 */
#define CHAR_TYPE_EOL       (1 << 0) /* End of line: [\r\n] */
#define CHAR_TYPE_EOL_EXT   (1 << 1) /* End of line: [\r\n], JSON5 */
#define CHAR_TYPE_ID_START  (1 << 2) /* ID start: [_$A-Za-z\], U+0080+ */
#define CHAR_TYPE_ID_NEXT   (1 << 3) /* ID next: [_$A-Za-z0-9\], U+0080+ */
#define CHAR_TYPE_ID_ASCII  (1 << 4) /* ID next ASCII: [_$A-Za-z0-9] */

/* char_table3 */
#define CHAR_TYPE_SIGN      (1 << 0) /* [-+] */
#define CHAR_TYPE_DIGIT     (1 << 1) /* [0-9] */
#define CHAR_TYPE_NONZERO   (1 << 2) /* [1-9] */
#define CHAR_TYPE_EXP       (1 << 3) /* [eE] */
#define CHAR_TYPE_DOT       (1 << 4) /* [.] */

static const u8 char_table1[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0C, 0x0C, 0x08, 0x08, 0x0C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x03, 0x02, 0x03, 0x03, 0x03, 0x03, 0x01,
    0x03, 0x03, 0x03, 0x13, 0x03, 0x13, 0x13, 0x23,
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x13, 0x13, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const u8 char_table2[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x00, 0x0C, 0x00, 0x00, 0x1C,
    0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C
};

static const u8 char_table3[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x10, 0x00,
    0x02, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** Match a whitespace: [ \t\n\r]. */
static_inline bool char_is_space(u8 c) {
    return !!(char_table1[c] & CHAR_TYPE_SPACE);
}

/** Match an extended whitespace: [ \t\n\r\\v\\f], JSON5 whitespace. */
static_inline bool char_is_space_ext(u8 c) {
    return !!(char_table1[c] & CHAR_TYPE_SPACE_EXT);
}

/** Match a JSON number: [.-+0-9]. */
static_inline bool char_is_num(u8 c) {
    return !!(char_table1[c] & CHAR_TYPE_NUM);
}

/** Match an ASCII character in string: ["\], [0x00-0x1F, 0x80-0xFF]. */
static_inline bool char_is_ascii_skip(u8 c) {
    return !!(char_table1[c] & CHAR_TYPE_ASCII);
}

/** Match an ASCII character single-quoted: ['\], [0x00-0x1F, 0x80-0xFF]. */
static_inline bool char_is_ascii_skip_sq(u8 c) {
    return !!(char_table1[c] & CHAR_TYPE_ASCII_SQ);
}

/** Match a trivia character: extended whitespace or comment. */
static_inline bool char_is_trivia(u8 c) {
    return !!(char_table1[c] & (CHAR_TYPE_SPACE_EXT | CHAR_TYPE_COMMENT));
}

/** Match a line end character: [\r\n]. */
static_inline bool char_is_eol(u8 c) {
    return !!(char_table2[c] & CHAR_TYPE_EOL);
}

/** Match an extended line end character: [\r\n], JSON5 line terminator. */
static_inline bool char_is_eol_ext(u8 c) {
    return !!(char_table2[c] & CHAR_TYPE_EOL_EXT);
}

/** Match an identifier name start: [_$A-Za-z\], U+0080+. */
static_inline bool char_is_id_start(u8 c) {
    return !!(char_table2[c] & CHAR_TYPE_ID_START);
}

/** Match an identifier name next: [_$A-Za-z0-9\], U+0080+. */
static_inline bool char_is_id_next(u8 c) {
    return !!(char_table2[c] & CHAR_TYPE_ID_NEXT);
}

/** Match an identifier name ASCII: [_$A-Za-z0-9]. */
static_inline bool char_is_id_ascii(u8 c) {
    return !!(char_table2[c] & CHAR_TYPE_ID_ASCII);
}

/** Match a sign: [+-] */
static_inline bool char_is_sign(u8 d) {
    return !!(char_table3[d] & CHAR_TYPE_SIGN);
}

/** Match a none-zero digit: [1-9] */
static_inline bool char_is_nonzero(u8 d) {
    return !!(char_table3[d] & CHAR_TYPE_NONZERO);
}

/** Match a digit: [0-9] */
static_inline bool char_is_digit(u8 d) {
    return !!(char_table3[d] & CHAR_TYPE_DIGIT);
}

/** Match an exponent sign: [eE]. */
static_inline bool char_is_exp(u8 d) {
    return !!(char_table3[d] & CHAR_TYPE_EXP);
}

/** Match a floating point indicator: [.eE]. */
static_inline bool char_is_fp(u8 d) {
    return !!(char_table3[d] & (CHAR_TYPE_DOT | CHAR_TYPE_EXP));
}

/** Match a digit or floating point indicator: [0-9.eE]. */
static_inline bool char_is_digit_or_fp(u8 d) {
    return !!(char_table3[d] & (CHAR_TYPE_DIGIT | CHAR_TYPE_DOT |
                                CHAR_TYPE_EXP));
}

/** Match a JSON container: `{` or `[`. */
static_inline bool char_is_ctn(u8 c) {
    return (c & 0xDF) == 0x5B; /* '[': 0x5B, '{': 0x7B */
}

/** Convert ASCII letter to lowercase; valid only for [A-Za-z]. */
static_inline u8 char_to_lower(u8 c) {
    return c | 0x20;
}

/** Match UTF-8 byte order mask. */
static_inline bool is_utf8_bom(const u8 *cur) {
    return byte_load_3(cur) == byte_load_3("\xEF\xBB\xBF");
}

/** Match UTF-16 byte order mask. */
static_inline bool is_utf16_bom(const u8 *cur) {
    return byte_load_2(cur) == byte_load_2("\xFE\xFF") ||
           byte_load_2(cur) == byte_load_2("\xFF\xFE");
}

/** Match UTF-32 byte order mask, need length check to avoid zero padding. */
static_inline bool is_utf32_bom(const u8 *cur) {
    return byte_load_4(cur) == byte_load_4("\x00\x00\xFE\xFF") ||
           byte_load_4(cur) == byte_load_4("\xFF\xFE\x00\x00");
}

/** Get the extended line end length. Used with `char_is_eol_ext`. */
static_inline usize ext_eol_len(const u8 *cur) {
    if (cur[0] < 0x80) return 1;
    if (cur[1] == 0x80 && (cur[2] == 0xA8 || cur[2] == 0xA9)) return 3;
    return 0;
}

/** Get the extended whitespace length. Used with `char_is_space_ext`. */
static_inline usize ext_space_len(const u8 *cur) {
    if (cur[0] < 0x80) {
        return 1;
    } else if (byte_load_2(cur) == byte_load_2("\xC2\xA0")) {
        return 2;
    } else if (byte_load_2(cur) == byte_load_2("\xE2\x80")) {
        if (cur[2] >= 0x80 && cur[2] <= 0x8A) return 3;
        if (cur[2] == 0xA8 || cur[2] == 0xA9 || cur[2] == 0xAF) return 3;
    } else {
        u32 uni = byte_load_3(cur);
        if (uni == byte_load_3("\xE1\x9A\x80") ||
            uni == byte_load_3("\xE2\x81\x9F") ||
            uni == byte_load_3("\xE3\x80\x80") ||
            uni == byte_load_3("\xEF\xBB\xBF")) return 3;
    }
    return 0;
}



/*==============================================================================
 * MARK: - Hex Character Reader (Private)
 * This function is used by JSON reader to read escaped characters.
 *============================================================================*/

/**
 This table is used to convert 4 hex character sequence to a number.
 A valid hex character [0-9A-Fa-f] will mapped to it's raw number [0x00, 0x0F],
 an invalid hex character will mapped to [0xF0].
 (generate with misc/make_tables.c)
 */
static const u8 hex_conv_table[256] = {
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0
};

/** Load 4 hex characters to `u16`, return true on valid input. */
static_inline bool hex_load_4(const u8 *src, u16 *dst) {
    u16 c0 = hex_conv_table[src[0]];
    u16 c1 = hex_conv_table[src[1]];
    u16 c2 = hex_conv_table[src[2]];
    u16 c3 = hex_conv_table[src[3]];
    u16 t0 = (u16)((c0 << 8) | c2);
    u16 t1 = (u16)((c1 << 8) | c3);
    *dst = (u16)((t0 << 4) | t1);
    return ((t0 | t1) & (u16)0xF0F0) == 0;
}

/** Load 2 hex characters to `u8`, return true on valid input. */
static_inline bool hex_load_2(const u8 *src, u8 *dst) {
    u8 c0 = hex_conv_table[src[0]];
    u8 c1 = hex_conv_table[src[1]];
    *dst = (u8)((c0 << 4) | c1);
    return ((c0 | c1) & 0xF0) == 0;
}

/** Match a hexadecimal numeric character: [0-9a-fA-F]. */
static_inline bool char_is_hex(u8 c) {
    return hex_conv_table[c] != 0xF0;
}



/*==============================================================================
 * MARK: - UTF8 Validation (Private)
 * Each Unicode code point is encoded using 1 to 4 bytes in UTF-8.
 * Validation is performed using a 4-byte mask and pattern-based approach,
 * which requires the input data to be padded with four zero bytes at the end.
 *============================================================================*/

/* Macro for concatenating four u8 into a u32 and keeping the byte order. */
#if YYJSON_ENDIAN == YYJSON_LITTLE_ENDIAN
#   define utf8_seq_def(name, a, b, c, d) \
        static const u32 utf8_seq_##name = 0x##d##c##b##a##UL;
#   define utf8_seq(name) utf8_seq_##name
#elif YYJSON_ENDIAN == YYJSON_BIG_ENDIAN
#   define utf8_seq_def(name, a, b, c, d) \
        static const u32 utf8_seq_##name = 0x##a##b##c##d##UL;
#   define utf8_seq(name) utf8_seq_##name
#else
#   define utf8_seq_def(name, a, b, c, d) \
        static const v32_uni utf8_uni_##name = {{ 0x##a, 0x##b, 0x##c, 0x##d }};
#   define utf8_seq(name) utf8_uni_##name.u
#endif

/*
 1-byte sequence (U+0000 to U+007F)
 bit min        [.......0] (U+0000)
 bit max        [.1111111] (U+007F)
 bit mask       [x.......] (80)
 bit pattern    [0.......] (00)
 */
utf8_seq_def(b1_mask, 80, 00, 00, 00)
utf8_seq_def(b1_patt, 00, 00, 00, 00)
#define is_utf8_seq1(uni) ( \
    ((uni & utf8_seq(b1_mask)) == utf8_seq(b1_patt)) )

/*
 2-byte sequence (U+0080 to U+07FF)
 bit min        [......10 ..000000] (U+0080)
 bit max        [...11111 ..111111] (U+07FF)
 bit mask       [xxx..... xx......] (E0 C0)
 bit pattern    [110..... 10......] (C0 80)
 bit require    [...xxxx. ........] (1E 00)
 */
utf8_seq_def(b2_mask, E0, C0, 00, 00)
utf8_seq_def(b2_patt, C0, 80, 00, 00)
utf8_seq_def(b2_requ, 1E, 00, 00, 00)
#define is_utf8_seq2(uni) ( \
    ((uni & utf8_seq(b2_mask)) == utf8_seq(b2_patt)) && \
    ((uni & utf8_seq(b2_requ))) )

/*
 3-byte sequence (U+0800 to U+FFFF)
 bit min        [........ ..100000 ..000000] (U+0800)
 bit max        [....1111 ..111111 ..111111] (U+FFFF)
 bit mask       [xxxx.... xx...... xx......] (F0 C0 C0)
 bit pattern    [1110.... 10...... 10......] (E0 80 80)
 bit require    [....xxxx ..x..... ........] (0F 20 00)

 3-byte invalid sequence, reserved for surrogate halves (U+D800 to U+DFFF)
 bit min        [....1101 ..100000 ..000000] (U+D800)
 bit max        [....1101 ..111111 ..111111] (U+DFFF)
 bit mask       [....xxxx ..x..... ........] (0F 20 00)
 bit pattern    [....1101 ..1..... ........] (0D 20 00)
 */
utf8_seq_def(b3_mask, F0, C0, C0, 00)
utf8_seq_def(b3_patt, E0, 80, 80, 00)
utf8_seq_def(b3_requ, 0F, 20, 00, 00)
utf8_seq_def(b3_erro, 0D, 20, 00, 00)
#define is_utf8_seq3(uni) ( \
    ((uni & utf8_seq(b3_mask)) == utf8_seq(b3_patt)) && \
    ((tmp = (uni & utf8_seq(b3_requ)))) && \
    ((tmp != utf8_seq(b3_erro))) )

/*
 4-byte sequence (U+10000 to U+10FFFF)
 bit min        [........ ...10000 ..000000 ..000000] (U+10000)
 bit max        [.....100 ..001111 ..111111 ..111111] (U+10FFFF)
 bit mask       [xxxxx... xx...... xx...... xx......] (F8 C0 C0 C0)
 bit pattern    [11110... 10...... 10...... 10......] (F0 80 80 80)
 bit require    [.....xxx ..xx.... ........ ........] (07 30 00 00)
 bit require 1  [.....x.. ........ ........ ........] (04 00 00 00)
 bit require 2  [......xx ..xx.... ........ ........] (03 30 00 00)
 */
utf8_seq_def(b4_mask, F8, C0, C0, C0)
utf8_seq_def(b4_patt, F0, 80, 80, 80)
utf8_seq_def(b4_requ, 07, 30, 00, 00)
utf8_seq_def(b4_req1, 04, 00, 00, 00)
utf8_seq_def(b4_req2, 03, 30, 00, 00)
#define is_utf8_seq4(uni) ( \
    ((uni & utf8_seq(b4_mask)) == utf8_seq(b4_patt)) && \
    ((tmp = (uni & utf8_seq(b4_requ)))) && \
    ((tmp & utf8_seq(b4_req1)) == 0 || (tmp & utf8_seq(b4_req2)) == 0) )



/*==============================================================================
 * MARK: - Power10 Lookup Table (Private)
 * These data are used by the floating-point number reader and writer.
 *============================================================================*/

#if !YYJSON_DISABLE_FAST_FP_CONV

/** Maximum pow10 exponent that can be represented exactly as a float64. */
#define F64_POW10_MAX_EXACT_EXP 22

#if YYJSON_DOUBLE_MATH_CORRECT
/** Cached pow10 table. */
static const f64 f64_pow10_table[F64_POW10_MAX_EXACT_EXP + 1] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12,
    1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22
};
#endif

/** Maximum pow10 exponent that can be represented exactly as a uint64. */
#define U64_POW10_MAX_EXACT_EXP 19

/** Table: [ 10^0, ..., 10^19 ] (generate with misc/make_tables.c) */
static const u64 u64_pow10_table[U64_POW10_MAX_EXACT_EXP + 1] = {
    U64(0x00000000, 0x00000001), U64(0x00000000, 0x0000000A),
    U64(0x00000000, 0x00000064), U64(0x00000000, 0x000003E8),
    U64(0x00000000, 0x00002710), U64(0x00000000, 0x000186A0),
    U64(0x00000000, 0x000F4240), U64(0x00000000, 0x00989680),
    U64(0x00000000, 0x05F5E100), U64(0x00000000, 0x3B9ACA00),
    U64(0x00000002, 0x540BE400), U64(0x00000017, 0x4876E800),
    U64(0x000000E8, 0xD4A51000), U64(0x00000918, 0x4E72A000),
    U64(0x00005AF3, 0x107A4000), U64(0x00038D7E, 0xA4C68000),
    U64(0x002386F2, 0x6FC10000), U64(0x01634578, 0x5D8A0000),
    U64(0x0DE0B6B3, 0xA7640000), U64(0x8AC72304, 0x89E80000)
};

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
    U64(0xBF29DCAB, 0xA82FDEAE), U64(0x7432EE87, 0x3880FC33), /* ~= 10^-343 */
    U64(0xEEF453D6, 0x923BD65A), U64(0x113FAA29, 0x06A13B3F), /* ~= 10^-342 */
    U64(0x9558B466, 0x1B6565F8), U64(0x4AC7CA59, 0xA424C507), /* ~= 10^-341 */
    U64(0xBAAEE17F, 0xA23EBF76), U64(0x5D79BCF0, 0x0D2DF649), /* ~= 10^-340 */
    U64(0xE95A99DF, 0x8ACE6F53), U64(0xF4D82C2C, 0x107973DC), /* ~= 10^-339 */
    U64(0x91D8A02B, 0xB6C10594), U64(0x79071B9B, 0x8A4BE869), /* ~= 10^-338 */
    U64(0xB64EC836, 0xA47146F9), U64(0x9748E282, 0x6CDEE284), /* ~= 10^-337 */
    U64(0xE3E27A44, 0x4D8D98B7), U64(0xFD1B1B23, 0x08169B25), /* ~= 10^-336 */
    U64(0x8E6D8C6A, 0xB0787F72), U64(0xFE30F0F5, 0xE50E20F7), /* ~= 10^-335 */
    U64(0xB208EF85, 0x5C969F4F), U64(0xBDBD2D33, 0x5E51A935), /* ~= 10^-334 */
    U64(0xDE8B2B66, 0xB3BC4723), U64(0xAD2C7880, 0x35E61382), /* ~= 10^-333 */
    U64(0x8B16FB20, 0x3055AC76), U64(0x4C3BCB50, 0x21AFCC31), /* ~= 10^-332 */
    U64(0xADDCB9E8, 0x3C6B1793), U64(0xDF4ABE24, 0x2A1BBF3D), /* ~= 10^-331 */
    U64(0xD953E862, 0x4B85DD78), U64(0xD71D6DAD, 0x34A2AF0D), /* ~= 10^-330 */
    U64(0x87D4713D, 0x6F33AA6B), U64(0x8672648C, 0x40E5AD68), /* ~= 10^-329 */
    U64(0xA9C98D8C, 0xCB009506), U64(0x680EFDAF, 0x511F18C2), /* ~= 10^-328 */
    U64(0xD43BF0EF, 0xFDC0BA48), U64(0x0212BD1B, 0x2566DEF2), /* ~= 10^-327 */
    U64(0x84A57695, 0xFE98746D), U64(0x014BB630, 0xF7604B57), /* ~= 10^-326 */
    U64(0xA5CED43B, 0x7E3E9188), U64(0x419EA3BD, 0x35385E2D), /* ~= 10^-325 */
    U64(0xCF42894A, 0x5DCE35EA), U64(0x52064CAC, 0x828675B9), /* ~= 10^-324 */
    U64(0x818995CE, 0x7AA0E1B2), U64(0x7343EFEB, 0xD1940993), /* ~= 10^-323 */
    U64(0xA1EBFB42, 0x19491A1F), U64(0x1014EBE6, 0xC5F90BF8), /* ~= 10^-322 */
    U64(0xCA66FA12, 0x9F9B60A6), U64(0xD41A26E0, 0x77774EF6), /* ~= 10^-321 */
    U64(0xFD00B897, 0x478238D0), U64(0x8920B098, 0x955522B4), /* ~= 10^-320 */
    U64(0x9E20735E, 0x8CB16382), U64(0x55B46E5F, 0x5D5535B0), /* ~= 10^-319 */
    U64(0xC5A89036, 0x2FDDBC62), U64(0xEB2189F7, 0x34AA831D), /* ~= 10^-318 */
    U64(0xF712B443, 0xBBD52B7B), U64(0xA5E9EC75, 0x01D523E4), /* ~= 10^-317 */
    U64(0x9A6BB0AA, 0x55653B2D), U64(0x47B233C9, 0x2125366E), /* ~= 10^-316 */
    U64(0xC1069CD4, 0xEABE89F8), U64(0x999EC0BB, 0x696E840A), /* ~= 10^-315 */
    U64(0xF148440A, 0x256E2C76), U64(0xC00670EA, 0x43CA250D), /* ~= 10^-314 */
    U64(0x96CD2A86, 0x5764DBCA), U64(0x38040692, 0x6A5E5728), /* ~= 10^-313 */
    U64(0xBC807527, 0xED3E12BC), U64(0xC6050837, 0x04F5ECF2), /* ~= 10^-312 */
    U64(0xEBA09271, 0xE88D976B), U64(0xF7864A44, 0xC633682E), /* ~= 10^-311 */
    U64(0x93445B87, 0x31587EA3), U64(0x7AB3EE6A, 0xFBE0211D), /* ~= 10^-310 */
    U64(0xB8157268, 0xFDAE9E4C), U64(0x5960EA05, 0xBAD82964), /* ~= 10^-309 */
    U64(0xE61ACF03, 0x3D1A45DF), U64(0x6FB92487, 0x298E33BD), /* ~= 10^-308 */
    U64(0x8FD0C162, 0x06306BAB), U64(0xA5D3B6D4, 0x79F8E056), /* ~= 10^-307 */
    U64(0xB3C4F1BA, 0x87BC8696), U64(0x8F48A489, 0x9877186C), /* ~= 10^-306 */
    U64(0xE0B62E29, 0x29ABA83C), U64(0x331ACDAB, 0xFE94DE87), /* ~= 10^-305 */
    U64(0x8C71DCD9, 0xBA0B4925), U64(0x9FF0C08B, 0x7F1D0B14), /* ~= 10^-304 */
    U64(0xAF8E5410, 0x288E1B6F), U64(0x07ECF0AE, 0x5EE44DD9), /* ~= 10^-303 */
    U64(0xDB71E914, 0x32B1A24A), U64(0xC9E82CD9, 0xF69D6150), /* ~= 10^-302 */
    U64(0x892731AC, 0x9FAF056E), U64(0xBE311C08, 0x3A225CD2), /* ~= 10^-301 */
    U64(0xAB70FE17, 0xC79AC6CA), U64(0x6DBD630A, 0x48AAF406), /* ~= 10^-300 */
    U64(0xD64D3D9D, 0xB981787D), U64(0x092CBBCC, 0xDAD5B108), /* ~= 10^-299 */
    U64(0x85F04682, 0x93F0EB4E), U64(0x25BBF560, 0x08C58EA5), /* ~= 10^-298 */
    U64(0xA76C5823, 0x38ED2621), U64(0xAF2AF2B8, 0x0AF6F24E), /* ~= 10^-297 */
    U64(0xD1476E2C, 0x07286FAA), U64(0x1AF5AF66, 0x0DB4AEE1), /* ~= 10^-296 */
    U64(0x82CCA4DB, 0x847945CA), U64(0x50D98D9F, 0xC890ED4D), /* ~= 10^-295 */
    U64(0xA37FCE12, 0x6597973C), U64(0xE50FF107, 0xBAB528A0), /* ~= 10^-294 */
    U64(0xCC5FC196, 0xFEFD7D0C), U64(0x1E53ED49, 0xA96272C8), /* ~= 10^-293 */
    U64(0xFF77B1FC, 0xBEBCDC4F), U64(0x25E8E89C, 0x13BB0F7A), /* ~= 10^-292 */
    U64(0x9FAACF3D, 0xF73609B1), U64(0x77B19161, 0x8C54E9AC), /* ~= 10^-291 */
    U64(0xC795830D, 0x75038C1D), U64(0xD59DF5B9, 0xEF6A2417), /* ~= 10^-290 */
    U64(0xF97AE3D0, 0xD2446F25), U64(0x4B057328, 0x6B44AD1D), /* ~= 10^-289 */
    U64(0x9BECCE62, 0x836AC577), U64(0x4EE367F9, 0x430AEC32), /* ~= 10^-288 */
    U64(0xC2E801FB, 0x244576D5), U64(0x229C41F7, 0x93CDA73F), /* ~= 10^-287 */
    U64(0xF3A20279, 0xED56D48A), U64(0x6B435275, 0x78C1110F), /* ~= 10^-286 */
    U64(0x9845418C, 0x345644D6), U64(0x830A1389, 0x6B78AAA9), /* ~= 10^-285 */
    U64(0xBE5691EF, 0x416BD60C), U64(0x23CC986B, 0xC656D553), /* ~= 10^-284 */
    U64(0xEDEC366B, 0x11C6CB8F), U64(0x2CBFBE86, 0xB7EC8AA8), /* ~= 10^-283 */
    U64(0x94B3A202, 0xEB1C3F39), U64(0x7BF7D714, 0x32F3D6A9), /* ~= 10^-282 */
    U64(0xB9E08A83, 0xA5E34F07), U64(0xDAF5CCD9, 0x3FB0CC53), /* ~= 10^-281 */
    U64(0xE858AD24, 0x8F5C22C9), U64(0xD1B3400F, 0x8F9CFF68), /* ~= 10^-280 */
    U64(0x91376C36, 0xD99995BE), U64(0x23100809, 0xB9C21FA1), /* ~= 10^-279 */
    U64(0xB5854744, 0x8FFFFB2D), U64(0xABD40A0C, 0x2832A78A), /* ~= 10^-278 */
    U64(0xE2E69915, 0xB3FFF9F9), U64(0x16C90C8F, 0x323F516C), /* ~= 10^-277 */
    U64(0x8DD01FAD, 0x907FFC3B), U64(0xAE3DA7D9, 0x7F6792E3), /* ~= 10^-276 */
    U64(0xB1442798, 0xF49FFB4A), U64(0x99CD11CF, 0xDF41779C), /* ~= 10^-275 */
    U64(0xDD95317F, 0x31C7FA1D), U64(0x40405643, 0xD711D583), /* ~= 10^-274 */
    U64(0x8A7D3EEF, 0x7F1CFC52), U64(0x482835EA, 0x666B2572), /* ~= 10^-273 */
    U64(0xAD1C8EAB, 0x5EE43B66), U64(0xDA324365, 0x0005EECF), /* ~= 10^-272 */
    U64(0xD863B256, 0x369D4A40), U64(0x90BED43E, 0x40076A82), /* ~= 10^-271 */
    U64(0x873E4F75, 0xE2224E68), U64(0x5A7744A6, 0xE804A291), /* ~= 10^-270 */
    U64(0xA90DE353, 0x5AAAE202), U64(0x711515D0, 0xA205CB36), /* ~= 10^-269 */
    U64(0xD3515C28, 0x31559A83), U64(0x0D5A5B44, 0xCA873E03), /* ~= 10^-268 */
    U64(0x8412D999, 0x1ED58091), U64(0xE858790A, 0xFE9486C2), /* ~= 10^-267 */
    U64(0xA5178FFF, 0x668AE0B6), U64(0x626E974D, 0xBE39A872), /* ~= 10^-266 */
    U64(0xCE5D73FF, 0x402D98E3), U64(0xFB0A3D21, 0x2DC8128F), /* ~= 10^-265 */
    U64(0x80FA687F, 0x881C7F8E), U64(0x7CE66634, 0xBC9D0B99), /* ~= 10^-264 */
    U64(0xA139029F, 0x6A239F72), U64(0x1C1FFFC1, 0xEBC44E80), /* ~= 10^-263 */
    U64(0xC9874347, 0x44AC874E), U64(0xA327FFB2, 0x66B56220), /* ~= 10^-262 */
    U64(0xFBE91419, 0x15D7A922), U64(0x4BF1FF9F, 0x0062BAA8), /* ~= 10^-261 */
    U64(0x9D71AC8F, 0xADA6C9B5), U64(0x6F773FC3, 0x603DB4A9), /* ~= 10^-260 */
    U64(0xC4CE17B3, 0x99107C22), U64(0xCB550FB4, 0x384D21D3), /* ~= 10^-259 */
    U64(0xF6019DA0, 0x7F549B2B), U64(0x7E2A53A1, 0x46606A48), /* ~= 10^-258 */
    U64(0x99C10284, 0x4F94E0FB), U64(0x2EDA7444, 0xCBFC426D), /* ~= 10^-257 */
    U64(0xC0314325, 0x637A1939), U64(0xFA911155, 0xFEFB5308), /* ~= 10^-256 */
    U64(0xF03D93EE, 0xBC589F88), U64(0x793555AB, 0x7EBA27CA), /* ~= 10^-255 */
    U64(0x96267C75, 0x35B763B5), U64(0x4BC1558B, 0x2F3458DE), /* ~= 10^-254 */
    U64(0xBBB01B92, 0x83253CA2), U64(0x9EB1AAED, 0xFB016F16), /* ~= 10^-253 */
    U64(0xEA9C2277, 0x23EE8BCB), U64(0x465E15A9, 0x79C1CADC), /* ~= 10^-252 */
    U64(0x92A1958A, 0x7675175F), U64(0x0BFACD89, 0xEC191EC9), /* ~= 10^-251 */
    U64(0xB749FAED, 0x14125D36), U64(0xCEF980EC, 0x671F667B), /* ~= 10^-250 */
    U64(0xE51C79A8, 0x5916F484), U64(0x82B7E127, 0x80E7401A), /* ~= 10^-249 */
    U64(0x8F31CC09, 0x37AE58D2), U64(0xD1B2ECB8, 0xB0908810), /* ~= 10^-248 */
    U64(0xB2FE3F0B, 0x8599EF07), U64(0x861FA7E6, 0xDCB4AA15), /* ~= 10^-247 */
    U64(0xDFBDCECE, 0x67006AC9), U64(0x67A791E0, 0x93E1D49A), /* ~= 10^-246 */
    U64(0x8BD6A141, 0x006042BD), U64(0xE0C8BB2C, 0x5C6D24E0), /* ~= 10^-245 */
    U64(0xAECC4991, 0x4078536D), U64(0x58FAE9F7, 0x73886E18), /* ~= 10^-244 */
    U64(0xDA7F5BF5, 0x90966848), U64(0xAF39A475, 0x506A899E), /* ~= 10^-243 */
    U64(0x888F9979, 0x7A5E012D), U64(0x6D8406C9, 0x52429603), /* ~= 10^-242 */
    U64(0xAAB37FD7, 0xD8F58178), U64(0xC8E5087B, 0xA6D33B83), /* ~= 10^-241 */
    U64(0xD5605FCD, 0xCF32E1D6), U64(0xFB1E4A9A, 0x90880A64), /* ~= 10^-240 */
    U64(0x855C3BE0, 0xA17FCD26), U64(0x5CF2EEA0, 0x9A55067F), /* ~= 10^-239 */
    U64(0xA6B34AD8, 0xC9DFC06F), U64(0xF42FAA48, 0xC0EA481E), /* ~= 10^-238 */
    U64(0xD0601D8E, 0xFC57B08B), U64(0xF13B94DA, 0xF124DA26), /* ~= 10^-237 */
    U64(0x823C1279, 0x5DB6CE57), U64(0x76C53D08, 0xD6B70858), /* ~= 10^-236 */
    U64(0xA2CB1717, 0xB52481ED), U64(0x54768C4B, 0x0C64CA6E), /* ~= 10^-235 */
    U64(0xCB7DDCDD, 0xA26DA268), U64(0xA9942F5D, 0xCF7DFD09), /* ~= 10^-234 */
    U64(0xFE5D5415, 0x0B090B02), U64(0xD3F93B35, 0x435D7C4C), /* ~= 10^-233 */
    U64(0x9EFA548D, 0x26E5A6E1), U64(0xC47BC501, 0x4A1A6DAF), /* ~= 10^-232 */
    U64(0xC6B8E9B0, 0x709F109A), U64(0x359AB641, 0x9CA1091B), /* ~= 10^-231 */
    U64(0xF867241C, 0x8CC6D4C0), U64(0xC30163D2, 0x03C94B62), /* ~= 10^-230 */
    U64(0x9B407691, 0xD7FC44F8), U64(0x79E0DE63, 0x425DCF1D), /* ~= 10^-229 */
    U64(0xC2109436, 0x4DFB5636), U64(0x985915FC, 0x12F542E4), /* ~= 10^-228 */
    U64(0xF294B943, 0xE17A2BC4), U64(0x3E6F5B7B, 0x17B2939D), /* ~= 10^-227 */
    U64(0x979CF3CA, 0x6CEC5B5A), U64(0xA705992C, 0xEECF9C42), /* ~= 10^-226 */
    U64(0xBD8430BD, 0x08277231), U64(0x50C6FF78, 0x2A838353), /* ~= 10^-225 */
    U64(0xECE53CEC, 0x4A314EBD), U64(0xA4F8BF56, 0x35246428), /* ~= 10^-224 */
    U64(0x940F4613, 0xAE5ED136), U64(0x871B7795, 0xE136BE99), /* ~= 10^-223 */
    U64(0xB9131798, 0x99F68584), U64(0x28E2557B, 0x59846E3F), /* ~= 10^-222 */
    U64(0xE757DD7E, 0xC07426E5), U64(0x331AEADA, 0x2FE589CF), /* ~= 10^-221 */
    U64(0x9096EA6F, 0x3848984F), U64(0x3FF0D2C8, 0x5DEF7621), /* ~= 10^-220 */
    U64(0xB4BCA50B, 0x065ABE63), U64(0x0FED077A, 0x756B53A9), /* ~= 10^-219 */
    U64(0xE1EBCE4D, 0xC7F16DFB), U64(0xD3E84959, 0x12C62894), /* ~= 10^-218 */
    U64(0x8D3360F0, 0x9CF6E4BD), U64(0x64712DD7, 0xABBBD95C), /* ~= 10^-217 */
    U64(0xB080392C, 0xC4349DEC), U64(0xBD8D794D, 0x96AACFB3), /* ~= 10^-216 */
    U64(0xDCA04777, 0xF541C567), U64(0xECF0D7A0, 0xFC5583A0), /* ~= 10^-215 */
    U64(0x89E42CAA, 0xF9491B60), U64(0xF41686C4, 0x9DB57244), /* ~= 10^-214 */
    U64(0xAC5D37D5, 0xB79B6239), U64(0x311C2875, 0xC522CED5), /* ~= 10^-213 */
    U64(0xD77485CB, 0x25823AC7), U64(0x7D633293, 0x366B828B), /* ~= 10^-212 */
    U64(0x86A8D39E, 0xF77164BC), U64(0xAE5DFF9C, 0x02033197), /* ~= 10^-211 */
    U64(0xA8530886, 0xB54DBDEB), U64(0xD9F57F83, 0x0283FDFC), /* ~= 10^-210 */
    U64(0xD267CAA8, 0x62A12D66), U64(0xD072DF63, 0xC324FD7B), /* ~= 10^-209 */
    U64(0x8380DEA9, 0x3DA4BC60), U64(0x4247CB9E, 0x59F71E6D), /* ~= 10^-208 */
    U64(0xA4611653, 0x8D0DEB78), U64(0x52D9BE85, 0xF074E608), /* ~= 10^-207 */
    U64(0xCD795BE8, 0x70516656), U64(0x67902E27, 0x6C921F8B), /* ~= 10^-206 */
    U64(0x806BD971, 0x4632DFF6), U64(0x00BA1CD8, 0xA3DB53B6), /* ~= 10^-205 */
    U64(0xA086CFCD, 0x97BF97F3), U64(0x80E8A40E, 0xCCD228A4), /* ~= 10^-204 */
    U64(0xC8A883C0, 0xFDAF7DF0), U64(0x6122CD12, 0x8006B2CD), /* ~= 10^-203 */
    U64(0xFAD2A4B1, 0x3D1B5D6C), U64(0x796B8057, 0x20085F81), /* ~= 10^-202 */
    U64(0x9CC3A6EE, 0xC6311A63), U64(0xCBE33036, 0x74053BB0), /* ~= 10^-201 */
    U64(0xC3F490AA, 0x77BD60FC), U64(0xBEDBFC44, 0x11068A9C), /* ~= 10^-200 */
    U64(0xF4F1B4D5, 0x15ACB93B), U64(0xEE92FB55, 0x15482D44), /* ~= 10^-199 */
    U64(0x99171105, 0x2D8BF3C5), U64(0x751BDD15, 0x2D4D1C4A), /* ~= 10^-198 */
    U64(0xBF5CD546, 0x78EEF0B6), U64(0xD262D45A, 0x78A0635D), /* ~= 10^-197 */
    U64(0xEF340A98, 0x172AACE4), U64(0x86FB8971, 0x16C87C34), /* ~= 10^-196 */
    U64(0x9580869F, 0x0E7AAC0E), U64(0xD45D35E6, 0xAE3D4DA0), /* ~= 10^-195 */
    U64(0xBAE0A846, 0xD2195712), U64(0x89748360, 0x59CCA109), /* ~= 10^-194 */
    U64(0xE998D258, 0x869FACD7), U64(0x2BD1A438, 0x703FC94B), /* ~= 10^-193 */
    U64(0x91FF8377, 0x5423CC06), U64(0x7B6306A3, 0x4627DDCF), /* ~= 10^-192 */
    U64(0xB67F6455, 0x292CBF08), U64(0x1A3BC84C, 0x17B1D542), /* ~= 10^-191 */
    U64(0xE41F3D6A, 0x7377EECA), U64(0x20CABA5F, 0x1D9E4A93), /* ~= 10^-190 */
    U64(0x8E938662, 0x882AF53E), U64(0x547EB47B, 0x7282EE9C), /* ~= 10^-189 */
    U64(0xB23867FB, 0x2A35B28D), U64(0xE99E619A, 0x4F23AA43), /* ~= 10^-188 */
    U64(0xDEC681F9, 0xF4C31F31), U64(0x6405FA00, 0xE2EC94D4), /* ~= 10^-187 */
    U64(0x8B3C113C, 0x38F9F37E), U64(0xDE83BC40, 0x8DD3DD04), /* ~= 10^-186 */
    U64(0xAE0B158B, 0x4738705E), U64(0x9624AB50, 0xB148D445), /* ~= 10^-185 */
    U64(0xD98DDAEE, 0x19068C76), U64(0x3BADD624, 0xDD9B0957), /* ~= 10^-184 */
    U64(0x87F8A8D4, 0xCFA417C9), U64(0xE54CA5D7, 0x0A80E5D6), /* ~= 10^-183 */
    U64(0xA9F6D30A, 0x038D1DBC), U64(0x5E9FCF4C, 0xCD211F4C), /* ~= 10^-182 */
    U64(0xD47487CC, 0x8470652B), U64(0x7647C320, 0x0069671F), /* ~= 10^-181 */
    U64(0x84C8D4DF, 0xD2C63F3B), U64(0x29ECD9F4, 0x0041E073), /* ~= 10^-180 */
    U64(0xA5FB0A17, 0xC777CF09), U64(0xF4681071, 0x00525890), /* ~= 10^-179 */
    U64(0xCF79CC9D, 0xB955C2CC), U64(0x7182148D, 0x4066EEB4), /* ~= 10^-178 */
    U64(0x81AC1FE2, 0x93D599BF), U64(0xC6F14CD8, 0x48405530), /* ~= 10^-177 */
    U64(0xA21727DB, 0x38CB002F), U64(0xB8ADA00E, 0x5A506A7C), /* ~= 10^-176 */
    U64(0xCA9CF1D2, 0x06FDC03B), U64(0xA6D90811, 0xF0E4851C), /* ~= 10^-175 */
    U64(0xFD442E46, 0x88BD304A), U64(0x908F4A16, 0x6D1DA663), /* ~= 10^-174 */
    U64(0x9E4A9CEC, 0x15763E2E), U64(0x9A598E4E, 0x043287FE), /* ~= 10^-173 */
    U64(0xC5DD4427, 0x1AD3CDBA), U64(0x40EFF1E1, 0x853F29FD), /* ~= 10^-172 */
    U64(0xF7549530, 0xE188C128), U64(0xD12BEE59, 0xE68EF47C), /* ~= 10^-171 */
    U64(0x9A94DD3E, 0x8CF578B9), U64(0x82BB74F8, 0x301958CE), /* ~= 10^-170 */
    U64(0xC13A148E, 0x3032D6E7), U64(0xE36A5236, 0x3C1FAF01), /* ~= 10^-169 */
    U64(0xF18899B1, 0xBC3F8CA1), U64(0xDC44E6C3, 0xCB279AC1), /* ~= 10^-168 */
    U64(0x96F5600F, 0x15A7B7E5), U64(0x29AB103A, 0x5EF8C0B9), /* ~= 10^-167 */
    U64(0xBCB2B812, 0xDB11A5DE), U64(0x7415D448, 0xF6B6F0E7), /* ~= 10^-166 */
    U64(0xEBDF6617, 0x91D60F56), U64(0x111B495B, 0x3464AD21), /* ~= 10^-165 */
    U64(0x936B9FCE, 0xBB25C995), U64(0xCAB10DD9, 0x00BEEC34), /* ~= 10^-164 */
    U64(0xB84687C2, 0x69EF3BFB), U64(0x3D5D514F, 0x40EEA742), /* ~= 10^-163 */
    U64(0xE65829B3, 0x046B0AFA), U64(0x0CB4A5A3, 0x112A5112), /* ~= 10^-162 */
    U64(0x8FF71A0F, 0xE2C2E6DC), U64(0x47F0E785, 0xEABA72AB), /* ~= 10^-161 */
    U64(0xB3F4E093, 0xDB73A093), U64(0x59ED2167, 0x65690F56), /* ~= 10^-160 */
    U64(0xE0F218B8, 0xD25088B8), U64(0x306869C1, 0x3EC3532C), /* ~= 10^-159 */
    U64(0x8C974F73, 0x83725573), U64(0x1E414218, 0xC73A13FB), /* ~= 10^-158 */
    U64(0xAFBD2350, 0x644EEACF), U64(0xE5D1929E, 0xF90898FA), /* ~= 10^-157 */
    U64(0xDBAC6C24, 0x7D62A583), U64(0xDF45F746, 0xB74ABF39), /* ~= 10^-156 */
    U64(0x894BC396, 0xCE5DA772), U64(0x6B8BBA8C, 0x328EB783), /* ~= 10^-155 */
    U64(0xAB9EB47C, 0x81F5114F), U64(0x066EA92F, 0x3F326564), /* ~= 10^-154 */
    U64(0xD686619B, 0xA27255A2), U64(0xC80A537B, 0x0EFEFEBD), /* ~= 10^-153 */
    U64(0x8613FD01, 0x45877585), U64(0xBD06742C, 0xE95F5F36), /* ~= 10^-152 */
    U64(0xA798FC41, 0x96E952E7), U64(0x2C481138, 0x23B73704), /* ~= 10^-151 */
    U64(0xD17F3B51, 0xFCA3A7A0), U64(0xF75A1586, 0x2CA504C5), /* ~= 10^-150 */
    U64(0x82EF8513, 0x3DE648C4), U64(0x9A984D73, 0xDBE722FB), /* ~= 10^-149 */
    U64(0xA3AB6658, 0x0D5FDAF5), U64(0xC13E60D0, 0xD2E0EBBA), /* ~= 10^-148 */
    U64(0xCC963FEE, 0x10B7D1B3), U64(0x318DF905, 0x079926A8), /* ~= 10^-147 */
    U64(0xFFBBCFE9, 0x94E5C61F), U64(0xFDF17746, 0x497F7052), /* ~= 10^-146 */
    U64(0x9FD561F1, 0xFD0F9BD3), U64(0xFEB6EA8B, 0xEDEFA633), /* ~= 10^-145 */
    U64(0xC7CABA6E, 0x7C5382C8), U64(0xFE64A52E, 0xE96B8FC0), /* ~= 10^-144 */
    U64(0xF9BD690A, 0x1B68637B), U64(0x3DFDCE7A, 0xA3C673B0), /* ~= 10^-143 */
    U64(0x9C1661A6, 0x51213E2D), U64(0x06BEA10C, 0xA65C084E), /* ~= 10^-142 */
    U64(0xC31BFA0F, 0xE5698DB8), U64(0x486E494F, 0xCFF30A62), /* ~= 10^-141 */
    U64(0xF3E2F893, 0xDEC3F126), U64(0x5A89DBA3, 0xC3EFCCFA), /* ~= 10^-140 */
    U64(0x986DDB5C, 0x6B3A76B7), U64(0xF8962946, 0x5A75E01C), /* ~= 10^-139 */
    U64(0xBE895233, 0x86091465), U64(0xF6BBB397, 0xF1135823), /* ~= 10^-138 */
    U64(0xEE2BA6C0, 0x678B597F), U64(0x746AA07D, 0xED582E2C), /* ~= 10^-137 */
    U64(0x94DB4838, 0x40B717EF), U64(0xA8C2A44E, 0xB4571CDC), /* ~= 10^-136 */
    U64(0xBA121A46, 0x50E4DDEB), U64(0x92F34D62, 0x616CE413), /* ~= 10^-135 */
    U64(0xE896A0D7, 0xE51E1566), U64(0x77B020BA, 0xF9C81D17), /* ~= 10^-134 */
    U64(0x915E2486, 0xEF32CD60), U64(0x0ACE1474, 0xDC1D122E), /* ~= 10^-133 */
    U64(0xB5B5ADA8, 0xAAFF80B8), U64(0x0D819992, 0x132456BA), /* ~= 10^-132 */
    U64(0xE3231912, 0xD5BF60E6), U64(0x10E1FFF6, 0x97ED6C69), /* ~= 10^-131 */
    U64(0x8DF5EFAB, 0xC5979C8F), U64(0xCA8D3FFA, 0x1EF463C1), /* ~= 10^-130 */
    U64(0xB1736B96, 0xB6FD83B3), U64(0xBD308FF8, 0xA6B17CB2), /* ~= 10^-129 */
    U64(0xDDD0467C, 0x64BCE4A0), U64(0xAC7CB3F6, 0xD05DDBDE), /* ~= 10^-128 */
    U64(0x8AA22C0D, 0xBEF60EE4), U64(0x6BCDF07A, 0x423AA96B), /* ~= 10^-127 */
    U64(0xAD4AB711, 0x2EB3929D), U64(0x86C16C98, 0xD2C953C6), /* ~= 10^-126 */
    U64(0xD89D64D5, 0x7A607744), U64(0xE871C7BF, 0x077BA8B7), /* ~= 10^-125 */
    U64(0x87625F05, 0x6C7C4A8B), U64(0x11471CD7, 0x64AD4972), /* ~= 10^-124 */
    U64(0xA93AF6C6, 0xC79B5D2D), U64(0xD598E40D, 0x3DD89BCF), /* ~= 10^-123 */
    U64(0xD389B478, 0x79823479), U64(0x4AFF1D10, 0x8D4EC2C3), /* ~= 10^-122 */
    U64(0x843610CB, 0x4BF160CB), U64(0xCEDF722A, 0x585139BA), /* ~= 10^-121 */
    U64(0xA54394FE, 0x1EEDB8FE), U64(0xC2974EB4, 0xEE658828), /* ~= 10^-120 */
    U64(0xCE947A3D, 0xA6A9273E), U64(0x733D2262, 0x29FEEA32), /* ~= 10^-119 */
    U64(0x811CCC66, 0x8829B887), U64(0x0806357D, 0x5A3F525F), /* ~= 10^-118 */
    U64(0xA163FF80, 0x2A3426A8), U64(0xCA07C2DC, 0xB0CF26F7), /* ~= 10^-117 */
    U64(0xC9BCFF60, 0x34C13052), U64(0xFC89B393, 0xDD02F0B5), /* ~= 10^-116 */
    U64(0xFC2C3F38, 0x41F17C67), U64(0xBBAC2078, 0xD443ACE2), /* ~= 10^-115 */
    U64(0x9D9BA783, 0x2936EDC0), U64(0xD54B944B, 0x84AA4C0D), /* ~= 10^-114 */
    U64(0xC5029163, 0xF384A931), U64(0x0A9E795E, 0x65D4DF11), /* ~= 10^-113 */
    U64(0xF64335BC, 0xF065D37D), U64(0x4D4617B5, 0xFF4A16D5), /* ~= 10^-112 */
    U64(0x99EA0196, 0x163FA42E), U64(0x504BCED1, 0xBF8E4E45), /* ~= 10^-111 */
    U64(0xC06481FB, 0x9BCF8D39), U64(0xE45EC286, 0x2F71E1D6), /* ~= 10^-110 */
    U64(0xF07DA27A, 0x82C37088), U64(0x5D767327, 0xBB4E5A4C), /* ~= 10^-109 */
    U64(0x964E858C, 0x91BA2655), U64(0x3A6A07F8, 0xD510F86F), /* ~= 10^-108 */
    U64(0xBBE226EF, 0xB628AFEA), U64(0x890489F7, 0x0A55368B), /* ~= 10^-107 */
    U64(0xEADAB0AB, 0xA3B2DBE5), U64(0x2B45AC74, 0xCCEA842E), /* ~= 10^-106 */
    U64(0x92C8AE6B, 0x464FC96F), U64(0x3B0B8BC9, 0x0012929D), /* ~= 10^-105 */
    U64(0xB77ADA06, 0x17E3BBCB), U64(0x09CE6EBB, 0x40173744), /* ~= 10^-104 */
    U64(0xE5599087, 0x9DDCAABD), U64(0xCC420A6A, 0x101D0515), /* ~= 10^-103 */
    U64(0x8F57FA54, 0xC2A9EAB6), U64(0x9FA94682, 0x4A12232D), /* ~= 10^-102 */
    U64(0xB32DF8E9, 0xF3546564), U64(0x47939822, 0xDC96ABF9), /* ~= 10^-101 */
    U64(0xDFF97724, 0x70297EBD), U64(0x59787E2B, 0x93BC56F7), /* ~= 10^-100 */
    U64(0x8BFBEA76, 0xC619EF36), U64(0x57EB4EDB, 0x3C55B65A), /* ~= 10^-99 */
    U64(0xAEFAE514, 0x77A06B03), U64(0xEDE62292, 0x0B6B23F1), /* ~= 10^-98 */
    U64(0xDAB99E59, 0x958885C4), U64(0xE95FAB36, 0x8E45ECED), /* ~= 10^-97 */
    U64(0x88B402F7, 0xFD75539B), U64(0x11DBCB02, 0x18EBB414), /* ~= 10^-96 */
    U64(0xAAE103B5, 0xFCD2A881), U64(0xD652BDC2, 0x9F26A119), /* ~= 10^-95 */
    U64(0xD59944A3, 0x7C0752A2), U64(0x4BE76D33, 0x46F0495F), /* ~= 10^-94 */
    U64(0x857FCAE6, 0x2D8493A5), U64(0x6F70A440, 0x0C562DDB), /* ~= 10^-93 */
    U64(0xA6DFBD9F, 0xB8E5B88E), U64(0xCB4CCD50, 0x0F6BB952), /* ~= 10^-92 */
    U64(0xD097AD07, 0xA71F26B2), U64(0x7E2000A4, 0x1346A7A7), /* ~= 10^-91 */
    U64(0x825ECC24, 0xC873782F), U64(0x8ED40066, 0x8C0C28C8), /* ~= 10^-90 */
    U64(0xA2F67F2D, 0xFA90563B), U64(0x72890080, 0x2F0F32FA), /* ~= 10^-89 */
    U64(0xCBB41EF9, 0x79346BCA), U64(0x4F2B40A0, 0x3AD2FFB9), /* ~= 10^-88 */
    U64(0xFEA126B7, 0xD78186BC), U64(0xE2F610C8, 0x4987BFA8), /* ~= 10^-87 */
    U64(0x9F24B832, 0xE6B0F436), U64(0x0DD9CA7D, 0x2DF4D7C9), /* ~= 10^-86 */
    U64(0xC6EDE63F, 0xA05D3143), U64(0x91503D1C, 0x79720DBB), /* ~= 10^-85 */
    U64(0xF8A95FCF, 0x88747D94), U64(0x75A44C63, 0x97CE912A), /* ~= 10^-84 */
    U64(0x9B69DBE1, 0xB548CE7C), U64(0xC986AFBE, 0x3EE11ABA), /* ~= 10^-83 */
    U64(0xC24452DA, 0x229B021B), U64(0xFBE85BAD, 0xCE996168), /* ~= 10^-82 */
    U64(0xF2D56790, 0xAB41C2A2), U64(0xFAE27299, 0x423FB9C3), /* ~= 10^-81 */
    U64(0x97C560BA, 0x6B0919A5), U64(0xDCCD879F, 0xC967D41A), /* ~= 10^-80 */
    U64(0xBDB6B8E9, 0x05CB600F), U64(0x5400E987, 0xBBC1C920), /* ~= 10^-79 */
    U64(0xED246723, 0x473E3813), U64(0x290123E9, 0xAAB23B68), /* ~= 10^-78 */
    U64(0x9436C076, 0x0C86E30B), U64(0xF9A0B672, 0x0AAF6521), /* ~= 10^-77 */
    U64(0xB9447093, 0x8FA89BCE), U64(0xF808E40E, 0x8D5B3E69), /* ~= 10^-76 */
    U64(0xE7958CB8, 0x7392C2C2), U64(0xB60B1D12, 0x30B20E04), /* ~= 10^-75 */
    U64(0x90BD77F3, 0x483BB9B9), U64(0xB1C6F22B, 0x5E6F48C2), /* ~= 10^-74 */
    U64(0xB4ECD5F0, 0x1A4AA828), U64(0x1E38AEB6, 0x360B1AF3), /* ~= 10^-73 */
    U64(0xE2280B6C, 0x20DD5232), U64(0x25C6DA63, 0xC38DE1B0), /* ~= 10^-72 */
    U64(0x8D590723, 0x948A535F), U64(0x579C487E, 0x5A38AD0E), /* ~= 10^-71 */
    U64(0xB0AF48EC, 0x79ACE837), U64(0x2D835A9D, 0xF0C6D851), /* ~= 10^-70 */
    U64(0xDCDB1B27, 0x98182244), U64(0xF8E43145, 0x6CF88E65), /* ~= 10^-69 */
    U64(0x8A08F0F8, 0xBF0F156B), U64(0x1B8E9ECB, 0x641B58FF), /* ~= 10^-68 */
    U64(0xAC8B2D36, 0xEED2DAC5), U64(0xE272467E, 0x3D222F3F), /* ~= 10^-67 */
    U64(0xD7ADF884, 0xAA879177), U64(0x5B0ED81D, 0xCC6ABB0F), /* ~= 10^-66 */
    U64(0x86CCBB52, 0xEA94BAEA), U64(0x98E94712, 0x9FC2B4E9), /* ~= 10^-65 */
    U64(0xA87FEA27, 0xA539E9A5), U64(0x3F2398D7, 0x47B36224), /* ~= 10^-64 */
    U64(0xD29FE4B1, 0x8E88640E), U64(0x8EEC7F0D, 0x19A03AAD), /* ~= 10^-63 */
    U64(0x83A3EEEE, 0xF9153E89), U64(0x1953CF68, 0x300424AC), /* ~= 10^-62 */
    U64(0xA48CEAAA, 0xB75A8E2B), U64(0x5FA8C342, 0x3C052DD7), /* ~= 10^-61 */
    U64(0xCDB02555, 0x653131B6), U64(0x3792F412, 0xCB06794D), /* ~= 10^-60 */
    U64(0x808E1755, 0x5F3EBF11), U64(0xE2BBD88B, 0xBEE40BD0), /* ~= 10^-59 */
    U64(0xA0B19D2A, 0xB70E6ED6), U64(0x5B6ACEAE, 0xAE9D0EC4), /* ~= 10^-58 */
    U64(0xC8DE0475, 0x64D20A8B), U64(0xF245825A, 0x5A445275), /* ~= 10^-57 */
    U64(0xFB158592, 0xBE068D2E), U64(0xEED6E2F0, 0xF0D56712), /* ~= 10^-56 */
    U64(0x9CED737B, 0xB6C4183D), U64(0x55464DD6, 0x9685606B), /* ~= 10^-55 */
    U64(0xC428D05A, 0xA4751E4C), U64(0xAA97E14C, 0x3C26B886), /* ~= 10^-54 */
    U64(0xF5330471, 0x4D9265DF), U64(0xD53DD99F, 0x4B3066A8), /* ~= 10^-53 */
    U64(0x993FE2C6, 0xD07B7FAB), U64(0xE546A803, 0x8EFE4029), /* ~= 10^-52 */
    U64(0xBF8FDB78, 0x849A5F96), U64(0xDE985204, 0x72BDD033), /* ~= 10^-51 */
    U64(0xEF73D256, 0xA5C0F77C), U64(0x963E6685, 0x8F6D4440), /* ~= 10^-50 */
    U64(0x95A86376, 0x27989AAD), U64(0xDDE70013, 0x79A44AA8), /* ~= 10^-49 */
    U64(0xBB127C53, 0xB17EC159), U64(0x5560C018, 0x580D5D52), /* ~= 10^-48 */
    U64(0xE9D71B68, 0x9DDE71AF), U64(0xAAB8F01E, 0x6E10B4A6), /* ~= 10^-47 */
    U64(0x92267121, 0x62AB070D), U64(0xCAB39613, 0x04CA70E8), /* ~= 10^-46 */
    U64(0xB6B00D69, 0xBB55C8D1), U64(0x3D607B97, 0xC5FD0D22), /* ~= 10^-45 */
    U64(0xE45C10C4, 0x2A2B3B05), U64(0x8CB89A7D, 0xB77C506A), /* ~= 10^-44 */
    U64(0x8EB98A7A, 0x9A5B04E3), U64(0x77F3608E, 0x92ADB242), /* ~= 10^-43 */
    U64(0xB267ED19, 0x40F1C61C), U64(0x55F038B2, 0x37591ED3), /* ~= 10^-42 */
    U64(0xDF01E85F, 0x912E37A3), U64(0x6B6C46DE, 0xC52F6688), /* ~= 10^-41 */
    U64(0x8B61313B, 0xBABCE2C6), U64(0x2323AC4B, 0x3B3DA015), /* ~= 10^-40 */
    U64(0xAE397D8A, 0xA96C1B77), U64(0xABEC975E, 0x0A0D081A), /* ~= 10^-39 */
    U64(0xD9C7DCED, 0x53C72255), U64(0x96E7BD35, 0x8C904A21), /* ~= 10^-38 */
    U64(0x881CEA14, 0x545C7575), U64(0x7E50D641, 0x77DA2E54), /* ~= 10^-37 */
    U64(0xAA242499, 0x697392D2), U64(0xDDE50BD1, 0xD5D0B9E9), /* ~= 10^-36 */
    U64(0xD4AD2DBF, 0xC3D07787), U64(0x955E4EC6, 0x4B44E864), /* ~= 10^-35 */
    U64(0x84EC3C97, 0xDA624AB4), U64(0xBD5AF13B, 0xEF0B113E), /* ~= 10^-34 */
    U64(0xA6274BBD, 0xD0FADD61), U64(0xECB1AD8A, 0xEACDD58E), /* ~= 10^-33 */
    U64(0xCFB11EAD, 0x453994BA), U64(0x67DE18ED, 0xA5814AF2), /* ~= 10^-32 */
    U64(0x81CEB32C, 0x4B43FCF4), U64(0x80EACF94, 0x8770CED7), /* ~= 10^-31 */
    U64(0xA2425FF7, 0x5E14FC31), U64(0xA1258379, 0xA94D028D), /* ~= 10^-30 */
    U64(0xCAD2F7F5, 0x359A3B3E), U64(0x096EE458, 0x13A04330), /* ~= 10^-29 */
    U64(0xFD87B5F2, 0x8300CA0D), U64(0x8BCA9D6E, 0x188853FC), /* ~= 10^-28 */
    U64(0x9E74D1B7, 0x91E07E48), U64(0x775EA264, 0xCF55347D), /* ~= 10^-27 */
    U64(0xC6120625, 0x76589DDA), U64(0x95364AFE, 0x032A819D), /* ~= 10^-26 */
    U64(0xF79687AE, 0xD3EEC551), U64(0x3A83DDBD, 0x83F52204), /* ~= 10^-25 */
    U64(0x9ABE14CD, 0x44753B52), U64(0xC4926A96, 0x72793542), /* ~= 10^-24 */
    U64(0xC16D9A00, 0x95928A27), U64(0x75B7053C, 0x0F178293), /* ~= 10^-23 */
    U64(0xF1C90080, 0xBAF72CB1), U64(0x5324C68B, 0x12DD6338), /* ~= 10^-22 */
    U64(0x971DA050, 0x74DA7BEE), U64(0xD3F6FC16, 0xEBCA5E03), /* ~= 10^-21 */
    U64(0xBCE50864, 0x92111AEA), U64(0x88F4BB1C, 0xA6BCF584), /* ~= 10^-20 */
    U64(0xEC1E4A7D, 0xB69561A5), U64(0x2B31E9E3, 0xD06C32E5), /* ~= 10^-19 */
    U64(0x9392EE8E, 0x921D5D07), U64(0x3AFF322E, 0x62439FCF), /* ~= 10^-18 */
    U64(0xB877AA32, 0x36A4B449), U64(0x09BEFEB9, 0xFAD487C2), /* ~= 10^-17 */
    U64(0xE69594BE, 0xC44DE15B), U64(0x4C2EBE68, 0x7989A9B3), /* ~= 10^-16 */
    U64(0x901D7CF7, 0x3AB0ACD9), U64(0x0F9D3701, 0x4BF60A10), /* ~= 10^-15 */
    U64(0xB424DC35, 0x095CD80F), U64(0x538484C1, 0x9EF38C94), /* ~= 10^-14 */
    U64(0xE12E1342, 0x4BB40E13), U64(0x2865A5F2, 0x06B06FB9), /* ~= 10^-13 */
    U64(0x8CBCCC09, 0x6F5088CB), U64(0xF93F87B7, 0x442E45D3), /* ~= 10^-12 */
    U64(0xAFEBFF0B, 0xCB24AAFE), U64(0xF78F69A5, 0x1539D748), /* ~= 10^-11 */
    U64(0xDBE6FECE, 0xBDEDD5BE), U64(0xB573440E, 0x5A884D1B), /* ~= 10^-10 */
    U64(0x89705F41, 0x36B4A597), U64(0x31680A88, 0xF8953030), /* ~= 10^-9 */
    U64(0xABCC7711, 0x8461CEFC), U64(0xFDC20D2B, 0x36BA7C3D), /* ~= 10^-8 */
    U64(0xD6BF94D5, 0xE57A42BC), U64(0x3D329076, 0x04691B4C), /* ~= 10^-7 */
    U64(0x8637BD05, 0xAF6C69B5), U64(0xA63F9A49, 0xC2C1B10F), /* ~= 10^-6 */
    U64(0xA7C5AC47, 0x1B478423), U64(0x0FCF80DC, 0x33721D53), /* ~= 10^-5 */
    U64(0xD1B71758, 0xE219652B), U64(0xD3C36113, 0x404EA4A8), /* ~= 10^-4 */
    U64(0x83126E97, 0x8D4FDF3B), U64(0x645A1CAC, 0x083126E9), /* ~= 10^-3 */
    U64(0xA3D70A3D, 0x70A3D70A), U64(0x3D70A3D7, 0x0A3D70A3), /* ~= 10^-2 */
    U64(0xCCCCCCCC, 0xCCCCCCCC), U64(0xCCCCCCCC, 0xCCCCCCCC), /* ~= 10^-1 */
    U64(0x80000000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^0 */
    U64(0xA0000000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^1 */
    U64(0xC8000000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^2 */
    U64(0xFA000000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^3 */
    U64(0x9C400000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^4 */
    U64(0xC3500000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^5 */
    U64(0xF4240000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^6 */
    U64(0x98968000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^7 */
    U64(0xBEBC2000, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^8 */
    U64(0xEE6B2800, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^9 */
    U64(0x9502F900, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^10 */
    U64(0xBA43B740, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^11 */
    U64(0xE8D4A510, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^12 */
    U64(0x9184E72A, 0x00000000), U64(0x00000000, 0x00000000), /* == 10^13 */
    U64(0xB5E620F4, 0x80000000), U64(0x00000000, 0x00000000), /* == 10^14 */
    U64(0xE35FA931, 0xA0000000), U64(0x00000000, 0x00000000), /* == 10^15 */
    U64(0x8E1BC9BF, 0x04000000), U64(0x00000000, 0x00000000), /* == 10^16 */
    U64(0xB1A2BC2E, 0xC5000000), U64(0x00000000, 0x00000000), /* == 10^17 */
    U64(0xDE0B6B3A, 0x76400000), U64(0x00000000, 0x00000000), /* == 10^18 */
    U64(0x8AC72304, 0x89E80000), U64(0x00000000, 0x00000000), /* == 10^19 */
    U64(0xAD78EBC5, 0xAC620000), U64(0x00000000, 0x00000000), /* == 10^20 */
    U64(0xD8D726B7, 0x177A8000), U64(0x00000000, 0x00000000), /* == 10^21 */
    U64(0x87867832, 0x6EAC9000), U64(0x00000000, 0x00000000), /* == 10^22 */
    U64(0xA968163F, 0x0A57B400), U64(0x00000000, 0x00000000), /* == 10^23 */
    U64(0xD3C21BCE, 0xCCEDA100), U64(0x00000000, 0x00000000), /* == 10^24 */
    U64(0x84595161, 0x401484A0), U64(0x00000000, 0x00000000), /* == 10^25 */
    U64(0xA56FA5B9, 0x9019A5C8), U64(0x00000000, 0x00000000), /* == 10^26 */
    U64(0xCECB8F27, 0xF4200F3A), U64(0x00000000, 0x00000000), /* == 10^27 */
    U64(0x813F3978, 0xF8940984), U64(0x40000000, 0x00000000), /* == 10^28 */
    U64(0xA18F07D7, 0x36B90BE5), U64(0x50000000, 0x00000000), /* == 10^29 */
    U64(0xC9F2C9CD, 0x04674EDE), U64(0xA4000000, 0x00000000), /* == 10^30 */
    U64(0xFC6F7C40, 0x45812296), U64(0x4D000000, 0x00000000), /* == 10^31 */
    U64(0x9DC5ADA8, 0x2B70B59D), U64(0xF0200000, 0x00000000), /* == 10^32 */
    U64(0xC5371912, 0x364CE305), U64(0x6C280000, 0x00000000), /* == 10^33 */
    U64(0xF684DF56, 0xC3E01BC6), U64(0xC7320000, 0x00000000), /* == 10^34 */
    U64(0x9A130B96, 0x3A6C115C), U64(0x3C7F4000, 0x00000000), /* == 10^35 */
    U64(0xC097CE7B, 0xC90715B3), U64(0x4B9F1000, 0x00000000), /* == 10^36 */
    U64(0xF0BDC21A, 0xBB48DB20), U64(0x1E86D400, 0x00000000), /* == 10^37 */
    U64(0x96769950, 0xB50D88F4), U64(0x13144480, 0x00000000), /* == 10^38 */
    U64(0xBC143FA4, 0xE250EB31), U64(0x17D955A0, 0x00000000), /* == 10^39 */
    U64(0xEB194F8E, 0x1AE525FD), U64(0x5DCFAB08, 0x00000000), /* == 10^40 */
    U64(0x92EFD1B8, 0xD0CF37BE), U64(0x5AA1CAE5, 0x00000000), /* == 10^41 */
    U64(0xB7ABC627, 0x050305AD), U64(0xF14A3D9E, 0x40000000), /* == 10^42 */
    U64(0xE596B7B0, 0xC643C719), U64(0x6D9CCD05, 0xD0000000), /* == 10^43 */
    U64(0x8F7E32CE, 0x7BEA5C6F), U64(0xE4820023, 0xA2000000), /* == 10^44 */
    U64(0xB35DBF82, 0x1AE4F38B), U64(0xDDA2802C, 0x8A800000), /* == 10^45 */
    U64(0xE0352F62, 0xA19E306E), U64(0xD50B2037, 0xAD200000), /* == 10^46 */
    U64(0x8C213D9D, 0xA502DE45), U64(0x4526F422, 0xCC340000), /* == 10^47 */
    U64(0xAF298D05, 0x0E4395D6), U64(0x9670B12B, 0x7F410000), /* == 10^48 */
    U64(0xDAF3F046, 0x51D47B4C), U64(0x3C0CDD76, 0x5F114000), /* == 10^49 */
    U64(0x88D8762B, 0xF324CD0F), U64(0xA5880A69, 0xFB6AC800), /* == 10^50 */
    U64(0xAB0E93B6, 0xEFEE0053), U64(0x8EEA0D04, 0x7A457A00), /* == 10^51 */
    U64(0xD5D238A4, 0xABE98068), U64(0x72A49045, 0x98D6D880), /* == 10^52 */
    U64(0x85A36366, 0xEB71F041), U64(0x47A6DA2B, 0x7F864750), /* == 10^53 */
    U64(0xA70C3C40, 0xA64E6C51), U64(0x999090B6, 0x5F67D924), /* == 10^54 */
    U64(0xD0CF4B50, 0xCFE20765), U64(0xFFF4B4E3, 0xF741CF6D), /* == 10^55 */
    U64(0x82818F12, 0x81ED449F), U64(0xBFF8F10E, 0x7A8921A4), /* ~= 10^56 */
    U64(0xA321F2D7, 0x226895C7), U64(0xAFF72D52, 0x192B6A0D), /* ~= 10^57 */
    U64(0xCBEA6F8C, 0xEB02BB39), U64(0x9BF4F8A6, 0x9F764490), /* ~= 10^58 */
    U64(0xFEE50B70, 0x25C36A08), U64(0x02F236D0, 0x4753D5B4), /* ~= 10^59 */
    U64(0x9F4F2726, 0x179A2245), U64(0x01D76242, 0x2C946590), /* ~= 10^60 */
    U64(0xC722F0EF, 0x9D80AAD6), U64(0x424D3AD2, 0xB7B97EF5), /* ~= 10^61 */
    U64(0xF8EBAD2B, 0x84E0D58B), U64(0xD2E08987, 0x65A7DEB2), /* ~= 10^62 */
    U64(0x9B934C3B, 0x330C8577), U64(0x63CC55F4, 0x9F88EB2F), /* ~= 10^63 */
    U64(0xC2781F49, 0xFFCFA6D5), U64(0x3CBF6B71, 0xC76B25FB), /* ~= 10^64 */
    U64(0xF316271C, 0x7FC3908A), U64(0x8BEF464E, 0x3945EF7A), /* ~= 10^65 */
    U64(0x97EDD871, 0xCFDA3A56), U64(0x97758BF0, 0xE3CBB5AC), /* ~= 10^66 */
    U64(0xBDE94E8E, 0x43D0C8EC), U64(0x3D52EEED, 0x1CBEA317), /* ~= 10^67 */
    U64(0xED63A231, 0xD4C4FB27), U64(0x4CA7AAA8, 0x63EE4BDD), /* ~= 10^68 */
    U64(0x945E455F, 0x24FB1CF8), U64(0x8FE8CAA9, 0x3E74EF6A), /* ~= 10^69 */
    U64(0xB975D6B6, 0xEE39E436), U64(0xB3E2FD53, 0x8E122B44), /* ~= 10^70 */
    U64(0xE7D34C64, 0xA9C85D44), U64(0x60DBBCA8, 0x7196B616), /* ~= 10^71 */
    U64(0x90E40FBE, 0xEA1D3A4A), U64(0xBC8955E9, 0x46FE31CD), /* ~= 10^72 */
    U64(0xB51D13AE, 0xA4A488DD), U64(0x6BABAB63, 0x98BDBE41), /* ~= 10^73 */
    U64(0xE264589A, 0x4DCDAB14), U64(0xC696963C, 0x7EED2DD1), /* ~= 10^74 */
    U64(0x8D7EB760, 0x70A08AEC), U64(0xFC1E1DE5, 0xCF543CA2), /* ~= 10^75 */
    U64(0xB0DE6538, 0x8CC8ADA8), U64(0x3B25A55F, 0x43294BCB), /* ~= 10^76 */
    U64(0xDD15FE86, 0xAFFAD912), U64(0x49EF0EB7, 0x13F39EBE), /* ~= 10^77 */
    U64(0x8A2DBF14, 0x2DFCC7AB), U64(0x6E356932, 0x6C784337), /* ~= 10^78 */
    U64(0xACB92ED9, 0x397BF996), U64(0x49C2C37F, 0x07965404), /* ~= 10^79 */
    U64(0xD7E77A8F, 0x87DAF7FB), U64(0xDC33745E, 0xC97BE906), /* ~= 10^80 */
    U64(0x86F0AC99, 0xB4E8DAFD), U64(0x69A028BB, 0x3DED71A3), /* ~= 10^81 */
    U64(0xA8ACD7C0, 0x222311BC), U64(0xC40832EA, 0x0D68CE0C), /* ~= 10^82 */
    U64(0xD2D80DB0, 0x2AABD62B), U64(0xF50A3FA4, 0x90C30190), /* ~= 10^83 */
    U64(0x83C7088E, 0x1AAB65DB), U64(0x792667C6, 0xDA79E0FA), /* ~= 10^84 */
    U64(0xA4B8CAB1, 0xA1563F52), U64(0x577001B8, 0x91185938), /* ~= 10^85 */
    U64(0xCDE6FD5E, 0x09ABCF26), U64(0xED4C0226, 0xB55E6F86), /* ~= 10^86 */
    U64(0x80B05E5A, 0xC60B6178), U64(0x544F8158, 0x315B05B4), /* ~= 10^87 */
    U64(0xA0DC75F1, 0x778E39D6), U64(0x696361AE, 0x3DB1C721), /* ~= 10^88 */
    U64(0xC913936D, 0xD571C84C), U64(0x03BC3A19, 0xCD1E38E9), /* ~= 10^89 */
    U64(0xFB587849, 0x4ACE3A5F), U64(0x04AB48A0, 0x4065C723), /* ~= 10^90 */
    U64(0x9D174B2D, 0xCEC0E47B), U64(0x62EB0D64, 0x283F9C76), /* ~= 10^91 */
    U64(0xC45D1DF9, 0x42711D9A), U64(0x3BA5D0BD, 0x324F8394), /* ~= 10^92 */
    U64(0xF5746577, 0x930D6500), U64(0xCA8F44EC, 0x7EE36479), /* ~= 10^93 */
    U64(0x9968BF6A, 0xBBE85F20), U64(0x7E998B13, 0xCF4E1ECB), /* ~= 10^94 */
    U64(0xBFC2EF45, 0x6AE276E8), U64(0x9E3FEDD8, 0xC321A67E), /* ~= 10^95 */
    U64(0xEFB3AB16, 0xC59B14A2), U64(0xC5CFE94E, 0xF3EA101E), /* ~= 10^96 */
    U64(0x95D04AEE, 0x3B80ECE5), U64(0xBBA1F1D1, 0x58724A12), /* ~= 10^97 */
    U64(0xBB445DA9, 0xCA61281F), U64(0x2A8A6E45, 0xAE8EDC97), /* ~= 10^98 */
    U64(0xEA157514, 0x3CF97226), U64(0xF52D09D7, 0x1A3293BD), /* ~= 10^99 */
    U64(0x924D692C, 0xA61BE758), U64(0x593C2626, 0x705F9C56), /* ~= 10^100 */
    U64(0xB6E0C377, 0xCFA2E12E), U64(0x6F8B2FB0, 0x0C77836C), /* ~= 10^101 */
    U64(0xE498F455, 0xC38B997A), U64(0x0B6DFB9C, 0x0F956447), /* ~= 10^102 */
    U64(0x8EDF98B5, 0x9A373FEC), U64(0x4724BD41, 0x89BD5EAC), /* ~= 10^103 */
    U64(0xB2977EE3, 0x00C50FE7), U64(0x58EDEC91, 0xEC2CB657), /* ~= 10^104 */
    U64(0xDF3D5E9B, 0xC0F653E1), U64(0x2F2967B6, 0x6737E3ED), /* ~= 10^105 */
    U64(0x8B865B21, 0x5899F46C), U64(0xBD79E0D2, 0x0082EE74), /* ~= 10^106 */
    U64(0xAE67F1E9, 0xAEC07187), U64(0xECD85906, 0x80A3AA11), /* ~= 10^107 */
    U64(0xDA01EE64, 0x1A708DE9), U64(0xE80E6F48, 0x20CC9495), /* ~= 10^108 */
    U64(0x884134FE, 0x908658B2), U64(0x3109058D, 0x147FDCDD), /* ~= 10^109 */
    U64(0xAA51823E, 0x34A7EEDE), U64(0xBD4B46F0, 0x599FD415), /* ~= 10^110 */
    U64(0xD4E5E2CD, 0xC1D1EA96), U64(0x6C9E18AC, 0x7007C91A), /* ~= 10^111 */
    U64(0x850FADC0, 0x9923329E), U64(0x03E2CF6B, 0xC604DDB0), /* ~= 10^112 */
    U64(0xA6539930, 0xBF6BFF45), U64(0x84DB8346, 0xB786151C), /* ~= 10^113 */
    U64(0xCFE87F7C, 0xEF46FF16), U64(0xE6126418, 0x65679A63), /* ~= 10^114 */
    U64(0x81F14FAE, 0x158C5F6E), U64(0x4FCB7E8F, 0x3F60C07E), /* ~= 10^115 */
    U64(0xA26DA399, 0x9AEF7749), U64(0xE3BE5E33, 0x0F38F09D), /* ~= 10^116 */
    U64(0xCB090C80, 0x01AB551C), U64(0x5CADF5BF, 0xD3072CC5), /* ~= 10^117 */
    U64(0xFDCB4FA0, 0x02162A63), U64(0x73D9732F, 0xC7C8F7F6), /* ~= 10^118 */
    U64(0x9E9F11C4, 0x014DDA7E), U64(0x2867E7FD, 0xDCDD9AFA), /* ~= 10^119 */
    U64(0xC646D635, 0x01A1511D), U64(0xB281E1FD, 0x541501B8), /* ~= 10^120 */
    U64(0xF7D88BC2, 0x4209A565), U64(0x1F225A7C, 0xA91A4226), /* ~= 10^121 */
    U64(0x9AE75759, 0x6946075F), U64(0x3375788D, 0xE9B06958), /* ~= 10^122 */
    U64(0xC1A12D2F, 0xC3978937), U64(0x0052D6B1, 0x641C83AE), /* ~= 10^123 */
    U64(0xF209787B, 0xB47D6B84), U64(0xC0678C5D, 0xBD23A49A), /* ~= 10^124 */
    U64(0x9745EB4D, 0x50CE6332), U64(0xF840B7BA, 0x963646E0), /* ~= 10^125 */
    U64(0xBD176620, 0xA501FBFF), U64(0xB650E5A9, 0x3BC3D898), /* ~= 10^126 */
    U64(0xEC5D3FA8, 0xCE427AFF), U64(0xA3E51F13, 0x8AB4CEBE), /* ~= 10^127 */
    U64(0x93BA47C9, 0x80E98CDF), U64(0xC66F336C, 0x36B10137), /* ~= 10^128 */
    U64(0xB8A8D9BB, 0xE123F017), U64(0xB80B0047, 0x445D4184), /* ~= 10^129 */
    U64(0xE6D3102A, 0xD96CEC1D), U64(0xA60DC059, 0x157491E5), /* ~= 10^130 */
    U64(0x9043EA1A, 0xC7E41392), U64(0x87C89837, 0xAD68DB2F), /* ~= 10^131 */
    U64(0xB454E4A1, 0x79DD1877), U64(0x29BABE45, 0x98C311FB), /* ~= 10^132 */
    U64(0xE16A1DC9, 0xD8545E94), U64(0xF4296DD6, 0xFEF3D67A), /* ~= 10^133 */
    U64(0x8CE2529E, 0x2734BB1D), U64(0x1899E4A6, 0x5F58660C), /* ~= 10^134 */
    U64(0xB01AE745, 0xB101E9E4), U64(0x5EC05DCF, 0xF72E7F8F), /* ~= 10^135 */
    U64(0xDC21A117, 0x1D42645D), U64(0x76707543, 0xF4FA1F73), /* ~= 10^136 */
    U64(0x899504AE, 0x72497EBA), U64(0x6A06494A, 0x791C53A8), /* ~= 10^137 */
    U64(0xABFA45DA, 0x0EDBDE69), U64(0x0487DB9D, 0x17636892), /* ~= 10^138 */
    U64(0xD6F8D750, 0x9292D603), U64(0x45A9D284, 0x5D3C42B6), /* ~= 10^139 */
    U64(0x865B8692, 0x5B9BC5C2), U64(0x0B8A2392, 0xBA45A9B2), /* ~= 10^140 */
    U64(0xA7F26836, 0xF282B732), U64(0x8E6CAC77, 0x68D7141E), /* ~= 10^141 */
    U64(0xD1EF0244, 0xAF2364FF), U64(0x3207D795, 0x430CD926), /* ~= 10^142 */
    U64(0x8335616A, 0xED761F1F), U64(0x7F44E6BD, 0x49E807B8), /* ~= 10^143 */
    U64(0xA402B9C5, 0xA8D3A6E7), U64(0x5F16206C, 0x9C6209A6), /* ~= 10^144 */
    U64(0xCD036837, 0x130890A1), U64(0x36DBA887, 0xC37A8C0F), /* ~= 10^145 */
    U64(0x80222122, 0x6BE55A64), U64(0xC2494954, 0xDA2C9789), /* ~= 10^146 */
    U64(0xA02AA96B, 0x06DEB0FD), U64(0xF2DB9BAA, 0x10B7BD6C), /* ~= 10^147 */
    U64(0xC83553C5, 0xC8965D3D), U64(0x6F928294, 0x94E5ACC7), /* ~= 10^148 */
    U64(0xFA42A8B7, 0x3ABBF48C), U64(0xCB772339, 0xBA1F17F9), /* ~= 10^149 */
    U64(0x9C69A972, 0x84B578D7), U64(0xFF2A7604, 0x14536EFB), /* ~= 10^150 */
    U64(0xC38413CF, 0x25E2D70D), U64(0xFEF51385, 0x19684ABA), /* ~= 10^151 */
    U64(0xF46518C2, 0xEF5B8CD1), U64(0x7EB25866, 0x5FC25D69), /* ~= 10^152 */
    U64(0x98BF2F79, 0xD5993802), U64(0xEF2F773F, 0xFBD97A61), /* ~= 10^153 */
    U64(0xBEEEFB58, 0x4AFF8603), U64(0xAAFB550F, 0xFACFD8FA), /* ~= 10^154 */
    U64(0xEEAABA2E, 0x5DBF6784), U64(0x95BA2A53, 0xF983CF38), /* ~= 10^155 */
    U64(0x952AB45C, 0xFA97A0B2), U64(0xDD945A74, 0x7BF26183), /* ~= 10^156 */
    U64(0xBA756174, 0x393D88DF), U64(0x94F97111, 0x9AEEF9E4), /* ~= 10^157 */
    U64(0xE912B9D1, 0x478CEB17), U64(0x7A37CD56, 0x01AAB85D), /* ~= 10^158 */
    U64(0x91ABB422, 0xCCB812EE), U64(0xAC62E055, 0xC10AB33A), /* ~= 10^159 */
    U64(0xB616A12B, 0x7FE617AA), U64(0x577B986B, 0x314D6009), /* ~= 10^160 */
    U64(0xE39C4976, 0x5FDF9D94), U64(0xED5A7E85, 0xFDA0B80B), /* ~= 10^161 */
    U64(0x8E41ADE9, 0xFBEBC27D), U64(0x14588F13, 0xBE847307), /* ~= 10^162 */
    U64(0xB1D21964, 0x7AE6B31C), U64(0x596EB2D8, 0xAE258FC8), /* ~= 10^163 */
    U64(0xDE469FBD, 0x99A05FE3), U64(0x6FCA5F8E, 0xD9AEF3BB), /* ~= 10^164 */
    U64(0x8AEC23D6, 0x80043BEE), U64(0x25DE7BB9, 0x480D5854), /* ~= 10^165 */
    U64(0xADA72CCC, 0x20054AE9), U64(0xAF561AA7, 0x9A10AE6A), /* ~= 10^166 */
    U64(0xD910F7FF, 0x28069DA4), U64(0x1B2BA151, 0x8094DA04), /* ~= 10^167 */
    U64(0x87AA9AFF, 0x79042286), U64(0x90FB44D2, 0xF05D0842), /* ~= 10^168 */
    U64(0xA99541BF, 0x57452B28), U64(0x353A1607, 0xAC744A53), /* ~= 10^169 */
    U64(0xD3FA922F, 0x2D1675F2), U64(0x42889B89, 0x97915CE8), /* ~= 10^170 */
    U64(0x847C9B5D, 0x7C2E09B7), U64(0x69956135, 0xFEBADA11), /* ~= 10^171 */
    U64(0xA59BC234, 0xDB398C25), U64(0x43FAB983, 0x7E699095), /* ~= 10^172 */
    U64(0xCF02B2C2, 0x1207EF2E), U64(0x94F967E4, 0x5E03F4BB), /* ~= 10^173 */
    U64(0x8161AFB9, 0x4B44F57D), U64(0x1D1BE0EE, 0xBAC278F5), /* ~= 10^174 */
    U64(0xA1BA1BA7, 0x9E1632DC), U64(0x6462D92A, 0x69731732), /* ~= 10^175 */
    U64(0xCA28A291, 0x859BBF93), U64(0x7D7B8F75, 0x03CFDCFE), /* ~= 10^176 */
    U64(0xFCB2CB35, 0xE702AF78), U64(0x5CDA7352, 0x44C3D43E), /* ~= 10^177 */
    U64(0x9DEFBF01, 0xB061ADAB), U64(0x3A088813, 0x6AFA64A7), /* ~= 10^178 */
    U64(0xC56BAEC2, 0x1C7A1916), U64(0x088AAA18, 0x45B8FDD0), /* ~= 10^179 */
    U64(0xF6C69A72, 0xA3989F5B), U64(0x8AAD549E, 0x57273D45), /* ~= 10^180 */
    U64(0x9A3C2087, 0xA63F6399), U64(0x36AC54E2, 0xF678864B), /* ~= 10^181 */
    U64(0xC0CB28A9, 0x8FCF3C7F), U64(0x84576A1B, 0xB416A7DD), /* ~= 10^182 */
    U64(0xF0FDF2D3, 0xF3C30B9F), U64(0x656D44A2, 0xA11C51D5), /* ~= 10^183 */
    U64(0x969EB7C4, 0x7859E743), U64(0x9F644AE5, 0xA4B1B325), /* ~= 10^184 */
    U64(0xBC4665B5, 0x96706114), U64(0x873D5D9F, 0x0DDE1FEE), /* ~= 10^185 */
    U64(0xEB57FF22, 0xFC0C7959), U64(0xA90CB506, 0xD155A7EA), /* ~= 10^186 */
    U64(0x9316FF75, 0xDD87CBD8), U64(0x09A7F124, 0x42D588F2), /* ~= 10^187 */
    U64(0xB7DCBF53, 0x54E9BECE), U64(0x0C11ED6D, 0x538AEB2F), /* ~= 10^188 */
    U64(0xE5D3EF28, 0x2A242E81), U64(0x8F1668C8, 0xA86DA5FA), /* ~= 10^189 */
    U64(0x8FA47579, 0x1A569D10), U64(0xF96E017D, 0x694487BC), /* ~= 10^190 */
    U64(0xB38D92D7, 0x60EC4455), U64(0x37C981DC, 0xC395A9AC), /* ~= 10^191 */
    U64(0xE070F78D, 0x3927556A), U64(0x85BBE253, 0xF47B1417), /* ~= 10^192 */
    U64(0x8C469AB8, 0x43B89562), U64(0x93956D74, 0x78CCEC8E), /* ~= 10^193 */
    U64(0xAF584166, 0x54A6BABB), U64(0x387AC8D1, 0x970027B2), /* ~= 10^194 */
    U64(0xDB2E51BF, 0xE9D0696A), U64(0x06997B05, 0xFCC0319E), /* ~= 10^195 */
    U64(0x88FCF317, 0xF22241E2), U64(0x441FECE3, 0xBDF81F03), /* ~= 10^196 */
    U64(0xAB3C2FDD, 0xEEAAD25A), U64(0xD527E81C, 0xAD7626C3), /* ~= 10^197 */
    U64(0xD60B3BD5, 0x6A5586F1), U64(0x8A71E223, 0xD8D3B074), /* ~= 10^198 */
    U64(0x85C70565, 0x62757456), U64(0xF6872D56, 0x67844E49), /* ~= 10^199 */
    U64(0xA738C6BE, 0xBB12D16C), U64(0xB428F8AC, 0x016561DB), /* ~= 10^200 */
    U64(0xD106F86E, 0x69D785C7), U64(0xE13336D7, 0x01BEBA52), /* ~= 10^201 */
    U64(0x82A45B45, 0x0226B39C), U64(0xECC00246, 0x61173473), /* ~= 10^202 */
    U64(0xA34D7216, 0x42B06084), U64(0x27F002D7, 0xF95D0190), /* ~= 10^203 */
    U64(0xCC20CE9B, 0xD35C78A5), U64(0x31EC038D, 0xF7B441F4), /* ~= 10^204 */
    U64(0xFF290242, 0xC83396CE), U64(0x7E670471, 0x75A15271), /* ~= 10^205 */
    U64(0x9F79A169, 0xBD203E41), U64(0x0F0062C6, 0xE984D386), /* ~= 10^206 */
    U64(0xC75809C4, 0x2C684DD1), U64(0x52C07B78, 0xA3E60868), /* ~= 10^207 */
    U64(0xF92E0C35, 0x37826145), U64(0xA7709A56, 0xCCDF8A82), /* ~= 10^208 */
    U64(0x9BBCC7A1, 0x42B17CCB), U64(0x88A66076, 0x400BB691), /* ~= 10^209 */
    U64(0xC2ABF989, 0x935DDBFE), U64(0x6ACFF893, 0xD00EA435), /* ~= 10^210 */
    U64(0xF356F7EB, 0xF83552FE), U64(0x0583F6B8, 0xC4124D43), /* ~= 10^211 */
    U64(0x98165AF3, 0x7B2153DE), U64(0xC3727A33, 0x7A8B704A), /* ~= 10^212 */
    U64(0xBE1BF1B0, 0x59E9A8D6), U64(0x744F18C0, 0x592E4C5C), /* ~= 10^213 */
    U64(0xEDA2EE1C, 0x7064130C), U64(0x1162DEF0, 0x6F79DF73), /* ~= 10^214 */
    U64(0x9485D4D1, 0xC63E8BE7), U64(0x8ADDCB56, 0x45AC2BA8), /* ~= 10^215 */
    U64(0xB9A74A06, 0x37CE2EE1), U64(0x6D953E2B, 0xD7173692), /* ~= 10^216 */
    U64(0xE8111C87, 0xC5C1BA99), U64(0xC8FA8DB6, 0xCCDD0437), /* ~= 10^217 */
    U64(0x910AB1D4, 0xDB9914A0), U64(0x1D9C9892, 0x400A22A2), /* ~= 10^218 */
    U64(0xB54D5E4A, 0x127F59C8), U64(0x2503BEB6, 0xD00CAB4B), /* ~= 10^219 */
    U64(0xE2A0B5DC, 0x971F303A), U64(0x2E44AE64, 0x840FD61D), /* ~= 10^220 */
    U64(0x8DA471A9, 0xDE737E24), U64(0x5CEAECFE, 0xD289E5D2), /* ~= 10^221 */
    U64(0xB10D8E14, 0x56105DAD), U64(0x7425A83E, 0x872C5F47), /* ~= 10^222 */
    U64(0xDD50F199, 0x6B947518), U64(0xD12F124E, 0x28F77719), /* ~= 10^223 */
    U64(0x8A5296FF, 0xE33CC92F), U64(0x82BD6B70, 0xD99AAA6F), /* ~= 10^224 */
    U64(0xACE73CBF, 0xDC0BFB7B), U64(0x636CC64D, 0x1001550B), /* ~= 10^225 */
    U64(0xD8210BEF, 0xD30EFA5A), U64(0x3C47F7E0, 0x5401AA4E), /* ~= 10^226 */
    U64(0x8714A775, 0xE3E95C78), U64(0x65ACFAEC, 0x34810A71), /* ~= 10^227 */
    U64(0xA8D9D153, 0x5CE3B396), U64(0x7F1839A7, 0x41A14D0D), /* ~= 10^228 */
    U64(0xD31045A8, 0x341CA07C), U64(0x1EDE4811, 0x1209A050), /* ~= 10^229 */
    U64(0x83EA2B89, 0x2091E44D), U64(0x934AED0A, 0xAB460432), /* ~= 10^230 */
    U64(0xA4E4B66B, 0x68B65D60), U64(0xF81DA84D, 0x5617853F), /* ~= 10^231 */
    U64(0xCE1DE406, 0x42E3F4B9), U64(0x36251260, 0xAB9D668E), /* ~= 10^232 */
    U64(0x80D2AE83, 0xE9CE78F3), U64(0xC1D72B7C, 0x6B426019), /* ~= 10^233 */
    U64(0xA1075A24, 0xE4421730), U64(0xB24CF65B, 0x8612F81F), /* ~= 10^234 */
    U64(0xC94930AE, 0x1D529CFC), U64(0xDEE033F2, 0x6797B627), /* ~= 10^235 */
    U64(0xFB9B7CD9, 0xA4A7443C), U64(0x169840EF, 0x017DA3B1), /* ~= 10^236 */
    U64(0x9D412E08, 0x06E88AA5), U64(0x8E1F2895, 0x60EE864E), /* ~= 10^237 */
    U64(0xC491798A, 0x08A2AD4E), U64(0xF1A6F2BA, 0xB92A27E2), /* ~= 10^238 */
    U64(0xF5B5D7EC, 0x8ACB58A2), U64(0xAE10AF69, 0x6774B1DB), /* ~= 10^239 */
    U64(0x9991A6F3, 0xD6BF1765), U64(0xACCA6DA1, 0xE0A8EF29), /* ~= 10^240 */
    U64(0xBFF610B0, 0xCC6EDD3F), U64(0x17FD090A, 0x58D32AF3), /* ~= 10^241 */
    U64(0xEFF394DC, 0xFF8A948E), U64(0xDDFC4B4C, 0xEF07F5B0), /* ~= 10^242 */
    U64(0x95F83D0A, 0x1FB69CD9), U64(0x4ABDAF10, 0x1564F98E), /* ~= 10^243 */
    U64(0xBB764C4C, 0xA7A4440F), U64(0x9D6D1AD4, 0x1ABE37F1), /* ~= 10^244 */
    U64(0xEA53DF5F, 0xD18D5513), U64(0x84C86189, 0x216DC5ED), /* ~= 10^245 */
    U64(0x92746B9B, 0xE2F8552C), U64(0x32FD3CF5, 0xB4E49BB4), /* ~= 10^246 */
    U64(0xB7118682, 0xDBB66A77), U64(0x3FBC8C33, 0x221DC2A1), /* ~= 10^247 */
    U64(0xE4D5E823, 0x92A40515), U64(0x0FABAF3F, 0xEAA5334A), /* ~= 10^248 */
    U64(0x8F05B116, 0x3BA6832D), U64(0x29CB4D87, 0xF2A7400E), /* ~= 10^249 */
    U64(0xB2C71D5B, 0xCA9023F8), U64(0x743E20E9, 0xEF511012), /* ~= 10^250 */
    U64(0xDF78E4B2, 0xBD342CF6), U64(0x914DA924, 0x6B255416), /* ~= 10^251 */
    U64(0x8BAB8EEF, 0xB6409C1A), U64(0x1AD089B6, 0xC2F7548E), /* ~= 10^252 */
    U64(0xAE9672AB, 0xA3D0C320), U64(0xA184AC24, 0x73B529B1), /* ~= 10^253 */
    U64(0xDA3C0F56, 0x8CC4F3E8), U64(0xC9E5D72D, 0x90A2741E), /* ~= 10^254 */
    U64(0x88658996, 0x17FB1871), U64(0x7E2FA67C, 0x7A658892), /* ~= 10^255 */
    U64(0xAA7EEBFB, 0x9DF9DE8D), U64(0xDDBB901B, 0x98FEEAB7), /* ~= 10^256 */
    U64(0xD51EA6FA, 0x85785631), U64(0x552A7422, 0x7F3EA565), /* ~= 10^257 */
    U64(0x8533285C, 0x936B35DE), U64(0xD53A8895, 0x8F87275F), /* ~= 10^258 */
    U64(0xA67FF273, 0xB8460356), U64(0x8A892ABA, 0xF368F137), /* ~= 10^259 */
    U64(0xD01FEF10, 0xA657842C), U64(0x2D2B7569, 0xB0432D85), /* ~= 10^260 */
    U64(0x8213F56A, 0x67F6B29B), U64(0x9C3B2962, 0x0E29FC73), /* ~= 10^261 */
    U64(0xA298F2C5, 0x01F45F42), U64(0x8349F3BA, 0x91B47B8F), /* ~= 10^262 */
    U64(0xCB3F2F76, 0x42717713), U64(0x241C70A9, 0x36219A73), /* ~= 10^263 */
    U64(0xFE0EFB53, 0xD30DD4D7), U64(0xED238CD3, 0x83AA0110), /* ~= 10^264 */
    U64(0x9EC95D14, 0x63E8A506), U64(0xF4363804, 0x324A40AA), /* ~= 10^265 */
    U64(0xC67BB459, 0x7CE2CE48), U64(0xB143C605, 0x3EDCD0D5), /* ~= 10^266 */
    U64(0xF81AA16F, 0xDC1B81DA), U64(0xDD94B786, 0x8E94050A), /* ~= 10^267 */
    U64(0x9B10A4E5, 0xE9913128), U64(0xCA7CF2B4, 0x191C8326), /* ~= 10^268 */
    U64(0xC1D4CE1F, 0x63F57D72), U64(0xFD1C2F61, 0x1F63A3F0), /* ~= 10^269 */
    U64(0xF24A01A7, 0x3CF2DCCF), U64(0xBC633B39, 0x673C8CEC), /* ~= 10^270 */
    U64(0x976E4108, 0x8617CA01), U64(0xD5BE0503, 0xE085D813), /* ~= 10^271 */
    U64(0xBD49D14A, 0xA79DBC82), U64(0x4B2D8644, 0xD8A74E18), /* ~= 10^272 */
    U64(0xEC9C459D, 0x51852BA2), U64(0xDDF8E7D6, 0x0ED1219E), /* ~= 10^273 */
    U64(0x93E1AB82, 0x52F33B45), U64(0xCABB90E5, 0xC942B503), /* ~= 10^274 */
    U64(0xB8DA1662, 0xE7B00A17), U64(0x3D6A751F, 0x3B936243), /* ~= 10^275 */
    U64(0xE7109BFB, 0xA19C0C9D), U64(0x0CC51267, 0x0A783AD4), /* ~= 10^276 */
    U64(0x906A617D, 0x450187E2), U64(0x27FB2B80, 0x668B24C5), /* ~= 10^277 */
    U64(0xB484F9DC, 0x9641E9DA), U64(0xB1F9F660, 0x802DEDF6), /* ~= 10^278 */
    U64(0xE1A63853, 0xBBD26451), U64(0x5E7873F8, 0xA0396973), /* ~= 10^279 */
    U64(0x8D07E334, 0x55637EB2), U64(0xDB0B487B, 0x6423E1E8), /* ~= 10^280 */
    U64(0xB049DC01, 0x6ABC5E5F), U64(0x91CE1A9A, 0x3D2CDA62), /* ~= 10^281 */
    U64(0xDC5C5301, 0xC56B75F7), U64(0x7641A140, 0xCC7810FB), /* ~= 10^282 */
    U64(0x89B9B3E1, 0x1B6329BA), U64(0xA9E904C8, 0x7FCB0A9D), /* ~= 10^283 */
    U64(0xAC2820D9, 0x623BF429), U64(0x546345FA, 0x9FBDCD44), /* ~= 10^284 */
    U64(0xD732290F, 0xBACAF133), U64(0xA97C1779, 0x47AD4095), /* ~= 10^285 */
    U64(0x867F59A9, 0xD4BED6C0), U64(0x49ED8EAB, 0xCCCC485D), /* ~= 10^286 */
    U64(0xA81F3014, 0x49EE8C70), U64(0x5C68F256, 0xBFFF5A74), /* ~= 10^287 */
    U64(0xD226FC19, 0x5C6A2F8C), U64(0x73832EEC, 0x6FFF3111), /* ~= 10^288 */
    U64(0x83585D8F, 0xD9C25DB7), U64(0xC831FD53, 0xC5FF7EAB), /* ~= 10^289 */
    U64(0xA42E74F3, 0xD032F525), U64(0xBA3E7CA8, 0xB77F5E55), /* ~= 10^290 */
    U64(0xCD3A1230, 0xC43FB26F), U64(0x28CE1BD2, 0xE55F35EB), /* ~= 10^291 */
    U64(0x80444B5E, 0x7AA7CF85), U64(0x7980D163, 0xCF5B81B3), /* ~= 10^292 */
    U64(0xA0555E36, 0x1951C366), U64(0xD7E105BC, 0xC332621F), /* ~= 10^293 */
    U64(0xC86AB5C3, 0x9FA63440), U64(0x8DD9472B, 0xF3FEFAA7), /* ~= 10^294 */
    U64(0xFA856334, 0x878FC150), U64(0xB14F98F6, 0xF0FEB951), /* ~= 10^295 */
    U64(0x9C935E00, 0xD4B9D8D2), U64(0x6ED1BF9A, 0x569F33D3), /* ~= 10^296 */
    U64(0xC3B83581, 0x09E84F07), U64(0x0A862F80, 0xEC4700C8), /* ~= 10^297 */
    U64(0xF4A642E1, 0x4C6262C8), U64(0xCD27BB61, 0x2758C0FA), /* ~= 10^298 */
    U64(0x98E7E9CC, 0xCFBD7DBD), U64(0x8038D51C, 0xB897789C), /* ~= 10^299 */
    U64(0xBF21E440, 0x03ACDD2C), U64(0xE0470A63, 0xE6BD56C3), /* ~= 10^300 */
    U64(0xEEEA5D50, 0x04981478), U64(0x1858CCFC, 0xE06CAC74), /* ~= 10^301 */
    U64(0x95527A52, 0x02DF0CCB), U64(0x0F37801E, 0x0C43EBC8), /* ~= 10^302 */
    U64(0xBAA718E6, 0x8396CFFD), U64(0xD3056025, 0x8F54E6BA), /* ~= 10^303 */
    U64(0xE950DF20, 0x247C83FD), U64(0x47C6B82E, 0xF32A2069), /* ~= 10^304 */
    U64(0x91D28B74, 0x16CDD27E), U64(0x4CDC331D, 0x57FA5441), /* ~= 10^305 */
    U64(0xB6472E51, 0x1C81471D), U64(0xE0133FE4, 0xADF8E952), /* ~= 10^306 */
    U64(0xE3D8F9E5, 0x63A198E5), U64(0x58180FDD, 0xD97723A6), /* ~= 10^307 */
    U64(0x8E679C2F, 0x5E44FF8F), U64(0x570F09EA, 0xA7EA7648), /* ~= 10^308 */
    U64(0xB201833B, 0x35D63F73), U64(0x2CD2CC65, 0x51E513DA), /* ~= 10^309 */
    U64(0xDE81E40A, 0x034BCF4F), U64(0xF8077F7E, 0xA65E58D1), /* ~= 10^310 */
    U64(0x8B112E86, 0x420F6191), U64(0xFB04AFAF, 0x27FAF782), /* ~= 10^311 */
    U64(0xADD57A27, 0xD29339F6), U64(0x79C5DB9A, 0xF1F9B563), /* ~= 10^312 */
    U64(0xD94AD8B1, 0xC7380874), U64(0x18375281, 0xAE7822BC), /* ~= 10^313 */
    U64(0x87CEC76F, 0x1C830548), U64(0x8F229391, 0x0D0B15B5), /* ~= 10^314 */
    U64(0xA9C2794A, 0xE3A3C69A), U64(0xB2EB3875, 0x504DDB22), /* ~= 10^315 */
    U64(0xD433179D, 0x9C8CB841), U64(0x5FA60692, 0xA46151EB), /* ~= 10^316 */
    U64(0x849FEEC2, 0x81D7F328), U64(0xDBC7C41B, 0xA6BCD333), /* ~= 10^317 */
    U64(0xA5C7EA73, 0x224DEFF3), U64(0x12B9B522, 0x906C0800), /* ~= 10^318 */
    U64(0xCF39E50F, 0xEAE16BEF), U64(0xD768226B, 0x34870A00), /* ~= 10^319 */
    U64(0x81842F29, 0xF2CCE375), U64(0xE6A11583, 0x00D46640), /* ~= 10^320 */
    U64(0xA1E53AF4, 0x6F801C53), U64(0x60495AE3, 0xC1097FD0), /* ~= 10^321 */
    U64(0xCA5E89B1, 0x8B602368), U64(0x385BB19C, 0xB14BDFC4), /* ~= 10^322 */
    U64(0xFCF62C1D, 0xEE382C42), U64(0x46729E03, 0xDD9ED7B5), /* ~= 10^323 */
    U64(0x9E19DB92, 0xB4E31BA9), U64(0x6C07A2C2, 0x6A8346D1)  /* ~= 10^324 */
};

/**
 Get the cached pow10 value from `pow10_sig_table`.
 @param exp10 The exponent of pow(10, e). This value must in range
              `POW10_SIG_TABLE_MIN_EXP` to `POW10_SIG_TABLE_MAX_EXP`.
 @param hi    The highest 64 bits of pow(10, e).
 @param lo    The lower 64 bits after `hi`.
 */
static_inline void pow10_table_get_sig(i32 exp10, u64 *hi, u64 *lo) {
    i32 idx = exp10 - (POW10_SIG_TABLE_MIN_EXP);
    *hi = pow10_sig_table[idx * 2];
    *lo = pow10_sig_table[idx * 2 + 1];
}

/**
 Get the exponent (base 2) for highest 64 bits significand in `pow10_sig_table`.
 */
static_inline void pow10_table_get_exp(i32 exp10, i32 *exp2) {
    /* e2 = floor(log2(pow(10, e))) - 64 + 1 */
    /*    = floor(e * log2(10) - 63)         */
    *exp2 = (exp10 * 217706 - 4128768) >> 16;
}

#endif



/*==============================================================================
 * MARK: - Number and Bit Utils (Private)
 *============================================================================*/

/** Convert bits to double. */
static_inline f64 f64_from_bits(u64 u) {
    f64 f;
    memcpy(&f, &u, sizeof(u));
    return f;
}

/** Convert double to bits. */
static_inline u64 f64_to_bits(f64 f) {
    u64 u;
    memcpy(&u, &f, sizeof(u));
    return u;
}

/** Convert double to bits. */
static_inline u32 f32_to_bits(f32 f) {
    u32 u;
    memcpy(&u, &f, sizeof(u));
    return u;
}

/** Get 'infinity' bits with sign. */
static_inline u64 f64_bits_inf(bool sign) {
#if YYJSON_HAS_IEEE_754
    return F64_BITS_INF | ((u64)sign << 63);
#elif defined(INFINITY)
    return f64_to_bits(sign ? -INFINITY : INFINITY);
#else
    return f64_to_bits(sign ? -HUGE_VAL : HUGE_VAL);
#endif
}

/** Get 'nan' bits with sign. */
static_inline u64 f64_bits_nan(bool sign) {
#if YYJSON_HAS_IEEE_754
    return F64_BITS_NAN | ((u64)sign << 63);
#elif defined(NAN)
    return f64_to_bits(sign ? (f64)-NAN : (f64)NAN);
#else
    return f64_to_bits((sign ? -0.0 : 0.0) / 0.0);
#endif
}

/** Casting double to float, allow overflow. */
#if yyjson_has_attribute(no_sanitize)
__attribute__((no_sanitize("undefined")))
#elif yyjson_gcc_available(4, 9, 0)
__attribute__((__no_sanitize_undefined__))
#endif
static_inline f32 f64_to_f32(f64 val) {
    return (f32)val;
}

/** Returns the number of leading 0-bits in value (input should not be 0). */
static_inline u32 u64_lz_bits(u64 v) {
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
    /* branchless, use De Bruijn sequence */
    /* see: https://www.chessprogramming.org/BitScan */
    const u8 table[64] = {
        63, 16, 62,  7, 15, 36, 61,  3,  6, 14, 22, 26, 35, 47, 60,  2,
         9,  5, 28, 11, 13, 21, 42, 19, 25, 31, 34, 40, 46, 52, 59,  1,
        17,  8, 37,  4, 23, 27, 48, 10, 29, 12, 43, 20, 32, 41, 53, 18,
        38, 24, 49, 30, 44, 33, 54, 39, 50, 45, 55, 51, 56, 57, 58,  0
    };
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
static_inline u32 u64_tz_bits(u64 v) {
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
    /* branchless, use De Bruijn sequence */
    /* see: https://www.chessprogramming.org/BitScan */
    const u8 table[64] = {
         0,  1,  2, 53,  3,  7, 54, 27,  4, 38, 41,  8, 34, 55, 48, 28,
        62,  5, 39, 46, 44, 42, 22,  9, 24, 35, 59, 56, 49, 18, 29, 11,
        63, 52,  6, 26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
    };
    return table[((v & (~v + 1)) * U64(0x022FDD63, 0xCC95386D)) >> 58];
#endif
}

/** Multiplies two 64-bit unsigned integers (a * b),
    returns the 128-bit result as 'hi' and 'lo'. */
static_inline void u128_mul(u64 a, u64 b, u64 *hi, u64 *lo) {
#if YYJSON_HAS_INT128
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
static_inline void u128_mul_add(u64 a, u64 b, u64 c, u64 *hi, u64 *lo) {
#if YYJSON_HAS_INT128
    u128 m = (u128)a * b + c;
    *hi = (u64)(m >> 64);
    *lo = (u64)(m);
#else
    u64 h, l, t;
    u128_mul(a, b, &h, &l);
    t = l + c;
    h += (u64)(((t < l) | (t < c)));
    *hi = h;
    *lo = t;
#endif
}



#if !YYJSON_DISABLE_WRITER /* writer begin */

/* Check write flag, avoids `always false` warning when disabled. */
#define has_flg(_flg) unlikely(has_wflag(flg, YYJSON_WRITE_##_flg, 0))
#define has_allow(_flg) unlikely(has_wflag(flg, YYJSON_WRITE_ALLOW_##_flg, 1))
static_inline bool has_wflag(yyjson_write_flag flg, yyjson_write_flag chk,
                             bool non_standard) {
#if YYJSON_DISABLE_NON_STANDARD
    if (non_standard) return false;
#endif
    return (flg & chk) != 0;
}

/*==============================================================================
 * MARK: - Integer Writer (Private)
 *
 * The maximum value of uint32_t is 4294967295 (10 digits),
 * these digits are named as 'aabbccddee' here.
 *
 * Although most compilers may convert the "division by constant value" into
 * "multiply and shift", manual conversion can still help some compilers
 * generate fewer and better instructions.
 *
 * Reference:
 * Division by Invariant Integers using Multiplication, 1994.
 * https://gmplib.org/~tege/divcnst-pldi94.pdf
 * Improved division by invariant integers, 2011.
 * https://gmplib.org/~tege/division-paper.pdf
 *============================================================================*/

/** Digit table from 00 to 99. */
yyjson_align(2)
static const char digit_table[200] = {
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
    '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'
};

static_inline u8 *write_u32_len_8(u32 val, u8 *buf) {
    u32 aa, bb, cc, dd, aabb, ccdd;                 /* 8 digits: aabbccdd */
    aabb = (u32)(((u64)val * 109951163) >> 40);     /* (val / 10000) */
    ccdd = val - aabb * 10000;                      /* (val % 10000) */
    aa = (aabb * 5243) >> 19;                       /* (aabb / 100) */
    cc = (ccdd * 5243) >> 19;                       /* (ccdd / 100) */
    bb = aabb - aa * 100;                           /* (aabb % 100) */
    dd = ccdd - cc * 100;                           /* (ccdd % 100) */
    byte_copy_2(buf + 0, digit_table + aa * 2);
    byte_copy_2(buf + 2, digit_table + bb * 2);
    byte_copy_2(buf + 4, digit_table + cc * 2);
    byte_copy_2(buf + 6, digit_table + dd * 2);
    return buf + 8;
}

static_inline u8 *write_u32_len_4(u32 val, u8 *buf) {
    u32 aa, bb;                                     /* 4 digits: aabb */
    aa = (val * 5243) >> 19;                        /* (val / 100) */
    bb = val - aa * 100;                            /* (val % 100) */
    byte_copy_2(buf + 0, digit_table + aa * 2);
    byte_copy_2(buf + 2, digit_table + bb * 2);
    return buf + 4;
}

static_inline u8 *write_u32_len_1_to_8(u32 val, u8 *buf) {
    u32 aa, bb, cc, dd, aabb, bbcc, ccdd, lz;

    if (val < 100) {                                /* 1-2 digits: aa */
        lz = val < 10;                              /* leading zero: 0 or 1 */
        byte_copy_2(buf + 0, digit_table + val * 2 + lz);
        buf -= lz;
        return buf + 2;

    } else if (val < 10000) {                       /* 3-4 digits: aabb */
        aa = (val * 5243) >> 19;                    /* (val / 100) */
        bb = val - aa * 100;                        /* (val % 100) */
        lz = aa < 10;                               /* leading zero: 0 or 1 */
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        return buf + 4;

    } else if (val < 1000000) {                     /* 5-6 digits: aabbcc */
        aa = (u32)(((u64)val * 429497) >> 32);      /* (val / 10000) */
        bbcc = val - aa * 10000;                    /* (val % 10000) */
        bb = (bbcc * 5243) >> 19;                   /* (bbcc / 100) */
        cc = bbcc - bb * 100;                       /* (bbcc % 100) */
        lz = aa < 10;                               /* leading zero: 0 or 1 */
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        byte_copy_2(buf + 4, digit_table + cc * 2);
        return buf + 6;

    } else {                                        /* 7-8 digits: aabbccdd */
        aabb = (u32)(((u64)val * 109951163) >> 40); /* (val / 10000) */
        ccdd = val - aabb * 10000;                  /* (val % 10000) */
        aa = (aabb * 5243) >> 19;                   /* (aabb / 100) */
        cc = (ccdd * 5243) >> 19;                   /* (ccdd / 100) */
        bb = aabb - aa * 100;                       /* (aabb % 100) */
        dd = ccdd - cc * 100;                       /* (ccdd % 100) */
        lz = aa < 10;                               /* leading zero: 0 or 1 */
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        byte_copy_2(buf + 4, digit_table + cc * 2);
        byte_copy_2(buf + 6, digit_table + dd * 2);
        return buf + 8;
    }
}

static_inline u8 *write_u32_len_5_to_8(u32 val, u8 *buf) {
    u32 aa, bb, cc, dd, aabb, bbcc, ccdd, lz;

    if (val < 1000000) {                            /* 5-6 digits: aabbcc */
        aa = (u32)(((u64)val * 429497) >> 32);      /* (val / 10000) */
        bbcc = val - aa * 10000;                    /* (val % 10000) */
        bb = (bbcc * 5243) >> 19;                   /* (bbcc / 100) */
        cc = bbcc - bb * 100;                       /* (bbcc % 100) */
        lz = aa < 10;                               /* leading zero: 0 or 1 */
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        byte_copy_2(buf + 4, digit_table + cc * 2);
        return buf + 6;

    } else {                                        /* 7-8 digits: aabbccdd */
        aabb = (u32)(((u64)val * 109951163) >> 40); /* (val / 10000) */
        ccdd = val - aabb * 10000;                  /* (val % 10000) */
        aa = (aabb * 5243) >> 19;                   /* (aabb / 100) */
        cc = (ccdd * 5243) >> 19;                   /* (ccdd / 100) */
        bb = aabb - aa * 100;                       /* (aabb % 100) */
        dd = ccdd - cc * 100;                       /* (ccdd % 100) */
        lz = aa < 10;                               /* leading zero: 0 or 1 */
        byte_copy_2(buf + 0, digit_table + aa * 2 + lz);
        buf -= lz;
        byte_copy_2(buf + 2, digit_table + bb * 2);
        byte_copy_2(buf + 4, digit_table + cc * 2);
        byte_copy_2(buf + 6, digit_table + dd * 2);
        return buf + 8;
    }
}

static_inline u8 *write_u64(u64 val, u8 *buf) {
    u64 tmp, hgh;
    u32 mid, low;

    if (val < 100000000) {                          /* 1-8 digits */
        buf = write_u32_len_1_to_8((u32)val, buf);
        return buf;

    } else if (val < (u64)100000000 * 100000000) {  /* 9-16 digits */
        hgh = val / 100000000;                      /* (val / 100000000) */
        low = (u32)(val - hgh * 100000000);         /* (val % 100000000) */
        buf = write_u32_len_1_to_8((u32)hgh, buf);
        buf = write_u32_len_8(low, buf);
        return buf;

    } else {                                        /* 17-20 digits */
        tmp = val / 100000000;                      /* (val / 100000000) */
        low = (u32)(val - tmp * 100000000);         /* (val % 100000000) */
        hgh = (u32)(tmp / 10000);                   /* (tmp / 10000) */
        mid = (u32)(tmp - hgh * 10000);             /* (tmp % 10000) */
        buf = write_u32_len_5_to_8((u32)hgh, buf);
        buf = write_u32_len_4(mid, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    }
}



/*==============================================================================
 * MARK: - Number Writer (Private)
 *============================================================================*/

#if !YYJSON_DISABLE_FAST_FP_CONV  /* FP_WRITER */

/** Trailing zero count table for number 0 to 99.
    (generate with misc/make_tables.c) */
static const u8 dec_trailing_zero_table[] = {
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static_inline u8 *write_u64_len_1_to_16(u64 val, u8 *buf) {
    u64 hgh;
    u32 low;
    if (val < 100000000) {                          /* 1-8 digits */
        buf = write_u32_len_1_to_8((u32)val, buf);
        return buf;
    } else {                                        /* 9-16 digits */
        hgh = val / 100000000;                      /* (val / 100000000) */
        low = (u32)(val - hgh * 100000000);         /* (val % 100000000) */
        buf = write_u32_len_1_to_8((u32)hgh, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    }
}

static_inline u8 *write_u64_len_1_to_17(u64 val, u8 *buf) {
    u64 hgh;
    u32 mid, low, one;
    if (val >= (u64)100000000 * 10000000) {         /* len: 16 to 17 */
        hgh = val / 100000000;                      /* (val / 100000000) */
        low = (u32)(val - hgh * 100000000);         /* (val % 100000000) */
        one = (u32)(hgh / 100000000);               /* (hgh / 100000000) */
        mid = (u32)(hgh - (u64)one * 100000000);    /* (hgh % 100000000) */
        *buf = (u8)((u8)one + (u8)'0');
        buf += one > 0;
        buf = write_u32_len_8(mid, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    } else if (val >= (u64)100000000){              /* len: 9 to 15 */
        hgh = val / 100000000;                      /* (val / 100000000) */
        low = (u32)(val - hgh * 100000000);         /* (val % 100000000) */
        buf = write_u32_len_1_to_8((u32)hgh, buf);
        buf = write_u32_len_8(low, buf);
        return buf;
    } else {                                        /* len: 1 to 8 */
        buf = write_u32_len_1_to_8((u32)val, buf);
        return buf;
    }
}

/**
 Write an unsigned integer with a length of 7 to 9 with trailing zero trimmed.
 These digits are named as "abbccddee" here.
 For example, input 123456000, output "123456".
 */
static_inline u8 *write_u32_len_7_to_9_trim(u32 val, u8 *buf) {
    bool lz;
    u32 tz, tz1, tz2;

    u32 abbcc = val / 10000;                        /* (abbccddee / 10000) */
    u32 ddee = val - abbcc * 10000;                 /* (abbccddee % 10000) */
    u32 abb = (u32)(((u64)abbcc * 167773) >> 24);   /* (abbcc / 100) */
    u32 a = (abb * 41) >> 12;                       /* (abb / 100) */
    u32 bb = abb - a * 100;                         /* (abb % 100) */
    u32 cc = abbcc - abb * 100;                     /* (abbcc % 100) */

    /* write abbcc */
    buf[0] = (u8)(a + '0');
    buf += a > 0;
    lz = bb < 10 && a == 0;
    byte_copy_2(buf + 0, digit_table + bb * 2 + lz);
    buf -= lz;
    byte_copy_2(buf + 2, digit_table + cc * 2);

    if (ddee) {
        u32 dd = (ddee * 5243) >> 19;               /* (ddee / 100) */
        u32 ee = ddee - dd * 100;                   /* (ddee % 100) */
        byte_copy_2(buf + 4, digit_table + dd * 2);
        byte_copy_2(buf + 6, digit_table + ee * 2);
        tz1 = dec_trailing_zero_table[dd];
        tz2 = dec_trailing_zero_table[ee];
        tz = ee ? tz2 : (tz1 + 2);
        buf += 8 - tz;
        return buf;
    } else {
        tz1 = dec_trailing_zero_table[bb];
        tz2 = dec_trailing_zero_table[cc];
        tz = cc ? tz2 : (tz1 + tz2);
        buf += 4 - tz;
        return buf;
    }
}

/**
 Write an unsigned integer with a length of 16 or 17 with trailing zero trimmed.
 These digits are named as "abbccddeeffgghhii" here.
 For example, input 1234567890123000, output "1234567890123".
 */
static_inline u8 *write_u64_len_16_to_17_trim(u64 val, u8 *buf) {
    u32 tz, tz1, tz2;

    u32 abbccddee = (u32)(val / 100000000);
    u32 ffgghhii = (u32)(val - (u64)abbccddee * 100000000);
    u32 abbcc = abbccddee / 10000;
    u32 ddee = abbccddee - abbcc * 10000;
    u32 abb = (u32)(((u64)abbcc * 167773) >> 24);   /* (abbcc / 100) */
    u32 a = (abb * 41) >> 12;                       /* (abb / 100) */
    u32 bb = abb - a * 100;                         /* (abb % 100) */
    u32 cc = abbcc - abb * 100;                     /* (abbcc % 100) */
    buf[0] = (u8)(a + '0');
    buf += a > 0;
    byte_copy_2(buf + 0, digit_table + bb * 2);
    byte_copy_2(buf + 2, digit_table + cc * 2);

    if (ffgghhii) {
        u32 dd = (ddee * 5243) >> 19;               /* (ddee / 100) */
        u32 ee = ddee - dd * 100;                   /* (ddee % 100) */
        u32 ffgg = (u32)(((u64)ffgghhii * 109951163) >> 40); /* (val / 10000) */
        u32 hhii = ffgghhii - ffgg * 10000;         /* (val % 10000) */
        u32 ff = (ffgg * 5243) >> 19;               /* (aabb / 100) */
        u32 gg = ffgg - ff * 100;                   /* (aabb % 100) */
        byte_copy_2(buf + 4, digit_table + dd * 2);
        byte_copy_2(buf + 6, digit_table + ee * 2);
        byte_copy_2(buf + 8, digit_table + ff * 2);
        byte_copy_2(buf + 10, digit_table + gg * 2);
        if (hhii) {
            u32 hh = (hhii * 5243) >> 19;           /* (ccdd / 100) */
            u32 ii = hhii - hh * 100;               /* (ccdd % 100) */
            byte_copy_2(buf + 12, digit_table + hh * 2);
            byte_copy_2(buf + 14, digit_table + ii * 2);
            tz1 = dec_trailing_zero_table[hh];
            tz2 = dec_trailing_zero_table[ii];
            tz = ii ? tz2 : (tz1 + 2);
            return buf + 16 - tz;
        } else {
            tz1 = dec_trailing_zero_table[ff];
            tz2 = dec_trailing_zero_table[gg];
            tz = gg ? tz2 : (tz1 + 2);
            return buf + 12 - tz;
        }
    } else {
        if (ddee) {
            u32 dd = (ddee * 5243) >> 19;           /* (ddee / 100) */
            u32 ee = ddee - dd * 100;               /* (ddee % 100) */
            byte_copy_2(buf + 4, digit_table + dd * 2);
            byte_copy_2(buf + 6, digit_table + ee * 2);
            tz1 = dec_trailing_zero_table[dd];
            tz2 = dec_trailing_zero_table[ee];
            tz = ee ? tz2 : (tz1 + 2);
            return buf + 8 - tz;
        } else {
            tz1 = dec_trailing_zero_table[bb];
            tz2 = dec_trailing_zero_table[cc];
            tz = cc ? tz2 : (tz1 + tz2);
            return buf + 4 - tz;
        }
    }
}

/** Write exponent part in range `e-45` to `e38`. */
static_inline u8 *write_f32_exp(i32 exp, u8 *buf) {
    bool lz;
    byte_copy_2(buf, "e-");
    buf += 2 - (exp >= 0);
    exp = exp < 0 ? -exp : exp;
    lz = exp < 10;
    byte_copy_2(buf + 0, digit_table + (u32)exp * 2 + lz);
    return buf + 2 - lz;
}

/** Write exponent part in range `e-324` to `e308`. */
static_inline u8 *write_f64_exp(i32 exp, u8 *buf) {
    byte_copy_2(buf, "e-");
    buf += 2 - (exp >= 0);
    exp = exp < 0 ? -exp : exp;
    if (exp < 100) {
        bool lz = exp < 10;
        byte_copy_2(buf + 0, digit_table + (u32)exp * 2 + lz);
        return buf + 2 - lz;
    } else {
        u32 hi = ((u32)exp * 656) >> 16;    /* exp / 100 */
        u32 lo = (u32)exp - hi * 100;       /* exp % 100 */
        buf[0] = (u8)((u8)hi + (u8)'0');
        byte_copy_2(buf + 1, digit_table + lo * 2);
        return buf + 3;
    }
}

/** Magic number for fast `divide by power of 10`. */
typedef struct {
    u64 p10, mul;
    u32 shr1, shr2;
} div_pow10_magic;

/** Generated with llvm, see https://github.com/llvm/llvm-project/
    blob/main/llvm/lib/Support/DivisionByConstantInfo.cpp */
static const div_pow10_magic div_pow10_table[] = {
    { U64(0x00000000, 0x00000001), U64(0x00000000, 0x00000000), 0,  0  },
    { U64(0x00000000, 0x0000000A), U64(0xCCCCCCCC, 0xCCCCCCCD), 0,  3  },
    { U64(0x00000000, 0x00000064), U64(0x28F5C28F, 0x5C28F5C3), 2,  2  },
    { U64(0x00000000, 0x000003E8), U64(0x20C49BA5, 0xE353F7CF), 3,  4  },
    { U64(0x00000000, 0x00002710), U64(0x346DC5D6, 0x3886594B), 0,  11 },
    { U64(0x00000000, 0x000186A0), U64(0x0A7C5AC4, 0x71B47843), 5,  7  },
    { U64(0x00000000, 0x000F4240), U64(0x431BDE82, 0xD7B634DB), 0,  18 },
    { U64(0x00000000, 0x00989680), U64(0xD6BF94D5, 0xE57A42BD), 0,  23 },
    { U64(0x00000000, 0x05F5E100), U64(0xABCC7711, 0x8461CEFD), 0,  26 },
    { U64(0x00000000, 0x3B9ACA00), U64(0x0044B82F, 0xA09B5A53), 9,  11 },
    { U64(0x00000002, 0x540BE400), U64(0xDBE6FECE, 0xBDEDD5BF), 0,  33 },
    { U64(0x00000017, 0x4876E800), U64(0xAFEBFF0B, 0xCB24AAFF), 0,  36 },
    { U64(0x000000E8, 0xD4A51000), U64(0x232F3302, 0x5BD42233), 0,  37 },
    { U64(0x00000918, 0x4E72A000), U64(0x384B84D0, 0x92ED0385), 0,  41 },
    { U64(0x00005AF3, 0x107A4000), U64(0x0B424DC3, 0x5095CD81), 0,  42 },
    { U64(0x00038D7E, 0xA4C68000), U64(0x00024075, 0xF3DCEAC3), 15, 20 },
    { U64(0x002386F2, 0x6FC10000), U64(0x39A5652F, 0xB1137857), 0,  51 },
    { U64(0x01634578, 0x5D8A0000), U64(0x00005C3B, 0xD5191B53), 17, 22 },
    { U64(0x0DE0B6B3, 0xA7640000), U64(0x000049C9, 0x7747490F), 18, 24 },
    { U64(0x8AC72304, 0x89E80000), U64(0x760F253E, 0xDB4AB0d3), 0,  62 },
};

/** Divide a number by power of 10. */
static_inline void div_pow10(u64 num, u32 exp, u64 *div, u64 *mod, u64 *p10) {
    u64 hi, lo;
    div_pow10_magic m = div_pow10_table[exp];
    u128_mul(num >> m.shr1, m.mul, &hi, &lo);
    *div = hi >> m.shr2;
    *mod = num - (*div * m.p10);
    *p10 = m.p10;
}

/** Multiplies 64-bit integer and returns highest 64-bit rounded value. */
static_inline u32 u64_round_to_odd(u64 u, u32 cp) {
    u64 hi, lo;
    u32 y_hi, y_lo;
    u128_mul(cp, u, &hi, &lo);
    y_hi = (u32)hi;
    y_lo = (u32)(lo >> 32);
    return y_hi | (y_lo > 1);
}

/** Multiplies 128-bit integer and returns highest 64-bit rounded value. */
static_inline u64 u128_round_to_odd(u64 hi, u64 lo, u64 cp) {
    u64 x_hi, x_lo, y_hi, y_lo;
    u128_mul(cp, lo, &x_hi, &x_lo);
    u128_mul_add(cp, hi, x_hi, &y_hi, &y_lo);
    return y_hi | (y_lo > 1);
}

/** Convert f32 from binary to decimal (shortest but may have trailing zeros).
    The input should not be 0, inf or nan. */
static_inline void f32_bin_to_dec(u32 sig_raw, u32 exp_raw,
                                  u32 sig_bin, i32 exp_bin,
                                  u32 *sig_dec, i32 *exp_dec) {

    bool is_even, irregular, round_up, trim;
    bool u0_inside, u1_inside, w0_inside, w1_inside;
    u64 p10_hi, p10_lo, hi, lo;
    u32 s, sp, cb, cbl, cbr, vb, vbl, vbr, upper, lower, mid;
    i32 k, h;

    /* Fast path, see f64_bin_to_dec(). */
    while (likely(sig_raw)) {
        u32 mod, dec, add_1, add_10, s_hi, s_lo;
        u32 c, half_ulp, t0, t1;

        /* k = floor(exp_bin * log10(2)); */
        /* h = exp_bin + floor(log2(10) * -k); (h = 0/1/2/3) */
        k = (i32)(exp_bin * 315653) >> 20;
        h = exp_bin + ((-k * 217707) >> 16);
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);

        /* sig_bin << (1/2/3/4) */
        cb = sig_bin << (h + 1);
        u128_mul(cb, p10_hi, &hi, &lo);
        s_hi = (u32)(hi);
        s_lo = (u32)(lo >> 32);
        mod = s_hi % 10;
        dec = s_hi - mod;

        /* right shift 4 to fit in u32 */
        c = (mod << (32 - 4)) | (s_lo >> 4);
        half_ulp = (u32)(p10_hi >> (32 + 4 - h));

        /* check w1, u0, w0 range */
        w1_inside = (s_lo >= ((u32)1 << 31));
        if (unlikely(s_lo == ((u32)1 << 31))) break;
        u0_inside = (half_ulp >= c);
        if (unlikely(half_ulp == c)) break;
        t0 = (u32)10 << (32 - 4);
        t1 = c + half_ulp;
        w0_inside = (t1 >= t0);
        if (unlikely(t0 - t1 <= (u32)1)) break;

        trim = (u0_inside | w0_inside);
        add_10 = (w0_inside ? 10 : 0);
        add_1 = mod + w1_inside;
        *sig_dec = dec + (trim ? add_10 : add_1);
        *exp_dec = k;
        return;
    }

    /* Schubfach algorithm, see f64_bin_to_dec(). */
    irregular = (sig_raw == 0 && exp_raw > 1);
    is_even = !(sig_bin & 1);
    cbl = 4 * sig_bin - 2 + irregular;
    cb  = 4 * sig_bin;
    cbr = 4 * sig_bin + 2;

    /* k = floor(exp_bin * log10(2) + (irregular ? log10(3.0 / 4.0) : 0)); */
    /* h = exp_bin + floor(log2(10) * -k) + 1; (h = 1/2/3/4) */
    k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
    h = exp_bin + ((-k * 217707) >> 16) + 1;
    pow10_table_get_sig(-k, &p10_hi, &p10_lo);
    p10_hi += 1;

    vbl = u64_round_to_odd(p10_hi, cbl << h);
    vb  = u64_round_to_odd(p10_hi, cb  << h);
    vbr = u64_round_to_odd(p10_hi, cbr << h);
    lower = vbl + !is_even;
    upper = vbr - !is_even;

    s = vb / 4;
    if (s >= 10) {
        sp = s / 10;
        u0_inside = (lower <= 40 * sp);
        w0_inside = (upper >= 40 * sp + 40);
        if (u0_inside != w0_inside) {
            *sig_dec = sp * 10 + (w0_inside ? 10 : 0);
            *exp_dec = k;
            return;
        }
    }
    u1_inside = (lower <= 4 * s);
    w1_inside = (upper >= 4 * s + 4);
    mid = 4 * s + 2;
    round_up = (vb > mid) || (vb == mid && (s & 1) != 0);
    *sig_dec = s + ((u1_inside != w1_inside) ? w1_inside : round_up);
    *exp_dec = k;
}

/** Convert f64 from binary to decimal (shortest but may have trailing zeros).
    The input should not be 0, inf or nan. */
static_inline void f64_bin_to_dec(u64 sig_raw, u32 exp_raw,
                                  u64 sig_bin, i32 exp_bin,
                                  u64 *sig_dec, i32 *exp_dec) {

    bool is_even, irregular, round_up, trim;
    bool u0_inside, u1_inside, w0_inside, w1_inside;
    u64 s, sp, cb, cbl, cbr, vb, vbl, vbr, p10_hi, p10_lo, upper, lower, mid;
    i32 k, h;

    /*
     Fast path:
     For regular spacing significand 'c', there are 4 candidates:

             u0             u1 c  w1                            w0
     ----|----|----|----|----|-*--|----|----|----|----|----|----|----|----
         9    0    1    2    3    4    5    6    7    8    9    0    1
           |___________________|___________________|
                             1ulp

     The `1ulp` is in the range [1.0, 10.0).
     If (c - 0.5ulp < u0), trim the last digit and round down.
     If (c + 0.5ulp > w0), trim the last digit and round up.
     If (c - 0.5ulp < u1), round down.
     If (c + 0.5ulp > w1), round up.
     */
    while (likely(sig_raw)) {
        u64 mod, dec, add_1, add_10, s_hi, s_lo;
        u64 c, half_ulp, t0, t1;

        /* k = floor(exp_bin * log10(2)); */
        /* h = exp_bin + floor(log2(10) * -k); (h = 0/1/2/3) */
        k = (i32)(exp_bin * 315653) >> 20;
        h = exp_bin + ((-k * 217707) >> 16);
        pow10_table_get_sig(-k, &p10_hi, &p10_lo);

        /* sig_bin << (1/2/3/4) */
        cb = sig_bin << (h + 1);
        u128_mul(cb, p10_lo, &s_hi, &s_lo);
        u128_mul_add(cb, p10_hi, s_hi, &s_hi, &s_lo);
        mod = s_hi % 10;
        dec = s_hi - mod;

        /* right shift 4 to fit in u64 */
        c = (mod << (64 - 4)) | (s_lo >> 4);
        half_ulp = p10_hi >> (4 - h);

        /* check w1, u0, w0 range */
        w1_inside = (s_lo >= ((u64)1 << 63));
        if (unlikely(s_lo == ((u64)1 << 63))) break;
        u0_inside = (half_ulp >= c);
        if (unlikely(half_ulp == c)) break;
        t0 = ((u64)10 << (64 - 4));
        t1 = c + half_ulp;
        w0_inside = (t1 >= t0);
        if (unlikely(t0 - t1 <= (u64)1)) break;

        trim = (u0_inside | w0_inside);
        add_10 = (w0_inside ? 10 : 0);
        add_1 = mod + w1_inside;
        *sig_dec = dec + (trim ? add_10 : add_1);
        *exp_dec = k;
        return;
    }

    /*
     Schubfach algorithm:
     Raffaello Giulietti, The Schubfach way to render doubles, 2022.
     https://drive.google.com/file/d/1gp5xv4CAa78SVgCeWfGqqI4FfYYYuNFb (Paper)
     https://github.com/openjdk/jdk/pull/3402 (Java implementation)
     https://github.com/abolz/Drachennest (C++ implementation)
     */
    irregular = (sig_raw == 0 && exp_raw > 1);
    is_even = !(sig_bin & 1);
    cbl = 4 * sig_bin - 2 + irregular;
    cb  = 4 * sig_bin;
    cbr = 4 * sig_bin + 2;

    /* k = floor(exp_bin * log10(2) + (irregular ? log10(3.0 / 4.0) : 0)); */
    /* h = exp_bin + floor(log2(10) * -k) + 1; (h = 1/2/3/4) */
    k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
    h = exp_bin + ((-k * 217707) >> 16) + 1;
    pow10_table_get_sig(-k, &p10_hi, &p10_lo);
    p10_lo += 1;

    vbl = u128_round_to_odd(p10_hi, p10_lo, cbl << h);
    vb  = u128_round_to_odd(p10_hi, p10_lo, cb  << h);
    vbr = u128_round_to_odd(p10_hi, p10_lo, cbr << h);
    lower = vbl + !is_even;
    upper = vbr - !is_even;

    s = vb / 4;
    if (s >= 10) {
        sp = s / 10;
        u0_inside = (lower <= 40 * sp);
        w0_inside = (upper >= 40 * sp + 40);
        if (u0_inside != w0_inside) {
            *sig_dec = sp * 10 + (w0_inside ? 10 : 0);
            *exp_dec = k;
            return;
        }
    }
    u1_inside = (lower <= 4 * s);
    w1_inside = (upper >= 4 * s + 4);
    mid = 4 * s + 2;
    round_up = (vb > mid) || (vb == mid && (s & 1) != 0);
    *sig_dec = s + ((u1_inside != w1_inside) ? w1_inside : round_up);
    *exp_dec = k;
}

/** Convert f64 from binary to decimal (fast but not the shortest).
    The input should not be 0, inf, nan. */
static_inline void f64_bin_to_dec_fast(u64 sig_raw, u32 exp_raw,
                                       u64 sig_bin, i32 exp_bin,
                                       u64 *sig_dec, i32 *exp_dec,
                                       bool *round_up) {
    u64 cb, p10_hi, p10_lo, s_hi, s_lo;
    i32 k, h;
    bool irregular, u;

    irregular = (sig_raw == 0 && exp_raw > 1);

    /* k = floor(exp_bin * log10(2) + (irregular ? log10(3.0 / 4.0) : 0)); */
    /* h = exp_bin + floor(log2(10) * -k) + 1; (h = 1/2/3/4) */
    k = (i32)(exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
    h = exp_bin + ((-k * 217707) >> 16);
    pow10_table_get_sig(-k, &p10_hi, &p10_lo);

    /* sig_bin << (1/2/3/4) */
    cb = sig_bin << (h + 1);
    u128_mul(cb, p10_lo, &s_hi, &s_lo);
    u128_mul_add(cb, p10_hi, s_hi, &s_hi, &s_lo);

    /* round up */
    u = s_lo >= (irregular ? U64(0x55555555, 0x55555555) : ((u64)1 << 63));

    *sig_dec = s_hi + u;
    *exp_dec = k;
    *round_up = u;
    return;
}

/** Write inf/nan if allowed. */
static_inline u8 *write_inf_or_nan(u8 *buf, yyjson_write_flag flg,
                                   u64 sig_raw, bool sign) {
    if (has_flg(INF_AND_NAN_AS_NULL)) {
        byte_copy_4(buf, "null");
        return buf + 4;
    }
    if (has_allow(INF_AND_NAN)) {
        if (sig_raw == 0) {
            buf[0] = '-';
            buf += sign;
            byte_copy_8(buf, "Infinity");
            return buf + 8;
        } else {
            byte_copy_4(buf, "NaN");
            return buf + 3;
        }
    }
    return NULL;
}

/**
 Write a float number (requires 40 bytes buffer).
 We follow the ECMAScript specification for printing floating-point numbers,
 similar to `Number.prototype.toString()`, but with the following changes:
 1. Keep the negative sign of `-0.0` to preserve input information.
 2. Keep decimal point to indicate the number is floating point.
 3. Remove positive sign in the exponent part.
 */
static_noinline u8 *write_f32_raw(u8 *buf, u64 raw_f64,
                                  yyjson_write_flag flg) {
    u32 sig_bin, sig_dec, sig_raw;
    i32 exp_bin, exp_dec, sig_len, dot_ofs;
    u32 exp_raw, raw;
    u8 *end;
    bool sign;

    /* cast double to float */
    //raw = f32_to_bits(f64_to_f32(f64_from_bits(raw_f64)));
    raw = *(u32*)&raw_f64;// modify code to  get correct result

    /* decode raw bytes from IEEE-754 double format. */
    sign = (bool)(raw >> (F32_BITS - 1));
    sig_raw = raw & F32_SIG_MASK;
    exp_raw = (raw & F32_EXP_MASK) >> F32_SIG_BITS;

    /* return inf or nan */
    if (unlikely(exp_raw == ((u32)1 << F32_EXP_BITS) - 1)) {
        return write_inf_or_nan(buf, flg, sig_raw, sign);
    }

    /* add sign for all finite number */
    buf[0] = '-';
    buf += sign;

    /* return zero */
    if ((raw << 1) == 0) {
        byte_copy_4(buf, "0.0");
        return buf + 3;
    }

    if (likely(exp_raw != 0)) {
        /* normal number */
        sig_bin = sig_raw | ((u32)1 << F32_SIG_BITS);
        exp_bin = (i32)exp_raw - F32_EXP_BIAS - F32_SIG_BITS;

        /* fast path for small integer number without fraction */
        if ((-F32_SIG_BITS <= exp_bin && exp_bin <= 0) &&
            (u64_tz_bits(sig_bin) >= (u32)-exp_bin)) {
            sig_dec = sig_bin >> -exp_bin; /* range: [1, 0xFFFFFF] */
            buf = write_u32_len_1_to_8(sig_dec, buf);
            byte_copy_2(buf, ".0");
            return buf + 2;
        }

        /* binary to decimal */
        f32_bin_to_dec(sig_raw, exp_raw, sig_bin, exp_bin, &sig_dec, &exp_dec);

        /* the sig length is 7 or 9 */
        sig_len = 7 + (sig_dec >= (u32)10000000) + (sig_dec >= (u32)100000000);

        /* the decimal point offset relative to the first digit */
        dot_ofs = sig_len + exp_dec;

        if (-6 < dot_ofs && dot_ofs <= 21) {
            i32 num_sep_pos, dot_set_pos, pre_ofs;
            u8 *num_hdr, *num_end, *num_sep, *dot_end;
            bool no_pre_zero;

            /* fill zeros */
            memset(buf, '0', 32);

            /* not prefixed with zero, e.g. 1.234, 1234.0 */
            no_pre_zero = (dot_ofs > 0);

            /* write the number as digits */
            pre_ofs = no_pre_zero ? 0 : (2 - dot_ofs);
            num_hdr = buf + pre_ofs;
            num_end = write_u32_len_7_to_9_trim(sig_dec, num_hdr);

            /* seperate these digits to leave a space for dot */
            num_sep_pos = no_pre_zero ? dot_ofs : 0;
            num_sep = num_hdr + num_sep_pos;
            byte_move_8(num_sep + no_pre_zero, num_sep);
            num_end += no_pre_zero;

            /* write the dot */
            dot_set_pos = yyjson_max(dot_ofs, 1);
            buf[dot_set_pos] = '.';

            /* return the ending */
            dot_end = buf + dot_ofs + 2;
            return yyjson_max(dot_end, num_end);

        } else {
            /* write with scientific notation, e.g. 1.234e56 */
            end = write_u32_len_7_to_9_trim(sig_dec, buf + 1);
            end -= (end == buf + 2); /* remove '.0', e.g. 2.0e34 -> 2e34 */
            exp_dec += sig_len - 1;
            buf[0] = buf[1];
            buf[1] = '.';
            return write_f32_exp(exp_dec, end);
        }

    } else {
        /* subnormal number */
        sig_bin = sig_raw;
        exp_bin = 1 - F32_EXP_BIAS - F32_SIG_BITS;

        /* binary to decimal */
        f32_bin_to_dec(sig_raw, exp_raw, sig_bin, exp_bin, &sig_dec, &exp_dec);

        /* write significand part */
        end = write_u32_len_1_to_8(sig_dec, buf + 1);
        buf[0] = buf[1];
        buf[1] = '.';
        exp_dec += (i32)(end - buf) - 2;

        /* trim trailing zeros */
        end -= *(end - 1) == '0'; /* branchless for last zero */
        end -= *(end - 1) == '0'; /* branchless for second last zero */
        while (*(end - 1) == '0') end--; /* for unlikely more zeros */
        end -= *(end - 1) == '.'; /* remove dot, e.g. 2.e-321 -> 2e-321 */

        /* write exponent part */
        return write_f32_exp(exp_dec, end);
    }
}

/**
 Write a double number (requires 40 bytes buffer).
 We follow the ECMAScript specification for printing floating-point numbers,
 similar to `Number.prototype.toString()`, but with the following changes:
 1. Keep the negative sign of `-0.0` to preserve input information.
 2. Keep decimal point to indicate the number is floating point.
 3. Remove positive sign in the exponent part.
 */
static_noinline u8 *write_f64_raw(u8 *buf, u64 raw, yyjson_write_flag flg) {
    u64 sig_bin, sig_dec, sig_raw;
    i32 exp_bin, exp_dec, sig_len, dot_ofs;
    u32 exp_raw;
    u8 *end;
    bool sign;

    /* decode raw bytes from IEEE-754 double format. */
    sign = (bool)(raw >> (F64_BITS - 1));
    sig_raw = raw & F64_SIG_MASK;
    exp_raw = (u32)((raw & F64_EXP_MASK) >> F64_SIG_BITS);

    /* return inf or nan */
    if (unlikely(exp_raw == ((u32)1 << F64_EXP_BITS) - 1)) {
        return write_inf_or_nan(buf, flg, sig_raw, sign);
    }

    /* add sign for all finite number */
    buf[0] = '-';
    buf += sign;

    /* return zero */
    if ((raw << 1) == 0) {
        byte_copy_4(buf, "0.0");
        return buf + 3;
    }

    if (likely(exp_raw != 0)) {
        /* normal number */
        sig_bin = sig_raw | ((u64)1 << F64_SIG_BITS);
        exp_bin = (i32)exp_raw - F64_EXP_BIAS - F64_SIG_BITS;

        /* fast path for small integer number without fraction */
        if ((-F64_SIG_BITS <= exp_bin && exp_bin <= 0) &&
            (u64_tz_bits(sig_bin) >= (u32)-exp_bin)) {
            sig_dec = sig_bin >> -exp_bin; /* range: [1, 0x1FFFFFFFFFFFFF] */
            buf = write_u64_len_1_to_16(sig_dec, buf);
            byte_copy_2(buf, ".0");
            return buf + 2;
        }

        /* binary to decimal */
        f64_bin_to_dec(sig_raw, exp_raw, sig_bin, exp_bin, &sig_dec, &exp_dec);

        /* the sig length is 16 or 17 */
        sig_len = 16 + (sig_dec >= (u64)100000000 * 100000000);

        /* the decimal point offset relative to the first digit */
        dot_ofs = sig_len + exp_dec;

        if (-6 < dot_ofs && dot_ofs <= 21) {
            i32 num_sep_pos, dot_set_pos, pre_ofs;
            u8 *num_hdr, *num_end, *num_sep, *dot_end;
            bool no_pre_zero;

            /* fill zeros */
            memset(buf, '0', 32);

            /* not prefixed with zero, e.g. 1.234, 1234.0 */
            no_pre_zero = (dot_ofs > 0);

            /* write the number as digits */
            pre_ofs = no_pre_zero ? 0 : (2 - dot_ofs);
            num_hdr = buf + pre_ofs;
            num_end = write_u64_len_16_to_17_trim(sig_dec, num_hdr);

            /* seperate these digits to leave a space for dot */
            num_sep_pos = no_pre_zero ? dot_ofs : 0;
            num_sep = num_hdr + num_sep_pos;
            byte_move_16(num_sep + no_pre_zero, num_sep);
            num_end += no_pre_zero;

            /* write the dot */
            dot_set_pos = yyjson_max(dot_ofs, 1);
            buf[dot_set_pos] = '.';

            /* return the ending */
            dot_end = buf + dot_ofs + 2;
            return yyjson_max(dot_end, num_end);

        } else {
            /* write with scientific notation, e.g. 1.234e56 */
            end = write_u64_len_16_to_17_trim(sig_dec, buf + 1);
            end -= (end == buf + 2); /* remove '.0', e.g. 2.0e34 -> 2e34 */
            exp_dec += sig_len - 1;
            buf[0] = buf[1];
            buf[1] = '.';
            return write_f64_exp(exp_dec, end);
        }

    } else {
        /* subnormal number */
        sig_bin = sig_raw;
        exp_bin = 1 - F64_EXP_BIAS - F64_SIG_BITS;

        /* binary to decimal */
        f64_bin_to_dec(sig_raw, exp_raw, sig_bin, exp_bin, &sig_dec, &exp_dec);

        /* write significand part */
        end = write_u64_len_1_to_17(sig_dec, buf + 1);
        buf[0] = buf[1];
        buf[1] = '.';
        exp_dec += (i32)(end - buf) - 2;

        /* trim trailing zeros */
        end -= *(end - 1) == '0'; /* branchless for last zero */
        end -= *(end - 1) == '0'; /* branchless for second last zero */
        while (*(end - 1) == '0') end--; /* for unlikely more zeros */
        end -= *(end - 1) == '.'; /* remove dot, e.g. 2.e-321 -> 2e-321 */

        /* write exponent part */
        return write_f64_exp(exp_dec, end);
    }
}

/**
 Write a double number using fixed-point notation (requires 40 bytes buffer).

 We follow the ECMAScript specification for printing floating-point numbers,
 similar to `Number.prototype.toFixed(prec)`, but with the following changes:
 1. Keep the negative sign of `-0.0` to preserve input information.
 2. Keep decimal point to indicate the number is floating point.
 3. Remove positive sign in the exponent part.
 4. Remove trailing zeros and reduce unnecessary precision.
 */
static_noinline u8 *write_f64_raw_fixed(u8 *buf, u64 raw, yyjson_write_flag flg,
                                        u32 prec) {
    u64 sig_bin, sig_dec, sig_raw;
    i32 exp_bin, exp_dec, sig_len, dot_ofs;
    u32 exp_raw;
    u8 *end;
    bool sign;

    /* decode raw bytes from IEEE-754 double format. */
    sign = (bool)(raw >> (F64_BITS - 1));
    sig_raw = raw & F64_SIG_MASK;
    exp_raw = (u32)((raw & F64_EXP_MASK) >> F64_SIG_BITS);

    /* return inf or nan */
    if (unlikely(exp_raw == ((u32)1 << F64_EXP_BITS) - 1)) {
        return write_inf_or_nan(buf, flg, sig_raw, sign);
    }

    /* add sign for all finite number */
    buf[0] = '-';
    buf += sign;

    /* return zero */
    if ((raw << 1) == 0) {
        byte_copy_4(buf, "0.0");
        return buf + 3;
    }

    if (likely(exp_raw != 0)) {
        /* normal number */
        sig_bin = sig_raw | ((u64)1 << F64_SIG_BITS);
        exp_bin = (i32)exp_raw - F64_EXP_BIAS - F64_SIG_BITS;

        /* fast path for small integer number without fraction */
        if ((-F64_SIG_BITS <= exp_bin && exp_bin <= 0) &&
            (u64_tz_bits(sig_bin) >= (u32)-exp_bin)) {
            sig_dec = sig_bin >> -exp_bin; /* range: [1, 0x1FFFFFFFFFFFFF] */
            buf = write_u64_len_1_to_16(sig_dec, buf);
            byte_copy_2(buf, ".0");
            return buf + 2;
        }

        /* only `fabs(num) < 1e21` are processed here. */
        if ((raw << 1) < (U64(0x444B1AE4, 0xD6E2EF50) << 1)) {
            i32 num_sep_pos, dot_set_pos, pre_ofs;
            u8 *num_hdr, *num_end, *num_sep;
            bool round_up, no_pre_zero;

            /* binary to decimal */
            f64_bin_to_dec_fast(sig_raw, exp_raw, sig_bin, exp_bin,
                                &sig_dec, &exp_dec, &round_up);

            /* the sig length is 16 or 17 */
            sig_len = 16 + (sig_dec >= (u64)100000000 * 100000000);

            /* limit the length of digits after the decimal point */
            if (exp_dec < -1) {
                i32 sig_len_cut = -exp_dec - (i32)prec;
                if (sig_len_cut > sig_len) {
                    byte_copy_4(buf, "0.0");
                    return buf + 3;
                }
                if (sig_len_cut > 0) {
                    u64 div, mod, p10;

                    /* remove round up */
                    sig_dec -= round_up;
                    sig_len = 16 + (sig_dec >= (u64)100000000 * 100000000);

                    /* cut off some digits */
                    div_pow10(sig_dec, (u32)sig_len_cut, &div, &mod, &p10);

                    /* add round up */
                    sig_dec = div + (mod >= p10 / 2);

                    /* update exp and sig length */
                    exp_dec += sig_len_cut;
                    sig_len -= sig_len_cut;
                    sig_len += (sig_len >= 0) &&
                               (sig_dec >= div_pow10_table[sig_len].p10);
                }
                if (sig_len <= 0) {
                    byte_copy_4(buf, "0.0");
                    return buf + 3;
                }
            }

            /* fill zeros */
            memset(buf, '0', 32);

            /* the decimal point offset relative to the first digit */
            dot_ofs = sig_len + exp_dec;

            /* not prefixed with zero, e.g. 1.234, 1234.0 */
            no_pre_zero = (dot_ofs > 0);

            /* write the number as digits */
            pre_ofs = no_pre_zero ? 0 : (1 - dot_ofs);
            num_hdr = buf + pre_ofs;
            num_end = write_u64_len_1_to_17(sig_dec, num_hdr);

            /* seperate these digits to leave a space for dot */
            num_sep_pos = no_pre_zero ? dot_ofs : -dot_ofs;
            num_sep = buf + num_sep_pos;
            byte_move_16(num_sep + 1, num_sep);
            num_end += (exp_dec < 0);

            /* write the dot */
            dot_set_pos = yyjson_max(dot_ofs, 1);
            buf[dot_set_pos] = '.';

            /* remove trailing zeros */
            buf += dot_set_pos + 2;
            buf = yyjson_max(buf, num_end);
            buf -= *(buf - 1) == '0'; /* branchless for last zero */
            buf -= *(buf - 1) == '0'; /* branchless for second last zero */
            while (*(buf - 1) == '0') buf--; /* for unlikely more zeros */
            buf += *(buf - 1) == '.'; /* keep a zero after dot */
            return buf;

        } else {
            /* binary to decimal */
            f64_bin_to_dec(sig_raw, exp_raw, sig_bin, exp_bin,
                           &sig_dec, &exp_dec);

            /* the sig length is 16 or 17 */
            sig_len = 16 + (sig_dec >= (u64)100000000 * 100000000);

            /* write with scientific notation, e.g. 1.234e56 */
            end = write_u64_len_16_to_17_trim(sig_dec, buf + 1);
            end -= (end == buf + 2); /* remove '.0', e.g. 2.0e34 -> 2e34 */
            exp_dec += sig_len - 1;
            buf[0] = buf[1];
            buf[1] = '.';
            return write_f64_exp(exp_dec, end);
        }
    } else {
        /* subnormal number */
        byte_copy_4(buf, "0.0");
        return buf + 3;
    }
}

#else /* FP_WRITER */
#endif /* FP_WRITER */

#endif /* YYJSON_DISABLE_WRITER */
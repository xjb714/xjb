#include <stdint.h>
#include <string.h> // memcpy

// check cpu has neon or sse2 ; most all CPU has neon or sse2;

#if defined(__aarch64__) && defined(__ARM_NEON__)
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


/*
    author : Xiang Jun Bo
    link : https://github.com/xjb714/xjb
    date : 2025-11-10

    this file contains the implementation of dtoa_xjb64 and ftoa_xjb32 function.
    64-bit version : dtoa_xjb64 ; print a double number to buffer string
    32-bit version : ftoa_xjb32 ; print a float number to buffer string
    dtoa_xjb64(double x, char* buf) == xjb64_32::xjb64(double x, char* buf)
    ftoa_xjb32(float x, char* buf) == xjb64_32::xjb32(float x, char* buf)
    these two functions only run on little-endian system.

    (1)dtoa_xjb64
    function : char* dtoa_xjb64(double x, char* buf)
    input    : double x  , A double number
               char* buf , At least 33byte
    output   : The function returns a pointer to the end of the string. The string is null-terminated.

    (2)ftoa_xjb32
    function : char* ftoa_xjb32(float x, char* buf)
    input    : float x  , A float number
               char* buf , At least 24byte
    output   : The function returns a pointer to the end of the string. The string is null-terminated.


    dtoa_xjb64 is a function that converts a double number to a string that satisfies the requirements of the Steele & White algorithm.
    ftoa_xjb32 is a function that converts a float number to a string that satisfies the requirements of the Steele & White algorithm.

    hera are some expample for dtoa_xjb64 function :
    example : input   =>   output
              123     =>   "123.0"
              100     =>   "100.0"
              -123    =>   "-123.0"
              123.456 =>   "123.456"
              0.001   =>   "0.001"
              0.00123 =>   "0.00123"
              1e-10   =>   "1e-10"
              1e-100  =>   "1e-100"
              0.0001  =>   "1e-04"
              1234567890123456 => "1234567890123456.0"
              1e20    =>   "1e+20"
              1e-20   =>   "1e-20"
              1e100   =>   "1e+100"
              1.2e-10 =>   "1.2e-10"
              0       =>   "0.0"
              NaN     =>   "NaN"
              Inf     =>   "Inf"

    In the arm platform, dtoa_xjb64 uses the neon instruction to accelerate the conversion.
    In the x64 platform, dtoa_xjb64 uses the sse2 instruction to accelerate the conversion.
    If the cpu not contains neon or sse2, dtoa_xjb64 performances will be poor.

    On the x64 platform, the performance of this code may not be optimal when compiled by the gcc compiler or the clang compiler.
    Under the gcc compiler, due to certain conditional move statements being compiled into branch instructions,
    a large number of branch prediction failures occur, which seriously affects performance.
    Under the clang compiler, the performance is very poor, and the cause is still under investigation.
    Only under the intel compiler icpx does the performance meet expectations.
    I don't know how to force the conditional move statement to be compiled into the cmov instruction.
    The incorrect optimization of the compiler has led to performance not meeting expectations. If you know how to solve it, please provide feedback.

    here are the performance test results on the x64 platform AMD R7-7840H and Ubuntu 24.04 :
    compiler options          : -O3 -march=native  // enable sse4.1
                icpx 2025.0.4 : 9.7-10.2ns  37.5-38.5cycle ipc=3.48  branch-miss=0.13%
                clang 18.1.3  : 16.2-17ns   61.6-62.5cycle ipc=2.13  branch-miss=0.15%
                gcc 13.3      : 12.2-13.1ns 46.7-50cycle   ipc=3.28  branch-miss=0.7%-1.22%

    compiler options          : -O3
                icpx 2025.0.4 : 10.5-10.8ns 39.5-41.5cycle ipc=3.57  branch-miss=0.15%
                clang 18.1.3  : 16.8-17.5ns 63.4-66cycle   ipc=2.25  branch-miss=0.15%
                gcc 13.3      : 11.6-12.5ns 45.2-47cycle   ipc=3.56  branch-miss=0.7%-1.16%

    for clang compiler, the performance is very poor, the ipc value is very low.
    for gcc compiler, the performance is poor, and the branch-miss rate is high.
    only icpx compiler has good performance, and the ipc value is high(the instruction-level parallelism is high), and the branch-miss rate is low.

    The icpx compiler generates the best code : (1)lower instructions, (2)higher ipc, (3)low branch-miss rate.

    In summary, the performance of this code is not optimal, and the cause is still under investigation.

    In fact , this code can use AVX512F instruction to accelerate the conversion, but the performance is only 3-5% faster than the current implementation.
    So , I don't write the AVX512F code in this code.

    This code can be used as a reference implementation for the Steele & White algorithm, and can be further optimized for better performance.
    Steele & White algorithm satisfies the following requirements: (1) lossless (2)shortness (3)correct round.

    If you have any questions or suggestions, please feel free to contact me.

    note : dtoa_xjb64 is a 64-bit version of xjb, which is a 32-bit version not completed.

*/

// Compiler Macros code from https://github.com/ibireme/c_numconv_benchmark/blob/master/vendor/yy_double/yy_double.c
/*==============================================================================
 * Compiler Macros
 *============================================================================*/

/* compiler builtin check (since gcc 10.0, clang 2.6, icc 2021) */
#ifndef yy_has_builtin
#   ifdef __has_builtin
#       define yy_has_builtin(x) __has_builtin(x)
#   else
#       define yy_has_builtin(x) 0
#   endif
#endif

/* compiler attribute check (since gcc 5.0, clang 2.9, icc 17) */
#ifndef yy_has_attribute
#   ifdef __has_attribute
#       define yy_has_attribute(x) __has_attribute(x)
#   else
#       define yy_has_attribute(x) 0
#   endif
#endif

/* include check (since gcc 5.0, clang 2.7, icc 16, msvc 2017 15.3) */
#ifndef yy_has_include
#   ifdef __has_include
#       define yy_has_include(x) __has_include(x)
#   else
#       define yy_has_include(x) 0
#   endif
#endif

/* inline for compiler */
#ifndef yy_inline
#   if _MSC_VER >= 1200
#       define yy_inline __forceinline
#   elif defined(_MSC_VER)
#       define yy_inline __inline
#   elif yy_has_attribute(always_inline) || __GNUC__ >= 4
#       define yy_inline __inline__ __attribute__((always_inline))
#   elif defined(__clang__) || defined(__GNUC__)
#       define yy_inline __inline__
#   elif defined(__cplusplus) || (__STDC__ >= 1 && __STDC_VERSION__ >= 199901L)
#       define yy_inline inline
#   else
#       define yy_inline
#   endif
#endif

/* noinline for compiler */
#ifndef yy_noinline
#   if _MSC_VER >= 1200
#       define yy_noinline __declspec(noinline)
#   elif yy_has_attribute(noinline) || __GNUC__ >= 4
#       define yy_noinline __attribute__((noinline))
#   else
#       define yy_noinline
#   endif
#endif

/* align for compiler */
#ifndef yy_align
#   if defined(_MSC_VER)
#       define yy_align(x) __declspec(align(x))
#   elif yy_has_attribute(aligned) || defined(__GNUC__)
#       define yy_align(x) __attribute__((aligned(x)))
#   elif __cplusplus >= 201103L
#       define yy_align(x) alignas(x)
#   else
#       define yy_align(x)
#   endif
#endif

/* likely for compiler */
#ifndef yy_likely
#   if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#       define yy_likely(expr) __builtin_expect(!!(expr), 1)
#   else
#       define yy_likely(expr) (expr)
#   endif
#endif

/* unlikely for compiler */
#ifndef yy_unlikely
#   if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#       define yy_unlikely(expr) __builtin_expect(!!(expr), 0)
#   else
#       define yy_unlikely(expr) (expr)
#   endif
#endif

/* compiler version (GCC) */
#ifndef yy_gcc_available
#   define yy_gcc_available(major, minor, patch) \
        ((__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= \
        (major * 10000 + minor * 100 + patch))
#endif

/* real gcc check */
#ifndef yy_is_real_gcc
#   if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
        !defined(__clang__) && !defined(__llvm__) && \
        !defined(__INTEL_COMPILER) && !defined(__ICC)
#       define yy_is_real_gcc 1
#   endif
#endif

#ifndef is_intel_compiler
#   if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
        (defined(__INTEL_COMPILER) || defined(__ICC))
#       define is_intel_compiler 1
#   endif
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
#   define gcc_load_barrier(val)   __asm__ volatile(""::"m"(val))
#   define gcc_store_barrier(val)  __asm__ volatile("":"=m"(val))
#   define gcc_full_barrier(val)   __asm__ volatile("":"=m"(val):"m"(val))
#else
#   define gcc_load_barrier(val)
#   define gcc_store_barrier(val)
#   define gcc_full_barrier(val)
#endif

/* msvc intrinsic */
#if _MSC_VER >= 1400
#   include <intrin.h>
#   if defined(_M_AMD64) || defined(_M_ARM64)
#       define MSC_HAS_BIT_SCAN_64 1
#       pragma intrinsic(_BitScanForward64)
#       pragma intrinsic(_BitScanReverse64)
#   endif
#   if defined(_M_AMD64) || defined(_M_ARM64) || \
        defined(_M_IX86) || defined(_M_ARM)
#       define MSC_HAS_BIT_SCAN 1
#       pragma intrinsic(_BitScanForward)
#       pragma intrinsic(_BitScanReverse)
#   endif
#   if defined(_M_AMD64)
#       define MSC_HAS_UMUL128 1
#       pragma intrinsic(_umul128)
#   if (defined(_M_IX86_FP) && _M_IX86_FP == 2) || defined(_M_X64)
#           ifndef HAS_NEON_OR_SSE2
#               define HAS_NEON_OR_SSE2 1
#           endif
#           ifndef HAS_SSE2
#               define HAS_SSE2 1
#           endif
#       endif
#   endif
#endif

/* gcc builtin */
#if yy_has_builtin(__builtin_clzll) || yy_gcc_available(3, 4, 0)
#   define GCC_HAS_CLZLL 1
#endif

#if yy_has_builtin(__builtin_ctzll) || yy_gcc_available(3, 4, 0)
#   define GCC_HAS_CTZLL 1
#endif

/* int128 type */
#ifndef YY_HAS_INT128
#   if (__SIZEOF_INT128__ == 16) && \
       (defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER))
#       define YY_HAS_INT128 1
#   else
#       define YY_HAS_INT128 0
#   endif
#endif

/* IEEE 754 floating-point binary representation */
#ifndef YY_HAS_IEEE_754
#   if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
#       define YY_HAS_IEEE_754 1
#   elif FLT_RADIX == 2 && \
        DBL_MANT_DIG == 53 && DBL_DIG == 15 && \
        DBL_MIN_EXP == -1021 && DBL_MAX_EXP == 1024 && \
        DBL_MIN_10_EXP == -307 && DBL_MAX_10_EXP == 308
#       define YY_HAS_IEEE_754 1
#   else
#       define YY_HAS_IEEE_754 0
#       warning IEEE 754 not detected
#       if __FAST_MATH__ || __USE_FAST_MATH__
#           warning "-ffast-math" may break the nan/inf check
#       endif
#   endif
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
#    define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif

#if defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD != 0 && FLT_EVAL_METHOD != 1
#    define YY_DOUBLE_MATH_CORRECT 0
#elif defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(_X86_) || defined(__X86__) || defined(_M_IX86) || \
    defined(__I86__) || defined(__IA32__) || defined(__THW_INTEL)
#   if (defined(_MSC_VER) && defined(_M_IX86_FP) && _M_IX86_FP == 2) || \
        (defined(__SSE2_MATH__) && __SSE2_MATH__)
#       define YY_DOUBLE_MATH_CORRECT 1
#   else
#       define YY_DOUBLE_MATH_CORRECT 0
#   endif
#elif defined(__mc68000__) || defined(__pnacl__) || defined(__native_client__)
#   define YY_DOUBLE_MATH_CORRECT 0
#else
#   define YY_DOUBLE_MATH_CORRECT 1
#endif



/*==============================================================================
 * Macros
 *============================================================================*/
namespace xjb64_32_comp {

typedef __uint128_t u128;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#if HAS_NEON_OR_SSE2
    #if HAS_NEON
        typedef uint64x2_t byte16_reg; // 128 bit register for neon
    #elif HAS_SSE2
        typedef __m128i byte16_reg; // 128 bit register for sse2
    #endif
#else
        typedef struct{
            u64 hi;
            u64 lo;
        }byte16_reg;
#endif

static inline u32 u64_lz_bits(u64 v) {
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
        return table[(v * ((0x03F79D71ull << 32) + 0xB4CB0A89ull)) >> 58];
    #endif
}
static inline u32 u32_lz_bits(u32 v) {
    #if GCC_HAS_CLZLL
        return (u32)__builtin_clz(v);
    #elif MSC_HAS_BIT_SCAN
        unsigned long lz;
        _BitScanReverse(&lz, (u32)v);
        return (u32)lz;
    #else
        /* branchless, use de Bruijn sequences */
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
        u32 lz = table[(v * ((0x03F79D71ull << 32) + 0xB4CB0A89ull)) >> 58];
        return lz - 32;
    #endif
}

#if HAS_NEON_OR_SSE2
    #if HAS_NEON
    static inline u64 endcode_16digit_fast(const u64 x,byte16_reg* x_ASCII)
        {
            // require x in [1 , 1e16 - 1] = [1 , 99999999*1e8 + 99999999]
            // return tail zero count of x in base 10
            const u64 ZERO = 0x30303030ull + (0x30303030ull << 32);
            u64 aabbccdd = x / 100000000;
            u64 eeffgghh = x - aabbccdd * 100000000;
            u64 aabb = ((aabbccdd * 109951163) >> 40);
            u64 eeff = ((eeffgghh * 109951163) >> 40);
            uint64x2_t merge8 = vcombine_u64(vld1_u64(&aabbccdd), vld1_u64(&eeffgghh));
            uint64x2_t merge_aabb_eeff = vcombine_u64(vld1_u64(&aabb), vld1_u64(&eeff));
            uint64x2_t merge4 = vorrq_u8(vshlq_n_u64(vmlsq_n_u32(merge8,merge_aabb_eeff,10000),32),merge_aabb_eeff);
            uint64x2_t merge2 = vmlsq_n_u32(vshlq_n_u32(merge4,16), vshrq_n_u32(vmulq_n_u32(merge4,10486),20) , ((100<<16) - 1) );
            uint64x2_t merge = vmlsq_n_u16(vshlq_n_u16(merge2,8), vshrq_n_u16(vmulq_n_u16(merge2,103),10) , ((10<<8) - 1) );
            const u64 ZERO_2[2] = {ZERO,ZERO};
            uint64x2_t ASCII_16 = vorrq_u8(merge,vld1q_u64((uint64_t*)ZERO_2));
            *x_ASCII = ASCII_16;
            u64 aabbccdd_BCD = vgetq_lane_u64(merge, 0);
            u64 eeffgghh_BCD = vgetq_lane_u64(merge, 1);
            u64 aabbccdd_tz = u64_lz_bits(aabbccdd_BCD);
            u64 eeffgghh_tz = u64_lz_bits(eeffgghh_BCD);
            u64 tz = (eeffgghh_BCD == 0) ? 64 | aabbccdd_tz : eeffgghh_tz;//when eeffgghh_BCD is zero, aabbccdd_tz is the not-zero value ; because of v > 5e-324 ; x!=0
            tz = tz / 8;
            return tz;
        }
    static inline u64 encode_8digit_fast(const u64 x,u64* ASCII){
            u64 aabb = (x * (u128)1844674407370956) >> 64;
            u64 ccdd = x - aabb * 10000;
            uint32x2_t merge4_t = vld1_u32((uint32_t const *)&aabb);// 0000 aabb
            merge4_t = vset_lane_u32(ccdd, merge4_t, 1);// ccdd aabb
            uint64x1_t merge4 = vreinterpret_u64_u32(merge4_t);
            uint64x1_t merge2 = vmls_n_u32(vshl_n_u32(merge4,16), vshr_n_u32(vmul_n_u32(merge4,10486),20) , ((100<<16) - 1) );
            uint16x4_t merge = vmls_n_u16(vshl_n_u16(merge2,8), vshr_n_u16(vmul_n_u16(merge2,103),10) , ((10<<8) - 1) );
            uint64x1_t merge_final = vreinterpret_u64_u16(merge);
            u64 aabbccdd_BCD = vget_lane_u64(merge_final,0);
            aabbccdd_BCD = (x >= (u64)1e7) ? aabbccdd_BCD : (aabbccdd_BCD >> 8);
            *ASCII = aabbccdd_BCD | ((0x30303030ull << 32) + 0x30303030ull);
            return u64_lz_bits(aabbccdd_BCD) / 8;
        }
    #endif // endif HAS_NEON

    #if HAS_SSE2
    static inline u64 endcode_16digit_fast(const u64 v,byte16_reg* ASCII)
    {

// magic numbers for 16-bit division
#define DIV_10		0x199a	// shift = 0 + 16
#define DIV_100		0x147b	// shift = 3 + 16
// magic number for 32-bit division
#define DIV_10000	0xd1b71759		// shift = 13 + 32

    // v is 16-digit number = abcdefghijklmnop

    const __m128i div_10000 = _mm_set1_epi32(DIV_10000);//sse2
    const __m128i mul_10000 = _mm_set1_epi32(10000);
    const int div_10000_shift = 45;

    const __m128i div_100   = _mm_set1_epi16(DIV_100);//sse2
    const __m128i mul_100   = _mm_set1_epi16(100);
    const int div_100_shift = 3;

    const __m128i div_10    = _mm_set1_epi16(DIV_10);//sse2
    const __m128i mul_10    = _mm_set1_epi16(10);

    const __m128i ascii0    = _mm_set1_epi8('0');//sse2

	// can't be easliy done in SSE
	const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
	const uint32_t b = v - a * 100000000; // 8-digit number: ijklmnop

    //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 ]
    // x            = [       0       |      ijklmnop |       0       |      abcdefgh ]
    __m128i x = _mm_set_epi64x(b, a);//sse2

	// x div 10^4   = [       0       |          ijkl |       0       |          abcd ]
    __m128i x_div_10000;
    x_div_10000 = _mm_mul_epu32(x, div_10000);//sse2
    x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);//sse2

    // x mod 10^4   = [       0       |          mnop |       0       |          efgh ]
    __m128i x_mod_10000;
    x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
    x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

    // y            = [          mnop |          ijkl |          efgh |          abcd ]
    __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

#if defined(__SSE4_1__) // __GNUC__ : gcc icpx clang  ; for MSVC how to check cpu support sse4.1
    __m128i z = _mm_sub_epi32( _mm_slli_epi32(y,16) , _mm_mullo_epi32( _mm_set1_epi32((100<<16)-1) , _mm_srli_epi32( _mm_mulhi_epi16(y,_mm_set1_epi32(10486)),4)));
#else
    // y_div_100    = [   0   |    mn |   0   |    ij |   0   |    ef |   0   |    ab ]
    __m128i y_div_100;
    y_div_100 = _mm_mulhi_epu16(y, div_100);
    y_div_100 = _mm_srli_epi16(y_div_100, div_100_shift);

    // y_mod_100    = [   0   |    op |   0   |    kl |   0   |    gh |   0   |    cd ]
    __m128i y_mod_100;
    y_mod_100 = _mm_mullo_epi16(y_div_100, mul_100);
    y_mod_100 = _mm_sub_epi16(y, y_mod_100);

    // z            = [    mn |    op |    ij |    kl |    ef |    gh |    ab |    cd ]
    __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif


    // z_div_10     = [ 0 | m | 0 | o | 0 | i | 0 | k | 0 | e | 0 | g | 0 | a | 0 | c ]
    __m128i z_div_10 = _mm_mulhi_epu16(z, div_10);
    //(z<<8) - 2559 * z/10;
    __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z,8) , _mm_mullo_epi16(_mm_set1_epi16(2559) , z_div_10));

    unsigned int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(tmp, _mm_setzero_si128()));

    //u32 tz = __builtin_clz( mask ) - 16 ;
    //u32 tz = u64_lz_bits(mask) - 48;
    u32 tz = u32_lz_bits(mask);

    tmp = _mm_add_epi8(tmp, ascii0);

    *ASCII = tmp;

    return tz;

    }
    static inline u64 encode_8digit_fast(u64 x, u64* ASCII){//to be completed
        // this code convert 8 digit number to 8 digit ASCII number;
        // require x in [1 , 1e8 - 1] = [1 , 99999999]
        // return tail zero count of x in base 10 , range [0,7]

        // 12345678 => "12345678"
        // 01234567 => "12345670"
        //u64 aabb = (x * 109951163) >> 40;
        u64 aabb = (x * (u128)1844674407370956) >> 64;
        u64 aabb_ccdd_merge = (x << 32) - ((10000ull<<32) - 1) * (aabb);
        __m128i y = _mm_set1_epi64x(aabb_ccdd_merge);
#if defined(__SSE4_1__) // __GNUC__ : gcc icpx clang  ; for MSVC how to check cpu support sse4.1
    //_mm_mullo_epi32 need sse4.1
    __m128i z = _mm_sub_epi32( _mm_slli_epi32(y,16) , _mm_mullo_epi32( _mm_set1_epi32((100<<16)-1) , _mm_srli_epi32( _mm_mulhi_epi16(y,_mm_set1_epi32(10486)),4)));
#else
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epi16(y, _mm_set1_epi16(0x147b)),3);

    __m128i y_mod_100 = _mm_sub_epi16(y,_mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));

    __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
        __m128i z_div_10 = _mm_mulhi_epi16(z, _mm_set1_epi16(0x199a));
        __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z,8) , _mm_mullo_epi16(_mm_set1_epi16(2559) , z_div_10));
        u64 aabbccdd_BCD = _mm_cvtsi128_si64(tmp);
        aabbccdd_BCD = (x >= (u64)1e7) ? aabbccdd_BCD : (aabbccdd_BCD >> 8) ;
        *ASCII =  aabbccdd_BCD + ((0x30303030ull << 32) + 0x30303030ull);
        return u64_lz_bits(aabbccdd_BCD) / 8;
    }
    #endif // endif HAS_SSE2
#else
    static inline u64 endcode_16digit_fast(const u64 v,byte16_reg* ASCII)
    {
        const u64 ZERO = (0x30303030ull << 32) + 0x30303030ull;
        u64 aabbccdd = v / 100000000;
        u64 eeffgghh = v - aabbccdd * 100000000;
        u64 aabb_ccdd_merge = (aabbccdd << 32) - ((10000ull<<32) - 1) * ((aabbccdd * 109951163) >> 40);
        u64 eeff_gghh_merge = (eeffgghh << 32) - ((10000ull<<32) - 1) * ((eeffgghh * 109951163) >> 40);
        u64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) - ((100ull<<16) - 1) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
        u64 ee_ff_gg_hh_merge = (eeff_gghh_merge << 16) - ((100ull<<16) - 1) * (((eeff_gghh_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
        u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) - ((10ull<<8) - 1) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
        u64 eeffgghh_BCD = (ee_ff_gg_hh_merge << 8) - ((10ull<<8) - 1) * (((ee_ff_gg_hh_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
        u64 aabbccdd_tz = u64_lz_bits(aabbccdd_BCD);
        u64 eeffgghh_tz = u64_lz_bits(eeffgghh_BCD);
        u64 tz = (eeffgghh_BCD == 0) ? 64 | aabbccdd_tz : eeffgghh_tz;
        tz = tz / 8;
        u64 aabbccdd_ASCII = aabbccdd_BCD | ZERO;
        u64 eeffgghh_ASCII = eeffgghh_BCD | ZERO;
        (*ASCII).hi = aabbccdd_ASCII;
        (*ASCII).lo = eeffgghh_ASCII;
        return tz;
    }
    static inline u64 encode_8digit_fast(const u64 x, u64* ASCII){
        // this code convert 8 digit number to 8 digit ASCII number;
        // require x in [1 , 1e8 - 1] = [1 , 99999999]
        // return tail zero count of x in base 10 , range [0,7]

        // 12345678 => "12345678"
        // 01234567 => "12345670"
        u64 aabb_ccdd_merge = (x << 32) - ((10000ull<<32) - 1) * ((x * 109951163) >> 40);
        u64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) - ((100ull<<16) - 1) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
        u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) - ((10ull<<8) - 1) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
        aabbccdd_BCD = (x >= (u64)1e7) ? aabbccdd_BCD : (aabbccdd_BCD >> 8);
        *ASCII =  aabbccdd_BCD | ((0x30303030ull << 32) + 0x30303030ull);
        return u64_lz_bits(aabbccdd_BCD) / 8;
    }
#endif // endif HAS_NEON_OR_SSE2

// u64 encode_16digit_avx512(u64 x, __m128i *out)
// {
//     // need AVX512F,AVX512CD,AVX512BW,AVX512BW,AVX,sse4.1

//     u64 aabbccdd = x / 100000000;
//     u64 eeffgghh = x % 100000000;

//     __m256i l8_4r = _mm256_set1_epi64x(eeffgghh); // AVX
//     __m512i h8_4r = _mm512_castsi256_si512(_mm256_set1_epi64x(aabbccdd));
//     __m512i n8r = _mm512_inserti64x4(h8_4r, l8_4r, 1); // AVX512F
//     const u64 m8 = 180143986;                          //>>> 2**54/1e8
//     const u64 m6 = 18014398510;                        //>>> 2**54/1e6
//     const u64 m4 = 1801439850949;                      //>>> 2**54/1e4
//     const u64 m2 = 180143985094820;                    //>>> 2**54/1e2
//     const __m512i mr = _mm512_set_epi64(m8, m6, m4, m2, m8, m6, m4, m2);
//     const __m512i M54_8_all = _mm512_set1_epi64(((u64)1 << 54) - 1);
//     const __m512i M8_8_2 = _mm512_set1_epi64(0xff00);
//     const __m512i t10r = _mm512_set1_epi64(10);
//     __m512i tmp_8_0 = _mm512_mullo_epi64(n8r, mr);          // AVX512DQ
//     __m512i tmp_8_1 = _mm512_and_epi64(tmp_8_0, M54_8_all); // AVX512F
//     __m512i tmp_8_2 = _mm512_mullo_epi64(tmp_8_1, t10r);
//     __m512i tmp_8_3_t = _mm512_and_epi64(tmp_8_2, M54_8_all);
//     __m512i tmp_8_3 = _mm512_mullo_epi64(tmp_8_3_t, t10r);
//     __m512i tmp_8_1_print = _mm512_srli_epi64(tmp_8_2, 54);
//     __m512i tmp_8_2_print = _mm512_and_epi64(_mm512_srli_epi64(tmp_8_3, (54 - 8)), M8_8_2);
//     __m512i BCD = tmp_8_1_print | tmp_8_2_print;
//     const short idx[8] = {12, 8, 4, 0, 28, 24, 20, 16}; // 16byte
//     const __m512i idxr_epi16 = _mm512_castsi128_si512(_mm_loadu_epi64(idx));
//     BCD = _mm512_permutexvar_epi16(idxr_epi16, BCD); // AVX512BW
//     __m512i ASCII = _mm512_set1_epi64((0x30303030ull << 32) + 0x30303030ull) | BCD;
//     __m512i tz = _mm512_srli_epi64(_mm512_lzcnt_epi64(BCD), 3); // AVX512CD  , lzcnt(0)=64
//     *out = _mm512_extracti32x4_epi32(ASCII, 0);                 // xmm register ; no operation on zmm register
//     __m128i tz2 = _mm512_extracti32x4_epi32(tz, 0);             // xmm register
//     u64 tz0 = _mm_extract_epi64(tz2, 0);                        // sse4.1
//     u64 tz1 = _mm_extract_epi64(tz2, 1);
//     return tz1 == 8 ? 8 + tz0 : tz1;
// }


static inline u64 encode_16digit(const u64 x, u64* hi,u64* lo){
    // this code convert 16 digit number to 16 digit ASCII number;
    // require x in [1 , 1e16 - 1] = [1 , 99999999*1e8 + 99999999]
    // return tail zero count of x in base 10 , range [0,15]

    // 1234567890123456 => hi:"12345678" + lo:"90123456"
    const u64 ZERO = (0x30303030ull << 32) + 0x30303030ull;
    u64 aabbccdd = x / 100000000;
    u64 eeffgghh = x - aabbccdd * 100000000;
    u64 aabb_ccdd_merge = (aabbccdd << 32) - ((10000ull<<32) - 1) * ((aabbccdd * 109951163) >> 40);
    u64 eeff_gghh_merge = (eeffgghh << 32) - ((10000ull<<32) - 1) * ((eeffgghh * 109951163) >> 40);
    u64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) - ((100ull<<16) - 1) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 ee_ff_gg_hh_merge = (eeff_gghh_merge << 16) - ((100ull<<16) - 1) * (((eeff_gghh_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) - ((10ull<<8) - 1) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
    u64 eeffgghh_BCD = (ee_ff_gg_hh_merge << 8) - ((10ull<<8) - 1) * (((ee_ff_gg_hh_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
    u64 aabbccdd_tz = u64_lz_bits(aabbccdd_BCD);
    u64 eeffgghh_tz = u64_lz_bits(eeffgghh_BCD);
    u64 tz = (eeffgghh_BCD == 0) ? 64 | aabbccdd_tz : eeffgghh_tz;
    tz = tz / 8;
    u64 aabbccdd_ASCII = aabbccdd_BCD | ZERO;
    u64 eeffgghh_ASCII = eeffgghh_BCD | ZERO;
    *hi = aabbccdd_ASCII;
    *lo = eeffgghh_ASCII;
    return tz;
}
static inline u64 encode_8digit(const u64 x, u64* ASCII){
    // this code convert 8 digit number to 8 digit ASCII number;
    // require x in [1 , 1e8 - 1] = [1 , 99999999]
    // return tail zero count of x in base 10 , range [0,7]

    // 12345678 => "12345678"
    // 01234567 => "12345670"
    // when x=0 , return value tz = u64_lz_bits(0) / 8, so tz is not sure.
    const u64 ZERO = (0x30303030ull << 32) + 0x30303030ull;
    u64 aabbccdd = x;
    u64 aabb_ccdd_merge = (aabbccdd << 32) - ((10000ull<<32) - 1) * ((aabbccdd * 109951163) >> 40);
    u64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) - ((100ull<<16) - 1) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) - ((10ull<<8) - 1) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
    aabbccdd_BCD = (x >= (u64)1e7) ? aabbccdd_BCD : (aabbccdd_BCD >> 8);//if top digit = 0,remove it.  example : 01234567 => "12345670"
    u64 tz = u64_lz_bits(aabbccdd_BCD) / 8;
    u64 aabbccdd_ASCII = aabbccdd_BCD + ZERO;
    *ASCII = aabbccdd_ASCII;
    return tz;
}
static inline void byte_move_16(void * dst,const void* src){
    // move 16byte from src to dst;
#if HAS_NEON
    uint64x2_t src_value = vld1q_u64((const uint64_t*)src);
    vst1q_u64((uint64_t*)dst, src_value);
#elif HAS_SSE2
    __m128i src_value = _mm_loadu_si128((const __m128i*)src);
    _mm_storeu_si128((__m128i*)dst, src_value);
#else
    // avoid overlap issue
    u64 hi;
    u64 lo;
    char * psrc = (char*)src;
    memcpy(&hi, psrc, 8);
    memcpy(&lo, psrc + 8, 8);
    char * pdst = (char*)dst;
    memcpy(pdst, &hi, 8);
    memcpy(pdst + 8, &lo, 8);
#endif
}

static inline void byte_move_8(void * dst,const void* src){
    // move 8byte from src to dst; no overlap issue
    u64 src_value;
    memcpy(&src_value, src, 8);
    memcpy(dst, &src_value, 8);
}
static inline u64 calc_pow5_rlz(unsigned int pow5_offset)
{
    // require 0<= pow5_offset <= 27; max_u64 = (2**64)-1 > 5**27
    // return (5**pow5_offset) << clz(5**pow5_offset);
#if 1
    const u64 p5_4 = 5*5*5*5;
    const u64 p5_8 = p5_4*p5_4;
    const u64 p5_0_3 = (125<<24) + (25<<16) + (5<<8) + 1;
    u64 p5 = ((pow5_offset & 16) ? p5_8*p5_8 : 1) * ((pow5_offset & 8) ? p5_8 : 1) * ((pow5_offset & 4) ? p5_4 : 1) * ( (p5_0_3 >> ((pow5_offset % 4) * 8)) & 0xff );
    u64 clz = 63 - ((pow5_offset * 149) >> 6);// equal to clz(p5)
    return p5 << clz;
#else
    u64 p5 = 1;
    u64 p5_base = 5;
    while (pow5_offset > 0)
    {
        if (pow5_offset & 1)
            p5 *= p5_base;
        p5_base *= p5_base;
        pow5_offset >>= 1;
    }
    u64 clz = u64_lz_bits(p5);
    //u64 clz2 = 63 - ((pow5_offset * 217707) >> 16) + pow5_offset;
    return p5 << clz;
#endif
}

//extern "C"
//static inline
char* xjb64(double v,char* buf)
{

    // compress lookup table version
    // lookup table size :
    //        (1)  x86-64 : 592 byte
    //        (2)  arm64 : 592+256 = 848 byte

    // icpx -O3 -c ./dtoa_xjb_comp.cpp -o dtoa_xjb_comp.o
    // dtoa_xjb_comp.o file size : 3816 byte

    u64 vi;
    memcpy(&vi, &v, sizeof(v));//double to u64 bit copy
    buf[0]='-';
    buf += vi>>63;

    u64 dec,m;
    int e10;
    u64 tz;// tail zero
    u64 hi,lo;// 15 or 16digit
    u64 one_ASCII;// last digit
    u64 dec_sig_len;// decimal length
    u64 dec_sig_len_ofs;// = dec_sig_len + 2
    u64 D17;// 1:17 digits 0:16 digits
    u64 ieee_significand = vi & ((1ull<<52) - 1);
    u64 ieee_exponent = (vi << 1 ) >> 53;
    // if(exp == 2047)[[unlikely]]
    // {
    //     *(u32*)buf = sig ? *(u32*)"NaN" : *(u32*)"Inf";//end with '\0'
    //     return buf + 3;
    // }
    // if( (vi << 1) < 2 )[[unlikely]]
    // {
    //     *(u64*)buf = (vi << 1) ? *(u64*)"5e-324" : *(u32*)"0.0";//end with '\0'
    //     return buf + ((vi << 1) ? 6 : 3);
    // }
    //if( (vi << 1) < 3 || (vi << 1) >= (2047ull<<53)  )[[unlikely]]
    if  ( (vi<<1) - 3 >= (2047ull<<53) - 3 ) [[unlikely]]
    {
        // *(u64*)buf = ((vi << 1) < 3) ? ((vi << 1) ? *(u64*)"5e-324\0" : *(u32*)"0.0")
        //                              : (vi << 1) == (2047ull<<53) ? *(u32*)"Inf" : *(u32*)"NaN";//end with '\0'
        u64 write_value;
        if( (vi << 1) == 0 ) memcpy(&write_value , "0.0\0\0\0\0", 8);
        if( (vi << 1) == (1 << 1) ) memcpy(&write_value , "5e-324\0", 8);
        if( (vi << 1) == (2047ull<<53) ) memcpy(&write_value , "Inf\0\0\0\0", 8);
        if( (vi << 1) > (2047ull<<53) ) memcpy(&write_value , "NaN\0\0\0\0", 8);
        memcpy(buf , &write_value, 8);

        // if( (vi << 1) == 0 )memcpy(buf , "0.0\0\0\0\0", 8);
        // if( (vi << 1) == (1 << 1) )memcpy(buf , "5e-324\0", 8);
        // if( (vi << 1) == (2047ull<<53) )memcpy(buf , "Inf\0\0\0\0", 8);
        // if( (vi << 1) > (2047ull<<53) )memcpy(buf , "NaN\0\0\0\0", 8);

        // memcpy(buf , ((vi << 1) < 3) ? ((vi << 1) ? "5e-324\0" : "0.0\0\0\0\0")
        //                               : (vi << 1) == (2047ull<<53) ? "Inf\0\0\0\0" : "NaN\0\0\0\0", 8);
        return buf + ((vi << 1) - 3 == (u64)-1 ? 6 : 3);//end with '\0'
    }
    //*(u64*)buf = *(u64*)"0.00000";
    memcpy(buf, "0.000000", 8);
    u64 c;
    int32_t q;
#ifdef __amd64__
//#if 0
    if (ieee_exponent > 0) [[likely]] // branch
    {
        c = (1ull<<52) | ieee_significand;// 53 bit
        q = ieee_exponent - 1075;
    }
    else
    {
        c = ieee_significand;
        q = 1 - 1075; // -1074
    }
#else
    c = (ieee_exponent > 0) ? ( (1ull<<52) | ieee_significand) : ieee_significand;
    q = (ieee_exponent > 0) ? (ieee_exponent - 1075) : 1-1075;
#endif
    int k;
    const int offset = 6; // [5,10] ; 5 + 64 >= 69    6+64=70 >=69
    u64 regular = ieee_significand > 0;
    u64 irregular = (ieee_significand == 0);

    const int base_start = -11, base_end = 11,  ratio = 27;

#define direct_via_lut_get_pow5  1
// direct_via_lut_get_pow5 = 1 : direct use looup table to get (5**n)<<clz(5**n)
// direct_via_lut_get_pow5 = 0 : calc the  (5**n)<<clz(5**n)  directly, without using lookup table

    //compress ratio = 27; all lut size = 23*2*8 + (27+1)*8 = 592 byte
    static const u64 pow10_base_and_pow5_rlz[(base_end - base_start + 1) * 2 + direct_via_lut_get_pow5 * (ratio + 1)]={
//pow10_base table
0xa76c582338ed2621, 0xaf2af2b80af6f24f, // e10 =  -11 * 27 = -297
0x873e4f75e2224e68, 0x5a7744a6e804a292, // e10 =  -10 * 27 = -270
0xda7f5bf590966848, 0xaf39a475506a899f, // e10 =  -9 * 27 = -243
0xb080392cc4349dec, 0xbd8d794d96aacfb4, // e10 =  -8 * 27 = -216
0x8e938662882af53e, 0x547eb47b7282ee9d, // e10 =  -7 * 27 = -189
0xe65829b3046b0afa, 0x0cb4a5a3112a5113, // e10 =  -6 * 27 = -162
0xba121a4650e4ddeb, 0x92f34d62616ce414, // e10 =  -5 * 27 = -135
0x964e858c91ba2655, 0x3a6a07f8d510f870, // e10 =  -4 * 27 = -108
0xf2d56790ab41c2a2, 0xfae27299423fb9c4, // e10 =  -3 * 27 = -81
0xc428d05aa4751e4c, 0xaa97e14c3c26b887, // e10 =  -2 * 27 = -54
0x9e74d1b791e07e48, 0x775ea264cf55347e, // e10 =  -1 * 27 = -27
0x8000000000000000, 0x0000000000000000, // e10 =  0 * 27 = 0
0xcecb8f27f4200f3a, 0x0000000000000000, // e10 =  1 * 27 = 27
0xa70c3c40a64e6c51, 0x999090b65f67d924, // e10 =  2 * 27 = 54
0x86f0ac99b4e8dafd, 0x69a028bb3ded71a4, // e10 =  3 * 27 = 81
0xda01ee641a708de9, 0xe80e6f4820cc9496, // e10 =  4 * 27 = 108
0xb01ae745b101e9e4, 0x5ec05dcff72e7f90, // e10 =  5 * 27 = 135
0x8e41ade9fbebc27d, 0x14588f13be847308, // e10 =  6 * 27 = 162
0xe5d3ef282a242e81, 0x8f1668c8a86da5fb, // e10 =  7 * 27 = 189
0xb9a74a0637ce2ee1, 0x6d953e2bd7173693, // e10 =  8 * 27 = 216
0x95f83d0a1fb69cd9, 0x4abdaf101564f98f, // e10 =  9 * 27 = 243
0xf24a01a73cf2dccf, 0xbc633b39673c8ced, // e10 =  10 * 27 = 270
0xc3b8358109e84f07, 0x0a862f80ec4700c9, // e10 =  11 * 27 = 297
#if direct_via_lut_get_pow5
// //pow5_remove_left_zero table or pow10_hi table ; when 0 <= -k-1 && -k-1 <= 27; direct use below table for pow10_hi , and pow_lo=0
0x8000000000000000,// = (5** 0) << clz(5** 0) = (5** 0) << 63 ; e10 = 0
0xa000000000000000,// = (5** 1) << clz(5** 1) = (5** 1) << 61 ; e10 = 1
0xc800000000000000,// = (5** 2) << clz(5** 2) = (5** 2) << 59 ; e10 = 2
0xfa00000000000000,// = (5** 3) << clz(5** 3) = (5** 3) << 57 ; e10 = 3
0x9c40000000000000,// = (5** 4) << clz(5** 4) = (5** 4) << 54 ; e10 = 4
0xc350000000000000,// = (5** 5) << clz(5** 5) = (5** 5) << 52 ; e10 = 5
0xf424000000000000,// = (5** 6) << clz(5** 6) = (5** 6) << 50 ; e10 = 6
0x9896800000000000,// = (5** 7) << clz(5** 7) = (5** 7) << 47 ; e10 = 7
0xbebc200000000000,// = (5** 8) << clz(5** 8) = (5** 8) << 45 ; e10 = 8
0xee6b280000000000,// = (5** 9) << clz(5** 9) = (5** 9) << 43 ; e10 = 9
0x9502f90000000000,// = (5**10) << clz(5**10) = (5**10) << 40 ; e10 = 10
0xba43b74000000000,// = (5**11) << clz(5**11) = (5**11) << 38 ; e10 = 11
0xe8d4a51000000000,// = (5**12) << clz(5**12) = (5**12) << 36 ; e10 = 12
0x9184e72a00000000,// = (5**13) << clz(5**13) = (5**13) << 33 ; e10 = 13
0xb5e620f480000000,// = (5**14) << clz(5**14) = (5**14) << 31 ; e10 = 14
0xe35fa931a0000000,// = (5**15) << clz(5**15) = (5**15) << 29 ; e10 = 15
0x8e1bc9bf04000000,// = (5**16) << clz(5**16) = (5**16) << 26 ; e10 = 16
0xb1a2bc2ec5000000,// = (5**17) << clz(5**17) = (5**17) << 24 ; e10 = 17
0xde0b6b3a76400000,// = (5**18) << clz(5**18) = (5**18) << 22 ; e10 = 18
0x8ac7230489e80000,// = (5**19) << clz(5**19) = (5**19) << 19 ; e10 = 19
0xad78ebc5ac620000,// = (5**20) << clz(5**20) = (5**20) << 17 ; e10 = 20
0xd8d726b7177a8000,// = (5**21) << clz(5**21) = (5**21) << 15 ; e10 = 21
0x878678326eac9000,// = (5**22) << clz(5**22) = (5**22) << 12 ; e10 = 22
0xa968163f0a57b400,// = (5**23) << clz(5**23) = (5**23) << 10 ; e10 = 23
0xd3c21bcecceda100,// = (5**24) << clz(5**24) = (5**24) <<  8 ; e10 = 24
0x84595161401484a0,// = (5**25) << clz(5**25) = (5**25) <<  5 ; e10 = 25
0xa56fa5b99019a5c8,// = (5**26) << clz(5**26) = (5**26) <<  3 ; e10 = 26
0xcecb8f27f4200f3a,// = (5**27) << clz(5**27) = (5**27) <<  1 ; e10 = 27
#endif
    };
#ifdef __amd64__
        if (regular) [[likely]] // branch
            k = ((ieee_exponent - 1075) * 315653) >> 20;
        else
            k = ((ieee_exponent - 1075) * 315653 - 131237) >> 20;
#else
        // use this branchless code for apple M1, better performance
        // when ieee_exponent == 1 or 0 ; k=-324
        // so we can use (ieee_exponent - 1075) to replace q
        // k = (q * 315653 - (regular ? 0 : 131237 ))>>20;
        k = ((ieee_exponent - 1075) * 315653 - (regular ? 0 : 131237 ))>>20;
#endif
        int get_e10 = -k - 1;

        static const u64 *pow5_rlz_ptr = &pow10_base_and_pow5_rlz[(base_end - base_start + 1)*2];
        u64 pow10_hi;
        u128 hi128;
        int h;
        if( 0 <= get_e10 && get_e10 <= ratio )  // direct use the pow5_rlz table value ; likely
        {
            //fast path ; likely
            h = q + ((get_e10 * 217707) >> 16);
#if direct_via_lut_get_pow5
            pow10_hi = pow5_rlz_ptr[ get_e10 ];// pow10_lo = 0
#else
            pow10_hi = calc_pow5_rlz(get_e10);
#endif
            u128 cb = c << (h + 1 + offset);
            hi128 = cb * pow10_hi;
        }
        else
        {
            //int base = ( get_e10 - (base_start * ratio) ) / ratio;// range = [0,22] = [0/27,616/27]
            int base = (( get_e10 - (base_start * ratio) ) * 1214) >> 15;// div 27
            u32 pow5_offset = get_e10 - (base + base_start) * ratio;// range = [0,26]
            u64 pow10_base_high = pow10_base_and_pow5_rlz[base  * 2];
            u64 pow10_base_low = pow10_base_and_pow5_rlz[base * 2 + 1];
#if direct_via_lut_get_pow5
            u128 pow5_rlz_v = pow5_rlz_ptr[  pow5_offset ];// (5**pow5_offset) << clz(5**pow5_offset)
#else
            u128 pow5_rlz_v = calc_pow5_rlz(pow5_offset);
#endif
            u128 pow10 = pow5_rlz_v * pow10_base_high + ((pow5_rlz_v * pow10_base_low) >> 64) ;// u64 * u128
            pow10_hi = pow10 >> 64;
            u64 pow10_lo = pow10;
            int alpha = ((( (base + base_start) * ratio) * 217707) >> 16) + ((pow5_offset * 217707) >> 16);
            h = 1 + alpha + q;
            u128 cb = c << (h + offset + 1);
            hi128 = (cb * pow10_hi + ((cb * pow10_lo) >> 64)); // u64 * u128
        }
        u64 dot_one = hi128 >> offset;   // == floor(2**64*n)
        //u64 half_ulp = (pow10_hi >> (-h) ) + ((c + 1) & 1) ;
        u64 half_ulp = (h > 0 ? pow10_hi * 2 : (pow10_hi >> (-h))) + ((c + 1) & 1);
        u64 up = (half_ulp  > ~0 - dot_one);
        u64 down = ( (half_ulp >> (1 - regular )) > dot_one );
        m = (hi128 >> (offset + 64)) + up;
        u64 up_down = up + down;
        D17 = (m >= (u64)1e15);
        byte16_reg ASCII_16;
        u64 mr = D17 ? m : m * 10;//remove left zero
        tz = endcode_16digit_fast(mr,&ASCII_16);// convert mr to ASCII , and return tail zero number.
#if yy_is_real_gcc //  for gcc compiler , prevent branch instruction cause branch miss;

#if HAS_SSE2 // when use sse2,the return value equal to (tail zero number + 16);
        //dec_sig_len_ofs = ( ( (2+16+16)*256 + 2+16 - tz*256 + D17 ) >> (up_down ? 8 : 0)) & 0xff;
        dec_sig_len = ( ( (16+16)*256 + 16 - tz*256 + D17 ) >> (up_down ? 8 : 0)) & 0xff;
#else
        //dec_sig_len_ofs = ( ( (2+16)*256 + 2+16 - tz*256 + D17 ) >> (up_down ? 8 : 0)) & 0xff;
        dec_sig_len = ( ( (16)*256 + 16 - tz*256 + D17 ) >> (up_down ? 8 : 0)) & 0xff;
#endif

#else

#if HAS_SSE2 // when use sse2,the return value equal to (tail zero number + 16);
        //dec_sig_len_ofs = up_down  ?  2+16+16 - tz : 2+16 + D17;
        dec_sig_len = up_down  ?  16+16 - tz : 16 + D17;
#else
        //dec_sig_len_ofs = up_down  ?  2+16 - tz : 2+16 + D17;
        dec_sig_len = up_down  ?  16 - tz : 16 + D17;
        //u64 dec_sig_len_1 = up_down  ?  16+1 - tz : 16+1 + D17;
#endif

#endif
        // in fact : dec_sig_len = (one > 0) ? 16 + D17 : 16 - tz; add 2 for calc the offset in the array.
        k += 15 + D17;
        e10 = k;

// calc one and determine one is 0 or not
#ifdef __amd64__
        // (dot_one == (1ull << 62)) equal to (n==0.25)
        u64 offset_num = (dot_one == (1ull << 62)) ? 0 : (1ull<<63) + 6 ;
        u64 one = ((dot_one * (u128)10 + offset_num ) >> 64) + (u64)('0' + '0' * 256);
        if(!regular)[[unlikely]]
            if (((((dot_one >> 4) * 10) << 4) >> 4) > (((half_ulp >> 4) * 5)))
                 one = (((dot_one >> 4) * 10) >> 60) + 1 + (u64)('0' + '0' * 256);
#else // for apple M1 , better performance
        u64 one = ((dot_one * (u128)10) >> 64)  + ( (u64)(dot_one * (u128)10) > ((dot_one == (1ull << 62)) ? ~0 : 0x7ffffffffffffff9ull) ) + (u64)('0' + '0' * 256);
        static const u64 bitarray_irregular[32] = {
        0x0000000000010040, 0x0000000000000004, 0x0000000000000000, 0x0020090000000000,
        0x0000000000000000, 0x0000000000000100, 0x0000000000000000, 0x0000000000000000,
        0x0000000000400000, 0x0000000000000000, 0x0000000000020000, 0x0000000000800000,
        0x0000000000000000, 0x0008000000000000, 0x0004000040000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000001000000, 0x0020000000000000, 0x0000000000000000,
        0x0001000000040000, 0x0200000001000000, 0x0000000000102000, 0x0000000100000000,
        0x2000000000000000, 0x0000000000020000, 0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000040000000, 0x0000000000000000, 0x0000000000008000};
        if(!regular)[[unlikely]]
            one += (bitarray_irregular[ieee_exponent/64]>>(ieee_exponent%64)) & 1;
#endif

        // when -3<=e10 && e10 <= 15 ; we use %lf format print float number
        const int e10_DN = -3;//do not change this value
        const int e10_UP = 15;//do not change this value
#if 0
        // precompute all possible data : size = 20*20 = 400 byte
        static const unsigned char e10_variable_data[e10_UP-(e10_DN) + 1 + 1][20]={
4,1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,// e10=-3
3,1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,// e10=-2
2,1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,// e10=-1
0,1,2,3,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,// e10=0
0,2,3,4,4,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,// e10=1
0,3,4,5,5,5,5,6,7,8,9,10,11,12,13,14,15,16,17,18,// e10=2
0,4,5,6,6,6,6,6,7,8,9,10,11,12,13,14,15,16,17,18,// e10=3
0,5,6,7,7,7,7,7,7,8,9,10,11,12,13,14,15,16,17,18,// e10=4
0,6,7,8,8,8,8,8,8,8,9,10,11,12,13,14,15,16,17,18,// e10=5
0,7,8,9,9,9,9,9,9,9,9,10,11,12,13,14,15,16,17,18,// e10=6
0,8,9,10,10,10,10,10,10,10,10,10,11,12,13,14,15,16,17,18,// e10=7
0,9,10,11,11,11,11,11,11,11,11,11,11,12,13,14,15,16,17,18,// e10=8
0,10,11,12,12,12,12,12,12,12,12,12,12,12,13,14,15,16,17,18,// e10=9
0,11,12,13,13,13,13,13,13,13,13,13,13,13,13,14,15,16,17,18,// e10=10
0,12,13,14,14,14,14,14,14,14,14,14,14,14,14,14,15,16,17,18,// e10=11
0,13,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,17,18,// e10=12
0,14,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,18,// e10=13
0,15,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,18,// e10=14
0,16,17,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,// e10=15
0,1,2,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,// e10=other
        };
        u64 e10_3 = e10 + 3;//convert to unsigned number
        u64 e10_data_ofs = e10_3 < 15+3+1 ? e10_3 : 15+3+1;//compute offset
        u64 first_sig_pos = e10_variable_data[e10_data_ofs][0];
        u64 dot_pos = e10_variable_data[e10_data_ofs][1];
        u64 move_pos = e10_variable_data[e10_data_ofs][2];
        u64 exp_pos = e10_variable_data[e10_data_ofs][dec_sig_len_ofs];
#else
        u64 first_sig_pos = (e10_DN<=e10 && e10<=-1) ? 1 - e10 : 0 ;
        u64 dot_pos = ( 0 <= e10 && e10<= e10_UP ) ? 1 + e10 : 1 ;
        u64 move_pos = dot_pos + (1 - (e10_DN<=e10 && e10<=-1) );
        // u64 exp_pos = ((e10_DN <= e10 && e10 <= -1) ? dec_sig_len - 1 : (
        //     (0<=e10 && e10<= e10_UP) ? (e10+2 > dec_sig_len ? e10+2: dec_sig_len) : (
        //         dec_sig_len - (dec_sig_len == 1)
        //     )
        // ) ) + 1;
        u64 exp_pos = (0<=e10 && e10<= e10_UP) ? (e10+3 > dec_sig_len+1 ? e10+3 : dec_sig_len+1) : (
            dec_sig_len + ( (1 - ( e10_DN <= e10 && e10 <= -1 )) > (dec_sig_len == 1) )
        );
#endif
        char * buf_origin = buf;
        buf += first_sig_pos;
#if HAS_NEON_OR_SSE2
    #if HAS_NEON
        vst1q_u64((uint64_t*)buf, ASCII_16);
    #endif
    #if HAS_SSE2
        _mm_storeu_si128((__m128i*)buf, ASCII_16);
    #endif
#else
        //*(u64*)&buf[0] = ASCII_16.hi;
        //*(u64*)&buf[8] = ASCII_16.lo;
        memcpy(buf + 0, &ASCII_16.hi, 8);
        memcpy(buf + 8, &ASCII_16.lo, 8);
#endif
        //*(u64*)&buf[15+D17] = one;//write 2 byte
        memcpy(&buf[15 + D17], &one, 8);
        byte_move_16(&buf[move_pos],&buf[dot_pos]);// dot_pos+first_sig_pos+sign max = 16+1 = 17; require 17+16=33 byte buffer
        //byte_move_16(buf + move_pos , buf + dot_pos );
        buf_origin[dot_pos] = '.';
        //static const u64 *exp_ptr = (u64*)&exp_result_precalc[324];

//process the some special case : subnormal number
        if(m < (u64)1e14 ) [[unlikely]]
        {
            // some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
            u64 lz = 0;
            while(buf[2+lz] == '0')lz++;
            lz += 2;
            e10 -= lz - 1;
            buf[0] = buf[lz];
            byte_move_16(&buf[2], &buf[lz+1]);
            //byte_move_16(buf+2, buf+lz+1);
            exp_pos = exp_pos - lz + 1 - (exp_pos - lz == 1 );
// #if is_intel_compiler
//             buf += exp_pos;
//             u64 exp_result = exp_ptr[e10];
//             //*(u64*)buf = exp_result;
//             memcpy(buf, &exp_result, 8);
//             return buf + 5;// return the end of buffer with '\0';
// #endif
        }
// write exponent , set 0 to use lookup table to get exp_result , set 1 to use next code to calc exp_result
#if 1
        buf += exp_pos;
        u64 neg = e10 < 0;
        u32 e10_abs = neg ? -e10 : e10;
        u64 e = neg ? ('e' + '-' * 256) : ('e' + '+' * 256);
        // memcpy(buf, &e, 2);
        // u64 a = (e10_abs * (u128)184467440737095517) >> 64;
        // u64 a_ascii = a+'0';
        // memcpy(buf+2,&a_ascii,1);
        // u64 _bc = (e10_abs * (u128)184467440737095517);
        // u64 b = (_bc * (u128)10) >> 64;
        // u64 b_ascii = b+'0';
        // memcpy(buf+2+(a>0),&b_ascii,1);
        // u64 _c = (_bc * (u128)10);
        // u64 c1 = (_c * (u128)10) >> 64;
        // u64 c_ascii = c1 + '0';
        // u32 write_pos = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : 2 + (a>0) + 1;
        // u32 write_len = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : write_pos + 1;
        // u32 write_value = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : c_ascii;
        // memcpy(buf + write_pos ,&write_value, 2 );
        // return buf + write_len;

        //buf += 2;

        // u32 ab = (e10_abs * 410) >> 12;//e10_abs div 10
        // u32 e10_c = (e10_abs - ab * 10) << 16;
        // u32 e10_ab = ab * 256 - 2559 * ((ab * 103) >> 10) ;
        // u32 abc_ascii = e10_ab + e10_c + (u32)('0' + '0' * 256 + '0' * 65536);
        // abc_ascii = e10_abs > 99 ? abc_ascii : abc_ascii >> 8;
        // u64 exp_result = e + (abc_ascii << 16);


        u32 a = (e10_abs * 656u) >> 16; /* e10_abs / 100 */
        u32 bc = e10_abs - a * 100;    /* e10_abs % 100 */
        u64 bc_ASCII = bc * 256u - (256 * 10 - 1) * ((bc * 103u) >> 10) + (u64)('0' + '0' * 256 + (4ull << 40) + (4ull << 32)); // 12 => "12"
        u64 exp_result = e + ( ( (e10_abs > 99u) ? a + ('0' | (1ull << 40)) + (bc_ASCII << 8) : bc_ASCII) << 16);

        // u64 bc_ASCII = bc * 256 - (256 * 10 - 1) * ((bc * 103u) >> 10) + (u64)('0' + '0' * 256); // 12 => "12"
        // u64 exp_result = e + ( ( (e10_abs > 99u) ? a + '0' + (bc_ASCII << 8) : bc_ASCII) << 16);
        exp_result = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : exp_result;// e10_DN<=e10 && e10<=e10_UP : no need to print exponent

#else
        //u64 exp_result = exp_ptr[e10];
#endif

        //*(u64*)buf = exp_result;
        memcpy(buf, &exp_result, 8);
        u64 exp_len = (e10_DN<=e10 && e10<= e10_UP ) ? 0 : (4 + (e10_abs > 99u) ) ;// "e+20" "e+308" : 4 or 5
        //u64 exp_len = exp_result >> 56; // 0 or 4 or 5 ; equal to above code
        return buf + exp_len;// return the end of buffer with '\0';
}
#if 1
char* xjb32(float v,char* buf)
{
    // all lut size = 56 byte
    // recommend buf size >= 24byte;

    u32 vi;
    memcpy(&vi, &v, 4);

    buf[0]='-';
    buf+=vi>>31;

    u64 dec,m;
    int e10;
    u32 tz;// tail zero
    //u32 dec_sig_len;// decimal length
    u32 dec_sig_len_ofs;// = dec_sig_len + 2
    u32 D9;// 1:9 digits 0:8 digits
    u32 sig = vi & ((1<<23) - 1);
    //u32 sig = (vi << 9 ) >> 9;
    u32 exp = (vi << 1 ) >> 24;

    // if( (vi << 1) == 0 )[[unlikely]]
    // {
    //     memcpy(buf, "0.0" , 4);//end with '\0'
    //     return buf + 3;
    // }
    // if(exp == 255)[[unlikely]]
    // {
    //     memcpy(buf, sig ? "NaN" : "Inf", 4);//end with '\0'
    //     return buf + 3;
    // }
    //memcpy(buf ,"0.000000",8);
    int exp_bin, k;
    u64 sig_bin, regular = sig > 0;
    if (exp > 0) [[likely]] // branch
    {
        if(exp == 255)[[unlikely]]
        {
            memcpy(buf, sig ? "NaN" : "Inf", 4);//end with '\0'
            return buf + 3;
        }
        exp_bin = exp - 150; //-127-23
        sig_bin = sig | (1u << 23);
    }
    else
    {
        if( (vi << 1) == 0 )[[unlikely]]
        {
            memcpy(buf, "0.0" , 4);//end with '\0'
            return buf + 3;
        }
        exp_bin = 1 - 150;
        sig_bin = sig;
    }
    //memcpy(buf ,"0.000000",8);
    u64 irregular = sig_bin == 1<<23;
//#ifdef __amd64__
#if 1
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
#else
    //k = (exp_bin * 315653 - (regular ? 0 : 131237 ))>>20;
    k = (exp_bin * 315653 - (irregular ? 131237 : 0 ))>>20;
#endif
    int get_e10 = -1 - k;// [-32,44]
    int h = exp_bin + ((get_e10 * 217707) >> 16); // [-4,-1]
    u32 p10_base_index = (u32)(get_e10 + 32) / 16;// [0,4]
    int p10_base = p10_base_index * 16 - 32;
    //u32 p5_off = get_e10 - p10_base;// [0,15]
    u32 p5_off = get_e10 + 32 - p10_base_index * 16;//[0,15]
    const u64 p5_4 = 5*5*5*5;
    static const u64 pow10_base_table_pow5[5 + 2 + 1] = { //40byte + 24byte = 64byte = cache line size
        //pow10_table
        0xcfb11ead453994bb, // e10 =  -32
        0xe69594bec44de15c, // e10 =  -16
        0x8000000000000000, // e10 =  0
        0x8e1bc9bf04000000, // e10 =  16
        0x9dc5ada82b70b59e, // e10 =  32
        //pow5_table
        1 + (p5_4<<32),
        ( p5_4*p5_4 ) + ( (p5_4*p5_4*p5_4) << 32),
        (125<<24) + (25<<16) + (5<<8) + 1
    };
    const u64 p5_0_3 = (125<<24) + (25<<16) + (5<<8) + 1;
    u64 pow10_base = pow10_base_table_pow5[p10_base_index];
    int shift = ((get_e10 * 217707) >> 16) - ((p10_base * 217707) >> 16) - p5_off;
    u32 pow5_base;
    u8 pow5_offset;
    static const char* start_ptr = ((char*)pow10_base_table_pow5) + 5 * sizeof(u64);
    static const char* start_ptr2 = ((char*)pow10_base_table_pow5) + 7 * sizeof(u64);
    memcpy(&pow5_base, &start_ptr[4 * (p5_off / 4)], 4);
    memcpy(&pow5_offset, &start_ptr2[(p5_off % 4)], 1);
    //u64 p5 = (u64)pow5_base * (u64)( (p5_0_3 >> ((p5_off % 4) * 8)) & 0xff );// p5 = 5**p5_off
    u64 p5 = (u64)pow5_base * (u64)pow5_offset;
    // u64 p5 = ((((p5_off / 4 > 1) ?  ( p5_4*p5_4 ) + ( (p5_4*p5_4*p5_4) << 32) : 1 + (p5_4<<32)) >> ((p5_off & 4) * 8 )) & ((1ull<<32) - 1))
    //             * (u64)( (p5_0_3 >> ((p5_off % 4) * 8)) & 0xff );// this code direct calc p5, but slow than above code

    // u64 p5 = 1;
    // u64 p5_base = 5;
    // //for(int i=0;i<p5_off;++i)p5*=5;
    // while (p5_off > 0)
    // {
    //     if (p5_off & 1)
    //         p5 *= p5_base;
    //     p5_base *= p5_base;
    //     p5_off >>= 1;
    // }

    u64 pow10_hi = ( (__uint128_t)pow10_base * p5 ) >> shift;
    u64 even = ((sig_bin + 1) & 1);
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64; // one mulxq instruction on x86 , need BMI2
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = (pow10_hi >> ((64 - BIT) - h)) + even;
#ifdef __amd64__
    u64 up = (half_ulp + dot_one_36bit) >> BIT;// 1 or 0
#else
    u64 up = (half_ulp  > (((u64)1 << BIT) - 1) - dot_one_36bit);
#endif
    u64 down =  ((half_ulp >> (irregular)) > dot_one_36bit);//1 or 0
    u64 up_down = up + down;// 1 or 0
    m = (sig_hi >> BIT) + up;
    D9 = m >= (u32)1e7;
    memcpy(buf ,"0.000000",8);
    u64 ASCII_8;
    tz = encode_8digit_fast(m,&ASCII_8);//When m=0, the return value tz is not used. because of up_down is must be 0. m=0 => updown=0;
#if yy_is_real_gcc
    // use this code to prevent gcc compiler generate branch instructions
    u64 dec_sig_len = ( ( (8)*256 +8 - tz*256  + D9 ) >> (up_down ? 8 : 0)) & 0xff;
#else
    // icpx clang use this code to generate cmov instructions
    //dec_sig_len_ofs = up_down ? 2+8 - tz : 2+8 + D9;// when mr = 0, up_down = 0, so can avoid use tz
    u64 dec_sig_len = up_down ? 8 - tz : 8 + D9;
#endif
    k += 7 + D9;
    e10 = k;// euqal to e10 = k+7+D9
    u64 offset_num  = ((u64)('0' + '0' * 256) << (BIT - 1)) + (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
    if(irregular)[[unlikely]]{ // branch
        if( (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150) )
            one+=1;
    }
    const int e10_DN = -3;
    const int e10_UP = 7;
#if 0
    // size = 12*12 = 144 byte
    static const u8 e10_variable_data[e10_UP-(e10_DN) + 1 + 1][3+9]={
4,1,1,1,2,3,4,5,6,7,8,9,// e10=-3
3,1,1,1,2,3,4,5,6,7,8,9,// e10=-2
2,1,1,1,2,3,4,5,6,7,8,9,// e10=-1
0,1,2,3,3,4,5,6,7,8,9,10,// e10=0
0,2,3,4,4,4,5,6,7,8,9,10,// e10=1
0,3,4,5,5,5,5,6,7,8,9,10,// e10=2
0,4,5,6,6,6,6,6,7,8,9,10,// e10=3
0,5,6,7,7,7,7,7,7,8,9,10,// e10=4
0,6,7,8,8,8,8,8,8,8,9,10,// e10=5
0,7,8,9,9,9,9,9,9,9,9,10,// e10=6
0,8,9,10,10,10,10,10,10,10,10,10,// e10=7
0,1,2,1,3,4,5,6,7,8,9,10// e10=other
        };
    u64 e10_3 = e10 + (-e10_DN);//convert to unsigned number
    u64 e10_data_ofs = e10_3 < e10_UP-e10_DN+1 ? e10_3 : e10_UP-e10_DN+1;//compute offset
    u64 first_sig_pos = e10_variable_data[e10_data_ofs][0];  // we use lookup table to get first_sig_pos
    u64 dot_pos = e10_variable_data[e10_data_ofs][1];
    u64 move_pos = e10_variable_data[e10_data_ofs][2];
    u64 exp_pos = e10_variable_data[e10_data_ofs][dec_sig_len_ofs];
#else
    u64 first_sig_pos = ( e10_DN <= e10 && e10 <= -1 ) ? 1 - e10 : 0;
    u64 dot_pos = ( 0 <= e10 && e10<= e10_UP ) ? 1 + e10 : 1;
    u64 move_pos = dot_pos + (1 - ( e10_DN <= e10 && e10 <= -1 ) );
    u64 exp_pos = (0<=e10 && e10<= e10_UP) ? (e10+3 > dec_sig_len+1 ? e10+3 : dec_sig_len+1) : (
                dec_sig_len + ( (1 - ( e10_DN <= e10 && e10 <= -1 )) > (dec_sig_len == 1) )
            );
#endif
    char* buf_origin = (char*)buf;
    buf += first_sig_pos;
    memcpy(buf, &ASCII_8, 8);
    memcpy(&buf[7 + D9], &one, 8);
    byte_move_8(&buf[move_pos] , &buf[dot_pos]);
    buf_origin[dot_pos] = '.';
    if(m < (u32)1e6 )[[unlikely]]
    {
        u64 lz = 0;
        while(buf[2+lz] == '0')lz++;
        lz += 2;
        e10 -= lz - 1;
        buf[0] = buf[lz];
        byte_move_8(&buf[2], &buf[lz+1]);
        exp_pos = exp_pos - lz + 1 - (exp_pos - lz == 1 );
    }
//write exponent
    u64 neg = e10 < 0;
    u64 e10_abs = neg ? -e10 : e10;
    u64 e = neg ? ('e' + '-' * 256) | ((u64)('0' + '0' * 256) << 16) : ('e' + '+' * 256) | ((u64)('0' + '0' * 256) << 16);
    u64 e10_BCD = (e10_abs << 24) - ((u64)(256 * 10 - 1) << 16) * ((e10_abs * 103u) >> 10); // 12 => "12"
    u64 exp_result = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : e + e10_BCD;// e10_DN<=e10 && e10<=e10_UP : no need to print exponent
    buf += exp_pos;
    memcpy(buf, &exp_result, 8);
    u64 exp_len = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : 4;
    return buf + exp_len;
}
#endif
}

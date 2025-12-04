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
namespace xjb64_32 {

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
    #endif // endif HAS_SSE2
#else
    u64 endcode_16digit_fast(const u64 v,byte16_reg* ASCII)
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
    const u64 ZERO = (0x30303030ull << 32) + 0x30303030ull;
    u64 aabbccdd = x;
    u64 aabb_ccdd_merge = (aabbccdd << 32) - ((10000ull<<32) - 1) * ((aabbccdd * 109951163) >> 40);
    u64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) - ((100ull<<16) - 1) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) - ((10ull<<8) - 1) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
    aabbccdd_BCD = (x >= (u64)1e7) ? aabbccdd_BCD : (aabbccdd_BCD >> 8);
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
//extern "C"
char* xjb64(double v,char* buf)
{
    u64 vi;
    memcpy(&vi, &v, sizeof(v));//double to u64 bit copy
    u64 sign = vi>>63;
    buf[0]='-';
    buf+=sign;
    
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
    if  ( (vi<<1) - 3 >= (2047ull<<53) - 3 )[[unlikely]]
    {
        // *(u64*)buf = ((vi << 1) < 3) ? ((vi << 1) ? *(u64*)"5e-324\0" : *(u32*)"0.0")
        //                              : (vi << 1) == (2047ull<<53) ? *(u32*)"Inf" : *(u32*)"NaN";//end with '\0'
        if( (vi << 1) == 0 )memcpy(buf , "0.0", 4);
        if( (vi << 1) == (1 << 1) )memcpy(buf , "5e-324\0", 8);
        if( (vi << 1) == (2047ull<<53) )memcpy(buf , "Inf", 4);
        if( (vi << 1) > (2047ull<<53) )memcpy(buf , "NaN", 4);
        return buf + ((vi << 1) - 3 == (u64)-1 ? 6 : 3);//end with '\0'
    }
    //*(u64*)buf = *(u64*)"0.00000";
    memcpy(buf, "0.000000", 8);
    u64 c;
    int32_t q;
#ifdef __amd64__    
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
    static const u64 exp_result_precalc[324 + 308 + 1]={
0x500003432332d65, // e10=-324
0x500003332332d65, // e10=-323
0x500003232332d65, // e10=-322
0x500003132332d65, // e10=-321
0x500003032332d65, // e10=-320
0x500003931332d65, // e10=-319
0x500003831332d65, // e10=-318
0x500003731332d65, // e10=-317
0x500003631332d65, // e10=-316
0x500003531332d65, // e10=-315
0x500003431332d65, // e10=-314
0x500003331332d65, // e10=-313
0x500003231332d65, // e10=-312
0x500003131332d65, // e10=-311
0x500003031332d65, // e10=-310
0x500003930332d65, // e10=-309
0x500003830332d65, // e10=-308
0x500003730332d65, // e10=-307
0x500003630332d65, // e10=-306
0x500003530332d65, // e10=-305
0x500003430332d65, // e10=-304
0x500003330332d65, // e10=-303
0x500003230332d65, // e10=-302
0x500003130332d65, // e10=-301
0x500003030332d65, // e10=-300
0x500003939322d65, // e10=-299
0x500003839322d65, // e10=-298
0x500003739322d65, // e10=-297
0x500003639322d65, // e10=-296
0x500003539322d65, // e10=-295
0x500003439322d65, // e10=-294
0x500003339322d65, // e10=-293
0x500003239322d65, // e10=-292
0x500003139322d65, // e10=-291
0x500003039322d65, // e10=-290
0x500003938322d65, // e10=-289
0x500003838322d65, // e10=-288
0x500003738322d65, // e10=-287
0x500003638322d65, // e10=-286
0x500003538322d65, // e10=-285
0x500003438322d65, // e10=-284
0x500003338322d65, // e10=-283
0x500003238322d65, // e10=-282
0x500003138322d65, // e10=-281
0x500003038322d65, // e10=-280
0x500003937322d65, // e10=-279
0x500003837322d65, // e10=-278
0x500003737322d65, // e10=-277
0x500003637322d65, // e10=-276
0x500003537322d65, // e10=-275
0x500003437322d65, // e10=-274
0x500003337322d65, // e10=-273
0x500003237322d65, // e10=-272
0x500003137322d65, // e10=-271
0x500003037322d65, // e10=-270
0x500003936322d65, // e10=-269
0x500003836322d65, // e10=-268
0x500003736322d65, // e10=-267
0x500003636322d65, // e10=-266
0x500003536322d65, // e10=-265
0x500003436322d65, // e10=-264
0x500003336322d65, // e10=-263
0x500003236322d65, // e10=-262
0x500003136322d65, // e10=-261
0x500003036322d65, // e10=-260
0x500003935322d65, // e10=-259
0x500003835322d65, // e10=-258
0x500003735322d65, // e10=-257
0x500003635322d65, // e10=-256
0x500003535322d65, // e10=-255
0x500003435322d65, // e10=-254
0x500003335322d65, // e10=-253
0x500003235322d65, // e10=-252
0x500003135322d65, // e10=-251
0x500003035322d65, // e10=-250
0x500003934322d65, // e10=-249
0x500003834322d65, // e10=-248
0x500003734322d65, // e10=-247
0x500003634322d65, // e10=-246
0x500003534322d65, // e10=-245
0x500003434322d65, // e10=-244
0x500003334322d65, // e10=-243
0x500003234322d65, // e10=-242
0x500003134322d65, // e10=-241
0x500003034322d65, // e10=-240
0x500003933322d65, // e10=-239
0x500003833322d65, // e10=-238
0x500003733322d65, // e10=-237
0x500003633322d65, // e10=-236
0x500003533322d65, // e10=-235
0x500003433322d65, // e10=-234
0x500003333322d65, // e10=-233
0x500003233322d65, // e10=-232
0x500003133322d65, // e10=-231
0x500003033322d65, // e10=-230
0x500003932322d65, // e10=-229
0x500003832322d65, // e10=-228
0x500003732322d65, // e10=-227
0x500003632322d65, // e10=-226
0x500003532322d65, // e10=-225
0x500003432322d65, // e10=-224
0x500003332322d65, // e10=-223
0x500003232322d65, // e10=-222
0x500003132322d65, // e10=-221
0x500003032322d65, // e10=-220
0x500003931322d65, // e10=-219
0x500003831322d65, // e10=-218
0x500003731322d65, // e10=-217
0x500003631322d65, // e10=-216
0x500003531322d65, // e10=-215
0x500003431322d65, // e10=-214
0x500003331322d65, // e10=-213
0x500003231322d65, // e10=-212
0x500003131322d65, // e10=-211
0x500003031322d65, // e10=-210
0x500003930322d65, // e10=-209
0x500003830322d65, // e10=-208
0x500003730322d65, // e10=-207
0x500003630322d65, // e10=-206
0x500003530322d65, // e10=-205
0x500003430322d65, // e10=-204
0x500003330322d65, // e10=-203
0x500003230322d65, // e10=-202
0x500003130322d65, // e10=-201
0x500003030322d65, // e10=-200
0x500003939312d65, // e10=-199
0x500003839312d65, // e10=-198
0x500003739312d65, // e10=-197
0x500003639312d65, // e10=-196
0x500003539312d65, // e10=-195
0x500003439312d65, // e10=-194
0x500003339312d65, // e10=-193
0x500003239312d65, // e10=-192
0x500003139312d65, // e10=-191
0x500003039312d65, // e10=-190
0x500003938312d65, // e10=-189
0x500003838312d65, // e10=-188
0x500003738312d65, // e10=-187
0x500003638312d65, // e10=-186
0x500003538312d65, // e10=-185
0x500003438312d65, // e10=-184
0x500003338312d65, // e10=-183
0x500003238312d65, // e10=-182
0x500003138312d65, // e10=-181
0x500003038312d65, // e10=-180
0x500003937312d65, // e10=-179
0x500003837312d65, // e10=-178
0x500003737312d65, // e10=-177
0x500003637312d65, // e10=-176
0x500003537312d65, // e10=-175
0x500003437312d65, // e10=-174
0x500003337312d65, // e10=-173
0x500003237312d65, // e10=-172
0x500003137312d65, // e10=-171
0x500003037312d65, // e10=-170
0x500003936312d65, // e10=-169
0x500003836312d65, // e10=-168
0x500003736312d65, // e10=-167
0x500003636312d65, // e10=-166
0x500003536312d65, // e10=-165
0x500003436312d65, // e10=-164
0x500003336312d65, // e10=-163
0x500003236312d65, // e10=-162
0x500003136312d65, // e10=-161
0x500003036312d65, // e10=-160
0x500003935312d65, // e10=-159
0x500003835312d65, // e10=-158
0x500003735312d65, // e10=-157
0x500003635312d65, // e10=-156
0x500003535312d65, // e10=-155
0x500003435312d65, // e10=-154
0x500003335312d65, // e10=-153
0x500003235312d65, // e10=-152
0x500003135312d65, // e10=-151
0x500003035312d65, // e10=-150
0x500003934312d65, // e10=-149
0x500003834312d65, // e10=-148
0x500003734312d65, // e10=-147
0x500003634312d65, // e10=-146
0x500003534312d65, // e10=-145
0x500003434312d65, // e10=-144
0x500003334312d65, // e10=-143
0x500003234312d65, // e10=-142
0x500003134312d65, // e10=-141
0x500003034312d65, // e10=-140
0x500003933312d65, // e10=-139
0x500003833312d65, // e10=-138
0x500003733312d65, // e10=-137
0x500003633312d65, // e10=-136
0x500003533312d65, // e10=-135
0x500003433312d65, // e10=-134
0x500003333312d65, // e10=-133
0x500003233312d65, // e10=-132
0x500003133312d65, // e10=-131
0x500003033312d65, // e10=-130
0x500003932312d65, // e10=-129
0x500003832312d65, // e10=-128
0x500003732312d65, // e10=-127
0x500003632312d65, // e10=-126
0x500003532312d65, // e10=-125
0x500003432312d65, // e10=-124
0x500003332312d65, // e10=-123
0x500003232312d65, // e10=-122
0x500003132312d65, // e10=-121
0x500003032312d65, // e10=-120
0x500003931312d65, // e10=-119
0x500003831312d65, // e10=-118
0x500003731312d65, // e10=-117
0x500003631312d65, // e10=-116
0x500003531312d65, // e10=-115
0x500003431312d65, // e10=-114
0x500003331312d65, // e10=-113
0x500003231312d65, // e10=-112
0x500003131312d65, // e10=-111
0x500003031312d65, // e10=-110
0x500003930312d65, // e10=-109
0x500003830312d65, // e10=-108
0x500003730312d65, // e10=-107
0x500003630312d65, // e10=-106
0x500003530312d65, // e10=-105
0x500003430312d65, // e10=-104
0x500003330312d65, // e10=-103
0x500003230312d65, // e10=-102
0x500003130312d65, // e10=-101
0x500003030312d65, // e10=-100
0x404000039392d65, // e10=-99
0x404000038392d65, // e10=-98
0x404000037392d65, // e10=-97
0x404000036392d65, // e10=-96
0x404000035392d65, // e10=-95
0x404000034392d65, // e10=-94
0x404000033392d65, // e10=-93
0x404000032392d65, // e10=-92
0x404000031392d65, // e10=-91
0x404000030392d65, // e10=-90
0x404000039382d65, // e10=-89
0x404000038382d65, // e10=-88
0x404000037382d65, // e10=-87
0x404000036382d65, // e10=-86
0x404000035382d65, // e10=-85
0x404000034382d65, // e10=-84
0x404000033382d65, // e10=-83
0x404000032382d65, // e10=-82
0x404000031382d65, // e10=-81
0x404000030382d65, // e10=-80
0x404000039372d65, // e10=-79
0x404000038372d65, // e10=-78
0x404000037372d65, // e10=-77
0x404000036372d65, // e10=-76
0x404000035372d65, // e10=-75
0x404000034372d65, // e10=-74
0x404000033372d65, // e10=-73
0x404000032372d65, // e10=-72
0x404000031372d65, // e10=-71
0x404000030372d65, // e10=-70
0x404000039362d65, // e10=-69
0x404000038362d65, // e10=-68
0x404000037362d65, // e10=-67
0x404000036362d65, // e10=-66
0x404000035362d65, // e10=-65
0x404000034362d65, // e10=-64
0x404000033362d65, // e10=-63
0x404000032362d65, // e10=-62
0x404000031362d65, // e10=-61
0x404000030362d65, // e10=-60
0x404000039352d65, // e10=-59
0x404000038352d65, // e10=-58
0x404000037352d65, // e10=-57
0x404000036352d65, // e10=-56
0x404000035352d65, // e10=-55
0x404000034352d65, // e10=-54
0x404000033352d65, // e10=-53
0x404000032352d65, // e10=-52
0x404000031352d65, // e10=-51
0x404000030352d65, // e10=-50
0x404000039342d65, // e10=-49
0x404000038342d65, // e10=-48
0x404000037342d65, // e10=-47
0x404000036342d65, // e10=-46
0x404000035342d65, // e10=-45
0x404000034342d65, // e10=-44
0x404000033342d65, // e10=-43
0x404000032342d65, // e10=-42
0x404000031342d65, // e10=-41
0x404000030342d65, // e10=-40
0x404000039332d65, // e10=-39
0x404000038332d65, // e10=-38
0x404000037332d65, // e10=-37
0x404000036332d65, // e10=-36
0x404000035332d65, // e10=-35
0x404000034332d65, // e10=-34
0x404000033332d65, // e10=-33
0x404000032332d65, // e10=-32
0x404000031332d65, // e10=-31
0x404000030332d65, // e10=-30
0x404000039322d65, // e10=-29
0x404000038322d65, // e10=-28
0x404000037322d65, // e10=-27
0x404000036322d65, // e10=-26
0x404000035322d65, // e10=-25
0x404000034322d65, // e10=-24
0x404000033322d65, // e10=-23
0x404000032322d65, // e10=-22
0x404000031322d65, // e10=-21
0x404000030322d65, // e10=-20
0x404000039312d65, // e10=-19
0x404000038312d65, // e10=-18
0x404000037312d65, // e10=-17
0x404000036312d65, // e10=-16
0x404000035312d65, // e10=-15
0x404000034312d65, // e10=-14
0x404000033312d65, // e10=-13
0x404000032312d65, // e10=-12
0x404000031312d65, // e10=-11
0x404000030312d65, // e10=-10
0x404000039302d65, // e10=-9
0x404000038302d65, // e10=-8
0x404000037302d65, // e10=-7
0x404000036302d65, // e10=-6
0x404000035302d65, // e10=-5
0x404000034302d65, // e10=-4
0x0, // e10=-3
0x0, // e10=-2
0x0, // e10=-1
0x0, // e10=0
0x0, // e10=1
0x0, // e10=2
0x0, // e10=3
0x0, // e10=4
0x0, // e10=5
0x0, // e10=6
0x0, // e10=7
0x0, // e10=8
0x0, // e10=9
0x0, // e10=10
0x0, // e10=11
0x0, // e10=12
0x0, // e10=13
0x0, // e10=14
0x0, // e10=15
0x404000036312b65, // e10=16
0x404000037312b65, // e10=17
0x404000038312b65, // e10=18
0x404000039312b65, // e10=19
0x404000030322b65, // e10=20
0x404000031322b65, // e10=21
0x404000032322b65, // e10=22
0x404000033322b65, // e10=23
0x404000034322b65, // e10=24
0x404000035322b65, // e10=25
0x404000036322b65, // e10=26
0x404000037322b65, // e10=27
0x404000038322b65, // e10=28
0x404000039322b65, // e10=29
0x404000030332b65, // e10=30
0x404000031332b65, // e10=31
0x404000032332b65, // e10=32
0x404000033332b65, // e10=33
0x404000034332b65, // e10=34
0x404000035332b65, // e10=35
0x404000036332b65, // e10=36
0x404000037332b65, // e10=37
0x404000038332b65, // e10=38
0x404000039332b65, // e10=39
0x404000030342b65, // e10=40
0x404000031342b65, // e10=41
0x404000032342b65, // e10=42
0x404000033342b65, // e10=43
0x404000034342b65, // e10=44
0x404000035342b65, // e10=45
0x404000036342b65, // e10=46
0x404000037342b65, // e10=47
0x404000038342b65, // e10=48
0x404000039342b65, // e10=49
0x404000030352b65, // e10=50
0x404000031352b65, // e10=51
0x404000032352b65, // e10=52
0x404000033352b65, // e10=53
0x404000034352b65, // e10=54
0x404000035352b65, // e10=55
0x404000036352b65, // e10=56
0x404000037352b65, // e10=57
0x404000038352b65, // e10=58
0x404000039352b65, // e10=59
0x404000030362b65, // e10=60
0x404000031362b65, // e10=61
0x404000032362b65, // e10=62
0x404000033362b65, // e10=63
0x404000034362b65, // e10=64
0x404000035362b65, // e10=65
0x404000036362b65, // e10=66
0x404000037362b65, // e10=67
0x404000038362b65, // e10=68
0x404000039362b65, // e10=69
0x404000030372b65, // e10=70
0x404000031372b65, // e10=71
0x404000032372b65, // e10=72
0x404000033372b65, // e10=73
0x404000034372b65, // e10=74
0x404000035372b65, // e10=75
0x404000036372b65, // e10=76
0x404000037372b65, // e10=77
0x404000038372b65, // e10=78
0x404000039372b65, // e10=79
0x404000030382b65, // e10=80
0x404000031382b65, // e10=81
0x404000032382b65, // e10=82
0x404000033382b65, // e10=83
0x404000034382b65, // e10=84
0x404000035382b65, // e10=85
0x404000036382b65, // e10=86
0x404000037382b65, // e10=87
0x404000038382b65, // e10=88
0x404000039382b65, // e10=89
0x404000030392b65, // e10=90
0x404000031392b65, // e10=91
0x404000032392b65, // e10=92
0x404000033392b65, // e10=93
0x404000034392b65, // e10=94
0x404000035392b65, // e10=95
0x404000036392b65, // e10=96
0x404000037392b65, // e10=97
0x404000038392b65, // e10=98
0x404000039392b65, // e10=99
0x500003030312b65, // e10=100
0x500003130312b65, // e10=101
0x500003230312b65, // e10=102
0x500003330312b65, // e10=103
0x500003430312b65, // e10=104
0x500003530312b65, // e10=105
0x500003630312b65, // e10=106
0x500003730312b65, // e10=107
0x500003830312b65, // e10=108
0x500003930312b65, // e10=109
0x500003031312b65, // e10=110
0x500003131312b65, // e10=111
0x500003231312b65, // e10=112
0x500003331312b65, // e10=113
0x500003431312b65, // e10=114
0x500003531312b65, // e10=115
0x500003631312b65, // e10=116
0x500003731312b65, // e10=117
0x500003831312b65, // e10=118
0x500003931312b65, // e10=119
0x500003032312b65, // e10=120
0x500003132312b65, // e10=121
0x500003232312b65, // e10=122
0x500003332312b65, // e10=123
0x500003432312b65, // e10=124
0x500003532312b65, // e10=125
0x500003632312b65, // e10=126
0x500003732312b65, // e10=127
0x500003832312b65, // e10=128
0x500003932312b65, // e10=129
0x500003033312b65, // e10=130
0x500003133312b65, // e10=131
0x500003233312b65, // e10=132
0x500003333312b65, // e10=133
0x500003433312b65, // e10=134
0x500003533312b65, // e10=135
0x500003633312b65, // e10=136
0x500003733312b65, // e10=137
0x500003833312b65, // e10=138
0x500003933312b65, // e10=139
0x500003034312b65, // e10=140
0x500003134312b65, // e10=141
0x500003234312b65, // e10=142
0x500003334312b65, // e10=143
0x500003434312b65, // e10=144
0x500003534312b65, // e10=145
0x500003634312b65, // e10=146
0x500003734312b65, // e10=147
0x500003834312b65, // e10=148
0x500003934312b65, // e10=149
0x500003035312b65, // e10=150
0x500003135312b65, // e10=151
0x500003235312b65, // e10=152
0x500003335312b65, // e10=153
0x500003435312b65, // e10=154
0x500003535312b65, // e10=155
0x500003635312b65, // e10=156
0x500003735312b65, // e10=157
0x500003835312b65, // e10=158
0x500003935312b65, // e10=159
0x500003036312b65, // e10=160
0x500003136312b65, // e10=161
0x500003236312b65, // e10=162
0x500003336312b65, // e10=163
0x500003436312b65, // e10=164
0x500003536312b65, // e10=165
0x500003636312b65, // e10=166
0x500003736312b65, // e10=167
0x500003836312b65, // e10=168
0x500003936312b65, // e10=169
0x500003037312b65, // e10=170
0x500003137312b65, // e10=171
0x500003237312b65, // e10=172
0x500003337312b65, // e10=173
0x500003437312b65, // e10=174
0x500003537312b65, // e10=175
0x500003637312b65, // e10=176
0x500003737312b65, // e10=177
0x500003837312b65, // e10=178
0x500003937312b65, // e10=179
0x500003038312b65, // e10=180
0x500003138312b65, // e10=181
0x500003238312b65, // e10=182
0x500003338312b65, // e10=183
0x500003438312b65, // e10=184
0x500003538312b65, // e10=185
0x500003638312b65, // e10=186
0x500003738312b65, // e10=187
0x500003838312b65, // e10=188
0x500003938312b65, // e10=189
0x500003039312b65, // e10=190
0x500003139312b65, // e10=191
0x500003239312b65, // e10=192
0x500003339312b65, // e10=193
0x500003439312b65, // e10=194
0x500003539312b65, // e10=195
0x500003639312b65, // e10=196
0x500003739312b65, // e10=197
0x500003839312b65, // e10=198
0x500003939312b65, // e10=199
0x500003030322b65, // e10=200
0x500003130322b65, // e10=201
0x500003230322b65, // e10=202
0x500003330322b65, // e10=203
0x500003430322b65, // e10=204
0x500003530322b65, // e10=205
0x500003630322b65, // e10=206
0x500003730322b65, // e10=207
0x500003830322b65, // e10=208
0x500003930322b65, // e10=209
0x500003031322b65, // e10=210
0x500003131322b65, // e10=211
0x500003231322b65, // e10=212
0x500003331322b65, // e10=213
0x500003431322b65, // e10=214
0x500003531322b65, // e10=215
0x500003631322b65, // e10=216
0x500003731322b65, // e10=217
0x500003831322b65, // e10=218
0x500003931322b65, // e10=219
0x500003032322b65, // e10=220
0x500003132322b65, // e10=221
0x500003232322b65, // e10=222
0x500003332322b65, // e10=223
0x500003432322b65, // e10=224
0x500003532322b65, // e10=225
0x500003632322b65, // e10=226
0x500003732322b65, // e10=227
0x500003832322b65, // e10=228
0x500003932322b65, // e10=229
0x500003033322b65, // e10=230
0x500003133322b65, // e10=231
0x500003233322b65, // e10=232
0x500003333322b65, // e10=233
0x500003433322b65, // e10=234
0x500003533322b65, // e10=235
0x500003633322b65, // e10=236
0x500003733322b65, // e10=237
0x500003833322b65, // e10=238
0x500003933322b65, // e10=239
0x500003034322b65, // e10=240
0x500003134322b65, // e10=241
0x500003234322b65, // e10=242
0x500003334322b65, // e10=243
0x500003434322b65, // e10=244
0x500003534322b65, // e10=245
0x500003634322b65, // e10=246
0x500003734322b65, // e10=247
0x500003834322b65, // e10=248
0x500003934322b65, // e10=249
0x500003035322b65, // e10=250
0x500003135322b65, // e10=251
0x500003235322b65, // e10=252
0x500003335322b65, // e10=253
0x500003435322b65, // e10=254
0x500003535322b65, // e10=255
0x500003635322b65, // e10=256
0x500003735322b65, // e10=257
0x500003835322b65, // e10=258
0x500003935322b65, // e10=259
0x500003036322b65, // e10=260
0x500003136322b65, // e10=261
0x500003236322b65, // e10=262
0x500003336322b65, // e10=263
0x500003436322b65, // e10=264
0x500003536322b65, // e10=265
0x500003636322b65, // e10=266
0x500003736322b65, // e10=267
0x500003836322b65, // e10=268
0x500003936322b65, // e10=269
0x500003037322b65, // e10=270
0x500003137322b65, // e10=271
0x500003237322b65, // e10=272
0x500003337322b65, // e10=273
0x500003437322b65, // e10=274
0x500003537322b65, // e10=275
0x500003637322b65, // e10=276
0x500003737322b65, // e10=277
0x500003837322b65, // e10=278
0x500003937322b65, // e10=279
0x500003038322b65, // e10=280
0x500003138322b65, // e10=281
0x500003238322b65, // e10=282
0x500003338322b65, // e10=283
0x500003438322b65, // e10=284
0x500003538322b65, // e10=285
0x500003638322b65, // e10=286
0x500003738322b65, // e10=287
0x500003838322b65, // e10=288
0x500003938322b65, // e10=289
0x500003039322b65, // e10=290
0x500003139322b65, // e10=291
0x500003239322b65, // e10=292
0x500003339322b65, // e10=293
0x500003439322b65, // e10=294
0x500003539322b65, // e10=295
0x500003639322b65, // e10=296
0x500003739322b65, // e10=297
0x500003839322b65, // e10=298
0x500003939322b65, // e10=299
0x500003030332b65, // e10=300
0x500003130332b65, // e10=301
0x500003230332b65, // e10=302
0x500003330332b65, // e10=303
0x500003430332b65, // e10=304
0x500003530332b65, // e10=305
0x500003630332b65, // e10=306
0x500003730332b65, // e10=307
0x500003830332b65, // e10=308
    };
    static const u64 bitarray_irregular[32] = {
        0x0000000000010040, 0x0000000000000004, 0x0000000000000000, 0x0020090000000000, 
        0x0000000000000000, 0x0000000000000100, 0x0000000000000000, 0x0000000000000000, 
        0x0000000000400000, 0x0000000000000000, 0x0000000000020000, 0x0000000000800000, 
        0x0000000000000000, 0x0008000000000000, 0x0004000040000000, 0x0000000000000000, 
        0x0000000000000000, 0x0000000001000000, 0x0020000000000000, 0x0000000000000000, 
        0x0001000000040000, 0x0200000001000000, 0x0000000000102000, 0x0000000100000000, 
        0x2000000000000000, 0x0000000000020000, 0x0000000000000000, 0x0000000000000000, 
        0x0000000000000000, 0x0000000040000000, 0x0000000000000000, 0x0000000000008000};
    static const u64 g[(323 - (-293) + 1) * 2] = {
        0xcc5fc196fefd7d0c, 0x1e53ed49a96272c9, // -293
        0xff77b1fcbebcdc4f, 0x25e8e89c13bb0f7b, // -292
        0x9faacf3df73609b1, 0x77b191618c54e9ad, // -291
        0xc795830d75038c1d, 0xd59df5b9ef6a2418, // -290
        0xf97ae3d0d2446f25, 0x4b0573286b44ad1e, // -289
        0x9becce62836ac577, 0x4ee367f9430aec33, // -288
        0xc2e801fb244576d5, 0x229c41f793cda740, // -287
        0xf3a20279ed56d48a, 0x6b43527578c11110, // -286
        0x9845418c345644d6, 0x830a13896b78aaaa, // -285
        0xbe5691ef416bd60c, 0x23cc986bc656d554, // -284
        0xedec366b11c6cb8f, 0x2cbfbe86b7ec8aa9, // -283
        0x94b3a202eb1c3f39, 0x7bf7d71432f3d6aa, // -282
        0xb9e08a83a5e34f07, 0xdaf5ccd93fb0cc54, // -281
        0xe858ad248f5c22c9, 0xd1b3400f8f9cff69, // -280
        0x91376c36d99995be, 0x23100809b9c21fa2, // -279
        0xb58547448ffffb2d, 0xabd40a0c2832a78b, // -278
        0xe2e69915b3fff9f9, 0x16c90c8f323f516d, // -277
        0x8dd01fad907ffc3b, 0xae3da7d97f6792e4, // -276
        0xb1442798f49ffb4a, 0x99cd11cfdf41779d, // -275
        0xdd95317f31c7fa1d, 0x40405643d711d584, // -274
        0x8a7d3eef7f1cfc52, 0x482835ea666b2573, // -273
        0xad1c8eab5ee43b66, 0xda3243650005eed0, // -272
        0xd863b256369d4a40, 0x90bed43e40076a83, // -271
        0x873e4f75e2224e68, 0x5a7744a6e804a292, // -270
        0xa90de3535aaae202, 0x711515d0a205cb37, // -269
        0xd3515c2831559a83, 0xd5a5b44ca873e04,  // -268
        0x8412d9991ed58091, 0xe858790afe9486c3, // -267
        0xa5178fff668ae0b6, 0x626e974dbe39a873, // -266
        0xce5d73ff402d98e3, 0xfb0a3d212dc81290, // -265
        0x80fa687f881c7f8e, 0x7ce66634bc9d0b9a, // -264
        0xa139029f6a239f72, 0x1c1fffc1ebc44e81, // -263
        0xc987434744ac874e, 0xa327ffb266b56221, // -262
        0xfbe9141915d7a922, 0x4bf1ff9f0062baa9, // -261
        0x9d71ac8fada6c9b5, 0x6f773fc3603db4aa, // -260
        0xc4ce17b399107c22, 0xcb550fb4384d21d4, // -259
        0xf6019da07f549b2b, 0x7e2a53a146606a49, // -258
        0x99c102844f94e0fb, 0x2eda7444cbfc426e, // -257
        0xc0314325637a1939, 0xfa911155fefb5309, // -256
        0xf03d93eebc589f88, 0x793555ab7eba27cb, // -255
        0x96267c7535b763b5, 0x4bc1558b2f3458df, // -254
        0xbbb01b9283253ca2, 0x9eb1aaedfb016f17, // -253
        0xea9c227723ee8bcb, 0x465e15a979c1cadd, // -252
        0x92a1958a7675175f, 0xbfacd89ec191eca,  // -251
        0xb749faed14125d36, 0xcef980ec671f667c, // -250
        0xe51c79a85916f484, 0x82b7e12780e7401b, // -249
        0x8f31cc0937ae58d2, 0xd1b2ecb8b0908811, // -248
        0xb2fe3f0b8599ef07, 0x861fa7e6dcb4aa16, // -247
        0xdfbdcece67006ac9, 0x67a791e093e1d49b, // -246
        0x8bd6a141006042bd, 0xe0c8bb2c5c6d24e1, // -245
        0xaecc49914078536d, 0x58fae9f773886e19, // -244
        0xda7f5bf590966848, 0xaf39a475506a899f, // -243
        0x888f99797a5e012d, 0x6d8406c952429604, // -242
        0xaab37fd7d8f58178, 0xc8e5087ba6d33b84, // -241
        0xd5605fcdcf32e1d6, 0xfb1e4a9a90880a65, // -240
        0x855c3be0a17fcd26, 0x5cf2eea09a550680, // -239
        0xa6b34ad8c9dfc06f, 0xf42faa48c0ea481f, // -238
        0xd0601d8efc57b08b, 0xf13b94daf124da27, // -237
        0x823c12795db6ce57, 0x76c53d08d6b70859, // -236
        0xa2cb1717b52481ed, 0x54768c4b0c64ca6f, // -235
        0xcb7ddcdda26da268, 0xa9942f5dcf7dfd0a, // -234
        0xfe5d54150b090b02, 0xd3f93b35435d7c4d, // -233
        0x9efa548d26e5a6e1, 0xc47bc5014a1a6db0, // -232
        0xc6b8e9b0709f109a, 0x359ab6419ca1091c, // -231
        0xf867241c8cc6d4c0, 0xc30163d203c94b63, // -230
        0x9b407691d7fc44f8, 0x79e0de63425dcf1e, // -229
        0xc21094364dfb5636, 0x985915fc12f542e5, // -228
        0xf294b943e17a2bc4, 0x3e6f5b7b17b2939e, // -227
        0x979cf3ca6cec5b5a, 0xa705992ceecf9c43, // -226
        0xbd8430bd08277231, 0x50c6ff782a838354, // -225
        0xece53cec4a314ebd, 0xa4f8bf5635246429, // -224
        0x940f4613ae5ed136, 0x871b7795e136be9a, // -223
        0xb913179899f68584, 0x28e2557b59846e40, // -222
        0xe757dd7ec07426e5, 0x331aeada2fe589d0, // -221
        0x9096ea6f3848984f, 0x3ff0d2c85def7622, // -220
        0xb4bca50b065abe63, 0xfed077a756b53aa,  // -219
        0xe1ebce4dc7f16dfb, 0xd3e8495912c62895, // -218
        0x8d3360f09cf6e4bd, 0x64712dd7abbbd95d, // -217
        0xb080392cc4349dec, 0xbd8d794d96aacfb4, // -216
        0xdca04777f541c567, 0xecf0d7a0fc5583a1, // -215
        0x89e42caaf9491b60, 0xf41686c49db57245, // -214
        0xac5d37d5b79b6239, 0x311c2875c522ced6, // -213
        0xd77485cb25823ac7, 0x7d633293366b828c, // -212
        0x86a8d39ef77164bc, 0xae5dff9c02033198, // -211
        0xa8530886b54dbdeb, 0xd9f57f830283fdfd, // -210
        0xd267caa862a12d66, 0xd072df63c324fd7c, // -209
        0x8380dea93da4bc60, 0x4247cb9e59f71e6e, // -208
        0xa46116538d0deb78, 0x52d9be85f074e609, // -207
        0xcd795be870516656, 0x67902e276c921f8c, // -206
        0x806bd9714632dff6, 0xba1cd8a3db53b7,   // -205
        0xa086cfcd97bf97f3, 0x80e8a40eccd228a5, // -204
        0xc8a883c0fdaf7df0, 0x6122cd128006b2ce, // -203
        0xfad2a4b13d1b5d6c, 0x796b805720085f82, // -202
        0x9cc3a6eec6311a63, 0xcbe3303674053bb1, // -201
        0xc3f490aa77bd60fc, 0xbedbfc4411068a9d, // -200
        0xf4f1b4d515acb93b, 0xee92fb5515482d45, // -199
        0x991711052d8bf3c5, 0x751bdd152d4d1c4b, // -198
        0xbf5cd54678eef0b6, 0xd262d45a78a0635e, // -197
        0xef340a98172aace4, 0x86fb897116c87c35, // -196
        0x9580869f0e7aac0e, 0xd45d35e6ae3d4da1, // -195
        0xbae0a846d2195712, 0x8974836059cca10a, // -194
        0xe998d258869facd7, 0x2bd1a438703fc94c, // -193
        0x91ff83775423cc06, 0x7b6306a34627ddd0, // -192
        0xb67f6455292cbf08, 0x1a3bc84c17b1d543, // -191
        0xe41f3d6a7377eeca, 0x20caba5f1d9e4a94, // -190
        0x8e938662882af53e, 0x547eb47b7282ee9d, // -189
        0xb23867fb2a35b28d, 0xe99e619a4f23aa44, // -188
        0xdec681f9f4c31f31, 0x6405fa00e2ec94d5, // -187
        0x8b3c113c38f9f37e, 0xde83bc408dd3dd05, // -186
        0xae0b158b4738705e, 0x9624ab50b148d446, // -185
        0xd98ddaee19068c76, 0x3badd624dd9b0958, // -184
        0x87f8a8d4cfa417c9, 0xe54ca5d70a80e5d7, // -183
        0xa9f6d30a038d1dbc, 0x5e9fcf4ccd211f4d, // -182
        0xd47487cc8470652b, 0x7647c32000696720, // -181
        0x84c8d4dfd2c63f3b, 0x29ecd9f40041e074, // -180
        0xa5fb0a17c777cf09, 0xf468107100525891, // -179
        0xcf79cc9db955c2cc, 0x7182148d4066eeb5, // -178
        0x81ac1fe293d599bf, 0xc6f14cd848405531, // -177
        0xa21727db38cb002f, 0xb8ada00e5a506a7d, // -176
        0xca9cf1d206fdc03b, 0xa6d90811f0e4851d, // -175
        0xfd442e4688bd304a, 0x908f4a166d1da664, // -174
        0x9e4a9cec15763e2e, 0x9a598e4e043287ff, // -173
        0xc5dd44271ad3cdba, 0x40eff1e1853f29fe, // -172
        0xf7549530e188c128, 0xd12bee59e68ef47d, // -171
        0x9a94dd3e8cf578b9, 0x82bb74f8301958cf, // -170
        0xc13a148e3032d6e7, 0xe36a52363c1faf02, // -169
        0xf18899b1bc3f8ca1, 0xdc44e6c3cb279ac2, // -168
        0x96f5600f15a7b7e5, 0x29ab103a5ef8c0ba, // -167
        0xbcb2b812db11a5de, 0x7415d448f6b6f0e8, // -166
        0xebdf661791d60f56, 0x111b495b3464ad22, // -165
        0x936b9fcebb25c995, 0xcab10dd900beec35, // -164
        0xb84687c269ef3bfb, 0x3d5d514f40eea743, // -163
        0xe65829b3046b0afa, 0xcb4a5a3112a5113,  // -162
        0x8ff71a0fe2c2e6dc, 0x47f0e785eaba72ac, // -161
        0xb3f4e093db73a093, 0x59ed216765690f57, // -160
        0xe0f218b8d25088b8, 0x306869c13ec3532d, // -159
        0x8c974f7383725573, 0x1e414218c73a13fc, // -158
        0xafbd2350644eeacf, 0xe5d1929ef90898fb, // -157
        0xdbac6c247d62a583, 0xdf45f746b74abf3a, // -156
        0x894bc396ce5da772, 0x6b8bba8c328eb784, // -155
        0xab9eb47c81f5114f, 0x66ea92f3f326565,  // -154
        0xd686619ba27255a2, 0xc80a537b0efefebe, // -153
        0x8613fd0145877585, 0xbd06742ce95f5f37, // -152
        0xa798fc4196e952e7, 0x2c48113823b73705, // -151
        0xd17f3b51fca3a7a0, 0xf75a15862ca504c6, // -150
        0x82ef85133de648c4, 0x9a984d73dbe722fc, // -149
        0xa3ab66580d5fdaf5, 0xc13e60d0d2e0ebbb, // -148
        0xcc963fee10b7d1b3, 0x318df905079926a9, // -147
        0xffbbcfe994e5c61f, 0xfdf17746497f7053, // -146
        0x9fd561f1fd0f9bd3, 0xfeb6ea8bedefa634, // -145
        0xc7caba6e7c5382c8, 0xfe64a52ee96b8fc1, // -144
        0xf9bd690a1b68637b, 0x3dfdce7aa3c673b1, // -143
        0x9c1661a651213e2d, 0x6bea10ca65c084f,  // -142
        0xc31bfa0fe5698db8, 0x486e494fcff30a63, // -141
        0xf3e2f893dec3f126, 0x5a89dba3c3efccfb, // -140
        0x986ddb5c6b3a76b7, 0xf89629465a75e01d, // -139
        0xbe89523386091465, 0xf6bbb397f1135824, // -138
        0xee2ba6c0678b597f, 0x746aa07ded582e2d, // -137
        0x94db483840b717ef, 0xa8c2a44eb4571cdd, // -136
        0xba121a4650e4ddeb, 0x92f34d62616ce414, // -135
        0xe896a0d7e51e1566, 0x77b020baf9c81d18, // -134
        0x915e2486ef32cd60, 0xace1474dc1d122f,  // -133
        0xb5b5ada8aaff80b8, 0xd819992132456bb,  // -132
        0xe3231912d5bf60e6, 0x10e1fff697ed6c6a, // -131
        0x8df5efabc5979c8f, 0xca8d3ffa1ef463c2, // -130
        0xb1736b96b6fd83b3, 0xbd308ff8a6b17cb3, // -129
        0xddd0467c64bce4a0, 0xac7cb3f6d05ddbdf, // -128
        0x8aa22c0dbef60ee4, 0x6bcdf07a423aa96c, // -127
        0xad4ab7112eb3929d, 0x86c16c98d2c953c7, // -126
        0xd89d64d57a607744, 0xe871c7bf077ba8b8, // -125
        0x87625f056c7c4a8b, 0x11471cd764ad4973, // -124
        0xa93af6c6c79b5d2d, 0xd598e40d3dd89bd0, // -123
        0xd389b47879823479, 0x4aff1d108d4ec2c4, // -122
        0x843610cb4bf160cb, 0xcedf722a585139bb, // -121
        0xa54394fe1eedb8fe, 0xc2974eb4ee658829, // -120
        0xce947a3da6a9273e, 0x733d226229feea33, // -119
        0x811ccc668829b887, 0x806357d5a3f5260,  // -118
        0xa163ff802a3426a8, 0xca07c2dcb0cf26f8, // -117
        0xc9bcff6034c13052, 0xfc89b393dd02f0b6, // -116
        0xfc2c3f3841f17c67, 0xbbac2078d443ace3, // -115
        0x9d9ba7832936edc0, 0xd54b944b84aa4c0e, // -114
        0xc5029163f384a931, 0xa9e795e65d4df12,  // -113
        0xf64335bcf065d37d, 0x4d4617b5ff4a16d6, // -112
        0x99ea0196163fa42e, 0x504bced1bf8e4e46, // -111
        0xc06481fb9bcf8d39, 0xe45ec2862f71e1d7, // -110
        0xf07da27a82c37088, 0x5d767327bb4e5a4d, // -109
        0x964e858c91ba2655, 0x3a6a07f8d510f870, // -108
        0xbbe226efb628afea, 0x890489f70a55368c, // -107
        0xeadab0aba3b2dbe5, 0x2b45ac74ccea842f, // -106
        0x92c8ae6b464fc96f, 0x3b0b8bc90012929e, // -105
        0xb77ada0617e3bbcb, 0x9ce6ebb40173745,  // -104
        0xe55990879ddcaabd, 0xcc420a6a101d0516, // -103
        0x8f57fa54c2a9eab6, 0x9fa946824a12232e, // -102
        0xb32df8e9f3546564, 0x47939822dc96abfa, // -101
        0xdff9772470297ebd, 0x59787e2b93bc56f8, // -100
        0x8bfbea76c619ef36, 0x57eb4edb3c55b65b, // -99
        0xaefae51477a06b03, 0xede622920b6b23f2, // -98
        0xdab99e59958885c4, 0xe95fab368e45ecee, // -97
        0x88b402f7fd75539b, 0x11dbcb0218ebb415, // -96
        0xaae103b5fcd2a881, 0xd652bdc29f26a11a, // -95
        0xd59944a37c0752a2, 0x4be76d3346f04960, // -94
        0x857fcae62d8493a5, 0x6f70a4400c562ddc, // -93
        0xa6dfbd9fb8e5b88e, 0xcb4ccd500f6bb953, // -92
        0xd097ad07a71f26b2, 0x7e2000a41346a7a8, // -91
        0x825ecc24c873782f, 0x8ed400668c0c28c9, // -90
        0xa2f67f2dfa90563b, 0x728900802f0f32fb, // -89
        0xcbb41ef979346bca, 0x4f2b40a03ad2ffba, // -88
        0xfea126b7d78186bc, 0xe2f610c84987bfa9, // -87
        0x9f24b832e6b0f436, 0xdd9ca7d2df4d7ca,  // -86
        0xc6ede63fa05d3143, 0x91503d1c79720dbc, // -85
        0xf8a95fcf88747d94, 0x75a44c6397ce912b, // -84
        0x9b69dbe1b548ce7c, 0xc986afbe3ee11abb, // -83
        0xc24452da229b021b, 0xfbe85badce996169, // -82
        0xf2d56790ab41c2a2, 0xfae27299423fb9c4, // -81
        0x97c560ba6b0919a5, 0xdccd879fc967d41b, // -80
        0xbdb6b8e905cb600f, 0x5400e987bbc1c921, // -79
        0xed246723473e3813, 0x290123e9aab23b69, // -78
        0x9436c0760c86e30b, 0xf9a0b6720aaf6522, // -77
        0xb94470938fa89bce, 0xf808e40e8d5b3e6a, // -76
        0xe7958cb87392c2c2, 0xb60b1d1230b20e05, // -75
        0x90bd77f3483bb9b9, 0xb1c6f22b5e6f48c3, // -74
        0xb4ecd5f01a4aa828, 0x1e38aeb6360b1af4, // -73
        0xe2280b6c20dd5232, 0x25c6da63c38de1b1, // -72
        0x8d590723948a535f, 0x579c487e5a38ad0f, // -71
        0xb0af48ec79ace837, 0x2d835a9df0c6d852, // -70
        0xdcdb1b2798182244, 0xf8e431456cf88e66, // -69
        0x8a08f0f8bf0f156b, 0x1b8e9ecb641b5900, // -68
        0xac8b2d36eed2dac5, 0xe272467e3d222f40, // -67
        0xd7adf884aa879177, 0x5b0ed81dcc6abb10, // -66
        0x86ccbb52ea94baea, 0x98e947129fc2b4ea, // -65
        0xa87fea27a539e9a5, 0x3f2398d747b36225, // -64
        0xd29fe4b18e88640e, 0x8eec7f0d19a03aae, // -63
        0x83a3eeeef9153e89, 0x1953cf68300424ad, // -62
        0xa48ceaaab75a8e2b, 0x5fa8c3423c052dd8, // -61
        0xcdb02555653131b6, 0x3792f412cb06794e, // -60
        0x808e17555f3ebf11, 0xe2bbd88bbee40bd1, // -59
        0xa0b19d2ab70e6ed6, 0x5b6aceaeae9d0ec5, // -58
        0xc8de047564d20a8b, 0xf245825a5a445276, // -57
        0xfb158592be068d2e, 0xeed6e2f0f0d56713, // -56
        0x9ced737bb6c4183d, 0x55464dd69685606c, // -55
        0xc428d05aa4751e4c, 0xaa97e14c3c26b887, // -54
        0xf53304714d9265df, 0xd53dd99f4b3066a9, // -53
        0x993fe2c6d07b7fab, 0xe546a8038efe402a, // -52
        0xbf8fdb78849a5f96, 0xde98520472bdd034, // -51
        0xef73d256a5c0f77c, 0x963e66858f6d4441, // -50
        0x95a8637627989aad, 0xdde7001379a44aa9, // -49
        0xbb127c53b17ec159, 0x5560c018580d5d53, // -48
        0xe9d71b689dde71af, 0xaab8f01e6e10b4a7, // -47
        0x9226712162ab070d, 0xcab3961304ca70e9, // -46
        0xb6b00d69bb55c8d1, 0x3d607b97c5fd0d23, // -45
        0xe45c10c42a2b3b05, 0x8cb89a7db77c506b, // -44
        0x8eb98a7a9a5b04e3, 0x77f3608e92adb243, // -43
        0xb267ed1940f1c61c, 0x55f038b237591ed4, // -42
        0xdf01e85f912e37a3, 0x6b6c46dec52f6689, // -41
        0x8b61313bbabce2c6, 0x2323ac4b3b3da016, // -40
        0xae397d8aa96c1b77, 0xabec975e0a0d081b, // -39
        0xd9c7dced53c72255, 0x96e7bd358c904a22, // -38
        0x881cea14545c7575, 0x7e50d64177da2e55, // -37
        0xaa242499697392d2, 0xdde50bd1d5d0b9ea, // -36
        0xd4ad2dbfc3d07787, 0x955e4ec64b44e865, // -35
        0x84ec3c97da624ab4, 0xbd5af13bef0b113f, // -34
        0xa6274bbdd0fadd61, 0xecb1ad8aeacdd58f, // -33
        0xcfb11ead453994ba, 0x67de18eda5814af3, // -32
        0x81ceb32c4b43fcf4, 0x80eacf948770ced8, // -31
        0xa2425ff75e14fc31, 0xa1258379a94d028e, // -30
        0xcad2f7f5359a3b3e, 0x96ee45813a04331,  // -29
        0xfd87b5f28300ca0d, 0x8bca9d6e188853fd, // -28
        0x9e74d1b791e07e48, 0x775ea264cf55347e, // -27
        0xc612062576589dda, 0x95364afe032a819e, // -26
        0xf79687aed3eec551, 0x3a83ddbd83f52205, // -25
        0x9abe14cd44753b52, 0xc4926a9672793543, // -24
        0xc16d9a0095928a27, 0x75b7053c0f178294, // -23
        0xf1c90080baf72cb1, 0x5324c68b12dd6339, // -22
        0x971da05074da7bee, 0xd3f6fc16ebca5e04, // -21
        0xbce5086492111aea, 0x88f4bb1ca6bcf585, // -20
        0xec1e4a7db69561a5, 0x2b31e9e3d06c32e6, // -19
        0x9392ee8e921d5d07, 0x3aff322e62439fd0, // -18
        0xb877aa3236a4b449, 0x9befeb9fad487c3,  // -17
        0xe69594bec44de15b, 0x4c2ebe687989a9b4, // -16
        0x901d7cf73ab0acd9, 0xf9d37014bf60a11,  // -15
        0xb424dc35095cd80f, 0x538484c19ef38c95, // -14
        0xe12e13424bb40e13, 0x2865a5f206b06fba, // -13
        0x8cbccc096f5088cb, 0xf93f87b7442e45d4, // -12
        0xafebff0bcb24aafe, 0xf78f69a51539d749, // -11
        0xdbe6fecebdedd5be, 0xb573440e5a884d1c, // -10
        0x89705f4136b4a597, 0x31680a88f8953031, // -9
        0xabcc77118461cefc, 0xfdc20d2b36ba7c3e, // -8
        0xd6bf94d5e57a42bc, 0x3d32907604691b4d, // -7
        0x8637bd05af6c69b5, 0xa63f9a49c2c1b110, // -6
        0xa7c5ac471b478423, 0xfcf80dc33721d54,  // -5
        0xd1b71758e219652b, 0xd3c36113404ea4a9, // -4
        0x83126e978d4fdf3b, 0x645a1cac083126ea, // -3
        0xa3d70a3d70a3d70a, 0x3d70a3d70a3d70a4, // -2
        0xcccccccccccccccc, 0xcccccccccccccccd, // -1
        0x8000000000000000, 0x0,                // 0
        0xa000000000000000, 0x0,                // 1
        0xc800000000000000, 0x0,                // 2
        0xfa00000000000000, 0x0,                // 3
        0x9c40000000000000, 0x0,                // 4
        0xc350000000000000, 0x0,                // 5
        0xf424000000000000, 0x0,                // 6
        0x9896800000000000, 0x0,                // 7
        0xbebc200000000000, 0x0,                // 8
        0xee6b280000000000, 0x0,                // 9
        0x9502f90000000000, 0x0,                // 10
        0xba43b74000000000, 0x0,                // 11
        0xe8d4a51000000000, 0x0,                // 12
        0x9184e72a00000000, 0x0,                // 13
        0xb5e620f480000000, 0x0,                // 14
        0xe35fa931a0000000, 0x0,                // 15
        0x8e1bc9bf04000000, 0x0,                // 16
        0xb1a2bc2ec5000000, 0x0,                // 17
        0xde0b6b3a76400000, 0x0,                // 18
        0x8ac7230489e80000, 0x0,                // 19
        0xad78ebc5ac620000, 0x0,                // 20
        0xd8d726b7177a8000, 0x0,                // 21
        0x878678326eac9000, 0x0,                // 22
        0xa968163f0a57b400, 0x0,                // 23
        0xd3c21bcecceda100, 0x0,                // 24
        0x84595161401484a0, 0x0,                // 25
        0xa56fa5b99019a5c8, 0x0,                // 26
        0xcecb8f27f4200f3a, 0x0,                // 27
        0x813f3978f8940984, 0x4000000000000000, // 28
        0xa18f07d736b90be5, 0x5000000000000000, // 29
        0xc9f2c9cd04674ede, 0xa400000000000000, // 30
        0xfc6f7c4045812296, 0x4d00000000000000, // 31
        0x9dc5ada82b70b59d, 0xf020000000000000, // 32
        0xc5371912364ce305, 0x6c28000000000000, // 33
        0xf684df56c3e01bc6, 0xc732000000000000, // 34
        0x9a130b963a6c115c, 0x3c7f400000000000, // 35
        0xc097ce7bc90715b3, 0x4b9f100000000000, // 36
        0xf0bdc21abb48db20, 0x1e86d40000000000, // 37
        0x96769950b50d88f4, 0x1314448000000000, // 38
        0xbc143fa4e250eb31, 0x17d955a000000000, // 39
        0xeb194f8e1ae525fd, 0x5dcfab0800000000, // 40
        0x92efd1b8d0cf37be, 0x5aa1cae500000000, // 41
        0xb7abc627050305ad, 0xf14a3d9e40000000, // 42
        0xe596b7b0c643c719, 0x6d9ccd05d0000000, // 43
        0x8f7e32ce7bea5c6f, 0xe4820023a2000000, // 44
        0xb35dbf821ae4f38b, 0xdda2802c8a800000, // 45
        0xe0352f62a19e306e, 0xd50b2037ad200000, // 46
        0x8c213d9da502de45, 0x4526f422cc340000, // 47
        0xaf298d050e4395d6, 0x9670b12b7f410000, // 48
        0xdaf3f04651d47b4c, 0x3c0cdd765f114000, // 49
        0x88d8762bf324cd0f, 0xa5880a69fb6ac800, // 50
        0xab0e93b6efee0053, 0x8eea0d047a457a00, // 51
        0xd5d238a4abe98068, 0x72a4904598d6d880, // 52
        0x85a36366eb71f041, 0x47a6da2b7f864750, // 53
        0xa70c3c40a64e6c51, 0x999090b65f67d924, // 54
        0xd0cf4b50cfe20765, 0xfff4b4e3f741cf6d, // 55
        0x82818f1281ed449f, 0xbff8f10e7a8921a5, // 56
        0xa321f2d7226895c7, 0xaff72d52192b6a0e, // 57
        0xcbea6f8ceb02bb39, 0x9bf4f8a69f764491, // 58
        0xfee50b7025c36a08, 0x2f236d04753d5b5,  // 59
        0x9f4f2726179a2245, 0x1d762422c946591,  // 60
        0xc722f0ef9d80aad6, 0x424d3ad2b7b97ef6, // 61
        0xf8ebad2b84e0d58b, 0xd2e0898765a7deb3, // 62
        0x9b934c3b330c8577, 0x63cc55f49f88eb30, // 63
        0xc2781f49ffcfa6d5, 0x3cbf6b71c76b25fc, // 64
        0xf316271c7fc3908a, 0x8bef464e3945ef7b, // 65
        0x97edd871cfda3a56, 0x97758bf0e3cbb5ad, // 66
        0xbde94e8e43d0c8ec, 0x3d52eeed1cbea318, // 67
        0xed63a231d4c4fb27, 0x4ca7aaa863ee4bde, // 68
        0x945e455f24fb1cf8, 0x8fe8caa93e74ef6b, // 69
        0xb975d6b6ee39e436, 0xb3e2fd538e122b45, // 70
        0xe7d34c64a9c85d44, 0x60dbbca87196b617, // 71
        0x90e40fbeea1d3a4a, 0xbc8955e946fe31ce, // 72
        0xb51d13aea4a488dd, 0x6babab6398bdbe42, // 73
        0xe264589a4dcdab14, 0xc696963c7eed2dd2, // 74
        0x8d7eb76070a08aec, 0xfc1e1de5cf543ca3, // 75
        0xb0de65388cc8ada8, 0x3b25a55f43294bcc, // 76
        0xdd15fe86affad912, 0x49ef0eb713f39ebf, // 77
        0x8a2dbf142dfcc7ab, 0x6e3569326c784338, // 78
        0xacb92ed9397bf996, 0x49c2c37f07965405, // 79
        0xd7e77a8f87daf7fb, 0xdc33745ec97be907, // 80
        0x86f0ac99b4e8dafd, 0x69a028bb3ded71a4, // 81
        0xa8acd7c0222311bc, 0xc40832ea0d68ce0d, // 82
        0xd2d80db02aabd62b, 0xf50a3fa490c30191, // 83
        0x83c7088e1aab65db, 0x792667c6da79e0fb, // 84
        0xa4b8cab1a1563f52, 0x577001b891185939, // 85
        0xcde6fd5e09abcf26, 0xed4c0226b55e6f87, // 86
        0x80b05e5ac60b6178, 0x544f8158315b05b5, // 87
        0xa0dc75f1778e39d6, 0x696361ae3db1c722, // 88
        0xc913936dd571c84c, 0x3bc3a19cd1e38ea,  // 89
        0xfb5878494ace3a5f, 0x4ab48a04065c724,  // 90
        0x9d174b2dcec0e47b, 0x62eb0d64283f9c77, // 91
        0xc45d1df942711d9a, 0x3ba5d0bd324f8395, // 92
        0xf5746577930d6500, 0xca8f44ec7ee3647a, // 93
        0x9968bf6abbe85f20, 0x7e998b13cf4e1ecc, // 94
        0xbfc2ef456ae276e8, 0x9e3fedd8c321a67f, // 95
        0xefb3ab16c59b14a2, 0xc5cfe94ef3ea101f, // 96
        0x95d04aee3b80ece5, 0xbba1f1d158724a13, // 97
        0xbb445da9ca61281f, 0x2a8a6e45ae8edc98, // 98
        0xea1575143cf97226, 0xf52d09d71a3293be, // 99
        0x924d692ca61be758, 0x593c2626705f9c57, // 100
        0xb6e0c377cfa2e12e, 0x6f8b2fb00c77836d, // 101
        0xe498f455c38b997a, 0xb6dfb9c0f956448,  // 102
        0x8edf98b59a373fec, 0x4724bd4189bd5ead, // 103
        0xb2977ee300c50fe7, 0x58edec91ec2cb658, // 104
        0xdf3d5e9bc0f653e1, 0x2f2967b66737e3ee, // 105
        0x8b865b215899f46c, 0xbd79e0d20082ee75, // 106
        0xae67f1e9aec07187, 0xecd8590680a3aa12, // 107
        0xda01ee641a708de9, 0xe80e6f4820cc9496, // 108
        0x884134fe908658b2, 0x3109058d147fdcde, // 109
        0xaa51823e34a7eede, 0xbd4b46f0599fd416, // 110
        0xd4e5e2cdc1d1ea96, 0x6c9e18ac7007c91b, // 111
        0x850fadc09923329e, 0x3e2cf6bc604ddb1,  // 112
        0xa6539930bf6bff45, 0x84db8346b786151d, // 113
        0xcfe87f7cef46ff16, 0xe612641865679a64, // 114
        0x81f14fae158c5f6e, 0x4fcb7e8f3f60c07f, // 115
        0xa26da3999aef7749, 0xe3be5e330f38f09e, // 116
        0xcb090c8001ab551c, 0x5cadf5bfd3072cc6, // 117
        0xfdcb4fa002162a63, 0x73d9732fc7c8f7f7, // 118
        0x9e9f11c4014dda7e, 0x2867e7fddcdd9afb, // 119
        0xc646d63501a1511d, 0xb281e1fd541501b9, // 120
        0xf7d88bc24209a565, 0x1f225a7ca91a4227, // 121
        0x9ae757596946075f, 0x3375788de9b06959, // 122
        0xc1a12d2fc3978937, 0x52d6b1641c83af,   // 123
        0xf209787bb47d6b84, 0xc0678c5dbd23a49b, // 124
        0x9745eb4d50ce6332, 0xf840b7ba963646e1, // 125
        0xbd176620a501fbff, 0xb650e5a93bc3d899, // 126
        0xec5d3fa8ce427aff, 0xa3e51f138ab4cebf, // 127
        0x93ba47c980e98cdf, 0xc66f336c36b10138, // 128
        0xb8a8d9bbe123f017, 0xb80b0047445d4185, // 129
        0xe6d3102ad96cec1d, 0xa60dc059157491e6, // 130
        0x9043ea1ac7e41392, 0x87c89837ad68db30, // 131
        0xb454e4a179dd1877, 0x29babe4598c311fc, // 132
        0xe16a1dc9d8545e94, 0xf4296dd6fef3d67b, // 133
        0x8ce2529e2734bb1d, 0x1899e4a65f58660d, // 134
        0xb01ae745b101e9e4, 0x5ec05dcff72e7f90, // 135
        0xdc21a1171d42645d, 0x76707543f4fa1f74, // 136
        0x899504ae72497eba, 0x6a06494a791c53a9, // 137
        0xabfa45da0edbde69, 0x487db9d17636893,  // 138
        0xd6f8d7509292d603, 0x45a9d2845d3c42b7, // 139
        0x865b86925b9bc5c2, 0xb8a2392ba45a9b3,  // 140
        0xa7f26836f282b732, 0x8e6cac7768d7141f, // 141
        0xd1ef0244af2364ff, 0x3207d795430cd927, // 142
        0x8335616aed761f1f, 0x7f44e6bd49e807b9, // 143
        0xa402b9c5a8d3a6e7, 0x5f16206c9c6209a7, // 144
        0xcd036837130890a1, 0x36dba887c37a8c10, // 145
        0x802221226be55a64, 0xc2494954da2c978a, // 146
        0xa02aa96b06deb0fd, 0xf2db9baa10b7bd6d, // 147
        0xc83553c5c8965d3d, 0x6f92829494e5acc8, // 148
        0xfa42a8b73abbf48c, 0xcb772339ba1f17fa, // 149
        0x9c69a97284b578d7, 0xff2a760414536efc, // 150
        0xc38413cf25e2d70d, 0xfef5138519684abb, // 151
        0xf46518c2ef5b8cd1, 0x7eb258665fc25d6a, // 152
        0x98bf2f79d5993802, 0xef2f773ffbd97a62, // 153
        0xbeeefb584aff8603, 0xaafb550ffacfd8fb, // 154
        0xeeaaba2e5dbf6784, 0x95ba2a53f983cf39, // 155
        0x952ab45cfa97a0b2, 0xdd945a747bf26184, // 156
        0xba756174393d88df, 0x94f971119aeef9e5, // 157
        0xe912b9d1478ceb17, 0x7a37cd5601aab85e, // 158
        0x91abb422ccb812ee, 0xac62e055c10ab33b, // 159
        0xb616a12b7fe617aa, 0x577b986b314d600a, // 160
        0xe39c49765fdf9d94, 0xed5a7e85fda0b80c, // 161
        0x8e41ade9fbebc27d, 0x14588f13be847308, // 162
        0xb1d219647ae6b31c, 0x596eb2d8ae258fc9, // 163
        0xde469fbd99a05fe3, 0x6fca5f8ed9aef3bc, // 164
        0x8aec23d680043bee, 0x25de7bb9480d5855, // 165
        0xada72ccc20054ae9, 0xaf561aa79a10ae6b, // 166
        0xd910f7ff28069da4, 0x1b2ba1518094da05, // 167
        0x87aa9aff79042286, 0x90fb44d2f05d0843, // 168
        0xa99541bf57452b28, 0x353a1607ac744a54, // 169
        0xd3fa922f2d1675f2, 0x42889b8997915ce9, // 170
        0x847c9b5d7c2e09b7, 0x69956135febada12, // 171
        0xa59bc234db398c25, 0x43fab9837e699096, // 172
        0xcf02b2c21207ef2e, 0x94f967e45e03f4bc, // 173
        0x8161afb94b44f57d, 0x1d1be0eebac278f6, // 174
        0xa1ba1ba79e1632dc, 0x6462d92a69731733, // 175
        0xca28a291859bbf93, 0x7d7b8f7503cfdcff, // 176
        0xfcb2cb35e702af78, 0x5cda735244c3d43f, // 177
        0x9defbf01b061adab, 0x3a0888136afa64a8, // 178
        0xc56baec21c7a1916, 0x88aaa1845b8fdd1,  // 179
        0xf6c69a72a3989f5b, 0x8aad549e57273d46, // 180
        0x9a3c2087a63f6399, 0x36ac54e2f678864c, // 181
        0xc0cb28a98fcf3c7f, 0x84576a1bb416a7de, // 182
        0xf0fdf2d3f3c30b9f, 0x656d44a2a11c51d6, // 183
        0x969eb7c47859e743, 0x9f644ae5a4b1b326, // 184
        0xbc4665b596706114, 0x873d5d9f0dde1fef, // 185
        0xeb57ff22fc0c7959, 0xa90cb506d155a7eb, // 186
        0x9316ff75dd87cbd8, 0x9a7f12442d588f3,  // 187
        0xb7dcbf5354e9bece, 0xc11ed6d538aeb30,  // 188
        0xe5d3ef282a242e81, 0x8f1668c8a86da5fb, // 189
        0x8fa475791a569d10, 0xf96e017d694487bd, // 190
        0xb38d92d760ec4455, 0x37c981dcc395a9ad, // 191
        0xe070f78d3927556a, 0x85bbe253f47b1418, // 192
        0x8c469ab843b89562, 0x93956d7478ccec8f, // 193
        0xaf58416654a6babb, 0x387ac8d1970027b3, // 194
        0xdb2e51bfe9d0696a, 0x6997b05fcc0319f,  // 195
        0x88fcf317f22241e2, 0x441fece3bdf81f04, // 196
        0xab3c2fddeeaad25a, 0xd527e81cad7626c4, // 197
        0xd60b3bd56a5586f1, 0x8a71e223d8d3b075, // 198
        0x85c7056562757456, 0xf6872d5667844e4a, // 199
        0xa738c6bebb12d16c, 0xb428f8ac016561dc, // 200
        0xd106f86e69d785c7, 0xe13336d701beba53, // 201
        0x82a45b450226b39c, 0xecc0024661173474, // 202
        0xa34d721642b06084, 0x27f002d7f95d0191, // 203
        0xcc20ce9bd35c78a5, 0x31ec038df7b441f5, // 204
        0xff290242c83396ce, 0x7e67047175a15272, // 205
        0x9f79a169bd203e41, 0xf0062c6e984d387,  // 206
        0xc75809c42c684dd1, 0x52c07b78a3e60869, // 207
        0xf92e0c3537826145, 0xa7709a56ccdf8a83, // 208
        0x9bbcc7a142b17ccb, 0x88a66076400bb692, // 209
        0xc2abf989935ddbfe, 0x6acff893d00ea436, // 210
        0xf356f7ebf83552fe, 0x583f6b8c4124d44,  // 211
        0x98165af37b2153de, 0xc3727a337a8b704b, // 212
        0xbe1bf1b059e9a8d6, 0x744f18c0592e4c5d, // 213
        0xeda2ee1c7064130c, 0x1162def06f79df74, // 214
        0x9485d4d1c63e8be7, 0x8addcb5645ac2ba9, // 215
        0xb9a74a0637ce2ee1, 0x6d953e2bd7173693, // 216
        0xe8111c87c5c1ba99, 0xc8fa8db6ccdd0438, // 217
        0x910ab1d4db9914a0, 0x1d9c9892400a22a3, // 218
        0xb54d5e4a127f59c8, 0x2503beb6d00cab4c, // 219
        0xe2a0b5dc971f303a, 0x2e44ae64840fd61e, // 220
        0x8da471a9de737e24, 0x5ceaecfed289e5d3, // 221
        0xb10d8e1456105dad, 0x7425a83e872c5f48, // 222
        0xdd50f1996b947518, 0xd12f124e28f7771a, // 223
        0x8a5296ffe33cc92f, 0x82bd6b70d99aaa70, // 224
        0xace73cbfdc0bfb7b, 0x636cc64d1001550c, // 225
        0xd8210befd30efa5a, 0x3c47f7e05401aa4f, // 226
        0x8714a775e3e95c78, 0x65acfaec34810a72, // 227
        0xa8d9d1535ce3b396, 0x7f1839a741a14d0e, // 228
        0xd31045a8341ca07c, 0x1ede48111209a051, // 229
        0x83ea2b892091e44d, 0x934aed0aab460433, // 230
        0xa4e4b66b68b65d60, 0xf81da84d56178540, // 231
        0xce1de40642e3f4b9, 0x36251260ab9d668f, // 232
        0x80d2ae83e9ce78f3, 0xc1d72b7c6b42601a, // 233
        0xa1075a24e4421730, 0xb24cf65b8612f820, // 234
        0xc94930ae1d529cfc, 0xdee033f26797b628, // 235
        0xfb9b7cd9a4a7443c, 0x169840ef017da3b2, // 236
        0x9d412e0806e88aa5, 0x8e1f289560ee864f, // 237
        0xc491798a08a2ad4e, 0xf1a6f2bab92a27e3, // 238
        0xf5b5d7ec8acb58a2, 0xae10af696774b1dc, // 239
        0x9991a6f3d6bf1765, 0xacca6da1e0a8ef2a, // 240
        0xbff610b0cc6edd3f, 0x17fd090a58d32af4, // 241
        0xeff394dcff8a948e, 0xddfc4b4cef07f5b1, // 242
        0x95f83d0a1fb69cd9, 0x4abdaf101564f98f, // 243
        0xbb764c4ca7a4440f, 0x9d6d1ad41abe37f2, // 244
        0xea53df5fd18d5513, 0x84c86189216dc5ee, // 245
        0x92746b9be2f8552c, 0x32fd3cf5b4e49bb5, // 246
        0xb7118682dbb66a77, 0x3fbc8c33221dc2a2, // 247
        0xe4d5e82392a40515, 0xfabaf3feaa5334b,  // 248
        0x8f05b1163ba6832d, 0x29cb4d87f2a7400f, // 249
        0xb2c71d5bca9023f8, 0x743e20e9ef511013, // 250
        0xdf78e4b2bd342cf6, 0x914da9246b255417, // 251
        0x8bab8eefb6409c1a, 0x1ad089b6c2f7548f, // 252
        0xae9672aba3d0c320, 0xa184ac2473b529b2, // 253
        0xda3c0f568cc4f3e8, 0xc9e5d72d90a2741f, // 254
        0x8865899617fb1871, 0x7e2fa67c7a658893, // 255
        0xaa7eebfb9df9de8d, 0xddbb901b98feeab8, // 256
        0xd51ea6fa85785631, 0x552a74227f3ea566, // 257
        0x8533285c936b35de, 0xd53a88958f872760, // 258
        0xa67ff273b8460356, 0x8a892abaf368f138, // 259
        0xd01fef10a657842c, 0x2d2b7569b0432d86, // 260
        0x8213f56a67f6b29b, 0x9c3b29620e29fc74, // 261
        0xa298f2c501f45f42, 0x8349f3ba91b47b90, // 262
        0xcb3f2f7642717713, 0x241c70a936219a74, // 263
        0xfe0efb53d30dd4d7, 0xed238cd383aa0111, // 264
        0x9ec95d1463e8a506, 0xf4363804324a40ab, // 265
        0xc67bb4597ce2ce48, 0xb143c6053edcd0d6, // 266
        0xf81aa16fdc1b81da, 0xdd94b7868e94050b, // 267
        0x9b10a4e5e9913128, 0xca7cf2b4191c8327, // 268
        0xc1d4ce1f63f57d72, 0xfd1c2f611f63a3f1, // 269
        0xf24a01a73cf2dccf, 0xbc633b39673c8ced, // 270
        0x976e41088617ca01, 0xd5be0503e085d814, // 271
        0xbd49d14aa79dbc82, 0x4b2d8644d8a74e19, // 272
        0xec9c459d51852ba2, 0xddf8e7d60ed1219f, // 273
        0x93e1ab8252f33b45, 0xcabb90e5c942b504, // 274
        0xb8da1662e7b00a17, 0x3d6a751f3b936244, // 275
        0xe7109bfba19c0c9d, 0xcc512670a783ad5,  // 276
        0x906a617d450187e2, 0x27fb2b80668b24c6, // 277
        0xb484f9dc9641e9da, 0xb1f9f660802dedf7, // 278
        0xe1a63853bbd26451, 0x5e7873f8a0396974, // 279
        0x8d07e33455637eb2, 0xdb0b487b6423e1e9, // 280
        0xb049dc016abc5e5f, 0x91ce1a9a3d2cda63, // 281
        0xdc5c5301c56b75f7, 0x7641a140cc7810fc, // 282
        0x89b9b3e11b6329ba, 0xa9e904c87fcb0a9e, // 283
        0xac2820d9623bf429, 0x546345fa9fbdcd45, // 284
        0xd732290fbacaf133, 0xa97c177947ad4096, // 285
        0x867f59a9d4bed6c0, 0x49ed8eabcccc485e, // 286
        0xa81f301449ee8c70, 0x5c68f256bfff5a75, // 287
        0xd226fc195c6a2f8c, 0x73832eec6fff3112, // 288
        0x83585d8fd9c25db7, 0xc831fd53c5ff7eac, // 289
        0xa42e74f3d032f525, 0xba3e7ca8b77f5e56, // 290
        0xcd3a1230c43fb26f, 0x28ce1bd2e55f35ec, // 291
        0x80444b5e7aa7cf85, 0x7980d163cf5b81b4, // 292
        0xa0555e361951c366, 0xd7e105bcc3326220, // 293
        0xc86ab5c39fa63440, 0x8dd9472bf3fefaa8, // 294
        0xfa856334878fc150, 0xb14f98f6f0feb952, // 295
        0x9c935e00d4b9d8d2, 0x6ed1bf9a569f33d4, // 296
        0xc3b8358109e84f07, 0xa862f80ec4700c9,  // 297
        0xf4a642e14c6262c8, 0xcd27bb612758c0fb, // 298
        0x98e7e9cccfbd7dbd, 0x8038d51cb897789d, // 299
        0xbf21e44003acdd2c, 0xe0470a63e6bd56c4, // 300
        0xeeea5d5004981478, 0x1858ccfce06cac75, // 301
        0x95527a5202df0ccb, 0xf37801e0c43ebc9,  // 302
        0xbaa718e68396cffd, 0xd30560258f54e6bb, // 303
        0xe950df20247c83fd, 0x47c6b82ef32a206a, // 304
        0x91d28b7416cdd27e, 0x4cdc331d57fa5442, // 305
        0xb6472e511c81471d, 0xe0133fe4adf8e953, // 306
        0xe3d8f9e563a198e5, 0x58180fddd97723a7, // 307
        0x8e679c2f5e44ff8f, 0x570f09eaa7ea7649, // 308
        0xb201833b35d63f73, 0x2cd2cc6551e513db, // 309
        0xde81e40a034bcf4f, 0xf8077f7ea65e58d2, // 310
        0x8b112e86420f6191, 0xfb04afaf27faf783, // 311
        0xadd57a27d29339f6, 0x79c5db9af1f9b564, // 312
        0xd94ad8b1c7380874, 0x18375281ae7822bd, // 313
        0x87cec76f1c830548, 0x8f2293910d0b15b6, // 314
        0xa9c2794ae3a3c69a, 0xb2eb3875504ddb23, // 315
        0xd433179d9c8cb841, 0x5fa60692a46151ec, // 316
        0x849feec281d7f328, 0xdbc7c41ba6bcd334, // 317
        0xa5c7ea73224deff3, 0x12b9b522906c0801, // 318
        0xcf39e50feae16bef, 0xd768226b34870a01, // 319
        0x81842f29f2cce375, 0xe6a1158300d46641, // 320
        0xa1e53af46f801c53, 0x60495ae3c1097fd1, // 321
        0xca5e89b18b602368, 0x385bb19cb14bdfc5, // 322
        0xfcf62c1dee382c42, 0x46729e03dd9ed7b6, // 323
    };
    static const u64 *pow10_ptr = g + 293 * 2; 
#ifdef __amd64__
        if (regular) [[likely]] // branch
            k = (q * 315653) >> 20;
        else
            k = (q * 315653 - 131237) >> 20;
#else
        // use this branchless code for apple M1, better performance
        // when ieee_exponent == 1 or 0 ; k=-324
        // so we can use (ieee_exponent - 1075) to replace q
        k = ((ieee_exponent - 1075) * 315653 - (regular ? 0 : 131237 ))>>20;
#endif
        int h = q + (((-1 - k) * 217707) >> 16);
        u64 *p10 = (u64 *)&pow10_ptr[(-1 - k) * 2];
        u128 cb = c << (h + 1 + offset);
        u128 hi128 = (cb * p10[0] + ((cb * p10[1]) >> 64)); // p10[0] : high 64bit ; p10[1] : low 64bit
        u64 dot_one = hi128 >> offset;   // == floor(2**64*n)
        u64 half_ulp = (p10[0] >> (-h)) + ((c + 1) & 1) ;   // -h ---> range [1,4]  ; 2**(q-1) * 10^(-k-1)
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
        dec_sig_len_ofs = ( ( (2+16+16)*256 + 2+16 - tz*256 + D17 ) >> (up_down ? 8 : 0)) & 0xff;
#else
        dec_sig_len_ofs = ( ( (2+16)*256 + 2+16 - tz*256 + D17 ) >> (up_down ? 8 : 0)) & 0xff;
#endif

#else

#if HAS_SSE2 // when use sse2,the return value equal to (tail zero number + 16);
        dec_sig_len_ofs = up_down  ?  2+16+16 - tz : 2+16 + D17;
#else
        dec_sig_len_ofs = up_down  ?  2+16 - tz : 2+16 + D17;
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
        if(!regular)[[unlikely]]
            one += (bitarray_irregular[ieee_exponent/64]>>(ieee_exponent%64)) & 1;
#endif
        
        // when -3<=e10 && e10 <= 15 ; we use %lf format print float number
        const int e10_DN = -3;//do not change this value
        const int e10_UP = 15;//do not change this value
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
        // u64 first_sig_pos = (e10_DN<=e10 && e10<=-1) ? 1 - e10 : 0 ;
        // u64 dot_pos = ( 0 <= e10 && e10<= e10_UP ) ? 1 + e10 : 1 ;
        // u64 move_pos = dot_pos + (!(e10_DN<=e10 && e10<=-1) );
        // u64 exp_pos = ((e10_DN <= e10 && e10 <= -1) ? dec_sig_len - 1 : (
        //     (0<=e10 && e10<= e10_UP) ? (e10+2 > dec_sig_len ? e10+2: dec_sig_len) : (
        //         dec_sig_len - (dec_sig_len == 1)
        //     )
        // ) ) + 1;
        u64 first_sig_pos = e10_variable_data[e10_data_ofs][0];
        u64 dot_pos = e10_variable_data[e10_data_ofs][1];
        u64 move_pos = e10_variable_data[e10_data_ofs][2];
        u64 exp_pos = e10_variable_data[e10_data_ofs][dec_sig_len_ofs];
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
        memcpy(buf + 15 + D17, &one, 8);
        //byte_move_16(&buf[move_pos],&buf[dot_pos]);// dot_pos+first_sig_pos+sign max = 16+1 = 17; require 17+16=33 byte buffer
        byte_move_16(buf + move_pos , buf + dot_pos );
        buf_origin[dot_pos] = '.';
        static const u64 *exp_ptr = (u64*)&exp_result_precalc[324];
        
//process the some special case : subnormal number 
        if(m < (u64)1e14 ) [[unlikely]]
        {
            // some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
            u64 lz = 0;
            while(buf[2+lz] == '0')lz++;
            lz += 2;
            e10 -= lz - 1;
            buf[0] = buf[lz];
            //byte_move_16(&buf[2], &buf[lz+1]);
            byte_move_16(buf+2, buf+lz+1);
            exp_pos = exp_pos - lz + 1 - (exp_pos - lz == 1 );
#if is_intel_compiler
            buf += exp_pos;
            u64 exp_result = exp_ptr[e10];
            //*(u64*)buf = exp_result;
            memcpy(buf, &exp_result, 8);
            return buf + 5;// return the end of buffer with '\0';
#endif
        }
// write exponent , set 0 to use lookup table to get exp_result , set 1 to use next code to calc exp_result 
#if 0
        u64 neg = e10 < 0;
        u64 e10_abs = neg ? -e10 : e10;
        u64 e = neg ? ('e' + '-' * 256) : ('e' + '+' * 256) ;
        u64 a = (e10_abs * 656) >> 16; /* e10_abs / 100 */
        u64 bc = e10_abs - a * 100;    /* e10_abs % 100 */
        u64 bc_ASCII = bc * 256u - (256 * 10 - 1) * ((bc * 103u) >> 10) + (u64)('0' + '0' * 256 + (4ull << 40) + (4ull << 32)); // 12 => "12"
        u64 exp_result = e | ( ( (e10_abs > 99u) ? a | ('0' | (1ull << 40)) | (bc_ASCII << 8) : bc_ASCII) << 16);
        exp_result = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : exp_result;// e10_DN<=e10 && e10<=e10_UP : no need to print exponent
#else   // use lookup table to get exp_result maybe faster than above code , but need 5064byte lookup table ;
        u64 exp_result = exp_ptr[e10];
#endif
        buf += exp_pos;
        //*(u64*)buf = exp_result;
        memcpy(buf, &exp_result, 8);
        //u64 exp_len = (e10_DN<=e10 && e10<= e10_UP ) ? 0 : (4 | (e10_abs > 99u) ) ;// "e+20" "e+308" : 4 or 5
        u64 exp_len = exp_result >> 56; // 0 or 4 or 5 ; equal to above code
        return buf + exp_len;// return the end of buffer with '\0';
}
#if 1
char* xjb32(float v,char* buf)
{
    // all lut size = 336+144+616 = 1096byte
    // recommend buf size >= 24byte;

    // benchmark result on AMD R7-7840H
    // clang  : 33-34 cycle
    // icpx   : 33-34 cycle
    // g++    : 35-36 cycle 

    u32 vi;
    memcpy(&vi, &v, 4);

    buf[0]='-';
    u32 sign = vi>>31;
    buf+=sign;

    u64 dec,m;
    int e10;
    u32 tz;// tail zero
    //u32 dec_sig_len;// decimal length
    u32 dec_sig_len_ofs;// = dec_sig_len + 2
    u32 D9;// 1:9 digits 0:8 digits
    u32 sig = vi & ((1<<23) - 1);
    //u32 sig = (vi << 9 ) >> 9;
    u32 exp = (vi << 1 ) >> 24;

    if( (vi << 1) == 0 )[[unlikely]]
    {
        memcpy(buf, "0.0" , 4);//end with '\0'
        return buf + 3;
    }
    if(exp == 255)[[unlikely]]
    {
        memcpy(buf, sig ? "NaN" : "Inf", 4);//end with '\0'
        return buf + 3;
    }
    //*(u64*)buf = *(u64*)"0.00000";
    memcpy(buf ,"0.000000",8);

    // size = 77*8 = 616 byte
    static const u64 pow10_table[(44 - (-32) + 1)] = {
        0xcfb11ead453994bb, // -32
        0x81ceb32c4b43fcf5, // -31
        0xa2425ff75e14fc32, // -30
        0xcad2f7f5359a3b3f, // -29
        0xfd87b5f28300ca0e, // -28
        0x9e74d1b791e07e49, // -27
        0xc612062576589ddb, // -26
        0xf79687aed3eec552, // -25
        0x9abe14cd44753b53, // -24
        0xc16d9a0095928a28, // -23
        0xf1c90080baf72cb2, // -22
        0x971da05074da7bef, // -21
        0xbce5086492111aeb, // -20
        0xec1e4a7db69561a6, // -19
        0x9392ee8e921d5d08, // -18
        0xb877aa3236a4b44a, // -17
        0xe69594bec44de15c, // -16
        0x901d7cf73ab0acda, // -15
        0xb424dc35095cd810, // -14
        0xe12e13424bb40e14, // -13
        0x8cbccc096f5088cc, // -12
        0xafebff0bcb24aaff, // -11
        0xdbe6fecebdedd5bf, // -10
        0x89705f4136b4a598, // -9
        0xabcc77118461cefd, // -8
        0xd6bf94d5e57a42bd, // -7
        0x8637bd05af6c69b6, // -6
        0xa7c5ac471b478424, // -5
        0xd1b71758e219652c, // -4
        0x83126e978d4fdf3c, // -3
        0xa3d70a3d70a3d70b, // -2
        0xcccccccccccccccd, // -1
        0x8000000000000000, // 0
        0xa000000000000000, // 1
        0xc800000000000000, // 2
        0xfa00000000000000, // 3
        0x9c40000000000000, // 4
        0xc350000000000000, // 5
        0xf424000000000000, // 6
        0x9896800000000000, // 7
        0xbebc200000000000, // 8
        0xee6b280000000000, // 9
        0x9502f90000000000, // 10
        0xba43b74000000000, // 11
        0xe8d4a51000000000, // 12
        0x9184e72a00000000, // 13
        0xb5e620f480000000, // 14
        0xe35fa931a0000000, // 15
        0x8e1bc9bf04000000, // 16
        0xb1a2bc2ec5000000, // 17
        0xde0b6b3a76400000, // 18
        0x8ac7230489e80000, // 19
        0xad78ebc5ac620000, // 20
        0xd8d726b7177a8000, // 21
        0x878678326eac9000, // 22
        0xa968163f0a57b400, // 23
        0xd3c21bcecceda100, // 24
        0x84595161401484a0, // 25
        0xa56fa5b99019a5c8, // 26
        0xcecb8f27f4200f3a, // 27
        0x813f3978f8940985, // 28
        0xa18f07d736b90be6, // 29
        0xc9f2c9cd04674edf, // 30
        0xfc6f7c4045812297, // 31
        0x9dc5ada82b70b59e, // 32
        0xc5371912364ce306, // 33
        0xf684df56c3e01bc7, // 34
        0x9a130b963a6c115d, // 35
        0xc097ce7bc90715b4, // 36
        0xf0bdc21abb48db21, // 37
        0x96769950b50d88f5, // 38
        0xbc143fa4e250eb32, // 39
        0xeb194f8e1ae525fe, // 40
        0x92efd1b8d0cf37bf, // 41
        0xb7abc627050305ae, // 42
        0xe596b7b0c643c71a, // 43
        0x8f7e32ce7bea5c70, // 44
    };
    int exp_bin, k;
    u64 sig_bin, regular = sig > 0;
    u64 irregular = (sig == 0);
    if (exp > 0) [[likely]] // branch
    {
        exp_bin = exp - 150; //-127-23
        sig_bin = sig | (1u << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = sig;
    }
#ifdef __amd64__
    if (regular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
#else
    //k = (exp_bin * 315653 - (irregular ? 131237 : 0 ))>>20;
    k = (exp_bin * 315653 - (regular ? 0 : 131237 ))>>20;
#endif
    int h = exp_bin + (((-1 - k) * 217707) >> 16); // [-4,-1]
    static const u64 *pow10 = &pow10_table[32];
    u64 pow10_hi = pow10[(-1 - k)];
    //u64 pow10_hi = pow10_table[(-1-k)+32];
    u64 even = ((sig_bin + 1) & 1);
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64; // one mulxq instruction on x86 , need BMI2
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = (pow10_hi >> ((64 - BIT) - h)) + even;
    //u64 up = (half_ulp  > (((u64)1 << BIT) - 1) - dot_one_36bit);
    u64 up = (half_ulp + dot_one_36bit) >> BIT;
    u64 down =  ((half_ulp >> (1 - regular)) > dot_one_36bit);
    u64 up_down = up + down;
    m = (sig_hi >> BIT) + up;
    D9 = m >= (u32)1e7;
    u64 mr = D9 ? m : m * 10;//remove left zero
    u64 ASCII_8;
    tz = encode_8digit(m,&ASCII_8);
    //dec_sig_len = up_down ? 8 - tz : 8 + D9;
#if yy_is_real_gcc 
    // use this code to prevent gcc compiler generate branch instructions
    //dec_sig_len_ofs = ( ( ((2+8 - tz)*256) + 2+8 + D9 ) >> (up_down ? 8 : 0)) & 0xff;
    dec_sig_len_ofs = ( ( (2+8)*256 +2+8 - tz*256  + D9 ) >> (up_down ? 8 : 0)) & 0xff;
#else
    // icpx clang use this code to generate cmov instructions
    dec_sig_len_ofs = up_down ? 2+8 - tz : 2+8 + D9;// when mr = 0, up_down = 0, so can avoid use tz
#endif
    k += 7 + D9;
    e10 = k;// euqal to e10 = k+7+D9

    u64 offset_num  = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = ((dot_one_36bit * 20 + offset_num) >> (BIT + 1))  + (u64)('0' + '0' * 256);
    if(!regular)[[unlikely]]{ // branch
        if( (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150) )
            one+=1;
    }

    const int e10_DN = -3;
    const int e10_UP = 7;
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
    // u64 first_sig_pos = (e10_DN<=e10 && e10<=-1) ? 1 - e10 : 0 ;
    // u64 dot_pos = ( 0 <= e10 && e10<= e10_UP ) ? 1 + e10 : 1 ;
    // u64 move_pos = dot_pos + (!(e10_DN<=e10 && e10<=-1) );
    // u64 exp_pos = ((e10_DN <= e10 && e10 <= -1) ? dec_sig_len - 1 : (
    //         (0<=e10 && e10<= e10_UP) ? (e10+2 > dec_sig_len ? e10+2: dec_sig_len ) : (
    //             dec_sig_len - (dec_sig_len == 1)
    //         )
    //     ) ) + 1;
    char* buf_origin = (char*)buf;
    buf += first_sig_pos;
    //byte_move_8(buf,&ASCII_8);//7 or 8 byte
    memcpy(buf, &ASCII_8, 8);
    //memcpy(&buf[7 + D9], &one, 8);
    memcpy(buf + 7 + D9, &one, 8);
    //byte_move_8(&buf[move_pos],&buf[dot_pos]);
    byte_move_8(buf + move_pos , buf + dot_pos);
    buf_origin[dot_pos] = '.';
    
    // -45 -> 38 ; size = 84*4 =  336 byte
    static const u32 exp_result_precalc[45 + 38 + 1]={
0x35342d65, // e10 = -45
0x34342d65, // e10 = -44
0x33342d65, // e10 = -43
0x32342d65, // e10 = -42
0x31342d65, // e10 = -41
0x30342d65, // e10 = -40
0x39332d65, // e10 = -39
0x38332d65, // e10 = -38
0x37332d65, // e10 = -37
0x36332d65, // e10 = -36
0x35332d65, // e10 = -35
0x34332d65, // e10 = -34
0x33332d65, // e10 = -33
0x32332d65, // e10 = -32
0x31332d65, // e10 = -31
0x30332d65, // e10 = -30
0x39322d65, // e10 = -29
0x38322d65, // e10 = -28
0x37322d65, // e10 = -27
0x36322d65, // e10 = -26
0x35322d65, // e10 = -25
0x34322d65, // e10 = -24
0x33322d65, // e10 = -23
0x32322d65, // e10 = -22
0x31322d65, // e10 = -21
0x30322d65, // e10 = -20
0x39312d65, // e10 = -19
0x38312d65, // e10 = -18
0x37312d65, // e10 = -17
0x36312d65, // e10 = -16
0x35312d65, // e10 = -15
0x34312d65, // e10 = -14
0x33312d65, // e10 = -13
0x32312d65, // e10 = -12
0x31312d65, // e10 = -11
0x30312d65, // e10 = -10
0x39302d65, // e10 = -9
0x38302d65, // e10 = -8
0x37302d65, // e10 = -7
0x36302d65, // e10 = -6
0x35302d65, // e10 = -5
0x34302d65, // e10 = -4
0x0, // e10 = -3
0x0, // e10 = -2
0x0, // e10 = -1
0x0, // e10 = 0
0x0, // e10 = 1
0x0, // e10 = 2
0x0, // e10 = 3
0x0, // e10 = 4
0x0, // e10 = 5
0x0, // e10 = 6
0x0, // e10 = 7
0x38302b65, // e10 = 8
0x39302b65, // e10 = 9
0x30312b65, // e10 = 10
0x31312b65, // e10 = 11
0x32312b65, // e10 = 12
0x33312b65, // e10 = 13
0x34312b65, // e10 = 14
0x35312b65, // e10 = 15
0x36312b65, // e10 = 16
0x37312b65, // e10 = 17
0x38312b65, // e10 = 18
0x39312b65, // e10 = 19
0x30322b65, // e10 = 20
0x31322b65, // e10 = 21
0x32322b65, // e10 = 22
0x33322b65, // e10 = 23
0x34322b65, // e10 = 24
0x35322b65, // e10 = 25
0x36322b65, // e10 = 26
0x37322b65, // e10 = 27
0x38322b65, // e10 = 28
0x39322b65, // e10 = 29
0x30332b65, // e10 = 30
0x31332b65, // e10 = 31
0x32332b65, // e10 = 32
0x33332b65, // e10 = 33
0x34332b65, // e10 = 34
0x35332b65, // e10 = 35
0x36332b65, // e10 = 36
0x37332b65, // e10 = 37
0x38332b65, // e10 = 38
};
    static const u32 *exp_ptr = (u32*)&exp_result_precalc[45];
    //u32 exp_result = exp_ptr[e10];
    if(m < (u32)1e6 )[[unlikely]]
    {
        u64 lz = 0;
        while(buf[2+lz] == '0')lz++;
        lz += 2;
        e10 -= lz - 1;
        buf[0] = buf[lz];
        //byte_move_8(&buf[2], &buf[lz+1]);
        byte_move_8(buf + 2, buf+lz+1);
        exp_pos = exp_pos - lz + 1 - (exp_pos - lz == 1 );
        // buf += exp_pos;
        // u32 exp_result = exp_ptr[e10];
        // memcpy(buf, &exp_result, 4);
        // buf += 4;
        // buf[0]='\0';
        // return buf;
    }
//write exponent    
#if 0
        u64 neg = e10 < 0;
        u64 bc = neg ? -e10 : e10;
        u64 e = neg ? ('e' + '-' * 256) : ('e' + '+' * 256) ;
        u64 bc_ASCII = bc * 256u - (256 * 10 - 1) * ((bc * 103u) >> 10) + (u64)('0' + '0' * 256); // 12 => "12"
        u64 exp_result = e | ( bc_ASCII << 16 );
        exp_result = ( e10_DN <= e10 && e10 <= e10_UP ) ? 0 : exp_result;// e10_DN<=e10 && e10<=e10_UP : no need to print exponent
#else
        u64 exp_result = exp_ptr[e10];
#endif
    buf += exp_pos;
    //*(u64*)buf = exp_result;// contain '\0';
    memcpy(buf, &exp_result, 8);
    u32 exp_len = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : 4;//may this is faster than below code?
    //u32 exp_len = (exp_result + ((1u<<28) - 1)) >> 28;//(e10_DN <= e10 && e10 <= e10_UP) ? 0 : 4
    buf += exp_len;
    return buf;
}
#endif
}
#include <stdint.h>

#define USE_NEON_SSE2 1


#if USE_NEON_SSE2


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

#endif // endif USE_NEON_SSE2

#ifndef is_real_gcc
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    !defined(__clang__) && !defined(__llvm__) &&    \
    !defined(__INTEL_COMPILER) && !defined(__ICC)
#define is_real_gcc 1
#endif
#endif

typedef __uint128_t u128;
typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint16_t u16;

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
    _BitScanForward64(&idx, x); // Fallback to the BSF instruction.
    return 63 - idx;
#else
    int n = 64;
    for (; x > 0; x <<= 1)
        --n;
    return n;
#endif
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

static inline uint64_t is_little_endian()
{
    const int n = 1;
    return *(char *)(&n) == 1;
}
static inline uint64_t byteswap64(uint64_t x){
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

static inline uint64_t cmov_branchless(uint64_t up_down, uint64_t a, uint64_t b)
{
    // if up_down == 1 return a
    // if up_down == 0 return b
#if is_real_gcc
    // prevent the gcc compiler generating branch instructions
    return ((~(up_down - 1)) & a) | ((up_down - 1) & b); // only up_down = 1 or 0 can correctly execute.
#else
    return up_down ? a : b;
#endif
}

static inline uint64_t compute_double_dec_sig_len(uint64_t up_down, int tz, uint64_t D17)
{
    return cmov_branchless(up_down, 15 - tz, 15 + D17);
}
static inline uint64_t compute_double_dec_sig_len_sse2(uint64_t up_down, int tz_add_48, uint64_t D17)
{
    return cmov_branchless(up_down, 15 + 48 - tz_add_48, 15 + D17);
}
static inline uint64_t compute_float_dec_sig_len(uint64_t up_down, int tz, uint64_t lz)
{
    return cmov_branchless(up_down, 7 - tz, 8 - lz);
}

static inline shortest_ascii16 to_ascii16(const uint64_t m, const uint64_t up_down, const uint64_t D17)
{
    // m range : [1, 1e16 - 1] ; m = abcdefgh * 10^8 + ijklmnop
    const uint64_t ZERO = 0x3030303030303030ull;
    uint64_t abcdefgh = m / 100000000;
    uint64_t ijklmnop = m - abcdefgh * 100000000;
#if HAS_NEON
    // src from : https://gist.github.com/dougallj/b4f600ab30ef79bb6789bc3f86cd597a#file-convert-neon-cpp-L144-L169
    // bolg : https://dougallj.wordpress.com/2022/04/01/converting-integers-to-fixed-width-strings-faster-with-neon-simd-on-the-apple-m1/
    // author : https://github.com/dougallj
    uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
    int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(0x68db8bb)), 9);
    int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(-10000 + 0x10000));
    int32x4_t extended = vshll_n_u16(tenthousands, 0);
    int32x4_t high_100 = vqdmulhq_s32(extended, vdupq_n_s32(0x147b000));
    int32x4_t hundreds = vmlaq_s32(extended, high_100, vdupq_n_s32(-100 + 0x10000));
    int16x8_t high_10 = vqdmulhq_s16(hundreds, vdupq_n_s16(0xce0));
    int16x8_t BCD_big_endian = vmlaq_s16(hundreds, high_10, vdupq_n_s16(-10 + 0x100));
    int8x16_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    int16x8_t ascii16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    u64 abcdefgh_BCD = vgetq_lane_u64(BCD_little_endian, 0);
    u64 ijklmnop_BCD = vgetq_lane_u64(BCD_little_endian, 1);
    int abcdefgh_tz = u64_lz_bits(abcdefgh_BCD);
    int ijklmnop_tz = u64_lz_bits(ijklmnop_BCD);
    int tz = ijklmnop ? ijklmnop_tz : 64 + abcdefgh_tz;
    tz = tz / 8;
    return {ascii16, compute_double_dec_sig_len(up_down, tz, D17)};
#endif

#if HAS_SSE2
// method 1 : AVX512IFMA, AVX512VBMI
// method 2 : SSE4.1
// method 3 : SSSE3
// method 4 : SSE2

// method 1 : AVX512IFMA, AVX512VBMI
#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
    const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
    const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
    const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
    const __m512i zmmTen = _mm512_set1_epi64(10);
    const __m512i zero = _mm512_set1_epi64(0);
    const __m512i asciizero = _mm512_set1_epi64('0');
    const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
                                                 0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
    const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
    __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
    __m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
    __m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
    __m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
#elif defined(__SSSE3__) //&& (false)
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
#endif
    int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(little_endian_bcd, _mm_setzero_si128()));
    int tz = u64_lz_bits(mask);
    __m128i ascii16 = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
    return {ascii16, compute_double_dec_sig_len_sse2(up_down, tz, D17)};
#endif // endif HAS_SSE2

#if !HAS_NEON_OR_SSE2
    uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
    uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bb) >> 40);
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
    return {abcdefgh_bcd | ZERO, ijklmnop_bcd | ZERO, compute_double_dec_sig_len(up_down, tz, D17)};
#endif

}

struct const_value_float {//size = 40 bytes
    int64_t c1;
    uint64_t div10000;
    uint32_t e7;
    uint32_t e6;
    uint32_t e5;
    uint32_t m;
    int32_t m32_0;
    int32_t m32_1;
};
struct float_table_t {
    uint64_t pow10_float_reverse[(44 - (-32) + 1)];// 77*8 = 616 byte
    uint32_t exp_result_float[45 + 38 + 1];// 84*4 = 336 byte
    unsigned char e10_variable_data[7 - (-3) + 1 + 1][3+9];// 144byte
    unsigned char h37[256];// 256byte
};
static inline shortest_ascii8 to_ascii8(const uint64_t m, const uint64_t up_down,const uint64_t lz, const struct const_value_float *c)
{
    // m range : [0, 1e8 - 1] ; m = abcdefgh
    const uint64_t ZERO = 0x3030303030303030;
#if HAS_NEON

    // u64 abcd_efgh = m + ((1ull << 32) - 10000) * ((m * (u128)1844674407370956) >> 64);
    // int32x2_t tenthousands = vld1_u64((uint64_t const *)&abcd_efgh);
    // int32x2_t hundreds = vmla_s32(tenthousands, vqdmulh_s32(tenthousands, vdup_n_s32(0x147b000)), vdup_n_s32(-100 + 0x10000));
    // int16x4_t BCD_big_endian = vmla_s16(hundreds, vqdmulh_s16(hundreds, vdup_n_s16(0xce0)), vdup_n_s16(-10 + 0x100));
    // u64 abcdefgh_BCD = byteswap64(vget_lane_u64(BCD_big_endian, 0));// big_endian to little_endian , reverse 8 bytes

    u64 abcd_efgh = m + c->m * ((m * (u128)c->div10000) >> 64);
    int32x2_t tenthousands = vld1_u64((uint64_t const *)&abcd_efgh);
    int32x2_t hundreds = vmla_n_s32(tenthousands, vqdmulh_s32(tenthousands, vdup_n_s32(c->m32_0)), c->m32_1);
    int16x4_t BCD_big_endian = vmla_n_s16(hundreds, vqdmulh_s16(hundreds, vdup_n_s16(0xce0)), -10+0x100);
    u64 abcdefgh_BCD = byteswap64(vget_lane_u64(BCD_big_endian, 0));// big_endian to little_endian , reverse 8 bytes
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

//method 1 : compute little endian bcd
    u64 aabb_ccdd_merge = (m << 32) + (1 - (10000ull << 32)) * ((m * (u128)1844674407370956) >> 64);
    //u64 aabb_ccdd_merge = (m << 32) - ((10000ull << 32) - 1) * ((m * 109951163) >> 40);
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

//method 2 : compute big endian bcd , performance worse than method 1
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
//     u64 abcdefgh_BCD = byteswap64(_mm_cvtsi128_si64(big_endian_bcd));   

#endif

#endif // endif HAS_SSE2

#if !HAS_NEON_OR_SSE2
    i64 aabb_ccdd_merge = (m << 32) + (1 - (10000ull << 32)) * ((m * 109951163) >> 40);
    i64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) + (1 - (100ull << 16)) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 abcdefgh_BCD = (aa_bb_cc_dd_merge << 8) + (1 - (10ull << 8)) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
#endif
    //abcdefgh_BCD = D9 ? abcdefgh_BCD : (abcdefgh_BCD >> 8);
    abcdefgh_BCD = abcdefgh_BCD >> (lz * 8);
    int tz = u64_lz_bits(abcdefgh_BCD) / 8;
    abcdefgh_BCD = is_little_endian() ? abcdefgh_BCD : byteswap64(abcdefgh_BCD);
    return {abcdefgh_BCD | ZERO, compute_float_dec_sig_len(up_down, tz, lz)};
}

static inline char *write_1_to_16_digit(u64 x, char *buf)
{
    // require 1 <= x < 1e16
    const u64 ZERO = 0x3030303030303030;
    const u64 mask = (0x7FULL << 32) | 0x7FULL;
    const u64 mask2 = (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
    i64 xi = x;
    if (x < 100'000'000)
    {
        // write 1-8 digit
#if HAS_NEON
        u64 abcd_efgh_u64 = xi + (0x100000000 - 10000) * ((xi * (u128)1844674407370956) >> 64); // xi = abcdefgh
        int32x2_t abcd_efgh = vld1_u64((uint64_t const *)&abcd_efgh_u64);                     // (abcd << 32) + efgh
        int32x2_t ab_cd_ef_gh = vmla_s32(abcd_efgh, vqdmulh_s32(abcd_efgh, vdup_n_s32(0x147b000)), vdup_n_s32(-100 + 0x10000));
        int16x4_t a_b_c_d_e_f_g_h = vmla_s16(ab_cd_ef_gh, vqdmulh_s16(ab_cd_ef_gh, vdup_n_s16(0xce0)), vdup_n_s16(-10 + 0x100));
        u64 bcd_big_endian = vget_lane_u64(a_b_c_d_e_f_g_h, 0);
        u64 lz = u64_lz_bits(bcd_big_endian) / 8; // lz max is 7 , bcd_big_endian = 0 is impossible
        u64 abcdefgh_bcd = is_little_endian() ? byteswap64(bcd_big_endian) : bcd_big_endian ;
        u64 abcdefgh_ascii = abcdefgh_bcd | ZERO;
        abcdefgh_ascii = is_little_endian() ? abcdefgh_ascii >> (8 * lz) : abcdefgh_ascii << (8 * lz);
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
        uint32_t abcdefgh = ((__uint128_t)xi * 0xabcc77118461cefd) >> 90;
        uint32_t ijklmnop = xi - abcdefgh * 100000000;
        uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
        int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(0x68db8bb)), 9);
        int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(-10000 + 0x10000));
        int32x4_t extended = vshll_n_u16(tenthousands, 0);
        int32x4_t hundreds = vmlaq_s32(extended, vqdmulhq_s32(extended, vdupq_n_s32(0x147b000)), vdupq_n_s32(-100 + 0x10000));
        int16x8_t BCD_big_endian = vmlaq_s16(hundreds, vqdmulhq_s16(hundreds, vdupq_n_s16(0xce0)), vdupq_n_s16(-10 + 0x100));
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
        u64 abcdefgh_bcd = vgetq_lane_u64(BCD_little_endian, 0);  // hi
        u64 ijklmnop_bcd = vgetq_lane_u64(BCD_little_endian, 1);  // lo
        //u64 abcdefgh_lz = u64_tz_bits(abcdefgh_bcd) / 8;          // aabbccdd_BCD != 0
        u64 abcdefgh_lz = u64_lz_bits(vgetq_lane_u64(BCD_big_endian, 0)) / 8;
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
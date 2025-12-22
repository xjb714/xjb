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

typedef __uint128_t u128;
typedef uint64_t u64;
typedef int64_t i64;
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
    typedef struct
    { 
        u64 hi;
        u64 lo;
    }byte16_reg;// 2x64 bit register for no neon or sse2
#endif

inline uint64_t bswap64(uint64_t x)
{
#ifdef _MSC_VER
    return _byteswap_uint64(x);
#else
    return __builtin_bswap64(x);
#endif
}

byte16_reg to_bcd16(uint64_t x)
{
    // x range: [0, 1e16 - 1]
#if HAS_NEON
    uint32_t abcdefgh = x / 100000000;
    uint32_t ijklmnop = x - abcdefgh * 100000000;
    uint64x1_t hundredmillions = {abcdefgh | ((uint64_t)ijklmnop << 32)};
    int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(0x68db8bb)), 9);
    int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(-10000 + 0x10000));
    int32x4_t extended = vshll_n_u16(tenthousands, 0);
    int32x4_t high_100 = vqdmulhq_s32(extended, vdupq_n_s32(0x147b000));
    int32x4_t hundreds = vmlaq_s32(extended, high_100, vdupq_n_s32(-100 + 0x10000));
    int16x8_t high_10 = vqdmulhq_s16(hundreds, vdupq_n_s16(0xce0));
    int16x8_t BCD_big_endian = vmlaq_s16(hundreds, high_10, vdupq_n_s16(-10 + 0x100));
    int8x16_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    return BCD_little_endian;
#endif

#if HAS_SSE2

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
    uint64_t n_15_08 = v / 100000000;
    uint64_t n_07_00 = v + n_15_08 * (-100000000);
    const __m512i bcstq_h = _mm512_set1_epi64(n_15_08);
    const __m512i bcstq_l = _mm512_set1_epi64(n_07_00);
    const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
    const __m512i zmmTen = _mm512_set1_epi64(10);
    const __m512i zero = _mm512_set1_epi64(0);
    const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b,
                                                 0x0000000010c6f7a0c, 0x00000000a7c5ac472, 0x000000068db8bac72, 0x0000004189374bc6b,
                                                 0x0000028f5c28f5c29, 0x0000199999999999a);
    const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58,
                                                                    0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
    __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
    __m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
    __m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
    __m128i tmp = _mm512_castsi512_si128(bcd);
    return tmp;
#elif defined(__SSSE3__) //&& (false)
    const uint64_t a = v / 100000000;        // 8-digit number: abcdefgh
    const uint64_t b = v + a * (-100000000); // 8-digit number: ijklmnop
    __m128i x = _mm_set_epi64x(b, a);
    __m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#ifdef __SSE4_1__
    __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
#else
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_epi32(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#endif
    __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
    __m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)); // ssse3
    return little_endian_bcd;

#else // sse2

#define DIV_10 0x199a        // shift = 0 + 16
#define DIV_100 0x147b       // shift = 3 + 16
// magic number for 32-bit division
#define DIV_10000 0xd1b71759 // shift = 13 + 32

    // v is 16-digit number = abcdefghijklmnop

    const __m128i div_10000 = _mm_set1_epi32(DIV_10000); // sse2
    const __m128i mul_10000 = _mm_set1_epi32(10000);
    const int div_10000_shift = 45;

    const __m128i div_100 = _mm_set1_epi16(DIV_100); // sse2
    const __m128i mul_100 = _mm_set1_epi16(100);
    const int div_100_shift = 3;

    const __m128i div_10 = _mm_set1_epi16(DIV_10); // sse2
    const __m128i mul_10 = _mm_set1_epi16(10);

    // can't be easliy done in SSE
    const uint32_t a = x / 100000000;     // 8-digit number: abcdefgh
    const uint32_t b = x - a * 100000000; // 8-digit number: ijklmnop

    //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 ]
    // x            = [       0       |      ijklmnop |       0       |      abcdefgh ]
    __m128i x1 = _mm_set_epi64x(b, a); // sse2

    // x div 10^4   = [       0       |          ijkl |       0       |          abcd ]
    __m128i x_div_10000;
    x_div_10000 = _mm_mul_epu32(x1, div_10000);                 // sse2
    x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift); // sse2

    // x mod 10^4   = [       0       |          mnop |       0       |          efgh ]
    __m128i x_mod_10000;
    x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
    x_mod_10000 = _mm_sub_epi32(x1, x_mod_10000);

    // y            = [          mnop |          ijkl |          efgh |          abcd ]
    __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

#if defined(__SSE4_1__)
    __m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
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
    __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
    return tmp;
#endif

#endif

#if !HAS_NEON_OR_SSE2

    uint64_t abcdefgh = v / 100000000;
    uint64_t ijklmnop = v - abcdefgh * 100000000;
    uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
    uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bb) >> 40);
    uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f); //+ 0x3030303030303030;
    uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f); //+ 0x3030303030303030;
    const int one = 1;
    const int is_little_endian = *(char *)&one == 1;
    uint64_t abcdefgh_bcd = is_little_endian ? bswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
    uint64_t ijklmnop_bcd = is_little_endian ? bswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;
    byte16_reg bcd16;
    bcd16.hi = abcdefgh_bcd;
    bcd16.lo = ijklmnop_bcd;
    return bcd16;

#endif
}

uint64_t to_bcd8(uint64_t x)
{
    // x range: [0, 1e8 - 1]
#if HAS_NEON
    u64 abcd_efgh = x + ((1ull << 32) - 10000) * ((x * (u128)1844674407370956) >> 64); // x = abcdefgh
    int32x2_t tenthousands = vld1_u64((uint64_t const *)&abcd_efgh);                   // (abcd << 32) + efgh
    int32x2_t hundreds = vmla_s32(tenthousands, vqdmulh_s32(tenthousands, vdup_n_s32(0x147b000)), vdup_n_s32(-100 + 0x10000));
    int16x4_t BCD_big_endian = vmla_s16(hundreds, vqdmulh_s16(hundreds, vdup_n_s16(0xce0)), vdup_n_s16(-10 + 0x100));
    int8x8_t BCD_little_endian = vrev64_u8(BCD_big_endian); // big_endian to little_endian , reverse 8 bytes
    u64 aabbccdd_BCD = vget_lane_u64(BCD_little_endian, 0);
    return aabbccdd_BCD;
#endif

#if HAS_SSE2

#if defined(__AVX512F__) && defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
    __m512i bcstq_l = _mm512_set1_epi64(x);
    const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
    const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b,
                                                 0x0000000010c6f7a0c, 0x00000000a7c5ac472, 0x000000068db8bac72, 0x0000004189374bc6b,
                                                 0x0000028f5c28f5c29, 0x0000199999999999a);
    const u64 idx = 0x00 + (0x08 << 8) + (0x10 << 16) + (0x18 << 24) + (0x20ull << 32) + (0x28ull << 40) + (0x30ull << 48) + (0x38ull << 56);
    const __m512i permb_const = _mm512_set1_epi64(idx);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_l = _mm512_srli_epi64(_mm512_add_epi64(lowbits_l, _mm512_slli_epi64(lowbits_l, 2)), 51);
    __m512i bcd = _mm512_permutexvar_epi8(permb_const, highbits_l);
    u64 aabbccdd_BCD = _mm_cvtsi128_si64(_mm512_castsi512_si128(bcd));
    return aabbccdd_BCD;

#elif defined(__SSSE3__) // && (false) // endif avx512ifma avx512vbmi
    u64 abcd_efgh = x + ((1ull << 32) - 10000) * ((x * (u128)1844674407370956) >> 64);
    __m128i y = _mm_set1_epi64x(abcd_efgh);
#ifdef __SSE4_1__
    __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
#else
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_epi32(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#endif
    __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
    const u64 idx = 7 + (6 << 8) + (5 << 16) + (4 << 24) + (3ull << 32) + (2ull << 40) + (1ull << 48) + (0ull << 56);
    __m128i little_endian_bcd = _mm_shuffle_epi8(big_endian_bcd, _mm_set1_epi64x(idx));
    u64 aabbccdd_BCD = _mm_cvtsi128_si64(little_endian_bcd);
    return aabbccdd_BCD;

#else                   // endif ssse3
    // u64 aabb = (x * 109951163) >> 40;
    u64 aabb = (x * (u128)1844674407370956) >> 64;
    u64 aabb_ccdd_merge = (x << 32) - ((10000ull << 32) - 1) * (aabb);
    __m128i y = _mm_set1_epi64x(aabb_ccdd_merge);
#if defined(__SSE4_1__) // __GNUC__ : gcc icpx clang  ; for MSVC how to check cpu support sse4.1
    //_mm_mullo_epi32 need sse4.1
    __m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epi16(y, _mm_set1_epi16(0x147b)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
    __m128i z_div_10 = _mm_mulhi_epi16(z, _mm_set1_epi16(0x199a));
    __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
    u64 aabbccdd_BCD = _mm_cvtsi128_si64(tmp);
    return aabbccdd_BCD;

#endif

#endif

#if !HAS_NEON_OR_SSE2
    i64 aabb_ccdd_merge = (xi << 32) + (1 - (10000ull << 32)) * ((xi * 109951163) >> 40);
    i64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) + (1 - (100ull << 16)) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));
    u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) + (1 - (10ull << 8)) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));
    return aabbccdd_BCD;
#endif

}

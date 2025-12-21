#include <iostream>
#include <cstdint>
#include <cstring>

#ifdef __ARM_NEON

#include <arm_neon.h>

typedef __uint128_t u128;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;

void to_string_neon(uint64_t v, char *out)
{
    // Equivalent to hi = v / 100000000, lo = v % 100000000, but for some reason
    // clang emits a UDIV.
    uint32_t hi = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
    uint32_t lo = v - hi * 100000000;

    uint64x1_t hundredmillions = {hi | ((uint64_t)lo << 32)}; // abcdefgh

    int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(0x68db8bb)), 9);    // abcd
    int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(-10000 + 0x10000)); //(abcd << 32) + efgh

    // Equivalent to `extended = vshll_n_u16(tenthousands, 0)`, but clang can see through
    // that and breaks the subsequent MLA into UADDW + MUL.
    // int32x2_t zero = {0, 0};
    // int32x4_t extended = vzip1q_u16(vcombine_u16(tenthousands, zero), vcombine_u16(zero, zero));
    int32x4_t extended = vshll_n_u16(tenthousands, 0);

    int32x4_t high_100 = vqdmulhq_s32(extended, vdupq_n_s32(0x147b000));
    int32x4_t hundreds = vmlaq_s32(extended, high_100, vdupq_n_s32(-100 + 0x10000));

    int16x8_t high_10 = vqdmulhq_s16(hundreds, vdupq_n_s16(0xce0));
    int16x8_t digits = vmlaq_s16(vaddq_s8(hundreds, vdupq_n_s8('0')), high_10, vdupq_n_s16(-10 + 0x100));

    int8x16_t result = vrev64q_u8(digits);

    memcpy(out, &result, sizeof(result));
}

void to_string_neon_v2(uint64_t v, char *out)
{
    uint64_t abcdefgh = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
    uint64_t ijklmnop = v + abcdefgh * (-100000000);
    uint64_t abcd_efgh = abcdefgh + ((1ull << 32) - 10000) * ((abcdefgh * 109951163) >> 40); // (abcd << 32) + efgh
    uint64_t ijkl_mnop = ijklmnop + ((1ull << 32) - 10000) * ((ijklmnop * 109951163) >> 40); // (ijkl << 32) + mnop
    uint64x2_t merge4 = vcombine_u64(vcreate_u64(abcd_efgh), vcreate_u64(ijkl_mnop));
    uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
    uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
    uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
void to_string_neon_v3(uint64_t v, char *out)
{
    uint64_t abcdefgh = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
    uint64_t ijklmnop = v + abcdefgh * (-100000000);
    uint64_t abcd_efgh = abcdefgh + ((1ull << 32) - 10000) * ((abcdefgh * (__uint128_t)1844674407370956) >> 64); // (abcd << 32) + efgh
    uint64_t ijkl_mnop = ijklmnop + ((1ull << 32) - 10000) * ((ijklmnop * (__uint128_t)1844674407370956) >> 64); // (ijkl << 32) + mnop
    uint64x2_t merge4 = vcombine_u64(vcreate_u64(abcd_efgh), vcreate_u64(ijkl_mnop));
    uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
    uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
    uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
void to_string_neon_v4(uint64_t v, char *out)
{
  uint64_t abcdefgh = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
  uint64_t ijklmnop = v + abcdefgh * (-100000000);
  uint64_t abcd = (abcdefgh * (__uint128_t)1844674407370956) >> 64;
  uint64_t ijkl = (ijklmnop * (__uint128_t)1844674407370956) >> 64;
  // uint64_t abcd = (abcdefgh * 109951163) >> 40;
  // uint64_t ijkl = (ijklmnop * 109951163) >> 40;
  uint64_t abcd_efgh = (abcd << 32) + abcdefgh + abcd * (-10000); // (abcd << 32) + efgh
  uint64_t ijkl_mnop = (ijkl << 32) + ijklmnop + ijkl * (-10000); // (ijkl << 32) + mnop
  uint64x2_t merge4 = vcombine_u64(vcreate_u64(abcd_efgh), vcreate_u64(ijkl_mnop));
  uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
  uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
  uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
  uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
  memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
void to_string_neon_v5(uint64_t v, char *out) // slower than v2 or v3
{
    uint64_t abcdefgh = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
    uint64_t ijklmnop = v + abcdefgh * (-100000000);
    uint64x2_t merge8 = vsetq_lane_s32(ijklmnop, vld1q_u64(&abcdefgh) , 1);
    int32x4_t high_10000 = vshrq_n_u32(vqdmulhq_s32(merge8, vdupq_n_s32(0x68db8bb)), 9); // 0000 , abcd , 0000 , ijkl
    int32x4_t low_10000 = vmlaq_n_u32(merge8, high_10000, -10000);                       // 0000 , efgh , 0000 , mnop
    //int32x4_t merge4 = vorrq_u64(vshlq_n_u64(high_10000, 32), low_10000);              // abcd , efgh , ijkl , mnop
    int32x4_t merge4 = vzip1q_u32(low_10000 , high_10000 );
    uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
    uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
    uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
void to_string_neon_v6(uint64_t v, char *out) // slower than v2 or v3
{
    uint64_t abcdefgh = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
    uint64_t ijklmnop = v + abcdefgh * (-100000000);
    //uint64x2_t merge8 = vsetq_lane_s32(ijklmnop, vld1q_u64(&abcdefgh) , 1);
    //uint64x2_t merge8 = vdupq_n_s64((abcdefgh << 32) | ijklmnop);
    uint64x2_t merge8 = vdupq_n_s64((ijklmnop << 32) | abcdefgh);
    //uint64x2_t merge8 = vcombine_u32(vld1_u32(&abcdefgh), vld1_u32(&ijklmnop));
    int32x4_t high_10000 = vshrq_n_u32(vqdmulhq_s32(merge8, vdupq_n_s32(0x68db8bb)), 9); // 0000 , abcd , 0000 , ijkl
    int32x4_t low_10000 = vmlaq_n_u32(merge8, high_10000, -10000);                       // 0000 , efgh , 0000 , mnop
    //int32x4_t merge4 = vorrq_u64(vshlq_n_u64(high_10000, 32), low_10000);              // abcd , efgh , ijkl , mnop
    int32x4_t merge4 = vzip1q_u32(low_10000 , high_10000 );
    uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
    uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
    uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
    uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
    memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
#endif
void to_string_scalar(uint64_t v, char *out)
{
  uint64_t abcdefgh = v / 100000000;
  uint64_t ijklmnop = v - abcdefgh * 100000000;

  uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
  uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bb) >> 40);

  uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
  uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);

  uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;
  uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;

  const int one = 1;
  const int is_little_endian = *(char *)&one == 1;
  uint64_t h_g_f_e_d_c_b_a = is_little_endian ? __builtin_bswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
  uint64_t p_o_n_m_l_k_j_i = is_little_endian ? __builtin_bswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;

  memcpy(out + 0, &h_g_f_e_d_c_b_a, sizeof(uint64_t));
  memcpy(out + 8, &p_o_n_m_l_k_j_i, sizeof(uint64_t));
}

int main()
{

    uint64_t v[] = {
        12345678 * 100000000ull + 90123456,
        1234567 * 100000000ull + 90123456,
        123456 * 100000000ull + 90123456,
        12345 * 100000000ull + 90123456,
        1234 * 100000000ull + 90123456,
        123 * 100000000ull + 90123456,
        12 * 100000000ull + 90123456,
        1 * 100000000ull + 90123456,
        0 * 100000000ull + 90123456,
        12345678 * 100000000ull,
        1234567 * 100000000ull,
        123456 * 100000000ull,
        12345 * 100000000ull,
        1234 * 100000000ull,
        123 * 100000000ull,
        12 * 100000000ull,
        1 * 100000000ull,
        0 * 100000000ull,
    };

    char out[17];
    for (int i = 0; i < sizeof(v) / sizeof(v[0]); i++)
    {
#ifdef __ARM_NEON
        to_string_neon_v6(v[i], out);
#else
        to_string_scalar(v[i], out);
#endif
        out[16] = '\0';
        std::cout << out << std::endl;
    }

    printf("NEON not available\n");


    return 0;
}

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

typedef __uint128_t u128;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;

const size_t N = 1ull << 16; // 65536
char *buf_ref;
char *buf;

uint64_t nano()
{
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
template <typename PROCEDURE>
double bench(PROCEDURE f, uint64_t threshold = 20'000'000) // 50ms
{
  uint64_t start = nano();
  uint64_t finish = start;
  size_t count{0};
  for (; finish - start < threshold;)
  {
    count += f();
    finish = nano();
  }

  // check result
  int is_ok = 1;
  for (size_t i = 0; i < N * 2; ++i)
  {
    uint64_t a, b;
    memcpy(&a, buf_ref + i * sizeof(uint64_t), sizeof(uint64_t));
    memcpy(&b, buf + i * sizeof(uint64_t), sizeof(uint64_t));
    //is_ok &= (a == b);
    if(a != b){
      is_ok = 0;
      printf("a = %llx,b = %llx ",(long long)a,(long long)b);
      break;
    }
  }
  if (is_ok == 0)
  {
    printf("check failed! ");
  }
  else
  {
    // printf("check ok!");
  }

  return double(finish - start) / count;
}
static inline void to_string_backlinear(uint64_t x, char *out)
{
  for (int z = 0; z < 16; z++)
  {
    out[15 - z] = (x % 10) + 0x30;
    x /= 10;
  }
}

static inline void to_string_linear(uint64_t x, char *out)
{
  out[0] = x / 1000000000000000 + 0x30;
  x %= 1000000000000000;
  out[1] = x / 100000000000000 + 0x30;
  x %= 100000000000000;
  out[2] = x / 10000000000000 + 0x30;
  x %= 10000000000000;
  out[3] = x / 1000000000000 + 0x30;
  x %= 1000000000000;
  out[4] = x / 100000000000 + 0x30;
  x %= 100000000000;
  out[5] = x / 10000000000 + 0x30;
  x %= 10000000000;
  out[6] = x / 1000000000 + 0x30;
  x %= 1000000000;
  out[7] = x / 100000000 + 0x30;
  x %= 100000000;
  out[8] = x / 10000000 + 0x30;
  x %= 10000000;
  out[9] = x / 1000000 + 0x30;
  x %= 1000000;
  out[10] = x / 100000 + 0x30;
  x %= 100000;
  out[11] = x / 10000 + 0x30;
  x %= 10000;
  out[12] = x / 1000 + 0x30;
  x %= 1000;
  out[13] = x / 100 + 0x30;
  x %= 100;
  out[14] = x / 10 + 0x30;
  x %= 10;
  out[15] = x + 0x30;
}

// credit: Paul Khuong
uint64_t encode_ten_thousands(uint64_t hi, uint64_t lo)
{
  uint64_t merged = hi | (lo << 32);
  /* Truncate division by 100: 10486 / 2**20 ~= 1/100. */
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

uint64_t encode_ten_thousands2(uint64_t hi, uint64_t lo)
{
  uint64_t merged = hi | (lo << 32);
  /* Truncate division by 100: 10486 / 2**20 ~= 1/100. */
  uint64_t top = ((merged * 0x147b) >> 19) & ((0x7FULL << 32) | 0x7FULL);
  /* Trailing 2 digits in the 1e4 chunks. */
  uint64_t bot = merged - 100ULL * top;
  uint64_t hundreds;
  uint64_t tens;

  hundreds = (bot << 16) + top;

  /* Divide and mod by 10 all 4 radix-100 digits in parallel. */
  tens = (hundreds * 103ULL) >> 10;
  tens &= (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
  tens += (hundreds - 10ULL * tens) << 8;
  return tens;
}

void to_string_khuong(uint64_t x, char *out)
{
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  uint64_t first =
      0x3030303030303030 + encode_ten_thousands(top / 10000, top % 10000);
  memcpy(out, &first, sizeof(first));
  uint64_t second =
      0x3030303030303030 + encode_ten_thousands(bottom / 10000, bottom % 10000);
  memcpy(out + 8, &second, sizeof(second));
}

#ifdef __ARM_NEON
// umulh       x11, x10, x8
// lsr         x11, x11, #26
// madd        w10, w11, w9, w10
// bfi         x11, x10, #32, #32
// fmov        d16, x11
// sqdmulh.2s  v17, v16, v0
// ushr.2s     v17, v17, #0x9
// mla.2s      v16, v17, v1
// zip1.8h     v16, v16, v4
// sqdmulh.4s  v17, v16, v2
// mla.4s      v16, v17, v3
// sqdmulh.8h  v17, v16, v5
// add.16b     v16, v16, v6
// mla.8h      v16, v17, v7
// rev64.16b   v16, v16

// ldr	x10, [x9], #8
// umulh	x11, x10, x22
// lsr	x11, x11, #26
// madd	w10, w11, w12, w10
// bfi	x11, x10, #32, #32
// fmov	d0, x11
// sqdmulh.2s	v1, v0, v9
// ushr.2s	v1, v1, #9
// mla.2s	v0, v1, v8
// ushll.4s	v1, v0, #0
// sqdmulh.4s	v1, v1, v2
// mul.4s	v1, v1, v3
// uaddw.4s	v0, v1, v0
// sqdmulh.8h	v1, v0, v4
// add.16b	v0, v0, v6
// mla.8h	v0, v1, v5
// rev64.16b	v0, v0
// str	q0, [x8], #16

void to_string_neon_old(uint64_t v, char *out)
{
  // Equivalent to hi = v / 100000000, lo = v % 100000000, but for some reason
  // clang emits a UDIV.
  uint32_t hi = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
  uint32_t lo = v - hi * 100000000;

  uint64x1_t hundredmillions = {hi | ((uint64_t)lo << 32)};

  int32x2_t high_10000 = vshr_n_u32(vqdmulh_s32(hundredmillions, vdup_n_s32(0x68db8bb)), 9);
  int32x2_t tenthousands = vmla_s32(hundredmillions, high_10000, vdup_n_s32(-10000 + 0x10000));

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
  // vst1q_s8((int8_t *)out, result);
}
// ldr	x10, [x9], #8
// ldp	x12, x11, [x13]
// umulh	x12, x12, x10
// lsr	x12, x12, #26
// msub	w10, w11, w12, w10
// bfi	x12, x10, #32, #32
// fmov	d0, x12
// ldp	q1, q2, [x13, #16]
// sqdmulh.2s	v3, v0, v1[0]
// ushr.2s	v3, v3, #9
// mla.2s	v0, v3, v1[1]
// ushll.4s	v0, v0, #0
// sqdmulh.4s	v3, v0, v1[2]
// mla.4s	v0, v3, v1[3]
// sqdmulh.8h	v1, v0, v2[0]
// add.16b	v0, v0, v4
// mla.8h	v0, v1, v2[1]
// rev64.16b	v0, v0
// str	q0, [x8], #16
void to_string_neon(uint64_t v, char *out)
{
  struct to_string_constants
  {
    uint64_t mul_const;       // 8
    uint64_t hundred_million; // 8
    int32x4_t multipliers32;  // 16
    int16x8_t multipliers16;  // 16
  };
  static const struct to_string_constants constants = {
      .mul_const = 0xabcc77118461cefd,
      .hundred_million = 100000000,
      .multipliers32 = {0x68db8bb, -10000 + 0x10000, 0x147b000, -100 + 0x10000},
      .multipliers16 = {0xce0, -10 + 0x100},
  };

  const struct to_string_constants *c = &constants;

  // Compiler barrier, or clang doesn't load from memory and generates 15 more instructions
  asm("" : "+r"(c));

  // Equivalent to hi = v / 100000000, lo = v % 100000000.
  uint64_t hundred_million = c->hundred_million;

  // Compiler barrier, or clang narrows the load to 32-bit and unpairs it.
  asm("" : "+r"(hundred_million));

  uint32_t hi = ((__uint128_t)v * c->mul_const) >> 90;
  uint32_t lo = v - hi * hundred_million;

  uint64x1_t hundredmillions = {hi | ((uint64_t)lo << 32)};

  int32x2_t high_10000 = vshr_n_u32(vqdmulh_n_s32(hundredmillions, c->multipliers32[0]), 9);
  int32x2_t tenthousands = vmla_n_s32(hundredmillions, high_10000, c->multipliers32[1]);

  int32x4_t extended = vshll_n_u16(tenthousands, 0);

  // Compiler barrier, or clang breaks the subsequent MLA into UADDW + MUL.
  asm("" : "+w"(extended));

  int32x4_t high_100 = vqdmulhq_n_s32(extended, c->multipliers32[2]);
  int32x4_t hundreds = vmlaq_n_s32(extended, high_100, c->multipliers32[3]);
  int16x8_t high_10 = vqdmulhq_n_s16(hundreds, c->multipliers16[0]);
  int16x8_t digits = vmlaq_n_s16(vaddq_s8(hundreds, vdupq_n_s8('0')), high_10, c->multipliers16[1]);
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
void to_string_neon_v3_new(uint64_t v, char *out)
{
  // uint64_t abcdefgh = ((__uint128_t)v * 0xabcc77118461cefd) >> 90;
  // uint64_t ijklmnop = v + abcdefgh * (-100000000);
  // uint64_t abcd_efgh = abcdefgh + ((1ull << 32) - 10000) * ((abcdefgh * (__uint128_t)1844674407370956) >> 64); // (abcd << 32) + efgh
  // uint64_t ijkl_mnop = ijklmnop + ((1ull << 32) - 10000) * ((ijklmnop * (__uint128_t)1844674407370956) >> 64); // (ijkl << 32) + mnop
  // uint64x2_t merge4 = vcombine_u64(vcreate_u64(abcd_efgh), vcreate_u64(ijkl_mnop));
  // uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
  // uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
  // uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
  // uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
  // memcpy(out, &ASCII_16, sizeof(ASCII_16));

  struct to_string_constants
  {
    uint64_t mul_const;       // 8
    int64_t hundred_million;  // 8
    uint32x4_t multipliers32; // 16
    int16x8_t multipliers16;  // 16
    uint64_t multipliers64;
  };
  static const struct to_string_constants constants = {
      .mul_const = 0xabcc77118461cefd,
      .hundred_million = -100000000,
      .multipliers32 = {0x100000000 - 10000, 109951163, 0x147b000, 0x10000 - 100},
      .multipliers16 = {0xce0, -10 + 0x100},
      .multipliers64 = 1844674407370956,
  };
  const struct to_string_constants *c = &constants;
  asm("" : "+r"(c));
  uint64_t hundred_million = c->hundred_million;
  asm("" : "+r"(hundred_million));
  uint64_t abcdefgh = ((__uint128_t)v * c->mul_const) >> 90;
  uint64_t ijklmnop = v + abcdefgh * hundred_million;
  uint64_t abcd_efgh = abcdefgh + c->multipliers32[0] * ((abcdefgh * (__uint128_t)c->multipliers64) >> 64); // (abcd << 32) + efgh
  uint64_t ijkl_mnop = ijklmnop + c->multipliers32[0] * ((ijklmnop * (__uint128_t)c->multipliers64) >> 64); // (ijkl << 32) + mnop
  // uint64_t abcd_efgh = abcdefgh + c->multipliers32[0] * ((abcdefgh * c->multipliers32[1]) >> 40); // (abcd << 32) + efgh
  // uint64_t ijkl_mnop = ijklmnop + c->multipliers32[0] * ((ijklmnop * c->multipliers32[1]) >> 40); // (ijkl << 32) + mnop
  uint64x2_t merge4 = vcombine_u64(vcreate_u64(abcd_efgh), vcreate_u64(ijkl_mnop));
  uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(c->multipliers32[2])), c->multipliers32[3]);
  uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(c->multipliers16[0])), c->multipliers16[1]);
  uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
  uint64x2_t ASCII_16 = vorrq_u64(BCD_little_endian, vdupq_n_s8('0'));
  memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
void to_string_neon_v4(uint64_t v, char *out) // equal to v3
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
  uint64x2_t merge8 = vsetq_lane_s32(ijklmnop, vdupq_n_s32(abcdefgh), 1);
  // uint64x2_t merge8 = vdupq_n_s64((ijklmnop << 32) | abcdefgh);
  int32x4_t high_10000 = vshrq_n_u32(vqdmulhq_s32(merge8, vdupq_n_s32(0x68db8bb)), 9); // 0000 , abcd , 0000 , ijkl
  int32x4_t low_10000 = vmlaq_n_u32(merge8, high_10000, -10000);                       // 0000 , efgh , 0000 , mnop
  int32x4_t merge4 = vzip1q_s32(low_10000, high_10000);                                // abcd , efgh , ijkl , mnop
  uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(0x147b000)), -100 + 0x10000);
  uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(0xce0)), -10 + 0x100);
  uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
  uint64x2_t ASCII_16 = vaddq_u64(BCD_little_endian, vdupq_n_s8('0'));
  memcpy(out, &ASCII_16, sizeof(ASCII_16));
}
void to_string_neon_v5_new(uint64_t v, char *out) // slower than v2 or v3
{
  struct to_string_constants
  {
    uint64_t mul_const;       // 8
    int64_t hundred_million;  // 8
    uint32x4_t multipliers32; // 16
    int16x8_t multipliers16;  // 16
  };
  static const struct to_string_constants constants = {
      .mul_const = 0xabcc77118461cefd,
      .hundred_million = -100000000,
      .multipliers32 = {0x100000000 - 10000, 109951163, 0x147b000, 0x10000 - 100},
      .multipliers16 = {0xce0, -10 + 0x100, -10000},
  };
  const struct to_string_constants *c = &constants;
  asm("" : "+r"(c));
  uint64_t hundred_million = c->hundred_million;
  asm("" : "+r"(hundred_million));
  uint64_t abcdefgh = ((__uint128_t)v * c->mul_const) >> 90;
  uint64_t ijklmnop = v + abcdefgh * hundred_million;
  uint64x2_t merge8 = vsetq_lane_s32(ijklmnop, vdupq_n_s32(abcdefgh), 1);
  int32x4_t high_10000 = vshrq_n_u32(vqdmulhq_s32(merge8, vdupq_n_s32(c->multipliers32[1])), 9); // 0000 , abcd , 0000 , ijkl
  int32x4_t low_10000 = vmlaq_n_u32(merge8, high_10000, c->multipliers16[2]);                    // 0000 , efgh , 0000 , mnop
  int32x4_t merge4 = vzip1q_s32(low_10000, high_10000);                                          // abcd , efgh , ijkl , mnop
  uint64x2_t merge2 = vmlaq_n_u32(merge4, vqdmulhq_s32(merge4, vdupq_n_s32(c->multipliers32[2])), c->multipliers32[3]);
  uint64x2_t BCD_big_endian = vmlaq_n_u16(merge2, vqdmulhq_s16(merge2, vdupq_n_s16(c->multipliers16[0])), c->multipliers16[1]);
  uint64x2_t BCD_little_endian = vrev64q_u8(BCD_big_endian);
  uint64x2_t ASCII_16 = vaddq_u64(BCD_little_endian, vdupq_n_s8('0'));
  memcpy(out, &ASCII_16, sizeof(ASCII_16));
}

#endif

// take a 16-digit integer, < 10000000000000000,
// and write it out.
void to_string_tree(uint64_t x, char *out)
{
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;
  //
  uint64_t toptoptop = toptop / 100;
  uint64_t toptopbottom = toptop % 100;

  uint64_t topbottomtop = topbottom / 100;
  uint64_t topbottombottom = topbottom % 100;

  uint64_t bottomtoptop = bottomtop / 100;
  uint64_t bottomtopbottom = bottomtop % 100;

  uint64_t bottombottomtop = bottombottom / 100;
  uint64_t bottombottombottom = bottombottom % 100;
  //
  out[0] = toptoptop / 10 + 0x30;
  out[1] = toptoptop % 10 + 0x30;
  out[2] = toptopbottom / 10 + 0x30;
  out[3] = toptopbottom % 10 + 0x30;
  out[4] = topbottomtop / 10 + 0x30;
  out[5] = topbottomtop % 10 + 0x30;
  out[6] = topbottombottom / 10 + 0x30;
  out[7] = topbottombottom % 10 + 0x30;
  out[8] = bottomtoptop / 10 + 0x30;
  out[9] = bottomtoptop % 10 + 0x30;
  out[10] = bottomtopbottom / 10 + 0x30;
  out[11] = bottomtopbottom % 10 + 0x30;
  out[12] = bottombottomtop / 10 + 0x30;
  out[13] = bottombottomtop % 10 + 0x30;
  out[14] = bottombottombottom / 10 + 0x30;
  out[15] = bottombottombottom % 10 + 0x30;
}

void write_tenthousand(uint64_t z, char *out)
{
  z = 429538 * z;
  out[0] = 0x30 + ((z * 10) >> 32);
  z = (z * 10) & 0xffffffff;
  out[1] = 0x30 + ((z * 10) >> 32);
  z = (z * 10) & 0xffffffff;
  out[2] = 0x30 + ((z * 10) >> 32);
  z = (z * 10) & 0xffffffff;
  out[3] = 0x30 + ((z * 10) >> 32);
}

void to_string_tree_str(uint64_t x, char *out)
{
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;
  //
  write_tenthousand(toptop, out);
  write_tenthousand(topbottom, out + 4);
  write_tenthousand(bottomtop, out + 8);
  write_tenthousand(bottombottom, out + 12);
}

void to_string_tree_table(uint64_t x, char *out)
{
  static const char table[] =
      "0001020304050607080910111213141516171819"
      "2021222324252627282930313233343536373839"
      "4041424344454647484950515253545556575859"
      "6061626364656667686970717273747576777879"
      "8081828384858687888990919293949596979899";
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;
  //
  uint64_t toptoptop = toptop / 100;
  uint64_t toptopbottom = toptop % 100;

  uint64_t topbottomtop = topbottom / 100;
  uint64_t topbottombottom = topbottom % 100;

  uint64_t bottomtoptop = bottomtop / 100;
  uint64_t bottomtopbottom = bottomtop % 100;

  uint64_t bottombottomtop = bottombottom / 100;
  uint64_t bottombottombottom = bottombottom % 100;
  //
  memcpy(out, &table[2 * toptoptop], 2);
  memcpy(out + 2, &table[2 * toptopbottom], 2);
  memcpy(out + 4, &table[2 * topbottomtop], 2);
  memcpy(out + 6, &table[2 * topbottombottom], 2);
  memcpy(out + 8, &table[2 * bottomtoptop], 2);
  memcpy(out + 10, &table[2 * bottomtopbottom], 2);
  memcpy(out + 12, &table[2 * bottombottomtop], 2);
  memcpy(out + 14, &table[2 * bottombottombottom], 2);
}
// http://www.cs.uiowa.edu/~jones/bcd/decimal.html
/*
void putdec( uint64_t n, char *out ) {
        uint32_t d4, d3, d2, d1, d0, q;

        d0 = n       & 0xFFFF;
        d1 = (n>>16) & 0xFFFF;
        d2 = (n>>32) & 0xFFFF;
        d3 = (n>>48) & 0xFFFF;

        d0 = 656 * d3 + 7296 * d2 + 5536 * d1 + d0;
        q = d0 / 10000;
        d0 = d0 % 10000;

        d1 = q + 7671 * d3 + 9496 * d2 + 6 * d1;
        q = d1 / 10000;
        d1 = d1 % 10000;

        d2 = q + 4749 * d3 + 42 * d2;
        q = d2 / 10000;
        d2 = d2 % 10000;

        d3 = q + 281 * d3;
        q = d3 / 10000;
        d3 = d3 % 10000;

        d4 = q;

        printf( "%4.4u", d4 );
        printf( "%4.4u", d3 );
        printf( "%4.4u", d2 );
        printf( "%4.4u", d1 );
        printf( "%4.4u", d0 );
 }
 */

#ifdef __SSE2__
// mula
#include <x86intrin.h>
void to_string_sse2(uint64_t v, char *out)
{

  // v is 16-digit number = abcdefghijklmnop
  const __m128i div_10000 = _mm_set1_epi32(0xd1b71759);
  const __m128i mul_10000 = _mm_set1_epi32(10000);
  const int div_10000_shift = 45;

  const __m128i div_100 = _mm_set1_epi16(0x147b);
  const __m128i mul_100 = _mm_set1_epi16(100);
  const int div_100_shift = 3;

  const __m128i div_10 = _mm_set1_epi16(0x199a);
  const __m128i mul_10 = _mm_set1_epi16(10);

  const __m128i ascii0 = _mm_set1_epi8('0');

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
  const uint32_t b = v % 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1
  //                | 0 ]
  // x            = [       0       |      ijklmnop |       0       | abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a);

  // x div 10^4   = [       0       |          ijkl |       0       | abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);

  // x mod 10^4   = [       0       |          mnop |       0       | efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh | abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

  // y_div_100    = [   0   |    mn |   0   |    ij |   0   |    ef |   0   | ab
  // ]
  __m128i y_div_100;
  y_div_100 = _mm_mulhi_epu16(y, div_100);
  y_div_100 = _mm_srli_epi16(y_div_100, div_100_shift);

  // y_mod_100    = [   0   |    op |   0   |    kl |   0   |    gh |   0   | cd
  // ]
  __m128i y_mod_100;
  y_mod_100 = _mm_mullo_epi16(y_div_100, mul_100);
  y_mod_100 = _mm_sub_epi16(y, y_mod_100);

  // z            = [    mn |    op |    ij |    kl |    ef |    gh |    ab | cd
  // ]
  __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));

  // z_div_10     = [ 0 | m | 0 | o | 0 | i | 0 | k | 0 | e | 0 | g | 0 | a | 0
  // | c ]
  __m128i z_div_10 = _mm_mulhi_epu16(z, div_10);

  // z_mod_10     = [ 0 | n | 0 | p | 0 | j | 0 | l | 0 | f | 0 | h | 0 | b | 0
  // | d ]
  __m128i z_mod_10;
  z_mod_10 = _mm_mullo_epi16(z_div_10, mul_10);
  z_mod_10 = _mm_sub_epi16(z, z_mod_10);

  // interleave z_mod_10 and z_div_10 -
  // tmp          = [ m | n | o | p | i | j | k | l | e | f | g | h | a | b | c
  // | d ]
  __m128i tmp = _mm_or_si128(z_div_10, _mm_slli_epi16(z_mod_10, 8));

  // convert to ascii
  tmp = _mm_add_epi8(tmp, ascii0);

  // and save result
  _mm_storeu_si128((__m128i *)out, tmp);
}
void to_string_sse2_v2(uint64_t v, char *out)
{
  // magic numbers for 16-bit division
#define DIV_10 0x199a  // shift = 0 + 16
#define DIV_100 0x147b // shift = 3 + 16
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
  // const __m128i mul_10    = _mm_set1_epi16(10);

  const __m128i ascii0 = _mm_set1_epi8('0'); // sse2

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000;     // 8-digit number: abcdefgh
  const uint32_t b = v - a * 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 ]
  // x            = [       0       |      ijklmnop |       0       |      abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a); // sse2

  // x div 10^4   = [       0       |          ijkl |       0       |          abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);                  // sse2
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift); // sse2

  // x mod 10^4   = [       0       |          mnop |       0       |          efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh |          abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

// #if defined(__SSE4_1__) // __GNUC__ : gcc icpx clang  ; for MSVC how to check cpu support sse4.1
#if 0
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
  __m128i tmp = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));

  // unsigned int mask = _mm_movemask_epi8(_mm_cmpgt_epi8(tmp, _mm_setzero_si128()));

  tmp = _mm_add_epi8(tmp, ascii0);

  _mm_storeu_si128((__m128i *)out, tmp);
}
void to_string_sse2_v3(uint64_t v, char *out)
{
  const uint64_t a = v / 100000000;        // 8-digit number: abcdefgh
  const uint64_t b = v + a * (-100000000); // 8-digit number: ijklmnop
  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 ]
  // x            = [       0       |      ijklmnop |       0       |      abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a); // sse2
  // y            = [          ijkl |          mnop |          abcd |          efgh ]
  __m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
  // z            = [    ik |    kl |    mn |    op |    ab |    cd |    ef |    gh ]
#ifdef __SSE4_1__
  __m128i z = _mm_add_epi64(y, _mm_mullo_epi32(_mm_set1_epi32((1ull << 16) - 100), _mm_srli_epi32(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3))); //_mm_mullo_epi32 : sse4.1
#else
  __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(5243)), 3);
  __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
  // z            = [    ik |    kl |    mn |    op |    ab |    cd |    ef |    gh ]
  __m128i z = _mm_or_epi32(y_mod_100, _mm_slli_epi32(y_div_100, 16));
#endif
  // big_endian   = [ i | j | k | l | m | n | o | p | a | b | c | d | e | f | g | h ]
  __m128i big_endian_bcd = _mm_add_epi64(z, _mm_mullo_epi16(_mm_set1_epi16((1 << 8) - 10), _mm_mulhi_epu16(z, _mm_set1_epi16(6554))));
  __m128i big_endian_ascii = _mm_add_epi64(big_endian_bcd, _mm_set1_epi8('0')); // BCD to ASCII

#if defined(__SSSE3__)
  // method 1
  __m128i little_endian_ascii = _mm_shuffle_epi8(big_endian_ascii, _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)); // ssse3
  _mm_storeu_si128((__m128i *)out, little_endian_ascii);
#else
  // method 2 : only need sse2;
  uint64_t tmp_buf[2];
  _mm_storeu_si128((__m128i *)tmp_buf, big_endian_ascii);
  uint64_t hi = _bswap64(tmp_buf[0]); // big-endian to little-endian
  uint64_t lo = _bswap64(tmp_buf[1]);
  memcpy(out + 0, &hi, 8);
  memcpy(out + 8, &lo, 8);
#endif
}

#endif

#ifdef __SSE4_1__
void to_string_sse2__pshufb(uint64_t v, char *out)
{

  // v is 16-digit number = abcdefghijklmnop
  const __m128i div_10000 = _mm_set1_epi32(0xd1b71759);
  const __m128i mul_10000 = _mm_set1_epi32(10000);
  const int div_10000_shift = 45;

  const __m128i div_100 = _mm_set1_epi16(0x147b);
  const __m128i mul_100 = _mm_set1_epi16(100);
  const int div_100_shift = 3;

  const __m128i div_10 = _mm_set1_epi16(0x199a);
  const __m128i mul_10 = _mm_set1_epi16(10);

  const __m128i ascii0 = _mm_set1_epi8('0');

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
  const uint32_t b = v % 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1
  //                | 0 ]
  // x            = [       0       |      ijklmnop |       0       | abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a);

  // x div 10^4   = [       0       |          ijkl |       0       | abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);

  // x mod 10^4   = [       0       |          mnop |       0       | efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh | abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

  // y_div_100    = [   0   |    mn |   0   |    ij |   0   |    ef |   0   | ab
  // ]
  __m128i y_div_100;
  y_div_100 = _mm_mulhi_epu16(y, div_100);
  y_div_100 = _mm_srli_epi16(y_div_100, div_100_shift);

  // y_mod_100    = [   0   |    op |   0   |    kl |   0   |    gh |   0   | cd
  // ]
  __m128i y_mod_100;
  y_mod_100 = _mm_mullo_epi16(y_div_100, mul_100);
  y_mod_100 = _mm_sub_epi16(y, y_mod_100);

  // z            = [    mn |    ij |    ef |    ab |    op |    kl |    gh | cd
  // ]
  __m128i z = _mm_packus_epi32(y_div_100, y_mod_100);

  // z_div_10     = [ 0 | m | 0 | i | 0 | e | 0 | a | 0 | o | 0 | k | 0 | g | 0
  // | c ]
  __m128i z_div_10 = _mm_mulhi_epu16(z, div_10);

  // z_mod_10     = [ 0 | n | 0 | j | 0 | f | 0 | b | 0 | p | 0 | l | 0 | h | 0
  // | d ]
  __m128i z_mod_10;
  z_mod_10 = _mm_mullo_epi16(z_div_10, mul_10);
  z_mod_10 = _mm_sub_epi16(z, z_mod_10);

  // interleave z_mod_10 and z_div_10 -
  // tmp          = [ m | i | e | a | o | k | g | c | n | j | f | b | p | l | h
  // | d ]
  __m128i tmp = _mm_packus_epi16(z_div_10, z_mod_10);

  const __m128i reorder =
      _mm_set_epi8(15, 7, 11, 3, 14, 6, 10, 2, 13, 5, 9, 1, 12, 4, 8, 0);
  tmp = _mm_shuffle_epi8(tmp, reorder);

  // convert to ascii
  tmp = _mm_add_epi8(tmp, ascii0);

  // and save result
  _mm_storeu_si128((__m128i *)out, tmp);
}
void to_string_sse4_1_v2(uint64_t v, char *out) {
    uint32_t hi = ((__uint128_t)v * 0xabcc77118461cefdULL) >> 90;
    uint32_t lo = v - hi * 100000000;

    __m128i hundredmillions = _mm_set_epi64x(lo, hi);

    __m128i high_10000 = _mm_srli_epi64(_mm_mul_epi32(hundredmillions, _mm_set1_epi32(0x68db8bb)), 40);
    __m128d tenthousands = _mm_fmadd_pd((_mm_set1_pd((double)(-10000 + 0x100000000))), _mm_castsi128_pd(high_10000), _mm_castsi128_pd(hundredmillions));

    __m128i high_100 = _mm_srli_epi32(_mm_mullo_epi32(_mm_castpd_si128(tenthousands), _mm_set1_epi32(0x147b)), 19);
    __m128i hundreds = _mm_castpd_si128(_mm_castps_pd(_mm_fmadd_ps(_mm_set1_ps((float)(-100 + 0x10000)), _mm_castsi128_ps(high_100), _mm_castpd_ps(tenthousands))));

    __m128i high_10 = _mm_mulhi_epi16( hundreds, _mm_set1_epi16(0x19c0));
    __m128i digits = _mm_add_epi16( hundreds, _mm_mullo_epi16(high_10, _mm_set1_epi16(-10 + 0x100)));

    __m128i bcd = _mm_shuffle_epi8(digits, _mm_setr_epi8(7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8));

    __m128i ascii = _mm_add_epi16(bcd, _mm_set1_epi8('0'));

    _mm_storeu_si128((__m128i *)out, ascii);
}
#endif

#ifdef __AVX2__
void to_string_avx2_biglut(uint64_t v, char *out)
{

  // begin: copy of to_string_sse2

  // v is 16-digit number = abcdefghijklmnop
  const __m128i div_10000 = _mm_set1_epi32(0xd1b71759);
  const __m128i mul_10000 = _mm_set1_epi32(10000);
  const int div_10000_shift = 45;

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
  const uint32_t b = v % 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1
  //                | 0 ]
  // x            = [       0       |      ijklmnop |       0       | abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a);

  // x div 10^4   = [       0       |          ijkl |       0       | abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);

  // x mod 10^4   = [       0       |          mnop |       0       | efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh | abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

  // end of copy, AVX2 code now
#include "bigtable.h"

  const __m128i ascii =
      _mm_i32gather_epi32(reinterpret_cast<int const *>(&bigtable), y, 4);

  _mm_storeu_si128((__m128i *)out, ascii);
}
#endif

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__)

/*
The IFMA decimal print method:

Computing remainder of n by 10, 100, ..., 10^8 and then multiplication by 10.

n = 84736251
1              = n mod 10
51             = n mod 100
251            = n mod 1000
6251
36251
736251
4736251
84736251       = n mod 10^8

From this paper
https://arxiv.org/abs/1902.01961
 page 8:

      uint32_t d = ...; // your divisor > 0
      // c = ceil ( (1 < <64) / d ) ; we take L = N
      uint64_t c = UINT64_C (0 xFFFFFFFFFFFFFFFF ) / d + 1;
      // fastmod computes (n mod d) given precomputed c
      uint32_t fastmod ( uint32_t n, uint64_t c, uint32_t d) {
          uint64_t lowbits = c * n;
          return (( __uint128_t ) lowbits * d) >> 64;
      }

Fastmod fits well for this AVX512FMA instruction pair:
VPMADD52LUQ => lowbits = c * n + 0
VPMADD52HUQ => highbits = lowbits * 10 + asciiZero
just uses 52b and 104b numbers instead of 64 and 128, and highbits use 10 instead of d, and produces 8 decimal digits for 0 <= n <= 99999999.

The only problem is that in the 8th digit case the VPMADD52HUQ overflows, if we use the orginial 0x2af31dd ( = (2^53 - 1)/(10^8) + 1) constant as c in VPMADD52LUQ:

0x2af31dd * 99999999 = 0x10000001a50b23

Solution: we use 0x2af31dc = 0x2af31dd - 1 as c, and use 0x1A1A400 bias instead of 0. 0x1A1A400 is the smallest bias, which does not underflows in case of the smallest 8-digit number:

0x2af31dc * 10000000 = 0x19999996FD600 = 450359960000000
(0x19999996FD600 + 0x1A1A400) * 10 = 0x1000000EAEC400
*/

void to_string_avx512ifma_vbmi(uint64_t n, char *out)
{
  uint64_t n_15_08 = n / 100000000;
  uint64_t n_07_00 = n + n_15_08 * (-100000000);
  __m512i bcstq_h = _mm512_set1_epi64(n_15_08);
  __m512i bcstq_l = _mm512_set1_epi64(n_07_00);
  const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
  const __m512i zmmTen = _mm512_set1_epi64(10);
  const __m512i asciiZero = _mm512_set1_epi64('0');

  const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b,
                                         0x0000000010c6f7a0c, 0x00000000a7c5ac472, 0x000000068db8bac72, 0x0000004189374bc6b,
                                         0x0000028f5c28f5c29, 0x0000199999999999a);
  const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58,
                                                            0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
  __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
  __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
  __m512i highbits_h = _mm512_madd52hi_epu64(asciiZero, zmmTen, lowbits_h);
  __m512i highbits_l = _mm512_madd52hi_epu64(asciiZero, zmmTen, lowbits_l);
  __m512i perm = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
  __m128i digits_15_0 = _mm512_castsi512_si128(perm);
  _mm_storeu_si128((__m128i *)out, digits_15_0);
}
#endif

#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__)

#endif


#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__)
#include <immintrin.h>
// method ：vpshufb  , need AVX-512BW
__m512i bswap64_avx512(__m512i x)
{
  // 创建交换掩码：每个64位元素内的字节反转
  const __m512i mask = _mm512_set_epi8(
      56, 57, 58, 59, 60, 61, 62, 63, // 7
      48, 49, 50, 51, 52, 53, 54, 55, // 6
      40, 41, 42, 43, 44, 45, 46, 47, // 5
      32, 33, 34, 35, 36, 37, 38, 39, // 4
      24, 25, 26, 27, 28, 29, 30, 31, // 3
      16, 17, 18, 19, 20, 21, 22, 23, // 2
      8, 9, 10, 11, 12, 13, 14, 15,   // 1
      0, 1, 2, 3, 4, 5, 6, 7          // 0
  );

  return _mm512_shuffle_epi8(x, mask);
}
void to_string_avx512f_block32(uint64_t *data, char *out)
{
  // print 32 numbers to out buf; data range : [0, 10^16)
  // out buf size = 16*32 bytes = 512 bytes
#define FORI4 for (int i = 0; i < 4; i++)

  __m512i x[4]; // x = abcdefgh * 10^8 + ijklmnop
  __m512i abcdefgh[4], abcd[4], abcd_efgh[4], ab_cd_ef_gh[4], a_b_c_d_e_f_g_h[4], abcdefgh_ascii[4];
  __m512i ijklmnop[4], ijkl[4], ijkl_mnop[4], ij_kl_mn_op[4], i_j_k_l_m_n_o_p[4], ijklmnop_ascii[4];
  FORI4 x[i] = _mm512_loadu_epi64(&data[i * 8]);
  FORI4 abcdefgh[i] = _mm512_cvttpd_epi64(_mm512_mul_pd(_mm512_cvt_roundepi64_pd(x[i], _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC), // round down
                                                        _mm512_set1_pd(1e-8)));
  // FORI4 ijklmnop[i] = _mm512_sub_epi64(x[i], _mm512_mul_epu32(abcdefgh[i], _mm512_set1_epi64(100000000)));
  FORI4 ijklmnop[i] = _mm512_add_epi64(x[i], _mm512_mul_epi32(abcdefgh[i], _mm512_set1_epi64(-100000000)));
  // uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
  FORI4 abcd[i] = _mm512_srli_epi64(_mm512_mullo_epi64(abcdefgh[i], _mm512_set1_epi64(0x68db8bb)), 40);
  FORI4 ijkl[i] = _mm512_srli_epi64(_mm512_mullo_epi64(ijklmnop[i], _mm512_set1_epi64(0x68db8bb)), 40);
  FORI4 abcd_efgh[i] = _mm512_add_epi64(abcdefgh[i], _mm512_mullo_epi64(abcd[i], _mm512_set1_epi64((1ull << 32) - 10000)));
  FORI4 ijkl_mnop[i] = _mm512_add_epi64(ijklmnop[i], _mm512_mullo_epi64(ijkl[i], _mm512_set1_epi64((1ull << 32) - 10000)));
  // uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
  FORI4 ab_cd_ef_gh[i] = _mm512_add_epi64(abcd_efgh[i], _mm512_mullo_epi32(_mm512_set1_epi32(0x10000 - 100), _mm512_srli_epi32(_mm512_mullo_epi32(abcd_efgh[i], _mm512_set1_epi32(5243)), 19)));
  FORI4 ij_kl_mn_op[i] = _mm512_add_epi64(ijkl_mnop[i], _mm512_mullo_epi32(_mm512_set1_epi32(0x10000 - 100), _mm512_srli_epi32(_mm512_mullo_epi32(ijkl_mnop[i], _mm512_set1_epi32(5243)), 19)));
  // uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;
  FORI4 a_b_c_d_e_f_g_h[i] = _mm512_add_epi64(ab_cd_ef_gh[i], _mm512_mullo_epi16(_mm512_set1_epi16(0x100 - 10), _mm512_mulhi_epi16(ab_cd_ef_gh[i], _mm512_set1_epi16(6554))));
  FORI4 i_j_k_l_m_n_o_p[i] = _mm512_add_epi64(ij_kl_mn_op[i], _mm512_mullo_epi16(_mm512_set1_epi16(0x100 - 10), _mm512_mulhi_epi16(ij_kl_mn_op[i], _mm512_set1_epi16(6554))));
  FORI4 abcdefgh_ascii[i] = bswap64_avx512(_mm512_add_epi64(a_b_c_d_e_f_g_h[i], _mm512_set1_epi8('0')));
  FORI4 ijklmnop_ascii[i] = bswap64_avx512(_mm512_add_epi64(i_j_k_l_m_n_o_p[i], _mm512_set1_epi8('0')));

  alignas(64) static char tmp_buf[32 * 16]; // 512byte
  FORI4 _mm512_storeu_epi64(&tmp_buf[i * 64], abcdefgh_ascii[i]);
  FORI4 _mm512_storeu_epi64(&tmp_buf[i * 64 + 256], ijklmnop_ascii[i]);

  // out index : 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 ... 60 61 62 63
  // tmp_buf index : 0 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51 53 55 57 59 61 63
  for (int i = 0; i < 32; i++)
  {
    memcpy(&out[(2 * i + 0) * 8], &tmp_buf[(i + 0) * 8], 8);
    memcpy(&out[(2 * i + 1) * 8], &tmp_buf[(i + 32) * 8], 8);
  }

  // uint64_t variable[14];
  // FORI4 {
  //   _mm512_storeu_epi64(&out[i * 128], abcdefgh_ascii[i]);
  //   _mm512_storeu_epi64(&out[i * 128 + 64], ijklmnop_ascii[i]);
  //   for(int j=0;j<14;j++)memcpy(&variable[j],&out[i*128 + (j+1)*8],8);
  //   for(int j=0;j<7;j++)memcpy(&out[i*128 + (j+1)*16],&variable[j],8);
  //   for(int j=0;j<7;j++)memcpy(&out[i*128 + (j+1)*16 - 8],&variable[j+7],8);
  // }
}

#endif

#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__) && defined(__AVX2__) 

void to_string_avx512f_BW(uint64_t n, char *out)
{
    // need AVX512F,AVX512CD,AVX512BW,AVX512BW,AVX,sse4.1

    u64 aabbccdd = n / 100000000;
    u64 eeffgghh = n + aabbccdd * (-100000000);

    __m256i l8_4r = _mm256_set1_epi64x(eeffgghh); // AVX
    __m512i h8_4r = _mm512_castsi256_si512(_mm256_set1_epi64x(aabbccdd));
    __m512i n8r = _mm512_inserti64x4(h8_4r, l8_4r, 1); // AVX512F
    const u64 m8 = 180143986;                          //>>> 2**54/1e8
    const u64 m7 = 1801439851;                         //>>> 2**54/1e7
    const u64 m6 = 18014398510;                        //>>> 2**54/1e6
    const u64 m5 = 180143985095;                       //>>> 2**54/1e5
    const u64 m4 = 1801439850949;                      //>>> 2**54/1e4
    const u64 m3 = 18014398509482;                     //>>> 2**54/1e3
    const u64 m2 = 180143985094820;                    //>>> 2**54/1e2
    const u64 m1 = 1801439850948198;                   //>>> 2**54/1e1
    const __m512i mr = _mm512_set_epi64(m8, m6, m4, m2, m8, m6, m4, m2);
    const __m512i odd = _mm512_set_epi64(m7, m5, m3, m1, m7, m5, m3, m1);
    const __m512i M54_8_all = _mm512_set1_epi64(((u64)1 << 54) - 1);
    const __m512i M8_8_2 = _mm512_set1_epi64(0xff00);
    const __m512i t10r = _mm512_set1_epi64(10);
    const __m512i ZERO = _mm512_set1_epi64((0x30303030ull << 32) + 0x30303030ull);
    __m512i tmp_8_0 = _mm512_mullo_epi64(n8r, mr);          // AVX512DQ
    __m512i tmp_8_1 = _mm512_and_epi64(tmp_8_0, M54_8_all); // AVX512F
    __m512i tmp_8_2 = _mm512_mullo_epi64(tmp_8_1, t10r);
    __m512i tmp_8_3_t = _mm512_and_epi64(tmp_8_2, M54_8_all);
    __m512i tmp_8_3 = _mm512_mullo_epi64(tmp_8_3_t, t10r);
    __m512i tmp_8_1_print = _mm512_srli_epi64(tmp_8_2, 54);
    __m512i tmp_8_2_print = _mm512_and_epi64(_mm512_srli_epi64(tmp_8_3, (54 - 8)), M8_8_2);
    __m512i BCD = tmp_8_1_print | tmp_8_2_print | ZERO;
    const short idx[8] = {12, 8, 4, 0, 28, 24, 20, 16}; // 16byte
    const __m512i idxr_epi16 = _mm512_castsi128_si512(_mm_loadu_epi64(idx));
    BCD = _mm512_permutexvar_epi16(idxr_epi16, BCD); // AVX512BW
    //__m512i ASCII = _mm512_or_epi64(ZERO , BCD);
    
    _mm_storeu_si128((__m128i*)out, _mm512_castsi512_si128(BCD));

    // __m512i tz = _mm512_srli_epi64(_mm512_lzcnt_epi64(BCD), 3); // AVX512CD  , lzcnt(0)=64
    // *out = _mm512_extracti32x4_epi32(ASCII, 0);                 // xmm register ; no operation on zmm register
    // __m128i tz2 = _mm512_extracti32x4_epi32(tz, 0);             // xmm register
    // u64 tz0 = _mm_extract_epi64(tz2, 0);                        // sse4.1
    // u64 tz1 = _mm_extract_epi64(tz2, 1);
    // return tz1 == 8 ? 8 + tz0 : tz1;
}
void to_string_avx512f_BW_v2(uint64_t n, char *out)
{
    // need AVX512F,AVX512CD,AVX512BW,AVX512BW,AVX,sse4.1

    u64 aabbccdd = n / 100000000;
    u64 eeffgghh = n + aabbccdd * (-100000000);

    __m256i l8_4r = _mm256_set1_epi64x(eeffgghh); // AVX
    __m512i h8_4r = _mm512_castsi256_si512(_mm256_set1_epi64x(aabbccdd));
    __m512i n8r = _mm512_inserti64x4(h8_4r, l8_4r, 1); // AVX512F
    const u64 m8 = 180143986;                          //>>> 2**54/1e8
    const u64 m7 = 1801439851;                         //>>> 2**54/1e7
    const u64 m6 = 18014398510;                        //>>> 2**54/1e6
    const u64 m5 = 180143985095;                       //>>> 2**54/1e5
    const u64 m4 = 1801439850949;                      //>>> 2**54/1e4
    const u64 m3 = 18014398509482;                     //>>> 2**54/1e3
    const u64 m2 = 180143985094820;                    //>>> 2**54/1e2
    const u64 m1 = 1801439850948199;                   //>>> 2**54/1e1
    const __m512i even = _mm512_set_epi64(m8, m6, m4, m2, m8, m6, m4, m2);
    const __m512i odd = _mm512_set_epi64(m7, m5, m3, m1, m7, m5, m3, m1);
    const __m512i M54_8_all = _mm512_set1_epi64(((u64)1 << 54) - 1);
    const __m512i M8_8_2 = _mm512_set1_epi64(0xff00);
    const __m512i t10r = _mm512_set1_epi64(10);
    const __m512i ZERO = _mm512_set1_epi64((0x30303030ull << 32) + 0x30303030ull);

    __m512i even_digit = _mm512_and_epi64(_mm512_mullo_epi64(n8r, even),M54_8_all);
    __m512i odd_digit = _mm512_and_epi64(_mm512_mullo_epi64(n8r, odd),M54_8_all);

    // even_digit = _mm512_srli_epi64(_mm512_add_epi64(_mm512_slli_epi64(even_digit, 2), even_digit),53);
    // odd_digit = _mm512_srli_epi64(_mm512_add_epi64(_mm512_slli_epi64(odd_digit, 2), odd_digit),53);

    even_digit = _mm512_srli_epi64(_mm512_mullo_epi64(even_digit, t10r), 54);
    odd_digit = _mm512_srli_epi64(_mm512_mullo_epi64(odd_digit, t10r), 54);

    __m512i BCD = _mm512_or_epi64(even_digit, _mm512_slli_epi64(odd_digit, 8));
    BCD = _mm512_or_epi64(BCD , ZERO);
    const short idx[8] = {12, 8, 4, 0, 28, 24, 20, 16}; // 16byte
    const __m512i idxr_epi16 = _mm512_castsi128_si512(_mm_loadu_epi64(idx));



    BCD = _mm512_permutexvar_epi16(idxr_epi16, BCD); // AVX512BW
    //__m512i ASCII = _mm512_or_epi64(ZERO , BCD);
    
    _mm_storeu_si128((__m128i*)out, _mm512_castsi512_si128(BCD));
}
void to_string_avx512f_BW_v3(uint64_t n, char *out)
{
    // need AVX512F,AVX512CD,AVX512BW,AVX512BW,AVX,sse4.1

    u64 aabbccdd = n / 100000000;
    u64 eeffgghh = n + aabbccdd * (-100000000);
    __m512i hi = _mm512_set1_epi64x(aabbccdd);
    __m512i lo = _mm512_set1_epi64x(eeffgghh);
    __m256i l8_4r = _mm256_set1_epi64x(eeffgghh); // AVX
    __m512i h8_4r = _mm512_castsi256_si512(_mm256_set1_epi64x(aabbccdd));
    __m512i n8r = _mm512_inserti64x4(h8_4r, l8_4r, 1); // AVX512F
    const int BIT = 56;
    const u64 m8 = (1ull << BIT) / (100000000) + 1;
    const u64 m7 = (1ull << BIT) / (10000000) + 1;
    const u64 m6 = (1ull << BIT) / (1000000) + 1;
    const u64 m5 = (1ull << BIT) / (100000) + 1;
    const u64 m4 = (1ull << BIT) / (10000) + 1;
    const u64 m3 = (1ull << BIT) / (1000) + 1;
    const u64 m2 = (1ull << BIT) / (100) + 1;
    const u64 m1 = (1ull << BIT) / (10) + 1;
    const __m512i even = _mm512_set_epi64(m8, m6, m4, m2, m8, m6, m4, m2);
    const __m512i odd = _mm512_set_epi64(m7, m5, m3, m1, m7, m5, m3, m1);
    const __m512i multipier = _mm512_set_epi64(m8,m7,m6,m5,m4,m3,m2,m1);
    const __m512i MASK_BIT = _mm512_set1_epi64(((u64)1 << BIT) - 1);
    //const __m512i M8_8_2 = _mm512_set1_epi64(0xff00);
    const __m512i t10r = _mm512_set1_epi64(10);
    const __m512i ZERO = _mm512_set1_epi64((0x30303030ull << 32) + 0x30303030ull);


    __m512i hi_digit_low = _mm512_and_epi64(_mm512_mullo_epi64(hi, multipier),MASK_BIT);
    __m512i lo_digit_low = _mm512_and_epi64(_mm512_mullo_epi64(lo, multipier),MASK_BIT);
    __m512i even_digit = _mm512_and_epi64(_mm512_mullo_epi64(n8r, even),MASK_BIT);
    __m512i odd_digit = _mm512_and_epi64(_mm512_mullo_epi64(n8r, odd),MASK_BIT);

    // even_digit = _mm512_srli_epi64(_mm512_add_epi64(_mm512_slli_epi64(even_digit, 2), even_digit),53);
    // odd_digit = _mm512_srli_epi64(_mm512_add_epi64(_mm512_slli_epi64(odd_digit, 2), odd_digit),53);

    even_digit = _mm512_srli_epi64(_mm512_mullo_epi64(even_digit, t10r), BIT);
    odd_digit = _mm512_srli_epi64(_mm512_mullo_epi64(odd_digit, t10r), BIT);
    __512i hi_digit_high = _mm512_srli_epi64(_mm512_mullo_epi64(hi_digit_low, t10r),BIT);
    __512i lo_digit_high = _mm512_srli_epi64(_mm512_mullo_epi64(lo_digit_low, t10r),BIT);
    
    __m512i BCD = _mm512_or_epi64(even_digit, _mm512_slli_epi64(odd_digit, 8));
    BCD = _mm512_or_epi64(BCD , ZERO);
    static const short idx[8] = {12, 8, 4, 0, 28, 24, 20, 16}; // 16byte
    const __m512i idxr_epi16 = _mm512_castsi128_si512(_mm_loadu_epi64(idx));
    BCD = _mm512_permutexvar_epi16(idxr_epi16, BCD); // AVX512BW
    //__m512i ASCII = _mm512_or_epi64(ZERO , BCD);
    
    _mm_storeu_si128((__m128i*)out, _mm512_castsi512_si128(BCD));
}

#endif


void to_string_tree_bigtable(uint64_t x, char *out)
{
#include "bigtable.h"

  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;

  memcpy(out, &bigtable[4 * toptop], 4);
  memcpy(out + 4, &bigtable[4 * topbottom], 4);
  memcpy(out + 8, &bigtable[4 * bottomtop], 4);
  memcpy(out + 12, &bigtable[4 * bottombottom], 4);
}

// in x64, this function can be compile
void to_string_scalar(uint64_t v, char *out)
{
  const int one = 1;
  const int is_little_endian = *(char *)&one == 1;
  uint64_t abcdefgh = v / 100000000;
  uint64_t ijklmnop = v - abcdefgh * 100000000;
  if (is_little_endian)
  {
    uint64_t efgh_abcd = (abcdefgh << 32) + (1 - (10000ll << 32)) * ((abcdefgh * 0x68db8bb) >> 40);
    uint64_t mnop_ijkl = (ijklmnop << 32) + (1 - (10000ll << 32)) * ((ijklmnop * 0x68db8bb) >> 40);

    uint64_t gh_ef_cd_ab = (efgh_abcd << 16) + (1 - (100ll << 16)) * (((efgh_abcd * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t op_mn_kl_ij = (mnop_ijkl << 16) + (1 - (100ll << 16)) * (((mnop_ijkl * 0x147b) >> 19) & 0x7f0000007f);

    uint64_t h_g_f_e_d_c_b_a = (gh_ef_cd_ab << 8) + (1 - (10ll << 8)) * (((gh_ef_cd_ab * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;
    uint64_t p_o_n_m_l_k_j_i = (op_mn_kl_ij << 8) + (1 - (10ll << 8)) * (((op_mn_kl_ij * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;

    memcpy(out + 0, &h_g_f_e_d_c_b_a, sizeof(uint64_t));
    memcpy(out + 8, &p_o_n_m_l_k_j_i, sizeof(uint64_t));
  }
  else
  {
    uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
    uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bb) >> 40);

    uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);

    uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;
    uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;

    memcpy(out + 0, &a_b_c_d_e_f_g_h, sizeof(uint64_t));
    memcpy(out + 8, &i_j_k_l_m_n_o_p, sizeof(uint64_t));
  }
}
void to_string_scalar_v2(uint64_t v, char *out)
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
  uint64_t abcdefgh_ascii = is_little_endian ? __builtin_bswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
  uint64_t ijklmnop_ascii = is_little_endian ? __builtin_bswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;

  memcpy(out + 0, &abcdefgh_ascii, sizeof(uint64_t));
  memcpy(out + 8, &ijklmnop_ascii, sizeof(uint64_t));
}
void to_string_8digit_scalar(uint64_t v, char *out)
{
  uint64_t abcdefgh = v;
  // uint64_t ijklmnop = v - abcdefgh * 100000000;

  uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
  // uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bb) >> 40);

  uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
  // uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);

  uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;
  // uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;

  const int one = 1;
  const int is_little_endian = *(char *)&one == 1;
  uint64_t h_g_f_e_d_c_b_a = is_little_endian ? __builtin_bswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
  // uint64_t p_o_n_m_l_k_j_i = is_little_endian ? __builtin_bswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;

  memcpy(out + 0, &h_g_f_e_d_c_b_a, sizeof(uint64_t));
  // memcpy(out + 8, &p_o_n_m_l_k_j_i, sizeof(uint64_t));
}
uint64_t to_bcd8(uint64_t x)
{
  uint64_t abcdefgh = x;
  uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bb) >> 40);
  uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
  uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f) + 0x3030303030303030;
  const int one = 1;
  const int is_little_endian = *(char *)&one == 1;
  return is_little_endian ? __builtin_bswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
}

int main()
{

  std::vector<uint64_t> data;
  data.resize(N);
  for (size_t i = 0; i < N; i++)
  {
    // data.push_back(rand() % 10000000000000000);
    data[i] = rand() % 10000000000000000;
  }
  data[0]=0;
  //data[1]=10000000000000000-1;
  buf_ref = new char[data.size() * 16];
  for (size_t i = 0; i < data.size(); i++)
  {
    to_string_scalar_v2(data[i], &buf_ref[i * 16]); // use this function for reference
  }
  buf = new char[data.size() * 16]; // size  = 1MB
  auto backlinear_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_backlinear(val, b);
      b += 16;
    }
    return data.size();
  };
  auto linear_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_linear(val, b);
      b += 16;
    }
    return data.size();
  };
  auto tree_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_tree(val, b);
      b += 16;
    }
    return data.size();
  };
  auto tree_table_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_tree_table(val, b);
      b += 16;
    }
    return data.size();
  };
  auto tree_str_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_tree_str(val, b);
      b += 16;
    }
    return data.size();
  };

  auto tree_bigtable_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_tree_bigtable(val, b);
      b += 16;
    }
    return data.size();
  };
  auto khuong_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_khuong(val, b);
      b += 16;
    }
    return data.size();
  };

#ifdef __ARM_NEON
  auto neon_old_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_neon_old(val, b);
      b += 16;
    }
    return data.size();
  };
  auto neon_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_neon(val, b);
      b += 16;
    }
    return data.size();
  };
  auto neon_v2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_neon_v2(val, b);
      b += 16;
    }
    return data.size();
  };
  auto neon_v3_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_neon_v3(val, b);
      b += 16;
    }
    return data.size();
  };
  auto neon_v4_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_neon_v4(val, b);
      b += 16;
    }
    return data.size();
  };
  auto neon_v5_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_neon_v5(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

  auto scalar_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_scalar(val, b);
      b += 16;
    }
    return data.size();
  };
  auto scalar_v2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_scalar_v2(val, b);
      b += 16;
    }
    return data.size();
  };

#ifdef __SSE2__
  auto sse2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_sse2(val, b);
      b += 16;
    }
    return data.size();
  };
  auto sse2_v2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_sse2_v2(val, b);
      b += 16;
    }
    return data.size();
  };
  auto sse2_v3_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_sse2_v3(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

#ifdef __SSE4_1__
  auto sse2_pshufb_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_sse2__pshufb(val, b);
      b += 16;
    }
    return data.size();
  };
  auto sse41_v2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_sse4_1_v2(val, b);
      b += 16;
    }
    return data.size();
  };

#endif

#ifdef __AVX2__
  auto avx2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_avx2_biglut(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__)
  auto avx512ifma_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_avx512ifma_vbmi(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__)
  auto avx512f_block32_approach = [&data]() -> size_t
  {
    char *b = buf;
    size_t block_num = data.size() / 32;
    size_t rest_num = data.size() % 32;
    char *data_ptr = (char *)data.data();
    for (size_t i = 0; i < block_num; i++)
    {
      to_string_avx512f_block32((uint64_t *)(data_ptr + i * 32 * sizeof(uint64_t)), b);
      b += 16 * 32;
    }
    if (rest_num != 0)
    {
      uint64_t tmp_num[32];
      char tmp_buf[16 * 32];
      memcpy(tmp_num, data_ptr + block_num * 32, rest_num * sizeof(uint64_t));
      to_string_avx512f_block32(tmp_num, tmp_buf);
      memcpy(b, tmp_buf, rest_num * 16);
    }
    return data.size();
  };
#endif

#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__) && defined(__AVX2__)
  auto avx512f_BW_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_avx512f_BW(val, b);
      b += 16;
    }
    return data.size();
  };
  auto avx512f_BW_v2_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_avx512f_BW_v2(val, b);
      b += 16;
    }
    return data.size();
  };
  auto avx512f_BW_v3_approach = [&data]() -> size_t
  {
    char *b = buf;
    for (auto val : data)
    {
      to_string_avx512f_BW_v3(val, b);
      b += 16;
    }
    return data.size();
  };
#endif



  for (size_t i = 0; i < 3; i++)
  {
    // std::cout << "khuong     ";
    printf("%-20s", "khuong");
    std::cout << bench(khuong_approach) << std::endl;
#ifdef __ARM_NEON
    // std::cout << "neon_old   ";
    printf("%-20s", "neon_old");
    std::cout << bench(neon_old_approach) << std::endl;
    // std::cout << "neon   ";
    printf("%-20s", "neon");
    std::cout << bench(neon_approach) << std::endl;
    // std::cout << "neon_v2   ";
    printf("%-20s", "neon_v2");
    std::cout << bench(neon_v2_approach) << std::endl;
    // std::cout << "neon_v3   ";
    printf("%-20s", "neon_v3");
    std::cout << bench(neon_v3_approach) << std::endl;
    // std::cout << "neon_v4   ";
    printf("%-20s", "neon_v4");
    std::cout << bench(neon_v4_approach) << std::endl;
    // std::cout << "neon_v5   ";
    printf("%-20s", "neon_v5");
    std::cout << bench(neon_v5_approach) << std::endl;
#endif
    // std::cout << "scalar  ";
    printf("%-20s", "scalar");
    std::cout << bench(scalar_approach) << std::endl;
    // std::cout << "scalar_v2  ";
    printf("%-20s", "scalar_v2");
    std::cout << bench(scalar_v2_approach) << std::endl;
    // std::cout << "backlinear ";
    printf("%-20s", "backlinear");
    std::cout << bench(backlinear_approach) << std::endl;
    // std::cout << "linear ";
    printf("%-20s", "linear");
    std::cout << bench(linear_approach) << std::endl;
    // std::cout << "tree   ";
    printf("%-20s", "tree");
    std::cout << bench(tree_approach) << std::endl;
    // std::cout << "treetst ";
    printf("%-20s", "treetst");
    std::cout << bench(tree_str_approach) << std::endl;
    // std::cout << "treest ";
    printf("%-20s", "treest");
    std::cout << bench(tree_table_approach) << std::endl;
    // std::cout << "treebt ";
    printf("%-20s", "treebt");
    std::cout << bench(tree_bigtable_approach) << std::endl;
#ifdef __SSE2__
    // std::cout << "sse2   ";
    printf("%-20s", "sse2");
    std::cout << bench(sse2_approach) << std::endl;
    // std::cout << "sse2_v2   ";
    printf("%-20s", "sse2_v2");
    std::cout << bench(sse2_v2_approach) << std::endl;
    printf("%-20s", "sse2_v3");
    std::cout << bench(sse2_v3_approach) << std::endl;
#endif

#ifdef __SSE4_1__
    // std::cout << "sse2_pshufb   ";
    printf("%-20s", "sse2_pshufb");
    std::cout << bench(sse2_pshufb_approach) << std::endl;
    printf("%-20s", "sse4_1_v2");
    std::cout << bench(sse41_v2_approach) << std::endl;
#endif

#ifdef __AVX2__
    // std::cout << "avx2    ";
    printf("%-20s", "avx2_biglut");
    std::cout << bench(avx2_approach) << std::endl;
#endif

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__)
    // std::cout << "avx512ifma_vbmi    ";
    printf("%-20s", "avx512ifma_vbmi");
    std::cout << bench(avx512ifma_approach) << std::endl;
#endif

#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__)
    // std::cout << "avx512f_block32    ";
    printf("%-20s", "avx512f_block32");
    std::cout << bench(avx512f_block32_approach) << std::endl;
#endif

#if defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__) && defined(__AVX2__)
    // std::cout << "avx512f_BW    ";
    printf("%-20s", "avx512f_BW");
    std::cout << bench(avx512f_BW_approach) << std::endl;
    // std::cout << "avx512f_BW_v2    ";
    printf("%-20s", "avx512f_BW_v2");
    std::cout << bench(avx512f_BW_v2_approach) << std::endl;
    // std::cout << "avx512f_BW_v3    ";
    printf("%-20s", "avx512f_BW_v3");
    std::cout << bench(avx512f_BW_v3_approach) << std::endl;
#endif


    std::cout << std::endl;
  }
  delete[] buf;
  delete[] buf_ref;

  return 0;
}

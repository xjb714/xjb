// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

// This file was generated. DO NOT EDIT IT.

#include "ieee32_no_uint128.h"

#include "teju/literal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define teju_size                 32u
#define teju_exponent_min         -149
#define teju_mantissa_size        24u
#define teju_storage_index_offset -45
#define teju_calculation_div10    teju_built_in_2
#define teju_calculation_mshift   teju_built_in_2
#define teju_calculation_shift    64u

#define teju_function             teju_ieee32_no_uint128
#define teju_fields_t             teju32_fields_t
#define teju_u1_t                 teju32_u1_t

#if defined(teju32_u2_t)
  #define teju_u2_t               teju32_u2_t
#endif

#if defined(teju32_u4_t)
  #define teju_u4_t               teju32_u4_t
#endif

static struct {
  teju_u1_t const upper;
  teju_u1_t const lower;
} const multipliers[] = {
  { 0xb35dbf82, 0x1ae4f38c },
  { 0x8f7e32ce, 0x7bea5c70 },
  { 0xe596b7b0, 0xc643c71a },
  { 0xb7abc627, 0x050305ae },
  { 0x92efd1b8, 0xd0cf37bf },
  { 0xeb194f8e, 0x1ae525fe },
  { 0xbc143fa4, 0xe250eb32 },
  { 0x96769950, 0xb50d88f5 },
  { 0xf0bdc21a, 0xbb48db21 },
  { 0xc097ce7b, 0xc90715b4 },
  { 0x9a130b96, 0x3a6c115d },
  { 0xf684df56, 0xc3e01bc7 },
  { 0xc5371912, 0x364ce306 },
  { 0x9dc5ada8, 0x2b70b59e },
  { 0xfc6f7c40, 0x45812297 },
  { 0xc9f2c9cd, 0x04674edf },
  { 0xa18f07d7, 0x36b90be6 },
  { 0x813f3978, 0xf8940985 },
  { 0xcecb8f27, 0xf4200f3b },
  { 0xa56fa5b9, 0x9019a5c9 },
  { 0x84595161, 0x401484a1 },
  { 0xd3c21bce, 0xcceda101 },
  { 0xa968163f, 0x0a57b401 },
  { 0x87867832, 0x6eac9001 },
  { 0xd8d726b7, 0x177a8001 },
  { 0xad78ebc5, 0xac620001 },
  { 0x8ac72304, 0x89e80001 },
  { 0xde0b6b3a, 0x76400001 },
  { 0xb1a2bc2e, 0xc5000001 },
  { 0x8e1bc9bf, 0x04000001 },
  { 0xe35fa931, 0xa0000001 },
  { 0xb5e620f4, 0x80000001 },
  { 0x9184e72a, 0x00000001 },
  { 0xe8d4a510, 0x00000001 },
  { 0xba43b740, 0x00000001 },
  { 0x9502f900, 0x00000001 },
  { 0xee6b2800, 0x00000001 },
  { 0xbebc2000, 0x00000001 },
  { 0x98968000, 0x00000001 },
  { 0xf4240000, 0x00000001 },
  { 0xc3500000, 0x00000001 },
  { 0x9c400000, 0x00000001 },
  { 0xfa000000, 0x00000001 },
  { 0xc8000000, 0x00000001 },
  { 0xa0000000, 0x00000001 },
  { 0x80000000, 0x00000001 },
  { 0xcccccccc, 0xcccccccd },
  { 0xa3d70a3d, 0x70a3d70b },
  { 0x83126e97, 0x8d4fdf3c },
  { 0xd1b71758, 0xe219652c },
  { 0xa7c5ac47, 0x1b478424 },
  { 0x8637bd05, 0xaf6c69b6 },
  { 0xd6bf94d5, 0xe57a42bd },
  { 0xabcc7711, 0x8461cefd },
  { 0x89705f41, 0x36b4a598 },
  { 0xdbe6fece, 0xbdedd5bf },
  { 0xafebff0b, 0xcb24aaff },
  { 0x8cbccc09, 0x6f5088cc },
  { 0xe12e1342, 0x4bb40e14 },
  { 0xb424dc35, 0x095cd810 },
  { 0x901d7cf7, 0x3ab0acda },
  { 0xe69594be, 0xc44de15c },
  { 0xb877aa32, 0x36a4b44a },
  { 0x9392ee8e, 0x921d5d08 },
  { 0xec1e4a7d, 0xb69561a6 },
  { 0xbce50864, 0x92111aeb },
  { 0x971da050, 0x74da7bef },
  { 0xf1c90080, 0xbaf72cb2 },
  { 0xc16d9a00, 0x95928a28 },
  { 0x9abe14cd, 0x44753b53 },
  { 0xf79687ae, 0xd3eec552 },
  { 0xc6120625, 0x76589ddb },
  { 0x9e74d1b7, 0x91e07e49 },
  { 0xfd87b5f2, 0x8300ca0e },
  { 0xcad2f7f5, 0x359a3b3f },
  { 0xa2425ff7, 0x5e14fc32 },
  { 0x81ceb32c, 0x4b43fcf5 },
};

#define teju_calculation_sorted 0u

static struct {
  teju_u1_t const multiplier;
  teju_u1_t const bound;
} const minverse[] = {
  { 0x00000001, 0xffffffff },
  { 0xcccccccd, 0x33333333 },
  { 0xc28f5c29, 0x0a3d70a3 },
  { 0x26e978d5, 0x020c49ba },
  { 0x3afb7e91, 0x0068db8b },
  { 0x0bcbe61d, 0x0014f8b5 },
  { 0x68c26139, 0x000431bd },
  { 0xae8d46a5, 0x0000d6bf },
  { 0x22e90e21, 0x00002af3 },
  { 0x3a2e9c6d, 0x00000897 },
  { 0x3ed61f49, 0x000001b7 },
  { 0x0c913975, 0x00000057 },
  { 0xcf503eb1, 0x00000011 },
  { 0xf6433fbd, 0x00000003 },
};

#include "teju/teju.h"

#ifdef __cplusplus
}
#endif

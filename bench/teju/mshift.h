// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/mshift.h
 *
 * Multiply-and-shift operations.
 */

#ifndef TEJU_TEJU_MSHIFT_H_
#define TEJU_TEJU_MSHIFT_H_

#if !defined(teju_calculation_mshift) || !defined(teju_calculation_shift) || \
  !defined(teju_u1_t) || !defined(teju_size)
  #error "Macros teju_calculation_mshift, teju_calculation_shift, teju_size and teju_u1_t must be defined prior to inclusion of mshift.h."
#endif

#include "common.h"
#include "config.h"

#if defined(_MSC_VER) && !defined(__clang__)
  #include <intrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Returns x + y and sets a carry flag if the addition has wrapped up.
 *
 * @param  x                The value of x.
 * @param  y                The value of y.
 * @param  c                The address of the carry flag to be set.
 *
 * @returns The sum x + y.
 */
static inline
teju_u1_t
teju_add_and_carry(teju_u1_t x, teju_u1_t y, teju_u1_t* c) {

  #if defined(_MSC_VER) && !defined(__clang__)

    #if teju_size == 16u
      *c = _addcarry_u16(0, x, y, &x);
    #elif teju_size == 32u
      *c = _addcarry_u32(0, x, y, &x);
    #elif teju_size == 64u
      *c = _addcarry_u64(0, x, y, &x);
    #else
      #error "Size not supported by msvc."
    #endif

  #else

    x  += y;
    *c  = x < y;

  #endif

  return x;
}

/**
 * @brief Returns the quotient q = (r2 * 2^(2 * N) + r1 * 2^N) / 2^s, where
 *        N = teju_size and s = teju_calculation_shift.
 *
 * @param  r2               The value of r2.
 * @param  r1               The value of r1.
 *
 * @returns The quotient q.
 */
static inline
teju_u1_t
teju_rshift(teju_u1_t const r2, teju_u1_t const r1) {

  #if teju_calculation_shift >= 2u * teju_size

    return r2 >> (teju_calculation_shift - 2u * teju_size);

  #else

    return r2 << (2u * teju_size - teju_calculation_shift) |
      r1 >> (teju_calculation_shift - teju_size);

  #endif
}

/**
 * @brief Returns the quotient q = ((u * 2^N + l) * m) / 2^s, where
 *        N = teju_size and s = teju_calculation_shift.
 *
 * @param  m                The 1st multiplicand m.
 * @param  u                The 2nd multiplicand upper half u.
 * @param  l                The 2nd multiplicand lower half l.
 *
 * @returns The quotient q.
 */
static inline
teju_u1_t
teju_mshift(teju_u1_t const m, teju_u1_t const u, teju_u1_t const l) {

  // Let x := 2^N.

  #if teju_calculation_mshift == teju_built_in_4

    teju_u4_t const n = (((teju_u2_t) u) << teju_size) | l;
    return 1u * n * m >> teju_calculation_shift;

  #elif teju_calculation_mshift == teju_synthetic_2

    // (u * x + l) * m = r2 * x^2 + r1 * x + r0,
    //                   with r2, r1, r0 in [0, x[.

    teju_u2_t const n = (((teju_u2_t) u) << teju_size) | l;
    teju_u2_t r2;
    teju_u2_t const r1 = teju_multiply(n, m, &r2) >> teju_size;
    return teju_rshift(r2, r1);

  #elif teju_calculation_mshift == teju_built_in_2

    // (u * x + l) * m = s1 * x + s0,
    //                       with s1 := u * m, s0 := l * m in [0, x^2[,
    //                 = s1 * x + (s01 * x + s00)
    //                       with s01 := s0 / x, s00 := s0 % x in [0, x[,
    //                 = (s1 + s01) * x + s00.

    teju_u2_t const s0 = 1u * ((teju_u2_t) l) * m;
    teju_u2_t const s1 = 1u * ((teju_u2_t) u) * m;
    return (s1 + (s0 >> teju_size)) >> (teju_calculation_shift - teju_size);

  #elif teju_calculation_mshift == teju_synthetic_1

    // (u * x + l) * m = s1 * x + s0,
    //                       with s1 := u * m, s0 := l * m in [0, x^2[,
    //                 = (s11 * x + s10) * x + (s01 * x + s00),
    //                       with s11 := s1 / x, s10 := s1 % x,
    //                            s01 := s0 / x, s00 := s0 % x in [0, x[,
    //                 = s11 * x^2 +(s10 + s01) * x + s00

    teju_u1_t s01, s11, c;
    (void) teju_multiply(l, m, &s01);
    teju_u1_t const s10 = teju_multiply(u, m, &s11);
    teju_u1_t const r0  = teju_add_and_carry(s01, s10, &c);
    teju_u1_t const r1  = s11 + c;
    return teju_rshift(r1, r0);

  #elif teju_calculation_mshift == teju_built_in_1

    // Let y := 2^(N / 2), so that, x = y^2. Then:
    // u := (n3 * y + n2) with n3 := u / y, n2 = u % y in [0, y[,
    // l := (n1 * y + n0) with n1 := l / y, n0 = l % y in [0, y[,
    // m := (m1 * y + m0) with m1 := m / y, m0 = m % y in [0, y[.

    teju_u1_t const y  = teju_pow2(teju_u1_t, teju_size / 2u);
    teju_u1_t const n3 = u / y;
    teju_u1_t const n2 = u % y;
    teju_u1_t const n1 = l / y;
    teju_u1_t const n0 = l % y;
    teju_u1_t const m1 = m / y;
    teju_u1_t const m0 = m % y;

    // result, carry, temporary:
    teju_u1_t r1, r0, c, t;

    // (u * x + l) * m
    //     = ((n3 * y + n2) * y^2 + (n1 * y + n0)) * (m1 * y + m0),
    //     = (n3 * y^3 + n2 * y^2 + n1 * y + n0) * (m1 * y + m0),
    //     = (n3 * m1) * y^4 + (n3 * m0 + n2 * m1) * y^3 +
    //       (n2 * m0 + n1 * m1) * y^2 + (n1 * m0 + n0 * m1) * y +
    //       (n0 * m0)

    // order 0:
    t   = 1u * n0 * m0;
    r1  = t / y;

    // order 1:
    r1 += 1u * n0 * m1; // This addition doesn't wraparound.
    t   = 1u * n1 * m0;
    r1  = teju_add_and_carry(r1, t, &c);
    r1 /= y;

    // order 2:
    r1 += 1u * n1 * m1 + 1u * c * y; // This addition doesn't wraparound.
    t   = 1u * n2 * m0;
    r1  = teju_add_and_carry(r1, t, &c);
    r1 /= y;

    // order 3:
    r1 += 1u * n2 * m1 + 1u * c * y; // This addition doesn't wraparound.
    t   = 1u * n3 * m0;
    r1  = teju_add_and_carry(r1, t, &c);
    r0  = r1 % y;
    r1 /= y;

    // order 4:
    r1 += 1u * n3 * m1 + 1u * c * y;

    #if teju_calculation_shift >= 2u * teju_size
      (void) r0;
      return r1 >> (teju_calculation_shift - 2u * teju_size);
    #elif teju_calculation_shift >= 3u * teju_size / 2u
      return (r1 << (2u * teju_size - teju_calculation_shift)) |
        (r0 >> (teju_calculation_shift - 3u * teju_size / 2u));
    #else
      #error "Unsupported combination of size, shift and mshift calculation."
    #endif

  #else

    #error "Invalid definition of macro teju_calculation_mshift."

  #endif
}

/**
 * @brief Returns the quotient q = ((u * 2^N + l) * 2^k) / 2^s, where
 *        N = teju_size_size and s = teju_calculation_shift.
 *
 * @param  k                The exponent k.
 * @param  u                The upper part of the multiplicand u.
 * @param  l                The lower part of the multiplicand l.
 *
 * @returns The value of q.
 */
static inline
teju_u1_t
mshift_pow2(uint32_t const k, teju_u1_t const u, teju_u1_t const l) {
  int32_t const s = k - (teju_calculation_shift - teju_size);
  if (s <= 0)
    return u >> -s;
  return (u << s) | (l >> (teju_size - s));
}

#ifdef __cplusplus
}
#endif

#endif // TEJU_TEJU_MSHIFT_H_

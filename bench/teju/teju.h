// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2024 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/teju.h
 *
 * The implementation of Tejú Jaguá and some of its helpers.
 */

#ifndef TEJU_TEJU_TEJU_H_
#define TEJU_TEJU_TEJU_H_

#include "common.h"
#include "config.h"
#include "div10.h"
#include "mshift.h"

#if defined(_MSC_VER)
  #include <intrin.h>
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Helper functions.
//------------------------------------------------------------------------------

/**
 * @brief Checks whether mantissa m is multiple of 2^e.
 *
 * @param  e                The exponent e.
 * @param  m                The mantissa m.
 *
 * @pre 0 <= e && e < teju_size.
 *
 * @returns true if m is multiple of 2^e and false, otherwise.
 */
static inline
bool
is_multiple_of_pow2(int32_t const e, teju_u1_t const m) {
  assert(0 <= e && (uint32_t) e < teju_size);
  return (m >> e) << e == m;
}

/**
 * @brief Checks whether n is a "small" multiple of 5^f.
 *
 * @param  f                The exponent f.
 * @param  n                The number n.
 *
 * @pre 0 <= f && f < sizeof(minverse) / sizeof(minverse[0]).
 *
 * @returns true if n is multiple of 5^f and false, otherwise.
 */
 static inline
 bool
 is_multiple_of_pow5(int32_t const f, teju_u1_t const n) {
   assert(0 <= f && (uint32_t) f < sizeof(minverse) / sizeof(minverse[0]));
   return (teju_u1_t) (1u * n * minverse[f].multiplier) <= minverse[f].bound;
 }

 /**
 * @brief Rotates the bits of a given number 1 position to the right.
 *
 * @param  m                The given number.
 *
 * @returns The value of m after the rotation.
 */
 static inline
 teju_u1_t ror(teju_u1_t m) {
   return m << (teju_size - 1u) | m >> 1u;
 }

//------------------------------------------------------------------------------
// Tejú Jaguá
//------------------------------------------------------------------------------

/**
 * @brief Checks whether the number m * 2^e is a "small" integer.
 *
 * @param  e                The exponent e.
 * @param  m                The mantissa m.
 *
 * @returns true if m * 2^e is a "small" integer and false, otherwise.
 */
 static inline
 bool
 is_small_integer(int32_t const e, teju_u1_t const m) {
   return 0 <= -e && (uint32_t) -e < teju_mantissa_size &&
     is_multiple_of_pow2(-e, m);
 }

 /**
 * @brief Checks whether m, for m in { m_a, m_b, c_2 }, yields a tie.
 *
 * @param  f                The exponent f (for m == m_a and m == m_b) or
 *                          its negation -f for (m == c_2).
 * @param  m                The number m.
 *
 * @returns true if m yields a tie and false, otherwise.
 */
static inline
bool
is_tie(int32_t const f, teju_u1_t const m) {
  return 0 <= f && (uint32_t) f < sizeof(minverse) / sizeof(minverse[0]) &&
    is_multiple_of_pow5(f, m);
}

/**
 * @brief Checks whether m_a for the uncentred value yields a tie.
 *
 * @param  f                The exponent f.
 * @param  m_a              The number m_a.
 *
 * @returns true if m_a yields a tie and false, otherwise.
 */
static inline
bool
is_tie_uncentred(int32_t const f, teju_u1_t const m_a) {
  return m_a % 5u == 0 && 0 <= f && is_multiple_of_pow5(f, m_a);
}

/**
 * @brief Checks whether mantissa m wins the tiebreak against its neighbour.
 *
 * Implements the ties-to-even rule, i.e., m wins the tiebreak if it's even.
 * Contrarily to other tie-breaking rules, this one doesn't depend on the
 * neighbour that m is compeating against or the sign of the floating-point
 * number.
 *
 * @param  m                The mantissa m.
 *
 * @returns true if m wins the tiebreak and false, otherwise.
 */
static inline
bool
wins_tiebreak(teju_u1_t const m) {
  return m % 2u == 0;
}

/**
 * @brief Shortens the decimal representation of m * 10^e by removing trailing
 *        zeros from m and increasing e accordingly.
 *
 * @param  e                The exponent e.
 * @param  m                The mantissa m.
 *
 * @returns The fields of the shortest close decimal representation.
 */
static inline
teju_fields_t
remove_trailing_zeros(int32_t e, teju_u1_t m) {
  teju_u1_t const minv5 = -(((teju_u1_t) -1) / 5u);
  teju_u1_t const bound = ((teju_u1_t) -1) / 10u + 1u;
  while (true) {
    teju_u1_t const q = ror((teju_u1_t) (1u * m * minv5));
    if (q >= bound)
      return (teju_fields_t){e, m};
    ++e;
    m = q;
  }
}

/**
 * @brief Finds the shortest decimal representation of m * 2^e.
 *
 * This is Tejú Jaguá itself.
 *
 * @param  binary           The fields of the binary representation.
 *
 * @returns The fields of the shortest decimal representation.
 */
teju_fields_t
teju_function(teju_fields_t const binary) {

  int32_t   const e = binary.exponent;
  teju_u1_t const m = binary.mantissa;

  if (is_small_integer(e, m))
    return remove_trailing_zeros(0, 1u * m >> -e);

  int32_t   const f   = teju_log10_pow2(e);
  uint32_t  const r   = teju_log10_pow2_residual(e);
  uint32_t  const i   = f - teju_storage_index_offset;
  teju_u1_t const u   = multipliers[i].upper;
  teju_u1_t const l   = multipliers[i].lower;

  teju_u1_t const m_0 = teju_pow2(teju_u1_t, teju_mantissa_size - 1u);
  if (m != m_0 || e == teju_exponent_min) {

    teju_u1_t const m_a = (2u * m - 1u) << r;
    teju_u1_t const a   = teju_mshift(m_a, u, l);
    teju_u1_t const m_b = (2u * m + 1u) << r;
    teju_u1_t const b   = teju_mshift(m_b, u, l);
    teju_u1_t const q   = teju_div10(b);
    teju_u1_t const s   = 10u * q;

    if (a < s) {
      if (s < b || wins_tiebreak(m) || !is_tie(f, m_b))
        return remove_trailing_zeros(f + 1, q);
    }
    else if (s == a && wins_tiebreak(m) && is_tie(f, m_a))
      return remove_trailing_zeros(f + 1, q);

    if ((a + b) % 2u == 1u)
      return (teju_fields_t){f, (a + b) / 2u + 1u};

    teju_u1_t const m_c = 4u * m << r;
    teju_u1_t const c_2 = teju_mshift(m_c, u, l);
    teju_u1_t const c   = c_2 / 2u;

    if (c_2 % 2u == 0 || (wins_tiebreak(c) && is_tie(-f, c_2)))
      return (teju_fields_t){f, c};

    return (teju_fields_t){f, c + 1u};
  }

  teju_u1_t const m_a = (4u * m_0 - 1u) << r;
  teju_u1_t const a   = teju_mshift(m_a, u, l) / 2u;
  teju_u1_t const m_b = (2u * m_0 + 1u) << r;
  teju_u1_t const b   = teju_mshift(m_b, u, l);

  if (teju_calculation_sorted || a < b) {

    teju_u1_t const q = teju_div10(b);
    teju_u1_t const s = 10u * q;

    if (a < s) {
      if (s < b || wins_tiebreak(m_0) || !is_tie_uncentred(f, m_b))
        return remove_trailing_zeros(f + 1, q);
    }
    else if (s == a && wins_tiebreak(m_0) && is_tie_uncentred(f, m_a))
      return remove_trailing_zeros(f + 1, q);

    // m_c = 4 * m_0 * 2^r = 2^{teju_mantissa_size + r + 1}
    // c_2 = teju_mshift(m_c, upper, lower);
    uint32_t  const log2_m_c = teju_mantissa_size + r + 1u;
    teju_u1_t const c_2      = mshift_pow2(log2_m_c, u, l);
    teju_u1_t const c        = c_2 / 2u;

    if (c == a && !is_tie_uncentred(f, m_a))
      return (teju_fields_t){f, c + 1u};

    if (c_2 % 2u == 0 || (wins_tiebreak(c) && is_tie(-f, c_2)))
      return (teju_fields_t){f, c};

    return (teju_fields_t){f, c + 1u};
  }

  else if (is_tie_uncentred(f, m_a))
    return remove_trailing_zeros(f, a);

  teju_u1_t const m_c = 40u * m_0 << r;
  teju_u1_t const c_2 = teju_mshift(m_c, u, l);
  teju_u1_t const c   = c_2 / 2u;

  if (c_2 % 2u == 0 || (wins_tiebreak(c) && is_tie(-f, c_2)))
    return (teju_fields_t){f - 1 , c};

  return (teju_fields_t){f - 1, c + 1u};
}

#ifdef __cplusplus
}
#endif

#endif // TEJU_TEJU_TEJU_H_

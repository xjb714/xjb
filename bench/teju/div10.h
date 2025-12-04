// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/div10.h
 *
 * Different algorithms for division by 10.
 */

#ifndef TEJU_TEJU_DIV10_H_
#define TEJU_TEJU_DIV10_H_

#if !defined(teju_u1_t) || !defined(teju_calculation_shift) || \
  !defined(teju_size)
  #error "Macros teju_size and teju_u1_t must be defined prior to inclusion of div10.h."
#endif

#include "config.h"
#include "common.h"

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the quotient n / 10.
 *
 * @param  n                The dividend.
 *
 * @returns n / 10.
 */
static inline
teju_u1_t
teju_div10(teju_u1_t const n) {

  #if !defined(teju_calculation_div10) || \
    teju_calculation_div10 == teju_built_in_1

    return n / 10u;

  #elif teju_calculation_div10 == teju_built_in_2 || \
    teju_calculation_div10 == teju_synthetic_1

    // This is the algorithm in Theorem 4 of Neri C, Schneider L. "Euclidean
    // affine functions and their application to calendar algorithms." Softw
    // Pract Exper. 2023; 53(4):937-970.
    // https://onlinelibrary.wiley.com/doi/full/10.1002/spe.3172

    // Since 2^k % 10 != 0 we have 2^k / 10 = (2^k - 1) / 10, in particular,
    //   2^teju_size / 10 = (2^teju_size - 1) / 10u = ((teju_u1_t) -1) / 10u.
    teju_u1_t const a = ((teju_u1_t) -1) / 10u + 1u;

    #if teju_calculation_div10 == teju_built_in_2
      return (1u * a * ((teju_u2_t) n)) >> teju_size;
    #else // teju_calculation_div10 == teju_synthetic_1
      teju_u1_t upper;
      (void) teju_multiply(inv10, n, &upper);
      return upper;
    #endif

  #else

    #error "Invalid definition of macro teju_calculation_div10."

  #endif

}

#ifdef __cplusplus
}
#endif

#endif // TEJU_TEJU_DIV10_H_

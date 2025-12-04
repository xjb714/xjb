// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/config.h
 *
 * Platform configurations, notably, multiplication capabilities.
 */

#ifndef TEJU_TEJU_CONFIG_H_
#define TEJU_TEJU_CONFIG_H_

#include <stdint.h>

#if defined(_MSC_VER)
  #include <intrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//add for compile : by xjb 2025.5.13
#define teju_has_uint128

#if defined(teju_has_uint128)
  typedef __uint128_t uint128_t;
#else
  // Cannot support float128_t if uint128_t is not defined.
  #undef teju_has_float128
#endif

#if defined(teju_has_float16)
  typedef _Float16 float16_t;
#endif

#if defined(teju_has_float128)
  typedef __float128 float128_t;
#endif

//------------------------------------------------------------------------------
// Flags indicating the platform's multiplication capability.
//------------------------------------------------------------------------------

// The size in bits of the limb is teju_size. For instance, if teju_size == 32,
// then 1-, 2- and 4-limb integers have sizes 32, 64 and 128, respectively.
// Similarly, if teju_size == 64, then 1-, 2- and 4-limb integers have sizes 64,
// 128 and 256 respectively. The platform is required to support 1-limb integers
// but not necessarily 2- and 4-limb integers.

// Macros teju_calculation_div10 and teju_calculation_mshift define the
// algorithm used in teju_div10 and teju_mshift, respectively. They are set to
// one of the values below depending on the platform's capability as explained
// in their documentation.

/**
 * @brief The platform provides operator * for 1-limb unsigned integers.
 *
 * The platform provides operator * for 1-limb unsigned integers that yields the
 * lower 1-limb of the 2-limb product. For instance, assuming 1-limb is 32-bits:
 * @code{.cpp}
 *     uint32_t a, b;
 *     uint32_t lower = a * b;
 * @endcode
 */
#define teju_built_in_1 1u

/**
 * @brief The platform synthesises multiplication of 2-limb unsigned integers.
 *
 * The platform implements teju_multiply() which takes two 1-limb unsigned
 * integers and returns the lower 1-limb of the 2-limb product. It also takes a
 * third argument of pointer type where the upper 1-limb of the product is
 * stored on exit. For instance, assuming 1-limb is 32-bits:
 * @code{.cpp}
 *     uint32_t a, b, upper;
 *     uint32_t lower = teju_multiply(a, b, &upper);
 * @endcode
 */
#define teju_synthetic_1 2u

/**
 * @brief The platform provides operator * for 2-limb unsigned integers.
 *
 * The platform provides operator * for 2-limb unsigned integers that yields the
 * lower 2-limb of the 4-limb product. For instance, assuming 1-limb is 32-bits:
 * @code{.cpp}
 *     uint64_t a, b;
 *     uint64_t lower = a * b;
 * @endcode
 */
#define teju_built_in_2  3u

/**
 * @brief The platform synthesises multiplication of 4-limb unsigned integers.
 *
 * The platform implements teju_multiply() which takes two 2-limb unsigned
 * integers and returns the lower 2-limb of the 4-limb product. It also takes a
 * third argument of pointer type where the upper 2-limb of the product is
 * stored on exit. For instance, assuming 1-limb is 32-bits:
 * @code{.cpp}
 *     uint64_t a, b, upper;
 *     uint64_t lower = teju_multiply(a, b, &upper);
 * @endcode
 */
#define teju_synthetic_2 4u

/**
 * @brief The platform provides operator * for 4-limb unsigned integers.
 *
 * The platform provides operator * for 4-limb unsigned integers that yields the
 * lower 4-limb of the 8-limb product. For instance, assuming 1-limb is 32-bits,
 * gcc and clang support the following:
 * @code{.cpp}
 *     uint128_t a, b;
 *     uint128_t lower = a * b;
 * @endcode
 */
#define teju_built_in_4  5u

//------------------------------------------------------------------------------
// Limbs
//------------------------------------------------------------------------------

// Macros teju<X>_u1_t, where <X> = teju_size (e.g., teju32_u1_t for
// teju_size = 32), is set to the type of 1-limb unsigned integers, i.e., that
// whose size is teju_size. When defined, teju<X>_u2_t and teju<X>_u4_t are,
// respectively, set to the types of the 2- and 4-limb unsigned integers.

//----------//
//  16 bits //
//----------//

#define teju16_u1_t uint16_t
#define teju16_u2_t uint32_t
#define teju16_u4_t uint64_t

typedef struct {
  int32_t     exponent;
  teju16_u1_t mantissa;
} teju16_fields_t;

//----------//
//  32 bits //
//----------//

#define teju32_u1_t uint32_t
#define teju32_u2_t uint64_t

#if defined(teju_has_uint128)

  #define teju32_u4_t          uint128_t
  #define teju32_multiply_type teju_built_in_4

#elif defined(_MSC_VER)

  #define teju32_multiply_type teju_synthetic_2

#endif

typedef struct {
  int32_t     exponent;
  teju32_u1_t mantissa;
} teju32_fields_t;

//----------//
//  64 bits //
//----------//

#define teju64_u1_t uint64_t

#if defined(teju_has_uint128)

  #define teju64_u2_t          uint128_t
  #define teju64_multiply_type teju_built_in_2

#elif defined(_MSC_VER)

  #define teju64_multiply_type teju_synthetic_1

#endif

typedef struct {
  int32_t     exponent;
  teju64_u1_t mantissa;
} teju64_fields_t;

//----------//
// 128 bits //
//----------//

#if defined(teju_has_float128)

  #define teju128_u1_t          uint128_t
  #define teju128_multiply_type teju_built_in_1

  typedef struct {
    int32_t      exponent;
    teju128_u1_t mantissa;
  } teju128_fields_t;

#endif

//------------------------------------------------------------------------------
// teju_multiply
//------------------------------------------------------------------------------

// One might want to disabled the provided implementations of teju_multiply to
// be able to implement their own (e.g., for testing). For this, it suffices to
// define macro teju_do_not_define_teju_multiply prior to including this file.

#if defined(_MSC_VER) && !defined(teju_do_not_define_teju_multiply)

  /**
   * @brief Calculates the 128-bits product of two 64-bits unsigned numbers.
   *
   * @param  a              The 1st multiplicand.
   * @param  b              The 2nd multiplicand.
   * @param  upper          On exit the value of the highest 64-bits of the
   *                        product.
   *
   * @returns The lower 64-bits value of the product.
   */
  inline static
  uint64_t
  teju_multiply(uint64_t const a, uint64_t const b, uint64_t* upper) {
    return _umul128(a, b, upper);
  }

#endif // defined(_MSC_VER)

#ifdef __cplusplus
}
#endif

#endif // TEJU_TEJU_CONFIG_H_

// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/double.h
 *
 * Tejú Jaguá and helpers for double values.
 */

#ifndef TEJU_TEJU_DOUBLE_H_
#define TEJU_TEJU_DOUBLE_H_

#if defined(teju_has_uint128)
  #include "generated/ieee64_with_uint128.h"
#else
  #include "generated/ieee64_no_uint128.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the binary representation of a given value.
 *
 * @param  value             The given value.
 *
 * @pre isfinite(value) && value > 0.
 *
 * @returns The binary representation of the given value.
 */
teju64_fields_t
teju_double_to_binary(double value);

/**
 * @brief Gets the decimal representation of a given value.
 *
 * @param  value            The given value.
 *
 * @pre isfinite(value) && value > 0.
 *
 * @returns The decimal representation of the given value.
 */
teju64_fields_t
teju_double_to_decimal(double value);

#ifdef __cplusplus
}
#endif

#endif // TEJU_TEJU_DOUBLE_H_

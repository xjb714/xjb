// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/float128.h
 *
 * Tejú Jaguá and helpers for float128_t values.
 */

#ifndef TEJU_TEJU_FLOAT128_H_
#define TEJU_TEJU_FLOAT128_H_

#include "config.h"

#if defined(teju_has_float128)

#include "generated/ieee128.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the binary representation of a given value.
 *
 * @param  value            The given value.
 *
 * @pre isfinite(value) && value > 0.
 *
 * @returns The binary representation of the given value.
 */
teju128_fields_t
teju_float128_to_binary(float128_t value);

/**
 * @brief Gets the decimal representation of a given value.
 *
 * @param  value            The given value.
 *
 * @pre isfinite(value) && value > 0.
 *
 * @returns The decimal representation of the given value.
 */
teju128_fields_t
teju_float128_to_decimal(float128_t value);

#ifdef __cplusplus
}
#endif

#endif // defined(teju_has_float128)
#endif // TEJU_TEJU_FLOAT128_H_

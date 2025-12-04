// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

/**
 * @file teju/float16.h
 *
 * Tejú Jaguá and helpers for float16_t values.
 */

#ifndef TEJU_TEJU_FLOAT16_H_
#define TEJU_TEJU_FLOAT16_H_

#include "config.h"

#if defined(teju_has_float16)

#include "generated/ieee16.h"

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
teju32_fields_t
teju_float16_to_binary(float16_t value);

/**
 * @brief Gets the decimal representation of a given value.
 *
 * @param  value            The given value.
 *
 * @pre isfinite(value) && value > 0.
 *
 * @returns The decimal representation of the given value.
 */
teju32_fields_t
teju_float16_to_decimal(float16_t value);

#ifdef __cplusplus
}
#endif

#endif // defined(teju_has_float16)
#endif // TEJU_TEJU_FLOAT16_H_

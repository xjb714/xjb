// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

#include "config.h"

#if defined(teju_has_float128)

#include "common.h"
#include "float128.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

teju128_fields_t
teju_float128_to_binary(float128_t const value) {

  assert(isfinite(value) && value > 0 && "Invalid float128_t value.");

  typedef teju128_fields_t teju_fields_t;
  typedef teju128_u1_t     teju_u1_t;

  uint32_t const mantissa_size =    113u;
  int32_t  const exponent_min  = -16494;

  teju_u1_t bits;
  memcpy(&bits, &value, sizeof(value));

  teju_u1_t mantissa = teju_lsb(teju_u1_t, bits, mantissa_size - 1u);
  bits >>= (mantissa_size - 1u);

  int32_t exponent = (int32_t) bits;

  if (exponent != 0) {
    exponent -= 1;
    mantissa |= teju_pow2(teju_u1_t, mantissa_size - 1u);
  }

  exponent += exponent_min;
  teju128_fields_t binary = {exponent, mantissa};
  return binary;
}

teju128_fields_t
teju_float128_to_decimal(float128_t const value) {
  teju128_fields_t binary = teju_float128_to_binary(value);
  return teju_ieee128(binary);
}

#ifdef __cplusplus
}
#endif

# endif // defined(teju_has_float128)

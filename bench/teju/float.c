// SPDX-License-Identifier: APACHE-2.0
// SPDX-FileCopyrightText: 2021-2025 Cassio Neri <cassio.neri@gmail.com>

#include "common.h"
#include "float.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

teju32_fields_t
teju_float_to_binary(float const value) {

  assert(isfinite(value) && value > 0 && "Invalid float value.");

  typedef teju32_fields_t teju_fields_t;
  typedef teju32_u1_t     teju_u1_t;

  uint32_t const mantissa_size =  24u;
  int32_t  const exponent_min  = -149;

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
  teju32_fields_t binary = {exponent, mantissa};
  return binary;
}

teju32_fields_t
teju_float_to_decimal(float const value) {
  teju32_fields_t binary = teju_float_to_binary(value);
  #if defined(teju_has_uint128)
    return teju_ieee32_with_uint128(binary);
  #else
    return teju_ieee32_no_uint128(binary);
  #endif
}

#ifdef __cplusplus
}
#endif

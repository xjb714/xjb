// A double-to-string conversion library: https://github.com/vitaut/zmij/
//
// Copyright (c) 2025 - present, Victor Zverovich
// Distributed under the MIT license (see LICENSE) or alternatively
// the Boost Software License, Version 1.0.

#ifndef ZMIJ_H_
#define ZMIJ_H_

#include <stddef.h>  // size_t
#include <string.h>  // memcpy

namespace zmij {
namespace detail {
template <typename Float>
auto write(Float value, char* buffer) noexcept -> char*;

template <typename Float>
auto write_scientific(Float value, int precision, char* buffer) noexcept
    -> char*;

template <typename Float>
auto write_general(Float value, int precision, char* buffer) noexcept -> char*;

/// Clamps `precision` to the supported range [1, 18].
inline auto clamp_precision(int precision) noexcept -> int {
  if (precision < 1) return 1;
  if (precision > 18) return 18;
  return precision;
}
}  // namespace detail

enum {
  non_finite_exp = int(~0u >> 1),
};

// A decimal floating-point number sig * pow(10, exp).
// If exp is non_finite_exp then the number is a NaN or an infinity.
struct dec_fp {
  long long sig;  // significand
  int exp;        // exponent
  bool negative;
};

/// Converts `value` into the shortest correctly rounded decimal representation.
/// Usage:
///   auto [sig, exp, negative] = to_decimal(6.62607015e-34);
auto to_decimal(double value) noexcept -> dec_fp;

enum {
  float_buffer_size = 17,             // write
  double_buffer_size = 34,            // write
  float_precision_buffer_size = 24,   // write_scientific, write_general
  double_precision_buffer_size = 25,  // write_scientific, write_general
};

/// Writes the shortest correctly rounded decimal representation of `value` to
/// `out` without a null terminator. Returns a pointer past the last character
/// written; if the representation exceeds `n` characters, only the first `n`
/// are written.
inline auto write(char* out, size_t n, float value) noexcept -> char* {
  if (n >= float_buffer_size) return detail::write(value, out);
  char buffer[float_buffer_size];
  size_t size = detail::write(value, buffer) - buffer;
  if (size > n) size = n;
  memcpy(out, buffer, size);
  return out + size;
}

/// Writes the shortest correctly rounded decimal representation of `value` to
/// `out` without a null terminator. Returns a pointer past the last character
/// written; if the representation exceeds `n` characters, only the first `n`
/// are written.
inline auto write(char* out, size_t n, double value) noexcept -> char* {
  if (n >= double_buffer_size) return detail::write(value, out);
  char buffer[double_buffer_size];
  size_t size = detail::write(value, buffer) - buffer;
  if (size > n) size = n;
  memcpy(out, buffer, size);
  return out + size;
}

/// Writes `value` in scientific format with exactly `precision` significant
/// digits (e.g. 1.234e+05) to `out` without a null terminator. Returns a
/// pointer past the last character written; if the representation exceeds `n`
/// characters, only the first `n` are written. `precision` must be in [1, 18];
/// out-of-range values are clamped.
inline auto write_scientific(char* out, size_t n, float value,
                             int precision) noexcept -> char* {
  precision = detail::clamp_precision(precision);
  if (n >= float_precision_buffer_size)
    return detail::write_scientific(value, precision, out);
  char buffer[float_precision_buffer_size];
  size_t size = detail::write_scientific(value, precision, buffer) - buffer;
  if (size > n) size = n;
  memcpy(out, buffer, size);
  return out + size;
}

/// Writes `value` in scientific format with exactly `precision` significant
/// digits (e.g. 1.234e+05) to `out` without a null terminator. Returns a
/// pointer past the last character written; if the representation exceeds `n`
/// characters, only the first `n` are written. `precision` must be in [1, 18];
/// out-of-range values are clamped.
inline auto write_scientific(char* out, size_t n, double value,
                             int precision) noexcept -> char* {
  precision = detail::clamp_precision(precision);
  if (n >= double_precision_buffer_size)
    return detail::write_scientific(value, precision, out);
  char buffer[double_precision_buffer_size];
  size_t size = detail::write_scientific(value, precision, buffer) - buffer;
  if (size > n) size = n;
  memcpy(out, buffer, size);
  return out + size;
}

/// Writes `value` in general format with up to `precision` significant digits
/// and no trailing zeros (e.g. 1.5 or 1.5e+20) to `out` without a null
/// terminator. Fixed notation is used when `value`'s decimal exponent is in
/// [-4, precision), and scientific otherwise. Returns a pointer past the last
/// character written; if the representation exceeds `n` characters, only the
/// first `n` are written. `precision` must be in [1, 18]; out-of-range values
/// are clamped.
inline auto write_general(char* out, size_t n, float value,
                          int precision) noexcept -> char* {
  precision = detail::clamp_precision(precision);
  if (n >= float_precision_buffer_size)
    return detail::write_general(value, precision, out);
  char buffer[float_precision_buffer_size];
  size_t size = detail::write_general(value, precision, buffer) - buffer;
  if (size > n) size = n;
  memcpy(out, buffer, size);
  return out + size;
}

/// Writes `value` in general format with up to `precision` significant digits
/// and no trailing zeros (e.g. 1.5 or 1.5e+20) to `out` without a null
/// terminator. Fixed notation is used when `value`'s decimal exponent is in
/// [-4, precision), and scientific otherwise. Returns a pointer past the last
/// character written; if the representation exceeds `n` characters, only the
/// first `n` are written. `precision` must be in [1, 18]; out-of-range values
/// are clamped.
inline auto write_general(char* out, size_t n, double value,
                          int precision) noexcept -> char* {
  precision = detail::clamp_precision(precision);
  if (n >= double_precision_buffer_size)
    return detail::write_general(value, precision, out);
  char buffer[double_precision_buffer_size];
  size_t size = detail::write_general(value, precision, buffer) - buffer;
  if (size > n) size = n;
  memcpy(out, buffer, size);
  return out + size;
}

}  // namespace zmij

#endif  // ZMIJ_H_
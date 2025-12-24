// A double-to-string conversion algorithm based on Schubfach.
// Copyright (c) 2025 - present, Victor Zverovich
// Distributed under the MIT license (see LICENSE) or alternatively
// the Boost Software License, Version 1.0.

#ifndef ZMIJ_H_
#define ZMIJ_H_

namespace zmij {
namespace detail {
template <typename Float> char* to_string(Float value, char* buffer) noexcept;
}  // namespace detail

enum {
  double_buffer_size = 25,
  float_buffer_size = 17,
};

/// Writes the shortest correctly rounded decimal representation of `value` to
/// `buffer`. `buffer` should point to a buffer of size `double_buffer_size` or
/// larger.
inline char* to_string(double value, char* buffer) noexcept {
  return detail::to_string(value, buffer);
}

/// Writes the shortest correctly rounded decimal representation of `value` to
/// `buffer`. `buffer` should point to a buffer of size `float_buffer_size` or
/// larger.
inline char* to_string(float value, char* buffer) noexcept {
  return detail::to_string(value, buffer);
}

}  // namespace zmij

#endif  // ZMIJ_H_

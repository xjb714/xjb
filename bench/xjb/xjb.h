// A float/double-to-string conversion algorithm based on Schubfach.
// Copyright (c) 2025 - present, Xiang JunBo
// Distributed under the MIT license (see LICENSE) or alternatively
// the Boost Software License, Version 1.0.
// edit date : 2025.12.24

#ifndef XJB_H_
#define XJB_H_

namespace xjb
{
    namespace detail
    {
        template <typename Float>
        char *to_string(Float value, char *buffer);
    } // namespace detail

    enum
    {
        double_buffer_size = 33,
        float_buffer_size = 24,
    };

    /// Writes the shortest correctly rounded decimal representation of `value` to `buffer`.
    /// `buffer` should point to a buffer of size `double_buffer_size` or larger.
    inline char *to_string(double value, char *buffer)
    {
        return detail::to_string(value, buffer);
    }

    /// Writes the shortest correctly rounded decimal representation of `value` to `buffer`.
    /// `buffer` should point to a buffer of size `float_buffer_size` or larger.
    inline char *to_string(float value, char *buffer)
    {
        return detail::to_string(value, buffer);
    }

} // namespace xjb

#endif // XJB_H_

// Tests for https://github.com/xjb714/xjb, ported from
// https://github.com/vitaut/zmij/blob/main/test/zmij-test.cc.
// Original copyright (c) 2025 - present, Victor Zverovich
// Distributed under the MIT license.

#include "src/ftoa.cpp"  // xjb implementation (single-TU include)

#include <gtest/gtest.h>

#include <cmath>
#include <cstring>
#include <limits>
#include <string>

#include "dragonbox_to_chars.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::string dtoa(double value) {
  char buffer[33] = {};
  char* end = xjb::xjb64(value, buffer);
  return {buffer, static_cast<size_t>(end - buffer)};
}

static std::string ftoa(float value) {
  char buffer[17] = {};
  char* end = xjb::xjb32(value, buffer);
  return {buffer, static_cast<size_t>(end - buffer)};
}

// Count the number of decimal digits in an unsigned integer.
static int count_digits(uint64_t n) {
  int c = 1;
  while (n >= 10) { n /= 10; ++c; }
  return c;
}

// Format a (significand, exponent) pair the same way xjb64 does.
//   value = sig * 10^exp   (sig has no trailing zeros)
// xjb64 uses fixed notation when  -4 <= p <= 15  (p = exp + #digits - 1)
// and exponential notation otherwise.
static std::string format_like_xjb64(uint64_t sig, int exp) {
  std::string digits = std::to_string(sig);
  int n = static_cast<int>(digits.size());
  int p = exp + n - 1;  // position of most-significant digit

  if (p >= -4 && p <= 15) {
    int dot_pos = p + 1;  // number of digits before '.'
    if (dot_pos <= 0) {
      return "0." + std::string(-dot_pos, '0') + digits;
    } else if (dot_pos >= n) {
      return digits + std::string(dot_pos - n, '0') + ".0";
    } else {
      return digits.substr(0, dot_pos) + "." + digits.substr(dot_pos);
    }
  } else {
    // exponential: d.dddde±XX (at least two-digit exponent)
    std::string mantissa = (n == 1) ? digits
                                    : digits.substr(0, 1) + "." + digits.substr(1);
    char expstr[16];
    if (p >= 0)
      snprintf(expstr, sizeof(expstr), "e+%02d", p);
    else
      snprintf(expstr, sizeof(expstr), "e-%02d", -p);
    return mantissa + expstr;
  }
}

// Same for xjb32: fixed when  -3 <= p <= 6.
static std::string format_like_xjb32(uint32_t sig, int exp) {
  std::string digits = std::to_string(sig);
  int n = static_cast<int>(digits.size());
  int p = exp + n - 1;

  if (p >= -3 && p <= 6) {
    int dot_pos = p + 1;
    if (dot_pos <= 0) {
      return "0." + std::string(-dot_pos, '0') + digits;
    } else if (dot_pos >= n) {
      return digits + std::string(dot_pos - n, '0') + ".0";
    } else {
      return digits.substr(0, dot_pos) + "." + digits.substr(dot_pos);
    }
  } else {
    std::string mantissa = (n == 1) ? digits
                                    : digits.substr(0, 1) + "." + digits.substr(1);
    char expstr[16];
    if (p >= 0)
      snprintf(expstr, sizeof(expstr), "e+%02d", p);
    else
      snprintf(expstr, sizeof(expstr), "e-%02d", -p);
    return mantissa + expstr;
  }
}

// Build the expected string for a double, handling special values.
static std::string expected_dtoa(double value) {
  if (value == 0.0) {
    return std::signbit(value) ? "-0.0" : "0.0";
  }
  if (std::isinf(value)) {
    return value < 0 ? "-inf" : "inf";
  }
  if (std::isnan(value)) {
    return std::signbit(value) ? "-nan" : "nan";
  }
  auto dec = jkj::dragonbox::to_decimal(
      std::fabs(value), jkj::dragonbox::policy::sign::ignore);
  std::string s = format_like_xjb64(dec.significand, dec.exponent);
  return (value < 0 ? "-" : "") + s;
}

// Build the expected string for a float, handling special values.
static std::string expected_ftoa(float value) {
  if (value == 0.0f) {
    return std::signbit(value) ? "-0.0" : "0.0";
  }
  if (std::isinf(value)) {
    return value < 0 ? "-inf" : "inf";
  }
  if (std::isnan(value)) {
    return std::signbit(value) ? "-nan" : "nan";
  }
  auto dec = jkj::dragonbox::to_decimal(
      std::fabs(value), jkj::dragonbox::policy::sign::ignore);
  std::string s = format_like_xjb32(dec.significand, dec.exponent);
  return (value < 0 ? "-" : "") + s;
}

// ===================================================================
// double tests (ported from zmij-test.cc  dtoa_test)
// ===================================================================

TEST(dtoa_test, normal) {
  EXPECT_EQ(dtoa(6.62607015e-34), "6.62607015e-34");
  // Exact half-ulp tie when rounding to nearest integer.
  EXPECT_EQ(dtoa(5.444310685350916e+14), "544431068535091.6");
}

TEST(dtoa_test, subnormal) {
  EXPECT_EQ(dtoa(std::numeric_limits<double>::denorm_min()), "5e-324");
  EXPECT_EQ(dtoa(1e-323), "1e-323");
  EXPECT_EQ(dtoa(1.2e-322), "1.2e-322");
  EXPECT_EQ(dtoa(1.5e-323), "1.5e-323");
  EXPECT_EQ(dtoa(1.24e-322), "1.24e-322");
  EXPECT_EQ(dtoa(1.234e-320), "1.234e-320");
}

// Test all "irregular" doubles (IEEE significand == 0 -> value is a power of 2)
// for each biased exponent in [1, 0x3fe].
TEST(dtoa_test, all_irregular) {
  for (uint64_t exp = 1; exp < 0x3ff; ++exp) {
    uint64_t bits = exp << 52;
    double value = 0;
    memcpy(&value, &bits, sizeof(double));
    EXPECT_EQ(dtoa(value), expected_dtoa(value)) << "exp=" << exp;
  }
}

// Test the first non-irregular significand (sig==1) for each biased exponent
// in [0, 0x3ff].
TEST(dtoa_test, all_exponents) {
  for (uint64_t exp = 0; exp <= 0x3ff; ++exp) {
    uint64_t bits = (exp << 52) | 1;
    double value = 0;
    memcpy(&value, &bits, sizeof(double));
    EXPECT_EQ(dtoa(value), expected_dtoa(value)) << "exp=" << exp;
  }
}

TEST(dtoa_test, small_int) { EXPECT_EQ(dtoa(1), "1.0"); }

TEST(dtoa_test, zero) {
  EXPECT_EQ(dtoa(0), "0.0");
  EXPECT_EQ(dtoa(-0.0), "-0.0");
}

TEST(dtoa_test, inf) {
  EXPECT_EQ(dtoa(std::numeric_limits<double>::infinity()), "inf");
  EXPECT_EQ(dtoa(-std::numeric_limits<double>::infinity()), "-inf");
}

TEST(dtoa_test, nan) {
  EXPECT_EQ(dtoa(std::numeric_limits<double>::quiet_NaN()), "nan");
  EXPECT_EQ(dtoa(-std::numeric_limits<double>::quiet_NaN()), "-nan");
}

TEST(dtoa_test, shorter) {
  // A possibly shorter underestimate is picked.
  EXPECT_EQ(dtoa(-4.932096661796888e-226), "-4.932096661796888e-226");
  // A possibly shorter overestimate is picked.
  EXPECT_EQ(dtoa(3.439070283483335e+35), "3.439070283483335e+35");
}

TEST(dtoa_test, single_candidate) {
  // Only an underestimate is in the rounding region.
  EXPECT_EQ(dtoa(6.606854224493745e-17), "6.606854224493745e-17");
  // Only an overestimate is in the rounding region.
  EXPECT_EQ(dtoa(6.079537928711555e+61), "6.079537928711555e+61");
}

TEST(dtoa_test, no_underrun) {
  // Regression: must not write before the buffer start.
  dtoa(9.061488e+15);
}

TEST(dtoa_test, no_overrun) {
  EXPECT_EQ(dtoa(-1.2345678901234567e+123), "-1.2345678901234567e+123");
  EXPECT_EQ(dtoa(-1.3588129002659584e-245), "-1.3588129002659584e-245");
}

// ===================================================================
// float tests (ported from zmij-test.cc  ftoa_test)
// ===================================================================

TEST(ftoa_test, normal) {
  EXPECT_EQ(ftoa(6.62607e-34f), "6.62607e-34");
  EXPECT_EQ(ftoa(1.342178e+08f), "1.342178e+08");
  EXPECT_EQ(ftoa(1.3421781e+08f), "1.3421781e+08");
}

TEST(ftoa_test, subnormal) {
  EXPECT_EQ(ftoa(std::numeric_limits<float>::denorm_min()), "1e-45");
}

TEST(ftoa_test, zero) {
  EXPECT_EQ(ftoa(0.0f), "0.0");
  EXPECT_EQ(ftoa(-0.0f), "-0.0");
}

TEST(ftoa_test, inf) {
  EXPECT_EQ(ftoa(std::numeric_limits<float>::infinity()), "inf");
  EXPECT_EQ(ftoa(-std::numeric_limits<float>::infinity()), "-inf");
}

TEST(ftoa_test, nan) {
  EXPECT_EQ(ftoa(std::numeric_limits<float>::quiet_NaN()), "nan");
  EXPECT_EQ(ftoa(-std::numeric_limits<float>::quiet_NaN()), "-nan");
}

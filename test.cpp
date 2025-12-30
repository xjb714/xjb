#include "bench/xjb/xjb.cpp"

#include "bench/util/gtest/gtest/gtest.h"

#include <math.h>
#include <stdio.h>
#include <limits>
#include <string>

auto dtoa(double value) -> std::string {
  char buffer[64];
  xjb::xjb32(value, buffer);
  return buffer;
}


TEST(zmij_test, normal) { EXPECT_EQ(dtoa(6.62607015e-34), "6.62607015e-34"); }

TEST(zmij_test, small_int) { EXPECT_EQ(dtoa(1), "1.e+00"); }

TEST(zmij_test, zero) {
  EXPECT_EQ(dtoa(0), "0");
  EXPECT_EQ(dtoa(-0.0), "-0");
}

TEST(zmij_test, inf) {
  EXPECT_EQ(dtoa(std::numeric_limits<double>::infinity()), "inf");
  EXPECT_EQ(dtoa(-std::numeric_limits<double>::infinity()), "-inf");
}

TEST(zmij_test, nan) {
  EXPECT_EQ(dtoa(std::numeric_limits<double>::quiet_NaN()), "nan");
  EXPECT_EQ(dtoa(-std::numeric_limits<double>::quiet_NaN()), "-nan");
}

TEST(zmij_test, shorter) {
  // A possibly shorter underestimate is picked (u' in Schubfach).
  EXPECT_EQ(dtoa(-4.932096661796888e-226), "-4.932096661796888e-226");

  // A possibly shorter overestimate is picked (w' in Schubfach).
  EXPECT_EQ(dtoa(3.439070283483335e+35), "3.439070283483335e+35");
}

TEST(zmij_test, single_candidate) {
  // Only an underestimate is in the rounding region (u in Schubfach).
  EXPECT_EQ(dtoa(6.606854224493745e-17), "6.606854224493745e-17");

  // Only an overestimate is in the rounding region (w in Schubfach).
  EXPECT_EQ(dtoa(6.079537928711555e+61), "6.079537928711555e+61");
}

TEST(zmij_test, all_exponents) {
  using limits = std::numeric_limits<double>;
  for (int exp = limits::min_exponent; exp < limits::max_exponent; ++exp) {
    double expected = ldexp(1, exp);
    double actual = 0;
    sscanf(dtoa(expected).c_str(), "%lg", &actual);
    EXPECT_EQ(actual, expected);
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

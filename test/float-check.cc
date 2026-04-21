// Exhaustive verification of xjb32 (float-to-string) on all 2^32 IEEE-754
// single-precision bit patterns.
// Ported from https://github.com/vitaut/zmij/blob/main/test/float-check.cc.
// Original copyright (c) 2025 - present, Victor Zverovich
// Distributed under the MIT license.

// Usage:
// clang++ -O3 -march=native -std=c++20 float-check.cc -o float-check

#include <stdint.h>
#include <string.h>

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <thread>
#include <vector>

#include "../bench/dragonbox/dragonbox_to_chars.h"
#include "../src/ftoa.cpp"

#include "../bench/schubfach_xjb/schubfach_xjb_i.hpp"

// ---------------------------------------------------------------------------
// Reference formatter that reproduces xjb32's formatting conventions.
// xjb32 uses fixed notation when  -3 <= p <= 6  (p = exp + #digits - 1)
// and exponential notation otherwise.
// Integers in fixed notation always get ".0" appended.
// Exponential exponents are at least two digits: e+07, e-34.
// ---------------------------------------------------------------------------

static int count_digits(uint32_t n) {
  int c = 1;
  while (n >= 10) { n /= 10; ++c; }
  return c;
}

static void format_like_xjb32(char* out, uint32_t sig, int exp) {
  char digits[16];
  int n = snprintf(digits, sizeof(digits), "%u", sig);
  int p = exp + n - 1;

  if (p >= -3 && p <= 6) {
    int dot_pos = p + 1;
    if (dot_pos <= 0) {
      *out++ = '0'; *out++ = '.';
      for (int i = 0; i < -dot_pos; ++i) *out++ = '0';
      memcpy(out, digits, n); out += n;
    } else if (dot_pos >= n) {
      memcpy(out, digits, n); out += n;
      for (int i = 0; i < dot_pos - n; ++i) *out++ = '0';
      *out++ = '.'; *out++ = '0';
    } else {
      memcpy(out, digits, dot_pos); out += dot_pos;
      *out++ = '.';
      memcpy(out, digits + dot_pos, n - dot_pos); out += n - dot_pos;
    }
  } else {
    // exponential
    *out++ = digits[0];
    if (n > 1) { *out++ = '.'; memcpy(out, digits + 1, n - 1); out += n - 1; }
    *out++ = 'e';
    if (p >= 0) {
      *out++ = '+';
      out += snprintf(out, 8, "%02d", p);
    } else {
      *out++ = '-';
      out += snprintf(out, 8, "%02d", -p);
    }
  }
  *out = '\0';
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {
  unsigned num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0) num_threads = 1;
  std::vector<std::thread> threads(num_threads);
  std::atomic<unsigned long long> num_processed_floats(0);
  std::atomic<unsigned long long> num_errors(0);

  constexpr unsigned long long num_floats = 1ULL << 32;
  constexpr double percent = 100.0 / num_floats;

  fprintf(stderr, "Verifying all %llu floats using %u threads\n",
          num_floats, num_threads);

  auto start = std::chrono::steady_clock::now();

  for (unsigned i = 0; i < num_threads; ++i) {
    uint32_t begin = static_cast<uint32_t>(num_floats * i / num_threads);
    uint32_t end   = static_cast<uint32_t>(num_floats * (i + 1) / num_threads);
    uint32_t count = end - begin;

    threads[i] = std::thread([i, begin, count, &num_processed_floats,
                               &num_errors, percent, num_floats] {
      char actual[64] = {};
      char expected[64] = {};

      uint64_t last_count = 0;
      auto last_time = std::chrono::steady_clock::now();
      bool has_errors = false;

      for (uint32_t j = 0; j < count; ++j) {
        uint64_t batch = j - last_count + 1;
        if (batch >= (1u << 21) || j == count - 1) {
          num_processed_floats += batch;
          last_count += batch;
          auto now = std::chrono::steady_clock::now();
          if (i == 0 && now - last_time >= std::chrono::seconds(1)) {
            last_time = now;
            fprintf(stderr, "\rProgress: %5.2f%%",
                    num_processed_floats.load() * percent);
            fflush(stderr);
          }
        }

        uint32_t bits = begin + j;
        float value = 0;
        memcpy(&value, &bits, sizeof(float));

        // --- produce actual ---
        char* aend = xjb::xjb32(value, actual);
        *aend = '\0';

        // --- produce expected --- // faster
        //schubfach_xjb_f32_to_str(value, expected);

        // //--- produce expected ---
        if (value == 0.0f) {
          strcpy(expected, std::signbit(value) ? "-0.0" : "0.0");
        } else if (std::isinf(value)) {
          strcpy(expected, value < 0 ? "-inf" : "inf");
        } else if (std::isnan(value)) {
          strcpy(expected, std::signbit(value) ? "-nan" : "nan");
        } else {
          auto dec = jkj::dragonbox::to_decimal(
              std::fabs(value), jkj::dragonbox::policy::sign::ignore);
          char* p = expected;
          if (value < 0) *p++ = '-';
          format_like_xjb32(p, dec.significand, dec.exponent);
        }

        if (strcmp(actual, expected) == 0) continue;

        ++num_errors;
        if (!has_errors) {
          fprintf(stderr,
                  "\nOutput mismatch for bits=0x%08x: \"%s\" != \"%s\"\n",
                  bits, actual, expected);
          has_errors = true;
        }
      }
    });
  }

  for (auto& t : threads) t.join();

  auto finish = std::chrono::steady_clock::now();
  using seconds = std::chrono::duration<double>;
  fprintf(stderr,
          "\nTested %llu values in %.2f seconds, %llu error(s)\n",
          num_processed_floats.load(),
          std::chrono::duration_cast<seconds>(finish - start).count(),
          num_errors.load());

  return num_errors != 0 ? 1 : 0;
}

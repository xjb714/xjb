#define FMT_HEADER_ONLY 1

#define FMT_BEGIN_NAMESPACE namespace full_cache_test { namespace fmt {
#define FMT_END_NAMESPACE }}

namespace full_cache_test {}
using namespace full_cache_test;

#undef FMT_USE_FULL_CACHE_DRAGONBOX
#define FMT_USE_FULL_CACHE_DRAGONBOX 1

#include "include/fmt/compile.h"
#include "src/format.cc"

char* fmt_full_f64_to_str(double value, char* buffer) {
  buffer = full_cache_test::fmt::format_to(buffer, FMT_COMPILE("{}"), value);
  *buffer = '\0';
  return buffer;
}

char* fmt_comp_f64_to_str(double value, char* buffer) {
  buffer = fmt::format_to(buffer, FMT_COMPILE("{}"), value);
  *buffer = '\0';
  return buffer;
}

char* fmt_full_f32_to_str(float value, char* buffer) {
  buffer = full_cache_test::fmt::format_to(buffer, FMT_COMPILE("{}"), value);
  *buffer = '\0';
  return buffer;
}

char* fmt_comp_f32_to_str(float value, char* buffer) {
  buffer = fmt::format_to(buffer, FMT_COMPILE("{}"), value);
  *buffer = '\0';
  return buffer;
}
//#include "dragonbox.cc"
//#include "format-inl.h"
//#include "fmt-12.1.0/include/fmt/format-inl.h"

//#include "format.cc"

#include "dragonbox_to_chars.h"
#include "dragonbox_to_chars.cpp"


void dragonbox_f64_to_dec(double v,unsigned long long* dec,int *e10)
{
    // unsigned long  vi = *(unsigned  long*)&v;
    // unsigned long  sig = vi & ((1ull<<52) - 1);
    // unsigned long  exp = (vi & ((1ull<<63) - 1))>>52;
    
    // struct dragonbox_warp::FloatingDecimal64 res = dragonbox_warp::ToDecimal64(sig, exp);
    // *dec = res.significand;
    // *e10 = res.exponent;

    // auto res = fmt::v12::detail::dragonbox::to_decimal<double>(v);
    // *dec = res.significand;
    // *e10 = res.exponent;

    auto res = jkj::dragonbox::to_decimal<double>(v,jkj::dragonbox::policy::sign::ignore);
    *dec = res.significand;
    *e10 = res.exponent;
}
void dragonbox_f32_to_dec(float v,unsigned int* dec,int *e10)
{
    auto res = jkj::dragonbox::to_decimal<float>(v,jkj::dragonbox::policy::sign::ignore);
    *dec = res.significand;
    *e10 = res.exponent;
}

char* dragonbox_comp_f64_to_str(double x, char* buffer) {
  return jkj::dragonbox::to_chars(x, buffer, jkj::dragonbox::policy::cache::compact);
}


char* dragonbox_full_f64_to_str(double x, char* buffer) {
  return jkj::dragonbox::to_chars(x, buffer, jkj::dragonbox::policy::cache::full);
}

char* dragonbox_comp_f32_to_str(float x, char* buffer) {
    return jkj::dragonbox::to_chars(x, buffer, jkj::dragonbox::policy::cache::compact);
}

char* dragonbox_full_f32_to_str(float x, char* buffer) {
    return jkj::dragonbox::to_chars(x, buffer, jkj::dragonbox::policy::cache::full);
}
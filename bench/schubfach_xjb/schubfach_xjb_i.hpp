#include "schubfach_64.cc"
#include "schubfach_32.cpp"

void schubfach_xjb_f64_to_dec(double v,unsigned long long* dec,int *e10)
{
    unsigned long  vi = *(unsigned  long*)&v;
    unsigned long  sig = vi & ((1ull<<52) - 1);
    unsigned long  exp = (vi & ((1ull<<63) - 1))>>52;

    xjb::FloatingDecimal64 res = xjb::ToDecimal64(sig, exp);
    *dec = res.digits;
    *e10 = res.exponent;
}

// void schubfach_xjb_126_f64_to_dec(double v,unsigned long long* dec,int *e10)
// {
//     unsigned long  vi = *(unsigned  long*)&v;
//     unsigned long  sig = vi & ((1ull<<52) - 1);
//     unsigned long  exp = (vi & ((1ull<<63) - 1))>>52;

//     xjb::FloatingDecimal64 res = xjb::ToDecimal64_126bit(sig, exp);
//     *dec = res.digits;
//     *e10 = res.exponent;
// }


void schubfach_xjb_f32_to_dec(float v,unsigned int* dec,int* e10){
    unsigned int  vi = *(unsigned  int*)&v;
    unsigned int  sig = vi & ((1ull<<23) - 1);
    unsigned int  exp = (vi & ((1ull<<31) - 1))>>23;

    schubfach_xjb32::FloatingDecimal32 res = schubfach_xjb32::ToDecimal32_xjb(sig, exp);
    *dec = res.digits;
    *e10 = res.exponent;
}

char* schubfach_xjb_f64_to_str(double v,char* buffer){
    return xjb::Dtoa(buffer, v);
}

char* schubfach_xjb_f32_to_str(float v,char* buffer){
    return schubfach_32::Ftoa(buffer, v);
}
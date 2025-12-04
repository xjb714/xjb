#include "schubfach_64.cc"
#include "schubfach_32.cpp"



void schubfach_f64_to_dec(double v,unsigned long long* dec,int* e10){
    unsigned long  vi = *(unsigned  long*)&v;
    unsigned long  sig = vi & ((1ull<<52) - 1);
    unsigned long  exp = (vi & ((1ull<<63) - 1))>>52;

    FloatingDecimal64 res = ToDecimal64(sig, exp);
    *dec = res.digits;
    *e10 = res.exponent;
}

void schubfach_f32_to_dec(float v,unsigned int* dec,int* e10){
    unsigned int  vi = *(unsigned  int*)&v;
    unsigned int  sig = vi & ((1ull<<23) - 1);
    unsigned int  exp = (vi & ((1ull<<31) - 1))>>23;

    schubfach_32::FloatingDecimal32 res = schubfach_32::ToDecimal32(sig, exp);
    *dec = res.digits;
    *e10 = res.exponent;
}

char* schubfach_f64_to_str(double v,char* buffer){
    return schubfach::Dtoa(buffer, v);
}

char* schubfach_f32_to_str(float v,char* buffer){
    return schubfach_32::Ftoa(buffer, v);
}
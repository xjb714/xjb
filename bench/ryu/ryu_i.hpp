#include "d2s.c"
#include "f2s.c"

void ryu_f64_to_dec(double v,unsigned long long* dec,int *e10)
{
    unsigned long  vi = *(unsigned  long*)&v;
    unsigned long  sig = vi & ((1ull<<52) - 1);
    unsigned long  exp = (vi & ((1ull<<63) - 1))>>52;

    floating_decimal_64 res = d2d(sig, exp);
    
    *dec = res.mantissa;
    *e10 = res.exponent;
}
void ryu_f32_to_dec(float v,unsigned int* dec,int *e10)
{
    unsigned int  vi = *(unsigned  int*)&v;
    unsigned int  sig = vi & ((1ull<<23) - 1);
    unsigned int  exp = (vi & ((1ull<<31) - 1))>>23;

    floating_decimal_32 res = f2d(sig, exp);
    
    *dec = res.mantissa;
    *e10 = res.exponent;
}



char* ryu_f64_to_str(double v,char* buffer)
{
    int len = d2s_buffered_n(v,buffer);
    buffer[len] = '\0';
    return buffer+len;
}

char* ryu_f32_to_str(double v,char* buffer)
{
    int len = f2s_buffered_n(v,buffer);
    buffer[len] = '\0';
    return buffer+len;
}


// 0:double
// 1:float
#define TEJU_USE_FLOAT BENCH_FLOAT


#define USE_FLOAT TEJU_USE_FLOAT


#define USE_DOUBLE !USE_FLOAT


#if USE_DOUBLE
#include "generated/ieee64_with_uint128.c"
#endif

void teju_f64_to_dec(double v,unsigned long long* dec,int *e10)
{
#if USE_DOUBLE
    unsigned long  vi = *(unsigned  long*)&v;
    unsigned long  sig = vi & ((1ull<<52) - 1);
    unsigned long  exp = (vi>>52) & 2047;

    unsigned long sig_bin;
    long long exp_bin;
    if( exp )[[likely]]
    {
        sig_bin = sig | (1ull<<52);
        exp_bin = exp - 1075;
    } 
    else
    {
        sig_bin = sig;
        exp_bin = -1074;
    }
    teju64_fields_t binary;
    binary.mantissa = sig_bin;
    binary.exponent = exp_bin;

    teju64_fields_t res = teju_ieee64_with_uint128(binary) ;

    *dec = res.mantissa; 
    *e10 = res.exponent;
#endif 
}

#if USE_FLOAT
#include "generated/ieee32_with_uint128.c"
#endif 
void teju_f32_to_dec(float v,unsigned int* dec,int *e10)
{
#if USE_FLOAT
    unsigned int  vi = *(unsigned  int*)&v;
    unsigned int  sig = vi & ((1ull<<23) - 1);
    unsigned int  exp = (vi>>23) & 255;

    unsigned int sig_bin;
    int exp_bin;
    if( exp )[[likely]]
    {
        sig_bin = sig | (1u<<23);
        exp_bin = exp - 150;
    } 
    else
    {
        sig_bin = sig;
        exp_bin = 1-150;
    }
    teju32_fields_t binary;
    binary.mantissa = sig_bin;
    binary.exponent = exp_bin;

    teju32_fields_t res = teju_ieee32_with_uint128( binary) ;

    *dec = res.mantissa; 
    *e10 = res.exponent;
#endif 
}
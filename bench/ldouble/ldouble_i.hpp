#include "ldouble.cpp"
#include <stdio.h>

void ldouble_f64_to_dec(double v, unsigned long long *sig_dec, int *exp_dec)
{
    unsigned long vi = *(unsigned long *)&v;
    unsigned long sig = vi & ((1ull << 52) - 1);
    unsigned long exp = (vi >> 52) & 2047;

    unsigned long long vf;
    int ve;
    if ( exp ) [[likely]]
    {
        vf = sig | (1ull << 52);
        ve = exp - 0x3ff - 52;
        ldouble_convert(vf, ve);
    }
    else
    {
        vf = sig;
        ve = 1 - 0x3ff - 52;
        unsigned long long lshiftbit = __builtin_clzll(vf) - 11;
        vf <<= lshiftbit;
        ve -= lshiftbit;
        ldouble_convert_no_normalized(vf,ve,lshiftbit);
    }
    //ldouble_convert(vf, ve);
    *sig_dec = vf;
    *exp_dec = ve;
}
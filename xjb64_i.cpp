#include "xjb64.cpp"

static inline void xjb_f64_to_dec(double v,unsigned long long* dec,int *e10)
{
    unsigned long long vi = *(unsigned  long long*)&v;
    unsigned long long sig = vi & ((1ull<<52) - 1);
    unsigned long long exp = (vi>>52) & 2047;

    xjb64(sig,exp,(uint64_t*)dec,e10) ;
}
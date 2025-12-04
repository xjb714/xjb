//compile time is too long, set 0 to reduce compile time
#define use_yyjson 1

#include "yy_double.c"
#if use_yyjson
    #include "yyjson.c"
#endif

void yy_double_f64_to_dec(double v, unsigned long long *dec, int *e10)
{
    // src from yy_double.c
    unsigned long long vi = *(unsigned long long *)&v;
    unsigned long long sig = vi & ((1ull << 52) - 1);
    // unsigned long  exp = (vi>>52) & 2047;
    unsigned long long exp = (vi & (2047ull << 52)) >> 52;

    unsigned long long sig_bin, exp_bin;
    if (exp) [[likely]]
    {
        sig_bin = sig | (1ull << 52);
        exp_bin = exp - 1075;
    }
    else
    {
        sig_bin = sig;
        exp_bin = -1074;
    }

    yy_double::yy_double_f64_bin_to_dec(sig_bin, exp_bin, (uint64_t *)dec, e10);
}
void yy_double_full_f64_to_dec(double v, unsigned long long *dec, int *e10)
{
    // src from yy_double.c
    unsigned long long vi = *(unsigned long long *)&v;
    unsigned long long sig = vi & ((1ull << 52) - 1);
    // unsigned long  exp = (vi>>52) & 2047;
    unsigned long long exp = (vi & (2047ull << 52)) >> 52;

    unsigned long long sig_bin, exp_bin;
    if (exp) [[likely]]
    {
        sig_bin = sig | (1ull << 52);
        exp_bin = exp - 1075;
    }
    else
    {
        sig_bin = sig;
        exp_bin = -1074;
    }

    yy_double::yy_double_full_f64_bin_to_dec(sig_bin, exp_bin, (uint64_t *)dec, e10);
}

void yyjson_f64_to_dec(double v, unsigned long long *dec, int *e10)
{
    // src from yyjson.c
    unsigned long long vi = *(unsigned long long *)&v;
    unsigned long long sig = vi & ((1ull << 52) - 1);
    // unsigned long long exp = (vi>>52) & 2047;
    unsigned long long exp = (vi & (2047ull << 52)) >> 52;

    unsigned long long sig_bin, exp_bin;
    if (exp) [[likely]]
    {
        sig_bin = sig | (1ull << 52);
        exp_bin = exp - 1075;
    }
    else
    {
        sig_bin = sig;
        exp_bin = -1074;
    }
#if use_yyjson
    f64_bin_to_dec(sig, exp, sig_bin, exp_bin, (uint64_t *)dec, e10);
#endif
}
void yyjson_f32_to_dec(float v, unsigned int *dec, int *e10)
{
    // src from yyjson.c
    unsigned int vi = *(unsigned int *)&v;
    unsigned int sig = vi & ((1ull << 23) - 1);
    // unsigned int  exp = (vi>>23) & 255;
    unsigned int exp = (vi & (255u << 23)) >> 23;

    unsigned int sig_bin, exp_bin;
    if (exp) [[likely]]
    {
        sig_bin = sig | (1ull << 23);
        exp_bin = exp - 150;
    }
    else
    {
        sig_bin = sig;
        exp_bin = 1 - 150;
    }
#if use_yyjson
    f32_bin_to_dec(sig, exp, sig_bin, exp_bin, (uint32_t *)dec, e10);
#endif
}

char *yy_double_f64_to_str(double v, char *buffer)
{
    return yy_double::yy_double_to_string(v, buffer);
}

char *yyjson_f64_to_str(double v, char *buffer)
{
#if use_yyjson
    // src from yyjson.c
    return (char *)write_f64_raw((u8 *)buffer, *(u64 *)&v, YYJSON_WRITE_INF_AND_NAN_AS_NULL);
#else 
    return buffer;
#endif
}

char *yyjson_f32_to_str(float v, char *buffer)
{
#if use_yyjson
    // src from yyjson.c
    return (char *)write_f32_raw((u8 *)buffer, *(u64 *)&v, YYJSON_WRITE_INF_AND_NAN_AS_NULL);
#else 
    return buffer;
#endif
}
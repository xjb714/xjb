// this file to check xjb algorithm correctness

#include <iostream>
#include <random>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#include "schubfach_xjb/schubfach_xjb_i.hpp"
#include "xjb/xjb_i.hpp"

auto getns()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    return nanos;
}

std::random_device rd;
std::mt19937_64 gen(rd());

u64 f64_to_u64(double d)
{
    return *(u64 *)&d;
}

double u64_to_f64(u64 u)
{
    return *(double *)&u;
}

u32 f32_to_u32(float f)
{
    return *(u32 *)&f;
}

float u32_to_f32(u32 u)
{
    return *(float *)&u;
}

double gen_double_filter_NaN_Inf()
{
    u64 rnd, rnd_abs;
    do
    {
        rnd = gen();
        rnd_abs = rnd & ((1ull << 63) - 1);
    } while (rnd_abs >= (0x7ffull << 52)); // nan or inf
    return u64_to_f64(rnd);
}
unsigned check_xjb64_and_schubfach_xjb_jsonformat(double d)
{
#if BENCH_STR
    unsigned long long u = *(u64 *)&d;
    static char buf_xjb[64];
    // char buf_xjb_comp[33];
    static char buf_schubfach_xjb[32];
    // memset(buf_xjb, 0, 33);
    // memset(buf_xjb_comp, 0, 33);
    // memset(buf_schubfach_xjb, 0, 33);
    char *end_buf_xjb = xjb64_f64_to_str(d, buf_xjb);
    // char *end_buf_xjb_comp = xjb64_comp_f64_to_str(d, buf_xjb_comp);
    char *end_buf_schubfach_xjb = schubfach_xjb_f64_to_str(d, buf_schubfach_xjb);
    int len_xjb = end_buf_xjb - buf_xjb;
    // int len_xjb_comp = end_buf_xjb_comp - buf_xjb_comp;
    int len_schubfach_xjb = end_buf_schubfach_xjb - buf_schubfach_xjb;
    // if (len_xjb_comp != len_schubfach_xjb || len_xjb != len_schubfach_xjb)
    if (len_xjb != len_schubfach_xjb)
    {
        // printf("f = %.8le, u = %x, len_xjb=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n",f,u,len_xjb,len_schubfach_xjb,buf_xjb,buf_schubfach_xjb , (u>>23) ? "normal" : "subnormal");
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);
        // printf("f = %.16le, u = %llx, len_xjb=%d , len_xjb_comp=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s , %s\n", d, u, len_xjb, len_xjb_comp, len_schubfach_xjb, buf_xjb, buf_xjb_comp, buf_schubfach_xjb, (u & (2047ull << 52)) ? "normal" : "subnormal");
        printf("f = %.16le, u = %llx, len_xjb=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n", d, u, len_xjb, len_schubfach_xjb, buf_xjb, buf_schubfach_xjb, (u & (2047ull << 52)) ? "normal" : "subnormal");
        //  exit(0);
        return 1;
    }
    // if (memcmp(buf_xjb_comp, buf_schubfach_xjb, len_xjb_comp) == 0 && memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    if (memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    {
        // if three string equal return OK;
        return 0;
    }
    else
    {
        // printf("f = %.8le, u = %x, buf_xjb=%s, buf_schubfach_xjb=%s\n",f,u,buf_xjb,buf_schubfach_xjb);
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);

        // printf("f = %.16le, u = %llx, buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s \n", d, u, buf_xjb, buf_xjb_comp, buf_schubfach_xjb);
        printf("f = %.16le, u = %llx, buf_xjb=%s, buf_schubfach_xjb=%s \n", d, u, buf_xjb, buf_schubfach_xjb);
        //  exit(0);
    }
    return 1;
#else
    return 0;
    // use schubfach_xjb as reference implementation
    // unsigned long long dec, dec_xjb, dec_xjb_comp;
    // int e10, e10_xjb, e10_xjb_comp;
    // schubfach_xjb_f64_to_dec(d, &dec, &e10);
    // // schubfach_f64_to_dec(d,&dec_xjb,&e10_xjb); // schubfach_xjb and schubfach result always same
    // xjb64_f64_to_dec(d, &dec_xjb, &e10_xjb);
    // xjb64_comp_f64_to_dec(d, &dec_xjb_comp, &e10_xjb_comp);
    // if ((dec == dec_xjb && e10 == e10_xjb && dec == dec_xjb_comp && e10 == e10_xjb_comp))
    // {
    //     return 0;
    // }
    // else
    // {
    //     // printf("f = %.16le, dec=%llu,e10=%d , dec_xjb=%llu,e10_xjb=%d, dec_xjb_comp=%llu,e10_xjb_comp=%d,",d,dec,e10,dec_xjb,e10_xjb,dec_xjb_comp,e10_xjb_comp);
    //     // exit(0);
    // }
    // return 1;
#endif
}
unsigned check_xjb64_and_schubfach_xjb(double d)
{
#if BENCH_STR

    return check_xjb64_and_schubfach_xjb_jsonformat(d);

    unsigned long long u = *(u64 *)&d;
    static char buf_xjb[33];
    static char buf_xjb_comp[33];
    static char buf_schubfach_xjb[33];
    memset(buf_xjb, 0, 33);
    memset(buf_xjb_comp, 0, 33);
    memset(buf_schubfach_xjb, 0, 33);
    char *end_buf_xjb = xjb64_f64_to_str(d, buf_xjb);
    char *end_buf_xjb_comp = xjb64_comp_f64_to_str(d, buf_xjb_comp);
    char *end_buf_schubfach_xjb = schubfach_xjb_f64_to_str(d, buf_schubfach_xjb);
    int len_xjb = end_buf_xjb - buf_xjb;
    int len_xjb_comp = end_buf_xjb_comp - buf_xjb_comp;
    int len_schubfach_xjb = end_buf_schubfach_xjb - buf_schubfach_xjb;
    if (len_xjb_comp != len_schubfach_xjb || len_xjb != len_schubfach_xjb)
    // if (len_xjb != len_schubfach_xjb)
    {
        // printf("f = %.8le, u = %x, len_xjb=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n",f,u,len_xjb,len_schubfach_xjb,buf_xjb,buf_schubfach_xjb , (u>>23) ? "normal" : "subnormal");
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);
        printf("f = %.16le, u = %llx, len_xjb=%d , len_xjb_comp=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s , %s\n", d, u, len_xjb, len_xjb_comp, len_schubfach_xjb, buf_xjb, buf_xjb_comp, buf_schubfach_xjb, (u & (2047ull << 52)) ? "normal" : "subnormal");
        // printf("f = %.16le, u = %llx, len_xjb=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n", d, u, len_xjb, len_schubfach_xjb, buf_xjb, buf_schubfach_xjb, (u & (2047ull << 52)) ? "normal" : "subnormal");
        //  exit(0);
        return 1;
    }
    if (memcmp(buf_xjb_comp, buf_schubfach_xjb, len_xjb_comp) == 0 && memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    // if (memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    {
        // if three string equal return OK;
        return 0;
    }
    else
    {
        // printf("f = %.8le, u = %x, buf_xjb=%s, buf_schubfach_xjb=%s\n",f,u,buf_xjb,buf_schubfach_xjb);
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);

        printf("f = %.16le, u = %llx, buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s \n", d, u, buf_xjb, buf_xjb_comp, buf_schubfach_xjb);
        // printf("f = %.16le, u = %llx, buf_xjb=%s, buf_schubfach_xjb=%s \n", d, u, buf_xjb, buf_schubfach_xjb);
        //  exit(0);
    }
    return 1;
#else
    // use schubfach_xjb as reference implementation
    unsigned long long dec, dec_xjb, dec_xjb_comp;
    int e10, e10_xjb, e10_xjb_comp;
    schubfach_xjb_f64_to_dec(d, &dec, &e10);
    // schubfach_f64_to_dec(d,&dec_xjb,&e10_xjb); // schubfach_xjb and schubfach result always same
    xjb64_f64_to_dec(d, &dec_xjb, &e10_xjb);
    xjb64_comp_f64_to_dec(d, &dec_xjb_comp, &e10_xjb_comp);
    if ((dec == dec_xjb && e10 == e10_xjb && dec == dec_xjb_comp && e10 == e10_xjb_comp))
    {
        return 0;
    }
    else
    {
        // printf("f = %.16le, dec=%llu,e10=%d , dec_xjb=%llu,e10_xjb=%d, dec_xjb_comp=%llu,e10_xjb_comp=%d,",d,dec,e10,dec_xjb,e10_xjb,dec_xjb_comp,e10_xjb_comp);
        // exit(0);
    }
    return 1;
#endif
}

unsigned check_xjb32_and_schubfach32_xjb(float f)
{
    u32 u = *(u32 *)&f;
    unsigned int dec, dec_xjb, dec_xjb_comp;
    int e10, e10_xjb, e10_xjb_comp;
    schubfach_xjb_f32_to_dec(f, &dec, &e10);
    xjb_v2_f32_to_dec(f, &dec_xjb, &e10_xjb);
    xjb_comp_f32_to_dec(f, &dec_xjb_comp, &e10_xjb_comp);
    if ((dec == dec_xjb && e10 == e10_xjb && dec == dec_xjb_comp && e10 == e10_xjb_comp))
    {
        return 0;
    }
    else
    {
        printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n", f, dec, e10, dec_xjb, e10_xjb);
    }
    return 1;
}
unsigned check_xjb32_and_schubfach32_xjb_string_jsonformat(float f)
{
    u32 u = *(u32 *)&f;
    static char buf_xjb[32];
    // char buf_xjb_comp[32];
    static char buf_schubfach_xjb[32];
    memset(buf_xjb, 0, 32);
    // memset(buf_xjb_comp, 0, 32);
    memset(buf_schubfach_xjb, 0, 32);
    char *end_buf_xjb = xjb32_f32_to_str(f, buf_xjb);
    // char *end_buf_xjb_comp = xjb32_comp_f32_to_str(f, buf_xjb_comp);
    char *end_buf_schubfach_xjb = schubfach_xjb_f32_to_str(f, buf_schubfach_xjb);
    int len_xjb = end_buf_xjb - buf_xjb;
    // int len_xjb_comp = end_buf_xjb_comp - buf_xjb_comp;
    int len_schubfach_xjb = end_buf_schubfach_xjb - buf_schubfach_xjb;
    // if (len_xjb_comp != len_schubfach_xjb || len_xjb != len_schubfach_xjb)
    if (len_xjb != len_schubfach_xjb)
    {
        // printf("f = %.8le, u = %x, len_xjb=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n",f,u,len_xjb,len_schubfach_xjb,buf_xjb,buf_schubfach_xjb , (u>>23) ? "normal" : "subnormal");
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);
        // printf("f = %.8le, u = %x, len_xjb=%d , len_xjb_comp=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s , %s\n", f, u, len_xjb, len_xjb_comp, len_schubfach_xjb, buf_xjb, buf_xjb_comp, buf_schubfach_xjb, (u >> 23) ? "normal" : "subnormal");
        printf("f = %.8le, u = %x, len_xjb=%d  , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n", f, u, len_xjb, len_schubfach_xjb, buf_xjb, buf_schubfach_xjb, (u >> 23) ? "normal" : "subnormal");
        // exit(0);
        return 1;
    }
    // if (memcmp(buf_xjb_comp, buf_schubfach_xjb, len_xjb_comp) == 0 && memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    if (memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    {
        // if three string equal return OK;
        return 0;
    }
    else
    {
        // printf("f = %.8le, u = %x, buf_xjb=%s, buf_schubfach_xjb=%s\n",f,u,buf_xjb,buf_schubfach_xjb);
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);

        // printf("f = %.8e, u = %x, buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s \n", f, u, buf_xjb, buf_xjb_comp, buf_schubfach_xjb);
        printf("f = %.8e, u = %x, buf_xjb=%s, buf_schubfach_xjb=%s \n", f, u, buf_xjb, buf_schubfach_xjb);
        // exit(0);
    }
    return 1;
}
unsigned check_xjb32_and_schubfach32_xjb_string(float f)
{
    return check_xjb32_and_schubfach32_xjb_string_jsonformat(f);

    u32 u = *(u32 *)&f;
    static char buf_xjb[32];
    static char buf_xjb_comp[32];
    static char buf_schubfach_xjb[32];
    memset(buf_xjb, 0, 32);
    memset(buf_xjb_comp, 0, 32);
    memset(buf_schubfach_xjb, 0, 32);
    char *end_buf_xjb = xjb32_f32_to_str(f, buf_xjb);
    char *end_buf_xjb_comp = xjb32_comp_f32_to_str(f, buf_xjb_comp);
    char *end_buf_schubfach_xjb = schubfach_xjb_f32_to_str(f, buf_schubfach_xjb);
    int len_xjb = end_buf_xjb - buf_xjb;
    int len_xjb_comp = end_buf_xjb_comp - buf_xjb_comp;
    int len_schubfach_xjb = end_buf_schubfach_xjb - buf_schubfach_xjb;
    if (len_xjb_comp != len_schubfach_xjb || len_xjb != len_schubfach_xjb)
    {
        // printf("f = %.8le, u = %x, len_xjb=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_schubfach_xjb=%s , %s\n",f,u,len_xjb,len_schubfach_xjb,buf_xjb,buf_schubfach_xjb , (u>>23) ? "normal" : "subnormal");
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);
        printf("f = %.8le, u = %x, len_xjb=%d , len_xjb_comp=%d , len_schubfach_xjb=%d , buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s , %s\n", f, u, len_xjb, len_xjb_comp, len_schubfach_xjb, buf_xjb, buf_xjb_comp, buf_schubfach_xjb, (u >> 23) ? "normal" : "subnormal");
        // exit(0);
        return 1;
    }
    if (memcmp(buf_xjb_comp, buf_schubfach_xjb, len_xjb_comp) == 0 && memcmp(buf_xjb, buf_schubfach_xjb, len_xjb) == 0)
    {
        // if three string equal return OK;
        return 0;
    }
    else
    {
        // printf("f = %.8le, u = %x, buf_xjb=%s, buf_schubfach_xjb=%s\n",f,u,buf_xjb,buf_schubfach_xjb);
        //  unsigned int dec, dec_xjb,dec_xjb_comp;
        //  int e10, e10_xjb,e10_xjb_comp;
        //  schubfach_xjb_f32_to_dec(f, &dec, &e10);
        //  xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
        //  printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);

        printf("f = %.8le, u = %x, buf_xjb=%s, buf_xjb_comp=%s, buf_schubfach_xjb=%s \n", f, u, buf_xjb, buf_xjb_comp, buf_schubfach_xjb);
        // exit(0);
    }
    return 1;
}

void check_subnormal()
{
    unsigned error_sum = 0;
    const unsigned long NUM = 10000 * 10000;
    for (unsigned long i = 0; i < NUM; ++i)
    {
        u64 rnd = gen() & ((1ull << 52) - 1);
        double d = *(double *)&rnd;
        error_sum += check_xjb64_and_schubfach_xjb(d);
    }
    if (error_sum == 0)
    {
        printf("check_subnormal ok\n");
    }
    else
    {
        printf("check_subnormal fail error sum = %u\n", error_sum);
    }
}
void check_all_irregular_float_number_to_decimal()
{
    printf("check xjb32 algorithm ; check all irregular float number start\n");
    u64 error_sum = 0;
    auto t1 = getns();
    for (u32 i = 1; i <= 254; ++i)
    {
        u32 u = i << 23;
        float f = *(float *)&u;
        u32 error = check_xjb32_and_schubfach32_xjb(f);
        // if( (i & ((1u<<26) - 1)) == 0) {
        //     printf("check float number progress : %.3lf %% \r", (double)i * (100.0 / (double)0x7F7FFFFF)  );
        // }
        error_sum += error;
        if (error)
        {
            printf("i=%u, i=%x, f=%.8e, error=%u\n", i, i, f, error);
        }
    }
    auto t2 = getns();
    if (error_sum == 0)
    {
        printf("check_all_irregular_float ok, cost %.3lf second\n", (t2 - t1) / 1e9);
    }
    else
    {
        printf("check_all_irregular_float fail error sum = %llu , cost %.3lf second\n", (unsigned long long)error_sum, (double)(t2 - t1) / 1e9);
    }
}
void check_all_float_number_to_decimal()
{
    printf("check xjb32 algorithm ; about one minute, check all float number start\n");
    u64 error_sum = 0;
    auto t1 = getns();
    for (u32 i = 0x00000001u; i <= 0x7F7FFFFFu; ++i)
    {
        float f = *(float *)&i;
        error_sum += check_xjb32_and_schubfach32_xjb(f);
    }
    auto t2 = getns();
    if (error_sum == 0)
    {
        printf("check_all_float ok, cost %.3lf second\n", (t2 - t1) / 1e9);
    }
    else
    {
        printf("check_all_float fail error sum = %llu , cost %.3lf second\n", (unsigned long long)error_sum, (double)(t2 - t1) / 1e9);
    }
}
void check_all_float_number_to_string()
{
    printf("check xjb32 algorithm ; about one minute, check all float number start\n");
    u64 error_sum = 0;
    auto t1 = getns();
    // for (u32 i = 0x00000001u; i <= 0x7F7FFFFFu; ++i)//not contain 0,nan,inf
    for (u32 i = 0x00000000u; i <= 0x7F7FFFFFu + 2; ++i) // contain 0,nan,inf
    {
        float f = *(float *)&i;
        error_sum += check_xjb32_and_schubfach32_xjb_string(f); // about 80 second on apple M1 ; 60 second on AMD R7 7840H
    }
    auto t2 = getns();
    if (error_sum == 0)
    {
        printf("check_all_float ok, cost %.3lf second\n", (t2 - t1) / 1e9);
    }
    else
    {
        printf("check_all_float fail error sum = %llu , cost %.3lf second\n", (unsigned long long)error_sum, (double)(t2 - t1) / 1e9);
    }
}
void check_f2e_xjb()
{
    printf("check f2e_xjb algorithm ; about one minute, check all float number start\n");
    auto t1 = getns();
    for (u32 i = 0x00000001u; i <= 0x7F7FFFFFu; ++i) // contain 0,nan,inf
    {
        float f = *(float *)&i;
        static char buffer_xjb[32];
        char *end_buf_xjb = f2e_xjb_f32_to_str(f, buffer_xjb);
        float f2 = atof(buffer_xjb);
        if (i != *(u32 *)&f2)
        {
            printf("i=%u, i=%x, f=%.8e, f2=%.8e, buffer_xjb=%s\n", i, i, f, (float)f2, buffer_xjb);
        }
    }
    auto t2 = getns();
    printf("check_f2e_xjb ok, cost %.3lf second\n", (t2 - t1) / 1e9);
}
void check_d2e_xjb()
{
    printf("check d2e_xjb algorithm start: n");
    auto t1 = getns();
    const u64 N = (u64)1e8;
    for (u64 i = 1; i <= N; ++i)
    {
        double f = gen_double_filter_NaN_Inf();
        static char buffer_xjb[64];
        char *end_buf_xjb = d2e_xjb_f64_to_str(f, buffer_xjb);
        double f2 = atof(buffer_xjb);
        u64 u = *(u64 *)&f;
        if (u != *(u64 *)&f2)
        {
            printf("u=%llu, u=%llx, f=%.16le, f2=%.16le, buffer_xjb=%s\n", u, u, f, f2, buffer_xjb);
            break;
        }
    }
    auto t2 = getns();
    printf("check_d2e_xjb ok, cost %.3lf second\n", (t2 - t1) / 1e9);
}
void check_all_irregular_float_number_to_string()
{
    printf("check xjb32 algorithm :\n");
    u64 error_sum = 0;
    auto t1 = getns();
    for (u32 i = 0; i <= 246; ++i) // contain 0
    {
        u32 u = i << 23;
        float f = *(float *)&u;
        int is_error = check_xjb32_and_schubfach32_xjb_string(f);
        if (is_error)
            printf("exp=%d\n", (int)i);
        error_sum += is_error;
    }
    auto t2 = getns();
    if (error_sum == 0)
    {
        printf("check_all_irregular_float ok, cost %.3lf second\n", (t2 - t1) / 1e9);
    }
    else
    {
        printf("check_all_irregular_float fail error sum = %llu , cost %.3lf second\n", (unsigned long long)error_sum, (double)(t2 - t1) / 1e9);
    }
}
void check_irregular()
{
    unsigned error_sum = 0;
    for (u64 exp = 1; exp <= 2046; ++exp)
    {
        u64 num = exp << 52;
        double d = *(double *)&num;
        u64 is_error = check_xjb64_and_schubfach_xjb(d);
        error_sum += is_error;
        if (is_error)
        { // error
            printf("exp=%d\n", (int)exp);
        }
    }
    if (error_sum == 0)
    {
        printf("check_irregular ok\n");
    }
    else
    {
        printf("check_irregular fail error sum = %u\n", error_sum);
    }

    // // print irregular exponent bit set
    // u64 bitarray[2048/64]={0};
    // u64 q_num[26]={6,16,66,232,235,245,328,534,657,727,883,926,946,1112,1205,1298,1328,1368,1401,1421,1428,1504,1597,1617,1886,1999};
    // for(unsigned i=0;i<26;++i){
    //     //set_bit(bitarray,q_num[i]);
    //     u64 value = q_num[i];
    //     int arrayIndex = value / 64;
    //     int bitOffset = value % 64;
    //     bitarray[arrayIndex] |= (1ULL << bitOffset);
    // }
    // printf("static uint64_t bitarray[32] = {");
    // for(unsigned i=0;i<32;++i){
    //     printf("0x%016llx",bitarray[i]);
    //     if(i!=31)printf(",");
    //     else printf("};\n");
    // }
}
void check_special_value()
{
    // all double value from https://github.com/xjb714/xjb/blob/main/py_test/test8.py
    u64 num_u64[10] = {
        0x0D17C0747BD76FA1,
        0x0D27C0747BD76FA1,
        0x4D73DE005BD620DF,
        0x4D83DE005BD620DF,
        0x4D93DE005BD620DF,
        0x612491daad0ba280,
        0x6159b651584e8b20,
        0x619011f2d73116f4,
        0x61c4166f8cfd5cb1,
        0x61d4166f8cfd5cb1};
    unsigned error_sum = 0;
    for (int i = 0; i < 10; ++i)
    {
        double num;
        memcpy(&num, &num_u64[i], sizeof(double));
        error_sum += check_xjb64_and_schubfach_xjb(num);
    }
    if (error_sum == 0)
    {
        printf("check_special_value ok\n");
    }
    else
    {
        printf("check_special_value fail error sum = %u\n", error_sum);
    }
}
void check_rand_double()
{
    unsigned long long error_sum = 0;
    const unsigned long NUM = 10000 * 10000; // 1e8
    for (unsigned long i = 0; i < NUM; ++i)
    {
        double d = gen_double_filter_NaN_Inf();
        error_sum += check_xjb64_and_schubfach_xjb(d);
    }
    if (error_sum == 0)
    {
        printf("check_random_double ok\n");
    }
    else
    {
        printf("check_random_double fail error sum = %llu\n", error_sum);
    }
}
void check_rand_integer()
{
    unsigned long long error_sum = 0;
    const unsigned long NUM = 10000 * 10000; // 1e8
    for (unsigned long i = 0; i < NUM; ++i)
    {
        u64 u = gen();
        double d = double(u);
        error_sum += check_xjb64_and_schubfach_xjb(d);
    }
    if (error_sum == 0)
    {
        printf("check_random_integer ok\n");
    }
    else
    {
        printf("check_random_integer fail error sum = %llu\n", error_sum);
    }
}

void check_double()
{
    auto t1 = getns();
    printf("\ncheck start , may cost long time , please wait\n");
#if BENCH_STR
    printf("<=== check xjb64(double to string) algorithm ; use schubfach_xjb for correct result ===>\n");
#else
    printf("<=== check xjb64(double to decimal) algorithm ; use schubfach_xjb for correct result ===>\n");
#endif
    check_special_value(); // ten special double value
    check_irregular();     // all irregular double.
    check_subnormal();     // random subnormal double
    check_rand_double();   // random double
    check_rand_integer();  // random integer
    check_d2e_xjb();       // 
    auto t2 = getns();
    printf("check finish, check_double cost %.3lf second\n", (t2 - t1) / 1e9);
}
void check_float()
{
    check_all_float_number_to_decimal();

    check_all_float_number_to_string();

    check_f2e_xjb();
}
int main()
{
    check_float();

    check_double();

}
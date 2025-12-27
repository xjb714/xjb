#include <stdint.h>
#include <chrono>
#include <iostream>
#include <random>

#define FLOAT 0
#define DOUBLE 1

#ifndef PERF_DOUBLE_OR_FLOAT  // 1 : double ; 0 : float
    #define PERF_DOUBLE_OR_FLOAT 0
#endif


#include "../xjb.cpp"
// #include "xjb64.cpp"
//  char *xjb64(double value, char *buffer);
//  char *xjb32(float value, char *buffer);
// #include "dtoa_xjb64_xjb32.cpp"
//#include "dtoa_xjb_comp.cpp"

typedef uint64_t u64;

std::random_device rd;
std::mt19937_64 gen(rd());
const u64 N = 100'000'000; // data size
#if PERF_DOUBLE_OR_FLOAT == FLOAT
    float *data;
#else
    double *data;
#endif
u64 get_cycle()
{
#ifdef __amd64__
    uint64_t low, high;
    __asm volatile("rdtsc" : "=a"(low), "=d"(high));
    return (high << 32) | low;
#else
    return 0;
#endif
}
auto getns()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    return nanos;
}
double gen_double_filter_NaN_Inf()
{
    unsigned long long rnd,rnd_abs;
    do{
        rnd = gen() ;
        rnd_abs = rnd & ((1ull << 63) - 1);
    }
    while (rnd_abs >= (0x7ffull << 52)); // nan or inf
    return *(double *)&rnd;
}
double gen_double_filter_NaN_Inf_subnormal()
{
    unsigned long long rnd,rnd_abs;
    do{
        rnd = gen() ;
        rnd_abs = rnd & ((1ull << 63) - 1);
    }
    while (rnd_abs >= (0x7ffull << 52) && rnd_abs < (1ull << 52) ); // nan or inf or subnormal
    return *(double *)&rnd;
}
float gen_float_filter_NaN_Inf()
{
    unsigned int rnd,rnd_abs;
    do{
        rnd = gen() ;
        rnd_abs = rnd & ((1u << 31) - 1);
    }
    while (rnd_abs >= (0xffu << 23)); // nan or inf
    return *(float *)&rnd;
}
float gen_float_filter_NaN_Inf_subnormal()
{
    unsigned int rnd,rnd_abs;
    do{
        rnd = gen();
        rnd_abs = rnd & ((1u << 31) - 1);
    }
    while (rnd_abs >= (0xffu << 23) && rnd_abs < (1u << 23) ); // nan or inf or subnormal
    return *(float *)&rnd;
}
void init_data()
{
#if PERF_DOUBLE_OR_FLOAT == FLOAT
    data = new float[N];
    for (u64 i = 0; i < (N); ++i)
    {
        data[i] = gen_float_filter_NaN_Inf();
    }
#else
    data = new double[N];
    for (u64 i = 0; i < (N); ++i)
    {
        data[i] = gen_double_filter_NaN_Inf();
    }
#endif
}
int main()
{
    const int bench_or_check = 1; // 1 : bench ; 0 : check
    if (bench_or_check)
    {
        char buf[64];
        // char *buf = (char*)malloc(64);
        //init_data();
        u64 random_seed = gen();
        auto t1 = getns();
        auto c1 = get_cycle();
        //double num;
        u64 random_num = random_seed;
        for (u64 i = 0; i < N; ++i)
        {
            // num = gen_double_filter_NaN_Inf();
            random_num += random_seed;
            //num = *(double*)&random_num;
            
            //double num = data[i];
            //xjb64_32_comp::xjb64(num, buf);

            // float num = *(float*)&random_num;
            // xjb64_32_comp::xjb32(num,buf);
#if PERF_DOUBLE_OR_FLOAT == FLOAT
            float num = *(float*)&random_num;
            //float num = data[i];
            xjb32(num,buf);
#else
            double num = *(double *)&random_num;
            xjb64(num, buf);
#endif
        }
        auto c2 = get_cycle();
        auto t2 = getns();
#ifdef __amd64__
        printf("every number cost %lf ns,%lf cycle\n", (double)(t2 - t1) * (1.0 / double(N)), (double)(c2 - c1) * (1.0 / double(N)));
#else
        printf("every number cost %lf ns\n", (double)(t2 - t1) * (1.0 / double(N)));
#endif

#if PERF_DOUBLE_OR_FLOAT == FLOAT
        printf("data = %.8e \n buf = %s \n", *(float *)&random_num, buf);
#else
        printf("data = %.16le \n buf = %s \n", *(double *)&random_num, buf);
#endif
    }

    // if (!bench_or_check)
    // {
    //     int i = 0;
    //     while (i++ < 30)
    //     {
    //         double num;
    //         printf("input a real number:");
    //         scanf("%le", &num);
    //         char buf[64];
    //         // for(int j=0;j<32;j++)buf[j]=0;
    //         // char* end_buf = xjb64_32::xjb32(num, buf);
    //         //char *end_buf = xjb64_32_comp::xjb64(num, buf);
    //         //printf("len = %d, input num = %.16le \n buf = %s \n", int(end_buf - buf), num, buf);
    //     }
    // }
}
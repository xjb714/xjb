#include <stdio.h>
#include <vector>
#include <chrono>
#include <random>
#include <stdint.h>

#ifndef BENCH_DOUBLE
    #define BENCH_DOUBLE 1
#endif

#define BENCH_FLOAT !BENCH_DOUBLE

#ifndef BENCH_STR
    #define BENCH_STR 1
#endif


const int is_bench_double_to_decimal = !BENCH_STR;
const int is_bench_float_to_decimal = !BENCH_STR;

const int is_bench_double_to_string = BENCH_STR;
const int is_bench_float_to_string = BENCH_STR;

// double and float
#include "schubfach/schubfach_i.hpp"
#include "schubfach_xjb/schubfach_xjb_i.hpp"
#include "dragonbox/dragonbox_i.hpp"
#include "ryu/ryu_i.hpp"
#include "teju/teju_i.hpp"
#include "yy/yy_i.hpp"
#include "xjb/xjb64_i.hpp"
#include "xjb/dtoa_xjb64_xjb32.cpp"
#include "xjb/dtoa_xjb_comp.cpp"
#include "fmt/fmt_i.hpp"
// #include "ldouble/ldouble_i.hpp"
// #include "json/jnum.c"

// float
#include "xjb/xjb32_i.hpp"

const int N = (int)(1e7);
const int N_double = N; // double data size
const int N_float = N;  // float data size

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;


// double
double *data;
unsigned long long *dec;
int *e10;
// dec * 10^e10

// float
float *data_float; // all random float data range : [1 , ((0xff<<23)-1)]
unsigned int *dec_float;
int *e10_float;
// dec_float * 10^e10_float

std::vector<std::pair<std::string, void (*)(double, unsigned long long *, int *)>> double_to_decimal_algorithm_set; // double
std::vector<std::pair<std::string, void (*)(float, unsigned int *, int *)>> float_to_decimal_algorithm_set;         // float

std::vector<std::string> double_to_string_algorithm_set;
std::vector<std::string> float_to_string_algorithm_set;

std::random_device rd;
std::mt19937_64 gen(rd());

// unsigned long long get_cycle() // x86 , gcc
// {
//     uint64_t low, high;
//     __asm volatile("rdtsc" : "=a"(low), "=d"(high));
//     return (high << 32) | low;
// }

unsigned long long getns()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    return nanos;
}
double gen_double_filter_NaN_Inf()
{
    unsigned long long rnd;
    do
        rnd = gen() & ((1ull << 63) - 1); // abs
    while (rnd >= (0x7ffull << 52)); // nan or inf
    // while ((  (rnd & ((1ull << 63) - 1)) >= (0x7ffull << 52)) || (  (rnd & ((1ull << 63) - 1)) < (1ull << 52)));//subnormal or nan of inf

    return *(double *)&rnd;
}
void init_double()
{
    auto t1 = getns();

    data = (double *)malloc(N_double * sizeof(double));
    dec = (unsigned long long *)malloc(N_double * sizeof(unsigned long long));
    e10 = (int *)malloc(N_double * sizeof(int));

    // #pragma omp parallel for
    for (int i = 0; i < (N_double); ++i)
    {
        data[i] = gen_double_filter_NaN_Inf();
    }
    printf("generate random data finish\n");

    memset(&dec[0], 0, N_double * sizeof(unsigned long long));
    memset(&e10[0], 0, N_double * sizeof(int));

    double_to_decimal_algorithm_set.clear();

    double_to_decimal_algorithm_set.push_back({std::string("schubfach64"), schubfach_f64_to_dec});         // 0
    double_to_decimal_algorithm_set.push_back({std::string("schubfach64_xjb"), schubfach_xjb_f64_to_dec}); // 1
    double_to_decimal_algorithm_set.push_back({std::string("dragonbox"), dragonbox_f64_to_dec});           // 2
    double_to_decimal_algorithm_set.push_back({std::string("ryu64"), ryu_f64_to_dec});                     // 3
    double_to_decimal_algorithm_set.push_back({std::string("teju"), teju_f64_to_dec});                     // 4
    double_to_decimal_algorithm_set.push_back({std::string("yyjson"), yyjson_f64_to_dec});                 // 5
    double_to_decimal_algorithm_set.push_back({std::string("yy_double"), yy_double_f64_to_dec});           // 6
    double_to_decimal_algorithm_set.push_back({std::string("yy_double_full"), yy_double_full_f64_to_dec}); // 7
    double_to_decimal_algorithm_set.push_back({std::string("xjb64"), xjb64_f64_to_dec});                   // 8
    double_to_decimal_algorithm_set.push_back({std::string("xjb64_comp"), xjb64_comp_f64_to_dec});         // 9

    double_to_string_algorithm_set.clear();

    double_to_string_algorithm_set.push_back(std::string("ryu64"));           // 0
    double_to_string_algorithm_set.push_back(std::string("schubfach64"));     // 1
    double_to_string_algorithm_set.push_back(std::string("schubfach_xjb64")); // 2
    double_to_string_algorithm_set.push_back(std::string("yy_double"));       // 3
    double_to_string_algorithm_set.push_back(std::string("yyjson"));          // 4
    double_to_string_algorithm_set.push_back(std::string("dragonbox_comp"));  // 5
    double_to_string_algorithm_set.push_back(std::string("dragonbox_full"));  // 6
    double_to_string_algorithm_set.push_back(std::string("fmt_comp"));        // 7
    double_to_string_algorithm_set.push_back(std::string("fmt_full"));        // 8
    double_to_string_algorithm_set.push_back(std::string("xjb64"));           // 9
    double_to_string_algorithm_set.push_back(std::string("xjb64_comp"));      // 10

    // algorithm_set.push_back({std::string("ldouble"), ldouble_f64_to_dec});

    auto t2 = getns();
    printf("init : cost %lf second\n", (t2 - t1) / 1e9);
}
void init_float()
{
    auto t1 = getns();
    const int N = N_float;
    data_float = (float *)malloc(N * sizeof(float));
    dec_float = (unsigned int *)malloc(N * sizeof(unsigned int));
    e10_float = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; ++i)
    {
        // generate random float data : [1 , ((0xff<<23)-1)]
        unsigned int rnd;
        do
            rnd = gen() & ((1u << 31) - 1);
        while (rnd >= (0xffu << 23) || rnd == 0); // 0 or nan or inf
        data_float[i] = *(float *)&rnd;
    }
    printf("generate random float data finish\n");

    memset(&dec_float[0], 0, N * sizeof(unsigned int));
    memset(&e10_float[0], 0, N * sizeof(int));

    auto t2 = getns();
    printf("init float : cost %lf second\n", (t2 - t1) / 1e9);

    float_to_decimal_algorithm_set.clear();

    float_to_decimal_algorithm_set.push_back({std::string("xjb32"), xjb_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("xjb32_comp"), xjb_comp_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("schubfach32_xjb"), schubfach_xjb_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("schubfach32"), schubfach_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("ryu32"), ryu_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("teju32"), teju_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("yyjson32"), yyjson_f32_to_dec});
    float_to_decimal_algorithm_set.push_back({std::string("dragonbox32"), dragonbox_f32_to_dec});

    float_to_string_algorithm_set.clear();

    float_to_string_algorithm_set.push_back(std::string("ryu32"));
    float_to_string_algorithm_set.push_back(std::string("schubfach32"));
    float_to_string_algorithm_set.push_back(std::string("schubfach_xjb32"));
    float_to_string_algorithm_set.push_back(std::string("xjb32"));
    float_to_string_algorithm_set.push_back(std::string("xjb32_comp"));
    float_to_string_algorithm_set.push_back(std::string("yyjson32"));
    float_to_string_algorithm_set.push_back(std::string("dragonbox_comp32"));
    float_to_string_algorithm_set.push_back(std::string("dragonbox_full32"));
    float_to_string_algorithm_set.push_back(std::string("fmt_comp32"));
    float_to_string_algorithm_set.push_back(std::string("fmt_full32"));

    printf("init float algorithm set finish\n");
}
void free_double()
{
    if (data)
        free(data);
    if (dec)
        free(dec);
    if (e10)
        free(e10);
    data = nullptr;
    dec = nullptr;
    e10 = nullptr;
}
void free_float()
{
    if (data_float)
        free(data_float);
    if (dec_float)
        free(dec_float);
    if (e10_float)
        free(e10_float);
    data_float = nullptr;
    dec_float = nullptr;
    e10_float = nullptr;
}
void bench_double_single_impl(int i)
{
    const int N = N_double;
    std::string name;
    if (is_bench_double_to_decimal)
        name = double_to_decimal_algorithm_set[i].first;
    else if (is_bench_double_to_string)
        name = double_to_string_algorithm_set[i];
    else
        name = "unknown";

    // void (*func)(double, unsigned long long *, int *) = double_to_decimal_algorithm_set[i].second;
    unsigned long long *dec_p = &dec[0];
    int *e10_p = &e10[0];
    printf("%d. bench %16s : ", i, name.c_str());

    auto t1 = getns();

    // This method has additional overhead,
    // which affects the test results to some extent,
    // so this method is not used.
    // for(int j=0;j<N;++j)func(data[j],&dec_p[j],&e10_p[j]);

    if (is_bench_double_to_decimal)
    {
        if (i == 0)
            for (int j = 0; j < N; ++j)
                schubfach_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 1)
            for (int j = 0; j < N; ++j)
                schubfach_xjb_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 2)
            for (int j = 0; j < N; ++j)
                dragonbox_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 3)
            for (int j = 0; j < N; ++j)
                ryu_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 4)
            for (int j = 0; j < N; ++j)
                teju_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 5)
            for (int j = 0; j < N; ++j)
                yyjson_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 6)
            for (int j = 0; j < N; ++j)
                yy_double_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 7)
            for (int j = 0; j < N; ++j)
                yy_double_full_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 8)
            for (int j = 0; j < N; ++j)
                xjb64_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
        if (i == 9)
            for (int j = 0; j < N; ++j)
                xjb64_comp_f64_to_dec(data[j], &dec_p[j], &e10_p[j]);
    }

    if (is_bench_double_to_string)
    {
        char buffer[128];
        if (i == 0)
            for (int j = 0; j < N; ++j)
                ryu_f64_to_str(data[j], buffer);
        if (i == 1)
            for (int j = 0; j < N; ++j)
                schubfach_f64_to_str(data[j], buffer);
        if (i == 2)
            for (int j = 0; j < N; ++j)
                schubfach_xjb_f64_to_str(data[j], buffer);
        if (i == 3)
            for (int j = 0; j < N; ++j)
                yy_double_f64_to_str(data[j], buffer);
        if (i == 4)
            for (int j = 0; j < N; ++j)
                yyjson_f64_to_str(data[j], buffer);
        if (i == 5)
            for (int j = 0; j < N; ++j)
                dragonbox_comp_f64_to_str(data[j], buffer);
        if (i == 6)
            for (int j = 0; j < N; ++j)
                dragonbox_full_f64_to_str(data[j], buffer);
        if (i == 7)
            for (int j = 0; j < N; ++j)
                fmt_comp_f64_to_str(data[j], buffer);
        if (i == 8)
            for (int j = 0; j < N; ++j)
                fmt_full_f64_to_str(data[j], buffer);
        if (i == 9)
            for (int j = 0; j < N; ++j)
                xjb64_32::xjb64(data[j], buffer);
        if (i == 10)
            for (int j = 0; j < N; ++j)
                xjb64_32_comp::xjb64(data[j], buffer);
    }

    auto t2 = getns();
    for (int j = 0; j < N; ++j)
    {
        volatile unsigned int d = dec_p[j];
        volatile int e = e10_p[j];
        (void)d;
        (void)e;
    }
    printf("cost %5.4lf ms,every double cost %3.4lf ns\n", (t2 - t1) / 1e6, (t2 - t1) * (1.0 / N));
}
void bench_float_single_impl(int i)
{
    const int N = N_float;
    std::string name;
    if (is_bench_double_to_decimal)
        name = float_to_decimal_algorithm_set[i].first;
    else if (is_bench_float_to_string)
        name = float_to_string_algorithm_set[i];
    else
        name = "unknown";
    // void (*func)(float*, unsigned int *, int *) = algorithm_float_set[i].second;
    unsigned int *dec_p = &dec_float[0];
    int *e10_p = &e10_float[0];
    printf("%d. bench %16s : ", i, name.c_str());

    auto t1 = getns();

    // This method has additional overhead,
    // which affects the test results to some extent,
    // so this method is not used.
    // for(int j=0;j<N;++j)func(data[j],&dec_p[j],&e10_p[j]);

    if (is_bench_float_to_decimal)
    {
        if (i == 0)
            for (int j = 0; j < N; ++j)
                xjb_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 1)
            for (int j = 0; j < N; ++j)
                xjb_comp_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 2)
            for (int j = 0; j < N; ++j)
                schubfach_xjb_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 3)
            for (int j = 0; j < N; ++j)
                schubfach_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 4)
            for (int j = 0; j < N; ++j)
                ryu_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 5)
            for (int j = 0; j < N; ++j)
                teju_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 6)
            for (int j = 0; j < N; ++j)
                yyjson_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
        if (i == 7)
            for (int j = 0; j < N; ++j)
                dragonbox_f32_to_dec(data_float[j], &dec_p[j], &e10_p[j]);
    }

    if (is_bench_float_to_string)
    {
        char buffer[128];
        if (i == 0)
            for (int j = 0; j < N; ++j)
                ryu_f32_to_str(data_float[j], buffer);
        if (i == 1)
            for (int j = 0; j < N; ++j)
                schubfach_f32_to_str(data_float[j], buffer);
        if (i == 2)
            for (int j = 0; j < N; ++j)
                schubfach_xjb_f32_to_str(data_float[j], buffer);
        if (i == 3)
            for (int j = 0; j < N; ++j)
                xjb64_32::xjb32(data_float[j], buffer);
        if (i == 4)
            for (int j = 0; j < N; ++j)
                xjb64_32_comp::xjb32(data_float[j], buffer);
        if (i == 5)
            for (int j = 0; j < N; ++j)
                yyjson_f32_to_str(data_float[j], buffer);
        if (i == 6)
            for (int j = 0; j < N; ++j)
                dragonbox_comp_f32_to_str(data_float[j], buffer);
        if (i == 7)
            for (int j = 0; j < N; ++j)
                dragonbox_full_f32_to_str(data_float[j], buffer);
        if (i == 8)
            for (int j = 0; j < N; ++j)
                fmt_comp_f32_to_str(data_float[j], buffer);
        if (i == 9)
            for (int j = 0; j < N; ++j)
                fmt_full_f32_to_str(data_float[j], buffer);
    }

    auto t2 = getns();
    for (int j = 0; j < N; ++j)
    {
        volatile unsigned int d = dec_p[j];
        volatile int e = e10_p[j];
        (void)d;
        (void)e;
    }
    printf("cost %5.4lf ms,every float cost %3.4lf ns\n", (t2 - t1) / 1e6, (t2 - t1) * (1.0 / N));
}

void bench_double_all_algorithm()
{

    if (is_bench_double_to_decimal)
    {
        printf("bench double to decimal start\n");
        for (int i = 0; i < double_to_decimal_algorithm_set.size(); ++i)
        {
            bench_double_single_impl(i);
        }
        printf("bench double to decimal end\n");
    }
    if (is_bench_double_to_string)
    {
        printf("bench double to string start\n");
        for (int i = 0; i < double_to_string_algorithm_set.size(); ++i)
        {
            bench_double_single_impl(i);
        }
        printf("bench double to string end\n");
    }
}
void bench_float_all_algorithm()
{
    if (is_bench_float_to_decimal)
    {
        printf("bench float to decimal start\n");
        for (int i = 0; i < float_to_decimal_algorithm_set.size(); ++i)
        {
            bench_float_single_impl(i);
        }
        printf("bench float to decimal end\n");
    }
    if (is_bench_float_to_string)
    {
        printf("bench float to string start\n");
        for (int i = 0; i < float_to_string_algorithm_set.size(); ++i)
        {
            bench_float_single_impl(i);
        }
        printf("bench float to string end\n");
    }
}

unsigned check_xjb64_and_schubfach_xjb(double d)
{
    // use schubfach_xjb as reference implementation
    unsigned long long dec, dec_xjb,dec_xjb_comp;
    int e10, e10_xjb,e10_xjb_comp;
    schubfach_xjb_f64_to_dec(d, &dec, &e10);
    // schubfach_f64_to_dec(d,&dec_xjb,&e10_xjb); // schubfach_xjb and schubfach result always same
    xjb64_f64_to_dec(d, &dec_xjb, &e10_xjb);
    xjb64_comp_f64_to_dec(d, &dec_xjb_comp, &e10_xjb_comp);
    if ((dec == dec_xjb && e10 == e10_xjb && dec == dec_xjb_comp && e10 == e10_xjb_comp))
    {
        return 0;
    }else{
        //printf("f = %.16le, dec=%llu,e10=%d , dec_xjb=%llu,e10_xjb=%d, dec_xjb_comp=%llu,e10_xjb_comp=%d,",d,dec,e10,dec_xjb,e10_xjb,dec_xjb_comp,e10_xjb_comp);
        //exit(0);
    }
    return 1;
}
unsigned check_xjb32_and_schubfach32_xjb(float f)
{
    unsigned int dec, dec_xjb,dec_xjb_comp;
    int e10, e10_xjb,e10_xjb_comp;
    schubfach_xjb_f32_to_dec(f, &dec, &e10);
    xjb_f32_to_dec(f, &dec_xjb, &e10_xjb);
    xjb_comp_f32_to_dec(f, &dec_xjb_comp, &e10_xjb_comp);
    if ((dec == dec_xjb && e10 == e10_xjb && dec == dec_xjb_comp && e10 == e10_xjb_comp))
    {
        return 0;
    }
    else
    {
        //printf("");
        //printf("f = %.8le, dec=%u,e10=%d , dec_xjb=%u,e10_xjb=%d\n",f,dec,e10,dec_xjb,e10_xjb);
        //exit(0);
    }
    return 1;
}

void check_subnormal()
{
    unsigned error_sum = 0;
    const unsigned long NUM = 1 << 30;
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
void check_all_float_number()
{
    printf("check xjb32 algorithm ; check all float number start\n");
    unsigned error_sum = 0;
    for (u32 i = 0x00000001u; i <= 0x7F7FFFFFu; ++i)
    {
        float f = *(float *)&i;
        error_sum += check_xjb32_and_schubfach32_xjb(f);
    }
    if (error_sum == 0)
    {
        printf("check_all_float ok\n");
    }
    else
    {
        printf("check_all_float fail error sum = %u\n", error_sum);
    }
}
void check_float()
{
    // not contain subnormal float
    // because subnormal float convert to double , it will be 0.
    unsigned error_sum = 0;
    for (u32 i = 0x00800000u; i <= 0x7F7FFFFFu; ++i) // 2**31
    {
        float f = *(float *)&i;
        double d = f; // convert float to double
        error_sum += check_xjb64_and_schubfach_xjb(d);
    }
    if (error_sum == 0)
    {
        printf("check_float ok\n");
    }
    else
    {
        printf("check_float fail error sum = %u\n", error_sum);
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
    //     int arrayIndex = value / 64;      // 确定在哪个uint64_t中
    //     int bitOffset = value % 64;       // 确定在uint64_t中的哪一位
    //     bitarray[arrayIndex] |= (1ULL << bitOffset);
    // }
    // printf("static uint64_t bitarray[32] = {");
    // for(unsigned i=0;i<32;++i){
    //     printf("0x%016llx",bitarray[i]);
    //     if(i!=31)printf(",");
    //     else printf("};\n");
    // }
}
void check_special_value(){
    // all double value from https://github.com/xjb714/xjb/blob/main/py_test/test8.py
    u64 num_u64[10]={
                0x0D17C0747BD76FA1,
                0x0D27C0747BD76FA1,
                0x4D73DE005BD620DF,
                0x4D83DE005BD620DF,
                0x4D93DE005BD620DF,
                0x612491daad0ba280,
                0x6159b651584e8b20,
                0x619011f2d73116f4,
                0x61c4166f8cfd5cb1,
                0x61d4166f8cfd5cb1
            };
    unsigned error_sum = 0;
    for(int i=0;i<10;++i){
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
    unsigned long error_sum = 0;
    const unsigned long NUM = 1 << 30; // 1e9
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
        printf("check_random_double fail error sum = %u\n", error_sum);
    }
}

void check_double()
{
    printf("\ncheck start , may cost long time , please wait\n");
    printf("<=== check xjb64 algorithm ; use schubfach_xjb for correct result ===>\n");
    check_special_value();
    check_subnormal();
    check_float(); // not contain subnormal float  , very slow
    check_irregular();
    check_rand_double(); // random double
    printf("check finish\n");
}

void bench_double()
{
    init_double();

    bench_double_all_algorithm();

    free_double();
}
void bench_float()
{
    init_float();

    bench_float_all_algorithm();

    free_float();
}
int main()
{

#if BENCH_FLOAT
    bench_float();

    check_all_float_number(); // check all float number , may cost long time
#endif

#if BENCH_DOUBLE
    bench_double();

    check_double(); // check double correctness , may cost long time
#endif

    return 0;
}
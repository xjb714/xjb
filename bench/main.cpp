#include <stdio.h>
#include <vector>
#include <chrono>
#include <random>
#include <stdint.h>
#include <filesystem> //c++17

#include "util/get_cpu_name.cpp"

// #include "util/check_float_multi_thread.cpp" // use multi-thread to check float algorithm

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define USE_YYBENCH 1

#if USE_YYBENCH
#include "util/yybench/src/yybench.cpp"
#include "util/benchmark.cpp"
#endif

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

const int CHECK_CORRECTNESS = 0;

const u64 LOOP_UNROLL = 2;

// double and float algorithm set
#include "schubfach/schubfach_i.hpp"
// #include "schubfach_vitaut/schubfach_vitaut_i.hpp"
#include "schubfach_xjb/schubfach_xjb_i.hpp"
#include "dragonbox/dragonbox_i.hpp"
#include "ryu/ryu_i.hpp"
#include "teju/teju_i.hpp"
#include "yy/yy_i.hpp"
#include "xjb/xjb_i.hpp"
#include "fmt/fmt_i.hpp"
#include "zmij/zmij_i.hpp"
#include "jnum/jnum_i.hpp" // not satisfy the Steele and White algorithm
#include "uscalec/uscalec_i.hpp"

const u64 N = (1 << 24); // about 16M
const u64 N_double = N;  // double data size
const u64 N_float = N;   // float data size

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

uint64_t get_cycle() // x86 , gcc
{
    uint64_t low, high, cycles;
#if defined(__amd64__) && (defined(__GNUC__) || defined(__clang__))
    __asm volatile("rdtsc" : "=a"(low), "=d"(high));
    return (high << 32) | low;
// #elif defined(__aarch64__)
//     //__asm volatile("mrs %0, pmccntr_el0" : "=r"(cycles));
//     //__asm volatile("mrs %0, cntpct_el0" : "=r"(cycles));
//     //__asm volatile("mrs %0, cntvct_el0" : "=r"(cycles));
//     return cycles;
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
    unsigned long long rnd, rnd_abs;
    do
    {
        rnd = gen();
        rnd_abs = rnd & ((1ull << 63) - 1);
    } while (rnd_abs >= (0x7ffull << 52)); // nan or inf
    return *(double *)&rnd;
}
double gen_double_filter_NaN_Inf_subnormal()
{
    unsigned long long rnd, rnd_abs;
    do
    {
        rnd = gen();
        rnd_abs = rnd & ((1ull << 63) - 1);
    } while (rnd_abs >= (0x7ffull << 52) && rnd_abs < (1ull << 52)); // nan or inf or subnormal
    return *(double *)&rnd;
}
float gen_float_filter_NaN_Inf()
{
    unsigned int rnd, rnd_abs;
    do
    {
        rnd = gen();
        rnd_abs = rnd & ((1u << 31) - 1);
    } while (rnd_abs >= (0xffu << 23)); // nan or inf
    return *(float *)&rnd;
}
float gen_float_filter_NaN_Inf_subnormal()
{
    unsigned int rnd, rnd_abs;
    do
    {
        rnd = gen();
        rnd_abs = rnd & ((1u << 31) - 1);
    } while (rnd_abs >= (0xffu << 23) && rnd_abs < (1u << 23)); // nan or inf or subnormal
    return *(float *)&rnd;
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
    fflush(stdout);

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
    double_to_decimal_algorithm_set.push_back({std::string("zmij64"), zmij_f64_to_dec});                   // 10
    double_to_decimal_algorithm_set.push_back({std::string("uscalec"), uscalec_f64_to_dec});               // 11

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
    double_to_string_algorithm_set.push_back(std::string("zmij"));            // 11
    double_to_string_algorithm_set.push_back(std::string("jnum"));            // 12
    double_to_string_algorithm_set.push_back(std::string("d2e_xjb"));         // 13
    double_to_string_algorithm_set.push_back(std::string("uscalec"));         // 14

    auto t2 = getns();
    printf("init : cost %lf second\n", (t2 - t1) / 1e9);
    fflush(stdout);
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
        data_float[i] = gen_float_filter_NaN_Inf();
    }
    printf("generate random float data finish, total sample size : %d \n", N);

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
    float_to_decimal_algorithm_set.push_back({std::string("zmij32"), zmij_f32_to_dec});

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
    float_to_string_algorithm_set.push_back(std::string("zmij32"));
    float_to_string_algorithm_set.push_back(std::string("jnum32"));
    float_to_string_algorithm_set.push_back(std::string("f2e_xjb"));

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
    const u64 N = N_double;
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
    printf("%2d. bench %16s : ", i, name.c_str());

    char *buffer = (char *)malloc(128);
    memset(buffer, 0, 128);
    unsigned long long d;
    int k;
    u64 sum_final = 0;
    auto t1 = getns();
    auto c1 = get_cycle();

    // This method has additional overhead,
    // which affects the test results to some extent,
    // so this method is not used.
    // for(int j=0;j<N;++j)func(data[j],&dec_p[j],&e10_p[j]);

    if (is_bench_double_to_decimal)
    {
#define BENCH_DOUBLE_TO_DECIMAL(n, name)                \
    if (i == n)                                         \
        for (u64 j = 0; j < N; j += LOOP_UNROLL)        \
        {                                               \
            for (u64 u = 0; u < LOOP_UNROLL; u++)       \
            {                                           \
                name##_f64_to_dec(data[j + u], &d, &k); \
                sum_final += d;                         \
            }                                           \
        };

        BENCH_DOUBLE_TO_DECIMAL(0, schubfach)
        BENCH_DOUBLE_TO_DECIMAL(1, schubfach_xjb)
        BENCH_DOUBLE_TO_DECIMAL(2, dragonbox)
        BENCH_DOUBLE_TO_DECIMAL(3, ryu)
        BENCH_DOUBLE_TO_DECIMAL(4, teju)
        BENCH_DOUBLE_TO_DECIMAL(5, yyjson)
        BENCH_DOUBLE_TO_DECIMAL(6, yy_double)
        BENCH_DOUBLE_TO_DECIMAL(7, yy_double_full)
        BENCH_DOUBLE_TO_DECIMAL(8, xjb64_v2)
        BENCH_DOUBLE_TO_DECIMAL(9, xjb64_comp)
        BENCH_DOUBLE_TO_DECIMAL(10, zmij)
        BENCH_DOUBLE_TO_DECIMAL(11, uscalec)
    }

    if (is_bench_double_to_string)
    {
#define BENCH_DOUBLE_TO_STRING(n, name)                                       \
    if (i == n)                                                               \
        for (u64 j = 0; j < N; j += LOOP_UNROLL)                              \
        {                                                                     \
            for (u64 u = 0; u < LOOP_UNROLL; u++)                             \
            {                                                                 \
                sum_final += name##_f64_to_str(data[j + u], buffer) - buffer; \
            }                                                                 \
        };

        BENCH_DOUBLE_TO_STRING(0, ryu)
        BENCH_DOUBLE_TO_STRING(1, schubfach)
        BENCH_DOUBLE_TO_STRING(2, schubfach_xjb)
        BENCH_DOUBLE_TO_STRING(3, yy_double)
        BENCH_DOUBLE_TO_STRING(4, yyjson)
        BENCH_DOUBLE_TO_STRING(5, dragonbox_comp)
        BENCH_DOUBLE_TO_STRING(6, dragonbox_full)
        BENCH_DOUBLE_TO_STRING(7, fmt_comp)
        BENCH_DOUBLE_TO_STRING(8, fmt_full)
        BENCH_DOUBLE_TO_STRING(9, xjb64)
        BENCH_DOUBLE_TO_STRING(10, xjb64_comp)
        BENCH_DOUBLE_TO_STRING(11, zmij)
        BENCH_DOUBLE_TO_STRING(12, jnum)
        BENCH_DOUBLE_TO_STRING(13, d2e_xjb)
        BENCH_DOUBLE_TO_STRING(14, uscalec)
    }
    auto c2 = get_cycle();
    auto t2 = getns();
    uint64_t cycle_sum = c2 - c1;
    double cycle_avg = (double)cycle_sum * (1.0 / N);
#if defined(__amd64__) && (defined(__GNUC__) || defined(__clang__))
    printf("cost %5.4lf ms,every double cost %3.4lf ns ,%3.4lf cycle; buffer=%s, sum = %llu\n", (double)(t2 - t1) * 1e-6, (double)(t2 - t1) * (1.0 / N), cycle_avg, buffer, (unsigned long long)sum_final);
#else
    printf("cost %5.4lf ms,every double cost %3.4lf ns ; buffer=%s, sum = %llu\n", (double)(t2 - t1) * 1e-6, (double)(t2 - t1) * (1.0 / N), buffer, (unsigned long long)sum_final);
#endif
}
void bench_float_single_impl(int i)
{
    const int N = N_float;
    std::string name;
    if (is_bench_float_to_decimal)
        name = float_to_decimal_algorithm_set[i].first;
    else if (is_bench_float_to_string)
        name = float_to_string_algorithm_set[i];
    else
        name = "unknown";
    // void (*func)(float*, unsigned int *, int *) = algorithm_float_set[i].second;
    unsigned int *dec_p = &dec_float[0];
    int *e10_p = &e10_float[0];
    printf("%2d. bench %16s : ", i, name.c_str());

    char *buffer = (char *)malloc(128);
    memset(buffer, 0, 128);
    // u32 d;
    // int k;
    u32 sum_final = 0; // prevent compiler optimization
    auto t1 = getns();
    auto c1 = get_cycle();

    // This method has additional overhead,
    // which affects the test results to some extent,
    // so this method is not used.
    // for(int j=0;j<N;++j)func(data[j],&dec_p[j],&e10_p[j]);

    if (is_bench_float_to_decimal)
    {
#define BENCH_FLOAT_TO_DECIMAL(n, name)                       \
    if (i == n)                                               \
        for (u64 j = 0; j < N; j += LOOP_UNROLL)              \
        {                                                     \
            for (u64 u = 0; u < LOOP_UNROLL; u++)             \
            {                                                 \
                u32 d;                                        \
                int k;                                        \
                name##_f32_to_dec(data_float[j + u], &d, &k); \
                sum_final += d;                               \
            }                                                 \
        };

        BENCH_FLOAT_TO_DECIMAL(0, xjb_v2)
        BENCH_FLOAT_TO_DECIMAL(1, xjb_comp)
        BENCH_FLOAT_TO_DECIMAL(2, schubfach_xjb)
        BENCH_FLOAT_TO_DECIMAL(3, schubfach)
        BENCH_FLOAT_TO_DECIMAL(4, ryu)
        BENCH_FLOAT_TO_DECIMAL(5, teju)
        BENCH_FLOAT_TO_DECIMAL(6, yyjson)
        BENCH_FLOAT_TO_DECIMAL(7, dragonbox)
        BENCH_FLOAT_TO_DECIMAL(8, zmij)
    }
    if (is_bench_float_to_string)
    {
#define BENCH_FLOAT_TO_STRING(n, name)                                              \
    if (i == n)                                                                     \
        for (u64 j = 0; j < N; j += LOOP_UNROLL)                                    \
        {                                                                           \
            for (u64 u = 0; u < LOOP_UNROLL; u++)                                   \
            {                                                                       \
                sum_final += name##_f32_to_str(data_float[j + u], buffer) - buffer; \
            }                                                                       \
        };
        BENCH_FLOAT_TO_STRING(0, ryu)
        BENCH_FLOAT_TO_STRING(1, schubfach)
        BENCH_FLOAT_TO_STRING(2, schubfach_xjb)
        BENCH_FLOAT_TO_STRING(3, xjb32)
        BENCH_FLOAT_TO_STRING(4, xjb32_comp)
        BENCH_FLOAT_TO_STRING(5, yyjson)
        BENCH_FLOAT_TO_STRING(6, dragonbox_comp)
        BENCH_FLOAT_TO_STRING(7, dragonbox_full)
        BENCH_FLOAT_TO_STRING(8, fmt_comp)
        BENCH_FLOAT_TO_STRING(9, fmt_full)
        BENCH_FLOAT_TO_STRING(10, zmij)
        BENCH_FLOAT_TO_STRING(11, jnum)
        BENCH_FLOAT_TO_STRING(12, f2e_xjb)
    }
    auto c2 = get_cycle();
    auto t2 = getns();

    uint64_t cycle_sum = c2 - c1;
    double cycle_avg = (double)cycle_sum * (1.0 / N);
#if defined(__amd64__) && (defined(__GNUC__) || defined(__clang__))
    printf("cost %5.4lf ms,every float cost %3.4lf ns ,%3.4lf cycle; buffer=%s,sum_filnal = %llu\n", (double)(t2 - t1) * 1e-6, (double)(t2 - t1) * (1.0 / N), cycle_avg, buffer, (unsigned long long)sum_final);
#else
    printf("cost %5.4lf ms,every float cost %3.4lf ns ; buffer=%s,sum_final = %llu\n", (double)(t2 - t1) * 1e-6, (double)(t2 - t1) * (1.0 / N), buffer, (unsigned long long)sum_final);
#endif
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

#if USE_YYBENCH && BENCH_STR
bool createDirectories(const std::string &path)
{
    try
    {
        if (std::filesystem::create_directories(path))
        {
            std::cout << "OK : create directory success: " << path << std::endl;
            return true;
        }
        else if (std::filesystem::exists(path))
        {
            std::cout << "OK : directory already exists: " << path << std::endl;
            return true;
        }
        else
        {
            std::cout << "ERROR : create directory failed: " << path << std::endl;
            return false;
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "ERROR : create directory failed: " << e.what() << std::endl;
        return false;
    }
}
std::string getFileName(char *float_or_double)
{
    std::string directoryName = std::string("../bench_result/") + getCPUName();
    createDirectories(directoryName);
    std::string fileName = directoryName;
    fileName += std::string("/") + std::string(float_or_double);
    // fileName += std::string("_") + getCPUName();
    fileName += std::string("_") + keepAlnumOnly(std::string(yy_env_get_compiler_desc()));
    fileName += std::string(".html");
    return fileName;
}
#endif

void bench_double()
{

    init_double();

    bench_double_all_algorithm();

    free_double();

#if USE_YYBENCH && BENCH_STR
    printf("\nuse yy_bench to generate html report\n");
    // benchmark src from https://github.com/ibireme/c_numconv_benchmark. thanks for yy.
    printf("bench_double start , may cost long time , please wait\n");
    // std::string fileName = std::string("bench_double_result_") + getCPUName() + std::string(".html");
    // std::string fileName = std::string("bench_double_result_") + "Apple_M1" + std::string(".html");
    // std::string fileName = std::string("bench_double_result_") + "AMD64_7840H" + std::string(".html");

    std::string fileName = getFileName((char *)"double");

    benchmark_double(fileName.c_str());

    printf("bench_double finish , please open %s\n", fileName.c_str());
#else

#endif
}
void bench_float()
{
    init_float();

    bench_float_all_algorithm();

    free_float();

#if USE_YYBENCH && BENCH_STR
    printf("\nuse yy_bench to generate html report\n");
    // benchmark src from https://github.com/ibireme/c_numconv_benchmark. thanks for yy.
    printf("bench_float start , may cost long time , please wait\n");
    // std::string fileName = std::string("bench_float_result_") + getCPUName() + std::string(".html");
    // std::string fileName = std::string("bench_float_result_") + "Apple_M1" + std::string(".html");
    // std::string fileName = std::string("bench_float_result_") + "AMD64_7840H" + std::string(".html");

    std::string fileName = getFileName((char *)"float");

    benchmark_float(fileName.c_str());

    printf("bench_float finish , please open %s\n", fileName.c_str());
#else

#endif
}

void print_cpu_info()
{
    // printf("CPU Name: %s\n", getCPUName().c_str());

#if defined(__amd64__)
    printf("CPU ARCH: x86_64\n");
#elif defined(__aarch64__)
    printf("CPU ARCH: ARM64\n");
#else
    printf("CPU ARCH: unknown\n");
#endif

#if USE_YYBENCH
    printf("Compiler: %s\n", yy_env_get_compiler_desc());
#endif

#if defined(__amd64__)

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__)
    printf("CPU Feature: AVX512IFMA, AVX512VBMI\n");
#elif defined(__AVX512__)
    printf("CPU Feature: AVX512\n");
#elif defined(__AVX2__)
    printf("CPU Feature: AVX2\n");
#elif defined(__AVX__)
    printf("CPU Feature: AVX\n");
#elif defined(__SSE4_2__)
    printf("CPU Feature: SSE4.2\n");
#elif defined(__SSE4_1__)
    printf("CPU Feature: SSE4.1\n");
#elif defined(__SSE3__)
    printf("CPU Feature: SSE3\n");
#elif defined(__SSE2__)
    printf("CPU Feature: SSE2\n");
#elif defined(__SSE__)
    printf("CPU Feature: SSE\n");
#else
    printf("CPU Feature: none\n");
#endif

#elif defined(__aarch64__)

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    printf("CPU Feature: ARM_NEON\n");
#else
    printf("CPU Feature: none\n");
#endif

#else
    printf("CPU Feature: unknown\n");
#endif
}

int main()
{
    print_cpu_info();

#if BENCH_FLOAT
    bench_float();
#endif

#if BENCH_DOUBLE
    bench_double();
#endif

    return 0;
}

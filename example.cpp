#include <iostream>
#include <cmath>

#include "bench/xjb/dtoa_xjb_comp.cpp" // compress table
#include "bench/xjb/dtoa_xjb64_xjb32.cpp" // full table


int main()
{
    double v[]={
        123.45,
        1000,
        123,
        123000,
        1.2e23,
        1e100,
        1e-100,
        1.2e100,
        0.0123,
        0.001,
        0.000123,
        -123.45,
        1e20,
        1e-20,
        -1e20,
        sin(1),
        log2(10),
        log10(2),
        M_SQRT2,
        M_E,
        M_PI,
        0,
    };
    char buf_full_double[33],buf_full_float[24];
    char buf_comp_double[33],buf_comp_float[24];
    for(int i = 0; i < sizeof(v) / sizeof(double); i++)
    {
        xjb64_32::xjb64(v[i],buf_full_double);
        xjb64_32_comp::xjb64(v[i],buf_comp_double);

        xjb64_32::xjb32(v[i],buf_full_float);
        xjb64_32_comp::xjb32(v[i],buf_comp_float);

        printf("i = %d, xjb64_full : %10s  xjb32_full : %10s\n",i,buf_full_double,buf_full_float);
        printf("i = %d, xjb64_comp : %10s  xjb32_comp : %10s\n",i,buf_comp_double,buf_comp_float);
    }
    int i = 0;
    while(i++ < 30)
    {
        printf("input a real number:");
        double num;
        scanf("%le",&num);

        char* buf_end_full_double = xjb64_32::xjb64(num,buf_full_double);
        char* buf_end_comp_double = xjb64_32_comp::xjb64(num,buf_comp_double);
        char* buf_end_full_float = xjb64_32::xjb32(num,buf_full_float);
        char* buf_end_comp_float = xjb64_32_comp::xjb32(num,buf_comp_float);

        int write_length_full_double = buf_end_full_double - buf_full_double;
        int write_length_comp_double = buf_end_comp_double - buf_comp_double;
        int write_length_full_float = buf_end_full_float - buf_full_float;
        int write_length_comp_float = buf_end_comp_float - buf_comp_float;


        printf("xjb64_full : %10s write length = %2d\n",buf_full_double,write_length_full_double);
        printf("xjb64_comp : %10s write length = %2d\n",buf_comp_double,write_length_comp_double);
        printf("xjb32_full : %10s write length = %2d\n",buf_full_float,write_length_full_float);
        printf("xjb32_comp : %10s write length = %2d\n",buf_comp_float,write_length_comp_float);
    }
}

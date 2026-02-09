#include <iostream>
#include <cmath>

//#include "bench/xjb/xjb_comp.cpp" // compress table
//#include "bench/xjb/xjb.cpp" // full table

#include "../bench/xjb/ftoa_json_new.cpp"


int main()
{
    double v[]={
        1e-8,
        1e-7,
        // json format test
        1e-6,
        1e-5,
        1e-4,
        1e-3,
        1e-2,
        1e-1,
        1,
        1e1,
        1e2,
        1e3,
        1e4,
        1e5,
        1e6,
        1e7,
        1e8,
        1e9,
        1e10,
        1e11,
        1e12,
        1e13,
        1e14,
        1e15,
        1e16,
        1e17,
        1e18,
        1e19,
        1e20,
        //
        1e21,
        1e22,
        1e23,
        1000,
        123,
        626,
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
        //M_SQRT2,
        //M_E,
        //M_PI,
        0,
        9999999999990000,
        // some subnormal numbers
        5e-324,
        1e-323,
        1.23e-315,
        3e-310,
        1e-45,//min float
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::quiet_NaN(),
        -std::numeric_limits<double>::quiet_NaN(),
    };
    
    for(int i = 0; i < sizeof(v) / sizeof(double); i++)
    {
        static char buf_full_double[33],buf_full_float[24];
        //static char buf_comp_double[33],buf_comp_float[24];
        xjb::xjb64(v[i],buf_full_double);
        //xjb_comp::xjb64_comp(v[i],buf_comp_double);

        xjb::xjb32(v[i],buf_full_float);
        //xjb_comp::xjb32_comp(v[i],buf_comp_float);

        printf("i = %2d, xjb64_full : %25s  xjb32_full : %25s\n",i,buf_full_double,buf_full_float);
        //printf("i = %2d, xjb64_comp : %s  xjb32_comp : %s\n",i,buf_comp_double,buf_comp_float);
        // int j=0;
        // while(buf_full_double[j]){
        //     putchar(buf_full_double[j]);
        //     j++;
        // }
        // putchar('\n');
        //std::cout << i << " xjb64_full : " << buf_full_double << "  xjb32_full : " << buf_full_float << std::endl;
        //printf("i = %d, xjb64_comp : %10s  xjb32_comp : %10s\n",i,buf_comp_double,buf_comp_float);
    }
    int i = 0;
    while(i++ < 30)
    {
        static char buf_full_double[33],buf_full_float[24];
        //static char buf_comp_double[33],buf_comp_float[24];
        printf("input a real number:");
        double num;
        //scanf("%le",&num);
        std::cin >> num;

        char* buf_end_full_double = xjb::xjb64(num,buf_full_double);
        //char* buf_end_comp_double = xjb_comp::xjb64_comp(num,buf_comp_double);
        char* buf_end_full_float = xjb::xjb32(num,buf_full_float);
        //char* buf_end_comp_float = xjb_comp::xjb32_comp(num,buf_comp_float);

        int write_length_full_double = buf_end_full_double - buf_full_double;
        //int write_length_comp_double = buf_end_comp_double - buf_comp_double;
        int write_length_full_float = buf_end_full_float - buf_full_float;
        //int write_length_comp_float = buf_end_comp_float - buf_comp_float;


        printf("xjb64_full : %10s write length = %2d\n",buf_full_double,write_length_full_double);
        //printf("xjb64_comp : %10s write length = %2d\n",buf_comp_double,write_length_comp_double);
        printf("xjb32_full : %10s write length = %2d\n",buf_full_float,write_length_full_float);
        //printf("xjb32_comp : %10s write length = %2d\n",buf_comp_float,write_length_comp_float);
    }
}

#include <iostream>
#include <cmath>

//#include "bench/xjb/xjb_comp.cpp" // compress table
//#include "bench/xjb/xjb.cpp" // full table

#include "ftoa.c"


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
        static char buf[64];
        short1(buf, v[i]);
        printf("i = %2d, buf : %25s  \n",i, buf);
    }
    int i = 0;
    while(i++ < 30)
    {
        static char buf[33];
        printf("input a real number:");
        double num;
        //scanf("%le",&num);
        std::cin >> num;

        int len = short1(buf, num);

        printf("i = %d, buf : %s , len = %d\n",i, buf , len);
    }
}

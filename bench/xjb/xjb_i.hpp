//float/double to string
#include "dtoa_xjb_comp.cpp"
#include "dtoa_xjb64_xjb32.cpp"

//float/double to decimal
#include "xjb32_i.hpp" // contain xjb_f32_to_dec xjb xjb_comp_f32_to_dec
#include "xjb64_i.hpp" // conatin xjb_f64_to_dec xjb xjb_comp_f64_to_dec

char* xjb64_f64_to_str(double v,char* buf)
{
    return xjb64_32::xjb64(v,buf);
}

char* xjb64_comp_f64_to_str(double v,char* buf)
{
    return xjb64_32_comp::xjb64(v,buf);
}

char* xjb32_f32_to_str(float v,char* buf)
{
    return xjb64_32::xjb32(v,buf);
}

char* xjb32_comp_f32_to_str(float v,char* buf)
{
    return xjb64_32_comp::xjb32(v,buf);
}
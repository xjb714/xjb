//float/double to string

//#include "dtoa_xjb_comp.cpp"
//#include "dtoa_xjb64_xjb32.cpp"
//#include "xjb.cpp"
//#include "ftoa.cpp"

#define USE_xjb_comp 0

#include "ftoa_json.cpp"

#if USE_xjb_comp
    #include "xjb_comp.cpp"
#endif

#include "d2e_xjb.cpp"

//float/double to decimal
#include "xjb32_i.hpp" // contain xjb_f32_to_dec xjb xjb_comp_f32_to_dec
#include "xjb64_i.hpp" // conatin xjb_f64_to_dec xjb xjb_comp_f64_to_dec

char* xjb64_f64_to_str(double v,char* buf)
{
    //return buf;
    return xjb::xjb64(v,buf);
    //return xjb64_32::xjb64(v,buf);
}

char* xjb64_comp_f64_to_str(double v,char* buf)
{
    // to be implemented
    //return buf;
#if USE_xjb_comp
    return xjb_comp::xjb64_comp(v,buf);
#else
    return buf;
#endif
    //return xjb64_32_comp::xjb64(v,buf);
}

char* xjb32_f32_to_str(float v,char* buf)
{
    //return buf;
    return xjb::xjb32(v,buf);
    //return xjb::xjb32(v,buf);
    //return xjb64_32::xjb32(v,buf);
}

char* xjb32_comp_f32_to_str(float v,char* buf)
{
#if USE_xjb_comp
    return xjb_comp::xjb32_comp(v,buf);
#else
    return buf;
#endif
}

char* d2e_xjb_f64_to_str(double v,char* buf)
{
    return d2e_xjb(v,buf);
}

char* f2e_xjb_f32_to_str(float v,char* buf)
{
    return f2e_xjb(v,buf);
}
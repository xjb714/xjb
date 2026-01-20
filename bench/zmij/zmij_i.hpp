// #include "zmij.cpp"
// //#include "zmij_new.cpp" // assert fail

// char* zmij_f64_to_str(double value, char* buffer) {
//     char* buf = zmij_impl::zmij::dtoa(value, buffer);
//     return buf;
// }


// namespace zmij_impl
// {
//     #include "zmij.cc"
// }
// char* zmij_f64_to_str(double value, char* buffer) {
//     char* buf = zmij_impl::zmij::detail::write(value, buffer);
//     return buf;
// }
// char* zmij_f32_to_str(float value, char* buffer) {
//     char* buf = zmij_impl::zmij::detail::write(value, buffer);
//     return buf;
// }
// void zmij_f64_to_dec(double v,unsigned long long * dec,int *e10){
//     auto dec_fp = zmij_impl::zmij::to_decimal(v);
//     *dec = dec_fp.sig;
//     *e10 = dec_fp.exp;
// }
// void zmij_f32_to_dec(float v,unsigned int* dec,int *e10){
//     auto dec_fp = zmij_impl::zmij::to_decimal(v);
//     *dec = dec_fp.sig;
//     *e10 = dec_fp.exp;
// }


#include "zmij.cc"
char* zmij_f64_to_str(double value, char* buffer) {
    char* buf = zmij::detail::write(value, buffer);
    return buf;
}
char* zmij_f32_to_str(float value, char* buffer) {
    char* buf = zmij::detail::write(value, buffer);
    return buf;
}
void zmij_f64_to_dec(double v,unsigned long long * dec,int *e10){
    auto dec_fp = zmij::to_decimal(v);
    *dec = dec_fp.sig;
    *e10 = dec_fp.exp;
}
void zmij_f32_to_dec(float v,unsigned int* dec,int *e10){
    auto dec_fp = zmij::to_decimal(v);
    *dec = dec_fp.sig;
    *e10 = dec_fp.exp;
}
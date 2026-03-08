//#include "ftoa.c"
// #include "uscalec_mod.cpp"

// char* uscalec_f64_to_str(double value, char* buffer) {
//     return uscalec::dtoa(value,buffer);
// }

namespace uscalec {
    #include "uscale.c"
}
char* uscalec_f64_to_str(double value, char* buffer) {
    return uscalec::uscale_short(value,buffer);
}
static inline void uscalec_f64_to_dec(double v, unsigned long long *dec, int *e10){
    uscalec::Short(v, dec, e10);
}
//#include "ftoa.c"
#include "uscalec_mod.cpp"
char* uscalec_f64_to_str(double value, char* buffer) {
    return uscalec::dtoa(value,buffer);
}

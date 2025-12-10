#include "schubfach.cc"

char* schubfach_vitaut_f64_to_str(double value, char* buf){
    return schubfach_vitaut::dtoa(value, buf);
}
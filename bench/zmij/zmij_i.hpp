#include "zmij.cpp"
//#include "zmij_new.cpp" // assert fail

char* zmij_f64_to_str(double value, char* buffer) {
    char* buf = zmij_impl::zmij::dtoa(value, buffer);
    return buf;
}
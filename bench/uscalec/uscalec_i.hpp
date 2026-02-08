#include "ftoa.c"
char* uscalec_f64_to_str(double value, char* buffer) {
    int len = short1(buffer, value);
    return buffer + len;
}

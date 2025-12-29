// #include "zmij.cpp"
// //#include "zmij_new.cpp" // assert fail

// char* zmij_f64_to_str(double value, char* buffer) {
//     char* buf = zmij_impl::zmij::dtoa(value, buffer);
//     return buf;
// }


namespace zmij_impl_v20251223 {
    #include "zmij.cc"
}
char* zmij_f64_to_str(double value, char* buffer) {
    char* buf = zmij_impl_v20251223::zmij::detail::to_string(value, buffer);
    return buf;
}
char* zmij_f32_to_str(float value, char* buffer) {
    char* buf = zmij_impl_v20251223::zmij::detail::to_string(value, buffer);
    return buf;
}
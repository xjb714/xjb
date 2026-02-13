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
char *zmij_f64_to_str(double value, char *buffer)
{
    char *buf = zmij::detail::write(value, buffer);
    return buf;
}
char *zmij_f32_to_str(float value, char *buffer)
{
    char *buf = zmij::detail::write(value, buffer);
    return buf;
}
static inline void zmij_f64_to_dec(double v, unsigned long long *dec, int *e10)
{
    auto dec_fp = zmij::to_decimal(v);
    *dec = dec_fp.sig;
    *e10 = dec_fp.exp;
}
static inline void zmij_f32_to_dec(float v, unsigned int *dec_return, int *e10_return)
{
    // auto dec_fp = zmij::to_decimal(v);
    // *dec = dec_fp.sig;
    // *e10 = dec_fp.exp;
    using traits = float_traits<float>;
    auto bits = traits::to_bits(v);
    // It is beneficial to extract exponent and significand early.
    auto bin_exp = traits::get_exp(bits); // binary exponent
    auto bin_sig = traits::get_sig(bits); // binary significand

    to_decimal_result dec;
    constexpr uint64_t threshold = uint64_t(traits::num_bits == 64 ? 1e16 : 1e8);
    if (bin_exp == 0 || bin_exp == traits::exp_mask) [[ZMIJ_UNLIKELY]]
    {
        // if (bin_exp != 0) {
        //   memcpy(buffer, bin_sig == 0 ? "inf" : "nan", 4);
        //   return buffer + 3;
        // }
        // if (bin_sig == 0) {
        //   memcpy(buffer, "0", 2);
        //   return buffer + 1;
        // }
        dec = to_decimal_schubfach(bin_sig, 1 - traits::exp_offset, true);
        while (dec.sig < threshold)
        {
            dec.sig *= 10;
            --dec.exp;
        }
    }
    else
    {
        dec = to_decimal_fast<float>(bin_sig | traits::implicit_bit, bin_exp,
                                     bin_sig != 0);
    }

    *dec_return = dec.sig;
    *e10_return = dec.exp;
}
#include <stdint.h>
#include <string.h>
#include <stdio.h>



typedef __uint128_t u128;
typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;

#include "dec_to_ascii.cpp"
#include "table.cpp"

namespace xjb {

static inline void byte_move_16(void *dst, const void *src)
{
    // move 16byte from src to dst; no overlap issue
#if HAS_NEON
    uint64x2_t src_value = vld1q_u64((const uint64_t *)src);
    vst1q_u64((uint64_t *)dst, src_value);
#elif HAS_SSE2
    __m128i src_value = _mm_loadu_si128((const __m128i *)src);
    _mm_storeu_si128((__m128i *)dst, src_value);
#else
    u64 hi;
    u64 lo;
    char *psrc = (char *)src;
    memcpy(&hi, psrc, 8);
    memcpy(&lo, psrc + 8, 8);
    char *pdst = (char *)dst;
    memcpy(pdst, &hi, 8);
    memcpy(pdst + 8, &lo, 8);
#endif
}
static inline void byte_move_8(void *dst, const void *src)
{
    // move 8byte from src to dst; no overlap issue
    u64 src_value;
    memcpy(&src_value, src, 8);
    memcpy(dst, &src_value, 8);
}
char *xjb64(double v, char *buf)
{
    u64 vi;
    memcpy(&vi, &v, sizeof(v));
    buf[0] = '-';
    buf += vi >> 63;
    u64 vi_abs = (vi << 1) >> 1;
    u64 ieee_significand = vi & ((1ull << 52) - 1);
    u64 ieee_exponent = (vi << 1) >> 53;
    // if ((vi << 1) - 4 >= (2047ull << 53) - 3) [[unlikely]]
    // {
    //     if ((vi << 1) == 0)
    //         memcpy(buf, "0.0\0\0\0\0", 4);
    //     if ((vi << 1) == (1 << 1))
    //         memcpy(buf, "5e-324\0", 8);
    //     if ((vi << 1) == (2047ull << 53))
    //         memcpy(buf, "Inf\0\0\0\0", 4);
    //     if ((vi << 1) > (2047ull << 53))
    //         memcpy(buf, "NaN\0\0\0\0", 4);
    //     return buf + ((vi << 1) == 2 ? 6 : 3);
    // }

    // if(vi_abs >= (2047ull << 52))[[unlikely]]
    // {
    //     if(vi_abs == (2047ull << 52))[[likely]]
    //     {
    //         memcpy(buf, "Inf", 4);
    //         return buf + 3;
    //     }else{
    //         memcpy(buf, "NaN", 4);
    //         return buf + 3;
    //     }
    // }
    // if(vi_abs < 2)[[unlikely]]
    // {
    //         if(vi_abs > 0)[[likely]]
    //         {
    //             memcpy(buf, "5e-324\0", 8);
    //             return buf + 6;
    //         }else{
    //             memcpy(buf, "0.0", 4);
    //             return buf + 3;
    //         }
    // }

    // if(0)
    if ((u64)(vi_abs - 2) >= (u64)((2047ull << 52) - 2)) [[unlikely]]
    {
        // generate branch instruction
        if (vi_abs == 0)
            memcpy(buf, "0.0\0\0\0\0", 4);
        if (vi_abs == 1)
            memcpy(buf, "5e-324\0", 8);
        if (vi_abs == (2047ull << 52))
            memcpy(buf, "inf\0\0\0\0", 4);
        if (vi_abs > (2047ull << 52))
            memcpy(buf, "nan\0\0\0\0", 4);
        return buf + (vi_abs == 1 ? 6 : 3);

        // if (vi_abs == (2047ull << 52))
        //     memcpy(buf, "Inf", 4);
        // else if (vi_abs > (2047ull << 52))
        //     memcpy(buf, "NaN", 4);
        // else if (vi_abs == 0)
        //     memcpy(buf, "0.0", 4);
        // else
        //     memcpy(buf, "5e-324\0", 8);
        // return buf + (vi_abs == 1 ? 6 : 3);

        // if(vi_abs == (2047ull << 52))[[likely]]
        // {
        //     memcpy(buf, "Inf", 4);
        //     return buf + 3;
        // }
        // else if(vi_abs == 1)[[likely]]
        // {
        //     memcpy(buf, "5e-324\0", 8);
        //     return buf + 6;
        // }
        // else
        // {
        //     if(vi_abs > 0)[[likely]]
        //     {
        //         memcpy(buf, "NaN", 4);
        //         return buf + 3;
        //     }else{
        //         memcpy(buf, "0.0", 4);
        //         return buf + 3;
        //     }

        // }
    }
    i64 q = (i64)ieee_exponent - 1075;
    u64 nq = -q;
    u64 c = ((1ull << 52) | ieee_significand);
#if 1
#if is_real_gcc
    if (nq <= u64_tz_bits(c)) [[unlikely]] // use unlikely will generate jmp instruction
#else
    if (nq <= u64_tz_bits(c))
#endif
        return write_1_to_16_digit(c >> nq, buf); // fast path for integer
#endif
    if (ieee_exponent == 0) [[unlikely]]
    {
        c = ieee_significand;
        q = 1 - 1075; // -1074
    }
    i64 k;
    const int offset = 6;
    u64 regular = ieee_significand > 0;
    u64 irregular = (ieee_significand == 0);
#ifdef __amd64__
    if (!irregular) [[likely]] // branch
        k = (i64)((ieee_exponent - 1075) * 315653) >> 20;
    else
        k = (i64)((ieee_exponent - 1075) * 315653 - 131237) >> 20;
#else
    //k = (i64)(((i64)ieee_exponent - 1075) * 315653 - (irregular ? 131237 : 0)) >> 20;
    k = (i64)(((i64)ieee_exponent - 1075) * 315653 + (irregular ? -131072 : 0)) >> 20;
#endif

#ifdef __amd64__
    i64 get_e10 = -1 - k;
    i64 h = q + ((get_e10 * 217707) >> 16);
    static const u64 *pow10_ptr = pow10_double + 293 * 2;
    u64 *p10 = (u64 *)&pow10_ptr[get_e10 * 2]; // get 10**(-k-1)
    // u64 *p10 = (u64*)&pow10_double[293*2 + get_e10*2]; // gcc use this method , may cause performance issue. why?
#else
    i64 h = q + ((k * -217707 - 217707) >> 16);
    static const u64 *pow10_ptr = pow10_double + 293 * 2 - 2;
    u64 *p10 = (u64 *)&pow10_ptr[k * -2]; // get 10**(-k-1)
#endif
    u128 cb = c << (h + (1 + offset));
    u128 hi128 = (cb * p10[0] + ((cb * p10[1]) >> 64));
    u64 dot_one = hi128 >> offset;
    u64 half_ulp = (p10[0] >> (-h)) + ((c + 1) & 1);
    u64 up = (half_ulp > ~0 - dot_one);
    u64 down = ((half_ulp >> (irregular)) > dot_one);
    u64 m = (u64)(hi128 >> (offset + 64)) + up;
    u64 up_down = up + down;
    u64 D17 = (m >= (u64)1e15);
    u64 mr = D17 ? m : m * 10;
    memcpy(buf, "00000000", 8);
    shortest_ascii16 s = to_ascii16(mr, up_down, D17);
    k += 15 + D17;
    i64 e10 = k;
#ifdef __amd64__
    // u64 offset_num = (dot_one == (1ull << 62)) ? 0 : (1ull << 63) + 6;
    u64 offset_num = (1ull << 63) + 6;
    if (dot_one == (1ull << 62)) [[unlikely]]
        offset_num = 0;
    u64 one = ((dot_one * (u128)10 + offset_num) >> 64) + (u64)('0' + '0' * 256);
    if (irregular) [[unlikely]]
        if ((((dot_one >> 54) * 5) & ((1 << 9) - 1)) > (((half_ulp >> 55) * 5)))
            one = (((dot_one >> 54) * 5) >> 9) + 1 + (u64)('0' + '0' * 256);
#else
    u64 one = (((dot_one * (u128)10) >> 64) + (u64)('0' + '0' * 256)) + (((u64)(dot_one * (u128)10) > ((dot_one == (1ull << 62)) ? ~0 : 0x7ffffffffffffff9ull)));
    if (irregular) [[unlikely]] // Since the compiler tries to prevent access to memory, it generates branch instructions.
        one += (bit_array_irregular[ieee_exponent / 64] >> (ieee_exponent % 64)) & 1;
#endif

    const i64 e10_DN = -3;
    const i64 e10_UP = 15;
    u64 e10_3 = e10 + (-e10_DN);
    u64 e10_data_ofs = e10_3 < e10_UP - e10_DN + 1 ? e10_3 : e10_UP - e10_DN + 1; // compute offset , min(e10_3,19)
    u64 first_sig_pos = e10_variable_data2[e10_data_ofs][17 + 0];
    u64 dot_pos = e10_variable_data2[e10_data_ofs][17 + 1];
    u64 move_pos = e10_variable_data2[e10_data_ofs][17 + 2];
    u64 exp_pos = e10_variable_data2[e10_data_ofs][s.dec_sig_len];
    char *buf_origin = buf;
    buf += first_sig_pos;
#if HAS_NEON_OR_SSE2
    memcpy(buf, &(s.ascii16), 16);
#else
    memcpy(buf + 0, &(s.hi), 8);
    memcpy(buf + 8, &(s.lo), 8);
#endif
    memcpy(&buf[15 + D17], &one, 8);
    byte_move_16(&buf[move_pos], &buf[dot_pos]); // dot_pos+first_sig_pos+sign max = 16+1 = 17; require 17+16=33 byte buffer
    buf_origin[dot_pos] = '.';
    static const u64 *exp_ptr = (u64 *)&exp_result_double[324];
    // if (m < (u64)1e14) [[unlikely]]
    if (ieee_exponent == 0) [[unlikely]]
    {
        // some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
        if (buf[0] == '0')
        {
            u64 lz = 0;
            while (buf[2 + lz] == '0')
                lz++;
            lz += 2;
            e10 -= lz - 1;
            buf[0] = buf[lz];
            byte_move_16(&buf[2], &buf[lz + 1]);
            exp_pos = exp_pos - lz + (exp_pos - lz != 1);
        }
        // #if is_intel_compiler
        //             buf += exp_pos;
        //             u64 exp_result = exp_ptr[e10];
        //             memcpy(buf, &exp_result, 8);
        //             return buf + 5;
        // #endif
    }
    u64 exp_result = exp_ptr[e10];
    // u64 exp_result = exp_result_double[e10 +  324];
    buf += exp_pos;
    memcpy(buf, &exp_result, 8);
    u64 exp_len = exp_result >> 56;
    return buf + exp_len;
}
#if 1
//static inline 
char *xjb32(float v, char *buf, bool debug_mode = false)
{
    const struct const_value_float *c = &constants_float;
    const struct float_table_t *t = &float_table;
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // for arm64 processor , fewer instructions , MSVC not support inline asm
    asm("" : "+r"(c));                                                // read constant values from memory to register
#endif

    u32 vi;
    memcpy(&vi, &v, 4);
    buf[0] = '-';
    buf += vi >> 31;
    u64 sig = vi & ((1 << 23) - 1);
    u64 exp = (vi << 1) >> 24;
    u64 sig_bin = sig | (1 << 23);
    i64 exp_bin = (i64)exp - 150;
    if (exp == 0) [[unlikely]]
    {
        if (sig == 0)
            return (char *)memcpy(buf, "0.0", 4) + 3;
        exp_bin = 1 - 150;
        sig_bin = sig;
    }
    if (exp == 255) [[unlikely]]
        return (char *)memcpy(buf, sig ? "nan" : "inf", 4) + 3;
    unsigned char h37_precalc = t->h37[exp];
    u64 irregular = sig == 0;
    const int BIT = 36;                  // [33,36] all right
    i64 k = (i64)(exp_bin * 1233) >> 12; // exp_bin range : [-149,104] ; k range : [-45,31]
    if (irregular) [[unlikely]]
    {
        k = (i64)(exp_bin * 1233 - 512) >> 12;
        h37_precalc = (BIT + 1) + exp_bin + ((k * -1701 + (-1701)) >> 9);
    }
    u64 pow10_hi = t->pow10_float_reverse[45 + k];
    u64 even = (sig + 1) & 1; // or (sig_bin + 1) & 1
    u64 cb = sig_bin << h37_precalc;
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
    u64 half_ulp = (pow10_hi >> (65 - h37_precalc)) + even;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1);
#ifdef __amd64__
    u64 up = (half_ulp + dot_one_36bit) >> BIT;
#else
    u64 up = half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit;
#endif
    u64 down = (half_ulp >> irregular) > dot_one_36bit;
    u64 up_down = up + down;
    u64 m = (sig_hi >> BIT) + up;
    memcpy(buf, "0000", 4);
    //u64 lz = (m < (u32)1e7) + (m < (u32)1e6); // 0, 1, 2
    u64 lz = (m < c->e6) + (m < c->e7);
    //u64 lz = (m < c->e6) ? 2 : (m < c->e7); //branch instruction
    //u64 lz = (m < (u32)1e6) ? 2 : (m < (u32)1e7);
    shortest_ascii8 s = to_ascii8(m, up_down, lz, c);
    i64 e10 = k + (8 - lz);
    // u64 offset_num = (((u64)('0' + '0' * 256) << (BIT - 1)) + (((u64)1 << (BIT - 2)) - 7)) + (dot_one_36bit >> (BIT - 4));
    u64 offset_num = c->c1 + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
    //one = cmov_branchless(up_down, '0' + '0' * 256, one); // prevent gcc generate branch instruction
    if (irregular) [[unlikely]]
        if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150)) // branch instruction
            ++one;
    const i64 e10_DN = -3, e10_UP = 6;
    u64 e10_3 = e10 + (-e10_DN);
    u64 e10_data_ofs = e10_3 < e10_UP - e10_DN + 1 ? e10_3 : e10_UP - e10_DN + 1;
    u64 exp_len = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : 4;
    u64 first_sig_pos = t->e10_variable_data[e10_data_ofs][9 + 0];
    u64 dot_pos = t->e10_variable_data[e10_data_ofs][9 + 1];
    u64 move_pos = t->e10_variable_data[e10_data_ofs][9 + 2];
    u64 exp_pos = t->e10_variable_data[e10_data_ofs][s.dec_sig_len];
    char *buf_origin = (char *)buf;
    buf += first_sig_pos;
    memcpy(buf, &(s.ascii), 8);
    memcpy(&buf[8 - lz], &one, 8);
    byte_move_8(&buf[move_pos], &buf[dot_pos]);
    buf_origin[dot_pos] = '.';
    // if ( (is_little_endian() ? (s.ascii & 0xf) : (s.ascii & (0xfull<<56))) == 0)
    if (exp == 0) [[unlikely]]
        //if (m < c->e5) // m < 100000
        if (m < 100000)
        {
            u64 lz = 0;
            // while (buf[2 + lz] == '0')
            //     lz++;
            u64 u;
            memcpy(&u, &buf[2], 8);
            u = is_little_endian() ? u : byteswap64(u);
            lz = u64_tz_bits(u & 0x0f0f0f0f0f0f0f0f) / 8;
            lz += 2;
            e10 -= lz - 1;
            buf[0] = buf[lz];
            byte_move_8(&buf[2], &buf[lz + 1]);
            exp_pos = exp_pos - lz + (exp_pos - lz != 1);
        }
    u64 exp_result = t->exp_result_float[45 + e10];
    buf += exp_pos;
    memcpy(buf, &exp_result, 8);
    return buf + exp_len;
}

template <typename Float>
char *to_string(Float v, char *buf)
{
    if (sizeof(Float) == sizeof(float))
        return xjb32(v, buf);
    else if (sizeof(Float) == sizeof(double))
        return xjb64(v, buf);
    else
        return buf;
}
#endif

}//end of namespace xjb
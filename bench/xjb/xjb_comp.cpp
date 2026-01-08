#include <stdint.h>
#include <string.h>

typedef __uint128_t u128;
typedef uint64_t u64;
typedef int64_t i64;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#include "dec_to_ascii.cpp"

namespace xjb_comp
{
    static inline u64 calc_pow5_rlz(unsigned int pow5_offset)
    {
        // require 0 <= pow5_offset <= 27; max_u64 = (2**64)-1 > 5**27
        // return (5**pow5_offset) << clz(5**pow5_offset);
#if 1
        const u64 p5_4 = 5 * 5 * 5 * 5;
        const u64 p5_8 = p5_4 * p5_4;
        const u64 p5_0_3 = (125 << 24) + (25 << 16) + (5 << 8) + 1;
        u64 p5 = ((pow5_offset & 16) ? p5_8 * p5_8 : 1) * ((pow5_offset & 8) ? p5_8 : 1) * ((pow5_offset & 4) ? p5_4 : 1) * ((p5_0_3 >> ((pow5_offset % 4) * 8)) & 0xff);
        u64 clz = 63 - ((pow5_offset * 149) >> 6); // equal to clz(p5)
        return p5 << clz;
#else
        u64 p5 = 1;
        u64 p5_base = 5;
        while (pow5_offset > 0)
        {
            if (pow5_offset & 1)
                p5 *= p5_base;
            p5_base *= p5_base;
            pow5_offset >>= 1;
        }
        u64 clz = u64_lz_bits(p5);
        // u64 clz2 = 63 - ((pow5_offset * 217707) >> 16) + pow5_offset;
        return p5 << clz;
#endif
    }
    char *xjb64_comp(double v, char *buf)
    {
        static const struct const_value_double constants_double = {
            .c1 = 315653,
            .c2 = (uint64_t)-217707,
            .c3 = (uint64_t)1e15 - 1,
            //.c4 = 0x7ffffffffffffff9ull,
            .c4 = (1ull << 63) + 6,
            .c5 = (uint64_t)-131072,
            .c6 = (1 << 9) - 1,
            .mul_const = 0xabcc77118461cefd,
            .hundred_million = (int64_t)-100000000,
            .div10000 = 1844674407370956,
            .div10000_m = 0x100000000 - 10000,
            .div10000_2_d = (double)(-10000 + 0x100000000),
            .div10000_2 = 0xd1b7176000,
            .multipliers32 = {0x68db8bb, -10000 + 0x10000, 0x147b000, -100 + 0x10000},
            .multipliers16 = {0xce0, -10 + 0x100, '0' + '0' * 256},
        };
        const struct const_value_double *cv = &constants_double;
        // const struct double_table_t *t = &double_table;
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // for arm64 processor , fewer instructions , MSVC not support inline asm
                                                                      // asm("" : "+r"(cv));                                           // read constant values from memory to register
#endif
        u64 vi;
        memcpy(&vi, &v, sizeof(v));
        buf[0] = '-';
        buf += vi >> 63;
        u64 vi_abs = (vi << 1) >> 1;
        u64 ieee_significand = vi & ((1ull << 52) - 1);
        u64 ieee_exponent = (vi << 1) >> 53;
#ifdef __amd64__ // for x86_64 processor , if not use this code , the performance will be very poor on icpx compiler. 9ns -> 12.5ns. that's why we use this code.
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
        }
#endif
        i64 q = (i64)ieee_exponent - 1075;
        u64 nq = -q;
        u64 c = ((1ull << 52) | ieee_significand);
#if 0
#if is_real_gcc
        if (nq <= u64_tz_bits(c)) [[unlikely]] // use unlikely will generate jmp instruction
#else
        if (nq <= u64_tz_bits(c)) //[[unlikely]]
#endif
            return write_1_to_16_digit(c >> nq, buf,cv); // fast path for integer
#endif
        if (ieee_exponent == 0) [[unlikely]]
        {
#ifndef __amd64__
            if (ieee_significand <= 1)
            {
                return (char *)memcpy(buf, ieee_significand ? "5e-324\0" : "0.0\0\0\0\0", 8) + (ieee_significand ? 6 : 3);
            }
#endif
            c = ieee_significand;
            q = 1 - 1075; // -1074
        }
#ifndef __amd64__ // for arm64 processor , better performance
        if (ieee_exponent == 2047) [[unlikely]]
            return (char *)memcpy(buf, ieee_significand ? "nan" : "inf", 4) + 3;
#endif
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
        // k = (i64)(((i64)ieee_exponent - 1075) * 315653 - (irregular ? 131237 : 0)) >> 20;
        k = (i64)(((i64)ieee_exponent - 1075) * cv->c1 + (irregular ? cv->c5 : 0)) >> 20; // branch instruction , more efficient than cmov
#endif

#define direct_via_lut_get_pow5 1
        // direct_via_lut_get_pow5 = 1 : direct use looup table to get (5**n)<<clz(5**n)
        // direct_via_lut_get_pow5 = 0 : calc the  (5**n)<<clz(5**n) , without using lookup table

        // compress ratio = 27; all lut size = 23*2*8 + (27+1)*8 = 592 byte
        const int base_start = -11, base_end = 11, ratio = 27;
        static const u64 pow10_base_and_pow5_rlz[(base_end - base_start + 1) * 2 + direct_via_lut_get_pow5 * (ratio + 1)] = {
            // pow10_base table
            0xa76c582338ed2621,
            0xaf2af2b80af6f24f, // e10 =  -11 * 27 = -297
            0x873e4f75e2224e68,
            0x5a7744a6e804a292, // e10 =  -10 * 27 = -270
            0xda7f5bf590966848,
            0xaf39a475506a899f, // e10 =  -9 * 27 = -243
            0xb080392cc4349dec,
            0xbd8d794d96aacfb4, // e10 =  -8 * 27 = -216
            0x8e938662882af53e,
            0x547eb47b7282ee9d, // e10 =  -7 * 27 = -189
            0xe65829b3046b0afa,
            0x0cb4a5a3112a5113, // e10 =  -6 * 27 = -162
            0xba121a4650e4ddeb,
            0x92f34d62616ce414, // e10 =  -5 * 27 = -135
            0x964e858c91ba2655,
            0x3a6a07f8d510f870, // e10 =  -4 * 27 = -108
            0xf2d56790ab41c2a2,
            0xfae27299423fb9c4, // e10 =  -3 * 27 = -81
            0xc428d05aa4751e4c,
            0xaa97e14c3c26b887, // e10 =  -2 * 27 = -54
            0x9e74d1b791e07e48,
            0x775ea264cf55347e, // e10 =  -1 * 27 = -27
            0x8000000000000000,
            0x0000000000000000, // e10 =  0 * 27 = 0
            0xcecb8f27f4200f3a,
            0x0000000000000000, // e10 =  1 * 27 = 27
            0xa70c3c40a64e6c51,
            0x999090b65f67d924, // e10 =  2 * 27 = 54
            0x86f0ac99b4e8dafd,
            0x69a028bb3ded71a4, // e10 =  3 * 27 = 81
            0xda01ee641a708de9,
            0xe80e6f4820cc9496, // e10 =  4 * 27 = 108
            0xb01ae745b101e9e4,
            0x5ec05dcff72e7f90, // e10 =  5 * 27 = 135
            0x8e41ade9fbebc27d,
            0x14588f13be847308, // e10 =  6 * 27 = 162
            0xe5d3ef282a242e81,
            0x8f1668c8a86da5fb, // e10 =  7 * 27 = 189
            0xb9a74a0637ce2ee1,
            0x6d953e2bd7173693, // e10 =  8 * 27 = 216
            0x95f83d0a1fb69cd9,
            0x4abdaf101564f98f, // e10 =  9 * 27 = 243
            0xf24a01a73cf2dccf,
            0xbc633b39673c8ced, // e10 =  10 * 27 = 270
            0xc3b8358109e84f07,
            0x0a862f80ec4700c9, // e10 =  11 * 27 = 297
#if direct_via_lut_get_pow5
            // //pow5_remove_left_zero table or pow10_hi table ; when 0 <= -k-1 && -k-1 <= 27; direct use below table for pow10_hi , and pow_lo=0
            0x8000000000000000, // = (5** 0) << clz(5** 0) = (5** 0) << 63 ; e10 = 0
            0xa000000000000000, // = (5** 1) << clz(5** 1) = (5** 1) << 61 ; e10 = 1
            0xc800000000000000, // = (5** 2) << clz(5** 2) = (5** 2) << 59 ; e10 = 2
            0xfa00000000000000, // = (5** 3) << clz(5** 3) = (5** 3) << 57 ; e10 = 3
            0x9c40000000000000, // = (5** 4) << clz(5** 4) = (5** 4) << 54 ; e10 = 4
            0xc350000000000000, // = (5** 5) << clz(5** 5) = (5** 5) << 52 ; e10 = 5
            0xf424000000000000, // = (5** 6) << clz(5** 6) = (5** 6) << 50 ; e10 = 6
            0x9896800000000000, // = (5** 7) << clz(5** 7) = (5** 7) << 47 ; e10 = 7
            0xbebc200000000000, // = (5** 8) << clz(5** 8) = (5** 8) << 45 ; e10 = 8
            0xee6b280000000000, // = (5** 9) << clz(5** 9) = (5** 9) << 43 ; e10 = 9
            0x9502f90000000000, // = (5**10) << clz(5**10) = (5**10) << 40 ; e10 = 10
            0xba43b74000000000, // = (5**11) << clz(5**11) = (5**11) << 38 ; e10 = 11
            0xe8d4a51000000000, // = (5**12) << clz(5**12) = (5**12) << 36 ; e10 = 12
            0x9184e72a00000000, // = (5**13) << clz(5**13) = (5**13) << 33 ; e10 = 13
            0xb5e620f480000000, // = (5**14) << clz(5**14) = (5**14) << 31 ; e10 = 14
            0xe35fa931a0000000, // = (5**15) << clz(5**15) = (5**15) << 29 ; e10 = 15
            0x8e1bc9bf04000000, // = (5**16) << clz(5**16) = (5**16) << 26 ; e10 = 16
            0xb1a2bc2ec5000000, // = (5**17) << clz(5**17) = (5**17) << 24 ; e10 = 17
            0xde0b6b3a76400000, // = (5**18) << clz(5**18) = (5**18) << 22 ; e10 = 18
            0x8ac7230489e80000, // = (5**19) << clz(5**19) = (5**19) << 19 ; e10 = 19
            0xad78ebc5ac620000, // = (5**20) << clz(5**20) = (5**20) << 17 ; e10 = 20
            0xd8d726b7177a8000, // = (5**21) << clz(5**21) = (5**21) << 15 ; e10 = 21
            0x878678326eac9000, // = (5**22) << clz(5**22) = (5**22) << 12 ; e10 = 22
            0xa968163f0a57b400, // = (5**23) << clz(5**23) = (5**23) << 10 ; e10 = 23
            0xd3c21bcecceda100, // = (5**24) << clz(5**24) = (5**24) <<  8 ; e10 = 24
            0x84595161401484a0, // = (5**25) << clz(5**25) = (5**25) <<  5 ; e10 = 25
            0xa56fa5b99019a5c8, // = (5**26) << clz(5**26) = (5**26) <<  3 ; e10 = 26
            0xcecb8f27f4200f3a, // = (5**27) << clz(5**27) = (5**27) <<  1 ; e10 = 27
#endif
        };
        int get_e10 = -k - 1;

        const u64 *pow5_rlz_ptr = &pow10_base_and_pow5_rlz[(base_end - base_start + 1) * 2];
        u64 pow10_hi;
        u128 hi128;
        int h;
        if (0 <= get_e10 && get_e10 <= ratio) // direct use the pow5_rlz table value ; likely
        {
            // fast path ; likely
            h = q + ((get_e10 * 217707) >> 16);
#if direct_via_lut_get_pow5
            pow10_hi = pow5_rlz_ptr[get_e10]; // pow10_lo = 0
#else
            pow10_hi = calc_pow5_rlz(get_e10);
#endif
            u128 cb = c << (h + 1 + offset);
            hi128 = cb * pow10_hi;
        }
        else
        {
            // int base = ( get_e10 - (base_start * ratio) ) / ratio;// range = [0,22] = [0/27,616/27]
            int base = ((get_e10 - (base_start * ratio)) * 1214) >> 15; // div 27
            u32 pow5_offset = get_e10 - (base + base_start) * ratio;    // range = [0,26]
            u64 pow10_base_high = pow10_base_and_pow5_rlz[base * 2];
            u64 pow10_base_low = pow10_base_and_pow5_rlz[base * 2 + 1];
#if direct_via_lut_get_pow5
            u128 pow5_rlz_v = pow5_rlz_ptr[pow5_offset]; // (5**pow5_offset) << clz(5**pow5_offset)
#else
            u128 pow5_rlz_v = calc_pow5_rlz(pow5_offset);
#endif
            u128 pow10 = pow5_rlz_v * pow10_base_high + ((pow5_rlz_v * pow10_base_low) >> 64); // u64 * u128
            pow10_hi = pow10 >> 64;
            u64 pow10_lo = pow10;
            int alpha = ((((base + base_start) * ratio) * 217707) >> 16) + ((pow5_offset * 217707) >> 16);
            h = 1 + alpha + q;
            u128 cb = c << (h + offset + 1);
            hi128 = (cb * pow10_hi + ((cb * pow10_lo) >> 64)); // u64 * u128
        }
        u64 dot_one = hi128 >> offset; // == floor(2**64*n)
        // u64 half_ulp = (pow10_hi >> (-h) ) + ((c + 1) & 1) ;
        u64 half_ulp = (h > 0 ? pow10_hi * 2 : (pow10_hi >> (-h))) + ((c + 1) & 1);
        u64 up = (half_ulp > ~0 - dot_one);
        u64 down = ((half_ulp >> (1 - regular)) > dot_one);
        u64 m = (hi128 >> (offset + 64)) + up;
        u64 up_down = up + down;
        // u64 D17 = (m >= (u64)1e15);
        u64 D17 = m > (u64)cv->c3;
        u64 mr = D17 ? m : m * 10;
        memcpy(buf, "00000000", 8);
        // memcpy(buf, "0000", 4);
        shortest_ascii16 s = to_ascii16(mr, up_down, D17, cv);
        i64 e10 = k + (15 + D17);
        // i64 e10 = k;
#ifdef __amd64__
        // u64 offset_num = (dot_one == (1ull << 62)) ? 0 : (1ull << 63) + 6;
        u64 offset_num = (1ull << 63) + 6;
        if (dot_one == (1ull << 62)) [[unlikely]]
            offset_num = 0;
        u64 one = ((dot_one * (u128)10 + offset_num) >> 64) ;//+ (u64)('0' + '0' * 256);
        if (irregular) [[unlikely]]
            if ((((dot_one >> 54) * 5) & ((1 << 9) - 1)) > (((half_ulp >> 55) * 5)))
                one = (((dot_one >> 54) * 5) >> 9) + 1 ;//+ (u64)('0' + '0' * 256);
        // if (one == 10)
        //     one = 0;
        // one += ('0' + '0' * 256);
        one = one == 10 ? ('0' + '0' * 256) : one + ('0' + '0' * 256);
#else
        u64 one = ((dot_one * (u128)10 + cv->c4) >> 64); //+ (u64)('0' + '0' * 256);
        if (dot_one == (1ull << 62)) [[unlikely]]
            one = 2;
        // one = (u64)('2' + '0' * 256);
        // if (irregular) [[unlikely]] // Since the compiler tries to prevent access to memory, it generates branch instructions.
        //     one += (t->bit_array_irregular[ieee_exponent / 64] >> (ieee_exponent % 64)) & 1;
        if (irregular) [[unlikely]]       // This is a cold code, so it is more efficient to have the compiler automatically generate branch instructions.
        {                                 // Since the compiler tries to prevent access to memory, it generates branch instructions.
            u64 *mask = (u64 *)&(cv->c6); // read constant values from memory to register , so this code will be more fast.
#if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__))
            asm("" : "+r"(mask)); // prevent compiler from optimizing out the read operation.
#endif
            if ((((dot_one >> 54) * 5) & *mask) > (((half_ulp >> 55) * 5)))
                one = (((dot_one >> 54) * 5) >> 9) + 1; //+ (u64)('0' + '0' * 256);
        }
        // if (one == 10)
        //     one = 0;
        // one += ('0' + '0' * 256);
        one = one == 10 ? ('0' + '0' * 256) : one + ('0' + '0' * 256);
#endif

        const i64 e10_DN = -3;
        const i64 e10_UP = 15;
        u64 first_sig_pos = (e10_DN <= e10 && e10 <= -1) ? 1 - e10 : 0;
        u64 dot_pos = (0 <= e10 && e10 <= e10_UP) ? 1 + e10 : 1;
        u64 move_pos = dot_pos + (1 - (e10_DN <= e10 && e10 <= -1));
        u64 exp_pos = (0 <= e10 && e10 <= e10_UP) ? (e10 + 3 > s.dec_sig_len + 2 ? e10 + 3 : s.dec_sig_len + 2) : (s.dec_sig_len + 1 + ((1 - (e10_DN <= e10 && e10 <= -1)) > (s.dec_sig_len == 0)));
        // u64 e10_3 = e10 + (-e10_DN);
        // u64 e10_data_ofs = e10_3 < e10_UP - e10_DN + 1 ? e10_3 : e10_UP - e10_DN + 1; // compute offset , min(e10_3,19)
        // u64 first_sig_pos = t->e10_variable_data2[e10_data_ofs][17 + 0];
        // u64 dot_pos = t->e10_variable_data2[e10_data_ofs][17 + 1];
        // u64 move_pos = t->e10_variable_data2[e10_data_ofs][17 + 2];
        // u64 exp_pos = t->e10_variable_data2[e10_data_ofs][s.dec_sig_len];
        char *buf_origin = buf;
        buf += first_sig_pos;
#if HAS_NEON_OR_SSE2
        memcpy(buf, &(s.ascii16), 16);
#else
        memcpy(buf + 0, &(s.hi), 8);
        memcpy(buf + 8, &(s.lo), 8);
#endif
        memcpy(&buf[15 + D17], &one, 8);
        // byte_move_16(&buf[move_pos], &buf[dot_pos]); // dot_pos+first_sig_pos+sign max = 16+1 = 17; require 17+16=33 byte buffer
        memmove(&buf[move_pos], &buf[dot_pos], 16);
        buf_origin[dot_pos] = '.';
        if (ieee_exponent == 0) [[unlikely]]
        {
            // some subnormal number : range (5e-324,1e-309) = [1e-323,1e-309)
            // if (buf[0] == '0')
            if (m < (u64)1e14)
            {
                u64 lz = 0;
                while (buf[2 + lz] == '0')
                    lz++;
                lz += 2;
                e10 -= lz - 1;
                buf[0] = buf[lz];
                // byte_move_16(&buf[2], &buf[lz + 1]);
                memmove(&buf[2], &buf[lz + 1], 16);
                exp_pos = exp_pos - lz + (exp_pos - lz != 1);
            }
        }
        buf += exp_pos;
        u64 neg = e10 < 0;
        u32 e10_abs = neg ? -e10 : e10;
        u64 e = neg ? ('e' + '-' * 256) : ('e' + '+' * 256);
        int a = (e10_abs * 656) >> 16;                                                           /* e10_abs / 100 */
        int bc = e10_abs + a * (-100);                                                           /* e10_abs % 100 */
        u64 bc_ASCII = (bc << 8) + (1 - 256 * 10) * ((bc * 103) >> 10) + (u64)('0' + '0' * 256); // 12 => "12"
        u64 exp_result = e + (((a > 0) ? a + '0' + (bc_ASCII << 8) : bc_ASCII) << 16);
        exp_result = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : exp_result; // e10_DN<=e10 && e10<=e10_UP : no need to print exponent
        memcpy(buf, &exp_result, 8);
        u64 exp_len = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : (4 + (a > 0));
        return buf + exp_len;
    }
    // static inline
    char *xjb32_comp(float v, char *buf)
    {
        static const struct const_value_float constants_float_comp = {
            .c1 = (((u64)('0' + '0' * 256) << (36 - 1)) + (((u64)1 << (36 - 2)) - 7)),
            .div10000 = 1844674407370956,
            .e7 = 10000000,
            .e6 = 1000000,
            .e5 = 100000,
            .m = (1ull << 32) - 10000,
            .m32_4 = {0x147b000, -100 + 0x10000, 0xce0, -10 + 0x100},
        };
        const struct const_value_float *c = &constants_float_comp;

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
        i64 k;
        // u64 irregular = sig_bin == 1<<23;
        u64 irregular = sig == 0;
        k = (exp_bin * 1233) >> 12;
        if (irregular) [[unlikely]]
            k = (exp_bin * 1233 - 512) >> 12;
        // k = (exp_bin * 1233 - (irregular ? 512 : 0)) >> 12;
        i64 get_e10 = -1 - k;                          // [-32,44]
        i64 h = exp_bin + ((get_e10 * 1701) >> 9);     // [-4,0]
        i64 p10_base_index = (u64)(get_e10 + 32) / 16; // [0,4]
        i64 p10_base = p10_base_index * 16 + (-32);
        u64 p5_off = get_e10 + 32 - p10_base_index * 16; //[0,15]
        const u64 p5_4 = 5 * 5 * 5 * 5;
        static const u64 pow10_base_table_pow5[5 + 2 + 1] = {// 40byte + 24byte = 64byte = cache line size
                                                             // pow10_table
                                                             0xcfb11ead453994bb, // e10 =  -32
                                                             0xe69594bec44de15c, // e10 =  -16
                                                             0x8000000000000000, // e10 =  0
                                                             0x8e1bc9bf04000000, // e10 =  16
                                                             0x9dc5ada82b70b59e, // e10 =  32
                                                                                 // pow5_table , little_endian
                                                             1 + (p5_4 << 32),
                                                             (p5_4 * p5_4) + ((p5_4 * p5_4 * p5_4) << 32),
                                                             (125 << 24) + (25 << 16) + (5 << 8) + 1};
        u64 pow10_base = pow10_base_table_pow5[p10_base_index];
        u64 shift = ((get_e10 * 1701) >> 9) - ((p10_base * 1701) >> 9) - p5_off;
        u32 pow5_base;
        u8 pow5_offset;
        const char *start_ptr = ((char *)pow10_base_table_pow5) + 5 * sizeof(u64);
        const char *start_ptr2 = ((char *)pow10_base_table_pow5) + 7 * sizeof(u64);
        memcpy(&pow5_base, &start_ptr[4 * (p5_off / 4)], 4);
        memcpy(&pow5_offset, &start_ptr2[(p5_off % 4)], 1);
        u64 p5 = (u64)pow5_base * (u64)pow5_offset;
        u64 pow10_hi = ((__uint128_t)pow10_base * p5) >> shift;
        u64 even = ((sig_bin + 1) & 1);
        const int BIT = 36; // [33,36] all right
        u64 cb = sig_bin << (h + 1 + BIT);
        u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
        u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1);
        u64 half_ulp = (pow10_hi >> ((64 - BIT) - h)) + even;
#ifdef __amd64__
        u64 up = (half_ulp + dot_one_36bit) >> BIT; // 1 or 0
#else
        u64 up = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit);
#endif
        u64 down = ((half_ulp >> (irregular)) > dot_one_36bit);
        u64 up_down = up + down;
        u64 m = (sig_hi >> BIT) + up;
        memcpy(buf, "0000", 4);
        u64 lz = (m < 1000000) + (m < 10000000);
        shortest_ascii8 s = to_ascii8(m, up_down, lz, c);
        i64 e10 = k + (8 - lz);
        u64 offset_num = ((u64)('0' + '0' * 256) << (BIT - 1)) + (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
        u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
        if (irregular) [[unlikely]]
            if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150))
                one += 1;
        // one = cmov_branchless(up_down, '0' + '0' * 256, one); // prevent gcc generate branch instruction
        const int e10_DN = -3;
        const int e10_UP = 6;
#if 0
    // size = 12*12 = 144 byte
    static const u8 e10_variable_data[e10_UP-(e10_DN) + 1 + 1][3+9]={
4,1,1,1,2,3,4,5,6,7,8,9,// e10=-3
3,1,1,1,2,3,4,5,6,7,8,9,// e10=-2
2,1,1,1,2,3,4,5,6,7,8,9,// e10=-1
0,1,2,3,3,4,5,6,7,8,9,10,// e10=0
0,2,3,4,4,4,5,6,7,8,9,10,// e10=1
0,3,4,5,5,5,5,6,7,8,9,10,// e10=2
0,4,5,6,6,6,6,6,7,8,9,10,// e10=3
0,5,6,7,7,7,7,7,7,8,9,10,// e10=4
0,6,7,8,8,8,8,8,8,8,9,10,// e10=5
0,7,8,9,9,9,9,9,9,9,9,10,// e10=6
//0,8,9,10,10,10,10,10,10,10,10,10,// e10=7
0,1,2,1,3,4,5,6,7,8,9,10// e10=other
        };
    u64 e10_3 = e10 + (-e10_DN);//convert to unsigned number
    u64 e10_data_ofs = e10_3 < e10_UP-e10_DN+1 ? e10_3 : e10_UP-e10_DN+1;//compute offset
    u64 first_sig_pos = e10_variable_data[e10_data_ofs][0];  // we use lookup table to get first_sig_pos
    u64 dot_pos = e10_variable_data[e10_data_ofs][1];
    u64 move_pos = e10_variable_data[e10_data_ofs][2];
    u64 exp_pos = e10_variable_data[e10_data_ofs][s.dec_sig_len + 3];
#else
        // how to generate branchless code?
        u64 first_sig_pos = (e10_DN <= e10 && e10 <= -1) ? 1 - e10 : 0;
        u64 dot_pos = (0 <= e10 && e10 <= e10_UP) ? 1 + e10 : 1;
        u64 move_pos = dot_pos + (1 - (e10_DN <= e10 && e10 <= -1));
        u64 exp_pos = (0 <= e10 && e10 <= e10_UP) ? (e10 + 3 > s.dec_sig_len + 2 ? e10 + 3 : s.dec_sig_len + 2) : (s.dec_sig_len + 1 + ((1 - (e10_DN <= e10 && e10 <= -1)) > (s.dec_sig_len == 0)));
#endif
        char *buf_origin = (char *)buf;
        buf += first_sig_pos;
        memcpy(buf, &s.ascii, 8);
        memcpy(&buf[8 - lz], &one, 8);
        memmove(&buf[move_pos], &buf[dot_pos], 8);
        buf_origin[dot_pos] = '.';
        if (exp == 0) [[unlikely]]
            if (m < (u32)1e5)
            {
                u64 lz = 0;
                // while(buf[2+lz] == '0')lz++;
                u64 u;
                memcpy(&u, &buf[2], 8);
                u = is_little_endian() ? u : byteswap64(u);
                lz = u64_tz_bits(u & 0x0f0f0f0f0f0f0f0f) / 8;
                lz += 2;
                e10 -= lz - 1;
                buf[0] = buf[lz];
                memmove(&buf[2], &buf[lz + 1], 8);
                exp_pos = exp_pos - lz + (exp_pos - lz != 1);
            }
        // write exponent
        u64 neg = e10 < 0;
        u64 e10_abs = neg ? -e10 : e10;
        u64 e = neg ? ('e' + '-' * 256) | ((u64)('0' + '0' * 256) << 16) : ('e' + '+' * 256) | ((u64)('0' + '0' * 256) << 16);
        u64 e10_BCD = (e10_abs << 24) - ((u64)(256 * 10 - 1) << 16) * ((e10_abs * 103u) >> 10); // 12 => "12"
        u64 exp_result = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : e + e10_BCD;                    // e10_DN<=e10 && e10<=e10_UP : no need to print exponent
        buf += exp_pos;
        memcpy(buf, &exp_result, 8);
        u64 exp_len = (e10_DN <= e10 && e10 <= e10_UP) ? 0 : 4;
        return buf + exp_len;
    }

} // end of namespace xjb_comp
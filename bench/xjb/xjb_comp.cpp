#include <stdint.h>
#include <string.h>
#include <stdio.h>

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
    static inline char *xjb32_comp(float v, char *buf)
    {
        static const struct const_value_float constants_float_comp = {
            .c1 = (((u64)('0' + '0' * 256) << (36 - 1)) + (((u64)1 << (36 - 2)) - 7)),
            .div10000 = 1844674407370956,
            .e7 = 10000000,
            .e6 = 1000000,
            .e5 = 100000,
            .m = (1ull << 32) - 10000,
            // .m32_0 = 0x147b000,
            // .m32_1 = -100 + 0x10000,
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
        static const char *start_ptr = ((char *)pow10_base_table_pow5) + 5 * sizeof(u64);
        static const char *start_ptr2 = ((char *)pow10_base_table_pow5) + 7 * sizeof(u64);
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
        k += 8 - lz;
        i64 e10 = k;
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
0,8,9,10,10,10,10,10,10,10,10,10,// e10=7
0,1,2,1,3,4,5,6,7,8,9,10// e10=other
        };
    u64 e10_3 = e10 + (-e10_DN);//convert to unsigned number
    u64 e10_data_ofs = e10_3 < e10_UP-e10_DN+1 ? e10_3 : e10_UP-e10_DN+1;//compute offset
    u64 first_sig_pos = e10_variable_data[e10_data_ofs][0];  // we use lookup table to get first_sig_pos
    u64 dot_pos = e10_variable_data[e10_data_ofs][1];
    u64 move_pos = e10_variable_data[e10_data_ofs][2];
    u64 exp_pos = e10_variable_data[e10_data_ofs][dec_sig_len_ofs];
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
        //byte_move_8(&buf[move_pos], &buf[dot_pos]);
        memmove(&buf[move_pos], &buf[dot_pos],8);
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
                //byte_move_8(&buf[2], &buf[lz + 1]);
                memmove(&buf[2], &buf[lz + 1],8);
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
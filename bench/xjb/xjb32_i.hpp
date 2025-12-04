#include <stdint.h>
#include <string.h>
static inline void xjb_f32_to_dec(float v,unsigned int* dec,int *e10)
{
    typedef uint64_t u64;
    typedef uint32_t u32;
    unsigned int vi = *(unsigned int*)&v;
    unsigned int ieee_significand = vi & ((1u<<23) - 1);
    //unsigned int ieee_exponent = ((vi >> 23) & 255);
    unsigned int ieee_exponent = ((vi & (255u<<23) ) >> 23);
    // size = 77*8 = 616 byte
    static const u64 pow10_table[(44 - (-32) + 1)] = {
        0xcfb11ead453994bb, // -32
        0x81ceb32c4b43fcf5, // -31
        0xa2425ff75e14fc32, // -30
        0xcad2f7f5359a3b3f, // -29
        0xfd87b5f28300ca0e, // -28
        0x9e74d1b791e07e49, // -27
        0xc612062576589ddb, // -26
        0xf79687aed3eec552, // -25
        0x9abe14cd44753b53, // -24
        0xc16d9a0095928a28, // -23
        0xf1c90080baf72cb2, // -22
        0x971da05074da7bef, // -21
        0xbce5086492111aeb, // -20
        0xec1e4a7db69561a6, // -19
        0x9392ee8e921d5d08, // -18
        0xb877aa3236a4b44a, // -17
        0xe69594bec44de15c, // -16
        0x901d7cf73ab0acda, // -15
        0xb424dc35095cd810, // -14
        0xe12e13424bb40e14, // -13
        0x8cbccc096f5088cc, // -12
        0xafebff0bcb24aaff, // -11
        0xdbe6fecebdedd5bf, // -10
        0x89705f4136b4a598, // -9
        0xabcc77118461cefd, // -8
        0xd6bf94d5e57a42bd, // -7
        0x8637bd05af6c69b6, // -6
        0xa7c5ac471b478424, // -5
        0xd1b71758e219652c, // -4
        0x83126e978d4fdf3c, // -3
        0xa3d70a3d70a3d70b, // -2
        0xcccccccccccccccd, // -1
        0x8000000000000000, // 0
        0xa000000000000000, // 1
        0xc800000000000000, // 2
        0xfa00000000000000, // 3
        0x9c40000000000000, // 4
        0xc350000000000000, // 5
        0xf424000000000000, // 6
        0x9896800000000000, // 7
        0xbebc200000000000, // 8
        0xee6b280000000000, // 9
        0x9502f90000000000, // 10
        0xba43b74000000000, // 11
        0xe8d4a51000000000, // 12
        0x9184e72a00000000, // 13
        0xb5e620f480000000, // 14
        0xe35fa931a0000000, // 15
        0x8e1bc9bf04000000, // 16
        0xb1a2bc2ec5000000, // 17
        0xde0b6b3a76400000, // 18
        0x8ac7230489e80000, // 19
        0xad78ebc5ac620000, // 20
        0xd8d726b7177a8000, // 21
        0x878678326eac9000, // 22
        0xa968163f0a57b400, // 23
        0xd3c21bcecceda100, // 24
        0x84595161401484a0, // 25
        0xa56fa5b99019a5c8, // 26
        0xcecb8f27f4200f3a, // 27
        0x813f3978f8940985, // 28
        0xa18f07d736b90be6, // 29
        0xc9f2c9cd04674edf, // 30
        0xfc6f7c4045812297, // 31
        0x9dc5ada82b70b59e, // 32
        0xc5371912364ce306, // 33
        0xf684df56c3e01bc7, // 34
        0x9a130b963a6c115d, // 35
        0xc097ce7bc90715b4, // 36
        0xf0bdc21abb48db21, // 37
        0x96769950b50d88f5, // 38
        0xbc143fa4e250eb32, // 39
        0xeb194f8e1ae525fe, // 40
        0x92efd1b8d0cf37bf, // 41
        0xb7abc627050305ae, // 42
        0xe596b7b0c643c71a, // 43
        0x8f7e32ce7bea5c70, // 44
    };
    int exp_bin, k;
    u64 sig_bin, regular = ieee_significand > 0;
    u64 irregular = (ieee_significand == 0);
    if (ieee_exponent > 0) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
#ifdef __amd64__
    if (regular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
#else
    k = (exp_bin * 315653 - (irregular ?  131237 : 0 ))>>20;
#endif
    int h = exp_bin + (((-1 - k) * 217707) >> 16); // [-4,-1]
    const u64 *pow10 = &pow10_table[32];
    u64 pow10_hi = pow10[(-1 - k)];
    u64 even = ((ieee_significand + 1) & 1);
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64; // one mulxq instruction on x86
    u64 ten = (sig_hi >> BIT) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
#ifdef __amd64__
    u64 offset_num  = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    if (regular) [[likely]] // branch
    {
        one = (half_ulp + even > dot_one_36bit) ? 0 : one;
        one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        //one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    }
    else
    {
        one = (half_ulp / 2 > dot_one_36bit) ? 0 : one;
        one += (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150);// more fast
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        //one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    }
#else 
    u64 offset_num  = (((u64)1 << BIT) - 7) + ((sig_hi >> (BIT - 4)) & 0xF);
    u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    one = ( ((half_ulp + even) >> irregular) > dot_one_36bit) ? 0 : one;
    one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    if(irregular)[[unlikely]]{
        if( (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150) )
            one+=1;
    }
#endif
    *dec = ten + one;
    *e10 = k;
}

//compress lookup table version
static inline void xjb_comp_f32_to_dec(float v,unsigned int* dec,int *e10)
{
    typedef uint64_t u64;
    typedef uint32_t u32;
    unsigned int vi = *(unsigned int*)&v;
    unsigned int ieee_significand = vi & ((1u<<23) - 1);
    //unsigned int ieee_exponent = ((vi >> 23) & 255);
    unsigned int ieee_exponent = ((vi & (255u<<23) ) >> 23);

    int exp_bin, k;
    u64 sig_bin, regular = ieee_significand > 0;
    u64 irregular = (ieee_significand == 0);
    if (ieee_exponent > 0) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
#ifdef __amd64__
    if (regular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
#else
    k = (exp_bin * 315653 - (irregular ?  131237 : 0 ))>>20;
#endif

    int get_e10 = -1-k;
    int h = exp_bin + ((get_e10 * 217707) >> 16); // [-4,-1]
    u32 p10_base_index = (u32)(get_e10 + 32) / 16;// [0,4]
    int p10_base = p10_base_index * 16 - 32;
    u32 p5_off = get_e10 - p10_base;// [0,15]
    const u64 p5_4 = 5*5*5*5;
    const u32 p5_0_3 = (125<<24) + (25<<16) + (5<<8) + 1; 
    static const u64 pow10_base_table_pow5[5 + 2] = { //40byte + 16byte = 56byte
        0xcfb11ead453994bb, // e10 =  -32
        0xe69594bec44de15c, // e10 =  -16
        0x8000000000000000, // e10 =  0
        0x8e1bc9bf04000000, // e10 =  16
        0x9dc5ada82b70b59e, // e10 =  32
        //pow5_table
        1 + (p5_4<<32),
        ( p5_4*p5_4 ) + ( (p5_4*p5_4*p5_4) << 32)
    };
    u64 pow10_base = pow10_base_table_pow5[p10_base_index]; //-2 to 2
    int shift = ((get_e10*217707) >> 16) - (( p10_base*217707) >> 16) - p5_off;
    // const u32 p5_4 = 5*5*5*5;
    // static const u32 pow5_table[4] = {//16 byte
    //     1,
    //     p5_4,
    //     p5_4*p5_4,
    //     p5_4*p5_4*p5_4
    // };
    u32 pow5_res;
    static const char* start_ptr = ((char*)pow10_base_table_pow5) + 5 * sizeof(u64);
    memcpy(&pow5_res, start_ptr + 4 * (p5_off / 4), 4);
    u64 p5 = (u64)pow5_res * (u64)( (p5_0_3 >> ((p5_off % 4) * 8)) & 0xff );

    // u64 p5 = 1;
    // //while(p5_off-- > 0)p5*=5;
    // u64 p5_base = 5;
    // while(p5_off > 0)
    // {
    //     if (p5_off & 1) {
    //         p5 *= p5_base;
    //     }
    //     p5_base *= p5_base;
    //     p5_off >>= 1;
    // }

    u64 pow10_hi = ( (__uint128_t)pow10_base * p5 ) >> shift;

    u64 even = ((ieee_significand + 1) & 1);
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64; // one mulxq instruction on x86
    u64 ten = (sig_hi >> BIT) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
#ifdef __amd64__
    u64 offset_num  = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    if (regular) [[likely]] // branch
    {
        one = (half_ulp + even > dot_one_36bit) ? 0 : one;
        one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        //one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one; // equal to above line
    }
    else
    {
        one = (half_ulp / 2 > dot_one_36bit) ? 0 : one;
        one += (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150);// more fast
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        //one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    }
#else 
    u64 offset_num  = (((u64)1 << BIT) - 7) + ((sig_hi >> (BIT - 4)) & 0xF);
    u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    one = ( ((half_ulp + even) >> irregular) > dot_one_36bit) ? 0 : one;
    one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    if(irregular)[[unlikely]]{
        if( (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150) )
            one+=1;
    }
#endif
    *dec = ten + one;
    *e10 = k;
}
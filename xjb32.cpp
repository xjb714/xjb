#include <stdint.h>
struct FloatingDecimal32_xjb
{
    uint32_t digits; // num_digits <= 9
    int32_t exponent;
};
static inline FloatingDecimal32_xjb xjb32(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef __uint128_t u128;
    typedef uint64_t u64;
    // size = 77*8 = 616 byte
    const u64 pow10_table[(44 - (-32) + 1)] = {
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
    u64 sig_bin, regular = ieee_significand;
    u64 even = (ieee_significand + 1) & 1;
    if (ieee_exponent) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (regular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h = exp_bin + (((-1 - k) * 217707) >> 16); // [-4,-1]
    u64 pow10 = (&pow10_table[32]) [-1-k];
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (u128)pow10) >> 64; // one mulxq instruction on x86
    u64 ten = (sig_hi >> BIT) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = pow10 >> ((64 - BIT) - h);
    u64 one = (dot_one_36bit * 20 + (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4))) >> (BIT + 1);
    if (regular) [[likely]] // branch
    {
        one = (half_ulp + even > dot_one_36bit) ? 0 : one;
        one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    }
    else
    {
        one = (half_ulp / 2 > dot_one_36bit) ? 0 : one;
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        one += (exp_bin == 31 - 150) + (exp_bin == 214 - 150) + (exp_bin == 217 - 150);// more fast
    }
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ** k) ;
}
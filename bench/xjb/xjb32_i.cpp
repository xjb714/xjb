#include <stdint.h>
#include <string.h>
typedef int64_t i64;
typedef uint64_t u64;
typedef uint32_t u32;
typedef __uint128_t u128;

// the latest version is xjb_v2_f32_to_dec()
// date : 2026.02.14
// author : xjb
// https://github.com/xjb714/xjb

// this function src from https://github.com/vitaut/zmij/blob/main/zmij.cc
// Returns true_value if lhs < rhs, else false_value, without branching.
static inline uint64_t select_if_less_xjb32(uint64_t lhs, uint64_t rhs, uint64_t true_value, uint64_t false_value)
{
#if (defined(__x86_64__) && defined(__GNUC__) && !defined(__clang__))
    asm volatile("cmp %3, %2\n\t"
                 "cmovb %1, %0\n\t"
                 : "+r"(false_value) : "r"(true_value),
                                       "r"(lhs), "r"(rhs) : "cc");
    return false_value;
#else
    return lhs < rhs ? true_value : false_value;
#endif
}

static inline void xjb_v2_f32_to_dec(float v, unsigned int *dec, int *e10)
{
    // this function converts v to shortest decimal : dec * 10**e10
    // not contain inf,NaN. this function has passed all test cases.

    struct float_table_xjb
    {
        u64 pow10_table_reverse[44 - (-32) + 1];
        unsigned char h37[256];
    };
    static const struct float_table_xjb float_table_data = {
        .pow10_table_reverse = {
            0x8f7e32ce7bea5c70, // 44
            0xe596b7b0c643c71a, // 43
            0xb7abc627050305ae, // 42
            0x92efd1b8d0cf37bf, // 41
            0xeb194f8e1ae525fe, // 40
            0xbc143fa4e250eb32, // 39
            0x96769950b50d88f5, // 38
            0xf0bdc21abb48db21, // 37
            0xc097ce7bc90715b4, // 36
            0x9a130b963a6c115d, // 35
            0xf684df56c3e01bc7, // 34
            0xc5371912364ce306, // 33
            0x9dc5ada82b70b59e, // 32
            0xfc6f7c4045812297, // 31
            0xc9f2c9cd04674edf, // 30
            0xa18f07d736b90be6, // 29
            0x813f3978f8940985, // 28
            0xcecb8f27f4200f3a, // 27
            0xa56fa5b99019a5c8, // 26
            0x84595161401484a0, // 25
            0xd3c21bcecceda100, // 24
            0xa968163f0a57b400, // 23
            0x878678326eac9000, // 22
            0xd8d726b7177a8000, // 21
            0xad78ebc5ac620000, // 20
            0x8ac7230489e80000, // 19
            0xde0b6b3a76400000, // 18
            0xb1a2bc2ec5000000, // 17
            0x8e1bc9bf04000000, // 16
            0xe35fa931a0000000, // 15
            0xb5e620f480000000, // 14
            0x9184e72a00000000, // 13
            0xe8d4a51000000000, // 12
            0xba43b74000000000, // 11
            0x9502f90000000000, // 10
            0xee6b280000000000, // 9
            0xbebc200000000000, // 8
            0x9896800000000000, // 7
            0xf424000000000000, // 6
            0xc350000000000000, // 5
            0x9c40000000000000, // 4
            0xfa00000000000000, // 3
            0xc800000000000000, // 2
            0xa000000000000000, // 1
            0x8000000000000000, // 0
            0xcccccccccccccccd, // -1
            0xa3d70a3d70a3d70b, // -2
            0x83126e978d4fdf3c, // -3
            0xd1b71758e219652c, // -4
            0xa7c5ac471b478424, // -5
            0x8637bd05af6c69b6, // -6
            0xd6bf94d5e57a42bd, // -7
            0xabcc77118461cefd, // -8
            0x89705f4136b4a598, // -9
            0xdbe6fecebdedd5bf, // -10
            0xafebff0bcb24aaff, // -11
            0x8cbccc096f5088cc, // -12
            0xe12e13424bb40e14, // -13
            0xb424dc35095cd810, // -14
            0x901d7cf73ab0acda, // -15
            0xe69594bec44de15c, // -16
            0xb877aa3236a4b44a, // -17
            0x9392ee8e921d5d08, // -18
            0xec1e4a7db69561a6, // -19
            0xbce5086492111aeb, // -20
            0x971da05074da7bef, // -21
            0xf1c90080baf72cb2, // -22
            0xc16d9a0095928a28, // -23
            0x9abe14cd44753b53, // -24
            0xf79687aed3eec552, // -25
            0xc612062576589ddb, // -26
            0x9e74d1b791e07e49, // -27
            0xfd87b5f28300ca0e, // -28
            0xcad2f7f5359a3b3f, // -29
            0xa2425ff75e14fc32, // -30
            0x81ceb32c4b43fcf5, // -31
            0xcfb11ead453994bb, // -32
        },
        .h37 = {34, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34, 35, 36, 34, 35, 36, 34, 35, 36, 34, 35, 36, 33, 34}};
    const u64 *pow10_reverse = &(float_table_data.pow10_table_reverse[45]);
    const unsigned char *h37 = &(float_table_data.h37[0]);

    const int BIT = 36;

    u32 vi = *(u32 *)&v;
    u64 sig = vi & ((1u << 23) - 1);
    u64 exp = (vi << 1) >> 24;

    u64 irregular = (sig == 0);
    u64 c = sig | (1 << 23);
    i64 q = exp - 150;
    if (exp == 0) [[unlikely]]
    {
        if (sig == 0)
        {
            *dec = 0;
            *e10 = 0;
            return;
        }
        q = -149;
        c = sig;

        // this is subnormal number , branch instruction are more efficient than cmov. but some compilers still generate cmov.
        // such as apple clang 17.0.0, apple M1 generate cmov.
        // we use following code to prevent compiler optimization.
        //         const i64 q_min = -149;
        //         const i64* q_min_ptr = (i64 *)&q_min;
        // #if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // prevent compiler optimization , generate branch instruction
        // 		asm("" : "+r"(q_min_ptr));
        // #endif
        //         q = *q_min_ptr; // equal to q = -149;
        //         c = sig;
    }
    // if (!irregular)
    // {
    //     u64 h37_precalc = h37[exp]; // precalc h + 37;
    //     i64 k = ((i64)q * (u128)(1233ull << 52)) >> 64; // equal to k = (q * 1233) >> 12; arm64 emit smulh instruction,x64 emit imulq instruction.
    //     //i64 k = (q * 1233) >> 12;
    //     // i64 h = q + ((k * -1701 - 1701) >> 9);
    //     u64 pow10_hi = pow10_reverse[k]; // get 10^(-k-1)
    //     // u64 cb = c << (h + 1 + BIT);
    //     u64 cb = c << h37_precalc;
    //     u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
    //     u64 dot_one_36bit = sig_hi & ((1ULL << BIT) - 1);
    //     // u64 half_ulp = (pow10_hi >> ((64 - BIT) - h) ) + ((sig + 1) & 1);
    //     u64 half_ulp = (pow10_hi >> (65 - h37_precalc)) + ((sig + 1) & 1);
    //     u64 ten = (sig_hi >> BIT) * 10;
    //     //u64 ten = ((sig_hi + half_ulp) >> BIT) * 10;
    //     u64 offset_num = ((1ULL << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    //     u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);

    //     one = (half_ulp > dot_one_36bit) ? 0 : one;
    //     one = (half_ulp > ((1ULL << BIT) - 1) - dot_one_36bit) ? 0 : one;

    //     *dec = ten + one;
    //     *e10 = k;
    // }
    {
        u64 h37_precalc = h37[exp];                     // precalc h + 37;
        i64 k = ((i64)q * (u128)(1233ull << 52)) >> 64; // equal to k = (q * 1233) >> 12; arm64 emit smulh instruction,x64 emit imulq instruction.
        u64 pow10_hi = pow10_reverse[k];                // get 10^(-k-1)
        u64 cb = c << h37_precalc;
        u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
        u64 half_ulp = (pow10_hi >> (65 - h37_precalc)) + ((sig + 1) & 1);
        u64 shorter = ((sig_hi + half_ulp) >> BIT) * 10;
#if defined(__aarch64__)
        // maybe madd instruction is more efficient;
        // u64 longer = (sig_hi * 10 + ((1ULL << (BIT - 1)) - 3) + ((sig_hi >> 34) & 3) ) >> BIT;
        u64 longer = (sig_hi * 10 + ((1ULL << (BIT - 1)) - 7) + ((sig_hi >> 32) & 15)) >> BIT;
#else
        u64 longer = (sig_hi * 5 + ((1ULL << (BIT - 2)) - 7) + ((sig_hi >> 32) & 15)) >> (BIT - 1);
#endif

        // u64 up_down = ((sig_hi + half_ulp) >> BIT) > ((sig_hi - half_ulp) >> BIT);
        // *dec = up_down ? shorter : longer; // cmov instruction is more efficient than branch instruction. gcc not generate cmov.

        *dec = select_if_less_xjb32(((sig_hi - half_ulp) >> BIT), ((sig_hi + half_ulp) >> BIT), shorter, longer);
        *e10 = k;
    }
    if (irregular)
    {
        // irregular case : only conatin 254 value ; c = 1 << 23; exp range is [1,254]
        i64 k = (i64)(q * 1233 - 512) >> 12;
        i64 h = q + ((k * -1701 - 1701) >> 9);
        u64 pow10_hi = pow10_reverse[k]; // get 10^(-k-1)
        u64 sig_hi = pow10_hi >> (4 - h);
        u64 ten = (sig_hi >> BIT) * 10;
        u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
        u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
        u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
        u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
        one = (((half_ulp) >> 1) > dot_one_36bit) ? 0 : one;
        if ((q == 31 - 150) || (q == 214 - 150) || (q == 217 - 150))
            one++;
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        *dec = ten + one;
        *e10 = k;
    }
    // return;

    //     // if (ieee_exponent > 0) [[likely]] // branch
    //     // {
    //     //     exp_bin = ieee_exponent - 150; //-127-23
    //     //     sig_bin = ieee_significand | (1 << 23);
    //     // }
    //     // else
    //     // {
    //     //     exp_bin = 1 - 150;
    //     //     sig_bin = ieee_significand;
    //     // }
    //     // u64 irregular = sig_bin == 1<<23;
    // #ifdef __amd64__
    //     if (regular) [[likely]] // branch
    //         k = (exp_bin * 1233) >> 12;
    //     else
    //         k = (exp_bin * 1233 - 512) >> 12;
    // #else
    //     //k = (exp_bin * 1233 + (irregular ? -512 : 0)) >> 12;
    //     k = ((i64)exp_bin * (u128)(1233ull<< 52)) >> 64;
    // //     if (regular) [[likely]] // branch
    // //     k = (exp_bin * 1233) >> 12;
    // // else
    // //     k = (exp_bin * 1233 - 512) >> 12;
    //     // if(irregular)[[unlikely]]k = (exp_bin * 1233 - 512) >> 12;
    //     // else k = ((i64)exp_bin * (u128)(1233ull<< 52)) >> 64;
    // //     k = (exp_bin * 1233) >> 12;
    // //     if(irregular)[[unlikely]]
    // //     {
    // //         static const int offset_num = -512;
    // //         const int* n = &offset_num;
    // // #if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // for arm64 processor , fewer instructions , MSVC not support inline asm
    // //         asm("" : "+r"(n));                                            // read constant values from memory to register
    // // #endif
    // //         k = (exp_bin * 1233 + *n) >> 12;
    // //     }
    // #endif
    //     // int h = exp_bin + (((-1 - k) * 217707) >> 16);
    //     i64 h = exp_bin + ((k * -1701 - 1701) >> 9);
    //     // const u64 *pow10 = &pow10_table[32];
    //     // u64 pow10_hi = pow10[(-1 - k)];
    //     const u64 *pow10_reverse = &pow10_table_reverse[45];
    //     u64 pow10_hi = pow10_reverse[k]; // get 10^(-k-1)
    //     u64 even = ((sig_bin + 1) & 1);
    //     const int BIT = 36; // [33,36] all right
    //     u64 cb = sig_bin << (h + 1 + BIT);
    //     u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
    //     u64 ten = (sig_hi >> BIT) * 10;
    //     u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    //     u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
    // #ifdef __amd64__
    //     // u64 offset_num  = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    //     // u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);

    //     u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    //     u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);

    //     if (regular) [[likely]] // branch
    //     {
    //         one = (half_ulp + even > dot_one_36bit) ? 0 : one;
    //         one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    //         // one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    //     }
    //     else
    //     {
    //         one = (half_ulp / 2 > dot_one_36bit) ? 0 : one;
    //         one += (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150); // more fast
    //         one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    //         // one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    //     }
    // #else
    //     // u64 offset_num  = (((u64)1 << BIT) - 7) + ((sig_hi >> (BIT - 4)) & 0xF);
    //     // u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    //     u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    //     u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);

    //     //one = (((half_ulp + even) >> irregular) > dot_one_36bit) ? 0 : one;
    //     //one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    //     if (irregular) [[unlikely]]
    //     {
    //         one = (((half_ulp + even) >> 1) > dot_one_36bit) ? 0 : one;
    //         //one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    //         if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150))
    //             one += 1;
    //     }else{
    //         one = (((half_ulp + even) >> 0) > dot_one_36bit) ? 0 : one;
    //         //one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    //     }
    //     one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    // #endif
    //     *dec = ten + one;
    //     *e10 = k;
}
static inline void xjb_f32_to_dec(float v, unsigned int *dec, int *e10)
{
    typedef int64_t i64;
    typedef uint64_t u64;
    typedef uint32_t u32;
    u32 vi = *(u32 *)&v;
    u64 ieee_significand = vi & ((1u << 23) - 1);
    u64 ieee_exponent = (vi << 1) >> 24;
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
    static const u64 pow10_table_reverse[(44 - (-32) + 1)] = {
        0x8f7e32ce7bea5c70, // 44
        0xe596b7b0c643c71a, // 43
        0xb7abc627050305ae, // 42
        0x92efd1b8d0cf37bf, // 41
        0xeb194f8e1ae525fe, // 40
        0xbc143fa4e250eb32, // 39
        0x96769950b50d88f5, // 38
        0xf0bdc21abb48db21, // 37
        0xc097ce7bc90715b4, // 36
        0x9a130b963a6c115d, // 35
        0xf684df56c3e01bc7, // 34
        0xc5371912364ce306, // 33
        0x9dc5ada82b70b59e, // 32
        0xfc6f7c4045812297, // 31
        0xc9f2c9cd04674edf, // 30
        0xa18f07d736b90be6, // 29
        0x813f3978f8940985, // 28
        0xcecb8f27f4200f3a, // 27
        0xa56fa5b99019a5c8, // 26
        0x84595161401484a0, // 25
        0xd3c21bcecceda100, // 24
        0xa968163f0a57b400, // 23
        0x878678326eac9000, // 22
        0xd8d726b7177a8000, // 21
        0xad78ebc5ac620000, // 20
        0x8ac7230489e80000, // 19
        0xde0b6b3a76400000, // 18
        0xb1a2bc2ec5000000, // 17
        0x8e1bc9bf04000000, // 16
        0xe35fa931a0000000, // 15
        0xb5e620f480000000, // 14
        0x9184e72a00000000, // 13
        0xe8d4a51000000000, // 12
        0xba43b74000000000, // 11
        0x9502f90000000000, // 10
        0xee6b280000000000, // 9
        0xbebc200000000000, // 8
        0x9896800000000000, // 7
        0xf424000000000000, // 6
        0xc350000000000000, // 5
        0x9c40000000000000, // 4
        0xfa00000000000000, // 3
        0xc800000000000000, // 2
        0xa000000000000000, // 1
        0x8000000000000000, // 0
        0xcccccccccccccccd, // -1
        0xa3d70a3d70a3d70b, // -2
        0x83126e978d4fdf3c, // -3
        0xd1b71758e219652c, // -4
        0xa7c5ac471b478424, // -5
        0x8637bd05af6c69b6, // -6
        0xd6bf94d5e57a42bd, // -7
        0xabcc77118461cefd, // -8
        0x89705f4136b4a598, // -9
        0xdbe6fecebdedd5bf, // -10
        0xafebff0bcb24aaff, // -11
        0x8cbccc096f5088cc, // -12
        0xe12e13424bb40e14, // -13
        0xb424dc35095cd810, // -14
        0x901d7cf73ab0acda, // -15
        0xe69594bec44de15c, // -16
        0xb877aa3236a4b44a, // -17
        0x9392ee8e921d5d08, // -18
        0xec1e4a7db69561a6, // -19
        0xbce5086492111aeb, // -20
        0x971da05074da7bef, // -21
        0xf1c90080baf72cb2, // -22
        0xc16d9a0095928a28, // -23
        0x9abe14cd44753b53, // -24
        0xf79687aed3eec552, // -25
        0xc612062576589ddb, // -26
        0x9e74d1b791e07e49, // -27
        0xfd87b5f28300ca0e, // -28
        0xcad2f7f5359a3b3f, // -29
        0xa2425ff75e14fc32, // -30
        0x81ceb32c4b43fcf5, // -31
        0xcfb11ead453994bb, // -32
    };
    i64 exp_bin, k;
    u64 sig_bin, regular = ieee_significand > 0;
    u64 irregular = (ieee_significand == 0);
    exp_bin = ieee_exponent - 150; //-127-23
    sig_bin = ieee_significand | (1 << 23);
    if (ieee_exponent == 0) [[unlikely]]
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    // if (ieee_exponent > 0) [[likely]] // branch
    // {
    //     exp_bin = ieee_exponent - 150; //-127-23
    //     sig_bin = ieee_significand | (1 << 23);
    // }
    // else
    // {
    //     exp_bin = 1 - 150;
    //     sig_bin = ieee_significand;
    // }
    // u64 irregular = sig_bin == 1<<23;
#ifdef __amd64__
    if (regular) [[likely]] // branch
        k = (exp_bin * 1233) >> 12;
    else
        k = (exp_bin * 1233 - 512) >> 12;
#else
    k = (exp_bin * 1233 + (irregular ? -512 : 0)) >> 12;
    // k = ((i64)exp_bin * (u128)(1233ull<< 52)) >> 64;
//     if (regular) [[likely]] // branch
//     k = (exp_bin * 1233) >> 12;
// else
//     k = (exp_bin * 1233 - 512) >> 12;
// if(irregular)[[unlikely]]k = (exp_bin * 1233 - 512) >> 12;
// else k = ((i64)exp_bin * (u128)(1233ull<< 52)) >> 64;
//     k = (exp_bin * 1233) >> 12;
//     if(irregular)[[unlikely]]
//     {
//         static const int offset_num = -512;
//         const int* n = &offset_num;
// #if defined(__aarch64__) && (defined(__clang__) || defined(__GNUC__)) // for arm64 processor , fewer instructions , MSVC not support inline asm
//         asm("" : "+r"(n));                                            // read constant values from memory to register
// #endif
//         k = (exp_bin * 1233 + *n) >> 12;
//     }
#endif
    // int h = exp_bin + (((-1 - k) * 217707) >> 16);
    i64 h = exp_bin + ((k * -1701 - 1701) >> 9);
    // const u64 *pow10 = &pow10_table[32];
    // u64 pow10_hi = pow10[(-1 - k)];
    const u64 *pow10_reverse = &pow10_table_reverse[45];
    u64 pow10_hi = pow10_reverse[k]; // get 10^(-k-1)
    u64 even = ((sig_bin + 1) & 1);
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64;
    u64 ten = (sig_hi >> BIT) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
#ifdef __amd64__
    // u64 offset_num  = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    // u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);

    u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);

    if (regular) [[likely]] // branch
    {
        one = (half_ulp + even > dot_one_36bit) ? 0 : one;
        one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        // one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    }
    else
    {
        one = (half_ulp / 2 > dot_one_36bit) ? 0 : one;
        one += (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150); // more fast
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        // one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    }
#else
    // u64 offset_num  = (((u64)1 << BIT) - 7) + ((sig_hi >> (BIT - 4)) & 0xF);
    // u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);

    // one = (((half_ulp + even) >> irregular) > dot_one_36bit) ? 0 : one;
    // one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    if (irregular) [[unlikely]]
    {
        one = (((half_ulp + even) >> 1) > dot_one_36bit) ? 0 : one;
        // one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150))
            one += 1;
    }
    else
    {
        one = (((half_ulp + even) >> 0) > dot_one_36bit) ? 0 : one;
        // one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    }
    one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
#endif
    *dec = ten + one;
    *e10 = k;
}

// compress lookup table version
static inline void xjb_comp_f32_to_dec(float v, unsigned int *dec, int *e10)
{
    typedef uint64_t u64;
    typedef uint32_t u32;
    u32 vi = *(u32 *)&v;
    u32 ieee_significand = vi & ((1u << 23) - 1);
    u32 ieee_exponent = (vi << 1) >> 24;

    int exp_bin, k;
    u64 sig_bin, regular = ieee_significand > 0;
    u64 irregular = (ieee_significand == 0);
    exp_bin = ieee_exponent - 150; //-127-23
    sig_bin = ieee_significand | (1 << 23);
    if (ieee_exponent == 0) [[unlikely]]
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    // if (ieee_exponent > 0) [[likely]] // branch
    // {
    //     exp_bin = ieee_exponent - 150; //-127-23
    //     sig_bin = ieee_significand | (1 << 23);
    // }
    // else
    // {
    //     exp_bin = 1 - 150;
    //     sig_bin = ieee_significand;
    // }
#ifdef __amd64__
    if (regular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
#else
    k = (exp_bin * 1233 - (irregular ? 512 : 0)) >> 12;
#endif

    int get_e10 = -1 - k;
    int h = exp_bin + ((get_e10 * 1701) >> 9);     // [-4,0]
    u32 p10_base_index = (u32)(get_e10 + 32) / 16; // [0,4]
    int p10_base = p10_base_index * 16 - 32;
    u32 p5_off = get_e10 - p10_base; // [0,15]
    const u64 p5_4 = 5 * 5 * 5 * 5;
    const u32 p5_0_3 = (125 << 24) + (25 << 16) + (5 << 8) + 1;
    static const u64 pow10_base_table_pow5[5 + 2 + 1] = {
        // pow10_table
        0xcfb11ead453994bb, // e10 =  -32
        0xe69594bec44de15c, // e10 =  -16
        0x8000000000000000, // e10 =  0
        0x8e1bc9bf04000000, // e10 =  16
        0x9dc5ada82b70b59e, // e10 =  32
        // pow5_table
        1 + (p5_4 << 32),
        (p5_4 * p5_4) + ((p5_4 * p5_4 * p5_4) << 32),
        (125 << 24) + (25 << 16) + (5 << 8) + 1};
    u64 pow10_base = pow10_base_table_pow5[p10_base_index]; //-2 to 2
    int shift = ((get_e10 * 1701) >> 9) - p5_off - ((p10_base * 1701) >> 9);
    u32 pow5_base;
    char pow5_offset;
    const char *start_ptr = ((char *)pow10_base_table_pow5) + 5 * sizeof(u64);
    const char *start_ptr2 = ((char *)pow10_base_table_pow5) + 7 * sizeof(u64);
    memcpy(&pow5_base, &start_ptr[4 * (p5_off / 4)], 4);
    memcpy(&pow5_offset, &start_ptr2[(p5_off % 4)], 1);
    // u64 p5 = (u64)pow5_base * (u64)( (p5_0_3 >> ((p5_off % 4) * 8)) & 0xff );// p5 = 5**p5_off
    u64 p5 = (u64)pow5_base * (u64)pow5_offset;

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

    u64 pow10_hi = ((__uint128_t)pow10_base * p5) >> shift;

    u64 even = ((sig_bin + 1) & 1);
    const int BIT = 36; // [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);
    u64 sig_hi = (cb * (__uint128_t)pow10_hi) >> 64; // one mulxq instruction on x86
    u64 ten = (sig_hi >> BIT) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
#ifdef __amd64__
    // u64 offset_num  = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    // u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);
    if (regular) [[likely]] // branch
    {
        one = (half_ulp + even > dot_one_36bit) ? 0 : one;
        one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        // one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one; // equal to above line
    }
    else
    {
        one = (half_ulp / 2 > dot_one_36bit) ? 0 : one;
        one += (exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150); // more fast
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
        // one = ( (half_ulp + even + dot_one_36bit) >> BIT ) ? 10 : one;
    }
#else
    // u64 offset_num  = (((u64)1 << BIT) - 7) + ((sig_hi >> (BIT - 4)) & 0xF);
    // u64 one = (dot_one_36bit * 20 + offset_num) >> (BIT + 1);
    u64 offset_num = (((u64)1 << (BIT - 2)) - 7) + (dot_one_36bit >> (BIT - 4));
    u64 one = (dot_one_36bit * 5 + offset_num) >> (BIT - 1);

    one = (((half_ulp + even) >> irregular) > dot_one_36bit) ? 0 : one;
    one = (half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one;
    if (irregular) [[unlikely]]
    {
        if ((exp_bin == 31 - 150) | (exp_bin == 214 - 150) | (exp_bin == 217 - 150))
            one += 1;
    }
#endif
    *dec = ten + one;
    *e10 = k;
}

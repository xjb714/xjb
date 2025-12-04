// Copyright 2020 Alexander Bolz
//
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "schubfach_32.h"

//--------------------------------------------------------------------------------------------------
// This file contains an implementation of the Schubfach algorithm as described in
//
// [1] Raffaello Giulietti, "The Schubfach way to render doubles",
//     https://drive.google.com/open?id=1luHhyQF9zKlM8yJ1nebU0OgVYhfC6CBN
//--------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <cassert>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <limits>
#if _MSC_VER
#include <intrin.h>
#endif

#ifndef SF_ASSERT
#define SF_ASSERT(X) assert(X)
#endif

//==================================================================================================
//
//==================================================================================================
namespace schubfach_32
{
template <typename Dest, typename Source>
static inline Dest ReinterpretBits(Source source)
{
    static_assert(sizeof(Dest) == sizeof(Source), "size mismatch");

    Dest dest;
    std::memcpy(&dest, &source, sizeof(Source));
    return dest;
}

namespace
{
    struct Single
    {
        static_assert(std::numeric_limits<float>::is_iec559 && std::numeric_limits<float>::digits == 24 && std::numeric_limits<float>::max_exponent == 128,
                      "IEEE-754 single-precision implementation required");

        using value_type = float;
        using bits_type = uint32_t;

        //  static constexpr int32_t   MaxDigits10     = std::numeric_limits<value_type>::max_digits10;
        static constexpr int32_t SignificandSize = std::numeric_limits<value_type>::digits; // = p   (includes the hidden bit)
        static constexpr int32_t ExponentBias = std::numeric_limits<value_type>::max_exponent - 1 + (SignificandSize - 1);
        //  static constexpr int32_t   MaxExponent     = std::numeric_limits<value_type>::max_exponent - 1 - (SignificandSize - 1);
        //  static constexpr int32_t   MinExponent     = std::numeric_limits<value_type>::min_exponent - 1 - (SignificandSize - 1);
        static constexpr bits_type MaxIeeeExponent = bits_type{2 * std::numeric_limits<value_type>::max_exponent - 1};
        static constexpr bits_type HiddenBit = bits_type{1} << (SignificandSize - 1); // = 2^(p-1)
        static constexpr bits_type SignificandMask = HiddenBit - 1;                   // = 2^(p-1) - 1
        static constexpr bits_type ExponentMask = MaxIeeeExponent << (SignificandSize - 1);
        static constexpr bits_type SignMask = ~(~bits_type{0} >> 1);

        bits_type bits;

        explicit Single(bits_type bits_) : bits(bits_) {}
        explicit Single(value_type value) : bits(ReinterpretBits<bits_type>(value)) {}

        bits_type PhysicalSignificand() const
        {
            return bits & SignificandMask;
        }

        bits_type PhysicalExponent() const
        {
            return (bits & ExponentMask) >> (SignificandSize - 1);
        }

        bool IsFinite() const
        {
            return (bits & ExponentMask) != ExponentMask;
        }

        bool IsInf() const
        {
            return (bits & ExponentMask) == ExponentMask && (bits & SignificandMask) == 0;
        }

        bool IsNaN() const
        {
            return (bits & ExponentMask) == ExponentMask && (bits & SignificandMask) != 0;
        }

        bool IsZero() const
        {
            return (bits & ~SignMask) == 0;
        }

        bool SignBit() const
        {
            return (bits & SignMask) != 0;
        }
    };
} // namespace

//==================================================================================================
//
//==================================================================================================

// Returns floor(x / 2^n).
//
// Technically, right-shift of negative integers is implementation defined...
// Should easily be optimized into SAR (or equivalent) instruction.
static inline int32_t FloorDivPow2(int32_t x, int32_t n)
{
#if 0
    return x < 0 ? ~(~x >> n) : (x >> n);
#else
    return x >> n;
#endif
}

// Returns floor(log_10(2^e))
// static inline int32_t FloorLog10Pow2(int32_t e)
// {
//     SF_ASSERT(e >= -1500);
//     SF_ASSERT(e <=  1500);
//     return FloorDivPow2(e * 1262611, 22);
// }

// Returns floor(log_10(3/4 2^e))
// static inline int32_t FloorLog10ThreeQuartersPow2(int32_t e)
// {
//     SF_ASSERT(e >= -1500);
//     SF_ASSERT(e <=  1500);
//     return FloorDivPow2(e * 1262611 - 524031, 22);
// }

// Returns floor(log_2(10^e))
static inline int32_t FloorLog2Pow10(int32_t e)
{
    // SF_ASSERT(e >= -1233);
    // SF_ASSERT(e <= 1233);
    return FloorDivPow2(e * 1741647, 19);
}

//==================================================================================================
//
//==================================================================================================

static inline uint64_t ComputePow10_Single(int32_t k)
{
    // There are unique beta and r such that 10^k = beta 2^r and
    // 2^63 <= beta < 2^64, namely r = floor(log_2 10^k) - 63 and
    // beta = 2^-r 10^k.
    // Let g = ceil(beta), so (g-1) 2^r < 10^k <= g 2^r, with the latter
    // value being a pretty good overestimate for 10^k.

    // NB: Since for all the required exponents k, we have g < 2^64,
    //     all constants can be stored in 128-bit integers.

    static constexpr int32_t kMin = -31;
    static constexpr int32_t kMax = 45;
    static constexpr uint64_t g[kMax - kMin + 1] = {
        0x81CEB32C4B43FCF5, // -31
        0xA2425FF75E14FC32, // -30
        0xCAD2F7F5359A3B3F, // -29
        0xFD87B5F28300CA0E, // -28
        0x9E74D1B791E07E49, // -27
        0xC612062576589DDB, // -26
        0xF79687AED3EEC552, // -25
        0x9ABE14CD44753B53, // -24
        0xC16D9A0095928A28, // -23
        0xF1C90080BAF72CB2, // -22
        0x971DA05074DA7BEF, // -21
        0xBCE5086492111AEB, // -20
        0xEC1E4A7DB69561A6, // -19
        0x9392EE8E921D5D08, // -18
        0xB877AA3236A4B44A, // -17
        0xE69594BEC44DE15C, // -16
        0x901D7CF73AB0ACDA, // -15
        0xB424DC35095CD810, // -14
        0xE12E13424BB40E14, // -13
        0x8CBCCC096F5088CC, // -12
        0xAFEBFF0BCB24AAFF, // -11
        0xDBE6FECEBDEDD5BF, // -10
        0x89705F4136B4A598, //  -9
        0xABCC77118461CEFD, //  -8
        0xD6BF94D5E57A42BD, //  -7
        0x8637BD05AF6C69B6, //  -6
        0xA7C5AC471B478424, //  -5
        0xD1B71758E219652C, //  -4
        0x83126E978D4FDF3C, //  -3
        0xA3D70A3D70A3D70B, //  -2
        0xCCCCCCCCCCCCCCCD, //  -1
        0x8000000000000000, //   0
        0xA000000000000000, //   1
        0xC800000000000000, //   2
        0xFA00000000000000, //   3
        0x9C40000000000000, //   4
        0xC350000000000000, //   5
        0xF424000000000000, //   6
        0x9896800000000000, //   7
        0xBEBC200000000000, //   8
        0xEE6B280000000000, //   9
        0x9502F90000000000, //  10
        0xBA43B74000000000, //  11
        0xE8D4A51000000000, //  12
        0x9184E72A00000000, //  13
        0xB5E620F480000000, //  14
        0xE35FA931A0000000, //  15
        0x8E1BC9BF04000000, //  16
        0xB1A2BC2EC5000000, //  17
        0xDE0B6B3A76400000, //  18
        0x8AC7230489E80000, //  19
        0xAD78EBC5AC620000, //  20
        0xD8D726B7177A8000, //  21
        0x878678326EAC9000, //  22
        0xA968163F0A57B400, //  23
        0xD3C21BCECCEDA100, //  24
        0x84595161401484A0, //  25
        0xA56FA5B99019A5C8, //  26
        0xCECB8F27F4200F3A, //  27
        0x813F3978F8940985, //  28
        0xA18F07D736B90BE6, //  29
        0xC9F2C9CD04674EDF, //  30
        0xFC6F7C4045812297, //  31
        0x9DC5ADA82B70B59E, //  32
        0xC5371912364CE306, //  33
        0xF684DF56C3E01BC7, //  34
        0x9A130B963A6C115D, //  35
        0xC097CE7BC90715B4, //  36
        0xF0BDC21ABB48DB21, //  37
        0x96769950B50D88F5, //  38
        0xBC143FA4E250EB32, //  39
        0xEB194F8E1AE525FE, //  40
        0x92EFD1B8D0CF37BF, //  41
        0xB7ABC627050305AE, //  42
        0xE596B7B0C643C71A, //  43
        0x8F7E32CE7BEA5C70, //  44
        0xB35DBF821AE4F38C, //  45
    };

    // SF_ASSERT(k >= kMin);
    // SF_ASSERT(k <= kMax);
    return g[static_cast<uint32_t>(k - kMin)];
}
static inline uint64_t ComputePow10_Single_down(int32_t k)
{
    // There are unique beta and r such that 10^k = beta 2^r and
    // 2^63 <= beta < 2^64, namely r = floor(log_2 10^k) - 63 and
    // beta = 2^-r 10^k.
    // Let g = ceil(beta), so (g-1) 2^r < 10^k <= g 2^r, with the latter
    // value being a pretty good overestimate for 10^k.

    // NB: Since for all the required exponents k, we have g < 2^64,
    //     all constants can be stored in 128-bit integers.

    static constexpr int32_t kMin = -31;
    static constexpr int32_t kMax = 45;
    static constexpr uint64_t g[kMax - kMin + 1] = {
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
        0xb35dbf821ae4f38c, // 45
    };

    // SF_ASSERT(k >= kMin);
    // SF_ASSERT(k <= kMax);
    return g[static_cast<uint32_t>(k - kMin)];
}

static inline uint32_t Lo32(uint64_t x)
{
    return static_cast<uint32_t>(x);
}

static inline uint32_t Hi32(uint64_t x)
{
    return static_cast<uint32_t>(x >> 32);
}

#if defined(__SIZEOF_INT128__)

static inline uint32_t RoundToOdd(uint64_t g, uint32_t cp)
{
    __extension__ using uint128_t = unsigned __int128;

    const uint128_t p = uint128_t{g} * cp;

    const uint32_t y1 = Lo32(static_cast<uint64_t>(p >> 64));
    const uint32_t y0 = Hi32(static_cast<uint64_t>(p));

    return y1 | (y0 > 1);
}

#elif defined(_MSC_VER) && defined(_M_X64)

static inline uint32_t RoundToOdd(uint64_t g, uint32_t cpHi)
{
    uint64_t p1 = 0;
    uint64_t p0 = _umul128(g, cpHi, &p1);

    const uint32_t y1 = Lo32(p1);
    const uint32_t y0 = Hi32(p0);

    return y1 | (y0 > 1);
}

#else

static inline uint32_t RoundToOdd(uint64_t g, uint32_t cp)
{
    const uint64_t b01 = uint64_t{Lo32(g)} * cp;
    const uint64_t b11 = uint64_t{Hi32(g)} * cp;
    const uint64_t hi = b11 + Hi32(b01);

    const uint32_t y1 = Hi32(hi);
    const uint32_t y0 = Lo32(hi);

    return y1 | (y0 > 1);
}

#endif

// Returns whether value is divisible by 2^e2
static inline bool MultipleOfPow2(uint32_t value, int32_t e2)
{
    // SF_ASSERT(e2 >= 0);
    // SF_ASSERT(e2 <= 31);
    return (value & ((uint32_t{1} << e2) - 1)) == 0;
}

namespace
{
    struct FloatingDecimal32
    {
        uint32_t digits; // num_digits <= 9
        int32_t exponent;
    };
}

static inline FloatingDecimal32 ToDecimal32(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    uint32_t c;
    int32_t q;
    if (ieee_exponent != 0)
    {
        c = Single::HiddenBit | ieee_significand;
        q = static_cast<int32_t>(ieee_exponent) - Single::ExponentBias;

        // if (0 <= -q && -q < Single::SignificandSize && MultipleOfPow2(c, -q))
        // {
        //     return {c >> -q, 0};
        // }
    }
    else
    {
        c = ieee_significand;
        q = 1 - Single::ExponentBias;
    }

    const bool is_even = (c % 2 == 0);
    const bool accept_lower = is_even;
    const bool accept_upper = is_even;

    const bool lower_boundary_is_closer = (ieee_significand == 0 && ieee_exponent > 1);

    //  const int32_t qb = q - 2;
    const uint32_t cbl = 4 * c - 2 + lower_boundary_is_closer;
    const uint32_t cb = 4 * c;
    const uint32_t cbr = 4 * c + 2;

    // (q * 1262611         ) >> 22 == floor(log_10(    2^q))
    // (q * 1262611 - 524031) >> 22 == floor(log_10(3/4 2^q))
    //SF_ASSERT(q >= -1500);
    //SF_ASSERT(q <= 1500);
    const int32_t k = FloorDivPow2(q * 1262611 - (lower_boundary_is_closer ? 524031 : 0), 22);

    const int32_t h = q + FloorLog2Pow10(-k) + 1;
    //SF_ASSERT(h >= 1);
    //SF_ASSERT(h <= 4);

    const uint64_t pow10 = ComputePow10_Single(-k);
    const uint32_t vbl = RoundToOdd(pow10, cbl << h);
    const uint32_t vb = RoundToOdd(pow10, cb << h);
    const uint32_t vbr = RoundToOdd(pow10, cbr << h);

    const uint32_t lower = vbl + !accept_lower;
    const uint32_t upper = vbr - !accept_upper;

    // See Figure 4 in [1].
    // And the modifications in Figure 6.

    const uint32_t s = vb / 4; // NB: 4 * s == vb & ~3 == vb & -4

    if (s >= 10) // vb >= 40
    {
        const uint32_t sp = s / 10; // = vb / 40
        const bool up_inside = lower <= 40 * sp;
        const bool wp_inside = 40 * sp + 40 <= upper;
        //      if (up_inside || wp_inside) // NB: At most one of u' and w' is in R_v.
        if (up_inside != wp_inside)
        {
            // return {sp + wp_inside, k + 1};
            return {(sp + wp_inside) * 10, k};
        }
    }

    const bool u_inside = lower <= 4 * s;
    const bool w_inside = 4 * s + 4 <= upper;
    if (u_inside != w_inside)
    {
        return {s + w_inside, k};
    }

    // NB: s & 1 == vb & 0x4
    const uint32_t mid = 4 * s + 2; // = 2(s + t)
    const bool round_up = vb > mid || (vb == mid && (s & 1) != 0);

    return {s + round_up, k};
}
#if 0
static inline FloatingDecimal32 ToDecimal32_xjb(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    uint32_t c;
    int32_t q;
    if (ieee_exponent != 0) [[likely]]
    {
        c = Single::HiddenBit | ieee_significand;
        q = static_cast<int32_t>(ieee_exponent) - Single::ExponentBias;
    }
    else
    {
        c = ieee_significand;
        q = 1 - Single::ExponentBias;
    }
    const bool lower_boundary_is_closer = (ieee_significand == 0);
    const uint32_t cbl = 4 * c - 2 + lower_boundary_is_closer;
    const uint32_t cb = 4 * c;
    const uint32_t cbr = 4 * c + 2;
    int32_t k;
    if (lower_boundary_is_closer) [[unlikely]]
        k = (q * 1262611 - 524031) >> 22;
    else
        k = (q * 1262611) >> 22;
    const int32_t h = q + FloorLog2Pow10(-k) + 1;
    const uint64_t pow10 = ComputePow10_Single(-k);
    const uint32_t vbl = RoundToOdd(pow10, cbl << h); // y1 | (y0 > 1);  if(y0 > 1) y1 += 1; or if(y0 > 0) y1++;
    const uint32_t vb = RoundToOdd(pow10, cb << h);
    const uint32_t vbr = RoundToOdd(pow10, cbr << h);
    const uint32_t lower = vbl + (c & 1);
    const uint32_t upper = vbr - (c & 1);
    const uint32_t s = vb / 4; // NB: 4 * s == vb & ~3 == vb & -4
    uint32_t sp10 = ((s * (uint64_t)1717986919) >> 34) * 10;
    uint32_t d = s + (((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    if (lower_boundary_is_closer) [[unlikely]]
    {
        d += ieee_exponent == 31 | ieee_exponent == 214 | ieee_exponent == 217;
    }
    // uint32_t d = s + (((vb & -4) < (lower < (upper - 3) ? lower : (upper - 3))) | ((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    if (lower <= sp10 * 4) // vbl + (c & 1) <= (s/10) * 10 * 4
        d = sp10;          // s/10*10
    if ((sp10 * 4 + 40 - 3) <= (upper - 3))
        d = sp10 + 10; // s/10*10+10
    return {d, k};
}
static inline FloatingDecimal32 ToDecimal32_xjb_m(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    uint32_t c;
    int32_t q;
    if (ieee_exponent != 0) [[likely]]
    {
        c = Single::HiddenBit | ieee_significand;
        q = static_cast<int32_t>(ieee_exponent) - Single::ExponentBias;
    }
    else
    {
        c = ieee_significand;
        q = 1 - Single::ExponentBias;
    }
    const bool lower_boundary_is_closer = (ieee_significand == 0);
    const uint32_t cbl = 4 * c - 2 + lower_boundary_is_closer;
    const uint32_t cb = 4 * c;
    const uint32_t cbr = 4 * c + 2;
    int32_t k;
    if (lower_boundary_is_closer) [[unlikely]]
        k = (q * 1262611 - 524031) >> 22;
    else
        k = (q * 1262611) >> 22;
    const int32_t h = q + FloorLog2Pow10(-k) + 1;
    const uint64_t pow10 = ComputePow10_Single(-k);
    const uint32_t vbl = RoundToOdd(pow10, cbl << h); // y1 | (y0 > 1);  if(y0 > 1) y1 += 1; or if(y0 > 0) y1++;
    const uint32_t vb = RoundToOdd(pow10, cb << h);
    const uint32_t vbr = RoundToOdd(pow10, cbr << h);
    const uint32_t lower = vbl + (c & 1);
    const uint32_t upper = vbr - (c & 1);
    const uint32_t s = vb / 4; // NB: 4 * s == vb & ~3 == vb & -4
    uint32_t sp10 = ((s * (uint64_t)1717986919) >> 34) * 10;
    // uint32_t d = s + (((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    // if (lower_boundary_is_closer) [[unlikely]]
    // {
    //     d += ieee_exponent == 31 | ieee_exponent == 214 | ieee_exponent == 217;
    // }
    // // uint32_t d = s + (((vb & -4) < (lower < (upper - 3) ? lower : (upper - 3))) | ((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    // if (lower <= sp10 * 4) // vbl + (c & 1) <= (s/10) * 10 * 4
    //     d = sp10;          // s/10*10
    // if ((sp10 * 4 + 40 - 3) <= (upper - 3))
    //     d = sp10 + 10; // s/10*10+10
    return {sp10, k};
}
static inline FloatingDecimal32 ToDecimal32_xjb_down(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    uint32_t c;
    int32_t q;
    if (ieee_exponent != 0) [[likely]]
    {
        c = Single::HiddenBit | ieee_significand;
        q = static_cast<int32_t>(ieee_exponent) - Single::ExponentBias;
    }
    else
    {
        c = ieee_significand;
        q = 1 - Single::ExponentBias;
    }
    const bool lower_boundary_is_closer = (ieee_significand == 0);
    const uint32_t cbl = 4 * c - 2 + lower_boundary_is_closer;
    const uint32_t cb = 4 * c;
    const uint32_t cbr = 4 * c + 2;
    int32_t k;
    if (lower_boundary_is_closer) [[unlikely]]
        k = (q * 1262611 - 524031) >> 22;
    else
        k = (q * 1262611) >> 22;
    // const int32_t k = FloorDivPow2(q * 1262611 - (lower_boundary_is_closer ? 524031 : 0), 22);
    const int32_t h = q + FloorLog2Pow10(-k) + 1;
    const uint64_t pow10 = ComputePow10_Single_down(-k);
    const uint32_t vbl = RoundToOdd(pow10, cbl << h); // y1 | (y0 > 1);  if(y0 > 1) y1 += 1; or if(y0 > 0) y1++;
    const uint32_t vb = RoundToOdd(pow10, cb << h);
    const uint32_t vbr = RoundToOdd(pow10, cbr << h);
    const uint32_t lower = vbl + (c & 1);
    const uint32_t upper = vbr - (c & 1);
    const uint32_t s = vb / 4; // NB: 4 * s == vb & ~3 == vb & -4
    uint32_t sp10 = ((s * (uint64_t)1717986919) >> 34) * 10;
    uint32_t d = s + (((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    if (lower_boundary_is_closer) [[unlikely]]
    {
        d += ieee_exponent == 31 | ieee_exponent == 214 | ieee_exponent == 217;
    }
    // uint32_t d = s + (((vb & -4) < (lower < (upper - 3) ? lower : (upper - 3))) | ((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    if (lower <= sp10 * 4) // vbl + (c & 1) <= (s/10) * 10 * 4
        d = sp10;          // s/10*10
    if ((sp10 * 4 + 40 - 3) <= (upper - 3))
        d = sp10 + 10; // s/10*10+10
    return {d, k};
}

struct f96
{
    uint32_t a;
    uint64_t b;
};
struct f104
{
    uint64_t a; // 40bit
    uint64_t b; // 64bit
};

static inline FloatingDecimal32 ToDecimal32_xjb_v2(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef __uint128_t u128;
    typedef uint64_t u64;
    typedef uint32_t u32;

    u64 g100[(45 - (-32) + 1) * 2] = {
        0xcfb11ead4, 0x53994ba67de18eda, // -32
        0x81ceb32c4, 0xb43fcf480eacf948, // -31
        0xa2425ff75, 0xe14fc31a1258379a, // -30
        0xcad2f7f53, 0x59a3b3e096ee4581, // -29
        0xfd87b5f28, 0x300ca0d8bca9d6e1, // -28
        0x9e74d1b79, 0x1e07e48775ea264c, // -27
        0xc61206257, 0x6589dda95364afe0, // -26
        0xf79687aed, 0x3eec5513a83ddbd8, // -25
        0x9abe14cd4, 0x4753b52c4926a967, // -24
        0xc16d9a009, 0x5928a2775b7053c0, // -23
        0xf1c90080b, 0xaf72cb15324c68b1, // -22
        0x971da0507, 0x4da7beed3f6fc16e, // -21
        0xbce508649, 0x2111aea88f4bb1ca, // -20
        0xec1e4a7db, 0x69561a52b31e9e3d, // -19
        0x9392ee8e9, 0x21d5d073aff322e6, // -18
        0xb877aa323, 0x6a4b44909befeb9f, // -17
        0xe69594bec, 0x44de15b4c2ebe687, // -16
        0x901d7cf73, 0xab0acd90f9d37014, // -15
        0xb424dc350, 0x95cd80f538484c19, // -14
        0xe12e13424, 0xbb40e132865a5f20, // -13
        0x8cbccc096, 0xf5088cbf93f87b74, // -12
        0xafebff0bc, 0xb24aafef78f69a51, // -11
        0xdbe6feceb, 0xdedd5beb573440e5, // -10
        0x89705f413, 0x6b4a59731680a88f, // -9
        0xabcc77118, 0x461cefcfdc20d2b3, // -8
        0xd6bf94d5e, 0x57a42bc3d3290760, // -7
        0x8637bd05a, 0xf6c69b5a63f9a49c, // -6
        0xa7c5ac471, 0xb4784230fcf80dc3, // -5
        0xd1b71758e, 0x219652bd3c361134, // -4
        0x83126e978, 0xd4fdf3b645a1cac0, // -3
        0xa3d70a3d7, 0xa3d70a3d70a3d70,  // -2
        0xccccccccc, 0xcccccccccccccccc, // -1
        0x800000000, 0x0,                // 0
        0xa00000000, 0x0,                // 1
        0xc80000000, 0x0,                // 2
        0xfa0000000, 0x0,                // 3
        0x9c4000000, 0x0,                // 4
        0xc35000000, 0x0,                // 5
        0xf42400000, 0x0,                // 6
        0x989680000, 0x0,                // 7
        0xbebc20000, 0x0,                // 8
        0xee6b28000, 0x0,                // 9
        0x9502f9000, 0x0,                // 10
        0xba43b7400, 0x0,                // 11
        0xe8d4a5100, 0x0,                // 12
        0x9184e72a0, 0x0,                // 13
        0xb5e620f48, 0x0,                // 14
        0xe35fa931a, 0x0,                // 15
        0x8e1bc9bf0, 0x4000000000000000, // 16
        0xb1a2bc2ec, 0x5000000000000000, // 17
        0xde0b6b3a7, 0x6400000000000000, // 18
        0x8ac723048, 0x9e80000000000000, // 19
        0xad78ebc5a, 0xc620000000000000, // 20
        0xd8d726b71, 0x77a8000000000000, // 21
        0x878678326, 0xeac9000000000000, // 22
        0xa968163f0, 0xa57b400000000000, // 23
        0xd3c21bcec, 0xceda100000000000, // 24
        0x845951614, 0x1484a0000000000,  // 25
        0xa56fa5b99, 0x19a5c8000000000,  // 26
        0xcecb8f27f, 0x4200f3a000000000, // 27
        0x813f3978f, 0x8940984400000000, // 28
        0xa18f07d73, 0x6b90be5500000000, // 29
        0xc9f2c9cd0, 0x4674edea40000000, // 30
        0xfc6f7c404, 0x58122964d0000000, // 31
        0x9dc5ada82, 0xb70b59df02000000, // 32
        0xc53719123, 0x64ce3056c2800000, // 33
        0xf684df56c, 0x3e01bc6c73200000, // 34
        0x9a130b963, 0xa6c115c3c7f40000, // 35
        0xc097ce7bc, 0x90715b34b9f10000, // 36
        0xf0bdc21ab, 0xb48db201e86d4000, // 37
        0x96769950b, 0x50d88f4131444800, // 38
        0xbc143fa4e, 0x250eb3117d955a00, // 39
        0xeb194f8e1, 0xae525fd5dcfab080, // 40
        0x92efd1b8d, 0xcf37be5aa1cae50,  // 41
        0xb7abc6270, 0x50305adf14a3d9e4, // 42
        0xe596b7b0c, 0x643c7196d9ccd05d, // 43
        0x8f7e32ce7, 0xbea5c6fe4820023a, // 44
        0xb35dbf821, 0xae4f38bdda2802c8, // 45
    };
    u32 d;
    int k;
    u32 odd = ieee_significand & 1;
    int offset = 3; // range [3,10] all right
    const u64 maskL = (1ull << offset) - 1;
    int exp_bin;
    u64 sig_bin;
    u64 irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if ((!irregular)) [[likely]]
        k = (exp_bin * 315653) >> 20; // branch
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-k - 1) * 217707) >> 16) + 1 + offset; //[1,4]

    // int hmax=-100,hmin=100;
    // hmax = h1>hmax?h1:hmax;
    // hmin = h1<hmin?h1:hmin;

    // printf("h1=%d,hmax=%d,hmin=%d\n",h1,hmax,hmin);
    // const u64 *pow10_ptr = &g[32];
    // u64 pow10 = pow10_ptr[(-k - 1)];

    // const u64 *pow10_ptr2 = &g2[32];
    // u64 pow102 = pow10_ptr2[(-k - 1)];

    const u64 *pow10_ptr100 = &g100[32 * 2];
    u64 pow10_100_hi = pow10_ptr100[(-k - 1) * 2];
    u64 pow10_100_lo = pow10_ptr100[(-k - 1) * 2 + 1];

    u64 cb = sig_bin << (h1); // h1 max is 4
    // u64 sig_hi = (cb * (u128)pow10) >> 64; // one mulxq instruction
    // u64 sig_lo = (cb * (u128)pow10);

    // u64 sig_hi2 = (cb * (u128)pow102) >> 64; // one mulxq instruction
    // u64 sig_lo2 = (cb * (u128)pow102);

    // sig_hi2 += cb ;
    // sig_hi = sig_hi2;
    // sig_lo = sig_lo2;

    u64 sig_hi_100 = (cb * (u128)pow10_100_lo) >> 64; // one mulxq instruction
    u64 sig_lo_100 = (cb * (u128)pow10_100_lo);
    sig_hi_100 += cb * pow10_100_hi; // cb max: 28bit ; pow10_104_hi: 36bit ; not overflow;

    u64 sig_hi = sig_hi_100;
    u64 sig_lo = sig_lo_100;

    offset += 36; // offset=39
    u32 ten = (sig_hi >> offset) * 10;

    // u64 half_ulp = pow10 >> (1 + offset - h1); // 1+offset-h1 -> range [1,4] + 36 = [37,40]
    u64 dot_one = (sig_hi << (64 - offset)) + (sig_lo >> offset); // (sig_hi << 61) + (sig_lo >> 3)
    u64 half_ulp = ((pow10_100_hi << (64 - 36)) + (pow10_100_lo >> 36)) >> (1 + offset - 36 - h1);

    // printf("(1 + offset - h1) = %d\n",(1 + offset - h1));
    // u64 half_ulp2 = pow10;
    // u64 dot_one2 = (sig_hi << (65 - h1)) + (sig_lo >> (h1 - 1));//not correct

    u32 one;
    {
        // round to nearest
        // u64 rest = ((sig_lo & maskL) * 10) >> offset;
        one = (((dot_one) * (u128)10) >> 64) + (((u64)((dot_one) * (u128)10)) >> 63);
        // round to even
        if (((u64)((dot_one) * (u128)10) == (1ull << 63))) [[unlikely]] // generate branch instruction
            one -= ((!((((dot_one) * (u128)10) >> 64) & 1)));           // if(even) one--;
                                                                        // adjust to correct result
        if ((irregular)) [[unlikely]]
        {
            one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217);
        }
    }
    // printf("one = %u\n",one);
    //  round up
    if ((half_ulp == ((~0) - dot_one))) [[unlikely]] // generate branch instruction
    // if ((half_ulp + dot_one)==0)[[unlikely]]
    {
        if (!odd) // even
        {
            // printf("one = 10 even\n");
            one = 10;
        }
    }
    if (half_ulp > (~0 - dot_one)) // generate cmov instruction
    // if (half_ulp + dot_one < dot_one)
    {
        // printf("this line \n half_ulp = %llu,dot_one=%llu,(~0 - dot_one)=%llu,half_ulp + dot_one=%llu,\n",
        //     half_ulp,dot_one,(~0 - dot_one),half_ulp + dot_one);
        // printf("one = 10 round\n");
        one = 10;
    }
    // round down
    if ((!irregular)) [[likely]]
    {
        if (((half_ulp == dot_one))) [[unlikely]] // generate branch instruction
        {
            if (!odd)
                one = 0;
        }
        if (half_ulp > dot_one) // generate cmov instruction
            one = 0;
    }
    else
    {
        if (half_ulp / 2 >= dot_one) // generate cmov instruction
            one = 0;
    }

    u32 dec = ten + one;
    // printf("ten = %u, one = %u\n",ten,one);
    // printf("half_ulp = %llu,dot_one = %llu\n",half_ulp,dot_one);
    // printf("half_ulp + dot_one = %llu,%llx\n",(u64)(half_ulp + dot_one),(u64)(half_ulp + dot_one));
    return {dec, k};

    // // if (unlikely(irregular))
#if 0
    if (0)
    {
        // only exp_bin range [1,2046] , all 2046 double , generate correct result
        int exp_bin;
        u64 sig_bin;
        u64 irregular = (ieee_significand == 0);
        if (ieee_exponent != 0) [[likely]]
        {
            exp_bin = ieee_exponent - 150;
            sig_bin = ieee_significand | (1 << 23);
        }
        else
        {
            exp_bin = 1 - 150;
            sig_bin = ieee_significand;
        }
        int k = (exp_bin * 315653 - (irregular ? 131237 : 0)) >> 20;
        int h = exp_bin + ((k * -217707) >> 16) + 1;
        const u64 *pow10_ptr = g - (-31);
        u64 *sig = (u64 *)&pow10_ptr[(-k)];
        u64 p10_hi = sig[0];
        u64 cb = sig_bin << (h);
        u64 sig_hi = (cb * (u128)p10_hi) >> 64;
        u64 sig_lo = (cb * (u128)p10_hi);
        u64 ten = (((u128)sig_hi * 1844674407370955162) >> 64) * 10; // sig_hi / 10 * 10
        u64 one = sig_hi - ten;
        u64 c = (one << (64 - 4)) | (sig_lo >> 4);
        u64 half_ulp = p10_hi >> (5 - h);
        const u64 t0 = ((u64)10 << (64 - 4)); // const
        u64 t1 = c + half_ulp;

        u64 round_u1, round_u0, round_d0, round_d1;

        if ((!irregular)) // high branch
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            if ((sig_lo == ((u64)1 << 63))) // low branch
            {
                round_u1 = (sig_hi & 1);
            }
            round_d0 = (half_ulp > c);
            if ((half_ulp == c)) // low branch
            {
                round_d0 = !(sig_bin & 1);
            }
        }
        else // low branch
        {
            round_u1 = (sig_lo > ((u64)1 << 63));
            round_d1 = (half_ulp / 2 >= (sig_lo >> 4));
            if (!round_d1)
                round_u1 = true;
            // if ((half_ulp / 2 < (sig_lo >> 4))) round_u1 = true;
            round_d0 = (half_ulp / 2 >= c);
        }
        round_u0 = (t1 > t0);
        if ((t0 - t1 <= (u64)1)) // low branch
        {
            if (((k == 0) | (t0 - 1 == t1)))
                round_u0 = !(sig_bin & 1);
            // if ((t0 - t1) <= *(u64 *)&k)round_u0 = !(sig_bin & 1);
        }

        // u64 round_u1 = (sig_lo > ((u64)1 << 63));
        // if ((half_ulp < (sig_lo / 8))) round_u1 = true;
        // u64 round_d0 = (half_ulp / 2 >= c);
        // u64 round_u0 = (t1 > t0);

        u64 d = sig_hi + round_u1;
        if (round_d0)
            d = ten;
        if (round_u0)
            d = ten + 10;
        return {(u32)d, k};
    }
#endif
    // //*exp_dec = (exp_bin * 315653) >> 20; // exp_bin : [-1074,972]    k:[-324,292]
    // int h1 = exp_bin + (((-(*exp_dec) - 1) * 217707) >> 16) + 1 + offset; // [-3,0] + offset
    // // get 10^(-(*exp_dec) - 1)  from  pow10 u128 lut
    // const u64 *pow10_ptr = pow10_sig_table - POW10_SIG_TABLE_MIN_EXP * 2;
    // u64 *sig = (u64 *)&pow10_ptr[(-(*exp_dec) - 1) * 2];
    // u64 p10_hi = sig[0];
    // u64 p10_lo = sig[1];
    // // u128 * u64 = u192 ----> get high 128 bit
    // u64 cb = sig_bin << (h1); // h1 = [0,3]
    // u64 sig_hi = (cb * (u128)p10_lo) >> 64;
    // u64 sig_lo = (cb * (u128)p10_hi + sig_hi);
    // sig_hi = (cb * (u128)p10_hi + sig_hi) >> 64;
    // //select  from  ten , ten + one , ten + one + 1 , ten + 10
    // u64 ten = (sig_hi >> offset) * 10;

    // uint32_t c;
    // int32_t q;
    // if (ieee_exponent != 0)
    // {
    //     c = Single::HiddenBit | ieee_significand;
    //     q = static_cast<int32_t>(ieee_exponent) - Single::ExponentBias;
    // }
    // else
    // {
    //     c = ieee_significand;
    //     q = 1 - Single::ExponentBias;
    // }
    // const bool lower_boundary_is_closer = (ieee_significand == 0);
    // const uint32_t cbl = 4 * c - 2 + lower_boundary_is_closer;
    // const uint32_t cb = 4 * c;
    // const uint32_t cbr = 4 * c + 2;
    // const int32_t k = FloorDivPow2(q * 1262611 - (lower_boundary_is_closer ? 524031 : 0), 22);
    // const int32_t h = q + FloorLog2Pow10(-k-1) + 1;
    // const uint64_t pow10 = ComputePow10_Single(-k-1);

    // (u128)pow10 * c;

    // const uint32_t vbl = RoundToOdd(pow10, cbl << h);
    // const uint32_t vb = RoundToOdd(pow10, cb << h);
    // const uint32_t vbr = RoundToOdd(pow10, cbr << h);
    // const uint32_t lower = vbl + (c & 1);
    // const uint32_t upper = vbr - (c & 1);
    // const uint32_t s = vb / 4; // NB: 4 * s == vb & ~3 == vb & -4
    // uint32_t sp10 = ((s * (uint64_t)1717986919) >> 34) * 10;
    // uint32_t d = s + (((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    // if (__builtin_expect(lower_boundary_is_closer, 0))//unlikely
    // {// only 3 float num incorrect
    //     d += ieee_exponent == 31 | ieee_exponent == 214 | ieee_exponent == 217;
    // }
    // // uint32_t d = s + (((vb & -4) < (lower < (upper - 3) ? lower : (upper - 3))) | ((0b11001000 >> (vb & 7)) & 1)); // s or s+1
    // if (lower <= sp10 * 4)
    //     d = sp10; // s/10*10
    // if ((sp10 * 4 + 40 - 3) <= (upper - 3))
    //     d = sp10 + 10; // s/10*10+10
    // return {d, k};
}
static inline FloatingDecimal32 ToDecimal32_xjb_v3(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef __uint128_t u128;
    typedef uint64_t u64;
    typedef uint32_t u32;
    u64 g100[(45 - (-32) + 1) * 2] = {
        0xcfb11ead4, 0x53994ba67de18eda, // -32
        0x81ceb32c4, 0xb43fcf480eacf948, // -31
        0xa2425ff75, 0xe14fc31a1258379a, // -30
        0xcad2f7f53, 0x59a3b3e096ee4581, // -29
        0xfd87b5f28, 0x300ca0d8bca9d6e1, // -28
        0x9e74d1b79, 0x1e07e48775ea264c, // -27
        0xc61206257, 0x6589dda95364afe0, // -26
        0xf79687aed, 0x3eec5513a83ddbd8, // -25
        0x9abe14cd4, 0x4753b52c4926a967, // -24
        0xc16d9a009, 0x5928a2775b7053c0, // -23
        0xf1c90080b, 0xaf72cb15324c68b1, // -22
        0x971da0507, 0x4da7beed3f6fc16e, // -21
        0xbce508649, 0x2111aea88f4bb1ca, // -20
        0xec1e4a7db, 0x69561a52b31e9e3d, // -19
        0x9392ee8e9, 0x21d5d073aff322e6, // -18
        0xb877aa323, 0x6a4b44909befeb9f, // -17
        0xe69594bec, 0x44de15b4c2ebe687, // -16
        0x901d7cf73, 0xab0acd90f9d37014, // -15
        0xb424dc350, 0x95cd80f538484c19, // -14
        0xe12e13424, 0xbb40e132865a5f20, // -13
        0x8cbccc096, 0xf5088cbf93f87b74, // -12
        0xafebff0bc, 0xb24aafef78f69a51, // -11
        0xdbe6feceb, 0xdedd5beb573440e5, // -10
        0x89705f413, 0x6b4a59731680a88f, // -9
        0xabcc77118, 0x461cefcfdc20d2b3, // -8
        0xd6bf94d5e, 0x57a42bc3d3290760, // -7
        0x8637bd05a, 0xf6c69b5a63f9a49c, // -6
        0xa7c5ac471, 0xb4784230fcf80dc3, // -5
        0xd1b71758e, 0x219652bd3c361134, // -4
        0x83126e978, 0xd4fdf3b645a1cac0, // -3
        0xa3d70a3d7, 0xa3d70a3d70a3d70,  // -2
        0xccccccccc, 0xcccccccccccccccc, // -1
        0x800000000, 0x0,                // 0
        0xa00000000, 0x0,                // 1
        0xc80000000, 0x0,                // 2
        0xfa0000000, 0x0,                // 3
        0x9c4000000, 0x0,                // 4
        0xc35000000, 0x0,                // 5
        0xf42400000, 0x0,                // 6
        0x989680000, 0x0,                // 7
        0xbebc20000, 0x0,                // 8
        0xee6b28000, 0x0,                // 9
        0x9502f9000, 0x0,                // 10
        0xba43b7400, 0x0,                // 11
        0xe8d4a5100, 0x0,                // 12
        0x9184e72a0, 0x0,                // 13
        0xb5e620f48, 0x0,                // 14
        0xe35fa931a, 0x0,                // 15
        0x8e1bc9bf0, 0x4000000000000000, // 16
        0xb1a2bc2ec, 0x5000000000000000, // 17
        0xde0b6b3a7, 0x6400000000000000, // 18
        0x8ac723048, 0x9e80000000000000, // 19
        0xad78ebc5a, 0xc620000000000000, // 20
        0xd8d726b71, 0x77a8000000000000, // 21
        0x878678326, 0xeac9000000000000, // 22
        0xa968163f0, 0xa57b400000000000, // 23
        0xd3c21bcec, 0xceda100000000000, // 24
        0x845951614, 0x1484a0000000000,  // 25
        0xa56fa5b99, 0x19a5c8000000000,  // 26
        0xcecb8f27f, 0x4200f3a000000000, // 27
        0x813f3978f, 0x8940984400000000, // 28
        0xa18f07d73, 0x6b90be5500000000, // 29
        0xc9f2c9cd0, 0x4674edea40000000, // 30
        0xfc6f7c404, 0x58122964d0000000, // 31
        0x9dc5ada82, 0xb70b59df02000000, // 32
        0xc53719123, 0x64ce3056c2800000, // 33
        0xf684df56c, 0x3e01bc6c73200000, // 34
        0x9a130b963, 0xa6c115c3c7f40000, // 35
        0xc097ce7bc, 0x90715b34b9f10000, // 36
        0xf0bdc21ab, 0xb48db201e86d4000, // 37
        0x96769950b, 0x50d88f4131444800, // 38
        0xbc143fa4e, 0x250eb3117d955a00, // 39
        0xeb194f8e1, 0xae525fd5dcfab080, // 40
        0x92efd1b8d, 0xcf37be5aa1cae50,  // 41
        0xb7abc6270, 0x50305adf14a3d9e4, // 42
        0xe596b7b0c, 0x643c7196d9ccd05d, // 43
        0x8f7e32ce7, 0xbea5c6fe4820023a, // 44
        0xb35dbf821, 0xae4f38bdda2802c8, // 45
    };
    int k;
    u32 odd = ieee_significand & 1;
    const int offset = 3;
    int exp_bin;
    u64 sig_bin;
    u64 irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]] // generate branch instruction
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]]
        k = (exp_bin * 315653) >> 20; // generate branch instruction
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-k - 1) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr100 = &g100[32 * 2];
    u64 pow10_100_hi = pow10_ptr100[(-k - 1) * 2];
    u64 pow10_100_lo = pow10_ptr100[(-k - 1) * 2 + 1];
    u64 cb = sig_bin << (h1);                         // h1 max is 4
    u64 sig_hi_100 = (cb * (u128)pow10_100_lo) >> 64; // one mulxq instruction on x86
    u64 sig_lo_100 = (cb * (u128)pow10_100_lo);
    sig_hi_100 += cb * pow10_100_hi; // cb max: 28bit ; pow10_100_hi: 36bit ; not overflow;    one mulq , add instruction
    const int offset2 = offset + 36; // offset2 = 39
    u32 ten = (sig_hi_100 >> offset2) * 10;
    u64 dot_one = (sig_hi_100 << (64 - offset2)) + (sig_lo_100 >> offset2);
    u64 half_ulp = ((pow10_100_hi << 28) + (pow10_100_lo >> 36)) >> (1 + offset - h1);
    u32 one;
    {
        // round to nearest  ; one mulxq instrction
        one = ((dot_one * (u128)10) >> 64) + (((u64)(dot_one * (u128)10)) >> 63);
        // round to even
        if ((u64)(dot_one * (u128)10) == (1ull << 63)) [[unlikely]] // generate branch instruction
            one -= !(((dot_one * (u128)10) >> 64) & 1);             // if(even) one--;
        // adjust to correct result
        if (irregular) [[unlikely]] // generate branch instruction
        {
            one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217);
        }
    }
    // round up
    if ((half_ulp == ((~0) - dot_one))) [[unlikely]] // generate branch instruction
    {
        one = odd ? one : 10; // generate cmov instruction
    }
    if (half_ulp > (~0 - dot_one)) // generate cmov instruction
    {
        one = 10;
    }
    // round down
    if (!irregular) [[likely]]
    {
        if (((half_ulp == dot_one))) [[unlikely]] // generate branch instruction
        {
            one = odd ? one : 0; // generate cmov instruction
        }
        if (half_ulp > dot_one) // generate cmov instruction
        {
            one = 0;
        }
    }
    else
    {
        if (half_ulp / 2 >= dot_one) // generate cmov instruction
        {
            one = 0;
        }
    }
    u32 dec = ten + one; // four possible outcomes ;
    return {dec, k};     // == dec * (10 ^ k) ;
}
static inline FloatingDecimal32 ToDecimal32_xjb_v4(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef __uint128_t u128;
    typedef uint64_t u64;
    typedef uint32_t u32;
    const int BIT = 91; // need >= 91  ; 91 - 64 = 25; 25 > 24;
    const u64 g[(45 - (-32) + 1) * 2] = {
        0x67d88f5, 0x6a29cca5d33ef0c7, // -32
        0x40e7599, 0x625a1fe7a407567c, // -31
        0x51212ff, 0xbaf0a7e18d092c1b, // -30
        0x65697bf, 0xa9acd1d9f04b7722, // -29
        0x7ec3daf, 0x941806506c5e54eb, // -28
        0x4f3a68d, 0xbc8f03f243baf513, // -27
        0x6309031, 0x2bb2c4eed4a9b257, // -26
        0x7bcb43d, 0x769f762a89d41eed, // -25
        0x4d5f0a6, 0x6a23a9da96249354, // -24
        0x60b6cd0, 0x4ac94513badb829,  // -23
        0x78e4804, 0x5d7b9658a992634,  // -22
        0x4b8ed02, 0x83a6d3df769fb7e0, // -21
        0x5e72843, 0x249088d75447a5d8, // -20
        0x760f253, 0xedb4ab0d29598f4f, // -19
        0x49c9774, 0x7490eae839d7f991, // -18
        0x5c3bd51, 0x91b525a2484df7f5, // -17
        0x734aca5, 0xf6226f0ada6175f3, // -16
        0x480ebe7, 0xb9d58566c87ce9b8, // -15
        0x5a126e1, 0xa84ae6c07a9c2426, // -14
        0x709709a, 0x125da07099432d2f, // -13
        0x465e660, 0x4b7a84465fc9fc3d, // -12
        0x57f5ff8, 0x5e592557f7bc7b4d, // -11
        0x6df37f6, 0x75ef6eadf5ab9a20, // -10
        0x44b82fa, 0x9b5a52cb98b4054,  // -9
        0x55e63b8, 0x8c230e77e7ee1069, // -8
        0x6b5fca6, 0xaf2bd215e1e99483, // -7
        0x431bde8, 0x2d7b634dad31fcd2, // -6
        0x53e2d62, 0x38da3c21187e7c06, // -5
        0x68db8ba, 0xc710cb295e9e1b08, // -4
        0x4189374, 0xbc6a7ef9db22d0e5, // -3
        0x51eb851, 0xeb851eb851eb851e, // -2
        0x6666666, 0x6666666666666666, // -1
        0x4000000, 0x0,                // 0
        0x5000000, 0x0,                // 1
        0x6400000, 0x0,                // 2
        0x7d00000, 0x0,                // 3
        0x4e20000, 0x0,                // 4
        0x61a8000, 0x0,                // 5
        0x7a12000, 0x0,                // 6
        0x4c4b400, 0x0,                // 7
        0x5f5e100, 0x0,                // 8
        0x7735940, 0x0,                // 9
        0x4a817c8, 0x0,                // 10
        0x5d21dba, 0x0,                // 11
        0x746a528, 0x8000000000000000, // 12
        0x48c2739, 0x5000000000000000, // 13
        0x5af3107, 0xa400000000000000, // 14
        0x71afd49, 0x8d00000000000000, // 15
        0x470de4d, 0xf820000000000000, // 16
        0x58d15e1, 0x7628000000000000, // 17
        0x6f05b59, 0xd3b2000000000000, // 18
        0x4563918, 0x244f400000000000, // 19
        0x56bc75e, 0x2d63100000000000, // 20
        0x6c6b935, 0xb8bbd40000000000, // 21
        0x43c33c1, 0x9375648000000000, // 22
        0x54b40b1, 0xf852bda000000000, // 23
        0x69e10de, 0x76676d0800000000, // 24
        0x422ca8b, 0xa00a42500000000,  // 25
        0x52b7d2d, 0xcc80cd2e40000000, // 26
        0x6765c79, 0x3fa10079d0000000, // 27
        0x409f9cb, 0xc7c4a04c22000000, // 28
        0x50c783e, 0xb9b5c85f2a800000, // 29
        0x64f964e, 0x68233a76f5200000, // 30
        0x7e37be2, 0x22c0914b2680000,  // 31
        0x4ee2d6d, 0x415b85acef810000, // 32
        0x629b8c8, 0x91b267182b614000, // 33
        0x7b426fa, 0xb61f00de36399000, // 34
        0x4d0985c, 0xb1d3608ae1e3fa00, // 35
        0x604be73, 0xde4838ad9a5cf880, // 36
        0x785ee10, 0xd5da46d900f436a0, // 37
        0x4b3b4ca, 0x85a86c47a098a224, // 38
        0x5e0a1fd, 0x2712875988becaad, // 39
        0x758ca7c, 0x70d7292feaee7d58, // 40
        0x4977e8d, 0xc68679bdf2d50e57, // 41
        0x5bd5e31, 0x3828182d6f8a51ec, // 42
        0x72cb5bd, 0x86321e38cb6ce668, // 43
        0x47bf196, 0x73df52e37f241001, // 44
        0x59aedfc, 0x10d7279c5eed1401, // 45
    };

    int k;
    u32 odd = ieee_significand & 1;
    const int offset = 3;
    int exp_bin;
    u64 sig_bin;
    u64 irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]] // generate branch instruction
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]]
        k = (exp_bin * 315653) >> 20; // generate branch instruction
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-k - 1) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr100 = &g[32 * 2];
    u64 pow10_100_hi = pow10_ptr100[(-k - 1) * 2];
    u64 pow10_100_lo = pow10_ptr100[(-k - 1) * 2 + 1];
    u64 cb = sig_bin << (h1); // h1 max is 4
    // calc u91 mul u28; max is u119; 91 + 28 == 119;
    u64 sig_hi_100 = (cb * (u128)pow10_100_lo) >> 64; // one mulxq instruction on x86
    u64 sig_lo_100 = (cb * (u128)pow10_100_lo);
    sig_hi_100 += cb * pow10_100_hi;         // cb max: 28bit ; pow10_100_hi: 36bit ; not overflow;
    const int offset2 = offset + (BIT - 64); // offset2 = 39
    u32 ten = (sig_hi_100 >> offset2) * 10;
    u64 dot_one = (sig_hi_100 << (64 - offset2)) + (sig_lo_100 >> offset2);
    u64 half_ulp = ((pow10_100_hi << (64 - (BIT - 64))) + (pow10_100_lo >> (BIT - 64))) >> (1 + offset - h1);
    // round to nearest  ; one mulxq instrction
    u32 one = ((dot_one * (u128)10) >> 64) + (((u64)(dot_one * (u128)10)) >> 63);
    // round to even
    if ((u64)(dot_one * (u128)10) == (1ull << 63)) [[unlikely]] // generate branch instruction
        one -= !(((dot_one * (u128)10) >> 64) & 1);             // if(even) one--;
    // adjust to correct result
    if (irregular) [[unlikely]] // generate branch instruction
    {
        one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217);
    }
    // round up
    if ((half_ulp == ((~0) - dot_one))) [[unlikely]] // generate branch instruction
    {
        one = odd ? one : 10; // generate cmov instruction
    }
    if (half_ulp > (~0 - dot_one)) // generate cmov instruction
    {
        one = 10;
    }
    // round down
    if (!irregular) [[likely]]
    {
        if (((half_ulp == dot_one))) [[unlikely]] // generate branch instruction
        {
            one = odd ? one : 0; // generate cmov instruction
        }
        if (half_ulp > dot_one) // generate cmov instruction
        {
            one = 0;
        }
    }
    else
    {
        if (half_ulp / 2 >= dot_one) // generate cmov instruction
        {
            one = 0;
        }
    }
    u32 dec = ten + one; // four possible outcomes ;
    return {dec, k};     // == dec * (10 ^ k) ;
}
static inline FloatingDecimal32 ToDecimal32_xjb_v5(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    // icpx -O3 -march=native 3.22ns
    // but g++ 17ns , why?
    typedef __uint128_t u128;
    typedef uint64_t u64;
    typedef uint32_t u32;
    typedef int64_t i64;
    const int BIT = 91; // need >= 91  ; 91 - 64 = 25; 25 > 24;
    const u64 g[(45 - (-32) + 1) * 2] = {
        0x67d88f5, 0x6a29cca5d33ef0c7, // -32
        0x40e7599, 0x625a1fe7a407567c, // -31
        0x51212ff, 0xbaf0a7e18d092c1b, // -30
        0x65697bf, 0xa9acd1d9f04b7722, // -29
        0x7ec3daf, 0x941806506c5e54eb, // -28
        0x4f3a68d, 0xbc8f03f243baf513, // -27
        0x6309031, 0x2bb2c4eed4a9b257, // -26
        0x7bcb43d, 0x769f762a89d41eed, // -25
        0x4d5f0a6, 0x6a23a9da96249354, // -24
        0x60b6cd0, 0x4ac94513badb829,  // -23
        0x78e4804, 0x5d7b9658a992634,  // -22
        0x4b8ed02, 0x83a6d3df769fb7e0, // -21
        0x5e72843, 0x249088d75447a5d8, // -20
        0x760f253, 0xedb4ab0d29598f4f, // -19
        0x49c9774, 0x7490eae839d7f991, // -18
        0x5c3bd51, 0x91b525a2484df7f5, // -17
        0x734aca5, 0xf6226f0ada6175f3, // -16
        0x480ebe7, 0xb9d58566c87ce9b8, // -15
        0x5a126e1, 0xa84ae6c07a9c2426, // -14
        0x709709a, 0x125da07099432d2f, // -13
        0x465e660, 0x4b7a84465fc9fc3d, // -12
        0x57f5ff8, 0x5e592557f7bc7b4d, // -11
        0x6df37f6, 0x75ef6eadf5ab9a20, // -10
        0x44b82fa, 0x9b5a52cb98b4054,  // -9
        0x55e63b8, 0x8c230e77e7ee1069, // -8
        0x6b5fca6, 0xaf2bd215e1e99483, // -7
        0x431bde8, 0x2d7b634dad31fcd2, // -6
        0x53e2d62, 0x38da3c21187e7c06, // -5
        0x68db8ba, 0xc710cb295e9e1b08, // -4
        0x4189374, 0xbc6a7ef9db22d0e5, // -3
        0x51eb851, 0xeb851eb851eb851e, // -2
        0x6666666, 0x6666666666666666, // -1
        0x4000000, 0x0,                // 0
        0x5000000, 0x0,                // 1
        0x6400000, 0x0,                // 2
        0x7d00000, 0x0,                // 3
        0x4e20000, 0x0,                // 4
        0x61a8000, 0x0,                // 5
        0x7a12000, 0x0,                // 6
        0x4c4b400, 0x0,                // 7
        0x5f5e100, 0x0,                // 8
        0x7735940, 0x0,                // 9
        0x4a817c8, 0x0,                // 10
        0x5d21dba, 0x0,                // 11
        0x746a528, 0x8000000000000000, // 12
        0x48c2739, 0x5000000000000000, // 13
        0x5af3107, 0xa400000000000000, // 14
        0x71afd49, 0x8d00000000000000, // 15
        0x470de4d, 0xf820000000000000, // 16
        0x58d15e1, 0x7628000000000000, // 17
        0x6f05b59, 0xd3b2000000000000, // 18
        0x4563918, 0x244f400000000000, // 19
        0x56bc75e, 0x2d63100000000000, // 20
        0x6c6b935, 0xb8bbd40000000000, // 21
        0x43c33c1, 0x9375648000000000, // 22
        0x54b40b1, 0xf852bda000000000, // 23
        0x69e10de, 0x76676d0800000000, // 24
        0x422ca8b, 0xa00a42500000000,  // 25
        0x52b7d2d, 0xcc80cd2e40000000, // 26
        0x6765c79, 0x3fa10079d0000000, // 27
        0x409f9cb, 0xc7c4a04c22000000, // 28
        0x50c783e, 0xb9b5c85f2a800000, // 29
        0x64f964e, 0x68233a76f5200000, // 30
        0x7e37be2, 0x22c0914b2680000,  // 31
        0x4ee2d6d, 0x415b85acef810000, // 32
        0x629b8c8, 0x91b267182b614000, // 33
        0x7b426fa, 0xb61f00de36399000, // 34
        0x4d0985c, 0xb1d3608ae1e3fa00, // 35
        0x604be73, 0xde4838ad9a5cf880, // 36
        0x785ee10, 0xd5da46d900f436a0, // 37
        0x4b3b4ca, 0x85a86c47a098a224, // 38
        0x5e0a1fd, 0x2712875988becaad, // 39
        0x758ca7c, 0x70d7292feaee7d58, // 40
        0x4977e8d, 0xc68679bdf2d50e57, // 41
        0x5bd5e31, 0x3828182d6f8a51ec, // 42
        0x72cb5bd, 0x86321e38cb6ce668, // 43
        0x47bf196, 0x73df52e37f241001, // 44
        0x59aedfc, 0x10d7279c5eed1401, // 45
    };
    u64 odd = ieee_significand & 1; // 0 or 1
    u64 even = (ieee_significand + 1) & 1;
    // printf("odd=%llu\n",odd);
    const int offset = 3;
    int exp_bin, k;
    u64 sig_bin, irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]]
    {                                  // branch
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-k - 1) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr = &g[32 * 2];
    u64 pow10_hi = pow10_ptr[(-k - 1) * 2];
    u64 pow10_lo = pow10_ptr[(-k - 1) * 2 + 1];
    u64 cb = sig_bin << (h1);                 // h1 max is 4
    u64 sig_hi = (cb * (u128)pow10_lo) >> 64; // one mulxq instruction on x86
    u64 sig_lo = (cb * (u128)pow10_lo);
    sig_hi += cb * pow10_hi;                 // cb max: 28bit ; pow10_100_hi: 36bit ; not overflow;
    const int offset2 = offset + (BIT - 64); // offset2 = 3+(91-64) = 30
    u32 ten = (sig_hi >> offset2) * 10;      //(sig_hi >> 30) * 10
    u64 dot_one = (sig_hi << (64 - offset2)) + (sig_lo >> offset2);
    u64 half_ulp = ((pow10_hi << (64 - (BIT - 64))) + (pow10_lo >> (BIT - 64))) >> (1 + offset - h1);
    u32 one = ((dot_one * (u128)10) >> 64) + (((u64)(dot_one * (u128)10)) >> 63); // round to nearest
    if ((u64)(dot_one * (u128)10) == (1ull << 63)) [[unlikely]]                   // branch
        one -= !(((dot_one * (u128)10) >> 64) & 1);                               // if(even) one--;
    // if ((half_ulp == ((~0) - dot_one))) [[unlikely]] // branch
    //     one = odd ? one : 10; // cmov
    // if (half_ulp > (~0 - dot_one)) // cmov
    //     one = 10;
    if (half_ulp + even > (~0 - dot_one))
        one = 10;
    if (!irregular) [[likely]]
    {
        // if (half_ulp > dot_one) //  cmov
        //     one = 0;
        // if (((half_ulp == dot_one))) [[unlikely]] // branch
        //     one = odd ? one : 0; //  cmov
        if (half_ulp + even > dot_one)
            one = 0;
    }
    else
    {
        one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217);
        if (half_ulp / 2 >= dot_one) // cmov
            one = 0;
    }
    return {ten + one, k}; // == (ten+one) * (10 ^ k) ;
}
inline FloatingDecimal32 ToDecimal32_xjb_v6(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    // icpx -O3 -march=native : 3.22ns
    typedef __uint128_t u128;
    typedef uint64_t u64;
    const int BIT = 91; // need >= 91;
    const u64 g[(45 - (-32) + 1) * 2] = {
        0x67d88f5, 0x6a29cca5d33ef0c7, // -32
        0x40e7599, 0x625a1fe7a407567c, // -31
        0x51212ff, 0xbaf0a7e18d092c1b, // -30
        0x65697bf, 0xa9acd1d9f04b7722, // -29
        0x7ec3daf, 0x941806506c5e54eb, // -28
        0x4f3a68d, 0xbc8f03f243baf513, // -27
        0x6309031, 0x2bb2c4eed4a9b257, // -26
        0x7bcb43d, 0x769f762a89d41eed, // -25
        0x4d5f0a6, 0x6a23a9da96249354, // -24
        0x60b6cd0, 0x4ac94513badb829,  // -23
        0x78e4804, 0x5d7b9658a992634,  // -22
        0x4b8ed02, 0x83a6d3df769fb7e0, // -21
        0x5e72843, 0x249088d75447a5d8, // -20
        0x760f253, 0xedb4ab0d29598f4f, // -19
        0x49c9774, 0x7490eae839d7f991, // -18
        0x5c3bd51, 0x91b525a2484df7f5, // -17
        0x734aca5, 0xf6226f0ada6175f3, // -16
        0x480ebe7, 0xb9d58566c87ce9b8, // -15
        0x5a126e1, 0xa84ae6c07a9c2426, // -14
        0x709709a, 0x125da07099432d2f, // -13
        0x465e660, 0x4b7a84465fc9fc3d, // -12
        0x57f5ff8, 0x5e592557f7bc7b4d, // -11
        0x6df37f6, 0x75ef6eadf5ab9a20, // -10
        0x44b82fa, 0x9b5a52cb98b4054,  // -9
        0x55e63b8, 0x8c230e77e7ee1069, // -8
        0x6b5fca6, 0xaf2bd215e1e99483, // -7
        0x431bde8, 0x2d7b634dad31fcd2, // -6
        0x53e2d62, 0x38da3c21187e7c06, // -5
        0x68db8ba, 0xc710cb295e9e1b08, // -4
        0x4189374, 0xbc6a7ef9db22d0e5, // -3
        0x51eb851, 0xeb851eb851eb851e, // -2
        0x6666666, 0x6666666666666666, // -1
        0x4000000, 0x0,                // 0
        0x5000000, 0x0,                // 1
        0x6400000, 0x0,                // 2
        0x7d00000, 0x0,                // 3
        0x4e20000, 0x0,                // 4
        0x61a8000, 0x0,                // 5
        0x7a12000, 0x0,                // 6
        0x4c4b400, 0x0,                // 7
        0x5f5e100, 0x0,                // 8
        0x7735940, 0x0,                // 9
        0x4a817c8, 0x0,                // 10
        0x5d21dba, 0x0,                // 11
        0x746a528, 0x8000000000000000, // 12
        0x48c2739, 0x5000000000000000, // 13
        0x5af3107, 0xa400000000000000, // 14
        0x71afd49, 0x8d00000000000000, // 15
        0x470de4d, 0xf820000000000000, // 16
        0x58d15e1, 0x7628000000000000, // 17
        0x6f05b59, 0xd3b2000000000000, // 18
        0x4563918, 0x244f400000000000, // 19
        0x56bc75e, 0x2d63100000000000, // 20
        0x6c6b935, 0xb8bbd40000000000, // 21
        0x43c33c1, 0x9375648000000000, // 22
        0x54b40b1, 0xf852bda000000000, // 23
        0x69e10de, 0x76676d0800000000, // 24
        0x422ca8b, 0xa00a42500000000,  // 25
        0x52b7d2d, 0xcc80cd2e40000000, // 26
        0x6765c79, 0x3fa10079d0000000, // 27
        0x409f9cb, 0xc7c4a04c22000000, // 28
        0x50c783e, 0xb9b5c85f2a800000, // 29
        0x64f964e, 0x68233a76f5200000, // 30
        0x7e37be2, 0x22c0914b2680000,  // 31
        0x4ee2d6d, 0x415b85acef810000, // 32
        0x629b8c8, 0x91b267182b614000, // 33
        0x7b426fa, 0xb61f00de36399000, // 34
        0x4d0985c, 0xb1d3608ae1e3fa00, // 35
        0x604be73, 0xde4838ad9a5cf880, // 36
        0x785ee10, 0xd5da46d900f436a0, // 37
        0x4b3b4ca, 0x85a86c47a098a224, // 38
        0x5e0a1fd, 0x2712875988becaad, // 39
        0x758ca7c, 0x70d7292feaee7d58, // 40
        0x4977e8d, 0xc68679bdf2d50e57, // 41
        0x5bd5e31, 0x3828182d6f8a51ec, // 42
        0x72cb5bd, 0x86321e38cb6ce668, // 43
        0x47bf196, 0x73df52e37f241001, // 44
        0x59aedfc, 0x10d7279c5eed1401, // 45
    };
    // u64 even =  ((ieee_significand + 1) & 1);
    const int offset = 3;
    int exp_bin, k;                                   // exp_bin=q
    u64 sig_bin, irregular = (ieee_significand == 0); // sig_bin=c
    if (ieee_exponent != 0) [[likely]]                // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23,max is 254-150=104
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150; //-149
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-1 - k) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr = &g[32 * 2];
    u64 pow10_hi = pow10_ptr[(-1 - k) * 2 + 0];
    u64 pow10_lo = pow10_ptr[(-1 - k) * 2 + 1];
    u64 cb = sig_bin << (h1);                                   // h1 max is 4
    u64 sig_hi = ((cb * (u128)pow10_lo) >> 64) + cb * pow10_hi; // one mulxq instruction on x86
    u64 sig_lo = (cb * (u128)pow10_lo);
    // sig_hi += cb * pow10_hi;               // cb max: 28bit ; pow10_hi: 27bit ; not overflow;
    const int offset2 = offset + (BIT - 64); // offset2 = 3 + ( 91 - 64) = 30
    u64 ten = (sig_hi >> offset2) * 10;      // (sig_hi >> 30) * 10
    u64 dot_one = (sig_hi << (64 - offset2)) | (sig_lo >> offset2);
    // u64 dot_one = (sig_hi << (64 - offset2));
    u64 half_ulp = ((pow10_hi << (64 - (BIT - 64))) | (pow10_lo >> (BIT - 64))) >> (1 + offset - h1);
    u64 one = ((dot_one * (u128)10) >> 64) + (((u64)(dot_one * (u128)10)) >> 63); // round to nearest
    if ((u64)(dot_one * (u128)10) == (1ull << 63)) [[unlikely]]                   // branch
        one -= ((((dot_one * (u128)10) >> 64) + 1) & 1);                          // if(even) one--; round to even
    u64 even = ((ieee_significand + 1) & 1);
    if (half_ulp + even > (~0 - dot_one))
        one = 10; // round up , cmov
    if (!irregular) [[likely]]
    {
        if (half_ulp + even > dot_one)
            one = 0; // round down , cmov
    }
    else
    {
        if (half_ulp / 2 > dot_one)                                                     // use >= also right
            one = 0;                                                                    // round down , cmov
        one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217); // special value
    }
    // printf("half_ulp=%llx,half_ulp + even=%llx,dot_one=%llx,(~0 - dot_one)=%llx\n",half_ulp,half_ulp+even,dot_one,(~0 - dot_one));
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ^ k) ;
}
inline FloatingDecimal32 ToDecimal32_xjb_v7(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    // icpx -O3 -march=native : 3.22ns
    typedef __uint128_t u128;
    typedef uint64_t u64;
    const int BIT = 91; // need >= 91;
    const u64 g[(45 - (-32) + 1)] = {
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
        0xb35dbf821ae4f38c, // 45
    };
    // u64 even =  ((ieee_significand + 1) & 1);
    const int offset = 3;
    int exp_bin, k;
    u64 sig_bin, irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-1 - k) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr = &g[32];
    u64 pow10_hi = pow10_ptr[(-1 - k)];
    u64 cb = sig_bin << (h1);                   // h1 max is 4
    u64 sig_hi = ((cb * (u128)pow10_hi) >> 64); // one mulxq instruction on x86
    u64 sig_lo = (cb * (u128)pow10_hi);
    u64 ten = (sig_hi >> offset) * 10; // (sig_hi >> 30) * 10
    u64 dot_one = (sig_hi << (64 - offset)) | (sig_lo >> offset);
    u64 half_ulp = pow10_hi >> (1 + offset - h1);
    u64 one = ((dot_one * (u128)10) >> 64) + (((u64)(dot_one * (u128)10)) >> 63); // round to nearest
    if ((u64)(dot_one * (u128)10) == (1ull << 63)) [[unlikely]]                   // branch
        one -= ((((dot_one * (u128)10) >> 64) + 1) & 1);                          // if(even) one--; round to even
    u64 even = ((ieee_significand + 1) & 1);
    const int offset_bit = 64 - 36; // equal 28 ; only need high 36 bit.
    if (((half_ulp >> offset_bit) + even) > ((~0 - dot_one) >> offset_bit))
        one = 10; // round up , cmov
    if (!irregular) [[likely]]
    {
        if (((half_ulp >> offset_bit) + even) > (dot_one >> offset_bit))
            one = 0; // round down , cmov
    }
    else
    {
        if ((half_ulp >> (offset_bit + 1)) > (dot_one >> offset_bit))                   // use >= also right
            one = 0;                                                                    // round down , cmov
        one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217); // special value
    }
    if (0)
        printf("half_ulp=%llx,half_ulp + even=%llx,dot_one=%llx,(~0 - dot_one)=%llx\n ,(half_ulp>>25)=%llx, (half_ulp>>25) + even=%llx,(dot_one>>23)=%llx,(( ~0 - dot_one)>>23)=%llx\n",
               half_ulp, half_ulp + even, dot_one, (~0 - dot_one), (half_ulp >> offset_bit), (half_ulp >> offset_bit) + even, (dot_one >> offset_bit), ((~0 - dot_one) >> offset_bit));
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ^ k) ;
}
inline FloatingDecimal32 ToDecimal32_xjb_v7_v2(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    // icpx -O3 -march=native : 3.12ns
    typedef __uint128_t u128;
    typedef uint64_t u64;
    const u64 g[(45 - (-32) + 1)] = {
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
        0xb35dbf821ae4f38c, // 45
    };
    const int offset = 3;
    int exp_bin, k;
    u64 sig_bin, irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-1 - k) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr = &g[32];
    u64 pow10_hi = pow10_ptr[(-1 - k)];
    u64 cb = sig_bin << (h1);                   // h1 max is 4
    u64 sig_hi = ((cb * (u128)pow10_hi) >> 64); // one mulxq instruction on x86
    u64 sig_lo = (cb * (u128)pow10_hi);
    u64 ten = (sig_hi >> offset) * 10;
    const int offset_bit = 64 - 36; // equal 28 ; only need high 36 bit.
    u64 dot_one_36bit = ((sig_hi << (64 - offset)) | (sig_lo >> offset)) >> offset_bit;
    u64 half_ulp2 = pow10_hi >> (1 + offset + offset_bit - h1);
    u64 one = (dot_one_36bit * 10 + ((u64)1 << 35) - (((dot_one_36bit * 10) & ((u64)1 << 36)) == 0)) >> 36; // round to nearest
    // u64 one = (dot_one_36bit * 10 + (((u64)1 << 35) - 1) + ( ( (dot_one_36bit * 10) >> 36 ) & 1 ) ) >> 36; // round to nearest
    u64 even = ((ieee_significand + 1) & 1);
    if ((half_ulp2 + even) > (((u64)1 << 36) - 1) - dot_one_36bit)
        one = 10; // round up , cmov
    if (!irregular) [[likely]]
    {
        if ((half_ulp2 + even) > dot_one_36bit)
            one = 0; // round down , cmov
    }
    else
    {
        if ((half_ulp2 >> 1) > dot_one_36bit)
            one = 0;                                                                    // round down , cmov
        one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217); // special value
    }
    // if(0)
    // printf("half_ulp=%llx,half_ulp + even=%llx,dot_one=%llx,(~0 - dot_one)=%llx\n ,(half_ulp>>25)=%llx, (half_ulp>>25) + even=%llx,(dot_one>>23)=%llx,(( ~0 - dot_one)>>23)=%llx\n",
    //         half_ulp,half_ulp+even,dot_one,(~0 - dot_one),(half_ulp>>offset_bit),(half_ulp>>offset_bit) + even,(dot_one>>offset_bit),(( ~0 - dot_one)>>offset_bit));
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ^ k) ;
}
static inline FloatingDecimal32 ToDecimal32_xjb_v7_v3(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef uint64_t u64;
    typedef uint32_t u32;
    const u64 g[(44 - (-32) + 1)] = {
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
    u64 regular = ieee_significand , sig_bin;
    if (ieee_exponent) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1u << 23);
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
    int h = exp_bin + (((-1 - k) * 217707) >> 16);// [-4,-1]
    u64 pow10 = (&g[32]) [-1-k];
    const int BIT = 36;// [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);// h + 37 = [-3,0]+36 = [33,36]
    u64 sig_hi = (cb * (__uint128_t)pow10) >> 64; // one mulxq instruction on x86
    u64 ten = (sig_hi >> BIT) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    u64 half_ulp = pow10 >> ((64 - BIT) - h);
    u64 offset_num = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    //u64 add_num = (((u64)1 << (BIT)) - 1) + (dot_one_36bit >> (BIT - 1));  //why slower?
    u64 one = ( dot_one_36bit * 20 + offset_num ) >> (BIT + 1);
    u64 even = 1 - (ieee_significand & 1);
    if (regular) [[likely]] // branch
    {
        one = half_ulp + even > dot_one_36bit ? 0 : one; // round down , cmov
        one = half_ulp + even > (((u64)1 << BIT) - 1) - dot_one_36bit ? 10 : one; // round up , cmov
    }
    else
    {
        one = half_ulp >> 1 > dot_one_36bit ? 0 : one; // round down , cmov
        one = (half_ulp > (((u64)1 << BIT) - 1) - dot_one_36bit) ? 10 : one; // round up , cmov
        one += (exp_bin == 31-150) + (exp_bin == 214-150) + (exp_bin == 217-150);// more fast
    }
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ** k) ;
}

static inline FloatingDecimal32 ToDecimal32_xjb_v7_v3_m(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef __uint128_t u128;
    typedef uint64_t u64;
    typedef uint32_t u32;
    const u64 g[(44 - (-32) + 1)] = {
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
    u64 sig_bin, irregular = (ieee_significand == 0);
    u64 regular = (ieee_significand);
    u64 even = (ieee_significand + 1) & 1;
    u64 odd = (ieee_significand) & 1;
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
    int h = exp_bin + (((-1 - k) * 217707) >> 16);// [-4,-1]
    const u64 *pow10_ptr = &g[32];
    u64 pow10_hi = pow10_ptr[(-1 - k)];
    const int BIT = 36;// [33,36] all right
    u64 cb = sig_bin << (h + 1 + BIT);// h + 37 : 36
    u64 sig_hi = ((cb * (u128)pow10_hi) >> 64); // one mulxq instruction on x86
    u64 ten = (sig_hi >> BIT) * 10;
    return {(u32)ten,k};
    // u64 dot_one_36bit = sig_hi & (((u64)1 << BIT) - 1); // only need high 36 bit
    // u64 half_ulp = pow10_hi >> ((64 - BIT) - h);
    // //u64 add_num = (((u64)1 << BIT ) - 1) + (((dot_one_36bit * 20) >> (BIT + 1)) & 1 );//round to nearest
    // //u64 add_num = (((u64)1 << (BIT - 1 )) - 1) + (dot_one_36bit >> (BIT - 1));//also right
    // //u64 add_num = (((u64)1 << 36) - 7) + (((dot_one_36bit * 20) >> 37));//also right,
    // //u64 one = (dot_one_36bit * 20 + add_num) >> (BIT + 1);
    // //u64 add_num = (((u64)1 << (BIT - 1)) - 1) + (dot_one_36bit >> (BIT - 1));
    // u64 add_num = (((u64)1 << BIT) - 7) + (dot_one_36bit >> (BIT - 4));
    // //u64 add_num = (((u64)1 << BIT) - 1) + (dot_one_36bit >> (BIT - 1));
    // u64 one = ( dot_one_36bit * 20 + add_num ) >> (BIT + 1);
    // //u64 add_num = (((u64)1 << (BIT)) - 1) + (dot_one_36bit >> (BIT));
    // //u64 one = ( dot_one_36bit * 20 + add_num) >> (BIT + 1);
    // //u64 one = ((dot_one_36bit) + (dot_one_36bit >> 2) + add_num) >> 33;
    // //u64 one = (dot_one_36bit + (dot_one_36bit << 2) + (((u64)1 << 34) - 7) + (dot_one_36bit >> 32)) >> 35;
    // //u64 one = (dot_one_36bit * 20  + (((u64)1 << 36) - 1 ) + (dot_one_36bit >> 35) ) >> 37;
    // //u64 one = (dot_one_36bit * 20  + (((u64)1 << 36) - 1 ) + ((sig_hi >> 35) & 1) ) >> 37;
    // //u64 one = (dot_one_36bit * 21 - (((u64)1 << 36) + 1) ) >> 37;
    // //u64 even = (ieee_significand + 1) & 1;
    // //u64 r=0;
    // if (regular) [[likely]] // branch
    // {
    //     if ((half_ulp + even) > dot_one_36bit)
    //         one = 0; // round down , cmov
    //     if ((half_ulp + even) > (((u64)1 << BIT) - 1) - dot_one_36bit )
    //         one = 10;                       // round up , cmov

    //     // if ((half_ulp - odd ) >= dot_one_36bit)
    //     //     one = 0; // round down , cmov
    //     // if ((half_ulp - odd)  >= (((u64)1 << BIT) - 1) - dot_one_36bit)
    //     //     one = 10;                       // round up , cmov

    //     // u64 a = ((dot_one_36bit - half_ulp + odd   ) >> 60) & one;
    //     // u64 b = ((((((u64)1 << BIT) - 1) - dot_one_36bit) - half_ulp + odd    ) >> 60) & ( 10 - one);
    //     // //printf("a=%llx,b=%llx,half_ulp=%llx,odd=%llx,dot_one_36bit=%llx\n", a, b, half_ulp, odd, dot_one_36bit);
    //     // r = ten + one - a + b;
    // }
    // else
    // {
    //     // if (((dot_one_36bit * 10) & ((1ull << 36) - 1)) > half_ulp * 5) // slow
    //     //     one = ((dot_one_36bit * 10) >> 36) +1;
    //     if ((half_ulp >> 1) > dot_one_36bit)
    //         one = 0; // round down , cmov
    //     if ((half_ulp + even) > (((u64)1 << BIT) - 1) - dot_one_36bit )
    //         one = 10;                      // round up , cmov
    //     // if ((half_ulp - odd) > (((u64)1 << BIT) - 1) - dot_one_36bit )
    //     //     one = 10;                      // round up , cmov
    //     //one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217); // special value
    //     one += (exp_bin == 31-150) + (exp_bin == 214-150) + (exp_bin == 217-150);// more fast
    // }
    // return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ** k) ;
}



inline FloatingDecimal32 ToDecimal32_xjb_v7_v4(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    typedef __uint128_t u128;
    typedef uint64_t u64;
    const u64 g[(45 - (-32) + 1)] = {
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
        0xb35dbf821ae4f38c, // 45
    };
    int exp_bin, k;
    u64 sig_bin, irregular = (ieee_significand == 0);
    if (ieee_exponent != 0) [[likely]] // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150;
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h = exp_bin + (((-1 - k) * 217707) >> 16);
    const u64 *pow10_ptr = &g[32];
    u64 pow10_hi = pow10_ptr[(-1 - k)];
    u64 cb = sig_bin << (h + 37);
    u64 sig_hi = ((cb * (u128)pow10_hi) >> 64); // one mulxq instruction on x86
    u64 ten = (sig_hi >> 36) * 10;
    u64 dot_one_36bit = sig_hi & (((u64)1 << 36) - 1); // only need high 36 bit
    u64 half_ulp = pow10_hi >> (28 - h);

    u64 add_num = (((u64)1 << 36) - 1) + (((dot_one_36bit * 20) >> 37) & 1);
    if(irregular)[[unlikely]]
    {
        //if (((dot_one_36bit * 20) & ((1ull << 37) - 1)) > half_ulp * 10)
        if (((dot_one_36bit * 10) & ((1ull << 36) - 1)) > half_ulp * 5)
            add_num = 1ull<<37;
    }

    //u64 one = (dot_one_36bit * 20 + (((u64)1 << 36) - 1) + (((dot_one_36bit * 20) >> 37) & 1)) >> 37;
    u64 one = (dot_one_36bit * 20 + add_num) >> 37;
    u64 even = ((ieee_significand + 1) & 1);
    //u64 even = 1 - ((ieee_significand) & 1);
    // if ((half_ulp + even) > (((u64)1 << 36) - 1) - dot_one_36bit)
    //  if ((half_ulp + even) + dot_one_36bit > (((u64)1 << 36) - 1) )
    //      one = 10; // round up , cmov
    //  if ((half_ulp + even) > dot_one_36bit)
    //          one = 0; // round down , cmov

    // if (((half_ulp + even)>>irregular) > dot_one_36bit)
    //         one = 0; // round down , cmov
    if (!irregular) [[likely]]
    {
        if ((half_ulp + even) > dot_one_36bit)
            one = 0; // round down , cmov
    }
    else
    {
        // if (1)
        //     if (((dot_one_36bit * 10) & ((1ull << 36) - 1)) > half_ulp * 5)
        //     {
        //         one = ((dot_one_36bit * 10) >> 36) + 1;
        //     }
        // // more fast
        // else
        //     one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217); // special value

        if ((half_ulp >> 1) > dot_one_36bit)
            one = 0; // round down , cmov
    }
    if ((half_ulp + even) + dot_one_36bit > (((u64)1 << 36) - 1))
        one = 10;                      // round up , cmov
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ** k) ;
}

inline FloatingDecimal32 ToDecimal32_xjb_v8(uint32_t ieee_significand, uint32_t ieee_exponent)
{
    // icpx -O3 -march=native : 3.22ns
    typedef __uint128_t u128;
    typedef uint64_t u64;
    const int BIT = 91; // need >= 91;
    const u64 g[(44 - (-32) + 1) * 2] = {
        0x67d88f5, 0x6a29cca5d33ef0c8, // -32
        0x40e7599, 0x625a1fe7a407567d, // -31
        0x51212ff, 0xbaf0a7e18d092c1c, // -30
        0x65697bf, 0xa9acd1d9f04b7723, // -29
        0x7ec3daf, 0x941806506c5e54ec, // -28
        0x4f3a68d, 0xbc8f03f243baf514, // -27
        0x6309031, 0x2bb2c4eed4a9b258, // -26
        0x7bcb43d, 0x769f762a89d41eee, // -25
        0x4d5f0a6, 0x6a23a9da96249355, // -24
        0x60b6cd0, 0x4ac94513badb82a,  // -23
        0x78e4804, 0x5d7b9658a992635,  // -22
        0x4b8ed02, 0x83a6d3df769fb7e1, // -21
        0x5e72843, 0x249088d75447a5d9, // -20
        0x760f253, 0xedb4ab0d29598f50, // -19
        0x49c9774, 0x7490eae839d7f992, // -18
        0x5c3bd51, 0x91b525a2484df7f6, // -17
        0x734aca5, 0xf6226f0ada6175f4, // -16
        0x480ebe7, 0xb9d58566c87ce9b9, // -15
        0x5a126e1, 0xa84ae6c07a9c2427, // -14
        0x709709a, 0x125da07099432d30, // -13
        0x465e660, 0x4b7a84465fc9fc3e, // -12
        0x57f5ff8, 0x5e592557f7bc7b4e, // -11
        0x6df37f6, 0x75ef6eadf5ab9a21, // -10
        0x44b82fa, 0x9b5a52cb98b4055,  // -9
        0x55e63b8, 0x8c230e77e7ee106a, // -8
        0x6b5fca6, 0xaf2bd215e1e99484, // -7
        0x431bde8, 0x2d7b634dad31fcd3, // -6
        0x53e2d62, 0x38da3c21187e7c07, // -5
        0x68db8ba, 0xc710cb295e9e1b09, // -4
        0x4189374, 0xbc6a7ef9db22d0e6, // -3
        0x51eb851, 0xeb851eb851eb851f, // -2
        0x6666666, 0x6666666666666667, // -1
        0x4000000, 0x0,                // 0
        0x5000000, 0x0,                // 1
        0x6400000, 0x0,                // 2
        0x7d00000, 0x0,                // 3
        0x4e20000, 0x0,                // 4
        0x61a8000, 0x0,                // 5
        0x7a12000, 0x0,                // 6
        0x4c4b400, 0x0,                // 7
        0x5f5e100, 0x0,                // 8
        0x7735940, 0x0,                // 9
        0x4a817c8, 0x0,                // 10
        0x5d21dba, 0x0,                // 11
        0x746a528, 0x8000000000000000, // 12
        0x48c2739, 0x5000000000000000, // 13
        0x5af3107, 0xa400000000000000, // 14
        0x71afd49, 0x8d00000000000000, // 15
        0x470de4d, 0xf820000000000000, // 16
        0x58d15e1, 0x7628000000000000, // 17
        0x6f05b59, 0xd3b2000000000000, // 18
        0x4563918, 0x244f400000000000, // 19
        0x56bc75e, 0x2d63100000000000, // 20
        0x6c6b935, 0xb8bbd40000000000, // 21
        0x43c33c1, 0x9375648000000000, // 22
        0x54b40b1, 0xf852bda000000000, // 23
        0x69e10de, 0x76676d0800000000, // 24
        0x422ca8b, 0xa00a42500000000,  // 25
        0x52b7d2d, 0xcc80cd2e40000000, // 26
        0x6765c79, 0x3fa10079d0000000, // 27
        0x409f9cb, 0xc7c4a04c22000000, // 28
        0x50c783e, 0xb9b5c85f2a800000, // 29
        0x64f964e, 0x68233a76f5200000, // 30
        0x7e37be2, 0x22c0914b2680000,  // 31
        0x4ee2d6d, 0x415b85acef810000, // 32
        0x629b8c8, 0x91b267182b614000, // 33
        0x7b426fa, 0xb61f00de36399000, // 34
        0x4d0985c, 0xb1d3608ae1e3fa00, // 35
        0x604be73, 0xde4838ad9a5cf880, // 36
        0x785ee10, 0xd5da46d900f436a0, // 37
        0x4b3b4ca, 0x85a86c47a098a224, // 38
        0x5e0a1fd, 0x2712875988becaad, // 39
        0x758ca7c, 0x70d7292feaee7d59, // 40
        0x4977e8d, 0xc68679bdf2d50e58, // 41
        0x5bd5e31, 0x3828182d6f8a51ed, // 42
        0x72cb5bd, 0x86321e38cb6ce669, // 43
        0x47bf196, 0x73df52e37f241002, // 44
    };
    // u64 even =  ((ieee_significand + 1) & 1);
    const int offset = 3;
    int exp_bin, k;                                   // exp_bin=q
    u64 sig_bin, irregular = (ieee_significand == 0); // sig_bin=c
    if (ieee_exponent != 0) [[likely]]                // branch
    {
        exp_bin = ieee_exponent - 150; //-127-23,max is 254-150=104
        sig_bin = ieee_significand | (1 << 23);
    }
    else
    {
        exp_bin = 1 - 150; //-149
        sig_bin = ieee_significand;
    }
    if (!irregular) [[likely]] // branch
        k = (exp_bin * 315653) >> 20;
    else
        k = (exp_bin * 315653 - 131237) >> 20;
    int h1 = exp_bin + (((-1 - k) * 217707) >> 16) + 1 + offset; // [1,4]
    const u64 *pow10_ptr = &g[32 * 2];
    u64 pow10_hi = pow10_ptr[(-1 - k) * 2 + 0];
    u64 pow10_lo = pow10_ptr[(-1 - k) * 2 + 1];
    u64 cb = sig_bin << (h1);                                   // h1 max is 4
    u64 sig_hi = ((cb * (u128)pow10_lo) >> 64) + cb * pow10_hi; // one mulxq instruction on x86
    u64 sig_lo = (cb * (u128)pow10_lo);
    // sig_hi += cb * pow10_hi;               // cb max: 28bit ; pow10_hi: 27bit ; not overflow;
    const int offset2 = offset + (BIT - 64); // offset2 = 3 + ( 91 - 64) = 30
    u64 ten = (sig_hi >> offset2) * 10;      // (sig_hi >> 30) * 10
    u64 dot_one = (sig_hi << (64 - offset2)) | (sig_lo >> offset2);
    // u64 dot_one = (sig_hi << (64 - offset2));
    u64 half_ulp = ((pow10_hi << (64 - (BIT - 64))) | (pow10_lo >> (BIT - 64))) >> (1 + offset - h1);
    u64 one = ((dot_one * (u128)10) >> 64) + (((u64)(dot_one * (u128)10)) >> 63); // round to nearest
    if ((u64)(dot_one * (u128)10) == (1ull << 63)) [[unlikely]]                   // branch
        one -= ((((dot_one * (u128)10) >> 64) + 1) & 1);                          // if(even) one--; round to even
    u64 even = ((ieee_significand + 1) & 1);
    if (half_ulp + even > (~0 - dot_one))
        one = 10; // round up , cmov
    if (!irregular) [[likely]]
    {
        if (half_ulp + even > dot_one)
            one = 0; // round down , cmov
    }
    else
    {
        if (half_ulp / 2 > dot_one)                                                     // use >= also right
            one = 0;                                                                    // round down , cmov
        one += (ieee_exponent == 31) | (ieee_exponent == 214) | (ieee_exponent == 217); // special value
    }
    // printf("half_ulp=%llx,half_ulp + even=%llx,dot_one=%llx,(~0 - dot_one)=%llx\n",half_ulp,half_ulp+even,dot_one,(~0 - dot_one));
    return {(uint32_t)(ten + one), k}; // == (ten + one) * (10 ^ k) ;
}
#endif
//==================================================================================================
// ToChars
//==================================================================================================

static inline void Utoa_2Digits(char *buf, uint32_t digits)
{
    static constexpr char Digits100[200] = {
        '0',
        '0',
        '0',
        '1',
        '0',
        '2',
        '0',
        '3',
        '0',
        '4',
        '0',
        '5',
        '0',
        '6',
        '0',
        '7',
        '0',
        '8',
        '0',
        '9',
        '1',
        '0',
        '1',
        '1',
        '1',
        '2',
        '1',
        '3',
        '1',
        '4',
        '1',
        '5',
        '1',
        '6',
        '1',
        '7',
        '1',
        '8',
        '1',
        '9',
        '2',
        '0',
        '2',
        '1',
        '2',
        '2',
        '2',
        '3',
        '2',
        '4',
        '2',
        '5',
        '2',
        '6',
        '2',
        '7',
        '2',
        '8',
        '2',
        '9',
        '3',
        '0',
        '3',
        '1',
        '3',
        '2',
        '3',
        '3',
        '3',
        '4',
        '3',
        '5',
        '3',
        '6',
        '3',
        '7',
        '3',
        '8',
        '3',
        '9',
        '4',
        '0',
        '4',
        '1',
        '4',
        '2',
        '4',
        '3',
        '4',
        '4',
        '4',
        '5',
        '4',
        '6',
        '4',
        '7',
        '4',
        '8',
        '4',
        '9',
        '5',
        '0',
        '5',
        '1',
        '5',
        '2',
        '5',
        '3',
        '5',
        '4',
        '5',
        '5',
        '5',
        '6',
        '5',
        '7',
        '5',
        '8',
        '5',
        '9',
        '6',
        '0',
        '6',
        '1',
        '6',
        '2',
        '6',
        '3',
        '6',
        '4',
        '6',
        '5',
        '6',
        '6',
        '6',
        '7',
        '6',
        '8',
        '6',
        '9',
        '7',
        '0',
        '7',
        '1',
        '7',
        '2',
        '7',
        '3',
        '7',
        '4',
        '7',
        '5',
        '7',
        '6',
        '7',
        '7',
        '7',
        '8',
        '7',
        '9',
        '8',
        '0',
        '8',
        '1',
        '8',
        '2',
        '8',
        '3',
        '8',
        '4',
        '8',
        '5',
        '8',
        '6',
        '8',
        '7',
        '8',
        '8',
        '8',
        '9',
        '9',
        '0',
        '9',
        '1',
        '9',
        '2',
        '9',
        '3',
        '9',
        '4',
        '9',
        '5',
        '9',
        '6',
        '9',
        '7',
        '9',
        '8',
        '9',
        '9',
    };

    SF_ASSERT(digits <= 99);
    std::memcpy(buf, &Digits100[2 * digits], 2);
}

static inline int32_t TrailingZeros_2Digits(uint32_t digits)
{
    static constexpr int8_t TrailingZeros100[100] = {
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

    SF_ASSERT(digits <= 99);
    return TrailingZeros100[digits];
}

static inline int32_t PrintDecimalDigitsBackwards(char *buf, uint32_t output)
{
    int32_t tz = 0; // number of trailing zeros removed.
    int32_t nd = 0; // number of decimal digits processed.

    // At most 9 digits remaining

    if (output >= 10000)
    {
        const uint32_t q = output / 10000;
        const uint32_t r = output % 10000;
        output = q;
        buf -= 4;
        if (r != 0)
        {
            const uint32_t rH = r / 100;
            const uint32_t rL = r % 100;
            Utoa_2Digits(buf + 0, rH);
            Utoa_2Digits(buf + 2, rL);

            tz = TrailingZeros_2Digits(rL == 0 ? rH : rL) + (rL == 0 ? 2 : 0);
        }
        else
        {
            tz = 4;
        }
        nd = 4;
    }

    // At most 5 digits remaining.

    if (output >= 100)
    {
        const uint32_t q = output / 100;
        const uint32_t r = output % 100;
        output = q;
        buf -= 2;
        Utoa_2Digits(buf, r);
        if (tz == nd)
        {
            tz += TrailingZeros_2Digits(r);
        }
        nd += 2;

        if (output >= 100)
        {
            const uint32_t q2 = output / 100;
            const uint32_t r2 = output % 100;
            output = q2;
            buf -= 2;
            Utoa_2Digits(buf, r2);
            if (tz == nd)
            {
                tz += TrailingZeros_2Digits(r2);
            }
            nd += 2;
        }
    }

    // At most 2 digits remaining.

    SF_ASSERT(output >= 1);
    SF_ASSERT(output <= 99);

    if (output >= 10)
    {
        const uint32_t q = output;
        buf -= 2;
        Utoa_2Digits(buf, q);
        if (tz == nd)
        {
            tz += TrailingZeros_2Digits(q);
        }
        //      nd += 2;
    }
    else
    {
        const uint32_t q = output;
        SF_ASSERT(q >= 1);
        SF_ASSERT(q <= 9);
        *--buf = static_cast<char>('0' + q);
    }

    return tz;
}

static inline int32_t DecimalLength(uint32_t v)
{
    SF_ASSERT(v >= 1);
    SF_ASSERT(v <= 999999999u);

    if (v >= 100000000u)
    {
        return 9;
    }
    if (v >= 10000000u)
    {
        return 8;
    }
    if (v >= 1000000u)
    {
        return 7;
    }
    if (v >= 100000u)
    {
        return 6;
    }
    if (v >= 10000u)
    {
        return 5;
    }
    if (v >= 1000u)
    {
        return 4;
    }
    if (v >= 100u)
    {
        return 3;
    }
    if (v >= 10u)
    {
        return 2;
    }
    return 1;
}

static inline char *FormatDigits(char *buffer, uint32_t digits, int32_t decimal_exponent, bool force_trailing_dot_zero = false)
{
    static constexpr int32_t MinFixedDecimalPoint = -4;
    static constexpr int32_t MaxFixedDecimalPoint = 9;
    static_assert(MinFixedDecimalPoint <= -1, "internal error");
    static_assert(MaxFixedDecimalPoint >= 1, "internal error");

    SF_ASSERT(digits >= 1);
    SF_ASSERT(digits <= 999999999u);
    SF_ASSERT(decimal_exponent >= -99);
    SF_ASSERT(decimal_exponent <= 99);

    int32_t num_digits = DecimalLength(digits);
    const int32_t decimal_point = num_digits + decimal_exponent;

    const bool use_fixed = MinFixedDecimalPoint <= decimal_point && decimal_point <= MaxFixedDecimalPoint;

    // Prepare the buffer.
    // Avoid calling memset/memcpy with variable arguments below...

    std::memset(buffer + 0, '0', 16);
    std::memset(buffer + 16, '0', 16);
    static_assert(MinFixedDecimalPoint >= -30, "internal error");
    static_assert(MaxFixedDecimalPoint <= 32, "internal error");

    int32_t decimal_digits_position;
    if (use_fixed)
    {
        if (decimal_point <= 0)
        {
            // 0.[000]digits
            decimal_digits_position = 2 - decimal_point;
        }
        else
        {
            // dig.its
            // digits[000]
            decimal_digits_position = 0;
        }
    }
    else
    {
        // dE+123 or d.igitsE+123
        decimal_digits_position = 1;
    }

    char *digits_end = buffer + decimal_digits_position + num_digits;

    const int32_t tz = PrintDecimalDigitsBackwards(digits_end, digits);
    digits_end -= tz;
    num_digits -= tz;
    //  decimal_exponent += tz; // => decimal_point unchanged.

    if (use_fixed)
    {
        if (decimal_point <= 0)
        {
            // 0.[000]digits
            buffer[1] = '.';
            buffer = digits_end;
        }
        else if (decimal_point < num_digits)
        {
            // dig.its
            std::memmove(buffer + decimal_point + 1, buffer + decimal_point, 8);
            buffer[decimal_point] = '.';
            buffer = digits_end + 1;
        }
        else
        {
            // digits[000]
            buffer += decimal_point;
            if (force_trailing_dot_zero)
            {
                std::memcpy(buffer, ".0", 2);
                buffer += 2;
            }
        }
    }
    else
    {
        buffer[0] = buffer[1];
        if (num_digits == 1)
        {
            // dE+123
            ++buffer;
        }
        else
        {
            // d.igitsE+123
            buffer[1] = '.';
            buffer = digits_end;
        }

        const int32_t scientific_exponent = decimal_point - 1;
        //      SF_ASSERT(scientific_exponent != 0);

        std::memcpy(buffer, scientific_exponent < 0 ? "e-" : "e+", 2);
        buffer += 2;

        const uint32_t k = static_cast<uint32_t>(scientific_exponent < 0 ? -scientific_exponent : scientific_exponent);
        if (k < 10)
        {
            *buffer++ = static_cast<char>('0' + k);
        }
        else
        {
            Utoa_2Digits(buffer, k);
            buffer += 2;
        }
    }

    return buffer;
}

static inline char *ToChars(char *buffer, float value, bool force_trailing_dot_zero = false)
{
    const Single v(value);

    const uint32_t significand = v.PhysicalSignificand();
    const uint32_t exponent = v.PhysicalExponent();

    if (exponent != Single::MaxIeeeExponent) // [[likely]]
    {
        // Finite

        buffer[0] = '-';
        buffer += v.SignBit();

        if (exponent != 0 || significand != 0) // [[likely]]
        {
            // != 0

            const auto dec = ToDecimal32(significand, exponent);
            return FormatDigits(buffer, dec.digits, dec.exponent, force_trailing_dot_zero);
        }
        else
        {
            std::memcpy(buffer, "0.0 ", 4);
            buffer += force_trailing_dot_zero ? 3 : 1;
            return buffer;
        }
    }

    if (significand == 0)
    {
        buffer[0] = '-';
        buffer += v.SignBit();

        std::memcpy(buffer, "inf ", 4);
        return buffer + 3;
    }
    else
    {
        std::memcpy(buffer, "nan ", 4);
        return buffer + 3;
    }
}

//==================================================================================================
//
//==================================================================================================

char *Ftoa(char *buffer, float value)
{
    return ToChars(buffer, value);
}

}
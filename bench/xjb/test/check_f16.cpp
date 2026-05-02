#include <iostream>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>
#include <string>

// -------------------- 128 位分数类 --------------------
struct Fraction {
    __int128 num;   // 分子
    __int128 den;   // 分母 (恒正)

    Fraction(__int128 n = 0, __int128 d = 1) : num(n), den(d) {
        if (den < 0) { num = -num; den = -den; }
        reduce();
    }

    // 约分
    void reduce() {
        if (den == 0) throw std::runtime_error("Denominator zero");
        __int128 g = gcd(num < 0 ? -num : num, den);
        num /= g;
        den /= g;
    }

    static __int128 gcd(__int128 a, __int128 b) {
        while (b != 0) { __int128 t = b; b = a % b; a = t; }
        return a < 0 ? -a : a;
    }

    // 比较运算符
    bool operator<(const Fraction& rhs) const {
        return num * rhs.den < rhs.num * den;
    }
    bool operator<=(const Fraction& rhs) const {
        return num * rhs.den <= rhs.num * den;
    }
    bool operator==(const Fraction& rhs) const {
        return num == rhs.num && den == rhs.den;
    }
    bool operator!=(const Fraction& rhs) const { return !(*this == rhs); }
    bool operator>(const Fraction& rhs) const { return rhs < *this; }
    bool operator>=(const Fraction& rhs) const { return rhs <= *this; }

    // 算术运算
    Fraction operator+(const Fraction& rhs) const {
        return Fraction(num * rhs.den + rhs.num * den, den * rhs.den);
    }
    Fraction operator-(const Fraction& rhs) const {
        return Fraction(num * rhs.den - rhs.num * den, den * rhs.den);
    }
    Fraction operator*(const Fraction& rhs) const {
        return Fraction(num * rhs.num, den * rhs.den);
    }
    Fraction operator/(const Fraction& rhs) const {
        return Fraction(num * rhs.den, den * rhs.num);
    }

    // 取整 (floor)
    __int128 floor() const {
        if (num >= 0) return num / den;
        else return (num - den + 1) / den;
    }

    // 小数部分
    Fraction frac() const {
        return *this - Fraction(floor(), 1);
    }

    // 转换为 double (仅用于调试输出，算法中不使用)
    double toDouble() const {
        return (double)num / (double)den;
    }
};

// 幂函数
Fraction pow(int base, int exp) {
    if (exp == 0) return Fraction(1);
    if (exp < 0) return Fraction(1) / pow(base, -exp);
    __int128 result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return Fraction(result);
}

// -------------------- FP16 解析 --------------------
struct FP16Components {
    uint16_t bits;
    __int128 c;          // 有效数字 (整数)
    int q;               // 指数
    bool is_regular;     // f != 0 (包括次正规数)
    bool is_irregular;   // f == 0 且 e != 0 (2的幂)
};

FP16Components f16_to_components(uint16_t bits) {
    int exp = (bits >> 10) & 0x1F;
    int frac = bits & 0x3FF;

    if (exp == 0 && frac == 0) throw std::invalid_argument("+0 excluded");
    if (exp == 31) throw std::invalid_argument("inf/NaN excluded");

    FP16Components comp;
    comp.bits = bits;

    if (exp == 0) { // 次正规数
        comp.c = frac;
        comp.q = -24;
        comp.is_regular = true;
        comp.is_irregular = false;
    } else {        // 常规数
        comp.c = 1024 + frac;
        comp.q = exp - 25;
        comp.is_regular = (frac != 0);
        comp.is_irregular = (frac == 0);
    }
    return comp;
}

// -------------------- 计算 k (完全整数逻辑) --------------------
int compute_k(int q, bool is_regular) {
    int k;
    if(is_regular){
        k = (q * 1233) >> 12;
    }else{
        k = ((q * 1233) - 512) >> 12;
    }
    return k;
}

// -------------------- 算法1：计算 (d, k) --------------------
//std::pair<__int128, int>
void f16_to_decimal(uint16_t bits,int* dec) {
    auto comp = f16_to_components(bits);
    __int128 c = comp.c;
    int q = comp.q;
    bool is_regular = comp.is_regular;
    bool is_irregular = comp.is_irregular;

    int k = compute_k(q, is_regular);

    // v = c * 2^q
    Fraction v = Fraction(c) * pow(2, q);

    // R = v * 10^{-k-1}
    Fraction R = v * pow(10, -k-1);
    __int128 m = R.floor();
    Fraction n = R.frac();

    __int128 ten = 10 * m;

    // 10n = 10 * (R - m) = 10*R - 10*m
    Fraction tenR = v * pow(10, -k);    // v * 10^{-k}
    Fraction ten_n = tenR - Fraction(ten);
    __int128 floor_ten_n = ten_n.floor();
    Fraction delta = ten_n.frac();

    __int128 one;

    // Step 11-21: 根据 δ 确定 one 初值
    if (delta == Fraction(1, 2)) {
        if (floor_ten_n % 2 == 0)
            one = floor_ten_n;
        else
            one = floor_ten_n + 1;
    } else if (delta < Fraction(1, 2)) {
        one = floor_ten_n;
    } else {
        one = floor_ten_n + 1;
    }
    Fraction A = pow(2, q-1) * pow(10, -k-1);
    // Step 22-28: irregular 的特殊处理
    if (is_irregular) {
        Fraction cond1_val = pow(2, q-2) * pow(10, -k);
        Fraction cond2_val = pow(2, q-2) * pow(10, -k-1);

        if (delta > cond1_val) {
            one = floor_ten_n + 1;
        }
        if (cond2_val >= n) {
            one = 0;
        }
    } else {
        // Step 30-35: regular 情况下的最短表示检查
        if (A > n || (A == n && c % 2 == 0)) {
            one = 0;
        } 
    }
    if (A > (Fraction(1) - n) || (A == (Fraction(1) - n) && c % 2 == 0)) {
        one = 10;
    }
    //__int128 d = ten + one;
    //return {d, k};
    dec[0] = m + (one / 10);
    dec[1] = one % 10;
    dec[2] = k;
}

//std::pair<__int128, int> 
void f16_to_decimal_opt(uint16_t bits,int* dec) {
    uint32_t exp = (bits >> 10) & ((1 << 5) - 1);
    uint32_t sig = bits & ((1 << 10) - 1);
    uint32_t sig_bin = sig | (1 << 10);
    int32_t exp_bin = exp - ((1 << 4) - 1) - 10;
    if (exp == 0) [[unlikely]] {
        // if (sig == 0)
        //     return (char*)memcpy(buf, "0.0", 4) + 3;
        exp_bin = 1 - ((1 << 4) - 1) - 10;
        sig_bin = sig;
    }
    static uint32_t pow10_lut[10]={
0xa3d70a3e , // -2
0xcccccccd , // -1
0x80000000 , // 0
0xa0000000 , // 1
0xc8000000 , // 2
0xfa000000 , // 3
0x9c400000 , // 4
0xc3500000 , // 5
0xf4240000 , // 6
0x98968000 , // 7
    };
    const int offset = 4;
    const int BIT = 16;
    bool irregular = (sig == 0) ;
    int k = (exp_bin * 1233 - (irregular?512:0) ) >> 12;
    // exp_bin range : [1-25,31-25] ; k range : [-8,1] ; -k-1 range : [-2,7]

    //get pow10
    uint64_t pow10 = pow10_lut[-k-1 + 2];
    int h = exp_bin + ((k * -1701 + (-1701)) >> 9);
    uint64_t cb = sig_bin << (BIT + 1 + h);// h + 16
    uint64_t all = (cb * pow10) >> (BIT+1+31-BIT);//12+h+16+32<64
    uint64_t half_ulp = (pow10 >> (33 - (BIT + 1 + h))) + ((sig + 1) & 1);
    uint64_t dot_one = all & (((uint64_t)1 << BIT) - 1);
    uint32_t shorter = (all + half_ulp) >> BIT;
    uint32_t up_down = shorter > (uint32_t)((all - (half_ulp >> 0)) >> BIT);
    uint32_t longer = (all * 10 + ((1ULL << (BIT - 1)) - 7) + ((all >> (BIT - 4)) & 15)) >> BIT;
    //d = ((all - half_ulp) >> BIT) < ((all + half_ulp) >> BIT) ? shorter : longer;
    up_down = ((all + half_ulp) >> BIT) > ((all - half_ulp) >> BIT);
    
    dec[0] = shorter;
    dec[1] = up_down ? 0 : longer - shorter * 10;
    if(irregular) [[unlikely]]
    {
        if(exp_bin == 8 - 25){
            dec[1] = 2;
        }
        if(exp_bin == 9 - 25)
        {
            dec[1] = 3;
        }
    }
    dec[2] = k;
}

int main() {
    int error = 0;
    for (uint32_t bits = 0x0001; bits <= 0x7BFF; ++bits) {
        try {
            int dec[3];
            int dec_opt[3];
            f16_to_decimal(bits, dec);
            f16_to_decimal_opt(bits, dec_opt);
            if(dec[0] != dec_opt[0] || dec[1] != dec_opt[1] || dec[2] != dec_opt[2]) {
                //if(0)
                std::cerr << "Mismatch for 0x" << std::hex << std::uppercase << bits << ": "
                          << "dec = (" << dec[0] << ", " << dec[1] << ", " << dec[2] << ") vs "
                          << "dec_opt = (" << dec_opt[0] << ", " << dec_opt[1] << ", " << dec_opt[2] << ")\n";
                error++;
            }
            // std::cout << "0x" << std::hex << std::uppercase << bits << std::dec
            //     << " -> d: " << dec[0] << ", one: " << dec[1] << ", k: " << dec[2] << "\n";
        } catch (const std::invalid_argument&) {
            continue;  // 跳过 +0, inf, NaN
        } catch (const std::exception& e) {
            std::cerr << "Error processing 0x" << std::hex << bits << ": " << e.what() << std::endl;
            return 1;
        }
    }
    std::cout << "Total errors: " << error << std::endl;
}
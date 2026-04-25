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
std::pair<__int128, int> f16_to_decimal(uint16_t bits) {
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
        Fraction A = pow(2, q-1) * pow(10, -k-1);

        if (A > n || (A == n && c % 2 == 0)) {
            one = 0;
        } else if (A > (Fraction(1) - n) || (A == (Fraction(1) - n) && c % 2 == 0)) {
            one = 10;
        }
    }

    __int128 d = ten + one;
    return {d, k};
}

// -------------------- 主程序 --------------------
int main() {
    std::ofstream out("f16_decimal_results.txt");
    if (!out) {
        std::cerr << "Cannot open output file." << std::endl;
        return 1;
    }

    out << "# bits(hex)  d  k\n";

    uint32_t d_min = 99999999;
    uint32_t d_max = 0;
    // 遍历所有正 FP16 数值 (排除 0x0000, 0x7C00..0x7FFF)
    for (uint32_t bits = 0x0001; bits <= 0x7BFF; ++bits) {
        try {
            auto [d, k] = f16_to_decimal(static_cast<uint16_t>(bits));
            d_min = d > d_min ? d_min : d;
            d_max = d > d_max ? d : d_max;
            out << "0x" << std::hex << std::uppercase << bits << std::dec
                << " " << (int64_t)d << " " << k << "\n";
        } catch (const std::invalid_argument&) {
            // 跳过 +0, inf, NaN
            continue;
        } catch (const std::exception& e) {
            std::cerr << "Error processing 0x" << std::hex << bits << ": " << e.what() << std::endl;
            return 1;
        }
    }

    out.close();
    std::cout << "Results written to f16_decimal_results.txt" << std::endl;
    printf("d_min = %u, d_max = %u\n",d_min,d_max);
    return 0;
}
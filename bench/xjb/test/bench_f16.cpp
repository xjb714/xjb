#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

// ==================== 128位分数类（同前） ====================
struct Fraction {
    __int128 num, den;

    Fraction(__int128 n = 0, __int128 d = 1) : num(n), den(d) {
        if (den < 0) {
            num = -num;
            den = -den;
        }
        reduce();
    }

    void reduce() {
        if (den == 0)
            throw std::runtime_error("Denominator zero");
        __int128 g = gcd(num < 0 ? -num : num, den);
        num /= g;
        den /= g;
    }

    static __int128 gcd(__int128 a, __int128 b) {
        while (b) {
            __int128 t = b;
            b = a % b;
            a = t;
        }
        return a < 0 ? -a : a;
    }

    bool operator<(const Fraction& rhs) const { return num * rhs.den < rhs.num * den; }
    bool operator<=(const Fraction& rhs) const { return num * rhs.den <= rhs.num * den; }
    bool operator==(const Fraction& rhs) const { return num == rhs.num && den == rhs.den; }
    bool operator!=(const Fraction& rhs) const { return !(*this == rhs); }
    bool operator>(const Fraction& rhs) const { return rhs < *this; }
    bool operator>=(const Fraction& rhs) const { return rhs <= *this; }

    Fraction operator+(const Fraction& rhs) const { return Fraction(num * rhs.den + rhs.num * den, den * rhs.den); }
    Fraction operator-(const Fraction& rhs) const { return Fraction(num * rhs.den - rhs.num * den, den * rhs.den); }
    Fraction operator*(const Fraction& rhs) const { return Fraction(num * rhs.num, den * rhs.den); }
    Fraction operator/(const Fraction& rhs) const { return Fraction(num * rhs.den, den * rhs.num); }

    __int128 floor() const {
        if (num >= 0)
            return num / den;
        else
            return (num - den + 1) / den;
    }

    Fraction frac() const { return *this - Fraction(floor(), 1); }
};

Fraction pow(int base, int exp) {
    if (exp == 0)
        return Fraction(1);
    if (exp < 0)
        return Fraction(1) / pow(base, -exp);
    __int128 result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return Fraction(result);
}

// ==================== FP16 解析（同前） ====================
struct FP16Components {
    uint16_t bits;
    __int128 c;
    int q;
    bool is_regular;
    bool is_irregular;
};

FP16Components f16_to_components(uint16_t bits) {
    int exp = (bits >> 10) & 0x1F;
    int frac = bits & 0x3FF;

    FP16Components comp;
    comp.bits = bits;
    if (exp == 0) {
        comp.c = frac;
        comp.q = -24;
        comp.is_regular = true;
        comp.is_irregular = false;
    } else {
        comp.c = 1024 + frac;
        comp.q = exp - 25;
        comp.is_regular = (frac != 0);
        comp.is_irregular = (frac == 0);
    }
    return comp;
}

int compute_k(int q, bool is_regular) {
    int k;
    if (is_regular) {
        k = (q * 1233) >> 12;
    } else {
        k = ((q * 1233) - 512) >> 12;
    }
    return k;
}

std::pair<uint32_t, int> f16_to_decimal(uint16_t bits) {
    auto comp = f16_to_components(bits);
    __int128 c = comp.c;
    int q = comp.q;
    bool is_regular = comp.is_regular;
    bool is_irregular = comp.is_irregular;

    int k = compute_k(q, is_regular);
    Fraction v = Fraction(c) * pow(2, q);
    Fraction R = v * pow(10, -k - 1);
    __int128 m = R.floor();
    Fraction n = R.frac();

    __int128 ten = 10 * m;
    Fraction tenR = v * pow(10, -k);
    Fraction ten_n = tenR - Fraction(ten);
    __int128 floor_ten_n = ten_n.floor();
    Fraction delta = ten_n.frac();

    __int128 one;
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
    Fraction A = pow(2, q - 1) * pow(10, -k - 1);
    if (is_irregular) {
        Fraction cond1 = pow(2, q - 2) * pow(10, -k);
        Fraction cond2 = pow(2, q - 2) * pow(10, -k - 1);
        if (delta > cond1)
            one = floor_ten_n + 1;
        if (cond2 >= n)
            one = 0;
    } else {
        if (A > n || (A == n && c % 2 == 0))
            one = 0;
    }
    if (A > (Fraction(1) - n) || (A == (Fraction(1) - n) && c % 2 == 0))
        one = 10;
    __int128 d = ten + one;
    return {static_cast<uint32_t>(d), k};
}

// ==================== BCD 辅助 ====================
static inline uint32_t to_bcd4(uint32_t d) {
    // convert binary to little_endian bcd;
    // require d range [0,9999];
    uint64_t abcd = d;
    uint64_t ab_cd = (abcd << 16) + (1 - (100 << 16)) * (((abcd * 0x147b) >> 19));
    uint64_t a_b_c_d = (ab_cd << 8) + (1 - (10 << 8)) * (((ab_cd * 0x67) >> 10) & 0x000f000f);
    return static_cast<uint32_t>(a_b_c_d);
}

// ==================== xjb16 核心转换函数 ====================
char* xjb16(uint16_t bits, char* buf) {
    // same result as str(numpy.float16(v)) in python.
    // v has 16bits binary representation = bits

    // 16 = 1 + 5 + 10; sign + exp + sig
    *buf = '-';
    buf += bits >> (10 + 5);
    uint32_t exp = (bits >> 10) & ((1 << 5) - 1);
    uint32_t sig = bits & ((1 << 10) - 1);
    uint32_t sig_bin = sig | (1 << 10);
    int32_t exp_bin = exp - ((1 << 4) - 1) - 10;
    if (exp == 0) [[unlikely]] {
        if (sig == 0)
            return (char*)memcpy(buf, "0.0", 4) + 3;
        exp_bin = 1 - ((1 << 4) - 1) - 10;
        sig_bin = sig;
    }
    if (exp == 31) [[unlikely]]
        return (char*)memcpy(buf, sig ? "nan" : "inf", 4) + 3;

    uint16_t bits_abs = bits & ((1 << 15) - 1);
    auto [d, k] = f16_to_decimal(bits_abs);
    int tz = -1;
    int mul = 1;
    int d_div = d;
    while (d_div * mul == d) {
        d_div /= 10;
        mul *= 10;
        tz++;
    }

    // len range = [1, 5];
    // d range = [6, 19990];
    // f16 min = 6e-8; max is 65500
    uint32_t len = 1 + (d >= 10) + (d >= 100) + (d >= 1000) + (d >= 10000);
    memcpy(buf, "00000000", 8);
    uint32_t d_rem = (d >= 10000) ? (d - 10000) : d;

    int len_2 = len;
    while (len_2 < 4) {
        len_2++;
        d_rem *= 10;
    }

    uint32_t bcd = to_bcd4(d_rem);
    uint32_t ascii = bcd | 0x30303030;
    uint32_t dec_sig_len = len - tz;  // [1, 5];

    const int FIXED_MIN = -4;
    const int FIXED_MAX = 2;

    int e10 = k + len - 1;
    uint32_t first_sig_pos = (FIXED_MIN <= e10 && e10 <= -1) ? 1 - e10 : 0;
    uint32_t dot_pos = (0 <= e10 && e10 <= FIXED_MAX) ? 1 + e10 : 1;
    uint32_t move_pos = dot_pos + ((0 <= e10 || e10 < FIXED_MIN));

    uint32_t exp_pos = (FIXED_MIN <= e10 && e10 <= -1)
                           ? dec_sig_len
                           : (0 <= e10 && e10 <= FIXED_MAX ? (e10 + 3 > dec_sig_len + 1 ? e10 + 3 : dec_sig_len + 1)
                                                           : (dec_sig_len + 1 - (dec_sig_len == 1)));
    char* buf_origin = buf;

    buf += first_sig_pos;
    *buf = '1';
    memcpy(buf + (d >= 10000), &ascii, 4);

    memmove(&buf[move_pos], &buf[dot_pos], 8);  // the index (first_sig_pos + dot_pos + sign) max = 7+1=8,
    buf_origin[dot_pos] = '.';
    buf += exp_pos;

    // comput exp ascii;
    // only negative value print exp_result;
    int e10_neg = e10 < 0;
    uint32_t e10_abs = e10_neg ? -e10 : e10;
    uint32_t exp_result =
        'e' + ((e10_neg ? (uint32_t)'-' : (uint32_t)'+') << 8) + ((uint32_t)'0' << 16) + ((e10_abs + '0') << 24);
    if (FIXED_MIN <= e10 && e10 <= FIXED_MAX)
        exp_result = 0;
    uint32_t exp_len = (FIXED_MIN <= e10 && e10 <= FIXED_MAX) ? 0 : 4;
    memcpy(buf, &exp_result, 4);
    return buf + exp_len;
}

// ==================== 性能测试主程序 ====================
int main() {
    // 生成 N = 2^20 个随机 FP16 值，排除 inf 和 nan
    constexpr size_t N = 1 << 20;
    std::vector<uint16_t> values;
    values.reserve(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    while (values.size() < N) {
        uint16_t bits = dist(gen);
        uint16_t exp = (bits >> 10) & 0x1F;
        if (exp != 31) {          // 排除 exp == 31 (inf/nan)
            values.push_back(bits);
        }
    }

    // 缓冲区复用，避免多次分配栈内存影响测量（测量的是转换逻辑本身）
    char buffer[32];   // 足够大，xjb16 最多写十几个字符

    // 预热：调用一次确保缓存和分支预测稳定
    (void)xjb16(values[0], buffer);

    // 开始计时
    auto start = std::chrono::high_resolution_clock::now();

    // 执行 N 次转换，并将结果通过 volatile 防止优化
    volatile char sink = 0;
    for (uint16_t bits : values) {
        char* end = xjb16(bits, buffer);
        // 强制编译器认为结果被使用（防止循环被完全优化掉）
        sink += buffer[0] + (end - buffer);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double avg_ns = (duration_us * 1000.0) / N;

    // 输出结果
    std::cout << "Total operations: " << N << "\n";
    std::cout << "Total time: " << duration_us << " us\n";
    std::cout << "Average time per call: " << avg_ns << " ns\n";

    return 0;
}
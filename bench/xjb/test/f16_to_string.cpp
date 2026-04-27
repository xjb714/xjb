#include <string.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
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
    if (exp == 0 && frac == 0)
        throw std::invalid_argument("+0 excluded");
    if (exp == 31)
        throw std::invalid_argument("inf/NaN excluded");

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
    return {d, k};
}

// ==================== 字符串转换 ====================

// 将 __int128 转换为十进制字符串
// std::string int128_to_string(__int128 x) {
//     if (x == 0) return "0";
//     bool neg = (x < 0);
//     if (neg) x = -x;
//     std::string s;
//     while (x > 0) {
//         s.push_back('0' + (char)(x % 10));
//         x /= 10;
//     }
//     if (neg) s.push_back('-');
//     std::reverse(s.begin(), s.end());
//     return s;
// }

// // 计算十进制位数
// int len10(__int128 x) {
//     if (x == 0) return 0;
//     return int128_to_string(x).length();
// }

// 将 (d, k) 转换为最简字符串（固定点/科学记数法）
// std::string decimal_to_string(uint32_t d, int k) {

//     //len range = [1, 5];
//     //d range = [6, 19990];
//     uint32_t len = 1 + (d>=10) + (d>=100) + (d>=1000) + (d>=10000);
//     uint32_t sig_len = 5;

//     // 去掉尾随零，同时调整指数
//     // __int128 sig = d;
//     // int tz = 0;
//     // while (sig > 0 && sig % 10 == 0) {
//     //     sig /= 10;
//     //     ++tz;
//     // }
//     // int e10 = k + tz;                     // 实际指数
//     // std::string sig_str = int128_to_string(sig);
//     // int len = (int)sig_str.length();

//     // // 固定点范围（可调整，这里采用类似于 float 的 -3 ~ 5）
//     // const int FIXED_MIN = -3;
//     // const int FIXED_MAX = 5;

//     // if (e10 >= FIXED_MIN && e10 <= FIXED_MAX) {
//     //     // 固定点表示
//     //     if (e10 >= 0) {
//     //         // 整数部分：sig 后加 e10 个零
//     //         std::string result = sig_str;
//     //         result.append(e10, '0');
//     //         return result;
//     //     } else {
//     //         // 小数：需要在前面补零或插入小数点
//     //         int abs_e10 = -e10;
//     //         if (abs_e10 >= len) {
//     //             // 0.000...xxx
//     //             std::string result = "0.";
//     //             result.append(abs_e10 - len, '0');
//     //             result += sig_str;
//     //             return result;
//     //         } else {
//     //             // 在 len + e10 位置插入小数点 (e10 < 0)
//     //             int dot_pos = len + e10;
//     //             std::string result = sig_str.substr(0, dot_pos);
//     //             result += ".";
//     //             result += sig_str.substr(dot_pos);
//     //             return result;
//     //         }
//     //     }
//     // } else {
//     //     // 科学记数法：d.ddd...e±EE
//     //     // 将有效数字调整为 [1, 10) 范围
//     //     int new_exp = e10 + (len - 1);   // 对应 sig / 10^{len-1} * 10^{new_exp}
//     //     std::string result;
//     //     result += sig_str[0];            // 整数部分
//     //     if (len > 1) {
//     //         result += ".";
//     //         result += sig_str.substr(1); // 小数部分
//     //     }
//     //     result += "e";
//     //     if (new_exp >= 0) result += "+";
//     //     result += std::to_string(new_exp);
//     //     return result;
//     // }
// }

// 完整的 FP16 -> 字符串接口
// std::string fp16_to_string(uint16_t bits) {
//     auto [d, k] = f16_to_decimal(bits);
//     return decimal_to_string(d, k);
// }

static inline uint32_t to_bcd4(uint32_t d) {
    // convert binary to little_endian bcd;
    // require d range [0,9999];
    uint64_t abcd = d;
    uint64_t ab_cd = (abcd << 16) + (1 - (100 << 16)) * (((abcd * 0x147b) >> 19));
    uint64_t a_b_c_d = (ab_cd << 8) + (1 - (10 << 8)) * (((ab_cd * 0x67) >> 10) & 0x000f000f);
    return a_b_c_d;
}

char* xjb16(uint16_t bits, char* buf) {
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

// ==================== 主程序 ====================
int main() {
    std::ofstream out("f16_string_results.txt");
    if (!out) {
        std::cerr << "Cannot open output file." << std::endl;
        return 1;
    }

    uint16_t bits = 2048;
    auto [d, k] = f16_to_decimal(bits);
    std::cout << "d: " << d << ", k: " << k << std::endl;
    bits = 5120;
    auto [d2, k2] = f16_to_decimal(bits);
    std::cout << "d: " << d2 << ", k: " << k2 << std::endl;

    static char buf[16];
    // 输出格式：16进制位 -> 字符串
    for (uint32_t bits = 0x0000; bits <= (0x7FFF); ++bits) {
        try {
            // auto [d, k] = f16_to_decimal(bits);

            memset(buf, 0, 16);
            char* buf_end = xjb16(bits, buf);
            std::string str = std::string(buf, buf_end);
            // out << bits << " : 0x" << std::hex << std::uppercase << bits << std::dec << " " << d << " " << k << " "
            //     << str << "\n";
            out << bits << " " << str << "\n";
            // std::string str = fp16_to_string(static_cast<uint16_t>(bits));
            // out << "0x" << std::hex << std::uppercase << bits << std::dec
            //     << " -> " << str << "\n";
        } catch (const std::invalid_argument&) {
            continue;  // 跳过 +0, inf, NaN
        } catch (const std::exception& e) {
            std::cerr << "Error processing 0x" << std::hex << bits << ": " << e.what() << std::endl;
            return 1;
        }
    }

    out.close();
    std::cout << "Results written to f16_string_results.txt" << std::endl;
    return 0;
}

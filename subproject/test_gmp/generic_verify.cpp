#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <algorithm>   // for std::max / std::min

#include <omp.h>       // OpenMP

#include <gmpxx.h>

// ----------------------------------------------------------------------
// Integer power utility (base^exp) for mpz_class, exp >= 0
// ----------------------------------------------------------------------
mpz_class ipow(const mpz_class& base, unsigned long exp) {
    mpz_class result;
    mpz_pow_ui(result.get_mpz_t(), base.get_mpz_t(), exp);
    return result;
}

// ----------------------------------------------------------------------
// Rational power utility: base^exp, exp can be negative
// ----------------------------------------------------------------------
mpq_class pow_rational(const mpz_class& base, long long exp) {
    if (exp >= 0) {
        return mpq_class(ipow(base, static_cast<unsigned long>(exp)), 1);
    } else {
        return mpq_class(1, ipow(base, static_cast<unsigned long>(-exp)));
    }
}

// ----------------------------------------------------------------------
// Best rational approximation below and above b/c with denominator <= a
// ----------------------------------------------------------------------
std::pair<std::pair<mpz_class, mpz_class>, std::pair<mpz_class, mpz_class>>
find_best_rational_approximation_below_and_above(const mpz_class& a,
                                                 const mpz_class& b,
                                                 const mpz_class& c) {
    // if (c <= a)
    //     throw std::invalid_argument("c must be > a");

    mpz_class low_num(0), low_den(1);
    mpz_class high_num(1), high_den(0);
    mpz_class best_below_num(0), best_below_den(1);
    mpz_class best_above_num(1), best_above_den(0);

    auto compare = [](const mpz_class& n1, const mpz_class& d1,
                      const mpz_class& n2, const mpz_class& d2) -> int {
        mpz_class left  = n1 * d2;
        mpz_class right = n2 * d1;
        if (left < right) return -1;
        if (left > right) return  1;
        return 0;
    };

    while (true) {
        mpz_class med_num = low_num + high_num;
        mpz_class med_den = low_den + high_den;
        if (med_den > a) break;

        int cmp = compare(med_num, med_den, b, c);
        if (cmp <= 0) {
            // med <= target
            mpz_class current_diff = b * med_den - c * med_num;
            mpz_class best_diff    = b * best_below_den - c * best_below_num;
            if (current_diff < best_diff ||
                (current_diff == best_diff && med_den > best_below_den)) {
                best_below_num = med_num;
                best_below_den = med_den;
            }
            low_num = med_num;
            low_den = med_den;
        } else {
            // med > target
            mpz_class current_diff = c * med_num - b * med_den;
            mpz_class best_diff    = c * best_above_num - b * best_above_den;
            if (current_diff < best_diff ||
                (current_diff == best_diff && med_den > best_above_den)) {
                best_above_num = med_num;
                best_above_den = med_den;
            }
            high_num = med_num;
            high_den = med_den;
        }
    }
    return {{best_below_num, best_below_den}, {best_above_num, best_above_den}};
}

// ----------------------------------------------------------------------
// find_min_BIT: smallest Q>=1 with (c/d)*(1+2^{-Q}) < a/b
// ----------------------------------------------------------------------
int find_min_BIT(const mpz_class& a, const mpz_class& b,
                 const mpz_class& c, const mpz_class& d) {
    mpz_class num = b * c;
    mpz_class den = a * d - b * c;
    // if (den <= 0) {
    //     return std::numeric_limits<int>::max();
    // }
    // if (num == 0) {
    //     return 1;
    // }
    mpz_class n = num / den;               // floor(num/den)
    int bits = mpz_sizeinbase(n.get_mpz_t(), 2);  // bit length
    return std::max(1, bits);
}

// ----------------------------------------------------------------------
// generic_float_to_decimal: Algorithm 1, returns (d, k)
// ----------------------------------------------------------------------
std::pair<mpz_class, long long>
generic_float_to_decimal(const std::string& float_type,
                         const mpz_class& float_bits,
                         const std::map<std::string, std::tuple<int,int,int,int>>& types) {
    auto [sign_bits, exp_bits, sig_bits, explicit_leading_bit] = types.at(float_type);

    mpz_class sig = float_bits & ((mpz_class(1) << sig_bits) - 1);
    mpz_class exp = (float_bits >> sig_bits) & ((mpz_class(1) << exp_bits) - 1);

    mpz_class c;
    long long q;
    bool is_regular, is_irregular;

    if (exp == 0) {
        // subnormal
        c = sig;
        q = 1 - ((1LL << (exp_bits - 1)) - 1) + 1 - sig_bits;
        is_regular = true;
        is_irregular = false;
    } else {
        // normal
        if (explicit_leading_bit == 0)
            c = (mpz_class(1) << sig_bits) + sig;
        else
            c = sig;
        q = static_cast<long long>(exp.get_si()) - ((1LL << (exp_bits - 1)) - 1) - sig_bits;
        if (sig == 0) {
            is_regular = false;
            is_irregular = true;
        } else {
            is_regular = true;
            is_irregular = false;
        }
    }

    // Step 2-5: compute k
    long long k;
    double log10_2 = std::log10(2.0);
    if (is_regular) {
        k = static_cast<long long>(std::floor(q * log10_2));
    } else {
        k = static_cast<long long>(std::floor(q * log10_2 - std::log10(4.0/3.0)));
    }

    // v = c * 2^q
    mpq_class v = mpq_class(c) * pow_rational(2, q);

    // R = v * 10^{-k-1}
    mpq_class R = v * pow_rational(10, -k - 1);
    mpz_class m = R.get_num() / R.get_den();   // floor(R)
    mpq_class n = R - mpq_class(m);            // fractional part

    // ten = 10 * m
    mpz_class ten = 10 * m;

    // tenR = v * 10^{-k}
    mpq_class tenR = v * pow_rational(10, -k);
    mpq_class ten_n = tenR - mpq_class(ten);
    mpz_class floor_ten_n = ten_n.get_num() / ten_n.get_den();
    mpq_class delta = ten_n - mpq_class(floor_ten_n);

    // Step 11-21: determine one
    mpz_class one;
    if (delta == mpq_class(1, 2)) {
        if (floor_ten_n % 2 == 0)
            one = floor_ten_n;
        else
            one = floor_ten_n + 1;
    } else if (delta < mpq_class(1, 2)) {
        one = floor_ten_n;
    } else {
        one = floor_ten_n + 1;
    }

    // A = 2^{q-1} * 10^{-k-1}
    mpq_class A = pow_rational(2, q - 1) * pow_rational(10, -k - 1);

    if (is_irregular) {
        mpq_class cond1_val = pow_rational(2, q - 2) * pow_rational(10, -k);
        mpq_class cond2_val = pow_rational(2, q - 2) * pow_rational(10, -k - 1);
        if (delta > cond1_val)
            one = floor_ten_n + 1;
        if (cond2_val >= n)
            one = 0;
    } else {
        if (A > n || (A == n && c % 2 == 0))
            one = 0;
    }
    if (A > (mpq_class(1) - n) || (A == (mpq_class(1) - n) && c % 2 == 0))
        one = 10;

    // d = ten + one
    mpz_class d = ten + one;
    return {d, k};
}

// ----------------------------------------------------------------------
// Relative error function (a/b - c/d) / (c/d)
// ----------------------------------------------------------------------
mpq_class r(const mpz_class& a, const mpz_class& b,
            const mpz_class& c, const mpz_class& d) {
    mpz_class num = a * d - b * c;
    mpz_class den = b * c;
    return mpq_class(num, den);
}

// ----------------------------------------------------------------------
// Helper: format seconds into human-readable string (e.g., "2h 3m 15s")
// ----------------------------------------------------------------------
std::string format_time(double seconds) {
    if (seconds < 0.0) return "0s";
    int tot = static_cast<int>(seconds);
    int hrs = tot / 3600;
    int mins = (tot % 3600) / 60;
    int secs = tot % 60;
    std::string out;
    if (hrs > 0) out += std::to_string(hrs) + "h ";
    if (mins > 0 || hrs > 0) out += std::to_string(mins) + "m ";
    out += std::to_string(secs) + "s";
    return out;
}

// ----------------------------------------------------------------------
// find_min_BIT_calc_m_impl: core loop over q (OpenMP with progress)
// ----------------------------------------------------------------------
std::pair<double, int>
find_min_BIT_calc_m_impl(int sign_bits, int exp_bits, int sig_bits,
                         int explicit_leading_bit) {
    (void) sign_bits; // not used inside

    long long exp_bias = (1LL << (exp_bits - 1)) - 1;
    long long min_q = 1 - exp_bias - sig_bits;
    long long max_q = exp_bias - sig_bits + 1;

    mpz_class C = (explicit_leading_bit == 0)
                     ? (mpz_class(1) << (sig_bits + 1)) - 1
                     : (mpz_class(1) << sig_bits) - 1;

    double minr_d = std::numeric_limits<double>::max();
    int MAX_BIT = 1;
    const double log10_2 = std::log10(2.0);

    long long total_iters = max_q - min_q;
    long long progress = 0;
    double start_time = omp_get_wtime();   // wall-clock start time

    // OpenMP parallel for with reductions and dynamic scheduling
    #pragma omp parallel for reduction(max:MAX_BIT) reduction(min:minr_d) //num_threads(9) //schedule(dynamic) 
    for (long long q = min_q; q < max_q; ++q) {
        long long k = static_cast<long long>(std::floor(q * log10_2));
        long long A = q - k - 1;
        long long B = k + 1;

        mpz_class P(1), Q(1);
        if (A >= 0) P *= ipow(2, static_cast<unsigned long>(A));
        else        Q *= ipow(2, static_cast<unsigned long>(-A));
        if (B >= 0) Q *= ipow(5, static_cast<unsigned long>(B));
        else        P *= ipow(5, static_cast<unsigned long>(-B));

        mpz_class UP_num, UP_den;
        if (Q <= C) {
            UP_num = P * (C * Q + 1);
            UP_den = C * Q * Q;
        } else {
            auto [below, above] = find_best_rational_approximation_below_and_above(C, P, Q);
            UP_num = above.first;
            UP_den = above.second;
        }

        mpz_class diff_num = UP_num * Q - UP_den * P;
        mpz_class diff_den = UP_den * P;
        double maxr_d = mpq_class(diff_num, diff_den).get_d();

        int BIT = find_min_BIT(UP_num, UP_den, P, Q);

        // Update reduction variables (required by OpenMP)
        minr_d = std::min(minr_d, maxr_d);
        MAX_BIT = std::max(MAX_BIT, BIT);

        // Progress counter
        #pragma omp atomic
        ++progress;

        // Periodically print progress with elapsed/remaining time
        #pragma omp critical (progress_print)
        {
            if (progress % (total_iters / 100 + 1) == 0 || progress == total_iters) {
                double elapsed = omp_get_wtime() - start_time;
                double remaining = 0.0;
                if (progress > 0) {
                    remaining = elapsed * (static_cast<double>(total_iters - progress) / progress);
                }
                std::cout << "\rProgress: " << progress << "/" << total_iters << " ("
                          << (progress * 100 / total_iters) << "%)"
                          << " | Elapsed: " << format_time(elapsed)
                          << " | Remaining: " << format_time(remaining)
                          << std::flush;
            }
        }
    }
    std::cout << std::endl; // finish progress line

    return {minr_d, MAX_BIT};
}

// ----------------------------------------------------------------------
// Wrappers for verification output
// ----------------------------------------------------------------------
void find_min_BIT_calc_m(const std::string& float_type,
                         const std::tuple<int,int,int,int>& params) {
    auto [sign_bits, exp_bits, sig_bits, explicit_leading_bit] = params;
    auto [minr_d, max_bit] = find_min_BIT_calc_m_impl(sign_bits, exp_bits, sig_bits, explicit_leading_bit);
    static int test_count = 0;
    ++test_count;
    std::cout << test_count << ": " << float_type << ": minr=" << minr_d
              << ", log2(minr)=" << std::log2(minr_d)
              << ", max_bit=" << max_bit << std::endl;
}

void generic_verify_float_to_string(const std::string& float_type,
                                    const std::tuple<int,int,int,int>& params) {
    find_min_BIT_calc_m(float_type, params);
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------
int main() {
    std::vector<std::pair<std::string, std::tuple<int,int,int,int>>> types = {
        {"float8_e5m2", {1, 5, 2, 0}}, // 6
        {"float8_e4m3", {1, 4, 3, 0}}, // 8
        {"bfloat16",    {1, 8, 7, 0}}, // 19
        {"float16",     {1, 5, 10, 0}},// 22
        {"float32",     {1, 8, 23, 0}},// 52
        {"tfloat32",    {1, 8, 10, 0}},// 27
        {"float64",     {1, 11, 52, 0}},// 113
        {"float80",     {1, 15, 64, 1}},// 141
        {"float128",    {1, 15, 112, 0}},// 237
        // {"float256",    {1, 19, 236, 0}},// 494
        // {"float512",    {1, 18, 493, 0}}// 1001
    };

    for (const auto& type : types) {
        generic_verify_float_to_string(type.first, type.second);
    }

    return 0;
}
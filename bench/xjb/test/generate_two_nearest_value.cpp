// author: xjb
// only for regular value

// double value v = c * 2**q ; c != 2**52
// the vaild range is : (1) q in [-75,-2]  (2) c % 2**(k-q) == 2**(k-q-1)
// notice : k = math.floor(q*math.log10(2))

#include <cstdint>
#include <cstdio>
#include <random>
#include <charconv>
#include <iostream>
std::random_device rd;
std::mt19937_64 gen(rd());
double recover_v_from_c_q(uint64_t c, int q)
{
    // recovery v from c,q
    uint64_t ieee_exp = q + 1075;
    uint64_t ieee_sig = c & ((1ULL << 52) - 1);
    uint64_t v = ((ieee_exp << 52) | ieee_sig);
    return *(double *)&v;
}
void two_nearest_value()
{
    for (int q = -75; q <= -2; q++)
    {
        int k = (q * 78913) >> 18;
        int bit = k - q; // range : [1,52]
        // the last (k - q) bits of c is 2**(k-q-1)
        // example : q = -3 , k = -1, bit = 2
        // c has 53 bit
        // 1 | ........ | 10
        // a | b | d
        // three part : a (1 bits) = 1,
        //              b (52 - bit bits) = [0,2**(52-bit)]
        //              d (bit bits) = 2**(k-q-1)
        // 1+ 52-bit + bit = 53 bits
        for (uint64_t b = 0; b < 1ULL << (52 - bit); b++)
        {
            uint64_t c = (1ULL << 52) + (b << bit) + (1ULL << (bit - 1));
            double v = recover_v_from_c_q(c, q);
            uint64_t vi = *(uint64_t *)&v;
            printf("q=%d,c=%llu,v=%llx,v=%.17g\n", q, c, vi, v);
            // return ;
        }
    }
}
double generate_rand_two_nearest()
{
    int rand_q = gen() % 74 - 75; // range : [-75,-2]
    int k = (rand_q * 78913) >> 18;
    int bit = k - rand_q;
    uint64_t rand_b = gen() & ((1ULL << (52 - bit)) - 1);
    uint64_t rand_c = (1ULL << 52) + (rand_b << bit) + (1ULL << (bit - 1));
    double v = recover_v_from_c_q(rand_c, rand_q);
    return v;
}
int main()
{
    // two_nearest_value(); //find all value
    for (int i = 0; i < 20; ++i)
    {
        double v = generate_rand_two_nearest();
        printf("%.16le %.17g\n", v, v);
    }
}
#include "../../bench/ryu/generic_128.c"

#include <random>

typedef uint64_t u64;
__uint128_t random_u128() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<u64> dis(0, UINT64_MAX);
    return ((__uint128_t)dis(gen) << 64) | dis(gen);
}
void u128_to_string(__uint128_t value, char* buffer) {
    char temp[40]; // 128-bit integer can have at most 39 digits
    int index = 0;
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    while (value > 0) {
        uint8_t digit = value % 10;
        temp[index++] = '0' + digit;
        value /= 10;
    }
    // Reverse the string
    for (int i = 0; i < index; ++i) {
        buffer[i] = temp[index - 1 - i];
    }
    buffer[index] = '\0';
}
int main() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    for (int i = 0; i < 100; ++i) {
        auto rand_u128 = random_u128();
        auto decimal = generic_binary_to_decimal(rand_u128, 64, 15, true);
        char buf[64];
        int len = generic_to_chars(decimal, buf);
        buf[len] = 0;
        // For code audit.
        if (len <= 0 || len >= 54)
            throw "len is not in range (0, 54)";
        static char dec_buf[64];
        u128_to_string(decimal.mantissa, dec_buf);
        printf("%d : %s = %s * 10**(%d)\n", i, buf, dec_buf , decimal.exponent);
    }
    // todo
    return 0;
}
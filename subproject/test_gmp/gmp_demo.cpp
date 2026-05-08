#include <stdio.h>
#include <gmp.h>

int main() {
    // 声明大整数变量
    mpz_t a, b, sum, product, fact, power;

    // 初始化变量（分配内存）
    mpz_init(a);
    mpz_init(b);
    mpz_init(sum);
    mpz_init(product);
    mpz_init(fact);
    mpz_init(power);

    // 1. 赋值与基本运算
    mpz_set_ui(a, 123456789);          // a = 123456789
    mpz_set_str(b, "9876543210987654321", 10); // b = 9876543210987654321（十进制）

    // 加法
    mpz_add(sum, a, b);                // sum = a + b

    // 乘法
    mpz_mul(product, a, b);            // product = a * b

    // 2. 计算大数阶乘（200!）
    mpz_fac_ui(fact, 200);             // fact = 200!

    // 3. 幂运算：2^1000
    mpz_pow_ui(power, a, 1000);        // power = a^1000（这里 a 同上，仅为演示）

    // 输出结果
    gmp_printf("a = %Zd\n", a);
    gmp_printf("b = %Zd\n", b);
    gmp_printf("a + b = %Zd\n", sum);
    gmp_printf("a * b = %Zd\n", product);
    gmp_printf("200! = %Zd\n", fact);
    gmp_printf("123456789^1000 的前 100 位: %.100Zd\n", power);  // 只打印前100位，避免过长

    // 4. 清理内存
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(sum);
    mpz_clear(product);
    mpz_clear(fact);
    mpz_clear(power);

    return 0;
}
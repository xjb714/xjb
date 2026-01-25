#ifndef U128_MUL_H
#define U128_MUL_H

#include <stdint.h>
#include <inttypes.h>

typedef uint64_t u64;
typedef uint32_t u32;

// 定义一个128位无符号整数结构，用于跨平台兼容
typedef struct {
    u64 low;  // 低64位
    u64 high; // 高64位
} u128_struct;

// 计算 (a << 64 | b) * c，返回结果的高128位
// 结果存储在输出参数result中，result->high是高64位，result->low是低64位
static inline void u128_mul_u64(u64 a, u64 b, u64 c, u128_struct* result) {
    // 确保结果指针不为空
    if (result == NULL) {
        return;
    }
    
    // 初始化结果为0
    result->low = 0;
    result->high = 0;
    
#if defined(__GNUC__) || defined(__clang__)
    // GCC/Clang 支持 __uint128_t
    __uint128_t a128 = ((__uint128_t)a << 64) | b;
    __uint128_t c128 = c;
    
    // 计算完整的乘积 (128位 × 64位 = 192位)
    // 我们只需要高128位，即从第64位开始的部分
    
    // 分解为两部分计算：
    // a128 * c = (a_high * c) << 64 + (a_low * c)
    // 其中 a_high = a, a_low = b
    
    __uint128_t a_high_c = (__uint128_t)a * c;
    __uint128_t a_low_c = (__uint128_t)b * c;
    
    // a_low_c 是 128位，分解为 low_part 和 high_part
    u64 a_low_c_high = (u64)(a_low_c >> 64);
    
    // 高128位结果 = a_high_c + a_low_c_high
    __uint128_t high_result = a_high_c + a_low_c_high;
    
    // 存储结果
    result->low = (u64)high_result;
    result->high = (u64)(high_result >> 64);
#elif defined(_MSC_VER)
    // MSVC 不支持 __uint128_t，但提供了内在函数
    u64 b_c_low, b_c_high;    // b * c = b_c_low + (b_c_high << 64)
    u64 a_c_low, a_c_high;    // a * c = a_c_low + (a_c_high << 64)
    
    // 使用MSVC内在函数计算64位×64位=128位乘法
    b_c_low = _mul128(b, c, &b_c_high);
    a_c_low = _mul128(a, c, &a_c_high);
    
    // 高128位结果 = (a * c) + (b * c >> 64)
    // = (a_c_high << 64 | a_c_low) + b_c_high
    
    u64 carry = 0;
    u64 low_part = a_c_low + b_c_high;
    if (low_part < a_c_low) {
        carry = 1;
    }
    
    u64 high_part = a_c_high + carry;
    
    result->low = low_part;
    result->high = high_part;
#else
    // 其他编译器，手动实现64位×64位=128位乘法
    // 将64位操作数分解为32位部分
    u32 a_lo = (u32)a;
    u32 a_hi = (u32)(a >> 32);
    u32 b_lo = (u32)b;
    u32 b_hi = (u32)(b >> 32);
    u32 c_lo = (u32)c;
    u32 c_hi = (u32)(c >> 32);
    
    // 计算 b * c = b_lo*c_lo + (b_lo*c_hi + b_hi*c_lo) << 32 + (b_hi*c_hi) << 64
    u64 b_lo_c_lo = (u64)b_lo * c_lo;
    u64 b_lo_c_hi = (u64)b_lo * c_hi;
    u64 b_hi_c_lo = (u64)b_hi * c_lo;
    u64 b_hi_c_hi = (u64)b_hi * c_hi;
    
    // 计算 b_c_high = (b_lo*c_hi + b_hi*c_lo) + (b_lo*c_lo >> 32)
    u64 b_c_low = b_lo_c_lo;
    u64 b_c_high = (b_lo_c_hi << 32) + b_hi_c_lo + (b_c_low >> 32);
    u64 b_c_carry = (b_hi_c_hi << 32) + (b_c_high >> 32);
    b_c_high &= 0xFFFFFFFFULL;
    b_c_high += (b_hi_c_hi << 32);
    
    // 计算 a * c = a_lo*c_lo + (a_lo*c_hi + a_hi*c_lo) << 32 + (a_hi*c_hi) << 64
    u64 a_lo_c_lo = (u64)a_lo * c_lo;
    u64 a_lo_c_hi = (u64)a_lo * c_hi;
    u64 a_hi_c_lo = (u64)a_hi * c_lo;
    u64 a_hi_c_hi = (u64)a_hi * c_hi;
    
    // 计算 a_c_high = (a_lo*c_hi + a_hi*c_lo) + (a_lo*c_lo >> 32)
    u64 a_c_low = a_lo_c_lo;
    u64 a_c_high = (a_lo_c_hi << 32) + a_hi_c_lo + (a_c_low >> 32);
    u64 a_c_carry = (a_hi_c_hi << 32) + (a_c_high >> 32);
    a_c_high &= 0xFFFFFFFFULL;
    a_c_high += (a_hi_c_hi << 32);
    
    // 高128位结果 = (a * c) + (b * c >> 64)
    // = (a_c_carry << 64 | a_c_high << 32 | a_c_low) + (b_c_high << 32 | b_c_low) >> 64
    // = (a_c_carry << 64 | a_c_high) + b_c_high
    
    u64 carry = 0;
    u64 low_part = a_c_high + b_c_high;
    if (low_part < a_c_high) {
        carry = 1;
    }
    
    u64 high_part = a_c_carry + carry;
    
    result->low = low_part;
    result->high = high_part;
#endif
}

// 辅助函数：将u128_struct转换为字符串，用于调试
static inline void u128_struct_to_string(const u128_struct* val, char* buffer, size_t buffer_size) {
    if (val == NULL || buffer == NULL || buffer_size < 35) {
        return;
    }
    
    // 格式化字符串："0xHHHHHHHHHHHHHHHH_LLLLLLLLLLLLLLLL"
    // 使用%llx格式说明符，并将uint64_t转换为unsigned long long
    snprintf(buffer, buffer_size, "0x%016llx_%016llx", 
             (unsigned long long)val->high, 
             (unsigned long long)val->low);
}

#endif // U128_MUL_H
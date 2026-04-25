import sys
from fractions import Fraction
import math

# -------------------- 工具函数 --------------------

def f16_to_components(bits: int):
    """
    解析 IEEE 754 binary16 正数 (sign=0)，返回 (c, q, is_regular, is_irregular)。
    排除 +0, inf, NaN。
    """
    exp = (bits >> 10) & 0x1F      # 5 位指数
    frac = bits & 0x3FF            # 10 位尾数

    if exp == 0 and frac == 0:
        raise ValueError("+0 excluded")
    if exp == 31:
        raise ValueError("inf/NaN excluded")

    if exp == 0:                  # subnormal
        c = frac
        q = -24
        is_regular = True         # subnormal 属于 regular (因为 f≠0)
        is_irregular = False
    else:                         # normal
        c = 1024 + frac
        q = exp - 25
        if frac == 0:
            is_regular = False
            is_irregular = True
        else:
            is_regular = True
            is_irregular = False

    return c, q, is_regular, is_irregular


def compute_k(q: int, is_regular: bool) -> int:
    """
    对于 regular:  10^k <= 2^q < 10^{k+1}
    对于 irregular: (4/3)*10^k <= 2^q < (4/3)*10^{k+1}
    """
    if is_regular:
        k = math.floor(q * math.log10(2))
    else:
        k = math.floor(q * math.log10(2) - math.log10(4/3))
    return int(k)  # 取整

def f16_to_decimal(bits: int):
    """
    根据算法 1 计算 binary16 正数的 (d, k)，满足 SW 原则。
    """
    c, q, is_regular, is_irregular = f16_to_components(bits)

    # Step 2-5: 计算 k
    k = compute_k(q, is_regular)

    # v = c * 2^q
    v = c * (Fraction(2) ** q)

    # Step 7: m = floor(v * 10^{-k-1})
    # R = v * 10^{-k-1}
    R = v * (Fraction(10) ** (-k - 1))
    m = int(R)                      # floor(R)
    n = R - m                       # 小数部分

    # Step 9: ten = 10*m
    ten = 10 * m

    # Step 10: δ = fractional part of 10n
    # 10n = 10*(R - m) = 10*R - 10*m
    tenR = v * (Fraction(10) ** (-k))   # = v * 10^{-k}
    ten_n = tenR - ten
    floor_ten_n = int(ten_n)        # floor(10n)
    delta = ten_n - floor_ten_n

    # Step 11-21: 根据 δ 确定 one 初值
    if delta == Fraction(1, 2):
        # 0.5 的情况：round to even
        if floor_ten_n % 2 == 0:
            one = floor_ten_n
        else:
            one = floor_ten_n + 1
    elif delta < Fraction(1, 2):
        one = floor_ten_n
    else:
        one = floor_ten_n + 1

    # Step 22-28: irregular 的特殊处理
    if is_irregular:
        # 2^{q-2} * 10^{-k}
        cond1_val = (Fraction(2) ** (q - 2)) * (Fraction(10) ** (-k))
        # 2^{q-2} * 10^{-k-1}
        cond2_val = (Fraction(2) ** (q - 2)) * (Fraction(10) ** (-k - 1))

        if delta > cond1_val:
            one = floor_ten_n + 1
        if cond2_val >= n:
            one = 0
    else:
        # Step 30-35: regular 情况下检查是否 one=0 或 one=10 更短
        # 边界值 A = 2^{q-1} * 10^{-k-1}
        A = (Fraction(2) ** (q - 1)) * (Fraction(10) ** (-k - 1))

        # 条件1: one = 0
        if A > n or (A == n and c % 2 == 0):
            one = 0
        # 条件2: one = 10
        elif A > (1 - n) or (A == 1 - n and c % 2 == 0):
            one = 10

    # Step 37: d = ten + one
    d = ten + one

    return d, k


# -------------------- 主程序 --------------------

def main():
    output_filename = "f16_decimal_results.txt"

    with open(output_filename, "w", encoding="utf-8") as f:
        f.write("# bits(hex)  d  k\n")
        # 正数范围：0x0001 .. 0x7BFF （排除 +0, inf, NaN）
        for bits in range(0x0001, 0x7C00):
            try:
                d, k = f16_to_decimal(bits)
                f.write(f"0x{bits:04X} {d} {k}\n")
            except ValueError:
                # 正常情况不会触发，因为我们跳过了 +0, inf, NaN
                continue
            except Exception as e:
                print(f"Error processing 0x{bits:04X}: {e}", file=sys.stderr)
                raise

    print(f"Results written to {output_filename}")


if __name__ == "__main__":
    main()
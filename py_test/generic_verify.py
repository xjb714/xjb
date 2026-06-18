import math
from fractions import Fraction

# generic float type definition: (sign_bits, exp_bits, sig_bits, explicit_leading_bit)
type = {
        # sign bit + exponent bits + significand bits + explicit leading bit
        "float8_e5m2": (1, 5, 2, 0), 
        "float8_e4m3": (1, 4, 3, 0),
        "bfloat16": (1, 8, 7, 0), 
        "float16": (1, 5, 10, 0), 
        "float32": (1, 8, 23, 0), 
        "tfloat32": (1, 8, 10, 0),
        "float64": (1, 11, 52, 0), 
        "float80": (1, 15, 64, 1), 
        "float128": (1, 15, 112, 0),
        # "float256":(1, 19, 236, 0), # k = 256; round(4*log2(k)) - 13 = 19
        # "float512":(1, 18, 493, 0) # CEA-List VXP
    }

# base formula
def find_best_rational_approximation_below_and_above(a, b, c):
    """
    纯整数运算实现的分母不超过a的最佳有理逼近b/c
    
    参数:
        a (int): 分母最大值 
        b (int): 目标分子
        c (int): 目标分母 (c > a)
    
    返回:
        tuple: ((below_num, below_den), (above_num, above_den))
    """
    if c <= a:
        raise ValueError("c必须大于a")
    
    # 初始化法里数列边界 
    # 左边界 (小于等于目标的分数)
    low_num, low_den = 0, 1 # 0
    
    # 右边界 (大于等于目标的分数)
    high_num, high_den = 1, 0 # 无穷大
    
    # 最佳逼近初始化 
    best_below = (0, 1)
    best_above = (1, 0)
    
    # 计算交叉积差值用于比较
    def compare_fraction(num1, den1, num2, den2):
        """比较两个分数的大小，返回-1/0/1 表示小于/等于/大于"""
        left = num1 * den2 
        right = num2 * den1
        return (left > right) - (left < right)
    
    # 主循环
    while True:
        # 计算中间分数 (法里数列性质)
        med_num = low_num + high_num 
        med_den = low_den + high_den
        
        # 检查分母是否超过限制 
        if med_den > a:
            break
        
        # 比较中间分数与目标值 (使用交叉乘法避免浮点运算)
        cmp = compare_fraction(med_num, med_den, b, c)
        
        if cmp <= 0:
            # 中间分数 <= 目标值 
            # 检查是否为更好的下方逼近 
            current_diff = b * med_den - c * med_num
            best_diff = b * best_below[1] - c * best_below[0]
            
            if current_diff < best_diff or (current_diff == best_diff and med_den > best_below[1]):
                best_below = (med_num, med_den)
            
            # 更新左边界
            low_num, low_den = med_num, med_den 
        else:
            # 中间分数 > 目标值
            # 检查是否为更好的上方逼近 
            current_diff = c * med_num - b * med_den
            best_diff = c * best_above[0] - b * best_above[1]
            
            if current_diff < best_diff or (current_diff == best_diff and med_den > best_above[1]):
                best_above = (med_num, med_den)
            
            # 更新右边界 
            high_num, high_den = med_num, med_den
    
    return best_below, best_above

def generic_float_to_decimal(float_type,float_bits):
    """
    根据算法 1 计算 的 (d, k) 满足 SW 原则。
    参数:
        float_type (str): 浮点类型，如 "float16", "float32" 等
        float_bits (int): 浮点数的二进制表示（不包含符号位）
    返回:
        tuple: (d, k) 满足 SW 原则的十进制表示
    说明:
        1. float_bits 是去掉符号位后的二进制表示，长度为 exp_bits + sig_bits。
        2. 该函数实现了算法 1 中的步骤，计算出满足 SW 原则的十进制表示 (d, k)。
        3. 该函数使用了 Fraction 来保持计算的精确性，避免浮点误差。
        4. 不处理 0、inf、NaN 等特殊值，假设输入的 float_bits 是一个有效的正数表示。
    """

    sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    sig = float_bits & ((1 << sig_bits) - 1) # 提取尾数
    exp = (float_bits >> sig_bits) & ((1 << exp_bits) - 1) # 提取指数
    if exp == 0:
        # subnormal
        c = sig
        q = 1 - (1 << (exp_bits - 1)) + 1 - sig_bits
        is_regular = True
        is_irregular = False
    else:
        # normal
        if explicit_leading_bit == 0:
            c = (1 << sig_bits) + sig
        else:
            c = sig
        q = exp - ((1 << (exp_bits - 1)) - 1) - sig_bits
        if sig == 0:
            is_regular = False
            is_irregular = True
        else:
            is_regular = True
            is_irregular = False

    # Step 2-5: 计算 k
    if is_regular:
        k = math.floor(q * math.log10(2))
    else:
        k = math.floor(q * math.log10(2) - math.log10(4/3))

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

    A = (Fraction(2) ** (q - 1)) * (Fraction(10) ** (-k - 1))
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
        # 条件1: one = 0
        if A > n or (A == n and c % 2 == 0):
            one = 0
    # 条件2: one = 10
    if A > (1 - n) or (A == 1 - n and c % 2 == 0):
        one = 10
    # Step 37: d = ten + one
    d = ten + one

    return d, k



def generic_verify_float_to_string(float_type):
    # sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    # exp_bias = (1 << (exp_bits - 1)) - 1
    # max_exp = (1 << exp_bits) - 1 - exp_bias
    # min_exp = 1 - exp_bias
    # min_sig = 1
    # max_sig = (1 << sig_bits) - 1
    # print(f"{float_type}: sign={sign_bits} bits, exponent={exp_bits} bits, significand={sig_bits} bits, explicit_leading_bit={explicit_leading_bit}, exp_bias={exp_bias}, max_exp={max_exp}, min_exp={min_exp}, max_sig={max_sig}")
    
    #find_min_BIT_calc_m(float_type)
    find_q_range(float_type)

def verify_float_to_string(float_type, float_bits):
    d, k = generic_float_to_decimal(float_type, float_bits)
    print(f"{float_type} bits: {float_bits}, decimal: {d} * 10^{k}")

def verify():
    for float_type in type.keys():
        print(f"Verifying {float_type}...")
        sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
        total_bits =  exp_bits + sig_bits
        max_positive_value = (((1 << exp_bits) - 1) << sig_bits) - 1
        for float_bits in range(1, max_positive_value + 1):
            verify_float_to_string(float_type, float_bits)

def find_min_lut_bit_length(float_type):
    sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    exp_bias = (1 << (exp_bits - 1)) - 1
    max_exp = (1 << exp_bits) - 1 - exp_bias
    min_exp = 1 - exp_bias
    min_sig = 1
    max_sig = (1 << sig_bits) - 1
    max_positive_value = (((1 << exp_bits) - 1) << sig_bits) - 1
    print(f"{float_type}: sign={sign_bits} bits, exponent={exp_bits} bits, significand={sig_bits} bits, explicit_leading_bit={explicit_leading_bit}, exp_bias={exp_bias}, max_exp={max_exp}, min_exp={min_exp}, max_sig={max_sig}, max_positive_value={max_positive_value}")

def r(a,b,c,d):
    # (a/b - c/d) / (c/d) = (a*d - b*c)/(b*c)
    return (a * d - b * c) / (b * c)

# def find_min_BIT(a,b,c,d):
#     # (c/d) * ( 1 + 2**-Q) < (a/b)
#     Q=1
#     while True:
#         if (c * (2**Q + 1) * b < a * d * (2**Q)):
#             return Q
#         Q+=1
def find_min_BIT(a: int, b: int, c: int, d: int) -> int:
    """
    返回满足 (c/d) * (1 + 2^{-Q}) < a/b 的最小正整数 Q。
    假设 a, b, c, d 均为正整数，且 a*d > b*c(否则无解).
    """
    num = b * c
    den = a * d - b * c
    # 若无解（原函数会无限循环），这里返回 -1 表示无效输入
    # if den <= 0:
    #     return -1

    if num == 0:          # c == 0 时，不等式恒成立，最小 Q 为 1
        return 1

    # 最小 Q 满足 2^Q * den > num  =>  Q = max(1, (num // den).bit_length())
    n = num // den
    Q = n.bit_length()
    return Q if Q >= 1 else 1

def find_min_BIT_calc_m_impl(float_type):
    sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    if explicit_leading_bit == 0:
        C = (1 << (sig_bits + 1)) - 1
    else:
        C = (1 << (sig_bits + 0)) - 1
    min_q = 1 - ((1 << (exp_bits - 1)) - 1) - sig_bits
    max_q = ((1 << (exp_bits - 1)) - 1) - sig_bits + 1
    minr = 1
    MAX_BIT=1
    for q in range(min_q, max_q):  # [min_q,max_q-1] : { float:[-149, 104] ; double:[-1074, 971]; }
        k = math.floor(q * math.log10(2))
        A = q - k - 1
        B = k + 1
        # x = target = 2**A * 5**(-B)
        P = Q = 1
        # if A >= 0 and B >= 0:
        #     P = 2**A
        #     Q = 5**B
        # elif A >= 0 and B <= 0:
        #     P = 2**A * 5**(-B)
        #     Q = 1
        # else:  # q<-3
        #     P = 5**(-B)
        #     Q = 2**(-A)
        if(A>=0):
            P*=2**A
        else:
            Q*=2**(-A)
        if(B>=0):
            Q*=5**B
        else:
            P*=5**(-B)
    

        UP = (1 , 1)
        # x = target = P / Q
        # floor(c*x) = floor(c*x_above)
        # x <= x_above < UP_num/UP_num = UP[0]/UP[1]



        # compute up_bound
        if(Q <= C):
            # x * P / Q   <    x * P / Q * (1 + 1/(C*Q) )
            # P / Q * (1 + 1/(C*Q) = (C*P+1) / (C*Q)
            #UP=(C*P+1,C*Q)
            UP=(C*Q*P+P,C*Q*Q)
        else:
            (below,UP) = find_best_rational_approximation_below_and_above(C,P,Q)

        maxr = r(UP[0], UP[1], P, Q)

        BIT = find_min_BIT(UP[0], UP[1], P, Q)
        
        MAX_BIT=max(MAX_BIT,BIT)
        
        minr = min(minr, maxr)

        # if(maxr<=0):
        #     print("q = ", q, "error")
        # else:
        #     print("q =", q, "UP_num =", UP[0], "UP_den =", UP[1], "log2(r) =", math.log2(maxr),"BIT =",BIT)
        
    # print("checking end minr =",minr," log2(r) =",math.log2(minr),"MAX_BIT =",MAX_BIT)
    # print("")

    return (minr,MAX_BIT)

def find_2c_sub_1_range(float_type):
    sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    exp_bias = (1 << (exp_bits - 1)) - 1
    max_exp = (1 << exp_bits) - 1 - exp_bias
    min_exp = 1 - exp_bias
    min_sig = 1
    max_sig = (1 << sig_bits) - 1
    max_positive_value = (((1 << exp_bits) - 1) << sig_bits) - 1

    for exp in range(min_exp, max_exp + 1):
        for sig in range(min_sig, max_sig + 1):
            if explicit_leading_bit == 0:
                c = sig
            else:
                c = (1 << sig_bits) + sig
            q = exp - exp_bias - sig_bits
            if c % 2 == 0:
                print(f"{float_type}: c={c}, q={q} => regular")
            else:
                print(f"{float_type}: c={c}, q={q} => irregular")


def max_k_s1(b: int) -> int:
    """
    返回最大的非负整数 k, 使得存在 c ∈ [2^b+1, 2^(b+1)-1] 满足 5^(k+1) | (2c-1)。
    若不存在这样的 k, 则返回 -1。
    """
    if b <= 0:
        return -1
    low = (1 << (b + 1)) + 1   # 2^(b+1)+1
    high = (1 << (b + 2)) - 3  # 2^(b+2)-3
    # ---------- 情况：检查是否存在大模数 M = 5^(k+1) 或 3*M=3*5^(k+1) 在 [2^(b+1)+1, 2^(b+2)-3] 中 ----------
    # 计算候选指数 e = k+1 的近似值: 5^e ≈ 2^(b+1)  ⇒  e ≈ (b+1) / log2(5)
    log2_5 = math.log2(5)          # ≈ 2.321928094887362
    e_approx = int((b + 1) / log2_5)
    # 检查附近几个整数
    for e in range(e_approx + 2, max(1, e_approx - 2) - 1, -1):
        M = pow(5, e)              # 5^e
        if low <= M <= high or low <= 3 * M <= high:
            return e - 1           # k = e-1
    print(f"No valid k found for b = {b} in either case.")
    return -1

def max_k_a1(b: int) -> int:
    """
    返回最大的非负整数 k, 使得存在 c ∈ [2^b+1, 2^(b+1)-1] 满足 5^(k+1) | (2c+1)。
    若不存在这样的 k, 则返回 -1。
    """
    if b <= 0:
        return -1
    low = (1 << (b + 1)) + 3   # 2^(b+1)+3
    high = (1 << (b + 2)) - 1  # 2^(b+2)-1
    # ---------- 情况：检查是否存在大模数 M = 5^(k+1) 或 3*M=3*5^(k+1) 在 [2^(b+1)+3, 2^(b+2)-1] 中 ----------
    # 计算候选指数 e = k+1 的近似值: 5^e ≈ 2^(b+1)  ⇒  e ≈ (b+1) / log2(5)
    log2_5 = math.log2(5)          # ≈ 2.321928094887362
    e_approx = int((b + 1) / log2_5)
    # 检查附近几个整数
    for e in range(e_approx + 2, max(1, e_approx - 2) - 1, -1):
        M = pow(5, e)              # 5^e
        if low <= M <= high or low <= 3 * M <= high:
            return e - 1           # k = e-1
    print(f"No valid k found for b = {b} in either case.")
    return -1

def find_q_range(float_type):
    # for normal value
    # find max k:
    # (2c - 1) % 5**(k+1) = 0
    # (2c + 1) % 5**(k+1) = 0
    sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    #C = (1 << (sig_bits + 1)) - 1 if explicit_leading_bit == 0 else (1 << sig_bits) - 1
    if explicit_leading_bit == 0:
        C_min = (1 << (sig_bits + 0)) + 1
        C_max = (1 << (sig_bits + 1)) - 1
    else:
        C_min = (1 << (sig_bits - 1)) + 1
        C_max = (1 << (sig_bits + 0)) - 1
    C2_s1_min = 2 * C_min - 1
    C2_s1_max = 2 * C_max - 1
    C2_a1_min = 2 * C_min + 1
    C2_a1_max = 2 * C_max + 1
    k_max_s1 = max_k_s1(sig_bits - explicit_leading_bit) # 2c-1 的 c 范围是 [2^b+1, 2^(b+1)-1], b = sig_bits - explicit_leading_bit
    k_max_a1 = max_k_a1(sig_bits - explicit_leading_bit) # 2c+1 的 c 范围是 [2^b+1, 2^(b+1)-1], b = sig_bits - explicit_leading_bit
    if(k_max_s1 != k_max_a1):
        # print("error")
        print(f"Warning: for {float_type}, max k for s1 is {k_max_s1} but max k for a1 is {k_max_a1}.")
    k_max = k_max_a1
    # print(f"{float_type}: C range = [{C_min}, {C_max}], 2C-1 range = [{C2_s1_min}, {C2_s1_max}], 2C+1 range = [{C2_a1_min}, {C2_a1_max}], max k (s1) = {k_max_s1}, max k (a1) = {k_max_a1}")
    q_max = math.floor((k_max + 1) * math.log2(10)) # k = floor(q*log10(2))
    print(f"{float_type}: k range = [0, {k_max}], corresponding q range is approximately [2, {q_max}]")
    return ((0,k_max), (2,q_max))
    
def solve_X_and_BIT(float_type):
    sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    # X,BIT

    # ( 2**(q+X-2-k) % 5**(k+1) ) / 5**(k+1) + c * 2**(q+X-1-k) / 5**(k+1) * (r - 1) < 1
    ((k_min, k_max), (q_min, q_max)) = find_q_range(float_type)
    for q in range(q_min, q_max + 1):
        k = math.floor(q * math.log10(2))
        
        


def find_min_BIT_calc_m(float_type):
    #sign_bits, exp_bits, sig_bits, explicit_leading_bit = type[float_type]
    (minr, max_bit) = find_min_BIT_calc_m_impl(float_type)
    print(f"{float_type}: minr={minr}, log2(minr)={math.log2(minr)}, max_bit={max_bit}")

if __name__ == "__main__":
    #verify()
    for float_type in type.keys():
        generic_verify_float_to_string(float_type)
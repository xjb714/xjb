# float
import math

FLOAT = 0
C1 = 2**25 - 1
min_float_q = -149
max_float_q = 104


DOUBLE = 1
C2 = 2**54 - 1
min_double_q = -1074
max_double_q = 971

def find_best_rational_approximation_below_and_above(a, b, c):
    """
    纯整数运算实现的分母不超过a的最佳有理逼近b/c
    
    参数:
        a (uint): 分母最大值 
        b (uint): 目标分子
        c (uint): 目标分母 (c > a)
    
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
def simplify_fraction(numerator, denominator) :
    """
    化简大整数分数
    
    参数:
        numerator: 分子
        denominator: 分母
        
    返回:
        化简后的分子和分母
        
    异常:
        ValueError: 当分母为零时抛出
    """
    if denominator == 0:
        raise ValueError("分母不能为零")
    
    # 计算最大公约数(GCD)
    gcd = math.gcd(numerator, denominator)
    
    # 化简分数
    simplified_numerator = numerator // gcd
    simplified_denominator = denominator // gcd
    
    # 确保分母为正
    if simplified_denominator < 0:
        simplified_numerator = -simplified_numerator
        simplified_denominator = -simplified_denominator
    
    return simplified_numerator, simplified_denominator

def max_fraction(num1, den1, num2, den2):
    # if(num1/den1 >= num2/den2):return num1,den1
    # else:return num2,den2
    if(num1 * den2 >= num2 * den1):return (num1,den1)
    else:return (num2,den2)
def min_fraction(num1, den1, num2, den2):
    # if(num1/den1 <= num2/den2):return num1,den1
    # else:return num2,den2
    if(num1 * den2 <= num2 * den1):return (num1,den1)
    else:return (num2,den2)
def find_n_min_min_Q(a,b):
    # a/b > 2**(-Q)
    # find min Q
    Q = 1
    while True:
        if (a * (2**Q) > b):
            return Q
        Q+=1
def find_n_max_min_Q(a,b):
    # a/b < 1 - 2**(-Q)
    # 1 - a/b > 2**(-Q)
    # find min Q
    Q = 1
    while True:
        if ( (b - a) * (2**Q) >= b):
            return Q
        Q+=1
def check(float_or_double):

    C = 1 # C1 or C2
    if(float_or_double==FLOAT):
        C = C1
        C2_min = 2**24 + 3
        C2_max = 2**25 - 1
        min_q = min_float_q
        max_q = max_float_q + 1
        print("checking Float:")
    else:
        C = C2
        C2_min = 2**53 + 3
        C2_max = 2**54 - 1
        min_q = min_double_q
        max_q = max_double_q + 1
        print("checking Double:")
    MAX_N=(0,1)
    MIN_N=(1,1)
    for q in range(min_q, max_q):  # [min_q,max_q-1] : { float:[-149, 104] ; double:[-1074, 971]; }
        k = math.floor(q * math.log10(2))
        A = q - k - 2
        B = k + 1
        # x = target = 2**A * 5**(-B)
        P = Q = 1
        if(A>=0):
            P*=2**A
        else:
            Q*=2**(-A)
        if(B>=0):
            Q*=5**B
        else:
            P*=5**(-B)
        UP = (1 , 1)
        DOWN=(1 , 1)
        if(Q > C2_max):

            (DOWN,UP) = find_best_rational_approximation_below_and_above(C2_max,P,Q)

            #n_min_frac = ( C2_min * (P*DOWN[1] - Q*DOWN[0]) , (Q * DOWN[1]) )
            n_min_frac = ((DOWN[1]*P) % Q , Q)

            #n_max_frac =  ( (Q * UP[1]) - C2_min * (Q*UP[0] - UP[1]*P) , (Q * UP[1]) )
            n_max_frac = ((UP[1]*P) % Q , Q)

            MIN_N = min_fraction(MIN_N[0],MIN_N[1],n_min_frac[0],n_min_frac[1])

            MAX_N = max_fraction(MAX_N[0],MAX_N[1],n_max_frac[0],n_max_frac[1])

    MIN_N = simplify_fraction(MIN_N[0],MIN_N[1])
    MAX_N = simplify_fraction(MAX_N[0],MAX_N[1])
    return (MIN_N , MAX_N)

if __name__ == "__main__":

    # float
    (MIN_N,MAX_N) = check(FLOAT)
    Q_min = find_n_min_min_Q(MIN_N[0],MIN_N[1])
    Q_max = find_n_max_min_Q(MAX_N[0],MAX_N[1])
    print("Final float : MIN_N =",MIN_N[0], " / ", MIN_N[1]," > 2**( -",Q_min,")")
    print("Final float : MAX_N =",MAX_N[0], " / ", MAX_N[1]," < 1 - 2**( -",Q_max,")")
    # double
    (MIN_N,MAX_N) = check(DOUBLE)
    Q_min = find_n_min_min_Q(MIN_N[0],MIN_N[1])
    Q_max = find_n_max_min_Q(MAX_N[0],MAX_N[1])
    print("Final double : MIN_N =",MIN_N[0], " / ", MIN_N[1]," > 2**( -",Q_min,")")
    print("Final double : MAX_N =",MAX_N[0], " / ", MAX_N[1]," < 1 - 2**( -",Q_max,")")
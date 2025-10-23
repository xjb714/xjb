# float
import math

FLOAT = 0
C1 = 2**24 - 1
min_float_q = -149
max_float_q = 104


DOUBLE = 1
C2 = 2**53 - 1
min_double_q = -1074
max_double_q = 971

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

def r(a,b,c,d):
    # (a/b - c/d) / (c/d) = (a*d - b*c)/(b*c)
    return (a * d - b * c) / (b * c)

def find_min_BIT(a,b,c,d):
    # (c/d) * ( 1 + 2**-Q) < (a/b)
    Q=1
    while True:
        if (c * (2**Q + 1) * b < a * d * (2**Q)):
            return Q
        Q+=1

def check(float_or_double):

    C = 1 # C1 or C2
    if(float_or_double==FLOAT):
        C = C1
        min_q = min_float_q
        max_q = max_float_q + 1
        print("checking Float:")
    else:
        C = C2
        min_q = min_double_q
        max_q = max_double_q + 1
        print("checking Double:")
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

        if(maxr<=0):
            print("q = ", q, "error")
        else:
            print("q =", q, "UP_num =", UP[0], "UP_den =", UP[1], "log2(r) =", math.log2(maxr),"BIT =",BIT)
        
    print("checking end minr =",minr," log2(r) =",math.log2(minr),"MAX_BIT =",MAX_BIT)
    print("")

    return (minr,MAX_BIT)

if __name__ == "__main__":

    (r1,max_bit1) = check(FLOAT)
    (r2,max_bit2) = check(DOUBLE)

    print("Final float : min r =",r1,"log2(r) =",math.log2(r1),"ceil(-log2(r)) =",math.ceil(-math.log2(r1)),"max_bit = ",max_bit1)
    print("Final double : min r =",r2,"log2(r) =",math.log2(r2),"ceil(-log2(r)) =",math.ceil(-math.log2(r2)),"max_bit = ",max_bit2)
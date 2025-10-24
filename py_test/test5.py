import math
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


MIN_r=(1,1)
MAX_r=(0,1)
for q in range(-149,105):
    k = math.floor(q * math.log10(2))
    A = q - k + 1
    B = k
    # x = target = 2**A * 5**-B
    P = Q = 1
    if(A>=0):
        P*=2**A
    else:
        Q*=2**(-A)
    if(B>=0):
        Q*=5**B
    else:
        P*=5**(-B)
    # find best rational approximation of P/Q with den <= 2**24 - 1
    C = 2**24 - 1
    if(Q <= C):
        pass
    else:
        (below_num, below_den), (above_num, above_den) = find_best_rational_approximation_below_and_above(C, P, Q)
        min_r = ( (below_den * P) % Q , Q)
        max_r = ( (above_den * P) % Q , Q)
        #min_q = find_n_min_min_Q(min_r[0],min_r[1])
        #max_q = find_n_max_min_Q(max_r[0],max_r[1])
        MIN_r = min_fraction(MIN_r[0],MIN_r[1],min_r[0],min_r[1])
        MAX_r = max_fraction(MAX_r[0],MAX_r[1],max_r[0],max_r[1])
        #print(q," : min_r =",min_r[0]," / ",min_r[1],"= ",min_r[0]/min_r[1],"> 2**(" , -min_q ,")", "; max_r =",max_r[0]," / ",max_r[1],"= ",max_r[0]/max_r[1],"< 1 - 2**(",-max_q,")")
min_Q=find_n_min_min_Q(MIN_r[0],MIN_r[1])
max_Q=find_n_max_min_Q(MAX_r[0],MAX_r[1])
print("float : min_r =",MIN_r[0]," / ",MIN_r[1],"= ",MIN_r[0]/MIN_r[1],"> 2**(" , -min_Q ,")", "; max_r =",MAX_r[0]," / ",MAX_r[1],"= ",MAX_r[0]/MAX_r[1],"< 1 - 2**(",-max_Q,")")

MIN_r=(1,1)
MAX_r=(0,1)
for q in range(-1074,972):
    k = math.floor(q * math.log10(2))
    A = q - k + 1
    B = k
    # x = target = 2**A * 5**-B
    P = Q = 1
    if(A>=0):
        P*=2**A
    else:
        Q*=2**(-A)
    if(B>=0):
        Q*=5**B
    else:
        P*=5**(-B)
    # find best rational approximation of P/Q with den <= 2**53 - 1
    C = 2**53 - 1
    if(Q <= C):
        pass
    else:
        (below_num, below_den), (above_num, above_den) = find_best_rational_approximation_below_and_above(C, P, Q)
        min_r = ( (below_den * P) % Q , Q)
        max_r = ( (above_den * P) % Q , Q)
        #min_q = find_n_min_min_Q(min_r[0],min_r[1])
        #max_q = find_n_max_min_Q(max_r[0],max_r[1])
        MIN_r = min_fraction(MIN_r[0],MIN_r[1],min_r[0],min_r[1])
        MAX_r = max_fraction(MAX_r[0],MAX_r[1],max_r[0],max_r[1])
        #print(q," : min_r =",min_r[0]," / ",min_r[1],"= ",min_r[0]/min_r[1],"> 2**(" , -min_q ,")", "; max_r =",max_r[0]," / ",max_r[1],"= ",max_r[0]/max_r[1],"< 1 - 2**(",-max_q,")")
        # if(20*min_r[1] <= min_r[0]*2**64):
        #     pass
        #     #print(q," : min_r ","> 2**(" , -min_q ,")", "; max_r ","< 1 - 2**(",-max_q,")")
        # else:
        #     #print(q,below_den)
        #     if(below_den>=2**52+1 and below_den<=2**53-1):
        #         print(q,below_den,"error : min_r ","> 2**(" , -min_q ,")", "; max_r ","< 1 - 2**(",-max_q,") **********")
min_Q=find_n_min_min_Q(MIN_r[0],MIN_r[1])
max_Q=find_n_max_min_Q(MAX_r[0],MAX_r[1])

print("double : min_r =",MIN_r[0]," / ",MIN_r[1],"= ",MIN_r[0]/MIN_r[1],"> 2**(" , -min_Q ,")", "; max_r =",MAX_r[0]," / ",MAX_r[1],"< 1 - 2**(",-max_Q,")")
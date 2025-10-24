# float
import math

FLOAT = 0
min_float_q = -149
max_float_q = 104

DOUBLE = 1
min_double_q = -1074
max_double_q = 971

log2_10 = math.log2(10)

xjb32_pow10_lut = {} # length = 44 - (-32) + 1 = 77
xjb64_pow10_lut = {} # length = 323 - (-293) + 1 = 617

def generate_xjb32_lut():
    for i in range(-31-1,46-1,1):
        e10 = abs(i)
        e2 = abs(math.floor(i*log2_10) - 63)
        num = 0
        round_up = 0 if(i>=0 and i<=27) else 1
        if(i<0):
            num = (2**e2//10**e10)
        elif(i >= 20):
            num = 10**e10//2**e2
        else:
            num = 10**e10*2**e2
        result = num + round_up
        #print(hex(result),", //",i,end='\n')
        xjb32_pow10_lut[i+31+1] = result
    print("generate xjb32 lut finish:")
def generate_xjb64_lut():
    
    BIT = 128 - 1 ## 127
    ans = 0 ## 39
    i=0
    while 1:
        if(i*log2_10>=BIT):
            ans=i
            break
        i=i+1
    #print("ans = ",ans)
    for i in range(-292-1,325-1,1):
        e10 = abs(i)
        e2 = abs(math.floor(i*log2_10) - BIT)
        num=0
        round_up = 0
        if(i<0):         
            num = 2**e2//10**e10
            round_up = 1
        elif(i >= ans):
            num = 10**e10//2**e2
            round_up = 0 if(num * (2**e2) == 10**e10) else 1
        else:
            num = 10**e10*2**e2
        result = num + round_up
        xjb64_pow10_lut[i+292+1] = result
    print("generate xjb64 lut finish:")
def check_float():
    min_q = min_float_q
    max_q = max_float_q + 1
    print("checking Float:")
    all_right=1
    for q in range(min_q, max_q):  # [min_q,max_q-1] : { float:[-149, 104] ; double:[-1074, 971]; }
        k = math.floor(q * math.log10(2))
        h = q + math.floor((-k-1) * math.log2(10))
        offset = 28 - h
        A = q - k + 34
        B = k + 1
        # 2**(q+35) * 10**(-k-1) = 2**(q-k+34) * 5**(k+1)
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
        left = P//Q  # floor(P/Q)
        right = ( (xjb32_pow10_lut[(-k-1) + 31 + 1]) >> offset)  # xjb32_pow10_lut index: [0,76] for i in [-32,44]
        if(left != right):
            all_right=0
            print("mismatch q =",q,"k =",k,"A =",A,"B =",B,"-k-1 =",-k-1)
            print("left  =",left)
            print("right =",right)
            return
    if(all_right == 1):
        print("check float all right!")
def check_double():
    min_q = min_double_q
    max_q = max_double_q + 1
    print("checking Double:")
    all_right=1
    for q in range(min_q, max_q):  # [min_q,max_q-1] : { float:[-149, 104] ; double:[-1074, 971]; }
        k = math.floor(q * math.log10(2))
        h = q + math.floor((-k-1) * math.log2(10))
        offset = 64 - h
        A = q - k + 62
        B = k + 1
        # 2**(q+63) * 10**(-k-1) = 2**(q-k+62) * 5**(k+1)
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
        left = P//Q  # floor(P/Q)
        right = ( (xjb64_pow10_lut[(-k-1) + 292 + 1]) >> offset)  # xjb64_pow10_lut index: [0,617] for i in [-293,323]
        if(left != right):
            all_right = 0
            print("mismatch q =",q,"k =",k,"A =",A,"B =",B,"-k-1 =",-k-1)
            print("left  =",left)
            print("right =",right)
            return
    if(all_right == 1):
        print("check double all right!")

if __name__ == "__main__":

    generate_xjb32_lut()
    generate_xjb64_lut()

    check_float()
    check_double()
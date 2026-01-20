import math

for bit in range(14,18):
    magic = math.floor(2**bit * math.log2(10))
    for m in range(magic-10,magic+11,1):
        is_ok=1
        for k in range(-350,350+1):
            a = math.floor(k*math.log2(10))
            b = (k*m)>>bit
            if(a != b):
                is_ok=0
        if(is_ok==1):
            print("m =",m,"bit =",bit) # 108853,15
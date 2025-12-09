#schubfach32 xjb32

import math
log2_10 = math.log2(10)
print("generate xjb32 lut:")
#for i in range(-32,45,1): # [-32,44]
for i in range(44,-33,-1): # [44,-32]
    e10 = abs(i)
    e2 = abs(math.floor(i*log2_10) - 63)
    round_up = 0 if(i>=0 and i<=27) else 1
    if(i<0):
        num = (2**e2//10**e10)
    elif(i >= 20):
        num = 10**e10//2**e2
    else:
        num = 10**e10*2**e2
    result = num + round_up
    print(hex(result),", //",i,end='\n')

# import math
# log2_10 = math.log2(10)
# for i in range(-32,46,1):
#     e10 = abs(i)
#     e2 = abs(math.floor(i*log2_10) - 96)
#     pow2=math.floor(i*log2_10) - 95
#     num=0
#     if(i<0):
#         num = 2**e2//10**e10
#     elif(i >= 29):
#         num = 10**e10//2**e2
#     else:
#         num = 10**e10*2**e2
#     result = (num//2)
#     print(hex(result >> 64),end=",")
#     print(hex(result & ((2**64) - 1)),", //",i,end='\n')

#xjb32

# import math
# log2_10 = math.log2(10)
# BIT = 91-1 ## 90
# ans = 0 ## 28
# i=0
# while 1:
#     if(i*log2_10>=BIT):
#         ans=i
#         break
#     i=i+1
# print("ans = ",ans)
# for i in range(-32,45,1):
#     e10 = abs(i)
#     e2 = abs(math.floor(i*log2_10) - BIT)
#     num=0
#     round_up = 0
#     if(i<0):
#         num = 2**e2//10**e10
#         round_up = 1
#         r= ((num+1)*10**e10 - (2**e2)) / (2**e2)
#     elif(i >= ans):
#         num = 10**e10//2**e2
#         round_up = 0 if(num * (2**e2) == 10**e10) else 1
#         r= ((num+round_up)*2**e2 - (10**e10)) / (10**e10)
#     else:
#         num = 10**e10*2**e2
#         r= 0
#     result = num + round_up
#     if round_up==1:
#         print("i =",i,"r =",r,"log2(r) =",math.log2(r)) ## r>0
#     else:
#         print("i =",i,"r =",r) ## r==0
#     #print(hex(result >> 64),end=",")
#     #print(hex(result & ((2**64) - 1)),", //",i,end='\n')



#schubfach64 xjb64
print("\ngenerate xjb64 lut:")
BIT = 128 - 1 ## 127
ans = 0 ## 39
i=0
while 1:
    if(i*log2_10>=BIT):
        ans=i
        break
    i=i+1
#print("ans = ",ans)
for i in range(-292-1,325,1):
    e10 = abs(i)
    e2 = abs(math.floor(i*log2_10) - BIT)
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
    print(hex(result >> 64),end=",")
    print(hex(result & ((2**64) - 1)),", //",i,end='\n')

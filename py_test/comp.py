# this is an unused file, just for test


import math
log2_10 = math.log2(10)

pow_5 =[0x0000000000000001, 0x0000000000000005, 0x0000000000000019,
        0x000000000000007d, 0x0000000000000271, 0x0000000000000c35,
        0x0000000000003d09, 0x000000000001312d, 0x000000000005f5e1,
        0x00000000001dcd65, 0x00000000009502f9, 0x0000000002e90edd,
        0x000000000e8d4a51, 0x0000000048c27395, 0x000000016bcc41e9,
        0x000000071afd498d, 0x0000002386f26fc1, 0x000000b1a2bc2ec5,
        0x000003782dace9d9, 0x00001158e460913d, 0x000056bc75e2d631,
        0x0001b1ae4d6e2ef5, 0x000878678326eac9, 0x002a5a058fc295ed,
        0x00d3c21bcecceda1, 0x0422ca8b0a00a425, 0x14adf4b7320334b9]
# pow_10=[]
# for i in range(0, 20):
#     pow_10.append(10**i)
# for i in range(0, 20):
#     p10 = pow_10[i]
#     clz = (64 - p10.bit_length())
#     p10 = p10 << (clz)
#     pow_10[i] = p10
#     #print(p10,hex(p10),clz,end="\n")
pow5_rlz=[]
BIT = 128 - 1 ## 127
ans = 39 ## 39

def get_pow10(i):
    e10 = int(abs(i))
    e2 = int(abs(math.floor(i*log2_10) - BIT))
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
    return result

def get_pow10_not_round(i):
    e10 = int(abs(i))
    e2 = int(abs(math.floor(i*log2_10) - BIT))
    if(i<0):
        num = 2**e2//10**e10
    elif(i >= ans):
        num = 10**e10//2**e2
    else:
        num = 10**e10*2**e2
    result = num
    return result

# for i in range(0,27):
#     p5 = pow_5[i]
#     clz = (64 - p5.bit_length())
#     p5 = p5 << (clz)
#     pow_5[i] = p5
#     print(p5,hex(p5),clz,end="\n")

# def get_pow10_use_pow_10(i):
#     ratio = 20
#     base = math.floor(i / ratio)
#     off =  i - base * ratio
#     base_pow10 = get_pow10(base * ratio)
#     off_pow10 = pow_10[off]
#     p10_1 = ((base_pow10 * off_pow10) >> 64) # + (1 if i>=56 else 0)
#     p10_2 = ((base_pow10 * off_pow10) >> 63)
#     p10_3 = get_pow10(i)
#     diff = min(abs(p10_1 - p10_3), abs(p10_2 - p10_3))
#     #if(p10_1 != p10_3 or p10_2 != p10_3):
#     if(diff > 1):
#         print("error i =",i,"p10_1 =",hex(p10_1),"p10_2 =",hex(p10_2),"p10_3 =",hex(p10_3))
#     else:
#         print("ok i =",i,"p10_1 =",hex(p10_1),"p10_2 =",hex(p10_2),"p10_3 =",hex(p10_3))
#         pass

#last_base = -10000

def get_pow10_use_pow_5(i):
    ratio = 27
    base = math.floor(i / ratio) * ratio
    # if(base != last_base):
    #     print("base =",base)
    #     last_base = base
    pow5_offset =  i - base
    #pow10_base_nr = get_pow10_not_round(base)
    pow10_base = get_pow10(base)
    pow5 = 5 ** pow5_offset

    clz = 64 - pow5.bit_length()
    clz2 = 63 - ((pow5_offset * 149) >> 6)
    if(clz2 != clz): #
        print("error i =",i,"clz2 =",clz2,"clz =",clz)
    pow5_rlz = pow5 << clz2

    alpha = math.floor(i * math.log2(10)) - math.floor(base * math.log2(10)) - pow5_offset + clz
    alpha2 = math.floor(i * math.log2(10)) - math.floor(base * math.log2(10)) - math.floor(pow5_offset * math.log2(10)) + 63
    alpha3 = ((i*1741647) >> 19) - ((base*1741647) >> 19) - ((pow5_offset*1741647) >> 19) + 63
    alpha4 = ((i*1741647) >> 19) - ((base*1741647) >> 19) - ((pow5_offset*1741647) >> 19) # 0 or 1
    if(alpha != alpha2 or alpha != alpha3):
        print("error i =",i,"alpha =",alpha,"alpha2 =",alpha2,"alpha3 =",alpha3)

    #    a b
    #  x   c
    #  =(ac<<64) + bc
    pow10_base_hi = pow10_base >> 64 # a
    pow10_base_lo = pow10_base & ((2**64) - 1) # b
    ac = pow10_base_hi * pow5_rlz
    bc = pow10_base_lo * pow5_rlz
    p10_lo = (ac & ((2**64) - 1)) + (bc>>64) & ((2**64) - 1)
    p10_hi = ac >> 64
    p10 = (p10_hi << 64) + p10_lo
    p10_1 = ((pow10_base * pow5_rlz) >> 64)  # + (0 if 0<=i and i<=54 else 1))
    if(p10 == p10_1):
        #print("i =",i,"p10 == p10_1")
        pass
    else:
        #print("i =",i,"p10_hi =",p10_hi,"p10_lo =",p10_lo)
        #print("i =",i,"p10_1_hi =",p10_1>>64,"p10_1_lo =",p10_1 & ((2**64) - 1))
        pass
    #p10_1_nr = ((pow10_base_nr * pow5_rlz) >> 64)  # + (0 if 0<=i and i<=54 else 1))
    #p10_2 = ((pow10_base * pow5_rlz) >> 63)
    p10_3 = get_pow10(i)
    #diff = min(abs(p10_1 - p10_3), abs(p10_2 - p10_3))

    #p10 = (p10_1 if alpha3==64 else p10_2 ) + 1
    #p10_4 = p10_1 << (1 - alpha4)
    #p10_4_nr = p10_1_nr << (1 - alpha4)
    #diff = p10_4 - p10_3
    #diff_nr = p10_4_nr - p10_3
    #min_diff = min(abs(diff), abs(diff_nr))
    #print("i =",i,"base =",base,"pow5_offset =",pow5_offset,"p10_4 =",hex(p10_4),"p10_3 =",hex(p10_3),"diff =",diff,"diff_nr =",diff_nr,"min_diff = ",min_diff,"alpha4 =",alpha4)
    #print("i =",i,"p10 =",hex(p10),"p10_3 =",hex(p10_3),"diff =",diff)
    #print(i,alpha,clz,alpha + clz)
    #if(p10_1 != p10_3 or p10_2 != p10_3):

    #if(diff > 1):
    # if(abs(p10 - p10_3) > 1):
    #     #print("error i =",i,"p10_1 =",hex(p10_1),"p10_2 =",hex(p10_2),"p10_3 =",hex(p10_3))
    #     #print("er i =",i,alpha,alpha2)
    #     pass
    # else:
    #     #print("ok i =",i,"p10_1 =",hex(p10_1),"p10_2 =",hex(p10_2),"p10_3 =",hex(p10_3))
    #     #print("ok i =",i,alpha,alpha2,base,off)
    #     pass
    return p10_1


for i in range(-292-1,324,1):
    get_pow10_use_pow_5(i)

ratio = 27
start_base = math.floor(-293 / ratio) #-11
end_base = math.floor(323 / ratio) #11
#print("start_base =",start_base,"end_base =",end_base)
#
#print pow10_base lookup table
for base in range(start_base, end_base+1):
    pow10_base = get_pow10(base * ratio)
    hi = pow10_base >> 64
    lo = pow10_base & ((2**64) - 1)
    #print("0x{:016x}, 0x{:016x},".format(hi, lo), "// e10 = ", base,"*",ratio,"=",base*ratio)

#print pow5 lookup table
for i in range(0,ratio+1):
    p5 = 5**i
    clz = (64 - p5.bit_length())
    p5_rlz = p5 << clz
    pow5_rlz.append(p5_rlz)
    #print("0x{:016x},// = (5**{:2d}) << clz(5**{:2d}) = (5**{:2d}) << {:2d} ; e10 = {} ".format(p5_rlz,i,i,i,clz,i))

# for i in range(0,14):
#     p5_rlz = pow5_rlz[i*2]
#     p5_rlz_next = p5_rlz * 5

# for i in range(-292-1,324,1):
#     get_pow10_use_pow_10(i)

for i in range(0,700,1):
    base = math.floor(i / ratio)
    base2 = i // ratio
    #base3 = math.floor(i * (1.0/27) )
    base3 = (i * 1214) >> 15
    if(base != base2 or base != base3):
        print("error i =",i,"base =",base,"base2 =",base2,"base3 =",base3)


# q = -28 * math.log2(10)
# q2 = -1 * math.log2(10)


#print("q =",q)

# def get_pow10_float(i):
#     e10 = int(abs(i))
#     e2 = int(abs(math.floor(i*log2_10) - 63))
#     round_up = 0 if(i>=0 and i<=27) else 1
#     if(i<0):
#         num = (2**e2//10**e10)
#     elif(i >= 20):
#         num = 10**e10//2**e2
#     else:
#         num = 10**e10*2**e2
#     result = num + round_up
#     return result

# start = -32 // 16
# # -2,-1,0,1,2
# print("start =",start)
# for i in range(-32,45,1):
#     base = (((i+32) // 16) - 2 ) * 16
#     off = i - base
#     p5 = 5**off
#     clz = (64 - p5.bit_length())
#     p5_rlz = p5 << clz
#     pow10_f_base = get_pow10_float(base)
#     pow10_f = get_pow10_float(i)
#     #print("pow10_f[",i,"] =",hex(pow10_f),base,off,i)
#     alpha = ((i*217707) >> 16) - ((base*217707) >> 16) - ((off*217707) >> 16) + 63
#     shift = ( ((i*217707) >> 16) - ((base*217707) >> 16) - off)
#     pow10_calc = (pow10_f_base * p5_rlz) >> alpha
#     pow10_calc2 = (pow10_f_base * p5) >> (alpha - clz)
#     pow10_calc3 = (pow10_f_base * p5) >> shift
#     if(abs(pow10_calc3 - pow10_f) > 1):
#         #print("error i =",i,"pow10_calc =",hex(pow10_calc),"pow10_f =",hex(pow10_f))
#         pass
#     else:
#         #print("ok i =",i,"pow10_calc =",hex(pow10_calc),"pow10_f =",hex(pow10_f),"base =",base,"off =",off,"alpha =",alpha,"shift =",shift)
#         pass

# for i in range(-2,3):
#     base = (i) * 16
#     pow10_base_f = get_pow10_float(base)
#     print("0x{:016x},".format(pow10_base_f), "// e10 = ", base)

error_sum = 0
for q in range(1 - 1075, 2046 - 1075 + 1, 1):
    k_regular = (q * 315653) >> 20 # = math.floor(q*math.log10(2))
    k_irregular = (q * 315653 - 131237) >> 20 # = math.floor(q*math.log10(2) - math.log10(4/3) )
    get_e10 = - k_regular - 1
    h = q + ((get_e10 * 217707) >> 16)
    base = (get_e10 - (-11) * 27) // 27
    pow5_off = get_e10 - (base + (-11)) * 27
    alpha = ((((base + (-11))*27) * 217707 ) >> 16) + ((pow5_off * 217707) >> 16)
    diff = ((get_e10 * 217707) >> 16) - alpha # 0 or 1
    h2 = 1 + alpha + q - (1 - diff)
    h3 = 1 + alpha + q
    # if(h != h2):
    #     print("error: q =",q,"h =",h,"h2 =",h2)
    # if(-h2 < 0):
    #     print("error h2 > 0 : q =",q,"exp =",q+1075,"-h3 =",-h2,"diff =",diff)
    if(-h3 < 0):
        error_sum = error_sum + 1
        print("error", error_sum, " h3 > 0 : q =",q,"exp =",q+1075,"-h3 =",-h3)
    # if(q + alpha == 0):
    #     print("q + alpha=0, q =",q,"exp =",q+1075)
if(error_sum == 0):
    print("no error")

for p5_off in range(0,28,1):
    #this loop no output
    p5 = 5**p5_off
    clz = 64 - p5.bit_length()
    clz2 = 63 - ((p5_off * 217707) >> 16) + p5_off
    clz3 = 63 - ((p5_off * 149) >> 6)
    if(clz != clz2 or clz !=clz3):
        print("error p5_off =",p5_off,"clz =",clz,"clz2 =",clz2,"clz3 =",clz3)


# for q in range(1-150,254-150 + 1,1):
#     #k1 = (q * 315653) >> 20
#     k1 = (q * 315653 - 131237) >> 20
#     #k1 = math.floor(q*math.log10(2) - math.log10(4/3) )
#     get_e10 = - k1 - 1
#     h = q + ((get_e10 * 217707) >> 16)
#     base = (get_e10 + 297) // 27
#     pow5_off = get_e10 - (base + (-11)) * 27
#     alpha = ((((base + (-11))*27) * 217707 ) >> 16) + ((pow5_off * 217707) >> 16)
#     diff = ((get_e10 * 217707) >> 16) - alpha # 0 or 1
#     h2 = 1 + alpha + q - (1 - diff)
#     h3 = 1 + alpha + q
#     print("q =",q,"diff =",diff)
#     if(h != h2):
#         print("error: q =",q,"h =",h,"h2 =",h2)
#     if(-h2 < 0):
#         print("error h2 > 0 : q =",q,"exp =",q+150,"-h3 =",-h2,"diff =",diff)
#     if(-h3 < 0):
#         print("error h3 > 0 : q =",q,"exp =",q+150,"-h3 =",-h3,"diff =",diff)
#         pass

    # if(q + alpha == 0):
    #     print("q + alpha=0, q =",q,"exp =",q+1075)

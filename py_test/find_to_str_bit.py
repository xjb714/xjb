for i in range(0,16):
    p10 = 10**i
    bit=52
    while(2**52 < p10 * 2**bit):
        bit -= 1
    print(bit,end=",")
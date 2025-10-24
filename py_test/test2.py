import math
up = -1 # up = -1 or 1
###     up = -1   check 2c-1
###     up = 1    check 2c+1


all_float_ok=1
print("check float : ")
for q in range(2,34):
    k=math.floor(q*math.log10(2))
    K=math.floor((-k-1)*math.log2(10))
    a = 2**(q+35-k)
    b = 5**(k+1)
    f = 2**(63 - K)
    d = (f // (10**(k+1)) ) + 1
    a2=2**(q+34-k)
    e=10**(k+1)
    

    c_max = 2**24 - 1
    c_min = 2**23 + 1
    
    c_max2=2**25 - 1
    c_min2=2**24 + 1

    t = c_max2 // b
    if(t % 2 == 0): # t must be odd
        t -= 1 
    #find max t
    while(1):
        c = (t*b  - up) // 2
        if( c<=c_max and c>=c_min  and ( (2*c+up) % b == 0 ) and (2*c+up)>=c_min2 and (2*c+up)<=c_max2):
            break
        t -= 2

    if( c>c_max or c<c_min):
        print("q =",q,"error , c not in [c_min,c_max] ")
    if( (2*c+up) % b != 0 ):
        print("q =",q,"error in c")

    left = c * a * d * e - c * a * f + (a2%b)  * f 
    right = b  * f 
    print("q =",q,"t =",t,"c=",c,";", left ,"<", right, "is", left < right)
    if( not (left < right)):
        all_float_ok=0

# double
all_double_ok=1
print("\ncheck double : ")
for q in range(2,77):
    k=math.floor(q*math.log10(2))
    K=math.floor((-k-1)*math.log2(10))
    a = 2**(q+63-k)
    b = 5**(k+1)
    f = 2**(127 - K)
    d = (f // 10**(k+1)) + 1
    a2=2**(q + 62 - k)
    e=10**(k+1)
    
 
    c_max = 2**53 - 1
    c_min = 2**52 + 1

    c_max2=2**54 - 1
    c_min2=2**53 + 1

    t = c_max2 // b
    if(t % 2 == 0): # t must be odd
        t -= 1 
    #find max t
    while(1):
        c = (t*b - up) // 2
        if( c<=c_max and c>=c_min  and ( (2*c + up) % b == 0 ) and (2*c + up)>=c_min2 and (2*c + up)<=c_max2):
            break
        t -= 2

    if( c>c_max or c<c_min):
        print("q =",q,"error , c not in [c_min,c_max] ")
    if( ((2*c+up) % b) != 0 ):
        print("q =",q,"error in c")
    
    left = c * a * d * e - c * a * f + (a2%b)  * f 
    right = b  * f 
    print("q =",q,"t =",t,"c =",c,"k+1 =",k+1,";", left ,"<", right, "is", left < right)
    if( not (left < right)):
        all_double_ok=0

if(all_float_ok):
    print("\nAll float tests passed")
else:
    print("\nSome float tests failed")
if(all_double_ok):
    print("All double tests passed")
else:
    print("Some double tests failed")
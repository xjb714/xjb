for exp in range(0,255):
    exp_bin = exp - 150 if exp>0 else 1-150
    k1 = (exp_bin * 315653) >> 20
    k2 = (exp_bin * 315653 - 131237) >> 20
    h1 = exp_bin + ((k1 * -217707 + (-217707)) >> 16)
    h2 = exp_bin + ((k2 * -217707 + (-217707)) >> 16)
    nh1 = -h1
    nh2 = -h2
    #print(f"exp={exp}, h1={nh1}, h2={nh2}")
    r = h1 + 37
    print(f"{r},",end="")
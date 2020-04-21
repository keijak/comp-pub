a, b = map(int, input().split())


def cum_xor(x):
    if x < 0:
        return 0
    if x < 4:
        t = [0, 1, 3, 0]
        return t[x]
    p = 1
    while (p << 1) <= x:
        p <<= 1
    msb = p * ((x - p + 1) & 1)
    return msb | cum_xor(x - p)


print(cum_xor(b) ^ cum_xor(a - 1))

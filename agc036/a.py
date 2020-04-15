import math
S = int(input())
L = 10**9

def search():
    p = int(math.sqrt(S))
    q = p
    i = 0
    while p*q < S:
        if i % 2 == 0 or q == L: p += 1
        if i % 2 == 1 or p == L: q += 1
        i += 1
    assert p <= L and q <= L
    target = p*q - S
    assert target <= L
    divisor = 1
    return (0, 0, p, divisor, target, q)

print(*search())
import sys
import math

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]


def solve():
    p, q, b = inl()
    if p == 0:
        return True
    g = math.gcd(p, q)
    p //= g
    q //= g
    if q == 1:
        return True
    return pow(b, 64, q) == 0


n = ini()
for i in range(n):
    print(["Infinite", "Finite"][solve()])

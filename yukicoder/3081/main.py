import math


def solve():
    n = int(input())
    s = input()
    if "H" in s:
        return -1
    m = s.count("Q")
    if m == 0:
        return -1
    r = math.isqrt(m)
    if r * r != m:
        return -1
    if n % r != 0:
        return -1
    p = n // r
    t = s[:p]
    if s != t * r:
        return -1
    return t


print(solve())

import sys
import math


def solve():
    n, m = [int(x) for x in sys.stdin.readline().split()]
    s = sys.stdin.readline().rstrip()
    t = sys.stdin.readline().rstrip()
    l = n // math.gcd(n, m) * m
    a = {}
    dn = l // n
    for i in range(n):
        j = i * dn
        a[j] = s[i]
    dm = l // m
    for i in range(m):
        j = i * dm
        if j in a and a[j] != t[i]:
            return -1
    return l


print(solve())

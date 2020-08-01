import sys
import math

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
T = [ini() for _ in range(n)]


def solve():
    a = T[0]
    for t in T[1:]:
        g = math.gcd(a, t)
        a = g * (a // g) * (t // g)
    return a


print(solve())

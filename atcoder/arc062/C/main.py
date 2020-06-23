import sys
from fractions import gcd

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print(*a, **dict(file=sys.stderr, **kw))

N = ini()


def solve():
    p = q = 1
    for i in range(N):
        x, y = inm()
        k = max((p + x - 1) // x, (q + y - 1) // y)
        p = x * k
        q = y * k
    return p + q


print(solve())

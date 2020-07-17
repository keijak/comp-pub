import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()
C = Counter(A)


def choose2(k):
    return k * (k - 1) // 2


def solve():
    total = 0
    for x, k in C.items():
        total += choose2(k)

    for i in range(N):
        x = A[i]
        a = total - choose2(C[x]) + choose2(C[x] - 1)
        print(a)


solve()

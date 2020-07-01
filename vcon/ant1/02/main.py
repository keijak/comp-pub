import sys
import math

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
P = [None] * N
for i in range(N):
    x, y = inm()
    P[i] = (x, y)


def solve():
    md = 0
    for i in range(N):
        x1, y1 = P[i]
        for j in range(i):
            x2, y2 = P[j]
            dd = (x1 - x2) ** 2 + (y1 - y2) ** 2
            if md < dd:
                md = dd
    return math.sqrt(md)


print(solve())

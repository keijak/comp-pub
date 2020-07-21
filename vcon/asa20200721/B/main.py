import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

s = ins()
n = ini()


def solve():
    t = list(s)
    for i in range(n):
        l, r = inm()
        l -= 1
        t[l:r] = reversed(t[l:r])
    return "".join(t)


print(solve())

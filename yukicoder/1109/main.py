import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
T = set(inl())
M = 12


def solve():
    ans = set()
    for d in range(12):
        s = {(d + i) % 12 for i in [0, 2, 4, 5, 7, 9, 11]}
        if T.issubset(s):
            ans.add(d)
    if len(ans) == 1:
        return list(ans)[0]
    return -1


print(solve())

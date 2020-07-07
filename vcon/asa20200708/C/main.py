import sys

import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
D = inl()
M = ini()
T = inl()


def solve():
    cd = collections.Counter(D)
    ct = collections.Counter(T)
    for t, cnt in ct.items():
        if cd[t] >= cnt:
            cd[t] -= cnt
        else:
            return False
    return True


print("YES" if solve() else "NO")

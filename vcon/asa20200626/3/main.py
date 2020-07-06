import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()


def solve():
    s1 = set()
    sn = set()
    for i in range(M):
        a, b = inm()
        if a == 1:
            s1.add(b)
        if b == 1:
            s1.add(a)
        if a == N:
            sn.add(b)
        if b == N:
            sn.add(a)
    return bool(s1 & sn)


print("POSSIBLE" if solve() else "IMPOSSIBLE")

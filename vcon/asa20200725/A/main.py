import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
T = inl()
M = ini()


def solve():
    tsum = sum(T)
    for i in range(M):
        p, x = inm()
        p -= 1
        ans = tsum - T[p] + x
        print(ans)


solve()

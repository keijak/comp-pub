import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, K = inm()
P = inl()


def solve():
    ex = 0.0
    for i in range(K):
        ex += (P[i] + 1) / 2
    mex = ex
    for i in range(K, N):
        j = i - K
        ex += (P[i] + 1) / 2
        ex -= (P[j] + 1) / 2
        if mex < ex:
            mex = ex
    return mex


print(solve())

import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


INF = 10 ** 10


def solve():
    n = ini()
    K = inl()
    K = [INF] + K + [INF]
    L = [None] * n
    for i in range(n):
        L[i] = min(K[i], K[i + 1])
    return L


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(*solve())

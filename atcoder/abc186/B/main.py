import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    h, w = inl()
    A = [inl() for _ in range(h)]
    minv = 10 ** 9
    for i in range(h):
        m = min(A[i])
        minv = min(minv, m)
    ans = 0
    for i in range(h):
        for j in range(w):
            ans += A[i][j] - minv
    return ans


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve())

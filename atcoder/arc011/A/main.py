import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    m, n, N = inl()
    ans = N
    x = N
    while x >= m:
        q = x // m
        r = x % m
        ans += q * n
        x = r + q * n
    return ans


try:
    import frosch

    frosch.hook()
except ImportError:
    pass

print(solve())

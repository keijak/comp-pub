import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    a = inl()
    sa = 0
    ta = 0
    for i in range(n):
        sa += a[i] * a[i]
        ta += a[i]
    sa *= 2 * n
    sa -= 2 * ta * ta

    return sa // 2


print(solve())

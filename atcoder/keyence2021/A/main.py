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
    b = inl()
    am, abm = 0, 0
    c = [None] * n
    for i in range(n):
        am = max(am, a[i])
        abm = max(abm, b[i] * am)
        c[i] = abm
    return c


print(*solve(), sep="\n")

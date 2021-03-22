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


def f(x, y):
    q = y // x
    r = y % x
    t = q * 2 * x
    if r == 0:
        return t - x
    return t + f(r, x)


def solve():
    n, x = inl()
    total = n
    y = n - x
    if x > y:
        x, y = y, x
    total += f(x, y)
    return total


print(solve())

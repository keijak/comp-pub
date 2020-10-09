import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    n %= 12
    x = (60 * n + m) * 0.5
    y = m * 6.0
    z = abs(x - y)
    return min(z, 360 - z)


print(solve())

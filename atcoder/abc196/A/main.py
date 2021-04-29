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
    a, b = inl()
    c, d = inl()
    ans = -1000000
    for x in range(a, b + 1):
        for y in range(c, d + 1):
            ans = max(ans, x - y)

    return ans


print(solve())
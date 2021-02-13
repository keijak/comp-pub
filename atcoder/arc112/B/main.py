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
    b, c = inl()
    if c == 1:
        return 2 if b != 0 else 1

    if b <= 0:
        l1 = b - c // 2
        h1 = b + (c - 2) // 2
        l2 = -b - (c - 1) // 2
        h2 = -b + (c - 1) // 2
    else:
        l2 = b - c // 2
        h2 = b + (c - 2) // 2
        l1 = -b - (c - 1) // 2
        h1 = -b + (c - 1) // 2

    if h1 < l2:
        return (h2 - l2 + 1) + (h1 - l1 + 1)
    else:
        return h2 - l1 + 1


print(solve())

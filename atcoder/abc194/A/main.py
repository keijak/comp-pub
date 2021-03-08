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
    c = a + b

    if c >= 15 and b >= 8:
        return 1
    if c >= 10 and b >= 3:
        return 2
    if c >= 3:
        return 3
    return 4


print(solve())

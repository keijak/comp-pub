import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def dsum(x):
    r = 0
    while x:
        r += x % 10
        x //= 10
    return r


def solve():
    a, b = inl()
    return max(dsum(a), dsum(b))


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve())

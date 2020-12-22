import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    k = ini()
    k += 1
    a, b = 1, 0
    while k:
        a, b = a + b, a
        k -= 1
    return (a, b)


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(*solve())

import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x, y = [int(x) for x in sys.stdin.readline().split()]
    if x == y:
        return x
    return 3 - x - y


print(solve())

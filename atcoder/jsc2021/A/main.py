import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x, y, z = [int(x) for x in sys.stdin.readline().split()]
    a = y * z
    if a % x == 0:
        return a // x - 1
    else:
        return a // x

    return


print(solve())
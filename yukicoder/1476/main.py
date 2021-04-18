import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


MOD = 998244353


def solve():
    n = int(sys.stdin.readline())
    return pow(6, n // 2, MOD)


print(solve())

import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a = [int(x) for x in sys.stdin.readline().split()]
    ans = sum(7 - x for x in a)
    return ans


print(solve())

import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b, c = [int(x) for x in sys.stdin.readline().split()]
    return a * a + b * b < c * c


print(["No", "Yes"][solve()])

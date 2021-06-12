import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x, y = 20, 80
    for i in range(11):
        x, y = x + y, x
        debug(f"{i=}: {x=} {y=}")


solve()

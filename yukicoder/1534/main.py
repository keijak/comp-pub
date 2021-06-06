import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    S = sys.stdin.readline().rstrip()
    p = S.find("pain")
    if p < 0:
        return -1
    c = S[:p].count("pon")
    if c < 2:
        return -1
    return c - 1


print(solve())

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
    s = sum(a)
    if 3 * a[0] == s or 3 * a[1] == s or 3 * a[2] == s:
        return "Yes"
    return "No"


print(solve())

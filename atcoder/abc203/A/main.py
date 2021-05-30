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
    a.sort()
    s = sum(a)
    if a[0] == a[1]:
        return a[2]
    if a[1] == a[2]:
        return a[0]
    if a[0] == a[2]:
        return a[1]
    return 0


print(solve())

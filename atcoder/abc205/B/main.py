import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = int(sys.stdin.readline())
    a = [int(x) for x in sys.stdin.readline().split()]
    s = set()
    assert len(a) == n
    for x in a:
        s.add(x)
    t = set(x for x in range(1, n + 1))
    return s == t


print(["No", "Yes"][solve()])

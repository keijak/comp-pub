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
    xs = []
    for i in range(n):
        s, t = sys.stdin.readline().split()
        t = int(t)
        xs.append((t, s))
    xs.sort(reverse=True)
    return xs[1][1]


print(solve())

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
    BIG = 1e18
    ans = BIG
    for i in range(n):
        a, p, x = [int(x) for x in sys.stdin.readline().split()]
        if x > a:
            ans = min(ans, p)
    return ans if ans != BIG else -1


print(solve())

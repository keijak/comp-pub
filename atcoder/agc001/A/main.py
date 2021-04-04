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
    l = [int(x) for x in sys.stdin.readline().split()]
    l.sort()
    ans = 0
    for i in range(n):
        ans += l[2 * i]
    return ans


print(solve())
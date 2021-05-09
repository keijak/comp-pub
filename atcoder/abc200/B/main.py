import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, k = [int(x) for x in sys.stdin.readline().split()]
    for i in range(k):
        if n % 200 == 0:
            n //= 200
        else:
            n = n * 1000 + 200
    return n


print(solve())

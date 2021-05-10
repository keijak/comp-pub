import sys

sys.setrecursionlimit(10 ** 6)

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


MOD = 998244353


def f(x, y):
    if x == 0 or y == 0:
        return 1
    return (f(x - 1, y) + f(x, y - 1)) % MOD


def solve(d):
    return f(d - 1, d)


# d = int(sys.stdin.readline())
for d in range(1, 10):
    print(solve(d))

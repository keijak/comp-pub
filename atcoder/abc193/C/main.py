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
    pos = set()
    a = 2
    while True:
        x = a ** 2
        if x > n:
            break
        while x <= n:
            pos.add(x)
            x *= a
        a += 1
    return n - len(pos)


print(solve())

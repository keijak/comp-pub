import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

a, b = inm()


def p():
    s = a
    z = s % 10
    s //= 10
    y = s % 10
    s //= 10
    x = s % 10
    for i in range(1, 10):
        yield (100 * i + 10 * y + z) - b
    for i in range(10):
        yield (100 * x + 10 * i + z) - b
    for i in range(10):
        yield (100 * x + 10 * y + i) - b

    s = b
    z = s % 10
    s //= 10
    y = s % 10
    s //= 10
    x = s % 10
    for i in range(1, 10):
        yield a - (100 * i + 10 * y + z)
    for i in range(10):
        yield a - (100 * x + 10 * i + z)
    for i in range(10):
        yield a - (100 * x + 10 * y + i)


def solve():
    ans = -100000
    for x in p():
        if ans < x:
            ans = max(ans, x)
    return ans


print(solve())

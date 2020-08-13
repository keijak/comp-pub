import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, a, b = inl()
    if (b - a) % 2 == 0:
        return (b - a) // 2

    k = min(a - 1, n - b)
    return k + 1 + (b - a) // 2


print(solve())

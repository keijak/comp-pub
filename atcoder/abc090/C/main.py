import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()

    if n > m:
        n, m = m, n
    assert n <= m

    if n >= 2:
        return (n - 2) * (m - 2)

    if m >= 2:
        return m - 2

    assert n == 1 and m == 1
    return 1


print(solve())

import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    m -= 2 * n
    if m < 0:
        return -1, -1, -1
    for b in range(n + 1):
        e = m - 2 * b
        if e >= 0 and b + e <= n:
            a = n - b - e
            assert a + b + e == n
            assert a >= 0 and b >= 0 and e >= 0
            return a, e, b
    return -1, -1, -1


print(*solve())

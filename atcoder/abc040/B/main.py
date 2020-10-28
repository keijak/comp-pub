import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    ans = n
    for x in range(1, n + 1):
        if x * x > n:
            break
        y = n // x
        r = n - x * y
        cost = abs(x - y) + r
        ans = min(ans, cost)

    return ans


print(solve())

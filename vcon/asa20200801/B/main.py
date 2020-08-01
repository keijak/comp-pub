import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


a, b, c = inl()


def solve():
    ans = min(
        [
            abs((a // 2) - (a - a // 2)) * b * c,
            abs((b // 2) - (b - b // 2)) * a * c,
            abs((c // 2) - (c - c // 2)) * b * a,
        ]
    )
    return ans


print(solve())

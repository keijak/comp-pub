import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

X, Y = inm()
INF = 1 << 50


def solve():
    ans = INF
    for x, y, c in [(X, Y, 0), (-X, Y, 1), (X, -Y, 1), (-X, -Y, 2)]:
        if x <= y:
            ans = min(ans, c + y - x)
    return ans


print(solve())

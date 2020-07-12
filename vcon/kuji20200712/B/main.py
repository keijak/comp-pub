import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

s = ins()


def solve():
    ans = 9 * (len(s) - 1)
    x = [int(d) for d in s]
    sx = sum(x)
    ans = max(ans, sx)
    ans = max(ans, (x[0] - 1) + 9 * (len(s) - 1))
    return ans


print(solve())

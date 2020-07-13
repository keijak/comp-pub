import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

X = ins()


def solve():
    ans = sum([int(d) for d in X])
    ans = max(ans, int(X[0]) - 1 + 9 * (len(X) - 1))
    return ans


print(solve())

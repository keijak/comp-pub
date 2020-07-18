import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
p = inl()


def solve():
    ans = 1
    m = p[0]
    for i in range(1, n):
        if p[i] <= m:
            ans += 1
        m = min(m, p[i])
    return ans


print(solve())

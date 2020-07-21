import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()


def dsum(x):
    r = 0
    while x:
        r += x % 10
        x //= 10
    return r


def solve():
    ans = 1 << 60
    for a in range(1, N // 2 + 2):
        b = N - a
        ans = min(ans, dsum(a) + dsum(b))
    return ans


print(solve())

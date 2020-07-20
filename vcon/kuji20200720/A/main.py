import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
k = ini()
x = ini()
y = ini()


def solve():
    ans = min(k, n) * x
    if n > k:
        ans += (n - k) * y

    return ans


print(solve())

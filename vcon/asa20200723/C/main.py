import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, K = inm()


def solve():
    ans = 0
    for i in range(1, N + 1):
        x = i
        cnt = 0
        while x < K:
            x *= 2
            cnt += 1
        ans += 0.5 ** cnt
    return ans / N


print(solve())

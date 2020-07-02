import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    N, s, t = inm()
    a = [ini() for _ in range(N)]
    sm = 0
    cnt = 0
    for i in range(N):
        sm += a[i]
        if s <= sm <= t:
            cnt += 1
    return cnt


print(solve())

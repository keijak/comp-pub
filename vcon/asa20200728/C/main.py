import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()


def solve():
    r = [False] * N
    r[0] = True
    cnt = [1] * N
    for i in range(M):
        x, y = inm()
        x -= 1
        y -= 1
        if r[x]:
            r[y] = True
        cnt[x] -= 1
        cnt[y] += 1
        if cnt[x] == 0:
            r[x] = False

    return sum(r)


print(solve())

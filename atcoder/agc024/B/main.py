import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
P = list(map(int, sys.stdin.readlines()))
INF = 1 << 40


def solve():
    if N == 1:
        return 0

    inds = [None] * (N + 1)
    for i, x in enumerate(P):
        inds[x] = i

    streak = [1] * (N + 1)
    for k in range(2, N + 1):
        if inds[k - 1] < inds[k]:
            streak[k] = streak[k - 1] + 1
    ans = INF
    for k in range(2, N + 1):
        r = 0 if k == P[-1] else 1
        r += N - k
        l = (k - 1) - streak[k - 1]
        if ans > l + r:
            ans = l + r
        # dbg(f"{ans=} {l=} {r=}")
    return ans


print(solve())

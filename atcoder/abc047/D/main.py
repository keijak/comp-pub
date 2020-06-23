import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print(*a, **dict(file=sys.stderr, **kw))

N, T = inm()
A = inl()
assert N > 1
assert len(A) == N


def solve():
    lmin = [None] * N
    lmin[0] = A[0]
    for i in range(1, N):
        lmin[i] = min(lmin[i - 1], A[i])

    rmax = [None] * N
    rmax[N - 1] = A[N - 1]
    for i in range(N - 2, -1, -1):
        rmax[i] = max(rmax[i + 1], A[i])

    gap = -1
    gmin = set()
    for i in range(N - 1):
        g = rmax[i + 1] - lmin[i]
        if g <= 0:
            continue
        if g > gap:
            gap = g
            gmin = {lmin[i]}
            # debug(f"{gap=}: {rmax[i+1]} - {lmin[i]}")
        elif g == gap and lmin[i] not in gmin:
            gmin.add(lmin[i])
    buy = len(gmin)
    assert buy > 0
    return buy


print(solve())

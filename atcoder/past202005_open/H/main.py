import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, L = inm()
X = set(inl())
assert len(X) == N
T1, T2, T3 = inm()
INF = 1 << 50


def penalty(i):
    return T3 if i in X else 0


def solve():
    trun = [INF for _ in range(L + 1)]
    tjump = [INF for _ in range(L + 1)]
    trun[0] = 0
    for i in range(L):
        assert trun[i] < INF
        # action 1, gain distance 1 for T1 (+T3) sec.
        tt = trun[i] + T1 + penalty(i + 1)
        if trun[i + 1] > tt:
            trun[i + 1] = tt

        # action 2, gain distance 2 for T1 + T2
        tt = trun[i] + T1 // 2 + T2 // 2
        if i + 1 <= L and tjump[i + 1] > tt:
            tjump[i + 1] = tt
        tt = trun[i] + T1 + T2 + penalty(i + 2)
        if i + 2 <= L and trun[i + 2] > tt:
            trun[i + 2] = tt

        # action 3, gain distance 4 for T1 + 3*T2
        tt = trun[i] + T1 // 2 + (T2 // 2) * 3
        if i + 2 <= L and tjump[i + 2] > tt:
            tjump[i + 2] = tt
        tt = trun[i] + T1 // 2 + (T2 // 2) * 5
        if i + 3 <= L and tjump[i + 3] > tt:
            tjump[i + 3] = tt
        tt = trun[i] + T1 + T2 * 3 + penalty(i + 4)
        if i + 4 <= L and trun[i + 4] > tt:
            trun[i + 4] = tt

    return min(trun[L], tjump[L])


print(solve())

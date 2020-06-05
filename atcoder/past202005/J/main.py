import sys
import bisect

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, M = inm()
A = inl()
assert len(A) == M
INF = 1 << 40


def solve():
    best = [0] * N  # negative, ascending
    ans = []
    for i in range(M):
        yum = -A[i]
        j = bisect.bisect_right(best, yum)
        if j == N:
            ans.append(-1)
        else:
            ans.append(j + 1)
            best[j] = yum
    print(*ans, sep="\n")


solve()

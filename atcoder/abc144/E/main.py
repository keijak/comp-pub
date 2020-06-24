import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, K = inm()
A = inl()
F = inl()
A.sort()
F.sort(reverse=True)


def check(val):
    k = K
    for i in range(N):
        tmp = A[i] * F[i]
        if tmp <= val:
            continue
        dec = ((tmp - val) + F[i] - 1) // F[i]
        assert A[i] >= dec
        assert (A[i] - dec) * F[i] <= val
        k -= dec
        if k < 0:
            return False
    return True


def solve():
    ng, ok = -1, max([A[i] * F[i] for i in range(N)])
    while ok - ng > 1:
        mid = (ok + ng) // 2
        if check(mid):
            ok = mid
        else:
            ng = mid
    return ok


print(solve())

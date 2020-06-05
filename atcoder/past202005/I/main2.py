import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
Q = ini()


def solve():
    A = np.arange(N * N).reshape(N, N)
    for q in range(Q):
        qt, *ps = inm()
        if qt == 3:
            A = A.transpose()
            continue
        a, b = ps[0] - 1, ps[1] - 1
        if qt == 1:
            v = A[a, :].copy()
            A[a, :] = A[b, :]
            A[b, :] = v
        elif qt == 2:
            v = A[:, a].copy()
            A[:, a] = A[:, b]
            A[:, b] = v
        else:
            assert qt == 4
            print(A[a, b])


solve()

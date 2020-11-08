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
    rows = np.arange(N)
    cols = np.arange(N)
    trans = False
    # A = np.arange(N * N).reshape(N, N)
    for q in range(Q):
        # dbg(f"# rows = {rows}")
        # dbg(f"# cols = {cols}")
        # for i in range(N):
        #     dbg(">>", end=" ")
        #     for j in range(N):
        #         dbg(A[rows[i], cols[j]], end="\n" if j == N - 1 else " ")
        # dbg("-" * 80)

        qt, *ps = inm()
        if qt == 3:
            trans = not trans
            # A = A.transpose()
            continue
        a, b = ps[0] - 1, ps[1] - 1
        if qt == 1:
            v = cols if trans else rows
            v[a], v[b] = v[b], v[a]
        elif qt == 2:
            v = rows if trans else cols
            v[a], v[b] = v[b], v[a]
        else:
            assert qt == 4
            if trans:
                a, b = b, a
            print(N * rows[a] + cols[b])


solve()

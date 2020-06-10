import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
A = inl()
assert len(A) == N


def solve():
    if N == 1:
        return True
    asum = sum(A)
    k = N * (N + 1) // 2
    if asum % k != 0:
        return False
    t = asum // k  # how many times we operate

    B = [None] * N
    for i in range(N - 1):
        B[i] = A[i + 1] - A[i]
    B[N - 1] = A[0] - A[N - 1]
    if sum(B) != 0:
        return False
    bmax = max(B)
    B.sort()
    s = 0
    for b in B:
        if (bmax - b) % N != 0:
            return False
        s += (bmax - b) // N
    return s == t


print("YES" if solve() else "NO")

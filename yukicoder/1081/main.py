import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
A = inl()
assert len(A) == N

M = 10 ** 9 + 7


def solve():
    global A
    for i in range(N - 1):
        b = [None] * N
        for j in range(N - i - 1):
            b[j] = A[j] + A[j + 1]
            b[j] %= M
        A = b
    return A[0]


print(solve())

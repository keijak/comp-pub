import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
A = inl()


def solve():
    ans = 0
    for i in range(N):
        for j in range(i):
            v = A[i] ^ A[j]
            if ans < v:
                ans = v

    return ans


print(solve())

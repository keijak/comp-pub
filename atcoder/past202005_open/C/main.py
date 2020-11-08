import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)


A, R, N = inm()
L = 10 ** 9


def solve():
    a = A
    for i in range(N - 1):
        a *= R
        if a > L:
            return "large"
    return a


print(solve())

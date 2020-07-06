import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()
A.sort()


def solve():
    big = A[-1]
    small = A[0]
    res = []
    for i in range(1, N - 1):
        if A[i] > 0:
            res.append((small, A[i]))
            small -= A[i]
        else:
            res.append((big, A[i]))
            big -= A[i]
    res.append((big, small))
    print(big - small)
    for x, y in res:
        print(x, y)


solve()

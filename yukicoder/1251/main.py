import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    A = inl()
    B = inl()

    P = dict()
    for i in range(n):
        if A[i] not in P:
            P[A[i]] = B[i]
        else:
            P[A[i]] += B[i]
    P = [(k, v) for k, v in P.items()]
    P.sort()
    n = len(P)

    if n == 1:
        return A[0], 0

    cum = [None] * (n + 1)
    cum[0] = 0
    for i in range(n):
        cum[i + 1] = cum[i] + P[i][1]

    mp = n
    for i in range(n):
        if cum[i + 1] > cum[n] - cum[i + 1]:
            mp = i
            break

    def f(x):
        res = 0
        for i in range(len(A)):
            res += B[i] * abs(x - A[i])
        return res

    if mp == n:
        y = max(A)
        return y, int(f(y) + 0.5)

    return P[mp][0], int(f(P[mp][0]) + 0.5)


print(*solve())

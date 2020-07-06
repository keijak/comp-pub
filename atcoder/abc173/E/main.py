import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, K = inm()
A = inl()
B = [(abs(a), a < 0) for a in A]
B.sort(reverse=True)

MOD = 10 ** 9 + 7


def solve():
    fs = []
    negmin = posrep = None
    posmin = negrep = None
    negcnt = 0
    for i in range(N):
        v, neg = B[i]
        if len(fs) < K:
            fs.append(v)
            if neg:
                negcnt += 1
                negmin = v
            else:
                posmin = v
        elif neg and negrep is None:
            negrep = v
        elif not neg and posrep is None:
            posrep = v
    if negcnt % 2 == 0:
        a0 = 1
        for x in fs:
            a0 = (a0 * x) % MOD
        return a0

    a1 = a2 = None
    if negmin is not None and posrep is not None:
        a1 = 1
        replaced = False
        for x in fs:
            if x == negmin and not replaced:
                a1 = (a1 * posrep) % MOD
                replaced = True
            else:
                a1 = (a1 * x) % MOD
    if posmin is not None and negrep is not None:
        a2 = 1
        replaced = False
        for x in fs:
            if x == posmin and not replaced:
                a2 = (a2 * negrep) % MOD
                replaced = True
            else:
                a2 = (a2 * x) % MOD

    if a1 is None and a2 is None:
        a3 = 1
        sign = 1
        for x, neg in B[N - K :]:
            a3 = (a3 * x) % MOD
            if neg:
                sign *= -1
        return (MOD + sign * a3) % MOD

    if a1 is None:
        return a2
    if a2 is None:
        return a1

    if posrep * posmin < negrep * negmin:
        return a2
    else:
        return a1


print(solve())

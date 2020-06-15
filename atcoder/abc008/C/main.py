import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
C = [ini() for _ in range(N)]


def solve():
    C.sort()
    nfactors = [0] * N
    for i in range(N):
        count = 0
        for j in range(N):
            if i != j and C[i] % C[j] == 0:
                count += 1
        nfactors[i] = count
    prob = [None] * N
    for i in range(N):
        # num: choose(n, f[i]+1)* (n-f[i]-1)! * (f[i]!) * ((f[i]+1)//2)
        # den: n!
        num = 1
        for j in range(1 + nfactors[i]):
            num *= N - j
        den = 1 + nfactors[i]
        for j in range(N - nfactors[i], N + 1):
            den *= j
        num *= 1 + nfactors[i] // 2
        prob[i] = num / den
    return sum(prob)


print(solve())

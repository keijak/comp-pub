import sys
import collections

sys.setrecursionlimit(10**6)
N, M = map(int, input().split())

def factor(m):
    fac = collections.Counter()
    while (m&1) == 0:
        m >>= 1
        fac[2] += 1
    for p in range(3, m, 2):
        if p*p > m:
            break
        while m % p == 0:
            m //= p
            fac[p] += 1
    if m > 1:
        fac[m] += 1
    return fac

fac = factor(M)

def search(i, ps, divisor):
    if M < N * divisor:
        return 1
    if i == len(ps):
        assert M % divisor == 0
        return divisor
    p, cnt = ps[i]
    dmax = 1
    for j in range(cnt+1):
        dmax = max(dmax, search(i+1, ps, divisor))
        divisor *= p
    return dmax

print(search(0, list(fac.items()), 1))

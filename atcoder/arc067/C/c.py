from collections import Counter
M = 10**9 + 7
N = int(input())

prime = [True] * (N+1)
prime[0] = False
prime[1] = False
for i in range(4, N+1, 2):
    prime[i] = False
for i in range(3, N+1, 2):
    if prime[i]:
        for j in range(2*i, N+1, i):
            prime[j] = False

primes = [i for i, b in enumerate(prime) if b]

def fact(k, pdict):
    a = k
    for p in primes:
        if p*p > k:
            break
        c = 0
        while a % p == 0:
            a //= p
            c += 1
        pdict[p] += c
    if a > 1:
        pdict[a] += 1

def solve():
    pdict = Counter()
    for k in range(2, N+1):
        fact(k, pdict)

    ans = 1
    for v in pdict.values():
        ans = ans * (v+1) % M
    return ans


print(solve())

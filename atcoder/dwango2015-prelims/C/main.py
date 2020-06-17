import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()


def fact(n, facts=[1, 1, 2]):
    while len(facts) <= n:
        facts.append(facts[-1] * len(facts))
    return facts[n]


def comb(n, k, combs={}):
    if k > n - k:
        k = n - k
    if k == 0:
        return 1
    if k == 1:
        return n
    if (n, k) in combs:
        return combs[n, k]
    num = den = 1
    for i in range(k):
        num *= n - i
        den *= i + 1
    res = num // den
    combs[n, k] = res
    return res


dp = [None] * (N + 1)
dp[1] = 0  # one person, expected to finish in 0 rounds.


def expectation(n):
    if n <= 0:
        raise ValueError(n)
    if dp[n] is not None:
        return dp[n]
    base = 0.0
    for j in range(1, n):
        x1 = 0
        # Two kinds, only one minority.
        if 2 * j < n:  # j, (n-j)
            x1 += 3 * 2 * comb(n, j)
        # Three kinds, Only one minority. j < k
        for k in range(j + 1, n - 2 * j):
            x1 += 3 * comb(n, j) * comb(n - j, k)
        # Two minorities.
        if 2 * j == n:  # j,j,0
            x1 += 3 * comb(n, j)
        if n > 3 * j:  # j,j,n-2*j
            x1 += 3 * comb(n, j) * comb(n - j, j)
        base += x1 * (1.0 + expectation(j))
    base /= 3 ** n

    # n -> n (no change)
    p_nc = (1.0 / 3) ** (n - 1)  # all same
    if n % 3 == 0:
        k = n // 3
        p_nc += fact(n) / ((fact(k) ** 3) * (3 ** n))
    res = (base + p_nc) / (1.0 - p_nc)
    dp[n] = res
    return res


print(expectation(N))

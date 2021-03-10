import sys

inl = lambda: [int(x) for x in sys.stdin.readline().split()]
primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
M = len(primes)
MOD = 10 ** 9 + 7


def comb_table(n, p=MOD):
    C = [[0] * (n + 1) for _ in range(n + 1)]
    for i in range(n + 1):
        C[i][0] = C[i][i] = 1
    for i in range(1, n + 1):
        for j in range(1, i):
            C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % p
    return C


def solve():
    n, k = inl()
    a = inl()
    C = comb_table(n)
    count = [0] * (2 ** M)
    for x in a:
        y = 0
        for i in range(M):
            if x % primes[i] == 0:
                y |= 1 << i
        sub = y
        while sub:
            count[sub] += 1
            sub = (sub - 1) & y

    ans = C[n][k]
    for bits in range(1, 2 ** M):
        if count[bits] < k:
            continue
        popcnt = 0
        x = bits
        while x:
            if x & 1:
                popcnt += 1
            x >>= 1
        c = C[count[bits]][k]
        if popcnt & 1:
            ans -= c
            ans %= MOD
        else:
            ans += c
            ans %= MOD
    return ans


print(solve())

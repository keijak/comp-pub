#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines
N, M, K = map(int, readline().split())
MOD = 998244353

# Combination under a prime modulo: nCk % p
def combmod(n, k, p=MOD):
    if k > n:
        return 0
    if k > n - k:
        k = n - k
    num = den = 1
    for i in range(k):
        num = num * (n - i) % p
        den = den * (i + 1) % p
    # By Fermat's little theorem,
    # a^(p-1) ≡ 1 mod p
    # i.e. a^(p-2) ≡ a^(-1) mod p
    inv_dev = pow(den, p - 2, p)
    return num * inv_dev % p


def solve():
    m1pows = [1 for i in range(K + 1)]
    m1pows[0] = M * pow(M - 1, N - 1 - K, MOD) % MOD
    for i in range(1, K + 1):
        m1pows[i] = m1pows[i - 1] * (M - 1) % MOD
    cs = 1
    ans = 0
    for i in range(K + 1):
        ans += cs * m1pows[K - i]
        ans %= MOD
        cs *= (N - 1 - i) * pow(i + 1, MOD - 2, MOD)
        cs %= MOD
    return ans


if __name__ == "__main__":
    print(solve())

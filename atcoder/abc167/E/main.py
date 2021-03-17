#!/usr/bin/env python3
import sys
import collections

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines
N, M, K = map(int, readline().split())
MOD = 998244353

# Combination under a modulo: nCk % mod
# Modulo must be a prime number.
# Intermediate numerators and denominators are cached and reused.
def combmod(n, k, p=MOD, cache=collections.defaultdict(lambda: [(1, 1)])):
    if k > n:
        return 0
    if k > n - k:
        k = n - k
    # Reuse precalculated numerators and denominators.
    numden = cache[n]
    if k < len(numden):
        num, den = numden[k]
    else:
        # Resume from len(numden).
        num, den = numden[-1]
        for i in range(len(numden) - 1, k):
            num = num * (n - i) % p
            den = den * (i + 1) % p
            numden.append((num, den))
    # a^(p-1) ≡ 1 mod p (by Fermat's little theorem)
    # i.e. a^(p-2) ≡ a^(-1) mod p
    inv_dev = pow(den, p - 2, p)
    return num * inv_dev % p


def solve():
    ans = 0
    for i in range(K + 1):
        ans += combmod(N - 1, i) * M * pow(M - 1, N - 1 - i, MOD) % MOD
        ans %= MOD
    return ans


if __name__ == "__main__":
    print(solve())

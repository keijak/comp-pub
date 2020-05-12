import numpy as np

M = 10 ** 9 + 7
N = int(input())
A = list(map(int, input().split()))

zero = np.zeros(60, dtype=np.int64)
one = np.ones(60, dtype=np.int64)
pow2 = (2 ** np.arange(60, dtype=np.int64)) % M


def bitcnt(a):
    j = 0
    bits = np.zeros(60, dtype=np.int64)
    while a:
        bits[j] = a & 1
        a >>= 1
        j += 1
    return bits


ans = 0
a1 = bitcnt(A[0])
a0 = one - a1
for i in range(1, N):
    bits1 = bitcnt(A[i])
    bits0 = one - bits1
    ans += np.sum(pow2 * (a0 * bits1 % M) % M) % M
    ans %= M
    ans += np.sum(pow2 * (a1 * bits0 % M) % M) % M
    ans %= M
    a1 += bits1
    a1 %= M
    a0 += bits0
    a0 %= M

print(ans)

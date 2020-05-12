M = 10 ** 9 + 7
N, K = map(int, input().split())


def combmod(n, k):
    if k > n:
        return 0
    if k > n - k:
        k = n - k
    num, den = 1, 1
    for i in range(k):
        num = num * (n - i) % M
        den = den * (i + 1) % M
    inv_dev = pow(den, M - 2, M)
    return num * inv_dev % M


for i in range(1, K + 1):
    print(combmod(K - 1, i - 1) * combmod(N - K + 1, i) % M)

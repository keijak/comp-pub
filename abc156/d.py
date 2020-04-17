n, a, b = map(int, input().split())
M = 10**9 + 7
total = pow(2, n, M)

def comb(n, k):
    if k > n-k:
        k = n-k
    num, den = 1, 1
    for i in range(k):
        num = num * (n-i) % M
        den = den * (i+1) % M
    return num * pow(den, M-2, M) % M

print((total - 1 - comb(n,a) - comb(n,b)) % M)

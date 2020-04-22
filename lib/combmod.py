# Combination under a prime modulo: nCk % p
def combmod(n, k, p):
    if k > n:
        return 0
    if k > n - k:
        k = n - k
    num, den = 1, 1
    for i in range(k):
        num = num * (n - i) % p
        den = den * (i + 1) % p
    # By Fermat's little theorem,
    # a^(p-1) ≡ 1 mod p
    # i.e. a^(p-2) ≡ a^(-1) mod p
    inv_dev = pow(den, p - 2, p)
    return num * inv_dev % p

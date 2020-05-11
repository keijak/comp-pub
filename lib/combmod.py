# MOD = 998244353

# Combination under a modulo: nCk % p
# Modulo must be a prime number.
# Intermediate numerators and denominators are cached and reused.
def combmod(n, k, p=MOD, numden=[(1, 1)]):
    if k > n:
        return 0
    if k > n - k:
        k = n - k
    # Reuse precalculated numerators and denominators.
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

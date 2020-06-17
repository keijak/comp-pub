def fact(n, facts=[1, 1, 2]):
    """Factorial with memo."""
    while len(facts) <= n:
        facts.append(facts[-1] * len(facts))
    return facts[n]


def comb(n, k, combs={}):
    """Combination nCk with memo."""
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

def sieve(n):
    is_prime = [True] * n
    is_prime[0] = False
    is_prime[1] = False
    for i in range(4, n, 2):
        is_prime[i] = False
    for i in range(3, n, 2):
        if i*i > n:
            break
        if is_prime[i]:
            for j in range(2*i, n, i):
                is_prime[j] = False
    return is_prime

N = int(input())


def is_prime(n):
    if n % 2 == 0:
        return n == 2
    for k in range(3, n, 2):
        if k * k > n:
            break
        if n % k == 0:
            return False
    return True


def is_like_prime(n):
    if n == 1:
        return False
    if is_prime(n):
        return True
    if n % 2 == 0 or n % 10 == 5:
        return False
    dsum = 0
    while n:
        dsum += n % 10
        n //= 10
    if dsum % 3 == 0:
        return False
    return True


print("Prime" if is_like_prime(N) else "Not Prime")

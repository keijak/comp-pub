# Factorizes a number into {prime, count} pairs.
def factorize(n):
    res = []
    for k in range(2, n + 1):
        if k * k > n:
            break
        cnt = 0
        while n % k == 0:
            n //= k
            cnt += 1
        if cnt > 0:
            res.append((k, cnt))
    if n > 1:
        res.append((n, 1))
    return res

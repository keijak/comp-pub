import math
import sys

n = int(input())
B = [int(input()) for i in range(n)]


def solve():
    if n == 1:
        return 0
    if n == 2:
        if b[0] == b[1]:
            return 0
        else:
            return 1

    count = 0
    b = list(B)
    l = [None] * n
    for k in range(n + 1):
        i = (k) % n
        j = (k - 1) % n
        r = (k + 1) % n
        g = math.gcd(b[j], b[i])
        l[i] = b[j] // g * b[i]
        if k == 0:
            continue
        z = math.gcd(l[j], l[i])
        if z != b[j]:
            count += 1
            b[i] = math.gcd(b[j], b[r])
            l[i] = b[j]
            assert math.gcd(l[j], l[i]) == b[j]

    l = [None] * n
    b = list(B)
    b[0] = math.gcd(b[-1], b[1])
    count2 = 1
    for k in range(0, n):
        i = (k) % n
        j = (k - 1) % n
        r = (k + 1) % n
        g = math.gcd(b[j], b[i])
        l[i] = b[j] // g * b[i]
        if k == 0:
            continue
        z = math.gcd(l[j], l[i])
        if z != b[j]:
            count2 += 1
            q = math.gcd(b[j], b[r])
            b[i] = q
            l[i] = b[j]
            assert math.gcd(l[j], l[i]) == b[j]
    return min(count, count2)


print(solve())

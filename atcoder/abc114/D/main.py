import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())

N = ni()


def choose(n, k):
    num, den = n, 1
    for i in range(1, k):
        num *= n - i
        den *= i + 1
    return num // den


def solve():
    fs = collections.Counter()
    for n in range(2, N + 1):
        k = 2
        while k * k <= n:
            while n % k == 0:
                n //= k
                fs[k] += 1
            k += 1
        if n > 1:
            fs[n] += 1
    p74, p24, p14, p4, p2 = 0, 0, 0, 0, 0
    for k, v in fs.items():
        if v >= 74:
            p74 += 1
        if v >= 24:
            p24 += 1
        if v >= 14:
            p14 += 1
        if v >= 4:
            p4 += 1
        if v >= 2:
            p2 += 1

    a4_4_2 = p4 * (p4 - 1) // 2 * (p2 - 2)
    a14_4 = p14 * (p4 - 1)
    a24_2 = p24 * (p2 - 1)
    a74 = p74
    return a4_4_2 + a14_4 + a24_2 + a74


print(solve())

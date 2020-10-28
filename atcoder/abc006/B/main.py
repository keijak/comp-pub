import sys

ini = lambda: int(sys.stdin.readline())


MOD = 10007


def solve():
    n = ini()
    if n <= 2:
        return 0
    a = [1, 0, 0]
    for i in range(4, n + 1):
        a = [(a[0] + a[1] + a[2]) % MOD, a[0], a[1]]

    return a[0]


print(solve())

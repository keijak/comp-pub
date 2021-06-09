import sys


def ge(n, p):
    q = 1
    for i in range(n):
        q *= n
        if q >= p:
            return True
    return q >= p


def solve():
    n, m = [int(x) for x in sys.stdin.readline().split()]
    mod = 10 ** m
    res = str(pow(n, n, mod))
    if ge(n, mod):
        if len(res) < m:
            return ("0" * (m - len(res))) + res
    return res


print(solve())

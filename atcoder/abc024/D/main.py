import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

MOD = 10 ** 9 + 7


def solve():
    a = ini()
    b = ini()
    c = ini()

    n = b * (c - a) + c * (b - a)
    k = a * (c - a) + (c - a) * (b - a)
    n %= MOD
    k %= MOD
    d = (a - c) * b + a * c
    dinv = pow(d, -1, MOD)
    n *= dinv
    k *= dinv
    return ((n - k) % MOD, k % MOD)


print(*solve())

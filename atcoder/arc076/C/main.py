import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


MOD = 10 ** 9 + 7


def solve():
    n, m = inl()

    l = max(n, m) + 1
    fact = [1] * l
    for i in range(1, l):
        fact[i] = fact[i - 1] * i % MOD

    if n == m:
        return fact[n] * fact[m] * 2
    elif n == m + 1 or n + 1 == m:
        return fact[n] * fact[m]
    else:
        return 0


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve() % MOD)

import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

MOD = 10 ** 9 + 7


def solve():
    L = ini()
    a, b = 1, 2
    for i in range(1, L + 1):
        a = a * 3 + pow(b, 3, MOD)
        a %= MOD

        # b = int(bin(b)[2:])
        b = b + pow(b, 3, MOD)
        b %= MOD
    return a


print(solve())

import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

MOD = 998244353


def solve():
    a, b, c = inl()
    ans = a * (a + 1) // 2
    ans %= MOD
    ans *= b * (b + 1) // 2
    ans %= MOD
    ans *= c * (c + 1) // 2
    ans %= MOD
    return ans


print(solve())

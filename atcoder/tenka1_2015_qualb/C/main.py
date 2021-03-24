import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

L = ini()
MOD = 1000000007


def bsum(s, t):
    if s >= t:
        return 0
    return (s + t - 1) * (t - s) // 2 % MOD


def width(s, t):
    return max(t - s, 0)


def solve():
    if L < 9:
        return 0
    res = L // 3 - 2

    P = (L + 6) // 4
    s, t = 4, P
    res += bsum(s, t)
    res -= 3 * width(s, t)
    res %= MOD
    # for b in range(4, P):
    #     res += b - 3

    s, t = max(P, 4), (L - 1) // 3 + 1
    res += L * width(s, t)
    res -= 3 * bsum(s, t)
    res %= MOD
    # for b in range():
    #     res += L - 3 * b

    Q = (L + 3) // 3
    s, t = 4, Q
    res += bsum(s, t)
    res -= 3 * width(s, t)
    res %= MOD
    # for b in range(4, Q):
    #     res += b - 3

    s, t = max(Q, 4), min((L - 3) // 2 + 1, (L - 2) // 2 + 1)
    res += (L - 2) * width(s, t)
    res -= 2 * bsum(s, t)
    res %= MOD
    # for b in range(s, t):
    #     res += L - 2 - 2 * b

    return res


print(solve())

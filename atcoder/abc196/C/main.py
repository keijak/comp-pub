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


def solve():
    S = ins()
    m = len(S)
    if m % 2 == 1:
        if m == 1:
            return 0
        S = "9" * (m - 1)
        m -= 1

    h = int(S[: m // 2], 10)
    f = int(S[: m // 2], 10)
    p = int(S[m // 2 :], 10)
    if p < f:
        return max(h - 1, 0)
    return h


print(solve())

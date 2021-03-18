import sys
from collections import Counter

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
    n = ini()
    fac = Counter()
    for x in range(2, n + 1):
        k = 2
        y = x
        while y != 1:
            if y % k == 0:
                while y % k == 0:
                    fac[k] += 1
                    y //= k
            k += 1
    c = Counter()
    clis = [74, 14, 4, 24, 2]
    for p, k in fac.items():
        for x in clis:
            if k >= x:
                c[x] += 1

    ans = c[74]
    ans += c[14] * (c[4] - 1)
    ans += c[24] * (c[2] - 1)
    ans += c[4] * (c[4] - 1) // 2 * (c[2] - 2)
    return ans


print(solve())
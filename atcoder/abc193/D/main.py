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

K = ini()
S = ins()
T = ins()


def score(co):
    s = 0
    for d in range(1, 10):
        s += d * 10 ** co[d]
    return s


def solve():
    co = Counter()
    cs = Counter()
    ct = Counter()
    for d in range(1, 10):
        co[d] = K
    for i in range(4):
        x = int(S[i])
        co[x] -= 1
        cs[x] += 1
        x = int(T[i])
        co[x] -= 1
        ct[x] += 1

    rem = 9 * K - 8
    ans = 0.0
    for bs in range(1, 10):
        if co[bs] <= 0:
            continue
        ps = co[bs] / rem
        cs[bs] += 1
        ss = score(cs)
        cs[bs] -= 1
        for bt in range(1, 10):
            q = co[bt]
            if bt == bs:
                q -= 1
            if q <= 0:
                continue
            pt = q / (rem - 1)
            ct[bt] += 1
            st = score(ct)
            ct[bt] -= 1
            if ss > st:
                ans += ps * pt
    return ans


print(solve())

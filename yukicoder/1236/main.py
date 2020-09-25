import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

A, B = inl()


def solve():
    xt = []
    for h in range(25):
        if h in [11, 23]:
            continue
        if h in [0, 12, 24]:
            xt.append((h, 0))
            continue
        m = 60 * (h % 12) / 11
        xt.append((h, m))

    s = 60 * A + B
    ans = 10 ** 9
    for (h, m) in xt:
        t = h * 60 + m
        if t < s:
            continue
        ans = min(ans, int((t - s) * 60))

    return ans


print(solve())

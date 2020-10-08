import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    a = inl()

    maxt = -1e6
    maxi = -1
    for i in range(n):
        maxa = -1e6
        mj = -1
        mt = None
        for j in range(n):
            if i == j:
                continue
            pt, pa = 0, 0
            l, r = min(i, j), (max(i, j) + 1)
            for k in range(l, r):
                if (k - l) % 2 == 0:
                    pt += a[k]
                else:
                    pa += a[k]
            if maxa < pa:
                maxa = pa
                mj = j
                mt = pt
        if maxt < mt:
            maxt = mt
            maxi = i

    return maxt


print(solve())

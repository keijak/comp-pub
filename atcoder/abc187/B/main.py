import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    ps = [None] * n
    for i in range(n):
        x, y = inl()
        ps[i] = (x, y)
    ps.sort()
    ans = 0
    for i in range(n):
        xi, yi = ps[i]
        for j in range(i + 1, n):
            xj, yj = ps[j]
            if xi == xj:
                continue
            if yj - yi <= xj - xi and xi - xj <= yj - yi:
                ans += 1

    return ans


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve())

import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

W, H, N = inm()


def solve():
    xl = 0
    xr = W
    yb = 0
    yt = H
    for _ in range(N):
        x, y, a = inm()
        if a == 1:
            xl = min(max(xl, x), xr)
        elif a == 2:
            xr = max(min(xr, x), xl)
        elif a == 3:
            yb = min(max(yb, y), yt)
        else:
            assert a == 4
            yt = max(min(yt, y), yb)
    return (yt - yb) * (xr - xl)


print(solve())

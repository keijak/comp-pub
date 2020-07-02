import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()


def solve():
    tc, xc, yc = 0, 0, 0
    for i in range(N):
        t, x, y = inm()
        d = abs(xc - x) + abs(yc - y)
        if d > (t - tc):
            return False
        if ((t - tc) - d) % 2 == 1:
            return False
        tc, xc, yc = t, x, y

    return True


print("Yes" if solve() else "No")

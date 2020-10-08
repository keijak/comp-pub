import sys
import math

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    sx, sy, gx, gy, T, V = inl()
    n = ini()
    for i in range(n):
        x, y = inl()
        d1 = math.sqrt((sx - x) ** 2 + (sy - y) ** 2)
        d2 = math.sqrt((gx - x) ** 2 + (gy - y) ** 2)
        if (d1 + d2) <= V * T + 1e-9:
            return True
    return False


print("YES" if solve() else "NO")

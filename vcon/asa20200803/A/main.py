import sys
import math

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b, h, m = inl()
    t2 = 2 * math.pi * m / 60.0
    t1 = 2 * math.pi * (h * 60.0 + m) / (12 * 60.0)
    return math.sqrt(a * a + b * b - 2 * a * b * math.cos(t1 - t2))


print(solve())

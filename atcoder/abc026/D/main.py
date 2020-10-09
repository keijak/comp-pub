import sys
import math

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b, c = inl()

    def f(t):
        return a * t + b * math.sin(c * t * math.pi)

    lo, hi = 0, 101
    assert f(lo) < 100
    assert f(hi) > 100
    for iter in range(100):
        mid = (lo + hi) / 2.0
        v = f(mid)
        if v < 100:
            lo = mid
        elif v > 100:
            hi = mid
        else:
            lo = hi = mid
            break
    return (lo + hi) / 2.0


print(solve())

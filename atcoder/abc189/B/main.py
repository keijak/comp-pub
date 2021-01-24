import sys
from fractions import Fraction

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
    n, x = inl()

    z = Fraction(0)
    for i in range(n):
        v, p = inl()
        z += Fraction(v) * Fraction(p, 100)
        if z > x:
            return i + 1

    return -1


print(solve())

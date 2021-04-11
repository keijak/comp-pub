import sys
from decimal import Decimal
import math


try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    r, x, y = [int(x) for x in sys.stdin.readline().split()]
    r2 = Decimal(r * r)
    d2 = Decimal(x * x + y * y)
    if r2 > d2:
        return 2
    return math.ceil((d2 / r2).sqrt())


print(solve())
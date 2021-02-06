import sys
from fractions import Fraction
from decimal import Decimal
import math

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

D = 10000


def solve():
    X, Y, R = map(Decimal, input().split())
    X -= int(X)
    Y -= int(Y)
    X = int(X * D)
    Y = int(Y * D)
    R = int(R * D)
    R2 = R * R

    start = (X - R + 9999) // D
    end = (X + R) // D
    res = 0
    for i in range(start, end + 1):
        yd2 = R2 - (X - i * D) * (X - i * D)

        ytop_t = Y // D
        ytop_f = (Y + R + 9999) // D + 5
        while ytop_f - ytop_t > 1:
            mid = (ytop_t + ytop_f) // 2
            ym2 = mid * D - Y
            ym2 *= ym2
            if ym2 > yd2:
                ytop_f = mid
            else:
                ytop_t = mid

        ybot_t = (Y + 9999) // D
        ybot_f = (Y - R) // D - 5
        while ybot_t - ybot_f > 1:
            mid = (ybot_t + ybot_f) // 2
            ym2 = Y - mid * D
            ym2 *= ym2
            if ym2 > yd2:
                ybot_f = mid
            else:
                ybot_t = mid

        res += ytop_t - ybot_t + 1

    return res


print(solve())

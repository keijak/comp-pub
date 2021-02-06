import sys
from fractions import Fraction
import math

debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

D = 10000


def solve():
    X, Y, R = map(Fraction, input().split())

    YTOP_T = math.floor(Y)
    YTOP_F = math.ceil(Y + R) + 5
    YBOT_T = math.ceil(Y)
    YBOT_F = math.floor(Y - R) - 5
    RI = int(R * D)
    R2I = RI ** 2
    XI = int(X * D)
    YI = int(Y * D)

    start = math.ceil(X - R)
    end = math.floor(X + R)
    res = 0
    for i in range(start, end + 1):
        yd2 = R2I - (XI - i * D) ** 2

        ytop_t = YTOP_T
        ytop_f = YTOP_F
        while ytop_f - ytop_t > 1:
            mid = (ytop_t + ytop_f) // 2
            ym2 = (mid * D - YI) ** 2
            if ym2 > yd2:
                ytop_f = mid
            else:
                ytop_t = mid

        ybot_t = YBOT_T
        ybot_f = YBOT_F
        while ybot_t - ybot_f > 1:
            mid = (ybot_t + ybot_f) // 2
            ym2 = (YI - mid * D) ** 2
            if ym2 > yd2:
                ybot_f = mid
            else:
                ybot_t = mid

        res += ytop_t - ybot_t + 1

    return res


print(solve())
